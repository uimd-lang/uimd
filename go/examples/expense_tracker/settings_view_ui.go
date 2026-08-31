// Auto-generated UI code for settings_view - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type SettingsViewUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    ViewTitle *uimd.Label
    Currency *uimd.ComboBox
    ShowPaid *uimd.CheckBox
    MonthlyLimit *uimd.NumberInput
    ResetBtn *uimd.Button
}

func NewSettingsViewUI() *SettingsViewUI {
    ui := &SettingsViewUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Settings View")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 9,
            CellName: "view", CellWidth: 0, CellHeight: 9,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 58, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 58, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 8,
            Content: "view_title",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#fde68a"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 9,
            CellName: "view", CellWidth: 0, CellHeight: 9,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 48, MarginBottom: 6,
            Content: "\"Currency:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 2, Col: 18,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 9,
            CellName: "view", CellWidth: 0, CellHeight: 9,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 41, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 41, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 6,
            Content: "currency",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 4, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 9,
            CellName: "view", CellWidth: 0, CellHeight: 9,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 58, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 58, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 4,
            Content: "show_paid",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#b8c4d8"),
    },
        },
        {
            Row: 6, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 9,
            CellName: "view", CellWidth: 0, CellHeight: 9,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 16, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 16, CharsHeight: 1,
            MarginRight: 43, MarginBottom: 2,
            Content: "\"Monthly limit:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 6, Col: 18,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 9,
            CellName: "view", CellWidth: 0, CellHeight: 9,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 41, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 41, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 2,
            Content: "monthly_limit",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 8, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 9,
            CellName: "view", CellWidth: 0, CellHeight: 9,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 58, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 58, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "reset_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
    })
    ui.SetGeneratedKind("control")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "SettingsView", "# Settings View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Expense tracker settings view.\"\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Settings\n\ncurrency:\n  type: combobox\n  options: [EUR, USD, CZK]\n  selected_item: EUR\n\nshow_paid:\n  type: checkbox\n  title: Show paid expenses\n  value: true\n\nmonthly_limit:\n  type: numberinput\n  value: 500\n  format: \"0\"\n\nreset_btn:\n  type: button\n  title: Reset demo data\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#fde68a\"\nnumberinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Currency:\"      currency................................. |\n|                                                            |\n| show_paid................................................. |\n|                                                            |\n| \"Monthly limit:\" monthly_limit............................ |\n|                                                            |\n| reset_btn................................................. |\n+------------------------------------------------------------+\n```\n", "Expense tracker settings view.")
    ui.ViewTitle = uimd.NewLabel("view_title", "Settings")
    ui.AddElement(ui.ViewTitle)
    ui.ViewTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#fde68a"),
    })
    ui.ViewTitle.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Currency = uimd.NewComboBox("currency", []string{"EUR", "USD", "CZK"})
    ui.AddElement(ui.Currency)
    ui.Currency.SetSelectedIndex(0)
    ui.Currency.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Currency.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Currency.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Currency.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Currency.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ShowPaid = uimd.NewCheckBox("show_paid", "Show paid expenses", true)
    ui.AddElement(ui.ShowPaid)
    ui.ShowPaid.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#b8c4d8"),
    })
    ui.ShowPaid.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ShowPaid.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#44cc88"),
    })
    ui.ShowPaid.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
    })
    ui.ShowPaid.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.MonthlyLimit = uimd.NewNumberInput("monthly_limit", 500.000000)
    ui.AddElement(ui.MonthlyLimit)
    ui.MonthlyLimit.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.MonthlyLimit.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.MonthlyLimit.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.MonthlyLimit.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.MonthlyLimit.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ResetBtn = uimd.NewButton("reset_btn", "Reset demo data")
    ui.AddElement(ui.ResetBtn)
    ui.ResetBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ResetBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ResetBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *SettingsViewUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *SettingsViewUI) HandleGeneratedButton(name string) bool {
    if name == "reset_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnResetBtnClick() }); ok {
            handler.OnResetBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *SettingsViewUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "show_paid" {
        if handler, ok := ui.eventHandler.(interface{ OnShowPaidChange(string) }); ok {
            handler.OnShowPaidChange(value)
            return true
        }
        return false
    }
    if name == "monthly_limit" {
        if handler, ok := ui.eventHandler.(interface{ OnMonthlyLimitChange(string) }); ok {
            handler.OnMonthlyLimitChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *SettingsViewUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "monthly_limit" {
        if handler, ok := ui.eventHandler.(interface{ OnMonthlyLimitSubmit(string) }); ok {
            handler.OnMonthlyLimitSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *SettingsViewUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "currency" {
        if handler, ok := ui.eventHandler.(interface{ OnCurrencyChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnCurrencyChange(selected)
            return true
        }
        return false
    }
    return false
}

func (ui *SettingsViewUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *SettingsViewUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
