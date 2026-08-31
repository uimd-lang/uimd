// Auto-generated UI code for special_elements - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type SpecialElementsUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    HeaderLabel *uimd.Label
    InfoProcessing *uimd.Label
    InfoLoading *uimd.Label
    InfoComplete *uimd.Label
    InfoWarning *uimd.Label
    InfoMultiline *uimd.Label
    StatusLabel *uimd.Label
    NormalInfo *uimd.Label
    SpeedLabel *uimd.Label
    RawLabel *uimd.Label
    RawButton *uimd.Button
    RawCombo *uimd.ComboBox
    RawList *uimd.ListBox
    RawInput *uimd.TextInput
    RawArea *uimd.TextArea
}

func NewSpecialElementsUI() *SpecialElementsUI {
    ui := &SpecialElementsUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Special Elements Demo")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 1, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 64, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 32,
            Content: "header_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 31, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 35, MarginBottom: 30,
            Content: "info_processing",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    },
        },
        {
            Row: 3, Col: 35,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 30,
            Content: "info_loading",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    },
        },
        {
            Row: 4, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 31, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 35, MarginBottom: 29,
            Content: "info_complete",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    },
        },
        {
            Row: 4, Col: 35,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 29,
            Content: "info_warning",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    },
        },
        {
            Row: 6, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 40, MarginBottom: 27,
            Content: "\"Multiline animated text:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 7, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 3,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 64, CharsHeight: 3,
            MarginRight: 2, MarginBottom: 24,
            Content: "info_multiline",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    },
        },
        {
            Row: 11, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 31, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 35, MarginBottom: 22,
            Content: "status_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 11, Col: 35,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 2,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 2,
            MarginRight: 2, MarginBottom: 21,
            Content: "normal_info",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 12, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 64, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 21,
            Content: "speed_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 35, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    },
        },
        {
            Row: 14, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 21, CharsHeight: 1,
            MarginRight: 45, MarginBottom: 19,
            Content: "\"Raw read-only text:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 15, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 64, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 18,
            Content: "raw_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 17, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 13, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 53, MarginBottom: 16,
            Content: "\"Raw button:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 17, Col: 16,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 26, MarginBottom: 16,
            Content: "raw_button",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 19, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 54, MarginBottom: 14,
            Content: "\"Raw combo:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 19, Col: 16,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 21, MarginBottom: 14,
            Content: "raw_combo",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 21, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 55, MarginBottom: 12,
            Content: "\"Raw list:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 22, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 3,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 64, CharsHeight: 3,
            MarginRight: 2, MarginBottom: 9,
            Content: "raw_list",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 26, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 13, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 53, MarginBottom: 7,
            Content: "\"Text input:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 26, Col: 16,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 50, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 7,
            Content: "raw_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 28, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 54, MarginBottom: 5,
            Content: "\"Text area:\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 29, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 68, CellCharsHeight: 34,
            CellName: "", CellWidth: 68, CellHeight: 34,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 4,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 64, CharsHeight: 4,
            MarginRight: 2, MarginBottom: 1,
            Content: "raw_area",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.NewStyle(),
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
    ui.SetMCPMetadata(true, "SpecialElements", "# Special Elements Demo\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_label:\n  type: label\n  text: \"Special UI Elements\"\n  description: \"Window title.\"\n\ninfo_processing:\n  type: label\n  text: \"Processing request...\"\n  color: \"#ff0000\"\n  description: \"Animated processing status label.\"\n\ninfo_loading:\n  type: label\n  text: \"Loading data from server...\"\n  color: \"#00ff00\"\n  description: \"Animated loading status label.\"\n\ninfo_complete:\n  type: label\n  text: \"Operation completed successfully!\"\n  color: \"#0000ff\"\n  description: \"Animated completed status label.\"\n\ninfo_warning:\n  type: label\n  text: \"Warning: Low disk space detected\"\n  color: \"#ffffff\"\n  description: \"Animated warning status label.\"\n\ninfo_multiline:\n  type: label\n  text: \"Line one of animated text\\nLine two with different phase\\nLine three final row\"\n  description: \"Multiline animated label with different text rows.\"\n\nstatus_label:\n  type: label\n  text: \"Status: Idle\"\n  description: \"Current status text.\"\n\nnormal_info:\n  type: label\n  text: \"Regular static label for comparison\"\n  description: \"Regular static comparison label.\"\n\nspeed_label:\n  type: label\n  text: \"Fast animation speed demo\"\n  description: \"Fast animation speed demonstration label.\"\n\nraw_label:\n  type: label\n  text: \"Label tab\\tcontrol\\x1b bell\\x07 combining e\\u0301 wide\\u4e2d\"\n  user-select: text\n  description: \"Label containing tab, control, combining, and wide characters.\"\n\nraw_button:\n  type: button\n  title: \"Button\\tESC\\x1bWide\\u4e2d\"\n  description: \"Button title containing raw terminal-sensitive characters.\"\n\nraw_combo:\n  type: combobox\n  options: [\"Plain option\", \"Tab\\toption\", \"Esc\\x1boption\", \"Wide\\u4e2doption\"]\n  selected_item: \"Tab\\toption\"\n  description: \"ComboBox options containing raw terminal-sensitive characters.\"\n\nraw_list:\n  type: listbox\n  options: [\"Plain row\", \"Tab\\trow\", \"Esc\\x1brow\", \"Wide\\u4e2drow\"]\n  selected_items: [\"Tab\\trow\"]\n  description: \"ListBox options containing raw terminal-sensitive characters.\"\n\nraw_input:\n  type: textinput\n  value: \"Input\\tvalue\\x1bwide\\u4e2d\"\n  maxlength: 80\n  description: \"Text input containing raw terminal-sensitive characters.\"\n\nraw_area:\n  type: textarea\n  value: \"Python tabs example:\\ndef hello(name):\\n\\tif name:\\n\\t\\tprint(\\\"Hello, \\\" + name)\\n\\telse:\\n\\t\\tprint(\\\"Hello, world!\\\")\\n\\nControl\\x1bplaceholder\\nWide\\u4e2dplaceholder\"\n  maxlength: 240\n  description: \"Text area containing multiline raw terminal-sensitive characters.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\n@info_processing:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_loading:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_complete:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_warning:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_multiline:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@speed_label:\n  text-color-gradient:\n    interval: 35\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\ntextinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nlistbox:\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------+\n|                                                                    |\n|  header_label....................................................  |\n|                                                                    |\n|  info_processing................  info_loading...................  |\n|  info_complete..................  info_warning...................  |\n|                                                                    |\n|  \"Multiline animated text:\"                                         |\n|  info_multiline..................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  status_label...................  normal_info....................  |\n|  speed_label.....................................................  |\n|                                                                    |\n|  \"Raw read-only text:\"                                             |\n|  raw_label.......................................................  |\n|                                                                    |\n|  \"Raw button:\" raw_button................                          |\n|                                                                    |\n|  \"Raw combo:\"  raw_combo......................                     |\n|                                                                    |\n|  \"Raw list:\"                                                       |\n|  raw_list........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  \"Text input:\" raw_input.........................................  |\n|                                                                    |\n|  \"Text area:\"                                                      |\n|  raw_area........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n+--------------------------------------------------------------------+\n```\n", "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.")
    ui.HeaderLabel = uimd.NewLabel("header_label", "Special UI Elements")
    ui.AddElement(ui.HeaderLabel)
    ui.HeaderLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.HeaderLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.InfoProcessing = uimd.NewLabel("info_processing", "Processing request...")
    ui.AddElement(ui.InfoProcessing)
    ui.InfoProcessing.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ff0000"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    })
    ui.InfoProcessing.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.InfoLoading = uimd.NewLabel("info_loading", "Loading data from server...")
    ui.AddElement(ui.InfoLoading)
    ui.InfoLoading.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#00ff00"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    })
    ui.InfoLoading.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.InfoComplete = uimd.NewLabel("info_complete", "Operation completed successfully!")
    ui.AddElement(ui.InfoComplete)
    ui.InfoComplete.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#0000ff"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    })
    ui.InfoComplete.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.InfoWarning = uimd.NewLabel("info_warning", "Warning: Low disk space detected")
    ui.AddElement(ui.InfoWarning)
    ui.InfoWarning.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    })
    ui.InfoWarning.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.InfoMultiline = uimd.NewLabel("info_multiline", "Line one of animated text\nLine two with different phase\nLine three final row")
    ui.AddElement(ui.InfoMultiline)
    ui.InfoMultiline.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 70, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    })
    ui.InfoMultiline.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.StatusLabel = uimd.NewLabel("status_label", "Status: Idle")
    ui.AddElement(ui.StatusLabel)
    ui.StatusLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.StatusLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.NormalInfo = uimd.NewLabel("normal_info", "Regular static label for comparison")
    ui.AddElement(ui.NormalInfo)
    ui.NormalInfo.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.NormalInfo.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.SpeedLabel = uimd.NewLabel("speed_label", "Fast animation speed demo")
    ui.AddElement(ui.SpeedLabel)
    ui.SpeedLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 35, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#00000099"), uimd.NewColor("#00000066"), uimd.NewColor("#00000033"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000033"), uimd.NewColor("#00000066"), uimd.NewColor("#00000099")}},
    })
    ui.SpeedLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.RawLabel = uimd.NewLabel("raw_label", "Label tab\tcontrol\x1b bell\x07 combining é wide中")
    ui.AddElement(ui.RawLabel)
    ui.RawLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.RawLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.RawButton = uimd.NewButton("raw_button", "Button\tESC\x1bWide中")
    ui.AddElement(ui.RawButton)
    ui.RawButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.RawButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.RawCombo = uimd.NewComboBox("raw_combo", []string{"Plain option", "Tab\toption", "Esc\x1boption", "Wide中option"})
    ui.AddElement(ui.RawCombo)
    ui.RawCombo.SetSelectedIndex(1)
    ui.RawCombo.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.RawCombo.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawCombo.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.RawCombo.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawCombo.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.RawList = uimd.NewListBox("raw_list", []string{"Plain row", "Tab\trow", "Esc\x1brow", "Wide中row"}, false)
    ui.AddElement(ui.RawList)
    ui.RawList.SetSelectedIndex(1)
    ui.RawList.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.RawList.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1d2f4d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawList.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.RawList.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawList.SetActiveStyle(uimd.Style{
        Background: uimd.NewColor("#DDDDDD99"),
        Color: uimd.NewColor("#FFFFFF"),
    })
    ui.RawList.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.RawInput = uimd.NewTextInput("raw_input", "Input\tvalue\x1bwide中", 0)
    ui.AddElement(ui.RawInput)
    ui.RawInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.RawInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.RawInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.RawArea = uimd.NewTextArea("raw_area", "Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint(\"Hello, \" + name)\n\telse:\n\t\tprint(\"Hello, world!\")\n\nControl\x1bplaceholder\nWide中placeholder")
    ui.AddElement(ui.RawArea)
    ui.RawArea.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.RawArea.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawArea.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawArea.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.RawArea.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RawArea.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *SpecialElementsUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *SpecialElementsUI) HandleGeneratedButton(name string) bool {
    if name == "raw_button" {
        if handler, ok := ui.eventHandler.(interface{ OnRawButtonClick() }); ok {
            handler.OnRawButtonClick()
            return true
        }
        return false
    }
    return false
}

