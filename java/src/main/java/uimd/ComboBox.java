package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class ComboBox extends Element
{
    private final List<String> options = new ArrayList<>();
    private int selectedIndex;

    public ComboBox(String name)
    {
        this(name, List.of());
    }

    public ComboBox(String name, List<String> options)
    {
        super(name);
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
        selectedIndex = options.isEmpty() ? 0 : clamp(selectedIndex, 0, options.size() - 1);
    }

    public int selectedIndex()
    {
        return selectedIndex;
    }

    public void setSelectedIndex(int value)
    {
        selectedIndex = options.isEmpty() ? 0 : clamp(value, 0, options.size() - 1);
    }

    public String selectedText()
    {
        return selectedIndex >= 0 && selectedIndex < options.size() ? options.get(selectedIndex) : "";
    }

    @Override
    public boolean handleKey(String key)
    {
        if (options.isEmpty())
        {
            return false;
        }
        if ("Down".equals(key))
        {
            setSelectedIndex(selectedIndex + 1);
            return true;
        }
        if ("Up".equals(key))
        {
            setSelectedIndex(selectedIndex - 1);
            return true;
        }
        return false;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        int width = Math.max(1, size.width());
        Style baseStyle = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        List<List<TerminalCell>> rendered = new ArrayList<>();
        StringBuilder closed = new StringBuilder(RenderHelpers.align(selectedText(), width, "left"));
        if (effectiveState.focused() || effectiveState.editMode())
        {
            closed.setCharAt(width - 1, effectiveState.editMode() ? 'v' : '>');
        }
        rendered.add(RenderHelpers.renderPlainText(closed.toString(), width, 1, baseStyle).get(0));
        if (effectiveState.editMode())
        {
            for (int index = 0; index < options.size(); ++index)
            {
                Style rowStyle = baseStyle.copy();
                if (index == selectedIndex && selectedStyle() != null)
                {
                    rowStyle.merge(selectedStyle());
                }
                rendered.add(RenderHelpers.renderPlainText("  " + options.get(index), width, 1, rowStyle).get(0));
            }
        }
        return rendered;
    }

    private static int clamp(int value, int minimum, int maximum)
    {
        return Math.max(minimum, Math.min(maximum, value));
    }
}
