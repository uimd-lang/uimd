"""Auto-generated UI code for task_board_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'task_list'))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'task_dialog'))
from task_list import TaskList


class TaskBoardUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Task Board\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools."\ntags: [example, mcp]\n```\n\n## Definition\n\n```yaml\nuses:\n  - task_list/task_list\n  - task_dialog/task_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: "Task Board"\n  description: "Application title."\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: "Close the task list example."\n\nsearch_label:\n  type: label\n  text: Search\n  description: "Search filter label."\n  expose: false\n\nsearch:\n  type: textinput\n  value: ""\n  maxlength: 60\n  description: "Filter tasks by title or description."\n\nstatus_label:\n  type: label\n  text: Filter status\n  description: "Status filter label."\n  expose: false\n\nstatus_filter:\n  type: combobox\n  options: [Any, Todo, Doing, Blocked, Done]\n  selected_item: Any\n  description: "Filter tasks by status."\n\nowner_label:\n  type: label\n  text: Filter assignee\n  description: "Assignee filter label."\n  expose: false\n\nowner_filter:\n  type: combobox\n  options: [Any, Ava, Bruno, Chen, Dana]\n  selected_item: Any\n  description: "Filter tasks by assignee."\n\nreset_filters_btn:\n  type: button\n  title: Reset filters\n  description: "Reset search, status, and assignee filters."\n\nnew_task_btn:\n  type: button\n  title: New task\n  description: "Open a dialog for creating a task."\n\nboard:\n  type: task_list\n  cpp-class: task_board_example::TaskList\n  cpp-header: "task_list/task_list.hpp"\n  description: "Scrollable list of interactive task rows."\n\nstatus:\n  type: label\n  text: Ready\n  description: "Current board status."\n```\n\n## Tools\n\n```yaml\nquery_tasks:\n  description: "Return tasks matching optional title, status, assignee, priority, and done filters."\n  input:\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    done: bool\n  output:\n    tasks:\n      - id: string\n        title: string\n        status: string\n        assignee: string\n        priority: string\n        description: string\n        done: bool\n\nget_task:\n  description: "Return one task by id."\n  input:\n    id: string\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\nupdate_task:\n  description: "Update a task by id and return the updated task."\n  input:\n    id: string\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    description: string\n    done: bool\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\ndelete_task:\n  description: "Delete a task by id and return whether it was removed."\n  input:\n    id: string\n    required: [id]\n  output:\n    deleted: bool\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: "#1f2937"\n@title:\n  color: "#ffffff"\n  text-align: center\n@filters:\n  background: "#172033"\n  padding: 1, 1, 1, 1\n@board_cell:\n  background: "#030712"\n@board:\n  background: "#030712"\n  focus-background: "#07111f"\n  edit-background: "#0f172a"\n  scope-dim-background: "#0000004d"\ntextinput:\n  background: "#0b1220"\n  color: "#e5e7eb"\n  focus-background: "#25364f"\n  edit-background: "#0b1220"\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ncombobox:\n  background: "#0b1220"\n  color: "#e5e7eb"\n  focus-background: "#25364f"\n  edit-background: "#0b1220"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@quit_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n@reset_filters_btn:\n  background: "#711212"\n  focus-background: "#b91c1c"\n@new_task_btn:\n  background: "#14532d"\n  focus-background: "#16a34a"\n@status:\n  background: "#111827"\n  color: "#93c5fd"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**................ ..................................... quit_btn.... |\n+-filters-24----------------+-board_cell-**------------------------------------+\n| search_label..**......... | board..**....................................... |\n| search..**............... | ................................................ |\n*                           | *............................................... |\n* status_label..**......... | *............................................... |\n| status_filter............ | ................................................ |\n|                           | ................................................ |\n| owner_label..**.......... | ................................................ |\n| owner_filter............. | ................................................ |\n|                           | ................................................ |\n| reset_filters_btn..**.... | ................................................ |\n|                           | ................................................ |\n| new_task_btn..**......... | ................................................ |\n|                           | ................................................ |\n| status..**............... | ................................................ |\n| ......................... | ................................................ |\n| ......................... | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n+---------------------------+--------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.'}, 'elements': {'title': {'role': 'text', 'description': 'Application title.', 'expose': False}, 'quit_btn': {'role': 'action', 'description': 'Close the task list example.', 'expose': True}, 'search_label': {'role': 'text', 'description': 'Search filter label.', 'expose': False}, 'search': {'role': 'input', 'description': 'Filter tasks by title or description.', 'expose': True}, 'status_label': {'role': 'text', 'description': 'Status filter label.', 'expose': False}, 'status_filter': {'role': 'selection', 'description': 'Filter tasks by status.', 'expose': True}, 'owner_label': {'role': 'text', 'description': 'Assignee filter label.', 'expose': False}, 'owner_filter': {'role': 'selection', 'description': 'Filter tasks by assignee.', 'expose': True}, 'reset_filters_btn': {'role': 'action', 'description': 'Reset search, status, and assignee filters.', 'expose': True}, 'new_task_btn': {'role': 'action', 'description': 'Open a dialog for creating a task.', 'expose': True}, 'board': {'role': 'text', 'description': 'Scrollable list of interactive task rows.', 'expose': True}, 'status': {'role': 'text', 'description': 'Current board status.', 'expose': True}}}
    _mcp_app_tools = {'query_tasks': {'name': 'query_tasks', 'description': 'Return tasks matching optional title, status, assignee, priority, and done filters.', 'inputSchema': {'type': 'object', 'properties': {'title': {'type': 'string'}, 'status': {'type': 'string'}, 'assignee': {'type': 'string'}, 'priority': {'type': 'string'}, 'done': {'type': 'boolean'}}}, 'outputSchema': {'type': 'object', 'properties': {'tasks': {'type': 'array', 'items': {'type': 'object', 'properties': {'id': {'type': 'string'}, 'title': {'type': 'string'}, 'status': {'type': 'string'}, 'assignee': {'type': 'string'}, 'priority': {'type': 'string'}, 'description': {'type': 'string'}, 'done': {'type': 'boolean'}}}}}}}, 'get_task': {'name': 'get_task', 'description': 'Return one task by id.', 'inputSchema': {'type': 'object', 'properties': {'id': {'type': 'string'}}, 'required': ['id']}, 'outputSchema': {'type': 'object', 'properties': {'task': {'type': 'object', 'properties': {'id': {'type': 'string'}, 'title': {'type': 'string'}, 'status': {'type': 'string'}, 'assignee': {'type': 'string'}, 'priority': {'type': 'string'}, 'description': {'type': 'string'}, 'done': {'type': 'boolean'}}}}}}, 'update_task': {'name': 'update_task', 'description': 'Update a task by id and return the updated task.', 'inputSchema': {'type': 'object', 'properties': {'id': {'type': 'string'}, 'title': {'type': 'string'}, 'status': {'type': 'string'}, 'assignee': {'type': 'string'}, 'priority': {'type': 'string'}, 'description': {'type': 'string'}, 'done': {'type': 'boolean'}}, 'required': ['id']}, 'outputSchema': {'type': 'object', 'properties': {'task': {'type': 'object', 'properties': {'id': {'type': 'string'}, 'title': {'type': 'string'}, 'status': {'type': 'string'}, 'assignee': {'type': 'string'}, 'priority': {'type': 'string'}, 'description': {'type': 'string'}, 'done': {'type': 'boolean'}}}}}}, 'delete_task': {'name': 'delete_task', 'description': 'Delete a task by id and return whether it was removed.', 'inputSchema': {'type': 'object', 'properties': {'id': {'type': 'string'}}, 'required': ['id']}, 'outputSchema': {'type': 'object', 'properties': {'deleted': {'type': 'boolean'}}}}}

    def __init__(self):
        super().__init__(
            title="Task Board",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.board = self.create_element("board", "uielement", layout_class=TaskList)
        self.new_task_btn = self.create_element("new_task_btn", "button", title='New task')
        self.owner_filter = self.create_element("owner_filter", "combobox", options=['Any', 'Ava', 'Bruno', 'Chen', 'Dana'], selected_item='Any')
        self.owner_label = self.create_element("owner_label", "label", text='Filter assignee')
        self.quit_btn = self.create_element("quit_btn", "button", title='Quit')
        self.reset_filters_btn = self.create_element("reset_filters_btn", "button", title='Reset filters')
        self.search = self.create_element("search", "textinput", value='', maxlength=60)
        self.search_label = self.create_element("search_label", "label", text='Search')
        self.status = self.create_element("status", "label", text='Ready')
        self.status_filter = self.create_element("status_filter", "combobox", options=['Any', 'Todo', 'Doing', 'Blocked', 'Done'], selected_item='Any')
        self.status_label = self.create_element("status_label", "label", text='Filter status')
        self.title_element = self.create_element("title", "label", text='Task Board')
    def on_quit_btn_click(self):
        pass

    def on_search_change(self, value):
        pass

    def on_search_submit(self, value):
        pass

    def on_status_filter_change(self, value):
        pass

    def on_owner_filter_change(self, value):
        pass

    def on_reset_filters_btn_click(self):
        pass

    def on_new_task_btn_click(self):
        pass

    def delete_task(self, input=None):
        raise NotImplementedError('App tool not implemented: delete_task')

    def get_task(self, input=None):
        raise NotImplementedError('App tool not implemented: get_task')

    def query_tasks(self, input=None):
        raise NotImplementedError('App tool not implemented: query_tasks')

    def update_task(self, input=None):
        raise NotImplementedError('App tool not implemented: update_task')

    def _dispatch_element_changed(self, element, value):
        if element is self.quit_btn:
            self.on_quit_btn_click()
        elif element is self.search:
            self.on_search_change(value)
        elif element is self.reset_filters_btn:
            self.on_reset_filters_btn_click()
        elif element is self.new_task_btn:
            self.on_new_task_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.search:
            self.on_search_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.status_filter:
            self.on_status_filter_change(value)
        elif element is self.owner_filter:
            self.on_owner_filter_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "title": {
        "type": 'label',
        "text": 'Task Board',
        "description": 'Application title.',
        "expose": False,
    },
    "quit_btn": {
        "type": 'button',
        "title": 'Quit',
        "description": 'Close the task list example.',
    },
    "search_label": {
        "type": 'label',
        "text": 'Search',
        "description": 'Search filter label.',
        "expose": False,
    },
    "search": {
        "type": 'textinput',
        "value": '',
        "maxlength": 60,
        "description": 'Filter tasks by title or description.',
    },
    "status_label": {
        "type": 'label',
        "text": 'Filter status',
        "description": 'Status filter label.',
        "expose": False,
    },
    "status_filter": {
        "type": 'combobox',
        "options": ['Any', 'Todo', 'Doing', 'Blocked', 'Done'],
        "selected_item": 'Any',
        "description": 'Filter tasks by status.',
    },
    "owner_label": {
        "type": 'label',
        "text": 'Filter assignee',
        "description": 'Assignee filter label.',
        "expose": False,
    },
    "owner_filter": {
        "type": 'combobox',
        "options": ['Any', 'Ava', 'Bruno', 'Chen', 'Dana'],
        "selected_item": 'Any',
        "description": 'Filter tasks by assignee.',
    },
    "reset_filters_btn": {
        "type": 'button',
        "title": 'Reset filters',
        "description": 'Reset search, status, and assignee filters.',
    },
    "new_task_btn": {
        "type": 'button',
        "title": 'New task',
        "description": 'Open a dialog for creating a task.',
    },
    "board": {
        "type": 'task_list',
        "cpp-class": 'task_board_example::TaskList',
        "cpp-header": 'task_list/task_list.hpp',
        "description": 'Scrollable list of interactive task rows.',
    },
    "status": {
        "type": 'label',
        "text": 'Ready',
        "description": 'Current board status.',
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
        "background": '#0b1220',
        "color": '#e5e7eb',
        "focus-background": '#25364f',
        "focus-color": '#ffffff',
        "edit-background": '#0b1220',
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
        "background": '#0b1220',
        "color": '#e5e7eb',
        "focus-background": '#25364f',
        "focus-color": '#ffffff',
        "edit-background": '#0b1220',
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
        "background": '#1f2937',
    },
    "@title": {
        "color": '#ffffff',
        "text-align": 'center',
    },
    "@filters": {
        "background": '#172033',
        "padding": '1, 1, 1, 1',
    },
    "@board_cell": {
        "background": '#030712',
    },
    "@board": {
        "background": '#030712',
        "focus-background": '#07111f',
        "edit-background": '#0f172a',
        "scope-dim-background": '#0000004d',
    },
    "@quit_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "@reset_filters_btn": {
        "background": '#711212',
        "focus-background": '#b91c1c',
    },
    "@new_task_btn": {
        "background": '#14532d',
        "focus-background": '#16a34a',
    },
    "@status": {
        "background": '#111827',
        "color": '#93c5fd',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 28,
        'cell_chars_width': 50,
        'cell_chars_height': 19,
        'cell_name': 'board_cell',
        'cell_height': 19,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 48,
        'chars_height': 19,
        'margin_right': 1,
        'type': 'task_list',
        'name': 'board'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 18,
        'height': 1,
        'type': 'label',
        'name': 'search_label'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 52,
        'height': 1,
        'type': 'label',
        'name': 'title'
    },
    {
        'row': 0,
        'col': 65,
        'relative_col': 65,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 1,
        'width': 12,
        'height': 1,
        'type': 'button',
        'name': 'quit_btn'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 17,
        'height': 1,
        'type': 'textinput',
        'name': 'search'
    },
    {
        'row': 3,
        'col': 1,
        'relative_row': 3,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 15,
        'height': 1,
        'type': 'label',
        'name': 'status_label'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 14,
        'height': 1,
        'type': 'combobox',
        'name': 'status_filter'
    },
    {
        'row': 6,
        'col': 1,
        'relative_row': 6,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 12,
        'height': 1,
        'type': 'label',
        'name': 'owner_label'
    },
    {
        'row': 7,
        'col': 1,
        'relative_row': 7,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 11,
        'height': 1,
        'type': 'combobox',
        'name': 'owner_filter'
    },
    {
        'row': 9,
        'col': 1,
        'relative_row': 9,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 9,
        'height': 1,
        'type': 'button',
        'name': 'reset_filters_btn'
    },
    {
        'row': 11,
        'col': 1,
        'relative_row': 11,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 7,
        'height': 1,
        'type': 'button',
        'name': 'new_task_btn'
    },
    {
        'row': 13,
        'col': 1,
        'relative_row': 13,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 19,
        'cell_name': 'filters',
        'cell_width': 24,
        'cell_width_mode': 'fixed',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 3,
        'margin_right': 1,
        'margin_bottom': 3,
        'height': 3,
        'type': 'label',
        'name': 'status'
    }
]
