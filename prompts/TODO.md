# TODO

## Current Open Tasks

Date: 2026-06-05

- [x] **Priority next / restart here: SDK Store/Launcher local installation
  slice**. Implement the local/offline foundation before platform validation or
  release downloads: `UIMD_HOME`/default store layout, versioned
  `sdk/<version>/bin/uimd`, local launcher delegation via `exec()`, basic
  `uimd sdk list/install/use/remove`, and `uimd doctor --json` diagnostics
  against a temporary SDK Store in tests. The detailed product model remains in
  the Repository And Publishing SDK Store task below. Implemented in the native
  C++ SDK-facing CLI only (`cpp/tools/uimd/main.cpp`) with matching
  bootstrapper layout creation in `cpp/tools/uimd_init/main.cpp`; no Python
  compiler/CLI oracle was added. `tools/native_uimd_parity.py` now validates
  install/use/remove, `doctor --json`, `uimd-init` layout creation, and POSIX
  launcher delegation against a temporary `UIMD_HOME`. Validation passed:
  `cmake --build cpp/build --target uimd`,
  `cmake --build cpp/build --target uimd_init`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK version selection and offline policy slice**. Implement the next
  local SDK Store/Launcher layer in the native C++ CLI: read root `.uimd`
  `sdk-version` metadata, choose the highest installed compatible SDK version,
  warn for missing root SDK metadata unless strict mode is enabled, support
  `--require-sdk-version`/`UIMD_REQUIRE_SDK_VERSION=1`, support
  `--offline`/`UIMD_NO_AUTO_INSTALL=1` fail-fast behavior when the required SDK
  is not installed, and validate the launcher behavior against temporary
  `UIMD_HOME` stores in `tools/native_uimd_parity.py`. Parity decision:
  compiler/launcher behavior is native `cpp/tools/uimd` only; no Python
  compiler/CLI implementation should be added. Implemented in
  `cpp/tools/uimd/main.cpp`; `tools/native_uimd_parity.py` now validates
  `sdk-version` selection, cross-minor warnings, legacy fallback warnings,
  strict missing-metadata failure, and offline missing-SDK failure. Validation
  passed: `cmake --build cpp/build --target uimd`,
  `cmake --build cpp/build --target uimd_init`,
  `python3 -m py_compile tools/native_uimd_parity.py`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK target installation slice**. Implement local/offline
  `uimd sdk install-target <target>` support in the native C++ CLI for
  versioned SDK targets, starting with `python` and `cpp` target directories,
  JSON/plain diagnostics that show installed targets, `doctor --json` target
  status, and temporary `UIMD_HOME` smoke coverage. Release artifact download
  and real target payload population remain a later packaging task; this slice
  should fail fast for unsupported or missing-version target installs rather
  than adding network behavior. Implemented in `cpp/tools/uimd/main.cpp` with
  `uimd sdk install-target <target> [--version <version>]`, per-version target
  lists in `uimd sdk list --json`, and current target diagnostics in
  `uimd doctor --json`. `tools/native_uimd_parity.py` validates `cpp` target
  installation, unsupported target failure, JSON target maps, and doctor target
  status against a temporary `UIMD_HOME`. Validation passed:
  `cmake --build cpp/build --target uimd`,
  `cmake --build cpp/build --target uimd_init`,
  `python3 -m py_compile tools/native_uimd_parity.py`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK local release manifest slice**. Implement the GitHub-independent
  release artifact contract and local installer path before network downloads:
  line-oriented manifest fixtures under a local release root, SHA-256
  verification for every manifest file, `uimd sdk install <version>
  --release-root <path>` support, payload copy into `sdk/<version>/...`, and
  native smoke coverage for successful install plus checksum mismatch failure.
  This slice must not add GitHub/network access; it prepares the exact artifact
  shape that future GitHub Releases will publish. Implemented in
  `cpp/tools/uimd/main.cpp` with local manifests at
  `<release-root>/<version>/manifest.txt` or
  `<release-root>/uimd-sdk-<version>.manifest`, safe relative path validation,
  SHA-256 verification, executable install for `bin/uimd`, payload copy for
  target files, and `UIMD_RELEASE_ROOT`/`--release-root` support. Smoke coverage
  in `tools/native_uimd_parity.py` validates successful manifest install and
  checksum mismatch failure. Validation passed:
  `cmake --build cpp/build --target uimd`,
  `python3 -m py_compile tools/native_uimd_parity.py`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **uimd-init shell profile mutation slice**. Implement the GitHub-free
  bootstrapper shell configuration step: `uimd-init --modify-shell` should
  idempotently add the SDK launcher directory to the appropriate shell profile
  under the active user home, `--no-shell-config` must leave profiles untouched,
  JSON output should report the selected shell action, and native smoke tests
  must validate the behavior against a temporary `HOME`/`USERPROFILE`.
  Implemented in `cpp/tools/uimd_init/main.cpp` with POSIX zsh/bash/profile,
  Fish, and Windows PowerShell profile selection; quoted PATH mutation;
  `shell_profile`, `shell_status`, and `shell_changed` JSON diagnostics; and
  no mutation during `--check` or `--no-shell-config`. Smoke coverage in
  `tools/native_uimd_parity.py` validates `--no-shell-config` profile
  isolation and repeated `--modify-shell` idempotence against temporary homes.
  Validation passed: `cmake --build cpp/build --target uimd`,
  `cmake --build cpp/build --target uimd_init`,
  `python3 -m py_compile tools/native_uimd_parity.py`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK prune local retention slice**. Implement the GitHub-free
  `uimd sdk prune` manager command in the native C++ SDK CLI. It should keep
  the newest two patch versions per minor series, preserve the current SDK
  version even if it would otherwise be pruned, remove only safe version
  directories under the local SDK Store, support plain and JSON diagnostics,
  and add temporary `UIMD_HOME` smoke coverage. Implemented in
  `cpp/tools/uimd/main.cpp` with safe per-version removal, JSON diagnostics,
  current-version preservation, and retention by minor series. Smoke coverage
  in `tools/native_uimd_parity.py` validates pruning an older patch while
  keeping the selected older current SDK plus the newest two patches.
  Validation passed: `cmake --build cpp/build --target uimd`,
  `cmake --build cpp/build --target uimd_init`,
  `python3 -m py_compile tools/native_uimd_parity.py`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK self uninstall local slice**. Implement the GitHub-free
  `uimd self uninstall` launcher command in the native C++ SDK CLI. It should
  run locally from `UIMD_HOME/bin/uimd`, remove only the selected SDK Store
  root after safety checks, support JSON diagnostics, leave package-manager
  uninstall as a separate user step, and validate the behavior against a
  temporary `UIMD_HOME` smoke fixture. Implemented in `cpp/tools/uimd/main.cpp`
  as a launcher-handled top-level `self uninstall` command with safe SDK Store
  root checks, absent-store idempotence, and JSON/plain diagnostics. Smoke
  coverage in `tools/native_uimd_parity.py` installs a temporary SDK Store,
  invokes `UIMD_HOME/bin/uimd self uninstall --json`, and verifies that the
  store is removed. Validation passed:
  `cmake --build cpp/build --target uimd`,
  `cmake --build cpp/build --target uimd_init`,
  `python3 -m py_compile tools/native_uimd_parity.py`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK update local release-root slice**. Implement the GitHub-free
  `uimd sdk update` manager command in the native C++ SDK CLI for local release
  fixtures. It should update the current SDK to the newest available patch in
  the same minor series from installed versions and/or `--release-root` /
  `UIMD_RELEASE_ROOT`, keep network downloads unimplemented, support JSON
  diagnostics, and validate the behavior against temporary release manifests.
  Implemented in `cpp/tools/uimd/main.cpp` with local release manifest
  discovery for `<release-root>/<version>/manifest.txt` and
  `<release-root>/uimd-sdk-<version>.manifest`, same-minor latest-patch
  selection, installed-only update selection, release-root installation, and
  JSON/plain diagnostics. Smoke coverage in `tools/native_uimd_parity.py`
  validates updating from `3.4.0` to local release `3.4.2` while ignoring
  `3.5.0`, then selecting already installed `3.4.3` without a release root.
  Validation passed: `cmake --build cpp/build --target uimd`,
  `cmake --build cpp/build --target uimd_init`,
  `python3 -m py_compile tools/native_uimd_parity.py`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **SDK development override slice**. Implement the GitHub-free
  `UIMD_SDK_PATH` launcher override in the native C++ SDK CLI. When set, the
  installed launcher should bypass SDK Store version resolution for project
  commands and execute the configured local SDK binary/path directly, while
  still handling `sdk`, `self`, and `doctor` locally. Add temporary launcher
  smoke coverage. Implemented in `cpp/tools/uimd/main.cpp` with direct binary,
  SDK root `bin/uimd`, source-checkout native build, and source-checkout wrapper
  resolution, plus recursion protection when the override points back to the
  launcher. Smoke coverage in `tools/native_uimd_parity.py` validates project
  command delegation to the override binary and local handling of `sdk list`.
  Validation passed: `cmake --build cpp/build --target uimd`,
  `cmake --build cpp/build --target uimd_init`,
  `python3 -m py_compile tools/native_uimd_parity.py`, and
  `python3 tools/native_uimd_parity.py`.
