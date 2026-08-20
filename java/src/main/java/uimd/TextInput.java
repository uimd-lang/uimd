package uimd;

import java.util.ArrayList;
import java.util.List;

public class TextInput extends Element
{
    private static final int OPTION_HORIZONTAL_STEPS = 5;
    private static final int OPTION_VERTICAL_STEPS = 3;

    private String value;
    private int cursor;
    private Integer selectionAnchor;
    private final int maxLength;
    private final boolean multiline;
    private int colScrollOffset;
    private int rowScrollOffset;
    private boolean manualRowScroll;

    public TextInput(String name)
    {
        this(name, "", 0, false);
    }

    public TextInput(String name, String value, int maxLength)
    {
        this(name, value, maxLength, false);
    }

    protected TextInput(String name, String value, int maxLength, boolean multiline)
    {
        super(name);
        this.multiline = multiline;
        this.maxLength = Math.max(0, maxLength);
        this.value = normalizeValue(value, multiline);
        if (this.maxLength > 0 && this.value.length() > this.maxLength)
        {
            this.value = this.value.substring(0, this.maxLength);
        }
        cursor = this.value.length();
    }

    public String value()
    {
        return value;
    }

    public void setValue(String nextValue)
    {
        value = normalizeValue(nextValue, multiline);
        if (maxLength > 0 && value.length() > maxLength)
        {
            value = value.substring(0, maxLength);
        }
        setCursor(cursor);
    }

    public int cursor()
    {
        return cursor;
    }

    public void setCursor(int nextCursor)
    {
        manualRowScroll = false;
        cursor = clamp(nextCursor, 0, value.length());
        clearSelection();
    }

    public int selectionStart()
    {
        return hasSelection() ? Math.min(selectionAnchor, cursor) : cursor;
    }

    public int selectionEnd()
    {
        return hasSelection() ? Math.max(selectionAnchor, cursor) : cursor;
    }

    public int maxLength()
    {
        return maxLength;
    }

    public boolean multiline()
    {
        return multiline;
    }

    public void setSelection(int start, int end)
    {
        selectRange(start, end);
    }

    public void selectRange(int start, int end)
    {
        manualRowScroll = false;
        int resolvedStart = clamp(start, 0, value.length());
        int resolvedEnd = clamp(end, 0, value.length());
        if (resolvedStart == resolvedEnd)
        {
            selectionAnchor = null;
            cursor = resolvedEnd;
            return;
        }
        selectionAnchor = resolvedStart;
        cursor = resolvedEnd;
    }

    public void clearSelection()
    {
        selectionAnchor = null;
    }

    public String selectedText()
    {
        if (!hasSelection())
        {
            return "";
        }
        int start = selectionStart();
        int end = selectionEnd();
        return value.substring(start, end);
    }

    public void insertText(String text)
    {
        String nextText = text == null ? "" : text.replace("\r\n", "\n").replace('\r', '\n');
        if (!multiline)
        {
            nextText = nextText.replace('\n', ' ');
        }
        String prefix = value.substring(0, cursor);
        String suffix = value.substring(cursor);
        if (hasSelection())
        {
            int start = selectionStart();
            int end = selectionEnd();
            prefix = value.substring(0, start);
            suffix = value.substring(end);
            cursor = start;
        }
        String next = prefix + nextText + suffix;
        if (maxLength > 0 && next.length() > maxLength)
        {
            next = next.substring(0, maxLength);
        }
        cursor = Math.min(next.length(), cursor + nextText.length());
        value = next;
        clearSelection();
    }

    public int cursorForPoint(int row, int col, Size size)
    {
        int width = safeWidth(size);
        if (!multiline)
        {
            RenderHelpers.LabelVisualRow visualRow = makeVisualTextRow(
                0,
                RenderHelpers.visualGlyphs(value, 0, 0));
            return clamp(
                RenderHelpers.rawIndexForLabelVisualColumn(visualRow, colScrollOffset + col),
                0,
                value.length());
        }
        int height = safeHeight(size);
        List<RenderHelpers.LabelVisualRow> rows = RenderHelpers.buildLabelVisualRows(value, width);
        int targetRow = clamp(row, 0, height - 1) + rowScrollOffset;
        if (targetRow >= rows.size())
        {
            return value.length();
        }
        RenderHelpers.LabelVisualRow visualRow = rows.get(targetRow);
        return clamp(
            RenderHelpers.rawIndexForLabelVisualColumn(visualRow, col),
            visualRow.start(),
            visualRow.end());
    }

    public boolean scrollByRows(int delta, int viewportHeight)
    {
        return scrollByRows(delta, viewportHeight, true);
    }

