# Cross-Platform Bug-Fix Workflow

Use this workflow for every substantive bug fix, regression, failing test,
parity defect, or selected GitHub issue. It applies even when the report names
only one language: a one-platform symptom may expose a defect in the shared
runtime contract, generator, test infrastructure, or another port.

For a new generated language target, runtime port, platform backend, or full
example set, follow `docs/new-language-platform-workflow.md` as the primary
workflow. Use this document for individual defects discovered while that port
is being implemented or audited.

## Core Rule

Fix the owner of the behavior and preserve one observable contract across all
relevant ports. Do not make an example, snapshot, test, delay, or
platform-specific branch compensate for a shared runtime defect.

Python under `src/uimd/runtime` is the semantic reference for shared runtime
behavior. The native C++ tool under `cpp/tools/uimd` owns compiler, generator,
CLI, scaffold, SDK, and `mcp-test` behavior. C++ under `cpp/include/ui` and
`cpp/src` is the parity-validated native runtime implementation. Existing
language ports are implementations of the same contract, not independent
behavioral authorities.

If the Python reference is also wrong, define the intended shared behavior
from the specification, public API, existing parity evidence, and user-visible
contract, then correct Python and every affected port. If Python and C++
disagree before the fix, stop and audit the disagreement before using either as
an oracle.

## Trigger And Required Reads

Before diagnosing or editing a substantive bug:

1. Read `AGENTS.md` completely.
2. Immediately read `prompts/TODO.md`.
3. Read this workflow completely.
4. If the work is part of a new language or platform implementation, also read
   `docs/new-language-platform-workflow.md` completely.
5. Confirm the branch and inspect the working tree. Preserve unrelated user
   changes and never commit unless the user explicitly requests it.
6. Check whether the bug is already recorded in `prompts/TODO.md`. Add an open
   entry before substantive analysis or implementation if it is missing.

After automatic context compaction or resuming from a summary, repeat the
required reads before answering, editing, testing, or reporting status.

## Phase 1: Intake And Scope

Capture the report before changing code:

1. Record its source: user report, failing command, test name, log, GitHub
   issue number and link, or parity comparison.
2. Preserve the exact observed behavior, expected behavior, reproduction
   inputs, platform, language target, viewport, terminal, and dependency state.
3. Separate the user-visible failure from secondary errors caused by an
   earlier failure.
4. Identify whether the request authorizes diagnosis only or also authorizes
   implementation. Do not turn a review-only request into a code change.
5. For a GitHub issue list, follow the issue-triage rule in `AGENTS.md`: display
   the issues and wait for the user to select which ones to implement.
6. For selected issues, keep their numbers, links, titles, affected areas, and
   reproduction details in `prompts/TODO.md` until they are resolved.

Do not assume the issue title correctly identifies the owner. A report titled
"C++ bug" can be a shared Python/C++ contract defect, a generator defect that
appears in C++, or a terminal primitive that affects several ports.

### Related Bug-Fix Batches

Several confirmed bugs may be handled as one validation batch when they share
the same canonical owner, runtime subsystem, state transition, generated
surface, or focused test set. This avoids repeating an hours-long full suite
after every closely related repair.

Before starting a batch:

1. List every included bug or GitHub issue in one open `prompts/TODO.md` task.
2. State the exact shared owner and batch boundary.
3. Keep a separate reproduction and regression assertion for every bug.
4. Define the focused test set that runs after each individual repair.
5. Define the one final full or blast-radius validation gate that runs after
   every fix in the batch is green.

Focused validation is never deferred: each repair must pass its own unit or
regression test before the next repair can rely on it. Only the expensive full
supported-platform gate is consolidated. Do not mix unrelated owners merely to
reduce test runs. Split the batch when a failure cannot be attributed safely,
when one issue requires a materially different architecture, or when the user
asks for an independent fix.

## Phase 2: Reproduce Before Editing

Obtain the smallest reliable failing signal available:

