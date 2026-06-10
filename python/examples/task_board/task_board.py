"""ClickUp-style task list example."""

import copy
import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(_here)))
sys.path.insert(0, os.path.join(_here, "task_list"))
sys.path.insert(0, os.path.join(_here, "task_dialog"))

from dialogs import MessageBoxYesNo
from runtime import UIApplication
from task_board_ui import TaskBoardUI
from task_dialog import TaskDialog


ANY_FILTER = "Any"
DEFAULT_ASSIGNEE = "Ava"
DEFAULT_PRIORITY = "Med"
DEFAULT_STATUS = "Todo"
DONE_STATUS = "Done"
CANCELED_STATUS_TEXT = "Action canceled."
SEED_TASKS = (
    {
        "id": "t-101",
        "title": "Review invoice export",
        "status": "Doing",
        "assignee": "Ava",
        "priority": "High",
        "description": "Check exported columns and totals before finance signs off.",
        "done": False,
    },
    {
        "id": "t-102",
        "title": "Rewrite signup emails",
        "status": "Blocked",
        "assignee": "Bruno",
        "priority": "Med",
        "description": "Waiting for brand review on the onboarding sequence.",
        "done": False,
    },
    {
        "id": "t-103",
        "title": "Improve search ranking",
        "status": "Doing",
        "assignee": "Chen",
        "priority": "High",
        "description": "Tune ranking for exact title matches and recent activity.",
        "done": False,
    },
    {
        "id": "t-104",
        "title": "Draft data deletion policy",
        "status": "Todo",
        "assignee": "Dana",
        "priority": "Urgent",
        "description": "Prepare policy draft for privacy review.",
        "done": False,
    },
    {
        "id": "t-105",
        "title": "Update support templates",
        "status": "Todo",
        "assignee": "Ava",
        "priority": "Med",
        "description": "Refresh billing and account recovery snippets.",
        "done": False,
    },
    {
        "id": "t-106",
        "title": "Fix API contract tests",
        "status": "Blocked",
        "assignee": "Chen",
        "priority": "High",
        "description": "Coordinate schema change with partner sandbox.",
        "done": False,
    },
    {
        "id": "t-107",
        "title": "Clean left navigation",
        "status": "Todo",
        "assignee": "Bruno",
        "priority": "Low",
        "description": "Align labels and collapse unused spacing in admin views.",
        "done": False,
    },
    {
        "id": "t-108",
        "title": "Finalize launch checklist",
        "status": "Done",
        "assignee": "Dana",
        "priority": "Med",
        "description": "Checklist completed and reviewed by release owner.",
        "done": True,
    },
    {
        "id": "t-109",
        "title": "Define calendar sharing",
        "status": "Doing",
        "assignee": "Bruno",
        "priority": "Med",
        "description": "Document private, team, and organization visibility.",
        "done": False,
    },
    {
        "id": "t-110",
        "title": "Clean knowledge editor",
        "status": "Todo",
        "assignee": "Chen",
        "priority": "Low",
        "description": "Remove stale toolbar actions and simplify publish flow.",
        "done": False,
    },
    {
        "id": "t-111",
        "title": "Publish usage report",
        "status": "Doing",
        "assignee": "Dana",
        "priority": "Med",
        "description": "Send account summary to customer success leads.",
        "done": False,
    },
    {
        "id": "t-112",
        "title": "Retry failed imports",
        "status": "Blocked",
        "assignee": "Ava",
        "priority": "Urgent",
        "description": "Needs import worker patch before rerun.",
        "done": False,
    },
    {
        "id": "t-113",
        "title": "Tune digest settings",
        "status": "Todo",
        "assignee": "Bruno",
        "priority": "Med",
        "description": "Review default digest cadence for new workspaces.",
        "done": False,
    },
    {
        "id": "t-114",
        "title": "Verify account password reset cleanup",
        "status": "Done",
        "assignee": "Chen",
        "priority": "Low",
        "description": "Old reset token cleanup verified in staging.",
        "done": True,
    },
)


