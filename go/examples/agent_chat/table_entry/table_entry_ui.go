// Auto-generated UI code for table_entry - DO NOT EDIT MANUALLY.
package table_entry

import uimd "uimd"

type TableEntryUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Heading *uimd.SpanLabel
    Table *uimd.MessageTable
}

func NewTableEntryUI() *TableEntryUI {
    ui := &TableEntryUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Agent Chat Table Entry")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 5,
            CellName: "table_panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 70, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 4,
            Content: "heading",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
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
            CellName: "table_panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 71, CharsHeight: 4,
            MarginRight: 1, MarginBottom: 0,
            Content: "table",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#cbd5e1"),
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
    ui.SetMCPMetadata(true, "TableEntry", "# Agent Chat Table Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style simulated agent Markdown table block.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Table\"\n  description: \"Table title.\"\n\ntable:\n  type: messagetable\n  text: \"\"\n  description: \"Rendered Markdown table content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@table_panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#ddd6fe\"\n@table:\n  color: \"#cbd5e1\"\n```\n\n## User Interface\n\n```ui\n+-table_panel-**----------------------------------------------------------+\n| heading..**........................................................... .|\n| table..**.............................................................. |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n", "Heron-style simulated agent Markdown table block.")
    ui.Heading = uimd.NewSpanLabel("heading", "Table")
    ui.AddElement(ui.Heading)
    ui.Heading.SetStyle(uimd.Style{
        Color: uimd.NewColor("#ddd6fe"),
    })
    ui.Table = uimd.NewMessageTable("table")
    ui.AddElement(ui.Table)
    ui.Table.SetStyle(uimd.Style{
        Color: uimd.NewColor("#cbd5e1"),
    })
    return ui
}

func (ui *TableEntryUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TableEntryUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *TableEntryUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *TableEntryUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *TableEntryUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *TableEntryUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *TableEntryUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
