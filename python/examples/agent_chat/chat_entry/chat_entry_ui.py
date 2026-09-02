"""Auto-generated UI code for chat_entry_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl



class ChatEntryUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Agent Chat Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: "Fixed-height simulated agent chat entry for prose and structured transcript blocks."\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nrole:\n  type: label\n  text: Agent\n  description: "Simulated transcript speaker or activity type."\n\nphase:\n  type: label\n  text: response\n  description: "Deterministic scenario phase."\n\nbody:\n  type: spanlabel\n  text: ""\n  description: "Selectable simulated transcript content."\n\ndetail:\n  type: label\n  text: "simulated · no external action"\n  description: "Simulation-only activity detail."\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: "#00000000"\n@header:\n  background: "#162033"\n  padding: 0, 1, 0, 1\n@role:\n  color: "#a7f3d0"\n@phase:\n  color: "#c4b5fd"\n  text-align: right\n@body_panel:\n  background: "#0b1220"\n  padding: 1, 1, 1, 1\n@body:\n  color: "#d1d5db"\n  word-wrap: false\n  user-select: text\n@footer:\n  background: "#111827"\n  padding: 0, 1, 0, 1\n@detail:\n  color: "#64748b"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-header-**---------------------------------------------------------------+\n| role..**........................................ phase................. |\n+-body_panel-**-----------------------------------------------------------+\n| body..**............................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n+-footer-**---------------------------------------------------------------+\n| detail..**............................................................. |\n+-------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Fixed-height simulated agent chat entry for prose and structured transcript blocks.'}, 'elements': {'role': {'role': 'text', 'description': 'Simulated transcript speaker or activity type.', 'expose': True}, 'phase': {'role': 'text', 'description': 'Deterministic scenario phase.', 'expose': True}, 'body': {'role': 'text', 'description': 'Selectable simulated transcript content.', 'expose': True}, 'detail': {'role': 'text', 'description': 'Simulation-only activity detail.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Agent Chat Entry",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'component'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.body = self.create_element("body", "spanlabel", text='')
        self.detail = self.create_element("detail", "label", text='simulated · no external action')
        self.phase = self.create_element("phase", "label", text='response')
        self.role = self.create_element("role", "label", text='Agent')


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
    "role": {
        "type": 'label',
        "text": 'Agent',
        "description": 'Simulated transcript speaker or activity type.',
    },
    "phase": {
        "type": 'label',
        "text": 'response',
        "description": 'Deterministic scenario phase.',
    },
    "body": {
        "type": 'spanlabel',
        "text": '',
        "description": 'Selectable simulated transcript content.',
    },
    "detail": {
        "type": 'label',
        "text": 'simulated · no external action',
        "description": 'Simulation-only activity detail.',
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
    "@header": {
        "background": '#162033',
        "padding": '0, 1, 0, 1',
    },
    "@role": {
        "color": '#a7f3d0',
    },
    "@phase": {
        "color": '#c4b5fd',
        "text-align": 'right',
    },
    "@body_panel": {
        "background": '#0b1220',
        "padding": '1, 1, 1, 1',
    },
    "@body": {
        "color": '#d1d5db',
        "word-wrap": False,
        "user-select": 'text',
    },
    "@footer": {
        "background": '#111827',
        "padding": '0, 1, 0, 1',
    },
    "@detail": {
        "color": '#64748b',
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
        'cell_chars_width': 73,
        'cell_chars_height': 6,
        'cell_name': 'body_panel',
        'cell_height': 6,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 71,
        'chars_height': 6,
        'margin_right': 1,
        'height': 6,
        'type': 'spanlabel',
        'name': 'body'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 9,
        'cell_col': 0,
        'cell_chars_width': 73,
        'cell_chars_height': 1,
        'cell_name': 'footer',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 71,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'label',
        'name': 'detail'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 73,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 48,
        'chars_height': 1,
        'margin_right': 24,
        'height': 1,
        'type': 'label',
        'name': 'role'
    },
    {
        'row': 0,
        'col': 50,
        'relative_col': 50,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 73,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 22,
        'chars_height': 1,
        'margin_right': 1,
        'width': 22,
        'height': 1,
        'type': 'label',
        'name': 'phase'
    }
]
