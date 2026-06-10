"""Tests for UI elements (Label, TextInput, NumberInput, Button, CheckBox, ComboBox, ListBox)."""

import sys
import os
import re
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

import unittest
from runtime.elements import (Label, TextInput, TextArea, NumberInput, Button, CheckBox,
                                  ComboBox, ListBox, UIElementReusable, ViewHost,
                                  FrameBufferView)
from runtime.image import Image, terminal_supports_sixel
from runtime.rendering import TerminalCell
from runtime.UIBase import UIBase
from runtime.UIScrollView import UIScrollView
from runtime.uiwindow import UIWindow
from runtime.style import Style, Color


ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")


class FixedViewportScrollView(UIScrollView):
    def __init__(self, width, height, children=None):
        super().__init__()
        self.width = width
        self.height = height
        self._window_width = width
        self._window_height = height
        self._viewport = {
            "row": 0,
            "col": 0,
            "width": width,
            "height": height,
            "gap": 0,
        }
        for child in children or []:
            self.add_child(child)

    def _get_viewport_rect(self):
        return dict(self._viewport)

    def get_all_elements(self):
        return []


class TestImage(unittest.TestCase):
    """Test cases for the Image element."""

    def test_image_fallback_renders_colored_cells(self):
        image = Image(
            name="sample",
            source="shared/assets/image_samples/coins.png",
            alt="coins",
            render_mode="fallback",
        )
        image.width = 8
        image.height = 4

        rows = image.render_cells()

        self.assertEqual(len(rows), 4)
        self.assertEqual(len(rows[0]), 8)
        self.assertTrue(any(cell.text == "▀" for row in rows for cell in row))
        self.assertTrue(any(cell.foreground is not None for row in rows for cell in row))

    def test_image_sixel_mode_uses_raw_cell_when_forced(self):
        old_force = os.environ.get("UIMD_FORCE_SIXEL")
        old_disable = os.environ.get("UIMD_DISABLE_SIXEL")
        os.environ.pop("UIMD_FORCE_SIXEL", None)
        os.environ.pop("UIMD_DISABLE_SIXEL", None)
        try:
            image = Image(
                name="sample",
                source="shared/assets/image_samples/camera.png",
                alt="camera",
                render_mode="sixel",
            )
            image.width = 2
            image.height = 1

            rows = image.render_cells()

            self.assertTrue(rows[0][0].raw.startswith("\x1bPq"))
            self.assertEqual(rows[0][0].raw_width, 2)
            self.assertEqual(rows[0][0].raw_height, 1)
        finally:
            if old_force is None:
                os.environ.pop("UIMD_FORCE_SIXEL", None)
            else:
                os.environ["UIMD_FORCE_SIXEL"] = old_force
            if old_disable is None:
                os.environ.pop("UIMD_DISABLE_SIXEL", None)
            else:
                os.environ["UIMD_DISABLE_SIXEL"] = old_disable

    def test_image_sixel_mode_can_be_disabled_for_fallback(self):
        os.environ["UIMD_DISABLE_SIXEL"] = "1"
        try:
            image = Image(
                name="sample",
                source="shared/assets/image_samples/camera.png",
                alt="camera",
                render_mode="sixel",
            )
            image.width = 2
            image.height = 1

            rows = image.render_cells()

            self.assertEqual(rows[0][0].raw, "")
            self.assertTrue(any(cell.text == "▀" for row in rows for cell in row))
        finally:
            os.environ.pop("UIMD_DISABLE_SIXEL", None)

    def test_xterm_term_name_does_not_imply_sixel_support(self):
        old_term = os.environ.get("TERM")
        old_term_program = os.environ.get("TERM_PROGRAM")
        os.environ["TERM"] = "xterm-256color"
        os.environ.pop("TERM_PROGRAM", None)
        os.environ.pop("UIMD_FORCE_SIXEL", None)
        os.environ.pop("UIMD_DISABLE_SIXEL", None)
        try:
            self.assertFalse(terminal_supports_sixel())
        finally:
            if old_term is None:
                os.environ.pop("TERM", None)
            else:
                os.environ["TERM"] = old_term
            if old_term_program is None:
                os.environ.pop("TERM_PROGRAM", None)
            else:
                os.environ["TERM_PROGRAM"] = old_term_program


