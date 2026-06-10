from uimd.runtime.uiinstance import UIInstance


DEFAULT_BORDER_WIDTH_HORIZONTAL = 0
DEFAULT_BORDER_WIDTH_VERTICAL = 0
DEFAULT_SCOPE_DIM_BACKGROUND = "#00000066"


class Color:
    """Represents a color with r, g, b values (0-255) or a hex string."""

    def __init__(self, value=None, alpha=1.0):
        if isinstance(value, str):
            value = value.strip()
            if value.endswith(";"):
                value = value[:-1].rstrip()
            if value == "transparent":
                self.hex = "transparent"
                self.r, self.g, self.b = -1, -1, -1
                self.alpha = 0.0
            elif len(value) == 4:
                expanded = "#" + value[1]*2 + value[2]*2 + value[3]*2
                self.hex = expanded
                r = int(expanded[1:3], 16)
                g = int(expanded[3:5], 16)
                b = int(expanded[5:7], 16)
                self.r, self.g, self.b = r, g, b
            elif len(value) == 5:
                expanded = "#" + value[1]*2 + value[2]*2 + value[3]*2
                self.hex = expanded + value[4]*2
                r = int(expanded[1:3], 16)
                g = int(expanded[3:5], 16)
                b = int(expanded[5:7], 16)
                a = int(value[4]*2, 16) / 255
                self.r, self.g, self.b = r, g, b
                self.alpha = a
            elif len(value) == 9:
                self.hex = value
                r = int(value[1:3], 16)
                g = int(value[3:5], 16)
                b = int(value[5:7], 16)
                a = int(value[7:9], 16) / 255
                self.r, self.g, self.b = r, g, b
                self.alpha = a
            else:
                self.hex = value
                r = int(value[1:3], 16)
                g = int(value[3:5], 16)
                b = int(value[5:7], 16)
                self.r, self.g, self.b = r, g, b
        elif isinstance(value, Color):
            self.hex = value.hex
            self.r, self.g, self.b = value.r, value.g, value.b
            self.alpha = value.alpha
        else:
            self.hex = "#ffffff"
            self.r, self.g, self.b = 255, 255, 255
        if not hasattr(self, 'alpha'):
            self.alpha = alpha

    def __eq__(self, other):
        if isinstance(other, Color):
            return self.hex == other.hex and abs(self.alpha - other.alpha) < 0.001
        return False

    def __repr__(self):
        if self.alpha < 1.0:
            return f"Color({self.hex}, alpha={self.alpha:.2f})"
        return f"Color({self.hex})"


