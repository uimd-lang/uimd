"""Paragraph block for the markdown viewer."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

from paragraph_ui import ParagraphUI


class Paragraph(ParagraphUI):
    def set_text(self, text):
        self.text.text = text
