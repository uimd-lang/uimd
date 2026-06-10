"""Regression tests for the text editor example."""

import os
import re
import sys
import tempfile
import unittest


PROJECT_ROOT = os.path.join(os.path.dirname(__file__), "..")
TEXT_EDITOR_DIR = os.path.join(PROJECT_ROOT, "examples", "text_editor")
sys.path.insert(0, PROJECT_ROOT)
sys.path.insert(0, TEXT_EDITOR_DIR)

from runtime import UIApplication
from dialogs import FileBrowser
from text_editor import TextEditor

ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")


class TestTextEditor(unittest.TestCase):
    """Text editor example behavior."""

    def test_editor_geometry_is_stable_across_render_and_click(self):
        """Runtime placement should not accumulate padding on repeated renders."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)

        initial = self._editor_rect(window)
        if "width" in window.editor._layout_entry:
            self.assertEqual(window.editor.width, window.editor._layout_entry["width"])
        self.assertEqual(
            window.editor._cell_col + window.editor.col,
            window.new_btn._cell_col + window.new_btn.col,
        )
        self.assertEqual(
            window.editor._cell_col + window.editor.col + window.editor.width,
            window.editor._resolved_cell.col + window.editor._resolved_cell.width,
        )
        app.render()
        app.render()

        self.assertEqual(self._editor_rect(window), initial)

        row = window.editor._cell_row + window.editor.row
        col = window.editor._cell_col + window.editor.col
        app.handle_key({
            "type": "mouse",
            "event": "press",
            "button": 0,
            "row": row,
            "col": col,
        })
        app.render()

        self.assertEqual(self._editor_rect(window), initial)

    def test_loaded_file_path_displays_basename(self):
        """Displayed file path should be stable across backend directories."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)

        self.assertEqual(window.filename.text, os.path.basename(window._current_path))
        self.assertTrue(window.status.text.startswith(f"Loaded: {window.filename.text}"))

    def test_load_path_resets_editor_scroll_offset(self):
        """Loading a file should show the top of the new file."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)
        window.editor._scroll_offset = 10

        with tempfile.NamedTemporaryFile("w", delete=False, encoding="utf-8") as handle:
            handle.write("first\nsecond\n")
            path = handle.name
        try:
            window._load_path(path)
        finally:
            os.unlink(path)

        self.assertEqual(window.editor._scroll_offset, 0)
        self.assertTrue(ANSI_RE.sub("", window.editor.render()[0]).startswith("first"))

    def test_large_file_can_still_be_edited(self):
        """Text editor should not impose a fixed maxlength on loaded files."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)

        large_text = "x" * 33000
        with tempfile.NamedTemporaryFile("w", delete=False, encoding="utf-8") as handle:
            handle.write(large_text)
            path = handle.name
        try:
            window._load_path(path)
        finally:
            os.unlink(path)

        window.set_focus(window.editor)
        window._enter_edit_mode()
        window.handle_key("y")

        self.assertEqual(len(window.editor.value), len(large_text) + 1)
        self.assertTrue(window.editor.value.endswith("y"))

    def test_file_browser_open_button_after_list_selection_closes_once(self):
        """Clicking Open after selecting a list item should accept that file once."""
        with tempfile.TemporaryDirectory() as directory:
            path = os.path.join(directory, "temp.txt")
            with open(path, "w", encoding="utf-8") as handle:
                handle.write("content")
            picked = []
            browser = FileBrowser(directory, directory, mode="open", on_close=picked.append)
            app = UIApplication(width=80, height=24)
            app.open(browser)
            app.render()

            item_idx = browser.entries._options.index("temp.txt")
            app.handle_key({
                "type": "mouse",
                "event": "press",
                "button": 0,
                "row": browser.entries._cell_row + browser.entries.row + item_idx,
                "col": browser.entries._cell_col + browser.entries.col,
            })
            app.handle_key({
                "type": "mouse",
                "event": "press",
                "button": 0,
                "row": browser.open_btn._cell_row + browser.open_btn.row,
                "col": browser.open_btn._cell_col + browser.open_btn.col,
            })

            self.assertEqual(picked, [path])
            self.assertEqual(app.window_count, 0)

    def test_save_as_browser_keeps_save_mode_after_opening_window(self):
        """Save As should keep its own browser mode after runtime opens the dialog."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)

        window._dispatch_element_changed(window.save_as_btn, True)

        browser = app.active_window
        self.assertIsInstance(browser, FileBrowser)
        self.assertEqual(browser._browser_mode, "save")
        self.assertEqual(browser.dialog_header.text, "Save As")
        self.assertEqual(browser.open_btn.title, "Save")
        self.assertEqual(browser.filename.value, os.path.basename(window._current_path))

    def test_save_as_button_accepts_filename_without_navigating_to_parent(self):
        """Save button in save mode should use File field, not selected list entry."""
        with tempfile.TemporaryDirectory() as directory:
            picked = []
            browser = FileBrowser(
                directory,
                directory,
                mode="save",
                on_close=picked.append,
                initial_filename="new.txt",
            )
            app = UIApplication(width=80, height=24)
            app.open(browser)
            app.render()

            browser.elementchanged(browser.open_btn, True)

            self.assertEqual(picked, [os.path.join(directory, "new.txt")])
            self.assertEqual(browser._current_dir, directory)
            self.assertEqual(app.window_count, 0)

    def test_open_button_accepts_typed_filename_before_parent_selection(self):
        """Open mode should use the File field before the selected parent entry."""
        with tempfile.TemporaryDirectory() as directory:
            path = os.path.join(directory, "typed.txt")
            with open(path, "w", encoding="utf-8") as handle:
                handle.write("content")
            picked = []
            browser = FileBrowser(directory, directory, mode="open", on_close=picked.append)
            app = UIApplication(width=80, height=24)
            app.open(browser)
            app.render()

            self.assertEqual(browser.entries.selected_item, "..")
            browser.filename.value = "typed.txt"
            browser.elementchanged(browser.open_btn, True)

            self.assertEqual(picked, [path])
            self.assertEqual(browser._current_dir, directory)
            self.assertEqual(app.window_count, 0)

    def test_new_unsaved_no_discards_current_text(self):
        """Choosing No for unsaved changes before New should create an empty untitled file."""
        with tempfile.NamedTemporaryFile("w", delete=False, encoding="utf-8") as handle:
            handle.write("original")
            path = handle.name
        try:
            app = UIApplication(width=100, height=30)
            window = TextEditor()
            app.open(window)
            window._load_path(path)
            window.editor.value = "changed"

            window._dispatch_element_changed(window.new_btn, True)
            dialog = app.active_window
            dialog.elementchanged(dialog.no_btn, True)

            self.assertEqual(window.editor.value, "")
            self.assertEqual(window._loaded_text, "")
            self.assertIsNone(window._current_path)
            self.assertEqual(window.filename.text, "untitled.txt")
            self.assertEqual(app.active_window, window)
        finally:
            os.unlink(path)

    def test_new_unsaved_no_via_focused_enter_discards_current_text(self):
        """Focused No + Enter in the unsaved prompt should run New, not the default Cancel."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)
        window.editor.value = "changed"

        window._dispatch_element_changed(window.new_btn, True)
        dialog = app.active_window
        dialog.set_focus(dialog.no_btn)
        app.handle_key("Enter")

        self.assertEqual(window.editor.value, "")
        self.assertEqual(window._loaded_text, "")
        self.assertIsNone(window._current_path)
        self.assertEqual(window.filename.text, "untitled.txt")
        self.assertEqual(app.active_window, window)

    def test_new_unsaved_cancel_keeps_current_text(self):
        """Choosing Cancel for unsaved changes before New should keep the dirty document."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)
        original_path = window._current_path
        original_loaded = window._loaded_text
        window.editor.value = "changed"

        window._dispatch_element_changed(window.new_btn, True)
        dialog = app.active_window
        dialog.elementchanged(dialog.cancel_btn, True)

        self.assertEqual(window.editor.value, "changed")
        self.assertEqual(window._loaded_text, original_loaded)
        self.assertEqual(window._current_path, original_path)
        self.assertEqual(app.active_window, window)

    def test_new_unsaved_yes_saves_then_creates_empty_file(self):
        """Choosing Yes for unsaved changes before New should save then create a new file."""
        with tempfile.NamedTemporaryFile("w", delete=False, encoding="utf-8") as handle:
            handle.write("original")
            path = handle.name
        try:
            app = UIApplication(width=100, height=30)
            window = TextEditor()
            app.open(window)
            window._load_path(path)
            window.editor.value = "changed"

            window._dispatch_element_changed(window.new_btn, True)
            dialog = app.active_window
            dialog.elementchanged(dialog.yes_btn, True)

            with open(path, "r", encoding="utf-8") as handle:
                self.assertEqual(handle.read(), "changed")
            self.assertEqual(window.editor.value, "")
            self.assertEqual(window._loaded_text, "")
            self.assertIsNone(window._current_path)
            self.assertEqual(window.filename.text, "untitled.txt")
            self.assertEqual(app.active_window, window)
        finally:
            os.unlink(path)

    def test_new_unsaved_yes_via_focused_enter_opens_save_as_for_untitled_file(self):
        """Focused Yes + Enter should open Save As when the dirty document has no path."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)
        window._new_file()
        window.editor.value = "changed"

        window._dispatch_element_changed(window.new_btn, True)
        dialog = app.active_window
        dialog.set_focus(dialog.yes_btn)
        app.handle_key("Enter")

        self.assertIsInstance(app.active_window, FileBrowser)
        self.assertEqual(app.active_window._browser_mode, "save")
        self.assertEqual(window.editor.value, "changed")
        self.assertEqual(window._after_save_action.__name__, "_new_file")

    def test_cancelled_save_as_clears_pending_after_save_action(self):
        """Cancelling Save As during an unsaved prompt should not leave a stale pending action."""
        app = UIApplication(width=100, height=30)
        window = TextEditor()
        app.open(window)
        window._new_file()
        window.editor.value = "changed"

        window._with_saved_changes(window._new_file)
        dialog = app.active_window
        dialog.elementchanged(dialog.yes_btn, True)
        self.assertIsInstance(app.active_window, FileBrowser)

        window._on_save_selected(None)

        self.assertIsNone(window._after_save_action)
        self.assertEqual(window.editor.value, "changed")

    @staticmethod
    def _editor_rect(window):
        editor = window.editor
        return (
            editor.row,
            editor.col,
            editor.width,
            editor.height,
        )


if __name__ == "__main__":
    unittest.main()
