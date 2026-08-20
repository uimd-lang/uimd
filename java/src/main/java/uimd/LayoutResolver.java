package uimd;

import java.util.ArrayList;
import java.util.List;

public final class LayoutResolver
{
    public ResolvedCell resolve(LayoutCell root, Rect bounds)
    {
        if (root == null)
        {
            throw new IllegalArgumentException("root layout cell must not be null");
        }
        Rect resolvedBounds = bounds == null ? new Rect(0, 0, 0, 0) : bounds;
        Size measured = measureCell(root);
        int width = resolvedBounds.width() > 0
            ? resolvedBounds.width()
            : resolveAxis(root.width(), root.sourceSize().width(), measured.width());
        int height = resolvedBounds.height() > 0
            ? resolvedBounds.height()
            : resolveAxis(root.height(), root.sourceSize().height(), measured.height());
        return resolveCell(
            root,
            new Rect(resolvedBounds.row(), resolvedBounds.col(), width, height));
    }

    public static ResolvedCell findCellByName(ResolvedCell root, String name)
    {
        if (root == null)
        {
            return null;
        }
        if (root.name().equals(name))
        {
            return root;
        }
        for (ResolvedCell child : root.children())
        {
            ResolvedCell found = findCellByName(child, name);
            if (found != null)
            {
                return found;
            }
        }
        return null;
    }

    public static ResolvedElement findElementByName(ResolvedCell root, String name)
    {
        if (root == null)
        {
            return null;
        }
        for (ResolvedElement element : root.elements())
        {
            if (element.name().equals(name))
            {
                return element;
            }
        }
        for (ResolvedCell child : root.children())
        {
            ResolvedElement found = findElementByName(child, name);
            if (found != null)
            {
                return found;
            }
        }
        return null;
    }

    public static List<FlatResolvedCell> flattenCells(ResolvedCell root)
    {
        List<FlatResolvedCell> result = new ArrayList<>();
        if (root == null)
        {
            return result;
        }
        result.add(new FlatResolvedCell(root.name(), root.rect()));
        for (ResolvedCell child : root.children())
        {
            result.addAll(flattenCells(child));
        }
        return result;
    }

    public static List<FlatResolvedElement> flattenElements(ResolvedCell root)
    {
        List<FlatResolvedElement> result = new ArrayList<>();
        if (root == null)
        {
            return result;
        }
        for (ResolvedElement element : root.elements())
        {
            result.add(new FlatResolvedElement(root.name(), element.name(), element.rect()));
        }
        for (ResolvedCell child : root.children())
        {
            result.addAll(flattenElements(child));
        }
        return result;
    }

    private ResolvedCell resolveCell(LayoutCell cell, Rect bounds)
    {
        ResolvedCell resolved = new ResolvedCell(cell.name(), bounds);
        Rect content = inset(bounds, cell.padding());
        for (LayoutElement element : cell.elements())
        {
            resolved.addElement(resolveElement(element, content));
        }
        if (cell.children().isEmpty())
        {
            return resolved;
        }
        boolean horizontal = cell.direction() == LayoutDirection.ROW;
        int availableMain = horizontal ? content.width() : content.height();
        int availableCross = horizontal ? content.height() : content.width();
        List<Integer> mainSizes = resolveChildAxisSizes(cell, availableMain);
        int cursor = horizontal ? content.col() : content.row();
        for (int index = 0; index < cell.children().size(); ++index)
        {
            LayoutCell child = cell.children().get(index);
            int mainSize = mainSizes.get(index);
            int crossSize = childCrossSize(child, horizontal, availableCross);
            Rect childRect = horizontal
                ? new Rect(content.row(), cursor, mainSize, crossSize)
                : new Rect(cursor, content.col(), crossSize, mainSize);
            resolved.addChild(resolveCell(child, childRect));
            cursor += mainSize + cell.gap();
        }
        return resolved;
    }

    private Size measureCell(LayoutCell cell)
    {
        Size children = measureChildren(cell);
        int contentWidth = Math.max(cell.fitContentSize().width(), children.width());
        int contentHeight = Math.max(cell.fitContentSize().height(), children.height());
        for (LayoutElement element : cell.elements())
        {
            int elementWidth = resolveAxis(
                element.width(),
                element.relative().width(),
                element.fitContentSize().width());
            int elementHeight = resolveAxis(
                element.height(),
                element.relative().height(),
                element.fitContentSize().height());
            contentWidth = Math.max(contentWidth, element.relative().col() + elementWidth);
            contentHeight = Math.max(contentHeight, element.relative().row() + elementHeight);
        }
        int padding = nonNegative(cell.padding());
        return new Size(contentWidth + padding * 2, contentHeight + padding * 2);
    }

