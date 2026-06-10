"""Simple local text editor example."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(_here)))

from runtime import UIApplication
from dialogs import FileBrowser, MessageBoxYesNoCancel
from text_editor_ui import TextEditorUI


NOTES_DIR_ENV = "UI_TEXT_EDITOR_NOTES_DIR"
DEFAULT_NOTE_ENV = "UI_TEXT_EDITOR_DEFAULT_NOTE"
BROWSER_ROOT_DIR_ENV = "UI_TEXT_EDITOR_BROWSER_ROOT_DIR"
DEFAULT_BROWSER_DIR_ENV = "UI_TEXT_EDITOR_BROWSER_DIR"
NOTES_DIR = os.environ.get(NOTES_DIR_ENV, os.path.join(_here, "notes"))
DEFAULT_NOTE = os.environ.get(DEFAULT_NOTE_ENV, "welcome.txt")
UNTITLED_NAME = "untitled.txt"
USER_HOME_DIR = os.path.expanduser("~")
DESKTOP_DIR = os.path.join(USER_HOME_DIR, "Desktop")
BROWSER_ROOT_DIR = os.environ.get(BROWSER_ROOT_DIR_ENV, USER_HOME_DIR)
DEFAULT_BROWSER_DIR = os.environ.get(
    DEFAULT_BROWSER_DIR_ENV,
    DESKTOP_DIR if os.path.isdir(DESKTOP_DIR) else USER_HOME_DIR,
)


class TextEditor(TextEditorUI):
    def open(self):
        super().open()
        self._loaded_text = ""
        self._current_path = os.path.join(NOTES_DIR, DEFAULT_NOTE)
        self._pending_action = None
        self._after_save_action = None
        self._load_path(self._current_path)

    def on_new_btn_click(self):
        self._with_saved_changes(self._new_file)

    def on_open_btn_click(self):
        self._with_saved_changes(self._open_browser)

    def on_save_btn_click(self):
        self._save_or_save_as()

    def on_save_as_btn_click(self):
        self._save_as_browser()

    def on_quit_btn_click(self):
        self._with_saved_changes(self._quit)

    def on_editor_change(self, value):
        self._refresh_status("Modified")

    def _new_file(self):
        self._exit_edit_mode(commit=True, notify=False)
        self._current_path = None
        self.filename.text = UNTITLED_NAME
        self.editor.value = ""
        self.editor.cursor_pos = 0
        self.editor._scroll_offset = 0
        self._loaded_text = ""
        self._refresh_status("New file")

    def _load_path(self, path):
        self._exit_edit_mode(commit=True, notify=False)
        path = os.path.abspath(path)
        try:
            with open(path, "r", encoding="utf-8") as handle:
                text = handle.read()
        except OSError:
            text = ""
        self._current_path = path
        self.filename.text = os.path.basename(path)
        self.editor.value = text
        self.editor.cursor_pos = 0
        self.editor._scroll_offset = 0
        self._loaded_text = text
        self._refresh_status("Loaded")

    def _save_or_save_as(self):
        if self._current_path is None:
            self._save_as_browser()
        else:
            self._save_path(self._current_path)

    def _save_path(self, path):
        path = os.path.abspath(path)
        with open(path, "w", encoding="utf-8") as handle:
            handle.write(self.editor.value)
        self._current_path = path
        self.filename.text = os.path.basename(path)
        self._loaded_text = self.editor.value
        self._refresh_status("Saved")

    def _open_browser(self):
        browser = FileBrowser(BROWSER_ROOT_DIR, DEFAULT_BROWSER_DIR, mode="open", on_close=self._on_open_selected)
        self._app.open(browser)

    def _save_as_browser(self):
        start = os.path.dirname(self._current_path) if self._current_path else NOTES_DIR
        if not os.path.abspath(start).startswith(os.path.abspath(BROWSER_ROOT_DIR)):
            start = DEFAULT_BROWSER_DIR
        initial_filename = os.path.basename(self._current_path) if self._current_path else UNTITLED_NAME
        browser = FileBrowser(
            BROWSER_ROOT_DIR,
            start,
            mode="save",
            on_close=self._on_save_selected,
            initial_filename=initial_filename,
        )
        self._app.open(browser)

    def _on_open_selected(self, path):
        if path:
            self._load_path(path)

    def _on_save_selected(self, path):
        action = self._after_save_action
        self._after_save_action = None
        if path:
            self._save_path(path)
            if action is not None:
                action()
        elif action is not None:
            self._refresh_status("Cancelled")

    def _with_saved_changes(self, action):
        self._after_save_action = None
        if not self._is_modified():
            action()
            return
        self._pending_action = action
        dialog = MessageBoxYesNoCancel(
            header="Unsaved Changes",
            message="Save current file before continuing?",
            on_close=self._on_unsaved_choice,
        )
        self._app.open(dialog)

    def _on_unsaved_choice(self, choice):
        action = self._pending_action
        self._pending_action = None
        if choice == "cancel" or action is None:
            self._refresh_status("Cancelled")
            return
        if choice == "yes":
            if self._current_path is None:
                self._after_save_action = action
                self._save_as_browser()
                return
            self._save_or_save_as()
            action()
            return
        if choice == "no":
            action()

    def _quit(self):
        if self._app is not None:
            self._app._running = False

    def _refresh_status(self, prefix):
        text = self.editor.value
        modified = "*" if self._is_modified() else ""
        line_count = text.count("\n") + 1 if text else 0
        self.status.text = f"{prefix}: {self.filename.text}{modified} - {line_count} lines, {len(text)} chars"

    def _is_modified(self):
        return self.editor.value != self._loaded_text


def main():
    app = UIApplication()
    window = TextEditor()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
