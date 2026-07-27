#!/usr/bin/env python3
"""Direct PTY smoke tests for Swift terminal runtime parity with C++."""

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
DEFAULT_QUIET_SECONDS = 0.35
DEFAULT_DRAIN_SECONDS = 1.0
DEFAULT_START_SECONDS = 1.5
DEFAULT_TITLE_SECONDS = 5.0
DEFAULT_ANIMATION_SECONDS = 0.5
DEFAULT_INPUT_DELAY_SECONDS = 0.12
DEFAULT_STOP_SECONDS = 1.0
SPLIT_ESCAPE_SEQUENCE_DELAY_SECONDS = 0.003
SPLIT_ARROW_SEQUENCE_DELAY_SECONDS = 0.03
TERMINAL_COORDINATE_BASE = 1
ACTIVITY_FEED_WHEEL_X = 20
ACTIVITY_FEED_WHEEL_Y = 4
ACTIVITY_FEED_TIMESTAMP = "00:00"
IMAGE_BROWSER_FALLBACK_ROWS = 62
IMAGE_BROWSER_FALLBACK_COLS = 176
IMAGE_BROWSER_SIXEL_MODAL_ROWS = 35
IMAGE_BROWSER_SIXEL_MODAL_COLS = 90
TASK_BOARD_SCROLL_BOTTOM_ROWS = 62
TASK_BOARD_SCROLL_BOTTOM_COLS = 176
TASK_BOARD_SCROLL_BOTTOM_X = 120
TASK_BOARD_SCROLL_BOTTOM_Y = 35
TASK_BOARD_SCROLL_BOTTOM_BATCH_SIZE = 12
TASK_BOARD_SCROLL_BOTTOM_BATCHES = 2
FAKE_CLIPBOARD_DIR_NAME = "uimd-swift-direct-terminal-clipboard"
FAKE_PBCOPY_SCRIPT = "#!/bin/sh\ncat >/dev/null\n"
FAKE_PBCOPY_MODE = 0o755


