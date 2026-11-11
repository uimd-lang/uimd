// Auto-generated UI code for code_entry - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class CodeEntryUI: GeneratedWindowBase
{
    public var heading: SpanLabel!
    public var code: Label!

    public init()
    {
        super.init("Agent Chat Code Entry")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "CodeEntry", source: "# Agent Chat Code Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style code, activity, command output, or diff block.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Code\"\n  description: \"Code language or activity title.\"\n\ncode:\n  type: label\n  text: \"\"\n  description: \"Selectable formatted code, command output, or diff content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@code_panel:\n  background: \"#0f1b2d\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#ddd6fe\"\n@code:\n  color: \"#d1d5db\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-code_panel-**-----------------------------------------------------------+\n| heading..**............................................................ |\n| code..**............................................................... |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n", description: "Heron-style code, activity, command output, or diff block.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        heading = addElement(SpanLabel("heading", "Code"))
        heading.setStyle({
                    var style = Style()
                    style.color = Color("#ddd6fe")
                    return style
                }())
        code = addElement(Label("code", ""))
        code.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    style.userSelect = "text"
                    return style
                }())
        code.setCursorStyle({
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
                name: "heading",
                type: "spanlabel",
                cellName: "code_panel",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 73, height: 5),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 71, height: 1),
                cellCharsSize: Size(width: 73, height: 5),
                marginRight: 1,
                marginBottom: 4,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#0f1b2d")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#ddd6fe")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "code",
                type: "label",
                cellName: "code_panel",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 0, col: 0, width: 73, height: 5),
                width: .expanded,
                height: .fitContent,
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 71, height: 4),
                cellCharsSize: Size(width: 73, height: 5),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#0f1b2d")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    style.userSelect = "text"
                    return style
                }()
            ),
        ]
    }


    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        return false
    }
}