class TestLabel(unittest.TestCase):
    """Test cases for the Label element."""

    def test_label_default_text(self):
        """Test label with default text."""
        label = Label(name="test")
        self.assertEqual(label.text, "")

    def test_label_set_text(self):
        """Test setting label text."""
        label = Label(name="test", text="Hello")
        self.assertEqual(label.text, "Hello")

    def test_label_render_single_line(self):
        """Test rendering a single-line label."""
        label = Label(name="test", text="Hello World", width=10)
        lines = label.render()
        self.assertEqual(len(lines), 1)
        self.assertTrue("Hello W" in lines[0])

    def test_label_render_multi_line(self):
        """Test rendering a multi-line label with word wrap."""
        label = Label(name="test", text="Hello World Foo Bar", width=10, height=3)
        lines = label.render()
        self.assertEqual(len(lines), 3)

    def test_label_render_preserves_explicit_newlines(self):
        """Explicit newlines should render as separate rows."""
        label = Label(name="test", text="one\ntwo", width=5, height=2)
        lines = label.render()
        self.assertEqual(len(lines), 2)
        self.assertIn("one", lines[0])
        self.assertIn("two", lines[1])

    def test_label_render_line(self):
        """Test render_line method."""
        label = Label(name="test", text="Hello")
        self.assertEqual(label.render_line(0), "Hello"[:label.width if label.width else 5])

    def test_ansi_slice_resets_background_between_spans(self):
        """Sliced ANSI text should not leak a span background into following text."""
        line = (
            "\x1b[38;2;001;002;003mA\x1b[0m"
            "\x1b[38;2;004;005;006m\x1b[48;2;007;008;009mB\x1b[0m"
            "\x1b[38;2;004;005;006mC\x1b[0m"
        )

        sliced = UIBase._slice_rendered_text(line, 0, 3)
        cells = UIBase._parse_ansi_chars(sliced)

        self.assertEqual(cells, [
            ("A", "001002003", None),
            ("B", "004005006", "007008009"),
            ("C", "004005006", None),
        ])

    def test_label_word_wrap_default_enabled(self):
        """Word wrap should be enabled by default for labels with height > 1."""
        label = Label(name="test", text="Hello World Foo Bar Baz", width=10, height=3)
        lines = label.render()
        self.assertGreater(len(lines), 1)

    def test_label_word_wrap_disabled(self):
        """Word wrap disabled should not split text into multiple content rows."""
        label = Label(name="test", text="Hello World Foo Bar Baz", width=10, height=3)
        label.style.word_wrap = False
        lines = label.render()
        # height=3 means 1 content row + 2 padded empty rows
        self.assertEqual(len(lines), 3)
        # First line should be truncated, not wrapped
        self.assertIn("Hello", lines[0])

    def test_label_word_wrap_disabled_truncates(self):
        """Word wrap disabled should truncate text to width."""
        label = Label(name="test", text="Hello World Foo Bar Baz", width=10, height=3)
        label.style.word_wrap = False
        lines = label.render()
        self.assertTrue(len(lines[0]) <= 10)

    def test_label_word_wrap_explicit_true(self):
        """Explicit word-wrap: true should wrap text."""
        label = Label(name="test", text="Hello World Foo Bar Baz", width=10, height=3)
        label.style.word_wrap = True
        lines = label.render()
        self.assertGreater(len(lines), 1)

    def test_label_visual_rows_no_wrap(self):
        """_visual_rows with word wrap disabled should return single row."""
        label = Label(name="test", text="Hello World Foo Bar Baz", width=10, height=3)
        label.style.word_wrap = False
        rows = label._visual_rows(10)
        self.assertEqual(len(rows), 1)

    def test_label_selection_render_cells_uses_cursor_style(self):
        """Label render_cells should preserve per-character selection styling."""
        label = Label(name="test", text="Hello", width=5)
        label.style.set("color", "#e5e7eb")
        label.style.set("background", "#0d1524")
        label.cursor_style = Style()
        label.cursor_style.set("color", "#111827")
        label.cursor_style.set("background", "#facc15")
        label.select_range(1, 4)

        cells = label.render_cells()[0]

        self.assertEqual("".join(cell.text for cell in cells), "Hello")
        self.assertEqual(cells[1].foreground, Color("#111827"))
        self.assertEqual(cells[1].background, Color("#facc15"))
        self.assertEqual(cells[4].foreground, Color("#e5e7eb"))


