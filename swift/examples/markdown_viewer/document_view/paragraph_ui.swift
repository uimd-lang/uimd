// Auto-generated UI code for paragraph - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ParagraphUI: GeneratedWindowBase
{
    public var text: Label!

    public init()
    {
        super.init("Paragraph")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "Paragraph", source: "# Paragraph\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown paragraph block.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntext:\n  type: label\n  text: \"\"\n  description: \"Rendered paragraph text with mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@text:\n  color: \"#cbd5e1\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-body-**----------------------------------------+\n| text..**...................................... |\n| .............................................. |\n# #............................................ |\n+------------------------------------------------+\n```\n", description: "Reusable markdown paragraph block.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        text = addElement(Label("text", ""))
        text.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.userSelect = "text"
                    return style
                }())
        text.setCursorStyle({
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
                name: "text",
                type: "label",
                cellName: "body",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 48, height: 3),
                width: .expanded,
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 46, height: 3),
                cellCharsSize: Size(width: 48, height: 3),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
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
