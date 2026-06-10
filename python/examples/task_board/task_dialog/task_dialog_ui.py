"""Auto-generated UI code for task_dialog_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow



class TaskDialogUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Task Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: draft\ndescription: "Dialog form for creating or editing a task."\ntags: [example, dialog]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Task\n  description: "Dialog title."\n  expose: false\n\nname_label:\n  type: label\n  text: Title\n  description: "Label for task title."\n  expose: false\n\nname:\n  type: textinput\n  value: ""\n  maxlength: 60\n  description: "Required task title."\n\nstatus_label:\n  type: label\n  text: Status\n  description: "Label for task status."\n  expose: false\n\nproject_status:\n  type: combobox\n  options: [Todo, Doing, Blocked, Done]\n  selected_item: Todo\n  description: "Task status."\n\nowner_label:\n  type: label\n  text: Assignee\n  description: "Label for task assignee."\n  expose: false\n\nowner:\n  type: combobox\n  options: [Ava, Bruno, Chen, Dana]\n  selected_item: Ava\n  description: "Task assignee."\n\npriority_label:\n  type: label\n  text: Priority\n  description: "Label for task priority."\n  expose: false\n\npriority:\n  type: combobox\n  options: [Low, Med, High, Urgent]\n  selected_item: Med\n  description: "Task priority."\n\nnote_label:\n  type: label\n  text: Description\n  description: "Label for task description."\n  expose: false\n\nnote:\n  type: textarea\n  value: ""\n  maxlength: 160\n  description: "Task description."\n\nmessage:\n  type: label\n  text: ""\n  description: "Validation message."\n\nadd_btn:\n  type: button\n  title: Save task\n  description: "Save the task and close the dialog."\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: "Close the dialog without saving the task."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: "#1f2937"\n  color: "#ffffff"\ntextinput:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ntextarea:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ncombobox:\n  background: "#0f172a"\n  color: "#e5e7eb"\n  focus-background: "#1e293b"\n  edit-background: "#111827"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@add_btn:\n  background: "#14532d"\n  focus-background: "#16a34a"\n@message:\n  color: "#fbbf24"\n```\n\n## User Interface\n\n```ui\n+------------------------------------------------------+\n|header................................................|\n+------------------------------------------------------+\n|                                                      |\n|  name_label      name.............................   |\n|                                                      |\n|  status_label    project_status...................   |\n|                                                      |\n|  owner_label     owner............................   |\n|                                                      |\n|  priority_label  priority.........................   |\n|                                                      |\n|  note_label.     note.............................   |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                                                      |\n|  message..........................................   |\n|                                                      |\n+------------------------------------------------------+\n|  add_btn................  cancel_btn..............   |\n|                                                      |\n+------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Dialog form for creating or editing a task.'}, 'elements': {'header': {'role': 'text', 'description': 'Dialog title.', 'expose': False}, 'name_label': {'role': 'text', 'description': 'Label for task title.', 'expose': False}, 'name': {'role': 'input', 'description': 'Required task title.', 'expose': True}, 'status_label': {'role': 'text', 'description': 'Label for task status.', 'expose': False}, 'project_status': {'role': 'selection', 'description': 'Task status.', 'expose': True}, 'owner_label': {'role': 'text', 'description': 'Label for task assignee.', 'expose': False}, 'owner': {'role': 'selection', 'description': 'Task assignee.', 'expose': True}, 'priority_label': {'role': 'text', 'description': 'Label for task priority.', 'expose': False}, 'priority': {'role': 'selection', 'description': 'Task priority.', 'expose': True}, 'note_label': {'role': 'text', 'description': 'Label for task description.', 'expose': False}, 'note': {'role': 'input', 'description': 'Task description.', 'expose': True}, 'message': {'role': 'text', 'description': 'Validation message.', 'expose': True}, 'add_btn': {'role': 'action', 'description': 'Save the task and close the dialog.', 'expose': True}, 'cancel_btn': {'role': 'action', 'description': 'Close the dialog without saving the task.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Task Dialog",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'dialog'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.add_btn = self.create_element("add_btn", "button", title='Save task')
        self.cancel_btn = self.create_element("cancel_btn", "button", title='Cancel')
        self.header = self.create_element("header", "label", text='Task')
        self.message = self.create_element("message", "label", text='')
        self.name = self.create_element("name", "textinput", value='', maxlength=60)
        self.name_label = self.create_element("name_label", "label", text='Title')
        self.note = self.create_element("note", "textarea", value='', maxlength=160)
        self.note_label = self.create_element("note_label", "label", text='Description')
        self.owner = self.create_element("owner", "combobox", options=['Ava', 'Bruno', 'Chen', 'Dana'], selected_item='Ava')
        self.owner_label = self.create_element("owner_label", "label", text='Assignee')
        self.priority = self.create_element("priority", "combobox", options=['Low', 'Med', 'High', 'Urgent'], selected_item='Med')
        self.priority_label = self.create_element("priority_label", "label", text='Priority')
        self.project_status = self.create_element("project_status", "combobox", options=['Todo', 'Doing', 'Blocked', 'Done'], selected_item='Todo')
        self.status_label = self.create_element("status_label", "label", text='Status')
    def on_name_change(self, value):
        pass

    def on_name_submit(self, value):
        pass

    def on_project_status_change(self, value):
        pass

    def on_owner_change(self, value):
        pass

    def on_priority_change(self, value):
        pass

    def on_note_change(self, value):
        pass

    def on_note_submit(self, value):
        pass

    def on_add_btn_click(self):
        pass

    def on_cancel_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.name:
            self.on_name_change(value)
        elif element is self.note:
            self.on_note_change(value)
        elif element is self.add_btn:
            self.on_add_btn_click()
        elif element is self.cancel_btn:
            self.on_cancel_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.name:
            self.on_name_submit(value)
        elif element is self.note:
            self.on_note_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.project_status:
            self.on_project_status_change(value)
        elif element is self.owner:
            self.on_owner_change(value)
        elif element is self.priority:
            self.on_priority_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header": {
        "type": 'label',
        "text": 'Task',
        "description": 'Dialog title.',
        "expose": False,
    },
    "name_label": {
        "type": 'label',
        "text": 'Title',
        "description": 'Label for task title.',
        "expose": False,
    },
    "name": {
        "type": 'textinput',
        "value": '',
        "maxlength": 60,
        "description": 'Required task title.',
    },
    "status_label": {
        "type": 'label',
        "text": 'Status',
        "description": 'Label for task status.',
        "expose": False,
    },
    "project_status": {
        "type": 'combobox',
        "options": ['Todo', 'Doing', 'Blocked', 'Done'],
        "selected_item": 'Todo',
        "description": 'Task status.',
    },
    "owner_label": {
        "type": 'label',
        "text": 'Assignee',
        "description": 'Label for task assignee.',
        "expose": False,
    },
    "owner": {
        "type": 'combobox',
        "options": ['Ava', 'Bruno', 'Chen', 'Dana'],
        "selected_item": 'Ava',
        "description": 'Task assignee.',
    },
    "priority_label": {
        "type": 'label',
        "text": 'Priority',
        "description": 'Label for task priority.',
        "expose": False,
    },
    "priority": {
        "type": 'combobox',
        "options": ['Low', 'Med', 'High', 'Urgent'],
        "selected_item": 'Med',
        "description": 'Task priority.',
    },
    "note_label": {
        "type": 'label',
        "text": 'Description',
        "description": 'Label for task description.',
        "expose": False,
    },
    "note": {
        "type": 'textarea',
        "value": '',
        "maxlength": 160,
        "description": 'Task description.',
    },
    "message": {
        "type": 'label',
        "text": '',
        "description": 'Validation message.',
    },
    "add_btn": {
        "type": 'button',
        "title": 'Save task',
        "description": 'Save the task and close the dialog.',
    },
    "cancel_btn": {
        "type": 'button',
        "title": 'Cancel',
        "description": 'Close the dialog without saving the task.',
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
        "background": '#0f172a',
        "color": '#e5e7eb',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
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
    "textarea": {
        "background": '#0f172a',
        "color": '#e5e7eb',
        "focus-background": '#1e293b',
        "edit-background": '#111827',
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
    },
    "@add_btn": {
        "background": '#14532d',
        "focus-background": '#16a34a',
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
        'cell_chars_width': 54,
        'cell_chars_height': 1,
        'cell_width': 54,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 54,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': 'header'
    },
    {
        'row': 0,
        'col': 2,
        'relative_col': 2,
        'cell_row': 20,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 2,
        'cell_width': 54,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 23,
        'chars_height': 1,
        'margin_right': 29,
        'margin_bottom': 1,
        'width': 23,
        'height': 1,
        'type': 'button',
        'name': 'add_btn'
    },
    {
        'row': 0,
        'col': 27,
        'relative_col': 27,
        'cell_row': 20,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 2,
        'cell_width': 54,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 24,
        'chars_height': 1,
        'margin_right': 3,
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
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 42,
        'margin_bottom': 15,
        'width': 10,
        'height': 1,
        'type': 'label',
        'name': 'name_label'
    },
    {
        'row': 1,
        'col': 18,
        'relative_row': 1,
        'relative_col': 18,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 33,
        'chars_height': 1,
        'margin_right': 3,
        'margin_bottom': 15,
        'height': 1,
        'type': 'textinput',
        'name': 'name'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 40,
        'margin_bottom': 13,
        'width': 12,
        'height': 1,
        'type': 'label',
        'name': 'status_label'
    },
    {
        'row': 3,
        'col': 18,
        'relative_row': 3,
        'relative_col': 18,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 33,
        'chars_height': 1,
        'margin_right': 3,
        'margin_bottom': 13,
        'height': 1,
        'type': 'combobox',
        'name': 'project_status'
    },
    {
        'row': 5,
        'col': 2,
        'relative_row': 5,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 41,
        'margin_bottom': 11,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': 'owner_label'
    },
    {
        'row': 5,
        'col': 18,
        'relative_row': 5,
        'relative_col': 18,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 33,
        'chars_height': 1,
        'margin_right': 3,
        'margin_bottom': 11,
        'height': 1,
        'type': 'combobox',
        'name': 'owner'
    },
    {
        'row': 7,
        'col': 2,
        'relative_row': 7,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 14,
        'chars_height': 1,
        'margin_right': 38,
        'margin_bottom': 9,
        'width': 14,
        'height': 1,
        'type': 'label',
        'name': 'priority_label'
    },
    {
        'row': 7,
        'col': 18,
        'relative_row': 7,
        'relative_col': 18,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 33,
        'chars_height': 1,
        'margin_right': 3,
        'margin_bottom': 9,
        'height': 1,
        'type': 'combobox',
        'name': 'priority'
    },
    {
        'row': 9,
        'col': 2,
        'relative_row': 9,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 41,
        'margin_bottom': 7,
        'width': 11,
        'height': 1,
        'type': 'label',
        'name': 'note_label'
    },
    {
        'row': 9,
        'col': 18,
        'relative_row': 9,
        'relative_col': 18,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 33,
        'chars_height': 5,
        'margin_right': 3,
        'margin_bottom': 3,
        'height': 5,
        'type': 'textarea',
        'name': 'note'
    },
    {
        'row': 15,
        'col': 2,
        'relative_row': 15,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 17,
        'cell_width': 54,
        'cell_height': 17,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 49,
        'chars_height': 1,
        'margin_right': 3,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'message'
    }
]
