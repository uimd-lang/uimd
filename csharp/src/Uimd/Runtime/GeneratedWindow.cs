using System.Net;
using System.Net.Sockets;
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
    public bool KeepEditModeAfterConfirm { get; set; }
    public bool KeepEditModeAfterEscape { get; set; }
    public Action<string>? OnButton { get; set; }
    public Func<string, string, bool, bool>? OnKeyBeforeFocusedElement { get; set; }
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

public static class GeneratedWindowRuntime
{
    private const int MinimumRenderableSize = 1;
    private const int FallbackTerminalWidth = 100;
    private const int FallbackTerminalHeight = 32;
    private static readonly Color DefaultTerminalBackground = new("#000000");

    private enum GeneratedWindowMode
    {
        Normal,
        ExpandWidth,
        ExpandHeight,
        Fullscreen,
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
        bool activeScrollViewFresh = false,
        bool applyActiveScrollViewDim = true)
    {
        SyncWindowElementFramesTo(window, new Rect(0, 0, Math.Max(1, size.Width), Math.Max(1, size.Height)));
        TerminalBuffer buffer = new(size.Width, size.Height);
        FillRect(buffer, new Rect(0, 0, size.Width, size.Height), window.GeneratedWindowStyle);
        List<Element> focusable = FocusableElements(window, activeScrollView);
        Element? focused = focusedElement is not null && focusable.Contains(focusedElement)
            ? focusedElement
            : focusedIndex >= 0 && focusedIndex < focusable.Count
            ? focusable[focusedIndex]
            : null;
        ScrollView? scopedActiveScrollView = activeScrollView is not null && WindowContainsElement(window, activeScrollView)
            ? activeScrollView
            : null;
        ReusableElement? scopedActiveScrollViewProxy =
            scopedActiveScrollView is not null &&
            activeScrollViewProxy is not null &&
            WindowContainsElement(window, activeScrollViewProxy)
                ? activeScrollViewProxy
                : null;
        if (scopedActiveScrollView is not null && scopedActiveScrollViewProxy is null)
        {
            scopedActiveScrollViewProxy = ReusableContainingElement(window, scopedActiveScrollView);
        }
        List<ResolvedRuntimeCell> resolvedCells = ResolveRuntimeCells(window, size);

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

        if (editMode && scopedActiveScrollView is not null)
        {
            Color? focusBackground = ActiveScrollViewScopeFocusBackground(
                scopedActiveScrollView,
                activeScrollViewProxy ?? scopedActiveScrollViewProxy);
            if (focusBackground is not null)
            {
                Rect active = ActiveScrollViewScopeRect(scopedActiveScrollView, scopedActiveScrollViewProxy, activeScrollViewFresh);
                ApplyActiveScrollViewFocusBackground(buffer, active, focusBackground);
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
                activeScrollView,
                activeScrollViewProxy,
                activeScrollViewFresh);
        }
        if (applyActiveScrollViewDim)
        {
            DimOutsideActiveScrollViewScope(
                buffer,
                window,
                focused,
                editMode,
                scopedActiveScrollView,
                scopedActiveScrollViewProxy,
                activeScrollViewProxy ?? scopedActiveScrollViewProxy,
                activeScrollViewFresh);
        }
        return ContentFromBuffer(buffer);
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
        return result
            .OrderBy(element => element.Frame.Row)
            .ThenBy(element => element.Frame.Col)
            .ToList();
    }

    private static void AddFocusableElements(Element element, List<Element> result, ScrollView? activeScrollView)
    {
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
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
                foreach (Element child in scrollView.Children)
                {
                    AddFocusableElements(child, result, activeScrollView);
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
        bool activeScrollViewFresh = false)
    {
        viewportSize = new Size(
            Math.Max(MinimumRenderableSize, viewportSize.Width),
            Math.Max(MinimumRenderableSize, viewportSize.Height));
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
                activeScrollViewFresh),
            row,
            col);
        OffsetWindowElementFrames(
            window,
            row + BorderWidthHorizontal(window.GeneratedWindowStyle),
            col + BorderWidthVertical(window.GeneratedWindowStyle));
        OverlayFocusedComboBox(buffer, window, focusedIndex, editMode, activeScrollView);
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

