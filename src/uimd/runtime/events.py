"""Public runtime event value objects."""

from dataclasses import dataclass


@dataclass(frozen=True)
class KeyEvent:
    """One logical key routed through the active window and focused element."""

    key: str
    focused_element_id: str = ""
    edit_mode: bool = False
