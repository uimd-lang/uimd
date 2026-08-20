package uimd;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.jupiter.api.Test;


final class GeneratedWindowStackTest
{
    private static final Size TERMINAL_SIZE = new Size(20, 10);

    @Test
    void pushSelectsRequestedFocusAndStackOperationsKeepTopWindowUniversal()
    {
        GeneratedWindowBase first = buttonWindow("First", "first", "second");
        GeneratedWindowRuntimeOptions firstOptions = first.runtimeOptions();
        firstOptions.setInitialFocusName("second");
        GeneratedWindowBase second = buttonWindow("Second", "only", "");

        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame firstFrame = stack.push(first, firstOptions);
        GeneratedWindowStackFrame secondFrame = stack.push(second);

        assertEquals(1, firstFrame.focusedIndex());
        assertEquals(-1, secondFrame.focusedIndex());
        assertSame(second, stack.topWindow());
        stack.pop();
        assertSame(first, stack.topWindow());
        stack.remove(first);
        assertTrue(stack.empty());
    }

    @Test
    void stackFrameInspectionIsPublicAndReadOnly()
    {
        GeneratedWindowBase first = buttonWindow("First", "first", "second");
        GeneratedWindowBase second = buttonWindow("Second", "only", "other");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame firstFrame = stack.push(first);
        GeneratedWindowStackFrame secondFrame = stack.push(second);

        assertEquals(List.of(firstFrame, secondFrame), stack.frames());
        assertThrows(UnsupportedOperationException.class, () -> stack.frames().clear());
        assertSame(secondFrame, stack.topFrame());
    }

    @Test
    void runtimeOverlayDimsBeforeRenderingAndUsesTheCurrentBuffer()
    {
        TerminalBuffer buffer = new TerminalBuffer(1, 1);
        TerminalCell background = new TerminalCell();
        background.setBackground(new Color("#808080"));
        buffer.clear(background);
        GeneratedWindowRuntimeOptions options = new GeneratedWindowRuntimeOptions();
        AtomicInteger dimQueries = new AtomicInteger();
        options.setShouldDimOverlayBackground(() ->
        {
            dimQueries.incrementAndGet();
            return true;
        });
        options.setOnRenderOverlay(current ->
        {
            assertSame(buffer, current);
            assertEquals(new Color("#404040"), current.cell(0, 0).background());
            current.cell(0, 0).setText("O");
        });

        GeneratedWindowRuntime.renderRuntimeOverlay(buffer, options);

        assertEquals(1, dimQueries.get());
        assertEquals("O", buffer.cell(0, 0).text());
    }

    @Test
    void initialFocusMatchesNamedInvalidAndStartInEditBranches()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Inputs");
        window.setGeneratedLayout(List.of(
            layout("first", 0, 0, 6, 1),
            layout("second", 1, 0, 6, 1)));
        window.addElement(new TextInput("first", "", 10));
        window.addElement(new TextInput("second", "", 10));

        GeneratedWindowRuntimeOptions invalidOptions = new GeneratedWindowRuntimeOptions();
        invalidOptions.setInitialFocusName("missing");
        invalidOptions.setStartInEditMode(true);
        GeneratedWindowStackFrame invalidFrame = new GeneratedWindowStack().push(
            window,
            invalidOptions);
        assertEquals(-1, invalidFrame.focusedIndex());
        assertFalse(invalidFrame.editMode());

