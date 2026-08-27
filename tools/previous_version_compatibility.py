#!/usr/bin/env python3
"""Compile and exercise frozen previous-version applications against current runtimes."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import queue
import shutil
import socket
import subprocess
import sys
import tempfile
import threading
import time
import traceback
from typing import Any, Callable

from java_toolchain import java_build_env


ROOT = Path(__file__).resolve().parents[1]
FIXTURE_VERSION = "0.5.3"
FIXTURE_ROOT = ROOT / "tests/compatibility" / f"v{FIXTURE_VERSION}"
MANIFEST_PATH = FIXTURE_ROOT / "manifest.sha256"
VIEWPORT = "0,0,90,35"
RESPONSE_TIMEOUT_SECONDS = 15.0
BUILD_TIMEOUT_SECONDS = 15 * 60.0
EXCLUDED_DIRECTORY_NAMES = {
    ".build",
    ".gradle",
    "__pycache__",
    "build",
    "obj",
    "target",
}
EXPECTED_TEXT = "abc"
EXPECTED_COMBO_VALUE = "seven"


def is_windows() -> bool:
    return platform.system().lower() == "windows"


def executable_name(name: str) -> str:
    return f"{name}.exe" if is_windows() else name


def merged_env(updates: dict[str, str] | None = None) -> dict[str, str]:
    result = os.environ.copy()
    if updates:
        result.update(updates)
    return result


def command_text(command: list[str | Path]) -> str:
    return " ".join(str(part) for part in command)


def run_command(
    command: list[str | Path],
    *,
    cwd: Path = ROOT,
    env: dict[str, str] | None = None,
) -> None:
    print(f"==> {command_text(command)}", flush=True)
    subprocess.run(
        [str(part) for part in command],
        cwd=cwd,
        env=merged_env(env),
        check=True,
        timeout=BUILD_TIMEOUT_SECONDS,
    )


def required_command(name: str, *, fallback: Path | None = None) -> str:
    override = os.environ.get(name.upper())
    if override:
        return override
    discovered = shutil.which(name)
    if discovered:
        return discovered
    if fallback is not None and fallback.is_file():
        return str(fallback)
    raise FileNotFoundError(f"required command was not found: {name}")


def fixture_files() -> list[Path]:
    files: list[Path] = []
    for path in FIXTURE_ROOT.rglob("*"):
        if not path.is_file() or path == MANIFEST_PATH:
            continue
        relative = path.relative_to(FIXTURE_ROOT)
        if any(part in EXCLUDED_DIRECTORY_NAMES for part in relative.parts):
            continue
        if path.suffix in {".pyc", ".pyo"} or path.name == ".DS_Store":
            continue
        files.append(path)
    return sorted(files, key=lambda path: path.relative_to(FIXTURE_ROOT).as_posix())


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def read_manifest() -> dict[str, str]:
    if not MANIFEST_PATH.is_file():
        raise FileNotFoundError(f"compatibility manifest is missing: {MANIFEST_PATH}")
    entries: dict[str, str] = {}
    for line_number, raw_line in enumerate(MANIFEST_PATH.read_text(encoding="utf-8").splitlines(), 1):
        line = raw_line.strip()
        if not line:
            continue
        digest, separator, relative = line.partition("  ")
        if not separator or len(digest) != 64 or not relative:
            raise ValueError(
                f"invalid compatibility manifest line {line_number}: {raw_line!r}"
            )
        entries[relative] = digest
    return entries


def verify_manifest() -> None:
    expected = read_manifest()
    actual_paths = fixture_files()
    actual_names = {path.relative_to(FIXTURE_ROOT).as_posix() for path in actual_paths}
    expected_names = set(expected)
    if actual_names != expected_names:
        missing = sorted(expected_names - actual_names)
        unexpected = sorted(actual_names - expected_names)
        raise AssertionError(
            "frozen compatibility fixture file set changed; "
            f"missing={missing}, unexpected={unexpected}"
        )
    changed = [
        name
        for name in sorted(expected)
        if sha256(FIXTURE_ROOT / name) != expected[name]
    ]
    if changed:
        raise AssertionError(
            "frozen compatibility fixture contents changed: " + ", ".join(changed)
        )


def json_request(identifier: int, method: str, params: dict[str, Any] | None = None) -> dict[str, Any]:
    request: dict[str, Any] = {
        "jsonrpc": "2.0",
        "id": identifier,
        "method": method,
    }
    if params is not None:
        request["params"] = params
    return request


def decoded_tool_result(response: dict[str, Any], label: str, name: str) -> Any:
    result = response.get("result", {})
    content = result.get("content", [])
    text = content[0].get("text", "") if content else ""
    if result.get("isError"):
        raise AssertionError(f"{label} MCP tool {name} failed: {text}")
    try:
        return json.loads(text)
    except json.JSONDecodeError:
        return text


class McpSession:
    def __init__(
        self,
        command: list[str | Path],
        *,
        label: str,
        env: dict[str, str] | None = None,
    ) -> None:
        self.label = label
        self.next_identifier = 1
        self.lines: queue.Queue[str | None] = queue.Queue()
        self.stderr_lines: list[str] = []
        launch_command = [
            *[str(part) for part in command],
            "--mcp-server",
            "--headless",
            "--mcp-transport",
            "stdio",
            "--viewport",
            VIEWPORT,
        ]
        print(f"==> {command_text(launch_command)}", flush=True)
        self.process = subprocess.Popen(
            launch_command,
            cwd=ROOT,
            env=merged_env(env),
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
        )
        assert self.process.stdout is not None
        assert self.process.stderr is not None
        self.stdout_thread = threading.Thread(target=self._read_stdout, daemon=True)
        self.stderr_thread = threading.Thread(target=self._read_stderr, daemon=True)
        self.stdout_thread.start()
        self.stderr_thread.start()

    def _read_stdout(self) -> None:
        assert self.process.stdout is not None
        for line in self.process.stdout:
            self.lines.put(line)
        self.lines.put(None)

    def _read_stderr(self) -> None:
        assert self.process.stderr is not None
        self.stderr_lines.extend(self.process.stderr.readlines())

    def response(self, method: str, params: dict[str, Any] | None = None) -> dict[str, Any]:
        assert self.process.stdin is not None
        identifier = self.next_identifier
        self.next_identifier += 1
        self.process.stdin.write(json.dumps(json_request(identifier, method, params)) + "\n")
        self.process.stdin.flush()
        try:
            line = self.lines.get(timeout=RESPONSE_TIMEOUT_SECONDS)
        except queue.Empty as exc:
            raise AssertionError(f"{self.label} MCP timed out while waiting for {method}") from exc
        if line is None:
            detail = "".join(self.stderr_lines).strip()
            raise AssertionError(
                f"{self.label} MCP exited before responding to {method}: {detail}"
            )
        response = json.loads(line)
        if response.get("id") != identifier:
            raise AssertionError(
                f"{self.label} MCP returned response id {response.get('id')!r}, "
                f"expected {identifier}"
            )
        if "error" in response:
            raise AssertionError(f"{self.label} MCP {method} failed: {response['error']!r}")
        return response

    def tool(self, name: str, arguments: dict[str, Any] | None = None) -> Any:
        response = self.response(
            "tools/call",
            {"name": name, "arguments": arguments or {}},
        )
        return decoded_tool_result(response, self.label, name)

    def close(self) -> None:
        if self.process.stdin is not None and not self.process.stdin.closed:
            self.process.stdin.close()
        try:
            return_code = self.process.wait(timeout=RESPONSE_TIMEOUT_SECONDS)
        except subprocess.TimeoutExpired:
            self.process.kill()
            self.process.wait()
            raise AssertionError(f"{self.label} MCP did not stop after stdin closed")
        if return_code != 0:
            detail = "".join(self.stderr_lines).strip()
            raise AssertionError(f"{self.label} MCP exited {return_code}: {detail}")

    def abort(self) -> None:
        if self.process.poll() is None:
            self.process.kill()
        self.process.wait()


def reserve_port() -> int:
    with socket.socket() as listener:
        listener.bind(("127.0.0.1", 0))
        return int(listener.getsockname()[1])


class TcpMcpSession:
    def __init__(
        self,
        command: list[str | Path],
        *,
        label: str,
        env: dict[str, str] | None = None,
    ) -> None:
        self.label = label
        self.next_identifier = 1
        self.port = reserve_port()
        self.stdout_lines: list[str] = []
        self.stderr_lines: list[str] = []
        launch_command = [
            *[str(part) for part in command],
            "--mcp-server",
            "--headless",
            "--mcp-transport",
            "tcp",
            "--mcp-port",
            str(self.port),
            "--viewport",
            VIEWPORT,
        ]
        print(f"==> {command_text(launch_command)}", flush=True)
        self.process = subprocess.Popen(
            launch_command,
            cwd=ROOT,
            env=merged_env(env),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        assert self.process.stdout is not None
        assert self.process.stderr is not None
        self.stdout_thread = threading.Thread(
            target=lambda: self.stdout_lines.extend(self.process.stdout.readlines()),
            daemon=True,
        )
        self.stderr_thread = threading.Thread(
            target=lambda: self.stderr_lines.extend(self.process.stderr.readlines()),
            daemon=True,
        )
        self.stdout_thread.start()
        self.stderr_thread.start()

    def response(self, method: str, params: dict[str, Any] | None = None) -> dict[str, Any]:
        identifier = self.next_identifier
        self.next_identifier += 1
        payload = json.dumps(json_request(identifier, method, params)) + "\n"
        deadline = time.monotonic() + RESPONSE_TIMEOUT_SECONDS
        while True:
            if self.process.poll() is not None:
                detail = "".join(self.stderr_lines).strip()
                raise AssertionError(
                    f"{self.label} MCP exited before responding to {method}: {detail}"
                )
            try:
                connection = socket.create_connection(
                    ("127.0.0.1", self.port),
                    timeout=RESPONSE_TIMEOUT_SECONDS,
                )
                break
            except OSError as exc:
                if time.monotonic() >= deadline:
                    raise AssertionError(
                        f"{self.label} MCP did not listen on port {self.port}"
                    ) from exc
                time.sleep(0.02)
        with connection:
            connection.sendall(payload.encode("utf-8"))
            response_bytes = bytearray()
            while not response_bytes.endswith(b"\n"):
                block = connection.recv(65536)
                if not block:
                    break
                response_bytes.extend(block)
        if not response_bytes:
            raise AssertionError(f"{self.label} MCP returned no response for {method}")
        response = json.loads(response_bytes.decode("utf-8"))
        if response.get("id") != identifier:
            raise AssertionError(
                f"{self.label} MCP returned response id {response.get('id')!r}, "
                f"expected {identifier}"
            )
        if "error" in response:
            raise AssertionError(f"{self.label} MCP {method} failed: {response['error']!r}")
        return response

    def tool(self, name: str, arguments: dict[str, Any] | None = None) -> Any:
        response = self.response(
            "tools/call",
            {"name": name, "arguments": arguments or {}},
        )
        return decoded_tool_result(response, self.label, name)

    def close(self) -> None:
        if self.process.poll() is None:
            self.process.terminate()
        try:
            self.process.wait(timeout=RESPONSE_TIMEOUT_SECONDS)
        except subprocess.TimeoutExpired:
            self.process.kill()
            self.process.wait()

    def abort(self) -> None:
        self.close()


def render_rect(value: dict[str, Any]) -> tuple[int, int, int, int]:
    left = int(value.get("col", value.get("left", value.get("x", 0))))
    top = int(value.get("row", value.get("top", value.get("y", 0))))
    width = int(value.get("width", 0))
    height = int(value.get("height", 0))
    if width <= 0 or height <= 0:
        raise AssertionError(f"active render rectangle is empty: {value!r}")
    return left, top, left + width, top + height


def text_lines(snapshot: dict[str, Any], label: str) -> list[str]:
    lines = snapshot.get("text_lines")
    if not isinstance(lines, list) or not all(isinstance(line, str) for line in lines):
        raise AssertionError(f"{label} render snapshot has no text_lines")
    return lines


def surface_text_matches(
    lines: list[str],
    needle: str,
    rect: tuple[int, int, int, int],
) -> list[tuple[int, int]]:
    left, top, right, bottom = rect
    matches: list[tuple[int, int]] = []
    for row in range(top, min(bottom, len(lines))):
        start = left
        while True:
            col = lines[row].find(needle, start, right)
            if col < 0:
                break
            matches.append((col, row))
            start = col + 1
    return matches


def find_surface_text(
    lines: list[str],
    needle: str,
    rect: tuple[int, int, int, int],
    label: str,
) -> tuple[int, int]:
    matches = surface_text_matches(lines, needle, rect)
    if len(matches) != 1:
        raise AssertionError(
            f"{label} expected one {needle!r} occurrence in the active surface, "
            f"found {matches}"
        )
    return matches[0]


def element_bounds(snapshot: dict[str, Any]) -> tuple[int, int, int, int]:
    value = snapshot.get("bounds", snapshot.get("rect"))
    if not isinstance(value, dict):
        raise AssertionError(f"element snapshot has no bounds: {snapshot!r}")
    left = int(value.get("left", value.get("x", 0)))
    top = int(value.get("top", value.get("y", 0)))
    right = int(value.get("right", left + int(value.get("width", 0))))
    bottom = int(value.get("bottom", top + int(value.get("height", 0))))
    if right <= left or bottom <= top:
        raise AssertionError(f"element snapshot has empty bounds: {snapshot!r}")
    return left, top, right, bottom


def exercise_runtime(
    command: list[str | Path],
    label: str,
    *,
    env: dict[str, str] | None = None,
    transport: str = "stdio",
) -> None:
    session_type = TcpMcpSession if transport == "tcp" else McpSession
    session = session_type(command, label=label, env=env)
    try:
        initialized = session.response("initialize")
        if "serverInfo" not in initialized.get("result", {}):
            raise AssertionError(f"{label} initialize returned no serverInfo")

        tools_response = session.response("tools/list")
        listed_tools = tools_response.get("result", {})
        tool_definitions = (
            listed_tools
            if isinstance(listed_tools, list)
            else listed_tools.get("tools", [])
        )
        tool_names = {
            definition.get("name")
            for definition in tool_definitions
            if isinstance(definition, dict)
        }
        required_tools = {
            "enter_edit_mode",
            "exit_edit_mode",
            "focus_element",
            "get_edit_mode",
            "get_element",
            "get_render_cell",
            "get_render_rect",
            "get_render_snapshot",
            "mouse_click",
        }
        missing_tools = sorted(required_tools - tool_names)
        schema_tool = "get_schema" if "get_schema" in tool_names else "ui.get_schema"
        value_tool = "get_value" if "get_value" in tool_names else "ui.get_value"
        for tool_name in (schema_tool, value_tool):
            if tool_name not in tool_names:
                missing_tools.append(tool_name)
        if missing_tools:
            raise AssertionError(f"{label} MCP tools/list is missing {missing_tools}")

        schema = session.tool(schema_tool)
        schema_elements = schema if isinstance(schema, list) else schema.get("elements", [])
        element_ids = {
            element.get("id")
            for element in schema_elements
            if isinstance(element, dict)
        }
        expected_ids = {"aligned_input", "choice", "scroller"}
        if not expected_ids.issubset(element_ids):
            raise AssertionError(
                f"{label} generated schema is missing {sorted(expected_ids - element_ids)}"
            )

        initial_lines = text_lines(session.tool("get_render_snapshot"), label)
        surface_rect = render_rect(session.tool("get_render_rect"))
        choice_col, choice_row = find_surface_text(
            initial_lines,
            "one",
            surface_rect,
            label,
        )

        session.tool("focus_element", {"element_id": "aligned_input"})
        aligned_lines = text_lines(session.tool("get_render_snapshot"), label)
        input_matches = surface_text_matches(aligned_lines, EXPECTED_TEXT, surface_rect)
        input_snapshot = session.tool("get_element", {"element_id": "aligned_input"})
        input_left, _, input_right, _ = element_bounds(input_snapshot)
        input_width = input_right - input_left
        aligned_matches = [
            (col, row)
            for col, row in input_matches
            if choice_col + input_width == col + len(EXPECTED_TEXT)
        ]
        if len(aligned_matches) != 1:
            raise AssertionError(
                f"{label} retained TextInput was not aligned to its right edge; "
                f"text={input_matches}, left={choice_col}, width={input_width}"
            )
        input_col, input_row = aligned_matches[0]
        rendered_text = "".join(
            session.tool(
                "get_render_cell",
                {"x": input_col + offset, "y": input_row},
            ).get("char", "")
            for offset in range(len(EXPECTED_TEXT))
        )
        if rendered_text != EXPECTED_TEXT:
            raise AssertionError(f"{label} retained TextInput rendered {rendered_text!r}")
        session.tool("enter_edit_mode", {"element_id": "aligned_input"})
        text_lines(session.tool("get_render_snapshot"), label)
        session.tool("exit_edit_mode")

        session.tool("enter_edit_mode", {"element_id": "choice"})
        text_lines(session.tool("get_render_snapshot"), label)
        session.tool("mouse_click", {"x": choice_col + 1, "y": choice_row + 7})
        combo_value = session.tool(value_tool, {"element_id": "choice"}).get("value")
        if combo_value != EXPECTED_COMBO_VALUE:
            raise AssertionError(
                f"{label} lower ComboBox option selected {combo_value!r}, "
                f"expected {EXPECTED_COMBO_VALUE!r}"
            )
        edit_mode = session.tool("get_edit_mode").get("edit_mode")
        if edit_mode:
            raise AssertionError(f"{label} ComboBox remained in edit mode after option click")

        session.tool("focus_element", {"element_id": "scroller"})
        text_lines(session.tool("get_render_snapshot"), label)
        scroller_snapshot = session.tool("get_element", {"element_id": "scroller"})
        if not isinstance(scroller_snapshot, dict) or not scroller_snapshot.get("focused"):
            raise AssertionError(
                f"{label} retained generated ScrollView did not accept focus: "
                f"{scroller_snapshot!r}"
            )
        session.close()
    except BaseException:
        session.abort()
        raise


def find_one(root: Path, name: str) -> Path:
    matches = sorted(path for path in root.rglob(name) if path.is_file())
    if len(matches) != 1:
        raise FileNotFoundError(f"expected one {name!r} under {root}, found {matches}")
    return matches[0]


def find_executable(root: Path, name: str) -> Path:
    matches = sorted(
        path
        for path in root.rglob(name)
        if path.is_file()
        and ".dSYM" not in path.parts
        and (is_windows() or os.access(path, os.X_OK))
    )
    if len(matches) != 1:
        raise FileNotFoundError(f"expected one executable {name!r} under {root}, found {matches}")
    return matches[0]


def python_gate(_: Path) -> None:
    fixture = FIXTURE_ROOT / "python"
    python_path = os.pathsep.join([str(fixture), str(ROOT / "src")])
    exercise_runtime(
        [sys.executable, fixture / "compatibility.py"],
        "Python",
        env={"PYTHONPATH": python_path},
    )


def cpp_gate(build_root: Path) -> None:
    source = FIXTURE_ROOT / "cpp"
    build = build_root / "cpp"
    cmake = required_command("cmake")
    configure: list[str | Path] = [cmake, "-S", source, "-B", build]
    if not is_windows():
        configure.append("-DCMAKE_BUILD_TYPE=Release")
    run_command(configure)
    build_command: list[str | Path] = [cmake, "--build", build, "--target", "compatibility"]
    if is_windows():
        build_command.extend(["--config", "Release"])
    run_command(build_command)
    binary = find_executable(build, executable_name("compatibility"))
    exercise_runtime([binary], "C++")


def csharp_gate(build_root: Path) -> None:
    project = FIXTURE_ROOT / "csharp/compatibility.csproj"
    artifacts = build_root / "csharp"
    dotnet = required_command("dotnet", fallback=Path.home() / ".dotnet" / executable_name("dotnet"))
    run_command(
        [
            dotnet,
            "build",
            project,
            "--configuration",
            "Release",
            "--artifacts-path",
            artifacts,
        ]
    )
    assembly = artifacts / "bin/compatibility/release/compatibility.dll"
    if not assembly.is_file():
        raise FileNotFoundError(f"C# compatibility assembly is missing: {assembly}")
    exercise_runtime([dotnet, assembly], "C#")


def go_gate(build_root: Path) -> None:
    source = FIXTURE_ROOT / "go"
    output = build_root / "go" / executable_name("compatibility")
    output.parent.mkdir(parents=True, exist_ok=True)
    go = required_command("go")
    go_cache = os.environ.get("GOCACHE") or str(build_root / "go-cache")
    environment = {"GOCACHE": go_cache}
    run_command([go, "build", "-o", output, "."], cwd=source, env=environment)
    exercise_runtime([output], "Go", env=environment)


def rust_gate(build_root: Path) -> None:
    source = FIXTURE_ROOT / "rust"
    fixture_copy = build_root / "rust-fixture"
    project = fixture_copy / "rust"
    shutil.copytree(source, project)
    shutil.copytree(FIXTURE_ROOT / "source", fixture_copy / "source")
    manifest = project / "Cargo.toml"
    relative_dependency = 'uimd = { path = "../../../../rust/src/uimd" }'
    manifest_text = manifest.read_text(encoding="utf-8")
    if manifest_text.count(relative_dependency) != 1:
        raise AssertionError("frozen Rust compatibility dependency path is unexpected")
    current_runtime = (ROOT / "rust/src/uimd").as_posix()
    manifest.write_text(
        manifest_text.replace(
            relative_dependency,
            f'uimd = {{ path = "{current_runtime}" }}',
        ),
        encoding="utf-8",
    )
    target = build_root / "rust-target"
    cargo = required_command("cargo", fallback=Path.home() / ".cargo/bin/cargo")
    environment = {"CARGO_TARGET_DIR": str(target)}
    run_command(
        [cargo, "build", "--manifest-path", manifest, "--release", "--offline"],
        env=environment,
    )
    exercise_runtime([target / "release" / executable_name("compatibility")], "Rust")


def swift_gate(build_root: Path) -> None:
    source = FIXTURE_ROOT / "swift"
    scratch = build_root / "swift"
    module_cache = build_root / "swift-module-cache"
    module_cache.mkdir(parents=True, exist_ok=True)
    swift = required_command("swift")
    run_command(
        [swift, "build", "--package-path", source, "--scratch-path", scratch, "-c", "release"],
        env={
            "CLANG_MODULE_CACHE_PATH": str(module_cache),
            "SWIFTPM_MODULECACHE_OVERRIDE": str(module_cache),
        },
    )
    binary = find_executable(scratch, executable_name("compatibility"))
    exercise_runtime([binary], "Swift", transport="tcp")


def gradle_wrapper_command() -> list[str | Path]:
    if is_windows():
        return [os.environ.get("COMSPEC", "cmd.exe"), "/c", ROOT / "java/gradlew.bat"]
    return [ROOT / "java/gradlew"]


def java_gate(build_root: Path) -> None:
    source = FIXTURE_ROOT / "java"
    output = build_root / "java"
    project_cache = build_root / "java-project-cache"
    environment = java_build_env()
    environment["UIMD_COMPAT_BUILD_ROOT"] = str(output)
    run_command(
        [
            *gradle_wrapper_command(),
            "-p",
            source,
            "installDist",
            "--project-cache-dir",
            project_cache,
            "--console=plain",
        ],
        env=environment,
    )
    launcher_name = "compatibility.bat" if is_windows() else "compatibility"
    launcher = find_one(output / "install", launcher_name)
    launch_command: list[str | Path]
    if is_windows():
        launch_command = [os.environ.get("COMSPEC", "cmd.exe"), "/c", launcher]
    else:
        launch_command = [launcher]
    exercise_runtime(launch_command, "Java", env=environment)


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--only",
        action="append",
        choices=("python", "cpp", "csharp", "go", "java", "rust", "swift"),
        help="run only the selected language gate; may be repeated",
    )
    parser.add_argument("--no-swift", action="store_true")
    parser.add_argument("--no-rust", action="store_true")
    parser.add_argument("--no-java", action="store_true")
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(list(sys.argv[1:] if argv is None else argv))
    verify_manifest()
    failures: list[tuple[str, BaseException]] = []
    with tempfile.TemporaryDirectory(prefix="uimd-previous-version-compat-") as temporary:
        build_root = Path(temporary)
        selected = set(args.only or ())
        gates: list[tuple[str, Callable[[Path], None], bool]] = [
            ("Python", python_gate, not selected or "python" in selected),
            ("C++", cpp_gate, not selected or "cpp" in selected),
            ("C#", csharp_gate, not selected or "csharp" in selected),
            ("Go", go_gate, not selected or "go" in selected),
            ("Java", java_gate, (not selected or "java" in selected) and not args.no_java),
            (
                "Rust",
                rust_gate,
                (not selected or "rust" in selected) and not args.no_rust and not is_windows(),
            ),
            (
                "Swift",
                swift_gate,
                (not selected or "swift" in selected) and not args.no_swift and not is_windows(),
            ),
        ]
        for label, gate, enabled in gates:
            if not enabled:
                print(f"SKIP {label} previous-version compatibility", flush=True)
                continue
            try:
                gate(build_root)
                print(f"PASS {label} previous-version compatibility", flush=True)
            except BaseException as exc:
                failures.append((label, exc))
                traceback.print_exception(type(exc), exc, exc.__traceback__)
                print(
                    f"FAIL {label} previous-version compatibility | "
                    f"{type(exc).__name__}: {exc}",
                    flush=True,
                )
        verify_manifest()

    passed = len(gates) - sum(not enabled for _, _, enabled in gates) - len(failures)
    attempted = len(gates) - sum(not enabled for _, _, enabled in gates)
    if failures:
        print(
            f"FAIL previous-version compatibility | {passed}/{attempted} checks passed | "
            f"{len(failures)} failed",
            flush=True,
        )
        return 1
    print(
        f"PASS previous-version compatibility | {passed}/{attempted} checks passed",
        flush=True,
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
