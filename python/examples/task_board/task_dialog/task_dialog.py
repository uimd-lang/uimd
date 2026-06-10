"""Task edit dialog for the ClickUp-style task list example."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(_here)))))

from task_dialog_ui import TaskDialogUI


DEFAULT_ASSIGNEE = "Ava"
DEFAULT_PRIORITY = "Med"
DEFAULT_STATUS = "Todo"


class TaskDialog(TaskDialogUI):
    """Dialog that returns one task payload."""

    def __init__(self, task=None, on_close=None):
        super().__init__()
        self._task = dict(task or {})
        self._on_close = on_close
        self.result = None

    def open(self):
        super().open()
        if self._task:
            self.header.text = "Edit task"
            self.name.value = self._task.get("title", "")
            self.project_status.selected_item = self._task.get("status", DEFAULT_STATUS)
            self.owner.selected_item = self._task.get("assignee", DEFAULT_ASSIGNEE)
            self.priority.selected_item = self._task.get("priority", DEFAULT_PRIORITY)
            self.note.value = self._task.get("description", "")
        else:
            self.header.text = "New task"

    def on_add_btn_click(self):
        title = self.name.value.strip()
        if not title:
            self.message.text = "Title is required."
            return
        self.result = {
            "id": self._task.get("id", ""),
            "title": title,
            "status": self.project_status.selected_item or DEFAULT_STATUS,
            "assignee": self.owner.selected_item or DEFAULT_ASSIGNEE,
            "priority": self.priority.selected_item or DEFAULT_PRIORITY,
            "description": self.note.value.strip(),
            "done": (self.project_status.selected_item == "Done"),
        }
        self._close(self.result)

    def on_cancel_btn_click(self):
        self.result = None
        self._close(None)

    def _close(self, result):
        if self._app is not None:
            self._app.close_current()
        if self._on_close is not None:
            self._on_close(result)
