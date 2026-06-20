"""Auto-generated UI code for edit_page_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl



class EditPageUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Edit Page\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: "Edit page shown after a ScrollView row button replaces the active ViewHost page."\ntags: [regression, issue-5, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - edit_fields_scroll\n```\n\n## Members\n\n```yaml\nedit_title:\n  type: label\n  text: "Edit item"\n  description: "Edit page title."\n  expose: false\n\nback_btn:\n  type: button\n  title: Back\n  description: "Return to the list page."\n\nsave_btn:\n  type: button\n  title: Save\n  description: "Dummy save action used by the regression app."\n\nname_label:\n  type: label\n  text: Name\n  description: "Name field label."\n  expose: false\n\nname_input:\n  type: textinput\n  value: ""\n  maxlength: 80\n  description: "Editable item name."\n\nbody_label:\n  type: label\n  text: Fields\n  description: "Fields panel label."\n  expose: false\n\nfields:\n  type: viewhost\n  description: "Scrollable edit fields panel."\n\nstatus:\n  type: label\n  text: "Opened from ScrollView"\n  description: "Edit page status."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#030712"\n@top:\n  background: "#172033"\n@edit_title:\n  color: "#ffffff"\n  text-align: center\n@form:\n  background: "#111827"\n  padding: 1, 1, 1, 1\n@fields:\n  background: "#030712"\n  focus-background: "#07111f"\n  edit-background: "#0f172a"\n  scope-dim-background: "#0000004d"\n@status:\n  color: "#93c5fd"\nlabel:\n  color: "#e5e7eb"\ntextinput:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ntextarea:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@save_btn:\n  background: "#14532d"\n  focus-background: "#16a34a"\n```\n\n## User Interface\n\n```ui\n+-top-**-----------------------------------------------------------------------------------+\n| back_btn.... edit_title..**.............................................. save_btn...... |\n+-form-**----------------------------------------------------------------------------------+\n| name_label.... name_input..**........................................................... |\n|                                                                                          |\n| status..**.............................................................................. |\n|                                                                                          |\n| body_label..**.......................................................................... |\n| fields..**.............................................................................. |\n| *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n|                                                                                          |\n+------------------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Edit page shown after a ScrollView row button replaces the active ViewHost page.'}, 'elements': {'edit_title': {'role': 'text', 'description': 'Edit page title.', 'expose': False}, 'back_btn': {'role': 'action', 'description': 'Return to the list page.', 'expose': True}, 'save_btn': {'role': 'action', 'description': 'Dummy save action used by the regression app.', 'expose': True}, 'name_label': {'role': 'text', 'description': 'Name field label.', 'expose': False}, 'name_input': {'role': 'input', 'description': 'Editable item name.', 'expose': True}, 'body_label': {'role': 'text', 'description': 'Fields panel label.', 'expose': False}, 'fields': {'role': 'text', 'description': 'Scrollable edit fields panel.', 'expose': True}, 'status': {'role': 'text', 'description': 'Edit page status.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Edit Page",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'component'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.back_btn = self.create_element("back_btn", "button", title='Back')
        self.body_label = self.create_element("body_label", "label", text='Fields')
        self.edit_title = self.create_element("edit_title", "label", text='Edit item')
        self.fields = self.create_element("fields", "viewhost", )
        self.name_input = self.create_element("name_input", "textinput", value='', maxlength=80)
        self.name_label = self.create_element("name_label", "label", text='Name')
        self.save_btn = self.create_element("save_btn", "button", title='Save')
        self.status = self.create_element("status", "label", text='Opened from ScrollView')
    def on_back_btn_click(self):
        pass

    def on_save_btn_click(self):
        pass

    def on_name_input_change(self, value):
        pass

    def on_name_input_submit(self, value):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.back_btn:
            self.on_back_btn_click()
        elif element is self.save_btn:
            self.on_save_btn_click()
        elif element is self.name_input:
            self.on_name_input_change(value)
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.name_input:
            self.on_name_input_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "edit_title": {
        "type": 'label',
        "text": 'Edit item',
        "description": 'Edit page title.',
        "expose": False,
    },
    "back_btn": {
        "type": 'button',
        "title": 'Back',
        "description": 'Return to the list page.',
    },
    "save_btn": {
        "type": 'button',
        "title": 'Save',
        "description": 'Dummy save action used by the regression app.',
    },
    "name_label": {
        "type": 'label',
        "text": 'Name',
        "description": 'Name field label.',
        "expose": False,
    },
    "name_input": {
        "type": 'textinput',
        "value": '',
        "maxlength": 80,
        "description": 'Editable item name.',
    },
    "body_label": {
        "type": 'label',
        "text": 'Fields',
        "description": 'Fields panel label.',
        "expose": False,
    },
    "fields": {
        "type": 'viewhost',
        "description": 'Scrollable edit fields panel.',
    },
    "status": {
        "type": 'label',
        "text": 'Opened from ScrollView',
        "description": 'Edit page status.',
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
        "color": '#e5e7eb',
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
        "background": '#0f172a',
        "color": '#e5e7eb',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
        "focus-background": '#1e293b',
        "focus-color": '#ffffff',
        "edit-background": '#111827',
        "edit-color": '#ffffff',
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
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
    "@top": {
        "background": '#172033',
    },
    "@edit_title": {
        "color": '#ffffff',
        "text-align": 'center',
    },
    "@form": {
        "background": '#111827',
        "padding": '1, 1, 1, 1',
    },
    "@fields": {
        "background": '#030712',
        "focus-background": '#07111f',
        "edit-background": '#0f172a',
        "scope-dim-background": '#0000004d',
    },
    "@status": {
        "color": '#93c5fd',
    },
    "textarea": {
        "background": '#0f172a',
        "color": '#e5e7eb',
        "focus-background": '#1e293b',
        "edit-background": '#111827',
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
    },
    "@save_btn": {
        "background": '#14532d',
        "focus-background": '#16a34a',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 1,
        'cell_name': 'top',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 77,
        'width': 12,
        'height': 1,
        'type': 'button',
        'name': 'back_btn'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 21,
        'cell_name': 'form',
        'cell_height': 21,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 14,
        'chars_height': 1,
        'margin_right': 75,
        'margin_bottom': 20,
        'width': 14,
        'height': 1,
        'type': 'label',
        'name': 'name_label'
    },
    {
        'row': 0,
        'col': 14,
        'relative_col': 14,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 1,
        'cell_name': 'top',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 60,
        'chars_height': 1,
        'margin_right': 16,
        'height': 1,
        'type': 'label',
        'name': 'edit_title'
    },
    {
        'row': 0,
        'col': 16,
        'relative_col': 16,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 21,
        'cell_name': 'form',
        'cell_height': 21,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 73,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 20,
        'height': 1,
        'type': 'textinput',
        'name': 'name_input'
    },
    {
        'row': 0,
        'col': 75,
        'relative_col': 75,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 1,
        'cell_name': 'top',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 14,
        'chars_height': 1,
        'margin_right': 1,
        'width': 14,
        'height': 1,
        'type': 'button',
        'name': 'save_btn'
    },
    {
        'row': 2,
        'col': 1,
        'relative_row': 2,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 21,
        'cell_name': 'form',
        'cell_height': 21,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 88,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 18,
        'height': 1,
        'type': 'label',
        'name': 'status'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 21,
        'cell_name': 'form',
        'cell_height': 21,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 88,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 16,
        'height': 1,
        'type': 'label',
        'name': 'body_label'
    },
    {
        'row': 5,
        'col': 1,
        'relative_row': 5,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 21,
        'cell_name': 'form',
        'cell_height': 21,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 88,
        'chars_height': 15,
        'margin_right': 1,
        'margin_bottom': 1,
        'type': 'viewhost',
        'name': 'fields'
    }
]
