// Auto-generated UI code for reasoning_entry - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ReasoningEntryUI: GeneratedWindowBase
{
    public var heading: Label!
    public var body: SpanLabel!

    public init()
    {
        super.init("Agent Chat Reasoning Entry")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ReasoningEntry", source: "# Agent Chat Reasoning Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Animated Heron-style thinking and status message.\"\ntags: [component, simulation, reasoning]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: label\n  text: \"Thinking\"\n  description: \"Animated thinking or activity phase.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Visible simulated reasoning detail.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#e5e7eb\"\n  text-color-gradient:\n    interval: 90\n    step: 1\n    segment-size: 1\n    colors: [\"#000000aa\", \"#00000077\", \"#00000044\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000044\", \"#00000077\", \"#000000aa\"]\n@body:\n  color: \"#94a3b8\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-panel-**-----------------------------------------------------------------+\n| heading..**............................................................. |\n| body..**................................................................ |\n| ........................................................................ |\n# #....................................................................... |\n# #....................................................................... |\n+--------------------------------------------------------------------------+\n```\n", description: "Animated Heron-style thinking and status message.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        heading = addElement(Label("heading", "Thinking"))
        heading.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#e5e7eb")
                    style.textColorGradient = TextGradient(intervalMs: 90, step: 1, segmentSize: 1, colors: [Color("#000000aa"), Color("#00000077"), Color("#00000044"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000044"), Color("#00000077"), Color("#000000aa")])
                    return style
                }())
        heading.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        body = addElement(SpanLabel("body", ""))
        body.setStyle({
                    var style = Style()
                    style.color = Color("#94a3b8")
                    style.userSelect = "text"
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "heading",
                type: "label",
                cellName: "panel",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 74, height: 5),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 72, height: 1),
                cellCharsSize: Size(width: 74, height: 5),
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
                    style.background = Color("transparent")
                    style.color = Color("#e5e7eb")
                    style.textColorGradient = TextGradient(intervalMs: 90, step: 1, segmentSize: 1, colors: [Color("#000000aa"), Color("#00000077"), Color("#00000044"), Color("#00000011"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000000"), Color("#00000011"), Color("#00000044"), Color("#00000077"), Color("#000000aa")])
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "body",
                type: "spanlabel",
                cellName: "panel",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 0, col: 0, width: 74, height: 5),
                width: .expanded,
                height: .fitContent,
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 72, height: 4),
                cellCharsSize: Size(width: 74, height: 5),
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
                    style.color = Color("#94a3b8")
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
