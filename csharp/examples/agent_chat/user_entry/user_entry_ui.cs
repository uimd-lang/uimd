// Auto-generated UI code for user_entry - DO NOT EDIT MANUALLY.
using Uimd;

public class UserEntryUI : GeneratedWindowBase
{
    public Label body = null!;

    public UserEntryUI() : base("Agent Chat User Entry")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "UserEntry", """
# Agent Chat User Entry

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Heron-style user message with a separate accent."
tags: [component, simulation]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
body:
  type: spanlabel
  text: ""
  description: "Selectable user prompt."
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@panel:
  background: "#111827"
  padding: 0, 1, 0, 1
@body:
  color: "#e0f2fe"
  word-wrap: true
  user-select: text
```

## User Interface

```ui
+-panel-**-----------------------------------------------------------------+
| body..**................................................................ |
| ........................................................................ |
# #....................................................................... |
# #....................................................................... |
+--------------------------------------------------------------------------+
```

""", "Heron-style user message with a separate accent.", new[]
        {
            new GeneratedElementMetadata { Name = "body", Description = "Selectable user prompt.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        body = AddElement(new Label("body", ""));
        body.SetStyle(new Style
    {
        Color = new Color("#e0f2fe"),
        UserSelect = "text"
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "body",
                Type = "spanlabel",
                CellName = "panel",
                Relative = new Rect(0, 1, 536870911, 536870910),
                SourceCell = new Rect(0, 0, 74, 4),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.FitContent(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(72, 4),
                CellCharsSize = new Size(74, 4),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#111827"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Color = new Color("#e0f2fe"),
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
