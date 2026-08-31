// Auto-generated UI code for gallery_view - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type GalleryViewUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    GalleryScroll *uimd.ReusableElement
}

func NewGalleryViewUI() *GalleryViewUI {
    ui := &GalleryViewUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Gallery View")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 6,
            CellName: "view", CellWidth: 0, CellHeight: 6,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 25, CharsHeight: 6,
            MarginRight: 1, MarginBottom: 0,
            Content: "gallery_scroll",
            CellStyle: uimd.NewStyle(),
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
    ui.SetMCPMetadata(true, "GalleryView", "# Gallery View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Gallery view hosting a scrollable list of full-size images.\"\ntags: [example, image, gallery, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - gallery_scroll\n```\n\n## Members\n\n```yaml\ngallery_scroll:\n  type: gallery_scroll\n  description: \"Scrollable image list.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-view-**-------------------+\n| gallery_scroll..**....... |\n| ......................... |\n| *........................ |\n| *........................ |\n| ......................... |\n| ......................... |\n+---------------------------+\n```\n", "Gallery view hosting a scrollable list of full-size images.")
    ui.GalleryScroll = uimd.NewReusableElement("gallery_scroll", "gallery_scroll")
    ui.AddElement(ui.GalleryScroll)
    ui.GalleryScroll.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.GalleryScroll.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.GalleryScroll.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    return ui
}

func (ui *GalleryViewUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *GalleryViewUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *GalleryViewUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *GalleryViewUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *GalleryViewUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *GalleryViewUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *GalleryViewUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
