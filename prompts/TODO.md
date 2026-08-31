# TODO

## Current Open Tasks

Date: 2026-06-21

- [x] **Audit C# mouse-to-cursor placement in nested/modal TextInput and
  TextArea controls against the Python reference.** Reported from the generated
  C# ContactsManager contact add/edit window on 2026-08-31: a physical mouse
  click focuses the requested control but places its cursor at source position
  zero, while keyboard Enter and the root Formular example behave correctly.
  Determine whether the defect is example logic, generated ownership/frame
  metadata, modal coordinate translation, or shared C# runtime hit mapping.
  Reproduce both TextInput and TextArea through the shared runtime path, audit
  equivalent Python/C++ behavior, and keep any eventual repair out of the
  example.

  **Confirmed diagnosis (2026-08-31):** both ContactsManager `.uimd` sources
  are byte-identical to Python, and a real-PTY SGR click plus text insertion
  reproduces the divergence: C++ produces `KovZac` and
  `Primary Zcontact`, while C# produces `ZKovac` and
  `ZPrimary contact`. In
  `csharp/src/Uimd/Runtime/GeneratedWindow.cs::McpController.ToolMousePressAt`,
  the physical terminal point and initial target frames are correctly local to
  the active modal. When the click changes focus, `FocusElement` calls
  `RenderContent()`, whose viewport renderer rewrites the modal element frames
  to absolute viewport coordinates. The handler then subtracts those absolute
  frames from the still-local pointer before calling `CursorForPoint`; the
  negative result clamps to source position zero. Root Formular keeps both
  values in the same coordinate space, and Enter does not perform pointer
  mapping, which explains both reported exceptions. TextArea inherits the
  TextInput mapping. NumberInput and other modal handlers that derive a local
  row/column after the same focus transition must be audited in the eventual
  shared-runtime repair. Python retains the hit rectangle through focus, and
  C++ focus does not rerender/rebase frames in the middle of dispatch.

  **Completed on 2026-08-31:** the shared C# mouse-press dispatcher now keeps
  the hit-test layout stable while changing focus, then performs TextInput,
  TextArea, NumberInput, and other local pointer mapping in that same coordinate
  space. Other programmatic focus paths retain their existing layout refresh,
  so the repair changes no public API. The C# runtime regression suite passed
  6/6 including modal TextInput and TextArea cases. A real PTY replay passed on
  both C++ and C#, producing `KovZac` and `Primary Zcontact` after physical SGR
  clicks. The complete C# ContactsManager MCP scenario passed 21/21, frozen
  C# `0.5.3` compatibility passed, every supported platform/example was
  regenerated and rebuilt, and the resulting artifact manifest validated.

- [x] **Audit idle Go animation progression in WidgetGallery and
  SpecialElements against Python/C++.** Reported on 2026-08-31: animated
  content advances only after a key or mouse action. Compare the shared Go
  terminal event loop, bounded input polling, animation detection, tick/dirty
  scheduling, monotonic cadence, and render presentation with the reference
  runtime. Prove whether the divergence is shared runtime behavior or example
  logic; any eventual repair must use the centralized runtime timing contract
  and must not introduce an example-specific timer.

  **Confirmed diagnosis (2026-08-31):** the Python and Go WidgetGallery and
  SpecialElements `.uimd` sources are byte-identical, and Go's gradient color
  calculation already uses wall-clock frames correctly. The divergence is in
  `go/src/uimd/runtime.go::runInteractiveTerminal`: `dirty` is scheduled only
  for resize, window-stack changes, notification expiry, and user input. There
  is no animated-style discovery, idle tick, or bounded animation render
  cadence. C++ detects animated gradients recursively and schedules a frame on
  its centralized 70 ms cadence; Python propagates idle `tick()` calls and each
  gradient invalidates at its configured interval. The existing real-PTY idle
  animation probe passes C++ and deterministically fails Go before its first
  idle sample with `Go animated gradient emitted no terminal update`. A later
  key/mouse action marks the frame dirty, so the wall-clock gradient appears to
  advance only at that moment, exactly matching the report.

  **Completed on 2026-08-31:** the shared Go terminal loop now discovers
  animated foreground/background gradients recursively through element state
  styles, ScrollView children, reusable controls, view hosts, and nested
  generated windows, and schedules dirty frames on the same centralized 70 ms
  cadence used by C++. No example timer or platform-specific application logic
  was added. Go runtime tests passed, the permanent direct-terminal smoke now
  covers idle animation and passed 11/11, SpecialElements compare passed 10/10,
  WidgetGallery compare passed 123/123, frozen Go `0.5.3` compatibility passed,
  every supported platform/example was regenerated and rebuilt, and the
  resulting artifact manifest validated.

- [x] **Define a uniform public-API deprecation lifecycle across every UIMD
  language port.** When the user explicitly decides that a documented API is
  obsolete, retain it as a compatibility adapter, record the version and
  replacement, emit the native platform warning, and remove it automatically
  only at the second subsequent middle-version release. Document the policy in
  `AGENTS.md`, the cross-platform bug-fix workflow, the new-platform workflow,
  and a central deprecation register. Completed on 2026-08-28: the lifecycle is
  now mandatory in `AGENTS.md`, both development workflows reference it, and
  `docs/deprecations.md` defines the active/removed register and the automatic
  `0.x.y` -> `0.(x+2).0` removal rule.

