using System.Globalization;
using Uimd;

public sealed class WidgetGallery : WidgetGalleryUI
{
    private const string DefaultName = "Ada Lovelace";
    private const int DefaultCount = 3;
    private bool finished;

    public WidgetGallery()
    {
        RefreshSummary();
    }

    protected override void onResetBtnClick()
    {
        ResetValues();
    }

    protected override void onApplyBtnClick()
    {
        RefreshSummary();
    }

    protected override void onCloseBtnClick()
    {
        finished = true;
    }

    protected override void onNameInputChange(string value)
    {
    }

    protected override void onCountInputChange(string value)
    {
    }

    protected override void onEnabledCheckChange(string value)
    {
    }

    protected override void onThemeComboChange(string value)
    {
    }

    protected override void onModeListSelectionChange(List<string> value)
    {
    }

    protected override bool shouldClose()
    {
        return finished;
    }

    private string SummaryText()
    {
        string theme = theme_combo.SelectedText;
        string mode = mode_list.SelectedValues.FirstOrDefault() ?? "";
        return "values:\n  name: " + name_input.Value +
            "\n  count: " + count_input.Value.ToString("0.##########", CultureInfo.InvariantCulture) +
            "\n  theme: " + theme +
            "\n  mode: " + mode +
            "\n  enabled: " + (enabled_check.Checked ? "True" : "False");
    }

    private void RefreshSummary()
    {
        spans.SetSpans(new[]
        {
            new LabelSpan("SpanLabel", "#facc15"),
            new LabelSpan(" can mix "),
            new LabelSpan("foreground", "#38bdf8"),
            new LabelSpan(" and "),
            new LabelSpan("background", "#111827", "#f97316"),
            new LabelSpan(" colors."),
        });
        summary.SetText(SummaryText());
    }

    private void ResetValues()
    {
        name_input.SetValue(DefaultName);
        count_input.SetValue(DefaultCount);
        enabled_check.SetChecked(true);
        theme_combo.SetSelectedIndex(0);
        mode_list.SetSelectedIndex(0);
        RefreshSummary();
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            WidgetGallery app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
