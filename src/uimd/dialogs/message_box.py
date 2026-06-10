"""Shared behavior for standard message box dialogs."""

import time


MESSAGE_BOX_ESCAPE_FLASH_SECONDS = 0.18


class MessageBoxBehavior:
    """Common configurable header/message and button result handling."""

    def _configure_message_box(self, header, message, on_close, result_by_button, default_result=None):
        self._header_text = header
        self._message_text = message
        self._on_close = on_close
        self._result_by_button = dict(result_by_button)
        self._default_result = default_result
        self.result = None

    def open(self):
        super().open()
        if hasattr(self, "dialog_header"):
            self.dialog_header.text = self._header_text
        if hasattr(self, "message"):
            self.message.text = self._message_text
        button, _ = self._button_for_result(self._default_result)
        if button is not None:
            self.set_focus(button)

    def elementchanged(self, element, value):
        result = self._result_by_button.get(getattr(element, "name", ""))
        if result is not None:
            self._close(result)

    def handle_key(self, key):
        if key == "Escape":
            button, result = self._escape_button_result()
            if button is None:
                return False
            self.set_focus(button)
            app = getattr(self, "_app", None)
            if app is not None:
                app.mark_dirty()
                app._render_to_terminal()
            time.sleep(MESSAGE_BOX_ESCAPE_FLASH_SECONDS)
            self._close(result)
            return True
        return super().handle_key(key)

    def _button_for_result(self, target_result):
        if target_result is None:
            return None, None
        for button_name, result in self._result_by_button.items():
            if result == target_result:
                return getattr(self, button_name, None), result
        return None, None

    def _escape_button_result(self):
        for preferred in ("cancel", "no", "ok"):
            for button_name, result in self._result_by_button.items():
                if result == preferred:
                    return getattr(self, button_name, None), result
        for button_name, result in self._result_by_button.items():
            return getattr(self, button_name, None), result
        return None, None

    def _close(self, result):
        self.result = result
        app = self._app
        if app is not None:
            app.close_current()
        if self._on_close:
            self._on_close(result)
