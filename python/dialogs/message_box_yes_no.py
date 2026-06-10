"""Standard Yes/No message box."""

try:
    from dialogs.message_box import MessageBoxBehavior
    from dialogs.message_box_yes_no_ui import MessageBoxYesNoUI
except ImportError:
    from message_box import MessageBoxBehavior
    from message_box_yes_no_ui import MessageBoxYesNoUI


class MessageBoxYesNo(MessageBoxBehavior, MessageBoxYesNoUI):
    def __init__(self, header="Message", message="", on_close=None):
        super().__init__()
        self._configure_message_box(
            header=header,
            message=message,
            on_close=on_close,
            result_by_button={
                "yes_btn": "yes",
                "no_btn": "no",
            },
            default_result="no",
        )