class TestUIElementReusable(unittest.TestCase):
    """Test reusable custom UI element behavior."""

    def test_reusable_element_keeps_live_child_instance(self):
        """Custom UI children should keep state between renders."""
        class Child:
            def __init__(self):
                self.parent = None
                self._app = None
                self.open_count = 0
                self.messages = []
                self.size = None

            def open(self):
                self.open_count += 1

            def resize(self, width, height):
                self.size = (width, height)

            def append_message(self, message):
                self.messages.append(message)

            def render(self):
                return list(self.messages)

        elem = UIElementReusable(name="chat", layout_class=Child, width=20, height=3)
        elem.open()
        elem.append_message("first")
        first_child = elem.child
        elem.render()
        elem.append_message("second")

        self.assertIs(first_child, elem.child)
        self.assertEqual(first_child.messages, ["first", "second"])
        self.assertEqual(first_child.open_count, 1)
        self.assertEqual(first_child.size, (20, 3))

    def test_reusable_element_background_overrides_opaque_child_window_background(self):
        """Name styles on a reusable element should style the embedded child root."""
        class Child:
            def __init__(self):
                self.parent = None
                self._app = None
                self.style = Style()
                self.style.set("background", "#162033")

            def open(self):
                self.style.set("background", "#162033")

            def resize(self, width, height):
                pass

            def render(self):
                return []

        elem = UIElementReusable(name="chat", layout_class=Child, width=20, height=3)
        elem.style.set("background", "#000000")

        elem.open()

        self.assertEqual(elem.child.style.background.hex, "#000000")

    def test_reusable_element_keeps_translucent_child_window_background(self):
        """Translucent child backgrounds should blend over the reusable parent background."""
        class Child:
            def __init__(self):
                self.parent = None
                self._app = None
                self.style = Style()
                self.style.set("background", "#ff000000")

            def open(self):
                self.style.set("background", "#ff000000")

            def resize(self, width, height):
                pass

            def render(self):
                return []

        elem = UIElementReusable(name="chat", layout_class=Child, width=20, height=3)
        elem.style.set("background", "#000000")

        elem.open()

        self.assertEqual(elem.child.style.background.hex, "#ff000000")
        self.assertEqual(elem.child.style.background.alpha, 0.0)

    def test_descendant_focus_applies_reusable_focus_background(self):
        """A focused embedded child should render through the reusable element focus style."""
        class ChildWindow(UIWindow):
            def __init__(self):
                super().__init__(title="Child")
                self.style = Style()
                self.style.set("background", "#102030")
                button = self.create_element("pick", "button", title="Go")
                button.width = 4

        elem = UIElementReusable(name="gallery", layout_class=ChildWindow, width=4, height=1)
        elem.style = Style()
        elem.style.set("background", "#102030")
        elem.focus_style = Style()
        elem.focus_style.set("background", "#1e3a5f")
        elem.open()
        elem.child.set_focus(elem.child._elements["pick"])

        rows = elem.render_cells()
        backgrounds = {
            getattr(cell.background, "hex", None)
            for row in rows
            for cell in row
            if cell.background is not None
        }

        self.assertIn("#1e3a5f", backgrounds)


class TestViewHost(unittest.TestCase):
    """Test dynamic hosted view behavior."""

    @staticmethod
    def _single_element_window(element_type, element_name, kwargs):
        class ChildWindow(UIWindow):
            def __init__(self):
                super().__init__(
                    title="Child",
                    layout=[{
                        "row": 0,
                        "col": 0,
                        "cell_row": 0,
                        "cell_col": 0,
                        "cell_chars_width": 8,
                        "cell_chars_height": 1,
                        "chars_width": 8,
                        "chars_height": 1,
                        "width": 8,
                        "height": 1,
                        "type": element_type,
                        "name": element_name,
                    }],
                    members={element_name: {"type": element_type, **kwargs}},
                )
                setattr(self, element_name, self.create_element(element_name, element_type, **kwargs))

        return ChildWindow()

    def test_viewhost_can_replace_and_clear_current_view(self):
        host = ViewHost("main", width=8, height=1)
        first = host.set_view(self._single_element_window("button", "open_btn", {"title": "Open"}))
        self.assertIs(host.current_view(), first)
        self.assertIn("Open", ANSI_RE.sub("", "".join(host.render())))

        second = host.set_view(self._single_element_window("textinput", "name", {"value": "Ada"}))
        self.assertIs(host.current_view(), second)
        self.assertIn("Ada", ANSI_RE.sub("", "".join(host.render())))

        host.clear_view()
        self.assertIsNone(host.current_view())


