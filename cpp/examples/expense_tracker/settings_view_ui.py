"""Auto-generated UI code for settings_view_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIControl



class SettingsViewUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Settings View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: "Expense tracker settings view."\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Settings\n\ncurrency:\n  type: combobox\n  options: [EUR, USD, CZK]\n  selected_item: EUR\n\nshow_paid:\n  type: checkbox\n  title: Show paid expenses\n  value: true\n\nmonthly_limit:\n  type: numberinput\n  value: 500\n  format: "0"\n\nreset_btn:\n  type: button\n  title: Reset demo data\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#0b1f33"\n@view_title:\n  color: "#fde68a"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| "Currency:"      currency................................. |\n|                                                            |\n| show_paid................................................. |\n|                                                            |\n| "Monthly limit:" monthly_limit............................ |\n|                                                            |\n| reset_btn................................................. |\n+------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Expense tracker settings view.'}, 'elements': {'view_title': {'role': 'text', 'description': 'Settings', 'expose': True}, 'currency': {'role': 'selection', 'description': 'currency', 'expose': True}, 'show_paid': {'role': 'toggle', 'description': 'Show paid expenses', 'expose': True}, 'monthly_limit': {'role': 'input', 'description': 'monthly_limit', 'expose': True}, 'reset_btn': {'role': 'action', 'description': 'Reset demo data', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Settings View",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.currency = self.create_element("currency", "combobox", options=['EUR', 'USD', 'CZK'], selected_item='EUR')
        self.monthly_limit = self.create_element("monthly_limit", "numberinput", value=500)
        self.reset_btn = self.create_element("reset_btn", "button", title='Reset demo data')
        self.show_paid = self.create_element("show_paid", "checkbox", value=True, title='Show paid expenses')
        self.view_title = self.create_element("view_title", "label", text='Settings')
        self._label_1 = self.create_element("_label_1", "label", text='Currency:')
        self._label_4 = self.create_element("_label_4", "label", text='Monthly limit:')
    def on_currency_change(self, value):
        pass

    def on_show_paid_change(self, value):
        pass

    def on_monthly_limit_change(self, value):
        pass

    def on_monthly_limit_submit(self, value):
        pass

    def on_reset_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.show_paid:
            self.on_show_paid_change(value)
        elif element is self.monthly_limit:
            self.on_monthly_limit_change(value)
        elif element is self.reset_btn:
            self.on_reset_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.monthly_limit:
            self.on_monthly_limit_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.currency:
            self.on_currency_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "view_title": {
        "type": 'label',
        "text": 'Settings',
    },
    "currency": {
        "type": 'combobox',
        "options": ['EUR', 'USD', 'CZK'],
        "selected_item": 'EUR',
    },
    "show_paid": {
        "type": 'checkbox',
        "title": 'Show paid expenses',
        "value": True,
    },
    "monthly_limit": {
        "type": 'numberinput',
        "value": 500,
        "format": '0',
    },
    "reset_btn": {
        "type": 'button',
        "title": 'Reset demo data',
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
        "color": '#fde68a',
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
        'cell_chars_height': 9,
        'cell_name': 'view',
        'cell_height': 9,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 58,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 8,
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
        'cell_chars_height': 9,
        'cell_name': 'view',
        'cell_height': 9,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 48,
        'margin_bottom': 6,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': '_label_1',
        'text': 'Currency:'
    },
    {
        'row': 2,
        'col': 18,
        'relative_row': 2,
        'relative_col': 18,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 9,
        'cell_name': 'view',
        'cell_height': 9,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 41,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 6,
        'width': 41,
        'height': 1,
        'type': 'combobox',
        'name': 'currency'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 9,
        'cell_name': 'view',
        'cell_height': 9,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 58,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 4,
        'width': 58,
        'height': 1,
        'type': 'checkbox',
        'name': 'show_paid'
    },
    {
        'row': 6,
        'col': 1,
        'relative_row': 6,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 9,
        'cell_name': 'view',
        'cell_height': 9,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 16,
        'chars_height': 1,
        'margin_right': 43,
        'margin_bottom': 2,
        'width': 16,
        'height': 1,
        'type': 'label',
        'name': '_label_4',
        'text': 'Monthly limit:'
    },
    {
        'row': 6,
        'col': 18,
        'relative_row': 6,
        'relative_col': 18,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 9,
        'cell_name': 'view',
        'cell_height': 9,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 41,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 2,
        'width': 41,
        'height': 1,
        'type': 'numberinput',
        'name': 'monthly_limit'
    },
    {
        'row': 8,
        'col': 1,
        'relative_row': 8,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 9,
        'cell_name': 'view',
        'cell_height': 9,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 58,
        'chars_height': 1,
        'margin_right': 1,
        'width': 58,
        'height': 1,
        'type': 'button',
        'name': 'reset_btn'
    }
]
