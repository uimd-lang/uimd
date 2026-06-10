"""Auto-generated UI code for gallery_view_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl

from gallery_scroll import GalleryScroll


class GalleryViewUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Gallery View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: "Gallery view hosting a scrollable list of full-size images."\ntags: [example, image, gallery, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - gallery_scroll\n```\n\n## Members\n\n```yaml\ngallery_scroll:\n  type: gallery_scroll\n  description: "Scrollable image list."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#030712"\n```\n\n## User Interface\n\n```ui\n+-view-**-------------------+\n| gallery_scroll..**....... |\n| ......................... |\n| *........................ |\n| *........................ |\n| ......................... |\n| ......................... |\n+---------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Gallery view hosting a scrollable list of full-size images.'}, 'elements': {'gallery_scroll': {'role': 'text', 'description': 'Scrollable image list.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Gallery View",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'control'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.gallery_scroll = self.create_element("gallery_scroll", "uielement", layout_class=GalleryScroll)


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
    "gallery_scroll": {
        "type": 'gallery_scroll',
        "description": 'Scrollable image list.',
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
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 6,
        'cell_name': 'view',
        'cell_height': 6,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 6,
        'margin_right': 1,
        'type': 'gallery_scroll',
        'name': 'gallery_scroll'
    }
]
