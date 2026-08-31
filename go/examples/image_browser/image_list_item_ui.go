// Auto-generated UI code for image_list_item - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ImageListItemUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Sel *uimd.CheckBox
    Thumb *uimd.Image
    NameLabel *uimd.Label
    PathLabel *uimd.Label
    Spacer *uimd.Label
    ShowBtn *uimd.Button
    BrowseBtn *uimd.Button
    DeleteBtn *uimd.Button
}

func NewImageListItemUI() *ImageListItemUI {
    ui := &ImageListItemUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Image List Item")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 1, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 7,
            CellName: "card", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 3, Height: 5,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 5,
            MarginRight: 66, MarginBottom: 1,
            Content: "sel",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 6,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 7,
            CellName: "card", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 8, Height: 5,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 8, CharsHeight: 5,
            MarginRight: 57, MarginBottom: 1,
            Content: "thumb",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 1, Col: 16,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 7,
            CellName: "card", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 2,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 16, CharsHeight: 2,
            MarginRight: 39, MarginBottom: 4,
            Content: "name_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        UserSelect: "text",
    },
        },
        {
            Row: 1, Col: 34,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 7,
            CellName: "card", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 11, Height: 5,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 5,
            MarginRight: 26, MarginBottom: 1,
            Content: "show_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 46,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 7,
            CellName: "card", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 11, Height: 5,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 5,
            MarginRight: 14, MarginBottom: 1,
            Content: "browse_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 58,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 7,
            CellName: "card", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 11, Height: 5,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 5,
            MarginRight: 2, MarginBottom: 1,
            Content: "delete_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 16,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 7,
            CellName: "card", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 2,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 53, CharsHeight: 2,
            MarginRight: 2, MarginBottom: 2,
            Content: "path_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#64748b"),
        UserSelect: "text",
    },
        },
        {
            Row: 5, Col: 16,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 71, CellCharsHeight: 7,
            CellName: "card", CellWidth: 0, CellHeight: 7,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 53, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 1,
            Content: "spacer",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(0),
        PaddingRight: uimd.Int(0),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ImageListItem", "# Image List Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\ndescription: \"Reusable row for one image item.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nsel:\n  type: checkbox\n  title: \"\"\n  value: false\n  description: \"Select this image item.\"\n\nthumb:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Small image thumbnail.\"\n\nname_label:\n  type: label\n  text: \"\"\n  description: \"Image file name.\"\n\npath_label:\n  type: label\n  text: \"\"\n  description: \"Full image path.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible spacer before item actions.\"\n  expose: false\n\nshow_btn:\n  type: button\n  title: Show\n  description: \"Show this image in a dialog.\"\n\nbrowse_btn:\n  type: button\n  title: Browse\n  description: \"Choose a replacement image.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Delete this image item after confirmation.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 0, 0, 0, 0\n@name_label:\n  color: \"#f8fafc\"\n  user-select: text\n@path_label:\n  color: \"#64748b\"\n  user-select: text\ncheckbox:\n  color: \"#cbd5e1\"\n  focus-color: \"#facc15\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@show_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------------------------------+\n|                                                                       |\n|  sel thumb...  name_label..**..  show_btn... browse_btn. delete_btn.  |\n|  ... ........  .....................................................  |\n|  ... ........  path_label..**.......................................  |\n|  ... ........  .....................................................  |\n|  ... ........  spacer..**...........................................  |\n|                                                                       |\n+-----------------------------------------------------------------------+\n```\n", "Reusable row for one image item.")
    ui.Sel = uimd.NewCheckBox("sel", "", false)
    ui.AddElement(ui.Sel)
    ui.Sel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Sel.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#facc15"),
    })
    ui.Sel.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#22c55e"),
    })
    ui.Sel.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#94a3b8"),
    })
    ui.Sel.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.Thumb = uimd.NewImage("thumb", "", "", "contain", "auto", "center", "middle")
    ui.AddElement(ui.Thumb)
    ui.Thumb.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.NameLabel = uimd.NewLabel("name_label", "")
    ui.AddElement(ui.NameLabel)
    ui.NameLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        UserSelect: "text",
    })
    ui.NameLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.PathLabel = uimd.NewLabel("path_label", "")
    ui.AddElement(ui.PathLabel)
    ui.PathLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#64748b"),
        UserSelect: "text",
    })
    ui.PathLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
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
    ui.ShowBtn = uimd.NewButton("show_btn", "Show")
    ui.AddElement(ui.ShowBtn)
    ui.ShowBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ShowBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#16a34a"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ShowBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.BrowseBtn = uimd.NewButton("browse_btn", "Browse")
    ui.AddElement(ui.BrowseBtn)
    ui.BrowseBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.BrowseBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.BrowseBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.DeleteBtn = uimd.NewButton("delete_btn", "Delete")
    ui.AddElement(ui.DeleteBtn)
    ui.DeleteBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.DeleteBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DeleteBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *ImageListItemUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ImageListItemUI) HandleGeneratedButton(name string) bool {
    if name == "thumb" {
        if handler, ok := ui.eventHandler.(interface{ OnThumbClick() }); ok {
            handler.OnThumbClick()
            return true
        }
        return false
    }
    if name == "show_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnShowBtnClick() }); ok {
            handler.OnShowBtnClick()
            return true
        }
        return false
    }
    if name == "browse_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnBrowseBtnClick() }); ok {
            handler.OnBrowseBtnClick()
            return true
        }
        return false
    }
    if name == "delete_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnDeleteBtnClick() }); ok {
            handler.OnDeleteBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ImageListItemUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "sel" {
        if handler, ok := ui.eventHandler.(interface{ OnSelChange(string) }); ok {
            handler.OnSelChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ImageListItemUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ImageListItemUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *ImageListItemUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ImageListItemUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
