package uimd;

import java.util.ArrayList;
import java.util.List;
import java.util.function.IntFunction;

public class ScrollView extends Element
{
    public static final int DEFAULT_GAP = 0;
    public static final int DEFAULT_WHEEL_SCROLL_LINES = 4;
    public static final int WHEEL_SCROLL_VIEWPORT_FRACTION = 3;

    private int gap;
    private boolean autoScroll;
    private int scrollOffset;
    private int viewOffset;
    private int horizontalViewOffset;
    private int lastNaturalSkip;
    private int pendingTerminalScrollDelta;
    private Style descendantFocusStyle;
    private final List<List<List<TerminalCell>>> renderedChildren = new ArrayList<>();
    private IntFunction<List<List<List<TerminalCell>>>> dynamicChildrenRenderer;
    private Integer dynamicChildrenWidth;

    public ScrollView(String name)
    {
        this(name, DEFAULT_GAP);
    }

    public ScrollView(String name, int gap)
    {
        super(name);
        this.gap = Math.max(0, gap);
    }

    public int gap()
    {
        return gap;
    }

    public void setGap(int value)
    {
        gap = Math.max(0, value);
        invalidateHeightCache();
    }

    public boolean autoScroll()
    {
        return autoScroll;
    }

    public void setAutoScroll(boolean value)
    {
        autoScroll = value;
        if (autoScroll)
        {
            viewOffset = 0;
        }
    }

    public int scrollOffset()
    {
        return scrollOffset;
    }

    public int viewOffset()
    {
        return viewOffset;
    }

    public int horizontalViewOffset()
    {
        return horizontalViewOffset;
    }

    public int hViewOffset()
    {
        return horizontalViewOffset;
    }

    public Style descendantFocusStyle()
    {
        return descendantFocusStyle;
    }

    public void setDescendantFocusStyle(Style value)
    {
        descendantFocusStyle = value;
    }

    public ScrollViewPosition scrollPosition()
    {
        return new ScrollViewPosition(scrollOffset, viewOffset, autoScroll);
    }

    public void restoreScrollPosition(ScrollViewPosition position)
    {
        ScrollViewPosition resolved = position == null
            ? new ScrollViewPosition(0, 0, false)
            : position;
        refreshDynamicChildren(Math.max(1, frame().width()));
        int total = renderedChildren.size() + children().size();
        scrollOffset = clamp(resolved.scrollOffset(), 0, Math.max(0, total - 1));
        viewOffset = Math.max(0, resolved.viewOffset());
        autoScroll = resolved.autoScroll();
        Size viewport = paddedViewportSize(
            new Size(Math.max(1, frame().width()), Math.max(1, frame().height())),
            style());
        int naturalSkip = Math.max(0, contentHeight(viewport.width()) - viewport.height());
        viewOffset = autoScroll ? 0 : Math.min(viewOffset, naturalSkip);
        lastNaturalSkip = naturalSkip;
        pendingTerminalScrollDelta = 0;
    }

    @Override
    public <T extends Element> T addChild(T child)
    {
        T added = super.addChild(child);
        invalidateHeightCache();
        if (autoScroll)
        {
            viewOffset = 0;
        }
        return added;
    }

    public void addChild(List<List<TerminalCell>> child)
    {
        if (child == null)
        {
            throw new IllegalArgumentException("rendered child must not be null");
        }
        renderedChildren.add(copyContent(child));
        invalidateHeightCache();
        if (autoScroll)
        {
            viewOffset = 0;
        }
    }

    public void clearChildren()
    {
        clearChildElements();
        renderedChildren.clear();
        dynamicChildrenWidth = null;
        scrollOffset = 0;
        viewOffset = 0;
        lastNaturalSkip = 0;
        pendingTerminalScrollDelta = 0;
        invalidateHeightCache();
    }

    public void invalidateHeightCache()
    {
        // Java derives heights directly from the canonical child collections,
        // so there is no duplicate height cache to clear.
    }

    public void setDynamicChildrenRenderer(IntFunction<List<List<List<TerminalCell>>>> renderer)
    {
        dynamicChildrenRenderer = renderer;
        invalidateDynamicChildren();
    }

