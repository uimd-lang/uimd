"""Tests for UIApplication class."""

import sys
import os
import time
import tempfile
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'examples', 'activity_feed'))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'examples', 'calculator'))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'examples', 'cells'))

import unittest
from runtime.application import MOUSE_WHEEL_COALESCED_MAX_DELTA, UIApplication
from runtime.uiwindow import UIWindow
from runtime.UIScrollView import UIScrollView
from runtime.UIBase import _strip_ansi
from runtime.elements import Button, ComboBox, Label, ListBox, NumberInput, TextArea, TextInput
from runtime.rendering import (
    ANSI_CLEAR_SCREEN,
    ANSI_SYNC_UPDATE_BEGIN,
    ANSI_SYNC_UPDATE_END,
    TerminalBuffer,
    TerminalCell,
)
from runtime.image import reset_terminal_cell_px_cache, _terminal_cell_px
from runtime.style import Color, Style
from activity_feed_ui import ActivityFeedUI
from calculator_ui import CalculatorUI
from cells_ui import CellsUI
from dialogs import FileBrowser


class RawCellsElement:
    ELEMENT_TYPE = "image"

    def __init__(self, name):
        self.name = name
        self.row = 0
        self.col = 0
        self.width = 1
        self.height = 1
        self.focusable = False
        self.enabled = True
        self.style = Style()
        self.parent = None
        self._app = None

    def get_style(self):
        return self.style

    def open(self):
        pass

    def render_cells(self):
        width = max(1, int(self.width or 1))
        row = [
            TerminalCell(" ", raw_skip=True)
            for _ in range(width)
        ]
        row[0] = TerminalCell(" ", raw="RAW", raw_width=width, raw_height=1)
        return [row]


class FixedRenderChild:
    def __init__(self, lines):
        self._lines = list(lines)
        self.parent = None
        self._app = None

    def render(self):
        return list(self._lines)


class ClearOnlySelectionChild:
    def __init__(self):
        self.parent = None
        self._app = None
        self.cleared = False
        self.render_calls = 0

    def render(self):
        self.render_calls += 1
        raise AssertionError("selection cleanup must not render scrollview children")

    def _clear_label_selections(self):
        self.cleared = True


class FixedRenderWindow:
    def __init__(self, lines, natural_width=None, natural_height=None, fullscreen=False):
        self._lines = list(lines)
        self._natural_width = natural_width
        self._natural_height = natural_height
        self._layout = []
        if fullscreen:
            self._layout = [{
                "cell_width_mode": "expanded",
                "cell_height_mode": "expanded",
            }]
        self._window_width = natural_width or max((len(line) for line in self._lines), default=1)
        self._window_height = natural_height or len(self._lines)
        self.mode = "normal"
        self.parent = None
        self._app = None

    def get_natural_size(self):
        return self._window_width, self._window_height

    def resize(self, width, height):
        self._window_width = width
        self._window_height = height

    def open(self):
        pass

    def close(self):
        pass

    def render(self):
        width = max(1, int(self._window_width))
        height = max(1, int(self._window_height))
        lines = list(self._lines)[:height]
        lines += [""] * max(0, height - len(lines))
        return [line[:width].ljust(width) for line in lines]


class FixedPopup:
    def __init__(self, lines, row=0, col=0):
        self._lines = list(lines)
        self._row = row
        self._col = col

    def render(self):
        return list(self._lines)

    def get_popup_position(self, term_w, term_h):
        return self._row, self._col


class PlainScrollView(UIScrollView):
    def __init__(self, width, height, children=None, gap=0):
        super().__init__()
        self._viewport = {
            "row": 0,
            "col": 0,
            "width": width,
            "height": height,
            "gap": gap,
        }
        self._window_width = width
        self._window_height = height
        for child in children or []:
            self.add_child(child)

    def _get_viewport_rect(self):
        return dict(self._viewport)

    def _base_element_grid(self):
        return {}

    def _render(self, elem_grid):
        lines = []
        width = self._viewport["width"]
        for row in range(self._viewport["height"]):
            chars = [" "] * width
            for col, text in sorted(elem_grid.get(row, {}).items()):
                for offset, ch in enumerate(text):
                    target_col = col + offset
                    if 0 <= target_col < width:
                        chars[target_col] = ch
            lines.append("".join(chars))
        return lines