func (ui *SpecialElementsUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "raw_input" {
        if handler, ok := ui.eventHandler.(interface{ OnRawInputChange(string) }); ok {
            handler.OnRawInputChange(value)
            return true
        }
        return false
    }
    if name == "raw_area" {
        if handler, ok := ui.eventHandler.(interface{ OnRawAreaChange(string) }); ok {
            handler.OnRawAreaChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *SpecialElementsUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "raw_input" {
        if handler, ok := ui.eventHandler.(interface{ OnRawInputSubmit(string) }); ok {
            handler.OnRawInputSubmit(value)
            return true
        }
        return false
    }
    if name == "raw_area" {
        if handler, ok := ui.eventHandler.(interface{ OnRawAreaSubmit(string) }); ok {
            handler.OnRawAreaSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *SpecialElementsUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "raw_combo" {
        if handler, ok := ui.eventHandler.(interface{ OnRawComboChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnRawComboChange(selected)
            return true
        }
        return false
    }
    if name == "raw_list" {
        if handler, ok := ui.eventHandler.(interface{ OnRawListSelectionChange([]string) }); ok {
            handler.OnRawListSelectionChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *SpecialElementsUI) HandleGeneratedListBoxItemActivate(name string, elementID string, index int, value string) bool {
    _ = elementID
    if name == "raw_list" {
        if handler, ok := ui.eventHandler.(interface{ OnRawListItemActivate(int, string) bool }); ok {
            return handler.OnRawListItemActivate(index, value)
        }
        return false
    }
    return false
}

func (ui *SpecialElementsUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *SpecialElementsUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
