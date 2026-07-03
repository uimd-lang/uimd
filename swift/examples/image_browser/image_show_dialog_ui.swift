// Auto-generated UI code for image_show_dialog - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageShowDialogUI: GeneratedWindowBase
{
    public var title: Label!
    public var photo: Image!
    public var close_btn: Button!

    public init()
    {
        super.init("Image Show Dialog")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("dialog")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ImageShowDialog", source: "# Image Show Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\ndescription: \"Dialog for showing a selected image.\"\ntags: [example, image, dialog]\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Image\"\n  description: \"Dialog title.\"\n  expose: false\n\nphoto:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Selected image preview.\"\n\nclose_btn:\n  type: button\n  title: Close\n  description: \"Close the dialog.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 1\n  background: \"#081426\"\n  margin: 4, 6, 4, 6\ncell:\n  padding: 1, 1, 1, 1\n@title:\n  color: \"#f8fafc\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-dialog-**--------------------------------------------------------------------------------+\n| title..**............................................................................... |\n| photo..**............................................................................... |\n* ........................................................................................ |\n* *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+-**-------------------------------+---------------------+-**------------------------------+\n|                                  |  close_btn........  |                                 |\n+----------------------------------+---------------------+---------------------------------+\n```\n", description: "Dialog for showing a selected image.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#081426")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 1
                    style.borderWidthVertical = 1
                    style.marginBottom = 4
                    style.marginLeft = 6
                    style.marginRight = 6
                    style.marginTop = 4
                    return style
                }())
        title = addElement(Label("title", "Image"))
        title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f8fafc")
                    style.textAlign = "center"
                    return style
                }())
        title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        photo = addElement(Image("photo", source: "", alt: "", fit: "contain", renderMode: "auto", align: "center", verticalAlign: "middle"))
        photo.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        close_btn = addElement(Button("close_btn", "Close"))
        close_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        close_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "",
                type: "",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 22, col: 0, width: 34, height: 1),
                width: .auto(536870911),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 34, height: 1),
                cellCharsSize: Size(width: 34, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "",
                type: "",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 22, col: 57, width: 33, height: 1),
                width: .auto(536870911),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 33, height: 1),
                cellCharsSize: Size(width: 33, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "title",
                type: "label",
                cellName: "dialog",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 90, height: 21),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 88, height: 1),
                cellCharsSize: Size(width: 90, height: 21),
                marginRight: 1,
                marginBottom: 20,
                cellStyle: {
                    var style = Style()
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f8fafc")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "close_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 0, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 22, col: 35, width: 21, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(1),
                charsSize: Size(width: 17, height: 1),
                cellCharsSize: Size(width: 21, height: 1),
                marginRight: 2,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "photo",
                type: "image",
                cellName: "dialog",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 0, col: 0, width: 90, height: 21),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 88, height: 20),
                cellCharsSize: Size(width: 90, height: 21),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
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
    open func onCloseBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "photo"
        {
            onPhotoClick()
            return true
        }
        else if name == "close_btn"
        {
            onCloseBtnClick()
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
