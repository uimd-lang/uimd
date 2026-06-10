"""Gallery item: a fixed-height image with caption for the gallery scroll view."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from gallery_item_ui import GalleryItemUI


class GalleryItem(GalleryItemUI):
    def __init__(self, source, caption_text, render_mode="sixel"):
        super().__init__()
        self.photo.source = source
        self.photo.render_mode = render_mode
        self.caption.text = caption_text
        self.photo._render_cache.clear()
