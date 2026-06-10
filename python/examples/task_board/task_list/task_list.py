"""Task list scroll view."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(_here))))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "task_row"))

from task_list_ui import TaskListUI
from task_row import TaskRow


class TaskList(TaskListUI):
    """Scroll view that hosts interactive task rows."""

    def clear_tasks(self):
        self.clear_children()

    def set_tasks(self, records, on_open=None, on_delete=None, on_change=None):
        self.clear_tasks()
        for record in records:
            row = TaskRow()
            row.parent = self
            row._app = self._app
            row.mode = "expand_width"
            row.configure(
                record,
                on_open=on_open,
                on_delete=on_delete,
                on_change=on_change,
            )
            row.open()
            self.add_child(row)
        self.scroll_to_top()

    def _render_child(self, child, width):
        _natural_width, natural_height = child.get_natural_size()
        child.resize(width, max(1, natural_height))
        if child._resolved_cells_by_name:
            actual_height = max(
                info["row"] + info["height"]
                for info in child._resolved_cells_by_name.values()
            )
            if actual_height > child._window_height:
                child.resize(width, actual_height)
        return child.render()
