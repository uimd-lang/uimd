"""Scrollable activity feed panel."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(_here))))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "activity_item"))

from activity_feed_panel_ui import ActivityFeedPanelUI
from activity_item import ActivityItem


class ActivityFeedPanel(ActivityFeedPanelUI):
    """Activity feed scroll view with reusable child items."""

    def clear_activities(self):
        self.clear_children()

    def append_activity(self, timestamp, event_type, message, show_timestamp=True):
        item = ActivityItem()
        item.parent = self
        item._app = self._app
        item.mode = "expand_width"
        item.set_activity(timestamp, event_type, message, show_timestamp=show_timestamp)
        item.open()
        self.add_child(item)
        return item

    def set_auto_scroll(self, enabled):
        self._auto_scroll = bool(enabled)
        if self._auto_scroll:
            self.scroll_to_bottom()

    def _render_child(self, child, width):
        _natural_width, natural_height = child.get_natural_size()
        child.resize(width, max(1, natural_height))
        if hasattr(child, "message"):
            message_width = max(1, int(getattr(child.message, "width", 1) or 1))
            message_rows = self._wrapped_line_count(child.message.text, message_width)
            message_height = max(1, int(getattr(child.message, "height", 1) or 1))
            if message_rows > message_height:
                child.resize(width, max(1, natural_height + message_rows - message_height))
        if child._resolved_cells_by_name:
            actual_height = max(
                info["row"] + info["height"]
                for info in child._resolved_cells_by_name.values()
            )
            if actual_height > child._window_height:
                child.resize(width, actual_height)
        return child.render()

    def _wrapped_line_count(self, text, width):
        count = 0
        for line in str(text).splitlines() or [""]:
            count += max(1, (len(line) + width - 1) // width)
        return max(1, count)