class TestTextInput(unittest.TestCase):
    """Test cases for the TextInput element."""

    def test_textinput_default_value(self):
        """Test text input with default value."""
        inp = TextInput(name="test")
        self.assertEqual(inp.value, "")

    def test_textinput_render_cells_uses_cursor_style(self):
        """TextInput render_cells should expose the cursor cell directly."""
        inp = TextInput(name="test", value="abc", width=4)
        inp.style.set("color", "#e5e7eb")
        inp.style.set("background", "#0d1524")
        inp.cursor_style = Style()
        inp.cursor_style.set("color", "#111827")
        inp.cursor_style.set("background", "#facc15")
        inp.edit_mode = True
        inp.cursor_pos = 1

        cells = inp.render_cells()[0]

        self.assertEqual("".join(cell.text for cell in cells), "abc ")
        self.assertEqual(cells[1].foreground, Color("#111827"))
        self.assertEqual(cells[1].background, Color("#facc15"))

    def test_textinput_set_value(self):
        """Test setting text input value."""
        inp = TextInput(name="test", value="Hello")
        self.assertEqual(inp.value, "Hello")

    def test_textinput_cursor_position(self):
        """Test cursor position tracking."""
        inp = TextInput(name="test", value="Hello")
        self.assertEqual(inp.cursor_pos, 5)

    def test_textinput_handle_key_left(self):
        """Test left arrow key."""
        inp = TextInput(name="test", value="Hello")
        inp.cursor_pos = 5
        inp.handle_key("Left")
        self.assertEqual(inp.cursor_pos, 4)

    def test_textinput_handle_key_right(self):
        """Test right arrow key."""
        inp = TextInput(name="test", value="Hello")
        inp.handle_key("Right")
        self.assertEqual(inp.cursor_pos, 5)

    def test_textinput_handle_key_backspace(self):
        """Test backspace key."""
        inp = TextInput(name="test", value="Hello")
        inp.cursor_pos = 5
        inp.handle_key("Backspace")
        self.assertEqual(inp.value, "Hell")
        self.assertEqual(inp.cursor_pos, 4)

    def test_textinput_handle_key_delete(self):
        """Test delete key."""
        inp = TextInput(name="test", value="Hello")
        inp.cursor_pos = 2
        inp.handle_key("Delete")
        self.assertEqual(inp.value, "Helo")

    def test_textinput_handle_key_char(self):
        """Test typing a character."""
        inp = TextInput(name="test", value="He")
        inp.cursor_pos = 2
        inp.handle_key("l")
        inp.handle_key("l")
        inp.handle_key("o")
        self.assertEqual(inp.value, "Hello")

    def test_textinput_handle_key_home(self):
        """Test home key."""
        inp = TextInput(name="test", value="Hello")
        inp.cursor_pos = 5
        inp.handle_key("Home")
        self.assertEqual(inp.cursor_pos, 0)

    def test_textinput_handle_key_end(self):
        """Test end key."""
        inp = TextInput(name="test", value="Hello")
        inp.cursor_pos = 0
        inp.handle_key("End")
        self.assertEqual(inp.cursor_pos, 5)

    def test_textinput_maxlength(self):
        """Test maxlength restriction."""
        inp = TextInput(name="test", value="", maxlength=5)
        for ch in "Hello":
            inp.handle_key(ch)
        inp.handle_key("X")  # Should be ignored (max reached)
        self.assertEqual(inp.value, "Hello")

    def test_textinput_password_mode(self):
        """Test password mode rendering."""
        inp = TextInput(name="test", value="secret", password=True, width=10)
        lines = inp.render()
        # Password mode replaces all chars with asterisks
        self.assertTrue(lines[0].startswith("******"))

    def test_textinput_render(self):
        """Test basic rendering."""
        inp = TextInput(name="test", value="Hello", width=10)
        lines = inp.render()
        self.assertEqual(len(lines), 1)

    def test_textinput_render_uses_full_assigned_width(self):
        """Single-line text inputs should fill the whole assigned width."""
        inp = TextInput(name="test", value="Hello", width=10)
        lines = inp.render()
        self.assertEqual(len(lines[0]), 10)

    def test_textarea_mouse_wheel_scrolls(self):
        """Mouse wheel should scroll text areas."""
        inp = TextArea(name="test", value="one\ntwo\nthree", width=10, height=2)

        handled = inp.handle_key({"type": "mouse_wheel", "direction": "down"})

        self.assertTrue(handled)
        self.assertEqual(inp._scroll_offset, 1)
        self.assertIn("two", ANSI_RE.sub("", inp.render()[0]))

        handled = inp.handle_key({"type": "mouse_wheel", "direction": "up"})

        self.assertTrue(handled)
        self.assertEqual(inp._scroll_offset, 0)

    def test_textarea_coalesced_mouse_wheel_delta_scrolls_multiple_rows(self):
        """A coalesced wheel event should move by its bounded delta."""
        inp = TextArea(name="test", value="one\ntwo\nthree\nfour", width=10, height=2)

        handled = inp.handle_key({"type": "mouse_wheel", "direction": "down", "wheel_delta": -2})

        self.assertTrue(handled)
        self.assertEqual(inp._scroll_offset, 2)

    def test_textarea_selection_does_not_highlight_padding_after_short_line(self):
        """Multiline selection should not style nonexistent cells after row text."""
        inp = TextArea(name="test", value="aaa\nbbb", width=8, height=2)
        inp.cursor_style = Style()
        inp.cursor_style.set("background", "#b8d7ff")
        inp.cursor_style.set("color", "#000000")
        inp._edit_mode = True

        inp.select_range(1, len(inp.value))
        rows = [[bg for _ch, _fg, bg in UIBase._parse_ansi_chars(line)] for line in inp.render()]

        selected_bg = "184215255"
        self.assertEqual(rows[0][1:3], [selected_bg, selected_bg])
        self.assertNotIn(selected_bg, rows[0][3:])
        self.assertEqual(rows[1][:3], [selected_bg, selected_bg, selected_bg])
        self.assertNotIn(selected_bg, rows[1][3:])

    def test_textarea_mapped_cursor_highlights_only_current_visual_row(self):
        """Cursor rendering for tabs/placeholders should not leak into later rows."""
        value = "Plain line\nTabbed\tline\nControl\x1bline\nWide\u4e2dline"
        inp = TextArea(name="test", value=value, width=20, height=4)
        inp.style.set("background", "#2d456d")
        inp.cursor_style = Style()
        inp.cursor_style.set("background", "#facc15")
        inp.cursor_style.set("color", "#111827")
        inp._edit_mode = True

        for cursor_pos in (0, 1, value.index("\t"), value.index("\x1b"), value.index("\u4e2d"), len(value)):
            inp.cursor_pos = cursor_pos
            inp._selection_start = None
            highlighted = [
                (row_index, col_index)
                for row_index, row in enumerate(inp.render_cells())
                for col_index, cell in enumerate(row)
                if cell.background == Color("#facc15")
            ]
            self.assertEqual(len(highlighted), 1, cursor_pos)

    def test_textarea_mapped_selection_highlights_tab_expansion_without_extra_cursor(self):
        """Selecting a raw tab should highlight its visual spaces and not the cursor cell."""
        value = "Plain line\nTabbed\tline"
        inp = TextArea(name="test", value=value, width=20, height=2)
        inp.style.set("background", "#2d456d")
        inp.cursor_style = Style()
        inp.cursor_style.set("background", "#facc15")
        inp.cursor_style.set("color", "#111827")
        inp._edit_mode = True
        tab_pos = value.index("\t")

        inp.select_range(tab_pos, tab_pos + 1)

        highlighted = [
            (row_index, col_index)
            for row_index, row in enumerate(inp.render_cells())
            for col_index, cell in enumerate(row)
            if cell.background == Color("#facc15")
        ]
        self.assertEqual(highlighted, [(1, 6), (1, 7)])

    def test_textinput_format_validation(self):
        """Test format regex validation."""
        inp = TextInput(name="test", value="invalid", format_regex=r"^\d+$")
        self.assertTrue(inp.error)

    def test_textinput_format_valid(self):
        """Test format regex validation - valid."""
        inp = TextInput(name="test", value="12345", format_regex=r"^\d+$")
        self.assertFalse(inp.error)

    def test_textinput_cmd_copy_paste_shortcuts(self):
        """Text input should support cmd copy/paste shortcuts when terminal sends them."""
        inp = TextInput(name="test", value="Hello")
        inp.select_range(1, 4)
        inp.handle_key("cmd_c")
        inp.cursor_pos = 5
        inp._selection_start = None
        inp.handle_key("cmd_v")
        self.assertEqual(inp.value, "Helloell")

    def test_textinput_bracketed_paste_event_inserts_text(self):
        """Bracketed paste should insert pasted text directly."""
        inp = TextInput(name="test", value="Hello")
        inp.cursor_pos = 5

        inp.handle_key({"type": "paste", "text": " world"})

        self.assertEqual(inp.value, "Hello world")
        self.assertEqual(inp.cursor_pos, len("Hello world"))

    def test_textarea_paste_normalizes_crlf_newlines_for_multiline_render(self):
        """Pasted CRLF text should render as rows without raw terminal control chars."""
        inp = TextArea(name="test", value="", width=20, height=3)
        inp._edit_mode = True

        inp.handle_key({
            "type": "paste",
            "text": "line1 123 456\r\nline2 xyz\r\nline3 aaaa bbb ccc",
        })

        self.assertEqual(inp.value, "line1 123 456\nline2 xyz\nline3 aaaa bbb ccc")
        self.assertEqual(inp.cursor_pos, len(inp.value))
        lines = [ANSI_RE.sub("", line) for line in inp.render()]
        self.assertTrue(all("\r" not in line and "\n" not in line for line in lines))
        self.assertEqual(lines[0].rstrip(), "line1 123 456")
        self.assertEqual(lines[1].rstrip(), "line2 xyz")
        self.assertEqual(lines[2].rstrip(), "line3 aaaa bbb ccc")

    def test_textinput_value_normalizes_newlines_to_spaces(self):
        """Single-line text inputs should not preserve multiline values."""
        inp = TextInput(name="test", value="a\r\nb\rc")

        self.assertEqual(inp.value, "a b c")
        inp.value = "x\r\ny"
        self.assertEqual(inp.value, "x y")

    def test_textarea_value_normalizes_crlf_newlines(self):
        """Programmatic text area values should preserve normalized newlines."""
        inp = TextArea(name="test", value="a\r\nb\rc")

        self.assertEqual(inp.value, "a\nb\nc")
        inp.value = "x\r\ny"
        self.assertEqual(inp.value, "x\ny")

    def test_textinput_copy_selection_populates_clipboard(self):
        """Explicit copy_selection should populate the internal clipboard."""
        source = TextInput(name="source", value="Hello")
        target = TextInput(name="target", value="")

        source.select_range(1, 4)
        source.copy_selection()
        target.handle_key("cmd_v")

        self.assertEqual(target.value, "ell")

    def test_textinput_shift_selection_does_not_auto_copy(self):
        """Keyboard selection should NOT auto-copy; clipboard unchanged until explicit copy."""
        import runtime.elements as elements_module
        elements_module._TEXT_CLIPBOARD = "sentinel"

        source = TextInput(name="source", value="Hello")
        source.cursor_pos = 1
        source.handle_key("Shift+Right")
        source.handle_key("Shift+Right")

        self.assertEqual(elements_module._TEXT_CLIPBOARD, "sentinel")


