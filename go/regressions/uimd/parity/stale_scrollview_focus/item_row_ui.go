// Auto-generated UI code for item_row - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ItemRowUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Name *uimd.Label
    Summary *uimd.Label
    Spacer *uimd.Label
    OpenBtn *uimd.Button
}

func NewItemRowUI() *ItemRowUI {
    ui := &ItemRowUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Item Row")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 59, CharsHeight: 1,
            MarginRight: 30, MarginBottom: 1,
            Content: "name",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        UserSelect: "text",
    },
        },
        {
            Row: 0, Col: 61,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 28, Height: 2,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 28, CharsHeight: 2,
            MarginRight: 1, MarginBottom: 0,
            Content: "summary",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
        UserSelect: "text",
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 74, CharsHeight: 1,
            MarginRight: 15, MarginBottom: 0,
            Content: "spacer",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 76,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 2,
            CellName: "card", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 13, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "open_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ItemRow", "# Item Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Interactive row with an Open button inside a ScrollView.\"\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nname:\n  type: label\n  text: \"Item\"\n  description: \"Item name.\"\n\nsummary:\n  type: label\n  text: \"Summary\"\n  description: \"Item summary.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible row spacer.\"\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open this item in the edit page.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@name:\n  color: \"#f8fafc\"\n  user-select: text\n@summary:\n  color: \"#93c5fd\"\n  user-select: text\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@open_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-card-**----------------------------------------------------------------------------------+\n| name..**................................................... summary..................... |\n| spacer..**................................................................ open_btn..... |\n+------------------------------------------------------------------------------------------+\n```\n", "Interactive row with an Open button inside a ScrollView.")
    ui.Name = uimd.NewLabel("name", "Item")
    ui.AddElement(ui.Name)
    ui.Name.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        UserSelect: "text",
    })
    ui.Name.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Summary = uimd.NewLabel("summary", "Summary")
    ui.AddElement(ui.Summary)
    ui.Summary.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
        UserSelect: "text",
    })
    ui.Summary.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Spacer = uimd.NewLabel("spacer", "")
    ui.AddElement(ui.Spacer)
    ui.Spacer.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Spacer.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.OpenBtn = uimd.NewButton("open_btn", "Open")
    ui.AddElement(ui.OpenBtn)
    ui.OpenBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.OpenBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#16a34a"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.OpenBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *ItemRowUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ItemRowUI) HandleGeneratedButton(name string) bool {
    if name == "open_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnOpenBtnClick() }); ok {
            handler.OnOpenBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ItemRowUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ItemRowUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ItemRowUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
