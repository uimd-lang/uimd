// Auto-generated UI code for image_view - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageViewUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Caption *uimd.Label
    Photo *uimd.Image
}

func NewImageViewUI() *ImageViewUI {
    ui := &ImageViewUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Image View")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 31, CellCharsHeight: 12,
            CellName: "view", CellWidth: 0, CellHeight: 12,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 29, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 11,
            Content: "caption",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
        TextAlign: "center",
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 31, CellCharsHeight: 12,
            CellName: "view", CellWidth: 0, CellHeight: 12,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 29, CharsHeight: 11,
            MarginRight: 1, MarginBottom: 0,
            Content: "photo",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
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
    ui.SetMCPMetadata(true, "ImageView", "# Image View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Full-size image display panel hosted inside a ViewHost.\"\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ncaption:\n  type: label\n  text: \"Select an image\"\n  description: \"Name of the currently displayed image.\"\n\nphoto:\n  type: image\n  source: \"\"\n  alt: \"photo\"\n  fit: contain\n  render_mode: auto\n  description: \"Full-size image display.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@caption:\n  color: \"#bfdbfe\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------------+\n| caption..**.................. |\n| photo..**.................... |\n| ............................. |\n| *............................ |\n| *............................ |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n+-------------------------------+\n```\n", "Full-size image display panel hosted inside a ViewHost.")
    ui.Caption = uimd.NewLabel("caption", "Select an image")
    ui.AddElement(ui.Caption)
    ui.Caption.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
        TextAlign: "center",
    })
    ui.Caption.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Photo = uimd.NewImage("photo", "", "photo", "contain", "auto", "center", "middle")
    ui.AddElement(ui.Photo)
    ui.Photo.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    return ui
}

func (ui *ImageViewUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageViewUI) HandleGeneratedButton(name string) bool {
    if name == "photo" {
        if handler, ok := ui.eventHandler.(interface{ OnPhotoClick() }); ok {
            handler.OnPhotoClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ImageViewUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ImageViewUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageViewUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *ImageViewUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ImageViewUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
