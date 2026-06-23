// Auto-generated UI code for table_block - DO NOT EDIT MANUALLY.
using Uimd;

public class TableBlockUI : GeneratedWindowBase
{
    public Label kind = null!;
    public MessageTable table = null!;

    public TableBlockUI() : base("Table Block")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "TableBlock", """
# Table Block

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: stable
description: "Reusable markdown table block with a small header and message table content."
tags: [component]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
kind:
  type: label
  text: "markdown table"
  description: "Header label identifying the block as a markdown table."

table:
  type: messagetable
  text: ""
  description: "Rendered markdown table content."
```

## Style

```yaml
this:
  background: "#00000000"
  border-width: 0
@head:
  background: "#1d314c"
@body:
  background: "#0c1a2d"
@kind:
  color: "#93c5fd"
@table:
  color: "#e5e7eb"
```

## User Interface

```ui
+-head-**----------------------------------------+
| kind..**...................................... |
+-body-**----------------------------------------+
| table..**..................................... |
| .............................................. |
# #............................................ |
# #............................................ |
+------------------------------------------------+
```

""", "Reusable markdown table block with a small header and message table content.", new[]
        {
            new GeneratedElementMetadata { Name = "kind", Description = "Header label identifying the block as a markdown table.", Expose = true },
            new GeneratedElementMetadata { Name = "table", Description = "Rendered markdown table content.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        kind = AddElement(new Label("kind", "markdown table"));
        kind.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        kind.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        table = AddElement(new MessageTable("table", ""));
        table.SetStyle(new Style
    {
        Color = new Color("#e5e7eb")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "kind",
                Type = "label",
                CellName = "head",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 48, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(46, 1),
                CellCharsSize = new Size(48, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1d314c")
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "table",
                Type = "messagetable",
                CellName = "body",
                Relative = new Rect(0, 1, 536870911, 536870910),
                SourceCell = new Rect(2, 0, 48, 4),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.FitContent(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.FitContent(),
                CharsSize = new Size(46, 4),
                CellCharsSize = new Size(48, 4),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#0c1a2d")
    },
                ElementStyle = new Style
    {
        Color = new Color("#e5e7eb")
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