    public void invalidateDynamicChildren()
    {
        dynamicChildrenWidth = null;
        invalidateHeightCache();
    }

    public List<Integer> cachedChildHeights()
    {
        return List.copyOf(childHeights(Math.max(1, frame().width())));
    }

    public int contentHeight()
    {
        return contentHeight(Math.max(1, frame().width()));
    }

    public int contentHeight(int width)
    {
        List<Integer> heights = childHeights(Math.max(1, width));
        int height = 0;
        int itemCount = 0;
        for (int index = Math.max(0, scrollOffset); index < heights.size(); ++index)
        {
            height += heights.get(index);
            ++itemCount;
        }
        if (itemCount > 0)
        {
            height += gap * (itemCount - 1);
        }
        return height;
    }

    public int maxViewOffset(Size viewport)
    {
        int naturalSkip = Math.max(
            0,
            contentHeight(Math.max(1, viewport.width())) - Math.max(1, viewport.height()));
        syncViewOffsetForNaturalSkip(naturalSkip);
        lastNaturalSkip = naturalSkip;
        return naturalSkip;
    }

    public ScrollViewContentWindow contentWindow(Size viewport)
    {
        Size padded = paddedViewportSize(viewport, style());
        refreshDynamicChildren(padded.width());
        int total = renderedChildren.size() + children().size();
        if (total == 0)
        {
            lastNaturalSkip = 0;
            return new ScrollViewContentWindow(padded, 0, 0, 0);
        }
        int naturalSkip = maxViewOffset(padded);
        int clampedViewOffset = Math.min(viewOffset, naturalSkip);
        return new ScrollViewContentWindow(
            padded,
            naturalSkip - clampedViewOffset,
            naturalSkip,
            Math.max(0, gap));
    }

    public boolean scrollTo(int index, Size viewport)
    {
        Size padded = paddedViewportSize(viewport, style());
        refreshDynamicChildren(padded.width());
        int total = renderedChildren.size() + children().size();
        if (total == 0)
        {
            boolean changed = scrollOffset != 0;
            scrollOffset = 0;
            return changed;
        }
        int previous = scrollOffset;
        int maxOffset = Math.max(0, total - maxChildrenInViewport(padded));
        scrollOffset = clamp(index, 0, maxOffset);
        if (scrollOffset == 0)
        {
            autoScroll = false;
        }
        return scrollOffset != previous;
    }

    public boolean scrollBy(int delta, Size viewport)
    {
        return scrollTo(scrollOffset + delta, viewport);
    }

    public boolean scrollLines(int delta, Size viewport)
    {
        Size padded = paddedViewportSize(viewport, style());
        int maximum = maxViewOffset(padded);
        int previous = clamp(viewOffset, 0, maximum);
        viewOffset = clamp(previous + delta, 0, maximum);
        autoScroll = viewOffset == 0;
        if (viewOffset != previous)
        {
            pendingTerminalScrollDelta += viewOffset - previous;
        }
        return viewOffset != previous;
    }

    public boolean scrollToTop(Size viewport)
    {
        Size padded = paddedViewportSize(viewport, style());
        int previous = viewOffset;
        viewOffset = maxViewOffset(padded);
        autoScroll = false;
        return viewOffset != previous;
    }

    public boolean scrollToBottom(Size viewport)
    {
        Size padded = paddedViewportSize(viewport, style());
        maxViewOffset(padded);
        boolean changed = scrollOffset != 0 || viewOffset != 0 || !autoScroll;
        if (viewOffset != 0)
        {
            pendingTerminalScrollDelta -= viewOffset;
        }
        scrollOffset = 0;
        viewOffset = 0;
        autoScroll = true;
        return changed;
    }

    public boolean scrollPageUp(Size viewport)
    {
        return scrollLines(Math.max(1, paddedViewportSize(viewport, style()).height()), viewport);
    }

    public boolean scrollPageDown(Size viewport)
    {
        return scrollLines(-Math.max(1, paddedViewportSize(viewport, style()).height()), viewport);
    }

