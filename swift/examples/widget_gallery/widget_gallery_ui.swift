// Auto-generated UI code for widget_gallery - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class WidgetGalleryUI: GeneratedWindowBase
{
    public var header: Label!
    public var close_btn: Button!
    public var plain_label: Label!
    public var selectable_label: Label!
    public var spans: SpanLabel!
    public var animation: Label!
    public var name_label: Label!
    public var name_input: TextInput!
    public var count_label: Label!
    public var count_input: NumberInput!
    public var enabled_check: CheckBox!
    public var theme_label: Label!
    public var theme_combo: ComboBox!
    public var mode_label: Label!
    public var mode_list: ListBox!
    public var apply_btn: Button!
    public var reset_btn: Button!
    public var summary: Label!

    public init()
    {
        super.init("Widget Gallery")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "WidgetGallery", source: "# Widget Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Widget Gallery\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the widget gallery.\"\n\nplain_label:\n  type: label\n  text: \"Plain label with inherited theme colors.\"\n  description: \"Plain label using inherited theme colors.\"\n\nselectable_label:\n  type: label\n  text: \"Select this text with the mouse to test label selection.\"\n  description: \"Selectable label used to test mouse text selection.\"\n\nspans:\n  type: spanlabel\n  text: \"span label\"\n  description: \"Span label demonstrating mixed foreground and background styling.\"\n\nanimation:\n  type: label\n  text: \"Animated label shows gradient-based rendering.\"\n  description: \"Animated gradient label.\"\n\nname_label:\n  type: label\n  text: Name\n  description: \"Label for the name input.\"\n\nname_input:\n  type: textinput\n  value: \"Ada Lovelace\"\n  maxlength: 80\n  description: \"Editable name text input.\"\n\ncount_label:\n  type: label\n  text: Count\n  description: \"Label for the count input.\"\n\ncount_input:\n  type: numberinput\n  value: 3\n  step_size: 1\n  min_value: 0\n  max_value: 99\n  format_str: 0\n  description: \"Numeric count input with min, max, and step constraints.\"\n\nenabled_check:\n  type: checkbox\n  title: \"Feature enabled\"\n  value: 1\n  description: \"Toggle for the feature enabled state.\"\n\ntheme_label:\n  type: label\n  text: Theme\n  description: \"Label for the theme selector.\"\n\ntheme_combo:\n  type: combobox\n  options: [Dark, Light, Solarized]\n  selected_item: Dark\n  description: \"Combobox selecting a visual theme value.\"\n\nmode_label:\n  type: label\n  text: Mode\n  description: \"Label for the mode list.\"\n\nmode_list:\n  type: listbox\n  options: [Preview, Edit, Review, Publish, Archive]\n  selected_items: [Preview]\n  multiple: \"false\"\n  description: \"Single-select listbox choosing the current workflow mode.\"\n\napply_btn:\n  type: button\n  title: Apply\n  description: \"Apply the current widget values.\"\n\nreset_btn:\n  type: button\n  title: Reset\n  description: \"Reset widget values to defaults.\"\n\nsummary:\n  type: label\n  text: \"values:\\n  name: Ada Lovelace\\n  count: 3\\n  theme: Dark\\n  mode: Preview\\n  enabled: true\"\n  description: \"Read-only summary of current widget values.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#18324f\"\n  color: \"#ffffff\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@title:\n  background: \"#18324f\"\n@selectable_label:\n  user-select: text\n  color: \"#a7f3d0\"\n@spans:\n  user-select: text\n  color: \"#cbd5e1\"\n@animation:\n  color: \"#fbbf24\"\n  text-color-gradient:\n    interval: 100\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\ntextinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nnumberinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncheckbox:\n  color: \"#d1d5db\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#f97316\"\n@summary:\n  background: \"#0d1524\"\n  color: \"#dbeafe\"\n```\n\n## User Interface\n\n```ui\n+-title------------------------------------------------+\n| header................................ close_btn.....|\n+-label_section----------------------------------------+\n|                                                      |\n|  plain_label.......................................  |\n|                                                      |\n|  selectable_label..................................  |\n|                                                      |\n|  spans.............................................  |\n|                                                      |\n|  animation.........................................  |\n|                                                      |\n+-control_section-----------+--section2----------------+\n|  name_label               |  theme_label             |\n|  name_input.............  |   theme_combo..........  |\n|                           |                          |\n|  count_label              |  mode_label              |\n|  count_input............  |  mode_list.............  |\n|                           |  ......................  |\n|  enabled_check..........  |  ......................  |\n|                           |  ......................  |\n|                           |  ......................  |\n+---------------------------+--------------------------+\n|                                                      |\n|  summary...........................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|                                                      |\n|  apply_btn..............  reset_btn................  |\n|                                                      |\n+------------------------------------------------------+\n```\n", description: "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header = addElement(Label("header", "Widget Gallery"))
        header.setStyle({
                    var style = Style()
                    style.background = Color("#18324f")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        header.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        close_btn = addElement(Button("close_btn", "Quit"))
        close_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        plain_label = addElement(Label("plain_label", "Plain label with inherited theme colors."))
        plain_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        plain_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        selectable_label = addElement(Label("selectable_label", "Select this text with the mouse to test label selection."))
        selectable_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#a7f3d0")
                    style.userSelect = "text"
                    return style
                }())
        selectable_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        spans = addElement(SpanLabel("spans", "span label"))
        spans.setStyle({
                    var style = Style()
                    style.color = Color("#cbd5e1")
                    style.userSelect = "text"
                    return style
                }())
        animation = addElement(Label("animation", "Animated label shows gradient-based rendering."))
        animation.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#fbbf24")
                    style.textColorGradient = TextGradient(intervalMs: 100, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }())
        animation.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        name_label = addElement(Label("name_label", "Name"))
        name_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        name_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        name_input = addElement(TextInput("name_input", "Ada Lovelace", maxLength: 80))
        name_input.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        name_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        name_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        name_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        name_input.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        name_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        count_label = addElement(Label("count_label", "Count"))
        count_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        count_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        count_input = addElement(NumberInput("count_input", 3.0, step: 1.0))
        count_input.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        count_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        count_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        count_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        count_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        enabled_check = addElement(CheckBox("enabled_check", "Feature enabled", true))
        enabled_check.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        enabled_check.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#ffffff")
                    return style
                }())
        enabled_check.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#22c55e")
                    return style
                }())
        enabled_check.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f97316")
                    return style
                }())
        enabled_check.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        theme_label = addElement(Label("theme_label", "Theme"))
        theme_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        theme_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        theme_combo = addElement(ComboBox("theme_combo", ["Dark", "Light", "Solarized"], selectedIndex: 0))
        theme_combo.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        theme_combo.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        theme_combo.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        theme_combo.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        theme_combo.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        mode_label = addElement(Label("mode_label", "Mode"))
        mode_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        mode_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        mode_list = addElement(ListBox("mode_list", ["Preview", "Edit", "Review", "Publish", "Archive"], selectedValues: ["Preview"]))
        mode_list.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        mode_list.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1d2f4d")
                    style.color = Color("#ffffff")
                    return style
                }())
        mode_list.setEditStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        mode_list.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        mode_list.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        apply_btn = addElement(Button("apply_btn", "Apply"))
        apply_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        apply_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        apply_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        reset_btn = addElement(Button("reset_btn", "Reset"))
        reset_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        reset_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        reset_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        summary = addElement(Label("summary", "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true"))
        summary.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#dbeafe")
                    return style
                }())
        summary.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "header",
                type: "label",
                cellName: "title",
                relative: Rect(row: 0, col: 1, width: 38, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 1),
                width: .auto(38),
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(1),
                charsSize: Size(width: 38, height: 1),
                cellCharsSize: Size(width: 54, height: 1),
                marginRight: 15,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#18324f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#18324f")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "name_label",
                type: "label",
                cellName: "control_section",
                relative: Rect(row: 0, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 12, col: 0, width: 27, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(9),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 27, height: 9),
                marginRight: 15,
                marginBottom: 8,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "theme_label",
                type: "label",
                cellName: "section2",
                relative: Rect(row: 0, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 12, col: 28, width: 26, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(26),
                cellHeight: .auto(9),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 26, height: 9),
                marginRight: 13,
                marginBottom: 8,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "close_btn",
                type: "button",
                cellName: "title",
                relative: Rect(row: 0, col: 40, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(1),
                charsSize: Size(width: 14, height: 1),
                cellCharsSize: Size(width: 54, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#18324f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#ffffff")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "name_input",
                type: "textinput",
                cellName: "control_section",
                relative: Rect(row: 1, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 12, col: 0, width: 27, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(9),
                charsSize: Size(width: 23, height: 1),
                cellCharsSize: Size(width: 27, height: 9),
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
                name: "plain_label",
                type: "label",
                cellName: "label_section",
                relative: Rect(row: 1, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(9),
                charsSize: Size(width: 50, height: 1),
                cellCharsSize: Size(width: 54, height: 9),
                marginRight: 2,
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
                name: "summary",
                type: "label",
                cellName: "",
                relative: Rect(row: 1, col: 2, width: 536870911, height: 6),
                sourceCell: Rect(row: 22, col: 0, width: 54, height: 10),
                width: .expanded,
                height: .auto(6),
                cellWidth: .auto(54),
                cellHeight: .auto(10),
                charsSize: Size(width: 50, height: 6),
                cellCharsSize: Size(width: 54, height: 10),
                marginRight: 2,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#dbeafe")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "theme_combo",
                type: "combobox",
                cellName: "section2",
                relative: Rect(row: 1, col: 3, width: 536870911, height: 1),
                sourceCell: Rect(row: 12, col: 28, width: 26, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(26),
                cellHeight: .auto(9),
                charsSize: Size(width: 21, height: 1),
                cellCharsSize: Size(width: 26, height: 9),
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
                name: "count_label",
                type: "label",
                cellName: "control_section",
                relative: Rect(row: 3, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 12, col: 0, width: 27, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(9),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 27, height: 9),
                marginRight: 14,
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
                name: "mode_label",
                type: "label",
                cellName: "section2",
                relative: Rect(row: 3, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 12, col: 28, width: 26, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(26),
                cellHeight: .auto(9),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 26, height: 9),
                marginRight: 14,
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
                name: "selectable_label",
                type: "label",
                cellName: "label_section",
                relative: Rect(row: 3, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(9),
                charsSize: Size(width: 50, height: 1),
                cellCharsSize: Size(width: 54, height: 9),
                marginRight: 2,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#a7f3d0")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "count_input",
                type: "numberinput",
                cellName: "control_section",
                relative: Rect(row: 4, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 12, col: 0, width: 27, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(9),
                charsSize: Size(width: 23, height: 1),
                cellCharsSize: Size(width: 27, height: 9),
                marginRight: 2,
                marginBottom: 4,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "mode_list",
                type: "listbox",
                cellName: "section2",
                relative: Rect(row: 4, col: 2, width: 536870911, height: 5),
                sourceCell: Rect(row: 12, col: 28, width: 26, height: 9),
                width: .expanded,
                height: .auto(5),
                cellWidth: .auto(26),
                cellHeight: .auto(9),
                charsSize: Size(width: 22, height: 5),
                cellCharsSize: Size(width: 26, height: 9),
                marginRight: 2,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "spans",
                type: "spanlabel",
                cellName: "label_section",
                relative: Rect(row: 5, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(9),
                charsSize: Size(width: 50, height: 1),
                cellCharsSize: Size(width: 54, height: 9),
                marginRight: 2,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#cbd5e1")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "enabled_check",
                type: "checkbox",
                cellName: "control_section",
                relative: Rect(row: 6, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 12, col: 0, width: 27, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(9),
                charsSize: Size(width: 23, height: 1),
                cellCharsSize: Size(width: 27, height: 9),
                marginRight: 2,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "animation",
                type: "label",
                cellName: "label_section",
                relative: Rect(row: 7, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(9),
                charsSize: Size(width: 50, height: 1),
                cellCharsSize: Size(width: 54, height: 9),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#fbbf24")
                    style.textColorGradient = TextGradient(intervalMs: 100, step: 1, segmentSize: 1, colors: [Color("#00000099"), Color("#00000066"), Color("#00000033"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000033"), Color("#00000066"), Color("#00000099")])
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "apply_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 8, col: 2, width: 23, height: 1),
                sourceCell: Rect(row: 22, col: 0, width: 54, height: 10),
                width: .auto(23),
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(10),
                charsSize: Size(width: 23, height: 1),
                cellCharsSize: Size(width: 54, height: 10),
                marginRight: 29,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "reset_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 8, col: 27, width: 536870911, height: 1),
                sourceCell: Rect(row: 22, col: 0, width: 54, height: 10),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(10),
                charsSize: Size(width: 25, height: 1),
                cellCharsSize: Size(width: 54, height: 10),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onCloseBtnClick() { }
    open func onNameInputChange(_ value: String) { }
    open func onNameInputSubmit(_ value: String) { }
    open func onCountInputChange(_ value: String) { }
    open func onCountInputSubmit(_ value: String) { }
    open func onEnabledCheckChange(_ value: String) { }
    open func onThemeComboChange(_ value: String) { }
    open func onModeListSelectionChange(_ value: [String]) { }
    open func onApplyBtnClick() { }
    open func onResetBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "close_btn"
        {
            onCloseBtnClick()
            return true
        }
        else if name == "apply_btn"
        {
            onApplyBtnClick()
            return true
        }
        else if name == "reset_btn"
        {
            onResetBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "name_input"
        {
            onNameInputChange(value)
            return true
        }
        else if name == "count_input"
        {
            onCountInputChange(value)
            return true
        }
        else if name == "enabled_check"
        {
            onEnabledCheckChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "name_input"
        {
            onNameInputSubmit(value)
            return true
        }
        else if name == "count_input"
        {
            onCountInputSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "theme_combo"
        {
            onThemeComboChange(value.isEmpty ? "" : value[0])
            return true
        }
        else if name == "mode_list"
        {
            onModeListSelectionChange(value)
            return true
        }
        return false
    }
}
