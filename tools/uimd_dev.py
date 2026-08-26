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

from full_test_report import FullTestReporter, default_log_path
from java_toolchain import java_build_env


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "src"))

from uimd.testing.artifact_manifest import remove_manifest, validate_manifest, write_manifest


DEFAULT_POSIX_BUILD_DIR = Path("cpp/build")
DEFAULT_WINDOWS_BUILD_DIR = Path("cpp/build-windows")
DEFAULT_WINDOWS_CONFIG = "Release"
PARITY_CONFIGURATION = "Release"
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
JAVA_EXAMPLES_SOURCE_ROOT = Path("python/examples")
JAVA_EXAMPLES_ROOT = Path("java/examples")
JAVA_DIALOGS_SOURCE_ROOT = Path("src/uimd/dialogs")
JAVA_DIALOGS_OUTPUT_ROOT = Path("java/src/main/java/uimd")
DEFAULT_COMPARE_APP_SIZE = "90x35"
REGRESSION_PARITY_ROOT = Path("tests/regressions/uimd/parity")
REGRESSION_PARITY_PYTHON_ROOT = REGRESSION_PARITY_ROOT / "python"
REGRESSION_PARITY_CPP_SOURCE_ROOT = REGRESSION_PARITY_ROOT / "cpp"
REGRESSION_PARITY_MANIFEST = REGRESSION_PARITY_ROOT / "all.yaml"
GO_REGRESSION_PARITY_ROOT = Path("go/regressions/uimd/parity")
RUST_REGRESSION_PARITY_ROOT = Path("rust/regressions/uimd/parity")
JAVA_REGRESSION_PARITY_ROOT = Path("java/regressions/uimd/parity")
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


_full_test_reporter: FullTestReporter | None = None


def emit_line(value: str, *, file=None) -> None:
    if _full_test_reporter is not None:
        _full_test_reporter.write_raw_line(value)
        return
    print(value, file=file, flush=True)


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
    if _full_test_reporter is not None:
        _full_test_reporter.run(command, cwd=cwd)
        return
    emit_line(f"==> {printable}")
    subprocess.run([str(part) for part in command], cwd=cwd, check=True)


def run_with_env(command: list[str | Path], *, cwd: Path = ROOT, env: dict[str, str]) -> None:
    printable = " ".join(str(part) for part in command)
    merged_env = os.environ.copy()
    merged_env.update(env)
    if _full_test_reporter is not None:
        _full_test_reporter.run(command, cwd=cwd, env=merged_env)
        return
    emit_line(f"==> {printable}")
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
    if _full_test_reporter is not None:
        _full_test_reporter.skip_phase(name, detail)


def run_full_test_phase(
    phases: list[FullTestPhase],
    name: str,
    action,
    *,
    report_kind: str = "generic",
    continue_on_failure: bool = False,
):
    if _full_test_reporter is not None:
        _full_test_reporter.start_phase(
            name,
            report_kind,
            keep_going_commands=continue_on_failure,
        )
    started = time.monotonic()
    try:
        result = action()
        if _full_test_reporter is not None and _full_test_reporter.phase_has_failed_commands():
            command_error = _full_test_reporter.first_failed_command()
            assert command_error is not None
            raise command_error
    except BaseException as exc:
        elapsed = time.monotonic() - started
        detail = failure_detail(exc)
        phases.append(FullTestPhase(name, "FAIL", elapsed, detail))
        if _full_test_reporter is not None:
            _full_test_reporter.finish_phase("FAIL", elapsed, detail)
        if continue_on_failure:
            return None
        raise
    elapsed = time.monotonic() - started
    if isinstance(result, PhaseSkip):
        phases.append(FullTestPhase(name, "SKIP", elapsed, result.detail))
        if _full_test_reporter is not None:
            _full_test_reporter.finish_phase("SKIP", elapsed, result.detail)
        return None
    phases.append(FullTestPhase(name, "PASS", elapsed))
    if _full_test_reporter is not None:
        _full_test_reporter.finish_phase("PASS", elapsed)
    return result


def print_full_test_summary(phases: list[FullTestPhase]) -> None:
    if not phases:
        return
    emit_line("==> FULL TEST SUMMARY")
    width = max(len(phase.name) for phase in phases)
    for phase in phases:
        detail = f" ({phase.detail})" if phase.detail else ""
        emit_line(f"{phase.status:<4} {phase.name:<{width}} {phase.seconds:>6.1f}s{detail}")
    result = "FAIL" if any(phase.status == "FAIL" for phase in phases) else "PASS"
    emit_line(f"==> FULL TEST RESULT: {result}")


