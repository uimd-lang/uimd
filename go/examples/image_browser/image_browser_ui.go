// Auto-generated UI code for image_browser - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageBrowserUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Title *uimd.Label
    CloseBtn *uimd.Button
    RenderLabel *uimd.Label
    ModeSelect *uimd.ComboBox
    SidebarTitle *uimd.Label
    CameraThumb *uimd.ReusableElement
    AstroThumb *uimd.ReusableElement
    CoffeeThumb *uimd.ReusableElement
    GalleryLabel *uimd.Label
    ImageListLabel *uimd.Label
    GalleryMosaic *uimd.ReusableElement
    ImageListBtn *uimd.ReusableElement
    Spacer *uimd.Label
    Main *uimd.ViewHost
}

func NewImageBrowserUI() *ImageBrowserUI {
    ui := &ImageBrowserUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Image Browser")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 29,
            CellCharsWidth: 49, CellCharsHeight: 41,
            CellName: "content", CellWidth: 0, CellHeight: 41,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 47, CharsHeight: 41,
            MarginRight: 1, MarginBottom: 0,
            Content: "main",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 40,
            Content: "render_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 64, CharsHeight: 1,
            MarginRight: 13, MarginBottom: 0,
            Content: "title",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 66,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "close_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#ffffff"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 39,
            Content: "mode_select",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 3, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 37,
            Content: "sidebar_title",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 4, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 6,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 6,
            MarginRight: 1, MarginBottom: 31,
            Content: "camera_thumb",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 10, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 6,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 6,
            MarginRight: 1, MarginBottom: 25,
            Content: "astro_thumb",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 16, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 6,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 6,
            MarginRight: 1, MarginBottom: 19,
            Content: "coffee_thumb",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 22, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 18,
            Content: "gallery_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 23, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 9,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 9,
            MarginRight: 1, MarginBottom: 9,
            Content: "gallery_mosaic",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 32, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 8,
            Content: "image_list_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 33, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 5,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 5,
            MarginRight: 1, MarginBottom: 3,
            Content: "image_list_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 38, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 41,
            CellName: "sidebar", CellWidth: 28, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 3,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 3,
            MarginRight: 1, MarginBottom: 0,
            Content: "spacer",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
    })
    ui.SetGeneratedKind("window")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ImageBrowser", "# Image Browser\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: \"Image browser with a thumbnail sidebar and full-size image panel.\"\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_view\n  - gallery_view\n  - gallery_mosaic\n  - image_button\n  - image_list_button\n  - image_list_view\n  - image_show_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Image Browser\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the image browser.\"\n\nrender_label:\n  type: label\n  text: \"Render\"\n  description: \"Label above the render mode selector.\"\n  expose: false\n\nmode_select:\n  type: combobox\n  options: [Normal, Fallback]\n  selected_item: Normal\n  description: \"Switch between Sixel and fallback image rendering.\"\n\nsidebar_title:\n  type: label\n  text: \"Photos\"\n  description: \"Sidebar section label.\"\n  expose: false\n\ncamera_thumb:\n  type: image_button\n  description: \"Camera thumbnail. Click to view full size.\"\n\nastro_thumb:\n  type: image_button\n  description: \"Astronaut thumbnail. Click to view full size.\"\n\ncoffee_thumb:\n  type: image_button\n  description: \"Coffee thumbnail. Click to view full size.\"\n\ngallery_label:\n  type: label\n  text: \"Gallery\"\n  description: \"Label above the gallery mosaic.\"\n  expose: false\n\nimage_list_label:\n  type: label\n  text: \"Image items\"\n  description: \"List preview label.\"\n  expose: false\n\ngallery_mosaic:\n  type: gallery_mosaic\n  description: \"Click any image to open the full scrollable gallery.\"\n\nimage_list_btn:\n  type: image_list_button\n  description: \"Click to open the image item list.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Spacer that absorbs leftover sidebar height.\"\n  expose: false\n\nmain:\n  type: viewhost\n  description: \"Full-size image panel.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: \"#1f2937\"\n  padding: 0\n@sidebar:\n  background: \"#172033\"\n  padding: 0\n@content:\n  background: \"#030712\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@render_label:\n  color: \"#93c5fd\"\n@sidebar_title:\n  color: \"#93c5fd\"\n@gallery_label:\n  color: \"#93c5fd\"\n@image_list_label:\n  color: \"#93c5fd\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+--header-**-------------------------------------------------------------------+\n| title..**....................................................... close_btn.. |\n+--sidebar-------------------+--content-**-------------------------------------+\n| render_label.............. | main..**....................................... |\n* mode_select............... | ............................................... |\n*                            | *.............................................. |\n| sidebar_title............. | *.............................................. |\n| camera_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| astro_thumb............... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| coffee_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| gallery_label............. | ............................................... |\n| gallery_mosaic............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| image_list_label.......... | ............................................... |\n| image_list_btn............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n* spacer.................... | ............................................... |\n* .......................... | ............................................... |\n| .......................... | ............................................... |\n+----------------------------+-------------------------------------------------+\n```\n", "Image browser with a thumbnail sidebar and full-size image panel.")
    ui.Title = uimd.NewLabel("title", "Image Browser")
    ui.AddElement(ui.Title)
    ui.Title.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.Title.SetCursorStyle(uimd.Style{
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
    ui.RenderLabel = uimd.NewLabel("render_label", "Render")
    ui.AddElement(ui.RenderLabel)
    ui.RenderLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.RenderLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ModeSelect = uimd.NewComboBox("mode_select", []string{"Normal", "Fallback"})
    ui.AddElement(ui.ModeSelect)
    ui.ModeSelect.SetSelectedIndex(0)
    ui.ModeSelect.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ModeSelect.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ModeSelect.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ModeSelect.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ModeSelect.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SidebarTitle = uimd.NewLabel("sidebar_title", "Photos")
    ui.AddElement(ui.SidebarTitle)
    ui.SidebarTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.SidebarTitle.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.CameraThumb = uimd.NewReusableElement("camera_thumb", "image_button")
    ui.AddElement(ui.CameraThumb)
    ui.CameraThumb.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.CameraThumb.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.CameraThumb.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AstroThumb = uimd.NewReusableElement("astro_thumb", "image_button")
    ui.AddElement(ui.AstroThumb)
    ui.AstroThumb.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.AstroThumb.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.AstroThumb.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CoffeeThumb = uimd.NewReusableElement("coffee_thumb", "image_button")
    ui.AddElement(ui.CoffeeThumb)
    ui.CoffeeThumb.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.CoffeeThumb.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.CoffeeThumb.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.GalleryLabel = uimd.NewLabel("gallery_label", "Gallery")
    ui.AddElement(ui.GalleryLabel)
    ui.GalleryLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.GalleryLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ImageListLabel = uimd.NewLabel("image_list_label", "Image items")
    ui.AddElement(ui.ImageListLabel)
    ui.ImageListLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.ImageListLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.GalleryMosaic = uimd.NewReusableElement("gallery_mosaic", "gallery_mosaic")
    ui.AddElement(ui.GalleryMosaic)
    ui.GalleryMosaic.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.GalleryMosaic.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.GalleryMosaic.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ImageListBtn = uimd.NewReusableElement("image_list_btn", "image_list_button")
    ui.AddElement(ui.ImageListBtn)
    ui.ImageListBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.ImageListBtn.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.ImageListBtn.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
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
    ui.Main = uimd.NewViewHost("main")
    ui.AddElement(ui.Main)
    ui.Main.SetStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
    })
    return ui
}

func (ui *ImageBrowserUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageBrowserUI) HandleGeneratedButton(name string) bool {
    if name == "close_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnCloseBtnClick() }); ok {
            handler.OnCloseBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ImageBrowserUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ImageBrowserUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageBrowserUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "mode_select" {
        if handler, ok := ui.eventHandler.(interface{ OnModeSelectChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnModeSelectChange(selected)
            return true
        }
        return false
    }
    return false
}

func (ui *ImageBrowserUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ImageBrowserUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
