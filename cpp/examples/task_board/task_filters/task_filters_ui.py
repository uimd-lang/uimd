"""Auto-generated UI code for task_filters_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIControl



class TaskFiltersUI(UIControl):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Task Filters\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: draft\ndescription: "Reusable task board filter panel."\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfilters_label:\n  type: label\n  text: FILTERS\n  description: "Filter panel label."\n  expose: false\n\nsearch_label:\n  type: label\n  text: Search\n  description: "Search filter label."\n  expose: false\n\nstatus_label:\n  type: label\n  text: Status\n  description: "Status filter label."\n  expose: false\n\nowner_label:\n  type: label\n  text: Assignee\n  description: "Assignee filter label."\n  expose: false\n\nsearch:\n  type: textinput\n  value: ""\n  maxlength: 60\n  placeholder: "Task title or description"\n  description: "Filter tasks by title or description."\n\nstatus_filter:\n  type: combobox\n  options: [Any, Todo, Doing, Blocked, Done]\n  selected_item: Any\n  description: "Filter tasks by status."\n\nowner_filter:\n  type: combobox\n  options: [Any, Ava, Bruno, Chen, Dana]\n  selected_item: Any\n  description: "Filter tasks by assignee."\n\napply_filters_btn:\n  type: button\n  title: "Apply filter"\n  description: "Apply the current task filters."\n\nreset_filters_btn:\n  type: button\n  title: Reset\n  description: "Reset search, status, and assignee filters."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  background: "#15171c"\n  border-width: 0\nlabel:\n  color: "#f5f5f5"\ntextinput:\n  background: "#252b35"\n  color: "#ffffff"\n  focus-background: "#303a49"\n  edit-background: "#344459"\n  cursor-background: "#93c5fd"\n  cursor-color: "#ffffff"\ncombobox:\n  background: "#252b35"\n  color: "#ffffff"\n  focus-background: "#303a49"\n  edit-background: "#344459"\n  selected-background: "#2f80ed"\n  selected-color: "#ffffff"\nbutton:\n  background: "#2f3545"\n  color: "#f5f5f5"\n  focus-background: "#2f80ed"\n@filters_label:\n  color: "#aab2c3"\n@search_label:\n  color: "#cfd5e3"\n@status_label:\n  color: "#cfd5e3"\n@owner_label:\n  color: "#cfd5e3"\n@apply_filters_btn:\n  background: "#2f80ed"\n  focus-background: "#1d6fd8"\n@reset_filters_btn:\n  background: "#343b4b"\n```\n\n## User Interface\n\n```ui\n+-**---------------------------------------------------------------------------+\n|filters_label..**.............................................................|\n+----------------------------+----------------------+--------------------------+\n|search_label..**........... | status_label..**.... | owner_label..**......... |\n|search..**................. | status_filter.**.... | owner_filter..**........ |\n+-**-------------------------+----------------------+--------------------------+\n|                                                                              |\n|apply_filters_btn.....  reset_filters_btn.....                                |\n|                                                                              |\n+------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Reusable task board filter panel.'}, 'elements': {'filters_label': {'role': 'text', 'description': 'Filter panel label.', 'expose': False}, 'search_label': {'role': 'text', 'description': 'Search filter label.', 'expose': False}, 'status_label': {'role': 'text', 'description': 'Status filter label.', 'expose': False}, 'owner_label': {'role': 'text', 'description': 'Assignee filter label.', 'expose': False}, 'search': {'role': 'input', 'description': 'Filter tasks by title or description.', 'expose': True}, 'status_filter': {'role': 'selection', 'description': 'Filter tasks by status.', 'expose': True}, 'owner_filter': {'role': 'selection', 'description': 'Filter tasks by assignee.', 'expose': True}, 'apply_filters_btn': {'role': 'action', 'description': 'Apply the current task filters.', 'expose': True}, 'reset_filters_btn': {'role': 'action', 'description': 'Reset search, status, and assignee filters.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Task Filters",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'control'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.apply_filters_btn = self.create_element("apply_filters_btn", "button", title='Apply filter')
        self.filters_label = self.create_element("filters_label", "label", text='FILTERS')
        self.owner_filter = self.create_element("owner_filter", "combobox", options=['Any', 'Ava', 'Bruno', 'Chen', 'Dana'], selected_item='Any')
        self.owner_label = self.create_element("owner_label", "label", text='Assignee')
        self.reset_filters_btn = self.create_element("reset_filters_btn", "button", title='Reset')
        self.search = self.create_element("search", "textinput", value='', maxlength=60)
        self.search_label = self.create_element("search_label", "label", text='Search')
        self.status_filter = self.create_element("status_filter", "combobox", options=['Any', 'Todo', 'Doing', 'Blocked', 'Done'], selected_item='Any')
        self.status_label = self.create_element("status_label", "label", text='Status')
    def on_search_change(self, value):
        pass

    def on_search_submit(self, value):
        pass

    def on_status_filter_change(self, value):
        pass

    def on_owner_filter_change(self, value):
        pass

    def on_apply_filters_btn_click(self):
        pass

    def on_reset_filters_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.search:
            self.on_search_change(value)
        elif element is self.apply_filters_btn:
            self.on_apply_filters_btn_click()
        elif element is self.reset_filters_btn:
            self.on_reset_filters_btn_click()
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
    "filters_label": {
        "type": 'label',
        "text": 'FILTERS',
        "description": 'Filter panel label.',
        "expose": False,
    },
    "search_label": {
        "type": 'label',
        "text": 'Search',
        "description": 'Search filter label.',
        "expose": False,
    },
    "status_label": {
        "type": 'label',
        "text": 'Status',
        "description": 'Status filter label.',
        "expose": False,
    },
    "owner_label": {
        "type": 'label',
        "text": 'Assignee',
        "description": 'Assignee filter label.',
        "expose": False,
    },
    "search": {
        "type": 'textinput',
        "value": '',
        "maxlength": 60,
        "placeholder": 'Task title or description',
        "description": 'Filter tasks by title or description.',
    },
    "status_filter": {
        "type": 'combobox',
        "options": ['Any', 'Todo', 'Doing', 'Blocked', 'Done'],
        "selected_item": 'Any',
        "description": 'Filter tasks by status.',
    },
    "owner_filter": {
        "type": 'combobox',
        "options": ['Any', 'Ava', 'Bruno', 'Chen', 'Dana'],
        "selected_item": 'Any',
        "description": 'Filter tasks by assignee.',
    },
    "apply_filters_btn": {
        "type": 'button',
        "title": 'Apply filter',
        "description": 'Apply the current task filters.',
    },
    "reset_filters_btn": {
        "type": 'button',
        "title": 'Reset',
        "description": 'Reset search, status, and assignee filters.',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#15171c',
        "border-color": 'transparent',
        "border-width": 0,
    },
    "label": {
        "background": 'transparent',
        "color": '#f5f5f5',
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
        "background": '#252b35',
        "color": '#ffffff',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
        "focus-background": '#303a49',
        "focus-color": '#ffffff',
        "edit-background": '#344459',
        "edit-color": '#ffffff',
        "cursor-background": '#93c5fd',
        "cursor-color": '#ffffff',
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
        "background": '#252b35',
        "color": '#ffffff',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
        "focus-background": '#303a49',
        "focus-color": '#ffffff',
        "edit-background": '#344459',
        "edit-color": '#e5e7eb',
        "selected-background": '#2f80ed',
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
        "background": '#2f3545',
        "color": '#f5f5f5',
        "disabled-background": '#1f2937',
        "disabled-color": '#64748b',
        "focus-background": '#2f80ed',
        "focus-color": '#ffffff',
    },
    "@filters_label": {
        "color": '#aab2c3',
    },
    "@search_label": {
        "color": '#cfd5e3',
    },
    "@status_label": {
        "color": '#cfd5e3',
    },
    "@owner_label": {
        "color": '#cfd5e3',
    },
    "@apply_filters_btn": {
        "background": '#2f80ed',
        "focus-background": '#1d6fd8',
    },
    "@reset_filters_btn": {
        "background": '#343b4b',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 78,
        'chars_height': 1,
        'height': 1,
        'type': 'label',
        'name': 'filters_label'
    },
    {
        'row': 0,
        'col': 0,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 2,
        'cell_width': 28,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 27,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'search_label'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 52,
        'cell_chars_width': 26,
        'cell_chars_height': 2,
        'cell_width': 26,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 24,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'owner_label'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 29,
        'cell_chars_width': 22,
        'cell_chars_height': 2,
        'cell_width': 22,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'status_label'
    },
    {
        'row': 1,
        'col': 0,
        'relative_row': 1,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 3,
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 22,
        'chars_height': 1,
        'margin_right': 56,
        'margin_bottom': 1,
        'width': 22,
        'height': 1,
        'type': 'button',
        'name': 'apply_filters_btn'
    },
    {
        'row': 1,
        'col': 0,
        'relative_row': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 2,
        'cell_width': 28,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 27,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'textinput',
        'name': 'search'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 52,
        'cell_chars_width': 26,
        'cell_chars_height': 2,
        'cell_width': 26,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 24,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'combobox',
        'name': 'owner_filter'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 29,
        'cell_chars_width': 22,
        'cell_chars_height': 2,
        'cell_width': 22,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'combobox',
        'name': 'status_filter'
    },
    {
        'row': 1,
        'col': 24,
        'relative_row': 1,
        'relative_col': 24,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 3,
        'cell_height': 3,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 22,
        'chars_height': 1,
        'margin_right': 32,
        'margin_bottom': 1,
        'width': 22,
        'height': 1,
        'type': 'button',
        'name': 'reset_filters_btn'
    }
]
