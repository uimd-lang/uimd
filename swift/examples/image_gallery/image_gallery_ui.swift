// Auto-generated UI code for image_gallery - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ImageGalleryUI: GeneratedWindowBase
{
    public var header: Label!
    public var close_btn: Button!
    public var fallback_label: Label!
    public var sixel_label: Label!
    public var stretch_label: Label!
    public var camera_fb: Image!
    public var astro_fb: Image!
    public var coffee_fb: Image!
    public var coins_fb: Image!
    public var camera_ctn: Image!
    public var astro_ctn: Image!
    public var coffee_ctn: Image!
    public var coins_ctn: Image!
    public var camera_str: Image!
    public var astro_str: Image!
    public var coffee_str: Image!
    public var coins_str: Image!
    public var status: Label!

    public init()
    {
        super.init("Image Gallery")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ImageGallery", source: "# Image Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: \"Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.\"\ntags: [example, image]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Image Gallery\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the image gallery.\"\n\nfallback_label:\n  type: label\n  text: \"Fallback cover\"\n  description: \"Fallback row label.\"\n\nsixel_label:\n  type: label\n  text: \"Sixel contain\"\n  description: \"Sixel contain row label.\"\n\nstretch_label:\n  type: label\n  text: \"Sixel stretch\"\n  description: \"Sixel stretch row label.\"\n\ncamera_fb:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Camera image forced through fallback rendering.\"\n\nastro_fb:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Astronaut image forced through fallback rendering.\"\n\ncoffee_fb:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Coffee image forced through fallback rendering.\"\n\ncoins_fb:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Coins image forced through fallback rendering.\"\n\ncamera_ctn:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Camera image in forced Sixel contain mode.\"\n\nastro_ctn:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Astronaut image in forced Sixel contain mode.\"\n\ncoffee_ctn:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Coffee image in forced Sixel contain mode.\"\n\ncoins_ctn:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Coins image in forced Sixel contain mode.\"\n\ncamera_str:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Camera image in forced Sixel stretch mode.\"\n\nastro_str:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Astronaut image in forced Sixel stretch mode.\"\n\ncoffee_str:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Coffee image in forced Sixel stretch mode.\"\n\ncoins_str:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Coins image in forced Sixel stretch mode.\"\n\nstatus:\n  type: label\n  text: \"Rows: fallback cover, forced Sixel contain, forced Sixel stretch.\"\n  description: \"Current image validation status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@fallback_label:\n  color: \"#93c5fd\"\n@sixel_label:\n  color: \"#93c5fd\"\n@stretch_label:\n  color: \"#93c5fd\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n@status:\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------------------------+\n| header.................................................................. close_btn.. |\n|                                                                                      |\n| fallback_label...................................................................... |\n|  camera_fb........... astro_fb............ coffee_fb........... coins_fb............. |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| sixel_label......................................................................... |\n|  camera_ctn.......... astro_ctn........... coffee_ctn.......... coins_ctn........... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| stretch_label....................................................................... |\n|  camera_str.......... astro_str........... coffee_str.......... coins_str........... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| status.............................................................................. |\n+--------------------------------------------------------------------------------------+\n```\n", description: "Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header = addElement(Label("header", "Image Gallery"))
        header.setStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        header.setCursorStyle({
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
        fallback_label = addElement(Label("fallback_label", "Fallback cover"))
        fallback_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        fallback_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        sixel_label = addElement(Label("sixel_label", "Sixel contain"))
        sixel_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        sixel_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        stretch_label = addElement(Label("stretch_label", "Sixel stretch"))
        stretch_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        stretch_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        camera_fb = addElement(Image("camera_fb", source: "shared/assets/image_samples/camera.png", alt: "camera fallback", fit: "cover", renderMode: "fallback", align: "center", verticalAlign: "middle"))
        camera_fb.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        astro_fb = addElement(Image("astro_fb", source: "shared/assets/image_samples/astronaut.png", alt: "astronaut fallback", fit: "cover", renderMode: "fallback", align: "center", verticalAlign: "middle"))
        astro_fb.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        coffee_fb = addElement(Image("coffee_fb", source: "shared/assets/image_samples/coffee.png", alt: "coffee fallback", fit: "cover", renderMode: "fallback", align: "center", verticalAlign: "middle"))
        coffee_fb.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        coins_fb = addElement(Image("coins_fb", source: "shared/assets/image_samples/coins.png", alt: "coins fallback", fit: "cover", renderMode: "fallback", align: "center", verticalAlign: "middle"))
        coins_fb.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        camera_ctn = addElement(Image("camera_ctn", source: "shared/assets/image_samples/camera.png", alt: "camera sixel", fit: "contain", renderMode: "sixel", align: "center", verticalAlign: "middle"))
        camera_ctn.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        astro_ctn = addElement(Image("astro_ctn", source: "shared/assets/image_samples/astronaut.png", alt: "astronaut sixel", fit: "contain", renderMode: "sixel", align: "center", verticalAlign: "middle"))
        astro_ctn.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        coffee_ctn = addElement(Image("coffee_ctn", source: "shared/assets/image_samples/coffee.png", alt: "coffee sixel", fit: "contain", renderMode: "sixel", align: "center", verticalAlign: "middle"))
        coffee_ctn.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        coins_ctn = addElement(Image("coins_ctn", source: "shared/assets/image_samples/coins.png", alt: "coins sixel", fit: "contain", renderMode: "sixel", align: "center", verticalAlign: "middle"))
        coins_ctn.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        camera_str = addElement(Image("camera_str", source: "shared/assets/image_samples/camera.png", alt: "camera stretch", fit: "stretch", renderMode: "sixel", align: "center", verticalAlign: "middle"))
        camera_str.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        astro_str = addElement(Image("astro_str", source: "shared/assets/image_samples/astronaut.png", alt: "astronaut stretch", fit: "stretch", renderMode: "sixel", align: "center", verticalAlign: "middle"))
        astro_str.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        coffee_str = addElement(Image("coffee_str", source: "shared/assets/image_samples/coffee.png", alt: "coffee stretch", fit: "stretch", renderMode: "sixel", align: "center", verticalAlign: "middle"))
        coffee_str.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        coins_str = addElement(Image("coins_str", source: "shared/assets/image_samples/coins.png", alt: "coins stretch", fit: "stretch", renderMode: "sixel", align: "center", verticalAlign: "middle"))
        coins_str.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        status = addElement(Label("status", "Rows: fallback cover, forced Sixel contain, forced Sixel stretch."))
        status.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        status.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "header",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 1, width: 72, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(72),
                height: .auto(1),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 72, height: 1),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 13,
                marginBottom: 38,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "close_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 0, col: 74, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 1,
                marginBottom: 38,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#ffffff")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "fallback_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 2, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 84, height: 1),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 1,
                marginBottom: 36,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "camera_fb",
                type: "image",
                cellName: "",
                relative: Rect(row: 3, col: 2, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 64,
                marginBottom: 26,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "astro_fb",
                type: "image",
                cellName: "",
                relative: Rect(row: 3, col: 23, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 43,
                marginBottom: 26,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "coffee_fb",
                type: "image",
                cellName: "",
                relative: Rect(row: 3, col: 44, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 22,
                marginBottom: 26,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "coins_fb",
                type: "image",
                cellName: "",
                relative: Rect(row: 3, col: 65, width: 536870911, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .expanded,
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 21, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 0,
                marginBottom: 26,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "sixel_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 14, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 84, height: 1),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 1,
                marginBottom: 24,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "camera_ctn",
                type: "image",
                cellName: "",
                relative: Rect(row: 15, col: 2, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 64,
                marginBottom: 14,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "astro_ctn",
                type: "image",
                cellName: "",
                relative: Rect(row: 15, col: 23, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 43,
                marginBottom: 14,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "coffee_ctn",
                type: "image",
                cellName: "",
                relative: Rect(row: 15, col: 44, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 22,
                marginBottom: 14,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "coins_ctn",
                type: "image",
                cellName: "",
                relative: Rect(row: 15, col: 65, width: 536870911, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .expanded,
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 1,
                marginBottom: 14,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "stretch_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 26, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 84, height: 1),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 1,
                marginBottom: 12,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "camera_str",
                type: "image",
                cellName: "",
                relative: Rect(row: 27, col: 2, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 64,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "astro_str",
                type: "image",
                cellName: "",
                relative: Rect(row: 27, col: 23, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 43,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "coffee_str",
                type: "image",
                cellName: "",
                relative: Rect(row: 27, col: 44, width: 20, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .auto(20),
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 22,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "coins_str",
                type: "image",
                cellName: "",
                relative: Rect(row: 27, col: 65, width: 536870911, height: 10),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .expanded,
                height: .auto(10),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 20, height: 10),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 1,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status",
                type: "label",
                cellName: "",
                relative: Rect(row: 38, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 86, height: 39),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(86),
                cellHeight: .auto(39),
                charsSize: Size(width: 84, height: 1),
                cellCharsSize: Size(width: 86, height: 39),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
        ]
    }

    open func onCloseBtnClick() { }
    open func onCameraFbClick() { }
    open func onAstroFbClick() { }
    open func onCoffeeFbClick() { }
    open func onCoinsFbClick() { }
    open func onCameraCtnClick() { }
    open func onAstroCtnClick() { }
    open func onCoffeeCtnClick() { }
    open func onCoinsCtnClick() { }
    open func onCameraStrClick() { }
    open func onAstroStrClick() { }
    open func onCoffeeStrClick() { }
    open func onCoinsStrClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "close_btn"
        {
            onCloseBtnClick()
            return true
        }
        else if name == "camera_fb"
        {
            onCameraFbClick()
            return true
        }
        else if name == "astro_fb"
        {
            onAstroFbClick()
            return true
        }
        else if name == "coffee_fb"
        {
            onCoffeeFbClick()
            return true
        }
        else if name == "coins_fb"
        {
            onCoinsFbClick()
            return true
        }
        else if name == "camera_ctn"
        {
            onCameraCtnClick()
            return true
        }
        else if name == "astro_ctn"
        {
            onAstroCtnClick()
            return true
        }
        else if name == "coffee_ctn"
        {
            onCoffeeCtnClick()
            return true
        }
        else if name == "coins_ctn"
        {
            onCoinsCtnClick()
            return true
        }
        else if name == "camera_str"
        {
            onCameraStrClick()
            return true
        }
        else if name == "astro_str"
        {
            onAstroStrClick()
            return true
        }
        else if name == "coffee_str"
        {
            onCoffeeStrClick()
            return true
        }
        else if name == "coins_str"
        {
            onCoinsStrClick()
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
