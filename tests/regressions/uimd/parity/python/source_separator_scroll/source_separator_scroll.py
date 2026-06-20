"""Regression app for source separator rows inside ScrollView children."""

import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.abspath(os.path.join(_HERE, "..", "..", "..", "..", "..", ".."))
sys.path.insert(0, os.path.join(_ROOT, "src"))

from uimd.runtime import UIApplication

from source_separator_row_ui import SourceSeparatorRowUI
from source_separator_scroll_ui import SourceSeparatorScrollUI


class SourceSeparatorScroll(SourceSeparatorScrollUI):
    """ScrollView app seeded with one generated child row."""

    def open(self):
        super().open()
        self.clear_children()
        row = SourceSeparatorRowUI()
        row.parent = self
        row._app = self._app
        row.mode = "expand_width"
        row.open()
        self.add_child(row)


def main():
    app = UIApplication()
    window = SourceSeparatorScroll()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
