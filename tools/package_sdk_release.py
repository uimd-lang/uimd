#!/usr/bin/env python3
"""Build a local UIMD SDK release root for the current platform.

This is intentionally GitHub-independent. The generated directory satisfies the
native `uimd sdk install <version> --release-root <path>` manifest contract and
also emits upload-friendly files for a future GitHub Release.
"""

from __future__ import annotations

import argparse
import hashlib
import os
import platform as platform_module
import re
import shutil
import stat
import subprocess
import sys
import tarfile
from pathlib import Path


SUPPORTED_PLATFORMS = {
    "macos-x86_64",
    "macos-arm64",
    "linux-x86_64",
    "linux-arm64",
    "windows-x86_64",
    "windows-arm64",
}

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
RELEASE_PUBLIC_KEY_PATH = Path("signing") / "uimd-release.pub"
CHECKSUMS_SIGNATURE_NAME = "checksums.txt.minisig"
DEFAULT_SIGNING_KEY_RELATIVE_PATH = Path("projects-signing") / "uimd" / "uimd-release.key"


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
        return "windows-x86_64"
    if system == "windows" and machine in {"arm64", "aarch64"}:
        return "windows-arm64"
    if system == "linux" and machine in {"amd64", "x86_64"}:
        return "linux-x86_64"
    if system == "linux" and machine in {"arm64", "aarch64"}:
        return "linux-arm64"
    return f"{system}-{machine}"


def validate_platform(label: str, allow_cross_label: bool) -> None:
    if label not in SUPPORTED_PLATFORMS:
        raise RuntimeError(
            f"unsupported SDK release platform {label!r}; "
            f"supported platforms: {', '.join(sorted(SUPPORTED_PLATFORMS))}"
        )
    host_label = host_platform_label()
    if label != host_label and not allow_cross_label:
        raise RuntimeError(
            f"refusing to label host {host_label!r} output as {label!r}; "
            "pass --allow-cross-platform-label only for metadata-only dry runs"
        )


def candidate_volume_roots() -> list[Path]:
    roots: list[Path] = []
    if sys.platform == "darwin":
        volumes = Path("/Volumes")
        if volumes.is_dir():
            roots.extend(path for path in volumes.iterdir() if path.is_dir())
    elif sys.platform.startswith("linux"):
        user = os.environ.get("USER")
        for base in (Path("/media"), Path("/run/media"), Path("/mnt")):
            if user and (base / user).is_dir():
                roots.extend(path for path in (base / user).iterdir() if path.is_dir())
            if base.is_dir():
                roots.extend(path for path in base.iterdir() if path.is_dir())
    elif sys.platform.startswith("win"):
        for letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
            drive = Path(f"{letter}:\\")
            if drive.exists():
                roots.append(drive)
    return roots


def unique_existing_paths(paths: list[Path]) -> list[Path]:
    seen: set[Path] = set()
    result: list[Path] = []
    for path in paths:
        try:
            normalized = path.expanduser().resolve()
        except OSError:
            normalized = path.expanduser().absolute()
        if normalized in seen or not normalized.is_file():
            continue
        seen.add(normalized)
        result.append(normalized)
    return result


def config_signing_key_paths() -> list[Path]:
    paths: list[Path] = []
    for config_path in (
        Path.home() / ".config" / "uimd" / "release-signing-key",
        Path.home() / ".uimd-release-signing-key",
    ):
        if not config_path.is_file():
            continue
        for line in config_path.read_text(encoding="utf-8").splitlines():
            text = line.strip()
            if text and not text.startswith("#"):
                paths.append(Path(text).expanduser())
                break
    return paths


def discover_signing_key() -> Path | None:
    config_matches = unique_existing_paths(config_signing_key_paths())
    if len(config_matches) > 1:
        formatted = "\n".join(f"  - {path}" for path in config_matches)
        raise RuntimeError(
            "multiple configured release signing keys were found; pass --signing-key explicitly:\n"
            + formatted
        )
    if config_matches:
        return config_matches[0]

    volume_candidates = [root / DEFAULT_SIGNING_KEY_RELATIVE_PATH for root in candidate_volume_roots()]
    matches = unique_existing_paths(volume_candidates)
    if not matches:
        return None
    if len(matches) > 1:
        formatted = "\n".join(f"  - {path}" for path in matches)
        raise RuntimeError(
            "multiple release signing keys were found; pass --signing-key explicitly:\n"
            + formatted
        )
    return matches[0]


