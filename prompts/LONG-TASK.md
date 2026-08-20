# Long-Term Tasks

This file contains deferred, unrelated, and historical work moved out of the
current Java-focused [`TODO.md`](TODO.md).

## Deferred And Historical Tasks

- [x] **Install the local image-test dependencies and make aborted smoke/MCP
  summaries explicitly partial.** Requested on 2026-08-10 after the corrected
  live report showed that the source-checkout Xcode Python lacks Pillow, the
  machine lacks the system libsixel library and Python binding, and failed
  direct-terminal phases fall back to an unhelpful `0/1 gate` despite already
  completed `PASS` checks. Install libsixel plus Pillow and `libsixel-python`
  into the exact interpreter used by `tools/test_all.sh`; verify both native and
  Python dependency discovery. Count observed direct-smoke `PASS` records when
  a command aborts before its authoritative final `x/y` result, include the
  terminal failure as one attempted check, and visibly mark summary-derived MCP
  counts as partial when no final `RESULT` was emitted. Preserve final-result
  authority, complete raw logs, immediate/final failure reasons, exit behavior,
  fail-fast/keep-going behavior, test order, and coverage. Add focused reporter
  regressions, then run the complete full-test gate. Parity decision: dependency
  installation and developer/test-reporting only; runtime behavior, compiler
  output, public APIs, examples, MCP assertions, and snapshots are unchanged.

  Completed on 2026-08-10. Homebrew `libsixel 1.8.7-r2`, Xcode-Python-user-site
  `Pillow 11.3.0`, and `libsixel-python 0.5.0` are installed. The POSIX
  `test_all.sh` wrapper now discovers Homebrew's libsixel directory without a
  hardcoded prefix while respecting explicit `UIMD_LIBSIXEL_PATH` and
  `UIMD_LIBSIXEL_DIR`. Focused validation passed: reporter tests 14/14, image
  element tests 107/107 with no Pillow skips, Python/C++ MCP example compare
  1040/1040, Swift direct-terminal smoke 31/31, and Rust direct-terminal smoke
  8/8. Full-gate log:
  `.uimd/test-logs/test-all-20260810-164646-764443.log`. Before the intentional
  stop, every build/unit/lint/smoke/transport phase passed, as did MCP Python/C++
  1040/1040, C# 1972/1972, Swift 1972/1972, Go 1972/1972, general regression
  parity 14/14, and Go regression parity 29/29. The final recap's only two
  failures were manual `KeyboardInterrupt` checkpoints, not product failures:
  `MCP Rust example compare` was interrupted after 32.4 seconds and `MCP Rust
  regression parity compare` after 4/4 already-observed assertions. Validation
  then resumed with the exact interrupted coverage: the complete Rust example
  compare passed 1972/1972, `source_separator_scroll` passed 4/4, and
  `stale_scrollview_focus` passed 25/25. Together with the 26 phases that had
  already completed successfully in the full-gate run, all documented phases
  and assertions are covered without a product failure. No test process
  remains running.

- [x] **Make the final live report preserve every distinct failure cause and
  honest partial MCP counts from an aborted all-example command.** Requested on
  2026-08-10 after the first real complete `--live-report --keep-going` run.
  When earlier MCP scripts already emitted snapshot failures and a later script
  aborts the command for a different reason, retain and print both causes
  immediately and in the final recap. Attribute a command-level MCP failure to
  the active script rather than the executable, aggregate completed `SUMMARY`
  lines when no final `RESULT` is available, and avoid repeating an identical
  phase/failure label such as `Swift direct terminal smoke | Swift direct
  terminal smoke`. Preserve the complete raw log, existing exit behavior,
  fail-fast/keep-going semantics, test execution, and required coverage. Add
  focused regression cases based on the observed Python image snapshot plus
  later missing-libsixel abort and multi-command MCP aggregation. Do not install
  libsixel or change image/runtime/test expectations in this task. Parity
  decision: reporter-only developer tooling; Python/C++/C#/Swift/Go/Rust runtime
  behavior, native MCP execution, generated APIs, and snapshots are unaffected.

  Completed on 2026-08-10. The MCP parser now accumulates each completed
  per-script `SUMMARY` within a command and uses those honest partial counts
  only when that command never reaches its authoritative final `RESULT`.
  Command boundaries prevent summary/result double counting while preserving
  aggregation across the separate regression compare commands. If an MCP
  command exits after earlier structured failures, the reporter also retains a
  distinct final diagnostic that is not already represented, attributes it to
  the active YAML script, and emits it immediately at command failure and again
  in `FULL TEST RECAP`. Distinct failures within the same script are no longer
  collapsed, and an identical phase/failure label is printed only once.

  Validation passed: all 12 focused reporter/orchestration tests, including the
  observed snapshot-mismatch-then-libsixel-abort scenario and partial
  multi-command MCP aggregation; the complete Python suite with 501 passed and
  5 skipped; Python syntax compilation; and `git diff --check`. No libsixel
  package was installed, and runtime behavior, MCP scripts, image expectations,
  snapshots, and test coverage were not changed.

- [x] **Repeat concise failures in the final full-test recap and stabilize Rust
  inputs before writing the parity artifact manifest.** Requested on 2026-08-10
  after `Swift direct terminal smoke` rejected a manifest made stale when
  `rust/src/uimd/Cargo.lock` changed during the later Rust runtime test. Preserve
  immediate one-line failure reporting, but retain each deduplicated failure so
  `--live-report` also ends with one line per phase and one line per failed
  test/assertion/step including its reason. Keep fail-fast as the default and
  retain `--keep-going` as the explicit option that attempts independent later
  phases after a failure; document both concise commands in
  `docs/example_cli_commands.md`. Make the Rust build prerequisite cover the
  runtime crate before the manifest is written so subsequent tests cannot
  mutate tracked Rust dependency metadata and invalidate the artifact gate.
  Add focused regression coverage for the final recap, failure reasons,
  fail-fast/keep-going contract, and Rust prerequisite ordering. Parity
  decision: this changes developer/test orchestration only; runtime behavior,
  generated APIs, examples, MCP assertions, and cross-platform UI behavior are
  unaffected. POSIX and Windows wrappers continue forwarding the same options.

  Completed on 2026-08-10. The live reporter now retains every deduplicated
  failure event after its immediate terminal emission, retains the one-line
  count/status record for every completed or skipped phase, and ends with a
  `FULL TEST RECAP` that repeats all phase lines followed by every failed
  test/assertion/step and its concise reason before the aggregate result and log
  path. A failed command without a structured suite event uses its final
  one-line diagnostic, while smoke errors in the observed
  `ERROR <name> failed: <reason>` format are parsed and emitted immediately.
  Documentation now shows concise fail-fast and explicit `--keep-going`
  variants for POSIX, cmd, and PowerShell.

  The Rust prerequisite phase now builds `rust/src/uimd` with release
  `--all-targets` before examples/regressions and before the artifact manifest
  is written. This resolves all runtime/dev dependencies and synchronizes its
  lockfile before source hashes are recorded. Validation passed: 10 focused
  reporter/orchestration tests; the complete Python suite with 499 passed and
  5 skipped; Rust release all-targets build, 158/158 Rust tests, and Clippy with
  warnings denied; an identical SHA-256 for `rust/src/uimd/Cargo.lock` before
  and after Rust tests/Clippy; a valid parity artifact manifest afterward;
  Python syntax compilation; and `git diff --check`. A real
  `./tools/test_all.sh --live-report --keep-going` run confirmed concise phase
  output, immediate one-line reasons, and continuation after independent Swift,
  Rust, and MCP failures. The intentionally long remaining MCP platform matrix
  was stopped after C# compare because missing local libsixel would have repeated
  the same lengthy environment failure for later platforms; deterministic
  coverage verifies the final recap itself.

- [x] **Add an opt-in live concise report and one complete log for the full
  rebuild-and-test gate without changing existing test behavior.** Requested
  on 2026-08-10 for the command sequence documented in
  `docs/example_cli_commands.md`. Extend the canonical
  `tools/uimd_dev.py::test_all()` / `./tools/test_all.sh` path so one command
  streams every child process output immediately into a single timestamped log
  while the terminal prints only one completed summary line per build/test
  phase (`passed/total`, skips, duration), plus an immediate one-line diagnostic
  for every failed test, assertion, MCP script/example, or failed step. Preserve
  the complete raw output in the log, return a failing exit status when any gate
  fails, and keep the current default full-output/fail-fast behavior unchanged;
  concise reporting must be opt-in. Use explicit parsers for pytest, CTest, Go,
  Rust/Cargo, Swift, direct-terminal/transport smoke, and MCP result lines, with
  regression tests for chunked live output, counts, immediate failure emission,
  ANSI/control cleanup, logging, and exit status. If MCP needs a structured
  summary addition, make it additive in `src/uimd/testing/mcp_tester.py` and the
  native `cpp/tools/uimd` command contract without changing assertion execution,
  rendering, timing, cleanup, snapshots, or existing human-readable output.
  Parity decision: this is developer/test orchestration only; Python/C++/C#/
  Swift/Go/Rust runtime behavior and generated public APIs are unaffected, and
  POSIX/Windows wrappers must expose the same opt-in reporting contract.

  Completed on 2026-08-10. `./tools/test_all.sh --live-report --keep-going`
  now keeps the existing default full-output/fail-fast mode untouched while the
  opt-in reporter streams combined child stdout/stderr into a unique flushed
  `.uimd/test-logs/test-all-<timestamp>.log`. The terminal receives only the
  log path, one `passed/total` result per completed phase, immediate deduplicated
  one-line failures, and the final phase total. Parsers cover pytest, CTest, Go
  packages/tests, Cargo/libtest, Swift XCTest/Testing, direct/transport smokes,
  and aggregated single- or multi-command MCP results. A tiny opt-in pytest
  plugin emits the assertion message when each failure completes; existing MCP
  `FAIL step`, `SUMMARY`, and `RESULT` lines already provided the required live
  contract, so no MCP tester/runtime behavior changed. `--keep-going` continues
  independent test phases and commands inside multi-command phases, while
  configure/generate/build/manifest prerequisites remain fail-fast. POSIX/cmd
  wrappers already pass arguments through, and PowerShell now exposes matching
  `-LiveReport`, `-KeepGoing`, and `-LogFile` options.

  Validation passed: all 7 focused reporter/parser/process tests; Python with
  496 passed and 5 skipped; CTest 26/26; Rust 158/158; Swift 12/12; Python syntax
  compilation; and `git diff --check`. A real concise no-rebuild run preserved
  the missing-manifest diagnostics only in the full log and returned exit 1
  with the correct one-line terminal summaries. The exact full live command
  regenerated all sources and rebuilt the native tool plus the complete C++
  runtime/examples/regression tree, then stopped correctly at the prerequisite
  C# build because this machine has no `dotnet`; `go` is also unavailable.
  Therefore C#/Go builds and the final manifest-backed MCP matrix could not be
  executed in this environment. PowerShell (`pwsh`) is likewise unavailable,
  so its thin argument-forwarding update was source-audited but not executed.
  Follow-up on 2026-08-10: installed the arm64 .NET 10.0.302 SDK into
  `~/.dotnet`, exposed it through `~/.local/bin/dotnet`, regenerated the C#
  `activity_feed` sources, and verified its Release build with 0 warnings and
  0 errors. Installed Go 1.26.5 through Homebrew and verified the canonical Go
  runtime tests. A subsequent rebuild regenerated every platform and completed
  all C++, C#, Go, and Rust builds; the identical remaining Swift build phase
  completed for every example after granting the Codex sandbox access to the
  user-level Swift cache. The Rust builds also synchronized the example and
  regression lockfiles from UIMD 0.4.24 to the current 0.4.25 package version.
  PowerShell remains unavailable, so its wrapper has still not been executed.

- [x] **Replace mixed implicit Debug/Release artifact discovery with one
  explicit cross-platform build-profile and manifest contract.** Proposed on
  2026-08-01 after a stale Swift release binary was silently selected after a
  successful documented debug rebuild. Define one canonical `parity` profile
  for compiled example/regression artifacts (recommended mapping: C++ Release,
  C# Release, Swift release, Rust release, and the standard Go build; Python
  remains source-run), while unit-test commands continue compiling current
  sources in their native test profiles. Make `rebuild_all` write a
  content-addressed artifact manifest containing the exact generated-source
  fingerprint, build profile, build session, and exact executable path for
  every platform/app. Remove first-existing-candidate guessing from
  `src/uimd/testing/mcp_tester.py`; MCP compares, direct-terminal smokes,
  transport smokes, and regression commands must resolve through the manifest
  and fail clearly when the expected artifact is missing or its source hash is
  stale. Keep optional `debug` and `release` workflows explicit and isolated,
  never interchangeable fallbacks. Make `./tools/test_all.sh` the canonical
  atomic rebuild-and-test entry point and retain the explicit command sequence
  only as a diagnosable equivalent that validates the same manifest before
  consuming artifacts. Update `docs/example_cli_commands.md`, add profile and
  stale-source regression coverage across C++/C#/Swift/Go/Rust, and validate
  the complete full-test gate before completion. Approved for implementation
  by the user on 2026-08-01.

  Completed on 2026-08-01. `./tools/rebuild_all.sh` now produces one atomic
  `.uimd/build-manifest.json` for the canonical `parity` profile: C++/C#/Swift/
  Rust examples use Release artifacts, Go uses its direct build outputs, and
  Python remains source-run. The manifest records exact source fingerprints,
  artifact paths, profiles, build session metadata, and artifact hashes; a
  failed rebuild removes the old manifest and never publishes a partial one.
  The native `./uimd mcp-test` launcher, both MCP resolver backends, all direct
  terminal smoke scripts, and the Rust transport smoke validate this contract
  and fail closed for missing, stale, changed, or wrong-profile inputs instead
  of searching Debug/Release candidates. Unit-test commands still compile the
  current runtime sources in their native test profiles and do not substitute
  example artifacts.

  `docs/example_cli_commands.md` now documents the single parity profile and
  keeps the explicit Full Rebuild and Test block as the diagnosable equivalent
  of `./tools/test_all.sh`. Regression tests cover exact manifest-backed
  resolution for Python, C++, C#, Swift, Go, and Rust plus source/artifact
  mutation rejection. Validation passed after one successful rebuild and no
  intervening source edits: 494 Python tests, 26 CTest tests, Go/Rust/Swift
  runtime suites, Rust Clippy with warnings denied, Swift/Go/Rust direct
  terminal smokes, Rust MCP transport smoke, every Python/C++ and C++ versus
  C#/Swift/Go/Rust example compare, the Python/C++ regression corpus, and both
  Go and Rust regression compares. `./tools/test_all.sh --no-rebuild` ended
  with `FULL TEST RESULT: PASS`, and `git diff --check` passed.

