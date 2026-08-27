package uimd;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import java.util.function.Supplier;

public final class RenderHelpers
{
    static final class LabelVisualRow
    {
        private final int start;
        private final int end;
        private final List<VisualGlyph> cells;

        LabelVisualRow(int start, int end, List<VisualGlyph> cells)
        {
            this.start = start;
            this.end = end;
            this.cells = cells;
        }

        int start()
        {
            return start;
        }

        int end()
        {
            return end;
        }

        List<VisualGlyph> cells()
        {
            return cells;
        }
    }

    private static final String ANSI_RESET = "\u001b[0m";
    public static final int TEXT_TAB_WIDTH = 4;
    private static final ThreadLocal<Long> RENDER_TIME_OVERRIDE_MS = new ThreadLocal<>();

    private RenderHelpers()
    {
    }

    public static Long renderTimeOverrideMs()
    {
        return RENDER_TIME_OVERRIDE_MS.get();
    }

    public static void setRenderTimeOverrideMs(Long value)
    {
        if (value == null)
        {
            RENDER_TIME_OVERRIDE_MS.remove();
        }
        else
        {
            RENDER_TIME_OVERRIDE_MS.set(value);
        }
    }

    public static <T> T withRenderTimeOverride(Long value, Supplier<T> operation)
    {
        ScopedRenderTimeOverride scopedOverride = new ScopedRenderTimeOverride(value);
        try
        {
            return operation.get();
        }
        finally
        {
            scopedOverride.close();
        }
    }

    public static String safeTerminalCellText(String value)
    {
        if (value == null || value.isEmpty())
        {
            return " ";
        }
        int codePoint = value.codePointAt(0);
        if (codePoint == '\t')
        {
            return " ";
        }
        if (isUnsafeTerminalCodepoint(codePoint))
        {
            return "?";
        }
        return new String(Character.toChars(codePoint));
    }

    public static String sgrForCell(TerminalCell cell)
    {
        return ANSI_RESET + sgrForColor(cell.foreground(), true) + sgrForColor(cell.background(), false);
    }

    public static boolean sameColor(Color left, Color right)
    {
        return Objects.equals(left, right);
    }

    public static String align(String text, int width, String alignment)
    {
        int resolvedWidth = Math.max(1, width);
        List<VisualGlyph> cells = visualGlyphs(text == null ? "" : text, 0, 0);
        if (cells.size() > resolvedWidth)
        {
            cells = new ArrayList<>(cells.subList(0, resolvedWidth));
        }
        StringBuilder visible = new StringBuilder();
        for (VisualGlyph glyph : cells)
        {
            visible.append(glyph.text());
        }
        int remaining = Math.max(0, resolvedWidth - cells.size());
        if ("right".equals(alignment))
        {
            return " ".repeat(remaining) + visible;
        }
        if ("center".equals(alignment))
        {
            int left = remaining / 2;
            return " ".repeat(left) + visible + " ".repeat(remaining - left);
        }
        return visible + " ".repeat(remaining);
    }

    public static int visibleWidth(String text)
    {
        return visibleWidth(text, 0);
    }

    public static int visibleWidth(String text, int startColumn)
    {
        return visualGlyphs(text, startColumn, 0).size();
    }

    public static String visibleText(String text)
    {
        return visibleText(text, 0);
    }

    public static String visibleText(String text, int startColumn)
    {
        StringBuilder result = new StringBuilder();
        for (VisualGlyph glyph : visualGlyphs(text, startColumn, 0))
        {
            result.append(glyph.text());
        }
        return result.toString();
    }

    public static List<String> wrapText(String text, int width)
    {
        List<String> result = new ArrayList<>();
        for (LabelVisualRow row : buildLabelVisualRows(text == null ? "" : text, width))
        {
            StringBuilder line = new StringBuilder();
            for (VisualGlyph glyph : row.cells())
            {
                line.append(glyph.text());
            }
            result.add(line.toString());
        }
        return result;
    }

