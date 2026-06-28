# TODO

## Current Open Tasks

Date: 2026-06-21

- [ ] **C# FileBrowser long-list keyboard movement exposes modal background
  rendering parity mismatch**. While extending `image_browser_compare` coverage
  on 2026-06-27, opening the `Browse` FileBrowser via `mouse_press`/
  `mouse_release` and then pressing `Down` repeatedly inside the FileBrowser
  `entries` ListBox exposed a C++/C# full-surface snapshot mismatch. Snapshot
  `tests/mcp/snapshots/20260627-221556-step-64-image_browser_compare.json`
  shows C++ rendering the background `Image items` row at terminal row 4, while
  C# renders the same background row at row 5 because the background frame still
  uses `PendingModalScrollView` during modal rendering. Disabling the pending
  active ScrollView completely caused a different foreground mismatch in
  `tests/mcp/snapshots/20260627-221347-step-60-image_browser_compare.json`,
  so the fix must separate background focus restoration from visual background
  layout, not remove the cleanup state. Parity paths: C#
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs` `OpenWindow`,
  `RuntimeFrame.PendingModalScrollView`, `FocusedElementForRender`, and
  background-frame `RenderContent`; C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp` `renderWindowStackOverlay`,
  stack-frame focused index handling, and `renderGeneratedWindowContent`.
  Required validation: re-add the long-list FileBrowser keyboard coverage to
  `tests/mcp/image_browser_compare.yaml`, confirm native ListBox keeps the
  selected file visible, run focused C++/C# image_browser compare with
  `--compare-app-size 90x35`, and run the broader rebuild/parity gate for the
  touched shared runtime.

- [ ] **C# direct terminal mouse drag/text-selection parity remains
  incomplete**. While fixing direct C# Ctrl+C and SGR mouse press handling, the
  parser was taught to consume SGR release/drag reports so they no longer leak
  into app text input, but direct drag selection behavior itself is not yet a
  full 1:1 port of Python/C++ terminal mouse handling. Affected parity sources:
  Python `src/uimd/runtime/application.py` `_decode_sgr_mouse_sequence` and
  UI dispatch in `UIBase.py`, plus C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp` direct terminal mouse
  press/drag/release handling. C# must route drag/release through the same
  shared selection/click-candidate cleanup model rather than leaving it as a
  consumed no-op. Required validation: direct PTY drag-selection smoke for a
  selectable label/text input, direct release/click-candidate behavior on a
  button, focused MCP compare unchanged, and all C# examples build.
- [ ] **Nested ScrollView edit-scope dim overlay has a one-column Python/C++
  edge mismatch**. While adding arrow navigation coverage to
  `stale_scrollview_focus`, pressing `Enter` on the focused `page.fields`
  ScrollView proxy correctly enters nested edit scope on both platforms, but
  compare reports Python dimming the rightmost host edge cell where C++ leaves
  the base background. Affected paths: Python
  `src/uimd/runtime/UIBase.py` active scrollview scope dim rectangle and
  `src/uimd/runtime/UIScrollView.py` proxy/self focus rendering; C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp` `dimOutsideActiveScrollView`
  and generated child-window render clipping. Parity decision: this is render
  parity cleanup for active nested ScrollView scope, separate from paste routing
  and keyboard reachability.
- [ ] **C++ generated app startup errors must be visible on stderr**. When a
  generated C++ app fails before the runtime loop starts, for example because a
  `.uimd` file requires non-fallback Sixel images and `libsixel` is unavailable,
  the process must print a clear error message and exit nonzero instead of
  appearing to do nothing. Parity decision: this is C++ entrypoint/error
  reporting only; Python already raises a visible `SixelUnavailableError`, and
  runtime image fallback/Sixel rendering behavior must not be changed. Required
  validation: rebuild the affected C++ example/tool, run a C++ image example
  with Sixel unavailable, and verify stderr includes the dependency error.
- [ ] **Windows Sixel rendering can start without drawing visible images**.
  After installing/building native `libsixel-1.dll` through the Windows helper
  and setting `UIMD_LIBSIXEL_DIR`, Python `image_gallery` no longer fails the
  missing-dependency check and starts the app, but the user reports that no
  image is visibly drawn in the Windows SSH terminal. Current validation:
  `image._load_libsixel()` returns `True`, the app startup/render smoke emits
  terminal output without the missing-`libsixel` error, and C++/Python missing
  dependency messages are now actionable. Remaining work: determine whether the
  active Windows SSH/terminal path supports Sixel graphics, verify that Sixel
  escape sequences are actually emitted, decide whether runtime/doctor should
  warn or fall back when terminal graphics are unsupported, and keep macOS/Linux
  plus C++ behavior intact. Parity decision: this is terminal image capability
  detection/rendering behavior; do not change `.uimd` image semantics, non-image
  rendering, or existing macOS/Linux Sixel behavior without compare validation.
  Current Windows setup validation: `tools\install_sixel_windows.cmd` installed
  MSYS2 at `C:\msys64`, built `C:\msys64\ucrt64\bin\libsixel-1.dll`, installed
  `libsixel-python` into the local Windows Python venv, saved
  `UIMD_LIBSIXEL_DIR=C:\msys64\ucrt64\bin`, Python `_load_libsixel()` returns
  `True`, and `.\uimd.cmd doctor` reports C++ libsixel found from
  `UIMD_LIBSIXEL_DIR`.
