"""Settings dialog for the activity feed example."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

from runtime import UIApplication
from settings_ui import SettingsUI


class Settings(SettingsUI):
    """Settings dialog with confirm/cancel result callbacks."""

    def __init__(self):
        super().__init__()
        self.confirmed_settings = None
        self._on_close_callback = None

    def configure(self, auto_scroll=True, show_timestamps=True, default_type="Info"):
        self.auto_scroll.value = auto_scroll
        self.show_timestamps.value = show_timestamps
        self.default_type.selected_item = default_type

    def on_save_button_click(self):
        self._confirm()

    def on_cancel_button_click(self):
        self._cancel()

    def save(self):
        self._confirm()

    def _confirm(self):
        self.confirmed_settings = {
            "auto_scroll": self.auto_scroll.value,
            "show_timestamps": self.show_timestamps.value,
            "default_type": self.default_type.selected_item or "Info",
        }
        self._close(self.confirmed_settings)

    def _cancel(self):
        self._close(None)

    def _close(self, result):
        if self._app is not None:
            self._app.close_current()
            if self._on_close_callback is not None:
                self._on_close_callback(result)
            else:
                self._app._running = False


def main():
    app = UIApplication()
    window = Settings()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
