package uimd.terminal;

import java.io.ByteArrayOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.PushbackInputStream;
import java.nio.charset.StandardCharsets;
import java.util.Locale;

import com.sun.jna.Function;
import com.sun.jna.NativeLibrary;
import com.sun.jna.NativeLong;
import com.sun.jna.Platform;
import com.sun.jna.Structure;

import uimd.Size;
import uimd.Event;

public final class TerminalBackend
{
    public static final int FALLBACK_TERMINAL_WIDTH = 100;
    public static final int FALLBACK_TERMINAL_HEIGHT = 32;

    private static final String ENTER_ALTERNATE_SCREEN = "\u001b[?1049h";
    private static final String EXIT_ALTERNATE_SCREEN = "\u001b[?1049l";
    private static final String HIDE_CURSOR = "\u001b[?25l";
    private static final String SHOW_CURSOR = "\u001b[?25h";
    private static final String DISABLE_AUTOWRAP = "\u001b[?7l";
    private static final String ENABLE_AUTOWRAP = "\u001b[?7h";
    private static final String ENABLE_MOUSE = "\u001b[?1000h\u001b[?1002h\u001b[?1006h";
    private static final String DISABLE_MOUSE = "\u001b[?1006l\u001b[?1002l\u001b[?1000l";
    private static final String ENABLE_BRACKETED_PASTE = "\u001b[?2004h";
    private static final String DISABLE_BRACKETED_PASTE = "\u001b[?2004l";
    private static final String ENABLE_MODIFY_OTHER_KEYS = "\u001b[>4;2m";
    private static final String DISABLE_MODIFY_OTHER_KEYS = "\u001b[>4;0m";
    private static final String HOME_CURSOR = "\u001b[H";
    private static final String CLEAR_SCREEN = "\u001b[2J";
    private static final String RESET_STYLE = "\u001b[0m";
    private static final String TITLE_PREFIX = "\u001b]0;";
    private static final String TITLE_SUFFIX = " [java]";
    private static final String TITLE_TERMINATOR = "\u0007";
    private static final String TERMINAL_CELL_PIXEL_QUERY = "\u001b[16t";
    private static final String TERMINAL_TEXT_AREA_PIXEL_QUERY = "\u001b[14t";
    private static final String TERMINAL_CELL_PIXEL_RESPONSE_PREFIX = "\u001b[6;";
    private static final String TERMINAL_TEXT_AREA_PIXEL_RESPONSE_PREFIX = "\u001b[4;";
    private static final char TERMINAL_PIXEL_RESPONSE_SEPARATOR = ';';
    private static final char TERMINAL_PIXEL_RESPONSE_TERMINATOR = 't';
    private static final int TERMINAL_PIXEL_RESPONSE_MAX_BYTES = 64;
    private static final int TERMINAL_PIXEL_QUERY_TIMEOUT_MILLISECONDS = 50;
    private static final int TERMINAL_PIXEL_QUERY_POLL_MILLISECONDS = 1;
    private static final int POSIX_STANDARD_OUTPUT = 1;
    private static final long LINUX_TIOCGWINSZ = 0x5413L;
    private static final long DARWIN_TIOCGWINSZ = 0x40087468L;

    private final PrintStream output;
    private final PushbackInputStream input;
    private final InputParser inputParser;

    public TerminalBackend(PrintStream output)
    {
        this(System.in, output);
    }

    public TerminalBackend(InputStream input, PrintStream output)
    {
        this.input = inputStream(input);
        this.output = output;
        inputParser = new InputParser(this.input);
    }

    public boolean interactive()
    {
        return System.console() != null;
    }

    public Size size()
    {
        Size environmentSize = sizeFromEnvironment();
        if (environmentSize != null)
        {
            return environmentSize;
        }
        Size sttySize = sizeFromStty();
        if (sttySize != null)
        {
            return sttySize;
        }
        return new Size(FALLBACK_TERMINAL_WIDTH, FALLBACK_TERMINAL_HEIGHT);
    }

    public static PushbackInputStream inputStream(InputStream input)
    {
        return new PushbackInputStream(input, TERMINAL_PIXEL_RESPONSE_MAX_BYTES);
    }

