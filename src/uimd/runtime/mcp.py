"""MCP-style control server for generated terminal UI applications."""

import json
import http.server
from http import HTTPStatus
import re
import socketserver
import sys
import threading
import time

from .elements import GradientRenderTime
from .rendering import cells_to_ansi_lines, cells_to_snapshot_rows


DEFAULT_GUI_ACTION_DELAY_MS = 500
DEFAULT_GUI_TYPE_DELAY_MS = 50
DEFAULT_HEADLESS_ACTION_DELAY_MS = 0
DEFAULT_HEADLESS_TYPE_DELAY_MS = 0
DEFAULT_TCP_HOST = "127.0.0.1"
DEFAULT_TCP_PORT = 8765
DEFAULT_GUI_TRANSPORT = "tcp"
DEFAULT_HEADLESS_TRANSPORT = "stdio"
DEFAULT_HTTP_ENDPOINT = "/mcp"
DIALOG_BUTTON_CLOSE_DELAY_MS = 180
MCP_PROTOCOL_VERSION = "2024-11-05"
JSONRPC_VERSION = "2.0"
VIEWPORT_FIELD_COUNT = 4
SUPPORTED_HTTP_PROTOCOL_VERSIONS = {MCP_PROTOCOL_VERSION, "2025-03-26", "2025-06-18"}
HTTP_JSON_CONTENT_TYPE = "application/json"
HTTP_SSE_CONTENT_TYPE = "text/event-stream"
HTTP_ACCEPTED_CONTENT_TYPES = (HTTP_JSON_CONTENT_TYPE, HTTP_SSE_CONTENT_TYPE)
TEXT_ELEMENT_TYPES = {"textinput", "textarea", "numberinput"}
LABEL_ELEMENT_TYPES = {"label", "spanlabel", "infolabel"}
INTERACTIVE_ELEMENT_TYPES = {
    "textinput",
    "textarea",
    "numberinput",
    "button",
    "checkbox",
    "combobox",
    "listbox",
}
CURSOR_DIRECTIONS = {
    "left": "Left",
    "right": "Right",
    "up": "Up",
    "down": "Down",
    "home": "Home",
    "end": "End",
}

TOOL_ALIASES = {
    "ui.get_window": "get_window",
    "ui.get_elements": "get_elements",
    "ui.get_element": "get_element",
    "ui.get_value": "get_value",
    "ui.get_options": "get_options",
    "ui.get_focused_element": "get_focused_element",
    "ui.snapshot": "get_accessibility_snapshot",
    "ui.render_snapshot": "get_render_snapshot",
    "ui.text_snapshot": "get_text_snapshot",
    "ui.get_schema": "get_schema",
    "ui.get_source": "get_source",
    "ui.set_value": "set_value",
    "ui.focus": "focus_element",
    "ui.activate": "activate_element",
    "ui.click": "click_element",
    "ui.press": "press_key",
    "ui.fill": "set_text",
    "ui.clear": "clear_text",
    "ui.select_option": "select_option",
    "ui.set_selection": "set_selection",
    "ui.set_checked": "set_checked",
    "ui.check": "check",
    "ui.uncheck": "uncheck",
    "ui.set_cursor": "set_cursor",
    "ui.select_text": "select_text",
    "ui.copy_selection": "copy_selection",
    "ui.cut_selection": "cut_selection",
}

CANONICAL_TOOL_NAMES = tuple(TOOL_ALIASES.keys())


class MCPRuntimeConfig:
    """Runtime options for the MCP control layer."""

    def __init__(
        self,
        enabled=False,
        gui=True,
        transport=None,
        host=DEFAULT_TCP_HOST,
        port=DEFAULT_TCP_PORT,
        action_delay_ms=None,
        type_delay_ms=None,
        fast=False,
        wait_render=False,
        viewport=None,
        controlled_render=False,
    ):
        self.enabled = bool(enabled)
        self.gui = bool(gui)
        self.transport = transport or (DEFAULT_GUI_TRANSPORT if gui else DEFAULT_HEADLESS_TRANSPORT)
        self.host = host
        self.port = int(port)
        default_action_delay = 0 if fast else (DEFAULT_GUI_ACTION_DELAY_MS if gui else DEFAULT_HEADLESS_ACTION_DELAY_MS)
        default_type_delay = 0 if fast else (DEFAULT_GUI_TYPE_DELAY_MS if gui else DEFAULT_HEADLESS_TYPE_DELAY_MS)
        self.action_delay_ms = default_action_delay if action_delay_ms is None else int(action_delay_ms)
        self.type_delay_ms = default_type_delay if type_delay_ms is None else int(type_delay_ms)
        self.wait_render = bool(wait_render)
        self.viewport = viewport
        self.controlled_render = bool(controlled_render)


def parse_mcp_runtime_args(argv=None):
    """Parse MCP-specific runtime switches while leaving app arguments alone."""
    argv = list(sys.argv[1:] if argv is None else argv)
    enabled = "--mcp-server" in argv
    gui = "--headless" not in argv
    if "--gui" in argv:
        gui = True

    transport = _option_value(argv, "--mcp-transport")
    host = _option_value(argv, "--mcp-host", DEFAULT_TCP_HOST)
    port = int(_option_value(argv, "--mcp-port", DEFAULT_TCP_PORT))
    action_delay = _optional_int_option(argv, "--mcp-action-delay-ms")
    type_delay = _optional_int_option(argv, "--mcp-type-delay-ms")
    viewport = _parse_viewport_arg(argv)

    return MCPRuntimeConfig(
        enabled=enabled,
        gui=gui,
        transport=transport,
        host=host,
        port=port,
        action_delay_ms=action_delay,
        type_delay_ms=type_delay,
        fast="--mcp-fast" in argv,
        wait_render="--mcp-wait-render" in argv,
        viewport=viewport,
        controlled_render="--mcp-controlled-render" in argv,
    )


def _option_value(argv, name, default=None):
    if name not in argv:
        return default
    idx = argv.index(name)
    if idx + 1 >= len(argv):
        return default
    return argv[idx + 1]


def _optional_int_option(argv, name):
    value = _option_value(argv, name)
    return None if value is None else int(value)


def _parse_viewport_arg(argv):
    value = _option_value(argv, "--viewport")
    if value:
        parts = [int(part.strip()) for part in value.split(",")]
        if len(parts) != VIEWPORT_FIELD_COUNT:
            raise ValueError("--viewport must have form row,col,width,height")
        row, col, width, height = parts
        return {"row": row, "col": col, "width": width, "height": height}

    keys = ("--viewport-row", "--viewport-col", "--viewport-width", "--viewport-height")
    if not any(key in argv for key in keys):
        return None
    return {
        "row": int(_option_value(argv, "--viewport-row", 0)),
        "col": int(_option_value(argv, "--viewport-col", 0)),
        "width": int(_option_value(argv, "--viewport-width", 1)),
        "height": int(_option_value(argv, "--viewport-height", 1)),
    }


