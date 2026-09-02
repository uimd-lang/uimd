// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ChatEntryUI: GeneratedWindowBase
{
    public var role: Label!
    public var phase: Label!
    public var body: SpanLabel!
    public var detail: Label!

    public init()
    {
        super.init("Agent Chat Entry")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ChatEntry", source: "# Agent Chat Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Fixed-height simulated agent chat entry for prose and structured transcript blocks.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nrole:\n  type: label\n  text: Agent\n  description: \"Simulated transcript speaker or activity type.\"\n\nphase:\n  type: label\n  text: response\n  description: \"Deterministic scenario phase.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable simulated transcript content.\"\n\ndetail:\n  type: label\n  text: \"simulated · no external action\"\n  description: \"Simulation-only activity detail.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@header:\n  background: \"#162033\"\n  padding: 0, 1, 0, 1\n@role:\n  color: \"#a7f3d0\"\n@phase:\n  color: \"#c4b5fd\"\n  text-align: right\n@body_panel:\n  background: \"#0b1220\"\n  padding: 1, 1, 1, 1\n@body:\n  color: \"#d1d5db\"\n  word-wrap: false\n  user-select: text\n@footer:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@detail:\n  color: \"#64748b\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-header-**---------------------------------------------------------------+\n| role..**........................................ phase................. |\n+-body_panel-**-----------------------------------------------------------+\n| body..**............................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n+-footer-**---------------------------------------------------------------+\n| detail..**............................................................. |\n+-------------------------------------------------------------------------+\n```\n", description: "Fixed-height simulated agent chat entry for prose and structured transcript blocks.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        role = addElement(Label("role", "Agent"))
        role.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#a7f3d0")
                    return style
                }())
        role.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        phase = addElement(Label("phase", "response"))
        phase.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#c4b5fd")
                    style.textAlign = "right"
                    return style
                }())
        phase.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        body = addElement(SpanLabel("body", ""))
        body.setStyle({
                    var style = Style()
                    style.color = Color("#d1d5db")
                    style.userSelect = "text"
                    return style
                }())
        detail = addElement(Label("detail", "simulated · no external action"))
        detail.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#64748b")
                    style.userSelect = "text"
                    return style
                }())
        detail.setCursorStyle({
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
                name: "body",
                type: "spanlabel",
                cellName: "body_panel",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 6),
                sourceCell: Rect(row: 2, col: 0, width: 73, height: 6),
                width: .expanded,
                height: .auto(6),
                cellWidth: .expanded,
                cellHeight: .auto(6),
                charsSize: Size(width: 71, height: 6),
                cellCharsSize: Size(width: 73, height: 6),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#d1d5db")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "detail",
                type: "label",
                cellName: "footer",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 9, col: 0, width: 73, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 71, height: 1),
                cellCharsSize: Size(width: 73, height: 1),
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
                    style.background = Color("transparent")
                    style.color = Color("#64748b")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "role",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 73, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 48, height: 1),
                cellCharsSize: Size(width: 73, height: 1),
                marginRight: 24,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#162033")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#a7f3d0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "phase",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 50, width: 22, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 73, height: 1),
                width: .auto(22),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 22, height: 1),
                cellCharsSize: Size(width: 73, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#162033")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#c4b5fd")
                    style.textAlign = "right"
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
