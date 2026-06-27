# TODO

## Current Open Tasks

Date: 2026-06-21

- [x] **C# direct input/listbox/scroll restoration regressions after latest
  runtime parity fixes**. User reported on 2026-06-27 while manually testing C#
  examples that several direct-runtime behaviors regressed: clicking into a
  spinbox/NumberInput whose value is `0` and typing should replace the `0` with
  the first typed digit just like keyboard Enter edit does, but mouse-started
  editing keeps the `0`; `expense_tracker` does not scroll to the newly added
  expense location after adding an expense and must be checked against C++
  parity; the multi-select ListBox in `formular` cannot toggle items with
  arrow keys plus Enter, only mouse; FileBrowser/ListBox selection in long lists
  should automatically keep the selected file visible by native ListBox
  scrolling, not by example-specific code; and `task_board` should not scroll a
  ScrollView just because a dialog was opened and closed. Treat this as shared
  C# runtime parity, not as example workarounds. Parity paths to audit: C#
  `csharp/src/Uimd/Runtime/Elements.cs` NumberInput/ListBox/ScrollView state,
  C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs` direct mouse/edit/key and
  modal focus/scroll restoration, C# `csharp/src/Uimd/Runtime/Dialogs.cs`
  FileBrowser only where it mirrors generated dialog behavior, C++
  `cpp/src/elements/BasicElements.cpp`,
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, and Python reference paths in
  `src/uimd/runtime`. Required validation: reproduce or encode focused C#
  direct/MCP checks for the five user-visible paths, keep changes surgical and
  structurally aligned with C++/Python, rebuild affected C# examples, run
  focused C++/C# compares with `--compare-app-size 90x35`, and run the broader
  rebuild/parity gate appropriate for touched shared runtime code.
  Fixed on 2026-06-27 by preserving C# and C++ NumberInput first-replace state
  for mouse-started edits on zero-valued inputs, aligning ListBox active-row
  navigation and multi-select Enter toggling across Python/C++/C#, making
  ListBox rendering keep the active/selected item visible in long lists, and
  comparing ListBox selected values rather than only selected index for
  generated change dispatch/MCP tools. Python fallback active-scroll dimming was
  also aligned with C++/C# by blending both foreground and background colors for
  half-block image cells instead of leaving black bands. The work stayed in
  shared runtime/generated-window code plus focused `formular` parity coverage,
  with no example-specific workaround. Validation passed: focused C# smoke
  scripts for NumberInput mouse zero replacement, `formular` multi-select
  keyboard toggling, FileBrowser long-list visibility, `expense_tracker` add
  scroll visibility, and `task_board` dialog scroll preservation; focused
  C++/C# compares for `formular`, `expense_tracker`, `task_board`, and
  `text_editor`; full `./uimd mcp-test --headless --all --compare
  cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35` with
  626 asserts; `PATH=/Users/marekdubovsky/.dotnet:$PATH
  ./tools/rebuild_all.sh`; focused Python/C++ `image_browser` compare; and full
  `./uimd mcp-test --headless --all --compare python/examples
  cpp/build/examples --mcp-fast --compare-app-size 90x35` with 626 asserts.
- [x] **formular MCP compare reports one step failure after C# runtime parity
  commit**. User reported on 2026-06-27:
  `03:55:19 SUMMARY tests/mcp/formular.yaml [formular]: FAIL 8 asserts passed,
  0 failed, 1 step failures`. Treat this as a parity-sensitive MCP/runtime or
  generated C# regression, not as a test workaround. Affected paths to audit:
  `tests/mcp/formular.yaml`, Python reference runtime under `src/uimd/runtime`,
  C++ runtime/generated-window dispatch under `cpp/src/generated`, and C#
  runtime/generated-window dispatch under `csharp/src/Uimd/Runtime`. Required
  validation: reproduce the exact failing step with `--compare-app-size 90x35`,
  fix the shared runtime/generator behavior while preserving Python/C++/C#
  parity, rebuild/regenerate affected outputs, rerun focused `formular` compare,
  and run the necessary broader parity gate for the touched shared behavior.
  Fixed on 2026-06-27 in C# MCP coordinate dispatch: root-window MCP
  `mouse_click` now subtracts only the compare viewport origin before hit
  testing because C# root rendering stores element frames in viewport-relative
  coordinates, while modal/window-stack clicks still use `WindowStackContentPoint`
  so dialog hit testing remains content-relative. Validation passed:
  `/Users/marekdubovsky/.dotnet/dotnet build csharp/src/Uimd/Uimd.csproj
  --configuration Debug`, `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/formular/formular.csproj --configuration Debug`,
  `./uimd mcp-test --headless --compare cpp/build/examples csharp/examples
  tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35` with 76 asserts,
  `PATH=/Users/marekdubovsky/.dotnet:$PATH ./tools/rebuild_all.sh`, and
  `./uimd mcp-test --headless --all --compare cpp/build/examples csharp/examples
  --mcp-fast --compare-app-size 90x35` with 626 asserts.
- [x] **C# direct modal/dialog mouse clicks miss after recent image preview
  fixes**. User reported on 2026-06-27 that direct C# mouse clicks no longer
  work on controls inside dialog windows, message boxes, and browse dialogs;
  the likely regression is in the modal/window-stack mouse coordinate mapping
  touched while fixing image_browser preview `Close` clicks. Treat this as
  shared C# runtime parity, not as an image_browser workaround. Parity paths:
  C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs` direct terminal mouse parsing,
  window-stack coordinate translation, modal hit testing, and
  `McpController` mouse dispatch; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`
  direct terminal mouse/window-stack dispatch; Python `src/uimd/runtime`
  application/window-stack and dialog input reference. Required validation:
  reproduce a direct C# modal/button miss in `image_browser` and at least one
  MessageBox or FileBrowser/Browse path, fix the shared coordinate/state
  handling structurally aligned with C++/Python, rebuild affected C# examples,
  run focused C++/C# compares with `--compare-app-size 90x35`, and rebuild all
  examples after the runtime fix. Fixed on 2026-06-27 in the shared C# runtime
  by aligning modal mouse coordinate handling with C++: `WindowStackContentPoint`
  now subtracts both the modal frame origin and the generated-window content
  inset, and C# refreshes the active modal layout for mouse hit testing using
  the actual overlay frame content size instead of the full viewport size.
  Validation: `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/src/Uimd/Uimd.csproj --configuration Debug`,
  `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/image_browser/image_browser.csproj --configuration Debug`,
  TCP MCP `mouse_click` smokes against real C# apps confirmed terminal-coordinate
  clicks close the image preview `Image Show Dialog`, the task_board
  `MessageBoxYesNo`, and the image_browser `FileBrowser` Browse dialog; focused
  compares passed for
  `./uimd mcp-test --headless --compare cpp/build/examples csharp/examples tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35`
  with 46 asserts and
  `./uimd mcp-test --headless --compare cpp/build/examples csharp/examples tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35`
  with 144 asserts. Full rebuild gate
  `PATH=/Users/marekdubovsky/.dotnet:$PATH ./tools/rebuild_all.sh` completed
  successfully after regenerating supported Python/C++/C# sources, rebuilding
  C++ examples/regressions, rebuilding all C# examples, and running Python
  `compileall`.
- [x] **C# direct Image Browser still wraps focused Image items ScrollView at
  bottom and fallback images dim with black bands**. User reported on
  2026-06-27 that after the latest C# direct fixes, pressing ArrowDown at the
  bottom of the focused `Image items` ScrollView still jumps/scrolls back to
  the beginning in manual direct terminal use, even though compare coverage
  passed. User also reports that when render mode is `Fallback` and `Image
  items` is selected, image content should dim by darkening its colors, but the
  fallback half-block images show black horizontal bands instead. Treat this as
  shared C# runtime/rendering parity, not as an image_browser workaround. Parity
  paths: C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs` focus movement and
  active ScrollView rendering/dimming; C# `csharp/src/Uimd/Runtime/Elements.cs`
  fallback image cell metadata; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`
  focus movement and `dimOutsideActiveScrollView`; Python
  `src/uimd/runtime` ScrollView/image rendering references. Required validation:
  reproduce the direct bottom ArrowDown behavior or add a direct/PTY smoke,
  verify focused fallback image dimming darkens both foreground and background
  samples without black bands, rebuild C# runtime and image_browser/task_board,
  run focused C++/C# compares with `--compare-app-size 90x35`, and record any
  direct validation caveats. Fixed on 2026-06-27 by matching C# active
  ScrollView focusable collection to the C++ reusable-child frame sync path and
  by dimming fallback half-block image foreground/background samples instead of
  replacing half-block backgrounds with a fixed black fill. Validation:
  `/Users/marekdubovsky/.dotnet/dotnet build csharp/examples/image_browser/image_browser.csproj --configuration Debug`,
  `/Users/marekdubovsky/.dotnet/dotnet build csharp/examples/task_board/task_board.csproj --configuration Debug`,
  `cmake --build cpp/build --target image_browser task_board`,
  `./uimd mcp-test --backend python --headless csharp/examples/image_browser/bin/Debug/net10.0/image_browser.dll /private/tmp/uimd_image_items_down.yaml --mcp-fast --compare-app-size 90x35`
  confirmed focus stays on `main.items[19].sel` after repeated ArrowDown at the
  bottom, and focused compares passed:
  `./uimd mcp-test --headless --compare cpp/build/examples csharp/examples tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35`
  with 46 asserts and
  `./uimd mcp-test --headless --compare cpp/build/examples csharp/examples tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35`
  with 144 asserts. The full rebuild gate
  `PATH=/Users/marekdubovsky/.dotnet:$PATH ./tools/rebuild_all.sh` also
  completed successfully after regenerating supported Python/C++/C# sources,
  rebuilding C++ examples, rebuilding all C# examples, and running Python
  `compileall`.
- [x] **C# direct Image Browser still has focused ScrollView artifacts, bottom
  traversal reset, modal Close mouse miss, and slow Gallery wheel scrolling**.
  User confirmed on 2026-06-26 that the title suffix and TextArea mouse-wheel
  fixes are resolved, but the remaining direct C# `image_browser` issues are
  still reproducible after generating and building from this checkout: when the
  `Image items` ScrollView is focused, black background rectangles appear to the
  right of row `Delete` buttons; ArrowDown at the bottom of that ScrollView
  resets/scrolls back to the beginning instead of staying at the bottom, with
  the same bug visible in `task_board`; mouse click on the preview modal
  `Close` button still does not activate; and wheel scrolling in the `Gallery`
  ScrollView is too slow. Treat these as direct C# runtime parity bugs, not as
  image_browser/task_board workarounds. Parity paths to audit before edits:
  C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`, and `csharp/src/Uimd/Runtime/Core.cs`;
  C++ `cpp/src/generated/GeneratedWindowRuntime.cpp` and generated element/
  ScrollView runtime paths; Python `src/uimd/runtime` ScrollView/window/modal
  references. Required validation: use the direct generated C# example path,
  reproduce or instrument the four remaining direct failures, keep the behavior
  structurally aligned with C++/Python, rebuild the affected C# examples, run
  focused C++/C# compares with `--compare-app-size 90x35`, and record any
  direct PTY validation that could not be run.
  Completed 2026-06-26 as shared C# runtime parity work, without app-specific
  image_browser/task_board logic. C# active ScrollView focus traversal now walks
  visible `ScrollViewChildView` frames like C++ instead of stale child frames,
  fixing ArrowDown traversal at the bottom of focused ScrollViews in
  image_browser/task_board. The focused `Image items` black rectangles were
  traced to a C#-only external-proxy background restore that repainted the
  active ScrollView gap with the terminal default background; that restore was
  removed and image half-block dimming now preserves sampled cell backgrounds in
  parity with C++. C# modal stack rendering no longer mutates modal child frames
  into terminal coordinates, combobox overlay receives the window offset
  explicitly, and direct mouse hit testing maps terminal points through the
  frame-relative stack coordinate space used by the C# modal runtime; a direct
  PTY smoke clicked the visible `Close` label and closed the preview dialog.
  Gallery wheel performance was improved in the C# image fallback path by
  matching C++'s algorithm: resize the source raster once to the target
  terminal-cell raster and then copy sampled pixels, instead of recomputing
  source sampling for every rendered cell. The existing terminal scroll-region
  fast path remains full-width only in both C++ and C#, so nested Gallery
  ScrollViews still use normal diff repaint, but the expensive C# fallback image
  composition path is now substantially cheaper and structurally aligned with
  C++. Validation passed: `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/src/Uimd/Uimd.csproj --configuration Debug --no-incremental`,
  `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/image_browser/image_browser.csproj --configuration Debug
  --no-incremental`, `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/task_board/task_board.csproj --configuration Debug
  --no-incremental`, `./uimd mcp-test --headless --compare cpp/build/examples
  csharp/examples tests/mcp/image_browser_compare.yaml --mcp-fast
  --compare-app-size 90x35` with 46 asserts passed, and the same compare command
  for `tests/mcp/task_board_compare.yaml` with 144 asserts passed. Focused
  direct PTY smokes also verified no black cells to the right of `Delete` in
  the focused image list and verified visible modal `Close` mouse activation.
