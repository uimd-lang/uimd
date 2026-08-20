package uimd;

public final class ElementRenderState
{
    private boolean focused;
    private boolean editMode;
    private boolean passiveFocus;
    private Element focusedElement;
    private ScrollView activeScrollView;
    private Element activeScrollViewEditElement;
    private boolean suppressActiveScrollViewScopeVisuals;
    private Integer clipTop;
    private Integer clipBottom;

    public boolean focused()
    {
        return focused;
    }

    public void setFocused(boolean value)
    {
        focused = value;
    }

    public boolean editMode()
    {
        return editMode;
    }

    public void setEditMode(boolean value)
    {
        editMode = value;
    }

    public boolean passiveFocus()
    {
        return passiveFocus;
    }

    public void setPassiveFocus(boolean value)
    {
        passiveFocus = value;
    }

    public Element focusedElement()
    {
        return focusedElement;
    }

    public void setFocusedElement(Element value)
    {
        focusedElement = value;
    }

    public ScrollView activeScrollView()
    {
        return activeScrollView;
    }

    public void setActiveScrollView(ScrollView value)
    {
        activeScrollView = value;
    }

    public Element activeScrollViewEditElement()
    {
        return activeScrollViewEditElement;
    }

    public void setActiveScrollViewEditElement(Element value)
    {
        activeScrollViewEditElement = value;
    }

    public boolean suppressActiveScrollViewScopeVisuals()
    {
        return suppressActiveScrollViewScopeVisuals;
    }

    public void setSuppressActiveScrollViewScopeVisuals(boolean value)
    {
        suppressActiveScrollViewScopeVisuals = value;
    }

    public Integer clipTop()
    {
        return clipTop;
    }

    public void setClipTop(Integer value)
    {
        clipTop = value;
    }

    public Integer clipBottom()
    {
        return clipBottom;
    }

    public void setClipBottom(Integer value)
    {
        clipBottom = value;
    }
}
