// Auto-generated UI code for table_entry - DO NOT EDIT MANUALLY.
using Uimd;

public class TableEntryUI : GeneratedWindowBase
{
    public Label heading = null!;
    public MessageTable table = null!;

    public TableEntryUI() : base("Agent Chat Table Entry")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "TableEntry", """
# Agent Chat Table Entry

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Heron-style simulated agent Markdown table block."
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
  text: "Table"
  description: "Table title."

table:
  type: messagetable
  text: ""
  description: "Rendered Markdown table content."
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@table_panel:
  background: "#00000000"
  padding: 0, 1, 0, 1
@heading:
  color: "#ddd6fe"
@table:
  color: "#cbd5e1"
```

## User Interface

```ui
+-table_panel-**----------------------------------------------------------+
| heading..**........................................................... .|
| table..**.............................................................. |
| ....................................................................... |
# #...................................................................... |
# #...................................................................... |
+-------------------------------------------------------------------------+
```

""", "Heron-style simulated agent Markdown table block.", new[]
        {
            new GeneratedElementMetadata { Name = "heading", Description = "Table title.", Expose = true },
            new GeneratedElementMetadata { Name = "table", Description = "Rendered Markdown table content.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        heading = AddElement(new Label("heading", "Table"));
        heading.SetStyle(new Style
    {
        Color = new Color("#ddd6fe")
    });
        table = AddElement(new MessageTable("table", ""));
        table.SetStyle(new Style
    {
        Color = new Color("#cbd5e1")
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
                CellName = "table_panel",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 73, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(70, 1),
                CellCharsSize = new Size(73, 5),
                MarginRight = 2,
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
        Color = new Color("#ddd6fe")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "table",
                Type = "messagetable",
                CellName = "table_panel",
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
        Color = new Color("#cbd5e1")
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
