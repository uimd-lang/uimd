package uimd;

import java.util.Map;
import java.util.function.Consumer;

public final class MessageBoxYesNo extends MessageBoxYesNoUI
{
    private final MessageBoxBehavior behavior;

    public MessageBoxYesNo()
    {
        this("Message", "", null);
    }

    public MessageBoxYesNo(String header, String message)
    {
        this(header, message, null);
    }

    public MessageBoxYesNo(String header, String message, Consumer<String> onClose)
    {
        behavior = new MessageBoxBehavior(
            dialog_header,
            this.message,
            header,
            message,
            onClose,
            Map.of("yes_btn", "yes", "no_btn", "no"),
            "no");
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
}
