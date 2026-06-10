"""Markdown table block for the markdown viewer."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

from table_block_ui import TableBlockUI


class TableBlock(TableBlockUI):
    def set_table(self, text):
        self.table.text = text