- [x] **Guarantee that every command in the documented Full Rebuild and Test
  sequence consumes the sources or artifacts refreshed by
  `./tools/rebuild_all.sh`.** Audit `tools/uimd_dev.py::rebuild_all()`, every
  unit-test command, direct-terminal/MCP smoke script, examples-root resolver,
  and explicit regression binary path across Python, C++, C#, Swift, Go, and
  Rust. Record the exact producer/consumer mapping, add regression coverage for
  every configuration-sensitive examples-root choice, and fix any path or
  profile that can select an alternate stale build. Preserve each platform's
  documented profile (C# Debug, Swift debug, Rust release) rather than forcing
  all languages into one profile. Validate the tester unit suite and
  `git diff --check`; do not change runtimes, examples, snapshots, or waits for
  an artifact-selection problem.

  Completed on 2026-08-01. The producer/consumer audit found the complete
  documented sequence consistent after the Swift resolver correction:
  - Python generation and `compileall` refresh generated sources, while pytest
    and Python/C++ MCP compares import/run those current source files directly.
  - CMake configures the single-config C++ tree as Release by default, rebuilds
    `cpp/build`, and CTest, all direct-terminal smokes, MCP example compares,
    and regression compares consume binaries from that same build tree.
  - C# rebuilds `bin/Debug/net*/<name>.dll`, and examples-root MCP resolution
    deterministically prefers Debug over any Release artifact.
  - SwiftPM rebuilds `.build/debug/<name>` and both direct-terminal smoke and
    examples-root MCP resolution consume debug before the release fallback;
    `swift test` separately compiles the current runtime sources in its normal
    test/debug profile.
  - Go rebuilds each example and regression as the direct executable inside
    its app directory; direct-terminal and MCP/regression commands use those
    exact paths, while `go test` compiles the current runtime package itself.
  - Rust rebuilds examples and regressions with `cargo build --release`;
    direct-terminal, transport, examples-root MCP, and explicit regression
    commands all use `target/release`, while Cargo test and Clippy separately
    compile the current runtime sources in their appropriate test/dev profiles.

  The examples-root regression now protects all configuration-sensitive
  choices together: Python source, direct native C++/Go executable, C# Debug
  over Release, Swift debug over release, and Rust release over debug, with
  alternate candidates carrying different contents and timestamps. On the
  actual rebuilt calculator roots, resolution selected exactly
  `python/examples/calculator/calculator.py`,
  `cpp/build/examples/calculator/calculator`,
  `csharp/examples/calculator/bin/Debug/net10.0/calculator.dll`,
  `swift/examples/calculator/.build/debug/calculator`,
  `go/examples/calculator/calculator`, and
  `rust/examples/calculator/target/release/calculator`. All 56 MCP tester unit
  tests and Python syntax compilation passed. The preceding exact full Swift
  command passed all 1,972 assertions after the complete rebuild. The guarantee
  applies when `./tools/rebuild_all.sh` returns exit code zero, Swift/Rust were
  not explicitly skipped, and sources are not edited again before the tests.
  No runtime, example, snapshot, wait, or documented command required a change.

- [x] **Diagnose the C++/Swift `image_browser_compare` render-step failure from
  the complete Swift parity gate.** Reported on 2026-08-01 from
  `./uimd mcp-test --backend python --headless --all --compare
  cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35`,
  where `tests/mcp/image_browser_compare.yaml` passed 12 assertions but
  produced one step failure. Reproduce the focused command against
  `cpp/build/examples/image_browser/image_browser` and
  `swift/examples/image_browser/.build/debug/image_browser`, inspect the exact
  recorded snapshot and backend diagnostics, and audit the failing Swift path
  against Python shared semantics and C++ runtime behavior before changing
  implementation, examples, waits, masks, or snapshots. If the cause is a
  runtime/generator parity defect, record the exact Python/C++/Swift paths and
  required focused validation here before implementing a fix.

  Diagnosed on 2026-08-01 without changing runtime behavior. The saved bundle
  `tests/mcp/snapshots/20260731-234712-159810-step-005-image_browser_compare.json`
  is the automatic compact snapshot after `focus_element(astro_thumb)`. C++
  rendered the canonical `#3a4152`; the Swift target rendered `#293143`, which
  is the same `#ffffff14` reusable focus background applied one layer fewer.
  This was not intermittent runtime state. When a compare receives the examples
  root `swift/examples`,
  `src/uimd/testing/mcp_tester.py::_app_path_from_examples_root()` searches
  `.build/release/<name>` before `.build/debug/<name>`. It therefore launched
  `swift/examples/image_browser/.build/release/image_browser` from 2026-07-03,
  not the freshly built debug binary from 2026-07-31. A two-step root-path probe
  reproduced the exact colors and printed the stale release path. The same
  probe against the explicit debug binary matched C++, and the complete focused
  debug scenario passed 378 assertions. A separate Python/C++ probe confirms
  `#3a4152` is the shared reference result. No Python, C++, or Swift runtime
  implementation change is justified by this failure.

- [x] **Make Swift examples-root MCP compares use the build configuration
  produced by the documented Swift build commands instead of silently choosing
  a stale release binary.** The documented per-example and all-example commands
  in `docs/example_cli_commands.md` call `swift build` without `-c release`, so
  they refresh `.build/debug`; `src/uimd/testing/mcp_tester.py::
  _app_path_from_examples_root()` currently prefers `.build/release`. Define
  one deterministic configuration-selection contract, keep explicit binary
  paths unchanged, and prevent an older artifact in another configuration from
  overriding the documented build output. Add a tester regression with both
  Swift release/debug candidates and different timestamps/content, then run the
  focused root-path C++/Swift `image_browser` probe and the user's complete
  C++/Swift command with `--compare-app-size 90x35`. Audit C#, Rust, Go, and C++
  root resolution so the Swift correction does not change their documented
  build selection. Do not alter reusable-focus rendering, snapshots, waits, or
  example code to hide the stale-artifact mismatch.

  Completed on 2026-08-01. Swift examples-root resolution in
  `src/uimd/testing/mcp_tester.py::_app_path_from_examples_root()` now prefers
  `.build/debug/<name>`, which is the artifact produced by both
  `tools/uimd_dev.py::build_all_swift_examples()` and every documented Swift
  build/compare command. `.build/release/<name>` remains a fallback when no
  debug build exists, while explicit binary paths remain unchanged. The
  regression creates both candidates with different contents and timestamps
  and proves the documented debug output wins. C# still prefers its documented
  Debug build, Rust still prefers its documented release build, and the direct
  Python, C++, and Go candidates are unchanged. No runtime, example, snapshot,
  wait, or reusable-focus behavior was modified.

  Validation passed: all 56 `python/tests/test_mcp_tester.py` tests; a focused
  examples-root C++/Swift `image_browser` probe selected
  `.build/debug/image_browser` and returned `#3a4152` on both targets;
  `./tools/rebuild_all.sh` completed with exit code 0; and the exact documented
  full command `./uimd mcp-test --backend python --headless --all --compare
  cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35`
  passed all 14 scripts with 1,972 assertions, zero failed assertions, and zero
  step failures. In particular, `image_browser_compare.yaml` passed 378
  assertions. `docs/example_cli_commands.md` was already correct and required
  no change.

- [x] **Fix the user-reported Rust TextInput arrow/edit fall-through, complete
  Rust `formular` Save/Cancel behavior, and make FileBrowser mouse activation
  identical across every supported platform.** Reported from direct Rust
  validation on 2026-07-31. Treat Python under `src/uimd/runtime` and
  `src/uimd/dialogs/file_browser.py` as the shared semantic reference, C++
  under `cpp/src/{elements/BasicElements.cpp,generated/GeneratedWindowRuntime.cpp}`
  and `cpp/dialogs/file_browser.cpp` as the structural/native reference, and
  audit Rust under `rust/src/uimd/src/{elements,runtime,dialogs}.rs` plus
  `rust/examples/{formular,text_editor}` before changing behavior.

  Initial audit findings and required contracts:
  - C++ generated-window dispatch forwards every edit-mode key to the active
    element and consumes the event even when `TextInput::handleKey()` returns
    false; Python likewise returns from its edit branch without running
    navigation. Rust `RuntimeState::handle_key()` instead falls through after
    a single-line TextInput rejects Up/Down, then runs spatial focus movement.
    Port the exact C++ consume/notification order for root and ScrollView child
    edit scopes. Up/Down on a single-line TextInput must leave its cursor/value,
    focus, snapshot, and edit mode unchanged. Audit C#, Swift, and Go dispatch
    and change them only if a deterministic test proves the same fall-through.
  - `rust/examples/formular/formular.rs` implements an empty generated event
    trait, unlike `cpp/examples/formular/formular.cpp` and the Python/C#/Swift/
    Go ports. Add only the missing domain behavior through generated override
    hooks: Save records `save`, Cancel records `cancel`, both request close,
    and the final form YAML uses the same fields, selected roles, boolean/
    number formatting, and terminal newline handling as C++. Do not change the
    generated hook contract or add runtime-specific example workarounds.
  - The requested FileBrowser mouse contract is one click on `..` or a
    directory to enter it, while one click on a file only selects/previews it
    and never accepts/closes the dialog. Current C++/Rust concrete dialogs use
    a directory/open-file double-click path; Python/C#/Swift/Go currently rely
    on generic ListBox selection and do not enter a directory on one click.
    Implement one equivalent pre-focused mouse transition in the canonical
    FileBrowser on every supported platform, preserving ListBox row mapping,
    disabled-file behavior, edit/focus state, modal capture, callback order,
    and post-event cleanup. Remove the obsolete double-click acceptance state;
    do not special-case `text_editor` or any other example.

  Focused C++/Rust reproduction on 2026-07-31 exposed the exact Rust modal
  mouse-coordinate divergence behind the remaining FileBrowser failure. C++
  keeps active modal element frames content-local and converts terminal mouse
  coordinates once before the pre-focused callback and ordinary hit testing.
  Rust `render_modal_window_local()` translated every active modal element
  frame to terminal coordinates, while
  `dispatch_mouse_press_before_focused()` converted the pointer back to local
  coordinates only for the callback. The FileBrowser callback therefore
  rejected the directory row and ordinary ListBox routing merely selected it.
  Restore the C++ local-frame/local-pointer contract in the shared Rust modal
  render and complete press/move/release/wheel routes; do not special-case
  FileBrowser coordinates. Protect both one-click directory activation and
  file selection through a real modal runtime route.

  The final source-identity audit found that
  `cpp/examples/text_editor/text_editor.uimd` still carries an older C++-only
  metadata/member-description variant while Python, Rust, Go, C#, and Swift
  share the current canonical source. Replace the C++ copy byte-for-byte from
  `python/examples/text_editor/text_editor.uimd`, regenerate the C++ output,
  rebuild it, and repeat the focused FileBrowser compares; this is source
  synchronization only and must not introduce a C++ layout or behavior fork.

  Required validation: add failing-then-passing runtime/dialog tests for Rust
  root and ScrollView TextInput Up/Down edit retention; add Rust `formular`
  logic and direct-terminal Save/Cancel output coverage; add per-platform
  FileBrowser mouse tests for parent row, child directory, ordinary file, and
  filtered/disabled file, including a mouse press/release route through the
  real runtime. Regenerate and rebuild `formular` and `text_editor` for every
  affected port, run focused C++/Rust compares for both examples and relevant
  compares for any other changed port with `--compare-app-size 90x35`, run
  affected unit tests plus Rust Clippy/direct PTY coverage, audit byte-identical
  `.uimd` sources, and finish with `git diff --check`. Update
  `docs/example_cli_commands.md` if any new test entry point is added.

  Completed on 2026-07-31. The final implementation/audit found four distinct
  causes and removed each at its owning layer:
  - The TextInput Up/Down edit-mode fall-through was Rust-only. Python and C++
    already consume every key offered to the active editor, and the C#/Swift/Go
    audits found the same effective consume behavior. Rust root and active
    ScrollView edit scopes now return from that branch even when a single-line
    TextInput does not move its cursor, preserving focus, edit mode, snapshot,
    value, and cleanup order exactly like C++.
  - Rust `formular` was missing its C++-equivalent domain event overrides and
    terminal result. Save and Cancel now record their action and request close;
    the emitted scalar, boolean, number, selected role/country, CRLF, and
    two-space YAML indentation behavior matches C++ exactly. The final direct
    PTY route caught and removed an initially hidden Rust string-continuation
    indentation difference before completion.
  - FileBrowser's old mouse contract differed on every port. The canonical
    transition is now one click on `..`/a directory to select, preview, hide
    the active overlay, and enter it; one or repeated clicks on a file only
    select/preview it and never accept or close. C++ and Rust obsolete
    double-click timestamp state was removed, while Python, C#, Swift, and Go
    gained the same pre-focused FileBrowser transition in their shared runtime
    path. No example-specific mouse or dialog workaround was added.
  - Rust alone mixed terminal-coordinate modal frames with callback-local
    pointer coordinates. Active modal frames now remain local like C++, and
    terminal press/move/release/wheel coordinates are converted exactly once
    before callback, hit-test, drag, and wheel routing. A real stacked
    FileBrowser regression protects the full route. The source audit also
    found and synchronized an older metadata-only
    `cpp/examples/text_editor/text_editor.uimd` copy; all affected `.uimd`
    sources are now byte-identical to Python.

  Final validation passed: Rust runtime 158/158 tests and runtime Clippy with
  `--all-targets -- -D warnings`; Rust `formular` unit test plus its own Clippy;
  Python application tests 119/119; C++ dialog/runtime tests; Go runtime tests;
  Swift runtime tests 12/12; C# runtime build with zero warnings/errors; fresh
  builds of every affected `text_editor` port, all C++ examples, and all 13
  Rust examples. Focused `text_editor` compares passed Python/C++ with 150
  assertions and C++ versus Rust/Go/C#/Swift with 251 assertions each. Focused
  C++/Rust `formular` passed 239 assertions. The final complete 14-script
  C++/Rust gate passed 1,972 assertions with zero failed assertions or steps;
  Rust direct-terminal smoke passed 8/8 groups including Save/Cancel and exact
  YAML output. Affected `formular`, `text_editor`, and FileBrowser `.uimd`
  hashes match across every stored port; Python syntax checks and
  `git diff --check` pass. No new test entry point was added, so
  `docs/example_cli_commands.md` required no change.

- [ ] **Make the complete Rust runtime and native Rust generator structurally
  and behaviorally 1:1 with C++**. Reopened by user report on 2026-07-28 after
  direct validation showed that TextArea behavior and Sixel rendering still
  differ from C++ despite broad MCP compares passing. Audit only before any
  further implementation: compare Python shared semantics under
  `src/uimd/runtime`, every corresponding public/state/render/input/dialog/MCP
  path under `cpp/{include/ui,src,dialogs}`, the canonical native compiler and
  C++/Rust generators under `cpp/tools/uimd`, and the complete Rust port under
  `rust/src/uimd` plus representative generated outputs. Produce a concrete
  inventory of equivalent, structurally divergent, behaviorally missing,
  primitive-specific, and untested surfaces with exact paths, classes,
  methods, state fields, event order, and validation gaps. Expand this task
  with every discovered difference, required 1:1 remediation, and focused test
  gate, then stop for user review before changing runtime, generator, examples,
  tests, or snapshots. C++ is the structural/native oracle and Python remains
  the shared semantic oracle; Rust-specific logic is allowed only as the
  smallest language/OS adapter around identical public behavior.

  **Audit result (2026-07-28; implementation intentionally not started):**
  Rust is not currently structurally or behaviorally 1:1 with C++. The broad
  C++/Rust MCP compares prove only the routes asserted by those scripts; they
  do not cover the divergent state models and public APIs below. Complete the
  following open tasks in dependency order rather than applying more isolated
  Rust-only fixes:

  - [x] **Replace the monolithic Rust element model with structural equivalents
    of the C++ public element/control hierarchy.** C++ owns the base tree,
    identity, parent/child ownership, commit mode, liveness protection, and
    typed controls in `cpp/include/ui/core/Element.hpp`,
    `cpp/include/ui/app/{Application,Window,Control}.hpp`, and
    `cpp/include/ui/elements/*.hpp`. Rust currently puts nearly every
    control-specific value, cursor, selection, option, image, and child-window
    field into one `Element` selected by `ElementKind` in
    `rust/src/uimd/src/elements.rs`, exposes generic
    `Rc<RefCell<Element>>` members, and has no equivalent parent pointer/live
    element contract or public `Application`/`Window`/`Control` architecture.
    Port the same class responsibilities, state ownership, parent/child
    traversal, identity/liveness checks, typed public methods, and render-state
    roles; keep `Rc`/borrowing only as the smallest Rust memory adapter.
    Validate stale/replaced child handling, typed generated members, public API
    compile tests, focus ownership, and parent-background rendering against
    C++.

    Follow-up audit on 2026-07-28 found two remaining base-element deviations
    after the state variants, typed refs, parent links, identity/liveness
    registry, and public control APIs were ported. Rust `ElementBase` still
    stores `id` as a second qualified-name cache even though C++ stores only
    `Element::name()` and resolves paths through the live tree, and it stores
    an always-true `visible` flag absent from C++ while the MCP contract reports
    visibility from the live element. Remove both duplicate state fields,
    compute qualified IDs during traversal/snapshot routing from existing
    parent/owner relationships, and keep focus/hit/render/MCP behavior 1:1.
    Add replacement/reusable/ScrollView ID regressions plus public snapshot
    coverage before marking this structural task complete.

  - [x] **Port the complete generated-window runtime options and explicit
    window-stack frame model 1:1.** Reference
    `cpp/include/ui/generated/{GeneratedWindowBase,GeneratedWindowRuntime}.hpp`
    and `cpp/src/generated/GeneratedWindowRuntime.cpp`. C++ stores each
    frame's bounds, focused index, edit mode, active ScrollView/edit child,
    suppression flags, edit snapshot, remembered descendants, and per-frame
    callbacks, including pre-focused key, mouse-before-focused, wheel,
    edit-started, focus, close, overlay rendering, and overlay dim policy.
    `rust/src/uimd/src/runtime.rs` instead has a small
    `GeneratedWindowRuntimeOptions`, nested `modal_windows`, a generic
    interaction snapshot, and global `GeneratedApplication::handle_window_*`
    bridges. Port the same frame fields, callback ownership, event ordering,
    cleanup order, top-window routing, modal capture/restore, and overlay
    contract. Remove app-specific active-dialog/class-name dispatch from Rust
    examples after the runtime owns it. Validate modal push/pop, replaced
    descendants, Escape flash, scroll-scope restoration, first returned frame,
    mouse capture, and all post-event cleanup transitions.

  - [x] **Port `TextInput`/`TextArea` state and behavior exactly, replacing the
    partial selection-only correction.** Reference
    `cpp/include/ui/elements/TextInput.hpp` and
    `cpp/src/elements/BasicElements.cpp`; confirm shared semantics in
    `src/uimd/runtime/elements.py`. Rust
    `elements.rs::new_text_input()` currently ignores its `_max_length`,
    `new_text_area()` has no maximum-length argument, and the shared generic
    element lacks C++ `maxLength_`, distinct column/row scroll offsets, and
    `manualRowScroll_`. `handle_text_key()` lacks the C++ Alt+Left/Right
    five-step, Alt+Up/Down three-visual-row, and Alt+Enter transitions.
    Single-line rendering recomputes its visible offset rather than preserving
    the same state and clipping behavior. TextArea rendering computes a local
    row offset while `cursor_for_point()` uses the stored generic offset, so
    wheel scrolling, visible rows, mouse placement, and drag selection can
    disagree; Rust MCP mouse movement also lacks C++ drag-outside auto-scroll.
    Cursor/selection indices must use one C++-equivalent UTF-8 source-index
    contract throughout rather than mixing Rust character indices with
    byte-based visual glyph offsets. Port the exact fields, movement/selection
    state machine, maximum-length enforcement, manual wheel behavior,
    rendering, point mapping, replacement/copy, and edit cleanup. Add
    case-driven unit and real-PTY tests for every C++ key, Unicode, max length,
    non-edit clipping, multiline wheel, drag outside, copy/replacement, and
    re-entry transition.

  - [x] **Port `NumberInput` as its own C++-equivalent control state machine.**
    Reference `cpp/include/ui/elements/NumberInput.hpp` and
    `cpp/src/elements/BasicElements.cpp`. C++ owns value, configurable step,
    edit text/cursor, original value, editing, and replace-first state with
    explicit begin/cancel/commit operations. Rust stores generic number/text
    fields, has no constructor step, hardcodes increment/decrement by one, and
    relies on window-level snapshots for rollback. Port the same fields,
    methods, parsing/formatting, key order, step behavior, blur/confirm/Escape
    transitions, and public API. Validate non-default steps, zero replacement,
    invalid text, mouse blur, and modal/edit cleanup against C++.

  - [x] **Port the missing typed control behavior and render-state surface.**
    `FrameBufferView` in
    `cpp/include/ui/elements/FrameBufferView.hpp` and
    `cpp/src/elements/FrameBufferView.cpp` owns named frames, target order,
    regions, pan, dimming, interaction state, and corner overlays; Rust only
    has an enum value/constructor and currently falls through to ordinary label
    rendering. C++ `InfoLabel` applies its standard animated gradient while
    Rust falls through to the base label renderer. C++ `ViewHost` in
    `ReusableElement.hpp` has a dedicated `setView`/`clearView`/`currentView`
    contract, while Rust stores a generic child window on every element.
    `ElementRenderState::passiveFocus` is also absent in Rust. Port those APIs
    and algorithms and audit every remaining concrete control class, including
    selectable Label behavior. C++ supports one drag selection across multiple
    labels and reusable/ScrollView children through its scroll-selection model;
    Rust tracks only one selection element/anchor. Validate each control's
    public API, default style, state precedence, cross-label copy, reusable
    traversal, animation, pan/dim/overlay, and focus rendering.

  - [x] **Unify and port `ScrollView` as one C++-equivalent type and state
    machine.** Reference `cpp/include/ui/elements/ScrollView.hpp` and
    `cpp/src/elements/ScrollView.cpp`. Rust currently splits ScrollView
    behavior between a generic `ElementKind::ScrollView` in `elements.rs` and
    generated-window-owned rendered/dynamic-child state in `runtime.rs`. It
    does not expose the same rendered/actual/dynamic child model, height-cache
    invalidation, scroll/view/horizontal offsets, content window and child
    views, save/restore contract, item/line/page/top/bottom/horizontal methods,
    wheel line sizing/can-scroll queries, child visibility, or pending terminal
    scroll delta. Port the exact state roles and algorithms into one canonical
    Rust ScrollView and make generated windows consume it. Validate static,
    dynamic, and reusable children, growth/shrink restoration, bottom
    preservation, horizontal range, focus entry/exit, clipping, wheel routing,
    terminal scroll hints, and both parity regression apps.

  - [x] **Port the complete C++ `TerminalBuffer` ownership and presentation
    lifecycle before making another local Sixel-diff patch.** Reference
    `cpp/include/ui/terminal/TerminalBuffer.hpp`,
    `cpp/src/terminal/TerminalBuffer.cpp`, and scroll-region scheduling in
    `cpp/src/generated/GeneratedWindowRuntime.cpp`. C++ owns current and
    previous cells, full-redraw requests, render statistics, whole/region
    diffing, terminal scroll-region updates, and rejects scroll optimization
    whenever raw/raw-skip cells occur in either current or previous state.
    Rust `core.rs::TerminalBuffer` owns only current cells and
    `runtime.rs::write_ansi_frame_diff()` is a free current/optional-previous
    function with no equivalent region diff, render stats, scroll-region hint,
    or pending-delta lifecycle. The earlier raw-anchor fix copied one
    transition but not this architecture, so modal, scroll, resize, raw-image,
    and previous-frame invalidation can still diverge and cause the reported
    iTerm2 artifacts. Port the full ownership, invalidation, synchronized
    update, raw-overlap, region-scroll, resize, and modal frame sequence.
    Validate raw cells in current and previous frames, overlapping text,
    changed/removed anchors, scroll regions, modal open/close, resize, idle
    frames, and bounded emitted bytes.

  - [ ] **Port the complete image/Sixel pipeline and fallback contract.**
    Reference `cpp/include/ui/elements/Image.hpp`,
    `cpp/src/elements/Image.cpp`, terminal cell metrics in
    `cpp/src/terminal`, generated Sixel requirements in
    `cpp/tools/uimd/NativeCppGenerator.cpp`, and the standard fallback warning
    in `GeneratedWindowRuntime.cpp`. Rust has matching broad capability
    heuristics and similar raster/geometry cache keys, but uses only its custom
    encoder rather than the C++ libsixel-plus-fallback path, starts from
    default cell pixels instead of the same ioctl/query order, emits no
    generated `requireSixelForImageRendering` equivalent, and lacks the
    canonical Continue/Abort warning when fallback is required. Combine this
    with the TerminalBuffer port; do not patch `image_browser`, force fallback,
    add delays, or mask screenshots. Validate decoder/encoder parity, cache
    invalidation/eviction, cell metric changes, every crop edge, overlapping
    modal/background images, unsupported-terminal warning flow, real Sixel
    payload metadata, and saved before/after-scroll/focus/modal/resize
    screenshots in iTerm2.

  - [x] **Port terminal backend, event, and clipboard architecture with only
    minimal POSIX/Rust adapters.** Reference
    `cpp/include/ui/terminal/{Input,TerminalBackend}.hpp`,
    `cpp/src/terminal/{Input,TerminalBackend,Clipboard}.cpp`, and generated
    runtime dispatch. Rust's private event type and monolithic POSIX guard/input
    loop do not mirror the C++ backend abstraction and complete event model.
    Rust waits to disambiguate standalone Escape while C++ dispatches through
    its parser/backend framing, toggles terminal autowrap differently, and
    combines terminal-pixel queries with entry rather than the C++ detection
    order. Rust clipboard command coverage omits C++ `xsel` and macOS
    `osascript` write fallbacks and reads the external pasteboard back into
    runtime state where C++ uses its internal clipboard contract. Preserve the
    already-matching CSI/SS3/modifier/paste/mouse/pixel parser cases, but port
    the same logical events, dispatch order, terminal entry/teardown bytes,
    resize/focus lifecycle, clipboard chain, and copy/paste state. Explicitly
    document and test any unavoidable lone-Escape timing adapter.

  - [x] **Port core color/text-visual/layout public contracts exactly.**
    Reference `cpp/include/ui/core`, `cpp/src/core/{Color,TextVisual}.cpp`, and
    the public layout types. Rust color parsing accepts only long hex forms,
    so valid C++ `#RGB` values differ. Rust exposes `visual_glyphs` and a
    zero-column `visible_width`, but lacks C++ `safeTerminalCellText`,
    `visibleText`, and start-column-aware width APIs; this also contributes to
    inconsistent TextArea source indices. Port validation, short/long
    hex/alpha/named-color semantics, safe terminal-cell text, visible text,
    source offsets, tab/wide/combining behavior, and corresponding public
    layout/tree responsibilities. Prove generated layout resolution against
    C++ case by case rather than relying only on final snapshots.

  - [x] **Generate and own standard dialogs through concrete Rust dialog
    objects and the universal stack.** The shared dialog `.uimd` sources are
    already present, but C++ concrete behavior lives in
    `cpp/dialogs/{message_box,file_browser}.{hpp,cpp}` with object-owned
    callbacks, result/closed state, per-frame runtime options, FileBrowser
    double-click routing, and overwrite flow. `rust/src/uimd/src/dialogs.rs`
    returns bare `GeneratedWindow` values plus free handler functions, and Rust
    examples inspect active class names and manually route
    `handle_window_*` callbacks. Port concrete MessageBox/FileBrowser
    responsibilities and callback/result state; move double-click,
    directory/file commit, Escape, overwrite, and nested-modal behavior into
    the canonical dialog/runtime path and remove all example bridges. Validate
    keyboard, mouse, double click, open/save/overwrite, Escape flash, nested
    modal return, MCP, and direct-terminal behavior.

  - [x] **Port the complete concurrent C++ MCP runtime lifecycle, not only its
    tool inventory.** Basic Rust tool names, schemas, aliases, and
    stdio/TCP/HTTP request routes are broadly equivalent under current tests.
    C++ nevertheless has GUI/headless selection, action/type delays,
    wait-render/controlled-render options, recursive UI synchronization,
    condition-variable/render-generation state, full-redraw coordination, and
    concurrent background transports. Rust's smaller config omits those
    controls and branches into `serve_mcp()` instead of running MCP alongside
    an interactive UI; its listener mutates one app serially without the same
    shared render-generation contract. Port the configuration, synchronization,
    server/application lifecycle, observation points, active top-window
    routing, and post-tool cleanup order. Add C++-case-driven protocol/state
    tests for GUI plus MCP, headless, controlled rendering, delays, batch and
    notification requests, concurrent clients, modal tools, edit/selection
    cleanup, and app tools.

  - [x] **Make the native Rust emitter consume the same canonical compiled
    model and property semantics as the C++ emitter.** The CLI remains
    correctly owned by `cpp/tools/uimd`, but
    `NativeRustGenerator.cpp::compileRustFile()` reparses a `NativeDocument`
    and independently duplicates style/theme/metadata/MCP/event/member
    interpretation instead of consuming the
    `CompilerDocument`/dependency model used by
    `NativeCppGenerator.cpp`. Concrete drift already exists: C++ reads
    `maxlength`, while Rust reads nonexistent `max-length` and then ignores
    it; C++ emits NumberInput `step_size`/`step`, while Rust emits no step;
    Rust accepts fewer Image property aliases; single-file Rust generation
    does not resolve and instantiate `uses` dependencies like C++; generated
    members are all generic `ElementRef`; and no generated Sixel-requirement
    call is emitted. Extract/reuse one backend-neutral native compiler model and
    shared property/alias/dependency semantics, then keep only Rust syntax and
    build-file emission target-specific. Validate directory and single-file
    dependency generation, typed reusable children, every member property and
    alias, metadata/MCP/app tools, relevant event hooks, Sixel requirements,
    scaffold/local/installed SDK builds, and representative generated diffs.

  - [ ] **Close the structural coverage gaps before calling Rust complete
    again.** Existing Rust tests and full MCP compares do not exercise maximum
    length, NumberInput step, Alt text movement, manual TextArea wheel/drag,
    Unicode source indices, cross-label selection, default InfoLabel animation,
    FrameBufferView, ViewHost, short hex, complete ScrollView APIs,
    current/previous raw terminal-scroll interaction, dialog object ownership,
    generator single-file dependencies, or concurrent GUI+MCP behavior. Add
    C++-case-driven Rust unit/API tests, direct PTY cases, protocol tests, and
    real iTerm2 visual routes for those exact surfaces. After all structural
    tasks pass, regenerate and build both C++ and Rust for every affected
    example, then run Rust tests/Clippy, native generator/SDK parity, all
    C++/Rust example compares and both existing Rust regression compares with
    `--compare-app-size 90x35`, complete direct-terminal and MCP transport
    smoke, real Sixel screenshots, and `git diff --check`. A green snapshot
    gate alone must never be accepted as proof of structural parity.

  Audited surfaces that are already equivalent under current coverage, and
  must remain unchanged while the gaps above are fixed: canonical native CLI
  registration and SDK target discovery for `rust`; byte-identical shared
  example/regression `.uimd` sources; the basic MCP tool/schema inventory and
  stdio/TCP/HTTP baseline; CSI/SS3/modifier/bracketed-paste/SGR-mouse/pixel
  parser cases; Sixel capability heuristics and geometry/cache-key shape; and
  Rust ports of both currently present parity regression apps. These are
  validated baselines, not evidence that the complete port is 1:1.

  **Remediation audit findings (2026-07-28; implementation in progress):**
  Direct method-by-method comparison of C++
  `cpp/include/ui/elements/ScrollView.hpp` /
  `cpp/src/elements/ScrollView.cpp` with Rust
  `rust/src/uimd/src/elements.rs::ScrollViewState` found two contracts that
  broad MCP snapshots did not exercise. C++ `contentWindow()` returns a valid
  zero-skip window for an empty ScrollView, while Rust returned `None`; Rust now
  returns the same `viewport`, `actual_skip=0`, `natural_skip=0`, and `gap=0`
  record and has a focused lifecycle regression. C++ `setGap()` always
  invalidates the height cache after assigning the clamped gap, while Rust
  returned early for an unchanged value; remove that early return so dynamic
  child replacement cannot retain stale measurements and protect the exact
  invalidation order with a Rust state test. These fixes affect only the Rust
  port because the Python/C++ reference contracts are unchanged.

  Direct NumberInput comparison against
  `cpp/src/elements/BasicElements.cpp::NumberInput` also found that Rust
  `format_number()` used Rust's shortest round-trip `f64` display instead of
  C++ `std::setprecision(12)`/`defaultfloat`, and Rust commit required the
  complete edit string to parse while C++ `std::stod` accepts the leading
  numeric prefix. Port the 12-significant-digit fixed/scientific threshold,
  exponent spelling, negative zero/non-finite spelling, and prefix parse
  behavior into the shared Rust control. Add exact cases for high precision,
  large/scientific values, rounding across the exponent boundary, and a
  trailing malformed numeric suffix.

  Core color comparison against `cpp/src/core/Color.cpp::blendOver()` found
  that Rust `rust/src/uimd/src/core.rs::Color::blend_over()` checked for an RGB
  background before handling foreground alpha zero. Consequently
  `#RRGGBB00` over a named, transparent, or unset background returned the
  foreground in Rust, while C++ returns the background immediately. Restore
  the identical alpha-first decision order and cover non-RGB background cases;
  the C++ and Python reference implementations are unaffected.

  Terminal backend comparison against
  `cpp/src/terminal/TerminalBackend.cpp` and
  `cpp/include/ui/terminal/TerminalBackend.hpp` found Rust constants and drain
  behavior that were not 1:1: terminal pixel queries used a 100 ms total
  deadline and 256-byte response instead of the C++ per-read 50 ms timeout and
  64-byte cap; input used 4096-byte reads without the C++ 16384-byte per-drain
  cap; and the poll interval was 16 ms instead of the shared 10 ms idle
  interval. Match those limits and loop boundaries. Keep one explicit minimal
  POSIX/Rust adapter: Rust retains a 50 ms pending-Escape continuation window
  so split CSI/SS3/modified-key sequences remain one logical event; C++'s
  parser consumes a lone Escape immediately when one read contains only that
  byte. Direct split-sequence and standalone-Escape tests protect identical
  resulting logical events despite this unavoidable framing/timing adapter.

  Method-level `InputParser` comparison found further fixable event-shape
  drift. Rust treated any SGR button with bit 64 as a wheel event instead of
  C++'s exact button values 64/65; accepted direct Delete/PageUp/PageDown and
  tilde Home/End sequences absent from C++; limited CSI-u Enter to modifiers
  3/5 although C++ maps every non-default modified Enter to `Alt+Enter`; mapped
  backspace byte 0x08 and discarded other control bytes where C++ forwards
  their one-byte key values; and consumed an unknown complete SS3 sequence
  instead of emitting Escape and leaving its suffix for ordinary parsing.
  A second pass over the modified-key branches found that Rust also accepted
  legacy CSI `Cmd+C`/`Cmd+V` forms that C++ deliberately does not, omitted the
  exact legacy `Alt+Enter` forms `ESC[27;3;13~` and `ESC[27;5;13~`, accepted
  CSI-u payloads with trailing fields, and treated CSI-u codepoint 3 with the
  no-modifier value as Ctrl+C. Restore the exact strict field parsing,
  no-modifier rejection, legacy Ctrl+C-only rule, and explicit legacy
  Alt+Enter cases, then add case-driven regressions. Retain only two explicit
  Rust framing adapters: valid UTF-8 input is emitted as one Rust `String` key
  rather than invalid one-byte string fragments, and the already-recorded
  50 ms pending-Escape window preserves split terminal sequences.
  A further SGR/input-buffer pass found that Rust parsed mouse fields with
  `filter_map`, so an invalid or extra field could be discarded and the
  remaining three integers emitted as a valid event; C++ consumes such a
  complete malformed report without an event. Rust also cleared its parser at
  an arbitrary 1 MiB pending-input limit absent from C++. Require exactly three
  fully parsed fields and remove the target-only reset; cover invalid middle,
  missing, and extra fields while retaining complete-report consumption.

  Terminal pixel-response parsing also used Rust's strict integer/split
  parsing, while C++ `parseTerminalPixelResponse()` extracts the first
  height/width fields and uses `std::stoi`, accepting leading whitespace/sign
  and a numeric prefix. Mirror the same field boundaries and prefix parse so
  cell-size detection consumes identical terminal reports; add malformed,
  signed, prefixed, and embedded-response cases.

  POSIX terminal-mode comparison found that Rust used `cfmakeraw()`, which
  clears a broader set of termios flags than C++
  `TerminalModeGuard`, and restored the saved flags verbatim. C++ clears only
  `ECHO|ICANON|IEXTEN|ISIG`,
  `BRKINT|ICRNL|INPCK|ISTRIP|IXON`, and `OPOST`, sets `CS8`, and forces
  `OPOST|ONLCR` for both normal and signal restoration. Port those exact masks
  and restore order; retain Rust's signal-handler storage only as the minimal
  POSIX ownership adapter.

  FileBrowser comparison against `cpp/dialogs/file_browser.cpp` found four
  observable Rust state-order differences. Rust accepted a non-empty filename
  before processing the selected ListBox directory/`..` row, so Save/Open could
  select a file where C++ navigates first; it allowed manually typed nested
  paths instead of requiring the resolved parent to be the current directory;
  `refresh_entries()` forced the ListBox offset to zero after C++-equivalent
  selected-row visibility had been established; and edit-start cursor placement
  used Unicode scalar count even though TextInput public indices, like C++, are
  UTF-8 source-byte offsets. Port C++ selected-path-first acceptance, current
  directory validation, preserved selected-row scrolling, and byte-end cursor
  placement. Also add the missing object-owned FileBrowser close callback,
  fired with the selected path or an empty cancellation result exactly when
  the concrete dialog reaches a terminal outcome. Cover parent navigation with
  typed text, nested-path rejection, long selected-row visibility, Unicode
  cursor placement, and callback delivery.

  The same FileBrowser audit exposed a shared ListBox state-order deviation in
  `rust/src/uimd/src/elements.rs`: Rust `set_selected_index`,
  `set_selected_indices`, and `set_active_index` used the layout frame height
  before any render, while C++ `ListBox` changes scroll offset only when
  `lastViewportHeight_` was established by rendering. Remove the frame-height
  shortcut so pre-render setters and the subsequent render-time visibility
  repair occur in the same order as C++; update the existing Rust regression
  to assert both phases.

  A follow-up ScrollView lifecycle audit found two remaining ordering
  differences in `rust/src/uimd/src/elements.rs`. C++
  `ScrollView::clearChildren()` removes rendered children before native
  elements and only then resets offsets/cache state, while Rust cleared the
  native element tree first. C++ `restoreScrollPosition()` also refreshes
  dynamic children once at the raw frame width before calculating the padded
  content viewport; Rust began directly at the padded width. Mirror both
  sequences exactly and add a dynamic-renderer width/order regression plus a
  clear-state/native-liveness regression.

  Direct control-render comparison against
  `cpp/src/elements/{BasicElements,InfoLabel,MessageTable}.cpp` found that Rust
  forced every non-MessageTable width to one when a caller requested
  fit/natural width (`size.width <= 0`). C++ derives natural widths per
  concrete control: visible label width, title plus button/ComboBox/ListBox
  affordances, complete CheckBox text, TextInput caret space, current
  NumberInput display text, and byte length for InfoLabel. Rust also clipped a
  parsed MessageTable to the requested height even though C++ always returns
  its complete table rows. Port those concrete size contracts and protect each
  control with a public render-size regression, including Unicode and a table
  whose requested bounds are smaller than its natural output.

  The same direct-render pass found that C++
  `NumberInput::render(..., editMode=true)` calls `ensureEditText()` even when
  no generated-window transition called `beginEdit()` first. Rust rendered the
  display value without establishing the edit snapshot because
  `Element::render_with_state()` borrowed the control immutably. Preserve the
  const-render public contract with the smallest Rust interior-mutability
  adapter for NumberInput edit state, so direct and generated rendering enter
  the same original-value/text/cursor/replace-first state exactly once.

  TextInput key/API comparison found that Rust accepted a `"Delete"` key in
  `TextInputState::handle_key()` although C++ `TextInput::handleKey()` has no
  forward-delete branch and the C++ terminal parser deliberately does not emit
  Delete. Remove the Rust-only behavior so MCP/direct public key dispatch also
  matches, and cover the unchanged value/cursor result. Keep one explicit
  language-safety adapter for UTF-8: public cursor/selection values remain
  source-byte offsets like C++, but Rust moves, deletes, and clamps at complete
  scalar boundaries because a Rust `String` cannot represent the invalid UTF-8
  that C++ can temporarily create by erasing one byte. Validate identical
  ASCII behavior plus valid Unicode selection/render/copy behavior.
  The vertical-cursor helper also selected `target.cells[column].source_start`
  directly, while C++ calls `rawIndexForVisualColumn()` so the second terminal
  cell of a wide glyph resolves to its source end. Use the same midpoint helper
  and add a two-row wide-glyph movement regression.

  Base-element constructor comparison found that C++ `Element` initializes
  `focusable_ = true` for every concrete type and `ReusableElement::setChild`
  / `ViewHost::clearView` never rewrite that public property; focus routing
  separately filters concrete types and child-window capabilities. Rust
  initialized focusability from `ElementKind` and rewrote it whenever a child
  view changed. Use the same always-true base default and preserve explicit
  caller changes across set/replace/clear; keep the existing runtime type and
  generated-child filters as the behavioral gate. Cover disabled reusable
  replacement, generated ScrollView proxy focus, ViewHost exclusion, and
  public property stability.

  Generated-window stack comparison against
  `cpp/src/generated/GeneratedWindowRuntime.cpp::GeneratedWindowStack::push()`
  found that a C++ frame with `startInEditMode=true` and no explicit initial
  focus selects the first focusable element before enabling edit mode. Rust
  `GeneratedWindowStackFrame::new()` left `focused_index=-1` while setting
  `edit_mode=true`. Match the root-runtime and C++ frame initialization rule
  and cover both the implicit-first-focus and explicit-name cases.

  The same startup audit found a separate root-runtime difference. C++
  `runGeneratedWindow()` enables initial edit mode only when the resolved
  element satisfies `isEditableElement()` and immediately calls
  `onEditStarted`; Rust `RuntimeState::new()` enabled it for any focused type
  and the Rust terminal/headless entry points emitted no initial callback.
  Use the same enabled/concrete/reusable-ScrollView editability predicate,
  initialize the edit snapshot only for that case, and dispatch the initial
  callback exactly once from interactive, headless MCP, and non-TTY entry
  paths. Protect an editable TextInput and a non-editable Button case.

  Making the Rust base `focusable` property match C++ exposed a hidden MCP
  mouse-routing dependency: `perform_mouse_press()` used `focusable=false` to
  recognize a non-control child inside a selectable ScrollView. C++ keeps base
  focusability true and makes this decision from its concrete
  `isFocusableType()`/`mouseTargetElements()` contract, so a Label child begins
  cross-row ScrollView selection while a Button/TextInput child remains an
  interactive target. Change Rust routing to the same concrete-type decision
  and retain the existing cross-child selection regression.

  Selectable-Label point mapping comparison against
  `cpp/src/elements/BasicElements.cpp::Label::textPositionFromPoint()` found
  two Rust deviations in `rust/src/uimd/src/elements.rs::cursor_for_point()`.
  Rust returned `source_start` for every terminal cell of a wide glyph instead
  of using the C++ glyph-run midpoint rule, and its one-row path mapped through
  a word-wrapped visual row while C++ maps the unwrapped first logical line.
  Use the same raw-index helper and distinct one-row/multiline row construction;
  protect left/right halves of a wide glyph, aligned text, a long line with a
  wrap opportunity, and out-of-row points.
  The same API pass found that Rust `set_text()` and `set_spans()` cleared
  Label selection endpoints, while C++ `Label::setText()`/`setSpans()` retain
  them and clamp only when selected text is read. Preserve selection state
  across both content setters and cover shorter replacement text and span
  replacement.

  Text-visual predicate comparison against
  `cpp/src/core/TextVisual.cpp::isUnsafeTerminalCodepoint()` found that Rust
  `core.rs::is_unsafe_terminal_codepoint()` used Unicode `char::is_control()`,
  rejecting the complete C1 range even though C++ rejects only C0, DEL, and
  its explicit combining/format/wide ranges. Use the identical numeric
  predicate so safe-cell text, visible text/width, tabs, and source offsets
  agree; cover DEL, a C1 code point, combining text, and wide text.

  Base-tree ownership comparison against
  `cpp/src/core/Element.cpp::Element::addChild()` found that Rust accepted an
  `Rc` child already owned by another parent and could re-register a stale
  child after `clearChildElements()`. C++ `unique_ptr` ownership makes both
  states impossible. Keep `Rc` only as the memory primitive adapter: adding a
  child must require a live element with no current parent, establish exactly
  one parent link, and never resurrect an unregistered stale tree. Add
  duplicate-parent, stale-readd, normal add/clear, parent-link, and liveness
  regressions.

  Element style-state comparison against
  `cpp/src/core/Element.cpp::effectiveStyle()`/`mergedStateStyle()` found that
  Rust pre-blended a partially transparent base background over the rendering
  parent even when no disabled/edit/focus state style was applied. C++ performs
  that parent blend only inside an actual state-style merge; otherwise the
  base alpha remains for the later framebuffer composition. Move the Rust
  parent blend to the identical merge point and cover base-only, focused with
  an empty state style, disabled precedence, RGB parent, and non-RGB parent.

  The optional-style migration exposed one additional state-precedence
  difference in `rust/src/uimd/src/elements.rs::Element::effective_style()`.
  C++ returns the disabled layer only when the element is disabled *and* a
  disabled style exists; otherwise it continues to edit and focus style
  checks. Rust entered an unconditional disabled branch and skipped edit/focus
  even when `disabled_style` was absent. Port the exact conditional return
  order while converting state colors to `Option<Color>`, and protect disabled
  elements with and without a disabled layer plus simultaneous edit/focus
  states.

  NumberInput public edit-text comparison against
  `cpp/src/elements/BasicElements.cpp::NumberInput` found one required
  language-safety adapter. C++ accepts an arbitrary byte string through
  `setEditText()` and moves/erases one byte at a time; Rust accepts a valid
  Unicode `String`, but `set_number_edit_cursor()`, Left/Right, Backspace, and
  edit rendering currently permit a cursor between UTF-8 code-unit boundaries
  and then panic in `String::remove()` or slicing. Preserve the public
  C++-equivalent UTF-8 source-byte cursor contract while clamping and moving at
  complete scalar boundaries, as already required for Rust TextInput. Add
  direct public-setter, cursor movement, Backspace, and edit-render regressions
  proving valid Unicode remains representable and no panic occurs; ordinary
  ASCII numeric editing must remain byte-for-byte equivalent to C++.

  A public element API pass against `cpp/include/ui/core/Element.hpp` found
  that Rust's typed control references expose the base name/identity,
  commit/focusable/enabled/frame/base-style/render methods, but omit direct
  forwarding methods for effective style, every state-style setter/getter,
  parent/children traversal, generic child insertion, and child clearing.
  The underlying `Element` already owns the corresponding canonical state and
  behavior, so add only thin typed-reference forwards rather than another
  implementation. Protect generated-member compile coverage plus generic
  parent/child ownership and state-style/effective-style calls on representative
  typed controls.

  A further TextArea point-mapping pass against
  `cpp/src/elements/BasicElements.cpp::TextInput::cursorForPoint()` found that
  Rust clamps only the final visual-row index. C++ first clamps the requested
  local row into the viewport height, adds the current row-scroll offset, and
  returns the value end when that target lies below the available visual rows;
  Rust can instead jump to the final content row and place the cursor inside
  it. Mirror the exact viewport-row and below-content order and add cases for a
  point below the viewport, a short document below its final row, and a
  manually scrolled multiline value.

  TextInput maximum-length replacement comparison found one remaining ASCII
  state-order difference. C++ rejects a one-byte key only when the current
  value is already at/over `maxLength` and there is no selection; with a
  selection it deletes the range and inserts the key even when a public setter
  previously supplied an over-limit value. Rust instead rejected from the
  calculated resulting byte length. Restore the C++ pre-deletion check for
  ASCII keys and add an over-limit setter/selected-replacement regression.
  Retain the smallest UTF-8 safety adapter for a multi-byte Rust scalar: reject
  it when the resulting valid `String` would cross the byte limit rather than
  reproduce C++'s intermediate invalid byte string.

  Concrete-dialog public/lifecycle comparison against
  `cpp/dialogs/{message_box,file_browser}.{hpp,cpp}` found that Rust concrete
  message boxes and FileBrowser have no direct `run()` entry point, and the
  Rust FileBrowser wrapper omits the C++ public refresh/accept/select,
  directory-query, mouse, filename-cursor, close, and stack-frame option
  methods even though its internal state implements most of them. Add thin
  object-owned forwards around the same generated window/state, without a
  second dialog implementation. The mouse audit also found an observable
  double-click ordering bug: entering a directory returns the internal
  `Pending` outcome, so Rust reports the press unconsumed and may continue
  routing it, while C++ `handleEntryMousePress()` returns the successful
  `acceptCurrent()` result. The same path calls Rust's ScrollView-only
  `scroll_offset()` accessor on the FileBrowser ListBox and panics as soon as
  the direct mouse route reaches it; use the ListBox offset exactly like C++.
  Preserve a separate accepted/consumed result from the outcome and cover
  single click, directory double click, filtered-file double click,
  callback/close state, stack frame callbacks, and standalone `run()`
  construction.

  Core public-surface inventory found no Rust equivalent of C++
  `cpp/include/ui/core/Version.hpp::runtimeVersion()`, even though the Cargo
  package carries the synchronized SDK version. Export the package version
  through the same public concept from the canonical Rust crate and add a test
  tying it to Cargo's package metadata so generated applications and
  diagnostics cannot report a stale or independently hardcoded version.
  The repeated audit on 2026-07-29 found that the first remediation used the
  shorter Rust name `version()` instead of the structural Rust equivalent
  `runtime_version()`. Rename the public function and its exact metadata test;
  no compatibility alias is needed because the Rust target is still
  uncommitted. Required validation: focused Rust public API test, complete
  runtime tests, Clippy, generated example builds, and native SDK parity.

  Image decoder comparison against
  `cpp/src/elements/Image.cpp::loadRaster()` found that C++ delegates to the
  canonical `stb_image` implementation and therefore accepts its complete
  JPG/PNG/TGA/BMP/PSD/GIF/HDR/PIC/PNM surface, while
  `rust/src/uimd/src/image.rs::load_image_raster()` independently chains custom
  PNG/JPEG/GIF/BMP/TGA/PNM decoders and cannot load PSD, HDR, or PIC. Replace
  the duplicate Rust decoding pipeline with the same stb decoder contract,
  keeping only a minimal safe Rust wrapper that converts stb RGBA output into
  the existing raster state. Add in-memory format probes for the shared
  formats plus PSD/HDR/PIC coverage, invalid-data behavior, alpha preservation,
  and a generated image-example build/render regression; the C++ decoder is
  unchanged.

  MCP configuration/state comparison against
  `cpp/src/generated/GeneratedWindowRuntime.cpp::{McpRuntimeConfig,
  parseViewportArg,McpController}` and
  `cpp/src/terminal/Clipboard.cpp` found that Rust
  `rust/src/uimd/src/mcp.rs::McpRuntimeConfig` stores one optional viewport
  plus four duplicate row/column/width/height fields, hardcodes an implicit
  90x35 viewport absent from C++, adds a noncanonical `--mcp-viewport WxH`
  argument, and stores a second MCP-only clipboard string. Rust also silently
  replaces malformed numeric arguments with defaults, while C++ parses one
  optional `Rect` from `--viewport` or the four component flags and reports
  invalid values as startup errors; its delays/port retain signed integer
  semantics and its copy/paste tools use the centralized runtime clipboard.
  Remove the duplicate Rust fields and argument, port the exact parse/default/
  clamp/error order, derive coordinates and render sizes from the single
  optional viewport, and route copy, cut, mouse selection, and paste through
  the shared clipboard implementation. Update the Rust MCP transport smoke to
  use canonical `--viewport 0,0,width,height`. Add configuration/API tests for
  an absent viewport, combined and component forms, malformed and signed
  numbers, canonical delay/port behavior, runtime `set_viewport`, coordinate
  translation, and clipboard sharing; C++ and Python are unchanged.

  A method-level MCP text-tool pass against
  `cpp/src/generated/GeneratedWindowRuntime.cpp::McpController::{toolSetText,
  toolTypeText}` found that Rust
  `rust/src/uimd/src/mcp.rs::dispatch_tool()` implements `paste_text` by
  sending every Unicode scalar through `RuntimeState::handle_key()`. C++
  performs one `TextInput::insertText()` operation or one
  `NumberInput::setValue(std::stod(...))` operation, emits one changed
  callback/progress generation, and accepts the same numeric prefix as
  `std::stod`. Port `paste_text` as its own branch with the exact per-control
  operation and notification order; retain scalar-safe insertion as the
  smallest Rust string adapter. Protect final value, selection replacement,
  max length, numeric prefix/error behavior, edit-start count, text-change
  count, and render-progress count. Keep `type_text` character-by-character
  and `replace_selection` aliased to that path like C++.

  Continuing the method-level MCP audit found additional observable Rust
  deviations in `rust/src/uimd/src/mcp.rs` against the corresponding
  `McpController::tool*` methods in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`. Rust merged
  `activate_element` and `click_element` even though C++ activation uses the
  semantic control path while click performs a frame-centre mouse press and
  its modal/focus cleanup; Rust's edit preparation for `clear_text` and
  `move_cursor` can send a synthetic Enter to a non-editable target before
  returning an error, while C++ only sets MCP edit state; NumberInput
  `press_key cmd_v` reads the generic text value and can panic; checkbox
  text-change callbacks use lowercase instead of C++ `True`/`False`;
  `select_text` reports clamped endpoints instead of the requested C++
  endpoints; `get_focused_element` can expose a hidden element;
  `get_text_snapshot` renders the full stack instead of only the active
  window. Port each method's target validation, state/callback/cleanup order,
  result shape, and viewport scope exactly. Add focused protocol/state
  regressions for invalid controls,
  modal click versus activation, numeric clipboard paste, callback spelling,
  out-of-range selection, hidden focus, and active-modal text snapshots before
  considering the MCP lifecycle task complete.

  The same pass found that Rust's common element snapshot/schema contract is
  structurally different even before individual tools run. C++
  `elementType()`, `elementRole()`, `elementValueJson()`,
  `elementCapabilitiesJson()`, and `McpController::snapshot()` omit
  unsupported optional fields, return null for elements without a value, use
  the public type names `label`/`scrollview`/`element`, and treat Image as a
  text-role value. Rust always emits `name`, a duplicate `rect`, null
  cursor/selection and empty options fields, invents target-only type names
  such as `spanlabel`, `framebufferview`, `uiscrollview`, and
  `uielementreusable`, gives every non-reusable element `ui.get_value`, and
  assigns Image the action role. Port the exact conditional JSON shape,
  type/role/value mapping, and capability derivation; protect representative
  Label/InfoLabel/FrameBufferView/Image/ScrollView/Reusable/ViewHost plus
  editable and selection controls with full-object equality tests against the
  C++ contract.

  Reusable-control activation comparison found another structural MCP/direct
  input gap hidden by the broad image-browser snapshots. C++
  `GeneratedWindowBase::activateGeneratedControl()` remains a virtual
  capability on the concrete child object, and
  `GeneratedWindowRuntime.cpp::activateReusableControl()` invokes it before
  centre-point mouse routing for both `activate_element` and `click_element`.
  Rust `GeneratedWindow` has no equivalent capability; its
  `RuntimeState::activate_focused()` first sends the reusable's qualified name
  to the root application and otherwise enters child descendants, while
  `mcp.rs::click_element` routes directly to the child at the reusable's
  centre. Rust `image_browser` compensates by recognizing root reusable-name
  prefixes in `handle_root_button()`, rather than retaining the C++ child-owned
  activation callback. Add one object-owned
  `GeneratedWindow::activate_generated_control()` contract, make reusable
  keyboard/mouse/MCP activation consume it in the same order, and migrate the
  Rust focusable image controls to that capability without duplicating general
  routing in the example. Protect handled/unhandled child activation,
  centre-click interception, refocus/cleanup, and a generated focusable
  reusable compile/runtime route against C++.

  Getter traversal comparison found two more Rust MCP contract gaps. C++
  `toolGetState()` delegates its focused value to
  `toolGetFocusedElement()`, so an unexposed focused element remains null;
  Rust fixed the standalone getter but `state_snapshot()` still serializes that
  hidden element. C++ `toolGetElements()`/`toolGetSchema()` also descend from a
  `ScrollView` native child only through each reusable child's generated
  window; Rust `exposed_elements()` descends ordinary reusable child windows
  but never visits reusable rows owned by `Element::children()`. Reuse one
  exposed-focused helper for both results and mirror the exact ScrollView
  reusable-child traversal without exposing the row proxy itself. Add state
  equality for hidden focus and dynamic ScrollView row elements/schema.

  MCP focus-state comparison found that Rust still applies the ordinary
  keyboard edit-exit transition before `focus_element`,
  `activate_element`, and changed-target text tools. C++
  `focusActiveWindowElement()` does not commit or exit an ordinary root edit;
  it commits/notifies only an active ScrollView edit child while leaving that
  scope, and `focusActiveWindowElementWithScrollViewScope()` directly retargets
  a contained control without that commit. Rust therefore emits extra
  confirmation/change callbacks and clears edit state/snapshots at different
  points. Port both C++ focus helpers as distinct Rust MCP transitions and use
  them in the same tool paths. Cover root edit retargeting, leaving a scoped
  child, contained activation, callbacks, edit snapshot, and final scope state.

  `get_image_render_info` comparison found that C++ captures every live
  element frame, renders only to resolve the queried image bounds, and restores
  all frames on success or exception before computing/returning render info.
  Rust calls the ordinary MCP renderer and leaves its layout-frame mutations
  behind. Add the same recursive unwind-safe frame guard around this getter and
  protect root, reusable, and ScrollView child frames from query side effects;
  image geometry and payload metadata must remain unchanged.

  `press_key cmd_v` comparison found that C++ enters edit mode and emits the
  edit-start callback for any focused `isEditableElement()` (including
  ComboBox, ListBox, ScrollView, and a generated ScrollView proxy) before
  attempting the TextInput/NumberInput-only paste. Rust performs edit
  preparation only for text/number controls and routes it through the normal
  Enter transition. Mirror the C++ capture/begin/edit/notify order without
  entering a ScrollView scope, while preserving actual paste/change callbacks
  only for text and number inputs. Add non-text editable and generated-proxy
  regressions plus numeric newline filtering.

  Mouse capture/hit-test comparison found that Rust has no C++
  `MouseClickCandidate` state. `perform_mouse_press()` recursively returns any
  framed child and immediately dispatches Image activation, whereas C++
  builds `mouseTargetElements()` from concrete focusable types/reusable
  proxies, records a clickable Image candidate on press, cancels it after
  movement or an outside release, and dispatches only on a valid release.
  Port the exact target-list and candidate lifecycle for direct terminal and
  all MCP mouse tools, including reusable-generated-focusable proxy selection,
  nonselectable labels, press/move/release cancellation, modal-local
  coordinates, and owner-aware release dispatch.

  The focused `image_browser` mouse route exposed a second structural
  hit-testing dependency hidden by the earlier target inventory. C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp::{scrollViewAtPosition,
  mouseTargetElements,syncReusableChildFrames,syncWindowElementFramesTo}`
  resolves the actual generated `ScrollView` behind its reusable proxy and
  synchronizes every reusable/ScrollView descendant to absolute frames before
  testing the pointer. Rust `rust/src/uimd/src/mcp.rs` passed the proxy back
  into the nested target walk and `rust/src/uimd/src/runtime.rs` had no
  corresponding non-rendering frame synchronization, leaving dynamic row
  buttons in local coordinates. Port the same fullscreen fit-pass frame
  resolution, recursive reusable ownership, visible child-view translation,
  and hidden-child zero frame; use the generated `ScrollView` only for the
  nested target inventory while retaining Rust's documented proxy as the
  generated scope handle. Protect the local-to-absolute nested-button frame,
  inactive-scope pointer entry, owner-aware dispatch, and the complete
  `image_browser` click/modal route.

  The viewport audit also exposed a shared Rust layout-size deviation.
  C++ `generatedWindowContentSize()` resolves the runtime tree, recomputes
  fit-content heights at the resolved width, resolves it again, and returns
  the maximum resolved cell extent; both `activeWindowReportedSize()` and
  `windowBounds()` consume that result. Rust
  `rust/src/uimd/src/runtime.rs::{active_window_reported_size,window_bounds}`
  used only the first `natural_content_size()` tree measurement even though
  `resolve_runtime_cells()` already implements the canonical second pass.
  Add one canonical Rust `generated_window_content_size()` with the same
  resolved-extent algorithm and use it for reported sizes, bounds, and the MCP
  no-viewport fallback. Protect wrapped fit-content growth, expanded modes,
  borders, and the no-explicit-viewport MCP render/get-viewport contract.

  Core style-structure comparison against
  `cpp/include/ui/core/Style.hpp` found that Rust
  `rust/src/uimd/src/core.rs::Style` still models optional colors and strings
  with `ColorKind::Unset` and empty-string sentinels. C++ stores
  `optional<Color>` for color/background/texture color/border/scope dim,
  `optional<string>` for texture/alignment/user-select, and uses presence
  directly during merge. Rust has `Option`, so this deviation is not required
  by a language primitive. Port the exact optional fields/default scope-dim
  presence and merge order; update the native Rust style emitter and regenerate
  every Rust output instead of maintaining compatibility sentinels. Validate
  absent versus explicit transparent/named/RGBA values, explicit empty string
  handling where representable, state-style merging, generated style literals,
  all runtime tests, and full render parity.

  The same core pass found the corresponding terminal-cell deviation.
  C++ `cpp/include/ui/terminal/TerminalBuffer.hpp::TerminalCell` stores
  `optional<Color>` foreground/background in addition to `Color::Kind::Unset`;
  Rust `rust/src/uimd/src/core.rs::Cell` stores bare `Color` and again uses the
  unset kind as a missing-value sentinel. Port the cell fields and all blend,
  overlay, ANSI-diff, image/fallback, selection, dim, and compact-snapshot
  branches to the same optional-color decision order. Keep explicit
  `Color::Unset` distinct from absent color, and add a matrix for absent,
  explicit unset, transparent, named, opaque RGB, and partial-alpha foreground
  and background cells, including current/previous diff behavior.

  Generated-window ownership/API comparison found one remaining Rust ownership
  adapter that must be resolved or explicitly proven minimal. C++
  `GeneratedWindowRuntimeOptions` exposes a `GeneratedWindowStack*`, stack
  frames reference caller-owned windows, and concrete dialog objects remain
  directly addressable while stacked. Rust options omit `window_stack`;
  every `GeneratedWindow` owns an embedded stack whose frames own moved
  `GeneratedWindow` values, and dialogs transfer their window through
  `take_window()`. Refactor to one shared/reference-counted Rust stack and
  caller-addressable window handles if that can preserve safe ownership
  without duplicate state; otherwise retain only the smallest documented Rust
  ownership adapter while proving identical push/remove/top/frame callback,
  dialog result, nested-modal, and active-window behavior. The public stack
  option/handle surface and concrete-dialog lifecycle must not remain
  unexplained.

  Generated layout public-API comparison against declarations in
  `cpp/include/ui/generated/GeneratedWindowRuntime.hpp` and definitions in
  `cpp/src/generated/GeneratedWindowRuntime.cpp` found that Rust exposes
  rendering helpers but not the equivalent public
  `generatedWindowContentSize`, `generatedWindowContentSizeForWidth`,
  `generatedWindowResolvedContentSize`, or modal-background dim operation.
  Its private `generated_window_content_height_for_width()` independently
  clones/resolves element frames and adds window border height, whereas the C++
  function returns the complete resolved content `Size` without adding window
  borders and also accounts for resolved entry extents. Replace that partial
  helper with the same three size algorithms and make ScrollView/reusable
  layout consume the canonical result. Expose Rust-conventional public names
  and add API/fit-content/entry-overflow/border/expanded-size tests before
  rerunning reusable and ScrollView compares.

  The post-remediation installed-SDK gate on 2026-07-29 found one remaining
  Rust packaging dependency. `rust/src/uimd/build.rs` correctly compiles the
  canonical stb adapter but finds `stb_image.h` only through source-checkout
  paths (`vendor/stb` or `cpp/third_party/stb`).
  `tools/package_sdk_release.py` already packages the same single canonical
  header correctly, but the installed-target fixture in
  `tools/native_uimd_parity.py` omitted that production payload step and
  therefore failed to build its external generated Rust project. Make the
  installed-target fixture reproduce the packaged `vendor/stb/stb_image.h`
  contract without adding a second decoder implementation. Required
  validation: source-checkout runtime tests/Clippy, installed-target external
  Cargo build through
  `tools/native_uimd_parity.py --compile-examples`, packaged SDK target-content
  inspection, and `git diff --check`.

  The freshly regenerated full C++/Rust compare on 2026-07-29 found a
  multi-select ListBox construction-order difference at the initial
  `formular` frame. C++ `ListBox::ListBox()` assigns its options and calls
  `setSelectedIndex(0)` while `multiple_` is still false, then generated code
  calls `setMultiple(true)`, preserving `selectedIndices_={0}`. Rust
  `new_list_box()` called `set_multiple(true)` before `set_options()`, so the
  same generated `set_selected_index(0)` left the multi-select index set empty
  and rendered `Developer` without selected style. Mirror the C++ constructor
  order in shared Rust elements and add a direct initial-selection/render
  regression for both single- and multi-select lists. Required validation:
  Rust runtime tests/Clippy, freshly regenerated `formular`, its focused
  C++/Rust compare, and the complete C++/Rust example gate.

  After the ListBox startup fix let `formular` advance to step 22, the repeated
  MCP audit found that Rust's `string_argument(..., "text")` accepts only JSON
  strings while C++ `jsonTextField()` serializes JSON number and boolean values
  to text for `set_text`, `paste_text`, `type_text`, and
  `replace_selection`. The shared YAML intentionally sends numeric `34` to a
  NumberInput; Rust converted it to the empty fallback and returned `0`.
  Add one C++-equivalent text-argument conversion (string unchanged,
  number/boolean serialized, other types empty) and use it in all four text
  tools. Protect numeric, boolean, string, null, array, and object cases plus
  NumberInput state/callback behavior. Required validation: focused Rust MCP
  tests, `formular` C++/Rust compare, runtime tests/Clippy, and the complete
  example gate.

  The next fresh complete example compare reached `activity_feed` and exposed
  one domain-state ordering difference in the first frame after Add. C++
  `cpp/examples/activity_feed/activity_feed_panel/activity_feed_panel.cpp::
  ActivityFeedPanel::appendActivity()` invalidates dynamic children and, when
  automatic scrolling is enabled, immediately calls `scrollToBottom()`. Rust
  `rust/examples/activity_feed/activity_feed.rs::append_activity()` invalidated
  the same children but only called `set_auto_scroll(true)`, so the model and
  status advanced to three activities while the first rendered viewport could
  still omit the new bottom row. Use the same explicit bottom-scroll operation
  and event order in the Rust domain port; do not change the shared ScrollView
  state machine or the compare scenario. Required validation: freshly rebuild
  C++ and Rust `activity_feed`, pass its complete 47-assert compare, and rerun
  the complete C++/Rust example gate.

  After root-button activation was corrected, `text_editor` advanced from its
  former step-6 failure to step 52 and exposed one concrete consequence of the
  remaining stack ownership adapter. C++
  `cpp/examples/text_editor/text_editor.cpp::pushBrowserFrame()` installs an
  app-owned `GeneratedWindowFrameOptions::onButton` callback; its Open action
  first resolves a manually typed filename and only then delegates to
  `FileBrowser::acceptCurrent()`. Rust moves the FileBrowser window into the
  root-owned stack, retained only object-owned dialog behavior, and offered no
  generated-application hook equivalent to that active-frame callback.
  Consequently `rust/examples/text_editor/text_editor.rs` always took the
  canonical selected-row-first FileBrowser path and left the modal open.
  Add one generated `handle_active_window_button` adapter that runs in the same
  position as the C++ active-frame button callback, before object-owned dialog
  behavior, and use it only for the TextEditor domain-specific typed-file
  preference. This is the smallest safe Rust adapter around the C++ callback:
  the moved stack remains single-owner, no duplicate dialog state or raw
  pointer is introduced, and unhandled buttons continue into the canonical
  dialog behavior. Required validation: generated-hook/runtime unit coverage,
  freshly regenerate and rebuild Rust `text_editor`, pass its complete
  224-assert compare, and rerun all generated-output/native parity gates.

  The freshly rebuilt `task_board` compare localized the remaining
  `click_element("board[0].done")` failure to pre-click descendant frame
  synchronization. C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp::{toolClickElement,
  refreshActiveWindowLayoutForMouse,syncReusableChildFrames}` refreshes the
  active layout before reading the target frame, so the first row checkbox
  centre is computed from its root-window coordinates. Rust
  `rust/src/uimd/src/mcp.rs::click_element` renders first, but embedded
  ScrollView/reusable rendering leaves the stored descendant frame in its
  child-local coordinates until `perform_mouse_press()` later synchronizes the
  target inventory. The click therefore uses the stale local centre
  (`top=3,left=3`) instead of the live root frame (`top=10,left=30`); a direct
  press at the latter point toggles correctly. Add the C++-equivalent
  pre-target recursive frame refresh for the active window, preserving
  modal-local coordinates, and protect root reusable, generated ScrollView,
  dynamic row, and modal click centres. Required validation: focused runtime
  tests, freshly rebuilt Rust `task_board`, its complete 360-assert C++/Rust
  compare, and the complete example gate.

  Once the nested checkbox click advanced, `task_board` reached the first
  post-cancel frame and exposed the missing C++ background-focus cleanup
  transition. C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp::{
  captureBackgroundFocusCleanupContext,cleanupBackgroundFocusAfterModalClose,
  exitBackgroundEditModeAfterModalClose}` restores a live invoking ScrollView
  as the active edit scope after the final modal frame closes, clears only its
  active child edit/snapshot, preserves the descendant and scroll position, and
  renders the returned root frame dimmed outside that scope. Rust
  `rust/src/uimd/src/runtime.rs::{RuntimeState::sync_active_window,
  close_completed_modal}` restored the snapshot's temporary
  `edit_mode=false` verbatim, so focus returned to `board[2].open_btn` but the
  root frame rendered undimmed (`#1f2937` instead of `#161d26`). Port the same
  final-modal cleanup after the close callback and live-tree repair, including
  leave-commit notification order, without changing task-board behavior.
  Required validation: state regression for a false temporary button state
  returning into a live ScrollView, freshly rebuilt `task_board`, complete
  360-assert compare, modal/ScrollView regressions, and the full example gate.

  The rebuilt `task_board` then reached the later Done-filter surface and
  exposed an example-port event-value deviation shared with `image_browser`.
  The C++ reusable row handlers in
  `cpp/examples/{task_board/task_list/task_list.cpp,
  image_browser/image_browser.cpp}` deliberately ignore the textual CheckBox
  callback argument and read `checked()` from the concrete control before
  updating domain state. Rust
  `rust/examples/{task_board/task_board.rs,image_browser/image_browser.rs}`
  instead parsed only the lowercase string `"true"`, while the canonical
  generated/runtime callback value is C++-equivalent `"True"`/`"False"`.
  Consequently the visible CheckBox toggled but the Rust domain model retained
  the opposite value, which appeared only after a later rebuild/filter. Mirror
  the C++ object-owned transition by reading the live CheckBox state from the
  named generated descendant in both Rust examples; do not change the runtime
  callback spelling. Required validation: focused dynamic-row state coverage,
  freshly rebuilt `task_board` and `image_browser`, their complete C++/Rust
  compares, and the full example gate.

  The final full-example validation on 2026-07-29 exposed a nondeterministic
  Rust MCP transport/lifecycle failure that isolated scenario runs had not
  reproduced. One sequential 14-script C++/Rust run reported zero failed
  assertions or surface mismatches, but the Rust target closed a connection
  before an automatic render response in three unrelated places:
  `formular` step 58 after committing a ListBox row, `image_browser` step 39
  after returning from FileBrowser, and `expense_tracker` step 30 during its
  ordinary reusable-control route. The same freshly built `formular`,
  `image_browser`, and `task_board` binaries have otherwise passed their
  complete focused compares. Capture target exit status/stderr/backtrace and
  connection ownership around each failure, then audit
  `rust/src/uimd/src/mcp.rs` server/client-request lifetime and
  `src/uimd/testing/mcp_tester.py` process/connection handling against the C++
  MCP runtime. Fix the shared Rust runtime or general tester transport cause;
  do not add retries, waits, scenario changes, or example-specific recovery.
  Required validation: deterministic focused stress/repetition for all three
  routes, Rust MCP transport smoke, runtime tests/Clippy, and a complete
  14-script C++/Rust compare with no empty responses, failed assertions, or
  failed steps.

  Localization on 2026-07-29 reproduced the failure again at
  `image_browser` step 34 with the target process still running and an empty
  stderr tail, proving that neither a Rust panic nor application shutdown
  caused the missing response. The structural transport audit then found the
  race in `rust/src/uimd/src/mcp.rs::McpGuiServer`: Rust makes its listening
  socket nonblocking so its owner can stop the accept loop, but does not reset
  an accepted client stream to blocking mode. On macOS/BSD an accepted stream
  may retain nonblocking behavior; the per-connection worker can therefore run
  before the client's request bytes arrive, receive `WouldBlock` from
  `read_line()`, treat it like EOF, and close only that connection. C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp::McpTcpServer` uses a blocking
  accepted client and waits for the request. Keep the nonblocking listener as
  the minimal Rust thread-stop adapter, explicitly restore every accepted TCP
  and HTTP stream to blocking mode before handing it to a worker, and add a
  regression that connects first and deliberately sends later. Preserve the
  existing no-retry tester behavior and process/stderr diagnostics so a future
  response loss remains actionable.

  The repeated post-remediation audit on 2026-07-29 found one remaining
  transport-lifecycle difference outside the GUI path. C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp::{McpTcpServer,McpHttpServer}`
  accepts each TCP/HTTP client immediately on the listener thread and lets the
  controller's UI mutex serialize application work. Rust GUI MCP already
  mirrors that behavior through `McpGuiServer` and its UI-thread request
  channel, but headless `rust/src/uimd/src/mcp.rs::serve_mcp()` still reads and
  dispatches one complete client synchronously inside `listener.incoming()`.
  A client that connects and delays its request can therefore block every
  later headless client, which is not 1:1 with C++. Reuse one general Rust
  socket accept/worker/request-channel adapter for GUI and headless TCP/HTTP;
  keep all `Rc<RefCell>` application state on the owning UI thread, accept
  clients concurrently, and dispatch queued requests in the same serialized
  order as the C++ controller. Required validation: a delayed first headless
  client must not block a later complete request for both TCP and HTTP, the GUI
  delayed-send regression remains green, MCP transport smoke passes, Rust
  runtime tests/Clippy pass, and the complete C++/Rust example gate has no
  empty responses or failed steps.

  The isolated `expense_tracker` reproduction then exposed a deterministic
  render-state difference at the same step that the first full run reported as
  an empty response. After focusing `main.expenses[0].category` and pressing
  Enter, both targets report the same focused element, edit mode, and 80 live
  elements, but Rust renders the root filter panel at row 3 column 3 with the
  visible `F`/`#070b13` cell while C++ renders a dimmed blank
  `#0e131f` cell. Snapshot:
  `tests/mcp/snapshots/20260729-063745-588195-step-030-expense_tracker_compare.json`;
  viewer: `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260729-063745-588195-step-030-expense_tracker_compare.json`.
  Audit the C++ active generated-ScrollView/ComboBox overlay and outside-scope
  dim order in `cpp/src/generated/GeneratedWindowRuntime.cpp` against Rust
  `rust/src/uimd/src/runtime.rs` rendering and focus-scope transitions. Fix
  shared Rust runtime behavior only and protect the exact nested ComboBox
  Enter frame before continuing the complete scenario and full gate.

  The final direct-terminal gate on 2026-07-29 found one additional modal
  lifecycle ordering gap after the semantic negative button flash. Rust
  `rust/src/uimd/src/terminal.rs` calls
  `RuntimeState::complete_pending_standard_escape()`, which dispatches the
  dialog button and marks the concrete window closed, but unlike the MCP path
  it does not immediately call
  `runtime::close_completed_modal()`. With no subsequent input event, the
  closed modal therefore remains on the stack and the application-owned
  `on_window_closed` callback never publishes `Action canceled.`. C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp` completes the delayed
  semantic button action and modal close in one transition. Make the Rust
  interactive loop close completed modal frames immediately after the delayed
  dispatch, preserving the already-rendered 180 ms negative-button flash and
  callback-before-returned-frame order. Required validation: the focused Rust
  runtime modal test, repeated direct-PTY task-board Escape route, complete
  Rust direct-terminal smoke, task-board C++/Rust compare, runtime
  tests/Clippy, and the full example gate.

  The repeated native-generator audit on 2026-07-29 found one remaining
  structural duplication inside the canonical C++ tool. Although
  `cpp/tools/uimd/NativeCompilerModel.hpp` and
  `NativeCppGenerator.cpp::parseCompilerDocument()` now build the shared
  `CompilerDocument`/`CompilerMember` model consumed by
  `NativeRustGenerator.cpp`, the C++ emitter still independently reparses the
  raw `YamlMap` for shared member semantics in `membersRequireSixel()`,
  `ctorArgs()`, `mcpElementMetadataCode()`, `selectedIndex()` /
  `selectedValues()`, generated hook/member loops, and the main source-emission
  loop. That leaves two property interpreters for commit mode, multiple
  selection, options, initial selections, Image requirements, MCP element
  metadata, and constructor arguments, so future C++/Rust drift remains
  possible even though current generated examples compare successfully.
  Refactor the C++ emitter to consume `CompilerDocument::members` for every
  backend-neutral property and retain raw YAML only for genuinely C++-specific
  custom header/source/class and style syntax. Do not add a lookup cache or
  second model; derive any required raw C++ adapter entry by name. Required
  validation: rebuild the native tool, regenerate all C++ and Rust examples
  and supported regressions, inspect generated diffs, pass native parity both
  normally and with `--compile-examples`, build all affected C++/Rust outputs,
  run C++ CTest and Rust tests/Clippy, and rerun the complete C++/Rust compare
  plus the Python/C++ baseline with `--compare-app-size 90x35`.

  **Final repeated remediation audit (2026-07-29):** The complete
  runtime/compiler/tool pass found no further fixable or unexplained
  difference in the supported terminal-only POSIX slice after the generator
  finding above was removed. `NativeCppGenerator.cpp` now constructs shared
  behavior exclusively from `CompilerMember`: constructor arguments, member
  declarations, dependency/type resolution, layout types, event hooks,
  commit mode, selections, Image/Sixel requirement, and MCP metadata no longer
  reparse raw member YAML. The only remaining raw member reads in the C++
  emitter are the intended C++ custom class/header adapter and inline style
  syntax. `NativeRustGenerator.cpp` calls `parseCompilerDocument()` and has no
  independent document/member parser. A strengthened
  `tools/native_uimd_parity.py` fixture protects the shared `maxlength`,
  NumberInput step, commit mode, description/expose metadata, CheckBox,
  ComboBox/ListBox selection, multi-select, Image/Sixel, and reusable
  dependency properties on both emitters. Regeneration preserved the exact
  aggregate hashes of all generated C++ and Rust example outputs and produced
  no tracked C++ example/regression diff.

  The repeated structural inventory retains only these smallest language/OS
  adapters around the same behavior:
  - Rust typed references plus `ElementData` state variants and
    `Rc<RefCell>`/`Weak` liveness represent C++ virtual classes and
    `unique_ptr` ownership without adding a second state machine; base
    responsibilities, parent/child ownership, public typed methods, and
    replacement cleanup are equivalent.
  - Rust stack frames own moved windows and stable identity handles rather than
    caller raw pointers. The one `handle_active_window_button` hook preserves
    the C++ app-owned active-frame callback position for TextEditor without
    duplicating dialog state; push/remove/top, callbacks, and nested modal
    behavior are equivalent.
  - Rust preserves valid UTF-8 `String` values by moving/clamping at scalar
    boundaries while exposing C++-equivalent source-byte indices, and retains
    the documented 50 ms pending-Escape framing window for split terminal
    sequences.
  - Safe Rust wrappers call the canonical stb decoder and dynamically loaded
    libsixel contract; they do not implement a second decoder. MCP socket
    workers forward requests to the `Rc<RefCell>` owner thread through a
    channel, matching the C++ concurrent accept plus serialized controller
    contract without sharing the application tree across threads.
  - The direct interactive backend is validated on POSIX macOS/Linux. A native
    Windows console/ConPTY adapter remains outside this target's claimed
    support and is not treated as a hidden parity exception.

  Final automated validation passed: native parity normally and with
  `--compile-examples`; complete regenerated C++ build plus CTest 26/26; all 13
  Rust examples and both Rust regression apps in release mode; Rust runtime
  155/155 plus Clippy `--all-targets -- -D warnings`; Python 489/489; complete
  C++/Rust 14-script compare with 1,945 assertions; complete unchanged
  Python/C++ compare with 1,024 assertions; C++/Rust regressions with 4 and 25
  assertions plus the 14-assert Python/C++ regression baseline; Rust MCP
  transport smoke 5/5; Rust direct-terminal PTY smoke 8/8; 42/42 example and
  8/8 regression `.uimd` source pairs byte-identical; Python syntax checks,
  the 99-command fixed-viewport documentation audit, and `git diff --check`.

  Two child tasks intentionally remain open only for the real Sixel pixel
  gate: the complete image/Sixel task and final structural-coverage task.
  iTerm2 3.6.11 successfully launched the exact Rust `image_browser` command,
  reported one visible window (`id 4893`), exposed the expected application
  text through its scripting API, accepted focus/scroll arrow input, and kept
  the application alive. This execution context nevertheless cannot read
  display pixels: both full-display `screencapture` and
  `screencapture -l 4893` fail with `could not create image`, and the
  `computer-use` skill's required `node_repl`/Sky tool is not exposed in this
  session. Text/session contents omit Sixel pixels and therefore cannot prove
  absence of graphical artifacts. Keep the umbrella, image/Sixel, and coverage
  tasks open until Screen Recording/pixel access is granted and saved iTerm2
  screenshots pass the before/after-scroll, focus, modal, and resize routes.

- [x] **Fix Rust TextArea multi-character and multiline selection 1:1 with
  C++**. Reported during direct Rust validation on 2026-07-28: extending a
  selection across multiple characters in a TextArea does not work like the
  parity-validated C++ runtime. Reproduce first with direct terminal
  Shift+Left/Right/Up/Down/Home/End sequences, including split escape input,
  selection rendering, replacement, copy, and Escape/edit re-entry cleanup.
  Audit Python TextInput/TextArea selection semantics under
  `src/uimd/runtime`, C++ input dispatch and TextInput behavior under
  `cpp/src/{terminal,elements,generated}`, and Rust under
  `rust/src/uimd/src/{terminal,elements,runtime}.rs` structurally before
  editing. Preserve identical event shape, anchor/cursor roles, movement order,
  render precedence, replacement/copy behavior, and post-event cleanup; fix
  shared Rust runtime/input behavior only, never `formular` or another example.
  Required validation: failing-then-passing Rust unit/state regression,
  real-PTY `formular` selection/replacement/copy coverage against C++, Rust
  runtime tests and Clippy, regenerated/rebuilt C++ and Rust affected examples,
  focused C++/Rust compare at `--compare-app-size 90x35`, and
  `git diff --check`.

  Completed on 2026-07-28. Python
  `src/uimd/runtime/{application.py,elements.py}` and C++
  `cpp/src/{terminal/Input.cpp,elements/BasicElements.cpp}` already parse and
  apply horizontal shifted movement through one anchor/cursor selection state.
  Rust `terminal.rs` already emitted `Shift+Left`/`Shift+Right`, but
  `elements.rs::handle_text_key()` implemented only shifted vertical movement
  for TextArea, so each horizontal event was rejected before it could extend
  the range. Rust now mirrors the reference transition: establish the anchor
  only at the first shifted movement, move/clamp the cursor for every event,
  preserve the range for rendering/copy, and replace the complete range on the
  next text input.

  The new Rust state regression failed at the first `Shift+Left` before the
  fix and now verifies two-character selection, both selected render cells,
  replacement, and selection cleanup. The direct real-PTY `formular` route
  sends the same atomic terminal sequences to freshly built C++ and Rust,
  verifies horizontal selection replacement plus Cmd+C/Cmd+V range contents,
  and verifies shifted multiline selection/replacement. The complete Rust PTY
  smoke passed 8/8 groups. Rust runtime tests passed 60/60; Clippy passed with
  `--all-targets -- -D warnings`; freshly regenerated/rebuilt C++ and Rust
  `formular` passed its focused 239-assert compare; the complete 14-script
  C++/Rust gate passed 1,945 assertions with zero failures or failed steps; the
  Rust `source_separator_scroll` and `stale_scrollview_focus` regression
  compares passed 4 and 25 assertions; Python syntax checks and
  `git diff --check` passed. No Python, C++, native CLI, `.uimd`, or
  example-domain behavior changed.

- [x] **Fix disappearing, artifacted, and progressively slow Sixel images in
  `image_browser` across every runtime and verify the real terminal visually**.
  Expanded by direct user validation on 2026-08-12: Rust, C++, C#, Go, and
  Swift all lose images or leave corrupt/stale terminal regions during focus,
  view changes, and ScrollView movement; sustained 10-20 second scrolling
  becomes severely slow and can briefly scroll the whole terminal window with
  stray characters below the app before it recovers. The supplied screenshots
  show damaged sidebar text/selection blocks and inconsistent image surfaces in
  Rust and C#. Python cannot currently start from the documented direct
  `python3` command because it reports the Homebrew native libsixel as missing
  even though the full-test wrapper discovers it and its Python binding. This
  invalidates the earlier assumption that C++ is the correct visible reference
  and expands the task to the shared terminal/Sixel lifecycle plus consistent
  native-library discovery on Python. Reproduce the same interaction route in
  all six runtimes, locate the last known-good image/terminal behavior, and fix
  canonical runtime code only: Python `src/uimd/runtime`, C++ `cpp/src`, then
  port the identical state/order/geometry/cache/erase contract to C#, Swift,
  Go, and Rust. Do not change the example, force fallback, add sleeps, weaken
  tests, or treat headless MCP metadata parity as visible-terminal proof. Add a
  real PTY/terminal regression that exercises repeated scroll/view/focus redraw
  and bounds emitted Sixel work, verify native terminal scrolling/restoration,
  and make the ordinary Python launch discover the same installed libsixel as
  the canonical full-test environment without machine-specific paths.

  Diagnosis on 2026-08-12: iTerm2 reports no `CSI 16 t` cell-size response;
  its proprietary `OSC 1337;ReportCellSize` reports a 17x7-point cell with a
  Retina scale of 2, while `TIOCGWINSZ` reports the exact corresponding 34x14
  physical-pixel cell. The generic `CSI 14 t` fallback is unsafe in iTerm2
  because it reports the window frame rather than the terminal text grid. A
  real-size 107x37 C++ PTY reproduction emitted 786,231 bytes and 12 Sixel
  payloads for the first frame; the 1,022x1,020 main payload alone used 490,601
  bytes and ended exactly on terminal row 37. Gallery scroll frames then took
  4.1 seconds/226,277 bytes, followed by 28.5 seconds/762,320 bytes and 25.9
  seconds/706,199 bytes as new clipped payload variants were generated. This
  reproduces the user's progressive slowdown without an example-specific
  cause. Inline Sixel advances the terminal cursor by image rows; any decoded
  overrun at the bottom scrolls iTerm2's alternate grid, invalidates the
  runtime's `previous` terminal buffer, and explains the shifted/stale text and
  disappearing unchanged images in the supplied screenshots.

  Parity implementation contract: Python
  `src/uimd/runtime/{application.py,image.py,rendering.py}` remains the shared
  semantic reference; C++
  `cpp/src/{terminal/TerminalBackend.cpp,terminal/TerminalBuffer.cpp,elements/Image.cpp}`
  is the native structural path; C# `Runtime/{GeneratedWindow.cs,Core.cs,Elements.cs}`,
  Swift `Uimd.swift` plus `CUimdImageDecoder`, Go `runtime.go/core.go/elements.go`,
  and Rust `terminal.rs/core.rs/image.rs` must receive the same metric,
  placement, cache-key/eviction, direct visible-crop, palette/work bound, and
  no-terminal-scroll order. Protect each raw emission with a temporary scroll
  region outside its image path when a safe region exists, then reset margins;
  preserve exact full-grid images through accurate metrics. Include actual
  pixel target/crop geometry in cache identity, replace unbounded port caches
  with the same bounded runtime resource policy, and generate only the visible
  pixel rows rather than resampling a full off-screen image before cropping.
  Extend the existing direct-terminal emulator to model Sixel raster height and
  terminal scrolling with nonzero PTY pixel dimensions, and add repeated
  gallery/ScrollView output-work coverage. No `.uimd`, example, delay, fallback,
  snapshot mask, or test expectation may hide the defect.

  Implementation/checkpoint on 2026-08-12: the shared six-runtime correction
  is implemented. Python now discovers Homebrew libsixel dynamically for an
  ordinary direct launch while still respecting explicit environment paths.
  Python/C++/C#/Swift/Rust no longer invoke the noisy Homebrew
  `sixel_dither_initialize()` path, whose palette diagnostics (`making
  histogram`, `colors found`, `tupletable size`) were written to the same PTY
  as the UI and are visibly present as fragments in the supplied C# screenshot;
  they use the same fixed 64-color palette contract instead. All six runtimes
  now encode only visible pixel rows, split Sixel into terminal-cell-row
  payloads, include physical target/crop geometry in cache identity, apply the
  same 512-entry/32-MiB bounded cache policy, emit exact raster dimensions, and
  protect raw output with temporary scroll margins so inline Sixel cannot
  scroll the application grid. Go now obtains physical cell pixels through
  `TIOCGWINSZ` on supported POSIX systems instead of its former fixed 8x16
  assumption. The direct-terminal emulator models PTY pixel dimensions, Sixel
  raster height/margins, and physical terminal scrolling.

  Focused validation passed on the final implementation: the rebuilt Python,
  C++, C#, Swift, Go, and Rust `image_browser` artifacts all passed one
  persistent repeated Gallery/ScrollView Sixel route with bounded payloads,
  stable per-frame work, no terminal-scroll commands, no libsixel diagnostics,
  and intact text surfaces; direct Python startup without
  `UIMD_LIBSIXEL_PATH`/`UIMD_LIBSIXEL_DIR` discovered the installed Homebrew
  library, emitted Sixel, and remained alive. Python image/application tests
  passed 229/229 including a real-libsixel empty-stderr regression; CTest
  passed 26/26, Go runtime tests passed, Rust passed 158/158 plus Clippy, and
  Swift passed 12/12. A complete rebuild succeeded for every generated target.

  Canonical full-test checkpoint:
  `.uimd/test-logs/test-all-20260812-170542-546254.log`. All build, compile,
  manifest, unit, lint, direct-terminal, and transport phases through Rust MCP
  transport passed; the expanded direct-terminal phase passed 33/33 and
  contains the repeated all-six-runtime Sixel regression. MCP Python/C++ passed
  1040/1040, C++/C# passed 1972/1972 (including `image_browser` 378/378), and
  C++/Swift passed 1972/1972 (including `image_browser` 378/378). Per the
  user's request, the run was intentionally interrupted immediately after the
  Swift PASS; the Go phase had run for only 35 seconds and is recorded as a
  `KeyboardInterrupt`, while the later full Go/Rust and regression phases were
  not run in this checkpoint; do not interpret that interruption as a product
  failure.

  Resumed validation on 2026-08-12 completed the remaining runtime gates. The
  complete C++/Rust example compare passed 1972/1972. Python/C++ regression
  parity passed 14/14, C++/Go regressions passed 4/4 and 25/25, and C++/Rust
  regressions passed 4/4 and 25/25. The resumed aggregate C++/Go example run
  passed every scenario other than one unrelated `text_editor` FileBrowser
  fixture/render step; its `image_browser` route passed. A focused final-binary
  C++/Go `text_editor` rerun passed 251/251, proving the application/runtime
  route itself is green. The aggregate failure is tracked separately below as
  a tester target-name/fixture isolation defect. The intentionally interrupted
  monolithic log is not itself a clean 28/28 replacement.

  Originally reported during direct
  Rust validation on 2026-07-28: Sixel images intermittently disappear and
  Rust validation on 2026-07-28: Sixel images intermittently disappear and
  leave visual artifacts while scrolling or redrawing `image_browser`.
  Reproduce in a real Sixel-capable macOS terminal using the exact documented
  Rust command, capture screenshots before/after scroll, focus, modal, and
  redraw transitions, and compare the same route with freshly generated/built
  C++. Also capture raw PTY payload/update counts so a terminal screenshot is
  tied to deterministic emitted Sixel geometry and lifecycle evidence. Audit
  Python image/fallback scheduling under `src/uimd/runtime`, C++
  `cpp/src/{elements/Image.cpp,terminal/TerminalBuffer.cpp,generated/GeneratedWindowRuntime.cpp}`,
  and Rust `rust/src/uimd/src/{image,runtime,terminal}.rs` 1:1: source clipping,
  terminal-cell metrics, raster/Sixel cache keys, payload placement, previous
  frame invalidation, erase/redraw ordering, terminal-scroll interaction, and
  modal/background compositing. Fix shared Rust runtime/rendering only; do not
  change `image_browser`, force fallback, add sleeps, weaken snapshots, or mask
  artifacts. Required validation: saved real-terminal screenshots showing the
  failing-then-passing route, deterministic unit/PTY regression for the
  localized cause, Rust runtime tests and Clippy, freshly regenerated/rebuilt
  C++ and Rust `image_browser`, complete focused C++/Rust image compare at
  `--compare-app-size 90x35`, Rust direct-terminal image smoke, and
  `git diff --check`.

  Implementation progress on 2026-07-28 localized and fixed the deterministic
  Rust terminal-diff defect. Python image scheduling under
  `src/uimd/runtime/{application.py,image.py}` and C++
  `cpp/src/terminal/TerminalBuffer.cpp::renderDiffRegion()` preserve covered
  raw cells, re-emit a changed raw anchor inside a synchronized update, then
  redraw ordinary text layers above the Sixel. Rust
  `runtime.rs::write_ansi_frame_diff()` instead emitted a literal space for
  every changed `raw_skip` cell. A focus/style-only update could therefore
  erase pieces of an unchanged Sixel while the unchanged raw anchor prevented
  the payload from being sent again. Changed covered cells now emit nothing;
  changed raw anchors clear their rectangle, emit the Sixel when fully visible,
  and redraw non-raw layers in the same order as C++.

  Both focused diff regressions failed before the fix and now protect covered
  cell preservation and anchor-change raw/text re-emission. Rust runtime tests
  passed 60/60 and Clippy passed with `--all-targets -- -D warnings`.
  Freshly regenerated/rebuilt C++ and Rust `image_browser` passed its complete
  378-assert compare; all 13 Rust examples and both Rust regression apps built
  against the final runtime; the full 14-script C++/Rust compare passed 1,945
  assertions; both Rust regression compares passed; and the complete direct
  PTY smoke passed 8/8 groups, including bounded image diff output. A captured
  2,794-byte Rust Sixel payload decoded successfully with `sixel2png` to the
  expected 128x64 raster at `temp/rust-image-browser-after.png`, proving that
  the emitted payload itself is valid. Python syntax checks and
  `git diff --check` passed.

  At that checkpoint the task remained open only for the real-terminal pixel
  gate. The subsequent
  structural remediation has now ported the C++ TerminalBuffer-owned
  current/previous frame lifecycle, scroll-region/raw rejection, canonical stb
  decode and libsixel/fallback path, terminal-cell metric order, cache keys,
  generated Sixel requirement, and fallback-warning behavior. All automated
  image/runtime, full-compare, PTY, Clippy, and source gates listed in the final
  umbrella audit pass.

  The final 2026-07-29 retry launched the exact prepared
  `temp/run-rust-image-browser-iterm.command` in iTerm2 3.6.11. AppleScript
  reports one visible window (`id 4893`), returns the expected Image Browser
  session text, and delivered focus/scroll arrow input while the Rust process
  remained alive. Pixel capture is still unavailable to this execution
  context: both full-display `screencapture` and window-specific
  `screencapture -l 4893` fail with `could not create image`, and the
  `computer-use` skill's required `node_repl`/Sky tool is not exposed. Grant
  Screen Recording/pixel access to the Codex host, then rerun the exact
  launcher and save before/after-scroll, focus, modal, resize, and redraw
  screenshots.

  Completed on 2026-08-13 after the user performed the missing real-terminal
  visual gate. Direct Python launch with
  `python3 python/examples/image_browser/image_browser.py`, the C++ build, and
  the Rust build all rendered and remained functional during interaction; the
  previously reported disappearing images, terminal corruption, and severe
  progressive scrolling slowdown did not reproduce. Together with the
  automated six-runtime PTY, unit, build, compare, and regression results
  above, this closes the shared Sixel runtime defect. The separate aggregate
  Go MCP tester target-name/fixture issue below is not an image-rendering
  failure and remains open independently.

- [x] **Eliminate the remaining Python and Rust Sixel rendering throughput gap
  relative to C++**. Reported by direct user validation on 2026-08-13 after the
  disappearing-image/corruption fix: Python `image_browser` renders images
  extremely slowly, and Rust is visibly faster than Python but still markedly
  slower than C++. Reproduce the same first render, cached redraw, view switch,
  and repeated ScrollView route at identical terminal cell/pixel dimensions;
  record per-frame encode time, emitted bytes/payload count, cache hits/misses,
  and CPU profiles instead of relying only on wall-clock impressions. Audit
  Python `src/uimd/runtime/{image.py,rendering.py}`, C++
  `cpp/src/{elements/Image.cpp,terminal/TerminalBuffer.cpp}`, and Rust
  `rust/src/uimd/src/{image.rs,core.rs}` structurally 1:1, including decode,
  resize/crop, quantization, libsixel output callback/buffering, cache lookup,
  payload splitting, and terminal writes. Fix canonical runtime code only;
  preserve identical visible geometry, fixed-palette output, bounded cache,
  scroll-margin safety, and image quality. Do not alter `image_browser`, force
  fallback, lower resolution/colors, add delays, or weaken parity tests.
  Required validation: deterministic encode/cache work regression, quantified
  before/after Python/C++/Rust benchmark at the same PTY pixel geometry,
  Python image/application tests, CTest, Rust tests plus Clippy, rebuilt three
  `image_browser` targets, repeated direct-terminal Sixel PTY route, focused
  image compares at `--compare-app-size 90x35`, real-terminal user retry, and
  `git diff --check`.

  Implementation checkpoint on 2026-08-13: the Python gap was localized to
  `_fit_image_rows()`, not libsixel. A representative 70-cell-wide, 30-row
  cold render spent 11.93 of 12.07 profiled seconds in the interpreted
  area-resampling loop, making 999,600 `_sample_image_area()` calls; native
  libsixel encoding consumed only 0.10 seconds. Stretch/cover visible strips
  now use Pillow's native BOX area resampler with the same source mapping,
  background alpha flattening, direct visible-row crop, geometry, fixed Sixel
  palette, and cache identity. Contain/letterbox regions that may map outside
  the source retain the explicit reference sampler. A deterministic regression
  proves Sixel stretch/cover strips never enter the Python pixel loop and that
  separately rendered visible strips are byte-identical to the corresponding
  crop of the complete resized raster.

  Quantified result at identical 14x34 physical cell geometry: 30 cold Python
  Sixel strips improved from 6.935 seconds to 0.198 seconds (35x), with the hot
  cache still effectively immediate. The real 90x35 Gallery transition
  improved from 6.625 seconds to 0.853 seconds; C++ measured 0.562 and Rust
  Release 0.561 seconds in the same PTY. At the user's earlier 107x37 geometry,
  six precise scroll bursts showed Rust first-output latency of 17-48 ms versus
  C++ 17-39 ms, with Rust emitting no more data and usually completing the
  output burst sooner. The exact documented Rust Cargo progress launcher
  started the cached Release app in 0.916 seconds and its Gallery transition
  remained 0.561 seconds. This does not reproduce a Rust renderer throughput
  defect; a debug/stale binary or launch/build time must not be conflated with
  Release interaction performance.

  Focused Python image/application tests passed 230/230 and the complete Python
  suite passed 512/512 outside the localhost-restricted sandbox. Python compile
  checks and `git diff --check` passed, and the direct Python/C++/Rust PTY
  retained intact screens and bounded Sixel payloads. The task remains open for
  the user's real-terminal retry of the optimized Python runtime and
  confirmation of the exact Rust command/build mode; only a reproducible
  Release interaction gap should trigger a Rust runtime change.

  Refined Rust reproduction from the user on 2026-08-13: the slowdown is
  specific to continuous mouse-wheel scrolling. Rust begins stuttering after
  roughly ten seconds of sustained wheel input, while C++ remains immediately
  responsive even after a minute. The earlier benchmark serialized six wheel
  events by waiting for each frame to become quiet, so it could not expose an
  input backlog, redundant per-event rendering, unbounded queued work, or
  cache/state cost under a real burst. Reproduce with identical high-frequency
  SGR wheel streams for 10-60 seconds without per-event waits; record sent and
  consumed event counts, first/last-frame latency, output/payload growth,
  process CPU/RSS, cache occupancy/bytes, and recovery time after input stops.
  Audit Rust `terminal.rs` input draining/event dispatch and
  `core.rs` frame lifecycle against the C++ terminal backend/event loop before
  changing image encoding or cache policy. The fix must coalesce only
  semantically equivalent pending wheel deltas at the same scope/target and
  preserve exact final ScrollView offset, focus, modal routing, and one fully
  consistent post-event render; never drop arbitrary input or add throttling
  sleeps.

  Rust mouse-wheel implementation checkpoint on 2026-08-13: the sustained
  reproduction confirmed an input-dispatch backlog rather than a Sixel encode
  or image-cache regression. With 24 SGR reports submitted every 20 ms for ten
  seconds at the same 107x37 cells and 14x34 cell pixels, C++ accepted 9,312
  reports while remaining continuously productive; Rust accepted only 1,752,
  blocked 427 writes, and produced no output during two complete seconds. Rust
  `rust/src/uimd/src/terminal.rs` drained the same bounded input batch as C++
  but dispatched every wheel report individually through hit testing, state
  cleanup, and rendering. C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp` first coalesces each
  consecutive wheel run, keeps its latest pointer position, sums and clamps
  the delta to +/-12, preserves non-wheel boundaries, and discards only an
  exact net-zero run.

  Rust now uses that C++ algorithm 1:1 before event dispatch. It does not
  throttle, sleep, drop non-wheel input, or add image-browser logic. A unit
  regression protects positive/negative clamping, latest-position behavior,
  net-zero cancellation, and key/mouse boundaries. The existing direct PTY
  smoke now also sends 400 alternating down/up pairs (800 real SGR reports)
  followed by Ctrl+C to both release binaries while continuously draining
  output. Before the fix Rust needed 6.118 seconds to drain this sequence and
  failed the regression's 4.5-second ceiling; after the fix the rebuilt C++
  app took 0.547 seconds and Rust took 0.801 seconds. Rust runtime tests passed
  159/159, Clippy passed with `--all-targets -- -D warnings`, canonical
  `./tools/rebuild_all.sh` completed and refreshed the artifact manifest, and
  the focused C++/Rust `image_browser` compare passed 378/378 assertions with
  `--compare-app-size 90x35`. Python syntax checking and `git diff --check`
  pass. No new test entry point was added, so command documentation did not
  require another entry. Keep this aggregate throughput task open only for the
  user's final real-iTerm retry of the optimized Python renderer and the fixed
  sustained Rust wheel route.

  Completed on 2026-08-13 after the user confirmed that Python image rendering
  and sustained Rust scrolling now behave correctly in the real iTerm2 route.
  The subsequent canonical full test completed all 28 phases successfully,
  including every platform build, unit/lint suite, direct-terminal smoke,
  complete example compare, and regression parity compare.

- [x] **Restore Go sustained mouse-wheel responsiveness and C# Sixel raster
  row parity with C++**. Reported by direct iTerm validation on 2026-08-13
  after the Rust wheel backlog fix. Go `image_browser`, generated and built in
  Release-equivalent form with the documented `go build` command, appears to
  develop the same progressive mouse-wheel stutter that Rust previously had.
  Reproduce it with the identical high-frequency SGR stream and physical PTY
  geometry used by the C++/Rust regression, then audit Go input draining,
  consecutive-wheel coalescing, event dispatch, post-event cleanup, and render
  cadence in `go/src/uimd/runtime.go` against C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp`. Preserve every non-wheel
  boundary, latest pointer position, bounded accumulated delta, exact final
  ScrollView state, modal routing, and focus; do not throttle, sleep, discard
  arbitrary input, or change the example.

  In the same user validation, C# `image_browser` emits only one half of each
  terminal-cell image row, leaving horizontal black bands, while C++, Rust,
  Swift, Go, and Python render correctly. Reproduce normal, clipped, scrolled,
  and modal-preview images with real Sixel at identical terminal cell/pixel
  dimensions. Audit C# image fit/crop/raster geometry, libsixel encoding,
  payload splitting, cursor anchoring, cell-pixel height, raw-cell coverage,
  scroll-margin protection, and terminal frame writes in
  `csharp/src/Uimd/Runtime/{Elements.cs,Core.cs}` structurally against Python
  `src/uimd/runtime/{image.py,rendering.py}` and C++
  `cpp/src/{elements/Image.cpp,terminal/TerminalBuffer.cpp}`. Fix the canonical
  C# runtime only and preserve source dimensions, fit mode, palette, image
  quality, cache policy, and layout.

  Required validation: failing-then-passing Go wheel coalescing unit cases and
  the real PTY sustained burst against C++; deterministic C# raster/payload
  geometry and terminal-emulator row-coverage regressions that fail on the
  black-band behavior; regenerated and rebuilt C++, Go, and C# `image_browser`;
  affected Go/C# runtime tests; focused C++/Go and C++/C# image compares with
  `--compare-app-size 90x35`; repeated real-Sixel scrolling/modal PTY checks;
  structural post-fix audits against Python/C++; user real-iTerm retry; and
  `git diff --check`. Do not accept fallback-cell MCP snapshots alone as proof
  of correct raw Sixel terminal output.

  Implemented on 2026-08-13. The Go slowdown had the same aggregate-input
  cause as the repaired Rust path: `runInteractiveTerminal()` dispatched and
  rendered every parsed wheel report separately, while C++ coalesces each
  consecutive wheel run before dispatch. Go now mirrors C++ in
  `go/src/uimd/runtime.go`: it drains at most 16,384 input bytes in 256-byte
  reads, recognizes only canonical SGR wheel buttons 64/65, combines only
  consecutive wheel events, retains the latest coordinates, clamps the summed
  delta to +/-12, removes exact net-zero runs, and preserves every key or
  non-wheel mouse boundary. Focused unit coverage protects positive/negative
  clamping, cancellation, boundaries, latest coordinates, and strict SGR
  parsing. In the real PTY burst of 400 alternating down/up pairs followed by
  Ctrl+C, Go improved from 4.084 seconds before the fix to 0.647 seconds after
  it; the same rebuilt C++ binary took 0.480 seconds and Rust took 0.772
  seconds.

  The C# black bands were not caused by image fitting, cropping, encoding,
  payload segmentation, raw-cell coverage, or cache behavior. Unlike C++, C#
  deliberately skipped `TIOCGWINSZ` pixel geometry on macOS; when iTerm2 did
  not answer the CSI 16t query it therefore used the 8x16 fallback even though
  the PTY reported 14x34 cells. Each 16-pixel Sixel row occupied only half of
  one physical 34-pixel terminal cell. `GeneratedWindowRuntime` now uses the
  same ioctl-first metric order as C++ on every POSIX platform. Darwin calls
  libc's fixed `__ioctl` entry point because direct P/Invoke of the variadic
  `ioctl` function corrupts the arm64 call frame; Linux retains the ordinary
  `ioctl` adapter. This is the smallest OS-primitive exception around the same
  public geometry behavior. The deterministic real-Sixel PTY regression now
  observes C++ and C# both emitting exact 34-pixel raster rows; before the fix
  C# emitted 16 while C++ emitted 34.

  Regenerated C++, Go, and C# outputs and the canonical full rebuild completed,
  including every C++/C#/Go/Rust/Swift example and the parity artifact
  manifest. Go runtime tests pass; the C# runtime and all C# examples build in
  Release with zero warnings/errors; the direct PTY wheel comparison and C#
  cell-geometry regression pass; and the focused C++/Go `image_browser`
  compare passes 378/378 assertions with `--compare-app-size 90x35`; the
  focused C++/C# compare also passes 378/378 assertions with zero failed
  assertions or steps at the same explicit viewport. Python syntax checking
  and `git diff --check` pass. No new test entry point was added: the existing
  direct-terminal smoke command now owns both regressions, so command
  documentation needs no additional entry. Keep this task open only for the
  user's real-iTerm retry of Go sustained scrolling and C# image rows.

  Completed on 2026-08-13 after the user confirmed both corrected behaviors in
  iTerm2. The final canonical full test passed all 28/28 phases, including the
  updated 35-check direct-terminal smoke, Go runtime tests, all C# and Go
  examples, complete C++/C# and C++/Go example compares, and all regression
  parity gates.

- [ ] **Recognize Go as a platform target in aggregate MCP compares and keep
  `{platform}` fixtures isolated under the correct target name**. While
  resuming the interrupted Sixel full-test checkpoint on 2026-08-12,
  `./uimd mcp-test --backend python --headless --all --compare
  cpp/build/examples go/examples --mcp-fast --compare-app-size 90x35` passed
  all other scenarios but failed `tests/mcp/text_editor.yaml` step 19 because
  the Go FileBrowser opened the wrong retained fixture directory. Snapshot:
  `tests/mcp/snapshots/20260812-195215-888447-step-019-text_editor.json`;
  viewer: `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260812-195215-888447-step-019-text_editor.json`.
  `src/uimd/testing/mcp_tester.py::_compare_target_name()` explicitly
  recognizes Python, C++, C#, Swift, and Rust, but not Go, so the aggregate
  `go/examples` root is labeled `examples` and `{platform}` setup/environment
  paths use that accidental basename. The focused comparison with explicit
  final C++ and Go `text_editor` binaries passed 251/251. Audit and fix the
  general target-name and setup isolation path, add a focused MCP tester unit
  regression for a Go examples root plus `{platform}` fixtures, then rerun the
  full C++/Go aggregate compare with `--compare-app-size 90x35`; do not change
  `text_editor`, FileBrowser runtime behavior, snapshots, or waits.

- [x] **Preserve the real terminal when documented Rust example commands run
  through the Cargo progress launcher**. Reported on 2026-07-28: both
  `activity_feed` and `calculator` build and render once through
  `tools/cargo_with_progress.py run --release ...`, then immediately exit with
  code 0 because the launcher pipes the complete `cargo run` stdout stream and
  the generated TUI process no longer owns a real output TTY. Keep visible
  dependency/build progress and silent-operation heartbeat, but execute the
  final interactive binary with inherited stdin/stdout/stderr and terminal
  modes. Fix the general launcher/documented command, never either example.
  Required validation: failing-then-passing real-PTY smoke through the exact
  documented `activity_feed` and `calculator` command shape, normal user
  interaction remains alive until explicit quit/Ctrl+C, noninteractive
  test/build/Clippy streaming and heartbeat tests remain green, runtime tests
  and Clippy pass, and `git diff --check` passes.

  Completed on 2026-07-28. The failure was isolated to the progress launcher,
  not the Rust runtime or either example. The launcher piped the entire
  `cargo run` stdout stream so the final application observed
  `stdout.is_terminal() == false`; the Rust runtime correctly took its
  documented noninteractive single-frame path and returned success.

  `tools/cargo_with_progress.py` now handles `run` as one coordinated
  build-and-launch operation. It runs `cargo build` with Cargo JSON artifact
  reporting while continuing to stream human build/diagnostic output and emit
  silent-operation heartbeats, selects the exact reported binary artifact, and
  launches that executable directly with the launcher's original
  stdin/stdout/stderr. Application arguments after `--`, `--bin`, and
  `--example` selection remain supported. Build, test, and Clippy keep the
  existing streamed-output path.

  A failing-then-passing POSIX PTY regression proved the original
  `stdin=True, stdout=False` state and now requires both descriptors to remain
  real TTYs. The persistent Rust direct-terminal smoke also runs the exact
  documented generate/launcher command shape for `calculator` and
  `activity_feed`: calculator remained alive for `1+2`, and activity feed
  remained alive until the Quit click. The complete Rust PTY smoke passed 8/8
  groups; launcher tests passed 2/2; Rust runtime tests passed 57/57; Clippy
  passed with `--all-targets -- -D warnings`; Python syntax checks and
  `git diff --check` passed. No Rust runtime, generated application, example
  domain logic, native UIMD CLI, or other language/platform path changed for
  this fix.

- [x] **Make every documented Rust Cargo command copy-paste runnable and
  visibly alive during long silent operations**. Reported on 2026-07-28 after
  `cargo` was available only through the implementation session's isolated
  `/private/tmp` toolchain and the user's normal zsh had neither
  `$HOME/.cargo/env` nor `cargo` on `PATH`. Install and verify the official
  stable user toolchain, including Clippy, without making repository commands
  depend on the temporary agent cache. Add one shared Cargo progress launcher
  that streams Cargo output and emits a periodic timestamped heartbeat whenever
  Cargo itself is silent; update every Rust build/run/test/Clippy command in
  `docs/example_cli_commands.md` to use that launcher. Required validation:
  fresh interactive/login zsh resolves `cargo`, direct `cargo --version` and
  `cargo clippy` work, the progress launcher has deterministic heartbeat and
  exit-code tests, all documented Rust commands use it, runtime tests and
  Clippy pass through it, and `git diff --check` passes.

  Completed on 2026-07-28. The official stable Rust 1.97.1 default-profile
  toolchain is installed under the user's normal `$HOME/.cargo` and
  `$HOME/.rustup`, including Cargo, rustc, and Clippy. Rustup added
  `$HOME/.cargo/env` to both `.zshenv` and `.profile`; a fresh login/interactive
  zsh resolves `/Users/marekdubovsky/.cargo/bin/cargo` and reports Cargo,
  rustc, and Clippy 1.97.1. The permanent Cargo cache was populated from
  crates.io and no repository command depends on the temporary
  `/private/tmp/uimd-rust-toolchain`.

  Added `tools/cargo_with_progress.py` as the single documented Cargo launcher.
  It resolves `CARGO`, `PATH`, or `$HOME/.cargo/bin/cargo`, immediately prints
  the exact command, streams combined Cargo output, emits a timestamped
  heartbeat after every ten seconds of silence, reports elapsed time and exit
  status, preserves failures, and handles interruption. This also makes copied
  commands work in an already-open shell before it reloads `PATH`.
  `tools/uimd_dev.py` uses the same launcher for Rust builds, tests, and Clippy.
  Every Rust build/run/test/Clippy command in
  `docs/example_cli_commands.md` now uses the launcher; no direct project Cargo
  invocation remains.

  Validation passed: the launcher resolved the permanent Cargo executable with
  Cargo intentionally absent from `PATH`; its deterministic fake-Cargo test
  observed streamed output, multiple silent heartbeats, and preserved exit code
  7; Rust runtime tests passed 57/57 through the launcher after the permanent
  dependency cache was populated; Clippy passed with
  `--all-targets -- -D warnings`; a fresh zsh resolved Cargo and Clippy;
  Python syntax checks, `uimd_dev.py --help`, the documented-command audit, and
  `git diff --check` passed.

- [x] **Implement the complete Rust generated language target with terminal,
  runtime, generator, SDK, example, MCP, image, and regression parity**.
  Requested on 2026-07-27. Add the canonical target spelling `rust` to the
  native compiler/CLI under `cpp/tools/uimd`, generate Cargo projects and
  inheritance-equivalent event hooks from the shared native model, and add one
  Rust-owned runtime tree under `rust/src/uimd` without duplicating compiler
  behavior. Port shared behavior structurally 1:1 from the Python reference
  under `src/uimd/runtime` and the parity-validated C++ implementation under
  `cpp/{include/ui,src}`: terminal setup/teardown and direct input, frame
  rendering and blending, all public controls, focus/edit/navigation cleanup,
  modal/window stack, ScrollView and reusable children, standard dialogs and
  FileBrowser, MCP transports/tools/metadata, images/fallback/Sixel behavior,
  clipboard and copy notification, and public application APIs. Generate and
  implement the complete Rust example set from byte-identical Python `.uimd`
  sources with domain logic matching C++/Go/Swift/C#, and port every applicable
  app in `tests/regressions/uimd/parity`.

  Parity decision: Python remains the shared runtime behavior reference; C++ is
  the native terminal/compiler implementation and primary Rust compare oracle.
  Rust-specific code may differ only at Cargo/standard-library/OS primitive
  adapters while preserving the same state fields, event order, cleanup
  points, public APIs, render rules, direct-terminal bytes, MCP contract, and
  edge cases. No example-specific runtime fixes, alternate `.uimd` layouts,
  waits, masks, or snapshot weakening are allowed.

  Required validation: baseline native CLI smoke before edits; native Rust
  generation/new/scaffold/SDK/doctor parity; `cargo test` and `cargo clippy`
  for the Rust runtime; regenerate and build all Python, C++, and Rust examples;
  build all Rust examples with their normal Cargo commands; run Python/C++
  baseline tests, full C++/Rust example compare at
  `--compare-app-size 90x35`, Rust direct-terminal PTY smoke against C++, and
  every supported C++/Rust regression compare from
  `tests/regressions/uimd/parity`; update `docs/example_cli_commands.md` for
  every new command/test and finish with `git diff --check`. Record any truly
  unavoidable platform exception here before reporting completion.

  Completed on 2026-07-28. Rust is implemented as the terminal-only generated
  target `rust`. The canonical native compiler/CLI owns generation through
  `cpp/tools/uimd/NativeRustGenerator.{hpp,cpp}` and recognizes Rust in
  generation, project scaffolding, target discovery, SDK installation,
  packaging, doctor output, release metadata, and the source-checkout/full-test
  helpers. There is one Rust-owned runtime crate under `rust/src/uimd`; it
  contains the structurally equivalent frame buffer, controls, layout,
  focus/edit/navigation state machine, window/modal stack, ScrollView and
  reusable children, generated-window API, standard dialogs/FileBrowser,
  image fallback/Sixel pipeline, direct terminal adapter, and MCP stdio/TCP/HTTP
  transports. Generated application classes expose the established member and
  relevant override-hook model rather than a second compiler or app-local
  callback bridge.

  All 13 Rust example projects and both applicable parity regression apps are
  implemented with domain logic only. The final source audit confirmed all 42
  Python/Rust example `.uimd` pairs and all 8 regression `.uimd` pairs are
  byte-for-byte identical. Compare-driven fixes remained in shared Rust
  runtime architecture: dynamic ScrollView rendered-child support for
  `activity_feed`, full-viewport focused ComboBox overlay composition for
  modals, and contextual modal-return restoration when a focused reusable row
  is removed and its ScrollView becomes empty. Those transitions mirror Python
  `src/uimd/runtime` semantics and the corresponding C++ generated-window and
  ScrollView paths; no example-specific focus reset, wait, render mask,
  alternate layout, or snapshot weakening was added.

  Final validation on the completed source state passed:
  - Native CLI/SDK parity passed both normally and with `--compile-examples`,
    including external Rust scaffold generation, target lookup, and Cargo
    compilation.
  - Rust runtime tests passed 57/57 and runtime Clippy passed with
    `--all-targets -- -D warnings`; all 13 example projects and both regression
    projects built in release mode with the final runtime.
  - The complete C++/Rust example compare passed all 14 scripts with 1,945
    assertions, zero failed assertions, and zero step failures. This includes
    `activity_feed` 47, `image_browser` 378, `text_editor` 224,
    `task_board` 360, and `expense_tracker` 268 assertions.
  - The Python/C++ regression baseline passed 14 assertions; C++/Rust
    `source_separator_scroll` passed 4 and `stale_scrollview_focus` passed 25,
    all with zero failures or failed steps.
  - Rust direct-terminal PTY smoke passed 7/7 groups, covering terminal/title
    setup, normal and signal teardown, direct/split CSI and SS3 input, one paste
    event, SGR mouse drag/copy, modal Escape flash, bounded image diff output,
    and explicit Quit. Rust MCP transport smoke passed 4/4 groups for stdio,
    TCP, HTTP, metadata/app-tool schemas, batches/notifications, and unsupported
    transport errors.
  - The unchanged reference gates had already passed on this working state:
    Python tests 486/486, C++ CTest 26/26, and the full Python/C++ example
    baseline with 1,024 assertions and zero failures. Final Python syntax
    checks, documented Rust compare viewport audit, source-identity audit, and
    `git diff --check` also passed.

  Platform validation boundary: the direct interactive Rust terminal runtime
  and PTY smoke are implemented and validated on macOS/Linux POSIX terminals.
  Rust source generation is documented on Windows, but a native Windows
  console/ConPTY direct-terminal adapter was not requested or validated in this
  target slice and interactive Windows support is not claimed.

- [x] **Fix the C++/Go `text_editor` full-surface step failure exposed by the
  complete example compare on 2026-07-27**. Reproduction command:
  `./uimd mcp-test --backend python --headless --all --compare
  cpp/build/examples go/examples --mcp-fast --compare-app-size 90x35`.
  Thirteen scripts pass, while `tests/mcp/text_editor.yaml` reports 96
  assertions passed, zero failed assertions, and one render-step failure.
  Reproduce the focused scenario, inspect the recorded side-by-side snapshot
  and failing step, then audit the parity-validated C++ state/render transition
  against Go. Fix shared runtime, generator, or canonical dialog behavior; do
  not weaken the scenario, change snapshots/masks, add waits, or add a
  `text_editor`-specific runtime workaround. Parity paths and the exact
  validation gate must be updated here after localization. Localized snapshot:
  `tests/mcp/snapshots/20260727-005038-729697-step-037-text_editor.json`,
  row 14 col 24. After FileBrowser `Enter` on the `..` directory, C++ renders
  the committed selected row with `#2563eb`, while Go leaves the keyboard-active
  overlay visible with `#93ace3`. Parity decision: Python
  `src/uimd/dialogs/file_browser.py::handle_key()` commits/hides the active
  ListBox row before directory navigation; C++
  `cpp/dialogs/file_browser.cpp::runtimeOptions()` mirrors that transition with
  `ListBox::hideActiveItem()`. Go must use the same public ListBox transition in
  `go/src/uimd/{dialogs.go,elements.go}` before `AcceptCurrent()`; Python, C++,
  C#, and Swift are already correct and unaffected. The first canonical-dialog
  fix exposed a second cause: `go/examples/text_editor/text_editor.go`
  overwrites FileBrowser selection, confirmation, and pre-key callbacks with
  an older app-local state machine, while the parity-validated C++ app retains
  the shared `stackFrameOptions()` callbacks and overrides only domain-specific
  Open/Close actions. Remove those Go duplicate callbacks so there is one
  canonical FileBrowser behavior. Required validation: failing-then-passing Go
  FileBrowser state regression, Go runtime tests, regenerate/rebuild both C++
  and Go `text_editor`, run the focused compare with
  `--compare-app-size 90x35`, and rerun the complete C++/Go example compare.

  Completed on 2026-07-27. Go now exposes the same ListBox
  `ShowActiveItem()`/`HideActiveItem()` transition as C++/C#/Swift, and the
  canonical Go FileBrowser hides the keyboard-active row immediately after
  committing it and before directory navigation. The Go `text_editor` also no
  longer replaces canonical FileBrowser selection, confirmation, and pre-key
  callbacks with an older app-local state machine; it preserves the shared
  frame behavior and overrides only its domain-specific Open/Close actions,
  matching C++.

  A Go state regression now enters a directory with the active overlay visible
  and verifies that the dialog remains open/in list edit mode while the overlay
  is cleaned before rendering. The focused Go regression and full runtime
  tests passed, as did `go vet`. C++ and Go `text_editor` were freshly
  regenerated and rebuilt. The focused compare passed 224 assertions with zero
  failures, and the user's exact full C++/Go command passed all 14 scripts with
  1,945 assertions, zero failed assertions, and zero step failures. No
  snapshot, mask, wait, test weakening, or app-specific runtime workaround was
  added.

- [x] **Fix the deterministic `ui_cpp_tests` failure after the C++ ScrollView
  restoration regression was added**. Reported from
  `ctest --test-dir cpp/build --output-on-failure` on 2026-07-24: all 25
  example/logic tests pass, but `ui_cpp_tests` exits with status 1 and prints no
  failing assertion. Reproduce the standalone binary, identify the exact
  Release-active check, and fix either the shared runtime invariant or an
  incorrect regression expectation without weakening coverage. Add actionable
  failure output for the affected check. Required validation: rebuild
  `ui_cpp_tests`, run it directly, rerun the focused verbose CTest, then rerun
  all 26 CTests.

  Completed on 2026-07-24. The production ScrollView restoration was correct;
  the new state fixture rendered a detached ScrollView at `4x2` but never set
  its element frame. `restoreScrollPosition()` correctly derives its live clamp
  range from `frame()`, so the test accidentally restored against the default
  minimum `1x1` frame while asserting the `4x2` range. The fixture now assigns
  the same `4x2` frame it renders, matching a real generated-layout element.
  All Release-active early failures in `ui_cpp_tests` now print a named
  invariant instead of silently returning status 1. Validation passed for the
  rebuilt standalone binary, focused verbose CTest, and the complete 26-test
  CTest suite with zero failures.

- [x] **Fix the Go reusable/custom element contract so freshly generated
  `activity_feed` builds after the shared `Element.CommitMode` API addition**.
  Reported from the exact command
  `./uimd generate go/examples/activity_feed --target go &&
  GOCACHE=/tmp/uimd-go-cache go -C go/examples/activity_feed build -o
  activity_feed .` on 2026-07-23. `ActivityItemElement` is passed to
  `ScrollView.AddChild` as `uimd.Element`, but it no longer implements the
  expanded interface because `CommitMode()` was added only to built-in element
  bases. Audit the canonical Go element/reusable/generated-window adapter
  contract and native Go generator against C++/Python semantics; fix the shared
  abstraction rather than `activity_feed`. Add a compile-time/runtime
  regression for custom generated elements, regenerate/rebuild every affected
  Go example, and run focused Go tests plus the relevant C++/Go compare.

  Completed on 2026-07-23. Root cause was the shared FileBrowser commit-mode
  change adding `SetCommitMode` and `CommitMode` directly to the public Go
  `Element` interface. That was an unjustified breaking API change: existing
  custom elements such as `ActivityItemElement` implement the stable rendering,
  focus, frame, input, and text contract but have no reason to expose an edit
  commit policy. The mandatory methods were removed from `Element`. Built-in
  controls retain their commit-mode methods, while runtime leave-commit
  behavior now discovers the optional `CommitMode()` capability with a type
  assertion and defaults custom elements to standard behavior.

  A compile-time/runtime regression proves that an element without commit-mode
  methods still satisfies `Element`, can be added to a ScrollView, and does not
  use leave-commit behavior. Its focused POSIX and Windows commands are listed
  in `docs/example_cli_commands.md`. The user's exact generate/build command
  now succeeds. Fresh C++ `activity_feed` generation/build passed; the Go root
  example module and every standalone Go example module built; Go runtime tests
  and `go vet` passed; and the focused C++/Go `activity_feed` compare passed 47
  assertions with zero failures. No `activity_feed`-specific compatibility
  method or workaround was added.

- [x] **Preserve invoking ScrollView focus/edit/offset across modal dialogs and
  change FileBrowser ListBox Enter/Escape to commit without closing the
  dialog, identically on Python, C++, C#, Swift, and Go**. Reported during
  cross-platform `image_browser` validation on 2026-07-23. Reproduction:
  navigate into the `Image items` ScrollView, focus its `Browse` button, open
  FileBrowser, select an image, and close the dialog. Python, C#, Swift, and Go
  currently lose the invoking button focus and/or active ScrollView edit scope;
  C++ retains more focus state but can change the ScrollView offset. Required
  contract: opening/closing any modal must restore the exact live invoking
  element, active ScrollView/edit scope, remembered descendant, and saved
  vertical/horizontal offset. Content changes must not reset the saved offset;
  only clamp it to the new maximum when the content shrinks below the saved
  position.

  FileBrowser keyboard contract: Enter on a directory enters it and keeps the
  entries ListBox active; Enter on a file commits that selection and exits
  ListBox edit mode but keeps FileBrowser open; Escape from the entries ListBox
  also commits the current selection/exits ListBox edit mode and keeps the
  dialog open. Only a later dialog-level accept/cancel action may close the
  FileBrowser. Apply this in shared runtime/dialog code, never in
  `image_browser`.

  Parity paths to audit before editing: Python
  `src/uimd/runtime/{application.py,UIBase.py}` and
  `src/uimd/dialogs/file_browser.py`; C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  `cpp/src/elements/ScrollView.cpp`, and `cpp/dialogs/file_browser.cpp`; C#
  `csharp/src/Uimd/Runtime/{GeneratedWindow.cs,Elements.cs,Dialogs.cs}`; Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`; Go
  `go/src/uimd/{runtime.go,elements.go,dialogs.go}`. Add failing-then-passing
  state tests for modal push/pop focus restoration, offset preservation and
  shrink-only clamping, plus FileBrowser directory/file Enter and ListBox
  Escape. Extend `tests/mcp/image_browser_compare.yaml` to verify the complete
  user route and restored focus/edit/offset. Required validation: regenerate
  and rebuild `image_browser` for all five platforms; focused Python/C++,
  C++/C#, C++/Swift, and C++/Go compares with
  `--compare-app-size 90x35`; affected runtime/dialog unit tests; ScrollView
  regressions for every supported port; and full supported-platform validation
  if the resulting modal-stack change is architectural rather than local.

  Completed on 2026-07-23. Modal return now captures the active ScrollView,
  exact logical descendant, edit scope, and saved scroll position before a
  child window opens, then repairs recreated descendants and restores the
  offset last. The restored offset is preserved exactly while valid and is
  clamped only when rebuilt content has a smaller maximum. Python implements
  the canonical transition in `src/uimd/runtime/application.py` and
  `src/uimd/runtime/mcp.py`; C++ mirrors it in
  `cpp/src/generated/GeneratedWindowRuntime.cpp` and
  `cpp/src/elements/ScrollView.cpp`; C#, Swift, and Go mirror the same capture,
  repair, cleanup, and restore order in their shared runtimes. The C++ MCP
  Enter path was also corrected to dispatch a Browse button inside an active
  ScrollView to its embedded owner window rather than the root window.

  FileBrowser now has the same keyboard state machine on all five platforms:
  Enter on a directory navigates and keeps the entries ListBox active; Enter on
  a file commits/exits only the ListBox; Escape commits/exits only the ListBox;
  and the dialog remains open until Open/Close is activated. The final C#/Go
  Escape mismatch was traced to a structural generator/runtime gap rather than
  rendering: the canonical `entries` member declares `commit-mode: leave`, but
  C# FileBrowser omitted it and Go had no general element commit-mode contract.
  C#/Swift/Go FileBrowser now set the canonical mode, Go elements expose the
  same standard/leave state as the other runtimes, and the native C#, Swift,
  and Go generators now preserve both `commit-mode` and `commit_mode`.
  `tools/native_uimd_parity.py` permanently checks emitted commit mode for all
  five targets.

  Final focused validation passed: all five `image_browser` outputs were
  freshly regenerated and built; Python focused tests passed 3/3; C++
  `ui_cpp_tests` and `example_image_browser` passed; Go runtime tests passed,
  including modal same-size/shrink restoration and FileBrowser Escape commit;
  Swift runtime tests passed 9/9; C# runtime and example builds passed; native
  generator/CLI parity smoke passed; and the complete focused modal route
  passed Python/C++ with 20 assertions plus C++/C#, C++/Swift, and C++/Go with
  39 assertions each. The route verifies exact Browse focus and ScrollView edit
  scope before and after FileBrowser, Escape commit without dialog close, and
  the final Open return. Fresh C++/Go `source_separator_scroll` and
  `stale_scrollview_focus` regressions passed 4 and 25 assertions. Earlier
  Enter-focused runs of the same route passed on every port. `git diff --check`
  is clean. No example-specific cleanup, focus reset, scroll reset, wait, or
  snapshot mask was added.

- [x] **Make C++ `image_browser` ScrollView state transitions match the
  working C# implementation exactly**. Follow-up reported during direct C++
  validation on 2026-07-23 after the shared modal/FileBrowser fix. The
  previously established contract remains unchanged: an active ScrollView must
  keep its edit scope, exact focused descendant, and scroll offset across
  FileBrowser open/close; ordinary focus navigation must not reset or otherwise
  reinterpret the offset; content changes preserve the offset unless it exceeds
  the new maximum, in which case it clamps only to that maximum. Compare C++
  `cpp/src/elements/ScrollView.cpp`,
  `cpp/include/ui/elements/ScrollView.hpp`, and modal/focus transitions in
  `cpp/src/generated/GeneratedWindowRuntime.cpp` 1:1 against C#
  `csharp/src/Uimd/Runtime/Elements.cs` and
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`. Reproduce the direct
  `image_browser` sequence deterministically before editing, including repeated
  arrow movement before and after FileBrowser and unchanged/shrunk dynamic
  content. Fix only shared C++ runtime behavior; do not change C#, examples,
  waits, or snapshot masks. Required validation: failing-then-passing C++ state
  test, freshly regenerated/rebuilt C++ and C# `image_browser`, focused C++/C#
  compare with `--compare-app-size 90x35`, C++ runtime tests, and relevant
  ScrollView regressions.

  Completed on 2026-07-23. The direct repeated-arrow route and the complete
  Browse/FileBrowser return route were first captured as a deterministic
  C++/C# probe. The visible route already matched in the freshly built
  binaries, but the structural audit found real C++ state-order differences:
  `scrollTo`, range queries, and saved-position restoration could count stale
  dynamic children; horizontal rendering calculated vertical range at the
  unscrolled width; child visibility could read an uninitialized height cache;
  and the focus visibility helper retained a C++-only fallback absent from C#.
  C++ now refreshes dynamic children before every affected count/clamp,
  restores same-size offsets and clamps only after refreshed shrink state,
  measures vertical range at the effective render width, initializes the
  height cache before child visibility calculations, removes the duplicate
  cache pass, and returns `false` when the target is not represented by a
  ScrollView child exactly like C#.

  A new C++ state test failed before the runtime change at the first dynamic
  `scrollBy` because the stale child set appeared empty. It now covers initial
  dynamic scrolling, saved-position clamping after content shrinks, and
  horizontal-width-dependent vertical range; its checks remain active in
  Release builds. Fresh C++ and C# `image_browser` generation/builds passed.
  The extended arrow/FileBrowser/arrow C++/C# compare passed 55 assertions at
  `90x35`; C++ `ui_cpp_tests` and `example_image_browser` passed; Python/C++
  `source_separator_scroll` passed 4 assertions; Python/C++
  `stale_scrollview_focus` passed 25 assertions; and `git diff --check` is
  clean. No C#, example, wait, mask, or snapshot behavior was changed.

  Reopened on 2026-07-23 after direct C++ terminal validation disproved the
  completion claim. Focus, ScrollView edit mode, and the Browse descendant are
  restored correctly, but immediately after FileBrowser closes the terminal
  displays the wrong ScrollView viewport. The first subsequent arrow redraws
  the correct viewport, proving that logical focus/offset state and visible
  terminal output are temporarily desynchronized. Reproduce in a real C++ PTY
  and audit saved/restored `viewOffset`, pending terminal scroll deltas/regions,
  framebuffer invalidation, and modal-pop render ordering against C#. Add a
  regression that inspects the first frame emitted after modal close before any
  later key event; MCP logical snapshots alone are insufficient if they bypass
  the real terminal diff/scroll path.

  Finally completed on 2026-07-23 after reproducing the exact deep Browse route
  in a real PTY. The failing first C++ frame contained the top Camera/Astronaut/
  Coffee/Coins viewport, while an Up/Down round trip with the same logical
  Browse focus exposed the correct Sample 6-9 viewport. The captured output
  contained no terminal scroll-region sequence, disproving the earlier
  framebuffer/delta hypothesis. The corresponding C# PTY route rendered the
  correct deep viewport immediately.

  Root cause: `ImageListScroll::setItems(..., preserveScroll=true)` clears and
  rebuilds the shared C++ ScrollView. `clearChildren()` correctly resets
  `lastNaturalSkip_`, but `restoreScrollPosition()` then called the mutating
  `maxViewOffset()`. Its ordinary content-growth synchronization interpreted
  the saved `viewOffset_` against that reset previous range and moved it to the
  top. `restoreScrollPosition()` now calculates the refreshed natural range
  without applying growth synchronization, preserves the exact saved offset
  when valid, clamps it only after content shrink, establishes the refreshed
  range as `lastNaturalSkip_`, and discards a stale pending terminal-scroll
  delta because restoration replaces state rather than performing a user
  scroll. The C++-only focus-frame fallback was also removed so containment
  still matches the working C# implementation. No C#, example-specific, wait,
  or snapshot behavior changed.

  A Release-active C++ state regression failed before the fix and now covers
  clear/rebuild restoration at the same size, shrink-only clamping, and pending
  delta cleanup. Fresh C++ and C# `image_browser` generation/builds passed.
  The real-PTY first-frame regression passed independently for C++ and C#; the
  focused C++/C# modal compare passed 39 assertions; C++ `ui_cpp_tests` and
  `example_image_browser` passed; Python/C++ `source_separator_scroll` and
  `stale_scrollview_focus` passed 2 and 12 assertions; Python syntax validation
  and `git diff --check` passed.

- [x] **Resolve the pre-existing deterministic `image_browser` preview pixel
  mismatch after the modal/FileBrowser route**. The full Python/C++
  `tests/mcp/image_browser_compare.yaml` run reaches and passes the new modal,
  FileBrowser Enter/Escape, focus, edit-scope, and return assertions, then
  fails later at the unrelated deterministic fallback preview comparison:
  snapshot
  `tests/mcp/snapshots/20260723-170551-step-88-image_browser_compare.json`,
  row 7 col 34, expected `▀ #e0e0e0` but received `█ #dadada`. Viewer:
  `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260723-170551-step-88-image_browser_compare.json`.
  Audit deterministic image fallback sampling/cell encoding in Python and C++
  before changing any test, mask, or snapshot; preserve the now-passing modal
  state transitions. The audit identified a Python modal-scope parity bug:
  `src/uimd/runtime/{application.py,mcp.py}` forced compositable full-cell image
  rendering across the entire stack, including the top modal, while the
  parity-validated C++/other-port path ends that scope before rendering the top
  modal so it can retain deterministic half-blocks or a real Sixel payload.
  Restrict the Python scope to background windows and protect both direct and
  MCP render paths with tests that assert background=true and top=false.

  Completed on 2026-07-24. Python direct and MCP stack rendering now enables
  compositable full-cell image fallback only for background windows and ends
  that scope before the top modal, matching C++ and the other ports. Focused
  direct/MCP scope regressions passed, the complete affected Python test files
  passed 152 tests, and the final Python/C++ `image_browser` route passed all
  192 assertions, including the former step-88 preview mismatch. No sampling,
  snapshot, mask, example, C++, or other-port behavior was changed.

- [x] **Preserve the invoking C++/Go ScrollView edit scope when a standard
  modal is canceled with Escape**. The broadened Python/C++ `image_browser`
  compare
  on 2026-07-24 passes the former deterministic preview mismatch, then exposes
  a later step-127 mismatch after Escape cancels Delete Image: Python restores
  the invoking `main.items` ScrollView with `edit_mode=true`, while C++ forces
  the base window to `edit_mode=false` and renders a differently dimmed first
  frame. Snapshot:
  `tests/mcp/snapshots/20260724-174305-step-127-image_browser_compare.json`.
  Viewer: `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260724-174305-step-127-image_browser_compare.json`.
  Audit the Python modal-pop restoration against the C++
  `standardEscapeClosed` cleanup in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`. Preserve the established
  button-focus flash/cleanup behavior, exact focused descendant, saved
  ScrollView offset, and first returned frame; do not clear an invoking
  ScrollView scope merely because the standard dialog closed through Escape.
  The C++ fix passes the complete Python/C++ route with 192 assertions. A fresh
  final-binary C++/Go run then reproduced the same step-127 contract only in Go:
  snapshot
  `tests/mcp/snapshots/20260724-175746-098945-step-127-image_browser_compare.json`
  renders Go's returned root at full `#1f2937` instead of the canonical dimmed
  `#131921`. Audit confirmed that `GeneratedWindowStack.Remove()` correctly
  restores Go's captured scope/focus/offset, but
  `runtimeState.handleStandardEscapeButton()` immediately clears that restored
  background state after the negative-button callback. Remove that divergent
  post-callback cleanup and protect the Escape-driven stack-pop path with a Go
  state regression; do not change the now-green C++ restoration.
  Required validation: C++ runtime state regression, full Python/C++ and
  C++/Go `image_browser` compares with `--compare-app-size 90x35`, relevant
  modal/ScrollView regressions, and direct terminal dialog-Escape smoke.

  Completed on 2026-07-24. C++ standard-modal Escape now uses the same captured
  background cleanup/restore path as an explicit negative-button activation,
  preserving a live invoking ScrollView's exact descendant, edit scope, and
  saved offset while still clearing only invalid state. The negative button is
  still focused, rendered, and delayed before its callback closes the modal.
  Go's stack removal already performed the equivalent restoration, but
  `handleStandardEscapeButton()` then erased it a second time; that divergent
  post-callback clearing was removed. A failing-then-passing Go state regression
  covers Escape-driven modal removal without duplicating ScrollView state.

  Final validation passed: freshly generated/built Go `image_browser`; all Go
  runtime tests and `go vet`; C++ `ui_cpp_tests`; complete Python/C++
  `image_browser` with 192 assertions; complete C++/Go `image_browser` with 378
  assertions; C++/Go `source_separator_scroll` with 4 assertions and
  `stale_scrollview_focus` with 25; and the complete Go direct-terminal smoke,
  including the visible negative-button Escape flash. The C++ implementation
  was not changed during the Go correction, and the repeated Python/C++ gate
  proves the earlier C++ ScrollView/modal restoration remains intact.

- [x] **Port the final C++ modal-return and FileBrowser state fixes 1:1 to
  C#**. The user's complete
  `./uimd mcp-test --backend python --headless --all --compare
  cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35`
  run on 2026-07-24 passes 11 scripts and exposes three C# render-step parity
  failures with zero failed assertions:
  - `image_browser_compare.yaml` step 127,
    `tests/mcp/snapshots/20260724-191806-513286-step-127-image_browser_compare.json`:
    after standard-dialog Escape, C# clears the restored invoking ScrollView
    scope and renders full `#1f2937` instead of canonical dimmed `#131921`.
  - `text_editor.yaml` step 37,
    `tests/mcp/snapshots/20260724-192228-304040-step-037-text_editor.json`:
    after FileBrowser directory Enter, C# still renders the active ListBox
    overlay `#93ace3` instead of the committed selected row `#2563eb`.
  - `task_board_compare.yaml` step 44,
    `tests/mcp/snapshots/20260724-192816-769781-step-044-task_board_compare.json`:
    the first frame after modal cancel omits the restored background Done
    checkbox at row 8 col 30.

  Audit C# `csharp/src/Uimd/Runtime/{GeneratedWindow.cs,Dialogs.cs,Elements.cs}`
  structurally against the now-green Python modal/list state transitions and
  the parity-validated C++ implementation. Preserve the established C++
  ScrollView restore/clamp behavior, negative-button flash ordering, exact
  descendant focus, offset, and modal background suppression; do not change
  examples, waits, masks, snapshots, or tests to hide the differences.
  Required validation: C# runtime state regressions, regenerated/rebuilt C#
  `image_browser`, `text_editor`, and `task_board`; complete focused C++/C#
  compares for all three at `--compare-app-size 90x35`; relevant ScrollView
  regressions; then rerun the user's exact full C++/C# command.

  Completed on 2026-07-24. All three failures were real C# parity gaps, not
  tester crashes. `GeneratedWindow.cs` had a C#-only standard-Escape
  suppression flag that prevented the canonical modal-return cleanup from
  restoring the invoking ScrollView scope visuals; the flag and its divergent
  cleanup branch were removed, preserving the C++ negative-button flash and
  callback ordering. `Elements.cs::ScrollView.RestoreScrollPosition()` now
  mirrors the final C++ restore algorithm: refresh dynamic children, measure
  the padded live viewport without the mutating growth synchronizer, preserve
  a valid saved offset, clamp only after shrink, establish the refreshed
  natural range, and clear stale terminal-scroll delta state.

  `Dialogs.cs` now commits the active FileBrowser ListBox index, hides the
  active overlay, previews the selection, and only then performs directory
  navigation, in the same order as C++. The remaining `text_editor` mismatch
  exposed a pre-existing forbidden duplicate: its C# application
  `BrowserFrameOptions()` copied and replaced the shared FileBrowser selection,
  confirmation, and pre-key callbacks with an older local state machine. Those
  duplicate general-control callbacks were removed; the example retains only
  its domain-specific Open/Save/overwrite and close handling and now consumes
  the single canonical FileBrowser implementation like C++.

  Validation passed on the final source state: all 42 C# example outputs were
  freshly regenerated and all 13 C# example projects built with zero errors;
  focused C++/C# compares passed `image_browser` 378 assertions,
  `text_editor` 224 assertions, and `task_board` 360 assertions; C++
  `ui_cpp_tests` passed; the unchanged Python/C++ ScrollView regressions passed
  `source_separator_scroll` 2 assertions and `stale_scrollview_focus` 12
  assertions; and the user's exact full C++/C# command passed every one of its
  14 scripts with zero failed assertions and zero step failures. `git diff
  --check` is clean. No wait, mask, snapshot, tester cleanup, or
  example-specific runtime workaround was added, and the earlier C++
  ScrollView behavior remains green.

- [x] **Port the final C++ modal-return and FileBrowser state fixes 1:1 to
  Swift**. The user's complete
  `./uimd mcp-test --backend python --headless --all --compare
  cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35`
  run on 2026-07-24 passes 12 scripts and exposes two Swift render-step parity
  failures with zero failed assertions:
  - `image_browser_compare.yaml` step 127,
    `tests/mcp/snapshots/20260724-233543-920964-step-127-image_browser_compare.json`:
    after standard-dialog Escape, Swift clears/suppresses the restored invoking
    ScrollView scope and renders `#1f2937` instead of canonical dimmed
    `#131921`.
  - `text_editor.yaml` step 37,
    `tests/mcp/snapshots/20260724-233922-590917-step-037-text_editor.json`:
    after FileBrowser directory Enter, Swift still renders the active ListBox
    overlay `#93ace3` instead of the committed selected row `#2563eb`.

  Audit `swift/src/Uimd/Sources/Uimd/Uimd.swift` structurally against the now
  green Python/C++ modal, ScrollView, ListBox, and FileBrowser transitions.
  Remove the Swift-only standard-Escape suppression branch, port the final
  non-mutating ScrollView restore/clamp algorithm, and commit/hide/preview the
  FileBrowser row in canonical order. `swift/examples/text_editor/text_editor.swift`
  currently contains an independent FileBrowser implementation and replaces
  the shared callbacks with an older state machine; eliminate that duplicate
  behavior by consuming the canonical runtime FileBrowser rather than syncing
  two implementations. Do not add waits, masks, snapshots, or app-specific
  cleanup. Required validation: Swift runtime state tests, fresh C++/Swift
  generation/builds for `image_browser`, `text_editor`, and `task_board`;
  complete focused compares for all three at `--compare-app-size 90x35`;
  relevant Python/C++ ScrollView regressions and Swift direct-terminal smoke;
  then rerun the user's exact full C++/Swift command.

  Completed on 2026-07-25. The two reported failures were real Swift parity
  gaps. Swift had a standard-Escape-only suppression branch that discarded the
  restored invoking ScrollView visuals, restored saved offsets through the
  mutating ordinary growth synchronizer, and performed modal cleanup/scroll
  restoration in a different order from C++. Those paths now mirror the final
  C++ state transition: repair the live descendant and cleanup state first,
  restore the saved ScrollView position last without growth synchronization,
  preserve a valid offset, and clamp only after content shrink. Stale root edit
  snapshots and explicit suppression state are cleared at the same cleanup
  points as the reference implementation.

  The shared Swift FileBrowser now commits the active ListBox index, hides its
  active overlay, previews the selection, and only then enters a directory.
  `swift/examples/text_editor/text_editor.swift` no longer contains its
  independent FileBrowser class or replaces shared selection/confirmation/key
  callbacks with an older state machine; it retains only Open/Save/overwrite
  domain behavior and consumes `Uimd.FileBrowser`. The regenerated
  `file_browser_ui.swift` remains only the canonical generated UI class.

  The complete route then exposed one additional shared Swift focus divergence
  at the later `image_browser` step 135:
  `focusActiveWindowElementWithScrollViewScope()` manually changed focus when
  the destination was outside the active ScrollView, bypassing the canonical
  cleanup and leaving edit mode set. It now calls the same
  `focusActiveWindowElement()` transition as C++, so leaving the ScrollView
  clears its scope while modal return still preserves it.

  Final validation passed on freshly generated output: all 13 Swift example
  packages built; Swift runtime tests passed 11/11; focused C++/Swift compares
  passed `image_browser` 378 assertions, `text_editor` 224 assertions, and
  `task_board` 360 assertions; the focused step-135 route passed 10 assertions;
  the complete Swift direct-terminal smoke passed all 31 cases; C++
  `ui_cpp_tests` passed; and the unchanged Python/C++ ScrollView regressions
  passed `source_separator_scroll` with 4 assertions and
  `stale_scrollview_focus` with 25. The user's exact full C++/Swift command
  passed all 14 scripts with 1,945 assertions, zero failed assertions, and zero
  step failures. No C++ behavior was changed during this Swift correction, and
  no wait, mask, snapshot, tester cleanup, or example-specific runtime
  workaround was added.

- [x] **Fix Go `image_browser` ScrollView offset resetting during repeated
  Down-arrow focus navigation**. Reported during direct Go validation on
  2026-07-23: inside the `Image items` ScrollView, holding Down moves focus
  through roughly three or four checkbox controls, but the visible content then
  jumps back to its original position; Go `task_board` does not show the same
  reset. Reproduce against a freshly generated/built Go `image_browser` through
  direct terminal input and a deterministic runtime/MCP test before editing.
  Audit Python/C++ ScrollView focus visibility, dynamic/reusable-child rebuild,
  saved/view offsets, render-time normalization, and key-repeat event order
  against Go `ScrollView`, `runtimeState.handleKey`, focus movement, and
  `image_browser` item construction. Fix the shared Go runtime structurally
  against C++ rather than adding example-specific offset preservation. Required
  validation: a failing-then-passing regression that sends enough consecutive
  Down events to cross the viewport boundary and asserts monotonic focused-item
  visibility/offset; freshly regenerate/build C++ and Go `image_browser`;
  focused C++/Go `image_browser` compare with `--compare-app-size 90x35`; Go
  runtime tests; both Go ScrollView regressions; direct-terminal held/repeated
  arrow smoke; and audit `task_board` to explain why it was unaffected.
  Completed on 2026-07-23. Direct state and MCP probes showed that focus and
  `ScrollView.ViewOffset` were already monotonic; the apparent reset was stale
  terminal output, not a ScrollView state rollback. Go rendered a complete
  frame before every terminal read, including idle reads, and re-decoded,
  re-encoded, and retransmitted the large Sixel payload on every frame. Holding
  Down therefore queued old full screens faster than the terminal could display
  them. The pre-fix PTY probe produced 2,174,150 bytes and 33 synchronized full
  frames for 30 Down events; the final focused probe produced 62,417 bytes and
  two synchronized image updates, with focus visible at `Sample 19` and no idle
  output.

  Parity decision: Python dirty/diff scheduling in
  `src/uimd/runtime/application.py` and image caches in
  `src/uimd/runtime/image.py`, plus C++ scheduling in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, previous-frame diffing in
  `cpp/src/terminal/TerminalBuffer.cpp`, and image caches in
  `cpp/src/elements/Image.cpp`, are the references. Go now keeps a persistent
  presented buffer, renders only after state/size/stack/notification changes,
  emits terminal-cell diffs without retransmitting unchanged raw payloads, and
  caches decoded rasters and encoded Sixel payloads in
  `go/src/uimd/{runtime.go,core.go,elements.go}`. No example-specific offset or
  timing workaround was added. `task_board` was unaffected because it has no
  image/Sixel controls, so its redundant full text frames were small enough not
  to create a visible output backlog.

  Final validation passed: freshly regenerated/built C++ and Go
  `image_browser`; focused C++/Go compare with 339 assertions; Go runtime tests
  and `go vet`; C++/Go `source_separator_scroll` and
  `stale_scrollview_focus` regressions with 4 and 25 assertions; Python syntax
  validation; diff checks; and the focused real-PTY held-Down regression,
  including zero idle output, a 256 KiB output bound, bounded Sixel payloads,
  and final `Sample 19` visibility. The complete Go PTY smoke reaches the
  separately tracked standard-dialog Escape-flash failure; that existing
  failure remains open below and is not part of this image-browser fix.

- [x] **Fix the user-reported Go example data/Browse, TextArea wheel,
  image-preview Sixel, and FileBrowser ListBox regressions, with C# data parity
  included**. Reported during direct Go validation on 2026-07-22. Treat C++ as
  the structural/native reference and Python as the shared semantic reference;
  do not add example-specific runtime workarounds.
  - Audit every example on Python, C++, C#, Swift, and Go that reads auxiliary
    files or opens FileBrowser/Browse. `contacts_manager` currently loads the
    reference contacts file in C++ but not in Go or C#. Verify byte-identical
    fixtures, equivalent resource-copy/build rules, working-directory-independent
    path resolution, and identical Browse initial directories/file filters for
    every affected example. Fix shared generator/build/resource resolution or
    missing target-owned domain loading as appropriate, and add a persistent
    cross-target fixture/path inventory test.
  - Reproduce and fix mouse-wheel scrolling inside an editing multiline
    TextArea whose content exceeds its visible viewport. Audit Python
    TextInput/TextArea wheel routing and C++ `TextInput::scrollByRows` plus
    mouse-wheel dispatch against Go `TextInput` row-scroll state and
    `runtimeState.handleDirectMouse`; the TextArea must consume the wheel before
    any containing/background ScrollView, remain in edit mode, and clamp like
    C++.
  - In Go `image_browser`, keep the now-working normal images but make the
    `Show` preview modal render Sixel whenever the equivalent C++ preview does,
    rather than falling back. Audit image render mode/capability propagation,
    modal/background rendering context, clipping, and terminal-cell background
    mode in shared Go runtime and generated preview code. Add focused render-info
    and raw-payload parity coverage for the preview without weakening fallback
    behavior on unsupported terminals.
  - In FileBrowser, pressing Enter while its ListBox is active on a directory
    must enter that directory and keep ListBox edit mode active; it must not
    confirm/close the dialog. Escape must first leave ListBox edit mode so focus
    can navigate among dialog controls; only a subsequent dialog-level Escape
    cancels. Audit Python/C++ FileBrowser callbacks and ListBox confirmation
    order against Go, and apply the same contract to other ports only where the
    audit proves they differ. Add keyboard tests for directory Enter, file Enter,
    first Escape, second Escape, and unchanged Open/Cancel button behavior.
  Required validation: regenerate and rebuild every affected target/example;
  focused contacts_manager, text_editor/formular TextArea, image_browser, and
  FileBrowser compares with `--compare-app-size 90x35`; Go tests and direct PTY
  coverage; affected C# build/compare; fixture/path inventory; supported Go
  regressions; and full C++/Go compare because input, modal, image, and resource
  routing are shared surfaces.
  Completed on 2026-07-22. The auxiliary-resource audit covered every example
  resource group and every FileBrowser user across Python, C++, C#, Swift, and
  Go: contacts CSV, Markdown documents, text-editor notes, and the shared image
  sample directory. Target-local fixture trees now match the Python reference
  byte for byte; contact loading and C# output copying are deterministic; and
  Go/C++/C#/Swift Browse roots are derived from source/project locations rather
  than the process working directory. A persistent
  `python/tests/test_example_resource_parity.py` inventory test protects the
  target-local fixtures.
  Parity decision: Python semantics remain canonical in
  `src/uimd/runtime/UIBase.py` and `src/uimd/dialogs/file_browser.py`; the native
  structure is mirrored in `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  `cpp/dialogs/file_browser.cpp`, and the C++ TextInput/ListBox implementation;
  Go mirrors those transitions in `go/src/uimd/runtime.go`, `elements.go`, and
  `dialogs.go`; the audit also corrected the equivalent C# paths in
  `csharp/src/Uimd/Runtime/{GeneratedWindow,Elements,Dialogs}.cs` and Swift in
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`. Mouse wheel is offered to the
  directly targeted multiline TextInput before a containing ScrollView and
  preserves edit mode. FileBrowser Enter commits the active ListBox row into a
  directory traversal/file choice without leaving ListBox edit mode; the first
  Escape leaves ListBox edit mode and only a later dialog Escape cancels. MCP
  background cell-fallback scope now ends before the top modal is rendered, so
  an image preview can emit its real Sixel payload while the background remains
  snapshot-stable.
  Final validation passed: Python focused tests 115/115; Go runtime tests; C++
  runtime/dialog tests and full example build; Swift runtime tests 9/9; all Go
  examples plus affected C# and Swift examples built; fixture parity and Python
  syntax checks; focused C++/Go `formular` (239), `contacts_manager` (109),
  `text_editor` (215), and `image_browser` (339) compares; C++/C# contacts (109),
  text editor (215), and image browser (339) compares; C++/Swift text editor
  (215) and image browser (339) compares; Go `source_separator_scroll` (4) and
  `stale_scrollview_focus` (25) regression compares; and the full 14-script
  C++/Go compare with 1,897 assertions, zero failures, and zero failed steps.
  The direct Go PTY smoke confirms terminal setup/teardown, calculator input,
  root Escape, and split CSI/SS3 arrows inside and outside TextArea; its later
  standard-dialog Escape focus-flash assertion remains the separate open task
  below and is not claimed as fixed here. `git diff --check` is clean.

- [x] **Perform a deep structural 1:1 audit of the complete Go port against
  Python semantics and the parity-validated C++ implementation**. The
  user-requested audit on 2026-07-22 follows discovery that the Go direct
  terminal parser omitted C++ SS3 cursor-key support and treated individual
  `Read()` calls as complete logical input, despite the port having previously
  been described as 1:1. Audit every shared subsystem, not only currently
  failing tests: terminal setup/teardown and byte framing; complete keyboard,
  modifier, paste, mouse, resize, signal, and clipboard parsing; application
  loop and render scheduling; window/modal stack; focus/edit/scope state and
  cleanup order; every public element and style state; layout, clipping,
  transparency, dimming, and overlays; ScrollView and reusable children;
  dialogs/FileBrowser; image fallback/Sixel/caching; MCP tools, schema,
  snapshots, metadata, and app tools; generator-emitted public members/event
  hooks/project files; examples; and supported regression apps. Use Python
  under `src/uimd/runtime` as semantic reference, C++ under `cpp/include/ui`,
  `cpp/src`, and `cpp/tools/uimd` as structural/native reference, and Go under
  `go/src/uimd` plus native Go generation in `cpp/tools/uimd` as the audited
  port. Produce a concrete matrix of equivalent, structurally divergent,
  behaviorally missing, intentionally primitive-specific, and untested areas
  with exact paths/functions and validation evidence. Do not mark the Go port
  1:1 or complete while any unexplained divergence remains; record each fixable
  gap as an explicit follow-up task before reporting the audit.

  Completed on 2026-07-22 with C++ as the primary structural/native reference
  and Python as the semantic reference where the shared C++ behavior required
  confirmation. The result is that the Go port is not structurally 1:1 and is
  missing public behavior in several subsystems; the concrete gaps are retained
  as open tasks immediately below. Positive evidence is intentionally narrower:
  all 42 Python/Go example `.uimd` files and all 8 Python/Go regression `.uimd`
  files are byte-identical; the generated built-in element-type inventories
  match; the audited text-visual algorithm and most style fields have matching
  roles; Go runtime tests and `go vet` pass; a freshly generated and rebuilt Go
  `formular` passes the real PTY split-CSI/SS3 arrow cases; and the focused
  C++/Go `formular` MCP compare passes 239 assertions. Those gates validate the
  covered behavior only. They do not cover direct-terminal modifier parsing,
  structural runtime/API parity, label selection, FrameBufferView, the complete
  ScrollView API, shared generated dialogs, MCP schemas/transports, or image
  metrics/cache/diagnostics. The full Go PTY smoke currently passes the arrow
  section and then fails the separate deterministic dialog-Escape focus-flash
  assertion; that regression is explicitly tracked below.

- [ ] **Port the complete C++ direct-terminal input and dispatch contract to
  Go, preserving event shape and ordering**. Structural reference:
  `cpp/src/terminal/Input.cpp`, `cpp/src/terminal/TerminalBackend.cpp`,
  `cpp/include/ui/terminal/Input.hpp`, and key dispatch in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`; Go paths:
  `readTerminalInputs`, `terminalInputBytesNeedMore`, `parseTerminalInputs`,
  `parseKeyPrefix`, and `runtimeState.handleKey` in `go/src/uimd/runtime.go`.
  The Go parser still lacks C++ Alt/Alt+Shift/Ctrl/Ctrl+Shift cursor modifiers,
  Alt+Enter forms, CSI-u and legacy modified Ctrl+C/Cmd+C/Cmd+V forms, and
  terminal cell/text-area pixel-report consumption. Go expands bracketed paste
  into individual key events, whereas C++ emits one `EventType::Paste`, enters
  edit mode once, pastes once, and notifies once. Go also invokes
  `OnKeyBeforeFocusedElement` before edit Escape/standard dialog Escape, while
  C++ performs those Escape transitions before the callback and additionally
  exposes `onKeyBeforeFocused`. Preserve the now-passing split CSI/SS3 behavior,
  use one persistent parser/state machine with the same logical events, and add
  direct PTY plus unit coverage for every C++ parser case and dispatch order.

- [ ] **Port the C++ terminal lifecycle, render scheduler, and TerminalBuffer
  architecture 1:1 to Go**. References:
  `cpp/src/terminal/TerminalBackend.cpp`, `cpp/include/ui/terminal/TerminalBuffer.hpp`,
  `cpp/src/terminal/TerminalBuffer.cpp`, and the interactive loop in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`; Go paths:
  `runInteractiveTerminal`, `enterTerminalMode`, `terminalSize` in
  `go/src/uimd/runtime.go` and `TerminalBuffer` in `go/src/uimd/core.go`.
  Go currently has no SIGTERM/SIGHUP/SIGINT terminal-restore handlers, does not
  enable/disable modifyOtherKeys, and does not query/update terminal cell-pixel
  dimensions. The focused 2026-07-23 image-browser fix added persistent
  previous-frame state, dirty rendering after input/size/stack/notification
  changes, synchronized cell diff output, and full-redraw requests, but the
  broader architecture is still not 1:1: region/terminal-scroll rendering,
  render stats, controlled render/tick scheduling, and the remaining lifecycle
  behavior are absent. `RunGeneratedAppMain` also recovers a panic but returns
  success instead of C++'s nonzero startup-error result. Required validation:
  normal and abnormal teardown PTY tests, resize/pixel-report tests, idle
  animation and notification expiry, bounded redraw/diff assertions, modal
  transitions, and nonzero panic/startup-error coverage.

- [ ] **Make the Go base element, runtime options, and window-stack state model
  structurally equivalent to C++**. References:
  `cpp/include/ui/core/Element.hpp` and
  `cpp/include/ui/generated/GeneratedWindowRuntime.hpp`; Go paths:
  `elementBase` in `go/src/uimd/elements.go` and runtime option/frame/state
  structs in `go/src/uimd/runtime.go`. Go lacks C++ element identity,
  configurable commit mode, parent/generic child ownership, and live-element
  protection; it substitutes type-specific tree searches. Its stack frame
  stores a separate `runtimeState` instead of the C++ frame's explicit focus,
  edit, active ScrollView/edit element, suppression, selection snapshot, and
  remembered-descendant state. Go runtime/frame options also omit
  `onKeyBeforeFocused`, mouse-before-focused/wheel callbacks, `onEditStarted`,
  overlay rendering, and overlay dim policy. Port the same state fields,
  transition/cleanup order, and public callbacks, then add stale-child,
  replacement, modal-stack, focus/edit, and post-event cleanup tests.

- [ ] **Fix Go generator/public-control API omissions against the native C++
  generator**. References: `cpp/tools/uimd/NativeCppGenerator.cpp` and
  `cpp/tools/uimd/NativeGoGenerator.cpp`; generated evidence includes
  `cpp/examples/widget_gallery/widget_gallery_ui.cpp` versus
  `go/examples/widget_gallery/widget_gallery_ui.go` and the equivalent
  `special_elements` files. Go looks up the nonexistent `max-length` key, so
  real `maxlength` declarations generate `NewTextInput(..., 0)`; Go TextArea
  generation has no max-length argument at all, while C++ generates values such
  as 80 and 240. Go `NewNumberInput`/generation also omits C++'s step argument,
  and Go generation does not emit member `commit-mode`. Align constructor/public
  APIs and aliases, regenerate all affected Go outputs, add native generator
  assertions for non-default values, and compare behavior at and beyond each
  declared limit/step.

- [ ] **Port missing Go core/control behavior: selectable Label,
  FrameBufferView, and short hex colors**. C++ references:
  `cpp/include/ui/elements/Label.hpp`,
  `cpp/include/ui/elements/FrameBufferView.hpp`, `cpp/src/core/Color.cpp`, and
  label mouse routing in `cpp/src/generated/GeneratedWindowRuntime.cpp`; Go
  paths: `Label`, `FrameBufferView` in `go/src/uimd/elements.go`,
  `parseRgba` in `go/src/uimd/core.go`, and `beginMouseTextSelection` /
  `selectedText` in `go/src/uimd/runtime.go`. Go exposes `user-select: text` but
  mouse selection/copy accepts only TextInput/TextArea. Go FrameBufferView has
  only one cell matrix and `SetContent`, missing named frames, target order,
  regions, pan, dimming, interaction state, and corner overlays. Go also drops
  valid C++ `#RGB` colors such as `#abc`. Port the public APIs and behavior and
  add direct mouse-selection, MCP copy, multi-frame/pan/dim, and color-render
  parity tests.

- [ ] **Port UIScrollView structurally 1:1 from C++ to Go**. Reference:
  `cpp/include/ui/elements/ScrollView.hpp` and
  `cpp/src/elements/ScrollView.cpp`; Go path: `ScrollView` in
  `go/src/uimd/elements.go`. Go currently has only `Children`, one
  `ViewOffset`, basic vertical scrolling, and rendering. It lacks rendered and
  dynamic child sources, height-cache invalidation, distinct scroll/view/
  horizontal offsets, save/restore position, content windows and child views,
  index/page/horizontal navigation, wheel sizing/can-scroll queries,
  `ensureChildVisible`, and terminal scroll-delta consumption; its bottom and
  natural-skip algorithms are therefore not the same state machine. Port the
  same public methods/state roles and validate dynamic/reusable children,
  programmatic bottom preservation, focus entry, horizontal and wheel input,
  clipping, terminal scroll regions, and both Go regression apps against C++.

- [ ] **Generate Go standard dialogs/FileBrowser from the shared canonical
  `.uimd` sources and remove the hand-built duplicate design**. References:
  `src/uimd/dialogs/*.uimd`, generated Python dialogs, and `cpp/dialogs`; Go
  currently implements message boxes and FileBrowser in
  `go/src/uimd/dialogs.go` with hardcoded styles, rows, columns, widths, heights,
  and layout constructors, and has no corresponding canonical Go dialog
  `.uimd` generation. This violates identical UI sources, runtime-design
  ownership, and no-duplicate-implementation rules. Extend the native Go
  generation/rebuild flow for the same dialog sources, keep only Go behavior
  wrappers, remove manual layout/design constants, and run focused dialog and
  FileBrowser keyboard/mouse/modal compares plus direct Escape-flash coverage.

- [ ] **Port the complete C++ MCP runtime and generated metadata/tool contract
  to Go**. References: MCP controller/servers in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, metadata structures in
  `cpp/include/ui/generated/GeneratedWindowBase.hpp`, and native generation in
  `cpp/tools/uimd/NativeCppGenerator.cpp`; Go paths:
  `serveMcp`, `handleMcpRequest`, `mcpToolNames`, `GeneratedWindowBase`, and
  `NativeGoGenerator.cpp`. Go is missing C++ tools including `cut_selection`,
  `get_cursor`, `get_selection`, `get_text`, `move_cursor`, `select_all`, and
  many `ui.*` aliases. `tools/list` emits blank descriptions and an empty schema
  for every tool; `get_schema` is incorrectly the same state snapshot as
  `get_state`; per-element `description`/`expose` metadata and generated app-tool
  descriptions/input/output schemas are absent, forcing manual app-specific Go
  tool providers such as `task_board.go`. Go implements stdio/TCP only and
  silently treats any other transport, including `http`, as TCP; it lacks C++
  HTTP validation, JSON-RPC batch/notification handling, negotiated protocol
  behavior, and a real version. Add exact tool inventory/schema assertions,
  hidden-element checks, generated task-board app-tool coverage, TCP/stdio/HTTP
  protocol tests, batch/notification tests, and focused C++/Go contract compare.

- [ ] **Port the complete C++ image metrics, cache, Sixel, and fallback-warning
  pipeline to Go**. Reference: `cpp/src/elements/Image.cpp`,
  `cpp/include/ui/elements/Image.hpp`, terminal cell metrics, and
  `windowNeedsSixelFallbackWarning` in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`; Go path:
  `Image` and helpers in `go/src/uimd/elements.go`. The focused 2026-07-23
  image-browser fix added path-keyed decoded-raster caching and geometry/crop-
  keyed encoded-Sixel caching like the reference runtimes. Go still hardcodes
  terminal cell pixels to 8x16, uses only its own encoder instead of the C++
  libsixel/fallback path, and silently falls back without the standard
  continue/abort warning. Its render info also assigns `raw_present` from
  expectation instead of observed emitted payload. Port dynamic metrics, exact
  cache eviction/invalidation behavior, encoder/dependency behavior, warning
  modal, and truthful render metadata; validate Sixel/fallback, unsupported
  terminals, resize, clipping, repeat-render cache behavior, and image examples
  against C++.

- [x] **Fix the remaining Go direct-terminal standard-dialog Escape flash
  ordering regression without timing workarounds**. The full
  `tools/go_direct_terminal_smoke.py` run on 2026-07-22 passes title/teardown,
  calculator keys, root Escape, and all freshly generated `formular` split
  CSI/SS3 arrow cases, then fails `run_dialog_escape_flash_case`: the captured
  frame closes the Clear Board dialog without exposing the focused `No` style.
  Audit C++ `handleStandardEscapeButton` and the render-before-delay/close
  sequence in `cpp/src/generated/GeneratedWindowRuntime.cpp` against Go
  `handleStandardEscapeButton`, `beforeDialogButtonAction`, stack cleanup, and
  `runInteractiveTerminal` in `go/src/uimd/runtime.go`. Keep the centralized
  duration and repair event/render ordering in the shared runtime; do not add
  sleeps to the test or example. Required validation: repeated deterministic
  focused PTY runs, complete Go PTY smoke, dialog/FileBrowser compares, and
  unchanged root Escape behavior.

  Completed on 2026-07-24. The dialog focus/close order was already correct:
  Go focused the semantic negative button, rendered it, waited for the shared
  dialog-close duration, and only then closed the modal. The actual regression
  was earlier in the direct-terminal adapter. A lone ESC read made
  `readTerminalInputs` perform another kernel read under `stty VTIME=1`, which
  could block for 100 ms. The focused smoke intentionally inspects the flash
  after 80 ms, so Escape had not been dispatched yet and the captured frame
  still showed the dialog's previous `Yes` focus. Increasing the test wait to
  120 ms proved the diagnosis, but no test timing was changed.

  Go now uses one persistent `directTerminalInputReader`, matching the
  persistent framing role of the C++ input backend and Python's explicit
  escape-continuation wait. POSIX input is drained nonblocking, incomplete CSI,
  SS3, mouse, paste, and UTF-8 prefixes remain buffered across loop iterations,
  and a named 50 ms continuation timeout distinguishes a standalone Escape
  without the old 100 ms kernel stall. A deterministic unit regression covers
  split `ESC` + `[A` framing and standalone Escape timeout emission. No dialog,
  example, C++, C#, Swift, MCP, or shared flash-duration behavior changed.

  Final validation passed: the focused Go input-reader regression; all Go
  runtime tests and `go vet`; every Go example module build; five consecutive
  focused real-PTY dialog-Escape runs; the complete Go direct-terminal smoke,
  including unchanged root Escape, split CSI/SS3 arrows, mouse, explicit Quit,
  and teardown; and the focused C++/Go FileBrowser/modal-return compare with 39
  assertions, zero failures, and zero failed steps. The new test commands are
  documented for POSIX and Windows in `docs/example_cli_commands.md`, and
  `git diff --check` passes.

- [x] **Fix the C++ post-modal background focus-render mismatch in
  `task_board` against the Python reference and matching Go port**. Discovered
  while broadening validation after the direct PTY Escape fix on 2026-07-24.
  The full C++/Go `tests/mcp/task_board_compare.yaml` run passes 95 assertions
  and all logical focus checks through closing the Edit Task dialog, then fails
  the immediate post-cancel full-surface snapshot at step 44: C++ renders a
  blank background cell at row 8 column 30 while Go renders the leading `[` of
  a background Done checkbox. Snapshot:
  `tests/mcp/snapshots/20260724-163039-010782-step-044-task_board_compare.json`.
  Viewer: `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260724-163039-010782-step-044-task_board_compare.json`.
  A later full Python/C++ run reproduces exactly the same mismatch in
  `tests/mcp/snapshots/20260724-171727-step-44-task_board_compare.json`;
  therefore Python and Go agree and the C++ port is the deviation. The MCP path
  also never instantiates the new Go `directTerminalInputReader`, so this is not
  caused by or part of the completed POSIX input-framing fix. Audit Python
  modal-pop background focus suppression, restored ScrollView descendant
  rendering, and immediate post-close frame generation against the
  corresponding C++ stack-frame restore/render order. Fix shared C++ runtime
  state structurally, not the example, test, wait, or snapshot. Required
  validation: failing-then-passing focused prefix reproduction, full Python/C++
  and C++/Go `task_board` compares with `--compare-app-size 90x35`, C++ runtime
  state tests, and the relevant modal/ScrollView regression compares.

  Completed on 2026-07-24. C++ now preserves the stack frame's ScrollView scope
  state while suppressing only its background focus visuals under a modal, then
  restores the live descendant and first returned frame through the canonical
  modal cleanup path. The padded inner viewport is used when restoring and
  shrink-clamping saved ScrollView offsets, so the visible frame and logical
  offset cannot temporarily disagree. Release-active C++ state coverage
  protects same-size preservation, shrink-only clamping, and pending terminal
  delta cleanup. Full Python/C++ `task_board` passed 190 assertions, C++/Go
  passed 360, `ui_cpp_tests` passed, and the affected ScrollView regressions
  passed with 4 and 25 assertions.

- [x] **Fix C++ FileBrowser active/selected row rendering against Python in the
  `text_editor` directory-Enter route**. The full Python/C++ compare on
  2026-07-24 passes all FileBrowser state assertions through entering the
  `example_subdir` directory, but the automatic full-surface snapshot at step
  37 renders the active `..` row with C++ background `#93ace3` while Python
  renders the canonical selected background `#2563eb`. Snapshot:
  `tests/mcp/snapshots/20260724-171457-step-37-text_editor.json`. Viewer:
  `python3 tools/mcp_snapshot_viewer.py
  tests/mcp/snapshots/20260724-171457-step-37-text_editor.json`. The identical
  step already failed on 2026-07-22, so this predates the direct Go input fix.
  Audit Python `ListBox` active-versus-selected precedence and FileBrowser
  directory navigation state against C++ `ListBox` rendering and
  `cpp/dialogs/file_browser.cpp`; preserve Enter-on-directory edit mode and do
  not change the shared `.uimd`, example, wait, or snapshot. After the render
  mismatch was fixed, the compare reached a stale YAML assertion that expected
  Enter on a file to close FileBrowser. Update that assertion to the already
  established cross-platform contract: Enter commits the file and exits only
  the ListBox; the dialog remains open until the later Open activation. Required
  validation: a focused failing-then-passing `text_editor` prefix, full
  Python/C++ `text_editor` compare with `--compare-app-size 90x35`, C++
  ListBox/FileBrowser tests, and affected C++/other-port FileBrowser compares.

  Completed on 2026-07-24. C++ FileBrowser now applies the canonical ListBox
  Enter transition: it commits the active row and hides the active overlay;
  directories navigate and remain in list edit mode, while files leave only
  list edit mode and keep FileBrowser open. The C++ `text_editor` example no
  longer overrides canonical FileBrowser callbacks with a stale duplicate
  state machine, and the MCP scenario now asserts the established cross-port
  contract before activating Open. C++ ListBox/FileBrowser tests passed and the
  complete Python/C++ `text_editor` compare passed all 134 assertions.

- [ ] **Expand Go parity gates so structural/direct-terminal omissions cannot
  pass behind MCP snapshots again**. Add a C++-case-driven Go unit matrix for
  parser modifiers, paste event shape, pixel reports, short hex, Label,
  FrameBufferView, ScrollView, TerminalBuffer, metadata/schema, and startup
  errors; direct PTY coverage for every terminal-only path, signal restoration,
  clipboard sequences, dialog ordering, resize, and label selection; native
  generator assertions for actual member values, metadata, tools, schemas, and
  shared dialogs; and protocol tests for all transports. Update compare scripts
  so `get_schema` and `tools/list` outputs are compared rather than merely
  followed by a render snapshot. Run the complete C++/Go examples and both
  regression compares only after the focused structural gates pass.

- [x] **Fix the fatal Go `formular` direct-terminal arrow-key regression**.
  User validation on 2026-07-22 reports that arrow keys do not navigate between
  elements in the Go `formular` example. Reproduce the shipped Go binary
  through a real PTY before editing, including escape sequences split across
  reads; cover navigation mode plus all TextArea arrows and shifted vertical
  selection. Audit Go `readTerminalInputs`/`parseKeyPrefix` in
  `go/src/uimd/runtime.go` against Python direct input framing in
  `src/uimd/runtime/application.py`, C++ terminal parsing under
  `cpp/src/terminal`, and the now-covered Swift terminal adapter. Fix only the
  shared Go runtime, never the example. Required validation: persistent Go PTY
  regression coverage using `formular`, Go runtime tests, rebuild affected Go
  examples, focused C++/Go `formular` compare with
  `--compare-app-size 90x35`, full Go direct-terminal smoke, and an explanation
  of why earlier tests passed despite the real regression.
  Completed on 2026-07-22. The regression was reproduced before the runtime
  edit: the new split-PTY `formular` case failed to move focus from Name to
  Email. Root cause: Go terminal raw mode uses zero minimum bytes with a read
  timeout, and `readTerminalInputs()` parsed each successful `Read()` as a
  complete batch. If the first read contained only ESC, `parseKeyPrefix()`
  immediately emitted Escape; the later `[A`/`[B`/`[C`/`[D` bytes could no
  longer form an arrow. The shared Go adapter now continues reading only while
  the current batch ends in an incomplete CSI, SS3, SGR mouse, bracketed-paste,
  or UTF-8 sequence; a real standalone Escape is emitted after the existing
  terminal read timeout. No example code changed. Persistent Go PTY coverage
  now deliberately separates ESC from the remaining arrow bytes by 30 ms and
  verifies Up/Down navigation, Left/Right button navigation, all four arrows
  inside TextArea, and Shift+Up/Shift+Down selection/editing in `formular`.
  Earlier validation missed this because Go unit tests invoked `handleKey()`
  directly, MCP `press_key` bypassed direct-terminal byte framing, and the old
  Go PTY smoke neither launched `formular` with arrows nor split a sequence
  across writes; its calculator key batch arrived in one `os.write()`.
  Git history confirms that the single-read implementation existed from the
  initial Go target commit `0b4b5f8`; it appeared to work only when the kernel
  happened to coalesce the complete escape sequence into one read, so later
  timing/terminal fragmentation exposed a latent input-boundary bug rather
  than a valid navigation implementation.
  A second deterministic parser gap was then exposed by the same real-terminal
  audit: Go had no SS3 `ESC OA`/`OB`/`OC`/`OD` mappings at all. A focused test
  failed all four directions before the shared parser was corrected; the PTY
  regression now covers both split CSI and split SS3 in navigation mode and
  inside TextArea. Validation on the final arrow implementation passed: Go
  runtime tests and `go vet`; the user's exact `./uimd generate` plus
  `GOCACHE=/tmp/uimd-go-cache go -C ... build` flow; the focused real PTY
  `formular` CSI/SS3 cases; and focused C++/Go `formular` compare with 239
  assertions, zero failures, and zero failed steps at
  `--compare-app-size 90x35`. The complete Go PTY smoke is not claimed as
  passing: its arrow section passes, after which it fails the separately
  tracked standard-dialog Escape `No`-button flash regression.

- [x] **Fix the fatal Swift direct-terminal arrow-key regression before any
  further manual Swift validation**. User validation on 2026-07-22 found that
  arrow keys no longer move focus between elements in navigation mode, and any
  arrow pressed while editing a TextArea exits/commits the TextArea instead of
  moving its cursor or selection. The full C++/Swift MCP compare is not
  sufficient coverage because MCP `press_key` bypasses the direct-terminal
  byte parser/dispatcher. Reproduce this test-first through a real Swift PTY:
  assert that Up/Down/Left/Right navigate between controls in navigation mode;
  assert that all four arrows and shifted vertical arrows remain inside an
  active TextArea, preserve edit mode, and update cursor/selection like C++;
  include escape sequences split across terminal reads so an initial ESC is
  never dispatched prematurely as Escape/commit. Parity paths: Python direct
  input parsing in `src/uimd/runtime/application.py`, C++ direct key parsing and
  dispatch in `cpp/src/generated/GeneratedWindowRuntime.cpp`, and Swift direct
  input parsing/dispatch in `swift/src/Uimd/Sources/Uimd/Uimd.swift`. This is a
  Swift terminal-adapter fix only if the Python/C++ audit confirms their shared
  behavior is unchanged; do not weaken MCP tests or add an example workaround.
  Required validation: persistent Swift direct-terminal PTY coverage,
  `swift test --package-path swift/src/Uimd`, rebuild the affected Swift
  examples, focused C++/Swift `formular` compare with
  `--compare-app-size 90x35`, and a direct manual-equivalent PTY smoke showing
  navigation and TextArea arrows both work before closing this task.
  Completed on 2026-07-22. Root cause: Swift `readTerminalInputBytes()` read
  the initial ESC byte, slept for a fixed 10 ms, and then immediately passed
  whatever was available to `TerminalInputParser`. When a terminal or SSH PTY
  delivered the CSI continuation after that fixed sleep, the parser received a
  lone ESC and correctly interpreted it as Escape; this committed/exited an
  active TextArea and left the remaining `[A`/`[B`/`[C`/`[D` bytes unusable
  for navigation. The shared Swift runtime now frames CSI and SS3 sequences,
  polling for required continuation bytes up to the centralized escape
  timeout and dispatching standalone Escape only after that timeout. No
  example-specific logic or MCP workaround was added. Persistent coverage in
  `tools/swift_direct_terminal_smoke.py` now deliberately splits ESC from the
  rest of each sequence by 30 ms and verifies Up/Down navigation, Left/Right
  button navigation, all four TextArea arrows, and Shift+Up/Shift+Down
  selection/editing. Python `_read_escape_sequence()` already performs
  readiness waits while framing ANSI input; the C++ `PosixTerminalBackend` /
  `InputParser` key mappings and generated-runtime dispatch were audited and
  required no semantic change for this Swift terminal-adapter regression.
  Validation passed: Swift runtime tests 9/9; every Swift example rebuilt;
  the complete Swift direct-terminal PTY smoke passed; freshly built C++
  `formular` versus Swift `formular` passed 128 MCP assertions with zero
  failures and zero failed steps at `--compare-app-size 90x35`; Python syntax
  compilation and `git diff --check` passed. Compare log:
  `/tmp/uimd-formular-cpp-swift-arrow-fix.log`.

- [x] **Implement complete Go generated language target**. Add Go as the next
  terminal-only generated language/runtime target, using target spelling `go`
  consistently across CLI flags, SDK targets, generated files, docs, tests, and
  example/regression directories. Follow `docs/new-language-platform-workflow.md`
  end to end: shared `.uimd` sources, native C++ CLI/generator ownership, Python
  runtime as the semantic reference, C++ as the parity-validated native
  implementation, and no Go-specific example workarounds. Planned ownership:
  native CLI/generator changes in `cpp/tools/uimd`, Go runtime under
  `go/src/uimd`, Go examples under `go/examples`, and Go regression outputs
  under `go/regressions/uimd/parity` when the matching regression behavior is
  supported. Required implementation slices: add `go` to `new`, `generate`,
  `doctor`, SDK target install/auto-install, packaging metadata, generated app
  stubs, and `tools/native_uimd_parity.py`; port terminal application setup,
  raw/alternate screen handling, title/teardown, key and SGR mouse parsing,
  paste/clipboard handling, rendering/frame buffer/color blending/clipping,
  focus/edit mode, window/modal stack, post-event cleanup, base controls,
  TextInput/TextArea/NumberInput/Label/Button/CheckBox/ComboBox/ListBox,
  UIScrollView, UIElementReusable, dialogs/FileBrowser, MCP tools/snapshots,
  metadata/app tools, images with fallback/Sixel diagnostics, and tick/runtime
  lifecycle behavior. Examples must use byte-for-byte identical `.uimd` sources
  to Python/C++ equivalents and contain only Go domain application logic.
  Required validation before calling the target complete: regenerate Python,
  C++, and Go outputs; build the native CLI and C++ examples; run Go build/test
  for runtime and examples; run Python tests and C++ `ctest`; run Python/C++,
  C++/C#, C++/Swift, and C++/Go MCP compares with `--compare-app-size 90x35`;
  inspect `tests/regressions/uimd/parity`, port supported regression apps to Go,
  and run C++/Go regression compares; run direct-terminal Go PTY coverage for
  raw keys, mouse press/release/drag, Ctrl+C, teardown/title/full-frame writes;
  update `docs/example_cli_commands.md`, SDK/install docs, and any packaging
  docs; record any unavoidable Go runtime primitive differences here with exact
  affected paths, parity decision, and remaining validation. Completed:
  Go is implemented as a terminal-only generated target with native
  `cpp/tools/uimd` generator/CLI ownership, Go runtime under `go/src/uimd`,
  generated examples under `go/examples`, and Go regression outputs under
  `go/regressions/uimd/parity`. CLI/SDK/docs/tooling recognize target spelling
  `go` for `new`, `generate`, `doctor`, SDK target install/auto-install,
  packaging metadata, native parity smoke checks, and documented example/test
  commands. Runtime parity covers terminal setup/teardown/title/raw mode,
  keyboard and SGR mouse input, clipboard/paste notifications, rendering,
  styles, focus/edit mode, window/modal stack, post-event cleanup, core
  controls, TextInput/TextArea/NumberInput/Label/Button/CheckBox/ComboBox/
  ListBox, UIScrollView, UIElementReusable, dialogs/FileBrowser, MCP tools and
  snapshots, metadata/app tools, images/fallback/Sixel diagnostics, and tick
  lifecycle behavior. Parity decision: no remaining Go-specific runtime
  behavior exceptions are recorded on the validated POSIX direct-terminal
  path; Go-specific code is limited to language, terminal, build, and OS
  primitive adapters around the same public behavior. Native Windows
  direct-terminal setup remains a separately tracked platform adapter below.
  Commit-gate audit on 2026-07-17 confirmed that C++ and Go both retain the
  copy notification for the same centralized three-second duration. The
  apparent mismatch occurred because the PTY snapshot landed on the expiration
  boundary; `tools/go_direct_terminal_smoke.py` now deterministically requires
  notification appearance, text replacement, and notification expiration
  before comparing final screens. Two consecutive final PTY runs passed.
  Installed-SDK generation now resolves `targets/go` from the versioned native
  executable for both `new` and `generate`, safely quotes paths with spaces,
  and is covered by a native smoke that builds an external Go project without
  a source checkout. Release packaging excludes in-place Go example binaries
  and Swift `.build` artifacts. The full helper now runs Go runtime tests and
  correctly executes C#, Go example, and per-manifest Go regression compares.
  Final validation passed: native CLI and all supported examples/regressions
  regenerated and built; Go runtime `go test` and `go vet`; Python tests (476);
  C++ `ctest` (26/26); Swift runtime tests (9); native CLI/SDK smoke checks with
  and without `--compile-examples`; Swift and Go direct-terminal PTY smokes;
  Python/C++ full MCP compare (870 asserts); C++/C#, C++/Swift, and C++/Go full
  MCP compares (1653 asserts each); Python/C++ regression baseline (14
  asserts); and final C++/Go `source_separator_scroll` and
  `stale_scrollview_focus` compares (4 and 25 asserts). All compare gates
  reported zero failed asserts and zero step failures.

- [x] **Fix the user-reported Go direct-terminal control, dialog, overlay,
  scrolling, image, and focus regressions before treating Go parity as
  complete**. Manual validation on 2026-07-19 found behavior that the existing
  broad C++/Go MCP compare and PTY smoke did not exercise. This is a Go runtime
  parity task: Python under `src/uimd/runtime` is the semantic reference and
  the parity-validated C++ implementation under `cpp/include/ui`, `cpp/src`,
  and `cpp/src/generated/GeneratedWindowRuntime.cpp` is the native structural
  reference. Fix shared behavior in `go/src/uimd` or, only when generation is
  wrong, the canonical native generator under `cpp/tools/uimd`; do not add
  layout constants, event workarounds, focus resets, delays, or other fixes to
  individual Go examples. Audit every Go change against both reference paths
  and keep the same state fields, event order, cleanup points, overlay routing,
  focus rules, and render semantics.

  **FINAL COMPLETION CHECKPOINT (2026-07-22):** The user-reported Go
  regressions were implemented tests-first and fixed in shared runtime/tool
  code, the expanded scenarios remain persistent, and the widened parity gate
  is complete. No example-specific runtime workaround, test weakening, wait,
  or snapshot mask was introduced. The work remains intentionally uncommitted
  on `sdk-work`; unrelated untracked CMake files, Go module files, and `temp/`
  were preserved without cleanup or staging.

  Completed Go implementation and persistent coverage:
  - Fixed edit-entry/exit parity for TextInput, TextArea, and NumberInput:
    Escape rollback, fresh caret at the end, stale selection cleanup,
    TextArea Up/Down movement and multiline mouse placement, single-zero
    select-to-replace, and NumberInput blur commit.
  - Fixed modal Escape negative-action flash using the shared duration and
    ordering, root Escape no longer quits, and explicit Quit actions including
    `image_gallery` work.
  - Fixed ComboBox overlay mouse capture/selection and fall-through, reusable
    ScrollView wheel/focus routing, same-row spatial navigation, task-board
    dropdown overlay layout/clipping, image-browser focus styles, Sixel path,
    FileBrowser initial directory/`..`/ListBox wheel behavior, and related
    post-event cleanup in shared runtime/tool paths rather than examples.
  - Expanded persistent tests in `go/src/uimd/runtime_test.go`,
    `tools/go_direct_terminal_smoke.py`, Python/C++ runtime tests, and
    `tests/mcp/formular.yaml`, `widget_gallery.yaml`,
    `expense_tracker_compare.yaml`, `image_browser_compare.yaml`, and
    `task_board_compare.yaml` before/with the fixes.
  - Corrected every documented Go example command in
    `docs/example_cli_commands.md` to use `go -C <example-directory> run .`, so
    running an example no longer changes the caller's current directory.
  - All Go examples and supported regression outputs were regenerated and
    rebuilt. Go runtime tests, `go vet`, direct-terminal PTY coverage, focused
    C++/Go scenarios, and the full C++/Go compare passed. The final full
    C++/Go result was 1897 asserts with zero failures; the Python/C++ full
    baseline passed 994 asserts; Python tests passed 478 tests and C++ `ctest`
    passed 26/26.

  Widened Swift/C# parity work already completed because the new shared tests
  exposed pre-existing port gaps:
  - Swift changes are confined to
    `swift/src/Uimd/Sources/Uimd/Uimd.swift` and port the canonical
    selection/edit-entry, NumberInput zero replacement, mouse activation,
    reusable-focus blending, and scoped focus state transitions. Focused
    C++/Swift compares passed: `formular` 239, `widget_gallery` 123,
    `task_board` 360, `image_browser` 339, and `expense_tracker` 268 asserts,
    all with zero failures.
  - C# changes in `csharp/src/Uimd/Runtime/Elements.cs` and
    `GeneratedWindow.cs` include `NumberInput.SetEditText`, canonical
    `ToolSetText`/mouse zero replacement and blur commit, CheckBox activation
    refind/focus behavior, reusable descendant focus blending, dynamic
    reusable-child containment, click focus restoration, ScrollView child
    frame synchronization/visibility, and active-ScrollView directional key
    routing before ordinary edit-mode routing.
  - The final focused C# debugging pass additionally fixed the actual
    mouse-press path: focus inside a ScrollView now preserves/re-enters its
    scope; mouse CheckBox activation toggles directly without applying the
    separate MCP `activate_element` cleanup; a focused ComboBox counts as open
    inside a ScrollView only when it is the active child edit element; and
    `EnsureElementVisibleInContainingScrollView` no longer scrolls unrelated
    ScrollViews using an absolute-frame fallback.
  - All 13 C# examples were rebuilt after those final runtime changes with zero
    warnings/errors. Focused C++/C# compares now all pass: `formular` 239,
    `widget_gallery` 123, `task_board` 360, `image_browser` 339, and
    `expense_tracker` 268 asserts, all with zero failures. The last focused log
    is `/tmp/uimd-expense-tracker-cpp-csharp-go-fixes.log`.
  - Temporary C++/C# debug instrumentation was removed; a final `rg` audit for
    the temporary scroll-focus debug names returned no matches in product code.

  Final widened validation completed on 2026-07-22:
  - Full C++/C# example compare passed all 14 scripts and 1897 assertions with
    zero failed assertions and zero step failures. Log:
    `/tmp/uimd-cpp-csharp-full-go-fixes.log`.
  - `swift test --package-path swift/src/Uimd` passed all 9 tests, and every
    Swift example package rebuilt successfully.
  - Full C++/Swift example compare passed all 14 scripts and 1897 assertions
    with zero failed assertions and zero step failures. Log:
    `/tmp/uimd-cpp-swift-full-go-fixes.log`.
  - Final C++/Go `stale_scrollview_focus` regression compare passed 25
    assertions with zero failed assertions and zero step failures. Log:
    `/tmp/uimd-go-stale-scrollview-focus-final.log`.
  - `git diff --check` passed. The final status/name audit found only the
    tracked task files already described by this task, and a targeted diff
    search found no leftover temporary/debug output in the shared C++, C#,
    Swift, or Go runtime changes.

  Exact user-reported regressions to reproduce:
  - `formular`: Escape while editing TextInput does not restore the value from
    before edit mode. TextInput, TextArea, and NumberInput/SpinBox also retain a
    stale cursor or multi-character selection across edit sessions. On a fresh
    entry with no explicit preserved navigation contract, the caret must be
    after the final character and no old selection may remain. Audit all three
    editable control types.
  - `formular` and `widget_gallery`: entering a NumberInput/SpinBox containing
    only `0` must select that `0` so the next typed character replaces it.
    Editing a SpinBox and then clicking elsewhere must commit the parsed value;
    the new text must not disappear on blur and reappear only after re-entry.
  - `formular`: TextArea Up/Down cursor movement does not work and multiline
    mouse placement/selection must be audited against Python/C++. In
    `task_board` Edit Task, clicking a character on the second TextArea line
    does not place/select the expected character even though `text_editor`
    behaves correctly.
  - MessageBox Escape closes the modal without the short visible activation of
    the semantic negative action (`No` or `Cancel`). Audit FileBrowser/browse
    Escape as well. Use the same centralized activation-flash duration and
    close ordering as Python/C++; no Go-only delay constant is allowed.
  - `expense_tracker` currency and `formular` ComboBox items cannot be selected
    with the mouse. A click on an open dropdown row that visually overlaps
    another control is incorrectly delivered to that background control. The
    open dropdown must be the topmost mouse target, consume press/release, make
    the selection, and prevent fall-through activation.
  - Escape in the root Go app closes the application. Root Escape must only
    perform the same edit/modal/navigation cleanup as Python/C++ and must never
    be treated as an application quit command. Explicit Quit actions must still
    work; the Quit action in `image_gallery` currently does not.
  - In `expense_tracker`, mouse-wheel scrolling does not move the Expenses
    ScrollView, and arrow navigation inside the same ScrollView skips its
    ComboBoxes and visits only CheckBoxes. Audit reusable-child scroll routing,
    focusability, and spatial navigation against Python/C++.
  - `image_browser`: Sixel is not used when the equivalent C++ app renders
    Sixel. The left-panel focused-item styling is wrong and the last two items
    (`Gallery` and `Image items`) show no focus style. Browse dialogs must open
    in the same image directory as C++, `..` must navigate to the parent,
    mouse-wheel scrolling must move the file ListBox, and horizontal arrows in
    repeated image rows must stay in the same row (for example `Open` ->
    neighboring `Show`) instead of jumping to `Browse` in the highest visible
    row.
  - `task_board`: opening any filter ComboBox moves the complete filter area,
    including labels and TextInput, two rows downward until the dropdown closes;
    the Status menu is also clipped. Dropdowns must render as overlays without
    participating in layout or moving sibling controls, with the same clipping
    and topmost-hit behavior as Python/C++.

  Test-first plan (tests must reproduce/fail before the corresponding runtime
  fix, then remain as persistent coverage):
  1. Add focused Go runtime state-transition tests for TextInput, TextArea, and
     NumberInput: edit-entry snapshot, Escape rollback, fresh-entry caret at
     end, stale-selection cleanup, multiline Up/Down column preservation,
     single-zero select-to-replace, and blur commit. Mirror the assertions
     against the Python/C++ transition order during implementation.
  2. Extend the direct Go PTY smoke so root Escape demonstrably leaves the app
     alive, modal Escape shows the negative button during the centralized flash
     window before close, and the explicit `image_gallery` Quit action exits
     cleanly. The smoke must avoid sleeps that hide runtime cleanup; snapshots
     should be taken inside and after the named shared flash duration.
  3. Extend focused C++/Go MCP compare scenarios for `formular`,
     `widget_gallery`, and `task_board` to cover cursor/selection state after
     re-entry, TextArea vertical movement and second-line mouse placement,
     NumberInput replacement/blur, ComboBox mouse selection, overlay
     non-fall-through, dropdown geometry, and unchanged sibling positions.
  4. Extend `expense_tracker_compare.yaml` with mouse selection for every
     currency/category row, wheel scrolling over the Expenses viewport, and
     arrow traversal that reaches the ComboBoxes and CheckBoxes in the same
     reusable ScrollView. Assert post-event focus, selection, scroll, and full
     C++/Go surface parity at `--compare-app-size 90x35`.
  5. Extend `image_browser_compare.yaml` with left-panel focus-state coverage
     including the final two items, same-row button navigation, FileBrowser
     initial-directory parity, `..` parent navigation, file ListBox wheel
     scrolling, and dropdown/modal mouse capture. Add a deterministic Sixel
     capability/emission test that compares stable render metadata or escape
     payload presence with C++ without depending on terminal screenshots or
     brittle timing.
  6. Run the focused examples by regenerating and rebuilding both C++ and Go,
     then run the affected C++/Go MCP compares with
     `--compare-app-size 90x35`, Go runtime tests and `go vet`, the direct Go
     PTY smoke, and the supported C++/Go regression corpus. Because the reports
     span global input routing, focus, overlays, ScrollView, dialogs, and image
     rendering, finish with the full C++/Go example compare; if shared
     generator/runtime code affects other ports, run the wider supported full
     parity gate as required by the blast radius.

  Wider parity findings from the validation gate on 2026-07-19: the expanded
  shared scenarios also exposed pre-existing C#/Swift gaps in behavior now
  exercised by the Go regression coverage. Keep the Python transitions in
  `src/uimd/runtime/UIBase.py` (`_enter_edit_mode` and scoped edit entry),
  `src/uimd/runtime/elements.py` (`NumberInput.render_cells`), and
  `src/uimd/runtime/mcp.py` (`_prepare_text_element`/`tool_select_text`) aligned
  with C++ `beginElementEdit`, `NumberInput::render`, `toolSelectText`,
  `applyReusableFocusBackground`, and
  `applyReusableFocusBackgroundToBuffer` in
  `cpp/src/elements/BasicElements.cpp` and
  `cpp/src/generated/GeneratedWindowRuntime.cpp`. The affected port paths are
  C# `ToolSelectText`, `ToolEnterEditMode`, `ToolTypeText`, `BeginElementEdit`, and
  reusable-focus rendering in
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs` plus `NumberInput.Render` in
  `csharp/src/Uimd/Runtime/Elements.cs`; and Swift MCP `enter_edit_mode`,
  `performTypeText`, `mouseClick`/`mousePointerEvent`, `performMousePressTarget`,
  `beginElementEdit`, `NumberInput.render`, and reusable-focus rendering in
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`. Persistent failing coverage exists
  before these port fixes in `tests/mcp/formular.yaml`,
  `tests/mcp/widget_gallery.yaml`, and `tests/mcp/image_browser_compare.yaml`.
  The Python/C++ full compare passed 994 asserts with zero failures, confirming
  that C#/Swift must port the canonical focus/edit/selection/blending state
  transitions rather than weakening or masking these scenarios.
  The expanded `expense_tracker` mouse coverage additionally exposed a C#
  mouse-press state-order gap in
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`: focusing and activating a
  CheckBox inside a ScrollView cleared the active ScrollView/edit scope after
  the mouse target had established it, so C# omitted the same outside-scope dim
  that Python `_handle_mouse_event` and C++ `handleMousePress` retain. Port the
  canonical mouse-only CheckBox activation and focus-preservation order. The
  same path must distinguish an active ScrollView scope from a ComboBox that is
  actively being edited: scope `EditMode` alone does not mean the focused
  ComboBox dropdown is open when `ActiveScrollViewEditElement` is null. Keep
  the separate MCP `activate_element` cleanup contract unchanged. A later
  post-modal focus step also showed that C#
  `EnsureElementVisibleInContainingScrollView` applied a target-frame fallback
  to ScrollViews that did not contain the target, moving the focused proxy's
  own content by one row. Match C++ containment: scroll only the ScrollView
  whose child tree actually contains the target.

- [ ] **Implement and validate the native Go direct-terminal adapter on
  Windows**. The Go runtime currently enters raw mode, reads terminal size, and
  restores terminal state through `stty` in `go/src/uimd/runtime.go`, so the
  direct interactive app path is implemented and validated only on macOS/Linux
  POSIX terminals. Windows generation, compilation, runtime unit tests, and
  headless MCP commands may be documented and validated independently, but an
  interactive Windows example command must not be advertised as supported
  until the runtime uses a Windows console/ConPTY adapter with equivalent raw
  input, mouse, title, alternate-screen, resize, Ctrl+C, and teardown behavior.
  Parity paths: Python terminal handling in
  `src/uimd/runtime/application.py`, C++ terminal handling in
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, and Go terminal handling in
  `go/src/uimd/runtime.go`. Required validation: Windows direct-terminal PTY or
  ConPTY smoke for keyboard, SGR mouse press/drag/release, Ctrl+C, title,
  alternate screen, resize, and state restoration; Go runtime tests; focused
  Windows headless MCP compare; and unchanged POSIX Go PTY smoke.

- [x] **Complete the Go command inventory in
  `docs/example_cli_commands.md`**. Document `go test` and `go vet`, all Go
  example run commands, POSIX and Windows generation/build commands, the Go
  scaffold smoke, full and per-app C++/Go MCP compares, and both supported Go
  regression compares. Keep `--compare-app-size 90x35` on every compare command
  and explicitly distinguish the validated POSIX direct-terminal path from
  Windows build/unit/headless-MCP coverage. Completed on 2026-07-17: the
  inventory contains all 14 Go example commands, all 14 focused C++/Go MCP
  scenario commands plus the full compare, both regression compares, POSIX and
  Windows build forms, runtime test/static-check commands, PTY scope, and the Go
  scaffold smoke. Validation passed for `go test`, `go vet`, the documented Go
  scaffold build, a focused calculator compare (43 asserts, zero failures),
  Windows amd64 cross-builds of all 14 examples, both regressions, and the
  runtime test binary; every Go compare command includes
  `--compare-app-size 90x35`.

- [ ] **Distill previously missed user-reported regressions into reproducible tests**.
  User validation in early July 2026 exposed concrete regressions that existing
  tests did not catch when they were first reported. This task is not a vague
  "add more tests" bucket; it must turn the exact situations below into exact
  repro scripts and then focused regression tests. Initial exact situations:
  - Run `./uimd generate swift/examples/activity_feed --target swift && swift
    build --package-path swift/examples/activity_feed && swift run
    --package-path swift/examples/activity_feed activity_feed`. The app opened
    but initially could not be controlled with arrows or mouse and was hard to
    quit from the terminal. Expected: Swift direct terminal input should route
    arrows, mouse events, and quit/control events like C++.
  - Run the same Swift `activity_feed` command. After direct interaction started
    working, every input caused rendering to stutter badly, roughly one visible
    terminal frame every 2-3 seconds, worse than Python. Expected: Swift should
    be close to C++ terminal responsiveness and should not redraw with
    multi-second stalls after ordinary input.
  - In Swift `activity_feed`, open the `Settings` dialog, navigate with arrows
    down to the first checkbox, then press arrow keys to continue. Broken
    behavior: focus became trapped on the checkbox and could not move lower in
    the dialog. Expected: CheckBox in a dialog should activate/toggle when
    appropriate but arrows should still move focus to neighboring dialog
    controls like C++.
  - In Swift `activity_feed`, add a new activity while the activity ScrollView
    is not fully scrolled to the bottom. Broken behavior: the ScrollView moved
    only by about one item and did not reveal the newly added item. In Python
    `activity_feed`, adding a new activity initially did not scroll at all.
    Expected: appending a bottom item should scroll to the new bottom on every
    platform.
  - In Swift `formular`, enter the TextArea, type multiple lines, press Up/Down
    and Shift+Up/Shift+Down. Broken behavior: vertical cursor movement and
    multi-character/multi-line selection did not work. Expected: match C++ text
    editing, including vertical navigation and selection replacement.
  - In Swift `formular`, enter a TextInput, type text, use Shift+Left multiple
    times, and type replacement text. Broken behavior: selecting multiple
    characters in TextInput did not work like C++/C#. Expected: selected range
    is highlighted and replacement changes only the selected range.
  - Open a Yes/No message box in Swift, press Escape. Broken behavior: the box
    closed/cancelled, but did not briefly show the `No` button as activated, so
    the user could not see that cancel was chosen. Expected: same short
    activation flash as C++ for `No`; also check whether FileBrowser/browse
    dialogs have the same Escape/cancel visual parity.
  - In Swift `expense_tracker`, enter the title/name TextInput, optionally type
    text, then press Down while still editing. Broken behavior: focus moved to
    the next element even though the TextInput was in edit mode. Expected:
    arrow key handling inside editable TextInput should match `formular`/C++ and
    not leave edit mode incorrectly.
  - In Swift examples, run a Swift app after a Python example and inspect the
    terminal title. Broken behavior: the title showed stale `[python]` instead
    of `[swift]` in at least `image_browser` screenshot context. Expected:
    Swift terminal title always ends with `[swift]`.
  - Run Swift `image_browser` and `image_gallery`. Broken behavior: images did
    not display. In fallback mode the two half-block "pixels" appeared swapped
    top/bottom, visible in `fallback_problem.png`/`fallback.png`. Expected:
    fallback half-block foreground is the top sample and background is the
    bottom sample, matching C++/C#.
  - In Swift `image_browser`, use the button that shows an image preview in
    Sixel mode. Broken behavior: the preview showed fallback instead of Sixel in
    cases where Sixel should have been active. Expected: supported Sixel
    terminals render raw Sixel in the preview.
  - Run `image_browser` over SSH or in macOS Terminal where Sixel is not
    supported. Broken behavior: raw/random Sixel characters or only a small
    insufficient status text appeared. Expected: no raw Sixel garbage; show the
    standard UIMD warning/message box, then continue with fallback rendering if
    the user chooses to continue.
  - Run `image_browser` in iTerm2 with Sixel support. Broken behavior at one
    point: fallback was forced even though iTerm2 supports Sixel. Expected:
    iTerm2/known supported terminals should use Sixel, unsupported terminals
    should use fallback plus warning.
  - In Swift/iTerm2 `image_browser`, scroll so images are partially clipped at
    the top. Broken behavior: images that should be clipped from the top were
    moved down from the start instead of having their top cropped. Expected:
    visible Sixel region starts at the correct clipped source offset.
  - In Swift/iTerm2 `image_browser`, scroll so images are partially clipped at
    the bottom. Broken behavior: bottom-clipped images sometimes were not drawn
    at all. Expected: draw the visible upper part of the image.
  - In `image_browser`, use small app heights where an image is clipped both at
    the top and bottom. Broken behavior: clipped images were intermittent,
    sometimes visible and sometimes skipped depending on height/scroll. Expected:
    deterministic visible clipped Sixel payload for every top/bottom/both-side
    clipping case.
  - After an attempted Sixel clipping fix, run Python, Swift, and C++/C#
    `image_browser` and inspect `temp/sixel_bug.png`. Broken behavior: not only
    clipped images but also ordinary images, including the left panel/list, were
    rendered incorrectly. Expected: fixing clipping must not change non-clipped
    image placement or list thumbnails.
  - In `image_browser`, add a new image/item in the `Image items` area. The
    ScrollView correctly scrolls down immediately after insertion. Then move
    focus with arrows into that ScrollView. Broken behavior in C++/Swift/C#:
    entering the ScrollView reset its scroll position back to the top. Python
    behaved correctly. Expected: focus entry preserves the current programmatic
    scroll-to-bottom position.
  - In `image_browser`, open a browse/FileBrowser dialog over the image
    ScrollView to change an image. With the FileBrowser listbox in default edit
    mode, press arrows. Broken behavior: arrows scrolled the background image
    ScrollView behind the modal instead of moving inside the dialog listbox
    (`temp/browse_dialog.png`). Expected: modal/listbox consumes arrows and the
    background ScrollView does not move.
  - In that same browse/FileBrowser dialog, press Escape to leave the listbox
    edit mode, then move between dialog elements and press Enter on fields such
    as TextInput. Broken behavior: arrows could move between dialog elements,
    but Enter closed/accepted the dialog instead of entering the selected
    control. Expected: Enter enters/activates the focused dialog control unless
    the focused control is the accept/cancel action.
  - In browse/FileBrowser dialogs, compare `text_editor` versus `image_browser`.
    Broken behavior: mouse selection in the FileBrowser listbox worked in
    `text_editor` but not in other browse contexts. Expected: FileBrowser
    listbox mouse selection works identically regardless of which example opens
    the dialog.
  - In Swift `expense_tracker`, open the category ComboBox and try selecting
    `Health` and `Other`. Broken behavior: those values could not be selected
    because the dropdown overlapped a background ScrollView. Expected: open
    ComboBox dropdown is topmost for hit testing and selection even over a
    ScrollView background.
  - Run C# `image_browser` after
    `./uimd generate csharp/examples --target csharp && dotnet build
    csharp/examples/image_browser/image_browser.csproj --configuration Debug &&
    dotnet csharp/examples/image_browser/bin/Debug/net10.0/image_browser.dll`.
    Broken behavior: process crashed with `System.AccessViolationException` /
    segmentation fault at startup. Expected: image-heavy C# example starts and
    renders or reports a managed/runtime error, not native memory corruption.
  - In Swift `markdown_viewer`, click a ListBox item in the left panel with the
    mouse. Broken behavior: item was not selected by mouse unless the ListBox
    was first entered with Enter. Expected: direct mouse click selects the item
    like C++.
  - In Swift `special_elements`, click the ListBox with the mouse. Broken
    behavior: item selection did not work. Expected: ListBox mouse selection
    works without manually entering edit mode.
  - In Swift `widget_gallery`, click the ListBox with the mouse. Broken
    behavior: item selection did not work; same root issue as
    `markdown_viewer`/`special_elements`. Expected: mouse selection persists and
    updates the value.
  - In Swift `widget_gallery`, try selecting a ComboBox item with the mouse.
    Broken behavior: mouse selection from the dropdown did not work. Expected:
    clicked dropdown item becomes selected.
  - In Swift `widget_gallery`, try selecting text in TextInput with the mouse.
    Broken behavior: mouse text selection did not work. Expected: selected
    range is highlighted and can be replaced/copied like C++.
  - In Swift `widget_gallery`, click a SpinBox/NumberInput, type a new value,
    then click outside. Broken behavior: display reverted to the old value; if
    clicking back into the same SpinBox, the typed value reappeared. Expected:
    blur/leave commit applies the typed value.
  - In Swift `special_elements` and `widget_gallery`, wait without input.
    Broken behavior: animations did not play. Expected: animated elements
    update while idle without requiring user input.
  - In Swift `task_board`, open the `Edit task` dialog and try typing in the
    title TextInput. Broken behavior: text input did not accept typed keys.
    Expected: title TextInput enters edit mode and receives text.
  - In Swift `task_board`, open `Edit task`, focus the description TextArea,
    and type/edit text. Broken behavior: TextArea did not react to keypresses.
    Expected: TextArea behaves like C++ including multiline input and cursor
    movement.
  - In Swift `task_board`, open `Edit task`, open the assignee ComboBox and use
    arrows/Enter. Broken behavior: ComboBox keyboard navigation/selection did
    not work. Expected: arrows move active dropdown item and Enter selects it.
  - In Swift `task_board`, open `Edit task`, click one of the lower assignee
    ComboBox items. Broken behavior: assignee menu closed but priority
    ComboBox opened instead. Expected: the clicked assignee item is selected and
    no neighboring ComboBox opens from the same click.
  - In Swift `task_board`, scroll the task list after enough items exist that
    not all fit. Broken behavior: ScrollView scrolled beyond the last item and
    showed large blank space below content, visible in `temp/task_board.png`.
    Expected: bottom clamp stops when the last item is aligned with the
    viewport bottom.
  - In Swift `expense_tracker`, scroll list/ScrollView content to the bottom.
    Broken behavior: ScrollView could continue past the final item with extra
    blank space. Expected: no scroll position beyond content height.
  - In Swift `image_browser`, scroll image list/gallery content to the bottom.
    Broken behavior: after fixes there was still about 70-80% of one item of
    blank space below the last item (`scrollview_swirft_problem.png`).
    Expected: bottom clamp leaves no extra item-sized blank region.
  - ListBox multi-select styling: in `formular` or `widget_gallery`, select
    multiple ListBox items, then move the keyboard active item with arrows.
    Broken behavior: selected item and focused/active item used the same style,
    so the user could not see where keyboard focus was. Expected: selected
    style remains, active overlay is rendered on top with the agreed
    `#DDDDDD99` background and readable text.
  - ListBox active style after mouse: click ListBox items with the mouse.
    Broken behavior during refinement: active/focused item style was visible
    after mouse clicks even though it should appear only after keyboard arrow
    movement. Expected: mouse click hides active overlay; first arrow key shows
    it again.
  - Single-select ListBox behavior: in a single-select ListBox, enter edit mode,
    move with arrows, and do not press Enter yet. Broken behavior during
    refinement: arrow movement changed the selected value immediately or hid
    the original selected value. Expected: arrows move only the active item,
    original selected item remains visible, Enter confirms and exits edit mode.
  - Multi-select ListBox behavior: in a multi-select ListBox, enter edit mode,
    use arrows and Enter on several items, then press Escape. Expected behavior
    established during fixes: Enter toggles item selection without leaving edit
    mode, Escape leaves edit mode preserving selection. Tests should reproduce
    toggle, untoggle, move over selected/unselected rows, and exit.
  - Full Python/C++ compare after ListBox behavior changes failed in
    `tests/mcp/markdown_viewer.yaml`, `tests/mcp/text_editor.yaml`, and
    `tests/mcp/widget_gallery.yaml` because scripts still expected old
    single-select immediate-selection behavior. Repro: run
    `./uimd mcp-test --all --compare python/examples cpp/build/examples
    --mcp-fast --compare-app-size 90x35`. Expected tests should explicitly
    assert active-versus-selected behavior and Enter-to-confirm.
  - Full C++/C# compare failed in `tests/mcp/text_editor.yaml` after
    single-select/FileBrowser changes. Repro command reported:
    `./uimd mcp-test --backend python --headless --all --compare
    cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35`.
    Expected: FileBrowser/ListBox active row styling and confirmation path match
    C++.
  - Full C++/Swift compare failed in `widget_gallery`, `markdown_viewer`,
    `text_editor`, and `contacts_manager` after ListBox/FileBrowser active-row
    changes. Repro command reported:
    `./uimd mcp-test --backend python --headless --all --compare
    cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35`.
    Expected: Swift FileBrowser/ListBox selected/active visibility and
    callbacks match C++.
  - Swift direct terminal copy: run
    `./uimd generate swift/examples/formular --target swift && swift build
    --package-path swift/examples/formular && swift run --package-path
    swift/examples/formular formular`, type in `name_input`, select characters,
    press Cmd+C, move to `email_input`, press Cmd+V. Broken behavior: selected
    text did not copy/paste although C# did. Expected: selected text copies and
    pastes in TextInput.
  - Swift direct terminal copy with split escape: send legacy Cmd+C/Cmd+V as
    `ESC[27;9;99~` / `ESC[27;9;118~` split across reads. Broken behavior:
    Swift consumed the initial `ESC` as Escape before the rest arrived, losing
    Cmd+C. Expected: wait long enough to collect the modified-key sequence and
    parse it as `cmd_c`/`cmd_v`.
  - Swift direct terminal TextArea copy: in `formular`, enter description
    TextArea, type multiple lines, select `bc`, press Cmd+C, leave to
    `email_input`, press Cmd+V. Broken behavior: TextArea selected text did not
    copy/paste. Expected: same as C++/C#.
  - macOS/SSH clipboard fallback: direct Swift copy could update internal
    runtime clipboard and pass paste tests while `pbcopy` failed in the session;
    `osascript` still worked. Expected: Python/C++/C#/Swift attempt the same
    fallback chain so real macOS pasteboard copy works where possible.
  - Swift direct terminal copy feedback: after selected text copy in `formular`,
    especially over SSH, no visible feedback indicated the copy happened.
    Expected: transient top-right `Copied to clipboard` notification like
    C++/C#.
  For each item above, create exact repro notes first, then add the smallest
  deterministic test in the correct layer: Python unit tests, C++ runtime
  tests, direct PTY tests, MCP compare scripts with `--compare-app-size 90x35`,
  or a regression app. Do not change runtime behavior as part of the initial
  inventory step.

- [ ] **Add persistent Swift direct-terminal PTY smoke and signal restore
  coverage**. The immediate `activity_feed` keyboard/mouse control bug is fixed
  and manually validated through a PTY, but the direct-terminal smoke is not yet
  a committed reusable test entry point and abnormal external termination still
  needs the same explicit terminal restore coverage as Python/C++. Affected
  paths: `swift/src/Uimd/Sources/Uimd/Uimd.swift`, future test/tool coverage,
  and `docs/example_cli_commands.md` if a new test command is added. Required
  validation: reusable PTY smoke for arrow/Tab/mouse/`Ctrl+C`, signal restore
  check for `SIGTERM`/SIGHUP-style termination, and no regression in C++/Swift
  MCP compare. Progress: reusable raw control PTY smoke now exists in
  `tools/swift_direct_terminal_smoke.py`, is documented in
  `docs/example_cli_commands.md`, and is wired into POSIX
  `python3 tools/uimd_dev.py test-all`. It covers direct raw Tab/Shift+Tab,
  Enter, arrow, SGR wheel, ComboBox dropdown, TextArea newline, and `Ctrl+C`
  process exit parity against C++. The integrated POSIX
  `env UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib python3
  tools/uimd_dev.py test-all --no-rebuild` gate passed on 2026-07-04.
  Remaining open work is explicit abnormal signal terminal-restore coverage for
  `SIGTERM`/SIGHUP-style termination.

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
