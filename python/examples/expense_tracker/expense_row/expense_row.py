"""Expense row component."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

from runtime import UIApplication
from expense_row_ui import ExpenseRowUI


class ExpenseRow(ExpenseRowUI):
    """Interactive row used inside the expenses scroll view."""

    def configure(self, record, categories, on_delete=None, on_change=None):
        self.record = record
        self._on_delete = on_delete
        self._on_change = on_change
        self._categories = list(categories)
        self._sync_from_record()

    def on_paid_change(self, value):
        self.record["paid"] = bool(value)
        if self._on_change:
            self._on_change(self.record)

    def on_category_change(self, value):
        self.record["category"] = str(value)
        if self._on_change:
            self._on_change(self.record)

    def on_delete_btn_click(self):
        if self._on_delete:
            self._on_delete(self.record)

    def _sync_from_record(self):
        self.item.text = self.record.get("item", "")
        self.amount.text = f"{float(self.record.get('amount', 0)):.2f}"
        self.paid.value = bool(self.record.get("paid", False))
        self.category._options = list(self._categories)
        category = self.record.get("category") or (self._categories[0] if self._categories else "")
        self.category.selected_item = category if category in self._categories else (self._categories[0] if self._categories else "")


def main():
    app = UIApplication()
    window = ExpenseRow()
    window.configure({"item": "Coffee", "amount": 3.5, "category": "Food", "paid": True}, ["Food", "Other"])
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
