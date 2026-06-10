"""Application logic for calculator.uimd - extends generated UI."""

import ast
import math
import operator
import sys
import os

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from runtime import UIApplication
from calculator_ui import CalculatorUI

DISPLAY_ZERO = "0"
DISPLAY_ERROR = "Error"
PERCENT_FACTOR = 100
INTEGER_FORMAT_LIMIT = 10 ** 15
GENERAL_FORMAT = ".10g"

_BUTTON_DIGIT = {"n0": "0", "n1": "1", "n2": "2", "n3": "3", "n4": "4",
                 "n5": "5", "n6": "6", "n7": "7", "n8": "8", "n9": "9"}
_BUTTON_OP = {"ad": "+", "mn": "-", "ml": "*", "dv": "/"}
_KEY_DIGIT = set("0123456789")
_OPERATORS = set(_BUTTON_OP.values())
_BINARY_OPERATIONS = {
    ast.Add: operator.add,
    ast.Sub: operator.sub,
    ast.Mult: operator.mul,
    ast.Div: operator.truediv,
}
_UNARY_OPERATIONS = {
    ast.UAdd: operator.pos,
    ast.USub: operator.neg,
}


class Calculator(CalculatorUI):

    def __init__(self):
        super().__init__()
        self._expression = DISPLAY_ZERO
        self._after_result = False

    def open(self):
        super().open()
        self._refresh()

    def handle_key(self, key):
        if not isinstance(key, str):
            return super().handle_key(key)
        if key in _KEY_DIGIT:
            self._digit(key)
            return True
        if key == ".":
            self._digit(key)
            return True
        if key in _OPERATORS:
            self._operator(key)
            return True
        if key == "=" or (key == "Enter" and self._focused_element is None):
            self._equals()
            return True
        if key == "%":
            self._percent()
            return True
        if key == "Backspace":
            self._delete()
            return True
        return super().handle_key(key)

    def on_n0_click(self):
        self._digit("0")

    def on_n1_click(self):
        self._digit("1")

    def on_n2_click(self):
        self._digit("2")

    def on_n3_click(self):
        self._digit("3")

    def on_n4_click(self):
        self._digit("4")

    def on_n5_click(self):
        self._digit("5")

    def on_n6_click(self):
        self._digit("6")

    def on_n7_click(self):
        self._digit("7")

    def on_n8_click(self):
        self._digit("8")

    def on_n9_click(self):
        self._digit("9")

    def on_dt_click(self):
        self._digit(".")

    def on_ad_click(self):
        self._operator("+")

    def on_mn_click(self):
        self._operator("-")

    def on_ml_click(self):
        self._operator("*")

    def on_dv_click(self):
        self._operator("/")

    def on_sb_click(self):
        self._equals()

    def on_ac_click(self):
        self._reset()

    def on_dl_click(self):
        self._delete()

    def on_pr_click(self):
        self._percent()

    # ── input ─────────────────────────────────────────────────────────────

    def _digit(self, d):
        if self._after_result or self._expression == DISPLAY_ERROR:
            self._expression = ""
            self._after_result = False

        if d == ".":
            self._append_decimal_point()
        elif self._expression == DISPLAY_ZERO:
            self._expression = d
        elif self._current_number_text() in (DISPLAY_ZERO, f"-{DISPLAY_ZERO}"):
            self._replace_current_number(self._current_number_text().replace(DISPLAY_ZERO, d, 1))
        else:
            self._expression += d
        self._refresh()

    def _operator(self, op):
        if self._expression == DISPLAY_ERROR:
            self._expression = DISPLAY_ZERO

        if not self._expression:
            if op == "-":
                self._expression = op
                self._after_result = False
                self._refresh()
            return

        if self._expression[-1] in _OPERATORS:
            if op == "-" and self._expression[-1] != "-":
                self._expression += op
            else:
                self._expression = self._expression.rstrip("".join(_OPERATORS)) + op
        else:
            self._expression += op
        self._after_result = False
        self._refresh()

    def _equals(self):
        self._compute()

    def _delete(self):
        if self._after_result or self._expression == DISPLAY_ERROR:
            self._reset()
            return
        self._expression = self._expression[:-1] or DISPLAY_ZERO
        self._refresh()

    def _percent(self):
        try:
            current = self._current_number_text()
            if not current:
                return
            self._replace_current_number(self._format(float(current) / PERCENT_FACTOR))
            self._after_result = False
            self._refresh()
        except (TypeError, ValueError, ZeroDivisionError):
            self._show_error()

    # ── core ──────────────────────────────────────────────────────────────

    def _compute(self):
        try:
            result = self._evaluate_expression(self._expression)
            if not math.isfinite(result):
                raise ValueError("Non-finite calculator result")
            self._expression = self._format(result)
            self._after_result = True
            self._refresh()
        except (SyntaxError, ValueError, TypeError, ZeroDivisionError):
            self._show_error()

    def _reset(self):
        self._expression = DISPLAY_ZERO
        self._after_result = False
        self._refresh()

    def _refresh(self):
        self.display.text = self._expression or DISPLAY_ZERO

    def _show_error(self):
        self._expression = DISPLAY_ERROR
        self._after_result = True
        self._refresh()

    def _append_decimal_point(self):
        current = self._current_number_text()
        if "." in current:
            return
        if not self._expression or self._expression[-1] in _OPERATORS:
            self._expression += f"{DISPLAY_ZERO}."
        else:
            self._expression += "."

    def _current_number_span(self):
        expression = self._expression
        end = len(expression)
        if end == 0 or expression[end - 1] in _OPERATORS:
            return end, end

        start = end
        while start > 0 and (expression[start - 1].isdigit() or expression[start - 1] == "."):
            start -= 1
        if start > 0 and expression[start - 1] == "-":
            if start == 1 or expression[start - 2] in _OPERATORS:
                start -= 1
        return start, end

    def _current_number_text(self):
        start, end = self._current_number_span()
        return self._expression[start:end]

    def _replace_current_number(self, value):
        start, end = self._current_number_span()
        self._expression = self._expression[:start] + value + self._expression[end:]

    @classmethod
    def _evaluate_expression(cls, expression):
        if not expression or expression == DISPLAY_ERROR:
            raise ValueError("Empty calculator expression")
        tree = ast.parse(expression, mode="eval")
        return float(cls._evaluate_node(tree.body))

    @classmethod
    def _evaluate_node(cls, node):
        if isinstance(node, ast.Constant):
            if type(node.value) not in (int, float):
                raise ValueError("Unsupported calculator literal")
            return node.value
        if isinstance(node, ast.BinOp):
            operation = _BINARY_OPERATIONS.get(type(node.op))
            if operation is None:
                raise ValueError("Unsupported calculator operator")
            return operation(cls._evaluate_node(node.left), cls._evaluate_node(node.right))
        if isinstance(node, ast.UnaryOp):
            operation = _UNARY_OPERATIONS.get(type(node.op))
            if operation is None:
                raise ValueError("Unsupported calculator unary operator")
            return operation(cls._evaluate_node(node.operand))
        raise ValueError("Unsupported calculator expression")

    @staticmethod
    def _format(v):
        if v.is_integer() and abs(v) < INTEGER_FORMAT_LIMIT:
            return str(int(v))
        return f"{v:{GENERAL_FORMAT}}"

    def save(self):
        pass

    def focused(self, element, value):
        pass


def main():
    app = UIApplication()
    window = Calculator()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
