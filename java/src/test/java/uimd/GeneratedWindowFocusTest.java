package uimd;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.jupiter.api.Test;


final class GeneratedWindowFocusTest
{
    private static final Size CONTENT_SIZE = new Size(12, 3);

    @Test
    void reusableScrollScopeRoutesOwnedEventsAndRestoresRememberedDescendant()
    {
        FocusFixture fixture = new FocusFixture();
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowRuntimeOptions options = fixture.root.runtimeOptions();
        options.setInitialFocusName("host");
        GeneratedWindowStackFrame frame = stack.push(fixture.root, options);
        renderFrame(fixture.root, frame);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        assertSame(fixture.scrollWindow.scrollView(), frame.activeScrollView());
        assertTrue(frame.editMode());
        assertSame(fixture.first.button, focusedElement(fixture.root, frame));

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        assertEquals(1, fixture.first.buttonActivations);
        assertTrue(frame.editMode());

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Down"));
        assertSame(fixture.first.input, focusedElement(fixture.root, frame));
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        assertSame(fixture.first.input, frame.activeScrollViewEditElement());

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("x"));
        assertEquals("seedx", fixture.first.input.value());
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Escape"));
        assertEquals("seed", fixture.first.input.value());
        assertNull(frame.activeScrollViewEditElement());
        assertNotNull(frame.activeScrollView());
        assertTrue(frame.editMode());

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Escape"));
        assertNull(frame.activeScrollView());
        assertSame(fixture.host, focusedElement(fixture.root, frame));

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        assertSame(fixture.first.input, focusedElement(fixture.root, frame));
    }

    @Test
    void keyboardFocusMovementNotifiesOnlyActualPreviousAndNextElements()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Focus callbacks");
        window.addElement(new Button("first", "First"));
        window.addElement(new Button("second", "Second"));
        window.setGeneratedLayout(List.of(
            layout("first", 0, 1),
            layout("second", 2, 1)));
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setInitialFocusName("first");
        List<String> changes = new ArrayList<>();
        options.setOnFocusChanged((name, focused) -> changes.add(name + ":" + focused));
        GeneratedWindowStackFrame frame = GeneratedWindowRuntime.createFrame(window, options);

        GeneratedWindowRuntime.dispatchFrameEvent(frame, Event.key("Tab"));
        assertEquals(List.of("first:false", "second:true"), changes);
        changes.clear();
        GeneratedWindowRuntime.dispatchFrameEvent(frame, Event.key("Right"));
        assertTrue(changes.isEmpty());
    }

    @Test
    void scrollViewScopedReusableControlActivatesWithEnterAndSpace()
    {
        ActivatingControlWindow actionWindow = new ActivatingControlWindow();
        ReusableElement action = new ReusableElement("action", actionWindow);
        TestScrollWindow scrollWindow = new TestScrollWindow();
        scrollWindow.scrollView().addChild(action);
        ReusableElement host = new ReusableElement("items", scrollWindow);
        GeneratedWindowBase root = new GeneratedWindowBase("Root");
        root.setGeneratedLayout(List.of(layout("items", 0, CONTENT_SIZE.height())));
        root.addElement(host);
        GeneratedWindowRuntimeOptions options = root.runtimeOptions();
        options.setInitialFocusName("items");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(root, options);
        renderFrame(root, frame);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        assertSame(action, focusedElement(root, frame));
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        assertEquals(1, actionWindow.activationCount);
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key(" "));
        assertEquals(2, actionWindow.activationCount);
    }

    @Test
    void explicitDescendantFocusKeepsTheScrollScopeButEndsItsEditVisuals()
    {
        FocusFixture fixture = new FocusFixture();
        GeneratedWindowRuntimeOptions options = fixture.root.runtimeOptions();
        options.setInitialFocusName("host");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(fixture.root, options);
        renderFrame(fixture.root, frame);
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));

        GeneratedWindowRuntime.focusFrameElement(frame, fixture.second.button);

        assertSame(fixture.scrollWindow.scrollView(), frame.activeScrollView());
        assertSame(fixture.second.button, focusedElement(fixture.root, frame));
        assertNull(frame.activeScrollViewEditElement());
        assertFalse(frame.editMode());
    }

    @Test
    void wheelTargetsGeneratedScrollViewThroughReusableHost()
    {
        FocusFixture fixture = new FocusFixture();
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowRuntimeOptions options = fixture.root.runtimeOptions();
        options.setInitialFocusName("host");
        GeneratedWindowStackFrame frame = stack.push(fixture.root, options);
        renderFrame(fixture.root, frame);
        assertEquals(1, fixture.scrollWindow.scrollView().viewOffset());

        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.wheel(new Point(1, 1), -1));

        assertEquals(0, fixture.scrollWindow.scrollView().viewOffset());
    }

    @Test
    void applicationWheelRunsAfterTheSharedUnhandledControlPath()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Wheel callback");
        window.addElement(new Button("surface", "Surface"));
        window.setGeneratedLayout(List.of(layout("surface", 0, 1)));
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        List<String> callbacks = new ArrayList<>();
        options.setOnMouseWheelBeforeFocused((position, delta) ->
        {
            callbacks.add("before:" + delta);
            return false;
        });
        options.setOnMouseWheel((name, delta) ->
        {
            callbacks.add(name + ":" + delta);
            return true;
        });
        GeneratedWindowStackFrame frame = GeneratedWindowRuntime.createFrame(window, options);
        renderFrame(window, frame);

        GeneratedWindowRuntime.dispatchFrameEvent(
            frame,
            Event.wheel(new Point(0, 0), -2));

        assertEquals(List.of("before:-2", "surface:-2"), callbacks);
    }

    @Test
    void handledListBoxWheelDoesNotReachTheApplicationCallback()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Wheel control");
        ListBox list = window.addElement(new ListBox(
            "list",
            List.of("One", "Two", "Three", "Four"),
            false));
        window.setGeneratedLayout(List.of(layout("list", 0, 1)));
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        AtomicInteger applicationCallbacks = new AtomicInteger();
        options.setOnMouseWheel((name, delta) ->
        {
            applicationCallbacks.incrementAndGet();
            return true;
        });
        GeneratedWindowStackFrame frame = GeneratedWindowRuntime.createFrame(window, options);
        renderFrame(window, frame);

        GeneratedWindowRuntime.dispatchFrameEvent(
            frame,
            Event.wheel(new Point(list.frame().row(), list.frame().col()), -1));

        assertEquals(1, list.scrollOffset());
        assertEquals(0, applicationCallbacks.get());
    }

    @Test
    void generatedScrollViewDelegatesItsCompletePublicSurface()
    {
        TestScrollWindow window = new TestScrollWindow();
        ScrollView view = window.scrollView();
        Rect frame = new Rect(2, 3, 4, 2);
        view.setFrame(frame);
        view.addChild(List.of(
            List.of(new TerminalCell()),
            List.of(new TerminalCell()),
            List.of(new TerminalCell())));

        assertEquals(frame, window.frame());
        assertEquals(view.contentHeight(), window.contentHeight());
        assertEquals(view.contentHeight(frame.width()), window.contentHeight(frame.width()));
        assertEquals(view.scrollOffset(), window.scrollOffset());
        assertEquals(view.viewOffset(), window.viewOffset());
        assertTrue(window.scrollToTop());
        assertEquals(view.viewOffset(), window.viewOffset());
        assertTrue(window.handleKey("Down"));
        assertTrue(window.scrollToTop(new Size(frame.width(), 1)));
        assertTrue(window.scrollToBottom(new Size(frame.width(), 1)));
        ScrollViewPosition position = window.scrollPosition();
        window.restoreScrollPosition(position);
        assertEquals(position, window.scrollPosition());
    }

    @Test
    void mouseActivationEntersAndRetainsNestedScrollScope()
    {
        FocusFixture fixture = new FocusFixture();
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowRuntimeOptions options = fixture.root.runtimeOptions();
        options.setInitialFocusName("host");
        GeneratedWindowStackFrame frame = stack.push(fixture.root, options);
        renderFrame(fixture.root, frame);

        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.mouse(
                EventType.MOUSE_PRESS,
                new Point(fixture.first.button.frame().row(), fixture.first.button.frame().col())));

        assertEquals(1, fixture.first.buttonActivations);
        assertSame(fixture.scrollWindow.scrollView(), frame.activeScrollView());
        assertSame(fixture.first.button, focusedElement(fixture.root, frame));
        assertTrue(frame.editMode());
    }

    @Test
    void mouseFocusChangeCommitsAnEditedNumberInput()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Mouse commit");
        NumberInput amount = window.addElement(new NumberInput("amount", 0.0, 1.0));
        CheckBox enabled = window.addElement(new CheckBox("enabled", "Enabled", false));
        window.setGeneratedLayout(List.of(
            layout("amount", 0, 1),
            layout("enabled", 2, 1)));
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setInitialFocusName("amount");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window, options);
        renderFrame(window, frame);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("7"));
        assertEquals(0.0, amount.value());

        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.mouse(
                EventType.MOUSE_PRESS,
                new Point(enabled.frame().row(), enabled.frame().col())));

        assertEquals(7.0, amount.value());
        assertSame(enabled, focusedElement(window, frame));
        assertTrue(enabled.checked());
    }

    @Test
    void blankMouseTargetCommitsEditAndKeepsFocusCleared()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Blank mouse target");
        NumberInput amount = window.addElement(new NumberInput("amount", 0.0, 1.0));
        window.addElement(new CheckBox("enabled", "Enabled", false));
        window.setGeneratedLayout(List.of(
            layout("amount", 0, 1),
            layout("enabled", 2, 1)));
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setInitialFocusName("amount");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window, options);
        renderFrame(window, frame);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("7"));
        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.mouse(
                EventType.MOUSE_PRESS,
                new Point(1, CONTENT_SIZE.width())));

        assertEquals(7.0, amount.value());
        assertEquals(-1, frame.focusedIndex());
        assertNull(focusedElement(window, frame));
        assertFalse(frame.editMode());
    }

    @Test
    void enterWithoutFocusDoesNotSelectOrActivateTheFirstButton()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("No focus");
        window.addElement(new Button("zero", "0"));
        window.setGeneratedLayout(List.of(layout("zero", 0, 1)));
        AtomicInteger activations = new AtomicInteger();
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setOnButton(name -> activations.incrementAndGet());
        GeneratedWindowStackFrame frame = GeneratedWindowRuntime.createFrame(window, options);

        GeneratedWindowRuntime.dispatchFrameEvent(frame, Event.key("Enter"));

        assertEquals(-1, frame.focusedIndex());
        assertEquals(0, activations.get());
        assertFalse(frame.editMode());
    }

    @Test
    void bracketedPasteStartsTextEditReplacesSelectionAndNotifiesOnce()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Paste");
        TextArea description = window.addElement(new TextArea("description", "before", 0));
        window.setGeneratedLayout(List.of(layout("description", 0, CONTENT_SIZE.height())));
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setInitialFocusName("description");
        AtomicInteger changes = new AtomicInteger();
        options.setOnTextChanged((name, value) -> changes.incrementAndGet());
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window, options);
        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.paste(" first"));
        assertEquals("before first", description.value());
        assertTrue(frame.editMode());

        description.selectRange(0, description.value().length());
        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.paste("split paste\r\nvalue"));

        assertEquals("split paste\nvalue", description.value());
        assertTrue(frame.editMode());
        assertEquals(2, changes.get());
    }

    @Test
    void singleSelectListBoxEnterEndsEditEvenWithLeaveCommitMode()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("ListBox commit");
        ListBox entries = window.addElement(new ListBox("entries", List.of("A", "B"), false));
        entries.setCommitMode(Element.COMMIT_MODE_LEAVE);
        window.setGeneratedLayout(List.of(layout("entries", 0, CONTENT_SIZE.height())));
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setInitialFocusName("entries");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window, options);
        renderFrame(window, frame);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Down"));
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));

        assertEquals(List.of("B"), entries.selectedValues());
        assertFalse(frame.editMode());
    }

    @Test
    void mousePressSelectsSingleListBoxRowAndNotifiesTheOwner()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Single ListBox mouse");
        ListBox entries = window.addElement(new ListBox(
            "entries",
            List.of("A", "B", "C"),
            false));
        window.setGeneratedLayout(List.of(layout("entries", 0, CONTENT_SIZE.height())));
        List<List<String>> changes = new ArrayList<>();
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setOnSelectionChanged((name, values) -> changes.add(List.copyOf(values)));
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window, options);
        renderFrame(window, frame);

        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.mouse(
                EventType.MOUSE_PRESS,
                new Point(entries.frame().row() + 2, entries.frame().col())));

        assertEquals(2, entries.selectedIndex());
        assertEquals(List.of("C"), entries.selectedValues());
        assertEquals(2, entries.activeIndex());
        assertFalse(entries.activeItemVisible());
        assertSame(entries, focusedElement(window, frame));
        assertTrue(frame.editMode());
        assertEquals(List.of(List.of("C")), changes);
    }

    @Test
    void repeatedMousePressTogglesAMultipleListBoxRow()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Multiple ListBox mouse");
        ListBox entries = window.addElement(new ListBox(
            "entries",
            List.of("A", "B", "C"),
            true));
        entries.setSelectedValues(List.of("A"));
        window.setGeneratedLayout(List.of(layout("entries", 0, CONTENT_SIZE.height())));
        List<List<String>> changes = new ArrayList<>();
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setOnSelectionChanged((name, values) -> changes.add(List.copyOf(values)));
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window, options);
        renderFrame(window, frame);
        Event secondRowPress = Event.mouse(
            EventType.MOUSE_PRESS,
            new Point(entries.frame().row() + 1, entries.frame().col()));

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, secondRowPress);
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, secondRowPress);

        assertEquals(List.of("A"), entries.selectedValues());
        assertEquals(1, entries.activeIndex());
        assertTrue(frame.editMode());
        assertEquals(
            List.of(List.of("A", "B"), List.of("A")),
            changes);
    }

    @Test
    void animatedGradientDetectionCoversWindowStateAndNestedElementStyles()
    {
        GeneratedWindowBase plain = new GeneratedWindowBase("Plain");
        assertFalse(GeneratedWindowRuntime.windowHasAnimatedTextGradient(plain));

        Style windowStyle = new Style();
        windowStyle.setTextBackgroundGradient(new TextGradient());
        plain.setGeneratedWindowStyle(windowStyle);
        assertTrue(GeneratedWindowRuntime.windowHasAnimatedTextGradient(plain));

        GeneratedWindowBase nested = new GeneratedWindowBase("Nested gradient");
        Element parent = nested.addElement(new Element("parent"));
        Label child = parent.addChild(new Label("child", "Animated"));
        Style activeStyle = new Style();
        activeStyle.setTextColorGradient(new TextGradient());
        child.setActiveStyle(activeStyle);
        assertTrue(GeneratedWindowRuntime.windowHasAnimatedTextGradient(nested));
    }

    @Test
    void comboBoxDropdownMouseHitOverlaysFollowingSibling()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Combo mouse overlay");
        ComboBox theme = window.addElement(new ComboBox("theme", List.of("Light", "Dark")));
        window.addElement(new Button("covered", "Covered"));
        window.setGeneratedLayout(List.of(
            layout("theme", 0, 1),
            layout("covered", 2, 1)));
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setInitialFocusName("theme");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window, options);
        renderFrame(window, frame);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        renderFrame(window, frame);
        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.mouse(
                EventType.MOUSE_PRESS,
                new Point(theme.frame().row() + 2, theme.frame().col())));

        assertEquals("Dark", theme.selectedText());
        assertSame(theme, focusedElement(window, frame));
        assertFalse(frame.editMode());
    }

    @Test
    void spatialNavigationRejectsAVisuallyOverlappingDirectionalCandidate()
    {
        final Rect overlappingCandidateFrame = new Rect(7, 0, 5, 3);
        final Rect tallScrollProxyFrame = new Rect(0, 6, 14, 10);
        GeneratedWindowBase window = new GeneratedWindowBase("Spatial overlap");
        Button galleryMosaic = window.addElement(new Button("gallery_mosaic", "Gallery"));
        galleryMosaic.setFrame(overlappingCandidateFrame);
        TestScrollWindow scrollWindow = new TestScrollWindow();
        ReusableElement galleryScroll = window.addElement(
            new ReusableElement("gallery_scroll", scrollWindow));
        galleryScroll.setFrame(tallScrollProxyFrame);
        GeneratedWindowRuntimeOptions options = window.runtimeOptions();
        options.setInitialFocusName("gallery_mosaic");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window, options);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Right"));
        assertSame(galleryScroll, focusedElement(window, frame));

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Down"));
        assertSame(galleryScroll, focusedElement(window, frame));
    }

    @Test
    void viewHostPropagatesEditModeToItsFocusedChild()
    {
        Color baseBackground = new Color("#172033");
        Color focusBackground = new Color("#25364f");
        Color editBackground = new Color("#0b1220");
        GeneratedWindowBase child = new GeneratedWindowBase("Child");
        TextInput input = child.addElement(new TextInput("item_input", "Coffee", 0));
        input.setStyle(Style.fromProperties("background", baseBackground.text()));
        input.setFocusStyle(Style.fromProperties("background", focusBackground.text()));
        input.setEditStyle(Style.fromProperties("background", editBackground.text()));
        child.setGeneratedLayout(List.of(layout("item_input", 0, 1)));

        GeneratedWindowBase parent = new GeneratedWindowBase("Parent");
        ViewHost host = parent.addElement(new ViewHost("main"));
        host.setView(child);
        parent.setGeneratedLayout(List.of(layout("main", 0, 1)));
        int focusedIndex = GeneratedWindowRuntime.focusableElements(parent, null).indexOf(input);

        List<List<TerminalCell>> focused = GeneratedWindowRuntime.renderGeneratedWindowContent(
            parent,
            new Size(CONTENT_SIZE.width(), 1),
            focusedIndex,
            false,
            null,
            null,
            false);
        List<List<TerminalCell>> editing = GeneratedWindowRuntime.renderGeneratedWindowContent(
            parent,
            new Size(CONTENT_SIZE.width(), 1),
            focusedIndex,
            true,
            null,
            null,
            false);

        assertTrue(focusedIndex >= 0);
        assertEquals(focusBackground, focused.get(0).get(0).background());
        assertEquals(editBackground, editing.get(0).get(0).background());
    }

    private static Element focusedElement(
        GeneratedWindowBase window,
        GeneratedWindowStackFrame frame)
    {
        List<Element> focusable = GeneratedWindowRuntime.focusableElements(
            window,
            frame.activeScrollView());
        return frame.focusedIndex() < 0 || frame.focusedIndex() >= focusable.size()
            ? null
            : focusable.get(frame.focusedIndex());
    }

    private static void renderFrame(
        GeneratedWindowBase window,
        GeneratedWindowStackFrame frame)
    {
        GeneratedWindowRuntime.renderGeneratedWindowContent(
            window,
            CONTENT_SIZE,
            frame.focusedIndex(),
            frame.editMode(),
            frame.activeScrollView(),
            frame.activeScrollViewEditElement(),
            frame.suppressActiveScrollViewScopeVisuals());
    }

    private static GeneratedLayoutEntry layout(
        String name,
        int row,
        int height)
    {
        return new GeneratedLayoutEntry()
            .setName(name)
            .setRelative(new Rect(0, 0, CONTENT_SIZE.width(), height))
            .setSourceCell(new Rect(row, 0, CONTENT_SIZE.width(), height))
            .setWidth(AxisDimension.expanded())
            .setHeight(AxisDimension.auto(height))
            .setCellWidth(AxisDimension.auto(CONTENT_SIZE.width()))
            .setCellHeight(AxisDimension.auto(height));
    }

    private static final class FocusFixture
    {
        private final GeneratedWindowBase root = new GeneratedWindowBase("Root");
        private final TrackingRowWindow first = new TrackingRowWindow("first");
        private final TrackingRowWindow second = new TrackingRowWindow("second");
        private final TestScrollWindow scrollWindow = new TestScrollWindow();
        private final ReusableElement host = new ReusableElement("host", scrollWindow);

        FocusFixture()
        {
            scrollWindow.scrollView().addChild(new ReusableElement("first_row", first));
            scrollWindow.scrollView().addChild(new ReusableElement("second_row", second));
            root.setGeneratedLayout(List.of(layout("host", 0, CONTENT_SIZE.height())));
            root.addElement(host);
        }
    }

    private static final class TestScrollWindow extends GeneratedScrollViewBase
    {
        TestScrollWindow()
        {
            ScrollView scrollView = addElement(new ScrollView("__scrollview"));
            setGeneratedScrollView(scrollView);
            setGeneratedLayout(List.of(layout("__scrollview", 0, CONTENT_SIZE.height())));
        }
    }

    private static final class ActivatingControlWindow extends GeneratedWindowBase
    {
        private int activationCount;

        ActivatingControlWindow()
        {
            super("Action");
            setGeneratedFocusable(true);
        }

        @Override
        public boolean activateGeneratedControl()
        {
            ++activationCount;
            return true;
        }
    }

    private static final class TrackingRowWindow extends GeneratedWindowBase
    {
        private final Button button;
        private final TextInput input;
        private int buttonActivations;

        TrackingRowWindow(String prefix)
        {
            button = addElement(new Button(prefix + "_button", "Open"));
            input = addElement(new TextInput(prefix + "_input", "seed", 0));
            setGeneratedLayout(List.of(
                layout(button.name(), 0, 1),
                layout(input.name(), 2, 1)));
        }

        @Override
        public boolean handleGeneratedButton(String name)
        {
            if (button.name().equals(name))
            {
                ++buttonActivations;
                return true;
            }
            return false;
        }
    }
}
