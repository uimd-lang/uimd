// Auto-generated UI code for edit_field_row - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type EditFieldRowUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    FieldInput *uimd.TextInput
    FieldBtn *uimd.Button
}

func NewEditFieldRowUI() *EditFieldRowUI {
    ui := &EditFieldRowUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Edit Field Row")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 3,
            CellName: "row", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 88, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 2,
            Content: "field_input",
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
            Row: 2, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 90, CellCharsHeight: 3,
            CellName: "row", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 19, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 19, CharsHeight: 1,
            MarginRight: 70, MarginBottom: 0,
            Content: "field_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#14532d"),
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
    ui.SetMCPMetadata(true, "EditFieldRow", "# Edit Field Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"One editable field row with an action button inside the edit ScrollView.\"\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfield_input:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Editable regression field.\"\n\nfield_btn:\n  type: button\n  title: Action\n  description: \"Action button beside the editable field.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@row:\n  background: \"#111827\"\n  padding: 1, 1, 1, 1\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@field_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------------------------------------------+\n| field_input..**......................................................................... |\n|                                                                                          |\n| field_btn.......... .................................................................... |\n+------------------------------------------------------------------------------------------+\n```\n", "One editable field row with an action button inside the edit ScrollView.")
    ui.FieldInput = uimd.NewTextInput("field_input", "", 0)
    ui.AddElement(ui.FieldInput)
    ui.FieldInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.FieldInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.FieldInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.FieldInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.FieldInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.FieldInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.FieldBtn = uimd.NewButton("field_btn", "Action")
    ui.AddElement(ui.FieldBtn)
    ui.FieldBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#14532d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.FieldBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#16a34a"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.FieldBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *EditFieldRowUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *EditFieldRowUI) HandleGeneratedButton(name string) bool {
    if name == "field_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnFieldBtnClick() }); ok {
            handler.OnFieldBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *EditFieldRowUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "field_input" {
        if handler, ok := ui.eventHandler.(interface{ OnFieldInputChange(string) }); ok {
            handler.OnFieldInputChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *EditFieldRowUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "field_input" {
        if handler, ok := ui.eventHandler.(interface{ OnFieldInputSubmit(string) }); ok {
            handler.OnFieldInputSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *EditFieldRowUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *EditFieldRowUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *EditFieldRowUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
