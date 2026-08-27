// Auto-generated UI code for compat_scroll - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type CompatScrollUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    AlphaRow *uimd.Label
}

func NewCompatScrollUI() *CompatScrollUI {
    ui := &CompatScrollUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("UIMD 0.5.3 Compatibility Scroll")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#303545"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetGeneratedScrollViewStyle(uimd.Style{
        Background: uimd.NewColor("#303545"),
        Padding: uimd.Int(0),
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
    ui.SetMCPMetadata(true, "CompatScroll", "# UIMD 0.5.3 Compatibility Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.\"\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\n```\n\n## Members\n\n```yaml\nalpha_row:\n  type: label\n  text: \"legacy alpha row\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#303545\"\n@panel:\n  background: \"#303545\"\n  padding: 0\n@alpha_row:\n  background: \"#252a36cc\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------+\n|alpha_row.....................|\n|..............................|\n+------------------------------+\n```\n", "Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.")
    ui.AlphaRow = uimd.NewLabel("alpha_row", "legacy alpha row")
    ui.AddElement(ui.AlphaRow)
    ui.AlphaRow.SetStyle(uimd.Style{
        Background: uimd.NewColor("#252a36cc"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.AlphaRow.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *CompatScrollUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *CompatScrollUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *CompatScrollUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *CompatScrollUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *CompatScrollUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
