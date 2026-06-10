"""Auto-generated UI code for contacts_manager_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'contact_form'))


class ContactsManagerUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Contacts Manager\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage."\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - contact_form\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: "Contacts Manager"\n  description: "Application title."\n  expose: false\n\ncontacts_label:\n  type: label\n  text: Contacts\n  description: "Label for the contacts list."\n\ncontacts:\n  type: listbox\n  options: []\n  selected_items: []\n  multiple: "false"\n  description: "List of contact records; selecting a row updates the detail panel."\n\ndetail:\n  type: label\n  text: "Select a contact."\n  description: "Read-only details for the selected contact."\n\nadd_btn:\n  type: button\n  title: Add\n  description: "Open the contact form to create a new contact."\n\nedit_btn:\n  type: button\n  title: Edit\n  description: "Open the selected contact for editing."\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: "Delete the selected contact."\n\nsave_btn:\n  type: button\n  title: "Save CSV"\n  description: "Save contacts to the configured CSV file."\n\nreload_btn:\n  type: button\n  title: Reload\n  description: "Reload contacts from the configured CSV file."\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: "Close the contacts manager."\n\nstatus:\n  type: label\n  text: Ready\n  description: "Shows save, load, and validation status messages."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: "#1f2937"\n  color: "#ffffff"\n@list:\n  background: "#101a2b"\n@contacts:\n  background: "#ffffff11"\n  color: "#cbd5e1"\n  focus-background: "#ffffff22"\n  edit-background: "#00000000"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\n@detail_cell:\n  background: "#0b1120"\n  padding: 1, 1, 1, 1\n@detail:\n  color: "#d1d5db"\n  user-select: text\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@delete_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n@close_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n@status:\n  background: "#111827"\n  color: "#93c5fd"\n```\n\n## User Interface\n\n```ui\n+-title-**-------------------------------------------------------------------------------+\n|header..**...................................................................close_btn..|\n+-actions-**-----------------------------------------------------------------------------+\n|                                                                                        |\n| add_btn....... edit_btn....... delete_btn....... save_btn....... reload_btn.......     |\n|                                                                                        |\n+-list-32---------------------------+-detail_cell-**-------------------------------------+\n| contacts_label.**................ | detail..**........................................ |\n| contacts.......**................ | .................................................. |\n| #................................ | #................................................. |\n| #................................ | #................................................. |\n| ................................. | .................................................. |\n* ................................. | .................................................. |\n* ................................. * .................................................. |\n| ................................. * .................................................. |\n| ................................. | .................................................. |\n|                                   | .................................................. |\n+-----------------------------------+----------------------------------------------------+\n| status..**............................................................................ |\n+----------------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.'}, 'elements': {'header': {'role': 'text', 'description': 'Application title.', 'expose': False}, 'contacts_label': {'role': 'text', 'description': 'Label for the contacts list.', 'expose': True}, 'contacts': {'role': 'selection', 'description': 'List of contact records; selecting a row updates the detail panel.', 'expose': True}, 'detail': {'role': 'text', 'description': 'Read-only details for the selected contact.', 'expose': True}, 'add_btn': {'role': 'action', 'description': 'Open the contact form to create a new contact.', 'expose': True}, 'edit_btn': {'role': 'action', 'description': 'Open the selected contact for editing.', 'expose': True}, 'delete_btn': {'role': 'action', 'description': 'Delete the selected contact.', 'expose': True}, 'save_btn': {'role': 'action', 'description': 'Save contacts to the configured CSV file.', 'expose': True}, 'reload_btn': {'role': 'action', 'description': 'Reload contacts from the configured CSV file.', 'expose': True}, 'close_btn': {'role': 'action', 'description': 'Close the contacts manager.', 'expose': True}, 'status': {'role': 'text', 'description': 'Shows save, load, and validation status messages.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Contacts Manager",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.add_btn = self.create_element("add_btn", "button", title='Add')
        self.close_btn = self.create_element("close_btn", "button", title='Quit')
        self.contacts = self.create_element("contacts", "listbox", multiple=False)
        self.contacts_label = self.create_element("contacts_label", "label", text='Contacts')
        self.delete_btn = self.create_element("delete_btn", "button", title='Delete')
        self.detail = self.create_element("detail", "label", text='Select a contact.')
        self.edit_btn = self.create_element("edit_btn", "button", title='Edit')
        self.header = self.create_element("header", "label", text='Contacts Manager')
        self.reload_btn = self.create_element("reload_btn", "button", title='Reload')
        self.save_btn = self.create_element("save_btn", "button", title='Save CSV')
        self.status = self.create_element("status", "label", text='Ready')
    def on_contacts_selection_change(self, value):
        pass

    def on_add_btn_click(self):
        pass

    def on_edit_btn_click(self):
        pass

    def on_delete_btn_click(self):
        pass

    def on_save_btn_click(self):
        pass

    def on_reload_btn_click(self):
        pass

    def on_close_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.add_btn:
            self.on_add_btn_click()
        elif element is self.edit_btn:
            self.on_edit_btn_click()
        elif element is self.delete_btn:
            self.on_delete_btn_click()
        elif element is self.save_btn:
            self.on_save_btn_click()
        elif element is self.reload_btn:
            self.on_reload_btn_click()
        elif element is self.close_btn:
            self.on_close_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.contacts:
            self.on_contacts_selection_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header": {
        "type": 'label',
        "text": 'Contacts Manager',
        "description": 'Application title.',
        "expose": False,
    },
    "contacts_label": {
        "type": 'label',
        "text": 'Contacts',
        "description": 'Label for the contacts list.',
    },
    "contacts": {
        "type": 'listbox',
        "options": [],
        "selected_items": [],
        "multiple": 'false',
        "description": 'List of contact records; selecting a row updates the detail panel.',
    },
    "detail": {
        "type": 'label',
        "text": 'Select a contact.',
        "description": 'Read-only details for the selected contact.',
    },
    "add_btn": {
        "type": 'button',
        "title": 'Add',
        "description": 'Open the contact form to create a new contact.',
    },
    "edit_btn": {
        "type": 'button',
        "title": 'Edit',
        "description": 'Open the selected contact for editing.',
    },
    "delete_btn": {
        "type": 'button',
        "title": 'Delete',
        "description": 'Delete the selected contact.',
    },
    "save_btn": {
        "type": 'button',
        "title": 'Save CSV',
        "description": 'Save contacts to the configured CSV file.',
    },
    "reload_btn": {
        "type": 'button',
        "title": 'Reload',
        "description": 'Reload contacts from the configured CSV file.',
    },
    "close_btn": {
        "type": 'button',
        "title": 'Quit',
        "width": 9,
        "description": 'Close the contacts manager.',
    },
    "status": {
        "type": 'label',
        "text": 'Ready',
        "description": 'Shows save, load, and validation status messages.',
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
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "@header": {
        "text-align": 'center',
        "background": '#1f2937',
        "color": '#ffffff',
    },
    "@list": {
        "background": '#101a2b',
    },
    "@contacts": {
        "background": '#ffffff11',
        "color": '#cbd5e1',
        "focus-background": '#ffffff22',
        "edit-background": '#00000000',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
    "@detail_cell": {
        "background": '#0b1120',
        "padding": '1, 1, 1, 1',
    },
    "@detail": {
        "color": '#d1d5db',
        "user-select": 'text',
    },
    "@delete_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "@close_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "@status": {
        "background": '#111827',
        "color": '#93c5fd',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 88,
        'cell_chars_height': 1,
        'cell_name': 'title',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 77,
        'chars_height': 1,
        'margin_right': 11,
        'height': 1,
        'type': 'label',
        'name': 'header'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 6,
        'cell_col': 0,
        'cell_chars_width': 35,
        'cell_chars_height': 10,
        'cell_name': 'list',
        'cell_width': 32,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 33,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 9,
        'height': 1,
        'type': 'label',
        'name': 'contacts_label'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 6,
        'cell_col': 36,
        'cell_chars_width': 52,
        'cell_chars_height': 10,
        'cell_name': 'detail_cell',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'fit-content',
        'chars_width': 50,
        'chars_height': 10,
        'margin_right': 1,
        'type': 'label',
        'name': 'detail'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 17,
        'cell_col': 0,
        'cell_chars_width': 88,
        'cell_chars_height': 1,
        'cell_width': 88,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 86,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'label',
        'name': 'status'
    },
    {
        'row': 0,
        'col': 77,
        'relative_col': 77,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 88,
        'cell_chars_height': 1,
        'cell_name': 'title',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'width': 11,
        'height': 1,
        'type': 'button',
        'name': 'close_btn'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 88,
        'cell_chars_height': 3,
        'cell_name': 'actions',
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 14,
        'chars_height': 1,
        'margin_right': 73,
        'margin_bottom': 1,
        'width': 14,
        'height': 1,
        'type': 'button',
        'name': 'add_btn'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 6,
        'cell_col': 0,
        'cell_chars_width': 35,
        'cell_chars_height': 10,
        'cell_name': 'list',
        'cell_width': 32,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'fit-content',
        'chars_width': 33,
        'chars_height': 8,
        'margin_right': 1,
        'margin_bottom': 1,
        'type': 'listbox',
        'name': 'contacts'
    },
    {
        'row': 1,
        'col': 16,
        'relative_row': 1,
        'relative_col': 16,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 88,
        'cell_chars_height': 3,
        'cell_name': 'actions',
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 15,
        'chars_height': 1,
        'margin_right': 57,
        'margin_bottom': 1,
        'width': 15,
        'height': 1,
        'type': 'button',
        'name': 'edit_btn'
    },
    {
        'row': 1,
        'col': 32,
        'relative_row': 1,
        'relative_col': 32,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 88,
        'cell_chars_height': 3,
        'cell_name': 'actions',
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 17,
        'chars_height': 1,
        'margin_right': 39,
        'margin_bottom': 1,
        'width': 17,
        'height': 1,
        'type': 'button',
        'name': 'delete_btn'
    },
    {
        'row': 1,
        'col': 50,
        'relative_row': 1,
        'relative_col': 50,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 88,
        'cell_chars_height': 3,
        'cell_name': 'actions',
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 15,
        'chars_height': 1,
        'margin_right': 23,
        'margin_bottom': 1,
        'width': 15,
        'height': 1,
        'type': 'button',
        'name': 'save_btn'
    },
    {
        'row': 1,
        'col': 66,
        'relative_row': 1,
        'relative_col': 66,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 88,
        'cell_chars_height': 3,
        'cell_name': 'actions',
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 17,
        'chars_height': 1,
        'margin_right': 5,
        'margin_bottom': 1,
        'width': 17,
        'height': 1,
        'type': 'button',
        'name': 'reload_btn'
    }
]
