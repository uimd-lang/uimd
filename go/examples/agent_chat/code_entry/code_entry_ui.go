// Auto-generated UI code for code_entry - DO NOT EDIT MANUALLY.
package code_entry

import uimd "uimd"

type CodeEntryUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Heading *uimd.SpanLabel
    Code *uimd.Label
}

func NewCodeEntryUI() *CodeEntryUI {
    ui := &CodeEntryUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Agent Chat Code Entry")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 5,
            CellName: "code_panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 71, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 4,
            Content: "heading",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#0f1b2d"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#ddd6fe"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 5,
            CellName: "code_panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 71, CharsHeight: 4,
            MarginRight: 1, MarginBottom: 0,
            Content: "code",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#0f1b2d"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
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
    ui.SetMCPMetadata(true, "CodeEntry", "# Agent Chat Code Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style code, activity, command output, or diff block.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Code\"\n  description: \"Code language or activity title.\"\n\ncode:\n  type: label\n  text: \"\"\n  description: \"Selectable formatted code, command output, or diff content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@code_panel:\n  background: \"#0f1b2d\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#ddd6fe\"\n@code:\n  color: \"#d1d5db\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-code_panel-**-----------------------------------------------------------+\n| heading..**............................................................ |\n| code..**............................................................... |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n", "Heron-style code, activity, command output, or diff block.")
    ui.Heading = uimd.NewSpanLabel("heading", "Code")
    ui.AddElement(ui.Heading)
    ui.Heading.SetStyle(uimd.Style{
        Color: uimd.NewColor("#ddd6fe"),
    })
    ui.Code = uimd.NewLabel("code", "")
    ui.AddElement(ui.Code)
    ui.Code.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
        UserSelect: "text",
    })
    ui.Code.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *CodeEntryUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *CodeEntryUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *CodeEntryUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *CodeEntryUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *CodeEntryUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *CodeEntryUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *CodeEntryUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