class TestUIApplication(unittest.TestCase):
    """Test cases for the UIApplication class."""

    def test_app_default_size(self):
        """Test application default size."""
        from runtime.application import FIT_CONTENT
        app = UIApplication()
        self.assertEqual(app._width, FIT_CONTENT)
        self.assertEqual(app._height, FIT_CONTENT)

    def test_app_custom_size(self):
        """Test application custom size."""
        app = UIApplication(width=100, height=30)
        self.assertEqual(app._width, 100)
        self.assertEqual(app._height, 30)

    def test_app_no_windows(self):
        """Test app with no windows."""
        app = UIApplication()
        self.assertIsNone(app.active_window)
        self.assertEqual(app.window_count, 0)

    def test_app_open_window(self):
        """Test opening a window."""
        app = UIApplication()
        w = UIWindow(title="Test")
        app.open(w)
        self.assertEqual(app.window_count, 1)
        self.assertEqual(app.active_window, w)

    def test_app_open_second_window(self):
        """Test opening a second window."""
        app = UIApplication()
        w1 = UIWindow(title="Window 1")
        w2 = UIWindow(title="Window 2")

        app.open(w1)
        self.assertEqual(app.window_count, 1)

        app.open(w2)
        self.assertEqual(app.window_count, 2)
        self.assertEqual(app.active_window, w2)

    def test_app_second_window_dims_first(self):
        """Test that opening a second window dims the first."""
        app = UIApplication()
        w1 = UIWindow(title="Window 1")
        w2 = UIWindow(title="Window 2")

        app.open(w1)
        self.assertEqual(w1.mode, "normal")

        app.open(w2)
        self.assertEqual(w1.mode, "dialog")
        self.assertIn(w2.mode, ("normal", "fullscreen"))

    def test_app_close_current(self):
        """Test closing the current window."""
        app = UIApplication()
        w1 = UIWindow(title="Window 1")
        w2 = UIWindow(title="Window 2")

        app.open(w1)
        app.open(w2)
        self.assertEqual(app.window_count, 2)

        app.close_current()
        self.assertEqual(app.window_count, 1)
        self.assertEqual(app.active_window, w1)

    def test_app_close_last_window(self):
        """Test closing the last window."""
        app = UIApplication()
        w1 = UIWindow(title="Window 1")

        app.open(w1)
        self.assertEqual(app.window_count, 1)

        app.close_current()
        self.assertEqual(app.window_count, 0)
        self.assertIsNone(app.active_window)

    def test_app_handle_key_no_windows(self):
        """Test handling key with no windows."""
        app = UIApplication()
        result = app.handle_key("Escape")
        self.assertFalse(result)

    def test_app_render_no_windows(self):
        """Test rendering with no windows."""
        app = UIApplication()
        lines = app.render()
        self.assertEqual(lines, [])

    def test_app_render_one_window(self):
        """Test rendering with one window."""
        app = UIApplication()
        w = UIWindow(title="Test")
        app.open(w)

        lines = app.render()
        self.assertGreater(len(lines), 0)

    def test_notification_renders_over_window(self):
        """Notifications should render as a transient overlay."""
        app = UIApplication(width=40, height=6)
        w = UIWindow(title="Test")
        app.open(w)

        app.notify("Copied to clipboard")
        lines = app.render()

        self.assertTrue(any("Copied to clipboard" in line for line in lines))
        self.assertIn("Copied to clipboard", lines[1])

    def test_notification_overlay_strips_ansi_cleanly(self):
        """Notification overlay should not leave fragments of ANSI escape codes."""
        app = UIApplication(width=40, height=6)
        line = "\x1b[48;2;0;0;0m" + (" " * 40) + "\x1b[0m"

        rendered = app._overlay_plain_text(line, 10, "Copied")

        self.assertIn("Copied", rendered)
        self.assertNotIn("48;2", rendered)
        self.assertNotIn("[0m", rendered)

    def test_notification_expires(self):
        """Expired notifications should be removed from render output."""
        app = UIApplication(width=40, height=6)
        w = UIWindow(title="Test")
        app.open(w)

        app.notify("Short", duration=0)
        lines = app.render()

        self.assertFalse(any("Short" in line for line in lines))

    def test_build_terminal_frame_positions_each_row_explicitly(self):
        """Fullscreen terminal frames should not rely on newline wrapping."""
        app = UIApplication()

        frame = app._build_terminal_frame(["abc", "def"])

        self.assertEqual(frame, "\x1b[H\x1b[1;1H\x1b[2Kabc\x1b[2;1H\x1b[2Kdef\x1b[J")
        self.assertNotIn("\n", frame)

    def test_build_terminal_frame_does_not_clear_after_line_content(self):
        """Clearing after a full-width line can erase the final painted column."""
        app = UIApplication()

        frame = app._build_terminal_frame(["abc"])

        self.assertNotIn("abc\x1b[K", frame)
        self.assertIn("\x1b[2Kabc", frame)

    def test_build_offset_terminal_frame_clears_before_line_content(self):
        """Offset frames use the same clear-before-write behavior."""
        app = UIApplication()

        frame = app._build_terminal_frame(["abc"], col_offset=2, row_offset=3)

        self.assertIn("\x1b[4;3H\x1b[2Kabc", frame)
        self.assertNotIn("abc\x1b[K", frame)

    def test_terminal_diff_frame_emits_only_changed_runs(self):
        """Backend diff should repaint only changed contiguous cells."""
        app = UIApplication()
        app._get_terminal_size = lambda: (8, 2)

        first = app._build_terminal_diff_frame([(["abc"], 0, 0)])
        second = app._build_terminal_diff_frame([(["axc"], 0, 0)])

        self.assertIn("\x1b[H\x1b[2J", first)
        self.assertIn("\x1b[1;1H", first)
        self.assertIn("abc", first)
        self.assertNotIn("\x1b[H\x1b[2J", second)
        self.assertIn("\x1b[1;2H", second)
        self.assertIn("x", second)
        self.assertNotIn("abc", second)

    def test_terminal_diff_frame_preserves_protected_rects(self):
        """Protected rectangles must not be overwritten by backend diff output."""
        app = UIApplication()
        app._get_terminal_size = lambda: (8, 2)
        app._build_terminal_diff_frame([(["abcdef"], 0, 0)])
        app.set_protected_rects([{"row": 0, "col": 1, "width": 3, "height": 1}])

        frame = app._build_terminal_diff_frame([(["zXYZgh"], 0, 0)])

        self.assertNotIn("\x1b[1;2H", frame)
        self.assertNotIn("XYZ", frame)
        self.assertIn("\x1b[1;1H", frame)
        self.assertIn("\x1b[1;5H", frame)

    def test_terminal_diff_frame_forces_clear_on_resize(self):
        """Terminal size changes force a full clear before the next diff frame."""
        app = UIApplication()
        size = {"value": (8, 2)}
        app._get_terminal_size = lambda: size["value"]
        app._build_terminal_diff_frame([(["abc"], 0, 0)])

        size["value"] = (10, 2)
        frame = app._build_terminal_diff_frame([(["abc"], 0, 0)])

        self.assertIn("\x1b[H\x1b[2J", frame)

    def test_terminal_buffer_resize_same_size_preserves_previous(self):
        """TerminalBuffer.resize is a no-op when dimensions are unchanged."""
        buffer = TerminalBuffer(4, 2)
        buffer.set_cell(0, 0, TerminalCell("A"))
        buffer.render_diff()

        self.assertFalse(buffer.resize(4, 2))
        buffer.clear()
        buffer.set_cell(0, 0, TerminalCell("A"))

        self.assertEqual(buffer.render_diff(), "")

    def test_terminal_buffer_changed_run_only(self):
        """TerminalBuffer diff emits only the changed contiguous run."""
        buffer = TerminalBuffer(4, 1)
        for col, char in enumerate("abcd"):
            buffer.set_cell(0, col, TerminalCell(char))
        buffer.render_diff()

        buffer.clear()
        for col, char in enumerate("axcd"):
            buffer.set_cell(0, col, TerminalCell(char))
        frame = buffer.render_diff()

        self.assertIn("\x1b[1;2H", frame)
        self.assertIn("x", frame)
        self.assertNotIn("abcd", frame)

    def test_request_full_redraw_repaints_without_clear_screen(self):
        """Forced redraw repaints existing cells without clearing the terminal."""
        app = UIApplication()
        app._get_terminal_size = lambda: (4, 1)
        app._build_terminal_diff_frame([([[TerminalCell("A")]], 0, 0)])

        app.request_full_redraw()
        frame = app._build_terminal_diff_frame([([[TerminalCell("A")]], 0, 0)])

        self.assertNotIn(ANSI_CLEAR_SCREEN, frame)
        self.assertIn("\x1b[1;1H", frame)
        self.assertIn("A", frame)

    def test_terminal_buffer_raw_diff_uses_synchronized_update(self):
        """Raw image payloads are emitted inside a synchronized terminal update."""
        buffer = TerminalBuffer(2, 1)
        buffer.set_cell(0, 0, TerminalCell(" ", raw="RAW", raw_width=2, raw_height=1))
        buffer.set_cell(0, 1, TerminalCell(" ", raw_skip=True))

        frame = buffer.render_diff()

        self.assertTrue(frame.startswith(ANSI_SYNC_UPDATE_BEGIN))
        self.assertTrue(frame.endswith(ANSI_SYNC_UPDATE_END))
        self.assertLess(frame.index(ANSI_SYNC_UPDATE_BEGIN), frame.index("RAW"))
        self.assertLess(frame.index("RAW"), frame.index(ANSI_SYNC_UPDATE_END))

    def test_terminal_cell_pixel_response_is_ignored_as_key(self):
        """Late CSI 16 t responses must not become Escape key events."""
        app = UIApplication()

        self.assertIsNone(app._decode_escape_sequence(b"[6;10;8t"))
        self.assertIsNone(app._decode_escape_sequence(b"[4;350;720t"))

    def test_sync_terminal_cell_px_updates_image_renderer(self):
        """The application supplies discovered cell pixel metrics to Image."""
        reset_terminal_cell_px_cache()
        self.addCleanup(reset_terminal_cell_px_cache)
        app = UIApplication()
        app._terminal_cell_px_from_ioctl = lambda: None
        app._query_terminal_cell_px = lambda _fd: (8, 10)

        app._sync_terminal_cell_px(0)

        self.assertEqual(_terminal_cell_px(), (8, 10))

    def test_terminal_cell_px_falls_back_to_text_area_report(self):
        """CSI 14 t text-area pixels can derive cell pixels when CSI 16 t is absent."""
        app = UIApplication()
        app._get_terminal_size = lambda: (90, 35)

        def query(_fd, sent_query, _response_re):
            if sent_query == "\x1b[16t":
                return None
            return (720, 350)

        app._query_terminal_pixel_report = query

        self.assertEqual(app._query_terminal_cell_px(0), (8, 10))

    def test_viewport_diff_frame_repaints_text_after_raw_overlay(self):
        """Viewport rendering must use the same raw repaint path as full-screen rendering."""
        app = UIApplication()
        app.set_viewport(2, 5, 12, 5)
        raw_rows = [
            [TerminalCell(" ", raw="RAW", raw_width=8, raw_height=4)]
            + [TerminalCell(" ", raw_skip=True) for _ in range(7)]
        ]
        raw_rows.extend([
            [TerminalCell(" ", raw_skip=True) for _ in range(8)]
            for _row in range(3)
        ])
        close_row = [[TerminalCell(ch) for ch in " [Close] "]]

        frame = app._build_terminal_diff_frame([
            (raw_rows, 5, 2),
            (close_row, 5, 5),
        ])

        self.assertNotIn(ANSI_CLEAR_SCREEN, frame)
        self.assertTrue(frame.startswith(ANSI_SYNC_UPDATE_BEGIN))
        self.assertIn("RAW", frame)
        self.assertIn("Close", frame)
        self.assertGreater(frame.rfind("Close"), frame.find("RAW"))

    def test_layout_order_renders_controls_over_raw_image_cells(self):
        """Later layout entries must be able to cover raw image cells."""
        layout = [
            {
                "name": "photo",
                "type": "image",
                "row": 0,
                "col": 0,
                "cell_row": 0,
                "cell_col": 0,
                "cell_chars_width": 12,
                "cell_chars_height": 1,
                "cell_name": "dialog",
                "width": 12,
                "height": 1,
                "chars_width": 12,
                "chars_height": 1,
            },
            {
                "name": "close_btn",
                "type": "button",
                "row": 0,
                "col": 2,
                "cell_row": 0,
                "cell_col": 0,
                "cell_chars_width": 12,
                "cell_chars_height": 1,
                "cell_name": "dialog",
                "width": 9,
                "height": 1,
                "chars_width": 9,
                "chars_height": 1,
                "margin_right": 8,
            },
        ]
        window = UIWindow(layout=layout, members={})
        window.create_element("close_btn", "button", title="Close")
        raw = RawCellsElement("photo")
        raw.parent = window
        window._elements["photo"] = raw
        window.resize(12, 1)
        window.apply_layout()

        rendered = window.render_cells()
        text = "".join(cell.normalized_text() for cell in rendered[0])

        self.assertIn("Close", text)
        for col in range(text.index("Close"), text.index("Close") + len("Close")):
            self.assertFalse(rendered[0][col].raw_skip)
            self.assertEqual(rendered[0][col].raw, "")

    def test_fit_content_control_touching_expanded_cell_bottom_stays_at_source_row(self):
        """Touching the source bottom edge should not bottom-align a fit-content control."""
        layout = [
            {
                "name": "detail",
                "type": "label",
                "row": 0,
                "col": 0,
                "cell_row": 0,
                "cell_col": 0,
                "cell_chars_width": 10,
                "cell_chars_height": 4,
                "cell_name": "dialog",
                "cell_width": None,
                "cell_height": None,
                "cell_width_mode": "expanded",
                "cell_height_mode": "expanded",
                "width": None,
                "height": None,
                "width_mode": "expanded",
                "height_mode": "fit-content",
                "chars_width": 10,
                "chars_height": 4,
                "margin_right": 0,
                "margin_bottom": 0,
            },
        ]
        window = UIWindow(layout=layout, members={}, style={"this": {"border-width": "0"}})
        window.create_element("detail", "label", text="Details")
        window.mode = "fullscreen"
        window.resize(20, 12)

        elements = {
            entry["name"]: entry
            for entry in window.export_resolved_layout()["elements"]
        }

        self.assertEqual(elements["detail"]["row"], 0)
        self.assertEqual(elements["detail"]["height"], 1)

    def test_fixed_control_below_expanded_height_entry_anchors_to_bottom(self):
        """Controls after an expanded-height entry keep their bottom-side spacing."""
        layout = [
            {
                "name": "photo",
                "type": "label",
                "row": 0,
                "col": 0,
                "cell_row": 0,
                "cell_col": 0,
                "cell_chars_width": 10,
                "cell_chars_height": 6,
                "cell_name": "dialog",
                "cell_width": None,
                "cell_height": None,
                "cell_width_mode": "expanded",
                "cell_height_mode": "expanded",
                "width": None,
                "height": None,
                "width_mode": "expanded",
                "height_mode": "expanded",
                "chars_width": 10,
                "chars_height": 4,
                "margin_right": 0,
                "margin_bottom": 2,
            },
            {
                "name": "close_btn",
                "type": "button",
                "row": 5,
                "col": 3,
                "cell_row": 0,
                "cell_col": 0,
                "cell_chars_width": 10,
                "cell_chars_height": 6,
                "cell_name": "dialog",
                "cell_width": None,
                "cell_height": None,
                "cell_width_mode": "expanded",
                "cell_height_mode": "expanded",
                "width": 5,
                "height": 1,
                "chars_width": 5,
                "chars_height": 1,
                "margin_right": 2,
                "margin_bottom": 0,
            },
        ]
        window = UIWindow(layout=layout, members={}, style={"this": {"border-width": "0"}})
        window.create_element("photo", "label", text="photo")
        window.create_element("close_btn", "button", title="Close")
        window.mode = "fullscreen"
        window.resize(20, 12)

        elements = {
            entry["name"]: entry
            for entry in window.export_resolved_layout()["elements"]
        }

        self.assertEqual(elements["photo"]["height"], 10)
        self.assertEqual(elements["close_btn"]["row"], 11)
        self.assertEqual(
            elements["close_btn"]["row"],
            elements["photo"]["row"] + elements["photo"]["height"] + 1,
        )

    def test_disabled_button_is_not_focusable_or_activatable(self):
        """Disabled controls should render disabled and ignore focus/activation."""
        window = UIWindow(layout=[], members={})
        button = window.create_element("save", "button", title="Save")
        button.disabled_style = Style()
        button.disabled_style.set("color", "#64748b")
        button.enabled = False

        window.set_focus(button)

        self.assertIsNone(window._focused_element)
        self.assertEqual(window.get_focusable_elements(), [])
        self.assertFalse(window._activate_element(button))
        self.assertEqual(button.get_style().color, Color("#64748b"))

    def test_terminal_buffer_scroll_region_shifts_previous(self):
        """Scroll-region optimization updates previous before exposed rows repaint."""
        buffer = TerminalBuffer(1, 3)
        for row, char in enumerate("abc"):
            buffer.set_cell(row, 0, TerminalCell(char))
        buffer.render_diff()

        buffer.clear()
        for row, char in enumerate("bcd"):
            buffer.set_cell(row, 0, TerminalCell(char))
        scroll = buffer.render_scroll_region(0, 0, 3, -1)
        diff = buffer.render_diff()

        self.assertIn("\x1b[1S", scroll)
        self.assertIn("d", diff)
        self.assertNotIn("b", diff)
        self.assertNotIn("c", diff)

    def test_focus_only_button_movement_changes_old_and_new_regions(self):
        """Moving focus between buttons should diff only the affected button runs."""
        app = UIApplication()
        app._get_terminal_size = lambda: (32, 1)
        left = Button(name="left", title="One", width=9)
        right = Button(name="right", title="Two", width=9)

        def frame(left_focused):
            left.focused = left_focused
            right.focused = not left_focused
            row = [TerminalCell(" ") for _ in range(32)]
            for col, cells in ((0, left.render_cells()[0]), (12, right.render_cells()[0])):
                for offset, cell in enumerate(cells):
                    row[col + offset] = cell
            return [row]

        app._build_terminal_diff_frame([(frame(True), 0, 0)])
        diff = app._build_terminal_diff_frame([(frame(False), 0, 0)])
        stats = app._terminal_frame_diff._buffer.render_stats()

        self.assertEqual(stats.changed_cells, 4)
        self.assertEqual(stats.changed_runs, 4)
        self.assertNotIn("\x1b[H\x1b[2J", diff)
        self.assertIn("\x1b[1;2H", diff)
        self.assertIn("\x1b[1;8H", diff)
        self.assertIn("\x1b[1;14H", diff)
        self.assertIn("\x1b[1;20H", diff)

    def test_terminal_ui_sequences_disable_and_restore_wrap(self):
        """Fullscreen terminal mode should disable auto-wrap and restore it on exit."""
        app = UIApplication()
        writes = []
        app._write_terminal = writes.append

        app._enter_terminal_ui()
        app._leave_terminal_ui()

        self.assertEqual(writes[0], "\x1b[?1049h\x1b[?1000h\x1b[?1002h\x1b[?1006h\x1b[?2004h\x1b[?7l\x1b[?25l\x1b[>4;2m\x1b[H\x1b[2J")
        self.assertEqual(writes[1], "\x1b[0m\x1b[>4;0m\x1b[?25h\x1b[?7h\x1b[?2004l\x1b[?1006l\x1b[?1002l\x1b[?1000l\x1b[?1049l")

    def test_decode_escape_sequence_decodes_sgr_mouse_clicks(self):
        """SGR mouse press sequences should become mouse events."""
        app = UIApplication()

        self.assertEqual(app._decode_escape_sequence(b"[<0;12;8M"), {
            "type": "mouse",
            "event": "press",
            "button": 0,
            "row": 7,
            "col": 11,
        })

    def test_decode_escape_sequence_decodes_bracketed_paste(self):
        """Bracketed paste should become one paste event."""
        app = UIApplication()

        self.assertEqual(app._decode_escape_sequence(b"[200~hello\x1b[201~"), {
            "type": "paste",
            "text": "hello",
        })

    def test_decode_escape_sequence_decodes_sgr_mouse_drags(self):
        """SGR mouse drag sequences should become drag events."""
        app = UIApplication()

        self.assertEqual(app._decode_escape_sequence(b"[<32;12;8M"), {
            "type": "mouse",
            "event": "drag",
            "button": 0,
            "row": 7,
            "col": 11,
        })

    def test_decode_escape_sequence_decodes_mouse_wheel(self):
        """Mouse wheel sequences should be decoded as mouse_wheel events."""
        app = UIApplication()

        up = app._decode_escape_sequence(b"[<64;12;8M")
        self.assertIsNotNone(up)
        self.assertEqual(up["type"], "mouse_wheel")
        self.assertEqual(up["direction"], "up")
        self.assertEqual(up["col"], 11)
        self.assertEqual(up["row"], 7)

        down = app._decode_escape_sequence(b"[<65;12;8M")
        self.assertIsNotNone(down)
        self.assertEqual(down["type"], "mouse_wheel")
        self.assertEqual(down["direction"], "down")

        self.assertIsNone(app._decode_escape_sequence(b"[Mabc"))

    def test_read_key_coalesces_ready_mouse_wheel_burst(self):
        """Ready wheel reports should become one bounded logical event."""
        app = UIApplication()
        read_fd, write_fd = os.pipe()
        try:
            burst_count = MOUSE_WHEEL_COALESCED_MAX_DELTA + 5
            os.write(write_fd, b"\x1b[<65;12;8M" * burst_count)

            event = app._read_key(read_fd, timeout=0)

            self.assertEqual(event["type"], "mouse_wheel")
            self.assertEqual(event["direction"], "down")
            self.assertEqual(event["wheel_delta"], -MOUSE_WHEEL_COALESCED_MAX_DELTA)
            self.assertFalse(app._read_ready(read_fd, 0))
        finally:
            os.close(read_fd)
            os.close(write_fd)

    def test_mouse_wheel_coalescing_preserves_following_key(self):
        """Coalescing must leave non-wheel input available for the next read."""
        app = UIApplication()
        read_fd, write_fd = os.pipe()
        try:
            os.write(write_fd, b"\x1b[<65;12;8Ma")

            event = app._read_key(read_fd, timeout=0)
            next_key = app._read_key(read_fd, timeout=0)

            self.assertEqual(event["type"], "mouse_wheel")
            self.assertEqual(event["wheel_delta"], -1)
            self.assertEqual(next_key, "a")
        finally:
            os.close(read_fd)
            os.close(write_fd)

    def test_decode_escape_sequence_decodes_alt_arrows(self):
        """Option/Alt arrow CSI sequences should become Alt+arrow keys."""
        app = UIApplication()

        self.assertEqual(app._decode_escape_sequence(b"[1;3A"), "Alt+Up")
        self.assertEqual(app._decode_escape_sequence(b"[1;3B"), "Alt+Down")
        self.assertEqual(app._decode_escape_sequence(b"[1;3C"), "Alt+Right")
        self.assertEqual(app._decode_escape_sequence(b"[1;3D"), "Alt+Left")

    def test_decode_escape_sequence_decodes_ctrl_arrows(self):
        app = UIApplication()

        self.assertEqual(app._decode_escape_sequence(b"[1;5A"), "Ctrl+Up")
        self.assertEqual(app._decode_escape_sequence(b"[1;5B"), "Ctrl+Down")
        self.assertEqual(app._decode_escape_sequence(b"[1;5C"), "Ctrl+Right")
        self.assertEqual(app._decode_escape_sequence(b"[1;5D"), "Ctrl+Left")


