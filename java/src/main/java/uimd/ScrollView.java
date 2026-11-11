package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.TreeMap;
import java.util.function.IntFunction;

public class ScrollView extends Element
{
    public static final int DEFAULT_GAP = 0;
    public static final int DEFAULT_WHEEL_SCROLL_LINES = 4;
    public static final int WHEEL_SCROLL_VIEWPORT_FRACTION = 3;
    public static final int VIRTUAL_SCROLL_OVERSCAN_VIEWPORTS = 1;
    public static final int VIRTUAL_SCROLL_LAYOUT_STABILIZATION_PASSES = 2;

    private static final class VirtualHeightIndex
    {
        private final int count;
        private final int estimatedHeight;
        private final int gap;
        private final Map<Integer, Integer> tree = new HashMap<>();

        VirtualHeightIndex(int count, int estimatedHeight, int gap)
        {
            this.count = Math.max(0, count);
            this.estimatedHeight = Math.max(1, estimatedHeight);
            this.gap = Math.max(0, gap);
        }

        int count()
        {
            return count;
        }

        int gap()
        {
            return gap;
        }

        int unitHeight()
        {
            return estimatedHeight + gap;
        }

        int extentBefore(int index)
        {
            int end = clamp(index, 0, count);
            return end * unitHeight() + deltaPrefix(end);
        }

        int contentHeight()
        {
            return count == 0 ? 0 : Math.max(0, extentBefore(count) - gap);
        }

        int itemHeight(int index)
        {
            if (index < 0 || index >= count)
            {
                throw new IndexOutOfBoundsException("virtual ScrollView item index out of range");
            }
            int delta = deltaPrefix(index + 1) - deltaPrefix(index);
            return Math.max(1, estimatedHeight + delta);
        }

        boolean setItemHeight(int index, int height)
        {
            int previousDelta = itemHeight(index) - estimatedHeight;
            int nextDelta = Math.max(1, height) - estimatedHeight;
            int change = nextDelta - previousDelta;
            for (int treeIndex = index + 1; change != 0 && treeIndex <= count;
                treeIndex += treeIndex & -treeIndex)
            {
                int nextValue = tree.getOrDefault(treeIndex, 0) + change;
                if (nextValue == 0)
                {
                    tree.remove(treeIndex);
                }
                else
                {
                    tree.put(treeIndex, nextValue);
                }
            }
            return change != 0;
        }

        boolean resetItemHeight(int index)
        {
            return setItemHeight(index, estimatedHeight);
        }

        int indexAtOffset(int offset)
        {
            if (count == 0)
            {
                return 0;
            }
            int target = clamp(offset, 0, contentHeight());
            int index = 0;
            int prefix = 0;
            int bit = Integer.highestOneBit(count);
            while (bit > 0)
            {
                int candidate = index + bit;
                if (candidate <= count)
                {
                    int nodeWidth = candidate & -candidate;
                    int nodeExtent = nodeWidth * unitHeight() + tree.getOrDefault(candidate, 0);
                    if (prefix + nodeExtent <= target)
                    {
                        index = candidate;
                        prefix += nodeExtent;
                    }
                }
                bit >>= 1;
            }
            return Math.min(index, count - 1);
        }

        private int deltaPrefix(int end)
        {
            int total = 0;
            for (int index = clamp(end, 0, count); index > 0; index -= index & -index)
            {
                total += tree.getOrDefault(index, 0);
            }
            return total;
        }
    }

    private record VirtualMaterializedRow(Object key, Element child)
    {
    }

    private record VirtualScrollAnchor(int index, int within, int viewportHeight)
    {
    }

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
    private VirtualScrollDataSource virtualDataSource;
    private VirtualHeightIndex virtualHeightIndex;
    private Integer virtualHeightWidth;
    private final TreeMap<Integer, VirtualMaterializedRow> virtualMaterialized = new TreeMap<>();
    private final List<Element> virtualRecyclePool = new ArrayList<>();
    private Element virtualPinnedElement;
    private int virtualLastRowsMeasured;
    private int virtualLastRowsConsidered;
    private int virtualLastRowsRendered;

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

