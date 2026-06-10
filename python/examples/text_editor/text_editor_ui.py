"""Auto-generated UI code for text_editor_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../dialogs'))


class TextEditorUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Text Editor\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes."\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - ../../dialogs/file_browser\n```\n\n## Members\n\n```yaml\nwindow_header:\n  type: label\n  text: "Text Editor"\n  description: "Application title."\n  expose: false\n\nfilename:\n  type: label\n  text: welcome.txt\n  description: "Current file path or file name."\n\neditor:\n  type: textarea\n  value: ""\n  description: "Main multiline text editing area."\n\nnew_btn:\n  type: button\n  title: New\n  description: "Create a new empty document, asking about unsaved changes when needed."\n\nopen_btn:\n  type: button\n  title: Open\n  description: "Open the file browser to load a text file, asking about unsaved changes when needed."\n\nsave_btn:\n  type: button\n  title: Save\n  description: "Save the current document to its current path."\n\nsave_as_btn:\n  type: button\n  title: "Save As"\n  description: "Open the file browser in save mode and save the document to a chosen path."\n\nquit_btn:\n  type: button\n  title: Quit\n  description: "Close the editor, asking about unsaved changes when needed."\n\nstatus:\n  type: label\n  text: Ready\n  description: "Shows load, save, cancel, and error messages."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@filename:\n  color: "#bfdbfe"\n@quit_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n@status:\n  color: "#93c5fd"\n```\n\n## User Interface\n\n```ui\n+-title-**------------------------------------------------------------------------------+\n|window_header..**......................................................................|\n+-filebar-------------------------------------------------------------------------------+\n|                                                                                       |\n| "file" filename..**.................................................................. |\n|                                                                                       |\n| new_btn....... open_btn....... save_btn....... save_as_btn....... quit_btn.......     |\n+-main----------------------------------------------------------------------------------+\n|                                                                                       |\n| editor..**............................................................................|\n| ..................................................................................... |\n* *.................................................................................... |\n* *.................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n+---------------------------------------------------------------------------------------+\n| status............................................................................... |\n+---------------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.'}, 'elements': {'window_header': {'role': 'text', 'description': 'Application title.', 'expose': False}, 'filename': {'role': 'text', 'description': 'Current file path or file name.', 'expose': True}, 'editor': {'role': 'input', 'description': 'Main multiline text editing area.', 'expose': True}, 'new_btn': {'role': 'action', 'description': 'Create a new empty document, asking about unsaved changes when needed.', 'expose': True}, 'open_btn': {'role': 'action', 'description': 'Open the file browser to load a text file, asking about unsaved changes when needed.', 'expose': True}, 'save_btn': {'role': 'action', 'description': 'Save the current document to its current path.', 'expose': True}, 'save_as_btn': {'role': 'action', 'description': 'Open the file browser in save mode and save the document to a chosen path.', 'expose': True}, 'quit_btn': {'role': 'action', 'description': 'Close the editor, asking about unsaved changes when needed.', 'expose': True}, 'status': {'role': 'text', 'description': 'Shows load, save, cancel, and error messages.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Text Editor",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

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
        "description": 'Application title.',
        "expose": False,
    },
    "filename": {
        "type": 'label',
        "text": 'welcome.txt',
        "description": 'Current file path or file name.',
    },
    "editor": {
        "type": 'textarea',
        "value": '',
        "description": 'Main multiline text editing area.',
    },
    "new_btn": {
        "type": 'button',
        "title": 'New',
        "description": 'Create a new empty document, asking about unsaved changes when needed.',
    },
    "open_btn": {
        "type": 'button',
        "title": 'Open',
        "description": 'Open the file browser to load a text file, asking about unsaved changes when needed.',
    },
    "save_btn": {
        "type": 'button',
        "title": 'Save',
        "description": 'Save the current document to its current path.',
    },
    "save_as_btn": {
        "type": 'button',
        "title": 'Save As',
        "description": 'Open the file browser in save mode and save the document to a chosen path.',
    },
    "quit_btn": {
        "type": 'button',
        "title": 'Quit',
        "description": 'Close the editor, asking about unsaved changes when needed.',
    },
    "status": {
        "type": 'label',
        "text": 'Ready',
        "description": 'Shows load, save, cancel, and error messages.',
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
