# TODO

## Current Open Tasks

Date: 2026-06-21

- [x] **Swift all-example compare fails in ListBox/FileBrowser active-row parity after single-select changes**.
  User full C++/Swift compare run on 2026-07-09 reports four step failures:
  `tests/mcp/widget_gallery.yaml [cpp, swift]` with `97 asserts passed`,
  `tests/mcp/markdown_viewer.yaml [cpp, swift]` with `11 asserts passed`,
  `tests/mcp/text_editor.yaml [cpp, swift]` with `96 asserts passed`, and
  `tests/mcp/contacts_manager.yaml [cpp, swift]` with `68 asserts passed`; all
  show `0 failed, 1 step failures`. Parity decision: reproduce focused
  C++/Swift failures, audit Swift `swift/src/Uimd/Sources/Uimd/Uimd.swift`
  standard dialogs/ListBox behavior and any generated Swift dialog outputs
  against C++ generated/runtime behavior, then fix Swift without weakening MCP
  scripts or adding example-specific workarounds. Required validation: focused
  C++/Swift compares for the failed scripts with `--compare-app-size 90x35`,
  `swift test --package-path swift/src/Uimd`, affected Swift builds if needed,
  and rerun the full user-reported C++/Swift all-example compare.
  Done: Swift ListBox/FileBrowser parity now matches the C++ single-select
  confirmation path: standard and duplicated FileBrowser code uses
  `setSelectedIndex` so callbacks run through the same post-confirmation state
  transition, Swift ComboBox/ListBox direct selected-index writes sync through a
  shared selection-change hook, and Swift ListBox selected/active visibility
  scrolling now waits until the first real render viewport is known like C++.
  The first full rerun on 2026-07-09 fixed the originally reported
  `widget_gallery`, `markdown_viewer`, `text_editor`, and `contacts_manager`
  failures and exposed an additional `image_browser_compare` pre-render
  FileBrowser scroll mismatch at step 62; that mismatch was fixed by the Swift
  `lastViewportHeight` gate. Validation passed on 2026-07-09:
  `swift test --package-path swift/src/Uimd` (`9` tests), focused C++/Swift
  compares for `widget_gallery`, `markdown_viewer`, `text_editor`,
  `contacts_manager`, and `image_browser_compare` with `--compare-app-size
  90x35`, all Swift example builds, and the full user-reported C++/Swift
  compare command (`1635 asserts passed, 0 failed, 0 step failures`).

- [x] **C# text_editor compare fails after single-select FileBrowser changes**.
  User full C++/C# compare run on 2026-07-09 reports
  `tests/mcp/text_editor.yaml [cpp, csharp]` failing with `92 asserts passed,
  0 failed, 1 step failures` while the other app scripts pass. This is
  parity-sensitive runtime/example behavior in the FileBrowser/ListBox
  confirmation path after the single-select enter-to-confirm change. Parity
  decision: reproduce the focused C++/C# `text_editor` failure, compare C#
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Dialogs.cs`, and
  `csharp/examples/text_editor/text_editor.cs` against the already validated
  C++/Python behavior, then fix C# without changing examples as a workaround.
  Required validation: focused C++/C# `text_editor` compare with
  `--compare-app-size 90x35`, C# build, and rerun the affected full C++/C#
  compare if the fix touches shared C# runtime behavior.
  Done: C# standard FileBrowser now sets the `entries` ListBox active style to
  match the C++ generated dialog (`#DDDDDD99` background and `#FFFFFF` text),
  so the focused keyboard-active row renders the same as the C++ reference
  after single-select ListBox navigation. Validation passed on 2026-07-09:
  `dotnet build csharp/examples/text_editor/text_editor.csproj --configuration
  Debug`, focused C++/C# `text_editor.yaml` compare (`215 asserts passed, 0
  failed, 0 step failures`), and the full user-reported C++/C# compare command
  (`1635 asserts passed, 0 failed, 0 step failures`) with
  `--compare-app-size 90x35`.

- [x] **Update MCP single-select ListBox scripts for enter-to-confirm behavior**.
  User full Python/C++ compare run on 2026-07-08 reports failures in
  `markdown_viewer.yaml`, `text_editor.yaml`, and `widget_gallery.yaml` after
  the single-select ListBox behavior change. These scripts still assert the old
  behavior where arrow navigation immediately changes the selected value.
  Required behavior: single-select ListBox arrow keys move only the active item,
  and `Enter` confirms selection. Multi-select behavior remains unchanged.
  Parity decision: update MCP scenarios to exercise/expect the new public
  behavior instead of reverting runtime behavior. Focused rerun found an
  additional C++ parity bug: after single-select `Enter`, the selected row can
  still render with the active ListBox overlay in compare snapshots. Cleanup
  must hide the single-select active overlay after confirmation across Python,
  C++, C#, and Swift while preserving multi-select active row visibility.
  Focused `text_editor` rerun found a second parity issue in duplicated
  FileBrowser frame callbacks: C++/C#/Swift intercepted `entries` Enter before
  the ListBox could confirm the active item, while Python standard FileBrowser
  accepts the entry only from the post-confirmation callback. Align these
  callbacks with the standard dialog behavior and validate the focused
  `text_editor` compare.
  Done: `markdown_viewer.yaml`, `widget_gallery.yaml`, and `text_editor.yaml`
  now explicitly enter ListBox edit mode, assert that single-select arrow
  movement does not immediately change selection, and confirm selection with
  `Enter`. Python, C++, C#, and Swift single-select ListBox runtimes now hide
  the keyboard-active overlay after confirmation. Standard FileBrowser and the
  duplicated `text_editor` FileBrowser callbacks now accept the selected entry
  from the post-confirmation path instead of intercepting `Enter` before
  ListBox confirmation. C++ generated-window edit confirmation now clears edit
  state before invoking callbacks so closing a modal during confirmation cannot
  leave stale state or use the old focused element. Validation passed on
  2026-07-08: focused Python ListBox tests, C++ runtime build/tests, C#/Swift
  runtime/example builds, focused Python/C++ compares for `markdown_viewer`,
  `widget_gallery`, and `text_editor`, plus full Python/C++ compare
  (`860 asserts passed, 0 failed, 0 step failures`) with
  `--compare-app-size 90x35`.

- [x] **Single-select ListBox arrows should move active item without changing selection**.
  User refinement on 2026-07-08 asks that single-select ListBox keeps the
  originally selected item visible while arrow navigation moves the
  focused/active item; Enter should confirm the active item as selected. Multi
  select must keep the current behavior. Parity decision: implement this as
  shared ListBox runtime behavior across Python, C++, C#, and Swift, not as a
  formular-specific workaround. Required validation: focused Python ListBox
  tests and `formular` compare gates with `--compare-app-size 90x35`.
  Done: Python ListBox now keeps `_active_index` separate from selected items
  for single-select arrow navigation. C++, C#, and Swift now have matching
  active-index state, with `Up`/`Down` moving only the active item and `Enter`
  confirming it as selected. Multi-select selection/toggle behavior remains
  unchanged. Validation passed on 2026-07-08: focused Python ListBox tests (`21
  passed`), Python py_compile for touched Python files, C++ `ui_cpp_tests` and
  `formular` builds, `cpp/build/ui_cpp_tests`, C# `formular` build, Swift
  `formular` build, Swift runtime tests (`9` tests), and Python/C++, C++/C#,
  C++/Swift `formular` compares with `--compare-app-size 90x35` (`167 asserts
  passed, 0 failed, 0 step failures` each).

- [x] **ListBox active item style should be keyboard-visible only**.
  User refinement on 2026-07-08 asks that mouse clicks in a ListBox must not
  display the focused/active item style. The active item style should become
  visible only after keyboard arrow movement inside ListBox edit mode, and must
  disappear again after mouse clicking an item. Parity decision: implement this
  as shared ListBox runtime behavior across Python, C++, C#, and Swift, not as
  a formular-specific workaround. Required validation: focused Python ListBox
  tests and `formular` compare gates with `--compare-app-size 90x35`.
  Done: Python, C++, C#, and Swift ListBox runtimes now keep an explicit
  keyboard-active visibility state. Entering ListBox edit mode and mouse
  selection hide the active item overlay; `Up`/`Down` shows it. Added focused
  Python coverage for keyboard-only active style visibility and mouse-click
  reset. Validation passed on 2026-07-08: focused Python ListBox tests (`21
  passed`), Python py_compile for touched Python files, C++ `ui_cpp_tests` and
  `formular` builds, `cpp/build/ui_cpp_tests`, C# `formular` build, Swift
  `formular` build, Swift runtime tests (`9` tests), and Python/C++, C++/C#,
  C++/Swift `formular` compares with `--compare-app-size 90x35` (`167 asserts
  passed, 0 failed, 0 step failures` each).

- [x] **ListBox active item theme overlay should use #DDDDDD99**.
  User refinement on 2026-07-08 asks to restyle the focused ListBox item
  background from `#CCCCCCBB` to `#DDDDDD99`. Parity decision: keep shared
  ListBox layering and active text color unchanged, update shared/package
  themes and regenerated `formular` outputs across Python, C++, C#, and Swift.
  Done: shared and packaged dark/light themes now use
  `active-background: "#DDDDDD99"` and `active-color: "#FFFFFF"` for ListBox.
  Regenerated `formular` outputs for Python, C++, C#, and Swift. Updated the
  focused Python ListBox style test to verify the new alpha-blended selected
  active background. Validation passed on 2026-07-08: focused Python ListBox
  tests (`19 passed`), C++ `formular` build, C# `formular` build, Swift
  `formular` build, and Python/C++, C++/C#, C++/Swift `formular` compares with
  `--compare-app-size 90x35` (`167 asserts passed, 0 failed, 0 step failures`
  each).

