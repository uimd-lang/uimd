package uimd;

import java.util.Map;
import java.util.function.Consumer;

public final class MessageBoxYesNoCancel extends MessageBoxYesNoCancelUI
{
    private final MessageBoxBehavior behavior;

    public MessageBoxYesNoCancel()
    {
        this("Message", "", null);
    }

    public MessageBoxYesNoCancel(String header, String message)
    {
        this(header, message, null);
    }

    public MessageBoxYesNoCancel(String header, String message, Consumer<String> onClose)
    {
        behavior = new MessageBoxBehavior(
            dialog_header,
            this.message,
            header,
            message,
            onClose,
            Map.of("yes_btn", "yes", "no_btn", "no", "cancel_btn", "cancel"),
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
    protected void onYesBtnClick()
    {
        behavior.handleButton("yes_btn");
    }

    @Override
    protected void onNoBtnClick()
    {
        behavior.handleButton("no_btn");
    }

    @Override
    protected void onCancelBtnClick()
    {
        behavior.handleButton("cancel_btn");
    }
}