class TestUIApplicationWindowStack(unittest.TestCase):
    """Test window stack behavior."""

    def test_stack_order(self):
        """Test that windows are stacked in correct order."""
        app = UIApplication()
        w1 = UIWindow(title="Window 1")
        w2 = UIWindow(title="Window 2")
        w3 = UIWindow(title="Window 3")

        app.open(w1)
        self.assertEqual(app.active_window, w1)

        app.open(w2)
        self.assertEqual(app.active_window, w2)

        app.open(w3)
        self.assertEqual(app.active_window, w3)

        app.close_current()
        self.assertEqual(app.active_window, w2)

        app.close_current()
        self.assertEqual(app.active_window, w1)

    def test_focus_returned_on_close(self):
        """Test that focus returns to previous window on close."""
        app = UIApplication()
        w1 = UIWindow(title="Window 1")
        w2 = UIWindow(title="Window 2")

        app.open(w1)
        app.open(w2)

        # w2 should have focus
        self.assertIsNotNone(app.active_window)

        app.close_current()

        # w1 should be active again
        self.assertEqual(app.active_window, w1)

    def test_dialog_stack_render_uses_viewport_offsets(self):
        """Embedded modal windows should render inside the configured viewport."""
        app = UIApplication()
        app.set_viewport(5, 7, 40, 12)
        writes = []
        app._write_terminal = writes.append

        background = FixedRenderWindow(["BASE"], fullscreen=True)
        dialog = FixedRenderWindow(["DIALOG"], natural_width=10, natural_height=3)

        app.open(background)
        app.open(dialog)
        app._render_to_terminal()

        frame = writes[-1]
        self.assertIn("\x1b[6;8H", frame)
        self.assertIn("\x1b[10;8H", frame)
        self.assertIn("DIALOG", frame)

    def test_render_rect_covers_fullscreen_background_under_dialog(self):
        """Protected rects should cover the visible fullscreen child while a dialog is open."""
        app = UIApplication()
        app.set_viewport(5, 7, 40, 12)

        background = FixedRenderWindow(["BASE"], fullscreen=True)
        dialog = FixedRenderWindow(["DIALOG"], natural_width=10, natural_height=3)

        app.open(background)
        app.open(dialog)

        self.assertEqual(app.get_render_rect(), {
            "row": 5,
            "col": 7,
            "width": 40,
            "height": 12,
        })

    def test_viewport_frame_clips_rendered_lines_to_viewport(self):
        """Embedded frames must not write outside their assigned viewport."""
        app = UIApplication()
        app.set_viewport(2, 5, 4, 2)

        frame = app._build_viewport_frame(
            ["abcdef", "ghij", "klmn"],
            col_offset=5,
            row_offset=2,
        )

        self.assertIn("\x1b[3;6Habcd", frame)
        self.assertIn("\x1b[4;6Hghij", frame)
        self.assertNotIn("\x1b[3;6H    ", frame)
        self.assertNotIn("\x1b[4;6H    ", frame)
        self.assertNotIn("abcdef", frame)
        self.assertNotIn("klmn", frame)

    def test_viewport_frame_only_repaints_changed_rows(self):
        """Embedded frames should not blank and repaint unchanged viewport rows."""
        app = UIApplication()
        app.set_viewport(2, 5, 4, 2)

        first = app._build_viewport_frame(["abcd", "efgh"], col_offset=5, row_offset=2)
        second = app._build_viewport_frame(["abcd", "efgh"], col_offset=5, row_offset=2)
        third = app._build_viewport_frame(["ab", "efgh"], col_offset=5, row_offset=2)

        self.assertIn("\x1b[3;6Habcd", first)
        self.assertEqual(second, "")
        self.assertIn("\x1b[3;6Hab\x1b[0m  ", third)
        self.assertNotIn("\x1b[4;6H", third)

    def test_viewport_frame_does_not_paint_viewport_padding(self):
        """Centered embedded windows should not repaint outside their render rectangle."""
        app = UIApplication()
        app.set_viewport(2, 5, 10, 2)

        frame = app._build_viewport_frame(["abcd", "efgh"], col_offset=8, row_offset=2)

        self.assertIn("\x1b[3;9Habcd", frame)
        self.assertIn("\x1b[4;9Hefgh", frame)
        self.assertNotIn("\x1b[3;6H", frame)
        self.assertNotIn("\x1b[4;6H", frame)

    def test_viewport_frame_clips_when_window_starts_before_viewport(self):
        """Horizontal clipping should preserve only the visible fragment."""
        app = UIApplication()
        app.set_viewport(2, 5, 4, 1)

        frame = app._build_viewport_frame(["abcdef"], col_offset=3, row_offset=2)

        self.assertIn("\x1b[3;6Hcdef", frame)
        self.assertNotIn("\x1b[3;4H", frame)
        self.assertNotIn("abcdef", frame)

    def test_popup_overlay_uses_viewport_offset_and_clipping(self):
        """Popup overlays in embedded apps should stay inside the app viewport."""
        app = UIApplication()
        app.set_viewport(5, 7, 4, 1)
        app._active_popup = FixedPopup(["abcdef"])

        frame = app._build_popup_overlay()

        self.assertEqual(frame, "\x1b[6;8Habcd")

    def test_mode_change_reflows_layout_without_size_change(self):
        """Returning from a dialog must restore fullscreen element geometry."""
        app = UIApplication()
        app.set_viewport(1, 0, 85, 16)
        window = ActivityFeedUI()

        app.open(window)
        title = window.get_element("header_title")
        fullscreen_width = title.width

        window.mode = "dialog"
        dialog_width = title.width
        window.mode = "fullscreen"

        self.assertLess(dialog_width, fullscreen_width)
        self.assertEqual(title.width, fullscreen_width)

    def test_mouse_wheel_coordinates_are_translated_to_window(self):
        """Mouse wheel uses terminal coordinates but dispatches inside the active window."""
        app = UIApplication(width=10, height=4)
        app._get_terminal_size = lambda: (30, 12)
        window = UIWindow(title="Test")
        listbox = window.create_element("role", "listbox", options=["A", "B", "C", "D"], selected_items=["A"])
        listbox.width = 10
        listbox.height = 2
        app.open(window)
        window.set_focus(None)

        col_offset, row_offset = app._active_window_offsets()
        app.handle_key({
            "type": "mouse_wheel",
            "direction": "down",
            "row": row_offset + 1,
            "col": col_offset + 1,
        })

        self.assertEqual(listbox._scroll_offset, 1)
        self.assertIsNone(window._focused_element)


