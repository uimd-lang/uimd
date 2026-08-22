# New Language Or Platform Workflow

Use this workflow before adding a generated language target, runtime port,
platform backend, or full example set. It captures the C# implementation
approach in a repeatable form: shared sources, structural parity, small vertical
slices, compare-driven validation, and no example-specific fixes.

When an individual defect, regression, or failing test is discovered during
the port, also follow `docs/cross-platform-bug-fix-workflow.md` for that fix.

## Core Rule

The new target is a port of the existing UIMD behavior, not a new design. Python
runtime behavior under `src/uimd/runtime` is the reference for shared runtime
semantics. C++ under `cpp/include/ui`, `cpp/src`, and `cpp/tools/uimd` is the
parity-validated native implementation and the canonical compiler/CLI surface.

If Python and C++ disagree, stop and audit them before porting the behavior. If
the new language needs a different primitive because of its runtime, isolate the
difference behind the smallest adapter possible and record the exception in
`prompts/TODO.md`.

## Terminal-Only Language Target Scope

A generated language target means a terminal UIMD runtime unless the user
explicitly asks for a native GUI backend. Do not introduce SwiftUI, AppKit,
UIKit, desktop windows, web views, mobile views, or other GUI surfaces while
implementing a language target such as Swift. The target should run as a
terminal app through its normal command-line build/run flow and implement the
same terminal behavior as Python, C++, and C#: raw mode, alternate screen, ANSI
rendering, keyboard and mouse escape handling, focus/edit mode, ScrollView,
terminal modal layers, images/fallback/Sixel where supported, MCP, and clean
teardown. A native Apple UI backend is a separate future backend, not part of a
Swift terminal language target.

## Before Coding

1. Read `AGENTS.md`, `prompts/TODO.md`, and this file from disk.
2. Confirm the checkout is on `sdk-work` unless the user explicitly requested a
   different branch.
3. Check whether the new target or parity work is already tracked in
   `prompts/TODO.md`. Add a task before implementation when the work is
   substantive, parity-sensitive, or multi-step.
4. Identify the target name used by CLI flags, SDK targets, generated files,
   docs, and tests. Use one spelling everywhere, for example `csharp`.
5. Identify every existing platform that must stay equivalent: Python, C++, and
   any already-added language ports.
6. Build the existing native CLI and run a small known-good command before
   changing behavior, so failures are not confused with pre-existing setup
   issues.

## Source Ownership

Do not create a parallel compiler or second runtime tree for existing behavior.

- Compiler, generator, scaffold, SDK, and CLI behavior belongs in
  `cpp/tools/uimd`.
- Python runtime reference behavior belongs in `src/uimd/runtime`.
- Native C++ runtime behavior belongs in `cpp/include/ui` and `cpp/src`.
- A new language runtime belongs in one new language-owned runtime tree, for
  example `csharp/src/Uimd`.
- Generated files live beside that target's examples or generated output, but
  generated behavior must come from the canonical native generator.
- Compatibility files may only be wrappers, shims, forwarding entry points, or
  generated outputs.

## Architecture Map

Before implementing a feature, make a small map from Python and C++ to the new
target:

- application loop and terminal setup
- alternate screen, raw mode, mouse mode, teardown, and title handling
- key parser, SGR mouse parser, paste handling, and direct terminal input
- window/modal stack and active top-window selection
- focus model, edit mode, spatial navigation, and post-event cleanup
- rendering pipeline, frame buffer, ANSI/color blending, clipping, and dimming
- styles, state styles, cursor/selected/checked/error rendering
- `TextInput`, `NumberInput`, `Label`, `Button`, `CheckBox`, `ComboBox`,
  `ListBox`, `UIScrollView`, `UIElementReusable`, and `Image`
- reusable child focus, scroll scope, mouse capture, selection, and tick
  propagation
- standard dialogs and FileBrowser behavior
- MCP server lifecycle, tools, render snapshots, app tools, and metadata
- generator public API, event hooks, app stubs, and project files

Use the same state fields and transitions where practical. A different name is
acceptable only when language conventions require it; a different behavior is
not acceptable without an explicit recorded exception.

## CLI And SDK Target Slice

Add the target first as a thin, testable CLI/SDK slice.

1. Add the target to the native CLI target list.
2. Teach `new`, `generate`, `sdk install-target`, SDK auto-install, and `doctor`
   to recognize it.
3. Add generated project/app-stub templates for the target.
4. Add SDK packaging/install support for `targets/<target>` when the target is
   meant to ship in the SDK.
5. Update docs and `docs/example_cli_commands.md` with exact commands whenever
   a command, test, or example entry point is added.
6. Add native smoke coverage in `tools/native_uimd_parity.py` for target
   creation, generation, SDK target install, and installed SDK lookup.

This slice should prove that `./uimd generate hello.uimd --target <target>` can
produce the expected files before the full runtime is complete.

## Runtime Port Slice

Build the runtime in the same order users and tests will exercise it.