class TestNumberInput(unittest.TestCase):
    """Test cases for the NumberInput element."""

    def test_numberinput_default_value(self):
        """Test number input with default value."""
        inp = NumberInput(name="test")
        self.assertEqual(inp.value, 0.0)

    def test_numberinput_set_value(self):
        """Test setting number input value."""
        inp = NumberInput(name="test", value=42.5)
        self.assertEqual(inp.value, 42.5)

    def test_numberinput_up(self):
        """Test up arrow key increases value."""
        inp = NumberInput(name="test", value=0, step_size=1)
        inp.handle_key("Up")
        self.assertEqual(inp.value, 1.0)

    def test_numberinput_down(self):
        """Test down arrow key decreases value."""
        inp = NumberInput(name="test", value=0, step_size=1)
        inp.handle_key("Down")
        self.assertEqual(inp.value, -1.0)

    def test_numberinput_min_value(self):
        """Test minimum value constraint."""
        inp = NumberInput(name="test", value=0, step_size=1, min_value=0)
        inp.handle_key("Down")
        self.assertEqual(inp.value, 0.0)

    def test_numberinput_max_value(self):
        """Test maximum value constraint."""
        inp = NumberInput(name="test", value=10, step_size=1, max_value=10)
        inp.handle_key("Up")
        self.assertEqual(inp.value, 10.0)

    def test_numberinput_display_value(self):
        """Test display value formatting."""
        inp = NumberInput(name="test", value=3.14, format_str="0.00")
        self.assertEqual(inp.display_value, "3.14")

    def test_numberinput_render(self):
        """Test basic rendering."""
        inp = NumberInput(name="test", value=42, width=10)
        lines = inp.render()
        self.assertEqual(len(lines), 1)

    def test_numberinput_render_shows_cursor_in_edit_mode(self):
        """NumberInput should render cursor style in edit mode like TextInput."""
        inp = NumberInput(name="test", value=42, width=4)
        inp.style.set("background", "#0d1524")
        inp.style.set("color", "#e5e7eb")
        inp.cursor_style = Style()
        inp.cursor_style.set("background", "#facc15")
        inp.cursor_style.set("color", "#111827")
        inp.edit_mode = True
        inp.cursor_pos = len(inp.display_value)

        rendered = inp.render()[0]

        self.assertEqual(ANSI_RE.sub("", rendered), "42  ")
        self.assertIn("\x1b[48;2;250;204;21m", rendered)

    def test_numberinput_render_highlights_selection_in_edit_mode(self):
        """NumberInput should render selected numeric text with cursor style."""
        inp = NumberInput(name="test", value=42, width=4)
        inp.style.set("background", "#0d1524")
        inp.style.set("color", "#e5e7eb")
        inp.cursor_style = Style()
        inp.cursor_style.set("background", "#facc15")
        inp.cursor_style.set("color", "#111827")
        inp.edit_mode = True
        inp.select_range(0, len(inp.display_value))

        rendered = inp.render()[0]

        self.assertEqual(ANSI_RE.sub("", rendered), "42  ")
        self.assertIn("\x1b[48;2;250;204;21m42", rendered)

    def test_numberinput_replacement_cursor_moves_after_inserted_digit(self):
        """Replacing selected numeric text should draw cursor after the inserted digit."""
        inp = NumberInput(name="test", value=0, width=4)
        inp.style.set("background", "#0d1524")
        inp.style.set("color", "#e5e7eb")
        inp.cursor_style = Style()
        inp.cursor_style.set("background", "#facc15")
        inp.cursor_style.set("color", "#111827")
        inp.edit_mode = True
        inp.select_range(0, len(inp.display_value))

        inp.handle_key("1")
        rendered = inp.render()[0]

        self.assertEqual(inp.display_value, "1")
        self.assertEqual(inp.cursor_pos, 1)
        self.assertEqual(ANSI_RE.sub("", rendered), "1   ")
        self.assertIn("\x1b[48;2;250;204;21m ", rendered)

    def test_numberinput_empty_edit_text_stays_empty_until_commit(self):
        """Deleting all text in edit mode should not immediately display formatted zero."""
        inp = NumberInput(name="test", value=1, width=4)
        inp.edit_mode = True
        inp.begin_edit()
        inp.select_range(0, len(inp.display_value))

        inp.handle_key("Backspace")

        self.assertEqual(inp.display_value, "")
        self.assertEqual(ANSI_RE.sub("", inp.render()[0]), "    ")

    def test_numberinput_cmd_copy_paste_shortcuts(self):
        """Number input should support cmd copy/paste shortcuts when terminal sends them."""
        inp = NumberInput(name="test", value=12)
        inp.select_range(0, len(inp.display_value))
        inp.handle_key("cmd_c")
        inp.value = 0
        inp.cursor_pos = len(inp.display_value)
        inp.handle_key("cmd_v")
        self.assertEqual(inp.value, 12.0)

    def test_numberinput_copy_selection_populates_clipboard(self):
        """Explicit copy_selection should populate the internal clipboard."""
        source = NumberInput(name="source", value=12)
        target = NumberInput(name="target", value=0)

        source.select_range(0, len(source.display_value))
        source.copy_selection()
        target.cursor_pos = 0
        target.handle_key("cmd_v")

        self.assertEqual(target.value, 120.0)


