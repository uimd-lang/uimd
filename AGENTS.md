# AGENTS.md - UI Compiler Project

## Last Update

- Updated by: Codex (GPT-5)
- Date: 2026-06-12

## Strict Rules

- **LOG SUBSTANTIVE TASKS AND BUGS TO TODO.md**: Before starting work on a substantive bug, feature, refactor, cross-platform/parity-sensitive change, architecture cleanup, analysis that may affect implementation, or multi-step validation task, check whether it is already recorded in `prompts/TODO.md`. If it is not, add it under the appropriate section before starting work and mark it as `[x]` when done. Do not add TODO entries for trivial one-off housekeeping such as checking a file, removing obvious conflict markers, fixing a typo, or other small documentation cleanup unless it exposes a real bug, design decision, parity issue, or follow-up task that should remain visible across sessions.
- **NO UNREQUESTED COMMITS**: Committing any changes without an explicit user request is strictly forbidden. Never commit on your own. Always ask first. Commit only when the user gives an explicit command such as "commit", "make a commit", or "commit please".
- **STRICT COMMIT BAN WITHOUT USER REQUEST**: Never run `git commit` unless the user explicitly asks for a commit in the current conversation. Finishing an implementation, passing tests, preparing a release artifact, updating documentation, or seeing a clean staged diff is not permission to commit. If the user has not explicitly requested a commit, leave the changes uncommitted and report the working tree state.
- **Compilation**: Whenever the user asks you to compile or run a `.uimd` file or compatible `.md` file in a source checkout, use the universal repo entry point `./uimd`, never the legacy `python3 compile.py` or `python3 tools/compile_cpp.py` wrappers. For Python generation use `./uimd generate <path> --target python`; for C++ generation use `./uimd generate <path> --target cpp`; for running a Python app use `./uimd run <path>`. Always regenerate, even if generated files already exist. If the user asks for a simple compile-and-run command, show exactly two lines in sequence and no explanation: 1) the compile-and-run command (`./uimd run <path>`), 2) no further explanation.
- **NO HARDCODING**: Never use hardcoded numbers such as 80, 24, etc. as default values. All default values must be defined as constants, for example `FIT_CONTENT`. If you need a fallback, compute it from content or layout.
- **NO RUNTIME DESIGN CONSTANTS**: NOTHING THAT IS NOT IN THE `.uimd` FILE MAY AFFECT WINDOW DESIGN. NO MANUAL `PADDING`, `MARGIN`, `WIDTH`, `HEIGHT`, `ROW`, `COL`, `MIN_*`, `HEADER_*`, OR SIMILAR CONSTANTS IN PYTHON/C++ EXAMPLE LOGIC MAY MOVE, SHRINK, GROW, OR OTHERWISE ADJUST UI ELEMENTS. Window design belongs exclusively to the `.uimd` layout and styles. Runtime may handle only behavior and data.
- **NO EXAMPLE-SPECIFIC FIXES**: Never add logic, constants, branches, callbacks, delays, fallbacks, or other behavior to examples or specific applications when the issue belongs to general runtime, rendering, MCP, focus, input, dialog, modal, layout, style, or platform-parity behavior. Examples may contain only their own domain application logic and data. All general UI/runtime behavior must be fixed in shared runtime, generator, shared configuration, or specification code, consistently across all platforms.
- **POST-EVENT CLEANUP IS RUNTIME RESPONSIBILITY**: Every user action, MCP action, key event, mouse event, activation, focus move, selection change, scroll, modal transition, or reusable-control event must leave runtime state fully consistent before rendering, snapshots, or tests inspect it. Stale focus, edit mode, selection, scroll scope, modal/window stack, mouse capture, reusable child focus, or descendant state must be cleaned in the shared runtime at the point where the state changes. Tests may assert the cleanup happened, but tests, examples, MCP scripts, and app-specific code must never clean up stale runtime state on behalf of the runtime or add waits/workarounds that mask missing cleanup.
- **NO DUPLICATE IMPLEMENTATION CODE**: Never create a second implementation source tree for the same runtime, compiler, generator, tool, dialog, or example behavior. Every behavior must have exactly one canonical source. Compatibility files are allowed only when they are thin wrappers, import shims, forwarding entry points, or generated copies that contain no independent behavior. If a duplicate implementation already exists, do not edit both copies as a "sync" fix; first identify the canonical source, convert the other location to wrappers or generated output, and record the cleanup in `prompts/TODO.md`.
- **EXTREMELY IMPORTANT - NATIVE UIMD TOOL IS CANONICAL FOR COMPILER/CLI**: The SDK-facing compiler, generator, scaffold, doctor, inspect, run launcher, and `mcp-test` command behavior lives in the native C++ tool under `cpp/tools/uimd`. Do not recreate or extend a Python compiler/CLI implementation. `./uimd` is the source-checkout entry point and dispatches to the native binary. Python remains a supported generated target and runtime platform, but not the canonical compiler/CLI implementation.
- **PYTHON RUNTIME REMAINS THE REFERENCE FOR SHARED RUNTIME BEHAVIOR**: For behavior that exists in both Python and C++ runtimes, read the Python runtime implementation under `src/uimd/runtime` first and keep the C++ runtime behavior equivalent unless the user explicitly approves a documented platform-specific exception. This applies to rendering, focus, input, scrollview, dialogs, reusable elements, MCP runtime behavior, window/modal stack behavior, and user-visible runtime APIs.
- **WRITE THE SAME BEHAVIOR CODE FOR BOTH PLATFORMS - NO UNIQUE PLATFORM LOGIC**: When a behavior exists on both Python and C++, the implementation MUST be the same behavior, same algorithm, same state machine, same cleanup order, same render/blending rules, same focus/navigation rules, same MCP contract, and same edge-case handling on both platforms. It is strictly forbidden to write unique one-platform behavior code that differs from the other platform, even if it makes one failing test pass. A platform-specific implementation detail is allowed only for unavoidable language/OS primitives and only when the public behavior remains identical. If behavior must intentionally differ, the user must explicitly instruct that this parity rule is ignored for that specific change, and the exception must be recorded in `prompts/TODO.md`.
- **MANDATORY PARITY GATE FOR CROSS-PLATFORM CHANGES**: Any change touching runtime behavior, rendering, focus, scrollview, reusable elements, MCP tools, event handling, dialogs, compiler/generator output, generated public APIs, or examples that exist on more than one platform is a parity-sensitive change. For runtime behavior, identify the Python runtime path/function/state transition and the corresponding C++/other-platform path in `prompts/TODO.md`. For compiler/generator/CLI behavior, identify the native `cpp/tools/uimd` path and the generated outputs or command contract being protected; do not depend on a removed Python compiler oracle. During the edit, keep the same architecture, state fields, event order, cleanup points, rendering rules, edge cases, and public API unless a documented platform primitive makes that impossible. A one-platform fix is forbidden unless the TODO explicitly records why the other platform is unaffected. Before reporting the work as done, update the TODO with the parity decision, regenerate/build the affected outputs for both platforms when generated code is involved, and run or record the relevant compare/parity test. If a compare failure appears after a parity-sensitive change, assume an implementation parity bug first; do not change tests, snapshots, examples, waits, or masks until both platform implementations have been audited against the relevant canonical implementation.
- **EXTREMELY IMPORTANT - UIMD SOURCES MUST BE IDENTICAL ACROSS LANGUAGE VERSIONS**: For the same UI, example, tool, runtime dialog, reusable component, or tester shell, every `.uimd` source used by Python and by other programming-language implementations such as C++ MUST be identical. Python `.uimd` files are the reference source. Do not create a separate C++/other-language `.uimd` variant with different layout, styles, members, metadata, names, or behavior. If another language needs generated code, generate it from the same `.uimd` source or from an exact synchronized copy whose contents are byte-for-byte equivalent except for unavoidable repository path mechanics. Any difference between Python `.uimd` and another language's `.uimd` for the same UI is a bug that must be fixed before continuing.
- **SAME ARCHITECTURE ACROSS PLATFORMS**: Python and C++ runtime must use the same architectural model. If Python uses a window/modal stack, C++ must implement it the same way. Do not add C++-specific workarounds that compensate for missing runtime architecture.
- **SAME PUBLIC API AND CLASSES**: Public user-facing APIs, public classes, methods, and properties used by the application author or generated code must have the same names, meaning, and behavior in Python and C++. This applies to runtime classes, elements, dialogs, generated UI member names, event/callback names, MCP API, and the `.uimd` format. Internal helpers, backend-specific emitters, and private implementation functions do not need to be ported blindly by name if the public contract and behavior are demonstrably the same.
- **PREFERRED EVENT API**: For user application logic, prefer generated inheritance/override event hooks over manual global callback wiring. Examples and scaffolded apps should favor methods such as `onHelloButtonClick()` on a subclass of the generated UI class, while lower-level callback configuration such as `GeneratedWindowRuntimeOptions` may remain available for advanced use. New platform backends should implement this override style as their primary user-facing event API and keep event names aligned across platforms. Generate hooks only for event-capable elements and only for relevant events by element type, for example button click, checkbox change, input change/submit, combobox change, and listbox selection change; do not generate every possible event for every element.
- **ALLMAN BRACE STYLE**: For brace-based languages such as C++, C#, Java, and JavaScript, generated examples, scaffolded applications, and new hand-written code should use Allman style by default: opening `{` on its own line for classes, functions, conditionals, loops, and lambdas. Keep existing files internally consistent when editing older code with a different established style.
- **UNIVERSAL WINDOW/MODAL STACK**: Dialogs, popups, and modal windows must not be handled for MCP or rendering through app-specific bridge callbacks such as `activeMcpWindow`, `syncMcpFocus`, `mcpEditStarted`, or similar per-example mechanisms. The C++ runtime must have a universal window/modal stack, and MCP tool calls must always operate on the top active window in the same way as the user.
- **CENTRALIZED RUNTIME CONSTANTS**: Runtime timing constants such as delay/flash durations for closing dialogs are allowed only in centralized settings/runtime configuration, not in individual examples. Values must be named, documented, and the same across platforms.
- **UPDATE example_cli_commands.md FOR EVERY NEW EXAMPLE**: Whenever a new example is added under `python/examples/` or `cpp/examples/`, its run command MUST be added to `docs/example_cli_commands.md` in alphabetical order under the appropriate section (Python Examples, C++ Examples). For a Python-only addition, add the Python command and add a placeholder C++ command using `./uimd generate ... --target cpp && cmake --build cpp/build --target ... && ...`. Never leave a new example without a corresponding entry in this file.
- **UPDATE example_cli_commands.md FOR EVERY NEW TEST**: Whenever a new test is added or renamed (including `tests/mcp/*.yaml`, `python/tests/*`, C++ tests, smoke tests, or tool/tester checks), its exact run command MUST be added to `docs/example_cli_commands.md` under the appropriate test section. This file must list all test entry points. For MCP compare tests, always document commands with `--compare-app-size 90x35` and keep per-app commands in alphabetical order.
- **ALWAYS REBUILD ALL EXAMPLES AFTER LIBRARY FIXES**: After every fix in the library/runtime/compiler/generator/shared behavior, first regenerate all `.uimd` UI files for all supported platforms and then build all examples for all platforms. Building only the specific example where the bug appeared is not enough.
- **REBUILD BOTH PLATFORMS AFTER CROSS-PLATFORM EXAMPLE TEST FIXES**: When a bug is found while testing an example across two platforms, after fixing it always regenerate that example's `.uimd` outputs and build that example's source for both platforms before reporting the fix as done.
- **MCP COMPARE VIEWPORTS**: When running compare tests from CLI, scripts, or any automated context, always pass `--compare-app-size 90x35` to guarantee consistent and reproducible per-app viewport sizes regardless of the terminal environment (`180x35` total for Python/C++ side-by-side). Do not rely on auto-split terminal size in automated runs. When a compare snapshot fails, always provide the snapshot viewer command printed by the tester so the failure can be inspected from the recorded side-by-side terminal snapshot.
- **VERSIONING BEFORE COMMIT AND MAIN PUSH**: Before every commit, evaluate whether the version surfaces must be updated as part of the same change set. The project uses `0.x.y` while the public API is not stable. The patch number (`y`, for example `0.3.1 -> 0.3.2`) is the normal increment for bug fixes, performance tuning, small refactors, compatible behavior changes, documentation updates, packaging fixes, and ordinary new functionality. The middle number (`x`, for example `0.3.y -> 0.4.0`) is evaluated only immediately before a user-requested push to `main`, not during ordinary `sdk-work` commits. Before every `main` push, inspect the accumulated changes since the last `main` push and tell the user whether a middle-number bump is recommended, with concrete arguments such as a new supported platform, a new UI element, a major SDK installation milestone, or a substantial change to existing functionality. If the user approves the middle-number bump, run `python3 tools/set_version.py <new-version>` with the patch number reset to `0`, commit that version update, then push both `main` and `sdk-work` as requested. If the user does not approve, do not bump the middle number. Do not bump the middle number just because a single ordinary feature was added. If any version changes, use `python3 tools/set_version.py <version>` so all tracked Python package, CMake, C++ runtime, generated C++ fallback, changelog, and release documentation version surfaces are synchronized. For SDK installation work, keep the documented alpha status aligned with the actual supported installer/release-artifact state before committing.
- **PUBLIC SDK RELEASE REQUIRES GITHUB RELEASE ASSETS**: If the user asks to publish, release, update, or make an SDK version installable through the public `curl .../releases/download/v<version>/install.sh | sh` flow, commit and branch pushes are not enough. After the relevant version commit is on `main`, always run `python3 tools/package_sdk_release.py --build --output dist/sdk-release`, verify the local `install.sh` flow, create or update the matching GitHub Release for tag `v<version>`, upload exactly the matching versioned assets (`install.sh`, `install.ps1`, `checksums.txt`, `checksums.txt.minisig`, `uimd-init-<version>-<platform>`, and `uimd-sdk-<version>-<platform>.tar.gz` for the built platform slice such as `macos-arm64` or `macos-x86_64`), and run a public download/install smoke test from the GitHub Release URL. Do not tell the user that an SDK version is installable from the internet until the public release URL has been tested successfully.
- **MAIN RELEASE AND TAG FLOW**: For a new public UIMD version, explain the flow to the user in concrete terminal commands whenever authentication, tokens, or other user-only secrets block Codex from pushing. A Git tag is not a search keyword and not a branch; it is a fixed named pointer to one exact commit. `v0.4.0` means "this exact commit is release version 0.4.0". Branches such as `sdk-work` and `main` move forward with new commits; a published release tag should stay fixed. The normal flow is: finish and commit the release candidate on `sdk-work`; ask the user to run `git push origin sdk-work`; after explicit user approval for a stable main update, ask the user to run `git push origin sdk-work:main`; then create the version tag on that same commit with `git tag v<version>` and publish it with `git push origin v<version>`. Example: `git tag v0.4.0` then `git push origin v0.4.0`. Before giving tag commands, verify that local `sdk-work`, `origin/sdk-work`, and `origin/main` point at the intended release commit with `git log -1 --decorate sdk-work origin/sdk-work origin/main`. If the tag already exists locally or remotely, stop and inspect with `git tag --list v<version>` and `git ls-remote --tags origin v<version>`; do not move, delete, or overwrite a published tag unless the user explicitly requests that exact operation. After the tag is pushed, tell the user that GitHub Actions will automatically run workflows from `.github/workflows/` on GitHub's clean runners. A failed Actions email means remote CI failed; it does not mean the local push failed. Investigate and fix CI on `sdk-work`, then publish a normal follow-up commit or patch release instead of hiding or deleting tests.
- **RELEASE SIGNING REQUIRES USER-ENTERED SECRETS**: Never ask the user to paste the minisign private-key password into chat, never log it, and never try repeated non-interactive password attempts. If `tools/package_sdk_release.py` reaches `minisign -S` and fails at `Password:` or `Wrong password for that key`, stop and give the user exact terminal commands to run locally. To test the password without rebuilding the whole SDK, tell the user to run one chained command so success is not printed after a failed signature: `cd /Users/marekdubovsky/Projects/uimd && printf 'uimd signing password test\n' > /tmp/uimd-minisign-test.txt && minisign -S -s /Volumes/DUBOVSKY/projects-signing/uimd/uimd-release.key -m /tmp/uimd-minisign-test.txt -x /tmp/uimd-minisign-test.txt.minisig && minisign -Vq -p signing/uimd-release.pub -m /tmp/uimd-minisign-test.txt -x /tmp/uimd-minisign-test.txt.minisig && echo 'minisign password ok'`. After the user confirms that command succeeds, rerun `python3 tools/package_sdk_release.py --build --output dist/sdk-release`.
- **COMMIT MESSAGES IN ENGLISH**: Every commit message MUST be written in English, regardless of the language the user communicates in. This applies to the change summary and any body text. The only non-English token allowed is the `LLM: <model>` footer value (the model name itself). Never write a commit message in Slovak or any other language.
- **Commit**: Never commit changes without an explicit user request. Always ask first whether the user wants to commit. When the user asks for a commit ("commit", "make a commit", "commit please"), use Git and commit only the intended files. Before committing, run `git status --short --branch` and confirm the active branch is `sdk-work`; if the checkout is on `main`, create or switch to `sdk-work` before committing. Do not put WIP commits on `main`. The change summary MUST be in English (see **COMMIT MESSAGES IN ENGLISH**). Add a footer at the end of the commit message with the LLM model that worked on the changes. Format: English summary, blank line, `LLM: <model>`. Example: `git commit -m "Fix InfoLabel element-level color not applied from YAML members" -m "LLM: qwen3.6-35b-a3b-ud-mlx"`.
- **CORRECT COMMIT WORKFLOW**: When a commit is requested, do not start by attempting `git commit` or ad-hoc stash commands. First run `git status --short --branch`. If the branch is not `sdk-work`, switch to `sdk-work` before editing whenever possible. If changes were already made on `main`, save the intended tracked-file diff with `git diff --binary -- <files> > /private/tmp/<task>.patch`, verify the patch is non-empty, restore only those intended files on `main`, switch/create local `sdk-work` from `origin/sdk-work`, apply the patch with `git apply --3way`, resolve conflicts deliberately, and then stage only the intended files. If conflicts appear, finish resolving them before staging or committing. Never use `git stash` as a blind recovery step after branch mistakes or conflicts, and never leave unresolved conflict markers in files.
- **Find What Changed**: Whenever the user asks to find the latest changes ("find what changed", "what was changed last", etc.), run `git log -1 --stat` and summarize the latest commit.
- **Revert**: Whenever the user asks for a revert ("revert", "roll back", "undo changes", etc.), ask: "Do you want to revert all uncommitted changes?" After confirmation, inspect `git status --short`, restore tracked files with `git restore --worktree --staged .`, and remove untracked files only when the user explicitly confirms that cleanup too.
- **STRICT MAIN BRANCH PUSH BAN**: Never push to the `main` branch unless the user explicitly and directly asks for a `main` push in the current conversation. This is a strict prohibition. Do not infer permission from repository setup, documentation updates, release preparation, cleanup, or a general request to publish work. When in doubt, do not push to `main`; use `sdk-work` or ask first.
- **GitHub Repository**: Use Git for the GitHub working copy. The intended remote is `https://github.com/uimd-lang/uimd.git`. `main` is the stable public snapshot branch; `sdk-work` is the working branch for SDK/publishing checkpoints and partial implementation work. Do not commit or push WIP directly to `main`; use `sdk-work` unless the user explicitly asks for a stable `main` update.
- **Border In MD**: ASCII border (`+`, `-`, `|`) in a `.uimd` UI block is intended ONLY for the visual designer and MUST NOT be transferred to elements. Elements such as Label, TextInput, Button, etc. have and must have no border properties derived from the ASCII grid. The border is only a visual layout/design aid, and runtime ignores it.
- **UI EXPANSION MARKERS**: In `.uimd` `## User Interface` ASCII layout blocks, the star expansion marker (`*`) must never appear more than two times consecutively in one row or more than two times vertically in one column. Use at most two adjacent/stacked star markers to request expansion, then use ordinary empty space or `.` layout filler for additional visual rows or columns.
- **NO DUPLICATES**: Creating duplicate member variables, helper indexes, or other member variables whose only purpose is to simplify or speed up an algorithm is strictly forbidden. Every property must have a clear purpose and must not exist only as a cache or index for accessing other data. If data can be computed from existing fields, compute it instead of storing it redundantly.
- **NO ABSOLUTE POSITIONS**: Creating variables for absolute positions is strictly forbidden when variables for relative positions already exist. Always compute absolute position dynamically from existing relative values.

