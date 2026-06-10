"""Tests for UIInstance base class."""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

import unittest
from runtime.uiinstance import UIInstance


class TestUIInstance(unittest.TestCase):
    """Test cases for the UIInstance base class."""

    def test_enabled_property_default(self):
        """Test that enabled defaults to True."""
        instance = UIInstance()
        self.assertTrue(instance.enabled)

    def test_enabled_property_setter(self):
        """Test setting enabled property."""
        instance = UIInstance()
        instance.enabled = False
        self.assertFalse(instance.enabled)

    def test_enabled_property_setter_bool_conversion(self):
        """Test that enabled is converted to bool."""
        instance = UIInstance()
        instance.enabled = 1
        self.assertTrue(instance.enabled)

    def test_parent_property(self):
        """Test parent property getter and setter."""
        instance = UIInstance()
        self.assertIsNone(instance.parent)

        parent = UIInstance()
        instance.parent = parent
        self.assertEqual(instance.parent, parent)

    def test_virtual_methods_exist(self):
        """Test that all virtual methods exist and can be called."""
        instance = UIInstance()

        # All these should not raise exceptions
        instance.open()
        instance.save()
        instance.close()
        instance.focus()
        instance.blur()
        instance.resize(80, 24)
        instance.elementchanged(None, None)
        instance.focused(None, None)
        instance.enabled(None, None)
        result = instance.render()
        self.assertEqual(result, [])


class TestUIInstanceSubclass(unittest.TestCase):
    """Test subclassing UIInstance."""

    def test_subclass_override_open(self):
        """Test overriding open method."""
        called = []

        class MyInstance(UIInstance):
            def open(self):
                called.append(True)

        instance = MyInstance()
        instance.open()
        self.assertEqual(called, [True])

    def test_subclass_override_render(self):
        """Test overriding render method."""
        class MyInstance(UIInstance):
            def render(self):
                return ["line1", "line2"]

        instance = MyInstance()
        self.assertEqual(instance.render(), ["line1", "line2"])


if __name__ == "__main__":
    unittest.main()
