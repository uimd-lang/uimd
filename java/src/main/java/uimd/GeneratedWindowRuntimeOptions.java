package uimd;

import java.util.function.BooleanSupplier;
import java.util.function.BiConsumer;
import java.util.function.BiPredicate;
import java.util.function.Consumer;
import java.util.function.Predicate;

public final class GeneratedWindowRuntimeOptions
{
    private String className = "";
    private String footer = "";
    private String initialFocusName = "";
    private boolean startInEditMode;
    private boolean dimBackground = true;
    private boolean keepEditModeAfterConfirm;
    private boolean keepEditModeAfterEscape;
    private GeneratedWindowStack windowStack;
    private Consumer<String> onButton;
    private GeneratedFocusedKeyHandler onKeyBeforeFocusedElement;
    private Predicate<String> onKeyBeforeFocused;
    private Predicate<String> onKey;
    private Predicate<Point> onMousePressBeforeFocused;
    private BiPredicate<Point, Integer> onMouseWheelBeforeFocused;
    private BiPredicate<String, Integer> onMouseWheel;
    private BiConsumer<String, String> onTextChanged;
    private BiConsumer<String, String> onTextConfirmed;
    private BiConsumer<String, java.util.List<String>> onSelectionChanged;
    private Consumer<String> onEditStarted;
    private BiConsumer<String, Boolean> onFocusChanged;
    private Consumer<TerminalBuffer> onRenderOverlay;
    private BooleanSupplier shouldDimOverlayBackground;
    private BooleanSupplier shouldClose;

    public String className()
    {
        return className;
    }

    public void setClassName(String value)
    {
        className = value == null ? "" : value;
    }

    public String footer()
    {
        return footer;
    }

    public void setFooter(String value)
    {
        footer = value == null ? "" : value;
    }

    public String initialFocusName()
    {
        return initialFocusName;
    }

    public void setInitialFocusName(String value)
    {
        initialFocusName = value == null ? "" : value;
    }

    public boolean startInEditMode()
    {
        return startInEditMode;
    }

    public void setStartInEditMode(boolean value)
    {
        startInEditMode = value;
    }

    public boolean keepEditModeAfterConfirm()
    {
        return keepEditModeAfterConfirm;
    }

    public boolean dimBackground()
    {
        return dimBackground;
    }

    public void setDimBackground(boolean value)
    {
        dimBackground = value;
    }

    public void setKeepEditModeAfterConfirm(boolean value)
    {
        keepEditModeAfterConfirm = value;
    }

    public boolean keepEditModeAfterEscape()
    {
        return keepEditModeAfterEscape;
    }

    public GeneratedWindowStack windowStack()
    {
        return windowStack;
    }

    public void setWindowStack(GeneratedWindowStack value)
    {
        windowStack = value;
    }

    public void setKeepEditModeAfterEscape(boolean value)
    {
        keepEditModeAfterEscape = value;
    }

    public Predicate<String> onKey()
    {
        return onKey;
    }

    public GeneratedFocusedKeyHandler onKeyBeforeFocusedElement()
    {
        return onKeyBeforeFocusedElement;
    }

    public void setOnKeyBeforeFocusedElement(GeneratedFocusedKeyHandler value)
    {
        onKeyBeforeFocusedElement = value;
    }

    public Predicate<String> onKeyBeforeFocused()
    {
        return onKeyBeforeFocused;
    }

    public void setOnKeyBeforeFocused(Predicate<String> value)
    {
        onKeyBeforeFocused = value;
    }

    public Consumer<String> onButton()
    {
        return onButton;
    }

    public void setOnButton(Consumer<String> value)
    {
        onButton = value;
    }

    public void setOnKey(Predicate<String> value)
    {
        onKey = value;
    }

    public Predicate<Point> onMousePressBeforeFocused()
    {
        return onMousePressBeforeFocused;
    }

    public void setOnMousePressBeforeFocused(Predicate<Point> value)
    {
        onMousePressBeforeFocused = value;
    }

    public BiPredicate<Point, Integer> onMouseWheelBeforeFocused()
    {
        return onMouseWheelBeforeFocused;
    }

    public void setOnMouseWheelBeforeFocused(BiPredicate<Point, Integer> value)
    {
        onMouseWheelBeforeFocused = value;
    }

    public BiPredicate<String, Integer> onMouseWheel()
    {
        return onMouseWheel;
    }

    public void setOnMouseWheel(BiPredicate<String, Integer> value)
    {
        onMouseWheel = value;
    }

    public BiConsumer<String, String> onTextChanged()
    {
        return onTextChanged;
    }

    public void setOnTextChanged(BiConsumer<String, String> value)
    {
        onTextChanged = value;
    }

    public BiConsumer<String, String> onTextConfirmed()
    {
        return onTextConfirmed;
    }

    public void setOnTextConfirmed(BiConsumer<String, String> value)
    {
        onTextConfirmed = value;
    }

    public BiConsumer<String, java.util.List<String>> onSelectionChanged()
    {
        return onSelectionChanged;
    }

    public void setOnSelectionChanged(BiConsumer<String, java.util.List<String>> value)
    {
        onSelectionChanged = value;
    }

    public Consumer<String> onEditStarted()
    {
        return onEditStarted;
    }

    public void setOnEditStarted(Consumer<String> value)
    {
        onEditStarted = value;
    }

    public BiConsumer<String, Boolean> onFocusChanged()
    {
        return onFocusChanged;
    }

    public void setOnFocusChanged(BiConsumer<String, Boolean> value)
    {
        onFocusChanged = value;
    }

    public Consumer<TerminalBuffer> onRenderOverlay()
    {
        return onRenderOverlay;
    }

    public void setOnRenderOverlay(Consumer<TerminalBuffer> value)
    {
        onRenderOverlay = value;
    }

    public BooleanSupplier shouldDimOverlayBackground()
    {
        return shouldDimOverlayBackground;
    }

    public void setShouldDimOverlayBackground(BooleanSupplier value)
    {
        shouldDimOverlayBackground = value;
    }

    public BooleanSupplier shouldClose()
    {
        return shouldClose;
    }

    public void setShouldClose(BooleanSupplier value)
    {
        shouldClose = value;
    }
}
