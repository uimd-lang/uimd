// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
package chat_entry

import uimd "uimd"

type ChatEntryUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Role *uimd.Label
    Phase *uimd.Label
    Body *uimd.SpanLabel
    Detail *uimd.Label
}

func NewChatEntryUI() *ChatEntryUI {
    ui := &ChatEntryUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Agent Chat Entry")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 6,
            CellName: "body_panel", CellWidth: 0, CellHeight: 6,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 6,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 71, CharsHeight: 6,
            MarginRight: 1, MarginBottom: 0,
            Content: "body",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#d1d5db"),
        UserSelect: "text",
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 9, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 1,
            CellName: "footer", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 71, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "detail",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#64748b"),
        UserSelect: "text",
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 48, CharsHeight: 1,
            MarginRight: 24, MarginBottom: 0,
            Content: "role",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#162033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#a7f3d0"),
    },
        },
        {
            Row: 0, Col: 50,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 22, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 22, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "phase",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#162033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#c4b5fd"),
        TextAlign: "right",
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
    ui.SetMCPMetadata(true, "ChatEntry", "# Agent Chat Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Fixed-height simulated agent chat entry for prose and structured transcript blocks.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nrole:\n  type: label\n  text: Agent\n  description: \"Simulated transcript speaker or activity type.\"\n\nphase:\n  type: label\n  text: response\n  description: \"Deterministic scenario phase.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable simulated transcript content.\"\n\ndetail:\n  type: label\n  text: \"simulated · no external action\"\n  description: \"Simulation-only activity detail.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@header:\n  background: \"#162033\"\n  padding: 0, 1, 0, 1\n@role:\n  color: \"#a7f3d0\"\n@phase:\n  color: \"#c4b5fd\"\n  text-align: right\n@body_panel:\n  background: \"#0b1220\"\n  padding: 1, 1, 1, 1\n@body:\n  color: \"#d1d5db\"\n  word-wrap: false\n  user-select: text\n@footer:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@detail:\n  color: \"#64748b\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-header-**---------------------------------------------------------------+\n| role..**........................................ phase................. |\n+-body_panel-**-----------------------------------------------------------+\n| body..**............................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n+-footer-**---------------------------------------------------------------+\n| detail..**............................................................. |\n+-------------------------------------------------------------------------+\n```\n", "Fixed-height simulated agent chat entry for prose and structured transcript blocks.")
    ui.Role = uimd.NewLabel("role", "Agent")
    ui.AddElement(ui.Role)
    ui.Role.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#a7f3d0"),
    })
    ui.Role.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Phase = uimd.NewLabel("phase", "response")
    ui.AddElement(ui.Phase)
    ui.Phase.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#c4b5fd"),
        TextAlign: "right",
    })
    ui.Phase.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Body = uimd.NewSpanLabel("body", "")
    ui.AddElement(ui.Body)
    ui.Body.SetStyle(uimd.Style{
        Color: uimd.NewColor("#d1d5db"),
        UserSelect: "text",
    })
    ui.Detail = uimd.NewLabel("detail", "simulated · no external action")
    ui.AddElement(ui.Detail)
    ui.Detail.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#64748b"),
        UserSelect: "text",
    })
    ui.Detail.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
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
