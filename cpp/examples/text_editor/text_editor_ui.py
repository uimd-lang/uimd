"""Auto-generated UI code for text_editor_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../dialogs'))


class TextEditorUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Text Editor\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Text Editor UI source."\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - ../../dialogs/file_browser\n```\n\n## Members\n\n```yaml\nwindow_header:\n  type: label\n  text: "Text Editor"\n  expose: false\n\nfilename:\n  type: label\n  text: welcome.txt\n\neditor:\n  type: textarea\n  value: ""\n\nnew_btn:\n  type: button\n  title: New\n\nopen_btn:\n  type: button\n  title: Open\n\nsave_btn:\n  type: button\n  title: Save\n\nsave_as_btn:\n  type: button\n  title: "Save As"\n\nquit_btn:\n  type: button\n  title: Quit\n\nstatus:\n  type: label\n  text: Ready\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@filename:\n  color: "#bfdbfe"\n@quit_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n@status:\n  color: "#93c5fd"\n```\n\n## User Interface\n\n```ui\n+-title-**------------------------------------------------------------------------------+\n|window_header..**......................................................................|\n+-filebar-------------------------------------------------------------------------------+\n|                                                                                       |\n| "file" filename..**.................................................................. |\n|                                                                                       |\n| new_btn....... open_btn....... save_btn....... save_as_btn....... quit_btn.......     |\n+-main----------------------------------------------------------------------------------+\n|                                                                                       |\n| editor..**............................................................................|\n| ..................................................................................... |\n* *.................................................................................... |\n* *.................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n+---------------------------------------------------------------------------------------+\n| status............................................................................... |\n+---------------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Text Editor UI source.'}, 'elements': {'window_header': {'role': 'text', 'description': 'Text Editor', 'expose': False}, 'filename': {'role': 'text', 'description': 'welcome.txt', 'expose': True}, 'editor': {'role': 'input', 'description': 'editor', 'expose': True}, 'new_btn': {'role': 'action', 'description': 'New', 'expose': True}, 'open_btn': {'role': 'action', 'description': 'Open', 'expose': True}, 'save_btn': {'role': 'action', 'description': 'Save', 'expose': True}, 'save_as_btn': {'role': 'action', 'description': 'Save As', 'expose': True}, 'quit_btn': {'role': 'action', 'description': 'Quit', 'expose': True}, 'status': {'role': 'text', 'description': 'Ready', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Text Editor",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.editor = self.create_element("editor", "textarea", value='')
        self.filename = self.create_element("filename", "label", text='welcome.txt')
        self.new_btn = self.create_element("new_btn", "button", title='New')
        self.open_btn = self.create_element("open_btn", "button", title='Open')
        self.quit_btn = self.create_element("quit_btn", "button", title='Quit')
        self.save_as_btn = self.create_element("save_as_btn", "button", title='Save As')
        self.save_btn = self.create_element("save_btn", "button", title='Save')
        self.status = self.create_element("status", "label", text='Ready')
        self.window_header = self.create_element("window_header", "label", text='Text Editor')
        self._label_2 = self.create_element("_label_2", "label", text='file')
    def on_editor_change(self, value):
        pass

    def on_editor_submit(self, value):
        pass

    def on_new_btn_click(self):
        pass

    def on_open_btn_click(self):
        pass

    def on_save_btn_click(self):
        pass

    def on_save_as_btn_click(self):
        pass

    def on_quit_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.editor:
            self.on_editor_change(value)
        elif element is self.new_btn:
            self.on_new_btn_click()
        elif element is self.open_btn:
            self.on_open_btn_click()
        elif element is self.save_btn:
            self.on_save_btn_click()
        elif element is self.save_as_btn:
            self.on_save_as_btn_click()
        elif element is self.quit_btn:
            self.on_quit_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.editor:
            self.on_editor_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "window_header": {
        "type": 'label',
        "text": 'Text Editor',
        "expose": False,
    },
    "filename": {
        "type": 'label',
        "text": 'welcome.txt',
    },
    "editor": {
        "type": 'textarea',
        "value": '',
    },
    "new_btn": {
        "type": 'button',
        "title": 'New',
    },
    "open_btn": {
        "type": 'button',
        "title": 'Open',
    },
    "save_btn": {
        "type": 'button',
        "title": 'Save',
    },
    "save_as_btn": {
        "type": 'button',
        "title": 'Save As',
    },
    "quit_btn": {
        "type": 'button',
        "title": 'Quit',
    },
    "status": {
        "type": 'label',
        "text": 'Ready',
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
    "@filename": {
        "color": '#bfdbfe',
    },
    "@quit_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "@status": {
        "color": '#93c5fd',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 1,
        'cell_name': 'title',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 87,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': 'window_header'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 18,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 1,
        'cell_width': 87,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 85,
        'chars_height': 1,
        'margin_right': 1,
        'width': 85,
        'height': 1,
        'type': 'label',
        'name': 'status'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 4,
        'cell_name': 'filebar',
        'cell_width': 87,
        'cell_height': 4,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 6,
        'chars_height': 1,
        'margin_right': 80,
        'margin_bottom': 2,
        'width': 6,
        'height': 1,
        'type': 'label',
        'name': '_label_2',
        'text': 'file'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 7,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 10,
        'cell_name': 'main',
        'cell_width': 87,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 86,
        'chars_height': 9,
        'type': 'textarea',
        'name': 'editor'
    },
    {
        'row': 1,
        'col': 8,
        'relative_row': 1,
        'relative_col': 8,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 4,
        'cell_name': 'filebar',
        'cell_width': 87,
        'cell_height': 4,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 78,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 2,
        'height': 1,
        'type': 'label',
        'name': 'filename'
    },
    {
        'row': 3,
        'col': 1,
        'relative_row': 3,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 4,
        'cell_name': 'filebar',
        'cell_width': 87,
        'cell_height': 4,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 14,
        'chars_height': 1,
        'margin_right': 72,
        'width': 14,
        'height': 1,
        'type': 'button',
        'name': 'new_btn'
    },
    {
        'row': 3,
        'col': 16,
        'relative_row': 3,
        'relative_col': 16,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 4,
        'cell_name': 'filebar',
        'cell_width': 87,
        'cell_height': 4,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 15,
        'chars_height': 1,
        'margin_right': 56,
        'width': 15,
        'height': 1,
        'type': 'button',
        'name': 'open_btn'
    },
    {
        'row': 3,
        'col': 32,
        'relative_row': 3,
        'relative_col': 32,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 4,
        'cell_name': 'filebar',
        'cell_width': 87,
        'cell_height': 4,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 15,
        'chars_height': 1,
        'margin_right': 40,
        'width': 15,
        'height': 1,
        'type': 'button',
        'name': 'save_btn'
    },
    {
        'row': 3,
        'col': 48,
        'relative_row': 3,
        'relative_col': 48,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 4,
        'cell_name': 'filebar',
        'cell_width': 87,
        'cell_height': 4,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 18,
        'chars_height': 1,
        'margin_right': 21,
        'width': 18,
        'height': 1,
        'type': 'button',
        'name': 'save_as_btn'
    },
    {
        'row': 3,
        'col': 67,
        'relative_row': 3,
        'relative_col': 67,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 4,
        'cell_name': 'filebar',
        'cell_width': 87,
        'cell_height': 4,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 15,
        'chars_height': 1,
        'margin_right': 5,
        'width': 15,
        'height': 1,
        'type': 'button',
        'name': 'quit_btn'
    }
]
