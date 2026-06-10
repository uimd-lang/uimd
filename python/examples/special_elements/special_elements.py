"""Application logic for special_elements.uimd - demonstrates InfoLabel animation."""

import sys
import os

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from runtime import UIApplication
from special_elements_ui import SpecialElementsUI


def main():
    app = UIApplication()
    window = SpecialElementsUI()
    window._app = app
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
