package uimd;

import java.nio.charset.StandardCharsets;

public final class TerminalBuffer
{
    private static final int ANSI_BASE_ROW = 1;
    private static final int ANSI_BASE_COL = 1;
    private static final int MINIMUM_SCROLL_REGION_ROWS = 2;
    private static final String ANSI_RESET = "\u001b[0m";
    private static final String ANSI_SYNC_UPDATE_BEGIN = "\u001b[?2026h";
    private static final String ANSI_SYNC_UPDATE_END = "\u001b[?2026l";
    private static final String ANSI_RESET_SCROLL_REGION = "\u001b[r";

    private int width;
    private int height;
    private TerminalCell[][] cells;
    private TerminalCell[][] previous;
    private boolean forceFullRedraw = true;
    private TerminalBufferRenderStats renderStats = new TerminalBufferRenderStats(0, 0, 0, false);

    public TerminalBuffer(int width, int height)
    {
        resizeStorage(width, height);
    }

    public int width()
    {
        return width;
    }

    public int height()
    {
        return height;
    }

    public void resize(int newWidth, int newHeight)
    {
        validateDimensions(newWidth, newHeight);
        if (newWidth == width && newHeight == height)
        {
            return;
        }
        resizeStorage(newWidth, newHeight);
    }

    public void clear()
    {
        clear(new TerminalCell());
    }

