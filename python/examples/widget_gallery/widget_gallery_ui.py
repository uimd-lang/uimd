"""Auto-generated UI code for widget_gallery_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow



class WidgetGalleryUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Widget Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons."\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: "Widget Gallery"\n  description: "Window title."\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: "Close the widget gallery."\n\nplain_label:\n  type: label\n  text: "Plain label with inherited theme colors."\n  description: "Plain label using inherited theme colors."\n\nselectable_label:\n  type: label\n  text: "Select this text with the mouse to test label selection."\n  description: "Selectable label used to test mouse text selection."\n\nspans:\n  type: spanlabel\n  text: "span label"\n  description: "Span label demonstrating mixed foreground and background styling."\n\nanimation:\n  type: label\n  text: "Animated label shows gradient-based rendering."\n  description: "Animated gradient label."\n\nname_label:\n  type: label\n  text: Name\n  description: "Label for the name input."\n\nname_input:\n  type: textinput\n  value: "Ada Lovelace"\n  maxlength: 80\n  description: "Editable name text input."\n\ncount_label:\n  type: label\n  text: Count\n  description: "Label for the count input."\n\ncount_input:\n  type: numberinput\n  value: 3\n  step_size: 1\n  min_value: 0\n  max_value: 99\n  format_str: 0\n  description: "Numeric count input with min, max, and step constraints."\n\nenabled_check:\n  type: checkbox\n  title: "Feature enabled"\n  value: 1\n  description: "Toggle for the feature enabled state."\n\ntheme_label:\n  type: label\n  text: Theme\n  description: "Label for the theme selector."\n\ntheme_combo:\n  type: combobox\n  options: [Dark, Light, Solarized]\n  selected_item: Dark\n  description: "Combobox selecting a visual theme value."\n\nmode_label:\n  type: label\n  text: Mode\n  description: "Label for the mode list."\n\nmode_list:\n  type: listbox\n  options: [Preview, Edit, Review, Publish, Archive]\n  selected_items: [Preview]\n  multiple: "false"\n  description: "Single-select listbox choosing the current workflow mode."\n\napply_btn:\n  type: button\n  title: Apply\n  description: "Apply the current widget values."\n\nreset_btn:\n  type: button\n  title: Reset\n  description: "Reset widget values to defaults."\n\nsummary:\n  type: label\n  text: "values:\\n  name: Ada Lovelace\\n  count: 3\\n  theme: Dark\\n  mode: Preview\\n  enabled: true"\n  description: "Read-only summary of current widget values."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: "#18324f"\n  color: "#ffffff"\n@close_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n  color: "#ffffff"\n@title:\n  background: "#18324f"\n@selectable_label:\n  user-select: text\n  color: "#a7f3d0"\n@spans:\n  user-select: text\n  color: "#cbd5e1"\n@animation:\n  color: "#fbbf24"\n  text-color-gradient:\n    interval: 100\n    step: 1\n    segment-size: 1\n    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]\ntextinput:\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\nnumberinput:\n  cursor-background: "#facc15"\n  cursor-color: "#111827"\ncheckbox:\n  color: "#d1d5db"\n  checked-color: "#22c55e"\n  unchecked-color: "#f97316"\n@summary:\n  background: "#0d1524"\n  color: "#dbeafe"\n```\n\n## User Interface\n\n```ui\n+-title------------------------------------------------+\n| header................................ close_btn.....|\n+-label_section----------------------------------------+\n|                                                      |\n|  plain_label.......................................  |\n|                                                      |\n|  selectable_label..................................  |\n|                                                      |\n|  spans.............................................  |\n|                                                      |\n|  animation.........................................  |\n|                                                      |\n+-control_section-----------+--section2----------------+\n|  name_label               |  theme_label             |\n|  name_input.............  |   theme_combo..........  |\n|                           |                          |\n|  count_label              |  mode_label              |\n|  count_input............  |  mode_list.............  |\n|                           |  ......................  |\n|  enabled_check..........  |  ......................  |\n|                           |  ......................  |\n|                           |  ......................  |\n+---------------------------+--------------------------+\n|                                                      |\n|  summary...........................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|                                                      |\n|  apply_btn..............  reset_btn................  |\n|                                                      |\n+------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.'}, 'elements': {'header': {'role': 'text', 'description': 'Window title.', 'expose': False}, 'close_btn': {'role': 'action', 'description': 'Close the widget gallery.', 'expose': True}, 'plain_label': {'role': 'text', 'description': 'Plain label using inherited theme colors.', 'expose': True}, 'selectable_label': {'role': 'text', 'description': 'Selectable label used to test mouse text selection.', 'expose': True}, 'spans': {'role': 'text', 'description': 'Span label demonstrating mixed foreground and background styling.', 'expose': True}, 'animation': {'role': 'text', 'description': 'Animated gradient label.', 'expose': True}, 'name_label': {'role': 'text', 'description': 'Label for the name input.', 'expose': True}, 'name_input': {'role': 'input', 'description': 'Editable name text input.', 'expose': True}, 'count_label': {'role': 'text', 'description': 'Label for the count input.', 'expose': True}, 'count_input': {'role': 'input', 'description': 'Numeric count input with min, max, and step constraints.', 'expose': True}, 'enabled_check': {'role': 'toggle', 'description': 'Toggle for the feature enabled state.', 'expose': True}, 'theme_label': {'role': 'text', 'description': 'Label for the theme selector.', 'expose': True}, 'theme_combo': {'role': 'selection', 'description': 'Combobox selecting a visual theme value.', 'expose': True}, 'mode_label': {'role': 'text', 'description': 'Label for the mode list.', 'expose': True}, 'mode_list': {'role': 'selection', 'description': 'Single-select listbox choosing the current workflow mode.', 'expose': True}, 'apply_btn': {'role': 'action', 'description': 'Apply the current widget values.', 'expose': True}, 'reset_btn': {'role': 'action', 'description': 'Reset widget values to defaults.', 'expose': True}, 'summary': {'role': 'text', 'description': 'Read-only summary of current widget values.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Widget Gallery",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.animation = self.create_element("animation", "label", text='Animated label shows gradient-based rendering.')
        self.apply_btn = self.create_element("apply_btn", "button", title='Apply')
        self.close_btn = self.create_element("close_btn", "button", title='Quit')
        self.count_input = self.create_element("count_input", "numberinput", value=3, step_size=1, min_value=0, max_value=99)
        self.count_label = self.create_element("count_label", "label", text='Count')
        self.enabled_check = self.create_element("enabled_check", "checkbox", value=1, title='Feature enabled')
        self.header = self.create_element("header", "label", text='Widget Gallery')
        self.mode_label = self.create_element("mode_label", "label", text='Mode')
        self.mode_list = self.create_element("mode_list", "listbox", options=['Preview', 'Edit', 'Review', 'Publish', 'Archive'], selected_items=['Preview'], multiple=False)
        self.name_input = self.create_element("name_input", "textinput", value='Ada Lovelace', maxlength=80)
        self.name_label = self.create_element("name_label", "label", text='Name')
        self.plain_label = self.create_element("plain_label", "label", text='Plain label with inherited theme colors.')
        self.reset_btn = self.create_element("reset_btn", "button", title='Reset')
        self.selectable_label = self.create_element("selectable_label", "label", text='Select this text with the mouse to test label selection.')
        self.spans = self.create_element("spans", "spanlabel", text='span label')
        self.summary = self.create_element("summary", "label", text='values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true')
        self.theme_combo = self.create_element("theme_combo", "combobox", options=['Dark', 'Light', 'Solarized'], selected_item='Dark')
        self.theme_label = self.create_element("theme_label", "label", text='Theme')
    def on_close_btn_click(self):
        pass

    def on_name_input_change(self, value):
        pass

    def on_name_input_submit(self, value):
        pass

    def on_count_input_change(self, value):
        pass

    def on_count_input_submit(self, value):
        pass

    def on_enabled_check_change(self, value):
        pass

    def on_theme_combo_change(self, value):
        pass

    def on_mode_list_selection_change(self, value):
        pass

    def on_apply_btn_click(self):
        pass

    def on_reset_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.close_btn:
            self.on_close_btn_click()
        elif element is self.name_input:
            self.on_name_input_change(value)
        elif element is self.count_input:
            self.on_count_input_change(value)
        elif element is self.enabled_check:
            self.on_enabled_check_change(value)
        elif element is self.apply_btn:
            self.on_apply_btn_click()
        elif element is self.reset_btn:
            self.on_reset_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        if element is self.name_input:
            self.on_name_input_submit(value)
        elif element is self.count_input:
            self.on_count_input_submit(value)
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.theme_combo:
            self.on_theme_combo_change(value)
        elif element is self.mode_list:
            self.on_mode_list_selection_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header": {
        "type": 'label',
        "text": 'Widget Gallery',
        "description": 'Window title.',
        "expose": False,
    },
    "close_btn": {
        "type": 'button',
        "title": 'Quit',
        "description": 'Close the widget gallery.',
    },
    "plain_label": {
        "type": 'label',
        "text": 'Plain label with inherited theme colors.',
        "description": 'Plain label using inherited theme colors.',
    },
    "selectable_label": {
        "type": 'label',
        "text": 'Select this text with the mouse to test label selection.',
        "description": 'Selectable label used to test mouse text selection.',
    },
    "spans": {
        "type": 'spanlabel',
        "text": 'span label',
        "description": 'Span label demonstrating mixed foreground and background styling.',
    },
    "animation": {
        "type": 'label',
        "text": 'Animated label shows gradient-based rendering.',
        "description": 'Animated gradient label.',
    },
    "name_label": {
        "type": 'label',
        "text": 'Name',
        "description": 'Label for the name input.',
    },
    "name_input": {
        "type": 'textinput',
        "value": 'Ada Lovelace',
        "maxlength": 80,
        "description": 'Editable name text input.',
    },
    "count_label": {
        "type": 'label',
        "text": 'Count',
        "description": 'Label for the count input.',
    },
    "count_input": {
        "type": 'numberinput',
        "value": 3,
        "step_size": 1,
        "min_value": 0,
        "max_value": 99,
        "format_str": 0,
        "description": 'Numeric count input with min, max, and step constraints.',
    },
    "enabled_check": {
        "type": 'checkbox',
        "title": 'Feature enabled',
        "value": 1,
        "description": 'Toggle for the feature enabled state.',
    },
    "theme_label": {
        "type": 'label',
        "text": 'Theme',
        "description": 'Label for the theme selector.',
    },
    "theme_combo": {
        "type": 'combobox',
        "options": ['Dark', 'Light', 'Solarized'],
        "selected_item": 'Dark',
        "description": 'Combobox selecting a visual theme value.',
    },
    "mode_label": {
        "type": 'label',
        "text": 'Mode',
        "description": 'Label for the mode list.',
    },
    "mode_list": {
        "type": 'listbox',
        "options": ['Preview', 'Edit', 'Review', 'Publish', 'Archive'],
        "selected_items": ['Preview'],
        "multiple": 'false',
        "description": 'Single-select listbox choosing the current workflow mode.',
    },
    "apply_btn": {
        "type": 'button',
        "title": 'Apply',
        "description": 'Apply the current widget values.',
    },
    "reset_btn": {
        "type": 'button',
        "title": 'Reset',
        "description": 'Reset widget values to defaults.',
    },
    "summary": {
        "type": 'label',
        "text": 'values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true',
        "description": 'Read-only summary of current widget values.',
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
        "cursor-background": '#facc15',
        "cursor-color": '#111827',
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
        "color": '#d1d5db',
        "disabled-color": '#64748b',
        "focus-background": '#ffffff44',
        "focus-color": '#ffffff',
        "checked-background": 'transparent',
        "checked-color": '#22c55e',
        "unchecked-background": 'transparent',
        "unchecked-color": '#f97316',
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
        "background": '#18324f',
        "color": '#ffffff',
    },
    "@close_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
        "color": '#ffffff',
    },
    "@title": {
        "background": '#18324f',
    },
    "@selectable_label": {
        "user-select": 'text',
        "color": '#a7f3d0',
    },
    "@spans": {
        "user-select": 'text',
        "color": '#cbd5e1',
    },
    "@animation": {
        "color": '#fbbf24',
        "text-color-gradient": {'interval': 100, 'step': 1, 'segment-size': 1, 'colors': ['#00000099', '#00000066', '#00000033', '#00000011', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000000', '#00000011', '#00000033', '#00000066', '#00000099']},
    },
    "@summary": {
        "background": '#0d1524',
        "color": '#dbeafe',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 1,
        'cell_name': 'title',
        'cell_width': 54,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 38,
        'chars_height': 1,
        'margin_right': 15,
        'width': 38,
        'height': 1,
        'type': 'label',
        'name': 'header'
    },
    {
        'row': 0,
        'col': 2,
        'relative_col': 2,
        'cell_row': 12,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 9,
        'cell_name': 'control_section',
        'cell_width': 27,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 15,
        'margin_bottom': 8,
        'height': 1,
        'type': 'label',
        'name': 'name_label'
    },
    {
        'row': 0,
        'col': 2,
        'relative_col': 2,
        'cell_row': 12,
        'cell_col': 28,
        'cell_chars_width': 26,
        'cell_chars_height': 9,
        'cell_name': 'section2',
        'cell_width': 26,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 13,
        'margin_bottom': 8,
        'height': 1,
        'type': 'label',
        'name': 'theme_label'
    },
    {
        'row': 0,
        'col': 40,
        'relative_col': 40,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 1,
        'cell_name': 'title',
        'cell_width': 54,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 14,
        'chars_height': 1,
        'height': 1,
        'type': 'button',
        'name': 'close_btn'
    },
    {
        'row': 1,
        'col': 2,
        'relative_row': 1,
        'relative_col': 2,
        'cell_row': 12,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 9,
        'cell_name': 'control_section',
        'cell_width': 27,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 23,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 7,
        'height': 1,
        'type': 'textinput',
        'name': 'name_input'
    },
    {
        'row': 1,
        'col': 2,
        'relative_row': 1,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 9,
        'cell_name': 'label_section',
        'cell_width': 54,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 50,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 7,
        'height': 1,
        'type': 'label',
        'name': 'plain_label'
    },
    {
        'row': 1,
        'col': 2,
        'relative_row': 1,
        'relative_col': 2,
        'cell_row': 22,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 10,
        'cell_width': 54,
        'cell_height': 10,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 50,
        'chars_height': 6,
        'margin_right': 2,
        'margin_bottom': 3,
        'height': 6,
        'type': 'label',
        'name': 'summary'
    },
    {
        'row': 1,
        'col': 3,
        'relative_row': 1,
        'relative_col': 3,
        'cell_row': 12,
        'cell_col': 28,
        'cell_chars_width': 26,
        'cell_chars_height': 9,
        'cell_name': 'section2',
        'cell_width': 26,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 21,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 7,
        'height': 1,
        'type': 'combobox',
        'name': 'theme_combo'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 12,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 9,
        'cell_name': 'control_section',
        'cell_width': 27,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 14,
        'margin_bottom': 5,
        'height': 1,
        'type': 'label',
        'name': 'count_label'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 12,
        'cell_col': 28,
        'cell_chars_width': 26,
        'cell_chars_height': 9,
        'cell_name': 'section2',
        'cell_width': 26,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 14,
        'margin_bottom': 5,
        'height': 1,
        'type': 'label',
        'name': 'mode_label'
    },
    {
        'row': 3,
        'col': 2,
        'relative_row': 3,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 9,
        'cell_name': 'label_section',
        'cell_width': 54,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 50,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 5,
        'height': 1,
        'type': 'label',
        'name': 'selectable_label'
    },
    {
        'row': 4,
        'col': 2,
        'relative_row': 4,
        'relative_col': 2,
        'cell_row': 12,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 9,
        'cell_name': 'control_section',
        'cell_width': 27,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 23,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 4,
        'height': 1,
        'type': 'numberinput',
        'name': 'count_input'
    },
    {
        'row': 4,
        'col': 2,
        'relative_row': 4,
        'relative_col': 2,
        'cell_row': 12,
        'cell_col': 28,
        'cell_chars_width': 26,
        'cell_chars_height': 9,
        'cell_name': 'section2',
        'cell_width': 26,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 22,
        'chars_height': 5,
        'margin_right': 2,
        'height': 5,
        'type': 'listbox',
        'name': 'mode_list'
    },
    {
        'row': 5,
        'col': 2,
        'relative_row': 5,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 9,
        'cell_name': 'label_section',
        'cell_width': 54,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 50,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 3,
        'height': 1,
        'type': 'spanlabel',
        'name': 'spans'
    },
    {
        'row': 6,
        'col': 2,
        'relative_row': 6,
        'relative_col': 2,
        'cell_row': 12,
        'cell_col': 0,
        'cell_chars_width': 27,
        'cell_chars_height': 9,
        'cell_name': 'control_section',
        'cell_width': 27,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 23,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 2,
        'height': 1,
        'type': 'checkbox',
        'name': 'enabled_check'
    },
    {
        'row': 7,
        'col': 2,
        'relative_row': 7,
        'relative_col': 2,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 9,
        'cell_name': 'label_section',
        'cell_width': 54,
        'cell_height': 9,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 50,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'label',
        'name': 'animation'
    },
    {
        'row': 8,
        'col': 2,
        'relative_row': 8,
        'relative_col': 2,
        'cell_row': 22,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 10,
        'cell_width': 54,
        'cell_height': 10,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'chars_width': 23,
        'chars_height': 1,
        'margin_right': 29,
        'margin_bottom': 1,
        'width': 23,
        'height': 1,
        'type': 'button',
        'name': 'apply_btn'
    },
    {
        'row': 8,
        'col': 27,
        'relative_row': 8,
        'relative_col': 27,
        'cell_row': 22,
        'cell_col': 0,
        'cell_chars_width': 54,
        'cell_chars_height': 10,
        'cell_width': 54,
        'cell_height': 10,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 25,
        'chars_height': 1,
        'margin_right': 2,
        'margin_bottom': 1,
        'height': 1,
        'type': 'button',
        'name': 'reset_btn'
    }
]
