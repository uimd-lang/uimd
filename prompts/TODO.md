# TODO

## Current Open Tasks

Date: 2026-06-05

- [x] **Native `uimd issue-report` should generate sanitized Markdown bug
  reports**. Add an SDK-facing native CLI command that reads a `.uimd` source
  and emits a GitHub-ready Markdown issue report with a short problem summary,
  reproduction placeholders, and an embedded sanitized UIMD source. Default
  privacy behavior should remove description text and anonymize element/member
  names while preserving the ASCII `ui` layout geometry with same-length
  anonymized identifiers; colors must remain unchanged because render and parity
  bugs often depend on exact foreground/background values. Provide an opt-out
  for name anonymization, keep the command generation-only rather than
  submitting to GitHub directly, and validate that the sanitized source still
  parses. Parity decision: this is native compiler/CLI behavior in
  `cpp/tools/uimd`; it must not add a Python CLI implementation and must not
  change Python or C++ runtime rendering behavior. Implemented as
  `uimd issue-report <path> [title]` with `--privacy safe|none`,
  `--no-anonymize-names`, `--keep-descriptions`, `--kind`, `--targets`, and
  `--output`; sanitized output is reparsed and checked for preserved layout
  geometry. Validation passed: `cmake --build cpp/build --target uimd`,
  `cpp/build/tools/uimd/uimd issue-report
  python/examples/calculator/calculator.uimd --title "Issue report smoke"
  --output /tmp/uimd-issue-report.md`, `PYTHONPATH=python:src python3 -m
  unittest python.tests.test_native_cli`, and a `--privacy none` smoke.
- [x] **Windows C++ SDK support through final installer artifacts**. Implement
  and validate first-class Windows support for the native UIMD CLI, generated
  C++ target, SDK packaging, and installer flow. Scope: configure/build the
  native `uimd` and `uimd-init` tools on Windows, build the C++ runtime and
  examples with the available Windows toolchain, keep generated C++ public APIs
  and runtime behavior equivalent to Python/C++ behavior on other platforms,
  package a Windows SDK slice with `uimd-sdk-<version>-windows-<arch>.tar.gz`,
  `uimd-init-<version>-windows-<arch>.exe`, `install.ps1`, checksums, and
  signature metadata when signing is available, then smoke-test a clean
  `UIMD_HOME` install and an external C++ hello-world configure/build without
  relying on a sibling source checkout. Parity decision: this is native
  SDK-facing packaging/CLI plus cross-platform C++ runtime validation; do not
  add Python compiler/CLI behavior, and do not make Windows-only generated API
  differences. Required validation before completion: Windows CMake configure
  and build, native `uimd doctor --json`, `uimd generate --target cpp`, package
  creation, local `install.ps1 --NoShellConfig` install, installed launcher
  `doctor --json`, external project `uimd new`, C++ generation, and C++ build
  against installed SDK `targets/cpp`. Implemented with portable Windows socket
  support in the C++ generated runtime, Windows Visual Studio CMake packaging,
  SDK Store lookup in generated/scaffolded C++ CMake projects, and a
  self-contained `uimd-init` checksum verifier. macOS C++ protection: POSIX
  terminal/signal paths remain under non-Windows guards, the POSIX MCP tester is
  still built on `NOT WIN32` platforms, and portable parser/installer changes do
  not introduce Windows-only generated APIs. Validation passed on Windows:
  Visual Studio CMake configure/build, full C++ build including native `uimd`
  and `uimd-init`, `ctest` with `26/26` tests, package creation into
  `dist/sdk-release-windows`, clean local `install.ps1 --no-shell-config`
  install, installed `uimd doctor --json`, external `uimd new hello --target
  cpp`, `uimd generate hello.uimd --target cpp`, and external C++ configure/build
  against the installed SDK `targets/cpp`. Local Windows artifact validation used
  `--allow-unsigned-local-release` because production minisign credentials were
  not available on this machine; public release artifacts still require
  `checksums.txt.minisig` signing.
