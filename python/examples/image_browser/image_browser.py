"""Image browser example: thumbnail sidebar + full-size ViewHost panel."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
_python_dir = os.path.dirname(os.path.dirname(_here))
_project_dir = os.path.dirname(_python_dir)
sys.path.insert(0, os.path.join(_project_dir, "src"))
sys.path.insert(0, _python_dir)

from runtime import UIApplication
from image_data import IMAGE_LIBRARY, make_initial_image_records
from image_browser_ui import ImageBrowserUI
from image_view_ui import ImageViewUI
from gallery_view import GalleryView
from image_show_dialog import ImageShowDialog
from image_list_view import ImageListView

_SIDEBAR_IMAGES = [
    ("camera_thumb",  *IMAGE_LIBRARY[0]),
    ("astro_thumb",   *IMAGE_LIBRARY[1]),
    ("coffee_thumb",  *IMAGE_LIBRARY[2]),
]


class ImageView(ImageViewUI):
    def __init__(self, source, caption, render_mode="sixel"):
        super().__init__()
        self._initial_source = source
        self._initial_caption = caption
        self._render_mode = render_mode

    def open(self):
        super().open()
        self.photo.source = self._initial_source
        self.photo.render_mode = self._render_mode
        self.caption.text = self._initial_caption
        self.photo._render_cache.clear()
        self.photo._mark_dirty()


class ImageBrowser(ImageBrowserUI):
    def __init__(self):
        super().__init__()
        self._render_mode = "sixel"
        self._image_records = make_initial_image_records()
        self._thumbs = [
            (getattr(self, name).child, caption, source)
            for name, caption, source in _SIDEBAR_IMAGES
        ]
        for index, (tile, caption, source) in enumerate(self._thumbs):
            tile.set_source(source)
            tile.set_alt(caption)
            tile.set_render_mode("sixel")
            tile.set_on_click(lambda i=index: self._show_image(self._thumbs[i]))
        self.gallery_mosaic.child.set_render_mode("sixel")
        self.gallery_mosaic.child.set_on_gallery_click(self._show_gallery)
        self.image_list_btn.child.set_images(IMAGE_LIBRARY[:3])
        self.image_list_btn.child.set_render_mode("sixel")
        self.image_list_btn.child.set_on_click(self._show_image_list)
        self._current_thumb_entry = None
        self._show_image(self._thumbs[0])

    def open(self):
        super().open()
        if self._current_thumb_entry is not None:
            self._select_thumb(self._current_thumb_entry[0])
        # Start with keyboard focus on the first reusable tile wrapper.
        self.set_focus(self.camera_thumb)

    def on_close_btn_click(self):
        if self._app is not None:
            self._app._running = False

    def on_mode_select_change(self, value):
        self._render_mode = "sixel" if value == "Normal" else "fallback"
        for tile, _, _ in self._thumbs:
            tile.set_render_mode(self._render_mode)
        self.gallery_mosaic.child.set_render_mode(self._render_mode)
        self.image_list_btn.child.set_render_mode(self._render_mode)
        current = self.main.current_view()
        if isinstance(current, ImageView):
            current.photo.render_mode = self._render_mode
            current.photo._render_cache.clear()
            current.photo._mark_dirty()
        elif isinstance(current, GalleryView):
            current._render_mode = self._render_mode
            current.gallery_scroll.child.populate(self._render_mode, app=self._app)
        elif isinstance(current, ImageListView):
            current.set_render_mode(self._render_mode)

    def _current_render_mode(self):
        return self._render_mode

    def _show_image(self, thumb_entry):
        tile, caption, source = thumb_entry
        self._current_thumb_entry = thumb_entry
        self._select_thumb(tile)
        self.gallery_mosaic.child.set_selected(False)
        self.image_list_btn.child.set_selected(False)
        self.main.set_view(ImageView(source, caption, self._current_render_mode()))

    def _show_gallery(self):
        self._current_thumb_entry = None
        self._deselect_all_thumbs()
        self.gallery_mosaic.child.set_selected(True)
        self.image_list_btn.child.set_selected(False)
        self.main.set_view(GalleryView(self._current_render_mode()))

    def _show_image_list(self):
        self._current_thumb_entry = None
        self._deselect_all_thumbs()
        self.gallery_mosaic.child.set_selected(False)
        self.image_list_btn.child.set_selected(True)
        self.main.set_view(ImageListView(self._image_records, self._current_render_mode(), _project_dir))

    def _select_thumb(self, selected):
        for tile, _, _ in self._thumbs:
            tile.set_selected(tile is selected)

    def _deselect_all_thumbs(self):
        for tile, _, _ in self._thumbs:
            tile.set_selected(False)


def main():
    if "--logic-test" in sys.argv or "--smoke" in sys.argv:
        shell = ImageBrowser()
        assert shell.main.current_view() is not None
        assert isinstance(shell.main.current_view(), ImageView)
        for thumb, caption, source in shell._thumbs[1:]:
            shell._show_image((thumb, caption, source))
            assert isinstance(shell.main.current_view(), ImageView)
        shell.on_mode_select_change("Fallback")
        assert shell._current_render_mode() == "fallback"
        shell.on_mode_select_change("Normal")
        assert shell._current_render_mode() == "sixel"
        shell._show_gallery()
        assert isinstance(shell.main.current_view(), GalleryView)
        shell._show_image_list()
        assert isinstance(shell.main.current_view(), ImageListView)
        assert len(shell._image_records) == 20
        assert IMAGE_LIBRARY[5][1].endswith(".bmp")
        assert IMAGE_LIBRARY[6][1].endswith(".gif")
        assert IMAGE_LIBRARY[7][1].endswith(".png")
        assert IMAGE_LIBRARY[8][1].endswith(".tga")
        list_view = shell.main.current_view()
        replacement = os.path.join(_project_dir, "shared/assets/image_samples/chelsea.png")
        list_view._replace_image(shell._image_records[0], replacement)
        assert shell._image_records[0]["name"] == "Chelsea"
        row = list_view.items.child._children[0]
        assert row.thumb.render_mode == "sixel"
        assert row.name_label.text == "Chelsea"
        assert row.path_label.text == "shared/assets/image_samples/chelsea.png"
        rendered_list = list_view.items.child.render_cells()
        assert any(
            getattr(cell, "raw", "")
            for rendered_row in rendered_list
            for cell in rendered_row
        )
        dialog = ImageShowDialog(replacement, "Chelsea", "fallback")
        dialog.open()
        assert dialog._metadata_kind == "dialog"
        assert dialog.title_element.text == "Chelsea"
        mosaic = shell.gallery_mosaic.child
        assert not mosaic.gal_1.focusable
        assert not mosaic.gal_2.focusable
        assert not mosaic.gal_3.focusable
        assert not mosaic.gal_4.focusable
        assert not mosaic.gal_5.focusable
        shell.open()
        assert shell._focused_element is shell.camera_thumb
        shell.set_focus(shell.coffee_thumb)
        assert shell.coffee_thumb.focused
        shell.set_focus(shell.image_list_btn)
        assert shell.image_list_btn.focused
        assert not shell.coffee_thumb.focused
        assert getattr(shell.coffee_thumb.child, "_focused_element", None) is None
        shell.handle_key("Enter")
        assert isinstance(shell.main.current_view(), ImageListView)
        return 0

    app = UIApplication()
    app.open(ImageBrowser())
    app.run()
    return 0


if __name__ == "__main__":
    sys.exit(main())
