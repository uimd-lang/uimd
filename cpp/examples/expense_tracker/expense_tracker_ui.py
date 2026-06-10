"""Auto-generated UI code for expense_tracker_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow



class ExpenseTrackerUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Expense Tracker\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: experimental\ndescription: "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost."\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nuses:\n  - expenses_view\n  - categories_view\n  - settings_view\n```\n\n## Members\n\n```yaml\napp_title:\n  type: label\n  text: "Expense Tracker"\n\nquit_btn:\n  type: button\n  title: Quit\n\nnav_title:\n  type: label\n  text: Expenses\n\nexpenses_btn:\n  type: button\n  title: Expenses\n\ncategories_btn:\n  type: button\n  title: Categories\n\nsettings_btn:\n  type: button\n  title: Settings\n\nmain:\n  type: viewhost\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: "#111827"\n  padding: 0\n@app_title:\n  color: "#f8fafc"\n  text-align: center\n@nav:\n  background: "#14295a"\n@content:\n  background: "#0b1f33"\n@nav_title:\n  color: "#f8fafc"\nbutton:\n  background: "#1d4f8c"\n  color: "#e5e7eb"\n  focus-background: "#38bdf8"\n  focus-color: "#ffffff"\n@quit_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\nviewhost:\n  background: "#0b1f33"\n```\n\n## User Interface\n\n```ui\n+--header-**------------------------------------------+\n| app_title..**.......................... quit_btn... |\n+--nav------------------+--content-**-----------------+\n| nav_title............ | main..**................... |\n|                       | *.......................... |\n| expenses_btn......... | *.......................... |\n|                       | ........................... |\n| categories_btn....... | ........................... |\n|                       | ........................... |\n| settings_btn......... | ........................... |\n*                       * ........................... |\n*                       * ........................... |\n+-----------------------+-----------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.'}, 'elements': {'app_title': {'role': 'text', 'description': 'Expense Tracker', 'expose': True}, 'quit_btn': {'role': 'action', 'description': 'Quit', 'expose': True}, 'nav_title': {'role': 'text', 'description': 'Expenses', 'expose': True}, 'expenses_btn': {'role': 'action', 'description': 'Expenses', 'expose': True}, 'categories_btn': {'role': 'action', 'description': 'Categories', 'expose': True}, 'settings_btn': {'role': 'action', 'description': 'Settings', 'expose': True}, 'main': {'role': 'text', 'description': 'main', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Expense Tracker",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.app_title = self.create_element("app_title", "label", text='Expense Tracker')
        self.categories_btn = self.create_element("categories_btn", "button", title='Categories')
        self.expenses_btn = self.create_element("expenses_btn", "button", title='Expenses')
        self.main = self.create_element("main", "viewhost", )
        self.nav_title = self.create_element("nav_title", "label", text='Expenses')
        self.quit_btn = self.create_element("quit_btn", "button", title='Quit')
        self.settings_btn = self.create_element("settings_btn", "button", title='Settings')
    def on_quit_btn_click(self):
        pass

    def on_expenses_btn_click(self):
        pass

    def on_categories_btn_click(self):
        pass

    def on_settings_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.quit_btn:
            self.on_quit_btn_click()
        elif element is self.expenses_btn:
            self.on_expenses_btn_click()
        elif element is self.categories_btn:
            self.on_categories_btn_click()
        elif element is self.settings_btn:
            self.on_settings_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "app_title": {
        "type": 'label',
        "text": 'Expense Tracker',
    },
    "quit_btn": {
        "type": 'button',
        "title": 'Quit',
    },
    "nav_title": {
        "type": 'label',
        "text": 'Expenses',
    },
    "expenses_btn": {
        "type": 'button',
        "title": 'Expenses',
    },
    "categories_btn": {
        "type": 'button',
        "title": 'Categories',
    },
    "settings_btn": {
        "type": 'button',
        "title": 'Settings',
    },
    "main": {
        "type": 'viewhost',
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
        "background": '#1d4f8c',
        "color": '#e5e7eb',
        "focus-background": '#38bdf8',
        "focus-color": '#ffffff',
    },
    "cell": {
        "padding": 1,
    },
    "@header": {
        "background": '#111827',
        "padding": 0,
    },
    "@app_title": {
        "color": '#f8fafc',
        "text-align": 'center',
    },
    "@nav": {
        "background": '#14295a',
    },
    "@content": {
        "background": '#0b1f33',
    },
    "@nav_title": {
        "color": '#f8fafc',
    },
    "@quit_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "viewhost": {
        "background": '#0b1f33',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 53,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 39,
        'chars_height': 1,
        'margin_right': 13,
        'height': 1,
        'type': 'label',
        'name': 'app_title'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 24,
        'cell_chars_width': 29,
        'cell_chars_height': 9,
        'cell_name': 'content',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 27,
        'chars_height': 9,
        'margin_right': 1,
        'type': 'viewhost',
        'name': 'main'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 23,
        'cell_chars_height': 9,
        'cell_name': 'nav',
        'cell_width': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 21,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 8,
        'height': 1,
        'type': 'label',
        'name': 'nav_title'
    },
    {
        'row': 0,
        'col': 41,
        'relative_col': 41,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 53,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 1,
        'width': 11,
        'height': 1,
        'type': 'button',
        'name': 'quit_btn'
    },
    {
        'row': 2,
        'col': 1,
        'relative_row': 2,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 23,
        'cell_chars_height': 9,
        'cell_name': 'nav',
        'cell_width': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 21,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 6,
        'height': 1,
        'type': 'button',
        'name': 'expenses_btn'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 23,
        'cell_chars_height': 9,
        'cell_name': 'nav',
        'cell_width': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 21,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 4,
        'height': 1,
        'type': 'button',
        'name': 'categories_btn'
    },
    {
        'row': 6,
        'col': 1,
        'relative_row': 6,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 23,
        'cell_chars_height': 9,
        'cell_name': 'nav',
        'cell_width': 23,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 21,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 2,
        'height': 1,
        'type': 'button',
        'name': 'settings_btn'
    }
]
