"""Task row component."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

from runtime import UIApplication
from task_row_ui import TaskRowUI


class TaskRow(TaskRowUI):
    """Interactive task row used inside the task list scroll view."""

    def configure(self, record, on_open=None, on_delete=None, on_change=None):
        self.record = record
        self._on_open = on_open
        self._on_delete = on_delete
        self._on_change = on_change
        self._sync_from_record()

    def on_open_btn_click(self):
        if self._on_open:
            self._on_open(self.record)

    def on_delete_btn_click(self):
        if self._on_delete:
            self._on_delete(self.record)

    def on_done_change(self, value):
        self.record["done"] = bool(value)
        if self._on_change:
            self._on_change(self.record)

    def _sync_from_record(self):
        self.task_title.text = self.record.get("title", "")
        self.meta.text = " / ".join([
            self.record.get("status", ""),
            self.record.get("assignee", ""),
            self.record.get("priority", ""),
        ])
        self.done.value = bool(self.record.get("done", False))


def main():
    app = UIApplication()
    window = TaskRow()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