class TerminalScreen:
    def __init__(self, rows: int, cols: int) -> None:
        self.rows = rows
        self.cols = cols
        self.row = 0
        self.col = 0
        self.pending = ""
        self.title = ""
        self.grid = [[" "] * cols for _ in range(rows)]
        self.foreground = [[None] * cols for _ in range(rows)]
        self.background = [[None] * cols for _ in range(rows)]
        self.current_foreground = None
        self.current_background = None

    def clear(self) -> None:
        self.grid = [[" "] * self.cols for _ in range(self.rows)]
        self.foreground = [[None] * self.cols for _ in range(self.rows)]
        self.background = [[None] * self.cols for _ in range(self.rows)]
        self.row = 0
        self.col = 0

    def clone(self) -> "TerminalScreen":
        result = TerminalScreen(self.rows, self.cols)
        result.row = self.row
        result.col = self.col
        result.pending = self.pending
        result.title = self.title
        result.grid = [row[:] for row in self.grid]
        result.foreground = [row[:] for row in self.foreground]
        result.background = [row[:] for row in self.background]
        result.current_foreground = self.current_foreground
        result.current_background = self.current_background
        return result

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

    def styled_cells(self) -> tuple[tuple[tuple[str, object, object], ...], ...]:
        return tuple(
            tuple(
                (self.grid[row][col], self.foreground[row][col], self.background[row][col])
                for col in range(self.cols)
            )
            for row in range(self.rows)
        )

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
            self._handle_osc(payload)
            return end + (2 if text[end:end + 2] == "\x1b\\" else 1)
        if prefix == "P":
            end = index + 2
            while end < len(text) and text[end:end + 2] != "\x1b\\":
                end += 1
            if end >= len(text):
                return None
            return end + 2
        return index + 2

    def _handle_osc(self, payload: str) -> None:
        if payload.startswith("0;") or payload.startswith("2;"):
            self.title = payload[2:]

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
            self.foreground[self.row][self.col] = self.current_foreground
            self.background[self.row][self.col] = self.current_background
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
        elif final == "J":
            self._erase_screen(value(0, 0))
        elif final == "K":
            self._erase_line(value(0, 0))
        elif final == "m":
            self._handle_sgr(values)

    def _handle_sgr(self, values: list[int | None]) -> None:
        if not values:
            values = [0]
        index = 0
        while index < len(values):
            code = 0 if values[index] is None else values[index]
            if code == 0:
                self.current_foreground = None
                self.current_background = None
                index += 1
                continue
            if code == 39:
                self.current_foreground = None
                index += 1
                continue
            if code == 49:
                self.current_background = None
                index += 1
                continue
            if code in (38, 48) and index + 4 < len(values) and values[index + 1] == 2:
                red = values[index + 2]
                green = values[index + 3]
                blue = values[index + 4]
                if red is not None and green is not None and blue is not None:
                    color = (red, green, blue)
                    if code == 38:
                        self.current_foreground = color
                    else:
                        self.current_background = color
                index += 5
                continue
            index += 1

    def _erase_screen(self, mode: int) -> None:
        if mode in (2, 3):
            self.clear()
            return
        if mode == 0:
            for row in range(self.row, self.rows):
                start_col = self.col if row == self.row else 0
                for col in range(start_col, self.cols):
                    self.grid[row][col] = " "
                    self.foreground[row][col] = self.current_foreground
                    self.background[row][col] = self.current_background

    def _erase_line(self, mode: int) -> None:
        if mode == 0:
            columns = range(self.col, self.cols)
        elif mode == 1:
            columns = range(0, self.col + 1)
        elif mode == 2:
            columns = range(0, self.cols)
        else:
            return
        for col in columns:
            self.grid[self.row][col] = " "
            self.foreground[self.row][col] = self.current_foreground
            self.background[self.row][col] = self.current_background

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
    def __init__(self, command: list[str], cwd: Path, rows: int, cols: int, env: dict[str, str]) -> None:
        self.command = command
        self.cwd = cwd
        self.rows = rows
        self.cols = cols
        self.env = env
        self.master_fd: int | None = None
        self.process: subprocess.Popen[bytes] | None = None
        self.screen = TerminalScreen(rows, cols)
        self.output = bytearray()

    def __enter__(self) -> "PtyApp":
        master_fd, slave_fd = pty.openpty()
        self._set_pty_size(master_fd)
        self._set_pty_size(slave_fd)
        process_env = os.environ.copy()
        process_env.update(self.env)
        process_env.setdefault("TERM", "xterm-256color")
        self.process = subprocess.Popen(
            self.command,
            cwd=self.cwd,
            stdin=slave_fd,
            stdout=slave_fd,
            stderr=slave_fd,
            env=process_env,
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
        if self.process.poll() is None:
            if self.master_fd is not None:
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


def sgr_wheel(button: int, x: int, y: int) -> bytes:
    return f"\x1b[<{button};{x + TERMINAL_COORDINATE_BASE};{y + TERMINAL_COORDINATE_BASE}M".encode()


def sgr_press(button: int, x: int, y: int) -> bytes:
    return f"\x1b[<{button};{x + TERMINAL_COORDINATE_BASE};{y + TERMINAL_COORDINATE_BASE}M".encode()


def sgr_drag(x: int, y: int) -> bytes:
    return sgr_press(32, x, y)


def sgr_release(button: int, x: int, y: int) -> bytes:
    return f"\x1b[<{button};{x + TERMINAL_COORDINATE_BASE};{y + TERMINAL_COORDINATE_BASE}m".encode()


def sgr_click(x: int, y: int, button: int = 0) -> bytes:
    return sgr_press(button, x, y) + sgr_release(button, x, y)


def send_split_escape_sequence(
    app: PtyApp,
    sequence: bytes,
    delay_seconds: float = SPLIT_ESCAPE_SEQUENCE_DELAY_SECONDS,
) -> None:
    if app.master_fd is None:
        raise RuntimeError("PTY app is not running")
    if not sequence.startswith(b"\x1b"):
        raise ValueError("split escape sequence must start with ESC")
    os.write(app.master_fd, sequence[:1])
    time.sleep(delay_seconds)
    os.write(app.master_fd, sequence[1:])
    time.sleep(DEFAULT_INPUT_DELAY_SECONDS)
    app.drain()


def wait_for_screen_text(app: PtyApp, needle: str, timeout_seconds: float = DEFAULT_TITLE_SECONDS) -> tuple[int, int]:
    deadline = time.monotonic() + timeout_seconds
    last_error: AssertionError | None = None
    while time.monotonic() < deadline:
        try:
            return app.screen.position_of(needle)
        except AssertionError as exc:
            last_error = exc
            app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
    if last_error is not None:
        raise last_error
    raise AssertionError(f"screen text not found: {needle!r}")


def app_specs(cpp_build_dir: Path, swift_examples_dir: Path) -> dict[str, tuple[list[str], Path, list[str], Path]]:
    return {
        "activity_feed": (
            [str(ROOT / cpp_build_dir / "examples/activity_feed/activity_feed")],
            ROOT,
            [str(ROOT / swift_examples_dir / "activity_feed/.build/debug/activity_feed")],
            ROOT,
        ),
        "formular": (
            [str(ROOT / cpp_build_dir / "examples/formular/formular")],
            ROOT,
            [str(ROOT / swift_examples_dir / "formular/.build/debug/formular")],
            ROOT,
        ),
        "image_browser": (
            [str(ROOT / cpp_build_dir / "examples/image_browser/image_browser")],
            ROOT,
            [str(ROOT / swift_examples_dir / "image_browser/.build/debug/image_browser")],
            ROOT,
        ),
        "image_gallery": (
            [str(ROOT / cpp_build_dir / "examples/image_gallery/image_gallery")],
            ROOT,
            [str(ROOT / swift_examples_dir / "image_gallery/.build/debug/image_gallery")],
            ROOT,
        ),
        "expense_tracker": (
            [str(ROOT / cpp_build_dir / "examples/expense_tracker/expense_tracker")],
            ROOT,
            [str(ROOT / swift_examples_dir / "expense_tracker/.build/debug/expense_tracker")],
            ROOT,
        ),
        "task_board": (
            [str(ROOT / cpp_build_dir / "examples/task_board/task_board")],
            ROOT,
            [str(ROOT / swift_examples_dir / "task_board/.build/debug/task_board")],
            ROOT,
        ),
        "widget_gallery": (
            [str(ROOT / cpp_build_dir / "examples/widget_gallery/widget_gallery")],
            ROOT,
            [str(ROOT / swift_examples_dir / "widget_gallery/.build/debug/widget_gallery")],
            ROOT,
        ),
    }


def fake_clipboard_env() -> dict[str, str]:
    directory = Path(os.environ.get("TMPDIR", "/tmp")) / FAKE_CLIPBOARD_DIR_NAME
    directory.mkdir(parents=True, exist_ok=True)
    pbcopy = directory / "pbcopy"
    if not pbcopy.exists() or pbcopy.read_text(encoding="utf-8") != FAKE_PBCOPY_SCRIPT:
        pbcopy.write_text(FAKE_PBCOPY_SCRIPT, encoding="utf-8")
    pbcopy.chmod(FAKE_PBCOPY_MODE)
    path = f"{directory}{os.pathsep}{os.environ.get('PATH', '')}"
    return {"PATH": path}


def check_binaries(specs: dict[str, tuple[list[str], Path, list[str], Path]]) -> None:
    missing: list[Path] = []
    for cpp_command, _cpp_cwd, swift_command, swift_cwd in specs.values():
        cpp_path = Path(cpp_command[0])
        swift_path = Path(swift_command[0])
        if not cpp_path.exists():
            missing.append(cpp_path)
        if not swift_path.exists():
            missing.append(swift_path)
    if missing:
        details = "\n".join(f"  - {path.relative_to(ROOT)}" for path in missing)
        raise FileNotFoundError(f"missing direct-terminal smoke binary:\n{details}")


def run_screen(command: list[str], cwd: Path, rows: int, cols: int, inputs: bytes, env: dict[str, str]) -> str:
    with PtyApp(command, cwd, rows, cols, env) as app:
        app.send(inputs)
        return app.screen.text()


def run_dynamic_screen(
    command: list[str],
    cwd: Path,
    rows: int,
    cols: int,
    exercise: Callable[[PtyApp], None],
    env: dict[str, str],
) -> str:
    with PtyApp(command, cwd, rows, cols, env) as app:
        exercise(app)
        app.drain()
        return app.screen.text()


def run_dynamic_terminal_screen(
    command: list[str],
    cwd: Path,
    rows: int,
    cols: int,
    exercise: Callable[[PtyApp], None],
    env: dict[str, str],
) -> TerminalScreen:
    with PtyApp(command, cwd, rows, cols, env) as app:
        exercise(app)
        app.drain()
        return app.screen.clone()


def run_swift_title_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
    app: str,
    rows: int,
    cols: int,
) -> None:
    _cpp_command, _cpp_cwd, swift_command, swift_cwd = specs[app]
    env = {"UIMD_ACTIVITY_FEED_TIMESTAMP": ACTIVITY_FEED_TIMESTAMP}
    with PtyApp(swift_command, swift_cwd, rows, cols, env) as swift_app:
        deadline = time.monotonic() + DEFAULT_TITLE_SECONDS
        while not swift_app.screen.title.endswith(" [swift]") and time.monotonic() < deadline:
            swift_app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        if not swift_app.screen.title.endswith(" [swift]"):
            raise AssertionError(f"{name}: Swift terminal title is {swift_app.screen.title!r}")
    print(f"PASS {name}", flush=True)


