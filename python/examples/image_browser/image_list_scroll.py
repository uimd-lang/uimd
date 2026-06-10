"""Scrollable image item list."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(_here))))

from image_list_item import ImageListItem
from image_list_scroll_ui import ImageListScrollUI


class ImageListScroll(ImageListScrollUI):
    def set_items(self, records, render_mode, on_show=None, on_browse=None, on_delete=None, on_change=None, scroll_to_end=False, preserve_scroll=False, project_dir=None):
        position = self.scroll_position() if preserve_scroll else None
        self.clear_children()
        for record in records:
            item = ImageListItem()
            item.parent = self
            item._app = self._app
            item.mode = "expand_width"
            item.configure(
                record,
                render_mode,
                on_show=on_show,
                on_browse=on_browse,
                on_delete=on_delete,
                on_change=on_change,
                project_dir=project_dir,
            )
            item.open()
            self.add_child(item)
        if scroll_to_end:
            self.scroll_to_bottom()
        elif preserve_scroll:
            self.restore_scroll_position(position)
        else:
            self.scroll_to_top()

    def _render_child(self, child, width):
        _natural_width, natural_height = child.get_natural_size()
        child.resize(width, max(1, natural_height))
        if child._resolved_cells_by_name:
            actual_height = max(
                info["row"] + info["height"]
                for info in child._resolved_cells_by_name.values()
            )
            if actual_height > child._window_height:
                child.resize(width, actual_height)
        return child.render()
