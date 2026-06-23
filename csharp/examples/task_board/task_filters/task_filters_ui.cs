// Auto-generated UI code for task_filters - DO NOT EDIT MANUALLY.
using Uimd;

public class TaskFiltersUI : GeneratedWindowBase
{
    public Label filters_label = null!;
    public Label search_label = null!;
    public Label status_label = null!;
    public Label owner_label = null!;
    public TextInput search = null!;
    public ComboBox status_filter = null!;
    public ComboBox owner_filter = null!;
    public Button apply_filters_btn = null!;
    public Button reset_filters_btn = null!;

    public TaskFiltersUI() : base("Task Filters")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "TaskFilters", """
# Task Filters

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
status: draft
description: "Reusable task board filter panel."
tags: [component, example]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
filters_label:
  type: label
  text: FILTERS
  description: "Filter panel label."
  expose: false

search_label:
  type: label
  text: Search
  description: "Search filter label."
  expose: false

status_label:
  type: label
  text: Status
  description: "Status filter label."
  expose: false

owner_label:
  type: label
  text: Assignee
  description: "Assignee filter label."
  expose: false

search:
  type: textinput
  value: ""
  maxlength: 60
  placeholder: "Task title or description"
  description: "Filter tasks by title or description."

status_filter:
  type: combobox
  options: [Any, Todo, Doing, Blocked, Done]
  selected_item: Any
  description: "Filter tasks by status."

owner_filter:
  type: combobox
  options: [Any, Ava, Bruno, Chen, Dana]
  selected_item: Any
  description: "Filter tasks by assignee."

apply_filters_btn:
  type: button
  title: "Apply filter"
  description: "Apply the current task filters."

reset_filters_btn:
  type: button
  title: Reset
  description: "Reset search, status, and assignee filters."
```

## Style

```yaml
include: dark
this:
  background: "#15171c"
  border-width: 0
label:
  color: "#f5f5f5"
textinput:
  background: "#252b35"
  color: "#ffffff"
  focus-background: "#303a49"
  edit-background: "#344459"
  cursor-background: "#93c5fd"
  cursor-color: "#ffffff"
combobox:
  background: "#252b35"
  color: "#ffffff"
  focus-background: "#303a49"
  edit-background: "#344459"
  selected-background: "#2f80ed"
  selected-color: "#ffffff"
button:
  background: "#2f3545"
  color: "#f5f5f5"
  focus-background: "#2f80ed"
@filters_label:
  color: "#aab2c3"
@search_label:
  color: "#cfd5e3"
@status_label:
  color: "#cfd5e3"
@owner_label:
  color: "#cfd5e3"
@apply_filters_btn:
  background: "#2f80ed"
  focus-background: "#1d6fd8"
@reset_filters_btn:
  background: "#343b4b"
```

## User Interface

```ui
+-**---------------------------------------------------------------------------+
|filters_label..**.............................................................|
+----------------------------+----------------------+--------------------------+
|search_label..**........... | status_label..**.... | owner_label..**......... |
|search..**................. | status_filter.**.... | owner_filter..**........ |
+-**-------------------------+----------------------+--------------------------+
|                                                                              |
|apply_filters_btn.....  reset_filters_btn.....                                |
|                                                                              |
+------------------------------------------------------------------------------+
```

""", "Reusable task board filter panel.", new[]
        {
            new GeneratedElementMetadata { Name = "filters_label", Description = "Filter panel label.", Expose = false },
            new GeneratedElementMetadata { Name = "search_label", Description = "Search filter label.", Expose = false },
            new GeneratedElementMetadata { Name = "status_label", Description = "Status filter label.", Expose = false },
            new GeneratedElementMetadata { Name = "owner_label", Description = "Assignee filter label.", Expose = false },
            new GeneratedElementMetadata { Name = "search", Description = "Filter tasks by title or description.", Expose = true },
            new GeneratedElementMetadata { Name = "status_filter", Description = "Filter tasks by status.", Expose = true },
            new GeneratedElementMetadata { Name = "owner_filter", Description = "Filter tasks by assignee.", Expose = true },
            new GeneratedElementMetadata { Name = "apply_filters_btn", Description = "Apply the current task filters.", Expose = true },
            new GeneratedElementMetadata { Name = "reset_filters_btn", Description = "Reset search, status, and assignee filters.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#15171c"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        filters_label = AddElement(new Label("filters_label", "FILTERS"));
        filters_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#aab2c3")
    });
        filters_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        search_label = AddElement(new Label("search_label", "Search"));
        search_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cfd5e3")
    });
        search_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        status_label = AddElement(new Label("status_label", "Status"));
        status_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cfd5e3")
    });
        status_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        owner_label = AddElement(new Label("owner_label", "Assignee"));
        owner_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cfd5e3")
    });
        owner_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        search = AddElement(new TextInput("search", "", 60));
        search.SetStyle(new Style
    {
        Background = new Color("#252b35"),
        Color = new Color("#ffffff")
    });
        search.SetFocusStyle(new Style
    {
        Background = new Color("#303a49"),
        Color = new Color("#ffffff")
    });
        search.SetEditStyle(new Style
    {
        Background = new Color("#344459"),
        Color = new Color("#ffffff")
    });
        search.SetCursorStyle(new Style
    {
        Background = new Color("#93c5fd"),
        Color = new Color("#ffffff")
    });
        search.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        search.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        status_filter = AddElement(new ComboBox("status_filter", new[] {"Any", "Todo", "Doing", "Blocked", "Done"}));
        status_filter.SetSelectedIndex(0);
        status_filter.SetStyle(new Style
    {
        Background = new Color("#252b35"),
        Color = new Color("#ffffff")
    });
        status_filter.SetFocusStyle(new Style
    {
        Background = new Color("#303a49"),
        Color = new Color("#ffffff")
    });
        status_filter.SetEditStyle(new Style
    {
        Background = new Color("#344459"),
        Color = new Color("#e5e7eb")
    });
        status_filter.SetSelectedStyle(new Style
    {
        Background = new Color("#2f80ed"),
        Color = new Color("#ffffff")
    });
        status_filter.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        owner_filter = AddElement(new ComboBox("owner_filter", new[] {"Any", "Ava", "Bruno", "Chen", "Dana"}));
        owner_filter.SetSelectedIndex(0);
        owner_filter.SetStyle(new Style
    {
        Background = new Color("#252b35"),
        Color = new Color("#ffffff")
    });
        owner_filter.SetFocusStyle(new Style
    {
        Background = new Color("#303a49"),
        Color = new Color("#ffffff")
    });
        owner_filter.SetEditStyle(new Style
    {
        Background = new Color("#344459"),
        Color = new Color("#e5e7eb")
    });
        owner_filter.SetSelectedStyle(new Style
    {
        Background = new Color("#2f80ed"),
        Color = new Color("#ffffff")
    });
        owner_filter.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        apply_filters_btn = AddElement(new Button("apply_filters_btn", "Apply filter"));
        apply_filters_btn.SetStyle(new Style
    {
        Background = new Color("#2f80ed"),
        Color = new Color("#f5f5f5")
    });
        apply_filters_btn.SetFocusStyle(new Style
    {
        Background = new Color("#1d6fd8"),
        Color = new Color("#ffffff")
    });
        apply_filters_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        reset_filters_btn = AddElement(new Button("reset_filters_btn", "Reset"));
        reset_filters_btn.SetStyle(new Style
    {
        Background = new Color("#343b4b"),
        Color = new Color("#f5f5f5")
    });
        reset_filters_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2f80ed"),
        Color = new Color("#ffffff")
    });
        reset_filters_btn.SetDisabledStyle(new Style
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
                Name = "filters_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 78, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(78, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#aab2c3")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "search_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(2, 0, 28, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(27, 1),
                CellCharsSize = new Size(28, 2),
                MarginRight = 1,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cfd5e3")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "owner_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(2, 52, 26, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(24, 1),
                CellCharsSize = new Size(26, 2),
                MarginRight = 1,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cfd5e3")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(2, 29, 22, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(20, 1),
                CellCharsSize = new Size(22, 2),
                MarginRight = 1,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cfd5e3")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "apply_filters_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(1, 0, 22, 1),
                SourceCell = new Rect(5, 0, 78, 3),
                Width = AxisDimension.Auto(22),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(22, 1),
                CellCharsSize = new Size(78, 3),
                MarginRight = 56,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#2f80ed"),
        Color = new Color("#f5f5f5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "search",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(1, 0, 536870911, 1),
                SourceCell = new Rect(2, 0, 28, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(27, 1),
                CellCharsSize = new Size(28, 2),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#252b35"),
        Color = new Color("#ffffff")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "owner_filter",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(1, 1, 536870911, 1),
                SourceCell = new Rect(2, 52, 26, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(24, 1),
                CellCharsSize = new Size(26, 2),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#252b35"),
        Color = new Color("#ffffff")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status_filter",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(1, 1, 536870911, 1),
                SourceCell = new Rect(2, 29, 22, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(22),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(20, 1),
                CellCharsSize = new Size(22, 2),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#252b35"),
        Color = new Color("#ffffff")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "reset_filters_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(1, 24, 22, 1),
                SourceCell = new Rect(5, 0, 78, 3),
                Width = AxisDimension.Auto(22),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(22, 1),
                CellCharsSize = new Size(78, 3),
                MarginRight = 32,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#343b4b"),
        Color = new Color("#f5f5f5")
    },
            },
        };
    }

    protected virtual void onSearchChange(string value) { }
    protected virtual void onSearchSubmit(string value) { }
    protected virtual void onStatusFilterChange(string value) { }
    protected virtual void onOwnerFilterChange(string value) { }
    protected virtual void onApplyFiltersBtnClick() { }
    protected virtual void onResetFiltersBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "apply_filters_btn")
        {
            onApplyFiltersBtnClick();
            return true;
        }
        else if (name == "reset_filters_btn")
        {
            onResetFiltersBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "search")
        {
            onSearchChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "search")
        {
            onSearchSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "status_filter")
        {
            onStatusFilterChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        else if (name == "owner_filter")
        {
            onOwnerFilterChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}
