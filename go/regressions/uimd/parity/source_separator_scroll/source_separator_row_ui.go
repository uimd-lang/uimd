// Auto-generated UI code for source_separator_row - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type SourceSeparatorRowUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Sep *uimd.Label
    Left *uimd.Label
}

func NewSourceSeparatorRowUI() *SourceSeparatorRowUI {
    ui := &SourceSeparatorRowUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Source Separator Row")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 11, CellCharsHeight: 1,
            CellName: "", CellWidth: 11, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "left",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#15171c"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 11, CellCharsHeight: 1,
            CellName: "", CellWidth: 11, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "sep",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#15171c"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#15171c"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "SourceSeparatorRow", "# Source Separator Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Regression row with a design-only ASCII separator between content rows.\"\ntags: [regression, issue-3, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nsep:\n  type: label\n  text: Header\n\nleft:\n  type: label\n  text: Left\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#15171c\"\ncell:\n  background: \"#15171c\"\nlabel:\n  color: \"#e5e7eb\"\n```\n\n## User Interface\n\n```ui\n+-----------+\n|sep........|\n+-----------+\n|left.......|\n+-----------+\n```\n", "Regression row with a design-only ASCII separator between content rows.")
    ui.Sep = uimd.NewLabel("sep", "Header")
    ui.AddElement(ui.Sep)
    ui.Sep.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Sep.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Left = uimd.NewLabel("left", "Left")
    ui.AddElement(ui.Left)
    ui.Left.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Left.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *SourceSeparatorRowUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *SourceSeparatorRowUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *SourceSeparatorRowUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *SourceSeparatorRowUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *SourceSeparatorRowUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
