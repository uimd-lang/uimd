package uimd;

import java.util.List;

public class Label extends Element
{
    private String text;
    private final List<LabelSpan> spans = new java.util.ArrayList<>();
    private Integer selectionStart;
    private Integer selectionEnd;

    public Label(String name)
    {
        this(name, "");
    }

    public Label(String name, String text)
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
        spans.clear();
    }

    public void setSpans(List<LabelSpan> value)
    {
        spans.clear();
        if (value != null)
        {
            spans.addAll(value);
        }
        StringBuilder combined = new StringBuilder();
        for (LabelSpan span : spans)
        {
            combined.append(span.text());
        }
        text = combined.toString();
    }

    public void selectRange(int start, int end)
    {
        selectionStart = Math.max(0, Math.min(text.length(), start));
        selectionEnd = Math.max(0, Math.min(text.length(), end));
    }

    public void clearSelection()
    {
        selectionStart = null;
        selectionEnd = null;
    }

    public boolean hasSelection()
    {
        return selectionStart != null && selectionEnd != null && !selectionStart.equals(selectionEnd);
    }

    public Integer selectionStart()
    {
        return selectionStart;
    }

    public Integer selectionEnd()
    {
        return selectionEnd;
    }

    public String selectedText()
    {
        if (!hasSelection())
        {
            return "";
        }
        int from = Math.min(selectionStart, selectionEnd);
        int to = Math.max(selectionStart, selectionEnd);
        return text.substring(from, to);
    }

    public int textPositionFromPoint(int localRow, int localCol, Size size)
    {
        int width = Math.max(1, size.width() > 0 ? size.width() : RenderHelpers.visibleWidth(text));
        Style effectiveStyle = effectiveStyle(false, false);
        List<RenderHelpers.LabelVisualRow> rows = RenderHelpers.buildLabelVisualRows(text, width);
        if (rows.isEmpty() || localRow < 0)
        {
            return 0;
        }
        if (localRow >= rows.size())
        {
            return text.length();
        }
        RenderHelpers.LabelVisualRow row = rows.get(localRow);
        int contentWidth = row.cells().size();
        int offset = switch (effectiveStyle.textAlign())
        {
            case "center" -> Math.max(0, (width - contentWidth) / 2);
            case "right" -> Math.max(0, width - contentWidth);
            default -> 0;
        };
        return Math.max(0, Math.min(
            text.length(),
            RenderHelpers.rawIndexForLabelVisualColumn(row, localCol - offset)));
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        Style baseStyle = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        if (spans.isEmpty() && !hasSelection())
        {
            return RenderHelpers.renderPlainText(text, size.width(), size.height(), baseStyle);
        }
        Style cursor = baseStyle.copy();
        if (cursorStyle() != null)
        {
            cursor.merge(cursorStyle());
        }
        int width = Math.max(1, size.width() > 0 ? size.width() : RenderHelpers.visibleWidth(text));
        int height = Math.max(1, size.height());
        List<List<TerminalCell>> rendered = RenderHelpers.renderPlainText(text, width, height, baseStyle);
        int low = hasSelection() ? Math.min(selectionStart, selectionEnd) : 0;
        int high = hasSelection() ? Math.max(selectionStart, selectionEnd) : 0;
        List<RenderHelpers.LabelVisualRow> visualRows = RenderHelpers.buildLabelVisualRows(text, width);
        for (int rowIndex = 0; rowIndex < Math.min(height, visualRows.size()); ++rowIndex)
        {
            RenderHelpers.LabelVisualRow visualRow = visualRows.get(rowIndex);
            int contentWidth = Math.min(width, visualRow.cells().size());
            int alignmentOffset = switch (baseStyle.textAlign())
            {
                case "center" -> Math.max(0, (width - contentWidth) / 2);
                case "right" -> Math.max(0, width - contentWidth);
                default -> 0;
            };
            for (int visualCol = 0; visualCol < contentWidth; ++visualCol)
            {
                VisualGlyph glyph = visualRow.cells().get(visualCol);
                TerminalCell cell = rendered.get(rowIndex).get(alignmentOffset + visualCol);
                LabelSpan span = spanForSourceIndex(glyph.sourceStart());
                if (span != null)
                {
                    if (!span.foreground().isEmpty())
                    {
                        cell.setForeground(new Color(span.foreground()));
                    }
                    if (!span.background().isEmpty())
                    {
                        cell.setBackground(new Color(span.background()));
                    }
                }
                if (hasSelection() && glyph.sourceStart() >= low && glyph.sourceStart() < high)
                {
                    cell.setForeground(cursor.color());
                    cell.setBackground(cursor.background());
                }
            }
        }
        return rendered;
    }

    private LabelSpan spanForSourceIndex(int sourceIndex)
    {
        int offset = 0;
        for (LabelSpan span : spans)
        {
            int end = offset + span.text().length();
            if (sourceIndex >= offset && sourceIndex < end)
            {
                return span;
            }
            offset = end;
        }
        return null;
    }
}
