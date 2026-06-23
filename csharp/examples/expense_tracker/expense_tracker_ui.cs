// Auto-generated UI code for expense_tracker - DO NOT EDIT MANUALLY.
using Uimd;

public class ExpenseTrackerUI : GeneratedWindowBase
{
    public Label app_title = null!;
    public Button quit_btn = null!;
    public Label nav_title = null!;
    public Button expenses_btn = null!;
    public Button categories_btn = null!;
    public Button settings_btn = null!;
    public ViewHost main = null!;

    public ExpenseTrackerUI() : base("Expense Tracker")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "ExpenseTracker", """
# Expense Tracker

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: experimental
description: "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost."
tags: [example, viewhost, expenses]
```

## Definition

```yaml
uses:
  - expenses_view
  - categories_view
  - settings_view
```

## Members

```yaml
app_title:
  type: label
  text: "Expense Tracker"

quit_btn:
  type: button
  title: Quit

nav_title:
  type: label
  text: Expenses

expenses_btn:
  type: button
  title: Expenses

categories_btn:
  type: button
  title: Categories

settings_btn:
  type: button
  title: Settings

main:
  type: viewhost
```

## Style

```yaml
include: dark
this:
  border-width: 0
cell:
  padding: 1
@header:
  background: "#1f2937"
  padding: 0
@app_title:
  color: "#ffffff"
  text-align: center
@nav:
  background: "#172033"
@content:
  background: "#030712"
@nav_title:
  color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@quit_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
viewhost:
  background: "#030712"
```

## User Interface

```ui
+--header-**------------------------------------------+
| app_title..**.......................... quit_btn... |
+--nav------------------+--content-**-----------------+
| nav_title............ | main..**................... |
|                       | *.......................... |
| expenses_btn......... | *.......................... |
|                       | ........................... |
| categories_btn....... | ........................... |
|                       | ........................... |
| settings_btn......... | ........................... |
*                       * ........................... |
*                       * ........................... |
+-----------------------+-----------------------------+
```

""", "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.", new[]
        {
            new GeneratedElementMetadata { Name = "app_title", Description = "Expense Tracker", Expose = true },
            new GeneratedElementMetadata { Name = "quit_btn", Description = "Quit", Expose = true },
            new GeneratedElementMetadata { Name = "nav_title", Description = "Expenses", Expose = true },
            new GeneratedElementMetadata { Name = "expenses_btn", Description = "Expenses", Expose = true },
            new GeneratedElementMetadata { Name = "categories_btn", Description = "Categories", Expose = true },
            new GeneratedElementMetadata { Name = "settings_btn", Description = "Settings", Expose = true },
            new GeneratedElementMetadata { Name = "main", Description = "main", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        app_title = AddElement(new Label("app_title", "Expense Tracker"));
        app_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        app_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        quit_btn = AddElement(new Button("quit_btn", "Quit"));
        quit_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    });
        quit_btn.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        quit_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        nav_title = AddElement(new Label("nav_title", "Expenses"));
        nav_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff")
    });
        nav_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        expenses_btn = AddElement(new Button("expenses_btn", "Expenses"));
        expenses_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        expenses_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        expenses_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        categories_btn = AddElement(new Button("categories_btn", "Categories"));
        categories_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        categories_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        categories_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        settings_btn = AddElement(new Button("settings_btn", "Settings"));
        settings_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        settings_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        settings_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        main = AddElement(new ViewHost("main"));
        main.SetStyle(new Style
    {
        Background = new Color("#030712")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "app_title",
                Type = "label",
                CellName = "header",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 53, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(39, 1),
                CellCharsSize = new Size(53, 1),
                MarginRight = 13,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "main",
                Type = "viewhost",
                CellName = "content",
                Relative = new Rect(0, 1, 536870911, 536870911),
                SourceCell = new Rect(2, 24, 29, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(27, 9),
                CellCharsSize = new Size(29, 9),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#030712"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#030712")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "nav_title",
                Type = "label",
                CellName = "nav",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 23, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(23),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(21, 1),
                CellCharsSize = new Size(23, 9),
                MarginRight = 1,
                MarginBottom = 8,
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
                Name = "quit_btn",
                Type = "button",
                CellName = "header",
                Relative = new Rect(0, 41, 11, 1),
                SourceCell = new Rect(0, 0, 53, 1),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(53, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "expenses_btn",
                Type = "button",
                CellName = "nav",
                Relative = new Rect(2, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 23, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(23),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(21, 1),
                CellCharsSize = new Size(23, 9),
                MarginRight = 1,
                MarginBottom = 6,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "categories_btn",
                Type = "button",
                CellName = "nav",
                Relative = new Rect(4, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 23, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(23),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(21, 1),
                CellCharsSize = new Size(23, 9),
                MarginRight = 1,
                MarginBottom = 4,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "settings_btn",
                Type = "button",
                CellName = "nav",
                Relative = new Rect(6, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 23, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(23),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(21, 1),
                CellCharsSize = new Size(23, 9),
                MarginRight = 1,
                MarginBottom = 2,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onQuitBtnClick() { }
    protected virtual void onExpensesBtnClick() { }
    protected virtual void onCategoriesBtnClick() { }
    protected virtual void onSettingsBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "quit_btn")
        {
            onQuitBtnClick();
            return true;
        }
        else if (name == "expenses_btn")
        {
            onExpensesBtnClick();
            return true;
        }
        else if (name == "categories_btn")
        {
            onCategoriesBtnClick();
            return true;
        }
        else if (name == "settings_btn")
        {
            onSettingsBtnClick();
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
