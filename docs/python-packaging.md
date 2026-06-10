# Python Packaging

The public Python runtime package is `uimd`.

```text
Distribution name: uimd
Import namespace:  uimd
CLI command:       uimd-mcp-test
```

This package is for Python applications and local development installs. It must
not own the long-term SDK Store under `~/.uimd` or `%LOCALAPPDATA%\uimd`. The
SDK-facing `uimd` command is the native binary, not a Python package entry
point.

The SDK bootstrap package is planned as `uimd-sdk`. That package should install
only `uimd-init`, which verifies or repairs the SDK Store and installs the
native SDK-facing `uimd` launcher/binary. The native binary is built from the
C++ project:

```bash
cmake --build cpp/build --target uimd
```

The native SDK-facing tool is smoke-tested with:

```bash
python3 tools/native_uimd_parity.py
```

The package source lives under:

```text
src/uimd/
```

The root `pyproject.toml` allows installs from the repository root:

```bash
python3 -m pip install .
```

and from GitHub:

```bash
python3 -m pip install git+https://github.com/uimd-lang/uimd.git
```

## Build

```bash
python3 -m build
```

## TestPyPI

Use TestPyPI before the first public release.

## PyPI

Create a PyPI account before the first public release:

```text
https://pypi.org/account/register/
```

Official releases should use GitHub Actions with PyPI Trusted Publishing.
Each uploaded PyPI version is immutable. Fixes require a new version number.

Manual upload fallback:

```bash
python3 -m pip install build twine
python3 -m build
python3 -m twine upload dist/*
```

## Versioning

UIMD uses SemVer.

Before the public API is stable, versions use the `0.x.y` form:

- Bump `y` for the normal development flow: bug fixes, performance tuning,
  small refactors, compatible behavior changes, documentation updates,
  packaging fixes, and ordinary new functionality.
- Evaluate `x` only immediately before a `main` push. Bump it only when
  explicitly choosing a larger release line or when a larger batch of
  functionality has accumulated since the last `main` push, such as a new
  supported platform, a new UI element, a major SDK installation milestone, or a
  substantial change to existing functionality.
- Do not bump `x` just because one ordinary feature was added.
- When `x` is bumped, reset `y` to `0`.
- Keep unreleased SDK installation docs aligned with the actual supported
  installer/release-artifact state.

When the version changes, do not edit version surfaces by hand. Run:

```bash
python3 tools/set_version.py 0.x.y
```

The script updates all tracked source, CMake, and documentation version
surfaces that carry the current package/release version:

- `pyproject.toml`
- `src/uimd/__init__.py`
- `cpp/CMakeLists.txt`
- CMake package version emitted from `${PROJECT_VERSION}`
- generated C++ `FetchContent` fallback tags emitted by the native tool sources
- C++ runtime version reported by `ui::runtimeVersion()`
- `CHANGELOG.md`
- release documentation, CMake snippets, and tags

Run the focused version consistency test after changing the version:

```bash
PYTHONPATH=python:src python3 -m pytest python/tests/test_version_consistency.py
```

## Release Checklist

- Confirm `pyproject.toml` version.
- Decide whether this release needs a `0.x.y` version bump.
- Build the native SDK-facing `uimd` binary.
- Run native CLI smoke checks.
- Run Python tests.
- Regenerate UI sources.
- Build and test C++ examples.
- Build the Python package.
- Publish to TestPyPI for the first public release.
- Tag the release as the current `v0.x.y` version.
