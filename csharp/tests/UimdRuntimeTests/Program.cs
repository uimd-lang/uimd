using Uimd;

const int RegressionCount = 2;
int passed = 0;
passed += RunRegression("ScrollView scoped confirm keeps a fresh edit session", RunKeepEditModeRegression);
passed += RunRegression("ScrollView scoped confirm rebases focus after mutation", RunFocusRebaseRegression);
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

static void Require(bool condition, string message)
{
    if (!condition)
    {
        throw new InvalidOperationException(message);
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
}
