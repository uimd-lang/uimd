// Auto-generated UI code for table_block - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class TableBlockUI: GeneratedWindowBase
{
    public var kind: Label!
    public var table: MessageTable!

    public init()
    {
        super.init("Table Block")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "TableBlock", source: "# Table Block\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown table block with a small header and message table content.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nkind:\n  type: label\n  text: \"markdown table\"\n  description: \"Header label identifying the block as a markdown table.\"\n\ntable:\n  type: messagetable\n  text: \"\"\n  description: \"Rendered markdown table content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@head:\n  background: \"#1d314c\"\n@body:\n  background: \"#0c1a2d\"\n@kind:\n  color: \"#93c5fd\"\n@table:\n  color: \"#e5e7eb\"\n```\n\n## User Interface\n\n```ui\n+-head-**----------------------------------------+\n| kind..**...................................... |\n+-body-**----------------------------------------+\n| table..**..................................... |\n| .............................................. |\n# #............................................ |\n# #............................................ |\n+------------------------------------------------+\n```\n", description: "Reusable markdown table block with a small header and message table content.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        kind = addElement(Label("kind", "markdown table"))
        kind.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        kind.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        table = addElement(MessageTable("table", ""))
        table.setStyle({
                    var style = Style()
                    style.color = Color("#e5e7eb")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "kind",
                type: "label",
                cellName: "head",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 48, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 46, height: 1),
                cellCharsSize: Size(width: 48, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1d314c")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "table",
                type: "messagetable",
                cellName: "body",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 2, col: 0, width: 48, height: 4),
                width: .expanded,
                height: .fitContent,
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 46, height: 4),
                cellCharsSize: Size(width: 48, height: 4),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#0c1a2d")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#e5e7eb")
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
