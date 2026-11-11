// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
package chat_entry

import uimd "uimd"

type ChatEntryUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Heading *uimd.SpanLabel
    Body *uimd.SpanLabel
}

func NewChatEntryUI() *ChatEntryUI {
    ui := &ChatEntryUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Agent Chat Entry")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 5,
            CellName: "body_panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 71, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 4,
            Content: "heading",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#a7f3d0"),
        UserSelect: "text",
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 5,
            CellName: "body_panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 71, CharsHeight: 4,
            MarginRight: 1, MarginBottom: 0,
            Content: "body",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#cbd5e1"),
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
    ui.SetMCPMetadata(true, "ChatEntry", "# Agent Chat Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style assistant text entry.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Assistant\"\n  description: \"Assistant role label.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable Markdown-style transcript prose.\"\n\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@body_panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#a7f3d0\"\n  user-select: text\n@body:\n  color: \"#cbd5e1\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-body_panel-**-----------------------------------------------------------+\n| heading..**............................................................ |\n| body..**............................................................... |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n", "Heron-style assistant text entry.")
    ui.Heading = uimd.NewSpanLabel("heading", "Assistant")
    ui.AddElement(ui.Heading)
    ui.Heading.SetStyle(uimd.Style{
        Color: uimd.NewColor("#a7f3d0"),
        UserSelect: "text",
    })
    ui.Body = uimd.NewSpanLabel("body", "")
    ui.AddElement(ui.Body)
    ui.Body.SetStyle(uimd.Style{
        Color: uimd.NewColor("#cbd5e1"),
        UserSelect: "text",
    })
    return ui
}

func (ui *ChatEntryUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ChatEntryUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *ChatEntryUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ChatEntryUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ChatEntryUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *ChatEntryUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ChatEntryUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