def cmake_configure_args(
    build_dir: Path,
    *,
    configuration: str = PARITY_CONFIGURATION,
) -> list[str | Path]:
    args: list[str | Path] = [cmake_command(), "-S", "cpp", "-B", build_dir]
    if is_windows():
        args.extend(["-G", "Visual Studio 17 2022", "-A", os.environ.get("UIMD_CMAKE_ARCH", "x64")])
    else:
        args.append(f"-DCMAKE_BUILD_TYPE={configuration}")
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


def gradle_wrapper_command() -> list[str | Path]:
    if is_windows():
        wrapper = ROOT / "java/gradlew.bat"
        if not wrapper.is_file():
            raise FileNotFoundError("Java Gradle wrapper is missing: java/gradlew.bat")
        return [os.environ.get("COMSPEC", "cmd.exe"), "/c", wrapper]
    wrapper = ROOT / "java/gradlew"
    if not wrapper.is_file():
        raise FileNotFoundError("Java Gradle wrapper is missing: java/gradlew")
    return [wrapper]


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


def should_validate_java(args: argparse.Namespace) -> bool:
    return not args.no_java


def ensure_configured(build_dir: Path, *, configuration: str = PARITY_CONFIGURATION) -> None:
    if not (ROOT / build_dir / "CMakeCache.txt").exists():
        run(cmake_configure_args(build_dir, configuration=configuration))


def native_uimd_path(build_dir: Path, *, config: str | None = None) -> Path:
    names = ["uimd.exe"] if is_windows() else ["uimd", "uimd.exe"]
    configuration = config or PARITY_CONFIGURATION
    candidates: list[Path] = []
    for name in names:
        if not is_windows():
            candidates.append(ROOT / build_dir / "tools/uimd" / name)
        candidates.append(ROOT / build_dir / "tools/uimd" / configuration / name)
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError(
        f"repo-local uimd binary not found under {build_dir}; "
        f"run: {' '.join(str(part) for part in cmake_build_args(build_dir, target='uimd', config=config))}"
    )


def example_binary_path(name: str, build_dir: Path, *, config: str | None = None) -> Path:
    executable = f"{name}.exe" if is_windows() else name
    configuration = config or PARITY_CONFIGURATION
    candidates = []
    if not is_windows():
        candidates.append(ROOT / build_dir / "examples" / name / executable)
    candidates.append(ROOT / build_dir / "examples" / name / configuration / executable)
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
    configuration = config or PARITY_CONFIGURATION
    candidates = []
    if not is_windows():
        candidates.append(root / name / executable)
    candidates.append(root / name / configuration / executable)
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


def java_example_binary_path(name: str) -> Path:
    executable = f"{name}.bat" if is_windows() else name
    path = ROOT / JAVA_EXAMPLES_ROOT / name / "build/install" / name / "bin" / executable
    if not path.is_file():
        raise FileNotFoundError(
            f"Java parity artifact is missing: {path.relative_to(ROOT)}; "
            "run ./tools/rebuild_all.sh"
        )
    return path


def java_regression_binary_path(name: str) -> Path:
    executable = f"{name}.bat" if is_windows() else name
    path = ROOT / JAVA_REGRESSION_PARITY_ROOT / name / "build/install" / name / "bin" / executable
    if not path.is_file():
        raise FileNotFoundError(
            f"Java regression binary not found for {name!r} under "
            f"{JAVA_REGRESSION_PARITY_ROOT}"
        )
    return path


def csharp_example_project_path(name: str) -> Path:
    project = ROOT / "csharp/examples" / name / f"{name}.csproj"
    if project.exists():
        return project
    raise FileNotFoundError(f"C# example project not found for {name!r}: {project.relative_to(ROOT)}")


def csharp_example_dll_path(name: str, configuration: str = PARITY_CONFIGURATION) -> Path:
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


def java_example_app_dirs() -> list[Path]:
    root = ROOT / JAVA_EXAMPLES_ROOT
    if not root.exists():
        return []
    return sorted(
        directory
        for directory in root.iterdir()
        if directory.is_dir() and (directory / "build.gradle").is_file()
    )


