using Uimd;

const int RegressionCount = 7;
int passed = 0;
passed += RunRegression("ScrollView scoped confirm keeps a fresh edit session", RunKeepEditModeRegression);
passed += RunRegression("ScrollView scoped confirm rebases focus after mutation", RunFocusRebaseRegression);
passed += RunRegression("TextInput alignment shares one render and mouse offset", RunTextInputAlignmentRegression);
passed += RunRegression("ComboBox mouse reaches every rendered option row", RunComboBoxMouseGeometryRegression);
passed += RunRegression("Reusable ScrollView focus underlays alpha descendants", RunAlphaScrollFocusRegression);
passed += RunRegression("Modal mouse clicks preserve local text cursor coordinates", RunModalMouseCursorRegression);
passed += RunRegression("Obsolete raw image rectangles are erased before repaint", RunRawImageTeardownRegression);
Console.WriteLine($"{passed}/{RegressionCount} checks passed");
if (passed != RegressionCount)
{
    Environment.ExitCode = 1;
}

static int RunRegression(string name, Action regression)
{
    try
    {
        regression();
        Console.WriteLine($"PASS {name}");
        return 1;
    }
    catch (Exception exception)
    {
        Console.WriteLine($"FAIL {name}: {exception.Message}");
        return 0;
    }
}

static void RunKeepEditModeRegression()
{
    ConfirmFixture fixture = new(true);
    fixture.Controller.PressKey("Enter");
    fixture.Controller.PressKey("a");
    fixture.Controller.PressKey("Enter");
    Require(!fixture.Child.Leading.Enabled, "confirm callback did not mutate the focusable set");
    Require(fixture.EditStartedCount == 2, "keep-after-confirm did not start a fresh edit session");
    fixture.Controller.PressKey("b");
    Require(fixture.Child.Filter.Value == "ab", "fresh scoped edit did not accept text");
    fixture.Controller.PressKey("Escape");
    Require(fixture.Child.Filter.Value == "a", "Escape did not restore the freshly committed value");
}

static void RunFocusRebaseRegression()
{
    ConfirmFixture fixture = new(false);
    fixture.Controller.PressKey("Enter");
    fixture.Controller.PressKey("a");
    fixture.Controller.PressKey("Enter");
    fixture.Controller.PressKey("Enter");
    fixture.Controller.PressKey("b");
    Require(fixture.Child.Filter.Value == "ab", "focus followed a stale index instead of reopening the submitted input");
    Require(fixture.Child.TrailingActivations == 0, "stale focus activated the trailing control");
}

static void RunTextInputAlignmentRegression()
{
    TextInput input = new("field", "abc", 10);
    input.SetStyle(new Style { TextAlign = "right" });
    input.SetCursorStyle(new Style { Background = new Color("#facc15") });

    List<List<TerminalCell>> rendered = input.Render(new Size(6, 1));
    Require(RenderHelpers.RenderedText(rendered)[0] == "   abc", "right-aligned text used the wrong offset");

    input.SetCursor(1);
    rendered = input.Render(new Size(6, 1), new ElementRenderState { EditMode = true });
    Require(rendered[0][4].Background?.ToString() == "#facc15", "cursor used the wrong alignment offset");

    input.SelectRange(0, 2);
    rendered = input.Render(new Size(6, 1), new ElementRenderState { EditMode = true });
    Require(rendered[0][3].Background?.ToString() == "#facc15", "selection start used the wrong offset");
    Require(rendered[0][4].Background?.ToString() == "#facc15", "selection end used the wrong offset");
    Require(rendered[0][5].Background?.ToString() != "#facc15", "selection extended past its source range");
    Require(input.CursorForPoint(0, 0, new Size(6, 1)) == 0, "leading padding did not map to cursor zero");
    Require(input.CursorForPoint(0, 4, new Size(6, 1)) == 1, "text click ignored the alignment offset");

    input.SetValue("abcdefgh");
    input.SetCursor(8);
    rendered = input.Render(new Size(6, 1), new ElementRenderState { EditMode = true });
    Require(RenderHelpers.RenderedText(rendered)[0] == "defgh ",
        "overflowing text unexpectedly retained its alignment offset");
}

