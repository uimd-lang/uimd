package uimd.terminal;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import uimd.Event;
import uimd.EventType;
import uimd.Point;

public final class InputParser
{
    private static final int ESCAPE_BYTE = 0x1b;
    private static final int CTRL_C_BYTE = 0x03;
    private static final int CARRIAGE_RETURN_BYTE = 0x0d;
    private static final int LINE_FEED_BYTE = 0x0a;
    private static final int TAB_BYTE = 0x09;
    private static final int BACKSPACE_BYTE = 0x08;
    private static final int DELETE_BACKSPACE_BYTE = 0x7f;
    private static final int ASCII_MAX = 0x7f;
    private static final int UTF8_TWO_BYTE_PREFIX = 0xc0;
    private static final int UTF8_THREE_BYTE_PREFIX = 0xe0;
    private static final int UTF8_FOUR_BYTE_PREFIX = 0xf0;
    private static final int UTF8_PREFIX_MASK = 0xf0;
    private static final int UTF8_THREE_PREFIX_MASK = 0xe0;
    private static final int ESCAPE_SEQUENCE_WAIT_MILLIS = 50;
    private static final int ESCAPE_SEQUENCE_MAX_BYTES = 256;
    private static final int WHEEL_DELTA_MIN = -12;
    private static final int WHEEL_DELTA_MAX = 12;
    private static final int ENTER_CODEPOINT = 13;
    private static final int CTRL_C_CODEPOINT = 3;
    private static final int UPPERCASE_C_CODEPOINT = 67;
    private static final int LOWERCASE_C_CODEPOINT = 99;
    private static final int UPPERCASE_V_CODEPOINT = 86;
    private static final int LOWERCASE_V_CODEPOINT = 118;
    private static final int NO_MODIFIER = 1;
    private static final int SHIFT_MODIFIER = 2;
    private static final int ALT_MODIFIER = 3;
    private static final int ALT_SHIFT_MODIFIER = 4;
    private static final int CTRL_MODIFIER = 5;
    private static final int CTRL_SHIFT_MODIFIER = 6;
    private static final int META_MODIFIER = 9;
    private static final int META_SHIFT_MODIFIER = 10;
    private static final String BRACKETED_PASTE_START = "\u001b[200~";
    private static final String BRACKETED_PASTE_END = "\u001b[201~";
    private static final String TERMINAL_CELL_PIXEL_RESPONSE_PREFIX = "\u001b[6;";
    private static final String TERMINAL_TEXT_AREA_PIXEL_RESPONSE_PREFIX = "\u001b[4;";

    private final InputStream input;

    public InputParser(InputStream input)
    {
        this.input = input;
    }

    public Event readEvent() throws IOException
    {
        int value = input.read();
        if (value < 0)
        {
            return null;
        }
        if (value == CTRL_C_BYTE)
        {
            return Event.key("Ctrl+C");
        }
        if (value == CARRIAGE_RETURN_BYTE || value == LINE_FEED_BYTE)
        {
            return Event.key("Enter");
        }
        if (value == TAB_BYTE)
        {
            return Event.key("Tab");
        }
        if (value == BACKSPACE_BYTE || value == DELETE_BACKSPACE_BYTE)
        {
            return Event.key("Backspace");
        }
        if (value == ESCAPE_BYTE)
        {
            return eventFromEscapeSequence(readEscapeSequence());
        }
        return Event.key(readUtf8Character(value));
    }

    public List<Event> readAvailableEvents() throws IOException
    {
        List<Event> events = new ArrayList<>();
        while (true)
        {
            Event event = readEvent();
            if (event != null)
            {
                events.add(event);
            }
            if (input.available() <= 0)
            {
                break;
            }
        }
        return coalesceMouseWheelEvents(events);
    }

    public static List<Event> coalesceMouseWheelEvents(List<Event> events)
    {
        List<Event> result = new ArrayList<>(events.size());
        int index = 0;
        while (index < events.size())
        {
            Event event = events.get(index);
            if (event.type() != EventType.MOUSE_WHEEL)
            {
                result.add(event);
                ++index;
                continue;
            }

            int delta = 0;
            Point position = event.position();
            while (index < events.size()
                && events.get(index).type() == EventType.MOUSE_WHEEL)
            {
                Event wheel = events.get(index);
                delta += wheel.wheelDelta();
                position = wheel.position();
                ++index;
            }
            delta = Math.max(WHEEL_DELTA_MIN, Math.min(WHEEL_DELTA_MAX, delta));
            if (delta != 0)
            {
                result.add(Event.wheel(position, delta));
            }
        }
        return result;
    }

    private String readEscapeSequence() throws IOException
    {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        bytes.write(ESCAPE_BYTE);
        waitForPendingInput();
        while (input.available() > 0 && bytes.size() < ESCAPE_SEQUENCE_MAX_BYTES)
        {
            int value = input.read();
            if (value < 0)
            {
                break;
            }
            bytes.write(value);
            String current = bytes.toString(StandardCharsets.UTF_8);
            if (BRACKETED_PASTE_START.equals(current))
            {
                readBracketedPaste(bytes);
                break;
            }
            if (escapeSequenceComplete(current))
            {
                break;
            }
            waitForPendingInput();
        }
        return bytes.toString(StandardCharsets.UTF_8);
    }

