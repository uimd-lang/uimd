#!/usr/bin/env python3
"""Cross-platform developer helpers for repo-local UIMD builds."""

from __future__ import annotations

import argparse
from dataclasses import dataclass
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys
import tempfile
import time


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_POSIX_BUILD_DIR = Path("cpp/build")
DEFAULT_WINDOWS_BUILD_DIR = Path("cpp/build-windows")
DEFAULT_WINDOWS_CONFIG = "Release"
DEFAULT_GO_CACHE_DIR_NAME = "uimd-go-cache"
GENERATE_TARGETS = (
    ("python/dialogs", "python"),
    ("python/examples", "python"),
    ("src/uimd/dialogs", "python"),
    ("src/uimd/testing", "python"),
    ("cpp/dialogs", "cpp"),
    ("cpp/examples", "cpp"),
    ("csharp/examples", "csharp"),
    ("go/examples", "go"),
)
SWIFT_GENERATE_TARGET = ("swift/examples", "swift")
RUST_GENERATE_TARGET = ("rust/examples", "rust")
DEFAULT_COMPARE_APP_SIZE = "90x35"
REGRESSION_PARITY_ROOT = Path("tests/regressions/uimd/parity")
REGRESSION_PARITY_PYTHON_ROOT = REGRESSION_PARITY_ROOT / "python"
REGRESSION_PARITY_CPP_SOURCE_ROOT = REGRESSION_PARITY_ROOT / "cpp"
REGRESSION_PARITY_MANIFEST = REGRESSION_PARITY_ROOT / "all.yaml"
GO_REGRESSION_PARITY_ROOT = Path("go/regressions/uimd/parity")
RUST_REGRESSION_PARITY_ROOT = Path("rust/regressions/uimd/parity")
REGRESSION_GENERATE_TARGETS = (
    (REGRESSION_PARITY_PYTHON_ROOT, "python"),
    (REGRESSION_PARITY_CPP_SOURCE_ROOT, "cpp"),
    (GO_REGRESSION_PARITY_ROOT, "go"),
)


@dataclass
class FullTestPhase:
    name: str
    status: str
    seconds: float
    detail: str = ""


@dataclass(frozen=True)
class PhaseSkip:
    detail: str


def is_windows() -> bool:
    return platform.system().lower() == "windows"


def default_build_dir() -> Path:
    return DEFAULT_WINDOWS_BUILD_DIR if is_windows() else DEFAULT_POSIX_BUILD_DIR


def regression_parity_cpp_build_root(build_dir: Path) -> Path:
    return build_dir / "regressions/uimd/parity"


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


def run_with_env(command: list[str | Path], *, cwd: Path = ROOT, env: dict[str, str]) -> None:
    printable = " ".join(str(part) for part in command)
    print(f"==> {printable}", flush=True)
    merged_env = os.environ.copy()
    merged_env.update(env)
    subprocess.run([str(part) for part in command], cwd=cwd, check=True, env=merged_env)


def go_build_env() -> dict[str, str]:
    cache = os.environ.get("GOCACHE") or str(Path(tempfile.gettempdir()) / DEFAULT_GO_CACHE_DIR_NAME)
    return {"GOCACHE": cache}


def failure_detail(exc: BaseException) -> str:
    if isinstance(exc, subprocess.CalledProcessError):
        return f"exit {exc.returncode}"
    return f"{type(exc).__name__}: {exc}"


def record_skipped_phase(phases: list[FullTestPhase], name: str, detail: str) -> None:
    phases.append(FullTestPhase(name, "SKIP", 0.0, detail))


def run_full_test_phase(phases: list[FullTestPhase], name: str, action):
    started = time.monotonic()
    try:
        result = action()
    except BaseException as exc:
        phases.append(FullTestPhase(name, "FAIL", time.monotonic() - started, failure_detail(exc)))
        raise
    elapsed = time.monotonic() - started
    if isinstance(result, PhaseSkip):
        phases.append(FullTestPhase(name, "SKIP", elapsed, result.detail))
        return None
    phases.append(FullTestPhase(name, "PASS", elapsed))
    return result


def print_full_test_summary(phases: list[FullTestPhase]) -> None:
    if not phases:
        return
    print("==> FULL TEST SUMMARY", flush=True)
    width = max(len(phase.name) for phase in phases)
    for phase in phases:
        detail = f" ({phase.detail})" if phase.detail else ""
        print(f"{phase.status:<4} {phase.name:<{width}} {phase.seconds:>6.1f}s{detail}", flush=True)
    result = "FAIL" if any(phase.status == "FAIL" for phase in phases) else "PASS"
    print(f"==> FULL TEST RESULT: {result}", flush=True)


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


