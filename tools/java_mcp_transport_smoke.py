#!/usr/bin/env python3
"""Protocol smoke tests for Java MCP stdio, TCP, and HTTP transports."""

from __future__ import annotations

import argparse
from pathlib import Path
import sys

try:
    from rust_mcp_transport_smoke import (
        ROOT,
        TransportTarget,
        run_transport_smoke,
    )
except ModuleNotFoundError:
    from tools.rust_mcp_transport_smoke import (
        ROOT,
        TransportTarget,
        run_transport_smoke,
    )


JAVA_TARGET = TransportTarget("Java", "java", "ui-java-runtime")
DEFAULT_BINARY = Path(
    "java/examples/task_board/build/install/task_board/bin/task_board"
)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, default=DEFAULT_BINARY)
    args = parser.parse_args()
    binary = args.binary if args.binary.is_absolute() else ROOT / args.binary
    return run_transport_smoke(binary, JAVA_TARGET)


if __name__ == "__main__":
    sys.exit(main())
