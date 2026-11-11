"""Regression tests for the deterministic Agent Chat Simulation example."""

import os
import sys
import unittest
import json

PROJECT_ROOT = os.path.join(os.path.dirname(__file__), "..")
AGENT_CHAT_DIR = os.path.join(PROJECT_ROOT, "examples", "agent_chat")
sys.path.insert(0, PROJECT_ROOT)
sys.path.insert(0, AGENT_CHAT_DIR)

from agent_chat import (
    AgentChatSimulation,
    SIMULATION_DURATION_SECONDS,
    SIMULATION_EVENT_COUNT,
    SIMULATION_MESSAGE_TARGET,
    SCRIPTED_DURATIONS,
    SCRIPTED_STEPS,
    STRESS_ENTRY_COUNT,
    STRESS_LONG_MESSAGE_ROWS,
    STRESS_MIN_MESSAGE_ROWS,
    STRESS_TARGET_ROWS,
    stress_entry,
    stress_entry_line_count,
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

        self.clock.advance(SCRIPTED_DURATIONS[0])
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
        last = self.chat.transcript.child.entry_data(count_before_replacement - 1)
        self.assertEqual(last[1], "text")
        self.assertIn("smallest cohesive update", last[2])

    def test_reset_discards_progress_and_restores_initial_message(self):
        self.chat.start_simulation()

        self.chat.reset_simulation()

        self.assertEqual(self.chat.transcript.child.entry_count(), 1)
        self.assertEqual(self.chat.status.text, "Ready · 1 message")
        self.assertEqual(self.chat.transcript.child.entry_data(0)[0], "You")

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
        observed_heights = {
            stress_entry_line_count(index)
            for index in range(STRESS_LONG_MESSAGE_ROWS * 3)
        }
        self.assertEqual(min(observed_heights), STRESS_MIN_MESSAGE_ROWS)
        self.assertEqual(max(observed_heights), STRESS_LONG_MESSAGE_ROWS)
        self.assertGreater(len(SCRIPTED_STEPS), SIMULATION_MESSAGE_TARGET)
        self.assertEqual(SIMULATION_EVENT_COUNT, len(SCRIPTED_STEPS))
        self.assertAlmostEqual(sum(SCRIPTED_DURATIONS), SIMULATION_DURATION_SECONDS)
        self.assertIn("Unicode", stress_entry(7)[2])

    def test_heron_style_roles_and_rich_blocks(self):
        for _ in SCRIPTED_STEPS:
            self.chat.next_step()
        transcript = self.chat.transcript.child
        phases = [transcript.entry_data(index)[1] for index in range(transcript.entry_count())]
        table_index = phases.index("table")
        code_index = phases.index("code")
        reasoning_index = phases.index("reasoning")

        self.assertEqual(transcript.entry_data(0)[0], "You")
        self.assertEqual(transcript.entry_data(0)[1], "user")

        transcript.scroll_to(table_index)
        table_entry = next(
            item["child"]
            for item in transcript.child_view_entries()
            if item["index"] == table_index
        )
        self.assertEqual(type(table_entry).__name__, "TableEntryUI")
        self.assertIn("| Check", table_entry.table.text)
        table_text = "\n".join(
            "".join(cell.normalized_text() for cell in row)
            for row in table_entry.render_cells()
        )
        self.assertIn("┌", table_text)
        self.assertEqual(table_entry.heading.text, "Verification table")

        transcript.scroll_to(code_index)
        code_entry = next(
            item["child"]
            for item in transcript.child_view_entries()
            if item["index"] == code_index
        )
        self.assertEqual(type(code_entry).__name__, "CodeEntryUI")
        self.assertIn("const COLS", code_entry.code.text)
        self.assertNotIn("```", code_entry.code.text)
        self.assertTrue(code_entry.code.text.lstrip().startswith("1  "))

        transcript.scroll_to(reasoning_index)
        reasoning_entry = next(
            item["child"]
            for item in transcript.child_view_entries()
            if item["index"] == reasoning_index
        )
        self.assertEqual(type(reasoning_entry).__name__, "ReasoningEntryUI")
        self.assertEqual(reasoning_entry.heading.text, "Thinking")
        self.assertIn("board model", reasoning_entry.body.text)

    def test_stress_then_start_appends_scenario_and_top_bottom_navigation(self):
        self.chat.load_stress_transcript()
        transcript = self.chat.transcript.child
        self.assertEqual(transcript.entry_count(), STRESS_ENTRY_COUNT)

        self.chat.start_simulation()
        self.assertEqual(transcript.entry_count(), STRESS_ENTRY_COUNT + 2)
        self.assertIn("Tetris", transcript.entry_data(STRESS_ENTRY_COUNT)[2])
        self.assertEqual(transcript.scroll_position()["view_offset"], 0)

        self.chat.on_top_click()
        self.assertGreater(transcript.scroll_position()["view_offset"], 0)
        offset = transcript.scroll_position()["view_offset"]
        self.clock.advance(SCRIPTED_DURATIONS[0])
        self.chat.tick()
        self.assertGreaterEqual(transcript.scroll_position()["view_offset"], offset)
        self.chat.on_end_click()
        self.assertEqual(transcript.scroll_position()["view_offset"], 0)

    def test_next_step_follows_bottom_until_user_scrolls_up(self):
        transcript = self.chat.transcript.child
        for _ in range(20):
            self.chat.next_step()
        self.assertEqual(transcript.scroll_position()["view_offset"], 0)

        self.chat.on_top_click()
        self.assertGreater(transcript.scroll_position()["view_offset"], 0)
        self.chat.next_step()
        self.assertGreater(transcript.scroll_position()["view_offset"], 0)

        self.chat.on_end_click()
        self.assertEqual(transcript.scroll_position()["view_offset"], 0)

    def test_canonical_fixture_has_five_user_prompts_and_exact_timing(self):
        with open(os.path.join(AGENT_CHAT_DIR, "scenario.json"), encoding="utf-8") as source:
            messages = json.load(source)["messages"]
        self.assertEqual(len(messages), 100)
        self.assertEqual(sum(message["role"] == "user" for message in messages), 5)
        self.assertEqual(sum(message["duration_ms"] for message in messages), 180_000)
        self.assertEqual({message["kind"] for message in messages},
                         {"user", "reasoning", "status", "activity", "text", "code", "diff", "table"})


if __name__ == "__main__":
    unittest.main()