def dotnet_command() -> str:
    if os.environ.get("DOTNET"):
        return os.environ["DOTNET"]
    found = shutil.which("dotnet")
    if found:
        return found
    home_candidate = Path.home() / ".dotnet" / ("dotnet.exe" if is_windows() else "dotnet")
    if home_candidate.exists():
        return str(home_candidate)
    return "dotnet"


def swift_command() -> str:
    if os.environ.get("SWIFT"):
        return os.environ["SWIFT"]
    found = shutil.which("swift")
    if found:
        return found
    return "swift"


def require_swift_command() -> str:
    command = swift_command()
    if Path(command).exists() or shutil.which(command) is not None:
        return command
    raise FileNotFoundError("swift command not found; install SwiftPM or pass --no-swift")


def cargo_command() -> str:
    if os.environ.get("CARGO"):
        return os.environ["CARGO"]
    found = shutil.which("cargo")
    if found:
        return found
    home_candidate = Path.home() / ".cargo" / "bin" / "cargo"
    if home_candidate.is_file():
        return str(home_candidate)
    return "cargo"


def require_cargo_command() -> str:
    command = cargo_command()
    if Path(command).exists() or shutil.which(command) is not None:
        return command
    raise FileNotFoundError("cargo command not found; install Rust or pass --no-rust")


def cargo_with_progress_command(*args: str) -> list[str | Path]:
    return [
        sys.executable,
        ROOT / "tools/cargo_with_progress.py",
        "--cargo",
        require_cargo_command(),
        *args,
    ]


def should_validate_swift(args: argparse.Namespace) -> bool:
    return not args.no_swift and not is_windows()


def should_validate_rust(args: argparse.Namespace) -> bool:
    return not args.no_rust and not is_windows()


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


def regression_cpp_binary_path(name: str, build_dir: Path, *, config: str | None = None) -> Path:
    executable = f"{name}.exe" if is_windows() else name
    root = ROOT / regression_parity_cpp_build_root(build_dir)
    config_names = [config or DEFAULT_WINDOWS_CONFIG, "Release", "Debug", "RelWithDebInfo", "MinSizeRel"]
    candidates = [root / name / executable]
    for config_name in config_names:
        candidates.append(root / name / config_name / executable)
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError(f"C++ regression binary not found for {name!r} under {root.relative_to(ROOT)}")


def go_regression_binary_path(name: str) -> Path:
    executable = f"{name}.exe" if is_windows() else name
    candidate = ROOT / GO_REGRESSION_PARITY_ROOT / name / executable
    if candidate.exists():
        return candidate
    raise FileNotFoundError(f"Go regression binary not found for {name!r} under {GO_REGRESSION_PARITY_ROOT}")


def rust_regression_binary_path(name: str) -> Path:
    candidate = ROOT / RUST_REGRESSION_PARITY_ROOT / name / "target" / "release" / name
    if candidate.exists():
        return candidate
    raise FileNotFoundError(
        f"Rust regression binary not found for {name!r} under {RUST_REGRESSION_PARITY_ROOT}"
    )


def csharp_example_project_path(name: str) -> Path:
    project = ROOT / "csharp/examples" / name / f"{name}.csproj"
    if project.exists():
        return project
    raise FileNotFoundError(f"C# example project not found for {name!r}: {project.relative_to(ROOT)}")


def csharp_example_dll_path(name: str, configuration: str = "Debug") -> Path:
    output_root = ROOT / "csharp/examples" / name / "bin" / configuration
    candidates = sorted(output_root.glob(f"net*/{name}.dll"))
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError(
        f"C# example output not found for {name!r} under {output_root.relative_to(ROOT)}; "
        f"run: {dotnet_command()} build {csharp_example_project_path(name).relative_to(ROOT)}"
    )


def csharp_example_projects() -> list[Path]:
    return sorted((ROOT / "csharp/examples").glob("*/*.csproj"))


def go_example_app_dirs() -> list[Path]:
    root = ROOT / "go/examples"
    if not root.exists():
        return []
    return sorted(
        directory
        for directory in root.iterdir()
        if directory.is_dir() and (directory / f"{directory.name}.go").exists()
    )


def go_regression_app_dirs() -> list[Path]:
    root = ROOT / GO_REGRESSION_PARITY_ROOT
    if not root.exists():
        return []
    return sorted(
        directory
        for directory in root.iterdir()
        if directory.is_dir() and (directory / f"{directory.name}.go").exists()
    )


