// Auto-generated UI code for compatibility - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class CompatibilityUI: GeneratedWindowBase
{
    public var aligned_input: TextInput!
    public var choice: ComboBox!
    public var scroller: ReusableElement!

    public init()
    {
        super.init("UIMD 0.5.3 Compatibility Application")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "Compatibility", source: "# UIMD 0.5.3 Compatibility Application\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"Retained generated application for patch-level runtime compatibility.\"\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nuses:\n  - compat_scroll\n```\n\n## Members\n\n```yaml\naligned_input:\n  type: textinput\n  value: \"abc\"\n  maxlength: 16\n\nchoice:\n  type: combobox\n  options: [one, two, three, four, five, six, seven, eight]\n  selected_item: one\n\nscroller:\n  type: compat_scroll\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#303545\"\n@aligned_input:\n  text-align: right\n@scroller:\n  background: \"#303545\"\n  focus-background: \"#ffffff14\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------+\n|aligned_input...................|\n|choice..........................|\n|................................|\n|scroller........................|\n|................................|\n+--------------------------------+\n```\n", description: "Retained generated application for patch-level runtime compatibility.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#303545")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        aligned_input = addElement(TextInput("aligned_input", "abc", maxLength: 16))
        aligned_input.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    style.textAlign = "right"
                    return style
                }())
        aligned_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        aligned_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        aligned_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#ffffff")
                    style.color = Color("#0e1117")
                    return style
                }())
        aligned_input.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        aligned_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        choice = addElement(ComboBox("choice", ["one", "two", "three", "four", "five", "six", "seven", "eight"], selectedIndex: 0))
        choice.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        choice.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        choice.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        choice.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        choice.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        scroller = addElement(ReusableElement("scroller"))
        scroller.setStyle({
                    var style = Style()
                    style.background = Color("#303545")
                    return style
                }())
        scroller.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        scroller.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        scroller.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "aligned_input",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 32, height: 5),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(32),
                cellHeight: .auto(5),
                charsSize: Size(width: 32, height: 1),
                cellCharsSize: Size(width: 32, height: 5),
                marginRight: 0,
                marginBottom: 4,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    style.textAlign = "right"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "choice",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 1, col: 0, width: 536870911, height: 2),
                sourceCell: Rect(row: 0, col: 0, width: 32, height: 5),
                width: .expanded,
                height: .auto(2),
                cellWidth: .auto(32),
                cellHeight: .auto(5),
                charsSize: Size(width: 32, height: 2),
                cellCharsSize: Size(width: 32, height: 5),
                marginRight: 0,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "scroller",
                type: "compat_scroll",
                cellName: "",
                relative: Rect(row: 3, col: 0, width: 536870911, height: 2),
                sourceCell: Rect(row: 0, col: 0, width: 32, height: 5),
                width: .expanded,
                height: .auto(2),
                cellWidth: .auto(32),
                cellHeight: .auto(5),
                charsSize: Size(width: 32, height: 2),
                cellCharsSize: Size(width: 32, height: 5),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#303545")
                    return style
                }()
            ),
        ]
    }

    open func onAlignedInputChange(_ value: String) { }
    open func onAlignedInputSubmit(_ value: String) { }
    open func onChoiceChange(_ value: String) { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "aligned_input"
        {
            onAlignedInputChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "aligned_input"
        {
            onAlignedInputSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "choice"
        {
            onChoiceChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}
