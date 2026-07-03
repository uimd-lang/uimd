// Auto-generated UI code for image_view - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageViewUI: GeneratedWindowBase
{
    public var caption: Label!
    public var photo: Image!

    public init()
    {
        super.init("Image View")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ImageView", source: "# Image View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Full-size image display panel hosted inside a ViewHost.\"\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ncaption:\n  type: label\n  text: \"Select an image\"\n  description: \"Name of the currently displayed image.\"\n\nphoto:\n  type: image\n  source: \"\"\n  alt: \"photo\"\n  fit: contain\n  render_mode: auto\n  description: \"Full-size image display.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@caption:\n  color: \"#bfdbfe\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------------+\n| caption..**.................. |\n| photo..**.................... |\n| ............................. |\n| *............................ |\n| *............................ |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n+-------------------------------+\n```\n", description: "Full-size image display panel hosted inside a ViewHost.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        caption = addElement(Label("caption", "Select an image"))
        caption.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    style.textAlign = "center"
                    return style
                }())
        caption.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        photo = addElement(Image("photo", source: "", alt: "photo", fit: "contain", renderMode: "auto", align: "center", verticalAlign: "middle"))
        photo.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "caption",
                type: "label",
                cellName: "view",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 31, height: 12),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(12),
                charsSize: Size(width: 29, height: 1),
                cellCharsSize: Size(width: 31, height: 12),
                marginRight: 1,
                marginBottom: 11,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "photo",
                type: "image",
                cellName: "view",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 0, col: 0, width: 31, height: 12),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(12),
                charsSize: Size(width: 29, height: 11),
                cellCharsSize: Size(width: 31, height: 12),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
        ]
    }

    open func onPhotoClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "photo"
        {
            onPhotoClick()
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