static void RunComboBoxMouseGeometryRegression()
{
    List<string> options = Enumerable.Range(0, 8).Select(index => $"Option {index}").ToList();
    ComboMouseWindow window = new(options);
    ComboBox comboBox = window.Choice;
    int selectionChanges = 0;
    McpController controller = new(
        window,
        new GeneratedWindowRuntimeOptions
        {
            InitialFocusName = "choice",
            OnSelectionChanged = (_, _) => selectionChanges++,
        },
        new McpRuntimeConfig
        {
            ViewportWidth = TestLayout.ViewportWidth,
            ViewportHeight = 12,
        });

    controller.PressKey("Enter");
    controller.MousePressAt(new Point(comboBox.Frame.Row + 7, comboBox.Frame.Col + 1));

    Require(
        comboBox.SelectedIndex == 6,
        $"visible option below row six selected index {comboBox.SelectedIndex}, expected 6; frame {comboBox.Frame}");
    Require(selectionChanges == 1, "selection change was not dispatched exactly once");
    controller.PressKey("Down");
    Require(comboBox.SelectedIndex == 6, "ComboBox remained in edit mode after the click");
}

static void RunAlphaScrollFocusRegression()
{
    AlphaFocusScrollHostWindow window = new();
    Color parentBackground = new("#303545");
    Color focusBackground = new("#ffffff14");
    Color descendantBackground = new("#252a36cc");
    Color expectedUnfocused = descendantBackground.BlendOver(parentBackground);
    Color expectedFocused = descendantBackground.BlendOver(focusBackground.BlendOver(parentBackground));
    Size renderSize = new(TestLayout.ControlHeight, 2);

    List<List<TerminalCell>> unfocused = GeneratedWindowRuntime.RenderGeneratedWindowContent(
        window,
        renderSize);
    List<List<TerminalCell>> focused = GeneratedWindowRuntime.RenderGeneratedWindowContent(
        window,
        renderSize,
        0);

    Require(unfocused[0][0].Background?.ToString() == expectedUnfocused.ToString(),
        $"unfocused alpha descendant was {unfocused[0][0].Background}, expected {expectedUnfocused}");
    Require(focused[0][0].Background?.ToString() == expectedFocused.ToString(),
        $"focused alpha descendant was {focused[0][0].Background}, expected {expectedFocused}");
    Require(focused[1][0].Background?.ToString() == focusBackground.BlendOver(parentBackground).ToString(),
        $"identical proxy/inner focus was applied more than once: {focused[1][0].Background}");
}

static void RunModalMouseCursorRegression()
{
    AssertModalMouseCursor(multiline: false, clickCol: 3);
    AssertModalMouseCursor(multiline: true, clickCol: 6);
}

