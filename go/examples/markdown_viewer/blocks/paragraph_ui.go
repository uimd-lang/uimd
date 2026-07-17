// Auto-generated UI code for paragraph - DO NOT EDIT MANUALLY.
package blocks

import uimd "uimd"

type ParagraphUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Text *uimd.Label
}

func NewParagraphUI() *ParagraphUI {
    ui := &ParagraphUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Paragraph")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 48, CellCharsHeight: 3,
            CellName: "body", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 3,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 46, CharsHeight: 3,
            MarginRight: 1, MarginBottom: 0,
            Content: "text",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
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
    ui.SetMCPMetadata(true, "Paragraph", "# Paragraph\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown paragraph block.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntext:\n  type: label\n  text: \"\"\n  description: \"Rendered paragraph text with mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@text:\n  color: \"#cbd5e1\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-body-**----------------------------------------+\n| text..**...................................... |\n| .............................................. |\n# #............................................ |\n+------------------------------------------------+\n```\n", "Reusable markdown paragraph block.")
    ui.Text = uimd.NewLabel("text", "")
    ui.AddElement(ui.Text)
    ui.Text.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        UserSelect: "text",
    })
    ui.Text.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *ParagraphUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ParagraphUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *ParagraphUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ParagraphUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ParagraphUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
