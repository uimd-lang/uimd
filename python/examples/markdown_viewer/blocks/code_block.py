"""Code block for the markdown viewer."""

import keyword
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

from code_block_ui import CodeBlockUI


KEYWORD_RE = re.compile(r"\b(" + "|".join(keyword.kwlist) + r")\b")


class CodeBlock(CodeBlockUI):
    def set_code(self, lang, source):
        self.lang.text = lang or "text"
        self.code.set_spans(_highlight(source))


def _highlight(source):
    spans = []
    cursor = 0
    for match in KEYWORD_RE.finditer(source):
        if match.start() > cursor:
            spans.append((source[cursor:match.start()], None))
        spans.append((match.group(0), "#c084fc"))
        cursor = match.end()
    if cursor < len(source):
        spans.append((source[cursor:], None))
    return spans
