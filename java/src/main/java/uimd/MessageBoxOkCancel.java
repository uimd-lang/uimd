package uimd;

import java.util.Map;
import java.util.function.Consumer;

public final class MessageBoxOkCancel extends MessageBoxOkCancelUI
{
    private final MessageBoxBehavior behavior;

    public MessageBoxOkCancel()
    {
        this("Message", "", null);
    }

    public MessageBoxOkCancel(String header, String message)
    {
        this(header, message, null);
    }

    public MessageBoxOkCancel(String header, String message, Consumer<String> onClose)
    {
        behavior = new MessageBoxBehavior(
            dialog_header,
            this.message,
            header,
            message,
            onClose,
            Map.of("ok_btn", "ok", "cancel_btn", "cancel"),
            "cancel");
    }

    public String result()
    {
        return behavior.result();
    }

    public boolean closed()
    {
        return behavior.closed();
    }

    public int run(String[] arguments)
    {
        return GeneratedWindowRuntime.runGeneratedWindow(this, runtimeOptions(), arguments);
    }

    @Override
    public GeneratedWindowRuntimeOptions runtimeOptions()
    {
        return behavior.configure(super.runtimeOptions());
    }

    @Override
    protected void onOkBtnClick()
    {
        behavior.handleButton("ok_btn");
    }

    @Override
    protected void onCancelBtnClick()
    {
        behavior.handleButton("cancel_btn");
    }
}
