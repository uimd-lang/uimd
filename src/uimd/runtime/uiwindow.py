from uimd.runtime.UIBase import (
    UIBase, LayoutCell, ResolvedCell, LayoutNode, LeafNode, GroupNode,
    _strip_ansi, _make_ansi, _escape_to_list, Cell, LayoutGrid,
    MOUSE_LEFT_BUTTON, USER_SELECT_TEXT,
    DIMENSION_AUTO, DIMENSION_FIXED, DIMENSION_EXPANDED, DIMENSION_FIT_CONTENT,
    FIT_CONTENT, MIN_LAYOUT_SIZE, TEXTINPUT_CURSOR_SPACE, NUMBERINPUT_PADDING_X,
    CHECKBOX_INDICATOR_WIDTH, CHECKBOX_TEXT_GAP, COMBOBOX_ARROW_WIDTH, LISTBOX_ITEM_PREFIX_WIDTH,
)


class UIWindow(UIBase):
    """Window container with title bar support."""

    def __init__(self, title="", layout=None, members=None, style=None):
        super().__init__(layout=layout, members=members, style=style)
        self._title = title

    @property
    def title(self):
        return self._title

    @title.setter
    def title(self, value):
        self._title = str(value)