- [x] **Minimal GitHub Release SDK download slice**. Implement the smallest
  public-install path on top of the local macOS Intel release artifact work:
  generate a versioned `install.sh` release asset, let it download and verify
  `uimd-init-<version>-macos-x86_64` from a GitHub Release asset set, and teach
  `uimd-init` to download `checksums.txt` plus
  `uimd-sdk-<version>-macos-x86_64.tar.gz`, verify SHA-256, extract the SDK
  payload, install it into `UIMD_HOME`/default SDK Store, write the launcher and
  current version, and keep the existing `--modify-shell` / `--no-shell-config`
  behavior. Keep this slice platform-scoped to macOS Intel; Homebrew, PyPI
  `uimd-sdk`, Windows install script, signatures, latest-version discovery,
  and package-manager recipes remain later tasks. Implemented in
  `cpp/tools/uimd_init/main.cpp` with `curl`, `tar`, and SHA-256 verification
  via `shasum` for the current macOS Intel slice; `tools/package_sdk_release.py`
  now emits `install.sh` and includes it in `checksums.txt`. Validation passed:
  `cmake --build cpp/build --target uimd_init`,
  `python3 tools/package_sdk_release.py --build --output dist/sdk-release`,
  `UIMD_HOME=/private/tmp/uimd-ghdownload-smoke-20260610-2
  UIMD_RELEASE_BASE_URL=file:///Users/marekdubovsky/Projects/uimd/dist/sdk-release
  sh dist/sdk-release/install.sh --no-shell-config --json`,
  `UIMD_HOME=/private/tmp/uimd-ghdownload-smoke-20260610-2
  /private/tmp/uimd-ghdownload-smoke-20260610-2/bin/uimd doctor --json`, and
  `python3 tools/native_uimd_parity.py`.
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