- [ ] **C++ generated app startup errors must be visible on stderr**. When a
  generated C++ app fails before the runtime loop starts, for example because a
  `.uimd` file requires non-fallback Sixel images and `libsixel` is unavailable,
  the process must print a clear error message and exit nonzero instead of
  appearing to do nothing. Parity decision: this is C++ entrypoint/error
  reporting only; Python already raises a visible `SixelUnavailableError`, and
  runtime image fallback/Sixel rendering behavior must not be changed. Required
  validation: rebuild the affected C++ example/tool, run a C++ image example
  with Sixel unavailable, and verify stderr includes the dependency error.
- [x] **`uimd doctor` must explain optional Sixel image dependencies**. When
  diagnosing an install/source checkout, the native `uimd doctor` output should
  show whether the C++ runtime can find a candidate `libsixel` library, list the
  relevant `UIMD_LIBSIXEL_PATH`/`UIMD_LIBSIXEL_DIR` override mechanism, and give
  the Python `libsixel` binding install/verify commands. Parity decision: this
  is SDK-facing CLI diagnostics only; it must not change Python or C++ image
  runtime behavior, fallback rendering, or platform-specific loader behavior.
  Validation passed on Windows: rebuilt the native `uimd` target with Visual
  Studio CMake, ran `cpp\build-windows\tools\uimd\Release\uimd.exe doctor`,
  confirmed the new Images/Sixel section reports missing optional C++
  `libsixel` while keeping `Status: ok`, ran `doctor --json`, and parsed the
  JSON with `python -m json.tool`; rebuilt again after the short `--help`
  wording update and confirmed the help lists optional image dependency
  diagnostics under `doctor`.
- [x] **Python Sixel startup failures should be user-facing errors, not raw
  tracebacks**. Running a Python app that requires non-fallback Image/Sixel
  rendering without the optional Python `libsixel` binding currently prints a
  full traceback from generated UI initialization. Show a concise UIMD error
  with the install command and fallback environment command while preserving
  normal tracebacks for unrelated exceptions. Parity decision: this is Python
  runtime error presentation for the same missing optional dependency that C++
  generated app startup error handling covers separately; do not change Image
  fallback/Sixel rendering behavior or `.uimd` render-mode semantics. Required
  validation passed on Windows: focused `TestImage` pytest coverage confirmed
  the friendly error hook prints install/fallback guidance without a traceback,
  `cmd /c .\uimd.cmd run python\examples\image_gallery\image_gallery.uimd`
  printed the concise UIMD Sixel error and exited nonzero without a traceback,
  and full `python -m pytest python\tests` passed with `436 passed,
  18 skipped`.
- [x] **Python Sixel diagnostics must distinguish binding install from native
  DLL availability**. Installing `libsixel-python` on Windows can still fail
  with `libsixel not found.` because the Python package is only a binding and
  still needs a native `sixel`/`libsixel` DLL. Update the friendly Python
  startup error to explain the two-layer dependency, and let the Python runtime
  honor the same `UIMD_LIBSIXEL_PATH`/`UIMD_LIBSIXEL_DIR` override mechanism
  used by the C++ runtime where possible. Parity decision: this is optional
  Sixel dependency discovery and messaging only; do not change fallback
  rendering, required `render_mode: sixel` behavior, or non-image app startup.
  Implemented by making Python runtime Sixel loading honor
  `UIMD_LIBSIXEL_PATH`/`UIMD_LIBSIXEL_DIR` through the `libsixel-python`
  `find_library("sixel")` lookup path, updating the friendly error to describe
  the Python binding plus native DLL layers, and adding `libsixel-1.dll` to the
  C++ Windows search names. Validation passed on Windows: focused
  `TestImage` tests for native-DLL guidance and configured lookup helpers,
  missing-native-DLL `uimd run image_gallery` smoke showing the native library
  guidance, rebuilt native `uimd` and `ui_cpp_runtime`, `doctor`/`doctor --json`
  validation, full `python -m pytest python\tests` with `438 passed,
  18 skipped`, and Windows `ctest` with `26/26` tests passed.