## Project Description

This project is a compiler and runtime for terminal UIs defined in UIMD (`.uimd`) files. It parses the `## Metadata`, `## Definition`, `## Members`, `## Style`, and `## User Interface` sections and the ASCII grid layout from `.uimd` files, generates Python classes with components (Label, TextInput, NumberInput, Button, CheckBox, ComboBox, ListBox, UIElementReusable), and renders them in the terminal with borders (`+`, `-`, `|`), focus management (Tab/Shift+Tab, Enter, Escape), scrolling (UIScrollView), mouse text selection (label `user-select`), and a window stack.

## UI Documentation

- Use [UI_RULES.md](UI_RULES.md) when designing new `.uimd` UIs

## File Map

```
ui/
├── uimd                  # Universal source-checkout CLI wrapper
├── src/uimd/             # Canonical package source for the UIMD library
│   ├── runtime/          # CANONICAL Python runtime implementation
│   ├── dialogs/          # Packaged standard dialog .uimd sources
│   └── testing/          # Packaged tester UI .uimd sources
├── python/
│   ├── runtime/          # Legacy compatibility wrappers only; no behavior here
│   ├── dialogs/          # Dev/example standard reusable dialog windows
│   ├── tests/            # Python tests
│   └── examples/         # Python examples and generated *_ui.py files
├── shared/
│   ├── themes/           # Themes (section-based .md files without a ```ui``` block)
│   ├── ir/               # Backend-neutral IR documentation
│   └── specs/            # Format/layout/runtime behavior specifications
├── cpp/
│   ├── include/ui/       # C++ runtime public headers
│   ├── src/              # C++ runtime implementation
│   ├── examples/         # C++ examples and generated UI code
│   ├── dialogs/          # C++ generated standard dialog code
│   └── tools/            # Native UIMD CLI, bootstrapper, and MCP tester
├── tools/                # Development helpers and rebuild scripts
└── prompts/              # Pseudocode and specifications
```

