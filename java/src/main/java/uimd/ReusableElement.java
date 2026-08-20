package uimd;

import java.util.List;

public class ReusableElement extends Element
{
    private GeneratedWindowBase child;
    private final String childType;

    public ReusableElement(String name)
    {
        this(name, "");
    }

    public ReusableElement(String name, String childType)
    {
        super(name);
        this.childType = childType == null ? "" : childType;
    }

    public ReusableElement(String name, GeneratedWindowBase child)
    {
        this(name);
        setChild(child);
    }

    public String childType()
    {
        return childType;
    }

    public GeneratedWindowBase child()
    {
        return child;
    }

    public void setChild(GeneratedWindowBase value)
    {
        child = value;
        if (child != null)
        {
            child.setMode("fullscreen");
        }
    }

    @Override
    public boolean handleKey(String key)
    {
        if (child instanceof GeneratedScrollViewBase scrollView)
        {
            return scrollView.scrollView().handleKey(key);
        }
        return false;
    }

    @Override
    public List<List<TerminalCell>> render(Size size, ElementRenderState state)
    {
        if (child == null)
        {
            return super.render(size, state);
        }
        ElementRenderState effectiveState = state == null ? new ElementRenderState() : state;
        Element focused = effectiveState.focusedElement();
        ScrollView activeScrollView = effectiveState.activeScrollView();
        Element activeEditElement = effectiveState.activeScrollViewEditElement();
        List<Element> focusable = GeneratedWindowRuntime.focusableElements(child, activeScrollView);
        int focusedIndex = focusable.indexOf(focused);
        if (focusedIndex < 0
            && activeEditElement != null
            && GeneratedWindowRuntime.windowContainsElement(child, activeEditElement))
        {
            focusedIndex = focusable.indexOf(activeEditElement);
        }
        if (focusedIndex < 0
            && activeScrollView != null
            && GeneratedWindowRuntime.windowContainsElement(child, activeScrollView))
        {
            focusedIndex = focusable.indexOf(activeScrollView);
        }

        boolean childOwnsActiveScrollView = activeScrollView != null
            && GeneratedWindowRuntime.windowContainsElement(child, activeScrollView);
        boolean childOwnsActiveEditElement = activeEditElement != null
            && GeneratedWindowRuntime.windowContainsElement(child, activeEditElement);
        boolean childHasFocusedElement = focused != null
            && GeneratedWindowRuntime.windowContainsElement(child, focused);
        boolean directFocus = effectiveState.focused()
            && !effectiveState.suppressActiveScrollViewScopeVisuals();
        boolean navigationScopeDescendantFocused =
            !effectiveState.suppressActiveScrollViewScopeVisuals()
            && !effectiveState.editMode()
            && childOwnsActiveScrollView
            && childHasFocusedElement;
        boolean reusableDescendantFocused = directFocus
            || navigationScopeDescendantFocused
            || (effectiveState.editMode() && childOwnsActiveScrollView)
            || (effectiveState.editMode() && childOwnsActiveEditElement)
            || (effectiveState.editMode() && childHasFocusedElement);
        boolean childEditMode = focusedIndex >= 0
            && effectiveState.editMode()
            && (activeScrollView == null
                || childOwnsActiveScrollView
                || childOwnsActiveEditElement);
        ScrollView generatedScrollView = child.generatedScrollView();
        boolean applyFocusStyle = reusableFocusStyleAppliesToChild(
            directFocus,
            reusableDescendantFocused && !directFocus);
        Style childWindowStyle = null;
        Color focusBackground = null;
        if (reusableDescendantFocused && applyFocusStyle)
        {
            childWindowStyle = child.generatedWindowStyle().copy();
            if (generatedScrollView == null)
            {
                childWindowStyle.merge(focusStyle());
            }
            focusBackground = focusStyle().background();
        }
        if (reusableDescendantFocused
            && focusBackground == null
            && !childOwnsActiveScrollView
            && generatedScrollView != null)
        {
            Style scrollFocusStyle = generatedScrollView.descendantFocusStyle() != null
                ? generatedScrollView.descendantFocusStyle()
                : generatedScrollView.focusStyle();
            if (scrollFocusStyle != null)
            {
                focusBackground = scrollFocusStyle.background();
            }
        }

        List<List<TerminalCell>> rendered = GeneratedWindowRuntime.renderGeneratedWindowContent(
            child,
            size,
            focusedIndex,
            childEditMode,
            activeScrollView,
            activeEditElement,
            childWindowStyle,
            focusBackground,
            effectiveState.clipTop(),
            effectiveState.clipBottom(),
            false,
            true,
            this instanceof ViewHost && generatedScrollView != null,
            effectiveState.suppressActiveScrollViewScopeVisuals());
        if (focusBackground != null && generatedScrollView == null)
        {
            List<Color> descendantBackgrounds = new java.util.ArrayList<>();
            collectDescendantBaseStyleBackgrounds(child, descendantBackgrounds);
            applyFocusBackgroundToDescendantBackgrounds(
                rendered,
                focusBackground,
                descendantBackgrounds);
            Color baseBackground = style().background() == null
                ? Element.renderingParentBackground()
                : style().background();
            applyReusableFocusBackground(rendered, focusBackground, baseBackground);
        }
        return rendered;
    }

