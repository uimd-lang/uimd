"""Universal visual MCP tester app."""

import argparse
import ast
import datetime
import fcntl
import json
import math
import os
import pty
import re
import shutil
import socket
import struct
import subprocess
import sys
import termios
import threading
import time

from uimd.runtime.application import UIApplication
from uimd.testing.mcp_tester_ui import McpTesterUI


JSONRPC_VERSION = "2.0"
DEFAULT_HOST = "127.0.0.1"
DEFAULT_ACTION_DELAY_MS = 120
DEFAULT_TYPE_DELAY_MS = 100
DEFAULT_STEP_DELAY_SECONDS = 0.45
CONNECT_TIMEOUT_SECONDS = 5.0
DEFAULT_COMMAND_TIMEOUT_SECONDS = 60.0
PROCESS_STOP_TIMEOUT_SECONDS = 2.0
SOCKET_TIMEOUT_MARGIN_SECONDS = 2.0
TEXT_TIMEOUT_MULTIPLIER = 2.0
MILLISECONDS_PER_SECOND = 1000.0
DEFAULT_COMPARE_VIEWPORT_WIDTH = 100  # fallback only; compare mode defaults to terminal size / num apps
DEFAULT_COMPARE_VIEWPORT_HEIGHT = 40  # fallback only; compare mode defaults to terminal height
REQUEST_RETRY_COUNT = 3
REQUEST_RETRY_DELAY_SECONDS = 0.05
OUTPUT_READ_BYTES = 4096
OUTPUT_WRITE_LOCK = threading.Lock()
OUTPUT_IDLE_SECONDS = 0.03
OUTPUT_IDLE_TIMEOUT_SECONDS = 0.5
CONTROLLED_OUTPUT_IDLE_SECONDS = 0.01
CONTROLLED_OUTPUT_IDLE_TIMEOUT_SECONDS = 0.08
CONTROLLED_ANIMATION_REPAINT_SECONDS = 0.1
LIVE_BUFFER_REPAINT_FPS = 30
LIVE_BUFFER_REPAINT_INTERVAL_SECONDS = 1 / LIVE_BUFFER_REPAINT_FPS
HEADLESS_TICK_SECONDS = 0.01
TERMINAL_SYNC_UPDATE_BEGIN = b"\x1b[?2026h"
TERMINAL_SYNC_UPDATE_END = b"\x1b[?2026l"
YAML_LIST_MARKER = "- "
YAML_INDENT_STEP = 2
APP_PATH_KEYS = ("app", "path", "command")
TEST_KEYS = ("tests", "steps")
SETUP_KEYS = ("setup", "before")
CLEANUP_KEYS = ("cleanup", "after")
ENV_KEYS = ("env", "environment")
INCLUDE_KEYS = ("include", "includes")
TESTER_ACTION_KEYS = ("tester", "action")
FILE_CONTENT_KEYS = ("content", "text")
FILE_LINE_KEYS = ("lines",)
RESULT_SEPARATOR_CHAR = "-"
ALL_EXAMPLES_CONFIG = os.path.join("tests", "mcp", "all_examples.yaml")
DEFAULT_SNAPSHOT_DIR = os.path.join("tests", "mcp", "snapshots")
SNAPSHOT_COMPARE_TOOL = "get_render_snapshot"
SNAPSHOT_COMPACT_COMPARE_TOOL = "get_render_snapshot_compact"
TEXT_DELAY_TOOLS = ("set_text", "type_text", "replace_selection")
LIVE_OUTPUT_TOOLS = TEXT_DELAY_TOOLS + ("activate_element", "click_element")
COMPARE_OBSERVATION_TOOLS = (
    "get_element",
    "get_state",
    "get_window",
    "get_render_snapshot",
    "get_render_snapshot_compact",
    "get_render_rect",
)
TARGET_PLACEHOLDERS = ("{platform}", "{target}")
TOOLBAR_BUTTON_NAMES = ("run", "next", "pause", "copy", "quit")
PAUSE_BUTTON_TITLE = "Pause"
PLAY_BUTTON_TITLE = "Play"


class TargetApp:
    def __init__(self, name, path, root, env=None):
        self.name = name
        self.path = path
        self.root = root
        self.env = dict(env or {})
        self.port = None
        self.process = None
        self.viewport = None
        self.output_master_fd = None
        self.output_thread = None
        self.next_request_id = 1
        self.request_id_lock = threading.Lock()
        self.action_delay_ms = DEFAULT_ACTION_DELAY_MS
        self.type_delay_ms = DEFAULT_TYPE_DELAY_MS
        self.forward_output = True
        self.full_repaint_pending = False
        self.capture_output = False
        self.captured_output_chunks = []
        self.capture_output_lock = threading.Lock()
        self.output_sequence = 0
        self.output_sequence_lock = threading.Lock()

    def start(self, viewport, action_delay_ms, type_delay_ms, forward_output=True, controlled_render=False):
        self.viewport = dict(viewport)
        self.action_delay_ms = int(action_delay_ms)
        self.type_delay_ms = int(type_delay_ms)
        self.forward_output = bool(forward_output)
        self.port = _find_free_port()
        command = self._command(action_delay_ms, type_delay_ms, controlled_render)
        process_env = os.environ.copy()
        process_env.update({str(key): str(value) for key, value in self.env.items()})
        master_fd, slave_fd = pty.openpty()
        _set_pty_size(master_fd, self.viewport)
        _set_pty_size(slave_fd, self.viewport)
        self.output_master_fd = master_fd
        self.process = subprocess.Popen(
            command,
            cwd=self.root,
            stdin=subprocess.DEVNULL,
            stdout=slave_fd,
            stderr=subprocess.PIPE,
            text=True,
            env=process_env,
        )
        os.close(slave_fd)
        self.output_thread = threading.Thread(target=self._drain_output, daemon=True)
        self.output_thread.start()

    def stop(self):
        if self.process is None:
            return
        if self.process.poll() is None:
            self.process.terminate()
            try:
                self.process.wait(timeout=PROCESS_STOP_TIMEOUT_SECONDS)
            except subprocess.TimeoutExpired:
                self.process.kill()
                self.process.wait(timeout=PROCESS_STOP_TIMEOUT_SECONDS)
        self._close_output()
        self.process = None

    def _drain_output(self):
        output_fd = self.output_master_fd
        should_forward = bool(getattr(sys.stdout, "isatty", lambda: False)())
        stdout_buffer = getattr(sys.stdout, "buffer", None)
        while True:
            try:
                data = os.read(output_fd, OUTPUT_READ_BYTES)
            except OSError:
                return
            if not data:
                return
            with self.output_sequence_lock:
                self.output_sequence += 1
            with self.capture_output_lock:
                if self.capture_output:
                    self.captured_output_chunks.append(data)
                    continue
            if not should_forward or not self.forward_output:
                continue
            with OUTPUT_WRITE_LOCK:
                if stdout_buffer is not None:
                    stdout_buffer.write(data)
                    stdout_buffer.flush()
                else:
                    sys.stdout.write(data.decode("utf-8", errors="replace"))
                    sys.stdout.flush()

    def _close_output(self):
        if self.output_master_fd is None:
            return
        try:
            os.close(self.output_master_fd)
        except OSError:
            pass
        self.output_master_fd = None

    def call_tool(self, name, arguments=None, timeout_ms=None):
        return self.request("tools/call", {
            "name": name,
            "arguments": arguments or {},
        }, timeout_ms=timeout_ms)

    def set_output_forwarding(self, enabled):
        self.forward_output = bool(enabled)

    def begin_output_capture(self):
        with self.capture_output_lock:
            self.captured_output_chunks = []
            self.capture_output = True

    def end_output_capture(self):
        with self.capture_output_lock:
            self.capture_output = False
            data = b"".join(self.captured_output_chunks)
            self.captured_output_chunks = []
            return data

    def wait_output_idle(self, stable_seconds=OUTPUT_IDLE_SECONDS, timeout_seconds=OUTPUT_IDLE_TIMEOUT_SECONDS):
        deadline = time.monotonic() + timeout_seconds
        with self.output_sequence_lock:
            last_sequence = self.output_sequence
        stable_since = time.monotonic()
        while time.monotonic() < deadline:
            time.sleep(min(stable_seconds, 0.01))
            with self.output_sequence_lock:
                current_sequence = self.output_sequence
            if current_sequence != last_sequence:
                last_sequence = current_sequence
                stable_since = time.monotonic()
                continue
            if time.monotonic() - stable_since >= stable_seconds:
                return

    def request(self, method, params=None, timeout_ms=None):
        with self.request_id_lock:
            request_id = self.next_request_id
            self.next_request_id += 1
        payload = {
            "jsonrpc": JSONRPC_VERSION,
            "id": request_id,
            "method": method,
            "params": params or {},
        }
        timeout = self._request_timeout(method, params or {}, timeout_ms=timeout_ms)
        response = self._send_request(payload, timeout)
        if "error" in response:
            raise RuntimeError(response["error"]["message"])
        result = response.get("result", {})
        if method == "tools/call":
            return _decode_tool_result(result)
        return result

    def _send_request(self, payload, timeout):
        last_error = None
        for attempt in range(REQUEST_RETRY_COUNT + 1):
            try:
                with socket.create_connection((DEFAULT_HOST, self.port), timeout=timeout) as sock:
                    sock.settimeout(timeout)
                    sock.sendall((json.dumps(payload) + "\n").encode("utf-8"))
                    return _read_json_line(sock)
            except OSError as exc:
                last_error = exc
                if attempt >= REQUEST_RETRY_COUNT or self._process_exited():
                    raise
                time.sleep(REQUEST_RETRY_DELAY_SECONDS)
        raise last_error

    def _process_exited(self):
        return self.process is not None and self.process.poll() is not None

    def _request_timeout(self, method, params, timeout_ms=None):
        if timeout_ms is not None:
            timeout_ms = int(timeout_ms)
            if timeout_ms <= 0:
                return None
            return timeout_ms / MILLISECONDS_PER_SECOND

        timeout = DEFAULT_COMMAND_TIMEOUT_SECONDS
        if method != "tools/call":
            return timeout
        tool = params.get("name")
        arguments = params.get("arguments", {})
        if tool in TEXT_DELAY_TOOLS:
            action_seconds = self.action_delay_ms / MILLISECONDS_PER_SECOND
            text_seconds = self._text_argument_length(arguments) * self.type_delay_ms / MILLISECONDS_PER_SECOND
            timeout = max(
                timeout,
                action_seconds + text_seconds * TEXT_TIMEOUT_MULTIPLIER + SOCKET_TIMEOUT_MARGIN_SECONDS,
            )
        return timeout

    @staticmethod
    def _text_argument_length(arguments):
        return len(str(arguments.get("text", "")))

    def wait_until_ready(self):
        deadline = time.monotonic() + CONNECT_TIMEOUT_SECONDS
        last_error = None
        while time.monotonic() < deadline:
            if self.process is not None and self.process.poll() is not None:
                stderr = self.process.stderr.read() if self.process.stderr else ""
                raise RuntimeError(f"{self.name} exited early: {stderr.strip()}")
            try:
                self.call_tool("get_window")
                return
            except OSError as exc:
                last_error = exc
                time.sleep(0.05)
        raise TimeoutError(f"{self.name} MCP server did not start: {last_error}")

    def set_viewport(self, viewport):
        viewport = dict(viewport)
        if viewport == self.viewport:
            return
        self.viewport = viewport
        _set_pty_size(self.output_master_fd, viewport)
        self.call_tool("set_viewport", viewport)

    def repaint(self):
        arguments = {"full": True} if self.full_repaint_pending else {}
        self.full_repaint_pending = False
        self.call_tool("repaint", arguments)

    def mark_full_repaint_pending(self):
        self.full_repaint_pending = True

    def render_rect(self):
        return self.call_tool("get_render_rect")

    def _command(self, action_delay_ms, type_delay_ms, controlled_render=False):
        path = _abs_path(self.root, self.path)
        if path.endswith(".py"):
            command = [sys.executable, path]
        else:
            command = [path]
        viewport = self.viewport
        command += [
            "--mcp-server",
            "--gui",
            "--mcp-transport",
            "tcp",
            "--mcp-host",
            DEFAULT_HOST,
            "--mcp-port",
            str(self.port),
            "--mcp-action-delay-ms",
            str(action_delay_ms),
            "--mcp-type-delay-ms",
            str(type_delay_ms),
            "--viewport",
            f"{viewport['row']},{viewport['col']},{viewport['width']},{viewport['height']}",
        ]
        command.append("--mcp-wait-render")
        if controlled_render:
            command.append("--mcp-controlled-render")
        return command


