// Auto-generated UI code for task_list - DO NOT EDIT MANUALLY.
package task_list

import uimd "uimd"

type TaskListUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewTaskListUI() *TaskListUI {
    ui := &TaskListUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("Task List")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 59, CellCharsHeight: 10,
            CellName: "panel", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 59, CharsHeight: 10,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
        Gap: uimd.Int(1),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.NewStyle(),
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#00000000"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetGeneratedScrollViewStyle(uimd.Style{
        Background: uimd.NewColor("#00000000"),
        Gap: uimd.Int(1),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    })
    ui.SetGeneratedScrollViewFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.SetGeneratedScrollViewEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.SetGeneratedScrollViewSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SetMCPMetadata(true, "TaskList", "# Task List\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable scroll view for interactive task rows.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - ../task_row/task_row\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  gap: 1\n  padding: 1, 1, 1, 1\n  background: \"#00000000\"\n```\n\n## User Interface\n\n```ui\n+-panel-**--------------------------------------------------+\n|                                                           |\n|                                                           |\n|                                                           |\n|                                                           |\n|                                                           |\n*                                                           |\n*                                                           |\n*                                                           |\n*                                                           |\n|                                                           |\n+-----------------------------------------------------------+\n```\n", "Reusable scroll view for interactive task rows.")
    return ui
}

func (ui *TaskListUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TaskListUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *TaskListUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *TaskListUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *TaskListUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