- [x] **ListBox active item theme needs darker alpha overlay and white text**.
  User validation on 2026-07-08 reports that `#FFFFFFCC` makes the focused
  ListBox item too bright and the text blends into the selected/focused row.
  Parity decision: keep the already-implemented shared ListBox style layering
  unchanged and adjust the shared dark/light themes plus regenerated outputs
  across Python, C++, C#, and Swift. Required behavior: ListBox active/focused
  item uses `active-background: "#CCCCCCBB"` and `active-color: "#FFFFFF"` so
  active background still blends over selected background but keeps readable
  text.
  Done: shared and packaged dark/light themes now use
  `active-background: "#CCCCCCBB"` and `active-color: "#FFFFFF"` for ListBox.
  Regenerated `formular` outputs for Python, C++, C#, and Swift. The focused
  Python ListBox test now verifies the blended selected+active background and
  active white foreground. Validation passed on 2026-07-08: focused Python
  ListBox tests (`19 passed`), C++ `formular` build, C# `formular` build, Swift
  `formular` build, and Python/C++, C++/C#, C++/Swift `formular` compares with
  `--compare-app-size 90x35` (`167 asserts passed, 0 failed, 0 step failures`
  each).

- [x] **ListBox active item style must alpha-blend over selected style across platforms**.
  User request on 2026-07-08 refines ListBox active-item rendering: the focused
  item should use white with alpha, for example `#FFFFFFCC`, and the selected
  style must still render underneath it. Parity decision: this is a shared
  ListBox render/style-composition behavior change across Python, C++, C#, and
  Swift, not a theme-only or formular-specific workaround. Affected paths:
  Python `src/uimd/runtime/elements.py`; C++ `cpp/src/elements/BasicElements.cpp`;
  C# `csharp/src/Uimd/Runtime/Elements.cs`; Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`; shared and packaged themes; focused
  ListBox tests and `formular` compare validation. Required behavior: ListBox
  row style composition must start from base, merge selected style when selected,
  then merge/alpha-blend active style last when the row is active in edit mode;
  theme active ListBox background should use `#FFFFFFCC`.
  Done: Python, C++, C#, and Swift ListBox row rendering now composes row style
  as base, then selected, then active, with an alpha active background blended
  over the already selected/background color instead of replacing it. The shared
  and packaged dark/light themes now use `active-background: "#FFFFFFCC"` for
  ListBox and no active foreground override. Regenerated `formular` outputs for
  Python, C++, C#, and Swift. Validation passed on 2026-07-08: focused Python
  ListBox tests (`19 passed`), C++ `uimd ui_cpp_tests formular` build,
  `cpp/build/ui_cpp_tests`, C# `formular` build, Swift `formular` build, Swift
  runtime tests (`9` tests), standalone Python `formular` MCP (`46 asserts
  passed`), and Python/C++, C++/C#, C++/Swift `formular` compares with
  `--compare-app-size 90x35` (`167 asserts passed, 0 failed, 0 step failures`
  each).

- [x] **ListBox multi-select active-item style and edit-mode parity across platforms**.
  User request on 2026-07-08 asks for ListBox behavior changes across Python,
  C++, C#, and Swift. Parity decision: implement this as shared runtime and
  generator/test behavior, not as a formular-specific workaround. Affected
  paths: Python `src/uimd/runtime/elements.py` and `src/uimd/runtime/UIBase.py`;
  C++ `cpp/src/elements/BasicElements.cpp`,
  `cpp/include/ui/elements/ListBox.hpp`, `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  and generator style propagation if needed; C#
  `csharp/src/Uimd/Runtime/Elements.cs` and
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`; Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`; `formular` generated outputs and
  `tests/mcp/formular.yaml`. Required behavior: add a dedicated active/focused
  ListBox item style distinct from selected item style; in single-select mode,
  Enter selects and commits/leaves ListBox edit mode; in multi-select mode,
  Enter only toggles the active item and Escape commits/leaves edit mode like
  TextArea; Python is the reference implementation, then C++, then C#/Swift
  must match. Required validation during implementation: focused unit tests and
  `formular` MCP compare tests with `--compare-app-size 90x35`, including
  selecting multiple items, moving across selected and unselected items,
  unselecting an item, entering edit mode, and leaving edit mode.
  Done: added `active_style`/`active-background`/`active-color` support and
  themed active ListBox item styling across Python, C++, C#, and Swift. Python
  is the reference behavior: single-select ListBox Enter selects, commits, and
  exits edit mode; multi-select ListBox Enter toggles the active item and stays
  in edit mode, while Escape commits/leaves. C++, C#, and Swift were ported to
  the same state transition and render priority, with active item style taking
  priority over selected item style and falling back to selected style when no
  active style is configured. `formular` MCP coverage now exercises entering
  the multi-select ListBox, selecting multiple items, moving over selected and
  unselected items, unselecting an item, and leaving edit mode. Focused
  validation passed on 2026-07-08: Python ListBox unit tests, standalone Python
  `formular` MCP, C++ runtime build/tests, C# `formular` build, Swift runtime
  tests and `formular` build, plus Python/C++, C++/C#, and C++/Swift
  `formular` compares with `--compare-app-size 90x35` (`167 asserts passed, 0
  failed, 0 step failures` each).

- [x] **Raw Sixel bottom clipping must crop before terminal diff emission**.
  User validation on 2026-07-06 with `temp/sixel_bug.png` reports that the
  previous broad raw-cell fix made Python, C++, C#, and Swift render Sixel
  images again, but also corrupts image output when a raw Sixel payload extends
  beyond the terminal frame. Parity decision: this is a shared runtime rendering
  bug, not an image_browser/example workaround and not a terminal/libsixel
  issue. The terminal diff layer cannot safely crop an already encoded raw
  Sixel payload; the visible image region must be clipped before payload
  encoding, and terminal buffers must keep rejecting oversized raw payloads
  instead of emitting them. Affected paths: Python `src/uimd/runtime/image.py`,
  `src/uimd/runtime/UIBase.py`, and `src/uimd/runtime/rendering.py`; C++
  `cpp/src/elements/Image.cpp`, `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  and `cpp/src/terminal/TerminalBuffer.cpp`; C#
  `csharp/src/Uimd/Runtime/Elements.cs`,
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`, and
  `csharp/src/Uimd/Runtime/Core.cs`; Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`; plus focused regression coverage.
  Required work: revert the unsafe terminal-buffer raw emission rule, audit where
  window/viewport clipping is lost before `Image` encodes Sixel, implement the
  same pre-encoding crop propagation across runtimes, and validate top, bottom,
  and both-sided clipping at small app heights without modifying examples.
  Done: the corruption root cause was confirmed from the tracked diff: Python
  `TerminalBuffer`, C++ `TerminalBuffer`, C# `TerminalBuffer`, and Swift
  `TerminalFrameBuffer` had been changed from emitting raw Sixel only when the
  full declared raw height fit in the terminal frame to emitting whenever at
  least one row was visible. That let already encoded raw Sixel payloads draw
  outside the UIMD frame and corrupt neighboring images/panels. The unsafe rule
  was reverted in all runtimes, and visible-area clipping is now propagated into
  raw image rendering before encoding through Python `UIWindow`/reusable child
  rendering, C++/C#/Swift generated-window element render state, and existing
  image visible-region crop logic. Added regressions covering pre-encoding raw
  image crop propagation and terminal-buffer rejection of bottom-clipped raw
  payloads. Validation passed on 2026-07-06: Python py_compile for touched
  runtime/tests, focused Python application tests (`6 passed`), focused Python
  image visible-region tests (`4 passed, 1 skipped` for optional libsixel
  encoder), `cmake --build cpp/build --target ui_cpp_tests`,
  `cpp/build/ui_cpp_tests`, `swift test --package-path swift/src/Uimd` (`9`
  tests), `dotnet build csharp/src/Uimd/Uimd.csproj --configuration Debug`,
  image_browser compares for Python/C++, C++/Swift, and C++/C# (`262 asserts
  passed, 0 failed, 0 step failures` each), and `git diff --check`.

- [x] **Shared raw Sixel clipping intermittently skips partially visible images**.
  User validation on 2026-07-06 reports that images render normally when fully
  visible, but when an image should be cropped by a ScrollView/visible area it
  often does not render at all. The issue is now observed in Python, C++, C#,
  and Swift, so this is a shared UIMD raw Sixel clipping/redraw bug, not a
  Swift-only port issue and not a libsixel capability issue. Parity decision:
  fix the shared runtime behavior 1:1 across Python/C++/C#/Swift and do not
  hide it with app-specific `image_browser` workarounds. Affected paths:
  Python `src/uimd/runtime/image.py` and terminal/app render flow, C++
  `cpp/src/elements/Image.cpp`, `cpp/src/terminal/TerminalBuffer.cpp`, and
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, C#
  `csharp/src/Uimd/Runtime/Elements.cs` and
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`, Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`, plus direct PTY coverage. Required
  work: reproduce an intermittently missing partially clipped Sixel image in
  `image_browser`/`image_gallery`, compare raw cell anchor, crop offset, visible
  rows, raw skip clearing, and incremental/full redraw behavior across runtimes,
  fix the canonical behavior, port the same algorithm to all targets, and
  validate with direct terminal Sixel coverage plus `--compare-app-size 90x35`
  compare gates.
  Done: the reproducible shared failure was isolated to the terminal-buffer
  scroll-region optimization in Python/C++/C#. The optimization updated the
  previous-frame buffer as if text rows had scrolled, which is unsafe when the
  region contains raw Sixel anchors or skip cells because terminals do not
  reliably scroll raw graphics with the text buffer. The diff then incorrectly
  believed the cropped Sixel image was already present and skipped re-emitting
  it. Python, C++, and C# now disable the scroll-region fast path whenever the
  affected current or previous rows contain raw image cells, forcing the normal
  diff path to clear and re-emit visible Sixel payloads. Swift was audited for
  this specific root cause and has no equivalent scroll-region previous-buffer
  mutation, so no Swift code was changed for this narrow fix; the earlier Swift
  raw clipping parity work remains covered by its separate completed task.
  Added focused regressions for top, bottom, and top+bottom Sixel visible-region
  clipping plus terminal-buffer raw scroll-region redraw. Validation passed on
  2026-07-06: Python py_compile for touched files, focused pytest coverage in
  `python/tests/test_elements.py` and `python/tests/test_application.py`,
  `cmake --build cpp/build --target ui_cpp_tests`, `cpp/build/ui_cpp_tests`,
  `dotnet build csharp/src/Uimd/Uimd.csproj --configuration Debug`, focused
  Python/C++ `image_browser_compare.yaml` (`262 asserts passed, 0 failed, 0 step
  failures`), focused C++/C# `image_browser_compare.yaml` (`262 asserts passed,
  0 failed, 0 step failures`), and `git diff --check`.
  Follow-up after user direct-terminal reproduction with `./uimd run
  python/examples/image_browser/image_browser.uimd`: Python also had a separate
  direct-terminal scroll fast path in `src/uimd/runtime/application.py` that
  manually emitted ANSI scroll-region commands and bypassed the guarded
  `TerminalBuffer.render_scroll_region()` path. That direct path now rejects the
  optimization when the current scroll-region rows contain raw Sixel anchors or
  skip cells, and uses the shared terminal-frame scroll-region helper for the
  remaining text-only fast path. Added a focused regression covering
  `_build_terminal_scroll_frame()` with current raw Sixel cells. Validation
  passed on 2026-07-06: `python3 -m py_compile
  src/uimd/runtime/application.py python/tests/test_application.py` and
  `python3 -m pytest python/tests/test_application.py -k
  'direct_terminal_scroll_frame or terminal_scroll_region_falls_back or
  terminal_buffer_raw_diff'` (`3 passed`).
  Second follow-up after repeated direct-terminal reproduction: the terminal
  raw-cell diff itself skipped any Sixel payload whose declared raw height
  exceeded the remaining terminal-buffer rows (`clear_height >= raw_height`).
  That made bottom-clipped images disappear completely in small terminal heights
  instead of allowing the terminal to draw the visible part. Python, C++, C#,
  and Swift now emit the raw payload whenever at least one target row and column
  is visible. Added Python/C++/Swift regressions for a raw Sixel payload anchored
  on the last visible row with a larger raw height. Validation passed on
  2026-07-06: targeted Python py_compile and pytest (`4 passed`),
  `dotnet build csharp/src/Uimd/Uimd.csproj --configuration Debug`, `swift test
  --package-path swift/src/Uimd` (`10` tests), `cmake --build cpp/build --target
  ui_cpp_tests`, `cpp/build/ui_cpp_tests`, and `git diff --check`.