    private boolean reusableFocusStyleAppliesToChild(
        boolean directFocus,
        boolean descendantOnlyFocus)
    {
        if (focusStyle() == null)
        {
            return false;
        }
        Color background = focusStyle().background();
        if (!hasPartialAlpha(background))
        {
            return true;
        }
        if (child != null && child.generatedScrollView() != null)
        {
            return false;
        }
        return directFocus || !descendantOnlyFocus;
    }

    private static void collectDescendantBaseStyleBackgrounds(
        GeneratedWindowBase window,
        List<Color> backgrounds)
    {
        for (Element element : window.elements())
        {
            appendOpaqueBackground(backgrounds, element.style().background());
            if (element.focusStyle() != null)
            {
                appendOpaqueBackground(backgrounds, element.focusStyle().background());
            }
            if (element.editStyle() != null)
            {
                appendOpaqueBackground(backgrounds, element.editStyle().background());
            }
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                collectDescendantBaseStyleBackgrounds(reusable.child(), backgrounds);
            }
            if (element instanceof ScrollView scrollView)
            {
                for (Element childElement : scrollView.children())
                {
                    collectDescendantBaseStyleBackgrounds(childElement, backgrounds);
                }
            }
        }
    }

    private static void collectDescendantBaseStyleBackgrounds(
        Element element,
        List<Color> backgrounds)
    {
        appendOpaqueBackground(backgrounds, element.style().background());
        if (element.focusStyle() != null)
        {
            appendOpaqueBackground(backgrounds, element.focusStyle().background());
        }
        if (element.editStyle() != null)
        {
            appendOpaqueBackground(backgrounds, element.editStyle().background());
        }
        if (element instanceof ReusableElement reusable && reusable.child() != null)
        {
            collectDescendantBaseStyleBackgrounds(reusable.child(), backgrounds);
        }
        if (element instanceof ScrollView scrollView)
        {
            for (Element childElement : scrollView.children())
            {
                collectDescendantBaseStyleBackgrounds(childElement, backgrounds);
            }
        }
    }

    private static void appendOpaqueBackground(List<Color> backgrounds, Color background)
    {
        if (!isOpaque(background) || backgrounds.contains(background))
        {
            return;
        }
        backgrounds.add(background);
    }

    private static void applyFocusBackgroundToDescendantBackgrounds(
        List<List<TerminalCell>> content,
        Color focusBackground,
        List<Color> backgrounds)
    {
        if (backgrounds.isEmpty())
        {
            return;
        }
        for (List<TerminalCell> row : content)
        {
            for (TerminalCell cell : row)
            {
                Color cellBackground = cell.background();
                if (cellBackground == null)
                {
                    continue;
                }
                for (Color background : backgrounds)
                {
                    Color focusedBackground = blendOverExactAlpha(focusBackground, background);
                    if (cellBackground.equals(background)
                        || cellBackground.equals(focusedBackground))
                    {
                        cell.setBackground(focusedBackground);
                        break;
                    }
                }
            }
        }
    }

    private static void applyReusableFocusBackground(
        List<List<TerminalCell>> content,
        Color focusBackground,
        Color baseBackground)
    {
        Color focusedBaseBackground = baseBackground == null
            ? null
            : blendOverExactAlpha(focusBackground, baseBackground);
        for (List<TerminalCell> row : content)
        {
            for (TerminalCell cell : row)
            {
                Color cellBackground = cell.background();
                boolean matchesUnresolvedFocus = focusedBaseBackground != null
                    && focusBackground.equals(cellBackground);
                if (matchesUnresolvedFocus)
                {
                    cell.setBackground(blendOverExactAlpha(
                        focusBackground,
                        focusedBaseBackground));
                    continue;
                }
                boolean matchesBase = cellBackground == null
                    || (baseBackground != null && baseBackground.equals(cellBackground));
                boolean matchesFocusedBase = focusedBaseBackground != null
                    && focusedBaseBackground.equals(cellBackground);
                if ((!matchesBase && !matchesFocusedBase) || matchesFocusedBase)
                {
                    continue;
                }
                cell.setBackground(cellBackground == null
                    ? focusBackground
                    : blendOverExactAlpha(focusBackground, cellBackground));
            }
        }
    }

    private static Color blendOverExactAlpha(Color foreground, Color background)
    {
        if (foreground == null || background == null)
        {
            return foreground;
        }
        return foreground.blendOver(background);
    }

    private static boolean hasPartialAlpha(Color color)
    {
        return color != null
            && color.rgba().isPresent()
            && color.rgba().orElseThrow().alpha() < 255;
    }

    private static boolean isOpaque(Color color)
    {
        return color != null
            && color.rgba().isPresent()
            && color.rgba().orElseThrow().alpha() == 255;
    }
}
