"""Expense tracker example using a dynamic ViewHost."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
_python_dir = os.path.dirname(os.path.dirname(_here))
_project_dir = os.path.dirname(_python_dir)
sys.path.insert(0, os.path.join(_project_dir, "src"))
sys.path.insert(0, _python_dir)
sys.path.insert(0, os.path.join(_python_dir, "dialogs"))
sys.path.insert(0, os.path.join(_here, "expenses_list"))
sys.path.insert(0, os.path.join(_here, "expense_row"))

from dialogs import MessageBoxYesNo
from runtime import UIApplication
from categories_view_ui import CategoriesViewUI
from expenses_view_ui import ExpensesViewUI
from settings_view_ui import SettingsViewUI
from expense_tracker_ui import ExpenseTrackerUI


DEFAULT_CATEGORIES = ["Food", "Transport", "Home", "Health", "Other"]
DEFAULT_CURRENCY = "EUR"
DEFAULT_SHOW_PAID_EXPENSES = True
DEFAULT_MONTHLY_LIMIT = 500
SEED_EXPENSES = [
    {"id": "e-1", "item": "Groceries", "amount": 42.80, "category": "Food", "paid": True},
    {"id": "e-2", "item": "Bus ticket", "amount": 2.50, "category": "Transport", "paid": True},
    {"id": "e-3", "item": "Light bulbs", "amount": 8.90, "category": "Home", "paid": False},
    {"id": "e-4", "item": "Pharmacy", "amount": 14.20, "category": "Health", "paid": True},
    {"id": "e-5", "item": "Notebook", "amount": 5.40, "category": "Other", "paid": False},
    {"id": "e-6", "item": "Internet bill", "amount": 29.99, "category": "Home", "paid": True},
    {"id": "e-7", "item": "Team lunch", "amount": 18.50, "category": "Food", "paid": False},
    {"id": "e-8", "item": "Train pass", "amount": 24.00, "category": "Transport", "paid": True},
    {"id": "e-9", "item": "Vitamins", "amount": 11.75, "category": "Health", "paid": False},
    {"id": "e-10", "item": "Desk lamp", "amount": 21.30, "category": "Home", "paid": False},
    {"id": "e-11", "item": "Cloud storage", "amount": 9.99, "category": "Other", "paid": True},
    {"id": "e-12", "item": "Replacement keyboard", "amount": 44.00, "category": "Other", "paid": False},
]


class ExpensesView(ExpensesViewUI):
    def __init__(self, shell):
        super().__init__()
        self._shell = shell

    def open(self):
        super().open()
        self.refresh()

    def refresh(self):
        self.currency_label.text = self._shell.currency
        self.category_input._options = list(self._shell.categories)
        if self.category_input.selected_item not in self._shell.categories:
            self.category_input.selected_item = self._shell.categories[0]
        self.expenses.child.set_expenses(
            self._shell.visible_expenses(),
            self._shell.categories,
            on_delete=self._shell.confirm_delete_expense,
            on_change=self._shell.expense_changed,
        )

    def on_add_expense_btn_click(self):
        self._shell.add_expense(
            self.item_input.value.strip(),
            self.amount_input.value,
            self.category_input.selected_item,
        )
        self.item_input.value = ""
        self.amount_input.value = 0
        self.refresh()


class CategoriesView(CategoriesViewUI):
    def __init__(self, shell):
        super().__init__()
        self._shell = shell

    def open(self):
        super().open()
        self.refresh()

    def refresh(self, selected=None):
        self.categories._options = list(self._shell.categories)
        preferred = selected if selected in self._shell.categories else self.categories.selected_item
        if preferred not in self._shell.categories:
            preferred = self._shell.categories[0] if self._shell.categories else None
        self.categories.selected_items = [preferred] if preferred else []

    def on_add_category_btn_click(self):
        added = self._shell.add_category(self.category_name.value.strip())
        self.category_name.value = ""
        self.refresh(selected=added)

    def on_remove_category_btn_click(self):
        self._shell.remove_category(self.categories.selected_item)
        self.refresh()


class SettingsView(SettingsViewUI):
    def __init__(self, shell):
        super().__init__()
        self._shell = shell

    def open(self):
        super().open()
        self.refresh()

    def refresh(self):
        if self._shell.currency in self.currency._options:
            self.currency.selected_item = self._shell.currency
        self.show_paid.value = self._shell.show_paid_expenses
        self.monthly_limit.value = self._shell.monthly_limit

    def on_currency_change(self, value):
        self._shell.set_currency(value)

    def on_show_paid_change(self, value):
        self._shell.set_show_paid_expenses(value)

    def on_monthly_limit_change(self, value):
        self._shell.set_monthly_limit(value)

    def on_monthly_limit_submit(self, value):
        self._shell.set_monthly_limit(value)

    def on_reset_btn_click(self):
        self._shell.reset_demo_data()


class ExpenseTracker(ExpenseTrackerUI):
    def __init__(self):
        super().__init__()
        self.categories = list(DEFAULT_CATEGORIES)
        self.expenses = [dict(record) for record in SEED_EXPENSES]
        self.currency = DEFAULT_CURRENCY
        self.show_paid_expenses = DEFAULT_SHOW_PAID_EXPENSES
        self.monthly_limit = DEFAULT_MONTHLY_LIMIT
        self._next_expense_number = len(self.expenses) + 1
        self.show_expenses()

    def open(self):
        super().open()
        self._select_current_nav_button()

    def on_expenses_btn_click(self):
        self.show_expenses()

    def on_categories_btn_click(self):
        self.show_categories()

    def on_settings_btn_click(self):
        self.show_settings()

    def on_quit_btn_click(self):
        if self._app is not None:
            self._app._running = False

    def show_expenses(self):
        self._select_nav_button(self.expenses_btn)
        self.main.set_view(ExpensesView(self))
        self.set_focus(self.expenses_btn)

    def show_categories(self):
        self._select_nav_button(self.categories_btn)
        self.main.set_view(CategoriesView(self))
        self.set_focus(self.categories_btn)

    def show_settings(self):
        self._select_nav_button(self.settings_btn)
        self.main.set_view(SettingsView(self))
        self.set_focus(self.settings_btn)

    def _select_current_nav_button(self):
        current = self.main.current_view()
        if isinstance(current, CategoriesView):
            self._select_nav_button(self.categories_btn)
        elif isinstance(current, SettingsView):
            self._select_nav_button(self.settings_btn)
        else:
            self._select_nav_button(self.expenses_btn)

    def _select_nav_button(self, target):
        normal_style = self._normal_nav_button_style(target)
        active_style = self._active_nav_button_style(target)
        for button in (self.expenses_btn, self.categories_btn, self.settings_btn):
            button.style = normal_style.copy()
        target.style = active_style

    def _active_nav_button_style(self, button):
        style = button.style.copy()
        if button.focus_style is not None:
            style.merge(button.focus_style)
        return style

    def _normal_nav_button_style(self, target):
        current = self.main.current_view()
        candidates = (
            (self.expenses_btn, ExpensesView),
            (self.categories_btn, CategoriesView),
            (self.settings_btn, SettingsView),
        )
        for button, view_type in candidates:
            if button is not target and not isinstance(current, view_type):
                return button.style.copy()
        return target.style.copy()

    def set_currency(self, currency):
        self.currency = currency or DEFAULT_CURRENCY
        current = self.main.current_view()
        if isinstance(current, ExpensesView):
            current.refresh()

    def set_show_paid_expenses(self, value):
        self.show_paid_expenses = bool(value)
        current = self.main.current_view()
        if isinstance(current, ExpensesView):
            current.refresh()

    def set_monthly_limit(self, value):
        self.monthly_limit = float(value)

    def visible_expenses(self):
        if self.show_paid_expenses:
            return self.expenses
        return [expense for expense in self.expenses if not expense.get("paid", False)]

    def add_expense(self, item, amount, category):
        item = item or "Untitled expense"
        self.expenses.insert(0, {
            "id": f"e-{self._next_expense_number}",
            "item": item,
            "amount": float(amount),
            "category": category if category in self.categories else self.categories[0],
            "paid": False,
        })
        self._next_expense_number += 1

    def expense_changed(self, _record):
        pass

    def confirm_delete_expense(self, record):
        if self._app is None:
            self.delete_expense(record.get("id"), "yes")
            return
        message = MessageBoxYesNo(
            header="Delete Expense",
            message=f"Delete {record.get('item', 'expense')}?",
            on_close=lambda result, expense_id=record.get("id"): self.delete_expense(expense_id, result),
        )
        message._app = self._app
        self._app.open(message)

    def delete_expense(self, expense_id, result):
        if result != "yes":
            return
        self.expenses = [expense for expense in self.expenses if expense.get("id") != expense_id]
        current = self.main.current_view()
        if isinstance(current, ExpensesView):
            current.refresh()

    def add_category(self, name):
        if not name:
            return None
        if name not in self.categories:
            self.categories.append(name)
        return name

    def remove_category(self, name):
        if not name or len(self.categories) <= 1:
            return
        if name in self.categories:
            self.categories.remove(name)
        fallback = self.categories[0]
        for expense in self.expenses:
            if expense.get("category") == name:
                expense["category"] = fallback

    def reset_demo_data(self):
        self.categories = list(DEFAULT_CATEGORIES)
        self.expenses = [dict(record) for record in SEED_EXPENSES]
        self.currency = DEFAULT_CURRENCY
        self.show_paid_expenses = DEFAULT_SHOW_PAID_EXPENSES
        self.monthly_limit = DEFAULT_MONTHLY_LIMIT
        self._next_expense_number = len(self.expenses) + 1
        current = self.main.current_view()
        if isinstance(current, ExpensesView):
            current.refresh()
        elif isinstance(current, CategoriesView):
            current.refresh()
        elif isinstance(current, SettingsView):
            current.refresh()
        self._select_current_nav_button()


def main():
    if "--logic-test" in sys.argv or "--smoke" in sys.argv:
        shell = ExpenseTracker()
        assert shell.main.current_view() is not None
        assert isinstance(shell.main.current_view(), ExpensesView)
        shell.add_expense("Coffee", 3.5, "Food")
        assert shell.expenses[0]["item"] == "Coffee"
        shell.delete_expense(shell.expenses[0]["id"], "yes")
        assert shell.expenses[0]["item"] != "Coffee"
        shell.show_categories()
        assert isinstance(shell.main.current_view(), CategoriesView)
        shell.show_settings()
        assert isinstance(shell.main.current_view(), SettingsView)
        shell.set_currency("USD")
        assert shell.currency == "USD"
        shell.set_show_paid_expenses(False)
        assert all(not expense["paid"] for expense in shell.visible_expenses())
        shell.set_monthly_limit(750)
        assert shell.monthly_limit == 750.0
        return 0

    app = UIApplication()
    app.open(ExpenseTracker())
    app.run()
    return 0


if __name__ == "__main__":
    sys.exit(main())
