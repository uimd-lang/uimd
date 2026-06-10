from uimd.runtime.uiinstance import UIInstance
from uimd.runtime.style import Style, Color, DEFAULT_TEXT_COLOR, DEFAULT_BG_COLOR, DEFAULT_FOCUS_BG_COLOR
from uimd.runtime.rendering import parse_ansi_cells


class UIElement(UIInstance):
    """Base class for all UI elements."""

    ELEMENT_TYPE = None  # Override in subclasses

    def __init__(self, name="", row=0, col=0, width=None, height=1, style=None):
        super().__init__()
        self.name = name
        self.row = row
        self.col = col
        self.width = width
        self.height = height
        self.style = style or Style()
        self.focus_style = None
        self.edit_style = None
        self.cursor_style = None
        self.selected_style = None
        self.checked_style = None
        self.unchecked_style = None
        self.disabled_style = None
        self.error_style = None
        self.focusable = True
        self.commit_mode = "standard"
        self._focused = False
        self._edit_mode = False
        self._render_cache = {}  # (width, height, focused, edit_mode) -> list of lines

    @property
    def enabled(self):
        return bool(getattr(self, "_enabled", True))

    @enabled.setter
    def enabled(self, value):
        was_enabled = bool(getattr(self, "_enabled", True))
        self._enabled = bool(value)
        if self._enabled == was_enabled:
            return
        if not self._enabled and self._focused:
            owner = getattr(self, "parent", None)
            while owner is not None and not hasattr(owner, "set_focus"):
                owner = getattr(owner, "parent", None)
            if owner is not None:
                owner.set_focus(None)
            else:
                self.focused = False
        self._render_cache.clear()
        self._mark_layout_dirty()
        self._mark_dirty()

    @property
    def width(self):
        return self._width

    @width.setter
    def width(self, value):
        self._width = value
        cache = getattr(self, '_render_cache', None)
        if cache is not None:
            cache.clear()
        cache = getattr(self, '_visual_rows_cache', None)
        if cache is not None:
            cache.clear()

    @property
    def height(self):
        return self._height

    @height.setter
    def height(self, value):
        self._height = value
        cache = getattr(self, '_render_cache', None)
        if cache is not None:
            cache.clear()
        cache = getattr(self, '_visual_rows_cache', None)
        if cache is not None:
            cache.clear()

    @property
    def focused(self):
        return self._focused

    @focused.setter
    def focused(self, value):
        if value and not self.enabled:
            value = False
        was_focused = self._focused
        self._focused = bool(value)
        if self._focused != was_focused:
            self._render_cache.clear()
            self._mark_dirty()
            app = getattr(self, "_app", None)
            if app is not None and getattr(self, "ELEMENT_TYPE", None) in ("uielement", "uiscrollview"):
                app.request_full_redraw()
            if self._focused:
                self.focus()
            else:
                self.blur()

    @property
    def edit_mode(self):
        return self._edit_mode

    @edit_mode.setter
    def edit_mode(self, value):
        self._edit_mode = bool(value)
        self._render_cache.clear()
        self._mark_dirty()

    def get_style(self):
        if not self.enabled and self.disabled_style:
            return self._state_style(self.disabled_style)
        if self._edit_mode and self.edit_style:
            return self._state_style(self.edit_style)
        if self._focused and self.focus_style:
            return self._state_style(self.focus_style)
        if self._focused and not self.focus_style:
            s = self.style.copy()
            bg = s.background
            if bg is not None and bg.hex != "transparent":
                s.focus_background = Color(Style._lighten(bg.hex, 0.16))
            return s
        if self.error and self.error_style:
            s = self.style.copy()
            s.merge(self.error_style)
            return s
        return self.style

    def _state_style(self, state_style):
        s = self.style.copy()
        state_background = getattr(state_style, "background", None)
        if state_background is not None and getattr(state_background, "alpha", 1.0) < 1.0:
            base_background = self._effective_background(s.background)
            s.merge(state_style)
            s.background = Style._blend_color_over(state_background, base_background)
            return s
        s.merge(state_style)
        return s

    def _effective_background(self, background):
        if background is None:
            return self._get_cell_background()
        if getattr(background, "alpha", 1.0) < 1.0:
            return Style._blend_color_over(background, self._get_cell_background())
        return background

    @property
    def error(self):
        return False

    def _get_cell_background(self):
        """Get the structural cell background for alpha blending."""
        return getattr(self, '_cell_background', None)

    def _get_cell_color(self):
        """Get the structural cell color for foreground inheritance."""
        return getattr(self, '_cell_color', None)

    def render_line(self, line_idx):
        return ""

    def render_cells(self):
        return [parse_ansi_cells(line) for line in self.render()]

    def get_render_size(self):
        lines = self.render()
        return len(lines), max((len(l) for l in lines), default=0)

    def handle_key(self, key):
        return False

    def select_range(self, start, end):
        pass

    def _mark_dirty(self):
        """Mark the parent window/application as needing re-render."""
        app = getattr(self, '_app', None)
        if app is not None:
            app.mark_dirty()

    def _mark_layout_dirty(self):
        """Mark the owning layout as needing geometry recomputation."""
        parent = getattr(self, 'parent', None)
        while parent is not None:
            if hasattr(parent, 'invalidate_layout'):
                parent.invalidate_layout()
                return
            parent = getattr(parent, 'parent', None)
