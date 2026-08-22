package uimd;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.IdentityHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.IntSupplier;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.TimeUnit;

import uimd.terminal.TerminalBackend;
import uimd.terminal.TerminalModeGuard;

public final class GeneratedWindowRuntime
{
    public static final int MINIMUM_RENDERABLE_SIZE = 1;

    private static final double MODAL_BACKGROUND_DIM_FACTOR = 0.5;
    private static final int INPUT_IDLE_SLEEP_MILLISECONDS = 10;
    private static final long ANIMATED_RENDER_INTERVAL_NANOS = TimeUnit.MILLISECONDS.toNanos(70);
    private static final int DIALOG_BUTTON_CLOSE_DELAY_MILLISECONDS = 180;
    private static final int EXIT_SUCCESS = 0;
    private static final int EXIT_FAILURE = 1;
    private static final int COMBO_BOX_CLOSED_ROWS = 1;
    private static final int COMBO_BOX_DROPDOWN_ROWS = 6;
    private static final String ORIENTATION_HORIZONTAL = "horizontal";
    private static final String ORIENTATION_VERTICAL = "vertical";
    private static final String SCROLL_INDICATOR_ABOVE = "^";
    private static final String SCROLL_INDICATOR_BELOW = "v";
    private static final String COPY_NOTIFICATION = "Copied to clipboard";
    private static final long COPY_NOTIFICATION_DURATION_NANOS = TimeUnit.SECONDS.toNanos(3);
    private static final int NOTIFICATION_ROW = 0;
    private static final int NOTIFICATION_HORIZONTAL_PADDING = 1;
    private static final int NOTIFICATION_RIGHT_MARGIN = 1;
    private static final Color NOTIFICATION_FOREGROUND = new Color("#ffffff");
    private static final Color NOTIFICATION_BACKGROUND = new Color("#2255bb");

    private enum GeneratedWindowMode
    {
        NORMAL,
        EXPAND_WIDTH,
        EXPAND_HEIGHT,
        FULLSCREEN,
    }

    private static final class RuntimeCell
    {
        private final int sourceRow;
        private final int sourceCol;
        private final int sourceWidth;
        private final int sourceHeight;
        private Integer logicalWidth;
        private Integer logicalHeight;

        RuntimeCell(
            int sourceRow,
            int sourceCol,
            int sourceWidth,
            int sourceHeight,
            Integer logicalWidth,
            Integer logicalHeight)
        {
            this.sourceRow = sourceRow;
            this.sourceCol = sourceCol;
            this.sourceWidth = sourceWidth;
            this.sourceHeight = sourceHeight;
            this.logicalWidth = logicalWidth;
            this.logicalHeight = logicalHeight;
        }
    }

    private static final class RuntimeNode
    {
        private String orientation = "";
        private List<RuntimeNode> children = new ArrayList<>();
        private RuntimeCell cell;
        private int left;
        private int top;
        private int right;
        private int bottom;
        private Integer logicalWidth;
        private Integer logicalHeight;
    }

    private record ResolvedRuntimeCell(RuntimeCell cell, Rect rect)
    {
    }

    private record IntSpan(int start, int end)
    {
    }

    private record MouseSelectionAnchor(Element element, int position)
    {
    }

    static final class EditSnapshot
    {
        private final Element element;
        private String textValue = "";
        private int textCursor;
        private Integer textSelectionAnchor;
        private double numberValue;
        private int selectedIndex;

        EditSnapshot(Element element)
        {
            this.element = element;
        }
    }

    private static final class RuntimeState
    {
        private int focusedIndex;
        private boolean editMode;
        private ScrollView activeScrollView;
        private Element activeScrollViewEditElement;
        private boolean suppressActiveScrollViewScopeVisuals;
        private EditSnapshot editSnapshot;
        private final Map<ScrollView, Element> scrollViewLastDescendant;

        RuntimeState(int focusedIndex, boolean editMode)
        {
            this.focusedIndex = focusedIndex;
            this.editMode = editMode;
            scrollViewLastDescendant = new IdentityHashMap<>();
        }


        RuntimeState(GeneratedWindowStackFrame frame)
        {
            focusedIndex = frame.focusedIndex();
            editMode = frame.editMode();
            activeScrollView = frame.activeScrollView();
            activeScrollViewEditElement = frame.activeScrollViewEditElement();
            suppressActiveScrollViewScopeVisuals = frame.suppressActiveScrollViewScopeVisuals();
            editSnapshot = frame.editSnapshot();
            scrollViewLastDescendant = frame.scrollViewLastDescendant();
        }

        void writeBack(GeneratedWindowStackFrame frame)
        {
            frame.setFocusedIndex(focusedIndex);
            frame.setEditMode(editMode);
            frame.setActiveScrollView(activeScrollView);
            frame.setActiveScrollViewEditElement(activeScrollViewEditElement);
            frame.setSuppressActiveScrollViewScopeVisuals(suppressActiveScrollViewScopeVisuals);
            frame.setEditSnapshot(editSnapshot);
        }
    }

    private GeneratedWindowRuntime()
    {
    }

    public static int runGeneratedAppMain(IntSupplier application)
    {
        try
        {
            return application.getAsInt();
        }
        catch (RuntimeException exception)
        {
            return reportGeneratedAppUnhandledException(exception);
        }
        catch (Error error)
        {
            return reportGeneratedAppUnhandledException(error);
        }
    }

    public static int reportGeneratedAppUnhandledException(Throwable exception)
    {
        if (exception == null)
        {
            return reportGeneratedAppUnhandledException();
        }
        String message = exception.getMessage();
        if (message == null || message.isEmpty())
        {
            message = exception.getClass().getSimpleName();
        }
        System.err.println("uimd: error: " + message);
        return EXIT_FAILURE;
    }

    public static int reportGeneratedAppUnhandledException()
    {
        System.err.println("uimd: error: unhandled Java exception");
        return EXIT_FAILURE;
    }

    public static int runGeneratedWindow(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        String[] arguments)
    {
        McpRuntimeConfig mcpConfig = McpRuntimeConfig.parse(arguments);
        GeneratedWindowRuntimeOptions runtimeOptions = options == null
            ? window.runtimeOptions()
            : options;
        GeneratedWindowStack windowStack = runtimeOptions.windowStack();
        if (windowStack == null)
        {
            windowStack = new GeneratedWindowStack();
            runtimeOptions.setWindowStack(windowStack);
        }
        GeneratedWindowStackFrame rootFrame = createFrame(window, runtimeOptions);
        windowStack.setRootFrame(rootFrame);
        McpController mcpController = new McpController(window, runtimeOptions, mcpConfig);
        if (mcpConfig.enabled() && !mcpConfig.gui())
        {
            serveHeadlessMcp(mcpController, mcpConfig);
            return EXIT_SUCCESS;
        }
        TerminalBackend terminal = new TerminalBackend(System.out);
        if (!terminal.interactive() && !mcpConfig.enabled())
        {
            renderNonInteractive(window, terminal.size());
            return EXIT_SUCCESS;
        }
        McpSocketServer mcpServer = startGuiMcp(mcpController, mcpConfig);
        return runInteractive(
            window,
            runtimeOptions,
            rootFrame,
            terminal,
            mcpController,
            mcpConfig,
            mcpServer);
    }

    public static int runGeneratedWindow(GeneratedWindowBase window, String[] arguments)
    {
        return runGeneratedWindow(window, window.runtimeOptions(), arguments);
    }

    public static int runGeneratedWindow(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options)
    {
        return runGeneratedWindow(window, options, new String[0]);
    }

    public static int runGeneratedWindow(GeneratedWindowBase window)
    {
        return runGeneratedWindow(window, window.runtimeOptions(), new String[0]);
    }

    public static Element findElement(GeneratedWindowBase window, String name)
    {
        if (name == null || name.isEmpty())
        {
            return null;
        }
        for (Element element : window.elements())
        {
            Element found = findElementInTree(element, name);
            if (found != null)
            {
                return found;
            }
        }
        return null;
    }

    public static List<Element> focusableElements(GeneratedWindowBase window)
    {
        return focusableElements(window, null);
    }

    public static List<Element> focusableElements(
        GeneratedWindowBase window,
        ScrollView activeScrollView)
    {
        List<Element> result = new ArrayList<>();
        for (Element element : window.elements())
        {
            addFocusableElements(element, activeScrollView, result);
        }
        return result;
    }

    public static boolean isFocusable(Element element)
    {
        return element != null
            && element.enabled()
            && element.focusable()
            && isFocusableType(element);
    }

    public static Size generatedWindowContentSize(GeneratedWindowBase window)
    {
        Style style = window.generatedWindowStyle();
        List<RuntimeCell> runtimeCells = collectRuntimeCells(window);
        int horizontalSeparator = borderWidthHorizontal(style);
        int verticalSeparator = borderWidthVertical(style);
        Size natural = layoutSize(buildRuntimeTreeFromCells(
            runtimeCells,
            horizontalSeparator,
            verticalSeparator));
        List<ResolvedRuntimeCell> resolvedCells = resolveRuntimeCellsWithFitPass(
            window,
            runtimeCells,
            new Rect(0, 0, natural.width(), natural.height()),
            horizontalSeparator,
            verticalSeparator,
            windowMode(window));
        return resolvedContentExtent(resolvedCells, natural);
    }

