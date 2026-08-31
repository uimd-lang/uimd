// Auto-generated UI code for document_view - DO NOT EDIT MANUALLY.
package document_view

import uimd "uimd"

type DocumentViewUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewDocumentViewUI() *DocumentViewUI {
    ui := &DocumentViewUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("Document View")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 10,
            CellName: "panel", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 54, CharsHeight: 10,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
        Gap: uimd.Int(1),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(2),
        PaddingRight: uimd.Int(2),
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
        PaddingLeft: uimd.Int(2),
        PaddingRight: uimd.Int(2),
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
    ui.SetMCPMetadata(true, "DocumentView", "# Document View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable scroll view that contains rendered markdown blocks such as paragraphs, headings, code blocks, and tables.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - blocks/paragraph\n  - blocks/heading\n  - blocks/code_block\n  - blocks/table_block\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@panel:\n  gap: 1\n  background: \"#00000000\"\n  padding: 1, 2, 1, 2\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------+\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n*                                                      |\n*                                                      |\n*                                                      |\n*                                                      |\n|                                                      |\n+------------------------------------------------------+\n```\n", "Reusable scroll view that contains rendered markdown blocks such as paragraphs, headings, code blocks, and tables.")
    return ui
}

func (ui *DocumentViewUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *DocumentViewUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *DocumentViewUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *DocumentViewUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *DocumentViewUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *DocumentViewUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *DocumentViewUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