class TaskBoard(TaskBoardUI):
    """Task list with interactive scroll view rows and structured MCP tools."""

    def open(self):
        super().open()
        self._tasks = [copy.deepcopy(task) for task in SEED_TASKS]
        self._refresh_board()

    def on_search_change(self, value):
        self._refresh_board()

    def on_search_submit(self, value):
        self._refresh_board()

    def on_status_filter_change(self, value):
        self._refresh_board()

    def on_owner_filter_change(self, value):
        self._refresh_board()

    def on_new_task_btn_click(self):
        if self._app is not None:
            self._app.open(TaskDialog(on_close=self._save_task_from_dialog))

    def on_reset_filters_btn_click(self):
        self._reset_filters()
        self._refresh_board()

    def on_quit_btn_click(self):
        if self._app is not None:
            self._app._running = False

    def query_tasks(self, input=None):
        return {
            "tasks": [
                self._public_task(task)
                for task in self._matching_tasks(input or {})
            ]
        }

    def get_task(self, input=None):
        task_id = str((input or {}).get("id", ""))
        task = self._task_by_id(task_id)
        return {"task": self._public_task(task) if task else None}

    def update_task(self, input=None):
        data = input or {}
        task = self._task_by_id(str(data.get("id", "")))
        if task is None:
            return {"task": None}
        self._apply_task_update(task, data)
        self._refresh_board()
        return {"task": self._public_task(task)}

    def delete_task(self, input=None):
        task_id = str((input or {}).get("id", ""))
        before_count = len(self._tasks)
        self._tasks = [task for task in self._tasks if task.get("id") != task_id]
        deleted = len(self._tasks) != before_count
        if deleted:
            self._refresh_board()
            self.status.text = "Task deleted."
        return {"deleted": deleted}

    def _refresh_board(self):
        records = self._matching_tasks()
        self.board.child.set_tasks(
            records,
            on_open=self._open_task_dialog,
            on_delete=self._confirm_delete_task,
            on_change=self._task_changed,
        )
        self.status.text = f"{len(records)} visible / {len(self._tasks)} total"

    def _matching_tasks(self, filters=None):
        filters = filters or self._current_filters()
        title = str(filters.get("title", filters.get("name", "")) or "").strip().lower()
        status = str(filters.get("status", "") or ANY_FILTER)
        assignee = str(filters.get("assignee", filters.get("owner", "")) or ANY_FILTER)
        priority = str(filters.get("priority", "") or ANY_FILTER)
        done_filter = filters.get("done")
        result = []
        for task in self._tasks:
            haystack = " ".join([
                task.get("title", ""),
                task.get("description", ""),
                task.get("assignee", ""),
                task.get("priority", ""),
            ]).lower()
            if title and title not in haystack:
                continue
            if status != ANY_FILTER and task.get("status") != status:
                continue
            if assignee != ANY_FILTER and task.get("assignee") != assignee:
                continue
            if priority != ANY_FILTER and task.get("priority") != priority:
                continue
            if done_filter is not None and bool(task.get("done")) != bool(done_filter):
                continue
            result.append(task)
        return result

    def _current_filters(self):
        return {
            "title": self.search.value,
            "status": self.status_filter.selected_item or ANY_FILTER,
            "assignee": self.owner_filter.selected_item or ANY_FILTER,
        }

    def _open_task_dialog(self, task):
        if self._app is not None:
            self._app.open(TaskDialog(task=task, on_close=self._save_task_from_dialog))

    def _save_task_from_dialog(self, result):
        if result is None:
            self.status.text = CANCELED_STATUS_TEXT
            return
        task_id = result.get("id", "")
        task = self._task_by_id(task_id) if task_id else None
        if task is None:
            task = {
                "id": self._next_task_id(),
                "title": "",
                "status": DEFAULT_STATUS,
                "assignee": DEFAULT_ASSIGNEE,
                "priority": DEFAULT_PRIORITY,
                "description": "",
                "done": False,
            }
            self._tasks.insert(0, task)
            self._reset_filters()
        self._apply_task_update(task, result)
        self._refresh_board()
        self.status.text = "Task saved."

    def _confirm_delete_task(self, task):
        if self._app is None:
            return
        dialog = MessageBoxYesNo(
            header="Delete Task",
            message=f"Delete {task.get('title', 'this task')}?",
            on_close=lambda result, task_id=task.get("id", ""): self._delete_confirmed(task_id, result),
        )
        dialog._app = self._app
        self._app.open(dialog)

    def _delete_confirmed(self, task_id, result):
        if result != "yes":
            self.status.text = CANCELED_STATUS_TEXT
            return
        self.delete_task({"id": task_id})

    def _task_changed(self, task):
        self._sync_done_status(task)
        self.status.text = f"Updated {task.get('title', '')}"

    def _reset_filters(self):
        self.search.value = ""
        self.search.cursor_pos = 0
        self.status_filter.selected_item = ANY_FILTER
        self.owner_filter.selected_item = ANY_FILTER

    def _task_by_id(self, task_id):
        for task in self._tasks:
            if task.get("id") == task_id:
                return task
        return None

    def _apply_task_update(self, task, data):
        if "title" in data:
            task["title"] = str(data.get("title", "")).strip()
        if "status" in data:
            task["status"] = data.get("status") or DEFAULT_STATUS
        if "assignee" in data:
            task["assignee"] = data.get("assignee") or DEFAULT_ASSIGNEE
        if "priority" in data:
            task["priority"] = data.get("priority") or DEFAULT_PRIORITY
        if "description" in data:
            task["description"] = str(data.get("description", "")).strip()
        if "done" in data:
            task["done"] = bool(data.get("done"))
        self._sync_done_status(task)

    def _sync_done_status(self, task):
        if bool(task.get("done")):
            task["status"] = DONE_STATUS
        elif task.get("status") == DONE_STATUS:
            task["status"] = DEFAULT_STATUS

    def _next_task_id(self):
        numbers = []
        for task in self._tasks:
            try:
                numbers.append(int(str(task.get("id", "")).split("-")[-1]))
            except ValueError:
                continue
        return f"t-{max(numbers, default=100) + 1}"

    @staticmethod
    def _public_task(task):
        return {
            "id": task.get("id", ""),
            "title": task.get("title", ""),
            "status": task.get("status", ""),
            "assignee": task.get("assignee", ""),
            "priority": task.get("priority", ""),
            "description": task.get("description", ""),
            "done": bool(task.get("done", False)),
        }


def main():
    app = UIApplication()
    window = TaskBoard()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