    public void setVirtualDataSource(VirtualScrollDataSource value)
    {
        if (value == null)
        {
            throw new IllegalArgumentException("virtual ScrollView data source must not be null");
        }
        if (value.itemCount() < 0)
        {
            throw new IllegalArgumentException("virtual ScrollView item count must not be negative");
        }
        Integer estimate = value.estimatedHeight(Math.max(1, frame().width()));
        if (estimate != null && estimate <= 0)
        {
            throw new IllegalArgumentException("virtual ScrollView estimated height must be positive");
        }
        renderedChildren.clear();
        dynamicChildrenRenderer = null;
        dynamicChildrenWidth = null;
        clearChildElements();
        virtualMaterialized.clear();
        virtualRecyclePool.clear();
        virtualDataSource = value;
        virtualHeightIndex = null;
        virtualHeightWidth = null;
        virtualPinnedElement = null;
        scrollOffset = 0;
        viewOffset = 0;
        lastNaturalSkip = 0;
    }

    public void clearVirtualDataSource()
    {
        if (virtualDataSource == null)
        {
            return;
        }
        clearChildElements();
        virtualMaterialized.clear();
        virtualRecyclePool.clear();
        virtualDataSource = null;
        virtualHeightIndex = null;
        virtualHeightWidth = null;
        virtualPinnedElement = null;
        scrollOffset = 0;
        viewOffset = 0;
        lastNaturalSkip = 0;
    }

    public void virtualItemsInserted(int index)
    {
        virtualItemsInserted(index, 1);
    }

    public void virtualItemsInserted(int index, int count)
    {
        if (virtualDataSource == null || count <= 0)
        {
            return;
        }
        int first = Math.max(0, index);
        VirtualScrollAnchor anchor = captureVirtualAnchor(null);
        TreeMap<Integer, VirtualMaterializedRow> shifted = new TreeMap<>();
        for (Map.Entry<Integer, VirtualMaterializedRow> entry : virtualMaterialized.entrySet())
        {
            int oldIndex = entry.getKey();
            shifted.put(oldIndex >= first ? oldIndex + count : oldIndex, entry.getValue());
        }
        virtualMaterialized.clear();
        virtualMaterialized.putAll(shifted);
        rebuildVirtualHeightIndex(null);
        if (anchor != null && first <= anchor.index())
        {
            anchor = new VirtualScrollAnchor(
                anchor.index() + count,
                anchor.within(),
                anchor.viewportHeight());
        }
        restoreVirtualAnchor(anchor);
    }

    public void virtualItemsRemoved(int index)
    {
        virtualItemsRemoved(index, 1);
    }

    public void virtualItemsRemoved(int index, int count)
    {
        if (virtualDataSource == null || count <= 0)
        {
            return;
        }
        int first = Math.max(0, index);
        int end = first + count;
        VirtualScrollAnchor anchor = captureVirtualAnchor(null);
        TreeMap<Integer, VirtualMaterializedRow> shifted = new TreeMap<>();
        for (Map.Entry<Integer, VirtualMaterializedRow> entry : virtualMaterialized.entrySet())
        {
            int oldIndex = entry.getKey();
            if (oldIndex >= first && oldIndex < end)
            {
                recycleVirtualChild(entry.getValue().child());
            }
            else
            {
                shifted.put(oldIndex >= end ? oldIndex - count : oldIndex, entry.getValue());
            }
        }
        virtualMaterialized.clear();
        virtualMaterialized.putAll(shifted);
        rebuildVirtualHeightIndex(null);
        if (anchor != null)
        {
            if (anchor.index() >= end)
            {
                anchor = new VirtualScrollAnchor(
                    anchor.index() - count,
                    anchor.within(),
                    anchor.viewportHeight());
            }
            else if (anchor.index() >= first)
            {
                anchor = new VirtualScrollAnchor(
                    Math.min(first, Math.max(0, virtualCount() - 1)),
                    0,
                    anchor.viewportHeight());
            }
        }
        restoreVirtualAnchor(anchor);
    }

    public void virtualItemsReplaced(int index)
    {
        virtualItemsReplaced(index, 1);
    }

