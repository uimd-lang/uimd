"""Auto-generated UI code for activity_item_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl



class ActivityItemUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Activity Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: "Reusable activity feed item with timestamp, event type, and message text."\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntimestamp:\n  type: label\n  text: "10:42"\n  description: "Optional activity timestamp."\n\nevent_type:\n  type: label\n  text: Info\n  description: "Activity category label."\n\nmessage:\n  type: label\n  text: "Activity message"\n  description: "Selectable activity message body."\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@marker:\n  background-texture: "▐"\n  background-texture-color: "#38bdf8"\n@marker_info:\n  background-texture-color: "#38bdf8"\n@marker_task:\n  background-texture-color: "#a78bfa"\n@marker_warning:\n  background-texture-color: "#facc15"\n@marker_deploy:\n  background-texture-color: "#22c55e"\n@marker_note:\n  background-texture-color: "#fb7185"\n@body:\n  background: "#1f2e46"\n@timestamp:\n  color: "#94a3b8"\n  user-select: text\n@event_type:\n  color: "#bfdbfe"\n  user-select: text\n@message:\n  color: "#ffffff"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-marker-1-+-body-**----------------------------------+\n|          | timestamp.. event_type..**               |\n|          | message..**                              |\n|          | ........................................ |\n|          | ........................................ |\n#          # #                                        |\n#          # #                                        |\n+----------+------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Reusable activity feed item with timestamp, event type, and message text.'}, 'elements': {'timestamp': {'role': 'text', 'description': 'Optional activity timestamp.', 'expose': True}, 'event_type': {'role': 'text', 'description': 'Activity category label.', 'expose': True}, 'message': {'role': 'text', 'description': 'Selectable activity message body.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Activity Item",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'component'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.event_type = self.create_element("event_type", "label", text='Info')
        self.message = self.create_element("message", "label", text='Activity message')
        self.timestamp = self.create_element("timestamp", "label", text='10:42')
        self._cell_0 = self.create_element("_cell_0", "label", text='')


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
    "timestamp": {
        "type": 'label',
        "text": '10:42',
        "description": 'Optional activity timestamp.',
    },
    "event_type": {
        "type": 'label',
        "text": 'Info',
        "description": 'Activity category label.',
    },
    "message": {
        "type": 'label',
        "text": 'Activity message',
        "description": 'Selectable activity message body.',
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
    "@marker": {
        "background-texture": '▐',
        "background-texture-color": '#38bdf8',
    },
    "@marker_info": {
        "background-texture-color": '#38bdf8',
    },
    "@marker_task": {
        "background-texture-color": '#a78bfa',
    },
    "@marker_warning": {
        "background-texture-color": '#facc15',
    },
    "@marker_deploy": {
        "background-texture-color": '#22c55e',
    },
    "@marker_note": {
        "background-texture-color": '#fb7185',
    },
    "@body": {
        "background": '#1f2e46',
    },
    "@timestamp": {
        "color": '#94a3b8',
        "user-select": 'text',
    },
    "@event_type": {
        "color": '#bfdbfe',
        "user-select": 'text',
    },
    "@message": {
        "color": '#ffffff',
        "user-select": 'text',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 10,
        'cell_chars_height': 6,
        'cell_name': 'marker',
        'cell_width': 1,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'fit-content',
        'chars_width': 10,
        'chars_height': 6,
        'width': 1,
        'type': 'label',
        'name': '_cell_0'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 11,
        'cell_chars_width': 42,
        'cell_chars_height': 6,
        'cell_name': 'body',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'fit-content',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 30,
        'margin_bottom': 5,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': 'timestamp'
    },
    {
        'row': 0,
        'col': 13,
        'relative_col': 13,
        'cell_row': 0,
        'cell_col': 11,
        'cell_chars_width': 42,
        'cell_chars_height': 6,
        'cell_name': 'body',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'fit-content',
        'width_mode': 'expanded',
        'chars_width': 14,
        'chars_height': 1,
        'margin_right': 15,
        'margin_bottom': 5,
        'height': 1,
        'type': 'label',
        'name': 'event_type'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 11,
        'cell_chars_width': 42,
        'cell_chars_height': 6,
        'cell_name': 'body',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'fit-content',
        'width_mode': 'expanded',
        'height_mode': 'fit-content',
        'chars_width': 11,
        'chars_height': 5,
        'margin_right': 30,
        'type': 'label',
        'name': 'message'
    }
]
