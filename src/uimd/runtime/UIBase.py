import re
import sys
import time

from .uiinstance import UIInstance
from .uielement import UIElement
from .style import Style, Color, DEFAULT_TEXT_COLOR, DEFAULT_BG_COLOR
from .elements import _build_visual_rows, _copy_text_to_clipboard, _notify_copied
from .rendering import TerminalCell, cells_to_ansi_lines, coerce_cell, parse_ansi_cells


_ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")
MOUSE_LEFT_BUTTON = 0
USER_SELECT_TEXT = "text"
DIMENSION_AUTO = "auto"
DIMENSION_FIXED = "fixed"
DIMENSION_EXPANDED = "expanded"
DIMENSION_FIT_CONTENT = "fit-content"
EXPANDED = 0x1FFFFFFF
FIT_CONTENT = 0x1FFFFFFE
MIN_LAYOUT_SIZE = 1
TEXTINPUT_CURSOR_SPACE = 1
NUMBERINPUT_PADDING_X = 2
CHECKBOX_INDICATOR_WIDTH = 3
CHECKBOX_TEXT_GAP = 1
COMBOBOX_ARROW_WIDTH = 1
LISTBOX_ITEM_PREFIX_WIDTH = 0
LISTBOX_DOUBLE_CLICK_SECONDS = 0.35
DIALOG_BUTTON_CLOSE_DELAY_SECONDS = 0.18


class LayoutCell:
    """Structural cell from layout metadata."""
    __slots__ = ("source_row", "source_col", "source_width", "source_height",
                 "logical_width", "logical_height", "cell_name",
                 "width_mode", "height_mode")

    def __init__(self, source_row=0, source_col=0, source_width=1,
                 source_height=1, logical_width=None, logical_height=None,
                 cell_name="", width_mode=DIMENSION_AUTO,
                 height_mode=DIMENSION_AUTO):
        self.source_row = source_row
        self.source_col = source_col
        self.source_width = source_width
        self.source_height = source_height
        self.logical_width = logical_width
        self.logical_height = logical_height
        self.cell_name = cell_name
        self.width_mode = width_mode
        self.height_mode = height_mode


class ResolvedCell:
    """Resolved cell with final position and size."""
    __slots__ = ("row", "col", "width", "height",
                 "source_row", "source_col", "source_width", "source_height")

    def __init__(self, row=0, col=0, width=1, height=1,
                 source_row=0, source_col=0, source_width=1, source_height=1):
        self.row = row
        self.col = col
        self.width = width
        self.height = height
        self.source_row = source_row
        self.source_col = source_col
        self.source_width = source_width
        self.source_height = source_height


class LayoutNode:
    """Base class for layout tree nodes."""
    __slots__ = ("kind",)

    def __init__(self, kind):
        self.kind = kind


class LeafNode(LayoutNode):
    """Leaf node representing a structural cell."""
    __slots__ = ("left", "top", "right", "bottom",
                 "logical_width", "logical_height",
                 "source_row", "source_col", "source_width", "source_height")

    def __init__(self, left=0, top=0, right=1, bottom=1,
                 logical_width=None, logical_height=None,
                 source_row=0, source_col=0, source_width=1, source_height=1):
        super().__init__("leaf")
        self.left = left
        self.top = top
        self.right = right
        self.bottom = bottom
        self.logical_width = logical_width
        self.logical_height = logical_height
        self.source_row = source_row
        self.source_col = source_col
        self.source_width = source_width
        self.source_height = source_height


class GroupNode(LayoutNode):
    """Group node representing a horizontal or vertical arrangement."""
    __slots__ = ("orientation", "children", "left", "top", "right", "bottom",
                 "logical_width", "logical_height")

    def __init__(self, orientation="horizontal", children=None,
                 left=0, top=0, right=1, bottom=1,
                 logical_width=None, logical_height=None):
        super().__init__("group")
        self.orientation = orientation
        self.children = children or []
        self.left = left
        self.top = top
        self.right = right
        self.bottom = bottom
        self.logical_width = logical_width
        self.logical_height = logical_height


def _strip_ansi(text):
    """Remove ANSI escape codes from text."""
    return _ANSI_RE.sub("", text) if text else ""


def _make_ansi(fg, bg):
    """Build an ANSI escape string from Color objects or RGB strings."""
    parts = []
    if fg is not None:
        if hasattr(fg, "r") and fg.r >= 0:
            parts.append(f"\x1b[38;2;{fg.r};{fg.g};{fg.b}m")
        elif isinstance(fg, str) and len(fg) == 9:
            parts.append(f"\x1b[38;2;{fg[0:3]};{fg[3:6]};{fg[6:9]}m")
    if bg is not None:
        if hasattr(bg, "r") and bg.r >= 0:
            parts.append(f"\x1b[48;2;{bg.r};{bg.g};{bg.b}m")
        elif isinstance(bg, str) and len(bg) == 9:
            parts.append(f"\x1b[48;2;{bg[0:3]};{bg[3:6]};{bg[6:9]}m")
    return "".join(parts) if parts else ""


def _escape_to_list(s):
    """Convert a string to a list of single characters."""
    return list(s)


class Cell:
    """Represents a cell in the UI grid layout."""

    def __init__(self, row_start=0, col_start=0, width=None, height=None):
        self.row_start = row_start
        self.col_start = col_start
        self.width = width  # None means expand
        self.height = height  # None means expand


class LayoutGrid:
    """Parsed grid layout from ASCII art."""

    def __init__(self):
        self.cells = []  # List of (row, col, width, height, element_name) tuples
        self.total_width = 0
        self.total_height = 0




