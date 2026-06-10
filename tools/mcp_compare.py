#!/usr/bin/env python3
"""Replay one MCP action sequence against two UI servers and compare snapshots."""

import argparse
import json
import socket
import sys


DEFAULT_HOST = "127.0.0.1"
DEFAULT_LEFT_PORT = 8765
DEFAULT_RIGHT_PORT = 8766
SOCKET_TIMEOUT_SECONDS = 5
JSONRPC_VERSION = "2.0"
SNAPSHOT_TOOLS = ("get_state", "get_text_snapshot")


class MCPClient:
    def __init__(self, host, port):
        self.host = host
        self.port = int(port)
        self._next_id = 1

    def call_tool(self, name, arguments=None):
        return self.request("tools/call", {
            "name": name,
            "arguments": arguments or {},
        })

    def request(self, method, params=None):
        request_id = self._next_id
        self._next_id += 1
        payload = {
            "jsonrpc": JSONRPC_VERSION,
            "id": request_id,
            "method": method,
            "params": params or {},
        }
        with socket.create_connection((self.host, self.port), timeout=SOCKET_TIMEOUT_SECONDS) as sock:
            sock.sendall((json.dumps(payload) + "\n").encode("utf-8"))
            response = _read_json_line(sock)
        if "error" in response:
            raise RuntimeError(response["error"]["message"])
        result = response.get("result")
        if method == "tools/call":
            return _decode_tool_result(result)
        return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("sequence", help="JSON file containing a list of MCP tool calls")
    parser.add_argument("--left-host", default=DEFAULT_HOST)
    parser.add_argument("--left-port", type=int, default=DEFAULT_LEFT_PORT)
    parser.add_argument("--right-host", default=DEFAULT_HOST)
    parser.add_argument("--right-port", type=int, default=DEFAULT_RIGHT_PORT)
    parser.add_argument("--snapshot-tool", action="append", choices=SNAPSHOT_TOOLS)
    args = parser.parse_args()

    snapshot_tools = args.snapshot_tool or list(SNAPSHOT_TOOLS)
    sequence = _load_sequence(args.sequence)
    left = MCPClient(args.left_host, args.left_port)
    right = MCPClient(args.right_host, args.right_port)

    mismatches = []
    for index, step in enumerate(sequence):
        tool_name = step["tool"]
        arguments = step.get("arguments", {})
        left.call_tool(tool_name, arguments)
        right.call_tool(tool_name, arguments)

        for snapshot_tool in snapshot_tools:
            left_snapshot = left.call_tool(snapshot_tool)
            right_snapshot = right.call_tool(snapshot_tool)
            normalized_left = _normalize_snapshot(left_snapshot)
            normalized_right = _normalize_snapshot(right_snapshot)
            if normalized_left != normalized_right:
                mismatches.append({
                    "step": index,
                    "tool": tool_name,
                    "snapshot_tool": snapshot_tool,
                    "left": normalized_left,
                    "right": normalized_right,
                })

    if mismatches:
        print(json.dumps({"ok": False, "mismatches": mismatches}, indent=2))
        return 1

    print(json.dumps({"ok": True, "steps": len(sequence), "snapshot_tools": snapshot_tools}, indent=2))
    return 0


def _load_sequence(path):
    with open(path, encoding="utf-8") as f:
        sequence = json.load(f)
    if not isinstance(sequence, list):
        raise ValueError("sequence file must contain a JSON list")
    for step in sequence:
        if not isinstance(step, dict) or "tool" not in step:
            raise ValueError("each sequence step must be an object with a tool field")
    return sequence


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


def _decode_tool_result(result):
    content = (result or {}).get("content", [])
    if not content:
        return result
    text = content[0].get("text", "")
    return json.loads(text) if text else None


def _normalize_snapshot(value):
    if isinstance(value, list):
        return [_normalize_snapshot(item) for item in value]
    if not isinstance(value, dict):
        return value

    normalized = {}
    for key, item in value.items():
        if key in {"backend"}:
            continue
        normalized[key] = _normalize_snapshot(item)
    return normalized


if __name__ == "__main__":
    sys.exit(main())
