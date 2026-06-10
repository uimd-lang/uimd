"""Auto-generated UI code for file_browser_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow



class FileBrowserUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# File Browser\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: stable\ndescription: "File Browser UI source."\ntags: [dialog]\n```\n\n## Members\n\n```yaml\ndialog_header:\n  type: label\n  text: "Open File"\n\npath_label:\n  type: label\n  text: .\n\nentries:\n  type: listbox\n  options: []\n  selected_items: []\n  multiple: "false"\n  commit-mode: leave\n\nfilename:\n  type: textinput\n  value: ""\n  maxlength: 120\n\nopen_btn:\n  type: button\n  title: Open\n\nclose_btn:\n  type: button\n  title: Close\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@entries:\n  background: "#0d1524"\n  color: "#cbd5e1"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\n```\n\n## User Interface\n\n```ui\n+------------------------------------------------+\n|dialog_header...................................|\n+------------------------------------------------+\n|                                                |\n|   path_label................................   |\n|                                                |\n|   entries...................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|                                                |\n|   "File:"..  filename.......................   |\n|                                                |\n|   open_btn............  close_btn...........   |\n|                                                |\n+------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'File Browser UI source.'}, 'elements': {'dialog_header': {'role': 'text', 'description': 'Open File', 'expose': True}, 'path_label': {'role': 'text', 'description': '.', 'expose': True}, 'entries': {'role': 'selection', 'description': 'entries', 'expose': True}, 'filename': {'role': 'input', 'description': 'filename', 'expose': True}, 'open_btn': {'role': 'action', 'description': 'Open', 'expose': True}, 'close_btn': {'role': 'action', 'description': 'Close', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="File Browser",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.close_btn = self.create_element("close_btn", "button", title='Close')
        self.dialog_header = self.create_element("dialog_header", "label", text='Open File')
        self.entries = self.create_element("entries", "listbox", multiple=False)
        self.entries.commit_mode = 'leave'
        self.filename = self.create_element("filename", "textinput", value='', maxlength=120)
        self.open_btn = self.create_element("open_btn", "button", title='Open')
        self.path_label = self.create_element("path_label", "label", text='.')
        self._label_3 = self.create_element("_label_3", "label", text='File:')
    def on_entries_selection_change(self, value):
        pass

    def on_filename_change(self, value):
        pass

    def on_filename_submit(self, value):
        pass

    def on_open_btn_click(self):
        pass

    def on_close_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.filename:
            self.on_filename_change(value)
        elif element is self.open_btn:
            self.on_open_btn_click()
        elif element is self.close_btn:
            self.on_close_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.filename:
            self.on_filename_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.entries:
            self.on_entries_selection_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "dialog_header": {
        "type": 'label',
        "text": 'Open File',
    },
    "path_label": {
        "type": 'label',
        "text": '.',
    },
    "entries": {
        "type": 'listbox',
        "options": [],
        "selected_items": [],
        "multiple": 'false',
        "commit-mode": 'leave',
    },
    "filename": {
        "type": 'textinput',
        "value": '',
        "maxlength": 120,
    },
    "open_btn": {
        "type": 'button',
        "title": 'Open',
    },
    "close_btn": {
        "type": 'button',
        "title": 'Close',
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
    "@entries": {
        "background": '#0d1524',
        "color": '#cbd5e1',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 1,
        'cell_width': 48,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 48,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': 'dialog_header'
    },
    {
        'row': 1,
        'col': 3,
        'relative_row': 1,
        'relative_col': 3,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 14,
        'cell_width': 48,
        'cell_height': 14,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 42,
        'chars_height': 1,
        'margin_right': 3,
        'margin_bottom': 12,
        'height': 1,
        'type': 'label',
        'name': 'path_label'
    },
    {
        'row': 3,
        'col': 3,
        'relative_row': 3,
        'relative_col': 3,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 14,
        'cell_width': 48,
        'cell_height': 14,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 42,
        'chars_height': 6,
        'margin_right': 3,
        'margin_bottom': 5,
        'height': 6,
        'type': 'listbox',
        'name': 'entries'
    },
    {
        'row': 10,
        'col': 3,
        'relative_row': 10,
        'relative_col': 3,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 14,
        'cell_width': 48,
        'cell_height': 14,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 36,
        'margin_bottom': 3,
        'width': 9,
        'height': 1,
        'type': 'label',
        'name': '_label_3',
        'text': 'File:'
    },
    {
        'row': 10,
        'col': 14,
        'relative_row': 10,
        'relative_col': 14,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 14,
        'cell_width': 48,
        'cell_height': 14,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 3,
        'margin_bottom': 3,
        'height': 1,
        'type': 'textinput',
        'name': 'filename'
    },
    {
        'row': 12,
        'col': 3,
        'relative_row': 12,
        'relative_col': 3,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 14,
        'cell_width': 48,
        'cell_height': 14,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 25,
        'margin_bottom': 1,
        'width': 20,
        'height': 1,
        'type': 'button',
        'name': 'open_btn'
    },
    {
        'row': 12,
        'col': 25,
        'relative_row': 12,
        'relative_col': 25,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 48,
        'cell_chars_height': 14,
        'cell_width': 48,
        'cell_height': 14,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 3,
        'margin_bottom': 1,
        'height': 1,
        'type': 'button',
        'name': 'close_btn'
    }
]
