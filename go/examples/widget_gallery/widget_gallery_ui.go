// Auto-generated UI code for widget_gallery - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type WidgetGalleryUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Header *uimd.Label
    CloseBtn *uimd.Button
    PlainLabel *uimd.Label
    SelectableLabel *uimd.Label
    Spans *uimd.SpanLabel
    Animation *uimd.Label
    NameLabel *uimd.Label
    NameInput *uimd.TextInput
    CountLabel *uimd.Label
    CountInput *uimd.NumberInput
    EnabledCheck *uimd.CheckBox
    ThemeLabel *uimd.Label
    ThemeCombo *uimd.ComboBox
    ModeLabel *uimd.Label
    ModeList *uimd.ListBox
    ApplyBtn *uimd.Button
    ResetBtn *uimd.Button
    Summary *uimd.Label
}

func NewWidgetGalleryUI() *WidgetGalleryUI {
    ui := &WidgetGalleryUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Widget Gallery")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 1,
            CellName: "title", CellWidth: 54, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 38, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 38, CharsHeight: 1,
            MarginRight: 15, MarginBottom: 0,
            Content: "header",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#18324f"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#18324f"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 2,
            CellRow: 12, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 9,
            CellName: "control_section", CellWidth: 27, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 15, MarginBottom: 8,
            Content: "name_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 0, Col: 2,
            CellRow: 12, CellCol: 28,
            CellCharsWidth: 26, CellCharsHeight: 9,
            CellName: "section2", CellWidth: 26, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 13, MarginBottom: 8,
            Content: "theme_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 0, Col: 40,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 1,
            CellName: "title", CellWidth: 54, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "close_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#18324f"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#ffffff"),
    },
        },
        {
            Row: 1, Col: 2,
            CellRow: 12, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 9,
            CellName: "control_section", CellWidth: 27, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 23, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 7,
            Content: "name_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 1, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 9,
            CellName: "label_section", CellWidth: 54, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 50, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 7,
            Content: "plain_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 2,
            CellRow: 22, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 10,
            CellName: "", CellWidth: 54, CellHeight: 10,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 6,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 50, CharsHeight: 6,
            MarginRight: 2, MarginBottom: 3,
            Content: "summary",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#dbeafe"),
    },
        },
        {
            Row: 1, Col: 3,
            CellRow: 12, CellCol: 28,
            CellCharsWidth: 26, CellCharsHeight: 9,
            CellName: "section2", CellWidth: 26, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 21, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 7,
            Content: "theme_combo",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 12, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 9,
            CellName: "control_section", CellWidth: 27, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 14, MarginBottom: 5,
            Content: "count_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 12, CellCol: 28,
            CellCharsWidth: 26, CellCharsHeight: 9,
            CellName: "section2", CellWidth: 26, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 14, MarginBottom: 5,
            Content: "mode_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 9,
            CellName: "label_section", CellWidth: 54, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 50, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 5,
            Content: "selectable_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#a7f3d0"),
        UserSelect: "text",
    },
        },
        {
            Row: 4, Col: 2,
            CellRow: 12, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 9,
            CellName: "control_section", CellWidth: 27, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 23, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 4,
            Content: "count_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 4, Col: 2,
            CellRow: 12, CellCol: 28,
            CellCharsWidth: 26, CellCharsHeight: 9,
            CellName: "section2", CellWidth: 26, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 5,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 22, CharsHeight: 5,
            MarginRight: 2, MarginBottom: 0,
            Content: "mode_list",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 5, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 9,
            CellName: "label_section", CellWidth: 54, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 50, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 3,
            Content: "spans",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#cbd5e1"),
        UserSelect: "text",
    },
        },
        {
            Row: 6, Col: 2,
            CellRow: 12, CellCol: 0,
            CellCharsWidth: 27, CellCharsHeight: 9,
            CellName: "control_section", CellWidth: 27, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 23, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 2,
            Content: "enabled_check",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    },
        },
        {
            Row: 7, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 9,
            CellName: "label_section", CellWidth: 54, CellHeight: 9,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 50, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 1,
            Content: "animation",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#fbbf24"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 100, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    },
        },
        {
            Row: 8, Col: 2,
            CellRow: 22, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 10,
            CellName: "", CellWidth: 54, CellHeight: 10,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 23, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 23, CharsHeight: 1,
            MarginRight: 29, MarginBottom: 1,
            Content: "apply_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 8, Col: 27,
            CellRow: 22, CellCol: 0,
            CellCharsWidth: 54, CellCharsHeight: 10,
            CellName: "", CellWidth: 54, CellHeight: 10,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 25, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 1,
            Content: "reset_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
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
    ui.SetMCPMetadata(true, "WidgetGallery", "# Widget Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Widget Gallery\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the widget gallery.\"\n\nplain_label:\n  type: label\n  text: \"Plain label with inherited theme colors.\"\n  description: \"Plain label using inherited theme colors.\"\n\nselectable_label:\n  type: label\n  text: \"Select this text with the mouse to test label selection.\"\n  description: \"Selectable label used to test mouse text selection.\"\n\nspans:\n  type: spanlabel\n  text: \"span label\"\n  description: \"Span label demonstrating mixed foreground and background styling.\"\n\nanimation:\n  type: label\n  text: \"Animated label shows gradient-based rendering.\"\n  description: \"Animated gradient label.\"\n\nname_label:\n  type: label\n  text: Name\n  description: \"Label for the name input.\"\n\nname_input:\n  type: textinput\n  value: \"Ada Lovelace\"\n  maxlength: 80\n  description: \"Editable name text input.\"\n\ncount_label:\n  type: label\n  text: Count\n  description: \"Label for the count input.\"\n\ncount_input:\n  type: numberinput\n  value: 3\n  step_size: 1\n  min_value: 0\n  max_value: 99\n  format_str: 0\n  description: \"Numeric count input with min, max, and step constraints.\"\n\nenabled_check:\n  type: checkbox\n  title: \"Feature enabled\"\n  value: 1\n  description: \"Toggle for the feature enabled state.\"\n\ntheme_label:\n  type: label\n  text: Theme\n  description: \"Label for the theme selector.\"\n\ntheme_combo:\n  type: combobox\n  options: [Dark, Light, Solarized]\n  selected_item: Dark\n  description: \"Combobox selecting a visual theme value.\"\n\nmode_label:\n  type: label\n  text: Mode\n  description: \"Label for the mode list.\"\n\nmode_list:\n  type: listbox\n  options: [Preview, Edit, Review, Publish, Archive]\n  selected_items: [Preview]\n  multiple: \"false\"\n  description: \"Single-select listbox choosing the current workflow mode.\"\n\napply_btn:\n  type: button\n  title: Apply\n  description: \"Apply the current widget values.\"\n\nreset_btn:\n  type: button\n  title: Reset\n  description: \"Reset widget values to defaults.\"\n\nsummary:\n  type: label\n  text: \"values:\\n  name: Ada Lovelace\\n  count: 3\\n  theme: Dark\\n  mode: Preview\\n  enabled: true\"\n  description: \"Read-only summary of current widget values.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#18324f\"\n  color: \"#ffffff\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@title:\n  background: \"#18324f\"\n@selectable_label:\n  user-select: text\n  color: \"#a7f3d0\"\n@spans:\n  user-select: text\n  color: \"#cbd5e1\"\n@animation:\n  color: \"#fbbf24\"\n  text-color-gradient:\n    interval: 100\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\ntextinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nnumberinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncheckbox:\n  color: \"#d1d5db\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#f97316\"\n@summary:\n  background: \"#0d1524\"\n  color: \"#dbeafe\"\n```\n\n## User Interface\n\n```ui\n+-title------------------------------------------------+\n| header................................ close_btn.....|\n+-label_section----------------------------------------+\n|                                                      |\n|  plain_label.......................................  |\n|                                                      |\n|  selectable_label..................................  |\n|                                                      |\n|  spans.............................................  |\n|                                                      |\n|  animation.........................................  |\n|                                                      |\n+-control_section-----------+--section2----------------+\n|  name_label               |  theme_label             |\n|  name_input.............  |   theme_combo..........  |\n|                           |                          |\n|  count_label              |  mode_label              |\n|  count_input............  |  mode_list.............  |\n|                           |  ......................  |\n|  enabled_check..........  |  ......................  |\n|                           |  ......................  |\n|                           |  ......................  |\n+---------------------------+--------------------------+\n|                                                      |\n|  summary...........................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|                                                      |\n|  apply_btn..............  reset_btn................  |\n|                                                      |\n+------------------------------------------------------+\n```\n", "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.")
    ui.Header = uimd.NewLabel("header", "Widget Gallery")
    ui.AddElement(ui.Header)
    ui.Header.SetStyle(uimd.Style{
        Background: uimd.NewColor("#18324f"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.Header.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.CloseBtn = uimd.NewButton("close_btn", "Quit")
    ui.AddElement(ui.CloseBtn)
    ui.CloseBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CloseBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CloseBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.PlainLabel = uimd.NewLabel("plain_label", "Plain label with inherited theme colors.")
    ui.AddElement(ui.PlainLabel)
    ui.PlainLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.PlainLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.SelectableLabel = uimd.NewLabel("selectable_label", "Select this text with the mouse to test label selection.")
    ui.AddElement(ui.SelectableLabel)
    ui.SelectableLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#a7f3d0"),
        UserSelect: "text",
    })
    ui.SelectableLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Spans = uimd.NewSpanLabel("spans", "span label")
    ui.AddElement(ui.Spans)
    ui.Spans.SetStyle(uimd.Style{
        Color: uimd.NewColor("#cbd5e1"),
        UserSelect: "text",
    })
    ui.Animation = uimd.NewLabel("animation", "Animated label shows gradient-based rendering.")
    ui.AddElement(ui.Animation)
    ui.Animation.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#fbbf24"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 100, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    })
    ui.Animation.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.NameLabel = uimd.NewLabel("name_label", "Name")
    ui.AddElement(ui.NameLabel)
    ui.NameLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.NameLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.NameInput = uimd.NewTextInput("name_input", "Ada Lovelace", 0)
    ui.AddElement(ui.NameInput)
    ui.NameInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.NameInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NameInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
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
    ui.CountLabel = uimd.NewLabel("count_label", "Count")
    ui.AddElement(ui.CountLabel)
    ui.CountLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.CountLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.CountInput = uimd.NewNumberInput("count_input", 3.000000)
    ui.AddElement(ui.CountInput)
    ui.CountInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.CountInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CountInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CountInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.CountInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.EnabledCheck = uimd.NewCheckBox("enabled_check", "Feature enabled", true)
    ui.AddElement(ui.EnabledCheck)
    ui.EnabledCheck.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
    })
    ui.EnabledCheck.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.EnabledCheck.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#22c55e"),
    })
    ui.EnabledCheck.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f97316"),
    })
    ui.EnabledCheck.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.ThemeLabel = uimd.NewLabel("theme_label", "Theme")
    ui.AddElement(ui.ThemeLabel)
    ui.ThemeLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.ThemeLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ThemeCombo = uimd.NewComboBox("theme_combo", []string{"Dark", "Light", "Solarized"})
    ui.AddElement(ui.ThemeCombo)
    ui.ThemeCombo.SetSelectedIndex(0)
    ui.ThemeCombo.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ThemeCombo.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ThemeCombo.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.ThemeCombo.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ThemeCombo.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ModeLabel = uimd.NewLabel("mode_label", "Mode")
    ui.AddElement(ui.ModeLabel)
    ui.ModeLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.ModeLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ModeList = uimd.NewListBox("mode_list", []string{"Preview", "Edit", "Review", "Publish", "Archive"}, false)
    ui.AddElement(ui.ModeList)
    ui.ModeList.SetSelectedIndex(0)
    ui.ModeList.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.ModeList.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1d2f4d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ModeList.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.ModeList.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ModeList.SetActiveStyle(uimd.Style{
        Background: uimd.NewColor("#DDDDDD99"),
        Color: uimd.NewColor("#FFFFFF"),
    })
    ui.ModeList.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.ApplyBtn = uimd.NewButton("apply_btn", "Apply")
    ui.AddElement(ui.ApplyBtn)
    ui.ApplyBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ApplyBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ApplyBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ResetBtn = uimd.NewButton("reset_btn", "Reset")
    ui.AddElement(ui.ResetBtn)
    ui.ResetBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ResetBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ResetBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Summary = uimd.NewLabel("summary", "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true")
    ui.AddElement(ui.Summary)
    ui.Summary.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#dbeafe"),
    })
    ui.Summary.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *WidgetGalleryUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *WidgetGalleryUI) HandleGeneratedButton(name string) bool {
    if name == "close_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnCloseBtnClick() }); ok {
            handler.OnCloseBtnClick()
            return true
        }
        return false
    }
    if name == "apply_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnApplyBtnClick() }); ok {
            handler.OnApplyBtnClick()
            return true
        }
        return false
    }
    if name == "reset_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnResetBtnClick() }); ok {
            handler.OnResetBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *WidgetGalleryUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "name_input" {
        if handler, ok := ui.eventHandler.(interface{ OnNameInputChange(string) }); ok {
            handler.OnNameInputChange(value)
            return true
        }
        return false
    }
    if name == "count_input" {
        if handler, ok := ui.eventHandler.(interface{ OnCountInputChange(string) }); ok {
            handler.OnCountInputChange(value)
            return true
        }
        return false
    }
    if name == "enabled_check" {
        if handler, ok := ui.eventHandler.(interface{ OnEnabledCheckChange(string) }); ok {
            handler.OnEnabledCheckChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *WidgetGalleryUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "name_input" {
        if handler, ok := ui.eventHandler.(interface{ OnNameInputSubmit(string) }); ok {
            handler.OnNameInputSubmit(value)
            return true
        }
        return false
    }
    if name == "count_input" {
        if handler, ok := ui.eventHandler.(interface{ OnCountInputSubmit(string) }); ok {
            handler.OnCountInputSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *WidgetGalleryUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "theme_combo" {
        if handler, ok := ui.eventHandler.(interface{ OnThemeComboChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnThemeComboChange(selected)
            return true
        }
        return false
    }
    if name == "mode_list" {
        if handler, ok := ui.eventHandler.(interface{ OnModeListSelectionChange([]string) }); ok {
            handler.OnModeListSelectionChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *WidgetGalleryUI) HandleGeneratedListBoxItemActivate(name string, elementID string, index int, value string) bool {
    _ = elementID
    if name == "mode_list" {
        if handler, ok := ui.eventHandler.(interface{ OnModeListItemActivate(int, string) bool }); ok {
            return handler.OnModeListItemActivate(index, value)
        }
        return false
    }
    return false
}

func (ui *WidgetGalleryUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *WidgetGalleryUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
