#!/usr/bin/env python3
"""Cross-platform developer helpers for repo-local UIMD builds."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_POSIX_BUILD_DIR = Path("cpp/build")
DEFAULT_WINDOWS_BUILD_DIR = Path("cpp/build-windows")
DEFAULT_WINDOWS_CONFIG = "Release"
GENERATE_TARGETS = (
    ("python/dialogs", "python"),
    ("python/examples", "python"),
    ("src/uimd/dialogs", "python"),
    ("src/uimd/testing", "python"),
    ("cpp/dialogs", "cpp"),
    ("cpp/examples", "cpp"),
)


def is_windows() -> bool:
    return platform.system().lower() == "windows"


def default_build_dir() -> Path:
    return DEFAULT_WINDOWS_BUILD_DIR if is_windows() else DEFAULT_POSIX_BUILD_DIR


def cmake_command() -> str:
    if os.environ.get("CMAKE"):
        return os.environ["CMAKE"]
    found = shutil.which("cmake")
    if found:
        return found
    if is_windows():
        bundled = Path(
            "C:/Program Files/Microsoft Visual Studio/2022/Professional/"
            "Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
        )
        if bundled.exists():
            return str(bundled)
    return "cmake"


def run(command: list[str | Path], *, cwd: Path = ROOT) -> None:
    printable = " ".join(str(part) for part in command)
    print(f"==> {printable}", flush=True)
    subprocess.run([str(part) for part in command], cwd=cwd, check=True)


def cmake_configure_args(build_dir: Path) -> list[str | Path]:
    args: list[str | Path] = [cmake_command(), "-S", "cpp", "-B", build_dir]
    if is_windows():
        args.extend(["-G", "Visual Studio 17 2022", "-A", os.environ.get("UIMD_CMAKE_ARCH", "x64")])
    return args


def cmake_build_args(build_dir: Path, *, target: str | None = None, config: str | None = None) -> list[str | Path]:
    args: list[str | Path] = [cmake_command(), "--build", build_dir]
    if target:
        args.extend(["--target", target])
    if is_windows():
        args.extend(["--config", config or DEFAULT_WINDOWS_CONFIG])
    return args


def ctest_args(build_dir: Path, *, config: str | None = None) -> list[str | Path]:
    args: list[str | Path] = [ctest_command(), "--test-dir", build_dir, "--output-on-failure"]
    if is_windows():
        args.extend(["-C", config or DEFAULT_WINDOWS_CONFIG])
    return args


def ctest_command() -> str:
    cmake = Path(cmake_command())
    candidate = cmake.with_name("ctest.exe" if is_windows() else "ctest")
    if candidate.exists():
        return str(candidate)
    return "ctest"


def ensure_configured(build_dir: Path) -> None:
    if not (ROOT / build_dir / "CMakeCache.txt").exists():
        run(cmake_configure_args(build_dir))


def native_uimd_path(build_dir: Path, *, config: str | None = None) -> Path:
    names = ["uimd.exe"] if is_windows() else ["uimd", "uimd.exe"]
    config_names = [config or DEFAULT_WINDOWS_CONFIG, "Release", "Debug", "RelWithDebInfo", "MinSizeRel"]
    candidates: list[Path] = []
    for name in names:
        candidates.append(ROOT / build_dir / "tools/uimd" / name)
        for config_name in config_names:
            candidates.append(ROOT / build_dir / "tools/uimd" / config_name / name)
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError(
        f"repo-local uimd binary not found under {build_dir}; "
        f"run: {' '.join(str(part) for part in cmake_build_args(build_dir, target='uimd', config=config))}"
    )


def example_binary_path(name: str, build_dir: Path, *, config: str | None = None) -> Path:
    executable = f"{name}.exe" if is_windows() else name
    config_names = [config or DEFAULT_WINDOWS_CONFIG, "Release", "Debug", "RelWithDebInfo", "MinSizeRel"]
    candidates = [ROOT / build_dir / "examples" / name / executable]
    for config_name in config_names:
        candidates.append(ROOT / build_dir / "examples" / name / config_name / executable)
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError(
        f"C++ example binary not found for {name!r} under {build_dir}; "
        f"run: {' '.join(str(part) for part in cmake_build_args(build_dir, target=name, config=config))}"
    )


def ensure_native_uimd(build_dir: Path, *, config: str | None = None) -> Path:
    ensure_configured(build_dir)
    run(cmake_build_args(build_dir, target="uimd", config=config))
    return native_uimd_path(build_dir, config=config)


def generate_all(uimd: Path) -> None:
    for path, target in GENERATE_TARGETS:
        run([uimd, "generate", path, "--target", target])


def rebuild_all(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    generate_all(uimd)
    run(cmake_build_args(build_dir, config=args.config))
    run([sys.executable, "-m", "compileall", "python", "src", "tests", "tools"])
    if args.test:
        run(ctest_args(build_dir, config=args.config))


def run_cpp_example(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    run([uimd, "generate", "cpp/examples", "--target", "cpp"])
    run(cmake_build_args(build_dir, target=args.name, config=args.config))
    binary = example_binary_path(args.name, build_dir, config=args.config)
    run([binary, *args.app_args])


def mcp_cpp_example(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    run([uimd, "generate", "cpp/examples", "--target", "cpp"])
    run(cmake_build_args(build_dir, target=args.name, config=args.config))
    binary = example_binary_path(args.name, build_dir, config=args.config)
    command: list[str | Path] = [uimd, "mcp-test", "--headless", str(binary), args.yaml]
    if is_windows():
        command.extend(["--backend", "python"])
    if args.mcp_fast:
        command.append("--mcp-fast")
    if args.compare_app_size:
        command.extend(["--compare-app-size", args.compare_app_size])
    run(command)


def mcp_compare_example(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    run([uimd, "generate", "python/examples", "--target", "python"])
    run([uimd, "generate", "cpp/examples", "--target", "cpp"])
    run(cmake_build_args(build_dir, target=args.name, config=args.config))
    binary = example_binary_path(args.name, build_dir, config=args.config)
    python_app = ROOT / "python/examples" / args.name / f"{args.name}.py"
    command: list[str | Path] = [
        uimd,
        "mcp-test",
        "--headless",
        "--compare",
        python_app,
        binary,
        args.yaml,
    ]
    if is_windows():
        command.extend(["--backend", "python"])
    if args.mcp_fast:
        command.append("--mcp-fast")
    command.extend(["--compare-app-size", args.compare_app_size])
    run(command)


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", default=str(default_build_dir()))
    parser.add_argument("--config", default=DEFAULT_WINDOWS_CONFIG)
    subparsers = parser.add_subparsers(dest="command", required=True)

    rebuild = subparsers.add_parser("rebuild-all")
    rebuild.add_argument("--test", action="store_true")
    rebuild.set_defaults(func=rebuild_all)

    run_example = subparsers.add_parser("run-cpp-example")
    run_example.add_argument("name")
    run_example.add_argument("app_args", nargs=argparse.REMAINDER)
    run_example.set_defaults(func=run_cpp_example)

    mcp_example = subparsers.add_parser("mcp-cpp-example")
    mcp_example.add_argument("name")
    mcp_example.add_argument("yaml")
    mcp_example.add_argument("--compare-app-size", default=None)
    mcp_example.add_argument("--mcp-fast", action="store_true")
    mcp_example.set_defaults(func=mcp_cpp_example)

    compare = subparsers.add_parser("mcp-compare-example")
    compare.add_argument("name")
    compare.add_argument("yaml")
    compare.add_argument("--compare-app-size", default="90x35")
    compare.add_argument("--mcp-fast", action="store_true")
    compare.set_defaults(func=mcp_compare_example)

    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(sys.argv[1:] if argv is None else argv)
    try:
        args.func(args)
    except subprocess.CalledProcessError as exc:
        return exc.returncode
    except Exception as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
