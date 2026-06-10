"""Gallery mosaic: 3+2 image grid that opens the full scrollable gallery."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from gallery_mosaic_ui import GalleryMosaicUI


_TILE_NORMAL_BG = "#172033"
_TILE_SELECTED_BG = "#2563eb"


class GalleryMosaic(GalleryMosaicUI):
    def __init__(self):
        super().__init__()
        self._on_gallery_click = None
        for img in [self.gal_1, self.gal_2, self.gal_3, self.gal_4, self.gal_5]:
            img.focusable = False

    def set_on_gallery_click(self, callback):
        self._on_gallery_click = callback

    def set_selected(self, selected):
        bg = _TILE_SELECTED_BG if selected else _TILE_NORMAL_BG
        self._compiled_styles.setdefault("cell", {})["background"] = bg
        self._resolve_layout_geometry()
        for img in [self.gal_1, self.gal_2, self.gal_3, self.gal_4, self.gal_5]:
            img._render_cache.clear()
            img._mark_dirty()
        if self._app is not None:
            self._app.request_full_redraw()

    def set_render_mode(self, render_mode):
        for img in [self.gal_1, self.gal_2, self.gal_3, self.gal_4, self.gal_5]:
            img.render_mode = render_mode
            img._render_cache.clear()
            img._mark_dirty()

    def on_gal_1_click(self): self._fire()
    def on_gal_2_click(self): self._fire()
    def on_gal_3_click(self): self._fire()
    def on_gal_4_click(self): self._fire()
    def on_gal_5_click(self): self._fire()

    def _fire(self):
        if self._on_gallery_click:
            self._on_gallery_click()

    def activate(self):
        self._fire()
        return self._on_gallery_click is not None
