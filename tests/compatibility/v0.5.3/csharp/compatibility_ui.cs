// Auto-generated UI code for compatibility - DO NOT EDIT MANUALLY.
using Uimd;

public class CompatibilityUI : GeneratedWindowBase
{
    public TextInput aligned_input = null!;
    public ComboBox choice = null!;
    public ReusableElement scroller = null!;

    public CompatibilityUI() : base("UIMD 0.5.3 Compatibility Application")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "Compatibility", """
# UIMD 0.5.3 Compatibility Application

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: regression
description: "Retained generated application for patch-level runtime compatibility."
tags: [compatibility]
```

## Definition

```yaml
uses:
  - compat_scroll
```

## Members

```yaml
aligned_input:
  type: textinput
  value: "abc"
  maxlength: 16

choice:
  type: combobox
  options: [one, two, three, four, five, six, seven, eight]
  selected_item: one

scroller:
  type: compat_scroll
```

## Style

```yaml
this:
  border-width: 0
  background: "#303545"
@aligned_input:
  text-align: right
@scroller:
  background: "#303545"
  focus-background: "#ffffff14"
```

## User Interface

```ui
+--------------------------------+
|aligned_input...................|
|choice..........................|
|................................|
|scroller........................|
|................................|
+--------------------------------+
```

""", "Retained generated application for patch-level runtime compatibility.", new[]
        {
            new GeneratedElementMetadata { Name = "aligned_input", Description = "aligned_input", Expose = true },
            new GeneratedElementMetadata { Name = "choice", Description = "choice", Expose = true },
            new GeneratedElementMetadata { Name = "scroller", Description = "scroller", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#303545"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        aligned_input = AddElement(new TextInput("aligned_input", "abc", 16));
        aligned_input.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb"),
        TextAlign = "right"
    });
        aligned_input.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        aligned_input.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        aligned_input.SetCursorStyle(new Style
    {
        Background = new Color("#ffffff"),
        Color = new Color("#0e1117")
    });
        aligned_input.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        aligned_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        choice = AddElement(new ComboBox("choice", new[] {"one", "two", "three", "four", "five", "six", "seven", "eight"}));
        choice.SetSelectedIndex(0);
        choice.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        choice.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        choice.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#e5e7eb")
    });
        choice.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        choice.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        scroller = AddElement(new ReusableElement("scroller"));
        scroller.SetChild(new CompatScrollUI());
        scroller.SetStyle(new Style
    {
        Background = new Color("#303545")
    });
        scroller.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        scroller.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        scroller.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "aligned_input",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 32, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(32),
                CellHeight = AxisDimension.Auto(5),
                CharsSize = new Size(32, 1),
                CellCharsSize = new Size(32, 5),
                MarginRight = 0,
                MarginBottom = 4,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb"),
        TextAlign = "right"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "choice",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(1, 0, 536870911, 2),
                SourceCell = new Rect(0, 0, 32, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(2),
                CellWidth = AxisDimension.Auto(32),
                CellHeight = AxisDimension.Auto(5),
                CharsSize = new Size(32, 2),
                CellCharsSize = new Size(32, 5),
                MarginRight = 0,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "scroller",
                Type = "compat_scroll",
                CellName = "",
                Relative = new Rect(3, 0, 536870911, 2),
                SourceCell = new Rect(0, 0, 32, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(2),
                CellWidth = AxisDimension.Auto(32),
                CellHeight = AxisDimension.Auto(5),
                CharsSize = new Size(32, 2),
                CellCharsSize = new Size(32, 5),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#303545")
    },
            },
        };
    }

    protected virtual void onAlignedInputChange(string value) { }
    protected virtual void onAlignedInputSubmit(string value) { }
    protected virtual void onChoiceChange(string value) { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "aligned_input")
        {
            onAlignedInputChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "aligned_input")
        {
            onAlignedInputSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "choice")
        {
            onChoiceChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}