    public void clear(TerminalCell cell)
    {
        TerminalCell fill = cell == null ? new TerminalCell() : cell;
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                cells[row][col] = fill.copy();
            }
        }
    }

    public TerminalCell cell(int row, int col)
    {
        if (!inBounds(row, col))
        {
            throw new IndexOutOfBoundsException("terminal cell is outside the buffer");
        }
        return cells[row][col];
    }

    public void setCell(int row, int col, TerminalCell cell)
    {
        if (inBounds(row, col))
        {
            cells[row][col] = cell == null ? new TerminalCell() : cell;
        }
    }

    public void requestFullRedraw()
    {
        forceFullRedraw = true;
    }

    public void beginRenderStats()
    {
        renderStats = new TerminalBufferRenderStats(0, 0, 0, false);
    }

    public TerminalBufferRenderStats renderStats()
    {
        return renderStats;
    }

    public String renderDiff()
    {
        return renderDiff(0, 0);
    }

    public String renderDiff(int rowOffset, int colOffset)
    {
        return renderDiffRegion(rowOffset, colOffset, 0, 0, height, width);
    }

    public String renderDiffRegion(
        int rowOffset,
        int colOffset,
        int startRow,
        int startCol,
        int regionHeight,
        int regionWidth)
    {
        StringBuilder output = new StringBuilder();
        boolean fullRedraw = forceFullRedraw;
        boolean synchronizedUpdate = false;
        boolean rawEmitted = false;
        long changedCells = 0;
        long changedRuns = 0;
        int firstRow = Math.max(0, startRow);
        int firstCol = Math.max(0, startCol);
        int lastRow = Math.min(height, startRow + Math.max(0, regionHeight));
        int lastCol = Math.min(width, startCol + Math.max(0, regionWidth));

        for (int row = firstRow; row < lastRow; ++row)
        {
            int col = firstCol;
            while (col < lastCol)
            {
                TerminalCell start = cells[row][col];
                if (start.rawSkip())
                {
                    if (fullRedraw || !start.equals(previous[row][col]))
                    {
                        ++changedCells;
                    }
                    previous[row][col] = start.copy();
                    ++col;
                    continue;
                }
                if (!fullRedraw && start.equals(previous[row][col]))
                {
                    ++col;
                    continue;
                }

                TerminalCell styleCell = start;
                if (!styleCell.raw().isEmpty())
                {
                    synchronizedUpdate = true;
                    int rawWidth = Math.max(1, styleCell.rawWidth());
                    int rawHeight = Math.max(1, styleCell.rawHeight());
                    int clearWidth = Math.min(rawWidth, width - col);
                    int clearHeight = Math.min(rawHeight, height - row);
                    for (int clearRow = row; clearRow < row + clearHeight; ++clearRow)
                    {
                        output.append(cursorPosition(clearRow + rowOffset, col + colOffset));
                        output.append(RenderHelpers.sgrForCell(styleCell));
                        output.append(" ".repeat(clearWidth));
                    }
                    if (clearHeight >= rawHeight)
                    {
                        int anchorRow = row + rowOffset;
                        String noScrollRegion = rawNoScrollRegion(anchorRow, rawHeight, rowOffset + height);
                        output.append(noScrollRegion);
                        output.append(cursorPosition(row + rowOffset, col + colOffset));
                        output.append(styleCell.raw());
                        if (!noScrollRegion.isEmpty())
                        {
                            output.append(ANSI_RESET_SCROLL_REGION);
                        }
                        rawEmitted = true;
                    }
                    for (int coveredRow = row; coveredRow < row + clearHeight; ++coveredRow)
                    {
                        for (int coveredCol = col; coveredCol < col + clearWidth; ++coveredCol)
                        {
                            previous[coveredRow][coveredCol] = cells[coveredRow][coveredCol].copy();
                            ++changedCells;
                        }
                    }
                    changedRuns += clearHeight + 1L;
                    col += clearWidth;
                    continue;
                }

                StringBuilder run = new StringBuilder();
                int runCol = col;
                while (col < lastCol)
                {
                    TerminalCell current = cells[row][col];
                    if (!fullRedraw && current.equals(previous[row][col]))
                    {
                        break;
                    }
                    if (current.rawSkip() || !current.raw().isEmpty())
                    {
                        break;
                    }
                    if (!RenderHelpers.sameColor(current.foreground(), styleCell.foreground())
                        || !RenderHelpers.sameColor(current.background(), styleCell.background()))
                    {
                        break;
                    }
                    run.append(RenderHelpers.safeTerminalCellText(current.text()));
                    previous[row][col] = current.copy();
                    ++changedCells;
                    ++col;
                }
                output.append(cursorPosition(row + rowOffset, runCol + colOffset));
                output.append(RenderHelpers.sgrForCell(styleCell));
                output.append(run);
                ++changedRuns;
            }
        }

        if (!output.isEmpty())
        {
            if (rawEmitted)
            {
                changedRuns += repaintTextOverRaw(
                    output,
                    rowOffset,
                    colOffset,
                    firstRow,
                    firstCol,
                    lastRow,
                    lastCol);
            }
            output.append(ANSI_RESET);
        }
        forceFullRedraw = false;
        String result = output.toString();
        if (!result.isEmpty() && synchronizedUpdate)
        {
            result = ANSI_SYNC_UPDATE_BEGIN + result + ANSI_SYNC_UPDATE_END;
        }
        renderStats = new TerminalBufferRenderStats(
            renderStats.changedCells() + changedCells,
            renderStats.changedRuns() + changedRuns,
            renderStats.outputBytes() + result.getBytes(StandardCharsets.UTF_8).length,
            renderStats.fullRedraw() || fullRedraw);
        return result;
    }

    public String renderScrollRegion(int rowOffset, int startRow, int regionHeight, int delta)
    {
        int firstRow = Math.max(0, startRow);
        int lastRow = Math.min(height, startRow + Math.max(0, regionHeight));
        int actualHeight = lastRow - firstRow;
        int distance = Math.min(Math.abs(delta), actualHeight);
        if (forceFullRedraw || actualHeight <= 1 || distance <= 0 || distance >= actualHeight)
        {
            return "";
        }
        if (scrollRegionHasRawCells(firstRow, lastRow))
        {
            return "";
        }

        TerminalCell[][] before = copyGrid(previous);
        if (delta > 0)
        {
            for (int row = lastRow - 1; row >= firstRow + distance; --row)
            {
                for (int col = 0; col < width; ++col)
                {
                    previous[row][col] = before[row - distance][col].copy();
                }
            }
            blankPreviousRows(firstRow, firstRow + distance);
        }
        else
        {
            for (int row = firstRow; row < lastRow - distance; ++row)
            {
                for (int col = 0; col < width; ++col)
                {
                    previous[row][col] = before[row + distance][col].copy();
                }
            }
            blankPreviousRows(lastRow - distance, lastRow);
        }
        char command = delta > 0 ? 'T' : 'S';
        String output = "\u001b[" + (firstRow + rowOffset + ANSI_BASE_ROW) + ";" + (lastRow + rowOffset) + "r"
            + "\u001b[" + (firstRow + rowOffset + ANSI_BASE_ROW) + ";1H"
            + "\u001b[" + distance + command
            + ANSI_RESET_SCROLL_REGION;
        renderStats = new TerminalBufferRenderStats(
            renderStats.changedCells(),
            renderStats.changedRuns(),
            renderStats.outputBytes() + output.getBytes(StandardCharsets.UTF_8).length,
            renderStats.fullRedraw());
        return output;
    }

    private void resizeStorage(int newWidth, int newHeight)
    {
        validateDimensions(newWidth, newHeight);
        width = newWidth;
        height = newHeight;
        cells = new TerminalCell[height][width];
        previous = new TerminalCell[height][width];
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                cells[row][col] = new TerminalCell();
                previous[row][col] = new TerminalCell();
            }
        }
        forceFullRedraw = true;
        beginRenderStats();
    }

    private boolean inBounds(int row, int col)
    {
        return row >= 0 && row < height && col >= 0 && col < width;
    }

    private void blankPreviousRows(int firstRow, int lastRow)
    {
        for (int row = firstRow; row < lastRow; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                previous[row][col] = new TerminalCell();
            }
        }
    }

    private boolean scrollRegionHasRawCells(int firstRow, int lastRow)
    {
        for (int row = firstRow; row < lastRow; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                TerminalCell current = cells[row][col];
                TerminalCell prior = previous[row][col];
                if (!current.raw().isEmpty() || current.rawSkip()
                    || !prior.raw().isEmpty() || prior.rawSkip())
                {
                    return true;
                }
            }
        }
        return false;
    }

    private long repaintTextOverRaw(
        StringBuilder output,
        int rowOffset,
        int colOffset,
        int firstRow,
        int firstCol,
        int lastRow,
        int lastCol)
    {
        long changedRuns = 0;
        for (int row = firstRow; row < lastRow; ++row)
        {
            int col = firstCol;
            while (col < lastCol)
            {
                TerminalCell cell = cells[row][col];
                if (cell.rawSkip() || !cell.raw().isEmpty())
                {
                    ++col;
                    continue;
                }
                TerminalCell styleCell = cell;
                int runCol = col;
                StringBuilder run = new StringBuilder();
                while (col < lastCol)
                {
                    TerminalCell current = cells[row][col];
                    if (current.rawSkip() || !current.raw().isEmpty()
                        || !RenderHelpers.sameColor(current.foreground(), styleCell.foreground())
                        || !RenderHelpers.sameColor(current.background(), styleCell.background()))
                    {
                        break;
                    }
                    run.append(RenderHelpers.safeTerminalCellText(current.text()));
                    ++col;
                }
                output.append(cursorPosition(row + rowOffset, runCol + colOffset));
                output.append(RenderHelpers.sgrForCell(styleCell));
                output.append(run);
                ++changedRuns;
            }
        }
        return changedRuns;
    }

    private static String cursorPosition(int row, int col)
    {
        return "\u001b[" + (row + ANSI_BASE_ROW) + ";" + (col + ANSI_BASE_COL) + "H";
    }

    private static String rawNoScrollRegion(int anchorRow, int rawHeight, int bufferBottomExclusive)
    {
        if (anchorRow >= MINIMUM_SCROLL_REGION_ROWS)
        {
            return "\u001b[1;" + anchorRow + "r";
        }
        int rawBottomExclusive = anchorRow + Math.max(1, rawHeight);
        if (bufferBottomExclusive - rawBottomExclusive >= MINIMUM_SCROLL_REGION_ROWS)
        {
            return "\u001b[" + (rawBottomExclusive + ANSI_BASE_ROW) + ";" + bufferBottomExclusive + "r";
        }
        return "";
    }

    private static TerminalCell[][] copyGrid(TerminalCell[][] source)
    {
        TerminalCell[][] result = new TerminalCell[source.length][];
        for (int row = 0; row < source.length; ++row)
        {
            result[row] = new TerminalCell[source[row].length];
            for (int col = 0; col < source[row].length; ++col)
            {
                result[row][col] = source[row][col].copy();
            }
        }
        return result;
    }

    private static void validateDimensions(int width, int height)
    {
        if (width < 0 || height < 0)
        {
            throw new IllegalArgumentException("terminal buffer dimensions must not be negative");
        }
    }
}
