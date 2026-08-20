package uimd.terminal;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

public final class TerminalModeGuard implements AutoCloseable
{
    private final String savedMode;
    private boolean restored;

    public TerminalModeGuard()
    {
        savedMode = TerminalBackend.isWindows() ? "" : captureMode();
        if (!savedMode.isEmpty())
        {
            runStty("raw", "-echo", "min", "0", "time", "1");
        }
    }

    @Override
    public void close()
    {
        restore();
    }

    public void restore()
    {
        if (!restored && !savedMode.isEmpty())
        {
            runStty(savedMode);
        }
        restored = true;
    }

    private static String captureMode()
    {
        ProcessBuilder builder = new ProcessBuilder("stty", "-g");
        builder.redirectInput(ProcessBuilder.Redirect.INHERIT);
        builder.redirectError(ProcessBuilder.Redirect.DISCARD);
        try
        {
            Process process = builder.start();
            String line;
            try (BufferedReader reader = new BufferedReader(
                new InputStreamReader(process.getInputStream(), StandardCharsets.UTF_8)))
            {
                line = reader.readLine();
            }
            return process.waitFor() == 0 && line != null ? line.trim() : "";
        }
        catch (IOException | InterruptedException ignored)
        {
            if (Thread.currentThread().isInterrupted())
            {
                Thread.currentThread().interrupt();
            }
            return "";
        }
    }

    private static void runStty(String... arguments)
    {
        String[] command = new String[arguments.length + 1];
        command[0] = "stty";
        System.arraycopy(arguments, 0, command, 1, arguments.length);
        ProcessBuilder builder = new ProcessBuilder(command);
        builder.redirectInput(ProcessBuilder.Redirect.INHERIT);
        builder.redirectOutput(ProcessBuilder.Redirect.DISCARD);
        builder.redirectError(ProcessBuilder.Redirect.DISCARD);
        try
        {
            Process process = builder.start();
            process.waitFor();
        }
        catch (IOException | InterruptedException ignored)
        {
            if (Thread.currentThread().isInterrupted())
            {
                Thread.currentThread().interrupt();
            }
        }
    }
}
