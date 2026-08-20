package uimd;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.TimeUnit;

public final class RuntimeClipboard
{
    private static final int COPY_TIMEOUT_MILLISECONDS = 200;
    private static final List<List<String>> COPY_COMMANDS = List.of(
        List.of("pbcopy"),
        List.of("wl-copy"),
        List.of("xclip", "-selection", "clipboard"),
        List.of("xsel", "--clipboard", "--input"));
    private static String text = "";

    private RuntimeClipboard()
    {
    }

    public static synchronized boolean copyTextToClipboard(String value)
    {
        text = value == null ? "" : value;
        if (text.isEmpty() || System.console() == null)
        {
            return false;
        }
        for (List<String> command : COPY_COMMANDS)
        {
            if (run(command, text))
            {
                return true;
            }
        }
        if (System.getProperty("os.name", "").toLowerCase(Locale.ROOT).contains("mac"))
        {
            return run(List.of(
                "osascript",
                "-e",
                "on run argv",
                "-e",
                "set the clipboard to item 1 of argv",
                "-e",
                "end run",
                "--",
                text), "");
        }
        return false;
    }

    public static synchronized String runtimeClipboardText()
    {
        return text;
    }

    private static boolean run(List<String> command, String input)
    {
        Process process;
        try
        {
            process = new ProcessBuilder(command)
                .redirectOutput(ProcessBuilder.Redirect.DISCARD)
                .redirectError(ProcessBuilder.Redirect.DISCARD)
                .start();
            try (OutputStream stream = process.getOutputStream())
            {
                stream.write(input.getBytes(StandardCharsets.UTF_8));
            }
            if (!process.waitFor(COPY_TIMEOUT_MILLISECONDS, TimeUnit.MILLISECONDS))
            {
                process.destroyForcibly();
                return false;
            }
            return process.exitValue() == 0;
        }
        catch (IOException exception)
        {
            return false;
        }
        catch (InterruptedException exception)
        {
            Thread.currentThread().interrupt();
            return false;
        }
    }
}