    private Size measureChildren(LayoutCell cell)
    {
        if (cell.children().isEmpty())
        {
            return new Size(0, 0);
        }
        boolean horizontal = cell.direction() == LayoutDirection.ROW;
        int main = 0;
        int cross = 0;
        for (LayoutCell child : cell.children())
        {
            Size childSize = measureCell(child);
            main += horizontal ? childSize.width() : childSize.height();
            cross = Math.max(cross, horizontal ? childSize.height() : childSize.width());
        }
        main += nonNegative(cell.gap()) * (cell.children().size() - 1);
        return horizontal ? new Size(main, cross) : new Size(cross, main);
    }

    private int resolveAxis(AxisDimension dimension, int fallback, int fitContent)
    {
        return switch (dimension.mode())
        {
            case FIXED -> nonNegative(dimension.value());
            case FIT_CONTENT -> nonNegative(fitContent);
            case EXPANDED -> nonNegative(fallback);
            case AUTO -> nonNegative(fallback > 0 ? fallback : fitContent);
        };
    }

    private List<Integer> resolveChildAxisSizes(LayoutCell cell, int available)
    {
        boolean horizontal = cell.direction() == LayoutDirection.ROW;
        List<Integer> sizes = new ArrayList<>();
        for (int index = 0; index < cell.children().size(); ++index)
        {
            sizes.add(0);
        }
        int expandedCount = 0;
        int fixedTotal = 0;
        for (int index = 0; index < cell.children().size(); ++index)
        {
            LayoutCell child = cell.children().get(index);
            AxisDimension dimension = horizontal ? child.width() : child.height();
            if (dimension.mode() == DimensionMode.EXPANDED)
            {
                ++expandedCount;
                continue;
            }
            int size = childAxisFixedSize(child, horizontal);
            sizes.set(index, size);
            fixedTotal += size;
        }
        int gaps = nonNegative(cell.gap()) * Math.max(0, cell.children().size() - 1);
        int remaining = nonNegative(available - fixedTotal - gaps);
        int expandedShare = expandedCount > 0 ? remaining / expandedCount : 0;
        for (int index = 0; index < cell.children().size(); ++index)
        {
            LayoutCell child = cell.children().get(index);
            AxisDimension dimension = horizontal ? child.width() : child.height();
            if (dimension.mode() != DimensionMode.EXPANDED)
            {
                continue;
            }
            sizes.set(index, expandedShare);
            remaining -= expandedShare;
            --expandedCount;
            if (expandedCount == 0)
            {
                sizes.set(index, sizes.get(index) + remaining);
            }
        }
        return sizes;
    }

    private int childAxisFixedSize(LayoutCell child, boolean horizontal)
    {
        Rect measured = resolve(child, new Rect(0, 0, 0, 0)).rect();
        AxisDimension dimension = horizontal ? child.width() : child.height();
        int source = horizontal ? child.sourceSize().width() : child.sourceSize().height();
        int fit = horizontal ? measured.width() : measured.height();
        return switch (dimension.mode())
        {
            case FIXED -> nonNegative(dimension.value());
            case FIT_CONTENT -> fit;
            case AUTO -> source > 0 ? source : fit;
            case EXPANDED -> 0;
        };
    }

    private int childCrossSize(LayoutCell child, boolean horizontal, int available)
    {
        Rect measured = resolve(child, new Rect(0, 0, 0, 0)).rect();
        AxisDimension dimension = horizontal ? child.height() : child.width();
        int source = horizontal ? child.sourceSize().height() : child.sourceSize().width();
        int fit = horizontal ? measured.height() : measured.width();
        return switch (dimension.mode())
        {
            case FIXED -> nonNegative(dimension.value());
            case EXPANDED -> nonNegative(available);
            case FIT_CONTENT -> fit;
            case AUTO -> source > 0 ? source : fit;
        };
    }

    private ResolvedElement resolveElement(LayoutElement element, Rect content)
    {
        int width = resolveAxis(
            element.width(),
            element.relative().width(),
            element.fitContentSize().width());
        int height = resolveAxis(
            element.height(),
            element.relative().height(),
            element.fitContentSize().height());
        return new ResolvedElement(
            element.name(),
            new Rect(
                content.row() + element.relative().row(),
                content.col() + element.relative().col(),
                width,
                height));
    }

    private static Rect inset(Rect rect, int padding)
    {
        int safePadding = nonNegative(padding);
        return new Rect(
            rect.row() + safePadding,
            rect.col() + safePadding,
            nonNegative(rect.width() - safePadding * 2),
            nonNegative(rect.height() - safePadding * 2));
    }

    private static int nonNegative(int value)
    {
        return Math.max(0, value);
    }
}
