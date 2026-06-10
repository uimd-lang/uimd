#!/usr/bin/env python3
"""Smoke-check native `uimd` CLI behavior without the removed Python CLI."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
NATIVE_BINARY = ROOT / "cpp" / "build" / "tools" / "uimd" / "uimd"
NATIVE_INIT_BINARY = ROOT / "cpp" / "build" / "tools" / "uimd_init" / "uimd-init"
GENERATED_PATTERNS = ("*_ui.py", "*_ui.hpp", "*_ui.cpp")
GENERATED_ROOTS = (
    ROOT / "python" / "examples",
    ROOT / "python" / "dialogs",
    ROOT / "src" / "uimd" / "dialogs",
    ROOT / "src" / "uimd" / "testing",
    ROOT / "cpp" / "dialogs",
    ROOT / "cpp" / "examples",
    ROOT / "cpp" / "tools" / "mcp_tester",
)
EXECUTABLE_FILE_MODE = 0o755

HELLO_UIMD = """# Hello

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "Hello native smoke app."
```

## Members

```yaml
title:
  type: label
  text: "Hello"
  description: "Title."

name:
  type: textinput
  value: ""
  description: "Name."

greet:
  type: button
  title: "Greet"
  description: "Greet button."

output:
  type: label
  text: ""
  description: "Output."
