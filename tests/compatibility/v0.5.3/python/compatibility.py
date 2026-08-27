"""Application wrapper retained with the UIMD 0.5.3 generated fixture."""

import sys

from uimd.runtime import UIApplication

from compatibility_ui import CompatibilityUI


class Compatibility(CompatibilityUI):
    pass


def main() -> int:
    application = UIApplication()
    application.open(Compatibility())
    return application.run()


if __name__ == "__main__":
    sys.exit(main())