    public boolean scrollByRows(int delta, int viewportHeight, boolean manual)
    {
        if (!multiline)
        {
            return false;
        }
        int height = Math.max(1, viewportHeight);
        if (height <= 1)
        {
            return false;
        }
        int width = Math.max(1, frame().width());
        List<RenderHelpers.LabelVisualRow> rows = RenderHelpers.buildLabelVisualRows(value, width);
        int maxOffset = Math.max(0, rows.size() - height);
        int nextOffset = clamp(rowScrollOffset + delta, 0, maxOffset);
        if (nextOffset == rowScrollOffset)
        {
            return false;
        }
        rowScrollOffset = nextOffset;
        manualRowScroll = manual;
        return true;
    }

    @Override
    public boolean handleKey(String key)
    {
        manualRowScroll = false;
        boolean selection = hasSelection();
        switch (key)
        {
            case "Shift+Left":
                beginSelection();
                setCursorKeepingSelection(previousCodePointIndex(cursor));
                return true;
            case "Shift+Right":
                beginSelection();
                setCursorKeepingSelection(nextCodePointIndex(cursor));
                return true;
            case "Shift+Home":
                beginSelection();
                setCursorKeepingSelection(lineStartForCursor());
                return true;
            case "Shift+End":
                beginSelection();
                setCursorKeepingSelection(lineEndForCursor());
                return true;
            case "Shift+Up":
                if (!multiline)
                {
                    return false;
                }
                beginSelection();
                moveCursorVertical(-1, false);
                return true;
            case "Shift+Down":
                if (!multiline)
                {
                    return false;
                }
                beginSelection();
                moveCursorVertical(1, false);
                return true;
            case "Left":
                if (selection)
                {
                    cursor = selectionStart();
                    clearSelection();
                }
                else
                {
                    setCursor(previousCodePointIndex(cursor));
                }
                return true;
            case "Right":
                if (selection)
                {
                    cursor = selectionEnd();
                    clearSelection();
                }
                else
                {
                    setCursor(nextCodePointIndex(cursor));
                }
                return true;
            case "Alt+Left":
                clearSelection();
                for (int step = 0; step < OPTION_HORIZONTAL_STEPS; ++step)
                {
                    setCursor(previousCodePointIndex(cursor));
                }
                return true;
            case "Alt+Right":
                clearSelection();
                for (int step = 0; step < OPTION_HORIZONTAL_STEPS; ++step)
                {
                    setCursor(nextCodePointIndex(cursor));
                }
                return true;
            case "Up":
                if (!multiline)
                {
                    return false;
                }
                moveCursorVertical(-1, true);
                return true;
            case "Down":
                if (!multiline)
                {
                    return false;
                }
                moveCursorVertical(1, true);
                return true;
            case "Alt+Up":
                if (!multiline)
                {
                    return false;
                }
                clearSelection();
                for (int step = 0; step < OPTION_VERTICAL_STEPS; ++step)
                {
                    moveCursorVertical(-1, true);
                }
                return true;
            case "Alt+Down":
                if (!multiline)
                {
                    return false;
                }
                clearSelection();
                for (int step = 0; step < OPTION_VERTICAL_STEPS; ++step)
                {
                    moveCursorVertical(1, true);
                }
                return true;
            case "Home":
                setCursor(lineStartForCursor());
                return true;
            case "End":
                setCursor(lineEndForCursor());
                return true;
            case "Backspace":
                if (selection)
                {
                    deleteSelection();
                }
                else if (cursor > 0)
                {
                    int previous = previousCodePointIndex(cursor);
                    value = value.substring(0, previous) + value.substring(cursor);
                    cursor = previous;
                }
                return true;
            case "Delete":
                if (selection)
                {
                    deleteSelection();
                }
                else if (cursor < value.length())
                {
                    int next = nextCodePointIndex(cursor);
                    value = value.substring(0, cursor) + value.substring(next);
                }
                return true;
            case "Enter":
                if (multiline)
                {
                    insertText("\n");
                    return true;
                }
                return false;
            default:
                if (key != null && key.codePointCount(0, key.length()) == 1)
                {
                    insertText(key);
                    return true;
                }
                return false;
        }
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        int width = safeWidth(size) + (size.width() > 0 ? 0 : 1);
        int height = Math.max(1, multiline ? size.height() : 1);
        Style style = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        Style cursorStyleValue = style.copy();
        if (cursorStyle() != null)
        {
            cursorStyleValue.merge(cursorStyle());
        }

        if (!multiline)
        {
            return renderSingleLine(width, effectiveState, style, cursorStyleValue);
        }
        return renderMultiline(width, height, effectiveState, style, cursorStyleValue);
    }

