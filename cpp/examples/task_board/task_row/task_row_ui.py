"""Auto-generated UI code for task_row_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIControl



class TaskRowUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Task Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: "Reusable task row with editable controls."\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntask_title:\n  type: label\n  text: "Task"\n  description: "Task title."\n\nmeta:\n  type: label\n  text: "Status / assignee / priority"\n  description: "Task status, assignee, and priority."\n\ndone:\n  type: checkbox\n  title: Done\n  value: false\n  description: "Whether the task is done."\n\nspacer:\n  type: label\n  text: ""\n  description: "Flexible row spacer before task actions."\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: "Open this task in the edit dialog."\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: "Ask for confirmation and delete this task."\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@card:\n  background: "#172033"\n  padding: 1, 1, 1, 1\n@task_title:\n  color: "#f8fafc"\n  user-select: text\n@meta:\n  color: "#93c5fd"\n  user-select: text\ncheckbox:\n  color: "#cbd5e1"\n  focus-color: "#facc15"\n  checked-color: "#22c55e"\n  unchecked-color: "#94a3b8"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@delete_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------+\n| task_title..**....... meta................... |\n| done.... spacer..**.. open_btn.. delete_btn.. |\n+-----------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Reusable task row with editable controls.'}, 'elements': {'task_title': {'role': 'text', 'description': 'Task title.', 'expose': True}, 'meta': {'role': 'text', 'description': 'Task status, assignee, and priority.', 'expose': True}, 'done': {'role': 'toggle', 'description': 'Whether the task is done.', 'expose': True}, 'spacer': {'role': 'text', 'description': 'Flexible row spacer before task actions.', 'expose': False}, 'open_btn': {'role': 'action', 'description': 'Open this task in the edit dialog.', 'expose': True}, 'delete_btn': {'role': 'action', 'description': 'Ask for confirmation and delete this task.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Task Row",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.delete_btn = self.create_element("delete_btn", "button", title='Delete')
        self.done = self.create_element("done", "checkbox", value=False, title='Done')
        self.meta = self.create_element("meta", "label", text='Status / assignee / priority')
        self.open_btn = self.create_element("open_btn", "button", title='Open')
        self.spacer = self.create_element("spacer", "label", text='')
        self.task_title = self.create_element("task_title", "label", text='Task')
    def on_done_change(self, value):
        pass

    def on_open_btn_click(self):
        pass

    def on_delete_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.done:
            self.on_done_change(value)
        elif element is self.open_btn:
            self.on_open_btn_click()
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
    "task_title": {
        "type": 'label',
        "text": 'Task',
        "description": 'Task title.',
    },
    "meta": {
        "type": 'label',
        "text": 'Status / assignee / priority',
        "description": 'Task status, assignee, and priority.',
    },
    "done": {
        "type": 'checkbox',
        "title": 'Done',
        "value": False,
        "description": 'Whether the task is done.',
    },
    "spacer": {
        "type": 'label',
        "text": '',
        "description": 'Flexible row spacer before task actions.',
        "expose": False,
    },
    "open_btn": {
        "type": 'button',
        "title": 'Open',
        "description": 'Open this task in the edit dialog.',
    },
    "delete_btn": {
        "type": 'button',
        "title": 'Delete',
        "description": 'Ask for confirmation and delete this task.',
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
        "color": '#cbd5e1',
        "focus-color": '#facc15',
        "checked-background": 'transparent',
        "checked-color": '#22c55e',
        "unchecked-background": 'transparent',
        "unchecked-color": '#94a3b8',
    },
    "button": {
        "background": '#334155',
        "color": '#e2e8f0',
        "focus-background": '#2563eb',
        "focus-color": '#ffffff',
    },
    "@card": {
        "background": '#172033',
        "padding": '1, 1, 1, 1',
    },
    "@task_title": {
        "color": '#f8fafc',
        "user-select": 'text',
    },
    "@meta": {
        "color": '#93c5fd',
        "user-select": 'text',
    },
    "@delete_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 47,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 21,
        'chars_height': 1,
        'margin_right': 25,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'task_title'
    },
    {
        'row': 0,
        'col': 23,
        'relative_col': 23,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 47,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 23,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 1,
        'width': 23,
        'height': 1,
        'type': 'label',
        'name': 'meta'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 47,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 8,
        'chars_height': 1,
        'margin_right': 38,
        'width': 8,
        'height': 1,
        'type': 'checkbox',
        'name': 'done'
    },
    {
        'row': 1,
        'col': 10,
        'relative_row': 1,
        'relative_col': 10,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 47,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 25,
        'height': 1,
        'type': 'label',
        'name': 'spacer'
    },
    {
        'row': 1,
        'col': 23,
        'relative_row': 1,
        'relative_col': 23,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 47,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 14,
        'width': 10,
        'height': 1,
        'type': 'button',
        'name': 'open_btn'
    },
    {
        'row': 1,
        'col': 34,
        'relative_row': 1,
        'relative_col': 34,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 47,
        'cell_chars_height': 2,
        'cell_name': 'card',
        'cell_height': 2,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 1,
        'width': 12,
        'height': 1,
        'type': 'button',
        'name': 'delete_btn'
    }
]
