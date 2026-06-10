"""Local Markdown viewer example."""

import os
import re
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(_here)))
sys.path.insert(0, os.path.join(_here, "document_view"))

from runtime import UIApplication
from markdown_viewer_ui import MarkdownViewerUI


DOCS_DIR = os.path.join(_here, "docs")
TABLE_SEPARATOR_RE = re.compile(r"^\|[\s\-:|]+\|$")


class MarkdownViewer(MarkdownViewerUI):
    def open(self):
        super().open()
        self._documents = self._load_documents()
        self.docs._options = list(self._documents)
        if self._documents:
            self.docs.selected_items = [list(self._documents)[0]]
        self._show_selected()
        self.set_focus(self.docs)
        self._enter_edit_mode()

    def on_close_btn_click(self):
        self._close_app()

    def on_up_btn_click(self):
        self._scroll_viewer("up")

    def on_down_btn_click(self):
        self._scroll_viewer("down")

    def on_docs_selection_change(self, value):
        self._show_selected()

    def focused(self, element, value):
        if not value:
            return
        if element == self.viewer:
            self.status.text = "viewer focus - Enter scroll mode, Esc navigation, arrows/PageUp/PageDown/Home/End scroll"
        elif element in (self.up_btn, self.down_btn):
            self.status.text = "buttons scroll the viewer; Right focuses viewer"

    def handle_key(self, key):
        if key in ("q", "Q"):
            self._close_app()
            return True
        restoring_docs = key == "Escape" and self._focused_element is self.docs and self._edit_mode
        handled = super().handle_key(key)
        if restoring_docs:
            self._show_selected()
        return handled

    def _scroll_viewer(self, direction):
        key = "ArrowUp" if direction == "up" else "ArrowDown"
        self.viewer.handle_key(key)

    def _load_documents(self):
        docs = {}
        preferred = ["overview.markdown", "styles.markdown", "tables.markdown", "code.markdown"]
        names = [name for name in preferred if os.path.exists(os.path.join(DOCS_DIR, name))]
        names.extend(
            name for name in sorted(os.listdir(DOCS_DIR))
            if name.endswith(".markdown") and name not in names
        )
        for name in names:
            if not name.endswith(".markdown"):
                continue
            path = os.path.join(DOCS_DIR, name)
            with open(path, "r", encoding="utf-8") as handle:
                docs[name] = handle.read()
        return docs

    def _show_selected(self):
        name = self.docs.selected_item
        if not name and self._documents:
            name = list(self._documents)[0]
            self.docs.selected_items = [name]
        text = self._documents.get(name, "")
        blocks = parse_markdown(text)
        self.viewer.set_blocks(blocks)
        self.status.text = f"{name or 'no document'} - {len(blocks)} blocks - arrows preview"

    def _close_app(self):
        if self._app is not None:
            self._app._running = False


def parse_markdown(text):
    lines = text.splitlines()
    blocks = []
    paragraph = []
    index = 0

    def flush_paragraph():
        if paragraph:
            blocks.append({"kind": "paragraph", "text": "\n".join(paragraph).strip()})
            paragraph.clear()

    while index < len(lines):
        line = lines[index]
        if not line.strip():
            flush_paragraph()
            index += 1
            continue
        if line.startswith("```"):
            flush_paragraph()
            lang = line.strip("`").strip() or "text"
            index += 1
            code_lines = []
            while index < len(lines) and not lines[index].startswith("```"):
                code_lines.append(lines[index])
                index += 1
            if index < len(lines):
                index += 1
            blocks.append({"kind": "code", "lang": lang, "text": "\n".join(code_lines)})
            continue
        if line.startswith("#"):
            flush_paragraph()
            level = len(line) - len(line.lstrip("#"))
            blocks.append({"kind": "heading", "level": level, "text": line[level:].strip()})
            index += 1
            continue
        if line.startswith("|") and index + 1 < len(lines) and TABLE_SEPARATOR_RE.match(lines[index + 1].strip()):
            flush_paragraph()
            table_lines = [line, lines[index + 1]]
            index += 2
            while index < len(lines) and lines[index].startswith("|"):
                table_lines.append(lines[index])
                index += 1
            blocks.append({"kind": "table", "text": "\n".join(table_lines)})
            continue
        paragraph.append(line)
        index += 1

    flush_paragraph()
    return [block for block in blocks if block.get("text")]


def main():
    app = UIApplication()
    window = MarkdownViewer()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
