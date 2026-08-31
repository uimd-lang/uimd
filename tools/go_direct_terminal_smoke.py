#!/usr/bin/env python3
"""Direct PTY smoke tests for Go terminal runtime parity with C++."""

from __future__ import annotations

import argparse
from collections.abc import Callable
import fcntl
import os
from pathlib import Path
import pty
import re
import select
import signal
import struct
import subprocess
import sys
import termios
import time


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "src"))

from uimd.testing.artifact_manifest import validate_artifact_paths


DEFAULT_ROWS = 35
DEFAULT_COLS = 90
DEFAULT_START_SECONDS = 1.5
DEFAULT_DRAIN_SECONDS = 1.0
DEFAULT_QUIET_SECONDS = 0.35
DEFAULT_INPUT_DELAY_SECONDS = 0.12
DEFAULT_STOP_SECONDS = 1.0
DEFAULT_TITLE_SECONDS = 5.0
DIALOG_FLASH_CAPTURE_SECONDS = 0.08
SPLIT_ARROW_SEQUENCE_DELAY_SECONDS = 0.03
TERMINAL_COORDINATE_BASE = 1
IMAGE_BROWSER_HELD_DOWN_REPEAT_COUNT = 30
IMAGE_BROWSER_IDLE_CAPTURE_SECONDS = 0.7
IMAGE_BROWSER_HELD_DOWN_DRAIN_SECONDS = 6.0
IMAGE_BROWSER_HELD_DOWN_MAX_OUTPUT_BYTES = 256 * 1024
ANIMATION_IDLE_CAPTURE_SECONDS = 0.45
ANIMATION_IDLE_SAMPLE_COUNT = 2


class TerminalScreen:
    def __init__(self, rows: int, cols: int) -> None:
        self.rows = rows
        self.cols = cols
        self.row = 0
        self.col = 0
        self.pending = ""
        self.title = ""
        self.grid = [[" "] * cols for _ in range(rows)]
        self.scroll_top = 0
        self.scroll_bottom = rows - 1

    def clear(self) -> None:
        self.grid = [[" "] * self.cols for _ in range(self.rows)]
        self.row = 0
        self.col = 0

    def feed(self, data: bytes) -> None:
        text = self.pending + data.decode("utf-8", "ignore")
        self.pending = ""
        index = 0
        while index < len(text):
            char = text[index]
            if char == "\x1b":
                consumed = self._consume_escape(text, index)
                if consumed is None:
                    self.pending = text[index:]
                    return
                index = consumed
                continue
            self._write_char(char)
            index += 1

    def text(self) -> str:
        return "\n".join("".join(row).rstrip() for row in self.grid)

    def position_of(self, needle: str) -> tuple[int, int]:
        for row_index, row in enumerate(self.grid):
            column = "".join(row).find(needle)
            if column >= 0:
                return row_index, column
        raise AssertionError(f"screen text not found: {needle!r}")

    def _consume_escape(self, text: str, index: int) -> int | None:
        if index + 1 >= len(text):
            return None
        prefix = text[index + 1]
        if prefix == "[":
            final = index + 2
            while final < len(text) and not self._is_csi_final(text[final]):
                final += 1
            if final >= len(text):
                return None
            self._handle_csi(text[index + 2:final], text[final])
            return final + 1
        if prefix == "]":
            end = index + 2
            while end < len(text) and text[end] != "\x07" and text[end:end + 2] != "\x1b\\":
                end += 1
            if end >= len(text):
                return None
            payload = text[index + 2:end]
            if payload.startswith("0;") or payload.startswith("2;"):
                self.title = payload[2:]
            return end + (2 if text[end:end + 2] == "\x1b\\" else 1)
        if prefix == "P":
            end = index + 2
            while end < len(text) and text[end:end + 2] != "\x1b\\":
                end += 1
            if end >= len(text):
                return None
            return end + 2
        return index + 2

    def _write_char(self, char: str) -> None:
        if char == "\r":
            self.col = 0
            return
        if char == "\n":
            self.row = min(self.rows - 1, self.row + 1)
            return
        if ord(char) < ord(" "):
            return
        if 0 <= self.row < self.rows and 0 <= self.col < self.cols:
            self.grid[self.row][self.col] = char
        self.col = min(self.cols - 1, self.col + 1)

    def _handle_csi(self, params: str, final: str) -> None:
        if params.startswith("?"):
            return
        values = self._parse_csi_values(params)

        def value(position: int, fallback: int) -> int:
            if position < len(values) and values[position] is not None:
                return values[position]
            return fallback

        if final in ("H", "f"):
            self.row = self._clamp(value(0, 1) - 1, 0, self.rows - 1)
            self.col = self._clamp(value(1, 1) - 1, 0, self.cols - 1)
        elif final == "A":
            self.row = max(0, self.row - value(0, 1))
        elif final == "B":
            self.row = min(self.rows - 1, self.row + value(0, 1))
        elif final == "C":
            self.col = min(self.cols - 1, self.col + value(0, 1))
        elif final == "D":
            self.col = max(0, self.col - value(0, 1))
        elif final == "G":
            self.col = self._clamp(value(0, 1) - 1, 0, self.cols - 1)
        elif final == "J" and value(0, 0) in (2, 3):
            self.clear()
        elif final == "K":
            for col in range(self.col, self.cols):
                self.grid[self.row][col] = " "
        elif final == "r":
            self.scroll_top = self._clamp(value(0, 1) - 1, 0, self.rows - 1)
            self.scroll_bottom = self._clamp(value(1, self.rows) - 1, self.scroll_top, self.rows - 1)
            self.row = 0
            self.col = 0
        elif final in ("S", "T"):
            distance = min(value(0, 1), self.scroll_bottom - self.scroll_top + 1)
            region = self.grid[self.scroll_top:self.scroll_bottom + 1]
            blank = [[" "] * self.cols for _ in range(distance)]
            if final == "S":
                region = region[distance:] + blank
            else:
                region = blank + region[:-distance]
            self.grid[self.scroll_top:self.scroll_bottom + 1] = region

    @staticmethod
    def _parse_csi_values(params: str) -> list[int | None]:
        values: list[int | None] = []
        for part in re.split(r"[;:]", params or ""):
            if part == "":
                values.append(None)
                continue
            try:
                values.append(int(part))
            except ValueError:
                values.append(None)
        return values

    @staticmethod
    def _is_csi_final(char: str) -> bool:
        return char.isalpha() or "@" <= char <= "~"

    @staticmethod
    def _clamp(value: int, lower: int, upper: int) -> int:
        return max(lower, min(upper, value))


