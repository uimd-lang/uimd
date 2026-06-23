// Auto-generated UI code for expense_row - DO NOT EDIT MANUALLY.
using Uimd;

public class ExpenseRowUI : GeneratedWindowBase
{
    public CheckBox paid = null!;
    public Label item = null!;
    public Label amount = null!;
    public ComboBox category = null!;
    public Button delete_btn = null!;

    public ExpenseRowUI() : base("Expense Row")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ExpenseRow", """
# Expense Row

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: experimental
description: "Reusable expense row with paid state, category selector, and delete action."
tags: [component, example]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
paid:
  type: checkbox
  title: Paid
  value: false

item:
  type: label
  text: Item
  user-select: text

amount:
  type: label
  text: "0.00"

category:
  type: combobox
  options: [Food, Transport, Home, Health, Other]
  selected_item: Food

delete_btn:
  type: button
  title: Delete
```

## Style

```yaml
this:
  border-width: 0
@row:
  background: "#172033"
  padding: 1
@item:
  color: "#ffffff"
@amount:
  color: "#bfdbfe"
checkbox:
  color: "#cbd5e1"
  checked-color: "#22c55e"
  unchecked-color: "#94a3b8"
combobox:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@delete_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
```

## User Interface

```ui
+-row-**-----------------------------------------------+
| paid..... item..**..................... amount...... |
| category..**........................... delete_btn.. |
+------------------------------------------------------+
```

""", "Reusable expense row with paid state, category selector, and delete action.", new[]
        {
            new GeneratedElementMetadata { Name = "paid", Description = "Paid", Expose = true },
            new GeneratedElementMetadata { Name = "item", Description = "Item", Expose = true },
            new GeneratedElementMetadata { Name = "amount", Description = "0.00", Expose = true },
            new GeneratedElementMetadata { Name = "category", Description = "category", Expose = true },
            new GeneratedElementMetadata { Name = "delete_btn", Description = "Delete", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        paid = AddElement(new CheckBox("paid", "Paid", false));
        paid.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        paid.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#ffffff")
    });
        paid.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#22c55e")
    });
        paid.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#94a3b8")
    });
        paid.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        item = AddElement(new Label("item", "Item"));
        item.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff")
    });
        item.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        amount = AddElement(new Label("amount", "0.00"));
        amount.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe")
    });
        amount.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        category = AddElement(new ComboBox("category", new[] {"Food", "Transport", "Home", "Health", "Other"}));
        category.SetSelectedIndex(0);
        category.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        category.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        category.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        category.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        category.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        delete_btn = AddElement(new Button("delete_btn", "Delete"));
        delete_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    });
        delete_btn.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        delete_btn.SetDisabledStyle(new Style
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
                Name = "paid",
                Type = "checkbox",
                CellName = "row",
                Relative = new Rect(0, 1, 9, 1),
                SourceCell = new Rect(0, 0, 54, 2),
                Width = AxisDimension.Auto(9),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(54, 2),
                MarginRight = 44,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "item",
                Type = "label",
                CellName = "row",
                Relative = new Rect(0, 11, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 54, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(29, 2),
                CellCharsSize = new Size(54, 2),
                MarginRight = 14,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "amount",
                Type = "label",
                CellName = "row",
                Relative = new Rect(0, 41, 12, 1),
                SourceCell = new Rect(0, 0, 54, 2),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(54, 2),
                MarginRight = 1,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "category",
                Type = "combobox",
                CellName = "row",
                Relative = new Rect(1, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 54, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(39, 1),
                CellCharsSize = new Size(54, 2),
                MarginRight = 14,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "delete_btn",
                Type = "button",
                CellName = "row",
                Relative = new Rect(1, 41, 12, 1),
                SourceCell = new Rect(0, 0, 54, 2),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(54, 2),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onPaidChange(string value) { }
    protected virtual void onCategoryChange(string value) { }
    protected virtual void onDeleteBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "delete_btn")
        {
            onDeleteBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "paid")
        {
            onPaidChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "category")
        {
            onCategoryChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}
