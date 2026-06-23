// Auto-generated UI code for settings_view - DO NOT EDIT MANUALLY.
using Uimd;

public class SettingsViewUI : GeneratedWindowBase
{
    public Label view_title = null!;
    public ComboBox currency = null!;
    public CheckBox show_paid = null!;
    public NumberInput monthly_limit = null!;
    public Button reset_btn = null!;

    public SettingsViewUI() : base("Settings View")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "SettingsView", """
# Settings View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
status: experimental
description: "Expense tracker settings view."
tags: [example, viewhost, expenses]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
view_title:
  type: label
  text: Settings

currency:
  type: combobox
  options: [EUR, USD, CZK]
  selected_item: EUR

show_paid:
  type: checkbox
  title: Show paid expenses
  value: true

monthly_limit:
  type: numberinput
  value: 500
  format: "0"

reset_btn:
  type: button
  title: Reset demo data
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@view_title:
  color: "#fde68a"
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
| "Currency:"      currency................................. |
|                                                            |
| show_paid................................................. |
|                                                            |
| "Monthly limit:" monthly_limit............................ |
|                                                            |
| reset_btn................................................. |
+------------------------------------------------------------+
```

""", "Expense tracker settings view.", new[]
        {
            new GeneratedElementMetadata { Name = "view_title", Description = "Settings", Expose = true },
            new GeneratedElementMetadata { Name = "currency", Description = "currency", Expose = true },
            new GeneratedElementMetadata { Name = "show_paid", Description = "Show paid expenses", Expose = true },
            new GeneratedElementMetadata { Name = "monthly_limit", Description = "monthly_limit", Expose = true },
            new GeneratedElementMetadata { Name = "reset_btn", Description = "Reset demo data", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        view_title = AddElement(new Label("view_title", "Settings"));
        view_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#fde68a")
    });
        view_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        currency = AddElement(new ComboBox("currency", new[] {"EUR", "USD", "CZK"}));
        currency.SetSelectedIndex(0);
        currency.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        currency.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        currency.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        currency.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        currency.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        show_paid = AddElement(new CheckBox("show_paid", "Show paid expenses", true));
        show_paid.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#b8c4d8")
    });
        show_paid.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#ffffff")
    });
        show_paid.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#44cc88")
    });
        show_paid.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent")
    });
        show_paid.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        monthly_limit = AddElement(new NumberInput("monthly_limit", 500.0, 1.0));
        monthly_limit.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        monthly_limit.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        monthly_limit.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#ffffff")
    });
        monthly_limit.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        monthly_limit.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        reset_btn = AddElement(new Button("reset_btn", "Reset demo data"));
        reset_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        reset_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        reset_btn.SetDisabledStyle(new Style
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
                Name = "view_title",
                Type = "label",
                CellName = "view",
                Relative = new Rect(0, 1, 58, 1),
                SourceCell = new Rect(0, 0, 60, 9),
                Width = AxisDimension.Auto(58),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(58, 1),
                CellCharsSize = new Size(60, 9),
                MarginRight = 1,
                MarginBottom = 8,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#fde68a")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Currency:\"",
                Type = "label",
                CellName = "view",
                Relative = new Rect(2, 1, 11, 1),
                SourceCell = new Rect(0, 0, 60, 9),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(60, 9),
                MarginRight = 48,
                MarginBottom = 6,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "currency",
                Type = "combobox",
                CellName = "view",
                Relative = new Rect(2, 18, 41, 1),
                SourceCell = new Rect(0, 0, 60, 9),
                Width = AxisDimension.Auto(41),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(41, 1),
                CellCharsSize = new Size(60, 9),
                MarginRight = 1,
                MarginBottom = 6,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "show_paid",
                Type = "checkbox",
                CellName = "view",
                Relative = new Rect(4, 1, 58, 1),
                SourceCell = new Rect(0, 0, 60, 9),
                Width = AxisDimension.Auto(58),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(58, 1),
                CellCharsSize = new Size(60, 9),
                MarginRight = 1,
                MarginBottom = 4,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#b8c4d8")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Monthly limit:\"",
                Type = "label",
                CellName = "view",
                Relative = new Rect(6, 1, 16, 1),
                SourceCell = new Rect(0, 0, 60, 9),
                Width = AxisDimension.Auto(16),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(16, 1),
                CellCharsSize = new Size(60, 9),
                MarginRight = 43,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "monthly_limit",
                Type = "numberinput",
                CellName = "view",
                Relative = new Rect(6, 18, 41, 1),
                SourceCell = new Rect(0, 0, 60, 9),
                Width = AxisDimension.Auto(41),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(41, 1),
                CellCharsSize = new Size(60, 9),
                MarginRight = 1,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "reset_btn",
                Type = "button",
                CellName = "view",
                Relative = new Rect(8, 1, 58, 1),
                SourceCell = new Rect(0, 0, 60, 9),
                Width = AxisDimension.Auto(58),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(58, 1),
                CellCharsSize = new Size(60, 9),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onCurrencyChange(string value) { }
    protected virtual void onShowPaidChange(string value) { }
    protected virtual void onMonthlyLimitChange(string value) { }
    protected virtual void onMonthlyLimitSubmit(string value) { }
    protected virtual void onResetBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "reset_btn")
        {
            onResetBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "show_paid")
        {
            onShowPaidChange(value);
            return true;
        }
        else if (name == "monthly_limit")
        {
            onMonthlyLimitChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "monthly_limit")
        {
            onMonthlyLimitSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "currency")
        {
            onCurrencyChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}
