"""Tests for standard reusable dialogs."""

import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from dialogs import (
    MessageBoxOk,
    MessageBoxOkCancel,
    MessageBoxYesNo,
    MessageBoxYesNoCancel,
)
from runtime import UIApplication, UIWindow


class TestMessageBoxes(unittest.TestCase):
    def _open_dialog(self, dialog):
        width, height = dialog.get_natural_size()
        app = UIApplication(width=width, height=height)
        app.open(dialog)
        return app

    def test_header_and_message_are_configurable(self):
        dialog = MessageBoxOk(header="Saved", message="File saved.")

        self._open_dialog(dialog)

        self.assertEqual(dialog.dialog_header.text, "Saved")
        self.assertEqual(dialog.message.text, "File saved.")

    def test_ok_dialog_returns_ok(self):
        results = []
        dialog = MessageBoxOk(on_close=results.append)
        app = self._open_dialog(dialog)

        dialog.elementchanged(dialog.ok_btn, None)

        self.assertEqual(results, ["ok"])
        self.assertEqual(dialog.result, "ok")
        self.assertEqual(len(app._window_stack), 0)

    def test_callback_can_open_next_window_after_dialog_closes(self):
        next_window = UIWindow(title="Next")
        opened = []

        def on_close(result):
            opened.append(result)
            app.open(next_window)

        dialog = MessageBoxYesNoCancel(on_close=on_close)
        app = self._open_dialog(dialog)

        dialog.elementchanged(dialog.no_btn, None)

        self.assertEqual(opened, ["no"])
        self.assertIs(app.active_window, next_window)
        self.assertEqual(len(app._window_stack), 1)

    def _assert_button_result(self, dialog_class, button_name, expected_result):
        results = []
        dialog = dialog_class(on_close=results.append)
        self._open_dialog(dialog)

        dialog.elementchanged(getattr(dialog, button_name), None)

        self.assertEqual(results, [expected_result])
        self.assertEqual(dialog.result, expected_result)

    def test_ok_cancel_dialog_returns_each_button(self):
        self._assert_button_result(MessageBoxOkCancel, "ok_btn", "ok")
        self._assert_button_result(MessageBoxOkCancel, "cancel_btn", "cancel")

    def test_yes_no_dialog_returns_each_button(self):
        self._assert_button_result(MessageBoxYesNo, "yes_btn", "yes")
        self._assert_button_result(MessageBoxYesNo, "no_btn", "no")

    def test_yes_no_cancel_dialog_returns_each_button(self):
        self._assert_button_result(MessageBoxYesNoCancel, "yes_btn", "yes")
        self._assert_button_result(MessageBoxYesNoCancel, "no_btn", "no")
        self._assert_button_result(MessageBoxYesNoCancel, "cancel_btn", "cancel")