def platform_is_windows(label: str) -> bool:
    return label.startswith("windows-")


def executable_name(base_name: str, platform_label: str) -> str:
    return f"{base_name}.exe" if platform_is_windows(platform_label) else base_name


def init_asset_name(version: str, platform_label: str) -> str:
    return f"uimd-init-{version}-{platform_label}" + (".exe" if platform_is_windows(platform_label) else "")


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


def minisign_public_key(public_key_path: Path) -> str:
    ensure_file(public_key_path, "minisign public release key")
    lines = [line.strip() for line in public_key_path.read_text(encoding="utf-8").splitlines() if line.strip()]
    if len(lines) < 2:
        raise RuntimeError(f"minisign public key file is invalid: {public_key_path}")
    return lines[1]


def sign_checksums(root: Path, checksums_path: Path, signing_key: Path) -> Path:
    ensure_file(signing_key, "minisign private release signing key")
    signature_path = checksums_path.with_name(CHECKSUMS_SIGNATURE_NAME)
    remove_path(signature_path)
    run_command(
        root,
        [
            "minisign",
            "-S",
            "-s",
            str(signing_key),
            "-m",
            str(checksums_path),
            "-x",
            str(signature_path),
        ],
    )
    ensure_file(signature_path, "minisign checksums signature")
    return signature_path


def install_script_text(version: str, public_key: str) -> str:
    return f"""#!/bin/sh
set -eu

UIMD_VERSION="${{UIMD_VERSION:-{version}}}"
UIMD_PLATFORM="${{UIMD_PLATFORM:-}}"

detect_platform() {{
    os="$(uname -s)"
    arch="$(uname -m)"
    case "${{os}}:${{arch}}" in
        Darwin:x86_64)
            printf '%s\\n' 'macos-x86_64'
            ;;
        Darwin:arm64|Darwin:aarch64)
            printf '%s\\n' 'macos-arm64'
            ;;
        Linux:x86_64|Linux:amd64)
            printf '%s\\n' 'linux-x86_64'
            ;;
        Linux:arm64|Linux:aarch64)
            printf '%s\\n' 'linux-arm64'
            ;;
        *)
            printf '%s\\n' "unsupported platform: ${{os}} ${{arch}}" >&2
            exit 1
            ;;
    esac
}}

if [ -z "${{UIMD_PLATFORM}}" ]; then
    UIMD_PLATFORM="$(detect_platform)"
fi

BASE_URL="${{UIMD_RELEASE_BASE_URL:-https://github.com/uimd-lang/uimd/releases/download/v${{UIMD_VERSION}}}}"
BASE_URL="${{BASE_URL%/}}"
INIT_ASSET="uimd-init-${{UIMD_VERSION}}-${{UIMD_PLATFORM}}"

tmpdir="$(mktemp -d "${{TMPDIR:-/tmp}}/uimd-install.XXXXXX")"
cleanup() {{
    rm -rf "$tmpdir"
}}
trap cleanup EXIT INT TERM

download() {{
    url="$1"
    output="$2"
    curl -fsSL --retry 3 -o "$output" "$url"
}}

sha256_file() {{
    if command -v shasum >/dev/null 2>&1; then
        LC_ALL=C shasum -a 256 "$1" | awk '{{print $1}}'
    elif command -v sha256sum >/dev/null 2>&1; then
        sha256sum "$1" | awk '{{print $1}}'
    else
        printf '%s\\n' 'missing shasum or sha256sum for checksum verification' >&2
        exit 1
    fi
}}

checksum_for() {{
    awk -v asset="$1" '$2 == asset {{print $1; exit}}' "$tmpdir/checksums.txt"
}}

verify_checksums_signature() {{
    if ! command -v minisign >/dev/null 2>&1; then
        printf '%s\\n' 'missing minisign for signed release verification' >&2
        exit 1
    fi
    minisign -Vq -P '{public_key}' -m "$tmpdir/checksums.txt" -x "$tmpdir/checksums.txt.minisig"
}}

download "$BASE_URL/checksums.txt" "$tmpdir/checksums.txt"
download "$BASE_URL/checksums.txt.minisig" "$tmpdir/checksums.txt.minisig"
verify_checksums_signature
download "$BASE_URL/$INIT_ASSET" "$tmpdir/$INIT_ASSET"

expected="$(checksum_for "$INIT_ASSET")"
if [ -z "$expected" ]; then
    printf '%s\\n' "missing checksum for $INIT_ASSET" >&2
    exit 1
fi
actual="$(sha256_file "$tmpdir/$INIT_ASSET")"
if [ "$actual" != "$expected" ]; then
    printf '%s\\n' "checksum mismatch for $INIT_ASSET" >&2
    printf '%s\\n' "expected: $expected" >&2
    printf '%s\\n' "actual:   $actual" >&2
    exit 1
fi

chmod +x "$tmpdir/$INIT_ASSET"
exec "$tmpdir/$INIT_ASSET" "$@"
"""


