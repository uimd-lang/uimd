#!/usr/bin/env python3
"""Synchronize UIMD version surfaces."""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
VERSION_PATTERN = re.compile(r"^\d+\.\d+\.\d+$")


@dataclass(frozen=True)
class Replacement:
    path: str
    pattern: str
    replacement: str
    count: int = 0


def _replacement_plan(version: str) -> list[Replacement]:
    return [
        Replacement(
            "pyproject.toml",
            r'(?m)^version\s*=\s*"[^"]+"',
            f'version = "{version}"',
        ),
        Replacement(
            "src/uimd/__init__.py",
            r'(?m)^__version__\s*=\s*"[^"]+"',
            f'__version__ = "{version}"',
        ),
        Replacement(
            "cpp/CMakeLists.txt",
            r"project\(ui_cpp_runtime VERSION [^ )]+ LANGUAGES CXX\)",
            f"project(ui_cpp_runtime VERSION {version} LANGUAGES CXX)",
        ),
        Replacement(
            "CHANGELOG.md",
            r"(?m)^## \d+\.\d+\.\d+ - Unreleased",
            f"## {version} - Unreleased",
            count=1,
        ),
        Replacement(
            "docs/cpp-cmake.md",
            r"GIT_TAG v\d+\.\d+\.\d+",
            f"GIT_TAG v{version}",
        ),
    ]


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Update all tracked UIMD source, CMake, and documentation version surfaces.",
    )
    parser.add_argument("version", nargs="?", help="Version to set, for example 0.3.0")
    parser.add_argument("--check", action="store_true", help="Verify files already contain the requested version")
    args = parser.parse_args(argv)

    version = args.version or _current_pyproject_version()
    if not VERSION_PATTERN.match(version):
        print(f"error: version must use MAJOR.MINOR.PATCH form, got {version!r}", file=sys.stderr)
        return 2

    try:
        changes = _apply_replacements(version, check=args.check)
        _check_dynamic_surfaces()
    except RuntimeError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    if args.check:
        if changes:
            print("version surfaces are stale:")
            for path in changes:
                print(f"  - {path}")
            return 1
        print(f"version surfaces are consistent for {version}")
        return 0

    if changes:
        for path in changes:
            print(f"updated {path}")
    else:
        print(f"version surfaces already set to {version}")
    return 0


def _current_pyproject_version() -> str:
    text = _read("pyproject.toml")
    match = re.search(r'(?m)^version\s*=\s*"([^"]+)"', text)
    if not match:
        raise RuntimeError("pyproject.toml does not contain a project version")
    return match.group(1)


def _apply_replacements(version: str, *, check: bool) -> list[str]:
    changed: list[str] = []
    for replacement in _replacement_plan(version):
        path = ROOT / replacement.path
        text = path.read_text(encoding="utf-8")
        updated, count = re.subn(
            replacement.pattern,
            replacement.replacement,
            text,
            count=replacement.count,
        )
        if count == 0:
            raise RuntimeError(f"{replacement.path} did not match expected version pattern")
        if updated != text:
            changed.append(replacement.path)
            if not check:
                path.write_text(updated, encoding="utf-8")
    return changed


def _check_dynamic_surfaces() -> None:
    required_snippets = {
        "cpp/tools/uimd/NativeCppGenerator.cpp": 'GIT_TAG v" + std::string{UIMD_VERSION}',
        "cpp/tools/uimd/main.cpp": '"@VERSION@", runtimeVersion()',
        "cpp/CMakeLists.txt": 'VERSION ${PROJECT_VERSION}',
        "cpp/src/core/Version.cpp": "return UIMD_VERSION;",
        "cpp/tests/test_runtime_skeleton.cpp": "UIMD_EXPECTED_VERSION",
    }
    for path, snippet in required_snippets.items():
        if snippet not in _read(path):
            raise RuntimeError(f"{path} is no longer wired to the canonical version source")


def _read(path: str) -> str:
    return (ROOT / path).read_text(encoding="utf-8")


if __name__ == "__main__":
    raise SystemExit(main())
