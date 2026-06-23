// Auto-generated UI code for heading - DO NOT EDIT MANUALLY.
using Uimd;

public class HeadingUI : GeneratedWindowBase
{
    public Label heading_text = null!;

    public HeadingUI() : base("Heading")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "Heading", """
# Heading

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: stable
description: "Reusable markdown heading block."
tags: [component]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
heading_text:
  type: spanlabel
  text: ""
  description: "Rendered heading text with mouse selection enabled."
```

## Style

```yaml
this:
  background: "#00000000"
  border-width: 0
@heading_cell:
  background: "#00000000"
@heading_text:
  color: "#dbeafe"
  user-select: text
```

## User Interface

```ui
+-heading_cell-**--------------------------------+
| heading_text..**.............................. |
+------------------------------------------------+
```

""", "Reusable markdown heading block.", new[]
        {
            new GeneratedElementMetadata { Name = "heading_text", Description = "Rendered heading text with mouse selection enabled.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        heading_text = AddElement(new Label("heading_text", ""));
        heading_text.SetStyle(new Style
    {
        Color = new Color("#dbeafe"),
        UserSelect = "text"
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "heading_text",
                Type = "spanlabel",
                CellName = "heading_cell",
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
        Background = new Color("#00000000")
    },
                ElementStyle = new Style
    {
        Color = new Color("#dbeafe"),
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
