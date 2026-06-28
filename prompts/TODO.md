# TODO

## Current Open Tasks

Date: 2026-06-21

- [ ] **Add regression compare coverage for C# parity bugs that manual testing
  found after MCP compares passed**. Source notes: removed completed TODO blocks
  and git commits `768c481`, `02808c7`, and `9e7a0e8`; SVN history is not
  available in this checkout (`svn log` reports that the repo is not an SVN
  working copy). The goal is not to change example functionality, layout,
  visuals, data, or user-facing workflows. Add or extend tests and shared tester
  instrumentation so the existing examples can expose the historical C# vs
  Python/C++ parity failures through compare/direct regression tests. Example
  code may only gain missing MCP/test hooks or test-only plumbing that preserves
  normal behavior and visual output.

  Test infrastructure to design first: compare should be able to drive real
  PTY/direct-terminal key and mouse byte sequences where MCP tool calls are too
  high level, capture intermediate focus/edit/scroll state, inspect terminal
  teardown bytes, inspect modal/full-surface snapshots, and assert image render
  mode metadata without adding sleeps, waits, or test-side cleanup that masks
  runtime bugs. Keep `--compare-app-size 90x35` in documented compare commands.

  Historical C# parity failures to cover, with likely test hosts:
  1. `calculator`: direct C# examples rendered once and exited instead of
     staying interactive; add a direct PTY smoke that waits for the app loop,
     sends keys, verifies render changes, and exits cleanly.
  2. `calculator`: direct input used `Console.ReadKey`, so SGR mouse sequences
     leaked into text and `Ctrl+C` did not exit reliably; add PTY coverage for
     raw `Ctrl+C`, ordinary key input, SGR press, release, drag consumption, and
     absence of raw escape text in app state.
  3. `calculator`: direct terminal raw mode/frame rendering diverged from
     Python/C++ and could flash or interleave line-by-line artifacts; add a
     direct-frame output check for alternate screen, autowrap restoration, and
     full-frame buffered writes.
  4. `calculator`: arrow keys failed in direct C# because SS3 application-cursor
     sequences `ESC O A/B/C/D` were not parsed; add direct PTY compare coverage
     for both CSI and SS3 arrows, focus movement, Enter activation, mouse click,
     and `Ctrl+C` teardown.
  5. `activity_feed`: multiline TextInput/TextArea direct editing put the cursor
     on the wrong row, leaked raw `OB`/`0B` arrow fragments, mishandled Escape
     and Enter, and corrupted typed text; add direct or MCP compare steps for
     click/type/arrow/Escape/Add on the existing input without changing the UI.
  6. `formular`: Save/Cancel and Quit activation diverged in direct C#; add
     direct or MCP compare steps for button mouse activation and direct exit.
  7. `formular`: ListBox mouse clicks entered edit mode instead of selecting,
     and keyboard arrows plus Enter could not toggle multi-select items; extend
     existing ListBox compare coverage for mouse select, arrow movement,
     Enter-toggle, selected values, active row, and edit-mode cleanup.
  8. `formular` or `text_editor`: TextInput/TextArea/NumberInput selection and
     copy feedback diverged; add compare/direct coverage for shift selection,
     mouse drag selection where supported, copy feedback, and release/click
     candidate cleanup. Keep the separate open drag-selection task for the
     remaining unimplemented C# drag behavior.
  9. `text_editor`: direct C# launched from `bin/Debug/net10.0` could not find
     `notes/welcome.txt`; add a direct startup/resource-path regression that
     verifies the welcome note loads from the generated build output path.
  10. `text_editor`: TextArea with content taller than its viewport did not
      scroll with the mouse wheel in C#; add wheel-scroll compare coverage for
      internal TextArea scrolling and cursor visibility.
  11. `widget_gallery`: animated labels updated only after input refreshed the
      frame; add a tick/render-loop compare that advances time without input and
      checks the visible animated state changes consistently.
  12. `expense_tracker`: ScrollView keyboard traversal wrapped/restarted at the
      top at the bottom edge; add focused ArrowDown edge coverage that asserts
      focus and scroll offset remain at the end instead of jumping to the top.
  13. `expense_tracker`: spatial Right from an expense-row checkbox did not move
      to the row Delete button, and checkbox/delete mouse hit testing through
      ScrollView reusable children missed or routed incorrectly; add compare
      steps for row checkbox, row Delete, and spatial navigation inside a row.
  14. `expense_tracker`: newly added categories were not selectable until
      switching tabs, and adding an expense did not scroll to the inserted
      expense; add compare coverage for add-category ListBox selection and
      add-expense scroll visibility using existing app behavior.
  15. `task_board`: returning from the Edit Task dialog or message boxes lost
      originating ScrollView/reusable focus and could scroll the board; add
      modal open/close coverage that asserts background focus/edit state and
      scroll position are preserved.
  16. `task_board`: deleting a reusable row reset focus/scroll to the beginning
      instead of handing focus to the next remaining item, or previous item when
      deleting the last item; add compare coverage for both delete handoff
      boundary cases without changing task data semantics.
  17. `task_board` and `image_browser`: MessageBox/FileBrowser Escape did not
      produce the same No/Cancel result or transient highlight; add modal
      lifecycle coverage for Escape, No/Cancel highlighting, result callback,
      and cleanup order.
  18. `markdown_viewer`: scrolling felt delayed or continued after input; add
      deterministic compare coverage for wheel/page/key scroll deltas and
      no-extra-scroll-after-input behavior.
  19. `image_browser`: direct C# initially did not use Sixel when available and
      sometimes rendered deterministic checker/fallback or low-depth/pixelated
      images while C++ used Sixel; add image render-mode assertions for normal
      view and preview dialog, with explicit fallback-mode checks kept separate.
  20. `image_browser` and `image_gallery`: image sizing differed from C++, Sixel
      output corrupted the screen, and direct terminal teardown left raw mouse or
      terminal bytes; add compare/direct coverage for image cell geometry,
      render-mode metadata, clean Quit/Ctrl+C teardown, and no stray bytes.
  21. `image_browser`: direct C# stalled at high CPU and input stopped
      responding because image rasters/Sixel payloads were rebuilt every frame;
      add a tester-level performance/regression check or debug metric for
      repeated image frames that can catch unbounded rerender/cache misses
      without relying on brittle wall-clock timing.
  22. `image_browser`: left-panel keyboard navigation through reusable image
      buttons failed; add compare steps for arrow traversal through Photos,
      Gallery, and Image items reusable controls.
  23. `image_browser`: row `Show`, `Browse`, and `Delete` buttons in reusable
      image-list rows missed mouse activation or routed through C#-specific
      branches; add compare coverage for each row action through shared
      generated-control activation.
  24. `image_browser`: preview modal `Close` mouse click missed due to modal
      coordinate transforms; add full-surface modal mouse-hit coverage for the
      image preview close button.
  25. `image_browser`: after Browse/FileBrowser close, focus returned to the
      wrong place or lost the originating ScrollView/reusable focus; add compare
      coverage for opening Browse and closing/canceling while preserving
      background focus and scroll state.
  26. `image_browser`: focused `Image items` ScrollView showed black background
      rectangles around or to the right of Delete buttons; add visual snapshot
      assertions for reusable row background blending when focused and unfocused.
  27. `image_browser`: partially clipped images inside ScrollViews clipped from
      the wrong edge; add snapshot coverage for top and bottom partial clipping
      in the main/gallery ScrollView.
  28. `image_browser`: ArrowDown at the bottom of focused `Image items`
      ScrollView jumped to the beginning; add direct/compare coverage that holds
      focus at the lowest item and asserts no scroll reset.
  29. `image_browser`: Gallery wheel scrolling was too slow in fallback image
      mode; add a non-visual regression signal for fallback image sampling/cache
      reuse during repeated wheel scrolls.
  30. `image_browser`: fallback half-block images dimmed with black horizontal
      bands when an active ScrollView overlay dimmed the background; add visual
      compare coverage that validates foreground and background sample colors
      are both darkened, not replaced with black.
  31. `image_browser`: root-window MCP `mouse_click` coordinate handling
      diverged from modal/window-stack clicks and caused a `formular` compare
      step failure after runtime fixes; add coordinate-dispatch regression
      coverage for root window, nested reusable controls, and modal windows.
  32. `image_browser` and `image_gallery`: C# image fallback/Sixel render cache,
      source raster cache, terminal cell pixel sizing, and quantization must be
      covered against C++ so future image-runtime changes do not pass static
      compare while failing direct terminal use.
  33. `image_browser`, `task_board`, and nested ScrollViews generally: active
      ScrollView focus scope, excluded reusable child rects, root generated
      ScrollView indicator suppression, focus-background gap alpha blending,
      modified-cell writeback, and modal full-surface dim/backdrop rendering
      must have visual compare coverage.
  34. `FileBrowser`/ListBox dialogs: selected item in a long list must be kept
      visible by native ListBox scrolling; add compare coverage using existing
      browse dialog data or test-controlled temporary files without app-specific
      scroll workarounds.
  35. `NumberInput`: clicking a zero-valued field and typing must replace `0`
      with the first typed digit like keyboard Enter edit mode; add mouse-start
      and keyboard-start compare coverage, plus click-outside commit/cancel
      state checks.
  36. Direct terminal title: direct apps displayed `dotnet` instead of
      `<App Name> [C#]` and the equivalent Python/C++ suffixes; add a PTY
      title-sequence smoke that checks sanitized title output for all targets.
  37. Full-surface modal snapshots: tests previously compared only the foreground
      window and missed background/backdrop differences; keep default compare
      snapshots full-surface and add explicit coverage for modal background
      dimming, active-scrollview suppression behind modal windows, and dialog
      foreground content.
  38. Color/render sentinel parity: C# treated `#00000000` like transparent
      instead of only the explicit `transparent` sentinel; add cross-target
      compare/regression coverage for `transparent`, `#00000000`, alpha ignored
      in string form, and fallback image cell background handling.

  Coverage added on 2026-06-27 so far:
  - `tests/mcp/formular.yaml` now asserts ListBox edit-mode entry, keyboard
    Down/Enter multi-select toggling, selected values, and edit-mode cleanup.
  - `tests/mcp/task_board_compare.yaml` now covers dialog cancel focus restore,
    re-entering a ScrollView after modal close, bottom ArrowDown behavior,
    MessageBox Escape/No/Yes lifecycle for Mark All Done and Clear Board, and a
    full snapshot at the bottom edge so focus/scroll cannot jump back silently.
  - `tests/mcp/image_browser_compare.yaml` now covers `Image items` ScrollView
    bottom ArrowDown behavior, focused-row snapshots, row Show/Browse actions
    through shared `mouse_press`/`mouse_release`, preview Close mouse hit
    testing, Browse/FileBrowser close focus restore, Delete MessageBox No focus
    restore, normal/preview image render metadata, gallery scroll snapshots,
    fallback focused image cell sampling with `get_render_cell`, and fallback
    preview image metadata.
  - `tests/mcp/image_gallery_sixel_info_compare.yaml` now covers Sixel and
    fallback image render parity without pixel-perfect matching. It compares
    stable render metadata (`fit`, configured/resolved render mode, source
    dimensions, element bounds, computed image rect, visible rect, and absolute
    visible placement), asserts raw Sixel payload presence for Sixel elements,
    and checks a coarse quantized `sample_signature` so misplaced, blank, tiny,
    or completely wrong-color images are caught while minor raster differences
    are tolerated.
  - `tests/mcp/expense_tracker_compare.yaml` now covers expense ScrollView
    modal-return focus, bottom ArrowDown behavior, newly added expense
    visibility after inserting from a scrolled state, mouse-start NumberInput
    zero replacement, row checkbox/delete mouse hit testing, and spatial Right
    navigation from a row checkbox to its Delete button.
  - `tests/mcp/text_editor.yaml` now covers a multi-line TextArea scroll
    snapshot. C# MCP `scroll` was aligned with the existing C++/Python
    behavior for TextInput/TextArea targets so this can be tested through the
    shared MCP tool instead of a direct-terminal-only workaround.
  - Added shared MCP/runtime tester tools for stateful mouse press/move/release
    sequences, single-cell render inspection, image render-info inspection, and
    per-step `compare_fields` across Python, C++, C#, the Python tester, and
    the native C++ tester. `mouse_click` and `mouse_drag` now execute through
    the same press/release event path, and the existing examples exercise the
    new tools without changing example behavior or visuals.
  - C# modal Escape cleanup was aligned with the C++ behavior for standard
    Escape/cancel buttons while preserving ordinary modal button-close
    background rendering. Parity decision: Python/C++ already render modal
    backgrounds without leaving active ScrollView scope visuals after Escape;
    the C# change is the smallest runtime-state adapter needed to make the same
    public behavior observable in full-surface snapshots.
  - Fully covered by MCP compare in this branch: items 7, 10, 12, 13, 14, 15,
    17, 19, 22, 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, 35, and 37. Item 20 is
    covered for image geometry/render metadata/Sixel payload presence but not
    direct terminal teardown bytes. Item 38 is covered for fallback image cell
    background handling but not the separate `transparent`/`#00000000`
    sentinel matrix.
  - Still open after this coverage pass: direct-terminal-only items 1, 2, 3, 4,
    5, 6, 8, 9, 20 teardown bytes, 36, and direct drag selection; render-loop
    item 11; markdown scroll item 18; performance/cache metric items 21 and 29;
    task deletion boundary handoff item 16; long-list FileBrowser visibility
    item 34; and the remaining color-sentinel matrix from item 38.
  - Focused validation passed on 2026-06-27: `tests/mcp/image_browser_compare.yaml`
    for C++/C# (`126 asserts`), `tests/mcp/image_gallery_sixel_info_compare.yaml`
    for C++/C# (`52 asserts`), `tests/mcp/task_board_compare.yaml` for C++/C#
    (`176 asserts`), `tests/mcp/expense_tracker_compare.yaml` for C++/C#
    (`108 asserts`), `tests/mcp/formular.yaml` for C++/C# (`84 asserts`), and
    `tests/mcp/text_editor.yaml` for C++/C# (`128 asserts`). The broader
    regenerate/build gate also passed:
    `PATH=/Users/marekdubovsky/.dotnet:$PATH ./tools/rebuild_all.sh`.
    Additional validation passed: `python3 -m pytest
    python/tests/test_mcp_tester.py -q` (`53 passed`) and
    `ctest --test-dir cpp/build --output-on-failure` (`26 passed`).
  - Attempted long-list FileBrowser keyboard coverage in
    `tests/mcp/image_browser_compare.yaml` exposed a separate C# full-surface
    modal background parity bug after repeated `Down`: C++ renders the
    background `Image items` content one row higher after the FileBrowser
    ListBox selection moves, while C# keeps the pending active ScrollView layout
    behind the modal. The failing coverage was not kept in the main YAML until
    the runtime bug is fixed; tracked separately below as an open task.

  When implementing this task, prefer adding or extending MCP YAML scripts under
  `tests/mcp/` and shared tester/runtime inspection APIs. Do not add
  example-specific branches, waits, artificial data, layout constants, or visual
  tweaks. If a failure cannot be captured by current MCP tools, first add the
  minimal general-purpose tester/tool support needed to observe the shared
  runtime state, then test it through existing examples.

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
