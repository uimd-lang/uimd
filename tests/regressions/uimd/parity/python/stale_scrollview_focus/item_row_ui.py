"""Auto-generated UI code for item_row_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl



class ItemRowUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Item Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: "Interactive row with an Open button inside a ScrollView."\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nname:\n  type: label\n  text: "Item"\n  description: "Item name."\n\nsummary:\n  type: label\n  text: "Summary"\n  description: "Item summary."\n\nspacer:\n  type: label\n  text: ""\n  description: "Flexible row spacer."\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: "Open this item in the edit page."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: "#172033"\n  padding: 1, 1, 1, 1\n@name:\n  color: "#f8fafc"\n  user-select: text\n@summary:\n  color: "#93c5fd"\n  user-select: text\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@open_btn:\n  background: "#14532d"\n  focus-background: "#16a34a"\n```\n\n## User Interface\n\n```ui\n+-card-**----------------------------------------------------------------------------------+\n| name..**................................................... summary..................... |\n| spacer..**................................................................ open_btn..... |\n+------------------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Interactive row with an Open button inside a ScrollView.'}, 'elements': {'name': {'role': 'text', 'description': 'Item name.', 'expose': True}, 'summary': {'role': 'text', 'description': 'Item summary.', 'expose': True}, 'spacer': {'role': 'text', 'description': 'Flexible row spacer.', 'expose': False}, 'open_btn': {'role': 'action', 'description': 'Open this item in the edit page.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Item Row",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'component'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.name = self.create_element("name", "label", text='Item')
        self.open_btn = self.create_element("open_btn", "button", title='Open')
        self.spacer = self.create_element("spacer", "label", text='')
        self.summary = self.create_element("summary", "label", text='Summary')
    def on_open_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.open_btn:
            self.on_open_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "name": {
        "type": 'label',
        "text": 'Item',
        "description": 'Item name.',
    },
    "summary": {
        "type": 'label',
        "text": 'Summary',
        "description": 'Item summary.',
    },
    "spacer": {
        "type": 'label',
        "text": '',
        "description": 'Flexible row spacer.',
        "expose": False,
    },
    "open_btn": {
        "type": 'button',
        "title": 'Open',
        "description": 'Open this item in the edit page.',
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
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
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
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
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
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
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
        "disabled-color": '#64748b',
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
        "disabled-color": '#64748b',
        "focus-background": '#ffffff44',
        "focus-color": '#ffffff',
        "checked-background": 'transparent',
        "checked-color": '#44cc88',
        "unchecked-background": 'transparent',
    },
    "button": {
        "background": '#334155',
        "color": '#e2e8f0',
        "disabled-background": '#1f2937',
        "disabled-color": '#64748b',
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "@card": {
        "background": '#172033',
        "padding": '1, 1, 1, 1',
    },
    "@name": {
        "color": '#f8fafc',
        "user-select": 'text',
    },
    "@summary": {
        "color": '#93c5fd',
        "user-select": 'text',
    },
    "@open_btn": {
        "background": '#14532d',
        "focus-background": '#16a34a',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 59,
        'chars_height': 1,
        'margin_right': 30,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'name'
    },
    {
        'row': 0,
        'col': 61,
        'relative_col': 61,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 28,
        'chars_height': 2,
        'margin_right': 1,
        'width': 28,
        'height': 2,
        'type': 'label',
        'name': 'summary'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 74,
        'chars_height': 1,
        'margin_right': 15,
        'height': 1,
        'type': 'label',
        'name': 'spacer'
    },
    {
        'row': 1,
        'col': 76,
        'relative_row': 1,
        'relative_col': 76,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 13,
        'chars_height': 1,
        'margin_right': 1,
        'width': 13,
        'height': 1,
        'type': 'button',
        'name': 'open_btn'
    }
]
