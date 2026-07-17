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
DEFAULT_ROWS = 35
DEFAULT_COLS = 90
DEFAULT_START_SECONDS = 1.5
DEFAULT_DRAIN_SECONDS = 1.0
DEFAULT_QUIET_SECONDS = 0.35
DEFAULT_INPUT_DELAY_SECONDS = 0.12
DEFAULT_STOP_SECONDS = 1.0
DEFAULT_TITLE_SECONDS = 5.0
TERMINAL_COORDINATE_BASE = 1


class TerminalScreen:
    def __init__(self, rows: int, cols: int) -> None:
        self.rows = rows
        self.cols = cols
        self.row = 0
        self.col = 0
        self.pending = ""
        self.title = ""
        self.grid = [[" "] * cols for _ in range(rows)]

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


def check_binaries(cpp_build_dir: Path, go_examples_dir: Path) -> None:
    paths = [
        ROOT / cpp_build_dir / "examples/calculator/calculator",
        ROOT / cpp_build_dir / "examples/widget_gallery/widget_gallery",
        ROOT / go_examples_dir / "calculator/calculator",
        ROOT / go_examples_dir / "widget_gallery/widget_gallery",
    ]
    missing = [path for path in paths if not path.exists()]
    if missing:
        details = "\n".join(f"  - {path.relative_to(ROOT)}" for path in missing)
        raise FileNotFoundError(f"missing Go direct-terminal smoke binary:\n{details}")


def run_dynamic_screen(command: list[str], cwd: Path, exercise: Callable[[PtyApp], None]) -> str:
    with PtyApp(command, cwd, DEFAULT_ROWS, DEFAULT_COLS) as app:
        exercise(app)
        app.drain()
        return app.screen.text()


def assert_equal_screen(name: str, cpp_screen: str, go_screen: str) -> None:
    if cpp_screen == go_screen:
        print(f"PASS {name}", flush=True)
        return
    print(f"FAIL {name}", flush=True)
    print("--- C++ ---", flush=True)
    print(cpp_screen, flush=True)
    print("--- Go ---", flush=True)
    print(go_screen, flush=True)
    raise AssertionError(f"{name}: Go direct terminal screen differs from C++")


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
    go_calculator = [str(ROOT / args.go_examples_dir / "calculator/calculator")]
    cpp_widget_gallery = [str(ROOT / args.cpp_build_dir / "examples/widget_gallery/widget_gallery")]
    go_widget_gallery = [str(ROOT / args.go_examples_dir / "widget_gallery/widget_gallery")]

    run_title_and_teardown(go_calculator)
    run_raw_key_case(cpp_calculator, go_calculator)
    run_mouse_click_case(cpp_calculator, go_calculator)
    run_mouse_drag_case(cpp_widget_gallery, go_widget_gallery)
    return 0


if __name__ == "__main__":
    sys.exit(main())