    public static Size generatedWindowContentSizeForWidth(GeneratedWindowBase window, int width)
    {
        Style style = window.generatedWindowStyle();
        List<RuntimeCell> runtimeCells = collectRuntimeCells(window);
        int horizontalSeparator = borderWidthHorizontal(style);
        int verticalSeparator = borderWidthVertical(style);
        Size natural = layoutSize(buildRuntimeTreeFromCells(
            runtimeCells,
            horizontalSeparator,
            verticalSeparator));
        int resolvedWidth = Math.max(MINIMUM_RENDERABLE_SIZE, width);
        List<ResolvedRuntimeCell> resolvedCells = resolveRuntimeCellsWithFitPass(
            window,
            runtimeCells,
            new Rect(0, 0, resolvedWidth, natural.height()),
            horizontalSeparator,
            verticalSeparator,
            windowMode(window));
        Size result = resolvedContentExtent(
            resolvedCells,
            new Size(resolvedWidth, natural.height()));
        int resultWidth = result.width();
        int resultHeight = result.height();
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            ResolvedRuntimeCell cell = resolvedCellFor(resolvedCells, entry);
            if (cell == null)
            {
                continue;
            }
            Element element = findElement(window, entry.name());
            Size entrySize = renderSizeFor(entry, cell.rect(), element);
            resultWidth = Math.max(
                resultWidth,
                cell.rect().col() + paddingLeft(entry.cellStyle())
                    + entry.relative().col() + entrySize.width());
            resultHeight = Math.max(
                resultHeight,
                cell.rect().row() + paddingTop(entry.cellStyle())
                    + entry.relative().row() + entrySize.height());
        }
        return new Size(resultWidth, resultHeight);
    }

    public static Size generatedWindowResolvedContentSize(
        GeneratedWindowBase window,
        Size size)
    {
        int width = Math.max(MINIMUM_RENDERABLE_SIZE, size.width());
        int height = Math.max(MINIMUM_RENDERABLE_SIZE, size.height());
        Style style = window.generatedWindowStyle();
        List<RuntimeCell> runtimeCells = collectRuntimeCells(window);
        int horizontalSeparator = borderWidthHorizontal(style);
        int verticalSeparator = borderWidthVertical(style);
        List<ResolvedRuntimeCell> resolvedCells = resolveRuntimeCellsWithFitPass(
            window,
            runtimeCells,
            new Rect(0, 0, width, height),
            horizontalSeparator,
            verticalSeparator,
            windowMode(window));
        return resolvedContentExtent(resolvedCells, new Size(width, height));
    }

    public static List<List<TerminalCell>> renderGeneratedWindowContent(
        GeneratedWindowBase window,
        Size size,
        int focusedIndex)
    {
        return renderGeneratedWindowContent(window, size, focusedIndex, false);
    }

    public static List<List<TerminalCell>> renderGeneratedWindowContent(
        GeneratedWindowBase window,
        Size size,
        int focusedIndex,
        boolean editMode)
    {
        return renderGeneratedWindowContent(
            window,
            size,
            focusedIndex,
            editMode,
            null,
            null,
            false);
    }

    public static List<List<TerminalCell>> renderGeneratedWindowContent(
        GeneratedWindowBase window,
        Size size,
        int focusedIndex,
        boolean editMode,
        ScrollView activeScrollView,
        Element activeScrollViewEditElement,
        boolean suppressActiveScrollViewScopeVisuals)
    {
        return renderGeneratedWindowContent(
            window,
            size,
            focusedIndex,
            editMode,
            activeScrollView,
            activeScrollViewEditElement,
            suppressActiveScrollViewScopeVisuals,
            null,
            null,
            true,
            false,
            false);
    }

    static List<List<TerminalCell>> renderGeneratedWindowContent(
        GeneratedWindowBase window,
        Size size,
        int focusedIndex,
        boolean editMode,
        ScrollView activeScrollView,
        Element activeScrollViewEditElement,
        boolean suppressActiveScrollViewScopeVisuals,
        Style windowStyleOverride,
        Color activeScrollViewFocusBackgroundOverride,
        boolean applyActiveScrollViewDim,
        boolean forceFullscreenLayout,
        boolean useHostViewportForRootScrollViewIndicators)
    {
        return renderGeneratedWindowContent(
            window,
            size,
            focusedIndex,
            editMode,
            activeScrollView,
            activeScrollViewEditElement,
            windowStyleOverride,
            activeScrollViewFocusBackgroundOverride,
            null,
            null,
            applyActiveScrollViewDim,
            forceFullscreenLayout,
            useHostViewportForRootScrollViewIndicators,
            suppressActiveScrollViewScopeVisuals);
    }

    public static List<List<TerminalCell>> renderGeneratedWindowContent(
        GeneratedWindowBase window,
        Size size,
        int focusedIndex,
        boolean editMode,
        ScrollView activeScrollView,
        Element activeScrollViewEditElement,
        Style windowStyleOverride,
        Color activeScrollViewFocusBackgroundOverride,
        Integer clipTop,
        Integer clipBottom,
        boolean applyActiveScrollViewDim,
        boolean forceFullscreenLayout,
        boolean useHostViewportForRootScrollViewIndicators,
        boolean suppressActiveScrollViewScopeVisuals)
    {
        int width = Math.max(MINIMUM_RENDERABLE_SIZE, size.width());
        int height = Math.max(MINIMUM_RENDERABLE_SIZE, size.height());
        TerminalBuffer buffer = new TerminalBuffer(width, height);
        Style windowStyle = windowStyleOverride == null
            ? window.generatedWindowStyle()
            : windowStyleOverride;
        fill(buffer, new Rect(0, 0, width, height), windowStyle);

        List<RuntimeCell> runtimeCells = collectRuntimeCells(window);
        List<ResolvedRuntimeCell> resolvedCells = resolveRuntimeCellsWithFitPass(
            window,
            runtimeCells,
            new Rect(0, 0, width, height),
            borderWidthHorizontal(windowStyle),
            borderWidthVertical(windowStyle),
            forceFullscreenLayout ? GeneratedWindowMode.FULLSCREEN : windowMode(window));

        List<Element> focusable = focusableElements(window, activeScrollView);
        Element focused = focusedIndex >= 0 && focusedIndex < focusable.size()
            ? focusable.get(focusedIndex)
            : null;
        List<ResolvedRuntimeCell> paintedCells = new ArrayList<>();
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            ResolvedRuntimeCell cell = resolvedCellFor(resolvedCells, entry);
            if (cell != null && !paintedCells.contains(cell))
            {
                paintedCells.add(cell);
                fill(buffer, cell.rect(), entry.cellStyle());
            }
        }
        GeneratedLayoutEntry deferredComboBoxEntry = null;
        boolean elementEditMode = editMode && activeScrollView == null;
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            ResolvedRuntimeCell cell = resolvedCellFor(resolvedCells, entry);
            if (cell == null)
            {
                continue;
            }
            if (isFocusedComboBoxEntry(window, entry, focused, elementEditMode))
            {
                deferredComboBoxEntry = entry;
                continue;
            }
            renderEntry(
                buffer,
                window,
                entry,
                cell.rect(),
                focused,
                editMode,
                activeScrollView,
                activeScrollViewEditElement,
                clipTop,
                clipBottom,
                suppressActiveScrollViewScopeVisuals,
                activeScrollViewFocusBackgroundOverride);
        }
        if (deferredComboBoxEntry != null)
        {
            ResolvedRuntimeCell cell = resolvedCellFor(resolvedCells, deferredComboBoxEntry);
            if (cell != null)
            {
                renderEntry(
                    buffer,
                    window,
                    deferredComboBoxEntry,
                    cell.rect(),
                    focused,
                    elementEditMode,
                    activeScrollView,
                    activeScrollViewEditElement,
                    clipTop,
                    clipBottom,
                    suppressActiveScrollViewScopeVisuals,
                    activeScrollViewFocusBackgroundOverride);
            }
        }
        if (applyActiveScrollViewDim
            && editMode
            && activeScrollView != null
            && !suppressActiveScrollViewScopeVisuals
            && windowContainsElement(window, activeScrollView))
        {
            Rect active = elementFrameInWindow(window, activeScrollView);
            if (active != null)
            {
                dimOutsideActiveScrollView(
                    buffer,
                    active,
                    generatedScrollViewScopeDimBackground(window, activeScrollView));
            }
        }
        List<List<TerminalCell>> rendered = contentFromBuffer(buffer);
        if (useHostViewportForRootScrollViewIndicators)
        {
            overlayHostViewportRootScrollViewIndicators(window, rendered, new Size(width, height));
        }
        return rendered;
    }

    public static List<List<TerminalCell>> renderTerminalContent(
        GeneratedWindowBase window,
        Size terminalSize,
        int focusedIndex)
    {
        return renderTerminalContent(window, terminalSize, focusedIndex, false);
    }

    public static List<List<TerminalCell>> renderTerminalContent(
        GeneratedWindowBase window,
        Size terminalSize,
        int focusedIndex,
        boolean editMode)
    {
        return renderTerminalContent(
            window,
            terminalSize,
            focusedIndex,
            editMode,
            null,
            null,
            false);
    }

    public static List<List<TerminalCell>> renderTerminalContent(
        GeneratedWindowBase window,
        Size terminalSize,
        int focusedIndex,
        boolean editMode,
        ScrollView activeScrollView,
        Element activeScrollViewEditElement,
        boolean suppressActiveScrollViewScopeVisuals)
    {
        int terminalWidth = Math.max(MINIMUM_RENDERABLE_SIZE, terminalSize.width());
        int terminalHeight = Math.max(MINIMUM_RENDERABLE_SIZE, terminalSize.height());
        TerminalBuffer terminalBuffer = new TerminalBuffer(terminalWidth, terminalHeight);
        terminalBuffer.clear();

        Rect bounds = windowBounds(window, new Size(terminalWidth, terminalHeight));
        Rect contentBounds = contentBounds(
            new Rect(0, 0, bounds.width(), bounds.height()),
            window.generatedWindowStyle());
        fill(terminalBuffer, bounds, borderStyleFor(window.generatedWindowStyle()));
        List<List<TerminalCell>> content = renderGeneratedWindowContent(
            window,
            new Size(contentBounds.width(), contentBounds.height()),
            focusedIndex,
            suppressActiveScrollViewScopeVisuals ? false : editMode,
            activeScrollView,
            activeScrollViewEditElement,
            suppressActiveScrollViewScopeVisuals);
        int row = bounds.row() + contentBounds.row();
        int col = bounds.col() + contentBounds.col();
        blit(terminalBuffer, content, row, col);
        if (!suppressActiveScrollViewScopeVisuals)
        {
            overlayFocusedComboBox(
                terminalBuffer,
                window,
                focusedIndex,
                editMode,
                bounds.row(),
                bounds.col(),
                activeScrollView,
                activeScrollViewEditElement);
        }
        syncWindowElementFramesTo(
            window,
            new Rect(row, col, contentBounds.width(), contentBounds.height()));
        return contentFromBuffer(terminalBuffer);
    }

    public static void renderGeneratedWindow(
        GeneratedWindowBase window,
        TerminalBuffer buffer)
    {
        renderGeneratedWindow(window, buffer, -1, false);
    }

    public static void renderGeneratedWindow(
        GeneratedWindowBase window,
        TerminalBuffer buffer,
        int focusedIndex,
        boolean editMode)
    {
        renderGeneratedWindow(
            window,
            buffer,
            focusedIndex,
            editMode,
            "",
            null,
            null,
            false);
    }

    public static void renderGeneratedWindow(
        GeneratedWindowBase window,
        TerminalBuffer buffer,
        int focusedIndex,
        boolean editMode,
        String footer,
        ScrollView activeScrollView,
        Element activeScrollViewEditElement,
        boolean suppressActiveScrollViewScopeVisuals)
    {
        if (window == null)
        {
            throw new IllegalArgumentException("window must not be null");
        }
        if (buffer == null)
        {
            throw new IllegalArgumentException("buffer must not be null");
        }
        buffer.clear();
        copyContentToBuffer(
            renderTerminalContent(
                window,
                new Size(buffer.width(), buffer.height()),
                focusedIndex,
                editMode,
                activeScrollView,
                activeScrollViewEditElement,
                suppressActiveScrollViewScopeVisuals),
            buffer);
    }

    private static int runInteractive(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        GeneratedWindowStackFrame rootFrame,
        TerminalBackend terminal,
        McpController mcpController,
        McpRuntimeConfig mcpConfig,
        McpSocketServer mcpServer)
    {
        Rect activeViewport = mcpController.currentViewport();
        boolean embeddedViewport = activeViewport != null;
        Size terminalSize = embeddedViewport
            ? new Size(activeViewport.width(), activeViewport.height())
            : terminal.size();
        TerminalBuffer terminalBuffer = new TerminalBuffer(terminalSize.width(), terminalSize.height());
        TerminalModeGuard terminalMode = new TerminalModeGuard();
        Size cellPixelSize = terminal.cellPixelSize();
        if (cellPixelSize != null)
        {
            Image.setImageTerminalCellPixels(cellPixelSize);
        }
        if (embeddedViewport)
        {
            terminal.enterEmbedded();
        }
        else
        {
            terminal.enter(window.title());
        }
        AtomicBoolean terminalRestored = new AtomicBoolean();
        Runnable restoreTerminal = () ->
        {
            if (!terminalRestored.compareAndSet(false, true))
            {
                return;
            }
            terminalMode.close();
            if (embeddedViewport)
            {
                terminal.leaveEmbedded();
            }
            else
            {
                terminal.leave();
            }
        };
        Thread shutdownHook = new Thread(restoreTerminal, "uimd-terminal-restore");
        Runtime.getRuntime().addShutdownHook(shutdownHook);
        try
        {
            boolean running = true;
            boolean dirty = true;
            long renderedMutationGeneration = -1;
            String notification = "";
            long notificationExpiresAt = 0L;
            Element mouseSelectionElement = null;
            int mouseSelectionAnchor = 0;
            boolean hasAnimatedTextGradient = windowHasAnimatedTextGradient(window);
            long lastAnimatedRenderAt = System.nanoTime();
            while (running)
            {
                Rect nextViewport = mcpController.currentViewport();
                Size nextSize = nextViewport == null
                    ? terminal.size()
                    : new Size(nextViewport.width(), nextViewport.height());
                if (!nextSize.equals(terminalSize) || !sameViewportOrigin(activeViewport, nextViewport))
                {
                    activeViewport = nextViewport;
                    terminalSize = nextSize;
                    terminalBuffer.resize(terminalSize.width(), terminalSize.height());
                    terminalBuffer.requestFullRedraw();
                    dirty = true;
                }
                long now = System.nanoTime();
                if (!mcpConfig.controlledRender()
                    && hasAnimatedTextGradient
                    && now - lastAnimatedRenderAt >= ANIMATED_RENDER_INTERVAL_NANOS)
                {
                    dirty = true;
                    lastAnimatedRenderAt = now;
                }
                if (!notification.isEmpty() && now >= notificationExpiresAt)
                {
                    notification = "";
                    dirty = true;
                }
                synchronized (mcpController.uiLock())
                {
                    long currentMutationGeneration = mcpController.mutationGeneration();
                    if (dirty || currentMutationGeneration != renderedMutationGeneration)
                    {
                        RuntimeState state = new RuntimeState(rootFrame);
                        renderToTerminal(
                            window,
                            options,
                            terminalSize,
                            state,
                            terminalBuffer,
                            terminal,
                            activeViewport,
                            notification);
                        renderedMutationGeneration = currentMutationGeneration;
                        mcpController.terminalRendered();
                        dirty = false;
                    }
                }
                if (mcpController.shouldCloseRequested())
                {
                    break;
                }

                List<Event> events = readRuntimeEvents(terminal, mcpConfig.enabled());
                if (events.isEmpty())
                {
                    idleWait();
                    continue;
                }
                for (Event event : events)
                {
                    synchronized (mcpController.uiLock())
                    {
                        GeneratedWindowStackFrame activeFrame = options.windowStack().topFrame();
                        if (activeFrame == null)
                        {
                            activeFrame = rootFrame;
                        }
                        GeneratedWindowBase activeWindow = activeFrame.window();
                        if (event.type() == EventType.MOUSE_DRAG)
                        {
                            if (handleMouseDrag(
                                mouseSelectionElement,
                                mouseSelectionAnchor,
                                event.position()))
                            {
                                dirty = true;
                            }
                            continue;
                        }
                        if (event.type() == EventType.MOUSE_RELEASE
                            && mouseSelectionElement != null)
                        {
                            String selectedText = selectedText(mouseSelectionElement);
                            if (!selectedText.isEmpty())
                            {
                                RuntimeClipboard.copyTextToClipboard(selectedText);
                                notification = COPY_NOTIFICATION;
                                notificationExpiresAt = System.nanoTime()
                                    + COPY_NOTIFICATION_DURATION_NANOS;
                            }
                            mouseSelectionElement = null;
                            mouseSelectionAnchor = 0;
                            dirty = true;
                            continue;
                        }

                        Element mouseTarget = null;
                        if (event.type() == EventType.MOUSE_PRESS)
                        {
                            clearLabelSelectionsInWindow(activeWindow);
                            MouseSelectionAnchor labelSelection = handleLabelMousePress(
                                activeWindow,
                                event.position());
                            if (labelSelection != null)
                            {
                                mouseSelectionElement = labelSelection.element();
                                mouseSelectionAnchor = labelSelection.position();
                                dirty = true;
                                continue;
                            }
                            mouseTarget = elementAt(
                                focusableElements(activeWindow, activeFrame.activeScrollView()),
                                event.position());
                        }
                        if (event.type() == EventType.KEY)
                        {
                            clearLabelSelectionsInWindow(activeWindow);
                        }
                        if (event.type() == EventType.KEY && "Ctrl+C".equals(event.key()))
                        {
                            running = false;
                            break;
                        }
                        Event dispatchedEvent = event;
                        if (event.type() == EventType.KEY && "cmd_c".equals(event.key()))
                        {
                            if (copyFocusedText(focusedElement(activeFrame)))
                            {
                                notification = COPY_NOTIFICATION;
                                notificationExpiresAt = System.nanoTime()
                                    + COPY_NOTIFICATION_DURATION_NANOS;
                                dirty = true;
                            }
                            continue;
                        }
                        if (event.type() == EventType.KEY && "cmd_v".equals(event.key()))
                        {
                            dispatchedEvent = Event.paste(RuntimeClipboard.runtimeClipboardText());
                        }
                        final Size flashTerminalSize = terminalSize;
                        final Rect flashViewport = activeViewport;
                        final String flashNotification = notification;
                        Runnable beforeStandardEscapeAction = () ->
                        {
                            renderToTerminal(
                                window,
                                options,
                                flashTerminalSize,
                                new RuntimeState(rootFrame),
                                terminalBuffer,
                                terminal,
                                flashViewport,
                                flashNotification);
                            sleepDialogButtonCloseDelay();
                        };
                        if (options.windowStack().topFrame() == null)
                        {
                            dispatchFrameEvent(
                                rootFrame,
                                dispatchedEvent,
                                beforeStandardEscapeAction);
                        }
                        else
                        {
                            dispatchWindowStackEvent(
                                options.windowStack(),
                                dispatchedEvent,
                                beforeStandardEscapeAction);
                        }
                        if (event.type() == EventType.MOUSE_PRESS
                            && mouseTarget instanceof TextInput textInput)
                        {
                            mouseSelectionElement = textInput;
                            mouseSelectionAnchor = textInput.cursor();
                        }
                        dirty = true;
                    }
                }
            }
            return EXIT_SUCCESS;
        }
        catch (IOException exception)
        {
            throw new IllegalStateException("terminal input failed", exception);
        }
        finally
        {
            if (mcpServer != null)
            {
                mcpServer.stop();
            }
            try
            {
                Runtime.getRuntime().removeShutdownHook(shutdownHook);
            }
            catch (IllegalStateException ignored)
            {
                // JVM shutdown is already running; the hook owns terminal restoration.
            }
            restoreTerminal.run();
        }
    }

    private static List<Event> readRuntimeEvents(
        TerminalBackend terminal,
        boolean nonBlocking) throws IOException
    {
        if (nonBlocking && !terminal.inputAvailable())
        {
            return List.of();
        }
        return terminal.readEvents();
    }

    private static void idleWait()
    {
        try
        {
            Thread.sleep(INPUT_IDLE_SLEEP_MILLISECONDS);
        }
        catch (InterruptedException exception)
        {
            Thread.currentThread().interrupt();
        }
    }

    private static boolean sameViewportOrigin(Rect first, Rect second)
    {
        if (first == null || second == null)
        {
            return first == second;
        }
        return first.row() == second.row() && first.col() == second.col();
    }

    private static void serveHeadlessMcp(
        McpController controller,
        McpRuntimeConfig config)
    {
        switch (config.transport())
        {
            case McpRuntimeConfig.TRANSPORT_STDIO -> new McpStdioServer(controller).serveForever();
            case McpRuntimeConfig.TRANSPORT_TCP ->
            {
                try (McpTcpServer server = new McpTcpServer(controller, config))
                {
                    server.serveForever();
                }
            }
            case McpRuntimeConfig.TRANSPORT_HTTP ->
            {
                try (McpHttpServer server = new McpHttpServer(controller, config))
                {
                    server.serveForever();
                }
            }
            default -> throw new IllegalArgumentException(
                "Unsupported Java MCP transport: " + config.transport());
        }
    }

    private static McpSocketServer startGuiMcp(
        McpController controller,
        McpRuntimeConfig config)
    {
        if (!config.enabled())
        {
            return null;
        }
        McpSocketServer server = switch (config.transport())
        {
            case McpRuntimeConfig.TRANSPORT_TCP -> new McpTcpServer(controller, config);
            case McpRuntimeConfig.TRANSPORT_HTTP -> new McpHttpServer(controller, config);
            default -> throw new IllegalArgumentException(
                "Unsupported Java MCP GUI transport: " + config.transport());
        };
        server.start();
        return server;
    }

    public static boolean dispatchWindowStackEvent(
        GeneratedWindowStack stack,
        Event event)
    {
        return dispatchWindowStackEvent(stack, event, null);
    }

    static boolean dispatchWindowStackEvent(
        GeneratedWindowStack stack,
        Event event,
        Runnable beforeStandardEscapeAction)
    {
        if (stack == null || event == null)
        {
            return false;
        }
        GeneratedWindowStackFrame frame = stack.topFrame();
        if (frame == null)
        {
            return false;
        }
        prepareStandardEscapeAction(frame, event, beforeStandardEscapeAction);
        RuntimeState state = new RuntimeState(frame);
        dispatchEvent(frame.window(), frame.options(), event, state, false);
        normalizeState(frame.window(), state);
        if (stack.frames().contains(frame))
        {
            state.writeBack(frame);
            if (frame.options().shouldClose() != null
                && frame.options().shouldClose().getAsBoolean())
            {
                stack.remove(frame.window());
            }
        }
        return true;
    }

    static GeneratedWindowStackFrame createFrame(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options)
    {
        List<Element> focusable = focusableElements(window);
        int focusedIndex = initialFocusIndex(
            focusable,
            options.initialFocusName(),
            options.startInEditMode());
        Element requestedFocus = window.consumeRequestedFocusElement();
        if (requestedFocus != null)
        {
            int requestedIndex = focusable.indexOf(requestedFocus);
            if (requestedIndex >= 0)
            {
                focusedIndex = requestedIndex;
            }
        }
        GeneratedWindowStackFrame frame = new GeneratedWindowStackFrame(
            window,
            options,
            focusedIndex,
            options.startInEditMode());
        beginFrameEditIfRequested(frame);
        return frame;
    }

    static boolean dispatchFrameEvent(
        GeneratedWindowStackFrame frame,
        Event event)
    {
        return dispatchFrameEvent(frame, event, null);
    }

    private static boolean dispatchFrameEvent(
        GeneratedWindowStackFrame frame,
        Event event,
        Runnable beforeStandardEscapeAction)
    {
        if (frame == null || event == null)
        {
            return false;
        }
        prepareStandardEscapeAction(frame, event, beforeStandardEscapeAction);
        RuntimeState state = new RuntimeState(frame);
        dispatchEvent(frame.window(), frame.options(), event, state, true);
        normalizeState(frame.window(), state);
        state.writeBack(frame);
        return true;
    }

    private static void prepareStandardEscapeAction(
        GeneratedWindowStackFrame frame,
        Event event,
        Runnable beforeStandardEscapeAction)
    {
        if (event.type() != EventType.KEY
            || !"Escape".equals(event.key())
            || frame.editMode())
        {
            return;
        }
        Button button = standardEscapeButton(
            frame.window(),
            frame.options().className());
        if (button == null)
        {
            return;
        }
        List<Element> focusable = focusableElements(frame.window());
        int index = focusable.indexOf(button);
        if (index < 0)
        {
            return;
        }
        frame.setEditMode(false);
        frame.setActiveScrollView(null);
        frame.setActiveScrollViewEditElement(null);
        frame.setEditSnapshot(null);
        frame.setFocusedIndex(index);
        ensureElementVisibleInScrollView(
            scrollViewContainingElement(frame.window(), button),
            button);
        if (beforeStandardEscapeAction != null)
        {
            beforeStandardEscapeAction.run();
        }
    }

    private static Button standardEscapeButton(
        GeneratedWindowBase window,
        String configuredClassName)
    {
        String className = configuredClassName == null || configuredClassName.isEmpty()
            ? window.mcpClassName()
            : configuredClassName;
        if ("FileBrowser".equals(className))
        {
            Element closeButton = findElement(window, "close_btn");
            return closeButton instanceof Button button ? button : null;
        }
        if (className == null || !className.startsWith("MessageBox"))
        {
            return null;
        }
        for (String name : List.of("cancel_btn", "no_btn", "ok_btn"))
        {
            Element element = findElement(window, name);
            if (element instanceof Button button)
            {
                return button;
            }
        }
        return null;
    }

    private static void sleepDialogButtonCloseDelay()
    {
        try
        {
            Thread.sleep(DIALOG_BUTTON_CLOSE_DELAY_MILLISECONDS);
        }
        catch (InterruptedException exception)
        {
            Thread.currentThread().interrupt();
        }
    }

    static void focusFrameElement(GeneratedWindowStackFrame frame, Element element)
    {
        if (frame == null)
        {
            return;
        }
        RuntimeState state = new RuntimeState(frame);
        GeneratedWindowBase window = frame.window();
        state.suppressActiveScrollViewScopeVisuals = false;
        boolean activeScrollViewBelongsToWindow = state.activeScrollView != null
            && windowContainsElement(window, state.activeScrollView);
        List<Element> previousFocusable = focusableElements(
            window,
            activeScrollViewBelongsToWindow ? state.activeScrollView : null);
        Element previous = focusedElement(previousFocusable, state.focusedIndex);
        ScrollView containingScrollView = scrollViewContainingElement(window, element);
        if (activeScrollViewBelongsToWindow && state.editMode)
        {
            if (state.activeScrollViewEditElement != null)
            {
                commitEdit(state.activeScrollViewEditElement);
                dispatchChanged(window, frame.options(), state.activeScrollViewEditElement);
            }
            state.editSnapshot = null;
            state.activeScrollViewEditElement = null;
            state.editMode = false;
        }
        Element activeScrollViewProxy = activeScrollViewBelongsToWindow
            ? generatedScrollViewProxyFor(window, state.activeScrollView)
            : null;
        if (activeScrollViewBelongsToWindow
            && (element == state.activeScrollView || element == activeScrollViewProxy))
        {
            state.activeScrollView = null;
            state.activeScrollViewEditElement = null;
            state.editMode = false;
        }
        if (activeScrollViewBelongsToWindow
            && element != state.activeScrollView
            && containingScrollView != state.activeScrollView)
        {
            state.activeScrollView = null;
            state.activeScrollViewEditElement = null;
            state.editMode = false;
        }
        if (containingScrollView != null)
        {
            state.activeScrollView = containingScrollView;
            state.activeScrollViewEditElement = null;
            state.editMode = false;
        }
        boolean targetScrollViewBelongsToWindow = state.activeScrollView != null
            && windowContainsElement(window, state.activeScrollView);
        List<Element> focusable = focusableElements(
            window,
            targetScrollViewBelongsToWindow ? state.activeScrollView : null);
        int index = focusable.indexOf(element);
        if (index >= 0)
        {
            state.focusedIndex = index;
            ensureElementVisibleInScrollView(containingScrollView, element);
            if (state.activeScrollView != null
                && containingScrollView == state.activeScrollView)
            {
                rememberScrollViewDescendant(state, element);
            }
        }
        else if (previous != null)
        {
            state.focusedIndex = -1;
            state.editMode = false;
        }
        if (previous != element && index >= 0)
        {
            notifyFocusChanged(frame.options(), previous, false);
            notifyFocusChanged(frame.options(), element, true);
        }
        else if (previous != null && index < 0)
        {
            notifyFocusChanged(frame.options(), previous, false);
        }
        normalizeState(window, state);
        state.writeBack(frame);
    }

    static void beginFrameElementEdit(GeneratedWindowStackFrame frame, Element element)
    {
        if (frame == null || !isEditableElement(element))
        {
            return;
        }
        focusFrameElement(frame, element);
        RuntimeState state = new RuntimeState(frame);
        if (element instanceof ScrollView
            || element instanceof ReusableElement reusable
                && reusable.child() != null
                && reusable.child().generatedScrollView() != null)
        {
            enterScrollViewScope(frame.window(), state);
        }
        else
        {
            state.editSnapshot = captureSnapshot(element);
            beginElementEdit(element);
            state.editMode = true;
            if (state.activeScrollView != null)
            {
                state.activeScrollViewEditElement = element;
            }
            if (frame.options().onEditStarted() != null)
            {
                frame.options().onEditStarted().accept(element.name());
            }
        }
        normalizeState(frame.window(), state);
        state.writeBack(frame);
    }

    static void prepareFrameElementEdit(GeneratedWindowStackFrame frame, Element element)
    {
        if (frame == null || !isEditableElement(element))
        {
            return;
        }
        List<Element> focusable = focusableElements(frame.window(), frame.activeScrollView());
        Element focused = focusedElement(focusable, frame.focusedIndex());
        boolean targetAlreadyEditing = frame.editMode()
            && focused == element
            && (frame.activeScrollView() == null
                || frame.activeScrollViewEditElement() == element);
        if (!targetAlreadyEditing)
        {
            beginFrameElementEdit(frame, element);
        }
    }

    static void commitFrameElementEdit(GeneratedWindowStackFrame frame)
    {
        if (frame == null || !frame.editMode() || frame.activeScrollView() != null)
        {
            return;
        }
        RuntimeState state = new RuntimeState(frame);
        List<Element> focusable = focusableElements(frame.window());
        Element focused = focusedElement(focusable, state.focusedIndex);
        commitEdit(focused);
        state.editSnapshot = null;
        state.activeScrollViewEditElement = null;
        state.editMode = false;
        normalizeState(frame.window(), state);
        state.writeBack(frame);
    }

    static void notifyFrameChanged(GeneratedWindowStackFrame frame, Element element)
    {
        if (frame != null && element != null)
        {
            dispatchChanged(frame.window(), frame.options(), element);
        }
    }

    static void notifyFrameConfirmed(GeneratedWindowStackFrame frame, Element element)
    {
        if (frame != null && element != null)
        {
            dispatchConfirmed(frame.window(), frame.options(), element);
        }
    }

    static boolean editableElement(Element element)
    {
        return isEditableElement(element);
    }

    private static void normalizeState(GeneratedWindowBase window, RuntimeState state)
    {
        clearInvalidActiveScrollViewScope(window, state);
        List<Element> focusable = focusableElements(window, state.activeScrollView);
        if (focusable.isEmpty())
        {
            state.focusedIndex = -1;
            state.editMode = false;
            state.activeScrollViewEditElement = null;
            state.editSnapshot = null;
            return;
        }
        if (state.focusedIndex < -1 || state.focusedIndex >= focusable.size())
        {
            state.focusedIndex = -1;
        }
    }

    private static void dispatchEvent(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        Event event,
        RuntimeState state,
        boolean allowApplicationMouseWheel)
    {
        clearInvalidActiveScrollViewScope(window, state);
        List<Element> focusable = focusableElements(window, state.activeScrollView);
        Element focused = focusedElement(focusable, state.focusedIndex);
        if (event.type() == EventType.MOUSE_WHEEL)
        {
            if (options.onMouseWheelBeforeFocused() != null
                && options.onMouseWheelBeforeFocused().test(event.position(), event.wheelDelta()))
            {
                return;
            }
            boolean handled = false;
            ScrollView scrollView = scrollViewAtPosition(window, event.position());
            if (scrollView != null)
            {
                handled = scrollView.handleWheel(
                    event.wheelDelta(),
                    new Size(scrollView.frame().width(), scrollView.frame().height()));
            }
            else
            {
                Element target = elementAt(focusable, event.position());
                if (target instanceof ListBox listBox)
                {
                    listBox.scrollLines(event.wheelDelta());
                    handled = true;
                }
                else if (target instanceof TextInput input && input.multiline())
                {
                    handled = input.scrollByRows(
                        -event.wheelDelta(),
                        target.frame().height());
                }
            }
            if (!handled
                && allowApplicationMouseWheel
                && options.onMouseWheel() != null)
            {
                Element target = elementAt(layoutElements(window), event.position());
                if (target != null)
                {
                    options.onMouseWheel().test(target.name(), event.wheelDelta());
                }
            }
            return;
        }
        if (event.type() == EventType.MOUSE_PRESS)
        {
            if (options.onMousePressBeforeFocused() != null
                && options.onMousePressBeforeFocused().test(event.position()))
            {
                return;
            }
            state.suppressActiveScrollViewScopeVisuals = false;
            if (state.activeScrollView != null
                && !state.activeScrollView.frame().contains(event.position()))
            {
                exitScrollViewScope(window, state);
                focusable = focusableElements(window);
            }
            ScrollView mouseScrollView = state.activeScrollView == null
                ? scrollViewAtPosition(window, event.position())
                : state.activeScrollView;
            focusable = focusableElements(window, mouseScrollView);
            Element target = mouseTargetElement(
                focusable,
                focused,
                state.editMode,
                event.position(),
                state.activeScrollViewEditElement);
            if (mouseScrollView != null
                && target != null
                && target != mouseScrollView
                && target != generatedScrollViewProxyFor(window, mouseScrollView))
            {
                state.activeScrollView = mouseScrollView;
                state.editMode = true;
                state.focusedIndex = focusable.indexOf(target);
                rememberScrollViewDescendant(state, target);
            }
            boolean scrollViewScopeActive = state.editMode && state.activeScrollView != null;
            if (target == null)
            {
                clearFocusForEmptyMouseTarget(options, focusable, state);
                return;
            }
            if (state.editMode
                && !scrollViewScopeActive
                && focused != null
                && focused != target)
            {
                commitEdit(focused);
                state.editSnapshot = null;
                state.editMode = false;
            }
            int targetIndex = focusable.indexOf(target);
            if (targetIndex >= 0)
            {
                if (state.activeScrollViewEditElement != null
                    && state.activeScrollViewEditElement != target)
                {
                    commitEdit(state.activeScrollViewEditElement);
                    state.activeScrollViewEditElement = null;
                    state.editSnapshot = null;
                }
                if (targetIndex != state.focusedIndex)
                {
                    changeFocus(options, focusable, state, targetIndex);
                }
                if (target instanceof ListBox listBox)
                {
                    int localRow = event.position().row() - target.frame().row();
                    int optionIndex = listBox.scrollOffset() + localRow;
                    if (optionIndex < 0 || optionIndex >= listBox.options().size())
                    {
                        return;
                    }
                    int previousSelection = listBox.selectedIndex();
                    List<String> previousValues = new ArrayList<>(listBox.selectedValues());
                    if (listBox.multiple())
                    {
                        List<Integer> nextIndices = new ArrayList<>(listBox.selectedIndices());
                        if (nextIndices.contains(optionIndex))
                        {
                            nextIndices.remove(Integer.valueOf(optionIndex));
                        }
                        else
                        {
                            nextIndices.add(optionIndex);
                        }
                        listBox.setSelectedIndices(nextIndices);
                    }
                    else
                    {
                        listBox.setSelectedIndex(optionIndex);
                    }
                    listBox.setActiveIndex(optionIndex);
                    listBox.hideActiveItem();
                    state.editSnapshot = captureSnapshot(target);
                    state.editMode = true;
                    if (scrollViewScopeActive)
                    {
                        state.activeScrollViewEditElement = target;
                    }
                    if (options.onEditStarted() != null)
                    {
                        options.onEditStarted().accept(target.name());
                    }
                    dispatchChangedAfterKey(
                        window,
                        options,
                        target,
                        previousSelection,
                        previousValues);
                    return;
                }
                if (target instanceof ComboBox comboBox)
                {
                    boolean comboEditMode = scrollViewScopeActive
                        ? state.activeScrollViewEditElement == target
                        : state.editMode;
                    int localRow = event.position().row() - target.frame().row();
                    if (comboEditMode && localRow > 0)
                    {
                        int previousSelection = comboBox.selectedIndex();
                        comboBox.setSelectedIndex(comboBoxOptionIndexForLocalRow(comboBox, localRow));
                        commitEdit(target);
                        state.editSnapshot = null;
                        dispatchConfirmed(window, options, target);
                        if (scrollViewScopeActive)
                        {
                            state.activeScrollViewEditElement = null;
                            exitScrollViewScope(window, state);
                        }
                        else
                        {
                            state.editMode = false;
                        }
                        dispatchChangedAfterKey(
                            window,
                            options,
                            target,
                            previousSelection,
                            null);
                    }
                    else if (!comboEditMode)
                    {
                        activate(window, options, target, state);
                    }
                    return;
                }
                activate(window, options, target, state);
                if (target instanceof TextInput input)
                {
                    Point point = event.position();
                    input.setCursor(input.cursorForPoint(
                        point.row() - target.frame().row(),
                        point.col() - target.frame().col(),
                        new Size(target.frame().width(), target.frame().height())));
                }
            }
            return;
        }
        if (event.type() == EventType.PASTE)
        {
            Element target = state.activeScrollViewEditElement == null
                ? focused
                : state.activeScrollViewEditElement;
            if (target != null && !state.editMode && isEditableElement(target))
            {
                state.editSnapshot = captureSnapshot(target);
                beginElementEdit(target);
                state.editMode = true;
                if (options.onEditStarted() != null)
                {
                    options.onEditStarted().accept(target.name());
                }
            }
            if (pasteIntoFocused(target, event.text()))
            {
                dispatchChanged(window, options, target);
            }
            return;
        }
        if (event.type() != EventType.KEY)
        {
            return;
        }

        String key = event.key();
        if (!"Escape".equals(key))
        {
            state.suppressActiveScrollViewScopeVisuals = false;
        }
        if ("Escape".equals(key) && state.activeScrollView != null)
        {
            if (state.activeScrollViewEditElement != null)
            {
                escapeEdit(options, state, state.activeScrollViewEditElement);
            }
            else
            {
                exitScrollViewScope(window, state);
            }
            return;
        }
        if (state.editMode && "Escape".equals(key))
        {
            escapeEdit(options, state, focused);
            return;
        }
        if (options.onKeyBeforeFocusedElement() != null
            && options.onKeyBeforeFocusedElement().handle(
                key,
                focused == null ? "" : focused.name(),
                state.editMode))
        {
            return;
        }
        if (options.onKeyBeforeFocused() != null && options.onKeyBeforeFocused().test(key))
        {
            return;
        }
        if (state.activeScrollView != null)
        {
            focused = focusedElement(focusable, state.focusedIndex);
            if (state.activeScrollViewEditElement != null)
            {
                handleEditKey(window, options, state, state.activeScrollViewEditElement, key);
                return;
            }
            if (isArrowKey(key))
            {
                moveScrollViewScopeFocus(window, state, key);
                return;
            }
            if (("Enter".equals(key) || " ".equals(key)) && focused != null)
            {
                activate(window, options, focused, state);
            }
            return;
        }
        if (state.editMode && focused != null)
        {
            handleEditKey(window, options, state, focused, key);
            return;
        }
        if (options.onKey() != null && options.onKey().test(key))
        {
            return;
        }
        if (focusable.isEmpty())
        {
            state.focusedIndex = -1;
            return;
        }
        if ("Tab".equals(key))
        {
            changeFocus(options, focusable, state, Math.floorMod(state.focusedIndex + 1, focusable.size()));
            return;
        }
        if ("Shift+Tab".equals(key))
        {
            changeFocus(options, focusable, state, Math.floorMod(state.focusedIndex - 1, focusable.size()));
            return;
        }
        if ("Up".equals(key) || "Down".equals(key) || "Left".equals(key) || "Right".equals(key))
        {
            changeFocus(options, focusable, state, moveSpatial(focusable, state.focusedIndex, key));
            return;
        }
        if (("Enter".equals(key) || " ".equals(key)) && focused != null)
        {
            activate(window, options, focused, state);
            return;
        }
        if (isImmediateInput(focused))
        {
            int previousSelection = selectedIndexOf(focused);
            List<String> previousValues = selectionValues(focused);
            if (focused.handleKey(key))
            {
                dispatchChangedAfterKey(window, options, focused, previousSelection, previousValues);
            }
        }
    }

    private static void activate(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        Element element,
        RuntimeState state)
    {
        if (element instanceof Button)
        {
            dispatchButton(window, options, element);
            state.editMode = state.activeScrollView != null;
            state.activeScrollViewEditElement = null;
            state.editSnapshot = null;
        }
        else if (activateReusableControl(element))
        {
            state.editMode = state.activeScrollView != null;
            state.activeScrollViewEditElement = null;
            state.editSnapshot = null;
        }
        else if (isImmediateInput(element))
        {
            int previousSelection = selectedIndexOf(element);
            List<String> previousValues = selectionValues(element);
            if (element.handleKey("Enter"))
            {
                dispatchChangedAfterKey(window, options, element, previousSelection, previousValues);
            }
        }
        else if (isEditableElement(element))
        {
            if (state.activeScrollView == null && enterScrollViewScope(window, state))
            {
                state.activeScrollViewEditElement = null;
                state.editSnapshot = null;
                return;
            }
            state.editSnapshot = captureSnapshot(element);
            beginElementEdit(element);
            state.editMode = true;
            if (state.activeScrollView != null)
            {
                state.activeScrollViewEditElement = element;
            }
            if (options.onEditStarted() != null)
            {
                options.onEditStarted().accept(element.name());
            }
        }
    }

    private static void handleEditKey(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        RuntimeState state,
        Element focused,
        String key)
    {
        int previousSelection = selectedIndexOf(focused);
        List<String> previousValues = selectionValues(focused);
        boolean handled = focused.handleKey(key);
        if (handled)
        {
            dispatchChangedAfterKey(window, options, focused, previousSelection, previousValues);
        }
        if (!"Enter".equals(key))
        {
            return;
        }
        if (focused instanceof ListBox listBox)
        {
            if (listBox.multiple())
            {
                return;
            }
        }
        else if (usesLeaveCommit(focused))
        {
            return;
        }
        dispatchConfirmed(window, options, focused);
        commitEdit(focused);
        if (state.activeScrollView != null)
        {
            state.activeScrollViewEditElement = null;
            state.editSnapshot = null;
            state.editMode = true;
            return;
        }
        state.editMode = options.keepEditModeAfterConfirm() && isEditableElement(focused);
        if (state.editMode)
        {
            state.editSnapshot = captureSnapshot(focused);
            beginElementEdit(focused);
            if (options.onEditStarted() != null)
            {
                options.onEditStarted().accept(focused.name());
            }
        }
        else
        {
            state.editSnapshot = null;
        }
    }

    private static void escapeEdit(
        GeneratedWindowRuntimeOptions options,
        RuntimeState state,
        Element focused)
    {
        if (focused != null && usesLeaveCommit(focused))
        {
            commitEdit(focused);
        }
        else if (state.editSnapshot != null)
        {
            restoreSnapshot(state.editSnapshot);
        }
        state.editSnapshot = null;
        state.activeScrollViewEditElement = null;
        state.editMode = state.activeScrollView != null;
        if (state.activeScrollView == null
            && options.keepEditModeAfterEscape()
            && focused != null
            && isEditableElement(focused))
        {
            state.editSnapshot = captureSnapshot(focused);
            beginElementEdit(focused);
            state.editMode = true;
            if (options.onEditStarted() != null)
            {
                options.onEditStarted().accept(focused.name());
            }
        }
    }

    private static EditSnapshot captureSnapshot(Element element)
    {
        EditSnapshot snapshot = new EditSnapshot(element);
        if (element instanceof TextInput input)
        {
            snapshot.textValue = input.value();
            snapshot.textCursor = input.cursor();
            if (!input.selectedText().isEmpty())
            {
                snapshot.textSelectionAnchor = input.cursor() == input.selectionStart()
                    ? input.selectionEnd()
                    : input.selectionStart();
            }
        }
        else if (element instanceof NumberInput input)
        {
            snapshot.numberValue = input.value();
            input.beginEdit();
        }
        else if (element instanceof ComboBox comboBox)
        {
            snapshot.selectedIndex = comboBox.selectedIndex();
        }
        else if (element instanceof ListBox listBox)
        {
            snapshot.selectedIndex = listBox.selectedIndex();
        }
        return snapshot;
    }

    private static void beginElementEdit(Element element)
    {
        if (element instanceof TextInput input)
        {
            input.setCursor(input.value().length());
        }
        else if (element instanceof NumberInput input)
        {
            input.beginEdit();
        }
        else if (element instanceof ListBox listBox)
        {
            listBox.hideActiveItem();
        }
    }

    private static void restoreSnapshot(EditSnapshot snapshot)
    {
        if (snapshot.element instanceof TextInput input)
        {
            input.setValue(snapshot.textValue);
            input.selectRange(
                snapshot.textSelectionAnchor == null ? snapshot.textCursor : snapshot.textSelectionAnchor,
                snapshot.textCursor);
        }
        else if (snapshot.element instanceof NumberInput input)
        {
            input.cancelEdit();
            input.setValue(snapshot.numberValue);
        }
        else if (snapshot.element instanceof ComboBox comboBox)
        {
            comboBox.setSelectedIndex(snapshot.selectedIndex);
        }
        else if (snapshot.element instanceof ListBox listBox)
        {
            listBox.setSelectedIndex(snapshot.selectedIndex);
        }
    }

    private static void commitEdit(Element element)
    {
        if (element instanceof NumberInput input)
        {
            input.commitEdit();
        }
    }

    private static boolean pasteIntoFocused(Element focused, String text)
    {
        if (focused instanceof TextInput input)
        {
            input.insertText(text);
            return true;
        }
        if (focused instanceof NumberInput input)
        {
            String value = text == null ? "" : text;
            value.codePoints()
                .filter(codePoint -> codePoint != '\r' && codePoint != '\n')
                .mapToObj(Character::toString)
                .forEach(input::handleKey);
            return true;
        }
        return false;
    }

    private static boolean copyFocusedText(Element focused)
    {
        if (focused instanceof TextInput input)
        {
            String selected = input.selectedText();
            return RuntimeClipboard.copyTextToClipboard(
                selected.isEmpty() ? input.value() : selected);
        }
        if (focused instanceof NumberInput input)
        {
            return RuntimeClipboard.copyTextToClipboard(input.displayText());
        }
        return false;
    }

    private static String selectedText(Element element)
    {
        if (element instanceof TextInput input)
        {
            return input.selectedText();
        }
        if (element instanceof Label label)
        {
            return label.selectedText();
        }
        return "";
    }

    private static void clearLabelSelectionsInWindow(GeneratedWindowBase window)
    {
        for (Element element : window.elements())
        {
            if (element instanceof Label label)
            {
                label.clearSelection();
            }
            else if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                clearLabelSelectionsInWindow(reusable.child());
            }
        }
    }

    private static MouseSelectionAnchor handleLabelMousePress(
        GeneratedWindowBase window,
        Point position)
    {
        for (Element element : window.elements())
        {
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                MouseSelectionAnchor nested = handleLabelMousePress(reusable.child(), position);
                if (nested != null)
                {
                    return nested;
                }
            }
            if (!(element instanceof Label label)
                || !"text".equals(label.effectiveStyle(false, false).userSelect())
                || !label.frame().contains(position))
            {
                continue;
            }
            Rect frame = label.frame();
            int textPosition = label.textPositionFromPoint(
                position.row() - frame.row(),
                position.col() - frame.col(),
                new Size(frame.width(), frame.height()));
            label.selectRange(textPosition, textPosition);
            return new MouseSelectionAnchor(label, textPosition);
        }
        return null;
    }

    private static boolean handleMouseDrag(
        Element mouseSelectionElement,
        int mouseSelectionAnchor,
        Point position)
    {
        if (mouseSelectionElement instanceof TextInput input)
        {
            Rect frame = input.frame();
            int localRow = position.row() - frame.row();
            int localCol = position.col() - frame.col();
            if (input.multiline() && frame.height() > MINIMUM_RENDERABLE_SIZE)
            {
                if (localRow < 0)
                {
                    input.scrollByRows(-1, frame.height(), false);
                    localRow = 0;
                }
                else if (localRow >= frame.height())
                {
                    input.scrollByRows(1, frame.height(), false);
                    localRow = frame.height() - 1;
                }
            }
            int cursor = input.cursorForPoint(
                localRow,
                localCol,
                new Size(frame.width(), frame.height()));
            input.selectRange(mouseSelectionAnchor, cursor);
            return true;
        }
        if (mouseSelectionElement instanceof Label label)
        {
            Rect frame = label.frame();
            int cursor = label.textPositionFromPoint(
                position.row() - frame.row(),
                position.col() - frame.col(),
                new Size(frame.width(), frame.height()));
            label.selectRange(mouseSelectionAnchor, cursor);
            return true;
        }
        return false;
    }

    private static void dispatchChangedAfterKey(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        Element element,
        int previousSelection,
        List<String> previousValues)
    {
        if (element instanceof ListBox listBox && previousValues != null)
        {
            if (!previousValues.equals(listBox.selectedValues()))
            {
                dispatchChanged(window, options, element);
            }
            return;
        }
        if ((element instanceof ComboBox || element instanceof ListBox)
            && selectedIndexOf(element) == previousSelection)
        {
            return;
        }
        dispatchChanged(window, options, element);
    }

    private static void dispatchChanged(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        Element element)
    {
        GeneratedWindowBase owner = ownerWindowForElement(window, element);
        if (owner != null && owner != window)
        {
            if (element instanceof ComboBox comboBox)
            {
                owner.handleGeneratedSelectionChanged(comboBox.name(), List.of(comboBox.selectedText()));
            }
            else if (element instanceof ListBox listBox)
            {
                owner.handleGeneratedSelectionChanged(listBox.name(), listBox.selectedValues());
            }
            else
            {
                owner.handleGeneratedTextChanged(element.name(), textValueOf(element));
            }
            return;
        }
        if (element instanceof TextInput input && options.onTextChanged() != null)
        {
            options.onTextChanged().accept(input.name(), input.value());
        }
        else if (element instanceof NumberInput input && options.onTextChanged() != null)
        {
            options.onTextChanged().accept(input.name(), Double.toString(input.value()));
        }
        else if (element instanceof CheckBox checkBox && options.onTextChanged() != null)
        {
            options.onTextChanged().accept(checkBox.name(), Boolean.toString(checkBox.checked()));
        }
        else if (element instanceof ComboBox comboBox && options.onSelectionChanged() != null)
        {
            options.onSelectionChanged().accept(comboBox.name(), List.of(comboBox.selectedText()));
        }
        else if (element instanceof ListBox listBox && options.onSelectionChanged() != null)
        {
            options.onSelectionChanged().accept(listBox.name(), listBox.selectedValues());
        }
    }

    private static void dispatchConfirmed(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        Element element)
    {
        GeneratedWindowBase owner = ownerWindowForElement(window, element);
        if (owner != null && owner != window)
        {
            owner.handleGeneratedTextConfirmed(element.name(), textValueOf(element));
            return;
        }
        if (options.onTextConfirmed() != null
            && (element instanceof TextInput
                || element instanceof NumberInput
                || element instanceof ComboBox
                || element instanceof ListBox))
        {
            options.onTextConfirmed().accept(element.name(), textValueOf(element));
        }
    }

    private static String textValueOf(Element element)
    {
        if (element instanceof TextInput input)
        {
            return input.value();
        }
        if (element instanceof NumberInput input)
        {
            return Double.toString(input.value());
        }
        if (element instanceof ComboBox comboBox)
        {
            return comboBox.selectedText();
        }
        if (element instanceof ListBox listBox)
        {
            List<String> values = listBox.selectedValues();
            return values.isEmpty() ? "" : values.get(0);
        }
        if (element instanceof CheckBox checkBox)
        {
            return Boolean.toString(checkBox.checked());
        }
        return "";
    }

    private static boolean isEditableElement(Element element)
    {
        if (element instanceof ScrollView)
        {
            return element.enabled();
        }
        if (element instanceof ReusableElement reusable
            && reusable.child() != null
            && reusable.child().generatedScrollView() != null)
        {
            return element.enabled();
        }
        return element instanceof TextInput
            || element instanceof NumberInput
            || element instanceof ComboBox
            || element instanceof ListBox;
    }

    private static boolean isImmediateInput(Element element)
    {
        return element instanceof CheckBox;
    }

    private static boolean activateReusableControl(Element element)
    {
        return element instanceof ReusableElement reusable
            && reusable.enabled()
            && reusable.child() != null
            && reusable.child().generatedFocusable()
            && reusable.child().activateGeneratedControl();
    }

    private static void dispatchButton(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        Element element)
    {
        GeneratedWindowBase owner = ownerWindowForElement(window, element);
        if (owner != null && owner != window)
        {
            owner.handleGeneratedButton(element.name());
            return;
        }
        if (options.onButton() != null)
        {
            options.onButton().accept(element.name());
        }
        else
        {
            window.handleGeneratedButton(element.name());
        }
    }

    private static boolean usesLeaveCommit(Element element)
    {
        return element != null && Element.COMMIT_MODE_LEAVE.equals(element.commitMode());
    }

    static boolean windowHasAnimatedTextGradient(GeneratedWindowBase window)
    {
        if (styleHasAnimatedTextGradient(window.generatedWindowStyle()))
        {
            return true;
        }
        for (Element element : window.elements())
        {
            if (elementHasAnimatedTextGradient(element))
            {
                return true;
            }
        }
        return false;
    }

    private static boolean styleHasAnimatedTextGradient(Style style)
    {
        return style != null
            && (style.textColorGradient() != null || style.textBackgroundGradient() != null);
    }

    private static boolean elementHasAnimatedTextGradient(Element element)
    {
        if (styleHasAnimatedTextGradient(element.style())
            || styleHasAnimatedTextGradient(element.focusStyle())
            || styleHasAnimatedTextGradient(element.editStyle())
            || styleHasAnimatedTextGradient(element.cursorStyle())
            || styleHasAnimatedTextGradient(element.selectedStyle())
            || styleHasAnimatedTextGradient(element.activeStyle()))
        {
            return true;
        }
        for (Element child : element.children())
        {
            if (elementHasAnimatedTextGradient(child))
            {
                return true;
            }
        }
        return false;
    }

    private static int selectedIndexOf(Element element)
    {
        if (element instanceof ComboBox comboBox)
        {
            return comboBox.selectedIndex();
        }
        if (element instanceof ListBox listBox)
        {
            return listBox.selectedIndex();
        }
        return -1;
    }

    private static List<String> selectionValues(Element element)
    {
        return element instanceof ListBox listBox ? new ArrayList<>(listBox.selectedValues()) : null;
    }

    private static Element focusedElement(List<Element> elements, int index)
    {
        return index >= 0 && index < elements.size() ? elements.get(index) : null;
    }

    private static Element focusedElement(GeneratedWindowStackFrame frame)
    {
        if (frame.activeScrollViewEditElement() != null
            && windowContainsElement(frame.window(), frame.activeScrollViewEditElement()))
        {
            return frame.activeScrollViewEditElement();
        }
        return focusedElement(
            focusableElements(frame.window(), frame.activeScrollView()),
            frame.focusedIndex());
    }

    private static List<Element> layoutElements(GeneratedWindowBase window)
    {
        List<Element> elements = new ArrayList<>();
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            if (entry.name() == null || entry.name().isEmpty())
            {
                continue;
            }
            Element element = findElement(window, entry.name());
            if (element != null)
            {
                elements.add(element);
            }
        }
        return elements;
    }

    private static Element elementAt(List<Element> elements, Point position)
    {
        for (int index = elements.size() - 1; index >= 0; --index)
        {
            Element element = elements.get(index);
            if (mouseHitFrame(element).contains(position))
            {
                return element;
            }
        }
        return null;
    }

    private static Rect mouseHitFrame(Element element)
    {
        Rect frame = element.frame();
        if (element instanceof ComboBox)
        {
            return new Rect(frame.row(), frame.col(), frame.width(), COMBO_BOX_CLOSED_ROWS);
        }
        return frame;
    }

    static Point mouseClickPoint(Element element)
    {
        Rect frame = mouseHitFrame(element);
        return new Point(
            frame.row() + frame.height() / 2,
            frame.col() + frame.width() / 2);
    }

    private static Element mouseTargetElement(
        List<Element> elements,
        Element focused,
        boolean editMode,
        Point position,
        Element activeEditElement)
    {
        Element dropdown = activeEditElement == null ? focused : activeEditElement;
        if (editMode && dropdown instanceof ComboBox comboBox)
        {
            Rect frame = comboBox.frame();
            int visibleRows = Math.min(
                COMBO_BOX_DROPDOWN_ROWS,
                COMBO_BOX_CLOSED_ROWS + comboBox.options().size());
            Rect dropdownFrame = new Rect(
                frame.row(),
                frame.col(),
                frame.width(),
                Math.max(frame.height(), visibleRows));
            if (dropdownFrame.contains(position))
            {
                return dropdown;
            }
        }
        return elementAt(elements, position);
    }

    private static int comboBoxOptionIndexForLocalRow(ComboBox comboBox, int localRow)
    {
        return Math.max(0, Math.min(comboBox.options().size() - 1, localRow - 1));
    }

    private static void clearFocusForEmptyMouseTarget(
        GeneratedWindowRuntimeOptions options,
        List<Element> elements,
        RuntimeState state)
    {
        if (elements.size() <= 1)
        {
            return;
        }
        Element focused = focusedElement(elements, state.focusedIndex);
        Element editing = state.activeScrollViewEditElement == null
            ? focused
            : state.activeScrollViewEditElement;
        if (state.editMode)
        {
            commitEdit(editing);
            state.editSnapshot = null;
            state.editMode = false;
            state.activeScrollViewEditElement = null;
        }
        if (focused != null && options.onFocusChanged() != null)
        {
            options.onFocusChanged().accept(focused.name(), false);
        }
        state.focusedIndex = -1;
    }

    private static ScrollView generatedScrollViewForReusable(ReusableElement reusable)
    {
        return reusable == null || reusable.child() == null
            ? null
            : reusable.child().generatedScrollView();
    }

    private static Element generatedScrollViewProxyFor(
        GeneratedWindowBase window,
        ScrollView target)
    {
        if (target == null)
        {
            return null;
        }
        for (Element element : window.elements())
        {
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                syncReusableChildFrames(reusable);
                if (generatedScrollViewForReusable(reusable) == target)
                {
                    return reusable;
                }
                Element nested = generatedScrollViewProxyFor(reusable.child(), target);
                if (nested != null)
                {
                    return nested;
                }
            }
            if (!(element instanceof ScrollView scrollView))
            {
                continue;
            }
            syncScrollViewChildFrames(scrollView);
            for (Element child : scrollView.children())
            {
                if (!(child instanceof ReusableElement reusable) || reusable.child() == null)
                {
                    continue;
                }
                if (generatedScrollViewForReusable(reusable) == target)
                {
                    return reusable;
                }
                Element nested = generatedScrollViewProxyFor(reusable.child(), target);
                if (nested != null)
                {
                    return nested;
                }
            }
        }
        return null;
    }

    private static GeneratedWindowBase ownerWindowForElement(
        GeneratedWindowBase window,
        Element target)
    {
        if (target == null)
        {
            return null;
        }
        if (window.elements().contains(target))
        {
            return window;
        }
        for (Element element : window.elements())
        {
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                GeneratedWindowBase owner = ownerWindowForElement(reusable.child(), target);
                if (owner != null)
                {
                    return owner;
                }
            }
            if (!(element instanceof ScrollView scrollView))
            {
                continue;
            }
            for (Element child : scrollView.children())
            {
                if (child instanceof ReusableElement reusable && reusable.child() != null)
                {
                    GeneratedWindowBase owner = ownerWindowForElement(reusable.child(), target);
                    if (owner != null)
                    {
                        return owner;
                    }
                }
            }
        }
        return null;
    }

    private static boolean elementTreeContains(Element element, Element target)
    {
        if (element == target)
        {
            return true;
        }
        if (element instanceof ReusableElement reusable
            && reusable.child() != null
            && ownerWindowForElement(reusable.child(), target) != null)
        {
            return true;
        }
        for (Element child : element.children())
        {
            if (elementTreeContains(child, target))
            {
                return true;
            }
        }
        return false;
    }

    static boolean windowContainsElement(GeneratedWindowBase window, Element target)
    {
        for (Element element : window.elements())
        {
            if (elementTreeContains(element, target))
            {
                return true;
            }
        }
        return false;
    }

    private static ScrollView scrollViewAtPosition(GeneratedWindowBase window, Point position)
    {
        List<Element> elements = window.elements();
        for (int index = elements.size() - 1; index >= 0; --index)
        {
            Element element = elements.get(index);
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                syncReusableChildFrames(reusable);
                ScrollView nested = scrollViewAtPosition(reusable.child(), position);
                if (nested != null)
                {
                    return nested;
                }
            }
            if (element instanceof ScrollView scrollView && scrollView.frame().contains(position))
            {
                return scrollView;
            }
        }
        return null;
    }

    private static ScrollView scrollViewContainingElement(
        GeneratedWindowBase window,
        Element target)
    {
        if (window == null || target == null)
        {
            return null;
        }
        for (Element element : window.elements())
        {
            if (element instanceof ScrollView scrollView)
            {
                for (Element child : scrollView.children())
                {
                    if (elementTreeContains(child, target))
                    {
                        return scrollView;
                    }
                }
            }
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                ScrollView nested = scrollViewContainingElement(reusable.child(), target);
                if (nested != null)
                {
                    return nested;
                }
                ScrollView generated = generatedScrollViewForReusable(reusable);
                if (generated != null && generated != target)
                {
                    for (Element child : generated.children())
                    {
                        if (elementTreeContains(child, target))
                        {
                            return generated;
                        }
                    }
                }
            }
        }
        return null;
    }

    private static List<Element> scrollViewScopeElements(
        GeneratedWindowBase window,
        ScrollView scrollView)
    {
        List<Element> activeFocusable = focusableElements(window, scrollView);
        Element scopeRoot = generatedScrollViewProxyFor(window, scrollView);
        if (scopeRoot == null)
        {
            scopeRoot = scrollView;
        }
        int scrollIndex = activeFocusable.indexOf(scopeRoot);
        if (scrollIndex < 0)
        {
            return List.of();
        }
        Element nextNavigationElement = null;
        List<Element> navigationFocusable = focusableElements(window);
        int navigationIndex = navigationFocusable.indexOf(scopeRoot);
        if (navigationIndex >= 0 && navigationIndex + 1 < navigationFocusable.size())
        {
            nextNavigationElement = navigationFocusable.get(navigationIndex + 1);
        }
        List<Element> scoped = new ArrayList<>();
        for (int index = scrollIndex; index < activeFocusable.size(); ++index)
        {
            Element element = activeFocusable.get(index);
            if (index != scrollIndex && nextNavigationElement != null && element == nextNavigationElement)
            {
                break;
            }
            scoped.add(element);
        }
        return scoped;
    }

    private static Element firstFocusableDescendantInScrollView(
        GeneratedWindowBase window,
        ScrollView scrollView,
        RuntimeState state)
    {
        List<Element> scoped = scrollViewScopeElements(window, scrollView);
        Element scopeRoot = generatedScrollViewProxyFor(window, scrollView);
        Element remembered = state.scrollViewLastDescendant.get(scrollView);
        if (remembered != null && scoped.contains(remembered))
        {
            return remembered;
        }
        state.scrollViewLastDescendant.remove(scrollView);
        for (Element element : scoped)
        {
            if (element != scrollView
                && element != scopeRoot
                && elementVisibleInScrollView(scrollView, element))
            {
                return element;
            }
        }
        for (Element element : scoped)
        {
            if (element != scrollView && element != scopeRoot)
            {
                return element;
            }
        }
        return null;
    }

    private static boolean enterScrollViewScope(
        GeneratedWindowBase window,
        RuntimeState state)
    {
        List<Element> navigationFocusable = focusableElements(window);
        Element focused = focusedElement(navigationFocusable, state.focusedIndex);
        ScrollView scrollView = focused instanceof ScrollView direct
            ? direct
            : focused instanceof ReusableElement reusable
                ? generatedScrollViewForReusable(reusable)
                : null;
        if (scrollView == null)
        {
            return false;
        }
        state.activeScrollView = scrollView;
        state.editMode = true;
        Element target = firstFocusableDescendantInScrollView(window, scrollView, state);
        List<Element> activeFocusable = focusableElements(window, scrollView);
        if (target != null)
        {
            state.focusedIndex = activeFocusable.indexOf(target);
            rememberScrollViewDescendant(state, target);
            ensureElementVisibleInScrollView(scrollView, target);
        }
        else
        {
            Element scopeRoot = generatedScrollViewProxyFor(window, scrollView);
            state.focusedIndex = activeFocusable.indexOf(scopeRoot == null ? scrollView : scopeRoot);
        }
        return true;
    }

    private static void exitScrollViewScope(
        GeneratedWindowBase window,
        RuntimeState state)
    {
        ScrollView scrollView = state.activeScrollView;
        if (scrollView == null)
        {
            state.editMode = false;
            return;
        }
        List<Element> activeFocusable = focusableElements(window, scrollView);
        List<Element> scoped = scrollViewScopeElements(window, scrollView);
        Element focused = focusedElement(activeFocusable, state.focusedIndex);
        Element scopeRoot = generatedScrollViewProxyFor(window, scrollView);
        if (focused != null
            && focused != scrollView
            && focused != scopeRoot
            && scoped.contains(focused))
        {
            state.scrollViewLastDescendant.put(scrollView, focused);
        }
        state.activeScrollView = null;
        state.activeScrollViewEditElement = null;
        state.editSnapshot = null;
        state.editMode = false;
        state.focusedIndex = focusableElements(window).indexOf(scopeRoot == null ? scrollView : scopeRoot);
    }

    private static void rememberScrollViewDescendant(RuntimeState state, Element descendant)
    {
        if (state.activeScrollView != null && descendant != null)
        {
            state.scrollViewLastDescendant.put(state.activeScrollView, descendant);
        }
    }

    private static boolean moveScrollViewScopeFocus(
        GeneratedWindowBase window,
        RuntimeState state,
        String key)
    {
        ScrollView scrollView = state.activeScrollView;
        if (scrollView == null)
        {
            return false;
        }
        List<Element> activeFocusable = focusableElements(window, scrollView);
        Element current = focusedElement(activeFocusable, state.focusedIndex);
        List<Element> scoped = new ArrayList<>(scrollViewScopeElements(window, scrollView));
        Element scopeRoot = generatedScrollViewProxyFor(window, scrollView);
        scoped.removeIf(element -> element == scrollView || element == scopeRoot);
        if (scoped.isEmpty())
        {
            return ("Up".equals(key) || "Down".equals(key)) && scrollView.handleKey(key);
        }
        int scopedIndex = scoped.indexOf(current);
        int before = scopedIndex;
        int movedIndex = moveSpatial(scoped, scopedIndex, key);
        if (movedIndex != before && movedIndex >= 0 && movedIndex < scoped.size())
        {
            Element next = scoped.get(movedIndex);
            state.focusedIndex = activeFocusable.indexOf(next);
            rememberScrollViewDescendant(state, next);
            ensureElementVisibleInScrollView(scrollView, next);
            return true;
        }
        if (!("Up".equals(key) || "Down".equals(key)) || !scrollView.handleKey(key))
        {
            return false;
        }
        activeFocusable = focusableElements(window, scrollView);
        scoped = new ArrayList<>(scrollViewScopeElements(window, scrollView));
        scoped.removeIf(element -> element == scrollView || element == scopeRoot);
        if (scoped.isEmpty())
        {
            return true;
        }
        int nextIndex = scoped.indexOf(current);
        if (nextIndex >= 0)
        {
            int afterScroll = moveSpatial(scoped, nextIndex, key);
            if (afterScroll == nextIndex)
            {
                return true;
            }
            nextIndex = afterScroll;
        }
        else
        {
            nextIndex = "Down".equals(key) ? 0 : scoped.size() - 1;
        }
        if (nextIndex >= 0 && nextIndex < scoped.size())
        {
            Element next = scoped.get(nextIndex);
            state.focusedIndex = activeFocusable.indexOf(next);
            rememberScrollViewDescendant(state, next);
            ensureElementVisibleInScrollView(scrollView, next);
        }
        return true;
    }

    private static boolean ensureElementVisibleInScrollView(
        ScrollView scrollView,
        Element target)
    {
        if (scrollView == null || target == null)
        {
            return false;
        }
        Rect viewport = scrollViewViewport(scrollView);
        if (viewport.width() <= 0 || viewport.height() <= 0)
        {
            return false;
        }
        Rect scrollFrame = scrollView.frame();
        for (ScrollViewChildView childView : scrollView.childViews(frameSize(scrollView)))
        {
            Element child = childView.element();
            if (child == null)
            {
                continue;
            }
            Rect targetRect = focusRectWithinScrollChild(
                child,
                target,
                childView.frame().width(),
                childView.frame().height());
            if (targetRect == null)
            {
                continue;
            }
            int targetTop = scrollFrame.row() + childView.frame().row() + targetRect.row();
            int targetBottom = targetTop + Math.max(MINIMUM_RENDERABLE_SIZE, targetRect.height());
            int delta = 0;
            if (targetTop < viewport.row())
            {
                delta = targetTop - viewport.row();
            }
            else if (targetBottom > viewport.row() + viewport.height())
            {
                delta = targetBottom - viewport.row() - viewport.height();
            }
            return scrollViewByLines(scrollView, delta);
        }
        return false;
    }

    private static boolean elementVisibleInScrollView(
        ScrollView scrollView,
        Element target)
    {
        if (scrollView == null || target == null)
        {
            return false;
        }
        Rect viewport = scrollViewViewport(scrollView);
        if (viewport.width() <= 0 || viewport.height() <= 0)
        {
            return false;
        }
        Rect scrollFrame = scrollView.frame();
        for (ScrollViewChildView childView : scrollView.childViews(frameSize(scrollView)))
        {
            Element child = childView.element();
            if (!childView.visible() || child == null)
            {
                continue;
            }
            Rect targetRect = focusRectWithinScrollChild(
                child,
                target,
                childView.frame().width(),
                childView.frame().height());
            if (targetRect == null)
            {
                continue;
            }
            int targetTop = scrollFrame.row() + childView.frame().row() + targetRect.row();
            int targetBottom = targetTop + Math.max(MINIMUM_RENDERABLE_SIZE, targetRect.height());
            return targetBottom > viewport.row()
                && targetTop < viewport.row() + viewport.height();
        }
        return false;
    }

    private static Rect focusRectWithinScrollChild(
        Element child,
        Element target,
        int width,
        int height)
    {
        if (child == target)
        {
            return new Rect(
                0,
                0,
                Math.max(MINIMUM_RENDERABLE_SIZE, child.frame().width()),
                Math.max(MINIMUM_RENDERABLE_SIZE, height));
        }
        if (child instanceof ReusableElement reusable
            && reusable.child() != null
            && windowContainsElement(reusable.child(), target))
        {
            syncWindowElementFramesTo(
                reusable.child(),
                new Rect(
                    0,
                    0,
                    Math.max(MINIMUM_RENDERABLE_SIZE, width),
                    Math.max(MINIMUM_RENDERABLE_SIZE, height)),
                true);
            Rect targetFrame = target.frame();
            return new Rect(
                targetFrame.row(),
                targetFrame.col(),
                Math.max(MINIMUM_RENDERABLE_SIZE, targetFrame.width()),
                Math.max(MINIMUM_RENDERABLE_SIZE, targetFrame.height()));
        }
        if (elementTreeContains(child, target))
        {
            Rect targetFrame = target.frame();
            return new Rect(
                targetFrame.row(),
                targetFrame.col(),
                Math.max(MINIMUM_RENDERABLE_SIZE, targetFrame.width()),
                Math.max(MINIMUM_RENDERABLE_SIZE, targetFrame.height()));
        }
        return null;
    }

    private static boolean scrollViewByLines(ScrollView scrollView, int delta)
    {
        if (delta == 0)
        {
            return false;
        }
        String key = delta < 0 ? "Up" : "Down";
        boolean changed = false;
        for (int step = 0; step < Math.abs(delta); ++step)
        {
            changed = scrollView.handleKey(key) || changed;
        }
        return changed;
    }

    private static Rect scrollViewViewport(ScrollView scrollView)
    {
        Rect frame = scrollView.frame();
        Style style = scrollView.style();
        int left = Math.min(
            styleValue(style.paddingLeft(), style.padding()),
            Math.max(0, frame.width() - MINIMUM_RENDERABLE_SIZE));
        int right = Math.min(
            styleValue(style.paddingRight(), style.padding()),
            Math.max(0, frame.width() - left - MINIMUM_RENDERABLE_SIZE));
        int top = Math.min(
            styleValue(style.paddingTop(), style.padding()),
            Math.max(0, frame.height() - MINIMUM_RENDERABLE_SIZE));
        int bottom = Math.min(
            styleValue(style.paddingBottom(), style.padding()),
            Math.max(0, frame.height() - top - MINIMUM_RENDERABLE_SIZE));
        return new Rect(
            frame.row() + top,
            frame.col() + left,
            Math.max(0, frame.width() - left - right),
            Math.max(0, frame.height() - top - bottom));
    }

    private static void clearInvalidActiveScrollViewScope(
        GeneratedWindowBase window,
        RuntimeState state)
    {
        if (state.activeScrollView == null)
        {
            return;
        }
        Element proxy = generatedScrollViewProxyFor(window, state.activeScrollView);
        if (!windowContainsElement(window, state.activeScrollView)
            && (proxy == null || !windowContainsElement(window, proxy)))
        {
            state.scrollViewLastDescendant.remove(state.activeScrollView);
            state.activeScrollView = null;
            state.activeScrollViewEditElement = null;
            state.focusedIndex = -1;
            state.editMode = false;
            state.editSnapshot = null;
            return;
        }
        if (state.activeScrollViewEditElement != null
            && !windowContainsElement(window, state.activeScrollViewEditElement))
        {
            state.activeScrollViewEditElement = null;
            state.editSnapshot = null;
        }
    }

    private static void changeFocus(
        GeneratedWindowRuntimeOptions options,
        List<Element> elements,
        RuntimeState state,
        int nextIndex)
    {
        Element previous = focusedElement(elements, state.focusedIndex);
        if (previous != null && options.onFocusChanged() != null && state.focusedIndex != nextIndex)
        {
            options.onFocusChanged().accept(previous.name(), false);
        }
        state.focusedIndex = nextIndex;
        state.editMode = state.activeScrollView != null;
        if (state.activeScrollView == null)
        {
            state.activeScrollViewEditElement = null;
        }
        state.editSnapshot = null;
        Element next = focusedElement(elements, nextIndex);
        if (next != null && options.onFocusChanged() != null && previous != next)
        {
            options.onFocusChanged().accept(next.name(), true);
        }
    }

    private static void notifyFocusChanged(
        GeneratedWindowRuntimeOptions options,
        Element element,
        boolean focused)
    {
        if (element != null && options.onFocusChanged() != null)
        {
            options.onFocusChanged().accept(element.name(), focused);
        }
    }

    private static int moveSpatial(List<Element> elements, int currentIndex, String direction)
    {
        if (elements.isEmpty())
        {
            return -1;
        }
        if (currentIndex < 0 || currentIndex >= elements.size())
        {
            return 0;
        }
        Rect current = elements.get(currentIndex).frame();
        int currentRow = centerRow(current);
        int currentCol = centerCol(current);
        int bestIndex = -1;
        int bestBand = 0;
        int bestPrimary = 0;
        int bestGap = 0;
        int bestEdge = 0;
        for (int index = 0; index < elements.size(); ++index)
        {
            if (index == currentIndex)
            {
                continue;
            }
            Rect candidate = elements.get(index).frame();
            int primary = 0;
            int gap = 0;
            int edge = 0;
            Integer band = null;
            if ("Up".equals(direction) && centerRow(candidate) < currentRow)
            {
                if (candidate.row() + candidate.height() > current.row())
                {
                    continue;
                }
                primary = Math.max(
                    0,
                    current.row() - (candidate.row() + candidate.height()));
                band = bandScanRank(
                    current.col(),
                    current.col() + current.width(),
                    candidate.col(),
                    candidate.col() + candidate.width());
                gap = axisGap(
                    current.col(),
                    current.col() + current.width(),
                    candidate.col(),
                    candidate.col() + candidate.width());
                edge = Math.abs(candidate.col() - current.col());
            }
            else if ("Down".equals(direction) && centerRow(candidate) > currentRow)
            {
                if (candidate.row() < current.row() + current.height())
                {
                    continue;
                }
                primary = Math.max(
                    0,
                    candidate.row() - (current.row() + current.height()));
                band = bandScanRank(
                    current.col(),
                    current.col() + current.width(),
                    candidate.col(),
                    candidate.col() + candidate.width());
                gap = axisGap(
                    current.col(),
                    current.col() + current.width(),
                    candidate.col(),
                    candidate.col() + candidate.width());
                edge = Math.abs(candidate.col() - current.col());
            }
            else if ("Right".equals(direction) && centerCol(candidate) > currentCol)
            {
                if (candidate.col() < current.col() + current.width())
                {
                    continue;
                }
                primary = Math.max(
                    0,
                    candidate.col() - (current.col() + current.width()));
                band = bandScanRank(
                    current.row(),
                    current.row() + current.height(),
                    candidate.row(),
                    candidate.row() + candidate.height());
                gap = axisGap(
                    current.row(),
                    current.row() + current.height(),
                    candidate.row(),
                    candidate.row() + candidate.height());
                edge = Math.abs(candidate.row() - current.row());
            }
            else if ("Left".equals(direction) && centerCol(candidate) < currentCol)
            {
                if (candidate.col() + candidate.width() > current.col())
                {
                    continue;
                }
                primary = Math.max(
                    0,
                    current.col() - (candidate.col() + candidate.width()));
                band = bandScanRank(
                    current.row(),
                    current.row() + current.height(),
                    candidate.row(),
                    candidate.row() + candidate.height());
                gap = axisGap(
                    current.row(),
                    current.row() + current.height(),
                    candidate.row(),
                    candidate.row() + candidate.height());
                edge = Math.abs(candidate.row() - current.row());
            }
            else
            {
                continue;
            }
            if (band == null)
            {
                continue;
            }
            if (bestIndex < 0 || spatialScoreBefore(
                band,
                primary,
                gap,
                edge,
                bestBand,
                bestPrimary,
                bestGap,
                bestEdge))
            {
                bestIndex = index;
                bestBand = band;
                bestPrimary = primary;
                bestGap = gap;
                bestEdge = edge;
            }
        }
        return bestIndex < 0 ? currentIndex : bestIndex;
    }

    private static int centerRow(Rect rect)
    {
        return rect.row() + rect.height() / 2;
    }

    private static int centerCol(Rect rect)
    {
        return rect.col() + rect.width() / 2;
    }

    private static boolean axisOverlaps(int startA, int endA, int startB, int endB)
    {
        return Math.max(startA, startB) < Math.min(endA, endB);
    }

    private static int axisGap(int startA, int endA, int startB, int endB)
    {
        if (endA <= startB)
        {
            return startB - endA;
        }
        if (endB <= startA)
        {
            return startA - endB;
        }
        return 0;
    }

    private static Integer bandScanRank(
        int currentStart,
        int currentEnd,
        int candidateStart,
        int candidateEnd)
    {
        if (axisOverlaps(currentStart, currentEnd, candidateStart, candidateEnd))
        {
            return 0;
        }
        int maxDistance = Math.max(
            Math.max(
                Math.abs(candidateStart - currentStart),
                Math.abs(candidateEnd - currentEnd)),
            Math.max(
                Math.abs(candidateStart - currentEnd),
                Math.abs(candidateEnd - currentStart)))
            + Math.max(1, currentEnd - currentStart)
            + Math.max(1, candidateEnd - candidateStart);
        int rank = 1;
        for (int distance = 1; distance <= maxDistance; ++distance)
        {
            int[] offsets = {distance, -distance};
            for (int offset : offsets)
            {
                if (axisOverlaps(
                    currentStart + offset,
                    currentEnd + offset,
                    candidateStart,
                    candidateEnd))
                {
                    return rank;
                }
                ++rank;
            }
        }
        return null;
    }

    private static boolean spatialScoreBefore(
        int band,
        int primary,
        int gap,
        int edge,
        int otherBand,
        int otherPrimary,
        int otherGap,
        int otherEdge)
    {
        if (band != otherBand)
        {
            return band < otherBand;
        }
        if (primary != otherPrimary)
        {
            return primary < otherPrimary;
        }
        if (gap != otherGap)
        {
            return gap < otherGap;
        }
        return edge < otherEdge;
    }

    private static boolean isArrowKey(String key)
    {
        return "Up".equals(key)
            || "Down".equals(key)
            || "Left".equals(key)
            || "Right".equals(key);
    }

    static int initialFocusIndex(
        List<Element> elements,
        String initialFocusName,
        boolean startInEditMode)
    {
        if (initialFocusName != null && !initialFocusName.isEmpty())
        {
            for (int index = 0; index < elements.size(); ++index)
            {
                if (initialFocusName.equals(elements.get(index).name()))
                {
                    return index;
                }
            }
            return -1;
        }
        return startInEditMode && !elements.isEmpty() ? 0 : -1;
    }

    private static void renderToTerminal(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        Size terminalSize,
        RuntimeState state,
        TerminalBuffer terminalBuffer,
        TerminalBackend terminal,
        Rect viewport,
        String notification)
    {
        List<List<TerminalCell>> frame = options.windowStack() == null || options.windowStack().empty()
            ? renderTerminalContent(
                window,
                terminalSize,
                state.focusedIndex,
                state.editMode,
                state.activeScrollView,
                state.activeScrollViewEditElement,
                state.suppressActiveScrollViewScopeVisuals)
            : renderTerminalStack(window, options.windowStack(), terminalSize, state);
        copyContentToBuffer(frame, terminalBuffer);
        renderRuntimeOverlay(terminalBuffer, options);
        renderNotification(terminalBuffer, notification);
        terminal.write(terminalBuffer.renderDiff(
            viewport == null ? 0 : viewport.row(),
            viewport == null ? 0 : viewport.col()));
    }

    static void renderRuntimeOverlay(
        TerminalBuffer buffer,
        GeneratedWindowRuntimeOptions options)
    {
        boolean dimBackground = options.shouldDimOverlayBackground() != null
            && options.shouldDimOverlayBackground().getAsBoolean();
        if (options.onRenderOverlay() == null)
        {
            return;
        }
        if (dimBackground)
        {
            dimBufferForModalOverlay(buffer);
        }
        options.onRenderOverlay().accept(buffer);
    }

    private static void renderNotification(
        TerminalBuffer buffer,
        String notification)
    {
        if (notification == null || notification.isEmpty() || buffer.height() <= 0)
        {
            return;
        }
        int width = buffer.width();
        if (width <= 0)
        {
            return;
        }
        String padding = " ".repeat(NOTIFICATION_HORIZONTAL_PADDING);
        String text = padding + notification + padding;
        int col = Math.max(0, width - text.length() - NOTIFICATION_RIGHT_MARGIN);
        for (int index = 0; index < text.length() && col + index < width; ++index)
        {
            TerminalCell cell = new TerminalCell();
            cell.setText(Character.toString(text.charAt(index)));
            cell.setForeground(NOTIFICATION_FOREGROUND);
            cell.setBackground(NOTIFICATION_BACKGROUND);
            buffer.setCell(NOTIFICATION_ROW, col + index, cell);
        }
    }

    private static List<List<TerminalCell>> renderTerminalStack(
        GeneratedWindowBase rootWindow,
        GeneratedWindowStack stack,
        Size terminalSize,
        RuntimeState rootState)
    {
        TerminalBuffer buffer = new TerminalBuffer(terminalSize.width(), terminalSize.height());
        copyContentToBuffer(
            renderModalBackground(
                rootWindow,
                terminalSize,
                rootState.focusedIndex,
                rootState.editMode,
                rootState.activeScrollView,
                rootState.activeScrollViewEditElement),
            buffer);
        renderWindowStackOverlay(buffer, stack);
        return contentFromBuffer(buffer);
    }

    static List<List<TerminalCell>> renderRuntimeStack(
        GeneratedWindowStack stack,
        Size terminalSize)
    {
        if (stack == null || stack.rootFrame() == null)
        {
            return List.of();
        }
        GeneratedWindowStackFrame root = stack.rootFrame();
        TerminalBuffer buffer = new TerminalBuffer(
            Math.max(MINIMUM_RENDERABLE_SIZE, terminalSize.width()),
            Math.max(MINIMUM_RENDERABLE_SIZE, terminalSize.height()));
        List<List<TerminalCell>> rootContent = stack.empty()
            ? renderTerminalContent(
                root.window(),
                terminalSize,
                root.focusedIndex(),
                root.editMode(),
                root.activeScrollView(),
                root.activeScrollViewEditElement(),
                root.suppressActiveScrollViewScopeVisuals())
            : renderModalBackground(
                root.window(),
                terminalSize,
                root.focusedIndex(),
                root.editMode(),
                root.activeScrollView(),
                root.activeScrollViewEditElement());
        copyContentToBuffer(
            rootContent,
            buffer);
        renderWindowStackOverlay(buffer, stack);
        return contentFromBuffer(buffer);
    }

    private static List<List<TerminalCell>> renderModalBackground(
        GeneratedWindowBase window,
        Size terminalSize,
        int focusedIndex,
        boolean editMode,
        ScrollView activeScrollView,
        Element activeScrollViewEditElement)
    {
        ScopedImageCellBackgroundRendering imageCellBackgroundRendering =
            new ScopedImageCellBackgroundRendering();
        try
        {
            return renderTerminalContent(
                window,
                terminalSize,
                focusedIndex,
                editMode,
                activeScrollView,
                activeScrollViewEditElement,
                true);
        }
        finally
        {
            imageCellBackgroundRendering.close();
        }
    }

    public static void renderWindowStackOverlay(TerminalBuffer buffer, GeneratedWindowStack stack)
    {
        if (buffer == null || stack == null || stack.empty())
        {
            return;
        }
        boolean dimBase = stack.frames().stream()
            .anyMatch(frame -> frame.options().dimBackground());
        if (dimBase)
        {
            dimBufferForModalOverlay(buffer);
        }
        for (int index = 0; index < stack.frames().size(); ++index)
        {
            GeneratedWindowStackFrame frame = stack.frames().get(index);
            boolean topFrame = index == stack.frames().size() - 1;
            boolean dimFrame = !topFrame
                && stack.frames().subList(index + 1, stack.frames().size()).stream()
                    .anyMatch(later -> later.options().dimBackground());
            renderStackFrame(buffer, frame, topFrame, dimFrame);
        }
    }

    private static void renderStackFrame(
        TerminalBuffer target,
        GeneratedWindowStackFrame frame,
        boolean topFrame,
        boolean dimFrame)
    {
        GeneratedWindowBase window = frame.window();
        Rect bounds = windowBounds(window, new Size(target.width(), target.height()));
        Rect content = contentBounds(new Rect(0, 0, bounds.width(), bounds.height()), window.generatedWindowStyle());
        TerminalBuffer frameBuffer = new TerminalBuffer(bounds.width(), bounds.height());
        fill(frameBuffer, new Rect(0, 0, bounds.width(), bounds.height()), borderStyleFor(window.generatedWindowStyle()));
        fill(frameBuffer, content, window.generatedWindowStyle());
        List<List<TerminalCell>> rows;
        if (topFrame)
        {
            rows = renderGeneratedWindowContent(
                window,
                new Size(content.width(), content.height()),
                frame.focusedIndex(),
                frame.editMode(),
                frame.activeScrollView(),
                frame.activeScrollViewEditElement(),
                frame.suppressActiveScrollViewScopeVisuals());
        }
        else
        {
            ScopedImageCellBackgroundRendering imageCellBackgroundRendering =
                new ScopedImageCellBackgroundRendering();
            try
            {
                rows = renderGeneratedWindowContent(
                    window,
                    new Size(content.width(), content.height()),
                    frame.focusedIndex(),
                    false);
            }
            finally
            {
                imageCellBackgroundRendering.close();
            }
        }
        blit(frameBuffer, rows, content.row(), content.col());
        if (dimFrame)
        {
            dimBufferForModalOverlay(frameBuffer);
        }
        blit(target, contentFromBuffer(frameBuffer), bounds.row(), bounds.col());
        if (topFrame && !frame.suppressActiveScrollViewScopeVisuals())
        {
            overlayFocusedComboBox(
                target,
                window,
                frame.focusedIndex(),
                frame.editMode(),
                bounds.row(),
                bounds.col(),
                frame.activeScrollView(),
                frame.activeScrollViewEditElement());
        }
        syncWindowElementFramesTo(
            window,
            new Rect(
                bounds.row() + content.row(),
                bounds.col() + content.col(),
                content.width(),
                content.height()));
    }

    private static void overlayFocusedComboBox(
        TerminalBuffer buffer,
        GeneratedWindowBase window,
        int focusedIndex,
        boolean editMode,
        int windowRow,
        int windowCol,
        ScrollView activeScrollView,
        Element activeScrollViewEditElement)
    {
        if (!editMode)
        {
            return;
        }
        List<Element> focusable = focusableElements(window, activeScrollView);
        Element focused = focusedElement(focusable, focusedIndex);
        if (activeScrollView != null && activeScrollViewEditElement != focused)
        {
            return;
        }
        if (!(focused instanceof ComboBox comboBox))
        {
            return;
        }
        Rect comboFrame = comboBox.frame();
        Size overlaySize = new Size(
            comboFrame.width(),
            COMBO_BOX_CLOSED_ROWS + comboBox.options().size());
        ElementRenderState state = new ElementRenderState();
        state.setFocused(true);
        state.setEditMode(true);
        blit(
            buffer,
            comboBox.render(overlaySize, state),
            windowRow + comboFrame.row(),
            windowCol + comboFrame.col());
    }

    private static Rect windowBounds(GeneratedWindowBase window, Size terminalSize)
    {
        Style style = window.generatedWindowStyle();
        Size content = generatedWindowContentSize(window);
        int naturalWidth = content.width() + borderWidthVertical(style) * 2;
        int naturalHeight = content.height() + borderWidthHorizontal(style) * 2;
        int leftMargin = marginLeft(style);
        int rightMargin = marginRight(style);
        int topMargin = marginTop(style);
        int bottomMargin = marginBottom(style);
        int availableWidth = Math.max(MINIMUM_RENDERABLE_SIZE, terminalSize.width() - leftMargin - rightMargin);
        int availableHeight = Math.max(MINIMUM_RENDERABLE_SIZE, terminalSize.height() - topMargin - bottomMargin);
        GeneratedWindowMode mode = windowMode(window);
        boolean expandsWidth = mode == GeneratedWindowMode.FULLSCREEN
            || mode == GeneratedWindowMode.EXPAND_WIDTH;
        boolean expandsHeight = mode == GeneratedWindowMode.FULLSCREEN
            || mode == GeneratedWindowMode.EXPAND_HEIGHT;
        int width = expandsWidth ? availableWidth : Math.min(naturalWidth, availableWidth);
        int height = expandsHeight ? availableHeight : Math.min(naturalHeight, availableHeight);
        int row = expandsHeight
            ? topMargin
            : topMargin + Math.max(0, (availableHeight - height) / 2);
        int col = expandsWidth
            ? leftMargin
            : leftMargin + Math.max(0, (availableWidth - width) / 2);
        return new Rect(row, col, width, height);
    }

    private static Rect contentBounds(Rect bounds, Style style)
    {
        int horizontal = borderWidthHorizontal(style);
        int vertical = borderWidthVertical(style);
        return new Rect(
            bounds.row() + horizontal,
            bounds.col() + vertical,
            Math.max(MINIMUM_RENDERABLE_SIZE, bounds.width() - vertical * 2),
            Math.max(MINIMUM_RENDERABLE_SIZE, bounds.height() - horizontal * 2));
    }

    private static Style borderStyleFor(Style style)
    {
        Style border = new Style();
        border.setColor(style.borderColor());
        border.setBackground(style.background());
        return border;
    }

    private static int borderWidthHorizontal(Style style)
    {
        return Math.max(0, style.borderWidthHorizontal() == null ? 0 : style.borderWidthHorizontal());
    }

    private static int borderWidthVertical(Style style)
    {
        return Math.max(0, style.borderWidthVertical() == null ? 0 : style.borderWidthVertical());
    }

    private static int marginTop(Style style)
    {
        return styleValue(style.marginTop(), style.margin());
    }

    private static int marginRight(Style style)
    {
        return styleValue(style.marginRight(), style.margin());
    }

    private static int marginBottom(Style style)
    {
        return styleValue(style.marginBottom(), style.margin());
    }

    private static int marginLeft(Style style)
    {
        return styleValue(style.marginLeft(), style.margin());
    }

    private static int styleValue(Integer explicit, Integer common)
    {
        return Math.max(0, explicit == null ? (common == null ? 0 : common) : explicit);
    }

    public static void dimGeneratedWindowModalBackground(TerminalBuffer buffer)
    {
        if (buffer == null)
        {
            throw new IllegalArgumentException("buffer must not be null");
        }
        dimBufferForModalOverlay(buffer);
    }

    private static void dimBufferForModalOverlay(TerminalBuffer buffer)
    {
        for (int row = 0; row < buffer.height(); ++row)
        {
            for (int col = 0; col < buffer.width(); ++col)
            {
                TerminalCell cell = buffer.cell(row, col).copy();
                cell.setForeground(dimColor(cell.foreground()));
                cell.setBackground(dimColor(cell.background()));
                buffer.setCell(row, col, cell);
            }
        }
    }

    private static void dimOutsideActiveScrollView(
        TerminalBuffer buffer,
        Rect activeScrollView,
        Color dimBackground)
    {
        if (activeScrollView == null || dimBackground == null)
        {
            return;
        }
        for (int row = 0; row < buffer.height(); ++row)
        {
            for (int col = 0; col < buffer.width(); ++col)
            {
                if (activeScrollView.contains(new Point(row, col)))
                {
                    continue;
                }
                TerminalCell cell = buffer.cell(row, col).copy();
                if (Image.UPPER_HALF_BLOCK.equals(cell.text()))
                {
                    cell.setForeground(dimColorOverExisting(
                        dimBackground,
                        cell.foreground(),
                        false));
                    cell.setBackground(dimColorOverExisting(
                        dimBackground,
                        cell.background(),
                        true));
                }
                else
                {
                    cell.setBackground(dimColorOverExisting(
                        dimBackground,
                        cell.background(),
                        true));
                }
                buffer.setCell(row, col, cell);
            }
        }
    }

    private static Color dimColorOverExisting(
        Color dimBackground,
        Color color,
        boolean fillWhenMissing)
    {
        if (dimBackground != null && dimBackground.rgba().isPresent() && color != null)
        {
            return dimBackground.blendOver(color);
        }
        return fillWhenMissing || color != null ? dimBackground : null;
    }

    private static Color generatedScrollViewScopeDimBackground(
        GeneratedWindowBase window,
        ScrollView target)
    {
        Element proxy = generatedScrollViewProxyFor(window, target);
        if (proxy != null && proxy.style().scopeDimBackground() != null)
        {
            return proxy.style().scopeDimBackground();
        }
        return target == null ? null : target.style().scopeDimBackground();
    }

    private static Rect elementFrameInWindow(GeneratedWindowBase window, Element target)
    {
        if (window == null || target == null)
        {
            return null;
        }
        for (Element element : window.elements())
        {
            if (element == target)
            {
                return element.frame();
            }
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                Rect nested = elementFrameInWindow(reusable.child(), target);
                if (nested != null)
                {
                    return offsetRect(nested, reusable.frame());
                }
            }
            if (element instanceof ScrollView scrollView)
            {
                Rect nested = elementFrameInScrollView(scrollView, target);
                if (nested != null)
                {
                    return nested;
                }
            }
        }
        return null;
    }

    private static Rect elementFrameInScrollView(ScrollView scrollView, Element target)
    {
        for (Element child : scrollView.children())
        {
            if (child == target)
            {
                return child.frame();
            }
            if (child instanceof ReusableElement reusable && reusable.child() != null)
            {
                Rect nested = elementFrameInWindow(reusable.child(), target);
                if (nested != null)
                {
                    return offsetRect(nested, reusable.frame());
                }
            }
        }
        return null;
    }

    private static Rect offsetRect(Rect nested, Rect origin)
    {
        return new Rect(
            origin.row() + nested.row(),
            origin.col() + nested.col(),
            nested.width(),
            nested.height());
    }

    private static Color dimColor(Color color)
    {
        if (color == null || color.isTransparent() || color.rgba().isEmpty())
        {
            return color;
        }
        Rgba rgba = color.rgba().orElseThrow();
        return Color.rgb(
            (int) (rgba.red() * MODAL_BACKGROUND_DIM_FACTOR),
            (int) (rgba.green() * MODAL_BACKGROUND_DIM_FACTOR),
            (int) (rgba.blue() * MODAL_BACKGROUND_DIM_FACTOR),
            rgba.alpha());
    }

    private static void copyContentToBuffer(List<List<TerminalCell>> content, TerminalBuffer buffer)
    {
        for (int row = 0; row < content.size(); ++row)
        {
            List<TerminalCell> cells = content.get(row);
            for (int col = 0; col < cells.size(); ++col)
            {
                buffer.setCell(row, col, cells.get(col).copy());
            }
        }
    }

    static void beginFrameEditIfRequested(GeneratedWindowStackFrame frame)
    {
        if (!frame.editMode())
        {
            return;
        }
        List<Element> focusable = focusableElements(frame.window());
        Element focused = focusedElement(focusable, frame.focusedIndex());
        if (focused == null)
        {
            frame.setEditMode(false);
            return;
        }
        frame.setEditSnapshot(captureSnapshot(focused));
        beginElementEdit(focused);
    }

    private static void renderNonInteractive(GeneratedWindowBase window, Size size)
    {
        for (List<TerminalCell> row : renderTerminalContent(window, size, -1))
        {
            StringBuilder line = new StringBuilder();
            for (TerminalCell cell : row)
            {
                line.append(RenderHelpers.safeTerminalCellText(cell.text()));
            }
            System.out.println(line);
        }
    }

    private static void renderEntry(
        TerminalBuffer buffer,
        GeneratedWindowBase window,
        GeneratedLayoutEntry entry,
        Rect cell,
        Element focused,
        boolean editMode,
        ScrollView activeScrollView,
        Element activeScrollViewEditElement,
        Integer renderClipTop,
        Integer renderClipBottom,
        boolean suppressActiveScrollViewScopeVisuals,
        Color activeScrollViewFocusBackgroundOverride)
    {
        Element element = findElement(window, entry.name());
        Size resolvedSize = renderSizeFor(entry, cell, element);
        int row = renderRowFor(window.generatedLayout(), entry, cell);
        int col = renderColFor(window.generatedLayout(), entry, cell, resolvedSize);
        if (element == null)
        {
            if (entry.name() == null || entry.name().isEmpty())
            {
                return;
            }
            String text = entry.name();
            if (text.length() >= 2 && text.startsWith("\"") && text.endsWith("\""))
            {
                text = text.substring(1, text.length() - 1);
            }
            blitClipped(
                buffer,
                RenderHelpers.renderPlainText(
                    text,
                    resolvedSize.width(),
                    resolvedSize.height(),
                    entry.elementStyle()),
                row,
                col,
                cellContentClip(entry, cell));
            return;
        }

        boolean activeScrollViewFocused = !suppressActiveScrollViewScopeVisuals
            && (activeScrollView == element
                || (element instanceof ScrollView
                    && (scrollViewContainingElement(window, focused) == element
                        || scrollViewContainingElement(window, activeScrollViewEditElement) == element)));
        boolean suppressElementFocus = suppressActiveScrollViewScopeVisuals
            && (element instanceof ReusableElement || element instanceof ScrollView);
        boolean focusedElement = element == focused && !suppressElementFocus;
        boolean scrollViewSelfFocused = element instanceof ScrollView
            && focusedElement
            && !activeScrollViewFocused;
        boolean elementEditActive = focusedElement
            && editMode
            && (activeScrollView == null || activeScrollViewEditElement == element);
        if (elementEditActive && element instanceof ComboBox comboBox)
        {
            int dropdownRows = COMBO_BOX_CLOSED_ROWS + comboBox.options().size();
            int visibleRows = Math.min(COMBO_BOX_DROPDOWN_ROWS, dropdownRows);
            int height = Math.min(
                visibleRows,
                Math.max(resolvedSize.height(), buffer.height() - row));
            resolvedSize = new Size(resolvedSize.width(), height);
            row = renderRowFor(window.generatedLayout(), entry, cell);
            col = renderColFor(window.generatedLayout(), entry, cell, resolvedSize);
        }
        Rect frame = new Rect(row, col, resolvedSize.width(), resolvedSize.height());
        element.setFrame(frame);
        ElementRenderState state = new ElementRenderState();
        state.setFocused(focusedElement && !scrollViewSelfFocused && !activeScrollViewFocused);
        state.setEditMode(element instanceof ReusableElement ? editMode : elementEditActive);
        state.setFocusedElement(focused);
        state.setActiveScrollView(activeScrollView);
        state.setActiveScrollViewEditElement(activeScrollViewEditElement);
        state.setSuppressActiveScrollViewScopeVisuals(suppressActiveScrollViewScopeVisuals);
        Rect clip = cellContentClip(entry, cell);
        int baseClipTop = Math.max(0, clip.row());
        int baseClipBottom = Math.max(
            baseClipTop,
            Math.min(buffer.height(), clip.row() + clip.height()));
        int absoluteClipTop = Math.max(
            baseClipTop,
            renderClipTop == null ? baseClipTop : renderClipTop);
        int absoluteClipBottom = Math.max(
            absoluteClipTop,
            Math.min(
                baseClipBottom,
                renderClipBottom == null ? baseClipBottom : renderClipBottom));
        int visibleTop = Math.max(row, absoluteClipTop);
        int visibleBottom = Math.min(row + resolvedSize.height(), absoluteClipBottom);
        int elementClipTop = Math.max(0, visibleTop - row);
        int elementClipBottom = Math.max(0, visibleBottom - row);
        if (elementClipTop != 0 || elementClipBottom != resolvedSize.height())
        {
            state.setClipTop(elementClipTop);
            state.setClipBottom(elementClipBottom);
        }
        Color paintedBackground = row >= 0 && row < buffer.height()
            && col >= 0 && col < buffer.width()
                ? buffer.cell(row, col).background()
                : null;
        ScopedElementParentBackground parentBackground = new ScopedElementParentBackground(
            paintedBackground);
        List<List<TerminalCell>> content;
        try
        {
            content = element.render(resolvedSize, state);
        }
        finally
        {
            parentBackground.close();
        }
        if (element instanceof ComboBox && elementEditActive && !content.isEmpty())
        {
            int clipBottom = Math.max(clip.row() + clip.height(), row + content.size());
            clip = new Rect(
                clip.row(),
                clip.col(),
                clip.width(),
                Math.max(0, clipBottom - clip.row()));
        }
        blitClipped(buffer, content, row, col, clip);
        if (element instanceof ScrollView scrollView)
        {
            boolean generatedScrollViewProxyFocused = activeScrollViewFocusBackgroundOverride != null
                && window.generatedScrollView() == scrollView
                && !activeScrollViewFocused
                && !scrollViewSelfFocused;
            if (activeScrollViewFocused || generatedScrollViewProxyFocused || scrollViewSelfFocused)
            {
                Color baseBackground = scrollView.style().background() == null
                    ? (paintedBackground == null
                        ? entry.cellStyle().background()
                        : paintedBackground)
                    : scrollView.style().background();
                Color focusBackground = activeScrollViewFocusBackgroundOverride;
                if (focusBackground == null && scrollView.focusStyle() != null)
                {
                    focusBackground = scrollView.focusStyle().background();
                }
                applyActiveScrollViewFocusBackgroundGaps(
                    buffer,
                    scrollView,
                    frame,
                    baseBackground,
                    focusBackground,
                    paintedBackground == null
                        ? entry.cellStyle().background()
                        : paintedBackground);
            }
        }
    }

    private static void applyActiveScrollViewFocusBackgroundGaps(
        TerminalBuffer buffer,
        ScrollView scrollView,
        Rect active,
        Color baseBackground,
        Color focusBackground,
        Color fallbackBackground)
    {
        if (focusBackground == null)
        {
            return;
        }
        Color effectiveBaseBackground = baseBackground;
        if ((effectiveBaseBackground == null || hasPartialAlpha(effectiveBaseBackground))
            && active.row() >= 0 && active.row() < buffer.height()
            && active.col() >= 0 && active.col() < buffer.width())
        {
            effectiveBaseBackground = buffer.cell(active.row(), active.col()).background();
        }
        Color focusedBaseBackground = effectiveBaseBackground == null
            ? null
            : blendExact(focusBackground, effectiveBaseBackground);
        boolean transparentViewportBackground = hasPartialAlpha(baseBackground);
        int bottom = Math.min(buffer.height(), active.row() + active.height());
        int right = Math.min(buffer.width(), active.col() + active.width());
        for (int row = Math.max(0, active.row()); row < bottom; ++row)
        {
            Integer protectedContentStartCol = null;
            Integer protectedContentEndCol = null;
            if (transparentViewportBackground
                && rowRenderedContentBounds(
                    buffer,
                    row,
                    active.col(),
                    active.col() + active.width()) != null)
            {
                protectedContentStartCol = Math.max(
                    active.col(),
                    Math.min(
                        active.col() + active.width(),
                        active.col() + paddingLeft(scrollView.style())));
                protectedContentEndCol = Math.max(
                    active.col() - 1,
                    Math.min(
                        active.col() + active.width() - 1,
                        active.col() + active.width()
                            - paddingRight(scrollView.style()) - 1));
            }
            for (int col = Math.max(0, active.col()); col < right; ++col)
            {
                if (protectedContentStartCol != null
                    && protectedContentEndCol != null
                    && col >= protectedContentStartCol
                    && col <= protectedContentEndCol)
                {
                    continue;
                }
                TerminalCell cell = buffer.cell(row, col).copy();
                Color background = cell.background();
                boolean matchesBase = effectiveBaseBackground == null
                    || background == null
                    || effectiveBaseBackground.equals(background);
                boolean matchesFallback = fallbackBackground != null
                    && fallbackBackground.equals(background);
                boolean matchesFocusedBase = focusedBaseBackground != null
                    && focusedBaseBackground.equals(background);
                if ((!matchesBase && !matchesFallback && !matchesFocusedBase)
                    || matchesFocusedBase)
                {
                    continue;
                }
                cell.setBackground(background == null
                    ? focusBackground
                    : blendExact(focusBackground, background));
                buffer.setCell(row, col, cell);
            }
        }
    }

    private static boolean cellHasRenderedContent(TerminalCell cell)
    {
        return !cell.raw().isEmpty()
            || (!" ".equals(cell.text())
                && !SCROLL_INDICATOR_ABOVE.equals(cell.text())
                && !SCROLL_INDICATOR_BELOW.equals(cell.text()));
    }

    private static IntSpan rowRenderedContentBounds(
        TerminalBuffer buffer,
        int row,
        int startCol,
        int endCol)
    {
        if (row < 0 || row >= buffer.height())
        {
            return null;
        }
        int start = Math.max(0, startCol);
        int end = Math.min(buffer.width(), endCol);
        Integer first = null;
        Integer last = null;
        for (int col = start; col < end; ++col)
        {
            if (cellHasRenderedContent(buffer.cell(row, col)))
            {
                if (first == null)
                {
                    first = col;
                }
                last = col;
            }
        }
        return first == null || last == null ? null : new IntSpan(first, last);
    }

    private static Color blendExact(Color foreground, Color background)
    {
        if (foreground == null || background == null)
        {
            return foreground;
        }
        return foreground.blendOver(background);
    }

    private static boolean isFocusedComboBoxEntry(
        GeneratedWindowBase window,
        GeneratedLayoutEntry entry,
        Element focused,
        boolean editMode)
    {
        return editMode
            && focused != null
            && findElement(window, entry.name()) == focused
            && focused instanceof ComboBox;
    }

    private static Integer logicalCellSize(AxisDimension dimension, int sourceSize)
    {
        if (dimension.mode() == DimensionMode.EXPANDED)
        {
            return null;
        }
        if (dimension.mode() == DimensionMode.FIXED)
        {
            return Math.max(MINIMUM_RENDERABLE_SIZE, dimension.value());
        }
        return Math.max(
            MINIMUM_RENDERABLE_SIZE,
            dimension.value() > 0 ? dimension.value() : sourceSize);
    }

    private static boolean sameRuntimeCell(RuntimeCell cell, GeneratedLayoutEntry entry)
    {
        Rect source = entry.sourceCell();
        return cell.sourceRow == source.row()
            && cell.sourceCol == source.col()
            && cell.sourceWidth == source.width()
            && cell.sourceHeight == source.height();
    }

    private static boolean sameSourceCell(
        GeneratedLayoutEntry first,
        GeneratedLayoutEntry second)
    {
        Rect firstSource = first.sourceCell();
        Rect secondSource = second.sourceCell();
        return firstSource.row() == secondSource.row()
            && firstSource.col() == secondSource.col()
            && firstSource.width() == secondSource.width()
            && firstSource.height() == secondSource.height();
    }

    private static int paddingTop(Style style)
    {
        return styleValue(style.paddingTop(), style.padding());
    }

    private static int paddingRight(Style style)
    {
        return styleValue(style.paddingRight(), style.padding());
    }

    private static int paddingBottom(Style style)
    {
        return styleValue(style.paddingBottom(), style.padding());
    }

    private static int paddingLeft(Style style)
    {
        return styleValue(style.paddingLeft(), style.padding());
    }

    private static int fitEntryHeight(
        GeneratedWindowBase window,
        GeneratedLayoutEntry entry,
        Integer availableCellWidth)
    {
        Element element = findElement(window, entry.name());
        int fallback = entry.charsSize().height() > 0
            ? entry.charsSize().height()
            : entry.sourceCell().height();
        Integer contentWidth = availableCellWidth == null
            ? null
            : Math.max(
                MINIMUM_RENDERABLE_SIZE,
                availableCellWidth - entry.relative().col() - entry.marginRight());
        int contentHeight = entry.height().mode() == DimensionMode.FIT_CONTENT
            ? fitContentHeightFor(element, entry.type(), fallback, contentWidth)
            : Math.max(MINIMUM_RENDERABLE_SIZE, fallback);
        return Math.max(
            MINIMUM_RENDERABLE_SIZE,
            entry.relative().row() + contentHeight + entry.marginBottom());
    }

    private static int fitCellHeight(
        GeneratedWindowBase window,
        GeneratedLayoutEntry cellEntry,
        Integer resolvedCellWidth)
    {
        int contentHeight = cellEntry.sourceCell().height();
        Integer availableCellWidth = resolvedCellWidth == null
            ? null
            : Math.max(
                MINIMUM_RENDERABLE_SIZE,
                resolvedCellWidth - paddingLeft(cellEntry.cellStyle())
                    - paddingRight(cellEntry.cellStyle()));
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            if (sameSourceCell(entry, cellEntry))
            {
                contentHeight = Math.max(
                    contentHeight,
                    fitEntryHeight(window, entry, availableCellWidth));
            }
        }
        return Math.max(
            MINIMUM_RENDERABLE_SIZE,
            contentHeight + paddingTop(cellEntry.cellStyle())
                + paddingBottom(cellEntry.cellStyle()));
    }

    private static int fitEntryWidth(
        GeneratedWindowBase window,
        GeneratedLayoutEntry entry)
    {
        Element element = findElement(window, entry.name());
        int fallback = entry.charsSize().width() > 0
            ? entry.charsSize().width()
            : entry.sourceCell().width();
        int contentWidth = fallback;
        if (element instanceof ListBox listBox)
        {
            for (String option : listBox.options())
            {
                contentWidth = Math.max(contentWidth, RenderHelpers.visibleWidth(option));
            }
        }
        else if (element instanceof Label label)
        {
            for (String line : label.text().split("\\n", -1))
            {
                contentWidth = Math.max(contentWidth, RenderHelpers.visibleWidth(line));
            }
        }
        return Math.max(
            MINIMUM_RENDERABLE_SIZE,
            entry.relative().col() + contentWidth + entry.marginRight());
    }

    private static int fitCellWidth(
        GeneratedWindowBase window,
        GeneratedLayoutEntry cellEntry)
    {
        int contentWidth = cellEntry.sourceCell().width();
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            if (sameSourceCell(entry, cellEntry))
            {
                contentWidth = Math.max(contentWidth, fitEntryWidth(window, entry));
            }
        }
        return Math.max(
            MINIMUM_RENDERABLE_SIZE,
            contentWidth + paddingLeft(cellEntry.cellStyle())
                + paddingRight(cellEntry.cellStyle()));
    }

    private static List<RuntimeCell> collectRuntimeCells(GeneratedWindowBase window)
    {
        List<RuntimeCell> cells = new ArrayList<>();
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            boolean exists = cells.stream().anyMatch(cell -> sameRuntimeCell(cell, entry));
            if (exists)
            {
                continue;
            }
            Integer logicalWidth = logicalCellSize(
                entry.cellWidth(),
                entry.sourceCell().width());
            Integer logicalHeight = logicalCellSize(
                entry.cellHeight(),
                entry.sourceCell().height());
            if (entry.cellWidth().mode() == DimensionMode.FIT_CONTENT)
            {
                logicalWidth = fitCellWidth(window, entry);
            }
            if (entry.cellHeight().mode() == DimensionMode.FIT_CONTENT)
            {
                logicalHeight = fitCellHeight(window, entry, null);
            }
            if (logicalWidth != null
                && entry.cellWidth().mode() != DimensionMode.EXPANDED
                && entry.cellWidth().mode() != DimensionMode.FIT_CONTENT)
            {
                logicalWidth = Math.max(
                    MINIMUM_RENDERABLE_SIZE,
                    logicalWidth + paddingLeft(entry.cellStyle())
                        + paddingRight(entry.cellStyle()));
            }
            if (logicalHeight != null
                && entry.cellHeight().mode() != DimensionMode.EXPANDED
                && entry.cellHeight().mode() != DimensionMode.FIT_CONTENT)
            {
                logicalHeight = Math.max(
                    MINIMUM_RENDERABLE_SIZE,
                    logicalHeight + paddingTop(entry.cellStyle())
                        + paddingBottom(entry.cellStyle()));
            }
            Rect source = entry.sourceCell();
            cells.add(new RuntimeCell(
                source.row(),
                source.col(),
                Math.max(MINIMUM_RENDERABLE_SIZE, source.width()),
                Math.max(MINIMUM_RENDERABLE_SIZE, source.height()),
                logicalWidth,
                logicalHeight));
        }
        return cells;
    }

    private static RuntimeNode makeLeafNode(RuntimeCell cell)
    {
        RuntimeNode node = new RuntimeNode();
        node.cell = cell;
        node.left = cell.sourceCol;
        node.top = cell.sourceRow;
        node.right = node.left + cell.sourceWidth + MINIMUM_RENDERABLE_SIZE;
        node.bottom = node.top + cell.sourceHeight + MINIMUM_RENDERABLE_SIZE;
        node.logicalWidth = cell.logicalWidth;
        node.logicalHeight = cell.logicalHeight;
        return node;
    }

    private static boolean intervalsCover(List<IntSpan> spans, int start, int end)
    {
        if (spans.isEmpty())
        {
            return false;
        }
        spans.sort(Comparator.comparingInt(IntSpan::start).thenComparingInt(IntSpan::end));
        int cursor = start;
        for (IntSpan span : spans)
        {
            if (span.start() > cursor)
            {
                return false;
            }
            cursor = Math.max(cursor, span.end());
            if (cursor >= end)
            {
                return true;
            }
        }
        return cursor >= end;
    }

    private static List<Integer> horizontalCuts(
        List<RuntimeNode> nodes,
        int left,
        int top,
        int right,
        int bottom)
    {
        List<Integer> candidates = new ArrayList<>();
        for (RuntimeNode node : nodes)
        {
            candidates.add(node.top);
            candidates.add(node.bottom);
        }
        candidates = sortedUnique(candidates);
        List<Integer> cuts = new ArrayList<>();
        for (int candidate : candidates)
        {
            if (candidate <= top || candidate >= bottom)
            {
                continue;
            }
            List<IntSpan> spans = new ArrayList<>();
            for (RuntimeNode node : nodes)
            {
                if (node.top == candidate || node.bottom == candidate)
                {
                    spans.add(new IntSpan(node.left, node.right));
                }
            }
            if (intervalsCover(spans, left, right))
            {
                cuts.add(candidate);
            }
        }
        return cuts;
    }

    private static List<Integer> verticalCuts(
        List<RuntimeNode> nodes,
        int left,
        int top,
        int right,
        int bottom)
    {
        List<Integer> candidates = new ArrayList<>();
        for (RuntimeNode node : nodes)
        {
            candidates.add(node.left);
            candidates.add(node.right);
        }
        candidates = sortedUnique(candidates);
        List<Integer> cuts = new ArrayList<>();
        for (int candidate : candidates)
        {
            if (candidate <= left || candidate >= right)
            {
                continue;
            }
            List<IntSpan> spans = new ArrayList<>();
            for (RuntimeNode node : nodes)
            {
                if (node.left == candidate || node.right == candidate)
                {
                    spans.add(new IntSpan(node.top, node.bottom));
                }
            }
            if (intervalsCover(spans, top, bottom))
            {
                cuts.add(candidate);
            }
        }
        return cuts;
    }

    private static List<Integer> sortedUnique(List<Integer> values)
    {
        values.sort(Integer::compareTo);
        List<Integer> result = new ArrayList<>();
        for (int value : values)
        {
            if (result.isEmpty() || result.get(result.size() - 1) != value)
            {
                result.add(value);
            }
        }
        return result;
    }

    private static List<List<RuntimeNode>> sliceByRows(
        List<RuntimeNode> nodes,
        List<Integer> boundaries)
    {
        List<List<RuntimeNode>> groups = new ArrayList<>();
        boolean[] used = new boolean[nodes.size()];
        for (int index = 0; index + 1 < boundaries.size(); ++index)
        {
            int bandTop = boundaries.get(index);
            int bandBottom = boundaries.get(index + 1);
            List<RuntimeNode> group = new ArrayList<>();
            for (int nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
            {
                RuntimeNode node = nodes.get(nodeIndex);
                if (node.top >= bandTop && node.bottom <= bandBottom)
                {
                    group.add(node);
                    used[nodeIndex] = true;
                }
            }
            if (group.isEmpty())
            {
                return List.of();
            }
            group.sort(Comparator.comparingInt((RuntimeNode node) -> node.top)
                .thenComparingInt(node -> node.left));
            groups.add(group);
        }
        for (boolean value : used)
        {
            if (!value)
            {
                return List.of();
            }
        }
        return groups;
    }

    private static List<List<RuntimeNode>> sliceByCols(
        List<RuntimeNode> nodes,
        List<Integer> boundaries)
    {
        List<List<RuntimeNode>> groups = new ArrayList<>();
        boolean[] used = new boolean[nodes.size()];
        for (int index = 0; index + 1 < boundaries.size(); ++index)
        {
            int bandLeft = boundaries.get(index);
            int bandRight = boundaries.get(index + 1);
            List<RuntimeNode> group = new ArrayList<>();
            for (int nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
            {
                RuntimeNode node = nodes.get(nodeIndex);
                if (node.left >= bandLeft && node.right <= bandRight)
                {
                    group.add(node);
                    used[nodeIndex] = true;
                }
            }
            if (group.isEmpty())
            {
                return List.of();
            }
            group.sort(Comparator.comparingInt((RuntimeNode node) -> node.left)
                .thenComparingInt(node -> node.top));
            groups.add(group);
        }
        for (boolean value : used)
        {
            if (!value)
            {
                return List.of();
            }
        }
        return groups;
    }

    private static RuntimeNode makeGroupNode(
        String orientation,
        List<RuntimeNode> children,
        int left,
        int top,
        int right,
        int bottom,
        int horizontalSeparator,
        int verticalSeparator)
    {
        RuntimeNode node = new RuntimeNode();
        node.orientation = orientation;
        node.children = children;
        node.left = left;
        node.top = top;
        node.right = right;
        node.bottom = bottom;

        if (ORIENTATION_VERTICAL.equals(node.orientation))
        {
            for (RuntimeNode child : node.children)
            {
                if (child.logicalWidth == null)
                {
                    node.logicalWidth = null;
                    break;
                }
                node.logicalWidth = Math.max(
                    node.logicalWidth == null ? MINIMUM_RENDERABLE_SIZE : node.logicalWidth,
                    child.logicalWidth);
            }
            int height = 0;
            for (RuntimeNode child : node.children)
            {
                if (child.logicalHeight == null)
                {
                    node.logicalHeight = null;
                    return node;
                }
                height += child.logicalHeight;
            }
            height += Math.max(0, node.children.size() - 1) * horizontalSeparator;
            node.logicalHeight = height;
        }
        else
        {
            for (RuntimeNode child : node.children)
            {
                if (child.logicalHeight == null)
                {
                    node.logicalHeight = null;
                    break;
                }
                node.logicalHeight = Math.max(
                    node.logicalHeight == null ? MINIMUM_RENDERABLE_SIZE : node.logicalHeight,
                    child.logicalHeight);
            }
            int width = 0;
            for (RuntimeNode child : node.children)
            {
                if (child.logicalWidth == null)
                {
                    node.logicalWidth = null;
                    return node;
                }
                width += child.logicalWidth;
            }
            width += Math.max(0, node.children.size() - 1) * verticalSeparator;
            node.logicalWidth = width;
        }
        return node;
    }

    private static RuntimeNode buildRuntimeNodeTree(
        List<RuntimeNode> inputNodes,
        int horizontalSeparator,
        int verticalSeparator)
    {
        if (inputNodes.isEmpty())
        {
            return null;
        }
        if (inputNodes.size() == 1)
        {
            return inputNodes.get(0);
        }
        List<RuntimeNode> nodes = new ArrayList<>(inputNodes);
        nodes.sort(Comparator.comparingInt((RuntimeNode node) -> node.top)
            .thenComparingInt(node -> node.left)
            .thenComparingInt(node -> node.bottom)
            .thenComparingInt(node -> node.right));
        int left = nodes.stream().mapToInt(node -> node.left).min().orElse(0);
        int top = nodes.stream().mapToInt(node -> node.top).min().orElse(0);
        int right = nodes.stream().mapToInt(node -> node.right).max().orElse(0);
        int bottom = nodes.stream().mapToInt(node -> node.bottom).max().orElse(0);

        List<Integer> cuts = horizontalCuts(nodes, left, top, right, bottom);
        if (!cuts.isEmpty())
        {
            List<Integer> boundaries = new ArrayList<>();
            boundaries.add(top);
            boundaries.addAll(cuts);
            boundaries.add(bottom);
            List<List<RuntimeNode>> groups = sliceByRows(nodes, boundaries);
            if (groups.size() > 1)
            {
                List<RuntimeNode> children = new ArrayList<>();
                for (List<RuntimeNode> group : groups)
                {
                    children.add(buildRuntimeNodeTree(
                        group,
                        horizontalSeparator,
                        verticalSeparator));
                }
                return makeGroupNode(
                    ORIENTATION_VERTICAL,
                    children,
                    left,
                    top,
                    right,
                    bottom,
                    horizontalSeparator,
                    verticalSeparator);
            }
        }

        cuts = verticalCuts(nodes, left, top, right, bottom);
        if (!cuts.isEmpty())
        {
            List<Integer> boundaries = new ArrayList<>();
            boundaries.add(left);
            boundaries.addAll(cuts);
            boundaries.add(right);
            List<List<RuntimeNode>> groups = sliceByCols(nodes, boundaries);
            if (groups.size() > 1)
            {
                List<RuntimeNode> children = new ArrayList<>();
                for (List<RuntimeNode> group : groups)
                {
                    children.add(buildRuntimeNodeTree(
                        group,
                        horizontalSeparator,
                        verticalSeparator));
                }
                return makeGroupNode(
                    ORIENTATION_HORIZONTAL,
                    children,
                    left,
                    top,
                    right,
                    bottom,
                    horizontalSeparator,
                    verticalSeparator);
            }
        }
        return nodes.get(0);
    }

    private static RuntimeNode buildRuntimeTreeFromCells(
        List<RuntimeCell> cells,
        int horizontalSeparator,
        int verticalSeparator)
    {
        List<RuntimeNode> leaves = new ArrayList<>();
        for (RuntimeCell cell : cells)
        {
            leaves.add(makeLeafNode(cell));
        }
        return buildRuntimeNodeTree(leaves, horizontalSeparator, verticalSeparator);
    }

    private static void resolveRuntimeNode(
        RuntimeNode node,
        Rect requestedRect,
        List<ResolvedRuntimeCell> resolved,
        int horizontalSeparator,
        int verticalSeparator,
        GeneratedWindowMode mode)
    {
        if (node == null)
        {
            return;
        }
        Rect rect = new Rect(
            requestedRect.row(),
            requestedRect.col(),
            Math.max(MINIMUM_RENDERABLE_SIZE, requestedRect.width()),
            Math.max(MINIMUM_RENDERABLE_SIZE, requestedRect.height()));
        if (node.children.isEmpty())
        {
            resolved.add(new ResolvedRuntimeCell(node.cell, rect));
            return;
        }

        if (ORIENTATION_VERTICAL.equals(node.orientation))
        {
            int fixedTotal = 0;
            int expandedCount = 0;
            for (RuntimeNode child : node.children)
            {
                if (child.logicalHeight == null)
                {
                    ++expandedCount;
                }
                else
                {
                    fixedTotal += child.logicalHeight;
                }
            }
            int separators = Math.max(0, node.children.size() - 1) * horizontalSeparator;
            int remaining = Math.max(0, rect.height() - fixedTotal - separators);
            boolean distributeFixedExtra = expandedCount == 0
                && remaining > 0
                && mode == GeneratedWindowMode.FULLSCREEN;
            boolean appendTrailingExtra = expandedCount == 0
                && remaining > 0
                && mode != GeneratedWindowMode.FULLSCREEN
                && horizontalSeparator > 0;
            int expandTargets = distributeFixedExtra ? node.children.size() : expandedCount;
            int share = expandTargets > 0 ? remaining / expandTargets : 0;
            int remainder = expandTargets > 0 ? remaining % expandTargets : 0;
            int expandIndex = 0;
            int cursor = rect.row();
            for (int index = 0; index < node.children.size(); ++index)
            {
                RuntimeNode child = node.children.get(index);
                int childHeight = child.logicalHeight == null ? 0 : child.logicalHeight;
                if (child.logicalHeight == null || distributeFixedExtra)
                {
                    childHeight = (distributeFixedExtra ? childHeight : 0)
                        + share + (expandIndex < remainder ? 1 : 0);
                    ++expandIndex;
                }
                else if (appendTrailingExtra && index + 1 == node.children.size())
                {
                    childHeight += remaining;
                }
                int resolvedHeight = Math.max(MINIMUM_RENDERABLE_SIZE, childHeight);
                resolveRuntimeNode(
                    child,
                    new Rect(cursor, rect.col(), rect.width(), resolvedHeight),
                    resolved,
                    horizontalSeparator,
                    verticalSeparator,
                    mode);
                cursor += resolvedHeight + horizontalSeparator;
            }
            return;
        }

        int fixedTotal = 0;
        int expandedCount = 0;
        for (RuntimeNode child : node.children)
        {
            if (child.logicalWidth == null)
            {
                ++expandedCount;
            }
            else
            {
                fixedTotal += child.logicalWidth;
            }
        }
        int separators = Math.max(0, node.children.size() - 1) * verticalSeparator;
        int remaining = Math.max(0, rect.width() - fixedTotal - separators);
        boolean distributeFixedExtra = expandedCount == 0
            && remaining > 0
            && mode == GeneratedWindowMode.FULLSCREEN;
        boolean appendTrailingExtra = expandedCount == 0
            && remaining > 0
            && mode != GeneratedWindowMode.FULLSCREEN;
        int expandTargets = distributeFixedExtra ? node.children.size() : expandedCount;
        int share = expandTargets > 0 ? remaining / expandTargets : 0;
        int remainder = expandTargets > 0 ? remaining % expandTargets : 0;
        int expandIndex = 0;
        int cursor = rect.col();
        for (int index = 0; index < node.children.size(); ++index)
        {
            RuntimeNode child = node.children.get(index);
            int childWidth = child.logicalWidth == null ? 0 : child.logicalWidth;
            if (child.logicalWidth == null || distributeFixedExtra)
            {
                childWidth = (distributeFixedExtra ? childWidth : 0)
                    + share + (expandIndex < remainder ? 1 : 0);
                ++expandIndex;
            }
            else if (appendTrailingExtra && index + 1 == node.children.size())
            {
                childWidth += remaining;
            }
            int resolvedWidth = Math.max(MINIMUM_RENDERABLE_SIZE, childWidth);
            resolveRuntimeNode(
                child,
                new Rect(rect.row(), cursor, resolvedWidth, rect.height()),
                resolved,
                horizontalSeparator,
                verticalSeparator,
                mode);
            cursor += resolvedWidth + verticalSeparator;
        }
    }

    private static GeneratedWindowMode windowMode(GeneratedWindowBase window)
    {
        boolean expandsWidth = false;
        boolean expandsHeight = false;
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            expandsWidth = expandsWidth || entry.cellWidth().mode() == DimensionMode.EXPANDED;
            expandsHeight = expandsHeight || entry.cellHeight().mode() == DimensionMode.EXPANDED;
        }
        if (expandsWidth && expandsHeight)
        {
            return GeneratedWindowMode.FULLSCREEN;
        }
        if (expandsWidth)
        {
            return GeneratedWindowMode.EXPAND_WIDTH;
        }
        if (expandsHeight)
        {
            return GeneratedWindowMode.EXPAND_HEIGHT;
        }
        return GeneratedWindowMode.NORMAL;
    }

    private static Size layoutSize(RuntimeNode root)
    {
        if (root == null)
        {
            return new Size(MINIMUM_RENDERABLE_SIZE, MINIMUM_RENDERABLE_SIZE);
        }
        int sourceWidth = Math.max(
            MINIMUM_RENDERABLE_SIZE,
            root.right - root.left - MINIMUM_RENDERABLE_SIZE);
        int sourceHeight = Math.max(
            MINIMUM_RENDERABLE_SIZE,
            root.bottom - root.top - MINIMUM_RENDERABLE_SIZE);
        int width = Math.max(
            MINIMUM_RENDERABLE_SIZE,
            root.logicalWidth == null ? sourceWidth : root.logicalWidth);
        int height = Math.max(
            MINIMUM_RENDERABLE_SIZE,
            root.logicalHeight == null ? sourceHeight : root.logicalHeight);
        return new Size(width, height);
    }

    private static Rect layoutContentRect(
        RuntimeNode runtimeTree,
        Rect content,
        GeneratedWindowMode mode)
    {
        Size naturalContent = layoutSize(runtimeTree);
        int width = content.width();
        int height = content.height();
        if (mode == GeneratedWindowMode.FULLSCREEN
            || mode == GeneratedWindowMode.EXPAND_WIDTH)
        {
            width = Math.max(width, naturalContent.width());
        }
        if (mode == GeneratedWindowMode.FULLSCREEN
            || mode == GeneratedWindowMode.EXPAND_HEIGHT)
        {
            height = Math.max(height, naturalContent.height());
        }
        return new Rect(content.row(), content.col(), width, height);
    }

    private static List<ResolvedRuntimeCell> resolveRuntimeCellsWithFitPass(
        GeneratedWindowBase window,
        List<RuntimeCell> runtimeCells,
        Rect content,
        int horizontalSeparator,
        int verticalSeparator,
        GeneratedWindowMode mode)
    {
        RuntimeNode runtimeTree = buildRuntimeTreeFromCells(
            runtimeCells,
            horizontalSeparator,
            verticalSeparator);
        List<ResolvedRuntimeCell> resolvedCells = new ArrayList<>();
        resolveRuntimeNode(
            runtimeTree,
            layoutContentRect(runtimeTree, content, mode),
            resolvedCells,
            horizontalSeparator,
            verticalSeparator,
            mode);
        if (!recomputeFitContentHeightsPass2(window, runtimeCells, resolvedCells))
        {
            return resolvedCells;
        }
        runtimeTree = buildRuntimeTreeFromCells(
            runtimeCells,
            horizontalSeparator,
            verticalSeparator);
        resolvedCells = new ArrayList<>();
        resolveRuntimeNode(
            runtimeTree,
            layoutContentRect(runtimeTree, content, mode),
            resolvedCells,
            horizontalSeparator,
            verticalSeparator,
            mode);
        return resolvedCells;
    }

    private static boolean recomputeFitContentHeightsPass2(
        GeneratedWindowBase window,
        List<RuntimeCell> cells,
        List<ResolvedRuntimeCell> resolvedCells)
    {
        boolean changed = false;
        for (RuntimeCell runtimeCell : cells)
        {
            GeneratedLayoutEntry cellEntry = null;
            for (GeneratedLayoutEntry entry : window.generatedLayout())
            {
                if (sameRuntimeCell(runtimeCell, entry)
                    && entry.cellHeight().mode() == DimensionMode.FIT_CONTENT)
                {
                    cellEntry = entry;
                    break;
                }
            }
            if (cellEntry == null)
            {
                continue;
            }
            ResolvedRuntimeCell resolved = resolvedCellFor(resolvedCells, cellEntry);
            if (resolved == null)
            {
                continue;
            }
            int newHeight = fitCellHeight(window, cellEntry, resolved.rect().width());
            if (runtimeCell.logicalHeight == null || runtimeCell.logicalHeight != newHeight)
            {
                runtimeCell.logicalHeight = newHeight;
                changed = true;
            }
        }
        return changed;
    }

    private static Size resolvedContentExtent(
        List<ResolvedRuntimeCell> resolvedCells,
        Size fallback)
    {
        int width = Math.max(MINIMUM_RENDERABLE_SIZE, fallback.width());
        int height = Math.max(MINIMUM_RENDERABLE_SIZE, fallback.height());
        for (ResolvedRuntimeCell cell : resolvedCells)
        {
            width = Math.max(width, cell.rect().col() + cell.rect().width());
            height = Math.max(height, cell.rect().row() + cell.rect().height());
        }
        return new Size(width, height);
    }

    private static ResolvedRuntimeCell resolvedCellFor(
        List<ResolvedRuntimeCell> cells,
        GeneratedLayoutEntry entry)
    {
        for (ResolvedRuntimeCell cell : cells)
        {
            if (sameRuntimeCell(cell.cell(), entry))
            {
                return cell;
            }
        }
        return null;
    }

    private static int dimensionWithin(
        AxisDimension dimension,
        int requestedAvailable,
        int fallback)
    {
        int available = Math.max(MINIMUM_RENDERABLE_SIZE, requestedAvailable);
        if (dimension.mode() == DimensionMode.EXPANDED)
        {
            return available;
        }
        if (dimension.mode() == DimensionMode.FIT_CONTENT)
        {
            return Math.max(
                MINIMUM_RENDERABLE_SIZE,
                Math.min(Math.max(MINIMUM_RENDERABLE_SIZE, fallback), available));
        }
        int requested = dimension.value() > 0 ? dimension.value() : fallback;
        return Math.max(MINIMUM_RENDERABLE_SIZE, requested);
    }

    private static int fitContentHeightFor(
        Element element,
        String type,
        int fallback,
        Integer width)
    {
        if (element == null)
        {
            return Math.max(MINIMUM_RENDERABLE_SIZE, fallback);
        }
        if (element instanceof ListBox listBox && !listBox.options().isEmpty())
        {
            return Math.max(MINIMUM_RENDERABLE_SIZE, listBox.options().size());
        }
        if (element instanceof MessageTable messageTable)
        {
            return Math.max(MINIMUM_RENDERABLE_SIZE, messageTable.fitContentHeight());
        }
        if (element instanceof Label label)
        {
            if (width != null && !"spanlabel".equals(type) && !"infolabel".equals(type))
            {
                return Math.max(
                    MINIMUM_RENDERABLE_SIZE,
                    RenderHelpers.wrapText(label.text(), width).size());
            }
            int lines = textLineCount(label.text());
            return Math.max(MINIMUM_RENDERABLE_SIZE, lines);
        }
        return Math.max(MINIMUM_RENDERABLE_SIZE, fallback);
    }

    private static int textLineCount(String text)
    {
        if (text == null || text.isEmpty())
        {
            return 0;
        }
        int lines = MINIMUM_RENDERABLE_SIZE;
        for (int index = 0; index < text.length(); ++index)
        {
            if (text.charAt(index) == '\n')
            {
                ++lines;
            }
        }
        return lines;
    }

    private static Size renderSizeFor(
        GeneratedLayoutEntry entry,
        Rect cellRect,
        Element element)
    {
        int availableWidth = cellRect.width()
            - paddingLeft(entry.cellStyle())
            - paddingRight(entry.cellStyle())
            - entry.relative().col()
            - entry.marginRight();
        int availableHeight = cellRect.height()
            - paddingTop(entry.cellStyle())
            - paddingBottom(entry.cellStyle())
            - entry.relative().row()
            - entry.marginBottom();
        int fallbackWidth = entry.charsSize().width() > 0
            ? entry.charsSize().width()
            : entry.sourceCell().width();
        int fallbackHeight = entry.charsSize().height() > 0
            ? entry.charsSize().height()
            : entry.sourceCell().height();
        if (entry.height().mode() == DimensionMode.FIT_CONTENT)
        {
            fallbackHeight = fitContentHeightFor(
                element,
                entry.type(),
                fallbackHeight,
                Math.max(MINIMUM_RENDERABLE_SIZE, availableWidth));
        }
        if (element == null && "label".equals(entry.type()))
        {
            int availableStaticWidth = cellRect.width()
                - paddingLeft(entry.cellStyle())
                - entry.relative().col();
            return new Size(
                Math.max(
                    MINIMUM_RENDERABLE_SIZE,
                    Math.min(
                        Math.max(MINIMUM_RENDERABLE_SIZE, fallbackWidth),
                        Math.max(MINIMUM_RENDERABLE_SIZE, availableStaticWidth))),
                dimensionWithin(entry.height(), availableHeight, fallbackHeight));
        }
        return new Size(
            dimensionWithin(entry.width(), availableWidth, fallbackWidth),
            dimensionWithin(entry.height(), availableHeight, fallbackHeight));
    }

    private static boolean hasExpandedEntryBeforeInSourceRow(
        List<GeneratedLayoutEntry> layout,
        GeneratedLayoutEntry entry)
    {
        for (GeneratedLayoutEntry other : layout)
        {
            if (sameSourceCell(other, entry)
                && other.relative().row() == entry.relative().row()
                && other.relative().col() < entry.relative().col()
                && other.width().mode() == DimensionMode.EXPANDED)
            {
                return true;
            }
        }
        return false;
    }

    private static IntSpan entrySourceColSpan(GeneratedLayoutEntry entry)
    {
        int width = Math.max(
            MINIMUM_RENDERABLE_SIZE,
            entry.charsSize().width() > 0
                ? entry.charsSize().width()
                : entry.sourceCell().width());
        return new IntSpan(entry.relative().col(), entry.relative().col() + width);
    }

    private static boolean sourceSpansOverlap(IntSpan first, IntSpan second)
    {
        return first.start() < second.end() && second.start() < first.end();
    }

    private static boolean hasExpandedEntryBeforeInSourceColumn(
        List<GeneratedLayoutEntry> layout,
        GeneratedLayoutEntry entry)
    {
        IntSpan entrySpan = entrySourceColSpan(entry);
        for (GeneratedLayoutEntry other : layout)
        {
            if (sameSourceCell(other, entry)
                && other.relative().row() < entry.relative().row()
                && sourceSpansOverlap(entrySourceColSpan(other), entrySpan)
                && other.height().mode() == DimensionMode.EXPANDED)
            {
                return true;
            }
        }
        return false;
    }

    private static int renderColFor(
        List<GeneratedLayoutEntry> layout,
        GeneratedLayoutEntry entry,
        Rect cellRect,
        Size size)
    {
        int baseCol = paddingLeft(entry.cellStyle()) + entry.relative().col();
        int cellWidthDelta = Math.max(0, cellRect.width() - entry.cellCharsSize().width());
        boolean followsExpandedEntry = hasExpandedEntryBeforeInSourceRow(layout, entry);
        if (entry.marginRight() == 0
            && entry.cellWidth().mode() == DimensionMode.EXPANDED
            && entry.cellCharsSize().width() > 0
            && cellRect.width() > entry.cellCharsSize().width()
            && entry.width().mode() != DimensionMode.EXPANDED)
        {
            return cellRect.col() + Math.max(
                baseCol,
                cellRect.width() - size.width() - entry.marginRight()
                    - paddingRight(entry.cellStyle()));
        }
        if (followsExpandedEntry
            && entry.cellWidth().mode() == DimensionMode.EXPANDED
            && entry.cellCharsSize().width() > 0
            && cellWidthDelta > 0
            && entry.width().mode() != DimensionMode.EXPANDED)
        {
            return cellRect.col() + Math.max(baseCol, baseCol + cellWidthDelta);
        }
        return cellRect.col() + baseCol;
    }

    private static int renderRowFor(
        List<GeneratedLayoutEntry> layout,
        GeneratedLayoutEntry entry,
        Rect cellRect)
    {
        int baseRow = paddingTop(entry.cellStyle()) + entry.relative().row();
        int cellHeightDelta = Math.max(0, cellRect.height() - entry.cellCharsSize().height());
        boolean followsExpandedEntry = hasExpandedEntryBeforeInSourceColumn(layout, entry);
        if (followsExpandedEntry
            && entry.cellHeight().mode() == DimensionMode.EXPANDED
            && entry.cellCharsSize().height() > 0
            && cellHeightDelta > 0
            && entry.height().mode() != DimensionMode.EXPANDED)
        {
            return cellRect.row() + Math.max(baseRow, baseRow + cellHeightDelta);
        }
        return cellRect.row() + baseRow;
    }

    private static void fill(TerminalBuffer buffer, Rect rect, Style style)
    {
        if (style == null)
        {
            return;
        }
        List<String> textureGlyphs = unicodeGlyphs(style.backgroundTexture());
        int firstRow = Math.max(0, rect.row());
        int firstCol = Math.max(0, rect.col());
        int lastRow = Math.min(buffer.height(), rect.row() + Math.max(0, rect.height()));
        int lastCol = Math.min(buffer.width(), rect.col() + Math.max(0, rect.width()));
        for (int row = firstRow; row < lastRow; ++row)
        {
            for (int col = firstCol; col < lastCol; ++col)
            {
                TerminalCell cell = new TerminalCell();
                if (!textureGlyphs.isEmpty())
                {
                    int textureWidth = Math.max(MINIMUM_RENDERABLE_SIZE, rect.width());
                    int textureIndex = ((row - rect.row()) * textureWidth + col - rect.col())
                        % textureGlyphs.size();
                    cell.setText(textureGlyphs.get(textureIndex));
                    cell.setForeground(style.backgroundTextureColor() == null
                        ? style.color()
                        : style.backgroundTextureColor());
                }
                else
                {
                    cell.setForeground(style.color());
                }
                cell.setBackground(style.background());
                TerminalCell existing = buffer.cell(row, col);
                cell.setBackground(blendBackgroundOverExisting(
                    cell.background(),
                    existing.background()));
                cell.setForeground(blendForegroundOverBackground(
                    cell.foreground(),
                    existing.foreground(),
                    cell.background()));
                buffer.setCell(row, col, cell);
            }
        }
    }

    private static void blit(
        TerminalBuffer target,
        List<List<TerminalCell>> source,
        int targetRow,
        int targetCol)
    {
        blitClipped(
            target,
            source,
            targetRow,
            targetCol,
            new Rect(0, 0, target.width(), target.height()));
    }

    private static void blitClipped(
        TerminalBuffer target,
        List<List<TerminalCell>> source,
        int targetRow,
        int targetCol,
        Rect clip)
    {
        for (int row = 0; row < source.size(); ++row)
        {
            List<TerminalCell> sourceRow = source.get(row);
            for (int col = 0; col < sourceRow.size(); ++col)
            {
                int resolvedRow = targetRow + row;
                int resolvedCol = targetCol + col;
                if (!clip.contains(new Point(resolvedRow, resolvedCol))
                    || resolvedRow < 0 || resolvedRow >= target.height()
                    || resolvedCol < 0 || resolvedCol >= target.width())
                {
                    continue;
                }
                TerminalCell cell = sourceRow.get(col).copy();
                TerminalCell existing = target.cell(resolvedRow, resolvedCol);
                cell.setBackground(blendBackgroundOverExisting(
                    cell.background(),
                    existing.background()));
                cell.setForeground(blendForegroundOverBackground(
                    cell.foreground(),
                    existing.foreground(),
                    cell.background()));
                target.setCell(resolvedRow, resolvedCol, cell);
            }
        }
    }

    private static Rect cellContentClip(GeneratedLayoutEntry entry, Rect cell)
    {
        int top = paddingTop(entry.cellStyle());
        int right = paddingRight(entry.cellStyle());
        int bottom = paddingBottom(entry.cellStyle());
        int left = paddingLeft(entry.cellStyle());
        return new Rect(
            cell.row() + top,
            cell.col() + left,
            Math.max(0, cell.width() - left - right),
            Math.max(0, cell.height() - top - bottom));
    }

    private static void overlayHostViewportRootScrollViewIndicators(
        GeneratedWindowBase window,
        List<List<TerminalCell>> rendered,
        Size contentSize)
    {
        ScrollView scrollView = window.generatedScrollView();
        if (scrollView == null)
        {
            return;
        }
        Rect scrollFrame = generatedScrollViewSourceFrame(window, scrollView, contentSize);
        Rect viewport = scrollViewViewportClipClampedToContent(
            scrollFrame,
            scrollView.style(),
            contentSize);
        if (viewport.width() <= 0 || viewport.height() <= 0)
        {
            return;
        }

        boolean childContentAboveViewport = false;
        boolean childContentBelowViewport = false;
        for (ScrollViewChildView childView : scrollView.childViews(
            new Size(scrollFrame.width(), scrollFrame.height())))
        {
            Rect childFrame = new Rect(
                scrollFrame.row() + childView.frame().row(),
                scrollFrame.col() + childView.frame().col(),
                childView.frame().width(),
                childView.frame().height());
            if (childFrame.row() < viewport.row())
            {
                childContentAboveViewport = true;
            }
            if (childFrame.row() + childFrame.height()
                > viewport.row() + viewport.height())
            {
                childContentBelowViewport = true;
            }
        }

        Size viewportSize = new Size(viewport.width(), viewport.height());
        int indicatorCol = viewport.col() + viewport.width() - MINIMUM_RENDERABLE_SIZE;
        int maximumViewOffset = scrollView.maxViewOffset(viewportSize);
        int viewOffset = scrollView.viewOffset();
        if (viewOffset < maximumViewOffset || childContentAboveViewport)
        {
            applyScrollIndicator(
                rendered,
                viewport.row(),
                indicatorCol,
                SCROLL_INDICATOR_ABOVE,
                viewport.col());
        }
        if (viewOffset > 0 || childContentBelowViewport)
        {
            applyScrollIndicator(
                rendered,
                viewport.row() + viewport.height() - MINIMUM_RENDERABLE_SIZE,
                indicatorCol,
                SCROLL_INDICATOR_BELOW,
                viewport.col());
        }
    }

    private static Rect generatedScrollViewSourceFrame(
        GeneratedWindowBase window,
        ScrollView scrollView,
        Size fallback)
    {
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            if (entry.name().equals(scrollView.name()))
            {
                return entry.sourceCell();
            }
        }
        return new Rect(0, 0, fallback.width(), fallback.height());
    }

    private static Rect scrollViewViewportClipClampedToContent(
        Rect frame,
        Style style,
        Size contentSize)
    {
        int left = paddingLeft(style);
        int right = paddingRight(style);
        int top = paddingTop(style);
        int bottom = paddingBottom(style);
        int width = Math.max(0, frame.width() - left - right);
        int height = Math.max(0, frame.height() - top - bottom);
        int row = frame.row() + top;
        int col = frame.col() + left;
        return new Rect(
            row,
            col,
            Math.min(width, Math.max(0, contentSize.width() - col)),
            Math.min(height, Math.max(0, contentSize.height() - row)));
    }

    private static void applyScrollIndicator(
        List<List<TerminalCell>> content,
        int row,
        int col,
        String indicator,
        int foregroundStartCol)
    {
        if (row < 0 || row >= content.size())
        {
            return;
        }
        List<TerminalCell> renderedRow = content.get(row);
        if (col < 0 || col >= renderedRow.size())
        {
            return;
        }
        TerminalCell cell = renderedRow.get(col);
        cell.setText(indicator);
        if (transparentColor(cell.foreground()))
        {
            cell.setForeground(inferredScrollIndicatorForeground(
                renderedRow,
                foregroundStartCol,
                col));
        }
    }

    private static Color inferredScrollIndicatorForeground(
        List<TerminalCell> row,
        int startCol,
        int endCol)
    {
        Color foreground = null;
        int start = Math.max(0, startCol);
        int end = Math.min(row.size() - MINIMUM_RENDERABLE_SIZE, endCol);
        for (int col = start; col <= end; ++col)
        {
            Color scanned = row.get(col).foreground();
            if (!transparentColor(scanned))
            {
                foreground = scanned;
            }
        }
        return foreground;
    }

    private static List<String> unicodeGlyphs(String value)
    {
        List<String> glyphs = new ArrayList<>();
        if (value == null)
        {
            return glyphs;
        }
        value.codePoints().forEach(codePoint -> glyphs.add(
            new String(Character.toChars(codePoint))));
        return glyphs;
    }

    private static Color blendBackgroundOverExisting(Color color, Color existing)
    {
        if (transparentColor(color))
        {
            return existing;
        }
        if (hasPartialAlpha(color) && existing != null)
        {
            return color.blendOver(existing);
        }
        return color;
    }

    private static Color blendForegroundOverBackground(
        Color color,
        Color inherited,
        Color background)
    {
        if (transparentColor(color))
        {
            return inherited;
        }
        if (hasPartialAlpha(color) && background != null)
        {
            return color.blendOver(background);
        }
        return color;
    }

    private static boolean transparentColor(Color color)
    {
        return color == null || !color.isSet() || color.isTransparent();
    }

    private static boolean hasPartialAlpha(Color color)
    {
        return color != null
            && color.rgba().isPresent()
            && color.rgba().orElseThrow().alpha() < 255;
    }

    private static List<List<TerminalCell>> contentFromBuffer(TerminalBuffer buffer)
    {
        List<List<TerminalCell>> result = new ArrayList<>(buffer.height());
        for (int row = 0; row < buffer.height(); ++row)
        {
            List<TerminalCell> outputRow = new ArrayList<>(buffer.width());
            for (int col = 0; col < buffer.width(); ++col)
            {
                outputRow.add(buffer.cell(row, col).copy());
            }
            result.add(outputRow);
        }
        return result;
    }

    private static Element findElementInTree(Element element, String name)
    {
        if (element.name().equals(name))
        {
            return element;
        }
        for (Element child : element.children())
        {
            Element found = findElementInTree(child, name);
            if (found != null)
            {
                return found;
            }
        }
        if (element instanceof ReusableElement reusable && reusable.child() != null)
        {
            return findElement(reusable.child(), name);
        }
        return null;
    }

    private static void addFocusableElements(
        Element element,
        ScrollView activeScrollView,
        List<Element> output)
    {
        if (element instanceof ReusableElement reusable && reusable.child() != null)
        {
            syncReusableChildFrames(reusable);
            ScrollView generatedScrollView = reusable.child().generatedScrollView();
            if (generatedScrollView != null)
            {
                if (reusable.enabled() && reusable.focusable())
                {
                    output.add(reusable);
                }
                if (activeScrollView == generatedScrollView)
                {
                    for (Element child : focusableElements(reusable.child(), activeScrollView))
                    {
                        if (child != generatedScrollView)
                        {
                            output.add(child);
                        }
                    }
                }
                return;
            }
            List<Element> childFocusable = focusableElements(reusable.child(), activeScrollView);
            if (childFocusable.isEmpty()
                && reusable.enabled()
                && reusable.focusable()
                && reusable.child().generatedFocusable()
                && !(reusable instanceof ViewHost))
            {
                output.add(reusable);
            }
            else
            {
                output.addAll(childFocusable);
            }
            return;
        }
        if (element instanceof ScrollView scrollView)
        {
            if (isFocusable(scrollView))
            {
                output.add(scrollView);
            }
            if (activeScrollView == scrollView)
            {
                syncScrollViewChildFrames(scrollView);
                for (ScrollViewChildView childView : scrollView.childViews(frameSize(scrollView)))
                {
                    Element child = childView.element();
                    if (child == null)
                    {
                        continue;
                    }
                    addFocusableElements(child, activeScrollView, output);
                }
            }
            return;
        }
        if (isFocusable(element))
        {
            output.add(element);
        }
        for (Element child : element.children())
        {
            addFocusableElements(child, activeScrollView, output);
        }
    }

    private static Size frameSize(Element element)
    {
        return new Size(
            Math.max(MINIMUM_RENDERABLE_SIZE, element.frame().width()),
            Math.max(MINIMUM_RENDERABLE_SIZE, element.frame().height()));
    }

    private static boolean isFocusableType(Element element)
    {
        return element instanceof Button
            || element instanceof CheckBox
            || element instanceof TextInput
            || element instanceof NumberInput
            || element instanceof ComboBox
            || element instanceof ListBox
            || element instanceof ScrollView;
    }

    private static void syncReusableChildFrames(ReusableElement reusable)
    {
        if (reusable.child() == null)
        {
            return;
        }
        syncWindowElementFramesTo(reusable.child(), reusable.frame(), true);
    }

    private static void syncWindowElementFramesTo(GeneratedWindowBase window, Rect origin)
    {
        syncWindowElementFramesTo(window, origin, false);
    }

    private static void syncWindowElementFramesTo(
        GeneratedWindowBase window,
        Rect origin,
        boolean forceFullscreenLayout)
    {
        Size size = new Size(
            Math.max(MINIMUM_RENDERABLE_SIZE, origin.width()),
            Math.max(MINIMUM_RENDERABLE_SIZE, origin.height()));
        Style style = window.generatedWindowStyle();
        List<RuntimeCell> runtimeCells = collectRuntimeCells(window);
        List<ResolvedRuntimeCell> resolvedCells = resolveRuntimeCellsWithFitPass(
            window,
            runtimeCells,
            new Rect(0, 0, size.width(), size.height()),
            borderWidthHorizontal(style),
            borderWidthVertical(style),
            forceFullscreenLayout ? GeneratedWindowMode.FULLSCREEN : windowMode(window));
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            Element element = findElement(window, entry.name());
            if (element == null)
            {
                continue;
            }
            ResolvedRuntimeCell cell = resolvedCellFor(resolvedCells, entry);
            if (cell == null)
            {
                continue;
            }
            Size elementSize = renderSizeFor(entry, cell.rect(), element);
            element.setFrame(new Rect(
                origin.row() + renderRowFor(window.generatedLayout(), entry, cell.rect()),
                origin.col() + renderColFor(
                    window.generatedLayout(),
                    entry,
                    cell.rect(),
                    elementSize),
                elementSize.width(),
                elementSize.height()));
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                syncReusableChildFrames(reusable);
            }
            else if (element instanceof ScrollView scrollView)
            {
                syncScrollViewChildFrames(scrollView);
            }
        }
    }

    private static void syncScrollViewChildFrames(ScrollView scrollView)
    {
        Rect frame = scrollView.frame();
        for (ScrollViewChildView childView : scrollView.childViews(frameSize(scrollView)))
        {
            Element child = childView.element();
            if (child == null)
            {
                continue;
            }
            child.setFrame(new Rect(
                frame.row() + childView.frame().row(),
                frame.col() + childView.frame().col(),
                childView.frame().width(),
                childView.frame().height()));
            if (child instanceof ReusableElement reusable && reusable.child() != null)
            {
                syncReusableChildFrames(reusable);
            }
        }
    }
}
