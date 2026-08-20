package uimd;

import java.util.ArrayList;
import java.util.List;

public final class MessageTable extends Element
{
    private String text;

    private enum TableAlignment
    {
        LEFT,
        CENTER,
        RIGHT,
    }

    private record ParsedTable(
        List<String> headers,
        List<TableAlignment> alignments,
        List<List<String>> rows)
    {
    }

    public MessageTable(String name)
    {
        this(name, "");
    }

    public MessageTable(String name, String text)
    {
        super(name);
        this.text = text == null ? "" : text;
    }

    public String text()
    {
        return text;
    }

    public void setText(String value)
    {
        text = value == null ? "" : value;
    }

    public int fitContentHeight()
    {
        ParsedTable parsed = parseMarkdownTable(text());
        return parsed == null ? 1 : parsed.rows().size() + 4;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        Style style = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        int width = Math.max(1, size.width());
        ParsedTable parsed = parseMarkdownTable(text());
        if (parsed == null)
        {
            return RenderHelpers.renderPlainText("", width, 1, style);
        }
        List<Integer> widths = expandColumnWidths(columnWidths(parsed), width);
        Style borderStyle = style.copy();
        List<List<TerminalCell>> rendered = new ArrayList<>();
        rendered.add(separatorRow(widths, borderStyle, "┌", "┬", "┐"));
        rendered.add(dataRow(parsed.headers(), widths, parsed.alignments(), style, borderStyle));
        rendered.add(separatorRow(widths, borderStyle, "├", "┼", "┤"));
        for (List<String> row : parsed.rows())
        {
            rendered.add(dataRow(row, widths, parsed.alignments(), style, borderStyle));
        }
        rendered.add(separatorRow(widths, borderStyle, "└", "┴", "┘"));
        return rendered;
    }

    private static ParsedTable parseMarkdownTable(String text)
    {
        if (text == null || text.isBlank())
        {
            return null;
        }
        String normalized = text.strip().replace("\r\n", "\n").replace('\r', '\n');
        String[] lines = normalized.split("\n", -1);
        if (lines.length < 3)
        {
            return null;
        }
        List<String> headers = splitTableCells(lines[0]);
        List<String> separator = splitTableCells(lines[1]);
        if (headers.isEmpty()
            || headers.size() != separator.size()
            || !isMarkdownSeparatorRow(separator))
        {
            return null;
        }
        List<List<String>> rows = new ArrayList<>();
        for (int index = 2; index < lines.length; ++index)
        {
            List<String> row = splitTableCells(lines[index]);
            if (!row.isEmpty() && row.size() == headers.size())
            {
                rows.add(row);
            }
        }
        return rows.isEmpty() ? null : new ParsedTable(headers, parseAlignments(separator), rows);
    }

    private static List<String> splitTableCells(String line)
    {
        String trimmed = line == null ? "" : line.strip();
        if (trimmed.startsWith("|"))
        {
            trimmed = trimmed.substring(1);
        }
        if (trimmed.endsWith("|"))
        {
            trimmed = trimmed.substring(0, trimmed.length() - 1);
        }
        if (trimmed.isEmpty())
        {
            return List.of();
        }
        List<String> result = new ArrayList<>();
        for (String cell : trimmed.split("\\|", -1))
        {
            result.add(cell.strip());
        }
        return result;
    }

    private static boolean isMarkdownSeparatorRow(List<String> cells)
    {
        for (String cell : cells)
        {
            String compact = cell.strip();
            if (compact.isEmpty())
            {
                return false;
            }
            for (int index = 0; index < compact.length(); ++index)
            {
                char character = compact.charAt(index);
                if (character != '-' && character != ':')
                {
                    return false;
                }
            }
        }
        return !cells.isEmpty();
    }

    private static List<TableAlignment> parseAlignments(List<String> cells)
    {
        List<TableAlignment> result = new ArrayList<>();
        for (String cell : cells)
        {
            String compact = cell.strip();
            boolean left = compact.startsWith(":");
            boolean right = compact.endsWith(":");
            if (left && right)
            {
                result.add(TableAlignment.CENTER);
            }
            else if (right)
            {
                result.add(TableAlignment.RIGHT);
            }
            else
            {
                result.add(TableAlignment.LEFT);
            }
        }
        return result;
    }

    private static List<Integer> columnWidths(ParsedTable table)
    {
        List<Integer> result = new ArrayList<>();
        for (String header : table.headers())
        {
            result.add(RenderHelpers.visibleWidth(header));
        }
        for (List<String> row : table.rows())
        {
            for (int column = 0; column < row.size() && column < result.size(); ++column)
            {
                result.set(column, Math.max(result.get(column), RenderHelpers.visibleWidth(row.get(column))));
            }
        }
        return result;
    }

    private static List<Integer> expandColumnWidths(List<Integer> widths, int targetWidth)
    {
        int naturalWidth = widths.isEmpty() ? 0 : 2;
        for (int width : widths)
        {
            naturalWidth += width + 3;
        }
        if (!widths.isEmpty())
        {
            --naturalWidth;
        }
        int extra = targetWidth - naturalWidth;
        if (extra <= 0 || widths.isEmpty())
        {
            return widths;
        }
        int perColumn = extra / widths.size();
        int remainder = extra % widths.size();
        for (int index = 0; index < widths.size(); ++index)
        {
            widths.set(index, widths.get(index) + perColumn + (index < remainder ? 1 : 0));
        }
        return widths;
    }

    private static List<TerminalCell> separatorRow(
        List<Integer> widths,
        Style style,
        String left,
        String middle,
        String right)
    {
        List<TerminalCell> row = new ArrayList<>();
        appendText(row, left, style);
        for (int column = 0; column < widths.size(); ++column)
        {
            appendText(row, "─".repeat(widths.get(column) + 2), style);
            appendText(row, column + 1 < widths.size() ? middle : right, style);
        }
        return row;
    }

    private static List<TerminalCell> dataRow(
        List<String> cells,
        List<Integer> widths,
        List<TableAlignment> alignments,
        Style cellStyle,
        Style borderStyle)
    {
        List<TerminalCell> row = new ArrayList<>();
        appendText(row, "│", borderStyle);
        for (int column = 0; column < widths.size(); ++column)
        {
            String value = column < cells.size()
                ? alignText(cells.get(column), widths.get(column), alignments.get(column))
                : " ".repeat(widths.get(column));
            appendText(row, " " + value + " ", cellStyle);
            appendText(row, "│", borderStyle);
        }
        return row;
    }

    private static String alignText(String text, int width, TableAlignment alignment)
    {
        List<VisualGlyph> glyphs = RenderHelpers.visualGlyphs(text, 0, 0);
        if (glyphs.size() > width)
        {
            glyphs = new ArrayList<>(glyphs.subList(0, width));
        }
        StringBuilder visible = new StringBuilder();
        for (VisualGlyph glyph : glyphs)
        {
            visible.append(glyph.text());
        }
        int padding = Math.max(0, width - glyphs.size());
        return switch (alignment)
        {
            case RIGHT -> " ".repeat(padding) + visible;
            case CENTER -> " ".repeat(padding / 2) + visible
                + " ".repeat(padding - padding / 2);
            case LEFT -> visible + " ".repeat(padding);
        };
    }

    private static void appendText(List<TerminalCell> row, String text, Style style)
    {
        for (VisualGlyph glyph : RenderHelpers.visualGlyphs(text, 0, 0))
        {
            TerminalCell cell = new TerminalCell();
            cell.setText(glyph.text());
            cell.setForeground(style.color());
            cell.setBackground(style.background());
            row.add(cell);
        }
    }
}
