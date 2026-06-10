"""Standard OK/Cancel message box."""

try:
    from uimd.dialogs.message_box import MessageBoxBehavior
    from uimd.dialogs.message_box_ok_cancel_ui import MessageBoxOkCancelUI
except ImportError:
    from message_box import MessageBoxBehavior
    from message_box_ok_cancel_ui import MessageBoxOkCancelUI


class MessageBoxOkCancel(MessageBoxBehavior, MessageBoxOkCancelUI):
    def __init__(self, header="Message", message="", on_close=None):
        super().__init__()
        self._configure_message_box(
            header=header,
            message=message,
            on_close=on_close,
            result_by_button={
                "ok_btn": "ok",
                "cancel_btn": "cancel",
            },
            default_result="cancel",
        )
