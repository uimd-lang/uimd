// Auto-generated UI code for image_button - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageButtonUI: GeneratedWindowBase
{
    public var photo: Image!

    public init()
    {
        super.init("Image Button")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(true)
        setMcpMetadata(enabled: true, className: "ImageButton", source: "# Image Button\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Single-image clickable tile with padding around a centered square image.\"\ntags: [example, image, button]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\nphoto:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: cover\n  render_mode: auto\n  align: center\n  valign: middle\n  description: \"Tile image. Click to select.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 4, 1, 4\n  background: transparent\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------+\n| photo..**............. |\n| ...................... |\n| ...................... |\n| ...................... |\n+------------------------+\n```\n", description: "Single-image clickable tile with padding around a centered square image.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#172033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        photo = addElement(Image("photo", source: "", alt: "", fit: "cover", renderMode: "auto", align: "center", verticalAlign: "middle"))
        photo.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        photo.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "photo",
                type: "image",
                cellName: "view",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 4),
                sourceCell: Rect(row: 0, col: 0, width: 24, height: 4),
                width: .expanded,
                height: .auto(4),
                cellWidth: .expanded,
                cellHeight: .auto(4),
                charsSize: Size(width: 22, height: 4),
                cellCharsSize: Size(width: 24, height: 4),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.paddingBottom = 1
                    style.paddingLeft = 4
                    style.paddingRight = 4
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
