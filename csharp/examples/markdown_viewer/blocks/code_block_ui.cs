// Auto-generated UI code for code_block - DO NOT EDIT MANUALLY.
using Uimd;

public class CodeBlockUI : GeneratedWindowBase
{
    public Label lang = null!;
    public Label code = null!;

    public CodeBlockUI() : base("Code Block")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "CodeBlock", """
# Code Block

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: stable
description: "Reusable markdown code block with a language header and selectable code text."
tags: [component]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
lang:
  type: label
  text: code
  description: "Language label for the code block."

code:
  type: spanlabel
  text: ""
  description: "Rendered code content with horizontal preservation and mouse selection enabled."
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
@lang:
  color: "#93c5fd"
@code:
  color: "#d1d5db"
  word-wrap: false
  user-select: text
```

## User Interface

```ui
+-head-**----------------------------------------+
| lang..**...................................... |
+-body-**----------------------------------------+
| code..**...................................... |
| .............................................. |
# #............................................ |
# #............................................ |
+------------------------------------------------+
```

""", "Reusable markdown code block with a language header and selectable code text.", new[]
        {
            new GeneratedElementMetadata { Name = "lang", Description = "Language label for the code block.", Expose = true },
            new GeneratedElementMetadata { Name = "code", Description = "Rendered code content with horizontal preservation and mouse selection enabled.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        lang = AddElement(new Label("lang", "code"));
        lang.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        lang.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        code = AddElement(new Label("code", ""));
        code.SetStyle(new Style
    {
        Color = new Color("#d1d5db"),
        UserSelect = "text"
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "code",
                Type = "spanlabel",
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
        Color = new Color("#d1d5db"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "lang",
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
