"""Interactive image gallery example."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from runtime import UIApplication
from image_gallery_ui import ImageGalleryUI


class ImageGallery(ImageGalleryUI):
    def on_close_btn_click(self):
        if self._app is not None:
            self._app._running = False


def smoke():
    window = ImageGallery()
    assert window.title == "Image Gallery"
    assert window.camera_fb is not None
    assert window.astro_fb is not None
    assert window.coffee_fb is not None
    assert window.coins_fb is not None
    assert window.camera_ctn is not None
    assert window.astro_ctn is not None
    assert window.coffee_ctn is not None
    assert window.coins_ctn is not None
    window.coins_fb.width = 8
    window.coins_fb.height = 10
    rendered = window.coins_fb.render()
    assert len(rendered) == 10
    assert len(rendered[0]) == 8
    assert rendered[0]


def main():
    if len(sys.argv) > 1 and sys.argv[1] in ("--smoke", "--logic-test"):
        smoke()
        return 0

    app = UIApplication()
    window = ImageGallery()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
