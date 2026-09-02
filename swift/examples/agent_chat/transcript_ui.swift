// Auto-generated UI code for transcript - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class TranscriptUI: GeneratedScrollViewBase
{

    public init()
    {
        super.init("Agent Chat Transcript")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "Transcript", source: "# Agent Chat Transcript\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Scrollable collection of deterministic simulated agent chat entries.\"\ntags: [component, simulation, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - ../chat_entry/chat_entry\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  gap: 1\n  padding: 1, 1, 1, 1\n  background: \"#00000000\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------------------------------------------------+\n|                                                                         |\n|                                                                         |\n*                                                                         |\n*                                                                         |\n|                                                                         |\n+-------------------------------------------------------------------------+\n```\n", description: "Scrollable collection of deterministic simulated agent chat entries.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        setGeneratedScrollView(addElement(ScrollView("__scrollview", 1)))
        scrollView().setStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.gap = 1
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }())
        scrollView().setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        scrollView().setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        scrollView().setSelectedStyle({
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
                name: "__scrollview",
                type: "uiscrollview",
                cellName: "panel",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 0, col: 0, width: 73, height: 5),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 73, height: 5),
                cellCharsSize: Size(width: 73, height: 5),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#00000000")
                    style.gap = 1
                    return style
                }(),
                elementStyle: Style()
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
