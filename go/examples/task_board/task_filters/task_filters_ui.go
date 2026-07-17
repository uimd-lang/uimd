// Auto-generated UI code for task_filters - DO NOT EDIT MANUALLY.
package task_filters

import uimd "uimd"

type TaskFiltersUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    FiltersLabel *uimd.Label
    SearchLabel *uimd.Label
    StatusLabel *uimd.Label
    OwnerLabel *uimd.Label
    Search *uimd.TextInput
    StatusFilter *uimd.ComboBox
    OwnerFilter *uimd.ComboBox
    ApplyFiltersBtn *uimd.Button
    ResetFiltersBtn *uimd.Button
}

func NewTaskFiltersUI() *TaskFiltersUI {
    ui := &TaskFiltersUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Task Filters")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 78, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "filters_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#aab2c3"),
    },
        },
        {
            Row: 0, Col: 0,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 2,
            CellName: "", CellWidth: 28, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 27, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 1,
            Content: "search_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cfd5e3"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 52,
            CellCharsWidth: 26, CellCharsHeight: 2,
            CellName: "", CellWidth: 26, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 24, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 1,
            Content: "owner_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cfd5e3"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 29,
            CellCharsWidth: 22, CellCharsHeight: 2,
            CellName: "", CellWidth: 22, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 1,
            Content: "status_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cfd5e3"),
    },
        },
        {
            Row: 1, Col: 0,
            CellRow: 5, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 3,
            CellName: "", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 22, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 22, CharsHeight: 1,
            MarginRight: 56, MarginBottom: 1,
            Content: "apply_filters_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#2f80ed"),
        Color: uimd.NewColor("#f5f5f5"),
    },
        },
        {
            Row: 1, Col: 0,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 28, CellCharsHeight: 2,
            CellName: "", CellWidth: 28, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 27, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "search",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#252b35"),
        Color: uimd.NewColor("#ffffff"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 2, CellCol: 52,
            CellCharsWidth: 26, CellCharsHeight: 2,
            CellName: "", CellWidth: 26, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 24, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "owner_filter",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#252b35"),
        Color: uimd.NewColor("#ffffff"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 2, CellCol: 29,
            CellCharsWidth: 22, CellCharsHeight: 2,
            CellName: "", CellWidth: 22, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "status_filter",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#252b35"),
        Color: uimd.NewColor("#ffffff"),
    },
        },
        {
            Row: 1, Col: 24,
            CellRow: 5, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 3,
            CellName: "", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 22, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 22, CharsHeight: 1,
            MarginRight: 32, MarginBottom: 1,
            Content: "reset_filters_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#343b4b"),
        Color: uimd.NewColor("#f5f5f5"),
    },
        },
    })
    ui.SetGeneratedKind("control")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#15171c"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "TaskFilters", "# Task Filters\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: draft\ndescription: \"Reusable task board filter panel.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfilters_label:\n  type: label\n  text: FILTERS\n  description: \"Filter panel label.\"\n  expose: false\n\nsearch_label:\n  type: label\n  text: Search\n  description: \"Search filter label.\"\n  expose: false\n\nstatus_label:\n  type: label\n  text: Status\n  description: \"Status filter label.\"\n  expose: false\n\nowner_label:\n  type: label\n  text: Assignee\n  description: \"Assignee filter label.\"\n  expose: false\n\nsearch:\n  type: textinput\n  value: \"\"\n  maxlength: 60\n  placeholder: \"Task title or description\"\n  description: \"Filter tasks by title or description.\"\n\nstatus_filter:\n  type: combobox\n  options: [Any, Todo, Doing, Blocked, Done]\n  selected_item: Any\n  description: \"Filter tasks by status.\"\n\nowner_filter:\n  type: combobox\n  options: [Any, Ava, Bruno, Chen, Dana]\n  selected_item: Any\n  description: \"Filter tasks by assignee.\"\n\napply_filters_btn:\n  type: button\n  title: \"Apply filter\"\n  description: \"Apply the current task filters.\"\n\nreset_filters_btn:\n  type: button\n  title: Reset\n  description: \"Reset search, status, and assignee filters.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  background: \"#15171c\"\n  border-width: 0\nlabel:\n  color: \"#f5f5f5\"\ntextinput:\n  background: \"#252b35\"\n  color: \"#ffffff\"\n  focus-background: \"#303a49\"\n  edit-background: \"#344459\"\n  cursor-background: \"#93c5fd\"\n  cursor-color: \"#ffffff\"\ncombobox:\n  background: \"#252b35\"\n  color: \"#ffffff\"\n  focus-background: \"#303a49\"\n  edit-background: \"#344459\"\n  selected-background: \"#2f80ed\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#2f3545\"\n  color: \"#f5f5f5\"\n  focus-background: \"#2f80ed\"\n@filters_label:\n  color: \"#aab2c3\"\n@search_label:\n  color: \"#cfd5e3\"\n@status_label:\n  color: \"#cfd5e3\"\n@owner_label:\n  color: \"#cfd5e3\"\n@apply_filters_btn:\n  background: \"#2f80ed\"\n  focus-background: \"#1d6fd8\"\n@reset_filters_btn:\n  background: \"#343b4b\"\n```\n\n## User Interface\n\n```ui\n+-**---------------------------------------------------------------------------+\n|filters_label..**.............................................................|\n+----------------------------+----------------------+--------------------------+\n|search_label..**........... | status_label..**.... | owner_label..**......... |\n|search..**................. | status_filter.**.... | owner_filter..**........ |\n+-**-------------------------+----------------------+--------------------------+\n|                                                                              |\n|apply_filters_btn.....  reset_filters_btn.....                                |\n|                                                                              |\n+------------------------------------------------------------------------------+\n```\n", "Reusable task board filter panel.")
    ui.FiltersLabel = uimd.NewLabel("filters_label", "FILTERS")
    ui.AddElement(ui.FiltersLabel)
    ui.FiltersLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#aab2c3"),
    })
    ui.FiltersLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.SearchLabel = uimd.NewLabel("search_label", "Search")
    ui.AddElement(ui.SearchLabel)
    ui.SearchLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cfd5e3"),
    })
    ui.SearchLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.StatusLabel = uimd.NewLabel("status_label", "Status")
    ui.AddElement(ui.StatusLabel)
    ui.StatusLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cfd5e3"),
    })
    ui.StatusLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.OwnerLabel = uimd.NewLabel("owner_label", "Assignee")
    ui.AddElement(ui.OwnerLabel)
    ui.OwnerLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cfd5e3"),
    })
    ui.OwnerLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Search = uimd.NewTextInput("search", "", 0)
    ui.AddElement(ui.Search)
    ui.Search.SetStyle(uimd.Style{
        Background: uimd.NewColor("#252b35"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Search.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#303a49"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Search.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#344459"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Search.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#93c5fd"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Search.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Search.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.StatusFilter = uimd.NewComboBox("status_filter", []string{"Any", "Todo", "Doing", "Blocked", "Done"})
    ui.AddElement(ui.StatusFilter)
    ui.StatusFilter.SetSelectedIndex(0)
    ui.StatusFilter.SetStyle(uimd.Style{
        Background: uimd.NewColor("#252b35"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.StatusFilter.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#303a49"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.StatusFilter.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#344459"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.StatusFilter.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2f80ed"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.StatusFilter.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.OwnerFilter = uimd.NewComboBox("owner_filter", []string{"Any", "Ava", "Bruno", "Chen", "Dana"})
    ui.AddElement(ui.OwnerFilter)
    ui.OwnerFilter.SetSelectedIndex(0)
    ui.OwnerFilter.SetStyle(uimd.Style{
        Background: uimd.NewColor("#252b35"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.OwnerFilter.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#303a49"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.OwnerFilter.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#344459"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.OwnerFilter.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2f80ed"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.OwnerFilter.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ApplyFiltersBtn = uimd.NewButton("apply_filters_btn", "Apply filter")
    ui.AddElement(ui.ApplyFiltersBtn)
    ui.ApplyFiltersBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#2f80ed"),
        Color: uimd.NewColor("#f5f5f5"),
    })
    ui.ApplyFiltersBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1d6fd8"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ApplyFiltersBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ResetFiltersBtn = uimd.NewButton("reset_filters_btn", "Reset")
    ui.AddElement(ui.ResetFiltersBtn)
    ui.ResetFiltersBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#343b4b"),
        Color: uimd.NewColor("#f5f5f5"),
    })
    ui.ResetFiltersBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2f80ed"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ResetFiltersBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *TaskFiltersUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *TaskFiltersUI) HandleGeneratedButton(name string) bool {
    if name == "apply_filters_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnApplyFiltersBtnClick() }); ok {
            handler.OnApplyFiltersBtnClick()
            return true
        }
        return false
    }
    if name == "reset_filters_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnResetFiltersBtnClick() }); ok {
            handler.OnResetFiltersBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *TaskFiltersUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "search" {
        if handler, ok := ui.eventHandler.(interface{ OnSearchChange(string) }); ok {
            handler.OnSearchChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *TaskFiltersUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "search" {
        if handler, ok := ui.eventHandler.(interface{ OnSearchSubmit(string) }); ok {
            handler.OnSearchSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *TaskFiltersUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "status_filter" {
        if handler, ok := ui.eventHandler.(interface{ OnStatusFilterChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnStatusFilterChange(selected)
            return true
        }
        return false
    }
    if name == "owner_filter" {
        if handler, ok := ui.eventHandler.(interface{ OnOwnerFilterChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnOwnerFilterChange(selected)
            return true
        }
        return false
    }
    return false
}