class TestButton(unittest.TestCase):
    """Test cases for the Button element."""

    def test_button_default_title(self):
        """Test button with default title."""
        btn = Button(name="test")
        self.assertEqual(btn.title, "Button")

    def test_button_set_title(self):
        """Test setting button title."""
        btn = Button(name="test", title="Click Me")
        self.assertEqual(btn.title, "Click Me")

    def test_button_render(self):
        """Test button rendering."""
        btn = Button(name="test", title="OK", width=10)
        lines = btn.render()
        self.assertEqual(len(lines), 1)
        self.assertEqual(len(lines[0]), 10)

    def test_button_render_focused(self):
        """Test button rendering when focused."""
        btn = Button(name="test", title="OK", width=10)
        btn.focused = True
        lines = btn.render()
        self.assertTrue(lines[0].startswith(" ["))
        self.assertTrue(lines[0].endswith("] "))
        self.assertIn("OK", lines[0])
        self.assertEqual(len(lines[0]), 10)

    def test_button_focused_brackets_use_edges_with_inner_padding(self):
        """Wide focused buttons should inset brackets with inner padding."""
        btn = Button(name="test", title="0", width=7)
        btn.focused = True
        lines = btn.render()
        self.assertTrue(lines[0].startswith(" ["))
        self.assertTrue(lines[0].endswith("] "))
        self.assertIn(" 0 ", lines[0])
        self.assertEqual(len(lines[0]), 7)

    def test_button_render_respects_small_width(self):
        """Buttons must never overflow the assigned layout width."""
        btn = Button(name="test", title="DEL", width=3)
        lines = btn.render()
        self.assertEqual(lines[0], "DEL")


