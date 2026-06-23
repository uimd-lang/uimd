// Auto-generated UI code for expenses_list - DO NOT EDIT MANUALLY.
using Uimd;

public class ExpensesListUI : GeneratedScrollViewBase
{

    public ExpensesListUI() : base("Expenses List")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ExpensesList", """
# Expenses List

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: experimental
description: "Scrollable list of interactive expense rows."
tags: [component, example]
```

## Definition

```yaml
extends: uiscrollview
uses:
  - ../expense_row/expense_row
```

## Style

```yaml
this:
  border-width: 0
  background: "#172033"
@panel:
  gap: 1
  padding: 1
  background: "#172033"
```

## User Interface

```ui
+-panel-**---------------------------------------------+
|                                                      |
|                                                      |
|                                                      |
|                                                      |
*                                                      |
*                                                      |
|                                                      |
+------------------------------------------------------+
```

""", "Scrollable list of interactive expense rows.", Array.Empty<GeneratedElementMetadata>());
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#172033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        SetGeneratedScrollView(AddElement(new ScrollView("__scrollview", 1)));
        ScrollView().SetStyle(new Style
    {
        Background = new Color("#172033"),
        Gap = 1,
        Padding = 1
    });
        ScrollView().SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        ScrollView().SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        ScrollView().SetSelectedStyle(new Style
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
                Name = "__scrollview",
                Type = "uiscrollview",
                CellName = "panel",
                Relative = new Rect(0, 0, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 54, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(54, 7),
                CellCharsSize = new Size(54, 7),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Gap = 1
    },
                ElementStyle = new Style(),
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