- [x] **Swift Sixel images clipped by the visible area do not render like C++/C#**.
  User validation on 2026-07-06 reports that the standard Sixel warning dialog
  now works, but Swift still fails to display Sixel images when the image is only
  partially inside the visible viewport/ScrollView area. Parity decision: this is
  a Swift runtime rendering and terminal raw-cell clipping bug against C++/C#,
  not an example-specific image_browser/image_gallery workaround. Affected paths:
  Swift `swift/src/Uimd/Sources/Uimd/Uimd.swift` image Sixel crop/anchor logic,
  generated window clipping propagation, raw terminal cell emission, and direct
  PTY smoke coverage; references to audit are C++ `cpp/src/elements/Image.cpp`
  and `cpp/src/generated/GeneratedWindowRuntime.cpp`. Required work: reproduce a
  partially visible Sixel image in Swift direct terminal or raw render metadata,
  compare the crop offset, raw anchor row, raw height, raw skip/clearing behavior,
  and clip propagation 1:1 against C++, fix Swift only where it diverges, then
  validate with focused Swift direct terminal Sixel coverage, C++/Swift image
  compare using `--compare-app-size 90x35`, rebuild-all, and the relevant full
  parity gate.
  Done: the Swift direct-terminal renderer no longer forces the top modal layer
  into image-cell-background rendering when a modal stack is open. Background
  content underneath a modal still uses the deterministic/cell-background path,
  while the active top modal now emits normal raw Sixel like C++ and C#. MCP
  snapshot rendering keeps deterministic modal image output through a dedicated
  snapshot render path, so compare snapshots stay stable while real terminal
  output matches C++ behavior. Direct PTY smoke coverage now opens
  `image_browser`, enters the image items section, opens the `Show` preview
  dialog, and compares raw Sixel payload signatures between C++ and Swift; both
  now emit the clipped modal image at the same anchor and raster signature
  `((8, 35), '"1;1;288;288')`. Validation passed on 2026-07-06:
  `python3 -m py_compile tools/swift_direct_terminal_smoke.py`,
  `swift test --package-path swift/src/Uimd`, `swift build --package-path
  swift/examples/image_browser`, focused C++/Swift `image_browser_compare.yaml`
  (`262 asserts passed, 0 failed, 0 step failures`), `python3
  tools/uimd_dev.py rebuild-all --test`, `python3
  tools/swift_direct_terminal_smoke.py --cpp-build-dir cpp/build`, full
  `UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib python3 tools/uimd_dev.py
  test-all --no-rebuild` with Python tests, CTest, Swift runtime tests, Swift
  direct terminal smoke, Python/C++, C++/C#, C++/Swift example compares, and
  regression parity compare all passing.

