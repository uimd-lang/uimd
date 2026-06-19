# TODO

## Current Open Tasks

Date: 2026-06-05

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
- [x] **Expense tracker modal delete leaves Python/C++ scrollview gap
  background mismatch**. `tests/mcp/expense_tracker_compare.yaml` fails after
  activating `yes_btn` in the delete confirmation: snapshot
  `tests/mcp/snapshots/20260619-004848-step-18-expense_tracker_compare.json`
  reports row 12 col 28 background `#172033` on C++ versus `#293143` on Python
  in the empty scrollview gap above the first expense row. This is a
  parity-sensitive runtime/render cleanup issue, not an example-specific
  workaround. Audit Python `UIScrollView` selected/focus background rendering in
  `src/uimd/runtime/UIScrollView.py` and `src/uimd/runtime/UIBase.py` against
  C++ scrollview/render behavior in `cpp/src/elements/ScrollView.cpp` and
  `cpp/src/generated/GeneratedWindowRuntime.cpp`. Repro:
  `env UIMD_DISABLE_SIXEL=1 ./uimd mcp-test --compare
  python/examples/expense_tracker/expense_tracker.py
  cpp/build/examples/expense_tracker/expense_tracker
  tests/mcp/expense_tracker_compare.yaml --compare-app-size 90x35 --mcp-fast`.
  Snapshot viewer: `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260619-004848-step-18-expense_tracker_compare.json
  --plain`. Fixed by clearing the scrollview pending proxy-focus restore flag
  whenever shared Python descendant focus cleanup runs, so a render/snapshot
  cannot resurrect a scrollview proxy focus after the previously focused child
  was deleted. This matches the C++ runtime cleanup model, where
  `clearRemovedBackgroundScrollViewScope` removes the active scrollview scope
  and does not have a render-time proxy-focus restore path. Validation passed:
  Python bytecode compilation, `git diff --check`, and
  `env UIMD_DISABLE_SIXEL=1 ./uimd mcp-test --compare
  python/examples/expense_tracker/expense_tracker.py
  cpp/build/examples/expense_tracker/expense_tracker
  tests/mcp/expense_tracker_compare.yaml --compare-app-size 90x35 --mcp-fast`
  with `64 asserts passed, 0 failed, 0 step failures`. Post-fix validation
  also passed: `./tools/rebuild_all.sh`, `ctest --test-dir cpp/build
  --output-on-failure` with `26/26` tests, `env UIMD_DISABLE_SIXEL=1 ./uimd
  mcp-test --compare python/examples/task_board/task_board.py
  cpp/build/examples/task_board/task_board tests/mcp/task_board_compare.yaml
  --compare-app-size 90x35 --mcp-fast` with `144 asserts passed`, and the
  expense compare rerun after the rebuild with `64 asserts passed`.
- [x] **Task board filter apply/reset-only behavior polish**. Adjust the
  reusable `task_filters` component so changing `search`, `status_filter`, or
  `owner_filter` does not refresh the board automatically; only `Apply filter`
  and `Reset` may apply filtering. Update the filter layout so there are two
  spaces between `Apply filter` and `Reset`, with one empty row below the button
  row. Keep Python/C++ `.uimd` sources equivalent, regenerate affected outputs,
  rebuild C++, and rerun focused validation. Implemented with matching Python
  and C++ callback wrappers that no longer override text/selection change
  events, updated MCP steps to activate `filters.apply_filters_btn` explicitly,
  and regenerated affected Python/C++ outputs. Validation passed: Python
  bytecode compilation, C++ rebuild, C++ `--smoke`, C++ `--logic-test`, Python
  single-app MCP, C++ single-app MCP, and `git diff --check`. Cross-platform
  compare was rerun and still fails on the separate reusable-layout parity bug
  logged below.
- [x] **Task board reusable flatscraper-style filter component**. Correct the
  `task_board` filter example structure so it matches the flatscraper admin
  pattern: filters live in a separate reusable `.uimd` control with its own
  label/input/action layout, and the main `task_board.uimd` only hosts that
  control above the board. Keep Python `.uimd` files as the reference and make
  the C++ `.uimd` copies byte-for-byte equivalent. Parity decision: this is a
  cross-platform example/component architecture change; behavior must remain the
  same on Python and C++, with callbacks moved into matching Python/C++ filter
  component wrappers. Implemented `task_filters/task_filters.uimd` and matching
  Python/C++ wrappers, replaced inline filter controls in `task_board.uimd` with
  a reusable `filters` host, regenerated Python and C++ outputs, rebuilt the C++
  `task_board` example, and updated MCP coverage to address nested
  `filters.*` element IDs. Validation passed: Python bytecode compilation,
  C++ `--smoke`, C++ `--logic-test`, Python single-app MCP, and C++ single-app
  MCP with `72 asserts passed, 0 failed, 0 step failures`. Cross-platform
  compare was rerun and failed on the separate reusable-layout parity bug
  logged below.