    public Size cellPixelSize(PushbackInputStream input)
    {
        if (isWindows())
        {
            return null;
        }
        Size ioctlSize = cellPixelSizeFromIoctl();
        if (ioctlSize != null)
        {
            return ioctlSize;
        }
        Size direct = queryTerminalPixelReport(
            input,
            TERMINAL_CELL_PIXEL_QUERY,
            TERMINAL_CELL_PIXEL_RESPONSE_PREFIX);
        if (direct != null)
        {
            return direct;
        }
        Size terminalSize = size();
        Size textArea = queryTerminalPixelReport(
            input,
            TERMINAL_TEXT_AREA_PIXEL_QUERY,
            TERMINAL_TEXT_AREA_PIXEL_RESPONSE_PREFIX);
        if (textArea == null || terminalSize.width() <= 0 || terminalSize.height() <= 0)
        {
            return null;
        }
        return new Size(
            Math.max(1, (textArea.width() + terminalSize.width() / 2) / terminalSize.width()),
            Math.max(1, (textArea.height() + terminalSize.height() / 2) / terminalSize.height()));
    }

    public Size cellPixelSize()
    {
        return cellPixelSize(input);
    }

    public java.util.List<Event> readEvents() throws IOException
    {
        return inputParser.readAvailableEvents();
    }

    public boolean inputAvailable() throws IOException
    {
        return input.available() > 0;
    }

    public void enter(String title)
    {
        output.print(titleSequence(title));
        output.print(ENTER_ALTERNATE_SCREEN);
        output.print(ENABLE_MOUSE);
        output.print(ENABLE_BRACKETED_PASTE);
        output.print(DISABLE_AUTOWRAP);
        output.print(HIDE_CURSOR);
        output.print(ENABLE_MODIFY_OTHER_KEYS);
        output.print(HOME_CURSOR);
        output.print(CLEAR_SCREEN);
        output.flush();
    }

    public void enterEmbedded()
    {
        output.print(HIDE_CURSOR);
        output.flush();
    }

    public void leave()
    {
        output.print(RESET_STYLE);
        output.print(CLEAR_SCREEN);
        output.print(HOME_CURSOR);
        output.print(DISABLE_MODIFY_OTHER_KEYS);
        output.print(SHOW_CURSOR);
        output.print(ENABLE_AUTOWRAP);
        output.print(DISABLE_BRACKETED_PASTE);
        output.print(DISABLE_MOUSE);
        output.print(EXIT_ALTERNATE_SCREEN);
        output.print("\r\n");
        output.flush();
    }

    public void leaveEmbedded()
    {
        output.print(RESET_STYLE);
        output.print(SHOW_CURSOR);
        output.flush();
    }

    public void write(String value)
    {
        output.print(value);
        output.flush();
    }

    private static Size sizeFromEnvironment()
    {
        try
        {
            String columns = System.getenv("COLUMNS");
            String lines = System.getenv("LINES");
            if (columns == null || lines == null)
            {
                return null;
            }
            int width = Integer.parseInt(columns);
            int height = Integer.parseInt(lines);
            return new Size(Math.max(1, width), Math.max(1, height));
        }
        catch (NumberFormatException ignored)
        {
            return null;
        }
    }

