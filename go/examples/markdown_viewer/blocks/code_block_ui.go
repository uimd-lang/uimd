// Auto-generated UI code for code_block - DO NOT EDIT MANUALLY.
package blocks

import uimd "uimd"

type CodeBlockUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Lang *uimd.Label
    Code *uimd.SpanLabel
}

func NewCodeBlockUI() *CodeBlockUI {
    ui := &CodeBlockUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Code Block")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 48, CellCharsHeight: 4,
            CellName: "body", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 46, CharsHeight: 4,
            MarginRight: 1, MarginBottom: 0,
            Content: "code",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#0c1a2d"),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#d1d5db"),
        UserSelect: "text",
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 48, CellCharsHeight: 1,
            CellName: "head", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 46, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "lang",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1d314c"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
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
    ui.SetMCPMetadata(true, "CodeBlock", "# Code Block\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown code block with a language header and selectable code text.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nlang:\n  type: label\n  text: code\n  description: \"Language label for the code block.\"\n\ncode:\n  type: spanlabel\n  text: \"\"\n  description: \"Rendered code content with horizontal preservation and mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@head:\n  background: \"#1d314c\"\n@body:\n  background: \"#0c1a2d\"\n@lang:\n  color: \"#93c5fd\"\n@code:\n  color: \"#d1d5db\"\n  word-wrap: false\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-head-**----------------------------------------+\n| lang..**...................................... |\n+-body-**----------------------------------------+\n| code..**...................................... |\n| .............................................. |\n# #............................................ |\n# #............................................ |\n+------------------------------------------------+\n```\n", "Reusable markdown code block with a language header and selectable code text.")
    ui.Lang = uimd.NewLabel("lang", "code")
    ui.AddElement(ui.Lang)
    ui.Lang.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.Lang.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Code = uimd.NewSpanLabel("code", "")
    ui.AddElement(ui.Code)
    ui.Code.SetStyle(uimd.Style{
        Color: uimd.NewColor("#d1d5db"),
        UserSelect: "text",
    })
    return ui
}

func (ui *CodeBlockUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *CodeBlockUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *CodeBlockUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *CodeBlockUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *CodeBlockUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *CodeBlockUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *CodeBlockUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