def rust_example_app_dirs() -> list[Path]:
    root = ROOT / "rust/examples"
    if not root.exists():
        return []
    return sorted(
        directory
        for directory in root.iterdir()
        if directory.is_dir() and (directory / "Cargo.toml").exists()
    )


def rust_regression_app_dirs() -> list[Path]:
    root = ROOT / RUST_REGRESSION_PARITY_ROOT
    if not root.exists():
        return []
    return sorted(
        directory
        for directory in root.iterdir()
        if directory.is_dir() and (directory / "Cargo.toml").exists()
    )


def swift_example_packages() -> list[Path]:
    return sorted((ROOT / "swift/examples").glob("*/Package.swift"))


def ensure_native_uimd(build_dir: Path, *, config: str | None = None) -> Path:
    ensure_configured(build_dir)
    run(cmake_build_args(build_dir, target="uimd", config=config))
    return native_uimd_path(build_dir, config=config)


def generate_all(uimd: Path, *, include_swift: bool, include_rust: bool) -> None:
    for path, target in GENERATE_TARGETS:
        run([uimd, "generate", path, "--target", target])
    if include_swift:
        path, target = SWIFT_GENERATE_TARGET
        run([uimd, "generate", path, "--target", target])
    if include_rust:
        path, target = RUST_GENERATE_TARGET
        run([uimd, "generate", path, "--target", target])
    generate_regression_parity_if_available(uimd, include_rust=include_rust)


def build_csharp_example(name: str, configuration: str = "Debug") -> Path:
    project = csharp_example_project_path(name)
    command: list[str | Path] = [dotnet_command(), "build", project]
    if configuration:
        command.extend(["--configuration", configuration])
    run(command)
    return csharp_example_dll_path(name, configuration)


def build_all_csharp_examples(configuration: str = "Debug") -> None:
    projects = csharp_example_projects()
    if not projects:
        raise FileNotFoundError("no C# example projects found under csharp/examples")
    for project in projects:
        command: list[str | Path] = [dotnet_command(), "build", project]
        if configuration:
            command.extend(["--configuration", configuration])
        run(command)


def build_all_go_examples() -> None:
    app_dirs = go_example_app_dirs()
    if not app_dirs:
        raise FileNotFoundError("no Go example apps found under go/examples")
    go = shutil.which("go")
    if go is None:
        raise FileNotFoundError("go executable was not found on PATH")
    for app_dir in app_dirs:
        run_with_env([go, "build", "-o", app_dir.name, "."], cwd=app_dir, env=go_build_env())
    for app_dir in go_regression_app_dirs():
        run_with_env([go, "build", "-o", app_dir.name, "."], cwd=app_dir, env=go_build_env())


def run_go_tests() -> None:
    go = shutil.which("go")
    if go is None:
        raise FileNotFoundError("go executable was not found on PATH")
    run_with_env([go, "test", "./..."], cwd=ROOT / "go/src/uimd", env=go_build_env())


def build_all_rust_examples() -> None:
    app_dirs = rust_example_app_dirs()
    if not app_dirs:
        raise FileNotFoundError("no Rust example apps found under rust/examples")
    for app_dir in [*app_dirs, *rust_regression_app_dirs()]:
        run(cargo_with_progress_command("build", "--release"), cwd=app_dir)


def run_rust_tests() -> None:
    run(cargo_with_progress_command("test"), cwd=ROOT / "rust/src/uimd")


def run_rust_clippy() -> None:
    run(
        cargo_with_progress_command(
            "clippy",
            "--all-targets",
            "--",
            "-D",
            "warnings",
        ),
        cwd=ROOT / "rust/src/uimd",
    )


def build_all_swift_examples() -> None:
    packages = swift_example_packages()
    if not packages:
        raise FileNotFoundError("no Swift example packages found under swift/examples")
    command = require_swift_command()
    for package in packages:
        run([command, "build", "--package-path", package.parent])


def run_swift_tests() -> None:
    run([require_swift_command(), "test", "--package-path", "swift/src/Uimd"])


def run_swift_direct_terminal_smoke(build_dir: Path) -> None:
    run([sys.executable, "tools/swift_direct_terminal_smoke.py", "--cpp-build-dir", build_dir])


def run_go_direct_terminal_smoke(build_dir: Path) -> None:
    run(
        [
            sys.executable,
            "tools/go_direct_terminal_smoke.py",
            "--cpp-build-dir",
            build_dir,
            "--go-examples-dir",
            "go/examples",
        ]
    )


