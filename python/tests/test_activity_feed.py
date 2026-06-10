"""Regression tests for the activity_feed example."""

import os
import re
import sys
import unittest

PROJECT_ROOT = os.path.join(os.path.dirname(__file__), "..")
ACTIVITY_FEED_DIR = os.path.join(PROJECT_ROOT, "examples", "activity_feed")
sys.path.insert(0, PROJECT_ROOT)
sys.path.insert(0, ACTIVITY_FEED_DIR)

from activity_feed import ActivityFeed, INITIAL_SAMPLE_COUNT, SAMPLE_ACTIVITIES
from runtime import UIApplication


ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")


class TestActivityFeed(unittest.TestCase):
    """Activity feed example behavior."""

    def setUp(self):
        self.app = UIApplication(width=80, height=30)
        self.feed = ActivityFeed()
        self.app.open(self.feed)

    def plain_feed(self):
        return ANSI_RE.sub("", "\n".join(self.feed.feed.child.render()))

    def test_initial_feed_uses_sample_items(self):
        self.assertEqual(self.feed.activity_count(), INITIAL_SAMPLE_COUNT)
        text = self.plain_feed()
        self.assertIn("Workspace opened", text)
        self.assertIn("Report export queued", text)

    def test_layout_expands_feed_and_keeps_combobox_closed_row_compact(self):
        self.feed.resize(80, 30)

        self.assertGreater(self.feed.feed.height, 10)
        self.assertEqual(self.feed.activity_type.height, 1)
        self.assertEqual(self.feed.message.height, 3)
        self.assertEqual(
            self.feed._element_focus_rect(self.feed.message)["left"],
            self.feed._element_focus_rect(self.feed.activity_type)["left"],
        )
        self.assertEqual(
            self.feed._element_focus_rect(self.feed.message)["top"],
            self.feed._element_focus_rect(self.feed.message_label)["top"],
        )

    def test_header_trailing_buttons_stay_right_of_title_when_expanded(self):
        self.feed.resize(112, 38)

        title_rect = self.feed._element_focus_rect(self.feed.header_title)
        settings_rect = self.feed._element_focus_rect(self.feed.settings_button)
        quit_rect = self.feed._element_focus_rect(self.feed.quit_button)

        self.assertLessEqual(title_rect["right"], settings_rect["left"])
        self.assertLess(settings_rect["right"], quit_rect["left"])

    def test_feed_panel_has_vertical_padding(self):
        rendered = ANSI_RE.sub("", "\n".join(self.feed.feed.child.render())).splitlines()

        self.assertTrue(rendered[0].strip() == "")
        self.assertIn("Workspace opened", "\n".join(rendered[1:]))

    def test_window_render_cells_keeps_feed_items_visible(self):
        rendered = "\n".join(
            "".join(cell.text for cell in row)
            for row in self.feed.render_cells()
        )

        self.assertIn("Workspace opened", rendered)
        self.assertIn("Report export queued", rendered)
        self.assertEqual(self.feed.feed.child._cached_child_heights, [2, 2])

    def test_activity_type_colors_left_marker(self):
        self.feed.clear_activities()
        item = self.feed.append_activity("Warning", "Queue depth exceeded")
        item.render()

        self.assertEqual(
            item._cell_style("marker").background_texture_color.hex,
            "#facc15",
        )

    def test_add_appends_selected_activity_and_clears_input(self):
        self.feed.clear_activities()
        self.feed.activity_type.selected_item = "Warning"
        self.feed.message.value = "Queue depth exceeded"

        item = self.feed.add_current_activity()

        self.assertIsNotNone(item)
        self.assertEqual(self.feed.message.value, "")
        self.assertEqual(self.feed.status.text, "1 activities")
        self.assertEqual(self.feed.activity_count(), 1)
        self.assertEqual(item.event_type.text, "Warning")
        self.assertEqual(item.message.text, "Queue depth exceeded")
        self.assertRegex(item.timestamp.text, r"^\d\d:\d\d$")

    def test_add_requires_message(self):
        self.feed.clear_activities()
        self.feed.message.value = "   "

        item = self.feed.add_current_activity()

        self.assertIsNone(item)
        self.assertEqual(self.feed.activity_count(), 0)
        self.assertEqual(self.feed.status.text, "Enter an activity message.")

    def test_seed_and_clear_update_feed(self):
        self.feed.clear_activities()

        self.feed.seed_activities()

        self.assertEqual(self.feed.activity_count(), len(SAMPLE_ACTIVITIES))
        self.assertIn("Background sync finished", self.plain_feed())

        self.feed.clear_activities()

        self.assertEqual(self.feed.activity_count(), 0)
        self.assertEqual(self.feed.status.text, "Feed cleared")

    def test_short_item_uses_fit_content_height(self):
        self.feed.clear_activities()
        item = self.feed.append_activity("Info", "Short")

        rendered = self.feed.feed.child._render_child(item, self.feed.feed.width)

        self.assertLessEqual(len(rendered), 4)
        self.assertEqual(item.message.text, "Short")

    def test_long_item_wraps_without_runtime_layout_constants(self):
        self.feed.clear_activities()
        item = self.feed.append_activity("Note", "alpha beta gamma " * 12)

        rendered = self.feed.feed.child._render_child(item, 24)
        plain = ANSI_RE.sub("", "\n".join(rendered))

        self.assertGreater(len(rendered), 3)
        self.assertIn("alpha", plain)
        self.assertIn("gamma", plain)

    def test_settings_save_applies_future_item_behavior(self):
        self.feed.clear_activities()

        self.feed._settings_closed({
            "auto_scroll": False,
            "show_timestamps": False,
            "default_type": "Deploy",
        })

        self.assertFalse(self.feed.auto_scroll)
        self.assertFalse(self.feed.show_timestamps)
        self.assertEqual(self.feed.default_type, "Deploy")
        self.assertEqual(self.feed.activity_type.selected_item, "Deploy")
        self.assertFalse(self.feed.feed.child._auto_scroll)
        item = self.feed.append_activity(self.feed.activity_type.selected_item, "Release finished")
        self.assertEqual(item.timestamp.text, "")
        self.assertEqual(item.event_type.text, "Deploy")

    def test_settings_cancel_leaves_existing_values(self):
        previous = (self.feed.auto_scroll, self.feed.show_timestamps, self.feed.default_type)

        self.feed._settings_closed(None)

        self.assertEqual((self.feed.auto_scroll, self.feed.show_timestamps, self.feed.default_type), previous)
        self.assertEqual(self.feed.status.text, "Settings canceled")

    def test_quit_button_requests_app_shutdown(self):
        self.app._running = True

        self.feed._dispatch_element_changed(self.feed.quit_button, True)

        self.assertFalse(self.app._running)


if __name__ == "__main__":
    unittest.main()
