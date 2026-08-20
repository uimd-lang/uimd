#!/usr/bin/env python3
"""Protocol smoke tests for Rust MCP stdio, TCP, and HTTP transports."""

from __future__ import annotations

import argparse
from dataclasses import dataclass
import json
from pathlib import Path
import socket
import subprocess
import sys
import threading
import time
from typing import Any

try:
    from go_direct_terminal_smoke import (
        DEFAULT_COLS,
        DEFAULT_QUIET_SECONDS,
        DEFAULT_ROWS,
        DEFAULT_STOP_SECONDS,
        PtyApp,
        wait_for_screen_text,
    )
except ModuleNotFoundError:
    from tools.go_direct_terminal_smoke import (
        DEFAULT_COLS,
        DEFAULT_QUIET_SECONDS,
        DEFAULT_ROWS,
        DEFAULT_STOP_SECONDS,
        PtyApp,
        wait_for_screen_text,
    )


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_START_TIMEOUT_SECONDS = 5.0
DEFAULT_RESPONSE_TIMEOUT_SECONDS = 5.0
GUI_MCP_MODAL_STRESS_ITERATIONS = 64


@dataclass(frozen=True)
class TransportTarget:
    display_name: str
    backend_name: str
    server_name: str


RUST_TARGET = TransportTarget("Rust", "rust", "uimd-rust")


def request(identifier: int, method: str, params: dict[str, Any] | None = None) -> dict[str, Any]:
    value: dict[str, Any] = {"jsonrpc": "2.0", "id": identifier, "method": method}
    if params is not None:
        value["params"] = params
    return value


def assert_tools_contract(response: dict[str, Any], target: TransportTarget) -> None:
    tools = response["result"]["tools"]
    definitions = {tool["name"]: tool for tool in tools}
    for name in (
        "get_schema",
        "get_state",
        "get_cursor",
        "get_selection",
        "get_text",
        "move_cursor",
        "select_all",
        "cut_selection",
        "query_tasks",
        "get_task",
        "update_task",
        "delete_task",
    ):
        if name not in definitions:
            raise AssertionError(f"{target.display_name} tools/list is missing {name!r}")
        schema = definitions[name].get("inputSchema")
        if not isinstance(schema, dict) or schema.get("type") != "object":
            raise AssertionError(
                f"{target.display_name} tool {name!r} has no object input schema"
            )
    if not definitions["query_tasks"].get("description"):
        raise AssertionError("generated query_tasks description is empty")
    if "outputSchema" not in definitions["query_tasks"]:
        raise AssertionError("generated query_tasks output schema is missing")


def tool_result(response: dict[str, Any]) -> Any:
    return json.loads(response["result"]["content"][0]["text"])


def read_json_line(stream: Any, target: TransportTarget) -> Any:
    line = stream.readline()
    if not line:
        raise AssertionError(
            f"{target.display_name} MCP process closed before returning a response"
        )
    return json.loads(line)


