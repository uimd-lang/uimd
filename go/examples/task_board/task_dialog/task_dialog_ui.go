// Auto-generated UI code for task_dialog - DO NOT EDIT MANUALLY.
package task_dialog

import uimd "uimd"

type TaskDialogUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Header *uimd.Label
    NameLabel *uimd.Label
    Name *uimd.TextInput
    StatusLabel *uimd.Label
    ProjectStatus *uimd.ComboBox
    OwnerLabel *uimd.Label
    Owner *uimd.ComboBox
    PriorityLabel *uimd.Label
    Priority *uimd.ComboBox
    NoteLabel *uimd.Label
    Note *uimd.TextArea
    Message *uimd.Label
    AddBtn *uimd.Button
    CancelBtn *uimd.Button
}

func NewTaskDialogUI() *TaskDialogUI {
    ui := &TaskDialogUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Task Dialog")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 1,
            CellName: "", CellWidth: 54, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 54, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "header",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 2,
            CellRow: 20, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 2,
            CellName: "", CellWidth: 54, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 23, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 23, CharsHeight: 1,
            MarginRight: 29, MarginBottom: 1,
            Content: "add_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 27,
            CellRow: 20, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 2,
            CellName: "", CellWidth: 54, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 24, CharsHeight: 1,
            MarginRight: 3, MarginBottom: 1,
            Content: "cancel_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 10, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 42, MarginBottom: 15,
            Content: "name_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 18,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 33, CharsHeight: 1,
            MarginRight: 3, MarginBottom: 15,
            Content: "name",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 40, MarginBottom: 13,
            Content: "status_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 3, Col: 18,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 33, CharsHeight: 1,
            MarginRight: 3, MarginBottom: 13,
            Content: "project_status",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 5, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 41, MarginBottom: 11,
            Content: "owner_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 5, Col: 18,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 33, CharsHeight: 1,
            MarginRight: 3, MarginBottom: 11,
            Content: "owner",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 7, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 14, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 38, MarginBottom: 9,
            Content: "priority_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 7, Col: 18,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 33, CharsHeight: 1,
            MarginRight: 3, MarginBottom: 9,
            Content: "priority",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 9, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 41, MarginBottom: 7,
            Content: "note_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 9, Col: 18,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 5,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 33, CharsHeight: 5,
            MarginRight: 3, MarginBottom: 3,
            Content: "note",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 15, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 17,
            CellName: "", CellWidth: 54, CellHeight: 17,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 49, CharsHeight: 1,
            MarginRight: 3, MarginBottom: 1,
            Content: "message",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#fbbf24"),
    },
        },
    })
    ui.SetGeneratedKind("dialog")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "TaskDialog", "# Task Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: draft\ndescription: \"Dialog form for creating or editing a task.\"\ntags: [example, dialog]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Task\n  description: \"Dialog title.\"\n  expose: false\n\nname_label:\n  type: label\n  text: Title\n  description: \"Label for task title.\"\n  expose: false\n\nname:\n  type: textinput\n  value: \"\"\n  maxlength: 60\n  description: \"Required task title.\"\n\nstatus_label:\n  type: label\n  text: Status\n  description: \"Label for task status.\"\n  expose: false\n\nproject_status:\n  type: combobox\n  options: [Todo, Doing, Blocked, Done]\n  selected_item: Todo\n  description: \"Task status.\"\n\nowner_label:\n  type: label\n  text: Assignee\n  description: \"Label for task assignee.\"\n  expose: false\n\nowner:\n  type: combobox\n  options: [Ava, Bruno, Chen, Dana]\n  selected_item: Ava\n  description: \"Task assignee.\"\n\npriority_label:\n  type: label\n  text: Priority\n  description: \"Label for task priority.\"\n  expose: false\n\npriority:\n  type: combobox\n  options: [Low, Med, High, Urgent]\n  selected_item: Med\n  description: \"Task priority.\"\n\nnote_label:\n  type: label\n  text: Description\n  description: \"Label for task description.\"\n  expose: false\n\nnote:\n  type: textarea\n  value: \"\"\n  maxlength: 160\n  description: \"Task description.\"\n\nmessage:\n  type: label\n  text: \"\"\n  description: \"Validation message.\"\n\nadd_btn:\n  type: button\n  title: Save task\n  description: \"Save the task and close the dialog.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Close the dialog without saving the task.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@add_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@message:\n  color: \"#fbbf24\"\n```\n\n## User Interface\n\n```ui\n+------------------------------------------------------+\n|header................................................|\n+------------------------------------------------------+\n|                                                      |\n|  name_label      name.............................   |\n|                                                      |\n|  status_label    project_status...................   |\n|                                                      |\n|  owner_label     owner............................   |\n|                                                      |\n|  priority_label  priority.........................   |\n|                                                      |\n|  note_label.     note.............................   |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                                                      |\n|  message..........................................   |\n|                                                      |\n+------------------------------------------------------+\n|  add_btn................  cancel_btn..............   |\n|                                                      |\n+------------------------------------------------------+\n```\n", "Dialog form for creating or editing a task.")
    ui.Header = uimd.NewLabel("header", "Task")
    ui.AddElement(ui.Header)
    ui.Header.SetStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.Header.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.NameLabel = uimd.NewLabel("name_label", "Title")
    ui.AddElement(ui.NameLabel)
    ui.NameLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.NameLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Name = uimd.NewTextInput("name", "", 0)
    ui.AddElement(ui.Name)
    ui.Name.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Name.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Name.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Name.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.Name.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Name.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.StatusLabel = uimd.NewLabel("status_label", "Status")
    ui.AddElement(ui.StatusLabel)
    ui.StatusLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.StatusLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ProjectStatus = uimd.NewComboBox("project_status", []string{"Todo", "Doing", "Blocked", "Done"})
    ui.AddElement(ui.ProjectStatus)
    ui.ProjectStatus.SetSelectedIndex(0)
    ui.ProjectStatus.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ProjectStatus.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ProjectStatus.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ProjectStatus.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ProjectStatus.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.OwnerLabel = uimd.NewLabel("owner_label", "Assignee")
    ui.AddElement(ui.OwnerLabel)
    ui.OwnerLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.OwnerLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Owner = uimd.NewComboBox("owner", []string{"Ava", "Bruno", "Chen", "Dana"})
    ui.AddElement(ui.Owner)
    ui.Owner.SetSelectedIndex(0)
    ui.Owner.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Owner.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Owner.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Owner.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Owner.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.PriorityLabel = uimd.NewLabel("priority_label", "Priority")
    ui.AddElement(ui.PriorityLabel)
    ui.PriorityLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.PriorityLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Priority = uimd.NewComboBox("priority", []string{"Low", "Med", "High", "Urgent"})
    ui.AddElement(ui.Priority)
    ui.Priority.SetSelectedIndex(1)
    ui.Priority.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Priority.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Priority.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Priority.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Priority.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.NoteLabel = uimd.NewLabel("note_label", "Description")
    ui.AddElement(ui.NoteLabel)
    ui.NoteLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.NoteLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Note = uimd.NewTextArea("note", "")
    ui.AddElement(ui.Note)
    ui.Note.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Note.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Note.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Note.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.Note.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Note.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Message = uimd.NewLabel("message", "")
    ui.AddElement(ui.Message)
    ui.Message.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#fbbf24"),
    })
    ui.Message.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.AddBtn = uimd.NewButton("add_btn", "Save task")
    ui.AddElement(ui.AddBtn)
    ui.AddBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.AddBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#16a34a"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AddBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CancelBtn = uimd.NewButton("cancel_btn", "Cancel")
    ui.AddElement(ui.CancelBtn)
    ui.CancelBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.CancelBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CancelBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *TaskDialogUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TaskDialogUI) HandleGeneratedButton(name string) bool {
    if name == "add_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnAddBtnClick() }); ok {
            handler.OnAddBtnClick()
            return true
        }
        return false
    }
    if name == "cancel_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnCancelBtnClick() }); ok {
            handler.OnCancelBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *TaskDialogUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "name" {
        if handler, ok := ui.eventHandler.(interface{ OnNameChange(string) }); ok {
            handler.OnNameChange(value)
            return true
        }
        return false
    }
    if name == "note" {
        if handler, ok := ui.eventHandler.(interface{ OnNoteChange(string) }); ok {
            handler.OnNoteChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *TaskDialogUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "name" {
        if handler, ok := ui.eventHandler.(interface{ OnNameSubmit(string) }); ok {
            handler.OnNameSubmit(value)
            return true
        }
        return false
    }
    if name == "note" {
        if handler, ok := ui.eventHandler.(interface{ OnNoteSubmit(string) }); ok {
            handler.OnNoteSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *TaskDialogUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "project_status" {
        if handler, ok := ui.eventHandler.(interface{ OnProjectStatusChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnProjectStatusChange(selected)
            return true
        }
        return false
    }
    if name == "owner" {
        if handler, ok := ui.eventHandler.(interface{ OnOwnerChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnOwnerChange(selected)
            return true
        }
        return false
    }
    if name == "priority" {
        if handler, ok := ui.eventHandler.(interface{ OnPriorityChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnPriorityChange(selected)
            return true
        }
        return false
    }
    return false
}
