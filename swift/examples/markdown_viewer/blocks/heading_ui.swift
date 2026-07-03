// Auto-generated UI code for heading - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class HeadingUI: GeneratedWindowBase
{
    public var heading_text: SpanLabel!

    public init()
    {
        super.init("Heading")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "Heading", source: "# Heading\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown heading block.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading_text:\n  type: spanlabel\n  text: \"\"\n  description: \"Rendered heading text with mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@heading_cell:\n  background: \"#00000000\"\n@heading_text:\n  color: \"#dbeafe\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-heading_cell-**--------------------------------+\n| heading_text..**.............................. |\n+------------------------------------------------+\n```\n", description: "Reusable markdown heading block.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        heading_text = addElement(SpanLabel("heading_text", ""))
        heading_text.setStyle({
                    var style = Style()
                    style.color = Color("#dbeafe")
                    style.userSelect = "text"
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "heading_text",
                type: "spanlabel",
                cellName: "heading_cell",
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
                    style.background = Color("#00000000")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#dbeafe")
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
