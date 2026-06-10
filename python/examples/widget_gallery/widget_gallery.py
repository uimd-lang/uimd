"""Interactive widget gallery example."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from runtime import UIApplication
from widget_gallery_ui import WidgetGalleryUI


DEFAULT_NAME = "Ada Lovelace"
DEFAULT_COUNT = 3
DEFAULT_THEME = "Dark"
DEFAULT_MODE = "Preview"


class WidgetGallery(WidgetGalleryUI):
    """Small app that exercises the basic runtime widgets."""

    def open(self):
        super().open()
        self.spans.set_spans([
            ("SpanLabel", "#facc15"),
            (" can mix ", None),
            ("foreground", "#38bdf8"),
            (" and ", None),
            ("background", "#111827", "#f97316"),
            (" colors.", None),
        ])
        self._refresh_status("Ready")

    def on_close_btn_click(self):
        self._close_app()

    def on_apply_btn_click(self):
        self._refresh_status("Applied current values")

    def on_reset_btn_click(self):
        self._reset_values()

    def on_name_input_change(self, value):
        pass

    def on_count_input_change(self, value):
        pass

    def on_enabled_check_change(self, value):
        pass

    def on_theme_combo_change(self, value):
        pass

    def on_mode_list_selection_change(self, value):
        pass

    def on_name_input_submit(self, value):
        pass

    def on_count_input_submit(self, value):
        pass

    def _reset_values(self):
        self.name_input.value = DEFAULT_NAME
        self.count_input.value = DEFAULT_COUNT
        self.enabled_check.value = True
        self.theme_combo.selected_item = DEFAULT_THEME
        self.mode_list.selected_items = [DEFAULT_MODE]
        self._refresh_status("Values reset")

    def _refresh_status(self, prefix):
        enabled = "enabled" if self.enabled_check.value else "disabled"
        self.summary.text = "\n".join([
            "values:",
            f"  name: {self.name_input.value}",
            f"  count: {int(self.count_input.value)}",
            f"  theme: {self.theme_combo.selected_item}",
            f"  mode: {self.mode_list.selected_item}",
            f"  enabled: {enabled == 'enabled'}",
        ])

    def _close_app(self):
        if self._app is not None:
            self._app._running = False


def main():
    app = UIApplication()
    window = WidgetGallery()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