def run_swift_dynamic_presence_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
    app: str,
    exercise: Callable[[PtyApp], None],
    expected: list[str],
    rows: int,
    cols: int,
    env_extra: dict[str, str] | None = None,
) -> None:
    _cpp_command, _cpp_cwd, swift_command, swift_cwd = specs[app]
    env = {"UIMD_ACTIVITY_FEED_TIMESTAMP": ACTIVITY_FEED_TIMESTAMP}
    if env_extra:
        env.update(env_extra)
    swift_screen = run_dynamic_screen(swift_command, swift_cwd, rows, cols, exercise, env)
    assert_contains(f"{name} Swift", swift_screen, expected)
    print(f"PASS {name}", flush=True)


def assert_equal_screen(name: str, cpp_screen: str, swift_screen: str) -> None:
    if cpp_screen == swift_screen:
        print(f"PASS {name}", flush=True)
        return
    print(f"FAIL {name}", flush=True)
    print("--- C++ ---", flush=True)
    print(cpp_screen, flush=True)
    print("--- Swift ---", flush=True)
    print(swift_screen, flush=True)
    raise AssertionError(f"{name}: Swift direct terminal screen differs from C++")


def assert_equal_styled_screen(name: str, cpp_screen: TerminalScreen, swift_screen: TerminalScreen) -> None:
    cpp_cells = cpp_screen.styled_cells()
    swift_cells = swift_screen.styled_cells()
    if cpp_cells == swift_cells:
        print(f"PASS {name}", flush=True)
        return
    print(f"FAIL {name}", flush=True)
    for row in range(min(cpp_screen.rows, swift_screen.rows)):
        for col in range(min(cpp_screen.cols, swift_screen.cols)):
            if cpp_cells[row][col] != swift_cells[row][col]:
                print(
                    f"first styled cell diff at row={row} col={col}: "
                    f"C++={cpp_cells[row][col]!r} Swift={swift_cells[row][col]!r}",
                    flush=True,
                )
                break
        else:
            continue
        break
    print("--- C++ text ---", flush=True)
    print(cpp_screen.text(), flush=True)
    print("--- Swift text ---", flush=True)
    print(swift_screen.text(), flush=True)
    raise AssertionError(f"{name}: Swift direct terminal styled screen differs from C++")


