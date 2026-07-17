// Auto-generated UI code for activity_feed - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ActivityFeedUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    HeaderTitle *uimd.Label
    SettingsButton *uimd.Button
    QuitButton *uimd.Button
    Feed *uimd.ReusableElement
    TypeLabel *uimd.Label
    ActivityType *uimd.ComboBox
    MessageLabel *uimd.Label
    Message *uimd.TextArea
    AddButton *uimd.Button
    SeedButton *uimd.Button
    ClearButton *uimd.Button
    Status *uimd.Label
}

func NewActivityFeedUI() *ActivityFeedUI {
    ui := &ActivityFeedUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Activity Feed")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 33, CharsHeight: 1,
            MarginRight: 32, MarginBottom: 0,
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
            Row: 0, Col: 1,
            CellRow: 16, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 1,
            CellName: "actions", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 19, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 19, CharsHeight: 1,
            MarginRight: 45, MarginBottom: 0,
            Content: "add_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 7,
            CellName: "content", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 63, CharsHeight: 7,
            MarginRight: 1, MarginBottom: 0,
            Content: "feed",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 18, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 1,
            CellName: "", CellWidth: 65, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 63, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "status",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 10, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 5,
            CellName: "controls", CellWidth: 0, CellHeight: 5,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 10, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 54, MarginBottom: 4,
            Content: "type_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 0, Col: 16,
            CellRow: 10, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 5,
            CellName: "controls", CellWidth: 0, CellHeight: 5,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 22, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 22, CharsHeight: 1,
            MarginRight: 27, MarginBottom: 4,
            Content: "activity_type",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 0, Col: 21,
            CellRow: 16, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 1,
            CellName: "actions", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 20, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 1,
            MarginRight: 24, MarginBottom: 0,
            Content: "seed_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 34,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 17, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 17, CharsHeight: 1,
            MarginRight: 14, MarginBottom: 0,
            Content: "settings_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 42,
            CellRow: 16, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 1,
            CellName: "actions", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 22, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 22, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "clear_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 52,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 13, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "quit_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 10, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 5,
            CellName: "controls", CellWidth: 0, CellHeight: 5,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 13, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 51, MarginBottom: 2,
            Content: "message_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 2, Col: 16,
            CellRow: 10, CellCol: 0,
            CellCharsWidth: 65, CellCharsHeight: 5,
            CellName: "controls", CellWidth: 0, CellHeight: 5,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 3,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 47, CharsHeight: 3,
            MarginRight: 2, MarginBottom: 0,
            Content: "message",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0b1220"),
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
    ui.SetMCPMetadata(true, "ActivityFeed", "# Activity Feed\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Activity feed example with dynamically appended scroll view items and a settings dialog.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - activity_feed_panel/activity_feed_panel\n  - settings/settings\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: \"Activity Feed\"\n  description: \"Application title.\"\n  expose: false\n\nsettings_button:\n  type: button\n  title: Settings\n  description: \"Open the activity feed settings dialog.\"\n\nquit_button:\n  type: button\n  title: Quit\n  description: \"Close the activity feed example.\"\n\nfeed:\n  type: activity_feed_panel\n  description: \"Scrollable activity timeline.\"\n\ntype_label:\n  type: label\n  text: Type\n  description: \"Label for activity type selector.\"\n  expose: false\n\nactivity_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: \"Activity type selector.\"\n\nmessage_label:\n  type: label\n  text: Message\n  description: \"Label for activity message input.\"\n  expose: false\n\nmessage:\n  type: textarea\n  value: \"\"\n  maxlength: 180\n  description: \"Activity message input.\"\n\nadd_button:\n  type: button\n  title: Add\n  description: \"Append one activity item to the feed.\"\n\nseed_button:\n  type: button\n  title: Seed\n  description: \"Append sample activity items to the feed.\"\n\nclear_button:\n  type: button\n  title: Clear\n  description: \"Remove all activity items from the feed.\"\n\nstatus:\n  type: label\n  text: \"Ready\"\n  description: \"Shows activity feed status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@header_title:\n  text-align: center\n  color: \"#ffffff\"\n@content:\n  background: \"#030712\"\n@feed:\n  background: \"#00000000\"\n@controls:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@clear_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n@status:\n  background: \"#0f172a\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-header-**-------------------------------------------------------+\n|header_title..**................. settings_button.. quit_button..|\n+-content-**------------------------------------------------------+\n| feed..**....................................................... |\n| ............................................................... |\n| ............................................................... |\n* *.............................................................. |\n* *.............................................................. |\n* *.............................................................. |\n| ............................................................... |\n+-controls-**-----------------------------------------------------+\n| type_label     activity_type.........                           |\n|                                                                 |\n| message_label  message..**....................................  |\n|               ...............................................   |\n|               ...............................................   |\n+-actions-**------------------------------------------------------+\n| add_button......... seed_button......... clear_button.......... |\n+-----------------------------------------------------------------+\n| status..**..................................................... |\n+-----------------------------------------------------------------+\n```\n", "Activity feed example with dynamically appended scroll view items and a settings dialog.")
    ui.HeaderTitle = uimd.NewLabel("header_title", "Activity Feed")
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
    ui.SettingsButton = uimd.NewButton("settings_button", "Settings")
    ui.AddElement(ui.SettingsButton)
    ui.SettingsButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SettingsButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SettingsButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.QuitButton = uimd.NewButton("quit_button", "Quit")
    ui.AddElement(ui.QuitButton)
    ui.QuitButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.QuitButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.QuitButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Feed = uimd.NewReusableElement("feed", "activity_feed_panel")
    ui.AddElement(ui.Feed)
    ui.Feed.SetStyle(uimd.Style{
        Background: uimd.NewColor("#00000000"),
    })
    ui.Feed.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.Feed.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.Feed.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.TypeLabel = uimd.NewLabel("type_label", "Type")
    ui.AddElement(ui.TypeLabel)
    ui.TypeLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.TypeLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ActivityType = uimd.NewComboBox("activity_type", []string{"Info", "Task", "Warning", "Deploy", "Note"})
    ui.AddElement(ui.ActivityType)
    ui.ActivityType.SetSelectedIndex(0)
    ui.ActivityType.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ActivityType.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ActivityType.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ActivityType.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ActivityType.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.MessageLabel = uimd.NewLabel("message_label", "Message")
    ui.AddElement(ui.MessageLabel)
    ui.MessageLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.MessageLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Message = uimd.NewTextArea("message", "")
    ui.AddElement(ui.Message)
    ui.Message.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Message.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#25364f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Message.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Message.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.Message.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Message.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.AddButton = uimd.NewButton("add_button", "Add")
    ui.AddElement(ui.AddButton)
    ui.AddButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.AddButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AddButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SeedButton = uimd.NewButton("seed_button", "Seed")
    ui.AddElement(ui.SeedButton)
    ui.SeedButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SeedButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SeedButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ClearButton = uimd.NewButton("clear_button", "Clear")
    ui.AddElement(ui.ClearButton)
    ui.ClearButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ClearButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ClearButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Status = uimd.NewLabel("status", "Ready")
    ui.AddElement(ui.Status)
    ui.Status.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.Status.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *ActivityFeedUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ActivityFeedUI) HandleGeneratedButton(name string) bool {
    if name == "settings_button" {
        if handler, ok := ui.eventHandler.(interface{ OnSettingsButtonClick() }); ok {
            handler.OnSettingsButtonClick()
            return true
        }
        return false
    }
    if name == "quit_button" {
        if handler, ok := ui.eventHandler.(interface{ OnQuitButtonClick() }); ok {
            handler.OnQuitButtonClick()
            return true
        }
        return false
    }
    if name == "add_button" {
        if handler, ok := ui.eventHandler.(interface{ OnAddButtonClick() }); ok {
            handler.OnAddButtonClick()
            return true
        }
        return false
    }
    if name == "seed_button" {
        if handler, ok := ui.eventHandler.(interface{ OnSeedButtonClick() }); ok {
            handler.OnSeedButtonClick()
            return true
        }
        return false
    }
    if name == "clear_button" {
        if handler, ok := ui.eventHandler.(interface{ OnClearButtonClick() }); ok {
            handler.OnClearButtonClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ActivityFeedUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "message" {
        if handler, ok := ui.eventHandler.(interface{ OnMessageChange(string) }); ok {
            handler.OnMessageChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ActivityFeedUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "message" {
        if handler, ok := ui.eventHandler.(interface{ OnMessageSubmit(string) }); ok {
            handler.OnMessageSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ActivityFeedUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "activity_type" {
        if handler, ok := ui.eventHandler.(interface{ OnActivityTypeChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnActivityTypeChange(selected)
            return true
        }
        return false
    }
    return false
}