- [x] **Sixel unsupported warning must use the standard UIMD MessageBox**.
  User validation on 2026-07-06 reports that the Sixel unsupported warning
  should have been implemented with the standard UIMD message box instead of a
  custom message-box-like runtime overlay. Parity decision: this is shared
  runtime/modal behavior across Python/C++/C#/Swift, not an example-specific
  drawing fix. Affected paths: Python `src/uimd/runtime/application.py`, C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp` and standard dialogs under
  `cpp/dialogs`, C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs` and
  `csharp/src/Uimd/Runtime/Dialogs.cs`, Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`, plus focused tests. Required work:
  replace the custom Sixel warning dialog renderer with a startup standard UIMD
  MessageBox modal, keep unsupported terminals on fallback, keep known
  Sixel-capable terminals such as iTerm2 on Sixel, ensure direct terminal and
  MCP/headless startup semantics remain consistent, remove now-unused custom
  warning layout constants, and validate with focused runtime tests plus
  rebuild/compare gates using `--compare-app-size 90x35`.
  Done: the custom unsupported-Sixel warning overlay was removed and replaced
  with the standard generated UIMD `MessageBoxYesNo` dialog across
  Python/C++/C#/Swift. `Yes` continues with fallback image blocks; `No` and
  `Escape` quit the direct terminal app. C++ standard message-box sources are
  now linked through the core runtime target so startup/runtime code can open
  the same modal without app-specific drawing. Direct terminal smoke coverage
  now verifies the unsupported-terminal warning as a real message box. Validation
  passed on 2026-07-06: focused Python runtime tests for the Sixel fallback
  warning, `swift test --package-path swift/src/Uimd`, `cmake --build cpp/build
  --target ui_cpp_tests`, `cpp/build/ui_cpp_tests`, `dotnet build
  csharp/src/Uimd/Uimd.csproj --configuration Debug`, `python3
  tools/uimd_dev.py rebuild-all --test`, `python3
  tools/swift_direct_terminal_smoke.py --cpp-build-dir cpp/build`, full
  `UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib python3 tools/uimd_dev.py
  test-all --no-rebuild` with Python tests, CTest, Swift runtime tests, Swift
  direct terminal smoke, Python/C++, C++/C#, C++/Swift example compares, and
  regression parity compare all passing, plus `git diff --check`.

- [x] **Warning overlay needs message-box styling and Swift direct terminal
  still clips Sixel/ScrollView content incorrectly**. User validation on
  2026-07-06 reports that the new Sixel unsupported warning dialog is visually
  too harsh and should look more like a neutral message box with larger vertical
  and horizontal padding, ideally with continue/quit affordance and auto-close;
  `temp/sixel_swift_problem.png` shows Swift Sixel images in iTerm2 are clipped
  incorrectly when partially visible in a ScrollView (top-clipped images are
  shifted from the beginning instead of cropping their top, and bottom-clipped
  images are not drawn); `temp/scrollview_swirft_problem.png` shows Swift
  ScrollView still leaves a visible empty area after the final item in direct
  terminal despite compare gates passing. Parity decision: warning presentation
  is shared runtime UX across Python/C++/C#/Swift; Sixel clipping and remaining
  bottom slack are Swift runtime parity bugs against C++/Python behavior. Affected
  paths: Python `src/uimd/runtime/application.py`, C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, C#
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`, Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`, plus focused runtime tests and direct
  terminal/MCP validation. Required work: restyle warning as a neutral
  message-box-like overlay with at least two rows of padding above/below the text
  and 4-6 columns of side padding, decide and implement safe continue/quit
  behavior without breaking MCP/headless startup, fix Swift Sixel visible-region
  crop math, fix the remaining Swift direct-terminal ScrollView bottom slack,
  and validate against the referenced screenshots and C++/Swift compare gates.
  Done: the Sixel unsupported warning now uses a shared neutral
  message-box-like overlay across Python/C++/C#/Swift, with two rows of vertical
  padding, five columns of side padding, neutral foreground/background/border
  colors, and the same auto-closing behavior as before. Decision: the proposed
  Continue/Quit buttons were not added in this fix because they would turn a
  passive startup degradation notice into a blocking modal input flow across
  direct terminal, MCP/headless, and every runtime; implement that as a separate
  cross-runtime modal behavior change if explicitly requested. Swift Sixel
  rendering now propagates clip bounds through generated windows, reusable
  elements, and ScrollView rendering, then emits raw Sixel for the visible region
  only with the correct anchor and crop offset, matching the C++ visible-region
  behavior for top- and bottom-clipped images. Swift direct-terminal ScrollView
  wheel routing now stops at generated ScrollView boundaries instead of falling
  through to the generic child-index scroller at the bottom, removing the
  remaining trailing slack after the last item. Validation passed on 2026-07-06:
  `swift test --package-path swift/src/Uimd` (`9` tests), `python3 -m
  py_compile tools/swift_direct_terminal_smoke.py`, `python3 -m py_compile
  src/uimd/runtime/application.py python/tests/test_application.py`, `python3
  -m pytest python/tests/test_application.py -k notification`, focused
  C++/Swift `image_browser_compare.yaml` (`262 asserts passed, 0 failed, 0 step
  failures`), `python3 tools/uimd_dev.py rebuild-all --test`, full
  `UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib python3 tools/uimd_dev.py
  test-all --no-rebuild` with Python tests, CTest, Swift runtime tests, Swift
  direct terminal smoke, Python/C++, C++/C#, C++/Swift example compares, and
  regression parity compare all passing, plus `git diff --check`.

- [x] **Sixel warning must be a real dialog, iTerm2 must default to Sixel, and
  Swift ScrollView bottom clamp still leaves trailing slack**. User validation
  on 2026-07-06 reports that unsupported-terminal Sixel degradation currently
  renders only a short top-right notification instead of a warning dialog, that
  iTerm2 is incorrectly auto-degraded to fallback even though it supports Sixel,
  and that Swift ScrollViews no longer overscroll by whole items but still show
  empty space under the last item of roughly 70-80% of an item height. Parity
  decision: warning presentation and Sixel capability detection are shared
  runtime behavior across Python/C++/C#/Swift; the remaining trailing slack is a
  Swift runtime ScrollView parity bug against C++/C#/Python. Required work:
  replace the transient top-right Sixel notification with a real startup warning
  dialog/overlay, fix terminal detection so iTerm2 defaults to Sixel without
  requiring `UIMD_FORCE_SIXEL`, keep unsupported terminals such as Apple Terminal
  on fallback, and tighten Swift ScrollView content height/max offset so the
  bottom of the final rendered child aligns with the viewport without extra
  trailing slack. Required validation: focused unit/direct or MCP coverage for
  iTerm2 Sixel capability, unsupported-terminal warning dialog rendering, and
  Swift bottom clamp in `task_board`, `expense_tracker`, and `image_browser`,
  then rebuild affected outputs and run focused C++/Swift compares with
  `--compare-app-size 90x35`. Done: unsupported-terminal Sixel degradation now
  renders as a centered warning dialog/overlay instead of the short top-right
  notification, while normal info notifications keep the compact placement.
  Python/C++/C#/Swift terminal capability detection now treats iTerm2
  (`ITERM_SESSION_ID` or `LC_TERMINAL=...iTerm...`) as Sixel-capable by default
  and still keeps Apple Terminal on fallback unless explicitly forced. Swift
  ScrollView child-height and max-offset calculations now use one cached
  rendered-height path for content height, visible child lookup, focus keeping,
  and rendered rows, eliminating the remaining bottom slack. Validation passed
  on 2026-07-06: `python3 -m py_compile src/uimd/runtime/image.py
  src/uimd/runtime/application.py python/tests/test_application.py
  python/tests/test_elements.py`, focused Python warning and terminal detection
  tests, `swift test --package-path swift/src/Uimd`, `cmake --build cpp/build
  --target ui_cpp_tests`, `cpp/build/ui_cpp_tests`, `dotnet build
  csharp/src/Uimd/Uimd.csproj --configuration Debug`, `python3
  tools/uimd_dev.py rebuild-all --test`, CTest `26/26`, focused C++/Swift
  compares for `task_board_compare.yaml` (`333 asserts passed`),
  `expense_tracker_compare.yaml` (`200 asserts passed`), and
  `image_browser_compare.yaml` (`262 asserts passed`), all with `0 failed, 0
  step failures`, plus `git diff --check`.

- [x] **Terminal image capability detection, Sixel fallback warning, and Swift
  ScrollView overscroll parity**. User validation on 2026-07-06 compares
  `temp/iterm2_vs_terminal_macos_sixel.png` and
  `temp/iterm2_vs_terminal_macos_fallback.png`: iTerm2 renders Sixel while
  macOS Terminal prints raw Sixel payload as text, fallback rendering differs
  between terminals because terminal cell pixel geometry differs, and Swift
  ScrollViews in `task_board`, `expense_tracker`, and `image_browser` can scroll
  beyond the last item while C# does not. Parity decision: terminal image
  capability detection and fallback degradation are shared runtime behavior
  across Python/C++/C#/Swift; Swift overscroll is a Swift runtime parity bug.
  Required work: detect unsupported Sixel terminals even when a `.uimd` image
  explicitly asks for Sixel, render fallback instead of raw payload, show a
  startup runtime warning once when Sixel images are degraded, document/support
  known Sixel-capable terminals or explicit `UIMD_FORCE_SIXEL`, make fallback
  aspect use real terminal cell pixel geometry when available and otherwise
  degrade consistently, and fix Swift ScrollView max offset calculation to
  match C++/C#/Python. Required validation: direct terminal or MCP coverage for
  Sixel unsupported fallback, image render info mode resolution, and Swift
  ScrollView bottom clamp; rebuild all examples and run focused C++/Swift
  compares with `--compare-app-size 90x35`. Done: Python/C++/C#/Swift now use
  conservative terminal Sixel capability detection. Explicit Sixel degrades to
  fallback on unsupported terminals, `UIMD_FORCE_SIXEL=1` keeps raw Sixel
  available for known-capable terminals or SSH setups, and
  `UIMD_DISABLE_SIXEL=1` disables it. All runtimes show one transient startup
  notification when a non-fallback image is degraded. Swift image fallback uses
  terminal cell pixel geometry when available, guarded so non-image apps are not
  probed, and Swift ScrollView content height and bottom clamping now match
  C++/C#/Python. Validation passed on 2026-07-06: focused Python image terminal
  detection tests, `swift test --package-path swift/src/Uimd`,
  `cmake --build cpp/build --target ui_cpp_tests`, `cpp/build/ui_cpp_tests`,
  `dotnet build csharp/src/Uimd/Uimd.csproj --configuration Debug`,
  `python3 -m py_compile src/uimd/runtime/image.py
  src/uimd/runtime/application.py python/tests/test_elements.py`, `python3
  tools/uimd_dev.py rebuild-all --test`, focused C++/Swift
  `image_gallery_sixel_info_compare.yaml` (`52 asserts passed, 0 failed, 0 step
  failures`), and full `UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib
  python3 tools/uimd_dev.py test-all --no-rebuild` with Python tests, CTest,
  Swift runtime tests, Swift direct terminal smoke, Python/C++, C++/C#, C++/Swift
  example compares, and regression parity compare all passing.

- [x] **Swift fallback image rendering and ListBox mouse selection still fail in
  direct terminal despite prior parity gate**. User validation on 2026-07-06
  reports that `temp/fallback.png` still shows visibly incorrect Swift fallback
  image rendering versus the C++ behavior, and ListBox items still cannot be
  selected by mouse unless the ListBox is first entered with Enter. Parity
  decision: treat this as a Swift runtime/rendering parity bug and a direct PTY
  coverage gap, not as an example-specific issue and not as a reason to trust
  existing MCP image metadata checks. Affected Swift paths:
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` image fallback raster-to-half-block
  rendering, ANSI terminal cell output, mouse target routing, and ListBox
  direct-terminal press/release handling. References to audit: Python
  `src/uimd/runtime/image.py`, C++ `cpp/src/elements/Image.cpp`,
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, and direct PTY coverage in
  `tools/swift_direct_terminal_smoke.py`. Required validation: reproduce raw
  C++/Swift fallback output difference with color-aware terminal capture,
  reproduce ListBox mouse selection without pre-entering edit mode, fix Swift
  structurally against C++, then rerun the focused direct PTY smoke and relevant
  C++/Swift compare gates with `--compare-app-size 90x35`. Done: the fallback
  validation was corrected to run both C++ and Swift from the repository root
  with absolute binaries, so both runtimes resolve the same image assets before
  comparing terminal cells. With that fixed, raw fallback cell parity passes and
  verifies the upper-half block contract: `▀` foreground is the visual top pixel
  and background is the visual bottom pixel. The remaining Swift ListBox mouse
  bug was real: Swift `ListBox` inherits from `ComboBox`, so generic ComboBox
  hit-testing had incorrectly shrunk ListBox mouse targets to one row and
  treated ListBox edit mode as a ComboBox dropdown. Swift hit routing now
  excludes `ListBox` from those ComboBox-only paths, matching C++ where ListBox
  is not a ComboBox. Validation passed on 2026-07-06:
  `python3 -m py_compile tools/swift_direct_terminal_smoke.py`, `swift test
  --package-path swift/src/Uimd`, focused `widget_gallery.yaml` C++/Swift
  compare (`102 asserts passed, 0 failed, 0 step failures`),
  `python3 tools/uimd_dev.py rebuild-all --test`, post-rebuild
  `python3 tools/swift_direct_terminal_smoke.py --cpp-build-dir cpp/build`
  including `image_browser fallback styled screen parity`,
  `formular listbox mouse selection persists`, and
  `widget_gallery listbox mouse selection persists`, plus post-rebuild
  `image_browser_compare.yaml` C++/Swift compare (`262 asserts passed, 0
  failed, 0 step failures`).