    public boolean scrollHorizontal(int delta)
    {
        int previous = horizontalViewOffset;
        horizontalViewOffset = Math.max(0, horizontalViewOffset + delta);
        return horizontalViewOffset != previous;
    }

    public int wheelScrollLines(Size viewport)
    {
        int limited = Math.max(1, paddedViewportSize(viewport, style()).height()
            / WHEEL_SCROLL_VIEWPORT_FRACTION);
        return Math.max(1, Math.min(DEFAULT_WHEEL_SCROLL_LINES, limited));
    }

    public boolean canScrollUp(Size viewport)
    {
        return scrollOffset > 0;
    }

    public boolean canScrollDown(Size viewport)
    {
        Size padded = paddedViewportSize(viewport, style());
        refreshDynamicChildren(padded.width());
        int total = renderedChildren.size() + children().size();
        if (total == 0)
        {
            return false;
        }
        int maximumVisible = maxChildrenInViewport(padded);
        return total - scrollOffset > maximumVisible;
    }

    public boolean handleWheel(int wheelDelta, Size viewport)
    {
        if (wheelDelta == 0)
        {
            return false;
        }
        int step = wheelScrollLines(viewport) * Math.max(1, Math.abs(wheelDelta));
        return wheelDelta > 0 ? scrollLines(step, viewport) : scrollLines(-step, viewport);
    }

    @Override
    public boolean handleKey(String key)
    {
        Size viewport = new Size(Math.max(1, frame().width()), Math.max(1, frame().height()));
        return handleKey(key, viewport);
    }

    public boolean handleKey(String key, Size viewport)
    {
        boolean scrollY = style().scrollY() == null || style().scrollY();
        boolean scrollX = style().scrollX() != null && style().scrollX();
        if ("Alt+Up".equals(key))
        {
            return scrollLines(wheelScrollLines(viewport), viewport);
        }
        if ("Alt+Down".equals(key))
        {
            return scrollLines(-wheelScrollLines(viewport), viewport);
        }
        if ("Ctrl+Up".equals(key) || (scrollY && "PageUp".equals(key)))
        {
            return scrollPageUp(viewport);
        }
        if ("Ctrl+Down".equals(key) || (scrollY && "PageDown".equals(key)))
        {
            return scrollPageDown(viewport);
        }
        if (scrollY && ("ArrowUp".equals(key) || "Up".equals(key)))
        {
            return scrollLines(1, viewport);
        }
        if (scrollY && ("ArrowDown".equals(key) || "Down".equals(key)))
        {
            return scrollLines(-1, viewport);
        }
        if (scrollY && "Home".equals(key))
        {
            return scrollToTop(viewport);
        }
        if (scrollY && "End".equals(key))
        {
            return scrollToBottom(viewport);
        }
        if (scrollX && ("ArrowLeft".equals(key) || "Left".equals(key)))
        {
            return scrollHorizontal(-1);
        }
        if (scrollX && ("ArrowRight".equals(key) || "Right".equals(key)))
        {
            return scrollHorizontal(1);
        }
        return false;
    }

    public int consumeTerminalScrollDelta()
    {
        int delta = pendingTerminalScrollDelta;
        pendingTerminalScrollDelta = 0;
        return delta;
    }

