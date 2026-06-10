"""Standard Yes/No/Cancel message box."""

try:
    from uimd.dialogs.message_box import MessageBoxBehavior
    from uimd.dialogs.message_box_yes_no_cancel_ui import MessageBoxYesNoCancelUI
except ImportError:
    from message_box import MessageBoxBehavior
    from message_box_yes_no_cancel_ui import MessageBoxYesNoCancelUI


class MessageBoxYesNoCancel(MessageBoxBehavior, MessageBoxYesNoCancelUI):
    def __init__(self, header="Message", message="", on_close=None):
        super().__init__()
        self._configure_message_box(
            header=header,
            message=message,
            on_close=on_close,
            result_by_button={
                "yes_btn": "yes",
                "no_btn": "no",
                "cancel_btn": "cancel",
            },
            default_result="cancel",
        )
