// Auto-generated UI code for image_list_item - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageListItemUI: GeneratedWindowBase
{
    public var sel: CheckBox!
    public var thumb: Image!
    public var name_label: Label!
    public var path_label: Label!
    public var spacer: Label!
    public var show_btn: Button!
    public var browse_btn: Button!
    public var delete_btn: Button!

    public init()
    {
        super.init("Image List Item")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ImageListItem", source: "# Image List Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\ndescription: \"Reusable row for one image item.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nsel:\n  type: checkbox\n  title: \"\"\n  value: false\n  description: \"Select this image item.\"\n\nthumb:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Small image thumbnail.\"\n\nname_label:\n  type: label\n  text: \"\"\n  description: \"Image file name.\"\n\npath_label:\n  type: label\n  text: \"\"\n  description: \"Full image path.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible spacer before item actions.\"\n  expose: false\n\nshow_btn:\n  type: button\n  title: Show\n  description: \"Show this image in a dialog.\"\n\nbrowse_btn:\n  type: button\n  title: Browse\n  description: \"Choose a replacement image.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Delete this image item after confirmation.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 0, 0, 0, 0\n@name_label:\n  color: \"#f8fafc\"\n  user-select: text\n@path_label:\n  color: \"#64748b\"\n  user-select: text\ncheckbox:\n  color: \"#cbd5e1\"\n  focus-color: \"#facc15\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@show_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------------------------------+\n|                                                                       |\n|  sel thumb...  name_label..**..  show_btn... browse_btn. delete_btn.  |\n|  ... ........  .....................................................  |\n|  ... ........  path_label..**.......................................  |\n|  ... ........  .....................................................  |\n|  ... ........  spacer..**...........................................  |\n|                                                                       |\n+-----------------------------------------------------------------------+\n```\n", description: "Reusable row for one image item.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        sel = addElement(CheckBox("sel", "", false))
        sel.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        sel.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#facc15")
                    return style
                }())
        sel.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#22c55e")
                    return style
                }())
        sel.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#94a3b8")
                    return style
                }())
        sel.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        thumb = addElement(Image("thumb", source: "", alt: "", fit: "contain", renderMode: "auto", align: "center", verticalAlign: "middle"))
        thumb.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        name_label = addElement(Label("name_label", ""))
        name_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f8fafc")
                    style.userSelect = "text"
                    return style
                }())
        name_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        path_label = addElement(Label("path_label", ""))
        path_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#64748b")
                    style.userSelect = "text"
                    return style
                }())
        path_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        spacer = addElement(Label("spacer", ""))
        spacer.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        spacer.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        show_btn = addElement(Button("show_btn", "Show"))
        show_btn.setStyle({
                    var style = Style()
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        show_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#16a34a")
                    style.color = Color("#ffffff")
                    return style
                }())
        show_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        browse_btn = addElement(Button("browse_btn", "Browse"))
        browse_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        browse_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        browse_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        delete_btn = addElement(Button("delete_btn", "Delete"))
        delete_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        delete_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        delete_btn.setDisabledStyle({
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
                name: "sel",
                type: "checkbox",
                cellName: "card",
                relative: Rect(row: 1, col: 2, width: 3, height: 5),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 7),
                width: .auto(3),
                height: .auto(5),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 3, height: 5),
                cellCharsSize: Size(width: 71, height: 7),
                marginRight: 66,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 0
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "thumb",
                type: "image",
                cellName: "card",
                relative: Rect(row: 1, col: 6, width: 8, height: 5),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 7),
                width: .auto(8),
                height: .auto(5),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 8, height: 5),
                cellCharsSize: Size(width: 71, height: 7),
                marginRight: 57,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 0
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 0
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
                name: "name_label",
                type: "label",
                cellName: "card",
                relative: Rect(row: 1, col: 16, width: 536870911, height: 2),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 7),
                width: .expanded,
                height: .auto(2),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 16, height: 2),
                cellCharsSize: Size(width: 71, height: 7),
                marginRight: 39,
                marginBottom: 4,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 0
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f8fafc")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "show_btn",
                type: "button",
                cellName: "card",
                relative: Rect(row: 1, col: 34, width: 11, height: 5),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 7),
                width: .auto(11),
                height: .auto(5),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 11, height: 5),
                cellCharsSize: Size(width: 71, height: 7),
                marginRight: 26,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 0
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "browse_btn",
                type: "button",
                cellName: "card",
                relative: Rect(row: 1, col: 46, width: 11, height: 5),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 7),
                width: .auto(11),
                height: .auto(5),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 11, height: 5),
                cellCharsSize: Size(width: 71, height: 7),
                marginRight: 14,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 0
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 0
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
                name: "delete_btn",
                type: "button",
                cellName: "card",
                relative: Rect(row: 1, col: 58, width: 11, height: 5),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 7),
                width: .auto(11),
                height: .auto(5),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 11, height: 5),
                cellCharsSize: Size(width: 71, height: 7),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 0
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "path_label",
                type: "label",
                cellName: "card",
                relative: Rect(row: 3, col: 16, width: 536870911, height: 2),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 7),
                width: .expanded,
                height: .auto(2),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 53, height: 2),
                cellCharsSize: Size(width: 71, height: 7),
                marginRight: 2,
                marginBottom: 2,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 0
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#64748b")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "spacer",
                type: "label",
                cellName: "card",
                relative: Rect(row: 5, col: 16, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 71, height: 7),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(7),
                charsSize: Size(width: 53, height: 1),
                cellCharsSize: Size(width: 71, height: 7),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 0
                    style.paddingLeft = 0
                    style.paddingRight = 0
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
        ]
    }

    open func onSelChange(_ value: String) { }
    open func onThumbClick() { }
    open func onShowBtnClick() { }
    open func onBrowseBtnClick() { }
    open func onDeleteBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "thumb"
        {
            onThumbClick()
            return true
        }
        else if name == "show_btn"
        {
            onShowBtnClick()
            return true
        }
        else if name == "browse_btn"
        {
            onBrowseBtnClick()
            return true
        }
        else if name == "delete_btn"
        {
            onDeleteBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "sel"
        {
            onSelChange(value)
            return true
        }
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