    private List<List<TerminalCell>> renderSingleLine(
        int width,
        ElementRenderState state,
        Style style,
        Style cursorStyleValue)
    {
        RenderHelpers.LabelVisualRow visualRow = makeVisualTextRow(
            0,
            RenderHelpers.visualGlyphs(value, 0, 0));
        int textWidth = visualRow.cells().size();
        colScrollOffset = Math.max(0, colScrollOffset);
        if (textWidth <= width)
        {
            colScrollOffset = 0;
        }
        int cursorVisualCol = visualColumnForCursor(visualRow, cursor, Math.max(width, textWidth + 1));
        if (state.editMode())
        {
            if (cursorVisualCol < colScrollOffset)
            {
                colScrollOffset = cursorVisualCol;
            }
            else if (cursorVisualCol >= colScrollOffset + width)
            {
                colScrollOffset = Math.max(0, cursorVisualCol - width + 1);
            }
        }

        List<VisualGlyph> visible = new ArrayList<>();
        if (colScrollOffset < textWidth)
        {
            int end = Math.min(textWidth, colScrollOffset + width);
            visible.addAll(visualRow.cells().subList(colScrollOffset, end));
        }
        List<TerminalCell> renderedRow = glyphRow(visible, width, style);
        if (!state.editMode() && textWidth > colScrollOffset + width)
        {
            renderedRow.get(width - 1).setText(">");
        }
        List<List<TerminalCell>> result = new ArrayList<>();
        result.add(renderedRow);
        if (state.editMode() && hasSelection())
        {
            int low = selectionLow();
            int high = selectionHigh();
            for (int col = 0; col < Math.min(width, visible.size()); ++col)
            {
                int source = visible.get(col).sourceStart();
                if (source >= low && source < high)
                {
                    applyStyle(result.get(0).get(col), cursorStyleValue);
                }
            }
        }
        else if (state.editMode())
        {
            int visibleCol = clamp(cursorVisualCol - colScrollOffset, 0, width - 1);
            applyStyle(result.get(0).get(visibleCol), cursorStyleValue);
        }
        return result;
    }

    private List<List<TerminalCell>> renderMultiline(
        int width,
        int height,
        ElementRenderState state,
        Style style,
        Style cursorStyleValue)
    {
        List<RenderHelpers.LabelVisualRow> rows = RenderHelpers.buildLabelVisualRows(value, width);
        int cursorRow = visualRowForCursor(rows, width, cursor);
        rowScrollOffset = clamp(rowScrollOffset, 0, Math.max(0, rows.size() - height));
        if (state.editMode() && !manualRowScroll)
        {
            if (cursorRow < rowScrollOffset)
            {
                rowScrollOffset = cursorRow;
            }
            else if (cursorRow >= rowScrollOffset + height)
            {
                rowScrollOffset = cursorRow - height + 1;
            }
        }
        boolean hasAbove = rowScrollOffset > 0;
        boolean hasBelow = rowScrollOffset + height < rows.size();
        List<List<TerminalCell>> result = new ArrayList<>();
        for (int row = 0; row < height; ++row)
        {
            int rowIndex = rowScrollOffset + row;
            boolean hasVisualRow = rowIndex < rows.size();
            RenderHelpers.LabelVisualRow visualRow = hasVisualRow
                ? rows.get(rowIndex)
                : new RenderHelpers.LabelVisualRow(0, 0, new ArrayList<>());
            List<TerminalCell> renderedRow = glyphRow(visualRow.cells(), width, style);
            result.add(renderedRow);

            int cursorCol = state.editMode() && hasVisualRow
                ? visualColumnForCursor(visualRow, cursor, width)
                : -1;
            boolean indicatorVisible = visualRow.cells().size() < width
                && ((row == 0 && hasAbove) || (row == height - 1 && hasBelow));
            if (indicatorVisible && cursorCol != width - 1)
            {
                renderedRow.get(width - 1).setText(row == 0 && hasAbove ? "^" : "v");
            }
            if (state.editMode() && hasSelection())
            {
                int low = selectionLow();
                int high = selectionHigh();
                for (int col = 0; col < Math.min(width, visualRow.cells().size()); ++col)
                {
                    int source = visualRow.cells().get(col).sourceStart();
                    if (source >= low && source < high)
                    {
                        applyStyle(renderedRow.get(col), cursorStyleValue);
                    }
                }
            }
            else if (state.editMode() && cursorCol >= 0 && cursorCol < width)
            {
                applyStyle(renderedRow.get(cursorCol), cursorStyleValue);
            }
        }
        return result;
    }

    private void beginSelection()
    {
        if (selectionAnchor == null)
        {
            selectionAnchor = cursor;
        }
    }

    private void setCursorKeepingSelection(int nextCursor)
    {
        cursor = clamp(nextCursor, 0, value.length());
    }

    private boolean hasSelection()
    {
        return selectionAnchor != null && selectionAnchor != cursor;
    }

