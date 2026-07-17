// Auto-generated UI code for hello - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type HelloUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Headline *uimd.Label
    Name *uimd.TextInput
    HelloButton *uimd.Button
    QuitButton *uimd.Button
}

func NewHelloUI() *HelloUI {
    ui := &HelloUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Hello")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 41, CellCharsHeight: 1,
            CellName: "title", CellWidth: 41, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "headline",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
    },
        },
        {
            Row: 1, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 41, CellCharsHeight: 5,
            CellName: "", CellWidth: 41, CellHeight: 5,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 6, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 6, CharsHeight: 1,
            MarginRight: 33, MarginBottom: 3,
            Content: "\"Name\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 14,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 41, CellCharsHeight: 5,
            CellName: "", CellWidth: 41, CellHeight: 5,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 24, CharsHeight: 1,
            MarginRight: 3, MarginBottom: 3,
            Content: "name",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 41, CellCharsHeight: 5,
            CellName: "", CellWidth: 41, CellHeight: 5,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 19, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 19, CharsHeight: 1,
            MarginRight: 20, MarginBottom: 1,
            Content: "hello_button",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 23,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 41, CellCharsHeight: 5,
            CellName: "", CellWidth: 41, CellHeight: 5,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 16, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 1,
            Content: "quit_button",
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
    ui.SetMCPMetadata(true, "Hello", "# Hello\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Hello world UIMD application.\"\n```\n\n## Members\n\n```yaml\nheadline:\n  type: label\n  text: \"Hello from UIMD\"\n  description: \"Application title.\"\n\nname:\n  type: textinput\n  value: \"\"\n  placeholder: \"Your name\"\n  description: \"Name to greet.\"\n\nhello_button:\n  type: button\n  title: \"Say Hello\"\n  description: \"Create the greeting.\"\n\nquit_button:\n  type: button\n  title: \"Quit\"\n  description: \"Close the application.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\n@title:\n  background: \"#1f2937\"\n@headline:\n  color: \"#ffffff\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-title-----------------------------------+\n| headline............................... |\n+-----------------------------------------+\n|                                         |\n|  \"Name\"      name....................   |\n|                                         |\n|  hello_button.......  quit_button.....  |\n|                                         |\n+-----------------------------------------+\n```\n", "Hello world UIMD application.")
    ui.Headline = uimd.NewLabel("headline", "Hello from UIMD")
    ui.AddElement(ui.Headline)
    ui.Headline.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Headline.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Name = uimd.NewTextInput("name", "", 0)
    ui.AddElement(ui.Name)
    ui.Name.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Name.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Name.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Name.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff"),
        Color: uimd.NewColor("#0e1117"),
    })
    ui.Name.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Name.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.HelloButton = uimd.NewButton("hello_button", "Say Hello")
    ui.AddElement(ui.HelloButton)
    ui.HelloButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.HelloButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.HelloButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.QuitButton = uimd.NewButton("quit_button", "Quit")
    ui.AddElement(ui.QuitButton)
    ui.QuitButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.QuitButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.QuitButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *HelloUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *HelloUI) HandleGeneratedButton(name string) bool {
    if name == "hello_button" {
        if handler, ok := ui.eventHandler.(interface{ OnHelloButtonClick() }); ok {
            handler.OnHelloButtonClick()
            return true
        }
        return false
    }
    if name == "quit_button" {
        if handler, ok := ui.eventHandler.(interface{ OnQuitButtonClick() }); ok {
            handler.OnQuitButtonClick()
            return true
        }
        return false
    }
    return false
}

func (ui *HelloUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "name" {
        if handler, ok := ui.eventHandler.(interface{ OnNameChange(string) }); ok {
            handler.OnNameChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *HelloUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "name" {
        if handler, ok := ui.eventHandler.(interface{ OnNameSubmit(string) }); ok {
            handler.OnNameSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *HelloUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