class Style:
    """Style definition for a UI element or window."""

    @staticmethod
    def _lighten(hex_color, percent):
        r = int(hex_color[1:3], 16)
        g = int(hex_color[3:5], 16)
        b = int(hex_color[5:7], 16)
        r = min(255, round(r + (255 - r) * percent))
        g = min(255, round(g + (255 - g) * percent))
        b = min(255, round(b + (255 - b) * percent))
        return f"#{r:02x}{g:02x}{b:02x}"

    @staticmethod
    def _blend_color_over(color, background):
        if color is None:
            return background
        alpha = getattr(color, "alpha", 1.0)
        if alpha is None or alpha >= 1.0:
            return color
        if alpha <= 0.0:
            return background
        if background is None or getattr(background, "r", -1) < 0:
            return color
        r = round(alpha * color.r + (1 - alpha) * background.r)
        g = round(alpha * color.g + (1 - alpha) * background.g)
        b = round(alpha * color.b + (1 - alpha) * background.b)
        return Color(f"#{r:02x}{g:02x}{b:02x}")

    def __init__(self):
        self.color = None  # Color for text
        self.background = None  # Color for background
        self.background_texture = ""
        self.background_texture_color = None
        self.user_select = None
        self.scroll_x = False
        self.scroll_y = True
        self.color_alpha = None  # Alpha for text color (0.0-1.0)
        self.background_alpha = None  # Alpha for background color (0.0-1.0)
        self.padding = (0, 0, 0, 0)  # top, right, bottom, left
        self.margin = (0, 0, 0, 0)  # top, right, bottom, left
        self.border_color = None
        self.border_width_horizontal = DEFAULT_BORDER_WIDTH_HORIZONTAL
        self.border_width_vertical = DEFAULT_BORDER_WIDTH_VERTICAL
        self.focus_background = None
        self.focus_color = None
        self.text_align = None
        self.gap = 0
        self.word_wrap = True
        self.text_color_gradient = None
        self.text_background_gradient = None
        self.scope_dim_background = Color(DEFAULT_SCOPE_DIM_BACKGROUND)

    @property
    def border_width(self):
        return self.border_width_horizontal

    @border_width.setter
    def border_width(self, value):
        width = int(value)
        self.border_width_horizontal = width
        self.border_width_vertical = width

    @staticmethod
    def _parse_box_values(value):
        parts = str(value).split(",")
        nums = tuple(int(p.strip()) for p in parts)
        if len(nums) == 1:
            return (nums[0], nums[0], nums[0], nums[0])
        if len(nums) == 2:
            return (nums[0], nums[1], nums[0], nums[1])
        if len(nums) == 4:
            return nums
        return nums + (0,) * (4 - len(nums))

    @staticmethod
    def _set_box_side(values, side, value):
        top, right, bottom, left = values
        parsed = int(value)
        if side == "top":
            top = parsed
        elif side == "right":
            right = parsed
        elif side == "bottom":
            bottom = parsed
        elif side == "left":
            left = parsed
        return (top, right, bottom, left)

    def set(self, key, value):
        if key == "color":
            self.color = Color(value)
        elif key == "background":
            self.background = Color(value)
        elif key == "background-texture":
            self.background_texture = str(value)
        elif key == "background-texture-color":
            self.background_texture_color = Color(value)
        elif key == "user-select":
            self.user_select = str(value).strip().lower()
        elif key == "scroll-x":
            if isinstance(value, bool):
                self.scroll_x = value
            else:
                self.scroll_x = str(value).strip().lower() in ("true", "yes", "1")
        elif key == "scroll-y":
            if isinstance(value, bool):
                self.scroll_y = value
            else:
                self.scroll_y = str(value).strip().lower() in ("true", "yes", "1")
        elif key == "padding":
            self.padding = self._parse_box_values(value)
        elif key == "margin":
            self.margin = self._parse_box_values(value)
        elif key in ("margin-top", "top-margin"):
            self.margin = self._set_box_side(self.margin, "top", value)
        elif key in ("margin-right", "right-margin"):
            self.margin = self._set_box_side(self.margin, "right", value)
        elif key in ("margin-bottom", "bottom-margin"):
            self.margin = self._set_box_side(self.margin, "bottom", value)
        elif key in ("margin-left", "left-margin"):
            self.margin = self._set_box_side(self.margin, "left", value)
        elif key == "color-alpha":
            self.color_alpha = float(value)
        elif key == "background-alpha":
            self.background_alpha = float(value)
        elif key == "border-color":
            self.border_color = Color(value)
        elif key == "border-width":
            self.border_width = value
        elif key == "border-width-horizontal":
            self.border_width_horizontal = int(value)
        elif key == "border-width-vertical":
            self.border_width_vertical = int(value)
        elif key == "focus-background":
            self.focus_background = Color(value)
        elif key == "focus-color":
            self.focus_color = Color(value)
        elif key == "text-align":
            self.text_align = str(value).strip().lower()
        elif key == "gap":
            self.gap = int(value)
        elif key == "word-wrap":
            if isinstance(value, bool):
                self.word_wrap = value
            elif isinstance(value, str):
                self.word_wrap = value.lower() in ("true", "yes", "1")
        elif key == "text-color-gradient":
            self.text_color_gradient = self._parse_text_gradient(value)
        elif key == "text-background-gradient":
            self.text_background_gradient = self._parse_text_gradient(value)
        elif key == "scope-dim-background":
            self.scope_dim_background = Color(value)

    @staticmethod
    def _parse_text_gradient(value):
        if not isinstance(value, dict):
            return None
        colors = value.get("colors", [])
        if isinstance(colors, str):
            colors = [colors]
        parsed_colors = [Color(color) for color in colors if str(color).strip()]
        if not parsed_colors:
            return None
        return {
            "interval": max(1, int(value.get("interval", 70) or 70)),
            "step": int(value.get("step", 1) or 1),
            "segment_size": max(1, int(value.get("segment-size", value.get("segment_size", 1)) or 1)),
            "colors": parsed_colors,
        }

    def copy(self):
        s = Style()
        s.color = self.color
        s.background = self.background
        s.background_texture = self.background_texture
        s.background_texture_color = self.background_texture_color
        s.user_select = self.user_select
        s.scroll_x = self.scroll_x
        s.scroll_y = self.scroll_y
        s.color_alpha = self.color_alpha
        s.background_alpha = self.background_alpha
        s.padding = self.padding
        s.margin = self.margin
        s.border_color = self.border_color
        s.border_width_horizontal = self.border_width_horizontal
        s.border_width_vertical = self.border_width_vertical
        s.focus_background = self.focus_background
        s.focus_color = self.focus_color
        s.text_align = self.text_align
        s.gap = self.gap
        s.word_wrap = self.word_wrap
        s.text_color_gradient = self.text_color_gradient
        s.text_background_gradient = self.text_background_gradient
        s.scope_dim_background = self.scope_dim_background
        return s

    def merge(self, other):
        """Merge another style on top of this one (non-None values override)."""
        if other.color is not None:
            self.color = other.color
        if other.background is not None:
            self.background = other.background
        if other.background_texture:
            self.background_texture = other.background_texture
        if other.background_texture_color is not None:
            self.background_texture_color = other.background_texture_color
        if other.user_select is not None:
            self.user_select = other.user_select
        if other.scroll_x:
            self.scroll_x = other.scroll_x
        if not other.scroll_y:
            self.scroll_y = other.scroll_y
        if other.color_alpha is not None:
            self.color_alpha = other.color_alpha
        if other.background_alpha is not None:
            self.background_alpha = other.background_alpha
        if other.padding != (0, 0, 0, 0):
            self.padding = other.padding
        if other.margin != (0, 0, 0, 0):
            self.margin = other.margin
        if other.border_color is not None:
            self.border_color = other.border_color
        if other.border_width_horizontal != DEFAULT_BORDER_WIDTH_HORIZONTAL:
            self.border_width_horizontal = other.border_width_horizontal
        if other.border_width_vertical != DEFAULT_BORDER_WIDTH_VERTICAL:
            self.border_width_vertical = other.border_width_vertical
        if other.focus_background is not None:
            self.focus_background = other.focus_background
        if other.focus_color is not None:
            self.focus_color = other.focus_color
        if other.text_align is not None:
            self.text_align = other.text_align
        if other.gap != 0:
            self.gap = other.gap
        if other.word_wrap is not None:
            self.word_wrap = other.word_wrap
        if other.text_color_gradient is not None:
            self.text_color_gradient = other.text_color_gradient
        if other.text_background_gradient is not None:
            self.text_background_gradient = other.text_background_gradient
        if other.scope_dim_background is not None:
            self.scope_dim_background = other.scope_dim_background


DEFAULT_TEXT_COLOR = Color("#ffffff")
DEFAULT_BG_COLOR = Color("#000000")
DEFAULT_FOCUS_BG_COLOR = Color("#444444")
