package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicLong;

public class Element
{
    public static final String COMMIT_MODE_STANDARD = "standard";
    public static final String COMMIT_MODE_LEAVE = "leave";

    private static final ThreadLocal<Color> RENDERING_PARENT_BACKGROUND = new ThreadLocal<>();
    private static final AtomicLong NEXT_IDENTITY = new AtomicLong(1);

    private final String name;
    private final long identity;
    private String commitMode = COMMIT_MODE_STANDARD;
    private boolean focusable = true;
    private boolean enabled = true;
    private Rect frame = new Rect(0, 0, 0, 0);
    private Style style = new Style();
    private Style focusStyle;
    private Style editStyle;
    private Style cursorStyle;
    private Style selectedStyle;
    private Style activeStyle;
    private Style checkedStyle;
    private Style uncheckedStyle;
    private Style disabledStyle;
    private Element parent;
    private final List<Element> children = new ArrayList<>();

    public Element(String name)
    {
        this.name = name == null ? "" : name;
        identity = NEXT_IDENTITY.getAndIncrement();
    }

    public String name()
    {
        return name;
    }

    public long identity()
    {
        return identity;
    }

    public String commitMode()
    {
        return commitMode;
    }

    public void setCommitMode(String value)
    {
        commitMode = value == null ? COMMIT_MODE_STANDARD : value;
    }

    public boolean focusable()
    {
        return focusable;
    }

    public void setFocusable(boolean value)
    {
        focusable = value;
    }

    public boolean enabled()
    {
        return enabled;
    }

    public void setEnabled(boolean value)
    {
        enabled = value;
    }

    public Rect frame()
    {
        return frame;
    }

    public void setFrame(Rect value)
    {
        frame = value == null ? new Rect(0, 0, 0, 0) : value;
    }

    public Style style()
    {
        return style;
    }

    public void setStyle(Style value)
    {
        style = value == null ? new Style() : value;
    }

    public Style focusStyle()
    {
        return focusStyle;
    }

    public void setFocusStyle(Style value)
    {
        focusStyle = value;
    }

    public Style editStyle()
    {
        return editStyle;
    }

    public void setEditStyle(Style value)
    {
        editStyle = value;
    }

    public Style cursorStyle()
    {
        return cursorStyle;
    }

    public void setCursorStyle(Style value)
    {
        cursorStyle = value;
    }

    public Style selectedStyle()
    {
        return selectedStyle;
    }

    public void setSelectedStyle(Style value)
    {
        selectedStyle = value;
    }

    public Style activeStyle()
    {
        return activeStyle;
    }

    public void setActiveStyle(Style value)
    {
        activeStyle = value;
    }

    public Style checkedStyle()
    {
        return checkedStyle;
    }

    public void setCheckedStyle(Style value)
    {
        checkedStyle = value;
    }

    public Style uncheckedStyle()
    {
        return uncheckedStyle;
    }

    public void setUncheckedStyle(Style value)
    {
        uncheckedStyle = value;
    }

    public Style disabledStyle()
    {
        return disabledStyle;
    }

    public void setDisabledStyle(Style value)
    {
        disabledStyle = value;
    }

    public List<Element> children()
    {
        return Collections.unmodifiableList(children);
    }

    public Element parent()
    {
        return parent;
    }

    public <T extends Element> T addChild(T child)
    {
        if (child == null)
        {
            throw new IllegalArgumentException("child element must not be null");
        }
        ((Element) child).setParent(this);
        children.add(child);
        return child;
    }

    public void clearChildElements()
    {
        for (Element child : children)
        {
            child.parent = null;
        }
        children.clear();
    }

    public Style effectiveStyle(boolean focused, boolean editMode)
    {
        Style result = style.copy();
        if (!enabled && disabledStyle != null)
        {
            return mergedStateStyle(result, disabledStyle);
        }
        if (editMode && editStyle != null)
        {
            return mergedStateStyle(result, editStyle);
        }
        if (focused && focusStyle != null)
        {
            return mergedStateStyle(result, focusStyle);
        }
        return result;
    }

    public boolean handleKey(String key)
    {
        return false;
    }

    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        return RenderHelpers.renderPlainText(
            name,
            size.width(),
            size.height(),
            effectiveStyle(effectiveState.focused(), effectiveState.editMode()));
    }

    static Color renderingParentBackground()
    {
        return RENDERING_PARENT_BACKGROUND.get();
    }

    private void setParent(Element value)
    {
        parent = value;
    }

    static void setRenderingParentBackground(Color value)
    {
        RENDERING_PARENT_BACKGROUND.set(value);
    }

    private static boolean hasPartialAlpha(Color color)
    {
        return color != null && color.rgba().isPresent() && color.rgba().orElseThrow().alpha() < 255;
    }

    private static Style mergedStateStyle(Style baseStyle, Style state)
    {
        Color baseBackground = baseStyle.background();
        Color stateBackground = state.background();
        Color parentBackground = renderingParentBackground();
        if (hasPartialAlpha(baseBackground) && parentBackground != null && parentBackground.rgba().isPresent())
        {
            baseStyle.setBackground(baseBackground.blendOver(parentBackground));
            baseBackground = baseStyle.background();
        }
        baseStyle.merge(state);
        if (hasPartialAlpha(stateBackground) && baseBackground != null && baseBackground.rgba().isPresent())
        {
            if (stateBackground.rgba().orElseThrow().alpha() == 0)
            {
                baseStyle.setBackground(baseBackground);
            }
            else
            {
                baseStyle.setBackground(stateBackground.blendOver(baseBackground));
            }
        }
        return baseStyle;
    }
}