    public List<ScrollViewChildView> childViews(Size size)
    {
        Padding padding = constrainedPadding(size, style());
        Size viewport = paddedViewportSize(size, style());
        List<Integer> heights = childHeights(viewport.width());
        int skip = actualSkip(viewport);
        int globalRow = 0;
        int total = heights.size();
        int itemIndex = 0;
        List<ScrollViewChildView> views = new ArrayList<>();
        for (int index = 0; index < renderedChildren.size(); ++index)
        {
            int childHeight = heights.get(itemIndex);
            if (itemIndex >= scrollOffset)
            {
                int localRow = padding.top() + globalRow - skip;
                int visibleTop = Math.max(localRow, padding.top());
                int visibleBottom = Math.min(localRow + childHeight, padding.top() + viewport.height());
                boolean visible = visibleBottom > visibleTop;
                views.add(new ScrollViewChildView(
                    null,
                    new Rect(visibleTop, padding.left(), viewport.width(), visible ? visibleBottom - visibleTop : 0),
                    visible,
                    itemIndex,
                    visible && (visibleTop > localRow || visibleBottom < localRow + childHeight)));
                globalRow += childHeight;
            }
            globalRow += gapAfter(itemIndex, total);
            ++itemIndex;
        }
        for (Element child : children())
        {
            int childHeight = heights.get(itemIndex);
            if (itemIndex >= scrollOffset)
            {
                int localRow = padding.top() + globalRow - skip;
                int visibleTop = Math.max(localRow, padding.top());
                int visibleBottom = Math.min(localRow + childHeight, padding.top() + viewport.height());
                boolean visible = visibleBottom > visibleTop;
                views.add(new ScrollViewChildView(
                    child,
                    new Rect(localRow, padding.left(), viewport.width(), childHeight),
                    visible,
                    itemIndex,
                    visible && (visibleTop > localRow || visibleBottom < localRow + childHeight)));
                globalRow += childHeight;
            }
            globalRow += gapAfter(itemIndex, total);
            ++itemIndex;
        }
        return views;
    }

    public boolean ensureChildVisible(Element child, Size size)
    {
        if (child == null)
        {
            return false;
        }
        Size viewport = paddedViewportSize(size, style());
        List<Integer> heights = childHeights(viewport.width());
        int globalRow = 0;
        int total = heights.size();
        int itemIndex = 0;
        for (List<List<TerminalCell>> renderedChild : renderedChildren)
        {
            if (itemIndex >= scrollOffset)
            {
                globalRow += renderedChild.size();
            }
            globalRow += gapAfter(itemIndex, total);
            ++itemIndex;
        }
        for (Element nativeChild : children())
        {
            int childHeight = heights.get(itemIndex);
            if (nativeChild == child)
            {
                if (itemIndex < scrollOffset)
                {
                    return false;
                }
                int naturalSkip = maxViewOffset(viewport);
                int currentActualSkip = naturalSkip - Math.min(viewOffset, naturalSkip);
                int nextActualSkip = currentActualSkip;
                if (globalRow < currentActualSkip)
                {
                    nextActualSkip = globalRow;
                }
                else if (globalRow + childHeight > currentActualSkip + viewport.height())
                {
                    nextActualSkip = globalRow + childHeight - viewport.height();
                }
                nextActualSkip = clamp(nextActualSkip, 0, naturalSkip);
                int previous = viewOffset;
                viewOffset = naturalSkip - nextActualSkip;
                autoScroll = viewOffset == 0;
                return viewOffset != previous;
            }
            if (itemIndex >= scrollOffset)
            {
                globalRow += childHeight;
            }
            globalRow += gapAfter(itemIndex, total);
            ++itemIndex;
        }
        return false;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        Style effective = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        List<List<TerminalCell>> canvas = RenderHelpers.renderPlainText(
            "",
            Math.max(1, size.width()),
            Math.max(1, size.height()),
            effective);
        Padding padding = constrainedPadding(size, effective);
        Size viewport = paddedViewportSize(size, effective);
        int renderWidth = Math.max(1, viewport.width() + horizontalViewOffset);
        refreshDynamicChildren(renderWidth);
        int skip = actualSkip(new Size(renderWidth, viewport.height()));
        int cursor = padding.top() - skip;
        int total = renderedChildren.size() + children().size();
        int itemIndex = 0;
        for (List<List<TerminalCell>> child : renderedChildren)
        {
            if (itemIndex >= scrollOffset)
            {
                blitChild(canvas, child, cursor, padding, viewport);
                cursor += Math.max(1, child.size());
            }
            cursor += gapAfter(itemIndex, total);
            ++itemIndex;
        }
        for (Element child : children())
        {
            int childHeight = nativeChildHeight(child, renderWidth);
            if (itemIndex >= scrollOffset)
            {
                child.setFrame(new Rect(
                    frame().row() + cursor,
                    frame().col() + padding.left(),
                    viewport.width(),
                    childHeight));
                ElementRenderState childState = new ElementRenderState();
                childState.setFocused(child == effectiveState.focusedElement());
                childState.setEditMode(
                    child == effectiveState.focusedElement()
                        && effectiveState.editMode()
                        && (effectiveState.activeScrollView() == null
                            || effectiveState.activeScrollViewEditElement() == child));
                childState.setFocusedElement(effectiveState.focusedElement());
                childState.setActiveScrollView(effectiveState.activeScrollView());
                childState.setActiveScrollViewEditElement(effectiveState.activeScrollViewEditElement());
                childState.setSuppressActiveScrollViewScopeVisuals(
                    effectiveState.suppressActiveScrollViewScopeVisuals());
                List<List<TerminalCell>> rendered = child.render(
                    new Size(renderWidth, childHeight),
                    childState);
                blitChild(canvas, rendered, cursor, padding, viewport);
                cursor += childHeight;
            }
            cursor += gapAfter(itemIndex, total);
            ++itemIndex;
        }
        int naturalSkip = maxViewOffset(new Size(renderWidth, viewport.height()));
        int indicatorCol = padding.left() + viewport.width() - 1;
        if (skip > 0)
        {
            applyIndicator(canvas, padding.top(), padding.left(), indicatorCol, "^");
        }
        if (Math.min(viewOffset, naturalSkip) > 0)
        {
            applyIndicator(
                canvas,
                padding.top() + viewport.height() - 1,
                padding.left(),
                indicatorCol,
                "v");
        }
        return canvas;
    }

