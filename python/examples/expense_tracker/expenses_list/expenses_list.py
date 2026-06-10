"""Expense list scroll view."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(_here))))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "expense_row"))

from expense_row import ExpenseRow
from expenses_list_ui import ExpensesListUI


class ExpensesList(ExpensesListUI):
    """Scroll view that hosts interactive expense rows."""

    def clear_expenses(self):
        self.clear_children()

    def set_expenses(self, records, categories, on_delete=None, on_change=None):
        self.clear_expenses()
        for record in records:
            row = ExpenseRow()
            row.parent = self
            row._app = self._app
            row.mode = "expand_width"
            row.configure(record, categories, on_delete=on_delete, on_change=on_change)
            row.open()
            row._sync_from_record()
            self.add_child(row)
        self.scroll_to_top()

    def _render_child(self, child, width):
        _natural_width, natural_height = child.get_natural_size()
        child.resize(width, max(1, natural_height))
        return child.render()