```

## User Interface

```ui
+-----------------------------+
| title...................... |
+-----------------------------+
| name....................... |
| greet.......                |
+-----------------------------+
| output..................... |
+-----------------------------+
```
"""


def hello_uimd_with_sdk_version(version: str) -> str:
    return HELLO_UIMD.replace("status: draft\n", f"status: draft\nsdk-version: \"{version}\"\n", 1)


def file_sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def write_release_fixture(root: Path, version: str, *, corrupt_checksum: bool = False) -> None:
    release_dir = root / version
    (release_dir / "payload" / "targets" / "python").mkdir(parents=True)
    (release_dir / "payload" / "targets" / "cpp").mkdir(parents=True)

    binary_name = "uimd.exe" if os.name == "nt" else "uimd"
    binary = release_dir / "payload" / binary_name
    binary.write_text("#!/bin/sh\nprintf 'RELEASE:%s\\n' \"$*\"\n", encoding="utf-8")
    binary.chmod(EXECUTABLE_FILE_MODE)
    python_marker = release_dir / "payload" / "targets" / "python" / "runtime.txt"
    cpp_marker = release_dir / "payload" / "targets" / "cpp" / "runtime.txt"
    python_marker.write_text("python target\n", encoding="utf-8")
    cpp_marker.write_text("cpp target\n", encoding="utf-8")

    binary_checksum = file_sha256(binary)
    if corrupt_checksum:
        binary_checksum = "0" * len(binary_checksum)

    manifest = release_dir / "manifest.txt"
    manifest.write_text(
        "\n".join(
            [
                f"version {version}",
                f"file bin/{binary_name} {binary_checksum} payload/{binary_name}",
                f"file targets/python/runtime.txt {file_sha256(python_marker)} payload/targets/python/runtime.txt",
                f"file targets/cpp/runtime.txt {file_sha256(cpp_marker)} payload/targets/cpp/runtime.txt",
                "",
            ]
        ),
        encoding="utf-8",
    )


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--native-binary", default=str(NATIVE_BINARY), help="Path to the native uimd binary")
    parser.add_argument("--native-init-binary", default=str(NATIVE_INIT_BINARY), help="Path to the native uimd-init binary")
    parser.add_argument("--skip-backup", action="store_true", help="Do not back up generated files first")
    parser.add_argument(
        "--compile-examples",
        action="store_true",
        help="Regenerate all Python and C++ example UI sources after backing up generated outputs",
    )
    args = parser.parse_args(argv)

    native_binary = Path(args.native_binary).resolve()
    native_init_binary = Path(args.native_init_binary).resolve()
    if not native_binary.exists():
        print(f"error: native binary not found: {native_binary}", file=sys.stderr)
        print("hint: run `cmake --build cpp/build --target uimd` first", file=sys.stderr)
        return 2
    if not native_init_binary.exists():
        print(f"error: native init binary not found: {native_init_binary}", file=sys.stderr)
        print("hint: run `cmake --build cpp/build --target uimd_init` first", file=sys.stderr)
        return 2

    if not args.skip_backup:
        backup_dir = backup_generated_outputs()
        print(f"Backed up generated outputs to {backup_dir}")

    failures: list[str] = []
    with tempfile.TemporaryDirectory(prefix="uimd_native_smoke_") as tmp:
        workspace = Path(tmp)
        failures.extend(check_new(native_binary, workspace))
        failures.extend(check_generate(native_binary, workspace))
        failures.extend(check_installed_sdk_theme_lookup(native_binary, workspace))
        failures.extend(check_run(native_binary, workspace))
        failures.extend(check_mcp_test(native_binary))
        failures.extend(check_sdk(native_binary, workspace))
        failures.extend(check_inspect(native_binary, workspace))
        failures.extend(check_init(native_binary, native_init_binary, workspace))

    if args.compile_examples:
        failures.extend(compile_examples(native_binary))

    if failures:
        print("Native UIMD smoke checks failed:")
        for failure in failures:
            print(f"  - {failure}")
        return 1

    print("Native UIMD smoke checks passed.")
    return 0


def runtime_env() -> dict[str, str]:
    env = os.environ.copy()
    paths = [str(ROOT / "src"), str(ROOT / "python"), str(ROOT)]
    if env.get("PYTHONPATH"):
        paths.append(env["PYTHONPATH"])
    env["PYTHONPATH"] = os.pathsep.join(paths)
    env["UIMD_SOURCE_ROOT"] = str(ROOT)
    return env


def run_command(command: list[str], cwd: Path, env: dict[str, str] | None = None) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        cwd=cwd,
        env=env or runtime_env(),
        text=True,
        capture_output=True,
        check=False,
    )


def native_cli(native_binary: Path, *args: str) -> list[str]:
    return [str(native_binary), *args]


def configure_shell_test_env(env: dict[str, str], user_home: Path) -> Path:
    user_home.mkdir(parents=True, exist_ok=True)
    if os.name == "nt":
        env["USERPROFILE"] = str(user_home)
        return user_home / "Documents" / "PowerShell" / "Microsoft.PowerShell_profile.ps1"
    env["HOME"] = str(user_home)
    env["SHELL"] = "/bin/zsh"
    return user_home / ".zshrc"


def check_new(native_binary: Path, workspace: Path) -> list[str]:
    failures: list[str] = []
    cases = (
        ("python", ("hello.uimd", "hello.py"), ("hello_ui.py",)),
        ("cpp", ("hello.uimd", "hello.cpp", "CMakeLists.txt"), ("hello_ui.hpp", "hello_ui.cpp")),
    )
    for target, expected_files, absent_files in cases:
        target_dir = workspace / f"new_{target}"
        target_dir.mkdir()
        result = run_command(native_cli(native_binary, "new", "hello", "--target", target), target_dir)
        failures.extend(expect_success(f"new --target {target}", result))
        for relative_path in expected_files:
            failures.extend(expect_file(f"new --target {target}", target_dir / relative_path))
        for relative_path in absent_files:
            if (target_dir / relative_path).exists():
                failures.append(f"new --target {target}: unexpectedly generated {relative_path}")

    return failures


def check_generate(native_binary: Path, workspace: Path) -> list[str]:
    failures: list[str] = []
    cases = (
        ("python", (), ("hello_ui.py",)),
        ("cpp", ("--app-stub",), ("hello_ui.hpp", "hello_ui.cpp", "hello.cpp", "CMakeLists.txt")),
    )
    for target, extra_args, expected_files in cases:
        target_dir = workspace / f"generate_{target}"
        target_dir.mkdir()
        (target_dir / "hello.uimd").write_text(HELLO_UIMD, encoding="utf-8")
        result = run_command(native_cli(native_binary, "generate", "hello.uimd", "--target", target, *extra_args), target_dir)
        failures.extend(expect_success(f"generate --target {target}", result))
        for relative_path in expected_files:
            failures.extend(expect_file(f"generate --target {target}", target_dir / relative_path))

        if target == "python":
            content = (target_dir / "hello_ui.py").read_text(encoding="utf-8")
            if "class HelloUI" not in content or "_mcp_enabled = True" not in content:
                failures.append("generate --target python: hello_ui.py does not contain the expected UI class")
        else:
            header = (target_dir / "hello_ui.hpp").read_text(encoding="utf-8")
            source = (target_dir / "hello_ui.cpp").read_text(encoding="utf-8")
            app = (target_dir / "hello.cpp").read_text(encoding="utf-8")
            cmake = (target_dir / "CMakeLists.txt").read_text(encoding="utf-8")
            if "class HelloUI" not in header:
                failures.append("generate --target cpp: hello_ui.hpp does not contain the expected UI class")
            if "HelloUI::HelloUI()" not in source:
                failures.append("generate --target cpp: hello_ui.cpp does not contain the expected constructor")
            if "runGeneratedWindow" not in app:
                failures.append("generate --target cpp: hello.cpp does not contain the runtime launcher")
            if "GIT_TAG v" not in cmake:
                failures.append("generate --target cpp: CMakeLists.txt does not contain a runtime tag fallback")

    return failures


def check_installed_sdk_theme_lookup(native_binary: Path, workspace: Path) -> list[str]:
    failures: list[str] = []
    binary_name = "uimd.exe" if os.name == "nt" else "uimd"
    version_root = workspace / "installed_theme_sdk" / "sdk" / "9.9.9"
    installed_binary = version_root / "bin" / binary_name
    installed_binary.parent.mkdir(parents=True)
    shutil.copy2(native_binary, installed_binary)
    installed_binary.chmod(EXECUTABLE_FILE_MODE)

    package_root = version_root / "targets" / "python" / "uimd"
    package_root.mkdir(parents=True)
    shutil.copytree(ROOT / "src" / "uimd" / "themes", package_root / "themes")
    (package_root / "__init__.py").write_text('__version__ = "9.9.9"\n', encoding="utf-8")

    env = os.environ.copy()
    env["UIMD_SOURCE_ROOT"] = str(workspace / "missing_source_checkout")
    env.pop("UIMD_SDK_PYTHON_TARGET", None)

    cases = (
        ("python", "hello_ui.py"),
        ("cpp", "hello_ui.cpp"),
    )
    for target, generated_file in cases:
        target_dir = workspace / f"installed_theme_{target}"
        target_dir.mkdir()
        (target_dir / "hello.uimd").write_text(HELLO_UIMD, encoding="utf-8")
        result = run_command(native_cli(installed_binary, "generate", "hello.uimd", "--target", target), target_dir, env=env)
        failures.extend(expect_success(f"installed SDK theme lookup {target}", result))
        generated_path = target_dir / generated_file
        failures.extend(expect_file(f"installed SDK theme lookup {target}", generated_path))
        if generated_path.exists():
            generated = generated_path.read_text(encoding="utf-8")
            if "#0d1524" not in generated or "#334155" not in generated:
                failures.append(f"installed SDK theme lookup {target}: generated styles do not include dark theme colors")

    return failures


def check_run(native_binary: Path, workspace: Path) -> list[str]:
    run_dir = workspace / "run"
    run_dir.mkdir()
    (run_dir / "hello.uimd").write_text(HELLO_UIMD, encoding="utf-8")
    (run_dir / "hello.py").write_text('import sys\nprint("APP", "|".join(sys.argv[1:]))\n', encoding="utf-8")

    result = run_command(native_cli(native_binary, "run", "hello.uimd", "one", "two"), run_dir)
    failures = expect_success("run", result)
    if "APP one|two" not in result.stdout:
        failures.append(f"run: app stdout did not include forwarded arguments: {result.stdout.strip()!r}")
    failures.extend(expect_file("run", run_dir / "hello_ui.py"))
    return failures


def check_mcp_test(native_binary: Path) -> list[str]:
    failures: list[str] = []
    invalid = run_command(native_cli(native_binary, "mcp-test", "--backend", "invalid"), ROOT)
    if invalid.returncode != 2:
        failures.append(f"mcp-test invalid backend: expected return code 2, got {invalid.returncode}")
    if "unsupported MCP tester backend" not in invalid.stderr:
        failures.append("mcp-test invalid backend: expected unsupported backend message")

    help_result = run_command(native_cli(native_binary, "mcp-test", "--backend", "cpp", "--help"), ROOT)
    failures.extend(expect_success("mcp-test --backend cpp --help", help_result))
    if "Backend selection:" not in help_result.stdout:
        failures.append("mcp-test --backend cpp --help: missing native backend help")
    return failures


def check_sdk(native_binary: Path, workspace: Path) -> list[str]:
    failures: list[str] = []
    sdk_home = workspace / "uimd_home"
    env = runtime_env()
    env["UIMD_HOME"] = str(sdk_home)

    home = run_command(native_cli(native_binary, "sdk", "home"), workspace, env=env)
    failures.extend(expect_success("sdk home", home))
    if str(sdk_home) not in home.stdout:
        failures.append("sdk home: output does not include UIMD_HOME")

    sdk_help = run_command(native_cli(native_binary, "sdk", "--help"), workspace, env=env)
    failures.extend(expect_success("sdk --help", sdk_help))
    if "usage: uimd sdk <command> [args]" not in sdk_help.stdout or "list" not in sdk_help.stdout:
        failures.append("sdk --help: output does not describe SDK commands")

    empty_doctor = run_command(native_cli(native_binary, "doctor", "--json"), workspace, env=env)
    failures.extend(expect_success("doctor --json before sdk install", empty_doctor))
    try:
        empty_doctor_payload = json.loads(empty_doctor.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"doctor --json before sdk install: invalid JSON: {exc}")
    else:
        if empty_doctor_payload.get("sdk", {}).get("status") != "incomplete":
            failures.append("doctor --json before sdk install: expected incomplete SDK status")

    for version in ("0.3.0", "0.4.1"):
        install_result = run_command(native_cli(native_binary, "sdk", "install", version), workspace, env=env)
        failures.extend(expect_success(f"sdk install {version}", install_result))
        failures.extend(expect_file(f"sdk install {version}", sdk_home / "sdk" / version / "bin" / "uimd"))
        failures.extend(expect_file(f"sdk install {version}", sdk_home / "sdk" / version / "targets" / "python"))

    use_result = run_command(native_cli(native_binary, "sdk", "use", "0.3.0"), workspace, env=env)
    failures.extend(expect_success("sdk use", use_result))

    install_cpp_target = run_command(native_cli(native_binary, "sdk", "install-target", "cpp"), workspace, env=env)
    failures.extend(expect_success("sdk install-target cpp", install_cpp_target))
    failures.extend(expect_file("sdk install-target cpp", sdk_home / "sdk" / "0.3.0" / "targets" / "cpp"))

    unsupported_target = run_command(native_cli(native_binary, "sdk", "install-target", "unknown"), workspace, env=env)
    if unsupported_target.returncode == 0:
        failures.append("sdk install-target unknown: expected failure for unsupported target")
    if "supported SDK targets" not in unsupported_target.stderr:
        failures.append("sdk install-target unknown: expected supported-targets error message")

    list_result = run_command(native_cli(native_binary, "sdk", "list"), workspace, env=env)
    failures.extend(expect_success("sdk list", list_result))
    if "* 0.3.0" not in list_result.stdout or "0.4.1" not in list_result.stdout or "cpp" not in list_result.stdout:
        failures.append("sdk list: output does not include installed SDK versions")

    json_result = run_command(native_cli(native_binary, "sdk", "list", "--json"), workspace, env=env)
    failures.extend(expect_success("sdk list --json", json_result))
    try:
        payload = json.loads(json_result.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"sdk list --json: invalid JSON: {exc}")
    else:
        versions = set(payload.get("versions", []))
        if not {"0.3.0", "0.4.1"}.issubset(versions):
            failures.append("sdk list --json: payload does not include installed SDK versions")
        if payload.get("current") != "0.3.0":
            failures.append("sdk list --json: current version was not set by sdk use")
        targets = payload.get("targets", {})
        current_targets = set(targets.get("0.3.0", []))
        if not {"python", "cpp"}.issubset(current_targets):
            failures.append("sdk list --json: target map does not include python and cpp for current SDK")

    doctor_result = run_command(native_cli(native_binary, "doctor", "--json"), workspace, env=env)
    failures.extend(expect_success("doctor --json after sdk install", doctor_result))
    try:
        doctor_payload = json.loads(doctor_result.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"doctor --json after sdk install: invalid JSON: {exc}")
    else:
        sdk_payload = doctor_payload.get("sdk", {})
        if sdk_payload.get("status") != "ok":
            failures.append("doctor --json after sdk install: expected ok SDK status")
        if sdk_payload.get("current_version") != "0.3.0":
            failures.append("doctor --json after sdk install: expected current_version 0.3.0")
        if not {"python", "cpp"}.issubset(set(sdk_payload.get("current_targets", []))):
            failures.append("doctor --json after sdk install: expected current_targets to include python and cpp")

    if os.name != "nt":
        fake_binary = workspace / "fake-uimd"
        fake_binary.write_text("#!/bin/sh\nprintf 'DELEGATED:%s\\n' \"$*\"\n", encoding="utf-8")
        fake_binary.chmod(EXECUTABLE_FILE_MODE)
        fake_install = run_command(native_cli(native_binary, "sdk", "install", "0.5.0", "--from", str(fake_binary)), workspace, env=env)
        failures.extend(expect_success("sdk install --from", fake_install))
        fake_install_newer = run_command(native_cli(native_binary, "sdk", "install", "0.6.0", "--from", str(fake_binary)), workspace, env=env)
        failures.extend(expect_success("sdk install --from newer", fake_install_newer))
        fake_use = run_command(native_cli(native_binary, "sdk", "use", "0.5.0"), workspace, env=env)
        failures.extend(expect_success("sdk use fake", fake_use))
        delegated = run_command([str(sdk_home / "bin" / "uimd"), "delegated", "one", "two"], workspace, env=env)
        failures.extend(expect_success("launcher delegation", delegated))
        if "DELEGATED:delegated one two" not in delegated.stdout:
            failures.append("launcher delegation: expected output from versioned SDK binary")

        versioned_source = workspace / "sdk_versioned.uimd"
        versioned_source.write_text(hello_uimd_with_sdk_version("0.5.0"), encoding="utf-8")
        selected_by_metadata = run_command([str(sdk_home / "bin" / "uimd"), "generate", str(versioned_source)], workspace, env=env)
        failures.extend(expect_success("launcher sdk-version selection", selected_by_metadata))
        if "DELEGATED:generate" not in selected_by_metadata.stdout:
            failures.append("launcher sdk-version selection: expected delegation to versioned SDK binary")
        if "different minor series" not in selected_by_metadata.stderr:
            failures.append("launcher sdk-version selection: expected minor-series warning when selecting newer installed SDK")

        legacy_source = workspace / "legacy_no_sdk.uimd"
        legacy_source.write_text(HELLO_UIMD, encoding="utf-8")
        legacy_delegated = run_command([str(sdk_home / "bin" / "uimd"), "generate", str(legacy_source)], workspace, env=env)
        failures.extend(expect_success("launcher legacy sdk fallback", legacy_delegated))
        if "DELEGATED:generate" not in legacy_delegated.stdout:
            failures.append("launcher legacy sdk fallback: expected delegation to latest installed SDK binary")
        if "has no UIMD SDK version" not in legacy_delegated.stderr:
            failures.append("launcher legacy sdk fallback: expected missing sdk-version warning")

        override_binary = workspace / "override-uimd"
        override_binary.write_text("#!/bin/sh\nprintf 'OVERRIDE:%s\\n' \"$*\"\n", encoding="utf-8")
        override_binary.chmod(EXECUTABLE_FILE_MODE)
        override_env = env.copy()
        override_env["UIMD_SDK_PATH"] = str(override_binary)
        override_delegated = run_command([str(sdk_home / "bin" / "uimd"), "generate", str(legacy_source)], workspace, env=override_env)
        failures.extend(expect_success("launcher UIMD_SDK_PATH override", override_delegated))
        if "OVERRIDE:generate" not in override_delegated.stdout:
            failures.append("launcher UIMD_SDK_PATH override: expected delegation to override binary")
        if "has no UIMD SDK version" in override_delegated.stderr:
            failures.append("launcher UIMD_SDK_PATH override: should bypass sdk-version fallback warnings")
        override_sdk_list = run_command([str(sdk_home / "bin" / "uimd"), "sdk", "list"], workspace, env=override_env)
        failures.extend(expect_success("launcher UIMD_SDK_PATH local sdk command", override_sdk_list))
        if "OVERRIDE:" in override_sdk_list.stdout:
            failures.append("launcher UIMD_SDK_PATH local sdk command: sdk command was delegated")

        strict_missing = run_command([str(sdk_home / "bin" / "uimd"), "--require-sdk-version", "generate", str(legacy_source)], workspace, env=env)
        if strict_missing.returncode == 0:
            failures.append("launcher --require-sdk-version: expected failure for missing sdk-version")
        if "has no UIMD SDK version" not in strict_missing.stderr:
            failures.append("launcher --require-sdk-version: expected missing sdk-version error")

        missing_required = workspace / "missing_required_sdk.uimd"
        missing_required.write_text(hello_uimd_with_sdk_version("9.0.0"), encoding="utf-8")
        offline_missing = run_command([str(sdk_home / "bin" / "uimd"), "--offline", "generate", str(missing_required)], workspace, env=env)
        if offline_missing.returncode == 0:
            failures.append("launcher --offline missing SDK: expected failure for missing required SDK")
        if "offline mode is enabled" not in offline_missing.stderr:
            failures.append("launcher --offline missing SDK: expected offline failure message")

        restore_use = run_command(native_cli(native_binary, "sdk", "use", "0.3.0"), workspace, env=env)
        failures.extend(expect_success("sdk use restore", restore_use))

    release_root = workspace / "release_root"
    write_release_fixture(release_root, "0.7.0")
    release_install = run_command(native_cli(native_binary, "sdk", "install", "0.7.0", "--release-root", str(release_root)), workspace, env=env)
    failures.extend(expect_success("sdk install --release-root", release_install))
    binary_name = "uimd.exe" if os.name == "nt" else "uimd"
    failures.extend(expect_file("sdk install --release-root", sdk_home / "sdk" / "0.7.0" / "bin" / binary_name))
    failures.extend(expect_file("sdk install --release-root", sdk_home / "sdk" / "0.7.0" / "targets" / "python" / "runtime.txt"))
    failures.extend(expect_file("sdk install --release-root", sdk_home / "sdk" / "0.7.0" / "targets" / "cpp" / "runtime.txt"))

    bad_release_root = workspace / "bad_release_root"
    write_release_fixture(bad_release_root, "0.8.0", corrupt_checksum=True)
    bad_release_install = run_command(native_cli(native_binary, "sdk", "install", "0.8.0", "--release-root", str(bad_release_root)), workspace, env=env)
    if bad_release_install.returncode == 0:
        failures.append("sdk install --release-root checksum mismatch: expected failure")
    if "checksum mismatch" not in bad_release_install.stderr:
        failures.append("sdk install --release-root checksum mismatch: expected checksum failure message")

    update_home = workspace / "update_home"
    update_env = runtime_env()
    update_env["UIMD_HOME"] = str(update_home)
    update_install = run_command(native_cli(native_binary, "sdk", "install", "3.4.0"), workspace, env=update_env)
    failures.extend(expect_success("sdk update fixture install", update_install))
    update_release_root = workspace / "update_release_root"
    for version in ("3.4.1", "3.4.2", "3.5.0"):
        write_release_fixture(update_release_root, version)
    update_result = run_command(
        native_cli(native_binary, "sdk", "update", "--release-root", str(update_release_root), "--json"),
        workspace,
        env=update_env,
    )
    failures.extend(expect_success("sdk update --release-root --json", update_result))
    try:
        update_payload = json.loads(update_result.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"sdk update --release-root --json: invalid JSON: {exc}")
    else:
        if update_payload.get("current") != "3.4.0" or update_payload.get("selected") != "3.4.2":
            failures.append("sdk update --release-root --json: expected update from 3.4.0 to 3.4.2")
        if update_payload.get("updated") is not True or update_payload.get("installed") is not True:
            failures.append("sdk update --release-root --json: expected updated and installed true")
    failures.extend(expect_file("sdk update --release-root", update_home / "sdk" / "3.4.2" / "bin" / binary_name))
    failures.extend(expect_file("sdk update --release-root", update_home / "sdk" / "3.4.2" / "targets" / "cpp" / "runtime.txt"))

    update_install_newer = run_command(native_cli(native_binary, "sdk", "install", "3.4.3"), workspace, env=update_env)
    failures.extend(expect_success("sdk update installed-newer fixture", update_install_newer))
    update_installed_result = run_command(native_cli(native_binary, "sdk", "update", "--json"), workspace, env=update_env)
    failures.extend(expect_success("sdk update installed newer --json", update_installed_result))
    try:
        update_installed_payload = json.loads(update_installed_result.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"sdk update installed newer --json: invalid JSON: {exc}")
    else:
        if update_installed_payload.get("selected") != "3.4.3":
            failures.append("sdk update installed newer --json: expected selected 3.4.3")
        if update_installed_payload.get("installed") is not False:
            failures.append("sdk update installed newer --json: expected installed false for already-installed SDK")

    for version in ("1.2.0", "1.2.1", "1.2.2", "1.2.3"):
        prune_install = run_command(native_cli(native_binary, "sdk", "install", version), workspace, env=env)
        failures.extend(expect_success(f"sdk install {version} for prune", prune_install))
    prune_use = run_command(native_cli(native_binary, "sdk", "use", "1.2.0"), workspace, env=env)
    failures.extend(expect_success("sdk use old prune current", prune_use))
    prune_result = run_command(native_cli(native_binary, "sdk", "prune", "--json"), workspace, env=env)
    failures.extend(expect_success("sdk prune --json", prune_result))
    try:
        prune_payload = json.loads(prune_result.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"sdk prune --json: invalid JSON: {exc}")
    else:
        removed_versions = set(prune_payload.get("removed", []))
        if "1.2.1" not in removed_versions:
            failures.append("sdk prune --json: expected 1.2.1 to be pruned")
        if "1.2.0" in removed_versions:
            failures.append("sdk prune --json: current version 1.2.0 was pruned")
    for version in ("1.2.0", "1.2.2", "1.2.3"):
        failures.extend(expect_file(f"sdk prune kept {version}", sdk_home / "sdk" / version / "bin" / binary_name))
    if (sdk_home / "sdk" / "1.2.1").exists():
        failures.append("sdk prune: pruned version 1.2.1 still exists")
    prune_restore = run_command(native_cli(native_binary, "sdk", "use", "0.3.0"), workspace, env=env)
    failures.extend(expect_success("sdk use restore after prune", prune_restore))

    remove_result = run_command(native_cli(native_binary, "sdk", "remove", "0.4.1"), workspace, env=env)
    failures.extend(expect_success("sdk remove", remove_result))
    after_remove = run_command(native_cli(native_binary, "sdk", "list", "--json"), workspace, env=env)
    failures.extend(expect_success("sdk list --json after remove", after_remove))
    try:
        after_remove_payload = json.loads(after_remove.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"sdk list --json after remove: invalid JSON: {exc}")
    else:
        if "0.4.1" in set(after_remove_payload.get("versions", [])):
            failures.append("sdk remove: removed version still appears in sdk list")

    self_home = workspace / "self_uninstall_home"
    self_env = runtime_env()
    self_env["UIMD_HOME"] = str(self_home)
    self_install = run_command(native_cli(native_binary, "sdk", "install", "2.0.0"), workspace, env=self_env)
    failures.extend(expect_success("self uninstall fixture install", self_install))
    failures.extend(expect_file("self uninstall fixture launcher", self_home / "bin" / binary_name))
    self_uninstall = run_command([str(self_home / "bin" / binary_name), "self", "uninstall", "--json"], workspace, env=self_env)
    failures.extend(expect_success("self uninstall --json", self_uninstall))
    try:
        self_payload = json.loads(self_uninstall.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"self uninstall --json: invalid JSON: {exc}")
    else:
        if self_payload.get("removed") is not True or self_payload.get("status") != "ok":
            failures.append("self uninstall --json: expected removed true with ok status")
    if self_home.exists():
        failures.append("self uninstall --json: SDK home still exists after uninstall")
    return failures


def check_inspect(native_binary: Path, workspace: Path) -> list[str]:
    source_path = workspace / "inspect" / "hello.uimd"
    source_path.parent.mkdir()
    source_path.write_text(HELLO_UIMD, encoding="utf-8")

    result = run_command(native_cli(native_binary, "inspect", str(source_path), "--json"), ROOT)
    failures = expect_success("inspect --json", result)
    if failures:
        return failures

    try:
        payload = json.loads(result.stdout)
    except json.JSONDecodeError as exc:
        return [f"inspect --json: invalid JSON: {exc}"]

    expected = {
        "title": "Hello",
        "kind": "window",
        "extends": "uiwindow",
        "member_names": ["greet", "name", "output", "title"],
    }
    for key, value in expected.items():
        if payload.get(key) != value:
            failures.append(f"inspect --json: {key} was {payload.get(key)!r}, expected {value!r}")
    if set(payload.get("layout_member_names", [])) != {"title", "name", "greet", "output"}:
        failures.append("inspect --json: layout_member_names did not include the expected members")
    if payload.get("ui_text_length", 0) <= 0:
        failures.append("inspect --json: ui_text_length was not positive")
    if payload.get("metadata", {}).get("description") != "Hello native smoke app.":
        failures.append("inspect --json: metadata description was not parsed")
    return failures


def check_init(native_binary: Path, native_init_binary: Path, workspace: Path) -> list[str]:
    failures: list[str] = []
    sdk_home = workspace / "init_home"
    env = runtime_env()
    env["UIMD_HOME"] = str(sdk_home)
    env["UIMD_INIT_UIMD_BINARY"] = str(native_binary)
    no_shell_profile = configure_shell_test_env(env, workspace / "init_user_home")

    init_result = subprocess.run(
        [str(native_init_binary), "--no-shell-config", "--json"],
        cwd=workspace,
        env=env,
        text=True,
        capture_output=True,
        check=False,
    )
    failures.extend(expect_success("uimd-init", init_result))
    if failures:
        return failures

    check_result = subprocess.run(
        [str(native_init_binary), "--check", "--json"],
        cwd=workspace,
        env=env,
        text=True,
        capture_output=True,
        check=False,
    )
    failures.extend(expect_success("uimd-init --check", check_result))
    if failures:
        return failures
    if no_shell_profile.exists():
        failures.append("uimd-init --no-shell-config: unexpectedly created a shell profile")

    init_payload = json.loads(init_result.stdout)
    check_payload = json.loads(check_result.stdout)
    if init_payload["status"] != "ok" or check_payload["status"] != "ok":
        failures.append("uimd-init did not produce an ok SDK Store")
    failures.extend(expect_file("uimd-init", sdk_home / "bin" / "uimd"))
    if init_payload.get("current_version") != init_payload.get("version"):
        failures.append("uimd-init did not select its own SDK version")
    current_binary_value = init_payload.get("current_binary", "")
    if not current_binary_value:
        failures.append("uimd-init did not report current_binary")
    else:
        failures.extend(expect_file("uimd-init", Path(current_binary_value)))
    failures.extend(expect_file("uimd-init", sdk_home / "sdk" / init_payload["version"] / "targets" / "python"))

    doctor_result = run_command(native_cli(native_binary, "doctor", "--json"), workspace, env=env)
    failures.extend(expect_success("doctor --json after uimd-init", doctor_result))
    try:
        doctor_payload = json.loads(doctor_result.stdout)
    except json.JSONDecodeError as exc:
        failures.append(f"doctor --json after uimd-init: invalid JSON: {exc}")
    else:
        if doctor_payload.get("sdk", {}).get("status") != "ok":
            failures.append("doctor --json after uimd-init did not report an ok SDK Store")

    launcher_result = run_command([str(sdk_home / "bin" / "uimd"), "--version"], workspace, env=env)
    failures.extend(expect_success("uimd-init launcher --version", launcher_result))

    shell_sdk_home = workspace / "init_shell_home"
    shell_env = runtime_env()
    shell_env["UIMD_HOME"] = str(shell_sdk_home)
    shell_env["UIMD_INIT_UIMD_BINARY"] = str(native_binary)
    shell_profile = configure_shell_test_env(shell_env, workspace / "init_shell_user_home")

    modify_result = subprocess.run(
        [str(native_init_binary), "--modify-shell", "--json"],
        cwd=workspace,
        env=shell_env,
        text=True,
        capture_output=True,
        check=False,
    )
    failures.extend(expect_success("uimd-init --modify-shell", modify_result))
    if modify_result.returncode == 0:
        try:
            modify_payload = json.loads(modify_result.stdout)
        except json.JSONDecodeError as exc:
            failures.append(f"uimd-init --modify-shell: invalid JSON: {exc}")
        else:
            if modify_payload.get("shell_config") != "modify":
                failures.append("uimd-init --modify-shell: JSON did not report modify shell_config")
            if modify_payload.get("shell_changed") is not True:
                failures.append("uimd-init --modify-shell: JSON did not report shell_changed true")
            if Path(modify_payload.get("shell_profile", "")) != shell_profile:
                failures.append("uimd-init --modify-shell: JSON did not report the expected shell profile")

        failures.extend(expect_file("uimd-init --modify-shell", shell_profile))
        if shell_profile.exists():
            profile_content = shell_profile.read_text(encoding="utf-8")
            if "# UIMD SDK" not in profile_content or str(shell_sdk_home / "bin") not in profile_content:
                failures.append("uimd-init --modify-shell: shell profile does not add the SDK launcher directory")

            repeat_result = subprocess.run(
                [str(native_init_binary), "--modify-shell", "--json"],
                cwd=workspace,
                env=shell_env,
                text=True,
                capture_output=True,
                check=False,
            )
            failures.extend(expect_success("uimd-init --modify-shell repeat", repeat_result))
            repeat_content = shell_profile.read_text(encoding="utf-8")
            if repeat_content != profile_content:
                failures.append("uimd-init --modify-shell repeat: shell profile was not idempotent")
            if repeat_result.returncode == 0:
                try:
                    repeat_payload = json.loads(repeat_result.stdout)
                except json.JSONDecodeError as exc:
                    failures.append(f"uimd-init --modify-shell repeat: invalid JSON: {exc}")
                else:
                    if repeat_payload.get("shell_changed") is not False:
                        failures.append("uimd-init --modify-shell repeat: JSON did not report shell_changed false")
    return failures


def expect_success(label: str, result: subprocess.CompletedProcess[str]) -> list[str]:
    if result.returncode == 0:
        return []
    return [
        f"{label}: command failed with {result.returncode}; "
        f"stdout={result.stdout.strip()!r}; stderr={result.stderr.strip()!r}"
    ]


def expect_file(label: str, path: Path) -> list[str]:
    if path.exists():
        return []
    return [f"{label}: missing {path.name}"]


def backup_generated_outputs() -> Path:
    backup_root = Path(tempfile.mkdtemp(prefix="uimd_generated_backup_"))
    for generated_root in GENERATED_ROOTS:
        if not generated_root.exists():
            continue
        for pattern in GENERATED_PATTERNS:
            for path in generated_root.rglob(pattern):
                relative_path = path.relative_to(ROOT)
                destination = backup_root / relative_path
                destination.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(path, destination)
    return backup_root


def compile_examples(native_binary: Path) -> list[str]:
    commands = [
        native_cli(native_binary, "generate", "python/dialogs", "--target", "python"),
        native_cli(native_binary, "generate", "python/examples", "--target", "python"),
        native_cli(native_binary, "generate", "src/uimd/dialogs", "--target", "python"),
        native_cli(native_binary, "generate", "src/uimd/testing", "--target", "python"),
        native_cli(native_binary, "generate", "cpp/dialogs", "--target", "cpp"),
        native_cli(native_binary, "generate", "cpp/examples", "--target", "cpp"),
    ]
    failures: list[str] = []
    for command in commands:
        result = run_command(command, ROOT)
        if result.returncode != 0:
            failures.append(f"{' '.join(command)} failed with {result.returncode}")
    return failures


if __name__ == "__main__":
    raise SystemExit(main())
