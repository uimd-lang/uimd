package uimd;

import java.util.List;

public final class CheckBox extends Element
{
    private static final int INDICATOR_WIDTH = 3;

    private String title;
    private boolean checked;

    public CheckBox(String name)
    {
        this(name, "", false);
    }

    public CheckBox(String name, String title, boolean checked)
    {
        super(name);
        this.title = title == null ? "" : title;
        this.checked = checked;
    }

    public String title()
    {
        return title;
    }

    public void setTitle(String value)
    {
        title = value == null ? "" : value;
    }

    public boolean checked()
    {
        return checked;
    }

    public void setChecked(boolean value)
    {
        checked = value;
    }

    @Override
    public boolean handleKey(String key)
    {
        if (!"Enter".equals(key) && !" ".equals(key))
        {
            return false;
        }
        checked = !checked;
        return true;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        String indicator = checked ? "[x]" : "[ ]";
        String text = title.isEmpty() ? indicator : indicator + " " + title;
        int width = Math.max(1, size.width());
        Style baseStyle = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        List<List<TerminalCell>> rendered = RenderHelpers.renderPlainText(text, width, 1, baseStyle);
        Style indicatorStyle = baseStyle.copy();
        Color inheritedBackground = indicatorStyle.background();
        Style stateStyle = checked ? checkedStyle() : uncheckedStyle();
        if (stateStyle != null)
        {
            indicatorStyle.merge(stateStyle);
        }
        if ((effectiveState.focused() || effectiveState.editMode())
            && indicatorStyle.background() != null
            && indicatorStyle.background().isTransparent()
            && inheritedBackground != null)
        {
            indicatorStyle.setBackground(inheritedBackground);
        }
        for (int index = 0; index < Math.min(INDICATOR_WIDTH, width); ++index)
        {
            rendered.get(0).get(index).setForeground(indicatorStyle.color());
            rendered.get(0).get(index).setBackground(indicatorStyle.background());
        }
        return rendered;
    }
}
