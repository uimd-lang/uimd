// Auto-generated UI code for image_list_view - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageListViewUI: GeneratedWindowBase
{
    public var add_btn: Button!
    public var items: ReusableElement!

    public init()
    {
        super.init("Image List View")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ImageListView", source: "# Image List View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"ViewHost page with add button and scrollable image item list.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_list_scroll\n```\n\n## Members\n\n```yaml\nadd_btn:\n  type: button\n  title: Add\n  description: \"Add an image item.\"\n\nitems:\n  type: image_list_scroll\n  description: \"Scrollable list of image items.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@toolbar:\n  background: \"#030712\"\n@items_cell:\n  background: \"#030712\"\nbutton:\n  background: \"#14532d\"\n  color: \"#e2e8f0\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-toolbar-**------------------------------------------------------------+\n| add_btn............                                                   |\n+-items_cell-**---------------------------------------------------------+\n|items..**..............................................................|\n*.......................................................................|\n**......................................................................|\n|*......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n+-----------------------------------------------------------------------+\n```\n", description: "ViewHost page with add button and scrollable image item list.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        add_btn = addElement(Button("add_btn", "Add"))
        add_btn.setStyle({
                    var style = Style()
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        add_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#16a34a")
                    style.color = Color("#ffffff")
                    return style
                }())
        add_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        items = addElement(ReusableElement("items"))
        items.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        items.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        items.setSelectedStyle({
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
                name: "items",
                type: "image_list_scroll",
                cellName: "items_cell",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 2, col: 0, width: 71, height: 11),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 71, height: 11),
                cellCharsSize: Size(width: 71, height: 11),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "add_btn",
                type: "button",
                cellName: "toolbar",
                relative: Rect(row: 0, col: 1, width: 19, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 1),
                width: .auto(19),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 19, height: 1),
                cellCharsSize: Size(width: 71, height: 1),
                marginRight: 51,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onAddBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "add_btn"
        {
            onAddBtnClick()
            return true
        }
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