class PtyApp:
    def __init__(self, command: list[str], cwd: Path, rows: int, cols: int) -> None:
        self.command = command
        self.cwd = cwd
        self.rows = rows
        self.cols = cols
        self.master_fd: int | None = None
        self.process: subprocess.Popen[bytes] | None = None
        self.screen = TerminalScreen(rows, cols)
        self.output = bytearray()

    def __enter__(self) -> "PtyApp":
        master_fd, slave_fd = pty.openpty()
        self._set_pty_size(master_fd)
        self._set_pty_size(slave_fd)
        env = os.environ.copy()
        env.setdefault("TERM", "xterm-256color")
        self.process = subprocess.Popen(
            self.command,
            cwd=self.cwd,
            stdin=slave_fd,
            stdout=slave_fd,
            stderr=slave_fd,
            env=env,
            preexec_fn=os.setsid,
            close_fds=True,
        )
        os.close(slave_fd)
        flags = fcntl.fcntl(master_fd, fcntl.F_GETFL)
        fcntl.fcntl(master_fd, fcntl.F_SETFL, flags | os.O_NONBLOCK)
        self.master_fd = master_fd
        self.drain(total_seconds=DEFAULT_START_SECONDS)
        return self

    def __exit__(self, _exc_type: object, _exc: object, _traceback: object) -> None:
        self.stop()
        if self.master_fd is not None:
            os.close(self.master_fd)
            self.master_fd = None

    def send(self, data: bytes) -> None:
        if self.master_fd is None:
            raise RuntimeError("PTY app is not running")
        os.write(self.master_fd, data)
        time.sleep(DEFAULT_INPUT_DELAY_SECONDS)
        self.drain()

    def send_immediate(self, data: bytes) -> None:
        if self.master_fd is None:
            raise RuntimeError("PTY app is not running")
        os.write(self.master_fd, data)

    def drain(self, *, total_seconds: float = DEFAULT_DRAIN_SECONDS) -> None:
        if self.master_fd is None:
            return
        deadline = time.monotonic() + total_seconds
        last_data = time.monotonic()
        while time.monotonic() < deadline:
            readable, _, _ = select.select([self.master_fd], [], [], 0.05)
            if readable:
                try:
                    data = os.read(self.master_fd, 65536)
                except (BlockingIOError, OSError):
                    data = b""
                if not data:
                    break
                self.output.extend(data)
                self.screen.feed(data)
                last_data = time.monotonic()
            elif time.monotonic() - last_data >= DEFAULT_QUIET_SECONDS:
                break

    def stop(self) -> None:
        if self.process is None:
            return
        if self.process.poll() is None and self.master_fd is not None:
            try:
                os.write(self.master_fd, b"\x03")
                time.sleep(DEFAULT_INPUT_DELAY_SECONDS)
                self.drain(total_seconds=0.5)
            except OSError:
                pass
        try:
            self.process.wait(timeout=DEFAULT_STOP_SECONDS)
        except subprocess.TimeoutExpired:
            os.killpg(self.process.pid, signal.SIGTERM)
            self.process.wait(timeout=DEFAULT_STOP_SECONDS)

    def _set_pty_size(self, fd: int) -> None:
        fcntl.ioctl(fd, termios.TIOCSWINSZ, struct.pack("HHHH", self.rows, self.cols, 0, 0))


