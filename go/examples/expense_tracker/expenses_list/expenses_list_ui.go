// Auto-generated UI code for expenses_list - DO NOT EDIT MANUALLY.
package expenses_list

import uimd "uimd"

type ExpensesListUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewExpensesListUI() *ExpensesListUI {
    ui := &ExpensesListUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("Expenses List")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 7,
            CellName: "panel", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 54, CharsHeight: 7,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Gap: uimd.Int(1),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.NewStyle(),
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#172033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetGeneratedScrollViewStyle(uimd.Style{
        Background: uimd.NewColor("#172033"),
        Gap: uimd.Int(1),
        Padding: uimd.Int(1),
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
    ui.SetMCPMetadata(true, "ExpensesList", "# Expenses List\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: \"Scrollable list of interactive expense rows.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - ../expense_row/expense_row\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#172033\"\n@panel:\n  gap: 1\n  padding: 1\n  background: \"#172033\"\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------+\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n*                                                      |\n*                                                      |\n|                                                      |\n+------------------------------------------------------+\n```\n", "Scrollable list of interactive expense rows.")
    return ui
}

func (ui *ExpensesListUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ExpensesListUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *ExpensesListUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ExpensesListUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ExpensesListUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
