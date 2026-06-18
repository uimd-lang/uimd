"""Reusable task board filter panel."""

from task_filters_ui import TaskFiltersUI


class TaskFilters(TaskFiltersUI):
    """Filter control that applies values only through its action buttons."""

    def __init__(self):
        super().__init__()
        self._on_apply = None
        self._on_reset = None

    def configure(self, on_apply=None, on_reset=None):
        self._on_apply = on_apply
        self._on_reset = on_reset

    def on_apply_filters_btn_click(self):
        if self._on_apply is not None:
            self._on_apply()

    def on_reset_filters_btn_click(self):
        if self._on_reset is not None:
            self._on_reset()
