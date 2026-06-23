// Auto-generated UI code for task_list - DO NOT EDIT MANUALLY.
using Uimd;

public class TaskListUI : GeneratedScrollViewBase
{

    public TaskListUI() : base("Task List")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "TaskList", """
# Task List

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Reusable scroll view for interactive task rows."
tags: [component, example]
```

## Definition

```yaml
extends: uiscrollview
uses:
  - ../task_row/task_row
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@panel:
  gap: 1
  padding: 1, 1, 1, 1
  background: "#00000000"
```

## User Interface

```ui
+-panel-**--------------------------------------------------+
|                                                           |
|                                                           |
|                                                           |
|                                                           |
|                                                           |
*                                                           |
*                                                           |
*                                                           |
*                                                           |
|                                                           |
+-----------------------------------------------------------+
```

""", "Reusable scroll view for interactive task rows.", Array.Empty<GeneratedElementMetadata>());
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        SetGeneratedScrollView(AddElement(new ScrollView("__scrollview", 1)));
        ScrollView().SetStyle(new Style
    {
        Background = new Color("#00000000"),
        Gap = 1,
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
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
                SourceCell = new Rect(0, 0, 59, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(59, 10),
                CellCharsSize = new Size(59, 10),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#00000000"),
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
