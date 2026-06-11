# TODO

## Current Open Tasks

Date: 2026-06-05

- [x] **Automatic release signing key discovery**. Remove repetitive release
  signing setup from the normal packaging flow by teaching
  `tools/package_sdk_release.py` to discover the minisign private key from a
  stable encrypted-USB convention such as
  `/Volumes/*/projects-signing/uimd/uimd-release.key`, while still supporting
  explicit `--signing-key` and `UIMD_RELEASE_SIGNING_KEY` overrides. The
  private key must stay outside the repository; only the path discovery logic is
  automated. Implemented explicit/env/config/volume discovery in
  `tools/package_sdk_release.py`; validation passed with a temporary HOME
  config file and no explicit `--signing-key`.
- [x] **Network-backed `uimd sdk update`, SDK auto-install, and uninstall shell
  cleanup**. Complete the remaining SDK manager convenience gaps in the native
  CLI: `uimd sdk update` should use GitHub Release assets by default just like
  `uimd self update`, launcher delegation should auto-install a missing
  required SDK version from release assets before target checks, while retaining
  `--release-root`/environment overrides for tests and CI; `uimd self
  uninstall` should remove UIMD-owned PATH marker blocks from supported shell
  profiles instead of leaving manual cleanup.
  Parity decision: this is native SDK-facing installer/CLI behavior in
  `cpp/tools/uimd` and native smoke coverage; no Python compiler/CLI
  implementation is involved. Implemented in `cpp/tools/uimd/main.cpp` with
  default release-asset downloads for `uimd sdk update`, missing required SDK
  auto-install before launcher delegation, and marker-block shell cleanup for
  `uimd self uninstall`; smoke coverage in `tools/native_uimd_parity.py`
  exercises these paths through `UIMD_RELEASE_BASE_URL=file://...` and a
  temporary shell profile. Validation passed: `cmake --build cpp/build --target
  uimd uimd_init` and `python3 tools/native_uimd_parity.py`.
- [x] **Network-backed default SDK update flow**. Make ordinary user commands
  work without `UIMD_RELEASE_ROOT`: `uimd self update` should discover the
  latest same-minor SDK patch from GitHub Release checksums, download and
  verify the matching platform SDK tarball, install it, and refresh the
  launcher; missing target auto-install should similarly download the selected
  SDK version by default unless offline mode is enabled. `UIMD_RELEASE_ROOT`
  and `UIMD_RELEASE_BASE_URL` should remain test/CI/development overrides, not
  the normal user path. Implemented in `cpp/tools/uimd/main.cpp` with release
  checksum parsing, platform archive download/extract/install, default
  `uimd self update`, and default target auto-install. Smoke coverage in
  `tools/native_uimd_parity.py` uses `UIMD_RELEASE_BASE_URL=file://...` to
  exercise the network-style code path without requiring internet access.
  Validation passed: `cmake --build cpp/build --target uimd uimd_init` and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK self update and target auto-install slice**. Implement the next
  practical SDK manager layer in the native CLI: `uimd self update` should
  update the SDK Store launcher/current SDK from an installed newer patch or
  a local release root, project commands should auto-install missing supported
  targets from the current SDK release manifest when not offline. Parity
  decision: this is native SDK-facing installer/CLI behavior under
  `cpp/tools/uimd` and native smoke tests only; no Python compiler/CLI
  implementation is involved. Implemented with `uimd self update
  [--release-root <path>] [--json]`, target auto-install before launcher
  delegation for `generate --target`, `new --target`, and `run`, and smoke
  coverage in `tools/native_uimd_parity.py`. Validation passed:
  `cmake --build cpp/build --target uimd uimd_init` and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK install MVP hardening slice**. Complete the practical installer
  layer for comfortable UIMD installation before package-manager recipes:
  `uimd-init` must validate and repair the installed Python target, release
  download platform detection must cover macOS/Linux/Windows labels instead of
  only macOS Intel, release packaging must emit the matching macOS/Linux
  `install.sh` and Windows `install.ps1` bootstrap assets with SHA-256
  verification, and docs must describe the versioned GitHub Release install
  commands plus `--modify-shell` / `--no-shell-config` behavior. Parity
  decision: this is native SDK-facing installer/CLI behavior under
  `cpp/tools/uimd_init`, `cpp/tools/uimd`, and release tooling only; no Python
  compiler/CLI implementation is involved. Implemented in
  `cpp/tools/uimd_init/main.cpp`, `cpp/tools/uimd/main.cpp`,
  `tools/package_sdk_release.py`, `tools/native_uimd_parity.py`, `README.md`,
  `docs/installation.md`, and `docs/sdk-store.md`. Validation passed:
  `python3 -m py_compile tools/package_sdk_release.py
  tools/native_uimd_parity.py`, `cmake --build cpp/build --target uimd
  uimd_init`, `python3 tools/native_uimd_parity.py`,
  `python3 tools/package_sdk_release.py --build --output dist/sdk-release`,
  local `install.sh` smoke with `UIMD_RELEASE_BASE_URL=file://...` and
  `--no-shell-config --json`, installed launcher `doctor --json`, and plain
  `uimd-init --no-shell-config` next-steps output.
