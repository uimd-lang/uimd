// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
using Uimd;

public class ChatEntryUI : GeneratedWindowBase
{
    public Label heading = null!;
    public Label body = null!;

    public ChatEntryUI() : base("Agent Chat Entry")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ChatEntry", """
# Agent Chat Entry

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Heron-style assistant text entry."
tags: [component, simulation]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
heading:
  type: spanlabel
  text: "Assistant"
  description: "Assistant role label."

body:
  type: spanlabel
  text: ""
  description: "Selectable Markdown-style transcript prose."

```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@body_panel:
  background: "#00000000"
  padding: 0, 1, 0, 1
@heading:
  color: "#a7f3d0"
  user-select: text
@body:
  color: "#cbd5e1"
  word-wrap: true
  user-select: text
```

## User Interface

```ui
+-body_panel-**-----------------------------------------------------------+
| heading..**............................................................ |
| body..**............................................................... |
| ....................................................................... |
# #...................................................................... |
# #...................................................................... |
+-------------------------------------------------------------------------+
```

""", "Heron-style assistant text entry.", new[]
        {
            new GeneratedElementMetadata { Name = "heading", Description = "Assistant role label.", Expose = true },
            new GeneratedElementMetadata { Name = "body", Description = "Selectable Markdown-style transcript prose.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        heading = AddElement(new Label("heading", "Assistant"));
        heading.SetStyle(new Style
    {
        Color = new Color("#a7f3d0"),
        UserSelect = "text"
    });
        body = AddElement(new Label("body", ""));
        body.SetStyle(new Style
    {
        Color = new Color("#cbd5e1"),
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
                Type = "spanlabel",
                CellName = "body_panel",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 73, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(71, 1),
                CellCharsSize = new Size(73, 5),
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
        Color = new Color("#a7f3d0"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "body",
                Type = "spanlabel",
                CellName = "body_panel",
                Relative = new Rect(1, 1, 536870911, 536870910),
                SourceCell = new Rect(0, 0, 73, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.FitContent(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(71, 4),
                CellCharsSize = new Size(73, 5),
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
        Color = new Color("#cbd5e1"),
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
