"""Deterministic, offline agent chat simulation example."""

import os
import sys
import time

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(_here)))

from runtime import UIApplication
from agent_chat_ui import AgentChatUI


INITIAL_ENTRY = (
    "You",
    "request",
    "Audit the release pipeline and propose a safe fix.\n"
    "Constraints: deterministic output, no network, preserve compatibility.",
    "simulated user message",
)

SCRIPTED_STEPS = (
    (
        False,
        "Agent",
        "plan",
        "## Plan\n1. Inspect the failing path\n2. Compare all language ports\n"
        "3. Add focused tests\n4. Run the parity gate",
        "simulated reasoning summary",
    ),
    (
        False,
        "Tool",
        "search_code",
        "$ search_code query=release_manifest\n"
        "src/release/manifest.py:42 validate_manifest\n"
        "cpp/tools/release.cpp:88 validateManifest\nresult: 2 matches",
        "SIMULATED TOOL · no command executed",
    ),
    (
        False,
        "Agent",
        "table",
        "| Port   | Owner                  | State |\n"
        "|--------|------------------------|-------|\n"
        "| Python | validate_manifest      | ref   |\n"
        "| C++    | validateManifest       | drift |",
        "deterministic comparison table",
    ),
    (
        False,
        "Agent",
        "code · python",
        "```python\ndef validate(entry):\n    if not entry.digest:\n"
        "        raise ValueError(\"missing digest\")\n    return entry\n```",
        "simulated code block",
    ),
    (
        False,
        "Agent",
        "stream · 1/3",
        "I found the shared invariant. The manifest must reject empty digests",
        "simulated streaming response",
    ),
    (
        True,
        "Agent",
        "stream · 2/3",
        "I found the shared invariant. The manifest must reject empty digests\n"
        "before platform-specific packaging starts, while preserving old inputs.",
        "simulated streaming response",
    ),
    (
        True,
        "Agent",
        "stream · complete",
        "I found the shared invariant. The manifest must reject empty digests\n"
        "before platform-specific packaging starts, while preserving old inputs.\n"
        "Next I will apply the same transition in each affected port.",
        "simulated streaming response",
    ),
    (
        False,
        "Tool",
        "patch",
        "--- a/src/release/manifest.py\n+++ b/src/release/manifest.py\n"
        "@@ -41,2 +41,4 @@\n+ if not entry.digest:\n+     raise ValueError(\"missing digest\")",
        "SIMULATED PATCH · repository unchanged",
    ),
    (
        False,
        "Tool",
        "test failure",
        "ERROR test_manifest_rejects_empty_digest\n"
        "AssertionError: expected 'missing digest', got 'invalid manifest'\n"
        "  at tests/test_manifest.py:117\nstatus: 1 failed, 23 passed",
        "SIMULATED ERROR · no test executed",
    ),
    (
        False,
        "Agent",
        "structured data",
        "result:\n  status: fixed\n  ports: [python, cpp, csharp, go, rust, swift, java]\n"
        "  compatibility: preserved\n  evidence: {focused: 24, compare: 7}",
        "simulated YAML result",
    ),
    (
        False,
        "Agent",
        "final",
        "### Done ✓\n- [x] Focused regression\n- [x] 1:1 parity audit\n"
        "- [x] Backward compatibility\nUnicode: ľščťžýáíé · 東京 · 🙂\nOption: keep patch small",
        "simulated final answer",
    ),
)

STRESS_ENTRY_COUNT = 5_556
STRESS_TARGET_ROWS = 50_000
SIMULATION_MESSAGE_TARGET = 100
SIMULATION_DURATION_SECONDS = 180.0
STRESS_TEMPLATES = (
    ("You", "request", "Investigate deterministic case {index}.\n- preserve API\n- compare ports"),
    ("Agent", "plan", "## Plan {index}\n1. inspect\n2. reproduce\n3. repair\n4. validate"),
    ("Tool", "search_code", "$ search_code case_{index}\nsrc/module.py:{line}: match\nresult: simulated"),
    ("Agent", "code", "```python\ndef case_{index}():\n    return {{\"ok\": True}}\n```"),
    ("Tool", "diff", "--- before_{index}\n+++ after_{index}\n@@ -1 +1 @@\n-old\n+new"),
    ("Agent", "table", "| item | value |\n|------|-------|\n| id | {index} |\n| state | pass |"),
    ("Tool", "warning", "WARNING simulated retry {index}\ntrace: worker -> parser -> validator\nno external action"),
    ("Agent", "result", "{{\"id\": {index}, \"status\": \"ok\"}}\nUnicode: čau · 東京 · 🙂"),
)


def stress_entry(index):
    """Return one deterministic stress entry without I/O or random state."""
    role, phase, template = STRESS_TEMPLATES[index % len(STRESS_TEMPLATES)]
    return (
        role,
        phase,
        template.format(index=index, line=(index % 997) + 1),
        "SIMULATED STRESS DATA · no external action",
    )