    private List<Integer> childHeights(int width)
    {
        refreshDynamicChildren(width);
        List<Integer> result = new ArrayList<>(renderedChildren.size() + children().size());
        for (List<List<TerminalCell>> child : renderedChildren)
        {
            result.add(Math.max(1, child.size()));
        }
        for (Element child : children())
        {
            result.add(nativeChildHeight(child, width));
        }
        return result;
    }

    private static int nativeChildHeight(Element child, int width)
    {
        if (child instanceof ReusableElement reusable && reusable.child() != null)
        {
            return Math.max(1, GeneratedWindowRuntime.generatedWindowContentSizeForWidth(
                reusable.child(),
                Math.max(1, width)).height());
        }
        return Math.max(1, child.frame().height());
    }

    private int maxChildrenInViewport(Size viewport)
    {
        List<Integer> heights = childHeights(viewport.width());
        if (heights.isEmpty())
        {
            return 0;
        }
        int sampleSize = Math.min(WHEEL_SCROLL_VIEWPORT_FRACTION + 2, heights.size());
        double measured = 0.0;
        for (int index = 0; index < sampleSize; ++index)
        {
            measured += heights.get(index);
        }
        double average = Math.max(1.0, measured / sampleSize);
        int denominator = Math.max(1, (int) average + gap);
        return Math.max(1, (viewport.height() + gap) / denominator);
    }

    private int actualSkip(Size viewport)
    {
        int naturalSkip = maxViewOffset(viewport);
        return naturalSkip - Math.min(viewOffset, naturalSkip);
    }

    private void syncViewOffsetForNaturalSkip(int naturalSkip)
    {
        int previousNaturalSkip = Math.max(0, lastNaturalSkip);
        int nextNaturalSkip = Math.max(0, naturalSkip);
        if (autoScroll)
        {
            viewOffset = 0;
        }
        else if (viewOffset >= previousNaturalSkip)
        {
            viewOffset = nextNaturalSkip;
        }
        else if (viewOffset > nextNaturalSkip)
        {
            viewOffset = nextNaturalSkip;
        }
    }

    private void refreshDynamicChildren(int width)
    {
        if (dynamicChildrenRenderer == null || Integer.valueOf(width).equals(dynamicChildrenWidth))
        {
            return;
        }
        renderedChildren.clear();
        List<List<List<TerminalCell>>> rendered = dynamicChildrenRenderer.apply(width);
        if (rendered != null)
        {
            for (List<List<TerminalCell>> child : rendered)
            {
                renderedChildren.add(copyContent(child));
            }
        }
        dynamicChildrenWidth = width;
        if (autoScroll)
        {
            viewOffset = 0;
        }
    }

