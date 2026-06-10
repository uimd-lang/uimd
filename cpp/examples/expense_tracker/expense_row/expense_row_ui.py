"""Auto-generated UI code for expense_row_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIControl



class ExpenseRowUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Expense Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: "Reusable expense row with paid state, category selector, and delete action."\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\npaid:\n  type: checkbox\n  title: Paid\n  value: false\n\nitem:\n  type: label\n  text: Item\n  user-select: text\n\namount:\n  type: label\n  text: "0.00"\n\ncategory:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ndelete_btn:\n  type: button\n  title: Delete\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@row:\n  background: "#102a44"\n  padding: 1\n@item:\n  color: "#f8fafc"\n@amount:\n  color: "#bfdbfe"\ncheckbox:\n  color: "#cbd5e1"\n  checked-color: "#22c55e"\n  unchecked-color: "#94a3b8"\ncombobox:\n  background: "#081827"\n  color: "#e5e7eb"\n  focus-background: "#1d4f8c"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@delete_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------+\n| paid..... item..**..................... amount...... |\n| category..**........................... delete_btn.. |\n+------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Reusable expense row with paid state, category selector, and delete action.'}, 'elements': {'paid': {'role': 'toggle', 'description': 'Paid', 'expose': True}, 'item': {'role': 'text', 'description': 'Item', 'expose': True}, 'amount': {'role': 'text', 'description': '0.00', 'expose': True}, 'category': {'role': 'selection', 'description': 'category', 'expose': True}, 'delete_btn': {'role': 'action', 'description': 'Delete', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Expense Row",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.amount = self.create_element("amount", "label", text='0.00')
        self.category = self.create_element("category", "combobox", options=['Food', 'Transport', 'Home', 'Health', 'Other'], selected_item='Food')
        self.delete_btn = self.create_element("delete_btn", "button", title='Delete')
        self.item = self.create_element("item", "label", text='Item')
        self.paid = self.create_element("paid", "checkbox", value=False, title='Paid')
    def on_paid_change(self, value):
        pass

    def on_category_change(self, value):
        pass

    def on_delete_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.paid:
            self.on_paid_change(value)
        elif element is self.delete_btn:
            self.on_delete_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.category:
            self.on_category_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "paid": {
        "type": 'checkbox',
        "title": 'Paid',
        "value": False,
    },
    "item": {
        "type": 'label',
        "text": 'Item',
        "user-select": 'text',
    },
    "amount": {
        "type": 'label',
        "text": '0.00',
    },
    "category": {
        "type": 'combobox',
        "options": ['Food', 'Transport', 'Home', 'Health', 'Other'],
        "selected_item": 'Food',
    },
    "delete_btn": {
        "type": 'button',
        "title": 'Delete',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#162033',
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
        "background": '#081827',
        "color": '#e5e7eb',
        "focus-background": '#1d4f8c',
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
        "color": '#cbd5e1',
        "focus-color": '#66aaff',
        "checked-background": 'transparent',
        "checked-color": '#22c55e',
        "unchecked-background": 'transparent',
        "unchecked-color": '#94a3b8',
    },
    "button": {
        "background": '#334155',
        "color": '#e2e8f0',
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "@row": {
        "background": '#102a44',
        "padding": 1,
    },
    "@item": {
        "color": '#f8fafc',
    },
    "@amount": {
        "color": '#bfdbfe',
    },
    "@delete_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 2,
        'cell_name': 'row',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 44,
        'margin_bottom': 1,
        'width': 9,
        'height': 1,
        'type': 'checkbox',
        'name': 'paid'
    },
    {
        'row': 0,
        'col': 11,
        'relative_col': 11,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 2,
        'cell_name': 'row',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 29,
        'chars_height': 2,
        'margin_right': 14,
        'type': 'label',
        'name': 'item'
    },
    {
        'row': 0,
        'col': 41,
        'relative_col': 41,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 2,
        'cell_name': 'row',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 1,
        'width': 12,
        'height': 1,
        'type': 'label',
        'name': 'amount'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 2,
        'cell_name': 'row',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 39,
        'chars_height': 1,
        'margin_right': 14,
        'height': 1,
        'type': 'combobox',
        'name': 'category'
    },
    {
        'row': 1,
        'col': 41,
        'relative_row': 1,
        'relative_col': 41,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 2,
        'cell_name': 'row',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 1,
        'width': 12,
        'height': 1,
        'type': 'button',
        'name': 'delete_btn'
    }
]
