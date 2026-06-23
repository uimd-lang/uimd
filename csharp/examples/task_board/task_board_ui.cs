// Auto-generated UI code for task_board - DO NOT EDIT MANUALLY.
using Uimd;

public class TaskBoardUI : GeneratedWindowBase
{
    public Label title = null!;
    public Button quit_btn = null!;
    public ReusableElement filters = null!;
    public Button new_task_btn = null!;
    public Button mark_all_done_btn = null!;
    public Button clear_board_btn = null!;
    public ReusableElement board = null!;
    public Label status = null!;

    public TaskBoardUI() : base("Task Board")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "TaskBoard", """
# Task Board

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools."
tags: [example, mcp]
```

## Definition

```yaml
uses:
  - task_filters/task_filters
  - task_list/task_list
  - task_dialog/task_dialog
```

## Members

```yaml
title:
  type: label
  text: "Task Board"
  description: "Application title."
  expose: false

quit_btn:
  type: button
  title: Quit
  description: "Close the task list example."

filters:
  type: task_filters
  cpp-class: task_board_example::TaskFilters
  cpp-header: "task_filters/task_filters.hpp"
  description: "Reusable task filter panel."

new_task_btn:
  type: button
  title: New task
  description: "Open a dialog for creating a task."

mark_all_done_btn:
  type: button
  title: Mark all done
  description: "Ask before marking every task as done."

clear_board_btn:
  type: button
  title: Clear board
  description: "Ask before deleting every task."

board:
  type: task_list
  cpp-class: task_board_example::TaskList
  cpp-header: "task_list/task_list.hpp"
  description: "Scrollable list of interactive task rows."

status:
  type: label
  text: Ready
  description: "Current board status."
```

## Tools

```yaml
query_tasks:
  description: "Return tasks matching optional title, status, assignee, priority, and done filters."
  input:
    title: string
    status: string
    assignee: string
    priority: string
    done: bool
  output:
    tasks:
      - id: string
        title: string
        status: string
        assignee: string
        priority: string
        description: string
        done: bool

get_task:
  description: "Return one task by id."
  input:
    id: string
    required: [id]
  output:
    task:
      id: string
      title: string
      status: string
      assignee: string
      priority: string
      description: string
      done: bool

update_task:
  description: "Update a task by id and return the updated task."
  input:
    id: string
    title: string
    status: string
    assignee: string
    priority: string
    description: string
    done: bool
    required: [id]
  output:
    task:
      id: string
      title: string
      status: string
      assignee: string
      priority: string
      description: string
      done: bool

delete_task:
  description: "Delete a task by id and return whether it was removed."
  input:
    id: string
    required: [id]
  output:
    deleted: bool
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  background: "#1f2937"
@title:
  color: "#ffffff"
  text-align: center
@filter_cell:
  background: "#15171c"
@actions:
  background: "#172033"
  padding: 1, 1, 1, 1
@board_cell:
  background: "#030712"
@filters:
  background: "#15171c"
@board:
  background: "#030712"
  focus-background: "#07111f"
  edit-background: "#0f172a"
  scope-dim-background: "#0000004d"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@quit_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@new_task_btn:
  background: "#14532d"
  focus-background: "#16a34a"
@mark_all_done_btn:
  background: "#14532d"
  focus-background: "#16a34a"
@clear_board_btn:
  background: "#711212"
  focus-background: "#b91c1c"
@status:
  background: "#111827"
  color: "#93c5fd"
```

## User Interface

```ui
+-header-**--------------------------------------------------------------------+
| title..**................ ..................................... quit_btn.... |
+-filter_cell-**---------------------------------------------------------------+
| filters..**................................................................. |
| *........................................................................... |
| *........................................................................... |
| ............................................................................ |
| ............................................................................ |
| ............................................................................ |
+-actions-24----------------+-board_cell-**------------------------------------+
| new_task_btn..**......... | board..**....................................... |
|                           | ................................................ |
* mark_all_done_btn..**.... | ................................................ |
*                           | ................................................ |
| clear_board_btn..**...... | ................................................ |
|                           | *............................................... |
| status..**............... | *............................................... |
| ......................... | ................................................ |
| ......................... | ................................................ |
|                           | ................................................ |
|                           | ................................................ |
|                           | ................................................ |
+---------------------------+--------------------------------------------------+
```

""", "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.", new[]
        {
            new GeneratedElementMetadata { Name = "title", Description = "Application title.", Expose = false },
            new GeneratedElementMetadata { Name = "quit_btn", Description = "Close the task list example.", Expose = true },
            new GeneratedElementMetadata { Name = "filters", Description = "Reusable task filter panel.", Expose = true },
            new GeneratedElementMetadata { Name = "new_task_btn", Description = "Open a dialog for creating a task.", Expose = true },
            new GeneratedElementMetadata { Name = "mark_all_done_btn", Description = "Ask before marking every task as done.", Expose = true },
            new GeneratedElementMetadata { Name = "clear_board_btn", Description = "Ask before deleting every task.", Expose = true },
            new GeneratedElementMetadata { Name = "board", Description = "Scrollable list of interactive task rows.", Expose = true },
            new GeneratedElementMetadata { Name = "status", Description = "Current board status.", Expose = true },
        });
        SetMcpAppTools(new[]
        {
            new GeneratedAppToolMetadata { Name = "delete_task", Description = "Delete a task by id and return whether it was removed.", InputSchemaJson = """
{"type":"object","properties":{"id":{"type":"string"}},"required":["id"]}
""", OutputSchemaJson = """
{"type":"object","properties":{"deleted":{"type":"boolean"}}}
""" },
            new GeneratedAppToolMetadata { Name = "get_task", Description = "Return one task by id.", InputSchemaJson = """
{"type":"object","properties":{"id":{"type":"string"}},"required":["id"]}
""", OutputSchemaJson = """
{"type":"object","properties":{"task":{"type":"object","properties":{"id":{"type":"string"},"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"description":{"type":"string"},"done":{"type":"boolean"}}}}}
""" },
            new GeneratedAppToolMetadata { Name = "query_tasks", Description = "Return tasks matching optional title, status, assignee, priority, and done filters.", InputSchemaJson = """
{"type":"object","properties":{"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"done":{"type":"boolean"}}}
""", OutputSchemaJson = """
{"type":"object","properties":{"tasks":{"type":"array","items":{"type":"object","properties":{"id":{"type":"string"},"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"description":{"type":"string"},"done":{"type":"boolean"}}}}}}
""" },
            new GeneratedAppToolMetadata { Name = "update_task", Description = "Update a task by id and return the updated task.", InputSchemaJson = """
{"type":"object","properties":{"id":{"type":"string"},"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"description":{"type":"string"},"done":{"type":"boolean"}},"required":["id"]}
""", OutputSchemaJson = """
{"type":"object","properties":{"task":{"type":"object","properties":{"id":{"type":"string"},"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"description":{"type":"string"},"done":{"type":"boolean"}}}}}
""" },
        });
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        title = AddElement(new Label("title", "Task Board"));
        title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        title.SetCursorStyle(new Style
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
        filters = AddElement(new ReusableElement("filters"));
        filters.SetChild(new TaskFiltersUI());
        filters.SetStyle(new Style
    {
        Background = new Color("#15171c")
    });
        filters.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        filters.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        filters.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        new_task_btn = AddElement(new Button("new_task_btn", "New task"));
        new_task_btn.SetStyle(new Style
    {
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    });
        new_task_btn.SetFocusStyle(new Style
    {
        Background = new Color("#16a34a"),
        Color = new Color("#ffffff")
    });
        new_task_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        mark_all_done_btn = AddElement(new Button("mark_all_done_btn", "Mark all done"));
        mark_all_done_btn.SetStyle(new Style
    {
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    });
        mark_all_done_btn.SetFocusStyle(new Style
    {
        Background = new Color("#16a34a"),
        Color = new Color("#ffffff")
    });
        mark_all_done_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        clear_board_btn = AddElement(new Button("clear_board_btn", "Clear board"));
        clear_board_btn.SetStyle(new Style
    {
        Background = new Color("#711212"),
        Color = new Color("#e2e8f0")
    });
        clear_board_btn.SetFocusStyle(new Style
    {
        Background = new Color("#b91c1c"),
        Color = new Color("#ffffff")
    });
        clear_board_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        board = AddElement(new ReusableElement("board"));
        board.SetChild(new TaskListUI());
        board.SetStyle(new Style
    {
        Background = new Color("#030712"),
        ScopeDimBackground = new Color("#0000004d")
    });
        board.SetFocusStyle(new Style
    {
        Background = new Color("#07111f")
    });
        board.SetEditStyle(new Style
    {
        Background = new Color("#0f172a")
    });
        board.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        status = AddElement(new Label("status", "Ready"));
        status.SetStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#93c5fd")
    });
        status.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "board",
                Type = "task_list",
                CellName = "board_cell",
                Relative = new Rect(0, 1, 536870911, 536870911),
                SourceCell = new Rect(9, 28, 50, 12),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(12),
                CharsSize = new Size(48, 12),
                CellCharsSize = new Size(50, 12),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#030712")
    },
                ElementStyle = new Style
    {
        Background = new Color("#030712"),
        ScopeDimBackground = new Color("#0000004d")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "filters",
                Type = "task_filters",
                CellName = "filter_cell",
                Relative = new Rect(0, 1, 536870911, 536870911),
                SourceCell = new Rect(2, 0, 78, 6),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(6),
                CharsSize = new Size(76, 6),
                CellCharsSize = new Size(78, 6),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#15171c")
    },
                ElementStyle = new Style
    {
        Background = new Color("#15171c")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "new_task_btn",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(9, 0, 27, 12),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Fixed(24),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(25, 1),
                CellCharsSize = new Size(27, 12),
                MarginRight = 1,
                MarginBottom = 11,
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
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "title",
                Type = "label",
                CellName = "header",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 78, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(25, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 52,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937")
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
                Name = "quit_btn",
                Type = "button",
                CellName = "header",
                Relative = new Rect(0, 65, 12, 1),
                SourceCell = new Rect(0, 0, 78, 1),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937")
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "mark_all_done_btn",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(2, 1, 536870911, 1),
                SourceCell = new Rect(9, 0, 27, 12),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Fixed(24),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(25, 1),
                CellCharsSize = new Size(27, 12),
                MarginRight = 1,
                MarginBottom = 9,
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
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "clear_board_btn",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(4, 1, 536870911, 1),
                SourceCell = new Rect(9, 0, 27, 12),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Fixed(24),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(25, 1),
                CellCharsSize = new Size(27, 12),
                MarginRight = 1,
                MarginBottom = 7,
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
        Background = new Color("#711212"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status",
                Type = "label",
                CellName = "actions",
                Relative = new Rect(6, 1, 536870911, 3),
                SourceCell = new Rect(9, 0, 27, 12),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Fixed(24),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(25, 3),
                CellCharsSize = new Size(27, 12),
                MarginRight = 1,
                MarginBottom = 3,
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
        Background = new Color("#111827"),
        Color = new Color("#93c5fd")
    },
            },
        };
    }

    protected virtual void onQuitBtnClick() { }
    protected virtual void onNewTaskBtnClick() { }
    protected virtual void onMarkAllDoneBtnClick() { }
    protected virtual void onClearBoardBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "quit_btn")
        {
            onQuitBtnClick();
            return true;
        }
        else if (name == "new_task_btn")
        {
            onNewTaskBtnClick();
            return true;
        }
        else if (name == "mark_all_done_btn")
        {
            onMarkAllDoneBtnClick();
            return true;
        }
        else if (name == "clear_board_btn")
        {
            onClearBoardBtnClick();
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
