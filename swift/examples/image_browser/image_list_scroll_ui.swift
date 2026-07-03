// Auto-generated UI code for image_list_scroll - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageListScrollUI: GeneratedScrollViewBase
{

    public init()
    {
        super.init("Image List Scroll")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ImageListScroll", source: "# Image List Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\ndescription: \"Scrollable image item list.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - image_list_item\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n  focus-background: \"#1e3a5f\"\n@panel:\n  gap: 1\n  padding: 1, 1, 1, 1\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------------------------------------------------+\n|                                                                         |\n|                                                                         |\n|                                                                         |\n|                                                                         |\n|                                                                         |\n*                                                                         |\n*                                                                         |\n*                                                                         |\n*                                                                         |\n|                                                                         |\n+-------------------------------------------------------------------------+\n```\n", description: "Scrollable image item list.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        setGeneratedWindowFocusStyle({
                    var style = Style()
                    style.background = Color("#1e3a5f")
                    return style
                }())
        setGeneratedScrollView(addElement(ScrollView("__scrollview", 1)))
        scrollView().setStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.gap = 1
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }())
        scrollView().setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e3a5f")
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
                sourceCell: Rect(row: 0, col: 0, width: 73, height: 10),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 73, height: 10),
                cellCharsSize: Size(width: 73, height: 10),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#030712")
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
