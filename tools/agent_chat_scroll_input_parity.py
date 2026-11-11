#!/usr/bin/env python3
"""Verify Agent Chat physical keyboard and mouse scrolling across language ports."""

from __future__ import annotations

import argparse
import os
import shutil
import sys
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from swift_direct_terminal_smoke import (  # noqa: E402
    PtyApp,
    sgr_click,
    sgr_wheel,
    wait_for_screen_text,
)


APP_ROWS = 35
APP_COLS = 90
TRANSCRIPT_MOUSE_X = APP_COLS // 2
TRANSCRIPT_MOUSE_Y = APP_ROWS // 2
WHEEL_UP_BUTTON = 64
WHEEL_DOWN_BUTTON = 65
WHEEL_BURST_REPORTS = 3
ARROW_SCROLL_ROWS = 1
SINGLE_WHEEL_SCROLL_ROWS = 4
BURST_WHEEL_SCROLL_ROWS = SINGLE_WHEEL_SCROLL_ROWS * WHEEL_BURST_REPORTS
MAX_INPUT_RESPONSE_SECONDS = 2.0
INPUT_RESPONSE_DRAIN_SLICE_SECONDS = 0.25
BOUNDARY_NOOP_SETTLE_SECONDS = 0.25
STARTUP_TIMEOUT_SECONDS = 10.0
LOAD_TIMEOUT_SECONDS = 30.0
MAX_FOCUS_CYCLE_STEPS = 8
SCROLL_SHIFT_SEARCH_ROWS = 48
MIN_SCROLL_SHIFT_MATCHES = 3
SCROLL_INDICATOR_COLUMNS = 2
STRESS_FIRST_ENTRY_MARKER = "Please investigate deterministic case 0."


def swift_executable() -> Path:
    candidates = sorted((ROOT / "swift/examples/agent_chat/.build").glob("*/release/agent_chat"))
    if not candidates:
        raise FileNotFoundError("Swift Agent Chat release executable was not built")
    return candidates[0]


def commands() -> dict[str, list[str]]:
    dotnet = shutil.which("dotnet") or str(Path.home() / ".local/bin/dotnet")
    return {
        "python": [sys.executable, str(ROOT / "python/examples/agent_chat/agent_chat.py")],
        "cpp": [str(ROOT / "cpp/build/examples/agent_chat/agent_chat")],
        "csharp": [dotnet, str(ROOT / "csharp/examples/agent_chat/bin/Release/net10.0/agent_chat.dll")],
        "go": [str(ROOT / "go/examples/agent_chat/agent_chat")],
        "rust": [str(ROOT / "rust/examples/agent_chat/target/release/agent_chat")],
        "swift": [str(swift_executable())],
        "java": [str(ROOT / "java/examples/agent_chat/build/install/agent_chat/bin/agent_chat")],
    }


def send_timed(app: PtyApp, data: bytes, platform: str, action: str) -> int:
    app.output.clear()
    started = time.monotonic()
    app.send(data)
    deadline = started + MAX_INPUT_RESPONSE_SECONDS
    while not app.output and time.monotonic() < deadline:
        app.drain(total_seconds=INPUT_RESPONSE_DRAIN_SLICE_SECONDS)
    elapsed = time.monotonic() - started
    if elapsed > MAX_INPUT_RESPONSE_SECONDS:
        raise AssertionError(
            f"{platform}: {action} response took {elapsed:.3f}s; "
            f"budget is {MAX_INPUT_RESPONSE_SECONDS:.3f}s"
        )
    return len(app.output)


def capture_screen(app: PtyApp) -> list[str]:
    return ["".join(row) for row in app.screen.grid]


def transcript_rows(app: PtyApp) -> tuple[int, int]:
    first_row, _first_col = app.screen.position_of(STRESS_FIRST_ENTRY_MARKER)
    status_row, _status_col = app.screen.position_of("Stress loaded")
    return first_row, status_row


def scroll_shift_score(
    before: list[str],
    after: list[str],
    first_row: int,
    end_row: int,
    shift: int,
) -> int:
    score = 0
    for row in range(first_row, end_row):
        source_row = row + shift
        if source_row < first_row or source_row >= end_row:
            continue
        source = before[source_row][:-SCROLL_INDICATOR_COLUMNS]
        if source.strip() and after[row][:-SCROLL_INDICATOR_COLUMNS] == source:
            score += 1
    return score


def assert_scroll_shift(
    before: list[str],
    after: list[str],
    first_row: int,
    end_row: int,
    expected: int,
    platform: str,
    action: str,
) -> None:
    candidates = range(-SCROLL_SHIFT_SEARCH_ROWS, SCROLL_SHIFT_SEARCH_ROWS + 1)
    scores = {
        shift: scroll_shift_score(before, after, first_row, end_row, shift)
        for shift in candidates
    }
    actual = max(scores, key=scores.get)
    expected_score = scores[expected]
    competing_score = max(score for shift, score in scores.items() if shift != expected)
    if (
        actual != expected
        or expected_score < MIN_SCROLL_SHIFT_MATCHES
        or expected_score <= competing_score
    ):
        raise AssertionError(
            f"{platform}: {action} shifted visible content by {actual} rows; "
            f"expected {expected} (matching rows: expected={expected_score}, "
            f"best competing={competing_score})"
        )


def load_button_is_focused(app: PtyApp) -> bool:
    row, col = app.screen.position_of("Load 50k")
    line = "".join(app.screen.grid[row])
    before = line[max(0, col - MAX_FOCUS_CYCLE_STEPS) : col]
    after = line[col + len("Load 50k") : col + len("Load 50k") + MAX_FOCUS_CYCLE_STEPS]
    return "[" in before and "]" in after


