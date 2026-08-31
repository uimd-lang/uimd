// Auto-generated UI code for categories_view - DO NOT EDIT MANUALLY.
using Uimd;

public class CategoriesViewUI : GeneratedWindowBase
{
    public Label view_title = null!;
    public TextInput category_name = null!;
    public Button add_category_btn = null!;
    public ListBox categories = null!;
    public Button remove_category_btn = null!;

    public CategoriesViewUI() : base("Categories View")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "CategoriesView", """
# Categories View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
status: experimental
description: "Simple category list management view."
tags: [example, viewhost]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
view_title:
  type: label
  text: Categories

category_name:
  type: textinput
  value: ""
  maxlength: 40

add_category_btn:
  type: button
  title: Add

categories:
  type: listbox
  options: [Food, Transport, Home, Health, Other]
  selected_items: [Food]

remove_category_btn:
  type: button
  title: Remove selected
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@view_title:
  color: "#bbf7d0"
textinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
listbox:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
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
| "Name:" category_name..**................................ |
|                                                            |
| add_category_btn................ remove_category_btn..... |
|                                                            |
| categories..**............................................ |
| *......................................................... |
| *......................................................... |
| .......................................................... |
| .......................................................... |
+------------------------------------------------------------+
```

""", "Simple category list management view.", new[]
        {
            new GeneratedElementMetadata { Name = "view_title", Description = "Categories", Expose = true },
            new GeneratedElementMetadata { Name = "category_name", Description = "category_name", Expose = true },
            new GeneratedElementMetadata { Name = "add_category_btn", Description = "Add", Expose = true },
            new GeneratedElementMetadata { Name = "categories", Description = "categories", Expose = true },
            new GeneratedElementMetadata { Name = "remove_category_btn", Description = "Remove selected", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        view_title = AddElement(new Label("view_title", "Categories"));
        view_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bbf7d0")
    });
        view_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        category_name = AddElement(new TextInput("category_name", "", 40));
        category_name.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        category_name.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        category_name.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#ffffff")
    });
        category_name.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        category_name.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        category_name.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        add_category_btn = AddElement(new Button("add_category_btn", "Add"));
        add_category_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        add_category_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        add_category_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        categories = AddElement(new ListBox("categories", new[] {"Food", "Transport", "Home", "Health", "Other"}));
        categories.SetSelectedIndex(0);
        categories.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        categories.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        categories.SetEditStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#cbd5e1")
    });
        categories.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        categories.SetActiveStyle(new Style
    {
        Background = new Color("#DDDDDD99"),
        Color = new Color("#FFFFFF")
    });
        categories.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        remove_category_btn = AddElement(new Button("remove_category_btn", "Remove selected"));
        remove_category_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        remove_category_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        remove_category_btn.SetDisabledStyle(new Style
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
                SourceCell = new Rect(0, 0, 60, 11),
                Width = AxisDimension.Auto(58),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(58, 1),
                CellCharsSize = new Size(60, 11),
                MarginRight = 1,
                MarginBottom = 10,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bbf7d0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Name:\"",
                Type = "label",
                CellName = "view",
                Relative = new Rect(2, 1, 7, 1),
                SourceCell = new Rect(0, 0, 60, 11),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(7, 1),
                CellCharsSize = new Size(60, 11),
                MarginRight = 52,
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
                Name = "category_name",
                Type = "textinput",
                CellName = "view",
                Relative = new Rect(2, 9, 536870911, 1),
                SourceCell = new Rect(0, 0, 60, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(49, 1),
                CellCharsSize = new Size(60, 11),
                MarginRight = 2,
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
                Name = "add_category_btn",
                Type = "button",
                CellName = "view",
                Relative = new Rect(4, 1, 32, 1),
                SourceCell = new Rect(0, 0, 60, 11),
                Width = AxisDimension.Auto(32),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(32, 1),
                CellCharsSize = new Size(60, 11),
                MarginRight = 27,
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
                Name = "remove_category_btn",
                Type = "button",
                CellName = "view",
                Relative = new Rect(4, 34, 24, 1),
                SourceCell = new Rect(0, 0, 60, 11),
                Width = AxisDimension.Auto(24),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(24, 1),
                CellCharsSize = new Size(60, 11),
                MarginRight = 2,
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
                Name = "categories",
                Type = "listbox",
                CellName = "view",
                Relative = new Rect(6, 1, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 60, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(58, 5),
                CellCharsSize = new Size(60, 11),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
        };
    }

    protected virtual void onCategoryNameChange(string value) { }
    protected virtual void onCategoryNameSubmit(string value) { }
    protected virtual void onAddCategoryBtnClick() { }
    protected virtual void onCategoriesSelectionChange(List<string> value) { }
    protected virtual bool onCategoriesItemActivate(int index, string value) { return false; }
    protected virtual void onRemoveCategoryBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "add_category_btn")
        {
            onAddCategoryBtnClick();
            return true;
        }
        else if (name == "remove_category_btn")
        {
            onRemoveCategoryBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "category_name")
        {
            onCategoryNameChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "category_name")
        {
            onCategoryNameSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "categories")
        {
            onCategoriesSelectionChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedListBoxItemActivate(string name, string elementId, int index, string value)
    {
        if (name == "categories")
        {
            return onCategoriesItemActivate(index, value);
        }
        return false;
    }
}
