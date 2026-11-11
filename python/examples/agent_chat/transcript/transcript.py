"""Scrollable deterministic simulated agent transcript."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "chat_entry"))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "code_entry"))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "table_entry"))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "user_entry"))
sys.path.insert(0, os.path.join(os.path.dirname(_here), "reasoning_entry"))

from chat_entry_ui import ChatEntryUI
from code_entry_ui import CodeEntryUI
from table_entry_ui import TableEntryUI
from user_entry_ui import UserEntryUI
from reasoning_entry_ui import ReasoningEntryUI
from transcript_ui import TranscriptUI


CODE_PHASES = frozenset(
    {
        "code",
        "code · python",
        "diff",
        "patch",
        "result",
        "search_code",
        "structured data",
        "test failure",
        "warning",
    }
)


def _without_code_fence(text):
    lines = str(text).splitlines()
    if lines and lines[0].startswith("```"):
        lines = lines[1:]
    if lines and lines[-1].startswith("```"):
        lines = lines[:-1]
    return "\n".join(lines)


def _numbered_code(text):
    return "\n".join(
        f"{index}  {line}"
        for index, line in enumerate(_without_code_fence(text).splitlines(), 1)
    )


class Transcript(TranscriptUI):
    """Own the transcript children without a second cached item index."""

    def __init__(self):
        super().__init__()
        self._auto_scroll = False
        self._entries = []
        self.set_virtual_data_source(
            lambda: len(self._entries),
            lambda index: index,
            self._materialize_entry,
        )

    def append_entry(self, role, phase, body, detail):
        index = len(self._entries)
        self._entries.append((str(role), str(phase), str(body), str(detail)))
        self.virtual_items_inserted(index)
        self.invalidate_layout()

    def replace_last_entry(self, role, phase, body, detail):
        if not self._entries:
            self.append_entry(role, phase, body, detail)
            return
        index = len(self._entries) - 1
        self._entries[index] = (str(role), str(phase), str(body), str(detail))
        self.virtual_items_replaced(index)
        self.invalidate_layout()

    def clear_entries(self):
        count = len(self._entries)
        self._entries.clear()
        self.virtual_items_removed(0, count)
        self.scroll_to_top()

    def replace_entries(self, entries):
        self.clear_entries()
        self._entries.extend(
            (str(role), str(phase), str(body), str(detail))
            for role, phase, body, detail in entries
        )
        self.virtual_items_inserted(0, len(self._entries))
        self.invalidate_layout()

    def entry_count(self):
        return len(self._entries)

    def entry_data(self, index):
        return self._entries[index]

    def _materialize_entry(self, index, recycled):
        phase = self._entries[index][1]
        role = self._entries[index][0]
        entry_class = (
            UserEntryUI if role == "You" else
            ReasoningEntryUI if phase in ("reasoning", "status") else
            TableEntryUI if phase == "table" else
            CodeEntryUI if phase in CODE_PHASES else
            ChatEntryUI
        )
        entry = recycled if isinstance(recycled, entry_class) else entry_class()
        entry.mode = "expand_width"
        self._set_entry(entry, *self._entries[index])
        return entry

    @staticmethod
    def _set_entry(entry, role, phase, body, detail):
        if str(role) == "You":
            entry.body.text = str(body)
        elif str(phase) in ("reasoning", "status"):
            entry.heading.text = str(detail)
            entry.body.text = str(body)
        elif str(phase) == "table":
            entry.heading.text = str(detail)
            entry.table.text = str(body)
        elif str(phase) in CODE_PHASES:
            entry.heading.text = str(detail)
            entry.code.text = (
                _numbered_code(body)
                if str(phase) in ("code", "code · python")
                else _without_code_fence(body)
            )
        else:
            entry.heading.text = str(detail) if str(role) == "Tool" else (
                "Assistant" if str(role) == "Agent" else str(role)
            )
            entry.body.text = str(body)
        entry.invalidate_layout()