- [x] **C# direct Image Browser and Task Board still have focused ScrollView,
  modal mouse, wheel-scroll, TextArea scroll, and terminal-title parity
  regressions**. User manually reproduced remaining direct C# runtime issues on
  2026-06-26 with screenshots: when the `Image items` generated ScrollView in
  `image_browser` is focused, black background artifacts appear to the right of
  row `Delete` buttons; when that ScrollView is not focused, the artifacts are
  absent. Pressing ArrowDown to the lowest focusable row inside that ScrollView
  resets/scrolls back to the beginning instead of stopping at the bottom; the
  same traversal reset appears in `task_board`. The `Close` button in the image
  preview modal still does not activate by mouse. Wheel scrolling in the
  `Gallery` image ScrollView is visibly slow. The direct terminal/window title
  shows `dotnet` instead of the app name plus target suffix such as
  `Image Browser [C#]`, `... [python]`, or `... [c++]`. C# TextArea controls
  with content taller than the visible area do not scroll with the mouse wheel,
  while C++ does. Treat this as shared runtime/generator/direct-terminal parity,
  not as an `image_browser` or `task_board` workaround. Parity rule: focused
  ScrollView/reusable background blending, active ScrollView edge traversal,
  modal mouse hit testing, wheel dispatch/coalescing, TextArea internal
  scrolling, terminal title escape handling, and image ScrollView performance
  must stay structurally 1:1 with the C++ direct runtime and Python reference
  except for unavoidable terminal/.NET adapters. Affected paths to audit before
  edits: C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`, `csharp/src/Uimd/Runtime/Core.cs`, and
  generated/example C# hooks only where they mirror C++/Python; C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, element/ScrollView/TextArea
  runtime, terminal title/input paths, and corresponding examples; Python
  `src/uimd/runtime` application/window/ScrollView/TextArea reference paths.
  Required validation: reproduce or encode focused direct/MCP checks for the
  focused image-list row artifact, ArrowDown bottom-edge behavior in
  `image_browser` and `task_board`, image-preview modal `Close` mouse click,
  gallery wheel responsiveness, direct terminal title, and C# TextArea wheel
  scrolling; regenerate/build affected outputs after runtime/generator changes;
  run focused C++/C# compares with `--compare-app-size 90x35` and direct PTY
  smokes for the user-visible paths.
  - Completed 2026-06-26: C# direct mouse input now translates terminal
    coordinates through the top window-stack frame before dispatching to modal
    content, matching the C++ `stackFrameContentPoint` behavior. C# wheel
    dispatch now handles ScrollView, parent ScrollView, ListBox, and TextInput/
    TextArea paths in the shared runtime instead of only outer ScrollViews.
    C# ScrollView scope navigation now computes scope from the active
    scrollview/proxy to the next navigation element like C++, preventing the
    bottom-edge focus reset seen in image_browser/task_board. Python, C++, and
    C# direct launchers now emit sanitized terminal titles with `[python]`,
    `[c++]`, and `[C#]` suffixes. C# ScrollView/direct terminal rendering now
    carries a C++-style pending terminal scroll delta and render scroll-region
    hint where the scrollview spans the terminal width; non-full-width image
    scrollviews still fall back to normal diff repaint like C++. Validation:
    `/Users/marekdubovsky/.dotnet/dotnet build csharp/src/Uimd/Uimd.csproj
    --configuration Debug`, C# image_browser/task_board example builds,
    `cmake --build cpp/build --target image_browser task_board`,
    `python3 -m compileall src/uimd/runtime/application.py`, `./uimd mcp-test
    --headless --compare cpp/build/examples csharp/examples
    tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35`
    (46 asserts), and the same command for `tests/mcp/task_board_compare.yaml`
    (144 asserts) passed. Direct PTY real-mouse smoke was not rerun because the
    previously used `/private/tmp/uimd_real_mouse_close.py` helper was not
    present in this workspace.
- [x] **C# direct Image Browser still has ScrollView clipping, dialog Sixel,
  modal focus, row artifact, and delete-focus parity regressions**. User
  manually reproduced remaining direct C# parity gaps after the previous
  image_browser fix: when scrolling the main image/gallery ScrollView, partially
  clipped images stick to the top of their element and are clipped at the bottom
  instead of being clipped from the top like C++; the `Show` preview dialog still
  renders fallback/pixelated image output instead of the C++ Sixel path and its
  `Close` button does not react to mouse clicks; closing the `Browse` dialog in
  image_browser loses the originating ScrollView/reusable focus even though
  task_board now preserves it; the image item rows show unwanted black
  rectangular artifacts around reusable row controls; deleting an item in
  image_browser or task_board resets focus and scroll position to the beginning
  instead of moving focus to the next remaining item as C++ does. Treat this as
  shared C# runtime/generator parity, not as an image_browser-specific
  workaround. Parity rule: C# clipping, Sixel/fallback choice, modal focus
  restore, mouse hit testing in dialogs, reusable row background blending,
  post-delete focus repair, and ScrollView scroll-position preservation must stay
  structurally 1:1 with C++ `GeneratedWindowRuntime` and Python reference except
  for minimal .NET terminal adapters. Affected audit paths: C#
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`, `csharp/src/Uimd/Runtime/Core.cs`,
  C# `csharp/examples/image_browser/image_browser.cs` only where generated hook
  structure mirrors C++; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  `cpp/include/ui/generated/GeneratedWindowRuntime.hpp`, image/terminal runtime,
  and C++ `image_browser`/`task_board` example hooks; Python
  `src/uimd/runtime` ScrollView/image/modal/focus reference paths. Required
  validation: reproduce the direct failures, compare C# against C++ direct/MCP
  for the same UI source, fix shared runtime/generator behavior without
  example-only branches, regenerate/build all supported examples after runtime
  changes, then run focused direct PTY smokes for image_browser scroll clipping,
  preview dialog Sixel and close mouse click, Browse close focus restoration,
  row artifact absence, and delete focus handoff in image_browser/task_board,
  including the boundary case where deleting the last item focuses the previous
  remaining item if one exists. If this boundary behavior is missing in C++ or
  Python, fix it there too so Python/C++/C# stay 1:1. Also run focused and full
  C++/C# compares with `--compare-app-size 90x35`.
  Implemented on 2026-06-25 as a C# runtime parity correction kept structurally
  aligned with the C++ `GeneratedWindowRuntime` paths and the Python reference
  behavior where shared runtime behavior exists. C# now treats only the explicit
  `transparent` color sentinel as transparent, while `#00000000` round-trips as
  an RGB color like C++; C# `Color.ToString()` now mirrors the C++ string form by
  ignoring alpha except for the transparent sentinel. C# generated-window
  ScrollView rendering now follows the C++ focus-background gap and root-overlay
  flow, including excluded reusable child rects, exact alpha blending at focus
  gaps, modified-cell writeback, and root generated ScrollView indicator
  suppression. Delete focus repair was audited across Python, C++, and C#: when
  a removed reusable child has a next remaining item, focus moves to that item;
  when the removed child was the last one, focus moves to the previous remaining
  item if one exists; no extra one-platform patch was needed for that boundary
  because the current shared repair path already computes `min(old_index,
  last_index)`. Validation passed: `./tools/rebuild_all.sh`;
  `python3 -m pytest python/tests` with 460 passed; `ctest --test-dir
  cpp/build --output-on-failure` with 26/26 tests passed; focused C++/C#
  compares for `tests/mcp/image_browser_compare.yaml`,
  `tests/mcp/image_gallery_compare.yaml`, and `tests/mcp/markdown_viewer.yaml`;
  full Python/C++ compare with 626 asserts passed; full C++/C# headless compare
  with 626 asserts passed; and UIMD regression parity compare with 14 asserts
  passed.
- [x] **C# direct Image Browser still diverges from C++ in left-panel
  navigation, row actions, and dialog image rendering**. User manually
  reproduced that direct C# `image_browser` improved enough for some mouse
  clicks, but it is still not 1:1 with C++: arrow navigation cannot move through
  left-panel image items, some mouse clicks still miss, `Browse` and `Delete`
  buttons inside item rows do not activate, `Show/Open` activates but the dialog
  displays the fallback-style/pixelated image state instead of matching C++.
  Treat this as a C# example/runtime parity defect, not as a screenshot-specific
  workaround. Parity rule: C# `image_browser` application logic, generated
  reusable component wiring, ScrollView/reusable hit testing, keyboard spatial
  focus traversal, nested row button activation, dialog image render mode,
  Sixel/fallback selection, and modal state must stay structurally 1:1 with the
  C++ implementation except for unavoidable .NET terminal adapters. Affected
  audit paths: C# `csharp/examples/image_browser/image_browser.cs`, generated
  C# `*_ui.cs` files under `csharp/examples/image_browser`, C# runtime
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`, `Elements.cs`, `Core.cs`; C++
  `cpp/examples/image_browser/image_browser.cpp`, generated C++ image_browser
  files, and `cpp/src/generated/GeneratedWindowRuntime.cpp`. Required
  validation: reproduce the direct C# failures, compare against direct/MCP C++
  behavior for the same UI source, remove avoidable C# structural deviations in
  example hooks and runtime, regenerate/build affected outputs and all examples
  after runtime fixes, then run focused C++/C# image_browser compare with
  `--compare-app-size 90x35` plus direct PTY smoke covering keyboard navigation,
  row `Browse`/`Delete`, image dialog `Show/Open`, and mouse activation.
  Implemented on 2026-06-25 by aligning C# generated reusable-window rendering
  with the C++ `GeneratedWindowRuntime` structure. C# now propagates reusable
  descendant focus style, window style overrides, scrollview child sizing, and
  descendant background blending through nested generated windows the same way as
  C++, which restores left-panel keyboard traversal and row-button visual/input
  parity. C# `ListBox` and `FileBrowser` were also aligned with C++ disabled-row
  handling so filtered files render and behave consistently in `Browse`.
  Validation passed: `./tools/rebuild_all.sh`; `python3 -m pytest python/tests`
  with 460 passed; `ctest --test-dir cpp/build --output-on-failure` with 26/26
  passed; focused C++/C# `tests/mcp/image_browser_compare.yaml` headless and
  non-headless with 46 asserts passed; focused C++/C# direct smoke covering
  arrows through left-panel image buttons, item `Browse`, item `Delete`, and
  MessageBox `No` with 24 asserts passed; focused C++/C#
  `tests/mcp/image_gallery_compare.yaml` with 8 asserts passed; full C++/C#
  headless compare with 626 asserts passed; full Python/C++ compare with 626
  asserts passed; and UIMD regression parity compare with 14 asserts passed.
- [x] **C# direct Image Browser still stalls with high CPU and nonresponsive
  input despite MCP parity passing**. User manually reproduced direct
  `image_browser` after the previous fix: the UI only partially improved, but
  `dotnet` consumes about one full CPU core, mouse clicks and arrow keys do not
  respond, and the rendered Sixel images look low-depth/dithered compared with
  the expected C++ direct behavior. Treat this as a direct terminal runtime
  performance/input starvation/rendering parity bug, not as an example-specific
  workaround. Parity rule: C# direct render cadence, Sixel emission/caching,
  input polling/dispatch, mouse/key responsiveness, image quantization, and
  terminal cell pixel sizing must stay structurally 1:1 with the C++ direct
  runtime and Python reference except for minimal .NET terminal adapters.
  Affected audit paths: C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`, `csharp/src/Uimd/Runtime/Core.cs`,
  C# generated/example image hooks only where they mirror C++; C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp`, C++ image/terminal runtime,
  and Python `src/uimd/runtime/image.py` and application loop reference.
  Required validation: reproduce the direct C# high-CPU/nonresponsive
  `image_browser` path, compare the loop and image emission behavior against
  C++, fix shared runtime/Sixel/input scheduling only, then validate direct
  image_browser keyboard/mouse responsiveness and CPU behavior plus focused
  C++/C# image_browser/image_gallery compares with `--compare-app-size 90x35`.
  Implemented on 2026-06-25 by aligning the C# image runtime with the C++
  runtime's cached raster and cached Sixel payload flow. C# now caches decoded
  rasters, caches Sixel payloads by the same source/geometry/fit/background key
  shape, dynamically uses libsixel for high-quality 256-color encoding when
  available, and keeps the existing fallback encoder cached instead of
  rebuilding image payloads on every frame. Validation passed:
  `./tools/rebuild_all.sh`; focused C++/C# `tests/mcp/image_browser_compare.yaml`
  with 46 asserts passed; focused C++/C# `tests/mcp/image_gallery_compare.yaml`
  with 8 asserts passed; direct PTY C# `image_browser` stayed around 0.7% CPU
  after initial render and keyboard input, around 0.6% CPU after an SGR mouse
  click, the click changed the caption to `Astronaut`, and Ctrl+C terminated the
  process cleanly with no remaining `image_browser` process.
- [x] **C# direct runtime follow-up parity failures in copy feedback, ListBox,
  image rendering/activation, and modal focus restoration**. User manually
  reproduced remaining C# direct-runtime divergences after the previous full
  gate: copying selected text from TextInput and possibly TextArea/NumberInput
  copies but does not show the same transient "Copied" feedback as
  Python/C++; `expense_tracker` category ListBox cannot select newly added
  categories until switching tabs; `formular` ListBox cannot select an item with
  keyboard arrows plus Enter while mouse selection works; C# `image_browser`
  renders images at different sizes/layout than C++ and its image buttons do not
  react to mouse clicks or keyboard navigation; C# `image_gallery` image sizing
  differs from C++; and C# `task_board` loses the focused ScrollView/reusable
  `Open` button state after closing the Edit Task dialog while C++ preserves it.
  Treat these as parity-sensitive runtime/generator/example-hook defects, not as
  example-specific workarounds. Parity rule: C# implementation must stay
  structurally 1:1 with the C++ implementation and Python reference in element
  state machines, selection/copy feedback, ListBox edit/selection lifecycle,
  image sizing/rendering, reusable image-button activation, and modal focus
  save/restore, except for minimal language/terminal adapters. Affected audit
  paths: Python `src/uimd/runtime` element/window/image/dialog paths and example
  hooks; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`, C++ element/image
  runtime and example hooks; C# `csharp/src/Uimd/Runtime/Core.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`,
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`, and generated/example C# hooks
  only where they mirror C++/Python hooks. Required validation: reproduce or
  encode focused direct/MCP checks for copy feedback, ListBox keyboard
  selection, newly added expense categories, image_browser/image_gallery
  rendering and activation, and task_board dialog focus restoration; regenerate
  and build all supported examples after runtime/generator changes; rerun
  focused C++/C# compares with `--compare-app-size 90x35`; then run the full
  local gate. Implemented on 2026-06-25 as a C# parity correction kept aligned
  with the C++ runtime shape and Python reference behavior. C# direct copy now
  surfaces the transient copied feedback like the other runtimes; ListBox
  selection state was aligned around index-based selection so keyboard
  arrows/Enter and newly added categories behave consistently; image sizing and
  Sixel cell geometry now follow the C++ terminal pixel sizing path instead of
  a C#-specific fixed-cell approximation; reusable image buttons route through
  the same activation/hit-testing path as generated controls; and modal button
  activation now preserves/restores the background ScrollView/reusable focus
  context when the dialog closes. Focused C++/C# validation passed for
  `tests/mcp/expense_tracker_compare.yaml`, `tests/mcp/formular.yaml`,
  `tests/mcp/image_browser_compare.yaml`, `tests/mcp/image_gallery_compare.yaml`,
  and `tests/mcp/task_board_compare.yaml` with `--compare-app-size 90x35`.
  Full validation passed: `./tools/rebuild_all.sh`; `python3 -m pytest
  python/tests` with 460 passed; `ctest --test-dir cpp/build
  --output-on-failure` with 26/26 tests passed; `./uimd mcp-test --all
  --compare python/examples cpp/build/examples --mcp-fast --compare-app-size
  90x35` with 626 asserts passed; `./uimd mcp-test --headless --all --compare
  cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35` with
  626 asserts passed; and `./uimd mcp-test --compare
  tests/regressions/uimd/parity/python cpp/build/regressions/uimd/parity
  tests/regressions/uimd/parity/all.yaml --mcp-fast --compare-app-size 90x35`
  with 14 asserts passed.
- [x] **Direct runtime interaction parity gaps after C# port validation**. User
  manually reproduced behavior that current compare tests do not catch:
  ScrollView keyboard traversal in `expense_tracker` wraps/restarts at the top
  when reaching the end instead of stopping or preserving expected edge state;
  spatial Right from an expense-row checkbox does not move to the row `Delete`
  button; `formular` may not print the same post-exit output as Python/C++;
  direct `image_browser` and `image_gallery` Sixel rendering corrupt the screen
  and leave raw terminal/mouse escape fragments after abort/exit; direct example
  shutdown can leave stray terminal bytes in the shell; Markdown Viewer scrolling
  feels delayed or continues after input; text selection/copy behavior in
  TextInput/TextArea/NumberInput must be audited against Python/C++; returning
  from `task_board` Edit Task dialog loses the originating ScrollView edit/focus
  state; Escape on MessageBox/FileBrowser closes without the same temporary
  No/Cancel button highlight; FileBrowser/ListBox dialog keyboard lifecycle
  diverges (cannot exit listbox cleanly, Enter/Escape can close the dialog at the
  wrong level). This must be fixed as shared runtime behavior with Python, C++,
  and C# kept 1:1 in algorithm, state, cleanup order, focus restoration, input
  dispatch, and terminal teardown except for unavoidable terminal/language
  adapters. Affected audit paths: Python `src/uimd/runtime` window/input/dialog/
  scroll/image paths; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  `cpp/include/ui/generated/GeneratedWindowRuntime.hpp`, terminal input/image and
  element paths; C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`, `csharp/src/Uimd/Runtime/Core.cs`, and
  generated/example C# hooks only when they mirror C++/Python example hooks.
  Required validation: reproduce the user-visible failures in direct PTYs where
  possible, compare behavior against Python/C++ before deciding the intended
  shared behavior, fix all three platforms when the shared behavior is wrong,
  regenerate/build all supported examples, and rerun Python tests, C++ ctest,
  Python/C++ compare, C++/C# compare, and UIMD regression parity with
  `--compare-app-size 90x35`. Implemented on 2026-06-25 as a shared
  Python/C++/C# parity correction, with C# fixes kept structurally aligned to
  the C++ runtime and Python reference where shared behavior exists. The runtime
  now preserves ScrollView focus/edit context across modal open/close and
  reusable-child activation, resolves nested ScrollView hit testing for row
  controls, keeps spatial focus activation scoped through the same active
  ScrollView path, avoids wrapping ScrollView keyboard traversal back to the
  start at the edge, restores direct terminal teardown/mouse state cleanly for
  image apps, and keeps direct Sixel output enabled without deterministic
  checker fallback unless explicitly requested. Dialog Escape/No/Cancel,
  FileBrowser/ListBox lifecycle, TextInput/TextArea/NumberInput editing and
  selection/copy paths, markdown scrolling, formular post-exit output, and
  task-board modal focus restoration were audited against the C++ and Python
  paths and corrected in shared runtime/example hooks only where the existing
  C++/Python behavior required the same change. Focused validation passed for
  `tests/mcp/expense_tracker_compare.yaml`, `tests/mcp/formular.yaml`,
  `tests/mcp/task_board_compare.yaml`, `tests/mcp/image_browser_compare.yaml`,
  `tests/mcp/image_gallery_compare.yaml`, and `tests/mcp/text_editor.yaml`
  comparing `cpp/build/examples` to `csharp/examples` with
  `--compare-app-size 90x35`. Direct PTY smokes passed for C# `image_browser`
  and `image_gallery` with real Sixel output and clean Quit/mouse teardown.
  Full validation passed: `./tools/rebuild_all.sh`; `python3 -m pytest
  python/tests` with 460 passed; `ctest --test-dir cpp/build
  --output-on-failure` with 26/26 tests passed; `./uimd mcp-test --all
  --compare python/examples cpp/build/examples --mcp-fast --compare-app-size
  90x35` with 626 asserts passed; `./uimd mcp-test --headless --all --compare
  cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35` with
  626 asserts passed; and `./uimd mcp-test --compare
  tests/regressions/uimd/parity/python cpp/build/regressions/uimd/parity
  tests/regressions/uimd/parity/all.yaml --mcp-fast --compare-app-size 90x35`
  with 14 asserts passed.
- [ ] **Design stronger compare tests for behavioral parity beyond static
  snapshots**. Follow-up only; do not implement during the runtime fix above.
  Current compare tests validate rendering and many MCP operations well, but user
  manual testing exposed gaps in direct-key interaction details: ScrollView edge
  traversal/wrap behavior, spatial navigation inside reusable row controls,
  focus/edit-state restoration after modal dialogs, transient Escape-highlight
  behavior for No/Cancel buttons, terminal teardown byte cleanliness, direct
  Sixel screen integrity, delayed/continued scrolling, and selection/copy
  behavior for TextInput/TextArea/NumberInput. Design future compare coverage
  that can drive real PTY key/mouse byte sequences and assert intermediate focus,
  edit-mode, scroll offsets, terminal cleanup output, transient dialog states,
  top/bottom clipping of partially visible images inside ScrollView, actual
  Sixel-vs-fallback rendering mode inside modal image previews, mouse hit testing
  on modal buttons, focus restoration after FileBrowser close, visual artifact
  checks for reusable row background blending, and post-delete focus handoff to
  the next remaining reusable child, including the boundary case where deleting
  the last child must focus the previous remaining child. Also include coverage
  for alpha/transparent color sentinel parity (`transparent` versus
  `#00000000`), root ScrollView indicator suppression, focus-background gap
  alpha blending, and modified-cell writeback across Python, C++, and C#
  without adding waits or test-side cleanup that masks runtime bugs.
- [x] **C# direct terminal parity still diverges in real examples after full
  compare passes**. User manually reproduced additional C# direct-runtime gaps
  after `./uimd generate csharp/examples --target csharp && dotnet build ... &&
  dotnet ...dll`: `text_editor` fails to start because it looks for
  `notes/welcome.txt` under `bin/Debug/net10.0`; `widget_gallery` animated
  labels update only when input refreshes the frame; direct ListBox mouse clicks
  enter edit mode instead of selecting the clicked item immediately; TextInput
  and likely TextArea do not support multi-character mouse/shift selection like
  Python/C++; MessageBox and FileBrowser dialogs do not close on `Escape` with
  the same No/Cancel result as Python/C++; `expense_tracker` ScrollView child
  checkbox/delete mouse clicks miss or route incorrectly; direct NumberInput
  editing can hide/reappear typed text after clicking outside; `image_browser`
  and `image_gallery` direct C# do not use Sixel rendering when available and
  image-browser left-panel reusable items cannot be clicked/navigated like C++.
  Treat all of these as shared C# runtime/generator parity defects, not
  per-example workarounds. Parity rule: C# direct runtime, generated startup
  path/resource resolution, tick/render loop, SGR mouse press/drag/release,
  hit-testing through ScrollView/reusable children, element edit/commit/cancel
  lifecycle, dialog Escape behavior, NumberInput/TextInput/TextArea/ListBox
  direct interaction, and image/Sixel rendering must be structurally 1:1 with
  C++ and Python except for unavoidable .NET terminal/library adapters. Affected
  sources to audit before edits: C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`, `csharp/src/Uimd/Runtime/Core.cs`, and
  generated C# example outputs; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  C++ element/image/terminal runtime, and Python `src/uimd/runtime` reference
  paths. Required validation: reproduce each direct failure in PTY or equivalent
  direct terminal smoke, compare against C++ for the same byte sequence and UI
  source, fix only shared runtime/generator/resource plumbing unless proven
  example-only and mirrored in C++/Python, regenerate/build all examples, rerun
  focused direct smokes for `text_editor`, `widget_gallery`, `formular` or
  selection controls, dialogs, `expense_tracker`, `image_browser`, and
  `image_gallery`, then rerun Python tests, C++ ctest, Python/C++ compare,
  C++/C# compare, and regression parity with `--compare-app-size 90x35`.
  Implemented on 2026-06-24 as a shared C# runtime/generator parity continuation
  rather than per-example workarounds. C# direct startup/resource resolution now
  matches the C++ examples closely enough that `text_editor` finds its `notes`
  files when launched from `bin/Debug/net10.0`; the direct render loop ticks and
  refreshes animated labels without requiring key input; ListBox mouse clicks,
  TextInput/TextArea cursor/selection/scroll state, dialog Escape No/Cancel
  behavior, NumberInput edit/commit state, ScrollView child hit testing and
  wheel dispatch, and reusable generated-control activation were audited against
  the C++ runtime and Python reference. Image/Sixel handling was aligned so
  direct `image_browser` and `image_gallery` use terminal image output when the
  terminal supports it, while deterministic fallback stays an explicit test
  mode. Image Browser reusable item activation now mirrors the C++ overrides
  (`ImageButton`, `GalleryMosaic`, and `ImageListButton`) and routes through the
  same generated-control activation path instead of a C#-specific click branch.
  Parity audit paths: C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`, `csharp/src/Uimd/Runtime/Core.cs`, and
  generated/example C# apps; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`,
  `cpp/include/ui/generated/GeneratedWindowRuntime.hpp`, terminal input/runtime
  element paths, and image/example overrides; Python reference paths under
  `src/uimd/runtime`. Focused direct PTY validation passed for `text_editor`
  startup file loading, `widget_gallery` animation refresh without input,
  direct Sixel output in `image_browser` and `image_gallery`, and
  `image_browser` left-panel reusable click activation. Focused compare
  validation passed for `tests/mcp/text_editor.yaml`,
  `tests/mcp/widget_gallery.yaml`, `tests/mcp/expense_tracker_compare.yaml`,
  `tests/mcp/image_browser_compare.yaml`, and
  `tests/mcp/image_gallery_compare.yaml`. Full validation passed:
  `./tools/rebuild_all.sh`; `python3 -m pytest python/tests` with 460 passed;
  `ctest --test-dir cpp/build --output-on-failure` with 26/26 tests passed;
  `./uimd mcp-test --all --compare python/examples cpp/build/examples
  --mcp-fast --compare-app-size 90x35` with 626 asserts passed; `./uimd
  mcp-test --headless --all --compare cpp/build/examples csharp/examples
  --mcp-fast --compare-app-size 90x35` with 626 asserts passed; and `./uimd
  mcp-test --compare tests/regressions/uimd/parity/python
  cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml
  --mcp-fast --compare-app-size 90x35` with 14 asserts passed.
- [x] **C# direct terminal runtime has broad element/input/render parity
  failures outside MCP**. User reports direct C# examples are unusable in real
  terminal execution even though MCP compare passes: Activity Feed multiline
  message TextInput/TextArea shows the cursor on the wrong row when entered by
  keyboard, direct text editing can insert raw `0B`/`OB` arrow fragments,
  `Escape` does not reliably leave edit mode, typed characters are missing or
  corrupted, Enter-confirm behavior diverges, checkbox toggle leaves focus/input
  stuck until Escape, listbox confirm likely diverges, scrollview does not scroll
  by mouse or arrows, Quit buttons do not activate, Formular Save/Cancel do not
  work, and Image Browser direct rendering shows the test checker texture over
  images. Treat this as a shared C# direct-runtime parity failure, not as
  example-specific bugs. Parity rule: C# direct terminal input loop, key/mouse
  decoding, edit-mode lifecycle, button/checkbox/listbox activation, scrollview
  dispatch, frame rendering, and image rendering must be structurally 1:1 with
  the C++ direct runtime in `cpp/src/generated/GeneratedWindowRuntime.cpp` and
  relevant C++ element/image runtime paths, with Python runtime used where it is
  the shared behavior reference. Required validation: reproduce direct C#
  failures in PTY on Activity Feed, Formular, Image Browser, and at least one
  scrollview/listbox case; compare direct behavior against C++ for the same
  key/mouse byte sequences; fix only shared runtime/generator code; verify direct
  PTY text input, multiline input, Escape/Enter, checkbox, listbox, scroll,
  Quit, Save/Cancel, and Image Browser rendering; then rebuild all examples and
  rerun Python tests, C++ ctest, Python/C++ compare, C++/C# compare, and
  regression parity with `--compare-app-size 90x35`.
  Implemented on 2026-06-24 as a shared C# runtime parity fix rather than
  example-specific patches. C# direct terminal now ports the C++ direct runtime
  event/render shape: raw terminal mode, SS3/CSI key decoding, SGR mouse press
  and wheel dispatch, edit-mode snapshots/commit/escape cleanup, active
  ScrollView descendant scope, Button/CheckBox/ListBox/ComboBox/TextInput/TextArea
  Enter/Escape behavior, direct full-frame buffered rendering, direct close
  requests, and image fallback rendering without forcing deterministic checker
  texture unless `UIMD_DETERMINISTIC_IMAGE_FALLBACK` is explicitly set. ScrollView
  state was brought in line with C++/Python with separate child index and
  line-level view offset, wheel/page/ensure-visible helpers, scroll position
  save/restore, optional `scroll-x`/`scroll-y` style defaults, and generated
  example calls updated to use the shared runtime APIs (`ScrollToTop`,
  `ScrollToBottom`, `ScrollPosition`, `RestoreScrollPosition`) instead of
  arithmetic workarounds. Parity audit paths: C#
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Elements.cs`, and
  `csharp/src/Uimd/Runtime/Core.cs`; C++ direct/runtime references
  `cpp/src/generated/GeneratedWindowRuntime.cpp` and C++ element/ScrollView
  runtime paths; Python reference runtime paths remain under
  `src/uimd/runtime`. Direct PTY validation passed for both C++ and C# with the
  same real terminal byte sequences: Activity Feed textarea click/type/SS3
  `ESC O B`/Escape/Add did not leak raw `OB`/`0B` fragments and updated the
  feed, Formular direct Save and Cancel mouse clicks exited correctly, Image
  Browser direct launch rendered the normal screen without setting
  `UIMD_DETERMINISTIC_IMAGE_FALLBACK` and direct Quit mouse click exited, and
  Task Board direct SGR wheel events scrolled the board. Full validation passed:
  `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/src/Uimd/Uimd.csproj --configuration Debug`; affected C# examples
  `task_board`, `markdown_viewer`, and `image_browser` built; focused
  `./uimd mcp-test --headless --compare cpp/build/examples csharp/examples
  tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35` with
  144 asserts; `./tools/rebuild_all.sh`; `python3 -m pytest python/tests` with
  460 passed; `ctest --test-dir cpp/build --output-on-failure` with 26/26
  tests passed; `./uimd mcp-test --all --compare python/examples
  cpp/build/examples --mcp-fast --compare-app-size 90x35` with 626 asserts
  passed; `./uimd mcp-test --headless --all --compare cpp/build/examples
  csharp/examples --mcp-fast --compare-app-size 90x35` with 626 asserts
  passed; and `./uimd mcp-test --compare tests/regressions/uimd/parity/python
  cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml
  --mcp-fast --compare-app-size 90x35` with 14 asserts passed. The separate
  direct mouse drag/text-selection parity task remains open because this fix
  consumes SGR drag/release safely but does not yet port full drag-selection
  behavior.
- [x] **C# direct calculator arrow keys still fail after regenerate/build direct
  launch**. User reports that the exact command
  `./uimd generate csharp/examples --target csharp && dotnet build
  csharp/examples/calculator/calculator.csproj --configuration Debug && dotnet
  csharp/examples/calculator/bin/Debug/net10.0/calculator.dll` leaves the direct
  C# calculator with non-working arrow keys, even though MCP compare and a prior
  PTY smoke were reported as passing. This means the validation must reproduce
  the user-visible direct terminal path after regeneration, not only MCP
  `press_key` or synthetic narrow PTY checks. Parity rule: C# direct raw input
  parsing, escape/CSI timing, normalized key names, dispatch to
  `McpController.ToolPressKey`, focus state changes, and render refresh must be
  structurally 1:1 with C++ direct terminal handling except for unavoidable
  .NET terminal primitives. Affected paths: C#
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs` direct terminal parser/runtime
  loop; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp` direct terminal event
  loop and key dispatch; Python `src/uimd/runtime/application.py` terminal key
  decoding and runtime dispatch. Required validation: run the user's exact
  generate/build/direct command path, drive the launched C# calculator through a
  real PTY with Up/Down/Left/Right escape sequences, verify focused button
  movement visibly and/or by activation result, verify mouse clicks still work,
  verify `Ctrl+C` exits, run C++ direct calculator with the same sequence as a
  parity oracle, then run focused C++/C# calculator compare with
  `--compare-app-size 90x35` and rebuild/test the affected C# examples.
  Implemented on 2026-06-24 as a parity input-parser fix, not a C#-only
  workaround. Root cause: direct terminal startup emits application-cursor mode
  control sequences, so the user's terminal sends SS3 arrow sequences
  `ESC O A/B/C/D`; the previous validation only covered CSI arrows
  `ESC [ A/B/C/D`. Added the same application-cursor arrow decoding in C#
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`, Python
  `src/uimd/runtime/application.py`, and C++ `cpp/src/terminal/Input.cpp`, with
  focused Python and C++ parser regression tests. Validation passed:
  regenerated C# examples with `./uimd generate csharp/examples --target
  csharp`; built calculator with `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/calculator/calculator.csproj --configuration Debug`; launched
  the built C# calculator directly in a PTY and drove SS3 `Down`, `Right`, `Up`,
  and `Enter`, visibly moving focus and activating the focused button; verified
  SGR mouse click still activates the calculator button; verified raw `Ctrl+C`
  exits and restores terminal state; ran the same SS3 arrow sequence against the
  rebuilt C++ calculator as parity oracle; passed `python3 -m pytest
  python/tests/test_application.py -k application_cursor`; passed `ctest
  --test-dir cpp/build -R '^ui_cpp_tests$' --output-on-failure`; passed
  `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/src/Uimd/Uimd.csproj --configuration Debug`; passed `./uimd mcp-test
  --headless --compare cpp/build/examples csharp/examples tests/mcp/calculator.yaml
  --mcp-fast --compare-app-size 90x35`; passed `./tools/rebuild_all.sh`; passed
  full `python3 -m pytest python/tests`; passed full `ctest --test-dir
  cpp/build --output-on-failure`; and passed UIMD regression parity compare
  `./uimd mcp-test --compare tests/regressions/uimd/parity/python
  cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml
  --mcp-fast --compare-app-size 90x35`. User confirmed on 2026-06-24 that the
  direct C# calculator arrow-key issue is resolved in real manual use.
- [x] **C# direct terminal arrow-key handling regressed after mouse input
  fixes**. User reports that the last C# direct terminal mouse-event fix broke
  arrow-key movement in the C# branch. This is a structural parity bug, not a
  C#-only behavior to patch independently. Parity rule: the C# direct input
  parser, key normalization, dispatch order, focus navigation, and edit-mode
  arrow routing must be a direct port of the parity-validated C++ implementation
  and Python reference, including matching method responsibilities and state
  transitions except for unavoidable .NET/terminal primitives. Affected paths:
  C# `csharp/src/Uimd/Runtime/GeneratedWindow.cs`
  `ReadDirectTerminalEvent`, escape/CSI decoding, `McpController.ToolPressKey`,
  and focus navigation helpers; C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`
  direct terminal event loop and `McpController::toolPressKey`; Python
  `src/uimd/runtime/application.py` terminal key decoding and runtime key
  dispatch. Required validation: reproduce direct C# arrow behavior in a PTY,
  compare key names/dispatch against C++ for the same escape sequences, fix C#
  structurally rather than adding example-specific logic, verify direct
  keyboard arrows, direct mouse click, and `Ctrl+C`, then run focused C#
  calculator MCP smoke, C++/C# calculator compare with `--compare-app-size
  90x35`, and rebuild all examples after runtime changes. Implemented on
  2026-06-23 in `csharp/src/Uimd/Runtime/GeneratedWindow.cs`: parity audit
  found that C# returned `FocusableElements()` sorted by frame row/column, while
  C++ preserves generated `window.elements()` traversal order and uses that
  order for `focusedIndex`. That made the same arrow key start from a different
  element in C# than in C++. C# now preserves generated traversal order and
  `MoveFocusDirection()` ports the C++ `moveFocusSpatial` algorithm directly,
  including the same best-score tuple `(bandRank, primaryGap,
  perpendicularGap, edgeDelta)`, first-candidate tie behavior, focused element
  reference synchronization, and integer center calculations. No example logic
  was changed. Validation passed: `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/src/Uimd/Uimd.csproj --configuration Debug`;
  `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/calculator/calculator.csproj --configuration Debug`;
  diagnostic C++/C# calculator arrow compare with `Down`, `Right`, `Right`
  produced matching focus values `0`, `.`, `+`; direct PTY run of
  `csharp/examples/calculator/bin/Debug/net10.0/calculator.dll` accepted real
  arrow escape sequences, activated the focused calculator button with Enter,
  accepted SGR mouse clicks without raw text, and exited on `Ctrl+C`;
  `./tools/rebuild_all.sh`; `./uimd mcp-test --headless --compare
  cpp/build/examples csharp/examples tests/mcp/calculator.yaml --mcp-fast
  --compare-app-size 90x35`; and `./uimd mcp-test --headless --all --compare
  cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35`
  passed with 626 asserts, 0 failed, and 0 step failures.
- [x] **C# direct terminal raw mode and frame rendering are not 1:1 with
  Python/C++**. User reports that direct C# calculator no longer inserts raw
  escape characters and exits with `Ctrl+C`, but real mouse clicks still do not
  work and the UI sometimes flashes/interleaves artifacts while rendering. Deep
  parity audit found two direct-mode architecture gaps: C# direct mode still
  depends on `Console.ReadKey` without entering POSIX raw mode or Windows VT
  input mode, while Python `src/uimd/runtime/application.py` uses
  `tty.setraw(fd)` / Windows console mode updates and C++
  `cpp/src/generated/GeneratedWindowRuntime.cpp` goes through terminal backend
  raw input; and C# renders direct frames line-by-line with `Console.WriteLine`
  while Python/C++ build a complete terminal frame/diff and write it as terminal
  output with autowrap disabled. Fix in shared C# runtime only: add a terminal
  mode guard for POSIX raw input and Windows VT console input/output where
  available, read direct input from raw stdin bytes through a parser instead of
  `Console.ReadKey`, add alternate-screen enter/leave parity for autowrap, and
  write each full frame as one buffered string. Required validation: direct
  calculator in a PTY accepts keyboard input, real/SRG mouse click path activates
  a button without raw text, `Ctrl+C` exits and restores terminal state, no
  frame is emitted through per-line `WriteLine`, focused MCP calculator still
  passes, and all C# examples build. Implemented on 2026-06-23 in
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`: direct C# startup now enters a
  terminal mode guard before the interactive loop. POSIX direct mode saves the
  current terminal mode with `stty -g`, switches to raw/no-echo mode equivalent
  to the Python/C++ terminal loops, and reads input through POSIX `read(0, ...)`
  rather than `Console.ReadKey`/`FileStream.ReadByte`; Windows direct mode sets
  console VT input/output flags through `kernel32` where available. Direct frame
  rendering now builds a complete ANSI frame string and emits it through one
  `Console.Write`, uses explicit `\r\n` between rows because raw mode disables
  output post-processing, disables autowrap on enter (`?7l`), and restores it on
  leave (`?7h`). Validation passed: direct PTY run of
  `csharp/examples/calculator/bin/Debug/net10.0/calculator.dll` rendered the UI,
  accepted keyboard `4`, accepted a synthetic SGR mouse press on the `4` button
  and updated the display to `44` without raw escape text, exited on raw `0x03`
  `Ctrl+C`, and restored alternate-screen/mouse/autowrap state; `Console.ReadKey`
  no longer appears in `csharp/src/Uimd/Runtime`; the direct frame loop no longer
  uses per-row `Console.WriteLine`; `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/src/Uimd/Uimd.csproj --configuration Debug`;
  `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/calculator/calculator.csproj --configuration Debug`; `./uimd
  mcp-test --backend python --headless
  csharp/examples/calculator/bin/Debug/net10.0/calculator.dll
  tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35` with 5 asserts
  passed; and `./tools/rebuild_all.sh` passed with the existing non-blocking
  `activity_feed` `SetAutoScroll` hiding warning and existing C++ duplicate
  library linker warnings.
- [x] **C# direct terminal input is not 1:1 with Python/C++**. User reports
  that direct C# calculator cannot be reliably stopped with `Ctrl+C` and that
  clicking buttons injects escape-sequence characters rapidly into the app.
  Root parity issue: C# direct mode enabled terminal mouse reporting and
  alternate-screen behavior, but still reads input through `Console.ReadKey`
  instead of a Python/C++-equivalent terminal input parser. Mouse SGR escape
  sequences therefore reach app key handling as printable characters, and
  `Ctrl+C` is not normalized consistently. Fix in shared C# runtime only:
  direct C# input must parse `Ctrl+C`, keyboard keys, and SGR mouse press
  reports into the same logical runtime operations used by Python/C++, route
  mouse clicks through shared focus/activation cleanup, and must not add
  calculator-specific handling. Required validation: direct PTY run of
  calculator exits on `Ctrl+C`, keyboard digit input updates display once, and
  a mouse click on a calculator button activates that button without inserting
  raw escape-sequence text; rebuild C# examples and rerun focused calculator MCP
  smoke. Implemented on 2026-06-23 in shared C# runtime
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`: direct terminal input now parses
  raw `0x03` `Ctrl+C`, ordinary key characters, CSI modified keys, and SGR mouse
  reports instead of allowing escape-sequence bytes to reach app text handling.
  SGR mouse press events route through the same `McpController` focus/hit-test
  activation path as MCP mouse clicks, and wheel events dispatch to existing
  `ScrollView`/`ListBox` scroll APIs. No calculator-specific logic was added.
  Validation passed: direct PTY run of
  `csharp/examples/calculator/bin/Debug/net10.0/calculator.dll` accepted
  keyboard `4` once and updated the display to `4`; synthetic SGR mouse press
  `ESC [ < 0 ; 33 ; 13 M` on the calculator `4` button activated the button
  without printing raw escape text; raw `0x03` exited the app and restored
  alternate-screen/mouse tracking state; `/Users/marekdubovsky/.dotnet/dotnet
  build csharp/src/Uimd/Uimd.csproj --configuration Debug`;
  `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/calculator/calculator.csproj --configuration Debug`;
  `./uimd mcp-test --backend python --headless
  csharp/examples/calculator/bin/Debug/net10.0/calculator.dll
  tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35` with 5 asserts
  passed; all `csharp/examples/*/*.csproj` built successfully with only the
  existing non-blocking `activity_feed` `SetAutoScroll` hiding warning; and
  `./tools/rebuild_all.sh` passed during the fix cycle.
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
- [x] **C# examples exit after direct terminal launch instead of staying
  interactive**. User reproduced `dotnet
  csharp/examples/calculator/bin/Debug/net10.0/calculator.dll`: the app renders
  a static calculator screen, then returns to the shell prompt instead of
  entering the normal interactive runtime loop. MCP compare still passes, so
  the generated C# examples compile and the MCP/headless path can drive them,
  but direct user execution is broken. Fix this first so C# examples can be
  manually tested. Parity decision: direct C# app startup and terminal loop must
  behave 1:1 with Python/C++ direct app execution; changes belong in shared C#
  runtime/generator startup plumbing, not in individual examples. Required
  validation: run a direct C# example in an interactive terminal/PTY and verify
  it remains active for keyboard input, then rerun focused C# build and MCP
  smoke to ensure the MCP path still works. Implemented on 2026-06-23 in
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`: direct non-MCP launches now use
  the same `McpController` key/focus/window state machine as MCP, rendering the
  active frame in an interactive terminal loop and mapping `Console.ReadKey`
  input to UIMD key names. Follow-up fix after user screenshot: direct C#
  rendering now enters/leaves alternate screen like Python/C++, renders through
  a terminal-content path equivalent to C++ `renderGeneratedWindow(...)` rather
  than the MCP viewport snapshot path, and emits ANSI foreground/background
  sequences from `TerminalCell` data instead of plain text. Non-interactive
  redirected launches keep the old render-once fallback to avoid hanging
  scripts. Validation passed: direct PTY run of
  `csharp/examples/calculator/bin/Debug/net10.0/calculator.dll` stayed active,
  accepted `1+2=`, and updated the display to `3`; follow-up direct PTY run
  emitted ANSI color/background output, accepted `4`, updated the display to
  `4`, and restored alternate screen cleanly on `Ctrl+C`;
  `/Users/marekdubovsky/.dotnet/dotnet build
  csharp/examples/calculator/calculator.csproj --configuration Debug`;
  `/Users/marekdubovsky/.dotnet/dotnet build csharp/src/Uimd/Uimd.csproj
  --configuration Debug`; `./uimd mcp-test --backend python --headless
  csharp/examples/calculator/bin/Debug/net10.0/calculator.dll
  tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35` with 5
  asserts passed; and all `csharp/examples/*/*.csproj` built successfully
  with one existing non-blocking `activity_feed` `SetAutoScroll` hiding warning.
- [x] **MCP dialog compare must validate the full app background behind modal
  windows**. User reports visual differences in dialog/background rendering for
  C# examples using screenshots `/Users/marekdubovsky/Desktop/image_gallery.png`,
  `/Users/marekdubovsky/Desktop/texteditor_dialog.png`, and
  `/Users/marekdubovsky/Desktop/texteditor_dialog_messagebox.png`. Current
  tests can pass while missing background/backdrop differences behind the
  foreground dialog, which is not acceptable for parity. Add full-surface
  background validation for dialog states while keeping a future option to test
  only the foreground dialog window when explicitly requested. Parity decision:
  compare tests must protect the whole visible application surface, including
  underlying app content, dialog backdrop/dimming, and modal window foreground,
  across Python, C++, and C#. Affected tester paths include
  `src/uimd/testing/mcp_tester.py` and `cpp/tools/mcp_tester/main.cpp`; if the
  visual differences are runtime bugs, fix the platform runtimes instead of
  masking tests. C# runtime subtask: port the C++
  `BackgroundFocusCleanupContext` modal-close cleanup from
  `cpp/src/generated/GeneratedWindowRuntime.cpp` into
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs` so background scrollview focus is
  captured before modal button activation, rendered while the modal is open, and
  cleaned only after the modal callback has finished changing the base window.
  This must stay structurally 1:1 with C++ and must not use example-specific
  dialog, task board, or calculator logic. Implemented on 2026-06-24 as a
  full-surface MCP snapshot/runtime parity fix rather than a dialog-only mask.
  C++ MCP `get_render_snapshot` and `get_render_snapshot_compact` now support
  `render_scope` with default `full_surface`, while `active_window` remains
  available for future focused foreground-window checks. Full-surface rendering
  includes the base application, modal dim/backdrop, and foreground window by
  rendering the base with active scrollview scope visuals suppressed, then
  composing the window stack. C# ports the same shape with
  `SuppressActiveScrollViewScopeVisuals`, modal `DimBackground`, full-surface
  snapshot content, C++-equivalent image cell-background rendering for compare
  snapshots, and focused reusable/scrollview gap rendering that protects actual
  content rows instead of repainting them with focused background. Parity audit:
  C++ paths are `cpp/src/generated/GeneratedWindowRuntime.cpp` and
  `cpp/include/ui/generated/GeneratedWindowRuntime.hpp`; C# paths are
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs`,
  `csharp/src/Uimd/Runtime/Core.cs`, and
  `csharp/src/Uimd/Runtime/Elements.cs`; Python MCP snapshot handling remains
  in `src/uimd/runtime/mcp.py` and runtime rendering behavior remains in
  `src/uimd/runtime/UIBase.py`, `src/uimd/runtime/UIScrollView.py`, and
  `src/uimd/runtime/image.py`. Validation passed after a full rebuild:
  `./tools/rebuild_all.sh`; `python3 -m pytest python/tests` with 460 passed;
  `ctest --test-dir cpp/build --output-on-failure` with 26/26 tests passed;
  `./uimd mcp-test --all --compare python/examples cpp/build/examples
  --mcp-fast --compare-app-size 90x35` with 626 asserts passed; `./uimd
  mcp-test --headless --all --compare cpp/build/examples csharp/examples
  --mcp-fast --compare-app-size 90x35` with 626 asserts passed; and `./uimd
  mcp-test --compare tests/regressions/uimd/parity/python
  cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml
  --mcp-fast --compare-app-size 90x35` with 14 asserts passed.
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
