// Auto-generated UI code for table_entry - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class TableEntryUI: GeneratedWindowBase
{
    public var heading: SpanLabel!
    public var table: MessageTable!

    public init()
    {
        super.init("Agent Chat Table Entry")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "TableEntry", source: "# Agent Chat Table Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style simulated agent Markdown table block.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Table\"\n  description: \"Table title.\"\n\ntable:\n  type: messagetable\n  text: \"\"\n  description: \"Rendered Markdown table content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@table_panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#ddd6fe\"\n@table:\n  color: \"#cbd5e1\"\n```\n\n## User Interface\n\n```ui\n+-table_panel-**----------------------------------------------------------+\n| heading..**........................................................... .|\n| table..**.............................................................. |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n", description: "Heron-style simulated agent Markdown table block.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        heading = addElement(SpanLabel("heading", "Table"))
        heading.setStyle({
                    var style = Style()
                    style.color = Color("#ddd6fe")
                    return style
                }())
        table = addElement(MessageTable("table", ""))
        table.setStyle({
                    var style = Style()
                    style.color = Color("#cbd5e1")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "heading",
                type: "spanlabel",
                cellName: "table_panel",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 73, height: 5),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 70, height: 1),
                cellCharsSize: Size(width: 73, height: 5),
                marginRight: 2,
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
                    style.color = Color("#ddd6fe")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "table",
                type: "messagetable",
                cellName: "table_panel",
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
