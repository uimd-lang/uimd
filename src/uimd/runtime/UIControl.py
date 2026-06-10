from uimd.runtime.UIBase import UIBase


class UIControl(UIBase):
    """UI control container without title bar decorations."""

    def __init__(self, title="", layout=None, members=None, style=None):
        super().__init__(layout=layout, members=members, style=style)
        self._title = title

    @property
    def title(self):
        return self._title

    @title.setter
    def title(self, value):
        self._title = str(value)