def assert_text_row_positions(name: str, screen: TerminalScreen, expected: dict[str, int]) -> None:
    text = screen.text().splitlines()
    for needle, expected_row in expected.items():
        for row, line in enumerate(text):
            if needle in line:
                if row != expected_row:
                    raise AssertionError(
                        f"{name}: {needle!r} rendered on row {row}, expected {expected_row}"
                    )
                break
        else:
            raise AssertionError(f"{name}: expected screen text missing: {needle!r}")


def assert_contains(name: str, screen: str, expected: list[str]) -> None:
    missing = [text for text in expected if text not in screen]
    if not missing:
        return
    details = ", ".join(repr(text) for text in missing)
    raise AssertionError(f"{name}: expected screen text missing: {details}")


def sixel_payload_signatures(data: bytes) -> list[tuple[tuple[int, int] | None, str | None]]:
    signatures: list[tuple[tuple[int, int] | None, str | None]] = []
    position = 0
    while True:
        start = data.find(b"\x1bP", position)
        if start < 0:
            break
        end = data.find(b"\x1b\\", start)
        if end < 0:
            break
        before = data[:start]
        anchor: tuple[int, int] | None = None
        cursor_start = before.rfind(b"\x1b[")
        if cursor_start >= 0 and before.endswith(b"H"):
            try:
                params = before[cursor_start + 2:-1].decode("ascii").split(";")
                anchor = (int(params[0]), int(params[1]))
            except (ValueError, UnicodeDecodeError):
                anchor = None
        payload = data[start:end + 2]
        raster_header: str | None = None
        header_start = payload.find(b"\"")
        if header_start >= 0:
            header_end = payload.find(b"#", header_start)
            if header_end < 0:
                header_end = min(len(payload), header_start + 80)
            raster_header = payload[header_start:header_end].decode("ascii", "ignore")
        signatures.append((anchor, raster_header))
        position = end + 2
    return signatures


def run_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
    app: str,
    inputs: bytes,
    expected: list[str],
    rows: int,
    cols: int,
) -> None:
    cpp_command, cpp_cwd, swift_command, swift_cwd = specs[app]
    env = {"UIMD_ACTIVITY_FEED_TIMESTAMP": ACTIVITY_FEED_TIMESTAMP}
    cpp_screen = run_screen(cpp_command, cpp_cwd, rows, cols, inputs, env)
    swift_screen = run_screen(swift_command, swift_cwd, rows, cols, inputs, env)
    assert_contains(f"{name} C++", cpp_screen, expected)
    assert_contains(f"{name} Swift", swift_screen, expected)
    assert_equal_screen(name, cpp_screen, swift_screen)


def run_dynamic_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
    app: str,
    exercise: Callable[[PtyApp], None],
    expected: list[str],
    rows: int,
    cols: int,
) -> None:
    cpp_command, cpp_cwd, swift_command, swift_cwd = specs[app]
    env = {"UIMD_ACTIVITY_FEED_TIMESTAMP": ACTIVITY_FEED_TIMESTAMP}
    cpp_screen = run_dynamic_screen(cpp_command, cpp_cwd, rows, cols, exercise, env)
    swift_screen = run_dynamic_screen(swift_command, swift_cwd, rows, cols, exercise, env)
    assert_contains(f"{name} C++", cpp_screen, expected)
    assert_contains(f"{name} Swift", swift_screen, expected)
    assert_equal_screen(name, cpp_screen, swift_screen)


def run_dynamic_presence_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
    app: str,
    exercise: Callable[[PtyApp], None],
    expected: list[str],
    rows: int,
    cols: int,
    env_extra: dict[str, str] | None = None,
) -> None:
    cpp_command, cpp_cwd, swift_command, swift_cwd = specs[app]
    env = {"UIMD_ACTIVITY_FEED_TIMESTAMP": ACTIVITY_FEED_TIMESTAMP}
    if env_extra:
        env.update(env_extra)
    cpp_screen = run_dynamic_screen(cpp_command, cpp_cwd, rows, cols, exercise, env)
    swift_screen = run_dynamic_screen(swift_command, swift_cwd, rows, cols, exercise, env)
    assert_contains(f"{name} C++", cpp_screen, expected)
    assert_contains(f"{name} Swift", swift_screen, expected)
    print(f"PASS {name}", flush=True)


