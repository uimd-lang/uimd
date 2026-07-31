#!/usr/bin/env python3
"""Direct PTY smoke tests for Rust terminal runtime parity with C++."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import signal
import sys
import time

try:
    from go_direct_terminal_smoke import (
        DEFAULT_COLS,
        DEFAULT_DRAIN_SECONDS,
        DEFAULT_QUIET_SECONDS,
        DEFAULT_ROWS,
        DEFAULT_STOP_SECONDS,
        DEFAULT_TITLE_SECONDS,
        IMAGE_BROWSER_HELD_DOWN_DRAIN_SECONDS,
        IMAGE_BROWSER_HELD_DOWN_MAX_OUTPUT_BYTES,
        IMAGE_BROWSER_HELD_DOWN_REPEAT_COUNT,
        PtyApp,
        ROOT,
        assert_equal_screen,
        focused_button_visible,
        run_dynamic_screen,
        send_split_escape_sequence,
        sgr_click,
        sgr_drag,
        sgr_press,
        sgr_release,
        wait_for_focused_button,
        wait_for_screen_text,
        wait_for_screen_text_absent,
    )
except ModuleNotFoundError:
    from tools.go_direct_terminal_smoke import (
    DEFAULT_COLS,
    DEFAULT_DRAIN_SECONDS,
    DEFAULT_QUIET_SECONDS,
    DEFAULT_ROWS,
    DEFAULT_STOP_SECONDS,
    DEFAULT_TITLE_SECONDS,
    IMAGE_BROWSER_HELD_DOWN_DRAIN_SECONDS,
    IMAGE_BROWSER_HELD_DOWN_MAX_OUTPUT_BYTES,
    IMAGE_BROWSER_HELD_DOWN_REPEAT_COUNT,
    PtyApp,
    ROOT,
    assert_equal_screen,
    focused_button_visible,
    run_dynamic_screen,
    send_split_escape_sequence,
    sgr_click,
    sgr_drag,
    sgr_press,
    sgr_release,
    wait_for_focused_button,
    wait_for_screen_text,
    wait_for_screen_text_absent,
    )


SPLIT_SEQUENCE_DELAY_SECONDS = 0.03
DIALOG_FLASH_CAPTURE_SECONDS = 0.08
IDLE_CAPTURE_SECONDS = 0.7


def executable(examples_dir: Path, name: str) -> Path:
    return ROOT / examples_dir / name / "target" / "release" / name


def check_binaries(cpp_build_dir: Path, rust_examples_dir: Path) -> None:
    paths = [
        ROOT / cpp_build_dir / "examples/calculator/calculator",
        ROOT / cpp_build_dir / "examples/formular/formular",
        ROOT / cpp_build_dir / "examples/widget_gallery/widget_gallery",
        executable(rust_examples_dir, "calculator"),
        executable(rust_examples_dir, "formular"),
        executable(rust_examples_dir, "image_browser"),
        executable(rust_examples_dir, "image_gallery"),
        executable(rust_examples_dir, "task_board"),
        executable(rust_examples_dir, "widget_gallery"),
    ]
    missing = [path for path in paths if not path.exists()]
    if missing:
        details = "\n".join(f"  - {path.relative_to(ROOT)}" for path in missing)
        raise FileNotFoundError(f"missing Rust direct-terminal smoke binary:\n{details}")


def run_title_ctrl_c_and_teardown(command: list[str]) -> None:
    with PtyApp(command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        deadline = time.monotonic() + DEFAULT_TITLE_SECONDS
        while not app.screen.title.endswith(" [rust]") and time.monotonic() < deadline:
            app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        if not app.screen.title.endswith(" [rust]"):
            raise AssertionError(f"Rust terminal title is {app.screen.title!r}")
        output = bytes(app.output)
        for sequence in (
            b"\x1b[?1049h",
            b"\x1b[?1006h",
            b"\x1b[?2004h",
            b"\x1b[>4;2m",
            b"\x1b[16t",
            b"\x1b[14t",
        ):
            if sequence not in output:
                raise AssertionError(f"Rust terminal startup output missing {sequence!r}")
        app.send(b"\x03")
        if app.process is None:
            raise AssertionError("Rust process did not start")
        app.process.wait(timeout=DEFAULT_STOP_SECONDS)
        app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        output = bytes(app.output)
        for sequence in (
            b"\x1b[>4;0m",
            b"\x1b[?2004l",
            b"\x1b[?1006l",
            b"\x1b[?1049l",
        ):
            if sequence not in output:
                raise AssertionError(f"Rust terminal teardown output missing {sequence!r}")
    print("PASS Rust title, Ctrl+C, terminal modes, and teardown", flush=True)


def run_signal_teardown(command: list[str]) -> None:
    for caught_signal in (signal.SIGTERM, signal.SIGHUP):
        with PtyApp(command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
            wait_for_screen_text(app, "AC")
            if app.process is None:
                raise AssertionError("Rust signal test process did not start")
            os.killpg(app.process.pid, caught_signal)
            app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
            app.process.wait(timeout=DEFAULT_STOP_SECONDS)
            output = bytes(app.output)
            for sequence in (b"\x1b[>4;0m", b"\x1b[?1006l", b"\x1b[?1049l"):
                if sequence not in output:
                    raise AssertionError(
                        f"Rust {caught_signal.name} teardown output missing {sequence!r}"
                    )
    print("PASS Rust SIGTERM/SIGHUP restore terminal state", flush=True)


def run_calculator_parity(cpp_command: list[str], rust_command: list[str]) -> None:
    def exercise(app: PtyApp) -> None:
        wait_for_screen_text(app, "AC")
        app.send(b"1+2\r")
        wait_for_screen_text(app, "3")

    cpp_screen = run_dynamic_screen(cpp_command, ROOT, exercise)
    rust_screen = run_dynamic_screen(rust_command, ROOT, exercise)
    assert_equal_screen("calculator raw-key screen", cpp_screen, rust_screen)

    with PtyApp(rust_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        wait_for_screen_text(app, "AC")
        app.send(b"\x1b")
        if app.process is None or app.process.poll() is not None:
            raise AssertionError("root Escape terminated the Rust application")
        row, col = wait_for_screen_text(app, " 1 ")
        app.send(sgr_click(col + 1, row))
        wait_for_screen_text(app, "1")
    print("PASS Rust root Escape and SGR button press/release", flush=True)


def run_formular_input_cases(
    cpp_command: list[str],
    rust_command: list[str],
) -> None:
    with PtyApp(rust_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(b"\t" * 4 + b"\r")
        app.send(b"\x1b[200~split paste\nvalue\x1b[201~")
        wait_for_screen_text(app, "Description      split paste")
        wait_for_screen_text(app, "                  value")

    with PtyApp(rust_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(b"\t")
        send_split_escape_sequence(app, b"\x1b[B")
        send_split_escape_sequence(app, b"\x1b[A")
        send_split_escape_sequence(app, b"\x1bOB")
        app.send(b"\r" + b"split navigation")
        wait_for_screen_text(app, "Email            split navigation")

    with PtyApp(rust_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(b"\t" * 4 + b"\r" + b"ab\r" + b"cd")
        for sequence in (b"\x1b[A", b"\x1b[D", b"\x1b[C", b"\x1b[B", b"\x1bOA"):
            send_split_escape_sequence(app, sequence)
        app.send(b"X")
        wait_for_screen_text(app, "Description      abX")
        wait_for_screen_text(app, "                  cd")

    def exercise_shift_selection(app: PtyApp) -> None:
        app.send(b"\t" * 4 + b"\r" + b"abcdef")
        app.send(b"\x1b[1;2D\x1b[1;2D")
        app.send(b"\x1b[99;9u")
        app.send(b"X")
        wait_for_screen_text(app, "Description      abcdX")
        app.send(b"\x1b[118;9u")
        wait_for_screen_text(app, "Description      abcdXef")
        wait_for_screen_text_absent(app, "Copied to clipboard")

    cpp_screen = run_dynamic_screen(cpp_command, ROOT, exercise_shift_selection)
    rust_screen = run_dynamic_screen(rust_command, ROOT, exercise_shift_selection)
    assert_equal_screen("formular TextArea multi-character selection", cpp_screen, rust_screen)

    def exercise_multiline_selection(app: PtyApp) -> None:
        app.send(b"\t" * 4 + b"\r" + b"abc\rdef")
        app.send(b"\x1b[1;2A")
        app.send(b"X")
        wait_for_screen_text(app, "Description      abcX")

    cpp_screen = run_dynamic_screen(cpp_command, ROOT, exercise_multiline_selection)
    rust_screen = run_dynamic_screen(rust_command, ROOT, exercise_multiline_selection)
    assert_equal_screen("formular TextArea multiline selection", cpp_screen, rust_screen)
    print(
        "PASS Rust split CSI/SS3, TextArea arrows/selection, and one paste event",
        flush=True,
    )


def run_mouse_selection(
    cpp_command: list[str],
    rust_command: list[str],
) -> None:
    def exercise(app: PtyApp) -> None:
        wait_for_screen_text(app, "Widget Gallery")
        row, col = wait_for_screen_text(app, "Ada Lovelace")
        app.send(sgr_press(col, row))
        app.send(sgr_drag(col + 3, row))
        app.send(sgr_release(col + 3, row))
        wait_for_screen_text(app, "Copied to clipboard")
        app.send(b"Eve")
        wait_for_screen_text(app, "Eve Lovelace")
        wait_for_screen_text_absent(app, "Copied to clipboard")

    cpp_screen = run_dynamic_screen(cpp_command, ROOT, exercise)
    rust_screen = run_dynamic_screen(rust_command, ROOT, exercise)
    assert_equal_screen("widget_gallery mouse drag/copy", cpp_screen, rust_screen)


def run_dialog_escape_flash(command: list[str]) -> None:
    with PtyApp(command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        row, col = wait_for_screen_text(app, "Clear board")
        app.send(sgr_click(col + 1, row))
        wait_for_screen_text(app, "Delete every task from the board?")
        app.send(b"\x1b[D")
        wait_for_focused_button(app, "Yes")
        app.send_immediate(b"\x1b")
        app.drain(total_seconds=DIALOG_FLASH_CAPTURE_SECONDS)
        if not focused_button_visible(app, "No"):
            raise AssertionError(
                "Rust dialog Escape did not render the semantic negative action\n"
                + app.screen.text()
            )
        wait_for_screen_text(app, "Action canceled.")
        wait_for_screen_text_absent(app, "Delete every task from the board?")
    print("PASS Rust dialog Escape negative-button flash", flush=True)


def run_image_diff_and_quit(
    browser_command: list[str],
    gallery_command: list[str],
) -> None:
    with PtyApp(browser_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        wait_for_screen_text(app, "Image items")
        app.send(b"\t" * 4)
        app.send(b"\r")
        app.send(b"\x1b[C")
        app.send(b"\r")
        wait_for_screen_text(app, "Camera")
        app.drain(total_seconds=IDLE_CAPTURE_SECONDS)
        idle_start = len(app.output)
        app.drain(total_seconds=IDLE_CAPTURE_SECONDS)
        if app.output[idle_start:]:
            raise AssertionError(
                "idle Rust image_browser emitted redundant terminal output: "
                f"{len(app.output) - idle_start} bytes"
            )
        output_start = len(app.output)
        for _ in range(IMAGE_BROWSER_HELD_DOWN_REPEAT_COUNT):
            app.send_immediate(b"\x1b[B")
            time.sleep(SPLIT_SEQUENCE_DELAY_SECONDS)
        app.drain(total_seconds=IMAGE_BROWSER_HELD_DOWN_DRAIN_SECONDS)
        held_output = bytes(app.output[output_start:])
        if len(held_output) > IMAGE_BROWSER_HELD_DOWN_MAX_OUTPUT_BYTES:
            raise AssertionError(
                f"held Down emitted {len(held_output)} bytes; "
                f"limit is {IMAGE_BROWSER_HELD_DOWN_MAX_OUTPUT_BYTES}"
            )
        if held_output.count(b"\x1bPq") > IMAGE_BROWSER_HELD_DOWN_REPEAT_COUNT:
            raise AssertionError("unchanged Sixel payload was retransmitted")
        wait_for_screen_text(app, "Sample 19")

    with PtyApp(gallery_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        row, col = wait_for_screen_text(app, "Quit")
        app.send(sgr_click(col + 1, row))
        if app.process is None:
            raise AssertionError("Rust image_gallery did not start")
        app.process.wait(timeout=DEFAULT_STOP_SECONDS)
    print("PASS Rust image diff scheduling, bounded output, and explicit Quit", flush=True)


def documented_cargo_run_command(example: str) -> list[str]:
    command = (
        f"./uimd generate rust/examples/{example} --target rust && "
        "python3 tools/cargo_with_progress.py run --release "
        f"--manifest-path rust/examples/{example}/Cargo.toml"
    )
    return ["/bin/zsh", "-lc", command]


def run_documented_cargo_launcher_cases() -> None:
    with PtyApp(
        documented_cargo_run_command("calculator"),
        ROOT,
        DEFAULT_ROWS,
        DEFAULT_COLS,
    ) as app:
        wait_for_screen_text(app, "AC")
        if app.process is None or app.process.poll() is not None:
            raise AssertionError("documented calculator command exited before interaction")
        if b"==> launching interactive:" not in app.output:
            raise AssertionError("Cargo launcher did not report the interactive executable")
        app.send(b"1+2\r")
        wait_for_screen_text(app, "3")
        app.send_immediate(b"\x03")
        app.process.wait(timeout=DEFAULT_STOP_SECONDS)
        app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        if app.process.returncode != 0:
            raise AssertionError(
                f"documented calculator command exited with {app.process.returncode}"
            )

    with PtyApp(
        documented_cargo_run_command("activity_feed"),
        ROOT,
        DEFAULT_ROWS,
        DEFAULT_COLS,
    ) as app:
        row, col = wait_for_screen_text(app, "Quit")
        if app.process is None or app.process.poll() is not None:
            raise AssertionError("documented activity_feed command exited before interaction")
        if b"==> launching interactive:" not in app.output:
            raise AssertionError("Cargo launcher did not report the interactive executable")
        app.send(sgr_click(col + 1, row))
        app.process.wait(timeout=DEFAULT_STOP_SECONDS)
        app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        if app.process.returncode != 0:
            raise AssertionError(
                f"documented activity_feed command exited with {app.process.returncode}"
            )
    print(
        "PASS documented Rust Cargo launcher keeps calculator and activity_feed interactive",
        flush=True,
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cpp-build-dir", default="cpp/build", type=Path)
    parser.add_argument("--rust-examples-dir", default="rust/examples", type=Path)
    args = parser.parse_args()

    if os.name != "posix":
        print("SKIP Rust direct terminal smoke: POSIX PTY is required", flush=True)
        return 0

    check_binaries(args.cpp_build_dir, args.rust_examples_dir)
    cpp_calculator = [str(ROOT / args.cpp_build_dir / "examples/calculator/calculator")]
    cpp_widget_gallery = [
        str(ROOT / args.cpp_build_dir / "examples/widget_gallery/widget_gallery")
    ]
    cpp_formular = [str(ROOT / args.cpp_build_dir / "examples/formular/formular")]
    rust_calculator = [str(executable(args.rust_examples_dir, "calculator"))]
    rust_formular = [str(executable(args.rust_examples_dir, "formular"))]
    rust_task_board = [str(executable(args.rust_examples_dir, "task_board"))]
    rust_widget_gallery = [str(executable(args.rust_examples_dir, "widget_gallery"))]
    rust_image_browser = [
        "env",
        "UIMD_FORCE_SIXEL=1",
        str(executable(args.rust_examples_dir, "image_browser")),
    ]
    rust_image_gallery = [
        "env",
        "UIMD_DISABLE_SIXEL=1",
        str(executable(args.rust_examples_dir, "image_gallery")),
    ]

    run_documented_cargo_launcher_cases()
    run_title_ctrl_c_and_teardown(rust_calculator)
    run_signal_teardown(rust_calculator)
    run_calculator_parity(cpp_calculator, rust_calculator)
    run_formular_input_cases(cpp_formular, rust_formular)
    run_mouse_selection(cpp_widget_gallery, rust_widget_gallery)
    run_dialog_escape_flash(rust_task_board)
    run_image_diff_and_quit(rust_image_browser, rust_image_gallery)
    print("PASS Rust direct terminal smoke: 8/8 groups passed", flush=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
