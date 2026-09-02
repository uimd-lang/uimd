"""Scrollable deterministic simulated agent transcript."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "chat_entry"))

from chat_entry_ui import ChatEntryUI
from transcript_ui import TranscriptUI


class Transcript(TranscriptUI):
    """Own the transcript children without a second cached item index."""

    def __init__(self):
        super().__init__()
        self._auto_scroll = False

    def append_entry(self, role, phase, body, detail):
        entry = ChatEntryUI()
        entry.parent = self
        entry._app = self._app
        entry.mode = "expand_width"
        self._set_entry(entry, role, phase, body, detail)
        entry.open()
        self.add_child(entry)
        self.invalidate_layout()
        return entry

    def replace_last_entry(self, role, phase, body, detail):
        if not self._children:
            return self.append_entry(role, phase, body, detail)
        entry = self._children[-1]
        self._set_entry(entry, role, phase, body, detail)
        self.invalidate_layout()
        return entry

    def clear_entries(self):
        self.clear_children()
        self.scroll_to_top()

    def entry_count(self):
        return len(self._children)

    @staticmethod
    def _set_entry(entry, role, phase, body, detail):
        entry.role.text = str(role)
        entry.phase.text = str(phase)
        entry.body.text = str(body)
        entry.detail.text = str(detail)
        entry.invalidate_layout()
