"""Auto-generated UI code for expenses_view_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIControl

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'expenses_list'))
from expenses_list import ExpensesList


class ExpensesViewUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Expenses View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: "Expense entry view with a dynamic scrollable list."\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - expenses_list/expenses_list\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Expenses\n\nitem_input:\n  type: textinput\n  value: ""\n  maxlength: 80\n\namount_input:\n  type: numberinput\n  value: 0\n  format: "0.00"\n\ncategory_input:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ncurrency_label:\n  type: label\n  text: EUR\n\nadd_expense_btn:\n  type: button\n  title: Add expense\n\nexpenses:\n  type: expenses_list\n  cpp-class: expense_tracker_example::ExpensesList\n  cpp-header: "expenses_list/expenses_list.hpp"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#0b1f33"\n@view_title:\n  color: "#bfdbfe"\n@currency_label:\n  color: "#bfdbfe"\n@expenses:\n  background: "#18364d"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| "Item:"     item_input.................................... |\n|                                                            |\n| "Amount:"   amount_input............. currency_label...... |\n|                                                            |\n| "Category:" category_input................................ |\n|                                                            |\n| add_expense_btn........................................... |\n|                                                            |\n| expenses..**.............................................. |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Expense entry view with a dynamic scrollable list.'}, 'elements': {'view_title': {'role': 'text', 'description': 'Expenses', 'expose': True}, 'item_input': {'role': 'input', 'description': 'item_input', 'expose': True}, 'amount_input': {'role': 'input', 'description': 'amount_input', 'expose': True}, 'category_input': {'role': 'selection', 'description': 'category_input', 'expose': True}, 'currency_label': {'role': 'text', 'description': 'EUR', 'expose': True}, 'add_expense_btn': {'role': 'action', 'description': 'Add expense', 'expose': True}, 'expenses': {'role': 'text', 'description': 'expenses', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Expenses View",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.add_expense_btn = self.create_element("add_expense_btn", "button", title='Add expense')
        self.amount_input = self.create_element("amount_input", "numberinput", value=0)
        self.category_input = self.create_element("category_input", "combobox", options=['Food', 'Transport', 'Home', 'Health', 'Other'], selected_item='Food')
        self.currency_label = self.create_element("currency_label", "label", text='EUR')
        self.expenses = self.create_element("expenses", "uielement", layout_class=ExpensesList)
        self.item_input = self.create_element("item_input", "textinput", value='', maxlength=80)
        self.view_title = self.create_element("view_title", "label", text='Expenses')
        self._label_1 = self.create_element("_label_1", "label", text='Item:')
        self._label_3 = self.create_element("_label_3", "label", text='Amount:')
        self._label_6 = self.create_element("_label_6", "label", text='Category:')
    def on_item_input_change(self, value):
        pass

    def on_item_input_submit(self, value):
        pass

    def on_amount_input_change(self, value):
        pass

    def on_amount_input_submit(self, value):
        pass

    def on_category_input_change(self, value):
        pass

    def on_add_expense_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.item_input:
            self.on_item_input_change(value)
        elif element is self.amount_input:
            self.on_amount_input_change(value)
        elif element is self.add_expense_btn:
            self.on_add_expense_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.item_input:
            self.on_item_input_submit(value)
        elif element is self.amount_input:
            self.on_amount_input_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.category_input:
            self.on_category_input_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "view_title": {
        "type": 'label',
        "text": 'Expenses',
    },
    "item_input": {
        "type": 'textinput',
        "value": '',
        "maxlength": 80,
    },
    "amount_input": {
        "type": 'numberinput',
        "value": 0,
        "format": '0.00',
    },
    "category_input": {
        "type": 'combobox',
        "options": ['Food', 'Transport', 'Home', 'Health', 'Other'],
        "selected_item": 'Food',
    },
    "currency_label": {
        "type": 'label',
        "text": 'EUR',
    },
    "add_expense_btn": {
        "type": 'button',
        "title": 'Add expense',
    },
    "expenses": {
        "type": 'expenses_list',
        "cpp-class": 'expense_tracker_example::ExpensesList',
        "cpp-header": 'expenses_list/expenses_list.hpp',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#0b1f33',
        "border-color": 'transparent',
        "border-width": 0,
    },
    "label": {
        "background": 'transparent',
        "color": '#cbd5e1',
        "cursor-background": '#b8d7ff',
        "cursor-color": '#000000',
    },
    "@dialog_header": {
        "text-align": 'center',
        "background": '#1d314c',
    },
    "@window_header": {
        "text-align": 'center',
        "background": '#1d314c',
    },
    "infolabel": {
        "background": 'transparent',
        "color": '#b8c4d8',
    },
    "textinput": {
        "background": '#0d1524',
        "color": '#e5e7eb',
        "focus-background": '#243a5c',
        "focus-color": '#ffffff',
        "edit-background": '#2d456d',
        "edit-color": '#ffffff',
        "cursor-background": '#ffffff',
        "cursor-color": '#0e1117',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
    "numberinput": {
        "background": '#0d1524',
        "color": '#e5e7eb',
        "focus-background": '#243a5c',
        "focus-color": '#ffffff',
        "edit-background": '#2d456d',
        "edit-color": '#ffffff',
        "cursor-background": '#ffffff',
        "cursor-color": '#0e1117',
    },
    "combobox": {
        "background": '#0d1524',
        "color": '#e5e7eb',
        "focus-background": '#243a5c',
        "focus-color": '#ffffff',
        "edit-background": '#2d456d',
        "edit-color": '#e5e7eb',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
    "listbox": {
        "background": '#0d1524',
        "color": '#cbd5e1',
        "focus-background": '#1d2f4d',
        "focus-color": '#ffffff',
        "edit-background": '#243a5c',
        "edit-color": '#cbd5e1',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
    "uielement": {
        "focus-background": '#ffffff14',
        "edit-background": '#ffffff18',
        "selected-background": '#ffffff26',
        "selected-color": '#ffffff',
    },
    "uiscrollview": {
        "focus-background": '#ffffff14',
        "edit-background": '#ffffff18',
        "selected-background": '#ffffff26',
        "selected-color": '#ffffff',
    },
    "checkbox": {
        "background": 'transparent',
        "color": '#b8c4d8',
        "focus-color": '#66aaff',
        "checked-background": 'transparent',
        "checked-color": '#44cc88',
        "unchecked-background": 'transparent',
    },
    "button": {
        "background": '#334155',
        "color": '#e2e8f0',
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "@view_title": {
        "color": '#bfdbfe',
    },
    "@currency_label": {
        "color": '#bfdbfe',
    },
    "@expenses": {
        "background": '#18364d',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 58,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 14,
        'width': 58,
        'height': 1,
        'type': 'label',
        'name': 'view_title'
    },
    {
        'row': 2,
        'col': 1,
        'relative_row': 2,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 1,
        'margin_right': 52,
        'margin_bottom': 12,
        'width': 7,
        'height': 1,
        'type': 'label',
        'name': '_label_1',
        'text': 'Item:'
    },
    {
        'row': 2,
        'col': 13,
        'relative_row': 2,
        'relative_col': 13,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 46,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 12,
        'width': 46,
        'height': 1,
        'type': 'textinput',
        'name': 'item_input'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 50,
        'margin_bottom': 10,
        'width': 9,
        'height': 1,
        'type': 'label',
        'name': '_label_3',
        'text': 'Amount:'
    },
    {
        'row': 4,
        'col': 13,
        'relative_row': 4,
        'relative_col': 13,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 22,
        'margin_bottom': 10,
        'width': 25,
        'height': 1,
        'type': 'numberinput',
        'name': 'amount_input'
    },
    {
        'row': 4,
        'col': 39,
        'relative_row': 4,
        'relative_col': 39,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 10,
        'width': 20,
        'height': 1,
        'type': 'label',
        'name': 'currency_label'
    },
    {
        'row': 6,
        'col': 1,
        'relative_row': 6,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 48,
        'margin_bottom': 8,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': '_label_6',
        'text': 'Category:'
    },
    {
        'row': 6,
        'col': 13,
        'relative_row': 6,
        'relative_col': 13,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 46,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 8,
        'width': 46,
        'height': 1,
        'type': 'combobox',
        'name': 'category_input'
    },
    {
        'row': 8,
        'col': 1,
        'relative_row': 8,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 58,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 6,
        'width': 58,
        'height': 1,
        'type': 'button',
        'name': 'add_expense_btn'
    },
    {
        'row': 10,
        'col': 1,
        'relative_row': 10,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 15,
        'cell_name': 'view',
        'cell_height': 15,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 58,
        'chars_height': 5,
        'margin_right': 1,
        'type': 'expenses_list',
        'name': 'expenses'
    }
]