1. Prefer an existing focused unit, regression, MCP compare, or direct-terminal
   command over the full suite.
2. Reproduce with the same viewport and environment as the report. Automated
   MCP compares must use `--compare-app-size 90x35`.
3. Confirm that generated artifacts are current. If an artifact is stale,
   rebuild it before classifying the stale-artifact error as the product bug.
4. Save the first meaningful assertion, mismatch, exception, or terminal
   symptom. Do not diagnose only from the final nonzero exit code.
5. When a compare fails, retain and report the snapshot viewer command printed
   by the tester.
6. For raw terminal behavior, use a direct PTY test. Logical MCP snapshots are
   not proof of terminal teardown, escape-sequence, mouse-protocol, Sixel
   graphics-plane, or interactive idle-loop correctness.
7. If local reproduction is impossible because a platform or dependency is
   unavailable, record the exact missing gate and continue with source-level
   analysis only where it is safe. Do not claim the bug is fixed without the
   missing validation.

When a deterministic regression test can be written before the fix, add it to
the canonical test layer first and confirm that it fails for the intended
reason. Do not add a test that merely encodes the current implementation.

## Phase 3: Determine Canonical Ownership

Classify the failing behavior and identify its canonical source before editing:

| Behavior | Canonical source to inspect first |
| --- | --- |
| Focus, edit mode, input, rendering, ScrollView, reusable elements, dialogs, modal stack | `src/uimd/runtime` |
| Native runtime parity | `cpp/include/ui` and `cpp/src` |
| Compiler, parser, generator, CLI, scaffold, SDK, `mcp-test` | `cpp/tools/uimd` |
| Shared format or behavior contract | `shared/specs` and canonical Python runtime |
| Generated public API | Native generator plus generated outputs for every affected target |
| Language-specific runtime primitive | That language runtime, behind the smallest adapter possible |
| Test aggregation or reporting | `tools/` and its focused unit tests |

Then trace the complete state transition, not only the line that emits the
failure:

- input or MCP action
- active window and element resolution
- focus/edit/selection/scroll state mutation
- generated callback or owner routing
- post-event cleanup
- layout and rendering
- snapshot, terminal diff, or returned MCP state

For public APIs, also trace generator templates/emitters, generated classes,
hand-written application overrides, and compatibility behavior.

## Phase 4: Build The Port Parity Matrix

Identify every currently supported port that implements the behavior. Do not
use a fixed historical list; inspect the repository because supported targets
change over time.

Record a matrix in the active `prompts/TODO.md` task before implementation:

| Port or layer | Reference path/function | Current behavior | Required action | Focused validation |
| --- | --- | --- | --- | --- |
| Python reference | exact path/state transition | correct or defective | preserve or fix | exact test |
| C++ runtime/tool | exact corresponding path | correct or defective | preserve or fix | exact test |
| Each language port | exact corresponding path | equivalent, divergent, absent, or unaffected | fix, verify, or justify unaffected | exact test |
| Generator/generated API | emitter and representative output | equivalent or divergent | fix emitter and regenerate | generation/API test |
| Terminal/platform adapter | exact primitive | equivalent or unavoidable exception | smallest adapter | direct-terminal test |

Every port must end in one of these explicit states:

- **fix**: it contains the defect and must change;
- **verify**: it appears correct but needs a regression test;
- **unaffected**: it does not implement or cannot reach the behavior, with a
  concrete reason;
- **documented primitive exception**: the implementation must differ because
  of a language or OS primitive while preserving the public behavior.

Never silently omit a supported port. Never change several ports differently
just to make their individual tests pass.

## Phase 5: Design The Shared Fix

Before editing, define:

1. The desired externally observable behavior.
2. The state fields involved and their meaning.
3. The exact event and callback order.
4. The cleanup point at which runtime state becomes consistent.
5. Stable identity rules when elements can be destroyed and recreated.
6. Error behavior and edge cases.
7. Backward-compatibility requirements for public APIs.
8. The regression assertions that distinguish the fix from the old behavior.

