using Uimd;

public sealed class FormApp : FormularUI
{
    private bool closeRequested;

    protected override void onSaveBtnClick()
    {
        closeRequested = true;
    }

    protected override void onCancelBtnClick()
    {
        closeRequested = true;
    }

    protected override bool shouldClose()
    {
        return closeRequested;
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            FormApp app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
