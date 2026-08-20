"""Canonical build-artifact manifest for repository parity validation."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import tempfile
import uuid


MANIFEST_SCHEMA_VERSION = 1
PARITY_PROFILE = "parity"
MANIFEST_RELATIVE_PATH = Path(".uimd/build-manifest.json")
MANAGED_ROOT_PREFIXES = (
    Path("python/examples"),
    Path("cpp/build/examples"),
    Path("cpp/build-windows/examples"),
    Path("csharp/examples"),
    Path("swift/examples"),
    Path("go/examples"),
    Path("rust/examples"),
    Path("java/examples"),
    Path("tests/regressions/uimd/parity/python"),
    Path("cpp/build/regressions/uimd/parity"),
    Path("cpp/build-windows/regressions/uimd/parity"),
    Path("go/regressions/uimd/parity"),
    Path("rust/regressions/uimd/parity"),
    Path("java/regressions/uimd/parity"),
)
SOURCE_INPUTS = (
    Path("src"),
    Path("python/examples"),
    Path("cpp/CMakeLists.txt"),
    Path("cpp/cmake"),
    Path("cpp/dialogs"),
    Path("cpp/examples"),
    Path("cpp/include"),
    Path("cpp/src"),
    Path("cpp/third_party"),
    Path("cpp/tools"),
    Path("csharp/src"),
    Path("csharp/examples"),
    Path("swift/src"),
    Path("swift/examples"),
    Path("go/src"),
    Path("go/examples"),
    Path("go/regressions"),
    Path("rust/src"),
    Path("rust/examples"),
    Path("rust/regressions"),
    Path("java"),
    Path("tests/regressions/uimd/parity"),
    Path("tools/uimd_dev.py"),
    Path("tools/rebuild_all.sh"),
    Path("tools/rebuild_all.ps1"),
    Path("tools/rebuild_all.cmd"),
)
EXCLUDED_DIRECTORY_NAMES = {
    ".build",
    ".gradle",
    ".pytest_cache",
    ".swiftpm",
    "__pycache__",
    "bin",
    "build",
    "obj",
    "target",
}


class BuildManifestError(RuntimeError):
    """Raised when a managed build artifact is missing, stale, or ambiguous."""


def _relative_path(root: Path, path: Path) -> str:
    resolved_root = root.resolve()
    resolved_path = path.resolve()
    try:
        return resolved_path.relative_to(resolved_root).as_posix()
    except ValueError as exc:
        raise BuildManifestError(f"artifact is outside the project root: {resolved_path}") from exc


def _sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _excluded_directory(relative: Path) -> bool:
    if relative.name in EXCLUDED_DIRECTORY_NAMES:
        return True
    return len(relative.parts) >= 2 and relative.parts[0] == "cpp" and relative.parts[1].startswith("build")


def _source_files(root: Path, artifact_paths: set[str]) -> list[Path]:
    files: set[Path] = set()
    for relative_input in SOURCE_INPUTS:
        source = root / relative_input
        if source.is_file():
            if relative_input.as_posix() not in artifact_paths:
                files.add(source)
            continue
        if not source.is_dir():
            continue
        for current_root, directory_names, file_names in os.walk(source):
            current = Path(current_root)
            directory_names[:] = [
                name
                for name in directory_names
                if not _excluded_directory((current / name).relative_to(root))
            ]
            for name in file_names:
                candidate = current / name
                relative = candidate.relative_to(root).as_posix()
                if relative not in artifact_paths:
                    files.add(candidate)
    return sorted(files, key=lambda path: path.relative_to(root).as_posix())


def _source_hashes(root: Path, artifact_paths: set[str]) -> dict[str, str]:
    return {
        path.relative_to(root).as_posix(): _sha256_file(path)
        for path in _source_files(root, artifact_paths)
    }


def _source_fingerprint(source_hashes: dict[str, str]) -> str:
    digest = hashlib.sha256()
    for path, file_hash in sorted(source_hashes.items()):
        digest.update(path.encode("utf-8"))
        digest.update(b"\0")
        digest.update(file_hash.encode("ascii"))
        digest.update(b"\n")
    return digest.hexdigest()


def manifest_path(root: Path) -> Path:
    return root.resolve() / MANIFEST_RELATIVE_PATH


def remove_manifest(root: Path) -> None:
    path = manifest_path(root)
    if path.exists():
        path.unlink()


def create_manifest(root: Path, artifacts: list[dict[str, str]], *, platforms: list[str]) -> dict:
    resolved_root = root.resolve()
    normalized_artifacts = []
    for artifact in artifacts:
        path = Path(artifact["path"])
        if not path.is_absolute():
            path = resolved_root / path
        if not path.is_file():
            raise BuildManifestError(f"rebuilt artifact is missing: {_relative_path(resolved_root, path)}")
        normalized_artifacts.append(
            {
                "kind": artifact["kind"],
                "platform": artifact["platform"],
                "name": artifact["name"],
                "root": Path(artifact["root"]).as_posix(),
                "path": _relative_path(resolved_root, path),
                "sha256": _sha256_file(path),
            }
        )
    normalized_artifacts.sort(
        key=lambda item: (item["kind"], item["platform"], item["name"], item["path"])
    )
    artifact_paths = {artifact["path"] for artifact in normalized_artifacts}
    source_hashes = _source_hashes(resolved_root, artifact_paths)
    return {
        "schema_version": MANIFEST_SCHEMA_VERSION,
        "profile": PARITY_PROFILE,
        "build_session": str(uuid.uuid4()),
        "platforms": sorted(platforms),
        "source_fingerprint": _source_fingerprint(source_hashes),
        "sources": source_hashes,
        "artifacts": normalized_artifacts,
    }


def write_manifest(root: Path, artifacts: list[dict[str, str]], *, platforms: list[str]) -> Path:
    resolved_root = root.resolve()
    document = create_manifest(resolved_root, artifacts, platforms=platforms)
    path = manifest_path(resolved_root)
    path.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(prefix="build-manifest-", suffix=".json", dir=path.parent)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8") as handle:
            json.dump(document, handle, indent=2, sort_keys=True)
            handle.write("\n")
        os.replace(temporary_name, path)
    finally:
        if os.path.exists(temporary_name):
            os.unlink(temporary_name)
    return path


def load_manifest(root: Path, *, required: bool = True) -> dict | None:
    path = manifest_path(root)
    if not path.is_file():
        if required:
            raise BuildManifestError(
                f"parity build manifest is missing: {MANIFEST_RELATIVE_PATH}; run ./tools/rebuild_all.sh"
            )
        return None
    try:
        document = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise BuildManifestError(
            f"parity build manifest is invalid: {MANIFEST_RELATIVE_PATH}; run ./tools/rebuild_all.sh"
        ) from exc
    if document.get("schema_version") != MANIFEST_SCHEMA_VERSION:
        raise BuildManifestError(
            f"parity build manifest schema is obsolete: {MANIFEST_RELATIVE_PATH}; run ./tools/rebuild_all.sh"
        )
    if document.get("profile") != PARITY_PROFILE:
        raise BuildManifestError(
            f"expected {PARITY_PROFILE!r} build profile, got {document.get('profile')!r}; "
            "run ./tools/rebuild_all.sh"
        )
    return document


def validate_sources(root: Path, document: dict) -> None:
    artifact_paths = {artifact["path"] for artifact in document.get("artifacts", [])}
    current_hashes = _source_hashes(root.resolve(), artifact_paths)
    current_fingerprint = _source_fingerprint(current_hashes)
    if current_hashes != document.get("sources") or current_fingerprint != document.get("source_fingerprint"):
        recorded = document.get("sources", {})
        changed = sorted(
            path
            for path in set(recorded) | set(current_hashes)
            if recorded.get(path) != current_hashes.get(path)
        )
        detail = f" (first changed input: {changed[0]})" if changed else ""
        raise BuildManifestError(
            f"parity build is stale because source inputs changed after rebuild{detail}; "
            "run ./tools/rebuild_all.sh"
        )


def _artifact_absolute_path(root: Path, artifact: dict) -> Path:
    return root.resolve() / artifact["path"]


def validate_artifacts(root: Path, document: dict, artifacts: list[dict]) -> None:
    for artifact in artifacts:
        path = _artifact_absolute_path(root, artifact)
        if not path.is_file():
            raise BuildManifestError(f"parity artifact is missing: {artifact['path']}; run ./tools/rebuild_all.sh")
        if _sha256_file(path) != artifact.get("sha256"):
            raise BuildManifestError(f"parity artifact changed after rebuild: {artifact['path']}; run ./tools/rebuild_all.sh")


def validate_manifest(root: Path) -> dict:
    document = load_manifest(root)
    assert document is not None
    validate_sources(root, document)
    return document


def _normalized_relative(root: Path, value: Path | str) -> str | None:
    path = Path(value)
    if not path.is_absolute():
        path = root.resolve() / path
    try:
        return path.resolve().relative_to(root.resolve()).as_posix()
    except ValueError:
        return None


def resolve_artifact(root: Path, examples_root: Path | str, name: str) -> str | None:
    document = load_manifest(root, required=False)
    if document is None:
        return None
    relative_root = _normalized_relative(root, examples_root)
    matches = [
        artifact
        for artifact in document.get("artifacts", [])
        if artifact.get("root") == relative_root
        and artifact.get("name") == name
    ]
    if not matches:
        managed_roots = {artifact.get("root") for artifact in document.get("artifacts", [])}
        if relative_root in managed_roots:
            raise BuildManifestError(
                f"parity manifest has no rebuilt app {name!r} under {relative_root}; run ./tools/rebuild_all.sh"
            )
        return None
    if len(matches) != 1:
        raise BuildManifestError(f"parity manifest has ambiguous app {name!r} under {relative_root}")
    return str(_artifact_absolute_path(root, matches[0]))


def validate_artifact_paths(root: Path, paths: list[Path | str]) -> None:
    document = validate_manifest(root)
    artifacts = document.get("artifacts", [])
    by_path = {artifact["path"]: artifact for artifact in artifacts}
    selected: dict[str, dict] = {}
    for value in paths:
        relative = _normalized_relative(root, value)
        if relative in by_path:
            selected[relative] = by_path[relative]
            continue
        root_matches = [artifact for artifact in artifacts if artifact.get("root") == relative]
        for artifact in root_matches:
            selected[artifact["path"]] = artifact
        if root_matches:
            continue
        if relative is not None and any(
            relative == prefix.as_posix() or relative.startswith(prefix.as_posix() + "/")
            for prefix in MANAGED_ROOT_PREFIXES
        ):
            raise BuildManifestError(
                f"path is not part of the last parity rebuild: {relative}; run ./tools/rebuild_all.sh"
            )
    validate_artifacts(root, document, list(selected.values()))


def validate_cli_args(root: Path, arguments: list[str]) -> None:
    candidates = [argument for argument in arguments if argument and not argument.startswith("-")]
    document = load_manifest(root, required=False)
    manifest_roots = {
        artifact.get("root")
        for artifact in (document or {}).get("artifacts", [])
        if artifact.get("root")
    }
    manifest_paths = {
        artifact.get("path")
        for artifact in (document or {}).get("artifacts", [])
        if artifact.get("path")
    }
    managed = []
    for candidate in candidates:
        relative = _normalized_relative(root, candidate)
        if relative is not None and (
            relative in manifest_paths
            or relative in manifest_roots
            or any(
                relative == prefix.as_posix() or relative.startswith(prefix.as_posix() + "/")
                for prefix in MANAGED_ROOT_PREFIXES
            )
        ):
            managed.append(candidate)
    if managed:
        validate_artifact_paths(root, managed)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    validate = subparsers.add_parser("validate-cli")
    validate.add_argument("--root", type=Path, required=True)
    validate.add_argument("arguments", nargs=argparse.REMAINDER)
    args = parser.parse_args(argv)
    try:
        if args.command == "validate-cli":
            arguments = args.arguments[1:] if args.arguments[:1] == ["--"] else args.arguments
            validate_cli_args(args.root, arguments)
    except BuildManifestError as exc:
        print(f"error: {exc}", file=os.sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