## Project Structure Ownership

- `src/uimd/runtime` is the single canonical Python runtime implementation.
- `python/runtime` is a legacy compatibility layer for old generated examples and tools that still import `runtime.*`. Files there must only forward to `uimd.runtime.*`; they must not contain independent runtime behavior.
- Runtime fixes, rendering fixes, MCP fixes, focus/input fixes, dialog behavior, and shared Python behavior must be edited in `src/uimd/runtime` only.
- C++ runtime behavior lives in `cpp/include/ui` and `cpp/src`. It is a language port of the Python runtime contract, not a duplicate Python runtime tree.
- Compiler/generator/CLI behavior belongs in the native C++ tool under `cpp/tools/uimd`. Do not add new Python compiler, generator, or CLI implementation code.
- If two source trees appear to implement the same behavior, stop and resolve ownership before editing. Pick the canonical source, turn the other tree into wrappers/generated output, and document the cleanup in `prompts/TODO.md`.

## Current System State

### Focus And Edit Mode (Navigation)
- **Navigation mode** (default): Tab/Shift+Tab and arrows move focus between elements
- **Edit mode**: Enter enters edit mode for the active element; Escape exits
- In edit mode, all keys go to the element; arrows navigate only when the element does not return `True`
- ComboBox: Enter opens the dropdown, the next Enter selects an item and **automatically** exits edit mode
- CheckBox/Button: Enter in navigation mode activates directly (does not enter edit mode)

