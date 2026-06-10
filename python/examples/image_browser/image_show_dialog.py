"""Dialog showing one selected image."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from image_show_dialog_ui import ImageShowDialogUI


class ImageShowDialog(ImageShowDialogUI):
    _handles_escape = True

    def __init__(self, source, caption, render_mode="sixel"):
        super().__init__()
        self._source = source
        self._caption = caption
        self._render_mode = render_mode

    def open(self):
        super().open()
        self.title_element.text = self._caption
        self.photo.source = self._source
        self.photo.alt = self._caption
        self.photo.render_mode = self._render_mode
        self.photo._render_cache.clear()
        self.photo._mark_dirty()
        self.set_focus(self.close_btn)

    def on_close_btn_click(self):
        if self._app is not None:
            self._app.close_current()

    def handle_key(self, key):
        if key == "Escape":
            self.on_close_btn_click()
            return True
        return super().handle_key(key)
