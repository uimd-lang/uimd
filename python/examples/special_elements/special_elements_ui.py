"""Auto-generated UI code for special_elements_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow



class SpecialElementsUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Special Elements Demo\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text."\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_label:\n  type: label\n  text: "Special UI Elements"\n  description: "Window title."\n\ninfo_processing:\n  type: label\n  text: "Processing request..."\n  color: "#ff0000"\n  description: "Animated processing status label."\n\ninfo_loading:\n  type: label\n  text: "Loading data from server..."\n  color: "#00ff00"\n  description: "Animated loading status label."\n\ninfo_complete:\n  type: label\n  text: "Operation completed successfully!"\n  color: "#0000ff"\n  description: "Animated completed status label."\n\ninfo_warning:\n  type: label\n  text: "Warning: Low disk space detected"\n  color: "#ffffff"\n  description: "Animated warning status label."\n\ninfo_multiline:\n  type: label\n  text: "Line one of animated text\\nLine two with different phase\\nLine three final row"\n  description: "Multiline animated label with different text rows."\n\nstatus_label:\n  type: label\n  text: "Status: Idle"\n  description: "Current status text."\n\nnormal_info:\n  type: label\n  text: "Regular static label for comparison"\n  description: "Regular static comparison label."\n\nspeed_label:\n  type: label\n  text: "Fast animation speed demo"\n  description: "Fast animation speed demonstration label."\n\nraw_label:\n  type: label\n  text: "Label tab\\tcontrol\\x1b bell\\x07 combining e\\u0301 wide\\u4e2d"\n  user-select: text\n  description: "Label containing tab, control, combining, and wide characters."\n\nraw_button:\n  type: button\n  title: "Button\\tESC\\x1bWide\\u4e2d"\n  description: "Button title containing raw terminal-sensitive characters."\n\nraw_combo:\n  type: combobox\n  options: ["Plain option", "Tab\\toption", "Esc\\x1boption", "Wide\\u4e2doption"]\n  selected_item: "Tab\\toption"\n  description: "ComboBox options containing raw terminal-sensitive characters."\n\nraw_list:\n  type: listbox\n  options: ["Plain row", "Tab\\trow", "Esc\\x1brow", "Wide\\u4e2drow"]\n  selected_items: ["Tab\\trow"]\n  description: "ListBox options containing raw terminal-sensitive characters."\n\nraw_input:\n  type: textinput\n  value: "Input\\tvalue\\x1bwide\\u4e2d"\n  maxlength: 80\n  description: "Text input containing raw terminal-sensitive characters."\n\nraw_area:\n  type: textarea\n  value: "Python tabs example:\\ndef hello(name):\\n\\tif name:\\n\\t\\tprint(\\"Hello, \\" + name)\\n\\telse:\\n\\t\\tprint(\\"Hello, world!\\")\\n\\nControl\\x1bplaceholder\\nWide\\u4e2dplaceholder"\n  maxlength: 240\n  description: "Text area containing multiline raw terminal-sensitive characters."\n```\n\n## Style\n\n```yaml\ninclude: dark\n@info_processing:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]\n@info_loading:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]\n@info_complete:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]\n@info_warning:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]\n@info_multiline:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]\n@speed_label:\n  text-color-gradient:\n    interval: 35\n    step: 1\n    segment-size: 1\n    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]\ntextinput:\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ntextarea:\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ncombobox:\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\nlistbox:\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------+\n|                                                                    |\n|  header_label....................................................  |\n|                                                                    |\n|  info_processing................  info_loading...................  |\n|  info_complete..................  info_warning...................  |\n|                                                                    |\n|  "Multiline animated text:"                                         |\n|  info_multiline..................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  status_label...................  normal_info....................  |\n|  speed_label.....................................................  |\n|                                                                    |\n|  "Raw read-only text:"                                             |\n|  raw_label.......................................................  |\n|                                                                    |\n|  "Raw button:" raw_button................                          |\n|                                                                    |\n|  "Raw combo:"  raw_combo......................                     |\n|                                                                    |\n|  "Raw list:"                                                       |\n|  raw_list........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  "Text input:" raw_input.........................................  |\n|                                                                    |\n|  "Text area:"                                                      |\n|  raw_area........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n+--------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.'}, 'elements': {'header_label': {'role': 'text', 'description': 'Window title.', 'expose': True}, 'info_processing': {'role': 'text', 'description': 'Animated processing status label.', 'expose': True}, 'info_loading': {'role': 'text', 'description': 'Animated loading status label.', 'expose': True}, 'info_complete': {'role': 'text', 'description': 'Animated completed status label.', 'expose': True}, 'info_warning': {'role': 'text', 'description': 'Animated warning status label.', 'expose': True}, 'info_multiline': {'role': 'text', 'description': 'Multiline animated label with different text rows.', 'expose': True}, 'status_label': {'role': 'text', 'description': 'Current status text.', 'expose': True}, 'normal_info': {'role': 'text', 'description': 'Regular static comparison label.', 'expose': True}, 'speed_label': {'role': 'text', 'description': 'Fast animation speed demonstration label.', 'expose': True}, 'raw_label': {'role': 'text', 'description': 'Label containing tab, control, combining, and wide characters.', 'expose': True}, 'raw_button': {'role': 'action', 'description': 'Button title containing raw terminal-sensitive characters.', 'expose': True}, 'raw_combo': {'role': 'selection', 'description': 'ComboBox options containing raw terminal-sensitive characters.', 'expose': True}, 'raw_list': {'role': 'selection', 'description': 'ListBox options containing raw terminal-sensitive characters.', 'expose': True}, 'raw_input': {'role': 'input', 'description': 'Text input containing raw terminal-sensitive characters.', 'expose': True}, 'raw_area': {'role': 'input', 'description': 'Text area containing multiline raw terminal-sensitive characters.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Special Elements Demo",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.header_label = self.create_element("header_label", "label", text='Special UI Elements')
        self.info_complete = self.create_element("info_complete", "label", text='Operation completed successfully!', color='#0000ff')
        self.info_loading = self.create_element("info_loading", "label", text='Loading data from server...', color='#00ff00')
        self.info_multiline = self.create_element("info_multiline", "label", text='Line one of animated text\nLine two with different phase\nLine three final row')
        self.info_processing = self.create_element("info_processing", "label", text='Processing request...', color='#ff0000')
        self.info_warning = self.create_element("info_warning", "label", text='Warning: Low disk space detected', color='#ffffff')
        self.normal_info = self.create_element("normal_info", "label", text='Regular static label for comparison')
        self.raw_area = self.create_element("raw_area", "textarea", value='Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint("Hello, " + name)\n\telse:\n\t\tprint("Hello, world!")\n\nControl\x1bplaceholder\nWide中placeholder', maxlength=240)
        self.raw_button = self.create_element("raw_button", "button", title='Button\tESC\x1bWide中')
        self.raw_combo = self.create_element("raw_combo", "combobox", options=['Plain option', 'Tab\toption', 'Esc\x1boption', 'Wide中option'], selected_item='Tab\toption')
        self.raw_input = self.create_element("raw_input", "textinput", value='Input\tvalue\x1bwide中', maxlength=80)
        self.raw_label = self.create_element("raw_label", "label", text='Label tab\tcontrol\x1b bell\x07 combining é wide中')
        self.raw_list = self.create_element("raw_list", "listbox", options=['Plain row', 'Tab\trow', 'Esc\x1brow', 'Wide中row'], selected_items=['Tab\trow'])
        self.speed_label = self.create_element("speed_label", "label", text='Fast animation speed demo')
        self.status_label = self.create_element("status_label", "label", text='Status: Idle')
        self._label_5 = self.create_element("_label_5", "label", text='Multiline animated text:')
        self._label_10 = self.create_element("_label_10", "label", text='Raw read-only text:')
        self._label_12 = self.create_element("_label_12", "label", text='Raw button:')
        self._label_14 = self.create_element("_label_14", "label", text='Raw combo:')
        self._label_16 = self.create_element("_label_16", "label", text='Raw list:')
        self._label_18 = self.create_element("_label_18", "label", text='Text input:')
        self._label_20 = self.create_element("_label_20", "label", text='Text area:')
    def on_raw_button_click(self):
        pass

    def on_raw_combo_change(self, value):
        pass

    def on_raw_list_selection_change(self, value):
        pass

    def on_raw_input_change(self, value):
        pass

    def on_raw_input_submit(self, value):
        pass

    def on_raw_area_change(self, value):
        pass

    def on_raw_area_submit(self, value):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.raw_button:
            self.on_raw_button_click()
        elif element is self.raw_input:
            self.on_raw_input_change(value)
        elif element is self.raw_area:
            self.on_raw_area_change(value)
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.raw_input:
            self.on_raw_input_submit(value)
        elif element is self.raw_area:
            self.on_raw_area_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.raw_combo:
            self.on_raw_combo_change(value)
        elif element is self.raw_list:
            self.on_raw_list_selection_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header_label": {
        "type": 'label',
        "text": 'Special UI Elements',
        "description": 'Window title.',
    },
    "info_processing": {
        "type": 'label',
        "text": 'Processing request...',
        "color": '#ff0000',
        "description": 'Animated processing status label.',
    },
    "info_loading": {
        "type": 'label',
        "text": 'Loading data from server...',
        "color": '#00ff00',
        "description": 'Animated loading status label.',
    },
    "info_complete": {
        "type": 'label',
        "text": 'Operation completed successfully!',
        "color": '#0000ff',
        "description": 'Animated completed status label.',
    },
    "info_warning": {
        "type": 'label',
        "text": 'Warning: Low disk space detected',
        "color": '#ffffff',
        "description": 'Animated warning status label.',
    },
    "info_multiline": {
        "type": 'label',
        "text": 'Line one of animated text\nLine two with different phase\nLine three final row',
        "description": 'Multiline animated label with different text rows.',
    },
    "status_label": {
        "type": 'label',
        "text": 'Status: Idle',
        "description": 'Current status text.',
    },
    "normal_info": {
        "type": 'label',
        "text": 'Regular static label for comparison',
        "description": 'Regular static comparison label.',
    },
    "speed_label": {
        "type": 'label',
        "text": 'Fast animation speed demo',
        "description": 'Fast animation speed demonstration label.',
    },
    "raw_label": {
        "type": 'label',
        "text": 'Label tab\tcontrol\x1b bell\x07 combining é wide中',
        "user-select": 'text',
        "description": 'Label containing tab, control, combining, and wide characters.',
    },
    "raw_button": {
        "type": 'button',
        "title": 'Button\tESC\x1bWide中',
        "description": 'Button title containing raw terminal-sensitive characters.',
    },
    "raw_combo": {
        "type": 'combobox',
        "options": ['Plain option', 'Tab\toption', 'Esc\x1boption', 'Wide中option'],
        "selected_item": 'Tab\toption',
        "description": 'ComboBox options containing raw terminal-sensitive characters.',
    },
    "raw_list": {
        "type": 'listbox',
        "options": ['Plain row', 'Tab\trow', 'Esc\x1brow', 'Wide中row'],
        "selected_items": ['Tab\trow'],
        "description": 'ListBox options containing raw terminal-sensitive characters.',
    },
    "raw_input": {
        "type": 'textinput',
        "value": 'Input\tvalue\x1bwide中',
        "maxlength": 80,
        "description": 'Text input containing raw terminal-sensitive characters.',
    },
    "raw_area": {
        "type": 'textarea',
        "value": 'Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint("Hello, " + name)\n\telse:\n\t\tprint("Hello, world!")\n\nControl\x1bplaceholder\nWide中placeholder',
        "maxlength": 240,
        "description": 'Text area containing multiline raw terminal-sensitive characters.',
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
    "@info_processing": {
        "text-color-gradient": {'interval': 70, 'step': 1, 'segment-size': 1, 'colors': ['#00000099', '#00000066', '#00000033', '#00000011', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000011', '#00000033', '#00000066', '#00000099']},
    },
    "@info_loading": {
        "text-color-gradient": {'interval': 70, 'step': 1, 'segment-size': 1, 'colors': ['#00000099', '#00000066', '#00000033', '#00000011', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000011', '#00000033', '#00000066', '#00000099']},
    },
    "@info_complete": {
        "text-color-gradient": {'interval': 70, 'step': 1, 'segment-size': 1, 'colors': ['#00000099', '#00000066', '#00000033', '#00000011', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000011', '#00000033', '#00000066', '#00000099']},
    },
    "@info_warning": {
        "text-color-gradient": {'interval': 70, 'step': 1, 'segment-size': 1, 'colors': ['#00000099', '#00000066', '#00000033', '#00000011', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000011', '#00000033', '#00000066', '#00000099']},
    },
    "@info_multiline": {
        "text-color-gradient": {'interval': 70, 'step': 1, 'segment-size': 1, 'colors': ['#00000099', '#00000066', '#00000033', '#00000011', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000011', '#00000033', '#00000066', '#00000099']},
    },
    "@speed_label": {
        "text-color-gradient": {'interval': 35, 'step': 1, 'segment-size': 1, 'colors': ['#00000099', '#00000066', '#00000033', '#00000011', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000011', '#00000033', '#00000066', '#00000099']},
    },
    "textarea": {
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
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
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 64,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 32,
        'height': 1,
        'type': 'label',
        'name': 'header_label'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 35,
        'margin_bottom': 30,
        'width': 31,
        'height': 1,
        'type': 'label',
        'name': 'info_processing'
    },
    {
        'row': 3,
        'col': 35,
        'relative_row': 3,
        'relative_col': 35,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 30,
        'height': 1,
        'type': 'label',
        'name': 'info_loading'
    },
    {
        'row': 4,
        'col': 2,
        'relative_row': 4,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 35,
        'margin_bottom': 29,
        'width': 31,
        'height': 1,
        'type': 'label',
        'name': 'info_complete'
    },
    {
        'row': 4,
        'col': 35,
        'relative_row': 4,
        'relative_col': 35,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 29,
        'height': 1,
        'type': 'label',
        'name': 'info_warning'
    },
    {
        'row': 6,
        'col': 2,
        'relative_row': 6,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 40,
        'margin_bottom': 27,
        'height': 1,
        'type': 'label',
        'name': '_label_5',
        'text': 'Multiline animated text:'
    },
    {
        'row': 7,
        'col': 2,
        'relative_row': 7,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 64,
        'chars_height': 3,
        'margin_right': 2,
        'margin_bottom': 24,
        'height': 3,
        'type': 'label',
        'name': 'info_multiline'
    },
    {
        'row': 11,
        'col': 2,
        'relative_row': 11,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 35,
        'margin_bottom': 22,
        'width': 31,
        'height': 1,
        'type': 'label',
        'name': 'status_label'
    },
    {
        'row': 11,
        'col': 35,
        'relative_row': 11,
        'relative_col': 35,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 2,
        'margin_right': 2,
        'margin_bottom': 21,
        'height': 2,
        'type': 'label',
        'name': 'normal_info'
    },
    {
        'row': 12,
        'col': 2,
        'relative_row': 12,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 64,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 21,
        'height': 1,
        'type': 'label',
        'name': 'speed_label'
    },
    {
        'row': 14,
        'col': 2,
        'relative_row': 14,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 21,
        'chars_height': 1,
        'margin_right': 45,
        'margin_bottom': 19,
        'height': 1,
        'type': 'label',
        'name': '_label_10',
        'text': 'Raw read-only text:'
    },
    {
        'row': 15,
        'col': 2,
        'relative_row': 15,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 64,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 18,
        'height': 1,
        'type': 'label',
        'name': 'raw_label'
    },
    {
        'row': 17,
        'col': 2,
        'relative_row': 17,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 13,
        'chars_height': 1,
        'margin_right': 53,
        'margin_bottom': 16,
        'width': 13,
        'height': 1,
        'type': 'label',
        'name': '_label_12',
        'text': 'Raw button:'
    },
    {
        'row': 17,
        'col': 16,
        'relative_row': 17,
        'relative_col': 16,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 26,
        'margin_bottom': 16,
        'height': 1,
        'type': 'button',
        'name': 'raw_button'
    },
    {
        'row': 19,
        'col': 2,
        'relative_row': 19,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 54,
        'margin_bottom': 14,
        'width': 12,
        'height': 1,
        'type': 'label',
        'name': '_label_14',
        'text': 'Raw combo:'
    },
    {
        'row': 19,
        'col': 16,
        'relative_row': 19,
        'relative_col': 16,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 31,
        'chars_height': 1,
        'margin_right': 21,
        'margin_bottom': 14,
        'height': 1,
        'type': 'combobox',
        'name': 'raw_combo'
    },
    {
        'row': 21,
        'col': 2,
        'relative_row': 21,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 55,
        'margin_bottom': 12,
        'height': 1,
        'type': 'label',
        'name': '_label_16',
        'text': 'Raw list:'
    },
    {
        'row': 22,
        'col': 2,
        'relative_row': 22,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 64,
        'chars_height': 3,
        'margin_right': 2,
        'margin_bottom': 9,
        'height': 3,
        'type': 'listbox',
        'name': 'raw_list'
    },
    {
        'row': 26,
        'col': 2,
        'relative_row': 26,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 13,
        'chars_height': 1,
        'margin_right': 53,
        'margin_bottom': 7,
        'width': 13,
        'height': 1,
        'type': 'label',
        'name': '_label_18',
        'text': 'Text input:'
    },
    {
        'row': 26,
        'col': 16,
        'relative_row': 26,
        'relative_col': 16,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 50,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 7,
        'height': 1,
        'type': 'textinput',
        'name': 'raw_input'
    },
    {
        'row': 28,
        'col': 2,
        'relative_row': 28,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 54,
        'margin_bottom': 5,
        'height': 1,
        'type': 'label',
        'name': '_label_20',
        'text': 'Text area:'
    },
    {
        'row': 29,
        'col': 2,
        'relative_row': 29,
        'relative_col': 2,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 68,
        'cell_chars_height': 34,
        'cell_width': 68,
        'cell_height': 34,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 64,
        'chars_height': 4,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 4,
        'type': 'textarea',
        'name': 'raw_area'
    }
]