Prefer one shared algorithm and equivalent state machine in every port.
Language syntax may differ; ownership, state transitions, callback order,
rendering rules, and edge cases may not.

For an API extension, prefer an additive, backward-compatible path. Keep the
old callback or method working while adding the stable identity or richer
context required by the fix, unless the user explicitly approves a breaking
change.

Additive syntax alone is not sufficient proof of compatibility. A new required
field can break exhaustive Rust struct literals, and inserting a field into a
C++ aggregate can break positional initialization even when every repository
example regenerates successfully. Preserve every previously documented
construction pattern and previously generated source. A patch-level bug fix
may change the reported defective behavior only; it must not require an
ordinary application author to edit or regenerate an existing app.

When the user explicitly decides that an existing public API is obsolete,
follow the lifecycle in `docs/deprecations.md` instead of deleting it as part
of the fix. Mark it deprecated in every affected port, keep a forwarding or
behavior-preserving compatibility path, record the current version and its
replacement, and add native warnings. An API deprecated in `0.x.y` remains
through `0.(x+1).*` and is removed as one cross-platform change in `0.(x+2).0`.
There is no per-API removal-version decision.

For generated code, edit the canonical emitter or shared model. Do not hand
edit multiple generated outputs as independent implementations.

## Phase 6: Implement In Reference Order

Use this order unless the ownership analysis proves a different order is
necessary:

1. Correct the specification or contract if it is ambiguous.
2. Correct the canonical Python runtime behavior for shared semantics.
3. Correct the corresponding C++ runtime behavior with the same architecture
   and transition order.
4. Correct the canonical native compiler/generator when generated APIs or
   command behavior are affected.
5. Port the same fix to every affected language runtime.
6. Regenerate outputs from their canonical `.uimd` sources or native emitter.
7. Add or update focused tests for every affected implementation layer.

If Python is already correct, preserve it and use it as the semantic reference.
If another port is already correct, do not rewrite it unnecessarily; add the
focused validation that proves equivalence.

Keep general behavior out of examples. Examples may contain only domain logic
and data. Do not add waits, focus resets, layout constants, platform checks,
fallbacks, test-only branches, or manual cleanup to hide a runtime defect.

## Phase 7: Focused Validation While Developing

Run the smallest meaningful gates after each implementation step:

1. Unit test for the changed state transition or renderer.
2. Existing regression that originally exposed the bug.
3. The same focused test in each affected port.
4. C++ versus affected-port MCP compare when the behavior is observable through
   MCP, always with `--compare-app-size 90x35`.
5. Direct PTY smoke for physical keyboard, mouse, terminal lifecycle, images,
   Sixel, or teardown behavior.
6. Generator/API snapshot or compile test when public generated code changes.

Inspect unexpected failures as implementation evidence. Do not update
snapshots, masks, waits, expected errors, or examples until the canonical and
port implementations have been audited against the intended contract.

## Phase 8: Regeneration And Build Gates

Follow the blast-radius rules in `AGENTS.md`:

- Regenerate affected generated outputs after a generator or `.uimd` change.
- For a focused cross-platform example fix, regenerate and build both involved
  sides and every affected example.
- For broad runtime architecture, rendering pipeline, global input routing,
  compiler/generator, CLI/tool, layout, or window/modal changes, regenerate and
  build the full supported example set for every affected target.
- If focused implementation and debugging takes more than roughly four hours,
  run the full supported-platform validation after focused gates pass.
- Add every new or renamed test command to `docs/example_cli_commands.md`.

Use `./tools/rebuild_all.sh` or the documented explicit commands when the full
regeneration/build gate is required. Do not confuse a successful build with a
behavioral test pass.

## Backward Compatibility Gate

For every change that can affect a library, runtime, compiler, generator, CLI,
SDK, generated public API, or documented application contract:

