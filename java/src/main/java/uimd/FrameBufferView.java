package uimd;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

public final class FrameBufferView extends Element
{
    private static final double DIM_ALPHA = 0.4;
    private static final Color DEFAULT_FOCUSED_COLOR = new Color("#facc15");
    private static final Color DEFAULT_EDIT_COLOR = new Color("#22c55e");

    private Map<String, List<List<FrameBufferCell>>> frames = new LinkedHashMap<>();
    private List<String> targetOrder = new ArrayList<>();
    private Map<String, FrameBufferRegion> regions = new LinkedHashMap<>();
    private int panRow;
    private int panCol;
    private boolean dimmed;
    private boolean panelFocused;
    private boolean panelEdit;

    public FrameBufferView(String name)
    {
        super(name);
    }

    public void clearFrames()
    {
        frames = new LinkedHashMap<>();
        targetOrder = new ArrayList<>();
        regions = new LinkedHashMap<>();
    }

    public void setFrames(
        Map<String, List<List<FrameBufferCell>>> value,
        List<String> order)
    {
        setFrames(value, order, Map.of());
    }

    public void setFrames(
        Map<String, List<List<FrameBufferCell>>> value,
        List<String> order,
        Map<String, FrameBufferRegion> targetRegions)
    {
        frames = copyFrames(value);
        targetOrder = order == null ? new ArrayList<>(frames.keySet()) : new ArrayList<>(order);
        regions = targetRegions == null
            ? new LinkedHashMap<>()
            : new LinkedHashMap<>(targetRegions);
    }

    public void setPan(int row, int col)
    {
        panRow = Math.max(0, row);
        panCol = Math.max(0, col);
    }

    public void setDimmed(boolean value)
    {
        dimmed = value;
    }

    public void setInteractionState(boolean focused, boolean edit)
    {
        panelFocused = focused;
        panelEdit = edit;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        Style effective = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        int width = Math.max(1, size.width());
        int height = Math.max(1, size.height());
        List<List<TerminalCell>> content = RenderHelpers.renderPlainText(
            "",
            width,
            height,
            effective);
        Map<String, FrameBufferRegion> resolvedRegions = regions.isEmpty()
            ? defaultRegions(new Size(width, height))
            : regions;
        for (String target : targetOrder)
        {
            FrameBufferRegion region = resolvedRegions.get(target);
            List<List<FrameBufferCell>> rows = frames.get(target);
            if (region == null || rows == null)
            {
                continue;
            }
            renderTarget(content, rows, region, effective, width, height);
        }
        overlayCorners(content, resolvedRegions, effective);
        return content;
    }

    private void renderTarget(
        List<List<TerminalCell>> content,
        List<List<FrameBufferCell>> rows,
        FrameBufferRegion region,
        Style effective,
        int width,
        int height)
    {
        for (int localRow = 0; localRow < region.height(); ++localRow)
        {
            int outputRow = region.row() + localRow;
            int sourceRow = panRow + localRow;
            if (outputRow < 0 || outputRow >= height || sourceRow < 0 || sourceRow >= rows.size())
            {
                continue;
            }
            List<FrameBufferCell> sourceCells = rows.get(sourceRow);
            if (sourceCells == null)
            {
                continue;
            }
            for (int localCol = 0; localCol < region.width(); ++localCol)
            {
                int outputCol = region.col() + localCol;
                int sourceCol = panCol + localCol;
                if (outputCol < 0 || outputCol >= width
                    || sourceCol < 0 || sourceCol >= sourceCells.size())
                {
                    continue;
                }
                FrameBufferCell source = sourceCells.get(sourceCol);
                if (source == null)
                {
                    continue;
                }
                String foreground = dimmed ? dimHexColor(source.foreground()) : source.foreground();
                String background = dimmed ? dimHexColor(source.background()) : source.background();
                TerminalCell cell = new TerminalCell();
                cell.setText(RenderHelpers.safeTerminalCellText(source.text()));
                cell.setForeground(colorFromString(foreground, effective.color()));
                cell.setBackground(colorFromString(background, effective.background()));
                content.get(outputRow).set(outputCol, cell);
            }
        }
    }

    private Map<String, FrameBufferRegion> defaultRegions(Size size)
    {
        Map<String, FrameBufferRegion> result = new LinkedHashMap<>();
        List<Integer> widths = splitDimension(size.width(), Math.max(1, targetOrder.size()));
        int col = 0;
        for (int index = 0; index < targetOrder.size(); ++index)
        {
            int targetWidth = widths.get(index);
            result.put(
                targetOrder.get(index),
                new FrameBufferRegion(0, col, targetWidth, Math.max(1, size.height())));
            col += targetWidth;
        }
        return result;
    }

