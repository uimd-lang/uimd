// Auto-generated UI code for compatibility - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type CompatibilityUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    AlignedInput *uimd.TextInput
    Choice *uimd.ComboBox
    Scroller *uimd.ReusableElement
}

func NewCompatibilityUI() *CompatibilityUI {
    ui := &CompatibilityUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("UIMD 0.5.3 Compatibility Application")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 32, CellCharsHeight: 5,
            CellName: "", CellWidth: 32, CellHeight: 5,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 32, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 4,
            Content: "aligned_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
        TextAlign: "right",
    },
        },
        {
            Row: 1, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 32, CellCharsHeight: 5,
            CellName: "", CellWidth: 32, CellHeight: 5,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 2,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 32, CharsHeight: 2,
            MarginRight: 0, MarginBottom: 2,
            Content: "choice",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 3, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 32, CellCharsHeight: 5,
            CellName: "", CellWidth: 32, CellHeight: 5,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 2,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 32, CharsHeight: 2,
            MarginRight: 0, MarginBottom: 0,
            Content: "scroller",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#303545"),
    },
        },
    })
    ui.SetGeneratedKind("window")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#303545"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "Compatibility", "# UIMD 0.5.3 Compatibility Application\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"Retained generated application for patch-level runtime compatibility.\"\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nuses:\n  - compat_scroll\n```\n\n## Members\n\n```yaml\naligned_input:\n  type: textinput\n  value: \"abc\"\n  maxlength: 16\n\nchoice:\n  type: combobox\n  options: [one, two, three, four, five, six, seven, eight]\n  selected_item: one\n\nscroller:\n  type: compat_scroll\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#303545\"\n@aligned_input:\n  text-align: right\n@scroller:\n  background: \"#303545\"\n  focus-background: \"#ffffff14\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------+\n|aligned_input...................|\n|choice..........................|\n|................................|\n|scroller........................|\n|................................|\n+--------------------------------+\n```\n", "Retained generated application for patch-level runtime compatibility.")
    ui.AlignedInput = uimd.NewTextInput("aligned_input", "abc", 0)
    ui.AddElement(ui.AlignedInput)
    ui.AlignedInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
        TextAlign: "right",
    })
    ui.AlignedInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AlignedInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AlignedInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff"),
        Color: uimd.NewColor("#0e1117"),
    })
    ui.AlignedInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AlignedInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Choice = uimd.NewComboBox("choice", []string{"one", "two", "three", "four", "five", "six", "seven", "eight"})
    ui.AddElement(ui.Choice)
    ui.Choice.SetSelectedIndex(0)
    ui.Choice.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Choice.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Choice.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Choice.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Choice.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Scroller = uimd.NewReusableElement("scroller", "compat_scroll")
    ui.AddElement(ui.Scroller)
    ui.Scroller.SetStyle(uimd.Style{
        Background: uimd.NewColor("#303545"),
    })
    ui.Scroller.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.Scroller.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.Scroller.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    return ui
}

func (ui *CompatibilityUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *CompatibilityUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *CompatibilityUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "aligned_input" {
        if handler, ok := ui.eventHandler.(interface{ OnAlignedInputChange(string) }); ok {
            handler.OnAlignedInputChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *CompatibilityUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "aligned_input" {
        if handler, ok := ui.eventHandler.(interface{ OnAlignedInputSubmit(string) }); ok {
            handler.OnAlignedInputSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *CompatibilityUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "choice" {
        if handler, ok := ui.eventHandler.(interface{ OnChoiceChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnChoiceChange(selected)
            return true
        }
        return false
    }
    return false
}
