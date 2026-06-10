"""Auto-generated UI code for activity_feed_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'activity_feed_panel'))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'settings'))
from activity_feed_panel import ActivityFeedPanel


class ActivityFeedUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Activity Feed\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: "Activity feed example with dynamically appended scroll view items and a settings dialog."\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - activity_feed_panel/activity_feed_panel\n  - settings/settings\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: "Activity Feed"\n  description: "Application title."\n  expose: false\n\nsettings_button:\n  type: button\n  title: Settings\n  description: "Open the activity feed settings dialog."\n\nquit_button:\n  type: button\n  title: Quit\n  description: "Close the activity feed example."\n\nfeed:\n  type: activity_feed_panel\n  description: "Scrollable activity timeline."\n\ntype_label:\n  type: label\n  text: Type\n  description: "Label for activity type selector."\n  expose: false\n\nactivity_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: "Activity type selector."\n\nmessage_label:\n  type: label\n  text: Message\n  description: "Label for activity message input."\n  expose: false\n\nmessage:\n  type: textarea\n  value: ""\n  maxlength: 180\n  description: "Activity message input."\n\nadd_button:\n  type: button\n  title: Add\n  description: "Append one activity item to the feed."\n\nseed_button:\n  type: button\n  title: Seed\n  description: "Append sample activity items to the feed."\n\nclear_button:\n  type: button\n  title: Clear\n  description: "Remove all activity items from the feed."\n\nstatus:\n  type: label\n  text: "Ready"\n  description: "Shows activity feed status."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: "#1f2937"\n@header_title:\n  text-align: center\n  color: "#ffffff"\n@content:\n  background: "#030712"\n@feed:\n  background: "#00000000"\n@controls:\n  background: "#172033"\n  padding: 1, 1, 1, 1\n@actions:\n  background: "#172033"\n  padding: 1, 1, 1, 1\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@clear_button:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n@quit_button:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\ntextinput:\n  background: "#0b1220"\n  color: "#e5e7eb"\n  focus-background: "#25364f"\n  edit-background: "#0b1220"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ntextarea:\n  background: "#0b1220"\n  color: "#e5e7eb"\n  focus-background: "#25364f"\n  edit-background: "#0b1220"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ncombobox:\n  background: "#0b1220"\n  color: "#e5e7eb"\n  focus-background: "#25364f"\n  edit-background: "#0b1220"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\n@status:\n  background: "#0f172a"\n  color: "#93c5fd"\n```\n\n## User Interface\n\n```ui\n+-header-**-------------------------------------------------------+\n|header_title..**................. settings_button.. quit_button..|\n+-content-**------------------------------------------------------+\n| feed..**....................................................... |\n| ............................................................... |\n| ............................................................... |\n* *.............................................................. |\n* *.............................................................. |\n* *.............................................................. |\n| ............................................................... |\n+-controls-**-----------------------------------------------------+\n| type_label     activity_type.........                           |\n|                                                                 |\n| message_label  message..**....................................  |\n|               ...............................................   |\n|               ...............................................   |\n+-actions-**------------------------------------------------------+\n| add_button......... seed_button......... clear_button.......... |\n+-----------------------------------------------------------------+\n| status..**..................................................... |\n+-----------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Activity feed example with dynamically appended scroll view items and a settings dialog.'}, 'elements': {'header_title': {'role': 'text', 'description': 'Application title.', 'expose': False}, 'settings_button': {'role': 'action', 'description': 'Open the activity feed settings dialog.', 'expose': True}, 'quit_button': {'role': 'action', 'description': 'Close the activity feed example.', 'expose': True}, 'feed': {'role': 'text', 'description': 'Scrollable activity timeline.', 'expose': True}, 'type_label': {'role': 'text', 'description': 'Label for activity type selector.', 'expose': False}, 'activity_type': {'role': 'selection', 'description': 'Activity type selector.', 'expose': True}, 'message_label': {'role': 'text', 'description': 'Label for activity message input.', 'expose': False}, 'message': {'role': 'input', 'description': 'Activity message input.', 'expose': True}, 'add_button': {'role': 'action', 'description': 'Append one activity item to the feed.', 'expose': True}, 'seed_button': {'role': 'action', 'description': 'Append sample activity items to the feed.', 'expose': True}, 'clear_button': {'role': 'action', 'description': 'Remove all activity items from the feed.', 'expose': True}, 'status': {'role': 'text', 'description': 'Shows activity feed status.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Activity Feed",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.activity_type = self.create_element("activity_type", "combobox", options=['Info', 'Task', 'Warning', 'Deploy', 'Note'], selected_item='Info')
        self.add_button = self.create_element("add_button", "button", title='Add')
        self.clear_button = self.create_element("clear_button", "button", title='Clear')
        self.feed = self.create_element("feed", "uielement", layout_class=ActivityFeedPanel)
        self.header_title = self.create_element("header_title", "label", text='Activity Feed')
        self.message = self.create_element("message", "textarea", value='', maxlength=180)
        self.message_label = self.create_element("message_label", "label", text='Message')
        self.quit_button = self.create_element("quit_button", "button", title='Quit')
        self.seed_button = self.create_element("seed_button", "button", title='Seed')
        self.settings_button = self.create_element("settings_button", "button", title='Settings')
        self.status = self.create_element("status", "label", text='Ready')
        self.type_label = self.create_element("type_label", "label", text='Type')
    def on_settings_button_click(self):
        pass

    def on_quit_button_click(self):
        pass

    def on_activity_type_change(self, value):
        pass

    def on_message_change(self, value):
        pass

    def on_message_submit(self, value):
        pass

    def on_add_button_click(self):
        pass

    def on_seed_button_click(self):
        pass

    def on_clear_button_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.settings_button:
            self.on_settings_button_click()
        elif element is self.quit_button:
            self.on_quit_button_click()
        elif element is self.message:
            self.on_message_change(value)
        elif element is self.add_button:
            self.on_add_button_click()
        elif element is self.seed_button:
            self.on_seed_button_click()
        elif element is self.clear_button:
            self.on_clear_button_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.message:
            self.on_message_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.activity_type:
            self.on_activity_type_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header_title": {
        "type": 'label',
        "text": 'Activity Feed',
        "description": 'Application title.',
        "expose": False,
    },
    "settings_button": {
        "type": 'button',
        "title": 'Settings',
        "description": 'Open the activity feed settings dialog.',
    },
    "quit_button": {
        "type": 'button',
        "title": 'Quit',
        "description": 'Close the activity feed example.',
    },
    "feed": {
        "type": 'activity_feed_panel',
        "description": 'Scrollable activity timeline.',
    },
    "type_label": {
        "type": 'label',
        "text": 'Type',
        "description": 'Label for activity type selector.',
        "expose": False,
    },
    "activity_type": {
        "type": 'combobox',
        "options": ['Info', 'Task', 'Warning', 'Deploy', 'Note'],
        "selected_item": 'Info',
        "description": 'Activity type selector.',
    },
    "message_label": {
        "type": 'label',
        "text": 'Message',
        "description": 'Label for activity message input.',
        "expose": False,
    },
    "message": {
        "type": 'textarea',
        "value": '',
        "maxlength": 180,
        "description": 'Activity message input.',
    },
    "add_button": {
        "type": 'button',
        "title": 'Add',
        "description": 'Append one activity item to the feed.',
    },
    "seed_button": {
        "type": 'button',
        "title": 'Seed',
        "description": 'Append sample activity items to the feed.',
    },
    "clear_button": {
        "type": 'button',
        "title": 'Clear',
        "description": 'Remove all activity items from the feed.',
    },
    "status": {
        "type": 'label',
        "text": 'Ready',
        "description": 'Shows activity feed status.',
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
        "background": '#0b1220',
        "color": '#e5e7eb',
        "focus-background": '#25364f',
        "focus-color": '#ffffff',
        "edit-background": '#0b1220',
        "edit-color": '#ffffff',
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
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
        "background": '#0b1220',
        "color": '#e5e7eb',
        "focus-background": '#25364f',
        "focus-color": '#ffffff',
        "edit-background": '#0b1220',
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
    "@header": {
        "background": '#1f2937',
    },
    "@header_title": {
        "text-align": 'center',
        "color": '#ffffff',
    },
    "@content": {
        "background": '#030712',
    },
    "@feed": {
        "background": '#00000000',
    },
    "@controls": {
        "background": '#172033',
        "padding": '1, 1, 1, 1',
    },
    "@actions": {
        "background": '#172033',
        "padding": '1, 1, 1, 1',
    },
    "@clear_button": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "@quit_button": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "textarea": {
        "background": '#0b1220',
        "color": '#e5e7eb',
        "focus-background": '#25364f',
        "edit-background": '#0b1220',
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
    },
    "@status": {
        "background": '#0f172a',
        "color": '#93c5fd',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 33,
        'chars_height': 1,
        'margin_right': 32,
        'height': 1,
        'type': 'label',
        'name': 'header_title'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 16,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 1,
        'cell_name': 'actions',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 19,
        'chars_height': 1,
        'margin_right': 45,
        'width': 19,
        'height': 1,
        'type': 'button',
        'name': 'add_button'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 7,
        'cell_name': 'content',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 63,
        'chars_height': 7,
        'margin_right': 1,
        'type': 'activity_feed_panel',
        'name': 'feed'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 18,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 1,
        'cell_width': 65,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 63,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'label',
        'name': 'status'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 10,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 5,
        'cell_name': 'controls',
        'cell_height': 5,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 54,
        'margin_bottom': 4,
        'width': 10,
        'height': 1,
        'type': 'label',
        'name': 'type_label'
    },
    {
        'row': 0,
        'col': 16,
        'relative_col': 16,
        'cell_row': 10,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 5,
        'cell_name': 'controls',
        'cell_height': 5,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 22,
        'chars_height': 1,
        'margin_right': 27,
        'margin_bottom': 4,
        'width': 22,
        'height': 1,
        'type': 'combobox',
        'name': 'activity_type'
    },
    {
        'row': 0,
        'col': 21,
        'relative_col': 21,
        'cell_row': 16,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 1,
        'cell_name': 'actions',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 24,
        'width': 20,
        'height': 1,
        'type': 'button',
        'name': 'seed_button'
    },
    {
        'row': 0,
        'col': 34,
        'relative_col': 34,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 17,
        'chars_height': 1,
        'margin_right': 14,
        'width': 17,
        'height': 1,
        'type': 'button',
        'name': 'settings_button'
    },
    {
        'row': 0,
        'col': 42,
        'relative_col': 42,
        'cell_row': 16,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 1,
        'cell_name': 'actions',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 22,
        'chars_height': 1,
        'margin_right': 1,
        'width': 22,
        'height': 1,
        'type': 'button',
        'name': 'clear_button'
    },
    {
        'row': 0,
        'col': 52,
        'relative_col': 52,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 13,
        'chars_height': 1,
        'width': 13,
        'height': 1,
        'type': 'button',
        'name': 'quit_button'
    },
    {
        'row': 2,
        'col': 1,
        'relative_row': 2,
        'relative_col': 1,
        'cell_row': 10,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 5,
        'cell_name': 'controls',
        'cell_height': 5,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 13,
        'chars_height': 1,
        'margin_right': 51,
        'margin_bottom': 2,
        'width': 13,
        'height': 1,
        'type': 'label',
        'name': 'message_label'
    },
    {
        'row': 2,
        'col': 16,
        'relative_row': 2,
        'relative_col': 16,
        'cell_row': 10,
        'cell_col': 0,
        'cell_chars_width': 65,
        'cell_chars_height': 5,
        'cell_name': 'controls',
        'cell_height': 5,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 47,
        'chars_height': 3,
        'margin_right': 2,
        'height': 3,
        'type': 'textarea',
        'name': 'message'
    }
]