    public static List<List<TerminalCell>> renderPlainText(String text, int width, int height, Style style)
    {
        String value = text == null ? "" : text;
        int resolvedWidth = Math.max(1, width > 0 ? width : visibleWidth(value));
        int resolvedHeight = Math.max(1, height);
        Style effectiveStyle = style == null ? new Style() : style;
        List<List<TerminalCell>> result = new ArrayList<>(resolvedHeight);
        if (resolvedHeight > 1)
        {
            for (LabelVisualRow row : buildLabelVisualRows(value, resolvedWidth))
            {
                if (result.size() >= resolvedHeight)
                {
                    break;
                }
                result.add(renderGlyphRow(row.cells(), resolvedWidth, effectiveStyle));
            }
        }
        else
        {
            int newline = value.indexOf('\n');
            String firstLine = newline >= 0 ? value.substring(0, newline) : value;
            result.add(renderGlyphRow(visualGlyphs(firstLine, 0, 0), resolvedWidth, effectiveStyle));
        }
        while (result.size() < resolvedHeight)
        {
            result.add(renderGlyphRow(List.of(), resolvedWidth, effectiveStyle));
        }
        return result;
    }

    public static List<String> renderedText(List<List<TerminalCell>> rendered)
    {
        List<String> lines = new ArrayList<>();
        for (List<TerminalCell> row : rendered)
        {
            StringBuilder line = new StringBuilder();
            for (TerminalCell cell : row)
            {
                line.append(cell.text());
            }
            lines.add(line.toString());
        }
        return lines;
    }

    static List<LabelVisualRow> buildLabelVisualRows(String text, int width)
    {
        String value = text == null ? "" : text;
        int resolvedWidth = Math.max(1, width);
        List<LabelVisualRow> rows = new ArrayList<>();
        int segmentStart = 0;
        while (segmentStart <= value.length())
        {
            int newline = value.indexOf('\n', segmentStart);
            int segmentEnd = newline < 0 ? value.length() : newline;
            String segment = value.substring(segmentStart, segmentEnd);
            if (segment.isEmpty())
            {
                rows.add(new LabelVisualRow(segmentStart, segmentStart, new ArrayList<>()));
            }
            else
            {
                List<VisualGlyph> glyphs = visualGlyphs(segment, 0, segmentStart);
                int offset = 0;
                while (offset < glyphs.size())
                {
                    int chunkSize = Math.min(glyphs.size() - offset, resolvedWidth);
                    List<VisualGlyph> chunk = new ArrayList<>(glyphs.subList(offset, offset + chunkSize));
                    if (chunkSize == resolvedWidth)
                    {
                        int lastSpace = -1;
                        for (int index = chunk.size(); index > 1; --index)
                        {
                            VisualGlyph glyph = chunk.get(index - 1);
                            if (" ".equals(glyph.text())
                                && glyph.sourceStart() >= 0
                                && glyph.sourceStart() < value.length()
                                && value.charAt(glyph.sourceStart()) == ' ')
                            {
                                lastSpace = index - 1;
                                break;
                            }
                        }
                        if (lastSpace > 0)
                        {
                            chunk = new ArrayList<>(chunk.subList(0, lastSpace));
                            rows.add(makeLabelVisualRow(segmentStart, chunk));
                            offset += lastSpace + 1;
                            continue;
                        }
                    }
                    rows.add(makeLabelVisualRow(segmentStart, chunk));
                    offset += chunkSize;
                }
            }
            if (newline < 0)
            {
                break;
            }
            segmentStart = newline + 1;
        }
        if (rows.isEmpty())
        {
            rows.add(new LabelVisualRow(0, 0, new ArrayList<>()));
        }
        return rows;
    }

