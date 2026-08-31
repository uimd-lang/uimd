// Auto-generated UI code for image_gallery - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageGalleryUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Header *uimd.Label
    CloseBtn *uimd.Button
    FallbackLabel *uimd.Label
    SixelLabel *uimd.Label
    StretchLabel *uimd.Label
    CameraFb *uimd.Image
    AstroFb *uimd.Image
    CoffeeFb *uimd.Image
    CoinsFb *uimd.Image
    CameraCtn *uimd.Image
    AstroCtn *uimd.Image
    CoffeeCtn *uimd.Image
    CoinsCtn *uimd.Image
    CameraStr *uimd.Image
    AstroStr *uimd.Image
    CoffeeStr *uimd.Image
    CoinsStr *uimd.Image
    Status *uimd.Label
}

func NewImageGalleryUI() *ImageGalleryUI {
    ui := &ImageGalleryUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Image Gallery")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 72, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 72, CharsHeight: 1,
            MarginRight: 13, MarginBottom: 38,
            Content: "header",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 74,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 38,
            Content: "close_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#ffffff"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 84, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 36,
            Content: "fallback_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 64, MarginBottom: 26,
            Content: "camera_fb",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 3, Col: 23,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 43, MarginBottom: 26,
            Content: "astro_fb",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 3, Col: 44,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 22, MarginBottom: 26,
            Content: "coffee_fb",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 3, Col: 65,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 10,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 21, CharsHeight: 10,
            MarginRight: 0, MarginBottom: 26,
            Content: "coins_fb",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 14, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 84, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 24,
            Content: "sixel_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 15, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 64, MarginBottom: 14,
            Content: "camera_ctn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 15, Col: 23,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 43, MarginBottom: 14,
            Content: "astro_ctn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 15, Col: 44,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 22, MarginBottom: 14,
            Content: "coffee_ctn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 15, Col: 65,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 10,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 1, MarginBottom: 14,
            Content: "coins_ctn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 26, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 84, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 12,
            Content: "stretch_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 27, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 64, MarginBottom: 2,
            Content: "camera_str",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 27, Col: 23,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 43, MarginBottom: 2,
            Content: "astro_str",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 27, Col: 44,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 10,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 22, MarginBottom: 2,
            Content: "coffee_str",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 27, Col: 65,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 10,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 10,
            MarginRight: 1, MarginBottom: 2,
            Content: "coins_str",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 38, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 86, CellCharsHeight: 39,
            CellName: "", CellWidth: 86, CellHeight: 39,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 84, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "status",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
    })
    ui.SetGeneratedKind("window")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ImageGallery", "# Image Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: \"Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.\"\ntags: [example, image]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Image Gallery\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the image gallery.\"\n\nfallback_label:\n  type: label\n  text: \"Fallback cover\"\n  description: \"Fallback row label.\"\n\nsixel_label:\n  type: label\n  text: \"Sixel contain\"\n  description: \"Sixel contain row label.\"\n\nstretch_label:\n  type: label\n  text: \"Sixel stretch\"\n  description: \"Sixel stretch row label.\"\n\ncamera_fb:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Camera image forced through fallback rendering.\"\n\nastro_fb:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Astronaut image forced through fallback rendering.\"\n\ncoffee_fb:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Coffee image forced through fallback rendering.\"\n\ncoins_fb:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Coins image forced through fallback rendering.\"\n\ncamera_ctn:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Camera image in forced Sixel contain mode.\"\n\nastro_ctn:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Astronaut image in forced Sixel contain mode.\"\n\ncoffee_ctn:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Coffee image in forced Sixel contain mode.\"\n\ncoins_ctn:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Coins image in forced Sixel contain mode.\"\n\ncamera_str:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Camera image in forced Sixel stretch mode.\"\n\nastro_str:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Astronaut image in forced Sixel stretch mode.\"\n\ncoffee_str:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Coffee image in forced Sixel stretch mode.\"\n\ncoins_str:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Coins image in forced Sixel stretch mode.\"\n\nstatus:\n  type: label\n  text: \"Rows: fallback cover, forced Sixel contain, forced Sixel stretch.\"\n  description: \"Current image validation status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@fallback_label:\n  color: \"#93c5fd\"\n@sixel_label:\n  color: \"#93c5fd\"\n@stretch_label:\n  color: \"#93c5fd\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n@status:\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------------------------+\n| header.................................................................. close_btn.. |\n|                                                                                      |\n| fallback_label...................................................................... |\n|  camera_fb........... astro_fb............ coffee_fb........... coins_fb............. |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| sixel_label......................................................................... |\n|  camera_ctn.......... astro_ctn........... coffee_ctn.......... coins_ctn........... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| stretch_label....................................................................... |\n|  camera_str.......... astro_str........... coffee_str.......... coins_str........... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| status.............................................................................. |\n+--------------------------------------------------------------------------------------+\n```\n", "Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.")
    ui.Header = uimd.NewLabel("header", "Image Gallery")
    ui.AddElement(ui.Header)
    ui.Header.SetStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.Header.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.CloseBtn = uimd.NewButton("close_btn", "Quit")
    ui.AddElement(ui.CloseBtn)
    ui.CloseBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CloseBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CloseBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.FallbackLabel = uimd.NewLabel("fallback_label", "Fallback cover")
    ui.AddElement(ui.FallbackLabel)
    ui.FallbackLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.FallbackLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.SixelLabel = uimd.NewLabel("sixel_label", "Sixel contain")
    ui.AddElement(ui.SixelLabel)
    ui.SixelLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.SixelLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.StretchLabel = uimd.NewLabel("stretch_label", "Sixel stretch")
    ui.AddElement(ui.StretchLabel)
    ui.StretchLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.StretchLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.CameraFb = uimd.NewImage("camera_fb", "shared/assets/image_samples/camera.png", "camera fallback", "cover", "fallback", "center", "middle")
    ui.AddElement(ui.CameraFb)
    ui.CameraFb.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.AstroFb = uimd.NewImage("astro_fb", "shared/assets/image_samples/astronaut.png", "astronaut fallback", "cover", "fallback", "center", "middle")
    ui.AddElement(ui.AstroFb)
    ui.AstroFb.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CoffeeFb = uimd.NewImage("coffee_fb", "shared/assets/image_samples/coffee.png", "coffee fallback", "cover", "fallback", "center", "middle")
    ui.AddElement(ui.CoffeeFb)
    ui.CoffeeFb.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CoinsFb = uimd.NewImage("coins_fb", "shared/assets/image_samples/coins.png", "coins fallback", "cover", "fallback", "center", "middle")
    ui.AddElement(ui.CoinsFb)
    ui.CoinsFb.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CameraCtn = uimd.NewImage("camera_ctn", "shared/assets/image_samples/camera.png", "camera sixel", "contain", "sixel", "center", "middle")
    ui.AddElement(ui.CameraCtn)
    ui.CameraCtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.AstroCtn = uimd.NewImage("astro_ctn", "shared/assets/image_samples/astronaut.png", "astronaut sixel", "contain", "sixel", "center", "middle")
    ui.AddElement(ui.AstroCtn)
    ui.AstroCtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CoffeeCtn = uimd.NewImage("coffee_ctn", "shared/assets/image_samples/coffee.png", "coffee sixel", "contain", "sixel", "center", "middle")
    ui.AddElement(ui.CoffeeCtn)
    ui.CoffeeCtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CoinsCtn = uimd.NewImage("coins_ctn", "shared/assets/image_samples/coins.png", "coins sixel", "contain", "sixel", "center", "middle")
    ui.AddElement(ui.CoinsCtn)
    ui.CoinsCtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CameraStr = uimd.NewImage("camera_str", "shared/assets/image_samples/camera.png", "camera stretch", "stretch", "sixel", "center", "middle")
    ui.AddElement(ui.CameraStr)
    ui.CameraStr.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.AstroStr = uimd.NewImage("astro_str", "shared/assets/image_samples/astronaut.png", "astronaut stretch", "stretch", "sixel", "center", "middle")
    ui.AddElement(ui.AstroStr)
    ui.AstroStr.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CoffeeStr = uimd.NewImage("coffee_str", "shared/assets/image_samples/coffee.png", "coffee stretch", "stretch", "sixel", "center", "middle")
    ui.AddElement(ui.CoffeeStr)
    ui.CoffeeStr.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CoinsStr = uimd.NewImage("coins_str", "shared/assets/image_samples/coins.png", "coins stretch", "stretch", "sixel", "center", "middle")
    ui.AddElement(ui.CoinsStr)
    ui.CoinsStr.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.Status = uimd.NewLabel("status", "Rows: fallback cover, forced Sixel contain, forced Sixel stretch.")
    ui.AddElement(ui.Status)
    ui.Status.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.Status.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *ImageGalleryUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageGalleryUI) HandleGeneratedButton(name string) bool {
    if name == "close_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnCloseBtnClick() }); ok {
            handler.OnCloseBtnClick()
            return true
        }
        return false
    }
    if name == "camera_fb" {
        if handler, ok := ui.eventHandler.(interface{ OnCameraFbClick() }); ok {
            handler.OnCameraFbClick()
            return true
        }
        return false
    }
    if name == "astro_fb" {
        if handler, ok := ui.eventHandler.(interface{ OnAstroFbClick() }); ok {
            handler.OnAstroFbClick()
            return true
        }
        return false
    }
    if name == "coffee_fb" {
        if handler, ok := ui.eventHandler.(interface{ OnCoffeeFbClick() }); ok {
            handler.OnCoffeeFbClick()
            return true
        }
        return false
    }
    if name == "coins_fb" {
        if handler, ok := ui.eventHandler.(interface{ OnCoinsFbClick() }); ok {
            handler.OnCoinsFbClick()
            return true
        }
        return false
    }
    if name == "camera_ctn" {
        if handler, ok := ui.eventHandler.(interface{ OnCameraCtnClick() }); ok {
            handler.OnCameraCtnClick()
            return true
        }
        return false
    }
    if name == "astro_ctn" {
        if handler, ok := ui.eventHandler.(interface{ OnAstroCtnClick() }); ok {
            handler.OnAstroCtnClick()
            return true
        }
        return false
    }
    if name == "coffee_ctn" {
        if handler, ok := ui.eventHandler.(interface{ OnCoffeeCtnClick() }); ok {
            handler.OnCoffeeCtnClick()
            return true
        }
        return false
    }
    if name == "coins_ctn" {
        if handler, ok := ui.eventHandler.(interface{ OnCoinsCtnClick() }); ok {
            handler.OnCoinsCtnClick()
            return true
        }
        return false
    }
    if name == "camera_str" {
        if handler, ok := ui.eventHandler.(interface{ OnCameraStrClick() }); ok {
            handler.OnCameraStrClick()
            return true
        }
        return false
    }
    if name == "astro_str" {
        if handler, ok := ui.eventHandler.(interface{ OnAstroStrClick() }); ok {
            handler.OnAstroStrClick()
            return true
        }
        return false
    }
    if name == "coffee_str" {
        if handler, ok := ui.eventHandler.(interface{ OnCoffeeStrClick() }); ok {
            handler.OnCoffeeStrClick()
            return true
        }
        return false
    }
    if name == "coins_str" {
        if handler, ok := ui.eventHandler.(interface{ OnCoinsStrClick() }); ok {
            handler.OnCoinsStrClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ImageGalleryUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ImageGalleryUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageGalleryUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *ImageGalleryUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ImageGalleryUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