def run_rust_direct_terminal_smoke(build_dir: Path) -> None:
    run(
        [
            sys.executable,
            "tools/rust_direct_terminal_smoke.py",
            "--cpp-build-dir",
            build_dir,
            "--rust-examples-dir",
            "rust/examples",
        ]
    )


def run_rust_mcp_transport_smoke() -> None:
    run([sys.executable, "tools/rust_mcp_transport_smoke.py"])


def generate_regression_parity_if_available(uimd: Path, *, include_rust: bool) -> None:
    regression_root = ROOT / REGRESSION_PARITY_ROOT
    if not regression_root.exists():
        print(f"==> skip regression parity generation: {REGRESSION_PARITY_ROOT} does not exist", flush=True)
        return
    for path, _target in REGRESSION_GENERATE_TARGETS:
        if not (ROOT / path).exists():
            raise FileNotFoundError(f"regression parity generation root is missing: {path}")
    for path, target in REGRESSION_GENERATE_TARGETS:
        run([uimd, "generate", path, "--target", target])
    if include_rust:
        if not (ROOT / RUST_REGRESSION_PARITY_ROOT).exists():
            raise FileNotFoundError(
                f"regression parity generation root is missing: {RUST_REGRESSION_PARITY_ROOT}"
            )
        run([uimd, "generate", RUST_REGRESSION_PARITY_ROOT, "--target", "rust"])


def regression_manifest_scripts() -> list[Path]:
    manifest = ROOT / REGRESSION_PARITY_MANIFEST
    scripts: list[Path] = []
    for raw_line in manifest.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if not line.startswith("- include:"):
            continue
        script = line.split(":", 1)[1].strip()
        if script:
            scripts.append(REGRESSION_PARITY_ROOT / script)
    if not scripts:
        raise ValueError(f"regression parity manifest has no include entries: {REGRESSION_PARITY_MANIFEST}")
    return scripts


