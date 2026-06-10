"""Image list view with add, show, browse, and delete actions."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
_python_dir = os.path.dirname(os.path.dirname(_here))
_project_dir = os.path.dirname(_python_dir)
sys.path.insert(0, os.path.join(_project_dir, "src"))
sys.path.insert(0, _python_dir)

from dialogs import FileBrowser, MessageBoxYesNo
from image_data import IMAGE_EXTENSION_FILTER, IMAGE_SAMPLE_DIR, image_name_from_path, make_image_record
from image_list_view_ui import ImageListViewUI
from image_show_dialog import ImageShowDialog


class ImageListView(ImageListViewUI):
    def __init__(self, records, render_mode="sixel", project_dir=None):
        super().__init__()
        self._records = records
        self._render_mode = render_mode
        self._project_dir = project_dir or _project_dir

    def open(self):
        super().open()
        self._refresh_items()

    def set_render_mode(self, render_mode):
        self._render_mode = render_mode
        self._refresh_items(preserve_scroll=True)

    def on_add_btn_click(self):
        if self._app is None:
            return
        browser = FileBrowser(
            self._project_dir,
            os.path.join(self._project_dir, IMAGE_SAMPLE_DIR),
            mode="open",
            on_close=self._add_selected_image,
            extension_filter=IMAGE_EXTENSION_FILTER,
        )
        self._app.open(browser)

    def _show_image(self, record):
        if self._app is not None:
            self._app.open(ImageShowDialog(record.get("source", ""), record.get("name", ""), self._render_mode))

    def _browse_image(self, record):
        if self._app is None:
            return
        browser = FileBrowser(
            self._project_dir,
            record.get("source", "") or os.path.join(self._project_dir, IMAGE_SAMPLE_DIR),
            mode="open",
            on_close=lambda path: self._replace_image(record, path),
            extension_filter=IMAGE_EXTENSION_FILTER,
        )
        self._app.open(browser)

    def _confirm_delete_image(self, record):
        if self._app is None:
            self._delete_image(record)
            return

        def on_close(result):
            if result == "yes":
                self._delete_image(record)

        self._app.open(MessageBoxYesNo(
            header="Delete Image",
            message=f"Delete {record.get('name', '')}?",
            on_close=on_close,
        ))

    def _add_selected_image(self, path):
        if not path:
            return
        self._records.append(make_image_record(image_name_from_path(path), path))
        self._refresh_items(scroll_to_end=True)

    def _replace_image(self, record, path):
        if not path:
            return
        record["name"] = image_name_from_path(path)
        record["source"] = path
        self._refresh_items(preserve_scroll=True)

    def _delete_image(self, record):
        if record in self._records:
            self._records.remove(record)
            self._refresh_items(preserve_scroll=True)

    def _refresh_items(self, scroll_to_end=False, preserve_scroll=False):
        self.items.child.set_items(
            self._records,
            self._render_mode,
            on_show=self._show_image,
            on_browse=self._browse_image,
            on_delete=self._confirm_delete_image,
            scroll_to_end=scroll_to_end,
            preserve_scroll=preserve_scroll,
            project_dir=self._project_dir,
        )