- [x] **GitHub repository bootstrap**. The public repository
  `uimd-lang/uimd` has been bootstrapped from the preserved SVN checkout at
  `/Users/marekdubovsky/Projects/uimd-svn` into the separate Git working copy
  at `/Users/marekdubovsky/Projects/uimd`. The initial import excludes `.svn`,
  `.DS_Store`, build/cache output, MCP temporary snapshots, `.opencode`
  dependencies, and local Claude settings via the root `.gitignore`. The stable
  public `main` branch and the working `sdk-work` branch were both pushed to
  `https://github.com/uimd-lang/uimd.git` at initial import commit `7f2419d`;
  future SDK/publishing checkpoints should continue on `sdk-work`, not `main`.
  Validation passed: `find . -name .svn -type d -o -name .DS_Store -type f`
  returned no files, `git status --short --branch` was clean, and
  `git ls-remote --heads origin` showed `main` and `sdk-work`.
- [x] **README alpha install status correction**. Update the public README to
  avoid advertising unreleased PyPI, Homebrew, GitHub Release, or packaged SDK
  install flows. The current README now describes the project as alpha,
  documents macOS Intel (`x86_64`) as the only validated platform for now,
  points users to the source-checkout native build (`cmake -S cpp -B
  cpp/build`, `cmake --build cpp/build --target uimd`, `./uimd doctor`), and
  records that macOS Apple Silicon and Windows x64 support are planned later.
- [x] **macOS Intel local release artifact packaging slice**. Add
  `tools/package_sdk_release.py` to generate a local `dist/sdk-release` root
  for `macos-x86_64` from `cpp/build-release` with
  `-DUIMD_EMBED_SOURCE_ROOT=OFF`, `manifest.txt`, payload files, standalone
  `uimd-init-<version>-macos-x86_64`, `uimd-sdk-<version>-macos-x86_64.tar.gz`,
  and SHA-256 `checksums.txt`. The payload installs through the existing
  `uimd sdk install <version> --release-root <path>` manifest path and includes
  `bin/uimd`, `targets/python`, `targets/cpp`, and examples. The native
  launcher also recognizes installed `sdk/<version>/targets/python` as a
  Python runtime path when project commands are executed from a versioned SDK
  binary. This is a local maintainer/development artifact only; public GitHub
  Release downloads, signatures, package-manager recipes, and cross-platform
  release validation remain separate tasks.
- [ ] Validate native `uimd` release artifacts on macOS arm64 separately from
  the Windows/Linux platform migration work. The local macOS Intel/x86_64
  source-checkout flow is already validated; this task must prove the native
  binary builds, installs, generates Python/C++ outputs, runs C++-only flows
  without Python compiler fallback, and passes the relevant MCP/rebuild smoke
  checks on Apple Silicon.
