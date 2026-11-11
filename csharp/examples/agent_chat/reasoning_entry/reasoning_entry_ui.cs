// Auto-generated UI code for reasoning_entry - DO NOT EDIT MANUALLY.
using Uimd;

public class ReasoningEntryUI : GeneratedWindowBase
{
    public Label heading = null!;
    public Label body = null!;

    public ReasoningEntryUI() : base("Agent Chat Reasoning Entry")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ReasoningEntry", """
# Agent Chat Reasoning Entry

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Animated Heron-style thinking and status message."
tags: [component, simulation, reasoning]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
heading:
  type: label
  text: "Thinking"
  description: "Animated thinking or activity phase."

body:
  type: spanlabel
  text: ""
  description: "Visible simulated reasoning detail."
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@panel:
  background: "#00000000"
  padding: 0, 1, 0, 1
@heading:
  color: "#e5e7eb"
  text-color-gradient:
    interval: 90
    step: 1
    segment-size: 1
    colors: ["#000000aa", "#00000077", "#00000044", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000044", "#00000077", "#000000aa"]
@body:
  color: "#94a3b8"
  word-wrap: true
  user-select: text
```

## User Interface

```ui
+-panel-**-----------------------------------------------------------------+
| heading..**............................................................. |
| body..**................................................................ |
| ........................................................................ |
# #....................................................................... |
# #....................................................................... |
+--------------------------------------------------------------------------+
```

""", "Animated Heron-style thinking and status message.", new[]
        {
            new GeneratedElementMetadata { Name = "heading", Description = "Animated thinking or activity phase.", Expose = true },
            new GeneratedElementMetadata { Name = "body", Description = "Visible simulated reasoning detail.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        heading = AddElement(new Label("heading", "Thinking"));
        heading.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#e5e7eb"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 90,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#000000aa"), new Color("#00000077"), new Color("#00000044"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000044"), new Color("#00000077"), new Color("#000000aa")}
    }
    });
        heading.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        body = AddElement(new Label("body", ""));
        body.SetStyle(new Style
    {
        Color = new Color("#94a3b8"),
        UserSelect = "text"
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "heading",
                Type = "label",
                CellName = "panel",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 74, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(72, 1),
                CellCharsSize = new Size(74, 5),
                MarginRight = 1,
                MarginBottom = 4,
                CellStyle = new Style
    {
        Background = new Color("#00000000"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#e5e7eb"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 90,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#000000aa"), new Color("#00000077"), new Color("#00000044"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000044"), new Color("#00000077"), new Color("#000000aa")}
    }
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "body",
                Type = "spanlabel",
                CellName = "panel",
                Relative = new Rect(1, 1, 536870911, 536870910),
                SourceCell = new Rect(0, 0, 74, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.FitContent(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(72, 4),
                CellCharsSize = new Size(74, 5),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#00000000"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Color = new Color("#94a3b8"),
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