    private static void RenderToConsole(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options, Size size)
    {
        List<List<TerminalCell>> content = RenderWindowFrameContent(window, size);
        foreach (string line in RenderHelpers.RenderedText(content))
        {
            Console.WriteLine(line);
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

    private static List<List<TerminalCell>> RenderWindowFrameContent(
        GeneratedWindowBase window,
        Size frameSize,
        int focusedIndex = -1,
        Element? focusedElement = null,
        bool editMode = false,
        ScrollView? activeScrollView = null,
        ReusableElement? activeScrollViewProxy = null,
        bool activeScrollViewFresh = false)
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
                activeScrollViewFresh),
            content.Row,
            content.Col);
        return ContentFromBuffer(frameBuffer);
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
        bool activeScrollViewFresh = false)
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
                List<Element> childFocusable = FocusableElements(
                    reusable.Child,
                    childOwnsActiveScrollView ? activeScrollView : null);
                int childFocusedIndex = focusedElement is not null
                    ? childFocusable.IndexOf(focusedElement)
                    : -1;
                if (childFocusedIndex < 0 &&
                    ReferenceEquals(element, focusedElement) &&
                    reusable.Child is GeneratedScrollViewBase generatedScrollView)
                {
                    childFocusedIndex = childFocusable.IndexOf(generatedScrollView.ScrollView());
                }
                if (childFocusedIndex < 0 && childOwnsActiveScrollViewProxy)
                {
                    childFocusedIndex = childFocusable.IndexOf(activeScrollViewProxy!);
                }
                if (childFocusedIndex < 0 && childOwnsActiveScrollView)
                {
                    childFocusedIndex = childFocusable.IndexOf(activeScrollView!);
                }

                bool childEditMode =
                    editMode &&
                    (childFocusedIndex >= 0 || childOwnsActiveScrollView) &&
                    (activeScrollView is null || childOwnsActiveScrollView || childOwnsActiveScrollViewProxy);
                rendered = RenderGeneratedWindowContent(
                    reusable.Child,
                    size,
                    childFocusedIndex,
                    focusedElement,
                    childEditMode,
                    childOwnsActiveScrollView ? activeScrollView : null,
                    childOwnsActiveScrollView || childOwnsActiveScrollViewProxy ? activeScrollViewProxy : null,
                    activeScrollViewFresh,
                    false);
                if (childOwnsActiveScrollView &&
                    activeScrollView is not null &&
                    activeScrollViewProxy is not null &&
                    !childOwnsActiveScrollViewProxy)
                {
                    RestoreExternalProxyActiveScrollViewContentBackground(rendered, activeScrollView);
                }
                if (ReferenceEquals(element, focusedElement) && reusable.Child is GeneratedScrollViewBase focusedGeneratedScrollView)
                {
                    ApplyGeneratedScrollViewProxyFocusBackground(rendered, focusedGeneratedScrollView);
                }
                OffsetWindowElementFrames(reusable.Child, row, col);
            }
            else
            {
                bool activeScrollViewHasExternalProxy =
                    element is ScrollView candidateScrollView &&
                    ReferenceEquals(candidateScrollView, activeScrollView) &&
                    activeScrollViewProxy is not null &&
                    !WindowContainsElement(window, activeScrollViewProxy);
                bool elementFocused =
                    ReferenceEquals(element, focusedElement) ||
                    (element is ScrollView scrollView &&
                        ReferenceEquals(scrollView, activeScrollView) &&
                        !activeScrollViewHasExternalProxy);
                rendered = element.Render(size, new ElementRenderState
            {
                Focused = elementFocused,
                EditMode = ReferenceEquals(element, focusedElement) && editMode,
                FocusedElement = focusedElement,
            });
            }
        }
        Blit(buffer, rendered, row, col, EntryContentClip(cellRect, entry.CellStyle));
        if (element is ReusableElement focusedReusable &&
            ReferenceEquals(element, focusedElement) &&
            focusedReusable.Child is GeneratedScrollViewBase)
        {
            ApplyFocusedReusableProxyBackground(
                buffer,
                new Rect(row, col, size.Width, size.Height),
                focusedReusable,
                editMode &&
                    activeScrollView is not null &&
                    ReferenceEquals(activeScrollViewProxy, focusedReusable));
        }
    }

    private static void ApplyGeneratedScrollViewProxyFocusBackground(
        List<List<TerminalCell>> rendered,
        GeneratedScrollViewBase generatedScrollView)
    {
        Color? focusBackground = generatedScrollView.ScrollView().FocusStyle?.Background;
        if (focusBackground is null)
        {
            return;
        }
        Color? baseBackground = generatedScrollView.ScrollView().Style.Background ?? generatedScrollView.GeneratedWindowStyle.Background;
        foreach (List<TerminalCell> row in rendered)
        {
            foreach (TerminalCell cell in row)
            {
                if (baseBackground is null || ColorEquals(cell.Background, baseBackground))
                {
                    cell.Background = BlendBackgroundOverExisting(focusBackground, cell.Background);
                }
            }
        }
    }

    private static void ApplyFocusedReusableProxyBackground(
        TerminalBuffer buffer,
        Rect rect,
        ReusableElement reusable,
        bool skipContentCells)
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
        int lastCol = Math.Min(buffer.Width, rect.Col + rect.Width + (skipContentCells ? 1 : 0));
        for (int row = firstRow; row < lastRow; ++row)
        {
            (int First, int Last)? contentBounds = skipContentCells
                ? RowRenderedVisualContentBounds(buffer, row, firstCol, lastCol)
                : null;
            (int First, int Last)? renderedContentBounds = skipContentCells
                ? RowRenderedContentBounds(buffer, row, firstCol, lastCol)
                : null;
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
        bool editMode,
        ScrollView? activeScrollView)
    {
        if (!editMode || activeScrollView is not null)
        {
            return;
        }
        List<Element> focusable = FocusableElements(window, activeScrollView);
        if (focusedIndex < 0 || focusedIndex >= focusable.Count || focusable[focusedIndex] is not ComboBox comboBox)
        {
            return;
        }
        Size size = new(Math.Max(MinimumRenderableSize, comboBox.Frame.Width), Math.Max(MinimumRenderableSize, comboBox.Options.Count + 1));
        List<List<TerminalCell>> rendered = comboBox.Render(size, new ElementRenderState
        {
            Focused = true,
            EditMode = true,
        });
        Blit(buffer, rendered, comboBox.Frame.Row, comboBox.Frame.Col);
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
                Color? dimBaseBackground = cell.BackgroundFromImageSample
                    ? new Color("#000000")
                    : cell.Background;
                cell.Background = dim.Rgba.HasValue && dimBaseBackground is not null
                    ? dim.BlendOver(dimBaseBackground)
                    : dim;
                buffer.SetCell(row, col, cell);
            }
        }
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

    private static void RestoreExternalProxyActiveScrollViewContentBackground(
        List<List<TerminalCell>> rendered,
        ScrollView activeScrollView)
    {
        Color? focusBackground = activeScrollView.FocusStyle?.Background;
        if (focusBackground is null)
        {
            return;
        }
        Color focusedTerminalBackground = focusBackground.Rgba is Rgba rgba && rgba.Alpha < byte.MaxValue
            ? focusBackground.BlendOver(DefaultTerminalBackground)
            : focusBackground;
        for (int row = 0; row < rendered.Count; ++row)
        {
            List<TerminalCell> cells = rendered[row];
            (int First, int Last)? bounds = RowRenderedContentBounds(cells);
            if (bounds is null)
            {
                continue;
            }
            int contentStart = Math.Max(0, bounds.Value.First - 1);
            int contentEnd = cells.Count - 1;
            for (int col = contentStart; col <= contentEnd && col < cells.Count; ++col)
            {
                TerminalCell cell = cells[col];
                if (ColorEquals(cell.Background, focusedTerminalBackground))
                {
                    cell.Background = DefaultTerminalBackground;
                }
            }
        }
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

    private static Color? VisibleBackground(Color? background)
    {
        return background is null || background.IsTransparent ? null : background;
    }

    private static bool ColorEquals(Color? left, Color? right)
    {
        if (left is null || right is null)
        {
            return left is null && right is null;
        }
        return left.Equals(right);
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

    private static ReusableElement? ReusableContainingElement(GeneratedWindowBase window, Element target)
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

    private static bool WindowContainsElement(GeneratedWindowBase window, Element target)
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

    private static List<ResolvedRuntimeCell> ResolveRuntimeCells(GeneratedWindowBase window, Size size)
    {
        List<RuntimeCell> cells = CollectRuntimeCells(window);
        List<ResolvedRuntimeCell> resolved = new();
        ResolveRuntimeCellsWithFitPass(
            window,
            cells,
            new Rect(0, 0, Math.Max(MinimumRenderableSize, size.Width), Math.Max(MinimumRenderableSize, size.Height)),
            BorderWidthHorizontal(window.GeneratedWindowStyle),
            BorderWidthVertical(window.GeneratedWindowStyle),
            WindowMode(window),
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

    private static void SyncWindowElementFramesTo(GeneratedWindowBase window, Rect frame)
    {
        Size size = new(Math.Max(1, frame.Width), Math.Max(1, frame.Height));
        List<ResolvedRuntimeCell> resolvedCells = ResolveRuntimeCells(window, size);
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
                SyncWindowElementFramesTo(reusable.Child, element.Frame);
            }
            SyncElementChildFrames(element);
        }
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
        if (scrollView.Frame.Width <= 0 || scrollView.Frame.Height <= 0)
        {
            return false;
        }
        int viewportWidth = Math.Max(
            MinimumRenderableSize,
            scrollView.Frame.Width - PaddingLeft(scrollView.Style) - PaddingRight(scrollView.Style));
        int viewportHeight = Math.Max(
            MinimumRenderableSize,
            scrollView.Frame.Height - PaddingTop(scrollView.Style) - PaddingBottom(scrollView.Style));
        int sequenceRow = 0;
        for (int index = 0; index < scrollView.Children.Count; ++index)
        {
            Element child = scrollView.Children[index];
            int childHeight = Math.Max(MinimumRenderableSize, child.Frame.Height);
            Rect? targetRect = FocusRectWithinScrollChild(child, target, viewportWidth, childHeight);
            if (targetRect is not null)
            {
                int targetTop = sequenceRow + targetRect.Value.Row;
                int targetBottom = sequenceRow + targetRect.Value.Row + Math.Max(MinimumRenderableSize, targetRect.Value.Height);
                int nextOffset = scrollView.ScrollOffset;
                if (targetTop < scrollView.ScrollOffset)
                {
                    nextOffset = targetTop;
                }
                else if (targetBottom > scrollView.ScrollOffset + viewportHeight)
                {
                    nextOffset = targetBottom - viewportHeight;
                }
                int naturalSkip = Math.Max(
                    0,
                    scrollView.ContentHeight(viewportWidth) - viewportHeight);
                nextOffset = Math.Clamp(nextOffset, 0, naturalSkip);
                return scrollView.ScrollBy(
                    nextOffset - scrollView.ScrollOffset,
                    new Size(scrollView.Frame.Width, scrollView.Frame.Height));
            }
            sequenceRow += childHeight;
            if (index + 1 < scrollView.Children.Count)
            {
                sequenceRow += Math.Max(0, scrollView.Gap);
            }
        }
        return false;
    }

    private static Rect? FocusRectWithinScrollChild(Element child, Element target, int width, int height)
    {
        if (ReferenceEquals(child, target))
        {
            return new Rect(0, 0, Math.Max(MinimumRenderableSize, child.Frame.Width), Math.Max(MinimumRenderableSize, height));
        }
        if (child is ReusableElement reusable && reusable.Child is not null &&
            WindowContainsElement(reusable.Child, target))
        {
            SyncWindowElementFramesTo(
                reusable.Child,
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

    private static List<List<TerminalCell>> ContentFromBuffer(TerminalBuffer buffer)
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
        public bool ActiveScrollViewFresh { get; set; }
        public ScrollView? PendingModalScrollView { get; set; }
        public ReusableElement? PendingModalScrollViewProxy { get; set; }

        public RuntimeFrame(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options)
        {
            Window = window;
            Options = options;
        }
    }

    private readonly List<RuntimeFrame> frames = new();
    private readonly McpRuntimeConfig config;

    public McpController(GeneratedWindowBase window, GeneratedWindowRuntimeOptions options, McpRuntimeConfig config)
    {
        this.config = config;
        PushFrame(window, options);
        ApplyInitialFocus(Current);
    }

    private RuntimeFrame Current => frames[^1];

    private void PushFrame(GeneratedWindowBase nextWindow, GeneratedWindowRuntimeOptions nextOptions)
    {
        nextWindow.AttachRuntimeWindowStack(OpenWindow, CloseWindow, SetRuntimeWindowFocus);
        frames.Add(new RuntimeFrame(nextWindow, nextOptions));
    }

    private void OpenWindow(GeneratedWindowBase nextWindow, GeneratedWindowRuntimeOptions? nextOptions)
    {
        ClearActiveScrollViewScope(Current);
        PushFrame(nextWindow, nextOptions ?? nextWindow.RuntimeOptions());
        ApplyInitialFocus(Current);
    }

    private static void ClearActiveScrollViewScope(RuntimeFrame frame)
    {
        frame.EditMode = false;
        frame.EditScopeOwner = null;
        frame.ActiveScrollView = null;
        frame.ActiveScrollViewProxy = null;
        frame.ActiveScrollViewFresh = false;
    }

    private static bool RestorePendingModalScrollViewScope(RuntimeFrame frame)
    {
        ScrollView? scrollView = frame.PendingModalScrollView;
        ReusableElement? proxy = frame.PendingModalScrollViewProxy;
        frame.PendingModalScrollView = null;
        frame.PendingModalScrollViewProxy = null;
        if (scrollView is null || proxy is null)
        {
            return false;
        }
        Element? focused = frame.FocusedElementRef;
        if (focused is null)
        {
            return false;
        }
        (ReusableElement Proxy, ScrollView ScrollView)? context =
            ScrollViewFocusContextContainingElement(frame.Window, focused);
        if (context.HasValue &&
            ReferenceEquals(context.Value.Proxy, proxy) &&
            ReferenceEquals(context.Value.ScrollView, scrollView))
        {
            List<Element> focusable = GeneratedWindowRuntime.FocusableElements(frame.Window);
            int proxyIndex = focusable.IndexOf(proxy);
            if (proxyIndex >= 0)
            {
                frame.FocusedIndex = proxyIndex;
                frame.FocusedElementRef = proxy;
                frame.EditMode = false;
                frame.EditScopeOwner = null;
                frame.ActiveScrollView = null;
                frame.ActiveScrollViewProxy = null;
                frame.ActiveScrollViewFresh = false;
                return true;
            }
        }
        return false;
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
            : ScrollViewFocusContextContainingElement(frame.Window, element);
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
        frame.ActiveScrollViewFresh = false;
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

    private void CloseWindow()
    {
        if (frames.Count > 1)
        {
            frames.RemoveAt(frames.Count - 1);
        }
        if (Current.ActiveScrollViewProxy is not null)
        {
            List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, Current.ActiveScrollView);
            int proxyIndex = focusable.IndexOf(Current.ActiveScrollViewProxy);
            if (proxyIndex >= 0)
            {
                Current.FocusedIndex = proxyIndex;
                Current.FocusedElementRef = focusable[proxyIndex];
            }
        }
        Current.EditMode = false;
        Current.EditScopeOwner = null;
        Current.ActiveScrollView = null;
        Current.ActiveScrollViewProxy = null;
        Current.ActiveScrollViewFresh = false;
    }

    private void CloseCurrentWindowIfRequested()
    {
        if (frames.Count > 1 && Current.Options.ShouldClose?.Invoke() == true)
        {
            CloseWindow();
        }
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
            "get_text_snapshot" => ToolGetTextSnapshot(),
            "get_accessibility_snapshot" or "get_state" => ToolGetAccessibilitySnapshot(),
            "get_schema" => ToolGetAccessibilitySnapshot(),
            "get_source" => new JsonObject { ["class"] = Current.Window.GetType().Name, ["mcp_enabled"] = Current.Window.McpEnabled, ["source_markdown"] = Current.Window.McpSourceMarkdown },
            "get_viewport" => ToolGetViewport(),
            "set_viewport" => ToolSetViewport(arguments),
            "repaint" => new JsonObject(),
            "focus_element" => ToolFocusElement(arguments["element_id"]?.GetValue<string>() ?? ""),
            "enter_edit_mode" => ToolEnterEditMode(arguments["element_id"]?.GetValue<string>() ?? ""),
            "exit_edit_mode" => ToolExitEditMode(),
            "activate_element" => ToolActivateElement(arguments["element_id"]?.GetValue<string>() ?? ""),
            "click_element" => ToolClickElement(arguments["element_id"]?.GetValue<string>() ?? ""),
            "mouse_click" => ToolMouseClick(arguments),
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
            "get_render_rect", "get_text_snapshot", "get_state", "get_schema", "get_source", "get_viewport",
            "set_viewport", "repaint", "focus_element", "enter_edit_mode", "exit_edit_mode",
            "activate_element", "click_element", "mouse_click", "scroll",
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
        List<List<TerminalCell>> content = RenderContent();
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
            content = RenderContent();
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
            content = RenderContent();
        }
        return new JsonObject
        {
            ["format"] = "render-cells-v1",
            ["cells"] = RenderHelpers.CompactCells(content),
        };
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
            ScrollViewFocusContextContainingElement(Current.Window, element);
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

    private JsonNode ToolEnterEditMode(string elementId)
    {
        JsonNode result = ToolFocusElement(elementId);
        Element? entered = FocusedElement();
        if (entered is not null && EnterScrollViewScope(entered))
        {
            Current.Options.OnEditStarted?.Invoke(entered.Name);
            return result;
        }
        Current.EditMode = true;
        if (entered is not null)
        {
            if (entered is ComboBox comboBox && !comboBox.MenuOpen)
            {
                comboBox.HandleKey("Enter");
            }
            Current.Options.OnEditStarted?.Invoke(entered.Name);
        }
        return result;
    }

    private bool EnterScrollViewScope(Element focused)
    {
        if (focused is ReusableElement reusable && reusable.Child is GeneratedScrollViewBase generatedScrollView)
        {
            Current.EditMode = true;
            Current.EditScopeOwner = reusable;
            Current.ActiveScrollView = generatedScrollView.ScrollView();
            Current.ActiveScrollViewProxy = reusable;
            Current.ActiveScrollViewFresh = true;
            FocusFirstScrollViewScopeElement(Current, reusable, generatedScrollView.ScrollView());
            return true;
        }
        if (focused is ScrollView scrollView)
        {
            Current.EditMode = true;
            Current.EditScopeOwner = null;
            Current.ActiveScrollView = scrollView;
            Current.ActiveScrollViewProxy = null;
            Current.ActiveScrollViewFresh = true;
            return true;
        }
        Current.ActiveScrollView = null;
        Current.ActiveScrollViewProxy = null;
        Current.ActiveScrollViewFresh = false;
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
        Current.ActiveScrollViewFresh = false;
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
        return ActivateResolvedElement(element, element is Button or Image);
    }

    private JsonNode ActivateResolvedElement(Element element, bool activateScrollScopeAfterActivation = true)
    {
        GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, element);
        RuntimeFrame activationFrame = Current;
        int frameCountBeforeActivation = frames.Count;
        (ReusableElement Proxy, ScrollView ScrollView)? activatedScrollContext =
            ScrollViewFocusContextContainingElement(Current.Window, element);
        FocusElement(element);
        if (element is Button or Image or ReusableElement)
        {
            OptionsFor(element).OnButton?.Invoke(element.Name);
        }
        else if (element is CheckBox)
        {
            element.HandleKey("Enter");
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
        RestorePendingModalScrollViewScope(Current);
        return Snapshot(element);
    }

    private JsonNode ToolClickElement(string elementId)
    {
        RenderContent();
        Element element = RequireElement(elementId);
        if (!element.Enabled)
        {
            return Snapshot(element);
        }
        FocusElement(element);
        if (element is Button or CheckBox)
        {
            return ActivateResolvedElement(element);
        }
        if (element is TextInput textInput)
        {
            Current.EditMode = true;
            textInput.SetCursor(textInput.Value.Length);
            Current.Options.OnEditStarted?.Invoke(textInput.Name);
        }
        else if (element is NumberInput numberInput)
        {
            Current.EditMode = true;
            Current.Options.OnEditStarted?.Invoke(numberInput.Name);
        }
        else if (element is ComboBox comboBox)
        {
            Current.EditMode = true;
            if (!comboBox.MenuOpen)
            {
                comboBox.HandleKey("Enter");
            }
            Current.Options.OnEditStarted?.Invoke(comboBox.Name);
        }
        else
        {
            Current.EditMode = false;
        }
        return Snapshot(element);
    }

    private static void ActivateScrollViewScopeInFrame(RuntimeFrame frame, ReusableElement proxy, ScrollView scrollView)
    {
        frame.EditMode = true;
        frame.EditScopeOwner = proxy;
        frame.ActiveScrollView = scrollView;
        frame.ActiveScrollViewProxy = proxy;
        frame.ActiveScrollViewFresh = true;
    }

    private static void FocusScrollViewScopeInFrame(RuntimeFrame frame, ReusableElement proxy, ScrollView scrollView)
    {
        frame.EditMode = true;
        frame.EditScopeOwner = proxy;
        frame.ActiveScrollView = scrollView;
        frame.ActiveScrollViewProxy = proxy;
        frame.ActiveScrollViewFresh = true;
        FocusFirstScrollViewScopeElement(frame, proxy, scrollView);
    }

    private static void FocusFirstScrollViewScopeElement(RuntimeFrame frame, ReusableElement proxy, ScrollView scrollView)
    {
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(frame.Window, scrollView);
        Element? target = focusable.FirstOrDefault(element =>
        {
            if (ReferenceEquals(element, proxy))
            {
                return false;
            }
            (ReusableElement Proxy, ScrollView ScrollView)? context =
                ScrollViewFocusContextContainingElement(frame.Window, element);
            return context.HasValue &&
                ReferenceEquals(context.Value.Proxy, proxy) &&
                ReferenceEquals(context.Value.ScrollView, scrollView);
        });
        target ??= proxy;
        int index = focusable.IndexOf(target);
        if (index < 0)
        {
            return;
        }
        frame.FocusedIndex = index;
        frame.FocusedElementRef = focusable[index];
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
                WindowContainsElementForActivation(reusable.Child, target))
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

    private static bool WindowContainsElementForActivation(GeneratedWindowBase window, Element target)
    {
        foreach (Element element in window.Elements)
        {
            if (ReferenceEquals(element, target) || ElementTreeContainsForActivation(element, target))
            {
                return true;
            }
            if (element is ReusableElement reusable && reusable.Child is not null &&
                WindowContainsElementForActivation(reusable.Child, target))
            {
                return true;
            }
        }
        return false;
    }

    private static bool ElementTreeContainsForActivation(Element element, Element target)
    {
        foreach (Element child in element.Children)
        {
            if (ReferenceEquals(child, target) || ElementTreeContainsForActivation(child, target))
            {
                return true;
            }
            if (child is ReusableElement reusable && reusable.Child is not null &&
                WindowContainsElementForActivation(reusable.Child, target))
            {
                return true;
            }
        }
        return false;
    }

    private JsonNode ToolPressKey(string key)
    {
        RenderContent();
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, Current.ActiveScrollView);
        Element? focused = FocusedElement();
        int focusedListIndex = focused is null ? Current.FocusedIndex : focusable.IndexOf(focused);
        if (focusedListIndex >= 0)
        {
            Current.FocusedIndex = focusedListIndex;
        }
        if (Current.Options.OnKey?.Invoke(key) == true)
        {
            CloseCurrentWindowIfRequested();
            return focused is null ? new JsonObject { ["ok"] = true } : Snapshot(focused);
        }
        if (focused is not null && Current.Options.OnKeyBeforeFocusedElement?.Invoke(key, focused.Name, Current.EditMode) == true)
        {
            DispatchChanged(focused);
            CloseCurrentWindowIfRequested();
            return Snapshot(focused);
        }
        if (!Current.EditMode && IsDirectionalKey(key))
        {
            MoveFocusDirection(key);
            focused = FocusedElement();
            return focused is null ? new JsonObject { ["ok"] = true } : Snapshot(focused);
        }
        if (key == "Escape")
        {
            if (Current.EditScopeOwner is not null)
            {
                List<Element> scopedFocusable = GeneratedWindowRuntime.FocusableElements(Current.Window, Current.ActiveScrollView);
                int ownerIndex = scopedFocusable.IndexOf(Current.EditScopeOwner);
                if (ownerIndex >= 0)
                {
                    Current.FocusedIndex = ownerIndex;
                    Current.FocusedElementRef = scopedFocusable[ownerIndex];
                }
                focused = Current.EditScopeOwner;
                Current.EditScopeOwner = null;
            }
            Current.EditMode = false;
            Current.ActiveScrollView = null;
            Current.ActiveScrollViewProxy = null;
            Current.ActiveScrollViewFresh = false;
            return focused is null ? new JsonObject { ["ok"] = true } : Snapshot(focused);
        }
        if (key == "Tab" && focusable.Count > 0)
        {
            if (focused is not null)
            {
                Current.Options.OnFocusChanged?.Invoke(focused.Name, false);
            }
            int currentIndex = focused is null ? Current.FocusedIndex : focusable.IndexOf(focused);
            Current.FocusedIndex = (currentIndex + 1 + focusable.Count) % focusable.Count;
            Current.FocusedElementRef = focusable[Current.FocusedIndex];
            Current.EditMode = false;
            Current.EditScopeOwner = null;
            Current.ActiveScrollView = null;
            Current.ActiveScrollViewProxy = null;
            Current.ActiveScrollViewFresh = false;
            Current.Options.OnFocusChanged?.Invoke(focusable[Current.FocusedIndex].Name, true);
            return Snapshot(focusable[Current.FocusedIndex]);
        }
        if (key == "Shift+Tab" && focusable.Count > 0)
        {
            if (focused is not null)
            {
                Current.Options.OnFocusChanged?.Invoke(focused.Name, false);
            }
            int currentIndex = focused is null ? Current.FocusedIndex : focusable.IndexOf(focused);
            Current.FocusedIndex = (currentIndex - 1 + focusable.Count) % focusable.Count;
            Current.FocusedElementRef = focusable[Current.FocusedIndex];
            Current.EditMode = false;
            Current.EditScopeOwner = null;
            Current.ActiveScrollView = null;
            Current.ActiveScrollViewProxy = null;
            Current.ActiveScrollViewFresh = false;
            Current.Options.OnFocusChanged?.Invoke(focusable[Current.FocusedIndex].Name, true);
            return Snapshot(focusable[Current.FocusedIndex]);
        }
        if (focused is not null)
        {
            bool dispatchChanged = false;
            if (key == "Enter" && focused is Button or Image)
            {
                OptionsFor(focused).OnButton?.Invoke(focused.Name);
            }
            else if (key == "Enter" && focused is ReusableElement reusable && reusable.Child is not null)
            {
                if (EnterScrollViewScope(reusable))
                {
                    DispatchChanged(focused);
                    CloseCurrentWindowIfRequested();
                    return Snapshot(focused);
                }
                List<Element> descendants = DescendantFocusableElements(focused);
                if (descendants.Count > 0)
                {
                    Current.EditMode = true;
                    Current.EditScopeOwner = focused;
                    int descendantIndex = focusable.IndexOf(descendants[0]);
                    if (descendantIndex >= 0)
                    {
                        Current.FocusedIndex = descendantIndex;
                        focused = descendants[0];
                        Current.FocusedElementRef = focused;
                    }
                }
            }
            else if (Current.EditScopeOwner is not null && (key == "Down" || key == "Up"))
            {
                bool movedScopedFocus = false;
                List<Element> descendants = ScrollViewScopeFocusableElements();
                int scopedIndex = descendants.IndexOf(focused);
                if (scopedIndex >= 0 && descendants.Count > 0)
                {
                    Element? target = DirectionalScrollViewScopeTarget(focused, descendants, key);
                    if (target is not null)
                    {
                        int nextFocusIndex = focusable.IndexOf(target);
                        if (nextFocusIndex >= 0)
                        {
                            Current.FocusedIndex = nextFocusIndex;
                            focused = target;
                            Current.FocusedElementRef = focused;
                            GeneratedWindowRuntime.EnsureElementVisibleInContainingScrollView(Current.Window, target);
                            Current.ActiveScrollViewFresh = false;
                            movedScopedFocus = true;
                        }
                    }
                }
                if (!movedScopedFocus)
                {
                    if (Current.ActiveScrollView is not null)
                    {
                        Current.ActiveScrollView.HandleKey(key);
                    }
                    else
                    {
                        focused.HandleKey(key);
                    }
                    Current.ActiveScrollViewFresh = false;
                }
            }
            else if (key == "Enter")
            {
                if (!Current.EditMode)
                {
                    Current.EditMode = true;
                    Current.Options.OnEditStarted?.Invoke(focused.Name);
                }
                focused.HandleKey(key);
                dispatchChanged = true;
                if (focused is ComboBox comboBox && !comboBox.MenuOpen && Current.ActiveScrollView is null)
                {
                    Current.EditMode = false;
                }
            }
            else
            {
                focused.HandleKey(key);
                dispatchChanged = true;
            }
            if (dispatchChanged)
            {
                DispatchChanged(focused);
            }
            CloseCurrentWindowIfRequested();
            return Snapshot(focused);
        }
        return new JsonObject { ["ok"] = true };
    }

    private List<Element> ScrollViewScopeFocusableElements()
    {
        if (Current.EditScopeOwner is null || Current.ActiveScrollView is null)
        {
            return new List<Element>();
        }
        return GeneratedWindowRuntime.FocusableElements(Current.Window, Current.ActiveScrollView)
            .Where(element =>
            {
                if (ReferenceEquals(element, Current.EditScopeOwner))
                {
                    return false;
                }
                (ReusableElement Proxy, ScrollView ScrollView)? context =
                    ScrollViewFocusContextContainingElement(Current.Window, element);
                return context.HasValue &&
                    ReferenceEquals(context.Value.Proxy, Current.EditScopeOwner) &&
                    ReferenceEquals(context.Value.ScrollView, Current.ActiveScrollView);
            })
            .ToList();
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
            return;
        }
        Element? currentFocused = FocusedElement();
        int currentIndex = currentFocused is null ? Current.FocusedIndex : focusable.IndexOf(currentFocused);
        if (currentIndex < 0 || currentIndex >= focusable.Count)
        {
            Element? previous = FocusedElement();
            Current.FocusedIndex = 0;
            Current.FocusedElementRef = focusable[0];
            Current.EditMode = false;
            Current.EditScopeOwner = null;
            Current.ActiveScrollView = null;
            Current.ActiveScrollViewProxy = null;
            Current.ActiveScrollViewFresh = false;
            if (previous is not null)
            {
                Current.Options.OnFocusChanged?.Invoke(previous.Name, false);
            }
            Current.Options.OnFocusChanged?.Invoke(focusable[0].Name, true);
            return;
        }
        Element current = focusable[currentIndex];
        Rect currentRect = current.Frame;
        List<(int BandRank, int PrimaryGap, int PerpendicularGap, int EdgeDelta, int Order, Element Element)> candidates = new();
        for (int order = 0; order < focusable.Count; ++order)
        {
            Element candidate = focusable[order];
            if (ReferenceEquals(candidate, current))
            {
                continue;
            }
            (int BandRank, int PrimaryGap, int PerpendicularGap, int EdgeDelta)? score =
                DirectionalFocusScore(currentRect, candidate.Frame, direction);
            if (score.HasValue)
            {
                candidates.Add((
                    score.Value.BandRank,
                    score.Value.PrimaryGap,
                    score.Value.PerpendicularGap,
                    score.Value.EdgeDelta,
                    order,
                    candidate));
            }
        }
        if (candidates.Count == 0)
        {
            return;
        }
        candidates.Sort((left, right) =>
        {
            int compare = left.BandRank.CompareTo(right.BandRank);
            if (compare != 0) return compare;
            compare = left.PrimaryGap.CompareTo(right.PrimaryGap);
            if (compare != 0) return compare;
            compare = left.PerpendicularGap.CompareTo(right.PerpendicularGap);
            if (compare != 0) return compare;
            compare = left.EdgeDelta.CompareTo(right.EdgeDelta);
            if (compare != 0) return compare;
            return left.Order.CompareTo(right.Order);
        });
        FocusElement(candidates[0].Element);
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
        int currentCenterRow = current.Row * 2 + current.Height;
        int currentCenterCol = current.Col * 2 + current.Width;
        int candidateCenterRow = candidate.Row * 2 + candidate.Height;
        int candidateCenterCol = candidate.Col * 2 + candidate.Width;
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
            if (!AxisOverlaps(currentTop, currentBottom, candidateTop, candidateBottom))
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
            if (!AxisOverlaps(currentTop, currentBottom, candidateTop, candidateBottom))
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

    private JsonNode ToolMouseClickAt(Point position)
    {
        Element? focused = FocusedElement();
        if (focused is ComboBox comboBox && Current.EditMode && ComboBoxDropDownFrame(comboBox).Contains(position))
        {
            int localRow = position.Row - comboBox.Frame.Row;
            if (localRow > 0)
            {
                int index = Math.Clamp(localRow - 1, 0, Math.Max(0, comboBox.Options.Count - 1));
                comboBox.SetSelectedIndex(index);
                OptionsFor(comboBox).OnSelectionChanged?.Invoke(comboBox.Name, new List<string> { comboBox.SelectedText });
            }
            comboBox.CloseMenu();
            Current.EditMode = false;
            return Snapshot(comboBox);
        }
        if (focused is ListBox listBox && Current.EditMode && listBox.Frame.Contains(position))
        {
            int index = Math.Clamp(position.Row - listBox.Frame.Row, 0, Math.Max(0, listBox.Options.Count - 1));
            listBox.SetSelectedIndex(index);
            Current.EditMode = false;
            OptionsFor(listBox).OnSelectionChanged?.Invoke(listBox.Name, listBox.SelectedValues.ToList());
            return Snapshot(listBox);
        }
        Element? target = MouseTargetElement(position);
        if (target is null || !target.Enabled)
        {
            ClearFocusForEmptyMouseTarget();
            return ToolGetAccessibilitySnapshot();
        }
        if (target != focused)
        {
            FocusElement(target);
            focused = target;
        }
        if (target is Button or CheckBox)
        {
            return ActivateResolvedElement(target);
        }
        if (target is TextInput textInput)
        {
            Current.EditMode = true;
            textInput.SetCursor(Math.Max(0, Math.Min(textInput.Value.Length, position.Col - textInput.Frame.Col)));
            Current.Options.OnEditStarted?.Invoke(textInput.Name);
        }
        else if (target is NumberInput numberInput)
        {
            Current.EditMode = true;
            Current.Options.OnEditStarted?.Invoke(numberInput.Name);
        }
        else if (target is ComboBox targetComboBox)
        {
            Current.EditMode = true;
            if (!targetComboBox.MenuOpen)
            {
                targetComboBox.HandleKey("Enter");
            }
            Current.Options.OnEditStarted?.Invoke(targetComboBox.Name);
        }
        else
        {
            Current.EditMode = false;
        }
        return Snapshot(target);
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
        return new Point(point.Row - config.ViewportRow, point.Col - config.ViewportCol);
    }

    private Element? MouseTargetElement(Point position)
    {
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Current.Window, Current.ActiveScrollView);
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
        if (previous is ComboBox comboBox)
        {
            comboBox.CloseMenu();
        }
        Current.EditMode = false;
        Current.EditScopeOwner = null;
        Current.ActiveScrollView = null;
        Current.ActiveScrollViewProxy = null;
        Current.ActiveScrollViewFresh = false;
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
            scrollView.ScrollBy(-delta, new Size(scrollView.Frame.Width, scrollView.Frame.Height));
        }
        else if (target is ListBox listBox)
        {
            listBox.ScrollLines(delta);
        }
        return Snapshot(target);
    }

    private JsonNode ToolTypeText(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        string text = JsonText(arguments["text"] ?? arguments["value"]);
        Element target = string.IsNullOrEmpty(elementId) ? FocusedElement() ?? throw new InvalidOperationException("No focused element") : RequireElement(elementId);
        if (target is TextInput input)
        {
            input.InsertText(text);
            OptionsFor(input).OnTextChanged?.Invoke(input.Name, input.Value);
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
            OptionsFor(input).OnTextChanged?.Invoke(input.Name, input.Value);
        }
        else if (target is NumberInput number)
        {
            double parsed = double.TryParse(text, System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture, out double value)
                ? value
                : 0.0;
            number.SetValue(parsed);
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
                    listBox.SetSelectedIndex(index);
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
            input.SetSelection(start, end);
            return new JsonObject
            {
                ["start"] = input.SelectionStart,
                ["end"] = input.SelectionEnd,
                ["text"] = input.SelectionText(),
            };
        }
        return Snapshot(target);
    }

    private JsonNode ToolCopySelection(JsonObject arguments)
    {
        string elementId = arguments["element_id"]?.GetValue<string>() ?? "";
        Element target = RequireElement(elementId);
        return new JsonObject { ["text"] = target is TextInput input ? input.SelectionText() : "" };
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
                listBox.SetSelectedIndex(index);
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
        Element? focused = FocusedElement();
        return GeneratedWindowRuntime.RenderViewportContent(
            Current.Window,
            new Size(config.ViewportWidth, config.ViewportHeight),
            focused is null ? -1 : Current.FocusedIndex,
            focused,
            Current.EditMode,
            Current.ActiveScrollView,
            Current.ActiveScrollViewProxy,
            Current.ActiveScrollViewFresh);
    }

    private Element? FocusedElement()
    {
        return FocusedElement(Current);
    }

    private static Element? FocusedElement(RuntimeFrame frame)
    {
        List<Element> focusable = GeneratedWindowRuntime.FocusableElements(frame.Window, frame.ActiveScrollView);
        if (frame.FocusedElementRef is not null)
        {
            int refIndex = focusable.IndexOf(frame.FocusedElementRef);
            if (refIndex >= 0)
            {
                frame.FocusedIndex = refIndex;
                return frame.FocusedElementRef;
            }
            frame.FocusedElementRef = null;
            frame.FocusedIndex = -1;
            return null;
        }
        if (frame.FocusedIndex >= 0 && frame.FocusedIndex < focusable.Count)
        {
            frame.FocusedElementRef = focusable[frame.FocusedIndex];
            return frame.FocusedElementRef;
        }
        frame.FocusedElementRef = null;
        return null;
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
            string? nested = ElementPathInElement(element, target);
            if (nested is not null)
            {
                if (nested.Contains('[', StringComparison.Ordinal))
                {
                    return nested;
                }
                return element is ReusableElement
                    ? element.Name + "." + nested
                    : nested;
            }
        }
        return null;
    }

    private static string? ElementPathInElement(Element element, Element target)
    {
        if (element is ReusableElement reusable && reusable.Child is not null)
        {
            string? nested = ElementPath(reusable.Child, target);
            if (nested is not null)
            {
                return nested.Contains('[', StringComparison.Ordinal)
                    ? nested
                    : reusable.Name + "." + nested;
            }
        }
        if (element is ScrollView scrollView)
        {
            foreach (Element child in scrollView.Children)
            {
                string? nested = ElementPathInElement(child, target);
                if (nested is not null)
                {
                    return nested;
                }
            }
        }
        foreach (Element child in element.Children)
        {
            if (ReferenceEquals(child, target))
            {
                return child.Name;
            }
            string? nested = ElementPathInElement(child, target);
            if (nested is not null)
            {
                return child.Name + "." + nested;
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
