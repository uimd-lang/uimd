// Auto-generated UI code for source_separator_scroll - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type SourceSeparatorScrollUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewSourceSeparatorScrollUI() *SourceSeparatorScrollUI {
    ui := &SourceSeparatorScrollUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("Source Separator Scroll")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 13, CellCharsHeight: 4,
            CellName: "panel", CellWidth: 0, CellHeight: 4,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 4,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 4,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
        Gap: uimd.Int(0),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.NewStyle(),
        },
    })
    ui.SetGeneratedKind("window")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetGeneratedScrollViewStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        Gap: uimd.Int(0),
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
    ui.SetMCPMetadata(true, "SourceSeparatorScroll", "# Source Separator Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"ScrollView regression for reusable children whose source rows include design-only separators.\"\ntags: [regression, issue-3, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - source_separator_row\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@panel:\n  gap: 0\n  padding: 0\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----+\n|             |\n|             |\n|             |\n|             |\n+-------------+\n```\n", "ScrollView regression for reusable children whose source rows include design-only separators.")
    return ui
}

func (ui *SourceSeparatorScrollUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *SourceSeparatorScrollUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *SourceSeparatorScrollUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *SourceSeparatorScrollUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *SourceSeparatorScrollUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *SourceSeparatorScrollUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *SourceSeparatorScrollUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