def sgr_press(x: int, y: int, button: int = 0) -> bytes:
    return f"\x1b[<{button};{x + TERMINAL_COORDINATE_BASE};{y + TERMINAL_COORDINATE_BASE}M".encode()


def sgr_drag(x: int, y: int) -> bytes:
    return sgr_press(x, y, 32)


def sgr_release(x: int, y: int, button: int = 0) -> bytes:
    return f"\x1b[<{button};{x + TERMINAL_COORDINATE_BASE};{y + TERMINAL_COORDINATE_BASE}m".encode()


def sgr_click(x: int, y: int) -> bytes:
    return sgr_press(x, y) + sgr_release(x, y)


def send_split_escape_sequence(app: PtyApp, sequence: bytes) -> None:
    if app.master_fd is None:
        raise RuntimeError("PTY app is not running")
    if not sequence.startswith(b"\x1b"):
        raise ValueError("split escape sequence must start with ESC")
    os.write(app.master_fd, sequence[:1])
    time.sleep(SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
    os.write(app.master_fd, sequence[1:])
    time.sleep(DEFAULT_INPUT_DELAY_SECONDS)
    app.drain()


def wait_for_screen_text(app: PtyApp, needle: str, timeout_seconds: float = DEFAULT_TITLE_SECONDS) -> tuple[int, int]:
    deadline = time.monotonic() + timeout_seconds
    while time.monotonic() < deadline:
        try:
            return app.screen.position_of(needle)
        except AssertionError:
            app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
    return app.screen.position_of(needle)


def wait_for_screen_text_absent(app: PtyApp, needle: str, timeout_seconds: float = DEFAULT_TITLE_SECONDS) -> None:
    deadline = time.monotonic() + timeout_seconds
    while time.monotonic() < deadline:
        if needle not in app.screen.text():
            return
        app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
    raise AssertionError(f"screen text did not disappear: {needle!r}")


def focused_button_visible(app: PtyApp, title: str) -> bool:
    pattern = re.compile(r"\[[^\]\n]*\b" + re.escape(title) + r"\b[^\]\n]*\]")
    return pattern.search(app.screen.text()) is not None


def wait_for_focused_button(app: PtyApp, title: str, timeout_seconds: float = DEFAULT_TITLE_SECONDS) -> None:
    deadline = time.monotonic() + timeout_seconds
    while time.monotonic() < deadline:
        if focused_button_visible(app, title):
            return
        app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
    raise AssertionError(f"focused button did not appear: {title!r}\n{app.screen.text()}")


def check_binaries(cpp_build_dir: Path, go_examples_dir: Path) -> None:
    paths = [
        ROOT / cpp_build_dir / "examples/calculator/calculator",
        ROOT / cpp_build_dir / "examples/formular/formular",
        ROOT / cpp_build_dir / "examples/image_browser/image_browser",
        ROOT / cpp_build_dir / "examples/special_elements/special_elements",
        ROOT / cpp_build_dir / "examples/widget_gallery/widget_gallery",
        ROOT / go_examples_dir / "calculator/calculator",
        ROOT / go_examples_dir / "formular/formular",
        ROOT / go_examples_dir / "image_browser/image_browser",
        ROOT / go_examples_dir / "image_gallery/image_gallery",
        ROOT / go_examples_dir / "special_elements/special_elements",
        ROOT / go_examples_dir / "task_board/task_board",
        ROOT / go_examples_dir / "widget_gallery/widget_gallery",
    ]
    validate_artifact_paths(ROOT, paths)
    missing = [path for path in paths if not path.exists()]
    if missing:
        details = "\n".join(f"  - {path.relative_to(ROOT)}" for path in missing)
        raise FileNotFoundError(f"missing Go direct-terminal smoke binary:\n{details}")


def run_dynamic_screen(command: list[str], cwd: Path, exercise: Callable[[PtyApp], None]) -> str:
    with PtyApp(command, cwd, DEFAULT_ROWS, DEFAULT_COLS) as app:
        exercise(app)
        app.drain()
        return app.screen.text()


def run_idle_text_gradient_animation(
    cpp_command: list[str],
    go_command: list[str],
) -> None:
    def require_idle_updates(command: list[str], platform_name: str) -> None:
        with PtyApp(command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
            wait_for_screen_text(app, "Special UI Elements")
            for sample in range(ANIMATION_IDLE_SAMPLE_COUNT):
                output_start = len(app.output)
                app.drain(total_seconds=ANIMATION_IDLE_CAPTURE_SECONDS)
                if len(app.output) == output_start:
                    raise AssertionError(
                        f"{platform_name} animated gradient emitted no terminal update "
                        f"during idle sample {sample + 1}"
                    )

    require_idle_updates(cpp_command, "C++")
    require_idle_updates(go_command, "Go")
    print("PASS Go animated gradients advance without terminal input", flush=True)


def assert_equal_screen(
    name: str,
    cpp_screen: str,
    target_screen: str,
    target_name: str = "Go",
) -> None:
    if cpp_screen == target_screen:
        print(f"PASS {name}", flush=True)
        return
    print(f"FAIL {name}", flush=True)
    print("--- C++ ---", flush=True)
    print(cpp_screen, flush=True)
    print(f"--- {target_name} ---", flush=True)
    print(target_screen, flush=True)
    raise AssertionError(
        f"{name}: {target_name} direct terminal screen differs from C++"
    )


def run_title_and_teardown(go_command: list[str]) -> None:
    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        deadline = time.monotonic() + DEFAULT_TITLE_SECONDS
        while not app.screen.title.endswith(" [go]") and time.monotonic() < deadline:
            app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        if not app.screen.title.endswith(" [go]"):
            raise AssertionError(f"Go terminal title is {app.screen.title!r}")
        output = bytes(app.output)
        for sequence in (b"\x1b[?1049h", b"\x1b[?1006h", b"\x1b[?7l", b"\x1b[H", b"\x1b[2J"):
            if sequence not in output:
                raise AssertionError(f"Go terminal startup output missing {sequence!r}")
        app.send(b"\x03")
        if app.process is None:
            raise AssertionError("Go process did not start")
        app.process.wait(timeout=DEFAULT_STOP_SECONDS)
        app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        output = bytes(app.output)
        for sequence in (b"\x1b[?25h", b"\x1b[?7h", b"\x1b[?1006l", b"\x1b[?1049l"):
            if sequence not in output:
                raise AssertionError(f"Go terminal teardown output missing {sequence!r}")
    print("PASS terminal title, Ctrl+C, and teardown", flush=True)


def run_raw_key_case(cpp_command: list[str], go_command: list[str]) -> None:
    def exercise(app: PtyApp) -> None:
        wait_for_screen_text(app, "AC")
        app.send(b"1+2\r")
        wait_for_screen_text(app, "3")

    cpp_screen = run_dynamic_screen(cpp_command, ROOT, exercise)
    go_screen = run_dynamic_screen(go_command, ROOT, exercise)
    assert_equal_screen("calculator raw keys", cpp_screen, go_screen)


def run_root_escape_case(go_command: list[str]) -> None:
    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        wait_for_screen_text(app, "AC")
        app.send(b"\x1b")
        if app.process is None or app.process.poll() is not None:
            raise AssertionError("root Escape terminated the Go application")
        app.send(b"1+2\r")
        wait_for_screen_text(app, "3")
    print("PASS root Escape remains inside the app", flush=True)


def run_formular_split_arrow_cases(go_command: list[str]) -> None:
    tab = b"\t"
    enter = b"\r"
    up = b"\x1b[A"
    down = b"\x1b[B"
    left = b"\x1b[D"
    right = b"\x1b[C"
    shift_up = b"\x1b[1;2A"
    shift_down = b"\x1b[1;2B"
    ss3_up = b"\x1bOA"
    ss3_down = b"\x1bOB"
    ss3_left = b"\x1bOD"
    ss3_right = b"\x1bOC"

    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(tab)
        send_split_escape_sequence(app, down)
        send_split_escape_sequence(app, up)
        send_split_escape_sequence(app, down)
        app.send(enter + b"navigation target")
        wait_for_screen_text(app, "Email            navigation target")

    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(tab * 9)
        send_split_escape_sequence(app, right)
        send_split_escape_sequence(app, left)
        send_split_escape_sequence(app, right)
        app.send(enter)
        wait_for_screen_text(app, "action: cancel")

    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(tab * 4 + enter + b"ab" + enter + b"cd")
        send_split_escape_sequence(app, up)
        send_split_escape_sequence(app, left)
        send_split_escape_sequence(app, right)
        send_split_escape_sequence(app, down)
        send_split_escape_sequence(app, up)
        app.send(b"X")
        wait_for_screen_text(app, "Description      abX")
        wait_for_screen_text(app, "                  cd")

    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(tab * 4 + enter + b"ab" + enter + b"cd")
        send_split_escape_sequence(app, shift_up)
        app.send(b"X" + enter + b"cd")
        send_split_escape_sequence(app, up)
        send_split_escape_sequence(app, shift_down)
        app.send(b"Y")
        wait_for_screen_text(app, "Description      abY")

    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(tab)
        send_split_escape_sequence(app, ss3_down)
        send_split_escape_sequence(app, ss3_up)
        send_split_escape_sequence(app, ss3_down)
        app.send(enter + b"SS3 navigation target")
        wait_for_screen_text(app, "Email            SS3 navigation target")

    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(tab * 9)
        send_split_escape_sequence(app, ss3_right)
        send_split_escape_sequence(app, ss3_left)
        send_split_escape_sequence(app, ss3_right)
        app.send(enter)
        wait_for_screen_text(app, "action: cancel")

    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        app.send(tab * 4 + enter + b"ab" + enter + b"cd")
        send_split_escape_sequence(app, ss3_up)
        send_split_escape_sequence(app, ss3_left)
        send_split_escape_sequence(app, ss3_right)
        send_split_escape_sequence(app, ss3_down)
        send_split_escape_sequence(app, ss3_up)
        app.send(b"X")
        wait_for_screen_text(app, "Description      abX")
        wait_for_screen_text(app, "                  cd")

    print("PASS formular split CSI/SS3 arrows navigate and stay inside TextArea", flush=True)


def run_dialog_escape_flash_case(go_command: list[str]) -> None:
    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        row, col = wait_for_screen_text(app, "Clear board")
        app.send(sgr_click(col + 1, row))
        wait_for_screen_text(app, "Delete every task from the board?")
        app.send(b"\x1b[D")
        wait_for_focused_button(app, "Yes")

        app.send_immediate(b"\x1b")
        app.drain(total_seconds=DIALOG_FLASH_CAPTURE_SECONDS)
        if not focused_button_visible(app, "No"):
            raise AssertionError(
                "dialog Escape did not expose the focused No button before close\n"
                + app.screen.text()
            )
        if app.process is None or app.process.poll() is not None:
            raise AssertionError("dialog Escape terminated the Go application")

        wait_for_screen_text(app, "Action canceled.")
        wait_for_screen_text_absent(app, "Delete every task from the board?")
    print("PASS dialog Escape negative-button flash", flush=True)


def run_mouse_click_case(cpp_command: list[str], go_command: list[str]) -> None:
    def exercise(app: PtyApp) -> None:
        row, col = wait_for_screen_text(app, " 1 ")
        app.send(sgr_click(col + 1, row))
        wait_for_screen_text(app, "1")

    cpp_screen = run_dynamic_screen(cpp_command, ROOT, exercise)
    go_screen = run_dynamic_screen(go_command, ROOT, exercise)
    assert_equal_screen("calculator SGR mouse press/release", cpp_screen, go_screen)


def run_mouse_drag_case(cpp_command: list[str], go_command: list[str]) -> None:
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
    go_screen = run_dynamic_screen(go_command, ROOT, exercise)
    assert_equal_screen("widget_gallery SGR mouse drag selection", cpp_screen, go_screen)


def run_explicit_quit_case(go_command: list[str]) -> None:
    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        row, col = wait_for_screen_text(app, "Quit")
        app.send(sgr_click(col + 1, row))
        if app.process is None:
            raise AssertionError("Go image_gallery process did not start")
        try:
            app.process.wait(timeout=DEFAULT_STOP_SECONDS)
        except subprocess.TimeoutExpired as error:
            raise AssertionError("image_gallery Quit did not terminate the Go application") from error
    print("PASS image_gallery explicit Quit", flush=True)


def run_image_browser_held_down_case(go_command: list[str]) -> None:
    with PtyApp(go_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        wait_for_screen_text(app, "Image items")
        app.send(b"\t" * 4)
        app.send(b"\r")
        app.send(b"\x1b[C")
        app.send(b"\r")
        wait_for_screen_text(app, "Camera")

        app.drain(total_seconds=IMAGE_BROWSER_IDLE_CAPTURE_SECONDS)
        idle_start = len(app.output)
        app.drain(total_seconds=IMAGE_BROWSER_IDLE_CAPTURE_SECONDS)
        idle_output = bytes(app.output[idle_start:])
        if idle_output:
            raise AssertionError(
                "idle Go image_browser emitted redundant terminal frames: "
                f"{len(idle_output)} bytes"
            )

        output_start = len(app.output)
        for _ in range(IMAGE_BROWSER_HELD_DOWN_REPEAT_COUNT):
            app.send_immediate(b"\x1b[B")
            time.sleep(SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        app.drain(total_seconds=IMAGE_BROWSER_HELD_DOWN_DRAIN_SECONDS)
        held_output = bytes(app.output[output_start:])
        if len(held_output) > IMAGE_BROWSER_HELD_DOWN_MAX_OUTPUT_BYTES:
            raise AssertionError(
                "held Down produced an unbounded terminal-frame backlog: "
                f"{len(held_output)} bytes, limit "
                f"{IMAGE_BROWSER_HELD_DOWN_MAX_OUTPUT_BYTES}"
            )
        raw_payload_count = held_output.count(b"\x1bPq")
        if raw_payload_count > IMAGE_BROWSER_HELD_DOWN_REPEAT_COUNT:
            raise AssertionError(
                "held Down retransmitted more Sixel payloads than key events: "
                f"{raw_payload_count} payloads for "
                f"{IMAGE_BROWSER_HELD_DOWN_REPEAT_COUNT} events"
            )
        wait_for_screen_text(app, "Sample 19")
    print("PASS image_browser held Down has no idle/full-frame backlog", flush=True)


def run_cpp_image_browser_modal_return_case(cpp_command: list[str]) -> None:
    with PtyApp(cpp_command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        wait_for_screen_text(app, "Image items")
        app.send(b"\t" * 4)
        app.send(b"\r")
        app.send(b"\x1b[C")
        app.send(b"\r")
        wait_for_screen_text(app, "Camera")
        app.send(b"\x1b[B")
        app.send(b"\x1b[C")
        app.send(b"\x1b[C")
        wait_for_focused_button(app, "Browse")
        navigation_start = len(app.output)
        for _ in range(8):
            app.send(b"\x1b[B")

        app.send(b"\r")
        wait_for_screen_text(app, "Open File")
        app.send(b"\x1b")
        open_row = -1
        open_col = -1
        for row, line in enumerate(app.screen.text().splitlines()):
            if "Open" in line and "Close" in line:
                open_row = row
                open_col = line.index("Open")
                break
        if open_row < 0 or open_col < 0:
            raise AssertionError(f"FileBrowser Open button not found\n{app.screen.text()}")
        app.send(sgr_click(open_col + 1, open_row))
        wait_for_screen_text_absent(app, "Open File")
        after_modal = app.screen.text()
        app.send(b"\x1b[A")
        app.send(b"\x1b[B")
        after_round_trip = app.screen.text()
        if "Sample 6" not in after_round_trip:
            raise AssertionError(
                "image_browser Up/Down round trip did not return to the deep "
                f"Browse viewport\n{after_round_trip}"
            )
        normalized_after = re.sub(r"[▀█]", " ", after_modal)
        normalized_round_trip = re.sub(r"[▀█]", " ", after_round_trip)
        if normalized_after != normalized_round_trip:
            navigation_output = bytes(app.output[navigation_start:])
            scroll_sequences = re.findall(rb"\x1b\[[0-9;]*[rST]", navigation_output)
            raise AssertionError(
                "C++ image_browser first terminal frame after FileBrowser close "
                "did not match the same-focus frame after an Up/Down round trip; "
                f"terminal scroll sequences={scroll_sequences!r}\n"
                "--- immediately after modal ---\n"
                f"{after_modal}\n"
                "--- after Up/Down (same Browse focus) ---\n"
                f"{after_round_trip}"
            )
    print("PASS C++ image_browser modal return preserves first terminal frame", flush=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cpp-build-dir", default="cpp/build", type=Path)
    parser.add_argument("--go-examples-dir", default="go/examples", type=Path)
    args = parser.parse_args()

    if os.name != "posix":
        print("SKIP Go direct terminal smoke: POSIX PTY is required", flush=True)
        return 0

    check_binaries(args.cpp_build_dir, args.go_examples_dir)
    cpp_calculator = [str(ROOT / args.cpp_build_dir / "examples/calculator/calculator")]
    cpp_image_browser = [
        "env",
        "UIMD_DETERMINISTIC_IMAGE_FALLBACK=1",
        str(ROOT / args.cpp_build_dir / "examples/image_browser/image_browser"),
    ]
    go_calculator = [str(ROOT / args.go_examples_dir / "calculator/calculator")]
    go_formular = [str(ROOT / args.go_examples_dir / "formular/formular")]
    go_image_gallery = [
        "env",
        "UIMD_DISABLE_SIXEL=1",
        str(ROOT / args.go_examples_dir / "image_gallery/image_gallery"),
    ]
    go_image_browser = [
        "env",
        "UIMD_FORCE_SIXEL=1",
        str(ROOT / args.go_examples_dir / "image_browser/image_browser"),
    ]
    go_task_board = [str(ROOT / args.go_examples_dir / "task_board/task_board")]
    cpp_widget_gallery = [str(ROOT / args.cpp_build_dir / "examples/widget_gallery/widget_gallery")]
    cpp_special_elements = [
        str(ROOT / args.cpp_build_dir / "examples/special_elements/special_elements")
    ]
    go_special_elements = [
        str(ROOT / args.go_examples_dir / "special_elements/special_elements")
    ]
    go_widget_gallery = [str(ROOT / args.go_examples_dir / "widget_gallery/widget_gallery")]

    run_title_and_teardown(go_calculator)
    run_raw_key_case(cpp_calculator, go_calculator)
    run_root_escape_case(go_calculator)
    run_formular_split_arrow_cases(go_formular)
    run_cpp_image_browser_modal_return_case(cpp_image_browser)
    run_image_browser_held_down_case(go_image_browser)
    run_dialog_escape_flash_case(go_task_board)
    run_mouse_click_case(cpp_calculator, go_calculator)
    run_mouse_drag_case(cpp_widget_gallery, go_widget_gallery)
    run_idle_text_gradient_animation(cpp_special_elements, go_special_elements)
    run_explicit_quit_case(go_image_gallery)
    print("PASS Go direct terminal smoke: 11/11 checks passed", flush=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
