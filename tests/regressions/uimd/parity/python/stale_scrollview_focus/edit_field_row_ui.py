"""Auto-generated UI code for edit_field_row_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl



class EditFieldRowUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Edit Field Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: "One editable field row with an action button inside the edit ScrollView."\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfield_input:\n  type: textinput\n  value: ""\n  maxlength: 120\n  description: "Editable regression field."\n\nfield_btn:\n  type: button\n  title: Action\n  description: "Action button beside the editable field."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@row:\n  background: "#111827"\n  padding: 1, 1, 1, 1\ntextinput:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@field_btn:\n  background: "#14532d"\n  focus-background: "#16a34a"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------------------------------------------+\n| field_input..**......................................................................... |\n|                                                                                          |\n| field_btn.......... .................................................................... |\n+------------------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'One editable field row with an action button inside the edit ScrollView.'}, 'elements': {'field_input': {'role': 'input', 'description': 'Editable regression field.', 'expose': True}, 'field_btn': {'role': 'action', 'description': 'Action button beside the editable field.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Edit Field Row",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'component'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.field_btn = self.create_element("field_btn", "button", title='Action')
        self.field_input = self.create_element("field_input", "textinput", value='', maxlength=120)
    def on_field_input_change(self, value):
        pass

    def on_field_input_submit(self, value):
        pass

    def on_field_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.field_input:
            self.on_field_input_change(value)
        elif element is self.field_btn:
            self.on_field_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.field_input:
            self.on_field_input_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "field_input": {
        "type": 'textinput',
        "value": '',
        "maxlength": 120,
        "description": 'Editable regression field.',
    },
    "field_btn": {
        "type": 'button',
        "title": 'Action',
        "description": 'Action button beside the editable field.',
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
        "background": '#0f172a',
        "color": '#e5e7eb',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
        "focus-background": '#1e293b',
        "focus-color": '#ffffff',
        "edit-background": '#111827',
        "edit-color": '#ffffff',
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
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
    "@row": {
        "background": '#111827',
        "padding": '1, 1, 1, 1',
    },
    "@field_btn": {
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
        'cell_chars_height': 3,
        'cell_name': 'row',
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 88,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 2,
        'height': 1,
        'type': 'textinput',
        'name': 'field_input'
    },
    {
        'row': 2,
        'col': 1,
        'relative_row': 2,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 3,
        'cell_name': 'row',
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 19,
        'chars_height': 1,
        'margin_right': 70,
        'width': 19,
        'height': 1,
        'type': 'button',
        'name': 'field_btn'
    }
]
