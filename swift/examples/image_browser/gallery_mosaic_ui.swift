// Auto-generated UI code for gallery_mosaic - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class GalleryMosaicUI: GeneratedWindowBase
{
    public var gal_1: Image!
    public var gal_2: Image!
    public var gal_3: Image!
    public var gal_4: Image!
    public var gal_5: Image!

    public init()
    {
        super.init("Gallery Mosaic")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(true)
        setMcpMetadata(enabled: true, className: "GalleryMosaic", source: "# Gallery Mosaic\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"3+2 image mosaic that opens the scrollable gallery when clicked.\"\ntags: [example, image, gallery]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\ngal_1:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"Camera\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 1. Click to open gallery.\"\n\ngal_2:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"Astronaut\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 2. Click to open gallery.\"\n\ngal_3:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"Coffee\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 3. Click to open gallery.\"\n\ngal_4:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"Coins\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 4. Click to open gallery.\"\n\ngal_5:\n  type: image\n  source: \"shared/assets/image_samples/chelsea.png\"\n  alt: \"Chelsea\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 5. Click to open gallery.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 0, 1, 0\n  background: \"#172033\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**------------------+\n| gal_1.. gal_2.. gal_3... |\n| ....... ....... ........ |\n| ....... ....... ........ |\n|                          |\n| gal_4.. gal_5..          |\n| ....... .......          |\n| ....... .......          |\n+--------------------------+\n```\n", description: "3+2 image mosaic that opens the scrollable gallery when clicked.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#172033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        gal_1 = addElement(Image("gal_1", source: "shared/assets/image_samples/camera.png", alt: "Camera", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        gal_1.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        gal_1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
        gal_2 = addElement(Image("gal_2", source: "shared/assets/image_samples/astronaut.png", alt: "Astronaut", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        gal_2.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        gal_2.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
        gal_3 = addElement(Image("gal_3", source: "shared/assets/image_samples/coffee.png", alt: "Coffee", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        gal_3.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        gal_3.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
        gal_4 = addElement(Image("gal_4", source: "shared/assets/image_samples/coins.png", alt: "Coins", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        gal_4.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        gal_4.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
        gal_5 = addElement(Image("gal_5", source: "shared/assets/image_samples/chelsea.png", alt: "Chelsea", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        gal_5.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        gal_5.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "gal_1",
                type: "image",
                cellName: "view",
                relative: Rect(row: 0, col: 1, width: 7, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 26, height: 7),
                width: .auto(7),
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 7, height: 3),
                cellCharsSize: Size(width: 26, height: 7),
                marginRight: 18,
                marginBottom: 4,
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
                name: "gal_2",
                type: "image",
                cellName: "view",
                relative: Rect(row: 0, col: 9, width: 7, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 26, height: 7),
                width: .auto(7),
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 7, height: 3),
                cellCharsSize: Size(width: 26, height: 7),
                marginRight: 10,
                marginBottom: 4,
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
                name: "gal_3",
                type: "image",
                cellName: "view",
                relative: Rect(row: 0, col: 17, width: 8, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 26, height: 7),
                width: .auto(8),
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 8, height: 3),
                cellCharsSize: Size(width: 26, height: 7),
                marginRight: 1,
                marginBottom: 4,
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
                name: "gal_4",
                type: "image",
                cellName: "view",
                relative: Rect(row: 4, col: 1, width: 7, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 26, height: 7),
                width: .auto(7),
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 7, height: 3),
                cellCharsSize: Size(width: 26, height: 7),
                marginRight: 18,
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
                name: "gal_5",
                type: "image",
                cellName: "view",
                relative: Rect(row: 4, col: 9, width: 7, height: 3),
                sourceCell: Rect(row: 0, col: 0, width: 26, height: 7),
                width: .auto(7),
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 7, height: 3),
                cellCharsSize: Size(width: 26, height: 7),
                marginRight: 10,
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

    open func onGal1Click() { }
    open func onGal2Click() { }
    open func onGal3Click() { }
    open func onGal4Click() { }
    open func onGal5Click() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "gal_1"
        {
            onGal1Click()
            return true
        }
        else if name == "gal_2"
        {
            onGal2Click()
            return true
        }
        else if name == "gal_3"
        {
            onGal3Click()
            return true
        }
        else if name == "gal_4"
        {
            onGal4Click()
            return true
        }
        else if name == "gal_5"
        {
            onGal5Click()
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
