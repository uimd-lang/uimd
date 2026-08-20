package uimd;

import java.util.List;

public final class Button extends Element
{
    private static final int COMFORTABLE_FOCUS_DECORATION_WIDTH = 6;
    private static final int COMPACT_FOCUS_DECORATION_WIDTH = 4;
    private static final int MINIMUM_FOCUS_DECORATION_WIDTH = 2;
    private String title;

    public Button(String name)
    {
        this(name, "Button");
    }

    public Button(String name, String title)
    {
        super(name);
        this.title = title == null ? "" : title;
    }

    public String title()
    {
        return title;
    }

    public void setTitle(String value)
    {
        title = value == null ? "" : value;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        Style style = effectiveStyle(effectiveState.focused(), effectiveState.editMode());
        String alignment = style.textAlign().isEmpty() ? "center" : style.textAlign();
        int width = Math.max(1, size.width());
        String body = title;
        if (effectiveState.focused() && width >= title.length() + MINIMUM_FOCUS_DECORATION_WIDTH)
        {
            if (width >= title.length() + COMFORTABLE_FOCUS_DECORATION_WIDTH)
            {
                int innerWidth = Math.max(1, width - COMPACT_FOCUS_DECORATION_WIDTH);
                body = " [" + RenderHelpers.align(" " + title + " ", innerWidth, alignment) + "] ";
            }
            else if (width >= title.length() + COMPACT_FOCUS_DECORATION_WIDTH)
            {
                int innerWidth = Math.max(1, width - COMPACT_FOCUS_DECORATION_WIDTH);
                body = " [" + RenderHelpers.align(title, innerWidth, alignment) + "] ";
            }
            else
            {
                body = "[" + title + "]";
            }
        }
        return RenderHelpers.renderPlainText(RenderHelpers.align(body, width, alignment), width, 1, style);
    }
}