- [x] **Python Sixel missing-native-DLL message should state installed/missing
  layers explicitly**. Even after the improved guidance, the user can still
  read `python -m pip install libsixel-python` as the complete fix. When the
  binding package is installed but raises `libsixel not found.`, the startup
  error should explicitly say `Python binding: installed` and `Native
  sixel/libsixel DLL: missing`, then show the `UIMD_LIBSIXEL_PATH` and
  `UIMD_LIBSIXEL_DIR` fix. Parity decision: messaging only; do not change
  loader behavior, render-mode semantics, fallback behavior, or C++ runtime
  loading. Validation passed on Windows: confirmed `libsixel-python` is
  installed while `sixel.dll`, `libsixel.dll`, and `libsixel-1.dll` are absent
  from `PATH`, updated focused TestImage coverage, reran the focused Image test
  block with `6 passed, 3 skipped`, and verified
  `cmd /c .\uimd.cmd run python\examples\image_gallery\image_gallery.uimd`
  now prints `Python binding: installed` and
  `Native sixel/libsixel library: missing`.
- [x] **One-command Windows Sixel installer helper**. Since the current MSYS2
  package indexes do not provide a ready `libsixel` package, Windows users need
  a single repo command that installs MSYS2 if missing, installs the build
  dependencies, builds native `libsixel` from source in the UCRT64 environment,
  installs `libsixel-1.dll`, sets `UIMD_LIBSIXEL_DIR`, and verifies the Python
  UIMD runtime can load it. Parity decision: this is Windows developer/install
  tooling only; it must not change Python/C++ runtime behavior or macOS/Linux
  Sixel behavior. Required validation: run the helper in an already-installed
  MSYS2 setup and verify it skips/reuses the installed DLL, updates the current
  and future Windows environment, and passes the Python `image._load_libsixel`
  check. Implemented `tools\install_sixel_windows.cmd` as the Windows/cmd entry
  point plus `tools\install_sixel_windows_msys2.sh` for the UCRT64 build logic;
  documented it in installation and example CLI docs. Validation passed on
  Windows: `cmd /c .\tools\install_sixel_windows.cmd` reused the installed
  `/ucrt64/bin/libsixel-1.dll`, set `UIMD_LIBSIXEL_DIR`, saved it with `setx`,
  and verified `uimd libsixel load: True`; a configured `image_gallery` smoke
  started rendering instead of failing on missing `libsixel`.
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
- [ ] **Windows validation**: verify the new `image_button` control and the
  updated `image_browser` build and run on Windows for both Python and C++,
  confirming padding, centering, square sizing, click selection, and render-mode
  switching behave identically to macOS/Linux.
- [x] **Windows developer rebuild and example command parity**. Provide a
  Windows-native developer workflow matching the macOS/Linux `tools/rebuild_all.sh`
  and documented example commands. Scope: add a PowerShell full rebuild script,
  add a Windows checkout launcher wrapper for the native `uimd.exe`, avoid
  duplicating build logic where a small shared helper is practical, document
  Windows C++ example run/MCP/compare commands with correct Visual Studio
  `Release/*.exe` paths, and keep POSIX `./uimd`/`rebuild_all.sh` behavior
  intact for macOS Intel and Apple Silicon. Parity decision: this is compiler,
  C++ runtime, example, and MCP command-surface work; scripts may differ by
  shell/OS primitives, but they must invoke the same native compiler/generator
  and build the same C++ runtime/examples. Implemented with shared
  `tools/uimd_dev.py` orchestration, POSIX wrappers, Windows PowerShell wrappers,
  a Windows `uimd.ps1` checkout launcher, and POSIX `./uimd` lookup support for
  Windows Git Bash build layouts. Windows MCP command parity uses the Python MCP
  tester in headless mode because the native C++ tester still depends on POSIX
  PTY/fork primitives, but it launches and validates the built Windows C++
  example executables/runtime. Validation passed on Windows:
  `.\tools\rebuild_all.ps1 -Test`, Python bytecode compilation, full
  `cpp\build-windows` Release build, `ctest` with `26/26` tests, focused C++
  `activity_feed` MCP, focused Python/C++ `activity_feed` compare, focused
  Python/C++ `markdown_viewer` compare, PowerShell parser checks for the new
  scripts, and `git diff --check`. A full Windows `--all --compare` run was
  started with `--compare-app-size 90x35 --mcp-fast` but exceeded the local
  runner timeout before completion; focused compare coverage passed after the
  timeout cleanup.
