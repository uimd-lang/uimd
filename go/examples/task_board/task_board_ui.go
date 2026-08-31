// Auto-generated UI code for task_board - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type TaskBoardUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Title *uimd.Label
    QuitBtn *uimd.Button
    Filters *uimd.ReusableElement
    NewTaskBtn *uimd.Button
    MarkAllDoneBtn *uimd.Button
    ClearBoardBtn *uimd.Button
    Board *uimd.ReusableElement
    Status *uimd.Label
}

func NewTaskBoardUI() *TaskBoardUI {
    ui := &TaskBoardUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Task Board")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 9, CellCol: 28,
            CellCharsWidth: 50, CellCharsHeight: 12,
            CellName: "board_cell", CellWidth: 0, CellHeight: 12,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 48, CharsHeight: 12,
            MarginRight: 1, MarginBottom: 0,
            Content: "board",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
        ScopeDimBackground: uimd.NewColor("#0000004d"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 6,
            CellName: "filter_cell", CellWidth: 0, CellHeight: 6,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 76, CharsHeight: 6,
            MarginRight: 1, MarginBottom: 0,
            Content: "filters",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#15171c"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#15171c"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 9, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 12,
            CellName: "actions", CellWidth: 24, CellHeight: 0,
            CellWidthMode: "fixed", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 25, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 11,
            Content: "new_task_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 25, CharsHeight: 1,
            MarginRight: 52, MarginBottom: 0,
            Content: "title",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 65,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "quit_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 9, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 12,
            CellName: "actions", CellWidth: 24, CellHeight: 0,
            CellWidthMode: "fixed", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 25, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 9,
            Content: "mark_all_done_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 4, Col: 1,
            CellRow: 9, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 12,
            CellName: "actions", CellWidth: 24, CellHeight: 0,
            CellWidthMode: "fixed", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 25, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 7,
            Content: "clear_board_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#711212"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 6, Col: 1,
            CellRow: 9, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 12,
            CellName: "actions", CellWidth: 24, CellHeight: 0,
            CellWidthMode: "fixed", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 3,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 25, CharsHeight: 3,
            MarginRight: 1, MarginBottom: 3,
            Content: "status",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
    })
    ui.SetGeneratedKind("window")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "TaskBoard", "# Task Board\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.\"\ntags: [example, mcp]\n```\n\n## Definition\n\n```yaml\nuses:\n  - task_filters/task_filters\n  - task_list/task_list\n  - task_dialog/task_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Task Board\"\n  description: \"Application title.\"\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the task list example.\"\n\nfilters:\n  type: task_filters\n  cpp-class: task_board_example::TaskFilters\n  cpp-header: \"task_filters/task_filters.hpp\"\n  description: \"Reusable task filter panel.\"\n\nnew_task_btn:\n  type: button\n  title: New task\n  description: \"Open a dialog for creating a task.\"\n\nmark_all_done_btn:\n  type: button\n  title: Mark all done\n  description: \"Ask before marking every task as done.\"\n\nclear_board_btn:\n  type: button\n  title: Clear board\n  description: \"Ask before deleting every task.\"\n\nboard:\n  type: task_list\n  cpp-class: task_board_example::TaskList\n  cpp-header: \"task_list/task_list.hpp\"\n  description: \"Scrollable list of interactive task rows.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Current board status.\"\n```\n\n## Tools\n\n```yaml\nquery_tasks:\n  description: \"Return tasks matching optional title, status, assignee, priority, and done filters.\"\n  input:\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    done: bool\n  output:\n    tasks:\n      - id: string\n        title: string\n        status: string\n        assignee: string\n        priority: string\n        description: string\n        done: bool\n\nget_task:\n  description: \"Return one task by id.\"\n  input:\n    id: string\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\nupdate_task:\n  description: \"Update a task by id and return the updated task.\"\n  input:\n    id: string\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    description: string\n    done: bool\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\ndelete_task:\n  description: \"Delete a task by id and return whether it was removed.\"\n  input:\n    id: string\n    required: [id]\n  output:\n    deleted: bool\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@filter_cell:\n  background: \"#15171c\"\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@board_cell:\n  background: \"#030712\"\n@filters:\n  background: \"#15171c\"\n@board:\n  background: \"#030712\"\n  focus-background: \"#07111f\"\n  edit-background: \"#0f172a\"\n  scope-dim-background: \"#0000004d\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@new_task_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@mark_all_done_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@clear_board_btn:\n  background: \"#711212\"\n  focus-background: \"#b91c1c\"\n@status:\n  background: \"#111827\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**................ ..................................... quit_btn.... |\n+-filter_cell-**---------------------------------------------------------------+\n| filters..**................................................................. |\n| *........................................................................... |\n| *........................................................................... |\n| ............................................................................ |\n| ............................................................................ |\n| ............................................................................ |\n+-actions-24----------------+-board_cell-**------------------------------------+\n| new_task_btn..**......... | board..**....................................... |\n|                           | ................................................ |\n* mark_all_done_btn..**.... | ................................................ |\n*                           | ................................................ |\n| clear_board_btn..**...... | ................................................ |\n|                           | *............................................... |\n| status..**............... | *............................................... |\n| ......................... | ................................................ |\n| ......................... | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n+---------------------------+--------------------------------------------------+\n```\n", "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.")
    ui.Title = uimd.NewLabel("title", "Task Board")
    ui.AddElement(ui.Title)
    ui.Title.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.Title.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.QuitBtn = uimd.NewButton("quit_btn", "Quit")
    ui.AddElement(ui.QuitBtn)
    ui.QuitBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.QuitBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.QuitBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Filters = uimd.NewReusableElement("filters", "task_filters")
    ui.AddElement(ui.Filters)
    ui.Filters.SetStyle(uimd.Style{
        Background: uimd.NewColor("#15171c"),
    })
    ui.Filters.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.Filters.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.Filters.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NewTaskBtn = uimd.NewButton("new_task_btn", "New task")
    ui.AddElement(ui.NewTaskBtn)
    ui.NewTaskBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.NewTaskBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#16a34a"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NewTaskBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.MarkAllDoneBtn = uimd.NewButton("mark_all_done_btn", "Mark all done")
    ui.AddElement(ui.MarkAllDoneBtn)
    ui.MarkAllDoneBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.MarkAllDoneBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#16a34a"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.MarkAllDoneBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ClearBoardBtn = uimd.NewButton("clear_board_btn", "Clear board")
    ui.AddElement(ui.ClearBoardBtn)
    ui.ClearBoardBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#711212"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ClearBoardBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#b91c1c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ClearBoardBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Board = uimd.NewReusableElement("board", "task_list")
    ui.AddElement(ui.Board)
    ui.Board.SetStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        ScopeDimBackground: uimd.NewColor("#0000004d"),
    })
    ui.Board.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#07111f"),
    })
    ui.Board.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
    })
    ui.Board.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Status = uimd.NewLabel("status", "Ready")
    ui.AddElement(ui.Status)
    ui.Status.SetStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.Status.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *TaskBoardUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TaskBoardUI) HandleGeneratedButton(name string) bool {
    if name == "quit_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnQuitBtnClick() }); ok {
            handler.OnQuitBtnClick()
            return true
        }
        return false
    }
    if name == "new_task_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnNewTaskBtnClick() }); ok {
            handler.OnNewTaskBtnClick()
            return true
        }
        return false
    }
    if name == "mark_all_done_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnMarkAllDoneBtnClick() }); ok {
            handler.OnMarkAllDoneBtnClick()
            return true
        }
        return false
    }
    if name == "clear_board_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnClearBoardBtnClick() }); ok {
            handler.OnClearBoardBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *TaskBoardUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *TaskBoardUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *TaskBoardUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *TaskBoardUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *TaskBoardUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
