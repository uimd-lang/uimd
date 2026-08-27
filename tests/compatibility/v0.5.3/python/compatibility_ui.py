"""Auto-generated UI code for compatibility_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow

from compat_scroll import CompatScroll


class CompatibilityUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# UIMD 0.5.3 Compatibility Application\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: "Retained generated application for patch-level runtime compatibility."\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nuses:\n  - compat_scroll\n```\n\n## Members\n\n```yaml\naligned_input:\n  type: textinput\n  value: "abc"\n  maxlength: 16\n\nchoice:\n  type: combobox\n  options: [one, two, three, four, five, six, seven, eight]\n  selected_item: one\n\nscroller:\n  type: compat_scroll\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: "#303545"\n@aligned_input:\n  text-align: right\n@scroller:\n  background: "#303545"\n  focus-background: "#ffffff14"\n```\n\n## User Interface\n\n```ui\n+--------------------------------+\n|aligned_input...................|\n|choice..........................|\n|................................|\n|scroller........................|\n|................................|\n+--------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Retained generated application for patch-level runtime compatibility.'}, 'elements': {'aligned_input': {'role': 'input', 'description': 'aligned_input', 'expose': True}, 'choice': {'role': 'selection', 'description': 'choice', 'expose': True}, 'scroller': {'role': 'text', 'description': 'scroller', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="UIMD 0.5.3 Compatibility Application",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.aligned_input = self.create_element("aligned_input", "textinput", value='abc', maxlength=16)
        self.choice = self.create_element("choice", "combobox", options=['one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight'], selected_item='one')
        self.scroller = self.create_element("scroller", "uielement", layout_class=CompatScroll)
    def on_aligned_input_change(self, value):
        pass

    def on_aligned_input_submit(self, value):
        pass

    def on_choice_change(self, value):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.aligned_input:
            self.on_aligned_input_change(value)
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.aligned_input:
            self.on_aligned_input_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.choice:
            self.on_choice_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "aligned_input": {
        "type": 'textinput',
        "value": 'abc',
        "maxlength": 16,
    },
    "choice": {
        "type": 'combobox',
        "options": ['one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight'],
        "selected_item": 'one',
    },
    "scroller": {
        "type": 'compat_scroll',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#303545',
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
    "@aligned_input": {
        "text-align": 'right',
    },
    "@scroller": {
        "background": '#303545',
        "focus-background": '#ffffff14',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 32,
        'cell_chars_height': 5,
        'cell_width': 32,
        'cell_height': 5,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 32,
        'chars_height': 1,
        'margin_bottom': 4,
        'height': 1,
        'type': 'textinput',
        'name': 'aligned_input'
    },
    {
        'row': 1,
        'col': 0,
        'relative_row': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 32,
        'cell_chars_height': 5,
        'cell_width': 32,
        'cell_height': 5,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 32,
        'chars_height': 2,
        'margin_bottom': 2,
        'height': 2,
        'type': 'combobox',
        'name': 'choice'
    },
    {
        'row': 3,
        'col': 0,
        'relative_row': 3,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 32,
        'cell_chars_height': 5,
        'cell_width': 32,
        'cell_height': 5,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 32,
        'chars_height': 2,
        'height': 2,
        'type': 'compat_scroll',
        'name': 'scroller'
    }
]