static void RunRawImageTeardownRegression()
{
    TerminalBuffer buffer = new(3, 2);
    TerminalCell raw = new()
    {
        Raw = "RAW-ONE",
        RawWidth = 2,
        RawHeight = 2,
    };
    buffer.SetCell(0, 0, raw);
    buffer.SetCell(0, 1, new TerminalCell { RawSkip = true });
    buffer.SetCell(1, 0, new TerminalCell { RawSkip = true });
    buffer.SetCell(1, 1, new TerminalCell { RawSkip = true });
    Require(buffer.RenderDiff(4, 7).Contains("RAW-ONE"), "initial raw payload was not rendered");
    Require(buffer.RenderDiff(4, 7) == "", "unchanged raw payload produced terminal output");

    buffer.Clear(new TerminalCell { Text = "." });
    string removed = buffer.RenderDiff(4, 7);
    Require(removed.StartsWith("\x1b[?2026h"), "raw cleanup was not synchronized");
    Require(removed.Contains("\x1b[5;8;6;9$z"), "old raw rectangle was not erased");
    Require(!removed.Contains("RAW-ONE"), "removed raw payload was retransmitted");
    Require(removed.EndsWith("\x1b[?2026l"), "raw cleanup synchronization was not closed");

    raw.Raw = "RAW-TWO";
    buffer.SetCell(0, 0, raw);
    buffer.SetCell(0, 1, new TerminalCell { RawSkip = true });
    buffer.SetCell(1, 0, new TerminalCell { RawSkip = true });
    buffer.SetCell(1, 1, new TerminalCell { RawSkip = true });
    Require(buffer.RenderDiff(4, 7).Contains("RAW-TWO"), "replacement baseline was not rendered");
    raw.Raw = "RAW-THREE";
    buffer.SetCell(0, 0, raw);
    string replaced = buffer.RenderDiff(4, 7);
    Require(replaced.Contains("\x1b[5;8;6;9$z"), "replaced raw rectangle was not erased");
    Require(replaced.Contains("RAW-THREE"), "replacement raw payload was not rendered");

    TerminalBuffer selective = new(5, 1);
    selective.SetCell(0, 0, new TerminalCell
    {
        Raw = "RAW-REMOVED",
        RawWidth = 2,
        RawHeight = 1,
    });
    selective.SetCell(0, 1, new TerminalCell { RawSkip = true });
    selective.SetCell(0, 3, new TerminalCell
    {
        Raw = "RAW-UNCHANGED",
        RawWidth = 2,
        RawHeight = 1,
    });
    selective.SetCell(0, 4, new TerminalCell { RawSkip = true });
    _ = selective.RenderDiff();
    selective.SetCell(0, 0, new TerminalCell { Text = "." });
    selective.SetCell(0, 1, new TerminalCell { Text = "." });
    string selectiveFrame = selective.RenderDiff();
    Require(selectiveFrame.Contains("\x1b[1;1;1;2$z"), "selective raw rectangle was not erased");
    Require(!selectiveFrame.Contains("RAW-REMOVED"), "removed raw payload was retransmitted");
    Require(!selectiveFrame.Contains("RAW-UNCHANGED"), "unrelated raw payload was retransmitted");
}

static void AssertModalMouseCursor(bool multiline, int clickCol)
{
    GeneratedWindowBase root = new("Root");
    ModalMouseWindow modal = new(multiline);
    McpRuntimeConfig config = new()
    {
        ViewportWidth = 30,
        ViewportHeight = 12,
    };
    McpController controller = new(root, new GeneratedWindowRuntimeOptions(), config);
    controller.OpenModalWindow(modal);

    _ = controller.RenderTerminalFrame(new Size(config.ViewportWidth, config.ViewportHeight));
    Size modalSize = GeneratedWindowRuntime.GeneratedWindowContentSize(modal);
    int modalRow = (config.ViewportHeight - modalSize.Height) / 2;
    int modalCol = (config.ViewportWidth - modalSize.Width) / 2;
    Rect controlFrame = modal.Control.Frame;
    int expectedCursor = modal.Control.CursorForPoint(
        0,
        clickCol,
        new Size(controlFrame.Width, controlFrame.Height),
        new ElementRenderState { Focused = true, EditMode = true });
    controller.MousePressAt(new Point(
        modalRow + controlFrame.Row,
        modalCol + controlFrame.Col + clickCol));
    Require(modal.Control.Cursor == expectedCursor,
        $"modal {(multiline ? "TextArea" : "TextInput")} cursor was " +
        $"{modal.Control.Cursor}, expected {expectedCursor}; modal={modalSize}, " +
        $"offset=({modalRow},{modalCol}), frame={controlFrame}");
}

static void Require(bool condition, string message)
{
    if (!condition)
    {
        throw new InvalidOperationException(message);
    }
}

internal sealed class ComboMouseWindow : GeneratedWindowBase
{
    public ComboBox Choice { get; }