def java_regression_app_dirs() -> list[Path]:
    root = ROOT / JAVA_REGRESSION_PARITY_ROOT
    if not root.exists():
        return []
    return sorted(
        directory
        for directory in root.iterdir()
        if directory.is_dir() and (directory / "build.gradle").is_file()
    )


def swift_example_packages() -> list[Path]:
    return sorted((ROOT / "swift/examples").glob("*/Package.swift"))


def ensure_native_uimd(build_dir: Path, *, config: str | None = None) -> Path:
    ensure_configured(build_dir, configuration=config or PARITY_CONFIGURATION)
    run(cmake_build_args(build_dir, target="uimd", config=config))
    return native_uimd_path(build_dir, config=config)


def generate_all(
    uimd: Path,
    *,
    include_swift: bool,
    include_rust: bool,
    include_java: bool,
) -> None:
    for path, target in GENERATE_TARGETS:
        run([uimd, "generate", path, "--target", target])
    if include_swift:
        path, target = SWIFT_GENERATE_TARGET
        run([uimd, "generate", path, "--target", target])
    if include_rust:
        path, target = RUST_GENERATE_TARGET
        run([uimd, "generate", path, "--target", target])
    if include_java:
        run(
            [
                uimd,
                "generate",
                JAVA_EXAMPLES_SOURCE_ROOT,
                "--target",
                "java",
                "--output-dir",
                JAVA_EXAMPLES_ROOT,
            ]
        )
        run(
            [
                uimd,
                "generate",
                JAVA_DIALOGS_SOURCE_ROOT,
                "--target",
                "java",
                "--output-dir",
                JAVA_DIALOGS_OUTPUT_ROOT,
                "--java-package",
                "uimd",
            ]
        )
    generate_regression_parity_if_available(
        uimd,
        include_rust=include_rust,
        include_java=include_java,
    )


def build_csharp_example(name: str, configuration: str = PARITY_CONFIGURATION) -> Path:
    project = csharp_example_project_path(name)
    command: list[str | Path] = [dotnet_command(), "build", project]
    if configuration:
        command.extend(["--configuration", configuration])
    run(command)
    return csharp_example_dll_path(name, configuration)


def build_all_csharp_examples(configuration: str = PARITY_CONFIGURATION) -> None:
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


def run_csharp_tests() -> None:
    run(
        [
            dotnet_command(),
            "run",
            "--project",
            "csharp/tests/UimdRuntimeTests/UimdRuntimeTests.csproj",
            "--configuration",
            PARITY_CONFIGURATION,
        ]
    )


def run_gradle(project_dir: Path, *tasks: str) -> None:
    run_with_env(
        [*gradle_wrapper_command(), "-p", project_dir, *tasks, "--console=plain"],
        env=java_build_env(),
    )


def build_all_java_examples() -> None:
    app_dirs = java_example_app_dirs()
    regression_dirs = java_regression_app_dirs()
    if not app_dirs:
        raise FileNotFoundError("no Java example apps found under java/examples")
    run_gradle(Path("java"), "assemble")
    for app_dir in [*app_dirs, *regression_dirs]:
        run_gradle(app_dir.relative_to(ROOT), "clean", "installDist")


def run_java_tests() -> None:
    run_gradle(Path("java"), "check")


def build_all_rust_examples() -> None:
    app_dirs = rust_example_app_dirs()
    if not app_dirs:
        raise FileNotFoundError("no Rust example apps found under rust/examples")
    run(
        cargo_with_progress_command("build", "--release", "--all-targets"),
        cwd=ROOT / "rust/src/uimd",
    )
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
        run([command, "build", "-c", "release", "--package-path", package.parent])


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


def run_java_direct_terminal_smoke(build_dir: Path) -> None:
    run(
        [
            sys.executable,
            "tools/java_direct_terminal_smoke.py",
            "--cpp-build-dir",
            build_dir,
            "--java-examples-dir",
            JAVA_EXAMPLES_ROOT,
        ]
    )


def run_java_mcp_transport_smoke() -> None:
    run([sys.executable, "tools/java_mcp_transport_smoke.py"])


