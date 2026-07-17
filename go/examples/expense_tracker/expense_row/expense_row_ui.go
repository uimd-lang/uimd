// Auto-generated UI code for expense_row - DO NOT EDIT MANUALLY.
package expense_row

import uimd "uimd"

type ExpenseRowUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Paid *uimd.CheckBox
    Item *uimd.Label
    Amount *uimd.Label
    Category *uimd.ComboBox
    DeleteBtn *uimd.Button
}

func NewExpenseRowUI() *ExpenseRowUI {
    ui := &ExpenseRowUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Expense Row")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 2,
            CellName: "row", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 9, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 9, CharsHeight: 1,
            MarginRight: 44, MarginBottom: 1,
            Content: "paid",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 0, Col: 11,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 2,
            CellName: "row", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 29, CharsHeight: 2,
            MarginRight: 14, MarginBottom: 0,
            Content: "item",
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
            CellCharsWidth: 54, CellCharsHeight: 2,
            CellName: "row", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 1,
            Content: "amount",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 2,
            CellName: "row", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 14, MarginBottom: 0,
            Content: "category",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 1, Col: 41,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 2,
            CellName: "row", CellWidth: 0, CellHeight: 2,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "delete_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        Padding: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
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
    ui.SetMCPMetadata(true, "ExpenseRow", "# Expense Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: \"Reusable expense row with paid state, category selector, and delete action.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\npaid:\n  type: checkbox\n  title: Paid\n  value: false\n\nitem:\n  type: label\n  text: Item\n  user-select: text\n\namount:\n  type: label\n  text: \"0.00\"\n\ncategory:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ndelete_btn:\n  type: button\n  title: Delete\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@row:\n  background: \"#172033\"\n  padding: 1\n@item:\n  color: \"#ffffff\"\n@amount:\n  color: \"#bfdbfe\"\ncheckbox:\n  color: \"#cbd5e1\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------+\n| paid..... item..**..................... amount...... |\n| category..**........................... delete_btn.. |\n+------------------------------------------------------+\n```\n", "Reusable expense row with paid state, category selector, and delete action.")
    ui.Paid = uimd.NewCheckBox("paid", "Paid", false)
    ui.AddElement(ui.Paid)
    ui.Paid.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Paid.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Paid.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#22c55e"),
    })
    ui.Paid.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#94a3b8"),
    })
    ui.Paid.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.Item = uimd.NewLabel("item", "Item")
    ui.AddElement(ui.Item)
    ui.Item.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Item.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Amount = uimd.NewLabel("amount", "0.00")
    ui.AddElement(ui.Amount)
    ui.Amount.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
    })
    ui.Amount.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Category = uimd.NewComboBox("category", []string{"Food", "Transport", "Home", "Health", "Other"})
    ui.AddElement(ui.Category)
    ui.Category.SetSelectedIndex(0)
    ui.Category.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Category.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Category.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Category.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Category.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
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

func (ui *ExpenseRowUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ExpenseRowUI) HandleGeneratedButton(name string) bool {
    if name == "delete_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnDeleteBtnClick() }); ok {
            handler.OnDeleteBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ExpenseRowUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "paid" {
        if handler, ok := ui.eventHandler.(interface{ OnPaidChange(string) }); ok {
            handler.OnPaidChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ExpenseRowUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ExpenseRowUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "category" {
        if handler, ok := ui.eventHandler.(interface{ OnCategoryChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnCategoryChange(selected)
            return true
        }
        return false
    }
    return false
}
