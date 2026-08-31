// Auto-generated UI code for image_list_button - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageListButtonUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Prw1 *uimd.Image
    Prw2 *uimd.Image
    Prw3 *uimd.Image
}

func NewImageListButtonUI() *ImageListButtonUI {
    ui := &ImageListButtonUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Image List Button")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 25, CellCharsHeight: 3,
            CellName: "view", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 3,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 3,
            MarginRight: 17, MarginBottom: 0,
            Content: "prw_1",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 0, Col: 9,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 25, CellCharsHeight: 3,
            CellName: "view", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 3,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 3,
            MarginRight: 9, MarginBottom: 0,
            Content: "prw_2",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 0, Col: 17,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 25, CellCharsHeight: 3,
            CellName: "view", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 3,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 3,
            MarginRight: 1, MarginBottom: 0,
            Content: "prw_3",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
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
    ui.SetMCPMetadata(true, "ImageListButton", "# Image List Button\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Clickable image-list preview tile with three thumbnails.\"\ntags: [example, image, button, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\nprw_1:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"Preview image 1\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 1. Click to open image items.\"\n\nprw_2:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"Preview image 2\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 2. Click to open image items.\"\n\nprw_3:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"Preview image 3\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 3. Click to open image items.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 0, 1, 0\n  background: \"#172033\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------+\n| prw_1.. prw_2.. prw_3.. |\n| ....... ....... ....... |\n| ....... ....... ....... |\n+-------------------------+\n```\n", "Clickable image-list preview tile with three thumbnails.")
    ui.Prw1 = uimd.NewImage("prw_1", "shared/assets/image_samples/camera.png", "Preview image 1", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Prw1)
    ui.Prw1.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Prw1.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    ui.Prw2 = uimd.NewImage("prw_2", "shared/assets/image_samples/astronaut.png", "Preview image 2", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Prw2)
    ui.Prw2.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Prw2.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    ui.Prw3 = uimd.NewImage("prw_3", "shared/assets/image_samples/coffee.png", "Preview image 3", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Prw3)
    ui.Prw3.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Prw3.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    return ui
}

func (ui *ImageListButtonUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageListButtonUI) HandleGeneratedButton(name string) bool {
    if name == "prw_1" {
        if handler, ok := ui.eventHandler.(interface{ OnPrw1Click() }); ok {
            handler.OnPrw1Click()
            return true
        }
        return false
    }
    if name == "prw_2" {
        if handler, ok := ui.eventHandler.(interface{ OnPrw2Click() }); ok {
            handler.OnPrw2Click()
            return true
        }
        return false
    }
    if name == "prw_3" {
        if handler, ok := ui.eventHandler.(interface{ OnPrw3Click() }); ok {
            handler.OnPrw3Click()
            return true
        }
        return false
    }
    return false
}

func (ui *ImageListButtonUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ImageListButtonUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageListButtonUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *ImageListButtonUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ImageListButtonUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
