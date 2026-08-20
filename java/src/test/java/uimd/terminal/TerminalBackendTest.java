package uimd.terminal;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.List;

import org.junit.jupiter.api.Test;

import uimd.Size;
import uimd.Event;

final class TerminalBackendTest
{
    @Test
    void entersAndLeavesWithCanonicalTerminalModes()
    {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        TerminalBackend terminal = new TerminalBackend(
            new PrintStream(bytes, true, StandardCharsets.UTF_8));

        terminal.enter("Window\u0007 title");
        terminal.leave();

        String output = bytes.toString(StandardCharsets.UTF_8);
        assertTrue(output.startsWith("\u001b]0;Window title [java]\u0007"));
        assertTrue(output.contains("\u001b[?1049h"));
        assertTrue(output.contains("\u001b[?1006h"));
        assertTrue(output.contains("\u001b[?2004h"));
        assertTrue(output.contains("\u001b[>4;2m"));
        assertTrue(output.contains("\u001b[0m\u001b[2J\u001b[H\u001b[>4;0m"));
        assertTrue(output.contains("\u001b[?2004l"));
        assertTrue(output.contains("\u001b[?1006l"));
        assertTrue(output.endsWith("\u001b[?1049l\r\n"));
    }

    @Test
    void parsesDirectAndTextAreaPixelReports()
    {
        assertEquals(
            new Size(14, 34),
            TerminalBackend.parseTerminalPixelResponse(
                "prefix\u001b[6;34;14tsuffix",
                "\u001b[6;"));
        assertEquals(
            new Size(1260, 1190),
            TerminalBackend.parseTerminalPixelResponse(
                "\u001b[4;1190;1260t",
                "\u001b[4;"));
        assertNull(TerminalBackend.parseTerminalPixelResponse("\u001b[6;0;14t", "\u001b[6;"));
    }

    @Test
    void ownsOneInputParserAndExposesCanonicalEventReads() throws IOException
    {
        ByteArrayInputStream input = new ByteArrayInputStream("a\t".getBytes(StandardCharsets.UTF_8));
        TerminalBackend terminal = new TerminalBackend(
            input,
            new PrintStream(new ByteArrayOutputStream(), true, StandardCharsets.UTF_8));

        assertTrue(terminal.inputAvailable());
        List<Event> events = terminal.readEvents();

        assertEquals(List.of("a", "Tab"), events.stream().map(Event::key).toList());
    }

    @Test
    void terminalModeRestoreIsIdempotent()
    {
        TerminalModeGuard guard = new TerminalModeGuard();
        guard.restore();
        guard.restore();
        guard.close();
    }
}
