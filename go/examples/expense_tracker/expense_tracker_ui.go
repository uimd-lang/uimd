// Auto-generated UI code for expense_tracker - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ExpenseTrackerUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    AppTitle *uimd.Label
    QuitBtn *uimd.Button
    NavTitle *uimd.Label
    ExpensesBtn *uimd.Button
    CategoriesBtn *uimd.Button
    SettingsBtn *uimd.Button
    Main *uimd.ViewHost
}

func NewExpenseTrackerUI() *ExpenseTrackerUI {
    ui := &ExpenseTrackerUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Expense Tracker")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 53, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 13, MarginBottom: 0,
            Content: "app_title",
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
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 24,
            CellCharsWidth: 29, CellCharsHeight: 9,
            CellName: "content", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 27, CharsHeight: 9,
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
            CellCharsWidth: 23, CellCharsHeight: 9,
            CellName: "nav", CellWidth: 23, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 21, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 8,
            Content: "nav_title",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
    },
        },
        {
            Row: 0, Col: 41,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 53, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "quit_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Padding: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 23, CellCharsHeight: 9,
            CellName: "nav", CellWidth: 23, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 21, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 6,
            Content: "expenses_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 4, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 23, CellCharsHeight: 9,
            CellName: "nav", CellWidth: 23, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 21, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 4,
            Content: "categories_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 6, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 23, CellCharsHeight: 9,
            CellName: "nav", CellWidth: 23, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 21, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 2,
            Content: "settings_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
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
    ui.SetMCPMetadata(true, "ExpenseTracker", "# Expense Tracker\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: experimental\ndescription: \"Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.\"\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nuses:\n  - expenses_view\n  - categories_view\n  - settings_view\n```\n\n## Members\n\n```yaml\napp_title:\n  type: label\n  text: \"Expense Tracker\"\n\nquit_btn:\n  type: button\n  title: Quit\n\nnav_title:\n  type: label\n  text: Expenses\n\nexpenses_btn:\n  type: button\n  title: Expenses\n\ncategories_btn:\n  type: button\n  title: Categories\n\nsettings_btn:\n  type: button\n  title: Settings\n\nmain:\n  type: viewhost\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: \"#1f2937\"\n  padding: 0\n@app_title:\n  color: \"#ffffff\"\n  text-align: center\n@nav:\n  background: \"#172033\"\n@content:\n  background: \"#030712\"\n@nav_title:\n  color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+--header-**------------------------------------------+\n| app_title..**.......................... quit_btn... |\n+--nav------------------+--content-**-----------------+\n| nav_title............ | main..**................... |\n|                       | *.......................... |\n| expenses_btn......... | *.......................... |\n|                       | ........................... |\n| categories_btn....... | ........................... |\n|                       | ........................... |\n| settings_btn......... | ........................... |\n*                       * ........................... |\n*                       * ........................... |\n+-----------------------+-----------------------------+\n```\n", "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.")
    ui.AppTitle = uimd.NewLabel("app_title", "Expense Tracker")
    ui.AddElement(ui.AppTitle)
    ui.AppTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.AppTitle.SetCursorStyle(uimd.Style{
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
    ui.NavTitle = uimd.NewLabel("nav_title", "Expenses")
    ui.AddElement(ui.NavTitle)
    ui.NavTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NavTitle.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ExpensesBtn = uimd.NewButton("expenses_btn", "Expenses")
    ui.AddElement(ui.ExpensesBtn)
    ui.ExpensesBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ExpensesBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ExpensesBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CategoriesBtn = uimd.NewButton("categories_btn", "Categories")
    ui.AddElement(ui.CategoriesBtn)
    ui.CategoriesBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.CategoriesBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CategoriesBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SettingsBtn = uimd.NewButton("settings_btn", "Settings")
    ui.AddElement(ui.SettingsBtn)
    ui.SettingsBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SettingsBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SettingsBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Main = uimd.NewViewHost("main")
    ui.AddElement(ui.Main)
    ui.Main.SetStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
    })
    return ui
}

func (ui *ExpenseTrackerUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ExpenseTrackerUI) HandleGeneratedButton(name string) bool {
    if name == "quit_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnQuitBtnClick() }); ok {
            handler.OnQuitBtnClick()
            return true
        }
        return false
    }
    if name == "expenses_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnExpensesBtnClick() }); ok {
            handler.OnExpensesBtnClick()
            return true
        }
        return false
    }
    if name == "categories_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnCategoriesBtnClick() }); ok {
            handler.OnCategoriesBtnClick()
            return true
        }
        return false
    }
    if name == "settings_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnSettingsBtnClick() }); ok {
            handler.OnSettingsBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ExpenseTrackerUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ExpenseTrackerUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ExpenseTrackerUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
