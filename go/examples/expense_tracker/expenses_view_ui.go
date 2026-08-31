// Auto-generated UI code for expenses_view - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ExpensesViewUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    ViewTitle *uimd.Label
    ItemInput *uimd.TextInput
    AmountInput *uimd.NumberInput
    CategoryInput *uimd.ComboBox
    CurrencyLabel *uimd.Label
    AddExpenseBtn *uimd.Button
    Expenses *uimd.ReusableElement
}

func NewExpensesViewUI() *ExpensesViewUI {
    ui := &ExpensesViewUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Expenses View")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 58, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 58, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 14,
            Content: "view_title",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 1,
            MarginRight: 52, MarginBottom: 12,
            Content: "\"Item:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 2, Col: 13,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 46, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 46, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 12,
            Content: "item_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 4, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 9, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 9, CharsHeight: 1,
            MarginRight: 50, MarginBottom: 10,
            Content: "\"Amount:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 4, Col: 13,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 25, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 25, CharsHeight: 1,
            MarginRight: 22, MarginBottom: 10,
            Content: "amount_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 4, Col: 39,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 20, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 10,
            Content: "currency_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
    },
        },
        {
            Row: 6, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 48, MarginBottom: 8,
            Content: "\"Category:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 6, Col: 13,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 46, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 46, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 8,
            Content: "category_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 8, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 58, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 58, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 6,
            Content: "add_expense_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 10, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 15,
            CellName: "view", CellWidth: 0, CellHeight: 15,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 58, CharsHeight: 5,
            MarginRight: 1, MarginBottom: 0,
            Content: "expenses",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
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
    ui.SetMCPMetadata(true, "ExpensesView", "# Expenses View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Expense entry view with a dynamic scrollable list.\"\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - expenses_list/expenses_list\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Expenses\n\nitem_input:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n\namount_input:\n  type: numberinput\n  value: 0\n  format: \"0.00\"\n\ncategory_input:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ncurrency_label:\n  type: label\n  text: EUR\n\nadd_expense_btn:\n  type: button\n  title: Add expense\n\nexpenses:\n  type: expenses_list\n  cpp-class: expense_tracker_example::ExpensesList\n  cpp-header: \"expenses_list/expenses_list.hpp\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#bfdbfe\"\n@currency_label:\n  color: \"#bfdbfe\"\n@expenses:\n  background: \"#172033\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nnumberinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Item:\"     item_input.................................... |\n|                                                            |\n| \"Amount:\"   amount_input............. currency_label...... |\n|                                                            |\n| \"Category:\" category_input................................ |\n|                                                            |\n| add_expense_btn........................................... |\n|                                                            |\n| expenses..**.............................................. |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n", "Expense entry view with a dynamic scrollable list.")
    ui.ViewTitle = uimd.NewLabel("view_title", "Expenses")
    ui.AddElement(ui.ViewTitle)
    ui.ViewTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
    })
    ui.ViewTitle.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ItemInput = uimd.NewTextInput("item_input", "", 0)
    ui.AddElement(ui.ItemInput)
    ui.ItemInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ItemInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ItemInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ItemInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.ItemInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ItemInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.AmountInput = uimd.NewNumberInput("amount_input", 0.000000)
    ui.AddElement(ui.AmountInput)
    ui.AmountInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.AmountInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AmountInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AmountInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.AmountInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CategoryInput = uimd.NewComboBox("category_input", []string{"Food", "Transport", "Home", "Health", "Other"})
    ui.AddElement(ui.CategoryInput)
    ui.CategoryInput.SetSelectedIndex(0)
    ui.CategoryInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.CategoryInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CategoryInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.CategoryInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CategoryInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CurrencyLabel = uimd.NewLabel("currency_label", "EUR")
    ui.AddElement(ui.CurrencyLabel)
    ui.CurrencyLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
    })
    ui.CurrencyLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.AddExpenseBtn = uimd.NewButton("add_expense_btn", "Add expense")
    ui.AddElement(ui.AddExpenseBtn)
    ui.AddExpenseBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.AddExpenseBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AddExpenseBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Expenses = uimd.NewReusableElement("expenses", "expenses_list")
    ui.AddElement(ui.Expenses)
    ui.Expenses.SetStyle(uimd.Style{
        Background: uimd.NewColor("#172033"),
    })
    ui.Expenses.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.Expenses.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.Expenses.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    return ui
}

func (ui *ExpensesViewUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ExpensesViewUI) HandleGeneratedButton(name string) bool {
    if name == "add_expense_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnAddExpenseBtnClick() }); ok {
            handler.OnAddExpenseBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ExpensesViewUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "item_input" {
        if handler, ok := ui.eventHandler.(interface{ OnItemInputChange(string) }); ok {
            handler.OnItemInputChange(value)
            return true
        }
        return false
    }
    if name == "amount_input" {
        if handler, ok := ui.eventHandler.(interface{ OnAmountInputChange(string) }); ok {
            handler.OnAmountInputChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ExpensesViewUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "item_input" {
        if handler, ok := ui.eventHandler.(interface{ OnItemInputSubmit(string) }); ok {
            handler.OnItemInputSubmit(value)
            return true
        }
        return false
    }
    if name == "amount_input" {
        if handler, ok := ui.eventHandler.(interface{ OnAmountInputSubmit(string) }); ok {
            handler.OnAmountInputSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ExpensesViewUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "category_input" {
        if handler, ok := ui.eventHandler.(interface{ OnCategoryInputChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnCategoryInputChange(selected)
            return true
        }
        return false
    }
    return false
}

func (ui *ExpensesViewUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ExpensesViewUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
