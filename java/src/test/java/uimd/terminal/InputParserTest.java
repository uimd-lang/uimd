package uimd.terminal;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.concurrent.TimeUnit;

import org.junit.jupiter.api.Test;

import uimd.Event;
import uimd.EventType;
import uimd.Point;

final class InputParserTest
{
    private static final int SPLIT_ESCAPE_DELAY_MILLIS = 30;

    @Test
    void parsesKeyboardMouseAndUtf8Events() throws Exception
    {
        assertEquals("Up", parse("\u001b[A").key());
        assertEquals("Shift+Tab", parse("\u001b[Z").key());
        assertEquals("ž", parse("ž").key());

        Event wheelUp = parse("\u001b[<64;9;4M");
        assertEquals(EventType.MOUSE_WHEEL, wheelUp.type());
        assertEquals(new Point(3, 8), wheelUp.position());
        assertEquals(1, wheelUp.wheelDelta());

        Event wheelDown = parse("\u001b[<65;2;7M");
        assertEquals(-1, wheelDown.wheelDelta());
    }

    @Test
    void parsesModifiedCursorCodepointAndLegacyKeys() throws Exception
    {
        assertEquals("Shift+Left", parse("\u001b[1;2D").key());
        assertEquals("Ctrl+Up", parse("\u001b[1;5A").key());
        assertEquals("Alt+Left", parse("\u001bb").key());
        assertEquals("Alt+Right", parse("\u001bf").key());
        assertEquals("Home", parse("\u001b[7~").key());
        assertEquals("End", parse("\u001b[8~").key());
        assertEquals("Alt+Enter", parse("\u001b[13;3u").key());
        assertEquals("Alt+Enter", parse("\u001b[27;5;13~").key());
        assertEquals("Ctrl+C", parse("\u001b[99;5u").key());
        assertEquals("Ctrl+C", parse("\u001b[27;6;67~").key());
        assertEquals("cmd_c", parse("\u001b[99;9u").key());
        assertEquals("cmd_v", parse("\u001b[118;10u").key());
        assertNull(parse("\u001b[6;34;14t"));
    }

    @Test
    void preservesCoordinatesOutsideTheTerminalInsteadOfTurningThemIntoTopLeftClicks()
        throws Exception
    {
        Event press = parse("\u001b[<0;0;0M");
        assertEquals(EventType.MOUSE_PRESS, press.type());
        assertEquals(new Point(-1, -1), press.position());
    }

    @Test
    void parsesBracketedPasteAndDiscardsDelayedPixelReports() throws Exception
    {
        Event paste = parse("\u001b[200~split paste\nvalue\u001b[201~");
        assertEquals(EventType.PASTE, paste.type());
        assertEquals("split paste\nvalue", paste.text());

        InputParser parser = new InputParser(new ByteArrayInputStream(
            "\u001b[6;34;14tx".getBytes(StandardCharsets.UTF_8)));
        List<Event> events = parser.readAvailableEvents();
        assertEquals(1, events.size());
        assertEquals("x", events.get(0).key());
    }

    @Test
    void waitsForASeparatedEscapeSequenceContinuation() throws Exception
    {
        InputParser parser = new InputParser(new SplitEscapeInputStream("\u001b[B"));
        assertEquals("Down", parser.readEvent().key());
    }

    @Test
    void coalescesOnlyConsecutiveWheelReportsWithLatestPositionAndClamp()
    {
        List<Event> events = InputParser.coalesceMouseWheelEvents(List.of(
            Event.wheel(new Point(1, 1), 8),
            Event.wheel(new Point(2, 2), 8),
            Event.key("Enter"),
            Event.wheel(new Point(3, 3), 1),
            Event.wheel(new Point(4, 4), -1),
            Event.mouse(EventType.MOUSE_PRESS, new Point(5, 5))));

        assertEquals(3, events.size());
        assertEquals(12, events.get(0).wheelDelta());
        assertEquals(new Point(2, 2), events.get(0).position());
        assertEquals("Enter", events.get(1).key());
        assertEquals(EventType.MOUSE_PRESS, events.get(2).type());
    }

    private static Event parse(String bytes) throws Exception
    {
        InputParser parser = new InputParser(new ByteArrayInputStream(bytes.getBytes(StandardCharsets.UTF_8)));
        return parser.readEvent();
    }

    private static final class SplitEscapeInputStream extends InputStream
    {
        private final byte[] bytes;
        private final long continuationTime;
        private int index;

        private SplitEscapeInputStream(String value)
        {
            bytes = value.getBytes(StandardCharsets.UTF_8);
            continuationTime = System.nanoTime()
                + TimeUnit.MILLISECONDS.toNanos(SPLIT_ESCAPE_DELAY_MILLIS);
        }

        @Override
        public int read()
        {
            if (index >= bytes.length)
            {
                return -1;
            }
            return bytes[index++] & 0xff;
        }

        @Override
        public int available()
        {
            if (index == 0)
            {
                return 1;
            }
            return System.nanoTime() >= continuationTime ? bytes.length - index : 0;
        }
    }
}
