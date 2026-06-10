"""Tests for Style and Color classes."""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

import unittest
from runtime.style import (
    Color,
    Style,
    DEFAULT_TEXT_COLOR,
    DEFAULT_BG_COLOR,
    DEFAULT_FOCUS_BG_COLOR,
    DEFAULT_BORDER_WIDTH_HORIZONTAL,
    DEFAULT_BORDER_WIDTH_VERTICAL,
)


class TestColor(unittest.TestCase):
    """Test cases for the Color class."""

    def test_color_from_hex_string(self):
        """Test creating color from hex string."""
        c = Color("#ff0000")
        self.assertEqual(c.hex, "#ff0000")
        self.assertEqual(c.r, 255)
        self.assertEqual(c.g, 0)
        self.assertEqual(c.b, 0)

    def test_color_from_default(self):
        """Test creating color with default value."""
        c = Color()
        self.assertEqual(c.hex, "#ffffff")
        self.assertEqual(c.r, 255)
        self.assertEqual(c.g, 255)
        self.assertEqual(c.b, 255)

    def test_color_equality(self):
        """Test color equality."""
        c1 = Color("#ff0000")
        c2 = Color("#ff0000")
        c3 = Color("#00ff00")

        self.assertEqual(c1, c2)
        self.assertNotEqual(c1, c3)

    def test_color_copy(self):
        """Test creating color from another Color."""
        c1 = Color("#ff0000")
        c2 = Color(c1)

        self.assertEqual(c1, c2)

    def test_color_repr(self):
        """Test color string representation."""
        c = Color("#ff0000")
        self.assertEqual(repr(c), "Color(#ff0000)")


