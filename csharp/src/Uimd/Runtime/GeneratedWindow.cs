using System.Diagnostics;
using System.Globalization;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;
using System.Text.Json.Nodes;

namespace Uimd;

public sealed class GeneratedLayoutEntry
{
    public string Name { get; set; } = "";
    public string Type { get; set; } = "";
    public string CellName { get; set; } = "";
    public Rect Relative { get; set; }
    public Rect SourceCell { get; set; }
    public AxisDimension Width { get; set; }
    public AxisDimension Height { get; set; }
    public AxisDimension CellWidth { get; set; }
    public AxisDimension CellHeight { get; set; }
    public Size CharsSize { get; set; }
    public Size CellCharsSize { get; set; }
    public int MarginRight { get; set; }
    public int MarginBottom { get; set; }
    public Style CellStyle { get; set; } = new();
    public Style ElementStyle { get; set; } = new();
}

public sealed class GeneratedElementMetadata
{
    public string Name { get; set; } = "";
    public string Description { get; set; } = "";
    public bool Expose { get; set; } = true;
}

public sealed class GeneratedAppToolMetadata
{
    public string Name { get; set; } = "";
    public string Description { get; set; } = "";
    public string InputSchemaJson { get; set; } = "";
    public string OutputSchemaJson { get; set; } = "";
}

public class Window
{
    public string Title { get; }
    public List<Element> Elements { get; } = new();

    public Window(string title = "")
    {
        Title = title;
    }

    public T AddElement<T>(T element) where T : Element
    {
        Elements.Add(element);
        return element;
    }
}

public class GeneratedWindowBase : Window
{
    private List<GeneratedLayoutEntry> generatedLayout = new();
    private Style generatedWindowStyle = new();
    private readonly List<GeneratedElementMetadata> mcpElementMetadata = new();
    private readonly List<GeneratedAppToolMetadata> mcpAppTools = new();
    private Action<GeneratedWindowBase, GeneratedWindowRuntimeOptions?>? runtimeOpenWindow;
    private Action? runtimeCloseWindow;
    private Action<GeneratedWindowBase, Element?>? runtimeSetFocus;
    private Element? requestedFocusElement;

    public bool GeneratedFocusable { get; private set; }
    public string GeneratedKind { get; private set; } = "window";
    public bool McpEnabled { get; private set; } = true;
    public string McpClassName { get; private set; } = "";
    public string McpSourceMarkdown { get; private set; } = "";
    public string McpWindowDescription { get; private set; } = "";
    public string Mode { get; set; } = "";

    public GeneratedWindowBase(string title = "") : base(title)
    {
    }

    public IReadOnlyList<GeneratedLayoutEntry> GeneratedLayout => generatedLayout;
    public Style GeneratedWindowStyle => generatedWindowStyle;
    public IReadOnlyList<GeneratedAppToolMetadata> McpAppTools => mcpAppTools;

    public GeneratedElementMetadata? McpElementMetadata(string name)
    {
        return mcpElementMetadata.FirstOrDefault(item => item.Name == name);
    }

    protected void SetGeneratedLayout(IEnumerable<GeneratedLayoutEntry> layout)
    {
        generatedLayout = layout.ToList();
    }

    protected void SetGeneratedWindowStyle(Style style)
    {
        generatedWindowStyle = style;
    }

    protected void SetGeneratedFocusable(bool focusable)
    {
        GeneratedFocusable = focusable;
    }

    protected void SetGeneratedKind(string kind)
    {
        GeneratedKind = kind;
    }

    protected void SetMcpMetadata(
        bool enabled,
        string className,
        string sourceMarkdown,
        string windowDescription,
        IEnumerable<GeneratedElementMetadata> elementMetadata)
    {
        McpEnabled = enabled;
        McpClassName = className;
        McpSourceMarkdown = sourceMarkdown;
        McpWindowDescription = windowDescription;
        mcpElementMetadata.Clear();
        mcpElementMetadata.AddRange(elementMetadata);
    }

    protected void SetMcpAppTools(IEnumerable<GeneratedAppToolMetadata> appTools)
    {
        mcpAppTools.Clear();
        mcpAppTools.AddRange(appTools);
    }

    public virtual string CallAppTool(string name, string inputJson)
    {
        throw new InvalidOperationException("App MCP tool is not implemented: " + name);
    }

    public virtual bool HandleGeneratedButton(string name)
    {
        return false;
    }

    public virtual bool HandleGeneratedTextChanged(string name, string value)
    {
        return false;
    }

    public virtual bool HandleGeneratedTextConfirmed(string name, string value)
    {
        return false;
    }

    public virtual bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        return false;
    }

    public virtual bool ActivateGeneratedControl()
    {
        return false;
    }

    public virtual GeneratedWindowRuntimeOptions RuntimeOptions()
    {
        GeneratedWindowRuntimeOptions options = new();
        options.OnButton = name => HandleGeneratedButton(name);
        options.OnTextChanged = (name, value) => HandleGeneratedTextChanged(name, value);
        options.OnTextConfirmed = (name, value) => HandleGeneratedTextConfirmed(name, value);
        options.OnSelectionChanged = (name, value) => HandleGeneratedSelectionChanged(name, value);
        options.ShouldClose = shouldClose;
        return options;
    }

    protected virtual bool shouldClose()
    {
        return false;
    }

    public void SetFocus(Element? element)
    {
        if (element is not null && !element.Enabled)
        {
            element = null;
        }
        requestedFocusElement = element;
        runtimeSetFocus?.Invoke(this, element);
    }

    internal Element? ConsumeRequestedFocusElement()
    {
        Element? element = requestedFocusElement;
        requestedFocusElement = null;
        return element;
    }

    internal void AttachRuntimeWindowStack(
        Action<GeneratedWindowBase, GeneratedWindowRuntimeOptions?> openWindow,
        Action closeWindow,
        Action<GeneratedWindowBase, Element?> setFocus)
    {
        runtimeOpenWindow = openWindow;
        runtimeCloseWindow = closeWindow;
        runtimeSetFocus = setFocus;
    }

    protected void OpenWindow(GeneratedWindowBase window)
    {
        runtimeOpenWindow?.Invoke(window, null);
    }

    protected void OpenWindow(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options)
    {
        runtimeOpenWindow?.Invoke(window, options);
    }

    protected void CloseWindow()
    {
        runtimeCloseWindow?.Invoke();
    }
}

public class GeneratedScrollViewBase : GeneratedWindowBase
{
    private ScrollView? generatedScrollView;

    public GeneratedScrollViewBase(string title = "") : base(title)
    {
    }

    public ScrollView ScrollView()
    {
        return generatedScrollView ?? throw new InvalidOperationException("generated scroll view has not been initialized");
    }

    protected void SetGeneratedScrollView(ScrollView scrollView)
    {
        generatedScrollView = scrollView;
    }

    public void SetAutoScroll(bool autoScroll)
    {
        ScrollView().SetAutoScroll(autoScroll);
    }

    public bool ScrollToTop()
    {
        ScrollView scrollView = ScrollView();
        return scrollView.ScrollToTop(new Size(scrollView.Frame.Width, scrollView.Frame.Height));
    }

    public bool ScrollToBottom()
    {
        ScrollView scrollView = ScrollView();
        return scrollView.ScrollToBottom(new Size(scrollView.Frame.Width, scrollView.Frame.Height));
    }

    public ScrollViewPosition ScrollPosition()
    {
        return ScrollView().ScrollPosition();
    }

    public void RestoreScrollPosition(ScrollViewPosition position)
    {
        ScrollView().RestoreScrollPosition(position);
    }

    public void ClearChildren()
    {
        ScrollView().ClearChildren();
    }

    public T AddChild<T>(T child) where T : Element
    {
        return ScrollView().AddChild(child);
    }

    public void AddChild(List<List<TerminalCell>> child)
    {
        ScrollView().AddChild(child);
    }

    public void SetDynamicChildrenRenderer(Func<int, List<List<List<TerminalCell>>>> renderer)
    {
        ScrollView().SetDynamicChildrenRenderer(renderer);
    }

    public void InvalidateDynamicChildren()
    {
        ScrollView().InvalidateDynamicChildren();
    }
}

public sealed class GeneratedWindowRuntimeOptions
{
    public string Footer { get; set; } = "";
    public string InitialFocusName { get; set; } = "";
    public bool StartInEditMode { get; set; }
    public bool DimBackground { get; set; } = true;
    public bool KeepEditModeAfterConfirm { get; set; }
    public bool KeepEditModeAfterEscape { get; set; }
    public Action<string>? OnButton { get; set; }
    public Func<string, string, bool, bool>? OnKeyBeforeFocusedElement { get; set; }
    public Func<Point, bool>? OnMousePressBeforeFocused { get; set; }
    public Func<string, bool>? OnKey { get; set; }
    public Action<string, string>? OnTextChanged { get; set; }
    public Action<string, string>? OnTextConfirmed { get; set; }
    public Action<string, List<string>>? OnSelectionChanged { get; set; }
    public Action<string>? OnEditStarted { get; set; }
    public Action<string, bool>? OnFocusChanged { get; set; }
    public Func<bool>? ShouldClose { get; set; }
}

public sealed class McpRuntimeConfig
{
    public const int DefaultViewportRow = 0;
    public const int DefaultViewportCol = 0;
    public const int DefaultViewportWidth = 90;
    public const int DefaultViewportHeight = 35;

    public bool Enabled { get; set; }
    public bool Gui { get; set; } = true;
    public string Transport { get; set; } = "tcp";
    public string Host { get; set; } = "127.0.0.1";
    public int Port { get; set; }
    public int ViewportRow { get; set; } = DefaultViewportRow;
    public int ViewportCol { get; set; } = DefaultViewportCol;
    public int ViewportWidth { get; set; } = DefaultViewportWidth;
    public int ViewportHeight { get; set; } = DefaultViewportHeight;
}

internal readonly record struct ScrollRegionHint(Rect Rect, int Delta);

public static class GeneratedWindowRuntime
{
    internal const int MinimumRenderableSize = 1;
    private const int ContentTopRow = 0;
    private const int ContentLeftCol = 0;
    private const int FallbackTerminalWidth = 100;
    private const int FallbackTerminalHeight = 32;
    private const char TerminalEscape = '\x1b';
    private const char TerminalCtrlC = '\x03';
    private const char TerminalCarriageReturn = '\r';
    private const char TerminalLineFeed = '\n';
    private const char TerminalTab = '\t';
    private const char TerminalBackspace = '\b';
    private const char TerminalDeleteBackspace = '\x7f';
    private const int DirectInputEscapeSequenceTimeoutMilliseconds = 25;
    private const int DirectInputEscapeSequencePollMilliseconds = 1;
    private const int DirectInputIdleSleepMilliseconds = 10;
    private const int AnimatedRenderIntervalMilliseconds = 70;
    private const int DialogButtonCloseDelayMilliseconds = 180;
    private const int CopyNotificationDurationMilliseconds = 3000;
    private const int CopyNotificationRow = 0;
    private const int CopyNotificationRightMargin = 1;
    private const int TerminalExitDrainTimeoutMilliseconds = 20;
    private const int TerminalExitDrainBytes = 1024;
    private const int TerminalExitDrainMaxReads = 64;
    private const int TerminalCellPixelResponseMaxBytes = 64;
    private const int TerminalCellPixelQueryTimeoutMilliseconds = 50;
    private const double ModalBackgroundDimFactor = 0.5;
    private const int TerminalCoordinateBase = 1;
    private const int SgrMouseFieldCount = 3;
    private const int SgrMouseDragMask = 32;
    private const int SgrMouseWheelUpButton = 64;
    private const int SgrMouseWheelDownButton = 65;
    private const int SgrMouseWheelUpDelta = 1;
    private const int SgrMouseWheelDownDelta = -1;
    private const int MaxCoalescedMouseWheelDelta = 12;
    internal const int TextInputWheelScrollRows = 1;
    private const int CsiPrefixLength = 2;
    private const int SgrMousePrefixLength = 3;
    private const int CtrlCCodepoint = 3;
    private const int UppercaseCCodepoint = 67;
    private const int LowercaseCCodepoint = 99;
    private const int UppercaseVCodepoint = 86;
    private const int LowercaseVCodepoint = 118;
    private const int CtrlModifier = 5;
    private const int CtrlShiftModifier = 6;
    private const int MetaModifier = 9;
    private const int MetaShiftModifier = 10;
    private const short PosixPollInput = 0x0001;
    private const string SgrMousePrefix = "\x1b[<";
    private const string BracketedPasteStart = "\x1b[200~";
    private const string BracketedPasteEnd = "\x1b[201~";
    private const string AnsiHome = "\x1b[H";
    private const string AnsiEnterAlternateScreen = "\x1b[?1049h\x1b[?1000h\x1b[?1002h\x1b[?1006h\x1b[?2004h\x1b[?7l\x1b[>4;2m\x1b[?25l\x1b[H\x1b[2J";
    private const string AnsiLeaveAlternateScreen = "\x1b[0m\x1b[2J\x1b[H\x1b[>4;0m\x1b[?25h\x1b[?7h\x1b[?2004l\x1b[?1006l\x1b[?1002l\x1b[?1000l\x1b[?1049l\r\n";
    private const string AnsiLeaveApplicationKeypadMode = "\x1b[?1l\x1b>";
    private const string TerminalTitlePrefix = "\x1b]0;";
    private const string TerminalTitleSuffix = " [C#]";
    private const string TerminalTitleTerminator = "\x07";
    private const string TerminalCellPixelQuery = "\x1b[16t";
    private const string TerminalTextAreaPixelQuery = "\x1b[14t";
    private const string TerminalCellPixelResponsePrefix = "\x1b[6;";
    private const string TerminalTextAreaPixelResponsePrefix = "\x1b[4;";
    private const char TerminalCellPixelResponseSeparator = ';';
    private const char TerminalCellPixelResponseTerminator = 't';
    private const string TerminalImageHalfBlockGlyph = "▀";
    private const string CopyNotificationText = "Copied to clipboard";
    private const string SixelFallbackWarningText = "Sixel is not supported. Continue with fallback image blocks?";
    private const int SttySuccessExitCode = 0;
    private const int WindowsStdInputHandle = -10;
    private const int WindowsStdOutputHandle = -11;
    private const int WindowsStdErrorHandle = -12;
    private const uint WindowsEnableProcessedInput = 0x0001;
    private const uint WindowsEnableLineInput = 0x0002;
    private const uint WindowsEnableEchoInput = 0x0004;
    private const uint WindowsEnableWindowInput = 0x0008;
    private const uint WindowsEnableMouseInput = 0x0010;
    private const uint WindowsEnableVirtualTerminalInput = 0x0200;
    private const uint WindowsEnableVirtualTerminalProcessing = 0x0004;
    private const uint WindowsDisableNewlineAutoReturn = 0x0008;
    private const int PosixStdinFileDescriptor = 0;
    private const int PosixStdoutFileDescriptor = 1;
    private const ulong LinuxTiocgwinsz = 0x5413;
    private const ulong DarwinTiocgwinsz = 0x40087468;
    private static readonly Color DefaultTerminalBackground = new("#000000");
    private static readonly Color CopyNotificationForeground = new("#ffffff");
    private static readonly Color CopyNotificationBackground = new("#2255bb");

    private enum GeneratedWindowMode
    {
        Normal,
        ExpandWidth,
        ExpandHeight,
        Fullscreen,
    }

    private enum DirectTerminalEventKind
    {
        Key,
        MousePress,
        MouseRelease,
        MouseDrag,
        MouseWheel,
    }

    private sealed class DirectTerminalEvent
    {
        private DirectTerminalEvent(DirectTerminalEventKind kind, string? key, Point position, int wheelDelta)
        {
            Kind = kind;
            Key = key;
            Position = position;
            WheelDelta = wheelDelta;
        }

        public DirectTerminalEventKind Kind { get; }
        public string? Key { get; }
        public Point Position { get; }
        public int WheelDelta { get; }

        public static DirectTerminalEvent FromKey(string key)
        {
            return new DirectTerminalEvent(DirectTerminalEventKind.Key, key, new Point(), 0);
        }

        public static DirectTerminalEvent FromMouse(DirectTerminalEventKind kind, Point position)
        {
            return new DirectTerminalEvent(kind, null, position, 0);
        }

        public static DirectTerminalEvent FromMouseWheel(Point position, int wheelDelta)
        {
            return new DirectTerminalEvent(DirectTerminalEventKind.MouseWheel, null, position, wheelDelta);
        }
    }

    private sealed class TerminalModeScope : IDisposable
    {
        private readonly string? savedPosixMode;
        private readonly List<(IntPtr Handle, uint Mode)> savedWindowsModes = new();
        private bool disposed;

        public TerminalModeScope()
        {
            if (OperatingSystem.IsWindows())
            {
                ConfigureWindowsConsoleMode();
            }
            else
            {
                savedPosixMode = CapturePosixTerminalMode();
                if (!string.IsNullOrWhiteSpace(savedPosixMode))
                {
                    SetPosixRawMode();
                }
            }
        }

        public void Dispose()
        {
            if (disposed)
            {
                return;
            }
            disposed = true;
            if (OperatingSystem.IsWindows())
            {
                RestoreWindowsConsoleMode();
            }
            else if (!string.IsNullOrWhiteSpace(savedPosixMode))
            {
                RestorePosixTerminalMode(savedPosixMode);
            }
        }

        private static string? CapturePosixTerminalMode()
        {
            try
            {
                using Process process = StartStty("-g", redirectOutput: true);
                string output = process.StandardOutput.ReadToEnd().Trim();
                process.WaitForExit();
                return process.ExitCode == SttySuccessExitCode ? output : null;
            }
            catch
            {
                return null;
            }
        }

        private static void SetPosixRawMode()
        {
            RunStty("raw", "-echo", "min", "0", "time", "1");
        }

        private static void RestorePosixTerminalMode(string savedMode)
        {
            RunStty(savedMode);
        }

        private static void RunStty(params string[] arguments)
        {
            try
            {
                using Process process = StartStty(arguments, redirectOutput: false);
                process.WaitForExit();
            }
            catch
            {
            }
        }

        private static Process StartStty(string argument, bool redirectOutput)
        {
            return StartStty(new[] { argument }, redirectOutput);
        }

        private static Process StartStty(string[] arguments, bool redirectOutput)
        {
            ProcessStartInfo startInfo = new("stty")
            {
                RedirectStandardOutput = redirectOutput,
                RedirectStandardError = true,
                UseShellExecute = false,
            };
            foreach (string argument in arguments)
            {
                startInfo.ArgumentList.Add(argument);
            }
            return Process.Start(startInfo) ?? throw new InvalidOperationException("failed to start stty");
        }

        private void ConfigureWindowsConsoleMode()
        {
            ConfigureWindowsInputMode(WindowsStdInputHandle);
            ConfigureWindowsOutputMode(WindowsStdOutputHandle);
            ConfigureWindowsOutputMode(WindowsStdErrorHandle);
        }

        private void ConfigureWindowsInputMode(int standardHandle)
        {
            IntPtr handle = GetStdHandle(standardHandle);
            if (handle == IntPtr.Zero || handle == new IntPtr(-1) || !GetConsoleMode(handle, out uint mode))
            {
                return;
            }
            savedWindowsModes.Add((handle, mode));
            uint nextMode = mode;
            nextMode |= WindowsEnableProcessedInput;
            nextMode |= WindowsEnableWindowInput;
            nextMode |= WindowsEnableMouseInput;
            nextMode |= WindowsEnableVirtualTerminalInput;
            nextMode &= ~WindowsEnableLineInput;
            nextMode &= ~WindowsEnableEchoInput;
            _ = SetConsoleMode(handle, nextMode);
        }

        private void ConfigureWindowsOutputMode(int standardHandle)
        {
            IntPtr handle = GetStdHandle(standardHandle);
            if (handle == IntPtr.Zero || handle == new IntPtr(-1) || !GetConsoleMode(handle, out uint mode))
            {
                return;
            }
            savedWindowsModes.Add((handle, mode));
            uint nextMode = mode | WindowsEnableVirtualTerminalProcessing | WindowsDisableNewlineAutoReturn;
            _ = SetConsoleMode(handle, nextMode);
        }

        private void RestoreWindowsConsoleMode()
        {
            for (int index = savedWindowsModes.Count - 1; index >= 0; --index)
            {
                (IntPtr handle, uint mode) = savedWindowsModes[index];
                _ = SetConsoleMode(handle, mode);
            }
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetStdHandle(int nStdHandle);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool GetConsoleMode(IntPtr hConsoleHandle, out uint lpMode);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool SetConsoleMode(IntPtr hConsoleHandle, uint dwMode);
    }

    public static int RunGeneratedWindow(GeneratedWindowBase window, GeneratedWindowRuntimeOptions? options = null, string[]? args = null)
    {
        options ??= window.RuntimeOptions();
        args ??= Environment.GetCommandLineArgs().Skip(1).ToArray();
        McpRuntimeConfig config = ParseMcpRuntimeArgs(args);
        if (config.Enabled)
        {
            McpController controller = new(window, options, config);
            if (config.Gui)
            {
                RenderToConsole(window, options, new Size(config.ViewportWidth, config.ViewportHeight));
            }
            if (config.Transport == "stdio")
            {
                controller.ServeStdio();
            }
            else
            {
                controller.ServeTcp();
            }
            return 0;
        }
        if (!Console.IsInputRedirected && !Console.IsOutputRedirected)
        {
            return RunInteractiveTerminal(window, options);
        }
        RenderToConsole(window, options, new Size(FallbackTerminalWidth, FallbackTerminalHeight));
        return 0;
    }

    public static int RunGeneratedWindow(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options, int argc, string[] argv)
    {
        return RunGeneratedWindow(window, options, argv.Take(argc).ToArray());
    }

    public static int RunGeneratedAppMain(Func<int> body)
    {
        try
        {
            return body();
        }
        catch (Exception exc)
        {
            Console.Error.WriteLine("error: " + exc.Message);
            return 1;
        }
    }

    public static List<List<TerminalCell>> RenderGeneratedWindowContent(
        GeneratedWindowBase window,
        Size size,
        int focusedIndex = -1,
        Element? focusedElement = null,
        bool editMode = false,
        ScrollView? activeScrollView = null,
        ReusableElement? activeScrollViewProxy = null,
        Element? activeScrollViewEditElement = null,
        bool activeScrollViewFresh = false,
        bool applyActiveScrollViewDim = true,
        Color? activeScrollViewFocusBackgroundOverride = null,
        bool suppressActiveScrollViewScopeVisuals = false,
        Style? windowStyleOverride = null,
        int? clipTop = null,
        int? clipBottom = null,
        bool forceFullscreenLayout = false,
        bool useHostViewportForRootScrollViewIndicators = false)
    {
        int width = Math.Max(MinimumRenderableSize, size.Width);
        int height = Math.Max(MinimumRenderableSize, size.Height);
        SyncWindowElementFramesTo(window, new Rect(0, 0, width, height), forceFullscreenLayout);
        ScrollView? effectiveActiveScrollView =
            activeScrollView is not null && ActiveScrollViewRepresentedInCurrentLayout(window, activeScrollView)
                ? activeScrollView
                : null;
        Element? effectiveActiveScrollViewEditElement =
            activeScrollViewEditElement is not null && ElementRepresentedInCurrentLayout(window, activeScrollViewEditElement)
                ? activeScrollViewEditElement
                : null;
        ReusableElement? effectiveActiveScrollViewProxy =
            effectiveActiveScrollView is not null &&
            activeScrollViewProxy is not null &&
            WindowContainsElement(window, activeScrollViewProxy)
                ? activeScrollViewProxy
                : null;
        if (effectiveActiveScrollView is not null && effectiveActiveScrollViewProxy is null)
        {
            effectiveActiveScrollViewProxy = ReusableContainingElement(window, effectiveActiveScrollView);
        }
        TerminalBuffer buffer = new(width, height);
        Style windowStyle = windowStyleOverride ?? window.GeneratedWindowStyle;
        FillRect(buffer, new Rect(0, 0, width, height), windowStyle);
        List<Element> focusable = FocusableElements(window, effectiveActiveScrollView);
        Element? focused = focusedElement is not null && focusable.Contains(focusedElement)
            ? focusedElement
            : focusedIndex >= 0 && focusedIndex < focusable.Count
            ? focusable[focusedIndex]
            : null;
        if (editMode && effectiveActiveScrollView is null)
        {
            effectiveActiveScrollView =
                ScrollViewContainingElement(window, focused) ??
                ScrollViewContainingElement(window, effectiveActiveScrollViewEditElement);
            if (effectiveActiveScrollView is not null && effectiveActiveScrollViewProxy is null)
            {
                effectiveActiveScrollViewProxy = ReusableContainingElement(window, effectiveActiveScrollView);
            }
        }
        List<ResolvedRuntimeCell> resolvedCells = ResolveRuntimeCells(
            window,
            new Size(width, height),
            forceFullscreenLayout ? GeneratedWindowMode.Fullscreen : WindowMode(window));

        List<ResolvedRuntimeCell> paintedCells = new();
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            ResolvedRuntimeCell? cell = ResolvedCellFor(resolvedCells, entry);
            if (cell is null)
            {
                continue;
            }
            if (!paintedCells.Any(paintedCell => SameRuntimeCell(paintedCell.Cell, entry)))
            {
                paintedCells.Add(cell);
                RenderEntryCellStyle(buffer, entry, cell.Rect);
            }
        }

        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            Element? element = FindElement(window, entry.Name);
            ResolvedRuntimeCell? cell = ResolvedCellFor(resolvedCells, entry);
            if (cell is null)
            {
                continue;
            }
            RenderEntry(
                window,
                buffer,
                entry,
                element,
                focused,
                editMode,
                cell.Rect,
                effectiveActiveScrollView,
                effectiveActiveScrollViewProxy,
                effectiveActiveScrollViewEditElement,
                activeScrollViewFresh,
                activeScrollViewFocusBackgroundOverride,
                suppressActiveScrollViewScopeVisuals,
                clipTop,
                clipBottom,
                useHostViewportForRootScrollViewIndicators);
        }
        if (applyActiveScrollViewDim)
        {
            DimOutsideActiveScrollViewScope(
                buffer,
                window,
                focused,
                editMode,
                effectiveActiveScrollView,
                effectiveActiveScrollViewProxy,
                activeScrollViewProxy ?? effectiveActiveScrollViewProxy,
                activeScrollViewFresh);
        }
        List<List<TerminalCell>> rendered = ContentFromBuffer(buffer);
        if (useHostViewportForRootScrollViewIndicators &&
            window is GeneratedScrollViewBase generatedScrollViewWindow)
        {
            ScrollView generatedScrollView = generatedScrollViewWindow.ScrollView();
            Rect scrollFrame = GeneratedScrollViewSourceFrame(window, generatedScrollView, new Size(width, height));
            Rect scrollViewport = ScrollViewViewportClipClampedToContent(
                scrollFrame,
                generatedScrollView.Style,
                new Size(width, height));
            if (scrollViewport.Width > 0 && scrollViewport.Height > 0)
            {
                bool childContentAboveViewport = false;
                bool childContentBelowViewport = false;
                foreach (ScrollViewChildView childView in generatedScrollView.ChildViews(new Size(scrollFrame.Width, scrollFrame.Height)))
                {
                    Rect childFrame = new(
                        scrollFrame.Row + childView.Frame.Row,
                        scrollFrame.Col + childView.Frame.Col,
                        childView.Frame.Width,
                        childView.Frame.Height);
                    if (childFrame.Row < scrollViewport.Row)
                    {
                        childContentAboveViewport = true;
                    }
                    if (childFrame.Row + childFrame.Height > scrollViewport.Row + scrollViewport.Height)
                    {
                        childContentBelowViewport = true;
                    }
                }

                Size viewportSize = new(scrollViewport.Width, scrollViewport.Height);
                int indicatorCol = scrollViewport.Col + scrollViewport.Width - MinimumRenderableSize;
                int maxViewOffset = generatedScrollView.MaxViewOffset(viewportSize);
                int viewOffset = generatedScrollView.ViewOffset;
                if (viewOffset < maxViewOffset || childContentAboveViewport)
                {
                    ApplyScrollIndicator(rendered, scrollViewport.Row, indicatorCol, "^", scrollViewport.Col);
                }
                if (viewOffset > 0 || childContentBelowViewport)
                {
                    ApplyScrollIndicator(
                        rendered,
                        scrollViewport.Row + scrollViewport.Height - MinimumRenderableSize,
                        indicatorCol,
                        "v",
                        scrollViewport.Col);
                }
            }
        }
        if (activeScrollViewFocusBackgroundOverride is not null &&
            window is not GeneratedScrollViewBase)
        {
            ApplyReusableFocusBackground(rendered, activeScrollViewFocusBackgroundOverride, window.GeneratedWindowStyle.Background);
        }
        return rendered;
    }

    public static Element? FindElement(GeneratedWindowBase window, string name)
    {
        if (string.IsNullOrEmpty(name))
        {
            return null;
        }
        int dotIndex = name.IndexOf('.', StringComparison.Ordinal);
        if (dotIndex > 0)
        {
            string head = name[..dotIndex];
            string tail = name[(dotIndex + 1)..];
            foreach (Element element in window.Elements)
            {
                if (element.Name == head && element is ReusableElement reusable && reusable.Child is not null)
                {
                    Element? nested = FindElement(reusable.Child, tail);
                    if (nested is not null)
                    {
                        return nested;
                    }
                }
            }
        }
        foreach (Element element in window.Elements)
        {
            Element? found = FindElementInTree(element, name);
            if (found is not null)
            {
                return found;
            }
        }
        return null;
    }

    private static Element? FindElementInTree(Element element, string name)
    {
        if (element.Name == name)
        {
            return element;
        }
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            string prefix = element.Name + ".";
            if (name.StartsWith(prefix, StringComparison.Ordinal))
            {
                Element? nestedByPath = FindElement(reusable.Child, name[prefix.Length..]);
                if (nestedByPath is not null)
                {
                    return nestedByPath;
                }
            }
            Element? nested = FindElement(reusable.Child, name);
            if (nested is not null)
            {
                return nested;
            }
        }
        foreach (Element child in element.Children)
        {
            Element? nested = FindElementInTree(child, name);
            if (nested is not null)
            {
                return nested;
            }
        }
        return null;
    }

    public static List<Element> FocusableElements(GeneratedWindowBase window)
    {
        return FocusableElements(window, null);
    }

    internal static List<Element> FocusableElements(GeneratedWindowBase window, ScrollView? activeScrollView)
    {
        List<Element> result = new();
        foreach (Element element in window.Elements)
        {
            AddFocusableElements(element, result, activeScrollView);
        }
        return result;
    }

    private static bool HasMultipleFocusableElements(GeneratedWindowBase window, ScrollView? activeScrollView)
    {
        return FocusableElements(window, activeScrollView).Count > 1;
    }

    private static void AddFocusableElements(Element element, List<Element> result, ScrollView? activeScrollView)
    {
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            SyncReusableChildFrames(reusable, reusable.Frame);
            if (reusable.Child is GeneratedScrollViewBase generatedScrollView)
            {
                if (IsFocusable(reusable))
                {
                    result.Add(reusable);
                }
                if (ReferenceEquals(activeScrollView, generatedScrollView.ScrollView()))
                {
                    foreach (Element childElement in FocusableElements(reusable.Child, activeScrollView))
                    {
                        if (!ReferenceEquals(childElement, generatedScrollView.ScrollView()))
                        {
                            result.Add(childElement);
                        }
                    }
                }
                return;
            }

            List<Element> childFocusable = FocusableElements(reusable.Child, activeScrollView);
            if (childFocusable.Count == 0 &&
                IsFocusable(reusable) &&
                reusable.Child.GeneratedFocusable &&
                reusable is not ViewHost)
            {
                result.Add(reusable);
            }
            else
            {
                result.AddRange(childFocusable);
            }
            return;
        }

        if (element is ScrollView scrollView)
        {
            if (IsFocusable(element))
            {
                result.Add(element);
            }
            if (ReferenceEquals(activeScrollView, scrollView))
            {
                Rect frame = scrollView.Frame;
                foreach (ScrollViewChildView childView in scrollView.ChildViews(new Size(frame.Width, frame.Height)))
                {
                    if (childView.Element is null)
                    {
                        continue;
                    }
                    childView.Element.Frame = new Rect(
                        frame.Row + childView.Frame.Row,
                        frame.Col + childView.Frame.Col,
                        childView.Frame.Width,
                        childView.Frame.Height);
                    if (childView.Element is ReusableElement childReusable && childReusable.Child is not null)
                    {
                        SyncReusableChildFrames(childReusable, childView.Element.Frame);
                        result.AddRange(FocusableElements(childReusable.Child, activeScrollView));
                        continue;
                    }
                    if (IsFocusable(childView.Element))
                    {
                        result.Add(childView.Element);
                    }
                }
            }
            return;
        }

        if (IsFocusable(element))
        {
            result.Add(element);
        }
        foreach (Element child in element.Children)
        {
            AddFocusableElements(child, result, activeScrollView);
        }
    }

    public static bool IsFocusable(Element element)
    {
        return element.Enabled && element.Focusable &&
            (element is Button || element is CheckBox || element is TextInput ||
             element is NumberInput || element is ComboBox || element is ListBox ||
             (element is ScrollView && element.Name != "__scrollview") ||
             element is ReusableElement);
    }

    public static Size GeneratedWindowContentSize(GeneratedWindowBase window)
    {
        Style style = window.GeneratedWindowStyle;
        List<RuntimeCell> runtimeCells = CollectRuntimeCells(window);
        int horizontalSeparator = BorderWidthHorizontal(style);
        int verticalSeparator = BorderWidthVertical(style);
        Size natural = LayoutSize(BuildRuntimeTree(runtimeCells, horizontalSeparator, verticalSeparator));
        List<ResolvedRuntimeCell> resolvedCells = new();
        ResolveRuntimeCellsWithFitPass(
            window,
            runtimeCells,
            new Rect(0, 0, natural.Width, natural.Height),
            horizontalSeparator,
            verticalSeparator,
            WindowMode(window),
            resolvedCells);
        return ResolvedContentExtent(resolvedCells, natural);
    }

    public static Size GeneratedWindowContentSizeForWidth(GeneratedWindowBase window, int width)
    {
        Style style = window.GeneratedWindowStyle;
        List<RuntimeCell> runtimeCells = CollectRuntimeCells(window);
        int horizontalSeparator = BorderWidthHorizontal(style);
        int verticalSeparator = BorderWidthVertical(style);
        Size natural = LayoutSize(BuildRuntimeTree(runtimeCells, horizontalSeparator, verticalSeparator));
        int resolvedWidth = Math.Max(MinimumRenderableSize, width);
        List<ResolvedRuntimeCell> resolvedCells = new();
        ResolveRuntimeCellsWithFitPass(
            window,
            runtimeCells,
            new Rect(0, 0, resolvedWidth, natural.Height),
            horizontalSeparator,
            verticalSeparator,
            WindowMode(window),
            resolvedCells);
        Size result = ResolvedContentExtent(resolvedCells, new Size(resolvedWidth, natural.Height));
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            ResolvedRuntimeCell? cell = ResolvedCellFor(resolvedCells, entry);
            if (cell is null)
            {
                continue;
            }
            Element? element = string.IsNullOrEmpty(entry.Name) ? null : FindElement(window, entry.Name);
            Size entrySize = RenderSizeFor(entry, cell.Rect, element);
            result = new Size(
                Math.Max(result.Width, cell.Rect.Col + PaddingLeft(entry.CellStyle) + entry.Relative.Col + entrySize.Width),
                Math.Max(result.Height, cell.Rect.Row + PaddingTop(entry.CellStyle) + entry.Relative.Row + entrySize.Height));
        }
        return result;
    }

    public static Size GeneratedWindowResolvedContentSize(GeneratedWindowBase window, Size size)
    {
        int width = Math.Max(MinimumRenderableSize, size.Width);
        int height = Math.Max(MinimumRenderableSize, size.Height);
        Style style = window.GeneratedWindowStyle;
        List<RuntimeCell> runtimeCells = CollectRuntimeCells(window);
        List<ResolvedRuntimeCell> resolvedCells = new();
        ResolveRuntimeCellsWithFitPass(
            window,
            runtimeCells,
            new Rect(0, 0, width, height),
            BorderWidthHorizontal(style),
            BorderWidthVertical(style),
            WindowMode(window),
            resolvedCells);
        return ResolvedContentExtent(resolvedCells, new Size(width, height));
    }

    public static Size ActiveWindowReportedSize(GeneratedWindowBase window, Size viewportSize)
    {
        Size size = GeneratedWindowContentSize(window);
        Style style = window.GeneratedWindowStyle;
        int horizontalBorder = Math.Max(0, style.BorderWidthHorizontal ?? 0);
        int verticalBorder = Math.Max(0, style.BorderWidthVertical ?? 0);
        size = new Size(size.Width + verticalBorder * 2, size.Height + horizontalBorder * 2);
        GeneratedWindowMode mode = WindowMode(window);
        int leftMargin = Math.Max(0, style.MarginLeft ?? style.Margin ?? 0);
        int rightMargin = Math.Max(0, style.MarginRight ?? style.Margin ?? 0);
        int topMargin = Math.Max(0, style.MarginTop ?? style.Margin ?? 0);
        int bottomMargin = Math.Max(0, style.MarginBottom ?? style.Margin ?? 0);
        int availableWidth = Math.Max(MinimumRenderableSize, viewportSize.Width - leftMargin - rightMargin);
        int availableHeight = Math.Max(MinimumRenderableSize, viewportSize.Height - topMargin - bottomMargin);
        size = new Size(
            mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandWidth ? availableWidth : Math.Min(size.Width, availableWidth),
            mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandHeight ? availableHeight : Math.Min(size.Height, availableHeight));
        return size;
    }

    public static List<List<TerminalCell>> RenderViewportContent(
        GeneratedWindowBase window,
        Size viewportSize,
        int focusedIndex = -1,
        Element? focusedElement = null,
        bool editMode = false,
        ScrollView? activeScrollView = null,
        ReusableElement? activeScrollViewProxy = null,
        Element? activeScrollViewEditElement = null,
        bool activeScrollViewFresh = false,
        bool applyActiveScrollViewDim = true,
        bool suppressActiveScrollViewScopeVisuals = false)
    {
        viewportSize = new Size(
            Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, viewportSize.Width),
            Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, viewportSize.Height));
        Size contentSize = ActiveWindowReportedSize(window, viewportSize);
        TerminalBuffer buffer = new(viewportSize.Width, viewportSize.Height);
        GeneratedWindowMode mode = WindowMode(window);
        int row = mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandHeight
            ? 0
            : Math.Max(0, (viewportSize.Height - contentSize.Height) / 2);
        int col = mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandWidth
            ? 0
            : Math.Max(0, (viewportSize.Width - contentSize.Width) / 2);
        Blit(
            buffer,
            RenderWindowFrameContent(
                window,
                contentSize,
                focusedIndex,
                focusedElement,
                editMode,
                activeScrollView,
                activeScrollViewProxy,
                activeScrollViewEditElement,
                activeScrollViewFresh,
                applyActiveScrollViewDim,
                suppressActiveScrollViewScopeVisuals),
            row,
            col);
        OffsetWindowElementFrames(
            window,
            row + BorderWidthHorizontal(window.GeneratedWindowStyle),
            col + BorderWidthVertical(window.GeneratedWindowStyle));
        OverlayFocusedComboBox(buffer, window, focusedIndex, focusedElement, editMode, activeScrollView, activeScrollViewEditElement);
        return ContentFromBuffer(buffer);
    }

    public static List<List<TerminalCell>> RenderTerminalContent(
        GeneratedWindowBase window,
        Size terminalSize,
        int focusedIndex = -1,
        Element? focusedElement = null,
        bool editMode = false,
        ScrollView? activeScrollView = null,
        ReusableElement? activeScrollViewProxy = null,
        Element? activeScrollViewEditElement = null,
        bool activeScrollViewFresh = false,
        bool applyActiveScrollViewDim = true,
        bool suppressActiveScrollViewScopeVisuals = false)
    {
        terminalSize = new Size(
            Math.Max(MinimumRenderableSize, terminalSize.Width),
            Math.Max(MinimumRenderableSize, terminalSize.Height));
        TerminalBuffer buffer = new(terminalSize.Width, terminalSize.Height);
        Rect bounds = WindowBounds(window, terminalSize);
        Blit(
            buffer,
            RenderWindowFrameContent(
                window,
                new Size(bounds.Width, bounds.Height),
                focusedIndex,
                focusedElement,
                editMode,
                activeScrollView,
                activeScrollViewProxy,
                activeScrollViewEditElement,
                activeScrollViewFresh,
                applyActiveScrollViewDim,
                suppressActiveScrollViewScopeVisuals),
            bounds.Row,
            bounds.Col);
        OffsetWindowElementFrames(
            window,
            bounds.Row + BorderWidthHorizontal(window.GeneratedWindowStyle),
            bounds.Col + BorderWidthVertical(window.GeneratedWindowStyle));
        OverlayFocusedComboBox(buffer, window, focusedIndex, focusedElement, editMode, activeScrollView, activeScrollViewEditElement);
        return ContentFromBuffer(buffer);
    }

    private static McpRuntimeConfig ParseMcpRuntimeArgs(string[] args)
    {
        McpRuntimeConfig config = new();
        config.Enabled = args.Contains("--mcp-server");
        config.Gui = !args.Contains("--headless");
        config.Transport = OptionValue(args, "--mcp-transport", config.Gui ? "tcp" : "stdio");
        config.Host = OptionValue(args, "--mcp-host", config.Host);
        config.Port = int.Parse(OptionValue(args, "--mcp-port", config.Port.ToString()));
        config.ViewportRow = Math.Max(0, int.Parse(OptionValue(args, "--viewport-row", McpRuntimeConfig.DefaultViewportRow.ToString())));
        config.ViewportCol = Math.Max(0, int.Parse(OptionValue(args, "--viewport-col", McpRuntimeConfig.DefaultViewportCol.ToString())));
        config.ViewportWidth = Math.Max(1, int.Parse(OptionValue(args, "--viewport-width", McpRuntimeConfig.DefaultViewportWidth.ToString())));
        config.ViewportHeight = Math.Max(1, int.Parse(OptionValue(args, "--viewport-height", McpRuntimeConfig.DefaultViewportHeight.ToString())));
        string? viewport = OptionValueOrNull(args, "--viewport");
        if (!string.IsNullOrEmpty(viewport))
        {
            string[] parts = viewport.Split(',');
            if (parts.Length == 4)
            {
                config.ViewportRow = Math.Max(0, int.Parse(parts[0]));
                config.ViewportCol = Math.Max(0, int.Parse(parts[1]));
                config.ViewportWidth = Math.Max(1, int.Parse(parts[2]));
                config.ViewportHeight = Math.Max(1, int.Parse(parts[3]));
            }
            else
            {
                throw new InvalidOperationException("--viewport must have form row,col,width,height");
            }
        }
        return config;
    }

    private static string OptionValue(string[] args, string name, string fallback)
    {
        return OptionValueOrNull(args, name) ?? fallback;
    }

    private static string? OptionValueOrNull(string[] args, string name)
    {
        for (int index = 0; index + 1 < args.Length; ++index)
        {
            if (args[index] == name)
            {
                return args[index + 1];
            }
        }
        return null;
    }

    private static GeneratedWindowMode WindowMode(GeneratedWindowBase window)
    {
        string explicitMode = (window.Mode ?? "").Trim().ToLowerInvariant();
        if (explicitMode == "fullscreen")
        {
            return GeneratedWindowMode.Fullscreen;
        }
        if (explicitMode == "expand_width")
        {
            return GeneratedWindowMode.ExpandWidth;
        }
        if (explicitMode == "expand_height")
        {
            return GeneratedWindowMode.ExpandHeight;
        }
        bool expandsWidth = false;
        bool expandsHeight = false;
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            expandsWidth = expandsWidth || entry.CellWidth.Mode == DimensionMode.Expanded;
            expandsHeight = expandsHeight || entry.CellHeight.Mode == DimensionMode.Expanded;
        }
        if (expandsWidth && expandsHeight)
        {
            return GeneratedWindowMode.Fullscreen;
        }
        if (expandsWidth)
        {
            return GeneratedWindowMode.ExpandWidth;
        }
        if (expandsHeight)
        {
            return GeneratedWindowMode.ExpandHeight;
        }
        return GeneratedWindowMode.Normal;
    }

    private static Rect WindowBounds(GeneratedWindowBase window, Size terminalSize)
    {
        Style style = window.GeneratedWindowStyle;
        Size content = GeneratedWindowContentSize(window);
        GeneratedWindowMode mode = WindowMode(window);
        int naturalWidth = content.Width + BorderWidthVertical(style) * 2;
        int naturalHeight = content.Height + BorderWidthHorizontal(style) * 2;
        int leftMargin = Math.Max(0, style.MarginLeft ?? style.Margin ?? 0);
        int rightMargin = Math.Max(0, style.MarginRight ?? style.Margin ?? 0);
        int topMargin = Math.Max(0, style.MarginTop ?? style.Margin ?? 0);
        int bottomMargin = Math.Max(0, style.MarginBottom ?? style.Margin ?? 0);
        int availableWidth = Math.Max(MinimumRenderableSize, terminalSize.Width - leftMargin - rightMargin);
        int availableHeight = Math.Max(MinimumRenderableSize, terminalSize.Height - topMargin - bottomMargin - ContentTopRow);
        int width = mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandWidth
            ? availableWidth
            : Math.Min(naturalWidth, availableWidth);
        int height = mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandHeight
            ? availableHeight
            : Math.Min(naturalHeight, availableHeight);
        int row = mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandHeight
            ? ContentTopRow + topMargin
            : ContentTopRow + topMargin + Math.Max(0, (availableHeight - height) / 2);
        int col = mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandWidth
            ? ContentLeftCol + leftMargin
            : ContentLeftCol + leftMargin + Math.Max(0, (availableWidth - width) / 2);
        return new Rect(row, col, width, height);
    }

    private static void RenderToConsole(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options, Size size)
    {
        List<List<TerminalCell>> content = RenderWindowFrameContent(window, size);
        foreach (string line in RenderHelpers.RenderedAnsiText(content))
        {
            Console.WriteLine(line);
        }
    }

    private static bool StyleHasAnimatedTextGradient(Style style)
    {
        return style.TextColorGradient is not null || style.TextBackgroundGradient is not null;
    }

    private static bool ElementHasAnimatedTextGradient(Element element)
    {
        if (StyleHasAnimatedTextGradient(element.Style))
        {
            return true;
        }
        if (element.FocusStyle is not null && StyleHasAnimatedTextGradient(element.FocusStyle))
        {
            return true;
        }
        if (element.EditStyle is not null && StyleHasAnimatedTextGradient(element.EditStyle))
        {
            return true;
        }
        if (element.CursorStyle is not null && StyleHasAnimatedTextGradient(element.CursorStyle))
        {
            return true;
        }
        if (element.SelectedStyle is not null && StyleHasAnimatedTextGradient(element.SelectedStyle))
        {
            return true;
        }
        if (element.ActiveStyle is not null && StyleHasAnimatedTextGradient(element.ActiveStyle))
        {
            return true;
        }
        foreach (Element child in element.Children)
        {
            if (ElementHasAnimatedTextGradient(child))
            {
                return true;
            }
        }
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            return WindowHasAnimatedTextGradient(reusable.Child);
        }
        return false;
    }

    private static bool WindowHasAnimatedTextGradient(GeneratedWindowBase window)
    {
        if (StyleHasAnimatedTextGradient(window.GeneratedWindowStyle))
        {
            return true;
        }
        foreach (Element element in window.Elements)
        {
            if (ElementHasAnimatedTextGradient(element))
            {
                return true;
            }
        }
        return false;
    }

    private static int RunInteractiveTerminal(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options)
    {
        McpRuntimeConfig config = new()
        {
            ViewportWidth = TerminalViewportWidth(),
            ViewportHeight = TerminalViewportHeight(),
        };
        McpController controller = new(window, options, config);
        bool previousTreatControlCAsInput = Console.TreatControlCAsInput;
        Console.TreatControlCAsInput = true;
        TerminalModeScope terminalMode = new();
        using Stream input = Console.OpenStandardInput();
        UpdateImageTerminalCellPixels(input, new Size(config.ViewportWidth, config.ViewportHeight));
        Console.Write(TerminalTitleSequence(window));
        Console.Write(AnsiEnterAlternateScreen);
        try
        {
            bool renderNeeded = true;
            Size previousSize = new(config.ViewportWidth, config.ViewportHeight);
            TerminalBuffer terminalBuffer = new(previousSize.Width, previousSize.Height);
            bool hasAnimatedTextGradient = WindowHasAnimatedTextGradient(window);
            DateTime lastAnimatedRenderAt = DateTime.UtcNow;
            string notification = "";
            DateTime notificationExpiresAt = DateTime.MinValue;
            bool sixelFallbackWarningRejected = false;
            if (WindowNeedsSixelFallbackWarning(window))
            {
                MessageBoxYesNo warningDialog = new("Warning", SixelFallbackWarningText);
                warningDialog.OnClose = confirmed =>
                {
                    if (!confirmed)
                    {
                        sixelFallbackWarningRejected = true;
                    }
                };
                controller.OpenModalWindow(warningDialog);
            }
            controller.BeforeStandardEscapeButtonAction = () =>
            {
                RenderControllerToConsole(
                    controller,
                    new Size(config.ViewportWidth, config.ViewportHeight),
                    terminalBuffer,
                    notification);
                Thread.Sleep(DialogButtonCloseDelayMilliseconds);
            };
            controller.BeforeModalButtonAction = controller.BeforeStandardEscapeButtonAction;
            while (true)
            {
                if (controller.ShouldCloseRequested() || sixelFallbackWarningRejected)
                {
                    break;
                }
                Size terminalSize = new(TerminalViewportWidth(), TerminalViewportHeight());
                if (terminalSize != previousSize)
                {
                    UpdateImageTerminalCellPixels(input, terminalSize);
                    renderNeeded = true;
                    previousSize = terminalSize;
                    terminalBuffer = new TerminalBuffer(terminalSize.Width, terminalSize.Height);
                }
                config.ViewportWidth = terminalSize.Width;
                config.ViewportHeight = terminalSize.Height;
                DateTime now = DateTime.UtcNow;
                if (hasAnimatedTextGradient &&
                    (now - lastAnimatedRenderAt).TotalMilliseconds >= AnimatedRenderIntervalMilliseconds)
                {
                    renderNeeded = true;
                    lastAnimatedRenderAt = now;
                }
                if (!string.IsNullOrEmpty(notification) && now >= notificationExpiresAt)
                {
                    notification = "";
                    renderNeeded = true;
                }
                if (renderNeeded)
                {
                    RenderControllerToConsole(controller, terminalSize, terminalBuffer, notification);
                    renderNeeded = false;
                }

                List<DirectTerminalEvent> inputEvents = ReadDirectTerminalEvents(input);
                if (inputEvents.Count == 0)
                {
                    Thread.Sleep(DirectInputIdleSleepMilliseconds);
                    continue;
                }

                bool shouldStop = false;
                foreach (DirectTerminalEvent inputEvent in inputEvents)
                {
                    if (inputEvent.Kind == DirectTerminalEventKind.Key && inputEvent.Key == "Ctrl+C")
                    {
                        shouldStop = true;
                        break;
                    }
                    if (inputEvent.Kind == DirectTerminalEventKind.Key && inputEvent.Key is not null)
                    {
                        controller.PressKey(inputEvent.Key);
                        renderNeeded = true;
                    }
                    else if (inputEvent.Kind == DirectTerminalEventKind.MousePress)
                    {
                        controller.MousePressAt(inputEvent.Position);
                        renderNeeded = true;
                    }
                    else if (inputEvent.Kind == DirectTerminalEventKind.MouseDrag)
                    {
                        controller.MouseDragAt(inputEvent.Position);
                        renderNeeded = true;
                    }
                    else if (inputEvent.Kind == DirectTerminalEventKind.MouseRelease)
                    {
                        controller.MouseReleaseAt(inputEvent.Position);
                        renderNeeded = true;
                    }
                    else if (inputEvent.Kind == DirectTerminalEventKind.MouseWheel)
                    {
                        controller.MouseWheelAt(inputEvent.Position, inputEvent.WheelDelta);
                        renderNeeded = true;
                    }
                    if (controller.TakeCopyNotificationRequested())
                    {
                        notification = CopyNotificationText;
                        notificationExpiresAt = DateTime.UtcNow.AddMilliseconds(CopyNotificationDurationMilliseconds);
                        renderNeeded = true;
                    }
                }
                if (shouldStop)
                {
                    break;
                }
            }
        }
        finally
        {
            Console.TreatControlCAsInput = previousTreatControlCAsInput;
            Console.Write(AnsiLeaveAlternateScreen);
            Console.Out.Flush();
            DrainTerminalInput(input);
            terminalMode.Dispose();
            Console.Write(AnsiLeaveApplicationKeypadMode);
            Console.Out.Flush();
        }
        return 0;
    }

    private static int TerminalViewportWidth()
    {
        try
        {
            return Math.Max(MinimumRenderableSize, Console.WindowWidth);
        }
        catch (IOException)
        {
            return FallbackTerminalWidth;
        }
    }

    private static int TerminalViewportHeight()
    {
        try
        {
            return Math.Max(MinimumRenderableSize, Console.WindowHeight);
        }
        catch (IOException)
        {
            return FallbackTerminalHeight;
        }
    }

    private static string TerminalTitleSequence(GeneratedWindowBase window)
    {
        return TerminalTitlePrefix + SanitizedTerminalTitle(window.Title + TerminalTitleSuffix) + TerminalTitleTerminator;
    }

    private static string SanitizedTerminalTitle(string title)
    {
        StringBuilder sanitized = new();
        foreach (char value in title)
        {
            if (!char.IsControl(value))
            {
                sanitized.Append(value);
            }
        }
        return sanitized.ToString();
    }

    private static void UpdateImageTerminalCellPixels(Stream input, Size terminalSize)
    {
        Size? cellPixels = TerminalCellPixelSize(input, terminalSize);
        if (cellPixels.HasValue)
        {
            Image.SetTerminalCellPixels(cellPixels.Value);
        }
    }

    private static Size? TerminalCellPixelSize(Stream input, Size terminalSize)
    {
        if (OperatingSystem.IsWindows())
        {
            return null;
        }
        Size? ioctlSize = TerminalCellPixelSizeFromIoctl();
        if (ioctlSize.HasValue)
        {
            return ioctlSize.Value;
        }
        Size? direct = QueryTerminalPixelReport(input, TerminalCellPixelQuery, TerminalCellPixelResponsePrefix);
        if (direct.HasValue)
        {
            return direct.Value;
        }
        if (terminalSize.Width <= 0 || terminalSize.Height <= 0)
        {
            return null;
        }
        Size? textArea = QueryTerminalPixelReport(input, TerminalTextAreaPixelQuery, TerminalTextAreaPixelResponsePrefix);
        if (!textArea.HasValue)
        {
            return null;
        }
        int width = Math.Max(1, (textArea.Value.Width + terminalSize.Width / 2) / terminalSize.Width);
        int height = Math.Max(1, (textArea.Value.Height + terminalSize.Height / 2) / terminalSize.Height);
        return new Size(width, height);
    }

    private static Size? TerminalCellPixelSizeFromIoctl()
    {
        if (OperatingSystem.IsWindows())
        {
            return null;
        }
        PosixWinSize size = new();
        bool isDarwin = OperatingSystem.IsMacOS() || OperatingSystem.IsMacCatalyst();
        ulong request = isDarwin ? DarwinTiocgwinsz : LinuxTiocgwinsz;
        int result = isDarwin
            ? DarwinIoctl(PosixStdoutFileDescriptor, request, ref size)
            : PosixIoctl(PosixStdoutFileDescriptor, request, ref size);
        if (result != 0 ||
            size.Col == 0 ||
            size.Row == 0 ||
            size.XPixel == 0 ||
            size.YPixel == 0)
        {
            return null;
        }
        int width = Math.Max(1, (size.XPixel + size.Col / 2) / size.Col);
        int height = Math.Max(1, (size.YPixel + size.Row / 2) / size.Row);
        return new Size(width, height);
    }

    private static Size? QueryTerminalPixelReport(Stream input, string query, string responsePrefix)
    {
        if (OperatingSystem.IsWindows())
        {
            return null;
        }
        try
        {
            Console.Write(query);
            Console.Out.Flush();
        }
        catch (IOException)
        {
            return null;
        }
        catch (ObjectDisposedException)
        {
            return null;
        }

        StringBuilder response = new(TerminalCellPixelResponseMaxBytes);
        while (response.Length < TerminalCellPixelResponseMaxBytes)
        {
            if (!PosixStdinInputAvailable(TerminalCellPixelQueryTimeoutMilliseconds))
            {
                break;
            }
            int maxRead = Math.Max(1, TerminalCellPixelResponseMaxBytes - response.Length);
            byte[] buffer = new byte[maxRead];
            nint count = PosixRead(PosixStdinFileDescriptor, buffer, (nuint)buffer.Length);
            if (count <= 0)
            {
                break;
            }
            response.Append(Encoding.UTF8.GetString(buffer, 0, (int)count));
            Size? parsed = ParseTerminalPixelResponse(response.ToString(), responsePrefix);
            if (parsed.HasValue)
            {
                return parsed.Value;
            }
        }
        return null;
    }

    private static Size? ParseTerminalPixelResponse(string response, string responsePrefix)
    {
        int prefix = response.IndexOf(responsePrefix, StringComparison.Ordinal);
        if (prefix < 0)
        {
            return null;
        }
        int heightStart = prefix + responsePrefix.Length;
        int separator = response.IndexOf(TerminalCellPixelResponseSeparator, heightStart);
        if (separator < 0)
        {
            return null;
        }
        int terminator = response.IndexOf(TerminalCellPixelResponseTerminator, separator + 1);
        if (terminator < 0)
        {
            return null;
        }
        bool parsedHeight = int.TryParse(
            response.Substring(heightStart, separator - heightStart),
            out int height);
        bool parsedWidth = int.TryParse(
            response.Substring(separator + 1, terminator - separator - 1),
            out int width);
        if (!parsedWidth || !parsedHeight || width <= 0 || height <= 0)
        {
            return null;
        }
        return new Size(width, height);
    }

    private static void RenderControllerToConsole(
        McpController controller,
        Size terminalSize,
        TerminalBuffer terminalBuffer,
        string notification = "")
    {
        terminalBuffer.Clear();
        List<List<TerminalCell>> frame = controller.RenderTerminalFrame(terminalSize);
        for (int row = 0; row < terminalBuffer.Height && row < frame.Count; ++row)
        {
            List<TerminalCell> cells = frame[row];
            for (int col = 0; col < terminalBuffer.Width && col < cells.Count; ++col)
            {
                terminalBuffer.SetCell(row, col, cells[col].Clone());
            }
        }
        if (!string.IsNullOrEmpty(notification))
        {
            RenderNotification(terminalBuffer, notification);
        }
        ScrollRegionHint? scrollRegion = controller.TakePendingScrollRegion();
        string scrollOutput = "";
        if (scrollRegion.HasValue &&
            scrollRegion.Value.Rect.Col == 0 &&
            scrollRegion.Value.Rect.Width >= terminalBuffer.Width)
        {
            scrollOutput = terminalBuffer.RenderScrollRegion(
                0,
                scrollRegion.Value.Rect.Row,
                scrollRegion.Value.Rect.Height,
                scrollRegion.Value.Delta);
        }
        Console.Write(scrollOutput + terminalBuffer.RenderDiff());
        Console.Out.Flush();
    }

    private static void RenderNotification(TerminalBuffer buffer, string message)
    {
        string text = $" {message} ";
        int col = Math.Max(0, buffer.Width - text.Length - CopyNotificationRightMargin);
        for (int index = 0; index < text.Length && col + index < buffer.Width; ++index)
        {
            buffer.SetCell(CopyNotificationRow, col + index, new TerminalCell
            {
                Text = text[index].ToString(),
                Foreground = CopyNotificationForeground,
                Background = CopyNotificationBackground,
            });
        }
    }

    private static bool WindowNeedsSixelFallbackWarning(GeneratedWindowBase window)
    {
        HashSet<object> visited = new();
        return WindowNeedsSixelFallbackWarning(window, visited);
    }

    private static bool WindowNeedsSixelFallbackWarning(GeneratedWindowBase? window, HashSet<object> visited)
    {
        if (window is null || !visited.Add(window))
        {
            return false;
        }
        foreach (Element element in window.Elements)
        {
            if (ElementNeedsSixelFallbackWarning(element, visited))
            {
                return true;
            }
        }
        return false;
    }

    private static bool ElementNeedsSixelFallbackWarning(Element? element, HashSet<object> visited)
    {
        if (element is null || !visited.Add(element))
        {
            return false;
        }
        if (element is Image image && image.ShouldWarnSixelFallback())
        {
            return true;
        }
        if (element is ReusableElement reusable &&
            WindowNeedsSixelFallbackWarning(reusable.Child, visited))
        {
            return true;
        }
        foreach (Element child in element.Children)
        {
            if (ElementNeedsSixelFallbackWarning(child, visited))
            {
                return true;
            }
        }
        return false;
    }

    private static List<DirectTerminalEvent> ReadDirectTerminalEvents(Stream input)
    {
        DirectTerminalEvent? first = ReadDirectTerminalEvent(input);
        if (first is null)
        {
            return new List<DirectTerminalEvent>();
        }

        List<DirectTerminalEvent> events = new() { first };
        while (true)
        {
            DirectTerminalEvent? next = ReadDirectTerminalEvent(input);
            if (next is null)
            {
                break;
            }
            events.Add(next);
        }
        return CoalesceMouseWheelEvents(events);
    }

    private static List<DirectTerminalEvent> CoalesceMouseWheelEvents(List<DirectTerminalEvent> events)
    {
        List<DirectTerminalEvent> coalesced = new(events.Count);
        for (int index = 0; index < events.Count;)
        {
            DirectTerminalEvent inputEvent = events[index];
            if (inputEvent.Kind != DirectTerminalEventKind.MouseWheel)
            {
                coalesced.Add(inputEvent);
                ++index;
                continue;
            }

            Point position = inputEvent.Position;
            int delta = 0;
            while (index < events.Count && events[index].Kind == DirectTerminalEventKind.MouseWheel)
            {
                position = events[index].Position;
                delta += events[index].WheelDelta;
                ++index;
            }
            delta = Math.Clamp(delta, -MaxCoalescedMouseWheelDelta, MaxCoalescedMouseWheelDelta);
            if (delta != 0)
            {
                coalesced.Add(DirectTerminalEvent.FromMouseWheel(position, delta));
            }
        }
        return coalesced;
    }

    private static DirectTerminalEvent? ReadDirectTerminalEvent(Stream input)
    {
        if (!OperatingSystem.IsWindows() && !PosixStdinInputAvailable(0))
        {
            return null;
        }
        if (OperatingSystem.IsWindows() && !ConsoleInputAvailable())
        {
            return null;
        }
        int value = ReadInputByte(input);
        if (value < 0)
        {
            return null;
        }
        if (value == TerminalEscape)
        {
            return DirectTerminalEventFromEscapeSequence(ReadPendingEscapeSequence(input));
        }

        string? runtimeKey = RuntimeKeyFromInputByte(value);
        return runtimeKey is null ? null : DirectTerminalEvent.FromKey(runtimeKey);
    }

    private static int ReadInputByte(Stream input)
    {
        if (!OperatingSystem.IsWindows())
        {
            return ReadPosixStdinByte();
        }
        try
        {
            return input.ReadByte();
        }
        catch (IOException)
        {
            return -1;
        }
        catch (ObjectDisposedException)
        {
            return -1;
        }
    }

    private static int ReadPendingInputByte(Stream input)
    {
        if (!OperatingSystem.IsWindows())
        {
            DateTime posixDeadline = DateTime.UtcNow.AddMilliseconds(DirectInputEscapeSequenceTimeoutMilliseconds);
            while (DateTime.UtcNow <= posixDeadline)
            {
                int timeout = Math.Max(
                    DirectInputEscapeSequencePollMilliseconds,
                    (int)Math.Ceiling((posixDeadline - DateTime.UtcNow).TotalMilliseconds));
                if (PosixStdinInputAvailable(timeout))
                {
                    return ReadPosixStdinByte();
                }
            }
            return -1;
        }
        DateTime deadline = DateTime.UtcNow.AddMilliseconds(DirectInputEscapeSequenceTimeoutMilliseconds);
        while (DateTime.UtcNow <= deadline)
        {
            if (ConsoleInputAvailable())
            {
                return ReadInputByte(input);
            }
            Thread.Sleep(DirectInputEscapeSequencePollMilliseconds);
        }
        return -1;
    }

    private static int ReadPosixStdinByte()
    {
        byte[] buffer = new byte[1];
        nint read = PosixRead(PosixStdinFileDescriptor, buffer, (nuint)buffer.Length);
        return read == 1 ? buffer[0] : -1;
    }

    private static void DrainTerminalInput(Stream input)
    {
        try
        {
            for (int reads = 0; reads < TerminalExitDrainMaxReads; ++reads)
            {
                if (!OperatingSystem.IsWindows())
                {
                    if (!PosixStdinInputAvailable(TerminalExitDrainTimeoutMilliseconds))
                    {
                        break;
                    }
                    byte[] buffer = new byte[TerminalExitDrainBytes];
                    if (PosixRead(PosixStdinFileDescriptor, buffer, (nuint)buffer.Length) <= 0)
                    {
                        break;
                    }
                    continue;
                }

                if (!ConsoleInputAvailable())
                {
                    break;
                }
                int drained = 0;
                while (drained < TerminalExitDrainBytes && ConsoleInputAvailable())
                {
                    if (input.ReadByte() < 0)
                    {
                        break;
                    }
                    ++drained;
                }
                if (drained == 0)
                {
                    break;
                }
            }
        }
        catch (IOException)
        {
        }
        catch (ObjectDisposedException)
        {
        }
        catch (InvalidOperationException)
        {
        }
    }

    private static bool PosixStdinInputAvailable(int timeoutMilliseconds)
    {
        PollFd[] fds = { new() { Fd = PosixStdinFileDescriptor, Events = PosixPollInput } };
        int ready = PosixPoll(fds, (uint)fds.Length, Math.Max(0, timeoutMilliseconds));
        return ready > 0 && (fds[0].Revents & PosixPollInput) != 0;
    }

    private static bool ConsoleInputAvailable()
    {
        try
        {
            return Console.KeyAvailable;
        }
        catch (InvalidOperationException)
        {
            return false;
        }
        catch (IOException)
        {
            return false;
        }
    }

    private static string ReadPendingEscapeSequence(Stream input)
    {
        StringBuilder sequence = new();
        sequence.Append(TerminalEscape);
        while (true)
        {
            int value = ReadPendingInputByte(input);
            if (value < 0)
            {
                return sequence.ToString();
            }
            sequence.Append((char)value);
            if (IsCompleteEscapeSequence(sequence.ToString()))
            {
                return sequence.ToString();
            }
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct PollFd
    {
        public int Fd;
        public short Events;
        public short Revents;
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct PosixWinSize
    {
        public ushort Row;
        public ushort Col;
        public ushort XPixel;
        public ushort YPixel;
    }

    [DllImport("libc", EntryPoint = "poll", SetLastError = true)]
    private static extern int PosixPoll([In, Out] PollFd[] fds, uint nfds, int timeout);

    [DllImport("libc", EntryPoint = "read", SetLastError = true)]
    private static extern nint PosixRead(int fd, byte[] buffer, nuint count);

    [DllImport("libc", EntryPoint = "ioctl", SetLastError = true)]
    private static extern int PosixIoctl(int fd, ulong request, ref PosixWinSize size);

    [DllImport("libc", EntryPoint = "__ioctl", SetLastError = true)]
    private static extern int DarwinIoctl(int fd, ulong request, ref PosixWinSize size);

    private static bool IsCompleteEscapeSequence(string sequence)
    {
        if (sequence.Length <= 1)
        {
            return false;
        }
        if (sequence.StartsWith(BracketedPasteStart, StringComparison.Ordinal))
        {
            return sequence.Contains(BracketedPasteEnd, StringComparison.Ordinal);
        }
        if (sequence.StartsWith(SgrMousePrefix, StringComparison.Ordinal))
        {
            return sequence.IndexOf('M', SgrMousePrefixLength) >= 0 ||
                sequence.IndexOf('m', SgrMousePrefixLength) >= 0;
        }
        if (sequence.StartsWith("\x1b[", StringComparison.Ordinal))
        {
            for (int index = CsiPrefixLength; index < sequence.Length; ++index)
            {
                if (IsCsiFinal(sequence[index]))
                {
                    return true;
                }
            }
            return false;
        }
        if (sequence.StartsWith("\x1bO", StringComparison.Ordinal))
        {
            return sequence.Length >= SgrMousePrefixLength;
        }
        return true;
    }

    private static bool IsCsiFinal(char value)
    {
        return value >= '@' && value <= '~';
    }

    private static DirectTerminalEvent? DirectTerminalEventFromEscapeSequence(string sequence)
    {
        if (sequence == TerminalEscape.ToString())
        {
            return DirectTerminalEvent.FromKey("Escape");
        }
        if (sequence.StartsWith(SgrMousePrefix, StringComparison.Ordinal))
        {
            return DirectTerminalEventFromSgrMouseSequence(sequence);
        }
        if (sequence.StartsWith(BracketedPasteStart, StringComparison.Ordinal))
        {
            return null;
        }

        string? runtimeKey = RuntimeKeyFromEscapeSequence(sequence);
        return DirectTerminalEvent.FromKey(runtimeKey ?? "Escape");
    }

    private static DirectTerminalEvent? DirectTerminalEventFromSgrMouseSequence(string sequence)
    {
        int finalIndex = sequence.IndexOf('M', SgrMousePrefixLength);
        int releaseIndex = sequence.IndexOf('m', SgrMousePrefixLength);
        if (finalIndex < 0 || (releaseIndex >= 0 && releaseIndex < finalIndex))
        {
            finalIndex = releaseIndex;
        }
        if (finalIndex < 0)
        {
            return null;
        }

        string payload = sequence.Substring(SgrMousePrefixLength, finalIndex - SgrMousePrefixLength);
        string[] fields = payload.Split(';');
        if (fields.Length != SgrMouseFieldCount ||
            !int.TryParse(fields[0], out int button) ||
            !int.TryParse(fields[1], out int col) ||
            !int.TryParse(fields[2], out int row))
        {
            return null;
        }

        Point position = new(row - TerminalCoordinateBase, col - TerminalCoordinateBase);
        if (button == SgrMouseWheelUpButton)
        {
            return DirectTerminalEvent.FromMouseWheel(position, SgrMouseWheelUpDelta);
        }
        if (button == SgrMouseWheelDownButton)
        {
            return DirectTerminalEvent.FromMouseWheel(position, SgrMouseWheelDownDelta);
        }
        if (sequence[finalIndex] == 'm')
        {
            return DirectTerminalEvent.FromMouse(DirectTerminalEventKind.MouseRelease, position);
        }
        if ((button & SgrMouseDragMask) != 0)
        {
            return DirectTerminalEvent.FromMouse(DirectTerminalEventKind.MouseDrag, position);
        }
        return DirectTerminalEvent.FromMouse(DirectTerminalEventKind.MousePress, position);
    }

    private static string? RuntimeKeyFromInputByte(int value)
    {
        char key = (char)value;
        if (key == TerminalCtrlC)
        {
            return "Ctrl+C";
        }
        if (key is TerminalCarriageReturn or TerminalLineFeed)
        {
            return "Enter";
        }
        if (key == TerminalTab)
        {
            return "Tab";
        }
        if (key is TerminalBackspace or TerminalDeleteBackspace)
        {
            return "Backspace";
        }
        if (value is >= 1 and <= 26)
        {
            char control = (char)('a' + value - 1);
            return "ctrl_" + control;
        }
        return key.ToString();
    }

    private static string? RuntimeKeyFromEscapeSequence(string sequence)
    {
        return sequence switch
        {
            "\x1b[A" => "Up",
            "\x1b[B" => "Down",
            "\x1b[C" => "Right",
            "\x1b[D" => "Left",
            "\x1bOA" => "Up",
            "\x1bOB" => "Down",
            "\x1bOC" => "Right",
            "\x1bOD" => "Left",
            "\x1b[H" => "Home",
            "\x1b[F" => "End",
            "\x1b[Z" => "Shift+Tab",
            "\x1bOH" => "Home",
            "\x1bOF" => "End",
            "\x1b[1~" => "Home",
            "\x1b[3~" => "Delete",
            "\x1b[4~" => "End",
            "\x1b[7~" => "Home",
            "\x1b[8~" => "End",
            "\x1b[1;2A" => "Shift+Up",
            "\x1b[1;2B" => "Shift+Down",
            "\x1b[1;2C" => "Shift+Right",
            "\x1b[1;2D" => "Shift+Left",
            "\x1b[1;2H" => "Shift+Home",
            "\x1b[1;2F" => "Shift+End",
            "\x1b[1;3A" => "Alt+Up",
            "\x1b[1;3B" => "Alt+Down",
            "\x1b[1;3C" => "Alt+Right",
            "\x1b[1;3D" => "Alt+Left",
            "\x1b[1;5A" => "Ctrl+Up",
            "\x1b[1;5B" => "Ctrl+Down",
            "\x1b[1;5C" => "Ctrl+Right",
            "\x1b[1;5D" => "Ctrl+Left",
            "\x1b\r" => "Alt+Enter",
            "\x1b\n" => "Alt+Enter",
            "\x1b\r\n" => "Alt+Enter",
            "\x1b[13;3u" => "Alt+Enter",
            "\x1b[13;5u" => "Alt+Enter",
            "\x1b[27;3;13~" => "Alt+Enter",
            "\x1b[27;5;13~" => "Alt+Enter",
            _ => RuntimeModifiedKeyFromEscapeSequence(sequence) ?? RuntimeCsiKey(sequence),
        };
    }

    private static string? RuntimeModifiedKeyFromEscapeSequence(string sequence)
    {
        if (sequence.StartsWith("\x1b[", StringComparison.Ordinal) && sequence.EndsWith("u", StringComparison.Ordinal))
        {
            string[] fields = sequence.Substring(CsiPrefixLength, sequence.Length - CsiPrefixLength - 1).Split(';');
            if (fields.Length == 2 &&
                int.TryParse(fields[0], out int codepoint) &&
                int.TryParse(fields[1], out int modifier))
            {
                return RuntimeModifiedKey(codepoint, modifier);
            }
        }
        if (sequence.StartsWith("\x1b[27;", StringComparison.Ordinal) && sequence.EndsWith("~", StringComparison.Ordinal))
        {
            string[] fields = sequence.Substring(CsiPrefixLength, sequence.Length - CsiPrefixLength - 1).Split(';');
            if (fields.Length == SgrMouseFieldCount &&
                int.TryParse(fields[1], out int modifier) &&
                int.TryParse(fields[2], out int codepoint))
            {
                return RuntimeModifiedKey(codepoint, modifier);
            }
        }
        return null;
    }

    private static string? RuntimeModifiedKey(int codepoint, int modifier)
    {
        if (IsControlCCodepoint(codepoint, modifier))
        {
            return "Ctrl+C";
        }
        if (IsCommandCCodepoint(codepoint, modifier))
        {
            return "cmd_c";
        }
        if (IsCommandVCodepoint(codepoint, modifier))
        {
            return "cmd_v";
        }
        return null;
    }

    private static bool IsControlCCodepoint(int codepoint, int modifier)
    {
        return codepoint == CtrlCCodepoint ||
            ((codepoint == LowercaseCCodepoint || codepoint == UppercaseCCodepoint) &&
                (modifier == CtrlModifier || modifier == CtrlShiftModifier));
    }

    private static bool IsCommandCCodepoint(int codepoint, int modifier)
    {
        return (codepoint == LowercaseCCodepoint || codepoint == UppercaseCCodepoint) &&
            (modifier == MetaModifier || modifier == MetaShiftModifier);
    }

    private static bool IsCommandVCodepoint(int codepoint, int modifier)
    {
        return (codepoint == LowercaseVCodepoint || codepoint == UppercaseVCodepoint) &&
            (modifier == MetaModifier || modifier == MetaShiftModifier);
    }

    private static string? RuntimeCsiKey(string sequence)
    {
        if (!sequence.StartsWith("\x1b[", StringComparison.Ordinal) || sequence.Length < SgrMousePrefixLength)
        {
            return null;
        }
        char final = sequence[^1];
        if (sequence.Length == SgrMousePrefixLength)
        {
            return KeyForCsiFinal(final);
        }

        string payload = sequence.Substring(CsiPrefixLength, sequence.Length - CsiPrefixLength - 1);
        int separator = payload.LastIndexOf(';');
        if (separator < 0 || !int.TryParse(payload[(separator + 1)..], out int modifier))
        {
            return null;
        }

        string? key = KeyForCsiFinal(final);
        if (key is null)
        {
            return null;
        }
        return modifier switch
        {
            2 => "Shift+" + key,
            3 => "Alt+" + key,
            4 => "Alt+Shift+" + key,
            5 => "Ctrl+" + key,
            6 => "Ctrl+Shift+" + key,
            _ => key,
        };
    }

    private static string? KeyForCsiFinal(char final)
    {
        return final switch
        {
            'A' => "Up",
            'B' => "Down",
            'C' => "Right",
            'D' => "Left",
            'H' => "Home",
            'F' => "End",
            'Z' => "Shift+Tab",
            _ => null,
        };
    }

    private static Color? DimColor(Color? color)
    {
        if (color is null || color.IsTransparent || !color.Rgba.HasValue)
        {
            return color;
        }
        Rgba rgba = color.Rgba.Value;
        return Color.Rgb(
            (byte)(rgba.Red * ModalBackgroundDimFactor),
            (byte)(rgba.Green * ModalBackgroundDimFactor),
            (byte)(rgba.Blue * ModalBackgroundDimFactor),
            rgba.Alpha);
    }

    internal static void DimBufferForModalOverlay(TerminalBuffer buffer)
    {
        for (int row = 0; row < buffer.Height; ++row)
        {
            for (int col = 0; col < buffer.Width; ++col)
            {
                TerminalCell cell = buffer.Cell(row, col).Clone();
                cell.Foreground = DimColor(cell.Foreground);
                cell.Background = DimColor(cell.Background);
                buffer.SetCell(row, col, cell);
            }
        }
    }

    private static Style BorderStyleFor(Style style)
    {
        return new Style
        {
            Color = style.BorderColor,
            Background = style.Background,
        };
    }

    private static Rect ContentBounds(Rect bounds, Style style)
    {
        int horizontal = BorderWidthHorizontal(style);
        int vertical = BorderWidthVertical(style);
        return new Rect(
            bounds.Row + horizontal,
            bounds.Col + vertical,
            Math.Max(MinimumRenderableSize, bounds.Width - vertical * 2),
            Math.Max(MinimumRenderableSize, bounds.Height - horizontal * 2));
    }

    internal static bool WindowStackFrameContains(GeneratedWindowBase window, Size terminalSize, Point terminalPoint)
    {
        return WindowBounds(window, terminalSize).Contains(terminalPoint);
    }

    internal static Point WindowStackContentPoint(
        GeneratedWindowBase window,
        Size terminalSize,
        Point terminalPoint)
    {
        Rect bounds = WindowBounds(window, terminalSize);
        Rect content = ContentBounds(
            new Rect(0, 0, bounds.Width, bounds.Height),
            window.GeneratedWindowStyle);
        return new Point(
            terminalPoint.Row - bounds.Row - content.Row,
            terminalPoint.Col - bounds.Col - content.Col);
    }

    internal static Size WindowStackContentSize(GeneratedWindowBase window, Size terminalSize)
    {
        Rect bounds = WindowBounds(window, terminalSize);
        Rect content = ContentBounds(
            new Rect(0, 0, bounds.Width, bounds.Height),
            window.GeneratedWindowStyle);
        return new Size(content.Width, content.Height);
    }

    private static List<List<TerminalCell>> RenderWindowFrameContent(
        GeneratedWindowBase window,
        Size frameSize,
        int focusedIndex = -1,
        Element? focusedElement = null,
        bool editMode = false,
        ScrollView? activeScrollView = null,
        ReusableElement? activeScrollViewProxy = null,
        Element? activeScrollViewEditElement = null,
        bool activeScrollViewFresh = false,
        bool applyActiveScrollViewDim = true,
        bool suppressActiveScrollViewScopeVisuals = false)
    {
        frameSize = new Size(
            Math.Max(MinimumRenderableSize, frameSize.Width),
            Math.Max(MinimumRenderableSize, frameSize.Height));
        TerminalBuffer frameBuffer = new(frameSize.Width, frameSize.Height);
        Style style = window.GeneratedWindowStyle;
        Rect localBounds = new(0, 0, frameSize.Width, frameSize.Height);
        FillRect(frameBuffer, localBounds, BorderStyleFor(style));
        Rect content = ContentBounds(localBounds, style);
        FillRect(frameBuffer, content, style);
        Blit(
            frameBuffer,
            RenderGeneratedWindowContent(
                window,
                new Size(content.Width, content.Height),
                focusedIndex,
                focusedElement,
                editMode,
                activeScrollView,
                activeScrollViewProxy,
                activeScrollViewEditElement,
                activeScrollViewFresh,
                applyActiveScrollViewDim,
                null,
                suppressActiveScrollViewScopeVisuals),
            content.Row,
            content.Col);
        return ContentFromBuffer(frameBuffer);
    }

    internal static void RenderWindowStackOverlay(
        TerminalBuffer buffer,
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        int focusedIndex = -1,
        Element? focusedElement = null,
        bool editMode = false,
        ScrollView? activeScrollView = null,
        ReusableElement? activeScrollViewProxy = null,
        Element? activeScrollViewEditElement = null,
        bool activeScrollViewFresh = false,
        bool topFrame = true,
        bool dimFrame = false)
    {
        Rect bounds = WindowBounds(window, new Size(buffer.Width, buffer.Height));
        Style style = window.GeneratedWindowStyle;
        TerminalBuffer frameBuffer = new(bounds.Width, bounds.Height);
        Rect localBounds = new(0, 0, bounds.Width, bounds.Height);
        FillRect(frameBuffer, localBounds, BorderStyleFor(style));
        Rect content = ContentBounds(localBounds, style);
        FillRect(frameBuffer, content, style);
        List<List<TerminalCell>> contentRows;
        if (topFrame)
        {
            contentRows = RenderGeneratedWindowContent(
                window,
                new Size(content.Width, content.Height),
                focusedIndex,
                focusedElement,
                editMode,
                activeScrollView,
                activeScrollViewProxy,
                activeScrollViewEditElement,
                activeScrollViewFresh);
        }
        else
        {
            using IDisposable imageCellBackgroundRendering = Image.ForceCellBackgroundRendering();
            contentRows = RenderGeneratedWindowContent(
                window,
                new Size(content.Width, content.Height),
                focusedIndex,
                focusedElement,
                false,
                null,
                null,
                null,
                false);
        }
        Blit(frameBuffer, contentRows, content.Row, content.Col);
        if (dimFrame)
        {
            DimBufferForModalOverlay(frameBuffer);
        }
        Blit(buffer, ContentFromBuffer(frameBuffer), bounds.Row, bounds.Col);
        if (topFrame)
        {
            OverlayFocusedComboBox(
                buffer,
                window,
                focusedIndex,
                focusedElement,
                editMode,
                activeScrollView,
                activeScrollViewEditElement,
                bounds.Row,
                bounds.Col);
        }
    }

    private static void FillRect(TerminalBuffer buffer, Rect rect, Style style)
    {
        for (int row = Math.Max(0, rect.Row); row < Math.Min(buffer.Height, rect.Row + rect.Height); ++row)
        {
            for (int col = Math.Max(0, rect.Col); col < Math.Min(buffer.Width, rect.Col + rect.Width); ++col)
            {
                TerminalCell cell = new()
                {
                    Text = " ",
                    Foreground = style.Color,
                    Background = style.Background,
                };
                TerminalCell existing = buffer.Cell(row, col);
                cell.Background = BlendBackgroundOverExisting(cell.Background, existing.Background);
                cell.Foreground = BlendForegroundOverBackground(cell.Foreground, existing.Foreground, cell.Background);
                buffer.SetCell(row, col, cell);
            }
        }
    }

    private static void RenderEntryCellStyle(TerminalBuffer buffer, GeneratedLayoutEntry entry, Rect cellRect)
    {
        if (entry.CellStyle.Background is null && entry.CellStyle.Color is null)
        {
            return;
        }
        FillRect(buffer, cellRect, entry.CellStyle);
    }

    private static void RenderEntry(
        GeneratedWindowBase window,
        TerminalBuffer buffer,
        GeneratedLayoutEntry entry,
        Element? element,
        Element? focusedElement,
        bool editMode,
        Rect cellRect,
        ScrollView? activeScrollView = null,
        ReusableElement? activeScrollViewProxy = null,
        Element? activeScrollViewEditElement = null,
        bool activeScrollViewFresh = false,
        Color? activeScrollViewFocusBackgroundOverride = null,
        bool suppressActiveScrollViewScopeVisuals = false,
        int? renderClipTop = null,
        int? renderClipBottom = null,
        bool useHostViewportForRootScrollViewIndicators = false)
    {
        if (element is null)
        {
            if (!string.IsNullOrEmpty(entry.Name))
            {
                string text = entry.Name;
                if (text.Length >= 2 && text[0] == '"' && text[^1] == '"')
                {
                    text = text[1..^1];
                }
                Size staticSize = RenderSizeFor(entry, cellRect, null);
                int staticRow = RenderRowFor(window.GeneratedLayout, entry, cellRect, staticSize);
                if (staticRow >= buffer.Height)
                {
                    return;
                }
                int staticCol = RenderColFor(window.GeneratedLayout, entry, cellRect, staticSize);
                if (staticCol >= buffer.Width)
                {
                    return;
                }
                Blit(
                    buffer,
                    RenderHelpers.RenderPlainText(text, staticSize.Width, staticSize.Height, entry.ElementStyle),
                    staticRow,
                    staticCol,
                    EntryContentClip(cellRect, entry.CellStyle));
            }
            return;
        }
        Size size = RenderSizeFor(entry, cellRect, element);
        int row = RenderRowFor(window.GeneratedLayout, entry, cellRect, size);
        int col = RenderColFor(window.GeneratedLayout, entry, cellRect, size);
        Rect clip = EntryContentClip(cellRect, entry.CellStyle);
        int? elementClipTop = null;
        int? elementClipBottom = null;
        {
            int baseClipTop = Math.Max(0, clip.Row);
            int baseClipBottom = Math.Max(baseClipTop, Math.Min(buffer.Height, clip.Row + clip.Height));
            int absoluteClipTop = Math.Max(baseClipTop, renderClipTop ?? baseClipTop);
            int absoluteClipBottom = Math.Max(
                absoluteClipTop,
                Math.Min(baseClipBottom, renderClipBottom ?? baseClipBottom));
            int visibleTop = Math.Max(row, absoluteClipTop);
            int visibleBottom = Math.Min(row + size.Height, absoluteClipBottom);
            elementClipTop = Math.Max(0, visibleTop - row);
            elementClipBottom = Math.Max(0, visibleBottom - row);
            if (elementClipTop == 0 && elementClipBottom == size.Height)
            {
                elementClipTop = null;
                elementClipBottom = null;
            }
        }
        element.Frame = new Rect(row, col, size.Width, size.Height);
        Color? parentBackground =
            row >= 0 && row < buffer.Height && col >= 0 && col < buffer.Width
                ? buffer.Cell(row, col).Background
                : null;
        List<List<TerminalCell>> rendered;
        using (new ScopedElementParentBackground(parentBackground))
        {
            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                bool childOwnsActiveScrollView =
                    activeScrollView is not null && WindowContainsElement(reusable.Child, activeScrollView);
                bool childOwnsActiveScrollViewProxy =
                    activeScrollViewProxy is not null && WindowContainsElement(reusable.Child, activeScrollViewProxy);
                bool childOwnsActiveScrollViewEditElement =
                    activeScrollViewEditElement is not null && WindowContainsElement(reusable.Child, activeScrollViewEditElement);
                List<Element> childFocusable = FocusableElements(
                    reusable.Child,
                    childOwnsActiveScrollView ? activeScrollView : null);
                int childFocusedIndex = focusedElement is not null
                    ? childFocusable.IndexOf(focusedElement)
                    : -1;
                if (childFocusedIndex < 0 && childOwnsActiveScrollViewEditElement)
                {
                    childFocusedIndex = childFocusable.IndexOf(activeScrollViewEditElement!);
                }
                if (childFocusedIndex < 0 &&
                    ReferenceEquals(element, focusedElement) &&
                    reusable.Child is GeneratedScrollViewBase generatedScrollView)
                {
                    childFocusedIndex = childFocusable.IndexOf(generatedScrollView.ScrollView());
                }
                if (!suppressActiveScrollViewScopeVisuals && childFocusedIndex < 0 && childOwnsActiveScrollView)
                {
                    childFocusedIndex = childFocusable.IndexOf(activeScrollView!);
                }

                bool childHasFocusedElement =
                    focusedElement is not null && WindowContainsElement(reusable.Child, focusedElement);
                bool navigationScopeDescendantFocused =
                    !suppressActiveScrollViewScopeVisuals &&
                    !editMode &&
                    childOwnsActiveScrollView &&
                    childHasFocusedElement &&
                    ReferenceEquals(ScrollViewContainingElement(window, focusedElement!), activeScrollView);
                bool reusableDescendantFocused =
                    ReferenceEquals(element, focusedElement) ||
                    navigationScopeDescendantFocused ||
                    (editMode && childOwnsActiveScrollView) ||
                    (editMode && childOwnsActiveScrollViewEditElement) ||
                    (editMode && childHasFocusedElement);
                bool directFocus = ReferenceEquals(element, focusedElement);
                ScrollView? reusableGeneratedScrollView =
                    reusable.Child is GeneratedScrollViewBase reusableScrollViewWindow
                        ? reusableScrollViewWindow.ScrollView()
                        : null;
                Size childRenderSize = size;
                Size childContentSizeForWidth = GeneratedWindowContentSizeForWidth(
                    reusable.Child,
                    Math.Max(MinimumRenderableSize, childRenderSize.Width));
                if (reusableGeneratedScrollView is null)
                {
                    childRenderSize = new Size(
                        childRenderSize.Width,
                        Math.Max(childRenderSize.Height, childContentSizeForWidth.Height));
                }
                Style? childWindowStyle = null;
                Color? childActiveScrollViewFocusBackground = null;
                bool applyChildDescendantFocusBackground = true;
                if (reusableDescendantFocused)
                {
                    childWindowStyle = reusable.Child.GeneratedWindowStyle.Clone();
                    bool applyReusableFocusStyle = ReusableFocusStyleAppliesToChild(
                        reusable,
                        directFocus,
                        reusableDescendantFocused && !directFocus);
                    if (applyReusableFocusStyle)
                    {
                        if (reusableGeneratedScrollView is null)
                        {
                            childWindowStyle.Merge(reusable.FocusStyle!);
                        }
                        if (reusable.FocusStyle?.Background is not null)
                        {
                            childActiveScrollViewFocusBackground = reusable.FocusStyle.Background;
                        }
                    }
                    if (childActiveScrollViewFocusBackground is null &&
                        !childOwnsActiveScrollView &&
                        reusableGeneratedScrollView is not null)
                    {
                        if (reusableGeneratedScrollView.DescendantFocusStyle?.Background is not null)
                        {
                            childActiveScrollViewFocusBackground =
                                reusableGeneratedScrollView.DescendantFocusStyle.Background;
                        }
                        else if (reusableGeneratedScrollView.FocusStyle?.Background is not null)
                        {
                            childActiveScrollViewFocusBackground =
                                reusableGeneratedScrollView.FocusStyle.Background;
                        }
                        applyChildDescendantFocusBackground =
                            childOwnsActiveScrollView ||
                            childOwnsActiveScrollViewEditElement ||
                            (focusedElement is not null &&
                                WindowContainsElement(reusable.Child, focusedElement) &&
                                !ReferenceEquals(focusedElement, reusableGeneratedScrollView));
                    }
                }

                if (!suppressActiveScrollViewScopeVisuals &&
                    ReferenceEquals(element, focusedElement) &&
                    reusable.Child is GeneratedScrollViewBase focusedGeneratedScrollView)
                {
                    childActiveScrollViewFocusBackground = GeneratedScrollViewProxyFocusBackground(
                        focusedGeneratedScrollView.ScrollView(),
                        reusable);
                }

                bool childEditMode =
                    editMode &&
                    childFocusedIndex >= 0 &&
                    (activeScrollView is null ||
                        childOwnsActiveScrollView ||
                        childOwnsActiveScrollViewEditElement);
                bool useHostViewportForRootScrollViewChild =
                    element is ViewHost &&
                    reusableGeneratedScrollView is not null;
                rendered = RenderGeneratedWindowContent(
                    reusable.Child,
                    childRenderSize,
                    childFocusedIndex,
                    focusedElement,
                    childEditMode,
                    activeScrollView,
                    activeScrollViewProxy,
                    activeScrollViewEditElement,
                    activeScrollViewFresh,
                    false,
                    childActiveScrollViewFocusBackground,
                    suppressActiveScrollViewScopeVisuals,
                    childWindowStyle,
                    elementClipTop,
                    elementClipBottom,
                    true,
                    useHostViewportForRootScrollViewChild);
                if (childActiveScrollViewFocusBackground is not null &&
                    reusableGeneratedScrollView is null)
                {
                    if (applyChildDescendantFocusBackground)
                    {
                        List<Color> descendantBackgrounds = new();
                        CollectDescendantBaseStyleBackgrounds(reusable.Child, descendantBackgrounds);
                        ApplyReusableFocusBackgroundToDescendantBackgrounds(
                            rendered,
                            childActiveScrollViewFocusBackground,
                            descendantBackgrounds);
                    }
                    ApplyReusableFocusBackground(
                        rendered,
                        childActiveScrollViewFocusBackground,
                        ReusableFocusBaseBackground(reusable, entry));
                }
                OffsetWindowElementFrames(reusable.Child, row, col);
            }
            else
            {
                bool focused = ReferenceEquals(element, focusedElement);
                bool activeScrollViewFocused =
                    !suppressActiveScrollViewScopeVisuals &&
                    (ReferenceEquals(activeScrollView, element) ||
                        (element is ScrollView activeFocusedScrollView &&
                            (ReferenceEquals(ScrollViewContainingElement(window, focusedElement), activeFocusedScrollView) ||
                                ReferenceEquals(ScrollViewContainingElement(window, activeScrollViewEditElement), activeFocusedScrollView))));
                bool scrollViewSelfFocused =
                    element is ScrollView &&
                    focused &&
                    !activeScrollViewFocused;
                bool elementEditMode =
                    focused &&
                    editMode &&
                    (activeScrollView is null || ReferenceEquals(activeScrollViewEditElement, element));
                rendered = element.Render(size, new ElementRenderState
                    {
                        Focused = focused && !scrollViewSelfFocused && !activeScrollViewFocused,
                        EditMode = elementEditMode && !activeScrollViewFocused,
                        PassiveFocus =
                            !scrollViewSelfFocused &&
                            !activeScrollViewFocused &&
                            activeScrollView is not null &&
                            focused &&
                            !elementEditMode,
                        FocusedElement = focusedElement,
                        SuppressActiveScrollViewScopeVisuals = suppressActiveScrollViewScopeVisuals,
                        ClipTop = elementClipTop,
                        ClipBottom = elementClipBottom,
                    });
            }
        }
        Blit(buffer, rendered, row, col, clip);
        if (element is ReusableElement renderedReusable &&
            renderedReusable.Child is not null &&
            !suppressActiveScrollViewScopeVisuals)
        {
            bool childOwnsActiveScrollView =
                activeScrollView is not null && WindowContainsElement(renderedReusable.Child, activeScrollView);
            bool childOwnsActiveScrollViewEditElement =
                activeScrollViewEditElement is not null && WindowContainsElement(renderedReusable.Child, activeScrollViewEditElement);
            bool directFocus = ReferenceEquals(element, focusedElement);
            bool navigationScopeDescendantFocused =
                !suppressActiveScrollViewScopeVisuals &&
                !editMode &&
                childOwnsActiveScrollView &&
                focusedElement is not null &&
                WindowContainsElement(renderedReusable.Child, focusedElement) &&
                ReferenceEquals(ScrollViewContainingElement(window, focusedElement), activeScrollView);
            bool reusableDescendantFocused =
                directFocus ||
                navigationScopeDescendantFocused ||
                (editMode && childOwnsActiveScrollView) ||
                (editMode && childOwnsActiveScrollViewEditElement) ||
                (editMode &&
                    focusedElement is not null &&
                    WindowContainsElement(renderedReusable.Child, focusedElement));
            bool applyReusableFocusStyle = ReusableFocusStyleAppliesToChild(
                renderedReusable,
                directFocus,
                reusableDescendantFocused && !directFocus);
            if (reusableDescendantFocused &&
                applyReusableFocusStyle &&
                renderedReusable.Child is not GeneratedScrollViewBase &&
                renderedReusable.FocusStyle?.Background is not null)
            {
                ApplyReusableFocusBackgroundToBuffer(
                    buffer,
                    new Rect(row, col, size.Width, size.Height),
                    renderedReusable.FocusStyle.Background,
                    ReusableFocusBaseBackground(renderedReusable, entry));
            }
        }
        if (element is ScrollView renderedScrollView)
        {
            RenderScrollViewChildrenOverlay(
                window,
                buffer,
                entry,
                renderedScrollView,
                new Rect(row, col, size.Width, size.Height),
                focusedElement,
                editMode,
                activeScrollView,
                activeScrollViewProxy,
                activeScrollViewEditElement,
                activeScrollViewFresh,
                activeScrollViewFocusBackgroundOverride,
                suppressActiveScrollViewScopeVisuals,
                parentBackground,
                useHostViewportForRootScrollViewIndicators);
        }
        if (element is ReusableElement focusedReusable &&
            !suppressActiveScrollViewScopeVisuals &&
            ReferenceEquals(element, focusedElement) &&
            focusedReusable.Child is GeneratedScrollViewBase)
        {
            ApplyFocusedReusableProxyBackgroundGaps(
                buffer,
                new Rect(row, col, size.Width, size.Height),
                focusedReusable);
        }
    }

    private static void RenderScrollViewChildrenOverlay(
        GeneratedWindowBase window,
        TerminalBuffer buffer,
        GeneratedLayoutEntry entry,
        ScrollView scrollView,
        Rect frame,
        Element? focusedElement,
        bool editMode,
        ScrollView? activeScrollView,
        ReusableElement? activeScrollViewProxy,
        Element? activeScrollViewEditElement,
        bool activeScrollViewFresh,
        Color? activeScrollViewFocusBackgroundOverride,
        bool suppressActiveScrollViewScopeVisuals,
        Color? parentBackground,
        bool useHostViewportForRootScrollViewIndicators)
    {
        Rect scrollViewport = ScrollViewViewportClip(frame, scrollView.Style);
        Rect scrollClip = scrollViewport;
        List<(Element Element, Rect Frame, int Height)> overlays = new();
        bool childContentAboveViewport = false;
        bool childContentBelowViewport = false;
        foreach (ScrollViewChildView childView in scrollView.ChildViews(new Size(frame.Width, frame.Height)))
        {
            Rect childFrame = new(
                frame.Row + childView.Frame.Row,
                frame.Col + childView.Frame.Col,
                childView.Frame.Width,
                childView.Frame.Height);
            if (childFrame.Row < scrollViewport.Row)
            {
                childContentAboveViewport = true;
            }
            if (childFrame.Row + childFrame.Height > scrollViewport.Row + scrollViewport.Height)
            {
                childContentBelowViewport = true;
            }
            if (!childView.Visible || childView.Element is null)
            {
                continue;
            }

            childView.Element.Frame = childFrame;
            if (childView.Element is ReusableElement reusable && reusable.Child is not null)
            {
                List<Element> childFocusable = FocusableElements(reusable.Child, activeScrollView);
                int childFocusedIndex = focusedElement is not null
                    ? childFocusable.IndexOf(focusedElement)
                    : -1;
                int effectiveChildFocusedIndex = childFocusedIndex;
                if (effectiveChildFocusedIndex < 0 &&
                    activeScrollViewEditElement is not null &&
                    WindowContainsElement(reusable.Child, activeScrollViewEditElement))
                {
                    effectiveChildFocusedIndex = childFocusable.IndexOf(activeScrollViewEditElement);
                }
                if (effectiveChildFocusedIndex < 0 &&
                    activeScrollView is not null &&
                    WindowContainsElement(reusable.Child, activeScrollView))
                {
                    effectiveChildFocusedIndex = childFocusable.IndexOf(activeScrollView);
                }

                ComboBox? activeChildComboBox = activeScrollViewEditElement as ComboBox;
                bool childOwnsActiveScrollView =
                    activeScrollView is not null &&
                    WindowContainsElement(reusable.Child, activeScrollView);
                bool childOwnsActiveScrollViewEditElement =
                    activeScrollViewEditElement is not null &&
                    WindowContainsElement(reusable.Child, activeScrollViewEditElement);
                bool overlayActiveChildComboBox = childOwnsActiveScrollViewEditElement && activeChildComboBox is not null;
                bool childHasFocusedElement =
                    focusedElement is not null &&
                    WindowContainsElement(reusable.Child, focusedElement);
                bool navigationScopeDescendantFocused =
                    !suppressActiveScrollViewScopeVisuals &&
                    !editMode &&
                    activeScrollView is not null &&
                    childHasFocusedElement &&
                    ReferenceEquals(ScrollViewContainingElement(window, focusedElement), activeScrollView);
                bool reusableDescendantFocused =
                    ReferenceEquals(childView.Element, focusedElement) ||
                    navigationScopeDescendantFocused ||
                    (editMode &&
                        (childFocusedIndex >= 0 ||
                            effectiveChildFocusedIndex >= 0 ||
                            childOwnsActiveScrollViewEditElement ||
                            childHasFocusedElement ||
                            childOwnsActiveScrollView));
                bool reusableWholeChildFocus =
                    reusableDescendantFocused &&
                    !HasMultipleFocusableElements(reusable.Child, activeScrollView);
                bool reusableChildEditMode =
                    effectiveChildFocusedIndex >= 0 &&
                    editMode &&
                    (activeScrollView is null ||
                        childOwnsActiveScrollView ||
                        childOwnsActiveScrollViewEditElement);
                Style? childWindowStyle = null;
                Color? childActiveScrollViewFocusBackground = null;
                Color? childDescendantFocusBackground = null;
                ScrollView? reusableGeneratedScrollView =
                    reusable.Child is GeneratedScrollViewBase reusableScrollViewWindow
                        ? reusableScrollViewWindow.ScrollView()
                        : null;
                if (reusableWholeChildFocus)
                {
                    childWindowStyle = reusable.Child.GeneratedWindowStyle.Clone();
                    bool reusableDirectFocus = ReferenceEquals(childView.Element, focusedElement);
                    bool applyReusableFocusStyle = ReusableFocusStyleAppliesToChild(
                        reusable,
                        reusableDirectFocus,
                        reusableDescendantFocused && !reusableDirectFocus);
                    if (applyReusableFocusStyle)
                    {
                        if (reusableGeneratedScrollView is null)
                        {
                            childWindowStyle.Merge(reusable.FocusStyle!);
                        }
                        if (reusable.FocusStyle?.Background is not null)
                        {
                            childActiveScrollViewFocusBackground = reusable.FocusStyle.Background;
                        }
                    }
                    if (childActiveScrollViewFocusBackground is null &&
                        activeScrollView?.DescendantFocusStyle?.Background is not null)
                    {
                        childActiveScrollViewFocusBackground = activeScrollView.DescendantFocusStyle.Background;
                    }
                    if (childActiveScrollViewFocusBackground is null &&
                        scrollView.DescendantFocusStyle?.Background is not null)
                    {
                        childActiveScrollViewFocusBackground = scrollView.DescendantFocusStyle.Background;
                    }
                    if (childActiveScrollViewFocusBackground is null &&
                        scrollView.FocusStyle?.Background is not null)
                    {
                        childActiveScrollViewFocusBackground = scrollView.FocusStyle.Background;
                    }
                    childDescendantFocusBackground = childActiveScrollViewFocusBackground;
                }

                bool activeScrollViewDescendantFocused =
                    ReferenceEquals(activeScrollView, scrollView) &&
                    focusedElement is not null &&
                    !ReferenceEquals(focusedElement, scrollView) &&
                    ReferenceEquals(ScrollViewContainingElement(window, focusedElement), scrollView);
                if (childDescendantFocusBackground is null &&
                    activeScrollViewDescendantFocused &&
                    reusableWholeChildFocus)
                {
                    childDescendantFocusBackground =
                        scrollView.DescendantFocusStyle?.Background ??
                        scrollView.FocusStyle?.Background;
                }

                int? childClipTop = null;
                int? childClipBottom = null;
                if (childView.Clipped)
                {
                    childClipTop = Math.Max(0, scrollClip.Row - childFrame.Row);
                    childClipBottom = Math.Max(0, Math.Min(
                        childFrame.Height,
                        scrollClip.Row + scrollClip.Height - childFrame.Row));
                }

                bool useHostViewportForRootScrollViewChild =
                    reusable is ViewHost &&
                    reusableGeneratedScrollView is not null;
                List<List<TerminalCell>> reusableChildContent = RenderGeneratedWindowContent(
                    reusable.Child,
                    new Size(childFrame.Width, childFrame.Height),
                    effectiveChildFocusedIndex,
                    focusedElement,
                    reusableChildEditMode,
                    activeScrollView,
                    activeScrollViewProxy,
                    activeScrollViewEditElement,
                    activeScrollViewFresh,
                    false,
                    childActiveScrollViewFocusBackground,
                    suppressActiveScrollViewScopeVisuals,
                    childWindowStyle,
                    childClipTop,
                    childClipBottom,
                    true,
                    useHostViewportForRootScrollViewChild);
                if (childDescendantFocusBackground is not null &&
                    reusableGeneratedScrollView is null)
                {
                    List<Color> descendantBackgrounds = new();
                    CollectDescendantBaseStyleBackgrounds(reusable.Child, descendantBackgrounds);
                    ApplyReusableFocusBackgroundToDescendantBackgrounds(
                        reusableChildContent,
                        childDescendantFocusBackground,
                        descendantBackgrounds);
                    if (reusableWholeChildFocus &&
                        scrollView.SelectedStyle?.Background is not null)
                    {
                        ApplySelectedBackgroundToFocusedReusableChild(
                            reusableChildContent,
                            scrollView.SelectedStyle.Background,
                            childDescendantFocusBackground,
                            descendantBackgrounds);
                    }
                }
                if (childActiveScrollViewFocusBackground is not null &&
                    reusableGeneratedScrollView is null)
                {
                    ApplyReusableFocusBackground(
                        reusableChildContent,
                        childActiveScrollViewFocusBackground,
                        reusable.Style.Background);
                }
                Blit(buffer, reusableChildContent, childFrame.Row, childFrame.Col, scrollClip);
                OffsetWindowElementFrames(reusable.Child, childFrame.Row, childFrame.Col);
                if (overlayActiveChildComboBox && activeChildComboBox is not null)
                {
                    Rect comboFrame = activeChildComboBox.Frame;
                    int comboRenderHeight = Math.Max(
                        comboFrame.Height,
                        scrollClip.Row + scrollClip.Height - comboFrame.Row);
                    overlays.Add((activeChildComboBox, comboFrame, comboRenderHeight));
                }
                continue;
            }

            bool childFocused = ReferenceEquals(childView.Element, focusedElement);
            bool childEditMode =
                childFocused &&
                editMode &&
                (activeScrollView is null || ReferenceEquals(activeScrollViewEditElement, childView.Element));
            int? directChildClipTop = null;
            int? directChildClipBottom = null;
            if (childView.Clipped)
            {
                directChildClipTop = Math.Max(0, scrollClip.Row - childFrame.Row);
                directChildClipBottom = Math.Max(0, Math.Min(
                    childFrame.Height,
                    scrollClip.Row + scrollClip.Height - childFrame.Row));
            }
            Blit(
                buffer,
                childView.Element.Render(
                    new Size(childFrame.Width, childFrame.Height),
                    new ElementRenderState
                    {
                        Focused = childFocused,
                        EditMode = childEditMode,
                        PassiveFocus = activeScrollView is not null && childFocused && !childEditMode,
                        ClipTop = directChildClipTop,
                        ClipBottom = directChildClipBottom,
                    }),
                childFrame.Row,
                childFrame.Col,
                scrollClip);
            if (childEditMode && childView.Element is ComboBox)
            {
                int comboRenderHeight = Math.Max(
                    childFrame.Height,
                    scrollClip.Row + scrollClip.Height - childFrame.Row);
                overlays.Add((childView.Element, childFrame, comboRenderHeight));
            }
        }
        foreach ((Element element, Rect overlayFrame, int height) in overlays)
        {
            element.Frame = new Rect(overlayFrame.Row, overlayFrame.Col, overlayFrame.Width, height);
            Blit(
                buffer,
                element.Render(
                    new Size(overlayFrame.Width, height),
                    new ElementRenderState
                    {
                        Focused = true,
                        EditMode = true,
                    }),
                overlayFrame.Row,
                overlayFrame.Col,
                scrollClip);
        }

        if (scrollClip.Width > 0 &&
            scrollClip.Height > 0 &&
            !(useHostViewportForRootScrollViewIndicators &&
                window is GeneratedScrollViewBase generatedScrollViewIndicatorWindow &&
                ReferenceEquals(generatedScrollViewIndicatorWindow.ScrollView(), scrollView)))
        {
            Size viewportSize = new(scrollViewport.Width, scrollViewport.Height);
            int indicatorCol = scrollViewport.Col + scrollViewport.Width - MinimumRenderableSize;
            int maxViewOffset = scrollView.MaxViewOffset(viewportSize);
            int viewOffset = scrollView.ViewOffset;
            if (viewOffset < maxViewOffset || childContentAboveViewport)
            {
                ApplyScrollIndicator(buffer, scrollViewport.Row, indicatorCol, "^", scrollViewport.Col);
            }
            if (viewOffset > 0 || childContentBelowViewport)
            {
                ApplyScrollIndicator(
                    buffer,
                    scrollViewport.Row + scrollViewport.Height - MinimumRenderableSize,
                    indicatorCol,
                    "v",
                    scrollViewport.Col);
            }
        }

        bool activeScrollViewFocused =
            (ReferenceEquals(activeScrollView, scrollView) ||
                ReferenceEquals(ScrollViewContainingElement(window, focusedElement), scrollView) ||
                ReferenceEquals(ScrollViewContainingElement(window, activeScrollViewEditElement), scrollView));
        bool scrollViewSelfFocused =
            ReferenceEquals(scrollView, focusedElement) &&
            !activeScrollViewFocused;
        bool generatedScrollViewProxyFocused =
            activeScrollViewFocusBackgroundOverride is not null &&
            window is GeneratedScrollViewBase generatedScrollViewWindow &&
            ReferenceEquals(generatedScrollViewWindow.ScrollView(), scrollView) &&
            !activeScrollViewFocused &&
            !scrollViewSelfFocused;
        if (!suppressActiveScrollViewScopeVisuals &&
            (activeScrollViewFocused || generatedScrollViewProxyFocused || scrollViewSelfFocused))
        {
            Color? scrollViewBaseBackground =
                scrollView.Style.Background ??
                parentBackground ??
                entry.CellStyle.Background;
            ApplyActiveScrollViewFocusBackgroundGaps(
                buffer,
                scrollView,
                frame,
                scrollViewBaseBackground,
                activeScrollViewFocusBackgroundOverride,
                parentBackground ?? entry.CellStyle.Background,
                Array.Empty<Rect>(),
                false,
                true);
        }
    }

    private static void ApplyFocusedReusableProxyBackgroundGaps(
        TerminalBuffer buffer,
        Rect rect,
        ReusableElement reusable)
    {
        Color? focusBackground = reusable.FocusStyle?.Background;
        Color? baseBackground = VisibleBackground(reusable.Style.Background);
        if (focusBackground is null || baseBackground is null)
        {
            return;
        }
        Color? focusedBaseBackground = BlendBackgroundOverExisting(focusBackground, baseBackground);
        int firstRow = Math.Max(0, rect.Row);
        int lastRow = Math.Min(buffer.Height, rect.Row + rect.Height);
        int firstCol = Math.Max(0, rect.Col);
        int lastCol = Math.Min(buffer.Width, rect.Col + rect.Width);
        for (int row = firstRow; row < lastRow; ++row)
        {
            (int First, int Last)? contentBounds =
                RowRenderedVisualContentBounds(buffer, row, firstCol, lastCol);
            (int First, int Last)? renderedContentBounds =
                RowRenderedContentBounds(buffer, row, firstCol, lastCol);
            int? contentStartCol = renderedContentBounds?.First - 1 ?? contentBounds?.First - 1;
            int? contentEndCol = null;
            if (contentBounds.HasValue)
            {
                TerminalCell lastVisualContentCell = buffer.Cell(row, contentBounds.Value.Last);
                bool lastVisualCellIsScrollIndicator =
                    lastVisualContentCell.Text == "^" || lastVisualContentCell.Text == "v";
                contentEndCol = lastVisualCellIsScrollIndicator
                    ? contentBounds.Value.Last
                    : Math.Min(lastCol - 1, contentBounds.Value.Last + 1);
            }
            if (contentStartCol.HasValue)
            {
                contentStartCol = Math.Max(firstCol, contentStartCol.Value);
            }
            for (int col = firstCol; col < lastCol; ++col)
            {
                if (contentStartCol.HasValue &&
                    contentEndCol.HasValue &&
                    col >= contentStartCol.Value &&
                    col <= contentEndCol.Value)
                {
                    continue;
                }
                TerminalCell cell = buffer.Cell(row, col);
                if (!ColorEquals(cell.Background, baseBackground) &&
                    !ColorEquals(cell.Background, focusedBaseBackground))
                {
                    continue;
                }
                if (!ColorEquals(cell.Background, focusedBaseBackground))
                {
                    cell.Background = BlendBackgroundOverExisting(focusBackground, cell.Background);
                }
                buffer.SetCell(row, col, cell);
            }
        }
    }

    private static Rect EntryContentClip(Rect cellRect, Style cellStyle)
    {
        int top = cellRect.Row + PaddingTop(cellStyle);
        int left = cellRect.Col + PaddingLeft(cellStyle);
        int bottom = cellRect.Row + cellRect.Height - PaddingBottom(cellStyle);
        int right = cellRect.Col + cellRect.Width - PaddingRight(cellStyle);
        return new Rect(
            top,
            left,
            Math.Max(0, right - left),
            Math.Max(0, bottom - top));
    }

    internal static Rect ScrollViewViewportClip(Rect frame, Style style)
    {
        int top = PaddingTop(style);
        int right = PaddingRight(style);
        int bottom = PaddingBottom(style);
        int left = PaddingLeft(style);
        return new Rect(
            frame.Row + top,
            frame.Col + left,
            Math.Max(0, frame.Width - left - right),
            Math.Max(0, frame.Height - top - bottom));
    }

    private static void Blit(TerminalBuffer buffer, List<List<TerminalCell>> content, int row, int col, Rect? clip = null)
    {
        for (int localRow = 0; localRow < content.Count; ++localRow)
        {
            for (int localCol = 0; localCol < content[localRow].Count; ++localCol)
            {
                int targetRow = row + localRow;
                int targetCol = col + localCol;
                if (targetRow < 0 || targetRow >= buffer.Height || targetCol < 0 || targetCol >= buffer.Width)
                {
                    continue;
                }
                if (clip.HasValue && !clip.Value.Contains(new Point(targetRow, targetCol)))
                {
                    continue;
                }
                TerminalCell cell = content[localRow][localCol].Clone();
                TerminalCell existing = buffer.Cell(targetRow, targetCol);
                cell.Background = BlendBackgroundOverExisting(cell.Background, existing.Background);
                cell.Foreground = BlendForegroundOverBackground(cell.Foreground, existing.Foreground, cell.Background);
                buffer.SetCell(targetRow, targetCol, cell);
            }
        }
    }

    private static void OverlayFocusedComboBox(
        TerminalBuffer buffer,
        GeneratedWindowBase window,
        int focusedIndex,
        Element? focusedElement,
        bool editMode,
        ScrollView? activeScrollView,
        Element? activeScrollViewEditElement,
        int windowRow = 0,
        int windowCol = 0)
    {
        if (!editMode)
        {
            return;
        }
        ComboBox? comboBox = focusedElement as ComboBox;
        if (comboBox is null)
        {
            List<Element> focusable = FocusableElements(window, activeScrollView);
            if (focusedIndex < 0 || focusedIndex >= focusable.Count || focusable[focusedIndex] is not ComboBox focusedComboBox)
            {
                return;
            }
            comboBox = focusedComboBox;
        }
        if (activeScrollView is not null && !ReferenceEquals(activeScrollViewEditElement, comboBox))
        {
            return;
        }
        Size size = new(Math.Max(MinimumRenderableSize, comboBox.Frame.Width), Math.Max(MinimumRenderableSize, comboBox.Options.Count + 1));
        List<List<TerminalCell>> rendered = comboBox.Render(size, new ElementRenderState
        {
            Focused = true,
            EditMode = true,
        });
        Blit(buffer, rendered, windowRow + comboBox.Frame.Row, windowCol + comboBox.Frame.Col);
    }

    private static void DimOutsideActiveScrollViewScope(
        TerminalBuffer buffer,
        GeneratedWindowBase window,
        Element? focused,
        bool editMode,
        ScrollView? activeScrollView,
        ReusableElement? activeScrollViewProxy,
        ReusableElement? activeScrollViewStyleProxy,
        bool activeScrollViewFresh)
    {
        if (!editMode || (activeScrollView is null && focused is null))
        {
            return;
        }
        Rect? active;
        Color? dim;
        if (activeScrollView is not null)
        {
            active = ActiveScrollViewScopeRect(activeScrollView, activeScrollViewProxy, activeScrollViewFresh);
            dim = ActiveScrollViewScopeDimBackground(activeScrollView, activeScrollViewStyleProxy);
        }
        else
        {
            active = ActiveScrollViewScopeRect(window, focused!);
            dim = ActiveScrollViewScopeDimBackground(focused!);
        }
        if (active is null || dim is null)
        {
            return;
        }
        Rect rect = active.Value;
        Rect? proxyRect = activeScrollViewProxy?.Frame;
        Color? proxyFocusBackground = activeScrollViewProxy?.FocusStyle?.Background;
        Color? gapFocusBackground = activeScrollView is not null
            ? ActiveScrollViewScopeFocusBackground(activeScrollView, activeScrollViewStyleProxy)
            : null;
        Color? viewportBackground = activeScrollView is not null
            ? ActiveScrollViewViewportBackground(activeScrollView, activeScrollViewStyleProxy)
            : null;
        ApplyActiveScrollViewGapFocusBackground(buffer, rect, gapFocusBackground, viewportBackground);
        for (int row = 0; row < buffer.Height; ++row)
        {
            for (int col = 0; col < buffer.Width; ++col)
            {
                if (rect.Contains(new Point(row, col)))
                {
                    continue;
                }
                TerminalCell cell = buffer.Cell(row, col);
                if (proxyRect.HasValue &&
                    proxyRect.Value.Contains(new Point(row, col)) &&
                    proxyFocusBackground is not null)
                {
                    cell.Background = BlendBackgroundOverExisting(proxyFocusBackground, cell.Background);
                    buffer.SetCell(row, col, cell);
                    continue;
                }
                if (cell.Text == TerminalImageHalfBlockGlyph)
                {
                    cell.Foreground = DimForegroundOverExisting(dim, cell.Foreground);
                    cell.Background = DimBackgroundOverExisting(dim, cell.Background);
                    buffer.SetCell(row, col, cell);
                    continue;
                }
                cell.Background = DimBackgroundOverExisting(dim, cell.Background);
                buffer.SetCell(row, col, cell);
            }
        }
    }

    private static Color? DimBackgroundOverExisting(Color dim, Color? background)
    {
        return dim.Rgba.HasValue && background is not null
            ? dim.BlendOver(background)
            : dim;
    }

    private static Color? DimForegroundOverExisting(Color dim, Color? foreground)
    {
        if (foreground is null)
        {
            return null;
        }
        return dim.Rgba.HasValue
            ? dim.BlendOver(foreground)
            : dim;
    }

    private static void ApplyActiveScrollViewGapFocusBackground(
        TerminalBuffer buffer,
        Rect active,
        Color? focusBackground,
        Color? viewportBackground)
    {
        if (focusBackground is null || viewportBackground is null)
        {
            return;
        }
        Color? focusedViewportBackground = BlendBackgroundOverExisting(focusBackground, viewportBackground);
        int firstRow = Math.Max(0, active.Row);
        int lastRow = Math.Min(buffer.Height, active.Row + active.Height);
        int firstCol = Math.Max(0, active.Col);
        int lastCol = Math.Min(buffer.Width, active.Col + active.Width);
        for (int row = firstRow; row < lastRow; ++row)
        {
            int? contentStartCol = null;
            for (int col = firstCol; col < lastCol; ++col)
            {
                TerminalCell cell = buffer.Cell(row, col);
                if (CellHasRenderedContent(cell))
                {
                    contentStartCol = Math.Max(firstCol, col - 1);
                    break;
                }
            }
            for (int col = firstCol; col < lastCol; ++col)
            {
                if (contentStartCol.HasValue && col >= contentStartCol.Value)
                {
                    continue;
                }
                TerminalCell cell = buffer.Cell(row, col);
                if (!ColorEquals(cell.Background, viewportBackground) &&
                    !ColorEquals(cell.Background, focusedViewportBackground))
                {
                    continue;
                }
                if (!ColorEquals(cell.Background, focusedViewportBackground))
                {
                    cell.Background = BlendBackgroundOverExisting(
                        focusBackground,
                        BlendBackgroundOverExisting(focusBackground, cell.Background));
                }
                buffer.SetCell(row, col, cell);
            }
        }
    }

    private static void ApplyActiveScrollViewFocusBackground(
        TerminalBuffer buffer,
        Rect active,
        Color focusBackground)
    {
        int firstRow = Math.Max(0, active.Row);
        int lastRow = Math.Min(buffer.Height, active.Row + active.Height);
        int firstCol = Math.Max(0, active.Col);
        int lastCol = Math.Min(buffer.Width, active.Col + active.Width);
        for (int row = firstRow; row < lastRow; ++row)
        {
            for (int col = firstCol; col < lastCol; ++col)
            {
                TerminalCell cell = buffer.Cell(row, col);
                cell.Background = BlendBackgroundOverExisting(focusBackground, cell.Background);
                buffer.SetCell(row, col, cell);
            }
        }
    }

    private static void ApplyActiveScrollViewFocusBackgroundGaps(
        List<List<TerminalCell>> content,
        ScrollView activeScrollView,
        Rect active,
        Color? baseBackground,
        Color? focusBackgroundOverride = null,
        Color? fallbackBackground = null,
        IReadOnlyList<Rect>? excludedRects = null,
        bool doubleBlendBaseBackground = true,
        bool exactAlphaBlend = false)
    {
        Color? focusBackground = focusBackgroundOverride ?? activeScrollView.FocusStyle?.Background;
        if (focusBackground is null)
        {
            return;
        }
        Color? Blend(Color color, Color? background)
        {
            if (!color.Rgba.HasValue)
            {
                return color;
            }
            Rgba foreground = color.Rgba.Value;
            if (foreground.Alpha >= byte.MaxValue)
            {
                return color;
            }
            if (foreground.Alpha == 0)
            {
                return background;
            }
            if (background?.Rgba is not Rgba baseRgba)
            {
                return color;
            }
            double alpha = exactAlphaBlend
                ? foreground.Alpha / 255.0
                : Math.Round((foreground.Alpha / 255.0) * 100.0) / 100.0;
            static byte Channel(double alpha, byte foreground, byte background)
            {
                return (byte)Math.Round(alpha * foreground + (1.0 - alpha) * background);
            }
            return Color.Rgb(
                Channel(alpha, foreground.Red, baseRgba.Red),
                Channel(alpha, foreground.Green, baseRgba.Green),
                Channel(alpha, foreground.Blue, baseRgba.Blue));
        }
        Color? effectiveBaseBackground = baseBackground;
        if ((effectiveBaseBackground is null ||
                effectiveBaseBackground.Rgba is Rgba effectiveRgba &&
                effectiveRgba.Alpha < byte.MaxValue) &&
            active.Row >= 0 &&
            active.Row < content.Count &&
            active.Col >= 0 &&
            active.Col < content[active.Row].Count)
        {
            effectiveBaseBackground = content[active.Row][active.Col].Background;
        }
        Color? focusedBaseBackground = effectiveBaseBackground is not null
            ? Blend(focusBackground, effectiveBaseBackground)
            : null;
        bool transparentViewportBackground =
            baseBackground is not null &&
            baseBackground.Rgba is Rgba baseRgba &&
            baseRgba.Alpha < byte.MaxValue;
        int firstRow = Math.Max(0, active.Row);
        int lastRow = Math.Min(content.Count, active.Row + active.Height);
        for (int row = firstRow; row < lastRow; ++row)
        {
            int firstCol = Math.Max(0, active.Col);
            int lastCol = Math.Min(content[row].Count, active.Col + active.Width);
            int? protectedContentStartCol = null;
            int? protectedContentEndCol = null;
            if (transparentViewportBackground)
            {
                (int First, int Last)? contentBounds = RowRenderedContentBounds(content[row], firstCol, lastCol);
                if (contentBounds.HasValue)
                {
                    protectedContentStartCol = Math.Clamp(
                        active.Col + PaddingLeft(activeScrollView.Style),
                        active.Col,
                        active.Col + active.Width);
                    protectedContentEndCol = Math.Clamp(
                        lastCol - PaddingRight(activeScrollView.Style) - 1,
                        active.Col - 1,
                        lastCol - 1);
                }
            }
            for (int col = firstCol; col < lastCol; ++col)
            {
                if (excludedRects is not null &&
                    excludedRects.Any(rect => rect.Contains(new Point(row, col))))
                {
                    continue;
                }
                if (protectedContentStartCol.HasValue &&
                    protectedContentEndCol.HasValue &&
                    col >= protectedContentStartCol.Value &&
                    col <= protectedContentEndCol.Value)
                {
                    continue;
                }
                TerminalCell cell = content[row][col];
                bool matchesBaseBackground =
                    effectiveBaseBackground is null ||
                    cell.Background is null ||
                    ColorEquals(cell.Background, effectiveBaseBackground);
                bool matchesFallbackBackground =
                    fallbackBackground is not null &&
                    cell.Background is not null &&
                    ColorEquals(cell.Background, fallbackBackground);
                bool matchesFocusedBaseBackground =
                    focusedBaseBackground is not null &&
                    cell.Background is not null &&
                    ColorEquals(cell.Background, focusedBaseBackground);
                if (!matchesBaseBackground && !matchesFallbackBackground && !matchesFocusedBaseBackground)
                {
                    continue;
                }
                if (matchesFocusedBaseBackground)
                {
                    continue;
                }
                cell.Background = Blend(focusBackground, cell.Background);
                if (doubleBlendBaseBackground && (matchesBaseBackground || matchesFallbackBackground))
                {
                    cell.Background = Blend(focusBackground, cell.Background);
                }
                content[row][col] = cell;
            }
        }
    }

    private static void ApplyActiveScrollViewFocusBackgroundGaps(
        TerminalBuffer buffer,
        ScrollView activeScrollView,
        Rect active,
        Color? baseBackground,
        Color? focusBackgroundOverride = null,
        Color? fallbackBackground = null,
        IReadOnlyList<Rect>? excludedRects = null,
        bool doubleBlendBaseBackground = true,
        bool exactAlphaBlend = false)
    {
        Color? focusBackground = focusBackgroundOverride ?? activeScrollView.FocusStyle?.Background;
        if (focusBackground is null)
        {
            return;
        }
        Color? Blend(Color color, Color? background)
        {
            if (!color.Rgba.HasValue)
            {
                return color;
            }
            Rgba foreground = color.Rgba.Value;
            if (foreground.Alpha >= byte.MaxValue)
            {
                return color;
            }
            if (foreground.Alpha == 0)
            {
                return background;
            }
            if (background?.Rgba is not Rgba baseRgba)
            {
                return color;
            }
            double alpha = exactAlphaBlend
                ? foreground.Alpha / 255.0
                : Math.Round((foreground.Alpha / 255.0) * 100.0) / 100.0;
            static byte Channel(double alpha, byte foreground, byte background)
            {
                return (byte)Math.Round(alpha * foreground + (1.0 - alpha) * background);
            }
            return Color.Rgb(
                Channel(alpha, foreground.Red, baseRgba.Red),
                Channel(alpha, foreground.Green, baseRgba.Green),
                Channel(alpha, foreground.Blue, baseRgba.Blue));
        }
        Color? effectiveBaseBackground = baseBackground;
        if ((effectiveBaseBackground is null ||
                effectiveBaseBackground.Rgba is Rgba effectiveRgba &&
                effectiveRgba.Alpha < byte.MaxValue) &&
            active.Row >= 0 &&
            active.Row < buffer.Height &&
            active.Col >= 0 &&
            active.Col < buffer.Width)
        {
            effectiveBaseBackground = buffer.Cell(active.Row, active.Col).Background;
        }
        Color? focusedBaseBackground = effectiveBaseBackground is not null
            ? Blend(focusBackground, effectiveBaseBackground)
            : null;
        bool transparentViewportBackground =
            baseBackground is not null &&
            baseBackground.Rgba is Rgba baseRgba &&
            baseRgba.Alpha < byte.MaxValue;
        int firstRow = Math.Max(0, active.Row);
        int lastRow = Math.Min(buffer.Height, active.Row + active.Height);
        for (int row = firstRow; row < lastRow; ++row)
        {
            int firstCol = Math.Max(0, active.Col);
            int lastCol = Math.Min(buffer.Width, active.Col + active.Width);
            int? protectedContentStartCol = null;
            int? protectedContentEndCol = null;
            if (transparentViewportBackground)
            {
                (int First, int Last)? contentBounds = RowRenderedContentBounds(buffer, row, firstCol, lastCol);
                if (contentBounds.HasValue)
                {
                    protectedContentStartCol = Math.Clamp(
                        active.Col + PaddingLeft(activeScrollView.Style),
                        active.Col,
                        active.Col + active.Width);
                    protectedContentEndCol = Math.Clamp(
                        lastCol - PaddingRight(activeScrollView.Style) - 1,
                        active.Col - 1,
                        lastCol - 1);
                }
            }
            for (int col = firstCol; col < lastCol; ++col)
            {
                if (excludedRects is not null &&
                    excludedRects.Any(rect => rect.Contains(new Point(row, col))))
                {
                    continue;
                }
                if (protectedContentStartCol.HasValue &&
                    protectedContentEndCol.HasValue &&
                    col >= protectedContentStartCol.Value &&
                    col <= protectedContentEndCol.Value)
                {
                    continue;
                }
                TerminalCell cell = buffer.Cell(row, col);
                bool matchesBaseBackground =
                    effectiveBaseBackground is null ||
                    cell.Background is null ||
                    ColorEquals(cell.Background, effectiveBaseBackground);
                bool matchesFallbackBackground =
                    fallbackBackground is not null &&
                    cell.Background is not null &&
                    ColorEquals(cell.Background, fallbackBackground);
                bool matchesFocusedBaseBackground =
                    focusedBaseBackground is not null &&
                    cell.Background is not null &&
                    ColorEquals(cell.Background, focusedBaseBackground);
                if (!matchesBaseBackground && !matchesFallbackBackground && !matchesFocusedBaseBackground)
                {
                    continue;
                }
                if (matchesFocusedBaseBackground)
                {
                    continue;
                }
                cell.Background = Blend(focusBackground, cell.Background);
                if (doubleBlendBaseBackground && (matchesBaseBackground || matchesFallbackBackground))
                {
                    cell.Background = Blend(focusBackground, cell.Background);
                }
                buffer.SetCell(row, col, cell);
            }
        }
    }

    private static (int First, int Last)? RowRenderedContentBounds(
        List<TerminalCell> row,
        int firstCol,
        int lastCol)
    {
        int? first = null;
        int last = firstCol;
        for (int col = firstCol; col < lastCol; ++col)
        {
            if (!CellHasRenderedContent(row[col]))
            {
                continue;
            }
            first ??= col;
            last = col;
        }
        if (first is null)
        {
            return null;
        }
        return (first.Value, last);
    }

    private static (int First, int Last)? RowRenderedVisualContentBounds(
        List<TerminalCell> row,
        int firstCol,
        int lastCol)
    {
        int? first = null;
        int last = firstCol;
        for (int col = firstCol; col < lastCol; ++col)
        {
            TerminalCell cell = row[col];
            if (!CellHasRenderedContent(cell) && (cell.Foreground is null || cell.Foreground.IsTransparent))
            {
                continue;
            }
            first ??= col;
            last = col;
        }
        if (first is null)
        {
            return null;
        }
        return (first.Value, last);
    }

    private static bool CellHasRenderedContent(TerminalCell cell)
    {
        if (!string.IsNullOrEmpty(cell.Raw))
        {
            return true;
        }
        return !string.IsNullOrEmpty(cell.Text) &&
            cell.Text != " " &&
            cell.Text != "^" &&
            cell.Text != "v";
    }

    private static (int First, int Last)? RowRenderedContentBounds(List<TerminalCell> row)
    {
        int? first = null;
        int last = 0;
        for (int index = 0; index < row.Count; ++index)
        {
            if (!CellHasRenderedContent(row[index]))
            {
                continue;
            }
            first ??= index;
            last = index;
        }
        if (first is null)
        {
            return null;
        }
        return (first.Value, last);
    }

    private static (int First, int Last)? RowRenderedContentBounds(
        TerminalBuffer buffer,
        int row,
        int firstCol,
        int lastCol)
    {
        int? first = null;
        int last = firstCol;
        for (int col = firstCol; col < lastCol; ++col)
        {
            if (!CellHasRenderedContent(buffer.Cell(row, col)))
            {
                continue;
            }
            first ??= col;
            last = col;
        }
        if (first is null)
        {
            return null;
        }
        return (first.Value, last);
    }

    private static (int First, int Last)? RowRenderedVisualContentBounds(
        TerminalBuffer buffer,
        int row,
        int firstCol,
        int lastCol)
    {
        int? first = null;
        int last = firstCol;
        for (int col = firstCol; col < lastCol; ++col)
        {
            TerminalCell cell = buffer.Cell(row, col);
            if (!CellHasRenderedContent(cell) && (cell.Foreground is null || cell.Foreground.IsTransparent))
            {
                continue;
            }
            first ??= col;
            last = col;
        }
        if (first is null)
        {
            return null;
        }
        return (first.Value, last);
    }

    private static Color? ActiveScrollViewViewportBackground(ScrollView scrollView, ReusableElement? proxy)
    {
        if (proxy?.Child is GeneratedScrollViewBase child && ReferenceEquals(child.ScrollView(), scrollView))
        {
            Color? childBackground = VisibleBackground(child.GeneratedLayout
                .FirstOrDefault(entry => entry.Name == "__scrollview")?.CellStyle.Background);
            if (childBackground is not null)
            {
                return childBackground;
            }
        }
        Color? scrollViewBackground = VisibleBackground(scrollView.Style.Background);
        if (scrollViewBackground is not null)
        {
            return scrollViewBackground;
        }
        return VisibleBackground(proxy?.Style.Background);
    }

    private static Color? ActiveScrollViewScopeFocusBackground(ScrollView scrollView, ReusableElement? proxy)
    {
        Color? proxyBackground = proxy?.FocusStyle?.Background;
        if (proxyBackground is not null)
        {
            if (proxyBackground.Rgba is Rgba rgba && rgba.Alpha < byte.MaxValue)
            {
                return null;
            }
            return proxyBackground;
        }
        return scrollView.FocusStyle?.Background;
    }

    private static Color? GeneratedScrollViewProxyFocusBackground(ScrollView scrollView, ReusableElement proxy)
    {
        Color? proxyBackground = proxy.FocusStyle?.Background;
        if (proxyBackground is not null &&
            (proxyBackground.Rgba is not Rgba rgba || rgba.Alpha == byte.MaxValue))
        {
            return proxyBackground;
        }
        return scrollView.DescendantFocusStyle?.Background ?? scrollView.FocusStyle?.Background;
    }

    private static Color? VisibleBackground(Color? background)
    {
        return background is null || background.IsTransparent ? null : background;
    }

    private static bool OpaqueBackground(Color? background)
    {
        return background?.Rgba is Rgba rgba && rgba.Alpha == byte.MaxValue;
    }

    private static bool PartialBackground(Color? background)
    {
        return background?.Rgba is Rgba rgba && rgba.Alpha < byte.MaxValue;
    }

    private static bool ReusableFocusStyleAppliesToChild(
        ReusableElement reusable,
        bool directFocus,
        bool descendantOnlyFocus)
    {
        if (reusable.FocusStyle is null)
        {
            return false;
        }
        Color? background = reusable.FocusStyle.Background;
        if (!PartialBackground(background))
        {
            return true;
        }
        if (reusable.Child is GeneratedScrollViewBase)
        {
            return false;
        }
        return directFocus || !descendantOnlyFocus;
    }

    private static Color? ReusableFocusBaseBackground(
        ReusableElement reusable,
        GeneratedLayoutEntry entry)
    {
        return reusable.Style.Background ?? entry.CellStyle.Background;
    }

    private static void ApplyReusableFocusBackground(
        List<List<TerminalCell>> content,
        Color focusBackground,
        Color? baseBackground)
    {
        Color? focusedBaseBackground = baseBackground is not null
            ? BlendBackgroundOverExisting(focusBackground, baseBackground)
            : null;
        foreach (List<TerminalCell> row in content)
        {
            foreach (TerminalCell cell in row)
            {
                bool matchesUnresolvedFocus =
                    focusedBaseBackground is not null &&
                    ColorEquals(cell.Background, focusBackground);
                if (matchesUnresolvedFocus)
                {
                    cell.Background = focusBackground.Rgba.HasValue
                        ? BlendBackgroundOverExisting(focusBackground, focusedBaseBackground)
                        : focusBackground;
                    continue;
                }
                bool matchesBase =
                    cell.Background is null ||
                    (baseBackground is not null && ColorEquals(cell.Background, baseBackground));
                bool matchesFocusedBase =
                    focusedBaseBackground is not null &&
                    ColorEquals(cell.Background, focusedBaseBackground);
                if (!matchesBase && !matchesFocusedBase)
                {
                    continue;
                }
                if (matchesFocusedBase)
                {
                    continue;
                }
                cell.Background = focusBackground.Rgba.HasValue && cell.Background is not null
                    ? BlendBackgroundOverExisting(focusBackground, cell.Background)
                    : focusBackground;
            }
        }
    }

    private static void ApplyReusableFocusBackgroundToBuffer(
        TerminalBuffer buffer,
        Rect rect,
        Color focusBackground,
        Color? baseBackground)
    {
        Color? focusedBaseBackground = baseBackground is not null
            ? BlendBackgroundOverExisting(focusBackground, baseBackground)
            : null;
        int firstRow = Math.Max(0, rect.Row);
        int lastRow = Math.Min(buffer.Height, rect.Row + rect.Height);
        int firstCol = Math.Max(0, rect.Col);
        int lastCol = Math.Min(buffer.Width, rect.Col + rect.Width);
        for (int row = firstRow; row < lastRow; ++row)
        {
            for (int col = firstCol; col < lastCol; ++col)
            {
                TerminalCell cell = buffer.Cell(row, col);
                bool matchesBase =
                    cell.Background is null ||
                    (baseBackground is not null && ColorEquals(cell.Background, baseBackground));
                bool matchesFocusedBase = focusedBaseBackground is not null &&
                    ColorEquals(cell.Background, focusedBaseBackground);
                if (!matchesBase && !matchesFocusedBase)
                {
                    continue;
                }
                if (matchesFocusedBase)
                {
                    continue;
                }
                cell.Background = focusBackground.Rgba.HasValue && cell.Background is not null
                    ? BlendBackgroundOverExisting(focusBackground, cell.Background)
                    : focusBackground;
                buffer.SetCell(row, col, cell);
            }
        }
    }

    private static void AppendUniqueBackground(List<Color> backgrounds, Color? background)
    {
        if (!OpaqueBackground(background))
        {
            return;
        }
        if (!backgrounds.Any(existing => existing.Equals(background)))
        {
            backgrounds.Add(background!);
        }
    }

    private static void CollectDescendantBaseStyleBackgrounds(
        GeneratedWindowBase window,
        List<Color> backgrounds)
    {
        foreach (Element element in window.Elements)
        {
            CollectDescendantBaseStyleBackgrounds(element, backgrounds);
        }
    }

    private static void CollectDescendantBaseStyleBackgrounds(
        Element element,
        List<Color> backgrounds)
    {
        AppendUniqueBackground(backgrounds, element.Style.Background);
        AppendUniqueBackground(backgrounds, element.FocusStyle?.Background);
        AppendUniqueBackground(backgrounds, element.EditStyle?.Background);
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            CollectDescendantBaseStyleBackgrounds(reusable.Child, backgrounds);
        }
        if (element is ScrollView scrollView)
        {
            foreach (Element child in scrollView.Children)
            {
                CollectDescendantBaseStyleBackgrounds(child, backgrounds);
            }
        }
    }

    private static void ApplyReusableFocusBackgroundToDescendantBackgrounds(
        List<List<TerminalCell>> content,
        Color focusBackground,
        List<Color> backgrounds)
    {
        if (backgrounds.Count == 0)
        {
            return;
        }
        foreach (List<TerminalCell> row in content)
        {
            foreach (TerminalCell cell in row)
            {
                if (cell.Background is null)
                {
                    continue;
                }
                foreach (Color background in backgrounds)
                {
                    Color? focusedBackground = focusBackground.Rgba.HasValue
                        ? BlendBackgroundOverExisting(focusBackground, background)
                        : focusBackground;
                    if (ColorEquals(cell.Background, background) ||
                        ColorEquals(cell.Background, focusedBackground))
                    {
                        cell.Background = focusedBackground;
                        break;
                    }
                }
            }
        }
    }

    private static void ApplySelectedBackgroundToFocusedReusableChild(
        List<List<TerminalCell>> content,
        Color selectedBackground,
        Color? focusBackground,
        List<Color> descendantBackgrounds)
    {
        foreach (List<TerminalCell> row in content)
        {
            foreach (TerminalCell cell in row)
            {
                if (cell.Background is null)
                {
                    continue;
                }

                bool descendantBackground = false;
                foreach (Color background in descendantBackgrounds)
                {
                    if (ColorEquals(cell.Background, background))
                    {
                        descendantBackground = true;
                        break;
                    }
                    if (focusBackground is not null)
                    {
                        Color? focusedBackground = focusBackground.Rgba.HasValue
                            ? BlendBackgroundOverExisting(focusBackground, background)
                            : focusBackground;
                        if (ColorEquals(cell.Background, focusedBackground))
                        {
                            descendantBackground = true;
                            break;
                        }
                    }
                }
                if (descendantBackground)
                {
                    continue;
                }

                cell.Background = selectedBackground.Rgba.HasValue
                    ? BlendBackgroundOverExisting(selectedBackground, cell.Background)
                    : selectedBackground;
            }
        }
    }

    private static bool ColorEquals(Color? left, Color? right)
    {
        if (left is null || right is null)
        {
            return left is null && right is null;
        }
        return left.Equals(right);
    }

    private static Color? InferredScrollIndicatorForeground(
        TerminalBuffer buffer,
        int row,
        int startCol,
        int endCol)
    {
        if (row < 0 || row >= buffer.Height)
        {
            return null;
        }
        Color? foreground = null;
        int start = Math.Max(0, startCol);
        int end = Math.Min(buffer.Width - MinimumRenderableSize, endCol);
        for (int scanCol = start; scanCol <= end; ++scanCol)
        {
            Color? scanned = buffer.Cell(row, scanCol).Foreground;
            if (scanned is not null && !scanned.IsTransparent)
            {
                foreground = scanned;
            }
        }
        return foreground;
    }

    private static Color? InferredScrollIndicatorForeground(
        List<TerminalCell> row,
        int startCol,
        int endCol)
    {
        Color? foreground = null;
        int start = Math.Max(0, startCol);
        int end = Math.Min(row.Count - MinimumRenderableSize, endCol);
        for (int scanCol = start; scanCol <= end; ++scanCol)
        {
            Color? scanned = row[scanCol].Foreground;
            if (scanned is not null && !scanned.IsTransparent)
            {
                foreground = scanned;
            }
        }
        return foreground;
    }

    private static void ApplyScrollIndicator(
        TerminalBuffer buffer,
        int row,
        int col,
        string indicator,
        int foregroundStartCol)
    {
        if (row < 0 || row >= buffer.Height || col < 0 || col >= buffer.Width)
        {
            return;
        }
        TerminalCell cell = buffer.Cell(row, col).Clone();
        cell.Text = indicator;
        if (cell.Foreground is null || cell.Foreground.IsTransparent)
        {
            cell.Foreground = InferredScrollIndicatorForeground(buffer, row, foregroundStartCol, col);
        }
        buffer.SetCell(row, col, cell);
    }

    private static void ApplyScrollIndicator(
        List<List<TerminalCell>> content,
        int row,
        int col,
        string indicator,
        int foregroundStartCol)
    {
        if (row < 0 || row >= content.Count)
        {
            return;
        }
        List<TerminalCell> renderedRow = content[row];
        if (col < 0 || col >= renderedRow.Count)
        {
            return;
        }
        TerminalCell cell = renderedRow[col];
        cell.Text = indicator;
        if (cell.Foreground is null || cell.Foreground.IsTransparent)
        {
            cell.Foreground = InferredScrollIndicatorForeground(renderedRow, foregroundStartCol, col);
        }
    }

    private static Rect ScrollViewViewportClipClampedToContent(Rect frame, Style style, Size contentSize)
    {
        int left = PaddingLeft(style);
        int right = PaddingRight(style);
        int top = PaddingTop(style);
        int bottom = PaddingBottom(style);
        Rect viewport = new(
            frame.Row + top,
            frame.Col + left,
            Math.Max(0, frame.Width - left - right),
            Math.Max(0, frame.Height - top - bottom));
        viewport = new Rect(
            viewport.Row,
            viewport.Col,
            Math.Min(viewport.Width, Math.Max(0, contentSize.Width - viewport.Col)),
            Math.Min(viewport.Height, Math.Max(0, contentSize.Height - viewport.Row)));
        return viewport;
    }

    private static Rect GeneratedScrollViewSourceFrame(
        GeneratedWindowBase window,
        ScrollView scrollView,
        Size fallback)
    {
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            if (entry.Name == scrollView.Name)
            {
                return entry.SourceCell;
            }
        }
        return new Rect(0, 0, fallback.Width, fallback.Height);
    }

    private static Rect ActiveScrollViewScopeRect(ScrollView scrollView, ReusableElement? proxy, bool fresh)
    {
        if (proxy is not null)
        {
            return new Rect(
                proxy.Frame.Row,
                proxy.Frame.Col,
                Math.Max(MinimumRenderableSize, proxy.Frame.Width),
                Math.Max(MinimumRenderableSize, proxy.Frame.Height));
        }
        Rect frame = scrollView.Frame;
        int top = ActiveScrollViewViewportTopInset(scrollView, proxy, fresh);
        return new Rect(
            frame.Row + top,
            frame.Col,
            Math.Max(MinimumRenderableSize, frame.Width),
            Math.Max(MinimumRenderableSize, frame.Height - top));
    }

    private static int ActiveScrollViewViewportTopInset(ScrollView scrollView, ReusableElement? proxy, bool fresh)
    {
        if (!fresh && scrollView.ScrollOffset > 0)
        {
            return 0;
        }
        if (proxy?.Child is GeneratedScrollViewBase child && ReferenceEquals(child.ScrollView(), scrollView))
        {
            GeneratedLayoutEntry? entry = child.GeneratedLayout.FirstOrDefault(candidate => candidate.Name == "__scrollview");
            if (entry is not null)
            {
                return PaddingTop(entry.CellStyle);
            }
        }
        return PaddingTop(scrollView.Style);
    }

    private static Rect? ActiveScrollViewScopeRect(GeneratedWindowBase window, Element focused)
    {
        if (focused is ScrollView scrollView)
        {
            ReusableElement? host = ReusableContainingElement(window, scrollView);
            if (host is not null)
            {
                return PaddedReusableRect(host);
            }
            return scrollView.Frame;
        }
        if (focused is ReusableElement reusable && reusable.Child is GeneratedScrollViewBase child)
        {
            _ = child;
            return PaddedReusableRect(reusable);
        }
        return null;
    }

    private static Rect PaddedReusableRect(ReusableElement reusable)
    {
        int paddingTop = PaddingTop(reusable.Style);
        int paddingRight = PaddingRight(reusable.Style);
        int paddingBottom = PaddingBottom(reusable.Style);
        int paddingLeft = PaddingLeft(reusable.Style);
        if (reusable.Child is GeneratedScrollViewBase child)
        {
            Rect childFrame = child.ScrollView().Frame;
            Style childCellStyle = child.GeneratedLayout
                .FirstOrDefault(entry => entry.Name == "__scrollview")?.CellStyle ?? new Style();
            int childPaddingTop = PaddingTop(childCellStyle);
            int childPaddingRight = PaddingRight(childCellStyle);
            int childPaddingBottom = PaddingBottom(childCellStyle);
            int childPaddingLeft = PaddingLeft(childCellStyle);
            return new Rect(
                reusable.Frame.Row + childFrame.Row + paddingTop + childPaddingTop,
                reusable.Frame.Col + childFrame.Col + paddingLeft + childPaddingLeft,
                Math.Max(MinimumRenderableSize, childFrame.Width - paddingLeft - paddingRight - childPaddingLeft - childPaddingRight),
                Math.Max(MinimumRenderableSize, childFrame.Height - paddingTop - paddingBottom - childPaddingTop - childPaddingBottom));
        }
        return new Rect(
            reusable.Frame.Row + paddingTop,
            reusable.Frame.Col + paddingLeft,
            Math.Max(MinimumRenderableSize, reusable.Frame.Width - paddingLeft - paddingRight),
            Math.Max(MinimumRenderableSize, reusable.Frame.Height - paddingTop - paddingBottom));
    }

    internal static ReusableElement? ReusableContainingElement(GeneratedWindowBase window, Element target)
    {
        foreach (Element element in window.Elements)
        {
            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                if (WindowContainsElement(reusable.Child, target))
                {
                    return reusable;
                }
                ReusableElement? nested = ReusableContainingElement(reusable.Child, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
        }
        return null;
    }

    internal static bool WindowContainsElement(GeneratedWindowBase window, Element target)
    {
        foreach (Element element in window.Elements)
        {
            if (ReferenceEquals(element, target) || ElementTreeContains(element, target))
            {
                return true;
            }
            if (element is ReusableElement reusable && reusable.Child is not null && WindowContainsElement(reusable.Child, target))
            {
                return true;
            }
        }
        return false;
    }

    private static bool ChildElementRepresentedInCurrentLayout(Element element, Element target)
    {
        if (ReferenceEquals(element, target))
        {
            return true;
        }
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            if (reusable.Frame.Width > 0 && reusable.Frame.Height > 0)
            {
                SyncReusableChildFrames(reusable, reusable.Frame);
            }
            return ElementRepresentedInCurrentLayout(reusable.Child, target);
        }
        if (element is not ScrollView scrollView)
        {
            return false;
        }
        Rect frame = scrollView.Frame;
        Size viewport = new(
            Math.Max(MinimumRenderableSize, frame.Width),
            Math.Max(MinimumRenderableSize, frame.Height));
        foreach (ScrollViewChildView childView in scrollView.ChildViews(viewport))
        {
            if (childView.Element is not null &&
                ChildElementRepresentedInCurrentLayout(childView.Element, target))
            {
                return true;
            }
        }
        return false;
    }

    private static bool ElementRepresentedInCurrentLayout(GeneratedWindowBase window, Element? target)
    {
        if (target is null)
        {
            return false;
        }
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            if (string.IsNullOrEmpty(entry.Name))
            {
                continue;
            }
            Element? element = FindElement(window, entry.Name);
            if (element is not null && ChildElementRepresentedInCurrentLayout(element, target))
            {
                return true;
            }
        }
        return false;
    }

    private static bool ActiveScrollViewRepresentedInCurrentLayout(GeneratedWindowBase window, ScrollView? activeScrollView)
    {
        if (activeScrollView is null)
        {
            return false;
        }
        ReusableElement? proxy = ReusableContainingElement(window, activeScrollView);
        return (proxy is not null && ElementRepresentedInCurrentLayout(window, proxy)) ||
            ElementRepresentedInCurrentLayout(window, activeScrollView);
    }

    private static ScrollView? ScrollViewContainingElement(GeneratedWindowBase window, Element? target)
    {
        if (target is null)
        {
            return null;
        }
        foreach (Element element in window.Elements)
        {
            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                ScrollView? nested = ScrollViewContainingElement(reusable.Child, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
            if (element is ScrollView scrollView)
            {
                ScrollView? found = ScrollViewContainingElement(scrollView, target);
                if (found is not null)
                {
                    return found;
                }
            }
        }
        return null;
    }

    private static ScrollView? ScrollViewContainingElement(ScrollView scrollView, Element target)
    {
        foreach (Element child in scrollView.Children)
        {
            if (ReferenceEquals(child, target))
            {
                return scrollView;
            }
            if (child is ReusableElement reusable &&
                reusable.Child is not null &&
                WindowContainsElement(reusable.Child, target))
            {
                return scrollView;
            }
            if (child is ScrollView nestedScrollView)
            {
                ScrollView? nested = ScrollViewContainingElement(nestedScrollView, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
            ScrollView? descendant = ScrollViewContainingElementInTree(child, target);
            if (descendant is not null)
            {
                return descendant;
            }
        }
        return null;
    }

    private static ScrollView? ScrollViewContainingElementInTree(Element element, Element target)
    {
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            ScrollView? nested = ScrollViewContainingElement(reusable.Child, target);
            if (nested is not null)
            {
                return nested;
            }
        }
        foreach (Element child in element.Children)
        {
            if (child is ScrollView scrollView)
            {
                ScrollView? nested = ScrollViewContainingElement(scrollView, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
            ScrollView? descendant = ScrollViewContainingElementInTree(child, target);
            if (descendant is not null)
            {
                return descendant;
            }
        }
        return null;
    }

    private static bool WindowOwnsElement(GeneratedWindowBase window, Element target)
    {
        foreach (Element element in window.Elements)
        {
            if (ReferenceEquals(element, target) || ElementTreeContainsLocal(element, target))
            {
                return true;
            }
        }
        return false;
    }

    private static bool ElementTreeContainsLocal(Element element, Element target)
    {
        foreach (Element child in element.Children)
        {
            if (ReferenceEquals(child, target) || ElementTreeContainsLocal(child, target))
            {
                return true;
            }
        }
        return false;
    }

    private static bool ElementTreeContains(Element element, Element target)
    {
        if (element is ReusableElement reusable && reusable.Child is not null &&
            WindowContainsElement(reusable.Child, target))
        {
            return true;
        }
        foreach (Element child in element.Children)
        {
            if (ReferenceEquals(child, target) || ElementTreeContains(child, target))
            {
                return true;
            }
        }
        return false;
    }

    private static Color? ActiveScrollViewScopeDimBackground(Element focused)
    {
        if (focused is ScrollView scrollView)
        {
            return scrollView.Style.ScopeDimBackground;
        }
        if (focused is ReusableElement reusable && reusable.Child is GeneratedScrollViewBase child)
        {
            return reusable.Style.ScopeDimBackground ?? child.ScrollView().Style.ScopeDimBackground;
        }
        return null;
    }

    private static Color? ActiveScrollViewScopeDimBackground(ScrollView scrollView, ReusableElement? proxy)
    {
        return proxy?.Style.ScopeDimBackground ?? scrollView.Style.ScopeDimBackground;
    }

    private static Color? BlendBackgroundOverExisting(Color? color, Color? existing)
    {
        if (color is null || color.IsTransparent)
        {
            return existing;
        }
        if (color.Rgba.HasValue && color.Rgba.Value.Alpha < 255 && existing is not null)
        {
            return color.BlendOver(existing);
        }
        return color;
    }

    private static Color? BlendForegroundOverBackground(Color? color, Color? inherited, Color? background)
    {
        if (color is null || color.IsTransparent)
        {
            return inherited;
        }
        if (color.Rgba.HasValue && color.Rgba.Value.Alpha < 255 && background is not null)
        {
            return color.BlendOver(background);
        }
        return color;
    }

    private sealed class RuntimeCell
    {
        public int SourceRow { get; init; }
        public int SourceCol { get; init; }
        public int SourceWidth { get; init; } = MinimumRenderableSize;
        public int SourceHeight { get; init; } = MinimumRenderableSize;
        public int? LogicalWidth { get; set; }
        public int? LogicalHeight { get; set; }
    }

    private sealed class RuntimeNode
    {
        public string Orientation { get; set; } = "";
        public List<RuntimeNode> Children { get; } = new();
        public RuntimeCell? Cell { get; set; }
        public int Left { get; set; }
        public int Top { get; set; }
        public int Right { get; set; }
        public int Bottom { get; set; }
        public int? LogicalWidth { get; set; }
        public int? LogicalHeight { get; set; }
    }

    private sealed record ResolvedRuntimeCell(RuntimeCell Cell, Rect Rect);

    private static List<ResolvedRuntimeCell> ResolveRuntimeCells(
        GeneratedWindowBase window,
        Size size,
        GeneratedWindowMode? modeOverride = null)
    {
        List<RuntimeCell> cells = CollectRuntimeCells(window);
        List<ResolvedRuntimeCell> resolved = new();
        ResolveRuntimeCellsWithFitPass(
            window,
            cells,
            new Rect(0, 0, Math.Max(MinimumRenderableSize, size.Width), Math.Max(MinimumRenderableSize, size.Height)),
            BorderWidthHorizontal(window.GeneratedWindowStyle),
            BorderWidthVertical(window.GeneratedWindowStyle),
            modeOverride ?? WindowMode(window),
            resolved);
        return resolved;
    }

    private static ResolvedRuntimeCell? ResolvedCellFor(IReadOnlyList<ResolvedRuntimeCell> cells, GeneratedLayoutEntry entry)
    {
        return cells.FirstOrDefault(cell => SameRuntimeCell(cell.Cell, entry));
    }

    private static void ResolveRuntimeCellsWithFitPass(
        GeneratedWindowBase window,
        List<RuntimeCell> runtimeCells,
        Rect content,
        int horizontalSeparator,
        int verticalSeparator,
        GeneratedWindowMode mode,
        List<ResolvedRuntimeCell> resolvedCells)
    {
        RuntimeNode? runtimeTree = BuildRuntimeTree(runtimeCells, horizontalSeparator, verticalSeparator);
        resolvedCells.Clear();
        ResolveRuntimeNode(
            runtimeTree,
            LayoutContentRect(runtimeTree, content, mode),
            resolvedCells,
            horizontalSeparator,
            verticalSeparator,
            mode);
        if (!RecomputeFitContentHeightsPass2(window, runtimeCells, resolvedCells))
        {
            return;
        }
        runtimeTree = BuildRuntimeTree(runtimeCells, horizontalSeparator, verticalSeparator);
        resolvedCells.Clear();
        ResolveRuntimeNode(
            runtimeTree,
            LayoutContentRect(runtimeTree, content, mode),
            resolvedCells,
            horizontalSeparator,
            verticalSeparator,
            mode);
    }

    private static void ResolveRuntimeNode(
        RuntimeNode? node,
        Rect rect,
        List<ResolvedRuntimeCell> resolved,
        int horizontalSeparator,
        int verticalSeparator,
        GeneratedWindowMode mode)
    {
        if (node is null)
        {
            return;
        }
        rect = new Rect(
            rect.Row,
            rect.Col,
            Math.Max(MinimumRenderableSize, rect.Width),
            Math.Max(MinimumRenderableSize, rect.Height));
        if (node.Children.Count == 0)
        {
            if (node.Cell is not null)
            {
                resolved.Add(new ResolvedRuntimeCell(node.Cell, rect));
            }
            return;
        }

        if (node.Orientation == "vertical")
        {
            int fixedTotal = 0;
            int expandedCount = 0;
            foreach (RuntimeNode child in node.Children)
            {
                if (child.LogicalHeight.HasValue)
                {
                    fixedTotal += child.LogicalHeight.Value;
                }
                else
                {
                    ++expandedCount;
                }
            }
            int separators = Math.Max(0, node.Children.Count - 1) * horizontalSeparator;
            int remaining = Math.Max(0, rect.Height - fixedTotal - separators);
            bool distributeFixedExtra = expandedCount == 0 && remaining > 0 && mode == GeneratedWindowMode.Fullscreen;
            bool appendTrailingExtra = expandedCount == 0 && remaining > 0 && mode != GeneratedWindowMode.Fullscreen && horizontalSeparator > 0;
            int expandTargets = distributeFixedExtra ? node.Children.Count : expandedCount;
            int share = expandTargets > 0 ? remaining / expandTargets : 0;
            int remainder = expandTargets > 0 ? remaining % expandTargets : 0;
            int expandIndex = 0;
            int cursor = rect.Row;
            for (int index = 0; index < node.Children.Count; ++index)
            {
                RuntimeNode child = node.Children[index];
                int childHeight = child.LogicalHeight ?? 0;
                if (!child.LogicalHeight.HasValue || distributeFixedExtra)
                {
                    childHeight = (distributeFixedExtra ? childHeight : 0) + share + (expandIndex < remainder ? 1 : 0);
                    ++expandIndex;
                }
                else if (appendTrailingExtra && index + 1 == node.Children.Count)
                {
                    childHeight += remaining;
                }
                ResolveRuntimeNode(
                    child,
                    new Rect(cursor, rect.Col, rect.Width, Math.Max(MinimumRenderableSize, childHeight)),
                    resolved,
                    horizontalSeparator,
                    verticalSeparator,
                    mode);
                cursor += Math.Max(MinimumRenderableSize, childHeight) + horizontalSeparator;
            }
            return;
        }

        int horizontalFixedTotal = 0;
        int horizontalExpandedCount = 0;
        foreach (RuntimeNode child in node.Children)
        {
            if (child.LogicalWidth.HasValue)
            {
                horizontalFixedTotal += child.LogicalWidth.Value;
            }
            else
            {
                ++horizontalExpandedCount;
            }
        }
        int horizontalSeparators = Math.Max(0, node.Children.Count - 1) * verticalSeparator;
        int horizontalRemaining = Math.Max(0, rect.Width - horizontalFixedTotal - horizontalSeparators);
        bool horizontalDistributeFixedExtra = horizontalExpandedCount == 0 && horizontalRemaining > 0 && mode == GeneratedWindowMode.Fullscreen;
        bool horizontalAppendTrailingExtra = horizontalExpandedCount == 0 && horizontalRemaining > 0 && mode != GeneratedWindowMode.Fullscreen;
        int horizontalExpandTargets = horizontalDistributeFixedExtra ? node.Children.Count : horizontalExpandedCount;
        int horizontalShare = horizontalExpandTargets > 0 ? horizontalRemaining / horizontalExpandTargets : 0;
        int horizontalRemainder = horizontalExpandTargets > 0 ? horizontalRemaining % horizontalExpandTargets : 0;
        int horizontalExpandIndex = 0;
        int horizontalCursor = rect.Col;
        for (int index = 0; index < node.Children.Count; ++index)
        {
            RuntimeNode child = node.Children[index];
            int childWidth = child.LogicalWidth ?? 0;
            if (!child.LogicalWidth.HasValue || horizontalDistributeFixedExtra)
            {
                childWidth = (horizontalDistributeFixedExtra ? childWidth : 0) + horizontalShare + (horizontalExpandIndex < horizontalRemainder ? 1 : 0);
                ++horizontalExpandIndex;
            }
            else if (horizontalAppendTrailingExtra && index + 1 == node.Children.Count)
            {
                childWidth += horizontalRemaining;
            }
            ResolveRuntimeNode(
                child,
                new Rect(rect.Row, horizontalCursor, Math.Max(MinimumRenderableSize, childWidth), rect.Height),
                resolved,
                horizontalSeparator,
                verticalSeparator,
                mode);
            horizontalCursor += Math.Max(MinimumRenderableSize, childWidth) + verticalSeparator;
        }
    }

    private static Rect LayoutContentRect(RuntimeNode? runtimeTree, Rect content, GeneratedWindowMode mode)
    {
        Size naturalContent = LayoutSize(runtimeTree);
        Rect layoutContent = content;
        if (mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandWidth)
        {
            layoutContent = new Rect(layoutContent.Row, layoutContent.Col, Math.Max(layoutContent.Width, naturalContent.Width), layoutContent.Height);
        }
        if (mode is GeneratedWindowMode.Fullscreen or GeneratedWindowMode.ExpandHeight)
        {
            layoutContent = new Rect(layoutContent.Row, layoutContent.Col, layoutContent.Width, Math.Max(layoutContent.Height, naturalContent.Height));
        }
        return layoutContent;
    }

    private static Size LayoutSize(RuntimeNode? root)
    {
        if (root is null)
        {
            return new Size(MinimumRenderableSize, MinimumRenderableSize);
        }
        int sourceWidth = Math.Max(MinimumRenderableSize, root.Right - root.Left - 1);
        int sourceHeight = Math.Max(MinimumRenderableSize, root.Bottom - root.Top - 1);
        int width = Math.Max(MinimumRenderableSize, root.LogicalWidth ?? sourceWidth);
        int height = Math.Max(MinimumRenderableSize, root.LogicalHeight ?? sourceHeight);
        return new Size(width, height);
    }

    private static Size ResolvedContentExtent(IReadOnlyList<ResolvedRuntimeCell> resolvedCells, Size fallback)
    {
        Size result = new(
            Math.Max(MinimumRenderableSize, fallback.Width),
            Math.Max(MinimumRenderableSize, fallback.Height));
        foreach (ResolvedRuntimeCell cell in resolvedCells)
        {
            result = new Size(
                Math.Max(result.Width, cell.Rect.Col + cell.Rect.Width),
                Math.Max(result.Height, cell.Rect.Row + cell.Rect.Height));
        }
        return result;
    }

    private static List<RuntimeCell> CollectRuntimeCells(GeneratedWindowBase window)
    {
        List<RuntimeCell> cells = new();
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            if (cells.Any(cell => SameRuntimeCell(cell, entry)))
            {
                continue;
            }
            int? logicalWidth = LogicalCellSize(entry.CellWidth, entry.SourceCell.Width);
            int? logicalHeight = LogicalCellSize(entry.CellHeight, entry.SourceCell.Height);
            if (entry.CellWidth.Mode == DimensionMode.FitContent)
            {
                logicalWidth = FitCellWidth(window, entry);
            }
            if (entry.CellHeight.Mode == DimensionMode.FitContent)
            {
                logicalHeight = FitCellHeight(window, entry);
            }
            if (logicalWidth.HasValue &&
                entry.CellWidth.Mode != DimensionMode.Expanded &&
                entry.CellWidth.Mode != DimensionMode.FitContent)
            {
                logicalWidth = Math.Max(MinimumRenderableSize, logicalWidth.Value + PaddingLeft(entry.CellStyle) + PaddingRight(entry.CellStyle));
            }
            if (logicalHeight.HasValue &&
                entry.CellHeight.Mode != DimensionMode.Expanded &&
                entry.CellHeight.Mode != DimensionMode.FitContent)
            {
                logicalHeight = Math.Max(MinimumRenderableSize, logicalHeight.Value + PaddingTop(entry.CellStyle) + PaddingBottom(entry.CellStyle));
            }
            cells.Add(new RuntimeCell
            {
                SourceRow = entry.SourceCell.Row,
                SourceCol = entry.SourceCell.Col,
                SourceWidth = Math.Max(MinimumRenderableSize, entry.SourceCell.Width),
                SourceHeight = Math.Max(MinimumRenderableSize, entry.SourceCell.Height),
                LogicalWidth = logicalWidth,
                LogicalHeight = logicalHeight,
            });
        }
        return cells;
    }

    private static RuntimeNode MakeLeafNode(RuntimeCell cell)
    {
        return new RuntimeNode
        {
            Cell = cell,
            Left = cell.SourceCol,
            Top = cell.SourceRow,
            Right = cell.SourceCol + cell.SourceWidth + 1,
            Bottom = cell.SourceRow + cell.SourceHeight + 1,
            LogicalWidth = cell.LogicalWidth,
            LogicalHeight = cell.LogicalHeight,
        };
    }

    private static RuntimeNode MakeGroupNode(
        string orientation,
        List<RuntimeNode> children,
        int left,
        int top,
        int right,
        int bottom,
        int horizontalSeparator,
        int verticalSeparator)
    {
        RuntimeNode node = new()
        {
            Orientation = orientation,
            Left = left,
            Top = top,
            Right = right,
            Bottom = bottom,
        };
        node.Children.AddRange(children);

        if (node.Orientation == "vertical")
        {
            foreach (RuntimeNode child in node.Children)
            {
                if (!child.LogicalWidth.HasValue)
                {
                    node.LogicalWidth = null;
                    break;
                }
                node.LogicalWidth = Math.Max(node.LogicalWidth ?? MinimumRenderableSize, child.LogicalWidth.Value);
            }
            int height = 0;
            foreach (RuntimeNode child in node.Children)
            {
                if (!child.LogicalHeight.HasValue)
                {
                    node.LogicalHeight = null;
                    return node;
                }
                height += child.LogicalHeight.Value;
            }
            height += Math.Max(0, node.Children.Count - 1) * horizontalSeparator;
            node.LogicalHeight = height;
        }
        else
        {
            foreach (RuntimeNode child in node.Children)
            {
                if (!child.LogicalHeight.HasValue)
                {
                    node.LogicalHeight = null;
                    break;
                }
                node.LogicalHeight = Math.Max(node.LogicalHeight ?? MinimumRenderableSize, child.LogicalHeight.Value);
            }
            int width = 0;
            foreach (RuntimeNode child in node.Children)
            {
                if (!child.LogicalWidth.HasValue)
                {
                    node.LogicalWidth = null;
                    return node;
                }
                width += child.LogicalWidth.Value;
            }
            width += Math.Max(0, node.Children.Count - 1) * verticalSeparator;
            node.LogicalWidth = width;
        }
        return node;
    }

    private static RuntimeNode? BuildRuntimeTree(List<RuntimeCell> cells, int horizontalSeparator, int verticalSeparator)
    {
        return BuildRuntimeTree(cells.Select(MakeLeafNode).ToList(), horizontalSeparator, verticalSeparator);
    }

    private static RuntimeNode? BuildRuntimeTree(List<RuntimeNode> nodes, int horizontalSeparator, int verticalSeparator)
    {
        if (nodes.Count == 0)
        {
            return null;
        }
        if (nodes.Count == 1)
        {
            return nodes[0];
        }
        nodes.Sort((lhs, rhs) =>
        {
            int result = lhs.Top.CompareTo(rhs.Top);
            if (result != 0)
            {
                return result;
            }
            result = lhs.Left.CompareTo(rhs.Left);
            if (result != 0)
            {
                return result;
            }
            result = lhs.Bottom.CompareTo(rhs.Bottom);
            return result != 0 ? result : lhs.Right.CompareTo(rhs.Right);
        });

        int left = nodes.Min(node => node.Left);
        int top = nodes.Min(node => node.Top);
        int right = nodes.Max(node => node.Right);
        int bottom = nodes.Max(node => node.Bottom);

        List<int> cuts = HorizontalCuts(nodes, left, top, right, bottom);
        if (cuts.Count > 0)
        {
            List<int> boundaries = new() { top };
            boundaries.AddRange(cuts);
            boundaries.Add(bottom);
            List<List<RuntimeNode>> groups = SliceByRows(nodes, boundaries);
            if (groups.Count > 1)
            {
                List<RuntimeNode> children = new();
                foreach (List<RuntimeNode> group in groups)
                {
                    RuntimeNode? child = BuildRuntimeTree(group, horizontalSeparator, verticalSeparator);
                    if (child is not null)
                    {
                        children.Add(child);
                    }
                }
                return MakeGroupNode("vertical", children, left, top, right, bottom, horizontalSeparator, verticalSeparator);
            }
        }

        cuts = VerticalCuts(nodes, left, top, right, bottom);
        if (cuts.Count > 0)
        {
            List<int> boundaries = new() { left };
            boundaries.AddRange(cuts);
            boundaries.Add(right);
            List<List<RuntimeNode>> groups = SliceByCols(nodes, boundaries);
            if (groups.Count > 1)
            {
                List<RuntimeNode> children = new();
                foreach (List<RuntimeNode> group in groups)
                {
                    RuntimeNode? child = BuildRuntimeTree(group, horizontalSeparator, verticalSeparator);
                    if (child is not null)
                    {
                        children.Add(child);
                    }
                }
                return MakeGroupNode("horizontal", children, left, top, right, bottom, horizontalSeparator, verticalSeparator);
            }
        }

        return nodes[0];
    }

    private static List<int> HorizontalCuts(IReadOnlyList<RuntimeNode> nodes, int left, int top, int right, int bottom)
    {
        List<int> candidates = new();
        foreach (RuntimeNode node in nodes)
        {
            candidates.Add(node.Top);
            candidates.Add(node.Bottom);
        }
        candidates = candidates.Distinct().OrderBy(value => value).ToList();

        List<int> cuts = new();
        foreach (int candidate in candidates)
        {
            if (candidate <= top || candidate >= bottom)
            {
                continue;
            }
            List<(int Start, int End)> spans = new();
            foreach (RuntimeNode node in nodes)
            {
                if (node.Top == candidate || node.Bottom == candidate)
                {
                    spans.Add((node.Left, node.Right));
                }
            }
            if (IntervalsCover(spans, left, right))
            {
                cuts.Add(candidate);
            }
        }
        return cuts;
    }

    private static List<int> VerticalCuts(IReadOnlyList<RuntimeNode> nodes, int left, int top, int right, int bottom)
    {
        List<int> candidates = new();
        foreach (RuntimeNode node in nodes)
        {
            candidates.Add(node.Left);
            candidates.Add(node.Right);
        }
        candidates = candidates.Distinct().OrderBy(value => value).ToList();

        List<int> cuts = new();
        foreach (int candidate in candidates)
        {
            if (candidate <= left || candidate >= right)
            {
                continue;
            }
            List<(int Start, int End)> spans = new();
            foreach (RuntimeNode node in nodes)
            {
                if (node.Left == candidate || node.Right == candidate)
                {
                    spans.Add((node.Top, node.Bottom));
                }
            }
            if (IntervalsCover(spans, top, bottom))
            {
                cuts.Add(candidate);
            }
        }
        return cuts;
    }

    private static bool IntervalsCover(List<(int Start, int End)> spans, int start, int end)
    {
        if (spans.Count == 0)
        {
            return false;
        }
        spans.Sort();
        int cursor = start;
        foreach ((int spanStart, int spanEnd) in spans)
        {
            if (spanStart > cursor)
            {
                return false;
            }
            cursor = Math.Max(cursor, spanEnd);
            if (cursor >= end)
            {
                return true;
            }
        }
        return cursor >= end;
    }

    private static List<List<RuntimeNode>> SliceByRows(IReadOnlyList<RuntimeNode> nodes, IReadOnlyList<int> boundaries)
    {
        List<List<RuntimeNode>> groups = new();
        bool[] used = new bool[nodes.Count];
        for (int index = 0; index + 1 < boundaries.Count; ++index)
        {
            int bandTop = boundaries[index];
            int bandBottom = boundaries[index + 1];
            List<RuntimeNode> group = new();
            for (int nodeIndex = 0; nodeIndex < nodes.Count; ++nodeIndex)
            {
                if (nodes[nodeIndex].Top >= bandTop && nodes[nodeIndex].Bottom <= bandBottom)
                {
                    group.Add(nodes[nodeIndex]);
                    used[nodeIndex] = true;
                }
            }
            if (group.Count == 0)
            {
                return new List<List<RuntimeNode>>();
            }
            group.Sort((lhs, rhs) =>
            {
                int result = lhs.Top.CompareTo(rhs.Top);
                return result != 0 ? result : lhs.Left.CompareTo(rhs.Left);
            });
            groups.Add(group);
        }
        return used.All(value => value) ? groups : new List<List<RuntimeNode>>();
    }

    private static List<List<RuntimeNode>> SliceByCols(IReadOnlyList<RuntimeNode> nodes, IReadOnlyList<int> boundaries)
    {
        List<List<RuntimeNode>> groups = new();
        bool[] used = new bool[nodes.Count];
        for (int index = 0; index + 1 < boundaries.Count; ++index)
        {
            int bandLeft = boundaries[index];
            int bandRight = boundaries[index + 1];
            List<RuntimeNode> group = new();
            for (int nodeIndex = 0; nodeIndex < nodes.Count; ++nodeIndex)
            {
                if (nodes[nodeIndex].Left >= bandLeft && nodes[nodeIndex].Right <= bandRight)
                {
                    group.Add(nodes[nodeIndex]);
                    used[nodeIndex] = true;
                }
            }
            if (group.Count == 0)
            {
                return new List<List<RuntimeNode>>();
            }
            group.Sort((lhs, rhs) =>
            {
                int result = lhs.Left.CompareTo(rhs.Left);
                return result != 0 ? result : lhs.Top.CompareTo(rhs.Top);
            });
            groups.Add(group);
        }
        return used.All(value => value) ? groups : new List<List<RuntimeNode>>();
    }

    private static int? LogicalCellSize(AxisDimension dimension, int sourceSize)
    {
        return dimension.Mode switch
        {
            DimensionMode.Expanded => null,
            DimensionMode.Fixed => Math.Max(MinimumRenderableSize, dimension.Value),
            _ => Math.Max(MinimumRenderableSize, dimension.Value > 0 ? dimension.Value : sourceSize),
        };
    }

    private static bool RecomputeFitContentHeightsPass2(
        GeneratedWindowBase window,
        List<RuntimeCell> cells,
        IReadOnlyList<ResolvedRuntimeCell> resolvedCells)
    {
        bool changed = false;
        foreach (RuntimeCell runtimeCell in cells)
        {
            GeneratedLayoutEntry? cellEntry = null;
            foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
            {
                if (SameRuntimeCell(runtimeCell, entry) && entry.CellHeight.Mode == DimensionMode.FitContent)
                {
                    cellEntry = entry;
                    break;
                }
            }
            if (cellEntry is null)
            {
                continue;
            }

            ResolvedRuntimeCell? resolved = ResolvedCellFor(resolvedCells, cellEntry);
            if (resolved is null)
            {
                continue;
            }

            int newHeight = FitCellHeight(window, cellEntry, resolved.Rect.Width);
            if (!runtimeCell.LogicalHeight.HasValue || runtimeCell.LogicalHeight.Value != newHeight)
            {
                runtimeCell.LogicalHeight = newHeight;
                changed = true;
            }
        }
        return changed;
    }

    private static int FitCellWidth(GeneratedWindowBase window, GeneratedLayoutEntry cellEntry)
    {
        int contentWidth = cellEntry.SourceCell.Width;
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            if (SameSourceCell(entry, cellEntry))
            {
                contentWidth = Math.Max(contentWidth, FitEntryWidth(window, entry));
            }
        }
        return Math.Max(MinimumRenderableSize, contentWidth + PaddingLeft(cellEntry.CellStyle) + PaddingRight(cellEntry.CellStyle));
    }

    private static int FitEntryWidth(GeneratedWindowBase window, GeneratedLayoutEntry entry)
    {
        Element? element = FindElement(window, entry.Name);
        int fallback = entry.CharsSize.Width > 0 ? entry.CharsSize.Width : entry.SourceCell.Width;
        int contentWidth = fallback;
        if (element is ListBox listBox)
        {
            foreach (string option in listBox.Options)
            {
                contentWidth = Math.Max(contentWidth, option.Length);
            }
        }
        else if (element is Label label)
        {
            int lineWidth = 0;
            foreach (char ch in label.Text)
            {
                if (ch == '\n')
                {
                    contentWidth = Math.Max(contentWidth, lineWidth);
                    lineWidth = 0;
                }
                else if (ch != '\r')
                {
                    ++lineWidth;
                }
            }
            contentWidth = Math.Max(contentWidth, lineWidth);
        }
        return Math.Max(MinimumRenderableSize, entry.Relative.Col + contentWidth + entry.MarginRight);
    }

    private static int FitCellHeight(GeneratedWindowBase window, GeneratedLayoutEntry cellEntry, int? resolvedCellWidth = null)
    {
        int contentHeight = cellEntry.SourceCell.Height;
        int? availableCellWidth = null;
        if (resolvedCellWidth.HasValue)
        {
            availableCellWidth = Math.Max(
                MinimumRenderableSize,
                resolvedCellWidth.Value - PaddingLeft(cellEntry.CellStyle) - PaddingRight(cellEntry.CellStyle));
        }
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            if (SameSourceCell(entry, cellEntry))
            {
                contentHeight = Math.Max(contentHeight, FitEntryHeight(window, entry, availableCellWidth));
            }
        }
        return Math.Max(MinimumRenderableSize, contentHeight + PaddingTop(cellEntry.CellStyle) + PaddingBottom(cellEntry.CellStyle));
    }

    private static int FitEntryHeight(GeneratedWindowBase window, GeneratedLayoutEntry entry, int? availableCellWidth = null)
    {
        Element? element = FindElement(window, entry.Name);
        int fallback = entry.CharsSize.Height > 0 ? entry.CharsSize.Height : entry.SourceCell.Height;
        int? contentWidth = null;
        if (availableCellWidth.HasValue)
        {
            contentWidth = Math.Max(MinimumRenderableSize, availableCellWidth.Value - entry.Relative.Col - entry.MarginRight);
        }
        int contentHeight = entry.Height.Mode == DimensionMode.FitContent
            ? FitContentHeightFor(element, entry.Type, fallback, contentWidth)
            : Math.Max(MinimumRenderableSize, fallback);
        return Math.Max(MinimumRenderableSize, entry.Relative.Row + contentHeight + entry.MarginBottom);
    }

    private static int FitContentHeightFor(Element? element, string type, int fallback, int? width = null)
    {
        if (element is null)
        {
            return Math.Max(MinimumRenderableSize, fallback);
        }
        if (element is ListBox listBox && listBox.Options.Count > 0)
        {
            return Math.Max(MinimumRenderableSize, listBox.Options.Count);
        }
        if (element is MessageTable messageTable)
        {
            return Math.Max(MinimumRenderableSize, messageTable.FitContentHeight());
        }
        if (element is Label label)
        {
            if (width.HasValue && type != "spanlabel" && type != "infolabel")
            {
                return WrappedTextLineCount(label.Text, width.Value);
            }
            int lines = TextLineCount(label.Text);
            if (lines > 0)
            {
                return Math.Max(MinimumRenderableSize, lines);
            }
            return MinimumRenderableSize;
        }
        return Math.Max(MinimumRenderableSize, fallback);
    }

    private static int TextLineCount(string text)
    {
        if (string.IsNullOrEmpty(text))
        {
            return 0;
        }
        int lines = 1;
        foreach (char ch in text)
        {
            if (ch == '\n')
            {
                ++lines;
            }
        }
        return lines;
    }

    private static int WrappedTextLineCount(string text, int width)
    {
        width = Math.Max(MinimumRenderableSize, width);
        if (string.IsNullOrEmpty(text))
        {
            return MinimumRenderableSize;
        }
        int rows = 0;
        int segmentStart = 0;
        while (segmentStart <= text.Length)
        {
            int newline = text.IndexOf('\n', segmentStart);
            int segmentEnd = newline < 0 ? text.Length : newline;
            string segment = text[segmentStart..segmentEnd];
            if (segment.Length == 0)
            {
                ++rows;
            }
            else
            {
                int offset = 0;
                while (offset < segment.Length)
                {
                    int remaining = segment.Length - offset;
                    int chunkSize = Math.Min(remaining, width);
                    string chunk = segment.Substring(offset, chunkSize);
                    ++rows;
                    if (chunkSize < width)
                    {
                        offset += chunkSize;
                        continue;
                    }
                    int lastSpace = chunk.LastIndexOf(' ');
                    if (lastSpace > 0)
                    {
                        offset += lastSpace + 1;
                    }
                    else
                    {
                        offset += chunkSize;
                    }
                }
            }
            if (newline < 0)
            {
                break;
            }
            segmentStart = newline + 1;
        }
        return Math.Max(MinimumRenderableSize, rows);
    }

    private static bool SameRuntimeCell(RuntimeCell cell, GeneratedLayoutEntry entry)
    {
        return cell.SourceRow == entry.SourceCell.Row &&
            cell.SourceCol == entry.SourceCell.Col &&
            cell.SourceWidth == entry.SourceCell.Width &&
            cell.SourceHeight == entry.SourceCell.Height;
    }

    private static bool SameSourceCell(GeneratedLayoutEntry lhs, GeneratedLayoutEntry rhs)
    {
        return lhs.SourceCell.Row == rhs.SourceCell.Row &&
            lhs.SourceCell.Col == rhs.SourceCell.Col &&
            lhs.SourceCell.Width == rhs.SourceCell.Width &&
            lhs.SourceCell.Height == rhs.SourceCell.Height;
    }

    private static Size RenderSizeFor(GeneratedLayoutEntry entry, Rect cellRect, Element? element)
    {
        int availableWidth = cellRect.Width - PaddingLeft(entry.CellStyle) - PaddingRight(entry.CellStyle) - entry.Relative.Col - entry.MarginRight;
        int availableHeight = cellRect.Height - PaddingTop(entry.CellStyle) - PaddingBottom(entry.CellStyle) - entry.Relative.Row - entry.MarginBottom;
        int fallbackWidth = entry.CharsSize.Width > 0 ? entry.CharsSize.Width : entry.SourceCell.Width;
        int fallbackHeight = entry.CharsSize.Height > 0 ? entry.CharsSize.Height : entry.SourceCell.Height;
        if (entry.Height.Mode == DimensionMode.FitContent)
        {
            fallbackHeight = FitContentHeightFor(element, entry.Type, fallbackHeight, Math.Max(MinimumRenderableSize, availableWidth));
        }
        if (element is null && entry.Type == "label")
        {
            int availableStaticWidth = cellRect.Width - PaddingLeft(entry.CellStyle) - entry.Relative.Col;
            return new Size(
                Math.Max(
                    MinimumRenderableSize,
                    Math.Min(Math.Max(MinimumRenderableSize, fallbackWidth), Math.Max(MinimumRenderableSize, availableStaticWidth))),
                DimensionWithin(entry.Height, availableHeight, fallbackHeight));
        }
        return new Size(
            DimensionWithin(entry.Width, availableWidth, fallbackWidth),
            DimensionWithin(entry.Height, availableHeight, fallbackHeight));
    }

    private static int DimensionWithin(AxisDimension dimension, int available, int fallback)
    {
        available = Math.Max(1, available);
        if (dimension.Mode == DimensionMode.Expanded)
        {
            return available;
        }
        if (dimension.Mode == DimensionMode.FitContent)
        {
            return Math.Max(1, Math.Min(Math.Max(1, fallback), available));
        }
        int requested = dimension.Value > 0 ? dimension.Value : fallback;
        return Math.Max(1, requested);
    }

    private static int RenderColFor(IReadOnlyList<GeneratedLayoutEntry> layout, GeneratedLayoutEntry entry, Rect cellRect, Size size)
    {
        int baseCol = PaddingLeft(entry.CellStyle) + entry.Relative.Col;
        int cellWidthDelta = Math.Max(0, cellRect.Width - entry.CellCharsSize.Width);
        bool followsExpanded = layout.Any(other =>
            SameSourceCell(other, entry) &&
            other.Relative.Row == entry.Relative.Row &&
            other.Relative.Col < entry.Relative.Col &&
            other.Width.Mode == DimensionMode.Expanded);
        if (entry.MarginRight == 0 &&
            entry.CellWidth.Mode == DimensionMode.Expanded &&
            entry.CellCharsSize.Width > 0 &&
            cellRect.Width > entry.CellCharsSize.Width &&
            entry.Width.Mode != DimensionMode.Expanded)
        {
            return cellRect.Col + Math.Max(baseCol, cellRect.Width - size.Width - entry.MarginRight - PaddingRight(entry.CellStyle));
        }
        if (followsExpanded &&
            entry.CellWidth.Mode == DimensionMode.Expanded &&
            entry.CellCharsSize.Width > 0 &&
            cellWidthDelta > 0 &&
            entry.Width.Mode != DimensionMode.Expanded)
        {
            return cellRect.Col + Math.Max(baseCol, baseCol + cellWidthDelta);
        }
        return cellRect.Col + baseCol;
    }

    private static int RenderRowFor(IReadOnlyList<GeneratedLayoutEntry> layout, GeneratedLayoutEntry entry, Rect cellRect, Size size)
    {
        int baseRow = PaddingTop(entry.CellStyle) + entry.Relative.Row;
        int cellHeightDelta = Math.Max(0, cellRect.Height - entry.CellCharsSize.Height);
        bool followsExpanded = layout.Any(other =>
            SameSourceCell(other, entry) &&
            other.Relative.Row < entry.Relative.Row &&
            SourceSpansOverlap(EntrySourceColSpan(other), EntrySourceColSpan(entry)) &&
            other.Height.Mode == DimensionMode.Expanded);
        if (followsExpanded &&
            entry.CellHeight.Mode == DimensionMode.Expanded &&
            entry.CellCharsSize.Height > 0 &&
            cellHeightDelta > 0 &&
            entry.Height.Mode != DimensionMode.Expanded)
        {
            return cellRect.Row + Math.Max(baseRow, baseRow + cellHeightDelta);
        }
        return cellRect.Row + baseRow;
    }

    private static (int Start, int End) EntrySourceColSpan(GeneratedLayoutEntry entry)
    {
        int width = Math.Max(1, entry.CharsSize.Width > 0 ? entry.CharsSize.Width : entry.SourceCell.Width);
        return (entry.Relative.Col, entry.Relative.Col + width);
    }

    private static bool SourceSpansOverlap((int Start, int End) first, (int Start, int End) second)
    {
        return first.Start < second.End && second.Start < first.End;
    }

    private static int PaddingTop(Style style)
    {
        return Math.Max(0, style.PaddingTop ?? style.Padding ?? 0);
    }

    private static int PaddingRight(Style style)
    {
        return Math.Max(0, style.PaddingRight ?? style.Padding ?? 0);
    }

    private static int PaddingBottom(Style style)
    {
        return Math.Max(0, style.PaddingBottom ?? style.Padding ?? 0);
    }

    private static int PaddingLeft(Style style)
    {
        return Math.Max(0, style.PaddingLeft ?? style.Padding ?? 0);
    }

    private static int BorderWidthHorizontal(Style style)
    {
        return Math.Max(0, style.BorderWidthHorizontal ?? 0);
    }

    private static int BorderWidthVertical(Style style)
    {
        return Math.Max(0, style.BorderWidthVertical ?? 0);
    }

    internal static void SyncWindowElementFramesTo(
        GeneratedWindowBase window,
        Rect frame,
        bool forceFullscreenLayout = false)
    {
        Size size = new(Math.Max(1, frame.Width), Math.Max(1, frame.Height));
        List<ResolvedRuntimeCell> resolvedCells = ResolveRuntimeCells(
            window,
            size,
            forceFullscreenLayout ? GeneratedWindowMode.Fullscreen : WindowMode(window));
        foreach (GeneratedLayoutEntry entry in window.GeneratedLayout)
        {
            Element? element = FindElement(window, entry.Name);
            ResolvedRuntimeCell? cell = ResolvedCellFor(resolvedCells, entry);
            if (element is null || cell is null)
            {
                continue;
            }
            Size elementSize = RenderSizeFor(entry, cell.Rect, element);
            int row = frame.Row + RenderRowFor(window.GeneratedLayout, entry, cell.Rect, elementSize);
            int col = frame.Col + RenderColFor(window.GeneratedLayout, entry, cell.Rect, elementSize);
            element.Frame = new Rect(row, col, elementSize.Width, elementSize.Height);
            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                SyncReusableChildFrames(reusable, element.Frame);
                continue;
            }
            if (element is ScrollView scrollView)
            {
                Rect scrollFrame = scrollView.Frame;
                foreach (ScrollViewChildView childView in scrollView.ChildViews(new Size(scrollFrame.Width, scrollFrame.Height)))
                {
                    if (childView.Element is null)
                    {
                        continue;
                    }
                    if (!childView.Visible)
                    {
                        childView.Element.Frame = new Rect(0, 0, 0, 0);
                        continue;
                    }
                    childView.Element.Frame = new Rect(
                        scrollFrame.Row + childView.Frame.Row,
                        scrollFrame.Col + childView.Frame.Col,
                        childView.Frame.Width,
                        childView.Frame.Height);
                    if (childView.Element is ReusableElement childReusable && childReusable.Child is not null)
                    {
                        SyncReusableChildFrames(childReusable, childView.Element.Frame);
                    }
                }
                continue;
            }
            SyncElementChildFrames(element);
        }
    }

    private static void SyncReusableChildFrames(ReusableElement reusable, Rect frame)
    {
        if (reusable.Child is null)
        {
            return;
        }
        SyncWindowElementFramesTo(reusable.Child, frame, true);
    }

    private static void SyncElementChildFrames(Element element)
    {
        foreach (Element child in element.Children)
        {
            if (child.Frame.Width <= 0 || child.Frame.Height <= 0)
            {
                child.Frame = new Rect(
                    child.Frame.Row,
                    child.Frame.Col,
                    child.Frame.Width <= 0 ? element.Frame.Width : child.Frame.Width,
                    child.Frame.Height <= 0 ? element.Frame.Height : child.Frame.Height);
            }
            SyncElementChildFrames(child);
        }
    }

    internal static void OffsetWindowElementFrames(GeneratedWindowBase window, int rowOffset, int colOffset)
    {
        if (rowOffset == 0 && colOffset == 0)
        {
            return;
        }
        foreach (Element element in window.Elements)
        {
            element.Frame = new Rect(
                element.Frame.Row + rowOffset,
                element.Frame.Col + colOffset,
                element.Frame.Width,
                element.Frame.Height);
            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                OffsetWindowElementFrames(reusable.Child, rowOffset, colOffset);
            }
            OffsetElementChildFrames(element, rowOffset, colOffset);
        }
    }

    private static void OffsetElementChildFrames(Element element, int rowOffset, int colOffset)
    {
        foreach (Element child in element.Children)
        {
            child.Frame = new Rect(
                child.Frame.Row + rowOffset,
                child.Frame.Col + colOffset,
                child.Frame.Width,
                child.Frame.Height);
            if (child is ReusableElement reusable && reusable.Child is not null)
            {
                OffsetWindowElementFrames(reusable.Child, rowOffset, colOffset);
            }
            OffsetElementChildFrames(child, rowOffset, colOffset);
        }
    }

    internal static bool EnsureElementVisibleInContainingScrollView(GeneratedWindowBase window, Element target)
    {
        foreach (Element element in window.Elements)
        {
            if (EnsureElementVisibleInElement(element, target))
            {
                return true;
            }
            if (element is ReusableElement reusable && reusable.Child is not null &&
                EnsureElementVisibleInContainingScrollView(reusable.Child, target))
            {
                return true;
            }
        }
        return false;
    }

    private static bool EnsureElementVisibleInElement(Element element, Element target)
    {
        if (element is ScrollView scrollView && EnsureElementVisibleInScrollView(scrollView, target))
        {
            return true;
        }
        foreach (Element child in element.Children)
        {
            if (EnsureElementVisibleInElement(child, target))
            {
                return true;
            }
            if (child is ReusableElement reusable && reusable.Child is not null &&
                EnsureElementVisibleInContainingScrollView(reusable.Child, target))
            {
                return true;
            }
        }
        return false;
    }

    private static bool EnsureElementVisibleInScrollView(ScrollView scrollView, Element target)
    {
        Rect viewport = ScrollViewViewportClip(scrollView.Frame, scrollView.Style);
        if (viewport.Width <= 0 || viewport.Height <= 0)
        {
            return false;
        }

        Rect scrollFrame = scrollView.Frame;
        foreach (ScrollViewChildView childView in scrollView.ChildViews(new Size(scrollFrame.Width, scrollFrame.Height)))
        {
            if (childView.Element is null)
            {
                continue;
            }

            Rect? targetRect = FocusRectWithinScrollChild(
                childView.Element,
                target,
                childView.Frame.Width,
                childView.Frame.Height);
            if (targetRect is not null)
            {
                int targetTop = scrollFrame.Row + childView.Frame.Row + targetRect.Value.Row;
                int targetBottom = targetTop + Math.Max(MinimumRenderableSize, targetRect.Value.Height);
                int delta = 0;
                if (targetTop < viewport.Row)
                {
                    delta = targetTop - viewport.Row;
                }
                else if (targetBottom > viewport.Row + viewport.Height)
                {
                    delta = targetBottom - viewport.Row - viewport.Height;
                }
                return ScrollViewByLines(scrollView, delta);
            }
        }

        return false;
    }

    private static bool ScrollViewByLines(ScrollView scrollView, int delta)
    {
        if (delta == 0)
        {
            return false;
        }

        bool changed = false;
        string key = delta < 0 ? "Up" : "Down";
        int steps = Math.Abs(delta);
        for (int step = 0; step < steps; ++step)
        {
            changed = scrollView.HandleKey(key) || changed;
        }
        return changed;
    }

    internal static Rect? FocusRectWithinScrollChild(Element child, Element target, int width, int height)
    {
        if (ReferenceEquals(child, target))
        {
            return new Rect(0, 0, Math.Max(MinimumRenderableSize, child.Frame.Width), Math.Max(MinimumRenderableSize, height));
        }
        if (child is ReusableElement reusable && reusable.Child is not null &&
            WindowContainsElement(reusable.Child, target))
        {
            SyncReusableChildFrames(
                reusable,
                new Rect(0, 0, Math.Max(MinimumRenderableSize, width), Math.Max(MinimumRenderableSize, height)));
            return new Rect(
                target.Frame.Row,
                target.Frame.Col,
                Math.Max(MinimumRenderableSize, target.Frame.Width),
                Math.Max(MinimumRenderableSize, target.Frame.Height));
        }
        if (ElementTreeContains(child, target))
        {
            return new Rect(
                target.Frame.Row,
                target.Frame.Col,
                Math.Max(MinimumRenderableSize, target.Frame.Width),
                Math.Max(MinimumRenderableSize, target.Frame.Height));
        }
        return null;
    }

    internal static List<List<TerminalCell>> ContentFromBuffer(TerminalBuffer buffer)
    {
        List<List<TerminalCell>> rendered = new();
        for (int row = 0; row < buffer.Height; ++row)
        {
            List<TerminalCell> renderedRow = new();
            for (int col = 0; col < buffer.Width; ++col)
            {
                renderedRow.Add(buffer.Cell(row, col).Clone());
            }
            rendered.Add(renderedRow);
        }
        return rendered;
    }
}

public sealed class McpController
{
    private const int ComboBoxClosedRows = 1;

    private sealed class EditSnapshot
    {
        public Element Element { get; }
        public string TextValue { get; set; } = "";
        public int TextCursor { get; set; }
        public int? TextSelectionAnchor { get; set; }
        public double NumberValue { get; set; }
        public int SelectedIndex { get; set; }

        public EditSnapshot(Element element)
        {
            Element = element;
        }
    }

    private sealed class RuntimeFrame
    {
        public GeneratedWindowBase Window { get; }
        public GeneratedWindowRuntimeOptions Options { get; }
        public int FocusedIndex { get; set; } = -1;
        public Element? FocusedElementRef { get; set; }
        public bool EditMode { get; set; }
        public Element? EditScopeOwner { get; set; }
        public ScrollView? ActiveScrollView { get; set; }
        public ReusableElement? ActiveScrollViewProxy { get; set; }
        public Element? ActiveScrollViewEditElement { get; set; }
        public bool ActiveScrollViewFresh { get; set; }
        public bool SuppressActiveScrollViewScopeVisuals { get; set; }
        public ScrollView? PendingModalScrollView { get; set; }
        public ReusableElement? PendingModalScrollViewProxy { get; set; }
        public BackgroundFocusCleanupContext? ModalReturnContext { get; set; }
        public EditSnapshot? EditSnapshot { get; set; }
        public Dictionary<ScrollView, Element> ScrollViewLastDescendant { get; } = new();

        public RuntimeFrame(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options)
        {
            Window = window;
            Options = options;
        }
    }

    private readonly struct BackgroundFocusCleanupContext
    {
        public bool HadActiveStackFrame { get; }
        public Element? FocusedElement { get; }
        public int FocusedIndex { get; }
        public ScrollView? ActiveScrollView { get; }
        public ReusableElement? ActiveScrollViewProxy { get; }
        public ScrollViewPosition? ActiveScrollViewPosition { get; }

        public BackgroundFocusCleanupContext(
            bool hadActiveStackFrame,
            Element? focusedElement,
            int focusedIndex,
            ScrollView? activeScrollView,
            ReusableElement? activeScrollViewProxy,
            ScrollViewPosition? activeScrollViewPosition)
        {
            HadActiveStackFrame = hadActiveStackFrame;
            FocusedElement = focusedElement;
            FocusedIndex = focusedIndex;
            ActiveScrollView = activeScrollView;
            ActiveScrollViewProxy = activeScrollViewProxy;
            ActiveScrollViewPosition = activeScrollViewPosition;
        }
    }

    private readonly List<RuntimeFrame> frames = new();
    private readonly McpRuntimeConfig config;
    private Element? mouseSelectionElement;
    private int mouseSelectionAnchor;
    private bool copyNotificationRequested;
    private ScrollRegionHint? pendingScrollRegion;
    private static string runtimeClipboardText = "";

    public McpController(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options, McpRuntimeConfig config)
    {
        this.config = config;
        PushFrame(window, options);
        ApplyInitialFocus(Current);
    }

    public Action? BeforeStandardEscapeButtonAction { get; set; }
    public Action? BeforeModalButtonAction { get; set; }

    private RuntimeFrame Current => frames[^1];

    public void PressKey(string key)
    {
        _ = ToolPressKey(key);
    }

    public bool TakeCopyNotificationRequested()
    {
        bool requested = copyNotificationRequested;
        copyNotificationRequested = false;
        return requested;
    }

    public void MouseClickAt(Point position)
    {
        if (TryCurrentWindowContentPoint(position, out Point local))
        {
            _ = ToolMouseClickAt(local);
        }
    }

    public void MousePressAt(Point position)
    {
        if (TryCurrentWindowContentPoint(position, out Point local))
        {
            _ = ToolMousePressAt(local);
        }
    }

    public void MouseDragAt(Point position)
    {
        if (TryCurrentWindowContentPoint(position, out Point local))
        {
            _ = ToolMouseDragAt(local);
        }
    }

    public void MouseReleaseAt(Point position)
    {
        if (TryCurrentWindowContentPoint(position, out Point local))
        {
            _ = ToolMouseReleaseAt(local);
        }
    }

    public void MouseWheelAt(Point position, int wheelDelta)
    {
        if (TryCurrentWindowContentPoint(position, out Point local))
        {
            _ = ToolMouseWheelAt(local, wheelDelta);
        }
    }

    public bool ShouldCloseRequested()
    {
        return frames.Count <= 1 && Current.Options.ShouldClose?.Invoke() == true;
    }

    public List<List<TerminalCell>> RenderFrame()
    {
        return RenderFrameContent(new Size(config.ViewportWidth, config.ViewportHeight));
    }

    public List<List<TerminalCell>> RenderTerminalFrame(Size terminalSize)
    {
        return RenderFrameContent(terminalSize);
    }

    internal ScrollRegionHint? TakePendingScrollRegion()
    {
        QueueScrollRegionHint(Current.ActiveScrollView);
        ScrollRegionHint? hint = pendingScrollRegion;
        pendingScrollRegion = null;
        return hint;
    }

    private Size ViewportSize()
    {
        return new Size(
            Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, config.ViewportWidth),
            Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, config.ViewportHeight));
    }

    private void RefreshCurrentWindowLayoutForMouse()
    {
        if (frames.Count <= 1)
        {
            _ = RenderContent();
            return;
        }

        Size contentSize = GeneratedWindowRuntime.WindowStackContentSize(Current.Window, ViewportSize());
        (int focusedIndex, Element? focused) = FocusedElementForRender(
            Current,
            Current.ActiveScrollView,
            true);
        _ = GeneratedWindowRuntime.RenderGeneratedWindowContent(
            Current.Window,
            contentSize,
            focusedIndex,
            focused,
            Current.EditMode,
            Current.ActiveScrollView,
            Current.ActiveScrollViewProxy,
            Current.ActiveScrollViewEditElement,
            Current.ActiveScrollViewFresh);
    }

    private bool TryCurrentWindowContentPoint(Point position, out Point local)
    {
        local = position;
        if (frames.Count <= 1)
        {
            return true;
        }

        Size terminalSize = ViewportSize();
        if (!GeneratedWindowRuntime.WindowStackFrameContains(Current.Window, terminalSize, position))
        {
            return false;
        }

        local = GeneratedWindowRuntime.WindowStackContentPoint(Current.Window, terminalSize, position);
        return true;
    }

    private void QueueScrollRegionHint(ScrollView? scrollView)
    {
        if (scrollView is null)
        {
            return;
        }
        int delta = scrollView.ConsumeTerminalScrollDelta();
        if (delta == 0)
        {
            return;
        }
        Rect rect = scrollView.Frame;
        if (pendingScrollRegion.HasValue &&
            pendingScrollRegion.Value.Rect.Row == rect.Row &&
            pendingScrollRegion.Value.Rect.Col == rect.Col &&
            pendingScrollRegion.Value.Rect.Width == rect.Width &&
            pendingScrollRegion.Value.Rect.Height == rect.Height)
        {
            int nextDelta = pendingScrollRegion.Value.Delta + delta;
            pendingScrollRegion = nextDelta == 0 ? null : new ScrollRegionHint(rect, nextDelta);
            return;
        }
        pendingScrollRegion = new ScrollRegionHint(rect, delta);
    }

    private void PushFrame(GeneratedWindowBase nextWindow, GeneratedWindowRuntimeOptions nextOptions)
    {
        nextWindow.AttachRuntimeWindowStack(OpenWindow, CloseWindow, SetRuntimeWindowFocus);
        frames.Add(new RuntimeFrame(nextWindow, nextOptions));
    }

    private void OpenWindow(GeneratedWindowBase nextWindow, GeneratedWindowRuntimeOptions? nextOptions)
    {
        BackgroundFocusCleanupContext returnContext =
            CaptureBackgroundFocusCleanupContext(openingModal: true);
        if (Current.ActiveScrollView is not null)
        {
            Current.PendingModalScrollView = Current.ActiveScrollView;
            Current.PendingModalScrollViewProxy = Current.ActiveScrollViewProxy;
        }
        PushFrame(nextWindow, nextOptions ?? nextWindow.RuntimeOptions());
        Current.ModalReturnContext = returnContext;
        ApplyInitialFocus(Current);
    }

    public void OpenModalWindow(GeneratedWindowBase nextWindow)
    {
        OpenWindow(nextWindow, null);
    }

    private static void ClearActiveScrollViewScope(RuntimeFrame frame)
    {
        frame.EditMode = false;
        frame.EditScopeOwner = null;
        frame.ActiveScrollView = null;
        frame.ActiveScrollViewProxy = null;
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = false;
        frame.SuppressActiveScrollViewScopeVisuals = false;
        frame.EditSnapshot = null;
    }

    private static ScrollView? ActiveScrollViewForFrame(RuntimeFrame frame)
    {
        return frame.ActiveScrollView ?? frame.PendingModalScrollView;
    }

    private static ReusableElement? ActiveScrollViewProxyForFrame(RuntimeFrame frame)
    {
        return frame.ActiveScrollViewProxy ?? frame.PendingModalScrollViewProxy;
    }

    private BackgroundFocusCleanupContext CaptureBackgroundFocusCleanupContext(bool openingModal = false)
    {
        bool hadActiveStackFrame = openingModal || frames.Count > 1;
        if (!hadActiveStackFrame || frames.Count == 0)
        {
            return new BackgroundFocusCleanupContext(false, null, -1, null, null, null);
        }

        RuntimeFrame frame = frames[0];
        ScrollView? activeScrollView = ActiveScrollViewForFrame(frame);
        (int focusedIndex, Element? focused) = FocusedElementForRender(frame, activeScrollView, false);
        if (focusedIndex < 0 && focused is not null)
        {
            focusedIndex = GeneratedWindowRuntime.FocusableElements(frame.Window, activeScrollView).IndexOf(focused);
        }
        return new BackgroundFocusCleanupContext(
            true,
            focused,
            focusedIndex,
            activeScrollView,
            ActiveScrollViewProxyForFrame(frame),
            activeScrollView?.ScrollPosition());
    }

    private void CleanupBackgroundFocusAfterModalClose(BackgroundFocusCleanupContext context)
    {
        if (!context.HadActiveStackFrame || frames.Count != 1)
        {
            return;
        }

        RuntimeFrame frame = frames[0];
        ScrollView? activeScrollView = ActiveScrollViewForFrame(frame);
        if (activeScrollView is not null &&
            !FocusIdentityPresentIn(frame, activeScrollView, context.FocusedElement))
        {
            if (RepairRemovedBackgroundScrollViewScope(frame, context))
            {
                RestoreBackgroundScrollPosition(context, activeScrollView);
                return;
            }
            ClearRemovedBackgroundScrollViewScope(frame);
            return;
        }
        if (ExitBackgroundEditModeAfterModalClose(frame))
        {
            RestoreBackgroundScrollPosition(context, activeScrollView);
            return;
        }
        ClearFocusIfElementRemoved(frame, context.FocusedElement);
        RestoreBackgroundScrollPosition(context, activeScrollView);
    }

    private static void RestoreBackgroundScrollPosition(
        BackgroundFocusCleanupContext context,
        ScrollView? activeScrollView)
    {
        if (activeScrollView is not null && context.ActiveScrollViewPosition.HasValue)
        {
            activeScrollView.RestoreScrollPosition(context.ActiveScrollViewPosition.Value);
        }
    }

    private static bool RepairRemovedBackgroundScrollViewScope(
        RuntimeFrame frame,
        BackgroundFocusCleanupContext context)
    {
        ScrollView? activeScrollView = context.ActiveScrollView ?? ActiveScrollViewForFrame(frame);
        if (activeScrollView is null || !GeneratedWindowRuntime.WindowContainsElement(frame.Window, activeScrollView))
        {
            return false;
        }

        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(frame.Window, activeScrollView);
        if (focusable.Count == 0)
        {
            return false;
        }

        int index = context.FocusedIndex >= 0
            ? Math.Min(context.FocusedIndex, focusable.Count - 1)
            : Math.Min(Math.Max(0, frame.FocusedIndex), focusable.Count - 1);
        Element nextFocus = focusable[index];
        frame.EditMode = true;
        frame.EditScopeOwner = context.ActiveScrollViewProxy ?? ActiveScrollViewProxyForFrame(frame);
        frame.ActiveScrollView = activeScrollView;
        frame.ActiveScrollViewProxy = context.ActiveScrollViewProxy ?? ActiveScrollViewProxyForFrame(frame);
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = false;
        frame.SuppressActiveScrollViewScopeVisuals = false;
        frame.PendingModalScrollView = null;
        frame.PendingModalScrollViewProxy = null;
        frame.EditSnapshot = null;
        frame.FocusedIndex = index;
        frame.FocusedElementRef = nextFocus;
        if (!ReferenceEquals(nextFocus, activeScrollView))
        {
            frame.ScrollViewLastDescendant[activeScrollView] = nextFocus;
        }
        GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(frame.Window, nextFocus);
        return true;
    }

    private static bool FocusIdentityPresentIn(RuntimeFrame frame, ScrollView activeScrollView, Element? previous)
    {
        if (previous is null)
        {
            return true;
        }
        return GeneratedWindowRuntime.FocusableElements(frame.Window, activeScrollView)
            .Any(element => ReferenceEquals(element, previous));
    }

    private static void ClearRemovedBackgroundScrollViewScope(RuntimeFrame frame)
    {
        frame.EditMode = false;
        frame.EditScopeOwner = null;
        frame.ActiveScrollView = null;
        frame.ActiveScrollViewProxy = null;
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = false;
        frame.SuppressActiveScrollViewScopeVisuals = false;
        frame.PendingModalScrollView = null;
        frame.PendingModalScrollViewProxy = null;
        frame.EditSnapshot = null;
        frame.FocusedIndex = -1;
        frame.FocusedElementRef = null;
    }

    private static bool ExitBackgroundEditModeAfterModalClose(RuntimeFrame frame)
    {
        ScrollView? activeScrollView = ActiveScrollViewForFrame(frame);
        ReusableElement? activeScrollViewProxy = ActiveScrollViewProxyForFrame(frame);
        Element? focused = FocusedElementForRender(frame, activeScrollView, false).FocusedElement;
        if (activeScrollView is null)
        {
            return false;
        }

        List<Element> scopeFocusable = GeneratedWindowRuntime.FocusableElements(frame.Window, activeScrollView);
        int focusedIndex = focused is not null ? scopeFocusable.IndexOf(focused) : -1;
        frame.EditMode = true;
        frame.EditScopeOwner = activeScrollViewProxy;
        frame.ActiveScrollView = activeScrollView;
        frame.ActiveScrollViewProxy = activeScrollViewProxy;
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = false;
        frame.SuppressActiveScrollViewScopeVisuals = false;
        if (focusedIndex >= 0)
        {
            frame.FocusedIndex = focusedIndex;
            frame.FocusedElementRef = scopeFocusable[focusedIndex];
        }
        else if (focused is null)
        {
            frame.FocusedIndex = -1;
            frame.FocusedElementRef = null;
        }
        frame.PendingModalScrollView = null;
        frame.PendingModalScrollViewProxy = null;
        frame.EditSnapshot = null;
        return true;
    }

    private static void ClearFocusIfElementRemoved(RuntimeFrame frame, Element? previous)
    {
        if (previous is null)
        {
            return;
        }
        if (GeneratedWindowRuntime.FocusableElements(frame.Window)
            .Any(element => ReferenceEquals(element, previous)))
        {
            return;
        }
        if (ReferenceEquals(frame.FocusedElementRef, previous))
        {
            frame.FocusedIndex = -1;
            frame.FocusedElementRef = null;
        }
    }

    private static void ApplyInitialFocus(RuntimeFrame frame)
    {
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(frame.Window);
        frame.FocusedIndex = -1;
        frame.FocusedElementRef = null;
        Element? requestedFocusElement = frame.Window.ConsumeRequestedFocusElement();
        if (requestedFocusElement is not null)
        {
            int index = focusable.IndexOf(requestedFocusElement);
            if (index >= 0)
            {
                frame.FocusedIndex = index;
                frame.FocusedElementRef = requestedFocusElement;
            }
        }
        else if (!string.IsNullOrEmpty(frame.Options.InitialFocusName))
        {
            int index = focusable.FindIndex(element => element.Name == frame.Options.InitialFocusName);
            if (index >= 0)
            {
                frame.FocusedIndex = index;
                frame.FocusedElementRef = focusable[index];
            }
        }
        else if (frame.Options.StartInEditMode && focusable.Count > 0)
        {
            frame.FocusedIndex = 0;
            frame.FocusedElementRef = focusable[0];
        }
        if (frame.FocusedIndex < 0 || frame.FocusedIndex >= focusable.Count)
        {
            frame.EditMode = false;
            return;
        }
        Element focused = focusable[frame.FocusedIndex];
        frame.FocusedElementRef = focused;
        frame.EditMode = frame.Options.StartInEditMode && IsEditableElement(focused);
        if (frame.EditMode)
        {
            frame.EditSnapshot = CaptureSnapshot(focused);
            BeginElementEdit(focused);
            frame.Options.OnEditStarted?.Invoke(focused.Name);
        }
    }

    private void SetRuntimeWindowFocus(GeneratedWindowBase window, Element? element)
    {
        RuntimeFrame? frame = frames.LastOrDefault(candidate => ReferenceEquals(candidate.Window, window));
        if (frame is null)
        {
            return;
        }
        SetFocusInFrame(frame, element);
    }

    private void SetFocusInFrame(RuntimeFrame frame, Element? element)
    {
        if (element is not null && !element.Enabled)
        {
            element = null;
        }
        (ReusableElement Proxy, ScrollView ScrollView)? scrollContext = element is null
            ? null
            : ScrollViewFocusContextForElement(frame.Window, element);
        List<Element> focusable = scrollContext.HasValue
            ? GeneratedWindowRuntime.FocusableElements(frame.Window, scrollContext.Value.ScrollView)
            : GeneratedWindowRuntime.FocusableElements(frame.Window);
        int index = element is null ? -1 : focusable.IndexOf(element);
        Element? previous = FocusedElement(frame);
        if (previous is not null && previous != element)
        {
            frame.Options.OnFocusChanged?.Invoke(previous.Name, false);
        }
        frame.FocusedIndex = index;
        frame.FocusedElementRef = index >= 0 ? focusable[index] : null;
        frame.EditMode = false;
        frame.EditScopeOwner = null;
        frame.ActiveScrollView = scrollContext?.ScrollView;
        frame.ActiveScrollViewProxy = scrollContext?.Proxy;
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = false;
        frame.SuppressActiveScrollViewScopeVisuals = false;
        frame.EditSnapshot = null;
        if (index >= 0)
        {
            frame.Options.OnFocusChanged?.Invoke(focusable[index].Name, true);
        }
    }

    private static bool IsEditableElement(Element element)
    {
        if (!element.Enabled)
        {
            return false;
        }
        if (element is ReusableElement reusable && reusable.Child is GeneratedScrollViewBase)
        {
            return true;
        }
        return element is TextInput or NumberInput or ComboBox or ListBox or ScrollView;
    }

    private static bool IsButton(Element element)
    {
        return element.Enabled && element is Button;
    }

    private static bool IsClickableImage(Element element)
    {
        return element.Enabled && element is Image;
    }

    private static bool ActivateReusableControl(Element? element)
    {
        return element is ReusableElement { Enabled: true, Child: { } child } &&
            child.GeneratedFocusable &&
            child.ActivateGeneratedControl();
    }

    private static bool IsImmediateInput(Element element)
    {
        return element.Enabled && element is CheckBox;
    }

    private static bool ShouldRestoreFocusAfterClick(Element element)
    {
        return IsButton(element) || IsImmediateInput(element) || IsClickableImage(element);
    }

    private static bool UsesLeaveCommit(Element? element)
    {
        if (element is ListBox listBox && listBox.Multiple)
        {
            return true;
        }
        return element is not null && element.CommitMode == Element.CommitModeLeave;
    }

    private static bool IsStandardEscapeDialog(GeneratedWindowBase window)
    {
        string className = string.IsNullOrEmpty(window.McpClassName) ? window.GetType().Name : window.McpClassName;
        return className == "FileBrowser" || className.StartsWith("MessageBox", StringComparison.Ordinal);
    }

    private static string StandardEscapeButtonName(GeneratedWindowBase window)
    {
        if (!IsStandardEscapeDialog(window))
        {
            return "";
        }
        string className = string.IsNullOrEmpty(window.McpClassName) ? window.GetType().Name : window.McpClassName;
        if (className == "FileBrowser")
        {
            return GeneratedWindowRuntime.FindElement(window, "close_btn") is Button ? "close_btn" : "";
        }
        foreach (string buttonName in new[] { "cancel_btn", "no_btn", "ok_btn" })
        {
            if (GeneratedWindowRuntime.FindElement(window, buttonName) is Button)
            {
                return buttonName;
            }
        }
        return "";
    }

    private bool HandleStandardEscapeButton(RuntimeFrame frame)
    {
        string buttonName = StandardEscapeButtonName(frame.Window);
        if (string.IsNullOrEmpty(buttonName))
        {
            return false;
        }
        Element? button = GeneratedWindowRuntime.FindElement(frame.Window, buttonName);
        if (button is null)
        {
            return false;
        }
        frame.EditMode = false;
        frame.EditScopeOwner = null;
        frame.ActiveScrollView = null;
        frame.ActiveScrollViewProxy = null;
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = false;
        frame.EditSnapshot = null;
        SetFocusInFrame(frame, button);
        BeforeStandardEscapeButtonAction?.Invoke();
        OptionsFor(button).OnButton?.Invoke(button.Name);
        CloseCurrentWindowIfRequested();
        return true;
    }

    private static EditSnapshot CaptureSnapshot(Element element)
    {
        EditSnapshot snapshot = new(element);
        if (element is TextInput textInput)
        {
            snapshot.TextValue = textInput.Value;
            snapshot.TextCursor = textInput.Cursor;
            if (!string.IsNullOrEmpty(textInput.SelectionText()))
            {
                snapshot.TextSelectionAnchor = textInput.Cursor == textInput.SelectionStart
                    ? textInput.SelectionEnd
                    : textInput.SelectionStart;
            }
        }
        else if (element is NumberInput numberInput)
        {
            snapshot.NumberValue = numberInput.Value;
            numberInput.BeginEdit();
        }
        else if (element is ComboBox comboBox)
        {
            snapshot.SelectedIndex = comboBox.SelectedIndex;
        }
        else if (element is ListBox listBox)
        {
            snapshot.SelectedIndex = listBox.SelectedIndex;
        }
        return snapshot;
    }

    private static void BeginElementEdit(Element? element)
    {
        if (element is TextInput textInput)
        {
            textInput.SetCursor(textInput.Value.Length);
        }
        else if (element is NumberInput numberInput)
        {
            numberInput.BeginEdit();
        }
        else if (element is ListBox listBox)
        {
            listBox.HideActiveItem();
        }
    }

    private static void RestoreSnapshot(EditSnapshot snapshot)
    {
        if (snapshot.Element is TextInput textInput)
        {
            textInput.SetValue(snapshot.TextValue);
            textInput.SelectRange(snapshot.TextSelectionAnchor ?? snapshot.TextCursor, snapshot.TextCursor);
        }
        else if (snapshot.Element is NumberInput numberInput)
        {
            numberInput.CancelEdit();
            numberInput.SetValue(snapshot.NumberValue);
        }
        else if (snapshot.Element is ComboBox comboBox)
        {
            comboBox.SetSelectedIndex(snapshot.SelectedIndex);
        }
        else if (snapshot.Element is ListBox listBox)
        {
            listBox.SetSelectedIndex(snapshot.SelectedIndex);
        }
    }

    private static void CommitEdit(Element? element)
    {
        if (element is NumberInput numberInput)
        {
            numberInput.CommitEdit();
        }
    }

    private static void ApplyEditEscape(RuntimeFrame frame)
    {
        if (frame.EditSnapshot is not null)
        {
            RestoreSnapshot(frame.EditSnapshot);
            frame.EditSnapshot = null;
        }
        frame.EditMode = false;
    }

    private static void EscapeElementEdit(RuntimeFrame frame, Element? element)
    {
        if (UsesLeaveCommit(element))
        {
            CommitEdit(element);
            frame.EditSnapshot = null;
            frame.EditMode = false;
            return;
        }
        ApplyEditEscape(frame);
    }

    private static int SelectedIndexOf(Element? element)
    {
        return element switch
        {
            ComboBox comboBox => comboBox.SelectedIndex,
            ListBox listBox => listBox.SelectedIndex,
            _ => -1,
        };
    }

    private void CloseWindow()
    {
        BackgroundFocusCleanupContext cleanupContext =
            Current.ModalReturnContext ?? CaptureBackgroundFocusCleanupContext();
        if (frames.Count > 1)
        {
            frames.RemoveAt(frames.Count - 1);
        }
        CleanupBackgroundFocusAfterModalClose(cleanupContext);
    }

    private void CloseCurrentWindowIfRequested()
    {
        if (frames.Count > 1 && Current.Options.ShouldClose?.Invoke() == true)
        {
            CloseWindow();
        }
    }

    private bool HasActiveStackFrame()
    {
        return frames.Count > 1;
    }

    private bool ActiveWindowIsRoot()
    {
        return frames.Count == 1;
    }

    private bool ActiveWindowIs(GeneratedWindowBase window)
    {
        return frames.Count > 0 && ReferenceEquals(Current.Window, window);
    }

    private JsonObject ActivatedElementResult(string elementName)
    {
        return new JsonObject
        {
            ["ok"] = true,
            ["element_id"] = elementName,
        };
    }

    private bool HandleActiveFrameButton(string name)
    {
        if (Current.Options.OnButton is not null)
        {
            Current.Options.OnButton(name);
            return true;
        }
        return false;
    }

    private void DelayModalButtonActionForVisibleFocus()
    {
        if (!HasActiveStackFrame())
        {
            return;
        }
        BeforeModalButtonAction?.Invoke();
    }

    public void ServeStdio()
    {
        string? line;
        while ((line = Console.ReadLine()) is not null)
        {
            string response = HandleLine(line);
            if (response.Length > 0)
            {
                Console.WriteLine(response);
                Console.Out.Flush();
            }
        }
    }

    public void ServeTcp()
    {
        if (config.Port <= 0)
        {
            throw new InvalidOperationException("--mcp-port is required for TCP MCP transport.");
        }
        IPAddress address = IPAddress.Parse(string.IsNullOrEmpty(config.Host) ? "127.0.0.1" : config.Host);
        TcpListener listener = new(address, config.Port);
        listener.Start();
        try
        {
            while (true)
            {
                using TcpClient client = listener.AcceptTcpClient();
                using NetworkStream stream = client.GetStream();
                using StreamReader reader = new(stream);
                using StreamWriter writer = new(stream) { NewLine = "\n", AutoFlush = true };
                string? line = reader.ReadLine();
                if (line is null)
                {
                    continue;
                }
                string response = HandleLine(line);
                if (response.Length > 0)
                {
                    writer.WriteLine(response);
                }
            }
        }
        finally
        {
            listener.Stop();
        }
    }

    private string HandleLine(string line)
    {
        try
        {
            JsonNode? request = JsonNode.Parse(line);
            if (request is null)
            {
                return "";
            }
            JsonNode? response = HandleJsonRpc(request);
            return response?.ToJsonString(new JsonSerializerOptions { WriteIndented = false }) ?? "";
        }
        catch (Exception exc)
        {
            return JsonRpcError(null, exc.Message).ToJsonString();
        }
    }

    private JsonNode? HandleJsonRpc(JsonNode request)
    {
        if (request is JsonArray batch)
        {
            JsonArray responses = new();
            foreach (JsonNode? item in batch)
            {
                if (item is null)
                {
                    continue;
                }
                JsonNode? response = HandleJsonRpcObject(item);
                if (response is not null)
                {
                    responses.Add(response);
                }
            }
            return responses.Count == 0 ? null : responses;
        }
        return HandleJsonRpcObject(request);
    }

    private JsonNode? HandleJsonRpcObject(JsonNode request)
    {
        JsonNode? id = request["id"]?.DeepClone();
        bool hasId = request["id"] is not null;
        try
        {
            JsonNode? result = HandleRequest(request);
            if (!hasId)
            {
                return null;
            }
            return new JsonObject
            {
                ["jsonrpc"] = "2.0",
                ["id"] = id,
                ["result"] = result,
            };
        }
        catch (Exception exc)
        {
            return JsonRpcError(hasId ? id : null, exc.Message);
        }
    }

    private JsonNode? HandleRequest(JsonNode request)
    {
        string method = request["method"]?.GetValue<string>() ?? "";
        JsonObject parameters = request["params"] as JsonObject ?? new JsonObject();
        if (method == "initialize")
        {
            return new JsonObject
            {
                ["protocolVersion"] = "2024-11-05",
                ["serverInfo"] = new JsonObject { ["name"] = "ui-csharp-runtime", ["version"] = "0.1" },
                ["capabilities"] = new JsonObject { ["tools"] = new JsonObject() },
            };
        }
        if (method == "tools/list")
        {
            JsonArray tools = new();
            foreach (string name in BuiltinToolNames())
            {
                tools.Add(new JsonObject
                {
                    ["name"] = name,
                    ["description"] = "",
                    ["inputSchema"] = new JsonObject { ["type"] = "object", ["properties"] = new JsonObject() },
                });
            }
            foreach (GeneratedAppToolMetadata tool in Current.Window.McpAppTools)
            {
                tools.Add(new JsonObject
                {
                    ["name"] = tool.Name,
                    ["description"] = tool.Description,
                    ["inputSchema"] = JsonNode.Parse(string.IsNullOrEmpty(tool.InputSchemaJson) ? "{\"type\":\"object\",\"properties\":{}}" : tool.InputSchemaJson),
                });
            }
            return tools;
        }
        if (method == "tools/call")
        {
            string name = parameters["name"]?.GetValue<string>() ?? "";
            JsonObject arguments = parameters["arguments"] as JsonObject ?? new JsonObject();
            JsonNode? result = CallTool(name, arguments);
            return new JsonObject
            {
                ["content"] = new JsonArray(new JsonObject
                {
                    ["type"] = "text",
                    ["text"] = result?.ToJsonString() ?? "null",
                }),
            };
        }
        return CallTool(method.StartsWith("ui.", StringComparison.Ordinal) ? method[3..] : method, parameters);
    }

    private JsonNode? CallTool(string name, JsonObject arguments)
    {
        name = ResolveToolName(name);
        return name switch
        {
            "get_window" => ToolGetWindow(),
            "get_elements" => ToolGetElements(),
            "get_element" => ToolGetElement(arguments["element_id"]?.GetValue<string>() ?? ""),
            "get_value" => ToolGetValue(arguments["element_id"]?.GetValue<string>() ?? ""),
            "get_options" => ToolGetOptions(arguments["element_id"]?.GetValue<string>() ?? ""),
            "get_focused_element" => ToolGetFocusedElement(),
            "get_edit_mode" => new JsonObject { ["edit_mode"] = Current.EditMode },
            "get_render_frame" => ToolGetRenderFrame(),
            "get_render_rect" => ToolGetViewport(),
            "get_render_snapshot" => ToolGetRenderSnapshot(arguments),
            "get_render_snapshot_compact" => ToolGetRenderSnapshotCompact(arguments),
            "get_render_cell" => ToolGetRenderCell(arguments),
            "get_image_render_info" => ToolGetImageRenderInfo(arguments),
            "get_text_snapshot" => ToolGetTextSnapshot(),
            "get_accessibility_snapshot" or "get_state" => ToolGetAccessibilitySnapshot(),
            "get_schema" => ToolGetAccessibilitySnapshot(),
            "get_source" => new JsonObject { ["class"] = Current.Window.GetType().Name, ["mcp_enabled"] = Current.Window.McpEnabled, ["source_markdown"] = Current.Window.McpSourceMarkdown },
            "get_viewport" => ToolGetViewport(),
            "set_viewport" => ToolSetViewport(arguments),
            "repaint" => new JsonObject(),
            "focus_element" => ToolFocusElement(arguments["element_id"]?.GetValue<string>() ?? ""),
            "enter_edit_mode" => ToolEnterEditMode(arguments["element_id"]?.GetValue<string>()),
            "exit_edit_mode" => ToolExitEditMode(),
            "activate_element" => ToolActivateElement(arguments["element_id"]?.GetValue<string>() ?? ""),
            "click_element" => ToolClickElement(arguments["element_id"]?.GetValue<string>() ?? ""),
            "mouse_click" => ToolMouseClick(arguments),
            "mouse_drag" => ToolMouseDrag(arguments),
            "mouse_press" => ToolMousePress(arguments),
            "mouse_move" => ToolMouseMove(arguments),
            "mouse_release" => ToolMouseRelease(arguments),
            "scroll" => ToolScroll(arguments),
            "press_key" => ToolPressKey(arguments["key"]?.GetValue<string>() ?? ""),
            "type_text" or "paste_text" => ToolTypeText(arguments),
            "set_text" => ToolSetText(arguments),
            "clear_text" => ToolClearText(arguments["element_id"]?.GetValue<string>() ?? ""),
            "set_value" => ToolSetValue(arguments),
            "set_checked" => ToolSetChecked(arguments),
            "check" => ToolSetChecked(arguments, true),
            "uncheck" => ToolSetChecked(arguments, false),
            "set_cursor" => ToolSetCursor(arguments),
            "select_text" => ToolSelectText(arguments),
            "copy_selection" => ToolCopySelection(arguments),
            "replace_selection" => ToolReplaceSelection(arguments),
            "select_option" => ToolSelectOption(arguments),
            "set_selection" => ToolSetSelection(arguments),
            _ => ToolCallAppTool(name, arguments),
        };
    }

    private static string ResolveToolName(string name)
    {
        return name switch
        {
            "ui.snapshot" or "snapshot" => "get_accessibility_snapshot",
            "ui.render_snapshot" or "render_snapshot" => "get_render_snapshot",
            "ui.text_snapshot" or "text_snapshot" => "get_text_snapshot",
            "ui.click" or "click" => "click_element",
            "ui.press" or "press" => "press_key",
            "ui.fill" or "fill" => "set_text",
            "ui.focus" or "focus" => "focus_element",
            "ui.activate" or "activate" => "activate_element",
            "ui.set_checked" => "set_checked",
            "ui.check" => "check",
            "ui.uncheck" => "uncheck",
            "ui.set_selection" => "set_selection",
            "ui.set_value" => "set_value",
            "ui.get_options" => "get_options",
            _ => name.StartsWith("ui.", StringComparison.Ordinal) ? name[3..] : name,
        };
    }

    private static IEnumerable<string> BuiltinToolNames()
    {
        return new[]
        {
            "get_window", "get_elements", "get_element", "get_value", "get_options",
            "get_focused_element", "get_edit_mode", "get_accessibility_snapshot",
            "get_render_frame", "get_render_snapshot", "get_render_snapshot_compact",
            "get_render_cell", "get_image_render_info", "get_render_rect", "get_text_snapshot", "get_state", "get_schema", "get_source", "get_viewport",
            "set_viewport", "repaint", "focus_element", "enter_edit_mode", "exit_edit_mode",
            "activate_element", "click_element", "mouse_click", "mouse_drag",
            "mouse_move", "mouse_press", "mouse_release", "scroll",
            "press_key", "type_text", "set_text", "paste_text", "clear_text",
            "set_value", "set_checked", "check", "uncheck", "set_cursor", "select_text",
            "copy_selection", "replace_selection", "select_option", "set_selection",
            "ui.snapshot", "ui.render_snapshot",
            "ui.text_snapshot", "ui.click", "ui.press", "ui.fill",
            "ui.check", "ui.uncheck", "ui.set_selection",
        };
    }

    private JsonNode? ToolCallAppTool(string name, JsonObject arguments)
    {
        string result = Current.Window.CallAppTool(name, arguments.ToJsonString(new JsonSerializerOptions { WriteIndented = false }));
        if (string.IsNullOrEmpty(result))
        {
            return null;
        }
        return JsonNode.Parse(result);
    }

    private JsonObject ToolGetWindow()
    {
        return new JsonObject
        {
            ["title"] = Current.Window.Title,
            ["class"] = Current.Window.GetType().Name,
            ["backend"] = "csharp",
            ["mode"] = Current.Window.GeneratedKind,
            ["description"] = Current.Window.McpWindowDescription,
            ["width"] = config.ViewportWidth,
            ["height"] = config.ViewportHeight,
            ["mcp_enabled"] = Current.Window.McpEnabled,
        };
    }

    private JsonArray ToolGetElements()
    {
        JsonArray elements = new();
        foreach (Element element in Current.Window.Elements)
        {
            if (Exposed(element))
            {
                elements.Add(Snapshot(element));
            }
        }
        return elements;
    }

    private JsonNode ToolGetElement(string elementId)
    {
        return Snapshot(RequireElement(elementId));
    }

    private JsonObject ToolGetValue(string elementId)
    {
        return new JsonObject { ["value"] = ElementValue(RequireElement(elementId)) };
    }

    private JsonObject ToolGetOptions(string elementId)
    {
        Element element = RequireElement(elementId);
        JsonArray options = new();
        if (element is ComboBox comboBox)
        {
            foreach (string option in comboBox.Options)
            {
                options.Add(option);
            }
        }
        else if (element is ListBox listBox)
        {
            foreach (string option in listBox.Options)
            {
                options.Add(option);
            }
        }
        else
        {
            throw new InvalidOperationException("Element has no selectable options: " + elementId);
        }
        return new JsonObject { ["options"] = options };
    }

    private JsonNode? ToolGetFocusedElement()
    {
        Element? focused = FocusedElement();
        return focused is null ? null : Snapshot(focused);
    }

    private JsonObject ToolGetAccessibilitySnapshot()
    {
        return new JsonObject
        {
            ["window"] = ToolGetWindow(),
            ["focused_element"] = ToolGetFocusedElement(),
            ["edit_mode"] = Current.EditMode,
            ["elements"] = ToolGetElements(),
        };
    }

    private JsonObject ToolGetRenderFrame()
    {
        List<List<TerminalCell>> content = RenderFrame();
        return new JsonObject
        {
            ["width"] = content.Count == 0 ? 0 : content[0].Count,
            ["height"] = content.Count,
            ["cells"] = RenderHelpers.CompactCells(content),
        };
    }

    private JsonObject ToolGetRenderSnapshot(JsonObject arguments)
    {
        List<List<TerminalCell>> content;
        using (new GradientRenderTime(JsonInt64(arguments, "snapshot_time_ms")))
        {
            content = RenderSnapshotContent(arguments);
        }
        JsonArray textLines = new();
        foreach (string line in RenderHelpers.RenderedText(content))
        {
            textLines.Add(line);
        }
        return new JsonObject
        {
            ["ansi_lines"] = new JsonArray(),
            ["text_lines"] = textLines,
            ["cells"] = RenderHelpers.SnapshotCells(content),
        };
    }

    private JsonObject ToolGetRenderSnapshotCompact(JsonObject arguments)
    {
        List<List<TerminalCell>> content;
        using (new GradientRenderTime(JsonInt64(arguments, "snapshot_time_ms")))
        {
            content = RenderSnapshotContent(arguments);
        }
        return new JsonObject
        {
            ["format"] = "render-cells-v1",
            ["cells"] = RenderHelpers.CompactCells(content),
        };
    }

    private JsonObject ToolGetRenderCell(JsonObject arguments)
    {
        List<List<TerminalCell>> content;
        using (new GradientRenderTime(JsonInt64(arguments, "snapshot_time_ms")))
        {
            content = RenderSnapshotContent(arguments);
        }
        int row = JsonInt(arguments, "y");
        int col = JsonInt(arguments, "x");
        if (row < 0 || row >= content.Count)
        {
            throw new InvalidOperationException("render cell y out of range: " + row);
        }
        if (col < 0 || col >= content[row].Count)
        {
            throw new InvalidOperationException("render cell x out of range: " + col);
        }
        JsonObject cell = RenderCellJson(content[row][col]);
        cell["x"] = col;
        cell["y"] = row;
        return cell;
    }

    private JsonObject ToolGetImageRenderInfo(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        Element element = RequireElement(elementId);
        if (element is not Image image)
        {
            throw new InvalidOperationException("Element is not an image: " + elementId);
        }

        List<(Element Element, Rect Frame)> savedFrames = CaptureWindowElementFrames(Current.Window);
        Rect bounds;
        try
        {
            _ = RenderFrameContentCore(ViewportSize());
            bounds = element.Frame;
        }
        finally
        {
            RestoreElementFrames(savedFrames);
        }

        Size size = new(
            Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, bounds.Width),
            Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, bounds.Height));
        Element? focused = FocusedElement();
        ImageRenderInfo info = image.RenderInfo(size, new ElementRenderState
        {
            Focused = ReferenceEquals(focused, element),
            EditMode = ReferenceEquals(focused, element) && Current.EditMode,
        });

        JsonArray signature = new();
        foreach (string item in info.SampleSignature)
        {
            signature.Add(item);
        }
        int imageRight = info.ImageLeft + info.ImageWidth;
        int imageBottom = info.ImageTop + info.ImageHeight;
        int visibleRight = info.VisibleLeft + info.VisibleWidth;
        int visibleBottom = info.VisibleTop + info.VisibleHeight;
        return new JsonObject
        {
            ["id"] = elementId,
            ["source"] = info.Source,
            ["fit"] = info.Fit,
            ["configured_render_mode"] = info.ConfiguredRenderMode,
            ["resolved_render_mode"] = info.ResolvedRenderMode,
            ["source_loaded"] = info.SourceLoaded,
            ["source_width"] = info.SourceWidth,
            ["source_height"] = info.SourceHeight,
            ["element_width"] = info.ElementWidth,
            ["element_height"] = info.ElementHeight,
            ["cell_pixel_width"] = info.CellPixelWidth,
            ["cell_pixel_height"] = info.CellPixelHeight,
            ["image_left"] = info.ImageLeft,
            ["image_top"] = info.ImageTop,
            ["image_width"] = info.ImageWidth,
            ["image_height"] = info.ImageHeight,
            ["image_right"] = imageRight,
            ["image_bottom"] = imageBottom,
            ["visible_left"] = info.VisibleLeft,
            ["visible_top"] = info.VisibleTop,
            ["visible_width"] = info.VisibleWidth,
            ["visible_height"] = info.VisibleHeight,
            ["visible_right"] = visibleRight,
            ["visible_bottom"] = visibleBottom,
            ["raw_expected"] = info.RawExpected,
            ["raw_present"] = info.RawPresent,
            ["sample_signature"] = signature,
            ["bounds"] = new JsonObject
            {
                ["top"] = bounds.Row,
                ["left"] = bounds.Col,
                ["bottom"] = bounds.Row + bounds.Height,
                ["right"] = bounds.Col + bounds.Width,
                ["width"] = bounds.Width,
                ["height"] = bounds.Height,
            },
            ["absolute_image_left"] = bounds.Col + info.ImageLeft,
            ["absolute_image_top"] = bounds.Row + info.ImageTop,
            ["absolute_image_right"] = bounds.Col + imageRight,
            ["absolute_image_bottom"] = bounds.Row + imageBottom,
            ["absolute_visible_left"] = bounds.Col + info.VisibleLeft,
            ["absolute_visible_top"] = bounds.Row + info.VisibleTop,
            ["absolute_visible_right"] = bounds.Col + visibleRight,
            ["absolute_visible_bottom"] = bounds.Row + visibleBottom,
        };
    }

    private static List<(Element Element, Rect Frame)> CaptureWindowElementFrames(GeneratedWindowBase window)
    {
        List<(Element Element, Rect Frame)> frames = new();
        foreach (Element element in window.Elements)
        {
            CaptureElementFrame(element, frames);
        }
        return frames;
    }

    private static void CaptureElementFrame(
        Element element,
        List<(Element Element, Rect Frame)> frames)
    {
        if (frames.Any(item => ReferenceEquals(item.Element, element)))
        {
            return;
        }
        frames.Add((element, element.Frame));
        foreach (Element child in element.Children)
        {
            CaptureElementFrame(child, frames);
        }
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            foreach (Element child in reusable.Child.Elements)
            {
                CaptureElementFrame(child, frames);
            }
        }
        if (element is ScrollView scrollView)
        {
            foreach (Element child in scrollView.Children)
            {
                CaptureElementFrame(child, frames);
            }
        }
    }

    private static void RestoreElementFrames(List<(Element Element, Rect Frame)> frames)
    {
        foreach ((Element element, Rect frame) in frames)
        {
            element.Frame = frame;
        }
    }

    private static JsonObject RenderCellJson(TerminalCell cell)
    {
        return new JsonObject
        {
            ["char"] = cell.Text.Length == 0 ? " " : cell.Text,
            ["foreground"] = cell.Foreground is null || cell.Foreground.IsTransparent ? null : cell.Foreground.ToString(),
            ["background"] = cell.Background is null || cell.Background.IsTransparent ? null : cell.Background.ToString(),
            ["attributes"] = new JsonArray(),
        };
    }

    private List<List<TerminalCell>> RenderSnapshotContent(JsonObject arguments)
    {
        string renderScope = JsonString(arguments, "render_scope", "full_surface");
        if (renderScope == "full_surface" || renderScope.Length == 0)
        {
            return RenderFrame();
        }
        if (renderScope == "active_window")
        {
            return RenderContent();
        }
        throw new InvalidOperationException("unknown render_scope: " + renderScope);
    }

    private JsonObject ToolGetTextSnapshot()
    {
        JsonArray lines = new();
        foreach (string line in RenderHelpers.RenderedText(RenderContent()))
        {
            lines.Add(line);
        }
        return new JsonObject { ["lines"] = lines };
    }

    private JsonObject ToolSetViewport(JsonObject arguments)
    {
        config.ViewportRow = Math.Max(0, arguments["row"]?.GetValue<int>() ?? config.ViewportRow);
        config.ViewportCol = Math.Max(0, arguments["col"]?.GetValue<int>() ?? config.ViewportCol);
        config.ViewportWidth = Math.Max(1, arguments["width"]?.GetValue<int>() ?? config.ViewportWidth);
        config.ViewportHeight = Math.Max(1, arguments["height"]?.GetValue<int>() ?? config.ViewportHeight);
        return ToolGetViewport();
    }

    private JsonObject ToolGetViewport()
    {
        return new JsonObject
        {
            ["row"] = config.ViewportRow,
            ["col"] = config.ViewportCol,
            ["width"] = config.ViewportWidth,
            ["height"] = config.ViewportHeight,
        };
    }

    private JsonNode ToolFocusElement(string elementId)
    {
        RenderContent();
        Element element = RequireElement(elementId);
        GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, element);
        (ReusableElement Proxy, ScrollView ScrollView)? scrollContext =
            ScrollViewFocusContextForElement(Current.Window, element);
        List<Element> focusable = scrollContext.HasValue
            ? GeneratedWindowRuntime.FocusableElements(Current.Window, scrollContext.Value.ScrollView)
            : GeneratedWindowRuntime.FocusableElements(Current.Window);
        int index = focusable.IndexOf(element);
        if (index >= 0)
        {
            Element? previous = FocusedElement();
            if (previous is not null && previous != element)
            {
                Current.Options.OnFocusChanged?.Invoke(previous.Name, false);
            }
            Current.FocusedIndex = index;
            Current.FocusedElementRef = focusable[index];
            Current.EditMode = false;
            Current.EditScopeOwner = null;
            Current.ActiveScrollView = scrollContext?.ScrollView;
            Current.ActiveScrollViewProxy = scrollContext?.Proxy;
            Current.ActiveScrollViewFresh = false;
            Current.Options.OnFocusChanged?.Invoke(element.Name, true);
        }
        return Snapshot(element);
    }

    private JsonNode ToolEnterEditMode(string? elementId)
    {
        if (!string.IsNullOrEmpty(elementId))
        {
            if (Current.EditMode && Current.ActiveScrollView is null)
            {
                CommitEdit(FocusedElement());
                Current.EditSnapshot = null;
                Current.ActiveScrollViewEditElement = null;
                Current.EditMode = false;
            }
            _ = ToolFocusElement(elementId);
        }
        Element? entered = FocusedElement();
        if (entered is not null && EnterScrollViewScope(entered))
        {
            Current.Options.OnEditStarted?.Invoke(entered.Name);
            return new JsonObject { ["edit_mode"] = Current.EditMode };
        }
        Current.EditMode = true;
        if (entered is not null)
        {
            Current.EditSnapshot = CaptureSnapshot(entered);
            BeginElementEdit(entered);
            Current.Options.OnEditStarted?.Invoke(entered.Name);
        }
        return new JsonObject { ["edit_mode"] = Current.EditMode };
    }

    private bool EnterScrollViewScope(Element focused)
    {
        if (focused is ReusableElement reusable && reusable.Child is GeneratedScrollViewBase generatedScrollView)
        {
            Current.EditMode = true;
            Current.EditScopeOwner = reusable;
            Current.ActiveScrollView = generatedScrollView.ScrollView();
            Current.ActiveScrollViewProxy = reusable;
            Current.ActiveScrollViewEditElement = null;
            Current.ActiveScrollViewFresh = true;
            Current.SuppressActiveScrollViewScopeVisuals = false;
            Current.EditSnapshot = null;
            FocusFirstScrollViewScopeElement(Current, reusable, generatedScrollView.ScrollView());
            return true;
        }
        if (focused is ScrollView scrollView)
        {
            Current.EditMode = true;
            Current.EditScopeOwner = null;
            Current.ActiveScrollView = scrollView;
            Current.ActiveScrollViewProxy = null;
            Current.ActiveScrollViewEditElement = null;
            Current.ActiveScrollViewFresh = true;
            Current.SuppressActiveScrollViewScopeVisuals = false;
            Current.EditSnapshot = null;
            FocusFirstScrollViewScopeElement(Current, null, scrollView);
            return true;
        }
        Current.ActiveScrollView = null;
        Current.ActiveScrollViewProxy = null;
        Current.ActiveScrollViewEditElement = null;
        Current.ActiveScrollViewFresh = false;
        Current.SuppressActiveScrollViewScopeVisuals = false;
        return false;
    }

    private JsonNode ToolExitEditMode()
    {
        if (Current.EditScopeOwner is not null)
        {
            List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window);
            int ownerIndex = focusable.IndexOf(Current.EditScopeOwner);
            if (ownerIndex >= 0)
            {
                Current.FocusedIndex = ownerIndex;
                Current.FocusedElementRef = focusable[ownerIndex];
            }
            Current.EditScopeOwner = null;
        }
        Current.EditMode = false;
        Current.ActiveScrollView = null;
        Current.ActiveScrollViewProxy = null;
        Current.ActiveScrollViewEditElement = null;
        Current.ActiveScrollViewFresh = false;
        Current.SuppressActiveScrollViewScopeVisuals = false;
        Current.EditSnapshot = null;
        return FocusedElement() is Element focused ? Snapshot(focused) : new JsonObject { ["ok"] = true };
    }

    private JsonNode ToolActivateElement(string elementId)
    {
        RenderContent();
        Element element = RequireElement(elementId);
        if (!element.Enabled)
        {
            return Snapshot(element);
        }
        return ActivateResolvedElement(element, IsButton(element) || IsClickableImage(element), elementId);
    }

    private JsonNode ActivateResolvedElement(
        Element element,
        bool activateScrollScopeAfterActivation = true,
        string? elementId = null)
    {
        GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, element);
        RuntimeFrame activationFrame = Current;
        int frameCountBeforeActivation = frames.Count;
        (ReusableElement Proxy, ScrollView ScrollView)? activatedScrollContext =
            FocusElementWithScrollViewScope(element);
        if (IsButton(element) || IsClickableImage(element))
        {
            string activatedName = element.Name;
            GeneratedWindowBase activatedWindow = activationFrame.Window;
            bool hadActiveStackFrame = HasActiveStackFrame();
            BackgroundFocusCleanupContext cleanupContext = CaptureBackgroundFocusCleanupContext();
            if (IsButton(element) && HasActiveStackFrame())
            {
                DelayModalButtonActionForVisibleFocus();
            }
            activationFrame.EditMode = false;
            if (OwnerWindowFor(activatedWindow, element) is GeneratedWindowBase owner &&
                !ReferenceEquals(owner, activatedWindow))
            {
                owner.AttachRuntimeWindowStack(OpenWindow, CloseWindow, SetRuntimeWindowFocus);
                owner.HandleGeneratedButton(activatedName);
            }
            else
            {
                HandleActiveFrameButton(activatedName);
            }
            if (hadActiveStackFrame && ActiveWindowIsRoot())
            {
                CleanupBackgroundFocusAfterModalClose(cleanupContext);
            }
            if (ActiveWindowIs(activatedWindow))
            {
                Element? activeElement = elementId is not null
                    ? GeneratedWindowRuntime.FindElement(Current.Window, elementId)
                    : element;
                if (activeElement is not null)
                {
                    FocusElementWithScrollViewScope(activeElement);
                    return Snapshot(activeElement);
                }
            }
            return ActivatedElementResult(activatedName);
        }
        else if (ActivateReusableControl(element))
        {
            Current.EditMode = false;
        }
        else if (element is CheckBox)
        {
            string activatedElementId = elementId ?? ElementSnapshotId(element);
            string activatedName = element.Name;
            Current.EditMode = false;
            Current.ActiveScrollViewEditElement = null;
            element.HandleKey("Enter");
            DispatchChanged(element);
            if (ActiveWindowIs(activationFrame.Window))
            {
                Element? activeElement = GeneratedWindowRuntime.FindElement(Current.Window, activatedElementId);
                if (activeElement is not null)
                {
                    FocusElementWithScrollViewScope(activeElement);
                    Current.EditMode = false;
                    Current.ActiveScrollViewEditElement = null;
                    return Snapshot(activeElement);
                }
            }
            return ActivatedElementResult(activatedName);
        }
        else
        {
            Current.EditMode = true;
            element.HandleKey("Enter");
        }
        DispatchChanged(element);
        if (activateScrollScopeAfterActivation && activatedScrollContext.HasValue && frames.Count == frameCountBeforeActivation)
        {
            ActivateScrollViewScopeInFrame(
                activationFrame,
                activatedScrollContext.Value.Proxy,
                activatedScrollContext.Value.ScrollView);
        }
        else if (activateScrollScopeAfterActivation && activatedScrollContext.HasValue)
        {
            activationFrame.PendingModalScrollViewProxy = activatedScrollContext.Value.Proxy;
            activationFrame.PendingModalScrollView = activatedScrollContext.Value.ScrollView;
        }
        CloseCurrentWindowIfRequested();
        return Snapshot(element);
    }

    private (ReusableElement Proxy, ScrollView ScrollView)? FocusElementWithScrollViewScope(Element element)
    {
        FocusElement(element);
        (ReusableElement Proxy, ScrollView ScrollView)? scrollContext =
            ScrollViewFocusContextForElement(Current.Window, element);
        if (!scrollContext.HasValue)
        {
            return null;
        }

        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, scrollContext.Value.ScrollView);
        int index = focusable.IndexOf(element);
        Current.EditMode = true;
        Current.EditScopeOwner = scrollContext.Value.Proxy;
        Current.ActiveScrollView = scrollContext.Value.ScrollView;
        Current.ActiveScrollViewProxy = scrollContext.Value.Proxy;
        Current.ActiveScrollViewEditElement = null;
        Current.ActiveScrollViewFresh = false;
        Current.SuppressActiveScrollViewScopeVisuals = false;
        Current.EditSnapshot = null;
        if (index >= 0)
        {
            Current.FocusedIndex = index;
            Current.FocusedElementRef = focusable[index];
            Current.ScrollViewLastDescendant[scrollContext.Value.ScrollView] = focusable[index];
        }
        return scrollContext;
    }

    private JsonNode ToolClickElement(string elementId)
    {
        RenderContent();
        Element element = RequireElement(elementId);
        if (!element.Enabled)
        {
            return Snapshot(element);
        }
        if (ActivateReusableControl(element))
        {
            Element? activeReusable = GeneratedWindowRuntime.FindElement(Current.Window, elementId);
            if (activeReusable is not null)
            {
                if (ShouldRestoreFocusAfterClick(activeReusable))
                {
                    FocusElementWithScrollViewScope(activeReusable);
                }
                return Snapshot(activeReusable);
            }
            return new JsonObject { ["ok"] = true, ["element_id"] = elementId };
        }

        BackgroundFocusCleanupContext cleanupContext = CaptureBackgroundFocusCleanupContext();
        Rect frame = element.Frame;
        JsonNode result = ToolMousePressAt(new Point(
            frame.Row + frame.Height / 2,
            frame.Col + frame.Width / 2));
        CleanupBackgroundFocusAfterModalClose(cleanupContext);
        Element? activeElement = GeneratedWindowRuntime.FindElement(Current.Window, elementId);
        if (activeElement is not null)
        {
            if (ShouldRestoreFocusAfterClick(activeElement))
            {
                FocusElementWithScrollViewScope(activeElement);
            }
            return Snapshot(activeElement);
        }
        return result;
    }

    private static void ActivateScrollViewScopeInFrame(RuntimeFrame frame, ReusableElement proxy, ScrollView scrollView)
    {
        frame.EditMode = true;
        frame.EditScopeOwner = proxy;
        frame.ActiveScrollView = scrollView;
        frame.ActiveScrollViewProxy = proxy;
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = true;
        frame.SuppressActiveScrollViewScopeVisuals = false;
        frame.EditSnapshot = null;
    }

    private static void FocusScrollViewScopeInFrame(RuntimeFrame frame, ReusableElement proxy, ScrollView scrollView)
    {
        frame.EditMode = true;
        frame.EditScopeOwner = proxy;
        frame.ActiveScrollView = scrollView;
        frame.ActiveScrollViewProxy = proxy;
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = true;
        frame.SuppressActiveScrollViewScopeVisuals = false;
        frame.EditSnapshot = null;
        FocusFirstScrollViewScopeElement(frame, proxy, scrollView);
    }

    private static List<Element> ScrollViewScopeFocusableElements(RuntimeFrame frame, ScrollView scrollView)
    {
        List<Element> activeFocusable = GeneratedWindowRuntime.FocusableElements(frame.Window, scrollView);
        Element? scrollViewProxy =
            frame.ActiveScrollViewProxy ??
            GeneratedWindowRuntime.ReusableContainingElement(frame.Window, scrollView);
        Element scopeRoot = scrollViewProxy ?? scrollView;
        int scrollIndex = activeFocusable.IndexOf(scopeRoot);
        if (scrollIndex < 0)
        {
            return new List<Element>();
        }

        Element? nextNavigationElement = null;
        List<Element> navigationFocusable = GeneratedWindowRuntime.FocusableElements(frame.Window);
        int navigationIndex = navigationFocusable.IndexOf(scopeRoot);
        if (navigationIndex >= 0 && navigationIndex + 1 < navigationFocusable.Count)
        {
            nextNavigationElement = navigationFocusable[navigationIndex + 1];
        }

        List<Element> scoped = new();
        for (int index = scrollIndex; index < activeFocusable.Count; ++index)
        {
            Element element = activeFocusable[index];
            if (index != scrollIndex &&
                nextNavigationElement is not null &&
                ReferenceEquals(element, nextNavigationElement))
            {
                break;
            }
            scoped.Add(element);
        }
        return scoped;
    }

    private static void FocusFirstScrollViewScopeElement(RuntimeFrame frame, ReusableElement? proxy, ScrollView scrollView)
    {
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(frame.Window, scrollView);
        List<Element> scoped = ScrollViewScopeFocusableElements(frame, scrollView);
        Element? target = null;
        if (frame.ScrollViewLastDescendant.TryGetValue(scrollView, out Element? remembered) &&
            scoped.Contains(remembered))
        {
            target = remembered;
        }
        target ??= scoped.FirstOrDefault(element =>
            !ReferenceEquals(element, scrollView) &&
            !ReferenceEquals(element, proxy) &&
            ElementVisibleInScrollView(scrollView, element));
        target ??= scoped.FirstOrDefault(element =>
            !ReferenceEquals(element, scrollView) && !ReferenceEquals(element, proxy));
        target ??= proxy is not null ? proxy : scrollView;
        int index = focusable.IndexOf(target);
        if (index < 0)
        {
            return;
        }
        frame.FocusedIndex = index;
        frame.FocusedElementRef = focusable[index];
    }

    private static bool ElementVisibleInScrollView(ScrollView scrollView, Element element)
    {
        Rect viewport = GeneratedWindowRuntime.ScrollViewViewportClip(scrollView.Frame, scrollView.Style);
        if (viewport.Width <= 0 || viewport.Height <= 0)
        {
            return false;
        }
        Rect scrollFrame = scrollView.Frame;
        foreach (ScrollViewChildView childView in scrollView.ChildViews(new Size(scrollFrame.Width, scrollFrame.Height)))
        {
            if (!childView.Visible || childView.Element is null)
            {
                continue;
            }
            Rect? targetRect = GeneratedWindowRuntime.FocusRectWithinScrollChild(
                childView.Element,
                element,
                childView.Frame.Width,
                childView.Frame.Height);
            if (targetRect is null)
            {
                continue;
            }
            int targetTop = scrollFrame.Row + childView.Frame.Row + targetRect.Value.Row;
            int targetBottom = targetTop + Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, targetRect.Value.Height);
            return targetBottom > viewport.Row && targetTop < viewport.Row + viewport.Height;
        }
        return false;
    }

    private static (ReusableElement Proxy, ScrollView ScrollView)? ScrollViewFocusContextForElement(
        GeneratedWindowBase window,
        Element target)
    {
        (ReusableElement Proxy, ScrollView ScrollView)? context =
            ScrollViewFocusContextContainingElement(window, target);
        if (context.HasValue)
        {
            return context;
        }

        ScrollView? scrollView = ScrollViewContainingElement(window, target);
        if (scrollView is null)
        {
            return null;
        }
        ReusableElement? proxy =
            GeneratedWindowRuntime.ReusableContainingElement(window, scrollView) ??
            GeneratedWindowRuntime.ReusableContainingElement(window, target);
        return proxy is null ? null : (proxy, scrollView);
    }

    private static (ReusableElement Proxy, ScrollView ScrollView)? ScrollViewFocusContextContainingElement(
        GeneratedWindowBase window,
        Element target)
    {
        foreach (Element element in window.Elements)
        {
            (ReusableElement Proxy, ScrollView ScrollView)? found = ScrollViewFocusContextContainingElementInTree(element, target);
            if (found.HasValue)
            {
                return found;
            }
        }
        return null;
    }

    private static (ReusableElement Proxy, ScrollView ScrollView)? ScrollViewFocusContextContainingElementInTree(
        Element element,
        Element target)
    {
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            (ReusableElement Proxy, ScrollView ScrollView)? nested =
                ScrollViewFocusContextContainingElement(reusable.Child, target);
            if (nested.HasValue)
            {
                return nested;
            }
            if (reusable.Child is GeneratedScrollViewBase scrollView &&
                GeneratedWindowRuntime.WindowContainsElement(reusable.Child, target))
            {
                return (reusable, scrollView.ScrollView());
            }
        }
        foreach (Element child in element.Children)
        {
            (ReusableElement Proxy, ScrollView ScrollView)? nested =
                ScrollViewFocusContextContainingElementInTree(child, target);
            if (nested.HasValue)
            {
                return nested;
            }
        }
        return null;
    }

    private static ScrollView? ScrollViewContainingElement(GeneratedWindowBase window, Element target)
    {
        foreach (Element element in window.Elements)
        {
            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                ScrollView? nested = ScrollViewContainingElement(reusable.Child, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
            if (element is not ScrollView scrollView)
            {
                continue;
            }
            ScrollView? found = ScrollViewContainingElement(scrollView, target);
            if (found is not null)
            {
                return found;
            }
        }
        return null;
    }

    private static ScrollView? ScrollViewContainingElement(ScrollView scrollView, Element target)
    {
        foreach (Element child in scrollView.Children)
        {
            if (ReferenceEquals(child, target))
            {
                return scrollView;
            }
            if (child is ReusableElement reusable && reusable.Child is not null &&
                GeneratedWindowRuntime.WindowContainsElement(reusable.Child, target))
            {
                return scrollView;
            }
            if (child is ScrollView nestedScrollView)
            {
                ScrollView? nested = ScrollViewContainingElement(nestedScrollView, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
            ScrollView? descendant = ScrollViewContainingElementInTree(child, target);
            if (descendant is not null)
            {
                return descendant;
            }
        }
        return null;
    }

    private static ScrollView? ScrollViewContainingElementInTree(Element element, Element target)
    {
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            ScrollView? nested = ScrollViewContainingElement(reusable.Child, target);
            if (nested is not null)
            {
                return nested;
            }
        }
        foreach (Element child in element.Children)
        {
            if (child is ScrollView scrollView)
            {
                ScrollView? nested = ScrollViewContainingElement(scrollView, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
            ScrollView? descendant = ScrollViewContainingElementInTree(child, target);
            if (descendant is not null)
            {
                return descendant;
            }
        }
        return null;
    }

    private static bool CopyTextToClipboard(string text)
    {
        runtimeClipboardText = text;
        if (string.IsNullOrEmpty(text))
        {
            return false;
        }
        if (OperatingSystem.IsWindows())
        {
            return false;
        }
        foreach ((string FileName, string[] Arguments) command in ClipboardCommands())
        {
            if (RunClipboardCommand(command.FileName, command.Arguments, text))
            {
                return true;
            }
        }
        if (OperatingSystem.IsMacOS() && RunAppleScriptClipboardCommand(text))
        {
            return true;
        }
        return false;
    }

    private static IEnumerable<(string FileName, string[] Arguments)> ClipboardCommands()
    {
        yield return ("pbcopy", Array.Empty<string>());
        yield return ("wl-copy", Array.Empty<string>());
        yield return ("xclip", new[] { "-selection", "clipboard" });
        yield return ("xsel", new[] { "--clipboard", "--input" });
    }

    private static bool RunClipboardCommand(string fileName, IReadOnlyList<string> arguments, string text)
    {
        try
        {
            ProcessStartInfo startInfo = new()
            {
                FileName = fileName,
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
            };
            foreach (string argument in arguments)
            {
                startInfo.ArgumentList.Add(argument);
            }
            using Process? process = Process.Start(startInfo);
            if (process is null)
            {
                return false;
            }
            process.StandardInput.Write(text);
            process.StandardInput.Close();
            process.WaitForExit();
            return process.ExitCode == 0;
        }
        catch (Exception ex) when (ex is InvalidOperationException or IOException or System.ComponentModel.Win32Exception)
        {
            return false;
        }
    }

    private static bool RunAppleScriptClipboardCommand(string text)
    {
        try
        {
            ProcessStartInfo startInfo = new()
            {
                FileName = "osascript",
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
            };
            startInfo.ArgumentList.Add("-e");
            startInfo.ArgumentList.Add("on run argv");
            startInfo.ArgumentList.Add("-e");
            startInfo.ArgumentList.Add("set the clipboard to item 1 of argv");
            startInfo.ArgumentList.Add("-e");
            startInfo.ArgumentList.Add("end run");
            startInfo.ArgumentList.Add("--");
            startInfo.ArgumentList.Add(text);
            using Process? process = Process.Start(startInfo);
            if (process is null)
            {
                return false;
            }
            process.WaitForExit();
            return process.ExitCode == 0;
        }
        catch (Exception ex) when (ex is InvalidOperationException or IOException or System.ComponentModel.Win32Exception)
        {
            return false;
        }
    }

    private static bool CopyFocusedText(Element? focused)
    {
        if (focused is TextInput textInput)
        {
            string selectedText = textInput.SelectionText();
            string text = string.IsNullOrEmpty(selectedText) ? textInput.Value : selectedText;
            return !string.IsNullOrEmpty(text) && CopyTextToClipboard(text);
        }
        if (focused is NumberInput numberInput)
        {
            string text = numberInput.DisplayText();
            return !string.IsNullOrEmpty(text) && CopyTextToClipboard(text);
        }
        return false;
    }

    private static bool PasteIntoFocused(Element? focused, string text)
    {
        if (focused is TextInput textInput)
        {
            textInput.InsertText(text);
            return true;
        }
        if (focused is NumberInput numberInput)
        {
            foreach (char ch in text)
            {
                if (ch is '\r' or '\n')
                {
                    continue;
                }
                _ = numberInput.HandleKey(ch.ToString());
            }
            return true;
        }
        return false;
    }

    private JsonNode ToolPressKey(string key)
    {
        RenderContent();
        ClearLabelSelectionsInWindow(Current.Window);
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, Current.ActiveScrollView);
        Element? focused = FocusedElement();
        int focusedListIndex = focused is null ? Current.FocusedIndex : focusable.IndexOf(focused);
        if (focusedListIndex >= 0)
        {
            Current.FocusedIndex = focusedListIndex;
        }

        if (key == "cmd_c")
        {
            if (CopyFocusedText(focused))
            {
                copyNotificationRequested = true;
            }
            return ToolGetAccessibilitySnapshot();
        }
        if (key == "cmd_v")
        {
            if (focused is not null && !Current.EditMode && IsEditableElement(focused))
            {
                Current.EditSnapshot = CaptureSnapshot(focused);
                BeginElementEdit(focused);
                Current.EditMode = true;
                Current.Options.OnEditStarted?.Invoke(focused.Name);
            }
            if (PasteIntoFocused(focused, runtimeClipboardText) && focused is not null)
            {
                DispatchChanged(focused);
            }
            return ToolGetAccessibilitySnapshot();
        }

        if (Current.EditMode && key == "Escape")
        {
            if (Current.ActiveScrollView is not null)
            {
                if (Current.ActiveScrollViewEditElement is not null)
                {
                    if (!UsesLeaveCommit(Current.ActiveScrollViewEditElement) &&
                        Current.EditSnapshot is not null &&
                        ReferenceEquals(Current.EditSnapshot.Element, Current.ActiveScrollViewEditElement))
                    {
                        RestoreSnapshot(Current.EditSnapshot);
                    }
                    else
                    {
                        CommitEdit(Current.ActiveScrollViewEditElement);
                    }
                    Current.EditSnapshot = null;
                    Current.ActiveScrollViewEditElement = null;
                }
                else
                {
                    ExitScrollViewScope(Current);
                }
            }
            else
            {
                EscapeElementEdit(Current, focused);
                if (Current.Options.KeepEditModeAfterEscape && focused is not null && IsEditableElement(focused))
                {
                    Current.EditSnapshot = CaptureSnapshot(focused);
                    BeginElementEdit(focused);
                    Current.EditMode = true;
                    Current.Options.OnEditStarted?.Invoke(focused.Name);
                }
            }
            CloseCurrentWindowIfRequested();
            return FocusedElement() is Element escaped ? Snapshot(escaped) : new JsonObject { ["ok"] = true };
        }

        if (!Current.EditMode && key == "Escape" && Current.ActiveScrollView is not null)
        {
            ExitScrollViewScope(Current);
            return FocusedElement() is Element escapedScope ? Snapshot(escapedScope) : new JsonObject { ["ok"] = true };
        }

        if (key == "Escape" && HandleStandardEscapeButton(Current))
        {
            return FocusedElement() is Element escapedButton ? Snapshot(escapedButton) : new JsonObject { ["ok"] = true };
        }

        if (focused is not null &&
            Current.Options.OnKeyBeforeFocusedElement?.Invoke(key, focused.Name, Current.EditMode) == true)
        {
            CloseCurrentWindowIfRequested();
            return Snapshot(focused);
        }

        if (Current.ActiveScrollView is not null &&
            Current.ActiveScrollViewEditElement is null &&
            focused is not null)
        {
            if (IsDirectionalKey(key))
            {
                MoveScrollViewScopeFocus(key);
                return FocusedElement() is Element moved ? Snapshot(moved) : new JsonObject { ["ok"] = true };
            }
            if (key is "Enter" or " ")
            {
                focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, Current.ActiveScrollView);
                focused = FocusedElement();
                if (focused is not null && (IsButton(focused) || IsClickableImage(focused)))
                {
                    OptionsFor(focused).OnButton?.Invoke(focused.Name);
                }
                else if (focused is not null && IsImmediateInput(focused))
                {
                    int previousSelectionIndex = SelectedIndexOf(focused);
                    List<string>? previousSelectionValues = SelectionValuesForChange(focused);
                    focused.HandleKey(key);
                    DispatchChangedAfterHandledKey(focused, previousSelectionIndex, previousSelectionValues);
                }
                else if (focused is not null && IsEditableElement(focused))
                {
                    Current.EditSnapshot = CaptureSnapshot(focused);
                    BeginElementEdit(focused);
                    Current.ActiveScrollViewEditElement = focused;
                    Current.EditMode = true;
                    Current.Options.OnEditStarted?.Invoke(focused.Name);
                }
                CloseCurrentWindowIfRequested();
                return FocusedElement() is Element activated ? Snapshot(activated) : new JsonObject { ["ok"] = true };
            }
        }

        if (Current.EditMode && focused is not null)
        {
            if (Current.ActiveScrollView is not null)
            {
                if (Current.ActiveScrollViewEditElement is not null)
                {
                    if (key == "Enter")
                    {
                        int previousSelectionIndex = SelectedIndexOf(Current.ActiveScrollViewEditElement);
                        List<string>? previousSelectionValues = SelectionValuesForChange(Current.ActiveScrollViewEditElement);
                        Current.ActiveScrollViewEditElement.HandleKey(key);
                        DispatchChangedAfterHandledKey(
                            Current.ActiveScrollViewEditElement,
                            previousSelectionIndex,
                            previousSelectionValues);
                        if (Current.ActiveScrollViewEditElement is ListBox { Multiple: true })
                        {
                            return Snapshot(Current.ActiveScrollViewEditElement);
                        }
                        DispatchConfirmed(Current.ActiveScrollViewEditElement);
                        CommitEdit(Current.ActiveScrollViewEditElement);
                        Current.EditSnapshot = null;
                        Current.ActiveScrollViewEditElement = null;
                    }
                    else
                    {
                        int previousSelectionIndex = SelectedIndexOf(Current.ActiveScrollViewEditElement);
                        List<string>? previousSelectionValues = SelectionValuesForChange(Current.ActiveScrollViewEditElement);
                        Current.ActiveScrollViewEditElement.HandleKey(key);
                        DispatchChangedAfterHandledKey(
                            Current.ActiveScrollViewEditElement,
                            previousSelectionIndex,
                            previousSelectionValues);
                    }
                }
            }
            else if (key == "Enter" && focused is ListBox focusedListBox)
            {
                int previousSelectionIndex = SelectedIndexOf(focused);
                List<string>? previousSelectionValues = SelectionValuesForChange(focused);
                focused.HandleKey(key);
                DispatchChangedAfterHandledKey(focused, previousSelectionIndex, previousSelectionValues);
                if (focusedListBox.Multiple)
                {
                    Current.EditSnapshot = null;
                }
                else
                {
                    DispatchConfirmed(focused);
                    CommitEdit(focused);
                    Current.EditMode = Current.Options.KeepEditModeAfterConfirm && IsEditableElement(focused);
                    if (Current.EditMode)
                    {
                        Current.EditSnapshot = CaptureSnapshot(focused);
                        BeginElementEdit(focused);
                        Current.Options.OnEditStarted?.Invoke(focused.Name);
                    }
                    else
                    {
                        Current.EditSnapshot = null;
                    }
                }
            }
            else if (key == "Enter" && UsesLeaveCommit(focused))
            {
                int previousSelectionIndex = SelectedIndexOf(focused);
                List<string>? previousSelectionValues = SelectionValuesForChange(focused);
                focused.HandleKey(key);
                DispatchChangedAfterHandledKey(focused, previousSelectionIndex, previousSelectionValues);
                Current.EditSnapshot = null;
            }
            else if (key == "Enter" && !UsesLeaveCommit(focused))
            {
                int previousSelectionIndex = SelectedIndexOf(focused);
                List<string>? previousSelectionValues = SelectionValuesForChange(focused);
                focused.HandleKey(key);
                DispatchChangedAfterHandledKey(focused, previousSelectionIndex, previousSelectionValues);
                DispatchConfirmed(focused);
                CommitEdit(focused);
                Current.EditMode = Current.Options.KeepEditModeAfterConfirm && IsEditableElement(focused);
                if (Current.EditMode)
                {
                    Current.EditSnapshot = CaptureSnapshot(focused);
                    BeginElementEdit(focused);
                    Current.Options.OnEditStarted?.Invoke(focused.Name);
                }
                else
                {
                    Current.EditSnapshot = null;
                }
            }
            else
            {
                int previousSelectionIndex = SelectedIndexOf(focused);
                List<string>? previousSelectionValues = SelectionValuesForChange(focused);
                focused.HandleKey(key);
                DispatchChangedAfterHandledKey(focused, previousSelectionIndex, previousSelectionValues);
            }
            CloseCurrentWindowIfRequested();
            return FocusedElement() is Element edited ? Snapshot(edited) : new JsonObject { ["ok"] = true };
        }

        if (Current.Options.OnKey?.Invoke(key) == true)
        {
            CloseCurrentWindowIfRequested();
            return focused is null ? new JsonObject { ["ok"] = true } : Snapshot(focused);
        }

        if (key == "Tab" && focusable.Count > 0)
        {
            MoveFocusLinear(focusable, 1);
            return FocusedElement() is Element next ? Snapshot(next) : new JsonObject { ["ok"] = true };
        }
        if (key == "Shift+Tab" && focusable.Count > 0)
        {
            MoveFocusLinear(focusable, -1);
            return FocusedElement() is Element previous ? Snapshot(previous) : new JsonObject { ["ok"] = true };
        }
        if (IsDirectionalKey(key))
        {
            MoveFocusDirection(key);
            focused = FocusedElement();
            return focused is null ? new JsonObject { ["ok"] = true } : Snapshot(focused);
        }
        if (focused is null)
        {
            return new JsonObject { ["ok"] = true };
        }
        if (key is "Enter" or " ")
        {
            if (IsButton(focused) || IsClickableImage(focused))
            {
                string activatedName = focused.Name;
                GeneratedWindowBase activatedWindow = Current.Window;
                bool hadActiveStackFrame = IsButton(focused) && HasActiveStackFrame();
                BackgroundFocusCleanupContext cleanupContext = CaptureBackgroundFocusCleanupContext();
                if (IsButton(focused) && HasActiveStackFrame())
                {
                    DelayModalButtonActionForVisibleFocus();
                }
                Current.EditMode = false;
                OptionsFor(focused).OnButton?.Invoke(activatedName);
                if (hadActiveStackFrame && ActiveWindowIsRoot())
                {
                    CleanupBackgroundFocusAfterModalClose(cleanupContext);
                }
                if (!ActiveWindowIs(activatedWindow))
                {
                    return ActivatedElementResult(activatedName);
                }
            }
            else if (ActivateReusableControl(focused))
            {
                Current.EditMode = false;
            }
            else if (focused is ReusableElement && EnterScrollViewScope(focused))
            {
                Current.ActiveScrollViewEditElement = null;
                Current.EditSnapshot = null;
            }
            else if (IsImmediateInput(focused))
            {
                int previousSelectionIndex = SelectedIndexOf(focused);
                List<string>? previousSelectionValues = SelectionValuesForChange(focused);
                focused.HandleKey(key);
                DispatchChangedAfterHandledKey(focused, previousSelectionIndex, previousSelectionValues);
            }
            else if (IsEditableElement(focused))
            {
                if (!EnterScrollViewScope(focused))
                {
                    Current.EditSnapshot = CaptureSnapshot(focused);
                    BeginElementEdit(focused);
                    Current.EditMode = true;
                    Current.Options.OnEditStarted?.Invoke(focused.Name);
                }
                else
                {
                    Current.ActiveScrollViewEditElement = null;
                    Current.EditSnapshot = null;
                }
            }
            CloseCurrentWindowIfRequested();
            return Snapshot(focused);
        }
        if (IsImmediateInput(focused))
        {
            int previousSelectionIndex = SelectedIndexOf(focused);
            List<string>? previousSelectionValues = SelectionValuesForChange(focused);
            focused.HandleKey(key);
            DispatchChangedAfterHandledKey(focused, previousSelectionIndex, previousSelectionValues);
        }
        CloseCurrentWindowIfRequested();
        return new JsonObject { ["ok"] = true };
    }

    private void MoveFocusLinear(List<Element> focusable, int delta)
    {
        Element? focused = FocusedElement();
        if (focused is not null)
        {
            Current.Options.OnFocusChanged?.Invoke(focused.Name, false);
        }
        int currentIndex = focused is null ? Current.FocusedIndex : focusable.IndexOf(focused);
        Current.FocusedIndex = (currentIndex + delta + focusable.Count) % focusable.Count;
        Current.FocusedElementRef = focusable[Current.FocusedIndex];
        Current.EditMode = false;
        Current.EditScopeOwner = null;
        Current.ActiveScrollView = null;
        Current.ActiveScrollViewProxy = null;
        Current.ActiveScrollViewEditElement = null;
        Current.ActiveScrollViewFresh = false;
        Current.SuppressActiveScrollViewScopeVisuals = false;
        Current.EditSnapshot = null;
        Current.Options.OnFocusChanged?.Invoke(focusable[Current.FocusedIndex].Name, true);
        GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, focusable[Current.FocusedIndex]);
    }

    private static void ExitScrollViewScope(RuntimeFrame frame)
    {
        if (frame.ActiveScrollView is null)
        {
            frame.EditMode = false;
            frame.SuppressActiveScrollViewScopeVisuals = false;
            return;
        }

        ScrollView scrollView = frame.ActiveScrollView;
        List<Element> activeFocusable = GeneratedWindowRuntime.FocusableElements(frame.Window, scrollView);
        List<Element> scoped = ScrollViewScopeFocusableElements(frame, scrollView);
        if (frame.FocusedIndex >= 0 && frame.FocusedIndex < activeFocusable.Count)
        {
            Element focused = activeFocusable[frame.FocusedIndex];
            if (!ReferenceEquals(focused, scrollView) && scoped.Contains(focused))
            {
                frame.ScrollViewLastDescendant[scrollView] = focused;
            }
        }

        Element scopeRoot = frame.ActiveScrollViewProxy is not null ? frame.ActiveScrollViewProxy : scrollView;
        frame.ActiveScrollView = null;
        frame.ActiveScrollViewProxy = null;
        frame.ActiveScrollViewEditElement = null;
        frame.ActiveScrollViewFresh = false;
        frame.SuppressActiveScrollViewScopeVisuals = false;
        frame.EditSnapshot = null;
        frame.EditMode = false;
        frame.EditScopeOwner = null;
        List<Element> navigationFocusable = GeneratedWindowRuntime.FocusableElements(frame.Window);
        frame.FocusedIndex = navigationFocusable.IndexOf(scopeRoot);
        frame.FocusedElementRef = frame.FocusedIndex >= 0 ? navigationFocusable[frame.FocusedIndex] : null;
    }

    private bool MoveScrollViewScopeFocus(string key)
    {
        if (Current.ActiveScrollView is null)
        {
            return false;
        }

        ScrollView scrollView = Current.ActiveScrollView;
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, scrollView);
        Element? current = FocusedElement() ?? scrollView;
        List<Element> scoped = ScrollViewScopeFocusableElements();
        scoped.RemoveAll(element => ReferenceEquals(element, scrollView) || ReferenceEquals(element, Current.ActiveScrollViewProxy));
        if (scoped.Count == 0)
        {
            return key is "Up" or "Down" && scrollView.HandleKey(key);
        }

        int scopedIndex = scoped.IndexOf(current);
        int before = scopedIndex;
        MoveFocusSpatialIndex(ref scopedIndex, scoped, key);
        if (scopedIndex != before && scopedIndex >= 0 && scopedIndex < scoped.Count)
        {
            Element target = scoped[scopedIndex];
            int nextFocusIndex = focusable.IndexOf(target);
            if (nextFocusIndex >= 0)
            {
                Current.FocusedIndex = nextFocusIndex;
                Current.FocusedElementRef = target;
                Current.ScrollViewLastDescendant[scrollView] = target;
                GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, target);
                Current.ActiveScrollViewFresh = false;
                return true;
            }
        }

        if (key is not "Up" and not "Down" || !scrollView.HandleKey(key))
        {
            return false;
        }

        focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, scrollView);
        scoped = ScrollViewScopeFocusableElements();
        scoped.RemoveAll(element => ReferenceEquals(element, scrollView) || ReferenceEquals(element, Current.ActiveScrollViewProxy));
        if (scoped.Count == 0)
        {
            return true;
        }

        int nextIndex = scoped.IndexOf(current);
        if (nextIndex >= 0)
        {
            int beforeScrollMove = nextIndex;
            MoveFocusSpatialIndex(ref nextIndex, scoped, key);
            if (nextIndex == beforeScrollMove)
            {
                return true;
            }
        }
        else
        {
            nextIndex = key == "Down" ? 0 : scoped.Count - 1;
        }
        int index = nextIndex >= 0 && nextIndex < scoped.Count ? focusable.IndexOf(scoped[nextIndex]) : -1;
        if (index >= 0)
        {
            Element target = scoped[nextIndex];
            Current.FocusedIndex = index;
            Current.FocusedElementRef = target;
            Current.ScrollViewLastDescendant[scrollView] = target;
            GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, target);
        }
        Current.ActiveScrollViewFresh = false;
        return true;
    }

    private static void MoveFocusSpatialIndex(ref int focusedIndex, List<Element> focusable, string direction)
    {
        if (focusable.Count == 0)
        {
            focusedIndex = -1;
            return;
        }
        if (focusedIndex < 0 || focusedIndex >= focusable.Count)
        {
            focusedIndex = 0;
            return;
        }
        Element? target = DirectionalScrollViewScopeTarget(focusable[focusedIndex], focusable, direction);
        if (target is not null)
        {
            int nextIndex = focusable.IndexOf(target);
            if (nextIndex >= 0)
            {
                focusedIndex = nextIndex;
            }
        }
    }

    private List<Element> ScrollViewScopeFocusableElements()
    {
        if (Current.ActiveScrollView is null)
        {
            return new List<Element>();
        }
        return ScrollViewScopeFocusableElements(Current, Current.ActiveScrollView);
    }

    private static Element? DirectionalScrollViewScopeTarget(
        Element focused,
        IReadOnlyList<Element> candidates,
        string direction)
    {
        List<(int BandRank, int PrimaryGap, int PerpendicularGap, int EdgeDelta, int Order, Element Element)> scored = new();
        Rect currentRect = focused.Frame;
        for (int order = 0; order < candidates.Count; ++order)
        {
            Element candidate = candidates[order];
            if (ReferenceEquals(candidate, focused))
            {
                continue;
            }
            (int BandRank, int PrimaryGap, int PerpendicularGap, int EdgeDelta)? score =
                DirectionalFocusScore(currentRect, candidate.Frame, direction);
            if (score.HasValue)
            {
                scored.Add((
                    score.Value.BandRank,
                    score.Value.PrimaryGap,
                    score.Value.PerpendicularGap,
                    score.Value.EdgeDelta,
                    order,
                    candidate));
            }
        }
        return scored
            .OrderBy(candidate => candidate.BandRank)
            .ThenBy(candidate => candidate.PrimaryGap)
            .ThenBy(candidate => candidate.PerpendicularGap)
            .ThenBy(candidate => candidate.EdgeDelta)
            .ThenBy(candidate => candidate.Order)
            .Select(candidate => candidate.Element)
            .FirstOrDefault();
    }

    private void MoveFocusDirection(string direction)
    {
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, Current.ActiveScrollView);
        if (focusable.Count == 0)
        {
            Current.FocusedIndex = -1;
            Current.FocusedElementRef = null;
            return;
        }
        if (Current.FocusedElementRef is not null)
        {
            int refIndex = focusable.IndexOf(Current.FocusedElementRef);
            if (refIndex >= 0)
            {
                Current.FocusedIndex = refIndex;
            }
            else
            {
                Current.FocusedElementRef = null;
            }
        }
        if (Current.FocusedIndex < 0 || Current.FocusedIndex >= focusable.Count)
        {
            Current.FocusedIndex = 0;
            Current.FocusedElementRef = focusable[0];
            return;
        }

        Rect current = focusable[Current.FocusedIndex].Frame;
        int bestIndex = -1;
        int bestBand = 0;
        int bestPrimary = 0;
        int bestGap = 0;
        int bestCenter = 0;
        for (int index = 0; index < focusable.Count; ++index)
        {
            if (index == Current.FocusedIndex)
            {
                continue;
            }
            Element candidate = focusable[index];
            (int BandRank, int PrimaryGap, int PerpendicularGap, int EdgeDelta)? score =
                DirectionalFocusScore(current, candidate.Frame, direction);
            if (!score.HasValue)
            {
                continue;
            }

            int compare = score.Value.BandRank.CompareTo(bestBand);
            if (compare == 0)
            {
                compare = score.Value.PrimaryGap.CompareTo(bestPrimary);
            }
            if (compare == 0)
            {
                compare = score.Value.PerpendicularGap.CompareTo(bestGap);
            }
            if (compare == 0)
            {
                compare = score.Value.EdgeDelta.CompareTo(bestCenter);
            }
            if (bestIndex < 0 || compare < 0)
            {
                bestIndex = index;
                bestBand = score.Value.BandRank;
                bestPrimary = score.Value.PrimaryGap;
                bestGap = score.Value.PerpendicularGap;
                bestCenter = score.Value.EdgeDelta;
            }
        }

        if (bestIndex >= 0)
        {
            Current.FocusedIndex = bestIndex;
            Current.FocusedElementRef = focusable[bestIndex];
            GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, focusable[bestIndex]);
        }
    }

    private static bool IsDirectionalKey(string key)
    {
        return key is "Up" or "Down" or "Left" or "Right";
    }

    private static (int BandRank, int PrimaryGap, int PerpendicularGap, int EdgeDelta)? DirectionalFocusScore(
        Rect current,
        Rect candidate,
        string direction)
    {
        int currentTop = current.Row;
        int currentLeft = current.Col;
        int currentBottom = current.Row + current.Height;
        int currentRight = current.Col + current.Width;
        int candidateTop = candidate.Row;
        int candidateLeft = candidate.Col;
        int candidateBottom = candidate.Row + candidate.Height;
        int candidateRight = candidate.Col + candidate.Width;
        int currentCenterRow = CenterRow(current);
        int currentCenterCol = CenterCol(current);
        int candidateCenterRow = CenterRow(candidate);
        int candidateCenterCol = CenterCol(candidate);
        int primaryGap;
        int? bandRank;
        int perpendicularGap;
        int edgeDelta;
        if (direction == "Right")
        {
            if (candidateCenterCol <= currentCenterCol || candidateLeft < currentRight)
            {
                return null;
            }
            primaryGap = Math.Max(0, candidateLeft - currentRight);
            bandRank = BandScanRank(currentTop, currentBottom, candidateTop, candidateBottom);
            perpendicularGap = AxisGap(currentTop, currentBottom, candidateTop, candidateBottom);
            edgeDelta = Math.Abs(candidateTop - currentTop);
        }
        else if (direction == "Left")
        {
            if (candidateCenterCol >= currentCenterCol || candidateRight > currentLeft)
            {
                return null;
            }
            primaryGap = Math.Max(0, currentLeft - candidateRight);
            bandRank = BandScanRank(currentTop, currentBottom, candidateTop, candidateBottom);
            perpendicularGap = AxisGap(currentTop, currentBottom, candidateTop, candidateBottom);
            edgeDelta = Math.Abs(candidateTop - currentTop);
        }
        else if (direction == "Down")
        {
            if (candidateCenterRow <= currentCenterRow || candidateTop < currentBottom)
            {
                return null;
            }
            primaryGap = Math.Max(0, candidateTop - currentBottom);
            bandRank = BandScanRank(currentLeft, currentRight, candidateLeft, candidateRight);
            perpendicularGap = AxisGap(currentLeft, currentRight, candidateLeft, candidateRight);
            edgeDelta = Math.Abs(candidateLeft - currentLeft);
        }
        else if (direction == "Up")
        {
            if (candidateCenterRow >= currentCenterRow || candidateBottom > currentTop)
            {
                return null;
            }
            primaryGap = Math.Max(0, currentTop - candidateBottom);
            bandRank = BandScanRank(currentLeft, currentRight, candidateLeft, candidateRight);
            perpendicularGap = AxisGap(currentLeft, currentRight, candidateLeft, candidateRight);
            edgeDelta = Math.Abs(candidateLeft - currentLeft);
        }
        else
        {
            return null;
        }
        if (!bandRank.HasValue)
        {
            return null;
        }
        return (bandRank.Value, primaryGap, perpendicularGap, edgeDelta);
    }

    private static int AxisGap(int startA, int endA, int startB, int endB)
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

    private static int CenterRow(Rect rect)
    {
        return rect.Row + rect.Height / 2;
    }

    private static int CenterCol(Rect rect)
    {
        return rect.Col + rect.Width / 2;
    }

    private static bool AxisOverlaps(int startA, int endA, int startB, int endB)
    {
        return Math.Max(startA, startB) < Math.Min(endA, endB);
    }

    private static int? BandScanRank(int currentStart, int currentEnd, int candidateStart, int candidateEnd)
    {
        if (AxisOverlaps(currentStart, currentEnd, candidateStart, candidateEnd))
        {
            return 0;
        }
        int maxDistance =
            Math.Max(
                Math.Max(Math.Abs(candidateStart - currentStart), Math.Abs(candidateEnd - currentEnd)),
                Math.Max(Math.Abs(candidateStart - currentEnd), Math.Abs(candidateEnd - currentStart))) +
            Math.Max(1, currentEnd - currentStart) +
            Math.Max(1, candidateEnd - candidateStart);
        int rank = 1;
        for (int distance = 1; distance <= maxDistance; ++distance)
        {
            if (AxisOverlaps(currentStart + distance, currentEnd + distance, candidateStart, candidateEnd))
            {
                return rank;
            }
            ++rank;
            if (AxisOverlaps(currentStart - distance, currentEnd - distance, candidateStart, candidateEnd))
            {
                return rank;
            }
            ++rank;
        }
        return null;
    }

    private JsonNode ToolMouseClick(JsonObject arguments)
    {
        RenderContent();
        int x = arguments["x"]?.GetValue<int>() ?? 0;
        int y = arguments["y"]?.GetValue<int>() ?? 0;
        return ToolMouseClickAt(WindowPointFromTerminalPoint(new Point(y, x)));
    }

    private JsonNode ToolMouseDrag(JsonObject arguments)
    {
        RenderContent();
        Point from = WindowPointFromTerminalPoint(new Point(
            JsonInt(arguments, "from_y"),
            JsonInt(arguments, "from_x")));
        Point to = WindowPointFromTerminalPoint(new Point(
            JsonInt(arguments, "to_y"),
            JsonInt(arguments, "to_x")));
        _ = ToolMousePressAt(from);
        _ = ToolMouseDragAt(to);
        return ToolMouseReleaseAt(to);
    }

    private JsonNode ToolMousePress(JsonObject arguments)
    {
        RenderContent();
        return ToolMousePressAt(MousePointFromArguments(arguments));
    }

    private JsonNode ToolMouseMove(JsonObject arguments)
    {
        RenderContent();
        return ToolMouseDragAt(MousePointFromArguments(arguments));
    }

    private JsonNode ToolMouseRelease(JsonObject arguments)
    {
        RenderContent();
        return ToolMouseReleaseAt(MousePointFromArguments(arguments));
    }

    private Point MousePointFromArguments(JsonObject arguments)
    {
        return WindowPointFromTerminalPoint(new Point(JsonInt(arguments, "y"), JsonInt(arguments, "x")));
    }

    private JsonNode ToolMouseClickAt(Point position)
    {
        _ = ToolMousePressAt(position);
        return ToolMouseReleaseAt(position);
    }

    private JsonNode ToolMousePressAt(Point position)
    {
        RefreshCurrentWindowLayoutForMouse();
        if (Current.Options.OnMousePressBeforeFocused?.Invoke(position) == true)
        {
            return ToolGetAccessibilitySnapshot();
        }
        Element? focused = FocusedElement();
        bool focusedComboEditMode =
            Current.EditMode &&
            (Current.ActiveScrollView is null || ReferenceEquals(Current.ActiveScrollViewEditElement, focused));
        if (focused is ComboBox comboBox && focusedComboEditMode && ComboBoxDropDownFrame(comboBox).Contains(position))
        {
            int localRow = position.Row - comboBox.Frame.Row;
            int previousSelectionIndex = SelectedIndexOf(comboBox);
            if (localRow > 0)
            {
                int index = Math.Clamp(localRow - 1, 0, Math.Max(0, comboBox.Options.Count - 1));
                comboBox.SetSelectedIndex(index);
            }
            CommitEdit(comboBox);
            DispatchConfirmed(comboBox);
            Current.EditSnapshot = null;
            if (Current.ActiveScrollView is not null)
            {
                Current.ActiveScrollViewEditElement = null;
                ExitScrollViewScope(Current);
            }
            else
            {
                Current.EditMode = false;
            }
            DispatchChangedAfterHandledKey(comboBox, previousSelectionIndex);
            return Snapshot(comboBox);
        }
        ScrollView? mouseScrollView = Current.ActiveScrollView ?? ScrollViewAtPosition(Current.Window, position);
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, mouseScrollView);
        Element? target = MouseTargetElement(position, mouseScrollView);
        if (mouseScrollView is not null && target is not null && !ReferenceEquals(target, mouseScrollView))
        {
            Current.ActiveScrollView = mouseScrollView;
            (ReusableElement Proxy, ScrollView ScrollView)? context =
                ScrollViewFocusContextForElement(Current.Window, target);
            Current.ActiveScrollViewProxy = context?.Proxy;
            Current.EditMode = true;
            Current.SuppressActiveScrollViewScopeVisuals = false;
            Current.FocusedIndex = focusable.IndexOf(target);
            Current.FocusedElementRef = Current.FocusedIndex >= 0 ? focusable[Current.FocusedIndex] : null;
        }
        ClearLabelSelectionsInWindow(Current.Window);
        if (target is null && HandleLabelMousePress(Current.Window, position))
        {
            return ToolGetAccessibilitySnapshot();
        }
        if (target is null || !target.Enabled)
        {
            ClearFocusForEmptyMouseTarget();
            return ToolGetAccessibilitySnapshot();
        }
        bool scrollViewScopeActive = Current.EditMode && Current.ActiveScrollView is not null;
        if (scrollViewScopeActive &&
            Current.ActiveScrollViewEditElement is not null &&
            !ReferenceEquals(Current.ActiveScrollViewEditElement, target))
        {
            CommitEdit(Current.ActiveScrollViewEditElement);
            Current.ActiveScrollViewEditElement = null;
        }
        if (Current.EditMode &&
            !scrollViewScopeActive &&
            focused is not null &&
            !ReferenceEquals(focused, target))
        {
            CommitEdit(focused);
            Current.EditSnapshot = null;
            Current.EditMode = false;
        }
        if (target != focused)
        {
            if (scrollViewScopeActive)
            {
                _ = FocusElementWithScrollViewScope(target);
            }
            else
            {
                FocusElement(target);
            }
            focused = target;
        }
        scrollViewScopeActive = Current.EditMode && Current.ActiveScrollView is not null;
        if (IsButton(target))
        {
            return ActivateResolvedElement(target);
        }
        if (IsClickableImage(target))
        {
            return ActivateResolvedElement(target);
        }
        if (ActivateReusableControl(target))
        {
            return Snapshot(target);
        }
        if (target is CheckBox checkBox)
        {
            _ = checkBox.HandleKey("Enter");
            DispatchChanged(checkBox);
            return Snapshot(checkBox);
        }
        if (target is TextInput textInput)
        {
            Current.EditSnapshot = CaptureSnapshot(textInput);
            Current.EditMode = true;
            if (scrollViewScopeActive || mouseScrollView is not null)
            {
                Current.ActiveScrollViewEditElement = textInput;
            }
            int cursor = textInput.CursorForPoint(
                position.Row - textInput.Frame.Row,
                position.Col - textInput.Frame.Col,
                new Size(textInput.Frame.Width, textInput.Frame.Height));
            textInput.SelectRange(cursor, cursor);
            mouseSelectionElement = textInput;
            mouseSelectionAnchor = cursor;
            Current.Options.OnEditStarted?.Invoke(textInput.Name);
        }
        else if (target is NumberInput numberInput)
        {
            Current.EditSnapshot = CaptureSnapshot(numberInput);
            Current.EditMode = true;
            if (scrollViewScopeActive || mouseScrollView is not null)
            {
                Current.ActiveScrollViewEditElement = numberInput;
            }
            numberInput.SetEditCursor(
                position.Col - numberInput.Frame.Col,
                preserveReplaceOnFirstTextInput: numberInput.Value == 0.0);
            Current.Options.OnEditStarted?.Invoke(numberInput.Name);
        }
        else if (target is ComboBox targetComboBox)
        {
            Current.EditSnapshot = CaptureSnapshot(targetComboBox);
            Current.EditMode = true;
            if (scrollViewScopeActive || mouseScrollView is not null)
            {
                Current.ActiveScrollViewEditElement = targetComboBox;
            }
            Current.Options.OnEditStarted?.Invoke(targetComboBox.Name);
        }
        else if (target is ListBox targetListBox)
        {
            int optionIndex = targetListBox.ScrollOffset + position.Row - targetListBox.Frame.Row;
            if (optionIndex >= 0 && optionIndex < targetListBox.Options.Count)
            {
                List<string> previousValues = targetListBox.SelectedValues.ToList();
                if (targetListBox.Multiple)
                {
                    targetListBox.ToggleSelectedIndex(optionIndex);
                }
                else
                {
                    targetListBox.SetSelectedIndex(optionIndex);
                }
                targetListBox.SetActiveIndex(optionIndex);
                targetListBox.HideActiveItem();
                DispatchSelectionChangedIfDifferent(targetListBox, previousValues);
            }
            Current.EditSnapshot = CaptureSnapshot(targetListBox);
            Current.EditMode = true;
            if (scrollViewScopeActive || mouseScrollView is not null)
            {
                Current.ActiveScrollViewEditElement = targetListBox;
            }
            Current.Options.OnEditStarted?.Invoke(targetListBox.Name);
        }
        else
        {
            Current.EditMode = false;
        }
        return Snapshot(target);
    }

    private JsonNode ToolMouseDragAt(Point position)
    {
        if (mouseSelectionElement is TextInput textInput)
        {
            int localRow = position.Row - textInput.Frame.Row;
            int localCol = position.Col - textInput.Frame.Col;
            if (textInput is TextArea && textInput.Frame.Height > 1)
            {
                if (localRow < 0)
                {
                    _ = textInput.ScrollByRows(-1, textInput.Frame.Height, manual: false);
                    localRow = 0;
                }
                else if (localRow >= textInput.Frame.Height)
                {
                    _ = textInput.ScrollByRows(1, textInput.Frame.Height, manual: false);
                    localRow = textInput.Frame.Height - 1;
                }
            }
            int cursor = textInput.CursorForPoint(
                localRow,
                localCol,
                new Size(textInput.Frame.Width, textInput.Frame.Height));
            textInput.SelectRange(mouseSelectionAnchor, cursor);
            return Snapshot(textInput);
        }
        if (mouseSelectionElement is Label label)
        {
            int cursor = label.TextPositionFromPoint(
                position.Row - label.Frame.Row,
                position.Col - label.Frame.Col,
                new Size(label.Frame.Width, label.Frame.Height));
            label.SelectRange(mouseSelectionAnchor, cursor);
            return Snapshot(label);
        }
        return ToolGetAccessibilitySnapshot();
    }

    private JsonNode ToolMouseReleaseAt(Point position)
    {
        _ = position;
        Element? released = mouseSelectionElement;
        if (released is TextInput textInput)
        {
            string selectedText = textInput.SelectionText();
            if (!string.IsNullOrEmpty(selectedText))
            {
                _ = CopyTextToClipboard(selectedText);
                copyNotificationRequested = true;
            }
        }
        else if (released is Label label)
        {
            string selectedText = label.SelectedText();
            if (!string.IsNullOrEmpty(selectedText))
            {
                _ = CopyTextToClipboard(selectedText);
                copyNotificationRequested = true;
            }
        }
        mouseSelectionElement = null;
        mouseSelectionAnchor = 0;
        return released is not null ? Snapshot(released) : ToolGetAccessibilitySnapshot();
    }

    private bool HandleLabelMousePress(GeneratedWindowBase window, Point position)
    {
        foreach (Element element in window.Elements)
        {
            if (element is Label label)
            {
                if (!LabelAllowsTextSelection(label) || !label.Frame.Contains(position))
                {
                    continue;
                }
                int cursor = label.TextPositionFromPoint(
                    position.Row - label.Frame.Row,
                    position.Col - label.Frame.Col,
                    new Size(label.Frame.Width, label.Frame.Height));
                label.SelectRange(cursor, cursor);
                mouseSelectionElement = label;
                mouseSelectionAnchor = cursor;
                return true;
            }
            if (element is ReusableElement reusable && reusable.Child is not null &&
                HandleLabelMousePress(reusable.Child, position))
            {
                return true;
            }
        }
        return false;
    }

    private static bool LabelAllowsTextSelection(Label label)
    {
        return label.EffectiveStyle().UserSelect == "text";
    }

    private static void ClearLabelSelectionsInWindow(GeneratedWindowBase window)
    {
        foreach (Element element in window.Elements)
        {
            if (element is Label label)
            {
                label.ClearSelection();
                continue;
            }
            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                ClearLabelSelectionsInWindow(reusable.Child);
            }
        }
    }

    private JsonNode ToolMouseWheelAt(Point position, int wheelDelta)
    {
        RefreshCurrentWindowLayoutForMouse();
        Element? target = MouseTargetElement(position, Current.ActiveScrollView);
        if (target is ScrollView targetScrollView)
        {
            targetScrollView.HandleWheel(wheelDelta, new Size(targetScrollView.Frame.Width, targetScrollView.Frame.Height));
            QueueScrollRegionHint(targetScrollView);
            return Snapshot(targetScrollView);
        }
        if (target is ListBox listBox)
        {
            listBox.ScrollLines(wheelDelta);
            return Snapshot(listBox);
        }
        if (target is TextInput textInput)
        {
            int delta = (wheelDelta > 0 ? -Math.Abs(wheelDelta) : Math.Abs(wheelDelta)) *
                GeneratedWindowRuntime.TextInputWheelScrollRows;
            textInput.ScrollByRows(delta, textInput.Frame.Height);
            return Snapshot(textInput);
        }
        if (target is not null)
        {
            ScrollView? parentScrollView = ScrollViewContainingElement(Current.Window, target);
            if (parentScrollView is not null)
            {
                parentScrollView.HandleWheel(
                    wheelDelta,
                    new Size(parentScrollView.Frame.Width, parentScrollView.Frame.Height));
                QueueScrollRegionHint(parentScrollView);
                return Snapshot(parentScrollView);
            }
        }

        ScrollView? scrollView = ScrollViewAtPosition(Current.Window, position);
        if (scrollView is not null)
        {
            scrollView.HandleWheel(wheelDelta, new Size(scrollView.Frame.Width, scrollView.Frame.Height));
            QueueScrollRegionHint(scrollView);
            return Snapshot(scrollView);
        }
        return ToolGetAccessibilitySnapshot();
    }

    private static ScrollView? ScrollViewAtPosition(GeneratedWindowBase window, Point position)
    {
        for (int index = window.Elements.Count - 1; index >= 0; --index)
        {
            ScrollView? found = ScrollViewAtPosition(window.Elements[index], position);
            if (found is not null)
            {
                return found;
            }
        }
        return null;
    }

    private static ScrollView? ScrollViewAtPosition(Element element, Point position)
    {
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            ScrollView? child = reusable.Child is GeneratedScrollViewBase generatedScrollView &&
                reusable.Frame.Contains(position)
                    ? generatedScrollView.ScrollView()
                    : ScrollViewAtPosition(reusable.Child, position);
            if (child is not null)
            {
                return child;
            }
        }
        for (int index = element.Children.Count - 1; index >= 0; --index)
        {
            ScrollView? child = ScrollViewAtPosition(element.Children[index], position);
            if (child is not null)
            {
                return child;
            }
        }
        return element is ScrollView scrollView && scrollView.Frame.Contains(position) ? scrollView : null;
    }

    private static Rect ComboBoxDropDownFrame(ComboBox comboBox)
    {
        return new Rect(
            comboBox.Frame.Row,
            comboBox.Frame.Col,
            comboBox.Frame.Width,
            Math.Max(ComboBoxClosedRows, comboBox.Options.Count + ComboBoxClosedRows));
    }

    private Point WindowPointFromTerminalPoint(Point point)
    {
        Point viewportPoint = new(point.Row - config.ViewportRow, point.Col - config.ViewportCol);
        if (frames.Count <= 1)
        {
            return viewportPoint;
        }
        return GeneratedWindowRuntime.WindowStackContentPoint(Current.Window, ViewportSize(), viewportPoint);
    }

    private Element? MouseTargetElement(Point position, ScrollView? mouseScrollView)
    {
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, mouseScrollView);
        for (int index = focusable.Count - 1; index >= 0; --index)
        {
            Element element = focusable[index];
            Rect frame = element is ComboBox
                ? new Rect(element.Frame.Row, element.Frame.Col, element.Frame.Width, ComboBoxClosedRows)
                : element.Frame;
            if (frame.Contains(position))
            {
                return element;
            }
        }
        return null;
    }

    private void ClearFocusForEmptyMouseTarget()
    {
        Element? previous = FocusedElement();
        if (Current.EditMode)
        {
            Element? editElement = Current.ActiveScrollViewEditElement ?? previous;
            CommitEdit(editElement);
        }
        Current.EditMode = false;
        Current.EditScopeOwner = null;
        Current.ActiveScrollView = null;
        Current.ActiveScrollViewProxy = null;
        Current.ActiveScrollViewEditElement = null;
        Current.ActiveScrollViewFresh = false;
        Current.SuppressActiveScrollViewScopeVisuals = false;
        Current.EditSnapshot = null;
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window);
        if (focusable.Count <= 1)
        {
            return;
        }
        Current.FocusedIndex = -1;
        Current.FocusedElementRef = null;
        if (previous is not null)
        {
            Current.Options.OnFocusChanged?.Invoke(previous.Name, false);
        }
    }

    private void FocusElement(Element element)
    {
        RenderContent();
        GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, element);
        SetFocusInFrame(Current, element);
    }

    private JsonNode ToolScroll(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        int delta = arguments["delta"]?.GetValue<int>() ?? 0;
        Element target = RequireElement(elementId);
        ScrollView? scrollView = target as ScrollView;
        if (scrollView is null && target is ReusableElement reusable && reusable.Child is GeneratedScrollViewBase generatedScrollView)
        {
            scrollView = generatedScrollView.ScrollView();
        }
        if (scrollView is not null)
        {
            scrollView.ScrollLines(delta, new Size(scrollView.Frame.Width, scrollView.Frame.Height));
        }
        else if (target is ListBox listBox)
        {
            listBox.ScrollLines(delta);
        }
        else if (target is TextInput textInput)
        {
            int steps = Math.Max(1, Math.Abs(delta));
            int scrollDelta = delta < 0 ? 1 : -1;
            for (int index = 0; index < steps; ++index)
            {
                _ = textInput.ScrollByRows(
                    scrollDelta * GeneratedWindowRuntime.TextInputWheelScrollRows,
                    textInput.Frame.Height);
            }
        }
        return Snapshot(target);
    }

    private JsonNode ToolTypeText(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        string text = JsonText(arguments["text"] ?? arguments["value"]);
        Element target = string.IsNullOrEmpty(elementId) ? FocusedElement() ?? throw new InvalidOperationException("No focused element") : RequireElement(elementId);
        if (!string.IsNullOrEmpty(elementId))
        {
            _ = ToolFocusElement(elementId);
        }
        Current.EditMode = true;
        OptionsFor(target).OnEditStarted?.Invoke(target.Name);
        if (target is TextInput input)
        {
            foreach (char character in text)
            {
                input.InsertText(character.ToString());
                OptionsFor(input).OnTextChanged?.Invoke(input.Name, input.Value);
            }
        }
        else if (target is NumberInput number)
        {
            number.BeginEdit();
            foreach (char character in text)
            {
                _ = number.HandleKey(character.ToString());
                OptionsFor(number).OnTextChanged?.Invoke(
                    number.Name,
                    number.Value.ToString(System.Globalization.CultureInfo.InvariantCulture));
            }
        }
        else
        {
            throw new InvalidOperationException($"Element is not editable text: {target.Name}");
        }
        return Snapshot(target);
    }

    private JsonNode ToolSetText(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        string text = JsonText(arguments["text"] ?? arguments["value"]);
        Element target = RequireElement(elementId);
        _ = ToolFocusElement(elementId);
        if (target is TextInput or NumberInput)
        {
            Current.EditMode = true;
            OptionsFor(target).OnEditStarted?.Invoke(target.Name);
        }
        if (target is TextInput input)
        {
            input.SetValue(text);
            input.SetCursor(text.Length);
            OptionsFor(input).OnTextChanged?.Invoke(input.Name, input.Value);
        }
        else if (target is NumberInput number)
        {
            double parsed = double.TryParse(text, System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture, out double value)
                ? value
                : 0.0;
            number.SetEditText("");
            foreach (char character in text)
            {
                _ = number.HandleKey(character.ToString());
                OptionsFor(number).OnTextChanged?.Invoke(
                    number.Name,
                    number.Value.ToString(System.Globalization.CultureInfo.InvariantCulture));
            }
            number.CommitEdit();
            number.SetValue(parsed);
            number.SetEditText(text);
            OptionsFor(number).OnTextChanged?.Invoke(number.Name, number.Value.ToString(System.Globalization.CultureInfo.InvariantCulture));
        }
        else if (target is Label label)
        {
            label.SetText(text);
        }
        return Snapshot(target);
    }

    private JsonNode ToolClearText(string elementId)
    {
        Element target = RequireElement(elementId);
        if (target is TextInput input)
        {
            input.SetValue("");
            OptionsFor(input).OnTextChanged?.Invoke(input.Name, input.Value);
        }
        return Snapshot(target);
    }

    private JsonNode ToolSetValue(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        JsonNode? value = arguments["value"];
        Element target = RequireElement(elementId);
        if (target is TextInput input)
        {
            input.SetValue(JsonText(value));
            OptionsFor(input).OnTextChanged?.Invoke(input.Name, input.Value);
        }
        else if (target is NumberInput number)
        {
            number.SetValue(JsonDouble(value));
            OptionsFor(number).OnTextChanged?.Invoke(number.Name, number.Value.ToString(System.Globalization.CultureInfo.InvariantCulture));
        }
        else if (target is CheckBox checkBox)
        {
            checkBox.SetChecked(value?.GetValue<bool>() ?? false);
            OptionsFor(checkBox).OnTextChanged?.Invoke(checkBox.Name, checkBox.Checked ? "true" : "false");
        }
        else if (target is ComboBox comboBox)
        {
            string text = JsonText(value);
            int index = comboBox.Options.IndexOf(text);
            if (index >= 0)
            {
                comboBox.SetSelectedIndex(index);
                OptionsFor(comboBox).OnSelectionChanged?.Invoke(comboBox.Name, new List<string> { comboBox.SelectedText });
            }
        }
        else if (target is ListBox listBox)
        {
            if (value is JsonArray values)
            {
                SetListBoxSelection(listBox, values);
            }
            else
            {
                string text = JsonText(value);
                int index = listBox.Options.IndexOf(text);
                if (index >= 0)
                {
                    if (listBox.Multiple)
                    {
                        listBox.SetSelectedValues(new[] { text });
                    }
                    else
                    {
                        listBox.SetSelectedIndex(index);
                    }
                }
            }
            OptionsFor(listBox).OnSelectionChanged?.Invoke(listBox.Name, listBox.SelectedValues.ToList());
        }
        return Snapshot(target);
    }

    private JsonNode ToolSetSelection(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        Element target = RequireElement(elementId);
        if (target is not ListBox listBox)
        {
            throw new InvalidOperationException("Element is not a listbox: " + elementId);
        }
        if (arguments["values"] is not JsonArray values)
        {
            throw new InvalidOperationException("ListBox selection values must be an array: " + elementId);
        }
        SetListBoxSelection(listBox, values);
        OptionsFor(listBox).OnSelectionChanged?.Invoke(listBox.Name, listBox.SelectedValues.ToList());
        return Snapshot(listBox);
    }

    private static void SetListBoxSelection(ListBox listBox, JsonArray values)
    {
        List<string> requestedValues = values.Select(value => JsonText(value)).ToList();
        if (!listBox.Multiple && requestedValues.Count > 1)
        {
            throw new InvalidOperationException("ListBox is not multi-select: " + listBox.Name);
        }
        foreach (string value in requestedValues)
        {
            if (!listBox.Options.Contains(value))
            {
                throw new InvalidOperationException("Unknown option for " + listBox.Name + ": " + value);
            }
        }
        listBox.SetSelectedValues(listBox.Options.Where(requestedValues.Contains));
    }

    private JsonNode ToolSetChecked(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        bool value = arguments["checked"]?.GetValue<bool>() ?? arguments["value"]?.GetValue<bool>() ?? false;
        return ToolSetChecked(arguments, value);
    }

    private JsonNode ToolSetChecked(JsonObject arguments, bool value)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        Element target = RequireElement(elementId);
        if (target is CheckBox checkBox)
        {
            checkBox.SetChecked(value);
            OptionsFor(checkBox).OnTextChanged?.Invoke(checkBox.Name, checkBox.Checked ? "true" : "false");
        }
        return Snapshot(target);
    }

    private JsonNode ToolSetCursor(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        int offset = arguments["offset"]?.GetValue<int>() ?? 0;
        Element target = RequireElement(elementId);
        if (target is TextInput input)
        {
            input.SetCursor(offset);
        }
        return Snapshot(target);
    }

    private JsonNode ToolSelectText(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        int start = arguments["start"]?.GetValue<int>() ?? 0;
        int end = arguments["end"]?.GetValue<int>() ?? start;
        Element target = RequireElement(elementId);
        if (target is TextInput input)
        {
            bool editingTarget = Current.EditMode && ReferenceEquals(FocusedElement(), target);
            if (!editingTarget)
            {
                FocusElement(target);
                Current.EditSnapshot = CaptureSnapshot(target);
                BeginElementEdit(target);
                Current.EditMode = true;
                if (Current.ActiveScrollView is not null)
                {
                    Current.ActiveScrollViewEditElement = target;
                }
                Current.Options.OnEditStarted?.Invoke(target.Name);
            }
            input.SetSelection(start, end);
            return new JsonObject
            {
                ["start"] = input.SelectionStart,
                ["end"] = input.SelectionEnd,
                ["text"] = input.SelectionText(),
            };
        }
        if (target is Label label)
        {
            label.SelectRange(start, end);
            return new JsonObject
            {
                ["start"] = start,
                ["end"] = end,
                ["text"] = label.SelectedText(),
            };
        }
        return Snapshot(target);
    }

    private JsonNode ToolCopySelection(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        Element target = RequireElement(elementId);
        return new JsonObject
        {
            ["text"] = target switch
            {
                TextInput input => input.SelectionText(),
                Label label => label.SelectedText(),
                _ => "",
            },
        };
    }

    private JsonNode ToolReplaceSelection(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        string text = JsonText(arguments["text"]);
        Element target = RequireElement(elementId);
        if (target is TextInput input)
        {
            input.InsertText(text);
            OptionsFor(input).OnTextChanged?.Invoke(input.Name, input.Value);
        }
        return Snapshot(target);
    }

    private JsonNode ToolSelectOption(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        string value = JsonText(arguments["value"]);
        Element target = RequireElement(elementId);
        if (target is ComboBox comboBox)
        {
            int index = comboBox.Options.IndexOf(value);
            if (index >= 0)
            {
                comboBox.SetSelectedIndex(index);
                OptionsFor(comboBox).OnSelectionChanged?.Invoke(comboBox.Name, new List<string> { comboBox.SelectedText });
            }
        }
        else if (target is ListBox listBox)
        {
            int index = listBox.Options.IndexOf(value);
            if (index >= 0)
            {
                if (listBox.Multiple)
                {
                    listBox.SetSelectedValues(new[] { value });
                }
                else
                {
                    listBox.SetSelectedIndex(index);
                }
                OptionsFor(listBox).OnSelectionChanged?.Invoke(listBox.Name, listBox.SelectedValues.ToList());
            }
        }
        return Snapshot(target);
    }

    private void DispatchChanged(Element element)
    {
        switch (element)
        {
            case TextInput input:
                OptionsFor(input).OnTextChanged?.Invoke(input.Name, input.Value);
                break;
            case NumberInput number:
                OptionsFor(number).OnTextChanged?.Invoke(number.Name, number.Value.ToString(System.Globalization.CultureInfo.InvariantCulture));
                break;
            case CheckBox checkBox:
                OptionsFor(checkBox).OnTextChanged?.Invoke(checkBox.Name, checkBox.Checked ? "true" : "false");
                break;
            case ComboBox comboBox:
                OptionsFor(comboBox).OnSelectionChanged?.Invoke(comboBox.Name, new List<string> { comboBox.SelectedText });
                break;
            case ListBox listBox:
                OptionsFor(listBox).OnSelectionChanged?.Invoke(listBox.Name, listBox.SelectedValues.ToList());
                break;
        }
    }

    private static List<string>? SelectionValuesForChange(Element element)
    {
        return element is ListBox listBox ? listBox.SelectedValues.ToList() : null;
    }

    private void DispatchChangedAfterHandledKey(
        Element element,
        int previousSelectionIndex,
        List<string>? previousSelectionValues = null)
    {
        if (element is ListBox listBox && previousSelectionValues is not null)
        {
            if (!previousSelectionValues.SequenceEqual(listBox.SelectedValues))
            {
                DispatchChanged(element);
            }
            return;
        }
        if (element is ComboBox or ListBox)
        {
            if (SelectedIndexOf(element) != previousSelectionIndex)
            {
                DispatchChanged(element);
            }
            return;
        }
        DispatchChanged(element);
    }

    private void DispatchSelectionChangedIfDifferent(ListBox listBox, List<string> previousValues)
    {
        if (!previousValues.SequenceEqual(listBox.SelectedValues))
        {
            DispatchChanged(listBox);
        }
    }

    private void DispatchConfirmed(Element element)
    {
        if (element is not TextInput and not NumberInput and not ComboBox and not ListBox)
        {
            return;
        }
        OptionsFor(element).OnTextConfirmed?.Invoke(element.Name, TextValueOf(element));
    }

    private static string TextValueOf(Element element)
    {
        return element switch
        {
            TextInput input => input.Value,
            NumberInput number => number.Value.ToString(System.Globalization.CultureInfo.InvariantCulture),
            ComboBox comboBox => comboBox.SelectedText,
            ListBox listBox => listBox.SelectedValues.FirstOrDefault() ?? "",
            CheckBox checkBox => checkBox.Checked ? "true" : "false",
            _ => "",
        };
    }

    private GeneratedWindowRuntimeOptions OptionsFor(Element element)
    {
        GeneratedWindowBase owner = OwnerWindowFor(Current.Window, element) ?? Current.Window;
        if (owner == Current.Window)
        {
            return Current.Options;
        }
        owner.AttachRuntimeWindowStack(OpenWindow, CloseWindow, SetRuntimeWindowFocus);
        return owner.RuntimeOptions();
    }

    private static GeneratedWindowBase? OwnerWindowFor(GeneratedWindowBase window, Element target)
    {
        foreach (Element element in window.Elements)
        {
            if (ReferenceEquals(element, target))
            {
                return window;
            }
            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                GeneratedWindowBase? nested = OwnerWindowFor(reusable.Child, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
            if (element is ScrollView scrollView)
            {
                foreach (Element child in scrollView.Children)
                {
                    if (child is ReusableElement childReusable && childReusable.Child is not null)
                    {
                        GeneratedWindowBase? nested = OwnerWindowFor(childReusable.Child, target);
                        if (nested is not null)
                        {
                            return nested;
                        }
                    }
                }
            }
            foreach (Element child in element.Children)
            {
                GeneratedWindowBase? nested = OwnerWindowForElementTree(window, child, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
        }
        return null;
    }

    private static GeneratedWindowBase? OwnerWindowForElementTree(GeneratedWindowBase owner, Element element, Element target)
    {
        if (ReferenceEquals(element, target))
        {
            return owner;
        }
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            GeneratedWindowBase? nested = OwnerWindowFor(reusable.Child, target);
            if (nested is not null)
            {
                return nested;
            }
        }
        foreach (Element child in element.Children)
        {
            GeneratedWindowBase? nested = OwnerWindowForElementTree(owner, child, target);
            if (nested is not null)
            {
                return nested;
            }
        }
        return null;
    }

    private static List<Element> DescendantFocusableElements(Element owner)
    {
        List<Element> result = new();
        if (owner is ReusableElement reusable && reusable.Child is not null)
        {
            result.AddRange(GeneratedWindowRuntime.FocusableElements(reusable.Child));
        }
        foreach (Element child in owner.Children)
        {
            AddDescendantFocusable(child, result);
        }
        return result;
    }

    private static void AddDescendantFocusable(Element element, List<Element> result)
    {
        if (GeneratedWindowRuntime.IsFocusable(element))
        {
            result.Add(element);
        }
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            result.AddRange(GeneratedWindowRuntime.FocusableElements(reusable.Child));
        }
        foreach (Element child in element.Children)
        {
            AddDescendantFocusable(child, result);
        }
    }

    private List<List<TerminalCell>> RenderContent()
    {
        return RenderContent(Current, new Size(config.ViewportWidth, config.ViewportHeight));
    }

    private List<List<TerminalCell>> RenderContent(RuntimeFrame frame, Size viewportSize)
    {
        return RenderContent(frame, viewportSize, false);
    }

    private List<List<TerminalCell>> RenderContent(RuntimeFrame frame, Size viewportSize, bool backgroundFrame)
    {
        ScrollView? renderActiveScrollView = frame.ActiveScrollView;
        ReusableElement? renderActiveScrollViewProxy = frame.ActiveScrollViewProxy;
        bool renderActiveScrollViewFresh = frame.ActiveScrollViewFresh;
        bool renderEditMode = frame.EditMode;
        if (backgroundFrame)
        {
            renderActiveScrollView ??= frame.PendingModalScrollView;
            renderActiveScrollViewProxy ??= frame.PendingModalScrollViewProxy;
            renderActiveScrollViewFresh = false;
            renderEditMode = renderActiveScrollView is not null && frame.EditMode;
        }

        (int focusedIndex, Element? focused) = FocusedElementForRender(
            frame,
            renderActiveScrollView,
            !backgroundFrame);
        return GeneratedWindowRuntime.RenderViewportContent(
            frame.Window,
            viewportSize,
            focusedIndex,
            focused,
            renderEditMode,
            renderActiveScrollView,
            renderActiveScrollViewProxy,
            !backgroundFrame ? frame.ActiveScrollViewEditElement : null,
            renderActiveScrollViewFresh,
            !backgroundFrame,
            backgroundFrame || frame.SuppressActiveScrollViewScopeVisuals);
    }

    private List<List<TerminalCell>> RenderFrameContent(Size viewportSize)
    {
        viewportSize = new Size(
            Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, viewportSize.Width),
            Math.Max(GeneratedWindowRuntime.MinimumRenderableSize, viewportSize.Height));
        if (frames.Count == 0)
        {
            return new List<List<TerminalCell>>();
        }

        return RenderFrameContentCore(viewportSize);
    }

    private List<List<TerminalCell>> RenderFrameContentCore(Size viewportSize)
    {
        List<List<TerminalCell>> content;
        if (frames.Count > 1)
        {
            using IDisposable imageCellBackgroundRendering = Image.ForceCellBackgroundRendering();
            content = RenderContent(frames[0], viewportSize, true);
        }
        else
        {
            content = RenderContent(frames[0], viewportSize, false);
        }
        if (frames.Count == 1)
        {
            return content;
        }

        TerminalBuffer buffer = new(viewportSize.Width, viewportSize.Height);
        for (int row = 0; row < content.Count && row < buffer.Height; ++row)
        {
            for (int col = 0; col < content[row].Count && col < buffer.Width; ++col)
            {
                buffer.SetCell(row, col, content[row][col].Clone());
            }
        }

        if (frames.Any(frame => frame.Options.DimBackground))
        {
            GeneratedWindowRuntime.DimBufferForModalOverlay(buffer);
        }
        for (int index = 1; index < frames.Count; ++index)
        {
            RuntimeFrame frame = frames[index];
            bool topFrame = index == frames.Count - 1;
            bool dimFrame = !topFrame && frames
                .Skip(index + 1)
                .Any(laterFrame => laterFrame.Options.DimBackground);
            ScrollView? renderActiveScrollView = topFrame ? frame.ActiveScrollView : null;
            ReusableElement? renderActiveScrollViewProxy = topFrame ? frame.ActiveScrollViewProxy : null;
            bool renderActiveScrollViewFresh = topFrame && frame.ActiveScrollViewFresh;
            (int focusedIndex, Element? focused) = FocusedElementForRender(
                frame,
                renderActiveScrollView,
                topFrame);
            GeneratedWindowRuntime.RenderWindowStackOverlay(
                buffer,
                frame.Window,
                frame.Options,
                focusedIndex,
                focused,
                frame.EditMode,
                renderActiveScrollView,
                renderActiveScrollViewProxy,
                topFrame ? frame.ActiveScrollViewEditElement : null,
                renderActiveScrollViewFresh,
                topFrame,
                dimFrame);
        }

        return GeneratedWindowRuntime.ContentFromBuffer(buffer);
    }

    private Element? FocusedElement()
    {
        return FocusedElement(Current);
    }

    private static Element? FocusedElement(RuntimeFrame frame)
    {
        return FocusedElementForRender(frame, frame.ActiveScrollView, true).FocusedElement;
    }

    private static (int FocusedIndex, Element? FocusedElement) FocusedElementForRender(
        RuntimeFrame frame,
        ScrollView? activeScrollView,
        bool updateFrame)
    {
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(frame.Window, activeScrollView);
        if (frame.FocusedElementRef is not null)
        {
            int refIndex = focusable.IndexOf(frame.FocusedElementRef);
            if (refIndex >= 0)
            {
                if (updateFrame)
                {
                    frame.FocusedIndex = refIndex;
                }
                return (refIndex, frame.FocusedElementRef);
            }
            if (updateFrame)
            {
                frame.FocusedElementRef = null;
                frame.FocusedIndex = -1;
            }
            return (-1, null);
        }
        if (frame.FocusedIndex >= 0 && frame.FocusedIndex < focusable.Count)
        {
            Element focused = focusable[frame.FocusedIndex];
            if (updateFrame)
            {
                frame.FocusedElementRef = focused;
            }
            return (frame.FocusedIndex, focused);
        }
        if (updateFrame)
        {
            frame.FocusedElementRef = null;
        }
        return (-1, null);
    }

    private Element RequireElement(string elementId)
    {
        return GeneratedWindowRuntime.FindElement(Current.Window, elementId) ??
            throw new InvalidOperationException("Unknown element: " + elementId);
    }

    private bool Exposed(Element element)
    {
        return Current.Window.McpElementMetadata(element.Name)?.Expose ?? true;
    }

    private JsonObject Snapshot(Element element)
    {
        string id = ElementSnapshotId(element);
        JsonObject result = new()
        {
            ["id"] = id,
            ["name"] = element.Name,
            ["type"] = ElementType(element),
            ["role"] = RoleFor(element),
            ["description"] = Current.Window.McpElementMetadata(element.Name)?.Description ?? element.Name,
            ["value"] = ElementValue(element),
            ["focused"] = FocusedElement() == element,
            ["enabled"] = element.Enabled,
            ["visible"] = true,
            ["rect"] = new JsonObject
            {
                ["top"] = element.Frame.Row,
                ["left"] = element.Frame.Col,
                ["width"] = element.Frame.Width,
                ["height"] = element.Frame.Height,
            },
        };
        if (element is TextInput input)
        {
            result["cursor"] = input.Cursor;
            if (input.SelectionStart.HasValue && input.SelectionEnd.HasValue)
            {
                result["selection"] = new JsonObject
                {
                    ["start"] = input.SelectionStart,
                    ["end"] = input.SelectionEnd,
                    ["text"] = input.SelectionText(),
                };
            }
        }
        return result;
    }

    private string ElementSnapshotId(Element element)
    {
        return ElementPath(Current.Window, element) ?? element.Name;
    }

    private static string? ElementPath(GeneratedWindowBase window, Element target)
    {
        foreach (Element element in window.Elements)
        {
            if (ReferenceEquals(element, target))
            {
                return element.Name;
            }

            if (element is ReusableElement reusable && reusable.Child is not null)
            {
                if (reusable.Child is GeneratedScrollViewBase generatedScrollView)
                {
                    ScrollView scrollView = generatedScrollView.ScrollView();
                    if (ReferenceEquals(scrollView, target))
                    {
                        return reusable.Name;
                    }

                    string? generatedScrollViewPath = ScrollViewChildElementPath(
                        scrollView,
                        target,
                        reusable.Name);
                    if (generatedScrollViewPath is not null)
                    {
                        return generatedScrollViewPath;
                    }
                }

                string? nested = ElementPath(reusable.Child, target);
                if (nested is not null)
                {
                    return reusable.Name + "." + nested;
                }
            }

            if (element is ScrollView scrollViewElement)
            {
                string? scrollViewPath = ScrollViewChildElementPath(
                    scrollViewElement,
                    target,
                    scrollViewElement.Name);
                if (scrollViewPath is not null)
                {
                    return scrollViewPath;
                }
            }
        }
        return null;
    }

    private static string? ScrollViewChildElementPath(
        ScrollView scrollView,
        Element target,
        string prefix)
    {
        for (int index = 0; index < scrollView.Children.Count; ++index)
        {
            Element child = scrollView.Children[index];
            if (ReferenceEquals(child, target))
            {
                return prefix + "[" + index.ToString(CultureInfo.InvariantCulture) + "]." + child.Name;
            }

            if (child is ReusableElement reusable && reusable.Child is not null)
            {
                string? nested = ElementPath(reusable.Child, target);
                if (nested is not null)
                {
                    return prefix + "[" + index.ToString(CultureInfo.InvariantCulture) + "]." + nested;
                }
            }
        }

        return null;
    }

    private static JsonNode? ElementValue(Element element)
    {
        return element switch
        {
            Label label => label.Text,
            Button button => button.Title,
            CheckBox checkBox => checkBox.Checked,
            TextInput input => input.Value,
            NumberInput number => number.Value,
            ComboBox comboBox => comboBox.SelectedText,
            ListBox listBox => new JsonArray(listBox.SelectedValues.Select(value => JsonValue.Create(value)).ToArray<JsonNode?>()),
            Image image => image.Source,
            _ => null,
        };
    }

    private static string ElementType(Element element)
    {
        return element switch
        {
            MessageTable => "messagetable",
            InfoLabel => "infolabel",
            Label => "label",
            Button => "button",
            CheckBox => "checkbox",
            TextArea => "textarea",
            TextInput => "textinput",
            NumberInput => "numberinput",
            ComboBox => "combobox",
            ListBox => "listbox",
            ScrollView => "uiscrollview",
            ReusableElement => "uielement",
            Image => "image",
            FrameBufferView => "framebufferview",
            _ => "element",
        };
    }

    private static string RoleFor(Element element)
    {
        return element switch
        {
            TextInput or NumberInput => "input",
            Button or Image => "action",
            CheckBox => "toggle",
            ComboBox or ListBox => "selection",
            _ => "text",
        };
    }

    private static long? JsonInt64(JsonObject arguments, string name)
    {
        JsonNode? node = arguments[name];
        if (node is null)
        {
            return null;
        }
        try
        {
            return node.GetValue<long>();
        }
        catch (InvalidOperationException)
        {
            string text = node.GetValue<string>();
            return long.TryParse(text, out long value) ? value : null;
        }
        catch (FormatException)
        {
            string text = node.GetValue<string>();
            return long.TryParse(text, out long value) ? value : null;
        }
    }

    private static int JsonInt(JsonObject arguments, string name)
    {
        JsonNode? node = arguments[name];
        if (node is null)
        {
            return 0;
        }
        try
        {
            return node.GetValue<int>();
        }
        catch (InvalidOperationException)
        {
            string text = node.GetValue<string>();
            return int.TryParse(text, out int value) ? value : 0;
        }
        catch (FormatException)
        {
            string text = node.GetValue<string>();
            return int.TryParse(text, out int value) ? value : 0;
        }
    }

    private static string JsonString(JsonObject arguments, string name, string fallback = "")
    {
        JsonNode? node = arguments[name];
        if (node is null)
        {
            return fallback;
        }
        try
        {
            return node.GetValue<string>();
        }
        catch (InvalidOperationException)
        {
            return node.ToJsonString(new JsonSerializerOptions { WriteIndented = false });
        }
    }

    private static string JsonText(JsonNode? node, string fallback = "")
    {
        if (node is null)
        {
            return fallback;
        }
        if (node is JsonValue value)
        {
            if (value.TryGetValue<string>(out string? text))
            {
                return text ?? fallback;
            }
            if (value.TryGetValue<long>(out long integer))
            {
                return integer.ToString(System.Globalization.CultureInfo.InvariantCulture);
            }
            if (value.TryGetValue<double>(out double number))
            {
                return number.ToString(System.Globalization.CultureInfo.InvariantCulture);
            }
            if (value.TryGetValue<bool>(out bool boolean))
            {
                return boolean ? "true" : "false";
            }
        }
        return node.ToJsonString(new JsonSerializerOptions { WriteIndented = false });
    }

    private static double JsonDouble(JsonNode? node, double fallback = 0.0)
    {
        if (node is null)
        {
            return fallback;
        }
        if (node is JsonValue value)
        {
            if (value.TryGetValue<double>(out double number))
            {
                return number;
            }
            if (value.TryGetValue<long>(out long integer))
            {
                return integer;
            }
            if (value.TryGetValue<string>(out string? text) &&
                double.TryParse(text, System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture, out double parsed))
            {
                return parsed;
            }
        }
        return fallback;
    }

    private static JsonObject JsonRpcError(JsonNode? id, string message)
    {
        return new JsonObject
        {
            ["jsonrpc"] = "2.0",
            ["id"] = id,
            ["error"] = new JsonObject { ["code"] = -32000, ["message"] = message },
        };
    }
}
