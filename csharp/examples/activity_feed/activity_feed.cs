using System.Globalization;
using Uimd;

internal readonly record struct SampleActivity(string Type, string Message);

public readonly record struct SettingsResult(bool AutoScroll, bool ShowTimestamps, string DefaultType);

public sealed class SettingsDialog : SettingsUI
{
    public Action<SettingsResult, bool>? OnClosed { get; set; }

    public void Configure(SettingsResult settings)
    {
        auto_scroll.SetChecked(settings.AutoScroll);
        show_timestamps.SetChecked(settings.ShowTimestamps);
        SelectDefaultType(settings.DefaultType);
    }

    protected override void onSaveButtonClick()
    {
        OnClosed?.Invoke(Result(), true);
        CloseWindow();
    }

    protected override void onCancelButtonClick()
    {
        OnClosed?.Invoke(Result(), false);
        CloseWindow();
    }

    private SettingsResult Result()
    {
        return new SettingsResult(auto_scroll.Checked, show_timestamps.Checked, default_type.SelectedText);
    }

    private void SelectDefaultType(string value)
    {
        int index = default_type.Options.IndexOf(value);
        default_type.SetSelectedIndex(index >= 0 ? index : 0);
    }
}

public sealed class ActivityFeed : ActivityFeedUI
{
    private const string DefaultActivityType = "Info";
    private const string TimestampEnv = "UIMD_ACTIVITY_FEED_TIMESTAMP";
    private const int InitialSampleCount = 2;
    private static readonly SampleActivity[] SampleActivities =
    {
        new("Info", "Workspace opened"),
        new("Task", "Report export queued"),
        new("Deploy", "Version 1.4 deployed"),
        new("Warning", "Review queue is almost full"),
        new("Note", "Follow-up scheduled"),
        new("Task", "Background sync finished"),
    };

    private readonly ActivityFeedPanel feedPanel;
    private bool autoScroll = true;
    private bool showTimestamps = true;
    private bool quitRequested;
    private string defaultType = DefaultActivityType;

    public ActivityFeed()
    {
        feedPanel = new ActivityFeedPanel();
        feed.SetChild(feedPanel);
    }

    public void Open()
    {
        SelectActivityType(defaultType);
        _ = GeneratedWindowRuntime.RenderGeneratedWindowContent(this, GeneratedWindowRuntime.GeneratedWindowContentSize(this));
        feedPanel.SetAutoScroll(autoScroll);
        if (feedPanel.ActivityCount() == 0)
        {
            SeedInitial();
        }
    }

    protected override void onAddButtonClick()
    {
        AddCurrentActivity();
    }

    protected override void onSeedButtonClick()
    {
        SeedActivities();
    }

    protected override void onClearButtonClick()
    {
        ClearActivities();
    }

    protected override void onSettingsButtonClick()
    {
        OpenSettings();
    }

    protected override void onQuitButtonClick()
    {
        quitRequested = true;
    }

    protected override void onMessageSubmit(string value)
    {
        message.SetValue(value);
    }

    protected override bool shouldClose()
    {
        return quitRequested;
    }

    private void AddCurrentActivity()
    {
        string activityMessage = message.Value.Trim();
        if (activityMessage.Length == 0)
        {
            status.SetText("Enter an activity message.");
            return;
        }
        AppendActivity(activity_type.SelectedText, activityMessage);
        message.SetValue("");
        message.SetCursor(0);
        UpdateCountStatus();
    }

    private void AppendActivity(string eventType, string activityMessage)
    {
        feedPanel.AppendActivity(
            CurrentTimestamp(),
            string.IsNullOrEmpty(eventType) ? defaultType : eventType,
            activityMessage,
            showTimestamps);
    }

    private void SeedActivities()
    {
        foreach (SampleActivity activity in SampleActivities)
        {
            AppendActivity(activity.Type, activity.Message);
        }
        UpdateCountStatus();
    }

    private void ClearActivities()
    {
        feedPanel.ClearActivities();
        status.SetText("Feed cleared");
    }

    private void OpenSettings()
    {
        SettingsDialog dialog = new();
        dialog.Configure(new SettingsResult(autoScroll, showTimestamps, defaultType));
        dialog.OnClosed = ApplySettingsResult;
        OpenWindow(dialog);
    }

    private void ApplySettingsResult(SettingsResult settings, bool saved)
    {
        if (!saved)
        {
            status.SetText("Settings canceled");
            return;
        }
        autoScroll = settings.AutoScroll;
        showTimestamps = settings.ShowTimestamps;
        defaultType = string.IsNullOrEmpty(settings.DefaultType) ? DefaultActivityType : settings.DefaultType;
        SelectActivityType(defaultType);
        feedPanel.SetAutoScroll(autoScroll);
        status.SetText("Settings saved");
    }

    private void SeedInitial()
    {
        for (int index = 0; index < Math.Min(InitialSampleCount, SampleActivities.Length); ++index)
        {
            AppendActivity(SampleActivities[index].Type, SampleActivities[index].Message);
        }
        UpdateCountStatus();
    }

    private void UpdateCountStatus()
    {
        status.SetText(feedPanel.ActivityCount() + " activities");
    }

    private void SelectActivityType(string value)
    {
        int index = activity_type.Options.IndexOf(value);
        activity_type.SetSelectedIndex(index >= 0 ? index : 0);
    }

    private static string CurrentTimestamp()
    {
        string? fixedTimestamp = Environment.GetEnvironmentVariable(TimestampEnv);
        if (!string.IsNullOrEmpty(fixedTimestamp))
        {
            return fixedTimestamp;
        }
        return DateTime.Now.ToString("HH:mm", CultureInfo.InvariantCulture);
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            ActivityFeed app = new();
            app.Open();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