1. Terminal application shell: startup, loop, raw mode, alternate screen,
   teardown, title, and direct exit.
2. Frame buffer and render pipeline: full-frame writes, clipping, ANSI/color
   parsing, transparency, alpha/dim rules, and modal backdrops.
3. Base element model: shared properties, style states, focus/edit states,
   bounds, layout data, and event dispatch.
4. Basic controls: Label, Button, CheckBox, TextInput, NumberInput, ComboBox,
   ListBox.
5. Window/modal stack and post-event cleanup.
6. UIScrollView, reusable elements, nested focus scopes, tick propagation, and
   child-instance routing.
7. Standard dialogs and FileBrowser.
8. MCP server, tools, render snapshots, app tools, and metadata.
9. Image rendering, fallback rendering, Sixel or equivalent graphics support,
   cache behavior, and dependency diagnostics.

For each slice, read Python first, then C++, then implement the new target. If
a bug appears in compare, assume the new target is wrong until the Python/C++
paths have been audited.

## Generator Slice

The generator must emit a public API equivalent to Python and C++.

1. Reuse the native model/parser in `cpp/tools/uimd`; do not write a second
   compiler.
2. Generate one UI class per `.uimd` source with the same public member names.
3. Generate only relevant event hooks for event-capable elements.
4. Prefer inheritance/override hooks over global callback wiring for examples.
5. Generate runtime metadata, MCP source/metadata/tools, app tool dispatch, and
   dependencies consistently with Python and C++.
6. Generate project/build files that can find either the local source runtime or
   the installed SDK runtime.
7. Keep brace style and formatting consistent with the target language rules in
   `AGENTS.md`.
8. Regenerate outputs after every generator change and inspect representative
   diffs before testing.

## Example Implementation

Examples prove the target works; they must not compensate for runtime bugs.

1. Start from the existing Python `.uimd` source for the same example.
2. Use byte-for-byte identical `.uimd` content when the same UI exists across
   targets. If a synchronized copy is unavoidable, verify it is equivalent
   before continuing.
3. Generate target UI files with `./uimd generate <target examples dir>
   --target <target>`.
4. Write only domain application logic in the target's hand-written example
   file.
5. Mirror the Python/C++ example's public workflow, data, callbacks, app tools,
   and event-hook names.
6. Do not add target-only layout constants, padding, sleeps, cleanup calls,
   focus resets, scroll resets, modal workarounds, or test-only behavior.
7. If an example exposes a focus, input, dialog, layout, MCP, image, or render
   issue, fix the shared runtime/generator/tester behavior.
8. Build the example for the new target and compare it against C++ or Python
   with `--compare-app-size 90x35`.

For a full example set, implement one example vertically first, then expand:

1. `hello` or `calculator` for startup, input, buttons, and direct exit.
2. `formular` for TextInput, NumberInput, ComboBox, ListBox, CheckBox, and
   buttons.
3. `text_editor` for TextArea behavior, file/resource paths, and scrolling.
4. `task_board` or `expense_tracker` for reusable controls, ScrollView,
   dialogs, focus handoff, and generated callbacks.
5. `image_gallery` and `image_browser` for image rendering, Sixel/fallback
   parity, modal previews, FileBrowser, and performance/cache behavior.

## Regression Corpus

Existing regression apps are part of the platform contract, not optional sample
programs. Before calling a new target complete, inspect
`tests/regressions/uimd/parity` and any other documented regression corpus.

1. Generate or implement an equivalent new-target regression app for every
   existing regression whose behavior the target supports.
2. Keep the `.uimd` sources byte-for-byte equivalent to the Python/C++ sources
   unless repository path mechanics make an exact copy impossible.
3. Add the target regression build/run commands to `docs/example_cli_commands.md`
   when new entry points are added.
4. Run C++ versus the new target for each regression YAML with
   `--compare-app-size 90x35`.
5. Run the existing Python/C++ regression compare as baseline when it is
   relevant, but do not count it as a substitute for new-target coverage.
6. If a regression cannot be ported yet or is intentionally out of scope, record
   the exact reason, affected paths, parity decision, and required validation in
   `prompts/TODO.md` before reporting completion.

## Tester And Compare Workflow

Use compare tests as the main correctness gate.

1. Add or update MCP YAML under `tests/mcp/` or regression YAML under
   `tests/regressions/` when behavior must remain stable.
2. Use `--compare-app-size 90x35` for automated compare runs.
3. Prefer existing MCP tools. If they are too high-level, add minimal
   general-purpose tester/runtime inspection APIs rather than app-specific
   hooks.
4. Direct-terminal behavior needs direct PTY or platform terminal coverage:
   raw keys, SGR mouse press/release/drag, `Ctrl+C`, teardown bytes, title
   sequences, alternate screen, autowrap restoration, and full-frame writes.
   Exercise the physical SGR path for every stateful control family rather
   than treating MCP `activate_element` or `click_element` as a substitute.
   In particular, click nonzero rows in both single- and multiple-selection
   ListBoxes and verify the selected value plus its change callback.