class TestStyle(unittest.TestCase):
    """Test cases for the Style class."""

    def test_style_default(self):
        """Test default style values."""
        s = Style()
        self.assertIsNone(s.color)
        self.assertIsNone(s.background)
        self.assertEqual(s.background_texture, "")
        self.assertIsNone(s.background_texture_color)
        self.assertIsNone(s.user_select)
        self.assertEqual(s.padding, (0, 0, 0, 0))
        self.assertEqual(s.margin, (0, 0, 0, 0))
        self.assertIsNone(s.border_color)
        self.assertEqual(s.border_width_horizontal, DEFAULT_BORDER_WIDTH_HORIZONTAL)
        self.assertEqual(s.border_width_vertical, DEFAULT_BORDER_WIDTH_VERTICAL)

    def test_style_set_color(self):
        """Test setting color property."""
        s = Style()
        s.set("color", "#ffffff")
        self.assertEqual(s.color.hex, "#ffffff")

    def test_style_set_background(self):
        """Test setting background property."""
        s = Style()
        s.set("background", "#222222")
        self.assertEqual(s.background.hex, "#222222")

    def test_style_set_background_texture(self):
        """Test setting background texture properties."""
        s = Style()
        s.set("background-texture", ".,")
        s.set("background-texture-color", "#123456")

        self.assertEqual(s.background_texture, ".,")
        self.assertEqual(s.background_texture_color.hex, "#123456")

    def test_style_set_user_select(self):
        """Test setting text selection behavior."""
        s = Style()
        s.set("user-select", "text")

        self.assertEqual(s.user_select, "text")

    def test_style_set_padding(self):
        """Test setting padding property."""
        s = Style()
        s.set("padding", "1, 2, 3, 4")
        self.assertEqual(s.padding, (1, 2, 3, 4))

    def test_style_set_margin(self):
        """Test setting margin property."""
        s = Style()
        s.set("margin", "1, 2, 3, 4")
        self.assertEqual(s.margin, (1, 2, 3, 4))

    def test_style_set_margin_side_aliases(self):
        """Test setting side-specific margin aliases."""
        s = Style()
        s.set("bottom-margin", "2")
        s.set("margin-left", "3")
        self.assertEqual(s.margin, (0, 0, 2, 3))

    def test_style_set_border_color(self):
        """Test setting border-color property."""
        s = Style()
        s.set("border-color", "#000000")
        self.assertEqual(s.border_color.hex, "#000000")

    def test_style_set_border_width_sets_both_axes(self):
        """Test border-width keeps setting both border axes."""
        s = Style()
        s.set("border-width", "3")
        self.assertEqual(s.border_width_horizontal, 3)
        self.assertEqual(s.border_width_vertical, 3)

    def test_style_set_axis_specific_border_width(self):
        """Test axis-specific border widths can override one direction."""
        s = Style()
        s.set("border-width", "2")
        s.set("border-width-vertical", "4")
        self.assertEqual(s.border_width_horizontal, 2)
        self.assertEqual(s.border_width_vertical, 4)

    def test_style_set_gap(self):
        """Test setting gap property."""
        s = Style()
        s.set("gap", "1")
        self.assertEqual(s.gap, 1)

    def test_style_copy(self):
        """Test copying a style."""
        s1 = Style()
        s1.set("color", "#ffffff")
        s1.set("background", "#000000")
        s1.set("background-texture", ".,")
        s1.set("background-texture-color", "#123456")
        s1.set("user-select", "text")
        s1.set("border-width-horizontal", "2")
        s1.set("border-width-vertical", "3")

        s2 = s1.copy()
        self.assertEqual(s2.color.hex, "#ffffff")
        self.assertEqual(s2.background.hex, "#000000")
        self.assertEqual(s2.background_texture, ".,")
        self.assertEqual(s2.background_texture_color.hex, "#123456")
        self.assertEqual(s2.user_select, "text")
        self.assertEqual(s2.border_width_horizontal, 2)
        self.assertEqual(s2.border_width_vertical, 3)
        self.assertEqual(s2.gap, s1.gap)

    def test_style_merge(self):
        """Test merging two styles."""
        s1 = Style()
        s1.set("color", "#ffffff")

        s2 = Style()
        s2.set("background", "#000000")

        s1.merge(s2)
        self.assertEqual(s1.color.hex, "#ffffff")
        self.assertEqual(s1.background.hex, "#000000")

    def test_style_merge_background_texture(self):
        """Test merging background texture values."""
        s1 = Style()
        s1.set("color", "#ffffff")
        s2 = Style()
        s2.set("background-texture", "ab")
        s2.set("background-texture-color", "#123456")
        s2.set("user-select", "text")

        s1.merge(s2)

        self.assertEqual(s1.background_texture, "ab")
        self.assertEqual(s1.background_texture_color.hex, "#123456")
        self.assertEqual(s1.user_select, "text")

    def test_style_merge_gap(self):
        """Test merging gap values."""
        s1 = Style()
        s2 = Style()
        s2.set("gap", "2")

        s1.merge(s2)
        self.assertEqual(s1.gap, 2)

    def test_style_merge_overrides(self):
        """Test that merge overrides existing values."""
        s1 = Style()
        s1.set("color", "#ffffff")
        s1.set("border-width", "2")

        s2 = Style()
        s2.set("color", "#000000")
        s2.set("border-width-vertical", "4")

        s1.merge(s2)
        self.assertEqual(s1.color.hex, "#000000")
        self.assertEqual(s1.border_width_horizontal, 2)
        self.assertEqual(s1.border_width_vertical, 4)

    def test_style_merge_non_none_only(self):
        """Test that merge only overrides non-None values."""
        s1 = Style()
        s1.set("color", "#ffffff")

        s2 = Style()  # No color set

        s1.merge(s2)
        self.assertEqual(s1.color.hex, "#ffffff")


class TestDefaultColors(unittest.TestCase):
    """Test default color constants."""

    def test_default_text_color(self):
        """Test DEFAULT_TEXT_COLOR."""
        self.assertEqual(DEFAULT_TEXT_COLOR.hex, "#ffffff")

    def test_default_bg_color(self):
        """Test DEFAULT_BG_COLOR."""
        self.assertEqual(DEFAULT_BG_COLOR.hex, "#000000")

    def test_default_focus_bg_color(self):
        """Test DEFAULT_FOCUS_BG_COLOR."""
        self.assertEqual(DEFAULT_FOCUS_BG_COLOR.hex, "#444444")