def run_dynamic_styled_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
    app: str,
    exercise: Callable[[PtyApp], None],
    expected: list[str],
    rows: int,
    cols: int,
    env_extra: dict[str, str] | None = None,
) -> None:
    cpp_command, cpp_cwd, swift_command, swift_cwd = specs[app]
    env = {"UIMD_ACTIVITY_FEED_TIMESTAMP": ACTIVITY_FEED_TIMESTAMP}
    if env_extra:
        env.update(env_extra)
    cpp_screen = run_dynamic_terminal_screen(cpp_command, cpp_cwd, rows, cols, exercise, env)
    swift_screen = run_dynamic_terminal_screen(swift_command, swift_cwd, rows, cols, exercise, env)
    assert_contains(f"{name} C++", cpp_screen.text(), expected)
    assert_contains(f"{name} Swift", swift_screen.text(), expected)
    assert_equal_styled_screen(name, cpp_screen, swift_screen)


def run_image_browser_sixel_modal_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
) -> None:
    cpp_command, cpp_cwd, swift_command, swift_cwd = specs["image_browser"]
    env = {
        "TERM_PROGRAM": "iTerm.app",
        "ITERM_SESSION_ID": "uimd-test",
        "LC_TERMINAL": "iTerm2",
        "UIMD_FORCE_SIXEL": "1",
        "UIMD_DISABLE_SIXEL": "",
    }

    def capture(command: list[str], cwd: Path) -> tuple[list[tuple[tuple[int, int] | None, str | None]], str]:
        with PtyApp(command, cwd, IMAGE_BROWSER_SIXEL_MODAL_ROWS, IMAGE_BROWSER_SIXEL_MODAL_COLS, env) as app:
            row, col = wait_for_screen_text(app, "Image items")
            app.send(sgr_click(col + 2, row + 1))
            row, col = wait_for_screen_text(app, "Show")
            start = len(app.output)
            app.send(sgr_click(col, row))
            wait_for_screen_text(app, "Close")
            deadline = time.monotonic() + DEFAULT_TITLE_SECONDS
            signatures: list[tuple[tuple[int, int] | None, str | None]] = []
            while time.monotonic() < deadline:
                app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
                signatures = sixel_payload_signatures(bytes(app.output)[start:])
                if signatures:
                    break
            return signatures, app.screen.text()

    cpp_signatures, cpp_screen = capture(cpp_command, cpp_cwd)
    swift_signatures, swift_screen = capture(swift_command, swift_cwd)
    assert_contains(f"{name} C++", cpp_screen, ["Close"])
    assert_contains(f"{name} Swift", swift_screen, ["Close"])
    if not cpp_signatures:
        raise AssertionError(f"{name}: C++ did not emit Sixel payloads for image dialog")
    if cpp_signatures != swift_signatures:
        raise AssertionError(
            f"{name}: Swift Sixel dialog payloads differ from C++: "
            f"C++={cpp_signatures!r} Swift={swift_signatures!r}"
        )
    print(f"PASS {name}", flush=True)


def assert_idle_output_grows(name: str, command: list[str], cwd: Path, rows: int, cols: int, env: dict[str, str]) -> None:
    with PtyApp(command, cwd, rows, cols, env) as app:
        before = len(app.output)
        app.drain(total_seconds=DEFAULT_ANIMATION_SECONDS)
        after = len(app.output)
        if after <= before:
            raise AssertionError(f"{name}: no terminal output was emitted while idle")


def run_animation_idle_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
    app: str,
    rows: int,
    cols: int,
) -> None:
    cpp_command, cpp_cwd, swift_command, swift_cwd = specs[app]
    env = {"UIMD_ACTIVITY_FEED_TIMESTAMP": ACTIVITY_FEED_TIMESTAMP}
    assert_idle_output_grows(f"{name} C++", cpp_command, cpp_cwd, rows, cols, env)
    assert_idle_output_grows(f"{name} Swift", swift_command, swift_cwd, rows, cols, env)
    print(f"PASS {name}", flush=True)


