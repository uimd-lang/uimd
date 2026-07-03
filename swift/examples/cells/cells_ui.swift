// Auto-generated UI code for cells - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class CellsUI: GeneratedWindowBase
{
    public var a1: Button!
    public var a2: Button!
    public var b1: Button!
    public var c1: Button!
    public var c2: Button!
    public var c3: Button!
    public var c4: Button!
    public var c5: Button!
    public var c6: Button!
    public var c7: Button!
    public var c8: Button!
    public var c9: Button!
    public var c10: Button!
    public var d1: Button!
    public var d2: Button!
    public var d3: Button!
    public var d4: Button!
    public var d5: Button!
    public var d6: Button!
    public var e1: Button!
    public var e2: Button!
    public var e3: Button!
    public var e4: Button!
    public var e5: Button!
    public var e6: Button!
    public var e7: Button!
    public var e8: Button!
    public var f1: Button!
    public var f2: Button!
    public var f3: Button!

    public init()
    {
        super.init("Cells")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "Cells", source: "# Cells\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\na1:\n  type: button\n  title: A1\n  description: \"Button in the first red cell.\"\n\na2:\n  type: button\n  title: A2\n  description: \"Second button in the first red cell.\"\n\nb1:\n  type: button\n  title: BBB\n  description: \"Wide button in the green cell.\"\n\nc1:\n  type: button\n  title: C1\n  description: \"Button in the yellow cell group.\"\n\nc2:\n  type: button\n  title: C2\n  description: \"Button in the yellow cell group.\"\n\nc3:\n  type: button\n  title: C3\n  description: \"Button in the yellow cell group.\"\n\nc4:\n  type: button\n  title: C4\n  description: \"Button in the yellow cell group.\"\n\nc5:\n  type: button\n  title: C5\n  description: \"Button in the yellow cell group.\"\n\nc6:\n  type: button\n  title: C6\n  description: \"Button in the yellow cell group.\"\n\nc7:\n  type: button\n  title: C7\n  description: \"Button in the yellow cell group.\"\n\nc8:\n  type: button\n  title: C8\n  description: \"Button in the yellow cell group.\"\n\nc9:\n  type: button\n  title: C9\n  description: \"Button in the yellow cell group.\"\n\nc10:\n  type: button\n  title: C10\n  description: \"Button in the yellow cell group.\"\n\nd1:\n  type: button\n  title: D1\n  description: \"Top button in the right column.\"\n\nd2:\n  type: button\n  title: D2\n  description: \"Button in the right column.\"\n\nd3:\n  type: button\n  title: D3\n  description: \"Button in the right column.\"\n\nd4:\n  type: button\n  title: D4\n  description: \"Button in the right column.\"\n\nd5:\n  type: button\n  title: D5\n  description: \"Button in the right column.\"\n\nd6:\n  type: button\n  title: D6\n  description: \"Bottom button in the right column.\"\n\ne1:\n  type: button\n  title: E\n  description: \"Single-character E button in the lower left cell.\"\n\ne2:\n  type: button\n  title: EE\n  description: \"Two-character E button in the lower left cell.\"\n\ne3:\n  type: button\n  title: EEE\n  description: \"Three-character E button in the lower left cell.\"\n\ne4:\n  type: button\n  title: EEEE\n  description: \"Four-character E button in the lower left cell.\"\n\ne5:\n  type: button\n  title: EEEEE\n  description: \"Five-character E button in the lower left cell.\"\n\ne6:\n  type: button\n  title: EEEEEE\n  description: \"Six-character E button in the lower left cell.\"\n\ne7:\n  type: button\n  title: EEEEEEE\n  description: \"Seven-character E button in the lower left cell.\"\n\ne8:\n  type: button\n  title: EEEEEEEE\n  description: \"Eight-character E button in the lower left cell.\"\n\nf1:\n  type: button\n  title: F1\n  description: \"Top button in the cyan cell.\"\n\nf2:\n  type: button\n  title: F2\n  description: \"Middle button in the cyan cell with a local style override.\"\n\nf3:\n  type: button\n  title: F3\n  description: \"Bottom button in the cyan cell.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#1f2946\"\n  border-color: transparent\n  border-width: 1\n  border-width-vertical: 2\nbutton:\n  background: \"#526173\"\n  color: \"#eef2f7\"\n  focus-background: \"#6b7c91\"\n@d1:\n  color: \"#d7fbe8\"\ncell1:\n  background: \"#3b1f2d\"\n  button:\n    background: \"#7a3f59\"\n    focus-background: \"#96536d\"\ncell2:\n  background: \"#1f463f\"\n  button:\n    background: \"#3d7b6d\"\n    focus-background: \"#4f9484\"\ncl3:\n  background: \"#26345f\"\n  button:\n    background: \"#5267a3\"\n    focus-background: \"#657abd\"\ncell4:\n  background: \"#4a4422\"\n  button:\n    background: \"#81753b\"\n    color: \"#ecfdf5\"\n    focus-background: \"#9a8d4a\"\ncell5:\n  background: \"#482456\"\n  button:\n    background: \"#805196\"\n    focus-background: \"#9a65b0\"\ncl6:\n  background: \"#174f55\"\n  button:\n    background: \"#3a8890\"\n    focus-background: \"#4aa1aa\"\n  @f2:\n    background: \"#31565f\"\ncl7:\n  background: \"#2f3745\"\n  button:\n    background: \"#64748b\"\n    focus-background: \"#7b8ba0\"\n```\n\n## User Interface\n\n```ui\n+-cell1-----------+-cell2---+-cl3+\n| a1..... a2..... |  b1...  | d1 |\n+-cell4-----------+---------+    |\n| c1.. c2.. c3.. c4.. c5..  | d2 |\n| c6.. c7.. c8.. c9.. c10.  |    |\n+-cell5---------------+-cl6-+ d3 |\n|                     | f1. |    |\n| e1....... e5....... |     | d4 |\n|                     |     |    |\n| e2....... e6....... | f2. | d5 |\n|                     |     |    |\n| e3....... e7....... |     | d6 |\n|                     | f3. +-cl7+\n| e4....... e8....... |     |    |\n|                     |     |    |\n+---------------------+-----+----+\n```\n", description: "Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#1f2946")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 1
                    style.borderWidthVertical = 2
                    return style
                }())
        a1 = addElement(Button("a1", "A1"))
        a1.setStyle({
                    var style = Style()
                    style.background = Color("#7a3f59")
                    style.color = Color("#eef2f7")
                    return style
                }())
        a1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#96536d")
                    style.color = Color("#ffffff")
                    return style
                }())
        a1.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        a2 = addElement(Button("a2", "A2"))
        a2.setStyle({
                    var style = Style()
                    style.background = Color("#7a3f59")
                    style.color = Color("#eef2f7")
                    return style
                }())
        a2.setFocusStyle({
                    var style = Style()
                    style.background = Color("#96536d")
                    style.color = Color("#ffffff")
                    return style
                }())
        a2.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        b1 = addElement(Button("b1", "BBB"))
        b1.setStyle({
                    var style = Style()
                    style.background = Color("#3d7b6d")
                    style.color = Color("#eef2f7")
                    return style
                }())
        b1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#4f9484")
                    style.color = Color("#ffffff")
                    return style
                }())
        b1.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c1 = addElement(Button("c1", "C1"))
        c1.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c1.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c2 = addElement(Button("c2", "C2"))
        c2.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c2.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c2.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c3 = addElement(Button("c3", "C3"))
        c3.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c3.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c3.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c4 = addElement(Button("c4", "C4"))
        c4.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c4.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c4.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c5 = addElement(Button("c5", "C5"))
        c5.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c5.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c5.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c6 = addElement(Button("c6", "C6"))
        c6.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c6.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c6.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c7 = addElement(Button("c7", "C7"))
        c7.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c7.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c7.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c8 = addElement(Button("c8", "C8"))
        c8.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c8.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c8.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c9 = addElement(Button("c9", "C9"))
        c9.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c9.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c9.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        c10 = addElement(Button("c10", "C10"))
        c10.setStyle({
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }())
        c10.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a8d4a")
                    style.color = Color("#ffffff")
                    return style
                }())
        c10.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        d1 = addElement(Button("d1", "D1"))
        d1.setStyle({
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#d7fbe8")
                    return style
                }())
        d1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#657abd")
                    style.color = Color("#ffffff")
                    return style
                }())
        d1.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        d2 = addElement(Button("d2", "D2"))
        d2.setStyle({
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }())
        d2.setFocusStyle({
                    var style = Style()
                    style.background = Color("#657abd")
                    style.color = Color("#ffffff")
                    return style
                }())
        d2.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        d3 = addElement(Button("d3", "D3"))
        d3.setStyle({
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }())
        d3.setFocusStyle({
                    var style = Style()
                    style.background = Color("#657abd")
                    style.color = Color("#ffffff")
                    return style
                }())
        d3.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        d4 = addElement(Button("d4", "D4"))
        d4.setStyle({
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }())
        d4.setFocusStyle({
                    var style = Style()
                    style.background = Color("#657abd")
                    style.color = Color("#ffffff")
                    return style
                }())
        d4.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        d5 = addElement(Button("d5", "D5"))
        d5.setStyle({
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }())
        d5.setFocusStyle({
                    var style = Style()
                    style.background = Color("#657abd")
                    style.color = Color("#ffffff")
                    return style
                }())
        d5.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        d6 = addElement(Button("d6", "D6"))
        d6.setStyle({
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }())
        d6.setFocusStyle({
                    var style = Style()
                    style.background = Color("#657abd")
                    style.color = Color("#ffffff")
                    return style
                }())
        d6.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        e1 = addElement(Button("e1", "E"))
        e1.setStyle({
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }())
        e1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a65b0")
                    style.color = Color("#ffffff")
                    return style
                }())
        e1.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        e2 = addElement(Button("e2", "EE"))
        e2.setStyle({
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }())
        e2.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a65b0")
                    style.color = Color("#ffffff")
                    return style
                }())
        e2.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        e3 = addElement(Button("e3", "EEE"))
        e3.setStyle({
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }())
        e3.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a65b0")
                    style.color = Color("#ffffff")
                    return style
                }())
        e3.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        e4 = addElement(Button("e4", "EEEE"))
        e4.setStyle({
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }())
        e4.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a65b0")
                    style.color = Color("#ffffff")
                    return style
                }())
        e4.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        e5 = addElement(Button("e5", "EEEEE"))
        e5.setStyle({
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }())
        e5.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a65b0")
                    style.color = Color("#ffffff")
                    return style
                }())
        e5.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        e6 = addElement(Button("e6", "EEEEEE"))
        e6.setStyle({
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }())
        e6.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a65b0")
                    style.color = Color("#ffffff")
                    return style
                }())
        e6.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        e7 = addElement(Button("e7", "EEEEEEE"))
        e7.setStyle({
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }())
        e7.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a65b0")
                    style.color = Color("#ffffff")
                    return style
                }())
        e7.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        e8 = addElement(Button("e8", "EEEEEEEE"))
        e8.setStyle({
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }())
        e8.setFocusStyle({
                    var style = Style()
                    style.background = Color("#9a65b0")
                    style.color = Color("#ffffff")
                    return style
                }())
        e8.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        f1 = addElement(Button("f1", "F1"))
        f1.setStyle({
                    var style = Style()
                    style.background = Color("#3a8890")
                    style.color = Color("#eef2f7")
                    return style
                }())
        f1.setFocusStyle({
                    var style = Style()
                    style.background = Color("#4aa1aa")
                    style.color = Color("#ffffff")
                    return style
                }())
        f1.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        f2 = addElement(Button("f2", "F2"))
        f2.setStyle({
                    var style = Style()
                    style.background = Color("#31565f")
                    style.color = Color("#eef2f7")
                    return style
                }())
        f2.setFocusStyle({
                    var style = Style()
                    style.background = Color("#4aa1aa")
                    style.color = Color("#ffffff")
                    return style
                }())
        f2.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        f3 = addElement(Button("f3", "F3"))
        f3.setStyle({
                    var style = Style()
                    style.background = Color("#3a8890")
                    style.color = Color("#eef2f7")
                    return style
                }())
        f3.setFocusStyle({
                    var style = Style()
                    style.background = Color("#4aa1aa")
                    style.color = Color("#ffffff")
                    return style
                }())
        f3.setDisabledStyle({
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
                cellName: "cl7",
                relative: Rect(row: 0, col: 0, width: 4, height: 2),
                sourceCell: Rect(row: 12, col: 28, width: 4, height: 2),
                width: .auto(4),
                height: .auto(2),
                cellWidth: .auto(4),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 2),
                cellCharsSize: Size(width: 4, height: 2),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#2f3745")
                    return style
                }(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "a1",
                type: "button",
                cellName: "cell1",
                relative: Rect(row: 0, col: 1, width: 7, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 17, height: 1),
                width: .auto(7),
                height: .auto(1),
                cellWidth: .auto(17),
                cellHeight: .auto(1),
                charsSize: Size(width: 7, height: 1),
                cellCharsSize: Size(width: 17, height: 1),
                marginRight: 9,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#3b1f2d")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7a3f59")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c1",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 0, col: 1, width: 4, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .auto(4),
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 22,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "d1",
                type: "button",
                cellName: "cl3",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 28, width: 4, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(4),
                cellHeight: .auto(11),
                charsSize: Size(width: 2, height: 1),
                cellCharsSize: Size(width: 4, height: 11),
                marginRight: 1,
                marginBottom: 10,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#26345f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#d7fbe8")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "f1",
                type: "button",
                cellName: "cl6",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 5, col: 22, width: 5, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(5),
                cellHeight: .auto(9),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 5, height: 9),
                marginRight: 1,
                marginBottom: 8,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#174f55")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#3a8890")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "b1",
                type: "button",
                cellName: "cell2",
                relative: Rect(row: 0, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 18, width: 9, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(9),
                cellHeight: .auto(1),
                charsSize: Size(width: 5, height: 1),
                cellCharsSize: Size(width: 9, height: 1),
                marginRight: 2,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f463f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#3d7b6d")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c2",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 0, col: 6, width: 4, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .auto(4),
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 17,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "a2",
                type: "button",
                cellName: "cell1",
                relative: Rect(row: 0, col: 9, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 17, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(17),
                cellHeight: .auto(1),
                charsSize: Size(width: 7, height: 1),
                cellCharsSize: Size(width: 17, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#3b1f2d")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7a3f59")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c3",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 0, col: 11, width: 4, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .auto(4),
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 12,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c4",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 0, col: 16, width: 4, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .auto(4),
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 7,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c5",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 0, col: 21, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c6",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 1, col: 1, width: 4, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .auto(4),
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 22,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "e1",
                type: "button",
                cellName: "cell5",
                relative: Rect(row: 1, col: 1, width: 9, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 21, height: 9),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(9),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 21, height: 9),
                marginRight: 11,
                marginBottom: 7,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#482456")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c7",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 1, col: 6, width: 4, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .auto(4),
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 17,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c8",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 1, col: 11, width: 4, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .auto(4),
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 12,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "e5",
                type: "button",
                cellName: "cell5",
                relative: Rect(row: 1, col: 11, width: 536870911, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 21, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(9),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 21, height: 9),
                marginRight: 1,
                marginBottom: 7,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#482456")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c9",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 1, col: 16, width: 4, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .auto(4),
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 7,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "c10",
                type: "button",
                cellName: "cell4",
                relative: Rect(row: 1, col: 21, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 27, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(27),
                cellHeight: .auto(2),
                charsSize: Size(width: 4, height: 1),
                cellCharsSize: Size(width: 27, height: 2),
                marginRight: 2,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#4a4422")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#81753b")
                    style.color = Color("#ecfdf5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "d2",
                type: "button",
                cellName: "cl3",
                relative: Rect(row: 2, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 28, width: 4, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(4),
                cellHeight: .auto(11),
                charsSize: Size(width: 2, height: 1),
                cellCharsSize: Size(width: 4, height: 11),
                marginRight: 1,
                marginBottom: 8,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#26345f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "e2",
                type: "button",
                cellName: "cell5",
                relative: Rect(row: 3, col: 1, width: 9, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 21, height: 9),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(9),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 21, height: 9),
                marginRight: 11,
                marginBottom: 5,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#482456")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "f2",
                type: "button",
                cellName: "cl6",
                relative: Rect(row: 3, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 5, col: 22, width: 5, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(5),
                cellHeight: .auto(9),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 5, height: 9),
                marginRight: 1,
                marginBottom: 5,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#174f55")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#31565f")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "e6",
                type: "button",
                cellName: "cell5",
                relative: Rect(row: 3, col: 11, width: 536870911, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 21, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(9),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 21, height: 9),
                marginRight: 1,
                marginBottom: 5,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#482456")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "d3",
                type: "button",
                cellName: "cl3",
                relative: Rect(row: 4, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 28, width: 4, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(4),
                cellHeight: .auto(11),
                charsSize: Size(width: 2, height: 1),
                cellCharsSize: Size(width: 4, height: 11),
                marginRight: 1,
                marginBottom: 6,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#26345f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "e3",
                type: "button",
                cellName: "cell5",
                relative: Rect(row: 5, col: 1, width: 9, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 21, height: 9),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(9),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 21, height: 9),
                marginRight: 11,
                marginBottom: 3,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#482456")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "e7",
                type: "button",
                cellName: "cell5",
                relative: Rect(row: 5, col: 11, width: 536870911, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 21, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(9),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 21, height: 9),
                marginRight: 1,
                marginBottom: 3,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#482456")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "d4",
                type: "button",
                cellName: "cl3",
                relative: Rect(row: 6, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 28, width: 4, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(4),
                cellHeight: .auto(11),
                charsSize: Size(width: 2, height: 1),
                cellCharsSize: Size(width: 4, height: 11),
                marginRight: 1,
                marginBottom: 4,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#26345f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "f3",
                type: "button",
                cellName: "cl6",
                relative: Rect(row: 6, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 5, col: 22, width: 5, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(5),
                cellHeight: .auto(9),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 5, height: 9),
                marginRight: 1,
                marginBottom: 2,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#174f55")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#3a8890")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "e4",
                type: "button",
                cellName: "cell5",
                relative: Rect(row: 7, col: 1, width: 9, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 21, height: 9),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(9),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 21, height: 9),
                marginRight: 11,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#482456")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "e8",
                type: "button",
                cellName: "cell5",
                relative: Rect(row: 7, col: 11, width: 536870911, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 21, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(21),
                cellHeight: .auto(9),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 21, height: 9),
                marginRight: 1,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#482456")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#805196")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "d5",
                type: "button",
                cellName: "cl3",
                relative: Rect(row: 8, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 28, width: 4, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(4),
                cellHeight: .auto(11),
                charsSize: Size(width: 2, height: 1),
                cellCharsSize: Size(width: 4, height: 11),
                marginRight: 1,
                marginBottom: 2,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#26345f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "d6",
                type: "button",
                cellName: "cl3",
                relative: Rect(row: 10, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 28, width: 4, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(4),
                cellHeight: .auto(11),
                charsSize: Size(width: 2, height: 1),
                cellCharsSize: Size(width: 4, height: 11),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#26345f")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#5267a3")
                    style.color = Color("#eef2f7")
                    return style
                }()
            ),
        ]
    }

    open func onA1Click() { }
    open func onA2Click() { }
    open func onB1Click() { }
    open func onC1Click() { }
    open func onC2Click() { }
    open func onC3Click() { }
    open func onC4Click() { }
    open func onC5Click() { }
    open func onC6Click() { }
    open func onC7Click() { }
    open func onC8Click() { }
    open func onC9Click() { }
    open func onC10Click() { }
    open func onD1Click() { }
    open func onD2Click() { }
    open func onD3Click() { }
    open func onD4Click() { }
    open func onD5Click() { }
    open func onD6Click() { }
    open func onE1Click() { }
    open func onE2Click() { }
    open func onE3Click() { }
    open func onE4Click() { }
    open func onE5Click() { }
    open func onE6Click() { }
    open func onE7Click() { }
    open func onE8Click() { }
    open func onF1Click() { }
    open func onF2Click() { }
    open func onF3Click() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "a1"
        {
            onA1Click()
            return true
        }
        else if name == "a2"
        {
            onA2Click()
            return true
        }
        else if name == "b1"
        {
            onB1Click()
            return true
        }
        else if name == "c1"
        {
            onC1Click()
            return true
        }
        else if name == "c2"
        {
            onC2Click()
            return true
        }
        else if name == "c3"
        {
            onC3Click()
            return true
        }
        else if name == "c4"
        {
            onC4Click()
            return true
        }
        else if name == "c5"
        {
            onC5Click()
            return true
        }
        else if name == "c6"
        {
            onC6Click()
            return true
        }
        else if name == "c7"
        {
            onC7Click()
            return true
        }
        else if name == "c8"
        {
            onC8Click()
            return true
        }
        else if name == "c9"
        {
            onC9Click()
            return true
        }
        else if name == "c10"
        {
            onC10Click()
            return true
        }
        else if name == "d1"
        {
            onD1Click()
            return true
        }
        else if name == "d2"
        {
            onD2Click()
            return true
        }
        else if name == "d3"
        {
            onD3Click()
            return true
        }
        else if name == "d4"
        {
            onD4Click()
            return true
        }
        else if name == "d5"
        {
            onD5Click()
            return true
        }
        else if name == "d6"
        {
            onD6Click()
            return true
        }
        else if name == "e1"
        {
            onE1Click()
            return true
        }
        else if name == "e2"
        {
            onE2Click()
            return true
        }
        else if name == "e3"
        {
            onE3Click()
            return true
        }
        else if name == "e4"
        {
            onE4Click()
            return true
        }
        else if name == "e5"
        {
            onE5Click()
            return true
        }
        else if name == "e6"
        {
            onE6Click()
            return true
        }
        else if name == "e7"
        {
            onE7Click()
            return true
        }
        else if name == "e8"
        {
            onE8Click()
            return true
        }
        else if name == "f1"
        {
            onF1Click()
            return true
        }
        else if name == "f2"
        {
            onF2Click()
            return true
        }
        else if name == "f3"
        {
            onF3Click()
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
