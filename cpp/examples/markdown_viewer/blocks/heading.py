"""Application logic for heading.uimd - extends generated UI."""

import sys
import os

def _project_root():
    d = os.path.dirname(os.path.abspath(__file__))
    while d != os.path.dirname(d):
        if os.path.isdir(os.path.join(d, "runtime")):
            return d
        d = os.path.dirname(d)
    return d

sys.path.insert(0, _project_root())

from runtime import UIApplication
from heading_ui import HeadingUI


class Heading(HeadingUI):
    """Custom heading with application logic."""

    def open(self):
        super().open()

    def elementchanged(self, element, value):
        pass

    def save(self):
        pass

    def focused(self, element, value):
        pass


def main():
    app = UIApplication()
    window = Heading()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