def focus_load_button(app: PtyApp, platform: str) -> None:
    for _step in range(MAX_FOCUS_CYCLE_STEPS):
        if load_button_is_focused(app):
            return
        send_timed(app, b"\t", platform, "focus-cycle Tab")
    raise AssertionError(f"{platform}: could not establish focus on the Load 50k button")


def load_stress_transcript(app: PtyApp, platform: str) -> None:
    load_row, load_col = wait_for_screen_text(
        app,
        "Load 50k",
        timeout_seconds=STARTUP_TIMEOUT_SECONDS,
    )
    app.send(sgr_click(load_col + len("Load 50k") // 2, load_row))
    try:
        wait_for_screen_text(app, "Stress loaded", timeout_seconds=LOAD_TIMEOUT_SECONDS)
    except AssertionError as error:
        raise AssertionError(
            f"{platform}: Load 50k did not complete\n{app.screen.text()}"
        ) from error


def run_platform(platform: str, command: list[str]) -> None:
    for part in command:
        if os.path.sep in part and not Path(part).exists():
            raise FileNotFoundError(f"{platform}: required path does not exist: {part}")
    with PtyApp(command, ROOT, APP_ROWS, APP_COLS, {"TERM": "xterm-256color"}) as app:
        load_stress_transcript(app, platform)
        first_row, end_row = transcript_rows(app)
        focus_load_button(app, platform)
        send_timed(app, b"\t", platform, "focus Top button")
        send_timed(app, b"\t", platform, "focus Bottom button")
        send_timed(app, b"\t", platform, "focus transcript")
        send_timed(app, b"\r", platform, "enter transcript scope")

        before = capture_screen(app)
        first_arrow_output = send_timed(app, b"\x1b[B", platform, "first ArrowDown")
        if first_arrow_output == 0:
            raise AssertionError(f"{platform}: first ArrowDown produced no terminal update")
        after = capture_screen(app)
        assert_scroll_shift(
            before,
            after,
            first_row,
            end_row,
            ARROW_SCROLL_ROWS,
            platform,
            "first ArrowDown",
        )

        before = after
        second_arrow_output = send_timed(app, b"\x1b[B", platform, "second ArrowDown")
        if second_arrow_output == 0:
            raise AssertionError(f"{platform}: second ArrowDown produced no terminal update")
        after = capture_screen(app)
        assert_scroll_shift(
            before,
            after,
            first_row,
            end_row,
            ARROW_SCROLL_ROWS,
            platform,
            "second ArrowDown after an intervening render",
        )

    with PtyApp(command, ROOT, APP_ROWS, APP_COLS, {"TERM": "xterm-256color"}) as app:
        load_stress_transcript(app, platform)
        first_row, end_row = transcript_rows(app)
        wheel_up = sgr_wheel(WHEEL_UP_BUTTON, TRANSCRIPT_MOUSE_X, TRANSCRIPT_MOUSE_Y)
        wheel_down = sgr_wheel(WHEEL_DOWN_BUTTON, TRANSCRIPT_MOUSE_X, TRANSCRIPT_MOUSE_Y)
        boundary = capture_screen(app)
        app.output.clear()
        app.send(wheel_up)
        app.drain(total_seconds=BOUNDARY_NOOP_SETTLE_SECONDS)
        if capture_screen(app) != boundary:
            raise AssertionError(
                f"{platform}: wheel-up changed the transcript at its top boundary"
            )

        before = capture_screen(app)
        single_wheel_output = send_timed(app, wheel_down, platform, "single wheel-down report")
        if single_wheel_output == 0:
            raise AssertionError(f"{platform}: single wheel-down report produced no terminal update")
        after = capture_screen(app)
        assert_scroll_shift(
            before,
            after,
            first_row,
            end_row,
            SINGLE_WHEEL_SCROLL_ROWS,
            platform,
            "single wheel-down report",
        )

        before = after
        reverse_wheel_output = send_timed(app, wheel_up, platform, "single wheel-up report")
        if reverse_wheel_output == 0:
            raise AssertionError(f"{platform}: single wheel-up report produced no terminal update")
        after = capture_screen(app)
        assert_scroll_shift(
            before,
            after,
            first_row,
            end_row,
            -SINGLE_WHEEL_SCROLL_ROWS,
            platform,
            "single wheel-up report",
        )

        before = after
        wheel_burst_output = send_timed(
            app,
            wheel_down * WHEEL_BURST_REPORTS,
            platform,
            "three-report wheel burst",
        )
        if wheel_burst_output == 0:
            raise AssertionError(f"{platform}: wheel burst produced no terminal update")
        after = capture_screen(app)
        assert_scroll_shift(
            before,
            after,
            first_row,
            end_row,
            BURST_WHEEL_SCROLL_ROWS,
            platform,
            "three-report wheel burst",
        )
    print(
        f"PASS {platform}: arrows moved {ARROW_SCROLL_ROWS} row, wheel reports "
        f"moved {SINGLE_WHEEL_SCROLL_ROWS}/{BURST_WHEEL_SCROLL_ROWS} rows, "
        "the top boundary was stable, and repeated input remained responsive"
    )


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--platform",
        action="append",
        choices=("python", "cpp", "csharp", "go", "rust", "swift", "java"),
        help="limit the check to one or more platforms; default: all",
    )
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(sys.argv[1:] if argv is None else argv)
    available = commands()
    selected = args.platform or list(available)
    for platform in selected:
        run_platform(platform, available[platform])
    print(f"PASS Agent Chat physical scroll parity: {len(selected)}/{len(selected)} platforms")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
