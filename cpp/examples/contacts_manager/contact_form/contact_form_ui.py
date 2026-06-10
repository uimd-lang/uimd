"""Auto-generated UI code for contact_form_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow



class ContactFormUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Contact Form\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback."\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Contact\n  description: "Dialog title showing that a contact is being edited."\n  expose: false\n\nfirst_label:\n  type: label\n  text: "First name"\n  description: "Label for the first name input."\n\nfirst_name:\n  type: textinput\n  value: ""\n  maxlength: 80\n  description: "Required first name field."\n\nlast_label:\n  type: label\n  text: "Last name"\n  description: "Label for the last name input."\n\nlast_name:\n  type: textinput\n  value: ""\n  maxlength: 80\n  description: "Required last name field."\n\nemail_label:\n  type: label\n  text: Email\n  description: "Label for the email input."\n\nemail:\n  type: textinput\n  value: ""\n  maxlength: 120\n  description: "Required email address field."\n\nphone_label:\n  type: label\n  text: Phone\n  description: "Label for the phone input."\n\nphone:\n  type: textinput\n  value: ""\n  maxlength: 80\n  description: "Phone number field."\n\ncompany_label:\n  type: label\n  text: Company\n  description: "Label for the company input."\n\ncompany:\n  type: textinput\n  value: ""\n  maxlength: 120\n  description: "Company name field."\n\ncity_label:\n  type: label\n  text: City\n  description: "Label for the city input."\n\ncity:\n  type: textinput\n  value: ""\n  maxlength: 80\n  description: "City field."\n\ncategory_label:\n  type: label\n  text: Category\n  description: "Label for the contact category selector."\n\ncategory:\n  type: combobox\n  options: [Client, Vendor, Partner, Personal]\n  selected_item: Client\n  description: "Contact category combobox with client, vendor, partner, and personal options."\n\nactive:\n  type: checkbox\n  title: "Active contact"\n  value: 1\n  description: "Toggle whether the contact is active."\n\nnotes_label:\n  type: label\n  text: Notes\n  description: "Label for the notes field."\n\nnotes:\n  type: textarea\n  value: ""\n  maxlength: 300\n  description: "Multiline notes field for the contact."\n\nsave_btn:\n  type: button\n  title: Save\n  description: "Validate and save the contact form."\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: "Close the form without saving changes."\n\nmessage:\n  type: label\n  text: "Required: first name, last name, email."\n  description: "Validation and status message area."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: "#1f2937"\n  color: "#ffffff"\ntextinput:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ntextarea:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ncombobox:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@message:\n  color: "#fbbf24"\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------------+\n|header....................................................|\n+----------------------------------------------------------+\n|                                                          |\n|  first_label    first_name.............................  |\n|                                                          |\n|  last_label     last_name..............................  |\n|                                                          |\n|  email_label    email..................................  |\n|                                                          |\n|  phone_label    phone..................................  |\n|                                                          |\n|  company_label  company................................  |\n|                                                          |\n|  city_label     city...................................  |\n|                                                          |\n|  category_label  category..............................  |\n|                                                          |\n|  active................................................  |\n|                                                          |\n|  notes_label notes.....................................  |\n|              ..........................................  |\n|              ..........................................  |\n|                                                          |\n|  message...............................................  |\n|                                                          |\n+----------------------------------------------------------+\n|  save_btn..................  cancel_btn................  |\n|                                                          |\n+----------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.'}, 'elements': {'header': {'role': 'text', 'description': 'Dialog title showing that a contact is being edited.', 'expose': False}, 'first_label': {'role': 'text', 'description': 'Label for the first name input.', 'expose': True}, 'first_name': {'role': 'input', 'description': 'Required first name field.', 'expose': True}, 'last_label': {'role': 'text', 'description': 'Label for the last name input.', 'expose': True}, 'last_name': {'role': 'input', 'description': 'Required last name field.', 'expose': True}, 'email_label': {'role': 'text', 'description': 'Label for the email input.', 'expose': True}, 'email': {'role': 'input', 'description': 'Required email address field.', 'expose': True}, 'phone_label': {'role': 'text', 'description': 'Label for the phone input.', 'expose': True}, 'phone': {'role': 'input', 'description': 'Phone number field.', 'expose': True}, 'company_label': {'role': 'text', 'description': 'Label for the company input.', 'expose': True}, 'company': {'role': 'input', 'description': 'Company name field.', 'expose': True}, 'city_label': {'role': 'text', 'description': 'Label for the city input.', 'expose': True}, 'city': {'role': 'input', 'description': 'City field.', 'expose': True}, 'category_label': {'role': 'text', 'description': 'Label for the contact category selector.', 'expose': True}, 'category': {'role': 'selection', 'description': 'Contact category combobox with client, vendor, partner, and personal options.', 'expose': True}, 'active': {'role': 'toggle', 'description': 'Toggle whether the contact is active.', 'expose': True}, 'notes_label': {'role': 'text', 'description': 'Label for the notes field.', 'expose': True}, 'notes': {'role': 'input', 'description': 'Multiline notes field for the contact.', 'expose': True}, 'save_btn': {'role': 'action', 'description': 'Validate and save the contact form.', 'expose': True}, 'cancel_btn': {'role': 'action', 'description': 'Close the form without saving changes.', 'expose': True}, 'message': {'role': 'text', 'description': 'Validation and status message area.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Contact Form",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.active = self.create_element("active", "checkbox", value=1, title='Active contact')
        self.cancel_btn = self.create_element("cancel_btn", "button", title='Cancel')
        self.category = self.create_element("category", "combobox", options=['Client', 'Vendor', 'Partner', 'Personal'], selected_item='Client')
        self.category_label = self.create_element("category_label", "label", text='Category')
        self.city = self.create_element("city", "textinput", value='', maxlength=80)
        self.city_label = self.create_element("city_label", "label", text='City')
        self.company = self.create_element("company", "textinput", value='', maxlength=120)
        self.company_label = self.create_element("company_label", "label", text='Company')
        self.email = self.create_element("email", "textinput", value='', maxlength=120)
        self.email_label = self.create_element("email_label", "label", text='Email')
        self.first_label = self.create_element("first_label", "label", text='First name')
        self.first_name = self.create_element("first_name", "textinput", value='', maxlength=80)
        self.header = self.create_element("header", "label", text='Contact')
        self.last_label = self.create_element("last_label", "label", text='Last name')
        self.last_name = self.create_element("last_name", "textinput", value='', maxlength=80)
        self.message = self.create_element("message", "label", text='Required: first name, last name, email.')
        self.notes = self.create_element("notes", "textarea", value='', maxlength=300)
        self.notes_label = self.create_element("notes_label", "label", text='Notes')
        self.phone = self.create_element("phone", "textinput", value='', maxlength=80)
        self.phone_label = self.create_element("phone_label", "label", text='Phone')
        self.save_btn = self.create_element("save_btn", "button", title='Save')
    def on_first_name_change(self, value):
        pass

    def on_first_name_submit(self, value):
        pass

    def on_last_name_change(self, value):
        pass

    def on_last_name_submit(self, value):
        pass

    def on_email_change(self, value):
        pass

    def on_email_submit(self, value):
        pass

    def on_phone_change(self, value):
        pass

    def on_phone_submit(self, value):
        pass

    def on_company_change(self, value):
        pass

    def on_company_submit(self, value):
        pass

    def on_city_change(self, value):
        pass

    def on_city_submit(self, value):
        pass

    def on_category_change(self, value):
        pass

    def on_active_change(self, value):
        pass

    def on_notes_change(self, value):
        pass

    def on_notes_submit(self, value):
        pass

    def on_save_btn_click(self):
        pass

    def on_cancel_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.first_name:
            self.on_first_name_change(value)
        elif element is self.last_name:
            self.on_last_name_change(value)
        elif element is self.email:
            self.on_email_change(value)
        elif element is self.phone:
            self.on_phone_change(value)
        elif element is self.company:
            self.on_company_change(value)
        elif element is self.city:
            self.on_city_change(value)
        elif element is self.active:
            self.on_active_change(value)
        elif element is self.notes:
            self.on_notes_change(value)
        elif element is self.save_btn:
            self.on_save_btn_click()
        elif element is self.cancel_btn:
            self.on_cancel_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.first_name:
            self.on_first_name_submit(value)
        elif element is self.last_name:
            self.on_last_name_submit(value)
        elif element is self.email:
            self.on_email_submit(value)
        elif element is self.phone:
            self.on_phone_submit(value)
        elif element is self.company:
            self.on_company_submit(value)
        elif element is self.city:
            self.on_city_submit(value)
        elif element is self.notes:
            self.on_notes_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.category:
            self.on_category_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header": {
        "type": 'label',
        "text": 'Contact',
        "description": 'Dialog title showing that a contact is being edited.',
        "expose": False,
    },
    "first_label": {
        "type": 'label',
        "text": 'First name',
        "description": 'Label for the first name input.',
    },
    "first_name": {
        "type": 'textinput',
        "value": '',
        "maxlength": 80,
        "description": 'Required first name field.',
    },
    "last_label": {
        "type": 'label',
        "text": 'Last name',
        "description": 'Label for the last name input.',
    },
    "last_name": {
        "type": 'textinput',
        "value": '',
        "maxlength": 80,
        "description": 'Required last name field.',
    },
    "email_label": {
        "type": 'label',
        "text": 'Email',
        "description": 'Label for the email input.',
    },
    "email": {
        "type": 'textinput',
        "value": '',
        "maxlength": 120,
        "description": 'Required email address field.',
    },
    "phone_label": {
        "type": 'label',
        "text": 'Phone',
        "description": 'Label for the phone input.',
    },
    "phone": {
        "type": 'textinput',
        "value": '',
        "maxlength": 80,
        "description": 'Phone number field.',
    },
    "company_label": {
        "type": 'label',
        "text": 'Company',
        "description": 'Label for the company input.',
    },
    "company": {
        "type": 'textinput',
        "value": '',
        "maxlength": 120,
        "description": 'Company name field.',
    },
    "city_label": {
        "type": 'label',
        "text": 'City',
        "description": 'Label for the city input.',
    },
    "city": {
        "type": 'textinput',
        "value": '',
        "maxlength": 80,
        "description": 'City field.',
    },
    "category_label": {
        "type": 'label',
        "text": 'Category',
        "description": 'Label for the contact category selector.',
    },
    "category": {
        "type": 'combobox',
        "options": ['Client', 'Vendor', 'Partner', 'Personal'],
        "selected_item": 'Client',
        "description": 'Contact category combobox with client, vendor, partner, and personal options.',
    },
    "active": {
        "type": 'checkbox',
        "title": 'Active contact',
        "value": 1,
        "description": 'Toggle whether the contact is active.',
    },
    "notes_label": {
        "type": 'label',
        "text": 'Notes',
        "description": 'Label for the notes field.',
    },
    "notes": {
        "type": 'textarea',
        "value": '',
        "maxlength": 300,
        "description": 'Multiline notes field for the contact.',
    },
    "save_btn": {
        "type": 'button',
        "title": 'Save',
        "description": 'Validate and save the contact form.',
    },
    "cancel_btn": {
        "type": 'button',
        "title": 'Cancel',
        "description": 'Close the form without saving changes.',
    },
    "message": {
        "type": 'label',
        "text": 'Required: first name, last name, email.',
        "description": 'Validation and status message area.',
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
        "background": '#0f172a',
        "color": '#e5e7eb',
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
        "focus-background": '#243a5c',
        "focus-color": '#ffffff',
        "edit-background": '#2d456d',
        "edit-color": '#ffffff',
        "cursor-background": '#ffffff',
        "cursor-color": '#0e1117',
    },
    "combobox": {
        "background": '#0f172a',
        "color": '#e5e7eb',
        "focus-background": '#1e293b',
        "focus-color": '#ffffff',
        "edit-background": '#111827',
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
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "@header": {
        "text-align": 'center',
        "background": '#1f2937',
        "color": '#ffffff',
    },
    "textarea": {
        "background": '#0f172a',
        "color": '#e5e7eb',
        "focus-background": '#1e293b',
        "edit-background": '#111827',
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
    },
    "@message": {
        "color": '#fbbf24',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 1,
        'cell_width': 58,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 58,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': 'header'
    },
    {
        'row': 0,
        'col': 2,
        'relative_col': 2,
        'cell_row': 26,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 2,
        'cell_width': 58,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 30,
        'margin_bottom': 1,
        'width': 26,
        'height': 1,
        'type': 'button',
        'name': 'save_btn'
    },
    {
        'row': 0,
        'col': 30,
        'relative_col': 30,
        'cell_row': 26,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 2,
        'cell_width': 58,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'button',
        'name': 'cancel_btn'
    },
    {
        'row': 1,
        'col': 2,
        'relative_row': 1,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 45,
        'margin_bottom': 21,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': 'first_label'
    },
    {
        'row': 1,
        'col': 17,
        'relative_row': 1,
        'relative_col': 17,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 39,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 21,
        'height': 1,
        'type': 'textinput',
        'name': 'first_name'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 46,
        'margin_bottom': 19,
        'width': 10,
        'height': 1,
        'type': 'label',
        'name': 'last_label'
    },
    {
        'row': 3,
        'col': 17,
        'relative_row': 3,
        'relative_col': 17,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 39,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 19,
        'height': 1,
        'type': 'textinput',
        'name': 'last_name'
    },
    {
        'row': 5,
        'col': 2,
        'relative_row': 5,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 45,
        'margin_bottom': 17,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': 'email_label'
    },
    {
        'row': 5,
        'col': 17,
        'relative_row': 5,
        'relative_col': 17,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 39,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 17,
        'height': 1,
        'type': 'textinput',
        'name': 'email'
    },
    {
        'row': 7,
        'col': 2,
        'relative_row': 7,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 45,
        'margin_bottom': 15,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': 'phone_label'
    },
    {
        'row': 7,
        'col': 17,
        'relative_row': 7,
        'relative_col': 17,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 39,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 15,
        'height': 1,
        'type': 'textinput',
        'name': 'phone'
    },
    {
        'row': 9,
        'col': 2,
        'relative_row': 9,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 13,
        'chars_height': 1,
        'margin_right': 43,
        'margin_bottom': 13,
        'width': 13,
        'height': 1,
        'type': 'label',
        'name': 'company_label'
    },
    {
        'row': 9,
        'col': 17,
        'relative_row': 9,
        'relative_col': 17,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 39,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 13,
        'height': 1,
        'type': 'textinput',
        'name': 'company'
    },
    {
        'row': 11,
        'col': 2,
        'relative_row': 11,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 46,
        'margin_bottom': 11,
        'width': 10,
        'height': 1,
        'type': 'label',
        'name': 'city_label'
    },
    {
        'row': 11,
        'col': 17,
        'relative_row': 11,
        'relative_col': 17,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 39,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 11,
        'height': 1,
        'type': 'textinput',
        'name': 'city'
    },
    {
        'row': 13,
        'col': 2,
        'relative_row': 13,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 14,
        'chars_height': 1,
        'margin_right': 42,
        'margin_bottom': 9,
        'width': 14,
        'height': 1,
        'type': 'label',
        'name': 'category_label'
    },
    {
        'row': 13,
        'col': 18,
        'relative_row': 13,
        'relative_col': 18,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 38,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 9,
        'height': 1,
        'type': 'combobox',
        'name': 'category'
    },
    {
        'row': 15,
        'col': 2,
        'relative_row': 15,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 54,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 7,
        'height': 1,
        'type': 'checkbox',
        'name': 'active'
    },
    {
        'row': 17,
        'col': 2,
        'relative_row': 17,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 45,
        'margin_bottom': 5,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': 'notes_label'
    },
    {
        'row': 17,
        'col': 14,
        'relative_row': 17,
        'relative_col': 14,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 42,
        'chars_height': 3,
        'margin_right': 2,
        'margin_bottom': 3,
        'height': 3,
        'type': 'textarea',
        'name': 'notes'
    },
    {
        'row': 21,
        'col': 2,
        'relative_row': 21,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 58,
        'cell_chars_height': 23,
        'cell_width': 58,
        'cell_height': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 54,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'message'
    }
]