    private void blitChild(
        List<List<TerminalCell>> canvas,
        List<List<TerminalCell>> child,
        int cursor,
        Padding padding,
        Size viewport)
    {
        for (int row = 0; row < child.size(); ++row)
        {
            int targetRow = cursor + row;
            if (targetRow < padding.top() || targetRow >= padding.top() + viewport.height())
            {
                continue;
            }
            List<TerminalCell> source = child.get(row);
            for (int col = 0; col < viewport.width(); ++col)
            {
                int sourceCol = horizontalViewOffset + col;
                int targetCol = padding.left() + col;
                if (sourceCol >= source.size() || targetCol >= canvas.get(targetRow).size())
                {
                    continue;
                }
                TerminalCell existing = canvas.get(targetRow).get(targetCol);
                TerminalCell rendered = source.get(sourceCol).copy();
                Color background = rendered.background();
                if (transparentColor(background))
                {
                    rendered.setBackground(existing.background());
                }
                else if (background.rgba().isPresent()
                    && background.rgba().orElseThrow().alpha() < 255
                    && existing.background() != null)
                {
                    rendered.setBackground(background.blendOver(existing.background()));
                }
                if (transparentColor(rendered.foreground()))
                {
                    rendered.setForeground(existing.foreground());
                }
                canvas.get(targetRow).set(targetCol, rendered);
            }
        }
    }

    private static void applyIndicator(
        List<List<TerminalCell>> canvas,
        int row,
        int startCol,
        int col,
        String text)
    {
        if (row < 0 || row >= canvas.size() || col < 0 || col >= canvas.get(row).size())
        {
            return;
        }
        List<TerminalCell> renderedRow = canvas.get(row);
        TerminalCell indicatorCell = renderedRow.get(col);
        indicatorCell.setText(text);
        if (transparentColor(indicatorCell.foreground()))
        {
            Color inferredForeground = null;
            int firstCol = Math.max(0, startCol);
            int lastCol = Math.min(col, renderedRow.size() - 1);
            for (int currentCol = firstCol; currentCol <= lastCol; ++currentCol)
            {
                Color foreground = renderedRow.get(currentCol).foreground();
                if (!transparentColor(foreground))
                {
                    inferredForeground = foreground;
                }
            }
            indicatorCell.setForeground(inferredForeground);
        }
    }

    private static boolean transparentColor(Color color)
    {
        return color == null || !color.isSet() || color.isTransparent();
    }

    private int gapAfter(int index, int total)
    {
        return index >= scrollOffset && index + 1 < total ? gap : 0;
    }

    private static List<List<TerminalCell>> copyContent(List<List<TerminalCell>> content)
    {
        List<List<TerminalCell>> result = new ArrayList<>();
        for (List<TerminalCell> row : content)
        {
            List<TerminalCell> next = new ArrayList<>();
            for (TerminalCell cell : row)
            {
                next.add(cell.copy());
            }
            result.add(next);
        }
        return result;
    }

    private static Size paddedViewportSize(Size size, Style style)
    {
        Padding padding = constrainedPadding(size, style);
        return new Size(
            Math.max(1, size.width() - padding.left() - padding.right()),
            Math.max(1, size.height() - padding.top() - padding.bottom()));
    }

    private static Padding constrainedPadding(Size size, Style style)
    {
        int top = Math.min(styleValue(style.paddingTop(), style.padding()), Math.max(0, size.height() - 1));
        int bottom = Math.min(
            styleValue(style.paddingBottom(), style.padding()),
            Math.max(0, size.height() - top - 1));
        int left = Math.min(styleValue(style.paddingLeft(), style.padding()), Math.max(0, size.width() - 1));
        int right = Math.min(
            styleValue(style.paddingRight(), style.padding()),
            Math.max(0, size.width() - left - 1));
        return new Padding(top, right, bottom, left);
    }

    private static int styleValue(Integer specific, Integer common)
    {
        return Math.max(0, specific == null ? (common == null ? 0 : common) : specific);
    }

    private static int clamp(int value, int minimum, int maximum)
    {
        return Math.max(minimum, Math.min(maximum, value));
    }

    private record Padding(int top, int right, int bottom, int left)
    {
    }
}
