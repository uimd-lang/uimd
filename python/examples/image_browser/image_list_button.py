"""Image list preview tile."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from image_list_button_ui import ImageListButtonUI


_TILE_NORMAL_BG = "#172033"
_TILE_SELECTED_BG = "#2563eb"


class ImageListButton(ImageListButtonUI):
    def __init__(self):
        super().__init__()
        self._on_click = None
        for image in [self.prw_1, self.prw_2, self.prw_3]:
            image.focusable = False

    def set_images(self, images):
        for image, entry in zip([self.prw_1, self.prw_2, self.prw_3], images):
            name, source = entry
            image.source = source
            image.alt = name
            image._render_cache.clear()
            image._mark_dirty()

    def set_render_mode(self, render_mode):
        for image in [self.prw_1, self.prw_2, self.prw_3]:
            image.render_mode = render_mode
            image._render_cache.clear()
            image._mark_dirty()

    def set_on_click(self, callback):
        self._on_click = callback

    def set_selected(self, selected):
        bg = _TILE_SELECTED_BG if selected else _TILE_NORMAL_BG
        if self._compiled_styles.setdefault("cell", {}).get("background") == bg:
            return
        self._compiled_styles["cell"]["background"] = bg
        self._resolve_layout_geometry()
        for image in [self.prw_1, self.prw_2, self.prw_3]:
            image._render_cache.clear()
            image._mark_dirty()
        if self._app is not None:
            self._app.request_full_redraw()

    def on_prw_1_click(self): self._fire()
    def on_prw_2_click(self): self._fire()
    def on_prw_3_click(self): self._fire()

    def _fire(self):
        if self._on_click:
            self._on_click()

    def activate(self):
        self._fire()
        return self._on_click is not None
