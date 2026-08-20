package uimd;

public final class Version
{
    private static final String RUNTIME_VERSION = "0.5.0";

    private Version()
    {
    }

    public static String runtimeVersion()
    {
        return RUNTIME_VERSION;
    }
}