def generate_regression_parity_if_available(
    uimd: Path,
    *,
    include_rust: bool,
    include_java: bool,
) -> None:
    regression_root = ROOT / REGRESSION_PARITY_ROOT
    if not regression_root.exists():
        emit_line(f"==> skip regression parity generation: {REGRESSION_PARITY_ROOT} does not exist")
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
    if include_java:
        if not (ROOT / JAVA_REGRESSION_PARITY_ROOT).exists():
            raise FileNotFoundError(
                f"regression parity generation root is missing: {JAVA_REGRESSION_PARITY_ROOT}"
            )
        run(
            [
                uimd,
                "generate",
                REGRESSION_PARITY_PYTHON_ROOT,
                "--target",
                "java",
                "--output-dir",
                JAVA_REGRESSION_PARITY_ROOT,
            ]
        )


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


def _repo_relative(path: Path) -> Path:
    resolved = path if path.is_absolute() else ROOT / path
    return resolved.resolve().relative_to(ROOT.resolve())


def _artifact(
    platform_name: str,
    kind: str,
    name: str,
    root: Path,
    path: Path,
) -> dict[str, str]:
    return {
        "platform": platform_name,
        "kind": kind,
        "name": name,
        "root": _repo_relative(root).as_posix(),
        "path": _repo_relative(path).as_posix(),
    }


def python_example_app_dirs() -> list[Path]:
    root = ROOT / "python/examples"
    return sorted(
        directory
        for directory in root.iterdir()
        if directory.is_dir() and (directory / f"{directory.name}.py").is_file()
    )


def swift_example_binary_path(name: str) -> Path:
    path = ROOT / "swift/examples" / name / ".build/release" / name
    if not path.is_file():
        raise FileNotFoundError(
            f"Swift parity artifact is missing: {path.relative_to(ROOT)}; "
            "run ./tools/rebuild_all.sh"
        )
    return path


def rust_example_binary_path(name: str) -> Path:
    path = ROOT / "rust/examples" / name / "target/release" / name
    if not path.is_file():
        raise FileNotFoundError(
            f"Rust parity artifact is missing: {path.relative_to(ROOT)}; "
            "run ./tools/rebuild_all.sh"
        )
    return path


def parity_artifacts(
    build_dir: Path,
    *,
    include_swift: bool,
    include_rust: bool,
    include_java: bool,
) -> tuple[list[dict[str, str]], list[str]]:
    artifacts: list[dict[str, str]] = []
    platforms = ["cpp", "csharp", "go", "python"]
    python_root = Path("python/examples")
    cpp_root = build_dir / "examples"
    csharp_root = Path("csharp/examples")
    go_root = Path("go/examples")
    rust_root = Path("rust/examples")
    swift_root = Path("swift/examples")
    java_root = JAVA_EXAMPLES_ROOT

    for app_dir in python_example_app_dirs():
        name = app_dir.name
        artifacts.append(_artifact("python", "example", name, python_root, app_dir / f"{name}.py"))
        artifacts.append(
            _artifact(
                "cpp",
                "example",
                name,
                cpp_root,
                example_binary_path(name, build_dir, config=PARITY_CONFIGURATION),
            )
        )
    for project in csharp_example_projects():
        name = project.parent.name
        artifacts.append(
            _artifact(
                "csharp",
                "example",
                name,
                csharp_root,
                csharp_example_dll_path(name, PARITY_CONFIGURATION),
            )
        )
    for app_dir in go_example_app_dirs():
        artifacts.append(_artifact("go", "example", app_dir.name, go_root, app_dir / app_dir.name))
    if include_swift:
        platforms.append("swift")
        for package in swift_example_packages():
            name = package.parent.name
            artifacts.append(
                _artifact("swift", "example", name, swift_root, swift_example_binary_path(name))
            )
    if include_rust:
        platforms.append("rust")
        for app_dir in rust_example_app_dirs():
            artifacts.append(
                _artifact("rust", "example", app_dir.name, rust_root, rust_example_binary_path(app_dir.name))
            )
    if include_java:
        platforms.append("java")
        for app_dir in java_example_app_dirs():
            artifacts.append(
                _artifact(
                    "java",
                    "example",
                    app_dir.name,
                    java_root,
                    java_example_binary_path(app_dir.name),
                )
            )

    regression_python_root = REGRESSION_PARITY_PYTHON_ROOT
    regression_cpp_root = regression_parity_cpp_build_root(build_dir)
    for script in regression_manifest_scripts():
        name = script.stem
        python_path = ROOT / regression_python_root / name / f"{name}.py"
        if python_path.is_file():
            artifacts.append(
                _artifact("python", "regression", name, regression_python_root, python_path)
            )
        artifacts.append(
            _artifact(
                "cpp",
                "regression",
                name,
                regression_cpp_root,
                regression_cpp_binary_path(name, build_dir, config=PARITY_CONFIGURATION),
            )
        )
        go_path = ROOT / GO_REGRESSION_PARITY_ROOT / name / name
        if is_windows():
            go_path = go_path.with_suffix(".exe")
        if go_path.is_file():
            artifacts.append(
                _artifact("go", "regression", name, GO_REGRESSION_PARITY_ROOT, go_path)
            )
        if include_rust:
            artifacts.append(
                _artifact(
                    "rust",
                    "regression",
                    name,
                    RUST_REGRESSION_PARITY_ROOT,
                    rust_regression_binary_path(name),
                )
            )
        if include_java:
            artifacts.append(
                _artifact(
                    "java",
                    "regression",
                    name,
                    JAVA_REGRESSION_PARITY_ROOT,
                    java_regression_binary_path(name),
                )
            )
    return artifacts, platforms