    public ComboMouseWindow(IReadOnlyList<string> options) : base("Combo mouse")
    {
        Choice = AddElement(new ComboBox("choice", options));
        SetGeneratedLayout(new[]
        {
            new GeneratedLayoutEntry
            {
                Name = "choice",
                Type = "combobox",
                Relative = new Rect(0, 0, TestLayout.ControlWidth, TestLayout.ControlHeight),
                SourceCell = new Rect(0, 0, TestLayout.ControlWidth, TestLayout.ControlHeight),
                Width = AxisDimension.Fixed(TestLayout.ControlWidth),
                Height = AxisDimension.Fixed(TestLayout.ControlHeight),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(TestLayout.ControlWidth, TestLayout.ControlHeight),
                CellCharsSize = new Size(TestLayout.ControlWidth, TestLayout.ControlHeight),
            },
        });
    }
}

internal sealed class AlphaFocusScrollWindow : GeneratedScrollViewBase
{
    public ScrollView Items { get; }

    public AlphaFocusScrollWindow() : base("Alpha focus scroll")
    {
        GeneratedWindowStyle.Background = new Color("#303545");
        GeneratedWindowStyle.BorderWidthHorizontal = 0;
        GeneratedWindowStyle.BorderWidthVertical = 0;
        SetGeneratedFocusable(true);

        Items = AddElement(new ScrollView("items"));
        Items.SetStyle(new Style { Background = new Color("#303545") });
        Items.SetFocusStyle(new Style { Background = new Color("#ffffff14") });
        SetGeneratedScrollView(Items);
        Label row = Items.AddChild(new Label("row", " "));
        row.SetStyle(new Style { Background = new Color("#252a36cc") });

        GeneratedLayoutEntry entry = TestLayout.FixedEntry("items", "scrollview");
        entry.Relative = new Rect(0, 0, TestLayout.ControlHeight, 2);
        entry.SourceCell = new Rect(0, 0, TestLayout.ControlHeight, 2);
        entry.Height = AxisDimension.Fixed(2);
        entry.CellHeight = AxisDimension.Fixed(2);
        entry.CharsSize = new Size(TestLayout.ControlHeight, 2);
        entry.CellCharsSize = new Size(TestLayout.ControlHeight, 2);
        SetGeneratedLayout(new[] { entry });
    }
}

internal sealed class AlphaFocusScrollHostWindow : GeneratedWindowBase
{
    public AlphaFocusScrollHostWindow() : base("Alpha focus host")
    {
        GeneratedWindowStyle.Background = new Color("#303545");
        GeneratedWindowStyle.BorderWidthHorizontal = 0;
        GeneratedWindowStyle.BorderWidthVertical = 0;

        ReusableElement card = AddElement(new ReusableElement("card"));
        card.SetChild(new AlphaFocusScrollWindow());
        card.SetFocusStyle(new Style { Background = new Color("#ffffff14") });
        GeneratedLayoutEntry entry = TestLayout.FixedEntry("card", "uielement");
        entry.Relative = new Rect(0, 0, TestLayout.ControlHeight, 2);
        entry.SourceCell = new Rect(0, 0, TestLayout.ControlHeight, 2);
        entry.Height = AxisDimension.Fixed(2);
        entry.CellHeight = AxisDimension.Fixed(2);
        entry.CharsSize = new Size(TestLayout.ControlHeight, 2);
        entry.CellCharsSize = new Size(TestLayout.ControlHeight, 2);
        entry.CellStyle.Background = new Color("#303545");
        SetGeneratedLayout(new[] { entry });
    }
}

internal sealed class ModalMouseWindow : GeneratedWindowBase
{
    public TextInput Control { get; }

    public ModalMouseWindow(bool multiline) : base("Modal mouse")
    {
        GeneratedWindowStyle.BorderWidthHorizontal = 0;
        GeneratedWindowStyle.BorderWidthVertical = 0;
        Control = multiline
            ? AddElement(new TextArea("control", "alpha beta", 40))
            : AddElement(new TextInput("control", "abcdef", 20));
        int height = multiline ? 2 : 1;
        SetGeneratedLayout(new[]
        {
            Entry("control", multiline ? "textarea" : "textinput", new Rect(0, 0, 10, height)),
        });
    }

