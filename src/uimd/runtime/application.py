import fcntl
import os
import re
import select
import shutil
import signal
import sys
import termios
import threading
import time
import tty
import struct
from uimd.runtime.mcp import MCPServer, parse_mcp_runtime_args
from uimd.runtime.rendering import (
    ANSI_CLEAR_SCREEN,
    TerminalBuffer,
    TerminalCell,
    cells_to_ansi_lines,
    coerce_cell,
    parse_ansi_cells,
)

FIT_CONTENT = 0x1FFFFFFE
DEFAULT_TERMINAL_WIDTH = 80
DEFAULT_TERMINAL_HEIGHT = 24
ESCAPE_SEQUENCE_TIMEOUT = 0.01
INPUT_POLL_TIMEOUT = 0.1
INPUT_DRAIN_TIMEOUT = 0.0
TERMINAL_EXIT_DRAIN_TIMEOUT = 0.02
TERMINAL_EXIT_DRAIN_BYTES = 1024
TERMINAL_EXIT_DRAIN_MAX_READS = 64
RENDER_FRAME_INTERVAL_SECONDS = 1 / 30
MOUSE_WHEEL_COALESCE_READY_TIMEOUT = 0.0
MOUSE_WHEEL_COALESCE_MAX_EVENTS = 512
MOUSE_WHEEL_COALESCED_MAX_DELTA = 12
MOUSE_WHEEL_SEQUENCE_MAX_BYTES = 64
MOUSE_WHEEL_DELTA_UP = 1
MOUSE_WHEEL_DELTA_DOWN = -1
MOUSE_COORD_OFFSET = 1
MOUSE_DRAG_FLAG = 32
MOUSE_WHEEL_FLAG = 64
MOUSE_BUTTON_MASK = 3
MOUSE_LEFT_BUTTON = 0
ANSI_RE = re.compile(r"\x1b\[[0-9;?]*[A-Za-z]")
_RGB_COLOR_RE = re.compile(r"\x1b\[([34]8;2;)(\d+);(\d+);(\d+)m")
TERMINAL_CELL_PIXEL_RESPONSE_RE = re.compile(rb"\x1b\[6;([0-9]+);([0-9]+)t")
TERMINAL_TEXT_AREA_PIXEL_RESPONSE_RE = re.compile(rb"\x1b\[4;([0-9]+);([0-9]+)t")
TERMINAL_PIXEL_RESPONSE_BODY_RE = re.compile(rb"\[(?:4|6);([0-9]+);([0-9]+)t")
BACKGROUND_DIM_FACTOR = 0.5
DEFAULT_NOTIFICATION_DURATION = 4.0
NOTIFICATION_PADDING_X = 2
NOTIFICATION_MARGIN_TOP = 1
NOTIFICATION_MARGIN_RIGHT = 3
NOTIFICATION_MIN_WIDTH = 1
DIMENSION_EXPANDED = "expanded"
TERMINAL_CELL_PIXEL_QUERY = "\x1b[16t"
TERMINAL_TEXT_AREA_PIXEL_QUERY = "\x1b[14t"
TERMINAL_CELL_PIXEL_QUERY_TIMEOUT = 0.05
TERMINAL_CELL_PIXEL_QUERY_MAX_BYTES = 64
TERMINAL_WINSIZE_STRUCT = "HHHH"
TERMINAL_WINSIZE_STRUCT_BYTES = 8


class _TerminalFrameDiff:
    """Compatibility wrapper around the structured TerminalBuffer backend."""

    def __init__(self, size_provider):
        self._size_provider = size_provider
        self._buffer = None
        self._force_clear = True

    def reset(self):
        self._buffer = None
        self._force_clear = True

    def ready(self):
        width, height = self._size_provider()
        return (
            self._buffer is not None
            and (self._buffer.width, self._buffer.height) == (width, height)
            and not self._buffer.force_full_redraw
            and not self._force_clear
        )

    def render(self, layers, protected_rects=None, allow_clear=True, row_offset=0, col_offset=0):
        buffer, size_changed = self._prepare_buffer(layers)
        self._apply_protected_rects(buffer, protected_rects or [])
        parts = [ANSI_CLEAR_SCREEN] if (self._force_clear or size_changed) and allow_clear else []
        if self._force_clear or size_changed:
            buffer.request_full_redraw()
        buffer.begin_render_stats()
        parts.append(buffer.render_diff(row_offset=row_offset, col_offset=col_offset))
        self._force_clear = False
        return "".join(parts)

    def sync(self, layers, protected_rects=None):
        buffer, _size_changed = self._prepare_buffer(layers)
        self._apply_protected_rects(buffer, protected_rects or [])
        buffer.previous = [list(row) for row in buffer.cells]
        buffer.force_full_redraw = False
        self._force_clear = False

    def render_scroll_region(self, row_offset, start_row, height, delta):
        if self._buffer is None:
            return ""
        return self._buffer.render_scroll_region(row_offset, start_row, height, delta)

    def request_full_redraw(self):
        if self._buffer is not None:
            self._buffer.request_full_redraw()

    def _prepare_buffer(self, layers):
        width, height = self._size_provider()
        if self._buffer is None:
            self._buffer = TerminalBuffer(width, height)
            size_changed = True
        else:
            size_changed = self._buffer.resize(width, height)
        self._buffer.clear()
        self._blit_layers(self._buffer, layers)
        return self._buffer, size_changed

    def _blit_layers(self, buffer, layers):
        for rows, col_offset, row_offset in layers:
            for row_index, row in enumerate(rows):
                target_row = int(row_offset) + row_index
                if target_row < 0 or target_row >= buffer.height:
                    continue
                target_col = int(col_offset)
                for cell in self._row_to_cells(row):
                    if 0 <= target_col < buffer.width:
                        buffer.set_cell(target_row, target_col, cell)
                    target_col += 1
                    if target_col >= buffer.width:
                        break

    @staticmethod
    def _row_to_cells(row):
        if isinstance(row, str):
            return parse_ansi_cells(row)
        return [coerce_cell(cell) for cell in (row or [])]

    @staticmethod
    def _apply_protected_rects(buffer, protected_rects):
        if not protected_rects:
            return
        for rect in protected_rects:
            top = max(0, int(rect.get("row", 0)))
            left = max(0, int(rect.get("col", 0)))
            bottom = min(buffer.height, top + max(0, int(rect.get("height", 0))))
            right = min(buffer.width, left + max(0, int(rect.get("width", 0))))
            for row in range(top, bottom):
                for col in range(left, right):
                    buffer.copy_previous_to_current(row, col)