def write_parity_manifest(
    build_dir: Path,
    *,
    include_swift: bool,
    include_rust: bool,
    include_java: bool,
) -> Path:
    artifacts, platforms = parity_artifacts(
        build_dir,
        include_swift=include_swift,
        include_rust=include_rust,
        include_java=include_java,
    )
    path = write_manifest(ROOT, artifacts, platforms=platforms)
    emit_line(f"==> wrote parity artifact manifest: {path.relative_to(ROOT)}")
    return path


def rebuild_all(args: argparse.Namespace) -> None:
    build_dir = Path(args.build_dir)
    validate_swift = should_validate_swift(args)
    validate_rust = should_validate_rust(args)
    validate_java = should_validate_java(args)
    remove_manifest(ROOT)
    run(cmake_configure_args(build_dir, configuration=PARITY_CONFIGURATION))
    uimd = ensure_native_uimd(build_dir, config=PARITY_CONFIGURATION)
    generate_all(
        uimd,
        include_swift=validate_swift,
        include_rust=validate_rust,
        include_java=validate_java,
    )
    run(cmake_configure_args(build_dir, configuration=PARITY_CONFIGURATION))
    run(cmake_build_args(build_dir, config=PARITY_CONFIGURATION))
    build_all_csharp_examples(PARITY_CONFIGURATION)
    build_all_go_examples()
    if validate_java:
        build_all_java_examples()
    else:
        emit_line("==> skip Java runtime, examples, and regressions: --no-java")
    if validate_rust:
        build_all_rust_examples()
    elif args.no_rust:
        emit_line("==> skip Rust examples: --no-rust")
    else:
        emit_line("==> skip Rust examples: Rust validation is not enabled on Windows")
    if validate_swift:
        build_all_swift_examples()
    elif args.no_swift:
        emit_line("==> skip Swift examples: --no-swift")
    else:
        emit_line("==> skip Swift examples: Swift validation is not enabled on Windows")
    run([sys.executable, "-m", "compileall", "python", "src", "tests", "tools"])
    write_parity_manifest(
        build_dir,
        include_swift=validate_swift,
        include_rust=validate_rust,
        include_java=validate_java,
    )
    if args.test:
        run(ctest_args(build_dir, config=PARITY_CONFIGURATION))