    static int rawIndexForLabelVisualColumn(LabelVisualRow row, int col)
    {
        if (row.cells().isEmpty())
        {
            return row.start();
        }
        if (col >= row.cells().size())
        {
            return row.end();
        }
        int resolvedCol = Math.max(0, Math.min(row.cells().size() - 1, col));
        VisualGlyph glyph = row.cells().get(resolvedCol);
        int runStart = resolvedCol;
        int runEnd = resolvedCol + 1;
        while (runStart > 0
            && sameSource(row.cells().get(runStart - 1), glyph))
        {
            --runStart;
        }
        while (runEnd < row.cells().size()
            && sameSource(row.cells().get(runEnd), glyph))
        {
            ++runEnd;
        }
        double midpoint = runStart + (runEnd - runStart) / 2.0;
        return resolvedCol >= midpoint ? glyph.sourceEnd() : glyph.sourceStart();
    }

    public static List<VisualGlyph> visualGlyphs(String text)
    {
        return visualGlyphs(text, 0, 0);
    }

    public static List<VisualGlyph> visualGlyphs(String text, int startColumn)
    {
        return visualGlyphs(text, startColumn, 0);
    }

    public static List<VisualGlyph> visualGlyphs(String text, int startColumn, int sourceStart)
    {
        String value = text == null ? "" : text;
        List<VisualGlyph> result = new ArrayList<>();
        int column = Math.max(0, startColumn);
        int offset = 0;
        while (offset < value.length())
        {
            int codePoint = value.codePointAt(offset);
            int length = Character.charCount(codePoint);
            int rawStart = sourceStart + offset;
            int rawEnd = rawStart + length;
            if (codePoint == '\r')
            {
                offset += length;
                continue;
            }
            if (codePoint == '\t')
            {
                int spaces = tabSpacesForColumn(column);
                for (int index = 0; index < spaces; ++index)
                {
                    result.add(new VisualGlyph(" ", rawStart, rawEnd));
                    ++column;
                }
            }
            else
            {
                String visible = isUnsafeTerminalCodepoint(codePoint)
                    ? "?"
                    : new String(Character.toChars(codePoint));
                result.add(new VisualGlyph(visible, rawStart, rawEnd));
                ++column;
            }
            offset += length;
        }
        return result;
    }

    private static boolean sameSource(VisualGlyph left, VisualGlyph right)
    {
        return left.sourceStart() == right.sourceStart() && left.sourceEnd() == right.sourceEnd();
    }

    private static LabelVisualRow makeLabelVisualRow(int fallbackStart, List<VisualGlyph> cells)
    {
        if (cells.isEmpty())
        {
            return new LabelVisualRow(fallbackStart, fallbackStart, cells);
        }
        return new LabelVisualRow(
            cells.get(0).sourceStart(),
            cells.get(cells.size() - 1).sourceEnd(),
            cells);
    }

    private static List<TerminalCell> renderGlyphRow(List<VisualGlyph> glyphs, int width, Style style)
    {
        int contentWidth = Math.min(glyphs.size(), width);
        int offset = alignedTextOffset(contentWidth, width, style.textAlign());
        List<TerminalCell> row = new ArrayList<>(width);
        while (row.size() < offset)
        {
            row.add(styledCell(" ", style, null, null));
        }
        for (VisualGlyph glyph : glyphs)
        {
            if (row.size() >= width)
            {
                break;
            }
            Color foreground = blendGradientColor(
                gradientColorAt(style.textColorGradient(), glyph.sourceStart()),
                style.color());
            Color background = blendGradientColor(
                gradientColorAt(style.textBackgroundGradient(), glyph.sourceStart()),
                style.background());
            row.add(styledCell(glyph.text(), style, foreground, background));
        }
        while (row.size() < width)
        {
            row.add(styledCell(" ", style, null, null));
        }
        return row;
    }

    static int alignedTextOffset(int contentWidth, int width, String textAlign)
    {
        return switch (textAlign)
        {
            case "center" -> Math.max(0, (width - contentWidth) / 2);
            case "right" -> Math.max(0, width - contentWidth);
            default -> 0;
        };
    }