- [x] **Task board reusable filter expanded column layout parity**. The new
  flatscraper-style reusable `task_filters` control exposes a Python/C++ layout
  parity bug at the initial compare snapshot: C++ renders `status_label` at
  row 2 col 30, while Python has expanded the first filter column and renders a
  blank cell there. The mismatch is `expected char=S ... got char= ` in
  `tests/mcp/snapshots/20260618-234411-step-1-task_board_compare.json`.
  Python `.uimd` source remains the reference; audit Python runtime reusable
  layout sizing in `src/uimd/runtime/UIBase.py::_resolve_layout_geometry` and
  `src/uimd/runtime/elements.py::UIElementReusable.render` against C++ runtime
  layout/rendering in `cpp/src/generated/GeneratedWindowRuntime.cpp`
  (`resolveRuntimeCellsWithFitPass`, `renderColFor`,
  `syncReusableChildFrames`, and `renderGeneratedWindowContent`) before changing
  examples, tests, or snapshots. Repro:
  `env UIMD_DISABLE_SIXEL=1 ./uimd mcp-test --compare
  python/examples/task_board/task_board.py
  cpp/build/examples/task_board/task_board tests/mcp/task_board_compare.yaml
  --compare-app-size 90x35 --mcp-fast`. Snapshot viewer:
  `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260618-234411-step-1-task_board_compare.json --plain`.
  Fixed by rendering and syncing C++ reusable child windows with forced
  fullscreen layout, matching Python `UIElementReusable` child layout behavior.
  Validation passed: `cmake --build cpp/build --target task_board`,
  `env UIMD_DISABLE_SIXEL=1 ./uimd mcp-test --compare
  python/examples/task_board/task_board.py
  cpp/build/examples/task_board/task_board tests/mcp/task_board_compare.yaml
  --compare-app-size 90x35 --mcp-fast` with `144 asserts passed, 0 failed,
  0 step failures`, `./tools/rebuild_all.sh`, `ctest --test-dir cpp/build
  --output-on-failure` with `26/26` tests passed, and `git diff --check`.
- [x] **Task board filter bar exact flatscraper structure sync**. The Python
  `task_board.uimd` was manually corrected to match the flatscraper admin filter
  bar structure more closely, with label/input rows, a spacer row, and
  Apply/Reset actions below. Sync the C++ `.uimd` source byte-for-byte to that
  Python reference, regenerate both generated outputs, rebuild `task_board`, and
  rerun focused validation. Parity decision: this is an example/layout sync only;
  Python `.uimd` is the reference and runtime behavior remains unchanged.
  Validation passed: `.uimd` byte-for-byte identity, 80-column UI rows,
  regenerated Python and C++ outputs, rebuilt `task_board`, passed C++ smoke and
  logic tests, passed Python bytecode compilation, and passed Python-only and
  C++-only MCP runs with `72 asserts passed, 0 failed, 0 step failures`.
  Cross-platform compare was rerun and failed on the separate layout parity bug
  logged above.
- [x] **C++ task board render snapshot keeps focused ScrollView background after
  modal close**. The updated `task_board` compare now exposes a runtime render
  parity issue after saving through the task dialog: C++ `get_render_snapshot`
  keeps `board` focus background `#07111f` on row 14 col 27 while Python renders
  the same cell with base board background `#030712`. Python runtime behavior is
  the reference; audit Python modal-close/focus rendering in
  `src/uimd/runtime` against the C++ generated runtime path in
  `cpp/src/generated/GeneratedWindowRuntime.cpp` before changing tests or
  snapshots. Repro:
  `env UIMD_DISABLE_SIXEL=1 ./uimd mcp-test --compare
  python/examples/task_board/task_board.py
  cpp/build/examples/task_board/task_board tests/mcp/task_board_compare.yaml
  --compare-app-size 90x35 --mcp-fast`. Snapshot viewer:
  `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260618-224235-step-72-task_board_compare.json`.
  Fixed by using the shared C++ modal-close background focus cleanup path from
  MCP button activation and by clearing removed active scrollview descendant
  focus before restoring a proxy. Validation passed in the full task board
  compare with `144 asserts passed, 0 failed, 0 step failures`,
  `./tools/rebuild_all.sh`, `ctest --test-dir cpp/build --output-on-failure`
  with `26/26` tests passed, and `git diff --check`.
