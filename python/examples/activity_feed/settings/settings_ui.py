"""Auto-generated UI code for settings_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow



class SettingsUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Activity Feed Settings\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: "Settings dialog for activity feed auto-scroll, timestamps, and default activity type."\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: "Activity Feed Settings"\n  description: "Dialog title."\n  expose: false\n\nauto_scroll:\n  type: checkbox\n  title: "Auto-scroll to newest"\n  value: 1\n  description: "Toggle whether new activity items scroll into view."\n\nshow_timestamps:\n  type: checkbox\n  title: "Show timestamps"\n  value: 1\n  description: "Toggle whether newly added activity items include timestamps."\n\ndefault_type_label:\n  type: label\n  text: "Default type"\n  description: "Label for the default activity type selector."\n  expose: false\n\ndefault_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: "Default activity type used by the main window."\n\nsave_button:\n  type: button\n  title: Save\n  description: "Apply settings and close the dialog."\n\ncancel_button:\n  type: button\n  title: Cancel\n  description: "Close the dialog without applying settings."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: "#1f2937"\n@header_title:\n  text-align: center\n  color: "#ffffff"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\ncombobox:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\ncheckbox:\n  color: "#d1d5db"\n  checked-color: "#22c55e"\n  unchecked-color: "#f97316"\n@buttons:\n  padding: 0, 2, 1, 2\n```\n\n## User Interface\n\n```ui\n+-header-----------------------------------+\n|header_title..**..........................|\n+-content----------------------------------+\n|                                          |\n|  auto_scroll...........................  |\n|                                          |\n|  show_timestamps.......................  |\n|                                          |\n|  default_type_label..  default_type....  |\n|                                          |\n+-buttons----------------------------------+\n|  save_button......  cancel_button......  |\n+------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Settings dialog for activity feed auto-scroll, timestamps, and default activity type.'}, 'elements': {'header_title': {'role': 'text', 'description': 'Dialog title.', 'expose': False}, 'auto_scroll': {'role': 'toggle', 'description': 'Toggle whether new activity items scroll into view.', 'expose': True}, 'show_timestamps': {'role': 'toggle', 'description': 'Toggle whether newly added activity items include timestamps.', 'expose': True}, 'default_type_label': {'role': 'text', 'description': 'Label for the default activity type selector.', 'expose': False}, 'default_type': {'role': 'selection', 'description': 'Default activity type used by the main window.', 'expose': True}, 'save_button': {'role': 'action', 'description': 'Apply settings and close the dialog.', 'expose': True}, 'cancel_button': {'role': 'action', 'description': 'Close the dialog without applying settings.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Activity Feed Settings",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.auto_scroll = self.create_element("auto_scroll", "checkbox", value=1, title='Auto-scroll to newest')
        self.cancel_button = self.create_element("cancel_button", "button", title='Cancel')
        self.default_type = self.create_element("default_type", "combobox", options=['Info', 'Task', 'Warning', 'Deploy', 'Note'], selected_item='Info')
        self.default_type_label = self.create_element("default_type_label", "label", text='Default type')
        self.header_title = self.create_element("header_title", "label", text='Activity Feed Settings')
        self.save_button = self.create_element("save_button", "button", title='Save')
        self.show_timestamps = self.create_element("show_timestamps", "checkbox", value=1, title='Show timestamps')
    def on_auto_scroll_change(self, value):
        pass

    def on_show_timestamps_change(self, value):
        pass

    def on_default_type_change(self, value):
        pass

    def on_save_button_click(self):
        pass

    def on_cancel_button_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.auto_scroll:
            self.on_auto_scroll_change(value)
        elif element is self.show_timestamps:
            self.on_show_timestamps_change(value)
        elif element is self.save_button:
            self.on_save_button_click()
        elif element is self.cancel_button:
            self.on_cancel_button_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.default_type:
            self.on_default_type_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header_title": {
        "type": 'label',
        "text": 'Activity Feed Settings',
        "description": 'Dialog title.',
        "expose": False,
    },
    "auto_scroll": {
        "type": 'checkbox',
        "title": 'Auto-scroll to newest',
        "value": 1,
        "description": 'Toggle whether new activity items scroll into view.',
    },
    "show_timestamps": {
        "type": 'checkbox',
        "title": 'Show timestamps',
        "value": 1,
        "description": 'Toggle whether newly added activity items include timestamps.',
    },
    "default_type_label": {
        "type": 'label',
        "text": 'Default type',
        "description": 'Label for the default activity type selector.',
        "expose": False,
    },
    "default_type": {
        "type": 'combobox',
        "options": ['Info', 'Task', 'Warning', 'Deploy', 'Note'],
        "selected_item": 'Info',
        "description": 'Default activity type used by the main window.',
    },
    "save_button": {
        "type": 'button',
        "title": 'Save',
        "description": 'Apply settings and close the dialog.',
    },
    "cancel_button": {
        "type": 'button',
        "title": 'Cancel',
        "description": 'Close the dialog without applying settings.',
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
        "background": '#0f172a',
        "color": '#e5e7eb',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
        "focus-background": '#1e293b',
        "focus-color": '#ffffff',
        "edit-background": '#111827',
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
        "color": '#d1d5db',
        "disabled-color": '#64748b',
        "focus-background": '#ffffff44',
        "focus-color": '#ffffff',
        "checked-background": 'transparent',
        "checked-color": '#22c55e',
        "unchecked-background": 'transparent',
        "unchecked-color": '#f97316',
    },
    "button": {
        "background": '#334155',
        "color": '#e2e8f0',
        "disabled-background": '#1f2937',
        "disabled-color": '#64748b',
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
    "@buttons": {
        "padding": '0, 2, 1, 2',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 42,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_width': 42,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 42,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': 'header_title'
    },
    {
        'row': 0,
        'col': 2,
        'relative_col': 2,
        'cell_row': 10,
        'cell_col': 0,
        'cell_chars_width': 42,
        'cell_chars_height': 1,
        'cell_name': 'buttons',
        'cell_width': 42,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 17,
        'chars_height': 1,
        'margin_right': 23,
        'width': 17,
        'height': 1,
        'type': 'button',
        'name': 'save_button'
    },
    {
        'row': 0,
        'col': 21,
        'relative_col': 21,
        'cell_row': 10,
        'cell_col': 0,
        'cell_chars_width': 42,
        'cell_chars_height': 1,
        'cell_name': 'buttons',
        'cell_width': 42,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 19,
        'chars_height': 1,
        'margin_right': 2,
        'height': 1,
        'type': 'button',
        'name': 'cancel_button'
    },
    {
        'row': 1,
        'col': 2,
        'relative_row': 1,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 42,
        'cell_chars_height': 7,
        'cell_name': 'content',
        'cell_width': 42,
        'cell_height': 7,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 38,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 5,
        'height': 1,
        'type': 'checkbox',
        'name': 'auto_scroll'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 42,
        'cell_chars_height': 7,
        'cell_name': 'content',
        'cell_width': 42,
        'cell_height': 7,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 38,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 3,
        'height': 1,
        'type': 'checkbox',
        'name': 'show_timestamps'
    },
    {
        'row': 5,
        'col': 2,
        'relative_row': 5,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 42,
        'cell_chars_height': 7,
        'cell_name': 'content',
        'cell_width': 42,
        'cell_height': 7,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 20,
        'margin_bottom': 1,
        'width': 20,
        'height': 1,
        'type': 'label',
        'name': 'default_type_label'
    },
    {
        'row': 5,
        'col': 24,
        'relative_row': 5,
        'relative_col': 24,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 42,
        'cell_chars_height': 7,
        'cell_name': 'content',
        'cell_width': 42,
        'cell_height': 7,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 16,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'combobox',
        'name': 'default_type'
    }
]
