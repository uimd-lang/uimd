"""Auto-generated UI code for calculator_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow



class CalculatorUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Calculator\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression."\ntags: [example]\n```\n\n## Members\n\n```yaml\ndisplay:\n  type: label\n  text: ""\n  description: "Shows the current number, operator result, or calculation state."\n\nn0:\n  type: button\n  title: 0\n  description: "Enter digit 0."\n\nn1:\n  type: button\n  title: 1\n  description: "Enter digit 1."\n\nn2:\n  type: button\n  title: 2\n  description: "Enter digit 2."\n\nn3:\n  type: button\n  title: 3\n  description: "Enter digit 3."\n\nn4:\n  type: button\n  title: 4\n  description: "Enter digit 4."\n\nn5:\n  type: button\n  title: 5\n  description: "Enter digit 5."\n\nn6:\n  type: button\n  title: 6\n  description: "Enter digit 6."\n\nn7:\n  type: button\n  title: 7\n  description: "Enter digit 7."\n\nn8:\n  type: button\n  title: 8\n  description: "Enter digit 8."\n\nn9:\n  type: button\n  title: 9\n  description: "Enter digit 9."\n\nsb:\n  type: button\n  title: "="\n  description: "Evaluate the pending calculation."\n\ndt:\n  type: button\n  title: .\n  description: "Insert a decimal point into the current number."\n\nac:\n  type: button\n  title: AC\n  description: "Clear the current calculator state."\n\ndl:\n  type: button\n  title: DEL\n  description: "Delete the last entered character."\n\npr:\n  type: button\n  title: "%"\n  description: "Convert or apply percentage behavior for the current value."\n\ndv:\n  type: button\n  title: /\n  description: "Choose division as the pending operator."\n\nml:\n  type: button\n  title: "*"\n  description: "Choose multiplication as the pending operator."\n\nmn:\n  type: button\n  title: -\n  description: "Choose subtraction as the pending operator."\n\nad:\n  type: button\n  title: "+"\n  description: "Choose addition as the pending operator."\n```\n\n## Style\n\n```yaml\n@display:\n  background: #32446d\n  color: #c8e0ff\n  text-align: right\n```\n\n## User Interface\n\n```ui\n+----------------------+\n|                      |\n|  display...........  |\n|                      |\n|  ac.  dl.  pr.  dv.  |\n|                      |\n|  n7.  n8.  n9.  ml.  |\n|                      |\n|  n4.  n5.  n6.  mn.  |\n|                      |\n|  n1.  n2.  n3.  sb.  |\n|                      |\n|  n0......  dt.  ad.  |\n|                      |\n+----------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.'}, 'elements': {'display': {'role': 'text', 'description': 'Shows the current number, operator result, or calculation state.', 'expose': True}, 'n0': {'role': 'action', 'description': 'Enter digit 0.', 'expose': True}, 'n1': {'role': 'action', 'description': 'Enter digit 1.', 'expose': True}, 'n2': {'role': 'action', 'description': 'Enter digit 2.', 'expose': True}, 'n3': {'role': 'action', 'description': 'Enter digit 3.', 'expose': True}, 'n4': {'role': 'action', 'description': 'Enter digit 4.', 'expose': True}, 'n5': {'role': 'action', 'description': 'Enter digit 5.', 'expose': True}, 'n6': {'role': 'action', 'description': 'Enter digit 6.', 'expose': True}, 'n7': {'role': 'action', 'description': 'Enter digit 7.', 'expose': True}, 'n8': {'role': 'action', 'description': 'Enter digit 8.', 'expose': True}, 'n9': {'role': 'action', 'description': 'Enter digit 9.', 'expose': True}, 'sb': {'role': 'action', 'description': 'Evaluate the pending calculation.', 'expose': True}, 'dt': {'role': 'action', 'description': 'Insert a decimal point into the current number.', 'expose': True}, 'ac': {'role': 'action', 'description': 'Clear the current calculator state.', 'expose': True}, 'dl': {'role': 'action', 'description': 'Delete the last entered character.', 'expose': True}, 'pr': {'role': 'action', 'description': 'Convert or apply percentage behavior for the current value.', 'expose': True}, 'dv': {'role': 'action', 'description': 'Choose division as the pending operator.', 'expose': True}, 'ml': {'role': 'action', 'description': 'Choose multiplication as the pending operator.', 'expose': True}, 'mn': {'role': 'action', 'description': 'Choose subtraction as the pending operator.', 'expose': True}, 'ad': {'role': 'action', 'description': 'Choose addition as the pending operator.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Calculator",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.ac = self.create_element("ac", "button", title='AC')
        self.ad = self.create_element("ad", "button", title='+')
        self.display = self.create_element("display", "label", text='')
        self.dl = self.create_element("dl", "button", title='DEL')
        self.dt = self.create_element("dt", "button", title='.')
        self.dv = self.create_element("dv", "button", title='/')
        self.ml = self.create_element("ml", "button", title='*')
        self.mn = self.create_element("mn", "button", title='-')
        self.n0 = self.create_element("n0", "button", title='0')
        self.n1 = self.create_element("n1", "button", title='1')
        self.n2 = self.create_element("n2", "button", title='2')
        self.n3 = self.create_element("n3", "button", title='3')
        self.n4 = self.create_element("n4", "button", title='4')
        self.n5 = self.create_element("n5", "button", title='5')
        self.n6 = self.create_element("n6", "button", title='6')
        self.n7 = self.create_element("n7", "button", title='7')
        self.n8 = self.create_element("n8", "button", title='8')
        self.n9 = self.create_element("n9", "button", title='9')
        self.pr = self.create_element("pr", "button", title='%')
        self.sb = self.create_element("sb", "button", title='=')
    def on_n0_click(self):
        pass

    def on_n1_click(self):
        pass

    def on_n2_click(self):
        pass

    def on_n3_click(self):
        pass

    def on_n4_click(self):
        pass

    def on_n5_click(self):
        pass

    def on_n6_click(self):
        pass

    def on_n7_click(self):
        pass

    def on_n8_click(self):
        pass

    def on_n9_click(self):
        pass

    def on_sb_click(self):
        pass

    def on_dt_click(self):
        pass

    def on_ac_click(self):
        pass

    def on_dl_click(self):
        pass

    def on_pr_click(self):
        pass

    def on_dv_click(self):
        pass

    def on_ml_click(self):
        pass

    def on_mn_click(self):
        pass

    def on_ad_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.n0:
            self.on_n0_click()
        elif element is self.n1:
            self.on_n1_click()
        elif element is self.n2:
            self.on_n2_click()
        elif element is self.n3:
            self.on_n3_click()
        elif element is self.n4:
            self.on_n4_click()
        elif element is self.n5:
            self.on_n5_click()
        elif element is self.n6:
            self.on_n6_click()
        elif element is self.n7:
            self.on_n7_click()
        elif element is self.n8:
            self.on_n8_click()
        elif element is self.n9:
            self.on_n9_click()
        elif element is self.sb:
            self.on_sb_click()
        elif element is self.dt:
            self.on_dt_click()
        elif element is self.ac:
            self.on_ac_click()
        elif element is self.dl:
            self.on_dl_click()
        elif element is self.pr:
            self.on_pr_click()
        elif element is self.dv:
            self.on_dv_click()
        elif element is self.ml:
            self.on_ml_click()
        elif element is self.mn:
            self.on_mn_click()
        elif element is self.ad:
            self.on_ad_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "display": {
        "type": 'label',
        "text": '',
        "description": 'Shows the current number, operator result, or calculation state.',
    },
    "n0": {
        "type": 'button',
        "title": 0,
        "description": 'Enter digit 0.',
    },
    "n1": {
        "type": 'button',
        "title": 1,
        "description": 'Enter digit 1.',
    },
    "n2": {
        "type": 'button',
        "title": 2,
        "description": 'Enter digit 2.',
    },
    "n3": {
        "type": 'button',
        "title": 3,
        "description": 'Enter digit 3.',
    },
    "n4": {
        "type": 'button',
        "title": 4,
        "description": 'Enter digit 4.',
    },
    "n5": {
        "type": 'button',
        "title": 5,
        "description": 'Enter digit 5.',
    },
    "n6": {
        "type": 'button',
        "title": 6,
        "description": 'Enter digit 6.',
    },
    "n7": {
        "type": 'button',
        "title": 7,
        "description": 'Enter digit 7.',
    },
    "n8": {
        "type": 'button',
        "title": 8,
        "description": 'Enter digit 8.',
    },
    "n9": {
        "type": 'button',
        "title": 9,
        "description": 'Enter digit 9.',
    },
    "sb": {
        "type": 'button',
        "title": '=',
        "description": 'Evaluate the pending calculation.',
    },
    "dt": {
        "type": 'button',
        "title": '.',
        "description": 'Insert a decimal point into the current number.',
    },
    "ac": {
        "type": 'button',
        "title": 'AC',
        "description": 'Clear the current calculator state.',
    },
    "dl": {
        "type": 'button',
        "title": 'DEL',
        "description": 'Delete the last entered character.',
    },
    "pr": {
        "type": 'button',
        "title": '%',
        "description": 'Convert or apply percentage behavior for the current value.',
    },
    "dv": {
        "type": 'button',
        "title": '/',
        "description": 'Choose division as the pending operator.',
    },
    "ml": {
        "type": 'button',
        "title": '*',
        "description": 'Choose multiplication as the pending operator.',
    },
    "mn": {
        "type": 'button',
        "title": '-',
        "description": 'Choose subtraction as the pending operator.',
    },
    "ad": {
        "type": 'button',
        "title": '+',
        "description": 'Choose addition as the pending operator.',
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
        "background": '#334155',
        "color": '#e2e8f0',
        "focus-background": '#2b5f9f',
        "focus-color": '#ffffff',
    },
    "@display": {
        "background": '#32446d',
        "color": '#c8e0ff',
        "text-align": 'right',
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
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 18,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 11,
        'height': 1,
        'type': 'label',
        'name': 'display'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 17,
        'margin_bottom': 9,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'ac'
    },
    {
        'row': 3,
        'col': 7,
        'relative_row': 3,
        'relative_col': 7,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 12,
        'margin_bottom': 9,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'dl'
    },
    {
        'row': 3,
        'col': 12,
        'relative_row': 3,
        'relative_col': 12,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 7,
        'margin_bottom': 9,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'pr'
    },
    {
        'row': 3,
        'col': 17,
        'relative_row': 3,
        'relative_col': 17,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 9,
        'height': 1,
        'type': 'button',
        'name': 'dv'
    },
    {
        'row': 5,
        'col': 2,
        'relative_row': 5,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 17,
        'margin_bottom': 7,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n7'
    },
    {
        'row': 5,
        'col': 7,
        'relative_row': 5,
        'relative_col': 7,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 12,
        'margin_bottom': 7,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n8'
    },
    {
        'row': 5,
        'col': 12,
        'relative_row': 5,
        'relative_col': 12,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 7,
        'margin_bottom': 7,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n9'
    },
    {
        'row': 5,
        'col': 17,
        'relative_row': 5,
        'relative_col': 17,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 7,
        'height': 1,
        'type': 'button',
        'name': 'ml'
    },
    {
        'row': 7,
        'col': 2,
        'relative_row': 7,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 17,
        'margin_bottom': 5,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n4'
    },
    {
        'row': 7,
        'col': 7,
        'relative_row': 7,
        'relative_col': 7,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 12,
        'margin_bottom': 5,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n5'
    },
    {
        'row': 7,
        'col': 12,
        'relative_row': 7,
        'relative_col': 12,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 7,
        'margin_bottom': 5,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n6'
    },
    {
        'row': 7,
        'col': 17,
        'relative_row': 7,
        'relative_col': 17,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 5,
        'height': 1,
        'type': 'button',
        'name': 'mn'
    },
    {
        'row': 9,
        'col': 2,
        'relative_row': 9,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 17,
        'margin_bottom': 3,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n1'
    },
    {
        'row': 9,
        'col': 7,
        'relative_row': 9,
        'relative_col': 7,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 12,
        'margin_bottom': 3,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n2'
    },
    {
        'row': 9,
        'col': 12,
        'relative_row': 9,
        'relative_col': 12,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 7,
        'margin_bottom': 3,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'n3'
    },
    {
        'row': 9,
        'col': 17,
        'relative_row': 9,
        'relative_col': 17,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 3,
        'height': 1,
        'type': 'button',
        'name': 'sb'
    },
    {
        'row': 11,
        'col': 2,
        'relative_row': 11,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 8,
        'chars_height': 1,
        'margin_right': 12,
        'margin_bottom': 1,
        'width': 8,
        'height': 1,
        'type': 'button',
        'name': 'n0'
    },
    {
        'row': 11,
        'col': 12,
        'relative_row': 11,
        'relative_col': 12,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 7,
        'margin_bottom': 1,
        'width': 3,
        'height': 1,
        'type': 'button',
        'name': 'dt'
    },
    {
        'row': 11,
        'col': 17,
        'relative_row': 11,
        'relative_col': 17,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 22,
        'cell_chars_height': 13,
        'cell_width': 22,
        'cell_height': 13,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'button',
        'name': 'ad'
    }
]