def install_powershell_script_text(version: str, public_key: str) -> str:
    return f"""$ErrorActionPreference = "Stop"

$Version = if ($env:UIMD_VERSION) {{ $env:UIMD_VERSION }} else {{ "{version}" }}
$Platform = $env:UIMD_PLATFORM

function Get-UimdPlatform {{
    $arch = $env:PROCESSOR_ARCHITEW6432
    if (-not $arch) {{
        $arch = $env:PROCESSOR_ARCHITECTURE
    }}
    switch ($arch) {{
        "AMD64" {{ return "windows-x86_64" }}
        "ARM64" {{ return "windows-arm64" }}
        default {{ throw "unsupported Windows architecture: $arch" }}
    }}
}}

if (-not $Platform) {{
    $Platform = Get-UimdPlatform
}}
if (-not $Platform.StartsWith("windows-")) {{
    throw "install.ps1 supports Windows assets only, got $Platform"
}}

$BaseUrl = if ($env:UIMD_RELEASE_BASE_URL) {{
    $env:UIMD_RELEASE_BASE_URL.TrimEnd("/")
}} else {{
    "https://github.com/uimd-lang/uimd/releases/download/v$Version"
}}
$InitAsset = "uimd-init-$Version-$Platform.exe"
$TempRoot = Join-Path ([System.IO.Path]::GetTempPath()) ("uimd-install-" + [System.Guid]::NewGuid().ToString("N"))
New-Item -ItemType Directory -Path $TempRoot | Out-Null

try {{
    $ChecksumsPath = Join-Path $TempRoot "checksums.txt"
    $ChecksumsSignaturePath = Join-Path $TempRoot "checksums.txt.minisig"
    $InitPath = Join-Path $TempRoot $InitAsset
    Invoke-WebRequest -UseBasicParsing -Uri "$BaseUrl/checksums.txt" -OutFile $ChecksumsPath
    Invoke-WebRequest -UseBasicParsing -Uri "$BaseUrl/checksums.txt.minisig" -OutFile $ChecksumsSignaturePath
    $Minisign = Get-Command minisign -ErrorAction SilentlyContinue
    if (-not $Minisign) {{
        throw "missing minisign for signed release verification"
    }}
    & $Minisign.Source -Vq -P "{public_key}" -m $ChecksumsPath -x $ChecksumsSignaturePath
    if ($LASTEXITCODE -ne 0) {{
        throw "checksums.txt signature verification failed"
    }}
    Invoke-WebRequest -UseBasicParsing -Uri "$BaseUrl/$InitAsset" -OutFile $InitPath

    $Expected = $null
    foreach ($Line in Get-Content -Path $ChecksumsPath) {{
        if ($Line -match "^([0-9a-fA-F]{{64}})\\s+(.+)$" -and $Matches[2] -eq $InitAsset) {{
            $Expected = $Matches[1].ToLowerInvariant()
            break
        }}
    }}
    if (-not $Expected) {{
        throw "missing checksum for $InitAsset"
    }}

    $Actual = (Get-FileHash -Algorithm SHA256 -Path $InitPath).Hash.ToLowerInvariant()
    if ($Actual -ne $Expected) {{
        throw "checksum mismatch for $InitAsset; expected $Expected, got $Actual"
    }}

    & $InitPath @args
    exit $LASTEXITCODE
}} finally {{
    Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $TempRoot
}}
"""


def write_install_script(output_root: Path, version: str, public_key: str) -> Path:
    script_path = output_root / "install.sh"
    script_path.write_text(install_script_text(version, public_key), encoding="utf-8")
    script_path.chmod(
        stat.S_IRUSR
        | stat.S_IWUSR
        | stat.S_IXUSR
        | stat.S_IRGRP
        | stat.S_IXGRP
        | stat.S_IROTH
        | stat.S_IXOTH
    )
    return script_path


