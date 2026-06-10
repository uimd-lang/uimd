"""Auto-generated UI code for formular_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow



class FormularUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# FormApp\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "FormApp UI source."\ntags: [example]\n```\n\n## Members\n\n```yaml\ntitle_label:\n  type: label\n  text: "User profile"\n  expose: false\n\nname_label:\n  type: label\n  text: Name\n\nname_input:\n  type: textinput\n  value: ""\n\nemail_label:\n  type: label\n  text: Email\n\nemail_input:\n  type: textinput\n  value: ""\n\nage_label:\n  type: label\n  text: Age\n\nage_input:\n  type: numberinput\n\ndescription_input:\n  type: textarea\n\ncountry_label:\n  type: label\n  text: Country\n\ncountry_combo:\n  type: combobox\n  options: [Slovakia, "Czech Republic", Poland, Hungary, Austria]\n  selected_item: Slovakia\n\nrole_label:\n  type: label\n  text: Role\n\nrole_listbox:\n  type: listbox\n  options: [Developer, Designer, Manager, QA, DevOps, Tester, Hacker]\n  selected_items: [Developer]\n  multiple: "true"\n\nnotify_check:\n  type: checkbox\n  title: "Email notifications"\n\nterms_check:\n  type: checkbox\n  title: "I accept the terms and conditions"\n\nsave_btn:\n  type: button\n  title: Save\n\ncancel_btn:\n  type: button\n  title: Cancel\n```\n\n## Style\n\n```yaml\n@title_label:\n  text-align: center\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------+\n|                                                    |\n|  title_label.....................................  |\n|                                                    |\n|  name_label       name_input.....................  |\n|                                                    |\n|  email_label      email_input....................  |\n|                                                    |\n|  age_label        age_input......................  |\n|                                                    |\n|  country_label    country_combo..................  |\n|                                                    |\n|  "Description"    description_input..............  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  role_label       role_listbox...................  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  notify_check....................................  |\n|  terms_check.....................................  |\n|                                                    |\n|       save_btn......      cancel_btn.......        |\n|                                                    |\n+----------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'FormApp UI source.'}, 'elements': {'title_label': {'role': 'text', 'description': 'User profile', 'expose': False}, 'name_label': {'role': 'text', 'description': 'Name', 'expose': True}, 'name_input': {'role': 'input', 'description': 'name_input', 'expose': True}, 'email_label': {'role': 'text', 'description': 'Email', 'expose': True}, 'email_input': {'role': 'input', 'description': 'email_input', 'expose': True}, 'age_label': {'role': 'text', 'description': 'Age', 'expose': True}, 'age_input': {'role': 'input', 'description': 'age_input', 'expose': True}, 'description_input': {'role': 'input', 'description': 'description_input', 'expose': True}, 'country_label': {'role': 'text', 'description': 'Country', 'expose': True}, 'country_combo': {'role': 'selection', 'description': 'country_combo', 'expose': True}, 'role_label': {'role': 'text', 'description': 'Role', 'expose': True}, 'role_listbox': {'role': 'selection', 'description': 'role_listbox', 'expose': True}, 'notify_check': {'role': 'toggle', 'description': 'Email notifications', 'expose': True}, 'terms_check': {'role': 'toggle', 'description': 'I accept the terms and conditions', 'expose': True}, 'save_btn': {'role': 'action', 'description': 'Save', 'expose': True}, 'cancel_btn': {'role': 'action', 'description': 'Cancel', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="FormApp",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.age_input = self.create_element("age_input", "numberinput", value=0)
        self.age_label = self.create_element("age_label", "label", text='Age')
        self.cancel_btn = self.create_element("cancel_btn", "button", title='Cancel')
        self.country_combo = self.create_element("country_combo", "combobox", options=['Slovakia', 'Czech Republic', 'Poland', 'Hungary', 'Austria'], selected_item='Slovakia')
        self.country_label = self.create_element("country_label", "label", text='Country')
        self.description_input = self.create_element("description_input", "textarea", value='')
        self.email_input = self.create_element("email_input", "textinput", value='')
        self.email_label = self.create_element("email_label", "label", text='Email')
        self.name_input = self.create_element("name_input", "textinput", value='')
        self.name_label = self.create_element("name_label", "label", text='Name')
        self.notify_check = self.create_element("notify_check", "checkbox", value=False, title='Email notifications')
        self.role_label = self.create_element("role_label", "label", text='Role')
        self.role_listbox = self.create_element("role_listbox", "listbox", options=['Developer', 'Designer', 'Manager', 'QA', 'DevOps', 'Tester', 'Hacker'], selected_items=['Developer'], multiple=True)
        self.save_btn = self.create_element("save_btn", "button", title='Save')
        self.terms_check = self.create_element("terms_check", "checkbox", value=False, title='I accept the terms and conditions')
        self.title_label = self.create_element("title_label", "label", text='User profile')
        self._label_9 = self.create_element("_label_9", "label", text='Description')
    def on_name_input_change(self, value):
        pass

    def on_name_input_submit(self, value):
        pass

    def on_email_input_change(self, value):
        pass

    def on_email_input_submit(self, value):
        pass

    def on_age_input_change(self, value):
        pass

    def on_age_input_submit(self, value):
        pass

    def on_description_input_change(self, value):
        pass

    def on_description_input_submit(self, value):
        pass

    def on_country_combo_change(self, value):
        pass

    def on_role_listbox_selection_change(self, value):
        pass

    def on_notify_check_change(self, value):
        pass

    def on_terms_check_change(self, value):
        pass

    def on_save_btn_click(self):
        pass

    def on_cancel_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.name_input:
            self.on_name_input_change(value)
        elif element is self.email_input:
            self.on_email_input_change(value)
        elif element is self.age_input:
            self.on_age_input_change(value)
        elif element is self.description_input:
            self.on_description_input_change(value)
        elif element is self.notify_check:
            self.on_notify_check_change(value)
        elif element is self.terms_check:
            self.on_terms_check_change(value)
        elif element is self.save_btn:
            self.on_save_btn_click()
        elif element is self.cancel_btn:
            self.on_cancel_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.name_input:
            self.on_name_input_submit(value)
        elif element is self.email_input:
            self.on_email_input_submit(value)
        elif element is self.age_input:
            self.on_age_input_submit(value)
        elif element is self.description_input:
            self.on_description_input_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.country_combo:
            self.on_country_combo_change(value)
        elif element is self.role_listbox:
            self.on_role_listbox_selection_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "title_label": {
        "type": 'label',
        "text": 'User profile',
        "expose": False,
    },
    "name_label": {
        "type": 'label',
        "text": 'Name',
    },
    "name_input": {
        "type": 'textinput',
        "value": '',
    },
    "email_label": {
        "type": 'label',
        "text": 'Email',
    },
    "email_input": {
        "type": 'textinput',
        "value": '',
    },
    "age_label": {
        "type": 'label',
        "text": 'Age',
    },
    "age_input": {
        "type": 'numberinput',
    },
    "description_input": {
        "type": 'textarea',
    },
    "country_label": {
        "type": 'label',
        "text": 'Country',
    },
    "country_combo": {
        "type": 'combobox',
        "options": ['Slovakia', 'Czech Republic', 'Poland', 'Hungary', 'Austria'],
        "selected_item": 'Slovakia',
    },
    "role_label": {
        "type": 'label',
        "text": 'Role',
    },
    "role_listbox": {
        "type": 'listbox',
        "options": ['Developer', 'Designer', 'Manager', 'QA', 'DevOps', 'Tester', 'Hacker'],
        "selected_items": ['Developer'],
        "multiple": 'true',
    },
    "notify_check": {
        "type": 'checkbox',
        "title": 'Email notifications',
    },
    "terms_check": {
        "type": 'checkbox',
        "title": 'I accept the terms and conditions',
    },
    "save_btn": {
        "type": 'button',
        "title": 'Save',
    },
    "cancel_btn": {
        "type": 'button',
        "title": 'Cancel',
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
    "@title_label": {
        "text-align": 'center',
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
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 48,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 24,
        'height': 1,
        'type': 'label',
        'name': 'title_label'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 40,
        'margin_bottom': 22,
        'width': 10,
        'height': 1,
        'type': 'label',
        'name': 'name_label'
    },
    {
        'row': 3,
        'col': 19,
        'relative_row': 3,
        'relative_col': 19,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 22,
        'height': 1,
        'type': 'textinput',
        'name': 'name_input'
    },
    {
        'row': 5,
        'col': 2,
        'relative_row': 5,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 39,
        'margin_bottom': 20,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': 'email_label'
    },
    {
        'row': 5,
        'col': 19,
        'relative_row': 5,
        'relative_col': 19,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 20,
        'height': 1,
        'type': 'textinput',
        'name': 'email_input'
    },
    {
        'row': 7,
        'col': 2,
        'relative_row': 7,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 41,
        'margin_bottom': 18,
        'width': 9,
        'height': 1,
        'type': 'label',
        'name': 'age_label'
    },
    {
        'row': 7,
        'col': 19,
        'relative_row': 7,
        'relative_col': 19,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 18,
        'height': 1,
        'type': 'numberinput',
        'name': 'age_input'
    },
    {
        'row': 9,
        'col': 2,
        'relative_row': 9,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 13,
        'chars_height': 1,
        'margin_right': 37,
        'margin_bottom': 16,
        'width': 13,
        'height': 1,
        'type': 'label',
        'name': 'country_label'
    },
    {
        'row': 9,
        'col': 19,
        'relative_row': 9,
        'relative_col': 19,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 16,
        'height': 1,
        'type': 'combobox',
        'name': 'country_combo'
    },
    {
        'row': 11,
        'col': 2,
        'relative_row': 11,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 13,
        'chars_height': 1,
        'margin_right': 37,
        'margin_bottom': 14,
        'width': 13,
        'height': 1,
        'type': 'label',
        'name': '_label_9',
        'text': 'Description'
    },
    {
        'row': 11,
        'col': 19,
        'relative_row': 11,
        'relative_col': 19,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 4,
        'margin_right': 2,
        'margin_bottom': 11,
        'height': 4,
        'type': 'textarea',
        'name': 'description_input'
    },
    {
        'row': 16,
        'col': 2,
        'relative_row': 16,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 40,
        'margin_bottom': 9,
        'width': 10,
        'height': 1,
        'type': 'label',
        'name': 'role_label'
    },
    {
        'row': 16,
        'col': 19,
        'relative_row': 16,
        'relative_col': 19,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 4,
        'margin_right': 2,
        'margin_bottom': 6,
        'height': 4,
        'type': 'listbox',
        'name': 'role_listbox'
    },
    {
        'row': 21,
        'col': 2,
        'relative_row': 21,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 48,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 4,
        'height': 1,
        'type': 'checkbox',
        'name': 'notify_check'
    },
    {
        'row': 22,
        'col': 2,
        'relative_row': 22,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 48,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 3,
        'height': 1,
        'type': 'checkbox',
        'name': 'terms_check'
    },
    {
        'row': 24,
        'col': 7,
        'relative_row': 24,
        'relative_col': 7,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 14,
        'chars_height': 1,
        'margin_right': 31,
        'margin_bottom': 1,
        'width': 14,
        'height': 1,
        'type': 'button',
        'name': 'save_btn'
    },
    {
        'row': 24,
        'col': 27,
        'relative_row': 24,
        'relative_col': 27,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 52,
        'cell_chars_height': 26,
        'cell_width': 52,
        'cell_height': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 17,
        'chars_height': 1,
        'margin_right': 8,
        'margin_bottom': 1,
        'height': 1,
        'type': 'button',
        'name': 'cancel_btn'
    }
]
