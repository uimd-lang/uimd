#!/usr/bin/env python3
"""Direct PTY regression for C++ ScrollView callback-time rebuilds."""

from __future__ import annotations

import argparse
from pathlib import Path
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from swift_direct_terminal_smoke import (  # noqa: E402
    PtyApp,
    sgr_press,
    wait_for_screen_text,
)


APP_ROWS = 35
APP_COLS = 90
READY_SECONDS = 10.0
POST_PRESS_SECONDS = 1.0
OPEN_LABEL = "Open"
FIRST_ROW_LABEL = "Alpha Apartments"
EDIT_PAGE_NEEDLE = "Opened from ScrollView"


def open_button_for_first_row(app: PtyApp) -> tuple[int, int]:
    row, _ = wait_for_screen_text(app, FIRST_ROW_LABEL, timeout_seconds=READY_SECONDS)
    for candidate_row in range(row, min(app.rows, row + 3)):
        text = "".join(app.screen.grid[candidate_row])
        col = text.find(OPEN_LABEL)
        if col >= 0:
            return candidate_row, col + len(OPEN_LABEL) // 2
    raise AssertionError(f"screen text not found near {FIRST_ROW_LABEL!r}: {OPEN_LABEL!r}")


def default_binary(build_dir: Path) -> Path:
    return (
        build_dir
        / "regressions"
        / "uimd"
        / "parity"
        / "stale_scrollview_focus"
        / "stale_scrollview_focus"
    )


def run(binary: Path) -> None:
    if not binary.exists():
        raise FileNotFoundError(
            f"missing stale_scrollview_focus binary: {binary}\n"
            "Build it with: cmake --build cpp/build --target uimd_regression_stale_scrollview_focus"
        )
    command = [str(binary)]
    with PtyApp(command, ROOT, APP_ROWS, APP_COLS, {"TERM": "xterm-256color"}) as app:
        row, col = open_button_for_first_row(app)
        app.send(sgr_press(0, col, row))
        deadline = time.monotonic() + READY_SECONDS
        last_error: AssertionError | None = None
        while time.monotonic() < deadline:
            if app.process is not None and app.process.poll() is not None:
                output_tail = bytes(app.output[-4096:]).decode("utf-8", errors="replace").strip()
                raise AssertionError(
                    f"stale_scrollview_focus exited after the destructive mouse press "
                    f"with code {app.process.returncode}; output: {output_tail!r}"
                )
            try:
                wait_for_screen_text(app, EDIT_PAGE_NEEDLE, timeout_seconds=POST_PRESS_SECONDS)
                print("PASS C++ ScrollView callback rebuild PTY regression")
                return
            except AssertionError as exc:
                last_error = exc
        if last_error is not None:
            raise last_error
        raise AssertionError(f"screen text not found: {EDIT_PAGE_NEEDLE!r}")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--cpp-build-dir",
        type=Path,
        default=ROOT / "cpp" / "build",
        help="C++ build directory containing the stale_scrollview_focus regression binary.",
    )
    parser.add_argument(
        "--binary",
        type=Path,
        default=None,
        help="Explicit stale_scrollview_focus binary path.",
    )
    args = parser.parse_args()
    binary = args.binary if args.binary is not None else default_binary(args.cpp_build_dir)
    run(binary)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