    private void deleteSelection()
    {
        if (!hasSelection())
        {
            return;
        }
        int start = selectionStart();
        int end = selectionEnd();
        value = value.substring(0, start) + value.substring(end);
        cursor = start;
        clearSelection();
    }

    private int lineStartForCursor()
    {
        if (cursor <= 0)
        {
            return 0;
        }
        int index = value.lastIndexOf('\n', Math.max(0, cursor - 1));
        return index < 0 ? 0 : index + 1;
    }

    private int lineEndForCursor()
    {
        int index = value.indexOf('\n', cursor);
        return index < 0 ? value.length() : index;
    }

    private void moveCursorVertical(int delta, boolean clearCurrentSelection)
    {
        int width = Math.max(1, frame().width());
        List<RenderHelpers.LabelVisualRow> rows = RenderHelpers.buildLabelVisualRows(value, width);
        int currentRow = visualRowForCursor(rows, width, cursor);
        int targetRow = currentRow + delta;
        if (targetRow < 0 || targetRow >= rows.size())
        {
            return;
        }
        RenderHelpers.LabelVisualRow current = rows.get(currentRow);
        RenderHelpers.LabelVisualRow target = rows.get(targetRow);
        int currentVisualCol = clamp(visualColumnForCursor(current, cursor, width), 0, current.cells().size());
        int nextCursor = RenderHelpers.rawIndexForLabelVisualColumn(target, currentVisualCol);
        if (clearCurrentSelection)
        {
            setCursor(nextCursor);
        }
        else
        {
            setCursorKeepingSelection(nextCursor);
        }
    }

    private int previousCodePointIndex(int index)
    {
        return index <= 0 ? 0 : value.offsetByCodePoints(index, -1);
    }

    private int nextCodePointIndex(int index)
    {
        return index >= value.length() ? value.length() : value.offsetByCodePoints(index, 1);
    }

    private int selectionLow()
    {
        return selectionStart();
    }

    private int selectionHigh()
    {
        return selectionEnd();
    }

    private int safeWidth(Size size)
    {
        return Math.max(1, size.width() > 0 ? size.width() : RenderHelpers.visibleWidth(value));
    }

    private static int safeHeight(Size size)
    {
        return Math.max(1, size.height() > 0 ? size.height() : 1);
    }

    private static RenderHelpers.LabelVisualRow makeVisualTextRow(
        int fallbackStart,
        List<VisualGlyph> cells)
    {
        if (cells.isEmpty())
        {
            return new RenderHelpers.LabelVisualRow(fallbackStart, fallbackStart, cells);
        }
        return new RenderHelpers.LabelVisualRow(
            cells.get(0).sourceStart(),
            cells.get(cells.size() - 1).sourceEnd(),
            cells);
    }

    private static int visualRowForCursor(
        List<RenderHelpers.LabelVisualRow> rows,
        int width,
        int cursor)
    {
        for (int index = 0; index < rows.size(); ++index)
        {
            RenderHelpers.LabelVisualRow row = rows.get(index);
            if (cursor <= row.end() && (row.cells().size() < width || cursor < row.end()))
            {
                return index;
            }
        }
        return Math.max(0, rows.size() - 1);
    }

    private static int visualColumnForCursor(RenderHelpers.LabelVisualRow row, int cursor, int width)
    {
        if (cursor < row.start() || cursor > row.end())
        {
            return -1;
        }
        for (int index = 0; index < row.cells().size(); ++index)
        {
            VisualGlyph glyph = row.cells().get(index);
            if (glyph.sourceStart() >= 0 && cursor <= glyph.sourceStart())
            {
                return index;
            }
            if (glyph.sourceStart() >= 0 && cursor > glyph.sourceStart() && cursor < glyph.sourceEnd())
            {
                return index;
            }
        }
        return Math.min(row.cells().size(), Math.max(0, width - 1));
    }

    private static List<TerminalCell> glyphRow(
        List<VisualGlyph> glyphs,
        int width,
        Style style)
    {
        List<TerminalCell> row = new ArrayList<>(width);
        for (int index = 0; index < Math.min(width, glyphs.size()); ++index)
        {
            row.add(RenderHelpers.styledCell(glyphs.get(index).text(), style, null, null));
        }
        while (row.size() < width)
        {
            row.add(RenderHelpers.styledCell(" ", style, null, null));
        }
        return row;
    }

    private static void applyStyle(TerminalCell cell, Style style)
    {
        cell.setForeground(style.color());
        cell.setBackground(style.background());
    }

    private static String normalizeValue(String value, boolean multiline)
    {
        String normalized = value == null ? "" : value.replace("\r\n", "\n").replace('\r', '\n');
        return multiline ? normalized : normalized.replace('\n', ' ');
    }

    private static int clamp(int value, int minimum, int maximum)
    {
        return Math.max(minimum, Math.min(maximum, value));
    }
}
