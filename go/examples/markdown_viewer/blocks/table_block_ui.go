// Auto-generated UI code for table_block - DO NOT EDIT MANUALLY.
package blocks

import uimd "uimd"

type TableBlockUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Kind *uimd.Label
    Table *uimd.MessageTable
}

func NewTableBlockUI() *TableBlockUI {
    ui := &TableBlockUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Table Block")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
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
            Content: "kind",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1d314c"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
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
            Content: "table",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#0c1a2d"),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#e5e7eb"),
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
    ui.SetMCPMetadata(true, "TableBlock", "# Table Block\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown table block with a small header and message table content.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nkind:\n  type: label\n  text: \"markdown table\"\n  description: \"Header label identifying the block as a markdown table.\"\n\ntable:\n  type: messagetable\n  text: \"\"\n  description: \"Rendered markdown table content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@head:\n  background: \"#1d314c\"\n@body:\n  background: \"#0c1a2d\"\n@kind:\n  color: \"#93c5fd\"\n@table:\n  color: \"#e5e7eb\"\n```\n\n## User Interface\n\n```ui\n+-head-**----------------------------------------+\n| kind..**...................................... |\n+-body-**----------------------------------------+\n| table..**..................................... |\n| .............................................. |\n# #............................................ |\n# #............................................ |\n+------------------------------------------------+\n```\n", "Reusable markdown table block with a small header and message table content.")
    ui.Kind = uimd.NewLabel("kind", "markdown table")
    ui.AddElement(ui.Kind)
    ui.Kind.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.Kind.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Table = uimd.NewMessageTable("table")
    ui.AddElement(ui.Table)
    ui.Table.SetStyle(uimd.Style{
        Color: uimd.NewColor("#e5e7eb"),
    })
    return ui
}

func (ui *TableBlockUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TableBlockUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *TableBlockUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *TableBlockUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *TableBlockUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
