// Auto-generated UI code for code_entry - DO NOT EDIT MANUALLY.
using Uimd;

public class CodeEntryUI : GeneratedWindowBase
{
    public Label heading = null!;
    public Label code = null!;

    public CodeEntryUI() : base("Agent Chat Code Entry")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "CodeEntry", """
# Agent Chat Code Entry

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Heron-style code, activity, command output, or diff block."
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
  text: "Code"
  description: "Code language or activity title."

code:
  type: label
  text: ""
  description: "Selectable formatted code, command output, or diff content."
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@code_panel:
  background: "#0f1b2d"
  padding: 0, 1, 0, 1
@heading:
  color: "#ddd6fe"
@code:
  color: "#d1d5db"
  word-wrap: true
  user-select: text
```

## User Interface

```ui
+-code_panel-**-----------------------------------------------------------+
| heading..**............................................................ |
| code..**............................................................... |
| ....................................................................... |
# #...................................................................... |
# #...................................................................... |
+-------------------------------------------------------------------------+
```

""", "Heron-style code, activity, command output, or diff block.", new[]
        {
            new GeneratedElementMetadata { Name = "heading", Description = "Code language or activity title.", Expose = true },
            new GeneratedElementMetadata { Name = "code", Description = "Selectable formatted code, command output, or diff content.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        heading = AddElement(new Label("heading", "Code"));
        heading.SetStyle(new Style
    {
        Color = new Color("#ddd6fe")
    });
        code = AddElement(new Label("code", ""));
        code.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db"),
        UserSelect = "text"
    });
        code.SetCursorStyle(new Style
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
                Name = "heading",
                Type = "spanlabel",
                CellName = "code_panel",
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
        Background = new Color("#0f1b2d"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Color = new Color("#ddd6fe")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "code",
                Type = "label",
                CellName = "code_panel",
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
        Background = new Color("#0f1b2d"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db"),
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