def write_install_powershell_script(output_root: Path, version: str, public_key: str) -> Path:
    script_path = output_root / "install.ps1"
    script_path.write_text(install_powershell_script_text(version, public_key), encoding="utf-8")
    return script_path


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
    signing_key: Path | None,
    public_key_path: Path,
) -> list[Path]:
    uimd_binary_name = executable_name("uimd", platform_label)
    init_binary_name = executable_name("uimd-init", platform_label)
    uimd_binary = build_dir / "tools" / "uimd" / uimd_binary_name
    init_binary = build_dir / "tools" / "uimd_init" / init_binary_name

    if build:
        build_native_outputs(root, build_dir)

    if signing_key is None:
        raise RuntimeError("release signing key is required; pass --signing-key or set UIMD_RELEASE_SIGNING_KEY")
    public_key = minisign_public_key(public_key_path)

    ensure_file(uimd_binary, f"native uimd binary; run with --build or build {build_dir} first")
    ensure_file(init_binary, f"native uimd-init binary; run with --build or build {build_dir} first")

    output_root.mkdir(parents=True, exist_ok=True)
    release_dir = output_root / version
    payload_dir = release_dir / "payload"
    stage_dir = output_root / ".stage" / "cpp-install"

    remove_path(release_dir)
    release_dir.mkdir(parents=True, exist_ok=True)
    payload_dir.mkdir(parents=True, exist_ok=True)

    copy_file(uimd_binary, payload_dir / "bin" / uimd_binary_name, executable=True)

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
    init_asset = output_root / init_asset_name(version, platform_label)
    copy_file(init_binary, init_asset, executable=True)
    archive_path = create_tarball(output_root, release_dir, version, platform_label)
    install_script = write_install_script(output_root, version, public_key)
    install_powershell_script = write_install_powershell_script(output_root, version, public_key)
    checksums_path = write_checksums(output_root, [manifest_path, archive_path, init_asset, install_script, install_powershell_script])
    signature_path = sign_checksums(root, checksums_path, signing_key)

    remove_path(output_root / ".stage")
    artifacts = [manifest_path, archive_path, init_asset, install_script, install_powershell_script, checksums_path]
    artifacts.append(signature_path)
    return artifacts


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
        help=f"Release platform label (supported: {', '.join(sorted(SUPPORTED_PLATFORMS))}).",
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
    parser.add_argument(
        "--signing-key",
        type=Path,
        help=(
            "Minisign private key used to sign checksums.txt as checksums.txt.minisig. "
            "When omitted, UIMD_RELEASE_SIGNING_KEY, ~/.config/uimd/release-signing-key, "
            "and mounted */projects-signing/uimd/uimd-release.key paths are checked."
        ),
    )
    parser.add_argument(
        "--public-key",
        type=Path,
        default=Path(os.environ["UIMD_RELEASE_PUBLIC_KEY_FILE"]) if os.environ.get("UIMD_RELEASE_PUBLIC_KEY_FILE") else None,
        help="Minisign public key to embed in bootstrap scripts (default: signing/uimd-release.pub).",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    root = repo_root()
    version = args.version or project_version(root)
    platform_label = args.platform
    output_root = (root / args.output).resolve() if not args.output.is_absolute() else args.output
    build_dir = (root / args.build_dir).resolve() if not args.build_dir.is_absolute() else args.build_dir
    signing_key = args.signing_key
    if signing_key is None and os.environ.get("UIMD_RELEASE_SIGNING_KEY"):
        signing_key = Path(os.environ["UIMD_RELEASE_SIGNING_KEY"])
    if signing_key is None:
        signing_key = discover_signing_key()
    if signing_key is not None and not signing_key.is_absolute():
        signing_key = (root / signing_key).resolve()
    public_key_path = args.public_key
    if public_key_path is None:
        public_key_path = root / RELEASE_PUBLIC_KEY_PATH
    elif not public_key_path.is_absolute():
        public_key_path = (root / public_key_path).resolve()

    try:
        validate_platform(platform_label, args.allow_cross_platform_label)
        artifacts = package_release(
            root=root,
            output_root=output_root,
            build_dir=build_dir,
            version=version,
            platform_label=platform_label,
            build=args.build,
            signing_key=signing_key,
            public_key_path=public_key_path,
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
