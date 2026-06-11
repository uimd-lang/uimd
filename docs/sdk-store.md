# UIMD SDK Store

The long-term SDK install root is per user:

```text
macOS/Linux: ~/.uimd
Windows:     %LOCALAPPDATA%\uimd
Override:    UIMD_HOME
```

The native SDK-facing `uimd` command inspects this location with:

```bash
uimd sdk home
uimd sdk install 0.x.y
uimd sdk install 0.x.y --from /path/to/uimd
uimd sdk install 0.x.y --release-root /path/to/releases
uimd sdk install-target cpp
uimd sdk use 0.x.y
uimd sdk list
uimd sdk list --json
uimd sdk remove 0.x.y
uimd sdk prune
uimd sdk update
uimd sdk update --release-root /tmp/uimd-releases
uimd self update
uimd self update --release-root /tmp/uimd-releases
uimd self uninstall
uimd doctor --json
```

Project commands that run through `UIMD_HOME/bin/uimd` read `sdk-version` from
the root `.uimd` Metadata. The launcher selects the highest installed SDK
version that satisfies the requirement, warns when falling back for missing
metadata, and supports `--require-sdk-version` and `--offline` for strict or
network-free contexts. If the required SDK version is missing, or a project
command needs a supported target that is missing from the selected SDK, the
launcher auto-installs the SDK or target from release assets before delegating.
`--offline` / `UIMD_NO_AUTO_INSTALL=1` disables the auto-install attempt.
`UIMD_RELEASE_ROOT` and
`UIMD_RELEASE_BASE_URL` are development/CI overrides.

For development, `UIMD_SDK_PATH=/path/to/uimd` or a local SDK/source checkout
path bypasses SDK Store version resolution for project commands. The launcher
still handles `sdk`, `self`, and `doctor` locally.

Local source builds also provide the bootstrapper target:

```bash
cmake --build cpp/build --target uimd_init
UIMD_HOME=/tmp/uimd-home \
UIMD_INIT_UIMD_BINARY=$PWD/cpp/build/tools/uimd/uimd \
  cpp/build/tools/uimd_init/uimd-init --no-shell-config --json
```

For human installs, pass `--modify-shell` instead. It idempotently adds the
SDK launcher directory to the active user's shell profile (`.zshrc`, `.bashrc`,
`.profile`, Fish config, or the PowerShell profile). `--no-shell-config` is the
agent/CI path and leaves shell startup files untouched.

The intended layout is:

```text
~/.uimd/
├── bin/uimd
├── current
└── sdk/
    └── 0.x.y/
        ├── bin/uimd
        ├── targets/python/
        ├── targets/cpp/
        └── examples/
```

## Local Release Manifest

Before public GitHub Releases exist, the native installer can exercise the
release contract from a local release root:

```bash
uimd sdk install 0.x.y --release-root /tmp/uimd-releases
```

The installer looks for `/tmp/uimd-releases/0.x.y/manifest.txt` first, then
`/tmp/uimd-releases/uimd-sdk-0.x.y.manifest`. The manifest format is
line-oriented:

```text
version 0.x.y
file bin/uimd <sha256> payload/uimd
file targets/python/runtime.txt <sha256> payload/targets/python/runtime.txt
file targets/cpp/runtime.txt <sha256> payload/targets/cpp/runtime.txt
```

Each `file` entry is copied under `sdk/<version>/...` after SHA-256
verification. Paths must be relative and may not contain `..`; `bin/uimd`
(`bin/uimd.exe` on Windows) is installed executable. This is the offline fixture
contract that future GitHub Release assets should satisfy.

## macOS Local Artifacts

The first real packaging slice generates local macOS SDK artifacts for the host
architecture from a source checkout. The currently validated macOS artifact
labels are `macos-x86_64` and `macos-arm64`:

```bash
python3 tools/package_sdk_release.py --build --output dist/sdk-release
UIMD_HOME=/tmp/uimd-home \
  cpp/build-release/tools/uimd/uimd sdk install 0.4.0 --release-root dist/sdk-release
UIMD_HOME=/tmp/uimd-home-from-script \
UIMD_RELEASE_BASE_URL=file://$PWD/dist/sdk-release \
  sh dist/sdk-release/install.sh --no-shell-config --json
UIMD_HOME=/tmp/uimd-home /tmp/uimd-home/bin/uimd doctor --json
```

The generated `dist/sdk-release` directory is intentionally not committed. It
contains:

```text
dist/sdk-release/
├── 0.4.0/
│   ├── manifest.txt
│   └── payload/
│       ├── bin/uimd
│       ├── targets/python/
│       ├── targets/cpp/
│       └── examples/
├── checksums.txt
├── checksums.txt.minisig
├── install.sh
├── install.ps1
├── uimd-init-0.4.0-<platform>
└── uimd-sdk-0.4.0-<platform>.tar.gz
```

`manifest.txt` is the local installer contract consumed by `uimd sdk install
--release-root`. `install.sh` detects macOS/Linux platform labels and
downloads the matching standalone `uimd-init` asset. `install.ps1` does the
same for Windows. `uimd-init` downloads `checksums.txt` plus the matching SDK
tarball, verifies `checksums.txt.minisig` with minisign before using SHA-256,
extracts the payload, and installs it into the SDK Store. These are
upload-friendly outputs for GitHub Releases; package-manager installer flow
remains separate.

