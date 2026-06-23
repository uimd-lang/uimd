// Auto-generated UI code for task_row - DO NOT EDIT MANUALLY.
using Uimd;

public class TaskRowUI : GeneratedWindowBase
{
    public Label task_title = null!;
    public Label meta = null!;
    public CheckBox done = null!;
    public Label spacer = null!;
    public Button open_btn = null!;
    public Button delete_btn = null!;

    public TaskRowUI() : base("Task Row")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "TaskRow", """
# Task Row

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Reusable task row with editable controls."
tags: [component, example]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
task_title:
  type: label
  text: "Task"
  description: "Task title."

meta:
  type: label
  text: "Status / assignee / priority"
  description: "Task status, assignee, and priority."

done:
  type: checkbox
  title: Done
  value: false
  description: "Whether the task is done."

spacer:
  type: label
  text: ""
  description: "Flexible row spacer before task actions."
  expose: false

open_btn:
  type: button
  title: Open
  description: "Open this task in the edit dialog."

delete_btn:
  type: button
  title: Delete
  description: "Ask for confirmation and delete this task."
```

## Style

```yaml
this:
  border-width: 0
@card:
  background: "#172033"
  padding: 1, 1, 1, 1
@task_title:
  color: "#f8fafc"
  user-select: text
@meta:
  color: "#93c5fd"
  user-select: text
checkbox:
  color: "#cbd5e1"
  focus-color: "#facc15"
  checked-color: "#22c55e"
  unchecked-color: "#94a3b8"
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
+-card-**---------------------------------------+
| task_title..**....... meta................... |
| done.... spacer..**.. open_btn.. delete_btn.. |
+-----------------------------------------------+
```

""", "Reusable task row with editable controls.", new[]
        {
            new GeneratedElementMetadata { Name = "task_title", Description = "Task title.", Expose = true },
            new GeneratedElementMetadata { Name = "meta", Description = "Task status, assignee, and priority.", Expose = true },
            new GeneratedElementMetadata { Name = "done", Description = "Whether the task is done.", Expose = true },
            new GeneratedElementMetadata { Name = "spacer", Description = "Flexible row spacer before task actions.", Expose = false },
            new GeneratedElementMetadata { Name = "open_btn", Description = "Open this task in the edit dialog.", Expose = true },
            new GeneratedElementMetadata { Name = "delete_btn", Description = "Ask for confirmation and delete this task.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        task_title = AddElement(new Label("task_title", "Task"));
        task_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f8fafc"),
        UserSelect = "text"
    });
        task_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        meta = AddElement(new Label("meta", "Status / assignee / priority"));
        meta.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd"),
        UserSelect = "text"
    });
        meta.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        done = AddElement(new CheckBox("done", "Done", false));
        done.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        done.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#facc15")
    });
        done.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#22c55e")
    });
        done.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#94a3b8")
    });
        done.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        spacer = AddElement(new Label("spacer", ""));
        spacer.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        spacer.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        open_btn = AddElement(new Button("open_btn", "Open"));
        open_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        open_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        open_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
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
                Name = "task_title",
                Type = "label",
                CellName = "card",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 47, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(21, 1),
                CellCharsSize = new Size(47, 2),
                MarginRight = 25,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f8fafc"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "meta",
                Type = "label",
                CellName = "card",
                Relative = new Rect(0, 23, 23, 1),
                SourceCell = new Rect(0, 0, 47, 2),
                Width = AxisDimension.Auto(23),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(23, 1),
                CellCharsSize = new Size(47, 2),
                MarginRight = 1,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "done",
                Type = "checkbox",
                CellName = "card",
                Relative = new Rect(1, 1, 8, 1),
                SourceCell = new Rect(0, 0, 47, 2),
                Width = AxisDimension.Auto(8),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(8, 1),
                CellCharsSize = new Size(47, 2),
                MarginRight = 38,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "spacer",
                Type = "label",
                CellName = "card",
                Relative = new Rect(1, 10, 536870911, 1),
                SourceCell = new Rect(0, 0, 47, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(47, 2),
                MarginRight = 25,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "open_btn",
                Type = "button",
                CellName = "card",
                Relative = new Rect(1, 23, 10, 1),
                SourceCell = new Rect(0, 0, 47, 2),
                Width = AxisDimension.Auto(10),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(47, 2),
                MarginRight = 14,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "delete_btn",
                Type = "button",
                CellName = "card",
                Relative = new Rect(1, 34, 12, 1),
                SourceCell = new Rect(0, 0, 47, 2),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(47, 2),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onDoneChange(string value) { }
    protected virtual void onOpenBtnClick() { }
    protected virtual void onDeleteBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "open_btn")
        {
            onOpenBtnClick();
            return true;
        }
        else if (name == "delete_btn")
        {
            onDeleteBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "done")
        {
            onDoneChange(value);
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
        return false;
    }
}
