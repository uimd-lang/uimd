#!/usr/bin/env python3
"""Build a local UIMD SDK release root for the current platform.

This is intentionally GitHub-independent. The generated directory satisfies the
native `uimd sdk install <version> --release-root <path>` manifest contract and
also emits upload-friendly files for a future GitHub Release.
"""

from __future__ import annotations

import argparse
import hashlib
import platform as platform_module
import re
import shutil
import stat
import subprocess
import sys
import tarfile
from pathlib import Path


SUPPORTED_PLATFORM = "macos-x86_64"

IGNORED_DIR_NAMES = {
    ".git",
    ".mypy_cache",
    ".pytest_cache",
    ".svn",
    "__pycache__",
    "build",
    "build-release",
}
IGNORED_FILE_NAMES = {".DS_Store"}
IGNORED_SUFFIXES = {".pyc", ".pyo"}


def repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def project_version(root: Path) -> str:
    pyproject = root / "pyproject.toml"
    in_project = False
    for raw_line in pyproject.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if line == "[project]":
            in_project = True
            continue
        if in_project and line.startswith("["):
            break
        if in_project:
            match = re.fullmatch(r'version\s*=\s*"([^"]+)"', line)
            if match:
                return match.group(1)
    raise RuntimeError(f"cannot find [project] version in {pyproject}")


def host_platform_label() -> str:
    system = platform_module.system().lower()
    machine = platform_module.machine().lower()
    if system == "darwin" and machine in {"x86_64", "amd64"}:
        return "macos-x86_64"
    if system == "darwin" and machine in {"arm64", "aarch64"}:
        return "macos-arm64"
    if system == "windows" and machine in {"amd64", "x86_64"}:
        return "windows-x64"
    if system == "linux" and machine in {"amd64", "x86_64"}:
        return "linux-x64"
    return f"{system}-{machine}"


def validate_platform(label: str, allow_cross_label: bool) -> None:
    if label != SUPPORTED_PLATFORM:
        raise RuntimeError(
            f"unsupported SDK release platform {label!r}; "
            f"only {SUPPORTED_PLATFORM!r} is enabled for this alpha slice"
        )
    host_label = host_platform_label()
    if label != host_label and not allow_cross_label:
        raise RuntimeError(
            f"refusing to label host {host_label!r} output as {label!r}; "
            "pass --allow-cross-platform-label only for metadata-only dry runs"
        )


def run_command(root: Path, command: list[str]) -> None:
    print("$ " + " ".join(command), flush=True)
    subprocess.run(command, cwd=root, check=True)


def build_native_outputs(root: Path, build_dir: Path) -> None:
    run_command(
        root,
        [
            "cmake",
            "-S",
            "cpp",
            "-B",
            str(build_dir),
            "-DUIMD_EMBED_SOURCE_ROOT=OFF",
        ],
    )
    for target in ("ui_cpp_runtime", "ui_cpp_dialogs", "uimd", "uimd_init"):
        run_command(root, ["cmake", "--build", str(build_dir), "--target", target])


def remove_path(path: Path) -> None:
    if not path.exists() and not path.is_symlink():
        return
    if path.is_dir() and not path.is_symlink():
        shutil.rmtree(path)
    else:
        path.unlink()


def ensure_file(path: Path, hint: str) -> None:
    if not path.is_file():
        raise RuntimeError(f"missing {hint}: {path}")


def copy_file(source: Path, destination: Path, executable: bool = False) -> None:
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)
    if executable:
        destination.chmod(
            stat.S_IRUSR
            | stat.S_IWUSR
            | stat.S_IXUSR
            | stat.S_IRGRP
            | stat.S_IXGRP
            | stat.S_IROTH
            | stat.S_IXOTH
        )


def ignored_relative_path(path: Path) -> bool:
    for part in path.parts:
        if part in IGNORED_DIR_NAMES:
            return True
    return path.name in IGNORED_FILE_NAMES or path.suffix in IGNORED_SUFFIXES


def copy_tree(source: Path, destination: Path) -> None:
    if not source.exists():
        return
    for path in sorted(source.rglob("*")):
        relative = path.relative_to(source)
        if ignored_relative_path(relative):
            continue
        target = destination / relative
        if path.is_dir():
            target.mkdir(parents=True, exist_ok=True)
        elif path.is_file():
            copy_file(path, target)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def write_manifest(release_dir: Path, version: str, payload_dir: Path) -> Path:
    manifest_path = release_dir / "manifest.txt"
    lines = [f"version {version}"]
    payload_files = sorted(path for path in payload_dir.rglob("*") if path.is_file())
    if not payload_files:
        raise RuntimeError(f"release payload is empty: {payload_dir}")
    for path in payload_files:
        relative = path.relative_to(payload_dir).as_posix()
        source = f"payload/{relative}"
        if any(character.isspace() for character in relative):
            raise RuntimeError(f"manifest paths may not contain whitespace: {relative}")
        lines.append(f"file {relative} {sha256_file(path)} {source}")
    manifest_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return manifest_path