    public void virtualItemsReplaced(int index, int count)
    {
        if (virtualDataSource == null || count <= 0)
        {
            return;
        }
        int first = Math.max(0, index);
        int end = Math.min(virtualCount(), first + count);
        VirtualScrollAnchor anchor = captureVirtualAnchor(null);
        for (int itemIndex = first; itemIndex < end; ++itemIndex)
        {
            VirtualMaterializedRow row = virtualMaterialized.get(itemIndex);
            if (row != null)
            {
                Object key = virtualDataSource.itemKey(itemIndex);
                if (Objects.equals(row.key(), key))
                {
                    Element refreshed = virtualDataSource.materialize(itemIndex, row.child());
                    if (refreshed == null)
                    {
                        throw new IllegalStateException(
                            "virtual ScrollView materializer must return a child");
                    }
                    if (refreshed != row.child())
                    {
                        recycleVirtualChild(row.child());
                    }
                    virtualMaterialized.put(itemIndex, new VirtualMaterializedRow(key, refreshed));
                }
                else
                {
                    virtualMaterialized.remove(itemIndex);
                    recycleVirtualChild(row.child());
                }
            }
            if (virtualHeightIndex != null && itemIndex < virtualHeightIndex.count())
            {
                virtualHeightIndex.resetItemHeight(itemIndex);
            }
        }
        restoreVirtualAnchor(anchor);
    }

    public void invalidateVirtualHeights()
    {
        VirtualScrollAnchor anchor = captureVirtualAnchor(null);
        rebuildVirtualHeightIndex(null);
        restoreVirtualAnchor(anchor);
    }

    public void invalidateVirtualHeights(int index, int count)
    {
        if (virtualDataSource == null || count <= 0)
        {
            return;
        }
        VirtualScrollAnchor anchor = captureVirtualAnchor(null);
        if (virtualHeightIndex != null)
        {
            int first = clamp(index, 0, virtualHeightIndex.count());
            int end = clamp(index + count, first, virtualHeightIndex.count());
            for (int itemIndex = first; itemIndex < end; ++itemIndex)
            {
                virtualHeightIndex.resetItemHeight(itemIndex);
            }
        }
        restoreVirtualAnchor(anchor);
    }

    public VirtualScrollDiagnostics virtualDiagnostics()
    {
        return new VirtualScrollDiagnostics(
            virtualCount(),
            virtualMaterialized.size(),
            virtualRecyclePool.size(),
            virtualLastRowsMeasured,
            virtualLastRowsConsidered,
            virtualLastRowsRendered);
    }

    void setVirtualPinnedElement(Element value)
    {
        virtualPinnedElement = value;
    }

    @Override
    public List<Element> children()
    {
        if (virtualDataSource == null)
        {
            return super.children();
        }
        List<Element> result = new ArrayList<>(virtualMaterialized.size());
        for (VirtualMaterializedRow row : virtualMaterialized.values())
        {
            result.add(row.child());
        }
        return Collections.unmodifiableList(result);
    }

    private int virtualCount()
    {
        if (virtualDataSource == null)
        {
            return 0;
        }
        int count = virtualDataSource.itemCount();
        if (count < 0)
        {
            throw new IllegalStateException("virtual ScrollView item count must not be negative");
        }
        return count;
    }

    private Integer virtualEstimateForWidth(int width)
    {
        if (virtualDataSource == null)
        {
            return null;
        }
        Integer estimate = virtualDataSource.estimatedHeight(Math.max(1, width));
        if (estimate != null)
        {
            if (estimate <= 0)
            {
                throw new IllegalStateException("virtual ScrollView estimated height must be positive");
            }
            return estimate;
        }
        if (virtualCount() == 0)
        {
            return null;
        }
        return nativeChildHeight(materializeVirtualChild(0), width);
    }

    private void rebuildVirtualHeightIndex(Integer width)
    {
        if (virtualDataSource == null)
        {
            virtualHeightIndex = null;
            virtualHeightWidth = null;
            return;
        }
        Integer resolvedWidth = width == null ? virtualHeightWidth : width;
        if (resolvedWidth == null)
        {
            virtualHeightIndex = null;
            return;
        }
        int renderWidth = Math.max(1, resolvedWidth);
        int count = virtualCount();
        Integer estimate = virtualEstimateForWidth(renderWidth);
        if (count == 0 || estimate == null)
        {
            virtualHeightIndex = null;
            virtualHeightWidth = renderWidth;
            return;
        }
        VirtualHeightIndex next = new VirtualHeightIndex(count, estimate, gap);
        virtualHeightIndex = next;
        virtualHeightWidth = renderWidth;
        for (Map.Entry<Integer, VirtualMaterializedRow> entry : virtualMaterialized.entrySet())
        {
            if (entry.getKey() < count)
            {
                next.setItemHeight(
                    entry.getKey(),
                    nativeChildHeight(entry.getValue().child(), renderWidth));
            }
        }
    }

