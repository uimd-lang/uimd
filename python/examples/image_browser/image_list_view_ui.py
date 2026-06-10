"""Auto-generated UI code for image_list_view_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow

from image_list_scroll import ImageListScroll


class ImageListViewUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Image List View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: "ViewHost page with add button and scrollable image item list."\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_list_scroll\n```\n\n## Members\n\n```yaml\nadd_btn:\n  type: button\n  title: Add\n  description: "Add an image item."\n\nitems:\n  type: image_list_scroll\n  description: "Scrollable list of image items."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#030712"\n@toolbar:\n  background: "#030712"\n@items_cell:\n  background: "#030712"\nbutton:\n  background: "#14532d"\n  color: "#e2e8f0"\n  focus-background: "#16a34a"\n```\n\n## User Interface\n\n```ui\n+-toolbar-**------------------------------------------------------------+\n| add_btn............                                                   |\n+-items_cell-**---------------------------------------------------------+\n|items..**..............................................................|\n*.......................................................................|\n**......................................................................|\n|*......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n+-----------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'ViewHost page with add button and scrollable image item list.'}, 'elements': {'add_btn': {'role': 'action', 'description': 'Add an image item.', 'expose': True}, 'items': {'role': 'text', 'description': 'Scrollable list of image items.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Image List View",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'control'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.add_btn = self.create_element("add_btn", "button", title='Add')
        self.items = self.create_element("items", "uielement", layout_class=ImageListScroll)
    def on_add_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.add_btn:
            self.on_add_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "add_btn": {
        "type": 'button',
        "title": 'Add',
        "description": 'Add an image item.',
    },
    "items": {
        "type": 'image_list_scroll',
        "description": 'Scrollable list of image items.',
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
        "background": '#14532d',
        "color": '#e2e8f0',
        "disabled-background": '#1f2937',
        "disabled-color": '#64748b',
        "focus-background": '#16a34a',
        "focus-color": '#ffffff',
    },
    "@toolbar": {
        "background": '#030712',
    },
    "@items_cell": {
        "background": '#030712',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 11,
        'cell_name': 'items_cell',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 71,
        'chars_height': 11,
        'type': 'image_list_scroll',
        'name': 'items'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 1,
        'cell_name': 'toolbar',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 19,
        'chars_height': 1,
        'margin_right': 51,
        'width': 19,
        'height': 1,
        'type': 'button',
        'name': 'add_btn'
    }
]
