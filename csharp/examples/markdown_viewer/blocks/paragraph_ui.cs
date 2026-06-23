// Auto-generated UI code for paragraph - DO NOT EDIT MANUALLY.
using Uimd;

public class ParagraphUI : GeneratedWindowBase
{
    public Label text = null!;

    public ParagraphUI() : base("Paragraph")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "Paragraph", """
# Paragraph

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: stable
description: "Reusable markdown paragraph block."
tags: [component]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
text:
  type: label
  text: ""
  description: "Rendered paragraph text with mouse selection enabled."
```

## Style

```yaml
this:
  background: "#00000000"
  border-width: 0
@text:
  color: "#cbd5e1"
  user-select: text
```

## User Interface

```ui
+-body-**----------------------------------------+
| text..**...................................... |
| .............................................. |
# #............................................ |
+------------------------------------------------+
```

""", "Reusable markdown paragraph block.", new[]
        {
            new GeneratedElementMetadata { Name = "text", Description = "Rendered paragraph text with mouse selection enabled.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        text = AddElement(new Label("text", ""));
        text.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        UserSelect = "text"
    });
        text.SetCursorStyle(new Style
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
                Name = "text",
                Type = "label",
                CellName = "body",
                Relative = new Rect(0, 1, 536870911, 3),
                SourceCell = new Rect(0, 0, 48, 3),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(46, 3),
                CellCharsSize = new Size(48, 3),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
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
