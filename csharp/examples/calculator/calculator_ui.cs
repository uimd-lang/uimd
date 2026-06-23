// Auto-generated UI code for calculator - DO NOT EDIT MANUALLY.
using Uimd;

public class CalculatorUI : GeneratedWindowBase
{
    public Label display = null!;
    public Button n0 = null!;
    public Button n1 = null!;
    public Button n2 = null!;
    public Button n3 = null!;
    public Button n4 = null!;
    public Button n5 = null!;
    public Button n6 = null!;
    public Button n7 = null!;
    public Button n8 = null!;
    public Button n9 = null!;
    public Button sb = null!;
    public Button dt = null!;
    public Button ac = null!;
    public Button dl = null!;
    public Button pr = null!;
    public Button dv = null!;
    public Button ml = null!;
    public Button mn = null!;
    public Button ad = null!;

    public CalculatorUI() : base("Calculator")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "Calculator", """
# Calculator

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression."
tags: [example]
```

## Members

```yaml
display:
  type: label
  text: ""
  description: "Shows the current number, operator result, or calculation state."

n0:
  type: button
  title: 0
  description: "Enter digit 0."

n1:
  type: button
  title: 1
  description: "Enter digit 1."

n2:
  type: button
  title: 2
  description: "Enter digit 2."

n3:
  type: button
  title: 3
  description: "Enter digit 3."

n4:
  type: button
  title: 4
  description: "Enter digit 4."

n5:
  type: button
  title: 5
  description: "Enter digit 5."

n6:
  type: button
  title: 6
  description: "Enter digit 6."

n7:
  type: button
  title: 7
  description: "Enter digit 7."

n8:
  type: button
  title: 8
  description: "Enter digit 8."

n9:
  type: button
  title: 9
  description: "Enter digit 9."

sb:
  type: button
  title: "="
  description: "Evaluate the pending calculation."

dt:
  type: button
  title: .
  description: "Insert a decimal point into the current number."

ac:
  type: button
  title: AC
  description: "Clear the current calculator state."

dl:
  type: button
  title: DEL
  description: "Delete the last entered character."

pr:
  type: button
  title: "%"
  description: "Convert or apply percentage behavior for the current value."

dv:
  type: button
  title: /
  description: "Choose division as the pending operator."

ml:
  type: button
  title: "*"
  description: "Choose multiplication as the pending operator."

mn:
  type: button
  title: -
  description: "Choose subtraction as the pending operator."

ad:
  type: button
  title: "+"
  description: "Choose addition as the pending operator."
```

## Style

```yaml
@display:
  background: #32446d
  color: #c8e0ff
  text-align: right
```

## User Interface

```ui
+----------------------+
|                      |
|  display...........  |
|                      |
|  ac.  dl.  pr.  dv.  |
|                      |
|  n7.  n8.  n9.  ml.  |
|                      |
|  n4.  n5.  n6.  mn.  |
|                      |
|  n1.  n2.  n3.  sb.  |
|                      |
|  n0......  dt.  ad.  |
|                      |
+----------------------+
```

""", "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.", new[]
        {
            new GeneratedElementMetadata { Name = "display", Description = "Shows the current number, operator result, or calculation state.", Expose = true },
            new GeneratedElementMetadata { Name = "n0", Description = "Enter digit 0.", Expose = true },
            new GeneratedElementMetadata { Name = "n1", Description = "Enter digit 1.", Expose = true },
            new GeneratedElementMetadata { Name = "n2", Description = "Enter digit 2.", Expose = true },
            new GeneratedElementMetadata { Name = "n3", Description = "Enter digit 3.", Expose = true },
            new GeneratedElementMetadata { Name = "n4", Description = "Enter digit 4.", Expose = true },
            new GeneratedElementMetadata { Name = "n5", Description = "Enter digit 5.", Expose = true },
            new GeneratedElementMetadata { Name = "n6", Description = "Enter digit 6.", Expose = true },
            new GeneratedElementMetadata { Name = "n7", Description = "Enter digit 7.", Expose = true },
            new GeneratedElementMetadata { Name = "n8", Description = "Enter digit 8.", Expose = true },
            new GeneratedElementMetadata { Name = "n9", Description = "Enter digit 9.", Expose = true },
            new GeneratedElementMetadata { Name = "sb", Description = "Evaluate the pending calculation.", Expose = true },
            new GeneratedElementMetadata { Name = "dt", Description = "Insert a decimal point into the current number.", Expose = true },
            new GeneratedElementMetadata { Name = "ac", Description = "Clear the current calculator state.", Expose = true },
            new GeneratedElementMetadata { Name = "dl", Description = "Delete the last entered character.", Expose = true },
            new GeneratedElementMetadata { Name = "pr", Description = "Convert or apply percentage behavior for the current value.", Expose = true },
            new GeneratedElementMetadata { Name = "dv", Description = "Choose division as the pending operator.", Expose = true },
            new GeneratedElementMetadata { Name = "ml", Description = "Choose multiplication as the pending operator.", Expose = true },
            new GeneratedElementMetadata { Name = "mn", Description = "Choose subtraction as the pending operator.", Expose = true },
            new GeneratedElementMetadata { Name = "ad", Description = "Choose addition as the pending operator.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        display = AddElement(new Label("display", ""));
        display.SetStyle(new Style
    {
        Background = new Color("#32446d"),
        Color = new Color("#c8e0ff"),
        TextAlign = "right"
    });
        display.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        n0 = AddElement(new Button("n0", "0"));
        n0.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n0.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n0.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n1 = AddElement(new Button("n1", "1"));
        n1.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n1.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n1.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n2 = AddElement(new Button("n2", "2"));
        n2.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n2.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n2.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n3 = AddElement(new Button("n3", "3"));
        n3.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n3.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n3.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n4 = AddElement(new Button("n4", "4"));
        n4.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n4.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n4.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n5 = AddElement(new Button("n5", "5"));
        n5.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n5.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n5.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n6 = AddElement(new Button("n6", "6"));
        n6.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n6.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n6.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n7 = AddElement(new Button("n7", "7"));
        n7.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n7.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n7.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n8 = AddElement(new Button("n8", "8"));
        n8.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n8.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n8.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        n9 = AddElement(new Button("n9", "9"));
        n9.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        n9.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        n9.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        sb = AddElement(new Button("sb", "="));
        sb.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        sb.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        sb.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        dt = AddElement(new Button("dt", "."));
        dt.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        dt.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        dt.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        ac = AddElement(new Button("ac", "AC"));
        ac.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        ac.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        ac.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        dl = AddElement(new Button("dl", "DEL"));
        dl.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        dl.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        dl.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        pr = AddElement(new Button("pr", "%"));
        pr.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        pr.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        pr.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        dv = AddElement(new Button("dv", "/"));
        dv.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        dv.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        dv.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        ml = AddElement(new Button("ml", "*"));
        ml.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        ml.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        ml.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        mn = AddElement(new Button("mn", "-"));
        mn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        mn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        mn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        ad = AddElement(new Button("ad", "+"));
        ad.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        ad.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        ad.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "display",
                Type = "label",
                CellName = "",
                Relative = new Rect(1, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(18, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 2,
                MarginBottom = 11,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#32446d"),
        Color = new Color("#c8e0ff"),
        TextAlign = "right"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "ac",
                Type = "button",
                CellName = "",
                Relative = new Rect(3, 2, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 17,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "dl",
                Type = "button",
                CellName = "",
                Relative = new Rect(3, 7, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 12,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "pr",
                Type = "button",
                CellName = "",
                Relative = new Rect(3, 12, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 7,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "dv",
                Type = "button",
                CellName = "",
                Relative = new Rect(3, 17, 536870911, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 2,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n7",
                Type = "button",
                CellName = "",
                Relative = new Rect(5, 2, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 17,
                MarginBottom = 7,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n8",
                Type = "button",
                CellName = "",
                Relative = new Rect(5, 7, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 12,
                MarginBottom = 7,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n9",
                Type = "button",
                CellName = "",
                Relative = new Rect(5, 12, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 7,
                MarginBottom = 7,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "ml",
                Type = "button",
                CellName = "",
                Relative = new Rect(5, 17, 536870911, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 2,
                MarginBottom = 7,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n4",
                Type = "button",
                CellName = "",
                Relative = new Rect(7, 2, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 17,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n5",
                Type = "button",
                CellName = "",
                Relative = new Rect(7, 7, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 12,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n6",
                Type = "button",
                CellName = "",
                Relative = new Rect(7, 12, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 7,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "mn",
                Type = "button",
                CellName = "",
                Relative = new Rect(7, 17, 536870911, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 2,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n1",
                Type = "button",
                CellName = "",
                Relative = new Rect(9, 2, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 17,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n2",
                Type = "button",
                CellName = "",
                Relative = new Rect(9, 7, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 12,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n3",
                Type = "button",
                CellName = "",
                Relative = new Rect(9, 12, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 7,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "sb",
                Type = "button",
                CellName = "",
                Relative = new Rect(9, 17, 536870911, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 2,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "n0",
                Type = "button",
                CellName = "",
                Relative = new Rect(11, 2, 8, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(8),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(8, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 12,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "dt",
                Type = "button",
                CellName = "",
                Relative = new Rect(11, 12, 3, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 7,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "ad",
                Type = "button",
                CellName = "",
                Relative = new Rect(11, 17, 536870911, 1),
                SourceCell = new Rect(0, 0, 22, 13),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(13),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(22, 13),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onN0Click() { }
    protected virtual void onN1Click() { }
    protected virtual void onN2Click() { }
    protected virtual void onN3Click() { }
    protected virtual void onN4Click() { }
    protected virtual void onN5Click() { }
    protected virtual void onN6Click() { }
    protected virtual void onN7Click() { }
    protected virtual void onN8Click() { }
    protected virtual void onN9Click() { }
    protected virtual void onSbClick() { }
    protected virtual void onDtClick() { }
    protected virtual void onAcClick() { }
    protected virtual void onDlClick() { }
    protected virtual void onPrClick() { }
    protected virtual void onDvClick() { }
    protected virtual void onMlClick() { }
    protected virtual void onMnClick() { }
    protected virtual void onAdClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "n0")
        {
            onN0Click();
            return true;
        }
        else if (name == "n1")
        {
            onN1Click();
            return true;
        }
        else if (name == "n2")
        {
            onN2Click();
            return true;
        }
        else if (name == "n3")
        {
            onN3Click();
            return true;
        }
        else if (name == "n4")
        {
            onN4Click();
            return true;
        }
        else if (name == "n5")
        {
            onN5Click();
            return true;
        }
        else if (name == "n6")
        {
            onN6Click();
            return true;
        }
        else if (name == "n7")
        {
            onN7Click();
            return true;
        }
        else if (name == "n8")
        {
            onN8Click();
            return true;
        }
        else if (name == "n9")
        {
            onN9Click();
            return true;
        }
        else if (name == "sb")
        {
            onSbClick();
            return true;
        }
        else if (name == "dt")
        {
            onDtClick();
            return true;
        }
        else if (name == "ac")
        {
            onAcClick();
            return true;
        }
        else if (name == "dl")
        {
            onDlClick();
            return true;
        }
        else if (name == "pr")
        {
            onPrClick();
            return true;
        }
        else if (name == "dv")
        {
            onDvClick();
            return true;
        }
        else if (name == "ml")
        {
            onMlClick();
            return true;
        }
        else if (name == "mn")
        {
            onMnClick();
            return true;
        }
        else if (name == "ad")
        {
            onAdClick();
            return true;
        }
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