- [x] **Swift direct-terminal modal and ScrollView raw interaction regressions
  still escape MCP coverage**. User validation on 2026-07-05 reports that
  ListBox item selection by mouse still does not work unless the ListBox is
  first entered with Enter; in Swift `task_board`, the `Edit task` dialog opened
  from a ScrollView row is almost non-interactive in direct terminal: ComboBox
  arrow/open/select behavior, TextInput editing, TextArea editing, and mouse
  targeting do not work even though a simpler `activity_feed` settings dialog
  works; and an image/list ScrollView still jumps back to the top when entering
  it after auto-scroll moved it to the bottom. Parity decision: these are raw
  direct-terminal Swift runtime/focus/modal/ScrollView routing defects and test
  coverage gaps, not app-specific example workarounds and not reasons to trust
  high-level MCP compare alone. Affected paths: Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` terminal mouse/key dispatch,
  window/modal active-frame routing, active ScrollView scope, ListBox/ComboBox
  click targeting, TextInput/TextArea direct editing, and reusable child focus
  inside modal dialogs opened from ScrollView rows; coverage path:
  `tools/swift_direct_terminal_smoke.py`. C++ direct terminal behavior is the
  reference for the same raw PTY scenarios. Required validation: add PTY
  reproductions that click a ListBox item without pre-entering edit mode, open
  `task_board` Edit Task from a row and edit TextInput/TextArea/ComboBox with
  raw keys and mouse, and verify entering an auto-scrolled image/list
  ScrollView preserves bottom position; fix Swift 1:1 against C++ and rerun
  focused direct PTY smoke plus relevant compare gates. Additional user
  validation on 2026-07-05 reports that terminal fallback image rendering is
  still visibly wrong, with the two pixels encoded into each half-block cell
  appearing swapped. Done: Swift active modal/frame state no longer falls back
  to the root window while a dialog is active, active ScrollView entry preserves
  remembered or visible descendants instead of jumping to the first child, and
  the Swift direct PTY smoke now covers raw ListBox mouse selection, `task_board`
  dialog TextInput mouse editing, dialog ComboBox arrow selection, animations,
  text selection, NumberInput blur commit, checkbox traversal, and edit-mode
  arrow routing. Focused C++/Swift compares for `image_browser_compare.yaml` and
  `task_board_compare.yaml` passed after the fixes. The real fallback renderer
  was fixed in Python/C++/Swift/C# to use the same area sampling, cover/contain
  geometry, and upper-half block mapping, with explicit half-block tests that
  assert `▀` foreground is the visual top pixel and background is the visual
  bottom pixel in Python, C++, and Swift. Swift `get_image_render_info` no
  longer returns filename-hardcoded sample signatures; it computes signatures
  from the decoded raster with the same resize/sampling path as rendering.
  Validation passed on 2026-07-05:
  `UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib python3
  tools/uimd_dev.py test-all --no-rebuild`: Python tests `463 passed`, CTest
  `26/26`, Swift runtime tests passed, Swift direct terminal smoke passed,
  Python/C++ example compare `844 asserts passed`, C++/C# example compare
  `1599 asserts passed`, C++/Swift example compare `1599 asserts passed`
  including `image_gallery_sixel_info_compare.yaml` `52 asserts passed`, and
  regression parity compare `14 asserts passed`, all with `0 failed, 0 step
  failures`. Follow-up post-fix rebuild validation also passed:
  `python3 tools/uimd_dev.py rebuild-all --test` regenerated all supported
  outputs, rebuilt C++/C#/Swift examples, compiled Python sources, and ran CTest
  `26/26`; post-rebuild focused `image_gallery_sixel_info_compare.yaml` passed
  for Python/C++, C++/C#, and C++/Swift with `52 asserts passed, 0 failed, 0
  step failures` for each pair.

- [x] **Cross-platform direct-terminal interaction regressions after Swift parity
  pass**. User validation on 2026-07-05 reports remaining runtime parity issues
  outside the existing MCP gates: `activity_feed` should auto-scroll to the
  newly appended activity at the bottom across Python/C++/C#/Swift; Python
  currently does not scroll after add and Swift only scrolls by roughly one item
  when the view was not already at bottom. `widget_gallery` animations do not
  visibly tick in direct terminal; ListBox mouse selection, TextInput mouse text
  selection, and NumberInput/SpinBox commit-on-blur are broken. `image_browser`
  still has Sixel/fallback display issues in direct terminal and preview/show
  mode, image list ScrollView position can reset when entering the ScrollView in
  C++/Swift/C#, FileBrowser/Browse dialog ListBox arrows can route to the
  background image ScrollView instead of the top modal, and C# direct
  `image_browser` can crash with an `AccessViolationException`. In
  `expense_tracker`, ComboBox options that overlap an underlying ScrollView
  background cannot be selected. Parity decision: these are shared runtime,
  renderer, terminal-input, modal-stack, ScrollView, image, and generated
  language-port defects, not app-specific fixes and not reasons to weaken MCP
  masks. Affected paths to audit: Python `src/uimd/runtime/*`, C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, `cpp/src/elements/*`,
  C# `csharp/src/Uimd/Runtime/*`, Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`, image decoder/runtime paths, and
  generated examples only when generator/runtime wiring changes. Required
  validation: reproduce with direct PTY/manual-equivalent smoke where MCP misses
  the behavior, add reusable coverage for auto-scroll-on-append, modal ListBox
  arrow routing, mouse ListBox/TextInput selection, ComboBox overlay selection,
  NumberInput blur commit, animation ticking, image fallback/Sixel metadata, and
  C# `image_browser` startup; fix the shared implementation 1:1 across
  supported runtimes; regenerate/build all examples; rerun focused direct
  terminal checks and C++/Python/C#/Swift compare gates with
  `--compare-app-size 90x35`. Done: activity feed append auto-scroll now
  scrolls fully to the newly appended item when the app-level auto-scroll option
  is enabled across Python/C++/C#/Swift, with Python tests covering enabled and
  disabled behavior. Swift direct terminal raw input now uses nonblocking reads
  with idle ticking so animations render while idle, and direct PTY smoke covers
  animation ticking, ComboBox/ListBox mouse selection, TextInput mouse text
  selection, NumberInput blur commit, checkbox focus traversal, edit-mode arrow
  routing, and ScrollView scope. C++/Swift/C# ScrollView scope entry now matches
  Python by preferring remembered or visible descendants before falling back to
  the first child, preventing image-browser/list ScrollView resets after
  programmatic scroll. C# direct terminal startup no longer calls the unsafe
  macOS `TIOCGWINSZ` pixel-size ioctl path and uses the correct Darwin poll
  signature; direct PTY startup of C# `image_browser` now exits cleanly instead
  of crashing. Image fallback/Sixel parity was audited across Python/C++/Swift/C#:
  fallback uses the same upper-half block mapping (foreground = top pixel,
  background = bottom pixel), and raw Sixel output over SSH means the remote app
  emitted Sixel but the local terminal/client must support and enable Sixel to
  display it. Validation passed on 2026-07-05: `python3 -m pytest
  python/tests/test_activity_feed.py` (`16 passed`), C# activity_feed rebuild
  with `0 Warning(s)`, C# `image_browser` PTY startup with `exit=0`,
  `python3 tools/uimd_dev.py rebuild-all --test`, and
  `UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib python3 tools/uimd_dev.py
  test-all --no-rebuild`: Python tests `462 passed`, CTest `26/26`, Swift
  runtime tests passed, Swift direct terminal smoke passed, Python/C++ example
  compare passed, C#/C++ example compare passed, Swift/C++ example compare
  `1599 asserts passed, 0 failed, 0 step failures`, and regression parity
  compare `14 asserts passed, 0 failed, 0 step failures`.

- [x] **Swift interactive runtime still has broad direct-terminal parity gaps
  versus C++**. User validation on 2026-07-05 reports multiple Swift example
  failures that are not covered by the current MCP compare gate: CheckBox focus
  traps in dialog windows; TextArea cannot move vertically with arrows or
  extend multi-character selections; TextInput multi-character selection is
  incomplete and `expense_tracker` exits edit mode on Down; MessageBox Yes/No
  Escape does not briefly highlight the rejected No button; FileBrowser/Browse
  dialog arrow, mouse, ListBox edit-mode, TextInput enter, and modal routing are
  broken; ListBox/ComboBox mouse selection and ComboBox arrow-menu navigation
  fail in several examples; `task_board` dialog TextArea/TextInput/ComboBox
  input is broken; ScrollView focus handoff resets image item scroll position
  and `task_board` can scroll past useful content; Swift terminal titles show
  `[python]`; `image_browser` and `image_gallery` do not visibly render images;
  animations do not run in `special_elements` and `widget_gallery`. Parity
  decision: these are Swift runtime/generator/example-port parity defects, not
  app-specific fixes and not reasons to weaken MCP masks. Affected Swift paths:
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` terminal input parser/event routing,
  `GeneratedWindowBase` modal stack/focus/edit-mode/mouse/scroll dispatch,
  `TextInput`, `TextArea`, `ListBox`, `ComboBox`, `CheckBox`, `Image`,
  `Animation`, `UIScrollView`, terminal title setup, and generated example
  outputs under `swift/examples`; generator path:
  `cpp/tools/uimd/NativeSwiftGenerator.cpp` if generated Swift public/runtime
  wiring differs from C++. References to audit: Python `src/uimd/runtime/*`,
  C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  `cpp/src/elements/*`, `cpp/src/terminal/*`, and generated C++ examples.
  Required validation: reproduce the listed bugs through direct PTY and focused
  app compares, add reusable coverage to `tools/swift_direct_terminal_smoke.py`
  or MCP YAML where the current tests miss user-visible behavior, fix Swift to
  match C++ 1:1 without example workarounds, regenerate/build all examples
  after runtime/generator fixes, and rerun focused checks plus the broad
  C++/Swift compare with `--compare-app-size 90x35`. Done: Swift runtime,
  generated wiring, image decoding, modal/focus routing, TextInput/TextArea
  editing, selection, ComboBox/ListBox keyboard and mouse behavior, CheckBox
  focus movement, MessageBox escape handling, ScrollView focus handoff, terminal
  title suffixes, and animation ticking were brought back into parity without
  app-specific workarounds. Direct PTY smoke now covers Swift terminal title
  suffix, TextInput and TextArea shift-selection replacement, TextArea vertical
  arrow editing, ComboBox arrow selection, settings-dialog CheckBox focus
  traversal, `expense_tracker` TextInput Down staying in edit mode, ComboBox
  mouse selection, and ListBox mouse selection. Validation passed:
  `python3 tools/uimd_dev.py rebuild-all --test`, `python3 -m py_compile
  tools/swift_direct_terminal_smoke.py`, `swift test --package-path
  swift/src/Uimd`, focused C++/Swift compares for `image_browser_compare`,
  `markdown_viewer`, `widget_gallery`, `expense_tracker_compare`, and
  `task_board_compare`, and the full outside-sandbox POSIX gate
  `UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib python3 tools/uimd_dev.py
  test-all --no-rebuild`: Python tests `460 passed`, CTest `26/26`, Swift
  runtime tests passed, Swift direct terminal smoke passed, Python/C++ example
  compare `844 asserts passed`, C#/C++ example compare `1599 asserts passed`,
  C++/Swift example compare `1599 asserts passed`, and regression parity compare
  `14 asserts passed`, all with `0 failed, 0 step failures`.

- [x] **Swift direct terminal raw input still misses core interactive control
  parity**. User validation on 2026-07-03 reports that Swift direct terminal
  apps still cannot scroll with mouse or arrows, cannot open ComboBox dropdowns,
  and TextArea does not insert new lines on Enter. Parity decision: this is the
  raw direct-terminal Swift runtime path, not an example issue and not a reason
  to weaken compare expectations. MCP compare tests can pass while missing these
  bugs if the YAML uses tester tool calls that bypass the raw terminal parser or
  does not cover these exact edit-mode interactions. Affected Swift paths:
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` `TerminalInputParser`,
  `runTerminal`, `dispatchTerminalInputEvent`, `handleKeyMutation`,
  `TextArea.handleKey`, `ComboBox.handleKey`, scroll routing, and future direct
  PTY smoke coverage. C++ references: `cpp/src/terminal/Input.cpp`,
  `cpp/src/terminal/TerminalBackend.cpp`, and
  `cpp/src/generated/GeneratedWindowRuntime.cpp` key/mouse/scroll dispatch.
  Required validation: direct PTY smoke that sends raw arrow, Enter, SGR
  wheel/mouse, ComboBox open/select, and TextArea newline input to Swift example
  binaries; focused MCP compare must still pass; add/update reusable coverage
  so these direct terminal failures are caught before user testing. Done:
  current Swift direct terminal behavior was validated against C++ with the new
  reusable POSIX PTY smoke `python3 tools/swift_direct_terminal_smoke.py
  --cpp-build-dir cpp/build`. The smoke starts the real C++ and Swift binaries,
  sends raw Tab/Shift+Tab/Enter/arrow/SGR wheel input, and compares the terminal
  screen for `formular` ComboBox dropdown opening, `formular` TextArea newline
  insertion, `activity_feed` mouse-wheel scrolling, and `activity_feed` arrow
  scrolling inside the active ScrollView scope. Validation passed on
  2026-07-04 with all four smoke cases passing, `python3 -m py_compile
  tools/swift_direct_terminal_smoke.py tools/uimd_dev.py`, and the existing
  C++/Swift all-example MCP compare gate with `1599 asserts passed, 0 failed,
  0 step failures`. Integrated POSIX validation also passed outside the sandbox
  with `env UIMD_LIBSIXEL_DIR=/opt/homebrew/opt/libsixel/lib python3
  tools/uimd_dev.py test-all --no-rebuild`: Python tests `460 passed`, CTest
  `26/26`, Swift runtime tests passed, Swift direct terminal smoke passed, MCP
  Python/C++ example compare passed, MCP C#/C++ example compare passed, MCP
  Swift/C++ example compare passed with `1599 asserts passed, 0 failed, 0 step
  failures`, and regression parity compare passed with `14 asserts passed, 0
  failed, 0 step failures`. Abnormal signal terminal-restore coverage remains
  tracked separately.

- [x] **Swift full C++ compare still has scroll/reusable background parity
  mismatches**. After fixing the direct raw input path, the full
  `./uimd mcp-test --backend python --headless --all --compare
  cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35` gate on
  2026-07-03 still failed in `tests/mcp/image_browser_compare.yaml`,
  `tests/mcp/markdown_viewer.yaml`, `tests/mcp/task_board_compare.yaml`, and
  `tests/mcp/expense_tracker_compare.yaml` with snapshot background mismatches
  after interactive steps. Example snapshots include
  `tests/mcp/snapshots/20260703-204243-181300-step-004-task_board_compare.json`
  row 7 col 27 expected C++ `#07111f` but Swift rendered `#030712`, and
  `tests/mcp/snapshots/20260703-204315-563189-step-015-expense_tracker_compare.json`
  row 12 col 28 expected C++ `#293143` but Swift rendered `#172033`. Parity
  decision: this is Swift render/runtime parity for generated ScrollView,
  UIElementReusable/ViewHost focus backgrounds and child blitting, not a reason
  to weaken compare masks or change example logic. Affected paths:
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` generated render/content composition,
  ScrollView indicator/background handling, reusable host focus background, and
  ViewHost child rendering; C++ reference:
  `cpp/src/generated/GeneratedWindowRuntime.cpp` renderGeneratedWindowContent,
  reusable focus/background handling, active scroll scope dimming, and
  ScrollView child blit order. Required validation: focused compare for every
  failed script with `--compare-app-size 90x35`, then rerun the all-example
  C++/Swift compare; keep raw PTY ComboBox/TextArea/wheel smoke passing. Done:
  Swift `expense_tracker` now uses the same child `ReusableElement`/ScrollView
  row structure as C++ instead of a Swift-only manual row renderer, and the
  Swift runtime was aligned with the C++ reusable active-scroll, ComboBox edit
  scope, modal dimming, and modal-close scroll-position behavior. Validation
  passed: `swift build --package-path swift/src/Uimd`, `swift test
  --package-path swift/src/Uimd`, focused C++/Swift compares for
  `tests/mcp/expense_tracker_compare.yaml` (`200 asserts passed`),
  `tests/mcp/image_browser_compare.yaml` (`262 asserts passed`),
  `tests/mcp/markdown_viewer.yaml` (`52 asserts passed`), and
  `tests/mcp/task_board_compare.yaml` (`333 asserts passed`), broad
  `python3 tools/uimd_dev.py rebuild-all --test` regeneration/build gate with
  C++ `ctest` `26/26` passed, `python3 -m pytest python/tests` with
  `459 passed, 1 skipped`, and the full all-example C++/Swift gate
  `./uimd mcp-test --backend python --headless --all --compare
  cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35` with
  `1599 asserts passed, 0 failed, 0 step failures`. Raw direct-terminal Swift
  PTY coverage remains tracked separately and was not closed by this MCP
  compare fix.

- [x] **Swift local validation gate missing from dev helper and command docs**.
  Swift examples, runtime tests, and C++/Swift MCP compare commands exist, but
  the central POSIX `tools/uimd_dev.py` `rebuild-all`/`test-all` flow and the
  top `docs/example_cli_commands.md` full-gate block still describe only
  Python, C++, and C#. Parity decision: this is developer validation tooling and
  documentation for the Swift target; generated Swift runtime behavior and
  example logic must not be changed. Required validation: compile the updated
  helper, confirm Swift generation/build/test commands are represented in the
  documented full gate, and keep Windows commands unchanged unless Swift Windows
  support is explicitly validated. Done: POSIX `rebuild-all`/`test-all` now
  generate Swift examples, build all SwiftPM example packages, run
  `swift test --package-path swift/src/Uimd`, and run C++/Swift all-example MCP
  compare through the Python tester backend; `--no-swift` is available for
  intentionally unavailable Swift toolchains, and Windows validation remains
  skipped. Validated with `python3 -m py_compile tools/uimd_dev.py` and helper
  `--help` output for both commands; full Swift build/compare gate was not run
  in this documentation/helper update.

- [x] **C++/Swift all-example MCP compare had a non-reproduced step failure**. The command
  `./uimd mcp-test --backend python --all --compare cpp/build/examples
  swift/examples --mcp-fast --compare-app-size 90x35` reports `216 asserts
  passed, 0 failed, 1 step failures`. Parity decision: this is Swift target
  validation against the C++ parity baseline; investigate the reported script
  and step before changing examples or test expectations. Affected paths are
  expected to be under Swift runtime/generator/example code
  (`swift/src/Uimd`, `cpp/tools/uimd/NativeSwiftGenerator.*`,
  `swift/examples`) unless the reproduced failure points to tester plumbing.
  Required validation: reproduce the failing script/step, inspect the snapshot
  or error output, fix the shared Swift behavior without app-specific
  workarounds, and rerun the focused C++/Swift compare plus the all-example
  compare with `--compare-app-size 90x35`. Investigation result: the same
  all-example command was rerun and passed with `1599 asserts passed, 0 failed,
  0 step failures`; based on the original `216` passed count, the likely failing
  point was the next script after `formular`, `tests/mcp/widget_gallery.yaml`.
  A focused C++/Swift `widget_gallery.yaml` compare also passed with
  `102 asserts passed, 0 failed, 0 step failures`. No runtime or generator code
  change was made because the failure was not reproducible and no new failure
  snapshot was created.

- [x] **Swift direct terminal activity_feed cannot be controlled reliably**.
  Running `./uimd generate swift/examples/activity_feed --target swift &&
  swift build --package-path swift/examples/activity_feed && swift run
  --package-path swift/examples/activity_feed activity_feed` leaves the app
  visible but not practically controllable by the user's arrow keys or mouse,
  and the app had to be killed externally. Parity decision: this is direct
  terminal Swift runtime behavior, not an MCP compare failure; the previous
  C++/Swift MCP compare only validates tester-driven key/mouse tool paths.
  Affected paths: Swift direct terminal input and teardown in
  `swift/src/Uimd/Sources/Uimd/Uimd.swift`, generated/runtime options and
  focus initialization for `swift/examples/activity_feed`, and the corresponding
  Python/C++ direct terminal behavior for focus, mouse mode, `Ctrl+C`, and
  terminal restore. Required validation: reproduce in a PTY/direct terminal,
  verify initial focus/Tab/arrows/mouse click/`Ctrl+C` behavior, ensure abnormal
  termination does not leave the terminal in raw/alternate-screen/mouse mode,
  and add direct-terminal smoke coverage that is separate from MCP compare.
  Done: Swift navigation now matches the Python/C++ empty-focus behavior for
  Tab and arrow navigation; arrow keys from no focus select the first focusable
  element instead of doing nothing. Validated with `swift test --package-path
  swift/src/Uimd`, `swift build --package-path swift/examples/activity_feed`,
  PTY direct-terminal `activity_feed` smoke for `Down` then `Ctrl+C`
  (`exit=0`), PTY SGR mouse click on the `Quit` button (`exit=0`), and focused
  C++/Swift `tests/mcp/activity_feed.yaml` compare with `47 asserts passed, 0
  failed, 0 step failures`. The broad `python3 tools/uimd_dev.py rebuild-all
  --test` gate also passed outside the sandbox after the sandboxed run hit
  SwiftPM cache write permissions; this regenerated all supported example
  outputs, rebuilt C++/C#/Swift examples, and finished C++ `ctest` with 26/26
  tests passed.

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

- [x] **Swift direct terminal input/render loop is extremely slow**. After the
  empty-focus navigation fix, manual `activity_feed` control responds but each
  visible frame can take multiple seconds. Parity decision: this is Swift
  direct-terminal runtime performance, not an example-specific issue and not an
  MCP compare semantics change. Likely affected path:
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` direct terminal input dispatch versus
  MCP tool return/snapshot generation. Required validation: measure direct PTY
  key latency before/after, keep C++/Swift MCP compare passing, and ensure direct
  key/mouse paths no longer do unnecessary MCP JSON/snapshot work when their
  return value is ignored by the terminal loop. Done: Swift direct terminal
  dispatch now follows the C++ structure more closely: local key, paste, mouse,
  and wheel events call runtime mutation helpers directly, while MCP tool
  wrappers convert those same mutations to `get_state` or element snapshot
  payloads only for MCP responses. The temporary `suppressEventReturnPayload`
  flag was removed so the direct path is not a one-off wrapper around MCP
  behavior. PTY `Down` key latency on `activity_feed` measured at about
  70-90 ms per frame after the structural split, averaging about 76 ms, instead
  of multiple seconds. Validated with `swift test --package-path swift/src/Uimd`,
  `swift build --package-path swift/examples/activity_feed`, and focused
  C++/Swift `tests/mcp/activity_feed.yaml` compare with `47 asserts passed, 0
  failed, 0 step failures`. The broad `python3 tools/uimd_dev.py rebuild-all
  --test` gate also passed outside the sandbox after the structural split,
  rebuilding all C++/C#/Swift examples and finishing C++ `ctest` with 26/26
  tests passed.

- [x] **Swift direct terminal repaint/input path still lags behind C++**. User
  validation after the MCP/direct-event split still reports Swift interactive
  terminal rendering as slower than Python. Investigation first found and partly
  addressed the full-frame write gap by adding a Swift terminal diff buffer and
  image raster/render caches, but the remaining structural parity issue is the
  terminal input/render loop: Swift still parses one event per loop by sleeping
  after `ESC` and draining all pending bytes into a single escape sequence,
  while C++ uses `InputParser::feed(...)` to parse a byte stream into multiple
  events, coalesces consecutive wheel events, tracks dirty frames, and renders
  once for the resulting batch. Fast mouse/scroll input can therefore create a
  visible backlog in Swift or collapse multiple escape sequences into an
  unhandled `Escape`. Parity decision: this is Swift terminal backend
  architecture and must be ported toward the C++/Python stream-parser,
  event-coalescing, diff-buffer model, not hidden with app-specific sleeps,
  release-only documentation, or example changes. Affected paths:
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` direct terminal parser,
  `runTerminal`, terminal frame buffer, and image render path; C++ references:
  `cpp/src/terminal/Input.cpp`, `cpp/src/terminal/TerminalBackend.cpp`,
  `cpp/src/generated/GeneratedWindowRuntime.cpp` `coalesceMouseWheelEvents`,
  `TerminalBuffer::renderDiff`/`writeFrame`; Python reference:
  `src/uimd/runtime/rendering.py` `TerminalBuffer.render_diff` and
  `src/uimd/runtime/application.py` input readiness. Required validation:
  measure PTY latency for normal key and repeated wheel/event bursts
  before/after, verify Swift direct PTY remains responsive, keep focused
  C++/Swift MCP compare passing, run `swift test`, build Swift `activity_feed`
  and `image_browser`, and rerun broad rebuild/test gate for the runtime
  change. Done: Swift direct terminal now uses a C++-style byte-stream
  `TerminalInputParser`, event batching, consecutive wheel coalescing, dirty
  frame rendering, and `TerminalFrameBuffer.renderDiff()` instead of full-frame
  writes after every event. Direct terminal dispatch mutates runtime state
  directly while MCP wrappers remain responsible for MCP response payloads. The
  image path now caches decoded rasters/rendered cells/Sixel payloads, and the
  hot `Color.blended(over:)` path fast-returns for opaque `#rgb`, `#rrggbb`, and
  `#rrggbbff` colors instead of reparsing RGBA for every cell blend. PTY timing
  after the color fast path no longer reproduced multi-second frames:
  `activity_feed` Swift debug measured about 55 ms average per visible Down-key
  render versus Python about 48 ms and C++ about 31 ms in the same harness;
  Swift release measured about 40 ms. `image_browser` Swift debug measured
  about 69 ms average including the first image render and about 45-46 ms for
  subsequent keyed renders; Swift release measured about 41 ms, C++ about 33 ms,
  and Python about 63 ms including first render. Validation passed:
  `swift test --package-path swift/src/Uimd`, Swift debug builds for
  `activity_feed` and `image_browser`, focused C++/Swift
  `tests/mcp/activity_feed.yaml` compare with `47 asserts passed, 0 failed, 0
  step failures`, focused C++/Swift `tests/mcp/image_browser_compare.yaml`
  compare with `262 asserts passed, 0 failed, 0 step failures`, and broad
  `python3 tools/uimd_dev.py rebuild-all --test` regeneration/build gate with
  C++ `ctest` `26/26` passed.

- [x] **Swift image_browser reusable scroll-scope focus background parity
  mismatch**. Re-running
  `./uimd mcp-test --backend python --all --compare cpp/build/examples
  swift/examples --mcp-fast --compare-app-size 90x35` reproduced the earlier
  C++/Swift all-example failure in `tests/mcp/image_browser_compare.yaml`:
  `216 asserts passed, 0 failed, 1 step failures`. Snapshot
  `tests/mcp/snapshots/20260703-132415-766953-step-115-image_browser_compare.json`
  reports step 115 row 5 col 33 expected C++ background `#555b69`, but Swift
  rendered `#172033` for the same `[` cell after keyboard navigation inside the
  reusable image item scroll scope. Parity decision: this is Swift reusable
  child focus/active-scroll rendering behavior and must be audited against the
  C++ generated runtime, not masked in the test or fixed in `image_browser`
  example logic. Affected paths: Swift
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` `GeneratedWindowBase.renderContent`,
  `applyReusableHostFocusBackground`, and active scroll/reusable scope handling;
  C++ reference `cpp/src/generated/GeneratedWindowRuntime.cpp`
  `renderGeneratedWindowContent`, `dimOutsideActiveScrollView`, and
  `applyReusableFocusBackground`. Required validation: focused C++/Swift
  `tests/mcp/image_browser_compare.yaml` compare with `--compare-app-size
  90x35`, focused `tests/mcp/activity_feed.yaml` compare to preserve the direct
  terminal fixes, `swift test --package-path swift/src/Uimd`, and the broad
  rebuild/test gate for any Swift runtime change. Done: the earlier row 5/col
  33 focus-background mismatch was not reproduced after rebuilding the current
  Swift runtime and replaying the focused script. Standalone replay confirmed
  the expected focused reusable child state, and focused C++/Swift
  `tests/mcp/image_browser_compare.yaml` compare passed with `262 asserts
  passed, 0 failed, 0 step failures`. The only related render change kept for
  `image_browser_compare` was the C++ parity fix that re-applies ScrollView
  `^`/`v` indicators after child blits in
  `GeneratedWindowBase.renderContent`; no example-specific workaround or test
  masking was added. Additional validation passed: focused
  `tests/mcp/activity_feed.yaml` compare with `47 asserts passed, 0 failed, 0
  step failures`, `swift test --package-path swift/src/Uimd`, Swift
  `activity_feed`/`image_browser` builds, and broad `python3 tools/uimd_dev.py
  rebuild-all --test` regeneration/build gate with C++ `ctest` `26/26` passed.

- [x] **Swift image rendering lacks the C++/Python raster cache**. Direct
  terminal `image_browser` performance investigation found that Swift
  `Image.render()` calls `loadImageRaster(...)` on every render, while C++
  `cpp/src/elements/Image.cpp` caches decoded rasters and Python
  `src/uimd/runtime/image.py` uses `@lru_cache` for image loading/rendering
  helpers. Parity decision: this is Swift runtime image backend architecture,
  not an image_browser example issue; the Swift runtime must cache decoded
  image rasters and failed lookups like the C++/Python backends, without
  changing `.uimd` image semantics or example logic. Affected path:
  `swift/src/Uimd/Sources/Uimd/Uimd.swift` image loading/rendering. Required
  validation: direct PTY timing for `image_browser`, `swift test
  --package-path swift/src/Uimd`, Swift `image_browser` build, focused
  C++/Swift `tests/mcp/image_browser_compare.yaml` compare with
  `--compare-app-size 90x35`, and activity_feed compare to preserve non-image
  terminal behavior. Done: Swift `Image.render()` now uses shared runtime
  caches for decoded rasters, rendered terminal cells, failed raster lookups,
  and Sixel payloads, plus a runtime-loaded `libsixel` fast path in
  `CUimdImageDecoder` that searches `UIMD_LIBSIXEL_DIR`, package manager
  locations, and common Homebrew paths such as `/opt/homebrew/opt/libsixel/lib`.
  The cache is in the Swift runtime, not in `image_browser` example logic.
  Validation passed: direct PTY `image_browser` timing improved to roughly
  45-46 ms for repeated debug keyed renders after first render and roughly 41
  ms average in release, `swift test --package-path swift/src/Uimd`, Swift
  `image_browser` build, focused C++/Swift `tests/mcp/image_browser_compare.yaml`
  compare with `262 asserts passed, 0 failed, 0 step failures`, focused
  `activity_feed` compare with `47 asserts passed, 0 failed, 0 step failures`,
  and broad `python3 tools/uimd_dev.py rebuild-all --test` regeneration/build
  gate with C++ `ctest` `26/26` passed.

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