- [ ] **Windows validation**: verify the new `image_button` control and the
  updated `image_browser` build and run on Windows for both Python and C++,
  confirming padding, centering, square sizing, click selection, and render-mode
  switching behave identically to macOS/Linux.

## MCP Tester

- [ ] Add a performance benchmark/debug mode for C++ tester versus Python tester
  on representative compare viewports.
- [ ] Re-check compare/buffer-mode CPU cost after the C++ tester work and reduce
  remaining background repaint or snapshot polling overhead if it is still
  measurable.
- [ ] Improve failed-script summaries so the all-scripts summary includes the
  failing step number, target/platform, tool/assertion name, and first failure
  message.

## UIMD Admin

- [ ] After `ViewHost` exists, split UIMD admin top-level sections into separate
  UIMD views/layouts and host them inside the relevant shell/main panel instead
  of keeping them in one shared layout.

## Repository And Publishing

- [x] **Prepare 0.4.0 main release candidate from sdk-work**. Before pushing
  `sdk-work` to `main`, bump the release line from the already-tagged `v0.3.2`
  main snapshot to `0.4.0`, keep public install docs aligned with the new
  version, build signed macOS arm64 release assets from `sdk-work`, smoke-test
  the installer flow from the generated assets, and record the validation
  commands before any `main` push. Progress: version surfaces were bumped with
  `python3 tools/set_version.py 0.4.0`, public install docs were updated to
  `v0.4.0`, and validation passed: `python3 tools/set_version.py 0.4.0
  --check`, `python3 -m py_compile tools/package_sdk_release.py
  tools/native_uimd_parity.py`, `git diff --check`, `cmake --build cpp/build
  --target uimd uimd_init`, and `python3 tools/native_uimd_parity.py`.
  Signed release asset packaging found the production key at
  `/Volumes/DUBOVSKY/projects-signing/uimd/uimd-release.key`; after the user
  entered the private-key password interactively, `dist/sdk-release` contained
  `install.sh`, `install.ps1`, `checksums.txt`, `checksums.txt.minisig`,
  `uimd-init-0.4.0-macos-arm64`, and
  `uimd-sdk-0.4.0-macos-arm64.tar.gz`. Installer smoke passed:
  `UIMD_HOME=/private/tmp/uimd-040-smoke
  UIMD_RELEASE_BASE_URL=file:///Users/marekdubovsky/Projects/uimd/dist/sdk-release
  sh dist/sdk-release/install.sh --no-shell-config --json`,
  `/private/tmp/uimd-040-smoke/bin/uimd doctor --json`, external-project
  `uimd new hello`, `uimd generate hello.uimd --target python`, `uimd generate
  hello.uimd --target cpp`, Python `py_compile`, and C++ configure/build
  against installed `targets/cpp`. `file` confirmed arm64 Mach-O binaries for
  the installed launcher and versioned SDK binary.