5. Full-surface snapshots should include modal backdrops and background state,
   not only the foreground dialog.
6. Image checks should compare stable render metadata and coarse signatures
   before relying on pixel-perfect snapshots.
7. Performance checks should use stable counters, cache-hit signals, or bounded
   rerender metrics instead of brittle wall-clock-only assertions.
8. Animated rendering needs a real idle-loop gate: after the initial frame,
   observe at least two terminal updates while sending no keyboard, mouse, or
   MCP input. Deterministic MCP `snapshot_time_ms` checks validate gradient
   math, but they do not prove that the interactive loop schedules frames.
9. If a compare fails, inspect the snapshot. Do not change tests, masks, waits,
   or examples until the relevant runtime/generator implementations have been
   audited.

## Required Validation Gates

Scale validation with the changed surface, but a new target normally requires
all of these before reporting completion:

```bash
./uimd generate python/examples --target python
./uimd generate cpp/examples --target cpp
./uimd generate <new-target>/examples --target <target>
cmake --build cpp/build --parallel
cmake --build cpp/build --target uimd uimd_init
python3 tools/native_uimd_parity.py
python3 tools/native_uimd_parity.py --compile-examples
python3 -m pytest python/tests
ctest --test-dir cpp/build --output-on-failure
./uimd mcp-test --all --compare python/examples cpp/build/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --all --compare cpp/build/examples <new-target>/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --compare tests/regressions/uimd/parity/python cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/regressions/uimd/parity/<app>/<app> <new-target>/regressions/uimd/parity/<app>/<app> tests/regressions/uimd/parity/<app>.yaml --mcp-fast --compare-app-size 90x35
git diff --check
```

Use the target's normal build command as well, for example `dotnet build` for
C#. Repeat the new-target regression command for every supported regression
manifest. On Windows, use the documented `.cmd` or `.ps1` commands from
`docs/example_cli_commands.md` and keep the same compare viewport.

When a platform or dependency is unavailable, record exactly what was skipped
and why. Do not claim support for an unvalidated public install platform.

## Mandatory Post-Implementation Audit And Remediation

After the implementation appears complete and the initial validation gates
pass, do not report the language or platform as complete yet. Perform the same
complete structural and behavioral audit again:

1. Compare the entire new runtime and native compiler/generator/tool surface
   against Python shared semantics and the corresponding C++ architecture,
   classes, methods, state fields, event order, rendering flow, cleanup points,
   public API, dialogs, MCP lifecycle, terminal behavior, examples, and
   regression coverage.
2. Produce a concrete inventory of equivalent, divergent,
   primitive-specific, missing, and untested surfaces with exact paths and
   required validation.
3. Add every discovered difference to `prompts/TODO.md` as an open task before
   changing it. Each task must identify the reference and new-target paths, the
   required 1:1 result, and its focused validation gate.
4. Immediately continue with implementation and remove every fixable
   difference. Do not stop after recording the audit, and do not declare the
   target complete because broad builds or snapshot compares pass.
5. After remediation, repeat the complete audit and required validation.
   Continue this audit-record-fix cycle until no unexplained structural or
   behavioral difference remains. Any unavoidable language/OS primitive
   adapter must be the smallest possible exception and remain explicitly
   documented in `prompts/TODO.md` with its rationale and parity evidence.

## Handling Remaining Work

When an issue is discovered but cannot be finished in the current slice:

1. Keep the main behavior correct and do not hide the issue with an example
   workaround.
2. Add a focused open task to `prompts/TODO.md`.
3. Include affected Python, C++, and new-target paths.
4. Record the parity decision and required validation.
5. Keep unfinished direct-terminal, visual, image, performance, or platform
   tester gaps as tester/runtime tasks when they are not language-specific.

## Commit Rules

Do not commit unless the user explicitly asks.

Before a requested commit:

1. Run `git status --short --branch` and confirm the branch is `sdk-work`.
2. Evaluate the version bump rule in `AGENTS.md`; ordinary `sdk-work` commits
   that change tracked files require a patch bump through
   `python3 tools/set_version.py <version>`.
3. Stage only intended files.
4. Use an English commit message with an `LLM: <model>` footer.
5. Report the validation that passed and any skipped gates.

## Completion Criteria

A new language or platform is not complete just because it compiles. It is
complete for a slice only when:

- the CLI can generate it from `.uimd`
- the runtime implements the same public behavior as Python/C++
- examples use identical UI sources and contain only domain logic
- generated public APIs match the established event/member model
- SDK install/packaging paths know the target when applicable
- documented commands exist
- compare and direct-terminal tests cover the behavior exercised by users
- supported regression corpora are built and compared against C++
- the mandatory post-implementation audit/remediation cycle found no remaining
  unexplained structural or behavioral difference
- unresolved parity exceptions are recorded in `prompts/TODO.md`
