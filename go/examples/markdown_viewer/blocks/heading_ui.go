// Auto-generated UI code for heading - DO NOT EDIT MANUALLY.
package blocks

import uimd "uimd"

type HeadingUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    HeadingText *uimd.SpanLabel
}

func NewHeadingUI() *HeadingUI {
    ui := &HeadingUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Heading")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 48, CellCharsHeight: 1,
            CellName: "heading_cell", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 46, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "heading_text",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#dbeafe"),
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
    ui.SetMCPMetadata(true, "Heading", "# Heading\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown heading block.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading_text:\n  type: spanlabel\n  text: \"\"\n  description: \"Rendered heading text with mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@heading_cell:\n  background: \"#00000000\"\n@heading_text:\n  color: \"#dbeafe\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-heading_cell-**--------------------------------+\n| heading_text..**.............................. |\n+------------------------------------------------+\n```\n", "Reusable markdown heading block.")
    ui.HeadingText = uimd.NewSpanLabel("heading_text", "")
    ui.AddElement(ui.HeadingText)
    ui.HeadingText.SetStyle(uimd.Style{
        Color: uimd.NewColor("#dbeafe"),
        UserSelect: "text",
    })
    return ui
}

func (ui *HeadingUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *HeadingUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *HeadingUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *HeadingUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *HeadingUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *HeadingUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *HeadingUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
