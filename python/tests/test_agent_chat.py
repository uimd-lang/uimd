"""Regression tests for the deterministic Agent Chat Simulation example."""

import os
import sys
import unittest

PROJECT_ROOT = os.path.join(os.path.dirname(__file__), "..")
AGENT_CHAT_DIR = os.path.join(PROJECT_ROOT, "examples", "agent_chat")
sys.path.insert(0, PROJECT_ROOT)
sys.path.insert(0, AGENT_CHAT_DIR)

from agent_chat import (
    AgentChatSimulation,
    SIMULATION_DURATION_SECONDS,
    SIMULATION_EVENT_COUNT,
    SIMULATION_MESSAGE_TARGET,
    SIMULATION_STEP_INTERVAL_SECONDS,
    SCRIPTED_STEPS,
    STRESS_ENTRY_COUNT,
    STRESS_TARGET_ROWS,
    stress_entry,
)
from runtime import UIApplication


class FakeClock:
    def __init__(self):
        self.now = 0.0

    def __call__(self):
        return self.now

    def advance(self, seconds):
        self.now += seconds


class TestAgentChatSimulation(unittest.TestCase):
    """The example stays deterministic and never performs external work."""

    def setUp(self):
        self.clock = FakeClock()
        self.app = UIApplication(width=90, height=35)
        self.chat = AgentChatSimulation(clock=self.clock)
        self.app.open(self.chat)

    def test_initial_and_complete_scenario_have_stable_counts(self):
        self.assertEqual(self.chat.transcript.child.entry_count(), 1)
        self.assertEqual(self.chat.status.text, "Ready · 1 message")

        self.chat.start_simulation()

        self.assertEqual(self.chat.transcript.child.entry_count(), 2)
        self.assertEqual(self.chat.status.text, "Running · 2 messages")

        self.clock.advance(SIMULATION_STEP_INTERVAL_SECONDS)
        self.assertTrue(self.chat.tick())
        self.assertEqual(self.chat.transcript.child.entry_count(), 3)

        self.clock.advance(SIMULATION_DURATION_SECONDS)
        self.assertTrue(self.chat.tick())
        self.assertEqual(self.chat.transcript.child.entry_count(), SIMULATION_MESSAGE_TARGET)
        self.assertEqual(self.chat.status.text, "Scenario complete · 100 messages")

    def test_completed_scenario_scrolls_up_from_bottom_and_changes_render(self):
        self.chat.start_simulation()
        self.clock.advance(SIMULATION_DURATION_SECONDS)
        self.chat.tick()
        before = self.app.render()
        transcript = self.chat.transcript.child

        self.assertEqual(transcript.scroll_position()["view_offset"], 0)
        self.assertTrue(transcript.scroll_lines(10))
        self.assertGreater(transcript.scroll_position()["view_offset"], 0)
        self.assertNotEqual(self.app.render(), before)

    def test_streaming_steps_replace_the_last_entry(self):
        for _ in range(5):
            self.chat.next_step()
        count_before_replacement = self.chat.transcript.child.entry_count()

        self.chat.next_step()
        self.chat.next_step()

        self.assertEqual(self.chat.transcript.child.entry_count(), count_before_replacement)
        last = self.chat.transcript.child._children[-1]
        self.assertEqual(last.phase.text, "stream · complete")
        self.assertIn("same transition", last.body.text)

    def test_reset_discards_progress_and_restores_initial_message(self):
        self.chat.start_simulation()

        self.chat.reset_simulation()

        self.assertEqual(self.chat.transcript.child.entry_count(), 1)
        self.assertEqual(self.chat.status.text, "Ready · 1 message")
        self.assertEqual(self.chat.transcript.child._children[0].role.text, "You")

    def test_pause_and_next_step_preserve_deterministic_progress(self):
        self.chat.start_simulation()
        self.chat.pause_simulation()
        self.clock.advance(SIMULATION_DURATION_SECONDS)

        self.assertFalse(self.chat.tick())
        self.assertEqual(self.chat.transcript.child.entry_count(), 2)
        self.assertEqual(self.chat.status.text, "Paused · 2 messages")

        self.chat.next_step()
        self.assertEqual(self.chat.transcript.child.entry_count(), 3)
        self.assertEqual(self.chat.status.text, "Step applied · 3 messages")

    def test_stress_entries_are_pure_and_deterministic(self):
        self.assertEqual(STRESS_ENTRY_COUNT, 5_556)
        self.assertEqual(STRESS_TARGET_ROWS, 50_000)
        self.assertEqual(stress_entry(0), stress_entry(0))
        self.assertEqual(stress_entry(STRESS_ENTRY_COUNT - 1), stress_entry(STRESS_ENTRY_COUNT - 1))
        self.assertEqual(len(SCRIPTED_STEPS), 11)
        self.assertEqual(SIMULATION_EVENT_COUNT, 101)
        self.assertIn("Unicode", stress_entry(7)[2])

    def test_all_right_aligned_phases_fit_the_nested_padded_header(self):
        for _ in SCRIPTED_STEPS:
            self.chat.next_step()
        self.app.render()

        expected_phases = {
            "request",
            "patch",
            "test failure",
            "structured data",
            "final",
        }
        rendered_phases = set()
        for entry in self.chat.transcript.child._children:
            phase = entry.phase.text
            if phase not in expected_phases:
                continue
            header = "".join(cell.normalized_text() for cell in entry.render_cells()[0])
            self.assertIn(phase, header)
            self.assertLessEqual(
                entry.phase.col + entry.phase.width,
                entry.phase._resolved_cell.width - entry.phase._cell_padding[1],
            )
            rendered_phases.add(phase)

        self.assertEqual(rendered_phases, expected_phases)


if __name__ == "__main__":
    unittest.main()
