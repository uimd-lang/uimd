// Auto-generated UI code for settings - DO NOT EDIT MANUALLY.
package settings

import uimd "uimd"

type SettingsUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    HeaderTitle *uimd.Label
    AutoScroll *uimd.CheckBox
    ShowTimestamps *uimd.CheckBox
    DefaultTypeLabel *uimd.Label
    DefaultType *uimd.ComboBox
    SaveButton *uimd.Button
    CancelButton *uimd.Button
}

func NewSettingsUI() *SettingsUI {
    ui := &SettingsUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Activity Feed Settings")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 42, CellCharsHeight: 1,
            CellName: "header", CellWidth: 42, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 42, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "header_title",
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
            Row: 0, Col: 2,
            CellRow: 10, CellCol: 0,
            CellCharsWidth: 42, CellCharsHeight: 1,
            CellName: "buttons", CellWidth: 42, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 17, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 17, CharsHeight: 1,
            MarginRight: 23, MarginBottom: 0,
            Content: "save_button",
            CellStyle: uimd.Style{
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(2),
        PaddingRight: uimd.Int(2),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 21,
            CellRow: 10, CellCol: 0,
            CellCharsWidth: 42, CellCharsHeight: 1,
            CellName: "buttons", CellWidth: 42, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 19, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 0,
            Content: "cancel_button",
            CellStyle: uimd.Style{
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(2),
        PaddingRight: uimd.Int(2),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 42, CellCharsHeight: 7,
            CellName: "content", CellWidth: 42, CellHeight: 7,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 38, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 5,
            Content: "auto_scroll",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 42, CellCharsHeight: 7,
            CellName: "content", CellWidth: 42, CellHeight: 7,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 38, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 3,
            Content: "show_timestamps",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 5, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 42, CellCharsHeight: 7,
            CellName: "content", CellWidth: 42, CellHeight: 7,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 20, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 1,
            MarginRight: 20, MarginBottom: 1,
            Content: "default_type_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 5, Col: 24,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 42, CellCharsHeight: 7,
            CellName: "content", CellWidth: 42, CellHeight: 7,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 16, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 1,
            Content: "default_type",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
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
    ui.SetMCPMetadata(true, "Settings", "# Activity Feed Settings\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Settings dialog for activity feed auto-scroll, timestamps, and default activity type.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: \"Activity Feed Settings\"\n  description: \"Dialog title.\"\n  expose: false\n\nauto_scroll:\n  type: checkbox\n  title: \"Auto-scroll to newest\"\n  value: 1\n  description: \"Toggle whether new activity items scroll into view.\"\n\nshow_timestamps:\n  type: checkbox\n  title: \"Show timestamps\"\n  value: 1\n  description: \"Toggle whether newly added activity items include timestamps.\"\n\ndefault_type_label:\n  type: label\n  text: \"Default type\"\n  description: \"Label for the default activity type selector.\"\n  expose: false\n\ndefault_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: \"Default activity type used by the main window.\"\n\nsave_button:\n  type: button\n  title: Save\n  description: \"Apply settings and close the dialog.\"\n\ncancel_button:\n  type: button\n  title: Cancel\n  description: \"Close the dialog without applying settings.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@header_title:\n  text-align: center\n  color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\ncheckbox:\n  color: \"#d1d5db\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#f97316\"\n@buttons:\n  padding: 0, 2, 1, 2\n```\n\n## User Interface\n\n```ui\n+-header-----------------------------------+\n|header_title..**..........................|\n+-content----------------------------------+\n|                                          |\n|  auto_scroll...........................  |\n|                                          |\n|  show_timestamps.......................  |\n|                                          |\n|  default_type_label..  default_type....  |\n|                                          |\n+-buttons----------------------------------+\n|  save_button......  cancel_button......  |\n+------------------------------------------+\n```\n", "Settings dialog for activity feed auto-scroll, timestamps, and default activity type.")
    ui.HeaderTitle = uimd.NewLabel("header_title", "Activity Feed Settings")
    ui.AddElement(ui.HeaderTitle)
    ui.HeaderTitle.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.HeaderTitle.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.AutoScroll = uimd.NewCheckBox("auto_scroll", "Auto-scroll to newest", true)
    ui.AddElement(ui.AutoScroll)
    ui.AutoScroll.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.AutoScroll.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AutoScroll.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#22c55e"),
    })
    ui.AutoScroll.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f97316"),
    })
    ui.AutoScroll.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.ShowTimestamps = uimd.NewCheckBox("show_timestamps", "Show timestamps", true)
    ui.AddElement(ui.ShowTimestamps)
    ui.ShowTimestamps.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.ShowTimestamps.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ShowTimestamps.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#22c55e"),
    })
    ui.ShowTimestamps.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f97316"),
    })
    ui.ShowTimestamps.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.DefaultTypeLabel = uimd.NewLabel("default_type_label", "Default type")
    ui.AddElement(ui.DefaultTypeLabel)
    ui.DefaultTypeLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.DefaultTypeLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.DefaultType = uimd.NewComboBox("default_type", []string{"Info", "Task", "Warning", "Deploy", "Note"})
    ui.AddElement(ui.DefaultType)
    ui.DefaultType.SetSelectedIndex(0)
    ui.DefaultType.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.DefaultType.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DefaultType.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.DefaultType.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DefaultType.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SaveButton = uimd.NewButton("save_button", "Save")
    ui.AddElement(ui.SaveButton)
    ui.SaveButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SaveButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SaveButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CancelButton = uimd.NewButton("cancel_button", "Cancel")
    ui.AddElement(ui.CancelButton)
    ui.CancelButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.CancelButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CancelButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *SettingsUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *SettingsUI) HandleGeneratedButton(name string) bool {
    if name == "save_button" {
        if handler, ok := ui.eventHandler.(interface{ OnSaveButtonClick() }); ok {
            handler.OnSaveButtonClick()
            return true
        }
        return false
    }
    if name == "cancel_button" {
        if handler, ok := ui.eventHandler.(interface{ OnCancelButtonClick() }); ok {
            handler.OnCancelButtonClick()
            return true
        }
        return false
    }
    return false
}

func (ui *SettingsUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "auto_scroll" {
        if handler, ok := ui.eventHandler.(interface{ OnAutoScrollChange(string) }); ok {
            handler.OnAutoScrollChange(value)
            return true
        }
        return false
    }
    if name == "show_timestamps" {
        if handler, ok := ui.eventHandler.(interface{ OnShowTimestampsChange(string) }); ok {
            handler.OnShowTimestampsChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *SettingsUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *SettingsUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "default_type" {
        if handler, ok := ui.eventHandler.(interface{ OnDefaultTypeChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnDefaultTypeChange(selected)
            return true
        }
        return false
    }
    return false
}