- [x] **Public install command and PATH UX cleanup**. Update the public README,
  release notes, and install docs for the real `v0.4.0` GitHub Release install
  flow. Document the safe default command
  `curl -fsSL https://github.com/uimd-lang/uimd/releases/download/v0.4.0/install.sh | sh`,
  explain that it installs into `~/.uimd` but does not modify `PATH`, and show
  both immediate usage via `~/.uimd/bin/uimd` and human-friendly setup via
  `sh -s -- --modify-shell` followed by a new shell or `source ~/.zshrc`.
  Decide whether `install.sh`/`uimd-init` should print these next steps when
  `shell config: unchanged`. Do not advertise
  `releases/latest/download/install.sh` as the primary command until the
  prerelease/latest policy is verified; keep the versioned URL canonical for
  now. Implemented in `README.md`, `docs/installation.md`, `docs/sdk-store.md`,
  `CHANGELOG.md`, and `uimd-init` plain output.
- [x] Validate native `uimd` release artifacts on macOS arm64 separately from
  the Windows/Linux platform migration work. Implemented macOS arm64 release
  artifact support in the native bootstrapper (`cpp/tools/uimd_init/main.cpp`)
  and local packaging script (`tools/package_sdk_release.py`) while preserving
  the existing `macos-x86_64` artifact path. Parity decision: this is native
  SDK packaging/bootstrap behavior only; Python runtime behavior is unaffected,
  and generated Python/C++ outputs are both validated from the installed arm64
  SDK Store. Validation passed on Apple Silicon (`uname -m` = `arm64`):
  `python3 tools/package_sdk_release.py --build --build-dir
  cpp/build-release-arm64 --output dist/sdk-release-arm64`,
  `env UIMD_HOME=/private/tmp/uimd-arm64-release-manifest-20260611
  cpp/build-release-arm64/tools/uimd/uimd sdk install 0.3.2 --release-root
  dist/sdk-release-arm64`,
  `env UIMD_HOME=/private/tmp/uimd-arm64-release-script-20260611
  UIMD_RELEASE_BASE_URL=file:///Users/marekdubovsky/Projects/uimd/dist/sdk-release-arm64
  sh dist/sdk-release-arm64/install.sh --no-shell-config --json`,
  `env UIMD_HOME=/private/tmp/uimd-arm64-release-script-20260611
  /private/tmp/uimd-arm64-release-script-20260611/bin/uimd doctor --json`,
  external-project `uimd new hello`, `uimd generate hello.uimd --target
  python`, `uimd generate hello.uimd --target cpp`, Python `py_compile`,
  C++ configure/build against installed `targets/cpp`, `python3
  tools/native_uimd_parity.py --native-binary
  cpp/build-release-arm64/tools/uimd/uimd --native-init-binary
  cpp/build-release-arm64/tools/uimd_init/uimd-init`, and the same parity smoke
  with `--compile-examples`. `file` confirmed arm64 Mach-O binaries for
  packaged `uimd-init`, installed launcher, versioned SDK `bin/uimd`, and the
  generated external C++ hello app.
- [ ] Validate native `uimd` release artifacts as part of the Windows/Linux
  platform migration work, covering Linux x86_64, Linux arm64, Windows x86_64,
  and Windows arm64 where toolchains are available. This should include package
  install/bootstrap behavior, C++-only usage without Python installed, native
  `generate`, `doctor`, `sdk`, `inspect`, and MCP/rebuild smoke checks.