    private VirtualScrollAnchor captureVirtualAnchor(Integer viewportHeight)
    {
        if (autoScroll || virtualHeightIndex == null || virtualHeightIndex.count() == 0)
        {
            return null;
        }
        int inferredHeight = Math.max(
            1,
            virtualHeightIndex.contentHeight() - Math.max(0, lastNaturalSkip));
        int resolvedHeight = Math.max(1, viewportHeight == null ? inferredHeight : viewportHeight);
        int naturalSkip = Math.max(0, virtualHeightIndex.contentHeight() - resolvedHeight);
        int actualSkip = naturalSkip - Math.min(viewOffset, naturalSkip);
        int anchorIndex = virtualHeightIndex.indexAtOffset(actualSkip);
        return new VirtualScrollAnchor(
            anchorIndex,
            actualSkip - virtualHeightIndex.extentBefore(anchorIndex),
            resolvedHeight);
    }

    private void restoreVirtualAnchor(VirtualScrollAnchor anchor)
    {
        if (autoScroll)
        {
            viewOffset = 0;
            return;
        }
        if (anchor == null)
        {
            return;
        }
        if (virtualHeightIndex == null || virtualHeightIndex.count() == 0)
        {
            viewOffset = 0;
            lastNaturalSkip = 0;
            return;
        }
        int anchorIndex = clamp(anchor.index(), 0, virtualHeightIndex.count() - 1);
        int anchorSpan = virtualHeightIndex.itemHeight(anchorIndex);
        if (anchorIndex + 1 < virtualHeightIndex.count())
        {
            anchorSpan += virtualHeightIndex.gap();
        }
        int within = clamp(anchor.within(), 0, anchorSpan - 1);
        int naturalSkip = Math.max(
            0,
            virtualHeightIndex.contentHeight() - anchor.viewportHeight());
        int actualSkip = clamp(
            virtualHeightIndex.extentBefore(anchorIndex) + within,
            0,
            naturalSkip);
        viewOffset = naturalSkip - actualSkip;
        lastNaturalSkip = naturalSkip;
    }

    private VirtualHeightIndex ensureVirtualHeightIndex(int width)
    {
        int renderWidth = Math.max(1, width);
        int count = virtualCount();
        if (virtualHeightIndex == null
            || !Integer.valueOf(renderWidth).equals(virtualHeightWidth)
            || virtualHeightIndex.count() != count
            || virtualHeightIndex.gap() != gap)
        {
            VirtualScrollAnchor anchor = captureVirtualAnchor(null);
            rebuildVirtualHeightIndex(renderWidth);
            restoreVirtualAnchor(anchor);
        }
        return virtualHeightIndex;
    }

    private Element materializeVirtualChild(int index)
    {
        if (virtualDataSource == null)
        {
            throw new IllegalStateException("virtual ScrollView data source is not configured");
        }
        Object key = virtualDataSource.itemKey(index);
        VirtualMaterializedRow current = virtualMaterialized.get(index);
        if (current != null && Objects.equals(current.key(), key))
        {
            return current.child();
        }
        if (current != null)
        {
            virtualMaterialized.remove(index);
            recycleVirtualChild(current.child());
        }
        Element recycled = virtualRecyclePool.isEmpty()
            ? null
            : virtualRecyclePool.remove(virtualRecyclePool.size() - 1);
        if (recycled != null)
        {
            resetVirtualChildForReuse(recycled);
        }
        Element child = virtualDataSource.materialize(index, recycled);
        if (child == null)
        {
            throw new IllegalStateException("virtual ScrollView materializer must return a child");
        }
        virtualMaterialized.put(index, new VirtualMaterializedRow(key, child));
        return child;
    }

