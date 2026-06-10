"""Auto-generated UI code for cells_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow



class CellsUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Cells\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior."\ntags: [example]\n```\n\n## Members\n\n```yaml\na1:\n  type: button\n  title: A1\n  description: "Button in the first red cell."\n\na2:\n  type: button\n  title: A2\n  description: "Second button in the first red cell."\n\nb1:\n  type: button\n  title: BBB\n  description: "Wide button in the green cell."\n\nc1:\n  type: button\n  title: C1\n  description: "Button in the yellow cell group."\n\nc2:\n  type: button\n  title: C2\n  description: "Button in the yellow cell group."\n\nc3:\n  type: button\n  title: C3\n  description: "Button in the yellow cell group."\n\nc4:\n  type: button\n  title: C4\n  description: "Button in the yellow cell group."\n\nc5:\n  type: button\n  title: C5\n  description: "Button in the yellow cell group."\n\nc6:\n  type: button\n  title: C6\n  description: "Button in the yellow cell group."\n\nc7:\n  type: button\n  title: C7\n  description: "Button in the yellow cell group."\n\nc8:\n  type: button\n  title: C8\n  description: "Button in the yellow cell group."\n\nc9:\n  type: button\n  title: C9\n  description: "Button in the yellow cell group."\n\nc10:\n  type: button\n  title: C10\n  description: "Button in the yellow cell group."\n\nd1:\n  type: button\n  title: D1\n  description: "Top button in the right column."\n\nd2:\n  type: button\n  title: D2\n  description: "Button in the right column."\n\nd3:\n  type: button\n  title: D3\n  description: "Button in the right column."\n\nd4:\n  type: button\n  title: D4\n  description: "Button in the right column."\n\nd5:\n  type: button\n  title: D5\n  description: "Button in the right column."\n\nd6:\n  type: button\n  title: D6\n  description: "Bottom button in the right column."\n\ne1:\n  type: button\n  title: E\n  description: "Single-character E button in the lower left cell."\n\ne2:\n  type: button\n  title: EE\n  description: "Two-character E button in the lower left cell."\n\ne3:\n  type: button\n  title: EEE\n  description: "Three-character E button in the lower left cell."\n\ne4:\n  type: button\n  title: EEEE\n  description: "Four-character E button in the lower left cell."\n\ne5:\n  type: button\n  title: EEEEE\n  description: "Five-character E button in the lower left cell."\n\ne6:\n  type: button\n  title: EEEEEE\n  description: "Six-character E button in the lower left cell."\n\ne7:\n  type: button\n  title: EEEEEEE\n  description: "Seven-character E button in the lower left cell."\n\ne8:\n  type: button\n  title: EEEEEEEE\n  description: "Eight-character E button in the lower left cell."\n\nf1:\n  type: button\n  title: F1\n  description: "Top button in the cyan cell."\n\nf2:\n  type: button\n  title: F2\n  description: "Middle button in the cyan cell with a local style override."\n\nf3:\n  type: button\n  title: F3\n  description: "Bottom button in the cyan cell."\n```\n\n## Style\n\n```yaml\nthis:\n  background: "#1f2946"\n  border-color: transparent\n  border-width: 1\n  border-width-vertical: 2\nbutton:\n  background: "#526173"\n  color: "#eef2f7"\n  focus-background: "#6b7c91"\n@d1:\n  color: "#d7fbe8"\ncell1:\n  background: "#3b1f2d"\n  button:\n    background: "#7a3f59"\n    focus-background: "#96536d"\ncell2:\n  background: "#1f463f"\n  button:\n    background: "#3d7b6d"\n    focus-background: "#4f9484"\ncl3:\n  background: "#26345f"\n  button:\n    background: "#5267a3"\n    focus-background: "#657abd"\ncell4:\n  background: "#4a4422"\n  button:\n    background: "#81753b"\n    color: "#ecfdf5"\n    focus-background: "#9a8d4a"\ncell5:\n  background: "#482456"\n  button:\n    background: "#805196"\n    focus-background: "#9a65b0"\ncl6:\n  background: "#174f55"\n  button:\n    background: "#3a8890"\n    focus-background: "#4aa1aa"\n  @f2:\n    background: "#31565f"\ncl7:\n  background: "#2f3745"\n  button:\n    background: "#64748b"\n    focus-background: "#7b8ba0"\n```\n\n## User Interface\n\n```ui\n+-cell1-----------+-cell2---+-cl3+\n| a1..... a2..... |  b1...  | d1 |\n+-cell4-----------+---------+    |\n| c1.. c2.. c3.. c4.. c5..  | d2 |\n| c6.. c7.. c8.. c9.. c10.  |    |\n+-cell5---------------+-cl6-+ d3 |\n|                     | f1. |    |\n| e1....... e5....... |     | d4 |\n|                     |     |    |\n| e2....... e6....... | f2. | d5 |\n|                     |     |    |\n| e3....... e7....... |     | d6 |\n|                     | f3. +-cl7+\n| e4....... e8....... |     |    |\n|                     |     |    |\n+---------------------+-----+----+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.'}, 'elements': {'a1': {'role': 'action', 'description': 'Button in the first red cell.', 'expose': True}, 'a2': {'role': 'action', 'description': 'Second button in the first red cell.', 'expose': True}, 'b1': {'role': 'action', 'description': 'Wide button in the green cell.', 'expose': True}, 'c1': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c2': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c3': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c4': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c5': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c6': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c7': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c8': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c9': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'c10': {'role': 'action', 'description': 'Button in the yellow cell group.', 'expose': True}, 'd1': {'role': 'action', 'description': 'Top button in the right column.', 'expose': True}, 'd2': {'role': 'action', 'description': 'Button in the right column.', 'expose': True}, 'd3': {'role': 'action', 'description': 'Button in the right column.', 'expose': True}, 'd4': {'role': 'action', 'description': 'Button in the right column.', 'expose': True}, 'd5': {'role': 'action', 'description': 'Button in the right column.', 'expose': True}, 'd6': {'role': 'action', 'description': 'Bottom button in the right column.', 'expose': True}, 'e1': {'role': 'action', 'description': 'Single-character E button in the lower left cell.', 'expose': True}, 'e2': {'role': 'action', 'description': 'Two-character E button in the lower left cell.', 'expose': True}, 'e3': {'role': 'action', 'description': 'Three-character E button in the lower left cell.', 'expose': True}, 'e4': {'role': 'action', 'description': 'Four-character E button in the lower left cell.', 'expose': True}, 'e5': {'role': 'action', 'description': 'Five-character E button in the lower left cell.', 'expose': True}, 'e6': {'role': 'action', 'description': 'Six-character E button in the lower left cell.', 'expose': True}, 'e7': {'role': 'action', 'description': 'Seven-character E button in the lower left cell.', 'expose': True}, 'e8': {'role': 'action', 'description': 'Eight-character E button in the lower left cell.', 'expose': True}, 'f1': {'role': 'action', 'description': 'Top button in the cyan cell.', 'expose': True}, 'f2': {'role': 'action', 'description': 'Middle button in the cyan cell with a local style override.', 'expose': True}, 'f3': {'role': 'action', 'description': 'Bottom button in the cyan cell.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Cells",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.a1 = self.create_element("a1", "button", title='A1')
        self.a2 = self.create_element("a2", "button", title='A2')
        self.b1 = self.create_element("b1", "button", title='BBB')
        self.c1 = self.create_element("c1", "button", title='C1')
        self.c10 = self.create_element("c10", "button", title='C10')
        self.c2 = self.create_element("c2", "button", title='C2')
        self.c3 = self.create_element("c3", "button", title='C3')
        self.c4 = self.create_element("c4", "button", title='C4')
        self.c5 = self.create_element("c5", "button", title='C5')
        self.c6 = self.create_element("c6", "button", title='C6')
        self.c7 = self.create_element("c7", "button", title='C7')
        self.c8 = self.create_element("c8", "button", title='C8')
        self.c9 = self.create_element("c9", "button", title='C9')
        self.d1 = self.create_element("d1", "button", title='D1')
        self.d2 = self.create_element("d2", "button", title='D2')
        self.d3 = self.create_element("d3", "button", title='D3')
        self.d4 = self.create_element("d4", "button", title='D4')
        self.d5 = self.create_element("d5", "button", title='D5')
        self.d6 = self.create_element("d6", "button", title='D6')
        self.e1 = self.create_element("e1", "button", title='E')
        self.e2 = self.create_element("e2", "button", title='EE')
        self.e3 = self.create_element("e3", "button", title='EEE')
        self.e4 = self.create_element("e4", "button", title='EEEE')
        self.e5 = self.create_element("e5", "button", title='EEEEE')
        self.e6 = self.create_element("e6", "button", title='EEEEEE')
        self.e7 = self.create_element("e7", "button", title='EEEEEEE')
        self.e8 = self.create_element("e8", "button", title='EEEEEEEE')
        self.f1 = self.create_element("f1", "button", title='F1')
        self.f2 = self.create_element("f2", "button", title='F2')
        self.f3 = self.create_element("f3", "button", title='F3')
        self._cell_0 = self.create_element("_cell_0", "label", text='')
    def on_a1_click(self):
        pass

    def on_a2_click(self):
        pass

    def on_b1_click(self):
        pass

    def on_c1_click(self):
        pass

    def on_c2_click(self):
        pass

    def on_c3_click(self):
        pass

    def on_c4_click(self):
        pass

    def on_c5_click(self):
        pass

    def on_c6_click(self):
        pass

    def on_c7_click(self):
        pass

    def on_c8_click(self):
        pass

    def on_c9_click(self):
        pass

    def on_c10_click(self):
        pass

    def on_d1_click(self):
        pass

    def on_d2_click(self):
        pass

    def on_d3_click(self):
        pass

    def on_d4_click(self):
        pass

    def on_d5_click(self):
        pass

    def on_d6_click(self):
        pass

    def on_e1_click(self):
        pass

    def on_e2_click(self):
        pass

    def on_e3_click(self):
        pass

    def on_e4_click(self):
        pass

    def on_e5_click(self):
        pass

    def on_e6_click(self):
        pass

    def on_e7_click(self):
        pass

    def on_e8_click(self):
        pass

    def on_f1_click(self):
        pass

    def on_f2_click(self):
        pass

    def on_f3_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.a1:
            self.on_a1_click()
        elif element is self.a2:
            self.on_a2_click()
        elif element is self.b1:
            self.on_b1_click()
        elif element is self.c1:
            self.on_c1_click()
        elif element is self.c2:
            self.on_c2_click()
        elif element is self.c3:
            self.on_c3_click()
        elif element is self.c4:
            self.on_c4_click()
        elif element is self.c5:
            self.on_c5_click()
        elif element is self.c6:
            self.on_c6_click()
        elif element is self.c7:
            self.on_c7_click()
        elif element is self.c8:
            self.on_c8_click()
        elif element is self.c9:
            self.on_c9_click()
        elif element is self.c10:
            self.on_c10_click()
        elif element is self.d1:
            self.on_d1_click()
        elif element is self.d2:
            self.on_d2_click()
        elif element is self.d3:
            self.on_d3_click()
        elif element is self.d4:
            self.on_d4_click()
        elif element is self.d5:
            self.on_d5_click()
        elif element is self.d6:
            self.on_d6_click()
        elif element is self.e1:
            self.on_e1_click()
        elif element is self.e2:
            self.on_e2_click()
        elif element is self.e3:
            self.on_e3_click()
        elif element is self.e4:
            self.on_e4_click()
        elif element is self.e5:
            self.on_e5_click()
        elif element is self.e6:
            self.on_e6_click()
        elif element is self.e7:
            self.on_e7_click()
        elif element is self.e8:
            self.on_e8_click()
        elif element is self.f1:
            self.on_f1_click()
        elif element is self.f2:
            self.on_f2_click()
        elif element is self.f3:
            self.on_f3_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        pass
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "a1": {
        "type": 'button',
        "title": 'A1',
        "description": 'Button in the first red cell.',
    },
    "a2": {
        "type": 'button',
        "title": 'A2',
        "description": 'Second button in the first red cell.',
    },
    "b1": {
        "type": 'button',
        "title": 'BBB',
        "description": 'Wide button in the green cell.',
    },
    "c1": {
        "type": 'button',
        "title": 'C1',
        "description": 'Button in the yellow cell group.',
    },
    "c2": {
        "type": 'button',
        "title": 'C2',
        "description": 'Button in the yellow cell group.',
    },
    "c3": {
        "type": 'button',
        "title": 'C3',
        "description": 'Button in the yellow cell group.',
    },
    "c4": {
        "type": 'button',
        "title": 'C4',
        "description": 'Button in the yellow cell group.',
    },
    "c5": {
        "type": 'button',
        "title": 'C5',
        "description": 'Button in the yellow cell group.',
    },
    "c6": {
        "type": 'button',
        "title": 'C6',
        "description": 'Button in the yellow cell group.',
    },
    "c7": {
        "type": 'button',
        "title": 'C7',
        "description": 'Button in the yellow cell group.',
    },
    "c8": {
        "type": 'button',
        "title": 'C8',
        "description": 'Button in the yellow cell group.',
    },
    "c9": {
        "type": 'button',
        "title": 'C9',
        "description": 'Button in the yellow cell group.',
    },
    "c10": {
        "type": 'button',
        "title": 'C10',
        "description": 'Button in the yellow cell group.',
    },
    "d1": {
        "type": 'button',
        "title": 'D1',
        "description": 'Top button in the right column.',
    },
    "d2": {
        "type": 'button',
        "title": 'D2',
        "description": 'Button in the right column.',
    },
    "d3": {
        "type": 'button',
        "title": 'D3',
        "description": 'Button in the right column.',
    },
    "d4": {
        "type": 'button',
        "title": 'D4',
        "description": 'Button in the right column.',
    },
    "d5": {
        "type": 'button',
        "title": 'D5',
        "description": 'Button in the right column.',
    },
    "d6": {
        "type": 'button',
        "title": 'D6',
        "description": 'Bottom button in the right column.',
    },
    "e1": {
        "type": 'button',
        "title": 'E',
        "description": 'Single-character E button in the lower left cell.',
    },
    "e2": {
        "type": 'button',
        "title": 'EE',
        "description": 'Two-character E button in the lower left cell.',
    },
    "e3": {
        "type": 'button',
        "title": 'EEE',
        "description": 'Three-character E button in the lower left cell.',
    },
    "e4": {
        "type": 'button',
        "title": 'EEEE',
        "description": 'Four-character E button in the lower left cell.',
    },
    "e5": {
        "type": 'button',
        "title": 'EEEEE',
        "description": 'Five-character E button in the lower left cell.',
    },
    "e6": {
        "type": 'button',
        "title": 'EEEEEE',
        "description": 'Six-character E button in the lower left cell.',
    },
    "e7": {
        "type": 'button',
        "title": 'EEEEEEE',
        "description": 'Seven-character E button in the lower left cell.',
    },
    "e8": {
        "type": 'button',
        "title": 'EEEEEEEE',
        "description": 'Eight-character E button in the lower left cell.',
    },
    "f1": {
        "type": 'button',
        "title": 'F1',
        "description": 'Top button in the cyan cell.',
    },
    "f2": {
        "type": 'button',
        "title": 'F2',
        "description": 'Middle button in the cyan cell with a local style override.',
    },
    "f3": {
        "type": 'button',
        "title": 'F3',
        "description": 'Bottom button in the cyan cell.',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#1f2946',
        "border-color": 'transparent',
        "border-width": 1,
        "border-width-vertical": 2,
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
        "background": '#526173',
        "color": '#eef2f7',
        "disabled-background": '#1f2937',
        "disabled-color": '#64748b',
        "focus-background": '#6b7c91',
        "focus-color": '#ffffff',
    },
    "@d1": {
        "color": '#d7fbe8',
    },
    "cell1": {
        "background": '#3b1f2d',
    },
    "cell1 button": {
        "background": '#7a3f59',
        "focus-background": '#96536d',
    },
    "cell2": {
        "background": '#1f463f',
    },
    "cell2 button": {
        "background": '#3d7b6d',
        "focus-background": '#4f9484',
    },
    "cl3": {
        "background": '#26345f',
    },
    "cl3 button": {
        "background": '#5267a3',
        "focus-background": '#657abd',
    },
    "cell4": {
        "background": '#4a4422',
    },
    "cell4 button": {
        "background": '#81753b',
        "color": '#ecfdf5',
        "focus-background": '#9a8d4a',
    },
    "cell5": {
        "background": '#482456',
    },
    "cell5 button": {
        "background": '#805196',
        "focus-background": '#9a65b0',
    },
    "cl6": {
        "background": '#174f55',
    },
    "cl6 button": {
        "background": '#3a8890',
        "focus-background": '#4aa1aa',
    },
    "cl6 @f2": {
        "background": '#31565f',
    },
    "cl7": {
        "background": '#2f3745',
    },
    "cl7 button": {
        "background": '#64748b',
        "focus-background": '#7b8ba0',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 12,
        'cell_col': 28,
        'cell_chars_width': 4,
        'cell_chars_height': 2,
        'cell_name': 'cl7',
        'cell_width': 4,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 2,
        'width': 4,
        'height': 2,
        'type': 'label',
        'name': '_cell_0'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 17,
        'cell_chars_height': 1,
        'cell_name': 'cell1',
        'cell_width': 17,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 7,
        'chars_height': 1,
        'margin_right': 9,
        'width': 7,
        'height': 1,
        'type': 'button',
        'name': 'a1'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 22,
        'margin_bottom': 1,
        'width': 4,
        'height': 1,
        'type': 'button',
        'name': 'c1'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 28,
        'cell_chars_width': 4,
        'cell_chars_height': 11,
        'cell_name': 'cl3',
        'cell_width': 4,
        'cell_height': 11,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 2,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 10,
        'height': 1,
        'type': 'button',
        'name': 'd1'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 5,
        'cell_col': 22,
        'cell_chars_width': 5,
        'cell_chars_height': 9,
        'cell_name': 'cl6',
        'cell_width': 5,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 8,
        'height': 1,
        'type': 'button',
        'name': 'f1'
    },
    {
        'row': 0,
        'col': 2,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 18,
        'cell_chars_width': 9,
        'cell_chars_height': 1,
        'cell_name': 'cell2',
        'cell_width': 9,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 5,
        'chars_height': 1,
        'margin_right': 2,
        'height': 1,
        'type': 'button',
        'name': 'b1'
    },
    {
        'row': 0,
        'col': 6,
        'relative_col': 6,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 17,
        'margin_bottom': 1,
        'width': 4,
        'height': 1,
        'type': 'button',
        'name': 'c2'
    },
    {
        'row': 0,
        'col': 9,
        'relative_col': 9,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 17,
        'cell_chars_height': 1,
        'cell_name': 'cell1',
        'cell_width': 17,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 7,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'button',
        'name': 'a2'
    },
    {
        'row': 0,
        'col': 11,
        'relative_col': 11,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 12,
        'margin_bottom': 1,
        'width': 4,
        'height': 1,
        'type': 'button',
        'name': 'c3'
    },
    {
        'row': 0,
        'col': 16,
        'relative_col': 16,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 7,
        'margin_bottom': 1,
        'width': 4,
        'height': 1,
        'type': 'button',
        'name': 'c4'
    },
    {
        'row': 0,
        'col': 21,
        'relative_col': 21,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'button',
        'name': 'c5'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 22,
        'width': 4,
        'height': 1,
        'type': 'button',
        'name': 'c6'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 21,
        'cell_chars_height': 9,
        'cell_name': 'cell5',
        'cell_width': 21,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 11,
        'margin_bottom': 7,
        'width': 9,
        'height': 1,
        'type': 'button',
        'name': 'e1'
    },
    {
        'row': 1,
        'col': 6,
        'relative_row': 1,
        'relative_col': 6,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 17,
        'width': 4,
        'height': 1,
        'type': 'button',
        'name': 'c7'
    },
    {
        'row': 1,
        'col': 11,
        'relative_row': 1,
        'relative_col': 11,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 12,
        'width': 4,
        'height': 1,
        'type': 'button',
        'name': 'c8'
    },
    {
        'row': 1,
        'col': 11,
        'relative_row': 1,
        'relative_col': 11,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 21,
        'cell_chars_height': 9,
        'cell_name': 'cell5',
        'cell_width': 21,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 7,
        'height': 1,
        'type': 'button',
        'name': 'e5'
    },
    {
        'row': 1,
        'col': 16,
        'relative_row': 1,
        'relative_col': 16,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 7,
        'width': 4,
        'height': 1,
        'type': 'button',
        'name': 'c9'
    },
    {
        'row': 1,
        'col': 21,
        'relative_row': 1,
        'relative_col': 21,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 2,
        'cell_name': 'cell4',
        'cell_width': 27,
        'cell_height': 2,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 4,
        'chars_height': 1,
        'margin_right': 2,
        'height': 1,
        'type': 'button',
        'name': 'c10'
    },
    {
        'row': 2,
        'col': 1,
        'relative_row': 2,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 28,
        'cell_chars_width': 4,
        'cell_chars_height': 11,
        'cell_name': 'cl3',
        'cell_width': 4,
        'cell_height': 11,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 2,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 8,
        'height': 1,
        'type': 'button',
        'name': 'd2'
    },
    {
        'row': 3,
        'col': 1,
        'relative_row': 3,
        'relative_col': 1,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 21,
        'cell_chars_height': 9,
        'cell_name': 'cell5',
        'cell_width': 21,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 11,
        'margin_bottom': 5,
        'width': 9,
        'height': 1,
        'type': 'button',
        'name': 'e2'
    },
    {
        'row': 3,
        'col': 1,
        'relative_row': 3,
        'relative_col': 1,
        'cell_row': 5,
        'cell_col': 22,
        'cell_chars_width': 5,
        'cell_chars_height': 9,
        'cell_name': 'cl6',
        'cell_width': 5,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 5,
        'height': 1,
        'type': 'button',
        'name': 'f2'
    },
    {
        'row': 3,
        'col': 11,
        'relative_row': 3,
        'relative_col': 11,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 21,
        'cell_chars_height': 9,
        'cell_name': 'cell5',
        'cell_width': 21,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 5,
        'height': 1,
        'type': 'button',
        'name': 'e6'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 28,
        'cell_chars_width': 4,
        'cell_chars_height': 11,
        'cell_name': 'cl3',
        'cell_width': 4,
        'cell_height': 11,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 2,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 6,
        'height': 1,
        'type': 'button',
        'name': 'd3'
    },
    {
        'row': 5,
        'col': 1,
        'relative_row': 5,
        'relative_col': 1,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 21,
        'cell_chars_height': 9,
        'cell_name': 'cell5',
        'cell_width': 21,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 11,
        'margin_bottom': 3,
        'width': 9,
        'height': 1,
        'type': 'button',
        'name': 'e3'
    },
    {
        'row': 5,
        'col': 11,
        'relative_row': 5,
        'relative_col': 11,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 21,
        'cell_chars_height': 9,
        'cell_name': 'cell5',
        'cell_width': 21,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 3,
        'height': 1,
        'type': 'button',
        'name': 'e7'
    },
    {
        'row': 6,
        'col': 1,
        'relative_row': 6,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 28,
        'cell_chars_width': 4,
        'cell_chars_height': 11,
        'cell_name': 'cl3',
        'cell_width': 4,
        'cell_height': 11,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 2,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 4,
        'height': 1,
        'type': 'button',
        'name': 'd4'
    },
    {
        'row': 6,
        'col': 1,
        'relative_row': 6,
        'relative_col': 1,
        'cell_row': 5,
        'cell_col': 22,
        'cell_chars_width': 5,
        'cell_chars_height': 9,
        'cell_name': 'cl6',
        'cell_width': 5,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 3,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 2,
        'height': 1,
        'type': 'button',
        'name': 'f3'
    },
    {
        'row': 7,
        'col': 1,
        'relative_row': 7,
        'relative_col': 1,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 21,
        'cell_chars_height': 9,
        'cell_name': 'cell5',
        'cell_width': 21,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 11,
        'margin_bottom': 1,
        'width': 9,
        'height': 1,
        'type': 'button',
        'name': 'e4'
    },
    {
        'row': 7,
        'col': 11,
        'relative_row': 7,
        'relative_col': 11,
        'cell_row': 5,
        'cell_col': 0,
        'cell_chars_width': 21,
        'cell_chars_height': 9,
        'cell_name': 'cell5',
        'cell_width': 21,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 9,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 1,
        'height': 1,
        'type': 'button',
        'name': 'e8'
    },
    {
        'row': 8,
        'col': 1,
        'relative_row': 8,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 28,
        'cell_chars_width': 4,
        'cell_chars_height': 11,
        'cell_name': 'cl3',
        'cell_width': 4,
        'cell_height': 11,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 2,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 2,
        'height': 1,
        'type': 'button',
        'name': 'd5'
    },
    {
        'row': 10,
        'col': 1,
        'relative_row': 10,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 28,
        'cell_chars_width': 4,
        'cell_chars_height': 11,
        'cell_name': 'cl3',
        'cell_width': 4,
        'cell_height': 11,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 2,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'button',
        'name': 'd6'
    }
]