def rebuild_all(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    validate_swift = should_validate_swift(args)
    validate_rust = should_validate_rust(args)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    generate_all(uimd, include_swift=validate_swift, include_rust=validate_rust)
    run(cmake_configure_args(build_dir))
    run(cmake_build_args(build_dir, config=args.config))
    build_all_csharp_examples(args.csharp_config)
    build_all_go_examples()
    if validate_rust:
        build_all_rust_examples()
    elif args.no_rust:
        print("==> skip Rust examples: --no-rust", flush=True)
    else:
        print("==> skip Rust examples: Rust validation is not enabled on Windows", flush=True)
    if validate_swift:
        build_all_swift_examples()
    elif args.no_swift:
        print("==> skip Swift examples: --no-swift", flush=True)
    else:
        print("==> skip Swift examples: Swift validation is not enabled on Windows", flush=True)
    run([sys.executable, "-m", "compileall", "python", "src", "tests", "tools"])
    if args.test:
        run(ctest_args(build_dir, config=args.config))


def run_python_tests() -> None:
    probe = subprocess.run(
        [sys.executable, "-m", "pytest", "--version"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        check=False,
    )
    if probe.returncode == 0:
        run([sys.executable, "-m", "pytest", "python/tests"])
        return
    pytest = shutil.which("pytest")
    if pytest is not None:
        run([pytest, "python/tests"])
        return
    run([sys.executable, "-m", "pytest", "python/tests"])


def run_example_compare(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
) -> None:
    command: list[str | Path] = [
        uimd,
        "mcp-test",
        "--headless",
        "--all",
        "--compare",
        "python/examples",
        build_dir / "examples",
    ]
    if is_windows():
        command.extend(["--backend", "python"])
    if mcp_fast:
        command.append("--mcp-fast")
    command.extend(["--compare-app-size", compare_app_size])
    run(command)


def run_csharp_example_compare(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
) -> None:
    command: list[str | Path] = [
        uimd,
        "mcp-test",
        "--backend",
        "python",
        "--headless",
        "--all",
        "--compare",
        build_dir / "examples",
        "csharp/examples",
    ]
    if mcp_fast:
        command.append("--mcp-fast")
    command.extend(["--compare-app-size", compare_app_size])
    run(command)


def run_swift_example_compare(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
) -> None:
    command: list[str | Path] = [
        uimd,
        "mcp-test",
        "--backend",
        "python",
        "--headless",
        "--all",
        "--compare",
        build_dir / "examples",
        "swift/examples",
    ]
    if mcp_fast:
        command.append("--mcp-fast")
    command.extend(["--compare-app-size", compare_app_size])
    run(command)


def run_go_example_compare(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
) -> None:
    command: list[str | Path] = [
        uimd,
        "mcp-test",
        "--backend",
        "python",
        "--headless",
        "--all",
        "--compare",
        build_dir / "examples",
        "go/examples",
    ]
    if mcp_fast:
        command.append("--mcp-fast")
    command.extend(["--compare-app-size", compare_app_size])
    run(command)


def run_rust_example_compare(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
) -> None:
    command: list[str | Path] = [
        uimd,
        "mcp-test",
        "--backend",
        "python",
        "--headless",
        "--all",
        "--compare",
        build_dir / "examples",
        "rust/examples",
    ]
    if mcp_fast:
        command.append("--mcp-fast")
    command.extend(["--compare-app-size", compare_app_size])
    run(command)


def run_regression_compare_if_available(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
) -> PhaseSkip | None:
    python_root = ROOT / REGRESSION_PARITY_PYTHON_ROOT
    cpp_root_path = regression_parity_cpp_build_root(build_dir)
    cpp_root = ROOT / cpp_root_path
    manifest = ROOT / REGRESSION_PARITY_MANIFEST
    if not python_root.exists() and not cpp_root.exists() and not manifest.exists():
        print(f"==> skip regression parity compare: {REGRESSION_PARITY_ROOT} does not exist", flush=True)
        return PhaseSkip(f"{REGRESSION_PARITY_ROOT} does not exist")
    if not python_root.exists() or not cpp_root.exists() or not manifest.exists():
        if not python_root.exists():
            missing = REGRESSION_PARITY_PYTHON_ROOT
        elif not cpp_root.exists():
            missing = cpp_root_path
        else:
            missing = REGRESSION_PARITY_MANIFEST
        raise FileNotFoundError(f"regression parity compare root is missing: {missing}")

    command: list[str | Path] = [
        uimd,
        "mcp-test",
        "--headless",
        "--compare",
        REGRESSION_PARITY_PYTHON_ROOT,
        cpp_root_path,
        REGRESSION_PARITY_MANIFEST,
    ]
    if is_windows():
        command.extend(["--backend", "python"])
    if mcp_fast:
        command.append("--mcp-fast")
    command.extend(["--compare-app-size", compare_app_size])
    run(command)
    return None


def run_go_regression_compare_if_available(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
    config: str | None = None,
) -> PhaseSkip | None:
    cpp_root_path = regression_parity_cpp_build_root(build_dir)
    cpp_root = ROOT / cpp_root_path
    go_root = ROOT / GO_REGRESSION_PARITY_ROOT
    manifest = ROOT / REGRESSION_PARITY_MANIFEST
    if not cpp_root.exists() and not go_root.exists() and not manifest.exists():
        print(f"==> skip Go regression parity compare: {GO_REGRESSION_PARITY_ROOT} does not exist", flush=True)
        return PhaseSkip(f"{GO_REGRESSION_PARITY_ROOT} does not exist")
    if not cpp_root.exists() or not go_root.exists() or not manifest.exists():
        if not cpp_root.exists():
            missing = cpp_root_path
        elif not go_root.exists():
            missing = GO_REGRESSION_PARITY_ROOT
        else:
            missing = REGRESSION_PARITY_MANIFEST
        raise FileNotFoundError(f"Go regression parity compare root is missing: {missing}")

    for script in regression_manifest_scripts():
        app_name = script.stem
        command: list[str | Path] = [
            uimd,
            "mcp-test",
            "--backend",
            "python",
            "--headless",
            "--compare",
            regression_cpp_binary_path(app_name, build_dir, config=config),
            go_regression_binary_path(app_name),
            script,
        ]
        if mcp_fast:
            command.append("--mcp-fast")
        command.extend(["--compare-app-size", compare_app_size])
        run(command)
    return None


def run_rust_regression_compare_if_available(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
    config: str | None = None,
) -> PhaseSkip | None:
    cpp_root_path = regression_parity_cpp_build_root(build_dir)
    cpp_root = ROOT / cpp_root_path
    rust_root = ROOT / RUST_REGRESSION_PARITY_ROOT
    manifest = ROOT / REGRESSION_PARITY_MANIFEST
    if not cpp_root.exists() and not rust_root.exists() and not manifest.exists():
        print(f"==> skip Rust regression parity compare: {RUST_REGRESSION_PARITY_ROOT} does not exist", flush=True)
        return PhaseSkip(f"{RUST_REGRESSION_PARITY_ROOT} does not exist")
    if not cpp_root.exists() or not rust_root.exists() or not manifest.exists():
        if not cpp_root.exists():
            missing = cpp_root_path
        elif not rust_root.exists():
            missing = RUST_REGRESSION_PARITY_ROOT
        else:
            missing = REGRESSION_PARITY_MANIFEST
        raise FileNotFoundError(f"Rust regression parity compare root is missing: {missing}")

    for script in regression_manifest_scripts():
        app_name = script.stem
        command: list[str | Path] = [
            uimd,
            "mcp-test",
            "--backend",
            "python",
            "--headless",
            "--compare",
            regression_cpp_binary_path(app_name, build_dir, config=config),
            rust_regression_binary_path(app_name),
            script,
        ]
        if mcp_fast:
            command.append("--mcp-fast")
        command.extend(["--compare-app-size", compare_app_size])
        run(command)
    return None


def test_all(args: argparse.Namespace) -> None:
    phases: list[FullTestPhase] = []
    build_dir = Path(args.build_dir)
    validate_swift = should_validate_swift(args)
    validate_rust = should_validate_rust(args)
    try:
        uimd = run_full_test_phase(
            phases,
            "Build repo-local uimd tool",
            lambda: ensure_native_uimd(build_dir, config=args.config),
        )
        if not args.no_rebuild:
            run_full_test_phase(
                phases,
                "Generate UIMD sources",
                lambda: generate_all(
                    uimd,
                    include_swift=validate_swift,
                    include_rust=validate_rust,
                ),
            )
            run_full_test_phase(phases, "Configure CMake", lambda: run(cmake_configure_args(build_dir)))
            run_full_test_phase(
                phases,
                "Build C++ runtime, tools, examples, regressions",
                lambda: run(cmake_build_args(build_dir, config=args.config)),
            )
            run_full_test_phase(
                phases,
                "Build C# runtime and examples",
                lambda: build_all_csharp_examples(args.csharp_config),
            )
            run_full_test_phase(phases, "Build Go runtime, examples, regressions", build_all_go_examples)
            if validate_rust:
                run_full_test_phase(
                    phases,
                    "Build Rust runtime, examples, regressions",
                    build_all_rust_examples,
                )
            elif args.no_rust:
                record_skipped_phase(phases, "Build Rust runtime, examples, regressions", "--no-rust")
            else:
                record_skipped_phase(
                    phases,
                    "Build Rust runtime, examples, regressions",
                    "Rust validation is not enabled on Windows",
                )
            if validate_swift:
                run_full_test_phase(phases, "Build Swift runtime and examples", build_all_swift_examples)
            elif args.no_swift:
                record_skipped_phase(phases, "Build Swift runtime and examples", "--no-swift")
            else:
                record_skipped_phase(
                    phases,
                    "Build Swift runtime and examples",
                    "Swift validation is not enabled on Windows",
                )
            run_full_test_phase(
                phases,
                "Compile Python sources",
                lambda: run([sys.executable, "-m", "compileall", "python", "src", "tests", "tools"]),
            )
        else:
            record_skipped_phase(phases, "Generate UIMD sources", "--no-rebuild")
            record_skipped_phase(phases, "Configure CMake", "--no-rebuild")
            record_skipped_phase(phases, "Build C++ runtime, tools, examples, regressions", "--no-rebuild")
            record_skipped_phase(phases, "Build C# runtime and examples", "--no-rebuild")
            record_skipped_phase(phases, "Build Go runtime, examples, regressions", "--no-rebuild")
            record_skipped_phase(phases, "Build Rust runtime, examples, regressions", "--no-rebuild")
            record_skipped_phase(phases, "Build Swift runtime and examples", "--no-rebuild")
            record_skipped_phase(phases, "Compile Python sources", "--no-rebuild")
        run_full_test_phase(phases, "Python tests", run_python_tests)
        run_full_test_phase(phases, "CTest", lambda: run(ctest_args(build_dir, config=args.config)))
        run_full_test_phase(phases, "Go runtime tests", run_go_tests)
        if validate_rust:
            run_full_test_phase(phases, "Rust runtime tests", run_rust_tests)
            run_full_test_phase(phases, "Rust clippy", run_rust_clippy)
        elif args.no_rust:
            record_skipped_phase(phases, "Rust runtime tests", "--no-rust")
            record_skipped_phase(phases, "Rust clippy", "--no-rust")
        else:
            record_skipped_phase(phases, "Rust runtime tests", "Rust validation is not enabled on Windows")
            record_skipped_phase(phases, "Rust clippy", "Rust validation is not enabled on Windows")
        if validate_swift:
            run_full_test_phase(phases, "Swift runtime tests", run_swift_tests)
            run_full_test_phase(
                phases,
                "Swift direct terminal smoke",
                lambda: run_swift_direct_terminal_smoke(build_dir),
            )
        elif args.no_swift:
            record_skipped_phase(phases, "Swift runtime tests", "--no-swift")
            record_skipped_phase(phases, "Swift direct terminal smoke", "--no-swift")
        else:
            record_skipped_phase(phases, "Swift runtime tests", "Swift validation is not enabled on Windows")
            record_skipped_phase(
                phases,
                "Swift direct terminal smoke",
                "Swift validation is not enabled on Windows",
            )
        if is_windows():
            record_skipped_phase(phases, "Go direct terminal smoke", "POSIX PTY is required")
        else:
            run_full_test_phase(
                phases,
                "Go direct terminal smoke",
                lambda: run_go_direct_terminal_smoke(build_dir),
            )
        if validate_rust:
            run_full_test_phase(
                phases,
                "Rust direct terminal smoke",
                lambda: run_rust_direct_terminal_smoke(build_dir),
            )
            run_full_test_phase(phases, "Rust MCP transport smoke", run_rust_mcp_transport_smoke)
        elif args.no_rust:
            record_skipped_phase(phases, "Rust direct terminal smoke", "--no-rust")
            record_skipped_phase(phases, "Rust MCP transport smoke", "--no-rust")
        else:
            record_skipped_phase(phases, "Rust direct terminal smoke", "Rust validation is not enabled on Windows")
            record_skipped_phase(phases, "Rust MCP transport smoke", "Rust validation is not enabled on Windows")
        run_full_test_phase(
            phases,
            "MCP example compare",
            lambda: run_example_compare(
                uimd,
                build_dir,
                compare_app_size=args.compare_app_size,
                mcp_fast=not args.no_mcp_fast,
            ),
        )
        run_full_test_phase(
            phases,
            "MCP C# example compare",
            lambda: run_csharp_example_compare(
                uimd,
                build_dir,
                compare_app_size=args.compare_app_size,
                mcp_fast=not args.no_mcp_fast,
            ),
        )
        if validate_swift:
            run_full_test_phase(
                phases,
                "MCP Swift example compare",
                lambda: run_swift_example_compare(
                    uimd,
                    build_dir,
                    compare_app_size=args.compare_app_size,
                    mcp_fast=not args.no_mcp_fast,
                ),
            )
        elif args.no_swift:
            record_skipped_phase(phases, "MCP Swift example compare", "--no-swift")
        else:
            record_skipped_phase(phases, "MCP Swift example compare", "Swift validation is not enabled on Windows")
        run_full_test_phase(
            phases,
            "MCP Go example compare",
            lambda: run_go_example_compare(
                uimd,
                build_dir,
                compare_app_size=args.compare_app_size,
                mcp_fast=not args.no_mcp_fast,
            ),
        )
        if validate_rust:
            run_full_test_phase(
                phases,
                "MCP Rust example compare",
                lambda: run_rust_example_compare(
                    uimd,
                    build_dir,
                    compare_app_size=args.compare_app_size,
                    mcp_fast=not args.no_mcp_fast,
                ),
            )
        elif args.no_rust:
            record_skipped_phase(phases, "MCP Rust example compare", "--no-rust")
        else:
            record_skipped_phase(phases, "MCP Rust example compare", "Rust validation is not enabled on Windows")
        run_full_test_phase(
            phases,
            "MCP regression parity compare",
            lambda: run_regression_compare_if_available(
                uimd,
                build_dir,
                compare_app_size=args.compare_app_size,
                mcp_fast=not args.no_mcp_fast,
            ),
        )
        run_full_test_phase(
            phases,
            "MCP Go regression parity compare",
            lambda: run_go_regression_compare_if_available(
                uimd,
                build_dir,
                compare_app_size=args.compare_app_size,
                mcp_fast=not args.no_mcp_fast,
            ),
        )
        if validate_rust:
            run_full_test_phase(
                phases,
                "MCP Rust regression parity compare",
                lambda: run_rust_regression_compare_if_available(
                    uimd,
                    build_dir,
                    compare_app_size=args.compare_app_size,
                    mcp_fast=not args.no_mcp_fast,
                    config=args.config,
                ),
            )
        elif args.no_rust:
            record_skipped_phase(phases, "MCP Rust regression parity compare", "--no-rust")
        else:
            record_skipped_phase(
                phases,
                "MCP Rust regression parity compare",
                "Rust validation is not enabled on Windows",
            )
    finally:
        print_full_test_summary(phases)


def run_cpp_example(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    run([uimd, "generate", "cpp/examples", "--target", "cpp"])
    run(cmake_build_args(build_dir, target=args.name, config=args.config))
    binary = example_binary_path(args.name, build_dir, config=args.config)
    run([binary, *args.app_args])


def run_csharp_example(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    run([uimd, "generate", "csharp/examples", "--target", "csharp"])
    app = build_csharp_example(args.name, args.csharp_config)
    run([dotnet_command(), app, *args.app_args])


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


def mcp_csharp_example(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    run([uimd, "generate", "csharp/examples", "--target", "csharp"])
    app = build_csharp_example(args.name, args.csharp_config)
    command: list[str | Path] = [uimd, "mcp-test", "--backend", "python", "--headless", app, args.yaml]
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


def mcp_compare_csharp_example(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    uimd = ensure_native_uimd(build_dir, config=args.config)
    run([uimd, "generate", "csharp/examples", "--target", "csharp"])
    csharp_app = build_csharp_example(args.name, args.csharp_config)
    if args.against == "python":
        run([uimd, "generate", "python/examples", "--target", "python"])
        baseline: Path = ROOT / "python/examples" / args.name / f"{args.name}.py"
    else:
        run([uimd, "generate", "cpp/examples", "--target", "cpp"])
        run(cmake_build_args(build_dir, target=args.name, config=args.config))
        baseline = example_binary_path(args.name, build_dir, config=args.config)
    command: list[str | Path] = [
        uimd,
        "mcp-test",
        "--backend",
        "python",
        "--headless",
        "--compare",
        baseline,
        csharp_app,
        args.yaml,
    ]
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
    rebuild.add_argument("--csharp-config", default="Debug")
    rebuild.add_argument("--no-swift", action="store_true")
    rebuild.add_argument("--no-rust", action="store_true")
    rebuild.add_argument("--test", action="store_true")
    rebuild.set_defaults(func=rebuild_all)

    all_tests = subparsers.add_parser("test-all")
    all_tests.add_argument("--compare-app-size", default=DEFAULT_COMPARE_APP_SIZE)
    all_tests.add_argument("--no-mcp-fast", action="store_true")
    all_tests.add_argument("--no-rebuild", action="store_true")
    all_tests.add_argument("--csharp-config", default="Debug")
    all_tests.add_argument("--no-swift", action="store_true")
    all_tests.add_argument("--no-rust", action="store_true")
    all_tests.set_defaults(func=test_all)

    run_example = subparsers.add_parser("run-cpp-example")
    run_example.add_argument("name")
    run_example.add_argument("app_args", nargs=argparse.REMAINDER)
    run_example.set_defaults(func=run_cpp_example)

    run_csharp = subparsers.add_parser("run-csharp-example")
    run_csharp.add_argument("name")
    run_csharp.add_argument("--csharp-config", default="Debug")
    run_csharp.add_argument("app_args", nargs=argparse.REMAINDER)
    run_csharp.set_defaults(func=run_csharp_example)

    mcp_example = subparsers.add_parser("mcp-cpp-example")
    mcp_example.add_argument("name")
    mcp_example.add_argument("yaml")
    mcp_example.add_argument("--compare-app-size", default=None)
    mcp_example.add_argument("--mcp-fast", action="store_true")
    mcp_example.set_defaults(func=mcp_cpp_example)

    mcp_csharp = subparsers.add_parser("mcp-csharp-example")
    mcp_csharp.add_argument("name")
    mcp_csharp.add_argument("yaml")
    mcp_csharp.add_argument("--csharp-config", default="Debug")
    mcp_csharp.add_argument("--compare-app-size", default=None)
    mcp_csharp.add_argument("--mcp-fast", action="store_true")
    mcp_csharp.set_defaults(func=mcp_csharp_example)

    compare = subparsers.add_parser("mcp-compare-example")
    compare.add_argument("name")
    compare.add_argument("yaml")
    compare.add_argument("--compare-app-size", default=DEFAULT_COMPARE_APP_SIZE)
    compare.add_argument("--mcp-fast", action="store_true")
    compare.set_defaults(func=mcp_compare_example)

    compare_csharp = subparsers.add_parser("mcp-compare-csharp-example")
    compare_csharp.add_argument("name")
    compare_csharp.add_argument("yaml")
    compare_csharp.add_argument("--against", choices=("cpp", "python"), default="cpp")
    compare_csharp.add_argument("--csharp-config", default="Debug")
    compare_csharp.add_argument("--compare-app-size", default=DEFAULT_COMPARE_APP_SIZE)
    compare_csharp.add_argument("--mcp-fast", action="store_true")
    compare_csharp.set_defaults(func=mcp_compare_csharp_example)

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
