"""Auto-generated UI code for mcp_tester_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow

from uimd.testing.log_panel import LogPanel


class McpTesterUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# MCP Tester\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "MCP Tester UI source."\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - log_panel\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: "MCP Tester"\n  description: "MCP tester window title."\n  expose: false\n\nrun:\n  type: button\n  title: Run\n\nnext:\n  type: button\n  title: Next\n\npause:\n  type: button\n  title: Pause\n\ncopy:\n  type: button\n  title: "Copy Log"\n\nquit:\n  type: button\n  title: Quit\n\napp_viewport:\n  type: framebufferview\n\napp_header:\n  type: label\n  text: ""\n\nlog_output:\n  type: log_panel\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#0b1018"\n@header:\n  text-align: center\n  color: "#ffffff"\n  background: "#1f3a5f"\n@run:\n  background: "#14532d"\n  color: "#dcfce7"\n  focus-background: "#16a34a"\n@next:\n  background: "#1e3a8a"\n  color: "#dbeafe"\n  focus-background: "#2563eb"\n@pause:\n  background: "#713f12"\n  color: "#fef3c7"\n  focus-background: "#ca8a04"\n@copy:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@quit:\n  background: "#7f1d1d"\n  color: "#fee2e2"\n  focus-background: "#dc2626"\n@app_viewport:\n  background: "#0b1018"\n  color: "#e5e7eb"\n  focus-color: "#facc15"\n  edit-color: "#22c55e"\n  word-wrap: false\n@app_header:\n  background: "#000000"\n  color: "#e5e7eb"\n  word-wrap: false\n@log_output:\n  background: "#000000"\n  focus-background: "#1e3a5f"\n  edit-background: "#0f2a4a"\n```\n\n## User Interface\n\n```ui\n+-header_cell-**------------------------------------------------------------------------+\n| header..**............................... run... next... pause... copy....... quit... |\n+-app-**--------------------------------------------------------------------------------+\n|app_header...**....................................................................... |\n|app_viewport..**...................................................................... |\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n**......................................................................................|\n**......................................................................................|\n+-log-**--------------------------------------------------------------------------------+\n|log_output..**.........................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n|.......................................................................................|\n+---------------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'MCP Tester UI source.'}, 'elements': {'header': {'role': 'text', 'description': 'MCP tester window title.', 'expose': False}, 'run': {'role': 'action', 'description': 'Run', 'expose': True}, 'next': {'role': 'action', 'description': 'Next', 'expose': True}, 'pause': {'role': 'action', 'description': 'Pause', 'expose': True}, 'copy': {'role': 'action', 'description': 'Copy Log', 'expose': True}, 'quit': {'role': 'action', 'description': 'Quit', 'expose': True}, 'app_viewport': {'role': 'text', 'description': 'app_viewport', 'expose': True}, 'app_header': {'role': 'text', 'description': 'app_header', 'expose': True}, 'log_output': {'role': 'text', 'description': 'log_output', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="MCP Tester",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.app_header = self.create_element("app_header", "label", text='')
        self.app_viewport = self.create_element("app_viewport", "framebufferview", )
        self.copy = self.create_element("copy", "button", title='Copy Log')
        self.header = self.create_element("header", "label", text='MCP Tester')
        self.log_output = self.create_element("log_output", "uielement", layout_class=LogPanel)
        self.next = self.create_element("next", "button", title='Next')
        self.pause = self.create_element("pause", "button", title='Pause')
        self.quit = self.create_element("quit", "button", title='Quit')
        self.run = self.create_element("run", "button", title='Run')
    def on_run_click(self):
        pass

    def on_next_click(self):
        pass

    def on_pause_click(self):
        pass

    def on_copy_click(self):
        pass

    def on_quit_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.run:
            self.on_run_click()
        elif element is self.next:
            self.on_next_click()
        elif element is self.pause:
            self.on_pause_click()
        elif element is self.copy:
            self.on_copy_click()
        elif element is self.quit:
            self.on_quit_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header": {
        "type": 'label',
        "text": 'MCP Tester',
        "description": 'MCP tester window title.',
        "expose": False,
    },
    "run": {
        "type": 'button',
        "title": 'Run',
    },
    "next": {
        "type": 'button',
        "title": 'Next',
    },
    "pause": {
        "type": 'button',
        "title": 'Pause',
    },
    "copy": {
        "type": 'button',
        "title": 'Copy Log',
    },
    "quit": {
        "type": 'button',
        "title": 'Quit',
    },
    "app_viewport": {
        "type": 'framebufferview',
    },
    "app_header": {
        "type": 'label',
        "text": '',
    },
    "log_output": {
        "type": 'log_panel',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#0b1018',
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
    "@header": {
        "text-align": 'center',
        "color": '#ffffff',
        "background": '#1f3a5f',
    },
    "@run": {
        "background": '#14532d',
        "color": '#dcfce7',
        "focus-background": '#16a34a',
    },
    "@next": {
        "background": '#1e3a8a',
        "color": '#dbeafe',
        "focus-background": '#2563eb',
    },
    "@pause": {
        "background": '#713f12',
        "color": '#fef3c7',
        "focus-background": '#ca8a04',
    },
    "@copy": {
        "background": '#334155',
        "color": '#e2e8f0',
        "focus-background": '#2563eb',
    },
    "@quit": {
        "background": '#7f1d1d',
        "color": '#fee2e2',
        "focus-background": '#dc2626',
    },
    "@app_viewport": {
        "background": '#0b1018',
        "color": '#e5e7eb',
        "focus-color": '#facc15',
        "edit-color": '#22c55e',
        "word-wrap": False,
    },
    "@app_header": {
        "background": '#000000',
        "color": '#e5e7eb',
        "word-wrap": False,
    },
    "@log_output": {
        "background": '#000000',
        "focus-background": '#1e3a5f',
        "edit-background": '#0f2a4a',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 15,
        'cell_name': 'app',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 86,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 14,
        'height': 1,
        'type': 'label',
        'name': 'app_header'
    },
    {
        'row': 0,
        'col': 0,
        'cell_row': 18,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 10,
        'cell_name': 'log',
        'cell_height': 10,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 87,
        'chars_height': 10,
        'height': 10,
        'type': 'log_panel',
        'name': 'log_output'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 1,
        'cell_name': 'header_cell',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 41,
        'chars_height': 1,
        'margin_right': 45,
        'height': 1,
        'type': 'label',
        'name': 'header'
    },
    {
        'row': 0,
        'col': 43,
        'relative_col': 43,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 1,
        'cell_name': 'header_cell',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 6,
        'chars_height': 1,
        'margin_right': 38,
        'width': 6,
        'height': 1,
        'type': 'button',
        'name': 'run'
    },
    {
        'row': 0,
        'col': 50,
        'relative_col': 50,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 1,
        'cell_name': 'header_cell',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 1,
        'margin_right': 30,
        'width': 7,
        'height': 1,
        'type': 'button',
        'name': 'next'
    },
    {
        'row': 0,
        'col': 58,
        'relative_col': 58,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 1,
        'cell_name': 'header_cell',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 8,
        'chars_height': 1,
        'margin_right': 21,
        'width': 8,
        'height': 1,
        'type': 'button',
        'name': 'pause'
    },
    {
        'row': 0,
        'col': 67,
        'relative_col': 67,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 1,
        'cell_name': 'header_cell',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 9,
        'width': 11,
        'height': 1,
        'type': 'button',
        'name': 'copy'
    },
    {
        'row': 0,
        'col': 79,
        'relative_col': 79,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 1,
        'cell_name': 'header_cell',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 1,
        'margin_right': 1,
        'width': 7,
        'height': 1,
        'type': 'button',
        'name': 'quit'
    },
    {
        'row': 1,
        'col': 0,
        'relative_row': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 87,
        'cell_chars_height': 15,
        'cell_name': 'app',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 86,
        'chars_height': 14,
        'margin_right': 1,
        'type': 'framebufferview',
        'name': 'app_viewport'
    }
]
