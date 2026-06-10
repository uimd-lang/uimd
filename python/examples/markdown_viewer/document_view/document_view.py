"""Scrollable document view used by markdown_viewer."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(_here))))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "blocks"))

from document_view_ui import DocumentViewUI
from paragraph import Paragraph
from heading import Heading
from code_block import CodeBlock
from table_block import TableBlock


class DocumentView(DocumentViewUI):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._auto_scroll = False

    def set_blocks(self, blocks):
        self.clear_children()
        for block in blocks:
            child = self._make_child(block)
            child.parent = self
            child._app = self._app
            child.mode = "expand_width"
            child.open()
            self.add_child(child)
        self.scroll_to_top()
        self.invalidate_layout()

    def _make_child(self, block):
        kind = block["kind"]
        if kind == "heading":
            child = Heading()
            child.set_heading(block["level"], block["text"])
            return child
        if kind == "code":
            child = CodeBlock()
            child.set_code(block["lang"], block["text"])
            return child
        if kind == "table":
            child = TableBlock()
            child.set_table(block["text"])
            return child
        child = Paragraph()
        child.set_text(block["text"])
        return child

    def _render_child(self, child, width):
        _natural_width, natural_height = child.get_natural_size()
        child.resize(width, max(1, natural_height))
        if child._resolved_cells_by_name:
            actual_height = max(
                info["row"] + info["height"]
                for info in child._resolved_cells_by_name.values()
            )
            if actual_height > child._window_height:
                child.resize(width, actual_height)
        return child.render()
