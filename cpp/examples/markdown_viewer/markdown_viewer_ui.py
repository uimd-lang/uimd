"""Auto-generated UI code for markdown_viewer_ui.py - DO NOT EDIT MANUALLY."""

from runtime import UIWindow

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'document_view'))
from document_view import DocumentView


class MarkdownViewerUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Markdown Viewer\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents."\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - document_view\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: "Markdown Viewer"\n  description: "Application title."\n  expose: false\n\ndocs_label:\n  type: label\n  text: Documents\n  description: "Label for the document list."\n\ndocs:\n  type: listbox\n  options: [overview.markdown, styles.markdown, tables.markdown, code.markdown]\n  selected_items: [overview.markdown]\n  multiple: "false"\n  description: "Document listbox used to select the markdown document to render."\n\nup_btn:\n  type: button\n  title: up\n  description: "Scroll the rendered document upward."\n\ndown_btn:\n  type: button\n  title: down\n  description: "Scroll the rendered document downward."\n\nviewer:\n  type: document_view\n  cpp-class: "markdown_viewer_example::DocumentView"\n  cpp-header: document_view/document_view.hpp\n  description: "Scrollable rendered markdown document area."\n\nclose_btn:\n  type: button\n  title: X\n  width: 9\n  description: "Close the markdown viewer."\n\nstatus:\n  type: label\n  text: Ready\n  description: "Shows the current document and scroll state."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  color: "#ffffff"\n@title:\n  background: "#223a5a"\n@sidebar:\n  background: "#101a2b"\n@content:\n  background: "#000000"\n@viewer:\n  background: "#000000"\n  padding: 1, 2, 1, 2\n@docs:\n  background: "#ffffff11"\n  color: "#cbd5e1"\n  focus-background: "#ffffff22"\n  edit-background: "#00000000"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\nbutton:\n  background: "#334155"\n  color: "#e2e8f0"\n  focus-background: "#2b5f9f"\n@close_btn:\n  background: "#7f1d1d"\n  color: "#e2e8f0"\n  focus-background: "#dc2626"\n@status:\n  color: "#93c5fd"\n  background: "#111827"\n```\n\n## User Interface\n\n```ui\n+-title-**----------------------------------------------------------+\n|header..**................................................close_btn|\n+-sidebar------------------+-content-**-----------------------------+\n|                          |viewer..**..............................|\n| docs_label...**......... |........................................|\n| docs.........**......... |*.......................................|\n| ........................ |*.......................................|\n| #....................... |........................................|\n| #....................... |........................................|\n|                          |........................................|\n| up_btn.... down_btn..... |........................................|\n*                          |........................................|\n*                          |........................................|\n|                          |........................................|\n+--------------------------+----------------------------------------+\n| status..**....................................................... |\n+-------------------------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.'}, 'elements': {'header': {'role': 'text', 'description': 'Application title.', 'expose': False}, 'docs_label': {'role': 'text', 'description': 'Label for the document list.', 'expose': True}, 'docs': {'role': 'selection', 'description': 'Document listbox used to select the markdown document to render.', 'expose': True}, 'up_btn': {'role': 'action', 'description': 'Scroll the rendered document upward.', 'expose': True}, 'down_btn': {'role': 'action', 'description': 'Scroll the rendered document downward.', 'expose': True}, 'viewer': {'role': 'text', 'description': 'Scrollable rendered markdown document area.', 'expose': True}, 'close_btn': {'role': 'action', 'description': 'Close the markdown viewer.', 'expose': True}, 'status': {'role': 'text', 'description': 'Shows the current document and scroll state.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Markdown Viewer",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )

        self.close_btn = self.create_element("close_btn", "button", title='X')
        self.docs = self.create_element("docs", "listbox", options=['overview.markdown', 'styles.markdown', 'tables.markdown', 'code.markdown'], selected_items=['overview.markdown'], multiple=False)
        self.docs_label = self.create_element("docs_label", "label", text='Documents')
        self.down_btn = self.create_element("down_btn", "button", title='down')
        self.header = self.create_element("header", "label", text='Markdown Viewer')
        self.status = self.create_element("status", "label", text='Ready')
        self.up_btn = self.create_element("up_btn", "button", title='up')
        self.viewer = self.create_element("viewer", "uielement", layout_class=DocumentView)
    def on_docs_selection_change(self, value):
        pass

    def on_up_btn_click(self):
        pass

    def on_down_btn_click(self):
        pass

    def on_close_btn_click(self):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.up_btn:
            self.on_up_btn_click()
        elif element is self.down_btn:
            self.on_down_btn_click()
        elif element is self.close_btn:
            self.on_close_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.docs:
            self.on_docs_selection_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "header": {
        "type": 'label',
        "text": 'Markdown Viewer',
        "description": 'Application title.',
        "expose": False,
    },
    "docs_label": {
        "type": 'label',
        "text": 'Documents',
        "description": 'Label for the document list.',
    },
    "docs": {
        "type": 'listbox',
        "options": ['overview.markdown', 'styles.markdown', 'tables.markdown', 'code.markdown'],
        "selected_items": ['overview.markdown'],
        "multiple": 'false',
        "description": 'Document listbox used to select the markdown document to render.',
    },
    "up_btn": {
        "type": 'button',
        "title": 'up',
        "description": 'Scroll the rendered document upward.',
    },
    "down_btn": {
        "type": 'button',
        "title": 'down',
        "description": 'Scroll the rendered document downward.',
    },
    "viewer": {
        "type": 'document_view',
        "cpp-class": 'markdown_viewer_example::DocumentView',
        "cpp-header": 'document_view/document_view.hpp',
        "description": 'Scrollable rendered markdown document area.',
    },
    "close_btn": {
        "type": 'button',
        "title": 'X',
        "width": 9,
        "description": 'Close the markdown viewer.',
    },
    "status": {
        "type": 'label',
        "text": 'Ready',
        "description": 'Shows the current document and scroll state.',
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
    "@header": {
        "text-align": 'center',
        "color": '#ffffff',
    },
    "@title": {
        "background": '#223a5a',
    },
    "@sidebar": {
        "background": '#101a2b',
    },
    "@content": {
        "background": '#000000',
    },
    "@viewer": {
        "background": '#000000',
        "padding": '1, 2, 1, 2',
    },
    "@docs": {
        "background": '#ffffff11',
        "color": '#cbd5e1',
        "focus-background": '#ffffff22',
        "edit-background": '#00000000',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
    "@close_btn": {
        "background": '#7f1d1d',
        "color": '#e2e8f0',
        "focus-background": '#dc2626',
    },
    "@status": {
        "color": '#93c5fd',
        "background": '#111827',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 0,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 67,
        'cell_chars_height': 1,
        'cell_name': 'title',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 58,
        'chars_height': 1,
        'margin_right': 9,
        'height': 1,
        'type': 'label',
        'name': 'header'
    },
    {
        'row': 0,
        'col': 0,
        'cell_row': 2,
        'cell_col': 27,
        'cell_chars_width': 40,
        'cell_chars_height': 11,
        'cell_name': 'content',
        'cell_height': 11,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 40,
        'chars_height': 11,
        'type': 'document_view',
        'name': 'viewer'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 14,
        'cell_col': 0,
        'cell_chars_width': 67,
        'cell_chars_height': 1,
        'cell_width': 67,
        'cell_height': 1,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 65,
        'chars_height': 1,
        'margin_right': 1,
        'height': 1,
        'type': 'label',
        'name': 'status'
    },
    {
        'row': 0,
        'col': 58,
        'relative_col': 58,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 67,
        'cell_chars_height': 1,
        'cell_name': 'title',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 9,
        'chars_height': 1,
        'width': 9,
        'height': 1,
        'type': 'button',
        'name': 'close_btn'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 11,
        'cell_name': 'sidebar',
        'cell_width': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 24,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 9,
        'height': 1,
        'type': 'label',
        'name': 'docs_label'
    },
    {
        'row': 2,
        'col': 1,
        'relative_row': 2,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 11,
        'cell_name': 'sidebar',
        'cell_width': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'height_mode': 'fit-content',
        'chars_width': 24,
        'chars_height': 4,
        'margin_right': 1,
        'margin_bottom': 5,
        'type': 'listbox',
        'name': 'docs'
    },
    {
        'row': 7,
        'col': 1,
        'relative_row': 7,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 11,
        'cell_name': 'sidebar',
        'cell_width': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'chars_width': 10,
        'chars_height': 1,
        'margin_right': 15,
        'margin_bottom': 3,
        'width': 10,
        'height': 1,
        'type': 'button',
        'name': 'up_btn'
    },
    {
        'row': 7,
        'col': 12,
        'relative_row': 7,
        'relative_col': 12,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 26,
        'cell_chars_height': 11,
        'cell_name': 'sidebar',
        'cell_width': 26,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 13,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 3,
        'height': 1,
        'type': 'button',
        'name': 'down_btn'
    }
]
