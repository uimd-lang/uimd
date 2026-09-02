// Auto-generated UI code for transcript - DO NOT EDIT MANUALLY.
package transcript

import uimd "uimd"

type TranscriptUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewTranscriptUI() *TranscriptUI {
    ui := &TranscriptUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("Agent Chat Transcript")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 5,
            CellName: "panel", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 73, CharsHeight: 5,
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
    ui.SetMCPMetadata(true, "Transcript", "# Agent Chat Transcript\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Scrollable collection of deterministic simulated agent chat entries.\"\ntags: [component, simulation, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - ../chat_entry/chat_entry\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  gap: 1\n  padding: 1, 1, 1, 1\n  background: \"#00000000\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------------------------------------------------+\n|                                                                         |\n|                                                                         |\n*                                                                         |\n*                                                                         |\n|                                                                         |\n+-------------------------------------------------------------------------+\n```\n", "Scrollable collection of deterministic simulated agent chat entries.")
    return ui
}

func (ui *TranscriptUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TranscriptUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *TranscriptUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *TranscriptUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *TranscriptUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *TranscriptUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *TranscriptUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