    static TerminalCell styledCell(String text, Style style, Color foreground, Color background)
    {
        TerminalCell cell = new TerminalCell();
        cell.setText(safeTerminalCellText(text));
        cell.setForeground(foreground == null ? style.color() : foreground);
        cell.setBackground(background == null ? style.background() : background);
        return cell;
    }

    private static Color gradientColorAt(TextGradient gradient, int sourceIndex)
    {
        if (gradient == null || gradient.colors().isEmpty())
        {
            return null;
        }
        int segmentSize = Math.max(1, gradient.segmentSize());
        int interval = Math.max(1, gradient.intervalMs());
        long now = renderTimeOverrideMs() == null ? System.currentTimeMillis() : renderTimeOverrideMs();
        long frame = now / interval * gradient.step();
        int index = Math.floorMod((long) sourceIndex / segmentSize - frame, gradient.colors().size());
        return gradient.colors().get(index);
    }

    private static Color blendGradientColor(Color overlay, Color base)
    {
        if (overlay == null)
        {
            return base;
        }
        if (overlay.rgba().isEmpty())
        {
            return overlay;
        }
        int alpha = overlay.rgba().orElseThrow().alpha();
        if (alpha == 0)
        {
            return base;
        }
        if (alpha < 255 && base != null && base.rgba().isPresent())
        {
            return overlay.blendOver(base);
        }
        return overlay;
    }

    private static String sgrForColor(Color color, boolean foreground)
    {
        if (color == null || color.isTransparent())
        {
            return "";
        }
        if (color.rgba().isPresent())
        {
            Rgba rgba = color.rgba().orElseThrow();
            return String.format(
                Locale.ROOT,
                "\u001b[%d;2;%d;%d;%dm",
                foreground ? 38 : 48,
                rgba.red(),
                rgba.green(),
                rgba.blue());
        }
        return "";
    }

    private static boolean inRange(int codePoint, int first, int last)
    {
        return codePoint >= first && codePoint <= last;
    }

    public static int tabSpacesForColumn(int column)
    {
        int normalized = Math.max(0, column);
        return TEXT_TAB_WIDTH - normalized % TEXT_TAB_WIDTH;
    }

    public static boolean isUnsafeTerminalCodepoint(int codePoint)
    {
        if (codePoint < 0x20 || codePoint == 0x7f)
        {
            return true;
        }
        if (inRange(codePoint, 0x0300, 0x036f)
            || inRange(codePoint, 0x1ab0, 0x1aff)
            || inRange(codePoint, 0x1dc0, 0x1dff)
            || inRange(codePoint, 0x20d0, 0x20ff)
            || inRange(codePoint, 0xfe20, 0xfe2f))
        {
            return true;
        }
        if (inRange(codePoint, 0x200b, 0x200f)
            || inRange(codePoint, 0x202a, 0x202e)
            || inRange(codePoint, 0x2060, 0x206f)
            || inRange(codePoint, 0xfe00, 0xfe0f)
            || inRange(codePoint, 0xe0100, 0xe01ef))
        {
            return true;
        }
        return inRange(codePoint, 0x1100, 0x115f)
            || inRange(codePoint, 0x2329, 0x232a)
            || inRange(codePoint, 0x2e80, 0xa4cf)
            || inRange(codePoint, 0xac00, 0xd7a3)
            || inRange(codePoint, 0xf900, 0xfaff)
            || inRange(codePoint, 0xfe10, 0xfe19)
            || inRange(codePoint, 0xfe30, 0xfe6f)
            || inRange(codePoint, 0xff00, 0xff60)
            || inRange(codePoint, 0xffe0, 0xffe6)
            || inRange(codePoint, 0x1f000, 0x1faff)
            || inRange(codePoint, 0x20000, 0x3fffd);
    }
}
