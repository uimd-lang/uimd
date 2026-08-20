import uimd.GeneratedWindowRuntime;

public class SpecialElements extends SpecialElementsUI
{
    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            SpecialElements application = new SpecialElements();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }
}
