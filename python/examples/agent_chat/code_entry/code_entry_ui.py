"""Auto-generated UI code for code_entry_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl



class CodeEntryUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Agent Chat Code Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: "Heron-style code, activity, command output, or diff block."\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: "Code"\n  description: "Code language or activity title."\n\ncode:\n  type: label\n  text: ""\n  description: "Selectable formatted code, command output, or diff content."\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: "#00000000"\n@code_panel:\n  background: "#0f1b2d"\n  padding: 0, 1, 0, 1\n@heading:\n  color: "#ddd6fe"\n@code:\n  color: "#d1d5db"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-code_panel-**-----------------------------------------------------------+\n| heading..**............................................................ |\n| code..**............................................................... |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Heron-style code, activity, command output, or diff block.'}, 'elements': {'heading': {'role': 'text', 'description': 'Code language or activity title.', 'expose': True}, 'code': {'role': 'text', 'description': 'Selectable formatted code, command output, or diff content.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Agent Chat Code Entry",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'component'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.code = self.create_element("code", "label", text='')
        self.heading = self.create_element("heading", "spanlabel", text='Code')


    def _dispatch_element_changed(self, element, value):
        pass
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "heading": {
        "type": 'spanlabel',
        "text": 'Code',
        "description": 'Code language or activity title.',
    },
    "code": {
        "type": 'label',
        "text": '',
        "description": 'Selectable formatted code, command output, or diff content.',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#00000000',
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
        "active-background": '#DDDDDD99',
        "active-color": '#FFFFFF',
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
        "focus-background": '#2b5f9f',
        "focus-color": '#ffffff',
    },
    "@code_panel": {
        "background": '#0f1b2d',
        "padding": '0, 1, 0, 1',
    },
    "@heading": {
        "color": '#ddd6fe',
    },
    "@code": {
        "color": '#d1d5db',
        "word-wrap": True,
        "user-select": 'text',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 73,
        'cell_chars_height': 5,
        'cell_name': 'code_panel',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'fit-content',
        'width_mode': 'expanded',
        'chars_width': 71,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 4,
        'height': 1,
        'type': 'spanlabel',
        'name': 'heading'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 73,
        'cell_chars_height': 5,
        'cell_name': 'code_panel',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'fit-content',
        'width_mode': 'expanded',
        'height_mode': 'fit-content',
        'chars_width': 71,
        'chars_height': 4,
        'margin_right': 1,
        'type': 'label',
        'name': 'code'
    }
]
