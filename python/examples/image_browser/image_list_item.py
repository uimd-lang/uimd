"""Interactive image list row."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from image_data import image_display_path, image_name_from_path
from image_list_item_ui import ImageListItemUI


class ImageListItem(ImageListItemUI):
    def configure(self, record, render_mode, on_show=None, on_browse=None, on_delete=None, on_change=None, project_dir=None):
        self.record = record
        self._render_mode = render_mode
        self._on_show = on_show
        self._on_browse = on_browse
        self._on_delete = on_delete
        self._on_change = on_change
        self._project_dir = project_dir
        self._sync_from_record()

    def on_sel_change(self, value):
        self.record["selected"] = bool(value)
        if self._on_change:
            self._on_change(self.record)

    def on_show_btn_click(self):
        if self._on_show:
            self._on_show(self.record)

    def on_browse_btn_click(self):
        if self._on_browse:
            self._on_browse(self.record)

    def on_delete_btn_click(self):
        if self._on_delete:
            self._on_delete(self.record)

    def _sync_from_record(self):
        source = self.record.get("source", "")
        name = self.record.get("name") or image_name_from_path(source)
        self.thumb.source = source
        self.thumb.alt = name
        self.thumb.render_mode = self._render_mode
        self.thumb.fit = "contain"
        self.thumb._render_cache.clear()
        self.thumb._mark_dirty()
        self.name_label.text = name
        self.path_label.text = image_display_path(source, self._project_dir)
        self.sel.value = bool(self.record.get("selected", False))
