"""Activity feed item logic."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

from runtime import UIApplication
from activity_item_ui import ActivityItemUI


MARKER_STYLE_BY_TYPE = {
    "info": "@marker_info",
    "task": "@marker_task",
    "warning": "@marker_warning",
    "deploy": "@marker_deploy",
    "note": "@marker_note",
}


class ActivityItem(ActivityItemUI):
    """Reusable activity feed item."""

    def set_activity(self, timestamp, event_type, message, show_timestamp=True):
        self.timestamp.text = str(timestamp) if show_timestamp else ""
        self.event_type.text = str(event_type)
        self.message.text = str(message)
        self._apply_marker_style(str(event_type))

    def _apply_marker_style(self, event_type):
        selector = MARKER_STYLE_BY_TYPE.get(event_type.strip().lower(), "@marker_info")
        marker_style = self._compiled_styles.get("@marker", {})
        type_style = self._compiled_styles.get(selector, {})
        if "background-texture-color" in type_style:
            marker_style["background-texture-color"] = type_style["background-texture-color"]
            self.invalidate_layout()


def main():
    app = UIApplication()
    window = ActivityItem()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