class McpTester(McpTesterUI):
    def __init__(self, config):
        super().__init__()
        self.run_btn = self.run
        self.next_btn = self.next
        self.stop_btn = self.pause
        self.copy_btn = self.copy
        self.quit_btn = self.quit
        self.config = config
        self.targets = {}
        self.steps = []
        self.step_index = 0
        self.running_script = False
        self.paused = False
        self.finished = False
        self.last_step_at = 0.0
        self.last_animation_repaint_at = 0.0
        self.pending_repaint = False
        self.last_viewport = None
        self.start_targets_pending = False
        self.exit_code = 0
        self.cleanup_done = False
        self.assertions_passed = 0
        self.assertions_failed = 0
        self.step_failures = 0
        self.script_assertions_passed = 0
        self.script_assertions_failed = 0
        self.script_step_failures = 0
        self.summary_done = False
        self.script_index = -1
        self.current_script = None
        self.script_summaries = []
        self.compare_pan = {"v": 0, "h": 0}
        self._buffer_panel_focused = False
        self._buffer_panel_edit = False
        self._buffer_pre_focus = None   # element focused before entering buffer panel
        self._buffer_repaint_event = threading.Event()
        self._buffer_repaint_stop = threading.Event()
        self._buffer_repaint_thread = None
        self._cached_frames = {}
        self._cached_frames_lock = threading.Lock()

    def _restore_pre_focus(self):
        """Restore the element that was focused before entering buffer panel."""
        prev = self._buffer_pre_focus
        self._buffer_pre_focus = None
        if prev is not None:
            self.set_focus(prev)

    def _set_buffer_panel_state(self, focused, edit):
        """Update focus/edit state on the framebuffer element."""
        changed = (self._buffer_panel_focused != focused or self._buffer_panel_edit != edit)
        # On first entry: save current element and clear its visual focus.
        if focused and not self._buffer_panel_focused and not self._buffer_panel_edit:
            self._buffer_pre_focus = self._focused_element
            if self._focused_element is not None:
                self._set_element_focus_state(self._focused_element, False)
                self.invalidate()
        self._buffer_panel_focused = focused
        self._buffer_panel_edit = edit
        if changed:
            self._sync_buffer_panel_interaction_state()

    def _sync_buffer_panel_interaction_state(self):
        viewport = getattr(self, "app_viewport", None)
        if viewport is None or not hasattr(viewport, "set_interaction_state"):
            return
        changed = (
            getattr(viewport, "_panel_focused", None) != self._buffer_panel_focused or
            getattr(viewport, "_panel_edit", None) != self._buffer_panel_edit
        )
        viewport.set_interaction_state(self._buffer_panel_focused, self._buffer_panel_edit)
        if changed and getattr(self, "_app", None) is not None:
            self._app.mark_dirty()

    def post_render(self):
        """Called by UIApplication after each terminal render."""
        if not self._is_buffer_mode() or not self.targets:
            return
        # Repaint buffer when log panel enters/exits scope (dim state changes).
        scope = getattr(self, "_active_scrollview_scope", None)
        log = getattr(self, "log_output", None)
        log_in_scope = scope is not None and log is not None and scope.get("proxy") is log
        if log_in_scope != getattr(self, "_last_log_in_scope", False):
            self._last_log_in_scope = log_in_scope
            self._request_buffer_repaint()
        if self._buffer_panel_focused or self._buffer_panel_edit:
            self._sync_buffer_panel_interaction_state()

    def _request_buffer_repaint(self):
        """Signal the background repaint thread to fetch frames and redraw."""
        self._buffer_repaint_event.set()

    def _start_buffer_repaint_thread(self):
        """Start the background thread that handles buffer repaints."""
        self._buffer_repaint_stop.clear()
        self._buffer_repaint_event.clear()
        self._buffer_repaint_thread = threading.Thread(
            target=self._buffer_repaint_worker, daemon=True, name="buffer-repaint"
        )
        self._buffer_repaint_thread.start()

    def _stop_buffer_repaint_thread(self):
        """Stop the background repaint thread."""
        if self._buffer_repaint_thread is not None:
            self._buffer_repaint_stop.set()
            self._buffer_repaint_event.set()
            self._buffer_repaint_thread.join(timeout=2.0)
            self._buffer_repaint_thread = None
        self._buffer_repaint_stop.clear()
        self._buffer_repaint_event.clear()

    def _buffer_repaint_worker(self):
        """Background thread: fetch frames via MCP then draw from cache."""
        while not self._buffer_repaint_stop.is_set():
            self._buffer_repaint_event.wait()
            self._buffer_repaint_event.clear()
            if self._buffer_repaint_stop.is_set():
                break
            if not self._is_buffer_mode() or not self.targets:
                continue
            try:
                self._fetch_frames_from_mcp()
                self._repaint_buffer_targets()
            except Exception:
                pass

    def resize(self, width, height):
        super().resize(width, height)
        self._align_toolbar()

    def _align_toolbar(self):
        entries = {
            entry.get("name"): entry
            for entry in getattr(self, "_layout", [])
            if entry.get("name")
        }
        header_entry = entries.get("header")
        if not header_entry:
            return

        buttons = [
            getattr(self, name)
            for name in TOOLBAR_BUTTON_NAMES
            if hasattr(self, name) and entries.get(name)
        ]
        if not buttons:
            return

        last_entry = entries[buttons[-1].name]
        right_margin = int(last_entry.get("margin_right", 0) or 0)
        cursor = max(0, int(getattr(self, "_window_width", 0) or 0) - right_margin)

        for index in range(len(buttons) - 1, -1, -1):
            button = buttons[index]
            entry = entries[button.name]
            button_width = max(
                1,
                int(getattr(button, "width", 0) or entry.get("chars_width", 1) or 1),
            )
            cursor -= button_width
            button.col = max(0, cursor)
            button.row = int(entry.get("row", 0) or 0)
            if index > 0:
                previous_entry = entries[buttons[index - 1].name]
                previous_right = int(previous_entry.get("col", 0) or 0) + int(
                    previous_entry.get("chars_width", button_width) or button_width
                )
                gap = max(0, int(entry.get("col", 0) or 0) - previous_right)
                cursor -= gap

        run_entry = entries[buttons[0].name]
        header_col = int(header_entry.get("col", 0) or 0)
        header_right = header_col + int(header_entry.get("chars_width", 1) or 1)
        header_gap = max(0, int(run_entry.get("col", 0) or 0) - header_right)
        self.header.col = header_col
        self.header.row = int(header_entry.get("row", 0) or 0)
        self.header.width = max(1, cursor - header_col - header_gap)

    def open(self):
        super().open()
        self._append_log("tester ready")
        self._append_log(
            f"loaded {self.config.source_path}: "
            f"{len(self.config.scripts)} script(s)"
        )
        self._start_next_script()

    def close(self):
        self._stop_targets()
        self._run_cleanup()
        super().close()

    def elementchanged(self, element, value):
        name = getattr(element, "name", "")
        if name in ("run", "run_btn"):
            self._restart()
        elif name in ("next", "next_btn"):
            if not self.finished:
                self._run_next_step()
        elif name in ("pause", "stop_btn"):
            self._toggle_pause()
        elif name in ("copy", "copy_btn"):
            count = self.log_output.copy_all()
            self._append_log(f"copied {count} log lines")
        elif name in ("quit", "quit_btn"):
            self._stop_targets()
            if getattr(self, "_app", None) is not None:
                self._app._running = False

    def _toggle_pause(self):
        if self.finished:
            return
        if self.running_script:
            self.running_script = False
            self.paused = True
            self._update_pause_button()
            self._append_log("paused")
            return
        if self.paused:
            self.running_script = True
            self.paused = False
            self.last_step_at = time.monotonic()
            self._update_pause_button()
            self._append_log("resumed")

    def _update_pause_button(self):
        self.stop_btn.title = PLAY_BUTTON_TITLE if self.paused else PAUSE_BUTTON_TITLE
        if getattr(self, "_app", None) is not None:
            self._app.mark_dirty()

    def tick(self):
        self._update_viewports()
        if self.start_targets_pending:
            self.start_targets_pending = False
            self._start_targets()
            self.last_step_at = time.monotonic()
            return
        if self.pending_repaint:
            self._repaint_targets()
            self.pending_repaint = False
        self._repaint_controlled_animation_if_due()
        if not self.running_script or self.finished:
            return
        now = time.monotonic()
        if now - self.last_step_at < self.config.step_delay_seconds:
            return
        self.last_step_at = now
        self._run_next_step()

    def _restart(self):
        self._stop_targets()
        self._run_cleanup()
        self.step_index = 0
        self.finished = False
        self.paused = False
        self.exit_code = 0
        self.cleanup_done = False
        self.assertions_passed = 0
        self.assertions_failed = 0
        self.step_failures = 0
        self.script_assertions_passed = 0
        self.script_assertions_failed = 0
        self.script_step_failures = 0
        self.summary_done = False
        self.script_index = -1
        self.current_script = None
        self.script_summaries = []
        self.running_script = False
        self.start_targets_pending = False
        self.log_output.clear()
        self._append_log("restart")
        self._update_pause_button()
        self._start_next_script()

    def _start_next_script(self):
        self.script_index += 1
        if self.script_index >= len(self.config.scripts):
            self.finished = True
            self.running_script = False
            self.paused = False
            self._update_pause_button()
            self._append_summary()
            self._stop_if_exit_on_finish()
            return

        self.current_script = self.config.scripts[self.script_index]
        self.steps = list(self.current_script.steps)
        self.step_index = 0
        self.cleanup_done = False
        self.script_assertions_passed = 0
        self.script_assertions_failed = 0
        self.script_step_failures = 0
        self.running_script = False
        self.paused = False
        self.start_targets_pending = False
        self._update_pause_button()
        self._append_log(
            f"SCRIPT {self.script_index + 1}/{len(self.config.scripts)} "
            f"{self.current_script.name}: {self.current_script.source_path}"
        )
        self._append_log(
            f"script apps={len(self.current_script.apps)}, steps={len(self.current_script.steps)}"
        )
        self._run_setup()
        self.running_script = True
        self.start_targets_pending = True

    def _start_targets(self):
        self.compare_pan = {"v": 0, "h": 0}
        self._buffer_panel_focused = False
        self._buffer_panel_edit = False
        self._buffer_pre_focus = None
        with self._cached_frames_lock:
            self._cached_frames.clear()
        self._stop_buffer_repaint_thread()
        viewports = self._app_viewport_rects(self.current_script.apps.keys())
        controlled_output = self._requires_controlled_target_output(
            len(self.current_script.apps)
        )
        started = []
        for name, path in self.current_script.apps.items():
            target_env = _format_target_value(self.current_script.env.get(name, {}), name)
            target = TargetApp(name, path, self.config.root, target_env)
            self.targets[name] = target
            self._append_log(f"spawn {name}: {path}")
            target.start(
                viewports[name],
                self.config.action_delay_ms,
                self.config.type_delay_ms,
                forward_output=not controlled_output,
                controlled_render=controlled_output,
            )
            started.append(target)

        for target in started:
            target.wait_until_ready()
            self._append_log(f"connected {target.name} port={target.port}")

        self._sync_protected_rects()
        self._update_app_header(viewports)
        self.pending_repaint = True
        if self._is_buffer_mode():
            self._start_buffer_repaint_thread()
        if hasattr(self, "pause") and self.pause is not None:
            self.set_focus(self.pause)

    def _stop_targets(self):
        self._stop_buffer_repaint_thread()
        for target in self.targets.values():
            target.stop()
        self.targets = {}
        if getattr(self, "_app", None) is not None:
            self._app.set_protected_rects([])

    def _run_next_step(self):
        if self.step_index >= len(self.steps):
            self.finished = True
            self.running_script = False
            self.paused = False
            self._update_pause_button()
            self._finish_script(success=True)
            return

        step = self.steps[self.step_index]
        self.step_index += 1
        try:
            if "compare" in step:
                self._run_compare_step(step)
            else:
                self._run_tool_step(step)
        except Exception as exc:
            self._append_log(f"FAIL step {self.step_index}: {exc}")
            self.step_failures += 1
            self.script_step_failures += 1
            self.exit_code = 1
            self.finished = True
            self.running_script = False
            self.paused = False
            self._update_pause_button()
            self._finish_script(success=False)

    def _run_tool_step(self, step):
        action = _first_present(step, TESTER_ACTION_KEYS)
        if action is not None:
            self._run_tester_step(step)
            return

        target_names = self._target_names(step.get("target"))
        tool = step["tool"]
        arguments = step.get("arguments", {})
        timeout_ms = step.get("timeout_ms")
        arguments_by_target = {
            target_name: self._target_tool_arguments(tool, arguments, target_name)
            for target_name in target_names
        }
        assertions_by_target = {
            target_name: _format_target_value(step.get("assert"), target_name)
            for target_name in target_names
        }
        for target_name in target_names:
            self._append_log(
                f"{target_name}: {tool} {json.dumps(arguments_by_target[target_name], sort_keys=True)}"
            )
        results = self._call_targets(target_names, tool, arguments_by_target, timeout_ms)
        for target_name in target_names:
            result = results[target_name]
            self._append_log(f"{target_name}: -> {_compact_result(result)}")
            assertion = assertions_by_target[target_name]
            if assertion is not None:
                self._assert_result(target_name, result, assertion)
        if self.config.compare_mode:
            self._compare_tool_results(tool, results, step.get("assert"))

        exited_targets = self._exited_target_names(target_names)
        if exited_targets:
            if self.step_index >= len(self.steps):
                self._append_log(
                    f"skip post-step snapshot after target exit: {', '.join(exited_targets)}"
                )
                return
            raise RuntimeError(
                f"target exited before script completed: {', '.join(exited_targets)}"
            )

        self._sync_protected_rects()
        if self._tool_needs_repaint(tool):
            self._repaint_targets()
        if self.config.compare_mode:
            self._compare_render_snapshots(list(results))

    def _exited_target_names(self, target_names):
        exited = []
        for target_name in target_names:
            target = self.targets.get(target_name)
            process_exited = getattr(target, "_process_exited", None)
            if callable(process_exited) and process_exited():
                exited.append(target_name)
        return exited

    def _target_tool_arguments(self, tool, arguments, target_name):
        arguments = _format_target_value(arguments, target_name)
        if not self.config.compare_mode or tool not in {"mouse_click", "mouse_drag"}:
            return arguments
        viewport = getattr(self.targets.get(target_name), "viewport", None) or {}
        row_offset = int(viewport.get("row", 0) or 0)
        col_offset = int(viewport.get("col", 0) or 0)
        adjusted = dict(arguments or {})
        if tool == "mouse_click":
            if "x" in adjusted:
                adjusted["x"] = int(adjusted["x"]) + col_offset
            if "y" in adjusted:
                adjusted["y"] = int(adjusted["y"]) + row_offset
            return adjusted
        for key in ("from_x", "to_x"):
            if key in adjusted:
                adjusted[key] = int(adjusted[key]) + col_offset
        for key in ("from_y", "to_y"):
            if key in adjusted:
                adjusted[key] = int(adjusted[key]) + row_offset
        return adjusted

    def _call_targets(self, target_names, tool, arguments_by_target, timeout_ms):
        results = {}
        errors = {}
        threads = []

        def call_target(target_name):
            try:
                results[target_name] = self.targets[target_name].call_tool(
                    tool,
                    arguments_by_target[target_name],
                    timeout_ms=timeout_ms,
                )
            except Exception as exc:
                errors[target_name] = exc

        target_apps = [
            (self.targets[target_name], getattr(self.targets[target_name], "forward_output", True))
            for target_name in target_names
        ]
        controlled_output = self._uses_controlled_target_output()
        stream_live_output = controlled_output and tool in LIVE_OUTPUT_TOOLS and not self._is_buffer_mode()
        if controlled_output:
            for target, _was_enabled in target_apps:
                target.set_output_forwarding(stream_live_output)
        try:
            for target_name in target_names:
                thread = threading.Thread(target=call_target, args=(target_name,))
                thread.start()
                threads.append(thread)

            self._join_tool_threads_with_live_repaint(threads, tool)
            if self._tool_needs_repaint(tool):
                for target, _was_enabled in target_apps:
                    if hasattr(target, "mark_full_repaint_pending"):
                        target.mark_full_repaint_pending()
        finally:
            for target, _was_enabled in target_apps:
                if _was_enabled:
                    target.wait_output_idle()
                else:
                    target.wait_output_idle(
                        stable_seconds=CONTROLLED_OUTPUT_IDLE_SECONDS,
                        timeout_seconds=CONTROLLED_OUTPUT_IDLE_TIMEOUT_SECONDS,
                    )
            for target, was_enabled in target_apps:
                target.set_output_forwarding(was_enabled)

        if errors:
            target_name = next(iter(errors))
            raise RuntimeError(f"{target_name}: {errors[target_name]}") from errors[target_name]
        return {target_name: results[target_name] for target_name in target_names}

    def _join_tool_threads_with_live_repaint(self, threads, tool):
        if not self._should_live_repaint_tool(tool):
            for thread in threads:
                thread.join()
            return

        while True:
            alive = False
            for thread in threads:
                thread.join(timeout=LIVE_BUFFER_REPAINT_INTERVAL_SECONDS)
                alive = alive or thread.is_alive()
            self._fetch_frames_from_mcp()
            self._repaint_buffer_targets()
            if not alive:
                return

    def _should_live_repaint_tool(self, tool):
        return self._is_buffer_mode() and tool in LIVE_OUTPUT_TOOLS

    def _run_setup(self):
        self._run_tester_steps(self.current_script.setup_steps, "setup")

    def _run_cleanup(self):
        if self.cleanup_done:
            return
        self.cleanup_done = True
        if self.current_script is None:
            return
        self._run_tester_steps(self.current_script.cleanup_steps, "cleanup")

    def _run_tester_steps(self, steps, label):
        for step in self._expand_tester_steps(steps):
            self._append_log(f"{label}: {_tester_step_label(step)}")
            try:
                result = _run_tester_file_step(self.config.root, step)
            except AssertionError:
                if _is_tester_assert_step(step):
                    self.assertions_failed += 1
                    self.script_assertions_failed += 1
                raise
            self._append_log(f"{label}: -> {_compact_result(result)}")
            if _is_tester_assert_step(step):
                self.assertions_passed += 1
                self.script_assertions_passed += 1

    def _run_tester_step(self, step):
        for expanded in self._expand_tester_steps([step]):
            self._append_log(f"tester: {_tester_step_label(expanded)}")
            try:
                result = _run_tester_file_step(self.config.root, expanded)
            except AssertionError:
                if _is_tester_assert_step(expanded):
                    self.assertions_failed += 1
                    self.script_assertions_failed += 1
                raise
            self._append_log(f"tester: -> {_compact_result(result)}")
            if _is_tester_assert_step(expanded):
                self.assertions_passed += 1
                self.script_assertions_passed += 1

    def _expand_tester_steps(self, steps):
        names = list(self.targets) or list(self.current_script.apps)
        expanded = []
        for step in steps:
            if not _has_target_placeholder(step):
                expanded.append(step)
                continue
            for target_name in names:
                expanded.append(_format_target_value(step, target_name))
        return expanded

    def _run_compare_step(self, step):
        target_names = step["compare"]
        if not isinstance(target_names, list) or len(target_names) < 2:
            raise ValueError("compare step requires at least two target names")
        snapshot_tool = step.get("snapshot", SNAPSHOT_COMPARE_TOOL)
        if snapshot_tool != SNAPSHOT_COMPARE_TOOL:
            raise ValueError(f"snapshot compare only supports {SNAPSHOT_COMPARE_TOOL}")
        arguments = dict(step.get("arguments", {}))
        arguments.setdefault("snapshot_time_ms", int(time.time() * MILLISECONDS_PER_SECOND))
        snapshots = []
        for target_name in target_names:
            self._append_log(f"{target_name}: {snapshot_tool} {json.dumps(arguments, sort_keys=True)}")
            snapshots.append(self.targets[target_name].call_tool(snapshot_tool, arguments))
        first = snapshots[0]
        for target_name, snapshot in zip(target_names[1:], snapshots[1:]):
            mismatch = _render_snapshot_mismatch(first, snapshot)
            if mismatch is not None:
                bundle_path = self._write_snapshot_bundle(
                    snapshot_tool,
                    target_names,
                    snapshots,
                    mismatch,
                )
                if bundle_path:
                    self._append_snapshot_viewer_hint(bundle_path)
                self.assertions_failed += 1
                self.script_assertions_failed += 1
                raise AssertionError(f"snapshot mismatch for {target_name}: {mismatch}")
        self.assertions_passed += 1
        self.script_assertions_passed += 1
        self._append_log(f"COMPARE {snapshot_tool}: {', '.join(target_names)}")

    def _compare_render_snapshots(self, target_names):
        if len(target_names) < 2:
            return
        snapshot_time_ms = int(time.time() * MILLISECONDS_PER_SECOND)
        arguments_by_target = {
            target_name: {"snapshot_time_ms": snapshot_time_ms}
            for target_name in target_names
        }
        snapshots = self._call_targets(target_names, SNAPSHOT_COMPACT_COMPARE_TOOL, arguments_by_target, None)
        names = list(snapshots)
        first = snapshots[names[0]]
        for target_name in names[1:]:
            mismatch = _render_snapshot_mismatch(first, snapshots[target_name])
            if mismatch is not None:
                bundle_path = self._write_snapshot_bundle(
                    SNAPSHOT_COMPACT_COMPARE_TOOL,
                    names,
                    [snapshots[name] for name in names],
                    mismatch,
                )
                if bundle_path:
                    self._append_snapshot_viewer_hint(bundle_path)
                raise AssertionError(f"snapshot mismatch for {target_name}: {mismatch}")
        self.assertions_passed += 1
        self.script_assertions_passed += 1
        self._append_log(f"COMPARE {SNAPSHOT_COMPACT_COMPARE_TOOL}: {', '.join(names)}")

    def _write_snapshot_bundle(self, tool, target_names, snapshots, mismatch):
        if not self.config.snapshot_dir:
            return None
        timestamp = datetime.datetime.now().strftime("%Y%m%d-%H%M%S-%f")
        script_name = _safe_filename(os.path.splitext(os.path.basename(self.current_script.source_path))[0])
        file_name = f"{timestamp}-step-{self.step_index:03d}-{script_name}.json"
        path = os.path.join(self.config.snapshot_dir, file_name)
        payload = {
            "created_at": datetime.datetime.now().isoformat(timespec="milliseconds"),
            "source_path": self.current_script.source_path,
            "script_index": self.script_index + 1,
            "step_index": self.step_index,
            "tool": tool,
            "targets": [
                {
                    "name": target_name,
                    "snapshot": snapshot,
                }
                for target_name, snapshot in zip(target_names, snapshots)
            ],
            "mismatch": mismatch,
            "viewer": f"python3 tools/mcp_snapshot_viewer.py {os.path.relpath(path, self.config.root)}",
        }
        with open(path, "w", encoding="utf-8") as handle:
            json.dump(payload, handle, ensure_ascii=False, indent=2)
            handle.write("\n")
        return path

    def _append_snapshot_viewer_hint(self, path):
        rel_path = os.path.relpath(path, self.config.root)
        self._append_log(f"snapshot dump: {rel_path}")
        self._append_log(f"viewer: python3 tools/mcp_snapshot_viewer.py {rel_path}")

    def _assert_result(self, target_name, result, assertion):
        for key, expected in assertion.items():
            actual = result.get(key)
            passed, label = _assertion_matches(actual, expected)
            if not passed:
                self.assertions_failed += 1
                self.script_assertions_failed += 1
                raise AssertionError(f"{target_name}.{key} expected {label}, got {actual!r}")
            self.assertions_passed += 1
            self.script_assertions_passed += 1
            self._append_log(f"ASSERT {target_name}.{key} {label}")

    def _compare_tool_results(self, tool, results, assertion):
        if len(results) < 2:
            return
        fields = _comparison_assertion_fields(assertion) if assertion is not None else None
        if assertion is not None and not fields:
            return
        if fields is None and tool not in COMPARE_OBSERVATION_TOOLS:
            return

        names = list(results)
        first_name = names[0]
        if tool in (SNAPSHOT_COMPARE_TOOL, SNAPSHOT_COMPACT_COMPARE_TOOL) and fields is None:
            first = results[first_name]
            for target_name in names[1:]:
                mismatch = _render_snapshot_mismatch(first, results[target_name])
                if mismatch is not None:
                    raise AssertionError(f"compare mismatch for {target_name} on {tool}: {mismatch}")
            self._append_log(f"COMPARE {tool}: {', '.join(names)} cells")
            return

        first = _select_snapshot_fields(results[first_name], fields)
        for target_name in names[1:]:
            current = _select_snapshot_fields(results[target_name], fields)
            if current != first:
                raise AssertionError(f"compare mismatch for {target_name} on {tool}")
        compared = ", ".join(names)
        suffix = f" fields={fields}" if fields else ""
        self._append_log(f"COMPARE {tool}: {compared}{suffix}")

    def _target_names(self, target_name):
        if target_name == "*":
            return list(self.targets)
        if target_name not in self.targets:
            raise ValueError(f"unknown target: {target_name}")
        return [target_name]

    def _update_viewports(self):
        if not self.targets:
            return
        viewports = self._app_viewport_rects(self.targets.keys())
        if viewports == self.last_viewport:
            return
        self.last_viewport = dict(viewports)
        for name, target in self.targets.items():
            target.set_viewport(viewports[name])
        self._sync_protected_rects()
        self._update_app_header(viewports)
        self._repaint_targets()
        summary = ", ".join(
            f"{name}={viewport['row']},{viewport['col']} {viewport['width']}x{viewport['height']}"
            for name, viewport in viewports.items()
        )
        self._append_log(f"viewports {summary}")
        self.pending_repaint = True

    def _app_viewport_rect(self):
        if self.config.plain:
            explicit = self.config.compare_viewport
            return {
                "row": 0,
                "col": 0,
                "width": max(1, int(explicit.get("width") or DEFAULT_COMPARE_VIEWPORT_WIDTH)),
                "height": max(1, int(explicit.get("height") or DEFAULT_COMPARE_VIEWPORT_HEIGHT)),
            }
        elem = self.app_viewport
        offset_col = 0
        offset_row = 0
        if getattr(self, "_app", None) is not None:
            offset_col, offset_row = self._app._active_window_offsets()
        return {
            "row": int(offset_row + getattr(elem, "_cell_row", 0) + getattr(elem, "row", 0)),
            "col": int(offset_col + getattr(elem, "_cell_col", 0) + getattr(elem, "col", 0)),
            "width": max(1, int(getattr(elem, "width", None) or 1)),
            "height": max(1, int(getattr(elem, "height", None) or 1)),
        }

    def _app_viewport_rects(self, target_names):
        names = list(target_names)
        if not names:
            return {}

        bounds = self._app_viewport_rect()
        explicit = self.config.compare_viewport
        if explicit:
            count = len(names)
            target_width = int(explicit.get("width") or DEFAULT_COMPARE_VIEWPORT_WIDTH)
            target_height = int(explicit.get("height") or DEFAULT_COMPARE_VIEWPORT_HEIGHT)
            return {
                name: {
                    "row": bounds["row"],
                    "col": bounds["col"] + index * target_width,
                    "width": target_width,
                    "height": target_height,
                }
                for index, name in enumerate(names)
            }
        if len(names) == 1:
            return {names[0]: bounds}
        if self.config.compare_mode:
            count = len(names)
            target_width = max(1, bounds["width"] // count)
            target_height = max(1, bounds["height"])
            return {
                name: {
                    "row": bounds["row"],
                    "col": bounds["col"] + index * target_width,
                    "width": target_width,
                    "height": target_height,
                }
                for index, name in enumerate(names)
            }
        if len(names) > bounds["width"] * bounds["height"]:
            raise ValueError("app viewport is too small for all target panels")

        cols, rows = self._app_viewport_grid(len(names), bounds["width"], bounds["height"])
        widths = self._split_dimension(bounds["width"], cols)
        heights = self._split_dimension(bounds["height"], rows)
        rects = {}
        index = 0
        row_offset = 0
        for row_height in heights:
            col_offset = 0
            for col_width in widths:
                if index >= len(names):
                    return rects
                rects[names[index]] = {
                    "row": bounds["row"] + row_offset,
                    "col": bounds["col"] + col_offset,
                    "width": col_width,
                    "height": row_height,
                }
                index += 1
                col_offset += col_width
            row_offset += row_height
        return rects

    @staticmethod
    def _app_viewport_grid(count, width, height):
        count = max(1, int(count))
        width = max(1, int(width))
        height = max(1, int(height))
        cols = min(count, width, max(1, int(math.ceil(math.sqrt(count)))))
        rows = max(1, int(math.ceil(count / cols)))

        while rows > height and cols < min(count, width):
            cols += 1
            rows = max(1, int(math.ceil(count / cols)))

        while cols > width and rows < min(count, height):
            rows += 1
            cols = max(1, int(math.ceil(count / rows)))

        return cols, rows

    @staticmethod
    def _split_dimension(size, parts):
        size = max(1, int(size))
        parts = max(1, min(int(parts), size))
        base = size // parts
        extra = size % parts
        return [
            base + (1 if index < extra else 0)
            for index in range(parts)
        ]

    def _sync_protected_rects(self):
        if getattr(self, "_app", None) is None:
            return
        if self._is_buffer_mode() and self.targets:
            rects = []
        else:
            rects = [
                dict(target.viewport)
                for target in self.targets.values()
                if target.viewport is not None
            ]
        self._app.set_protected_rects(rects)

    def _update_app_header(self, viewports=None):
        if not hasattr(self, "app_header"):
            return
        viewports = viewports or self.last_viewport or {}
        width = max(1, int(getattr(self.app_header, "width", 0) or 1))
        header_col = self._absolute_element_col(self.app_header)
        cells = [" "] * width
        for name, viewport in viewports.items():
            title = self._panel_title(name)
            start = int(viewport["col"]) - header_col
            panel_width = int(viewport["width"])
            if panel_width <= 0:
                continue
            title = title[:panel_width]
            offset = start + max(0, (panel_width - len(title)) // 2)
            for index, char in enumerate(title):
                cell_index = offset + index
                if 0 <= cell_index < width:
                    cells[cell_index] = char
        self.app_header.text = "".join(cells)

    def _absolute_element_col(self, elem):
        offset_col = 0
        if getattr(self, "_app", None) is not None:
            offset_col, _offset_row = self._app._active_window_offsets()
        return int(offset_col + getattr(elem, "_cell_col", 0) + getattr(elem, "col", 0))

    def _absolute_element_row(self, elem):
        offset_row = 0
        if getattr(self, "_app", None) is not None:
            _offset_col, offset_row = self._app._active_window_offsets()
        return int(offset_row + getattr(elem, "_cell_row", 0) + getattr(elem, "row", 0))

    def _panel_title(self, name):
        if name == "cpp":
            return "C++"
        if name == "python":
            return "Python"
        return str(name)

    def _is_buffer_mode(self):
        """Buffer panel mode: interactive tester renders app frames into the panel."""
        return bool(self.config.apps)

    def _buffer_visible_size(self, bounds, count):
        count = max(1, int(count))
        available_w = max(1, int(bounds["width"]) // count)
        available_h = max(1, int(bounds["height"]))
        explicit = self.config.compare_viewport
        if not explicit:
            return available_w, available_h
        target_w = int(explicit.get("width") or DEFAULT_COMPARE_VIEWPORT_WIDTH)
        target_h = int(explicit.get("height") or DEFAULT_COMPARE_VIEWPORT_HEIGHT)
        return max(1, min(available_w, target_w)), max(1, min(available_h, target_h))

    def _frame_buffer_regions(self, names):
        bounds = self._app_viewport_rect()
        count = max(1, len(names))
        visible_w, visible_h = self._buffer_visible_size(bounds, count)
        return {
            name: {
                "row": 0,
                "col": index * visible_w,
                "width": visible_w,
                "height": visible_h,
            }
            for index, name in enumerate(names)
        }

    def _buffer_display_size(self, bounds, count):
        visible_w, visible_h = self._buffer_visible_size(bounds, count)
        return visible_w * max(1, int(count)), visible_h

    @staticmethod
    def _element_background_hex(element):
        style_getter = getattr(element, "get_style", None)
        style = style_getter() if callable(style_getter) else getattr(element, "style", None)
        background = getattr(style, "background", None)
        return getattr(background, "hex", None)

    def _fetch_frames_from_mcp(self):
        """Fetch render frames from all targets in parallel, then update cache."""
        items = list(self.targets.items())
        if not items:
            return
        frames = {}
        lock = threading.Lock()

        def fetch_one(name, target):
            try:
                frame = target.call_tool("get_render_frame", {})
                if frame:
                    with lock:
                        frames[name] = frame
            except Exception:
                pass

        threads = [threading.Thread(target=fetch_one, args=(n, t), daemon=True) for n, t in items]
        for t in threads:
            t.start()
        for t in threads:
            t.join()

        if frames:
            with self._cached_frames_lock:
                self._cached_frames.update(frames)

    def _repaint_buffer_targets(self):
        """Update the framebuffer element from cached frames. Fast — no MCP calls."""
        # Apply scope dim when log panel is in active scrollview scope.
        scope = getattr(self, "_active_scrollview_scope", None)
        log = getattr(self, "log_output", None)
        needs_dim = (scope is not None and log is not None and
                     scope.get("proxy") is log)
        names = list(self.targets.keys())
        if not names or not hasattr(self, "app_viewport"):
            return

        with self._cached_frames_lock:
            frames = dict(self._cached_frames)
        self.app_viewport.set_frames(frames, target_order=names, regions=self._frame_buffer_regions(names))
        self.app_viewport.set_pan(self.compare_pan["v"], self.compare_pan["h"])
        self.app_viewport.set_dimmed(needs_dim)
        self.app_viewport.set_interaction_state(self._buffer_panel_focused, self._buffer_panel_edit)
        if getattr(self, "_app", None) is not None:
            self._app.mark_dirty()

    def _buffer_panel_pan(self, key):
        """Apply pan delta for arrow key, clamped to valid range. Returns True if changed."""
        explicit = self.config.compare_viewport
        bounds = self._app_viewport_rect()
        count = max(1, len(self.targets))
        visible_w, visible_h = self._buffer_visible_size(bounds, count)
        if explicit:
            target_w = int(explicit.get("width", visible_w) or visible_w)
            target_h = int(explicit.get("height", visible_h) or visible_h)
        else:
            viewports = self.last_viewport or self._app_viewport_rects(self.targets.keys())
            first = next(iter(viewports.values()), None)
            target_w = int(first.get("width", visible_w)) if first else visible_w
            target_h = int(first.get("height", visible_h)) if first else visible_h
        max_v = max(0, target_h - visible_h)
        max_h = max(0, target_w - visible_w)
        prev_v, prev_h = self.compare_pan["v"], self.compare_pan["h"]
        if key == "Up":
            self.compare_pan["v"] = max(0, self.compare_pan["v"] - 1)
        elif key == "Down":
            self.compare_pan["v"] = min(max_v, self.compare_pan["v"] + 1)
        elif key == "Left":
            self.compare_pan["h"] = max(0, self.compare_pan["h"] - 1)
        elif key == "Right":
            self.compare_pan["h"] = min(max_h, self.compare_pan["h"] + 1)
        return self.compare_pan["v"] != prev_v or self.compare_pan["h"] != prev_h

    def _click_in_buffer_area(self, mouse_event):
        """Return True if mouse click is within the buffer panel area."""
        if not self._is_buffer_mode() or not self.targets:
            return False
        bounds = self._app_viewport_rect()
        visible_w, visible_h = self._buffer_display_size(bounds, len(self.targets))
        row = mouse_event.get("row", -1)
        col = mouse_event.get("col", -1)
        return (bounds["row"] <= row < bounds["row"] + visible_h and
                bounds["col"] <= col < bounds["col"] + visible_w)

    def handle_key(self, key):
        """Intercept keys for buffer panel focus/edit mode."""
        if not self._is_buffer_mode() or not self.targets:
            return super().handle_key(key)

        # Mouse click: focus+edit on panel, de-focus on click outside.
        if isinstance(key, dict) and key.get("type") == "mouse" and key.get("event") == "press":
            if self._click_in_buffer_area(key):
                self._set_buffer_panel_state(focused=True, edit=True)
                return True
            if self._buffer_panel_focused or self._buffer_panel_edit:
                self._set_buffer_panel_state(focused=False, edit=False)
            return super().handle_key(key)

        # --- Edit mode: arrows pan, Esc exits to focused ---
        if self._buffer_panel_edit:
            if key in ("Escape", "Enter"):
                self._set_buffer_panel_state(focused=True, edit=False)
                return True
            if key in ("Up", "Down", "Left", "Right"):
                self._buffer_panel_pan(key)
                self._repaint_buffer_targets()
                self._request_buffer_repaint()
                return True

        # --- Focused (not edit): Enter → edit, Esc/Tab → de-focus ---
        if self._buffer_panel_focused and not self._buffer_panel_edit:
            if key == "Enter":
                self._set_buffer_panel_state(focused=True, edit=True)
                return True
            if key == "Escape":
                self._set_buffer_panel_state(focused=False, edit=False)
                self._restore_pre_focus()
                return True
            # Any nav key: exit buffer panel, restore pre-focus, then navigate.
            self._set_buffer_panel_state(focused=False, edit=False)
            self._restore_pre_focus()
            return super().handle_key(key)

        # --- Navigation: Tab/arrows route through buffer panel ---
        _toolbar = {getattr(self, n, None) for n in ("run", "next", "pause", "copy", "quit")} - {None}
        _log = getattr(self, "log_output", None)

        if key == "Tab" and self._focused_element is getattr(self, "quit", None):
            self._set_buffer_panel_state(focused=True, edit=False)
            return True
        if key == "Shift+Tab" and self._focused_element is _log:
            self._set_buffer_panel_state(focused=True, edit=False)
            return True
        # Down from any toolbar button → buffer panel
        if key in ("Down", "ArrowDown") and self._focused_element in _toolbar:
            self._set_buffer_panel_state(focused=True, edit=False)
            return True
        # Up from log panel → buffer panel
        if (key in ("Up", "ArrowUp") and self._focused_element is _log
                and not self._edit_mode and self._active_scrollview_scope is None):
            self._set_buffer_panel_state(focused=True, edit=False)
            return True

        return super().handle_key(key)

    def _repaint_targets(self):
        controlled_output = self._uses_controlled_target_output()
        if controlled_output:
            if self._is_buffer_mode():
                self._request_buffer_repaint()
                return
            self._repaint_controlled_targets()
            return
        for target in self.targets.values():
            try:
                target.repaint()
                target.wait_output_idle()
            except Exception as exc:
                self._append_log(f"repaint failed for {target.name}: {exc}")

    def _repaint_controlled_targets(self):
        targets = list(self.targets.values())
        for target in targets:
            target.wait_output_idle(
                stable_seconds=CONTROLLED_OUTPUT_IDLE_SECONDS,
                timeout_seconds=CONTROLLED_OUTPUT_IDLE_TIMEOUT_SECONDS,
            )
            target.begin_output_capture()

        errors = {}
        threads = []

        def repaint_target(target):
            try:
                target.repaint()
            except Exception as exc:
                errors[target.name] = exc

        for target in targets:
            thread = threading.Thread(target=repaint_target, args=(target,))
            thread.start()
            threads.append(thread)

        for thread in threads:
            thread.join()

        output_chunks = []
        for target in targets:
            try:
                target.wait_output_idle(
                    stable_seconds=CONTROLLED_OUTPUT_IDLE_SECONDS,
                    timeout_seconds=CONTROLLED_OUTPUT_IDLE_TIMEOUT_SECONDS,
                )
            finally:
                output_chunks.append(target.end_output_capture())

        self._write_controlled_output(b"".join(data for data in output_chunks if data))

        for target_name, exc in errors.items():
            self._append_log(f"repaint failed for {target_name}: {exc}")

    def _write_controlled_output(self, data):
        if not data:
            return
        with OUTPUT_WRITE_LOCK:
            if getattr(sys.stdout, "isatty", lambda: False)():
                try:
                    os.write(sys.stdout.fileno(), TERMINAL_SYNC_UPDATE_BEGIN + data + TERMINAL_SYNC_UPDATE_END)
                    return
                except OSError:
                    pass
            stdout_buffer = getattr(sys.stdout, "buffer", None)
            if stdout_buffer is not None:
                stdout_buffer.write(data)
                stdout_buffer.flush()
            else:
                sys.stdout.write(data.decode("utf-8", errors="replace"))
                sys.stdout.flush()

    def _uses_controlled_target_output(self):
        return self._requires_controlled_target_output(len(self.targets))

    def _requires_controlled_target_output(self, target_count):
        return int(target_count) > 0

    def _repaint_controlled_animation_if_due(self):
        if self.finished or not self.running_script:
            return
        if not self._uses_controlled_target_output():
            return
        now = time.monotonic()
        if now - self.last_animation_repaint_at < CONTROLLED_ANIMATION_REPAINT_SECONDS:
            return
        self.last_animation_repaint_at = now
        self._repaint_targets()

    @staticmethod
    def _tool_needs_repaint(tool):
        return not str(tool).startswith("get_")

    def _append_log(self, line, kind=None):
        timestamp = time.strftime("%H:%M:%S")
        entry = f"{timestamp} {line}"
        self.log_output.append_line(entry, kind=kind)
        if self.config.log_path:
            with open(self.config.log_path, "a", encoding="utf-8") as handle:
                handle.write(entry + "\n")

    def _stop_if_exit_on_finish(self):
        if self.config.exit_on_finish and getattr(self, "_app", None) is not None:
            self._stop_targets()
            self._app._running = False

    def _finish_script(self, success):
        try:
            self._run_cleanup()
        except Exception as exc:
            self._append_log(f"FAIL cleanup: {exc}")
            self.exit_code = 1
            success = False

        self._append_script_summary(success)
        has_next_script = self.script_index + 1 < len(self.config.scripts)
        if has_next_script:
            self._stop_targets()
            self.finished = False
            self._start_next_script()
            return
        if not success:
            self._stop_targets()
        self._append_summary()
        self._stop_if_exit_on_finish()

    def _append_script_summary(self, success):
        status = "PASS" if success else "FAIL"
        summary = {
            "name": self.current_script.display_name if self.current_script is not None else "script",
            "status": status,
            "passed": self.script_assertions_passed,
            "failed": self.script_assertions_failed,
            "step_failures": self.script_step_failures,
        }
        self.script_summaries.append(summary)
        summary_line = (
            f"SUMMARY {summary['name']}: {status} "
            f"{summary['passed']} asserts passed, {summary['failed']} failed, "
            f"{summary['step_failures']} step failures"
        )
        kind = "log_result_ok" if success else "log_result_fail"
        self._append_log(self._result_separator(summary_line), kind=kind)
        self._append_log(summary_line, kind=kind)
        self._append_log("")

    def _append_summary(self):
        if self.summary_done:
            return
        self.summary_done = True
        if self.script_summaries:
            self._append_log("SUMMARY: all app scripts")
            for summary in self.script_summaries:
                self._append_log(
                    f"SUMMARY {summary['name']}: {summary['status']} "
                    f"{summary['passed']} asserts passed, {summary['failed']} failed, "
                    f"{summary['step_failures']} step failures"
                )
            self._append_log("RESULTS: all app scripts")
            for summary in self.script_summaries:
                kind = "log_result_ok" if summary["status"] == "PASS" else "log_result_fail"
                self._append_log(
                    f"RESULTS: {summary['name']}: {summary['status']} "
                    f"{summary['passed']} asserts passed, {summary['failed']} failed, "
                    f"{summary['step_failures']} step failures",
                    kind=kind,
                )
        if self.exit_code == 0:
            self._append_log("PASS: all MCP scripts completed")
        result_kind = (
            "log_result_ok"
            if self.exit_code == 0 and self.assertions_failed == 0 and self.step_failures == 0
            else "log_result_fail"
        )
        result = (
            f"RESULT: {self.assertions_passed} asserts passed, "
            f"{self.assertions_failed} failed, {self.step_failures} step failures"
        )
        separator = self._result_separator(result)
        self._append_log(separator, kind=result_kind)
        self._append_log(result, kind=result_kind)
        self._append_log(separator, kind=result_kind)

    def _result_separator(self, result):
        width = getattr(self.log_output, "width", None) or len(result)
        return RESULT_SEPARATOR_CHAR * max(len(result), int(width))


class TestScript:
    def __init__(self, source_path, apps, steps, setup_steps=None, cleanup_steps=None, env=None):
        self.source_path = source_path
        self.apps = apps
        self.steps = steps
        self.setup_steps = list(setup_steps or [])
        self.cleanup_steps = list(cleanup_steps or [])
        self.env = env or {}
        self.name = ", ".join(apps.keys())
        self.display_name = f"{source_path} [{self.name}]"


class TesterConfig:
    def __init__(
        self,
        root,
        apps,
        action_delay_ms,
        type_delay_ms,
        step_delay_seconds,
        source_path,
        steps,
        setup_steps=None,
        cleanup_steps=None,
        env=None,
        log_path=None,
        snapshot_dir=None,
        compare_viewport=None,
        exit_on_finish=False,
        compare_mode=False,
        plain=False,
        scripts=None,
    ):
        self.root = root
        self.apps = apps
        self.env = env or {}
        self.action_delay_ms = action_delay_ms
        self.type_delay_ms = type_delay_ms
        self.step_delay_seconds = step_delay_seconds
        self.source_path = source_path
        self.steps = steps
        self.setup_steps = list(setup_steps or [])
        self.cleanup_steps = list(cleanup_steps or [])
        self.log_path = log_path
        self.snapshot_dir = snapshot_dir
        self.compare_viewport = dict(compare_viewport or {})
        self.exit_on_finish = bool(exit_on_finish)
        self.compare_mode = bool(compare_mode)
        self.plain = bool(plain)
        self.scripts = list(scripts or [
            TestScript(source_path, apps, steps, self.setup_steps, self.cleanup_steps, self.env)
        ])


def parse_args(argv=None):
    parser = argparse.ArgumentParser(description="Universal visual MCP tester")
    parser.add_argument(
        "paths",
        nargs="*",
        metavar="path",
        help="APP_PATH_OR_ROOT YAML config",
    )
    parser.add_argument("--all", action="store_true", help="Run all known example MCP scripts")
    parser.add_argument("--app", help="App path or examples root directory")
    parser.add_argument(
        "--compare",
        nargs=2,
        metavar=("APP_A", "APP_B"),
        help="Run the same YAML test against two app processes at the same time",
    )
    parser.add_argument(
        "--mcp-fast",
        action="store_true",
        help="Run with zero MCP action, typing, and step delays.",
    )
    parser.add_argument("--mcp-action-delay-ms", type=int)
    parser.add_argument("--mcp-type-delay-ms", type=int)
    parser.add_argument("--step-delay-ms", type=int)
    parser.add_argument("--exit-on-finish", action="store_true")
    parser.add_argument("--log-file")
    parser.add_argument(
        "--snapshot-dir",
        default=DEFAULT_SNAPSHOT_DIR,
        help="Directory where compare snapshot mismatch bundles are written.",
    )
    parser.add_argument(
        "--compare-viewport",
        "--compare-app-size",
        default=None,
        metavar="WIDTHxHEIGHT",
        help="Fixed per-app viewport size. Omit to use the tester app area.",
    )
    parser.add_argument(
        "--plain",
        "--headless",
        action="store_true",
        help="Run without the interactive tester window.",
    )
    args = parser.parse_args(argv)

    root = _project_root()
    app_override = args.app
    compare_paths = args.compare
    if args.all:
        if compare_paths is not None and app_override is not None:
            parser.error("use either --compare or --app, not both")
        if args.paths:
            parser.error("--all does not accept a YAML config path")
        config_arg = ALL_EXAMPLES_CONFIG
    elif compare_paths is not None:
        if app_override is not None:
            parser.error("use either --compare or --app, not both")
        if len(args.paths) != 1:
            parser.error("--compare expects APP_A APP_B YAML config")
        config_arg = args.paths[0]
    elif len(args.paths) == 1:
        config_arg = args.paths[0]
    elif len(args.paths) == 2:
        if app_override is not None:
            parser.error("use either --app or positional APP_PATH, not both")
        app_override, config_arg = args.paths
    else:
        parser.error("expected APP_PATH_OR_ROOT YAML config")

    config_path = _abs_path(root, config_arg)
    log_path = _safe_test_path(root, args.log_file) if args.log_file else None
    snapshot_dir = _safe_test_path(root, args.snapshot_dir) if args.snapshot_dir else None
    if log_path:
        os.makedirs(os.path.dirname(log_path), exist_ok=True)
        with open(log_path, "w", encoding="utf-8"):
            pass
    if snapshot_dir:
        os.makedirs(snapshot_dir, exist_ok=True)
    compare_viewport = _parse_viewport_size(args.compare_viewport, parser)
    scripts = _load_yaml_test_scripts(root, config_path)
    if compare_paths is not None:
        scripts = _compare_app_paths(root, scripts, compare_paths)
    elif app_override is not None:
        scripts = _override_app_paths(root, scripts, app_override)
    default_action_delay_ms = 0 if args.mcp_fast else DEFAULT_ACTION_DELAY_MS
    default_type_delay_ms = 0 if args.mcp_fast else DEFAULT_TYPE_DELAY_MS
    default_step_delay_ms = 0 if args.mcp_fast else int(DEFAULT_STEP_DELAY_SECONDS * 1000)
    action_delay_ms = (
        default_action_delay_ms
        if args.mcp_action_delay_ms is None
        else args.mcp_action_delay_ms
    )
    type_delay_ms = (
        default_type_delay_ms
        if args.mcp_type_delay_ms is None
        else args.mcp_type_delay_ms
    )
    step_delay_ms = default_step_delay_ms if args.step_delay_ms is None else args.step_delay_ms
    first_script = scripts[0]
    return TesterConfig(
        root=root,
        apps=first_script.apps,
        env=first_script.env,
        action_delay_ms=action_delay_ms,
        type_delay_ms=type_delay_ms,
        step_delay_seconds=max(0, step_delay_ms) / 1000.0,
        source_path=os.path.relpath(config_path, root),
        steps=first_script.steps,
        setup_steps=first_script.setup_steps,
        cleanup_steps=first_script.cleanup_steps,
        log_path=log_path,
        snapshot_dir=snapshot_dir,
        compare_viewport=compare_viewport,
        exit_on_finish=args.exit_on_finish,
        compare_mode=compare_paths is not None,
        plain=args.plain,
        scripts=scripts,
    )


def main(argv=None):
    config = parse_args(argv)
    if config.plain or not getattr(sys.stdout, "isatty", lambda: False)():
        config.plain = True
        return _run_headless(config)
    app = UIApplication()
    window = McpTester(config)
    app.open(window)
    app.run()
    return window.exit_code


def _run_headless(config):
    app = UIApplication()
    window = McpTester(config)
    app.open(window)
    try:
        while not window.summary_done:
            window.tick()
            time.sleep(HEADLESS_TICK_SECONDS)
    finally:
        window.close()
    return window.exit_code


def _read_json_line(sock):
    chunks = []
    while True:
        chunk = sock.recv(1)
        if not chunk:
            break
        if chunk == b"\n":
            break
        chunks.append(chunk)
    if not chunks:
        raise RuntimeError("empty MCP response")
    return json.loads(b"".join(chunks).decode("utf-8"))


def _parse_viewport_size(value, parser):
    if value is None or str(value).strip().lower() in ("", "auto"):
        return {}
    match = re.fullmatch(r"\s*(\d+)x(\d+)\s*", str(value or ""))
    if not match:
        parser.error("--compare-viewport / --compare-app-size expects WIDTHxHEIGHT or omit for auto")
    width = int(match.group(1))
    height = int(match.group(2))
    if width <= 0 or height <= 0:
        parser.error("--compare-viewport / --compare-app-size dimensions must be positive")
    return {"width": width, "height": height}


def _decode_tool_result(result):
    content = (result or {}).get("content", [])
    if not content:
        return result
    text = content[0].get("text", "")
    return json.loads(text) if text else {}


def _load_yaml_test_config(root, path):
    scripts = _load_yaml_test_scripts(root, path)
    apps = {}
    steps = []
    setup_steps = []
    cleanup_steps = []
    env = {}
    for script in scripts:
        for name, app_path in script.apps.items():
            if name in apps:
                raise ValueError(f"Duplicate app name in YAML includes: {name}")
            apps[name] = app_path
            env[name] = dict(script.env.get(name, {}))
        setup_steps.extend(script.setup_steps)
        cleanup_steps.extend(script.cleanup_steps)
        steps.extend(script.steps)
    return apps, steps, setup_steps, cleanup_steps, env


def _override_app_paths(root, scripts, app_path):
    resolved = _resolve_config_path(root, os.path.join(root, scripts[0].source_path), str(app_path))
    if len(scripts) == 1 and len(scripts[0].apps) == 1 and not os.path.isdir(resolved):
        return [_script_with_apps(scripts[0], {next(iter(scripts[0].apps)): resolved})]

    if not os.path.isdir(resolved):
        raise ValueError("multiple-script app override requires an examples root directory")

    result = []
    for script in scripts:
        if len(script.apps) != 1:
            raise ValueError("examples root override requires every script to contain exactly one app")
        name = next(iter(script.apps))
        result.append(_script_with_apps(script, {name: _app_path_from_examples_root(resolved, name)}))
    return result


def _compare_app_paths(root, scripts, app_paths):
    resolved_paths = [
        _resolve_config_path(root, os.path.join(root, scripts[0].source_path), str(app_path))
        for app_path in app_paths
    ]
    names = _compare_target_names(resolved_paths)

    result = []
    for script in scripts:
        result.append(_compare_script_app_paths(root, script, resolved_paths, names, len(scripts) > 1))
    return result


def _compare_script_app_paths(root, script, resolved_paths, names, require_examples_roots):
    if len(script.apps) != 1:
        raise ValueError("--compare requires each YAML test script to contain exactly one app")

    original_name = next(iter(script.apps))
    config_path = os.path.join(root, script.source_path)
    apps = {}
    for name, app_path in zip(names, resolved_paths):
        if os.path.isdir(app_path):
            apps[name] = _app_path_from_examples_root(app_path, original_name)
        elif require_examples_roots:
            raise ValueError("--compare with include aggregate requires examples root directories")
        else:
            apps[name] = _resolve_config_path(root, config_path, str(app_path))

    original_env = dict(script.env.get(original_name, {}))
    env = {name: _format_target_value(original_env, name) for name in names}
    steps = [
        _compare_step(step, original_name)
        for step in script.steps
    ]
    return TestScript(
        script.source_path,
        apps,
        steps,
        script.setup_steps,
        script.cleanup_steps,
        env,
    )


def _format_target_value(value, target_name):
    if isinstance(value, str):
        return value.replace("{platform}", str(target_name)).replace("{target}", str(target_name))
    if isinstance(value, list):
        return [_format_target_value(item, target_name) for item in value]
    if isinstance(value, tuple):
        return tuple(_format_target_value(item, target_name) for item in value)
    if isinstance(value, dict):
        return {
            _format_target_value(key, target_name): _format_target_value(item, target_name)
            for key, item in value.items()
        }
    return value


def _has_target_placeholder(value):
    if isinstance(value, str):
        return any(placeholder in value for placeholder in TARGET_PLACEHOLDERS)
    if isinstance(value, dict):
        return any(
            _has_target_placeholder(key) or _has_target_placeholder(item)
            for key, item in value.items()
        )
    if isinstance(value, (list, tuple)):
        return any(_has_target_placeholder(item) for item in value)
    return False


def _comparison_assertion_fields(assertion):
    if assertion is None:
        return None
    return [
        key
        for key, expected in assertion.items()
        if not _has_target_placeholder(expected)
    ]


def _assertion_matches(actual, expected):
    if isinstance(expected, dict):
        known_ops = {"contains", "regex", "equals"}
        op_keys = [key for key in expected if key in known_ops]
        if len(op_keys) > 1:
            raise ValueError(f"assertion has multiple operators: {', '.join(op_keys)}")
        if op_keys:
            op = op_keys[0]
            value = expected[op]
            if op == "contains":
                return _contains_value(actual, value), f"contains {value!r}"
            if op == "regex":
                return re.search(str(value), "" if actual is None else str(actual)) is not None, f"matches /{value}/"
            return actual == value, f"== {value!r}"
    return actual == expected, f"== {expected!r}"


def _contains_value(actual, expected):
    if isinstance(actual, str):
        return str(expected) in actual
    if isinstance(actual, dict):
        return expected in actual or expected in actual.values()
    try:
        return expected in actual
    except TypeError:
        return str(expected) in str(actual)


def _render_snapshot_mismatch(expected, actual):
    expected_cells = _render_snapshot_cells(expected)
    actual_cells = _render_snapshot_cells(actual)
    if len(actual_cells) != len(expected_cells):
        return f"height expected {len(expected_cells)}, got {len(actual_cells)}"
    for row_index, (expected_row, actual_row) in enumerate(zip(expected_cells, actual_cells)):
        if len(actual_row) != len(expected_row):
            return f"row {row_index} width expected {len(expected_row)}, got {len(actual_row)}"
        for col_index, (expected_cell, actual_cell) in enumerate(zip(expected_row, actual_row)):
            expected_cell = _visual_cell(expected_cell)
            actual_cell = _visual_cell(actual_cell)
            if actual_cell != expected_cell:
                return (
                    f"row {row_index} col {col_index} "
                    f"expected {_format_cell(expected_cell)}, got {_format_cell(actual_cell)}"
                )
    return None


def _safe_filename(value):
    cleaned = re.sub(r"[^A-Za-z0-9_.-]+", "_", str(value)).strip("._")
    return cleaned or "snapshot"


def _render_snapshot_cells(snapshot):
    if not isinstance(snapshot, dict):
        raise ValueError("render snapshot must be an object")
    cells = snapshot.get("cells")
    if cells is not None:
        return [
            [_render_snapshot_cell(cell) for cell in row]
            for row in cells
        ]
    text_lines = snapshot.get("text_lines")
    if text_lines is not None:
        return [
            [{"char": char, "foreground": None, "background": None, "attributes": []} for char in str(line)]
            for line in text_lines
        ]
    raise ValueError("render snapshot must contain cells")


def _render_snapshot_cell(cell):
    if isinstance(cell, dict):
        return {
            "char": cell.get("char", " "),
            "foreground": cell.get("foreground"),
            "background": cell.get("background"),
            "attributes": cell.get("attributes", []),
        }
    if isinstance(cell, list):
        return {
            "char": cell[0] if len(cell) > 0 else " ",
            "foreground": cell[1] if len(cell) > 1 else None,
            "background": cell[2] if len(cell) > 2 else None,
            "attributes": cell[3] if len(cell) > 3 else [],
        }
    return {
        "char": str(cell),
        "foreground": None,
        "background": None,
        "attributes": [],
    }


def _visual_cell(cell):
    if not isinstance(cell, dict):
        return cell
    result = dict(cell)
    if result.get("char") == " ":
        result["foreground"] = None
    return result


def _format_cell(cell):
    if not isinstance(cell, dict):
        return repr(cell)
    return (
        f"char={cell.get('char')!r} "
        f"fg={cell.get('foreground')!r} "
        f"bg={cell.get('background')!r} "
        f"attrs={cell.get('attributes', [])!r}"
    )


def _compare_step(step, original_name):
    normalized = dict(step)
    if _first_present(normalized, TESTER_ACTION_KEYS) is not None or "compare" in normalized:
        return normalized
    if "tool" not in normalized:
        return normalized

    target = normalized.get("target")
    if target not in (None, original_name, "*"):
        raise ValueError(f"--compare cannot map YAML target {target!r}")
    normalized["target"] = "*"
    return normalized


def _compare_target_names(paths):
    names = [_compare_target_name(path, index) for index, path in enumerate(paths)]
    if names[0] != names[1]:
        return names
    return ["app_a", "app_b"]


def _compare_target_name(path, index):
    parts = set(os.path.normpath(path).split(os.sep))
    if "python" in parts or str(path).endswith(".py"):
        return "python"
    if "cpp" in parts:
        return "cpp"
    base = _target_name_from_path(str(path))
    return base or f"app_{index + 1}"


def _script_with_apps(script, apps):
    return TestScript(
        script.source_path,
        apps,
        script.steps,
        script.setup_steps,
        script.cleanup_steps,
        script.env,
    )


def _app_path_from_examples_root(examples_root, name):
    candidates = [
        os.path.join(examples_root, name, f"{name}.py"),
        os.path.join(examples_root, name, name),
        os.path.join(examples_root, f"{name}.py"),
        os.path.join(examples_root, name),
    ]
    for candidate in candidates:
        if os.path.exists(candidate):
            return candidate
    raise ValueError(f"cannot resolve app {name!r} under examples root: {examples_root}")


def _load_yaml_test_scripts(root, path, seen=None):
    path = os.path.abspath(path)
    seen = set(seen or ())
    if path in seen:
        raise ValueError(f"Recursive MCP tester include: {os.path.relpath(path, root)}")
    seen.add(path)

    data = _read_yaml_file(path)
    if not isinstance(data, list):
        raise ValueError("MCP tester YAML root must be an array")

    scripts = []
    app_items = []
    for index, item in enumerate(data):
        if not isinstance(item, dict):
            raise ValueError(f"YAML item {index + 1} must be a mapping")
        include_value = _first_present(item, INCLUDE_KEYS)
        if include_value is not None:
            for include_path in _include_paths(include_value):
                resolved = _resolve_config_path(root, path, include_path)
                scripts.extend(_load_yaml_test_scripts(root, resolved, seen))
            remaining_keys = set(item) - set(INCLUDE_KEYS)
            if not remaining_keys:
                continue
        app_items.append(item)

    if app_items:
        scripts.append(_script_from_yaml_items(root, path, app_items))
    if not scripts:
        raise ValueError("MCP tester YAML must contain at least one app or include")
    seen.remove(path)
    return scripts


def _script_from_yaml_items(root, path, data):
    apps = {}
    steps = []
    setup_steps = []
    cleanup_steps = []
    env = {}
    for index, item in enumerate(data):
        if not isinstance(item, dict):
            raise ValueError(f"YAML app item {index + 1} must be a mapping")

        app_path = _first_present(item, APP_PATH_KEYS)
        if not app_path:
            raise ValueError(f"YAML app item {index + 1} is missing app/path")

        name = item.get("name") or _target_name_from_path(str(app_path))
        if name in apps:
            raise ValueError(f"Duplicate app name in YAML: {name}")
        apps[name] = _resolve_config_path(root, path, str(app_path))
        env[name] = _normalize_env(_first_present(item, ENV_KEYS) or {})

        setup = _first_present(item, SETUP_KEYS) or []
        cleanup = _first_present(item, CLEANUP_KEYS) or []
        if not isinstance(setup, list):
            raise ValueError(f"YAML app item {name} setup must be an array")
        if not isinstance(cleanup, list):
            raise ValueError(f"YAML app item {name} cleanup must be an array")
        setup_steps.extend(_normalize_tester_steps(setup, name))
        cleanup_steps = _normalize_tester_steps(cleanup, name) + cleanup_steps

        tests = _first_present(item, TEST_KEYS)
        if not isinstance(tests, list):
            raise ValueError(f"YAML app item {name} must contain tests array")

        for step in tests:
            if not isinstance(step, dict):
                raise ValueError(f"YAML test step for {name} must be a mapping")
            normalized = dict(step)
            normalized.setdefault("target", name)
            steps.append(normalized)

    if not apps:
        raise ValueError("MCP tester YAML must contain at least one app")
    if not steps:
        raise ValueError("MCP tester YAML must contain at least one test step")
    return TestScript(
        os.path.relpath(path, root),
        apps,
        steps,
        setup_steps,
        cleanup_steps,
        env,
    )


def _include_paths(value):
    if isinstance(value, list):
        return [str(item) for item in value]
    return [str(value)]


def _normalize_env(value):
    if not isinstance(value, dict):
        raise ValueError("YAML env must be a mapping")
    return {str(key): str(item) for key, item in value.items()}


def _normalize_tester_steps(steps, app_name):
    normalized = []
    for step in steps:
        if not isinstance(step, dict):
            raise ValueError(f"YAML tester step for {app_name} must be a mapping")
        normalized.append(dict(step))
    return normalized


def _run_tester_file_step(root, step):
    action = _first_present(step, TESTER_ACTION_KEYS)
    if action is None:
        raise ValueError("tester step is missing action")
    action = str(action)
    if action == "mkdir":
        path = _safe_test_path(root, step["path"])
        os.makedirs(path, exist_ok=True)
        return {"ok": True, "path": os.path.relpath(path, root)}
    if action == "write_file":
        path = _safe_test_path(root, step["path"])
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w", encoding="utf-8") as handle:
            handle.write(_file_step_content(step))
        return {"ok": True, "path": os.path.relpath(path, root)}
    if action == "append_file":
        path = _safe_test_path(root, step["path"])
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "a", encoding="utf-8") as handle:
            handle.write(_file_step_content(step))
        return {"ok": True, "path": os.path.relpath(path, root)}
    if action == "remove":
        path = _safe_test_path(root, step["path"])
        if os.path.isdir(path):
            shutil.rmtree(path)
        elif os.path.exists(path):
            os.remove(path)
        return {"ok": True, "path": os.path.relpath(path, root)}
    if action == "assert_exists":
        path = _safe_test_path(root, step["path"])
        if not os.path.exists(path):
            raise AssertionError(f"path does not exist: {os.path.relpath(path, root)}")
        return {"ok": True, "path": os.path.relpath(path, root)}
    if action == "assert_missing":
        path = _safe_test_path(root, step["path"])
        if os.path.exists(path):
            raise AssertionError(f"path exists: {os.path.relpath(path, root)}")
        return {"ok": True, "path": os.path.relpath(path, root)}
    if action == "assert_file_content":
        path = _safe_test_path(root, step["path"])
        with open(path, encoding="utf-8") as handle:
            actual = handle.read()
        expected = _file_step_content(step)
        if actual != expected:
            raise AssertionError(f"file content mismatch: {os.path.relpath(path, root)}")
        return {"ok": True, "path": os.path.relpath(path, root)}
    if action == "assert_file_contains":
        path = _safe_test_path(root, step["path"])
        with open(path, encoding="utf-8") as handle:
            actual = handle.read()
        expected = _file_step_content(step)
        if expected not in actual:
            raise AssertionError(f"file does not contain expected text: {os.path.relpath(path, root)}")
        return {"ok": True, "path": os.path.relpath(path, root)}
    raise ValueError(f"unknown tester action: {action}")


def _file_step_content(step):
    lines = _first_present(step, FILE_LINE_KEYS)
    if lines is not None:
        if not isinstance(lines, list):
            raise ValueError("file step lines must be an array")
        text = "\n".join(str(line) for line in lines)
        return text + "\n"
    content = _first_present(step, FILE_CONTENT_KEYS)
    return "" if content is None else str(content)


def _safe_test_path(root, path):
    result = os.path.abspath(_abs_path(root, str(path)))
    root = os.path.abspath(root)
    if result == root or not result.startswith(root + os.sep):
        raise ValueError(f"refusing path outside project root: {path}")
    return result


def _tester_step_label(step):
    action = _first_present(step, TESTER_ACTION_KEYS)
    path = step.get("path")
    if path is None:
        return str(action)
    return f"{action} {path}"


def _is_tester_assert_step(step):
    action = _first_present(step, TESTER_ACTION_KEYS)
    return str(action).startswith("assert_")


def _read_yaml_file(path):
    with open(path, encoding="utf-8") as f:
        return _parse_yaml(f.read())


def _parse_yaml(content):
    lines = _yaml_lines(content)
    if not lines:
        return []
    value, index = _parse_yaml_block(lines, 0, lines[0][0])
    if index != len(lines):
        raise ValueError(f"Unexpected YAML content near line {lines[index][2]}")
    return value


def _yaml_lines(content):
    result = []
    for line_number, raw in enumerate(content.splitlines(), start=1):
        if "\t" in raw:
            raise ValueError(f"YAML cannot contain tabs, line {line_number}")
        stripped = raw.strip()
        if not stripped or stripped.startswith("#"):
            continue
        indent = len(raw) - len(raw.lstrip(" "))
        result.append((indent, stripped, line_number))
    return result


def _parse_yaml_block(lines, index, indent):
    if lines[index][0] < indent:
        return None, index
    if lines[index][1].startswith(YAML_LIST_MARKER):
        return _parse_yaml_list(lines, index, indent)
    return _parse_yaml_mapping(lines, index, indent)


def _parse_yaml_list(lines, index, indent):
    result = []
    while index < len(lines):
        line_indent, stripped, line_number = lines[index]
        if line_indent < indent:
            break
        if line_indent != indent or not stripped.startswith(YAML_LIST_MARKER):
            break

        item_text = stripped[len(YAML_LIST_MARKER):].strip()
        index += 1
        if not item_text:
            if index < len(lines) and lines[index][0] > indent:
                item, index = _parse_yaml_block(lines, index, lines[index][0])
            else:
                item = None
            result.append(item)
            continue

        if _looks_like_yaml_pair(item_text):
            item = {}
            key, value = _parse_yaml_pair(item_text, line_number)
            if value == "":
                if index < len(lines) and lines[index][0] > indent:
                    item[key], index = _parse_yaml_block(lines, index, lines[index][0])
                else:
                    item[key] = {}
            else:
                item[key] = _parse_yaml_scalar(value)

            if index < len(lines) and lines[index][0] > indent:
                extra, index = _parse_yaml_mapping(lines, index, lines[index][0])
                item.update(extra)
            result.append(item)
        else:
            result.append(_parse_yaml_scalar(item_text))
    return result, index


def _parse_yaml_mapping(lines, index, indent):
    result = {}
    while index < len(lines):
        line_indent, stripped, line_number = lines[index]
        if line_indent < indent:
            break
        if line_indent != indent or stripped.startswith(YAML_LIST_MARKER):
            break

        key, value = _parse_yaml_pair(stripped, line_number)
        index += 1
        if value == "":
            if index < len(lines) and lines[index][0] > indent:
                result[key], index = _parse_yaml_block(lines, index, lines[index][0])
            else:
                result[key] = {}
        else:
            result[key] = _parse_yaml_scalar(value)
    return result, index


def _looks_like_yaml_pair(value):
    if ":" not in value:
        return False
    key, _separator, _rest = value.partition(":")
    return bool(key.strip())


def _parse_yaml_pair(value, line_number):
    if ":" not in value:
        raise ValueError(f"Expected YAML key/value on line {line_number}")
    key, _separator, scalar = value.partition(":")
    key = key.strip().strip('"').strip("'")
    if not key:
        raise ValueError(f"Empty YAML key on line {line_number}")
    return key, scalar.strip()


def _parse_yaml_scalar(value):
    value = value.strip()
    if not value:
        return ""

    if (
        (value.startswith('"') and value.endswith('"'))
        or (value.startswith("'") and value.endswith("'"))
    ):
        try:
            return ast.literal_eval(value)
        except (SyntaxError, ValueError):
            return value[1:-1]

    if value in ("null", "Null", "NULL", "~"):
        return None
    if value in ("true", "True", "TRUE"):
        return True
    if value in ("false", "False", "FALSE"):
        return False

    if value.startswith(("{", "[")) and value.endswith(("}", "]")):
        try:
            return ast.literal_eval(value)
        except (SyntaxError, ValueError):
            pass

    try:
        return int(value)
    except ValueError:
        pass
    try:
        return float(value)
    except ValueError:
        return value


def _first_present(mapping, keys):
    for key in keys:
        if key in mapping:
            return mapping[key]
    return None


def _target_name_from_path(path):
    base = os.path.basename(path.rstrip(os.sep))
    name, _ext = os.path.splitext(base)
    return name or "app"


def _resolve_config_path(root, config_path, value):
    if os.path.isabs(value):
        return value
    config_relative = os.path.normpath(os.path.join(os.path.dirname(config_path), value))
    if os.path.exists(config_relative):
        return config_relative
    return os.path.normpath(os.path.join(root, value))


def _find_free_port():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind((DEFAULT_HOST, 0))
        return sock.getsockname()[1]


def _set_pty_size(fd, viewport):
    if fd is None:
        return
    try:
        rows = max(1, int(viewport["height"]))
        cols = max(1, int(viewport["width"]))
        fcntl.ioctl(fd, termios.TIOCSWINSZ, struct.pack("HHHH", rows, cols, 0, 0))
    except (KeyError, OSError, TypeError, ValueError):
        pass


def _select_snapshot_fields(value, fields):
    if not fields:
        return value
    return {field: _snapshot_field_value(value, field) for field in fields}


def _snapshot_field_value(value, field):
    current = value
    for part in str(field).split("."):
        if isinstance(current, dict) and part in current:
            current = current[part]
            continue
        return None
    return current


def _compact_result(value):
    if not isinstance(value, dict):
        return repr(value)
    if "id" in value and "value" in value:
        return f"{value.get('id')} value={value.get('value')!r}"
    if "window" in value and "elements" in value:
        elements = value.get("elements") or []
        return f"state elements={len(elements)}"
    if "lines" in value:
        return f"snapshot lines={len(value.get('lines') or [])}"
    keys = sorted(value.keys())
    shown = {key: value[key] for key in keys[:4]}
    suffix = "" if len(keys) <= 4 else " ..."
    return f"{shown}{suffix}"


def _abs_path(root, path):
    return path if os.path.isabs(path) else os.path.join(root, path)


def _project_root():
    current = os.path.realpath(os.getcwd())
    while current != os.path.dirname(current):
        if os.path.isdir(os.path.join(current, "tests", "mcp")):
            return current
        current = os.path.dirname(current)
    return os.path.realpath(os.getcwd())


if __name__ == "__main__":
    sys.exit(main())