    private static Size sizeFromStty()
    {
        if (isWindows())
        {
            return null;
        }
        ProcessBuilder builder = new ProcessBuilder("stty", "size");
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
            if (process.waitFor() != 0 || line == null)
            {
                return null;
            }
            String[] fields = line.trim().split("\\s+");
            if (fields.length != 2)
            {
                return null;
            }
            int height = Integer.parseInt(fields[0]);
            int width = Integer.parseInt(fields[1]);
            return new Size(Math.max(1, width), Math.max(1, height));
        }
        catch (IOException | InterruptedException | NumberFormatException ignored)
        {
            if (Thread.currentThread().isInterrupted())
            {
                Thread.currentThread().interrupt();
            }
            return null;
        }
    }

    static boolean isWindows()
    {
        return System.getProperty("os.name", "").toLowerCase(Locale.ROOT).contains("win");
    }

    static Size parseTerminalPixelResponse(String response, String responsePrefix)
    {
        PixelResponse parsed = parseTerminalPixelResponseDetails(response, responsePrefix);
        return parsed == null ? null : parsed.size();
    }

    private Size queryTerminalPixelReport(
        PushbackInputStream input,
        String query,
        String responsePrefix)
    {
        output.print(query);
        output.flush();
        long deadline = System.nanoTime()
            + TERMINAL_PIXEL_QUERY_TIMEOUT_MILLISECONDS * 1_000_000L;
        ByteArrayOutputStream response = new ByteArrayOutputStream();
        try
        {
            while (response.size() < TERMINAL_PIXEL_RESPONSE_MAX_BYTES
                && System.nanoTime() < deadline)
            {
                int available = input.available();
                if (available <= 0)
                {
                    Thread.sleep(TERMINAL_PIXEL_QUERY_POLL_MILLISECONDS);
                    continue;
                }
                int count = Math.min(
                    available,
                    TERMINAL_PIXEL_RESPONSE_MAX_BYTES - response.size());
                byte[] buffer = input.readNBytes(count);
                response.write(buffer);
                byte[] received = response.toByteArray();
                PixelResponse parsed = parseTerminalPixelResponseDetails(
                    new String(received, StandardCharsets.ISO_8859_1),
                    responsePrefix);
                if (parsed != null)
                {
                    unreadOutsideResponse(input, received, parsed.start(), parsed.end());
                    return parsed.size();
                }
            }
        }
        catch (IOException ignored)
        {
            return null;
        }
        catch (InterruptedException ignored)
        {
            Thread.currentThread().interrupt();
        }
        unread(input, response.toByteArray());
        return null;
    }

    private static Size cellPixelSizeFromIoctl()
    {
        try
        {
            PosixWindowSize size = new PosixWindowSize();
            String functionName = Platform.isMac() ? "__ioctl" : "ioctl";
            long request = Platform.isMac() ? DARWIN_TIOCGWINSZ : LINUX_TIOCGWINSZ;
            Function ioctl = NativeLibrary.getInstance(Platform.C_LIBRARY_NAME)
                .getFunction(functionName);
            int result = ioctl.invokeInt(new Object[] {
                POSIX_STANDARD_OUTPUT,
                new NativeLong(request),
                size,
            });
            size.read();
            int rows = Short.toUnsignedInt(size.rows);
            int cols = Short.toUnsignedInt(size.cols);
            int pixelWidth = Short.toUnsignedInt(size.pixelWidth);
            int pixelHeight = Short.toUnsignedInt(size.pixelHeight);
            if (result != 0 || rows <= 0 || cols <= 0 || pixelWidth <= 0 || pixelHeight <= 0)
            {
                return null;
            }
            return new Size(
                Math.max(1, (pixelWidth + cols / 2) / cols),
                Math.max(1, (pixelHeight + rows / 2) / rows));
        }
        catch (RuntimeException | UnsatisfiedLinkError ignored)
        {
            return null;
        }
    }

    private static PixelResponse parseTerminalPixelResponseDetails(
        String response,
        String responsePrefix)
    {
        int prefix = response.indexOf(responsePrefix);
        if (prefix < 0)
        {
            return null;
        }
        int heightStart = prefix + responsePrefix.length();
        int separator = response.indexOf(TERMINAL_PIXEL_RESPONSE_SEPARATOR, heightStart);
        int terminator = separator < 0
            ? -1
            : response.indexOf(TERMINAL_PIXEL_RESPONSE_TERMINATOR, separator + 1);
        if (separator < 0 || terminator < 0)
        {
            return null;
        }
        try
        {
            int height = Integer.parseInt(response.substring(heightStart, separator));
            int width = Integer.parseInt(response.substring(separator + 1, terminator));
            if (width <= 0 || height <= 0)
            {
                return null;
            }
            return new PixelResponse(new Size(width, height), prefix, terminator + 1);
        }
        catch (NumberFormatException ignored)
        {
            return null;
        }
    }

    private static void unreadOutsideResponse(
        PushbackInputStream input,
        byte[] received,
        int start,
        int end)
    {
        byte[] remaining = new byte[received.length - (end - start)];
        System.arraycopy(received, 0, remaining, 0, start);
        System.arraycopy(received, end, remaining, start, received.length - end);
        unread(input, remaining);
    }

    private static void unread(PushbackInputStream input, byte[] bytes)
    {
        if (bytes.length == 0)
        {
            return;
        }
        try
        {
            input.unread(bytes);
        }
        catch (IOException ignored)
        {
            // Input preservation is best-effort when a platform stream closes during startup.
        }
    }

    private static String titleSequence(String title)
    {
        String safeTitle = title == null ? "" : title.replaceAll("[\\x00-\\x1f\\x7f]", "");
        return TITLE_PREFIX + safeTitle + TITLE_SUFFIX + TITLE_TERMINATOR;
    }

    private record PixelResponse(Size size, int start, int end)
    {
    }

    @Structure.FieldOrder({"rows", "cols", "pixelWidth", "pixelHeight"})
    public static final class PosixWindowSize extends Structure
    {
        public short rows;
        public short cols;
        public short pixelWidth;
        public short pixelHeight;
    }
}
