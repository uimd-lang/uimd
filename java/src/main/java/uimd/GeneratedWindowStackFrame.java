package uimd;

import java.util.IdentityHashMap;
import java.util.Map;

public final class GeneratedWindowStackFrame
{
    private final GeneratedWindowBase window;
    private final GeneratedWindowRuntimeOptions options;
    private int focusedIndex;
    private boolean editMode;
    private ScrollView activeScrollView;
    private Element activeScrollViewEditElement;
    private boolean suppressActiveScrollViewScopeVisuals;
    private GeneratedWindowRuntime.EditSnapshot editSnapshot;
    private final Map<ScrollView, Element> scrollViewLastDescendant = new IdentityHashMap<>();

    GeneratedWindowStackFrame(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        int focusedIndex,
        boolean editMode)
    {
        this.window = window;
        this.options = options;
        this.focusedIndex = focusedIndex;
        this.editMode = editMode;
    }

    public GeneratedWindowBase window()
    {
        return window;
    }

    public GeneratedWindowRuntimeOptions options()
    {
        return options;
    }

    public int focusedIndex()
    {
        return focusedIndex;
    }

    void setFocusedIndex(int value)
    {
        focusedIndex = value;
    }

    public boolean editMode()
    {
        return editMode;
    }

    void setEditMode(boolean value)
    {
        editMode = value;
    }

    public ScrollView activeScrollView()
    {
        return activeScrollView;
    }

    void setActiveScrollView(ScrollView value)
    {
        activeScrollView = value;
    }

    public Element activeScrollViewEditElement()
    {
        return activeScrollViewEditElement;
    }

    void setActiveScrollViewEditElement(Element value)
    {
        activeScrollViewEditElement = value;
    }

    public boolean suppressActiveScrollViewScopeVisuals()
    {
        return suppressActiveScrollViewScopeVisuals;
    }

    void setSuppressActiveScrollViewScopeVisuals(boolean value)
    {
        suppressActiveScrollViewScopeVisuals = value;
    }

    GeneratedWindowRuntime.EditSnapshot editSnapshot()
    {
        return editSnapshot;
    }

    void setEditSnapshot(GeneratedWindowRuntime.EditSnapshot value)
    {
        editSnapshot = value;
    }

    Map<ScrollView, Element> scrollViewLastDescendant()
    {
        return scrollViewLastDescendant;
    }
}
