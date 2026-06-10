"""Auto-generated UI code for image_list_item_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl
from uimd.runtime.image import require_sixel_for_image_rendering



class ImageListItemUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Image List Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\ndescription: "Reusable row for one image item."\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nsel:\n  type: checkbox\n  title: ""\n  value: false\n  description: "Select this image item."\n\nthumb:\n  type: image\n  source: ""\n  alt: ""\n  fit: contain\n  render_mode: auto\n  description: "Small image thumbnail."\n\nname_label:\n  type: label\n  text: ""\n  description: "Image file name."\n\npath_label:\n  type: label\n  text: ""\n  description: "Full image path."\n\nspacer:\n  type: label\n  text: ""\n  description: "Flexible spacer before item actions."\n  expose: false\n\nshow_btn:\n  type: button\n  title: Show\n  description: "Show this image in a dialog."\n\nbrowse_btn:\n  type: button\n  title: Browse\n  description: "Choose a replacement image."\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: "Delete this image item after confirmation."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: "#172033"\n  padding: 0, 0, 0, 0\n@name_label:\n  color: "#f8fafc"\n  user-select: text\n@path_label:\n  color: "#64748b"\n  user-select: text\ncheckbox:\n  color: "#cbd5e1"\n  focus-color: "#facc15"\n  checked-color: "#22c55e"\n  unchecked-color: "#94a3b8"\nimage:\n  background: transparent\n  color: "#d1d5db"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@show_btn:\n  background: "#14532d"\n  focus-background: "#16a34a"\n@delete_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------------------------------+\n|                                                                       |\n|  sel thumb...  name_label..**..  show_btn... browse_btn. delete_btn.  |\n|  ... ........  .....................................................  |\n|  ... ........  path_label..**.......................................  |\n|  ... ........  .....................................................  |\n|  ... ........  spacer..**...........................................  |\n|                                                                       |\n+-----------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Reusable row for one image item.'}, 'elements': {'sel': {'role': 'toggle', 'description': 'Select this image item.', 'expose': True}, 'thumb': {'role': 'text', 'description': 'Small image thumbnail.', 'expose': True}, 'name_label': {'role': 'text', 'description': 'Image file name.', 'expose': True}, 'path_label': {'role': 'text', 'description': 'Full image path.', 'expose': True}, 'spacer': {'role': 'text', 'description': 'Flexible spacer before item actions.', 'expose': False}, 'show_btn': {'role': 'action', 'description': 'Show this image in a dialog.', 'expose': True}, 'browse_btn': {'role': 'action', 'description': 'Choose a replacement image.', 'expose': True}, 'delete_btn': {'role': 'action', 'description': 'Delete this image item after confirmation.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Image List Item",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        require_sixel_for_image_rendering()
        self.focusable = False
        self._metadata_kind = 'component'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.browse_btn = self.create_element("browse_btn", "button", title='Browse')
        self.delete_btn = self.create_element("delete_btn", "button", title='Delete')
        self.name_label = self.create_element("name_label", "label", text='')
        self.path_label = self.create_element("path_label", "label", text='')
        self.sel = self.create_element("sel", "checkbox", value=False)
        self.show_btn = self.create_element("show_btn", "button", title='Show')
        self.spacer = self.create_element("spacer", "label", text='')
        self.thumb = self.create_element("thumb", "image", source='', alt='', fit='contain', render_mode='auto')
    def on_sel_change(self, value):
        pass

    def on_show_btn_click(self):
        pass

    def on_browse_btn_click(self):
        pass

    def on_delete_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.sel:
            self.on_sel_change(value)
        elif element is self.show_btn:
            self.on_show_btn_click()
        elif element is self.browse_btn:
            self.on_browse_btn_click()
        elif element is self.delete_btn:
            self.on_delete_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "sel": {
        "type": 'checkbox',
        "title": '',
        "value": False,
        "description": 'Select this image item.',
    },
    "thumb": {
        "type": 'image',
        "source": '',
        "alt": '',
        "fit": 'contain',
        "render_mode": 'auto',
        "description": 'Small image thumbnail.',
    },
    "name_label": {
        "type": 'label',
        "text": '',
        "description": 'Image file name.',
    },
    "path_label": {
        "type": 'label',
        "text": '',
        "description": 'Full image path.',
    },
    "spacer": {
        "type": 'label',
        "text": '',
        "description": 'Flexible spacer before item actions.',
        "expose": False,
    },
    "show_btn": {
        "type": 'button',
        "title": 'Show',
        "description": 'Show this image in a dialog.',
    },
    "browse_btn": {
        "type": 'button',
        "title": 'Browse',
        "description": 'Choose a replacement image.',
    },
    "delete_btn": {
        "type": 'button',
        "title": 'Delete',
        "description": 'Delete this image item after confirmation.',
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
        "color": '#cbd5e1',
        "disabled-color": '#64748b',
        "focus-background": '#ffffff44',
        "focus-color": '#facc15',
        "checked-background": 'transparent',
        "checked-color": '#22c55e',
        "unchecked-background": 'transparent',
        "unchecked-color": '#94a3b8',
    },
    "button": {
        "background": '#334155',
        "color": '#e2e8f0',
        "disabled-background": '#1f2937',
        "disabled-color": '#64748b',
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "@card": {
        "background": '#172033',
        "padding": '0, 0, 0, 0',
    },
    "@name_label": {
        "color": '#f8fafc',
        "user-select": 'text',
    },
    "@path_label": {
        "color": '#64748b',
        "user-select": 'text',
    },
    "image": {
        "background": 'transparent',
        "color": '#d1d5db',
    },
    "@show_btn": {
        "background": '#14532d',
        "focus-background": '#16a34a',
    },
    "@delete_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 1,
        'col': 2,
        'relative_row': 1,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 7,
        'cell_name': 'card',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 5,
        'margin_right': 66,
        'margin_bottom': 1,
        'width': 3,
        'height': 5,
        'type': 'checkbox',
        'name': 'sel'
    },
    {
        'row': 1,
        'col': 6,
        'relative_row': 1,
        'relative_col': 6,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 7,
        'cell_name': 'card',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 8,
        'chars_height': 5,
        'margin_right': 57,
        'margin_bottom': 1,
        'width': 8,
        'height': 5,
        'type': 'image',
        'name': 'thumb'
    },
    {
        'row': 1,
        'col': 16,
        'relative_row': 1,
        'relative_col': 16,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 7,
        'cell_name': 'card',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 16,
        'chars_height': 2,
        'margin_right': 39,
        'margin_bottom': 4,
        'height': 2,
        'type': 'label',
        'name': 'name_label'
    },
    {
        'row': 1,
        'col': 34,
        'relative_row': 1,
        'relative_col': 34,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 7,
        'cell_name': 'card',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 5,
        'margin_right': 26,
        'margin_bottom': 1,
        'width': 11,
        'height': 5,
        'type': 'button',
        'name': 'show_btn'
    },
    {
        'row': 1,
        'col': 46,
        'relative_row': 1,
        'relative_col': 46,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 7,
        'cell_name': 'card',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 5,
        'margin_right': 14,
        'margin_bottom': 1,
        'width': 11,
        'height': 5,
        'type': 'button',
        'name': 'browse_btn'
    },
    {
        'row': 1,
        'col': 58,
        'relative_row': 1,
        'relative_col': 58,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 7,
        'cell_name': 'card',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 5,
        'margin_right': 2,
        'margin_bottom': 1,
        'width': 11,
        'height': 5,
        'type': 'button',
        'name': 'delete_btn'
    },
    {
        'row': 3,
        'col': 16,
        'relative_row': 3,
        'relative_col': 16,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 7,
        'cell_name': 'card',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 53,
        'chars_height': 2,
        'margin_right': 2,
        'margin_bottom': 2,
        'height': 2,
        'type': 'label',
        'name': 'path_label'
    },
    {
        'row': 5,
        'col': 16,
        'relative_row': 5,
        'relative_col': 16,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 71,
        'cell_chars_height': 7,
        'cell_name': 'card',
        'cell_height': 7,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 53,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'spacer'
    }
]
