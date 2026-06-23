# TODO

## Current Open Tasks

Date: 2026-06-21

- [x] **Begin C# target implementation**. User wants to start implementing C#
  support as a new UIMD backend/SDK target. Initial target name should be
  `csharp`, matching the existing SDK Store planning text (`targets/csharp`).
  MOST IMPORTANT RULE: C# must be implemented 1:1 with the Python reference
  behavior. C++ may be used as the faster implementation/compare baseline only
  where it is already demonstrably 1:1 with Python; if Python and C++ differ,
  Python wins and C++ parity must be treated as suspect. Every C#, C++, and
  Python branch of the same example, regression example, dialog, tester UI, or
  reusable component must use byte-for-byte identical `.uimd` sources except
  for unavoidable repository path mechanics. Do not finish the C# work until
  every generated API, event hook, layout result, runtime behavior, example,
  regression example, compare script, and documented command that exists for
  Python/C++ has an equivalent C# implementation and all tests pass.
  PORTING RULE: C# implementation work must be a direct port/translation of the
  existing Python reference and/or already-parity-validated C++ implementation.
  Do not design a fresh C# architecture, invent new public classes, introduce
  new behavior helpers, rename concepts, or add C#-only feature surfaces just
  because they feel idiomatic. Runtime, generator, UIMD target plumbing, tools,
  dialogs, examples, and tester integration should reuse the same concepts,
  state transitions, function responsibilities, public names, and file
  organization patterns as the source implementation being ported. Exceptions
  are allowed only for unavoidable C#/.NET language, runtime, terminal, build,
  or packaging specifics; every exception must preserve public behavior and be
  recorded as a parity exception before it is accepted.
  CONTINUATION RULE: If context is compacted/autocompacted at any point during
  the C# implementation, the resumed agent must reread `AGENTS.md` and
  `prompts/TODO.md` before continuing implementation, validation, or reporting
  completion.
  Required first implementation steps: add/update this TODO with parity notes
  before code changes, extend native `./uimd generate <path> --target csharp`
  in `cpp/tools/uimd`, add a minimal `NativeCSharpGenerator`, create the
  initial C# runtime/project tree, generate from the same Python-reference
  `.uimd` sources rather than C#-specific copies, and document new build/test
  commands in `docs/example_cli_commands.md`. Parity decision: this is a new
  cross-platform target, so public generated APIs, event hook names, layout,
  style, focus/input behavior, and reusable/window behavior must follow the
  Python runtime reference and stay architecturally aligned with C++ where the
  behavior already exists. Local toolchain state recorded on 2026-06-23: user
  installed .NET SDK `10.0.301` into `/Users/marekdubovsky/.dotnet` using
  `dotnet-install.sh`; `dotnet --info` reported RID `osx-x64`, OS Platform
  `Darwin`, host runtime `10.0.9`, and no workloads installed. If `dotnet` is
  not on PATH after restart, use `/Users/marekdubovsky/.dotnet/dotnet`
  directly or add `export PATH="$HOME/.dotnet:$PATH"` to the active shell
  profile.
  Implementation breakdown:
  1. Inventory Python/C++ surface area before coding: native generator targets,
     generated public classes/members/hooks, runtime public API, standard
     dialogs, reusable elements, examples, regression examples, MCP scripts,
     rebuild scripts, and docs command lists.
  2. Add C# target plumbing in the native C++ CLI/generator only:
     `cpp/tools/uimd` target parsing, diagnostics, recursive generation, output
     naming, dependency generation, and rebuild integration. Do not create a
     Python compiler/CLI path.
  3. Build the C# generator to mirror Python/C++ output contracts: generated
     window class, element fields, constructor options, style mapping, event
     override hooks, reusable-component embedding, dialogs, imports/namespaces,
     and dependency path handling.
  4. Create the canonical C# runtime with the same architecture as Python/C++:
     window/modal stack, UIBase/UIControl model, elements, focus/edit mode,
     layout/rendering, styles, text input selection/cursor behavior, listbox and
     combobox behavior, scrollview, reusable components, label text selection,
     image/Sixel handling or explicit parity-gated unsupported diagnostics, MCP
     runtime contract, and post-event cleanup.
  5. Add C# examples for every Python/C++ example, generated from the same
     `.uimd` source. Application logic may be target-specific C#, but no
     example-specific runtime/layout workarounds are allowed.
  6. Add C# regression examples/tests for every relevant Python/C++ regression
     case. Regression `.uimd` files must be exact copies across Python, C++,
     and C# branches or generated from one shared source.
  7. Extend MCP compare support so C# can be compared against C++ for speed and
     against Python when needed to settle parity disagreements. All automated
     compare commands must use `--compare-app-size 90x35`.
  8. Update `docs/example_cli_commands.md` with every new C# example, C#
     regression example, generation/build/run command, and compare/test command
     in the correct sorted sections.
  9. Validate in layers: generator unit/smoke tests, C# build, focused
     example/regression compares, all C# examples, all Python tests, all C++
     tests/builds, full all-platform MCP compare suite, and final clean working
     tree review. Record any unavoidable unsupported C# platform primitive as a
     TODO parity exception before accepting it.
  Implemented parity notes: `expense_tracker` compare found that C# highlighted
  only rendered-content rows for a ScrollView child whose descendant checkbox
  has focus, while Python `UIScrollView._apply_selected_background_to_viewport_rows`
  and `UIScrollView._apply_self_focus_to_viewport` highlight the whole visible
  ScrollView row range, including blank/padding rows. C# `ScrollView.Render`
  now ports that behavior in the shared runtime rather than adding an
  example-specific workaround. `formular` and `expense_tracker` compare found
  that C# MCP mouse clicks in the right compare pane were using terminal
  coordinates directly, while C++ uses `windowPointFromTerminalPoint` and
  Python translates mouse events relative to the app viewport. C#
  `GeneratedWindow` now parses and stores viewport row/col/width/height,
  returns that viewport from MCP viewport tools, converts terminal mouse
  coordinates to window coordinates before hit testing, and keeps ComboBox
  dropdown clicks aligned with the shared behavior by selecting only option rows
  and closing edit mode without changing selection when the closed row is
  clicked. Focused validation passed for `tests/mcp/expense_tracker_compare.yaml`;
  focused `formular` validation passed for both C++/C# with 135 asserts and
  Python/C# with 76 asserts; full C# vs Python all-examples compare passed with
  626 asserts; standalone C# all-examples MCP passed with 313 asserts; C++/C#
  all-examples compare passed with 1224 asserts. Final validation passed with
  `./tools/test_all.sh --compare-app-size 90x35`: repo-local native tool build,
  UIMD source regeneration, CMake configure, C++ runtime/tools/examples/regression
  build, C# runtime/examples build, Python compile, Python tests, CTest, Python/C++
  MCP example compare, C++/C# MCP example compare, and Python/C++ regression
  parity compare all passed. Final source parity check confirmed all 42 C#
  example `.uimd` files match the Python example `.uimd` files byte-for-byte.
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