    private void overlayCorners(
        List<List<TerminalCell>> content,
        Map<String, FrameBufferRegion> resolvedRegions,
        Style effective)
    {
        if (!panelFocused && !panelEdit)
        {
            return;
        }
        Style stateStyle = panelEdit && editStyle() != null
            ? editStyle()
            : (panelFocused && focusStyle() != null ? focusStyle() : effective);
        Color color = stateStyle.color();
        if (color == null)
        {
            color = panelEdit ? DEFAULT_EDIT_COLOR : DEFAULT_FOCUSED_COLOR;
        }
        for (String target : targetOrder)
        {
            FrameBufferRegion region = resolvedRegions.get(target);
            if (region == null || region.width() < 2 || region.height() < 2)
            {
                continue;
            }
            putCorner(content, region.row(), region.col(), "┌", color);
            putCorner(content, region.row(), region.col() + 1, "─", color);
            putCorner(content, region.row(), region.col() + region.width() - 2, "─", color);
            putCorner(content, region.row(), region.col() + region.width() - 1, "┐", color);
            putCorner(content, region.row() + region.height() - 1, region.col(), "└", color);
            putCorner(content, region.row() + region.height() - 1, region.col() + 1, "─", color);
            putCorner(
                content,
                region.row() + region.height() - 1,
                region.col() + region.width() - 2,
                "─",
                color);
            putCorner(
                content,
                region.row() + region.height() - 1,
                region.col() + region.width() - 1,
                "┘",
                color);
        }
    }

    private static void putCorner(
        List<List<TerminalCell>> content,
        int row,
        int col,
        String text,
        Color color)
    {
        if (row < 0 || row >= content.size() || col < 0 || col >= content.get(row).size())
        {
            return;
        }
        TerminalCell cell = content.get(row).get(col);
        cell.setText(text);
        cell.setForeground(color);
    }

    private static List<Integer> splitDimension(int size, int parts)
    {
        int resolvedSize = Math.max(1, size);
        int resolvedParts = Math.max(1, Math.min(parts, resolvedSize));
        int base = resolvedSize / resolvedParts;
        int extra = resolvedSize % resolvedParts;
        List<Integer> result = new ArrayList<>(resolvedParts);
        for (int index = 0; index < resolvedParts; ++index)
        {
            result.add(base + (index < extra ? 1 : 0));
        }
        return result;
    }

    private static String dimHexColor(String value)
    {
        if (value == null || value.length() != 7 || value.charAt(0) != '#')
        {
            return value;
        }
        try
        {
            int red = dimChannel(value.substring(1, 3));
            int green = dimChannel(value.substring(3, 5));
            int blue = dimChannel(value.substring(5, 7));
            return String.format(Locale.ROOT, "#%02x%02x%02x", red, green, blue);
        }
        catch (NumberFormatException ignored)
        {
            return value;
        }
    }

    private static int dimChannel(String value)
    {
        return (int) Math.round((1.0 - DIM_ALPHA) * Integer.parseInt(value, 16));
    }

    private static Color colorFromString(String value, Color fallback)
    {
        if (value == null || value.isEmpty())
        {
            return fallback;
        }
        String resolved = value;
        if (value.length() == 4 && value.charAt(0) == '#')
        {
            resolved = "#"
                + value.charAt(1) + value.charAt(1)
                + value.charAt(2) + value.charAt(2)
                + value.charAt(3) + value.charAt(3);
        }
        try
        {
            return new Color(resolved);
        }
        catch (IllegalArgumentException ignored)
        {
            return fallback;
        }
    }

    private static Map<String, List<List<FrameBufferCell>>> copyFrames(
        Map<String, List<List<FrameBufferCell>>> value)
    {
        Map<String, List<List<FrameBufferCell>>> result = new LinkedHashMap<>();
        if (value == null)
        {
            return result;
        }
        for (Map.Entry<String, List<List<FrameBufferCell>>> entry : value.entrySet())
        {
            List<List<FrameBufferCell>> rows = new ArrayList<>();
            if (entry.getValue() != null)
            {
                for (List<FrameBufferCell> row : entry.getValue())
                {
                    rows.add(row == null ? new ArrayList<>() : new ArrayList<>(row));
                }
            }
            result.put(entry.getKey(), rows);
        }
        return result;
    }
}