class TestCheckBox(unittest.TestCase):
    """Test cases for the CheckBox element."""

    def test_checkbox_default_value(self):
        """Test checkbox with default value."""
        cb = CheckBox(name="test")
        self.assertFalse(cb.value)

    def test_checkbox_set_value(self):
        """Test setting checkbox value."""
        cb = CheckBox(name="test", value=True)
        self.assertTrue(cb.value)

    def test_checkbox_toggle(self):
        """Test toggling checkbox with Enter key."""
        cb = CheckBox(name="test", value=False)
        cb.handle_key("Enter")
        self.assertTrue(cb.value)

    def test_checkbox_toggle_off(self):
        """Test toggling checkbox off."""
        cb = CheckBox(name="test", value=True)
        cb.handle_key("Enter")
        self.assertFalse(cb.value)

    def test_checkbox_render_checked(self):
        """Test rendering checked checkbox."""
        cb = CheckBox(name="test", value=True, title="Option")
        lines = cb.render()
        self.assertIn("[x]", lines[0])

    def test_checkbox_render_unchecked(self):
        """Test rendering unchecked checkbox."""
        cb = CheckBox(name="test", value=False, title="Option")
        lines = cb.render()
        self.assertIn("[ ]", lines[0])


class TestComboBox(unittest.TestCase):
    """Test cases for the ComboBox element."""

    def test_combobox_default_options(self):
        """Test combobox with no options."""
        cb = ComboBox(name="test")
        self.assertEqual(cb._options, [])

    def test_combobox_options(self):
        """Test combobox with options."""
        cb = ComboBox(name="test", options=["A", "B", "C"])
        self.assertEqual(cb._options, ["A", "B", "C"])

    def test_combobox_selected_item(self):
        """Test selected item property."""
        cb = ComboBox(name="test", options=["A", "B", "C"], selected_item="B")
        self.assertEqual(cb.selected_item, "B")

    def test_combobox_open_menu(self):
        """Test opening menu with Enter."""
        cb = ComboBox(name="test", options=["A", "B", "C"])
        self.assertFalse(cb._menu_open)
        cb.handle_key("Enter")
        self.assertTrue(cb._menu_open)

    def test_combobox_close_menu_escape(self):
        """Test closing menu with Escape."""
        cb = ComboBox(name="test", options=["A", "B", "C"])
        cb.handle_key("Enter")
        cb.handle_key("Escape")
        self.assertFalse(cb._menu_open)

    def test_combobox_navigate_options(self):
        """Test navigating options with arrow keys."""
        cb = ComboBox(name="test", options=["A", "B", "C"], selected_item="A")
        cb.handle_key("Enter")  # Open menu
        cb.handle_key("Down")   # Move to B
        self.assertEqual(cb.selected_item, "B")

    def test_combobox_render(self):
        """Test basic rendering."""
        cb = ComboBox(name="test", options=["A", "B"], width=10)
        lines = cb.render()
        self.assertGreater(len(lines), 0)


