"""Auto-generated UI code for agent_chat_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'transcript'))
from transcript import Transcript


class AgentChatUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Agent Chat Simulation\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: "Deterministic simulated agent chat for rich transcript and large ScrollView testing."\ntags: [example, simulation, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - transcript/transcript\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: "Agent Chat Simulation"\n  description: "Application title."\n  expose: false\n\nsimulation_badge:\n  type: label\n  text: "SIMULATED · OFFLINE"\n  description: "Clarifies that the transcript performs no real agent or tool work."\n  expose: false\n\nquit_button:\n  type: button\n  title: Quit\n  description: "Close the Agent Chat Simulation example."\n\nstart_button:\n  type: button\n  title: Start\n  description: "Run one hundred deterministic simulated messages over three minutes."\n\npause_button:\n  type: button\n  title: Pause\n  description: "Pause the timed simulation without discarding progress."\n\nnext_step_button:\n  type: button\n  title: "Next step"\n  description: "Apply one deterministic scripted chat step."\n\nreset_button:\n  type: button\n  title: Reset\n  description: "Reset the transcript to its initial message."\n\nload_stress_button:\n  type: button\n  title: "Load 50k"\n  description: "Load an approximately fifty-thousand-row deterministic stress transcript."\n\ntranscript:\n  type: transcript\n  cpp-class: "agent_chat_example::Transcript"\n  cpp-header: transcript/transcript.hpp\n  description: "Scrollable deterministic simulated agent transcript."\n\nstatus:\n  type: label\n  text: "Ready · 1 message"\n  description: "Shows deterministic simulation progress and transcript size."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: "#030712"\n@header:\n  background: "#111827"\n  padding: 0, 1, 0, 1\n@title:\n  color: "#f8fafc"\n  text-align: center\n@simulation_badge:\n  color: "#c4b5fd"\n  text-align: right\n@actions:\n  background: "#172033"\n  padding: 1, 1, 1, 1\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2563eb"\n@load_stress_button:\n  background: "#6d28d9"\n  focus-background: "#7c3aed"\n@quit_button:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n@content:\n  background: "#030712"\n@transcript:\n  background: "#00000000"\n@status:\n  background: "#0f172a"\n  color: "#93c5fd"\n  padding: 0, 1, 0, 1\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**......................... simulation_badge.... quit_button......... |\n+-actions-**-------------------------------------------------------------------+\n| start_button pause_button next_step_button reset_button load_stress_button.. |\n+-content-**-------------------------------------------------------------------+\n| transcript..**.............................................................. |\n| ............................................................................ |\n* *........................................................................... |\n* *........................................................................... |\n| ............................................................................ |\n+------------------------------------------------------------------------------+\n| status..**.................................................................. |\n+------------------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Deterministic simulated agent chat for rich transcript and large ScrollView testing.'}, 'elements': {'title': {'role': 'text', 'description': 'Application title.', 'expose': False}, 'simulation_badge': {'role': 'text', 'description': 'Clarifies that the transcript performs no real agent or tool work.', 'expose': False}, 'quit_button': {'role': 'action', 'description': 'Close the Agent Chat Simulation example.', 'expose': True}, 'start_button': {'role': 'action', 'description': 'Run one hundred deterministic simulated messages over three minutes.', 'expose': True}, 'pause_button': {'role': 'action', 'description': 'Pause the timed simulation without discarding progress.', 'expose': True}, 'next_step_button': {'role': 'action', 'description': 'Apply one deterministic scripted chat step.', 'expose': True}, 'reset_button': {'role': 'action', 'description': 'Reset the transcript to its initial message.', 'expose': True}, 'load_stress_button': {'role': 'action', 'description': 'Load an approximately fifty-thousand-row deterministic stress transcript.', 'expose': True}, 'transcript': {'role': 'text', 'description': 'Scrollable deterministic simulated agent transcript.', 'expose': True}, 'status': {'role': 'text', 'description': 'Shows deterministic simulation progress and transcript size.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Agent Chat Simulation",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.load_stress_button = self.create_element("load_stress_button", "button", title='Load 50k')
        self.next_step_button = self.create_element("next_step_button", "button", title='Next step')
        self.pause_button = self.create_element("pause_button", "button", title='Pause')
        self.quit_button = self.create_element("quit_button", "button", title='Quit')
        self.reset_button = self.create_element("reset_button", "button", title='Reset')
        self.simulation_badge = self.create_element("simulation_badge", "label", text='SIMULATED · OFFLINE')
        self.start_button = self.create_element("start_button", "button", title='Start')
        self.status = self.create_element("status", "label", text='Ready · 1 message')
        self.title_element = self.create_element("title", "label", text='Agent Chat Simulation')
        self.transcript = self.create_element("transcript", "uielement", layout_class=Transcript)
    def on_quit_button_click(self):
        pass

    def on_start_button_click(self):
        pass

    def on_pause_button_click(self):
        pass

    def on_next_step_button_click(self):
        pass

    def on_reset_button_click(self):
        pass

    def on_load_stress_button_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.quit_button:
            self.on_quit_button_click()
        elif element is self.start_button:
            self.on_start_button_click()
        elif element is self.pause_button:
            self.on_pause_button_click()
        elif element is self.next_step_button:
            self.on_next_step_button_click()
        elif element is self.reset_button:
            self.on_reset_button_click()
        elif element is self.load_stress_button:
            self.on_load_stress_button_click()
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
        "text": 'Agent Chat Simulation',
        "description": 'Application title.',
        "expose": False,
    },
    "simulation_badge": {
        "type": 'label',
        "text": 'SIMULATED · OFFLINE',
        "description": 'Clarifies that the transcript performs no real agent or tool work.',
        "expose": False,
    },
    "quit_button": {
        "type": 'button',
        "title": 'Quit',
        "description": 'Close the Agent Chat Simulation example.',
    },
    "start_button": {
        "type": 'button',
        "title": 'Start',
        "description": 'Run one hundred deterministic simulated messages over three minutes.',
    },
    "pause_button": {
        "type": 'button',
        "title": 'Pause',
        "description": 'Pause the timed simulation without discarding progress.',
    },
    "next_step_button": {
        "type": 'button',
        "title": 'Next step',
        "description": 'Apply one deterministic scripted chat step.',
    },
    "reset_button": {
        "type": 'button',
        "title": 'Reset',
        "description": 'Reset the transcript to its initial message.',
    },
    "load_stress_button": {
        "type": 'button',
        "title": 'Load 50k',
        "description": 'Load an approximately fifty-thousand-row deterministic stress transcript.',
    },
    "transcript": {
        "type": 'transcript',
        "cpp-class": 'agent_chat_example::Transcript',
        "cpp-header": 'transcript/transcript.hpp',
        "description": 'Scrollable deterministic simulated agent transcript.',
    },
    "status": {
        "type": 'label',
        "text": 'Ready · 1 message',
        "description": 'Shows deterministic simulation progress and transcript size.',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#030712',
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
        "active-background": '#DDDDDD99',
        "active-color": '#FFFFFF',
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
        "background": '#111827',
        "padding": '0, 1, 0, 1',
    },
    "@title": {
        "color": '#f8fafc',
        "text-align": 'center',
    },
    "@simulation_badge": {
        "color": '#c4b5fd',
        "text-align": 'right',
    },
    "@actions": {
        "background": '#172033',
        "padding": '1, 1, 1, 1',
    },
    "@load_stress_button": {
        "background": '#6d28d9',
        "focus-background": '#7c3aed',
    },
    "@quit_button": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
    },
    "@content": {
        "background": '#030712',
    },
    "@transcript": {
        "background": '#00000000',
    },
    "@status": {
        "background": '#0f172a',
        "color": '#93c5fd',
        "padding": '0, 1, 0, 1',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'actions',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 65,
        'width': 12,
        'height': 1,
        'type': 'button',
        'name': 'start_button'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 10,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_width': 78,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 76,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'label',
        'name': 'status'
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
        'chars_width': 34,
        'chars_height': 1,
        'margin_right': 43,
        'height': 1,
        'type': 'label',
        'name': 'title'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 4,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 5,
        'cell_name': 'content',
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 76,
        'chars_height': 5,
        'margin_right': 1,
        'type': 'transcript',
        'name': 'transcript'
    },
    {
        'row': 0,
        'col': 14,
        'relative_col': 14,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'actions',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 52,
        'width': 12,
        'height': 1,
        'type': 'button',
        'name': 'pause_button'
    },
    {
        'row': 0,
        'col': 27,
        'relative_col': 27,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'actions',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 16,
        'chars_height': 1,
        'margin_right': 35,
        'width': 16,
        'height': 1,
        'type': 'button',
        'name': 'next_step_button'
    },
    {
        'row': 0,
        'col': 36,
        'relative_col': 36,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 22,
        'width': 20,
        'height': 1,
        'type': 'label',
        'name': 'simulation_badge'
    },
    {
        'row': 0,
        'col': 44,
        'relative_col': 44,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'actions',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 12,
        'chars_height': 1,
        'margin_right': 22,
        'width': 12,
        'height': 1,
        'type': 'button',
        'name': 'reset_button'
    },
    {
        'row': 0,
        'col': 57,
        'relative_col': 57,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'actions',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 1,
        'width': 20,
        'height': 1,
        'type': 'button',
        'name': 'load_stress_button'
    },
    {
        'row': 0,
        'col': 57,
        'relative_col': 57,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 20,
        'chars_height': 1,
        'margin_right': 1,
        'width': 20,
        'height': 1,
        'type': 'button',
        'name': 'quit_button'
    }
]