        GeneratedWindowRuntimeOptions editOptions = new GeneratedWindowRuntimeOptions();
        editOptions.setStartInEditMode(true);
        GeneratedWindowStackFrame editFrame = new GeneratedWindowStack().push(window, editOptions);
        assertEquals(0, editFrame.focusedIndex());
        assertTrue(editFrame.editMode());
    }

    @Test
    void focusRequestedBeforeAttachmentIsAppliedOnceAndLaterRequestsUpdateTheFrame()
    {
        GeneratedWindowBase window = buttonWindow("Buttons", "first", "second");
        Element first = window.elements().get(0);
        Element second = window.elements().get(1);
        window.setFocus(second);

        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(window);
        assertEquals(1, frame.focusedIndex());

        window.setFocus(first);
        assertEquals(0, frame.focusedIndex());

        stack.pop();
        GeneratedWindowStackFrame nextFrame = stack.push(window);
        assertEquals(-1, nextFrame.focusedIndex());
    }

    @Test
    void nestedModalRenderingDimsEachCoveredLayerExactlyOnce()
    {
        TerminalBuffer buffer = new TerminalBuffer(TERMINAL_SIZE.width(), TERMINAL_SIZE.height());
        TerminalCell background = new TerminalCell();
        background.setBackground(new Color("#808080"));
        buffer.clear(background);

        GeneratedWindowBase first = coloredWindow("First", new Size(4, 3), "#204060");
        GeneratedWindowBase second = coloredWindow("Second", new Size(2, 1), "#ff0000");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        stack.push(first);
        stack.push(second);

        GeneratedWindowRuntime.renderWindowStackOverlay(buffer, stack);

        assertEquals(new Color("#404040"), buffer.cell(0, 0).background());
        assertEquals(new Color("#102030"), buffer.cell(3, 8).background());
        assertEquals(new Color("#ff0000"), buffer.cell(4, 9).background());
    }

    @Test
    void modalBackgroundSuppressesTheCoveredRootScrollViewEditScope()
    {
        Color rootBackground = new Color("#204060");
        Color scopeBackground = new Color("#ffffff80");
        GeneratedWindowBase root = new GeneratedWindowBase("Root");
        root.setMode("fullscreen");
        root.setGeneratedWindowStyle(Style.fromProperties(
            "background", rootBackground.toString()));
        ScrollView items = root.addElement(new ScrollView("items"));
        items.setStyle(Style.fromProperties(
            "background", rootBackground.toString(),
            "scope-dim-background", scopeBackground.text()));
        GeneratedLayoutEntry itemsLayout = layout("items", 0, 0, 2, 2)
            .setCellWidth(AxisDimension.expanded())
            .setCellHeight(AxisDimension.expanded())
            .setCellStyle(Style.fromProperties("background", rootBackground.toString()));
        root.setGeneratedLayout(List.of(itemsLayout));

        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame rootFrame = GeneratedWindowRuntime.createFrame(
            root,
            root.runtimeOptions());
        rootFrame.setActiveScrollView(items);
        rootFrame.setEditMode(true);
        stack.setRootFrame(rootFrame);

        int outsideRow = TERMINAL_SIZE.height() - 1;
        int outsideCol = TERMINAL_SIZE.width() - 1;
        List<List<TerminalCell>> activeScope = GeneratedWindowRuntime.renderRuntimeStack(
            stack,
            TERMINAL_SIZE);
        assertEquals(
            scopeBackground.blendOver(rootBackground),
            activeScope.get(outsideRow).get(outsideCol).background());

        stack.push(coloredWindow("Modal", new Size(2, 1), "#ff0000"));
        List<List<TerminalCell>> modal = GeneratedWindowRuntime.renderRuntimeStack(
            stack,
            TERMINAL_SIZE);

        assertEquals(
            Color.rgb(16, 32, 48, 255),
            modal.get(outsideRow).get(outsideCol).background());
    }

    @Test
    void modalBackgroundSuppressesNestedReusableScrollViewFocusSurface()
    {
        Color rootBackground = new Color("#204060");
        GeneratedScrollViewBase child = new GeneratedScrollViewBase("Board");
        child.setGeneratedWindowStyle(Style.fromProperties(
            "background", rootBackground.toString()));
        ScrollView board = child.addElement(new ScrollView("__scrollview"));
        board.setStyle(Style.fromProperties("background", rootBackground.toString()));
        board.setFocusStyle(Style.fromProperties("background", "#ffffff80"));
        Button open = new Button("open", "Open");
        board.addChild(open);
        child.setGeneratedScrollView(board);
        child.setGeneratedLayout(List.of(
            layout("__scrollview", 0, 0, 1, 1)
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCellStyle(Style.fromProperties(
                    "background", rootBackground.toString()))));

        GeneratedWindowBase root = new GeneratedWindowBase("Root");
        root.setMode("fullscreen");
        root.setGeneratedWindowStyle(Style.fromProperties(
            "background", rootBackground.toString()));
        ReusableElement host = root.addElement(new ReusableElement("board", child));
        host.setFocusStyle(Style.fromProperties("background", "#ffffff80"));
        root.setGeneratedLayout(List.of(
            layout("board", 0, 0, 1, 1)
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCellStyle(Style.fromProperties(
                    "background", rootBackground.toString()))));

        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame rootFrame = GeneratedWindowRuntime.createFrame(
            root,
            root.runtimeOptions());
        rootFrame.setActiveScrollView(board);
        rootFrame.setFocusedIndex(
            GeneratedWindowRuntime.focusableElements(root, board).indexOf(open));
        rootFrame.setEditMode(false);
        stack.setRootFrame(rootFrame);
        stack.push(coloredWindow("Modal", new Size(2, 1), "#ff0000"));

        List<List<TerminalCell>> rendered = GeneratedWindowRuntime.renderRuntimeStack(
            stack,
            TERMINAL_SIZE);

        assertEquals(
            Color.rgb(16, 32, 48, 255),
            rendered.get(TERMINAL_SIZE.height() - 1)
                .get(TERMINAL_SIZE.width() - 1)
                .background());
    }

    @Test
    void topModalComboBoxDropdownRendersBeyondItsNaturalFrame()
    {
        GeneratedWindowBase modal = new GeneratedWindowBase("Settings");
        ComboBox choice = modal.addElement(new ComboBox(
            "choice",
            List.of("One", "Two", "Three", "Four")));
        modal.setGeneratedLayout(List.of(layout("choice", 0, 0, 8, 1)));
        GeneratedWindowRuntimeOptions options = modal.runtimeOptions();
        options.setStartInEditMode(true);

        GeneratedWindowStack stack = new GeneratedWindowStack();
        stack.push(modal, options);
        TerminalBuffer buffer = new TerminalBuffer(
            TERMINAL_SIZE.width(),
            TERMINAL_SIZE.height());
        GeneratedWindowRuntime.renderWindowStackOverlay(buffer, stack);

        Rect frame = choice.frame();
        assertEquals(
            "F",
            buffer.cell(frame.row() + choice.options().size(), frame.col() + 2).text());
    }

    @Test
    void inputRoutesOnlyToTheTopWindowUntilItIsPopped()
    {
        AtomicInteger parentActivations = new AtomicInteger();
        AtomicInteger modalActivations = new AtomicInteger();
        GeneratedWindowBase parent = buttonWindow("Parent", "parent", "parent_other");
        GeneratedWindowBase modal = buttonWindow("Modal", "modal", "modal_other");
        GeneratedWindowRuntimeOptions parentOptions = parent.runtimeOptions();
        parentOptions.setInitialFocusName("parent");
        parentOptions.setOnButton(name -> parentActivations.incrementAndGet());
        GeneratedWindowRuntimeOptions modalOptions = modal.runtimeOptions();
        modalOptions.setInitialFocusName("modal");
        modalOptions.setOnButton(name -> modalActivations.incrementAndGet());
        GeneratedWindowStack stack = new GeneratedWindowStack();
        stack.push(parent, parentOptions);
        stack.push(modal, modalOptions);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        assertEquals(0, parentActivations.get());
        assertEquals(1, modalActivations.get());

        stack.pop();
        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));
        assertEquals(1, parentActivations.get());
        assertEquals(1, modalActivations.get());
    }

    private static GeneratedWindowBase buttonWindow(String title, String firstName, String secondName)
    {
        GeneratedWindowBase window = new GeneratedWindowBase(title);
        window.setGeneratedLayout(List.of(
            layout(firstName, 0, 0, 6, 1),
            layout(secondName, 1, 0, 6, 1)));
        window.addElement(new Button(firstName, firstName));
        window.addElement(new Button(secondName, secondName));
        return window;
    }

    private static GeneratedWindowBase coloredWindow(String title, Size size, String color)
    {
        GeneratedWindowBase window = new GeneratedWindowBase(title);
        Style style = Style.fromProperties("background", color);
        window.setGeneratedWindowStyle(style);
        window.setGeneratedLayout(List.of(
            layout("surface", 0, 0, size.width(), size.height())
                .setCellStyle(style)
                .setElementStyle(style)));
        Label surface = new Label("surface", "");
        surface.setStyle(style);
        window.addElement(surface);
        return window;
    }

    private static GeneratedLayoutEntry layout(
        String name,
        int row,
        int col,
        int width,
        int height)
    {
        return new GeneratedLayoutEntry()
            .setName(name)
            .setRelative(new Rect(0, 0, width, height))
            .setSourceCell(new Rect(row, col, width, height))
            .setWidth(AxisDimension.auto(width))
            .setHeight(AxisDimension.auto(height))
            .setCellWidth(AxisDimension.auto(width))
            .setCellHeight(AxisDimension.auto(height));
    }
}
