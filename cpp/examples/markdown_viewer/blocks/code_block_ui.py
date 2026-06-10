"""Auto-generated UI code for code_block_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIControl



class CodeBlockUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Code Block\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: "Code Block UI source."\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nlang:\n  type: label\n  text: code\n\ncode:\n  type: spanlabel\n  text: ""\n```\n\n## Style\n\n```yaml\nthis:\n  background: "#00000000"\n  border-width: 0\n@head:\n  background: "#1d314c"\n@body:\n  background: "#0c1a2d"\n@lang:\n  color: "#93c5fd"\n@code:\n  color: "#d1d5db"\n  word-wrap: false\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-head-**----------------------------------------+\n| lang..**...................................... |\n+-body-**----------------------------------------+\n| code..**...................................... |\n| .............................................. |\n# #............................................ |\n# #............................................ |\n+------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Code Block UI source.'}, 'elements': {'lang': {'role': 'text', 'description': 'code', 'expose': True}, 'code': {'role': 'text', 'description': 'code', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Code Block",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.code = self.create_element("code", "spanlabel", text='')
        self.lang = self.create_element("lang", "label", text='code')


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
    "lang": {
        "type": 'label',
        "text": 'code',
    },
    "code": {
        "type": 'spanlabel',
        "text": '',
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
        "focus-background": '#2b5f9f',
        "focus-color": '#ffffff',
    },
    "@head": {
        "background": '#1d314c',
    },
    "@body": {
        "background": '#0c1a2d',
    },
    "@lang": {
        "color": '#93c5fd',
    },
    "@code": {
        "color": '#d1d5db',
        "word-wrap": False,
        "user-select": 'text',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 4,
        'cell_name': 'body',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'fit-content',
        'width_mode': 'expanded',
        'height_mode': 'fit-content',
        'chars_width': 46,
        'chars_height': 4,
        'margin_right': 1,
        'type': 'spanlabel',
        'name': 'code'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 1,
        'cell_name': 'head',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 46,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'label',
        'name': 'lang'
    }
]