class TestColorAlpha(unittest.TestCase):
    """Test cases for Color alpha channel."""

    def test_color_default_alpha(self):
        """Test default alpha is 1.0."""
        c = Color("#ff0000")
        self.assertEqual(c.alpha, 1.0)

    def test_color_with_alpha(self):
        """Test creating color with alpha."""
        c = Color("#ff0000", alpha=0.5)
        self.assertEqual(c.hex, "#ff0000")
        self.assertEqual(c.alpha, 0.5)

    def test_color_equality_with_alpha(self):
        """Test color equality considers alpha."""
        c1 = Color("#ff0000", alpha=0.5)
        c2 = Color("#ff0000", alpha=0.5)
        c3 = Color("#ff0000", alpha=0.8)

        self.assertEqual(c1, c2)
        self.assertNotEqual(c1, c3)

    def test_color_copy_preserves_alpha(self):
        """Test copying color preserves alpha."""
        c1 = Color("#ff0000", alpha=0.7)
        c2 = Color(c1)

        self.assertEqual(c2.hex, "#ff0000")
        self.assertEqual(c2.alpha, 0.7)

    def test_color_from_8digit_hex(self):
        """Test creating color from 8-digit hex with alpha."""
        c = Color("#00000033")
        self.assertEqual(c.hex, "#00000033")
        self.assertEqual(c.r, 0)
        self.assertEqual(c.g, 0)
        self.assertEqual(c.b, 0)
        self.assertAlmostEqual(c.alpha, 51 / 255, places=4)

    def test_color_from_8digit_hex_full(self):
        """Test 8-digit hex with non-zero alpha."""
        c = Color("#ff0000cc")
        self.assertEqual(c.hex, "#ff0000cc")
        self.assertEqual(c.r, 255)
        self.assertEqual(c.g, 0)
        self.assertEqual(c.b, 0)
        self.assertAlmostEqual(c.alpha, 204 / 255, places=4)

    def test_color_from_5digit_hex(self):
        """Test creating color from 5-digit hex with alpha."""
        c = Color("#0003")
        self.assertEqual(c.hex, "#00000033")
        self.assertEqual(c.r, 0)
        self.assertEqual(c.g, 0)
        self.assertEqual(c.b, 0)
        self.assertAlmostEqual(c.alpha, 51 / 255, places=4)

    def test_color_from_5digit_hex_full(self):
        """Test 5-digit hex with non-zero alpha."""
        c = Color("#f0c8")
        self.assertEqual(c.hex, "#ff00cc88")
        self.assertEqual(c.r, 255)
        self.assertEqual(c.g, 0)
        self.assertEqual(c.b, 204)
        self.assertAlmostEqual(c.alpha, 136 / 255, places=4)

    def test_color_repr_with_alpha(self):
        """Test color repr includes alpha."""
        c = Color("#ff0000", alpha=0.5)
        self.assertEqual(repr(c), "Color(#ff0000, alpha=0.50)")

    def test_color_repr_without_alpha(self):
        """Test color repr without alpha when opaque."""
        c = Color("#ff0000")
        self.assertEqual(repr(c), "Color(#ff0000)")


class TestStyleAlpha(unittest.TestCase):
    """Test cases for Style alpha properties."""

    def test_style_default_alpha(self):
        """Test default alpha values are None."""
        s = Style()
        self.assertIsNone(s.color_alpha)
        self.assertIsNone(s.background_alpha)

    def test_style_set_color_alpha(self):
        """Test setting color-alpha."""
        s = Style()
        s.set("color-alpha", "0.5")
        self.assertEqual(s.color_alpha, 0.5)

    def test_style_set_background_alpha(self):
        """Test setting background-alpha."""
        s = Style()
        s.set("background-alpha", "0.75")
        self.assertEqual(s.background_alpha, 0.75)

    def test_style_copy_preserves_alpha(self):
        """Test copying style preserves alpha values."""
        s1 = Style()
        s1.set("color", "#ffffff")
        s1.set("background", "#000000")
        s1.set("color-alpha", "0.5")
        s1.set("background-alpha", "0.75")

        s2 = s1.copy()
        self.assertEqual(s2.color_alpha, 0.5)
        self.assertEqual(s2.background_alpha, 0.75)

    def test_style_merge_overrides_alpha(self):
        """Test that merge overrides alpha values."""
        s1 = Style()
        s1.set("color-alpha", "0.3")

        s2 = Style()
        s2.set("background-alpha", "0.8")

        s1.merge(s2)
        self.assertEqual(s1.color_alpha, 0.3)
        self.assertEqual(s1.background_alpha, 0.8)

    def test_style_merge_non_none_alpha_only(self):
        """Test that merge only overrides non-None alpha values."""
        s1 = Style()
        s1.set("color-alpha", "0.5")

        s2 = Style()  # No alpha set

        s1.merge(s2)
        self.assertEqual(s1.color_alpha, 0.5)


if __name__ == "__main__":
    unittest.main()