- [x] **Python MCP modal callback restores stale ScrollView descendant focus
  after bulk refresh**. After fixing the C++ modal-close cleanup path, the
  `task_board` compare now reaches the later `mark_all_done_btn` confirmation
  and Python renders a stale focused `board[0].delete_btn` after the bulk
  action rebuilds the scrollview children, while C++ leaves focus on
  `mark_all_done_btn`. Python cleanup should not restore a deleted scrollview
  descendant when the active focus is outside that scrollview. Audit Python
  focus cleanup in `src/uimd/runtime/UIBase.py`,
  `src/uimd/runtime/UIScrollView.py`, and `src/uimd/runtime/mcp.py` against the
  C++ generated runtime modal cleanup in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`; keep the same user-visible
  state after modal callbacks and scrollview child refreshes. Repro:
  `env UIMD_DISABLE_SIXEL=1 ./uimd mcp-test --compare
  python/examples/task_board/task_board.py
  cpp/build/examples/task_board/task_board tests/mcp/task_board_compare.yaml
  --compare-app-size 90x35 --mcp-fast`. Snapshot viewer:
  `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260619-002312-step-107-task_board_compare.json
  --plain`. Fixed by making Python `UIScrollView.clear_children()` restore a
  removed child focus only when the current owner focus or active scrollview
  scope belongs to that scrollview; stale descendant focus from inactive
  scrollviews is now cleared instead of restored onto newly rebuilt rows.
  Validation passed: Python runtime bytecode compilation, full task board
  Python/C++ compare with `144 asserts passed, 0 failed, 0 step failures`,
  `./tools/rebuild_all.sh`, `ctest --test-dir cpp/build --output-on-failure`
  with `26/26` tests passed, and `git diff --check`.
- [x] **Task board filter bar parity repro layout**. Move the existing
  `task_board` filter controls out of the left panel and into a top filter bar
  with Apply and Reset actions, matching the flatscraper admin filter structure
  closely enough to exercise the same Python/C++ render parity surface. Keep
  the left panel useful with task actions, including confirmed bulk actions for
  marking all tasks done and clearing the board. Keep the Python and C++
  `.uimd` sources byte-for-byte equivalent, regenerate both generated outputs,
  rebuild the C++ example, and rerun the existing `task_board` MCP compare with
  `--compare-app-size 90x35`. Parity decision: this is a cross-platform
  example/layout change only; runtime behavior should remain unchanged, but the
  top filter row intentionally provides coverage for TextInput/ComboBox
  background rendering inside a dynamic scrollview example. Implementation and
  single-runtime validation are done: regenerated Python and C++ outputs,
  rebuilt `task_board`, passed C++ smoke and logic tests, passed Python bytecode
  compilation, and passed the updated `task_board_compare.yaml` on Python-only
  and C++-only MCP runs with `72 asserts passed, 0 failed, 0 step failures`.
  Cross-platform compare was rerun and failed on the separate C++ render parity
  bug logged above.
- [x] **Formular MCP dropdown click regression after sync**. The full MCP
  compare suite fails in `tests/mcp/formular.yaml` because the mouse click that
  should choose `Hungary` still targets an old blank row after the combobox is
  opened. Python-only reproduction shows the same failure, so this is a stale
  test coordinate rather than a one-platform runtime parity bug. While
  investigating, fix the existing Python/C++ `formular.uimd` source mismatch so
  both platforms use byte-for-byte equivalent UI metadata before regenerating.
  Parity decision: runtime behavior is unchanged; regenerate the affected
  Python and C++ example outputs from matching `.uimd` sources, rebuild the C++
  example, and rerun isolated Python/C++ compare coverage. Validation passed:
  `./uimd generate python/examples/formular/formular.uimd --target python`,
  `./uimd generate cpp/examples/formular/formular.uimd --target cpp`,
  `cmake --build cpp/build --target formular`, and
  `./uimd mcp-test tests/mcp/formular.yaml --compare python/examples
  cpp/build/examples --mcp-fast --compare-app-size 90x35` with `76 asserts
  passed, 0 failed, 0 step failures`. Full MCP compare also passed:
  `./uimd mcp-test --all --compare python/examples cpp/build/examples
  --mcp-fast --compare-app-size 90x35` with `590 asserts passed, 0 failed,
  0 step failures`.
- [x] **Installed SDK Python runtime requires Pillow for non-image apps**.
  Fix the public `v0.4.0` installer smoke failure where `uimd run hello.uimd`
  in an external project imports `uimd.runtime`, then fails with
  `ModuleNotFoundError: No module named 'PIL'` before rendering any image.
  Pillow must be optional for non-image applications and required only when
  Python `Image` rendering actually needs it. This is Python runtime behavior;
  C++ image/runtime behavior is unaffected because C++ does not import Pillow.
  Local implementation is done by lazy-loading Pillow inside image rendering
  helpers and keeping runtime import usable without Pillow. Validation passed:
  `python/tests/test_elements.py::TestImage`, full Python suite with
  `431 passed, 11 skipped`, `./tools/rebuild_all.sh`, `git diff --check`, and
  a system-Python `PYTHONPATH=src` import/render smoke without Pillow. Release
  repair completed: commit `d0fe32f` was pushed to `sdk-work` and `main`, all
  GitHub Actions checks passed, `v0.4.0` was moved to the fixed commit, signed
  GitHub Release assets were rebuilt and replaced, the remote checksum
  signature verified, public install from
  `https://github.com/uimd-lang/uimd/releases/download/v0.4.0/install.sh`
  passed in `/Users/marekdubovsky/Projects/uimd-test`, and the first app
  Python/C++ smoke tests passed.