    private void readBracketedPaste(ByteArrayOutputStream bytes) throws IOException
    {
        while (true)
        {
            int value = input.read();
            if (value < 0)
            {
                return;
            }
            bytes.write(value);
            String current = bytes.toString(StandardCharsets.UTF_8);
            if (current.endsWith(BRACKETED_PASTE_END))
            {
                return;
            }
        }
    }

    private void waitForPendingInput()
    {
        if (availableWithoutFailure())
        {
            return;
        }
        try
        {
            Thread.sleep(ESCAPE_SEQUENCE_WAIT_MILLIS);
        }
        catch (InterruptedException exception)
        {
            Thread.currentThread().interrupt();
        }
    }

    private boolean availableWithoutFailure()
    {
        try
        {
            return input.available() > 0;
        }
        catch (IOException ignored)
        {
            return false;
        }
    }

    private static boolean escapeSequenceComplete(String sequence)
    {
        if (sequence.length() <= 1
            || (sequence.startsWith("\u001b[") && sequence.length() <= 2)
            || (sequence.startsWith("\u001bO") && sequence.length() <= 2))
        {
            return false;
        }
        char last = sequence.charAt(sequence.length() - 1);
        return last >= '@' && last <= '~';
    }

    private static Event eventFromEscapeSequence(String sequence)
    {
        if ("\u001b".equals(sequence))
        {
            return Event.key("Escape");
        }
        if ("\u001b\r".equals(sequence)
            || "\u001b\n".equals(sequence)
            || "\u001b\r\n".equals(sequence)
            || "\u001b[13;3u".equals(sequence)
            || "\u001b[13;5u".equals(sequence)
            || "\u001b[27;3;13~".equals(sequence)
            || "\u001b[27;5;13~".equals(sequence))
        {
            return Event.key("Alt+Enter");
        }
        if (sequence.startsWith("\u001b[<"))
        {
            Event mouse = parseSgrMouse(sequence);
            if (mouse != null)
            {
                return mouse;
            }
        }
        if (sequence.startsWith(BRACKETED_PASTE_START) && sequence.endsWith(BRACKETED_PASTE_END))
        {
            int start = BRACKETED_PASTE_START.length();
            int end = sequence.length() - BRACKETED_PASTE_END.length();
            return Event.paste(sequence.substring(start, end));
        }
        if ((sequence.startsWith(TERMINAL_CELL_PIXEL_RESPONSE_PREFIX)
            || sequence.startsWith(TERMINAL_TEXT_AREA_PIXEL_RESPONSE_PREFIX))
            && sequence.endsWith("t"))
        {
            return null;
        }
        String exactKey = switch (sequence)
        {
            case "\u001b[A", "\u001bOA" -> "Up";
            case "\u001b[B", "\u001bOB" -> "Down";
            case "\u001b[C", "\u001bOC" -> "Right";
            case "\u001b[D", "\u001bOD" -> "Left";
            case "\u001b[Z" -> "Shift+Tab";
            case "\u001b[H", "\u001bOH", "\u001b[1~" -> "Home";
            case "\u001b[7~" -> "Home";
            case "\u001b[F", "\u001bOF", "\u001b[4~", "\u001b[8~" -> "End";
            case "\u001b[3~" -> "Delete";
            case "\u001b[5~" -> "PageUp";
            case "\u001b[6~" -> "PageDown";
            case "\u001bb" -> "Alt+Left";
            case "\u001bf" -> "Alt+Right";
            default -> null;
        };
        if (exactKey != null)
        {
            return Event.key(exactKey);
        }
        String modifiedKey = modifiedKey(sequence);
        if (modifiedKey != null)
        {
            return Event.key(modifiedKey);
        }
        String cursorKey = csiKey(sequence);
        if (cursorKey != null)
        {
            return Event.key(cursorKey);
        }
        return Event.key("Escape");
    }

    private static String modifiedKey(String sequence)
    {
        if (sequence.startsWith("\u001b[") && sequence.endsWith("u"))
        {
            String[] fields = sequence.substring(2, sequence.length() - 1).split(";", -1);
            if (fields.length == 2)
            {
                Integer codepoint = integer(fields[0]);
                Integer modifier = integer(fields[1]);
                if (codepoint != null && modifier != null)
                {
                    return modifiedCodepointKey(codepoint, modifier);
                }
            }
        }
        if (sequence.startsWith("\u001b[27;") && sequence.endsWith("~"))
        {
            String[] fields = sequence.substring(2, sequence.length() - 1).split(";", -1);
            if (fields.length == 3)
            {
                Integer modifier = integer(fields[1]);
                Integer codepoint = integer(fields[2]);
                if (modifier != null
                    && codepoint != null
                    && isControlCCodepoint(codepoint, modifier))
                {
                    return "Ctrl+C";
                }
            }
        }
        return null;
    }

