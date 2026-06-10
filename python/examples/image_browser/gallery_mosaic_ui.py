"""Auto-generated UI code for gallery_mosaic_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl
from uimd.runtime.image import require_sixel_for_image_rendering



class GalleryMosaicUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Gallery Mosaic\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: "3+2 image mosaic that opens the scrollable gallery when clicked."\ntags: [example, image, gallery]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\ngal_1:\n  type: image\n  source: "shared/assets/image_samples/camera.png"\n  alt: "Camera"\n  fit: cover\n  render_mode: auto\n  description: "Gallery mosaic slot 1. Click to open gallery."\n\ngal_2:\n  type: image\n  source: "shared/assets/image_samples/astronaut.png"\n  alt: "Astronaut"\n  fit: cover\n  render_mode: auto\n  description: "Gallery mosaic slot 2. Click to open gallery."\n\ngal_3:\n  type: image\n  source: "shared/assets/image_samples/coffee.png"\n  alt: "Coffee"\n  fit: cover\n  render_mode: auto\n  description: "Gallery mosaic slot 3. Click to open gallery."\n\ngal_4:\n  type: image\n  source: "shared/assets/image_samples/coins.png"\n  alt: "Coins"\n  fit: cover\n  render_mode: auto\n  description: "Gallery mosaic slot 4. Click to open gallery."\n\ngal_5:\n  type: image\n  source: "shared/assets/image_samples/chelsea.png"\n  alt: "Chelsea"\n  fit: cover\n  render_mode: auto\n  description: "Gallery mosaic slot 5. Click to open gallery."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#172033"\ncell:\n  padding: 1, 0, 1, 0\n  background: "#172033"\nimage:\n  background: transparent\n  color: "#d1d5db"\n  focus-background: "#25364f"\n```\n\n## User Interface\n\n```ui\n+-view-**------------------+\n| gal_1.. gal_2.. gal_3... |\n| ....... ....... ........ |\n| ....... ....... ........ |\n|                          |\n| gal_4.. gal_5..          |\n| ....... .......          |\n| ....... .......          |\n+--------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': '3+2 image mosaic that opens the scrollable gallery when clicked.'}, 'elements': {'gal_1': {'role': 'text', 'description': 'Gallery mosaic slot 1. Click to open gallery.', 'expose': True}, 'gal_2': {'role': 'text', 'description': 'Gallery mosaic slot 2. Click to open gallery.', 'expose': True}, 'gal_3': {'role': 'text', 'description': 'Gallery mosaic slot 3. Click to open gallery.', 'expose': True}, 'gal_4': {'role': 'text', 'description': 'Gallery mosaic slot 4. Click to open gallery.', 'expose': True}, 'gal_5': {'role': 'text', 'description': 'Gallery mosaic slot 5. Click to open gallery.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Gallery Mosaic",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        require_sixel_for_image_rendering()
        self.focusable = True
        self._metadata_kind = 'control'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.gal_1 = self.create_element("gal_1", "image", source='shared/assets/image_samples/camera.png', alt='Camera', fit='cover', render_mode='auto')
        self.gal_2 = self.create_element("gal_2", "image", source='shared/assets/image_samples/astronaut.png', alt='Astronaut', fit='cover', render_mode='auto')
        self.gal_3 = self.create_element("gal_3", "image", source='shared/assets/image_samples/coffee.png', alt='Coffee', fit='cover', render_mode='auto')
        self.gal_4 = self.create_element("gal_4", "image", source='shared/assets/image_samples/coins.png', alt='Coins', fit='cover', render_mode='auto')
        self.gal_5 = self.create_element("gal_5", "image", source='shared/assets/image_samples/chelsea.png', alt='Chelsea', fit='cover', render_mode='auto')


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
    "gal_1": {
        "type": 'image',
        "source": 'shared/assets/image_samples/camera.png',
        "alt": 'Camera',
        "fit": 'cover',
        "render_mode": 'auto',
        "description": 'Gallery mosaic slot 1. Click to open gallery.',
    },
    "gal_2": {
        "type": 'image',
        "source": 'shared/assets/image_samples/astronaut.png',
        "alt": 'Astronaut',
        "fit": 'cover',
        "render_mode": 'auto',
        "description": 'Gallery mosaic slot 2. Click to open gallery.',
    },
    "gal_3": {
        "type": 'image',
        "source": 'shared/assets/image_samples/coffee.png',
        "alt": 'Coffee',
        "fit": 'cover',
        "render_mode": 'auto',
        "description": 'Gallery mosaic slot 3. Click to open gallery.',
    },
    "gal_4": {
        "type": 'image',
        "source": 'shared/assets/image_samples/coins.png',
        "alt": 'Coins',
        "fit": 'cover',
        "render_mode": 'auto',
        "description": 'Gallery mosaic slot 4. Click to open gallery.',
    },
    "gal_5": {
        "type": 'image',
        "source": 'shared/assets/image_samples/chelsea.png',
        "alt": 'Chelsea',
        "fit": 'cover',
        "render_mode": 'auto',
        "description": 'Gallery mosaic slot 5. Click to open gallery.',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#172033',
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
    "cell": {
        "padding": '1, 0, 1, 0',
        "background": '#172033',
    },
    "image": {
        "background": 'transparent',
        "color": '#d1d5db',
        "focus-background": '#25364f',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 7,
        'cell_name': 'view',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 3,
        'margin_right': 18,
        'margin_bottom': 4,
        'width': 7,
        'height': 3,
        'type': 'image',
        'name': 'gal_1'
    },
    {
        'row': 0,
        'col': 9,
        'relative_col': 9,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 7,
        'cell_name': 'view',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 3,
        'margin_right': 10,
        'margin_bottom': 4,
        'width': 7,
        'height': 3,
        'type': 'image',
        'name': 'gal_2'
    },
    {
        'row': 0,
        'col': 17,
        'relative_col': 17,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 7,
        'cell_name': 'view',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 8,
        'chars_height': 3,
        'margin_right': 1,
        'margin_bottom': 4,
        'width': 8,
        'height': 3,
        'type': 'image',
        'name': 'gal_3'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 7,
        'cell_name': 'view',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 3,
        'margin_right': 18,
        'width': 7,
        'height': 3,
        'type': 'image',
        'name': 'gal_4'
    },
    {
        'row': 4,
        'col': 9,
        'relative_row': 4,
        'relative_col': 9,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 7,
        'cell_name': 'view',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 3,
        'margin_right': 10,
        'width': 7,
        'height': 3,
        'type': 'image',
        'name': 'gal_5'
    }
]