    private void recycleVirtualChild(Element child)
    {
        resetVirtualChildForReuse(child);
        virtualRecyclePool.add(child);
    }

    private static void resetVirtualChildForReuse(Element child)
    {
        if (child instanceof Label label)
        {
            label.clearSelection();
        }
        if (child instanceof TextInput textInput)
        {
            textInput.clearSelection();
        }
        if (child instanceof ReusableElement reusable && reusable.child() != null)
        {
            for (Element nested : reusable.child().elements())
            {
                resetVirtualChildForReuse(nested);
            }
        }
        for (Element nested : child.children())
        {
            resetVirtualChildForReuse(nested);
        }
    }

    private boolean virtualChildIsPinned(Element child)
    {
        return virtualPinnedElement != null && elementTreeContains(child, virtualPinnedElement);
    }

    private static boolean elementTreeContains(Element root, Element target)
    {
        if (root == target)
        {
            return true;
        }
        for (Element child : root.children())
        {
            if (elementTreeContains(child, target))
            {
                return true;
            }
        }
        if (root instanceof ReusableElement reusable && reusable.child() != null)
        {
            for (Element child : reusable.child().elements())
            {
                if (elementTreeContains(child, target))
                {
                    return true;
                }
            }
        }
        return false;
    }

    private List<ScrollViewChildView> refreshVirtualMaterialization(Size viewport, Padding padding)
    {
        VirtualHeightIndex heightIndex = ensureVirtualHeightIndex(viewport.width());
        int count = virtualCount();
        virtualLastRowsMeasured = 0;
        virtualLastRowsConsidered = 0;
        if (count == 0 || heightIndex == null)
        {
            for (VirtualMaterializedRow row : virtualMaterialized.values())
            {
                recycleVirtualChild(row.child());
            }
            virtualMaterialized.clear();
            return List.of();
        }

        int viewportHeight = Math.max(1, viewport.height());
        int naturalSkip = Math.max(0, heightIndex.contentHeight() - viewportHeight);
        syncViewOffsetForNaturalSkip(naturalSkip);
        lastNaturalSkip = naturalSkip;
        int actualSkip = naturalSkip - Math.min(viewOffset, naturalSkip);
        int anchorIndex = heightIndex.indexAtOffset(actualSkip);
        int anchorWithin = actualSkip - heightIndex.extentBefore(anchorIndex);
        Map<Integer, Boolean> required = new HashMap<>();

        for (int pass = 0; pass < VIRTUAL_SCROLL_LAYOUT_STABILIZATION_PASSES; ++pass)
        {
            int overscan = viewportHeight * VIRTUAL_SCROLL_OVERSCAN_VIEWPORTS;
            int rangeStart = Math.max(0, actualSkip - overscan);
            int rangeEnd = Math.min(
                heightIndex.contentHeight(),
                actualSkip + viewportHeight + overscan);
            int first = heightIndex.indexAtOffset(rangeStart);
            if (pass == 0)
            {
                int coarseLast = heightIndex.indexAtOffset(rangeEnd);
                for (Map.Entry<Integer, VirtualMaterializedRow> entry
                    : new ArrayList<>(virtualMaterialized.entrySet()))
                {
                    if (entry.getKey() >= first && entry.getKey() <= coarseLast)
                    {
                        continue;
                    }
                    if (virtualChildIsPinned(entry.getValue().child()))
                    {
                        continue;
                    }
                    virtualMaterialized.remove(entry.getKey());
                    recycleVirtualChild(entry.getValue().child());
                }
            }
            int itemIndex = first;
            int row = heightIndex.extentBefore(first);
            boolean hasEntries = false;
            while (itemIndex < count && (row < rangeEnd || !hasEntries))
            {
                ++virtualLastRowsConsidered;
                Element child = materializeVirtualChild(itemIndex);
                int measured = nativeChildHeight(child, viewport.width());
                ++virtualLastRowsMeasured;
                heightIndex.setItemHeight(itemIndex, measured);
                required.put(itemIndex, Boolean.TRUE);
                hasEntries = true;
                row += heightIndex.itemHeight(itemIndex) + heightIndex.gap();
                ++itemIndex;
            }
            if (autoScroll)
            {
                viewOffset = 0;
            }
            else
            {
                int refinedNatural = Math.max(0, heightIndex.contentHeight() - viewportHeight);
                int refinedSkip = clamp(
                    heightIndex.extentBefore(anchorIndex) + anchorWithin,
                    0,
                    refinedNatural);
                viewOffset = refinedNatural - refinedSkip;
            }
            int nextNatural = Math.max(0, heightIndex.contentHeight() - viewportHeight);
            int nextActual = nextNatural - Math.min(viewOffset, nextNatural);
            naturalSkip = nextNatural;
            if (nextActual == actualSkip)
            {
                actualSkip = nextActual;
                break;
            }
            actualSkip = nextActual;
        }

        for (Map.Entry<Integer, VirtualMaterializedRow> entry : virtualMaterialized.entrySet())
        {
            if (virtualChildIsPinned(entry.getValue().child()))
            {
                required.put(entry.getKey(), Boolean.TRUE);
            }
        }
        for (Map.Entry<Integer, VirtualMaterializedRow> entry
            : new ArrayList<>(virtualMaterialized.entrySet()))
        {
            if (!required.containsKey(entry.getKey()))
            {
                virtualMaterialized.remove(entry.getKey());
                recycleVirtualChild(entry.getValue().child());
            }
        }
        int recycleLimit = required.size();
        if (virtualRecyclePool.size() > recycleLimit)
        {
            virtualRecyclePool.subList(0, virtualRecyclePool.size() - recycleLimit).clear();
        }
        lastNaturalSkip = naturalSkip;

        int viewportTop = padding.top();
        int viewportBottom = viewportTop + viewportHeight;
        List<ScrollViewChildView> result = new ArrayList<>();
        for (Map.Entry<Integer, VirtualMaterializedRow> entry : virtualMaterialized.entrySet())
        {
            int itemIndex = entry.getKey();
            int childHeight = heightIndex.itemHeight(itemIndex);
            int childRow = viewportTop + heightIndex.extentBefore(itemIndex) - actualSkip;
            int childBottom = childRow + childHeight;
            boolean visible = childBottom > viewportTop && childRow < viewportBottom;
            result.add(new ScrollViewChildView(
                entry.getValue().child(),
                new Rect(childRow, padding.left(), viewport.width(), childHeight),
                visible,
                itemIndex,
                visible && (childRow < viewportTop || childBottom > viewportBottom)));
        }
        return result;
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
        int total = virtualDataSource == null
            ? renderedChildren.size() + super.children().size()
            : virtualCount();
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
        if (virtualDataSource != null)
        {
            clearVirtualDataSource();
        }
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
        if (virtualDataSource != null)
        {
            clearVirtualDataSource();
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
        if (virtualDataSource != null)
        {
            clearVirtualDataSource();
            return;
        }
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
        if (virtualDataSource != null)
        {
            virtualHeightIndex = null;
        }
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
        if (virtualDataSource != null)
        {
            VirtualHeightIndex index = ensureVirtualHeightIndex(Math.max(1, width));
            return index == null ? 0 : index.contentHeight();
        }
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
        int total = virtualDataSource == null
            ? renderedChildren.size() + super.children().size()
            : virtualCount();
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
        if (virtualDataSource != null)
        {
            int count = virtualCount();
            VirtualHeightIndex heightIndex = ensureVirtualHeightIndex(padded.width());
            if (count == 0 || heightIndex == null)
            {
                boolean changed = scrollOffset != 0 || viewOffset != 0;
                scrollOffset = 0;
                viewOffset = 0;
                return changed;
            }
            int targetIndex = clamp(index, 0, count - 1);
            int naturalSkip = Math.max(0, heightIndex.contentHeight() - padded.height());
            int targetSkip = Math.min(naturalSkip, heightIndex.extentBefore(targetIndex));
            int previous = viewOffset;
            scrollOffset = 0;
            lastNaturalSkip = naturalSkip;
            viewOffset = naturalSkip - targetSkip;
            autoScroll = viewOffset == 0;
            return viewOffset != previous;
        }
        refreshDynamicChildren(padded.width());
        int total = renderedChildren.size() + super.children().size();
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
        if (virtualDataSource != null)
        {
            Size padded = paddedViewportSize(viewport, style());
            VirtualHeightIndex heightIndex = ensureVirtualHeightIndex(padded.width());
            if (heightIndex == null)
            {
                return false;
            }
            int naturalSkip = Math.max(0, heightIndex.contentHeight() - padded.height());
            int actualSkip = naturalSkip - Math.min(viewOffset, naturalSkip);
            return scrollTo(heightIndex.indexAtOffset(actualSkip) + delta, viewport);
        }
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
        if (virtualDataSource != null)
        {
            Size padded = paddedViewportSize(viewport, style());
            int maximum = maxViewOffset(padded);
            return maximum - Math.min(viewOffset, maximum) > 0;
        }
        return scrollOffset > 0;
    }

    public boolean canScrollDown(Size viewport)
    {
        if (virtualDataSource != null)
        {
            return viewOffset > 0;
        }
        Size padded = paddedViewportSize(viewport, style());
        refreshDynamicChildren(padded.width());
        int total = renderedChildren.size() + super.children().size();
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
        if (virtualDataSource != null)
        {
            return refreshVirtualMaterialization(viewport, padding);
        }
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
        for (Element child : super.children())
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
        if (virtualDataSource != null)
        {
            Integer itemIndex = null;
            for (Map.Entry<Integer, VirtualMaterializedRow> entry : virtualMaterialized.entrySet())
            {
                if (elementTreeContains(entry.getValue().child(), child))
                {
                    itemIndex = entry.getKey();
                    break;
                }
            }
            VirtualHeightIndex heightIndex = ensureVirtualHeightIndex(viewport.width());
            if (itemIndex == null || heightIndex == null)
            {
                return false;
            }
            int targetTop = heightIndex.extentBefore(itemIndex);
            int targetBottom = targetTop + heightIndex.itemHeight(itemIndex);
            int naturalSkip = Math.max(0, heightIndex.contentHeight() - viewport.height());
            int currentActualSkip = naturalSkip - Math.min(viewOffset, naturalSkip);
            int nextActualSkip = currentActualSkip;
            if (targetTop < currentActualSkip)
            {
                nextActualSkip = targetTop;
            }
            else if (targetBottom > currentActualSkip + viewport.height())
            {
                nextActualSkip = targetBottom - viewport.height();
            }
            nextActualSkip = clamp(nextActualSkip, 0, naturalSkip);
            int nextViewOffset = naturalSkip - nextActualSkip;
            if (nextViewOffset == viewOffset)
            {
                return false;
            }
            viewOffset = nextViewOffset;
            autoScroll = viewOffset == 0;
            return true;
        }
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
        for (Element nativeChild : super.children())
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
        if (virtualDataSource != null)
        {
            List<ScrollViewChildView> views = refreshVirtualMaterialization(
                new Size(renderWidth, viewport.height()),
                padding);
            virtualLastRowsRendered = 0;
            for (ScrollViewChildView view : views)
            {
                if (!view.visible() || view.element() == null)
                {
                    continue;
                }
                Element child = view.element();
                child.setFrame(new Rect(
                    frame().row() + view.frame().row(),
                    frame().col() + view.frame().col(),
                    viewport.width(),
                    view.frame().height()));
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
                    new Size(renderWidth, view.frame().height()),
                    childState);
                blitChild(canvas, rendered, view.frame().row(), padding, viewport);
                ++virtualLastRowsRendered;
            }
            VirtualHeightIndex heightIndex = ensureVirtualHeightIndex(renderWidth);
            int naturalSkip = heightIndex == null
                ? 0
                : Math.max(0, heightIndex.contentHeight() - viewport.height());
            int skip = naturalSkip - Math.min(viewOffset, naturalSkip);
            int indicatorCol = padding.left() + viewport.width() - 1;
            if (skip > 0)
            {
                applyIndicator(canvas, padding.top(), padding.left(), indicatorCol, "^");
            }
            if (viewOffset > 0)
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
        for (Element child : super.children())
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
        List<Integer> result = new ArrayList<>(renderedChildren.size() + super.children().size());
        for (List<List<TerminalCell>> child : renderedChildren)
        {
            result.add(Math.max(1, child.size()));
        }
        for (Element child : super.children())
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
