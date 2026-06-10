"""Regression tests for the calculator example."""

import os
import sys
import unittest

PROJECT_ROOT = os.path.join(os.path.dirname(__file__), "..")
CALCULATOR_DIR = os.path.join(PROJECT_ROOT, "examples", "calculator")
sys.path.insert(0, PROJECT_ROOT)
sys.path.insert(0, CALCULATOR_DIR)

from calculator import Calculator


class TestCalculator(unittest.TestCase):
    """Calculator expression behavior."""

    def setUp(self):
        self.calculator = Calculator()
        self.calculator.open()

    def press(self, keys):
        for key in keys:
            self.calculator.handle_key(key)

    def test_expression_uses_operator_precedence(self):
        self.press("2+3*4=")
        self.assertEqual(self.calculator.display.text, "14")

    def test_decimal_expression(self):
        self.press("1.5+2.25=")
        self.assertEqual(self.calculator.display.text, "3.75")

    def test_percent_rewrites_current_number(self):
        self.press("200+10%=")
        self.assertEqual(self.calculator.display.text, "200.1")

    def test_division_by_zero_shows_error(self):
        self.press("8/0=")
        self.assertEqual(self.calculator.display.text, "Error")

    def test_next_digit_after_result_starts_new_expression(self):
        self.press("2+3=4")
        self.assertEqual(self.calculator.display.text, "4")

    def test_operator_after_result_continues_expression(self):
        self.press("2+3=*4=")
        self.assertEqual(self.calculator.display.text, "20")

    def test_enter_key_evaluates_expression(self):
        self.press(["2", "+", "3", "Enter"])
        self.assertEqual(self.calculator.display.text, "5")

    def test_enter_key_activates_focused_button(self):
        self.press("2+3")
        self.calculator.set_focus(self.calculator.n4)
        self.calculator.handle_key("Enter")
        self.assertEqual(self.calculator.display.text, "2+34")

    def test_button_event_path_uses_same_expression_logic(self):
        for element in (self.calculator.n2, self.calculator.ad,
                        self.calculator.n3, self.calculator.ml,
                        self.calculator.n4, self.calculator.sb):
            self.calculator._dispatch_element_changed(element, True)
        self.assertEqual(self.calculator.display.text, "14")

    def test_mouse_click_on_button_does_not_crash(self):
        rect = self.calculator._element_focus_rect(self.calculator.n2)
        self.calculator.handle_key({
            "type": "mouse",
            "event": "press",
            "button": 0,
            "row": rect["top"],
            "col": rect["left"],
        })

        self.assertEqual(self.calculator.display.text, "2")


if __name__ == "__main__":
    unittest.main()
