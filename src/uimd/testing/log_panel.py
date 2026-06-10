"""Scrollable log panel for the visual MCP tester."""

from uimd.runtime import Label, Style
from uimd.runtime.elements import _copy_text_to_clipboard, _notify_copied
from uimd.testing.log_panel_ui import LogPanelUI


NORMAL_STYLE = "log_normal"
ASSERT_OK_STYLE = "log_assert_ok"
ASSERT_FAIL_STYLE = "log_assert_fail"


class LogPanel(LogPanelUI):
    def __init__(self):
        super().__init__()
        self._next_line_id = 0

    def clear(self):
        self._children.clear()
        self._cached_child_heights = None
        self._next_line_id = 0
        self.scroll_to_bottom()
        self.invalidate_layout()

    def append_line(self, text, kind=None):
        label = Label(
            name=f"log_line_{self._next_line_id}",
            text=str(text),
            height=1,
        )
        self._next_line_id += 1
        label.style = self._style_for_kind(kind or self.kind_for_line(str(text)))
        label.parent = self
        label._app = self._app
        self.add_child(label)
        self.scroll_to_bottom()

    def text(self):
        return "\n".join(str(child.text) for child in self._children)

    def copy_all(self):
        _copy_text_to_clipboard(self.text())
        _notify_copied(self)
        return len(self._children)

    def kind_for_line(self, line):
        if "FAIL" in line:
            return ASSERT_FAIL_STYLE
        if "ASSERT" in line or "PASS" in line:
            return ASSERT_OK_STYLE
        return NORMAL_STYLE

    def _style_for_kind(self, kind):
        style = Style()
        for key, value in self._compiled_styles.get(f"@{kind}", {}).items():
            style.set(key, value)
        return style

    def _render_child(self, child, width):
        child.width = width
        child.height = 1
        return child.render()
