// Auto-generated UI code for calculator - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class CalculatorUI: GeneratedWindowBase
{
    public var display: Label!
    public var n0: Button!
    public var n1: Button!
    public var n2: Button!
    public var n3: Button!
    public var n4: Button!
    public var n5: Button!
    public var n6: Button!
    public var n7: Button!
    public var n8: Button!
    public var n9: Button!
    public var sb: Button!
    public var dt: Button!
    public var ac: Button!
    public var dl: Button!
    public var pr: Button!
    public var dv: Button!
    public var ml: Button!
    public var mn: Button!
    public var ad: Button!

    public init()
    {
        super.init("Calculator")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "Calculator", source: "# Calculator\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\ndisplay:\n  type: label\n  text: \"\"\n  description: \"Shows the current number, operator result, or calculation state.\"\n\nn0:\n  type: button\n  title: 0\n  description: \"Enter digit 0.\"\n\nn1:\n  type: button\n  title: 1\n  description: \"Enter digit 1.\"\n\nn2:\n  type: button\n  title: 2\n  description: \"Enter digit 2.\"\n\nn3:\n  type: button\n  title: 3\n  description: \"Enter digit 3.\"\n\nn4:\n  type: button\n  title: 4\n  description: \"Enter digit 4.\"\n\nn5:\n  type: button\n  title: 5\n  description: \"Enter digit 5.\"\n\nn6:\n  type: button\n  title: 6\n  description: \"Enter digit 6.\"\n\nn7:\n  type: button\n  title: 7\n  description: \"Enter digit 7.\"\n\nn8:\n  type: button\n  title: 8\n  description: \"Enter digit 8.\"\n\nn9:\n  type: button\n  title: 9\n  description: \"Enter digit 9.\"\n\nsb:\n  type: button\n  title: \"=\"\n  description: \"Evaluate the pending calculation.\"\n\ndt:\n  type: button\n  title: .\n  description: \"Insert a decimal point into the current number.\"\n\nac:\n  type: button\n  title: AC\n  description: \"Clear the current calculator state.\"\n\ndl:\n  type: button\n  title: DEL\n  description: \"Delete the last entered character.\"\n\npr:\n  type: button\n  title: \"%\"\n  description: \"Convert or apply percentage behavior for the current value.\"\n\ndv:\n  type: button\n  title: /\n  description: \"Choose division as the pending operator.\"\n\nml:\n  type: button\n  title: \"*\"\n  description: \"Choose multiplication as the pending operator.\"\n\nmn:\n  type: button\n  title: -\n  description: \"Choose subtraction as the pending operator.\"\n\nad:\n  type: button\n  title: \"+\"\n  description: \"Choose addition as the pending operator.\"\n```\n\n## Style\n\n```yaml\n@display:\n  background: #32446d\n  color: #c8e0ff\n  text-align: right\n```\n\n## User Interface\n\n```ui\n+----------------------+\n|                      |\n|  display...........  |\n|                      |\n|  ac.  dl.  pr.  dv.  |\n|                      |\n|  n7.  n8.  n9.  ml.  |\n|                      |\n|  n4.  n5.  n6.  mn.  |\n|                      |\n|  n1.  n2.  n3.  sb.  |\n|                      |\n|  n0......  dt.  ad.  |\n|                      |\n+----------------------+\n```\n", description: "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        display = addElement(Label("display", ""))
        display.setStyle({
                    var style = Style()
                    style.background = Color("#32446d")
                    style.color = Color("#c8e0ff")
                    style.textAlign = "right"
                    return style
                }())
        display.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        n0 = addElement(Button("n0", "0"))
        n0.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n0.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n0.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n1 = addElement(Button("n1", "1"))
        n1.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n1.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n2 = addElement(Button("n2", "2"))
        n2.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n2.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n2.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n3 = addElement(Button("n3", "3"))
        n3.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n3.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n3.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n4 = addElement(Button("n4", "4"))
        n4.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n4.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n4.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n5 = addElement(Button("n5", "5"))
        n5.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n5.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n5.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n6 = addElement(Button("n6", "6"))
        n6.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n6.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n6.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n7 = addElement(Button("n7", "7"))
        n7.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n7.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n7.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n8 = addElement(Button("n8", "8"))
        n8.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n8.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n8.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        n9 = addElement(Button("n9", "9"))
        n9.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        n9.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        n9.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        sb = addElement(Button("sb", "="))
        sb.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        sb.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        sb.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        dt = addElement(Button("dt", "."))
        dt.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        dt.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        dt.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        ac = addElement(Button("ac", "AC"))
        ac.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        ac.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        ac.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        dl = addElement(Button("dl", "DEL"))
        dl.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        dl.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        dl.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        pr = addElement(Button("pr", "%"))
        pr.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        pr.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        pr.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        dv = addElement(Button("dv", "/"))
        dv.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        dv.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        dv.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        ml = addElement(Button("ml", "*"))
        ml.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        ml.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        ml.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        mn = addElement(Button("mn", "-"))
        mn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        mn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        mn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        ad = addElement(Button("ad", "+"))
        ad.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        ad.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        ad.setDisabledStyle({
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
                name: "display",
                type: "label",
                cellName: "",
                relative: Rect(row: 1, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 18, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 2,
                marginBottom: 11,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#32446d")
                    style.color = Color("#c8e0ff")
                    style.textAlign = "right"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "ac",
                type: "button",
                cellName: "",
                relative: Rect(row: 3, col: 2, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 17,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "dl",
                type: "button",
                cellName: "",
                relative: Rect(row: 3, col: 7, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 12,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "pr",
                type: "button",
                cellName: "",
                relative: Rect(row: 3, col: 12, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 7,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "dv",
                type: "button",
                cellName: "",
                relative: Rect(row: 3, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 2,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n7",
                type: "button",
                cellName: "",
                relative: Rect(row: 5, col: 2, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 17,
                marginBottom: 7,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n8",
                type: "button",
                cellName: "",
                relative: Rect(row: 5, col: 7, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 12,
                marginBottom: 7,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n9",
                type: "button",
                cellName: "",
                relative: Rect(row: 5, col: 12, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 7,
                marginBottom: 7,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "ml",
                type: "button",
                cellName: "",
                relative: Rect(row: 5, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 2,
                marginBottom: 7,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n4",
                type: "button",
                cellName: "",
                relative: Rect(row: 7, col: 2, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 17,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n5",
                type: "button",
                cellName: "",
                relative: Rect(row: 7, col: 7, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 12,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n6",
                type: "button",
                cellName: "",
                relative: Rect(row: 7, col: 12, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 7,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "mn",
                type: "button",
                cellName: "",
                relative: Rect(row: 7, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 2,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n1",
                type: "button",
                cellName: "",
                relative: Rect(row: 9, col: 2, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 17,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n2",
                type: "button",
                cellName: "",
                relative: Rect(row: 9, col: 7, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 12,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n3",
                type: "button",
                cellName: "",
                relative: Rect(row: 9, col: 12, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 7,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "sb",
                type: "button",
                cellName: "",
                relative: Rect(row: 9, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 2,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "n0",
                type: "button",
                cellName: "",
                relative: Rect(row: 11, col: 2, width: 8, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(8),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 8, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 12,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "dt",
                type: "button",
                cellName: "",
                relative: Rect(row: 11, col: 12, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .auto(3),
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 7,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "ad",
                type: "button",
                cellName: "",
                relative: Rect(row: 11, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 22, height: 13),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(13),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 22, height: 13),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onN0Click() { }
    open func onN1Click() { }
    open func onN2Click() { }
    open func onN3Click() { }
    open func onN4Click() { }
    open func onN5Click() { }
    open func onN6Click() { }
    open func onN7Click() { }
    open func onN8Click() { }
    open func onN9Click() { }
    open func onSbClick() { }
    open func onDtClick() { }
    open func onAcClick() { }
    open func onDlClick() { }
    open func onPrClick() { }
    open func onDvClick() { }
    open func onMlClick() { }
    open func onMnClick() { }
    open func onAdClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "n0"
        {
            onN0Click()
            return true
        }
        else if name == "n1"
        {
            onN1Click()
            return true
        }
        else if name == "n2"
        {
            onN2Click()
            return true
        }
        else if name == "n3"
        {
            onN3Click()
            return true
        }
        else if name == "n4"
        {
            onN4Click()
            return true
        }
        else if name == "n5"
        {
            onN5Click()
            return true
        }
        else if name == "n6"
        {
            onN6Click()
            return true
        }
        else if name == "n7"
        {
            onN7Click()
            return true
        }
        else if name == "n8"
        {
            onN8Click()
            return true
        }
        else if name == "n9"
        {
            onN9Click()
            return true
        }
        else if name == "sb"
        {
            onSbClick()
            return true
        }
        else if name == "dt"
        {
            onDtClick()
            return true
        }
        else if name == "ac"
        {
            onAcClick()
            return true
        }
        else if name == "dl"
        {
            onDlClick()
            return true
        }
        else if name == "pr"
        {
            onPrClick()
            return true
        }
        else if name == "dv"
        {
            onDvClick()
            return true
        }
        else if name == "ml"
        {
            onMlClick()
            return true
        }
        else if name == "mn"
        {
            onMnClick()
            return true
        }
        else if name == "ad"
        {
            onAdClick()
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
