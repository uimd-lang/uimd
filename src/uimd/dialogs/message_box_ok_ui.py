"""Auto-generated UI code for message_box_ok_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow



class MessageBoxOkUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Message Box\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: stable\ndescription: "Message Box UI source."\ntags: [dialog]\n```\n\n## Members\n\n```yaml\ndialog_header:\n  type: label\n  text: Message\n\nmessage:\n  type: label\n  text: ""\n\nok_btn:\n  type: button\n  title: OK\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------+\n|dialog_header...............................|\n+--------------------------------------------+\n|                                            |\n|  message.................................  |\n|  ........................................  |\n|                                            |\n|  ok_btn..................................  |\n|                                            |\n+--------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Message Box UI source.'}, 'elements': {'dialog_header': {'role': 'text', 'description': 'Message', 'expose': True}, 'message': {'role': 'text', 'description': 'message', 'expose': True}, 'ok_btn': {'role': 'action', 'description': 'OK', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Message Box",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'dialog'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.dialog_header = self.create_element("dialog_header", "label", text='Message')
        self.message = self.create_element("message", "label", text='')
        self.ok_btn = self.create_element("ok_btn", "button", title='OK')
    def on_ok_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.ok_btn:
            self.on_ok_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "dialog_header": {
        "type": 'label',
        "text": 'Message',
    },
    "message": {
        "type": 'label',
        "text": '',
    },
    "ok_btn": {
        "type": 'button',
        "title": 'OK',
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
        "focus-background": '#2b5f9f',
        "focus-color": '#ffffff',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 44,
        'cell_chars_height': 1,
        'cell_width': 44,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 44,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': 'dialog_header'
    },
    {
        'row': 1,
        'col': 2,
        'relative_row': 1,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 44,
        'cell_chars_height': 6,
        'cell_width': 44,
        'cell_height': 6,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 40,
        'chars_height': 2,
        'margin_right': 2,
        'margin_bottom': 3,
        'height': 2,
        'type': 'label',
        'name': 'message'
    },
    {
        'row': 4,
        'col': 2,
        'relative_row': 4,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 44,
        'cell_chars_height': 6,
        'cell_width': 44,
        'cell_height': 6,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 40,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'button',
        'name': 'ok_btn'
    }
]