def run_task_board_repeated_wheel_bottom_case(
    name: str,
    specs: dict[str, tuple[list[str], Path, list[str], Path]],
) -> None:
    cpp_command, cpp_cwd, swift_command, swift_cwd = specs["task_board"]
    env = {"UIMD_ACTIVITY_FEED_TIMESTAMP": ACTIVITY_FEED_TIMESTAMP}
    wheel_down = sgr_wheel(65, TASK_BOARD_SCROLL_BOTTOM_X, TASK_BOARD_SCROLL_BOTTOM_Y)

    def exercise(app: PtyApp) -> None:
        for _ in range(TASK_BOARD_SCROLL_BOTTOM_BATCHES):
            if app.master_fd is None:
                raise RuntimeError("PTY app is not running")
            os.write(app.master_fd, wheel_down * TASK_BOARD_SCROLL_BOTTOM_BATCH_SIZE)
            time.sleep(DEFAULT_INPUT_DELAY_SECONDS)
            app.drain(total_seconds=DEFAULT_DRAIN_SECONDS)

    expected_rows = {
        "Draft data deletion policy": 8,
        "Update support templates": 13,
        "Publish usage report": 43,
        "Retry failed imports": 48,
        "Tune digest settings": 53,
        "Verify account password reset cleanup": 58,
    }
    cpp_screen = run_dynamic_terminal_screen(
        cpp_command,
        cpp_cwd,
        TASK_BOARD_SCROLL_BOTTOM_ROWS,
        TASK_BOARD_SCROLL_BOTTOM_COLS,
        exercise,
        env,
    )
    swift_screen = run_dynamic_terminal_screen(
        swift_command,
        swift_cwd,
        TASK_BOARD_SCROLL_BOTTOM_ROWS,
        TASK_BOARD_SCROLL_BOTTOM_COLS,
        exercise,
        env,
    )
    assert_text_row_positions(f"{name} C++", cpp_screen, expected_rows)
    assert_text_row_positions(f"{name} Swift", swift_screen, expected_rows)
    assert_equal_screen(name, cpp_screen.text(), swift_screen.text())


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cpp-build-dir", default="cpp/build")
    parser.add_argument("--swift-examples-dir", default="swift/examples")
    parser.add_argument("--rows", type=int, default=DEFAULT_ROWS)
    parser.add_argument("--cols", type=int, default=DEFAULT_COLS)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if os.name == "nt":
        print("SKIP Swift direct terminal smoke: POSIX PTY is required", flush=True)
        return 0

    specs = app_specs(Path(args.cpp_build_dir), Path(args.swift_examples_dir))
    check_binaries(specs)

    tab = b"\t"
    shift_tab = b"\x1b[Z"
    enter = b"\r"
    up = b"\x1b[A"
    down = b"\x1b[B"
    left = b"\x1b[D"
    right = b"\x1b[C"
    shift_up = b"\x1b[1;2A"
    shift_down = b"\x1b[1;2B"
    shift_left = b"\x1b[1;2D"
    escape = b"\x1b"
    legacy_cmd_c = b"\x1b[27;9;99~"
    legacy_cmd_v = b"\x1b[27;9;118~"
    wheel_up = sgr_wheel(64, ACTIVITY_FEED_WHEEL_X, ACTIVITY_FEED_WHEEL_Y)
    wheel_down = sgr_wheel(65, ACTIVITY_FEED_WHEEL_X, ACTIVITY_FEED_WHEEL_Y)

    run_swift_title_case(
        "activity_feed terminal title uses swift suffix",
        specs,
        "activity_feed",
        args.rows,
        args.cols,
    )

    def exercise_formular_split_navigation_arrow(app: PtyApp) -> None:
        app.send(tab)
        send_split_escape_sequence(app, down, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, up, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, down, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        app.send(enter + b"navigation target")

    run_swift_dynamic_presence_case(
        "formular split arrow navigates between elements",
        specs,
        "formular",
        exercise_formular_split_navigation_arrow,
        ["Email            navigation target"],
        args.rows,
        args.cols,
    )

    def exercise_formular_split_horizontal_navigation_arrows(app: PtyApp) -> None:
        app.send(tab * 9)
        send_split_escape_sequence(app, right, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, left, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, right, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        app.send(enter)

    run_swift_dynamic_presence_case(
        "formular split horizontal arrows navigate between buttons",
        specs,
        "formular",
        exercise_formular_split_horizontal_navigation_arrows,
        ["action: cancel"],
        args.rows,
        args.cols,
    )
    run_animation_idle_case(
        "widget_gallery animation renders while idle",
        specs,
        "widget_gallery",
        args.rows,
        args.cols,
    )
    run_case(
        "formular combobox enter opens dropdown",
        specs,
        "formular",
        tab * 5 + enter,
        ["Czech Republic", "Austria"],
        args.rows,
        args.cols,
    )
    run_case(
        "formular textarea enter inserts newline",
        specs,
        "formular",
        tab * 4 + enter + b"a" + enter + b"b",
        ["Description      a", "                  b"],
        args.rows,
        args.cols,
    )
    run_case(
        "formular textinput shift selection replaces selected text",
        specs,
        "formular",
        tab + enter + b"abcd" + shift_left * 2 + b"Z",
        ["Name             abZ"],
        args.rows,
        args.cols,
    )

    def exercise_formular_textinput_legacy_cmd_copy(app: PtyApp) -> None:
        app.send(tab + enter + b"abcd" + shift_left * 2)
        app.send(legacy_cmd_c)
        app.send(enter + tab + enter)
        app.send(legacy_cmd_v)

    run_swift_dynamic_presence_case(
        "formular textinput legacy cmd copy paste",
        specs,
        "formular",
        exercise_formular_textinput_legacy_cmd_copy,
        ["Email            cd"],
        args.rows,
        args.cols,
    )

    def exercise_formular_textinput_legacy_cmd_copy_notification(app: PtyApp) -> None:
        app.send(tab + enter + b"abcd" + shift_left * 2)
        app.send(legacy_cmd_c)

    run_swift_dynamic_presence_case(
        "formular textinput legacy cmd copy notification",
        specs,
        "formular",
        exercise_formular_textinput_legacy_cmd_copy_notification,
        ["Copied to clipboard"],
        args.rows,
        args.cols,
        env_extra=fake_clipboard_env(),
    )

    def exercise_formular_textinput_split_legacy_cmd_copy(app: PtyApp) -> None:
        app.send(tab + enter + b"uv" + shift_left * 2)
        send_split_escape_sequence(app, legacy_cmd_c)
        app.send(enter + tab + enter)
        send_split_escape_sequence(app, legacy_cmd_v)

    run_swift_dynamic_presence_case(
        "formular textinput split legacy cmd copy paste",
        specs,
        "formular",
        exercise_formular_textinput_split_legacy_cmd_copy,
        ["Email            uv"],
        args.rows,
        args.cols,
    )

    run_case(
        "formular textarea arrow up edits previous line",
        specs,
        "formular",
        tab * 4 + enter + b"a" + enter + b"b" + up + b"X",
        ["Description      aX", "                  b"],
        args.rows,
        args.cols,
    )

    def exercise_formular_textarea_split_arrow(app: PtyApp) -> None:
        app.send(tab * 4 + enter + b"ab" + enter + b"cd")
        send_split_escape_sequence(app, up, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, left, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, right, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, down, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, up, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        app.send(b"X")

    run_swift_dynamic_presence_case(
        "formular textarea split arrow stays in edit mode",
        specs,
        "formular",
        exercise_formular_textarea_split_arrow,
        ["Description      abX", "                  cd"],
        args.rows,
        args.cols,
    )

    def exercise_formular_textarea_split_shift_arrows(app: PtyApp) -> None:
        app.send(tab * 4 + enter + b"ab" + enter + b"cd")
        send_split_escape_sequence(app, shift_up, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        app.send(b"X" + enter + b"cd")
        send_split_escape_sequence(app, up, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        send_split_escape_sequence(app, shift_down, SPLIT_ARROW_SEQUENCE_DELAY_SECONDS)
        app.send(b"Y")

    run_swift_dynamic_presence_case(
        "formular textarea split shifted arrows preserve selection editing",
        specs,
        "formular",
        exercise_formular_textarea_split_shift_arrows,
        ["Description      abY"],
        args.rows,
        args.cols,
    )

    def exercise_formular_textarea_legacy_cmd_copy(app: PtyApp) -> None:
        app.send(tab * 4 + enter + b"a" + enter + b"bc" + shift_left * 2)
        app.send(legacy_cmd_c)
        app.send(escape)
        app.send(shift_tab * 2 + enter)
        app.send(legacy_cmd_v)

    run_swift_dynamic_presence_case(
        "formular textarea legacy cmd copy paste",
        specs,
        "formular",
        exercise_formular_textarea_legacy_cmd_copy,
        ["Email            bc"],
        args.rows,
        args.cols,
    )
    run_case(
        "formular textarea shift selection replaces selected text",
        specs,
        "formular",
        tab * 4 + enter + b"a" + enter + b"bc" + shift_left * 2 + b"Z",
        ["Description      a", "                  Z"],
        args.rows,
        args.cols,
    )
    run_case(
        "formular combobox arrow menu selects next option",
        specs,
        "formular",
        tab * 5 + enter + down + enter,
        ["Czech Republic"],
        args.rows,
        args.cols,
    )
    run_case(
        "activity_feed mouse wheel scroll",
        specs,
        "activity_feed",
        tab * 7 + enter + wheel_up * 4 + wheel_down * 4,
        ["8 activities", "Background sync finished"],
        args.rows,
        args.cols,
    )
    run_case(
        "activity_feed arrow scroll scope",
        specs,
        "activity_feed",
        tab * 7 + enter + shift_tab * 4 + enter + up * 4 + down * 4,
        ["8 activities", "Background sync finished"],
        args.rows,
        args.cols,
    )

    def accept_sixel_warning(app: PtyApp, timeout_seconds: float = DEFAULT_TITLE_SECONDS) -> bool:
        try:
            wait_for_screen_text(app, "Sixel is not supported", timeout_seconds=timeout_seconds)
        except AssertionError:
            return False
        row, col = wait_for_screen_text(app, "Yes")
        app.send(sgr_click(col, row))
        return True

    def exercise_image_gallery_sixel_warning(app: PtyApp) -> None:
        wait_for_screen_text(app, "Sixel is not supported")
        wait_for_screen_text(app, "Yes")
        wait_for_screen_text(app, "No")

    run_dynamic_presence_case(
        "image_gallery sixel fallback warning message box",
        specs,
        "image_gallery",
        exercise_image_gallery_sixel_warning,
        ["Sixel is not supported", "Yes", "No"],
        args.rows,
        args.cols,
        {
            "TERM_PROGRAM": "Apple_Terminal",
            "ITERM_SESSION_ID": "",
            "LC_TERMINAL": "",
            "UIMD_DISABLE_SIXEL": "",
            "UIMD_FORCE_SIXEL": "",
        },
    )

    def exercise_image_browser_fallback(app: PtyApp) -> None:
        accept_sixel_warning(app, timeout_seconds=1.0)
        row, col = wait_for_screen_text(app, "Normal")
        app.send(sgr_click(col, row))
        row, col = wait_for_screen_text(app, "Fallback")
        app.send(sgr_click(col, row))

    run_dynamic_styled_case(
        "image_browser fallback styled screen parity",
        specs,
        "image_browser",
        exercise_image_browser_fallback,
        ["Image Browser", "Fallback", "Camera"],
        max(args.rows, IMAGE_BROWSER_FALLBACK_ROWS),
        max(args.cols, IMAGE_BROWSER_FALLBACK_COLS),
        {"UIMD_DISABLE_SIXEL": "1"},
    )
    run_image_browser_sixel_modal_case(
        "image_browser top modal emits clipped sixel payload",
        specs,
    )

    def exercise_activity_feed_settings_checkbox_arrows(app: PtyApp) -> None:
        app.send(tab + enter)
        app.send(tab)
        app.send(down)
        app.send(down)
        app.send(enter)

    run_dynamic_case(
        "activity_feed settings checkbox arrows reach combobox",
        specs,
        "activity_feed",
        exercise_activity_feed_settings_checkbox_arrows,
        ["Task", "Warning", "Deploy"],
        args.rows,
        args.cols,
    )

    run_case(
        "expense_tracker textinput down stays in edit mode",
        specs,
        "expense_tracker",
        tab * 3 + enter + b"Rent" + down + b"X",
        ["RentX"],
        args.rows,
        args.cols,
    )

    def exercise_formular_combo_mouse(app: PtyApp) -> None:
        row, col = app.screen.position_of("Slovakia")
        app.send(sgr_click(col, row))
        row, col = app.screen.position_of("Czech Republic")
        app.send(sgr_click(col, row))
        row, col = app.screen.position_of("Save")
        app.send(sgr_click(col, row))

    run_dynamic_case(
        "formular combobox mouse selection persists",
        specs,
        "formular",
        exercise_formular_combo_mouse,
        ["country: Czech Republic"],
        args.rows,
        args.cols,
    )

    def exercise_formular_listbox_mouse(app: PtyApp) -> None:
        row, col = app.screen.position_of("Developer")
        app.send(sgr_click(col, row))
        row, col = app.screen.position_of("Designer")
        app.send(sgr_click(col, row))
        row, col = app.screen.position_of("Save")
        app.send(sgr_click(col, row))

    run_dynamic_case(
        "formular listbox mouse selection persists",
        specs,
        "formular",
        exercise_formular_listbox_mouse,
        ["  - Designer"],
        args.rows,
        args.cols,
    )

    def exercise_task_board_dialog_title_mouse_edit(app: PtyApp) -> None:
        row, col = app.screen.position_of("Open")
        app.send(sgr_click(col, row))
        row, col = app.screen.position_of("Review invoice export")
        app.send(sgr_click(col + len("Review invoice export") - 1, row))
        app.send(b"X")

    run_dynamic_case(
        "task_board dialog title mouse edit works",
        specs,
        "task_board",
        exercise_task_board_dialog_title_mouse_edit,
        ["Review invoice exporXt"],
        args.rows,
        args.cols,
    )

    def exercise_task_board_dialog_combobox_arrows(app: PtyApp) -> None:
        row, col = app.screen.position_of("Open")
        app.send(sgr_click(col, row))
        app.send(tab + tab + enter + down + enter)

    run_dynamic_case(
        "task_board dialog combobox arrows select option",
        specs,
        "task_board",
        exercise_task_board_dialog_combobox_arrows,
        ["Status          Blocked"],
        args.rows,
        args.cols,
    )

    run_task_board_repeated_wheel_bottom_case(
        "task_board repeated wheel stays clamped at bottom",
        specs,
    )

    def exercise_widget_listbox_mouse(app: PtyApp) -> None:
        row, col = app.screen.position_of("Review")
        app.send(sgr_click(col, row))
        row, col = app.screen.position_of("Apply")
        app.send(sgr_click(col, row))

    run_dynamic_case(
        "widget_gallery listbox mouse selection persists",
        specs,
        "widget_gallery",
        exercise_widget_listbox_mouse,
        ["mode: Review"],
        args.rows,
        args.cols,
    )

    def exercise_widget_textinput_mouse_selection(app: PtyApp) -> None:
        row, col = app.screen.position_of("Ada Lovelace")
        app.send(sgr_press(0, col, row))
        app.send(sgr_drag(col + len("Ada"), row))
        app.send(sgr_release(0, col + len("Ada"), row))
        app.send(b"Grace")
        row, col = app.screen.position_of("Apply")
        app.send(sgr_click(col, row))

    run_dynamic_case(
        "widget_gallery textinput mouse selection replaces text",
        specs,
        "widget_gallery",
        exercise_widget_textinput_mouse_selection,
        ["name: Grace Lovelace"],
        args.rows,
        args.cols,
    )

    def exercise_widget_numberinput_mouse_blur_commit(app: PtyApp) -> None:
        row, col = app.screen.position_of("Count")
        app.send(sgr_click(col, row + 1))
        app.send(b"8")
        row, col = app.screen.position_of("Apply")
        app.send(sgr_click(col, row))

    run_dynamic_case(
        "widget_gallery numberinput mouse blur commits value",
        specs,
        "widget_gallery",
        exercise_widget_numberinput_mouse_blur_commit,
        ["count: 83"],
        args.rows,
        args.cols,
    )
    print("PASS Swift direct terminal smoke: 31/31 checks passed", flush=True)
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        print(f"ERROR Swift direct terminal smoke failed: {exc}", file=sys.stderr)
        raise
