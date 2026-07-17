// Auto-generated UI code for stale_scrollview_focus - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type StaleScrollviewFocusUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Title *uimd.Label
    QuitBtn *uimd.Button
    TabList *uimd.Button
    TabOther *uimd.Button
    Hint *uimd.Label
    Page *uimd.ViewHost
}

func NewStaleScrollviewFocusUI() *StaleScrollviewFocusUI {
    ui := &StaleScrollviewFocusUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Stale ScrollView Focus")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 4, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 29,
            CellName: "content", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 88, CharsHeight: 29,
            MarginRight: 1, MarginBottom: 0,
            Content: "page",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 1,
            CellName: "nav", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 16, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 16, CharsHeight: 1,
            MarginRight: 73, MarginBottom: 0,
            Content: "tab_list",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#2f80ed"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 75, CharsHeight: 1,
            MarginRight: 14, MarginBottom: 0,
            Content: "title",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 18,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 1,
            CellName: "nav", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 17, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 17, CharsHeight: 1,
            MarginRight: 55, MarginBottom: 0,
            Content: "tab_other",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#343b4b"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 36,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 1,
            CellName: "nav", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 53, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "hint",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 0, Col: 77,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "quit_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
    },
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
    ui.SetMCPMetadata(true, "StaleScrollviewFocus", "# Stale ScrollView Focus\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button.\"\ntags: [regression, issue-5, viewhost, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - list_page\n  - edit_page\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Scroll Focus Repro\"\n  description: \"Regression window title.\"\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the regression app.\"\n\ntab_list:\n  type: button\n  title: List\n  description: \"Show the list page.\"\n\ntab_other:\n  type: button\n  title: Other\n  description: \"Inactive top-level navigation button used to expose stale focus.\"\n\nhint:\n  type: label\n  text: \"Focus list, press Enter, choose Open.\"\n  description: \"Manual reproduction hint.\"\n  expose: false\n\npage:\n  type: viewhost\n  description: \"Dynamic page host.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@nav:\n  background: \"#172033\"\n@content:\n  background: \"#030712\"\n@hint:\n  color: \"#93c5fd\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@tab_list:\n  background: \"#2f80ed\"\n  focus-background: \"#22c55e\"\n@tab_other:\n  background: \"#343b4b\"\n  focus-background: \"#9333ea\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------------------+\n| title..**.................................................................. quit_btn.... |\n+-nav-**-----------------------------------------------------------------------------------+\n| tab_list........ tab_other........ hint..**............................................. |\n+-content-**-------------------------------------------------------------------------------+\n| page..**................................................................................ |\n* *....................................................................................... |\n* *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+------------------------------------------------------------------------------------------+\n```\n", "Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button.")
    ui.Title = uimd.NewLabel("title", "Scroll Focus Repro")
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
    ui.TabList = uimd.NewButton("tab_list", "List")
    ui.AddElement(ui.TabList)
    ui.TabList.SetStyle(uimd.Style{
        Background: uimd.NewColor("#2f80ed"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.TabList.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#22c55e"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.TabList.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.TabOther = uimd.NewButton("tab_other", "Other")
    ui.AddElement(ui.TabOther)
    ui.TabOther.SetStyle(uimd.Style{
        Background: uimd.NewColor("#343b4b"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.TabOther.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#9333ea"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.TabOther.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Hint = uimd.NewLabel("hint", "Focus list, press Enter, choose Open.")
    ui.AddElement(ui.Hint)
    ui.Hint.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.Hint.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Page = uimd.NewViewHost("page")
    ui.AddElement(ui.Page)
    ui.Page.SetStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
    })
    return ui
}

func (ui *StaleScrollviewFocusUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *StaleScrollviewFocusUI) HandleGeneratedButton(name string) bool {
    if name == "quit_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnQuitBtnClick() }); ok {
            handler.OnQuitBtnClick()
            return true
        }
        return false
    }
    if name == "tab_list" {
        if handler, ok := ui.eventHandler.(interface{ OnTabListClick() }); ok {
            handler.OnTabListClick()
            return true
        }
        return false
    }
    if name == "tab_other" {
        if handler, ok := ui.eventHandler.(interface{ OnTabOtherClick() }); ok {
            handler.OnTabOtherClick()
            return true
        }
        return false
    }
    return false
}

func (ui *StaleScrollviewFocusUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *StaleScrollviewFocusUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *StaleScrollviewFocusUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