1. Retain or create a fixture application and generated outputs produced by
   the immediately preceding version.
2. Do not edit or regenerate that fixture during validation.
3. Compile it against the updated library or SDK using its original documented
   build command.
4. Run it and exercise the affected startup and interaction path. Compilation
   alone is not sufficient.
5. Repeat the gate for every affected supported language. Classify a language
   as unaffected only with a concrete ownership reason in `prompts/TODO.md`.
6. Keep a regression command for this compatibility fixture in
   `docs/example_cli_commands.md` and the appropriate aggregate test runner.

Current examples regenerated from the new emitter prove current generation,
not backward compatibility. They never replace this gate. If the preceding
version's documented app or generated output does not compile and run without
source changes, stop: the change is breaking and cannot ship as a patch unless
the user explicitly approves the break and its migration/version plan.

## Phase 9: Final Validation

Before reporting a fix complete:

1. Re-run the original reproduction and confirm the old failure is gone.
2. Run the new regression test and prove it would have caught the old defect.
3. Run all focused unit, compare, and direct-terminal gates recorded in the
   parity matrix.
4. Run the backward compatibility gate for every affected platform without
   editing or regenerating the preceding-version fixtures.
5. Run the full supported-platform gate when required by blast radius or task
   duration. The repository entry point is `./tools/test_all.sh`; use its
   documented keep-going/live-report options when a complete failure inventory
   is needed.
6. Inspect the final one-line recap and the complete log. Do not report success
   merely because an intermediate phase passed.
7. Run `git diff --check` and inspect `git status --short --branch`.
8. Review the final diff for example-specific workarounds, duplicated behavior,
   generated-file-only edits, accidental platform divergence, unrelated user
   changes, and missing documentation.
9. Update the active TODO entry with the final parity decision, exact tests,
   skipped gates, and remaining caveats, then mark it complete only when no
   required work remains.

For an explicitly recorded related-fix batch, apply steps 1 through 3 to every
included bug, then run steps 4 through 7 once for the complete batch. A full
suite passed before the last batch edit is not the final gate.

## GitHub Issue And Commit Handling

Do not comment on or close an issue merely because a local edit appears to fix
it. Do not commit unless the user explicitly requests a commit.

After a user-requested commit succeeds:

1. Confirm that the committed diff contains the complete intended fix and
   regression coverage.
2. Add a concise comment to each resolved issue with the commit SHA or summary,
   validation performed, and any remaining platform caveats.
3. Close only issues genuinely resolved by that commit.
4. Leave duplicates, external failures, incomplete fixes, and unvalidated
   platform cases open with an accurate explanation.

Follow all versioning, branch, commit-message, and main-push rules in
`AGENTS.md`.

## Stop Conditions

Stop and report the blocker instead of guessing when:

- the desired public behavior is ambiguous and materially different choices
  are possible;
- Python and C++ disagree and available specifications do not resolve it;
- a fix requires a breaking public API change that the user has not approved;
- a required platform, terminal, dependency, credential, or external service
  is unavailable and no equivalent validation exists;
- unrelated user changes overlap the exact files or behavior that must change;
- the proposed solution would require an example-specific workaround or an
  undocumented platform divergence.

## Completion Criteria

A bug is complete only when:

- the root cause and canonical owner are identified;
- the original failure is reproducible or the missing reproduction is
  explicitly documented;
- every supported port is classified in the parity matrix;
- all affected ports implement the same public behavior and state transition;
- generated outputs come from the canonical generator and `.uimd` sources;
- a preceding-version documented application and its generated outputs compile
  and run against the update without source edits or regeneration for every
  affected platform;
- focused regression coverage proves the behavior;
- required compare and direct-terminal gates pass;
- regeneration/build coverage matches the change blast radius;
- the original reproduction passes;
- `prompts/TODO.md` records the final parity decision and validation;
- no issue, commit, push, version, or release state was changed without the
  user's explicit authorization.
