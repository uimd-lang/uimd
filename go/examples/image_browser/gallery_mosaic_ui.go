// Auto-generated UI code for gallery_mosaic - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type GalleryMosaicUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Gal1 *uimd.Image
    Gal2 *uimd.Image
    Gal3 *uimd.Image
    Gal4 *uimd.Image
    Gal5 *uimd.Image
}

func NewGalleryMosaicUI() *GalleryMosaicUI {
    ui := &GalleryMosaicUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Gallery Mosaic")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 26, CellCharsHeight: 7,
            CellName: "view", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 3,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 3,
            MarginRight: 18, MarginBottom: 4,
            Content: "gal_1",
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
            CellCharsWidth: 26, CellCharsHeight: 7,
            CellName: "view", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 3,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 3,
            MarginRight: 10, MarginBottom: 4,
            Content: "gal_2",
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
            CellCharsWidth: 26, CellCharsHeight: 7,
            CellName: "view", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 8, Height: 3,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 8, CharsHeight: 3,
            MarginRight: 1, MarginBottom: 4,
            Content: "gal_3",
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
            Row: 4, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 26, CellCharsHeight: 7,
            CellName: "view", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 3,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 3,
            MarginRight: 18, MarginBottom: 0,
            Content: "gal_4",
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
            Row: 4, Col: 9,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 26, CellCharsHeight: 7,
            CellName: "view", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 3,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 3,
            MarginRight: 10, MarginBottom: 0,
            Content: "gal_5",
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
    ui.SetMCPMetadata(true, "GalleryMosaic", "# Gallery Mosaic\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"3+2 image mosaic that opens the scrollable gallery when clicked.\"\ntags: [example, image, gallery]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\ngal_1:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"Camera\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 1. Click to open gallery.\"\n\ngal_2:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"Astronaut\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 2. Click to open gallery.\"\n\ngal_3:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"Coffee\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 3. Click to open gallery.\"\n\ngal_4:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"Coins\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 4. Click to open gallery.\"\n\ngal_5:\n  type: image\n  source: \"shared/assets/image_samples/chelsea.png\"\n  alt: \"Chelsea\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 5. Click to open gallery.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 0, 1, 0\n  background: \"#172033\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**------------------+\n| gal_1.. gal_2.. gal_3... |\n| ....... ....... ........ |\n| ....... ....... ........ |\n|                          |\n| gal_4.. gal_5..          |\n| ....... .......          |\n| ....... .......          |\n+--------------------------+\n```\n", "3+2 image mosaic that opens the scrollable gallery when clicked.")
    ui.Gal1 = uimd.NewImage("gal_1", "shared/assets/image_samples/camera.png", "Camera", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Gal1)
    ui.Gal1.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Gal1.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    ui.Gal2 = uimd.NewImage("gal_2", "shared/assets/image_samples/astronaut.png", "Astronaut", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Gal2)
    ui.Gal2.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Gal2.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    ui.Gal3 = uimd.NewImage("gal_3", "shared/assets/image_samples/coffee.png", "Coffee", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Gal3)
    ui.Gal3.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Gal3.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    ui.Gal4 = uimd.NewImage("gal_4", "shared/assets/image_samples/coins.png", "Coins", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Gal4)
    ui.Gal4.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Gal4.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    ui.Gal5 = uimd.NewImage("gal_5", "shared/assets/image_samples/chelsea.png", "Chelsea", "cover", "auto", "center", "middle")
    ui.AddElement(ui.Gal5)
    ui.Gal5.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Gal5.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
    })
    return ui
}

func (ui *GalleryMosaicUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *GalleryMosaicUI) HandleGeneratedButton(name string) bool {
    if name == "gal_1" {
        if handler, ok := ui.eventHandler.(interface{ OnGal1Click() }); ok {
            handler.OnGal1Click()
            return true
        }
        return false
    }
    if name == "gal_2" {
        if handler, ok := ui.eventHandler.(interface{ OnGal2Click() }); ok {
            handler.OnGal2Click()
            return true
        }
        return false
    }
    if name == "gal_3" {
        if handler, ok := ui.eventHandler.(interface{ OnGal3Click() }); ok {
            handler.OnGal3Click()
            return true
        }
        return false
    }
    if name == "gal_4" {
        if handler, ok := ui.eventHandler.(interface{ OnGal4Click() }); ok {
            handler.OnGal4Click()
            return true
        }
        return false
    }
    if name == "gal_5" {
        if handler, ok := ui.eventHandler.(interface{ OnGal5Click() }); ok {
            handler.OnGal5Click()
            return true
        }
        return false
    }
    return false
}

func (ui *GalleryMosaicUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *GalleryMosaicUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *GalleryMosaicUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *GalleryMosaicUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *GalleryMosaicUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
