"""Smoke tests for MCP transport entry points."""

import json
import os
import socket
import subprocess
import sys
import time
import unittest
import urllib.request


ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.join(ROOT_DIR, "python"))

from runtime.mcp import MCPController

PYTHON_FORMULAR = os.path.join(ROOT_DIR, "python", "examples", "formular", "formular.py")
CPP_FORMULAR = os.path.join(ROOT_DIR, "cpp", "build", "examples", "formular", "formular")
NETWORK_TESTS_ENV = "UI_MCP_NETWORK_TESTS"
EXPECTED_TOOL_NAMES = {tool["name"] for tool in MCPController(app=None).list_tools()}


class TestMCPTransportSmoke(unittest.TestCase):
    def test_python_headless_defaults_to_stdio(self):
        result = _stdio_call(["python3", PYTHON_FORMULAR, "--mcp-server", "--headless"])
        self.assertEqual(result["result"]["content"][0]["type"], "text")
        self.assertIn("FormApp", result["result"]["content"][0]["text"])

    def test_python_stdio_exposes_complete_tool_inventory(self):
        result = _stdio_call(["python3", PYTHON_FORMULAR, "--mcp-server", "--headless"], _tools_list_request())
        self.assertEqual(_tool_names(result), EXPECTED_TOOL_NAMES)

    def test_python_stdio_accepts_batch_requests(self):
        result = _stdio_call(["python3", PYTHON_FORMULAR, "--mcp-server", "--headless"], _batch_request())
        _assert_batch_get_window_result(result)

    @unittest.skipUnless(os.path.exists(CPP_FORMULAR), "C++ formular binary is not built")
    def test_cpp_headless_defaults_to_stdio(self):
        result = _stdio_call([CPP_FORMULAR, "--mcp-server", "--headless"])
        self.assertEqual(result["result"]["content"][0]["type"], "text")
        self.assertIn("FormApp", result["result"]["content"][0]["text"])

    @unittest.skipUnless(os.path.exists(CPP_FORMULAR), "C++ formular binary is not built")
    def test_cpp_stdio_exposes_complete_tool_inventory(self):
        result = _stdio_call([CPP_FORMULAR, "--mcp-server", "--headless"], _tools_list_request())
        self.assertEqual(_tool_names(result), EXPECTED_TOOL_NAMES)

    @unittest.skipUnless(os.path.exists(CPP_FORMULAR), "C++ formular binary is not built")
    def test_cpp_stdio_accepts_batch_requests(self):
        result = _stdio_call([CPP_FORMULAR, "--mcp-server", "--headless"], _batch_request())
        _assert_batch_get_window_result(result)

    @unittest.skipUnless(os.environ.get(NETWORK_TESTS_ENV), "network transport smoke tests are opt-in")
    def test_python_tcp_exposes_complete_tool_inventory(self):
        result = _tcp_call(["python3", PYTHON_FORMULAR], _unused_tcp_port(), _tools_list_request())
        self.assertEqual(_tool_names(result), EXPECTED_TOOL_NAMES)

    @unittest.skipUnless(os.environ.get(NETWORK_TESTS_ENV), "network transport smoke tests are opt-in")
    def test_python_tcp_accepts_batch_requests(self):
        result = _tcp_call(["python3", PYTHON_FORMULAR, "--headless"], _unused_tcp_port(), _batch_request())
        _assert_batch_get_window_result(result)

    @unittest.skipUnless(
        os.path.exists(CPP_FORMULAR) and os.environ.get(NETWORK_TESTS_ENV),
        "C++ TCP smoke test requires built binary and opt-in network tests",
    )
    def test_cpp_tcp_exposes_complete_tool_inventory(self):
        result = _tcp_call([CPP_FORMULAR], _unused_tcp_port(), _tools_list_request())
        self.assertEqual(_tool_names(result), EXPECTED_TOOL_NAMES)

    @unittest.skipUnless(
        os.path.exists(CPP_FORMULAR) and os.environ.get(NETWORK_TESTS_ENV),
        "C++ TCP smoke test requires built binary and opt-in network tests",
    )
    def test_cpp_tcp_accepts_batch_requests(self):
        result = _tcp_call([CPP_FORMULAR, "--headless"], _unused_tcp_port(), _batch_request())
        _assert_batch_get_window_result(result)

    @unittest.skipUnless(os.environ.get(NETWORK_TESTS_ENV), "network transport smoke tests are opt-in")
    def test_python_http_transport(self):
        result = _http_call(["python3", PYTHON_FORMULAR], _unused_tcp_port())
        self.assertIn("FormApp", result["result"]["content"][0]["text"])

    @unittest.skipUnless(os.environ.get(NETWORK_TESTS_ENV), "network transport smoke tests are opt-in")
    def test_python_http_exposes_complete_tool_inventory(self):
        result = _http_call(["python3", PYTHON_FORMULAR], _unused_tcp_port(), _tools_list_request())
        self.assertEqual(_tool_names(result), EXPECTED_TOOL_NAMES)

    @unittest.skipUnless(os.environ.get(NETWORK_TESTS_ENV), "network transport smoke tests are opt-in")
    def test_python_http_accepts_batch_requests(self):
        result = _http_call(["python3", PYTHON_FORMULAR, "--headless"], _unused_tcp_port(), _batch_request())
        _assert_batch_get_window_result(result)

    @unittest.skipUnless(
        os.path.exists(CPP_FORMULAR) and os.environ.get(NETWORK_TESTS_ENV),
        "C++ HTTP smoke test requires built binary and opt-in network tests",
    )
    def test_cpp_http_transport(self):
        result = _http_call([CPP_FORMULAR], _unused_tcp_port())
        self.assertIn("FormApp", result["result"]["content"][0]["text"])

    @unittest.skipUnless(
        os.path.exists(CPP_FORMULAR) and os.environ.get(NETWORK_TESTS_ENV),
        "C++ HTTP smoke test requires built binary and opt-in network tests",
    )
    def test_cpp_http_exposes_complete_tool_inventory(self):
        result = _http_call([CPP_FORMULAR], _unused_tcp_port(), _tools_list_request())
        self.assertEqual(_tool_names(result), EXPECTED_TOOL_NAMES)

    @unittest.skipUnless(
        os.path.exists(CPP_FORMULAR) and os.environ.get(NETWORK_TESTS_ENV),
        "C++ HTTP smoke test requires built binary and opt-in network tests",
    )
    def test_cpp_http_accepts_batch_requests(self):
        result = _http_call([CPP_FORMULAR, "--headless"], _unused_tcp_port(), _batch_request())
        _assert_batch_get_window_result(result)

