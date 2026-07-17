// Auto-generated UI code for categories_view - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type CategoriesViewUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    ViewTitle *uimd.Label
    CategoryName *uimd.TextInput
    AddCategoryBtn *uimd.Button
    Categories *uimd.ListBox
    RemoveCategoryBtn *uimd.Button
}

func NewCategoriesViewUI() *CategoriesViewUI {
    ui := &CategoriesViewUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Categories View")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 11,
            CellName: "view", CellWidth: 0, CellHeight: 11,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 58, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 58, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 10,
            Content: "view_title",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bbf7d0"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 11,
            CellName: "view", CellWidth: 0, CellHeight: 11,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 7, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 7, CharsHeight: 1,
            MarginRight: 52, MarginBottom: 8,
            Content: "\"Name:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 2, Col: 9,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 11,
            CellName: "view", CellWidth: 0, CellHeight: 11,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 49, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 8,
            Content: "category_name",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 4, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 11,
            CellName: "view", CellWidth: 0, CellHeight: 11,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 32, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 32, CharsHeight: 1,
            MarginRight: 27, MarginBottom: 6,
            Content: "add_category_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 4, Col: 34,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 11,
            CellName: "view", CellWidth: 0, CellHeight: 11,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 24, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 24, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 6,
            Content: "remove_category_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 6, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 60, CellCharsHeight: 11,
            CellName: "view", CellWidth: 0, CellHeight: 11,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 58, CharsHeight: 5,
            MarginRight: 1, MarginBottom: 0,
            Content: "categories",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
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
    ui.SetMCPMetadata(true, "CategoriesView", "# Categories View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Simple category list management view.\"\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Categories\n\ncategory_name:\n  type: textinput\n  value: \"\"\n  maxlength: 40\n\nadd_category_btn:\n  type: button\n  title: Add\n\ncategories:\n  type: listbox\n  options: [Food, Transport, Home, Health, Other]\n  selected_items: [Food]\n\nremove_category_btn:\n  type: button\n  title: Remove selected\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#bbf7d0\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nlistbox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Name:\" category_name..**................................ |\n|                                                            |\n| add_category_btn................ remove_category_btn..... |\n|                                                            |\n| categories..**............................................ |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n", "Simple category list management view.")
    ui.ViewTitle = uimd.NewLabel("view_title", "Categories")
    ui.AddElement(ui.ViewTitle)
    ui.ViewTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bbf7d0"),
    })
    ui.ViewTitle.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.CategoryName = uimd.NewTextInput("category_name", "", 0)
    ui.AddElement(ui.CategoryName)
    ui.CategoryName.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.CategoryName.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CategoryName.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CategoryName.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.CategoryName.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CategoryName.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.AddCategoryBtn = uimd.NewButton("add_category_btn", "Add")
    ui.AddElement(ui.AddCategoryBtn)
    ui.AddCategoryBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.AddCategoryBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AddCategoryBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Categories = uimd.NewListBox("categories", []string{"Food", "Transport", "Home", "Health", "Other"}, false)
    ui.AddElement(ui.Categories)
    ui.Categories.SetSelectedIndex(0)
    ui.Categories.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Categories.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Categories.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Categories.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Categories.SetActiveStyle(uimd.Style{
        Background: uimd.NewColor("#DDDDDD99"),
        Color: uimd.NewColor("#FFFFFF"),
    })
    ui.Categories.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.RemoveCategoryBtn = uimd.NewButton("remove_category_btn", "Remove selected")
    ui.AddElement(ui.RemoveCategoryBtn)
    ui.RemoveCategoryBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.RemoveCategoryBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RemoveCategoryBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *CategoriesViewUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *CategoriesViewUI) HandleGeneratedButton(name string) bool {
    if name == "add_category_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnAddCategoryBtnClick() }); ok {
            handler.OnAddCategoryBtnClick()
            return true
        }
        return false
    }
    if name == "remove_category_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnRemoveCategoryBtnClick() }); ok {
            handler.OnRemoveCategoryBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *CategoriesViewUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "category_name" {
        if handler, ok := ui.eventHandler.(interface{ OnCategoryNameChange(string) }); ok {
            handler.OnCategoryNameChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *CategoriesViewUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "category_name" {
        if handler, ok := ui.eventHandler.(interface{ OnCategoryNameSubmit(string) }); ok {
            handler.OnCategoryNameSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *CategoriesViewUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "categories" {
        if handler, ok := ui.eventHandler.(interface{ OnCategoriesSelectionChange([]string) }); ok {
            handler.OnCategoriesSelectionChange(value)
            return true
        }
        return false
    }
    return false
}
