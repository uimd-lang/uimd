"""Launcher for the repository MCP tester entrypoints.

The C++ tester is the default local implementation.  The Python tester remains
available as the packaged reference/fallback implementation via --backend
python.
"""

from __future__ import annotations

import os
import subprocess
import sys
from typing import Iterable


BACKEND_CPP = "cpp"
BACKEND_PYTHON = "python"
BACKEND_ENV = "UIMD_MCP_TESTER_BACKEND"
DEFAULT_BACKEND = BACKEND_CPP
CPP_TESTER_RELATIVE_PATH = os.path.join("cpp", "build", "tools", "mcp_tester", "uimd_mcp_tester")
CPP_BUILD_COMMAND = "cmake --build cpp/build --target uimd_mcp_tester"
CPP_TESTER_MISSING_EXIT_CODE = 127
PROJECT_MARKERS = (
    os.path.join("tests", "mcp", "all_examples.yaml"),
    os.path.join("cpp", "tools", "mcp_tester", "main.cpp"),
)
HELP_ARGS = ("-h", "--help")


def _candidate_roots() -> list[str]:
    here = os.path.realpath(os.path.dirname(__file__))
    return [
        os.getcwd(),
        here,
        os.path.dirname(os.path.dirname(os.path.dirname(here))),
    ]


def _is_project_root(path: str) -> bool:
    return all(os.path.exists(os.path.join(path, marker)) for marker in PROJECT_MARKERS)


def _project_root() -> str:
    for start in _candidate_roots():
        current = os.path.realpath(start)
        while True:
            if _is_project_root(current):
                return current
            parent = os.path.dirname(current)
            if parent == current:
                break
            current = parent
    return os.getcwd()


def _normalize_backend(value: str | None) -> str:
    backend = (value or DEFAULT_BACKEND).strip().lower()
    if backend in ("c++", "uimd_mcp_tester"):
        return BACKEND_CPP
    if backend in ("py", "python3", "reference"):
        return BACKEND_PYTHON
    if backend not in (BACKEND_CPP, BACKEND_PYTHON):
        raise ValueError(f"unsupported MCP tester backend: {value}")
    return backend


def _extract_backend(argv: Iterable[str]) -> tuple[str, list[str]]:
    backend = os.environ.get(BACKEND_ENV, DEFAULT_BACKEND)
    forwarded: list[str] = []
    args = list(argv)
    index = 0
    while index < len(args):
        arg = args[index]
        if arg == "--backend":
            index += 1
            if index >= len(args):
                raise ValueError("--backend requires cpp or python")
            backend = args[index]
        elif arg.startswith("--backend="):
            backend = arg.split("=", 1)[1]
        elif arg == "--cpp":
            backend = BACKEND_CPP
        elif arg == "--python":
            backend = BACKEND_PYTHON
        else:
            forwarded.append(arg)
        index += 1
    return _normalize_backend(backend), forwarded


def _cpp_tester_path(root: str | None = None) -> str:
    return os.path.join(root or _project_root(), CPP_TESTER_RELATIVE_PATH)


def _run_cpp_tester(args: list[str]) -> int:
    root = _project_root()
    binary = _cpp_tester_path(root)
    if not os.path.exists(binary):
        print(
            "C++ MCP tester binary not found: "
            f"{binary}\nBuild it with: {CPP_BUILD_COMMAND}",
            file=sys.stderr,
        )
        return CPP_TESTER_MISSING_EXIT_CODE
    return subprocess.call([binary, *args], cwd=root)


def _run_python_tester(args: list[str]) -> int:
    from uimd.testing import mcp_tester

    return int(mcp_tester.main(args))


def _print_backend_help() -> None:
    print(
        "\nBackend selection:\n"
        "  --backend cpp|python  Choose tester backend; default is cpp.\n"
        "  --cpp                 Alias for --backend cpp.\n"
        "  --python              Alias for --backend python.\n"
        f"  {BACKEND_ENV}=cpp|python  Environment fallback for backend selection.",
        flush=True,
    )


def main(argv: list[str] | None = None) -> int:
    raw_args = sys.argv[1:] if argv is None else argv
    try:
        backend, forwarded = _extract_backend(raw_args)
    except ValueError as exc:
        print(f"mcp_tester: {exc}", file=sys.stderr)
        return 2
    wants_help = any(arg in HELP_ARGS for arg in raw_args)
    if wants_help:
        _print_backend_help()
    if backend == BACKEND_PYTHON:
        return _run_python_tester(forwarded)
    return _run_cpp_tester(forwarded)


def main_argv(argv: list[str]) -> int:
    return main(argv)


if __name__ == "__main__":
    sys.exit(main())