def _stdio_call(command, request=None):
    request = request or _tool_call_request("get_window")
    process = subprocess.Popen(
        command,
        cwd=ROOT_DIR,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    stdout, stderr = process.communicate(json.dumps(request) + "\n", timeout=5)
    if process.returncode != 0:
        raise AssertionError(stderr)
    return json.loads(stdout)


def _unused_tcp_port():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind(("127.0.0.1", 0))
        return sock.getsockname()[1]


def _tcp_call(base_command, port, request=None):
    request = request or _tool_call_request("get_window")
    process = subprocess.Popen(
        base_command + ["--mcp-server", "--mcp-transport", "tcp", "--mcp-port", str(port)],
        cwd=ROOT_DIR,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        deadline = time.monotonic() + 5
        last_error = None
        while time.monotonic() < deadline:
            try:
                with socket.create_connection(("127.0.0.1", port), timeout=1) as sock:
                    sock.sendall((json.dumps(request) + "\n").encode("utf-8"))
                    return json.loads(sock.makefile("r", encoding="utf-8").readline())
            except OSError as exc:
                last_error = exc
                time.sleep(0.05)
        raise AssertionError(f"TCP MCP server did not start: {last_error}")
    finally:
        process.terminate()
        try:
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait(timeout=5)


def _http_call(base_command, port, request=None):
    request = request or _tool_call_request("get_window")
    process = subprocess.Popen(
        base_command + ["--mcp-server", "--mcp-transport", "http", "--mcp-port", str(port)],
        cwd=ROOT_DIR,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        time.sleep(0.5)
        http_request = urllib.request.Request(
            f"http://127.0.0.1:{port}/mcp",
            data=json.dumps(request).encode("utf-8"),
            headers={
                "Accept": "application/json, text/event-stream",
                "Content-Type": "application/json",
            },
        )
        with urllib.request.urlopen(http_request, timeout=5) as response:
            return json.loads(response.read().decode("utf-8"))
    finally:
        process.terminate()
        try:
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait(timeout=5)


def _tool_call_request(name, arguments=None):
    return {
        "jsonrpc": "2.0",
        "id": 1,
        "method": "tools/call",
        "params": {
            "name": name,
            "arguments": arguments or {},
        },
    }


def _tools_list_request():
    return {
        "jsonrpc": "2.0",
        "id": 1,
        "method": "tools/list",
        "params": {},
    }


def _batch_request():
    return [
        {
            "jsonrpc": "2.0",
            "method": "tools/call",
            "params": {
                "name": "ui.fill",
                "arguments": {"element_id": "name_input", "text": "Batch User"},
            },
        },
        {
            "jsonrpc": "2.0",
            "id": 2,
            "method": "tools/call",
            "params": {
                "name": "ui.get_value",
                "arguments": {"element_id": "name_input"},
            },
        },
        {
            "jsonrpc": "2.0",
            "id": 3,
            "method": "tools/call",
            "params": {
                "name": "get_window",
                "arguments": {},
            },
        },
    ]


def _tool_names(response):
    return {tool["name"] for tool in response["result"]["tools"]}


def _assert_batch_get_window_result(response):
    self_check = response
    assert isinstance(self_check, list)
    assert len(self_check) == 2
    assert self_check[0]["id"] == 2
    assert "Batch User" in self_check[0]["result"]["content"][0]["text"]
    assert self_check[1]["id"] == 3
    assert "FormApp" in self_check[1]["result"]["content"][0]["text"]


if __name__ == "__main__":
    unittest.main()
