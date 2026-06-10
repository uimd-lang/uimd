"""Heading block for the markdown viewer."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

from heading_ui import HeadingUI


HEADING_COLORS = {
    1: "#facc15",
    2: "#93c5fd",
    3: "#a7f3d0",
}


class Heading(HeadingUI):
    def set_heading(self, level, text):
        marker = "#" * max(1, min(3, int(level)))
        color = HEADING_COLORS.get(max(1, min(3, int(level))), "#dbeafe")
        self.heading_text.set_spans([
            (marker, "#64748b"),
            (" ", None),
            (text, color),
        ])
