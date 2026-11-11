// Auto-generated UI code for user_entry - DO NOT EDIT MANUALLY.
package user_entry

import uimd "uimd"

type UserEntryUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Body *uimd.SpanLabel
}

func NewUserEntryUI() *UserEntryUI {
    ui := &UserEntryUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Agent Chat User Entry")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 74, CellCharsHeight: 4,
            CellName: "panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 72, CharsHeight: 4,
            MarginRight: 1, MarginBottom: 0,
            Content: "body",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#e0f2fe"),
        UserSelect: "text",
    },
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
    ui.SetMCPMetadata(true, "UserEntry", "# Agent Chat User Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style user message with a separate accent.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable user prompt.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@body:\n  color: \"#e0f2fe\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-panel-**-----------------------------------------------------------------+\n| body..**................................................................ |\n| ........................................................................ |\n# #....................................................................... |\n# #....................................................................... |\n+--------------------------------------------------------------------------+\n```\n", "Heron-style user message with a separate accent.")
    ui.Body = uimd.NewSpanLabel("body", "")
    ui.AddElement(ui.Body)
    ui.Body.SetStyle(uimd.Style{
        Color: uimd.NewColor("#e0f2fe"),
        UserSelect: "text",
    })
    return ui
}

func (ui *UserEntryUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *UserEntryUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *UserEntryUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *UserEntryUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *UserEntryUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *UserEntryUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *UserEntryUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
