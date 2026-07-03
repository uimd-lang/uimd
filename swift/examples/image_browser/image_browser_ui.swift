// Auto-generated UI code for image_browser - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageBrowserUI: GeneratedWindowBase
{
    public var title: Label!
    public var close_btn: Button!
    public var render_label: Label!
    public var mode_select: ComboBox!
    public var sidebar_title: Label!
    public var camera_thumb: ReusableElement!
    public var astro_thumb: ReusableElement!
    public var coffee_thumb: ReusableElement!
    public var gallery_label: Label!
    public var image_list_label: Label!
    public var gallery_mosaic: ReusableElement!
    public var image_list_btn: ReusableElement!
    public var spacer: Label!
    public var main: ViewHost!

    public init()
    {
        super.init("Image Browser")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ImageBrowser", source: "# Image Browser\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: \"Image browser with a thumbnail sidebar and full-size image panel.\"\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_view\n  - gallery_view\n  - gallery_mosaic\n  - image_button\n  - image_list_button\n  - image_list_view\n  - image_show_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Image Browser\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the image browser.\"\n\nrender_label:\n  type: label\n  text: \"Render\"\n  description: \"Label above the render mode selector.\"\n  expose: false\n\nmode_select:\n  type: combobox\n  options: [Normal, Fallback]\n  selected_item: Normal\n  description: \"Switch between Sixel and fallback image rendering.\"\n\nsidebar_title:\n  type: label\n  text: \"Photos\"\n  description: \"Sidebar section label.\"\n  expose: false\n\ncamera_thumb:\n  type: image_button\n  description: \"Camera thumbnail. Click to view full size.\"\n\nastro_thumb:\n  type: image_button\n  description: \"Astronaut thumbnail. Click to view full size.\"\n\ncoffee_thumb:\n  type: image_button\n  description: \"Coffee thumbnail. Click to view full size.\"\n\ngallery_label:\n  type: label\n  text: \"Gallery\"\n  description: \"Label above the gallery mosaic.\"\n  expose: false\n\nimage_list_label:\n  type: label\n  text: \"Image items\"\n  description: \"List preview label.\"\n  expose: false\n\ngallery_mosaic:\n  type: gallery_mosaic\n  description: \"Click any image to open the full scrollable gallery.\"\n\nimage_list_btn:\n  type: image_list_button\n  description: \"Click to open the image item list.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Spacer that absorbs leftover sidebar height.\"\n  expose: false\n\nmain:\n  type: viewhost\n  description: \"Full-size image panel.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: \"#1f2937\"\n  padding: 0\n@sidebar:\n  background: \"#172033\"\n  padding: 0\n@content:\n  background: \"#030712\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@render_label:\n  color: \"#93c5fd\"\n@sidebar_title:\n  color: \"#93c5fd\"\n@gallery_label:\n  color: \"#93c5fd\"\n@image_list_label:\n  color: \"#93c5fd\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+--header-**-------------------------------------------------------------------+\n| title..**....................................................... close_btn.. |\n+--sidebar-------------------+--content-**-------------------------------------+\n| render_label.............. | main..**....................................... |\n* mode_select............... | ............................................... |\n*                            | *.............................................. |\n| sidebar_title............. | *.............................................. |\n| camera_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| astro_thumb............... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| coffee_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| gallery_label............. | ............................................... |\n| gallery_mosaic............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| image_list_label.......... | ............................................... |\n| image_list_btn............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n* spacer.................... | ............................................... |\n* .......................... | ............................................... |\n| .......................... | ............................................... |\n+----------------------------+-------------------------------------------------+\n```\n", description: "Image browser with a thumbnail sidebar and full-size image panel.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        title = addElement(Label("title", "Image Browser"))
        title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        close_btn = addElement(Button("close_btn", "Quit"))
        close_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        render_label = addElement(Label("render_label", "Render"))
        render_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        render_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        mode_select = addElement(ComboBox("mode_select", ["Normal", "Fallback"], selectedIndex: 0))
        mode_select.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        mode_select.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        mode_select.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        mode_select.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        mode_select.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        sidebar_title = addElement(Label("sidebar_title", "Photos"))
        sidebar_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        sidebar_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        camera_thumb = addElement(ReusableElement("camera_thumb"))
        camera_thumb.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        camera_thumb.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        camera_thumb.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
        astro_thumb = addElement(ReusableElement("astro_thumb"))
        astro_thumb.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        astro_thumb.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        astro_thumb.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
        coffee_thumb = addElement(ReusableElement("coffee_thumb"))
        coffee_thumb.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        coffee_thumb.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        coffee_thumb.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
        gallery_label = addElement(Label("gallery_label", "Gallery"))
        gallery_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        gallery_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        image_list_label = addElement(Label("image_list_label", "Image items"))
        image_list_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        image_list_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        gallery_mosaic = addElement(ReusableElement("gallery_mosaic"))
        gallery_mosaic.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        gallery_mosaic.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        gallery_mosaic.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
        image_list_btn = addElement(ReusableElement("image_list_btn"))
        image_list_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        image_list_btn.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        image_list_btn.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
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
        main = addElement(ViewHost("main"))
        main.setStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "main",
                type: "viewhost",
                cellName: "content",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 2, col: 29, width: 49, height: 41),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(41),
                charsSize: Size(width: 47, height: 41),
                cellCharsSize: Size(width: 49, height: 41),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "render_label",
                type: "label",
                cellName: "sidebar",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 40,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "title",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 78, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 64, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 13,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "close_btn",
                type: "button",
                cellName: "header",
                relative: Rect(row: 0, col: 66, width: 11, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 78, height: 1),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#ffffff")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "mode_select",
                type: "combobox",
                cellName: "sidebar",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 39,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "sidebar_title",
                type: "label",
                cellName: "sidebar",
                relative: Rect(row: 3, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 37,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "camera_thumb",
                type: "image_button",
                cellName: "sidebar",
                relative: Rect(row: 4, col: 1, width: 536870911, height: 6),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(6),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 6),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 31,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "astro_thumb",
                type: "image_button",
                cellName: "sidebar",
                relative: Rect(row: 10, col: 1, width: 536870911, height: 6),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(6),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 6),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 25,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "coffee_thumb",
                type: "image_button",
                cellName: "sidebar",
                relative: Rect(row: 16, col: 1, width: 536870911, height: 6),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(6),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 6),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 19,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "gallery_label",
                type: "label",
                cellName: "sidebar",
                relative: Rect(row: 22, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 18,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "gallery_mosaic",
                type: "gallery_mosaic",
                cellName: "sidebar",
                relative: Rect(row: 23, col: 1, width: 536870911, height: 9),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(9),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 9),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 9,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "image_list_label",
                type: "label",
                cellName: "sidebar",
                relative: Rect(row: 32, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 8,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "image_list_btn",
                type: "image_list_button",
                cellName: "sidebar",
                relative: Rect(row: 33, col: 1, width: 536870911, height: 5),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(5),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 5),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 3,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "spacer",
                type: "label",
                cellName: "sidebar",
                relative: Rect(row: 38, col: 1, width: 536870911, height: 3),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 41),
                width: .expanded,
                height: .auto(3),
                cellWidth: .auto(28),
                cellHeight: .expanded,
                charsSize: Size(width: 26, height: 3),
                cellCharsSize: Size(width: 28, height: 41),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 0
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

    open func onCloseBtnClick() { }
    open func onModeSelectChange(_ value: String) { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "close_btn"
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
        if name == "mode_select"
        {
            onModeSelectChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}
