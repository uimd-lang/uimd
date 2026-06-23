using Uimd;

public sealed class ImageGallery : ImageGalleryUI
{
    private bool finished;

    protected override void onCloseBtnClick()
    {
        finished = true;
    }

    protected override bool shouldClose()
    {
        return finished;
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            ImageGallery app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
