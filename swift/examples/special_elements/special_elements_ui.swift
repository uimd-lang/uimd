// Auto-generated UI code for special_elements - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class SpecialElementsUI: GeneratedWindowBase
{
    public var header_label: Label!
    public var info_processing: Label!
    public var info_loading: Label!
    public var info_complete: Label!
    public var info_warning: Label!
    public var info_multiline: Label!
    public var status_label: Label!
    public var normal_info: Label!
    public var speed_label: Label!
    public var raw_label: Label!
    public var raw_button: Button!
    public var raw_combo: ComboBox!
    public var raw_list: ListBox!
    public var raw_input: TextInput!
    public var raw_area: TextArea!

    public init()
    {
        super.init("Special Elements Demo")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "SpecialElements", source: "# Special Elements Demo\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_label:\n  type: label\n  text: \"Special UI Elements\"\n  description: \"Window title.\"\n\ninfo_processing:\n  type: label\n  text: \"Processing request...\"\n  color: \"#ff0000\"\n  description: \"Animated processing status label.\"\n\ninfo_loading:\n  type: label\n  text: \"Loading data from server...\"\n  color: \"#00ff00\"\n  description: \"Animated loading status label.\"\n\ninfo_complete:\n  type: label\n  text: \"Operation completed successfully!\"\n  color: \"#0000ff\"\n  description: \"Animated completed status label.\"\n\ninfo_warning:\n  type: label\n  text: \"Warning: Low disk space detected\"\n  color: \"#ffffff\"\n  description: \"Animated warning status label.\"\n\ninfo_multiline:\n  type: label\n  text: \"Line one of animated text\\nLine two with different phase\\nLine three final row\"\n  description: \"Multiline animated label with different text rows.\"\n\nstatus_label:\n  type: label\n  text: \"Status: Idle\"\n  description: \"Current status text.\"\n\nnormal_info:\n  type: label\n  text: \"Regular static label for comparison\"\n  description: \"Regular static comparison label.\"\n\nspeed_label:\n  type: label\n  text: \"Fast animation speed demo\"\n  description: \"Fast animation speed demonstration label.\"\n\nraw_label:\n  type: label\n  text: \"Label tab\\tcontrol\\x1b bell\\x07 combining e\\u0301 wide\\u4e2d\"\n  user-select: text\n  description: \"Label containing tab, control, combining, and wide characters.\"\n\nraw_button:\n  type: button\n  title: \"Button\\tESC\\x1bWide\\u4e2d\"\n  description: \"Button title containing raw terminal-sensitive characters.\"\n\nraw_combo:\n  type: combobox\n  options: [\"Plain option\", \"Tab\\toption\", \"Esc\\x1boption\", \"Wide\\u4e2doption\"]\n  selected_item: \"Tab\\toption\"\n  description: \"ComboBox options containing raw terminal-sensitive characters.\"\n\nraw_list:\n  type: listbox\n  options: [\"Plain row\", \"Tab\\trow\", \"Esc\\x1brow\", \"Wide\\u4e2drow\"]\n  selected_items: [\"Tab\\trow\"]\n  description: \"ListBox options containing raw terminal-sensitive characters.\"\n\nraw_input:\n  type: textinput\n  value: \"Input\\tvalue\\x1bwide\\u4e2d\"\n  maxlength: 80\n  description: \"Text input containing raw terminal-sensitive characters.\"\n\nraw_area:\n  type: textarea\n  value: \"Python tabs example:\\ndef hello(name):\\n\\tif name:\\n\\t\\tprint(\\\"Hello, \\\" + name)\\n\\telse:\\n\\t\\tprint(\\\"Hello, world!\\\")\\n\\nControl\\x1bplaceholder\\nWide\\u4e2dplaceholder\"\n  maxlength: 240\n  description: \"Text area containing multiline raw terminal-sensitive characters.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\n@info_processing:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_loading:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_complete:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_warning:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_multiline:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@speed_label:\n  text-color-gradient:\n    interval: 35\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\ntextinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nlistbox:\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------+\n|                                                                    |\n|  header_label....................................................  |\n|                                                                    |\n|  info_processing................  info_loading...................  |\n|  info_complete..................  info_warning...................  |\n|                                                                    |\n|  \"Multiline animated text:\"                                         |\n|  info_multiline..................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  status_label...................  normal_info....................  |\n|  speed_label.....................................................  |\n|                                                                    |\n|  \"Raw read-only text:\"                                             |\n|  raw_label.......................................................  |\n|                                                                    |\n|  \"Raw button:\" raw_button................                          |\n|                                                                    |\n|  \"Raw combo:\"  raw_combo......................                     |\n|                                                                    |\n|  \"Raw list:\"                                                       |\n|  raw_list........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  \"Text input:\" raw_input.........................................  |\n|                                                                    |\n|  \"Text area:\"                                                      |\n|  raw_area........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n+--------------------------------------------------------------------+\n```\n", description: "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header_label = addElement(Label("header_label", "Special UI Elements"))
        header_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        header_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        info_processing = addElement(Label("info_processing", "Processing request..."))
        info_processing.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ff0000")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }())
        info_processing.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        info_loading = addElement(Label("info_loading", "Loading data from server..."))
        info_loading.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#00ff00")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }())
        info_loading.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        info_complete = addElement(Label("info_complete", "Operation completed successfully!"))
        info_complete.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#0000ff")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }())
        info_complete.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        info_warning = addElement(Label("info_warning", "Warning: Low disk space detected"))
        info_warning.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }())
        info_warning.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        info_multiline = addElement(Label("info_multiline", "Line one of animated text\nLine two with different phase\nLine three final row"))
        info_multiline.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }())
        info_multiline.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        status_label = addElement(Label("status_label", "Status: Idle"))
        status_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        status_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        normal_info = addElement(Label("normal_info", "Regular static label for comparison"))
        normal_info.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        normal_info.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        speed_label = addElement(Label("speed_label", "Fast animation speed demo"))
        speed_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textColorGradient = TextGradient(intervalMs: 35, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }())
        speed_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        raw_label = addElement(Label("raw_label", "Label tab\tcontrol\u{1b} bell\u{07} combining é wide中"))
        raw_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        raw_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        raw_button = addElement(Button("raw_button", "Button\tESC\u{1b}Wide中"))
        raw_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        raw_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        raw_combo = addElement(ComboBox("raw_combo", ["Plain option", "Tab\toption", "Esc\u{1b}option", "Wide中option"], selectedIndex: 1))
        raw_combo.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        raw_combo.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_combo.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        raw_combo.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_combo.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        raw_list = addElement(ListBox("raw_list", ["Plain row", "Tab\trow", "Esc\u{1b}row", "Wide中row"], selectedValues: ["Tab\trow"]))
        raw_list.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        raw_list.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1d2f4d")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_list.setEditStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        raw_list.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_list.setActiveStyle({
                    var style = Style()
                    style.background = Color("#DDDDDD99")
                    style.color = Color("#FFFFFF")
                    return style
                }())
        raw_list.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        raw_input = addElement(TextInput("raw_input", "Input\tvalue\u{1b}wide中", maxLength: 80))
        raw_input.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        raw_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        raw_input.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        raw_area = addElement(TextArea("raw_area", "Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint(\"Hello, \" + name)\n\telse:\n\t\tprint(\"Hello, world!\")\n\nControl\u{1b}placeholder\nWide中placeholder", maxLength: 240))
        raw_area.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        raw_area.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_area.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_area.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        raw_area.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        raw_area.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "header_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 1, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 64, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 32,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "info_processing",
                type: "label",
                cellName: "",
                relative: Rect(row: 3, col: 2, width: 31, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .auto(31),
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 35,
                marginBottom: 30,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "info_loading",
                type: "label",
                cellName: "",
                relative: Rect(row: 3, col: 35, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 30,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "info_complete",
                type: "label",
                cellName: "",
                relative: Rect(row: 4, col: 2, width: 31, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .auto(31),
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 35,
                marginBottom: 29,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "info_warning",
                type: "label",
                cellName: "",
                relative: Rect(row: 4, col: 35, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 29,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Multiline animated text:\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 6, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 40,
                marginBottom: 27,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "info_multiline",
                type: "label",
                cellName: "",
                relative: Rect(row: 7, col: 2, width: 536870911, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(3),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 64, height: 3),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 24,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textColorGradient = TextGradient(intervalMs: 70, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 11, col: 2, width: 31, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .auto(31),
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 35,
                marginBottom: 22,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "normal_info",
                type: "label",
                cellName: "",
                relative: Rect(row: 11, col: 35, width: 536870911, height: 2),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(2),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 31, height: 2),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 21,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "speed_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 12, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 64, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 21,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textColorGradient = TextGradient(intervalMs: 35, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Raw read-only text:\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 14, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 21, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 45,
                marginBottom: 19,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "raw_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 15, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 64, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 18,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Raw button:\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 17, col: 2, width: 13, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .auto(13),
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 53,
                marginBottom: 16,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "raw_button",
                type: "button",
                cellName: "",
                relative: Rect(row: 17, col: 16, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 26,
                marginBottom: 16,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Raw combo:\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 19, col: 2, width: 12, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 54,
                marginBottom: 14,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "raw_combo",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 19, col: 16, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 21,
                marginBottom: 14,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Raw list:\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 21, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 55,
                marginBottom: 12,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "raw_list",
                type: "listbox",
                cellName: "",
                relative: Rect(row: 22, col: 2, width: 536870911, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(3),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 64, height: 3),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Text input:\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 26, col: 2, width: 13, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .auto(13),
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 53,
                marginBottom: 7,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "raw_input",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 26, col: 16, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 50, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 7,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Text area:\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 28, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 54,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "raw_area",
                type: "textarea",
                cellName: "",
                relative: Rect(row: 29, col: 2, width: 536870911, height: 4),
                sourceCell: Rect(row: 0, col: 0, width: 68, height: 34),
                width: .expanded,
                height: .auto(4),
                cellWidth: .auto(68),
                cellHeight: .auto(34),
                charsSize: Size(width: 64, height: 4),
                cellCharsSize: Size(width: 68, height: 34),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: Style()
            ),
        ]
    }

    open func onRawButtonClick() { }
    open func onRawComboChange(_ value: String) { }
    open func onRawListSelectionChange(_ value: [String]) { }
    open func onRawListItemActivate(_ index: Int, value: String) -> Bool { false }
    open func onRawInputChange(_ value: String) { }
    open func onRawInputSubmit(_ value: String) { }
    open func onRawAreaChange(_ value: String) { }
    open func onRawAreaSubmit(_ value: String) { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "raw_button"
        {
            onRawButtonClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "raw_input"
        {
            onRawInputChange(value)
            return true
        }
        else if name == "raw_area"
        {
            onRawAreaChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "raw_input"
        {
            onRawInputSubmit(value)
            return true
        }
        else if name == "raw_area"
        {
            onRawAreaSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "raw_combo"
        {
            onRawComboChange(value.isEmpty ? "" : value[0])
            return true
        }
        else if name == "raw_list"
        {
            onRawListSelectionChange(value)
            return true
        }
        return false
    }

    open override func handleGeneratedListBoxItemActivate(_ name: String, elementId: String, index: Int, value: String) -> Bool
    {
        if name == "raw_list"
        {
            return onRawListItemActivate(index, value: value)
        }
        return false
    }
}
