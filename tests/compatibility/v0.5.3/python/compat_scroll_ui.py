"""Auto-generated UI code for compat_scroll_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIScrollView



class CompatScrollUI(UIScrollView):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# UIMD 0.5.3 Compatibility Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: "Retained generated ScrollView input for the 0.5.3 patch-compatibility gate."\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\n```\n\n## Members\n\n```yaml\nalpha_row:\n  type: label\n  text: "legacy alpha row"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: "#303545"\n@panel:\n  background: "#303545"\n  padding: 0\n@alpha_row:\n  background: "#252a36cc"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------+\n|alpha_row.....................|\n|..............................|\n+------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.'}, 'elements': {'alpha_row': {'role': 'text', 'description': 'legacy alpha row', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="UIMD 0.5.3 Compatibility Scroll",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'component'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.alpha_row = self.create_element("alpha_row", "label", text='legacy alpha row')


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
    "alpha_row": {
        "type": 'label',
        "text": 'legacy alpha row',
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
    "@panel": {
        "background": '#303545',
        "padding": 0,
    },
    "@alpha_row": {
        "background": '#252a36cc',
    },
}

COMPILED_LAYOUT = []
