// Auto-generated UI code for activity_item - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ActivityItemUI: GeneratedWindowBase
{
    public var timestamp: Label!
    public var event_type: Label!
    public var message: Label!

    public init()
    {
        super.init("Activity Item")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ActivityItem", source: "# Activity Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable activity feed item with timestamp, event type, and message text.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntimestamp:\n  type: label\n  text: \"10:42\"\n  description: \"Optional activity timestamp.\"\n\nevent_type:\n  type: label\n  text: Info\n  description: \"Activity category label.\"\n\nmessage:\n  type: label\n  text: \"Activity message\"\n  description: \"Selectable activity message body.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@marker:\n  background-texture: \"▐\"\n  background-texture-color: \"#38bdf8\"\n@marker_info:\n  background-texture-color: \"#38bdf8\"\n@marker_task:\n  background-texture-color: \"#a78bfa\"\n@marker_warning:\n  background-texture-color: \"#facc15\"\n@marker_deploy:\n  background-texture-color: \"#22c55e\"\n@marker_note:\n  background-texture-color: \"#fb7185\"\n@body:\n  background: \"#1f2e46\"\n@timestamp:\n  color: \"#94a3b8\"\n  user-select: text\n@event_type:\n  color: \"#bfdbfe\"\n  user-select: text\n@message:\n  color: \"#ffffff\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-marker-1-+-body-**----------------------------------+\n|          | timestamp.. event_type..**               |\n|          | message..**                              |\n|          | ........................................ |\n|          | ........................................ |\n#          # #                                        |\n#          # #                                        |\n+----------+------------------------------------------+\n```\n", description: "Reusable activity feed item with timestamp, event type, and message text.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        timestamp = addElement(Label("timestamp", "10:42"))
        timestamp.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#94a3b8")
                    style.userSelect = "text"
                    return style
                }())
        timestamp.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        event_type = addElement(Label("event_type", "Info"))
        event_type.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    style.userSelect = "text"
                    return style
                }())
        event_type.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        message = addElement(Label("message", "Activity message"))
        message.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.userSelect = "text"
                    return style
                }())
        message.setCursorStyle({
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
                name: "",
                type: "",
                cellName: "marker",
                relative: Rect(row: 0, col: 0, width: 1, height: 536870910),
                sourceCell: Rect(row: 0, col: 0, width: 10, height: 6),
                width: .auto(1),
                height: .auto(536870910),
                cellWidth: .fixed(1),
                cellHeight: .fitContent,
                charsSize: Size(width: 10, height: 6),
                cellCharsSize: Size(width: 10, height: 6),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.backgroundTexture = "▐"
                    style.backgroundTextureColor = Color("#38bdf8")
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "timestamp",
                type: "label",
                cellName: "body",
                relative: Rect(row: 0, col: 1, width: 11, height: 1),
                sourceCell: Rect(row: 0, col: 11, width: 42, height: 6),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 42, height: 6),
                marginRight: 30,
                marginBottom: 5,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2e46")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#94a3b8")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "event_type",
                type: "label",
                cellName: "body",
                relative: Rect(row: 0, col: 13, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 11, width: 42, height: 6),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 14, height: 1),
                cellCharsSize: Size(width: 42, height: 6),
                marginRight: 15,
                marginBottom: 5,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2e46")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "message",
                type: "label",
                cellName: "body",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 0, col: 11, width: 42, height: 6),
                width: .expanded,
                height: .fitContent,
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 11, height: 5),
                cellCharsSize: Size(width: 42, height: 6),
                marginRight: 30,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2e46")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
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
