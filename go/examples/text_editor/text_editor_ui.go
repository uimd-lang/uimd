// Auto-generated UI code for text_editor - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type TextEditorUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    WindowHeader *uimd.Label
    Filename *uimd.Label
    Editor *uimd.TextArea
    NewBtn *uimd.Button
    OpenBtn *uimd.Button
    SaveBtn *uimd.Button
    SaveAsBtn *uimd.Button
    QuitBtn *uimd.Button
    Status *uimd.Label
}

func NewTextEditorUI() *TextEditorUI {
    ui := &TextEditorUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Text Editor")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 1,
            CellName: "title", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 87, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "window_header",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#1d314c"),
        Color: uimd.NewColor("#cbd5e1"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 18, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 1,
            CellName: "", CellWidth: 87, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 85, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 85, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "status",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 4,
            CellName: "filebar", CellWidth: 87, CellHeight: 4,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 6, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 6, CharsHeight: 1,
            MarginRight: 80, MarginBottom: 2,
            Content: "\"file\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 7, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 10,
            CellName: "main", CellWidth: 87, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 86, CharsHeight: 9,
            MarginRight: 0, MarginBottom: 0,
            Content: "editor",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 1, Col: 8,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 4,
            CellName: "filebar", CellWidth: 87, CellHeight: 4,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 78, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 2,
            Content: "filename",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
    },
        },
        {
            Row: 3, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 4,
            CellName: "filebar", CellWidth: 87, CellHeight: 4,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 14, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 72, MarginBottom: 0,
            Content: "new_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 16,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 4,
            CellName: "filebar", CellWidth: 87, CellHeight: 4,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 15, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 15, CharsHeight: 1,
            MarginRight: 56, MarginBottom: 0,
            Content: "open_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 32,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 4,
            CellName: "filebar", CellWidth: 87, CellHeight: 4,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 15, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 15, CharsHeight: 1,
            MarginRight: 40, MarginBottom: 0,
            Content: "save_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 48,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 4,
            CellName: "filebar", CellWidth: 87, CellHeight: 4,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 18, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 18, CharsHeight: 1,
            MarginRight: 21, MarginBottom: 0,
            Content: "save_as_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 67,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 87, CellCharsHeight: 4,
            CellName: "filebar", CellWidth: 87, CellHeight: 4,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 15, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 15, CharsHeight: 1,
            MarginRight: 5, MarginBottom: 0,
            Content: "quit_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
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
    ui.SetMCPMetadata(true, "TextEditor", "# Text Editor\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - ../../dialogs/file_browser\n```\n\n## Members\n\n```yaml\nwindow_header:\n  type: label\n  text: \"Text Editor\"\n  description: \"Application title.\"\n  expose: false\n\nfilename:\n  type: label\n  text: welcome.txt\n  description: \"Current file path or file name.\"\n\neditor:\n  type: textarea\n  value: \"\"\n  description: \"Main multiline text editing area.\"\n\nnew_btn:\n  type: button\n  title: New\n  description: \"Create a new empty document, asking about unsaved changes when needed.\"\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open the file browser to load a text file, asking about unsaved changes when needed.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Save the current document to its current path.\"\n\nsave_as_btn:\n  type: button\n  title: \"Save As\"\n  description: \"Open the file browser in save mode and save the document to a chosen path.\"\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the editor, asking about unsaved changes when needed.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows load, save, cancel, and error messages.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@filename:\n  color: \"#bfdbfe\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@status:\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-title-**------------------------------------------------------------------------------+\n|window_header..**......................................................................|\n+-filebar-------------------------------------------------------------------------------+\n|                                                                                       |\n| \"file\" filename..**.................................................................. |\n|                                                                                       |\n| new_btn....... open_btn....... save_btn....... save_as_btn....... quit_btn.......     |\n+-main----------------------------------------------------------------------------------+\n|                                                                                       |\n| editor..**............................................................................|\n| ..................................................................................... |\n* *.................................................................................... |\n* *.................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n+---------------------------------------------------------------------------------------+\n| status............................................................................... |\n+---------------------------------------------------------------------------------------+\n```\n", "Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.")
    ui.WindowHeader = uimd.NewLabel("window_header", "Text Editor")
    ui.AddElement(ui.WindowHeader)
    ui.WindowHeader.SetStyle(uimd.Style{
        Background: uimd.NewColor("#1d314c"),
        Color: uimd.NewColor("#cbd5e1"),
        TextAlign: "center",
    })
    ui.WindowHeader.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Filename = uimd.NewLabel("filename", "welcome.txt")
    ui.AddElement(ui.Filename)
    ui.Filename.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
    })
    ui.Filename.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Editor = uimd.NewTextArea("editor", "")
    ui.AddElement(ui.Editor)
    ui.Editor.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Editor.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Editor.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Editor.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff"),
        Color: uimd.NewColor("#0e1117"),
    })
    ui.Editor.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Editor.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.NewBtn = uimd.NewButton("new_btn", "New")
    ui.AddElement(ui.NewBtn)
    ui.NewBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.NewBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NewBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.OpenBtn = uimd.NewButton("open_btn", "Open")
    ui.AddElement(ui.OpenBtn)
    ui.OpenBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.OpenBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.OpenBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SaveBtn = uimd.NewButton("save_btn", "Save")
    ui.AddElement(ui.SaveBtn)
    ui.SaveBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SaveBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SaveBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SaveAsBtn = uimd.NewButton("save_as_btn", "Save As")
    ui.AddElement(ui.SaveAsBtn)
    ui.SaveAsBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SaveAsBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SaveAsBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.QuitBtn = uimd.NewButton("quit_btn", "Quit")
    ui.AddElement(ui.QuitBtn)
    ui.QuitBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.QuitBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.QuitBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Status = uimd.NewLabel("status", "Ready")
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

func (ui *TextEditorUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TextEditorUI) HandleGeneratedButton(name string) bool {
    if name == "new_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnNewBtnClick() }); ok {
            handler.OnNewBtnClick()
            return true
        }
        return false
    }
    if name == "open_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnOpenBtnClick() }); ok {
            handler.OnOpenBtnClick()
            return true
        }
        return false
    }
    if name == "save_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnSaveBtnClick() }); ok {
            handler.OnSaveBtnClick()
            return true
        }
        return false
    }
    if name == "save_as_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnSaveAsBtnClick() }); ok {
            handler.OnSaveAsBtnClick()
            return true
        }
        return false
    }
    if name == "quit_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnQuitBtnClick() }); ok {
            handler.OnQuitBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *TextEditorUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "editor" {
        if handler, ok := ui.eventHandler.(interface{ OnEditorChange(string) }); ok {
            handler.OnEditorChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *TextEditorUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "editor" {
        if handler, ok := ui.eventHandler.(interface{ OnEditorSubmit(string) }); ok {
            handler.OnEditorSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *TextEditorUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