Supported release labels in the packaging/install tooling are
`macos-x86_64`, `macos-arm64`, `linux-x86_64`, `linux-arm64`,
`windows-x86_64`, and `windows-arm64`. The current validated public-install
slice is still macOS Intel until the other platform artifacts are built and
smoke-tested.

`pip install uimd` remains the Python runtime package. It does not create or
repair this store. Package managers should eventually install `uimd-sdk`, whose
only job is to provide `uimd-init`; `uimd-init` then verifies and repairs the
SDK Store and installs the native launcher/binary.

Current implementation status:

- The C++ build produces `cpp/build/tools/uimd/uimd`.
- The C++ build produces `cpp/build/tools/uimd_init/uimd-init`.
- CMake install exports the native `uimd` executable with the runtime targets.
- `uimd-init --no-shell-config --json` can create the local SDK Store,
  install `bin/uimd`, install the matching versioned
  `sdk/<version>/bin/uimd` from `UIMD_INIT_UIMD_BINARY`, create the Python
  target directory, and select that SDK through `current`.
- `uimd-init --check` validates the launcher, selected versioned binary, and
  Python target directory. A normal `uimd-init` run repairs a missing Python
  target instead of treating the store as healthy only because the top-level
  directory exists.
- `uimd-init --modify-shell --json` idempotently adds `UIMD_HOME/bin` or the
  platform default SDK launcher directory to the active user's shell profile
  and reports `shell_profile`, `shell_status`, and `shell_changed` in JSON.
- `uimd sdk home`, `uimd sdk install`, `uimd sdk use`, `uimd sdk list`,
  `uimd sdk install-target`, `uimd sdk remove`, `uimd sdk prune`,
  `uimd doctor --json`, and generation commands are implemented in the native
  binary.
- `uimd sdk install <version> --release-root <path>` installs from a local
  manifest and verifies SHA-256 checksums before copying payload files.
- `tools/package_sdk_release.py` creates the current release root, the
  matching tarball, standalone `uimd-init`, `install.sh`, `install.ps1`, and
  SHA-256 checksums for the selected platform label.
- `uimd sdk prune` removes old local SDK patch versions, keeping the newest two
  patches per minor series and preserving the current SDK selection.
- `uimd sdk update` selects the newest available patch in the current minor
  series from already installed SDKs or GitHub Release checksums. If the
  selected patch is missing locally, it downloads and verifies the matching
  platform tarball by default. `--release-root`, `UIMD_RELEASE_ROOT`, and
  `UIMD_RELEASE_BASE_URL` remain development/CI overrides.
- `uimd self update` updates the selected SDK and SDK Store launcher to the
  newest available patch in the current minor series from GitHub Release
  checksums and the matching platform tarball. `--release-root`,
  `UIMD_RELEASE_ROOT`, and `UIMD_RELEASE_BASE_URL` remain development/CI
  overrides.
- `uimd self uninstall` removes UIMD-owned PATH marker blocks from supported
  shell profiles and then removes the local SDK Store root after safety checks.
  Package-manager uninstall remains a separate user step.
- `uimd sdk list --json` includes installed targets per SDK version, and
  `uimd doctor --json` includes the current SDK target list.
- When the native binary is invoked as `UIMD_HOME/bin/uimd`, it handles `sdk`,
  `self`, and `doctor` locally and delegates other commands with process
  replacement to the selected `sdk/<version>/bin/uimd`. Project commands use
  root `.uimd` `sdk-version` metadata when available and otherwise fall back to
  the latest installed SDK with a reproducibility warning.
- If project metadata requires a missing SDK version and offline mode is not
  enabled, the launcher installs the newest compatible same-minor patch from
  release assets before delegating. This does not change the user's selected
  `current` SDK.
- Before delegation, `generate --target <target>`, `new --target <target>`, and
  `run` verify that the selected SDK has the needed target. Missing `python` or
  `cpp` targets are installed from release assets when offline mode is not
  enabled.
- `UIMD_SDK_PATH` bypasses SDK Store version selection for project commands and
  delegates directly to a configured local SDK binary/path during development.
- `python3 tools/native_uimd_parity.py` smoke-checks the native tool without
  using the removed Python compiler/CLI implementation.

Package-manager recipes are deliberately left for separate release hardening
work. If offline mode is enabled and a required SDK or target is missing, the
launcher reports the install command instead of downloading it.

## Signature Hardening

Release downloads verify two layers:

- `checksums.txt.minisig` is a minisign/Ed25519 signature over
  `checksums.txt`.
- `checksums.txt` contains SHA-256 hashes for the release assets.

The public verification key lives in `signing/uimd-release.pub` and is embedded
into generated `install.sh`, generated `install.ps1`, `uimd-init`, and the
native `uimd` updater. The private signing key must stay outside the repository,
for example on an encrypted USB volume at:

```text
/Volumes/NAME/projects-signing/uimd/uimd-release.key
```

`tools/package_sdk_release.py` discovers that convention automatically on
mounted volumes, so the normal release command stays short:

```bash
python3 tools/package_sdk_release.py --build --output dist/sdk-release
```

Explicit `--signing-key`, `UIMD_RELEASE_SIGNING_KEY`, or a one-line
`~/.config/uimd/release-signing-key` config file can override discovery.
`UIMD_RELEASE_PUBLIC_KEY` and `--public-key` exist only for tests and
controlled fixture generation; production releases use `signing/uimd-release.pub`.