def run_python_tests() -> None:
    probe = subprocess.run(
        [sys.executable, "-m", "pytest", "--version"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        check=False,
    )
    live_report_args = ["-p", "tools.pytest_live_report"] if _full_test_reporter is not None else []
    if probe.returncode == 0:
        run([sys.executable, "-m", "pytest", *live_report_args, "python/tests"])
        return
    pytest = shutil.which("pytest")
    if pytest is not None:
        run([pytest, *live_report_args, "python/tests"])
        return
    run([sys.executable, "-m", "pytest", *live_report_args, "python/tests"])


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


def run_java_example_compare(
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
        JAVA_EXAMPLES_ROOT,
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
        emit_line(f"==> skip regression parity compare: {REGRESSION_PARITY_ROOT} does not exist")
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
        emit_line(f"==> skip Go regression parity compare: {GO_REGRESSION_PARITY_ROOT} does not exist")
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
        emit_line(f"==> skip Rust regression parity compare: {RUST_REGRESSION_PARITY_ROOT} does not exist")
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


def run_java_regression_compare_if_available(
    uimd: Path,
    build_dir: Path,
    *,
    compare_app_size: str,
    mcp_fast: bool,
    config: str | None = None,
) -> PhaseSkip | None:
    cpp_root_path = regression_parity_cpp_build_root(build_dir)
    cpp_root = ROOT / cpp_root_path
    java_root = ROOT / JAVA_REGRESSION_PARITY_ROOT
    manifest = ROOT / REGRESSION_PARITY_MANIFEST
    if not cpp_root.exists() and not java_root.exists() and not manifest.exists():
        emit_line(
            f"==> skip Java regression parity compare: "
            f"{JAVA_REGRESSION_PARITY_ROOT} does not exist"
        )
        return PhaseSkip(f"{JAVA_REGRESSION_PARITY_ROOT} does not exist")
    if not cpp_root.exists() or not java_root.exists() or not manifest.exists():
        if not cpp_root.exists():
            missing = cpp_root_path
        elif not java_root.exists():
            missing = JAVA_REGRESSION_PARITY_ROOT
        else:
            missing = REGRESSION_PARITY_MANIFEST
        raise FileNotFoundError(f"Java regression parity compare root is missing: {missing}")

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
            java_regression_binary_path(app_name),
            script,
        ]
        if mcp_fast:
            command.append("--mcp-fast")
        command.extend(["--compare-app-size", compare_app_size])
        run(command)
    return None


def test_all(args: argparse.Namespace) -> None:
    global _full_test_reporter

    if args.keep_going and not args.live_report:
        raise ValueError("--keep-going requires --live-report")
    if args.log_file and not args.live_report:
        raise ValueError("--log-file requires --live-report")

    reporter: FullTestReporter | None = None
    if args.live_report:
        log_path = Path(args.log_file) if args.log_file else default_log_path(ROOT)
        if not log_path.is_absolute():
            log_path = ROOT / log_path
        reporter = FullTestReporter(log_path)
        reporter.open()
        _full_test_reporter = reporter

    phases: list[FullTestPhase] = []
    build_dir = Path(args.build_dir)
    validate_swift = should_validate_swift(args)
    validate_rust = should_validate_rust(args)
    validate_java = should_validate_java(args)
    try:
        if not args.no_rebuild:
            remove_manifest(ROOT)
            run_full_test_phase(
                phases,
                "Configure CMake parity profile",
                lambda: run(
                    cmake_configure_args(
                        build_dir,
                        configuration=PARITY_CONFIGURATION,
                    )
                ),
            )
        uimd = run_full_test_phase(
            phases,
            "Build repo-local uimd tool",
            lambda: ensure_native_uimd(build_dir, config=PARITY_CONFIGURATION),
        )
        if not args.no_rebuild:
            run_full_test_phase(
                phases,
                "Generate UIMD sources",
                lambda: generate_all(
                    uimd,
                    include_swift=validate_swift,
                    include_rust=validate_rust,
                    include_java=validate_java,
                ),
            )
            run_full_test_phase(
                phases,
                "Build C++ runtime, tools, examples, regressions",
                lambda: run(cmake_build_args(build_dir, config=PARITY_CONFIGURATION)),
            )
            run_full_test_phase(
                phases,
                "Build C# runtime and examples",
                lambda: build_all_csharp_examples(PARITY_CONFIGURATION),
            )
            run_full_test_phase(phases, "Build Go runtime, examples, regressions", build_all_go_examples)
            if validate_java:
                run_full_test_phase(
                    phases,
                    "Build Java runtime, examples, regressions",
                    build_all_java_examples,
                )
            else:
                record_skipped_phase(
                    phases,
                    "Build Java runtime, examples, regressions",
                    "--no-java",
                )
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
            run_full_test_phase(
                phases,
                "Write parity artifact manifest",
                lambda: write_parity_manifest(
                    build_dir,
                    include_swift=validate_swift,
                    include_rust=validate_rust,
                    include_java=validate_java,
                ),
            )
        else:
            record_skipped_phase(phases, "Generate UIMD sources", "--no-rebuild")
            record_skipped_phase(phases, "Configure CMake parity profile", "--no-rebuild")
            record_skipped_phase(phases, "Build C++ runtime, tools, examples, regressions", "--no-rebuild")
            record_skipped_phase(phases, "Build C# runtime and examples", "--no-rebuild")
            record_skipped_phase(phases, "Build Go runtime, examples, regressions", "--no-rebuild")
            record_skipped_phase(phases, "Build Java runtime, examples, regressions", "--no-rebuild")
            record_skipped_phase(phases, "Build Rust runtime, examples, regressions", "--no-rebuild")
            record_skipped_phase(phases, "Build Swift runtime and examples", "--no-rebuild")
            record_skipped_phase(phases, "Compile Python sources", "--no-rebuild")
            run_full_test_phase(phases, "Validate parity artifact manifest", lambda: validate_manifest(ROOT))
        run_full_test_phase(
            phases,
            "Python tests",
            run_python_tests,
            report_kind="pytest",
            continue_on_failure=args.keep_going,
        )
        run_full_test_phase(
            phases,
            "CTest",
            lambda: run(ctest_args(build_dir, config=PARITY_CONFIGURATION)),
            report_kind="ctest",
            continue_on_failure=args.keep_going,
        )
        run_full_test_phase(
            phases,
            "C# runtime tests",
            run_csharp_tests,
            report_kind="smoke",
            continue_on_failure=args.keep_going,
        )
        run_full_test_phase(
            phases,
            "Go runtime tests",
            run_go_tests,
            report_kind="go",
            continue_on_failure=args.keep_going,
        )
        if validate_java:
            run_full_test_phase(
                phases,
                "Java runtime tests and Checkstyle",
                run_java_tests,
                continue_on_failure=args.keep_going,
            )
        else:
            record_skipped_phase(phases, "Java runtime tests and Checkstyle", "--no-java")
        if validate_rust:
            run_full_test_phase(
                phases,
                "Rust runtime tests",
                run_rust_tests,
                report_kind="cargo",
                continue_on_failure=args.keep_going,
            )
            run_full_test_phase(
                phases,
                "Rust clippy",
                run_rust_clippy,
                continue_on_failure=args.keep_going,
            )
        elif args.no_rust:
            record_skipped_phase(phases, "Rust runtime tests", "--no-rust")
            record_skipped_phase(phases, "Rust clippy", "--no-rust")
        else:
            record_skipped_phase(phases, "Rust runtime tests", "Rust validation is not enabled on Windows")
            record_skipped_phase(phases, "Rust clippy", "Rust validation is not enabled on Windows")
        if validate_swift:
            run_full_test_phase(
                phases,
                "Swift runtime tests",
                run_swift_tests,
                report_kind="swift",
                continue_on_failure=args.keep_going,
            )
            run_full_test_phase(
                phases,
                "Swift direct terminal smoke",
                lambda: run_swift_direct_terminal_smoke(build_dir),
                report_kind="smoke",
                continue_on_failure=args.keep_going,
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
                report_kind="smoke",
                continue_on_failure=args.keep_going,
            )
        if validate_java and not is_windows():
            run_full_test_phase(
                phases,
                "Java direct terminal smoke",
                lambda: run_java_direct_terminal_smoke(build_dir),
                report_kind="smoke",
                continue_on_failure=args.keep_going,
            )
            run_full_test_phase(
                phases,
                "Java MCP transport smoke",
                run_java_mcp_transport_smoke,
                report_kind="smoke",
                continue_on_failure=args.keep_going,
            )
        elif not validate_java:
            record_skipped_phase(phases, "Java direct terminal smoke", "--no-java")
            record_skipped_phase(phases, "Java MCP transport smoke", "--no-java")
        else:
            record_skipped_phase(phases, "Java direct terminal smoke", "POSIX PTY is required")
            record_skipped_phase(phases, "Java MCP transport smoke", "POSIX PTY is required")
        if validate_rust:
            run_full_test_phase(
                phases,
                "Rust direct terminal smoke",
                lambda: run_rust_direct_terminal_smoke(build_dir),
                report_kind="smoke",
                continue_on_failure=args.keep_going,
            )
            run_full_test_phase(
                phases,
                "Rust MCP transport smoke",
                run_rust_mcp_transport_smoke,
                report_kind="smoke",
                continue_on_failure=args.keep_going,
            )
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
            report_kind="mcp",
            continue_on_failure=args.keep_going,
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
            report_kind="mcp",
            continue_on_failure=args.keep_going,
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
                report_kind="mcp",
                continue_on_failure=args.keep_going,
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
            report_kind="mcp",
            continue_on_failure=args.keep_going,
        )
        if validate_java:
            run_full_test_phase(
                phases,
                "MCP Java example compare",
                lambda: run_java_example_compare(
                    uimd,
                    build_dir,
                    compare_app_size=args.compare_app_size,
                    mcp_fast=not args.no_mcp_fast,
                ),
                report_kind="mcp",
                continue_on_failure=args.keep_going,
            )
        else:
            record_skipped_phase(phases, "MCP Java example compare", "--no-java")
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
                report_kind="mcp",
                continue_on_failure=args.keep_going,
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
            report_kind="mcp",
            continue_on_failure=args.keep_going,
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
            report_kind="mcp",
            continue_on_failure=args.keep_going,
        )
        if validate_java:
            run_full_test_phase(
                phases,
                "MCP Java regression parity compare",
                lambda: run_java_regression_compare_if_available(
                    uimd,
                    build_dir,
                    compare_app_size=args.compare_app_size,
                    mcp_fast=not args.no_mcp_fast,
                    config=PARITY_CONFIGURATION,
                ),
                report_kind="mcp",
                continue_on_failure=args.keep_going,
            )
        else:
            record_skipped_phase(
                phases,
                "MCP Java regression parity compare",
                "--no-java",
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
                    config=PARITY_CONFIGURATION,
                ),
                report_kind="mcp",
                continue_on_failure=args.keep_going,
            )
        elif args.no_rust:
            record_skipped_phase(phases, "MCP Rust regression parity compare", "--no-rust")
        else:
            record_skipped_phase(
                phases,
                "MCP Rust regression parity compare",
                "Rust validation is not enabled on Windows",
            )
    except Exception as exc:
        if reporter is not None:
            return_code = exc.returncode if isinstance(exc, subprocess.CalledProcessError) else 1
            raise subprocess.CalledProcessError(return_code, ["test-all"]) from exc
        raise
    finally:
        try:
            print_full_test_summary(phases)
            if reporter is not None:
                reporter.finish_gate(phases)
        finally:
            if reporter is not None:
                reporter.close()
            _full_test_reporter = None

    if any(phase.status == "FAIL" for phase in phases):
        raise subprocess.CalledProcessError(1, ["test-all"])


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
    rebuild.add_argument("--csharp-config", default=PARITY_CONFIGURATION, choices=(PARITY_CONFIGURATION,))
    rebuild.add_argument("--no-swift", action="store_true")
    rebuild.add_argument("--no-rust", action="store_true")
    rebuild.add_argument("--no-java", action="store_true")
    rebuild.add_argument("--test", action="store_true")
    rebuild.set_defaults(func=rebuild_all)

    all_tests = subparsers.add_parser("test-all")
    all_tests.add_argument("--compare-app-size", default=DEFAULT_COMPARE_APP_SIZE)
    all_tests.add_argument("--no-mcp-fast", action="store_true")
    all_tests.add_argument("--no-rebuild", action="store_true")
    all_tests.add_argument("--csharp-config", default=PARITY_CONFIGURATION, choices=(PARITY_CONFIGURATION,))
    all_tests.add_argument("--no-swift", action="store_true")
    all_tests.add_argument("--no-rust", action="store_true")
    all_tests.add_argument("--no-java", action="store_true")
    all_tests.add_argument("--live-report", action="store_true")
    all_tests.add_argument("--log-file")
    all_tests.add_argument("--keep-going", action="store_true")
    all_tests.set_defaults(func=test_all)

    run_example = subparsers.add_parser("run-cpp-example")
    run_example.add_argument("name")
    run_example.add_argument("app_args", nargs=argparse.REMAINDER)
    run_example.set_defaults(func=run_cpp_example)

    run_csharp = subparsers.add_parser("run-csharp-example")
    run_csharp.add_argument("name")
    run_csharp.add_argument("--csharp-config", default=PARITY_CONFIGURATION)
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
    mcp_csharp.add_argument("--csharp-config", default=PARITY_CONFIGURATION)
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
    compare_csharp.add_argument("--csharp-config", default=PARITY_CONFIGURATION)
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
