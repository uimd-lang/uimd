// Auto-generated UI code for image_list_scroll - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageListScrollUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
}

func NewImageListScrollUI() *ImageListScrollUI {
    ui := &ImageListScrollUI{GeneratedWindowBase: uimd.NewGeneratedScrollViewBase("Image List Scroll")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 73, CellCharsHeight: 10,
            CellName: "panel", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 73, CharsHeight: 10,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
        Gap: uimd.Int(1),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.NewStyle(),
        },
    })
    ui.SetGeneratedKind("component")
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
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    })
    ui.SetGeneratedScrollViewFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e3a5f"),
    })
    ui.SetGeneratedScrollViewEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.SetGeneratedScrollViewSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SetGeneratedScrollViewDescendantFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e3a5f"),
    })
    ui.SetMCPMetadata(true, "ImageListScroll", "# Image List Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\ndescription: \"Scrollable image item list.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - image_list_item\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n  focus-background: \"#1e3a5f\"\n@panel:\n  gap: 1\n  padding: 1, 1, 1, 1\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------------------------------------------------+\n|                                                                         |\n|                                                                         |\n|                                                                         |\n|                                                                         |\n|                                                                         |\n*                                                                         |\n*                                                                         |\n*                                                                         |\n*                                                                         |\n|                                                                         |\n+-------------------------------------------------------------------------+\n```\n", "Scrollable image item list.")
    return ui
}

func (ui *ImageListScrollUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageListScrollUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *ImageListScrollUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ImageListScrollUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageListScrollUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *ImageListScrollUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ImageListScrollUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
