"""Standard reusable file browser dialog."""

import os
import re
import time

try:
    from uimd.dialogs.file_browser_ui import FileBrowserUI
    from uimd.dialogs.message_box_yes_no import MessageBoxYesNo
except ImportError:
    from file_browser_ui import FileBrowserUI
    from message_box_yes_no import MessageBoxYesNo


PARENT_ENTRY = ".."
FILE_BROWSER_ESCAPE_FLASH_SECONDS = 0.18


class FileBrowser(FileBrowserUI):
    _handles_escape = True

    def __init__(self, root_dir, start_path=None, mode="open", on_close=None, initial_filename=None, extension_filter=None):
        super().__init__()
        self._root_dir = os.path.abspath(root_dir)
        requested_start = os.path.abspath(start_path or root_dir)
        if os.path.isfile(requested_start):
            self._current_dir = os.path.dirname(requested_start)
            if initial_filename is None:
                initial_filename = os.path.basename(requested_start)
        else:
            self._current_dir = requested_start
        self._browser_mode = mode
        self._initial_filename = initial_filename or ""
        self._on_close = on_close
        self._extension_filter = str(extension_filter or "").strip()

    def open(self):
        super().open()
        self.dialog_header.text = "Save As" if self._browser_mode == "save" else "Open File"
        self.open_btn.title = "Save" if self._browser_mode == "save" else "Open"
        self._current_dir = self._clamp_dir(self._current_dir)
        self._refresh_entries()
        if self._browser_mode == "save":
            self.filename.value = self._initial_filename
            self._update_open_enabled()
        self.set_focus(self.entries)
        self._enter_edit_mode()

    def handle_key(self, key):
        if key == "Escape":
            if getattr(self, "_edit_mode", False):
                focused = getattr(self, "_focused_element", None)
                commit = focused is self.entries or self._uses_leave_commit(focused)
                notify = focused is not self.entries
                self._exit_edit_mode(commit=commit, notify=notify)
                return True
            self._flash_close_button()
            self._close(None)
            return True
        return super().handle_key(key)

    def elementchanged(self, element, value):
        if element == self.open_btn:
            if self._browser_mode == "save":
                self._accept_filename()
            else:
                self._accept_current()
        elif element == self.close_btn:
            self._close(None)
        elif element == self.entries:
            self._accept_current()
        elif element == self.filename:
            self._update_open_enabled()

    def selectionchanged(self, element, value):
        if element == self.entries:
            self._preview_selected()

    def confirmed(self, element, value):
        if element == self.entries:
            self._accept_current()

    def _refresh_entries(self):
        entries = [PARENT_ENTRY]
        try:
            names = sorted(os.listdir(self._current_dir))
        except OSError:
            names = []
        dirs = [f"{name}/" for name in names if os.path.isdir(os.path.join(self._current_dir, name))]
        files = [name for name in names if os.path.isfile(os.path.join(self._current_dir, name))]
        entries.extend(dirs)
        entries.extend(files)
        self.entries._options = entries
        self.entries.disabled_items = [
            name for name in files
            if not self._path_matches_filter(os.path.join(self._current_dir, name))
        ]
        self.entries._scroll_offset = 0
        selected = self._initial_filename if self._initial_filename in entries else (entries[0] if entries else None)
        self.entries.selected_items = [selected] if selected else []
        self.path_label.text = self._current_dir
        self._preview_selected()

    def _preview_selected(self):
        selected = self.entries.selected_item or ""
        if selected and selected != PARENT_ENTRY:
            self.filename.value = selected.rstrip("/")
        elif self._browser_mode == "open":
            self.filename.value = ""
        self._update_open_enabled()

    def _accept_current(self):
        selected = self.entries.selected_item or ""
        name = (self.filename.value or "").strip()
        if name:
            path = os.path.abspath(os.path.join(self._current_dir, name))
            if not path.startswith(self._root_dir):
                return
            if os.path.isdir(path):
                self._current_dir = self._clamp_dir(path)
                self._refresh_entries()
                return
            if self._browser_mode == "open" and not os.path.isfile(path):
                return
            if self._browser_mode == "open" and not self._path_matches_filter(path):
                return
            self._close(path)
            return

        if selected == PARENT_ENTRY:
            self._current_dir = self._clamp_dir(os.path.dirname(self._current_dir))
            self._refresh_entries()
            return
        if selected.endswith("/"):
            self._current_dir = self._clamp_dir(os.path.join(self._current_dir, selected.rstrip("/")))
            self._refresh_entries()
            return

        name = selected.strip()
        if not name:
            return
        path = os.path.abspath(os.path.join(self._current_dir, name))
        if not path.startswith(self._root_dir):
            return
        if self._browser_mode == "open" and not os.path.isfile(path):
            return
        if self._browser_mode == "open" and not self._path_matches_filter(path):
            return
        self._close(path)

    def _accept_filename(self):
        name = (self.filename.value or "").strip()
        if not name:
            return
        path = os.path.abspath(os.path.join(self._current_dir, name))
        if not path.startswith(self._root_dir) or os.path.isdir(path):
            return
        if os.path.isfile(path):
            self._confirm_overwrite(path)
            return
        self._close(path)

    def _confirm_overwrite(self, path):
        if self._app is None:
            self._close(path)
            return

        def on_close(result):
            if result == "yes":
                self._close(path)

        self._app.open(MessageBoxYesNo(
            header="Overwrite File",
            message=f"File already exists:\n{path}\n\nOverwrite it?",
            on_close=on_close,
        ))

    def _close(self, path):
        if self._on_close:
            self._on_close(path)
        if self._app is not None:
            self._app.close_current()

    def _flash_close_button(self):
        self.set_focus(self.close_btn)
        app = getattr(self, "_app", None)
        if app is not None:
            app.mark_dirty()
            app._render_to_terminal()
        time.sleep(FILE_BROWSER_ESCAPE_FLASH_SECONDS)

    def _clamp_dir(self, path):
        path = os.path.abspath(path)
        if not path.startswith(self._root_dir):
            return self._root_dir
        return path if os.path.isdir(path) else self._root_dir

    def _path_matches_filter(self, path):
        if not self._extension_filter:
            return True
        _, ext = os.path.splitext(str(path))
        ext = ext.lstrip(".").lower()
        if not ext:
            return False
        try:
            return re.fullmatch(self._extension_filter, ext, re.IGNORECASE) is not None
        except re.error:
            allowed = {
                part.strip().lower().lstrip(".")
                for part in self._extension_filter.split("|")
                if part.strip()
            }
            return ext in allowed

    def _selected_path_for_open_state(self):
        name = (self.filename.value or "").strip()
        if name:
            return os.path.abspath(os.path.join(self._current_dir, name))
        selected = self.entries.selected_item or ""
        if not selected:
            return None
        if selected == PARENT_ENTRY:
            return self._clamp_dir(os.path.dirname(self._current_dir))
        return os.path.abspath(os.path.join(self._current_dir, selected.rstrip("/")))

    def _update_open_enabled(self):
        if self._browser_mode == "save":
            self.open_btn.enabled = bool((self.filename.value or "").strip())
            return
        path = self._selected_path_for_open_state()
        if not path:
            self.open_btn.enabled = False
            return
        if os.path.isdir(path):
            self.open_btn.enabled = True
            return
        self.open_btn.enabled = os.path.isfile(path) and self._path_matches_filter(path)
