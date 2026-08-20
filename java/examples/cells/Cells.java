import uimd.GeneratedWindowRuntime;

public class Cells extends CellsUI
{
    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            Cells application = new Cells();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }
}
