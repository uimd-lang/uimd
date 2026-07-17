// Auto-generated UI code for image_button - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageButtonUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Photo *uimd.Image
}

func NewImageButtonUI() *ImageButtonUI {
    ui := &ImageButtonUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Image Button")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 24, CellCharsHeight: 4,
            CellName: "view", CellWidth: 0, CellHeight: 4,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 4,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 22, CharsHeight: 4,
            MarginRight: 1, MarginBottom: 0,
            Content: "photo",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(4),
        PaddingRight: uimd.Int(4),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
    })
    ui.SetGeneratedKind("control")
    ui.SetGeneratedFocusable(true)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#172033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ImageButton", "# Image Button\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Single-image clickable tile with padding around a centered square image.\"\ntags: [example, image, button]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\nphoto:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: cover\n  render_mode: auto\n  align: center\n  valign: middle\n  description: \"Tile image. Click to select.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 4, 1, 4\n  background: transparent\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------+\n| photo..**............. |\n| ...................... |\n| ...................... |\n| ...................... |\n+------------------------+\n```\n", "Single-image clickable tile with padding around a centered square image.")
    ui.Photo = uimd.NewImage("photo", "", "", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Photo)
    ui.Photo.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Photo.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    return ui
}

func (ui *ImageButtonUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageButtonUI) HandleGeneratedButton(name string) bool {
    if name == "photo" {
        if handler, ok := ui.eventHandler.(interface{ OnPhotoClick() }); ok {
            handler.OnPhotoClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ImageButtonUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ImageButtonUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageButtonUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
