// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ChatEntryUI: GeneratedWindowBase
{
    public var heading: SpanLabel!
    public var body: SpanLabel!

    public init()
    {
        super.init("Agent Chat Entry")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ChatEntry", source: "# Agent Chat Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style assistant text entry.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Assistant\"\n  description: \"Assistant role label.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable Markdown-style transcript prose.\"\n\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@body_panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#a7f3d0\"\n  user-select: text\n@body:\n  color: \"#cbd5e1\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-body_panel-**-----------------------------------------------------------+\n| heading..**............................................................ |\n| body..**............................................................... |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n", description: "Heron-style assistant text entry.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        heading = addElement(SpanLabel("heading", "Assistant"))
        heading.setStyle({
                    var style = Style()
                    style.color = Color("#a7f3d0")
                    style.userSelect = "text"
                    return style
                }())
        body = addElement(SpanLabel("body", ""))
        body.setStyle({
                    var style = Style()
                    style.color = Color("#cbd5e1")
                    style.userSelect = "text"
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "heading",
                type: "spanlabel",
                cellName: "body_panel",
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
                    style.background = Color("#00000000")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#a7f3d0")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "body",
                type: "spanlabel",
                cellName: "body_panel",
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
                    style.background = Color("#00000000")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#cbd5e1")
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
