"""Auto-generated UI code for formular_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow



class FormularUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# FormApp\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance."\ntags: [example]\n```\n\n## Members\n\n```yaml\ntitle_label:\n  type: label\n  text: "User profile"\n  description: "Title of the form."\n  expose: false\n\nname_label:\n  type: label\n  text: Name\n  description: "Label for the name field."\n\nname_input:\n  type: textinput\n  value: ""\n  description: "User name text field."\n\nemail_label:\n  type: label\n  text: Email\n  description: "Label for the email field."\n\nemail_input:\n  type: textinput\n  value: ""\n  description: "Email address text field."\n\nage_label:\n  type: label\n  text: Age\n  description: "Label for the age field."\n\nage_input:\n  type: numberinput\n  description: "Numeric age input."\n\ndescription_input:\n  type: textarea\n  description: "Multiline profile description field."\n\ncountry_label:\n  type: label\n  text: Country\n  description: "Label for the country selector."\n\ncountry_combo:\n  type: combobox\n  options: [Slovakia, "Czech Republic", Poland, Hungary, Austria]\n  selected_item: Slovakia\n  description: "Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options."\n\nrole_label:\n  type: label\n  text: Role\n  description: "Label for the role selector."\n\nrole_listbox:\n  type: listbox\n  options: [Developer, Designer, Manager, QA, DevOps, Tester, Hacker]\n  selected_items: [Developer]\n  multiple: "true"\n  description: "Multi-select role listbox for choosing one or more user roles."\n\nnotify_check:\n  type: checkbox\n  title: "Email notifications"\n  description: "Toggle email notifications."\n\nterms_check:\n  type: checkbox\n  title: "I accept the terms and conditions"\n  description: "Toggle acceptance of terms and conditions."\n\nsave_btn:\n  type: button\n  title: Save\n  description: "Submit the form and serialize the entered data."\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: "Cancel the form action."\n```\n\n## Style\n\n```yaml\n@title_label:\n  text-align: center\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------+\n|                                                    |\n|  title_label.....................................  |\n|                                                    |\n|  name_label       name_input.....................  |\n|                                                    |\n|  email_label      email_input....................  |\n|                                                    |\n|  age_label        age_input......................  |\n|                                                    |\n|  country_label    country_combo..................  |\n|                                                    |\n|  "Description"    description_input..............  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  role_label       role_listbox...................  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  notify_check....................................  |\n|  terms_check.....................................  |\n|                                                    |\n|       save_btn......      cancel_btn.......        |\n|                                                    |\n+----------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.'}, 'elements': {'title_label': {'role': 'text', 'description': 'Title of the form.', 'expose': False}, 'name_label': {'role': 'text', 'description': 'Label for the name field.', 'expose': True}, 'name_input': {'role': 'input', 'description': 'User name text field.', 'expose': True}, 'email_label': {'role': 'text', 'description': 'Label for the email field.', 'expose': True}, 'email_input': {'role': 'input', 'description': 'Email address text field.', 'expose': True}, 'age_label': {'role': 'text', 'description': 'Label for the age field.', 'expose': True}, 'age_input': {'role': 'input', 'description': 'Numeric age input.', 'expose': True}, 'description_input': {'role': 'input', 'description': 'Multiline profile description field.', 'expose': True}, 'country_label': {'role': 'text', 'description': 'Label for the country selector.', 'expose': True}, 'country_combo': {'role': 'selection', 'description': 'Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.', 'expose': True}, 'role_label': {'role': 'text', 'description': 'Label for the role selector.', 'expose': True}, 'role_listbox': {'role': 'selection', 'description': 'Multi-select role listbox for choosing one or more user roles.', 'expose': True}, 'notify_check': {'role': 'toggle', 'description': 'Toggle email notifications.', 'expose': True}, 'terms_check': {'role': 'toggle', 'description': 'Toggle acceptance of terms and conditions.', 'expose': True}, 'save_btn': {'role': 'action', 'description': 'Submit the form and serialize the entered data.', 'expose': True}, 'cancel_btn': {'role': 'action', 'description': 'Cancel the form action.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="FormApp",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

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
        "description": 'Title of the form.',
        "expose": False,
    },
    "name_label": {
        "type": 'label',
        "text": 'Name',
        "description": 'Label for the name field.',
    },
    "name_input": {
        "type": 'textinput',
        "value": '',
        "description": 'User name text field.',
    },
    "email_label": {
        "type": 'label',
        "text": 'Email',
        "description": 'Label for the email field.',
    },
    "email_input": {
        "type": 'textinput',
        "value": '',
        "description": 'Email address text field.',
    },
    "age_label": {
        "type": 'label',
        "text": 'Age',
        "description": 'Label for the age field.',
    },
    "age_input": {
        "type": 'numberinput',
        "description": 'Numeric age input.',
    },
    "description_input": {
        "type": 'textarea',
        "description": 'Multiline profile description field.',
    },
    "country_label": {
        "type": 'label',
        "text": 'Country',
        "description": 'Label for the country selector.',
    },
    "country_combo": {
        "type": 'combobox',
        "options": ['Slovakia', 'Czech Republic', 'Poland', 'Hungary', 'Austria'],
        "selected_item": 'Slovakia',
        "description": 'Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.',
    },
    "role_label": {
        "type": 'label',
        "text": 'Role',
        "description": 'Label for the role selector.',
    },
    "role_listbox": {
        "type": 'listbox',
        "options": ['Developer', 'Designer', 'Manager', 'QA', 'DevOps', 'Tester', 'Hacker'],
        "selected_items": ['Developer'],
        "multiple": 'true',
        "description": 'Multi-select role listbox for choosing one or more user roles.',
    },
    "notify_check": {
        "type": 'checkbox',
        "title": 'Email notifications',
        "description": 'Toggle email notifications.',
    },
    "terms_check": {
        "type": 'checkbox',
        "title": 'I accept the terms and conditions',
        "description": 'Toggle acceptance of terms and conditions.',
    },
    "save_btn": {
        "type": 'button',
        "title": 'Save',
        "description": 'Submit the form and serialize the entered data.',
    },
    "cancel_btn": {
        "type": 'button',
        "title": 'Cancel',
        "description": 'Cancel the form action.',
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
