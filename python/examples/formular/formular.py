"""Application logic for formular.uimd - extends generated UI."""

import sys
import os

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from runtime import UIApplication
from formular_ui import FormularUI

ACTION_SAVE = "save"
ACTION_CANCEL = "cancel"
YAML_INDENT = "  "


class FormApp(FormularUI):
    """Custom form with application logic."""

    def __init__(self):
        super().__init__()
        self.output_yaml = ""
        self._app = None

    def open(self):
        super().open()

    def on_save_btn_click(self):
        self._finish(ACTION_SAVE)

    def on_cancel_btn_click(self):
        self._finish(ACTION_CANCEL)

    def save(self):
        self._finish(ACTION_SAVE)

    def _finish(self, action):
        self.output_yaml = self._to_yaml(self._form_data(action))
        if self._app is not None:
            self._app.close_current()
            self._app._running = False

    def _form_data(self, action):
        return {
            "action": action,
            "form": {
                "name": self.name_input.value,
                "email": self.email_input.value,
                "age": self.age_input.value,
                "description": self.description_input.value,
                "country": self.country_combo.selected_item,
                "roles": self.role_listbox.selected_items,
                "email_notifications": self.notify_check.value,
                "accepted_terms": self.terms_check.value,
            },
        }

    def _to_yaml(self, data, indent=0):
        lines = []
        prefix = YAML_INDENT * indent
        if isinstance(data, dict):
            for key, value in data.items():
                if isinstance(value, (dict, list)):
                    lines.append(f"{prefix}{key}:")
                    lines.append(self._to_yaml(value, indent + 1).rstrip())
                else:
                    lines.append(f"{prefix}{key}: {self._yaml_scalar(value)}")
        elif isinstance(data, list):
            if not data:
                lines.append(f"{prefix}[]")
            for item in data:
                if isinstance(item, (dict, list)):
                    lines.append(f"{prefix}-")
                    lines.append(self._to_yaml(item, indent + 1).rstrip())
                else:
                    lines.append(f"{prefix}- {self._yaml_scalar(item)}")
        return "\n".join(lines) + "\n"

    @staticmethod
    def _yaml_scalar(value):
        if isinstance(value, bool):
            return "true" if value else "false"
        if value is None:
            return "null"
        if isinstance(value, (int, float)):
            return str(value)
        text = str(value)
        if text == "":
            return '""'
        return text

    def focused(self, element, value):
        pass


def main():
    app = UIApplication()
    window = FormApp()
    window._app = app
    app.open(window)
    result = app.run()
    if window.output_yaml:
        print(window.output_yaml, end="")
    return result


if __name__ == "__main__":
    sys.exit(main())
