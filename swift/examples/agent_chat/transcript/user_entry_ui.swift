// Auto-generated UI code for user_entry - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class UserEntryUI: GeneratedWindowBase
{
    public var body: SpanLabel!

    public init()
    {
        super.init("Agent Chat User Entry")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "UserEntry", source: "# Agent Chat User Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style user message with a separate accent.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable user prompt.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@body:\n  color: \"#e0f2fe\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-panel-**-----------------------------------------------------------------+\n| body..**................................................................ |\n| ........................................................................ |\n# #....................................................................... |\n# #....................................................................... |\n+--------------------------------------------------------------------------+\n```\n", description: "Heron-style user message with a separate accent.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        body = addElement(SpanLabel("body", ""))
        body.setStyle({
                    var style = Style()
                    style.color = Color("#e0f2fe")
                    style.userSelect = "text"
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "body",
                type: "spanlabel",
                cellName: "panel",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 0, col: 0, width: 74, height: 4),
                width: .expanded,
                height: .fitContent,
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 72, height: 4),
                cellCharsSize: Size(width: 74, height: 4),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#111827")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#e0f2fe")
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