- [x] **Release signature hardening**. Pick a production signature scheme and
  implement signed release verification beyond SHA-256 checksums. Required
  decisions: signature format (`minisign`/`signify`, Ed25519, or another
  portable verifier), offline private-key custody, embedded public verification
  key distribution for `install.sh`, `install.ps1`, `uimd-init`, and
  `uimd self update`, `checksums.txt.minisig` publication, required-vs-optional
  verification policy during alpha, key rotation, and emergency revocation.
  Implemented the current alpha policy with minisign/Ed25519 signatures:
  `tools/package_sdk_release.py` requires `--signing-key` or
  `UIMD_RELEASE_SIGNING_KEY`, emits `checksums.txt.minisig`, embeds
  `signing/uimd-release.pub` into generated `install.sh`/`install.ps1`, and
  native `uimd`/`uimd-init` verify the signed checksum file before SHA-256
  asset checks. Test-only public-key overrides are available for fixture
  generation. Validation passed: `cmake --build cpp/build --target uimd
  uimd_init`, `python3 tools/native_uimd_parity.py`, signed
  `tools/package_sdk_release.py` smoke with a temporary minisign key, and local
  signed `install.sh --no-shell-config --json` smoke.
- [ ] Design and implement the long-term UIMD SDK Store/Launcher installation
  model before public packaging hardens.

  **SDK Store layout**: per-user root at `~/.uimd` on macOS/Linux and
  `%LOCALAPPDATA%\uimd` on Windows, overridable via `UIMD_HOME`.
  ```
  ~/.uimd/
  ├── bin/uimd                        ← real launcher (owned by UIMD only)
  └── sdk/
      └── 0.5.0/
          ├── bin/uimd                ← compiler/generator for this version
          ├── targets/
          │   ├── python/             ← Python runtime
          │   ├── cpp/                ← C++ headers, static lib, CMake config
          │   └── csharp/             ← C# support (future)
          └── examples/
              ├── shared/             ← .uimd source files (language-agnostic)
              ├── python/             ← generated Python examples
              └── cpp/                ← generated C++ examples
  ```
  Examples are part of the SDK so users and agents can use them as reference
  and inspiration. Each SDK version ships its own examples; new features in a
  new version bring updated examples with them.
  Only the UIMD SDK manager may write into `~/.uimd/`. Package managers and
  pip never write into the SDK Store.
  SDK binaries and `uimd-init` are distributed via GitHub Releases at
  `https://github.com/uimd-lang/uimd` (primary source). `uimd-lang` is the
  GitHub organization name (following the rust-lang/golang convention); the
  tool and package are named `uimd` and `uimd-sdk` respectively. See the
  separate GitHub and publishing tasks below for the full publication checklist.

  **Delegation chain**: every entry point delegates via `exec()` (not subprocess
  spawn) to avoid latency stacking.
  ```
  /opt/homebrew/bin/uimd-init   ← installer binary (owned by brew)
  ~/.local/bin/uimd-init        ← installer binary (owned by pip)
        ↓ runs once, then exits
  ~/.uimd/bin/uimd              ← real launcher (owned by UIMD)
        ↓  exec()
  ~/.uimd/sdk/0.5.0/bin/uimd   ← versioned compiler for this project
  ```
  Package managers install only `uimd-init` — a small one-time installer binary
  whose version is the installer version, independent of any SDK version. The
  installer is rarely updated (only if the bootstrap mechanism changes).
  `uimd-init` is modelled after `rustup-init`: run once by the user, sets up
  `~/.uimd/`, optionally adds `~/.uimd/bin/` to shell PATH (`~/.zshrc`,
  `~/.bashrc`, PowerShell profile), then exits. Shell config changes must be
  explicit: use `uimd-init --modify-shell` for human installs that should edit
  shell startup files, and `uimd-init --no-shell-config` for agents/CI or users
  who want to manage PATH themselves. After setup the user works with `uimd`
  exclusively. `brew upgrade uimd-sdk` updates only the `uimd-init` binary;
  `~/.uimd/bin/uimd` is updated exclusively via `uimd self update`.
  `uimd-init` always verifies the installation state, not just the existence of
  `~/.uimd/` — it checks launcher binary integrity and Python SDK presence, and
  repairs or re-downloads anything missing or corrupted. If everything is valid
  it exits immediately as a no-op.
  `uimd-init` prints setup instructions itself when `~/.uimd/` is missing or
  incomplete; no pip or brew post-install hook is used for messaging.
  `uimd-init` must support fully non-interactive agent/CI operation. Agents run
  `uimd-init --no-shell-config`, skip shell startup file edits, and then call
  `~/.uimd/bin/uimd` directly by full path. `uimd doctor --json` can be used to
  check installation state before deciding whether to run `uimd-init`.

  **Default target**: every `uimd-init` run always installs the Python target
  (`targets/python/`) because Python is the default compilation target and
  `uimd compile` without a language flag compiles Python. Other targets (C++,
  C#, etc.) are installed on demand when first used: `uimd compile --cpp`
  triggers a silent auto-install of `targets/cpp/` if not present. This
  guarantees agents and users never wait for a download on the default Python
  path; a download only occurs the first time a non-default target is requested,
  which is acceptable.

  **Version resolution**: the required SDK version is declared once in the
  project's root/top-level `.uimd` file, not in every component `.uimd` file.
  The launcher finds the root `.uimd`, reads the SDK version from its `##
  Metadata`, and selects the highest installed SDK version that satisfies
  `>=required_version`. Component `.uimd` files inherit the root SDK version.
  If the satisfying version crosses a minor-series boundary (e.g. root requires
  0.5.0 but only 0.6.x is installed), the launcher emits a warning and runs
  with the available version; update the root `.uimd` Metadata to silence it. If
  no satisfying version is installed, the launcher auto-installs the latest
  patch of the required minor series unless offline mode disables downloads.
  Backward compatibility within a minor series is guaranteed. The SDK version
  in the root `.uimd` Metadata is recommended, but not mandatory. New projects
  created by `uimd new` should include it. If a root `.uimd` file has no SDK
  version in Metadata, the launcher uses the latest installed SDK version as a
  legacy/quick-start fallback and emits a reproducibility warning such as:
  "root .uimd has no UIMD SDK version; using installed SDK 0.6.2; add
  sdk-version to Metadata for reproducible builds." Strict contexts may opt in
  to failure with `--require-sdk-version` or `UIMD_REQUIRE_SDK_VERSION=1`.
  Feature/element heuristics may be used only for diagnostics such as "this
  source uses Image and requires at least 0.5.0"; heuristics must not be the
  primary SDK version selection mechanism. `uimd.toml` is optional and used only
  for project-level overrides (e.g. `UIMD_SDK_PATH`); it does not declare the
  SDK version. `uimd init` creates a `uimd.toml` template with commented-out
  override options.

  **Auto-install**: all downloads and installs happen automatically and silently
  with no confirmation prompt, for both interactive users and agents. A download
  mid-task (e.g. first C++ compile) is acceptable; the command completes after
  the download finishes. Set `UIMD_NO_AUTO_INSTALL=1` or pass `--offline` to
  disable all network access, SDK downloads, target downloads, and repair
  downloads for that command; if required SDK/target files are missing, the
  command must fail fast with a clear error and exact install/ensure command.

  **Python package split**: `pip install uimd` installs the Python UIMD runtime
  package only. It is for users running Python UIMD applications that report
  missing UIMD Python libraries, and it must not install, bootstrap, or manage
  the SDK. `pip install uimd-sdk` is a separate developer/agent bootstrap
  package that installs `uimd-init` only.

  **Bootstrap entry points**:
  - `brew install uimd-sdk` — installs `uimd-init`; prints post-install message
    "To complete installation run: uimd-init"; `brew upgrade uimd-sdk` updates
    `uimd-init` only
  - `pip install uimd-sdk` — installs `uimd-init`; prints the same post-install
    message; skips SDK download if `~/.uimd/` already exists
  - `winget install uimd-sdk` / `apt install uimd-sdk` — same model
  Every package manager prints the same post-install message so both users and
  agents know to run `uimd-init` (or `uimd-init --no-shell-config` for agents).
  Since `uimd-init` is a no-op when `~/.uimd/` already exists, agents may
  always call it unconditionally after any package manager install.

  **SDK manager commands**: `uimd sdk install <version>`,
  `uimd sdk install-target <target>` (e.g. `cpp`, `csharp`),
  `uimd sdk list`, `uimd sdk remove <version>`, `uimd sdk use <version>`,
  `uimd sdk update` (latest patch of current minor series),
  `uimd sdk prune` (keep last 2 patches per minor series, remove the rest).
  Launcher: `uimd self update`, `uimd self uninstall`.
  Diagnostics: `uimd doctor --json`, `uimd sdk list --json` (for agents/CI).
  Project init: `uimd init` creates `uimd.toml` template with commented-out
  project-level override options.

  **Development override**: `UIMD_SDK_PATH=<path>` bypasses the SDK Store and
  points the launcher at a local SDK build; required when developing UIMD itself.

  **Uninstall**: `brew uninstall uimd-sdk` removes only the brew shim; `~/.uimd/`
  is untouched. To fully remove UIMD run `uimd self uninstall` first, then
  uninstall via the package manager.

  **Platform packaging**: generated C++ apps prefer static linking so end users
  need no UIMD runtime installed. C#/other compiled targets are similarly bundled
  into the built app. Python apps import the runtime from the SDK Store via
  PYTHONPATH injection by the launcher; no pip dependency is needed at app
  runtime.

  Remaining implementation gaps: package-manager recipes and cross-platform
  validation.
- [x] Design GitHub Release based bootstrap scripts for UIMD SDK installation as
  a separate packaging task. Initial bootstrap commands should not require a
  custom UIMD domain and can use release assets directly, for example
  `curl -sSf https://github.com/uimd-lang/uimd/releases/latest/download/install.sh | sh`
  on macOS/Linux and
  `iwr https://github.com/uimd-lang/uimd/releases/latest/download/install.ps1 | iex`
  on Windows. The scripts must download and run `uimd-init`, verify
  checksums/signatures for downloaded binaries, avoid raw branch URLs such as
  `raw.githubusercontent.com/.../main/...`, and remain compatible with a future
  friendly redirect such as `https://install.uimd.dev` if a domain/server is
  later added. Implemented as generated versioned-release `install.sh` and
  `install.ps1` assets with minisign verification of `checksums.txt.minisig`
  before SHA-256 verification through `checksums.txt`.
- [ ] Verify that the PyPI package name `uimd` is available before the first
  public release.
- [ ] Confirm GitHub repository description is set to:
  `User Interface Markdown. Define app interfaces in Markdown and generate UI
  and agent access.`
- [ ] Confirm GitHub repository topics are set:
  `markdown`, `ui`, `mcp`, `code-generation`, `terminal-ui`, `python`, `cpp`.
- [ ] Decide how the current SVN source of truth maps to GitHub publication.
- [ ] Configure PyPI Trusted Publisher for repository `uimd-lang/uimd` once the
  package is ready to publish.

## Layout Migration

- [ ] Retire or migrate the existing root `python/` layout after package files,
  tests, and examples have moved to their final locations.
- [ ] Move Python tests into the public test layout, keeping them able to test
  the installed `uimd` package rather than repository-local imports.
- [ ] Move Python examples into the public examples layout or document why
  backend-specific examples remain under their backend directory.
- [ ] Finalize the public repository layout:
  `pyproject.toml`, `README.md`, `LICENSE`, `src/uimd/`, `cpp/`, `shared/`,
  `docs/`, `examples/`, `tests/`, and `tools/`.

## Validation

- [ ] Verify clean install into a fresh virtualenv on Windows.
- [ ] Verify `python3 -m pip install git+https://github.com/uimd-lang/uimd.git`
  once the GitHub repository is populated.
- [ ] Verify `uimd run hello.uimd` from a clean external project.
- [ ] Verify C++ hello-world flow from a clean external project after the first
  public GitHub release/tag, without relying on a sibling `../uimd/cpp`
  checkout.