def create_tarball(output_root: Path, release_dir: Path, version: str, platform_label: str) -> Path:
    archive_stem = f"uimd-sdk-{version}-{platform_label}"
    archive_path = output_root / f"{archive_stem}.tar.gz"
    remove_path(archive_path)
    with tarfile.open(archive_path, "w:gz") as archive:
        archive.add(release_dir / "manifest.txt", arcname=f"{archive_stem}/manifest.txt")
        archive.add(release_dir / "payload", arcname=f"{archive_stem}/payload")
    return archive_path


def write_checksums(output_root: Path, artifacts: list[Path]) -> Path:
    checksums_path = output_root / "checksums.txt"
    lines = []
    for artifact in sorted(artifacts):
        relative = artifact.relative_to(output_root).as_posix()
        lines.append(f"{sha256_file(artifact)}  {relative}")
    checksums_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return checksums_path


def stage_cpp_install(root: Path, build_dir: Path, stage_dir: Path) -> None:
    remove_path(stage_dir)
    stage_dir.mkdir(parents=True, exist_ok=True)
    run_command(root, ["cmake", "--install", str(build_dir), "--prefix", str(stage_dir)])


def package_release(
    root: Path,
    output_root: Path,
    build_dir: Path,
    version: str,
    platform_label: str,
    build: bool,
) -> list[Path]:
    uimd_binary = build_dir / "tools" / "uimd" / "uimd"
    init_binary = build_dir / "tools" / "uimd_init" / "uimd-init"

    if build:
        build_native_outputs(root, build_dir)

    ensure_file(uimd_binary, f"native uimd binary; run with --build or build {build_dir} first")
    ensure_file(init_binary, f"native uimd-init binary; run with --build or build {build_dir} first")

    output_root.mkdir(parents=True, exist_ok=True)
    release_dir = output_root / version
    payload_dir = release_dir / "payload"
    stage_dir = output_root / ".stage" / "cpp-install"

    remove_path(release_dir)
    release_dir.mkdir(parents=True, exist_ok=True)
    payload_dir.mkdir(parents=True, exist_ok=True)

    copy_file(uimd_binary, payload_dir / "bin" / "uimd", executable=True)

    python_target = payload_dir / "targets" / "python"
    copy_tree(root / "src" / "uimd", python_target / "uimd")
    copy_file(root / "pyproject.toml", python_target / "pyproject.toml")
    copy_file(root / "README.md", python_target / "README.md")
    copy_file(root / "LICENSE", python_target / "LICENSE")

    stage_cpp_install(root, build_dir, stage_dir)
    cpp_target = payload_dir / "targets" / "cpp"
    copy_tree(stage_dir / "bin", cpp_target / "bin")
    copy_tree(stage_dir / "include", cpp_target / "include")
    copy_tree(stage_dir / "lib", cpp_target / "lib")
    copy_file(root / "LICENSE", cpp_target / "LICENSE")

    examples_target = payload_dir / "examples"
    copy_tree(root / "python" / "examples", examples_target / "python")
    copy_tree(root / "cpp" / "examples", examples_target / "cpp")
    copy_tree(root / "shared", examples_target / "shared")

    manifest_path = write_manifest(release_dir, version, payload_dir)
    init_asset = output_root / f"uimd-init-{version}-{platform_label}"
    copy_file(init_binary, init_asset, executable=True)
    archive_path = create_tarball(output_root, release_dir, version, platform_label)
    checksums_path = write_checksums(output_root, [manifest_path, archive_path, init_asset])

    remove_path(output_root / ".stage")
    return [manifest_path, archive_path, init_asset, checksums_path]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("dist") / "sdk-release",
        help="Release root to write (default: dist/sdk-release).",
    )
    parser.add_argument(
        "--version",
        help="SDK version to package (default: [project].version from pyproject.toml).",
    )
    parser.add_argument(
        "--platform",
        default=host_platform_label(),
        help=f"Release platform label (currently only {SUPPORTED_PLATFORM}).",
    )
    parser.add_argument(
        "--build",
        action="store_true",
        help="Configure and build the native targets before packaging.",
    )
    parser.add_argument(
        "--build-dir",
        type=Path,
        default=Path("cpp") / "build-release",
        help="CMake build directory to package (default: cpp/build-release).",
    )
    parser.add_argument(
        "--allow-cross-platform-label",
        action="store_true",
        help="Allow writing a supported label that does not match the host.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    root = repo_root()
    version = args.version or project_version(root)
    platform_label = args.platform
    output_root = (root / args.output).resolve() if not args.output.is_absolute() else args.output
    build_dir = (root / args.build_dir).resolve() if not args.build_dir.is_absolute() else args.build_dir

    try:
        validate_platform(platform_label, args.allow_cross_platform_label)
        artifacts = package_release(
            root=root,
            output_root=output_root,
            build_dir=build_dir,
            version=version,
            platform_label=platform_label,
            build=args.build,
        )
    except (OSError, RuntimeError, subprocess.CalledProcessError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    print("Wrote SDK release artifacts:")
    for artifact in artifacts:
        print(f"  {artifact}")
    print()
    print("Install locally with:")
    installer_binary = build_dir / "tools" / "uimd" / "uimd"
    print(f"  UIMD_HOME=/tmp/uimd-home {installer_binary} sdk install {version} --release-root {output_root}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
