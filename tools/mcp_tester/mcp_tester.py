"""Repository MCP tester entrypoint.

Defaults to the C++ tester. Pass --backend python to run the Python tester
implementation from src/uimd/testing.
"""

import os
import sys

PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC_ROOT = os.path.join(PROJECT_ROOT, "src")
PYTHON_ROOT = os.path.join(PROJECT_ROOT, "python")
for _path in (PYTHON_ROOT, SRC_ROOT):
    if _path not in sys.path:
        sys.path.insert(0, _path)

from uimd.testing import mcp_tester_launcher

main = mcp_tester_launcher.main
main_argv = mcp_tester_launcher.main_argv

if __name__ == "__main__":
    sys.exit(main())
