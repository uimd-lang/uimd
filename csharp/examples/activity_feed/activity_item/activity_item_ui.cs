// Auto-generated UI code for activity_item - DO NOT EDIT MANUALLY.
using Uimd;

public class ActivityItemUI : GeneratedWindowBase
{
    public Label timestamp = null!;
    public Label event_type = null!;
    public Label message = null!;

    public ActivityItemUI() : base("Activity Item")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ActivityItem", """
# Activity Item

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Reusable activity feed item with timestamp, event type, and message text."
tags: [component]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
timestamp:
  type: label
  text: "10:42"
  description: "Optional activity timestamp."

event_type:
  type: label
  text: Info
  description: "Activity category label."

message:
  type: label
  text: "Activity message"
  description: "Selectable activity message body."
```

## Style

```yaml
this:
  border-width: 0
@marker:
  background-texture: "▐"
  background-texture-color: "#38bdf8"
@marker_info:
  background-texture-color: "#38bdf8"
@marker_task:
  background-texture-color: "#a78bfa"
@marker_warning:
  background-texture-color: "#facc15"
@marker_deploy:
  background-texture-color: "#22c55e"
@marker_note:
  background-texture-color: "#fb7185"
@body:
  background: "#1f2e46"
@timestamp:
  color: "#94a3b8"
  user-select: text
@event_type:
  color: "#bfdbfe"
  user-select: text
@message:
  color: "#ffffff"
  user-select: text
```

## User Interface

```ui
+-marker-1-+-body-**----------------------------------+
|          | timestamp.. event_type..**               |
|          | message..**                              |
|          | ........................................ |
|          | ........................................ |
#          # #                                        |
#          # #                                        |
+----------+------------------------------------------+
```

""", "Reusable activity feed item with timestamp, event type, and message text.", new[]
        {
            new GeneratedElementMetadata { Name = "timestamp", Description = "Optional activity timestamp.", Expose = true },
            new GeneratedElementMetadata { Name = "event_type", Description = "Activity category label.", Expose = true },
            new GeneratedElementMetadata { Name = "message", Description = "Selectable activity message body.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        timestamp = AddElement(new Label("timestamp", "10:42"));
        timestamp.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#94a3b8"),
        UserSelect = "text"
    });
        timestamp.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        event_type = AddElement(new Label("event_type", "Info"));
        event_type.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe"),
        UserSelect = "text"
    });
        event_type.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        message = AddElement(new Label("message", "Activity message"));
        message.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        UserSelect = "text"
    });
        message.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "",
                Type = "",
                CellName = "marker",
                Relative = new Rect(0, 0, 1, 536870910),
                SourceCell = new Rect(0, 0, 10, 6),
                Width = AxisDimension.Auto(1),
                Height = AxisDimension.Auto(536870910),
                CellWidth = AxisDimension.Fixed(1),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(10, 6),
                CellCharsSize = new Size(10, 6),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        BackgroundTexture = "▐",
        BackgroundTextureColor = new Color("#38bdf8")
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "timestamp",
                Type = "label",
                CellName = "body",
                Relative = new Rect(0, 1, 11, 1),
                SourceCell = new Rect(0, 11, 42, 6),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(42, 6),
                MarginRight = 30,
                MarginBottom = 5,
                CellStyle = new Style
    {
        Background = new Color("#1f2e46")
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#94a3b8"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "event_type",
                Type = "label",
                CellName = "body",
                Relative = new Rect(0, 13, 536870911, 1),
                SourceCell = new Rect(0, 11, 42, 6),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(14, 1),
                CellCharsSize = new Size(42, 6),
                MarginRight = 15,
                MarginBottom = 5,
                CellStyle = new Style
    {
        Background = new Color("#1f2e46")
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "message",
                Type = "label",
                CellName = "body",
                Relative = new Rect(1, 1, 536870911, 536870910),
                SourceCell = new Rect(0, 11, 42, 6),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.FitContent(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(11, 5),
                CellCharsSize = new Size(42, 6),
                MarginRight = 30,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2e46")
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        UserSelect = "text"
    },
            },
        };
    }

    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        return false;
    }
}
