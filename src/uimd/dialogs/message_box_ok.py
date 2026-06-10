"""Standard one-button message box."""

try:
    from uimd.dialogs.message_box import MessageBoxBehavior
    from uimd.dialogs.message_box_ok_ui import MessageBoxOkUI
except ImportError:
    from message_box import MessageBoxBehavior
    from message_box_ok_ui import MessageBoxOkUI


class MessageBoxOk(MessageBoxBehavior, MessageBoxOkUI):
    def __init__(self, header="Message", message="", on_close=None):
        super().__init__()
        self._configure_message_box(
            header=header,
            message=message,
            on_close=on_close,
            result_by_button={"ok_btn": "ok"},
            default_result="ok",
        )