    private static GeneratedLayoutEntry Entry(string name, string type, Rect rect)
    {
        return new GeneratedLayoutEntry
        {
            Name = name,
            Type = type,
            Relative = rect,
            SourceCell = rect,
            Width = AxisDimension.Fixed(rect.Width),
            Height = AxisDimension.Fixed(rect.Height),
            CellWidth = AxisDimension.Fixed(rect.Width),
            CellHeight = AxisDimension.Fixed(rect.Height),
            CharsSize = new Size(rect.Width, rect.Height),
            CellCharsSize = new Size(rect.Width, rect.Height),
        };
    }
}

internal sealed class ConfirmFixture
{
    public ConfirmScrollWindow Child { get; }
    public McpController Controller { get; }
    public int EditStartedCount { get; private set; }

    public ConfirmFixture(bool keepEditModeAfterConfirm)
    {
        Child = new ConfirmScrollWindow();
        GeneratedWindowBase root = new("Scoped confirm");
        ReusableElement host = root.AddElement(new ReusableElement("items"));
        host.Frame = new Rect(0, 0, TestLayout.ControlWidth, TestLayout.ViewportHeight);
        host.SetChild(Child);
        GeneratedWindowRuntimeOptions options = new()
        {
            InitialFocusName = "items",
            KeepEditModeAfterConfirm = keepEditModeAfterConfirm,
            OnEditStarted = _ => EditStartedCount++,
        };
        Controller = new McpController(
            root,
            options,
            new McpRuntimeConfig
            {
                ViewportWidth = TestLayout.ViewportWidth,
                ViewportHeight = TestLayout.ViewportHeight,
            });
        root.SetFocus(Child.Filter);
    }
}

internal sealed class ConfirmScrollWindow : GeneratedScrollViewBase
{
    public Button Leading { get; }
    public TextInput Filter { get; }
    public int TrailingActivations { get; private set; }

    public ConfirmScrollWindow() : base("Items")
    {
        ScrollView panel = AddElement(new ScrollView("panel"));
        panel.Frame = new Rect(0, 0, TestLayout.ControlWidth, TestLayout.ViewportHeight);
        SetGeneratedScrollView(panel);
        Leading = panel.AddChild(new Button("leading", "Leading"));
        Leading.Frame = new Rect(0, 0, TestLayout.ControlWidth, TestLayout.ControlHeight);
        Filter = panel.AddChild(new TextInput("filter"));
        Filter.Frame = new Rect(1, 0, TestLayout.ControlWidth, TestLayout.ControlHeight);
        Button trailing = panel.AddChild(new Button("trailing", "Trailing"));
        trailing.Frame = new Rect(2, 0, TestLayout.ControlWidth, TestLayout.ControlHeight);
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        Leading.Enabled = false;
        return true;
    }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "trailing")
        {
            TrailingActivations++;
        }
        return true;
    }
}

internal static class TestLayout
{
    public const int ViewportWidth = 16;
    public const int ViewportHeight = 5;
    public const int ControlWidth = 12;
    public const int ControlHeight = 1;

    public static GeneratedLayoutEntry FixedEntry(string name, string type)
    {
        return new GeneratedLayoutEntry
        {
            Name = name,
            Type = type,
            Relative = new Rect(0, 0, ControlHeight, ControlHeight),
            SourceCell = new Rect(0, 0, ControlHeight, ControlHeight),
            Width = AxisDimension.Fixed(ControlHeight),
            Height = AxisDimension.Fixed(ControlHeight),
            CellWidth = AxisDimension.Fixed(ControlHeight),
            CellHeight = AxisDimension.Fixed(ControlHeight),
            CharsSize = new Size(ControlHeight, ControlHeight),
            CellCharsSize = new Size(ControlHeight, ControlHeight),
        };
    }
}
