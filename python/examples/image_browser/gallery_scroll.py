"""Gallery scroll view: UIScrollView that hosts GalleryItem children."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from gallery_item import GalleryItem
from image_data import IMAGE_LIBRARY
from gallery_scroll_ui import GalleryScrollUI

_GALLERY_IMAGES = IMAGE_LIBRARY[:5]


class GalleryScroll(GalleryScrollUI):
    def populate(self, render_mode, app=None):
        self.clear_children()
        for name, source in _GALLERY_IMAGES:
            item = GalleryItem(source, name, render_mode)
            item.parent = self
            item._app = app
            item.mode = "expand_width"
            item.open()
            self.add_child(item)
        self.scroll_to_top()