- [ ] Validate native `uimd` release artifacts as part of the Windows/Linux
  platform migration work, covering Linux x86_64, Linux arm64, Windows x86_64,
  and Windows arm64 where toolchains are available. This should include package
  install/bootstrap behavior, C++-only usage without Python installed, native
  `generate`, `doctor`, `sdk`, `inspect`, and MCP/rebuild smoke checks.
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

  Current implementation work:
  - Native `uimd-init` builds as `cpp/build/tools/uimd_init/uimd-init`,
    supports `--check`, `--json`, `--no-shell-config`, and `--modify-shell`,
    creates the `UIMD_HOME`/default SDK Store skeleton, installs `bin/uimd`,
    installs versioned `sdk/<version>/bin/uimd`, creates the Python target
    directory, and selects that version from `UIMD_INIT_UIMD_BINARY`.
  - Native `uimd sdk home/install/use/list/list --json/remove` plus
    `uimd doctor --json` expose SDK Store state for agents and CI.
  - The local launcher at `UIMD_HOME/bin/uimd` handles `sdk`, `self`, and
    `doctor` locally and delegates other commands via process replacement to
    the selected versioned SDK binary.
  - The launcher reads root `.uimd` `sdk-version` metadata for project
    commands, selects the highest installed compatible SDK, supports strict
    missing-metadata mode, supports offline fail-fast for missing required SDKs,
    and warns when it falls back to a legacy project without `sdk-version`.
  - Native `uimd sdk install-target <target> [--version <version>]` creates
    local `targets/python` and `targets/cpp` directories for installed SDK
    versions, and `sdk list --json` plus `doctor --json` expose installed
    target state.
  - Native `uimd sdk install <version> --release-root <path>` installs SDK
    files from a local release manifest after SHA-256 verification, providing
    the offline fixture contract for future GitHub Release assets.
  - Native `uimd-init --modify-shell` idempotently adds the SDK launcher
    directory to the active user's shell profile, while `--no-shell-config`
    leaves shell startup files untouched.
  - Native `uimd sdk prune` removes older local SDK patches while preserving
    the current SDK selection and the newest two patches per minor series.
  - Native `uimd sdk update` selects the newest available patch in the current
    minor series from installed SDKs and/or a local release-root manifest
    fixture; network downloads remain unimplemented.
  - `UIMD_SDK_PATH` bypasses SDK Store version resolution for project commands
    and delegates directly to a configured local SDK binary/path while leaving
    `sdk`, `self`, and `doctor` local to the launcher.
  - Native `uimd self uninstall` removes the local SDK Store after safety
    checks; package-manager uninstall and shell profile cleanup remain separate
    user steps.
  - Remaining packaging gap: release asset downloads, checksum/signature
    verification against network-fetched assets, target auto-install,
    network-backed self-update, and package-manager recipes require real
    release artifacts plus platform-specific validation. Uninstall-time shell
    profile cleanup remains a separate cross-tool/platform cleanup so it can
    share the `uimd-init` shell-profile logic instead of duplicating it.
- [ ] Design GitHub Release based bootstrap scripts for UIMD SDK installation as
  a separate packaging task. Initial bootstrap commands should not require a
  custom UIMD domain and can use release assets directly, for example
  `curl -sSf https://github.com/uimd-lang/uimd/releases/latest/download/install.sh | sh`
  on macOS/Linux and
  `iwr https://github.com/uimd-lang/uimd/releases/latest/download/install.ps1 | iex`
  on Windows. The scripts must download and run `uimd-init`, verify
  checksums/signatures for downloaded binaries, avoid raw branch URLs such as
  `raw.githubusercontent.com/.../main/...`, and remain compatible with a future
  friendly redirect such as `https://install.uimd.dev` if a domain/server is
  later added.
- [ ] Verify that the PyPI package name `uimd` is available before the first
  public release.
- [ ] Confirm GitHub repository description is set to:
  `User Interface Markdown. Define app interfaces in Markdown and generate UI
  and agent access.`
- [ ] Confirm GitHub repository topics are set:
  `markdown`, `ui`, `mcp`, `code-generation`, `terminal-ui`, `python`, `cpp`.
- [ ] Decide how the current SVN source of truth maps to GitHub publication.
- [ ] Add or document GitHub-specific ignore rules only when actually publishing
  to GitHub, without violating the current SVN workflow rules.
- [ ] Configure PyPI Trusted Publisher for repository `uimd-lang/uimd` once the
  package is ready to publish.
- [ ] Publish the first GitHub release tag before relying on generated CMake
  `FetchContent` fallback for external users.

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
