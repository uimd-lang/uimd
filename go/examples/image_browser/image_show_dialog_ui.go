// Auto-generated UI code for image_show_dialog - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageShowDialogUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Title *uimd.Label
    Photo *uimd.Image
    CloseBtn *uimd.Button
}

func NewImageShowDialogUI() *ImageShowDialogUI {
    ui := &ImageShowDialogUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Image Show Dialog")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 22, CellCol: 0,
            CellCharsWidth: 34, CellCharsHeight: 1,
            CellName: "", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 34, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 0, Col: 0,
            CellRow: 22, CellCol: 57,
            CellCharsWidth: 33, CellCharsHeight: 1,
            CellName: "", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 33, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 21,
            CellName: "dialog", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 88, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 20,
            Content: "title",
            CellStyle: uimd.Style{
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 2,
            CellRow: 22, CellCol: 35,
            CellCharsWidth: 21, CellCharsHeight: 1,
            CellName: "", CellWidth: 21, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 17, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 0,
            Content: "close_btn",
            CellStyle: uimd.Style{
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 21,
            CellName: "dialog", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 88, CharsHeight: 20,
            MarginRight: 1, MarginBottom: 0,
            Content: "photo",
            CellStyle: uimd.Style{
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
    })
    ui.SetGeneratedKind("dialog")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#081426"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(1),
        BorderWidthVertical: uimd.Int(1),
        MarginBottom: uimd.Int(4),
        MarginLeft: uimd.Int(6),
        MarginRight: uimd.Int(6),
        MarginTop: uimd.Int(4),
    })
    ui.SetMCPMetadata(true, "ImageShowDialog", "# Image Show Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\ndescription: \"Dialog for showing a selected image.\"\ntags: [example, image, dialog]\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Image\"\n  description: \"Dialog title.\"\n  expose: false\n\nphoto:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Selected image preview.\"\n\nclose_btn:\n  type: button\n  title: Close\n  description: \"Close the dialog.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 1\n  background: \"#081426\"\n  margin: 4, 6, 4, 6\ncell:\n  padding: 1, 1, 1, 1\n@title:\n  color: \"#f8fafc\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-dialog-**--------------------------------------------------------------------------------+\n| title..**............................................................................... |\n| photo..**............................................................................... |\n* ........................................................................................ |\n* *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+-**-------------------------------+---------------------+-**------------------------------+\n|                                  |  close_btn........  |                                 |\n+----------------------------------+---------------------+---------------------------------+\n```\n", "Dialog for showing a selected image.")
    ui.Title = uimd.NewLabel("title", "Image")
    ui.AddElement(ui.Title)
    ui.Title.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        TextAlign: "center",
    })
    ui.Title.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Photo = uimd.NewImage("photo", "", "", "contain", "auto", "center", "middle")
    ui.AddElement(ui.Photo)
    ui.Photo.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.CloseBtn = uimd.NewButton("close_btn", "Close")
    ui.AddElement(ui.CloseBtn)
    ui.CloseBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.CloseBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CloseBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *ImageShowDialogUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageShowDialogUI) HandleGeneratedButton(name string) bool {
    if name == "photo" {
        if handler, ok := ui.eventHandler.(interface{ OnPhotoClick() }); ok {
            handler.OnPhotoClick()
            return true
        }
        return false
    }
    if name == "close_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnCloseBtnClick() }); ok {
            handler.OnCloseBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ImageShowDialogUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ImageShowDialogUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageShowDialogUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
