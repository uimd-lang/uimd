"""Activity feed example."""

import os
import sys
from datetime import datetime

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(_here)))
sys.path.insert(0, os.path.join(_here, "settings"))

from runtime import UIApplication
from activity_feed_ui import ActivityFeedUI
from settings import Settings


DEFAULT_ACTIVITY_TYPE = "Info"
INITIAL_SAMPLE_COUNT = 2
SAMPLE_ACTIVITIES = (
    ("Info", "Workspace opened"),
    ("Task", "Report export queued"),
    ("Deploy", "Version 1.4 deployed"),
    ("Warning", "Review queue is almost full"),
    ("Note", "Follow-up scheduled"),
    ("Task", "Background sync finished"),
)


class ActivityFeed(ActivityFeedUI):
    """Window logic for dynamically appending activity feed items."""

    def __init__(self):
        super().__init__()
        self.auto_scroll = True
        self.show_timestamps = True
        self.default_type = DEFAULT_ACTIVITY_TYPE

    def open(self):
        super().open()
        self.activity_type.selected_item = self.default_type
        self.feed.child.set_auto_scroll(self.auto_scroll)
        if not self.feed.child._children:
            self._seed_initial()

    def on_add_button_click(self):
        self.add_current_activity()

    def on_seed_button_click(self):
        self.seed_activities()

    def on_clear_button_click(self):
        self.clear_activities()

    def on_settings_button_click(self):
        self.open_settings()

    def on_quit_button_click(self):
        self.quit()

    def on_message_submit(self, value):
        self.message.value = value

    def add_current_activity(self):
        message = (self.message.value or "").strip()
        if not message:
            self.status.text = "Enter an activity message."
            return None
        item = self.append_activity(self.activity_type.selected_item or self.default_type, message)
        self.message.value = ""
        self.message.cursor_pos = 0
        self.status.text = f"{self.activity_count()} activities"
        return item

    def append_activity(self, event_type, message):
        return self.feed.child.append_activity(
            self._current_timestamp(),
            event_type or self.default_type,
            message,
            show_timestamp=self.show_timestamps,
        )

    def seed_activities(self):
        for event_type, message in SAMPLE_ACTIVITIES:
            self.append_activity(event_type, message)
        self.status.text = f"{self.activity_count()} activities"

    def clear_activities(self):
        self.feed.child.clear_activities()
        self.status.text = "Feed cleared"

    def activity_count(self):
        return len(self.feed.child._children)

    def open_settings(self):
        settings = Settings()
        settings.configure(
            auto_scroll=self.auto_scroll,
            show_timestamps=self.show_timestamps,
            default_type=self.default_type,
        )
        settings._on_close_callback = self._settings_closed
        self._app.open(settings)

    def quit(self):
        if self._app is not None:
            self._app._running = False

    def _settings_closed(self, result):
        if result is None:
            self.status.text = "Settings canceled"
            return
        self.auto_scroll = bool(result["auto_scroll"])
        self.show_timestamps = bool(result["show_timestamps"])
        self.default_type = result["default_type"] or DEFAULT_ACTIVITY_TYPE
        self.activity_type.selected_item = self.default_type
        self.feed.child.set_auto_scroll(self.auto_scroll)
        self.status.text = "Settings saved"

    def _seed_initial(self):
        for event_type, message in SAMPLE_ACTIVITIES[:INITIAL_SAMPLE_COUNT]:
            self.append_activity(event_type, message)
        self.status.text = f"{self.activity_count()} activities"

    def _current_timestamp(self):
        return datetime.now().strftime("%H:%M")


def main():
    app = UIApplication()
    window = ActivityFeed()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
