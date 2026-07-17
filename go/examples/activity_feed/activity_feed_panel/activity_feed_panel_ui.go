// Auto-generated UI code for activity_feed_panel - DO NOT EDIT MANUALLY.
package activity_feed_panel

import uimd "uimd"

type ActivityFeedPanelUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewActivityFeedPanelUI() *ActivityFeedPanelUI {
    ui := &ActivityFeedPanelUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("Activity Feed Panel")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 9,
            CellName: "panel", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 54, CharsHeight: 9,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
        Gap: uimd.Int(1),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
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
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
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
    ui.SetMCPMetadata(true, "ActivityFeedPanel", "# Activity Feed Panel\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable scroll view for activity feed items.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - ../activity_item/activity_item\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  gap: 1\n  padding: 1, 0, 1, 0\n  background: \"#00000000\"\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------+\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n*                                                      |\n*                                                      |\n*                                                      |\n|                                                      |\n+------------------------------------------------------+\n```\n", "Reusable scroll view for activity feed items.")
    return ui
}

func (ui *ActivityFeedPanelUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ActivityFeedPanelUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *ActivityFeedPanelUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ActivityFeedPanelUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ActivityFeedPanelUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