### Element Style States
Each element has these styles (all `None` values fall back to the base style):
- `style` - base style
- `focus_style` - when the element is focused (navigation mode)
- `edit_style` - when the element is in edit mode
- `cursor_style` - style of the character under the cursor (TextInput in edit mode)
- `selected_style` - selected items (ComboBox, ListBox)
- `checked_style` / `unchecked_style` - CheckBox indicator
- `error_style` - validation error

### Style Prefixes In YAML
In the `## Style` YAML block, prefixes map as follows:
- `focus-background`, `focus-color` -> `focus_style`
- `edit-background`, `edit-color` -> `edit_style`
- `cursor-background`, `cursor-color` -> `cursor_style`
- `selected-background`, `selected-color` -> `selected_style`
- `checked-background`, `checked-color` -> `checked_style`
- `unchecked-background`, `unchecked-color` -> `unchecked_style`

### Themes (Theme System)
- Themes are `.uimd` files in `shared/themes/` with `## Metadata` and `## Style` sections, but without a `\`\`\`ui\`\`\`` block
- `include: dark` in `## Style` loads styles from `dark.md` (searches in the file directory first, then in `shared/themes/`)
- Local styles override included styles (per-selector merge)
- If a file has no `## Style` section, `dark.md` is applied automatically

### Standard Dialogs
- Standard dialog windows live in `python/dialogs/`, not in `python/examples/`
- Available classes are imported with `from dialogs import ...`
- `MessageBoxOk`, `MessageBoxOkCancel`, `MessageBoxYesNo`, and `MessageBoxYesNoCancel` have configurable `header`, `message`, and `on_close(result)` callback
- `FileBrowser` is a reusable dialog for `mode="open"` and `mode="save"`; `open` returns an existing file, `save` returns the path from the `File:` field
- If an example uses a dialog through a `.uimd` dependency, its `uses:` in `## Definition` should point to `../../dialogs/<dialog_name>` or another relative path to `python/dialogs/`, so the dialog is compiled in place and the import path is generated automatically

