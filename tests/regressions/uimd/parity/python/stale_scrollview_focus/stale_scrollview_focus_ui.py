"""Auto-generated UI code for stale_scrollview_focus_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow



class StaleScrollviewFocusUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Stale ScrollView Focus\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: "Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button."\ntags: [regression, issue-5, viewhost, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - list_page\n  - edit_page\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: "Scroll Focus Repro"\n  description: "Regression window title."\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: "Close the regression app."\n\ntab_list:\n  type: button\n  title: List\n  description: "Show the list page."\n\ntab_other:\n  type: button\n  title: Other\n  description: "Inactive top-level navigation button used to expose stale focus."\n\nhint:\n  type: label\n  text: "Focus list, press Enter, choose Open."\n  description: "Manual reproduction hint."\n  expose: false\n\npage:\n  type: viewhost\n  description: "Dynamic page host."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: "#1f2937"\n@title:\n  color: "#ffffff"\n  text-align: center\n@nav:\n  background: "#172033"\n@content:\n  background: "#030712"\n@hint:\n  color: "#93c5fd"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@tab_list:\n  background: "#2f80ed"\n  focus-background: "#22c55e"\n@tab_other:\n  background: "#343b4b"\n  focus-background: "#9333ea"\n@quit_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\nviewhost:\n  background: "#030712"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------------------+\n| title..**.................................................................. quit_btn.... |\n+-nav-**-----------------------------------------------------------------------------------+\n| tab_list........ tab_other........ hint..**............................................. |\n+-content-**-------------------------------------------------------------------------------+\n| page..**................................................................................ |\n* *....................................................................................... |\n* *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+------------------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button.'}, 'elements': {'title': {'role': 'text', 'description': 'Regression window title.', 'expose': False}, 'quit_btn': {'role': 'action', 'description': 'Close the regression app.', 'expose': True}, 'tab_list': {'role': 'action', 'description': 'Show the list page.', 'expose': True}, 'tab_other': {'role': 'action', 'description': 'Inactive top-level navigation button used to expose stale focus.', 'expose': True}, 'hint': {'role': 'text', 'description': 'Manual reproduction hint.', 'expose': False}, 'page': {'role': 'text', 'description': 'Dynamic page host.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Stale ScrollView Focus",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.hint = self.create_element("hint", "label", text='Focus list, press Enter, choose Open.')
        self.page = self.create_element("page", "viewhost", )
        self.quit_btn = self.create_element("quit_btn", "button", title='Quit')
        self.tab_list = self.create_element("tab_list", "button", title='List')
        self.tab_other = self.create_element("tab_other", "button", title='Other')
        self.title_element = self.create_element("title", "label", text='Scroll Focus Repro')
    def on_quit_btn_click(self):
        pass

    def on_tab_list_click(self):
        pass

    def on_tab_other_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.quit_btn:
            self.on_quit_btn_click()
        elif element is self.tab_list:
            self.on_tab_list_click()
        elif element is self.tab_other:
            self.on_tab_other_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "title": {
        "type": 'label',
        "text": 'Scroll Focus Repro',
        "description": 'Regression window title.',
        "expose": False,
    },
    "quit_btn": {
        "type": 'button',
        "title": 'Quit',
        "description": 'Close the regression app.',
    },
    "tab_list": {
        "type": 'button',
        "title": 'List',
        "description": 'Show the list page.',
    },
    "tab_other": {
        "type": 'button',
        "title": 'Other',
        "description": 'Inactive top-level navigation button used to expose stale focus.',
    },
    "hint": {
        "type": 'label',
        "text": 'Focus list, press Enter, choose Open.',
        "description": 'Manual reproduction hint.',
        "expose": False,
    },
    "page": {
        "type": 'viewhost',
        "description": 'Dynamic page host.',
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
        "background": '#1f2937',
    },
    "@title": {
        "color": '#ffffff',
        "text-align": 'center',
    },
    "@nav": {
        "background": '#172033',
    },
    "@content": {
        "background": '#030712',
    },
    "@hint": {
        "color": '#93c5fd',
    },
    "@tab_list": {
        "background": '#2f80ed',
        "focus-background": '#22c55e',
    },
    "@tab_other": {
        "background": '#343b4b',
        "focus-background": '#9333ea',
    },
    "@quit_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "viewhost": {
        "background": '#030712',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 4,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 29,
        'cell_name': 'content',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 88,
        'chars_height': 29,
        'margin_right': 1,
        'type': 'viewhost',
        'name': 'page'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 1,
        'cell_name': 'nav',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 16,
        'chars_height': 1,
        'margin_right': 73,
        'width': 16,
        'height': 1,
        'type': 'button',
        'name': 'tab_list'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 75,
        'chars_height': 1,
        'margin_right': 14,
        'height': 1,
        'type': 'label',
        'name': 'title'
    },
    {
        'row': 0,
        'col': 18,
        'relative_col': 18,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 1,
        'cell_name': 'nav',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 17,
        'chars_height': 1,
        'margin_right': 55,
        'width': 17,
        'height': 1,
        'type': 'button',
        'name': 'tab_other'
    },
    {
        'row': 0,
        'col': 36,
        'relative_col': 36,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 90,
        'cell_chars_height': 1,
        'cell_name': 'nav',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 53,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'label',
        'name': 'hint'
    },
    {
        'row': 0,
        'col': 77,
        'relative_col': 77,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 90,
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
    }
]
