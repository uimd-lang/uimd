"""Auto-generated UI code for image_view_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl
from uimd.runtime.image import require_sixel_for_image_rendering



class ImageViewUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Image View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: "Full-size image display panel hosted inside a ViewHost."\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ncaption:\n  type: label\n  text: "Select an image"\n  description: "Name of the currently displayed image."\n\nphoto:\n  type: image\n  source: ""\n  alt: "photo"\n  fit: contain\n  render_mode: auto\n  description: "Full-size image display."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#030712"\n@caption:\n  color: "#bfdbfe"\n  text-align: center\nimage:\n  background: transparent\n  color: "#d1d5db"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------------+\n| caption..**.................. |\n| photo..**.................... |\n| ............................. |\n| *............................ |\n| *............................ |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n+-------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Full-size image display panel hosted inside a ViewHost.'}, 'elements': {'caption': {'role': 'text', 'description': 'Name of the currently displayed image.', 'expose': True}, 'photo': {'role': 'text', 'description': 'Full-size image display.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Image View",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        require_sixel_for_image_rendering()
        self.focusable = False
        self._metadata_kind = 'control'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.caption = self.create_element("caption", "label", text='Select an image')
        self.photo = self.create_element("photo", "image", source='', alt='photo', fit='contain', render_mode='auto')


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
    "caption": {
        "type": 'label',
        "text": 'Select an image',
        "description": 'Name of the currently displayed image.',
    },
    "photo": {
        "type": 'image',
        "source": '',
        "alt": 'photo',
        "fit": 'contain',
        "render_mode": 'auto',
        "description": 'Full-size image display.',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#030712',
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
    "@caption": {
        "color": '#bfdbfe',
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
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 31,
        'cell_chars_height': 12,
        'cell_name': 'view',
        'cell_height': 12,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 29,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 11,
        'height': 1,
        'type': 'label',
        'name': 'caption'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 31,
        'cell_chars_height': 12,
        'cell_name': 'view',
        'cell_height': 12,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 29,
        'chars_height': 11,
        'margin_right': 1,
        'type': 'image',
        'name': 'photo'
    }
]