SCRIPTED_MESSAGE_COUNT = 1 + sum(1 for step in SCRIPTED_STEPS if not step[0])
SIMULATION_EVENT_COUNT = (
    len(SCRIPTED_STEPS) + SIMULATION_MESSAGE_TARGET - SCRIPTED_MESSAGE_COUNT
)
SIMULATION_STEP_INTERVAL_SECONDS = (
    SIMULATION_DURATION_SECONDS / (SIMULATION_EVENT_COUNT - 1)
)


class AgentChatSimulation(AgentChatUI):
    """Run a deterministic visual chat scenario without external work."""

    def __init__(self, clock=None):
        super().__init__()
        self._clock = clock or time.monotonic
        self._next_step = 0
        self._running_simulation = False
        self._next_step_at = None

    def open(self):
        super().open()
        if not self.transcript.child._children:
            self.reset_simulation()

    def on_start_button_click(self):
        self.start_simulation()

    def on_pause_button_click(self):
        self.pause_simulation()

    def on_next_step_button_click(self):
        self.next_step()

    def on_reset_button_click(self):
        self.reset_simulation()

    def on_load_stress_button_click(self):
        self.load_stress_transcript()

    def on_quit_button_click(self):
        self.quit()

    def start_simulation(self):
        if self._next_step >= SIMULATION_EVENT_COUNT:
            self.reset_simulation()
        if self._next_step == 0:
            self._apply_next_timeline_event()
        self.transcript.child.scroll_to_bottom()
        if self._next_step >= SIMULATION_EVENT_COUNT:
            self._finish_simulation()
            return
        self._running_simulation = True
        self._next_step_at = self._clock() + SIMULATION_STEP_INTERVAL_SECONDS
        self._set_status("Running")

    def pause_simulation(self):
        self._running_simulation = False
        self._next_step_at = None
        self._set_status("Paused")

    def next_step(self):
        self._running_simulation = False
        self._next_step_at = None
        if self._next_step >= SIMULATION_EVENT_COUNT:
            self._set_status("Scenario complete")
            return
        self._apply_next_timeline_event()
        self.transcript.child.scroll_to_bottom()
        prefix = "Scenario complete" if self._next_step == SIMULATION_EVENT_COUNT else "Step applied"
        self._set_status(prefix)

    def reset_simulation(self):
        self._running_simulation = False
        self._next_step_at = None
        transcript = self.transcript.child
        transcript.clear_entries()
        transcript.append_entry(*INITIAL_ENTRY)
        transcript.scroll_to_top()
        self._next_step = 0
        self._set_status("Ready")

    def load_stress_transcript(self):
        self._running_simulation = False
        self._next_step_at = None
        transcript = self.transcript.child
        transcript.clear_entries()
        for index in range(STRESS_ENTRY_COUNT):
            transcript.append_entry(*stress_entry(index))
        transcript.scroll_to_top()
        self._next_step = SIMULATION_EVENT_COUNT
        self.status.text = (
            f"Stress loaded · {transcript.entry_count()} messages · "
            f"~{STRESS_TARGET_ROWS} rows"
        )

    def _apply_step(self, step):
        replace_last, role, phase, body, detail = step
        transcript = self.transcript.child
        if replace_last:
            transcript.replace_last_entry(role, phase, body, detail)
        else:
            transcript.append_entry(role, phase, body, detail)

    def _apply_next_timeline_event(self):
        if self._next_step < len(SCRIPTED_STEPS):
            self._apply_step(SCRIPTED_STEPS[self._next_step])
        else:
            stress_index = self._next_step - len(SCRIPTED_STEPS)
            self.transcript.child.append_entry(*stress_entry(stress_index))
        self._next_step += 1

    def _finish_simulation(self):
        self._running_simulation = False
        self._next_step_at = None
        self._set_status("Scenario complete")

    def tick(self):
        changed = bool(super().tick())
        if not self._running_simulation or self._next_step_at is None:
            return changed
        now = self._clock()
        applied = False
        while now >= self._next_step_at and self._next_step < SIMULATION_EVENT_COUNT:
            self._apply_next_timeline_event()
            self._next_step_at += SIMULATION_STEP_INTERVAL_SECONDS
            applied = True
        if not applied:
            return changed
        self.transcript.child.scroll_to_bottom()
        if self._next_step >= SIMULATION_EVENT_COUNT:
            self._finish_simulation()
        else:
            self._set_status("Running")
        return True

    def _set_status(self, prefix):
        count = self.transcript.child.entry_count()
        noun = "message" if count == 1 else "messages"
        self.status.text = f"{prefix} · {count} {noun}"


def main():
    app = UIApplication()
    window = AgentChatSimulation()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