def run_stdio(binary: Path, target: TransportTarget) -> None:
    process = subprocess.Popen(
        [
            str(binary),
            "--mcp-server",
            "--headless",
            "--mcp-transport",
            "stdio",
            "--viewport",
            "0,0,90,35",
        ],
        cwd=ROOT,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    assert process.stdin is not None
    assert process.stdout is not None
    process.stdin.write(json.dumps(request(1, "initialize")) + "\n")
    process.stdin.flush()
    initialized = read_json_line(process.stdout, target)
    if initialized["result"]["serverInfo"]["name"] != target.server_name:
        raise AssertionError(
            f"unexpected {target.display_name} MCP server info: {initialized!r}"
        )

    process.stdin.write(json.dumps(request(2, "tools/list")) + "\n")
    process.stdin.flush()
    assert_tools_contract(read_json_line(process.stdout, target), target)

    process.stdin.write(
        json.dumps(
            request(
                3,
                "tools/call",
                {"name": "get_window", "arguments": {}},
            )
        )
        + "\n"
    )
    process.stdin.flush()
    window = tool_result(read_json_line(process.stdout, target))
    expected_window = {
        "backend": target.backend_name,
        "class": "TaskBoard",
        "mode": "fullscreen",
        "width": 90,
        "height": 35,
        "mcp_enabled": True,
    }
    for field, expected in expected_window.items():
        if window.get(field) != expected:
            raise AssertionError(
                f"{target.display_name} get_window {field!r} was "
                f"{window.get(field)!r}, expected {expected!r}"
            )
    if not window.get("description"):
        raise AssertionError(f"{target.display_name} get_window description is empty")

    process.stdin.write(
        json.dumps(
            request(
                4,
                "tools/call",
                {"name": "get_schema", "arguments": {}},
            )
        )
        + "\n"
    )
    process.stdin.flush()
    schema = tool_result(read_json_line(process.stdout, target))
    if schema.get("window") != window:
        raise AssertionError(
            f"{target.display_name} get_schema window metadata differs from get_window"
        )
    if not schema.get("elements"):
        raise AssertionError(f"{target.display_name} get_schema returned no exposed elements")

    process.stdin.write(
        json.dumps(
            request(
                5,
                "tools/call",
                {"name": "query_tasks", "arguments": {"status": "Doing"}},
            )
        )
        + "\n"
    )
    process.stdin.flush()
    app_result = read_json_line(process.stdout, target)
    content = json.loads(app_result["result"]["content"][0]["text"])
    if not isinstance(content, dict) or "tasks" not in content:
        raise AssertionError(f"generated query_tasks returned the wrong shape: {content!r}")

    batch = [
        {"jsonrpc": "2.0", "method": "initialize"},
        request(6, "tools/list"),
    ]
    process.stdin.write(json.dumps(batch) + "\n")
    process.stdin.flush()
    batch_result = read_json_line(process.stdout, target)
    if len(batch_result) != 1 or batch_result[0]["id"] != 6:
        raise AssertionError(
            f"{target.display_name} batch did not suppress notification success: "
            f"{batch_result!r}"
        )
    process.stdin.close()
    return_code = process.wait(timeout=DEFAULT_RESPONSE_TIMEOUT_SECONDS)
    if return_code != 0:
        stderr = process.stderr.read() if process.stderr is not None else ""
        raise AssertionError(
            f"{target.display_name} stdio MCP exited {return_code}: {stderr}"
        )
    print(
        f"PASS {target.display_name} MCP stdio metadata, app tools, batch, and notifications",
        flush=True,
    )


def reserve_port() -> int:
    with socket.socket() as listener:
        listener.bind(("127.0.0.1", 0))
        return int(listener.getsockname()[1])


def connect(port: int) -> socket.socket:
    deadline = time.monotonic() + DEFAULT_START_TIMEOUT_SECONDS
    while True:
        try:
            connection = socket.create_connection(
                ("127.0.0.1", port),
                timeout=DEFAULT_RESPONSE_TIMEOUT_SECONDS,
            )
            connection.settimeout(DEFAULT_RESPONSE_TIMEOUT_SECONDS)
            return connection
        except OSError:
            if time.monotonic() >= deadline:
                raise
            time.sleep(0.02)


def start_server(binary: Path, transport: str, port: int) -> subprocess.Popen[bytes]:
    return subprocess.Popen(
        [
            str(binary),
            "--mcp-server",
            "--headless",
            "--mcp-transport",
            transport,
            "--mcp-host",
            "127.0.0.1",
            "--mcp-port",
            str(port),
            "--viewport",
            "0,0,90,35",
        ],
        cwd=ROOT,
        stdin=subprocess.DEVNULL,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )


def stop_server(process: subprocess.Popen[bytes]) -> None:
    if process.poll() is None:
        process.terminate()
    process.wait(timeout=DEFAULT_RESPONSE_TIMEOUT_SECONDS)


def run_tcp(binary: Path, target: TransportTarget) -> None:
    port = reserve_port()
    process = start_server(binary, "tcp", port)
    try:
        with connect(port) as delayed:
            time.sleep(0.1)
            assert_tools_contract(
                tcp_request(port, request(10, "tools/list"), target),
                target,
            )
            delayed.sendall((json.dumps(request(11, "tools/list")) + "\n").encode())
            reader = delayed.makefile("r", encoding="utf-8")
            assert_tools_contract(json.loads(reader.readline()), target)
    finally:
        stop_server(process)
    print(
        f"PASS {target.display_name} MCP TCP newline transport accepts a later client "
        "while the first request is delayed",
        flush=True,
    )


def tcp_request(
    port: int,
    value: dict[str, Any],
    target: TransportTarget,
) -> dict[str, Any]:
    with connect(port) as connection:
        connection.sendall((json.dumps(value) + "\n").encode())
        reader = connection.makefile("r", encoding="utf-8")
        response = reader.readline()
        if not response:
            raise AssertionError(
                f"{target.display_name} GUI MCP connection closed without a response"
            )
        return json.loads(response)


def run_gui_tcp(binary: Path, target: TransportTarget) -> None:
    port = reserve_port()
    command = [
        str(binary),
        "--mcp-server",
        "--mcp-transport",
        "tcp",
        "--mcp-host",
        "127.0.0.1",
        "--mcp-port",
        str(port),
        "--viewport",
        f"0,0,{DEFAULT_COLS},{DEFAULT_ROWS}",
        "--mcp-fast",
        "--mcp-wait-render",
        "--mcp-controlled-render",
    ]
    with PtyApp(command, ROOT, DEFAULT_ROWS, DEFAULT_COLS) as app:
        wait_for_screen_text(app, "Task Board")
        focused = tool_result(
            tcp_request(
                port,
                request(
                    15,
                    "tools/call",
                    {
                        "name": "focus_element",
                        "arguments": {"element_id": "new_task_btn"},
                    },
                ),
                target,
            )
        )
        if focused.get("id") != "new_task_btn":
            raise AssertionError(f"GUI MCP focused the wrong element: {focused!r}")
        app.send(b"\t")
        app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        moved = tool_result(
            tcp_request(
                port,
                request(
                    16,
                    "tools/call",
                    {"name": "get_focused_element", "arguments": {}},
                ),
                target,
            )
        )
        if moved.get("id") in (None, "new_task_btn"):
            raise AssertionError(
                f"direct terminal input did not remain active beside GUI MCP: {moved!r}"
            )
        drain_stop = threading.Event()

        def drain_terminal_output() -> None:
            while not drain_stop.is_set():
                app.drain(total_seconds=DEFAULT_QUIET_SECONDS)

        drain_thread = threading.Thread(target=drain_terminal_output, daemon=True)
        drain_thread.start()
        try:
            for iteration in range(GUI_MCP_MODAL_STRESS_ITERATIONS):
                request_id = 1000 + iteration * 4
                tcp_request(
                    port,
                    request(
                        request_id,
                        "tools/call",
                        {
                            "name": "activate_element",
                            "arguments": {"element_id": "clear_board_btn"},
                        },
                    ),
                    target,
                )
                modal = tool_result(
                    tcp_request(
                        port,
                        request(
                            request_id + 1,
                            "tools/call",
                            {"name": "get_window", "arguments": {}},
                        ),
                        target,
                    )
                )
                if modal.get("class") != "MessageBoxYesNo":
                    raise AssertionError(
                        f"GUI MCP modal stress opened the wrong window: {modal!r}"
                    )
                tcp_request(
                    port,
                    request(
                        request_id + 2,
                        "tools/call",
                        {
                            "name": "activate_element",
                            "arguments": {"element_id": "no_btn"},
                        },
                    ),
                    target,
                )
                root = tool_result(
                    tcp_request(
                        port,
                        request(
                            request_id + 3,
                            "tools/call",
                            {"name": "get_window", "arguments": {}},
                        ),
                        target,
                    )
                )
                if root.get("class") != "TaskBoard":
                    raise AssertionError(
                        f"GUI MCP modal stress did not restore the root window: {root!r}"
                    )
        finally:
            drain_stop.set()
            drain_thread.join(timeout=DEFAULT_STOP_SECONDS)
        app.send(b"\x03")
        if app.process is None:
            raise AssertionError(
                f"{target.display_name} GUI MCP PTY process did not start"
            )
        app.process.wait(timeout=DEFAULT_STOP_SECONDS)
        app.drain(total_seconds=DEFAULT_QUIET_SECONDS)
        output = bytes(app.output)
        if b"\x1b[?1049h" in output:
            raise AssertionError("embedded GUI MCP viewport entered the alternate screen")
        if b"\x1b[?25l" not in output or b"\x1b[?25h" not in output:
            raise AssertionError("embedded GUI MCP viewport did not restore cursor visibility")
    print(
        f"PASS {target.display_name} GUI MCP concurrent terminal input, render waits, "
        "and repeated modal lifecycle",
        flush=True,
    )


def post_http(port: int, body: Any, *, origin: str = "http://localhost") -> tuple[int, bytes]:
    encoded = json.dumps(body).encode()
    headers = (
        "POST /mcp HTTP/1.1\r\n"
        "Host: 127.0.0.1\r\n"
        f"Origin: {origin}\r\n"
        "Accept: application/json, text/event-stream\r\n"
        "MCP-Protocol-Version: 2024-11-05\r\n"
        "Content-Type: application/json\r\n"
        f"Content-Length: {len(encoded)}\r\n"
        "Connection: close\r\n"
        "\r\n"
    ).encode()
    with connect(port) as connection:
        connection.sendall(headers + encoded)
        response = bytearray()
        while True:
            chunk = connection.recv(65536)
            if not chunk:
                break
            response.extend(chunk)
    head, _, response_body = bytes(response).partition(b"\r\n\r\n")
    status = int(head.split(b" ", 2)[1])
    return status, response_body


def run_http(binary: Path, target: TransportTarget) -> None:
    port = reserve_port()
    process = start_server(binary, "http", port)
    try:
        with connect(port) as delayed:
            time.sleep(0.1)
            status, body = post_http(port, request(20, "tools/list"))
            if status != 200:
                raise AssertionError(
                    f"{target.display_name} HTTP tools/list returned status {status}"
                )
            assert_tools_contract(json.loads(body), target)

            encoded = json.dumps(request(21, "tools/list")).encode()
            delayed.sendall(
                (
                    "POST /mcp HTTP/1.1\r\n"
                    "Host: 127.0.0.1\r\n"
                    "Origin: http://localhost\r\n"
                    "Accept: application/json\r\n"
                    "MCP-Protocol-Version: 2024-11-05\r\n"
                    "Content-Type: application/json\r\n"
                    f"Content-Length: {len(encoded)}\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                ).encode()
                + encoded
            )
            response = bytearray()
            while True:
                chunk = delayed.recv(65536)
                if not chunk:
                    break
                response.extend(chunk)
            head, _, body = bytes(response).partition(b"\r\n\r\n")
            status = int(head.split(b" ", 2)[1])
            if status != 200:
                raise AssertionError(
                    f"{target.display_name} delayed HTTP tools/list returned status {status}"
                )
            assert_tools_contract(json.loads(body), target)

        status, body = post_http(port, {"jsonrpc": "2.0", "method": "initialize"})
        if status != 202 or body:
            raise AssertionError(
                f"{target.display_name} HTTP notification returned "
                f"status/body {status}/{body!r}"
            )
        status, _ = post_http(
            port,
            request(22, "initialize"),
            origin="https://example.com",
        )
        if status != 403:
            raise AssertionError(
                f"{target.display_name} HTTP cross-origin request returned {status}"
            )
    finally:
        stop_server(process)
    print(
        f"PASS {target.display_name} MCP HTTP endpoint accepts a later client "
        "while the first request is delayed",
        flush=True,
    )


def run_unsupported_transport(binary: Path, target: TransportTarget) -> None:
    completed = subprocess.run(
        [
            str(binary),
            "--mcp-server",
            "--headless",
            "--mcp-transport",
            "websocket",
        ],
        cwd=ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        timeout=DEFAULT_RESPONSE_TIMEOUT_SECONDS,
        check=False,
    )
    expected_error = f"unsupported {target.display_name} MCP transport"
    if completed.returncode == 0 or expected_error.lower() not in completed.stderr.lower():
        raise AssertionError(
            f"unsupported {target.display_name} MCP transport did not fail clearly: "
            f"{completed.returncode}/{completed.stderr!r}"
        )
    print(
        f"PASS {target.display_name} unsupported MCP transport fails clearly",
        flush=True,
    )


def run_transport_smoke(binary: Path, target: TransportTarget) -> int:
    sys.path.insert(0, str(ROOT / "src"))
    from uimd.testing.artifact_manifest import validate_artifact_paths

    validate_artifact_paths(ROOT, [binary])
    if not binary.exists():
        raise FileNotFoundError(
            f"missing {target.display_name} MCP smoke binary: {binary}"
        )
    run_stdio(binary, target)
    run_tcp(binary, target)
    run_gui_tcp(binary, target)
    run_http(binary, target)
    run_unsupported_transport(binary, target)
    print(
        f"PASS {target.display_name} MCP transport smoke: 5/5 groups passed",
        flush=True,
    )
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--binary",
        type=Path,
        default=ROOT / "rust/examples/task_board/target/release/task_board",
    )
    args = parser.parse_args()
    binary = args.binary if args.binary.is_absolute() else ROOT / args.binary
    return run_transport_smoke(binary, RUST_TARGET)


if __name__ == "__main__":
    sys.exit(main())
