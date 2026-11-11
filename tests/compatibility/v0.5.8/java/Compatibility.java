import uimd.GeneratedWindowRuntime;

public class Compatibility extends CompatibilityUI
{
    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            Compatibility application = new Compatibility();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }
}