class TestListBox(unittest.TestCase):
    """Test cases for the ListBox element."""

    def test_listbox_default_options(self):
        """Test listbox with no options."""
        lb = ListBox(name="test")
        self.assertEqual(lb._options, [])

    def test_listbox_options(self):
        """Test listbox with options."""
        lb = ListBox(name="test", options=["A", "B", "C"])
        self.assertEqual(lb._options, ["A", "B", "C"])

    def test_listbox_selected_items(self):
        """Test selected items property."""
        lb = ListBox(name="test", options=["A", "B", "C"], selected_items=["B"])
        self.assertEqual(lb.selected_items, ["B"])

    def test_listbox_single_select(self):
        """Test single selection mode."""
        lb = ListBox(name="test", options=["A", "B", "C"], multiple=False)
        lb._selected_items = ["A"]
        lb.handle_key("Down")  # Move to B
        self.assertEqual(lb.selected_items, ["B"])

    def test_listbox_multi_select(self):
        """Test multi selection mode."""
        lb = ListBox(name="test", options=["A", "B", "C"], multiple=True)
        lb._selected_items = ["A"]
        lb.handle_key("Enter")  # Toggle A off
        self.assertEqual(lb.selected_items, [])

    def test_listbox_render(self):
        """Test basic rendering."""
        lb = ListBox(name="test", options=["A", "B", "C"], width=10, height=3)
        lines = lb.render()
        self.assertEqual(len(lines), 3)
        self.assertEqual(ANSI_RE.sub("", lines[0]), "A         ")

    def test_listbox_mouse_wheel_scrolls_without_changing_selection(self):
        """Mouse wheel should move the visible listbox window only."""
        lb = ListBox(name="test", options=["A", "B", "C", "D"], selected_items=["A"], width=10, height=2)

        handled = lb.handle_key({"type": "mouse_wheel", "direction": "down"})

        self.assertTrue(handled)
        self.assertEqual(lb._scroll_offset, 1)
        self.assertEqual(lb.selected_items, ["A"])

        handled = lb.handle_key({"type": "mouse_wheel", "direction": "up"})

        self.assertTrue(handled)
        self.assertEqual(lb._scroll_offset, 0)
        self.assertEqual(lb.selected_items, ["A"])

    def test_listbox_coalesced_mouse_wheel_delta_scrolls_multiple_rows(self):
        """A coalesced wheel event should apply its combined delta once."""
        lb = ListBox(name="test", options=["A", "B", "C", "D", "E"], selected_items=["A"], width=10, height=2)

        handled = lb.handle_key({"type": "mouse_wheel", "direction": "down", "wheel_delta": -2})

        self.assertTrue(handled)
        self.assertEqual(lb._scroll_offset, 2)
        self.assertEqual(lb.selected_items, ["A"])

    def test_listbox_mouse_wheel_ignores_fully_visible_list(self):
        """Mouse wheel should not handle a listbox that has no hidden rows."""
        lb = ListBox(name="test", options=["A", "B"], selected_items=["A"], width=10, height=3)

        handled = lb.handle_key({"type": "mouse_wheel", "direction": "down"})

        self.assertFalse(handled)
        self.assertEqual(lb._scroll_offset, 0)

    def test_listbox_style_padding_controls_item_text(self):
        """ListBox item text uses explicit style padding when provided."""
        lb = ListBox(name="test", options=["Alpha"], selected_items=["Alpha"], width=9, height=1)
        lb.style.set("padding", "0, 2, 0, 2")
        rendered = ANSI_RE.sub("", lb.render()[0])
        self.assertEqual(rendered, "  Alpha  ")


class TestStructuredCellRendering(unittest.TestCase):
    """Direct structured-cell rendering tests for shared element paths."""

    def test_scrollview_render_cells_clips_colored_child_row(self):
        child = Label(name="child", text="ABCDE", width=5)
        child.style.set("color", "#abcdef")
        child.style.set("background", "#123456")
        scroll = FixedViewportScrollView(3, 1, [child])

        row = scroll.render_cells()[0]

        self.assertEqual("".join(cell.text for cell in row), "ABC")
        self.assertEqual(row[0].foreground, "171205239")
        self.assertEqual(row[0].background, "018052086")
        self.assertEqual(row[2].foreground, "171205239")

    def test_transparent_cell_background_preserves_parent_background(self):
        window = UIWindow()
        window.resize(2, 1)
        window.style.background = Color("#123456")

        row = window._render_cell_frame({
            0: [(0, [TerminalCell(" ", None, Color("transparent"))])]
        })[0]

        self.assertEqual(row[0].background, "018052086")

    def test_framebuffer_view_render_cells_pan_and_focus_edit_markers(self):
        view = FrameBufferView(name="frame", width=4, height=2)
        view.focus_style = Style()
        view.focus_style.set("color", "#facc15")
        view.edit_style = Style()
        view.edit_style.set("color", "#22c55e")
        view.set_frames({
            "app": {
                "cells": [
                    [["a", "#010203", "#040506"], ["b", "#070809", "#0a0b0c"], ["c", "#0d0e0f", "#101112"]],
                    [["d", "#131415", "#161718"], ["e", "#191a1b", "#1c1d1e"], ["f", "#1f2021", "#222324"]],
                ],
            },
        }, target_order=["app"], regions={"app": {"row": 0, "col": 0, "width": 4, "height": 2}})
        view.set_pan(col=1)

        cells = view.render_cells()
        self.assertEqual(cells[0][0].text, "b")
        self.assertEqual(cells[0][0].foreground, Color("#070809"))
        self.assertEqual(cells[0][0].background, Color("#0a0b0c"))

        view.set_interaction_state(focused=True)
        focused = view.render_cells()
        self.assertEqual(focused[0][0].text, "┌")
        self.assertEqual(focused[0][0].foreground, Color("#facc15"))

        view.set_interaction_state(focused=True, edit=True)
        editing = view.render_cells()
        self.assertEqual(editing[0][0].text, "┌")
        self.assertEqual(editing[0][0].foreground, Color("#22c55e"))


if __name__ == "__main__":
    unittest.main()