class MCPController:
    """Backend-neutral UI inspector and driver used by the MCP server."""

    def __init__(self, app, config=None):
        self.app = app
        self.config = config or MCPRuntimeConfig()

    @property
    def window(self):
        return self.app.active_window

    def call_tool(self, name, arguments=None):
        arguments = arguments or {}
        lock = getattr(self.app, "_render_lock", None)
        if lock is not None:
            with lock:
                return self._call_tool_unlocked(name, arguments)
        return self._call_tool_unlocked(name, arguments)

    def _call_tool_unlocked(self, name, arguments):
        method_name = self._resolve_tool_name(name)
        method = getattr(self, f"tool_{method_name}", None)
        if method is not None:
            return method(**arguments)
        if method_name in self._app_tools():
            return self._call_app_tool(method_name, arguments)
        raise ValueError(f"Unknown MCP UI tool: {name}")

    def list_tools(self):
        names = sorted({
            *(name[5:] for name in dir(self) if name.startswith("tool_") and callable(getattr(self, name))),
            *CANONICAL_TOOL_NAMES,
            *self._app_tools().keys(),
        })
        return [self._tool_definition(name) for name in names]

    @staticmethod
    def _resolve_tool_name(name):
        name = str(name)
        if name in TOOL_ALIASES:
            return TOOL_ALIASES[name]
        if name.startswith("ui."):
            return TOOL_ALIASES.get(name[3:], name[3:])
        return TOOL_ALIASES.get(name, name)

    def _tool_definition(self, name):
        resolved = self._resolve_tool_name(name)
        method = getattr(self, f"tool_{resolved}", None)
        if method is None and resolved in self._app_tools():
            definition = dict(self._app_tools()[resolved])
            result = {
                "name": name,
                "description": definition.get("description", ""),
                "inputSchema": definition.get("inputSchema", {"type": "object", "properties": {}}),
            }
            if "outputSchema" in definition:
                result["outputSchema"] = definition["outputSchema"]
            return result
        description = _first_sentence(getattr(method, "__doc__", "")) if method is not None else ""
        return {
            "name": name,
            "description": description,
            "inputSchema": self._tool_input_schema(resolved),
        }

    @staticmethod
    def _tool_input_schema(name):
        properties = {}
        required = []

        def add_element_id():
            properties["element_id"] = {"type": "string"}
            required.append("element_id")

        if name in {
            "get_element", "get_value", "get_options", "focus_element", "activate_element",
            "click_element", "clear_text", "select_all", "copy_selection", "cut_selection",
            "get_text", "get_selection", "get_cursor", "check", "uncheck",
        }:
            add_element_id()
        elif name == "set_value":
            add_element_id()
            properties["value"] = {
                "oneOf": [
                    {"type": "string"},
                    {"type": "number"},
                    {"type": "boolean"},
                    {"type": "array", "items": {"type": "string"}},
                ]
            }
            required.append("value")
        elif name in {"set_text", "paste_text", "type_text", "replace_selection"}:
            add_element_id()
            properties["text"] = {"type": "string"}
            required.append("text")
        elif name == "set_checked":
            add_element_id()
            properties["checked"] = {"type": "boolean"}
            required.append("checked")
        elif name == "select_option":
            add_element_id()
            properties["value"] = {"type": "string"}
            required.append("value")
        elif name == "set_selection":
            add_element_id()
            properties["values"] = {"type": "array", "items": {"type": "string"}}
            required.append("values")
        elif name == "press_key":
            properties["key"] = {"type": "string"}
            required.append("key")
        elif name == "move_cursor":
            add_element_id()
            properties["direction"] = {"type": "string"}
            properties["count"] = {"type": "integer", "minimum": 1}
            required.append("direction")
        elif name == "set_cursor":
            add_element_id()
            properties["offset"] = {"type": "integer", "minimum": 0}
            required.append("offset")
        elif name == "select_text":
            add_element_id()
            properties["start"] = {"type": "integer", "minimum": 0}
            properties["end"] = {"type": "integer", "minimum": 0}
            required.extend(["start", "end"])
        elif name == "set_viewport":
            for field in ("row", "col", "width", "height"):
                properties[field] = {"type": "integer"}
            required.extend(["row", "col", "width", "height"])
        elif name == "get_render_frame":
            pass
        elif name == "get_render_snapshot":
            properties["snapshot_time_ms"] = {"type": "integer"}
        elif name == "get_render_snapshot_compact":
            properties["snapshot_time_ms"] = {"type": "integer"}
        elif name == "repaint":
            properties["full"] = {"type": "boolean"}
        elif name == "mouse_click":
            properties["x"] = {"type": "integer"}
            properties["y"] = {"type": "integer"}
            required.extend(["x", "y"])
        elif name == "mouse_drag":
            for field in ("from_x", "from_y", "to_x", "to_y"):
                properties[field] = {"type": "integer"}
            required.extend(["from_x", "from_y", "to_x", "to_y"])
        elif name == "scroll":
            add_element_id()
            properties["delta"] = {"type": "integer"}
            required.append("delta")

        schema = {"type": "object", "properties": properties}
        if required:
            schema["required"] = required
        return schema

    def tool_get_window(self):
        """Return active window identity, size, mode, backend, and description."""
        window = self._require_window()
        metadata = self._metadata()
        return {
            "title": getattr(window, "title", ""),
            "class": type(window).__name__,
            "backend": "python",
            "mode": getattr(window, "mode", None),
            "description": metadata.get("window", {}).get("description", ""),
            "width": getattr(window, "_window_width", None),
            "height": getattr(window, "_window_height", None),
            "mcp_enabled": self._window_mcp_enabled(window),
        }

    def tool_get_elements(self):
        """Return all elements in the active window."""
        return [
            self._element_snapshot(elem, element_id=element_id)
            for element_id, elem in self._iter_exposed_element_items()
        ]

    def tool_get_element(self, element_id):
        """Return one element by id."""
        return self._element_snapshot(self._require_element(element_id))

    def tool_get_value(self, element_id):
        """Return only the current value for one element."""
        return {"value": self._element_value(self._require_element(element_id))}

    def tool_get_options(self, element_id):
        """Return only selectable options for one selection element."""
        elem = self._require_element(element_id)
        options = self._element_options(elem)
        if options is None:
            raise ValueError(f"Element has no selectable options: {element_id}")
        return {"options": options}

    def tool_get_focused_element(self):
        """Return the currently focused element."""
        window = self._require_window()
        elem = getattr(window, "_focused_element", None)
        if elem is None or not self._element_exposed(elem):
            return None
        return self._element_snapshot(elem, element_id=self._element_id(elem))

    def tool_get_edit_mode(self):
        """Return whether the active window is in edit mode."""
        window = self._require_window()
        return {"edit_mode": bool(getattr(window, "_edit_mode", False))}

    def tool_get_render_frame(self):
        """Return current render frame as compact cell grid for tester display."""
        cells = self._render_full_viewport_cells()
        return {
            "width": len(cells[0]) if cells else 0,
            "height": len(cells),
            "cells": self._compact_cells(cells),
        }

    def _render_full_viewport_cells(self):
        """Render the full scene (all windows in stack) into a viewport-sized cell grid.
        Same as _render_to_terminal but returns cells instead of writing to stdout."""
        viewport = self.app.get_viewport()
        vp_w = max(1, int(viewport["width"]))
        vp_h = max(1, int(viewport["height"]))
        blank = {"char": " ", "foreground": None, "background": None, "attributes": []}
        cells = [[dict(blank) for _ in range(vp_w)] for _ in range(vp_h)]

        if not self.app._window_stack:
            return cells

        def blit_rows(cell_rows, window, col_off, row_off):
            raw = cells_to_snapshot_rows(cell_rows)
            for r, row_data in enumerate(raw):
                tr = row_off + r
                if tr < 0 or tr >= vp_h:
                    continue
                for c, cell in enumerate(row_data):
                    tc = col_off + c
                    if tc < 0 or tc >= vp_w:
                        continue
                    if (cell.get("char", " ") != " "
                            or cell.get("background") is not None
                            or cell.get("foreground") is not None):
                        cells[tr][tc] = cell

        last = len(self.app._window_stack) - 1
        for index, window in enumerate(self.app._window_stack):
            mode = self.app._resolved_window_mode(window)
            saved_mode = window.mode
            window.mode = mode
            self.app._resize_window(window)
            cell_rows = self.app._render_window_cells(window)
            window.mode = saved_mode

            if index < last:
                cell_rows = self.app._dim_cell_rows(cell_rows)

            if mode in ("fullscreen", "expand_width", "expand_height"):
                col_off, row_off = 0, 0
            else:
                win_w = getattr(window, "_window_width", vp_w)
                win_h = len(cell_rows)
                col_off = max(0, (vp_w - win_w) // 2)
                row_off = max(0, (vp_h - win_h) // 2)

            blit_rows(cell_rows, window, col_off, row_off)

            if index == last:
                self._overlay_open_combobox_cells(cells, col_off, row_off)

        return cells

    def tool_get_render_snapshot(self, snapshot_time_ms=None):
        """Return ANSI and plain-text render snapshots."""
        with GradientRenderTime(snapshot_time_ms):
            cell_rows = self._render_cell_rows()
        cells = self._viewport_cells_from_rows(cell_rows)
        return {
            "ansi_lines": self._cells_to_ansi_lines(cells),
            "text_lines": ["".join(cell["char"] for cell in row) for row in cells],
            "cells": cells,
        }

    def tool_get_render_snapshot_compact(self, snapshot_time_ms=None):
        """Return compact render cells for backend comparison."""
        with GradientRenderTime(snapshot_time_ms):
            cell_rows = self._render_cell_rows()
        cells = self._viewport_cells_from_rows(cell_rows)
        return {
            "format": "render-cells-v1",
            "cells": self._compact_cells(cells),
        }

    def tool_get_text_snapshot(self):
        """Return a plain-text render snapshot."""
        return {"lines": [self._strip_ansi(line) for line in self._render_lines()]}

    def tool_get_accessibility_snapshot(self):
        """Return structural state suitable for agents."""
        return {
            "window": self.tool_get_window(),
            "focused_element": self.tool_get_focused_element(),
            "edit_mode": self.tool_get_edit_mode()["edit_mode"],
            "elements": self.tool_get_elements(),
        }

    def tool_get_schema(self):
        """Return UI schema and per-element capabilities for agents."""
        window = self._require_window()
        return {
            "window": self.tool_get_window(),
            "elements": [
                self._element_schema(elem, element_id=element_id)
                for element_id, elem in self._iter_exposed_element_items()
            ],
        }

    def tool_get_source(self):
        """Return explicit debug provenance for the active generated window."""
        window = self._require_window()
        return {
            "class": window.__class__.__name__,
            "mcp_enabled": self._window_mcp_enabled(window),
            "source_markdown": getattr(window, "_mcp_source_md", ""),
        }

    def tool_get_state(self):
        """Return compact UI state."""
        return self.tool_get_accessibility_snapshot()

    def tool_get_viewport(self):
        """Return current runtime viewport."""
        return self.app.get_viewport()

    def tool_get_render_rect(self):
        """Return active window terminal render rectangle."""
        return self.app.get_render_rect()

    def tool_set_viewport(self, row, col, width, height):
        """Resize the runtime viewport."""
        self.app.set_viewport(int(row), int(col), int(width), int(height))
        self._after_action()
        return self.tool_get_viewport()

    def tool_repaint(self, full=False):
        """Force a terminal repaint."""
        self.app.mark_dirty()
        if full:
            if hasattr(self.app, "_viewport_frame_rect"):
                self.app._viewport_frame_rect = None
            if hasattr(self.app, "_viewport_frame_rows"):
                self.app._viewport_frame_rows = []
        self.app._render_to_terminal()
        self.app.clear_dirty()
        return {"ok": True}

    def tool_focus_element(self, element_id):
        """Focus an element."""
        self._action_delay()
        window = self._require_window()
        elem = self._require_element(element_id)
        window.set_focus(elem)
        self._after_action()
        return self._element_snapshot(elem)

    def tool_enter_edit_mode(self, element_id=None):
        """Enter edit mode for an editable focused element."""
        self._action_delay()
        window = self._require_window()
        if element_id is not None:
            window.set_focus(self._require_element(element_id))
        window._enter_edit_mode()
        self._after_action()
        return self.tool_get_edit_mode()

    def tool_exit_edit_mode(self):
        """Exit edit mode."""
        self._action_delay()
        window = self._require_window()
        window._exit_edit_mode(commit=True)
        self._after_action()
        return self.tool_get_edit_mode()

    def tool_click_element(self, element_id):
        """Click an element through runtime mouse behavior."""
        self._action_delay()
        elem = self._require_element(element_id)
        if not getattr(elem, "enabled", True):
            return self._element_snapshot(elem, element_id=element_id)
        rect = self._element_rect(elem)
        offset_col, offset_row = self.app._active_window_offsets()
        self.app.handle_key({
            "type": "mouse",
            "event": "press",
            "button": 0,
            "row": rect["top"] + rect["height"] // 2 + offset_row,
            "col": rect["left"] + rect["width"] // 2 + offset_col,
        })
        self._after_action()
        refreshed = self._element_by_id(element_id)
        return self._element_snapshot(refreshed or elem, element_id=element_id)

    def tool_activate_element(self, element_id):
        """Activate a control with Enter."""
        self._action_delay()
        window = self._require_window()
        elem = self._require_element(element_id)
        if not getattr(elem, "enabled", True):
            return self._element_snapshot(elem, element_id=element_id)
        background_window = self._background_window_before_modal_action()
        background_scopes = self._active_scrollview_scopes(background_window)
        self._focus_element_for_activation(window, elem)
        if self._should_show_button_focus_before_activate(elem):
            self._after_action()
            _sleep_ms(DIALOG_BUTTON_CLOSE_DELAY_MS)
        if getattr(elem, "ELEMENT_TYPE", None) == "uielement" and window._activate_element(elem):
            self._after_action()
            return {"ok": True, "element_id": element_id}
        self.app.handle_key("Enter")
        self._cleanup_background_modal_scrollview_focus(background_window, background_scopes)
        self._after_action()
        return self._element_snapshot(elem)

    def _focus_element_for_activation(self, window, elem):
        window.set_focus(elem)
        elem_type = getattr(elem, "ELEMENT_TYPE", None)
        if elem_type not in ("button", "image"):
            return None
        if not hasattr(window, "_scrollview_focus_context_for_element"):
            return None
        context = window._scrollview_focus_context_for_element(elem)
        if context is None:
            return None
        proxy = context.get("proxy")
        scrollview = context.get("scrollview")
        if proxy is None or scrollview is None:
            return None
        window._active_scrollview_scope = {
            "proxy": proxy,
            "scrollview": scrollview,
            "scrollview_rect": context.get("scrollview_rect"),
        }
        window._edit_mode = True
        window._edit_snapshot = None
        window._focused_element = elem
        elem.focused = True
        if hasattr(window, "_set_element_focus_state"):
            window._set_element_focus_state(proxy, True)
        owner = context.get("owner")
        if owner is not None and owner is not window:
            owner._focused_element = elem
        if hasattr(scrollview, "ensure_element_visible"):
            scrollview.ensure_element_visible(elem)
        if hasattr(window, "_scrollview_last_descendant"):
            window._scrollview_last_descendant[id(scrollview)] = elem
        return context

    def tool_set_checked(self, element_id, checked):
        """Set a checkbox state without toggling it blindly."""
        self._action_delay()
        elem = self._require_element(element_id)
        if getattr(elem, "ELEMENT_TYPE", None) != "checkbox":
            raise ValueError(f"Element is not a checkbox: {element_id}")
        next_value = bool(checked)
        if getattr(elem, "value", False) != next_value:
            elem.value = next_value
            self._notify_element_changed(elem)
        self._after_action()
        return self._element_snapshot(elem)

    def tool_check(self, element_id):
        """Check a checkbox."""
        return self.tool_set_checked(element_id, True)

    def tool_uncheck(self, element_id):
        """Uncheck a checkbox."""
        return self.tool_set_checked(element_id, False)

    def tool_set_value(self, element_id, value):
        """Set a control value directly in one command."""
        self._action_delay()
        elem = self._require_element(element_id)
        elem_type = getattr(elem, "ELEMENT_TYPE", None)
        if elem_type in ("textinput", "textarea"):
            elem.value = "" if value is None else str(value)
            elem.cursor_pos = len(elem.value)
            if hasattr(elem, "_selection_start"):
                elem._selection_start = None
            if hasattr(elem, "_scroll_offset"):
                elem._scroll_offset = 0
            self._notify_element_changed(elem)
        elif elem_type == "numberinput":
            elem.value = 0 if value in (None, "") else float(value)
            elem.cursor_pos = len(getattr(elem, "display_value", str(elem.value)))
            if hasattr(elem, "_selection_start"):
                elem._selection_start = None
            self._notify_element_changed(elem)
        elif elem_type == "checkbox":
            if not isinstance(value, bool):
                raise ValueError(f"Checkbox value must be boolean: {element_id}")
            if getattr(elem, "value", False) != value:
                elem.value = value
                self._notify_element_changed(elem)
        elif elem_type == "combobox":
            self._set_single_selection_value(elem, element_id, value)
        elif elem_type == "listbox":
            if isinstance(value, (list, tuple)):
                self._set_listbox_selection_values(elem, element_id, value)
            else:
                self._set_single_selection_value(elem, element_id, value)
        else:
            raise ValueError(f"Element value cannot be set directly: {element_id}")
        self._after_action()
        return self._element_snapshot(elem)

    def tool_select_option(self, element_id, value):
        """Select an option in a combobox or listbox by exact value."""
        self._action_delay()
        elem = self._require_element(element_id)
        self._set_single_selection_value(elem, element_id, value)
        self._after_action()
        return self._element_snapshot(elem)

    def _set_single_selection_value(self, elem, element_id, value):
        elem_type = getattr(elem, "ELEMENT_TYPE", None)
        options = self._element_options(elem)
        if elem_type not in ("combobox", "listbox") or options is None:
            raise ValueError(f"Element is not a supported selection control: {element_id}")
        value = str(value)
        if value not in options:
            raise ValueError(f"Unknown option for {element_id}: {value}")
        if elem_type == "combobox":
            elem.selected_item = value
            if hasattr(elem, "_menu_open"):
                elem._menu_open = False
        else:
            elem.selected_items = [value]
            if hasattr(elem, "_ensure_selection_visible"):
                elem._ensure_selection_visible(options.index(value))
        self._notify_element_changed(elem)

    def tool_set_selection(self, element_id, values):
        """Replace a listbox selection with exact option values."""
        self._action_delay()
        elem = self._require_element(element_id)
        self._set_listbox_selection_values(elem, element_id, values)
        self._after_action()
        return self._element_snapshot(elem)

    def _set_listbox_selection_values(self, elem, element_id, values):
        if getattr(elem, "ELEMENT_TYPE", None) != "listbox":
            raise ValueError(f"Element is not a listbox: {element_id}")
        options = self._element_options(elem) or []
        values = [str(value) for value in (values or [])]
        if not getattr(elem, "multiple", False) and len(values) > 1:
            raise ValueError(f"ListBox is not multi-select: {element_id}")
        unknown = [value for value in values if value not in options]
        if unknown:
            raise ValueError(f"Unknown option for {element_id}: {unknown[0]}")
        elem.selected_items = [option for option in options if option in values]
        if elem.selected_items and hasattr(elem, "_ensure_selection_visible"):
            elem._ensure_selection_visible(options.index(elem.selected_items[-1]))
        self._notify_element_changed(elem)

    def tool_press_key(self, key):
        """Send one key to the runtime."""
        self._action_delay()
        self.app.handle_key(key)
        self._after_action()
        return self.tool_get_state()

    def tool_mouse_click(self, x, y):
        """Send a mouse click at terminal/window coordinates."""
        self._action_delay()
        row = int(y)
        col = int(x)
        self.app.handle_key({
            "type": "mouse",
            "event": "press",
            "button": 0,
            "row": row,
            "col": col,
        })
        self.app.handle_key({
            "type": "mouse",
            "event": "release",
            "button": 0,
            "row": row,
            "col": col,
        })
        self._after_action()
        return self.tool_get_state()

    def tool_mouse_drag(self, from_x, from_y, to_x, to_y):
        """Send a mouse drag."""
        self._action_delay()
        self.app.handle_key({
            "type": "mouse",
            "event": "press",
            "button": 0,
            "row": int(from_y),
            "col": int(from_x),
        })
        self.app.handle_key({
            "type": "mouse",
            "event": "drag",
            "button": 0,
            "row": int(to_y),
            "col": int(to_x),
        })
        self.app.handle_key({
            "type": "mouse",
            "event": "release",
            "button": 0,
            "row": int(to_y),
            "col": int(to_x),
        })
        self._after_action()
        return self.tool_get_state()

    def tool_scroll(self, element_id, delta):
        """Scroll a scrollable element."""
        self._action_delay()
        elem = self._require_element(element_id)
        target = self._scroll_target(elem)
        line_delta = int(delta)
        if hasattr(target, "scroll_lines"):
            target.scroll_lines(line_delta)
        else:
            direction = "down" if line_delta < 0 else "up"
            for _ in range(max(1, abs(line_delta))):
                target.handle_key({"type": "mouse_wheel", "direction": direction})
        consume_scroll_delta = getattr(target, "consume_terminal_scroll_delta", None)
        if callable(consume_scroll_delta):
            consume_scroll_delta()
        if hasattr(self.app, "request_full_redraw"):
            self.app.request_full_redraw()
        self._after_action()
        return self._element_snapshot(elem)

    @staticmethod
    def _scroll_target(elem):
        child = getattr(elem, "_child_instance", None)
        if child is None and hasattr(elem, "_ensure_child"):
            child = elem._ensure_child()
        if child is not None and hasattr(child, "scroll_lines"):
            return child
        return elem

    def tool_type_text(self, element_id, text):
        """Type text at the current cursor position, one character at a time."""
        elem = self._prepare_text_element(element_id)
        self._type_text_into_element(elem, text)
        self._after_action()
        return self._element_snapshot(elem)

    def tool_set_text(self, element_id, text):
        """Replace existing text by clearing it and typing characters."""
        elem = self._prepare_text_element(element_id)
        self._set_force_scroll_start(elem, True)
        try:
            self._clear_text_for_typing(elem)
            self._mark_text_progress()
            self._type_text_into_element(elem, text)
            if self._text_value(elem) != str(text):
                self._set_text_value(elem, text)
            self._after_action()
            return self._element_snapshot(elem)
        finally:
            self._set_force_scroll_start(elem, False)

    def tool_paste_text(self, element_id, text):
        """Paste text at cursor or over the current selection as one action."""
        self._action_delay()
        elem = self._prepare_text_element(element_id, delay=False)
        self.app.handle_key({"type": "paste", "text": str(text)})
        self._after_action()
        return self._element_snapshot(elem)

    def tool_select_text(self, element_id, start, end):
        """Select a text range."""
        self._action_delay()
        elem = self._prepare_text_element(element_id, delay=False)
        elem.select_range(int(start), int(end))
        self._after_action()
        return self._selection_snapshot(elem)

    def tool_select_all(self, element_id):
        """Select all text."""
        self._action_delay()
        elem = self._prepare_text_element(element_id, delay=False)
        self._select_all_text(elem)
        self._after_action()
        return self._selection_snapshot(elem)

    def tool_replace_selection(self, element_id, text):
        """Replace current selection by typing characters."""
        elem = self._prepare_text_element(element_id)
        self._type_text_into_element(elem, text)
        self._after_action()
        return self._element_snapshot(elem)

    def tool_copy_selection(self, element_id):
        """Return selected text and copy it without showing transient UI."""
        self._action_delay()
        elem = self._require_element(element_id)
        text = self._selected_text(elem)
        if text:
            from .elements import _copy_text_to_clipboard
            _copy_text_to_clipboard(text)
        self._after_action()
        return {"text": text}

    def tool_cut_selection(self, element_id):
        """Return selected text and remove it."""
        self._action_delay()
        elem = self._prepare_text_element(element_id, delay=False)
        text = self._selected_text(elem)
        if text:
            if getattr(elem, "ELEMENT_TYPE", None) == "numberinput":
                elem._replace_selection("")
            else:
                elem._delete_selection()
        self._after_action()
        return {"text": text}

    def tool_clear_text(self, element_id):
        """Clear a text element."""
        elem = self._prepare_text_element(element_id)
        self._select_all_text(elem)
        self._action_delay()
        self.app.handle_key("Backspace")
        self._after_action()
        return self._element_snapshot(elem)

    def tool_move_cursor(self, element_id, direction, count=1):
        """Move text cursor."""
        self._action_delay()
        elem = self._prepare_text_element(element_id, delay=False)
        key = CURSOR_DIRECTIONS.get(str(direction).lower())
        if key is None:
            raise ValueError(f"Unsupported cursor direction: {direction}")
        for _ in range(max(1, int(count))):
            self.app.handle_key(key)
        self._after_action()
        return self._cursor_snapshot(elem)

    def tool_set_cursor(self, element_id, offset):
        """Set cursor offset."""
        self._action_delay()
        elem = self._prepare_text_element(element_id, delay=False)
        length = len(self._text_value(elem))
        elem.cursor_pos = max(0, min(int(offset), length))
        if hasattr(elem, "_selection_start"):
            elem._selection_start = None
        if hasattr(elem, "_manual_scroll_away_from_cursor"):
            elem._manual_scroll_away_from_cursor = False
        self._after_action()
        return self._cursor_snapshot(elem)

    def tool_get_text(self, element_id):
        """Return text value."""
        elem = self._require_element(element_id)
        return {"text": self._text_value(elem)}

    def tool_get_selection(self, element_id):
        """Return current text selection."""
        return self._selection_snapshot(self._require_element(element_id))

    def tool_get_cursor(self, element_id):
        """Return current cursor position."""
        return self._cursor_snapshot(self._require_element(element_id))

    def _send_text_key(self, char):
        if char == "\n":
            self.app.handle_key("Enter")
        else:
            self.app.handle_key(char)

    def _type_text_into_element(self, elem, text):
        for char in str(text):
            self._type_delay()
            self._insert_text_char(elem, char)
            self._mark_text_progress()

    def _insert_text_char(self, elem, char):
        if getattr(elem, "ELEMENT_TYPE", None) == "numberinput":
            has_selection = (
                getattr(elem, "_selection_start", None) is not None
                and getattr(elem, "_selection_start", None) != getattr(elem, "cursor_pos", None)
            )
            if has_selection:
                elem._replace_selection(char)
            else:
                elem._insert_text(char)
            self._notify_element_changed(elem)
            self._clear_unrelated_scrollview_scope_for_element(elem)
            return
        if hasattr(elem, "_insert_text"):
            elem._insert_text(char)
            self._notify_element_changed(elem)
            self._clear_unrelated_scrollview_scope_for_element(elem)
            return
        self._send_text_key(char)

    def _set_text_value(self, elem, text):
        text = str(text)
        if getattr(elem, "ELEMENT_TYPE", None) == "numberinput":
            elem._set_from_text(text)
            elem.cursor_pos = len(getattr(elem, "display_value", ""))
        else:
            elem.value = text
            elem.cursor_pos = len(getattr(elem, "value", ""))
        if hasattr(elem, "_selection_start"):
            elem._selection_start = None
        self._notify_element_changed(elem)
        self._clear_unrelated_scrollview_scope_for_element(elem)
        self._mark_text_progress()

    def _clear_text_for_typing(self, elem):
        if getattr(elem, "ELEMENT_TYPE", None) == "numberinput":
            elem._set_from_text("")
        else:
            elem.value = ""
        elem.cursor_pos = 0
        if hasattr(elem, "_selection_start"):
            elem._selection_start = None
        if hasattr(elem, "_scroll_offset"):
            elem._scroll_offset = 0
        self._notify_element_changed(elem)
        self._clear_unrelated_scrollview_scope_for_element(elem)

    def _notify_element_changed(self, elem):
        window = self._require_window()
        value = self._element_value(elem)
        if getattr(elem, "ELEMENT_TYPE", None) in ("combobox", "listbox"):
            if hasattr(window, "_dispatch_selection_changed_for"):
                window._dispatch_selection_changed_for(elem, value)
                return
            if hasattr(window, "_dispatch_selection_changed"):
                window._dispatch_selection_changed(elem, value)
            elif hasattr(window, "selectionchanged"):
                window.selectionchanged(elem, value)
            return
        if hasattr(window, "_dispatch_element_changed_for"):
            window._dispatch_element_changed_for(elem, value)
            return
        if hasattr(window, "_dispatch_element_changed"):
            window._dispatch_element_changed(elem, value)
        elif hasattr(window, "elementchanged"):
            window.elementchanged(elem, value)

    def _background_window_before_modal_action(self):
        stack = getattr(self.app, "_window_stack", [])
        if len(stack) < 2:
            return None
        return stack[-2]

    def _active_scrollview_scopes(self, window):
        if window is None:
            return []
        scopes = []
        seen = set()

        def visit(view):
            if view is None or id(view) in seen:
                return
            seen.add(id(view))
            scope = getattr(view, "_active_scrollview_scope", None)
            if scope is not None:
                focusable_index = None
                focused = getattr(view, "_focused_element", None)
                if hasattr(view, "get_focusable_elements") and focused is not None:
                    try:
                        focusable_index = view.get_focusable_elements().index(focused)
                    except ValueError:
                        focusable_index = None
                scopes.append({
                    "owner": view,
                    "proxy": scope.get("proxy"),
                    "scrollview": scope.get("scrollview"),
                    "scrollview_rect": scope.get("scrollview_rect"),
                    "focused": focused,
                    "focused_index": focusable_index,
                })
            for element in getattr(view, "_elements", {}).values():
                current_view = element.current_view() if hasattr(element, "current_view") else None
                visit(current_view)
                visit(getattr(element, "_child_instance", None))

        visit(window)
        return scopes

    def _cleanup_background_modal_scrollview_focus(self, background_window, scopes):
        if background_window is None or self.app.active_window is not background_window:
            return
        pending_scopes = []
        seen = set()
        for scope in list(scopes) + self._active_scrollview_scopes(background_window):
            key = (id(scope.get("owner")), id(scope.get("scrollview")))
            if key in seen:
                continue
            seen.add(key)
            pending_scopes.append(scope)
        for scope in pending_scopes:
            owner = scope.get("owner")
            proxy = scope.get("proxy")
            scrollview = scope.get("scrollview")
            if owner is None or proxy is None or scrollview is None:
                continue
            if not self._scrollview_scope_focused_element_present(owner, scope):
                if getattr(scrollview, "_pending_proxy_focus_restore", False):
                    if hasattr(owner, "_clear_descendant_focus_state"):
                        owner._clear_descendant_focus_state(scrollview)
                    owner._active_scrollview_scope = None
                    owner._edit_mode = False
                    owner._edit_snapshot = None
                    owner._focused_element = proxy
                    if hasattr(owner, "_set_element_focus_state"):
                        owner._set_element_focus_state(proxy, True)
                    if hasattr(scrollview, "_focused"):
                        scrollview._focused = True
                    parent_owner = getattr(proxy, "parent", None)
                    while parent_owner is not None:
                        if hasattr(parent_owner, "_focused_element"):
                            parent_owner._focused_element = proxy
                        parent_owner = getattr(parent_owner, "parent", None)
                    clear_pending = getattr(scrollview, "_clear_pending_proxy_focus_restore", None)
                    if callable(clear_pending):
                        clear_pending()
                    continue
                self._clear_removed_scrollview_scope(owner, proxy, scrollview)
                continue
            focused = getattr(owner, "_focused_element", None)
            if focused is not None and focused is not proxy and hasattr(focused, "focused"):
                focused.focused = False
            if hasattr(owner, "_clear_descendant_focus_state"):
                owner._clear_descendant_focus_state(scrollview)
            owner._active_scrollview_scope = None
            owner._edit_mode = False
            owner._edit_snapshot = None
            owner._focused_element = proxy
            if hasattr(owner, "_set_element_focus_state"):
                owner._set_element_focus_state(proxy, True)
            if hasattr(scrollview, "_focused"):
                scrollview._focused = True
            key = id(scrollview)
            previous = scope.get("focused")
            if previous is not None and previous is not proxy:
                owner._scrollview_last_descendant[key] = previous

    def _scrollview_scope_focused_element_present(self, owner, scope):
        focused = scope.get("focused")
        if focused is None:
            return True
        proxy = scope.get("proxy")
        scrollview = scope.get("scrollview")
        if focused is proxy:
            return focused in getattr(owner, "_elements", {}).values()
        if owner is None or proxy is None or scrollview is None:
            return False
        if not hasattr(owner, "_scrollview_child_focus_contexts"):
            return False
        origin_rect = scope.get("scrollview_rect")
        if origin_rect is None:
            if hasattr(owner, "_scrollview_proxy_rect"):
                origin_rect = owner._scrollview_proxy_rect(proxy)
            else:
                return False
        contexts = owner._scrollview_child_focus_contexts(
            proxy,
            scrollview,
            origin_rect,
            visible_only=False,
        )
        if any(context.get("element") is focused for context in contexts):
            return True
        return self._scope_navigation_focus_fallback_present(owner, scope)

    def _scope_navigation_focus_fallback_present(self, owner, scope):
        focused_index = scope.get("focused_index")
        if focused_index is None or not hasattr(owner, "get_focusable_elements"):
            return False
        previous_scope = getattr(owner, "_active_scrollview_scope", None)
        try:
            owner._active_scrollview_scope = None
            focusable = owner.get_focusable_elements()
        finally:
            owner._active_scrollview_scope = previous_scope
        return 0 <= focused_index < len(focusable)

    def _clear_removed_scrollview_scope(self, owner, proxy, scrollview):
        focused = getattr(owner, "_focused_element", None)
        if focused is not None and hasattr(focused, "focused"):
            focused.focused = False
        if hasattr(owner, "_clear_descendant_focus_state"):
            owner._clear_descendant_focus_state(scrollview)
        if hasattr(proxy, "focused"):
            proxy.focused = False
        if hasattr(scrollview, "_focused"):
            scrollview._focused = False
        owner._active_scrollview_scope = None
        owner._edit_mode = False
        owner._edit_snapshot = None
        owner._focused_element = None
        key = id(scrollview)
        if hasattr(owner, "_scrollview_last_descendant"):
            owner._scrollview_last_descendant.pop(key, None)

    def _set_force_scroll_start(self, elem, enabled):
        if getattr(elem, "ELEMENT_TYPE", None) in ("textinput",):
            elem._force_scroll_start = bool(enabled)

    def _mark_text_progress(self):
        self.app.mark_dirty()
        if self.config.wait_render and self.config.gui and sys.stdout.isatty():
            self.app._render_to_terminal()
            self.app.clear_dirty()
        time.sleep(0)

    def _prepare_text_element(self, element_id, delay=True):
        if delay:
            self._action_delay()
        window = self._require_window()
        elem = self._require_element(element_id)
        if getattr(elem, "ELEMENT_TYPE", None) not in TEXT_ELEMENT_TYPES:
            raise ValueError(f"Element is not editable text: {element_id}")
        if getattr(window, "_focused_element", None) is not elem:
            window.set_focus(elem)
        self._clear_unrelated_scrollview_scope_for_element(elem)
        if not getattr(window, "_edit_mode", False):
            window._enter_edit_mode()
        self._mark_text_progress()
        return elem

    def _clear_unrelated_scrollview_scope_for_element(self, elem):
        window = self._require_window()
        scope = getattr(window, "_active_scrollview_scope", None)
        if scope is None:
            return
        context = (
            window._scrollview_focus_context_for_element(elem)
            if hasattr(window, "_scrollview_focus_context_for_element")
            else None
        )
        if (
            context is not None
            and context.get("proxy") is scope.get("proxy")
            and context.get("scrollview") is scope.get("scrollview")
        ):
            return

        scrollview = scope.get("scrollview")
        proxy = scope.get("proxy")
        if scrollview is not None and hasattr(window, "_clear_descendant_focus_state"):
            window._clear_descendant_focus_state(scrollview)
        clear_selected = getattr(scrollview, "_clear_selected_focus_child", None)
        if callable(clear_selected):
            clear_selected()
        if scrollview is not None and hasattr(scrollview, "_focused"):
            scrollview._focused = False
        if proxy is not elem and hasattr(proxy, "focused"):
            proxy.focused = False
        window._active_scrollview_scope = None
        window._focused_element = elem
        if hasattr(window, "_set_element_focus_state"):
            window._set_element_focus_state(elem, True)
        owner = window._event_owner_for_element(elem) if hasattr(window, "_event_owner_for_element") else window
        if owner is not window and hasattr(owner, "_focused_element"):
            owner._focused_element = elem

    def _select_all_text(self, elem):
        elem.select_range(0, len(self._text_value(elem)))
        self._mark_text_progress()

    def _selected_text(self, elem):
        if hasattr(elem, "_selected_text"):
            return elem._selected_text()
        if getattr(elem, "ELEMENT_TYPE", None) in LABEL_ELEMENT_TYPES:
            return elem.selected_text()
        return ""

    def _text_value(self, elem):
        if getattr(elem, "ELEMENT_TYPE", None) == "numberinput":
            return str(getattr(elem, "display_value", ""))
        return str(getattr(elem, "value", getattr(elem, "text", "")) or "")

    def _selection_snapshot(self, elem):
        selection_start = getattr(elem, "_selection_start", None)
        cursor_pos = getattr(elem, "cursor_pos", None)
        if selection_start is None or cursor_pos is None:
            return {"start": None, "end": None, "text": ""}
        start = min(selection_start, cursor_pos)
        end = max(selection_start, cursor_pos)
        if start == end:
            return {"start": start, "end": end, "text": ""}
        return {"start": start, "end": end, "text": self._text_value(elem)[start:end]}

    def _cursor_snapshot(self, elem):
        return {"cursor": getattr(elem, "cursor_pos", None)}

    def _element_snapshot(self, elem, element_id=None):
        rect = self._element_rect(elem)
        metadata = self._metadata_for_element(elem).get("elements", {}).get(getattr(elem, "name", ""), {})
        snapshot = {
            "id": element_id or self._element_id(elem),
            "type": getattr(elem, "ELEMENT_TYPE", type(elem).__name__),
            "role": metadata.get("role", self._default_role(elem)),
            "description": metadata.get("description", ""),
            "focused": bool(getattr(elem, "focused", False)),
            "visible": True,
            "enabled": bool(getattr(elem, "enabled", True)),
            "bounds": rect,
            "style_state": self._style_state(elem),
        }
        value = self._element_value(elem)
        if value is not None:
            snapshot["value"] = value
        options = self._element_options(elem)
        if options is not None:
            snapshot["options"] = options
        if hasattr(elem, "cursor_pos"):
            snapshot["cursor"] = getattr(elem, "cursor_pos", None)
            snapshot["selection"] = self._selection_snapshot(elem)
        return snapshot

    def _element_exposed(self, elem):
        metadata = self._metadata_for_element(elem).get("elements", {}).get(getattr(elem, "name", ""), {})
        return bool(metadata.get("expose", True))

    def _element_schema(self, elem, element_id=None):
        schema = self._element_snapshot(elem, element_id=element_id)
        schema["capabilities"] = self._element_capabilities(elem)
        return schema

    def _element_value(self, elem):
        elem_type = getattr(elem, "ELEMENT_TYPE", None)
        if elem_type in ("textinput", "textarea"):
            return getattr(elem, "value", "")
        if elem_type == "numberinput":
            return getattr(elem, "value", None)
        if elem_type == "checkbox":
            return bool(getattr(elem, "value", False))
        if elem_type == "combobox":
            return getattr(elem, "selected_item", None)
        if elem_type == "listbox":
            return list(getattr(elem, "selected_items", []) or [])
        if elem_type in LABEL_ELEMENT_TYPES:
            return getattr(elem, "text", "")
        if elem_type == "button":
            return getattr(elem, "title", "")
        return getattr(elem, "value", None)

    @staticmethod
    def _element_options(elem):
        if getattr(elem, "ELEMENT_TYPE", None) in ("combobox", "listbox"):
            return list(getattr(elem, "_options", []) or [])
        return None

    def _element_capabilities(self, elem):
        elem_type = getattr(elem, "ELEMENT_TYPE", None)
        capabilities = ["ui.get_element"]
        if self._element_value(elem) is not None:
            capabilities.append("ui.get_value")
        if elem_type in ("textinput", "textarea", "numberinput"):
            capabilities.extend(["ui.set_value", "ui.fill", "ui.clear", "ui.focus"])
        elif elem_type == "button":
            capabilities.extend(["ui.activate", "ui.click", "ui.focus"])
        elif elem_type == "checkbox":
            capabilities.extend([
                "ui.set_value", "ui.check", "ui.uncheck", "ui.set_checked",
                "ui.activate", "ui.click", "ui.focus",
            ])
        elif elem_type == "combobox":
            capabilities.extend(["ui.set_value", "ui.select_option", "ui.get_options", "ui.focus", "ui.press"])
        elif elem_type == "listbox":
            capabilities.extend(["ui.set_value", "ui.select_option", "ui.set_selection", "ui.get_options", "ui.focus", "ui.press"])
        return capabilities

    def _style_state(self, elem):
        window = self._require_window()
        if getattr(window, "_edit_mode", False) and getattr(window, "_focused_element", None) is elem:
            return "edit"
        if getattr(window, "_focused_element", None) is elem:
            return "focus"
        return "base"

    def _element_rect(self, elem):
        window = self._require_window()
        rect = window._element_focus_rect(elem) if hasattr(window, "_element_focus_rect") else None
        if rect is None:
            top = int(getattr(elem, "_cell_row", 0) + getattr(elem, "row", 0))
            left = int(getattr(elem, "_cell_col", 0) + getattr(elem, "col", 0))
            width = max(1, int(getattr(elem, "width", None) or 1))
            height = max(1, int(getattr(elem, "height", None) or 1))
        else:
            top = int(rect["top"])
            left = int(rect["left"])
            width = max(1, int(rect["right"] - rect["left"]))
            height = max(1, int(rect["bottom"] - rect["top"]))
        return {
            "top": top,
            "left": left,
            "bottom": top + height,
            "right": left + width,
            "width": width,
            "height": height,
        }

    def _default_role(self, elem):
        elem_type = getattr(elem, "ELEMENT_TYPE", "")
        if elem_type in ("textinput", "textarea", "numberinput"):
            return "input"
        if elem_type == "button":
            return "action"
        if elem_type == "checkbox":
            return "toggle"
        if elem_type in ("combobox", "listbox"):
            return "selection"
        return "text"

    def _require_window(self):
        if self.window is None:
            raise ValueError("No active UI window")
        return self.window

    def _require_element(self, element_id):
        elem = self._element_by_id(element_id)
        if elem is None:
            raise ValueError(f"Unknown element: {element_id}")
        return elem

    def _metadata(self):
        window = self._require_window()
        if not hasattr(window, "_mcp_agent_metadata"):
            source = getattr(window, "_mcp_source_md", "")
            window._mcp_agent_metadata = parse_agent_metadata(source)
        return window._mcp_agent_metadata

    def _app_tools(self):
        window = self.app.active_window if self.app is not None else None
        if window is None:
            return {}
        if not hasattr(window, "_mcp_app_tools"):
            source = getattr(window, "_mcp_source_md", "")
            window._mcp_app_tools = parse_app_tools(source)
        return getattr(window, "_mcp_app_tools", {}) or {}

    def _call_app_tool(self, name, arguments):
        window = self._require_window()
        method = getattr(window, name, None)
        if method is None or not callable(method):
            raise ValueError(f"App MCP tool is not implemented: {name}")
        result = method(arguments)
        return {} if result is None else result

    def _metadata_for_element(self, elem):
        window = self._require_window()
        if hasattr(window, "_event_owner_for_element"):
            owner = window._event_owner_for_element(elem)
            if owner is not window and hasattr(owner, "_mcp_agent_metadata"):
                return owner._mcp_agent_metadata
        return self._metadata()

    def _iter_element_items(self):
        window = self._require_window()
        for elem in window.get_all_elements():
            yield getattr(elem, "name", ""), elem
        if not hasattr(window, "_embedded_scrollview_contexts"):
            return
        for proxy, scrollview, origin_rect in window._embedded_scrollview_contexts():
            proxy_name = getattr(proxy, "name", "")
            for context in window._scrollview_child_focus_contexts(proxy, scrollview, origin_rect, focusable_only=False):
                child_index = context.get("child_index", 0)
                elem = context["element"]
                yield f"{proxy_name}[{child_index}].{getattr(elem, 'name', '')}", elem
        if not hasattr(window, "_embedded_view_contexts"):
            return
        seen = set()
        for context in window._embedded_view_contexts(focusable_only=False):
            elem = context["element"]
            key = id(elem)
            if key in seen:
                continue
            seen.add(key)
            proxy_name = getattr(context["proxy"], "name", "")
            elem_name = getattr(elem, "name", "")
            element_id = f"{proxy_name}.{elem_name}"
            yield element_id, elem
            scrollview = getattr(elem, "_child_instance", None)
            if scrollview is not None and hasattr(scrollview, "child_view_entries") and hasattr(scrollview, "_clamped_viewport_rect"):
                for child_context in window._scrollview_child_focus_contexts(
                    elem,
                    scrollview,
                    context["rect"],
                    focusable_only=False,
                ):
                    child_index = child_context.get("child_index", 0)
                    child_elem = child_context["element"]
                    yield f"{element_id}[{child_index}].{getattr(child_elem, 'name', '')}", child_elem

    def _iter_exposed_element_items(self):
        for element_id, elem in self._iter_element_items():
            if self._element_exposed(elem):
                yield element_id, elem

    def _element_by_id(self, element_id):
        element_id = str(element_id)
        for candidate_id, elem in self._iter_element_items():
            if candidate_id == element_id:
                return elem
        window = self._require_window()
        return window.get_element(element_id)

    def _element_id(self, elem):
        for element_id, candidate in self._iter_element_items():
            if candidate is elem:
                return element_id
        return getattr(elem, "name", "")

    def _window_mcp_enabled(self, window):
        return bool(getattr(window, "_mcp_enabled", True))

    def _render_lines(self):
        self.app._sync_active_window_size()
        return self.app.render()

    def _render_cell_rows(self):
        self.app._sync_active_window_size()
        return self.app.render_cells()

    def _viewport_cells_from_rows(self, cell_rows):
        viewport = self.app.get_viewport()
        width = max(1, int(viewport["width"]))
        height = max(1, int(viewport["height"]))
        window = self._require_window()
        rendered_cells = cells_to_snapshot_rows(cell_rows)
        base_cell = {
            "char": " ",
            "foreground": None,
            "background": None,
            "attributes": [],
        }
        cells = [[dict(base_cell) for _col in range(width)] for _row in range(height)]
        if not cell_rows:
            return cells

        mode = self.app._resolved_window_mode(window)
        if mode == "fullscreen":
            col_offset = 0
            row_offset = 0
        else:
            term_width, term_height = self.app._get_terminal_size()
            window_width = getattr(window, "_window_width", 0)
            window_height = len(cell_rows)
            col_offset = 0 if mode == "expand_width" else max(0, (term_width - window_width) // 2)
            row_offset = 0 if mode == "expand_height" else max(0, (term_height - window_height) // 2)

        for source_row, row_cells in enumerate(rendered_cells):
            target_row = row_offset + source_row
            if target_row < 0 or target_row >= height:
                continue
            for source_col, cell in enumerate(row_cells):
                target_col = col_offset + source_col
                if target_col < 0 or target_col >= width:
                    continue
                cells[target_row][target_col] = cell
        self._overlay_open_combobox_cells(cells, col_offset, row_offset)
        return cells

    def _viewport_cells(self, lines):
        viewport = self.app.get_viewport()
        width = max(1, int(viewport["width"]))
        height = max(1, int(viewport["height"]))
        window = self._require_window()
        rendered_cells = self._styled_render_cells(lines, window)
        base_cell = {
            "char": " ",
            "foreground": None,
            "background": None,
            "attributes": [],
        }
        cells = [[dict(base_cell) for _col in range(width)] for _row in range(height)]
        if not lines:
            return cells

        mode = self.app._resolved_window_mode(window)
        if mode == "fullscreen":
            col_offset = 0
            row_offset = 0
        else:
            term_width, term_height = self.app._get_terminal_size()
            window_width = getattr(window, "_window_width", 0)
            window_height = len(lines)
            col_offset = 0 if mode == "expand_width" else max(0, (term_width - window_width) // 2)
            row_offset = 0 if mode == "expand_height" else max(0, (term_height - window_height) // 2)

        for source_row, line in enumerate(lines):
            target_row = row_offset + source_row
            if target_row < 0 or target_row >= height:
                continue
            row_cells = rendered_cells[source_row] if source_row < len(rendered_cells) else self._render_cells(line)
            for source_col, cell in enumerate(row_cells):
                target_col = col_offset + source_col
                if target_col < 0 or target_col >= width:
                    continue
                cells[target_row][target_col] = cell
        self._overlay_open_combobox_cells(cells, col_offset, row_offset)
        return cells

    def _styled_render_cells(self, lines, window):
        styles = getattr(window, "styles", None)
        if not styles:
            return [self._render_cells(line) for line in lines]

        rows = []
        for row_index, line in enumerate(lines):
            parsed = self._render_cells(line)
            row_styles = styles[row_index] if row_index < len(styles) else []
            if not row_styles:
                rows.append(parsed)
                continue
            rows.append(self._apply_row_styles(parsed, row_styles))
        return rows

    def _apply_row_styles(self, parsed_cells, row_styles):
        cells = []
        style_index = 0
        for col, parsed_cell in enumerate(parsed_cells):
            while style_index + 1 < len(row_styles) and int(row_styles[style_index + 1].get("char_pos", 0)) <= col:
                style_index += 1
            style = row_styles[style_index] if row_styles else {}
            cells.append({
                "char": parsed_cell.get("char", " "),
                "foreground": self._snapshot_style_color(style.get("color")),
                "background": self._snapshot_style_color(style.get("background")),
                "attributes": parsed_cell.get("attributes", []),
            })
        return cells

    def _overlay_open_combobox_cells(self, cells, col_offset, row_offset):
        window = self._require_window()
        if not getattr(window, "_edit_mode", False):
            return
        elem = getattr(window, "_focused_element", None)
        if elem is None or getattr(elem, "ELEMENT_TYPE", None) != "combobox":
            return
        if not getattr(elem, "_menu_open", False):
            return

        rect = window._element_focus_rect(elem)
        if hasattr(elem, "render_cells"):
            rendered_cells = cells_to_snapshot_rows(elem.render_cells())
        else:
            rendered_cells = [self._render_cells(line) for line in elem.render()]
        if not rendered_cells:
            return

        top = row_offset + int(rect["top"])
        left = col_offset + int(rect["left"])
        height = len(cells)
        width = len(cells[0]) if cells else 0

        for source_row, row_cells in enumerate(rendered_cells):
            target_row = top + source_row
            if target_row < 0 or target_row >= height:
                continue
            for source_col, cell in enumerate(row_cells):
                target_col = left + source_col
                if target_col < 0 or target_col >= width:
                    continue
                cells[target_row][target_col] = cell

    def _render_cells(self, line):
        cells = []
        foreground = None
        background = None
        i = 0
        while i < len(line):
            ch = line[i]
            if ch == "\x1b" and i + 1 < len(line) and line[i + 1] == "[":
                end = i + 2
                while end < len(line) and not (0x40 <= ord(line[end]) <= 0x7E):
                    end += 1
                if end < len(line) and line[end] == "m":
                    foreground, background = self._apply_sgr(line[i + 2:end], foreground, background)
                i = end + 1
                continue
            cells.append({
                "char": ch,
                "foreground": foreground,
                "background": background,
                "attributes": [],
            })
            i += 1
        return cells

    def _cells_to_ansi_lines(self, rows):
        return ["".join(self._ansi_for_cell(cell) for cell in row) + "\x1b[0m" for row in rows]

    def _compact_cells(self, rows):
        return [
            [
                [
                    cell.get("char", " "),
                    cell.get("foreground"),
                    cell.get("background"),
                    cell.get("attributes", []),
                ]
                for cell in row
            ]
            for row in rows
        ]

    def _ansi_for_cell(self, cell):
        codes = []
        foreground = cell.get("foreground")
        background = cell.get("background")
        if foreground is not None:
            codes.append(self._ansi_color_code(foreground, "38"))
        if background is not None:
            codes.append(self._ansi_color_code(background, "48"))
        prefix = "\x1b[" + ";".join(codes) + "m" if codes else ""
        return prefix + str(cell.get("char", " "))

    @staticmethod
    def _ansi_color_code(color, prefix):
        if isinstance(color, str) and color.startswith("#") and len(color) == 7:
            return f"{prefix};2;{int(color[1:3], 16)};{int(color[3:5], 16)};{int(color[5:7], 16)}"
        return "39" if prefix == "38" else "49"

    def _apply_sgr(self, raw, foreground, background):
        if raw == "":
            return None, None
        codes = []
        for part in raw.split(";"):
            try:
                codes.append(int(part))
            except ValueError:
                pass
        if not codes:
            return foreground, background
        index = 0
        while index < len(codes):
            code = codes[index]
            if code == 0:
                foreground = None
                background = None
            elif code == 39:
                foreground = None
            elif code == 49:
                background = None
            elif code == 38 and index + 4 < len(codes) and codes[index + 1] == 2:
                foreground = self._snapshot_color(codes[index + 2], codes[index + 3], codes[index + 4])
                index += 4
            elif code == 48 and index + 4 < len(codes) and codes[index + 1] == 2:
                background = self._snapshot_color(codes[index + 2], codes[index + 3], codes[index + 4])
                index += 4
            index += 1
        return foreground, background

    @staticmethod
    def _snapshot_color(red, green, blue):
        return f"#{int(red):02x}{int(green):02x}{int(blue):02x}"

    @staticmethod
    def _snapshot_style_color(color):
        if color is None:
            return None
        if isinstance(color, str):
            if color == "transparent":
                return None
            if color.startswith("#"):
                return color
            if len(color) == 9 and color.isdigit():
                return (
                    f"#{int(color[0:3]):02x}"
                    f"{int(color[3:6]):02x}"
                    f"{int(color[6:9]):02x}"
                )
            return color
        red = getattr(color, "r", -1)
        green = getattr(color, "g", -1)
        blue = getattr(color, "b", -1)
        alpha = getattr(color, "alpha", 1.0)
        if red < 0 or green < 0 or blue < 0 or alpha <= 0:
            return None
        return f"#{int(red):02x}{int(green):02x}{int(blue):02x}"

    def _after_action(self):
        self.app.mark_dirty()
        if self.config.wait_render:
            if self.config.gui and sys.stdout.isatty():
                self.app._render_to_terminal()
            else:
                self._render_lines()
            self.app.clear_dirty()

    def _action_delay(self):
        _sleep_ms(self.config.action_delay_ms)

    def _should_show_button_focus_before_activate(self, elem):
        if getattr(elem, "ELEMENT_TYPE", None) != "button":
            return False
        stack = getattr(self.app, "_window_stack", [])
        return len(stack) > 1 or getattr(self.window, "mode", None) == "dialog"

    def _type_delay(self):
        _sleep_ms(self.config.type_delay_ms)

    @staticmethod
    def _strip_ansi(text):
        return re.sub(r"\x1b\[[0-9;?]*[A-Za-z]", "", text or "")


class MCPServer:
    """Small JSON-RPC/MCP-style server over stdio or local TCP."""

    def __init__(self, app, config):
        self.app = app
        self.config = config
        self.controller = MCPController(app, config)
        self._tcp_server = None
        self._http_server = None
        self._thread = None

    def start_background(self):
        if self.config.transport == "stdio":
            raise ValueError("stdio MCP transport cannot run in GUI background mode")
        self._thread = threading.Thread(target=self.serve_forever, daemon=True)
        self._thread.start()
        return self

    def serve_forever(self):
        if self.config.transport == "stdio":
            self.serve_stdio()
            return
        if self.config.transport == "tcp":
            self.serve_tcp()
            return
        if self.config.transport == "http":
            self.serve_http()
            return
        raise ValueError(f"Unsupported MCP transport: {self.config.transport}")

    def serve_stdio(self):
        for raw in sys.stdin:
            response = self.handle_json_line(raw)
            if response is not None:
                sys.stdout.write(json.dumps(response) + "\n")
                sys.stdout.flush()

    def serve_tcp(self):
        handler = self._make_tcp_handler()
        self._tcp_server = socketserver.ThreadingTCPServer(
            (self.config.host, self.config.port),
            handler,
        )
        self._tcp_server.daemon_threads = True
        self._tcp_server.serve_forever()

    def serve_http(self):
        handler = self._make_http_handler()
        self._http_server = http.server.ThreadingHTTPServer(
            (self.config.host, self.config.port),
            handler,
        )
        self._http_server.daemon_threads = True
        self._http_server.serve_forever()

    def shutdown(self):
        if self._tcp_server is not None:
            self._tcp_server.shutdown()
            self._tcp_server.server_close()
        if self._http_server is not None:
            self._http_server.shutdown()
            self._http_server.server_close()

    def handle_json_line(self, raw):
        try:
            request = json.loads(raw)
            return self.handle_json_request(request)
        except Exception as exc:
            request_id = None
            if isinstance(locals().get("request"), dict):
                request_id = request.get("id")
            return {
                "jsonrpc": JSONRPC_VERSION,
                "id": request_id,
                "error": {"code": -32000, "message": str(exc)},
            }

    def handle_json_request(self, request):
        if isinstance(request, list):
            if not request:
                return self._jsonrpc_error(None, "Invalid JSON-RPC batch: empty array")
            responses = []
            for item in request:
                if not isinstance(item, dict):
                    responses.append(self._jsonrpc_error(None, "JSON-RPC request must be an object"))
                    continue
                try:
                    response = self._handle_json_request_object(item)
                except Exception as exc:
                    responses.append(self._jsonrpc_error(item.get("id"), str(exc)))
                    continue
                if response is not None:
                    responses.append(response)
            return responses or None
        if not isinstance(request, dict):
            raise ValueError("JSON-RPC request must be an object")
        return self._handle_json_request_object(request)

    def _handle_json_request_object(self, request):
        result = self.handle_request(request)
        if request.get("id") is None:
            return None
        return {"jsonrpc": JSONRPC_VERSION, "id": request.get("id"), "result": result}

    @staticmethod
    def _jsonrpc_error(request_id, message):
        return {
            "jsonrpc": JSONRPC_VERSION,
            "id": request_id,
            "error": {"code": -32000, "message": str(message)},
        }

    def handle_http_post(self, path, headers, body):
        """Handle one Streamable HTTP POST and return status, headers, payload."""
        if path != DEFAULT_HTTP_ENDPOINT:
            return HTTPStatus.NOT_FOUND, {}, b""
        if not self._http_origin_allowed(headers.get("Origin")):
            return HTTPStatus.FORBIDDEN, {}, b""
        if not self._http_protocol_allowed(headers.get("MCP-Protocol-Version")):
            return HTTPStatus.BAD_REQUEST, {}, b""
        if not self._http_accept_allowed(headers.get("Accept", "")):
            return HTTPStatus.NOT_ACCEPTABLE, {}, b""

        try:
            request = json.loads(body.decode("utf-8"))
            response = self.handle_json_request(request)
        except Exception as exc:
            response = {
                "jsonrpc": JSONRPC_VERSION,
                "id": None,
                "error": {"code": -32000, "message": str(exc)},
            }

        if response is None:
            return HTTPStatus.ACCEPTED, {}, b""

        payload = json.dumps(response).encode("utf-8")
        response_headers = {
            "Content-Type": HTTP_JSON_CONTENT_TYPE,
            "MCP-Protocol-Version": MCP_PROTOCOL_VERSION,
        }
        return HTTPStatus.OK, response_headers, payload

    def handle_http_get(self, path, headers):
        """Handle one Streamable HTTP GET."""
        if path != DEFAULT_HTTP_ENDPOINT:
            return HTTPStatus.NOT_FOUND, {}, b""
        if not self._http_origin_allowed(headers.get("Origin")):
            return HTTPStatus.FORBIDDEN, {}, b""
        if not self._http_protocol_allowed(headers.get("MCP-Protocol-Version")):
            return HTTPStatus.BAD_REQUEST, {}, b""
        return HTTPStatus.METHOD_NOT_ALLOWED, {}, b""

    @staticmethod
    def _http_accept_allowed(accept):
        if not accept or "*/*" in accept:
            return True
        return any(content_type in accept for content_type in HTTP_ACCEPTED_CONTENT_TYPES)

    @staticmethod
    def _http_protocol_allowed(protocol):
        return protocol is None or protocol in SUPPORTED_HTTP_PROTOCOL_VERSIONS

    @staticmethod
    def _http_origin_allowed(origin):
        if not origin:
            return True
        return (
            origin == "null"
            or origin.startswith("http://127.0.0.1")
            or origin.startswith("http://localhost")
            or origin.startswith("https://127.0.0.1")
            or origin.startswith("https://localhost")
        )

    def handle_request(self, request):
        method = request.get("method")
        params = request.get("params") or {}

        if method == "initialize":
            return {
                "protocolVersion": MCP_PROTOCOL_VERSION,
                "serverInfo": {"name": "uimd-runtime", "version": "0.1"},
                "capabilities": {"tools": {}},
            }
        if method == "tools/list":
            return {"tools": self.controller.list_tools()}
        if method == "tools/call":
            name = params.get("name")
            arguments = params.get("arguments") or {}
            return {
                "content": [
                    {
                        "type": "text",
                        "text": json.dumps(self.controller.call_tool(name, arguments)),
                    }
                ]
            }

        if method and method.startswith("ui."):
            return self.controller.call_tool(method[3:], params)
        return self.controller.call_tool(method, params)

    def _make_tcp_handler(self):
        server = self

        class _Handler(socketserver.StreamRequestHandler):
            def handle(self):
                for raw in self.rfile:
                    response = server.handle_json_line(raw.decode("utf-8"))
                    if response is not None:
                        self.wfile.write((json.dumps(response) + "\n").encode("utf-8"))
                        self.wfile.flush()

        return _Handler

    def _make_http_handler(self):
        server = self

        class _Handler(http.server.BaseHTTPRequestHandler):
            server_version = "UIMCPHTTP/0.1"

            def do_POST(self):
                length = int(self.headers.get("Content-Length", "0"))
                body = self.rfile.read(length)
                status, headers, payload = server.handle_http_post(self.path, self.headers, body)
                self.send_response(status)
                for name, value in headers.items():
                    self.send_header(name, value)
                self.send_header("Content-Length", str(len(payload)))
                self.end_headers()
                if payload:
                    self.wfile.write(payload)

            def do_GET(self):
                status, headers, payload = server.handle_http_get(self.path, self.headers)
                self.send_response(status)
                for name, value in headers.items():
                    self.send_header(name, value)
                self.send_header("Content-Length", str(len(payload)))
                self.end_headers()
                if payload:
                    self.wfile.write(payload)

            def do_DELETE(self):
                if self.path != DEFAULT_HTTP_ENDPOINT:
                    self._send_empty(HTTPStatus.NOT_FOUND)
                    return
                self._send_empty(HTTPStatus.METHOD_NOT_ALLOWED)

            def log_message(self, _format, *args):
                return

            def _send_empty(self, status):
                self.send_response(status)
                self.send_header("Content-Length", "0")
                self.end_headers()

        return _Handler


def parse_agent_metadata(source):
    """Extract generated agent metadata from an embedded section-based md source."""
    if not source:
        return {"window": {}, "elements": {}}

    try:
        metadata = _parse_flat_yaml_mapping(_first_fenced_block(_section_after_heading(source, "Metadata")))
        members = _parse_member_metadata_mapping(_first_fenced_block(_section_after_heading(source, "Members")))
        return {
            "window": {"description": str(metadata.get("description", "") or "")},
            "elements": members,
        }
    except Exception:
        return {"window": {}, "elements": {}}


def parse_app_tools(source):
    """Extract generated app-specific MCP tool metadata from an embedded source."""
    if not source:
        return {}

    try:
        return _parse_app_tools_mapping(_first_fenced_block(_section_after_heading(source, "Tools")))
    except Exception:
        return {}


def _section_after_heading(source, heading_text):
    pattern = re.compile(rf"^##+\s+{re.escape(heading_text)}\s*$", re.MULTILINE | re.IGNORECASE)
    match = pattern.search(source)
    if not match:
        return ""
    rest = source[match.end():]
    next_heading = re.search(r"^##+\s+", rest, re.MULTILINE)
    return rest[:next_heading.start()] if next_heading else rest


def _first_fenced_block(section):
    match = re.search(r"```(?:yaml|yml)?\s*\n(.*?)\n```", section, re.DOTALL | re.IGNORECASE)
    return match.group(1) if match else ""


def _parse_flat_yaml_mapping(block):
    result = {}
    for line in block.splitlines():
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        if line.startswith(" ") or ":" not in line:
            continue
        key, value = _split_yaml_scalar(line.strip())
        result[key] = value
    return result


def _parse_member_metadata_mapping(block):
    members = {}
    current_member = None
    current_values = {}

    def finish_member():
        if not current_member:
            return
        member_type = str(current_values.get("type", "label") or "label").lower()
        description = str(
            current_values.get("description")
            or current_values.get("title")
            or current_values.get("text")
            or current_member
        )
        members[current_member] = {
            "role": _role_for_type(member_type),
            "description": description,
            "expose": _truthy(current_values.get("expose", True)),
        }

    for line in block.splitlines():
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        indent = len(line) - len(line.lstrip(" "))
        stripped = line.strip()

        if indent == 0 and stripped.endswith(":"):
            finish_member()
            current_member = stripped[:-1]
            current_values = {}
            continue

        if current_member and indent >= 2 and ":" in stripped:
            key, value = _split_yaml_scalar(stripped)
            current_values[key] = value

    finish_member()
    return members


def _role_for_type(member_type):
    mapping = {
        "button": "action",
        "checkbox": "toggle",
        "combobox": "selection",
        "listbox": "selection",
        "textinput": "input",
        "numberinput": "input",
        "textarea": "input",
    }
    return mapping.get(str(member_type or "").lower(), "text")


def _truthy(value):
    if isinstance(value, bool):
        return value
    if value is None:
        return False
    return str(value).strip().lower() not in {"false", "0", "no", "off", "none", ""}


def _parse_app_tools_mapping(block):
    tools = {}
    current_tool = None
    current_section = None
    current_nested = None

    for line in block.splitlines():
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        indent = len(line) - len(line.lstrip(" "))
        stripped = line.strip()

        if indent == 0 and stripped.endswith(":"):
            current_tool = stripped[:-1]
            current_section = None
            current_nested = None
            tools[current_tool] = {"input_schema": {"type": "object", "properties": {}}, "output_schema": {}}
            continue

        if not current_tool:
            continue

        if indent == 2 and ":" in stripped:
            key, value = _split_yaml_scalar(stripped)
            current_nested = None
            if key == "description":
                tools[current_tool]["description"] = value
                current_section = None
            elif key == "input":
                current_section = "input"
            elif key == "output":
                current_section = "output"
            continue

        if current_section == "input" and indent >= 4 and ":" in stripped:
            key, value = _split_yaml_scalar(stripped)
            if key == "required":
                tools[current_tool]["input_schema"]["required"] = _parse_inline_list(value)
            else:
                tools[current_tool]["input_schema"]["properties"][key] = _schema_for_type(value)
            continue

        if current_section == "output" and indent >= 4:
            if indent == 4 and ":" in stripped:
                key, value = _split_yaml_scalar(stripped)
                if value:
                    tools[current_tool]["output_schema"][key] = _schema_for_type(value)
                    current_nested = None
                else:
                    tools[current_tool]["output_schema"][key] = {"type": "object", "properties": {}}
                    current_nested = key
                continue
            if current_nested and indent >= 6 and ":" in stripped:
                key, value = _split_yaml_scalar(stripped.lstrip("- "))
                properties = tools[current_tool]["output_schema"][current_nested].setdefault("properties", {})
                properties[key] = _schema_for_type(value)

    return tools


def _parse_inline_list(value):
    text = str(value or "").strip()
    if text.startswith("[") and text.endswith("]"):
        text = text[1:-1]
    return [item.strip().strip('"').strip("'") for item in text.split(",") if item.strip()]


def _schema_for_type(type_name):
    mapping = {"bool": "boolean", "boolean": "boolean", "int": "integer", "integer": "integer", "number": "number"}
    return {"type": mapping.get(str(type_name or "").strip().lower(), "string")}


def _parse_agent_metadata_mapping(block):
    metadata = {"window": {}, "elements": {}}
    current_section = None
    current_element = None

    for line in block.splitlines():
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        indent = len(line) - len(line.lstrip(" "))
        stripped = line.strip()

        if indent == 0 and stripped.endswith(":"):
            current_section = stripped[:-1]
            current_element = None
            continue

        if current_section == "window" and indent >= 2 and ":" in stripped:
            key, value = _split_yaml_scalar(stripped)
            metadata["window"][key] = value
            continue

        if current_section == "elements":
            if indent == 2 and stripped.endswith(":"):
                current_element = stripped[:-1]
                metadata["elements"].setdefault(current_element, {})
                continue
            if current_element and indent >= 4 and ":" in stripped:
                key, value = _split_yaml_scalar(stripped)
                metadata["elements"][current_element][key] = value

    return metadata


def _split_yaml_scalar(stripped):
    key, _, value = stripped.partition(":")
    return key.strip(), value.strip().strip('"').strip("'")


def _sleep_ms(ms):
    ms = int(ms or 0)
    if ms > 0:
        time.sleep(ms / 1000.0)


def _first_sentence(text):
    text = (text or "").strip()
    if not text:
        return ""
    return text.split(".", 1)[0] + "."
