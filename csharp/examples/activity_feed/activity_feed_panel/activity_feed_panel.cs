using Uimd;

internal readonly record struct ActivityItemData(string Timestamp, string EventType, string Message, bool ShowTimestamp);

public sealed class ActivityFeedPanel : ActivityFeedPanelUI
{
    private readonly List<ActivityItemData> activities = new();

    public ActivityFeedPanel()
    {
        SetDynamicChildrenRenderer(RenderActivities);
    }

    public void ClearActivities()
    {
        activities.Clear();
        ClearChildren();
        InvalidateDynamicChildren();
    }

    public void AppendActivity(string timestamp, string eventType, string message, bool showTimestamp)
    {
        activities.Add(new ActivityItemData(timestamp, eventType, message, showTimestamp));
        InvalidateDynamicChildren();
    }

    public void SetAutoScroll(bool enabled)
    {
        ScrollView().SetAutoScroll(enabled);
    }

    public int ActivityCount()
    {
        return activities.Count;
    }

    private List<List<List<TerminalCell>>> RenderActivities(int width)
    {
        List<List<List<TerminalCell>>> rendered = new();
        foreach (ActivityItemData activity in activities)
        {
            rendered.Add(RenderActivityItem(activity, Math.Max(1, width)));
        }
        return rendered;
    }

    private static List<List<TerminalCell>> RenderActivityItem(ActivityItemData item, int width)
    {
        ActivityItemUI view = new();
        view.timestamp.SetText(item.ShowTimestamp ? item.Timestamp : "");
        view.event_type.SetText(item.EventType);
        view.message.SetText(item.Message);

        int sourceHeight = GeneratedSourceHeight(view);
        _ = GeneratedWindowRuntime.RenderGeneratedWindowContent(view, new Size(Math.Max(1, width), sourceHeight));
        int messageRows = WrappedLineCount(item.Message, Math.Max(1, view.message.Frame.Width));
        List<List<TerminalCell>> rendered = GeneratedWindowRuntime.RenderGeneratedWindowContent(
            view,
            new Size(
                Math.Max(1, width),
                CompactContentHeight(view, messageRows)));
        ApplyLeftMarker(rendered, MarkerColor(item.EventType));
        return rendered;
    }

    private static int GeneratedSourceHeight(GeneratedWindowBase view)
    {
        int height = 0;
        foreach (GeneratedLayoutEntry entry in view.GeneratedLayout)
        {
            height = Math.Max(height, entry.SourceCell.Row + entry.SourceCell.Height);
        }
        return Math.Max(1, height);
    }

    private static int CompactContentHeight(GeneratedWindowBase view, int messageRows)
    {
        int height = 1;
        foreach (GeneratedLayoutEntry entry in view.GeneratedLayout)
        {
            if (string.IsNullOrEmpty(entry.Name))
            {
                continue;
            }
            int entryHeight = entry.Name == "message"
                ? Math.Max(1, messageRows)
                : Math.Max(1, entry.CharsSize.Height);
            height = Math.Max(height, entry.Relative.Row + entryHeight);
        }
        return height;
    }

    private static int WrappedLineCount(string text, int width)
    {
        int safeWidth = Math.Max(1, width);
        int count = 0;
        foreach (string line in SplitLines(text))
        {
            count += Math.Max(1, (line.Length + safeWidth - 1) / safeWidth);
        }
        return Math.Max(1, count);
    }

    private static IEnumerable<string> SplitLines(string text)
    {
        string[] lines = text.Split('\n');
        return lines.Length == 0 ? new[] { "" } : lines;
    }

    private static Color MarkerColor(string eventType)
    {
        return eventType switch
        {
            "Task" => new Color("#a78bfa"),
            "Warning" => new Color("#facc15"),
            "Deploy" => new Color("#22c55e"),
            "Note" => new Color("#fb7185"),
            _ => new Color("#38bdf8"),
        };
    }

    private static void ApplyLeftMarker(List<List<TerminalCell>> rendered, Color color)
    {
        foreach (List<TerminalCell> row in rendered)
        {
            if (row.Count == 0)
            {
                continue;
            }
            row[0].Text = "▐";
            row[0].Foreground = color;
        }
    }
}