- [x] **Python 3.10 CI mock import regression after the workflow fix push**.
  Fix the remaining `main` GitHub Actions failure on commit `51898cf`: Python
  3.10 fails `test_dialog_button_activation_delays_action_for_visible_focus`
  because `unittest.mock.patch("uimd.runtime.UIBase.time.sleep")` resolves
  `uimd.runtime.UIBase` to the exported `UIBase` class from
  `uimd.runtime.__init__` instead of the module. This is test-only CI
  hardening; runtime behavior and Python/C++ parity are unaffected. Implemented
  by importing the real `uimd.runtime.UIBase` module with `importlib` and using
  `patch.object` against that module's `time.sleep`. Validation passed:
  isolated dialog-delay pytest and `PATH=/private/tmp/uimd-ci-fix-venv/bin:$PATH
  /private/tmp/uimd-ci-fix-venv/bin/python -m pytest python/tests` with
  `430 passed, 11 skipped`.
- [x] **GitHub CI failures after the v0.4.0 main/tag push**. Fix the
  GitHub-hosted Python workflow failures without deleting local tests: keep
  sixel image coverage while handling the optional `libsixel` dependency
  explicitly in CI or through a correct skip condition, make MCP tester config
  tests independent of an existing local `cpp/build/examples` directory, and
  patch dialog-delay mocking through the canonical import path so the test
  passes on both Python 3.10 and 3.12. This is test/CI behavior only; no
  runtime parity behavior should change unless the investigation finds a real
  runtime bug. Implemented by keeping a required mocked UIMD sixel render-path
  test plus an optional real `libsixel` integration test, creating temporary
  Python/C++ example roots inside the MCP tester config tests, and patching
  `uimd.runtime.UIBase.time.sleep` in the dialog-delay test. Validation passed:
  `PATH=/private/tmp/uimd-ci-fix-venv/bin:$PATH
  /private/tmp/uimd-ci-fix-venv/bin/python -m pytest python/tests` with
  `427 passed, 14 skipped`.
- [x] **GitHub workflow failures after the CI test fix push**. Fix the next
  `main` push failures on commit `ea2aa94`: `.github/workflows/generated.yml`
  still calls removed legacy compiler wrappers (`compile.py` and
  `tools/compile_cpp.py`), `.github/workflows/mcp.yml` launches the default
  C++ MCP tester without building `uimd_mcp_tester`, and the Linux C++ workflow
  fails because `cpp/src/generated/GeneratedWindowRuntime.cpp` uses
  `std::nearbyint` without including `<cmath>`. While validating locally,
  `ctest` also exposed that C++ image smoke tests require the optional
  `libsixel` runtime library unless they explicitly force fallback mode. This
  is CI/runtime portability cleanup only; keep canonical generation through the
  native `./uimd` entry point and do not remove sixel coverage. Implemented by
  switching generated-source CI to build the native `uimd` launcher and run
  `./uimd generate ...`, pinning the MCP workflow to the Python backend it
  installs, adding the missing C++ `<cmath>` include for Linux, forcing fallback
  mode only for C++ image smoke tests that do not require system `libsixel`, and
  correcting the stale `formular.yaml` dropdown click coordinate for `Hungary`.
  Validation passed: local generated-source commands for Python and C++ targets,
  `cmake -S cpp -B cpp/build`, `cmake --build cpp/build --parallel`,
  `ctest --test-dir cpp/build --output-on-failure` with `26/26` tests passed,
  isolated `formular.yaml` on both Python and C++ MCP backends, and
  `python3 tools/mcp_tester/mcp_tester.py --backend python
  tests/mcp/all_examples.yaml --exit-on-finish` with `305 asserts passed, 0
  failed, 0 step failures`.
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