class UIApplication:
    """Main application class that manages the window stack and rendering."""

    def __init__(self, width=None, height=None):
        self._width = width if width is not None else FIT_CONTENT
        self._height = height if height is not None else FIT_CONTENT
        self._window_stack = []
        self._running = False
        self._needs_resize = False
        self._dirty = True
        self._last_render_at = 0.0
        self._notifications = []
        self._active_popup = None
        self._viewport = None
        self._viewport_frame_rect = None
        self._viewport_frame_rows = []
        self._protected_rects = []
        self._pending_terminal_scrolls = []
        self._terminal_frame_diff = _TerminalFrameDiff(lambda: self._get_terminal_size())
        self._render_lock = threading.RLock()
        self._input_buffer = bytearray()

    @property
    def active_window(self):
        return self._window_stack[-1] if self._window_stack else None

    @property
    def window_count(self):
        return len(self._window_stack)

    def open(self, window):
        """Open a new window. Previous windows lose full focus."""
        if self._window_stack:
            self._terminal_frame_diff.reset()
            for previous in self._window_stack:
                previous.mode = "dialog"
        window.parent = None
        window._app = self
        window.mode = self._resolved_window_mode(window)

        width, height = self._window_dimensions(window)
        window.resize(width, height)
        window.open()
        self._window_stack.append(window)
        self._dirty = True

    def mark_dirty(self):
        """Mark the application as needing a re-render."""
        self._dirty = True

    def request_full_redraw(self):
        """Force the next frame to repaint every cell.

        Needed when a change repaints the background around a Sixel image: the
        incremental diff leaves the image's raw cell untouched (its own
        background did not change) while clobbering the pixels around it, so the
        image must be re-emitted without clearing the terminal screen."""
        self._terminal_frame_diff.request_full_redraw()
        self._dirty = True

    def queue_terminal_scroll(self, operation):
        """Queue a terminal scroll-region hint for a full-width ScrollView."""
        if not operation:
            return
        normalized = {
            "row": max(0, int(operation.get("row", 0))),
            "col": max(0, int(operation.get("col", 0))),
            "height": max(0, int(operation.get("height", 0))),
            "width": max(0, int(operation.get("width", 0))),
            "delta": int(operation.get("delta", 0) or 0),
        }
        if normalized["height"] <= 0 or normalized["width"] <= 0 or normalized["delta"] == 0:
            return
        if self._pending_terminal_scrolls:
            previous = self._pending_terminal_scrolls[-1]
            same_region = all(previous[key] == normalized[key] for key in ("row", "col", "height", "width"))
            if same_region:
                previous["delta"] += normalized["delta"]
                if previous["delta"] == 0:
                    self._pending_terminal_scrolls.pop()
                self.mark_dirty()
                return
        self._pending_terminal_scrolls.append(normalized)
        self.mark_dirty()

    def _sync_active_window_size(self):
        """Keep the active window aligned with the current app or terminal size."""
        if not self.active_window:
            return
        self._resize_window(self.active_window)

    def notify(self, message, duration=DEFAULT_NOTIFICATION_DURATION, level="info"):
        """Show a transient notification over the active UI."""
        if not message:
            return
        self._notifications.append({
            "message": str(message),
            "expires_at": time.monotonic() + float(duration),
            "level": level,
        })
        self.mark_dirty()

    def _active_notifications(self):
        now = time.monotonic()
        self._notifications = [
            notification for notification in self._notifications
            if notification["expires_at"] > now
        ]
        return list(self._notifications)

    def _resolved_window_mode(self, window):
        """Resolve sizing mode from layout: expanded cells determine full-width/height."""
        explicit_mode = str(getattr(window, "mode", "") or "").strip().lower()
        if explicit_mode in ("fullscreen", "expand_width", "expand_height"):
            return explicit_mode
        layout = getattr(window, "_layout", None) or []
        expand_w = any(self._entry_dimension_expands(entry, "width") for entry in layout)
        expand_h = any(self._entry_dimension_expands(entry, "height") for entry in layout)
        if expand_w and expand_h:
            return "fullscreen"
        if expand_w:
            return "expand_width"
        if expand_h:
            return "expand_height"
        if explicit_mode == "dialog" or self._is_dialog_window(window):
            return "dialog"
        return "normal"

    @staticmethod
    def _is_dialog_window(window):
        if window is None:
            return False
        explicit_mode = str(getattr(window, "mode", "") or "").strip().lower()
        metadata_kind = str(getattr(window, "_metadata_kind", "") or "").strip().lower()
        return explicit_mode == "dialog" or metadata_kind == "dialog"

    @staticmethod
    def _entry_dimension_expands(entry, axis):
        mode = entry.get(f"cell_{axis}_mode")
        if mode:
            return mode == DIMENSION_EXPANDED
        return entry.get(f"cell_{axis}") is None

    def _get_terminal_size(self):
        """Return the current terminal size."""
        if self._viewport is not None:
            return max(1, int(self._viewport["width"])), max(1, int(self._viewport["height"]))
        size = shutil.get_terminal_size((DEFAULT_TERMINAL_WIDTH, DEFAULT_TERMINAL_HEIGHT))
        return max(1, int(size.columns)), max(1, int(size.lines))

    def _sync_terminal_cell_px(self, fd):
        """Update image renderer cell pixel metrics from the active terminal."""
        size = self._terminal_cell_px_from_ioctl()
        if size is None:
            size = self._query_terminal_cell_px(fd)
        if size is None:
            return
        from uimd.runtime.image import set_terminal_cell_px
        set_terminal_cell_px(size[0], size[1])

    def _terminal_cell_px_from_ioctl(self):
        try:
            data = fcntl.ioctl(
                sys.stdout.fileno(),
                termios.TIOCGWINSZ,
                b"\x00" * TERMINAL_WINSIZE_STRUCT_BYTES,
            )
            rows, cols, xpx, ypx = struct.unpack(TERMINAL_WINSIZE_STRUCT, data)
        except Exception:
            return None
        if rows <= 0 or cols <= 0 or xpx <= 0 or ypx <= 0:
            return None
        return max(1, round(xpx / cols)), max(1, round(ypx / rows))

    def _query_terminal_cell_px(self, fd):
        """Ask the terminal for cell pixel metrics using xterm window reports."""
        direct = self._query_terminal_pixel_report(
            fd,
            TERMINAL_CELL_PIXEL_QUERY,
            TERMINAL_CELL_PIXEL_RESPONSE_RE,
        )
        if direct is not None:
            return direct

        text_area = self._query_terminal_pixel_report(
            fd,
            TERMINAL_TEXT_AREA_PIXEL_QUERY,
            TERMINAL_TEXT_AREA_PIXEL_RESPONSE_RE,
        )
        if text_area is None:
            return None
        columns, rows = self._get_terminal_size()
        if columns <= 0 or rows <= 0:
            return None
        return max(1, round(text_area[0] / columns)), max(1, round(text_area[1] / rows))

    def _query_terminal_pixel_report(self, fd, query, response_re):
        """Send a terminal query and parse a height/width pixel response."""
        try:
            self._write_terminal(query)
        except Exception:
            return None
        deadline = time.monotonic() + TERMINAL_CELL_PIXEL_QUERY_TIMEOUT
        response = bytearray()
        while len(response) < TERMINAL_CELL_PIXEL_QUERY_MAX_BYTES:
            timeout = max(0.0, deadline - time.monotonic())
            if timeout <= 0.0 or not self._read_ready(fd, timeout):
                break
            chunk = os.read(fd, TERMINAL_CELL_PIXEL_QUERY_MAX_BYTES - len(response))
            if not chunk:
                break
            response.extend(chunk)
            match = response_re.search(bytes(response))
            if match is not None:
                height = int(match.group(1))
                width = int(match.group(2))
                if width > 0 and height > 0:
                    return width, height
        return None

    def set_viewport(self, row, col, width, height):
        """Constrain rendering to a terminal rectangle."""
        self._viewport = {
            "row": max(0, int(row)),
            "col": max(0, int(col)),
            "width": max(1, int(width)),
            "height": max(1, int(height)),
        }
        self._viewport_frame_rect = None
        self._viewport_frame_rows = []
        self._terminal_frame_diff.reset()
        self._sync_active_window_size()
        self.mark_dirty()

    def get_viewport(self):
        """Return current viewport or the full terminal rectangle."""
        if self._viewport is not None:
            return dict(self._viewport)
        size = shutil.get_terminal_size((DEFAULT_TERMINAL_WIDTH, DEFAULT_TERMINAL_HEIGHT))
        return {
            "row": 0,
            "col": 0,
            "width": max(1, int(size.columns)),
            "height": max(1, int(size.lines)),
        }

    def set_protected_rects(self, rects):
        """Prevent terminal frame rendering from overwriting child-process rectangles."""
        self._protected_rects = [
            {
                "row": max(0, int(rect.get("row", 0))),
                "col": max(0, int(rect.get("col", 0))),
                "width": max(1, int(rect.get("width", 1))),
                "height": max(1, int(rect.get("height", 1))),
            }
            for rect in (rects or [])
        ]
        self.mark_dirty()

    def get_render_rect(self):
        """Return the terminal rectangle covered by the currently rendered window stack."""
        if not self._window_stack:
            return {"row": 0, "col": 0, "width": 1, "height": 1}

        rects = []
        for window in self._window_stack:
            self._resize_window(window)
            col_offset, row_offset = self._window_offsets(window)
            rects.append({
                "row": int(row_offset),
                "col": int(col_offset),
                "right": int(col_offset) + max(1, int(getattr(window, "_window_width", 1) or 1)),
                "bottom": int(row_offset) + max(1, int(getattr(window, "_window_height", 1) or 1)),
            })

        top = min(rect["row"] for rect in rects)
        left = min(rect["col"] for rect in rects)
        right = max(rect["right"] for rect in rects)
        bottom = max(rect["bottom"] for rect in rects)
        return {
            "row": top,
            "col": left,
            "width": max(1, right - left),
            "height": max(1, bottom - top),
        }

    def _window_dimensions(self, window):
        """Resolve the target size for one window based on its sizing mode."""
        mode = self._resolved_window_mode(window)
        term_w, term_h = self._get_terminal_size()
        margin_top, margin_right, margin_bottom, margin_left = self._window_margins(window)
        available_w = max(1, term_w - margin_left - margin_right)
        available_h = max(1, term_h - margin_top - margin_bottom)

        if self._width != FIT_CONTENT:
            width = self._width
        elif mode in ("fullscreen", "expand_width"):
            width = available_w
        else:
            width = self._compute_natural_width(window)

        if self._height != FIT_CONTENT:
            height = self._height
        elif mode in ("fullscreen", "expand_height"):
            height = available_h
        else:
            height = self._compute_natural_height(window)

        if self._width != FIT_CONTENT:
            width = max(1, int(width))
        else:
            width = max(1, min(int(width), available_w)) if width else min(DEFAULT_TERMINAL_WIDTH, available_w)
        if self._height != FIT_CONTENT:
            height = max(1, int(height))
        else:
            height = max(1, min(int(height), available_h)) if height else min(DEFAULT_TERMINAL_HEIGHT, available_h)
        return width, height

    @staticmethod
    def _window_margins(window):
        style = getattr(window, "style", None)
        margin = getattr(style, "margin", (0, 0, 0, 0)) if style is not None else (0, 0, 0, 0)
        if not isinstance(margin, (tuple, list)) or len(margin) != 4:
            return 0, 0, 0, 0
        return tuple(max(0, int(value or 0)) for value in margin)

    def _resize_window(self, window):
        """Resize one window to its current target dimensions."""
        width, height = self._window_dimensions(window)
        window.resize(width, height)
        self._dirty = True
        return width, height

    def _sync_active_window_size(self):
        """Keep the active window aligned with the current app or terminal size."""
        if not self.active_window:
            return
        self._resize_window(self.active_window)

    def _compute_natural_width(self, window):
        """Compute natural width from structural cells at the top edge."""
        if hasattr(window, "get_natural_size"):
            return window.get_natural_size()[0]
        if not window._layout:
            return 0
        cells = {}
        for entry in window._layout:
            cell_row = int(entry.get("cell_row", 0) or 0)
            cell_col = int(entry.get("cell_col", 0) or 0)
            cw = int(entry.get("cell_chars_width", entry.get("chars_width", 1)) or 1)
            key = (cell_row, cell_col)
            if key not in cells or cw > cells[key]:
                cells[key] = cw
        if not cells:
            return 0
        min_row = min(r for r, c in cells)
        row_cells = [(c, w) for (r, c), w in cells.items() if r == min_row]
        max_right = 0
        for col, width in row_cells:
            right = col + width
            if right > max_right:
                max_right = right
        return max_right

    def _compute_natural_height(self, window):
        """Compute natural height from structural cells at the left edge."""
        if hasattr(window, "get_natural_size"):
            return window.get_natural_size()[1]
        if not window._layout:
            return 0
        cells = {}
        for entry in window._layout:
            cell_row = int(entry.get("cell_row", 0) or 0)
            cell_col = int(entry.get("cell_col", 0) or 0)
            ch = int(entry.get("cell_chars_height", entry.get("chars_height", 1)) or 1)
            key = (cell_row, cell_col)
            if key not in cells or ch > cells[key]:
                cells[key] = ch
        if not cells:
            return 0
        min_col = min(c for r, c in cells)
        col_cells = [(r, height) for (r, c), height in cells.items() if c == min_col]
        max_bottom = 0
        for row, height in col_cells:
            bottom = row + height
            if bottom > max_bottom:
                max_bottom = bottom
        return max_bottom

    def close_current(self, exit_previous_edit=True):
        """Close the topmost window, returning focus to previous."""
        if not self._window_stack:
            return

        window = self._window_stack.pop()
        window.close()
        self._terminal_frame_diff.reset()

        if self._window_stack:
            prev = self._window_stack[-1]
            prev.mode = self._resolved_window_mode(prev)
            self._resize_window(prev)
            if (
                exit_previous_edit
                and
                hasattr(prev, "_exit_edit_mode")
                and getattr(prev, "_active_scrollview_scope", None) is None
                and not self._window_has_active_scrollview_scope(prev)
            ):
                prev._exit_edit_mode(commit=True)

        self._dirty = True

    @classmethod
    def _window_has_active_scrollview_scope(cls, view):
        if getattr(view, "_active_scrollview_scope", None) is not None:
            return True
        for element in getattr(view, "_elements", {}).values():
            current_view = element.current_view() if hasattr(element, "current_view") else None
            if current_view is not None and cls._window_has_active_scrollview_scope(current_view):
                return True
            child = getattr(element, "_child_instance", None)
            if child is not None and cls._window_has_active_scrollview_scope(child):
                return True
        return False

    def handle_key(self, key):
        """Handle a keyboard event."""
        if not self._window_stack:
            return

        if isinstance(key, dict) and key.get("type") in ("mouse", "mouse_wheel"):
            key = self._translate_mouse_event(key)

        if self._active_popup is not None:
            if key == "Enter":
                self._active_popup.dismiss()
                # fall through — let Enter also reach the active window
            elif isinstance(key, dict) and key.get("type") == "mouse" and key.get("event") == "press":
                self._active_popup.dismiss()
                return  # swallow click so background isn't accidentally triggered

        if key == "Escape":
            if self._is_dialog_window(self.active_window):
                if self.active_window.handle_key(key):
                    self._dirty = True
                    return
                if getattr(self.active_window, "_handles_escape", False):
                    self._dirty = True
                    return
                if not getattr(self.active_window, '_edit_mode', False):
                    self.close_current()
                    return

        if key == "Alt+Enter":
            if self.active_window:
                if getattr(self.active_window, '_edit_mode', False):
                    self.active_window.handle_key(key)
                else:
                    self.active_window.save()
            return

        if key == "cmd_q":
            self._running = False
            return

        if self.active_window.handle_key(key):
            self._dirty = True

    def _translate_mouse_event(self, event):
        """Translate terminal coordinates into active-window coordinates."""
        col_offset, row_offset = self._active_window_offsets()
        translated = dict(event)
        translated["col"] = event.get("col", 0) - col_offset
        translated["row"] = event.get("row", 0) - row_offset
        return translated

    def _active_window_offsets(self):
        """Return active window terminal offsets matching render placement."""
        return self._window_offsets(self.active_window)

    def _window_offsets(self, window, mode=None):
        """Return one window's terminal offsets matching render placement."""
        if not window:
            return 0, 0
        mode = mode or self._resolved_window_mode(window)
        viewport_col = int(self._viewport["col"]) if self._viewport is not None else 0
        viewport_row = int(self._viewport["row"]) if self._viewport is not None else 0
        margin_top, margin_right, margin_bottom, margin_left = self._window_margins(window)
        if mode == "fullscreen":
            return margin_left + viewport_col, margin_top + viewport_row

        term_w, term_h = self._get_terminal_size()
        win_w = getattr(window, "_window_width", 0)
        win_h = getattr(window, "_window_height", 0)
        available_w = max(1, term_w - margin_left - margin_right)
        available_h = max(1, term_h - margin_top - margin_bottom)

        col_offset = margin_left if mode in ("fullscreen", "expand_width") else margin_left + max(0, (available_w - win_w) // 2)
        row_offset = margin_top if mode in ("fullscreen", "expand_height") else margin_top + max(0, (available_h - win_h) // 2)
        return col_offset + viewport_col, row_offset + viewport_row

    def render(self):
        """Render all windows in the stack as ANSI lines."""
        return cells_to_ansi_lines(self.render_cells())

    def render_cells(self):
        """Render the active window stack top as structured terminal cells."""
        if not self._window_stack:
            return []

        win = self.active_window
        self._sync_active_window_size()
        result = self._render_window_cells(win)
        return self._render_notifications_cells(result)

    @staticmethod
    def _render_window_cells(window):
        if hasattr(window, "render_cells"):
            return window.render_cells()
        return [parse_ansi_cells(line) for line in window.render()]

    @staticmethod
    def _row_to_cells(row):
        if isinstance(row, str):
            return parse_ansi_cells(row)
        return [coerce_cell(cell) for cell in (row or [])]

    @classmethod
    def _row_to_ansi(cls, row):
        if isinstance(row, str):
            return row
        cells = cls._row_to_cells(row)
        if not cells:
            return ""
        if all(cell.foreground is None and cell.background is None for cell in cells):
            return "".join(cell.normalized_text() for cell in cells)
        return cells_to_ansi_lines([cells])[0]

    def _render_notifications(self, lines):
        """Overlay active notifications in the top-right of rendered lines."""
        notifications = self._active_notifications()
        if not notifications or not lines:
            return lines

        rendered = list(lines)
        width = max((len(self._strip_ansi(line)) for line in rendered), default=0)
        if width <= 0:
            return rendered

        row = min(max(0, NOTIFICATION_MARGIN_TOP), len(rendered) - 1)
        for notification in notifications[-1:]:
            message = notification["message"]
            box_width = max(NOTIFICATION_MIN_WIDTH, len(message) + NOTIFICATION_PADDING_X * 2)
            left = max(0, width - box_width - NOTIFICATION_MARGIN_RIGHT)
            text = " " * NOTIFICATION_PADDING_X + message + " " * NOTIFICATION_PADDING_X
            text = text[:box_width].ljust(box_width)
            rendered[row] = self._overlay_plain_text(rendered[row], left, text)
        return rendered

    def _render_notifications_cells(self, rows):
        """Overlay active notifications in the top-right of structured rows."""
        notifications = self._active_notifications()
        if not notifications or not rows:
            return rows
        rendered = [list(row) for row in rows]
        width = max((len(row) for row in rendered), default=0)
        if width <= 0:
            return rendered
        row = min(max(0, NOTIFICATION_MARGIN_TOP), len(rendered) - 1)
        for notification in notifications[-1:]:
            message = notification["message"]
            box_width = max(NOTIFICATION_MIN_WIDTH, len(message) + NOTIFICATION_PADDING_X * 2)
            left = max(0, width - box_width - NOTIFICATION_MARGIN_RIGHT)
            text = (" " * NOTIFICATION_PADDING_X + message + " " * NOTIFICATION_PADDING_X)
            text = text[:box_width].ljust(box_width)
            for offset, char in enumerate(text):
                col = left + offset
                if col >= width:
                    break
                previous = rendered[row][col]
                rendered[row][col] = TerminalCell(char, previous.foreground, previous.background)
        return rendered

    @staticmethod
    def _strip_ansi(text):
        return ANSI_RE.sub("", text)

    @classmethod
    def _overlay_plain_text(cls, line, col, text):
        plain = cls._strip_ansi(line)
        if col >= len(plain):
            return line
        prefix = plain[:col]
        suffix_start = min(len(plain), col + len(text))
        return prefix + text + plain[suffix_start:]

    def _read_ready(self, fd, timeout):
        """Wait for terminal input readiness."""
        if self._input_buffer:
            return True
        readable, _, _ = select.select([fd], [], [], timeout)
        return bool(readable)

    def _read_input_bytes(self, fd, count):
        """Read bytes from the local input buffer first, then from the terminal."""
        count = max(0, int(count or 0))
        if count == 0:
            return b""
        if self._input_buffer:
            chunk = bytes(self._input_buffer[:count])
            del self._input_buffer[:count]
            if len(chunk) >= count:
                return chunk
            return chunk + os.read(fd, count - len(chunk))
        return os.read(fd, count)

    def _unread_input_bytes(self, data):
        """Push bytes back into the logical input stream."""
        if not data:
            return
        self._input_buffer[:0] = bytes(data)

    def _read_escape_sequence(self, fd):
        """Decode an ANSI escape sequence into one logical key."""
        if not self._read_ready(fd, ESCAPE_SEQUENCE_TIMEOUT):
            return "Escape"

        sequence = bytearray(self._read_input_bytes(fd, 1))

        if not sequence:
            return "Escape"
        if sequence[:1] not in (b"[", b"O"):
            return self._decode_escape_sequence(bytes(sequence))

        while self._read_ready(fd, ESCAPE_SEQUENCE_TIMEOUT):
            sequence.extend(self._read_input_bytes(fd, 1))
            if sequence.startswith(b"[200~"):
                if b"\x1b[201~" in sequence:
                    break
                continue
            if sequence.startswith(b"[M") and len(sequence) < 6:
                continue
            if len(sequence) > 1 and 0x40 <= sequence[-1] <= 0x7e:
                break

        return self._decode_escape_sequence(bytes(sequence))

    def _decode_escape_sequence(self, seq):
        """Decode one ANSI escape sequence captured after ESC."""
        paste = self._decode_bracketed_paste_sequence(seq)
        if paste is not None:
            return paste

        if seq.startswith(b"[<"):
            return self._decode_sgr_mouse_sequence(seq)
        if seq.startswith(b"[M"):
            return None
        if TERMINAL_PIXEL_RESPONSE_BODY_RE.fullmatch(seq):
            return None

        # modifyOtherKeys CSI 27;modifier;keycode~ sequences
        if seq.startswith(b"[27;") and seq.endswith(b"~"):
            try:
                parts = seq[4:-1].split(b";")
                mod, keycode = int(parts[0]), int(parts[1])
                if mod == 3 and keycode == 13:
                    return "Alt+Enter"
                if mod == 5 and 97 <= keycode <= 122:
                    if keycode == ord('c'):
                        raise KeyboardInterrupt
                    return f"ctrl_{chr(keycode)}"
            except (ValueError, IndexError):
                pass

        key_map = {
            b"[A": "Up",
            b"[B": "Down",
            b"[C": "Right",
            b"[D": "Left",
            b"[H": "Home",
            b"[F": "End",
            b"[Z": "Shift+Tab",
            b"OH": "Home",
            b"OF": "End",
            b"[1~": "Home",
            b"[3~": "Delete",
            b"[4~": "End",
            b"[7~": "Home",
            b"[8~": "End",
            b"[1;2A": "Shift+Up",
            b"[1;2B": "Shift+Down",
            b"[1;2C": "Shift+Right",
            b"[1;2D": "Shift+Left",
            b"[1;2H": "Shift+Home",
            b"[1;2F": "Shift+End",
            b"[1;3A": "Alt+Up",
            b"[1;3B": "Alt+Down",
            b"[1;3C": "Alt+Right",
            b"[1;3D": "Alt+Left",
            b"[1;5A": "Ctrl+Up",
            b"[1;5B": "Ctrl+Down",
            b"[1;5C": "Ctrl+Right",
            b"[1;5D": "Ctrl+Left",
            b"b": "Alt+Left",
            b"f": "Alt+Right",
            b"\r": "Alt+Enter",
            b"\n": "Alt+Enter",
            b"\r\n": "Alt+Enter",
            b"[13;3u": "Alt+Enter",
        }
        return key_map.get(seq, "Escape")

    def _decode_bracketed_paste_sequence(self, seq):
        """Decode bracketed paste: ESC [ 200 ~ text ESC [ 201 ~."""
        prefix = b"[200~"
        suffix = b"\x1b[201~"
        if not seq.startswith(prefix):
            return None
        body = seq[len(prefix):]
        if body.endswith(suffix):
            body = body[:-len(suffix)]
        try:
            text = body.decode("utf-8")
        except UnicodeDecodeError:
            text = body.decode("utf-8", errors="ignore")
        return {"type": "paste", "text": text}

    def _decode_sgr_mouse_sequence(self, seq):
        """Decode SGR mouse sequence: ESC [ < button ; col ; row M/m.

        When multiple events arrive concatenated (e.g. rapid trackpad scrolling),
        only the first complete event is decoded.
        """
        try:
            # Find the end of the first complete event (first M or m terminator)
            end = next((i for i, b in enumerate(seq) if b in (ord("M"), ord("m"))), -1)
            if end == -1:
                return None
            seq = seq[: end + 1]

            final = chr(seq[-1])
            body = seq[2:-1].decode("ascii")
            button_text, col_text, row_text = body.split(";")
            button_code = int(button_text)
            col = int(col_text) - MOUSE_COORD_OFFSET
            row = int(row_text) - MOUSE_COORD_OFFSET
        except (ValueError, UnicodeDecodeError):
            return None

        if button_code & MOUSE_WHEEL_FLAG:
            direction = "up" if (button_code & MOUSE_BUTTON_MASK) == 0 else "down"
            return {
                "type": "mouse_wheel",
                "direction": direction,
                "wheel_delta": MOUSE_WHEEL_DELTA_UP if direction == "up" else MOUSE_WHEEL_DELTA_DOWN,
                "row": row,
                "col": col,
            }

        event = "release" if final == "m" else "press"
        if event == "press" and button_code & MOUSE_DRAG_FLAG:
            event = "drag"

        return {
            "type": "mouse",
            "event": event,
            "button": button_code & MOUSE_BUTTON_MASK,
            "row": row,
            "col": col,
        }

    def _read_key(self, fd, timeout=INPUT_POLL_TIMEOUT):
        """Read one logical key from the terminal."""
        if not self._read_ready(fd, timeout):
            return None

        chunk = self._read_input_bytes(fd, 1)
        if not chunk:
            self._running = False
            return None

        code = chunk[0]
        if chunk == b"\x03":
            raise KeyboardInterrupt
        if chunk == b"\x1b":
            key = self._read_escape_sequence(fd)
            if isinstance(key, dict) and key.get("type") == "mouse_wheel":
                return self._coalesce_mouse_wheel_event(fd, key)
            return key
        if chunk in (b"\r", b"\n"):
            return "Enter"
        if chunk == b"\t":
            return "Tab"
        if chunk in (b"\x08", b"\x7f"):
            return "Backspace"

        if 1 <= code <= 26:
            return f"ctrl_{chr(code + 96)}"

        try:
            return chunk.decode("utf-8")
        except UnicodeDecodeError:
            return None

    @staticmethod
    def _mouse_wheel_delta(event):
        if "wheel_delta" in event:
            return int(event.get("wheel_delta") or 0)
        return MOUSE_WHEEL_DELTA_UP if event.get("direction") == "up" else MOUSE_WHEEL_DELTA_DOWN

    @staticmethod
    def _clamp_mouse_wheel_delta(delta):
        return max(-MOUSE_WHEEL_COALESCED_MAX_DELTA, min(MOUSE_WHEEL_COALESCED_MAX_DELTA, int(delta or 0)))

    def _coalesce_mouse_wheel_event(self, fd, event):
        """Drain ready wheel reports into one bounded logical wheel event."""
        total_delta = self._mouse_wheel_delta(event)
        latest = dict(event)
        drained = 1
        while drained < MOUSE_WHEEL_COALESCE_MAX_EVENTS:
            next_event = self._try_read_next_sgr_mouse_wheel_event(fd)
            if next_event is None:
                break
            latest = next_event
            total_delta += self._mouse_wheel_delta(next_event)
            drained += 1

        total_delta = self._clamp_mouse_wheel_delta(total_delta)
        latest = dict(latest)
        latest["type"] = "mouse_wheel"
        latest["wheel_delta"] = total_delta
        if total_delta > 0:
            latest["direction"] = "up"
        elif total_delta < 0:
            latest["direction"] = "down"
        return latest

    def _try_read_next_sgr_mouse_wheel_event(self, fd):
        """Read the next ready SGR wheel event, leaving non-wheel input untouched."""
        if not self._read_ready(fd, MOUSE_WHEEL_COALESCE_READY_TIMEOUT):
            return None

        raw = bytearray()
        raw.extend(self._read_input_bytes(fd, 1))
        if raw != b"\x1b":
            self._unread_input_bytes(raw)
            return None

        for expected in (b"[", b"<"):
            if not self._read_ready(fd, ESCAPE_SEQUENCE_TIMEOUT):
                self._unread_input_bytes(raw)
                return None
            raw.extend(self._read_input_bytes(fd, 1))
            if raw[-1:] != expected:
                self._unread_input_bytes(raw)
                return None

        while len(raw) < MOUSE_WHEEL_SEQUENCE_MAX_BYTES:
            if not self._read_ready(fd, ESCAPE_SEQUENCE_TIMEOUT):
                self._unread_input_bytes(raw)
                return None
            raw.extend(self._read_input_bytes(fd, 1))
            if raw[-1:] in (b"M", b"m"):
                event = self._decode_sgr_mouse_sequence(bytes(raw[1:]))
                if isinstance(event, dict) and event.get("type") == "mouse_wheel":
                    return event
                self._unread_input_bytes(raw)
                return None

        self._unread_input_bytes(raw)
        return None

    def _write_terminal(self, text):
        """Write raw control sequences to stdout."""
        sys.stdout.write(text)
        sys.stdout.flush()

    def _enter_terminal_ui(self):
        """Switch the terminal into fullscreen UI mode."""
        self._terminal_frame_diff.reset()
        self._write_terminal("\x1b[?1049h\x1b[?1000h\x1b[?1002h\x1b[?1006h\x1b[?2004h\x1b[?7l\x1b[?25l\x1b[>4;2m\x1b[H\x1b[2J")

    def _leave_terminal_ui(self):
        """Restore the terminal after fullscreen UI mode."""
        self._terminal_frame_diff.reset()
        self._write_terminal("\x1b[0m\x1b[>4;0m\x1b[?25h\x1b[?7h\x1b[?2004l\x1b[?1006l\x1b[?1002l\x1b[?1000l\x1b[?1049l")

    def _drain_terminal_input(self, fd):
        """Discard pending terminal input before echo mode is restored."""
        reads = 0
        while reads < TERMINAL_EXIT_DRAIN_MAX_READS and self._read_ready(fd, TERMINAL_EXIT_DRAIN_TIMEOUT):
            os.read(fd, TERMINAL_EXIT_DRAIN_BYTES)
            reads += 1

    def _build_terminal_frame(self, lines, col_offset=0, row_offset=0):
        """Build one terminal frame. Offset > 0 clears the screen first."""
        if self._viewport is not None:
            return self._build_viewport_frame(lines, col_offset, row_offset)
        if self._protected_rects:
            return self._build_protected_terminal_frame(lines, col_offset, row_offset)

        parts = []

        if col_offset == 0 and row_offset == 0:
            parts.append("\x1b[H")
            for row, line in enumerate(lines, start=1):
                parts.append(f"\x1b[{row};1H\x1b[2K{line}")
            parts.append("\x1b[J")
        else:
            parts.append("\x1b[H\x1b[2J")
            for row, line in enumerate(lines, start=1):
                parts.append(f"\x1b[{row + row_offset};{col_offset + 1}H\x1b[2K{line}")

        return "".join(parts)

    def _build_terminal_diff_frame(self, layers, protected_rects=None, allow_clear=True):
        """Build a terminal frame by diffing rendered cell layers against the previous frame."""
        if self._viewport is not None:
            if not layers:
                return ""
            viewport = self.get_viewport()
            vp_row = int(viewport["row"])
            vp_col = int(viewport["col"])
            viewport_layers = [
                (rows, int(col_offset) - vp_col, int(row_offset) - vp_row)
                for rows, col_offset, row_offset in layers
            ]
            viewport_protected = self._viewport_relative_rects(
                self._protected_rects if protected_rects is None else protected_rects
            )
            return self._terminal_frame_diff.render(
                viewport_layers,
                protected_rects=viewport_protected,
                allow_clear=False,
                row_offset=vp_row,
                col_offset=vp_col,
            )
        protected = self._protected_rects if protected_rects is None else protected_rects
        return self._terminal_frame_diff.render(
            layers,
            protected_rects=protected,
            allow_clear=allow_clear and not protected,
        )

    def _sync_terminal_diff_frame(self, layers, protected_rects=None):
        if self._viewport is not None:
            viewport = self.get_viewport()
            vp_row = int(viewport["row"])
            vp_col = int(viewport["col"])
            viewport_layers = [
                (rows, int(col_offset) - vp_col, int(row_offset) - vp_row)
                for rows, col_offset, row_offset in layers
            ]
            self._terminal_frame_diff.sync(
                viewport_layers,
                protected_rects=self._viewport_relative_rects(
                    self._protected_rects if protected_rects is None else protected_rects
                ),
            )
            return
        protected = self._protected_rects if protected_rects is None else protected_rects
        self._terminal_frame_diff.sync(layers, protected_rects=protected)

    def _viewport_relative_rects(self, rects):
        if not rects or self._viewport is None:
            return rects or []
        viewport = self.get_viewport()
        vp_row = int(viewport["row"])
        vp_col = int(viewport["col"])
        relative = []
        for rect in rects:
            relative.append({
                "row": int(rect.get("row", 0)) - vp_row,
                "col": int(rect.get("col", 0)) - vp_col,
                "width": int(rect.get("width", 1)),
                "height": int(rect.get("height", 1)),
            })
        return relative

    def _build_protected_terminal_frame(self, lines, col_offset=0, row_offset=0):
        """Build a terminal frame while leaving protected rectangles untouched."""
        parts = ["\x1b[H"]
        for row, line in enumerate(lines):
            target_row = row + row_offset
            line_width = len(self._strip_ansi(line))
            protected = self._protected_segments_for_row(target_row, col_offset, line_width)
            if not protected:
                parts.append(f"\x1b[{target_row + 1};{col_offset + 1}H\x1b[2K{line}")
                continue

            cursor = 0
            for start, end in protected:
                if cursor < start:
                    fragment = self._slice_rendered_text(line, cursor, start - cursor)
                    parts.append(f"\x1b[{target_row + 1};{col_offset + cursor + 1}H{fragment}")
                cursor = max(cursor, end)
            if cursor < line_width:
                fragment = self._slice_rendered_text(line, cursor, line_width - cursor)
                parts.append(f"\x1b[{target_row + 1};{col_offset + cursor + 1}H{fragment}")
        return "".join(parts)

    def _protected_segments_for_row(self, target_row, col_offset, line_width):
        segments = []
        for rect in self._protected_rects:
            top = rect["row"]
            bottom = rect["row"] + rect["height"]
            if target_row < top or target_row >= bottom:
                continue
            start = max(0, rect["col"] - col_offset)
            end = min(line_width, rect["col"] + rect["width"] - col_offset)
            if start < end:
                segments.append((start, end))
        if not segments:
            return []
        segments.sort()
        merged = [segments[0]]
        for start, end in segments[1:]:
            prev_start, prev_end = merged[-1]
            if start <= prev_end:
                merged[-1] = (prev_start, max(prev_end, end))
            else:
                merged.append((start, end))
        return merged

    @staticmethod
    def _slice_rendered_text(line, start, width):
        try:
            from uimd.runtime.UIBase import UIBase
            return UIBase._slice_rendered_text(line, start, width)
        except Exception:
            plain = ANSI_RE.sub("", line or "")
            return plain[start:start + width]

    @staticmethod
    def _clip_rendered_text_width(text, width):
        width = max(0, int(width))
        if width <= 0 or not text:
            return ""
        visible = 0
        index = 0
        parts = []
        for match in ANSI_RE.finditer(text):
            if match.start() > index:
                chunk = text[index:match.start()]
                take = min(len(chunk), width - visible)
                if take > 0:
                    parts.append(chunk[:take])
                    visible += take
                if visible >= width:
                    return "".join(parts)
            parts.append(match.group(0))
            index = match.end()
        if index < len(text) and visible < width:
            chunk = text[index:]
            parts.append(chunk[:width - visible])
        return "".join(parts)

    def _build_viewport_frame(self, lines, col_offset=0, row_offset=0):
        """Build one frame clipped to the rendered window inside the configured viewport."""
        viewport = self.get_viewport()
        vp_row = int(viewport["row"])
        vp_col = int(viewport["col"])
        vp_width = max(1, int(viewport["width"]))
        vp_height = max(1, int(viewport["height"]))
        vp_right = vp_col + vp_width
        content_width = max((len(self._strip_ansi(line)) for line in lines), default=0)
        content_left = int(col_offset)
        content_right = content_left + content_width

        rect = (vp_row, vp_col, vp_width, vp_height)
        if self._viewport_frame_rect != rect:
            self._viewport_frame_rect = rect
            self._viewport_frame_rows = [None] * vp_height
        elif len(self._viewport_frame_rows) != vp_height:
            self._viewport_frame_rows = [None] * vp_height

        parts = []
        for local_row in range(vp_height):
            target_row = vp_row + local_row
            source_row = target_row - row_offset
            rendered_row = None

            draw_col = max(vp_col, content_left)
            draw_right = min(vp_right, content_right)
            if 0 <= source_row < len(lines) and draw_col < draw_right:
                line = lines[source_row]
                line_width = len(self._strip_ansi(line))
                segment_start = draw_col - content_left
                segment_width = draw_right - draw_col
                fragment_width = min(max(0, line_width - segment_start), segment_width)
                fragment = ""
                if fragment_width > 0:
                    fragment = self._slice_rendered_text(line, segment_start, fragment_width)
                    fragment = self._clip_rendered_text_width(fragment, fragment_width)
                visible_width = min(fragment_width, len(self._strip_ansi(fragment)))
                rendered_row = (
                    draw_col,
                    segment_width,
                    fragment + "\x1b[0m" + (" " * max(0, segment_width - visible_width)),
                )

            if self._viewport_frame_rows[local_row] == rendered_row:
                continue
            previous_row = self._viewport_frame_rows[local_row]
            self._viewport_frame_rows[local_row] = rendered_row
            if previous_row is not None:
                previous_col, previous_width, _ = previous_row
                if (
                    rendered_row is None
                    or previous_col != rendered_row[0]
                    or previous_width != rendered_row[1]
                ):
                    parts.append(f"\x1b[0m\x1b[{target_row + 1};{previous_col + 1}H")
                    parts.append(" " * previous_width)
            if rendered_row is not None:
                current_col, _, current_text = rendered_row
                parts.append(f"\x1b[0m\x1b[{target_row + 1};{current_col + 1}H")
                parts.append(current_text)
        return "".join(parts)

    @property
    def is_dirty(self):
        """Check if the application needs re-rendering."""
        return self._dirty

    def clear_dirty(self):
        """Clear the dirty flag after rendering."""
        self._dirty = False

    def _should_render_now(self, fd):
        """Return True when a dirty frame should be flushed to the terminal."""
        if not self._dirty:
            now = time.monotonic()
            if not any(notification["expires_at"] <= now for notification in self._notifications):
                return False
            self._dirty = True
        if not self._read_ready(fd, INPUT_DRAIN_TIMEOUT):
            return True
        if self._last_render_at <= 0:
            return True
        return (time.monotonic() - self._last_render_at) >= RENDER_FRAME_INTERVAL_SECONDS

    def _render_to_terminal(self):
        """Render the current frame into the alternate terminal screen."""
        with self._render_lock:
            if len(self._window_stack) > 1:
                self._pending_terminal_scrolls.clear()
                self._render_dialog_over_background()
            else:
                cell_rows = self.render_cells()
                lines = cells_to_ansi_lines(cell_rows)
                window = self.active_window
                mode = self._resolved_window_mode(window) if window else "fullscreen"

                if mode == "fullscreen":
                    viewport_col = int(self._viewport["col"]) if self._viewport is not None else 0
                    viewport_row = int(self._viewport["row"]) if self._viewport is not None else 0
                    col_offset = viewport_col
                    row_offset = viewport_row
                else:
                    term_w, term_h = self._get_terminal_size()
                    win_w = getattr(window, "_window_width", 0)
                    win_h = len(lines)

                    viewport_col = int(self._viewport["col"]) if self._viewport is not None else 0
                    viewport_row = int(self._viewport["row"]) if self._viewport is not None else 0
                    col_offset = 0 if mode == "expand_width" else max(0, (term_w - win_w) // 2)
                    row_offset = 0 if mode == "expand_height" else max(0, (term_h - win_h) // 2)
                    col_offset += viewport_col
                    row_offset += viewport_row

                optimized_frame = self._build_terminal_scroll_frame(
                    lines, cell_rows, col_offset, row_offset, window, mode
                )
                if optimized_frame is not None:
                    frame = optimized_frame
                else:
                    self._pending_terminal_scrolls.clear()
                    layers = [(cell_rows, col_offset, row_offset)]
                    popup_layer = self._popup_overlay_layer()
                    if popup_layer is not None:
                        layers.append(popup_layer)
                    combobox_layer = self._combobox_overlay_layer()
                    if combobox_layer is not None:
                        layers.append(combobox_layer)
                    frame = self._build_terminal_diff_frame(layers)
                self._write_terminal(frame)

                if window is not None and hasattr(window, "post_render"):
                    window.post_render()

    def _build_terminal_scroll_frame(self, lines, cell_rows, col_offset, row_offset, window, mode):
        if not self._pending_terminal_scrolls:
            return None
        if self._viewport is not None or self._protected_rects:
            return None
        if not self._terminal_frame_diff.ready():
            return None
        if self._active_popup is not None or self._combobox_overlay_active(window):
            return None
        if mode not in ("fullscreen", "expand_width"):
            return None
        term_w, term_h = self._get_terminal_size()
        operation = self._pending_terminal_scrolls[-1]
        if len(self._pending_terminal_scrolls) != 1:
            return None
        if int(operation["col"]) + int(col_offset) != 0:
            return None
        if int(operation["width"]) < term_w:
            return None
        target_top = int(row_offset) + int(operation["row"])
        height = int(operation["height"])
        target_bottom = target_top + height
        if target_top < 0 or target_bottom > term_h or height <= 1:
            return None
        delta = int(operation["delta"])
        distance = min(abs(delta), height)
        if distance <= 0 or distance >= height:
            return None

        self._pending_terminal_scrolls.clear()
        command = "T" if delta > 0 else "S"
        parts = [
            f"\x1b[{target_top + 1};{target_bottom}r",
            f"\x1b[{target_top + 1};1H",
            f"\x1b[{distance}{command}",
            "\x1b[r",
        ]
        rows_to_repaint = set()
        if delta > 0:
            rows_to_repaint.update(range(target_top, target_top + distance))
        else:
            rows_to_repaint.update(range(target_bottom - distance, target_bottom))
        rows_to_repaint.add(target_top)
        rows_to_repaint.add(target_bottom - 1)
        for row in sorted(rows_to_repaint):
            source_index = row - int(row_offset)
            line = lines[source_index] if 0 <= source_index < len(lines) else ""
            visible_width = len(self._strip_ansi(line))
            parts.append(f"\x1b[{row + 1};1H\x1b[2K{line}")
            if visible_width < term_w:
                parts.append(" " * (term_w - visible_width))
        parts.append("\x1b[0m")
        self._sync_terminal_diff_frame([(cell_rows, col_offset, row_offset)], protected_rects=[])
        return "".join(parts)

    @staticmethod
    def _combobox_overlay_active(window):
        if window is None or not getattr(window, "_edit_mode", False):
            return False
        element = getattr(window, "_focused_element", None)
        return (
            element is not None and
            getattr(element, "ELEMENT_TYPE", None) == "combobox" and
            bool(getattr(element, "_menu_open", False))
        )

    def _build_popup_overlay(self):
        """Build ANSI escape string that draws the popup over the current frame."""
        popup = self._active_popup
        if popup is None:
            return ""
        term_w, term_h = self._get_terminal_size()
        popup_rows = self._render_window_cells(popup)
        if not popup_rows:
            return ""
        row_off, col_off = popup.get_popup_position(term_w, term_h)
        if self._viewport is not None:
            viewport = self.get_viewport()
            return self._build_positioned_overlay_frame(
                popup_rows,
                col_off + int(viewport["col"]),
                row_off + int(viewport["row"]),
            )
        return "".join(
            f"\x1b[{row_off + i + 1};{col_off + 1}H{self._row_to_ansi(row)}"
            for i, row in enumerate(popup_rows)
        )

    def _popup_overlay_layer(self):
        popup = self._active_popup
        if popup is None:
            return None
        term_w, term_h = self._get_terminal_size()
        popup_rows = self._render_window_cells(popup) if hasattr(popup, "render_cells") else [parse_ansi_cells(line) for line in popup.render()]
        if not popup_rows:
            return None
        row_off, col_off = popup.get_popup_position(term_w, term_h)
        if self._viewport is not None:
            viewport = self.get_viewport()
            col_off += int(viewport["col"])
            row_off += int(viewport["row"])
        return popup_rows, col_off, row_off

    def _build_combobox_overlay(self):
        """Build an ANSI overlay for an open combobox dropdown above all windows."""
        window = self.active_window
        if window is None or not getattr(window, "_edit_mode", False):
            return ""
        element = getattr(window, "_focused_element", None)
        if element is None or getattr(element, "ELEMENT_TYPE", None) != "combobox":
            return ""
        if not getattr(element, "_menu_open", False):
            return ""

        rect = window._element_focus_rect(element)
        rows = element.render_cells() if hasattr(element, "render_cells") else [parse_ansi_cells(line) for line in element.render()]
        if not rows:
            return ""
        col_offset, row_offset = self._window_offsets(window)
        return self._build_positioned_overlay_frame(
            rows,
            col_offset + rect["left"],
            row_offset + rect["top"],
        )

    def _combobox_overlay_layer(self):
        window = self.active_window
        if window is None or not getattr(window, "_edit_mode", False):
            return None
        element = getattr(window, "_focused_element", None)
        if element is None or getattr(element, "ELEMENT_TYPE", None) != "combobox":
            return None
        if not getattr(element, "_menu_open", False):
            return None

        rect = window._element_focus_rect(element)
        rows = element.render_cells() if hasattr(element, "render_cells") else [parse_ansi_cells(line) for line in element.render()]
        if not rows:
            return None
        col_offset, row_offset = self._window_offsets(window)
        return rows, col_offset + rect["left"], row_offset + rect["top"]

    def _render_dialog_over_background(self):
        """Render all background windows dimmed, then overlay the active dialog."""
        frame = ""
        layers = []
        last_index = len(self._window_stack) - 1
        for index, window in enumerate(self._window_stack):
            mode = self._resolved_window_mode(window)
            saved_mode = window.mode
            window.mode = mode
            self._resize_window(window)
            if index == last_index:
                cell_rows = self._render_window_cells(window)
            else:
                from uimd.runtime.image import force_image_cell_background_rendering
                with force_image_cell_background_rendering():
                    cell_rows = self._render_window_cells(window)
            window.mode = saved_mode

            if index == last_index:
                cell_rows = self._render_notifications_cells(cell_rows)
            else:
                cell_rows = self._dim_cell_rows(cell_rows)

            col_offset, row_offset = self._window_offsets(window, mode)

            layers.append((cell_rows, col_offset, row_offset))

        popup_layer = self._popup_overlay_layer()
        if popup_layer is not None:
            layers.append(popup_layer)
        combobox_layer = self._combobox_overlay_layer()
        if combobox_layer is not None:
            layers.append(combobox_layer)
        frame = self._build_terminal_diff_frame(layers)
        self._write_terminal(frame)

    def _build_positioned_overlay_frame(self, lines, col_offset, row_offset):
        """Build a positioned overlay frame, clipped to the configured viewport."""
        if self._viewport is None:
            return "".join(
                f"\x1b[{row_offset + i + 1};{col_offset + 1}H{self._row_to_ansi(line)}"
                for i, line in enumerate(lines)
            )

        viewport = self.get_viewport()
        vp_row = int(viewport["row"])
        vp_col = int(viewport["col"])
        vp_right = vp_col + max(1, int(viewport["width"]))
        vp_bottom = vp_row + max(1, int(viewport["height"]))

        parts = []
        for row, line in enumerate(lines):
            target_row = row_offset + row
            if target_row < vp_row or target_row >= vp_bottom:
                continue
            line_cells = self._row_to_cells(line)
            line_width = len(line_cells)
            start = max(0, vp_col - col_offset)
            end = min(line_width, vp_right - col_offset)
            if start >= end:
                continue
            fragment = self._row_to_ansi(line_cells[start:start + end - start])
            parts.append(f"\x1b[{target_row + 1};{col_offset + start + 1}H{fragment}")
        return "".join(parts)

    @staticmethod
    def _dim_lines(lines):
        """Darken all 24-bit RGB colors in rendered lines by BACKGROUND_DIM_FACTOR."""
        def _darken(m):
            r = int(int(m.group(2)) * BACKGROUND_DIM_FACTOR)
            g = int(int(m.group(3)) * BACKGROUND_DIM_FACTOR)
            b = int(int(m.group(4)) * BACKGROUND_DIM_FACTOR)
            return f"\x1b[{m.group(1)}{r};{g};{b}m"
        return [_RGB_COLOR_RE.sub(_darken, line) for line in lines]

    @staticmethod
    def _dim_color_value(color):
        if color is None:
            return None
        if isinstance(color, str):
            try:
                if color.startswith("#") and len(color) >= 7:
                    r = int(int(color[1:3], 16) * BACKGROUND_DIM_FACTOR)
                    g = int(int(color[3:5], 16) * BACKGROUND_DIM_FACTOR)
                    b = int(int(color[5:7], 16) * BACKGROUND_DIM_FACTOR)
                    return f"#{r:02x}{g:02x}{b:02x}"
                if len(color) == 9 and color.isdigit():
                    r = int(int(color[0:3]) * BACKGROUND_DIM_FACTOR)
                    g = int(int(color[3:6]) * BACKGROUND_DIM_FACTOR)
                    b = int(int(color[6:9]) * BACKGROUND_DIM_FACTOR)
                    return f"{r:03d}{g:03d}{b:03d}"
            except (ValueError, IndexError):
                return color
            return color
        red = getattr(color, "r", None)
        green = getattr(color, "g", None)
        blue = getattr(color, "b", None)
        if red is None or green is None or blue is None:
            return color
        try:
            return type(color)(
                f"#{int(red * BACKGROUND_DIM_FACTOR):02x}"
                f"{int(green * BACKGROUND_DIM_FACTOR):02x}"
                f"{int(blue * BACKGROUND_DIM_FACTOR):02x}"
            )
        except Exception:
            return color

    @classmethod
    def _dim_cell_rows(cls, rows):
        return [
            [
                TerminalCell(
                    cell.text,
                    cls._dim_color_value(cell.foreground),
                    cls._dim_color_value(cell.background),
                    cell.raw,
                    cell.raw_width,
                    cell.raw_height,
                    cell.raw_skip,
                )
                for cell in row
            ]
            for row in rows
        ]

    def run(self):
        """Run the application main loop."""
        mcp_config = parse_mcp_runtime_args()
        if mcp_config.viewport is not None:
            self.set_viewport(**mcp_config.viewport)
        if mcp_config.enabled:
            if not self._mcp_available():
                print("MCP support is disabled for this generated UI.", file=sys.stderr)
                return 2
            if not mcp_config.gui:
                return self._run_mcp_headless(mcp_config)
            mcp_server = MCPServer(self, mcp_config)
            try:
                mcp_server.start_background()
            except ValueError as exc:
                print(str(exc), file=sys.stderr)
                return 2
            return self._run_gui_loop(
                close_mcp_server=mcp_server,
                embedded_viewport=mcp_config.viewport is not None,
                controlled_render=mcp_config.controlled_render,
            )

        return self._run_gui_loop()

    def _mcp_available(self):
        """Return whether every open generated window allows MCP."""
        for window in self._window_stack:
            if not bool(getattr(window, "_mcp_enabled", True)):
                return False
        return True

    def _run_mcp_headless(self, mcp_config):
        """Run without terminal drawing, driven only through MCP."""
        self._running = True
        if not self._window_stack:
            return 0
        server = MCPServer(self, mcp_config)
        try:
            server.serve_forever()
        except KeyboardInterrupt:
            pass
        finally:
            server.shutdown()
            for window in self._window_stack:
                window.close()
        return 0

    def _run_gui_loop(self, close_mcp_server=None, embedded_viewport=False, controlled_render=False):
        """Run the application main loop with terminal UI rendering."""
        self._running = True

        if not self._window_stack:
            return 0

        stdin = sys.stdin
        stdout = sys.stdout
        if embedded_viewport and stdout.isatty():
            try:
                while self._running:
                    if not controlled_render and self._dirty:
                        self._render_to_terminal()
                        self._last_render_at = time.monotonic()
                        self.clear_dirty()
                    if not controlled_render and self.active_window and hasattr(self.active_window, 'tick'):
                        self.active_window.tick()
                    time.sleep(INPUT_POLL_TIMEOUT)
            finally:
                if close_mcp_server is not None:
                    close_mcp_server.shutdown()
                for window in self._window_stack:
                    window.close()
            return 0

        if not stdin.isatty() or not stdout.isatty():
            if close_mcp_server is not None:
                try:
                    while self._running:
                        time.sleep(0.05)
                finally:
                    close_mcp_server.shutdown()
                return 0
            try:
                num_frames = 30
                for frame_idx in range(num_frames):
                    if frame_idx > 0:
                        sys.stdout.write("\033[2J\033[H")
                    for line in self.render():
                        print(line)
                    if frame_idx < num_frames - 1:
                        time.sleep(0.1)
            finally:
                if close_mcp_server is not None:
                    close_mcp_server.shutdown()
            return 0

        fd = stdin.fileno()
        old_term_attrs = termios.tcgetattr(fd)
        previous_winch_handler = signal.getsignal(signal.SIGWINCH)

        # Restore the terminal on signals that would otherwise bypass the
        # `finally` cleanup (SIGTERM from `kill`, SIGHUP on disconnect). Without
        # this the alternate screen and mouse tracking stay enabled, so the shell
        # prints raw mouse-report sequences when the user scrolls.
        _terminate_signals = [
            getattr(signal, name)
            for name in ("SIGTERM", "SIGHUP")
            if hasattr(signal, name)
        ]
        previous_terminate_handlers = {
            signum: signal.getsignal(signum) for signum in _terminate_signals
        }

        def _handle_resize(signum, frame):
            self._needs_resize = True

        def _handle_terminate(signum, frame):
            raise KeyboardInterrupt

        self._needs_resize = True

        try:
            tty.setraw(fd)
            signal.signal(signal.SIGWINCH, _handle_resize)
            for signum in _terminate_signals:
                signal.signal(signum, _handle_terminate)
            self._sync_terminal_cell_px(fd)
            self._enter_terminal_ui()

            while self._running:
                if self._needs_resize:
                    self._needs_resize = False
                    self._sync_terminal_cell_px(fd)
                    self._sync_active_window_size()

                if self._should_render_now(fd):
                    self._render_to_terminal()
                    self._last_render_at = time.monotonic()
                    self.clear_dirty()

                key = self._read_key(fd)
                if key is None:
                    if self.active_window and hasattr(self.active_window, 'tick'):
                        self.active_window.tick()
                    continue
                self.handle_key(key)
        except KeyboardInterrupt:
            pass
        finally:
            signal.signal(signal.SIGWINCH, previous_winch_handler)
            for signum, handler in previous_terminate_handlers.items():
                signal.signal(signum, handler)
            for window in self._window_stack:
                window.close()
            if close_mcp_server is not None:
                close_mcp_server.shutdown()
            self._leave_terminal_ui()
            self._drain_terminal_input(fd)
            termios.tcsetattr(fd, termios.TCSADRAIN, old_term_attrs)

        return 0
