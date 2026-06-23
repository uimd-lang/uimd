// Auto-generated UI code for expenses_view - DO NOT EDIT MANUALLY.
using Uimd;

public class ExpensesViewUI : GeneratedWindowBase
{
    public Label view_title = null!;
    public TextInput item_input = null!;
    public NumberInput amount_input = null!;
    public ComboBox category_input = null!;
    public Label currency_label = null!;
    public Button add_expense_btn = null!;
    public ReusableElement expenses = null!;

    public ExpensesViewUI() : base("Expenses View")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "ExpensesView", """
# Expenses View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
status: experimental
description: "Expense entry view with a dynamic scrollable list."
tags: [example, viewhost]
```

## Definition

```yaml
extends: uicontrol
uses:
  - expenses_list/expenses_list
```

## Members

```yaml
view_title:
  type: label
  text: Expenses

item_input:
  type: textinput
  value: ""
  maxlength: 80

amount_input:
  type: numberinput
  value: 0
  format: "0.00"

category_input:
  type: combobox
  options: [Food, Transport, Home, Health, Other]
  selected_item: Food

currency_label:
  type: label
  text: EUR

add_expense_btn:
  type: button
  title: Add expense

expenses:
  type: expenses_list
  cpp-class: expense_tracker_example::ExpensesList
  cpp-header: "expenses_list/expenses_list.hpp"
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@view_title:
  color: "#bfdbfe"
@currency_label:
  color: "#bfdbfe"
@expenses:
  background: "#172033"
textinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
numberinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
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
```

## User Interface

```ui
+-view-**----------------------------------------------------+
| view_title................................................ |
|                                                            |
| "Item:"     item_input.................................... |
|                                                            |
| "Amount:"   amount_input............. currency_label...... |
|                                                            |
| "Category:" category_input................................ |
|                                                            |
| add_expense_btn........................................... |
|                                                            |
| expenses..**.............................................. |
| *......................................................... |
| *......................................................... |
| .......................................................... |
| .......................................................... |
+------------------------------------------------------------+
```

""", "Expense entry view with a dynamic scrollable list.", new[]
        {
            new GeneratedElementMetadata { Name = "view_title", Description = "Expenses", Expose = true },
            new GeneratedElementMetadata { Name = "item_input", Description = "item_input", Expose = true },
            new GeneratedElementMetadata { Name = "amount_input", Description = "amount_input", Expose = true },
            new GeneratedElementMetadata { Name = "category_input", Description = "category_input", Expose = true },
            new GeneratedElementMetadata { Name = "currency_label", Description = "EUR", Expose = true },
            new GeneratedElementMetadata { Name = "add_expense_btn", Description = "Add expense", Expose = true },
            new GeneratedElementMetadata { Name = "expenses", Description = "expenses", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        view_title = AddElement(new Label("view_title", "Expenses"));
        view_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe")
    });
        view_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        item_input = AddElement(new TextInput("item_input", "", 80));
        item_input.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        item_input.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        item_input.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#ffffff")
    });
        item_input.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        item_input.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        item_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        amount_input = AddElement(new NumberInput("amount_input", 0.0, 1.0));
        amount_input.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        amount_input.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        amount_input.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#ffffff")
    });
        amount_input.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        amount_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        category_input = AddElement(new ComboBox("category_input", new[] {"Food", "Transport", "Home", "Health", "Other"}));
        category_input.SetSelectedIndex(0);
        category_input.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        category_input.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        category_input.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        category_input.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        category_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        currency_label = AddElement(new Label("currency_label", "EUR"));
        currency_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe")
    });
        currency_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        add_expense_btn = AddElement(new Button("add_expense_btn", "Add expense"));
        add_expense_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        add_expense_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        add_expense_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        expenses = AddElement(new ReusableElement("expenses"));
        expenses.SetChild(new ExpensesListUI());
        expenses.SetStyle(new Style
    {
        Background = new Color("#172033")
    });
        expenses.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        expenses.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        expenses.SetSelectedStyle(new Style
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
                Name = "view_title",
                Type = "label",
                CellName = "view",
                Relative = new Rect(0, 1, 58, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(58),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(58, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 1,
                MarginBottom = 14,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Item:\"",
                Type = "label",
                CellName = "view",
                Relative = new Rect(2, 1, 7, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(7, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 52,
                MarginBottom = 12,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "item_input",
                Type = "textinput",
                CellName = "view",
                Relative = new Rect(2, 13, 46, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(46),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(46, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 1,
                MarginBottom = 12,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Amount:\"",
                Type = "label",
                CellName = "view",
                Relative = new Rect(4, 1, 9, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(9),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 50,
                MarginBottom = 10,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "amount_input",
                Type = "numberinput",
                CellName = "view",
                Relative = new Rect(4, 13, 25, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(25),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(25, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 22,
                MarginBottom = 10,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "currency_label",
                Type = "label",
                CellName = "view",
                Relative = new Rect(4, 39, 20, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(20, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 1,
                MarginBottom = 10,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Category:\"",
                Type = "label",
                CellName = "view",
                Relative = new Rect(6, 1, 11, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 48,
                MarginBottom = 8,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "category_input",
                Type = "combobox",
                CellName = "view",
                Relative = new Rect(6, 13, 46, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(46),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(46, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 1,
                MarginBottom = 8,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "add_expense_btn",
                Type = "button",
                CellName = "view",
                Relative = new Rect(8, 1, 58, 1),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Auto(58),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(58, 1),
                CellCharsSize = new Size(60, 15),
                MarginRight = 1,
                MarginBottom = 6,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "expenses",
                Type = "expenses_list",
                CellName = "view",
                Relative = new Rect(10, 1, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 60, 15),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(15),
                CharsSize = new Size(58, 5),
                CellCharsSize = new Size(60, 15),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#172033")
    },
            },
        };
    }

    protected virtual void onItemInputChange(string value) { }
    protected virtual void onItemInputSubmit(string value) { }
    protected virtual void onAmountInputChange(string value) { }
    protected virtual void onAmountInputSubmit(string value) { }
    protected virtual void onCategoryInputChange(string value) { }
    protected virtual void onAddExpenseBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "add_expense_btn")
        {
            onAddExpenseBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "item_input")
        {
            onItemInputChange(value);
            return true;
        }
        else if (name == "amount_input")
        {
            onAmountInputChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "item_input")
        {
            onItemInputSubmit(value);
            return true;
        }
        else if (name == "amount_input")
        {
            onAmountInputSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "category_input")
        {
            onCategoryInputChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}
