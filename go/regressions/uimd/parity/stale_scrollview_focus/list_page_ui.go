// Auto-generated UI code for list_page - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ListPageUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewListPageUI() *ListPageUI {
    ui := &ListPageUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("List Page")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 28,
            CellName: "panel", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 90, CharsHeight: 28,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
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
        Background: uimd.NewColor("#030712"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetGeneratedScrollViewStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
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
    ui.SetMCPMetadata(true, "ListPage", "# List Page\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Scrollable list page with interactive row buttons for the ViewHost focus regression.\"\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - item_row\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@panel:\n  gap: 1\n  padding: 1, 1, 1, 1\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------------------------------------------+\n|                                                                                          |\n*                                                                                          |\n*                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n|                                                                                          |\n+------------------------------------------------------------------------------------------+\n```\n", "Scrollable list page with interactive row buttons for the ViewHost focus regression.")
    return ui
}

func (ui *ListPageUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ListPageUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *ListPageUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ListPageUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ListPageUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
