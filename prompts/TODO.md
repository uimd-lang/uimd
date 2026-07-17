# TODO

## Current Open Tasks

Date: 2026-06-21

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
  behavior exceptions are recorded; Go-specific code is limited to language,
  terminal, build, and OS primitive adapters around the same public behavior.
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
