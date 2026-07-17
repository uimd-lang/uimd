// Auto-generated UI code for task_row - DO NOT EDIT MANUALLY.
package task_row

import uimd "uimd"

type TaskRowUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    TaskTitle *uimd.Label
    Meta *uimd.Label
    Done *uimd.CheckBox
    Spacer *uimd.Label
    OpenBtn *uimd.Button
    DeleteBtn *uimd.Button
}

func NewTaskRowUI() *TaskRowUI {
    ui := &TaskRowUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Task Row")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 47, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 21, CharsHeight: 1,
            MarginRight: 25, MarginBottom: 1,
            Content: "task_title",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        UserSelect: "text",
    },
        },
        {
            Row: 0, Col: 23,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 47, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 23, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 23, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 1,
            Content: "meta",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
        UserSelect: "text",
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 47, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 8, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 8, CharsHeight: 1,
            MarginRight: 38, MarginBottom: 0,
            Content: "done",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 10,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 47, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 25, MarginBottom: 0,
            Content: "spacer",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 23,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 47, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 10, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 14, MarginBottom: 0,
            Content: "open_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 34,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 47, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "delete_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "TaskRow", "# Task Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable task row with editable controls.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntask_title:\n  type: label\n  text: \"Task\"\n  description: \"Task title.\"\n\nmeta:\n  type: label\n  text: \"Status / assignee / priority\"\n  description: \"Task status, assignee, and priority.\"\n\ndone:\n  type: checkbox\n  title: Done\n  value: false\n  description: \"Whether the task is done.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible row spacer before task actions.\"\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open this task in the edit dialog.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Ask for confirmation and delete this task.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@task_title:\n  color: \"#f8fafc\"\n  user-select: text\n@meta:\n  color: \"#93c5fd\"\n  user-select: text\ncheckbox:\n  color: \"#cbd5e1\"\n  focus-color: \"#facc15\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------+\n| task_title..**....... meta................... |\n| done.... spacer..**.. open_btn.. delete_btn.. |\n+-----------------------------------------------+\n```\n", "Reusable task row with editable controls.")
    ui.TaskTitle = uimd.NewLabel("task_title", "Task")
    ui.AddElement(ui.TaskTitle)
    ui.TaskTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        UserSelect: "text",
    })
    ui.TaskTitle.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Meta = uimd.NewLabel("meta", "Status / assignee / priority")
    ui.AddElement(ui.Meta)
    ui.Meta.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
        UserSelect: "text",
    })
    ui.Meta.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Done = uimd.NewCheckBox("done", "Done", false)
    ui.AddElement(ui.Done)
    ui.Done.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Done.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#facc15"),
    })
    ui.Done.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#22c55e"),
    })
    ui.Done.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#94a3b8"),
    })
    ui.Done.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.Spacer = uimd.NewLabel("spacer", "")
    ui.AddElement(ui.Spacer)
    ui.Spacer.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Spacer.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.OpenBtn = uimd.NewButton("open_btn", "Open")
    ui.AddElement(ui.OpenBtn)
    ui.OpenBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.OpenBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.OpenBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.DeleteBtn = uimd.NewButton("delete_btn", "Delete")
    ui.AddElement(ui.DeleteBtn)
    ui.DeleteBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.DeleteBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DeleteBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *TaskRowUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TaskRowUI) HandleGeneratedButton(name string) bool {
    if name == "open_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnOpenBtnClick() }); ok {
            handler.OnOpenBtnClick()
            return true
        }
        return false
    }
    if name == "delete_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnDeleteBtnClick() }); ok {
            handler.OnDeleteBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *TaskRowUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "done" {
        if handler, ok := ui.eventHandler.(interface{ OnDoneChange(string) }); ok {
            handler.OnDoneChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *TaskRowUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *TaskRowUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
