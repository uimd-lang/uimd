"""Image button: a single-image clickable tile that fires a click callback."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from image_button_ui import ImageButtonUI

_TILE_NORMAL_BG = "transparent"
_TILE_SELECTED_BG = "#2563eb"


class ImageButton(ImageButtonUI):
    def __init__(self):
        super().__init__()
        self.photo.focusable = False
        self._on_click = None
        self._selected = False

    def set_source(self, source):
        self.photo.source = source
        self.photo._render_cache.clear()
        self.photo._mark_dirty()

    def set_alt(self, alt):
        self.photo.alt = alt

    def set_render_mode(self, render_mode):
        self.photo.render_mode = render_mode
        self.photo._render_cache.clear()
        self.photo._mark_dirty()

    def set_on_click(self, callback):
        self._on_click = callback

    def set_selected(self, selected):
        # The image covers the whole photo area (fit: cover), so the selection
        # highlight is shown via the tile (cell padding) background that
        # surrounds it. Keyboard focus is handled by the runtime's default
        # control focus overlay (same subtle look as the gallery), so it must
        # not be painted here.
        self._selected = bool(selected)
        bg = _TILE_SELECTED_BG if self._selected else _TILE_NORMAL_BG
        if self._compiled_styles.setdefault("cell", {}).get("background") == bg:
            return
        self._compiled_styles["cell"]["background"] = bg
        self._resolve_layout_geometry()
        self.photo._render_cache.clear()
        self.photo._mark_dirty()
        if self._app is not None:
            self._app.request_full_redraw()

    def on_photo_click(self):
        if self._on_click:
            self._on_click()

    def activate(self):
        self.on_photo_click()
        return self._on_click is not None
