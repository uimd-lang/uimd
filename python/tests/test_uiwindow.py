"""Tests for UIWindow class."""

import sys
import os
import re
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

import unittest
from unittest.mock import patch
import runtime.elements as elements_module
from runtime import UIApplication
from runtime.UIBase import DIALOG_BUTTON_CLOSE_DELAY_SECONDS
from runtime.uiwindow import UIWindow
from runtime.elements import Label, TextInput, NumberInput, Button

ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")


class TestUIWindow(unittest.TestCase):
    """Test cases for the UIWindow class."""

    def test_window_default_title(self):
        """Test window with default title."""
        w = UIWindow()
        self.assertEqual(w.title, "")

    def test_window_set_title(self):
        """Test setting window title."""
        w = UIWindow(title="My Window")
        self.assertEqual(w.title, "My Window")

    def test_window_create_label(self):
        """Test creating a label element."""
        w = UIWindow(title="Test")
        elem = w.create_element("my_label", "label", text="Hello")
        self.assertIsInstance(elem, Label)
        self.assertEqual(elem.text, "Hello")

    def test_window_create_textinput(self):
        """Test creating a text input element."""
        w = UIWindow(title="Test")
        elem = w.create_element("my_input", "textinput", value="World")
        self.assertIsInstance(elem, TextInput)
        self.assertEqual(elem.value, "World")

    def test_window_create_button(self):
        """Test creating a button element."""
        w = UIWindow(title="Test")
        elem = w.create_element("my_button", "button", title="Click")
        self.assertIsInstance(elem, Button)
        self.assertEqual(elem.title, "Click")

    def test_dialog_button_activation_delays_action_for_visible_focus(self):
        app = UIApplication(width=20, height=5)
        main = UIWindow(title="Main")
        dialog = UIWindow(title="Dialog")
        button = dialog.create_element("ok", "button", title="OK")
        calls = []

        def elementchanged(element, value):
            calls.append((element.name, value))

        dialog.elementchanged = elementchanged
        app.open(main)
        app.open(dialog)
        dialog.set_focus(button)

        with patch("runtime.UIBase.time.sleep") as sleep:
            dialog.handle_key("Enter")

        sleep.assert_called_once_with(DIALOG_BUTTON_CLOSE_DELAY_SECONDS)
        self.assertEqual(calls, [("ok", True)])

    def test_numberinput_enter_edit_selects_zero_for_replacement(self):
        """Entering edit mode on zero should let the first digit replace it."""
        w = UIWindow(title="Test")
        elem = w.create_element("count", "numberinput", value=0)
        self.assertIsInstance(elem, NumberInput)

        w.set_focus(elem)
        w.handle_key("Enter")
        w.handle_key("1")

        self.assertEqual(elem.display_value, "1")
        self.assertEqual(elem.cursor_pos, 1)
        self.assertIsNone(elem._selection_start)

    def test_numberinput_enter_edit_appends_after_nonzero_value(self):
        """Entering edit mode on a nonzero value should place cursor after the number."""
        w = UIWindow(title="Test")
        elem = w.create_element("count", "numberinput", value=1)
        self.assertIsInstance(elem, NumberInput)

        w.set_focus(elem)
        w.handle_key("Enter")
        w.handle_key("2")

        self.assertEqual(elem.display_value, "12")
        self.assertEqual(elem.cursor_pos, 2)
        self.assertIsNone(elem._selection_start)

    def test_numberinput_mouse_edit_selects_zero_for_replacement(self):
        """Clicking a zero number input should let the first digit replace it."""
        w = UIWindow(title="Test")
        elem = w.create_element("count", "numberinput", value=0)
        elem.width = 4

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 0})
        w.handle_key("1")

        self.assertEqual(elem.display_value, "1")
        self.assertEqual(elem.cursor_pos, 1)
        self.assertIsNone(elem._selection_start)

    def test_numberinput_mouse_edit_appends_after_nonzero_value(self):
        """Clicking a nonzero number input should place cursor after the number."""
        w = UIWindow(title="Test")
        elem = w.create_element("count", "numberinput", value=1)
        elem.width = 4

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 0})
        w.handle_key("2")

        self.assertEqual(elem.display_value, "12")
        self.assertEqual(elem.cursor_pos, 2)
        self.assertIsNone(elem._selection_start)

    def test_window_get_element(self):
        """Test getting element by name."""
        w = UIWindow(title="Test")
        w.create_element("my_label", "label", text="Hello")
        elem = w.get_element("my_label")
        self.assertIsNotNone(elem)
        self.assertEqual(elem.text, "Hello")

    def test_window_get_nonexistent_element(self):
        """Test getting non-existent element."""
        w = UIWindow(title="Test")
        elem = w.get_element("nonexistent")
        self.assertIsNone(elem)

    def test_window_get_all_elements(self):
        """Test getting all elements."""
        w = UIWindow(title="Test")
        w.create_element("label1", "label", text="A")
        w.create_element("input1", "textinput", value="B")
        elements = w.get_all_elements()
        self.assertEqual(len(elements), 2)

    def test_window_get_focusable_elements(self):
        """Test getting focusable elements (excludes labels)."""
        w = UIWindow(title="Test")
        w.create_element("label1", "label", text="A")
        w.create_element("input1", "textinput", value="B")
        w.create_element("button1", "button", title="C")
        focusable = w.get_focusable_elements()
        self.assertEqual(len(focusable), 2)

    def test_window_set_focus(self):
        """Test setting focus on an element."""
        w = UIWindow(title="Test")
        btn = w.create_element("btn", "button", title="OK")
        w.set_focus(btn)
        self.assertTrue(btn.focused)

    def test_window_clear_focus(self):
        """Test clearing focus."""
        w = UIWindow(title="Test")
        btn = w.create_element("btn", "button", title="OK")
        w.set_focus(btn)
        w.set_focus(None)
        self.assertFalse(btn.focused)

    def test_window_next_focus(self):
        """Test moving focus to next element."""
        w = UIWindow(title="Test")
        btn1 = w.create_element("btn1", "button", title="A")
        btn2 = w.create_element("btn2", "button", title="B")
        w.set_focus(btn1)
        w.next_focus()
        self.assertTrue(btn2.focused)

    def test_window_prev_focus(self):
        """Test moving focus to previous element."""
        w = UIWindow(title="Test")
        btn1 = w.create_element("btn1", "button", title="A")
        btn2 = w.create_element("btn2", "button", title="B")
        w.set_focus(btn1)
        w.prev_focus()
        self.assertTrue(btn2.focused)

    def test_window_handle_tab(self):
        """Test Tab key moves focus."""
        w = UIWindow(title="Test")
        btn1 = w.create_element("btn1", "button", title="A")
        btn2 = w.create_element("btn2", "button", title="B")
        w.set_focus(btn1)
        w.handle_key("Tab")
        self.assertTrue(btn2.focused)

    def test_edit_enter_commits_and_escape_rolls_back(self):
        """Enter should commit edit changes; Escape should restore the snapshot."""
        w = UIWindow(title="Test")
        inp = w.create_element("inp", "textinput", value="A")
        w.set_focus(inp)

        w.handle_key("Enter")
        inp.handle_key("B")
        w.handle_key("Escape")
        self.assertEqual(inp.value, "A")
        self.assertFalse(w._edit_mode)

        w.handle_key("Enter")
        inp.handle_key("C")
        w.handle_key("Enter")
        self.assertEqual(inp.value, "AC")
        self.assertFalse(w._edit_mode)

    def test_mouse_click_button_activates_it(self):
        """Clicking a button should fire elementchanged."""
        called = []

        class MyWindow(UIWindow):
            def elementchanged(self, element, value):
                called.append((element.name, value))

        w = MyWindow(title="Test")
        btn = w.create_element("btn", "button", title="OK")
        btn.width = 4

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})

        self.assertEqual(called, [("btn", True)])

    def test_mouse_click_image_activates_on_release(self):
        """Clicking an image should fire only after a matching mouse release."""
        called = []

        class MyWindow(UIWindow):
            def elementchanged(self, element, value):
                called.append((element.name, value))

        w = MyWindow(title="Test")
        image = w.create_element("photo", "image", source="")
        image.width = 4
        image.height = 2

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        self.assertEqual(called, [])

        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 0, "col": 1})
        self.assertEqual(called, [("photo", True)])

    def test_mouse_drag_from_image_does_not_activate_click(self):
        """Dragging after pressing an image should cancel image click activation."""
        called = []

        class MyWindow(UIWindow):
            def elementchanged(self, element, value):
                called.append((element.name, value))

        w = MyWindow(title="Test")
        image = w.create_element("photo", "image", source="")
        image.width = 4
        image.height = 2

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": 0, "col": 2})
        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 0, "col": 2})

        self.assertEqual(called, [])

    def test_mouse_click_checkbox_toggles_it(self):
        """Clicking a checkbox should toggle its value."""
        w = UIWindow(title="Test")
        checkbox = w.create_element("cb", "checkbox", title="Accept")
        checkbox.width = 10

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})

        self.assertTrue(checkbox.value)

    def test_mouse_click_textinput_sets_cursor_and_drag_selects(self):
        """Mouse click and drag should position/select text in text inputs."""
        w = UIWindow(title="Test")
        inp = w.create_element("inp", "textinput", value="Hello")
        inp.width = 10

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        self.assertTrue(w._edit_mode)
        self.assertEqual(inp.cursor_pos, 1)

        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": 0, "col": 4})
        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 0, "col": 4})
        inp.cursor_pos = len(inp.value)
        inp._selection_start = None
        inp.handle_key("cmd_v")
        self.assertEqual(inp.value, "Helloell")

    def test_mouse_text_selection_notifies_copy(self):
        """Auto-copy selection should show a copy notification when an app is attached."""
        from runtime.application import UIApplication

        app = UIApplication(width=40, height=6)
        w = UIWindow(title="Test")
        inp = w.create_element("inp", "textinput", value="Hello")
        inp.width = 10
        app.open(w)

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": 0, "col": 4})

        self.assertFalse(any("Copied to clipboard" in line for line in app.render()))

        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 0, "col": 4})

        self.assertTrue(any("Copied to clipboard" in line for line in app.render()))

    def test_mouse_click_zero_numberinput_does_not_notify_copy(self):
        """Automatic zero selection should not be treated as a copied mouse selection."""
        from runtime.application import UIApplication

        app = UIApplication(width=20, height=4)
        w = UIWindow(title="Test")
        inp = w.create_element("count", "numberinput", value=0)
        inp.width = 4
        app.open(w)

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 0})
        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 0, "col": 0})

        self.assertFalse(any("Copied to clipboard" in line for line in app.render()))

    def test_mouse_click_combobox_item_selects_it(self):
        """Clicking an open combobox item should select that item."""
        w = UIWindow(title="Test")
        combo = w.create_element("country", "combobox", options=["SK", "CZ", "PL"], selected_item="SK")
        combo.width = 10

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        self.assertTrue(combo._menu_open)
        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 2, "col": 1})

        self.assertEqual(combo.selected_item, "CZ")

    def test_closed_combobox_only_hit_tests_closed_row(self):
        """A closed combobox should not treat layout rows below it as menu options."""
        w = UIWindow(title="Test")
        combo = w.create_element("country", "combobox", options=["SK", "CZ", "PL"], selected_item="SK")
        combo.width = 10
        combo.height = 4

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 2, "col": 1})

        self.assertEqual(combo.selected_item, "SK")
        self.assertFalse(combo._menu_open)

    def test_open_combobox_overlay_wins_over_listbox_underneath(self):
        """Combobox dropdown clicks should not leak to overlapping listboxes."""
        w = UIWindow(title="Test")
        combo = w.create_element("country", "combobox", options=["SK", "CZ", "PL"], selected_item="SK")
        combo.width = 10
        listbox = w.create_element("role", "listbox", options=["A", "B", "C"], selected_items=["A"])
        listbox.row = 1
        listbox.width = 10
        listbox.height = 3

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        self.assertTrue(combo._menu_open)
        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 2, "col": 1})

        self.assertEqual(combo.selected_item, "CZ")
        self.assertEqual(listbox.selected_items, ["A"])

    def test_open_combobox_renders_above_later_overlapping_elements(self):
        """Open combobox rows should paint over controls that start farther right."""
        w = UIWindow(title="Test")
        combo = w.create_element("country", "combobox", options=["SK", "CZ", "PL"], selected_item="SK")
        combo.width = 10
        button = w.create_element("action", "button", title="BUTTON")
        button.row = 2
        button.col = 4
        button.width = 10

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        row = ANSI_RE.sub("", w.render()[2])

        self.assertEqual(row[:10], "  CZ      ")

    def test_open_combobox_uses_global_overlay_beyond_window_height(self):
        """A dropdown near the bottom should render globally without resizing the window."""
        app = UIApplication(width=12, height=3)
        w = UIWindow(title="Test")
        combo = w.create_element("country", "combobox", options=["SK", "CZ", "PL"], selected_item="SK")
        combo.row = 2
        combo.width = 10
        app.open(w)

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 2, "col": 1})
        rendered = [ANSI_RE.sub("", line) for line in w.render()]
        overlay = ANSI_RE.sub("", app._build_combobox_overlay())

        self.assertEqual(len(rendered), 3)
        self.assertIn("  PL", overlay)

    def test_mouse_click_listbox_item_selects_it(self):
        """Clicking a listbox row should select the clicked item."""
        w = UIWindow(title="Test")
        listbox = w.create_element("role", "listbox", options=["A", "B", "C"], selected_items=["A"])
        listbox.width = 10
        listbox.height = 3

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 2, "col": 1})

        self.assertEqual(listbox.selected_items, ["C"])

    def test_mouse_double_click_listbox_item_confirms_it(self):
        """Double-clicking a listbox row should confirm the selected item."""
        events = []

        class TestWindow(UIWindow):
            def confirmed(self, element, value):
                events.append(element.selected_item)

        w = TestWindow(title="Test")
        listbox = w.create_element("role", "listbox", options=["A", "B", "C"], selected_items=["A"])
        listbox.width = 10
        listbox.height = 3

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 1, "col": 1, "time": 1.0})
        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 1, "col": 1, "time": 1.1})

        self.assertEqual(listbox.selected_items, ["B"])
        self.assertEqual(events, ["B"])

    def test_mouse_click_button_after_listbox_focus_does_not_confirm_listbox(self):
        """Moving focus from a listbox to a button should not confirm the listbox."""
        events = []

        class TestWindow(UIWindow):
            def elementchanged(self, element, value):
                events.append(("changed", element.name))

            def confirmed(self, element, value):
                events.append(("confirmed", element.name))

        w = TestWindow(title="Test")
        listbox = w.create_element("items", "listbox", options=["A", "B"], selected_items=["A"])
        button = w.create_element("open_btn", "button", title="Open")
        listbox.width = 10
        listbox.height = 2
        button.row = 3
        button.width = 10
        w.set_focus(listbox)
        w._enter_edit_mode()

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 3, "col": 1})

        self.assertEqual(events, [("changed", "open_btn")])

    def test_textinput_key_change_notifies_elementchanged(self):
        """Text input edits should notify the owning window immediately."""
        events = []

        class TestWindow(UIWindow):
            def elementchanged(self, element, value):
                events.append((element.name, value))

        w = TestWindow(title="Test")
        editor = w.create_element("editor", "textinput", value="")
        editor.width = 10
        w.set_focus(editor)
        w._enter_edit_mode()

        w.handle_key("x")

        self.assertEqual(events, [("editor", "x")])

    def test_mouse_wheel_over_listbox_scrolls_it(self):
        """Wheel input should dispatch to the listbox under the cursor."""
        w = UIWindow(title="Test")
        listbox = w.create_element("role", "listbox", options=["A", "B", "C", "D"], selected_items=["A"])
        listbox.width = 10
        listbox.height = 2

        handled = w.handle_key({"type": "mouse_wheel", "direction": "down", "row": 1, "col": 1})

        self.assertTrue(handled)
        self.assertEqual(listbox._scroll_offset, 1)
        self.assertEqual(listbox.selected_items, ["A"])

    def test_mouse_wheel_over_textarea_scrolls_it(self):
        """Wheel input should dispatch to a multiline text area under the cursor."""
        w = UIWindow(title="Test")
        editor = w.create_element("editor", "textarea", value="one\ntwo\nthree")
        editor.width = 10
        editor.height = 2

        handled = w.handle_key({"type": "mouse_wheel", "direction": "down", "row": 1, "col": 1})

        self.assertTrue(handled)
        self.assertEqual(editor._scroll_offset, 1)

    def test_mouse_wheel_combobox_overlay_does_not_scroll_listbox_underneath(self):
        """Wheel input on an open combobox overlay should not leak to a listbox below it."""
        w = UIWindow(title="Test")
        combo = w.create_element("country", "combobox", options=["SK", "CZ", "PL"], selected_item="SK")
        combo.width = 10
        listbox = w.create_element("role", "listbox", options=["A", "B", "C", "D"], selected_items=["A"])
        listbox.row = 1
        listbox.width = 10
        listbox.height = 2

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        handled = w.handle_key({"type": "mouse_wheel", "direction": "down", "row": 2, "col": 1})

        self.assertFalse(handled)
        self.assertEqual(listbox._scroll_offset, 0)

    def test_window_arrow_keys_move_by_visual_geometry(self):
        """Arrow keys should follow on-screen element positions, not tab order."""
        w = UIWindow(title="Test")
        buttons = {}
        positions = {
            "n7": (0, 0), "n8": (0, 4), "n9": (0, 8),
            "n4": (2, 0), "n5": (2, 4), "n6": (2, 8),
            "n1": (4, 0), "n2": (4, 4), "n3": (4, 8),
        }
        for name, (row, col) in positions.items():
            button = w.create_element(name, "button", title=name[-1])
            button.row = row
            button.col = col
            button.width = 3
            buttons[name] = button

        w.set_focus(buttons["n5"])
        w.handle_key("Up")
        self.assertTrue(buttons["n8"].focused)

        w.set_focus(buttons["n5"])
        w.handle_key("Down")
        self.assertTrue(buttons["n2"].focused)

        w.set_focus(buttons["n5"])
        w.handle_key("Left")
        self.assertTrue(buttons["n4"].focused)

        w.set_focus(buttons["n5"])
        w.handle_key("Right")
        self.assertTrue(buttons["n6"].focused)

    def test_window_render_empty(self):
        """Test rendering an empty window."""
        w = UIWindow(title="Test")
        lines = w.render()
        self.assertEqual(len(lines), 24)  # Default height

    def test_window_mode_property(self):
        """Test window mode property."""
        w = UIWindow(title="Test")
        self.assertEqual(w.mode, "normal")

    def test_window_mode_dialog(self):
        """Test setting dialog mode."""
        w = UIWindow(title="Test")
        w.mode = "dialog"
        self.assertEqual(w.mode, "dialog")

    def test_window_mode_fullscreen(self):
        """Test setting fullscreen mode."""
        w = UIWindow(title="Test")
        w.mode = "fullscreen"
        self.assertEqual(w.mode, "fullscreen")

    def test_cell_background_texture_fills_empty_cell_space(self):
        """Cell textures should fill empty space in row-major order."""
        layout = [
            {
                "name": "missing",
                "row": 0,
                "col": 0,
                "cell_row": 0,
                "cell_col": 0,
                "cell_chars_width": 4,
                "cell_chars_height": 2,
                "cell_name": "panel",
                "cell_width": 4,
                "cell_height": 2,
                "cell_width_mode": "auto",
                "cell_height_mode": "auto",
                "chars_width": 4,
                "chars_height": 2,
                "width": 4,
                "height": 2,
                "type": "label",
            },
        ]
        w = UIWindow(
            layout=layout,
            style={
                "this": {"border-width": "0"},
                "@panel": {
                    "background-texture": "abc",
                    "background-texture-color": "#123456",
                },
            },
        )

        w.open()
        plain = [ANSI_RE.sub("", line) for line in w.render()]

        self.assertEqual(plain[0][:4], "abca")
        self.assertEqual(plain[1][:4], "bcab")
        self.assertEqual(w.styles[0][0]["color"].hex, "#123456")

    def test_element_background_texture_fills_empty_element_space(self):
        """Element textures should sit behind text and use color fallback."""
        layout = [
            {
                "name": "label",
                "row": 0,
                "col": 0,
                "cell_row": 0,
                "cell_col": 0,
                "cell_chars_width": 6,
                "cell_chars_height": 2,
                "cell_name": "panel",
                "cell_width": 6,
                "cell_height": 2,
                "cell_width_mode": "auto",
                "cell_height_mode": "auto",
                "chars_width": 6,
                "chars_height": 2,
                "width": 6,
                "height": 2,
                "type": "label",
            },
        ]
        w = UIWindow(
            layout=layout,
            style={
                "this": {"border-width": "0"},
                "@label": {
                    "color": "#abcdef",
                    "background": "#222222",
                    "background-texture": ".,",
                },
            },
        )
        w.create_element("label", "label", text="X")

        w.open()
        plain = [ANSI_RE.sub("", line) for line in w.render()]

        self.assertEqual(plain[0][:6], "X,.,.,")
        self.assertEqual(plain[1][:6], ".,.,.,")
        self.assertEqual(w.styles[1][0]["color"].hex, "#abcdef")
        self.assertEqual(w.styles[0][0]["background"], "034034034")

    def test_copyable_label_drag_copies_selected_text_only(self):
        """Labels opt into mouse selection with user-select: text."""
        elements_module._TEXT_CLIPBOARD = ""
        w = UIWindow(
            title="Test",
            style={
                "label": {
                    "user-select": "text",
                    "cursor-background": "#b8d7ff",
                    "cursor-color": "#000000",
                    "background-texture": ".",
                },
            },
        )
        label = w.create_element("copy", "label", text="Hello")
        label.width = 8

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 0})
        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": 0, "col": 5})
        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 0, "col": 5})

        self.assertEqual(elements_module._TEXT_CLIPBOARD, "Hello")

    def test_non_copyable_label_does_not_start_text_selection(self):
        """Labels must opt in before mouse selection copies them."""
        elements_module._TEXT_CLIPBOARD = ""
        w = UIWindow(title="Test")
        label = w.create_element("copy", "label", text="Hello")
        label.width = 8

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 0})
        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": 0, "col": 5})
        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 0, "col": 5})

        self.assertEqual(elements_module._TEXT_CLIPBOARD, "")

    def test_copyable_label_selection_across_elements_joins_with_newlines(self):
        """Copying across multiple labels should join selected label text by newline."""
        elements_module._TEXT_CLIPBOARD = ""
        w = UIWindow(
            title="Test",
            style={
                "label": {
                    "user-select": "text",
                    "cursor-background": "#b8d7ff",
                    "cursor-color": "#000000",
                },
            },
        )
        user = w.create_element("user", "label", text="user")
        user.width = 8
        text = w.create_element("text", "label", text="hello")
        text.row = 1
        text.width = 8

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": 1, "col": 3})
        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 1, "col": 3})

        self.assertEqual(elements_module._TEXT_CLIPBOARD, "ser\nhel")

    def test_copyable_label_selection_uses_cursor_style(self):
        """Label selection should render with cursor-background and cursor-color."""
        w = UIWindow(
            title="Test",
            style={
                "label": {
                    "user-select": "text",
                    "cursor-background": "#b8d7ff",
                    "cursor-color": "#000000",
                },
            },
        )
        label = w.create_element("copy", "label", text="Hello")
        label.width = 8

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": 0, "col": 4})
        w.render()

        self.assertEqual(w.styles[0][1]["color"], "000000000")
        self.assertEqual(w.styles[0][1]["background"], "184215255")

    def test_copyable_label_drag_above_and_below_clamps_to_text_edges(self):
        """Dragging outside a multi-line label should select from the text edge."""
        elements_module._TEXT_CLIPBOARD = ""
        w = UIWindow(
            title="Test",
            style={
                "label": {
                    "user-select": "text",
                    "cursor-background": "#b8d7ff",
                    "cursor-color": "#000000",
                },
            },
        )
        label = w.create_element("copy", "label", text="aa\nbb\ncc")
        label.width = 2
        label.height = 3

        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 2, "col": 1})
        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": -1, "col": 1})
        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": -1, "col": 1})

        self.assertEqual(elements_module._TEXT_CLIPBOARD, "aa\nbb\nc")

        elements_module._TEXT_CLIPBOARD = ""
        w.handle_key({"type": "mouse", "event": "press", "button": 0, "row": 0, "col": 1})
        w.handle_key({"type": "mouse", "event": "drag", "button": 0, "row": 4, "col": 1})
        w.handle_key({"type": "mouse", "event": "release", "button": 0, "row": 4, "col": 1})

        self.assertEqual(elements_module._TEXT_CLIPBOARD, "a\nbb\ncc")


class TestUIWindowLifecycle(unittest.TestCase):
    """Test window lifecycle methods."""

    def test_window_open(self):
        """Test calling open on window."""
        w = UIWindow(title="Test")
        called = []

        class MyWindow(UIWindow):
            def open(self):
                called.append(True)

        w = MyWindow(title="Test")
        w.open()
        self.assertEqual(called, [True])

    def test_window_close(self):
        """Test calling close on window."""
        w = UIWindow(title="Test")
        called = []

        class MyWindow(UIWindow):
            def close(self):
                called.append(True)

        w = MyWindow(title="Test")
        w.close()
        self.assertEqual(called, [True])

    def test_window_save(self):
        """Test calling save on window."""
        w = UIWindow(title="Test")
        called = []

        class MyWindow(UIWindow):
            def save(self):
                called.append(True)

        w = MyWindow(title="Test")
        w.save()
        self.assertEqual(called, [True])


if __name__ == "__main__":
    unittest.main()
