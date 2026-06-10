"""Gallery view: ViewHost view that shows all images in a scrollable list."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from gallery_view_ui import GalleryViewUI


class GalleryView(GalleryViewUI):
    def __init__(self, render_mode="sixel"):
        super().__init__()
        self._render_mode = render_mode

    def open(self):
        super().open()
        self.gallery_scroll.child.populate(self._render_mode, app=self._app)
