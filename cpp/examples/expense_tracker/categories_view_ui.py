"""Auto-generated UI code for categories_view_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIControl



class CategoriesViewUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Categories View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: "Simple category list management view."\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Categories\n\ncategory_name:\n  type: textinput\n  value: ""\n  maxlength: 40\n\nadd_category_btn:\n  type: button\n  title: Add\n\ncategories:\n  type: listbox\n  options: [Food, Transport, Home, Health, Other]\n  selected_items: [Food]\n\nremove_category_btn:\n  type: button\n  title: Remove selected\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#0b1f33"\n@view_title:\n  color: "#bbf7d0"\nlistbox:\n  background: "#081827"\n  color: "#e5e7eb"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| "Name:" category_name..**................................ |\n|                                                            |\n| add_category_btn................ remove_category_btn..... |\n|                                                            |\n| categories..**............................................ |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Simple category list management view.'}, 'elements': {'view_title': {'role': 'text', 'description': 'Categories', 'expose': True}, 'category_name': {'role': 'input', 'description': 'category_name', 'expose': True}, 'add_category_btn': {'role': 'action', 'description': 'Add', 'expose': True}, 'categories': {'role': 'selection', 'description': 'categories', 'expose': True}, 'remove_category_btn': {'role': 'action', 'description': 'Remove selected', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Categories View",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.add_category_btn = self.create_element("add_category_btn", "button", title='Add')
        self.categories = self.create_element("categories", "listbox", options=['Food', 'Transport', 'Home', 'Health', 'Other'], selected_items=['Food'])
        self.category_name = self.create_element("category_name", "textinput", value='', maxlength=40)
        self.remove_category_btn = self.create_element("remove_category_btn", "button", title='Remove selected')
        self.view_title = self.create_element("view_title", "label", text='Categories')
        self._label_1 = self.create_element("_label_1", "label", text='Name:')
    def on_category_name_change(self, value):
        pass

    def on_category_name_submit(self, value):
        pass

    def on_add_category_btn_click(self):
        pass

    def on_categories_selection_change(self, value):
        pass

    def on_remove_category_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.category_name:
            self.on_category_name_change(value)
        elif element is self.add_category_btn:
            self.on_add_category_btn_click()
        elif element is self.remove_category_btn:
            self.on_remove_category_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.category_name:
            self.on_category_name_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.categories:
            self.on_categories_selection_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "view_title": {
        "type": 'label',
        "text": 'Categories',
    },
    "category_name": {
        "type": 'textinput',
        "value": '',
        "maxlength": 40,
    },
    "add_category_btn": {
        "type": 'button',
        "title": 'Add',
    },
    "categories": {
        "type": 'listbox',
        "options": ['Food', 'Transport', 'Home', 'Health', 'Other'],
        "selected_items": ['Food'],
    },
    "remove_category_btn": {
        "type": 'button',
        "title": 'Remove selected',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#0b1f33',
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
        "background": '#081827',
        "color": '#e5e7eb',
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
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "@view_title": {
        "color": '#bbf7d0',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 11,
        'cell_name': 'view',
        'cell_height': 11,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 58,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 10,
        'width': 58,
        'height': 1,
        'type': 'label',
        'name': 'view_title'
    },
    {
        'row': 2,
        'col': 1,
        'relative_row': 2,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 11,
        'cell_name': 'view',
        'cell_height': 11,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 1,
        'margin_right': 52,
        'margin_bottom': 8,
        'width': 7,
        'height': 1,
        'type': 'label',
        'name': '_label_1',
        'text': 'Name:'
    },
    {
        'row': 2,
        'col': 9,
        'relative_row': 2,
        'relative_col': 9,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 11,
        'cell_name': 'view',
        'cell_height': 11,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 49,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 8,
        'height': 1,
        'type': 'textinput',
        'name': 'category_name'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 11,
        'cell_name': 'view',
        'cell_height': 11,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 32,
        'chars_height': 1,
        'margin_right': 27,
        'margin_bottom': 6,
        'width': 32,
        'height': 1,
        'type': 'button',
        'name': 'add_category_btn'
    },
    {
        'row': 4,
        'col': 34,
        'relative_row': 4,
        'relative_col': 34,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 11,
        'cell_name': 'view',
        'cell_height': 11,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 24,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 6,
        'width': 24,
        'height': 1,
        'type': 'button',
        'name': 'remove_category_btn'
    },
    {
        'row': 6,
        'col': 1,
        'relative_row': 6,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 60,
        'cell_chars_height': 11,
        'cell_name': 'view',
        'cell_height': 11,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 58,
        'chars_height': 5,
        'margin_right': 1,
        'type': 'listbox',
        'name': 'categories'
    }
]
