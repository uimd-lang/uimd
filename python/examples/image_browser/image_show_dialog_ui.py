"""Auto-generated UI code for image_show_dialog_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow
from uimd.runtime.image import require_sixel_for_image_rendering



class ImageShowDialogUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Image Show Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\ndescription: "Dialog for showing a selected image."\ntags: [example, image, dialog]\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: "Image"\n  description: "Dialog title."\n  expose: false\n\nphoto:\n  type: image\n  source: ""\n  alt: ""\n  fit: contain\n  render_mode: auto\n  description: "Selected image preview."\n\nclose_btn:\n  type: button\n  title: Close\n  description: "Close the dialog."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 1\n  background: "#081426"\n  margin: 4, 6, 4, 6\ncell:\n  padding: 1, 1, 1, 1\n@title:\n  color: "#f8fafc"\n  text-align: center\nimage:\n  background: transparent\n  color: "#d1d5db"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n```\n\n## User Interface\n\n```ui\n+-dialog-**--------------------------------------------------------------------------------+\n| title..**............................................................................... |\n| photo..**............................................................................... |\n* ........................................................................................ |\n* *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+-**-------------------------------+---------------------+-**------------------------------+\n|                                  |  close_btn........  |                                 |\n+----------------------------------+---------------------+---------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Dialog for showing a selected image.'}, 'elements': {'title': {'role': 'text', 'description': 'Dialog title.', 'expose': False}, 'photo': {'role': 'text', 'description': 'Selected image preview.', 'expose': True}, 'close_btn': {'role': 'action', 'description': 'Close the dialog.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Image Show Dialog",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        require_sixel_for_image_rendering()
        self.focusable = False
        self._metadata_kind = 'dialog'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.close_btn = self.create_element("close_btn", "button", title='Close')
        self.photo = self.create_element("photo", "image", source='', alt='', fit='contain', render_mode='auto')
        self.title_element = self.create_element("title", "label", text='Image')
        self._cell_0 = self.create_element("_cell_0", "label", text='')
        self._cell_1 = self.create_element("_cell_1", "label", text='')
    def on_close_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.close_btn:
            self.on_close_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "title": {
        "type": 'label',
        "text": 'Image',
        "description": 'Dialog title.',
        "expose": False,
    },
    "photo": {
        "type": 'image',
        "source": '',
        "alt": '',
        "fit": 'contain',
        "render_mode": 'auto',
        "description": 'Selected image preview.',
    },
    "close_btn": {
        "type": 'button',
        "title": 'Close',
        "description": 'Close the dialog.',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#081426',
        "border-color": 'transparent',
        "border-width": 1,
        "margin": '4, 6, 4, 6',
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
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "cell": {
        "padding": '1, 1, 1, 1',
    },
    "@title": {
        "color": '#f8fafc',
        "text-align": 'center',
    },
    "image": {
        "background": 'transparent',
        "color": '#d1d5db',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 22,
        'cell_col': 0,
        'cell_chars_width': 34,
        'cell_chars_height': 1,
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 34,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': '_cell_0'
    },
    {
        'row': 0,
        'col': 0,
        'cell_row': 22,
        'cell_col': 57,
        'cell_chars_width': 33,
        'cell_chars_height': 1,
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 33,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': '_cell_1'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 21,
        'cell_name': 'dialog',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 88,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 20,
        'height': 1,
        'type': 'label',
        'name': 'title'
    },
    {
        'row': 0,
        'col': 2,
        'relative_col': 2,
        'cell_row': 22,
        'cell_col': 35,
        'cell_chars_width': 21,
        'cell_chars_height': 1,
        'cell_width': 21,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 17,
        'chars_height': 1,
        'margin_right': 2,
        'height': 1,
        'type': 'button',
        'name': 'close_btn'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 21,
        'cell_name': 'dialog',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 88,
        'chars_height': 20,
        'margin_right': 1,
        'type': 'image',
        'name': 'photo'
    }
]