    private static String modifiedCodepointKey(int codepoint, int modifier)
    {
        if (modifier == NO_MODIFIER)
        {
            return null;
        }
        if (codepoint == ENTER_CODEPOINT)
        {
            return "Alt+Enter";
        }
        if (isControlCCodepoint(codepoint, modifier))
        {
            return "Ctrl+C";
        }
        if ((codepoint == LOWERCASE_C_CODEPOINT || codepoint == UPPERCASE_C_CODEPOINT)
            && (modifier == META_MODIFIER || modifier == META_SHIFT_MODIFIER))
        {
            return "cmd_c";
        }
        if ((codepoint == LOWERCASE_V_CODEPOINT || codepoint == UPPERCASE_V_CODEPOINT)
            && (modifier == META_MODIFIER || modifier == META_SHIFT_MODIFIER))
        {
            return "cmd_v";
        }
        return null;
    }

    private static boolean isControlCCodepoint(int codepoint, int modifier)
    {
        return codepoint == CTRL_C_CODEPOINT
            || ((codepoint == LOWERCASE_C_CODEPOINT || codepoint == UPPERCASE_C_CODEPOINT)
                && (modifier == CTRL_MODIFIER || modifier == CTRL_SHIFT_MODIFIER));
    }

    private static String csiKey(String sequence)
    {
        if (!sequence.startsWith("\u001b[") || sequence.length() < 3)
        {
            return null;
        }
        char finalCharacter = sequence.charAt(sequence.length() - 1);
        String key = keyForCsiFinal(finalCharacter);
        if (key == null || sequence.length() == 3)
        {
            return key;
        }
        String payload = sequence.substring(2, sequence.length() - 1);
        int separator = payload.lastIndexOf(';');
        Integer modifier = separator < 0 ? null : integer(payload.substring(separator + 1));
        return modifier == null ? null : modifiedCursorKey(key, modifier);
    }

    private static String keyForCsiFinal(char finalCharacter)
    {
        return switch (finalCharacter)
        {
            case 'A' -> "Up";
            case 'B' -> "Down";
            case 'C' -> "Right";
            case 'D' -> "Left";
            case 'H' -> "Home";
            case 'F' -> "End";
            case 'Z' -> "Shift+Tab";
            default -> null;
        };
    }

    private static String modifiedCursorKey(String key, int modifier)
    {
        return switch (modifier)
        {
            case SHIFT_MODIFIER -> "Shift+" + key;
            case ALT_MODIFIER -> "Alt+" + key;
            case ALT_SHIFT_MODIFIER -> "Alt+Shift+" + key;
            case CTRL_MODIFIER -> "Ctrl+" + key;
            case CTRL_SHIFT_MODIFIER -> "Ctrl+Shift+" + key;
            default -> key;
        };
    }

    private static Integer integer(String value)
    {
        try
        {
            return Integer.valueOf(value);
        }
        catch (NumberFormatException ignored)
        {
            return null;
        }
    }

    private static Event parseSgrMouse(String sequence)
    {
        char terminator = sequence.charAt(sequence.length() - 1);
        if (terminator != 'M' && terminator != 'm')
        {
            return null;
        }
        String[] fields = sequence.substring(3, sequence.length() - 1).split(";");
        if (fields.length != 3)
        {
            return null;
        }
        try
        {
            int button = Integer.parseInt(fields[0]);
            int col = Integer.parseInt(fields[1]) - 1;
            int row = Integer.parseInt(fields[2]) - 1;
            Point position = new Point(row, col);
            if ((button & 64) != 0)
            {
                return Event.wheel(position, (button & 1) == 0 ? 1 : -1);
            }
            if (terminator == 'm')
            {
                return Event.mouse(EventType.MOUSE_RELEASE, position);
            }
            if ((button & 32) != 0)
            {
                return Event.mouse(EventType.MOUSE_DRAG, position);
            }
            return Event.mouse(EventType.MOUSE_PRESS, position);
        }
        catch (NumberFormatException ignored)
        {
            return null;
        }
    }

    private String readUtf8Character(int firstByte) throws IOException
    {
        if (firstByte <= ASCII_MAX)
        {
            return Character.toString(firstByte);
        }
        int length;
        if ((firstByte & UTF8_THREE_PREFIX_MASK) == UTF8_TWO_BYTE_PREFIX)
        {
            length = 2;
        }
        else if ((firstByte & UTF8_PREFIX_MASK) == UTF8_THREE_BYTE_PREFIX)
        {
            length = 3;
        }
        else if ((firstByte & UTF8_PREFIX_MASK) == UTF8_FOUR_BYTE_PREFIX)
        {
            length = 4;
        }
        else
        {
            return "?";
        }
        byte[] bytes = new byte[length];
        bytes[0] = (byte) firstByte;
        for (int index = 1; index < length; ++index)
        {
            int value = input.read();
            if (value < 0)
            {
                return "?";
            }
            bytes[index] = (byte) value;
        }
        return new String(bytes, StandardCharsets.UTF_8);
    }
}