class TestUIApplicationSizing(unittest.TestCase):
    """Test size propagation and expanded layout behavior."""

    def _make_expanded_layout(self):
        """Layout with both width and height expanded — triggers fullscreen mode."""
        return [{"name": "cell", "cell_row": 1, "cell_col": 1,
                 "cell_chars_width": 10, "cell_chars_height": 3,
                 "chars_width": 10, "chars_height": 3, "type": "label"}]

    def test_expanded_layout_uses_terminal_size(self):
        """Window with expanded cells should be sized to the live terminal."""
        app = UIApplication()
        app._get_terminal_size = lambda: (120, 40)
        window = UIWindow(title="Expanded", layout=self._make_expanded_layout())
        window.create_element("cell", "label", text="x")

        app.open(window)

        self.assertEqual(window.mode, "fullscreen")
        self.assertEqual(window._window_width, 120)
        self.assertEqual(window._window_height, 40)

    def test_expanded_window_tracks_terminal_resizes(self):
        """Windows with expanded cells should resize on every render after terminal changes."""
        app = UIApplication()
        size = {"value": (90, 20)}
        app._get_terminal_size = lambda: size["value"]
        window = UIWindow(title="Expanded", layout=self._make_expanded_layout())
        window.create_element("cell", "label", text="x")

        app.open(window)
        self.assertEqual(window._window_width, 90)
        self.assertEqual(window._window_height, 20)

        size["value"] = (132, 33)
        app.render()

        self.assertEqual(window._window_width, 132)
        self.assertEqual(window._window_height, 33)

    def test_open_propagates_window_size(self):
        """Opening a window should resize it to the app dimensions."""
        app = UIApplication(width=100, height=30)
        w = UIWindow(title="Sized")

        app.open(w)

        self.assertEqual(w._window_width, 100)
        self.assertEqual(w._window_height, 30)

    def test_fit_content_layout_does_not_trigger_expanded_window_mode(self):
        """Fit-content cells should size naturally, not request terminal expansion."""
        app = UIApplication()
        window = UIWindow(
            title="Fit",
            layout=[
                {
                    "name": "title",
                    "row": 0,
                    "col": 1,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 12,
                    "cell_chars_height": 1,
                    "cell_width_mode": "fit-content",
                    "cell_height": 1,
                    "cell_height_mode": "auto",
                    "chars_width": 9,
                    "chars_height": 1,
                    "margin_right": 1,
                    "width_mode": "fit-content",
                    "height": 1,
                    "type": "label",
                },
            ],
        )
        window.create_element("title", "label", text="hello")

        app.open(window)

        self.assertEqual(window.mode, "normal")
        self.assertEqual(window.get_element("title").width, 5)
        self.assertEqual(window.get_element("title")._resolved_cell.width, 7)

    def test_auto_cell_padding_combines_with_ascii_element_margins(self):
        """Padding should inset cell content while preserving element margins inside it."""
        app = UIApplication(width=20, height=6)
        window = UIWindow(
            title="Padding",
            style={
                "this": {"border-width": "0"},
                "section": {"padding": "0, 1, 0, 1"},
            },
            layout=[
                {
                    "name": "padded",
                    "row": 0,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 10,
                    "cell_chars_height": 1,
                    "cell_name": "section",
                    "cell_width": 10,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 10,
                    "chars_height": 1,
                    "width": None,
                    "height": 1,
                    "type": "label",
                },
                {
                    "name": "edge",
                    "row": 0,
                    "col": 5,
                    "cell_row": 2,
                    "cell_col": 0,
                    "cell_chars_width": 10,
                    "cell_chars_height": 1,
                    "cell_width": 10,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 5,
                    "chars_height": 1,
                    "width": 5,
                    "height": 1,
                    "type": "button",
                },
                {
                    "name": "fixed",
                    "row": 0,
                    "col": 1,
                    "cell_row": 4,
                    "cell_col": 0,
                    "cell_chars_width": 10,
                    "cell_chars_height": 1,
                    "cell_name": "section",
                    "cell_width": 10,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 8,
                    "chars_height": 1,
                    "margin_right": 1,
                    "width": 8,
                    "height": 1,
                    "type": "label",
                },
            ],
        )
        window.create_element("padded", "label", text="content")
        window.create_element("edge", "button", title="Reset")
        window.create_element("fixed", "label", text="fixed width")

        self.assertEqual(window.get_natural_size(), (12, 3))

        app.open(window)

        self.assertEqual(window.get_cell_rect("section")["width"], 12)
        self.assertEqual(window.get_element("padded").col, 1)
        self.assertEqual(window.get_element("padded").width, 10)
        self.assertEqual(window.get_element("edge")._resolved_cell.width, 12)
        edge = window.get_element("edge")
        self.assertEqual(edge._cell_col + edge.col + edge.width, 10)
        fixed = window.get_element("fixed")
        self.assertEqual(fixed.col, 2)
        self.assertEqual(fixed.width, 8)
        self.assertEqual(fixed._cell_col + fixed.col + fixed.width, 10)

    def test_fixed_cell_padding_expands_cell_dimensions(self):
        """Fixed structural dimensions should include cell padding in the final cell size."""
        app = UIApplication(width=20, height=8)
        window = UIWindow(
            title="Fixed Padding",
            style={
                "this": {"border-width": "0"},
                "panel": {"padding": "1, 2, 3, 4"},
            },
            layout=[
                {
                    "name": "field",
                    "row": 0,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 5,
                    "cell_chars_height": 2,
                    "cell_name": "panel",
                    "cell_width": 5,
                    "cell_height": 2,
                    "cell_width_mode": "fixed",
                    "cell_height_mode": "fixed",
                    "chars_width": 5,
                    "chars_height": 2,
                    "width": None,
                    "height": None,
                    "type": "label",
                },
            ],
        )
        window.create_element("field", "label", text="field")

        self.assertEqual(window.get_natural_size(), (11, 6))

        app.open(window)
        field = window.get_element("field")
        self.assertEqual(field._resolved_cell.width, 11)
        self.assertEqual(field._resolved_cell.height, 6)
        self.assertEqual(field.col, 4)
        self.assertEqual(field.row, 1)
        self.assertEqual(field.width, 5)
        self.assertEqual(field.height, 2)

    def test_expanded_cell_padding_does_not_expand_natural_size(self):
        """Expanded dimensions should keep their natural placeholder size before app expansion."""
        app = UIApplication(width=20, height=5)
        window = UIWindow(
            title="Expanded Padding",
            style={
                "this": {"border-width": "0"},
                "panel": {"padding": "0, 2, 0, 2"},
            },
            layout=[
                {
                    "name": "body",
                    "row": 0,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 8,
                    "cell_chars_height": 1,
                    "cell_name": "panel",
                    "cell_width": None,
                    "cell_height": 1,
                    "cell_width_mode": "expanded",
                    "cell_height_mode": "auto",
                    "chars_width": 8,
                    "chars_height": 1,
                    "width": None,
                    "height": 1,
                    "type": "label",
                },
            ],
        )
        window.create_element("body", "label", text="body")

        self.assertEqual(window.get_natural_size(), (8, 1))

        app.open(window)
        body = window.get_element("body")
        self.assertEqual(body._resolved_cell.width, 20)
        self.assertEqual(body.col, 2)
        self.assertEqual(body.width, 16)

    def test_fit_content_cell_padding_is_not_double_counted(self):
        """Fit-content dimensions already include padding during content measurement."""
        app = UIApplication(width=20, height=5)
        window = UIWindow(
            title="Fit Padding",
            style={
                "this": {"border-width": "0"},
                "panel": {"padding": "0, 1, 0, 1"},
            },
            layout=[
                {
                    "name": "title",
                    "row": 0,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 12,
                    "cell_chars_height": 1,
                    "cell_name": "panel",
                    "cell_width_mode": "fit-content",
                    "cell_height": 1,
                    "cell_height_mode": "auto",
                    "chars_width": 12,
                    "chars_height": 1,
                    "width_mode": "fit-content",
                    "height": 1,
                    "type": "label",
                },
            ],
        )
        window.create_element("title", "label", text="abcd")

        self.assertEqual(window.get_natural_size(), (6, 1))

        app.open(window)
        title = window.get_element("title")
        self.assertEqual(title._resolved_cell.width, 6)
        self.assertEqual(title.col, 1)
        self.assertEqual(title.width, 4)

    def test_fit_content_cell_uses_element_style_margin(self):
        """Element style margins should contribute to fit-content cell dimensions."""
        app = UIApplication(width=20, height=8)
        window = UIWindow(
            title="Fit Margin",
            style={
                "this": {"border-width": "0"},
                "@message": {"margin": "1, 1, 2, 1"},
            },
            layout=[
                {
                    "name": "message",
                    "row": 0,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 12,
                    "cell_chars_height": 1,
                    "cell_width_mode": "fit-content",
                    "cell_height_mode": "fit-content",
                    "chars_width": 12,
                    "chars_height": 1,
                    "width_mode": "fit-content",
                    "height_mode": "fit-content",
                    "type": "label",
                },
            ],
        )
        window.create_element("message", "label", text="hi")

        self.assertEqual(window.get_natural_size(), (4, 4))

        app.open(window)
        message = window.get_element("message")
        self.assertEqual(message._resolved_cell.width, 4)
        self.assertEqual(message._resolved_cell.height, 4)
        self.assertEqual(message.row, 1)
        self.assertEqual(message.col, 1)
        self.assertEqual(message.width, 2)
        self.assertEqual(message.height, 1)

    def test_cell_padding_clips_elements_to_cell_bounds(self):
        """Elements pushed into padding must not render outside the content box."""
        app = UIApplication(width=12, height=8)
        window = UIWindow(
            title="Padding Clip",
            style={
                "this": {"border-width": "0"},
                "section": {"padding": "1, 0, 1, 0"},
            },
            layout=[
                {
                    "name": "overflow",
                    "row": 4,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 12,
                    "cell_chars_height": 4,
                    "cell_name": "section",
                    "cell_width": 12,
                    "cell_height": 4,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 8,
                    "chars_height": 1,
                    "width": 8,
                    "height": 1,
                    "type": "label",
                },
                {
                    "name": "bottom",
                    "row": 0,
                    "col": 0,
                    "cell_row": 5,
                    "cell_col": 0,
                    "cell_chars_width": 12,
                    "cell_chars_height": 1,
                    "cell_width": 12,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 6,
                    "chars_height": 1,
                    "width": 6,
                    "height": 1,
                    "type": "label",
                },
            ],
        )
        window.create_element("overflow", "label", text="OVERFLOW")
        window.create_element("bottom", "label", text="BOTTOM")

        app.open(window)
        plain = [_strip_ansi(line) for line in window.render()]

        self.assertNotIn("OVERFLOW", "\n".join(plain))
        self.assertIn("BOTTOM", "\n".join(plain))

    def test_open_combobox_menu_is_not_clipped_to_cell_height(self):
        """Combobox dropdown is an overlay and may extend below its layout cell."""
        app = UIApplication(width=12, height=5)
        window = UIWindow(
            title="Combo Clip",
            style={"this": {"border-width": "0"}},
            layout=[
                {
                    "name": "choice",
                    "row": 0,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 12,
                    "cell_chars_height": 1,
                    "cell_width": 12,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 10,
                    "chars_height": 1,
                    "width": 10,
                    "height": 1,
                    "type": "combobox",
                },
            ],
        )
        combo = window.create_element("choice", "combobox", options=["One", "Two"], selected_item="One")
        app.open(window)
        window.set_focus(combo)
        window._enter_edit_mode()

        plain = "\n".join(_strip_ansi(line) for line in window.render())

        self.assertIn("One", plain)
        self.assertIn("Two", plain)

    def test_zero_border_horizontal_group_fills_parent_width(self):
        """Collapsed vertical separators should not leave a trailing empty column."""
        app = UIApplication(width=10, height=2)
        window = UIWindow(
            title="Zero Border Width",
            style={"this": {"border-width": "0"}},
            layout=[
                {
                    "name": "top",
                    "row": 0,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 10,
                    "cell_chars_height": 1,
                    "cell_width": 10,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 10,
                    "chars_height": 1,
                    "width": None,
                    "height": 1,
                    "type": "label",
                },
                {
                    "name": "left",
                    "row": 0,
                    "col": 0,
                    "cell_row": 2,
                    "cell_col": 0,
                    "cell_chars_width": 5,
                    "cell_chars_height": 1,
                    "cell_width": 5,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 5,
                    "chars_height": 1,
                    "width": None,
                    "height": 1,
                    "type": "label",
                },
                {
                    "name": "right",
                    "row": 0,
                    "col": 0,
                    "cell_row": 2,
                    "cell_col": 6,
                    "cell_chars_width": 4,
                    "cell_chars_height": 1,
                    "cell_width": 4,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 4,
                    "chars_height": 1,
                    "width": None,
                    "height": 1,
                    "type": "label",
                },
            ],
        )
        window.create_element("top", "label", text="top")
        window.create_element("left", "label", text="left")
        window.create_element("right", "label", text="right")

        app.open(window)

        left = window.get_element("left")
        right = window.get_element("right")
        self.assertEqual(left._resolved_cell.width, 5)
        self.assertEqual(right._resolved_cell.col, 5)
        self.assertEqual(right._resolved_cell.width, 5)
        self.assertEqual(right._resolved_cell.col + right._resolved_cell.width, 10)

    def test_fit_content_cell_height_ignores_trailing_ascii_margin(self):
        """Fit-content cells should size from content, not visual sketch padding."""
        app = UIApplication(width=40, height=10)
        window = UIWindow(
            title="Fit Height",
            style={"this": {"border-width": "0"}},
            layout=[
                {
                    "name": "user",
                    "row": 0,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 20,
                    "cell_chars_height": 5,
                    "cell_name": "text_cell",
                    "cell_width": 20,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "fit-content",
                    "chars_width": 4,
                    "chars_height": 1,
                    "margin_bottom": 4,
                    "height": 1,
                    "type": "label",
                },
                {
                    "name": "text",
                    "row": 1,
                    "col": 0,
                    "cell_row": 0,
                    "cell_col": 0,
                    "cell_chars_width": 20,
                    "cell_chars_height": 5,
                    "cell_name": "text_cell",
                    "cell_width": 20,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "fit-content",
                    "chars_width": 10,
                    "chars_height": 4,
                    "height_mode": "fit-content",
                    "type": "label",
                },
                {
                    "name": "price",
                    "row": 0,
                    "col": 0,
                    "cell_row": 6,
                    "cell_col": 0,
                    "cell_chars_width": 20,
                    "cell_chars_height": 1,
                    "cell_name": "price_cell",
                    "cell_width": 20,
                    "cell_height": 1,
                    "cell_width_mode": "auto",
                    "cell_height_mode": "auto",
                    "chars_width": 10,
                    "chars_height": 1,
                    "height": 1,
                    "type": "label",
                },
            ],
        )
        window.create_element("user", "label", text="user")
        window.create_element("text", "label", text="one\ntwo")
        window.create_element("price", "label", text="price")

        app.open(window)

        self.assertEqual(window.get_cell_rect("text_cell")["height"], 3)
        self.assertEqual(window.get_cell_rect("price_cell")["row"], 3)
        self.assertEqual(window.get_element("text").height, 2)

    def test_expanded_layout_uses_available_space(self):
        """Windows with expanded cells fill the available app size."""
        app = UIApplication(width=40, height=12)
        w = UIWindow(
            title="Expanded",
            layout=[
                {
                    "name": "header",
                    "row": 0,
                    "col": 0,
                    "relative_row": 0,
                    "relative_col": 0,
                    "cell_row": 1,
                    "cell_col": 1,
                    "cell_chars_width": 10,
                    "cell_chars_height": 1,
                    "cell_width": None,
                    "cell_height": 1,
                    "chars_width": 10,
                    "chars_height": 1,
                    "margin_right": 0,
                    "margin_bottom": 0,
                    "width": None,
                    "height": 1,
                    "type": "label",
                },
                {
                    "name": "body",
                    "row": 0,
                    "col": 0,
                    "relative_row": 0,
                    "relative_col": 0,
                    "cell_row": 3,
                    "cell_col": 1,
                    "cell_chars_width": 10,
                    "cell_chars_height": 3,
                    "cell_width": 10,
                    "cell_height": None,
                    "chars_width": 10,
                    "chars_height": 3,
                    "margin_right": 0,
                    "margin_bottom": 0,
                    "width": 10,
                    "height": None,
                    "type": "label",
                },
            ],
        )
        w.create_element("header", "label", text="header")
        w.create_element("body", "label", text="body")

        app.open(w)

        self.assertEqual(w.get_element("header").col, 0)
        self.assertEqual(w.get_element("header")._cell_col, 0)
        self.assertEqual(w.get_element("header")._cell_row, 0)
        self.assertEqual(w.get_element("header").width, 40)
        self.assertEqual(w.get_element("body").height, 11)

    def test_zero_border_collapses_internal_ascii_separator_height(self):
        """Auto-height spanning cells should not keep ASCII separator rows at border-width 0."""
        layout = [
            {
                "name": "left",
                "row": 0,
                "col": 0,
                "cell_row": 0,
                "cell_col": 0,
                "cell_chars_width": 2,
                "cell_chars_height": 4,
                "cell_width": 2,
                "cell_height": 4,
                "cell_width_mode": "fixed",
                "cell_height_mode": "auto",
                "chars_width": 2,
                "chars_height": 1,
                "width": 2,
                "height": 1,
                "type": "label",
            },
            {
                "name": "top",
                "row": 0,
                "col": 0,
                "cell_row": 0,
                "cell_col": 3,
                "cell_chars_width": 3,
                "cell_chars_height": 2,
                "cell_width": 3,
                "cell_height": 2,
                "cell_width_mode": "auto",
                "cell_height_mode": "auto",
                "chars_width": 3,
                "chars_height": 1,
                "width": 3,
                "height": 1,
                "type": "label",
            },
            {
                "name": "bottom",
                "row": 0,
                "col": 0,
                "cell_row": 3,
                "cell_col": 3,
                "cell_chars_width": 3,
                "cell_chars_height": 1,
                "cell_width": 3,
                "cell_height": 1,
                "cell_width_mode": "auto",
                "cell_height_mode": "auto",
                "chars_width": 3,
                "chars_height": 1,
                "width": 3,
                "height": 1,
                "type": "label",
            },
        ]
        window = UIWindow(layout=layout, style={"this": {"border-width": "0"}})
        window.create_element("left", "label", text="L")
        window.create_element("top", "label", text="T")
        window.create_element("bottom", "label", text="B")

        app = UIApplication(width=5, height=3)
        app.open(window)

        self.assertEqual(window.get_element("left")._resolved_cell.height, 3)
        self.assertEqual(window.get_element("bottom")._resolved_cell.row, 2)

    def test_calculator_layout_element_positions(self):
        """Calculator layout should resolve element positions correctly."""
        window = CalculatorUI()
        width, height = window.get_natural_size()
        app = UIApplication(width=width, height=height)

        app.open(window)

        display = window.get_element("display")
        ac = window.get_element("ac")
        dl = window.get_element("dl")
        pr = window.get_element("pr")
        dv = window.get_element("dv")
        n0 = window.get_element("n0")

        self.assertEqual(window._window_width, 22)
        self.assertEqual(display.col, 2)
        self.assertEqual(display.width, 18)
        self.assertEqual(ac.col, 2)
        self.assertEqual(dl.col, 7)
        self.assertEqual(pr.col, 12)
        self.assertEqual(dv.col, 17)
        self.assertEqual(n0.width, 8)

    def test_cells_window_supports_axis_specific_border_width(self):
        """Cells example should allow overriding only one border-width axis."""
        window = CellsUI()
        width, height = window.get_natural_size()
        app = UIApplication(width=width, height=height)

        app.open(window)

        a1 = window.get_element("a1")
        b1 = window.get_element("b1")
        d1 = window.get_element("d1")
        f1 = window.get_element("f1")

        self.assertEqual(window.style.border_width_horizontal, 1)
        self.assertEqual(window.style.border_width_vertical, 2)
        self.assertEqual(a1._resolved_cell.col, 2)
        self.assertEqual(b1._resolved_cell.col, 21)
        self.assertEqual(d1._resolved_cell.col, 32)
        self.assertEqual(f1._resolved_cell.col, 25)
        self.assertEqual(a1._cell_col + a1.col, 3)
        self.assertEqual(a1._cell_row + a1.row, 1)

    def test_cells_window_exports_resolved_layout(self):
        """Resolved layout export should expose comparable cells and element rects."""
        window = CellsUI()
        width, height = window.get_natural_size()
        app = UIApplication(width=width, height=height)

        app.open(window)
        exported = window.export_resolved_layout()

        cells = {cell["name"]: cell for cell in exported["cells"]}
        elements = {element["name"]: element for element in exported["elements"]}
        self.assertEqual(cells["cell1"]["col"], 2)
        self.assertEqual(cells["cell2"]["col"], 21)
        self.assertEqual(cells["cl3"]["col"], 32)
        self.assertEqual(cells["cl6"]["col"], 25)
        self.assertEqual(elements["a1"]["cell_name"], "cell1")
        self.assertEqual(elements["a1"]["row"], 1)
        self.assertEqual(elements["a1"]["col"], 3)
        self.assertEqual(elements["b1"]["cell_name"], "cell2")
        self.assertEqual(elements["d1"]["cell_name"], "cl3")
        self.assertEqual(elements["f1"]["cell_name"], "cl6")

    def test_named_cell_style_selector_applies_to_cell_background(self):
        """Named-cell selectors should style the cell background and nested selectors its elements."""
        window = CellsUI()
        width, height = window.get_natural_size()
        app = UIApplication(width=width, height=height)

        app.open(window)
        window.render()

        self.assertEqual(window.get_element("a1").style.background.hex, "#7a3f59")
        self.assertEqual(window.get_element("b1").style.background.hex, "#3d7b6d")
        self.assertEqual(window.get_element("c1").style.color.hex, "#ecfdf5")
        self.assertEqual(window.get_element("d1").style.color.hex, "#d7fbe8")
        self.assertEqual(window.get_element("f1").style.background.hex, "#3a8890")
        self.assertEqual(window.get_element("f2").style.background.hex, "#31565f")
        self.assertEqual(window.styles[1][1]["background"].hex, "#3b1f2d")
        self.assertEqual(window.styles[3][1]["background"].hex, "#4a4422")
        self.assertEqual(window.styles[6][1]["background"].hex, "#482456")
        row1 = {style["char_pos"]: style["background"] for style in window.styles[1]}
        row6 = {style["char_pos"]: style["background"] for style in window.styles[6]}
        row13 = {style["char_pos"]: style["background"] for style in window.styles[13]}
        self.assertEqual(row1[21].hex, "#1f463f")
        self.assertEqual(row1[32].hex, "#26345f")
        self.assertEqual(row6[25].hex, "#174f55")
        self.assertEqual(row6[32].hex, "#26345f")
        self.assertEqual(row13[32].hex, "#2f3745")

    def test_transparent_cell_background_keeps_underlying_window_background(self):
        """Transparent named cells should not let the previous ANSI background bleed."""
        window = UIWindow()
        window.resize(6, 1)
        window.style.background = Color("#111827")
        cell_style = Style()
        cell_style.background = Color("transparent")
        window._resolved_cell_backgrounds = [{
            "row": 0,
            "col": 0,
            "width": 6,
            "height": 1,
            "style": cell_style,
        }]

        window._render({0: {0: "\x1b[48;2;37;99;235mAB\x1b[0m"}})
        row_styles = {style["char_pos"]: style["background"] for style in window.styles[0]}

        self.assertEqual(row_styles[0], "037099235")
        self.assertEqual(row_styles[2].hex, "#111827")

    def test_transparent_window_background_uses_parent_background(self):
        """Transparent root backgrounds should render over the reusable parent background."""
        class Parent:
            def __init__(self):
                self.style = Style()
                self.style.background = Color("#112233")

            def get_style(self):
                return self.style

        window = UIWindow()
        window.parent = Parent()
        window.resize(4, 1)
        window.style.background = Color("#ff000000")

        window._render({})

        self.assertEqual(window.styles[0][0]["background"].hex, "#112233")

    def test_translucent_window_background_blends_with_parent_background(self):
        """Translucent root backgrounds should blend before ANSI rendering."""
        class Parent:
            def __init__(self):
                self.style = Style()
                self.style.background = Color("#000000")

            def get_style(self):
                return self.style

        window = UIWindow()
        window.parent = Parent()
        window.resize(4, 1)
        window.style.background = Color("#ff000080")

        window._render({})

        self.assertEqual(window.styles[0][0]["background"].hex, "#800000")

    def test_translucent_embedded_window_background_uses_container_window_background(self):
        """Embedded windows should not fall back to black when only the container has a background."""
        class Wrapper:
            def __init__(self, container):
                self.style = Style()
                self.parent = container

            def get_style(self):
                return self.style

            def _get_cell_background(self):
                return None

        container = UIWindow()
        container.style.background = Color("#102030")
        window = UIWindow()
        window.parent = Wrapper(container)
        window.resize(4, 1)
        window.style.background = Color("#ff000080")

        window._render({})

        self.assertEqual(window.styles[0][0]["background"].hex, "#881018")

    def test_scrollview_scrolls_within_child_taller_than_viewport(self):
        """Line-level scroll should start at the top and move inside a tall child."""
        child = FixedRenderChild(["0   ", "1   ", "2   ", "3   ", "4   "])
        view = PlainScrollView(width=4, height=3, children=[child])

        lines = view.render()
        self.assertEqual([line[0] for line in lines], ["0", "1", "2"])

        view._view_offset = 1
        lines = view.render()
        self.assertEqual([line[0] for line in lines], ["1", "2", "3"])

        view._view_offset = 0
        lines = view.render()
        self.assertEqual([line[0] for line in lines], ["2", "3", "4"])

    def test_scrollview_cached_average_height_is_not_divided_twice(self):
        """Cached child heights should produce the same viewport estimate as measured heights."""
        children = [FixedRenderChild(["x"] * 4) for _ in range(10)]
        view = PlainScrollView(width=4, height=8, children=children)
        view._cached_child_heights = [4] * len(children)

        self.assertEqual(view._max_children_in_viewport(view._get_viewport_rect()), 2)

    def test_scrollview_height_cache_measures_children_before_scroll_offset(self):
        """A first render at a non-zero child offset should not leave earlier heights as zero."""
        children = [
            FixedRenderChild(["0"]),
            FixedRenderChild(["1", "1"]),
            FixedRenderChild(["2", "2", "2"]),
            FixedRenderChild(["3", "3", "3", "3"]),
            FixedRenderChild(["4", "4", "4", "4", "4"]),
        ]
        view = PlainScrollView(width=4, height=3, children=children)
        view._cached_child_heights = None
        view._scroll_offset = 3

        view.render()

        self.assertEqual(view._cached_child_heights, [1, 2, 3, 4, 5])

    def test_scrollview_clear_selections_does_not_render_children(self):
        """Key cleanup should clear label selections without rendering child rows."""
        child = ClearOnlySelectionChild()
        view = PlainScrollView(width=4, height=3, children=[child])

        view._clear_selections()

        self.assertTrue(child.cleared)
        self.assertEqual(child.render_calls, 0)

    def test_scrollview_resizes_generated_child_to_viewport_width(self):
        """Default child rendering should frame generated controls like the C++ runtime."""
        child = FixedRenderWindow(["abcdef", "ghijkl"], natural_width=6, natural_height=2)
        view = PlainScrollView(width=4, height=3, children=[child])

        lines = view.render()

        self.assertEqual(child._window_width, 4)
        self.assertEqual(child._window_height, 2)
        self.assertEqual(lines[:2], ["abcd", "ghij"])

    def _window_with_active_scrollview_child(self, elem):
        window = UIWindow()
        window._focused_element = elem
        window._edit_mode = True
        window._active_scrollview_scope = {
            "proxy": Label(text="items"),
            "scrollview": PlainScrollView(width=20, height=3),
        }
        window._begin_scoped_child_edit(elem)
        return window

    def test_scrollview_scoped_combobox_escape_closes_child_menu_only(self):
        combo = ComboBox(name="category", options=["Food", "Transport"], selected_item="Food")
        window = self._window_with_active_scrollview_child(combo)
        combo.handle_key("Down")

        self.assertTrue(window._handle_scrollview_scope_key("Escape"))

        self.assertIsNotNone(window._active_scrollview_scope)
        self.assertTrue(window._edit_mode)
        self.assertFalse(combo._edit_mode)
        self.assertFalse(combo._menu_open)
        self.assertEqual(combo.selected_item, "Food")

    def test_scrollview_scoped_escape_restores_standard_child_controls(self):
        cases = [
            (TextInput(name="title", value="old"), lambda elem: setattr(elem, "value", "new"), lambda elem: elem.value, "old"),
            (NumberInput(name="amount", value=5), lambda elem: elem.handle_key("1"), lambda elem: elem.value, 5.0),
            (ListBox(name="status", options=["A", "B"], selected_items=["A"]),
             lambda elem: elem.handle_key("Down"), lambda elem: list(elem.selected_items), ["A"]),
        ]
        for elem, mutate, value_of, expected in cases:
            with self.subTest(elem=elem.ELEMENT_TYPE):
                window = self._window_with_active_scrollview_child(elem)
                mutate(elem)

                self.assertTrue(window._handle_scrollview_scope_key("Escape"))

                self.assertIsNotNone(window._active_scrollview_scope)
                self.assertFalse(elem._edit_mode)
                self.assertEqual(value_of(elem), expected)

    def test_scrollview_scoped_textarea_escape_commits_leave_mode(self):
        area = TextArea(name="message", value="old", width=10, height=3)
        window = self._window_with_active_scrollview_child(area)
        area.value = "new"

        self.assertTrue(window._handle_scrollview_scope_key("Escape"))

        self.assertIsNotNone(window._active_scrollview_scope)
        self.assertFalse(area._edit_mode)
        self.assertEqual(area.value, "new")

    def test_label_text_marks_app_dirty(self):
        """Programmatic label text changes should render after dirty rendering is enabled."""
        app = UIApplication()
        label = Label(text="old")
        label._app = app
        app.clear_dirty()

        label.text = "new"

        self.assertTrue(app.is_dirty)

    def test_notify_marks_app_dirty(self):
        """Notifications should be visible even when the app was otherwise clean."""
        app = UIApplication()
        app.clear_dirty()

        app.notify("Copied")

        self.assertTrue(app.is_dirty)

    def test_expired_notification_requests_clean_render(self):
        """Expired notifications should disappear without waiting for user input."""
        app = UIApplication()
        read_fd, write_fd = os.pipe()
        try:
            app.notify("Copied", duration=0)
            app.clear_dirty()

            self.assertTrue(app._should_render_now(read_fd))
        finally:
            os.close(read_fd)
            os.close(write_fd)

    def test_textinput_typing_marks_app_dirty(self):
        """Typing into an active text input should request a re-render."""
        app = UIApplication()
        window = UIWindow()
        inp = window.create_element("prompt", "textinput", value="")
        app.open(window)
        window.set_focus(inp)
        window._enter_edit_mode()
        app.clear_dirty()

        handled = window.handle_key("h")

        self.assertTrue(handled)
        self.assertEqual(inp.value, "h")
        self.assertTrue(app.is_dirty)

    def test_escape_in_file_browser_entries_does_not_close_dialog(self):
        """Escape while editing file entries should enter navigation mode, not close Save As."""
        with tempfile.TemporaryDirectory() as root:
            with open(os.path.join(root, "note.txt"), "w", encoding="utf-8") as handle:
                handle.write("note")

            app = UIApplication()
            app.open(UIWindow(title="Main"))
            browser = FileBrowser(root, root, mode="save", initial_filename="note.txt")
            app.open(browser)

            self.assertEqual(app.window_count, 2)
            self.assertIs(app.active_window, browser)
            self.assertIs(browser._focused_element, browser.entries)
            self.assertTrue(browser._edit_mode)

            app.handle_key("Escape")

            self.assertEqual(app.window_count, 2)
            self.assertIs(app.active_window, browser)
            self.assertIs(browser._focused_element, browser.entries)
            self.assertFalse(browser._edit_mode)

    def test_escape_in_file_browser_entries_on_selected_file_does_not_close_dialog(self):
        """Escape on a selected file entry should enter navigation mode without activation."""
        with tempfile.TemporaryDirectory() as root:
            path = os.path.join(root, ".htaccess")
            with open(path, "w", encoding="utf-8") as handle:
                handle.write("deny")

            app = UIApplication()
            app.open(UIWindow(title="Main"))
            browser = FileBrowser(root, root, mode="save", initial_filename=".htaccess")
            app.open(browser)
            browser.entries.selected_items = [".htaccess"]

            app.handle_key("Escape")

            self.assertEqual(app.window_count, 2)
            self.assertIs(app.active_window, browser)
            self.assertIs(browser._focused_element, browser.entries)
            self.assertFalse(browser._edit_mode)

    def test_escape_in_file_browser_filename_edit_does_not_close_dialog(self):
        """Escape while editing filename should enter navigation mode, not close Save As."""
        with tempfile.TemporaryDirectory() as root:
            app = UIApplication()
            app.open(UIWindow(title="Main"))
            browser = FileBrowser(root, root, mode="save", initial_filename="note.txt")
            app.open(browser)
            browser.set_focus(browser.filename)
            browser._enter_edit_mode()

            app.handle_key("Escape")

            self.assertEqual(app.window_count, 2)
            self.assertIs(app.active_window, browser)
            self.assertIs(browser._focused_element, browser.filename)
            self.assertFalse(browser._edit_mode)

    def test_file_browser_extension_filter_disables_and_blocks_unmatched_files(self):
        """Open-mode FileBrowser should dim and reject files outside the extension filter."""
        with tempfile.TemporaryDirectory() as root:
            txt_path = os.path.join(root, "note.txt")
            png_path = os.path.join(root, "photo.png")
            with open(txt_path, "w", encoding="utf-8") as handle:
                handle.write("note")
            with open(png_path, "w", encoding="utf-8") as handle:
                handle.write("png")

            picked = []
            browser = FileBrowser(root, root, mode="open", on_close=picked.append, extension_filter="png|jpg")
            browser.open()

            self.assertIn("note.txt", browser.entries.disabled_items)
            browser.entries.selected_items = ["note.txt"]
            browser._preview_selected()
            self.assertFalse(browser.open_btn.enabled)
            browser._accept_current()
            self.assertEqual(picked, [])

            browser.entries.selected_items = ["photo.png"]
            browser._preview_selected()
            self.assertTrue(browser.open_btn.enabled)
            browser._accept_current()
            self.assertEqual(picked, [png_path])

    def test_dirty_render_defers_while_input_is_queued_until_frame_interval(self):
        """Queued input should be coalesced instead of rendering every event."""
        app = UIApplication()
        read_fd, write_fd = os.pipe()
        try:
            app.mark_dirty()
            app._last_render_at = time.monotonic()
            os.write(write_fd, b"x")

            self.assertFalse(app._should_render_now(read_fd))

            app._last_render_at = time.monotonic() - 1
            self.assertTrue(app._should_render_now(read_fd))
        finally:
            os.close(read_fd)
            os.close(write_fd)

    def test_scrollview_wheel_at_boundary_does_not_mark_dirty(self):
        """Wheel input that cannot change the offset should not force a frame."""
        app = UIApplication()
        view = PlainScrollView(width=4, height=3, children=[FixedRenderChild(["x"])])
        view._app = app
        view._last_natural_skip = 0
        app.clear_dirty()

        handled = view.handle_key({"type": "mouse_wheel", "direction": "down"})

        self.assertFalse(handled)
        self.assertFalse(app.is_dirty)

    def test_scrollview_wheel_moves_multiple_lines(self):
        """One wheel event should move by the configured accelerated line step."""
        app = UIApplication()
        view = PlainScrollView(width=4, height=12, children=[FixedRenderChild([str(i) for i in range(30)])])
        view._app = app
        view.render()
        start_offset = view._view_offset
        app.clear_dirty()

        handled = view.handle_key({"type": "mouse_wheel", "direction": "down"})

        self.assertTrue(handled)
        self.assertEqual(view._view_offset, start_offset - view.LINES_PER_SCROLL)
        self.assertTrue(app.is_dirty)

    def test_scrollview_coalesced_wheel_delta_moves_once_by_combined_lines(self):
        """Coalesced wheel input should apply one bounded multi-line move."""
        app = UIApplication()
        view = PlainScrollView(width=4, height=12, children=[FixedRenderChild([str(i) for i in range(30)])])
        view._app = app
        view.render()
        start_offset = view._view_offset
        app.clear_dirty()

        handled = view.handle_key({"type": "mouse_wheel", "direction": "down", "wheel_delta": -3})

        self.assertTrue(handled)
        self.assertEqual(view._view_offset, start_offset - view.LINES_PER_SCROLL * 3)
        self.assertTrue(app.is_dirty)

    def test_scrollview_alt_arrows_move_multiple_lines(self):
        """Alt+Up/Alt+Down should use the same accelerated step as wheel scroll."""
        app = UIApplication()
        view = PlainScrollView(width=4, height=12, children=[FixedRenderChild([str(i) for i in range(30)])])
        view._app = app
        view.render()
        top_offset = view._view_offset
        app.clear_dirty()

        self.assertTrue(view.handle_key("Alt+Down"))
        self.assertEqual(view._view_offset, top_offset - view.LINES_PER_SCROLL)
        self.assertTrue(app.is_dirty)

        app.clear_dirty()
        self.assertTrue(view.handle_key("Alt+Up"))
        self.assertEqual(view._view_offset, top_offset)
        self.assertTrue(app.is_dirty)

    def test_scrollview_ctrl_arrows_scroll_by_page(self):
        app = UIApplication()
        view = PlainScrollView(width=4, height=12, children=[FixedRenderChild([str(i) for i in range(30)])])
        view._app = app
        view.render()
        top_offset = view._view_offset
        app.clear_dirty()

        self.assertTrue(view.handle_key("Ctrl+Down"))
        self.assertEqual(view._view_offset, top_offset - view._clamped_viewport_rect()["height"])
        self.assertTrue(app.is_dirty)

        app.clear_dirty()
        self.assertTrue(view.handle_key("Ctrl+Up"))
        self.assertEqual(view._view_offset, top_offset)
        self.assertTrue(app.is_dirty)


if __name__ == "__main__":
    unittest.main()
