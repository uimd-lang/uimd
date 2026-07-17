// Auto-generated UI code for gallery_scroll - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type GalleryScrollUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewGalleryScrollUI() *GalleryScrollUI {
    ui := &GalleryScrollUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("Gallery Scroll")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 40, CellCharsHeight: 7,
            CellName: "panel", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 40, CharsHeight: 7,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
        Gap: uimd.Int(1),
    },
            ElementStyle: uimd.NewStyle(),
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
    ui.SetGeneratedScrollViewStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        Gap: uimd.Int(1),
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
    ui.SetMCPMetadata(true, "GalleryScroll", "# Gallery Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Scrollable container for gallery image items.\"\ntags: [example, image, gallery]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - gallery_item\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@panel:\n  gap: 1\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-panel-**-------------------------------+\n|                                        |\n*                                        |\n*                                        |\n|                                        |\n|                                        |\n|                                        |\n|                                        |\n+----------------------------------------+\n```\n", "Scrollable container for gallery image items.")
    return ui
}

func (ui *GalleryScrollUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *GalleryScrollUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *GalleryScrollUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *GalleryScrollUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *GalleryScrollUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
