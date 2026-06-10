"""Application logic for cells.uimd - extends generated UI."""

import sys
import os

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from runtime import UIApplication
from cells_ui import CellsUI


class Cells(CellsUI):
    """Custom cells with application logic."""

    def open(self):
        super().open()

    def save(self):
        pass

    def focused(self, element, value):
        pass


def main():
    app = UIApplication()
    window = Cells()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
