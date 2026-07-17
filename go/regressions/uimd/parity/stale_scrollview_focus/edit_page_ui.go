// Auto-generated UI code for edit_page - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type EditPageUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    EditTitle *uimd.Label
    BackBtn *uimd.Button
    SaveBtn *uimd.Button
    NameLabel *uimd.Label
    NameInput *uimd.TextInput
    BodyLabel *uimd.Label
    Fields *uimd.ViewHost
    Status *uimd.Label
}

func NewEditPageUI() *EditPageUI {
    ui := &EditPageUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Edit Page")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 1,
            CellName: "top", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 77, MarginBottom: 0,
            Content: "back_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 21,
            CellName: "form", CellWidth: 0, CellHeight: 21,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 14, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 75, MarginBottom: 20,
            Content: "name_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 0, Col: 14,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 1,
            CellName: "top", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 60, CharsHeight: 1,
            MarginRight: 16, MarginBottom: 0,
            Content: "edit_title",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 16,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 21,
            CellName: "form", CellWidth: 0, CellHeight: 21,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 73, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 20,
            Content: "name_input",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 0, Col: 75,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 1,
            CellName: "top", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 14, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "save_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 21,
            CellName: "form", CellWidth: 0, CellHeight: 21,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 88, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 18,
            Content: "status",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 4, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 21,
            CellName: "form", CellWidth: 0, CellHeight: 21,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 88, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 16,
            Content: "body_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 5, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 21,
            CellName: "form", CellWidth: 0, CellHeight: 21,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 88, CharsHeight: 15,
            MarginRight: 1, MarginBottom: 1,
            Content: "fields",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
        ScopeDimBackground: uimd.NewColor("#0000004d"),
    },
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "EditPage", "# Edit Page\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Edit page shown after a ScrollView row button replaces the active ViewHost page.\"\ntags: [regression, issue-5, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - edit_fields_scroll\n```\n\n## Members\n\n```yaml\nedit_title:\n  type: label\n  text: \"Edit item\"\n  description: \"Edit page title.\"\n  expose: false\n\nback_btn:\n  type: button\n  title: Back\n  description: \"Return to the list page.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Dummy save action used by the regression app.\"\n\nname_label:\n  type: label\n  text: Name\n  description: \"Name field label.\"\n  expose: false\n\nname_input:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Editable item name.\"\n\nbody_label:\n  type: label\n  text: Fields\n  description: \"Fields panel label.\"\n  expose: false\n\nfields:\n  type: viewhost\n  description: \"Scrollable edit fields panel.\"\n\nstatus:\n  type: label\n  text: \"Opened from ScrollView\"\n  description: \"Edit page status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@top:\n  background: \"#172033\"\n@edit_title:\n  color: \"#ffffff\"\n  text-align: center\n@form:\n  background: \"#111827\"\n  padding: 1, 1, 1, 1\n@fields:\n  background: \"#030712\"\n  focus-background: \"#07111f\"\n  edit-background: \"#0f172a\"\n  scope-dim-background: \"#0000004d\"\n@status:\n  color: \"#93c5fd\"\nlabel:\n  color: \"#e5e7eb\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@save_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-top-**-----------------------------------------------------------------------------------+\n| back_btn.... edit_title..**.............................................. save_btn...... |\n+-form-**----------------------------------------------------------------------------------+\n| name_label.... name_input..**........................................................... |\n|                                                                                          |\n| status..**.............................................................................. |\n|                                                                                          |\n| body_label..**.......................................................................... |\n| fields..**.............................................................................. |\n| *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n|                                                                                          |\n+------------------------------------------------------------------------------------------+\n```\n", "Edit page shown after a ScrollView row button replaces the active ViewHost page.")
    ui.EditTitle = uimd.NewLabel("edit_title", "Edit item")
    ui.AddElement(ui.EditTitle)
    ui.EditTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.EditTitle.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.BackBtn = uimd.NewButton("back_btn", "Back")
    ui.AddElement(ui.BackBtn)
    ui.BackBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.BackBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.BackBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SaveBtn = uimd.NewButton("save_btn", "Save")
    ui.AddElement(ui.SaveBtn)
    ui.SaveBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SaveBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#16a34a"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SaveBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.NameLabel = uimd.NewLabel("name_label", "Name")
    ui.AddElement(ui.NameLabel)
    ui.NameLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.NameLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.NameInput = uimd.NewTextInput("name_input", "", 0)
    ui.AddElement(ui.NameInput)
    ui.NameInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.NameInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NameInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NameInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.NameInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NameInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.BodyLabel = uimd.NewLabel("body_label", "Fields")
    ui.AddElement(ui.BodyLabel)
    ui.BodyLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.BodyLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Fields = uimd.NewViewHost("fields")
    ui.AddElement(ui.Fields)
    ui.Fields.SetStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        ScopeDimBackground: uimd.NewColor("#0000004d"),
    })
    ui.Fields.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#07111f"),
    })
    ui.Fields.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
    })
    ui.Status = uimd.NewLabel("status", "Opened from ScrollView")
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

func (ui *EditPageUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *EditPageUI) HandleGeneratedButton(name string) bool {
    if name == "back_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnBackBtnClick() }); ok {
            handler.OnBackBtnClick()
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
    return false
}

func (ui *EditPageUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "name_input" {
        if handler, ok := ui.eventHandler.(interface{ OnNameInputChange(string) }); ok {
            handler.OnNameInputChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *EditPageUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "name_input" {
        if handler, ok := ui.eventHandler.(interface{ OnNameInputSubmit(string) }); ok {
            handler.OnNameInputSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *EditPageUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
