// Auto-generated UI code for image_list_view - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageListViewUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    AddBtn *uimd.Button
    Items *uimd.ReusableElement
}

func NewImageListViewUI() *ImageListViewUI {
    ui := &ImageListViewUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Image List View")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 11,
            CellName: "items_cell", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 71, CharsHeight: 11,
            MarginRight: 0, MarginBottom: 0,
            Content: "items",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 1,
            CellName: "toolbar", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 19, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 19, CharsHeight: 1,
            MarginRight: 51, MarginBottom: 0,
            Content: "add_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
    })
    ui.SetGeneratedKind("control")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ImageListView", "# Image List View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"ViewHost page with add button and scrollable image item list.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_list_scroll\n```\n\n## Members\n\n```yaml\nadd_btn:\n  type: button\n  title: Add\n  description: \"Add an image item.\"\n\nitems:\n  type: image_list_scroll\n  description: \"Scrollable list of image items.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@toolbar:\n  background: \"#030712\"\n@items_cell:\n  background: \"#030712\"\nbutton:\n  background: \"#14532d\"\n  color: \"#e2e8f0\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-toolbar-**------------------------------------------------------------+\n| add_btn............                                                   |\n+-items_cell-**---------------------------------------------------------+\n|items..**..............................................................|\n*.......................................................................|\n**......................................................................|\n|*......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n+-----------------------------------------------------------------------+\n```\n", "ViewHost page with add button and scrollable image item list.")
    ui.AddBtn = uimd.NewButton("add_btn", "Add")
    ui.AddElement(ui.AddBtn)
    ui.AddBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.AddBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#16a34a"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AddBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Items = uimd.NewReusableElement("items", "image_list_scroll")
    ui.AddElement(ui.Items)
    ui.Items.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.Items.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.Items.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    return ui
}

func (ui *ImageListViewUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageListViewUI) HandleGeneratedButton(name string) bool {
    if name == "add_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnAddBtnClick() }); ok {
            handler.OnAddBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ImageListViewUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ImageListViewUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageListViewUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
