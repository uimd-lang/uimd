# TODO

## Current Open Tasks

Date: 2026-06-21

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
