using Uimd;

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            CellsUI app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
