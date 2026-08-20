package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class ListBox extends Element
{
    private final List<String> options = new ArrayList<>();
    private final List<Integer> selectedIndices = new ArrayList<>();
    private final List<String> disabledValues = new ArrayList<>();
    private int selectedIndex;
    private int activeIndex;
    private boolean multiple;
    private boolean activeItemVisible;
    private int scrollOffset;
    private int lastViewportHeight;

    public ListBox(String name)
    {
        this(name, List.of());
    }

    public ListBox(String name, List<String> options)
    {
        super(name);
        setOptions(options);
    }

    public ListBox(String name, List<String> options, boolean multiple)
    {
        super(name);
        this.multiple = multiple;
        setOptions(options);
    }

    public List<String> options()
    {
        return Collections.unmodifiableList(options);
    }

    public void setOptions(List<String> value)
    {
        options.clear();
        if (value != null)
        {
            options.addAll(value);
        }
        if (selectedIndices.isEmpty())
        {
            setSelectedIndex(selectedIndex);
        }
        else
        {
            setSelectedIndices(new ArrayList<>(selectedIndices));
        }
        if (options.isEmpty())
        {
            scrollOffset = 0;
        }
    }

    public List<String> disabledValues()
    {
        return Collections.unmodifiableList(disabledValues);
    }

    public void setDisabledValues(List<String> values)
    {
        disabledValues.clear();
        if (values != null)
        {
            disabledValues.addAll(values);
        }
    }

    public int selectedIndex()
    {
        return selectedIndex;
    }

    public List<Integer> selectedIndices()
    {
        return Collections.unmodifiableList(selectedIndices);
    }

    public int activeIndex()
    {
        return activeIndex;
    }

    public boolean multiple()
    {
        return multiple;
    }

    public void setMultiple(boolean value)
    {
        multiple = value;
        if (!multiple && selectedIndices.size() > 1)
        {
            setSelectedIndex(selectedIndex);
        }
    }

    public boolean activeItemVisible()
    {
        return activeItemVisible;
    }

    public int scrollOffset()
    {
        return scrollOffset;
    }

    public List<String> selectedValues()
    {
        List<String> result = new ArrayList<>();
        for (int index : selectedIndices)
        {
            if (index >= 0 && index < options.size())
            {
                result.add(options.get(index));
            }
        }
        return result;
    }

    public void setSelectedIndex(int value)
    {
        if (options.isEmpty())
        {
            selectedIndex = 0;
            activeIndex = 0;
            selectedIndices.clear();
            scrollOffset = 0;
            return;
        }
        selectedIndex = clamp(value, 0, options.size() - 1);
        activeIndex = selectedIndex;
        if (!multiple)
        {
            selectedIndices.clear();
            selectedIndices.add(selectedIndex);
        }
        ensureSelectionVisible();
    }

    public void setSelectedIndices(List<Integer> indices)
    {
        selectedIndices.clear();
        if (options.isEmpty())
        {
            selectedIndex = 0;
            activeIndex = 0;
            scrollOffset = 0;
            return;
        }
        if (indices != null)
        {
            for (int index : indices)
            {
                int resolved = clamp(index, 0, options.size() - 1);
                if (!selectedIndices.contains(resolved))
                {
                    selectedIndices.add(resolved);
                }
                if (!multiple)
                {
                    break;
                }
            }
        }
        if (!selectedIndices.isEmpty())
        {
            selectedIndex = selectedIndices.get(selectedIndices.size() - 1);
            activeIndex = selectedIndex;
        }
        ensureSelectionVisible();
    }

    public void setSelectedValues(List<String> values)
    {
        List<Integer> indices = new ArrayList<>();
        if (values != null)
        {
            for (String value : values)
            {
                int index = options.indexOf(value);
                if (index >= 0)
                {
                    indices.add(index);
                }
            }
        }
        setSelectedIndices(indices);
    }

    public void setActiveIndex(int value)
    {
        if (options.isEmpty())
        {
            selectedIndex = 0;
            activeIndex = 0;
            scrollOffset = 0;
            return;
        }
        activeIndex = clamp(value, 0, options.size() - 1);
        if (lastViewportHeight > 0)
        {
            ensureIndexVisible(activeIndex, lastViewportHeight);
        }
    }

    public void toggleSelectedIndex(int value)
    {
        if (options.isEmpty())
        {
            return;
        }
        setActiveIndex(value);
        if (selectedIndices.contains(activeIndex))
        {
            selectedIndices.remove(Integer.valueOf(activeIndex));
        }
        else
        {
            selectedIndices.add(activeIndex);
        }
    }

    public boolean scrollLines(int delta)
    {
        int viewportHeight = lastViewportHeight > 0 ? lastViewportHeight : Math.max(1, frame().height());
        int previous = scrollOffset;
        scrollBy(-delta, viewportHeight);
        return scrollOffset != previous;
    }

    public void scrollBy(int delta, int viewportHeight)
    {
        if (options.isEmpty())
        {
            scrollOffset = 0;
            return;
        }
        int maxOffset = Math.max(0, options.size() - Math.max(1, viewportHeight));
        int next = clamp(scrollOffset + delta, 0, maxOffset);
        if (next == scrollOffset)
        {
            return;
        }
        scrollOffset = next;
    }

    public void showActiveItem()
    {
        activeItemVisible = true;
    }

    public void hideActiveItem()
    {
        activeItemVisible = false;
    }

    @Override
    public boolean handleKey(String key)
    {
        if ("Up".equals(key))
        {
            setActiveIndex(activeIndex - 1);
            showActiveItem();
            return true;
        }
        if ("Down".equals(key))
        {
            setActiveIndex(activeIndex + 1);
            showActiveItem();
            return true;
        }
        if ("Enter".equals(key) && multiple && activeIndex >= 0 && activeIndex < options.size())
        {
            toggleSelectedIndex(activeIndex);
            return true;
        }
        if ("Enter".equals(key))
        {
            setSelectedIndex(activeIndex);
            hideActiveItem();
            return true;
        }
        return false;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        Style style = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        int height = Math.max(1, size.height());
        int width = Math.max(1, size.width());
        lastViewportHeight = height;
        scrollOffset = clamp(scrollOffset, 0, Math.max(0, options.size() - height));
        if (effectiveState.editMode() && activeItemVisible)
        {
            ensureIndexVisible(activeIndex, height);
        }
        else
        {
            ensureIndexVisible(selectedIndex, height);
        }
        boolean hasAbove = scrollOffset > 0;
        boolean hasBelow = scrollOffset + height < options.size();
        List<List<TerminalCell>> rows = new ArrayList<>();
        for (int row = 0; row < height; ++row)
        {
            int optionIndex = scrollOffset + row;
            String text = optionIndex < options.size() ? options.get(optionIndex) : "";
            boolean selected = selectedIndices.contains(optionIndex);
            boolean active = optionIndex < options.size()
                && effectiveState.editMode()
                && activeItemVisible
                && optionIndex == activeIndex;
            boolean disabled = optionIndex < options.size() && disabledValues.contains(options.get(optionIndex));
            Style rowStyle = style.copy();
            if (selected)
            {
                rowStyle = mergeRowLayer(rowStyle, selectedStyle());
            }
            if (active)
            {
                rowStyle = mergeRowLayer(rowStyle, activeStyle() != null ? activeStyle() : selectedStyle());
            }
            if (disabled)
            {
                rowStyle = style.copy();
                if (disabledStyle() != null)
                {
                    rowStyle.merge(disabledStyle());
                }
            }
            List<TerminalCell> rendered = RenderHelpers.renderPlainText(text, width, 1, rowStyle).get(0);
            if (row == 0 && hasAbove)
            {
                rendered.get(width - 1).setText("^");
            }
            else if (row == height - 1 && hasBelow)
            {
                rendered.get(width - 1).setText("v");
            }
            rows.add(rendered);
        }
        return rows;
    }

    private void ensureSelectionVisible()
    {
        if (lastViewportHeight > 0)
        {
            ensureIndexVisible(selectedIndex, lastViewportHeight);
        }
    }

    private void ensureIndexVisible(int index, int height)
    {
        if (index < scrollOffset)
        {
            scrollOffset = index;
        }
        else if (index >= scrollOffset + height)
        {
            scrollOffset = index - height + 1;
        }
    }

    private static Style mergeRowLayer(Style base, Style layer)
    {
        if (layer == null)
        {
            return base;
        }
        Color inheritedBackground = base.background();
        base.merge(layer);
        if (layer.background() != null
            && layer.background().rgba().isPresent()
            && layer.background().rgba().orElseThrow().alpha() < 255
            && inheritedBackground != null)
        {
            base.setBackground(layer.background().blendOver(inheritedBackground));
        }
        return base;
    }

    private static int clamp(int value, int minimum, int maximum)
    {
        return Math.max(minimum, Math.min(maximum, value));
    }
}