- [x] **Windows cmd.exe wrappers for developer commands**. The documented
  Windows PowerShell scripts are not directly runnable from `cmd.exe`-style
  prompts, which can make `.\tools\rebuild_all.ps1 -Test` appear to do nothing
  or fail before the developer reaches the actual rebuild flow. Add thin `.cmd`
  wrappers that delegate to the PowerShell scripts and the repo-local
  `uimd.ps1` launcher, document the `cmd.exe` forms separately from PowerShell,
  and validate that the wrappers execute from `cmd /c` without duplicating build
  logic. Implemented `uimd.cmd` and `.cmd` wrappers for rebuild, C++ example
  run, C++ MCP, and compare helper scripts. Validation passed:
  `cmd /c .\uimd.cmd --help` reached the repo-local native `uimd.exe`, and
  `cmd /c .\tools\rebuild_all.cmd -Test` regenerated sources, built Windows
  Release C++ runtime/examples/tools under `cpp\build-windows`, compiled Python
  sources, and passed `ctest` with `26/26` tests.
- [x] **Remove PowerShell dependency from Windows SSH/cmd developer flow**.
  Windows developers connecting over SSH may land in a `cmd.exe`-style shell
  where `.ps1` scripts do not execute directly and PowerShell should not be
  required for the normal rebuild/test path. Make the `.cmd` wrappers invoke the
  shared Python helper and repo-local `uimd.exe` directly, keep `.ps1` scripts as
  optional PowerShell convenience only, document the SSH/cmd commands as the
  primary non-PowerShell Windows flow, and validate from `cmd /c`. Implemented
  `tools\uimd_dev.cmd` as the Python-helper launcher, changed all developer
  `.cmd` wrappers to avoid `powershell.exe`, and made `uimd.cmd` locate and run
  the repo-local `uimd.exe` directly. Validation passed:
  `cmd /c .\uimd.cmd --help` reached the native tool, `rg -n "powershell|pwsh"
  -g "*.cmd" .` found no PowerShell dependency in `.cmd` files, and
  `cmd /c .\tools\rebuild_all.cmd -Test` regenerated sources, built Windows
  Release C++ runtime/examples/tools under `cpp\build-windows`, compiled Python
  sources, and passed `ctest` with `26/26` tests. Follow-up hardening changed
  wrapper-to-wrapper calls to use `call` and avoids `exit /b` on successful
  completion, so interactive SSH/cmd shells should return to the prompt instead
  of closing after a successful command. Validation passed:
  `cmd /c ".\uimd.cmd --version && echo AFTER_UIMD"` and
  `cmd /c "call .\tools\uimd_dev.cmd --help && echo AFTER_HELP"` both printed
  their `AFTER_*` marker after the wrapper returned.
- [x] **MCP all-compare progress output for Windows SSH/cmd validation**.
  Long `uimd mcp-test --backend python --headless --all --compare ...` runs can
  appear hung over Windows SSH because the tester may spend minutes on an app
  without emitting progress. Add lightweight per-script/per-target progress
  output that flushes immediately, without changing MCP assertions or runtime
  behavior, so developers can tell which app/script is currently running.
  Implemented by mirroring tester log lines to stdout only in plain/headless
  mode. macOS/POSIX interactive PTY tester UI behavior is unchanged because the
  stdout mirror is gated on `config.plain`. Validation passed: focused Windows
  headless Python/C++ `activity_feed` compare printed immediate progress lines
  (`tester ready`, `SCRIPT`, `spawn`, `connected`, tool steps) throughout the
  run.