- [ ] **Flatscraper admin project filters/table layout still differs between
  Python and C++ runtimes**. After the scrollview focus parity fix, the
  flatscraper admin compare still fails on the initial Projects page even
  though the Python and C++ `.uimd` sources are byte-for-byte equivalent. First
  reported mismatch: row 6 col 33, Python has a blank cell while C++ renders
  `A` from the Address filter label; C++ also appears to widen/shift filter
  columns, add an empty table row before the first project, and wrap address
  text differently. Initial investigation reproduced the first mismatch with
  the read-only compare and found it is introduced after generation by
  app-specific C++ code in `flatscraper_cpp_admin.cpp`:
  `ProjectsFilters::ProjectsFilters()` mutates generated layout widths/relative
  columns and changes `filter_address_label`, `filter_developer_label`, and
  `filter_extra_label` text by adding C++-only leading spaces. The generated
  Python/C++ `projects_filters` layout entries are otherwise equivalent. Before
  treating this as a remaining UIMD runtime bug, remove/sync those C++-only app
  layout/text overrides in `flatscraper-admin` and rerun compare; if table gap
  or address clipping mismatches remain after wrapper parity is restored, audit
  Python layout/text clipping in `src/uimd/runtime/UIBase.py` and
  `src/uimd/runtime/elements.py` against C++ layout/rendering in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  `cpp/src/elements/BasicElements.cpp`, and `cpp/src/elements/ScrollView.cpp`.
  Repro used the read-only flatscraper compare with
  `UIMD_DISABLE_SIXEL=1`, `FLATSCRAPER_UIMD_DIRECT_READONLY=1`, and the TSV
  fixture/snapshot so no admin data is mutated.
- [ ] **Windows visual MCP tester window parity**. Windows MCP compare commands
  currently execute correctly through the Python tester's plain/console progress
  mode, but they do not show the same interactive UIMD tester window with
  side-by-side captured Python/C++ app panes that macOS/Linux can show through
  the POSIX PTY-backed tester path. Implement a Windows terminal capture backend
  for the tester, likely using ConPTY or an equivalent Windows pseudo-terminal
  primitive, so `.\uimd.cmd mcp-test --all --compare ...` can run with the
  visual tester UI on Windows without requiring `--headless` and without
  breaking the current POSIX PTY path. Parity decision: this is tester
  infrastructure only; generated examples, Python runtime app behavior, C++
  runtime behavior, MCP assertions, and POSIX macOS/Linux tester behavior must
  remain unchanged. Validation required: focused Windows visual compare with two
  app panes visible in the tester UI, full Windows compare smoke, full Python
  tests, and a POSIX/macOS smoke or documented audit showing the POSIX PTY path
  is untouched. Implemented progress: the Python tester now
  treats Windows interactive terminals as visual-tester capable, starts target
  apps through ConPTY only when the Windows API is available and a smoke test
  passes, and otherwise falls back to headless target processes while keeping
  the tester UI populated from MCP render frames. Focused validation passed:
  `python -m py_compile src\uimd\testing\mcp_tester.py` and
  `python -m pytest python\tests\test_mcp_tester.py -q` with
  `50 passed, 2 skipped, 2 warnings`; full `python -m pytest python\tests`
  with `435 passed, 18 skipped, 2 warnings`; and
  `cmd /c .\uimd.cmd mcp-test --plain --compare python\examples\activity_feed\activity_feed.py cpp\build-windows\examples\activity_feed\Release\activity_feed.exe tests\mcp\activity_feed.yaml --compare-app-size 90x35 --mcp-fast`
  with `47 asserts passed`. Earlier full Windows all-example compare runs
  reached `tests/mcp/image_browser_compare.yaml` before the display-path parity
  fix above; the Windows plain log output now escapes the unencodable `\u2580`
  character instead of crashing with `UnicodeEncodeError`. Remaining
  validation: user-visible interactive Windows SSH compare pane check, full
  compare smoke now that image_browser parity is fixed, and a POSIX/macOS smoke
  or documented audit.

## MCP Tester

- [ ] **General MCP compare coverage gaps from the completed C# parity pass**.
  The broad C# parity coverage task is resolved, but it surfaced reusable
  tester/coverage gaps that are not C#-specific: render-loop/tick assertions
  without input, deterministic markdown wheel/page/key scroll deltas with no
  extra scroll after input, non-brittle image render/cache performance signals
  for repeated Sixel/fallback frames, task deletion focus handoff boundary
  coverage, long-list FileBrowser/ListBox selected-item visibility, and the
  remaining `transparent`/`#00000000`/alpha color-sentinel matrix. Keep these
  as general MCP/tester improvements and avoid app-specific waits, cleanup, or
  layout/data changes.
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

  Remaining implementation gaps: package-manager recipes and cross-platform
  validation.
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