- [x] **Replace the global pre-focused key workaround with a window/element
  event pipeline and resolve GitHub issue
  [#14](https://github.com/uimd-lang/uimd/issues/14).** Requested by the user
  on 2026-08-28. Introduce one shared dispatch contract,
  `window.onPreviewKey -> focusedElement.onKey -> window.onKey`, where each
  stage runs only when the preceding stage returned unhandled. The preview
  event must carry the logical key, the focused element's stable full runtime
  ID/path, and edit-mode state. Add a cancellable ListBox item-activation event
  carrying the full element ID, active index, and value; migrate FileBrowser
  Enter handling to that semantic event and migrate ActivityFeed Settings
  Escape handling to the window preview event. Keep ordinary after-the-fact
  change/selection notifications non-cancellable.

  **Compatibility and deprecation contract:** this is an additive `0.5.x`
  public API change. Preserve the existing `onKeyBeforeFocusedElement` /
  equivalent generated hook with its original callback order and local-name
  arguments as a behavior-preserving adapter, mark it deprecated with native
  warnings across every affected port, and register it in
  `docs/deprecations.md` as deprecated since `0.5.4`, replaced by
  `onPreviewKey`, and automatically removable in `0.7.0`. Do not change the
  existing `onKey` signature. Previously generated `0.5.3` applications must
  compile and run without editing or regeneration.

  **Port parity matrix (recorded before implementation):**

  | Port or layer | Reference path/function | Current behavior | Required action | Focused validation |
  | --- | --- | --- | --- | --- |
  | Python reference | `src/uimd/runtime/{application.py::UIApplication._handle_key_impl,UIBase.py::handle_key,elements.py::ListBox.handle_key}` and `src/uimd/dialogs/file_browser.py` | Window dispatch has no public preview/fallback hooks; FileBrowser overrides the complete window `handle_key` to intercept ListBox Enter. | **Fix first:** add the shared preview/target/fallback order, canonical runtime element IDs, cancellable item activation, and migrate FileBrowser without an example workaround. | Focused Python dispatch-order, repeated-ID, ListBox-consumption, and FileBrowser tests. |
  | C++ runtime | `cpp/include/ui/generated/GeneratedWindowRuntime.hpp`, `cpp/include/ui/generated/GeneratedWindowBase.hpp`, and `cpp/src/generated/GeneratedWindowRuntime.cpp` | `onKeyBeforeFocusedElement` runs before target handling but receives only `focused->name()`; `onKey` is the later window fallback. | **Fix:** add `KeyEvent`, `onPreviewKey`, owner-aware ListBox activation, full `elementIdForElement` identity, and preserve the old callback at its existing point. | Focused root, stack-frame, repeated reusable, edit/non-edit, and consumed/unhandled C++ tests. |
  | C# runtime | `csharp/src/Uimd/Runtime/{GeneratedWindow.cs,Dialogs.cs}` | Mirrors the local-name global callback and FileBrowser workaround. | **Fix:** port the same event object, order, activation dispatch, migration, and `[Obsolete]` compatibility surface. | C# runtime console dispatch and FileBrowser regression. |
  | Go runtime | `go/src/uimd/{runtime.go,dialogs.go}` | Mirrors the callback/options model and FileBrowser workaround; generated handlers are interface-bound. | **Fix:** add equivalent event/interface dispatch and Go `Deprecated:` markers without adding required struct fields to old positional construction. | Focused Go runtime and dialog tests. |
  | Swift runtime | `swift/src/Uimd/Sources/Uimd/Uimd.swift` | Mirrors the callback and FileBrowser workaround; it already computes full runtime element IDs. | **Fix:** add the equivalent event, hooks, semantic activation, migration, and Swift availability warning. | Swift package dispatch/dialog tests. |
  | Rust runtime | `rust/src/uimd/src/{runtime.rs,dialogs.rs}` | Exposes both option callbacks and generated behavior hooks; FileBrowser behavior intercepts Enter by local name. | **Fix:** add equivalent event/trait dispatch and item activation while retaining deprecated fields/hooks without breaking old exhaustive construction patterns. | Focused Rust runtime/dialog tests plus clippy. |
  | Java runtime | `java/src/main/java/uimd/{GeneratedWindowRuntime,GeneratedWindowRuntimeOptions,GeneratedWindowBase,FileBrowser}.java` | Mirrors the local-name callback and FileBrowser workaround. | **Fix:** add the equivalent immutable event, hooks, activation, migration, and `@Deprecated(since="0.5.4", forRemoval=true)` adapter. | Focused JUnit runtime/dialog tests plus Checkstyle. |
  | Native generator/generated APIs | `cpp/tools/uimd/Native{Python,Cpp,CSharp,Go,Swift,Rust,Java}Generator.cpp` and representative generated classes | ListBox emits selection-change only; generic preview/target-window hooks are absent or inconsistently exposed. | **Fix canonical emitters:** generate only the ListBox item-activate hook for ListBox members, bind the window preview/fallback hooks consistently, regenerate all supported outputs, and never hand-maintain divergent generated copies. | Native generator tests plus representative generated-source assertions. |
  | Terminal/MCP adapters | Per-port terminal and MCP key entry points | They deliver logical key strings but traverse several duplicated runtime dispatch paths. | **Fix shared routing:** every physical/MCP key path must use the same preview/target/fallback and activation order; transports and key names remain unchanged. | Focused MCP `press_key` parity and direct-terminal smoke, followed by one consolidated full gate. |

  **Validation policy:** run focused tests while implementing each port, then
  regenerate and build all examples because this changes global input routing
  and generated public APIs. Run the retained previous-version compatibility
  gate, affected FileBrowser/ActivityFeed direct and MCP compares at
  `--compare-app-size 90x35`, and one final
  `./tools/test_all.sh --live-report --keep-going` after every focused gate is
  green. Do not weaken snapshots, insert waits, or special-case examples.

  **Go validation follow-up (2026-08-28):** the first consolidated full gate
  exposed recursive dispatch when an application embeds a generated Go UI but
  does not override `OnPreviewKey`: Go method promotion made the generated
  forwarding method appear to be an application override and it forwarded
  back to the same owner. Fix this in the canonical Go generator/runtime
  dispatch, preserve separate `SetEventHandler` objects, and cover both an
  implicit promoted handler and a real application override before rerunning
  the failed Go direct/MCP phases.

  **Completed on 2026-08-28:** all supported ports now implement the shared
  preview/element/fallback pipeline and semantic ListBox activation with stable
  full element IDs. FileBrowser and ActivityFeed use the new semantic hooks;
  the legacy pre-focused callback remains behavior-compatible, is deprecated
  since `0.5.4`, and is registered for removal in `0.7.0`. All generated
  outputs were regenerated and every platform/example rebuild passed. Focused
  runtime, generator, dialog, direct-terminal, MCP, clippy/checkstyle, and
  previous-version compatibility gates passed. The first consolidated full
  gate passed 31/36 phases and isolated all five remaining failures to the Go
  method-promotion recursion described above. After the canonical Go fix, its
  runtime regression, direct-terminal smoke (10/10), both regression parity
  compares, Swift and Java cross-check smokes, and the complete Go example MCP
  compare (14/14 scripts, 1980/1980 assertions) all passed with zero failures.

- [x] **Fix GitHub issues #7, #12, and #11 as one shared element-rendering
  and interaction-geometry batch, in the user-selected order.** Selected by
  the user on 2026-08-27. The batch is limited to shared TextInput alignment,
  ComboBox rendered-versus-interactive dropdown geometry, and reusable /
  generated ScrollView focus-background compositing. Each issue keeps its own
  deterministic reproduction and focused regression; the expensive complete
  supported-platform gate runs once only after all three focused repairs and
  compatibility gates are green.

  - [x] [#7: C++ TextInput ignores `text-align: right`](https://github.com/uimd-lang/uimd/issues/7).
    Reproduce a right- and center-aligned TextInput whose text fits without
    horizontal scrolling. The visible text, cursor, and selection must share
    the resolved alignment offset; overflowing or horizontally scrolled text
    must retain the canonical left/scrolled behavior.
  - [x] [#12: ComboBox mouse hit area is clipped to six dropdown rows](https://github.com/uimd-lang/uimd/issues/12).
    Reproduce an edited ComboBox with more than six visibly rendered options
    and click a visible option below row six. Rendering, hit testing, selected
    index/value notification, and edit cleanup must use the same dropdown
    geometry.
  - [x] [#11: Reusable/generated ScrollView focus background ignores alpha child backgrounds](https://github.com/uimd-lang/uimd/issues/11).
    Reproduce a focused reusable or generated ScrollView whose descendant
    surface uses a partially transparent background such as `#252a36cc`.
    Composite the focus underlay through that alpha background with the same
    render order in every affected runtime; do not add an example workaround.

  **Shared batch boundary:** element render-state resolution through local
  text/overlay geometry, mouse hit geometry, reusable/generated ScrollView
  parent-background propagation, alpha compositing, and the resulting
  terminal/MCP cells. Compiler syntax, application domain logic, layout
  constants, delays, snapshots, and unrelated focus state transitions are out
  of scope. If ownership analysis shows that an issue does not share this
  boundary, retain its selected order but split its implementation and focused
  validation before the one final batch gate.

  **Validation policy:** add and first run a failing focused regression for
  each issue; run the affected port tests after each repair; use MCP compares
  with `--compare-app-size 90x35` for externally visible cells and direct PTY
  input where physical mouse routing differs from MCP; regenerate/build every
  affected output; compile and run retained `0.5.3` generated applications
  against the updated runtimes without editing or regeneration; then run one
  final `./tools/test_all.sh --live-report --keep-going` if the completed port
  audit confirms the shared rendering blast radius is broad.

  **Issue #7 shared contract:** for a single-line TextInput with no horizontal
  overflow and a zero scroll offset, resolve `left`, `center`, or `right` from
  the effective state style and use one visual-cell alignment offset for text,
  selection, cursor, and pointer-to-cursor mapping. Once content overflows or
  scrolling is active, the alignment offset is zero and the existing scroll
  contract remains unchanged. TextArea wrapping is out of scope.

  **Issue #7 port parity matrix (recorded before implementation):**

  | Port or layer | Reference path/function | Current behavior | Required action | Focused validation |
  | --- | --- | --- | --- | --- |
  | Python reference | `src/uimd/runtime/elements.py::TextInput::{render_cells,cursor_position_from_point}` | Style parsing exists, but text, cursor, selection, and pointer mapping always start at column zero. | **Fix:** add the shared single-line visual alignment offset and preserve overflow scrolling. | Focused `python/tests/test_elements.py` alignment/render/pointer regression. |
  | C++ runtime | `cpp/src/elements/BasicElements.cpp::TextInput::{render,cursorForPoint}` | Same defect as Python; `Style::textAlign` is already populated. | **Fix:** port the shared visual-cell offset for render and pointer mapping. | Focused `ui_cpp_tests` TextInput regression. |
  | C# runtime | `csharp/src/Uimd/Runtime/Elements.cs::TextInput::{Render,CursorForPoint}` | Same defect. | **Fix:** port the same offset and Unicode visual-cell mapping. | Focused C# runtime console regression. |
  | Swift runtime | `swift/src/Uimd/Sources/Uimd/Uimd.swift::TextInput::{render,cursorForPoint}` | Same defect; the current implementation uses character columns. | **Fix:** preserve the existing Swift character model while applying the identical nonoverflow alignment rule. | Swift package runtime test. |
  | Go runtime | `go/src/uimd/elements.go::TextInput.Render` and `go/src/uimd/runtime.go::textInputCursorAtPoint` | Same defect. | **Fix:** port the alignment offset to render and hit mapping. | Focused Go runtime test. |
  | Rust runtime | `rust/src/uimd/src/elements.rs::{render_text_input,TextInputState::cursor_for_point}` | Same defect. | **Fix:** use the existing `aligned_col` helper for the same nonoverflow rule. | Focused Rust element/runtime test. |
  | Java runtime | `java/src/main/java/uimd/TextInput.java::{renderSingleLine,cursorForPoint}` | Same defect. | **Fix:** port the same visual-cell offset and pointer mapping. | Focused `ControlsTest` regression plus Checkstyle. |
  | Generator/generated API | Native style emitters and existing generated `text-align` properties | The property is already parsed/emitted; no public API or generated source change is needed. | **Unaffected:** do not edit emitters or generated examples. | Existing generator gates in the final run. |
  | Terminal/MCP adapters | Shared mouse coordinates and rendered snapshot paths | They already forward local coordinates and runtime cells. | **Unaffected:** runtime element geometry is the canonical fix. | Focused runtime tests plus affected MCP compare if an existing example exercises it. |

  **Issue #12 shared contract:** an open ComboBox renders the closed row plus
  every option row that fits the final overlay surface; the same full rendered
  rectangle must win hit testing above siblings. A click on visible local row
  `N + 1` selects option `N`, dispatches one change notification when needed,
  commits, and leaves edit mode. Do not introduce an unrendered scroll model or
  retain an independent six-row hit cap.

  **Issue #12 port parity matrix (recorded before implementation):**

  | Port or layer | Reference path/function | Current behavior | Required action | Focused validation |
  | --- | --- | --- | --- | --- |
  | Python reference | `src/uimd/runtime/{elements.py::ComboBox.render_cells,UIBase.py::_element_mouse_rect}` | Rendering and hit testing both use `1 + len(options)`. | **Verify:** preserve as semantic reference. | Focused Python mouse regression with an option below row six. |
  | C++ runtime | `cpp/src/generated/GeneratedWindowRuntime.cpp::{renderEntry,mouseTargetElement,overlayFocusedComboBox}` | Final overlay renders every option, but inline render and mouse target height are capped by `kComboBoxDropdownRows = 6`. | **Fix:** derive both paths from the full overlay row count and remove the independent cap. | Focused stdio-MCP/runtime mouse regression. |
  | C# runtime | `csharp/src/Uimd/Runtime/GeneratedWindow.cs::{ComboBoxDropDownFrame,ToolMousePressAt}` | Already uses the full option count for overlay hit testing. | **Verify:** add/extend lower-option regression. | C# runtime console/MCP regression. |
  | Swift runtime | `swift/src/Uimd/Sources/Uimd/Uimd.swift::{comboBoxDropdownFrame,overlayFocusedComboBox}` | Overlay renders all options but hit frame is capped at six rows. | **Fix:** use full rendered row count. | Swift package mouse regression. |
  | Go runtime | `go/src/uimd/runtime.go::{comboBoxMouseFrame,handleOpenComboBoxMousePress}` | Already uses the full option count. | **Verify:** focused Go runtime mouse regression. | `go -C go/src/uimd test` focused by name. |
  | Rust runtime | `rust/src/uimd/src/mcp.rs::activate_combo_box_option_at` and runtime overlay | Already accepts every visible option row through `options.len()`. | **Verify:** extend the existing ComboBox mouse-scope regression below row six. | Focused Rust MCP test. |
  | Java runtime | `java/src/main/java/uimd/GeneratedWindowRuntime.java::{mouseTargetElement,renderEntry}` | Same six-row cap as C++ while the final overlay renders all options. | **Fix:** use the full rendered row count. | Focused Java generated-runtime mouse regression plus Checkstyle. |
  | Generator/generated API | Existing ComboBox member/options emission | No syntax or API defect. | **Unaffected.** | Existing generator gates. |
  | Terminal/MCP adapters | SGR/MCP pointer delivery | Coordinates are delivered correctly; divergence begins in runtime hit geometry. | **Unaffected:** cover shared runtime path through MCP and direct PTY only if routing differs. | Focused runtime/MCP tests. |

  **Issue #11 shared contract:** when a directly focused reusable host or
  generated ScrollView supplies focus background `F`, a descendant background
  `D` with partial alpha is rendered as `D over (F over P)`, where `P` is its
  structural parent background. Opaque descendant behavior remains the
  established focus overlay. Descendant-only edit focus must not reapply a
  wrapper's partial focus style over the edited child. The focus underlay is
  resolved during canonical parent-background propagation, not by app code or
  by guessing from final terminal colors.

  **Issue #11 port parity matrix (recorded before implementation):**

  | Port or layer | Reference path/function | Current behavior | Required action | Focused validation |
  | --- | --- | --- | --- | --- |
  | Python reference | `src/uimd/runtime/UIScrollView.py::{_apply_self_focus_to_viewport,_focused_descendant_background_map}` plus element parent backgrounds | Partially transparent descendant styles are excluded from the candidate map, and partial proxy focus exits before affecting content. | **Fix:** propagate a direct focus underlay through effective descendant parent backgrounds while preserving descendant-edit suppression. | Focused Python generated/reusable ScrollView cell-color regression. |
  | C++ runtime | `cpp/src/generated/GeneratedWindowRuntime.cpp::{renderEntry,reusableFocusStyleAppliesToChild,collectDescendantBaseStyleBackgrounds}` | Parent background is available during render, but direct partial wrapper focus is rejected for generated ScrollViews and the later collector accepts only opaque backgrounds. | **Fix:** allow direct-host partial focus, pass the focus-adjusted structural parent into descendant rendering, and retain the opaque post-pass for existing behavior. | Focused `ui_cpp_tests` exact RGB regression for unfocused/focused/edited states. |
  | C# runtime | `csharp/src/Uimd/Runtime/GeneratedWindow.cs::{RenderEntry,ReusableFocusStyleAppliesToChild,CollectDescendantBaseStyleBackgrounds}` | Structurally the same gap as C++. | **Fix:** port the same render-time underlay and suppression rule. | Focused C# runtime regression. |
  | Swift runtime | `swift/src/Uimd/Sources/Uimd/Uimd.swift::{renderGeneratedContent,collectDescendantBaseStyleBackgrounds}` | It records partial style tokens but final rendered cells already contain parent-composited colors, so the post-pass cannot apply the underlay reliably. | **Fix:** feed the focus-adjusted parent into the existing `ElementRenderState.parentBackground` path and retain direct-versus-descendant suppression. | Swift package exact-color regression. |
  | Go runtime | `go/src/uimd/elements.go::ReusableElement.Render` and descendant-background helpers | Opaque post-processing exists; partial generated-ScrollView wrapper focus is suppressed and child rows render over the unfocused parent. | **Fix:** render the generated child with the focused structural underlay for direct host/ScrollView focus, preserve descendant-edit suppression, and keep opaque protection logic. | Focused Go exact-color regression. |
  | Rust runtime | `rust/src/uimd/src/runtime.rs::{render_entry,collect_descendant_base_style_backgrounds,apply_reusable_focus_to_descendant_backgrounds}` | Same opaque-only post-pass gap; render-time parent background is available. | **Fix:** propagate the focus-adjusted parent before partial style resolution and preserve the existing opaque pass. | Focused Rust runtime exact-color regression. |
  | Java runtime | `java/src/main/java/uimd/{ReusableElement,GeneratedWindowRuntime}.java` | Same opaque-only collector and generated-ScrollView partial-focus suppression. | **Fix:** port the same direct underlay and descendant-edit guard. | Focused Java reusable/ScrollView regression plus Checkstyle. |
  | Generator/generated API | Existing reusable/generated ScrollView styles | No emitted API or `.uimd` syntax change. | **Unaffected:** use runtime state/style already generated. | Existing generator gates. |
  | Terminal/MCP adapters | Snapshot/terminal cell serialization | They serialize the resolved runtime colors and do not own compositing. | **Unaffected:** validate through focused exact-cell tests and a compare at `90x35`. | Shared focused compare and final terminal gates. |

  **Issue #11 focused-debug checkpoint (2026-08-27):** the first exact
  Python/C++ ImageBrowser compare reached direct focus at step 22 and exposed
  that Python treated a generated ScrollView's structural `@panel` background
  as protected descendant content, so the child focus surface did not replace
  the reusable host underlay. After that repair, the compare reached step 135
  and exposed a second, related case: direct focus of `main.gallery_scroll`
  marked both the reusable proxy and the generated ScrollView focused, causing
  the same partial focus background to be alpha-composited twice in Python
  (`#292c36`) while C++ applied it once (`#171a25`). The canonical Python
  propagation now distinguishes structural viewport color from the effective
  parent underlay, recognizes when the proxy's identical partial focus has
  already been applied, and still allows a genuinely distinct child focus
  style to override it. The regression uses a real generated-style
  `UIScrollView`, marks both proxy and child focus as the runtime does, and
  proves shared partial-focus deduplication plus distinct opaque child-focus
  precedence. The three focused Python #11 regressions pass, and the complete
  Python suite passes 525/525 outside the filesystem/network sandbox. Exact
  rebuilt MCP comparisons and the final cross-platform gate remain pending.
  A subsequent full rebuild showed that the earlier step-135 comparison had
  still used the pre-change C++ runtime: the newly rebuilt C++ underlaid the
  proxy focus correctly but then reapplied the identical generated-ScrollView
  self-focus to structural gap cells, yielding two layers (`#292c36`) against
  Python's single layer (`#171a25`). The C++ fixture now includes the same
  focus background on both the proxy and the inner ScrollView and separately
  asserts an alpha descendant row and an empty structural row; it reproduced
  the double layer before the fix. The scoped C++ underlay now temporarily
  suppresses only an identical inner focus background after that background
  has already been applied, preserving all other focus-style fields and every
  distinct child focus. The focused `ui_cpp_tests` binary passes with this
  stricter regression. The final full rebuild completed, but the exact
  Python/C++ ImageBrowser rerun still failed at step 135 with the opposite
  remaining transition: after `Right` enters `main.gallery_scroll`, C++ keeps
  the unfocused structural background `#030712` while Python applies the
  single focused layer `#171a25` at row 2 column 31. The run passed 150
  assertions with no assertion failures before this one snapshot-step
  failure. Snapshot:
  `tests/mcp/snapshots/20260827-103041-step-135-image_browser_compare.json`;
  viewer:
  `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-103041-step-135-image_browser_compare.json`.
  Next action is to audit the C++ post-key proxy/inner focus transition against
  Python before changing any further compositing rule; remaining exact
  compares, compatibility rerun, and the final full gate stay pending.

  **Issue #11 Go focused-debug checkpoint (2026-08-27):** after the final C++
  structural-background repair, the complete Python/C++ ImageBrowser,
  MarkdownViewer, and ExpenseTracker compares pass. The exact C++/Go
  ImageBrowser compare now fails at step 22 immediately after
  `focus_element("main.items")`: Go keeps one blank descendant cell at the
  structural `#030712` background while C++ applies the direct reusable /
  generated ScrollView focus surface `#1e3a5f`. The run reached 57 passing
  assertions before the snapshot mismatch. Snapshot:
  `tests/mcp/snapshots/20260827-112045-353007-step-022-image_browser_compare.json`;
  viewer:
  `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-112045-353007-step-022-image_browser_compare.json`.
  Audit Go's render-time parent underlay and structural-background remap
  against the final Python/C++ contract, add an exact-color Go regression,
  and rerun this complete compare before continuing the remaining port gates.
  The repaired Go path passes the complete C++/Go ImageBrowser compare with
  378/378 assertions and zero step failures. The next focused
  C++/Go MarkdownViewer compare reaches step 40 after 65 passing assertions,
  then entering edit mode on `viewer` leaves a structural gap at `#000000` in
  Go while C++ applies the one expected focus layer `#141414`. Snapshot:
  `tests/mcp/snapshots/20260827-114017-504102-step-040-markdown_viewer.json`;
  viewer:
  `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-114017-504102-step-040-markdown_viewer.json`.
  Audit the generated-ScrollView edit-state structural-gap branch without
  changing document-content backgrounds, then rerun both focused compares.
  Root cause was Go `ScrollView.Render` replacing a dynamic child row instead
  of inheriting the effective ScrollView row style as C++
  `inheritRowStyle` does. Transparent generated child content therefore
  discarded the already resolved focus surface. Go now composites each child
  background over the effective ScrollView background and inherits only a
  missing/transparent foreground. The expanded exact-color regression, the
  complete Go package, and rebuilt C++/Go compares now pass: ImageBrowser
  378/378, MarkdownViewer 75/75, and ExpenseTracker 268/268 assertions/steps,
  all with zero failures.

  **Issue #11 Swift focused-debug checkpoint (2026-08-27):** the first rebuilt
  C++/Swift ImageBrowser compare reaches step 132 after 148 passing
  assertions, then Swift paints row 2 column 31 as `#292c35` while C++ keeps
  the not-yet-focused structural `#030712` background. The color corresponds
  to two premature partial focus layers. Snapshot:
  `tests/mcp/snapshots/20260827-115039-step-132-image_browser_compare.json`;
  viewer:
  `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-115039-step-132-image_browser_compare.json`.
  Audit the Swift reusable proxy versus inner generated-ScrollView focus flags
  and identical-partial-focus deduplication before proceeding to the other
  Swift compares. Root cause was Swift optional identity comparison treating
  two absent references as identical: a dynamic reusable item with neither a
  generated ScrollView nor current focus therefore received direct focus
  styling. The runtime now requires a non-absent focused element before the
  identity check, applies the same transient structural-background underlay
  and identical inner-focus suppression as Python/C++, and restores the
  generated ScrollView styles after rendering. The retained regression covers
  the unfocused dynamic-item case plus alpha descendants, opaque structural
  descendants, blank structural gaps, and identical partial-focus
  deduplication. Swift package tests pass 16/16. Rebuilt exact C++/Swift MCP
  comparisons pass ImageBrowser 192/192, MarkdownViewer 26/26, and
  ExpenseTracker 142/142 assertions, all with zero assertion and step
  failures. The original step-132 reproduction is therefore resolved.
  `./tools/rebuild_all.sh` subsequently regenerated and rebuilt every
  supported platform and refreshed `.uimd/build-manifest.json` successfully.
  The retained, unmodified 0.5.3 applications and generated outputs then
  compiled and ran against the updated Python, C++, C#, Go, Java, Rust, and
  Swift runtimes; `python3 tools/previous_version_compatibility.py` passed
  7/7 checks. Post-rebuild reruns of the original ImageBrowser reproduction
  pass Python/C++ 192/192, C++/Go 378/378, and C++/Swift 192/192 assertions,
  all with zero assertion and step failures. The consolidated
  supported-platform gate remains pending.

  **Consolidated full-gate checkpoint (2026-08-27):**
  `./tools/test_all.sh --live-report --keep-going --no-rebuild` completed its
  full keep-going inventory against the validated parity manifest. It passed
  23/27 executed phases; all seven preceding-version compatibility checks,
  every unit/lint gate, all direct-terminal and MCP transport smokes, the
  Python/C++ and C++/Go all-example comparisons, and every regression parity
  comparison passed. Full log:
  `.uimd/test-logs/test-all-20260827-123200-327647.log`.

  The remaining failures are confined to issue #11 rendering parity. C#,
  Java, and Rust each passed 1644/1647 all-example assertions/steps and have
  the same three identical-partial-focus double-composition mismatches:

  - ImageBrowser step 135 expected one focus layer `#171a25` but rendered
    `#292c36`. Snapshot viewers:
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-130617-306402-step-135-image_browser_compare.json`
    (C#),
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-144340-716410-step-135-image_browser_compare.json`
    (Java), and
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-152003-654112-step-135-image_browser_compare.json`
    (Rust).
  - MarkdownViewer step 40 expected one focus layer `#141414` but rendered
    `#262626`. Snapshot viewers:
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-130840-782270-step-040-markdown_viewer.json`
    (C#),
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-144546-262366-step-040-markdown_viewer.json`
    (Java), and
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-152327-418903-step-040-markdown_viewer.json`
    (Rust).
  - ExpenseTracker step 17 expected one focus layer `#293143` but rendered
    `#3a4152`. Snapshot viewers:
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-132813-479796-step-017-expense_tracker_compare.json`
    (C#),
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-150425-866550-step-017-expense_tracker_compare.json`
    (Java), and
    `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-154448-324269-step-017-expense_tracker_compare.json`
    (Rust).

  Swift passed 1703/1704 all-example assertions/steps. Its only remaining
  mismatch is TaskBoard step 38 with a modal over the focused reusable /
  ScrollView surface: expected the dimmed canonical base `#010309`, but Swift
  retained a dimmed focus-underlay result `#03080f`. Viewer:
  `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-135628-857584-step-038-task_board_compare.json`.
  Next action is to port the final Python/C++ identical-inner-focus
  suppression structurally to C#, Java, and Rust, and audit Swift's covered
  modal-frame suppression before changing its compositor. No example or
  snapshot change is permitted.

  **Post-full-gate repair checkpoint (2026-08-27):** the remaining four
  failures were reproduced by retained exact-color regressions and repaired in
  the shared runtimes without changing examples or snapshots. C#, Java, and
  Rust now match the final C++ scoped generated-ScrollView underlay algorithm:
  an identical inner partial focus background is suppressed only while the
  reusable focus underlay is already active, blank or structural cells are
  remapped to exactly one focused structural background, and every transient
  style is restored after rendering. Swift now matches C++ modal-background
  rendering by suppressing the direct focused state of a covered reusable or
  ScrollView entry before the modal backdrop is applied. Focused validation
  passes C# 5/5, the forced Java #11 regression plus the complete Gradle
  `check`/Checkstyle gate, Rust 164/164 plus Clippy with `-D warnings`, and
  Swift 17/17. The first sandboxed Rust suite attempt correctly exposed four
  localhost permission failures; the same complete suite passed outside that
  restriction. Full regeneration/build, the exact affected MCP comparisons,
  the retained 0.5.3 compatibility gate, and one final consolidated full test
  remain pending.

  **Final focused-parity checkpoint (2026-08-27):** exact post-repair C#
  comparisons pass MarkdownViewer 75/75 and ExpenseTracker 268/268; the
  rebuilt ImageBrowser run produced no failure snapshot and is covered again
  by the pending complete all-example gate. Exact Java comparisons pass
  ImageBrowser 378/378, MarkdownViewer 75/75, and ExpenseTracker 268/268.
  Rust's first repaired ImageBrowser rerun exposed one additional canonical
  precedence case at step 22: a genuinely distinct generated-ScrollView focus
  background (`#1e3a5f`) must replace the reusable proxy's partial focus
  background (`#171a25`) rather than inherit it. Snapshot viewer:
  `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-170600-925287-step-022-image_browser_compare.json`.
  The Rust runtime now selects the effective child focus for that direct
  generated ScrollView, suppresses only an identical already-applied focus,
  and retains the shared structural-background remap. Its exact comparisons
  now pass ImageBrowser 378/378, MarkdownViewer 75/75, and ExpenseTracker
  268/268; the focused exact-color regression and Clippy with `-D warnings`
  also pass.

  Swift's first exact TaskBoard rerun confirmed the original modal-covered
  failure at step 38: the reusable/ViewHost path still propagated an inherited
  active-ScrollView focus background after direct focused-state suppression.
  Snapshot viewer:
  `python3 tools/mcp_snapshot_viewer.py tests/mcp/snapshots/20260827-173446-step-38-task_board_compare.json`.
  ViewHost, ReusableElement, and generated-ScrollView proxy focus now all honor
  the same covered-frame scope-visual suppression before deriving the child
  focus underlay. The strengthened regression first proves the opaque focused
  surface, then opens the modal and proves the dimmed canonical base. Swift
  package tests pass 17/17 and the rebuilt exact C++/Swift TaskBoard scenario
  passes 190/190 assertions with zero step failures.

  A second `./tools/rebuild_all.sh` after these last Rust/Swift changes
  regenerated and built every supported target and rewrote
  `.uimd/build-manifest.json` successfully. The retained, unmodified 0.5.3
  fixture applications and generated outputs compile and run against the
  updated Python, C++, C#, Go, Java, Rust, and Swift runtimes;
  `python3 tools/previous_version_compatibility.py` passes 7/7. The only
  remaining completion gate is one post-last-edit
  `./tools/test_all.sh --live-report --keep-going --no-rebuild` run and its
  final log inspection.

  **User-requested validation pause (2026-08-27):** the final post-last-edit
  run at `.uimd/test-logs/test-all-20260827-175111-820315.log` passed the
  rebuilt-tool and manifest gates, previous-version compatibility 7/7, Python
  525/525, CTest 26/26, C# 5/5, Go, Java/Checkstyle, Rust 164/164 plus Clippy,
  Swift 17/17, Swift direct terminal 35/35, Go direct terminal 10/10, Java
  direct terminal 12/12, Java MCP transport 5/5, Rust direct terminal 8/8,
  Rust MCP transport 5/5, and the complete Python/C++ example comparison
  1044/1044. The user then required work to stop within ten minutes while the
  C# all-example phase was still running. Its 65/65 completed assertions/steps
  were green before interruption. Because `--keep-going` treated each
  Ctrl+C as a phase failure and started the next platform, the log contains
  `KeyboardInterrupt` failure lines for C#, Swift, Go, and Java; these are
  intentional interruption artifacts, not observed product mismatches. The
  complete runner was then terminated before the Rust all-example phase
  finished, and no test runner remains. On resume, rerun the same full command
  from the beginning and inspect its final recap before marking this batch or
  issues #7/#12/#11 complete.

  **Completed on 2026-08-28.** A fresh uninterrupted post-last-edit
  `./tools/test_all.sh --live-report --keep-going --no-rebuild` run passed all
  27/27 executed phases with exit code 0; its nine skipped phases were exactly
  the regeneration/build steps already completed by the immediately preceding
  successful `./tools/rebuild_all.sh`. The final gate passed the parity
  manifest, preceding-version compatibility 7/7, Python 525/525, CTest 26/26,
  C# 5/5, Go, Java/Checkstyle, Rust 164/164 plus Clippy, Swift 17/17, every
  direct-terminal and MCP transport smoke, Python/C++ examples 1044/1044, and
  C#/Swift/Go/Java/Rust examples 1980/1980 each. Base regression parity passed
  14/14 and Go/Java/Rust regression parity passed 29/29 each. Full log:
  `.uimd/test-logs/test-all-20260827-202640-516750.log`.

  Final parity decision: all supported runtimes implement the same TextInput
  alignment/point-mapping rule, full rendered ComboBox dropdown hit geometry,
  and reusable/generated ScrollView focus-underlay, distinct-focus precedence,
  and covered-modal suppression behavior. No `.uimd` example, snapshot,
  timing workaround, public API, generated application contract, or
  platform-specific visible behavior was changed. The retained unmodified
  0.5.3 generated applications compile and run against every updated runtime,
  so the fixes preserve documented existing-application compatibility.

  Commit preparation on 2026-08-28 raised the patch version to `0.5.4` and
  exposed that Cargo rewrote the frozen Rust fixture lockfile when resolving
  the updated path dependency. The compatibility runner now builds Rust from
  a complete temporary fixture copy and rewrites only that copy's equivalent
  runtime path, while Swift uses a temporary writable module cache. Focused
  runner tests pass 17/17, the isolated Rust gate passes without changing the
  fixture manifest, all version/report tests pass, and the final unsandboxed
  preceding-version compatibility run passes Python, C++, C#, Go, Java, Rust,
  and Swift 7/7 against `0.5.4`.

- [x] **Guarantee backward compatibility for existing documented UIMD
  applications.** Requested on 2026-08-27. Add a strict `AGENTS.md` rule and
  mandatory development/bug-fix workflow gates requiring an application and
  previously generated outputs from the preceding version to compile and run
  against the updated library without source edits or regeneration. Ordinary
  patch updates must preserve documented public APIs, generated inheritance
  hooks, callbacks, option construction, imports, schemas, and runtime
  behavior except for the explicitly corrected defect. Any unavoidable
  breaking change requires explicit user approval before implementation.
  Parity decision: process and compatibility-test policy only; no compiler,
  runtime, generated API, or application behavior changes in this task.
  Completed on 2026-08-27. `AGENTS.md` now makes compile-and-run compatibility
  of documented existing applications a strict patch-release rule, including
  previously generated sources and language-specific option construction.
  Both the cross-platform bug-fix and new-language/platform workflows now
  require retained immediately-preceding-version fixtures to compile and run
  against the updated library without source edits or regeneration, document
  the exact aggregate command, and stop for explicit approval plus a migration
  and non-patch version plan if the gate fails. Validation confirmed all three
  mandatory rule/gate anchors are present and `git diff --check` passes.

- [x] **Fix GitHub issues #8 and #9 as one ScrollView confirm/rebuild focus
  batch.** Selected by the user on 2026-08-23. Both reports cross the same
  runtime boundary: Enter confirms an editable descendant of an active
  ScrollView, the owner-aware submit callback may rebuild dynamic ScrollView
  children, and the runtime must reconcile the active edit target and logical
  focus before rendering.

  - [x] [#8: C++ ScrollView Enter confirm ignores `keepEditModeAfterConfirm`](https://github.com/uimd-lang/uimd/issues/8).
    Reproduce Enter on a ScrollView-scoped TextInput with the option enabled;
    the same live logical element must remain the active edit target with a
    fresh edit snapshot after its confirm callback completes.
  - [x] [#9: C++ focus moves to adjacent controls after scrollview row content is rebuilt from TextInput submit](https://github.com/uimd-lang/uimd/issues/9).
    Reproduce a submit callback that replaces dynamic ScrollView rows while
    the submitted TextInput remains present; focus must stay on that logical
    input rather than following a stale numeric focus index to a neighbour.

  **Shared batch boundary:** key dispatch from the resolved active ScrollView
  edit target through text confirmation, owner-aware callback mutation,
  liveness/stable-identity reconciliation, edit snapshot refresh, remembered
  ScrollView descendant restoration, and post-event normalization. Do not
  include issue #14's public identity-aware key callback API because it is a
  separate backward-compatible API design task.

  **Validation policy:** retain one deterministic regression for each issue;
  run the affected port's focused test after each repair; run cross-language
  MCP comparison at `--compare-app-size 90x35` when the state is externally
  observable; rebuild the affected examples and regressions; then consolidate
  the expensive complete supported-platform gate into one final run after the
  whole batch is green.

  **Shared confirm/rebuild contract (recorded before implementation):** Enter
  first applies the edited control's local key handling and value-change
  notification, then commits the control and clears the old child-edit
  snapshot/target before invoking the owner-aware confirmed callback. The
  runtime captures the submitted element's live identity before that callback.
  After the callback may mutate the ScrollView contents, it recomputes the
  active focusable collection and restores the numeric focus/index only to the
  same still-live, enabled, focusable element. It must not reuse the old index
  or match a destroyed/recreated element by an ambiguous local name. If
  `keepEditModeAfterConfirm` is enabled and that same element remains live,
  the runtime captures a fresh snapshot, begins a new edit session, restores
  it as the active scoped edit target, and emits `onEditStarted`; otherwise it
  remains only in ScrollView navigation scope. All of this completes before a
  render, MCP response, or terminal frame can inspect the state.

  **Port parity matrix (recorded before implementation):**

  | Port or layer | Reference path/function | Current behavior | Required action | Focused validation |
  | --- | --- | --- | --- | --- |
  | Python reference | `src/uimd/runtime/UIBase.py::{_handle_scrollview_scope_key,_end_scoped_child_edit}` | Ends/commits the child edit before dispatching confirmation and retains focus by object reference. It has no generated-runtime `keepEditModeAfterConfirm` option. | **Verify #9 / unaffected #8:** preserve the object-reference focus contract and add a callback-mutation regression; do not invent a Python-only option surface. | Focused `python/tests/test_application.py` test. |
  | C++ runtime | `cpp/src/generated/GeneratedWindowRuntime.cpp::{handleStackFrameKey,McpController::toolPressKey,runGeneratedWindow}` plus `focusableElements` | All three scoped-confirm routes commit but clear the edit target unconditionally; after a callback mutates children they retain a stale numeric focus index. Stack-frame options also omit the keep flag. | **Fix #8 and #9:** use one identity-checked post-callback reconciliation algorithm in terminal, MCP, and modal-frame routes; add the missing additive frame option. | Focused `ui_cpp_tests` root and stack-backed stdio-MCP regressions. |
  | C# runtime | `csharp/src/Uimd/Runtime/GeneratedWindow.cs::McpController.ToolPressKey` and `RuntimeFrame.FocusedElementRef` | Stable object focus already rebases the index, but scoped confirmation ignores the keep flag and invokes the callback before commit. | **Fix #8 / verify #9:** commit before callback, re-resolve the retained object through the current focusable collection, and start a fresh scoped edit only when requested. | Retained C# runtime console regression plus existing stale-focus compare. |
  | Swift runtime | `swift/src/Uimd/Sources/Uimd/Uimd.swift::GeneratedRuntimeController.handleActiveScrollViewKey` and `focusedName` | Stable full runtime IDs already survive sibling rebuilds; this port does not expose the C++ keep-after-confirm option, but it invokes confirmation before commit. | **Verify #9 / fix shared ordering / unaffected #8:** commit and clear the old child edit before callback, then keep the same represented full ID in navigation scope. | Swift package test plus existing stale-focus compare. |
  | Go runtime | `go/src/uimd/runtime.go::{handleReusableScrollScopeKey,endElementEdit}` and `focusedOverride` | Commit already precedes callback and stable object focus survives sibling changes, but scoped confirmation always clears the edit target even when the keep flag is enabled. Frame options already carry both keep flags. | **Fix #8 / verify #9:** re-resolve the focused live object after callback and begin a fresh scoped edit when requested. | Focused `go -C go/src/uimd test` regression. |
  | Rust runtime | `rust/src/uimd/src/runtime.rs::{RuntimeState::handle_key,dispatch_confirm,repair_live_interaction_with_ids}` | Confirmation already commits first and repairs interaction by full runtime ID, but the scoped branch clears the repaired edit target and ignores the keep flag. Modal frame options omit both keep flags. | **Fix #8 / verify #9:** use the repaired live focused element for fresh scoped edit, preserve navigation-only state otherwise, and add the frame-option mapping. | Focused Rust runtime test plus existing stale-focus compare. |
  | Java runtime | `java/src/main/java/uimd/GeneratedWindowRuntime.java::{handleEditKey,normalizeState}` | Scoped confirmation ignores the keep flag, invokes callback before commit, and retains only a stale numeric focus index after the focusable set changes. | **Fix #8 and #9:** port the identity-checked commit/callback/reindex/re-enter sequence. | Focused `GeneratedWindowFocusTest` regressions. |
  | Generator/generated API | Native emitters and representative generated classes | The root runtime keep option is hand-written and already exposed by affected generated targets; no `.uimd` layout or event-hook emission is involved. | **Unaffected:** keep emitters/generated outputs unchanged; only additive runtime frame-option parity is required where currently missing. | Existing generator/API gates in the final full run. |
  | Terminal/MCP adapters | Per-port key parsers and MCP transports | Enter is already delivered correctly; the defect begins after active scoped edit resolution. | **Unaffected:** do not change transport, key parsing, delays, examples, or snapshots. | Focused runtime/MCP regressions and final direct-terminal/MCP gates. |

  **Completed on 2026-08-23.** C++, C#, Go, Rust, and Java now use the same
  identity-checked scoped-confirm sequence: commit and clear the old edit
  session before the callback, recompute current focusables after callback
  mutation, retain only the same live object, rebase its numeric index, and
  create a fresh edit snapshot/session only when
  `keepEditModeAfterConfirm` is enabled. Swift now follows the same
  commit-before-callback ordering and retains its stable full runtime ID;
  Python's existing object-reference behavior was preserved and covered by a
  callback-mutation regression. C++ stack frames and Rust modal frames gained
  their missing additive keep-option mapping. No example, `.uimd` source,
  snapshot, transport, delay, or generator workaround was introduced.

  Focused validation passed: C++ `ui_cpp_tests` covers root and stack-backed
  stdio MCP with both keep-option branches and proves the pre-fix assertions;
  the new C# console runtime suite passed 2/2 and is a first-class
  `tools/test_all.sh` phase; Python's focused application regression plus the
  report tests passed 15 tests; Go's focused runtime regression plus the full
  package and `vet` passed; Java's focused
  `GeneratedWindowFocusTest.scopedConfirmRetainsLiveInputAndRebasesFocusAfterMutation`
  plus the complete Gradle `check` passed; Rust's focused
  `runtime::tests::scoped_confirm_retains_live_input_and_rebases_focus_after_mutation`
  plus clippy passed; and Swift's complete package suite passed 12/12.
  C#/Swift do not own standalone `stale_scrollview_focus` regression apps, so
  their shared behavior is covered by the C# console regression and Swift
  package tests rather than an invented duplicate app.

  The focused parity corpus passed Python/C++ 14/14, C++/Go stale-focus
  25/25, C++/Rust stale-focus 25/25, and C++/Java stale-focus 25/25 at
  `--compare-app-size 90x35`. `./tools/rebuild_all.sh` regenerated and built
  every supported platform and refreshed `.uimd/build-manifest.json`; that
  required rebuild also synchronized all tracked Rust lockfiles from the
  previous `0.4.26` dependency reference to the current `0.5.1` runtime.
  The consolidated final command
  `./tools/test_all.sh --live-report --keep-going` passed all 35/35 phases:
  Python 519/519, CTest 26/26, C# 2/2, Rust 162/162, Swift 12/12, every build,
  lint, direct-terminal and MCP transport gate, Python/C++ examples 1044/1044,
  C#/Swift/Go/Java/Rust examples 1980/1980 each, base regressions 14/14, and
  Go/Java/Rust regressions 29/29 each. Full log:
  `.uimd/test-logs/test-all-20260823-020429-492797.log`. Final parity decision:
  all affected ports implement the same public behavior and cleanup order;
  Python and Swift retain only their documented existing option-surface
  differences, with no platform-specific behavior exception.

- [x] **Allow related bug fixes to share one expensive full-validation gate.**
  Requested on 2026-08-22 because the complete multi-platform test suite can
  take hours. Added a durable `AGENTS.md` rule and a Related Bug-Fix Batches
  section to `docs/cross-platform-bug-fix-workflow.md`. A batch must share a
  canonical owner, subsystem, state transition, generated surface, or focused
  test set; every bug retains its own reproduction and regression; focused
  validation still runs after each repair; and the expensive full supported-
  platform gate runs once after the complete batch is green. Unrelated or
  diagnostically ambiguous changes must be split. Parity decision:
  documentation/process only; runtime behavior is unaffected.

- [x] **Fix GitHub issues #10 and #13 as one keyboard focus/activation routing
  batch.** Selected by Codex with user authorization on 2026-08-22 because both
  reports traverse the generated runtime's keyboard routing and active
  ScrollView focus scope, so they can share the port audit and one final
  validation gate while retaining separate regressions.

  - [x] [#10: C++ keyboard focus movement does not fire `onFocusChanged`](https://github.com/uimd-lang/uimd/issues/10).
    Reproduce Tab/Shift+Tab/spatial movement versus mouse focus, preserve the
    required previous-focus `false` then next-focus `true` callback order, and
    cover terminal, MCP, modal-frame, and relevant language-port paths.
  - [x] [#13: Enter/Space does not activate a focused reusable control inside
    an active ScrollView](https://github.com/uimd-lang/uimd/issues/13).
    Reproduce activation inside the active scope, route it through the owning
    reusable child exactly once, and cover terminal, MCP, stack-frame, Python
    reference, and every relevant language port.

  **Shared batch boundary:** keyboard dispatch after the active window and
  focused element are resolved, through focus movement or reusable activation,
  owner-aware callback dispatch, ScrollView-scope synchronization, and
  post-event cleanup. Do not include edit-confirm/rebuild issues #8/#9 or
  public identity API issue #14 in this batch because they require different
  state/API design.

  **Validation policy:** add one focused regression per issue and run the
  affected port's focused tests immediately after each repair. After both
  issues and the complete port parity matrix are green, regenerate/build the
  affected examples and run their focused compares/direct-terminal gates.
  Consolidate any required full supported-platform validation into one final
  run for this batch.

  **Intended shared contract and callback order:** a runtime-managed keyboard
  focus change emits the previous logical focused element with `false`, then
  the new logical focused element with `true`, and emits nothing when the
  logical element did not change. This applies to linear and spatial movement
  plus entering, moving within, and leaving a ScrollView focus scope. Enter or
  Space on a reusable element that is the focused descendant of an active
  ScrollView first invokes the same owner-aware reusable-control activation as
  ordinary focus; it runs exactly once and only falls back to entering a
  nested scope or edit session when the reusable child did not consume it.
  State normalization and rendering observe the completed callback/activation
  transition.

  **Issue #10 port parity matrix (recorded before implementation):**

  | Port or layer | Reference path/function | Current behavior | Required action | Focused validation |
  | --- | --- | --- | --- | --- |
  | Python reference | `src/uimd/runtime/UIBase.py::set_focus` | Its public focus hook already runs from the canonical focus transition, but Python has no generated-runtime `onFocusChanged` option matching this issue's callback surface. | **Unaffected:** preserve the semantic focus transition; use it as the event-order reference. | Existing Python focus tests plus the #13 regression below. |
  | C++ runtime | `cpp/src/generated/GeneratedWindowRuntime.cpp::{moveFocus,moveFocusSpatial,enterScrollViewScope,moveScrollViewScopeFocus,exitScrollViewScope,notifyFocusChanged}` | Mouse/explicit MCP focus notifies, but keyboard terminal, MCP, and stack-frame movement omits some or all previous/next callbacks. | **Fix:** notify once around every successful logical keyboard focus transition, before rendering or MCP state capture. | Focused C++ generated-runtime tests and keyboard MCP regression at `90x35`. |
  | C# runtime | `csharp/src/Uimd/Runtime/GeneratedWindow.cs::{MoveFocusLinear,MoveFocusDirection,EnterScrollViewScope,MoveScrollViewScopeFocus,ExitScrollViewScope}` | Linear movement notifies even on a self-wrap; spatial and ScrollView-scope transitions do not notify consistently. | **Fix:** centralize actual-change-only previous-false/next-true notification across every movement path. | Focused C#/C++ keyboard MCP regression at `90x35`. |
  | Swift runtime | `swift/src/Uimd/Sources/Uimd/Uimd.swift::{focusedName,setFocusedElement}` | `focusedName` already emits previous `false` then next `true` only when the name changes, and movement paths use that setter. | **Verify:** preserve implementation and cover it through the shared keyboard regression. | Focused C++/Swift MCP compare at `90x35`. |
  | Go runtime | `go/src/uimd/runtime.go::{notifyFocusTransition,moveFocusLinear,moveFocusSpatial}` | Already captures the previous logical focus and notifies after movement. | **Verify:** preserve implementation and add/extend focused runtime coverage. | `go -C go/src/uimd test` focused by the new test name. |
  | Rust runtime | `rust/src/uimd/src/runtime.rs::handle_key` | The public key wrapper captures previous/next logical focus and dispatches the correct pair. | **Verify:** preserve implementation and add/extend focused runtime coverage. | `cargo test` focused by the new test name. |
  | Java runtime | `java/src/main/java/uimd/GeneratedWindowRuntime.java::changeFocus` | Movement paths use one transition helper which emits previous `false` then next `true`. | **Verify:** preserve implementation and add focused JUnit coverage. | Gradle focused `GeneratedWindowFocusTest`. |
  | Generator/public API | Native emitters and generated runtime options | The callback surface already exists where supported; no emitted layout or API change is required. | **Unaffected:** do not regenerate APIs for a runtime-routing defect. | Existing generator/API checks. |
  | Terminal/MCP adapters | Per-port terminal parsers and MCP transports | They already deliver the relevant key names; the defect occurs after input resolution. | **Unaffected:** keep raw input/transport code unchanged. | Shared MCP keyboard regression plus existing direct-terminal smoke in the final gate. |

  **Issue #13 port parity matrix (recorded before implementation):**

  | Port or layer | Reference path/function | Current behavior | Required action | Focused validation |
  | --- | --- | --- | --- | --- |
  | Python reference | `src/uimd/runtime/UIBase.py::{_handle_scrollview_scope_key,_activate_reusable_element,_activate_element}` | Ordinary activation supports reusable children, but the active ScrollView Enter/Space branch skips `uielement`. | **Fix:** invoke the canonical reusable activation before nested-scope/edit fallback. | New focused `python/tests/test_application.py` regression. |
  | C++ runtime | `cpp/src/generated/GeneratedWindowRuntime.cpp::{focusableElements,activateReusableControl}` and active-scope key branches | Stack-frame routing activates reusable controls, while root terminal and MCP active-scope branches omit activation; active-scope enumeration also drops a direct focusable reusable child when its generated child has no inner focus target. | **Fix:** enumerate that reusable wrapper and use the same activation-first ordering in all three routes. | Focused `ui_cpp_tests` stdio-MCP regression plus MCP compare at `90x35`. |
  | C# runtime | `csharp/src/Uimd/Runtime/GeneratedWindow.cs::{AddFocusableElements,ActivateReusableControl,HandleKey}` | General activation is correct; active ScrollView routing omits it and active-scope enumeration drops the direct reusable wrapper. | **Fix:** enumerate the wrapper and use the same activation-first ordering in the active scope. | Focused C# stdio-MCP probe plus C#/C++ MCP regression at `90x35`. |
  | Swift runtime | `swift/src/Uimd/Sources/Uimd/Uimd.swift::{handleActiveScrollViewKey,performActivateElement}` | Active-scope and general paths both route reusable IDs through the generated button/host callback contract. | **Verify:** preserve the Swift adapter and cover it through the shared regression. | Focused C++/Swift MCP compare at `90x35`. |
  | Go runtime | `go/src/uimd/runtime.go::{activateFocused,activateReusableScrollScopeFocused}` | General activation calls the reusable child; active-scope handling only attempts nested scope entry for reusable/view-host elements. | **Fix:** activate once first, then fall back to nested scope entry. | New focused Go runtime test. |
  | Rust runtime | `rust/src/uimd/src/runtime.rs::{focusable_descendants,activate_focused,activate_scope_focused}` | General activation calls the reusable child; active-scope enumeration and activation both omit a direct focusable reusable wrapper. | **Fix:** enumerate that wrapper and port the same activation-first branch. | New focused Rust runtime test. |
  | Java runtime | `java/src/main/java/uimd/GeneratedWindowRuntime.java::{addFocusableElements,activate,activateReusableControl}` | Active ScrollView Enter/Space delegates to the correct generic activation helper, but its child enumeration bypasses the reusable-wrapper fallback. | **Fix:** recurse through the canonical reusable enumeration helper and add focused JUnit coverage. | Gradle focused `GeneratedWindowFocusTest`. |
  | Generator/public API | Native emitters and generated reusable hooks | Existing generated reusable ownership/activation hooks are sufficient. | **Unaffected:** no emitter or generated source change. | Existing generator/API checks. |
  | Terminal/MCP adapters | Per-port terminal parsers and MCP transports | Enter/Space arrives correctly; the missing branch is shared runtime dispatch. | **Unaffected:** keep raw input/transport code unchanged. | Shared MCP regression and final direct-terminal smoke. |

  **Focused validation completed before the final batch gate:** Python's new
  scoped reusable regression passes; the C++ monolithic runtime test now drives
  headless stdio MCP through scope entry plus Enter and Space and observes
  exactly two activations; an equivalent temporary C# stdio-MCP probe exits 0
  after exactly two activations; focused Go and Rust tests pass; Java's two new
  JUnit regressions and the complete Gradle `check` pass; Swift's existing
  transition path is unchanged and `swift test` passes. The shared
  `markdown_viewer.yaml` keyboard regression passes with 75/75 assertions and
  no failed steps for C++ against C#, Swift, Go, Rust, and Java at `90x35`.

  **Completed on 2026-08-22.** The final full regeneration/build completed
  successfully for every supported target and rewrote the parity artifact
  manifest. The one consolidated final batch gate
  `./tools/test_all.sh --live-report --keep-going` passed all 34/34 phases:
  Python 518/518, CTest 26/26, Rust 161/161, Swift 12/12, every language build,
  lint, direct-terminal and MCP transport phase, Python/C++ 1044/1044 example
  assertions/steps, C#/Swift/Go/Java/Rust 1980/1980 example
  assertions/steps each, and all Python, Go, Java, and Rust regression parity
  phases. Full log:
  `.uimd/test-logs/test-all-20260822-181358-157918.log`. Final parity decision:
  Python, C++, C#, Go, Rust, and Java received the same reusable-scope
  activation/focus-target contract where affected; C++ and C# received the
  missing actual-transition callback routing; Swift's existing transition and
  activation behavior was verified unchanged. No example-specific workaround,
  generated API change, or platform exception was introduced.

- [x] **Document and require the cross-platform bug-fix workflow.** Requested
  on 2026-08-22. Add a durable workflow under `docs/` that defines issue
  intake, canonical-source ownership, reproduction, per-port parity mapping,
  implementation order, focused and full validation, generated-output rules,
  GitHub issue handling, and completion criteria for substantive fixes. Link
  it from `AGENTS.md` with a mandatory trigger for requested bug fixes,
  regressions, failing tests, and selected GitHub issues. Confirm and preserve
  the existing mandatory link to `docs/new-language-platform-workflow.md` for
  new language/platform work. Parity decision: documentation and agent process
  only; no compiler, runtime, generated API, example, or test behavior changes.
  Completed on 2026-08-22. Added the mandatory 305-line workflow at
  `docs/cross-platform-bug-fix-workflow.md`, linked it bidirectionally with the
  new-language workflow, and added normal plus post-compaction triggers to
  `AGENTS.md`. The workflow covers intake authorization, reproduction,
  canonical ownership, a required per-port parity matrix, shared-fix design,
  reference-first implementation, focused validation, blast-radius-aware
  regeneration/build gates, final validation, GitHub/commit handling, stop
  conditions, and completion criteria. Validation: required files and links
  exist, all required workflow sections are present, no trailing whitespace was
  found, and `git diff --check` passes.

- [x] **Triage the currently reported GitHub bugs #6 through #15 against the
  current `sdk-work` sources.** Requested on 2026-08-22. Review each open issue
  read-only, verify its stated runtime/test path against the local canonical
  implementations, identify overlap and likely fix order, and report whether
  each issue is confirmed, related to another report, or already addressed.
  Do not implement fixes, change issue state, or comment on GitHub in this
  triage task. Parity decision: analysis only; no runtime, generator, API,
  example, snapshot, or test behavior is changed.
  Completed on 2026-08-22. All ten issues remain open and have no comments.
  Source audit result: #6 is already implemented on `sdk-work`; #7, #8, #10,
  #11, #12, and #14 are confirmed in current sources; #9 is a confirmed focus
  identity/rebuild symptom closely related to #8; #13 is implemented for stack
  frames but still missing from the main terminal, MCP, and Python ScrollView
  key paths; #15 is strongly supported by the missing previous-raw-image
  cleanup in `TerminalBuffer`, but still requires a direct PTY reproduction.
  Several reports described as C++ defects expose shared Python/C++ behavior or
  public API contracts, so their eventual fixes require the parity gate rather
  than isolated C++ patches.

- [ ] **Implement Java as the next complete generated terminal language target.**
  Approved by the user on 2026-08-13 after the cross-platform Sixel and
  sustained-wheel fixes were validated, committed as `7a4c570`, and pushed to
  `origin/sdk-work`. Use the canonical target spelling `java` consistently in
  CLI flags, generated files, SDK target discovery, documentation, manifests,
  tests, and artifact resolution. This is a complete language-port task, so
  after every context compaction reread `AGENTS.md`, immediately reread this
  `prompts/TODO.md`, and also reread
  `docs/new-language-platform-workflow.md` before answering or continuing.

  **Strict current scope:** implement a terminal-only Java target with the same
  behavior, architecture, public API, generated event hooks, examples, MCP
  support, direct-terminal behavior, Sixel/fallback images, dialogs, and parity
  regressions as Python/C++. The user's long-term motivation includes a future
  Android/graphical renderer and a compile-time choice of terminal, GUI, or
  both, but that explanation is context only. Do not implement Android,
  Compose, Android Views, JavaFX, GUI rendering, a surface/backend compilation
  switch, GUI-specific `.uimd` syntax, or a broad existing-runtime refactor in
  this task. Do not create separate terminal and GUI `.uimd` files. Keep Java
  application/domain logic free of example-specific terminal workarounds so a
  future graphical backend is not needlessly blocked, while preserving the
  established terminal-first architecture and preferred generated
  inheritance/override event API.

  **Canonical ownership and parity decision:** Python under
  `src/uimd/runtime` remains the shared semantic reference; C++ under
  `cpp/include/ui`, `cpp/src`, and `cpp/dialogs` remains the structural/native
  runtime oracle; the C# port under `csharp/src/Uimd` is a useful
  garbage-collected, class-based implementation reference but never a new
  semantic oracle. Java compiler/generator/CLI behavior belongs only in the
  native tool under `cpp/tools/uimd` (for example a canonical native Java
  emitter registered beside the existing emitters), and Java runtime behavior
  belongs in one Java-owned runtime tree under `java/`. Do not implement a Java
  compiler in Java or Python, duplicate runtime behavior, fork example layouts,
  or fix parity problems in examples/tests. Before implementation, record a
  method/state map for terminal setup/input, render buffers, elements, layout,
  focus/edit cleanup, ScrollView/reusable ownership, dialogs/window stack,
  image/Sixel behavior, MCP lifecycle, generator APIs, and SDK packaging.

  **Pre-implementation audit and Java method/state map (2026-08-13):**

  - **Build/toolchain contract:** use the checked-in Gradle Wrapper pinned to
    Gradle 9.7.0, a Gradle Java toolchain fixed at Java 17, UTF-8 compilation,
    strict compiler warnings, JUnit 5, and Checkstyle. The wrapper is the
    canonical macOS/Linux/Windows entry point (`./java/gradlew` or
    `java\\gradlew.bat`), while generated projects resolve the one runtime from
    either the source checkout or `targets/java` in an installed SDK. Java 17
    is the portability baseline; no Android or GUI dependency is introduced.
    The unavoidable JVM/OS adapter is limited to terminal/clipboard/native
    library calls; shared UI state and algorithms stay ordinary Java.
  - **Terminal lifecycle/input:** port Python
    `src/uimd/runtime/application.py` (`UIApplication.run`, `_read_key`, SGR
    parsing, resize/cell-pixel queries, terminal teardown) and C++
    `cpp/src/terminal/{TerminalBackend,Input}.cpp` plus
    `runGeneratedWindow` in
    `cpp/src/generated/GeneratedWindowRuntime.cpp`. Java ownership will be
    `uimd.terminal.TerminalBackend`, `TerminalModeGuard`, and `InputParser`,
    feeding the same `Event` variants. Preserve alternate screen, mouse,
    bracketed paste, modifyOtherKeys, autowrap/cursor restoration, Ctrl+C,
    POSIX/Windows behavior, and the pre-dispatch consecutive-wheel merge
    (`latest position`, clamped summed delta `+/-12`, zero removal, non-wheel
    boundaries unchanged).
  - **Core/render buffers:** port Python `style.py`, `rendering.py` and C++
    `cpp/{include,src}/ui/core` plus `terminal/TerminalBuffer`. Java maps these
    to `Color`/`Rgba`, `Style`/`TextGradient`, `Point`/`Size`/`Rect`,
    `AxisDimension`, `TerminalCell`, and `TerminalBuffer`, retaining current
    and previous cell grids, full-redraw state, raw anchor dimensions/skips,
    changed-run statistics, diff regions, protected Sixel rows, synchronized
    updates, and terminal scroll-region mutation in the same order.
  - **Elements and layout:** port Python `UIBase.py`, `uielement.py`,
    `elements.py`, and `image.py`, checked against C++ `core/{Element,Layout}`
    and every header/source under `cpp/{include,src}/ui/elements`. Java keeps
    the same class split and state roles for `Element`, render state, Label,
    Button, CheckBox, TextInput/TextArea, NumberInput, ComboBox, ListBox,
    InfoLabel, MessageTable, FrameBufferView, Image, and style-state fallback.
    Layout is resolved dynamically from generated relative cells and axis
    dimensions; no Java-only absolute positions, design constants, or caches
    are permitted.
  - **Focus/edit/post-event cleanup:** port Python `UIBase` focus/edit/
    selection transitions and C++ generated-runtime helpers around
    `focusableElements`, edit snapshots, spatial movement, owner-aware event
    dispatch, and cleanup. Java runtime-frame state remains the same:
    `focusedIndex`, `editMode`, `activeScrollView`,
    `activeScrollViewEditElement`, `suppressActiveScrollViewScopeVisuals`,
    `editSnapshot`, and per-ScrollView remembered descendant identities. Every
    key, mouse, MCP, modal, and activation path performs cleanup before render.
  - **ScrollView/reusable ownership:** port Python `UIScrollView.py` and
    `UIElementReusable`, checked against C++ `ScrollView` and
    `ReusableElement`. Java preserves child ownership, dynamic child renderer,
    gap/auto-scroll/view and horizontal offsets, terminal-scroll delta,
    visible child views, descendant focus scope, proxy-focus restoration,
    text selection/auto-scroll/tick propagation, raw-cell preservation, and
    `ViewHost` child-window routing without redundant indexes or absolute
    coordinates.
  - **Window/modal stack and dialogs:** port Python `application.py`,
    `uiwindow.py`, standard `src/uimd/dialogs`, and C++
    `GeneratedWindowStack`/`GeneratedWindowRuntime` plus `cpp/dialogs`. Java
    uses `GeneratedWindowBase`, `GeneratedScrollViewBase`, runtime/frame
    options, stack frames, universal top-window routing, captured modal return
    state, dimming, size/layout refresh, MessageBox variants, and FileBrowser.
    No dialog or example bridge callbacks may replace runtime ownership.
  - **Images/Sixel:** port Python `image.py` and C++ `Image.cpp`, with the
    parity-repaired C# `Elements.cs` path used only as a JVM-friendly shape
    reference. Java uses standard image decoding plus the smallest missing
    format adapter, then the same contain/cover/stretch mapping, alpha over
    effective background, fixed 64-color palette, exact physical cell pixels,
    one-cell-row raw chunks, bounded LRU payload cache, crop/source-row
    identity, modal/resize invalidation, fallback diagnostics, and raw-anchor
    repaint/scroll-margin rules.
  - **MCP lifecycle:** port Python `mcp.py` and the C++ generated-runtime MCP
    controller/server paths, checked against C# `McpController`. Java keeps
    `McpRuntimeConfig`, shared runtime state and UI lock, top-window element
    paths, schemas/source/snapshots/image info, app tools, controlled render
    generations/conditions, stdio/TCP/HTTP transports, owner-aware dispatch,
    and modal/focus cleanup order. JSON/native transport libraries are adapters
    only and cannot own UI semantics.
  - **Generator/public API:** add one native emitter beside
    `Native{Python,Cpp,CSharp,Swift,Go,Rust}Generator` under `cpp/tools/uimd`.
    It emits typed Java members, identical layout/style/MCP metadata, only
    relevant protected override hooks, dependency classes from the same
    canonical `.uimd`, domain-only app stubs, and reproducible Gradle project
    files. Registration remains solely in native `main.cpp`/CMake; there is no
    Java or Python compiler fork.
  - **SDK/build artifacts:** extend native target validation, `new`, `generate`,
    `sdk install-target`, auto-install/runtime environment, `doctor`, release
    packaging, build manifest, rebuild/full-test scripts, native parity checks,
    docs, and exact example/test commands. The installed Java target contains
    the single Gradle runtime source/build contract and wrapper assets; examples
    and regressions are generated artifacts from canonical Python `.uimd`
    sources, not an implementation copy.

  **Current implementation checkpoint (2026-08-14):**

  **Current audit checkpoint (2026-08-14):** every previously recorded Java
  runtime, generator, terminal, MCP, dialog, image, control, ownership, layout,
  and public-surface remediation below is implemented. Focused JUnit suites,
  both Java Checkstyle tasks, the complete Java `check` gate, direct-terminal
  and MCP transport smoke, the native CLI/installed-SDK parity gate,
  representative example compares, and both Java regression compares pass.
  The mandatory repeat audit found the runtime-version/build-version,
  clipboard, terminal public-surface, and MCP bounds-contract gaps recorded
  immediately below; all are now remediated with focused coverage. The first
  complete C++/Java all-example run passed 1936 explicit assertions and exposed
  only the MCP image-info bounds schema mismatch; the rebuilt focused compare
  passes all 52 assertions after the shared serializer repair. The parity
  artifact manifest validates after the final Java builds, `git diff --check`
  is clean, and Java has generated outputs for all 42 canonical example and 8
  regression `.uimd` sources without a Java-owned `.uimd` fork. The remaining
  completion gate is the canonical full test, which will rerun the complete
  all-example suite together with every existing platform. All work remains
  uncommitted on `sdk-work`.

  **Paused final-validation checkpoint (2026-08-14):** the first canonical
  `--keep-going` run completed 33/34 phases. Its only failure was the Java
  WidgetGallery clean-close response race recorded and fixed below; every
  later Rust example and Python/C++/Go/Java/Rust regression phase passed. A
  fresh post-fix run at
  `.uimd/test-logs/test-all-20260814-175325-495223.log` then passed every
  build, manifest, unit/lint, direct-terminal, transport, and Python/C++ MCP
  phase through `MCP example compare` (1040/1040). Its C# compare reached
  984/984 successful assertions/steps, completing the current MarkdownViewer
  scenario, before the user-requested pause; the runner was then terminated
  deliberately before continuing the remaining platforms. No test process
  remains. On resume, run the complete canonical full test again from the
  beginning; this interrupted run is useful evidence but is not the final
  completion gate.

  - [x] **Make direct Java mouse selection of ordinary `ListBox` rows match
    Python/C++ and cover the real SGR input path.** Manual testing on
    2026-08-19 found that clicking rows in the left ContactsManager ListBox
    and the Formular ListBox does not select them, although keyboard and MCP
    paths pass. Audit Python `src/uimd/runtime/UIBase.py` and
    `src/uimd/runtime/elements.py`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` and
    `cpp/src/elements/BasicElements.cpp`, and Java
    `java/src/main/java/uimd/{GeneratedWindowRuntime,ListBox}.java` for
    terminal-coordinate hit testing, focus/edit entry, row mapping, selection
    notification, and post-event cleanup. Reproduce it with an actual SGR
    mouse press in a PTY, repair only the shared Java runtime, add focused
    JUnit coverage plus a real-terminal regression for both single- and
    multiple-selection ListBoxes, rebuild the affected C++/Java examples, and
    run their compares at `--compare-app-size 90x35`.

    Completed on 2026-08-19. Java's shared generated-window mouse-press path
    now ports C++'s exact visible-row mapping, single/multiple selection,
    active-row, edit-snapshot, edit-start, and owner-aware change-notification
    order. Focused JUnit coverage proves single selection and repeated
    multiple-selection toggling. The expanded Java direct-terminal gate sends
    real SGR press/release input to a nonzero ContactsManager row and a
    multiple-selection Formular row; all 12 groups pass. Regenerated/rebuilt
    C++ and Java ContactsManager, Formular, and SpecialElements targets pass
    their focused compares with 34, 128, and 6 assertions respectively and no
    failed steps at `--compare-app-size 90x35`.

  - [x] **Advance Java animations while the terminal is idle, 1:1 with
    Python/C++, and test progress without injecting input.** Manual testing on
    2026-08-19 found that SpecialElements and other animated examples update
    only after a key or mouse event. Audit Python
    `src/uimd/runtime/application.py`, C++ terminal/generated run-loop code
    under `cpp/src`, and Java
    `java/src/main/java/uimd/{GeneratedWindowRuntime,RenderHelpers}.java` plus
    `java/src/main/java/uimd/terminal` for bounded input polling, render/tick
    cadence, gradient time, differential presentation, and idle wakeups.
    Preserve the same centralized timing contract and avoid an example-level
    timer. Add deterministic loop-level coverage and a real-PTY test proving
    that two frames differ after idle wall-clock time with no input, then run
    Checkstyle, the affected example compares, and direct-terminal smoke.

    Completed on 2026-08-19. Java now detects animated gradients through the
    same window, element-state, and recursive-child style walk as C++ and
    schedules dirty frames on the same centralized 70 ms monotonic cadence
    whenever MCP controlled rendering is inactive. The fix lives only in the
    shared runtime. Focused JUnit coverage proves window and nested state-style
    detection, while the direct-PTY gate observes two consecutive idle update
    intervals from both C++ and Java without sending any input. The complete
    Java Gradle `check`/Checkstyle gate, smoke-script compilation,
    SpecialElements compare, and `git diff --check` pass.

  - [ ] **Align Formular multi-select terminal serialization across every
    language implementation.** The new physical ListBox regression exposed a
    pre-existing example-domain mismatch after selecting a second role:
    Python, Go, and Java serialize every selected role, while C++, C#, Swift,
    and Rust serialize only one active/first role. Runtime and MCP selection
    state already expose the complete selection; audit each hand-written
    Formular application against the Python reference, preserve the identical
    `.uimd` source, and add a direct-terminal Save-output parity gate that
    proves every selected value and callback result without weakening the
    lower-level physical SGR selection test.

  - [x] **Propagate the resolved Java 17 toolchain to every test-launched Java
    example and regression without requiring a caller-exported `JAVA_HOME`.**
    The user-run full test in
    `.uimd/test-logs/test-all-20260814-214218-566182.log` built Java, passed
    Java JUnit/Checkstyle, direct-terminal smoke, and MCP transport smoke, but
    the later all-example and regression MCP phases launched the macOS Java
    placeholder and printed the java.com installation message. The apparent
    Swift direct-terminal `Gallery` failure has the same cause: that phase's
    shared repeated-Sixel-scroll check iterates over every runtime, reaches the
    Java ImageBrowser last, and currently reports only the missing screen text
    when that Java launcher exits. Audit
    `tools/{java_toolchain,uimd_dev}.py`, the Java Gradle `installDist`
    launchers, parity artifact resolution, the native MCP process environment,
    and `tools/swift_direct_terminal_smoke.py`'s all-runtime image process
    environment/error attribution. Preserve the single Java 17 toolchain
    contract and prove the focused Java example, both Java regression
    compares, and the complete shared direct-terminal gate from a normal shell
    with no manually exported Java environment.

    Completed on 2026-08-18 with one SDK-shipped Java 17 resolver contract and
    the smallest POSIX/Windows platform adapters. The source and installed-SDK
    Gradle wrappers, every generated `installDist` launcher, development build
    helper, native `uimd doctor`, release payload, and native generator all use
    that resolver. Standard JDK installations (including Homebrew's keg-only
    `openjdk@17`) require no shell-profile change; `UIMD_JAVA_HOME` remains an
    explicit override only for non-standard locations. Test-launched apps no
    longer receive a hidden `JAVA_HOME`, so the real user launcher path is the
    tested path, and early PTY exits now report the platform, command, exit
    code, and output instead of a misleading missing-screen-text failure.
    With both `JAVA_HOME` and `UIMD_JAVA_HOME` removed, Java JUnit/Checkstyle,
    all 13 example and 2 regression distributions, Calculator compare 43/43,
    regression compares 4/4 and 25/25, shared direct-terminal 35/35, Java
    direct-terminal 10/10, Java MCP transports 5/5, and the isolated native
    CLI/installed-SDK `--compile-examples` gate passed. The canonical full test
    remains the final completion gate for the parent Java-target task. The
    Windows batch adapter is included and structurally covered, but cannot be
    executed on the current macOS host; a real Windows CI run remains required
    before public Windows Java-target installation is advertised as validated.

  - [x] **Return Java's MCP activation response before a clean application
    close, 1:1 with Python/C++.** The canonical full test reached
    `tests/mcp/widget_gallery.yaml` step 52 with all 123 preceding Java
    assertions green, then `activate_element(close_btn)` closed the Java
    process with exit code 0 before the MCP client received its response;
    C++ returns the successful activation result before completing shutdown.
    Audit Python `src/uimd/runtime/mcp.py`, the C++ MCP dispatch/server and
    `toolActivateElement` paths under `cpp/src/generated`, and Java
    `java/src/main/java/uimd/{McpController,McpServer,
    GeneratedWindowRuntime}.java` for callback, response-write, run-loop, and
    shutdown ordering. Keep the repair in the shared Java runtime/transport,
    add focused clean-close response coverage, run Checkstyle, and rerun the
    complete C++/Java WidgetGallery compare with
    `--compare-app-size 90x35`. Do not delay or special-case the example/test.

    Completed on 2026-08-14. `McpSocketServer.stop()` now closes the listener
    immediately and then performs a bounded drain of already accepted TCP/HTTP
    clients, so an in-flight JSON-RPC response is flushed before JVM shutdown.
    A deterministic concurrency test proves that server stop waits for the
    accepted response, the focused JUnit test and both Checkstyle tasks pass,
    all Java example/regression distributions were rebuilt, and the complete
    C++/Java WidgetGallery compare now passes all 123 assertions and the final
    clean-close step with zero failures at `--compare-app-size 90x35`.

  - [x] **Expose and synchronize the Java runtime version contract 1:1 with
    C++.** C++ `cpp/include/ui/core/Version.hpp` and
    `cpp/src/core/Version.cpp` expose `runtimeVersion()` from the canonical
    build version, while Java currently has only the literal Gradle project
    version in `java/build.gradle`. `tools/set_version.py` does not update that
    Java surface, so the next required patch bump would leave the Java runtime
    artifact and generated dependency coordinates stale. Add one public Java
    version API backed by a single synchronized source value, extend
    `tools/set_version.py` and its dynamic checks to update/verify all Java
    version surfaces, and add focused tests proving the public value, Gradle
    artifact version, generated dependency coordinate, and dry-run version
    consistency. Do not introduce a second runtime-version source.

    Completed on 2026-08-14. Java exposes one public
    `Version.runtimeVersion()` value synchronized with `java/build.gradle`,
    every example/regression runtime dependency coordinate, and the native
    Java generator through `tools/set_version.py`. The focused consistency
    suite passed 3 tests, `tools/set_version.py --check` confirmed every
    surface at `0.4.26`, and the complete native CLI/SDK parity gate with
    generated-project compilation passed.

  - [x] **Expose Java's canonical runtime clipboard contract with the same
    public names and behavior as C++.** C++
    `cpp/include/ui/terminal/Clipboard.hpp` publicly exposes
    `copyTextToClipboard` and `runtimeClipboardText`; Java
    `java/src/main/java/uimd/RuntimeClipboard.java` is package-private and
    exposes only differently named internal methods. Make the Java class and
    canonical entry points public, retain existing internal names only as thin
    forwarding aliases where needed, and preserve the one shared clipboard
    state used by terminal and MCP paths. Add focused public API/state/failure
    coverage, run Checkstyle, and rerun the direct-terminal clipboard gate.

    Completed on 2026-08-14. `RuntimeClipboard` and the canonical
    `copyTextToClipboard`/`runtimeClipboardText` entry points are public and
    own the single state used by terminal and MCP paths. Java MCP cut now
    preserves clipboard state like Python/C++. Focused JUnit and Checkstyle,
    the full Java `check` gate, Java direct-terminal smoke 10/10, MCP transport
    smoke 5/5, and the complete Formular compare passed.

  - [x] **Close the remaining Java terminal/render RAII public-surface gaps.**
    The repeat header/method inventory found that C++ exposes
    `ScopedRenderTimeOverride`, `TerminalModeGuard::restore`, and
    `TerminalBackend::{cellPixelSize,readEvents}` directly, while Java exposes
    only a callback helper, `AutoCloseable.close`, a pixel-query overload that
    takes the input stream, and runtime-owned parser reads. Add the canonical
    Java class/method surface and route the generated runtime through the one
    backend-owned input/parser path, retaining old overloads only as thin
    adapters. Java `InputParser`'s `InputStream` plus bounded 50 ms continuation
    layer remains the smallest documented JVM primitive exception to C++'s
    stateful `feed(string_view)` entry point; it must continue producing the
    same events without a second parser implementation. Add focused scoped-
    restore, idempotent terminal restore, backend input/read, pixel-query, and
    parser-boundary tests, then run Checkstyle and the complete direct-terminal
    gate.

    Completed on 2026-08-14. Java now exposes an idempotent
    `ScopedRenderTimeOverride`, `TerminalModeGuard.restore`, and backend-owned
    `cellPixelSize`, `readEvents`, and `inputAvailable`; the generated runtime
    routes through the one backend `PushbackInputStream`/`InputParser`. The
    `InputStream` plus bounded continuation remains the documented smallest
    JVM adapter for C++ `feed(string_view)`. Focused restore/input/parser tests,
    Checkstyle, the complete Java `check` gate, and all 10 direct-terminal
    groups passed.

  - [x] **Align Java MCP image render-info metadata 1:1 with Python/C++.**
    The current complete C++/Java all-example compare passed 1936 explicit
    assertions with zero assertion failures, but
    `tests/mcp/image_gallery_sixel_info_compare.yaml` step 2 reported one
    compare-field mismatch for fallback `camera_fb` image metadata. All 16
    explicit source/load/fit/render-mode/raw/sample assertions passed, and the
    complete ImageGallery render compare passed 13 assertions, so the defect is
    isolated to one response field in the public image-info contract rather
    than visible rendering. Compare Python
    `src/uimd/runtime/{image,mcp}.py`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::toolGetImageRenderInfo`, and
    Java `java/src/main/java/uimd/{Image,McpController}.java`; identify the
    exact divergent field, preserve one canonical source/geometry identity,
    add focused Java coverage, run Checkstyle, and rerun the affected C++/Java
    script with `--compare-app-size 90x35`. The failing run is recorded in
    `.uimd/test-logs/java-all-compare-current.log`; this response-data mismatch
    does not produce a render snapshot bundle or viewer command.

    Completed on 2026-08-14. The divergent field was `bounds`: Java emitted
    `{x,y,width,height}` while Python/C++ expose
    `{left,top,right,bottom,width,height}`. The shared Java MCP rectangle
    serializer now uses the canonical edge schema for both element snapshots
    and image metadata. Focused JUnit and both Checkstyle tasks passed, and the
    rebuilt C++/Java Sixel-info compare passed all 52 assertions with zero
    step failures at `--compare-app-size 90x35`.

  - [x] **Expose Java geometry and text-visual core API 1:1 with C++.** C++
    `cpp/include/ui/core/{Rect,TextVisual}.hpp` publicly exposes
    `Rect::{empty,origin,size}`, the `VisualGlyph` value type, the canonical
    tab width, and `isUnsafeTerminalCodepoint`, `safeTerminalCellText`,
    `visualGlyphs`, `visibleText`, `visibleWidth`, and
    `tabSpacesForColumn`. Java `java/src/main/java/uimd/{Rect,
    RenderHelpers}.java` currently implements most text behavior only through
    a package-private nested glyph and private helpers, and omits the Rect
    methods. Add the equivalent public Java value type and overloads, migrate
    runtime users without changing glyph/source-index semantics, and retain no
    duplicate implementation or state. Add focused Unicode, control,
    combining/wide-codepoint, tab-column/source-range, visible-text, and Rect
    JUnit coverage; run Checkstyle and a representative text/render compare at
    `--compare-app-size 90x35`.

    Completed on 2026-08-14. Java exposes `Rect.empty/origin/size`, public
    `VisualGlyph`, the four-cell tab constant, and the canonical safe-cell,
    visual-glyph, visible-text/width, and tab-column helpers without a second
    text implementation. Focused control, combining, wide/supplementary,
    tab/source-range, visible-text, and Rect JUnit coverage, both Checkstyle
    tasks, and the full Java `check` gate passed. Fresh C++/Java
    MarkdownViewer and WidgetGallery text/render compares passed 67 and 123
    assertions respectively with zero failures at `--compare-app-size 90x35`.

  - [x] **Align Java terminal-buffer state, diff statistics, and cell identity
    1:1 with Python/C++.** Python
    `src/uimd/runtime/rendering.py::TerminalBuffer` and C++
    `cpp/{include,src}/ui/terminal/TerminalBuffer.*` accumulate changed cells,
    changed runs, output bytes, and full-redraw state across every diff/region
    call after `beginRenderStats`, including raw-skip cells, raw clear/repaint
    runs, and scroll-region output. Java
    `java/src/main/java/uimd/{TerminalBuffer,TerminalCell}.java` currently
    replaces the complete statistics record per diff, omits several raw/scroll
    contributions, clamps zero-sized buffers to one cell, and carries the
    unused C#-derived `backgroundFromImageSample` field even though it is not
    part of the Python/C++ cell contract. Port the canonical accumulation and
    nonnegative zero-size behavior, remove the unused identity field and image
    writes, and retain Python's initial full-redraw semantic as the documented
    reference choice where the C++ constructor starts clean. Add focused
    multi-region/raw/scroll/zero-size/negative-size JUnit coverage, run
    Checkstyle, and rerun direct-terminal image/diff gates.

    Completed on 2026-08-14. Java terminal buffers accept zero dimensions,
    reject negative dimensions, preserve the canonical cell identity, and
    accumulate full-redraw, changed-cell/run, raw-skip, raw clear/repaint,
    scroll-region, and output-byte statistics across every render call after
    `beginRenderStats`. The unused image-sample identity field was removed.
    Focused `TerminalBufferTest`, both Checkstyle tasks, and the full Java
    `check` gate passed. The real direct-terminal suite passed all 10 groups,
    including image diff scheduling, exact Sixel geometry, bounded repeated
    scrolling, and sustained wheel input; fresh ImageGallery/ImageBrowser
    compares passed 13 and 378 assertions with zero failures.

  - [x] **Close Java terminal-input escape and mouse-coordinate parity gaps.**
    Python `src/uimd/runtime/application.py::{_decode_escape_sequence,
    _decode_sgr_mouse_sequence}` and C++
    `cpp/src/terminal/Input.cpp` preserve terminal coordinates after the
    one-based conversion and expose the canonical modified cursor/key names;
    Python additionally defines the common `Esc+b`/`Esc+f` word-navigation
    sequences and `[7~`/`[8~` Home/End aliases. Java
    `java/src/main/java/uimd/terminal/InputParser.java` currently consumes and
    drops the character after `Esc+b/f`, omits those Home/End aliases, and
    clamps a reported zero coordinate to `(0,0)`, which can turn malformed or
    outside input into a real top-left click. Preserve the JVM `InputStream`
    and bounded continuation layer as the smallest platform adapter, but port
    the same decoded events and unmodified coordinate semantics. Add focused
    parser coverage for every sequence, negative converted coordinates,
    split input, UTF-8, paste, and wheel boundaries; run Checkstyle and the
    complete Java direct-terminal smoke.

    Completed on 2026-08-14. Java preserves `Esc+b/f`, both Home/End aliases,
    modified-key and UTF-8 decoding, bracketed paste, incomplete escape
    continuation, negative one-based mouse conversions, delayed pixel-report
    filtering, and consecutive-wheel boundaries through the one canonical
    `Event` model. Focused parser coverage, both Checkstyle tasks, and the full
    Java `check` gate passed. The complete real-PTY Java smoke passed all 10
    groups, including split/modified sequences, mouse selection, clipboard,
    image rendering, teardown, and the 800-report wheel burst.

  - [x] **Complete Java generated-window public API and runtime-option
    behavior 1:1 with C++.** C++
    `cpp/include/ui/generated/{GeneratedWindowBase,
    GeneratedWindowRuntime}.hpp` exposes name-based MCP element metadata,
    the complete `GeneratedScrollViewBase` delegation surface, stack-frame
    inspection, resolved-content/render/dim/error helpers, and runtime
    callbacks for application mouse-wheel handling, render overlays, and
    conditional overlay dimming. Java `java/src/main/java/uimd/{
    GeneratedWindowBase,GeneratedScrollViewBase,GeneratedWindowStack,
    GeneratedWindowRuntimeOptions,GeneratedWindowRuntime}.java` currently
    omits those entry points and drops application wheel callbacks after the
    shared control handlers. Add the equivalent public methods and callback
    order, keep Java's unified options object only as a thin JVM compatibility
    representation of C++ frame/runtime options rather than a second state
    machine, expose collections read-only, and do not store new absolute
    geometry. Add focused metadata/delegation/stack/helper/wheel/overlay JUnit
    coverage, run the full generated focus/stack tests and Checkstyle, then
    rerun representative ScrollView/modal/MCP compares at
    `--compare-app-size 90x35`.

    Completed on 2026-08-14. Java now exposes name-based MCP metadata lookup,
    the complete generated ScrollView delegation surface, read-only stack
    frames, resolved-content/render/dim/error helpers, class metadata, and the
    root application wheel plus overlay callbacks. Shared controls consume
    wheel input before the application callback, modal frames do not inherit
    the root callback, overlay dimming precedes overlay paint, and vertical
    clipping propagates through reusable children without cached absolute
    geometry. Focused generated focus/layout/stack and MCP controller tests,
    both Checkstyle tasks, all Java example/regression builds, and the complete
    Java `check` gate passed. Regenerated and rebuilt representative C++/Java
    comparisons passed Calculator (43 assertions), ImageBrowser (378
    assertions), and TaskBoard (360 assertions), each with zero failed
    assertions or steps at `--compare-app-size 90x35`; `git diff --check` was
    clean.

  - [x] **Expose the canonical Java image-runtime helper contract.** C++
    `cpp/{include,src}/ui/elements/Image.*` publicly exposes
    `setImageTerminalCellPixels`, `imageModeNeedsSixelFallbackWarning`, and
    `requireSixelForImageRendering`; Java
    `java/src/main/java/uimd/Image.java` currently has only the differently
    named `setTerminalCellPixels` and an instance warning helper. Add the
    canonical static entry points and retain existing names only as thin
    aliases. `requireSixelForImageRendering` is an unavoidable smallest Java
    adapter and intentionally succeeds because Java owns a pure-Java Sixel
    encoder rather than dynamically loading libsixel; no UI behavior may live
    in that adapter. Add focused helper/render-info coverage, run Image JUnit
    and Checkstyle, and rerun ImageGallery/ImageBrowser compare plus direct-PTY
    Sixel gates at `--compare-app-size 90x35` where applicable.

    Completed on 2026-08-14. Java exposes
    `setImageTerminalCellPixels`, `imageModeNeedsSixelFallbackWarning`, and
    `requireSixelForImageRendering`; `setTerminalCellPixels` and the instance
    warning query remain thin aliases only. The require helper intentionally
    performs no native-library load because Java owns the same rendering
    contract through its pure-Java Sixel encoder. Focused Image JUnit coverage,
    both Checkstyle tasks, and the full Java `check` gate passed. The rebuilt
    C++/Java ImageGallery and ImageBrowser compares passed 13 and 378
    assertions respectively with zero failures at `--compare-app-size 90x35`.
    The real-PTY Java gate passed all 10 groups, including exact 34-pixel Sixel
    rows, bounded repeated Sixel scrolling, image diff/quit behavior, and an
    800-report sustained mouse-wheel burst that remained responsive.

  - [x] **Port Java generated-runtime focusability filtering 1:1 from
    Python/C++.** Python `src/uimd/runtime/UIBase.py::_is_direct_focusable_element`
    and C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::{isFocusableType,
    focusableElements}` keep the base element's public `focusable` flag
    independent from the runtime type whitelist: only Button, CheckBox,
    TextInput/TextArea, NumberInput, ComboBox, ListBox, and ScrollView enter
    ordinary navigation, while reusable/ViewHost proxy branches evaluate
    their flags explicitly. Java
    `java/src/main/java/uimd/GeneratedWindowRuntime.java::isFocusable`
    currently accepts every enabled/focusable element and compensates by
    changing Label/Image constructor flags; after the structural MessageTable
    repair it would also navigate into MessageTable/FrameBufferView. Port the
    exact type filter and reusable branch conditions, remove constructor-level
    compensation, and add focused coverage proving public flags and runtime
    navigation are both correct. Run the complete generated focus/stack suite,
    Checkstyle, and affected MarkdownViewer/ImageBrowser comparisons at
    `--compare-app-size 90x35`.

    Completed on 2026-08-14. Java now keeps each element's public `focusable`
    flag independent from generated-window navigation and applies the same
    Button, CheckBox, TextInput/TextArea, NumberInput, ComboBox, ListBox, and
    ScrollView type whitelist as C++. Reusable, generated ScrollView, and
    ViewHost branches retain their explicit proxy/child conditions; Label,
    Image, MessageTable, and FrameBufferView remain publicly focusable but are
    excluded from ordinary navigation. Focused control plus generated
    focus/stack tests, both Checkstyle tasks, and the complete Java `check`
    gate passed. The rebuilt C++/Java MarkdownViewer and ImageBrowser compares
    passed 67 and 378 assertions respectively with zero failures at
    `--compare-app-size 90x35`.

  - [x] **Port Java `InfoLabel` default animation and `TextGradient` defaults
    1:1 from Python/C++.** Python
    `src/uimd/runtime/elements.py::InfoLabel` and C++
    `cpp/{include,src}/ui/elements/InfoLabel.*` apply the canonical
    nine-color informational gradient whenever no explicit text-color gradient
    is present, while C++ `cpp/include/ui/core/Style.hpp::TextGradient`
    defaults to interval `70`, step `1`, and segment size `1`. Java
    `java/src/main/java/uimd/{InfoLabel,TextGradient}.java` currently leaves
    `InfoLabel` as a behaviorless constructor alias and initializes all three
    gradient fields to zero. Port the same fallback render flow and defaults,
    while preserving an explicitly supplied gradient. Add focused deterministic
    render/JUnit coverage for default and override gradients and run both Java
    Checkstyle tasks plus a representative gradient snapshot comparison at
    `--compare-app-size 90x35`.

    Completed on 2026-08-14. Java `TextGradient` now defaults to interval 70,
    step 1, and segment size 1, and `InfoLabel.render` applies the canonical
    nine-color informational gradient only when the effective style does not
    provide one. The effective style remains a per-render copy, so the fallback
    does not mutate element style state and an explicit gradient is preserved.
    Deterministic default/override JUnit coverage, both Checkstyle tasks, and
    the full Java `check` gate passed. The regenerated/rebuilt C++/Java
    WidgetGallery compare passed all 123 assertions with zero failures at
    `--compare-app-size 90x35`.

  - [x] **Align Java text/number input selection state and public display API
    structurally with Python/C++.** Python
    `src/uimd/runtime/elements.py` and C++
    `cpp/{include,src}/ui/elements/{TextInput,NumberInput}.*` represent a text
    selection with one optional anchor plus the canonical cursor; their public
    `selectionStart`/`selectionEnd` return the cursor when no selection exists,
    and `NumberInput::displayValue` is public. Java
    `java/src/main/java/uimd/{TextInput,NumberInput}.java` redundantly stores
    both `selectionEnd` and `cursor`, returns nullable selection endpoints, and
    keeps `displayValue` private behind the differently named `displayText`
    method. Port the anchor/cursor state machine and endpoint contract 1:1,
    expose `displayValue`, and retain `displayText` only as a thin compatibility
    alias. Add focused JUnit coverage for forward/reverse/no selection,
    collapse/delete/insert behavior, snapshot restoration, and number display;
    run Checkstyle plus Formular/WidgetGallery input compares at
    `--compare-app-size 90x35`.

    Completed on 2026-08-14. Java `TextInput` now stores one optional selection
    anchor plus the canonical cursor; `selectionStart` and `selectionEnd`
    return the cursor when no selection exists and preserve forward/reverse
    collapse, deletion, insertion, and edit-snapshot behavior. `NumberInput`
    exposes `displayValue`, with `displayText` retained only as a forwarding
    compatibility alias. Focused control/runtime JUnit coverage, both
    Checkstyle tasks, and the full Java `check` gate passed. The rebuilt
    C++/Java Formular and WidgetGallery compares passed 239 and 123 assertions
    respectively with zero failures at `--compare-app-size 90x35`.

  - [x] **Complete Java `ComboBox`/`ListBox` public collection, selection,
    and scrolling parity.** C++
    `cpp/{include,src}/ui/elements/{ComboBox,ListBox}.*` exposes read-only
    option/selection collections, uses selected index zero as the empty-list
    sentinel, and provides `ListBox::selectedIndices` plus explicit
    `scrollBy(delta, viewportHeight)` behavior; Python
    `src/uimd/runtime/elements.py` confirms the same externally visible
    selection and bounded-scroll semantics. Java
    `java/src/main/java/uimd/{ComboBox,ListBox}.java` currently returns its
    mutable internal option list, reports `-1` for an empty ComboBox, omits
    `selectedIndices`, and offers only a last-render-size-dependent
    `scrollLines` helper. Return unmodifiable views, port the missing public
    methods and sentinel semantics, retain `scrollLines` only as a thin
    compatibility adapter, and add focused JUnit coverage for mutation
    rejection, empty/nonempty selection, ordered multiple selections, and
    explicit viewport scrolling. Run Checkstyle plus Formular and
    SpecialElements comparisons at `--compare-app-size 90x35`.

    Completed on 2026-08-14. Java `ComboBox` and `ListBox` expose unmodifiable
    option/selection views, use selected index zero for the empty sentinel, and
    preserve ordered de-duplicated multiple selections. `ListBox.scrollBy`
    accepts an explicit viewport height with canonical bounded semantics;
    `scrollLines` remains only a last-render/frame-size compatibility adapter.
    Focused mutation, sentinel, selection-order, and scrolling JUnit coverage,
    both Checkstyle tasks, and the full Java `check` gate passed. The rebuilt
    C++/Java Formular and SpecialElements compares passed 239 and 10
    assertions respectively with zero failures at `--compare-app-size 90x35`.

  - [x] **Port Java `Color` and `Style` public semantics 1:1 from
    Python/C++.** The mandatory public-surface audit found that Python
    `src/uimd/runtime/style.py` and C++
    `cpp/{include,src}/ui/core/{Color,Style}.*` preserve distinct unset,
    transparent, RGB, and named-color states, expose the corresponding
    construction/query contract, accept canonical short hexadecimal color
    forms, and provide a non-mutating merged-style operation. Java
    `java/src/main/java/uimd/{Color,Style}.java` currently collapses those
    states into text/RGBA storage, parses only six/eight-digit hexadecimal
    forms, and exposes only mutating merge behavior. Add the equivalent public
    color kind/value/set-state API and `Style.merged`, retain Java's existing
    compatibility accessors only as thin aliases, and align short `#RGB` plus
    Python-supported `#RGBA` parsing without changing render/blend behavior.
    Add focused JUnit coverage for every color state, short/long forms,
    invalid input, and mutating versus non-mutating style merge; then run both
    Java Checkstyle tasks and representative render/MCP comparison gates.

    Completed on 2026-08-14. Java now exposes distinct unset, transparent,
    RGB, and named kinds with the canonical construction/query state, validates
    hexadecimal input, and supports C++ short `#RGB` plus Python-compatible
    short `#RGBA` alongside the long forms. Existing text/RGBA accessors are
    thin views of that one state. `Style.merged` returns a copied merged style,
    while `merge` retains the canonical mutating operation. Focused state,
    parsing, invalid-input, and merge JUnit coverage, both Checkstyle tasks,
    and the full Java `check` gate passed. Fresh C++/Java Calculator and
    WidgetGallery render/MCP compares passed 43 and 123 assertions
    respectively with zero failures at `--compare-app-size 90x35`.

  - [x] **Port Java `FrameBufferView` structurally and behaviorally 1:1 from
    Python/C++.** The mandatory post-implementation audit found that Python
    `src/uimd/runtime/elements.py::FrameBufferView` and C++
    `cpp/{include,src}/ui/elements/FrameBufferView.*` expose a multi-target
    framebuffer with `clearFrames`, `setFrames`, target ordering, explicit or
    evenly split regions, pan offsets, dimming, interaction-state corner
    overlays, invalid-color fallback, and safe one-cell glyph extraction.
    Java `java/src/main/java/uimd/FrameBufferView.java` currently stores only
    one `List<List<TerminalCell>>`, exposes only `setContent`, and performs a
    direct blit. Add the equivalent public `FrameBufferCell` and
    `FrameBufferRegion` value types and the same runtime state/render order,
    without an example-specific adapter or redundant cache. Add focused JUnit
    coverage for target splitting, explicit clipping/regions, pan, dimming,
    color fallback, and focused/edit corners; run Java Checkstyle and the MCP
    tester/runtime gates that use the framebuffer panel.

    Completed on 2026-08-14. Java now exposes the canonical `FrameBufferCell`
    and `FrameBufferRegion` value types and a multi-target `FrameBufferView`
    with clear/set frames, stable target order, explicit or evenly split
    regions, pan, dimming, safe one-cell glyphs, color fallback, and
    focus/edit corner overlays in the same render order as Python/C++. The
    implementation owns only canonical source/render state and no redundant
    framebuffer cache. Focused `ControlsTest` and `McpControllerTest`, both
    Checkstyle tasks, and the complete Java `check` gate passed; the canonical
    MCP tester configuration/runtime suite also passed all 60 tests after it
    was run with localhost access.
  - [x] **Complete Java `ScrollView`'s public runtime contract without adding
    redundant cached state.** C++
    `cpp/{include,src}/ui/elements/ScrollView.*` exposes
    `invalidateHeightCache`, `cachedChildHeights`, `hViewOffset`,
    `contentWindow`, `canScrollUp`, `canScrollDown`, and
    `ensureChildVisible`, plus the explicit-viewport `handleKey` overload,
    including the `ScrollViewContentWindow` value type;
    Java `java/src/main/java/uimd/ScrollView.java` implements the underlying
    scrolling/render behavior but omits those public surfaces or uses only the
    longer `horizontalViewOffset` spelling. Preserve the Python/C++ behavior
    and call order while computing child heights from the existing child
    collections as required by the project no-duplicate-state rule. Add
    focused JUnit coverage for the complete public contract, then run Java
    Checkstyle plus the ScrollView example and regression compares at
    `--compare-app-size 90x35`.

    Completed on 2026-08-14. Java exposes `invalidateHeightCache`, computed
    read-only child heights, `hViewOffset`, `ScrollViewContentWindow`,
    directional scroll queries, explicit-viewport key handling, and
    `ensureChildVisible` with the same call order and padded-viewport behavior
    as C++. Height inspection is recomputed from the canonical child
    collections, so the public compatibility surface adds no duplicate cache.
    Focused public-contract JUnit coverage, both Checkstyle tasks, and the full
    Java `check` gate passed. Fresh ImageBrowser and TaskBoard example compares
    passed 378 and 360 assertions; the C++/Java `source_separator_scroll` and
    `stale_scrollview_focus` regression compares passed 4 and 25 assertions.
    Every gate had zero failures at `--compare-app-size 90x35`.
  - [x] **Port the public Java application/element ownership surface 1:1 from
    C++.** The mandatory audit found no Java equivalents for
    `cpp/include/ui/app/{Application,Control}.hpp`; Java
    `java/src/main/java/uimd/{Window,Element}.java` also exposes mutable child
    collections and does not establish the canonical parent link when a child
    is added. Add `Application` and `Control`, align null rejection and
    read-only collection exposure in `Window`, and make `Element.addChild` /
    `clearChildElements` own the parent transition. Preserve JVM object
    lifetime as the smallest adapter for C++'s native live-pointer registry;
    add a stable public element identity only if it serves the same observable
    contract without becoming redundant lookup state. Add focused ownership,
    active-window, null-rejection, and collection tests plus Checkstyle.

    Completed on 2026-08-14. Java now provides `Application` and `Control`,
    preserves ordered active-window ownership, rejects null additions, and
    exposes application, window, and element collections as unmodifiable
    views. `Element.addChild` establishes the canonical parent link and
    `clearChildElements` removes it. Its monotonically assigned public
    identity mirrors C++ `Element::identity` and is not used as a duplicate
    lookup index; JVM reachability remains the smallest adapter for C++'s
    native live-pointer registry. Focused ownership/identity/active-window
    JUnit coverage, both Checkstyle tasks, and the complete Java `check` gate
    passed.
  - [x] **Port C++'s public layout model and resolver to Java.** Java currently
    has only `AxisDimension`/`DimensionMode` and private generated-runtime tree
    helpers, while `cpp/{include,src}/ui/core/Layout.*` publicly exposes
    `LayoutDirection`, `LayoutElement`, `LayoutCell`, `ResolvedElement`,
    `ResolvedCell`, `FlatResolvedCell`, `FlatResolvedElement`, lookup/flatten
    helpers, and `LayoutResolver`. Add the same data roles and resolution
    algorithm as a public Java core API without routing generated UI through a
    second compiler or changing generated runtime geometry. Port the focused
    row/column/fixed/expanded/fit/padding/gap/lookup/flatten C++ skeleton cases
    to JUnit and run Checkstyle.

    Completed on 2026-08-14. Java now exposes `LayoutDirection`,
    `LayoutElement`, `LayoutCell`, `ResolvedElement`, `ResolvedCell`, both flat
    result types, recursive lookup/flatten helpers, and `LayoutResolver` with
    the C++ row/column, fixed/expanded/auto/fit-content, padding, gap, measure,
    and remainder-distribution algorithm. This remains an independent public
    core API and does not introduce a second generated-window geometry path.
    Focused resolver/lookup/flatten JUnit coverage, both Checkstyle tasks, and
    the complete Java `check` gate passed.
  - [x] **Use one canonical Java runtime event model equivalent to C++
    `Event`/`EventType`.** C++ `cpp/include/ui/core/Event.hpp` and terminal
    input share one event value carrying key, text, point, wheel delta, and
    size for key/mouse/paste/resize/focus/window lifecycle events. Java
    `java/src/main/java/uimd/terminal/TerminalEvent.java` currently defines a
    terminal-only subset and is threaded throughout the generated runtime.
    Replace it with public `uimd.Event`/`EventType` as the single behavior
    model (or retain only a behavior-free forwarding compatibility shim if a
    real compatibility need is demonstrated), update input/runtime/MCP tests,
    and run the complete parser, generated-window, direct-PTY, and MCP
    transport gates.

    Completed on 2026-08-14. Java now has one public `uimd.Event` and
    `EventType` carrying key, text, point, wheel delta, and size across every
    C++ event kind; the terminal parser, generated runtime, and MCP controller
    all consume it directly, and no `TerminalEvent` compatibility model
    remains. Complete parser/generated-window coverage and the full Java
    `check` gate passed. The real direct-terminal smoke passed all 10 groups,
    and Java MCP stdio/TCP/HTTP, concurrency/modal lifecycle, and unsupported-
    transport smoke passed all 5 groups.
  - [x] **Make Java `MessageTable` structurally match Python/C++.** Python
    `src/uimd/runtime/elements.py::MessageTable` and C++
    `cpp/{include,src}/ui/elements/MessageTable.*` inherit directly from the
    base element and own only table text/render state. Java
    `java/src/main/java/uimd/MessageTable.java` inherits `Label`, which makes
    it non-focusable and adds unrelated span/selection state. Move the text
    getter/setter into `MessageTable`, retain the same parsing/render output,
    add focused inheritance/focusability/table tests, and run the Markdown
    Viewer compare at `--compare-app-size 90x35`.

    Completed on 2026-08-14. Java `MessageTable` now inherits directly from
    `Element`, owns only its table text/parser/render state, and retains the
    base element's public focusable flag while the generated-runtime type
    filter excludes it from ordinary navigation. Focused inheritance,
    focusability, fit-height, setter, and table-render JUnit coverage, both
    Checkstyle tasks, and the complete Java `check` gate passed. The rebuilt
    C++/Java MarkdownViewer compare passed all 67 assertions with zero failures
    at `--compare-app-size 90x35`.

  - [x] **Match Java full-surface blank-cell rendering to Python/C++.** The
    first real C++/Java calculator compare exposed that
    `java/src/main/java/uimd/GeneratedWindowRuntime.java` paints every cell
    outside the centered window with an explicit black background, while
    Python `src/uimd/runtime/mcp.py::_render_full_viewport_cells` and C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::renderViewportContent`
    preserve unstyled blank cells. Remove the Java-only terminal background
    fill, add focused MCP snapshot coverage, rebuild the Java calculator, and
    rerun `tests/mcp/calculator.yaml` against C++ at
    `--compare-app-size 90x35`. Completed on 2026-08-14 with focused MCP tests,
    Checkstyle, a rebuilt Java calculator, and the complete calculator compare.
  - [x] **Match Java focused Button decoration geometry to Python/C++.** The
    calculator compare then reached step 31 and showed that
    `java/src/main/java/uimd/Button.java` subtracts the six-cell comfortable
    threshold from the inner title width. Python
    `src/uimd/runtime/elements.py::Button.render_cells` and C++
    `cpp/src/elements/BasicElements.cpp::Button::render` both subtract only
    the four actual decoration cells. Port the same formula, add focused
    control coverage, rebuild the Java calculator, and rerun the same compare.
    Completed on 2026-08-14; the compare passed all 10 assertions and every
    render step with zero failures.
  - [x] **Remove a closed Java FileBrowser from the universal modal stack when
    an overwrite confirmation closes it from a nested MessageBox callback.**
    Python `src/uimd/dialogs/file_browser.py::_close` invokes the selection
    callback and then closes the browser window, including when
    `_confirm_overwrite` is returning from its child MessageBox. Java
    `java/src/main/java/uimd/FileBrowser.java::close` currently only marks the
    browser closed; `GeneratedWindowRuntime.dispatchWindowStackEvent` can
    automatically remove only the child MessageBox frame that received the
    event, leaving the closed browser underneath. Keep removal in the shared
    FileBrowser/runtime stack path, preserve Python callback-before-browser-
    removal order, add a nested overwrite stack test, and validate the full
    TextEditor MCP compare at `--compare-app-size 90x35`.

    Completed on 2026-08-14. The focused nested overwrite stack test, Java
    Checkstyle, regenerated/rebuilt C++ and Java TextEditor, and the complete
    TextEditor compare all passed; the compare covered 150 assertions and
    every render/modal/file-operation step with zero failures.
  - [x] **Match Java ListBox Enter activation/edit cleanup to Python/C++.**
    The C++/Java TextEditor compare reached FileBrowser selection at step 54
    with 82 assertions passing, then C++ left ListBox edit mode after Enter
    activated the selected file while Java retained `edit_mode=true` and the
    editing selection style. Audit Python
    `src/uimd/runtime/elements.py` ListBox key handling and C++
    `cpp/src/elements/BasicElements.cpp` plus generated-runtime dispatch
    against `java/src/main/java/uimd/{ListBox,GeneratedWindowRuntime}.java`.
    Preserve callback/modal transition order, perform cleanup in the shared
    runtime rather than FileBrowser/example code, add focused key-activation
    coverage, and rerun the full TextEditor compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-024853-step-54-text_editor.json`.

    Completed on 2026-08-14 by keeping the single-select ListBox confirmation
    branch ahead of generic leave-commit handling, matching C++. Focused JUnit
    and both Checkstyle tasks passed, then the complete TextEditor compare
    passed all 150 assertions and every step.
  - [x] **Regenerate stale Java gradient examples after the native emitter
    fix.** The first C++/Java SpecialElements frame showed reference
    `#ee0000` but Java `#ff0000`. Audit confirmed both start from explicit
    `#ff0000`; C++ applies the timed text gradient, while the checked Java
    `SpecialElementsUI.java` predates the repaired
    `NativeJavaGenerator.cpp::{textGradientCode,styleCode}` and contains no
    gradient object. Regenerate the Java example from the canonical Python
    `.uimd`, rebuild both sides, rerun SpecialElements at
    `--compare-app-size 90x35`, and ensure the later rebuild-script integration
    always regenerates Java outputs. Failure snapshot:
    `tests/mcp/snapshots/20260814-025511-step-1-special_elements.json`.

    Completed on 2026-08-14. The canonical SpecialElements `.uimd` was
    regenerated through the native Java target after the full parity rebuild;
    the rebuilt C++/Java comparison passed all 6 assertions and every render
    step at `--compare-app-size 90x35`.
  - [x] **Emit initial ComboBox/ListBox selections from the native Java
    generator.** After gradient regeneration, SpecialElements showed Java on
    the first ComboBox/ListBox option while C++ preserved the canonical
    `selected_item`/`selected_items` values. C++
    `cpp/tools/uimd/NativeCppGenerator.cpp` emits the resolved selection after
    construction; `cpp/tools/uimd/NativeJavaGenerator.cpp` currently emits
    only the option list. Port the same compiler-model-driven selection logic,
    preserve multiple-selection order, add native Java generator coverage,
    regenerate/rebuild both examples, and rerun SpecialElements at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-025705-step-1-special_elements.json`.

    Reopened on 2026-08-14 after the first Formular compare. The native
    generator smoke and SpecialElements proved ComboBox plus single-select and
    multi-value ListBox initialization, but Formular exposed the remaining
    one-value/multiple ListBox branch: C++ constructs the ListBox in
    single-select mode, establishes its initial selected index, then enables
    `multiple`; Java passes `multiple=true` into the constructor before
    `setOptions`, so `setSelectedIndex` updates only the active row and leaves
    `selectedIndices` empty. Match the C++ construction/state order in
    `java/src/main/java/uimd/ListBox.java` and the native Java emitter. During
    the audit, also correct the existing native C++/Java emitter branch that
    calls `setSelectedIndex` for an explicit one-value multiple selection:
    unlike Python's direct `selected_items` initialization, that call only
    moves the active row once multiple mode is enabled. Both native emitters
    must use the resolved selected-value list for every explicit multiple
    selection, including one nonzero item. Extend focused runtime/generator
    coverage for one-value multiple ListBoxes, then rerun Formular and
    SpecialElements. Formular failure snapshot:
    `tests/mcp/snapshots/20260814-031130-step-1-formular.json`.

    Completed on 2026-08-14. Java now follows the C++ constructor/multiple-mode
    order, both native emitters preserve every explicit multiple selection via
    selected values (including one value), focused runtime and native emitter
    tests plus Checkstyle passed, SpecialElements remained green, and the
    complete Formular compare passed all 128 assertions with zero step
    failures.
  - [x] **Port generated runtime-cell tree layout and natural-size resolution
    to Java 1:1 with Python/C++.** The first WidgetGallery compare showed that
    `java/src/main/java/uimd/GeneratedWindowRuntime.java` still derives natural
    content size and cell frames directly from each generated `sourceCell`,
    while Python layout semantics and C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::{collectRuntimeCells,
    buildRuntimeTree,resolveRuntimeCellsWithFitPass,resolvedContentExtent}`
    reconstruct shared separators, nested cells, expansion, and fit-content
    before rendering. This makes Java WidgetGallery one column wider and will
    affect other nested/expanded layouts. Port the same runtime-node structure,
    sizing passes, resolved-cell lookup, element frame/render coordinates, and
    window-mode behavior without Java-only geometry constants. Add focused
    nested/expanded layout tests, rerun WidgetGallery compare, and cover the
    complete Java example/regression compare suite at `--compare-app-size
    90x35`.

    Completed on 2026-08-14. Java now has the same collected runtime-cell,
    separator reconstruction, runtime-node tree, natural-size, expansion,
    fit-content second pass, resolved-cell lookup, content-extent, and
    window/reusable sizing flow as C++. Focused adjacent-cell, expanded-cell,
    and reusable fullscreen-distribution JUnit coverage passed, followed by
    all 13 C++/Java example compares and both Java regression compares at
    `--compare-app-size 90x35`.
  - [x] **Do not composite Java reusable-wrapper focus color over a directly
    edited generated ScrollView child, 1:1 with Python/C++.** The C++/Java
    MarkdownViewer compare passed all document selection and button-scroll
    steps, then diverged when MCP entered edit mode on `viewer`: C++ retained
    the canonical black document surface while Java blended the reusable
    wrapper focus color into it as `#141414`. Audit Python
    `src/uimd/runtime/{UIElementReusable,UIScrollView}.py`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` plus
    `cpp/src/elements/{ReusableElement,ScrollView}.cpp`, and Java
    `java/src/main/java/uimd/{ReusableElement,GeneratedWindowRuntime,
    ScrollView}.java` for direct wrapper focus, generated ScrollView ownership,
    edit-mode render state, and focus-background application. Keep the repair
    in the shared Java runtime, add focused reusable-ScrollView edit coverage,
    regenerate/rebuild both MarkdownViewer targets, and rerun the complete
    compare at `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-060349-step-36-markdown_viewer.json`.

    Completed on 2026-08-14. Java now ports C++'s transparent-viewport
    content protection in the root ScrollView focus-gap pass: padding receives
    the proxy focus surface while rows containing rendered document content
    retain their canonical backgrounds. The focused reusable-ScrollView edit
    regression, Java Checkstyle, regenerated/rebuilt C++ and Java
    MarkdownViewer targets, and the complete compare passed all 22 assertions
    and every selection, button-scroll, edit-scroll, and snapshot step.
  - [x] **Exit Java's current ordinary edit session before explicit MCP
    activation changes focus.** The first C++/Java ActivityFeed compare passed
    the initial window and text-edit steps, then diverged immediately after
    activating `add_button`: C++ appended and rendered the third activity,
    while Java retained the two-child dynamic feed. A direct MCP-controller
    probe confirmed that ActivityFeed's dynamic renderer and invalidation are
    correct; Java `McpController.toolActivateElement` focuses the Button while
    leaving the previous TextArea's `editMode=true`, so the following Enter is
    routed through `handleEditKey` on the Button and never dispatches its click
    callback. Python `UIBase.set_focus` exits the old edit session, and C++
    `toolActivateElement` clears active edit state before direct Button/Image/
    reusable activation. Port that cleanup order to
    `java/src/main/java/uimd/{McpController,GeneratedWindowRuntime}.java`, add
    focused edit-then-activate callback/value coverage, regenerate/build both
    ActivityFeed targets, and rerun `tests/mcp/activity_feed.yaml` at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-050205-step-4-activity_feed.json`.

    Completed on 2026-08-14. Explicit MCP activation now commits and exits the
    previous ordinary edit session before focus moves, focused controller tests
    and both Checkstyle tasks passed, and ActivityFeed subsequently exercised
    the Add callback and dynamic feed update successfully in the complete
    C++/Java compare.
  - [x] **Keep Java ComboBox dropdown overlays inside modal frame clipping and
    sizing 1:1 with C++.** After edit-to-activation cleanup was repaired, the
    ActivityFeed compare passed 18 assertions through add/seed/clear and modal
    opening, then diverged when `default_type` entered edit mode: C++ renders a
    dropdown option at row 22 while Java has already returned to the dimmed
    parent surface there. Audit Python modal/ComboBox rendering, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` overlay frame and stack
    bounds, and Java `java/src/main/java/uimd/GeneratedWindowRuntime.java` for
    expanded-row resolution, modal content bounds, clip extension, centering,
    and blit order. Add focused ComboBox-near-modal-bottom coverage, rebuild
    both ActivityFeed targets, and rerun the complete compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-050749-step-14-activity_feed.json`.

    Completed on 2026-08-14 by porting C++
    `overlayFocusedComboBox` into Java's shared terminal and modal-stack render
    paths. The active dropdown is repainted as the top terminal layer after the
    natural window frame is blitted, so all options remain visible outside a
    short modal without changing modal geometry. Focused modal-bottom JUnit
    coverage and both Checkstyle tasks passed; both ActivityFeed targets were
    regenerated/rebuilt and the complete compare passed all 26 assertions and
    every step at `--compare-app-size 90x35`.
  - [x] **Honor Java focus requests made before the generated runtime frame is
    attached, 1:1 with Python/C++.** The first C++/Java ExpenseTracker frame
    has the same active navigation-button colors on both sides, but C++ renders
    the focused `expenses_btn` decoration while Java renders an unfocused
    button. ExpenseTracker calls `setFocus(expenses_btn)` while constructing
    its initial ViewHost child, before the runtime stack is attached. Audit
    Python `UIBase.set_focus`, C++ generated-frame initialization/requested
    focus handling, and Java
    `java/src/main/java/uimd/{GeneratedWindowBase,GeneratedWindowRuntime,
    GeneratedWindowStack}.java` for pending-focus consumption, frame index,
    edit cleanup, and modal-stack attachment order. Keep the fix in the shared
    Java runtime, add focused pre-attachment and post-attachment tests, rebuild
    both ExpenseTracker targets, and rerun the complete compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-052103-step-1-expense_tracker_compare.json`.

    Completed on 2026-08-14. `GeneratedWindowBase` now retains only genuinely
    pre-attachment focus requests and `GeneratedWindowRuntime.createFrame`
    consumes them after constructing the canonical focusable list. Focused
    pre/post-attachment stack tests, Checkstyle, and the complete Expense
    Tracker compare passed.
  - [x] **Do not blend a Java ViewHost wrapper focus background over a directly
    focused child window 1:1 with Python/C++.** After pre-attachment focus was
    repaired, ExpenseTracker progressed through initial rendering and editing
    `main.item_input`, then Java rendered the input background as `#25364f`
    while C++ retained the child's canonical `#0b1220`. Audit Python
    `UIElementReusable`/ViewHost descendant rendering, C++
    `renderEntry` reusable focus composition, and Java
    `java/src/main/java/uimd/{ReusableElement,GeneratedWindowRuntime}.java`
    for direct wrapper focus versus descendant focus, active ScrollView
    ownership, parent background propagation, and alpha application. Add a
    focused ViewHost-child edit test, rebuild both ExpenseTracker targets, and
    rerun the complete compare at `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-052318-step-3-expense_tracker_compare.json`.

    Completed on 2026-08-14. A reusable/ViewHost render branch now receives
    the global edit context while its child performs the same owner check as
    C++; Java no longer turns a directly edited descendant into wrapper
    navigation styling. Focused child-edit rendering coverage, Checkstyle, and
    the complete Expense Tracker compare passed.
  - [x] **Suppress Java reusable/ScrollView focus surfaces behind a newly
    opened modal 1:1 with Python/C++.** The C++/Java TaskBoard compare passes
    46 assertions through nested row navigation, then opening `board[2]`'s
    task dialog leaves Java's parent board surface at the dimmed focus color
    while C++ renders the canonical base board surface before applying the
    modal backdrop. Audit Python window-stack rendering and reusable/
    ScrollView scope suppression, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` modal-background and
    `renderEntry` paths, and Java
    `java/src/main/java/uimd/{GeneratedWindowRuntime,ReusableElement,
    GeneratedWindowStack}.java` for the captured parent frame, active scope,
    direct reusable focus, suppression propagation, and backdrop order. Keep
    the repair in the shared Java runtime, add focused modal-over-reusable-
    ScrollView coverage, rebuild both TaskBoard sides, and rerun the complete
    compare at `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-062234-step-38-task_board_compare.json`.

    Completed on 2026-08-14. Java modal-background rendering now follows
    C++ by disabling the covered frame's edit presentation and by preventing
    reusable navigation-scope focus propagation while scope visuals are
    suppressed. A focused modal-over-nested-reusable-ScrollView JUnit test,
    both Checkstyle tasks, regenerated/rebuilt C++ and Java TaskBoard targets,
    and the complete compare passed all 190 assertions with zero step
    failures.
  - [x] **Match Java generated ScrollView overflow extent after a ViewHost
    page swap 1:1 with Python/C++.** The first C++/Java
    `stale_scrollview_focus` regression compare passes initial window parity,
    then after activating `page[1].open_btn` C++ renders the bottom `v`
    overflow indicator at row 31 while Java leaves the same cell blank. Audit
    Python `src/uimd/runtime/{UIElementReusable,UIScrollView}.py`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` plus
    `cpp/src/elements/{ReusableElement,ScrollView}.cpp`, and Java
    `java/src/main/java/uimd/{GeneratedWindowRuntime,ReusableElement,
    ScrollView}.java` for ViewHost child replacement, generated natural
    content extent, viewport padding, max offset, and overflow-marker render
    order. Keep the repair in the shared Java runtime, add focused page-swap /
    generated-ScrollView coverage, rebuild both regression targets, and rerun
    `tests/regressions/uimd/parity/stale_scrollview_focus.yaml` at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-063811-step-2-stale_scrollview_focus.json`.

    Completed on 2026-08-14 by porting C++'s host-viewport root ScrollView
    indicator overlay into the shared Java generated runtime. The focused
    ViewHost page-swap JUnit test, Java Checkstyle, regenerated/rebuilt C++ and
    Java regression targets, and the complete compare passed all 12
    assertions with zero step failures.
  - [x] **Emit Java app stubs with the public class filename required by
    Java.** Native `cpp/tools/uimd/NativeJavaGenerator.cpp` currently writes
    `hello.java` for a `Hello` public application class, while Java requires
    the filename `Hello.java` on case-sensitive filesystems. Match the
    generated class-name/file-name contract in `new` and `generate
    --app-stub`, keep the canonical lowercase `.uimd` source name unchanged,
    extend `tools/native_uimd_parity.py` to compile the generated Java project,
    and validate source-checkout plus installed-SDK runtime lookup.

    Completed on 2026-08-14. Native `new` and `generate --app-stub` now emit
    `Hello.java` beside `HelloUI.java`, keep the lowercase `hello.uimd`, and
    resolve the Java runtime from both the source checkout and
    `targets/java` in an installed SDK. The complete
    `tools/native_uimd_parity.py --compile-examples` gate passed with the Java
    17/Gradle toolchain as well as all pre-existing target checks.
  - [x] **Exclude Gradle `build/` output from the canonical parity-manifest
    source fingerprint.** Java is now a source input under
    `src/uimd/testing/artifact_manifest.py`, but its generated class files,
    jars, reports, and `installDist` trees currently remain in the source hash
    because only `.gradle` is excluded. Match the existing CMake/Swift/Rust/
    Go build-output exclusions, retain Java launchers as declared parity
    artifacts, add focused manifest stability coverage, and prove that Java
    tests/builds performed after manifest creation do not make source inputs
    stale.

    Completed on 2026-08-14. The canonical source walker now excludes every
    `build/` directory while declared Java launchers remain hashed parity
    artifacts. Focused Python coverage includes Java in the all-platform
    resolver and mutates a Gradle report after manifest creation; all four
    focused manifest tests passed. A real manifest was then written, the full
    Java `check` task ran, and `validate_manifest` remained green afterward.
  - [x] **Emit and detect Java terminal title setup 1:1 with C++ in a real
    PTY.** The first `tools/java_direct_terminal_smoke.py` run started the
    generated Calculator launcher but parsed an empty OSC title, while Python
    `src/uimd/runtime/application.py` and C++ terminal startup emit the window
    title before the initial frame. Audit
    `java/src/main/java/uimd/{GeneratedWindowRuntime,TerminalBackend}.java`
    and the newly shared `tools/rust_direct_terminal_smoke.py` title parser,
    preserve the same startup/teardown byte order, add focused coverage, then
    rerun the complete Java direct-terminal smoke.
  - [x] **Port Java modified-key escape parsing 1:1 with Python/C++.** Enabling
    `modifyOtherKeys` exposes CSI modifier forms, but
    `java/src/main/java/uimd/terminal/InputParser.java` currently handles only
    unmodified arrows and Shift+Tab. Port Python
    `src/uimd/runtime/application.py` and C++
    `cpp/src/terminal/Input.cpp` handling for modified cursor/Home/End keys,
    CSI-u codepoints, legacy `27;modifier;codepoint~`, Alt+Enter, and Ctrl+C;
    add focused parser tests and rerun the Formular direct-PTY selection/input
    cases against C++.
  - [x] **Keep incomplete Java escape sequences pending through the canonical
    continuation window.** The next direct-terminal Formular case splits ESC
    from `[B`, `[A`, and `OB` by 30 ms. Go/Rust retain an incomplete escape for
    the shared 50 ms continuation window, while Java
    `java/src/main/java/uimd/terminal/InputParser.java` waits only 5 ms and
    emits standalone Escape followed by literal bytes. Use the same bounded
    continuation semantics without delaying already-buffered input, add a
    focused split-sequence parser test, and rerun all Formular PTY cases.
  - [x] **Make the shared Rust/Java direct-terminal startup gate wait for the
    complete mode sequence and report the actual target name.** The Java PTY
    rerun parsed the OSC title before `TerminalBackend.enter` had finished
    writing mouse/paste/modifyOtherKeys sequences, so
    `tools/rust_direct_terminal_smoke.py::run_title_ctrl_c_and_teardown`
    inspected a partial output buffer and failed nondeterministically. The same
    shared helper also imports `assert_equal_screen` whose diagnostics are
    hardcoded to `Go`. Wait within the existing startup deadline for title and
    every required sequence, parameterize the comparison label, retain Go
    defaults, and rerun Java plus the affected shared smoke coverage.
  - [x] **Preserve Java's no-focus sentinel when Enter arrives from a real
    terminal.** In the direct C++/Java Calculator PTY comparison, raw
    `1+2<Enter>` leaves C++ unfocused and does not activate a control, while
    Java changes `focusedIndex=-1` to the first Button and appends `0`. Audit
    Python `src/uimd/runtime/UIBase.py`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` key dispatch, and Java
    `java/src/main/java/uimd/GeneratedWindowRuntime.java`; only activate Enter
    when a valid focused element exists, keep post-event cleanup in the shared
    runtime, add a focused regression, and rerun Calculator PTY parity.
  - [x] **Dispatch Java bracketed paste as a dedicated runtime event 1:1 with
    Python/C++.** The Java direct-terminal Formular smoke reaches the correct
    TextArea edit session but does not render the split bracketed-paste payload
    `split paste\nvalue`. Python
    `src/uimd/runtime/application.py::{_read_key,_decode_bracketed_paste_sequence}`
    produces a paste event and C++ `cpp/src/terminal/Input.cpp::parseBracketedPaste`
    emits `EventType::Paste`, while Java
    `java/src/main/java/uimd/terminal/{InputParser,TerminalEvent}.java` currently
    encodes the complete payload as an ordinary multi-character key. Add the
    dedicated event kind/text payload and shared generated-runtime paste
    dispatch, preserve selection/edit notification and newline behavior, add
    focused parser/runtime coverage, and rerun the complete Java direct-terminal
    smoke against C++.
  - [x] **Centralize Java runtime clipboard handling and direct `cmd_c`/
    `cmd_v` notification behavior 1:1 with Python/C++.** The Formular PTY
    selection case parses both modified keys but Java `TextInput` receives
    them as ordinary keys because clipboard state currently exists only as a
    private field in `java/src/main/java/uimd/McpController.java`. Port the
    shared runtime clipboard contract from `src/uimd/runtime/elements.py` and
    `cpp/src/terminal/Clipboard.cpp`, route direct top-window copy/paste through
    the same edit and change-notification order, render the bounded
    `Copied to clipboard` terminal overlay from the shared runtime, and make
    MCP tools use the same storage. Add focused copy/paste tests and rerun the
    Formular plus WidgetGallery real-PTY cases.
  - [x] **Port Java terminal mouse text-selection lifecycle 1:1 with
    Python/C++.** WidgetGallery's real SGR press-drag-release never produces a
    selection or clipboard notification because Java
    `GeneratedWindowRuntime.dispatchEvent` currently handles mouse press and
    wheel only; drag/release are dropped. Port label/TextInput selection
    anchors, selectable-style checks, recursive reusable ownership, drag
    updates, release copy/notification, and key-driven label-selection cleanup
    from `src/uimd/runtime/UIBase.py` and
    `cpp/src/generated/GeneratedWindowRuntime.cpp::{handleLabelMousePress,
    handleMouseDrag,clearLabelSelectionsInWindow}`. Keep state in the shared
    interactive runtime, add focused label/TextInput tests, and rerun the full
    WidgetGallery PTY comparison.
  - [x] **Configure Java POSIX raw mode with the same bounded read timeout as
    C++.** After the clipboard overlay appeared correctly, it never expired
    until another key arrived because
    `java/src/main/java/uimd/terminal/TerminalModeGuard.java` runs `stty raw
    -echo` with blocking `VMIN=1`, whereas C++
    `cpp/src/terminal/TerminalBackend.cpp::TerminalModeGuard` uses `VMIN=0`,
    `VTIME=1`. Port those settings so idle render generations, notification
    expiry, animations, MCP state, and clean shutdown can progress without
    input; extend terminal-mode coverage and rerun the complete Java PTY smoke.
  - [x] **Clear/home Java's terminal surface before leaving the alternate
    screen 1:1 with the parity-validated C++ launcher.** Formular Save/Cancel
    correctly waits until `runGeneratedWindow` returns before printing YAML,
    but Java `TerminalBackend.leave` restores the alternate screen without
    C++ `cpp/src/generated/GeneratedWindowRuntime.cpp`/Rust's
    clear-home-newline teardown.
    The PTY therefore retains the UI and cursor position, overwriting YAML into
    stale rows (`accepted_terms: falsefalse`). Match the canonical teardown
    byte order, extend terminal backend/PTY assertions, and rerun both Formular
    terminal-output cases.
  - [x] **Flash Java's semantic negative dialog action before Escape closes the
    modal, 1:1 with Python/C++.** The complete Java direct-terminal smoke now
    passes startup, signal teardown, raw Calculator input, Formular paste/edit/
    selection/output, and WidgetGallery mouse drag/copy, but TaskBoard Escape
    closes its confirmation dialog without first rendering the canonical
    highlighted `No` action. Audit Python dialog/window-stack Escape dispatch,
    C++ `cpp/src/generated/GeneratedWindowRuntime.cpp` modal key/flash/close
    order, and Java `java/src/main/java/uimd/{GeneratedWindowRuntime,
    GeneratedWindowStack,MessageBox}.java`; keep the delay/flash duration in
    centralized runtime settings, add focused state/render coverage, rebuild
    TaskBoard, and rerun the complete Java real-PTY smoke.
  - [x] **Discard delayed Java terminal pixel reports without inserting null
    input events.** Java `InputParser.eventFromEscapeSequence` correctly maps
    CSI 4/6 pixel reports to `null`, but `readAvailableEvents` currently adds
    that null into its event list and `coalesceMouseWheelEvents` dereferences
    it. Match Python/C++ report filtering in
    `src/uimd/runtime/application.py` and `cpp/src/terminal/Input.cpp`, retain
    following real input events, add focused mixed-report coverage, and include
    it in the complete Java direct-terminal smoke.

    Completed on 2026-08-14. Focused Java parser/runtime/dialog/terminal tests
    and Checkstyle passed, the affected C++ and Java applications were rebuilt,
    and `tools/java_direct_terminal_smoke.py` passed all 10 groups. The real PTY
    gate covered startup/title/modes and Ctrl+C/signals, no-focus/raw keys,
    modified and split escape sequences, bracketed paste, multiline selection,
    shared clipboard notifications and expiry, clear/home teardown, semantic
    dialog Escape flash, image diff/quit, exact 34-pixel Sixel cell rows,
    bounded repeated Sixel scrolling, and the sustained 800-report wheel burst
    (Java remained responsive at 0.061 s in that run).
  - [x] **Route Java mouse activation into a dynamic reusable ScrollView child
    1:1 with Python/C++.** After ViewHost descendant edit rendering matched,
    ExpenseTracker passed 20 assertions through adding its first expense, then
    a click on `main.expenses[0]` entered the active ScrollView scope in C++
    while Java retained `edit_mode=false` and the undimmed parent surface.
    Audit Python `src/uimd/runtime/{UIBase,UIScrollView}.py`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::handleMousePress`, and Java
    `java/src/main/java/uimd/{GeneratedWindowRuntime,ScrollView,
    ReusableElement}.java` for nested child hit testing, proxy activation,
    focused-index ownership, scope entry, and post-event cleanup. Keep the fix
    in the shared Java runtime, add focused nested mouse coverage, rebuild both
    ExpenseTracker targets, and rerun the complete compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-052835-step-15-expense_tracker_compare.json`.

    Completed on 2026-08-14. Java now evaluates active ScrollView scope after
    the mouse target has entered it, matching C++ event order instead of
    immediately clearing the newly established scope. Focused nested mouse
    coverage, Checkstyle, and the complete Expense Tracker compare passed.
  - [x] **Restore Java's owning ScrollView scope when MCP `click_element`
    activates a nested editable control, 1:1 with Python/C++.** After physical
    mouse scope entry matched, ExpenseTracker passed 50 assertions through
    nested keyboard navigation and a ComboBox selection, then a second
    `click_element` on `main.expenses[0].category` restored the active scope
    and dimmed the outer surface in C++ while Java left the parent undimmed.
    Audit Python `src/uimd/runtime/mcp.py`, C++ MCP focus/click handling in
    `cpp/src/generated/GeneratedWindowRuntime.cpp`, and Java
    `java/src/main/java/uimd/{McpController,GeneratedWindowRuntime}.java` for
    containing-ScrollView discovery, focus preparation, scope/edit ownership,
    activation order, and cleanup. Add focused nested MCP click coverage,
    rebuild both ExpenseTracker targets, and rerun the complete compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-053203-step-43-expense_tracker_compare.json`.

    Root cause confirmed on 2026-08-14 after the first closed-ComboBox hit-box
    repair: repeated Java `renderFullSurface` calls cumulatively offset a
    ViewHost descendant frame, moving the real ExpenseTracker category from
    `(15,31)` to `(42,115)` before the second click. The resulting point is
    outside the viewport and correctly tears down the ScrollView scope. Port
    C++ `refreshActiveWindowLayoutForMouse` semantics to Java so every MCP
    mouse tool recomputes absolute frames from generated relative layout and
    active stack bounds before hit testing; retain closed ComboBox click
    geometry and add a nonzero nested-host repeated-render regression. Latest
    confirming snapshot:
    `tests/mcp/snapshots/20260814-054423-step-43-expense_tracker_compare.json`.

    Completed on 2026-08-14. Terminal and modal-stack renders now recompute
    absolute descendant frames from generated relative layout after every
    frame, replacing the cumulative Java-only offset walk; MCP click geometry
    remains the closed ComboBox row. A nonzero nested-ViewHost repeated-render
    regression, focused runtime/MCP/stack tests, both Checkstyle tasks, rebuilt
    C++ and Java Expense Tracker targets, and the complete compare passed all
    142 assertions with zero step failures.
  - [x] **Route Java mouse clicks across the expanded ComboBox overlay 1:1
    with Python/C++.** The C++/Java Formular compare passed 34 assertions, then
    an MCP click on the fourth visible dropdown row selected `Hungary` and
    left edit mode in C++, while Java retained `Czech Republic` and the open
    dropdown. Audit Python ComboBox mouse dispatch and C++
    `GeneratedWindowRuntime.cpp::{isFocusedComboBoxEntry,
    comboBoxOptionIndexForLocalRow,handleMousePress}` against Java
    `GeneratedWindowRuntime.java` hit testing. The previously ported overlay
    paint geometry must also be the interactive geometry, with the same
    bounded row mapping, selection notification, and edit cleanup. Add focused
    sibling-overlay mouse coverage and rerun the complete Formular compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-031941-step-33-formular.json`.

    Completed on 2026-08-14. Java now uses the same closed-row normal hit
    frame, expanded edited-dropdown target frame, option-row mapping,
    notification order, and edit cleanup as C++. Focused overlay/sibling mouse
    coverage and Checkstyle passed, followed by all 128 Formular assertions.
  - [x] **Preserve Java's cleared `focusedIndex = -1` during post-event state
    normalization.** After the ComboBox overlay fix, Formular passed 74
    assertions before a blank-area mouse press. C++ committed the current edit
    and left no focused element; Java's empty-target path also sets `-1`, but
    `GeneratedWindowRuntime.java::normalizeState` immediately clamps it back
    to element zero, restoring the first input's focus style. Audit Python
    empty mouse focus cleanup and the C++ generated runtime normalization,
    preserve `-1` while still repairing out-of-range positive indexes, add a
    focused blank-target test, and rerun the complete Formular compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-032455-step-73-formular.json`.

    Completed on 2026-08-14. Runtime normalization now preserves the canonical
    no-focus sentinel and only clears genuinely invalid indexes. Focused blank
    mouse-target commit/focus coverage and Checkstyle passed; the complete
    Formular compare then passed all 128 assertions and every render step.
  - [x] **Match Java fallback image sampling and terminal-color quantization
    to Python/C++ exactly.** The first C++/Java ImageGallery compare reached
    the initial snapshot with equal geometry and half-block characters, but a
    sampled grayscale cell was `#c0c0c0` in C++ and `#e0e0e0` in Java. Audit
    Python `src/uimd/runtime/image.py`, C++ `cpp/src/elements/Image.cpp`, and
    Java `java/src/main/java/uimd/Image.java` for source-region mapping,
    resize/sample coordinates, alpha-over-effective-background, half-block
    pairing, and ANSI palette conversion. Port the same algorithm without
    example-specific color adjustments, add focused image fixtures, rebuild
    C++ and Java ImageGallery, and rerun
    `tests/mcp/image_gallery_compare.yaml` at `--compare-app-size 90x35`.
    Failure snapshot:
    `tests/mcp/snapshots/20260814-032818-step-1-image_gallery_compare.json`.

    Completed on 2026-08-14. The sampling, placement, blending, checker, and
    quantization algorithms were already structurally identical; the mismatch
    came from Java `BufferedImage.getRGB()` applying a grayscale color-space
    conversion that Pillow and stb do not apply. The Java ImageIO adapter now
    expands raw grayscale and grayscale-alpha samples directly to RGB while
    retaining the normal ImageIO path for color images. Focused grayscale,
    alpha, sampling, and image-info tests plus Checkstyle passed; regenerated
    and rebuilt C++/Java ImageGallery then passed all 8 assertions and every
    render step at `--compare-app-size 90x35`.
  - [x] **Emit Java Image click hooks with the same generated public API as
    C++/C#.** While porting ImageBrowser domain logic, the canonical generated
    C++ and C# classes expose and dispatch `onPhotoClick`, `onGal1Click`, and
    equivalent hooks for `image` members, but
    `cpp/tools/uimd/NativeJavaGenerator.cpp::eventSpecs` currently treats only
    Button as a click event. Java runtime mouse dispatch already routes an
    activated Image through its owning generated window; add Image to the same
    native event-spec branch, regenerate the canonical ImageBrowser dependency
    classes, extend native generator parity coverage, and verify both focused
    mouse/activation behavior and the complete C++/Java ImageBrowser compare at
    `--compare-app-size 90x35`. Do not add click workarounds to the Java
    example.
  - [x] **Composite Java reusable-control focus/edit state over its child 1:1
    with Python/C++.** The ImageBrowser compare reaches focused
    `astro_thumb`, where C++ blends the reusable wrapper focus background over
    the child's `#172033` surface (`#3a4152`) while Java leaves the child
    background unchanged. Audit Python `UIElementReusable` rendering and C++
    `ReusableElement`/generated-runtime compositing against
    `java/src/main/java/uimd/{ReusableElement,GeneratedWindowRuntime}.java`.
    Apply state styling in the shared runtime without changing ImageBrowser,
    add focused reusable child coverage, and rerun the complete C++/Java
    ImageBrowser compare at `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-034447-step-5-image_browser_compare.json`.
  - [x] **Dim Java content outside an active ScrollView edit scope 1:1 with
    Python/C++.** After reusable proxy focus parity was restored, the full
    ImageBrowser compare reached Enter on `main.items`; C++ applies the active
    ScrollView scope dim background to every cell outside the viewport while
    Java leaves the parent window unchanged. Audit Python
    `src/uimd/runtime/UIBase.py` active-scope rendering and C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::{dimOutsideActiveScrollView,
    renderGeneratedWindowContent}` against Java
    `java/src/main/java/uimd/GeneratedWindowRuntime.java`. Port the same
    top-window/descendant ownership, color blending, image-half-block handling,
    suppression, and render order in the shared runtime; add focused coverage
    and rerun the complete C++/Java ImageBrowser compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-035836-step-24-image_browser_compare.json`.
  - [x] **Hide Java's internal generated root ScrollView name from public MCP
    element paths.** After active-scope dimming matched, ImageBrowser entered
    the list and C++ reported the first descendant as
    `main.items[0].sel`, while Java exposed
    `main.items.__scrollview[0].sel`. Audit Python MCP element traversal and
    C++ generated-runtime path construction against
    `java/src/main/java/uimd/McpController.java`; preserve reusable prefixes
    and child indexes while treating a generated root `__scrollview` as an
    implementation detail. Add nested path/lookup coverage and rerun the full
    C++/Java ImageBrowser compare at `--compare-app-size 90x35`.
  - [x] **Match Java universal modal-backdrop compositing to Python/C++.**
    After canonical nested MCP paths were restored, ImageBrowser opened the
    FileBrowser from a ScrollView child and C++ rendered the outer surface
    background as `#0f141b`, while Java rendered `#090c10`. Audit Python
    window-stack/modal rendering under `src/uimd/runtime`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp`, and Java
    `java/src/main/java/uimd/{GeneratedWindowRuntime,McpController}.java` for
    backdrop source color, alpha composition, active ScrollView dim-state
    suppression, layer count, and render order. Fix the shared Java runtime,
    add focused modal-over-active-ScrollView coverage, and rerun the complete
    C++/Java ImageBrowser compare at `--compare-app-size 90x35`. Failure
    snapshot:
    `tests/mcp/snapshots/20260814-040959-step-32-image_browser_compare.json`.

    The backdrop layer now matches, and the compare progressed through the
    complete FileBrowser open/close flow. On return, explicitly focusing
    `main.items[1].show_btn` still leaves Java's restored parent ScrollView
    surface at `#131921` while C++ renders `#1f2937`. Include post-modal
    active-scope restoration, descendant focus propagation, and MCP explicit
    focus cleanup in the same audit and regression coverage. Follow-up
    snapshot:
    `tests/mcp/snapshots/20260814-041359-step-42-image_browser_compare.json`.
  - [x] **Match Java ScrollView descendant navigation visibility to
    Python/C++.** After modal-return and explicit-focus cleanup matched, the
    ImageBrowser compare passed 76 assertions and then diverged on the fourth
    consecutive `Down` after explicitly focusing `main.items[0].sel`: C++
    rendered the next focused row's checkbox edge while Java left the same
    cell blank. Audit Python `src/uimd/runtime/{UIBase,UIScrollView}.py`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` plus
    `cpp/src/elements/ScrollView.cpp`, and Java
    `java/src/main/java/uimd/{GeneratedWindowRuntime,ScrollView}.java` for
    descendant spatial movement, remembered focus, `ensureVisible`, scroll
    offset updates, and clipped child rendering. Keep the repair in the shared
    runtime, add focused repeated-descendant-navigation coverage, and rerun the
    complete C++/Java ImageBrowser compare at `--compare-app-size 90x35`.
    Failure snapshot:
    `tests/mcp/snapshots/20260814-041946-step-58-image_browser_compare.json`.

    Root cause confirmed on 2026-08-14: Java
    `ScrollView.maxViewOffset` applies `paddedViewportSize` internally even
    when `render`, `childViews`, and `actualSkip` already pass a padded
    viewport. For the ImageBrowser list this changes the correct natural skip
    from 122 to 124; the fourth descendant `Down` then normalizes 124 to 122
    and applies the requested one-line movement, producing a visible three-row
    jump. C++ `ScrollView::maxViewOffset` accepts an already-normalized
    viewport, while its public scrolling entry points apply padding exactly
    once. Port that caller/callee contract 1:1 across every Java
    `maxViewOffset` path, add a padded-viewport regression that proves render
    and child-view inspection cannot mutate the correct top offset, and rerun
    the focused tests plus the complete ImageBrowser compare. Latest confirming
    snapshot:
    `tests/mcp/snapshots/20260814-043636-step-58-image_browser_compare.json`.
  - [x] **Match Java generated ScrollView focus-surface state after spatial
    key handling to Python/C++.** With padded-offset parity repaired, the full
    ImageBrowser compare passed 152 assertions and reached the gallery view.
    `Right` focused `main.gallery_scroll` identically, but the following
    `Down` made C++ composite the ScrollView focus surface (`#171a25`) while
    Java reverted the same cells to the base `#030712`. Audit Python
    `src/uimd/runtime/{UIBase,UIScrollView}.py`, C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` and
    `cpp/src/elements/ScrollView.cpp`, and Java
    `java/src/main/java/uimd/{GeneratedWindowRuntime,ScrollView,
    ReusableElement}.java` for proxy focus, handled ScrollView keys, active
    descendant scope, focus/edit render-state flags, and post-event cleanup.
    Port the same state transition in the shared Java runtime, add focused
    direct-ScrollView navigation/render coverage, and rerun the complete
    C++/Java ImageBrowser compare at `--compare-app-size 90x35`. Failure
    snapshot:
    `tests/mcp/snapshots/20260814-044615-step-138-image_browser_compare.json`.

    Root cause confirmed with a focused MCP probe: Java moves focus from
    `main.gallery_scroll` back to the overlapping `gallery_mosaic` after
    `Down`, whereas C++ keeps the ScrollView proxy focused. Java
    `GeneratedWindowRuntime.moveSpatial` uses a center-distance heuristic;
    Python `UIBase._directional_focus_score` and C++ `moveFocusSpatial` require
    the candidate's directional edge to be fully beyond the current element
    and rank candidates by `(band, primary gap, perpendicular gap, edge)`.
    Port the complete canonical spatial scoring algorithm and visibility
    follow-up rather than special-casing Gallery, and cover an overlapping
    tall target where center-only scoring chooses an invalid candidate.
  - [x] **Match Java ScrollView overflow-indicator styling to Python/C++.**
    After padded viewport and spatial-navigation parity were repaired, the
    complete ImageBrowser compare passed 156 assertions and reached the final
    explicit gallery scroll. C++ renders the visible top overflow marker `^`
    with foreground `#d1d5db`, while Java renders the same character and
    background with an unset foreground. Audit Python
    `src/uimd/runtime/UIScrollView.py`, C++
    `cpp/src/elements/ScrollView.cpp`, and Java
    `java/src/main/java/uimd/ScrollView.java` for marker source style,
    foreground/background fallback, and raw/cell render order. Port the shared
    style behavior, add focused top/bottom indicator coverage, rebuild both
    ImageBrowser sides, and rerun the complete compare at
    `--compare-app-size 90x35`. Failure snapshot:
    `tests/mcp/snapshots/20260814-045509-step-147-image_browser_compare.json`.

    Completed on 2026-08-14 together with the Image click-hook, reusable-focus,
    active-scope dimming, public nested-path, modal-return, padded-viewport,
    descendant-navigation, and canonical spatial-scoring repairs. Focused Java
    runtime/JUnit coverage and Checkstyle passed, both C++ and Java
    ImageBrowser outputs were regenerated and rebuilt, and the complete
    C++/Java ImageBrowser compare passed all 192 assertions and every render,
    modal, focus, scroll, and image-info step at `--compare-app-size 90x35`.
  - [x] **Match Java's initial generated-window focus state to Python/C++.**
    Python `src/uimd/runtime/UIBase.py::UIBase.__init__` starts with
    `_focused_element = None`, and C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::{GeneratedWindowStack::push,
    runGeneratedWindow}` keeps `focusedIndex = -1` unless an explicit
    `initialFocusName` is valid or `startInEditMode` requires the first
    focusable element. Java
    `java/src/main/java/uimd/GeneratedWindowRuntime.java::initialFocusIndex`
    currently returns element zero unconditionally when no name is supplied,
    so WidgetGallery renders its Quit button in focus style on the first MCP
    frame. Port the same initialization branches, add focused runtime/stack
    tests for default, named, invalid-name, and start-in-edit behavior, then
    rebuild C++ and Java WidgetGallery and rerun its full MCP compare at
    `--compare-app-size 90x35`.
  - [x] **Port generated cell fill/blit compositing to Java 1:1 with C++.**
    The next WidgetGallery compare frame showed a transparent Label replacing
    its already-painted `#162033` parent background with an unset background.
    C++ `cpp/src/generated/GeneratedWindowRuntime.cpp::{fillRect,blit,
    blendBackgroundOverExisting,blendForegroundOverBackground}` composites
    transparent and partial-alpha colors over the existing terminal cell and
    emits background textures, while Java
    `java/src/main/java/uimd/GeneratedWindowRuntime.java::{fill,blit}` either
    copies style fields directly or replaces the complete cell. Port the same
    transparent/unset inheritance, exact-alpha foreground/background blending,
    clipped writes, and texture fill behavior, use the painted buffer cell as
    the scoped element parent background, add focused render tests, and rerun
    the C++/Java WidgetGallery compare at `--compare-app-size 90x35`.
  - [x] **Emit both text gradient style objects from the native Java
    generator.** C++
    `cpp/tools/uimd/NativeCppGenerator.cpp::{textGradientCode,styleCode}`
    preserves `text-color-gradient` and `text-background-gradient` interval,
    step, segment size, and colors, while
    `cpp/tools/uimd/NativeJavaGenerator.cpp::styleCode` currently discards both
    nested mappings even though Java `Style`/`RenderHelpers` implement them.
    Add the structurally equivalent Java emitter/helper code, regenerate
    WidgetGallery from the canonical Python `.uimd`, add native generator
    coverage for both gradient channels, rebuild C++ and Java WidgetGallery,
    and rerun its compare at `--compare-app-size 90x35`.

    Completed on 2026-08-14. The native Java emitter now preserves both
    gradient channels, and `tools/native_uimd_parity.py` verifies their
    interval, step, segment size, colors, and channel order. The focused native
    smoke passed, and the regenerated C++/Java WidgetGallery compare passed all
    72 assertions and every render step.
  - [x] **Honor MCP `snapshot_time_ms` while Java renders gradient
    snapshots.** Python
    `src/uimd/runtime/mcp.py::{tool_get_render_snapshot,
    tool_get_render_snapshot_compact,tool_get_render_cell}` wraps rendering in
    `GradientRenderTime`, and C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp` uses
    `ScopedRenderTimeOverride` in the same three tools. Java
    `java/src/main/java/uimd/McpController.java` currently advertises the
    argument but renders from wall-clock time, so otherwise-identical animated
    gradients disagree during deterministic compare. Add the same scoped,
    restoring override around all three Java tools, cover both restoration and
    deterministic cells in focused MCP tests, rebuild C++ and Java
    WidgetGallery, and rerun its compare at `--compare-app-size 90x35`.
  - [x] **Port Java MCP `NumberInput` text replacement 1:1 from C++.** C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::toolSetText` clears the
    number edit buffer, types each character with the normal key path, commits
    it, synchronizes the numeric value, and restores the active edit text;
    Python `src/uimd/runtime/mcp.py::tool_set_text` has the same externally
    visible replace-and-type contract. Java
    `java/src/main/java/uimd/McpController.java::toolSetText` currently only
    calls `NumberInput.setEditText` for non-paste input, leaving the committed
    value unchanged. Port the C++ event/notification order for both set and
    paste, add focused numeric zero/nonzero coverage, then rerun the complete
    C++/Java WidgetGallery compare at `--compare-app-size 90x35`.
  - [x] **Commit and restart an existing Java edit session before explicit MCP
    `enter_edit_mode`.** Python
    `src/uimd/runtime/{mcp.py::tool_enter_edit_mode,
    UIBase.py::set_focus}` exits and commits the current edit before focusing
    an explicit target, and C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::toolEnterEditMode` performs
    the same cleanup before it captures a fresh snapshot and calls
    `beginElementEdit`. Java
    `java/src/main/java/uimd/McpController.java::toolEnterEditMode` directly
    reuses `beginFrameElementEdit`, so a zero-valued `NumberInput` remains in
    its previous edit buffer instead of selecting/replacing the zero on the
    fresh edit session. Add the structurally equivalent shared runtime cleanup,
    focused repeated-entry state/render coverage, and rerun the complete
    C++/Java WidgetGallery compare at `--compare-app-size 90x35`.
  - [x] **Commit Java non-ScrollView edits before mouse focus moves to another
    element.** Python `src/uimd/runtime/UIBase.py::set_focus` exits the current
    edit session before changing focus, and C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::handleMousePress` commits
    the focused element when a non-ScrollView edit is active and the mouse
    target differs. Java
    `java/src/main/java/uimd/GeneratedWindowRuntime.java::dispatchEvent`
    currently commits only `activeScrollViewEditElement`, then clears the
    ordinary edit snapshot in `changeFocus`; a typed `NumberInput` therefore
    reverts to its old committed value when another control is clicked. Port
    the same pre-focus commit/cleanup order, add focused mouse transition
    coverage, and rerun the complete C++/Java WidgetGallery compare at
    `--compare-app-size 90x35`.
  - [x] **Preserve Java text cursor/selection state across consecutive MCP text
    tools.** Python `src/uimd/runtime/mcp.py::_prepare_text_element` focuses and
    enters edit mode only when necessary, and C++ MCP text/cursor tools in
    `cpp/src/generated/GeneratedWindowRuntime.cpp` preserve an existing edit
    session after `focusActiveWindowElement`. Java
    `java/src/main/java/uimd/McpController.java` currently calls
    `GeneratedWindowRuntime.beginFrameElementEdit` unconditionally for
    `type_text`, clear/cursor/selection helpers, paste, and related paths;
    consequently `set_cursor(0)` followed by `type_text` resets the cursor to
    the end. Add a shared prepare-if-needed transition with the same focus,
    ScrollView edit-target, snapshot, and notification semantics, use it for
    consecutive text operations while retaining explicit `enter_edit_mode`
    restart behavior, add focused cursor/selection sequencing coverage, and
    rerun the complete C++/Java WidgetGallery compare at
    `--compare-app-size 90x35`.
  - [x] **Port focused ComboBox dropdown overlay layout to Java 1:1 with
    C++.** C++
    `cpp/src/generated/GeneratedWindowRuntime.cpp::{renderEntry,
    isFocusedComboBoxEntry,renderGeneratedWindowContent}` expands an edited
    ComboBox to the bounded dropdown height, extends its content clip, and
    defers that entry until siblings have rendered so the menu overlays rows
    below it. Java
    `java/src/main/java/uimd/GeneratedWindowRuntime.java` currently renders
    every entry in source order at its closed one-row size and clips the
    remaining `ComboBox.render` rows to the source cell, leaving underlying
    labels visible where C++ shows options. Port the same constants, expanded
    frame/row resolution, clip extension, and deferred paint order; add focused
    sibling-overlay coverage and rerun the complete C++/Java WidgetGallery
    compare at `--compare-app-size 90x35`.

    Completed on 2026-08-14 together with the initial-focus, cell-compositing,
    deterministic-gradient-time, NumberInput edit, mouse-focus cleanup,
    consecutive-text-tool, and ComboBox overlay repairs. Focused JUnit tests
    and both Checkstyle tasks passed, then the regenerated/rebuilt C++ and Java
    WidgetGallery comparison passed all 72 assertions and every render step at
    `--compare-app-size 90x35`.

  - The native generator is implemented in
    `cpp/tools/uimd/NativeJavaGenerator.{hpp,cpp}`, registered in the native
    CMake/CLI surface, and supports Java package emission through
    `--java-package`. The Java 17/Gradle 9.7 wrapper project and generated
    sources remain under the single `java/` ownership tree.
  - The Java terminal shell, core buffer/styles/layout, controls, generated
    runtime, sustained-wheel coalescing, Sixel/fallback image path, reusable
    ScrollView focus scope, universal modal stack, MessageBox variants, and
    FileBrowser are implemented against the Python semantic and C++ structural
    paths recorded above. Canonical dialog UIs are generated from
    `src/uimd/dialogs`; canonical example UIs are generated from
    `python/examples` rather than copied Java-specific `.uimd` files.
  - Focused runtime, image, input, nested-focus, modal-stack, standard
    dialog/FileBrowser, and MCP controller/transport tests pass together with
    Java Checkstyle. The Java MCP runtime now uses the universal top-window
    stack, shared UI lock, controlled render coordination, and stdio/TCP/HTTP
    transports. A real C++/Java calculator compare passes all 10 assertions
    and every render step at `--compare-app-size 90x35`.
  - Example domain logic, the regression corpus, SDK/rebuild/test integration,
    direct-terminal/MCP gates, and the mandatory final audit/remediation cycle
    are complete. Only the canonical full-test gate remains before the target
    can be marked complete.

  **Implementation order:**
  1. Audit the current Python/C++/C# runtime and native target registration;
     choose and document one reproducible Java toolchain/build contract
     (Gradle is the expected candidate, but verify source-checkout, installed
     SDK, Windows, macOS, and Linux behavior before locking it in). Add `java`
     to native `generate`, `new`, target validation, `sdk install-target`, SDK
     auto-install, `doctor`, packaging, artifact manifests, native parity
     checks, and exact command documentation.
  2. Implement the terminal shell vertically: entry/teardown, raw mode,
     alternate screen, autowrap, resize, physical terminal-cell metrics,
     keyboard/CSI/SS3/bracketed-paste/SGR mouse parsing, clipboard, full and
     differential frame presentation, and clean Ctrl+C/signal exit. Keep every
     unavoidable JVM/OS primitive in the smallest adapter and record it here.
  3. Port the shared runtime structure and state transitions 1:1: geometry,
     colors/styles/text visual rules, base element/control ownership, Label,
     Button, CheckBox, TextInput/TextArea, NumberInput, ComboBox, ListBox,
     InfoLabel, MessageTable, FrameBufferView, Image, ScrollView, reusable
     elements/ViewHost, generated window/modal stack, focus/edit/selection,
     mouse capture, post-event cleanup, and centralized clipboard.
  4. Port the generated public API and MCP runtime: typed generated members,
     relevant override hooks only, shared `.uimd` dependencies, app tools,
     schemas/snapshots, top-window routing, controlled rendering, transports,
     delays/progress synchronization, and cleanup order. Implement standard
     MessageBox and FileBrowser objects in the shared Java runtime rather than
     examples.
  5. Prove one small vertical example first (`calculator`), then `formular`,
     `text_editor`, a reusable/ScrollView/dialog example, and finally
     `image_gallery`/`image_browser`. After the vertical slice is sound,
     generate and implement the complete existing example set from the same
     canonical Python `.uimd` sources. Never add Java-only layout values,
     sleeps, callbacks, focus resets, image fallbacks, or MCP accommodations.
  6. Inspect and port every applicable app under
     `tests/regressions/uimd/parity`, including at least
     `source_separator_scroll` and `stale_scrollview_focus`; build and compare
     each C++/Java regression at `--compare-app-size 90x35` before considering
     the target complete.

  **Image/performance requirements:** treat the recently repaired image path as
  a first-class Java gate, not an optional final snapshot. Match C++ visible
  geometry, contain/cover/stretch crop mapping, alpha/background rules,
  fixed-palette Sixel output, exact physical cell-pixel height, row-segmented
  raw anchors, scroll-margin protection, cache bounds/identity, modal/resize
  invalidation, dependency diagnostics, and fallback behavior. Port the C++
  consecutive mouse-wheel coalescing rule before dispatch (latest position,
  summed delta clamped to +/-12, exact net-zero removal, all non-wheel
  boundaries preserved) so Java cannot repeat the Rust/Go backlog. Extend the
  existing real-PTY smoke to cover Java Sixel row geometry, repeated scrolling,
  modal images, bounded output, and the sustained 800-report wheel burst.

  **Required completion gates:** regenerate and build both C++ and Java for
  every affected example; add Java runtime/generator/installed-SDK tests;
  compile and run the complete Java unit suite and strict lint/static checks;
  extend the canonical artifact manifest and `./tools/rebuild_all.sh` /
  `./tools/test_all.sh` with honest Java phases; run direct-terminal and MCP
  transport smoke; run the complete C++/Java example compare and every Java
  parity regression with `--compare-app-size 90x35`; inspect failures before
  changing tests or snapshots; audit generated `.uimd` source identity and the
  complete Java implementation structurally against Python/C++; run
  `git diff --check`; and finish with the canonical full test. Do not report
  Java complete merely because it compiles or because aggregate snapshots are
  green. Keep changes uncommitted until the user explicitly requests a commit,
  then follow the patch-version and `sdk-work` commit rules.

Unrelated long-term and historical work is tracked in
[`LONG-TASK.md`](LONG-TASK.md).
