"""UIPopup — lightweight overlay popup that dismisses on Enter or mouse click."""

from uimd.runtime.UIBase import UIBase

POPUP_DIRECTION_AUTO = "auto"
POPUP_DIRECTION_UP = "up"
POPUP_DIRECTION_DOWN = "down"
POPUP_DIRECTION_LEFT = "left"
POPUP_DIRECTION_RIGHT = "right"

_VALID_DIRECTIONS = frozenset({
    POPUP_DIRECTION_AUTO, POPUP_DIRECTION_UP, POPUP_DIRECTION_DOWN,
    POPUP_DIRECTION_LEFT, POPUP_DIRECTION_RIGHT,
})


class UIPopup(UIBase):
    """Overlay popup rendered above the active window without stealing focus.

    Subclass this via ``extends: uipopup`` in a .uimd file. Use ``direction:``
    in ``## Definition`` to declare the preferred expansion direction.

    Show with ``popup.show(app, anchor_row, anchor_col, anchor_width, anchor_height)``.
    The popup auto-positions itself relative to the anchor rectangle, choosing
    the best quadrant so it stays on screen. Dismissed by the application on
    Enter or mouse click — focus never leaves the underlying window.
    """

    _preferred_direction = POPUP_DIRECTION_AUTO

    def __init__(self, title="", layout=None, members=None, style=None):
        super().__init__(layout=layout, members=members, style=style)
        self._mode = "fullscreen"
        self._title = title
        self._anchor_row = 0
        self._anchor_col = 0
        self._anchor_width = 0
        self._anchor_height = 0
        self._opened = False

    def show(self, app, anchor_row, anchor_col, anchor_width=0, anchor_height=0):
        """Display the popup anchored to the given terminal rectangle."""
        self._anchor_row = int(anchor_row)
        self._anchor_col = int(anchor_col)
        self._anchor_width = int(anchor_width)
        self._anchor_height = int(anchor_height)
        self._app = app
        if not self._opened:
            _nat_w, nat_h = self.get_natural_size()
            w = int(anchor_width) if anchor_width else int(_nat_w)
            self.resize(max(1, w), max(1, int(nat_h)))
            self.open()
            self._opened = True
        app._active_popup = self

    def dismiss(self):
        """Remove this popup from the screen without affecting focus."""
        if self._app and getattr(self._app, '_active_popup', None) is self:
            self._app._active_popup = None

    def get_popup_position(self, term_w, term_h):
        """Return ``(row, col)`` of the popup's top-left corner on screen.

        Respects ``_preferred_direction``; falls back to the first direction
        where the popup fits when set to ``"auto"``.
        """
        w = getattr(self, '_window_width', 0) or 0
        h = getattr(self, '_window_height', 0) or 0

        direction = self._preferred_direction
        if direction not in _VALID_DIRECTIONS:
            direction = POPUP_DIRECTION_AUTO

        if direction == POPUP_DIRECTION_AUTO:
            if self._anchor_row - h >= 0:
                direction = POPUP_DIRECTION_UP
            elif self._anchor_row + self._anchor_height + h <= term_h:
                direction = POPUP_DIRECTION_DOWN
            elif self._anchor_col + self._anchor_width + w <= term_w:
                direction = POPUP_DIRECTION_RIGHT
            else:
                direction = POPUP_DIRECTION_LEFT

        if direction == POPUP_DIRECTION_UP:
            row = self._anchor_row - h
            col = self._anchor_col
        elif direction == POPUP_DIRECTION_DOWN:
            row = self._anchor_row + self._anchor_height
            col = self._anchor_col
        elif direction == POPUP_DIRECTION_RIGHT:
            row = self._anchor_row
            col = self._anchor_col + self._anchor_width
        else:  # left
            row = self._anchor_row
            col = self._anchor_col - w

        row = max(0, min(row, max(0, term_h - h)))
        col = max(0, min(col, max(0, term_w - w)))
        return row, col