class UIBase(UIInstance):
    """Base class for UI containers with layout, focus management, keyboard/mouse handling, and rendering."""

    def __init__(self, layout=None, members=None, style=None):
        super().__init__()
        self._layout = layout or []  # List of element definitions
        self._members = members or {}
        self._compiled_styles = {}  # raw style dict from COMPILED_STYLE
        self._mode = "normal"  # normal, expand_width, expand_height, fullscreen, dialog
        self._app = None
        self.focusable = False

        if isinstance(style, dict):
            window_style_dict = style.get("this", {})
            s = Style()
            for k, v in window_style_dict.items():
                s.set(k, v)
            self.style = s or Style()
            for selector, props in style.items():
                if selector != "this":
                    self._compiled_styles[selector] = dict(props)
        else:
            self.style = style or Style()

        if self.style.background is None:
            self.style.background = Color("#000000")

        self._elements = {}  # name -> UIElement instance
        self._focused_element = None
        self._edit_mode = False
        self._edit_snapshot = None
        self._active_scrollview_scope = None
        self._scrollview_last_descendant = {}
        self._mouse_selection_element = None
        self._mouse_selection_anchor = None
        self._mouse_selection_changed = False
        self._mouse_click_candidate_element = None
        self._mouse_click_candidate_moved = False
        self._last_listbox_click = None
        self._suppress_edit_notifications = False
        self._window_width = 80
        self._window_height = 24
        self._resolved_cell_backgrounds = []
        self._resolved_element_textures = []
        self._resolved_cells_by_name = {}
        self._layout_version = 0  # increments when layout needs re-resolution
        self._last_window_size = (0, 0)

    def open(self):
        """Open the window and apply layout."""
        for elem in self._elements.values():
            elem._app = self._app
        self.apply_layout()
        for elem in self._elements.values():
            elem.open()

    def resize(self, width, height):
        """Update the available window size for layout resolution."""
        if width:
            self._window_width = int(width)
        if height:
            self._window_height = int(height)
        new_size = (self._window_width, self._window_height)
        if new_size != self._last_window_size:
            self._last_window_size = new_size
            if self._layout and self._elements:
                self._resolve_layout_geometry()



    @property
    def focused(self):
        return self._focused_element is not None or True

    def _notify_focused(self, element, value):
        callback = type(self).__dict__.get("focused")
        if callback is not None and callable(callback):
            callback(self, element, value)

    @property
    def mode(self):
        return self._mode

    @mode.setter
    def mode(self, value):
        old_value = self._mode
        self._mode = value
        if old_value != value and self._layout and self._elements:
            self._resolve_layout_geometry()

    def create_element(self, name, elem_type, **kwargs):
        """Create a UI element instance from definition."""
        from .elements import (Label, SpanLabel, InfoLabel, FrameBufferView, TextInput, TextArea, NumberInput, Button,
                               CheckBox, ComboBox, ListBox, UIElementReusable, ViewHost, MessageTable)
        from .image import Image

        type_map = {
            "label": Label,
            "spanlabel": SpanLabel,
            "infolabel": InfoLabel,
            "framebufferview": FrameBufferView,
            "messagetable": MessageTable,
            "image": Image,
            "textinput": TextInput,
            "textarea": TextArea,
            "numberinput": NumberInput,
            "button": Button,
            "checkbox": CheckBox,
            "combobox": ComboBox,
            "listbox": ListBox,
            "uielement": UIElementReusable,
            "viewhost": ViewHost,
        }

        cls = type_map.get(elem_type)
        if not cls:
            raise ValueError(f"Unknown element type: {elem_type}")

        elem_kwargs = {k: v for k, v in kwargs.items() if k not in ('color', 'background')}
        elem = cls(name=name, **elem_kwargs)
        elem.parent = self
        elem._app = self._app
        self._apply_styles_to_element(elem, name, elem_type)
        for k in ('color', 'background'):
            if k in kwargs:
                elem.style.set(k, kwargs[k])
        self._elements[name] = elem
        return elem

    @staticmethod
    def _is_name_selector(selector):
        return selector.startswith("#") or selector.startswith("@")

    def _selector_matches_element(self, selector, name, elem_type, cell_name=""):
        """Check whether a compiled selector matches one element."""
        if " " in selector:
            scope, nested = selector.split(" ", 1)
            if scope == "cell":
                return self._selector_matches_element(nested, name, elem_type, "")
            if not cell_name or scope != cell_name:
                return False
            return self._selector_matches_element(nested, name, elem_type, "")

        if selector == elem_type:
            return True
        if elem_type == "textarea" and selector == "textinput":
            return True
        if self._is_name_selector(selector):
            return selector[1:] == name
        return False

    def _apply_styles_to_element(self, elem, name, elem_type, cell_name=""):
        """Apply matching compiled styles to an element."""
        # Priority: plain type < cell-scoped type < name selector (@/# prefix)
        def _priority(item):
            selector = item[0]
            final = selector.split(" ")[-1]
            if self._is_name_selector(final):
                return 2
            if " " in selector:
                return 1
            return 0

        for selector, props in sorted(self._compiled_styles.items(), key=_priority):
            if not self._selector_matches_element(selector, name, elem_type, cell_name):
                continue
            for k, v in props.items():
                if k.startswith("edit-"):
                    if elem.edit_style is None:
                        from .style import Style
                        elem.edit_style = Style()
                    elem.edit_style.set(k[len("edit-"):], v)
                elif k.startswith("focus-"):
                    if elem.focus_style is None:
                        from .style import Style
                        elem.focus_style = Style()
                    elem.focus_style.set(k[len("focus-"):], v)
                elif k.startswith("selected-"):
                    if elem.selected_style is None:
                        from .style import Style
                        elem.selected_style = Style()
                    elem.selected_style.set(k[len("selected-"):], v)
                elif k.startswith("checked-"):
                    if elem.checked_style is None:
                        from .style import Style
                        elem.checked_style = Style()
                    elem.checked_style.set(k[len("checked-"):], v)
                elif k.startswith("unchecked-"):
                    if elem.unchecked_style is None:
                        from .style import Style
                        elem.unchecked_style = Style()
                    elem.unchecked_style.set(k[len("unchecked-"):], v)
                elif k.startswith("disabled-"):
                    if elem.disabled_style is None:
                        from .style import Style
                        elem.disabled_style = Style()
                    elem.disabled_style.set(k[len("disabled-"):], v)
                elif k.startswith("cursor-"):
                    if elem.cursor_style is None:
                        from .style import Style
                        elem.cursor_style = Style()
                    elem.cursor_style.set(k[len("cursor-"):], v)
                else:
                    elem.style.set(k, v)

    def _cell_style(self, cell_name):
        """Return the merged compiled style for a structural cell."""
        style = Style()
        matched = False

        for selector in ("cell", cell_name, f"@{cell_name}" if cell_name else None):
            if not selector:
                continue
            props = self._compiled_styles.get(selector)
            if not props:
                continue
            matched = True
            for key, value in props.items():
                style.set(key, value)

        return style if matched else None

    def _border_width_horizontal(self):
        """Return the runtime top/bottom border thickness in characters."""
        return max(0, int(getattr(self.style, "border_width_horizontal", 0) or 0))

    def _border_width_vertical(self):
        """Return the runtime left/right border thickness in characters."""
        return max(0, int(getattr(self.style, "border_width_vertical", 0) or 0))

    def get_natural_size(self):
        """Return the window's natural size after applying border-width rules."""
        cells = self._collect_layout_cells()
        if not cells:
            return 0, 0

        self._apply_fit_content_dimensions(cells)
        root = self._build_layout_tree(cells)
        if not root:
            return 0, 0

        self._apply_auto_cell_dimensions(cells)
        self._apply_cell_padding_dimensions(cells)
        root = self._build_layout_tree(cells)

        base_width = getattr(root, "logical_width", None)
        base_height = getattr(root, "logical_height", None)

        if base_width is None:
            base_width = max(1, root.right - root.left - 1)
        if base_height is None:
            base_height = max(1, root.bottom - root.top - 1)

        return (
            base_width + self._border_width_vertical() * 2,
            base_height + self._border_width_horizontal() * 2,
        )

    def _dimension_mode(self, entry, axis, logical_value, source_value):
        """Return whether a structural cell dimension is auto, fixed, or expanded."""
        mode = entry.get(f"cell_{axis}_mode")
        if mode in (DIMENSION_AUTO, DIMENSION_FIXED, DIMENSION_EXPANDED, DIMENSION_FIT_CONTENT):
            return mode
        if logical_value is None:
            return DIMENSION_EXPANDED
        if int(logical_value) == int(source_value):
            return DIMENSION_AUTO
        return DIMENSION_FIXED

    def _apply_auto_cell_dimensions(self, cells):
        """Resolve auto cell sizes against the current runtime border thickness."""
        if not cells:
            return

        leaf_bounds = {}
        horizontal_boundaries = set()

        for cell in cells:
            left = cell.source_col - 1
            top = cell.source_row - 1
            right = left + cell.source_width + 1
            bottom = top + cell.source_height + 1
            leaf_bounds[id(cell)] = (left, top, right, bottom)
            horizontal_boundaries.update((top, bottom))

        horizontal_border = self._border_width_horizontal()

        for cell in cells:
            _left, top, _right, bottom = leaf_bounds[id(cell)]
            if cell.logical_height is not None and cell.height_mode == DIMENSION_AUTO:
                separators = sum(1 for boundary in horizontal_boundaries if top < boundary < bottom)
                padding_extra = max(0, int(cell.logical_height) - int(cell.source_height))
                cell.logical_height = self._auto_dimension_size(
                    cell.source_height, separators, horizontal_border
                ) + padding_extra

    @staticmethod
    def _auto_dimension_size(source_size, separator_count, border_width):
        return max(1, int(source_size) + int(separator_count) * (int(border_width) - 1))

    def _apply_cell_padding_dimensions(self, cells):
        """Add cell padding to non-expanded, non-fit-content logical dimensions."""
        for cell in cells:
            cell_style = self._cell_style(cell.cell_name)
            if not cell_style or cell_style.padding == (0, 0, 0, 0):
                continue

            pad_top, pad_right, pad_bottom, pad_left = cell_style.padding
            if (
                cell.logical_width is not None
                and cell.width_mode not in (DIMENSION_EXPANDED, DIMENSION_FIT_CONTENT)
            ):
                cell.logical_width = max(
                    MIN_LAYOUT_SIZE,
                    int(cell.logical_width) + pad_left + pad_right,
                )
            if (
                cell.logical_height is not None
                and cell.height_mode not in (DIMENSION_EXPANDED, DIMENSION_FIT_CONTENT)
            ):
                cell.logical_height = max(
                    MIN_LAYOUT_SIZE,
                    int(cell.logical_height) + pad_top + pad_bottom,
                )

    def _apply_fit_content_dimensions(self, cells):
        """Resolve fit-content structural cells from current element content."""
        if not cells:
            return

        entries_by_cell = {}
        for entry in self._layout:
            entries_by_cell.setdefault(self._layout_entry_key(entry), []).append(entry)

        for cell in cells:
            if cell.width_mode != DIMENSION_FIT_CONTENT and cell.height_mode != DIMENSION_FIT_CONTENT:
                continue

            entries = entries_by_cell.get(
                (cell.source_row, cell.source_col, cell.source_width, cell.source_height),
                [],
            )
            cell_style = self._cell_style(cell.cell_name)
            pad_top, pad_right, pad_bottom, pad_left = (
                cell_style.padding if cell_style else (0, 0, 0, 0)
            )

            if cell.width_mode == DIMENSION_FIT_CONTENT:
                content_width = max(
                    (self._fit_entry_width(entry) for entry in entries),
                    default=cell.source_width,
                )
                cell.logical_width = max(MIN_LAYOUT_SIZE, content_width + pad_left + pad_right)

            if cell.height_mode == DIMENSION_FIT_CONTENT:
                content_height = max(
                    (self._fit_entry_height(entry) for entry in entries),
                    default=cell.source_height,
                )
                cell.logical_height = max(MIN_LAYOUT_SIZE, content_height + pad_top + pad_bottom)

    def _recompute_fit_content_heights_pass2(self, cells, resolved_cells):
        """Second pass: recompute fit-content heights using actual cell widths for word-wrap."""
        resolved_by_src = {
            (c.source_row, c.source_col, c.source_width, c.source_height): c
            for c in resolved_cells
        }
        entries_by_cell = {}
        for entry in self._layout:
            entries_by_cell.setdefault(self._layout_entry_key(entry), []).append(entry)

        changed = False
        for cell in cells:
            if cell.height_mode != DIMENSION_FIT_CONTENT:
                continue
            resolved = resolved_by_src.get(
                (cell.source_row, cell.source_col, cell.source_width, cell.source_height)
            )
            if not resolved:
                continue

            cell_style = self._cell_style(cell.cell_name)
            pad_top, pad_right, pad_bottom, pad_left = (
                cell_style.padding if cell_style else (0, 0, 0, 0)
            )
            available_cell_width = resolved.width - pad_left - pad_right

            entries = entries_by_cell.get(
                (cell.source_row, cell.source_col, cell.source_width, cell.source_height), []
            )
            content_height = max(
                (self._fit_entry_height(entry, available_cell_width) for entry in entries),
                default=cell.source_height,
            )
            new_height = max(MIN_LAYOUT_SIZE, content_height + pad_top + pad_bottom)
            if new_height != cell.logical_height:
                cell.logical_height = new_height
                changed = True

        return changed

    @staticmethod
    def _layout_entry_key(entry):
        return (
            int(entry.get("cell_row", entry.get("row", 0)) or 0) + 1,
            int(entry.get("cell_col", entry.get("col", 0)) or 0) + 1,
            int(entry.get("cell_chars_width", entry.get("chars_width", 1)) or 1),
            int(entry.get("cell_chars_height", entry.get("chars_height", 1)) or 1),
        )

    def _entry_style_margin(self, entry):
        elem = self._elements.get(entry.get("name"))
        style = getattr(elem, "style", None) if elem is not None else None
        return style.margin if style else (0, 0, 0, 0)

    def _has_expanded_entry_before_in_source_row(self, entry):
        source_key = (
            int(entry.get("cell_row", entry.get("row", 0)) or 0),
            int(entry.get("cell_col", entry.get("col", 0)) or 0),
            int(entry.get("cell_chars_width", entry.get("chars_width", 1)) or 1),
            int(entry.get("cell_chars_height", entry.get("chars_height", 1)) or 1),
        )
        row = int(entry.get("row", 0) or 0)
        col = int(entry.get("col", 0) or 0)
        for other in getattr(self, "_layout", []):
            other_source_key = (
                int(other.get("cell_row", other.get("row", 0)) or 0),
                int(other.get("cell_col", other.get("col", 0)) or 0),
                int(other.get("cell_chars_width", other.get("chars_width", 1)) or 1),
                int(other.get("cell_chars_height", other.get("chars_height", 1)) or 1),
            )
            if other_source_key != source_key:
                continue
            if int(other.get("row", 0) or 0) != row:
                continue
            if int(other.get("col", 0) or 0) >= col:
                continue
            if other.get("width_mode") == DIMENSION_EXPANDED or other.get("width") == EXPANDED:
                return True
        return False

    @staticmethod
    def _entry_source_col_span(entry):
        col = int(entry.get("col", 0) or 0)
        width = int(entry.get("chars_width", entry.get("width", 1)) or 1)
        return col, col + max(MIN_LAYOUT_SIZE, width)

    @staticmethod
    def _source_spans_overlap(first, second):
        first_start, first_end = first
        second_start, second_end = second
        return first_start < second_end and second_start < first_end

    def _has_expanded_entry_before_in_source_column(self, entry):
        source_key = (
            int(entry.get("cell_row", entry.get("row", 0)) or 0),
            int(entry.get("cell_col", entry.get("col", 0)) or 0),
            int(entry.get("cell_chars_width", entry.get("chars_width", 1)) or 1),
            int(entry.get("cell_chars_height", entry.get("chars_height", 1)) or 1),
        )
        row = int(entry.get("row", 0) or 0)
        entry_span = self._entry_source_col_span(entry)
        for other in getattr(self, "_layout", []):
            other_source_key = (
                int(other.get("cell_row", other.get("row", 0)) or 0),
                int(other.get("cell_col", other.get("col", 0)) or 0),
                int(other.get("cell_chars_width", other.get("chars_width", 1)) or 1),
                int(other.get("cell_chars_height", other.get("chars_height", 1)) or 1),
            )
            if other_source_key != source_key:
                continue
            if int(other.get("row", 0) or 0) >= row:
                continue
            if not self._source_spans_overlap(self._entry_source_col_span(other), entry_span):
                continue
            if other.get("height_mode") == DIMENSION_EXPANDED or other.get("height") == EXPANDED:
                return True
        return False

    def _fit_entry_width(self, entry):
        elem = self._elements.get(entry.get("name"))
        _margin_top, margin_right, _margin_bottom, margin_left = self._entry_style_margin(entry)
        content_width = self._element_fit_width(
            elem,
            int(entry.get("chars_width", 0) or 0),
        )
        return (
            int(entry.get("col", 0) or 0)
            + margin_left
            + content_width
            + int(entry.get("margin_right", 0) or 0)
            + margin_right
        )

    def _fit_entry_height(self, entry, available_cell_width=None):
        elem = self._elements.get(entry.get("name"))
        margin_top, margin_right, margin_bottom, margin_left = self._entry_style_margin(entry)
        layout_margin_bottom = 0
        width = None
        if available_cell_width is not None:
            col = int(entry.get("col", 0) or 0)
            layout_margin_right = int(entry.get("margin_right", 0) or 0)
            width = max(
                MIN_LAYOUT_SIZE,
                available_cell_width - col - margin_left - layout_margin_right - margin_right,
            )
        content_height = self._element_fit_height(
            elem,
            int(entry.get("chars_height", 0) or 0),
            width,
        )
        return (
            int(entry.get("row", 0) or 0)
            + margin_top
            + content_height
            + layout_margin_bottom
            + margin_bottom
        )

    def _element_fit_width(self, elem, fallback):
        if elem is None:
            return max(MIN_LAYOUT_SIZE, fallback)

        if getattr(elem, "ELEMENT_TYPE", None) in ("label", "spanlabel", "infolabel"):
            text = str(getattr(elem, "text", "") or "")
            return max(MIN_LAYOUT_SIZE, max((len(line) for line in text.splitlines()), default=0))

        if getattr(elem, "ELEMENT_TYPE", None) in ("textinput", "textarea"):
            value = str(getattr(elem, "value", "") or "")
            return max(MIN_LAYOUT_SIZE, max((len(line) for line in value.splitlines()), default=len(value)) + TEXTINPUT_CURSOR_SPACE)

        if getattr(elem, "ELEMENT_TYPE", None) == "numberinput":
            return max(MIN_LAYOUT_SIZE, len(str(getattr(elem, "display_value", "") or "")) + NUMBERINPUT_PADDING_X)

        if getattr(elem, "ELEMENT_TYPE", None) == "button":
            return max(MIN_LAYOUT_SIZE, len(str(getattr(elem, "title", "") or "")))

        if getattr(elem, "ELEMENT_TYPE", None) == "checkbox":
            title = str(getattr(elem, "title", "") or "")
            gap = CHECKBOX_TEXT_GAP if title else 0
            return max(MIN_LAYOUT_SIZE, CHECKBOX_INDICATOR_WIDTH + gap + len(title))

        if getattr(elem, "ELEMENT_TYPE", None) == "combobox":
            selected = str(getattr(elem, "selected_item", "") or "")
            options = [str(option) for option in getattr(elem, "_options", [])]
            content_width = max([len(selected), *[len(option) for option in options]], default=0)
            return max(MIN_LAYOUT_SIZE, content_width + COMBOBOX_ARROW_WIDTH)

        if getattr(elem, "ELEMENT_TYPE", None) == "listbox":
            options = [str(option) for option in getattr(elem, "_options", [])]
            content_width = max((len(option) for option in options), default=0)
            style = elem.get_style() if hasattr(elem, "get_style") else None
            pad_top, pad_right, pad_bottom, pad_left = style.padding if style else (0, 0, 0, 0)
            if (pad_top, pad_right, pad_bottom, pad_left) == (0, 0, 0, 0):
                return max(MIN_LAYOUT_SIZE, content_width + LISTBOX_ITEM_PREFIX_WIDTH)
            return max(MIN_LAYOUT_SIZE, content_width + pad_left + pad_right)

        child = getattr(elem, "_child_instance", None)
        if child is not None and hasattr(child, "get_natural_size"):
            return max(MIN_LAYOUT_SIZE, child.get_natural_size()[0])

        text = str(getattr(elem, "text", "") or "")
        return max(MIN_LAYOUT_SIZE, max((len(line) for line in text.splitlines()), default=fallback))

    def _element_fit_height(self, elem, fallback, width=None):
        if elem is None:
            return max(MIN_LAYOUT_SIZE, fallback)

        if getattr(elem, "ELEMENT_TYPE", None) in ("label", "spanlabel", "infolabel"):
            text = str(getattr(elem, "text", "") or "")
            if not text:
                return MIN_LAYOUT_SIZE
            if getattr(elem, "ELEMENT_TYPE", None) == "spanlabel":
                return max(MIN_LAYOUT_SIZE, len(text.splitlines()))
            if getattr(elem, "ELEMENT_TYPE", None) == "infolabel":
                return max(MIN_LAYOUT_SIZE, len(text.splitlines()))
            if width:
                s = getattr(elem, 'style', None)
                do_wrap = True
                if s and hasattr(s, 'word_wrap'):
                    do_wrap = s.word_wrap
                if do_wrap:
                    return max(MIN_LAYOUT_SIZE, len(_build_visual_rows(text, width)))
            return max(MIN_LAYOUT_SIZE, len(text.splitlines()))

        if getattr(elem, "ELEMENT_TYPE", None) == "messagetable":
            from .elements import MessageTable as MT
            table_text = str(getattr(elem, "text", "") or "")
            parsed = MT._parse_markdown_table(table_text) if table_text else None
            if parsed:
                return max(MIN_LAYOUT_SIZE, 4 + len(parsed['rows']))  # top+header+separator+bottom + data rows
            return MIN_LAYOUT_SIZE

        if getattr(elem, "ELEMENT_TYPE", None) == "listbox":
            options = getattr(elem, "_options", []) or []
            if options:
                return max(MIN_LAYOUT_SIZE, len(options))
            return max(MIN_LAYOUT_SIZE, fallback)

        child = getattr(elem, "_child_instance", None)
        if child is not None and hasattr(child, "get_natural_size"):
            return max(MIN_LAYOUT_SIZE, child.get_natural_size()[1])

        return max(MIN_LAYOUT_SIZE, int(getattr(elem, "height", None) or fallback or MIN_LAYOUT_SIZE))

    def get_element(self, name):
        """Get element by name."""
        return self._elements.get(name)

    def get_all_elements(self):
        """Get all elements as a list."""
        if not self._layout:
            return list(self._elements.values())

        ordered = []
        seen = set()
        for entry in self._layout:
            name = entry.get("name")
            elem = self._elements.get(name) if name else None
            if elem is None:
                continue
            identity = id(elem)
            if identity in seen:
                continue
            ordered.append(elem)
            seen.add(identity)

        for elem in self._elements.values():
            identity = id(elem)
            if identity in seen:
                continue
            ordered.append(elem)
            seen.add(identity)
        return ordered

    @staticmethod
    def _is_direct_focusable_element(elem):
        if not getattr(elem, "enabled", True):
            return False
        if not getattr(elem, "focusable", True):
            return False
        return getattr(elem, "ELEMENT_TYPE", None) not in (
            "image",
            "label",
            "spanlabel",
            "infolabel",
            "messagetable",
            "viewhost",
        )

    @staticmethod
    def _is_scrollview_proxy(elem):
        child = getattr(elem, "_child_instance", None)
        return (
            child is not None
            and hasattr(child, "child_view_entries")
            and hasattr(child, "_clamped_viewport_rect")
        )

    def _local_focusable_elements(self):
        elems = [
            e
            for e in self._elements.values()
            if self._is_direct_focusable_element(e)
        ]
        return sorted(elems, key=lambda e: (
            getattr(e, "_cell_row", 0) + getattr(e, "row", 0),
            getattr(e, "_cell_col", 0) + getattr(e, "col", 0),
        ))

    def _embedded_scrollview_contexts(self):
        contexts = []
        for elem in self._elements.values():
            child = getattr(elem, "_child_instance", None)
            if (
                child is None
                or not hasattr(child, "child_view_entries")
                or not hasattr(child, "_clamped_viewport_rect")
            ):
                continue
            contexts.append((elem, child, self._element_focus_rect_direct(elem)))
        return contexts

    @staticmethod
    def _is_direct_embedded_view(elem):
        child = getattr(elem, "_child_instance", None)
        if child is None or not hasattr(child, "_element_focus_rect"):
            return False
        return not (
            hasattr(child, "child_view_entries")
            and hasattr(child, "_clamped_viewport_rect")
        )

    def _embedded_view_contexts(self, focusable_only=True, resolve_owner=True):
        contexts = []
        for proxy in self._elements.values():
            if not self._is_direct_embedded_view(proxy):
                continue
            child = getattr(proxy, "_child_instance", None)
            origin_rect = self._element_focus_rect_direct(proxy)
            child_elements = (
                child.get_focusable_elements()
                if focusable_only and hasattr(child, "get_focusable_elements")
                else child.get_all_elements()
            )
            for elem in child_elements:
                local_rect = child._element_focus_rect(elem)
                owner = child._event_owner_for_element(elem) if resolve_owner and hasattr(child, "_event_owner_for_element") else child
                context = {
                    "element": elem,
                    "owner": owner,
                    "view": child,
                    "proxy": proxy,
                    "rect": {
                        "top": origin_rect["top"] + local_rect["top"],
                        "left": origin_rect["left"] + local_rect["left"],
                        "bottom": origin_rect["top"] + local_rect["bottom"],
                        "right": origin_rect["left"] + local_rect["right"],
                        "center_row": origin_rect["top"] * 2 + local_rect["center_row"],
                        "center_col": origin_rect["left"] * 2 + local_rect["center_col"],
                    },
                }
                contexts.append(context)
        return contexts

    def _scrollview_child_focus_contexts(self, proxy, scrollview, origin_rect, visible_only=False, focusable_only=True):
        contexts = []
        viewport = scrollview._clamped_viewport_rect() if hasattr(scrollview, "_clamped_viewport_rect") else None
        if not viewport:
            return contexts
        viewport_top = origin_rect["top"] + viewport["row"]
        viewport_bottom = viewport_top + viewport["height"]
        viewport_left = origin_rect["left"] + viewport["col"]
        viewport_right = viewport_left + viewport["width"]
        for entry in scrollview.child_view_entries(viewport):
            child = entry["child"]
            if focusable_only and not hasattr(child, "_local_focusable_elements"):
                continue
            if not focusable_only and not hasattr(child, "get_all_elements"):
                continue
            child_origin_top = origin_rect["top"] + entry["row"]
            child_origin_left = origin_rect["left"] + entry["col"]
            child_elements = child._local_focusable_elements() if focusable_only else child.get_all_elements()
            for elem in child_elements:
                local_rect = child._element_focus_rect_direct(elem)
                rect = {
                    "top": child_origin_top + local_rect["top"],
                    "left": child_origin_left + local_rect["left"],
                    "bottom": child_origin_top + local_rect["bottom"],
                    "right": child_origin_left + local_rect["right"],
                    "center_row": child_origin_top * 2 + local_rect["center_row"],
                    "center_col": child_origin_left * 2 + local_rect["center_col"],
                }
                visible = (
                    rect["bottom"] > viewport_top
                    and rect["top"] < viewport_bottom
                    and rect["right"] > viewport_left
                    and rect["left"] < viewport_right
                )
                if visible_only and not visible:
                    continue
                contexts.append({
                    "element": elem,
                    "owner": child,
                    "child": child,
                    "scrollview": scrollview,
                    "proxy": proxy,
                    "scrollview_rect": origin_rect,
                    "rect": rect,
                    "visible": visible,
                    "child_index": entry["index"],
                })
        return contexts

    def _scrollview_focus_context_for_element(self, element):
        for proxy, scrollview, origin_rect in self._embedded_scrollview_contexts():
            for context in self._scrollview_child_focus_contexts(proxy, scrollview, origin_rect):
                if context["element"] is element:
                    return context
        for proxy in self._elements.values():
            if not self._is_direct_embedded_view(proxy):
                continue
            child = getattr(proxy, "_child_instance", None)
            if child is None or not hasattr(child, "_scrollview_focus_context_for_element"):
                continue
            child_context = child._scrollview_focus_context_for_element(element)
            if child_context is None:
                continue
            origin_rect = self._element_focus_rect_direct(proxy)
            translated = dict(child_context)
            rect = child_context["rect"]
            translated["rect"] = {
                "top": origin_rect["top"] + rect["top"],
                "left": origin_rect["left"] + rect["left"],
                "bottom": origin_rect["top"] + rect["bottom"],
                "right": origin_rect["left"] + rect["right"],
                "center_row": origin_rect["top"] * 2 + rect["center_row"],
                "center_col": origin_rect["left"] * 2 + rect["center_col"],
            }
            scroll_rect = child_context.get("scrollview_rect")
            if scroll_rect is not None:
                translated["scrollview_rect"] = {
                    "top": origin_rect["top"] + scroll_rect["top"],
                    "left": origin_rect["left"] + scroll_rect["left"],
                    "bottom": origin_rect["top"] + scroll_rect["bottom"],
                    "right": origin_rect["left"] + scroll_rect["right"],
                    "center_row": origin_rect["top"] * 2 + scroll_rect["center_row"],
                    "center_col": origin_rect["left"] * 2 + scroll_rect["center_col"],
                }
            return translated
        return None

    def _embedded_view_context_for_element(self, element, focusable_only=True):
        for context in self._embedded_view_contexts(focusable_only=focusable_only):
            if context["element"] is element:
                return context
        return None

    def _focus_rect_cache_for_elements(self, elements):
        element_ids = {id(elem) for elem in elements}
        rects = {}
        for proxy, scrollview, origin_rect in self._embedded_scrollview_contexts():
            for context in self._scrollview_child_focus_contexts(proxy, scrollview, origin_rect):
                if id(context["element"]) in element_ids:
                    rects[id(context["element"])] = dict(context["rect"])
        for context in self._embedded_view_contexts(focusable_only=True, resolve_owner=False):
            if id(context["element"]) in element_ids:
                rects[id(context["element"])] = dict(context["rect"])
        if self._active_scrollview_scope is not None:
            for context in self._scrollview_scope_contexts():
                if id(context["element"]) in element_ids:
                    rects[id(context["element"])] = dict(context["rect"])
        for elem in elements:
            rects.setdefault(id(elem), self._element_focus_rect_direct(elem))
        return rects

    @staticmethod
    def _rect_contains(rect, row, col):
        return (
            row >= rect["top"]
            and row < rect["bottom"]
            and col >= rect["left"]
            and col < rect["right"]
        )

    def _scrollview_context_at(self, row, col):
        for proxy, scrollview, origin_rect in reversed(self._embedded_scrollview_contexts()):
            if self._rect_contains(origin_rect, row, col):
                return {
                    "scrollview": scrollview,
                    "scrollview_rect": origin_rect,
                    "proxy": proxy,
                }
        for context in reversed(self._embedded_view_contexts(focusable_only=False)):
            elem = context["element"]
            if not self._is_scrollview_proxy(elem):
                continue
            rect = context["rect"]
            if self._rect_contains(rect, row, col):
                return {
                    "scrollview": elem._child_instance,
                    "scrollview_rect": rect,
                    "proxy": elem,
                }
        return None

    def _nested_focus_contexts(self, visible_only=False):
        contexts = []
        for proxy, scrollview, origin_rect in self._embedded_scrollview_contexts():
            contexts.extend(self._scrollview_child_focus_contexts(
                proxy,
                scrollview,
                origin_rect,
                visible_only=visible_only,
            ))
        embedded_contexts = self._embedded_view_contexts(focusable_only=True)
        contexts.extend(embedded_contexts)
        for context in embedded_contexts:
            view = context.get("view")
            if view is None or not hasattr(view, "_nested_focus_contexts"):
                continue
            origin_rect = self._element_focus_rect_direct(context["proxy"])
            for child_context in view._nested_focus_contexts(visible_only=visible_only):
                translated = dict(child_context)
                rect = child_context["rect"]
                translated["rect"] = {
                    "top": origin_rect["top"] + rect["top"],
                    "left": origin_rect["left"] + rect["left"],
                    "bottom": origin_rect["top"] + rect["bottom"],
                    "right": origin_rect["left"] + rect["right"],
                    "center_row": origin_rect["top"] * 2 + rect["center_row"],
                    "center_col": origin_rect["left"] * 2 + rect["center_col"],
                }
                scroll_rect = child_context.get("scrollview_rect")
                if scroll_rect is not None:
                    translated["scrollview_rect"] = {
                        "top": origin_rect["top"] + scroll_rect["top"],
                        "left": origin_rect["left"] + scroll_rect["left"],
                        "bottom": origin_rect["top"] + scroll_rect["bottom"],
                        "right": origin_rect["left"] + scroll_rect["right"],
                        "center_row": origin_rect["top"] * 2 + scroll_rect["center_row"],
                        "center_col": origin_rect["left"] * 2 + scroll_rect["center_col"],
                    }
                contexts.append(translated)
        return contexts

    def tick(self):
        """Called by the application loop on each idle timeout. Propagates to embedded child controls."""
        for elem in self._elements.values():
            if hasattr(elem, 'tick'):
                elem.tick()
            child = getattr(elem, '_child_instance', None)
            if child is not None:
                child.tick()

    def get_cell_rect(self, cell_name):
        """Get the resolved rectangle for a named structural cell."""
        rect = self._resolved_cells_by_name.get(cell_name)
        if rect is None and self._layout is not None:
            self._resolve_layout_geometry()
            rect = self._resolved_cells_by_name.get(cell_name)
        return dict(rect) if rect else None

    def get_focusable_elements(self):
        """Get all focusable elements in tab order (top-to-bottom, left-to-right)."""
        elems = []
        local_focusable = set(self._local_focusable_elements())
        embedded_contexts = None

        def get_embedded_contexts():
            nonlocal embedded_contexts
            if embedded_contexts is None:
                embedded_contexts = self._embedded_view_contexts(focusable_only=True, resolve_owner=False)
            return embedded_contexts

        ordered_elements = sorted(self._elements.values(), key=lambda e: (
            getattr(e, "_cell_row", 0) + getattr(e, "row", 0),
            getattr(e, "_cell_col", 0) + getattr(e, "col", 0),
        ))
        for elem in ordered_elements:
            if self._is_scrollview_proxy(elem):
                child = elem._child_instance
                origin_rect = self._element_focus_rect_direct(elem)
                if elem in local_focusable:
                    elems.append(elem)
                if self._active_scrollview_scope_for(child):
                    contexts = self._scrollview_child_focus_contexts(elem, child, origin_rect)
                    elems.extend(context["element"] for context in contexts)
                continue
            if self._is_direct_embedded_view(elem):
                child = getattr(elem, "_child_instance", None)
                embedded = [
                    context["element"]
                    for context in get_embedded_contexts()
                    if context.get("proxy") is elem
                ]
                if embedded:
                    elems.extend(embedded)
                elif elem in local_focusable and getattr(child, "focusable", False):
                    elems.append(elem)
                continue
            if elem in local_focusable:
                elems.append(elem)
        rects = self._focus_rect_cache_for_elements(elems)
        return sorted(elems, key=lambda e: (rects[id(e)]["top"], rects[id(e)]["left"]))

    def _has_single_focusable(self):
        """Return True if there is exactly one interactive (editable/activatable) element.

        UIElementReusable containers are excluded — they are not real focus
        targets from the window's perspective.  When True, focus should never
        be cleared so the single element always holds keyboard focus.
        """
        interactive = {"textinput", "textarea", "numberinput", "combobox", "listbox", "button", "checkbox"}
        count = sum(1 for e in self.get_focusable_elements() if getattr(e, "ELEMENT_TYPE", None) in interactive)
        return count == 1

    def _enter_edit_mode(self):
        if self._edit_mode:
            return
        if self._is_scrollview_proxy(self._focused_element):
            if self._enter_scrollview_scope(self._focused_element):
                return
        self._edit_mode = True
        self._edit_snapshot = self._snapshot_element_value(self._focused_element)
        if self._focused_element:
            self._focused_element._edit_mode = True
            if hasattr(self._focused_element, "begin_edit"):
                self._focused_element.begin_edit()
            if self._focused_element.ELEMENT_TYPE == "combobox":
                self._focused_element._menu_open = True
            elif self._focused_element.ELEMENT_TYPE == "numberinput":
                if getattr(self._focused_element, "value", None) == 0.0:
                    self._focused_element.select_range(0, len(self._focused_element.display_value))
                else:
                    self._focused_element.cursor_pos = len(self._focused_element.display_value)
                    self._focused_element._selection_start = None
            elif self._focused_element.ELEMENT_TYPE in ("textinput", "textarea"):
                self._focused_element.cursor_pos = len(getattr(self._focused_element, "value", "") or "")
                self._focused_element._selection_start = None
                if hasattr(self._focused_element, "_manual_scroll_away_from_cursor"):
                    self._focused_element._manual_scroll_away_from_cursor = False

    @staticmethod
    def _uses_leave_commit(elem):
        return getattr(elem, "commit_mode", "standard") == "leave"

    def _exit_edit_mode(self, commit=True, notify=True):
        if self._active_scrollview_scope is not None:
            scope = self._active_scrollview_scope
            elem = self._focused_element
            if elem is not None and elem is not scope["proxy"]:
                if getattr(elem, "_edit_mode", False):
                    self._end_scoped_child_edit(elem, commit=commit, notify=notify)
                elem._edit_mode = False
                elem.focused = False
                key = id(scope["scrollview"])
                self._scrollview_last_descendant[key] = elem
            self._clear_descendant_focus_state(scope["scrollview"])
            self._active_scrollview_scope = None
            self._edit_mode = False
            self._edit_snapshot = None
            self._focused_element = scope["proxy"]
            self._set_element_focus_state(self._focused_element, True)
            self._focused_element._edit_mode = False
            return
        elem = self._focused_element
        snapshot = self._edit_snapshot
        leave_commit = self._uses_leave_commit(elem) if elem is not None else False
        if not commit and elem is not None and snapshot is not None:
            self._restore_element_value(elem, snapshot)
        self._edit_mode = False
        self._edit_snapshot = None
        self._mouse_selection_element = None
        self._mouse_selection_anchor = None
        self._mouse_selection_changed = False
        if elem:
            if hasattr(elem, "end_edit"):
                elem.end_edit(commit=commit)
            elem._edit_mode = False
            if hasattr(elem, '_menu_open'):
                elem._menu_open = False
            notify = notify and not getattr(self, "_suppress_edit_notifications", False)
            if commit and notify and not leave_commit:
                self._dispatch_confirmed_for(elem, getattr(elem, "value", None))
            if commit and notify and snapshot is not None and self._element_value_changed(elem, snapshot):
                self._dispatch_element_changed_for(elem, getattr(elem, "value", True))

    def _snapshot_element_value(self, elem):
        if elem is None:
            return None
        elem_type = elem.ELEMENT_TYPE
        if elem_type in ("textinput", "textarea"):
            return {
                "value": elem.value,
                "cursor_pos": elem.cursor_pos,
                "selection_start": getattr(elem, "_selection_start", None),
            }
        if elem_type == "numberinput":
            return {"value": elem.value}
        if elem_type == "combobox":
            return {
                "selected_item": elem.selected_item,
                "menu_open": getattr(elem, "_menu_open", False),
            }
        if elem_type == "listbox":
            return {
                "selected_items": list(elem.selected_items),
                "scroll_offset": getattr(elem, "_scroll_offset", 0),
            }
        if elem_type == "checkbox":
            return {"value": elem.value}
        return {"value": getattr(elem, "value", None)}

    def _restore_element_value(self, elem, snapshot):
        elem_type = elem.ELEMENT_TYPE
        if elem_type in ("textinput", "textarea"):
            elem.value = snapshot["value"]
            elem.cursor_pos = snapshot["cursor_pos"]
            elem._selection_start = snapshot["selection_start"]
        elif elem_type == "numberinput":
            elem.value = snapshot["value"]
        elif elem_type == "combobox":
            elem.selected_item = snapshot["selected_item"]
            elem._menu_open = snapshot["menu_open"]
        elif elem_type == "listbox":
            elem.selected_items = snapshot["selected_items"]
            elem._scroll_offset = snapshot["scroll_offset"]
        elif elem_type == "checkbox":
            elem.value = snapshot["value"]

    def _element_value_changed(self, elem, snapshot):
        elem_type = elem.ELEMENT_TYPE
        if elem_type in ("textinput", "textarea"):
            return elem.value != snapshot["value"]
        if elem_type == "numberinput":
            return elem.value != snapshot["value"]
        if elem_type == "combobox":
            return elem.selected_item != snapshot["selected_item"]
        if elem_type == "listbox":
            return elem.selected_items != snapshot["selected_items"]
        if elem_type == "checkbox":
            return elem.value != snapshot["value"]
        return getattr(elem, "value", None) != snapshot.get("value")

    def set_focus(self, element):
        """Set focus to a specific element."""
        if element is not None and not getattr(element, "enabled", True):
            element = None
        self._suppress_edit_notifications = True
        try:
            self._exit_edit_mode()
        finally:
            self._suppress_edit_notifications = False
        if self._focused_element:
            previous_owner = self._event_owner_for_element(self._focused_element)
            if previous_owner is not self and getattr(previous_owner, "_focused_element", None) is self._focused_element:
                previous_owner._focused_element = None
            self._set_element_focus_state(self._focused_element, False)
        if element is not None:
            self._clear_pending_scrollview_proxy_focus_restore()
        self._clear_orphaned_reusable_focus(element)

        if element:
            self._set_element_focus_state(element, True)
            self._focused_element = element
            owner = self._event_owner_for_element(element)
            if owner is not self:
                owner._focused_element = element
            context = self._scrollview_focus_context_for_element(element)
            if context is not None:
                scrollview = context["scrollview"]
                self._scrollview_last_descendant[id(scrollview)] = element
                if hasattr(scrollview, "_focused"):
                    scrollview._focused = True
                clear_selected = getattr(scrollview, "_clear_selected_focus_child", None)
                if callable(clear_selected):
                    clear_selected()
                scrollview.ensure_element_visible(element)
            self._notify_focused(element, True)
        else:
            self._focused_element = None
            self._notify_focused(None, False)

    def _clear_orphaned_reusable_focus(self, active_element):
        for elem in self.get_all_elements():
            if getattr(elem, "ELEMENT_TYPE", None) != "uielement":
                continue
            child = getattr(elem, "_child_instance", None)
            if elem is active_element:
                if child is not None:
                    self._clear_descendant_focus_state(child)
                continue
            if child is not None and self._contains_descendant_element(child, active_element):
                continue
            elem.focused = False
            if child is not None:
                self._clear_descendant_focus_state(child)

    def _clear_pending_scrollview_proxy_focus_restore(self):
        def clear_view(view):
            clear_pending = getattr(view, "_clear_pending_proxy_focus_restore", None)
            if callable(clear_pending):
                clear_pending()
            get_children = getattr(view, "_get_children", None)
            if callable(get_children):
                for nested_child in get_children():
                    clear_view(nested_child)
            for nested in getattr(view, "_elements", {}).values():
                nested_child = getattr(nested, "_child_instance", None)
                if nested_child is not None:
                    clear_view(nested_child)

        for elem in self.get_all_elements():
            child = getattr(elem, "_child_instance", None)
            if child is not None:
                clear_view(child)

    @classmethod
    def _contains_descendant_element(cls, view, element):
        if view is element:
            return True
        get_children = getattr(view, "_get_children", None)
        if callable(get_children):
            for child in get_children():
                if cls._contains_descendant_element(child, element):
                    return True
        for child in getattr(view, "_elements", {}).values():
            if child is element:
                return True
            nested = getattr(child, "_child_instance", None)
            if nested is not None and cls._contains_descendant_element(nested, element):
                return True
        return False

    def _active_scrollview_scope_for(self, scrollview):
        return (
            self._active_scrollview_scope is not None
            and self._active_scrollview_scope.get("scrollview") is scrollview
        )

    def _scrollview_proxy_rect(self, proxy):
        if proxy in self._elements.values():
            return self._element_focus_rect_direct(proxy)
        context = self._embedded_view_context_for_element(proxy, focusable_only=True)
        if context is not None:
            return context["rect"]
        for nested in self._nested_focus_contexts(visible_only=False):
            if nested.get("element") is proxy:
                return nested["rect"]
        return self._element_focus_rect_direct(proxy)

    def _set_element_focus_state(self, element, focused):
        if element is None:
            return
        element.focused = focused
        if hasattr(element, "_focused"):
            element._focused = focused
        if focused and getattr(element, "ELEMENT_TYPE", None) == "uielement":
            child = getattr(element, "_child_instance", None)
            if child is None and hasattr(element, "_ensure_child"):
                child = element._ensure_child()
            if (
                child is not None
                and hasattr(child, "child_view_entries")
                and hasattr(child, "_clamped_viewport_rect")
            ):
                child._focused = True
        if not focused and getattr(element, "ELEMENT_TYPE", None) == "uielement":
            child = getattr(element, "_child_instance", None)
            if child is not None:
                self._clear_descendant_focus_state(child)

    @classmethod
    def _clear_descendant_focus_state(cls, view):
        if hasattr(view, "_focused"):
            view._focused = False
        clear_selected = getattr(view, "_clear_selected_focus_child", None)
        if callable(clear_selected):
            clear_selected()
        if hasattr(view, "_focused_element"):
            focused = getattr(view, "_focused_element", None)
            if focused is not None:
                focused.focused = False
            view._focused_element = None
        if hasattr(view, "_edit_mode"):
            view._edit_mode = False
        get_children = getattr(view, "_get_children", None)
        if callable(get_children):
            for child in get_children():
                cls._clear_descendant_focus_state(child)
        for child in getattr(view, "_elements", {}).values():
            child.focused = False
            nested = getattr(child, "_child_instance", None)
            if nested is not None:
                cls._clear_descendant_focus_state(nested)

    def _enter_scrollview_scope(self, proxy):
        if not self._is_scrollview_proxy(proxy):
            return False
        scrollview = proxy._child_instance
        origin_rect = self._scrollview_proxy_rect(proxy)
        contexts = self._scrollview_child_focus_contexts(proxy, scrollview, origin_rect)
        if not contexts:
            self._edit_mode = True
            self._edit_snapshot = None
            self._active_scrollview_scope = {
                "proxy": proxy,
                "scrollview": scrollview,
                "scrollview_rect": origin_rect,
            }
            self._set_element_focus_state(proxy, True)
            return True
        key = id(scrollview)
        remembered = self._scrollview_last_descendant.get(key)
        target_context = None
        if remembered is not None:
            for context in contexts:
                if context["element"] is remembered:
                    target_context = context
                    break
        if target_context is None:
            visible = [context for context in contexts if context.get("visible")]
            target_context = (visible or contexts)[0]
        self._set_element_focus_state(proxy, True)
        self._active_scrollview_scope = {
            "proxy": proxy,
            "scrollview": scrollview,
            "scrollview_rect": origin_rect,
        }
        self._edit_mode = True
        self._edit_snapshot = None
        target = target_context["element"]
        target.focused = True
        mark_selected = getattr(scrollview, "_mark_selected_focus_child", None)
        if callable(mark_selected):
            mark_selected(target_context.get("child") or owner)
        self._focused_element = target
        owner = target_context.get("owner")
        if owner is not None and owner is not self:
            owner._focused_element = target
        scrollview.ensure_element_visible(target)
        self._scrollview_last_descendant[key] = target
        return True

    def _scrollview_scope_contexts(self):
        scope = self._active_scrollview_scope
        if scope is None:
            return []
        proxy = scope["proxy"]
        scrollview = scope["scrollview"]
        origin_rect = scope.get("scrollview_rect") or self._element_focus_rect_direct(proxy)
        return self._scrollview_child_focus_contexts(
            proxy,
            scrollview,
            origin_rect,
        )

    def _move_scrollview_scope_focus(self, direction):
        scope = self._active_scrollview_scope
        if scope is None:
            return False
        contexts = self._scrollview_scope_contexts()
        if not contexts:
            if direction == "Up":
                return scope["scrollview"].handle_key("Up")
            if direction == "Down":
                return scope["scrollview"].handle_key("Down")
            return False
        current = self._focused_element
        current_context = None
        for context in contexts:
            if context["element"] is current:
                current_context = context
                break
        if current_context is None:
            current_context = contexts[0]
        navigation_contexts = contexts
        candidates = []
        for order, context in enumerate(navigation_contexts):
            if context is current_context:
                continue
            score = self._directional_focus_score(current_context["rect"], context["rect"], direction)
            if score is not None:
                candidates.append((score, order, context))
        if candidates:
            candidates.sort(key=lambda item: (item[0], item[1]))
            target_context = candidates[0][2]
            if self._focused_element is not None:
                owner = self._event_owner_for_element(self._focused_element)
                if owner is not self and getattr(owner, "_focused_element", None) is self._focused_element:
                    owner._focused_element = None
                self._focused_element.focused = False
            target = target_context["element"]
            target.focused = True
            mark_selected = getattr(scope["scrollview"], "_mark_selected_focus_child", None)
            if callable(mark_selected):
                mark_selected(target_context.get("child") or target_context.get("owner"))
            self._focused_element = target
            owner = target_context.get("owner")
            if owner is not None and owner is not self:
                owner._focused_element = target
            scope["scrollview"].ensure_element_visible(target)
            self._scrollview_last_descendant[id(scope["scrollview"])] = target
            return True
        delta = 1 if direction == "Up" else -1 if direction == "Down" else 0
        if delta and scope["scrollview"].scroll_lines(delta):
            self._queue_scrollview_terminal_scroll(
                scope["scrollview"],
                scope.get("scrollview_rect") or self._element_focus_rect(scope["proxy"]),
            )
            refreshed = self._scrollview_scope_contexts()
            visible = [context for context in refreshed if context.get("visible")]
            current_context = None
            for context in visible:
                if context["element"] is current:
                    current_context = context
                    break
            target_context = None
            if current_context is not None:
                candidates = []
                for order, context in enumerate(visible):
                    if context is current_context:
                        continue
                    score = self._directional_focus_score(current_context["rect"], context["rect"], direction)
                    if score is not None:
                        candidates.append((score, order, context))
                if candidates:
                    candidates.sort(key=lambda item: (item[0], item[1]))
                    target_context = candidates[0][2]
            elif visible:
                target_context = visible[0] if direction == "Down" else visible[-1]
            if target_context is not None:
                if self._focused_element is not None:
                    owner = self._event_owner_for_element(self._focused_element)
                    if owner is not self and getattr(owner, "_focused_element", None) is self._focused_element:
                        owner._focused_element = None
                    self._focused_element.focused = False
                target = target_context["element"]
                target.focused = True
                mark_selected = getattr(scope["scrollview"], "_mark_selected_focus_child", None)
                if callable(mark_selected):
                    mark_selected(target_context.get("child") or target_context.get("owner"))
                self._focused_element = target
                owner = target_context.get("owner")
                if owner is not None and owner is not self:
                    owner._focused_element = target
                self._scrollview_last_descendant[id(scope["scrollview"])] = target
            return True
        return False

    def _active_scrollview_scope_rect(self):
        scope = self._active_scrollview_scope
        if scope is None:
            return None
        return scope.get("scrollview_rect") or self._element_focus_rect(scope["proxy"])

    def _active_scrollview_scope_dim_background(self):
        scope = self._active_scrollview_scope
        if scope is None or not self._edit_mode:
            return None
        style = getattr(scope["proxy"], "style", None)
        dim_background = getattr(style, "scope_dim_background", None) if style is not None else None
        if dim_background is not None:
            return dim_background
        scrollview_style = getattr(scope["scrollview"], "style", None)
        return getattr(scrollview_style, "scope_dim_background", None) if scrollview_style is not None else None

    def _active_scrollview_scope_focus_background(self):
        scope = self._active_scrollview_scope
        if scope is None:
            return None
        focus_style = getattr(scope["proxy"], "focus_style", None)
        focus_background = getattr(focus_style, "background", None) if focus_style is not None else None
        if focus_background is not None:
            if (
                getattr(scope["proxy"], "ELEMENT_TYPE", None) == "uielement"
                and getattr(focus_background, "alpha", 1.0) < 1.0
            ):
                return None
            return focus_background
        scrollview_focus_style = getattr(scope["scrollview"], "focus_style", None)
        return getattr(scrollview_focus_style, "background", None) if scrollview_focus_style is not None else None

    def _ancestor_active_scrollview_scope_proxy(self):
        owner = getattr(self, "parent", None)
        while owner is not None:
            scope = getattr(owner, "_active_scrollview_scope", None)
            if scope is not None and scope.get("scrollview") is self:
                return scope.get("proxy")
            owner = getattr(owner, "parent", None)
        return None

    def _ancestor_active_scrollview_focus_background(self):
        proxy = self._ancestor_active_scrollview_scope_proxy()
        if proxy is None:
            return None
        focus_style = getattr(proxy, "focus_style", None)
        focus_background = getattr(focus_style, "background", None) if focus_style is not None else None
        if (
            focus_background is not None
            and getattr(proxy, "ELEMENT_TYPE", None) == "uielement"
            and getattr(focus_background, "alpha", 1.0) < 1.0
        ):
            return None
        return focus_background

    def _begin_scoped_child_edit(self, elem):
        self._edit_snapshot = self._snapshot_element_value(elem)
        elem._edit_mode = True
        if hasattr(elem, "begin_edit"):
            elem.begin_edit()
        if elem.ELEMENT_TYPE == "combobox":
            elem._menu_open = True
        elif elem.ELEMENT_TYPE == "numberinput":
            if getattr(elem, "value", None) == 0.0:
                elem.select_range(0, len(elem.display_value))
            else:
                elem.cursor_pos = len(elem.display_value)
                elem._selection_start = None
        elif elem.ELEMENT_TYPE in ("textinput", "textarea"):
            elem.cursor_pos = len(getattr(elem, "value", "") or "")
            elem._selection_start = None

    def _end_scoped_child_edit(self, elem, commit=True, notify=True):
        if elem is None or not getattr(elem, "_edit_mode", False):
            return
        snapshot = self._edit_snapshot
        leave_commit = self._uses_leave_commit(elem)
        if not commit and not leave_commit and snapshot is not None:
            self._restore_element_value(elem, snapshot)
        if hasattr(elem, "end_edit"):
            elem.end_edit(commit=commit)
        elem._edit_mode = False
        if hasattr(elem, "_menu_open"):
            elem._menu_open = False
        self._edit_snapshot = None
        if hasattr(elem, "_mark_dirty"):
            elem._mark_dirty()
        notify = notify and not getattr(self, "_suppress_edit_notifications", False)
        if commit and notify and not leave_commit:
            self._dispatch_confirmed_for(elem, getattr(elem, "value", None))
        if commit and notify and snapshot is not None and self._element_value_changed(elem, snapshot):
            self._dispatch_element_changed_for(elem, getattr(elem, "value", True))

    def _handle_scrollview_scope_key(self, key):
        if self._active_scrollview_scope is None:
            return False
        elem = self._focused_element
        if elem is not None and getattr(elem, "_edit_mode", False):
            if key == "Escape":
                self._end_scoped_child_edit(elem, commit=self._uses_leave_commit(elem))
                return True
            if key == "Enter":
                handled = self._handle_focused_element_key(key)
                self._end_scoped_child_edit(elem, commit=True)
                return handled or True
            if key in ("Up", "Down", "Left", "Right") and elem.ELEMENT_TYPE in ("textinput", "textarea", "numberinput", "combobox", "listbox"):
                handled = self._handle_focused_element_key(key)
                if elem.ELEMENT_TYPE == "combobox" and not getattr(elem, "_menu_open", True):
                    self._end_scoped_child_edit(elem, commit=True)
                return handled
            return self._handle_focused_element_key(key)
        if key in ("Up", "Down", "Left", "Right"):
            return self._move_scrollview_scope_focus(key)
        if key in ("Enter", " "):
            if elem is None:
                return True
            elem_type = elem.ELEMENT_TYPE
            if elem_type == "button":
                self._delay_dialog_button_action()
                self._dispatch_element_changed_for(elem, True)
                return True
            if elem_type == "checkbox":
                elem.handle_key(key)
                self._dispatch_element_changed_for(elem, elem.value)
                return True
            if elem_type == "image":
                self._dispatch_element_changed_for(elem, True)
                return True
            if elem_type in ("textinput", "textarea", "numberinput", "combobox", "listbox"):
                self._begin_scoped_child_edit(elem)
                return True
        if key == "Escape":
            self._exit_edit_mode()
            return True
        return True

    def _event_owner_for_element(self, element):
        context = self._scrollview_focus_context_for_element(element)
        if context is not None:
            return context["owner"]
        context = self._embedded_view_context_for_element(element)
        return context["owner"] if context is not None else self

    def _dispatch_element_changed_for(self, element, value):
        owner = self._event_owner_for_element(element)
        if owner is self:
            self._dispatch_element_changed(element, value)
            return
        owner._dispatch_element_changed(element, value)

    def _dispatch_confirmed_for(self, element, value):
        owner = self._event_owner_for_element(element)
        if owner is self:
            self._dispatch_confirmed(element, value)
            return
        owner._dispatch_confirmed(element, value)

    def _dispatch_selection_changed_for(self, element, value):
        owner = self._event_owner_for_element(element)
        if owner is self:
            self._dispatch_selection_changed(element, value)
            return
        owner._dispatch_selection_changed(element, value)

    def _is_activatable_reusable_element(self, element):
        if element is None or not getattr(element, "enabled", True) or getattr(element, "ELEMENT_TYPE", None) != "uielement":
            return False
        child = getattr(element, "_child_instance", None)
        if child is None and hasattr(element, "_sync_child"):
            child = element._sync_child(open_child=True)
        return child is not None and getattr(child, "focusable", False) and callable(getattr(child, "activate", None))

    def _activate_reusable_element(self, element):
        if not self._is_activatable_reusable_element(element):
            return False
        child = getattr(element, "_child_instance", None)
        if child is None and hasattr(element, "_sync_child"):
            child = element._sync_child(open_child=True)
        activate = getattr(child, "activate", None)
        return bool(activate())

    def _activate_element(self, element):
        if element is None or not getattr(element, "enabled", True):
            return False
        elem_type = getattr(element, "ELEMENT_TYPE", None)
        if elem_type == "button":
            self._delay_dialog_button_action()
            self._dispatch_element_changed_for(element, True)
            return True
        if elem_type == "checkbox":
            element.handle_key("Enter")
            self._dispatch_element_changed_for(element, element.value)
            return True
        if elem_type == "image":
            self._dispatch_element_changed_for(element, True)
            return True
        if elem_type == "uielement" and self._activate_reusable_element(element):
            return True
        return False

    def next_focus(self):
        """Move focus to the next focusable element."""
        focusable = self.get_focusable_elements()
        if not focusable:
            return

        current_idx = -1
        if self._focused_element:
            try:
                current_idx = focusable.index(self._focused_element)
            except ValueError:
                current_idx = -1

        next_idx = (current_idx + 1) % len(focusable)
        self.set_focus(focusable[next_idx])

    def prev_focus(self):
        """Move focus to the previous focusable element."""
        focusable = self.get_focusable_elements()
        if not focusable:
            return

        current_idx = -1
        if self._focused_element:
            try:
                current_idx = focusable.index(self._focused_element)
            except ValueError:
                current_idx = -1

        next_idx = (current_idx - 1) % len(focusable)
        self.set_focus(focusable[next_idx])

    def move_focus_direction(self, direction):
        """Move focus to the nearest element in a visual direction."""
        focusable = self.get_focusable_elements()
        if not focusable:
            return False
        rects = self._focus_rect_cache_for_elements(focusable)
        if self._focused_element not in focusable:
            if self._focused_element is None:
                self.set_focus(focusable[0])
                return True
            current_rect = self._element_focus_rect(self._focused_element)
        else:
            current_rect = rects.get(id(self._focused_element)) or self._element_focus_rect(self._focused_element)
        candidates = []
        for order, elem in enumerate(focusable):
            if elem is self._focused_element:
                continue
            score = self._directional_focus_score(current_rect, rects.get(id(elem)) or self._element_focus_rect(elem), direction)
            if score is not None:
                candidates.append((score, order, elem))

        if not candidates:
            return False

        candidates.sort(key=lambda item: (item[0], item[1]))
        self.set_focus(candidates[0][2])
        return True

    def _element_focus_rect_direct(self, elem):
        row = getattr(elem, "_cell_row", 0) + getattr(elem, "row", 0)
        col = getattr(elem, "_cell_col", 0) + getattr(elem, "col", 0)
        width = max(1, int(getattr(elem, "width", None) or len(getattr(elem, "title", "")) or 1))
        height = max(1, int(getattr(elem, "height", None) or 1))
        return {
            "top": row,
            "left": col,
            "bottom": row + height,
            "right": col + width,
            "center_row": row * 2 + height,
            "center_col": col * 2 + width,
        }

    def _element_focus_rect(self, elem):
        context = self._scrollview_focus_context_for_element(elem)
        if context is not None:
            return dict(context["rect"])
        context = self._embedded_view_context_for_element(elem, focusable_only=False)
        if context is not None:
            return dict(context["rect"])
        return self._element_focus_rect_direct(elem)

    @staticmethod
    def _axis_gap(start_a, end_a, start_b, end_b):
        if end_a <= start_b:
            return start_b - end_a
        if end_b <= start_a:
            return start_a - end_b
        return 0

    @staticmethod
    def _axis_overlaps(start_a, end_a, start_b, end_b):
        return max(start_a, start_b) < min(end_a, end_b)

    @classmethod
    def _band_scan_rank(cls, current_start, current_end, candidate_start, candidate_end):
        if cls._axis_overlaps(current_start, current_end, candidate_start, candidate_end):
            return 0

        max_distance = (
            max(
                abs(candidate_start - current_start),
                abs(candidate_end - current_end),
                abs(candidate_start - current_end),
                abs(candidate_end - current_start),
            )
            + max(1, current_end - current_start)
            + max(1, candidate_end - candidate_start)
        )
        rank = 1
        for distance in range(1, max_distance + 1):
            for offset in (distance, -distance):
                if cls._axis_overlaps(
                    current_start + offset,
                    current_end + offset,
                    candidate_start,
                    candidate_end,
                ):
                    return rank
                rank += 1
        return None

    @classmethod
    def _directional_focus_score(cls, current, candidate, direction):
        if direction == "Right":
            if candidate["center_col"] <= current["center_col"]:
                return None
            # Candidate's left edge must not be before source's right edge.
            if candidate["left"] < current["right"]:
                return None
            if not cls._axis_overlaps(current["top"], current["bottom"], candidate["top"], candidate["bottom"]):
                return None
            primary_gap = max(0, candidate["left"] - current["right"])
            band_rank = cls._band_scan_rank(
                current["top"], current["bottom"], candidate["top"], candidate["bottom"])
            if band_rank is None:
                return None
            perpendicular_gap = cls._axis_gap(
                current["top"], current["bottom"], candidate["top"], candidate["bottom"])
            edge_delta = abs(candidate["top"] - current["top"])
        elif direction == "Left":
            if candidate["center_col"] >= current["center_col"]:
                return None
            # Candidate's right edge must not extend past source's left edge.
            if candidate["right"] > current["left"]:
                return None
            if not cls._axis_overlaps(current["top"], current["bottom"], candidate["top"], candidate["bottom"]):
                return None
            primary_gap = max(0, current["left"] - candidate["right"])
            band_rank = cls._band_scan_rank(
                current["top"], current["bottom"], candidate["top"], candidate["bottom"])
            if band_rank is None:
                return None
            perpendicular_gap = cls._axis_gap(
                current["top"], current["bottom"], candidate["top"], candidate["bottom"])
            edge_delta = abs(candidate["top"] - current["top"])
        elif direction == "Down":
            if candidate["center_row"] <= current["center_row"]:
                return None
            # Candidate's top edge must not be above source's bottom edge.
            if candidate["top"] < current["bottom"]:
                return None
            primary_gap = max(0, candidate["top"] - current["bottom"])
            band_rank = cls._band_scan_rank(
                current["left"], current["right"], candidate["left"], candidate["right"])
            if band_rank is None:
                return None
            perpendicular_gap = cls._axis_gap(
                current["left"], current["right"], candidate["left"], candidate["right"])
            edge_delta = abs(candidate["left"] - current["left"])
        elif direction == "Up":
            if candidate["center_row"] >= current["center_row"]:
                return None
            # Candidate's bottom edge must not extend below source's top edge.
            if candidate["bottom"] > current["top"]:
                return None
            primary_gap = max(0, current["top"] - candidate["bottom"])
            band_rank = cls._band_scan_rank(
                current["left"], current["right"], candidate["left"], candidate["right"])
            if band_rank is None:
                return None
            perpendicular_gap = cls._axis_gap(
                current["left"], current["right"], candidate["left"], candidate["right"])
            edge_delta = abs(candidate["left"] - current["left"])
        else:
            return None

        return (
            band_rank,
            primary_gap,
            perpendicular_gap,
            edge_delta,
        )

    def _element_mouse_rect(self, elem):
        rect = self._element_focus_rect(elem)
        if elem.ELEMENT_TYPE == "combobox":
            rect = dict(rect)
            rect["bottom"] = rect["top"] + (
                max(1, len(getattr(elem, "_options", [])) + 1)
                if getattr(elem, "_menu_open", False)
                else 1
            )
        return rect

    def _element_at(self, row, col):
        for context in reversed(self._nested_focus_contexts(visible_only=True)):
            elem = context["element"]
            if getattr(elem, "ELEMENT_TYPE", None) != "combobox" or not getattr(elem, "_menu_open", False):
                continue
            rect = self._element_mouse_rect(elem)
            if row >= rect["top"] and row < rect["bottom"] and col >= rect["left"] and col < rect["right"]:
                return elem, rect

        for elem in reversed(self.get_all_elements()):
            if elem.ELEMENT_TYPE != "combobox" or not getattr(elem, "_menu_open", False):
                continue
            rect = self._element_mouse_rect(elem)
            if row >= rect["top"] and row < rect["bottom"] and col >= rect["left"] and col < rect["right"]:
                return elem, rect

        for context in reversed(self._nested_focus_contexts(visible_only=True)):
            elem = context["element"]
            rect = self._element_mouse_rect(elem)
            if row >= rect["top"] and row < rect["bottom"] and col >= rect["left"] and col < rect["right"]:
                return elem, rect

        for elem in reversed(self.get_all_elements()):
            if elem.ELEMENT_TYPE in ("label", "spanlabel", "infolabel", "messagetable"):
                continue
            rect = self._element_mouse_rect(elem)
            if row >= rect["top"] and row < rect["bottom"] and col >= rect["left"] and col < rect["right"]:
                return elem, rect
        return None, None

    @staticmethod
    def _element_allows_text_selection(elem):
        if elem is None or elem.ELEMENT_TYPE not in ("label", "spanlabel"):
            return False
        style = elem.get_style()
        return getattr(style, "user_select", None) == USER_SELECT_TEXT

    def _copyable_label_at(self, row, col):
        for elem in reversed(self.get_all_elements()):
            if not self._element_allows_text_selection(elem):
                continue
            rect = self._element_mouse_rect(elem)
            if row >= rect["top"] and row < rect["bottom"] and col >= rect["left"] and col < rect["right"]:
                return elem, rect
        return None, None

    def _copyable_labels(self):
        labels = [elem for elem in self.get_all_elements() if self._element_allows_text_selection(elem)]
        labels.extend(
            context["element"]
            for context in self._embedded_view_contexts(focusable_only=False)
            if self._element_allows_text_selection(context["element"])
        )
        return sorted(labels, key=lambda elem: (
            self._element_mouse_rect(elem)["top"],
            self._element_mouse_rect(elem)["left"],
        ))

    def _clear_label_selections(self):
        for elem in self.get_all_elements():
            if elem.ELEMENT_TYPE in ("label", "spanlabel") and hasattr(elem, "clear_selection"):
                elem.clear_selection()
            child = getattr(elem, '_child_instance', None)
            if child is not None:
                if hasattr(child, '_clear_selections'):
                    child._clear_selections()
                else:
                    child._clear_label_selections()

    def _label_text_position_from_mouse(self, elem, row, col, rect):
        rel_row = max(0, row - rect["top"])
        rel_col = max(0, col - rect["left"])
        return elem.text_position_from_point(rel_row, rel_col)

    def _label_selection_endpoint_at(self, row, col, allow_outside=False):
        labels = self._copyable_labels()
        if not labels:
            return None

        for label in labels:
            rect = self._element_mouse_rect(label)
            if (
                row >= rect["top"]
                and row < rect["bottom"]
                and (allow_outside or (col >= rect["left"] and col < rect["right"]))
            ):
                rel_row = row - rect["top"]
                rel_col = col - rect["left"]
                return label, label.text_position_from_point(rel_row, rel_col)

        if not allow_outside:
            return None

        first_label = labels[0]
        first_rect = self._element_mouse_rect(first_label)
        if row < first_rect["top"]:
            return first_label, 0

        last_label = labels[-1]
        last_rect = self._element_mouse_rect(last_label)
        if row >= last_rect["bottom"]:
            return last_label, len(getattr(last_label, "text", "") or "")

        previous = None
        for label in labels:
            rect = self._element_mouse_rect(label)
            if row < rect["top"]:
                if previous is None:
                    return label, 0
                return previous, len(getattr(previous, "text", "") or "")
            previous = label

        return last_label, len(getattr(last_label, "text", "") or "")

    def _first_label_selection_endpoint(self):
        labels = self._copyable_labels()
        if not labels:
            return None
        return labels[0], 0

    def _last_label_selection_endpoint(self):
        labels = self._copyable_labels()
        if not labels:
            return None
        label = labels[-1]
        return label, len(getattr(label, "text", "") or "")

    def _set_label_selection_from_endpoints(self, anchor, target):
        if anchor is None or target is None:
            return False
        anchor_elem, anchor_pos = anchor
        target_elem, target_pos = target
        return self._set_label_selection(anchor_elem, anchor_pos, target_elem, target_pos)

    def _set_label_selection(self, anchor_elem, anchor_pos, target_elem, target_pos):
        labels = self._copyable_labels()
        if anchor_elem not in labels or target_elem not in labels:
            return False

        self._clear_label_selections()
        anchor_idx = labels.index(anchor_elem)
        target_idx = labels.index(target_elem)
        if (target_idx, target_pos) < (anchor_idx, anchor_pos):
            anchor_elem, target_elem = target_elem, anchor_elem
            anchor_pos, target_pos = target_pos, anchor_pos
            anchor_idx, target_idx = target_idx, anchor_idx

        for idx, label in enumerate(labels):
            if idx < anchor_idx or idx > target_idx:
                continue
            text_len = len(getattr(label, "text", "") or "")
            if anchor_idx == target_idx:
                label.select_range(anchor_pos, target_pos)
            elif idx == anchor_idx:
                label.select_range(anchor_pos, text_len)
            elif idx == target_idx:
                label.select_range(0, target_pos)
            else:
                label.select_range(0, text_len)
        return True

    def _selected_label_text(self):
        parts = [
            selected
            for selected in (label.selected_text() for label in self._copyable_labels())
            if selected
        ]
        return "\n".join(parts)

    def _copy_label_selection(self):
        text = self._selected_label_text()
        if not text:
            return False
        _copy_text_to_clipboard(text)
        for label in self._copyable_labels():
            if label.selected_text():
                _notify_copied(label)
                break
        return True

    def _text_cursor_from_mouse(self, elem, row, col, rect):
        width = max(1, int(getattr(elem, "width", None) or 1))
        rel_row = max(0, row - rect["top"])
        rel_col = max(0, min(width, col - rect["left"]))
        if elem.ELEMENT_TYPE in ("textinput", "textarea"):
            if hasattr(elem, "cursor_position_from_point"):
                return elem.cursor_position_from_point(rel_row, rel_col)
            if elem.ELEMENT_TYPE == "textarea" and (getattr(elem, "height", 1) or 1) > 1:
                rows = _build_visual_rows(elem.value, width)
                scroll = getattr(elem, "_scroll_offset", 0)
                row_idx = min(rel_row + scroll, len(rows) - 1)
                rstart, rtext = rows[row_idx]
                return min(len(elem.value), rstart + min(rel_col, len(rtext)))
            return min(len(elem.value), getattr(elem, "_scroll_offset", 0) + rel_col)
        if elem.ELEMENT_TYPE == "numberinput":
            return min(len(elem.display_value), rel_col)
        return rel_col

    @staticmethod
    def _mouse_event_for_rect(event, rect):
        translated = dict(event)
        translated["row"] = event.get("row", 0) - rect["top"]
        translated["col"] = event.get("col", 0) - rect["left"]
        return translated

    def _queue_scrollview_terminal_scroll(self, scrollview, rect):
        app = getattr(self, "_app", None)
        if app is None or not hasattr(app, "queue_terminal_scroll"):
            return
        consume = getattr(scrollview, "consume_terminal_scroll_delta", None)
        if not callable(consume):
            return
        delta = int(consume() or 0)
        if delta == 0 or not rect:
            return
        app.queue_terminal_scroll({
            "row": int(rect["top"]),
            "col": int(rect["left"]),
            "height": max(0, int(rect["bottom"]) - int(rect["top"])),
            "width": max(0, int(rect["right"]) - int(rect["left"])),
            "delta": delta,
        })

    def _select_combo_item_from_mouse(self, elem, row, rect):
        option_idx = row - rect["top"] - 1
        if option_idx >= 0 and option_idx < len(elem._options):
            previous = getattr(elem, "selected_item", None)
            elem.selected_item = elem._options[option_idx]
            self._exit_edit_mode(commit=True)
            if elem.selected_item != previous:
                self._dispatch_selection_changed_for(elem, elem.selected_item)
            return True
        if self._active_scrollview_scope is not None:
            self._begin_scoped_child_edit(elem)
        else:
            self._enter_edit_mode()
        return True

    def _mouse_event_time(self, event):
        return float(event.get("time", time.monotonic()))

    def _select_list_item_from_mouse(self, elem, row, rect, event=None):
        option_idx = getattr(elem, "_scroll_offset", 0) + row - rect["top"]
        if option_idx < 0 or option_idx >= len(elem._options):
            return False
        click_time = self._mouse_event_time(event or {})
        previous_click = self._last_listbox_click
        is_double_click = (
            previous_click is not None
            and previous_click[0] is elem
            and previous_click[1] == option_idx
            and click_time - previous_click[2] <= LISTBOX_DOUBLE_CLICK_SECONDS
        )
        self._last_listbox_click = (elem, option_idx, click_time)
        before = list(elem.selected_items or [])
        if elem.multiple:
            item = elem._options[option_idx]
            if item in elem.selected_items:
                elem.selected_items = [selected for selected in elem.selected_items if selected != item]
            else:
                elem.selected_items = elem.selected_items + [item]
        else:
            elem.selected_items = [elem._options[option_idx]]
        elem._ensure_selection_visible(option_idx)
        after = list(elem.selected_items or [])
        if after != before:
            self._dispatch_selection_changed_for(elem, after)
        if self._active_scrollview_scope is not None:
            self._begin_scoped_child_edit(elem)
        else:
            self._enter_edit_mode()
        if is_double_click:
            self._dispatch_confirmed_for(elem, getattr(elem, "value", None))
        return True

    def _handle_mouse_event(self, event):
        if event.get("button") != MOUSE_LEFT_BUTTON:
            return False

        row = event.get("row", 0)
        col = event.get("col", 0)
        event_type = event.get("event")

        if self._app is not None:
            self._app.mark_dirty()

        if event_type == "drag":
            if self._mouse_click_candidate_element is not None:
                self._mouse_click_candidate_moved = True
                return True
            elem = self._mouse_selection_element
            if elem is None:
                return False
            if elem.ELEMENT_TYPE == "uielement":
                rect = self._element_mouse_rect(elem)
                return elem.handle_key(self._mouse_event_for_rect(event, rect))
            if elem.ELEMENT_TYPE in ("label", "spanlabel"):
                target = self._label_selection_endpoint_at(row, col, allow_outside=True)
                if target is None:
                    return False
                self._mouse_selection_changed = target != self._mouse_selection_anchor
                return self._set_label_selection_from_endpoints(self._mouse_selection_anchor, target)
            if elem.ELEMENT_TYPE not in ("textinput", "textarea", "numberinput"):
                return False
            rect = self._element_mouse_rect(elem)
            if elem.ELEMENT_TYPE == "textarea" and (getattr(elem, "height", 1) or 1) > 1:
                if row < rect["top"]:
                    if hasattr(elem, "scroll_by_rows"):
                        elem.scroll_by_rows(-1, manual=False)
                    row = rect["top"]
                elif row >= rect["bottom"]:
                    if hasattr(elem, "scroll_by_rows"):
                        elem.scroll_by_rows(1, manual=False)
                    row = rect["bottom"] - 1
            cursor_pos = self._text_cursor_from_mouse(elem, row, col, rect)
            elem.select_range(self._mouse_selection_anchor, cursor_pos)
            self._mouse_selection_changed = cursor_pos != self._mouse_selection_anchor
            return True

        if event_type == "release":
            if self._mouse_click_candidate_element is not None:
                candidate = self._mouse_click_candidate_element
                moved = self._mouse_click_candidate_moved
                self._mouse_click_candidate_element = None
                self._mouse_click_candidate_moved = False
                target, _ = self._element_at(row, col)
                if not moved and target is candidate:
                    self._activate_element(candidate)
                return True
            elem = self._mouse_selection_element
            if elem is not None and elem.ELEMENT_TYPE == "uielement":
                rect = self._element_mouse_rect(elem)
                elem.handle_key(self._mouse_event_for_rect(event, rect))
            elif elem is not None and elem.ELEMENT_TYPE in ("label", "spanlabel") and self._mouse_selection_changed:
                self._copy_label_selection()
            elif elem is not None and hasattr(elem, "copy_selection") and self._mouse_selection_changed:
                elem.copy_selection()
            self._mouse_selection_element = None
            self._mouse_selection_anchor = None
            self._mouse_selection_changed = False
            return True

        elem, rect = self._element_at(row, col)
        if elem is None:
            endpoint = self._label_selection_endpoint_at(row, col, allow_outside=False)
            if endpoint is not None:
                if not self._has_single_focusable():
                    self.set_focus(None)
                self._clear_label_selections()
                label, cursor_pos = endpoint
                label.select_range(cursor_pos, cursor_pos)
                self._mouse_selection_element = label
                self._mouse_selection_anchor = endpoint
                self._mouse_selection_changed = False
                return True
            self._clear_label_selections()
            if not self._has_single_focusable():
                self.set_focus(None)
            self._mouse_click_candidate_element = None
            self._mouse_click_candidate_moved = False
            return False

        if not getattr(elem, "enabled", True):
            if not self._has_single_focusable():
                self.set_focus(None)
            return False

        self._clear_label_selections()
        self._mouse_click_candidate_element = None
        self._mouse_click_candidate_moved = False
        elem_type = elem.ELEMENT_TYPE
        scroll_context = self._scrollview_focus_context_for_element(elem)
        pending_scroll_scope = None
        if scroll_context is not None:
            proxy = scroll_context["proxy"]
            scrollview = scroll_context["scrollview"]
            if (
                self._active_scrollview_scope_for(scrollview) is False
                or elem is not self._focused_element
            ):
                pending_scroll_scope = {"proxy": proxy, "scrollview": scrollview}
                scrollview_rect = scroll_context.get("scrollview_rect")
                if scrollview_rect is not None:
                    pending_scroll_scope["scrollview_rect"] = scrollview_rect
        if getattr(elem, "enabled", True) and elem is not self._focused_element and (
            elem_type != "uielement" or self._is_activatable_reusable_element(elem) or elem in self.get_focusable_elements()
        ):
            self.set_focus(elem)
        if pending_scroll_scope is not None and elem_type != "uielement":
            self._active_scrollview_scope = pending_scroll_scope
            self._edit_mode = True
            self._edit_snapshot = None
            self._set_element_focus_state(pending_scroll_scope["proxy"], True)

        if elem_type in ("button", "checkbox"):
            return self._activate_element(elem)
        if elem_type == "image":
            self._mouse_click_candidate_element = elem
            self._mouse_click_candidate_moved = False
            return True
        if elem_type in ("textinput", "textarea", "numberinput"):
            if self._active_scrollview_scope is not None:
                self._begin_scoped_child_edit(elem)
            else:
                self._enter_edit_mode()
            cursor_pos = self._text_cursor_from_mouse(elem, row, col, rect)
            if elem_type == "numberinput" and getattr(elem, "value", None) == 0.0:
                elem.select_range(0, len(elem.display_value))
            else:
                elem.cursor_pos = len(elem.display_value) if elem_type == "numberinput" else cursor_pos
                if hasattr(elem, "_selection_start"):
                    elem._selection_start = None
                if hasattr(elem, "_manual_scroll_away_from_cursor"):
                    elem._manual_scroll_away_from_cursor = False
            self._mouse_selection_element = elem
            self._mouse_selection_anchor = getattr(elem, "cursor_pos", cursor_pos)
            self._mouse_selection_changed = False
            return True
        if elem_type == "combobox":
            return self._select_combo_item_from_mouse(elem, row, rect)
        if elem_type == "listbox":
            return self._select_list_item_from_mouse(elem, row, rect, event)
        if elem_type == "uielement":
            if self._is_activatable_reusable_element(elem):
                self._mouse_click_candidate_element = elem
                self._mouse_click_candidate_moved = False
                return True
            handled = elem.handle_key(self._mouse_event_for_rect(event, rect))
            if handled:
                self._mouse_selection_element = elem
                self._mouse_selection_anchor = None
                self._mouse_selection_changed = False
            elif self._is_scrollview_proxy(elem):
                self.set_focus(elem)
                return True
            return handled

        self._enter_edit_mode()
        return True

    def _handle_mouse_wheel_event(self, event):
        """Dispatch a mouse wheel event to the element under the cursor."""
        row = event.get("row", 0)
        col = event.get("col", 0)
        elem, rect = self._element_at(row, col)
        context = self._scrollview_focus_context_for_element(elem)
        if context is not None:
            proxy_rect = context.get("scrollview_rect") or self._element_mouse_rect(context["proxy"])
            handled = context["scrollview"].handle_key(self._mouse_event_for_rect(event, proxy_rect))
            if handled:
                self._queue_scrollview_terminal_scroll(context["scrollview"], proxy_rect)
            return handled
        context = self._scrollview_context_at(row, col)
        if context is not None:
            handled = context["scrollview"].handle_key(self._mouse_event_for_rect(event, context["scrollview_rect"]))
            if handled:
                self._queue_scrollview_terminal_scroll(context["scrollview"], context["scrollview_rect"])
            return handled
        if elem is not None and elem.ELEMENT_TYPE in ("listbox", "textinput", "textarea", "uielement"):
            return elem.handle_key(self._mouse_event_for_rect(event, rect))
        return False

    def _delay_dialog_button_action(self):
        app = getattr(self, "_app", None)
        if app is None:
            return
        if getattr(app, "window_count", 0) <= 1 and getattr(self, "mode", None) != "dialog":
            return
        app.mark_dirty()
        if sys.stdout.isatty() and hasattr(app, "_render_to_terminal"):
            app._render_to_terminal()
        time.sleep(DIALOG_BUTTON_CLOSE_DELAY_SECONDS)

    def _handle_focused_element_key(self, key):
        """Dispatch an edit key and mark rendered/layout state dirty when it changes."""
        elem = self._focused_element
        if elem is None or not getattr(elem, "enabled", True):
            return False

        selection_before = None
        if elem.ELEMENT_TYPE == "listbox":
            selection_before = list(getattr(elem, "selected_items", []) or [])
        elif elem.ELEMENT_TYPE == "combobox":
            selection_before = getattr(elem, "selected_item", None)

        value_before = getattr(elem, "value", None)
        display_before = getattr(elem, "display_value", None)
        result = bool(elem.handle_key(key))
        if not result:
            return False

        value_after = getattr(elem, "value", None)
        display_after = getattr(elem, "display_value", None)
        if value_after != value_before or display_after != display_before:
            if hasattr(elem, "_mark_layout_dirty"):
                elem._mark_layout_dirty()
            self._dispatch_element_changed_for(elem, value_after if value_after is not None else display_after)
        if elem.ELEMENT_TYPE == "listbox":
            selection_after = list(getattr(elem, "selected_items", []) or [])
            if selection_after != selection_before:
                self._dispatch_selection_changed_for(elem, selection_after)
        elif elem.ELEMENT_TYPE == "combobox":
            selection_after = getattr(elem, "selected_item", None)
            if selection_after != selection_before:
                self._dispatch_selection_changed_for(elem, selection_after)
        if hasattr(elem, "_mark_dirty"):
            elem._mark_dirty()
        return True

    def handle_key(self, key):
        """Handle keyboard input."""
        if isinstance(key, dict) and key.get("type") == "mouse":
            return self._handle_mouse_event(key)
        if isinstance(key, dict) and key.get("type") == "mouse_wheel":
            return self._handle_mouse_wheel_event(key)
        if isinstance(key, dict) and key.get("type") == "paste":
            if self._focused_element:
                if not self._edit_mode and self._focused_element.ELEMENT_TYPE in ("textinput", "textarea", "numberinput"):
                    self._enter_edit_mode()
                return self._handle_focused_element_key(key)
            return False

        self._clear_label_selections()

        if self._edit_mode:
            if self._active_scrollview_scope is not None:
                return self._handle_scrollview_scope_key(key)
            if key == "Escape":
                commit = bool(self._focused_element and self._uses_leave_commit(self._focused_element))
                self._exit_edit_mode(commit=commit)
                return True
            if key == "Enter":
                if self._focused_element and self._uses_leave_commit(self._focused_element):
                    handled = self._handle_focused_element_key(key)
                    if handled and self._focused_element.ELEMENT_TYPE == "listbox":
                        self._dispatch_confirmed_for(self._focused_element, getattr(self._focused_element, "value", None))
                    return handled
                self._exit_edit_mode(commit=True)
                return True
            if self._focused_element:
                result = self._handle_focused_element_key(key)
                elem = self._focused_element
                if elem is not None:
                    etype = elem.ELEMENT_TYPE
                    if etype == "combobox" and not getattr(elem, "_menu_open", True):
                        self._exit_edit_mode()
                    elif etype == "listbox" and not getattr(elem, "multiple", False) and key == "Enter":
                        self._exit_edit_mode()
                return result
            return False

        # Navigation mode
        if key == "Tab":
            self.next_focus()
            return True
        if key == "Shift+Tab":
            self.prev_focus()
            return True
        if key in ("Up", "Down", "Left", "Right"):
            self.move_focus_direction(key)
            return True

        if key == "Enter":
            if self._focused_element:
                if not self._activate_element(self._focused_element):
                    self._enter_edit_mode()
            return True

        if key == " ":
            if self._focused_element and self._focused_element.ELEMENT_TYPE == "checkbox":
                self._focused_element.handle_key(key)
                self._dispatch_element_changed_for(self._focused_element, self._focused_element.value)
                return True

        if key in ("cmd_c", "cmd_v"):
            if key == "cmd_c" and self._copy_label_selection():
                return True
            if self._focused_element:
                return self._focused_element.handle_key(key)

        return False

    def apply_layout(self):
        """Apply COMPILED_LAYOUT to elements, setting row/col positions."""
        if not self._layout:
            return

        elem_map = {name: elem for name, elem in self._elements.items()}

        for layout_entry in self._layout:
            name = layout_entry.get("name")
            elem = elem_map.get(name) if name else None
            if not elem:
                continue

            elem._layout_entry = layout_entry
            elem._layout_applied = True
            elem._cell_name = layout_entry.get("cell_name", "")
            self._apply_styles_to_element(
                elem,
                name,
                getattr(elem, "ELEMENT_TYPE", layout_entry.get("type")),
                elem._cell_name,
            )

        # Re-apply element-level color/background from members (theme styles in
        # _apply_styles_to_element above may have overwritten them).
        for entry_name, member_def in self._members.items():
            elem = self._elements.get(entry_name)
            if elem is None:
                continue
            for k in ("color", "background"):
                if k in member_def:
                    elem.style.set(k, member_def[k])

        self._resolve_layout_geometry()

    def render(self):
        """Render the window to ANSI lines for compatibility callers."""
        return cells_to_ansi_lines(self.render_cells())

    def render_cells(self):
        """Render the window to structured terminal cells."""
        elem_grid = self._collect_element_grid(use_cell_render=True)
        return self._render_cell_frame(elem_grid)

    def _collect_element_grid(self, use_cell_render=False):
        """Collect rendered element rows by absolute window position."""
        elements = self.get_all_elements()
        elem_grid = {}

        # Render a local focused element in edit mode last so its overlay
        # (e.g. dropdown) is on top. Nested scrollview children render inside
        # their owning scrollview; drawing them here would use the wrong origin.
        focused_overlay = self._focused_element if self._edit_mode else None
        render_order = [e for e in elements if e is not focused_overlay]
        if focused_overlay in elements:
            render_order.append(focused_overlay)

        self._resolved_element_textures = []
        for elem in render_order:
            if getattr(elem, "name", "").startswith("_cell_"):
                continue
            style = elem.get_style()
            texture = getattr(style, "background_texture", "") if style else ""
            if texture:
                self._resolved_element_textures.append({
                    "row": getattr(elem, "_cell_row", 0) + elem.row,
                    "col": getattr(elem, "_cell_col", 0) + elem.col,
                    "width": max(MIN_LAYOUT_SIZE, int(getattr(elem, "width", None) or MIN_LAYOUT_SIZE)),
                    "height": max(MIN_LAYOUT_SIZE, int(getattr(elem, "height", None) or MIN_LAYOUT_SIZE)),
                    "style": style,
                })
            abs_row = getattr(elem, "_cell_row", 0) + elem.row
            abs_col = getattr(elem, "_cell_col", 0) + elem.col
            render_clip = getattr(self, "_render_cell_clip", None)
            previous_clip = getattr(elem, "_render_cell_clip", None)
            if use_cell_render and render_clip is not None and getattr(elem, "ELEMENT_TYPE", None) == "image":
                elem_top = abs_row
                elem_bottom = abs_row + max(1, int(getattr(elem, "height", 1) or 1))
                clip_top = max(elem_top, int(render_clip.get("top", elem_top) or elem_top))
                clip_bottom = min(elem_bottom, int(render_clip.get("bottom", elem_bottom) or elem_bottom))
                elem._render_cell_clip = {
                    "top": max(0, clip_top - elem_top),
                    "bottom": max(0, clip_bottom - elem_top),
                }
            try:
                rendered = (
                    elem.render_cells()
                    if use_cell_render and hasattr(elem, "render_cells")
                    else elem.render()
                )
            finally:
                if use_cell_render and render_clip is not None and getattr(elem, "ELEMENT_TYPE", None) == "image":
                    elem._render_cell_clip = previous_clip
            # ###TODO BEGIN###
            # Element absolute render position:
            #   abs_row = elem._cell_row (resolved cell row from tree) + elem.row (relative position within cell from COMPILED_LAYOUT)
            #   abs_col = elem._cell_col (resolved cell col from tree) + elem.col (relative position within cell from COMPILED_LAYOUT)
            # elem._cell_row / _cell_col come from ResolvedCell after _resolve_node()
            # elem.row / elem.col come from layout entry (relative_row, relative_col) in _resolve_layout_geometry()
            # Gap between cells: if two elements from different structural cells have abs_row/abs_col with a gap,
            # that gap is filled with spaces in _render() (line 867-874).
            # ###TODO END###
            resolved_cell = getattr(elem, "_resolved_cell", None)
            for i, line in enumerate(rendered):
                gr = abs_row + i
                line_col = abs_col
                if resolved_cell is not None:
                    pad_top, pad_right, pad_bottom, pad_left = getattr(
                        elem, "_cell_padding", (0, 0, 0, 0)
                    )
                    clip_top = resolved_cell.row + pad_top
                    clip_left = resolved_cell.col + pad_left
                    clip_bottom = resolved_cell.row + resolved_cell.height - pad_bottom
                    clip_right = resolved_cell.col + resolved_cell.width - pad_right
                    if elem.ELEMENT_TYPE == "combobox" and getattr(elem, "_menu_open", False):
                        clip_bottom = max(clip_bottom, abs_row + len(rendered))
                    if gr < clip_top or gr >= clip_bottom:
                        continue
                    clip_start = max(0, clip_left - abs_col)
                    clip_width = clip_right - max(abs_col, clip_left)
                    if clip_width <= 0:
                        continue
                    line = (
                        self._slice_rendered_cells(line, clip_start, clip_width)
                        if use_cell_render
                        else self._slice_rendered_text(line, clip_start, clip_width)
                    )
                    line_col = max(abs_col, clip_left)
                    if not line:
                        continue
                if gr not in elem_grid:
                    elem_grid[gr] = []
                elem_grid[gr].append((line_col, line))

        return elem_grid

    def _collect_layout_cells(self):
        """Build unique structural cells from layout metadata."""
        cells = []
        seen = set()

        for entry in self._layout:
            # ###TODO BEGIN###
            # Structural cell identification from COMPILED_LAYOUT entries:
            #   cell_row = entry["cell_row"] + 1  (grid row of structural cell, +1 offset)
            #   cell_col = entry["cell_col"] + 1  (grid col of structural cell, +1 offset)
            #   source_width = entry["cell_chars_width"]  (physical char width of cell)
            #   source_height = entry["cell_chars_height"]  (physical char height of cell)
            #   logical_width/height = entry["cell_width"/"cell_height"] (may be None for EXPANDED)
            # Dedup key: (cell_row, cell_col, source_width, source_height)
            # ###TODO END###
            cell_row = int(entry.get("cell_row", entry.get("row", 0)) or 0) + 1
            cell_col = int(entry.get("cell_col", entry.get("col", 0)) or 0) + 1
            source_width = int(entry.get("cell_chars_width", entry.get("chars_width", 1)) or 1)
            source_height = int(entry.get("cell_chars_height", entry.get("chars_height", 1)) or 1)
            logical_width = entry.get("cell_width")
            logical_height = entry.get("cell_height")

            if isinstance(logical_width, str):
                logical_width = None if logical_width in ("None", DIMENSION_FIT_CONTENT) else int(logical_width)
            if isinstance(logical_height, str):
                logical_height = None if logical_height in ("None", DIMENSION_FIT_CONTENT) else int(logical_height)

            width_mode = self._dimension_mode(entry, "width", logical_width, source_width)
            height_mode = self._dimension_mode(entry, "height", logical_height, source_height)

            key = (cell_row, cell_col, source_width, source_height)
            if key in seen:
                continue
            seen.add(key)
            cells.append(LayoutCell(
                source_row=cell_row, source_col=cell_col,
                source_width=source_width, source_height=source_height,
                logical_width=logical_width,
                logical_height=logical_height,
                cell_name=entry.get("cell_name", ""),
                width_mode=width_mode,
                height_mode=height_mode,
            ))

        return cells

    def _resolve_layout_geometry(self):
        """Resolve structural cells and element geometry for the current window size."""
        cells = self._collect_layout_cells()
        if not cells:
            return

        self._apply_fit_content_dimensions(cells)
        self._apply_auto_cell_dimensions(cells)
        self._apply_cell_padding_dimensions(cells)
        resolved_cells = self._resolve_cells(cells)
        if self._recompute_fit_content_heights_pass2(cells, resolved_cells):
            resolved_cells = self._resolve_cells(cells)
        cell_map = {
            (cell.source_row, cell.source_col, cell.source_width, cell.source_height): cell
            for cell in resolved_cells
        }
        self._resolved_cell_backgrounds = []
        self._resolved_cells_by_name = {}
        seen_backgrounds = set()
        window_background = self._effective_window_background()

        for layout_entry in self._layout:
            name = layout_entry.get("name")
            elem = self._elements.get(name) if name else None

            key = self._layout_entry_key(layout_entry)
            cell = cell_map.get(key)
            if not cell:
                continue

            cell_name = layout_entry.get("cell_name", "")
            if cell_name and cell_name not in self._resolved_cells_by_name:
                self._resolved_cells_by_name[cell_name] = {
                    "row": cell.row,
                    "col": cell.col,
                    "width": cell.width,
                    "height": cell.height,
                }

            if key not in seen_backgrounds:
                seen_backgrounds.add(key)
                cell_style = self._cell_style(cell_name)
                if cell_style and (
                    cell_style.background is not None
                    or cell_style.color is not None
                    or cell_style.background_texture
                ):
                    self._resolved_cell_backgrounds.append({
                        "row": cell.row,
                        "col": cell.col,
                        "width": cell.width,
                        "height": cell.height,
                        "style": cell_style,
                    })
                if cell_style and getattr(cell_style, "gap", 0):
                    self._resolved_cells_by_name[cell_name]["gap"] = cell_style.gap

            if not elem:
                continue

            # Store the effective background under the element for alpha blending /
            # inheritance. This mirrors the C++ renderer, which samples the
            # already-painted buffer cell before rendering each element.
            cell_style = self._cell_style(cell_name)
            if cell_style and cell_style.background is not None:
                elem._cell_background = self._blend_color_over(cell_style.background, window_background)
            else:
                elem._cell_background = window_background
            if cell_style and cell_style.color is not None:
                elem._cell_color = cell_style.color
            else:
                elem._cell_color = None

            pad_top, pad_right, pad_bottom, pad_left = (
                cell_style.padding if cell_style else (0, 0, 0, 0)
            )
            elem._cell_padding = (pad_top, pad_right, pad_bottom, pad_left)

            margin_top, margin_right, margin_bottom, margin_left = self._entry_style_margin(layout_entry)
            relative_row = int(layout_entry.get("row", 0) or 0) + pad_top + margin_top
            relative_col = int(layout_entry.get("col", 0) or 0) + pad_left + margin_left
            elem.row = relative_row
            elem.col = relative_col
            elem._cell_row = cell.row
            elem._cell_col = cell.col

            width_value = layout_entry.get("width")
            height_value = layout_entry.get("height")
            width_mode = layout_entry.get("width_mode", DIMENSION_AUTO)
            height_mode = layout_entry.get("height_mode", DIMENSION_AUTO)
            layout_margin_right = int(layout_entry.get("margin_right", 0) or 0)
            layout_margin_bottom = int(layout_entry.get("margin_bottom", 0) or 0)
            available_width = max(
                MIN_LAYOUT_SIZE,
                cell.width - relative_col - layout_margin_right - margin_right - pad_right,
            )
            available_height = max(
                MIN_LAYOUT_SIZE,
                cell.height - relative_row - layout_margin_bottom - margin_bottom - pad_bottom,
            )

            if width_mode == DIMENSION_FIT_CONTENT or width_value == FIT_CONTENT:
                elem.width = min(available_width, self._element_fit_width(elem, int(layout_entry.get("chars_width", 0) or 0)))
            elif width_value is None or width_value == "None":
                elem.width = available_width
            else:
                elem.width = max(MIN_LAYOUT_SIZE, int(width_value))

            source_cell_width = int(layout_entry.get("cell_chars_width", 0) or 0)
            cell_width_mode = layout_entry.get("cell_width_mode", DIMENSION_AUTO)
            touches_source_right_edge = layout_margin_right == 0
            cell_width_delta = max(0, cell.width - source_cell_width)
            follows_expanded_entry = self._has_expanded_entry_before_in_source_row(layout_entry)
            can_preserve_right_edge = (
                touches_source_right_edge
                and cell_width_mode == DIMENSION_EXPANDED
                and source_cell_width > 0
                and cell.width > source_cell_width
                and width_mode != DIMENSION_EXPANDED
            )
            can_preserve_trailing_group = (
                follows_expanded_entry
                and cell_width_mode == DIMENSION_EXPANDED
                and source_cell_width > 0
                and cell_width_delta > 0
                and width_mode != DIMENSION_EXPANDED
            )
            if can_preserve_right_edge:
                elem.col = max(
                    relative_col,
                    cell.width - int(elem.width or MIN_LAYOUT_SIZE) - margin_right - pad_right,
                )
            elif can_preserve_trailing_group:
                elem.col = max(relative_col, relative_col + cell_width_delta)

            if height_mode == DIMENSION_FIT_CONTENT or height_value == FIT_CONTENT:
                elem.height = min(available_height, self._element_fit_height(elem, int(layout_entry.get("chars_height", 0) or 0), available_width))
            elif height_value is None or height_value == "None":
                elem.height = available_height
            else:
                elem.height = max(MIN_LAYOUT_SIZE, int(height_value))

            source_cell_height = int(layout_entry.get("cell_chars_height", 0) or 0)
            cell_height_mode = layout_entry.get("cell_height_mode", DIMENSION_AUTO)
            cell_height_delta = max(0, cell.height - source_cell_height)
            follows_expanded_height_entry = self._has_expanded_entry_before_in_source_column(layout_entry)
            can_preserve_trailing_vertical_group = (
                follows_expanded_height_entry
                and cell_height_mode == DIMENSION_EXPANDED
                and source_cell_height > 0
                and cell_height_delta > 0
                and height_mode != DIMENSION_EXPANDED
            )
            if can_preserve_trailing_vertical_group:
                elem.row = max(relative_row, relative_row + cell_height_delta)

            elem._resolved_cell = cell

    def export_resolved_layout(self):
        """Return resolved cell and element geometry for backend compatibility checks."""
        if self._layout and self._elements:
            self._resolve_layout_geometry()

        layout_entries_by_name = {
            entry.get("name"): entry
            for entry in self._layout
            if entry.get("name")
        }

        cells = [
            {
                "name": name,
                "row": rect["row"],
                "col": rect["col"],
                "width": rect["width"],
                "height": rect["height"],
            }
            for name, rect in sorted(self._resolved_cells_by_name.items())
        ]

        elements = []
        for name, elem in sorted(self._elements.items()):
            entry = layout_entries_by_name.get(name, {})
            cell = getattr(elem, "_resolved_cell", None)
            elements.append({
                "name": name,
                "cell_name": entry.get("cell_name", ""),
                "row": getattr(elem, "_cell_row", 0) + getattr(elem, "row", 0),
                "col": getattr(elem, "_cell_col", 0) + getattr(elem, "col", 0),
                "width": getattr(elem, "width", 0),
                "height": getattr(elem, "height", 0),
                "cell_row": getattr(cell, "row", 0) if cell else 0,
                "cell_col": getattr(cell, "col", 0) if cell else 0,
                "relative_row": getattr(elem, "row", 0),
                "relative_col": getattr(elem, "col", 0),
            })

        return {
            "window": {
                "width": self._window_width,
                "height": self._window_height,
            },
            "cells": cells,
            "elements": elements,
        }

    def _resolve_cells(self, cells):
        """Resolve structural cell sizes from the source topology."""
        if not cells:
            return []

        root = self._build_layout_tree(cells)
        if not root:
            return []

        # ###TODO BEGIN###
        # Base dimensions from root group boundaries:
        #   base_width = root.right - root.left - 1
        #   base_height = root.bottom - root.top - 1
        # The -1 accounts for the border offset in leaf node coordinates.
        # Target dimensions (fullscreen mode):
        #   target_width = max(base_width, self._window_width - 2)
        #   target_height = max(base_height, self._window_height - 2)
        # Resolution starts at (row=0, col=0) with target dimensions.
        # ###TODO END###
        border_horizontal = self._border_width_horizontal()
        border_vertical = self._border_width_vertical()
        base_width = getattr(root, "logical_width", None)
        base_height = getattr(root, "logical_height", None)
        if base_width is None:
            base_width = max(1, root.right - root.left - 1)
        if base_height is None:
            base_height = max(1, root.bottom - root.top - 1)
        target_width = base_width
        target_height = base_height

        if self.mode in ("fullscreen", "expand_width"):
            target_width = max(base_width, self._window_width - border_vertical * 2)
        if self.mode in ("fullscreen", "expand_height"):
            target_height = max(base_height, self._window_height - border_horizontal * 2)

        resolved = []
        self._resolve_node(
            root,
            self._border_width_horizontal(),
            self._border_width_vertical(),
            target_width,
            target_height,
            resolved,
        )
        return resolved

    def _build_layout_tree(self, cells):
        leaves = [self._make_leaf_node(cell) for cell in cells]
        return self._build_group_node(leaves)

    def _make_leaf_node(self, cell):
        # ###TODO BEGIN###
        # Leaf node boundaries from structural cell metadata:
        #   left = source_col - 1
        #   top = source_row - 1
        #   right = left + source_width + 1
        #   bottom = top + source_height + 1
        # The -1/+1 offset maps grid coordinates to leaf node coordinate space.
        # source_row/source_col come from _collect_layout_cells (already +1).
        # source_width/source_height = cell_chars_width/cell_chars_height from grid parser.
        # ###TODO END###
        left = cell.source_col - 1
        top = cell.source_row - 1
        return LeafNode(
            left=left, top=top,
            right=left + cell.source_width + 1,
            bottom=top + cell.source_height + 1,
            logical_width=cell.logical_width,
            logical_height=cell.logical_height,
            source_row=cell.source_row, source_col=cell.source_col,
            source_width=cell.source_width, source_height=cell.source_height,
        )

    def _build_group_node(self, nodes):
        if not nodes:
            return None
        if len(nodes) == 1:
            return nodes[0]

        # ###TODO BEGIN###
        # Group boundaries computed from all child leaf nodes:
        #   left = min(node.left for node in nodes)
        #   top = min(node.top for node in nodes)
        #   right = max(node.right for node in nodes)
        #   bottom = max(node.bottom for node in nodes)
        # Partitioning: first try vertical split (full horizontal cuts), then horizontal split.
        # A "cut" is a row/col where elements from ALL sides touch, spanning the full group width/height.
        # ###TODO END###
        nodes = sorted(nodes, key=lambda node: (node.top, node.left, node.bottom, node.right))
        left = min(node.left for node in nodes)
        top = min(node.top for node in nodes)
        right = max(node.right for node in nodes)
        bottom = max(node.bottom for node in nodes)

        vertical_groups = self._partition_vertical(nodes, left, top, right, bottom)
        if vertical_groups:
            children = [self._build_group_node(group) for group in vertical_groups]
            return self._make_group("vertical", children, left, top, right, bottom)

        horizontal_groups = self._partition_horizontal(nodes, left, top, right, bottom)
        if horizontal_groups:
            children = [self._build_group_node(group) for group in horizontal_groups]
            return self._make_group("horizontal", children, left, top, right, bottom)

        # Fallback for layouts without a full-span separator: keep source order.
        span_top = min(node.top for node in nodes)
        span_bottom = max(node.bottom for node in nodes)
        span_left = min(node.left for node in nodes)
        span_right = max(node.right for node in nodes)
        if span_top == top and span_bottom == bottom:
            children = [self._build_group_node([node]) for node in sorted(nodes, key=lambda node: node.left)]
            return self._make_group("horizontal", children, left, top, right, bottom)
        if span_left == left and span_right == right:
            children = [self._build_group_node([node]) for node in sorted(nodes, key=lambda node: node.top)]
            return self._make_group("vertical", children, left, top, right, bottom)
        return nodes[0]

    def _partition_vertical(self, nodes, left, top, right, bottom):
        cuts = self._find_full_horizontal_cuts(nodes, left, top, right, bottom)
        if not cuts:
            return None
        boundaries = [top] + cuts + [bottom]
        return self._slice_groups_by_rows(nodes, boundaries)

    def _partition_horizontal(self, nodes, left, top, right, bottom):
        cuts = self._find_full_vertical_cuts(nodes, left, top, right, bottom)
        if not cuts:
            return None
        boundaries = [left] + cuts + [right]
        return self._slice_groups_by_cols(nodes, boundaries)

    def _find_full_horizontal_cuts(self, nodes, left, top, right, bottom):
        candidates = sorted({node.top for node in nodes} | {node.bottom for node in nodes})
        cuts = []
        for candidate in candidates:
            if candidate <= top or candidate >= bottom:
                continue
            spans = []
            for node in nodes:
                if node.top == candidate or node.bottom == candidate:
                    spans.append((node.left, node.right))
            if self._intervals_cover(spans, left, right):
                cuts.append(candidate)
        return cuts

    def _find_full_vertical_cuts(self, nodes, left, top, right, bottom):
        candidates = sorted({node.left for node in nodes} | {node.right for node in nodes})
        cuts = []
        for candidate in candidates:
            if candidate <= left or candidate >= right:
                continue
            spans = []
            for node in nodes:
                if node.left == candidate or node.right == candidate:
                    spans.append((node.top, node.bottom))
            if self._intervals_cover(spans, top, bottom):
                cuts.append(candidate)
        return cuts

    def _slice_groups_by_rows(self, nodes, boundaries):
        groups = []
        used = set()
        for band_top, band_bottom in zip(boundaries, boundaries[1:]):
            band_nodes = []
            for index, node in enumerate(nodes):
                if node.top >= band_top and node.bottom <= band_bottom:
                    band_nodes.append(node)
                    used.add(index)
            if not band_nodes:
                return None
            groups.append(sorted(band_nodes, key=lambda node: (node.top, node.left)))
        if len(used) != len(nodes):
            return None
        return groups if len(groups) > 1 else None

    def _slice_groups_by_cols(self, nodes, boundaries):
        groups = []
        used = set()
        for band_left, band_right in zip(boundaries, boundaries[1:]):
            band_nodes = []
            for index, node in enumerate(nodes):
                if node.left >= band_left and node.right <= band_right:
                    band_nodes.append(node)
                    used.add(index)
            if not band_nodes:
                return None
            groups.append(sorted(band_nodes, key=lambda node: (node.left, node.top)))
        if len(used) != len(nodes):
            return None
        return groups if len(groups) > 1 else None

    def _covers_width(self, nodes, left, right):
        cursor = left
        for node in nodes:
            if node.left != cursor:
                return False
            cursor = node.right
        return cursor == right

    def _covers_height(self, nodes, top, bottom):
        cursor = top
        for node in nodes:
            if node.top != cursor:
                return False
            cursor = node.bottom
        return cursor == bottom

    def _intervals_cover(self, spans, start, end):
        if not spans:
            return False
        cursor = start
        for span_start, span_end in sorted(spans):
            if span_start > cursor:
                return False
            cursor = max(cursor, span_end)
            if cursor >= end:
                return True
        return cursor >= end

    def _make_group(self, orientation, children, left, top, right, bottom):
        children = [child for child in children if child]
        logical_width = self._group_logical_width(orientation, children)
        logical_height = self._group_logical_height(orientation, children)
        return GroupNode(
            orientation=orientation, children=children,
            left=left, top=top, right=right, bottom=bottom,
            logical_width=logical_width, logical_height=logical_height,
        )

    def _group_logical_width(self, orientation, children):
        widths = [getattr(child, "logical_width", None) for child in children]
        if orientation == "vertical":
            fixed = [width for width in widths if width is not None]
            return None if len(fixed) != len(widths) else max(fixed)

        if any(width is None for width in widths):
            return None
        return sum(widths) + max(0, len(children) - 1) * self._border_width_vertical()

    def _group_logical_height(self, orientation, children):
        heights = [getattr(child, "logical_height", None) for child in children]
        if orientation == "horizontal":
            fixed = [height for height in heights if height is not None]
            return None if len(fixed) != len(heights) else max(fixed)

        if any(height is None for height in heights):
            return None
        return sum(heights) + max(0, len(children) - 1) * self._border_width_horizontal()

    def _resolve_node(self, node, row, col, width, height, resolved):
        width = max(1, int(width))
        height = max(1, int(height))

        if node.kind == "leaf":
            resolved.append(ResolvedCell(
                row=row, col=col, width=width, height=height,
                source_row=node.source_row, source_col=node.source_col,
                source_width=node.source_width, source_height=node.source_height,
            ))
            return

        children = getattr(node, "children", [])
        if not children:
            return

        if node.orientation == "vertical":
            self._resolve_vertical(node, row, col, width, height, resolved)
            return

        self._resolve_horizontal(node, row, col, width, height, resolved)

    def _resolve_vertical(self, node, row, col, width, height, resolved):
        # ###TODO BEGIN###
        # Vertical resolution: children stacked top-to-bottom.
        #   separators = len(children) - 1 (one gap between each pair of children)
        #   fixed_total = sum of explicit logical_height values (None = EXPANDED)
        #   extra = height - separators - fixed_total  (remaining space after fixed children + gaps)
        # Expandable children share 'extra' equally, with remainder distributed to first N.
        # Cursor advances: cursor += child_height + 1 (+1 for separator gap between children)
        # ###TODO END###
        children = node.children
        border = self._border_width_horizontal()
        separators = max(0, len(children) - 1) * border
        fixed_total = sum(child.logical_height for child in children if getattr(child, "logical_height", None) is not None)
        explicit_expandable = [child for child in children if getattr(child, "logical_height", None) is None]
        extra = max(0, height - separators - fixed_total)

        if explicit_expandable:
            expand_targets = explicit_expandable
            add_extra = False
        elif extra > 0 and self.mode == "fullscreen":
            expand_targets = children
            add_extra = True
        else:
            expand_targets = []
            add_extra = False

        share = extra // len(expand_targets) if expand_targets else 0
        remainder = extra % len(expand_targets) if expand_targets else 0
        trailing_extra = extra if extra > 0 and not explicit_expandable and self.mode != "fullscreen" and border > 0 else 0

        cursor = row
        for child in children:
            child_height = getattr(child, "logical_height", None)
            if child in expand_targets:
                if add_extra:
                    child_height = (child_height or 0) + share
                else:
                    child_height = share
                idx = expand_targets.index(child)
                if idx < remainder:
                    child_height += 1
            elif trailing_extra and child is children[-1]:
                child_height = (child_height or 0) + trailing_extra
            child_height = max(1, child_height)
            self._resolve_node(child, cursor, col, width, child_height, resolved)
            cursor += child_height + border

    def _resolve_horizontal(self, node, row, col, width, height, resolved):
        # ###TODO BEGIN###
        # Horizontal resolution: children placed left-to-right.
        #   separators = len(children) - 1 (one gap between each pair of children)
        #   fixed_total = sum of explicit logical_width values (None = EXPANDED)
        #   extra = width - separators - fixed_total  (remaining space after fixed children + gaps)
        # Expandable children share 'extra' equally, with remainder distributed to first N.
        # Cursor advances: cursor += child_width + 1 (+1 for separator gap between children)
        # ###TODO END###
        children = node.children
        border = self._border_width_vertical()
        separators = max(0, len(children) - 1) * border
        fixed_total = sum(child.logical_width for child in children if getattr(child, "logical_width", None) is not None)
        explicit_expandable = [child for child in children if getattr(child, "logical_width", None) is None]
        extra = max(0, width - separators - fixed_total)

        if explicit_expandable:
            expand_targets = explicit_expandable
            add_extra = False
        elif extra > 0 and self.mode == "fullscreen":
            expand_targets = children
            add_extra = True
        else:
            expand_targets = []
            add_extra = False

        share = extra // len(expand_targets) if expand_targets else 0
        remainder = extra % len(expand_targets) if expand_targets else 0
        trailing_extra = extra if extra > 0 and not explicit_expandable and self.mode != "fullscreen" else 0

        cursor = col
        for child in children:
            child_width = getattr(child, "logical_width", None)
            if child in expand_targets:
                if add_extra:
                    child_width = (child_width or 0) + share
                else:
                    child_width = share
                idx = expand_targets.index(child)
                if idx < remainder:
                    child_width += 1
            elif trailing_extra and child is children[-1]:
                child_width = (child_width or 0) + trailing_extra
            child_width = max(1, child_width)
            self._resolve_node(child, row, cursor, child_width, height, resolved)
            cursor += child_width + border

    @staticmethod
    def _color_key(color):
        """Normalize a color to a comparable tuple (type, value)."""
        if color is None:
            return (0, 0)
        if hasattr(color, "r"):
            return (1, color.r * 65536 + color.g * 256 + color.b)
        if isinstance(color, str):
            if len(color) == 9 and color.isdigit():
                return (1, int(color[0:3]) * 65536 + int(color[3:6]) * 256 + int(color[6:9]))
            return (2, color)
        if isinstance(color, int):
            return (3, color)
        return (4, hash(color))

    @staticmethod
    def _extract_fg_bg(text):
        """Extract (fg, bg) color from ANSI-styled text. Returns (None, None) for plain text."""
        fg = None
        bg = None
        for match in _ANSI_RE.finditer(text):
            codes_str = match.group()[2:-1]
            codes = [int(c) for c in codes_str.split(";")]

            # Identify which indices are part of truecolor/256-color sequences
            skip = set()
            for i, n in enumerate(codes):
                if n == 38 and i + 1 < len(codes) and codes[i + 1] == 2:
                    skip.update(range(i, min(i + 5, len(codes))))
                elif n == 48 and i + 1 < len(codes) and codes[i + 1] == 2:
                    skip.update(range(i, min(i + 5, len(codes))))
                elif n == 38 and i + 1 < len(codes) and codes[i + 1] == 5:
                    skip.update(range(i, min(i + 3, len(codes))))
                elif n == 48 and i + 1 < len(codes) and codes[i + 1] == 5:
                    skip.update(range(i, min(i + 3, len(codes))))

            # Process remaining codes
            for i, n in enumerate(codes):
                if i in skip:
                    continue
                # Basic foreground: 30-37
                if n >= 30 and n <= 37:
                    fg = n
                # Bright foreground: 90-97
                elif n >= 90 and n <= 97:
                    fg = n - 6
                # Basic background: 40-47
                elif n >= 40 and n <= 47:
                    bg = n
                # Bright background: 100-107
                elif n >= 100 and n <= 107:
                    bg = n - 6

            # Extract truecolor/256-color values (processed separately to avoid overlap)
            for i, n in enumerate(codes):
                if n == 38 and i + 1 < len(codes) and codes[i + 1] == 2:
                    fg = f"{codes[i+2]:03d}{codes[i+3]:03d}{codes[i+4]:03d}"
                elif n == 48 and i + 1 < len(codes) and codes[i + 1] == 2:
                    bg = f"{codes[i+2]:03d}{codes[i+3]:03d}{codes[i+4]:03d}"
                elif n == 38 and i + 1 < len(codes) and codes[i + 1] == 5:
                    fg = codes[i + 2] + 256
                elif n == 48 and i + 1 < len(codes) and codes[i + 1] == 5:
                    bg = codes[i + 2] + 256
        return fg, bg

    def _paint_cell_backgrounds(self, row, plain_chars, fg_map, bg_map):
        """Apply named-cell background styles to one row."""
        row_width = len(fg_map)
        for cell in self._resolved_cell_backgrounds:
            if row < cell["row"] or row >= cell["row"] + cell["height"]:
                continue

            start = max(0, cell["col"])
            end = min(row_width, cell["col"] + cell["width"])
            style = cell["style"]
            if start >= end:
                continue

            for col in range(start, end):
                if style.color is not None:
                    fg_map[col] = style.color
                if style.background is not None:
                    bg_map[col] = self._blend_color_over(style.background, bg_map[col])

            self._paint_texture(
                row,
                start,
                end,
                cell["row"],
                cell["col"],
                cell["width"],
                style,
                plain_chars,
                fg_map,
            )

    def _paint_element_textures(self, row, plain_chars, fg_map):
        row_width = len(fg_map)
        for element in self._resolved_element_textures:
            if row < element["row"] or row >= element["row"] + element["height"]:
                continue

            start = max(0, element["col"])
            end = min(row_width, element["col"] + element["width"])
            if start >= end:
                continue

            self._paint_texture(
                row,
                start,
                end,
                element["row"],
                element["col"],
                element["width"],
                element["style"],
                plain_chars,
                fg_map,
            )

    @staticmethod
    def _paint_texture(row, start, end, origin_row, origin_col, width, style, plain_chars, fg_map):
        texture = getattr(style, "background_texture", "") if style else ""
        if not texture:
            return

        texture_color = getattr(style, "background_texture_color", None) or getattr(style, "color", None)
        texture_width = max(MIN_LAYOUT_SIZE, int(width) or MIN_LAYOUT_SIZE)
        row_offset = max(0, row - origin_row)
        for col in range(start, end):
            if plain_chars[col] != " ":
                continue
            col_offset = max(0, col - origin_col)
            texture_index = (row_offset * texture_width + col_offset) % len(texture)
            plain_chars[col] = texture[texture_index]
            if texture_color is not None:
                fg_map[col] = texture_color

    @staticmethod
    def _parse_ansi_chars(text):
        """Parse ANSI-escaped text into per-character (char, fg, bg) tuples.

        fg/bg are either a 9-char RGB string "RRRGGGBBB", an int (basic code),
        or None meaning "inherit / no change".
        """
        result = []
        cur_fg = None
        cur_bg = None
        i = 0
        n = len(text)
        while i < n:
            ch = text[i]
            if ch == "\x1b" and i + 1 < n and text[i + 1] == "[":
                j = i + 2
                while j < n and not (0x40 <= ord(text[j]) <= 0x7E):
                    j += 1
                if j < n and text[j] == "m":
                    raw = text[i + 2:j]
                    if raw == "" or raw == "0":
                        cur_fg = None
                        cur_bg = None
                    else:
                        parts = raw.split(";")
                        nums = []
                        for p in parts:
                            try:
                                nums.append(int(p))
                            except ValueError:
                                pass
                        k = 0
                        while k < len(nums):
                            code = nums[k]
                            if code == 0:
                                cur_fg = None
                                cur_bg = None
                            elif code == 38 and k + 1 < len(nums) and nums[k + 1] == 2 and k + 4 < len(nums):
                                cur_fg = f"{nums[k+2]:03d}{nums[k+3]:03d}{nums[k+4]:03d}"
                                k += 4
                            elif code == 48 and k + 1 < len(nums) and nums[k + 1] == 2 and k + 4 < len(nums):
                                cur_bg = f"{nums[k+2]:03d}{nums[k+3]:03d}{nums[k+4]:03d}"
                                k += 4
                            k += 1
                i = j + 1
            else:
                result.append((ch, cur_fg, cur_bg))
                i += 1
        return result

    @staticmethod
    def _slice_rendered_text(text, start, width):
        start = max(0, int(start))
        width = max(0, int(width))
        if width <= 0 or not text:
            return ""
        if not UIBase._has_ansi(text):
            return text[start:start + width]

        chars = UIBase._parse_ansi_chars(text)[start:start + width]
        if not chars:
            return ""

        parts = []
        sentinel = object()
        current_fg = sentinel
        current_bg = sentinel
        for ch, fg, bg in chars:
            if fg != current_fg or bg != current_bg:
                if (
                    current_fg is not sentinel
                    and (
                        (fg is None and current_fg is not None)
                        or (bg is None and current_bg is not None)
                    )
                ):
                    parts.append("\x1b[0m")
                    current_fg = None
                    current_bg = None
                parts.append(_make_ansi(fg, bg))
                current_fg = fg
                current_bg = bg
            parts.append(ch)
        parts.append("\x1b[0m")
        return "".join(parts)

    @staticmethod
    def _row_to_cells(row):
        if not row:
            return []
        if isinstance(row, str):
            return parse_ansi_cells(row) if UIBase._has_ansi(row) else [TerminalCell(ch) for ch in row]
        return [coerce_cell(cell) for cell in row]

    @staticmethod
    def _element_style_value(color):
        if color is None:
            return None
        if hasattr(color, "r") and getattr(color, "r", -1) >= 0:
            return f"{int(color.r):03d}{int(color.g):03d}{int(color.b):03d}"
        return color

    @staticmethod
    def _blend_cell_background_over_existing(background, existing):
        # Only fully-transparent cell backgrounds keep what is underneath (this is
        # what lets a `fit: cover` tile show the panel/focus behind its padding).
        # Opaque and semi-transparent backgrounds resolve exactly as before so the
        # output stays identical to the C++ runtime (parity); blending a
        # semi-transparent cell over the focus tint would diverge from C++.
        if background is None:
            return existing
        if isinstance(background, str) and background == "transparent":
            return existing
        if getattr(background, "alpha", 1.0) <= 0.0:
            return existing
        return background

    @staticmethod
    def _slice_rendered_cells(row, start, width):
        start = max(0, int(start))
        width = max(0, int(width))
        if width <= 0 or not row:
            return []
        return UIBase._row_to_cells(row)[start:start + width]

    _PLAIN_ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")

    @staticmethod
    def _has_ansi(text):
        """Check if text contains ANSI escape codes."""
        return bool(UIBase._PLAIN_ANSI_RE.search(text)) if text else False

    def _render(self, elem_grid):
        """Render elements without grid borders."""
        return cells_to_ansi_lines(self._render_cell_frame(elem_grid))

    def _render_cell_frame(self, elem_grid):
        """Render elements without grid borders to structured terminal cells."""
        win_fg = self.style.color if self.style else None
        win_bg = self._effective_window_background()
        rows = []
        self.styles = []
        max_background_row = max(
            (cell["row"] + cell["height"] - 1 for cell in self._resolved_cell_backgrounds),
            default=-1,
        )
        max_element_row = max(elem_grid.keys(), default=-1)
        total_rows = max(self._window_height, max_element_row + 1, max_background_row + 1)

        for row in range(total_rows):
            plain_chars = [" "] * self._window_width
            fg_map = [win_fg] * self._window_width
            bg_map = [win_bg] * self._window_width
            raw_map = [""] * self._window_width
            raw_width_map = [1] * self._window_width
            raw_height_map = [1] * self._window_width
            raw_skip_map = [False] * self._window_width

            self._paint_cell_backgrounds(row, plain_chars, fg_map, bg_map)
            focus_background = self._active_scrollview_scope_focus_background()
            focus_rect = self._active_scrollview_scope_rect() if focus_background is not None else None
            if focus_rect is None:
                focus_background = self._ancestor_active_scrollview_focus_background()
                if focus_background is not None:
                    focus_rect = {
                        "top": 0,
                        "left": 0,
                        "bottom": self._window_height,
                        "right": self._window_width,
                    }
            if focus_rect is not None:
                inside_rows = row >= focus_rect["top"] and row < focus_rect["bottom"]
                if inside_rows:
                    start = max(0, focus_rect["left"])
                    end = min(self._window_width, focus_rect["right"])
                    for col in range(start, end):
                        bg_map[col] = self._blend_color_over(focus_background, bg_map[col])

            row_entries = elem_grid.get(row, [])
            if isinstance(row_entries, dict):
                row_entries = sorted(row_entries.items())
            for col, text in row_entries:
                for offset, cell in enumerate(self._row_to_cells(text)):
                    target_col = col + offset
                    if target_col < 0 or target_col >= self._window_width:
                        continue
                    plain_chars[target_col] = cell.normalized_text()
                    if cell.foreground is not None:
                        fg_map[target_col] = self._element_style_value(cell.foreground)
                    if cell.background is not None:
                        bg_map[target_col] = self._element_style_value(
                            self._blend_cell_background_over_existing(cell.background, bg_map[target_col])
                        )
                    raw_map[target_col] = getattr(cell, "raw", "")
                    raw_width_map[target_col] = int(getattr(cell, "raw_width", 1) or 1)
                    raw_height_map[target_col] = int(getattr(cell, "raw_height", 1) or 1)
                    raw_skip_map[target_col] = bool(getattr(cell, "raw_skip", False))

            self._paint_element_textures(row, plain_chars, fg_map)
            dim_background = self._active_scrollview_scope_dim_background()
            dim_rect = self._active_scrollview_scope_rect() if dim_background is not None else None
            if dim_rect is not None:
                inside_rows = row >= dim_rect["top"] and row < dim_rect["bottom"]
                for col in range(self._window_width):
                    inside = inside_rows and col >= dim_rect["left"] and col < dim_rect["right"]
                    if not inside:
                        bg_map[col] = self._blend_color_over(dim_background, bg_map[col])

            row_styles = []
            current_fg = None
            current_bg = None
            for col in range(self._window_width):
                fg = fg_map[col]
                bg = bg_map[col]
                if (
                    self._color_key(fg) != self._color_key(current_fg)
                    or self._color_key(bg) != self._color_key(current_bg)
                ):
                    row_styles.append({"char_pos": col, "color": fg, "background": bg})
                    current_fg = fg
                    current_bg = bg

            if not row_styles:
                row_styles = [{"char_pos": 0, "color": win_fg, "background": win_bg}]

            rows.append([
                TerminalCell(
                    plain_chars[col],
                    fg_map[col],
                    bg_map[col],
                    raw_map[col],
                    raw_width_map[col],
                    raw_height_map[col],
                    raw_skip_map[col],
                )
                for col in range(self._window_width)
            ])
            self.styles.append(row_styles)

        return rows[:self._window_height]

    def _parent_background(self):
        parent = getattr(self, "parent", None)
        if parent is None:
            return None

        parent_bg = self._style_background(parent)
        if parent_bg is not None:
            base_bg = None
            if hasattr(parent, "_parent_background"):
                base_bg = parent._parent_background()
            if hasattr(parent, "_get_cell_background"):
                base_bg = parent._get_cell_background() or base_bg
            if base_bg is None:
                container = getattr(parent, "parent", None)
                base_bg = self._style_background(container)
            return self._blend_color_over(parent_bg, base_bg)

        if hasattr(parent, "_get_cell_background"):
            cell_bg = parent._get_cell_background()
            if cell_bg is not None:
                return cell_bg

        container = getattr(parent, "parent", None)
        container_bg = self._style_background(container)
        if container_bg is not None:
            return container_bg
        return None

    def _effective_window_background(self):
        return self._blend_color_over(
            self.style.background if self.style else None,
            self._parent_background(),
        )

    @staticmethod
    def _style_background(source):
        if source is None:
            return None
        if hasattr(source, "get_style"):
            style = source.get_style()
        else:
            style = getattr(source, "style", None)
        return getattr(style, "background", None) if style else None

    @staticmethod
    def _blend_color_over(color, background):
        if color is None:
            return background

        alpha = getattr(color, "alpha", 1.0)
        if alpha is None or alpha >= 1.0:
            return color
        if alpha <= 0.0:
            return background

        if isinstance(background, str) and len(background) == 9 and background.isdigit():
            background = Color(
                f"#{int(background[0:3]):02x}{int(background[3:6]):02x}{int(background[6:9]):02x}"
            )
        if background is None or getattr(background, "r", -1) < 0:
            background = Color("#000000")

        r = round(alpha * color.r + (1 - alpha) * background.r)
        g = round(alpha * color.g + (1 - alpha) * background.g)
        b = round(alpha * color.b + (1 - alpha) * background.b)
        return Color(f"#{r:02x}{g:02x}{b:02x}")

    def invalidate(self):
        """Mark window for re-render."""
        if self._app is not None:
            self._app.mark_dirty()

    def invalidate_layout(self):
        """Force layout geometry to be recomputed on the next resize sync."""
        self._last_window_size = None
        self.invalidate()