- [x] **Activity feed compare timestamp nondeterminism on Windows headless MCP**.
  Focused Windows headless compare for `tests/mcp/activity_feed.yaml` can fail
  at the first render snapshot when Python and C++ start across a minute
  boundary: one app renders a timestamp ending in `3`, the other in `4`.
  This is example data nondeterminism, not a runtime rendering mismatch. Keep
  Python and C++ example behavior equivalent and make MCP/test startup
  timestamps deterministic without creating Windows-only logic. Implemented a
  shared `UIMD_ACTIVITY_FEED_TIMESTAMP` env override in the Python and C++
  activity feed examples, and set it in `tests/mcp/activity_feed.yaml` so compare
  tests use deterministic timestamps while normal app runs still use current
  local time. Validation passed: Python bytecode compile, focused Windows C++
  `activity_feed` rebuild, and focused Windows headless Python/C++ compare with
  `47 asserts passed, 0 failed, 0 step failures`.
- [x] **Windows Python pytest parity failures after enabling SSH/cmd workflow**.
  Running `python -m pytest python\tests` on Windows with Python 3.14 exposes
  test/runtime portability gaps: `UIApplication._read_ready()` still uses
  `select.select()` on pipe file descriptors even though Windows `select` only
  accepts sockets; image fallback tests expect blank text cells but the Windows
  fallback path renders nonblank cells; MCP tester config tests compare
  POSIX-style path strings against Windows-normalized paths; and the launcher
  default backend expectation must account for Windows using the Python tester
  while POSIX keeps the C++ tester. Fix the implementation where runtime
  behavior is wrong, update only platform-specific test expectations where the
  public behavior is unchanged, and keep macOS/POSIX behavior intact. Fixed by
  keeping the POSIX `select.select()` path unchanged, adding a Windows
  `PeekNamedPipe`/console-readiness fallback only for `WinError 10038`,
  normalizing MCP tester display `source_path` values to `/`, making app-path
  assertions compare native normalized paths, documenting/testing the Windows
  Python tester default, and skipping bitmap image rendering tests when optional
  Pillow is unavailable. Validation passed on Windows:
  `python -m pytest python\tests` with `425 passed, 18 skipped, 2 warnings`.
- [x] **Windows interactive Python `uimd run` terminal backend**. `.\uimd.cmd
  run python\examples\activity_feed\activity_feed.uimd` over Windows SSH/cmd
  failed with `interactive terminal mode is not supported on this platform`
  because the Python runtime only entered raw interactive terminal mode
  through POSIX `termios`/`tty`. Implement a Windows console input/output backend
  for `UIApplication._run_gui_loop()` that enables VT output/input where
  available, reads keyboard/escape input via Windows primitives, preserves the
  existing POSIX `termios` path for macOS/Linux unchanged, and keeps MCP/headless
  behavior intact. Also make the native `uimd run` launcher select the Windows
  `python` command by default instead of `python3`, while still honoring
  `UIMD_PYTHON`. Implemented with a Windows console mode path in the Python
  runtime, Windows key/escape decoding tests, UTF-8 console stream setup, and a
  Windows default Python launcher command in the native `uimd` tool. POSIX
  `termios`/`tty` behavior remains unchanged. Validation passed on Windows:
  focused Windows console/TTY dispatch input tests, focused pipe readiness
  tests, full `python -m pytest python\tests` with `430 passed, 18 skipped, 2 warnings`,
  `.\tools\rebuild_all.cmd -Test` with `ctest` `26/26` passed, and
  `cmd /c .\uimd.cmd run python\examples\activity_feed\activity_feed.uimd`
  rendered the app and no longer exits with the unsupported platform error.
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
  with `47 asserts passed`. The full Windows all-example compare currently
  reaches `tests/mcp/image_browser_compare.yaml` and reports an existing
  Python/C++ snapshot mismatch at row 6 col 6; the Windows plain log output now
  escapes the unencodable `\u2580` character instead of crashing with
  `UnicodeEncodeError`. Remaining validation: user-visible interactive Windows
  SSH compare pane check, full compare smoke after image_browser parity is
  fixed, and a POSIX/macOS smoke or documented audit.

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