### Per-character ANSI rendering
`UIWindow._render()` parses ANSI text character by character through `_parse_ansi_chars()` - each character in an element may have a different color. This enables cursor highlight, selection highlight, etc. without losing color information.

### TextInput - Cursor And Selection
- Cursor = character at `cursor_pos` styled with `cursor_style`
- Shift+Left/Right/Home/End = text selection; selected text uses `cursor_style` (same as cursor)
- Scroll offset is maintained automatically so the cursor is always visible

### ListBox - Scrolling
- Arrows automatically move `_scroll_offset` so the selected item is always visible
- `^` on the first visible row means there are items above; `v` on the last visible row means there are items below

### UIScrollView - Scrollable Container
- `extends: uiscrollview` in `## Definition` - UIScrollView class (extends UIControl extends UIBase)
- Viewport = cell named `panel` or `viewport` in the ASCII grid
- `@panel { gap: N }` = N empty rows between children
- Children are `UIControl` instances; they are added at runtime through `_add_child()`
- Scrolling: mouse wheel; during drag-select outside the viewport = auto-scroll (tick mechanism)
- Default scroll position is the top. `_auto_scroll = True` or
  `scroll_to_bottom()` explicitly keeps the newest/end content visible.

### UIElementReusable - Embedded Components
- `type: <name>` in members where `<name>` is not a built-in type = UIElementReusable embedding another `.uimd`
- It never receives window-level focus through mouse click
- In a window with a single interactive element, clicking UIElementReusable/label does NOT clear focus
- `tick()` propagation: `UIBase.tick()` -> `_child_instance.tick()` -> `UIScrollView.tick()` (auto-scroll step)

### Label - Mouse Text Selection
- `user-select: text` in style enables mouse-drag text selection
- Selection extends across multiple labels within a single drag (including UIScrollView children)
- Pressing any key clears selection in all labels (including labels in child instances)
- `word-wrap: true` (default) - long lines and `\n` are wrapped correctly; fit-content height matches

### Compilation - Subdirectory Support
- `uses:` in `## Definition` declares dependencies; the compiler searches for `.uimd` first and legacy `.md` second recursively (sibling -> subdir -> parent -> glob)
- Every dependency is compiled **in place** (into its own directory)
- Generated `_ui.py` gets `sys.path.insert` automatically for every dependency in another directory
- `./uimd generate <dir> --target python` generates Python UI files recursively in the directory
- `./uimd generate <dir> --target cpp` generates C++ UI files recursively in the directory
