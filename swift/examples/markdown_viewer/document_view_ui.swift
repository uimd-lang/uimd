// Auto-generated UI code for document_view - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class DocumentViewUI: GeneratedScrollViewBase
{

    public init()
    {
        super.init("Document View")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "DocumentView", source: "# Document View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable scroll view that contains rendered markdown blocks such as paragraphs, headings, code blocks, and tables.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - blocks/paragraph\n  - blocks/heading\n  - blocks/code_block\n  - blocks/table_block\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@panel:\n  gap: 1\n  background: \"#00000000\"\n  padding: 1, 2, 1, 2\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------+\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n*                                                      |\n*                                                      |\n*                                                      |\n*                                                      |\n|                                                      |\n+------------------------------------------------------+\n```\n", description: "Reusable scroll view that contains rendered markdown blocks such as paragraphs, headings, code blocks, and tables.")
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
                    style.paddingLeft = 2
                    style.paddingRight = 2
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
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 10),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 54, height: 10),
                cellCharsSize: Size(width: 54, height: 10),
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
