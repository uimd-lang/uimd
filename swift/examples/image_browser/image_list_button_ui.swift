// Auto-generated UI code for image_list_button - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageListButtonUI: GeneratedWindowBase
{
    public var prw_1: Image!
    public var prw_2: Image!
    public var prw_3: Image!

    public init()
    {
        super.init("Image List Button")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(true)
        setMcpMetadata(enabled: true, className: "ImageListButton", source: "# Image List Button\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Clickable image-list preview tile with three thumbnails.\"\ntags: [example, image, button, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\nprw_1:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"Preview image 1\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 1. Click to open image items.\"\n\nprw_2:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"Preview image 2\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 2. Click to open image items.\"\n\nprw_3:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"Preview image 3\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 3. Click to open image items.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 0, 1, 0\n  background: \"#172033\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------+\n| prw_1.. prw_2.. prw_3.. |\n| ....... ....... ....... |\n| ....... ....... ....... |\n+-------------------------+\n```\n", description: "Clickable image-list preview tile with three thumbnails.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#172033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        prw_1 = addElement(Image("prw_1", source: "shared/assets/image_samples/camera.png", alt: "Preview image 1", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        prw_1.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        prw_1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
        prw_2 = addElement(Image("prw_2", source: "shared/assets/image_samples/astronaut.png", alt: "Preview image 2", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        prw_2.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        prw_2.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
        prw_3 = addElement(Image("prw_3", source: "shared/assets/image_samples/coffee.png", alt: "Preview image 3", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        prw_3.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        prw_3.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "prw_1",
                type: "image",
                cellName: "view",
                relative: Rect(row: 0, col: 1, width: 7, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 25, height: 3),
                width: .auto(7),
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 7, height: 3),
                cellCharsSize: Size(width: 25, height: 3),
                marginRight: 17,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "prw_2",
                type: "image",
                cellName: "view",
                relative: Rect(row: 0, col: 9, width: 7, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 25, height: 3),
                width: .auto(7),
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 7, height: 3),
                cellCharsSize: Size(width: 25, height: 3),
                marginRight: 9,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "prw_3",
                type: "image",
                cellName: "view",
                relative: Rect(row: 0, col: 17, width: 7, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 25, height: 3),
                width: .auto(7),
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 7, height: 3),
                cellCharsSize: Size(width: 25, height: 3),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
        ]
    }

    open func onPrw1Click() { }
    open func onPrw2Click() { }
    open func onPrw3Click() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "prw_1"
        {
            onPrw1Click()
            return true
        }
        else if name == "prw_2"
        {
            onPrw2Click()
            return true
        }
        else if name == "prw_3"
        {
            onPrw3Click()
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
