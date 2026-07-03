// Auto-generated UI code for gallery_view - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class GalleryViewUI: GeneratedWindowBase
{
    public var gallery_scroll: ReusableElement!

    public init()
    {
        super.init("Gallery View")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "GalleryView", source: "# Gallery View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Gallery view hosting a scrollable list of full-size images.\"\ntags: [example, image, gallery, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - gallery_scroll\n```\n\n## Members\n\n```yaml\ngallery_scroll:\n  type: gallery_scroll\n  description: \"Scrollable image list.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-view-**-------------------+\n| gallery_scroll..**....... |\n| ......................... |\n| *........................ |\n| *........................ |\n| ......................... |\n| ......................... |\n+---------------------------+\n```\n", description: "Gallery view hosting a scrollable list of full-size images.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        gallery_scroll = addElement(ReusableElement("gallery_scroll"))
        gallery_scroll.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        gallery_scroll.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        gallery_scroll.setSelectedStyle({
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
                name: "gallery_scroll",
                type: "gallery_scroll",
                cellName: "view",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 0, col: 0, width: 27, height: 6),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(6),
                charsSize: Size(width: 25, height: 6),
                cellCharsSize: Size(width: 27, height: 6),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
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
