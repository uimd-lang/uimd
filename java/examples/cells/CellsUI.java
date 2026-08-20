// Auto-generated UI code for cells - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class CellsUI extends GeneratedWindowBase
{
    public Button a1;
    public Button a2;
    public Button b1;
    public Button c1;
    public Button c2;
    public Button c3;
    public Button c4;
    public Button c5;
    public Button c6;
    public Button c7;
    public Button c8;
    public Button c9;
    public Button c10;
    public Button d1;
    public Button d2;
    public Button d3;
    public Button d4;
    public Button d5;
    public Button d6;
    public Button e1;
    public Button e2;
    public Button e3;
    public Button e4;
    public Button e5;
    public Button e6;
    public Button e7;
    public Button e8;
    public Button f1;
    public Button f2;
    public Button f3;

    public CellsUI()
    {
        super("Cells");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "Cells",
            "# Cells\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\na1:\n  type: button\n  title: A1\n  description: \"Button in the first red cell.\"\n\na2:\n  type: button\n  title: A2\n  description: \"Second button in the first red cell.\"\n\nb1:\n  type: button\n  title: BBB\n  description: \"Wide button in the green cell.\"\n\nc1:\n  type: button\n  title: C1\n  description: \"Button in the yellow cell group.\"\n\nc2:\n  type: button\n  title: C2\n  description: \"Button in the yellow cell group.\"\n\nc3:\n  type: button\n  title: C3\n  description: \"Button in the yellow cell group.\"\n\nc4:\n  type: button\n  title: C4\n  description: \"Button in the yellow cell group.\"\n\nc5:\n  type: button\n  title: C5\n  description: \"Button in the yellow cell group.\"\n\nc6:\n  type: button\n  title: C6\n  description: \"Button in the yellow cell group.\"\n\nc7:\n  type: button\n  title: C7\n  description: \"Button in the yellow cell group.\"\n\nc8:\n  type: button\n  title: C8\n  description: \"Button in the yellow cell group.\"\n\nc9:\n  type: button\n  title: C9\n  description: \"Button in the yellow cell group.\"\n\nc10:\n  type: button\n  title: C10\n  description: \"Button in the yellow cell group.\"\n\nd1:\n  type: button\n  title: D1\n  description: \"Top button in the right column.\"\n\nd2:\n  type: button\n  title: D2\n  description: \"Button in the right column.\"\n\nd3:\n  type: button\n  title: D3\n  description: \"Button in the right column.\"\n\nd4:\n  type: button\n  title: D4\n  description: \"Button in the right column.\"\n\nd5:\n  type: button\n  title: D5\n  description: \"Button in the right column.\"\n\nd6:\n  type: button\n  title: D6\n  description: \"Bottom button in the right column.\"\n\ne1:\n  type: button\n  title: E\n  description: \"Single-character E button in the lower left cell.\"\n\ne2:\n  type: button\n  title: EE\n  description: \"Two-character E button in the lower left cell.\"\n\ne3:\n  type: button\n  title: EEE\n  description: \"Three-character E button in the lower left cell.\"\n\ne4:\n  type: button\n  title: EEEE\n  description: \"Four-character E button in the lower left cell.\"\n\ne5:\n  type: button\n  title: EEEEE\n  description: \"Five-character E button in the lower left cell.\"\n\ne6:\n  type: button\n  title: EEEEEE\n  description: \"Six-character E button in the lower left cell.\"\n\ne7:\n  type: button\n  title: EEEEEEE\n  description: \"Seven-character E button in the lower left cell.\"\n\ne8:\n  type: button\n  title: EEEEEEEE\n  description: \"Eight-character E button in the lower left cell.\"\n\nf1:\n  type: button\n  title: F1\n  description: \"Top button in the cyan cell.\"\n\nf2:\n  type: button\n  title: F2\n  description: \"Middle button in the cyan cell with a local style override.\"\n\nf3:\n  type: button\n  title: F3\n  description: \"Bottom button in the cyan cell.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#1f2946\"\n  border-color: transparent\n  border-width: 1\n  border-width-vertical: 2\nbutton:\n  background: \"#526173\"\n  color: \"#eef2f7\"\n  focus-background: \"#6b7c91\"\n@d1:\n  color: \"#d7fbe8\"\ncell1:\n  background: \"#3b1f2d\"\n  button:\n    background: \"#7a3f59\"\n    focus-background: \"#96536d\"\ncell2:\n  background: \"#1f463f\"\n  button:\n    background: \"#3d7b6d\"\n    focus-background: \"#4f9484\"\ncl3:\n  background: \"#26345f\"\n  button:\n    background: \"#5267a3\"\n    focus-background: \"#657abd\"\ncell4:\n  background: \"#4a4422\"\n  button:\n    background: \"#81753b\"\n    color: \"#ecfdf5\"\n    focus-background: \"#9a8d4a\"\ncell5:\n  background: \"#482456\"\n  button:\n    background: \"#805196\"\n    focus-background: \"#9a65b0\"\ncl6:\n  background: \"#174f55\"\n  button:\n    background: \"#3a8890\"\n    focus-background: \"#4aa1aa\"\n  @f2:\n    background: \"#31565f\"\ncl7:\n  background: \"#2f3745\"\n  button:\n    background: \"#64748b\"\n    focus-background: \"#7b8ba0\"\n```\n\n## User Interface\n\n```ui\n+-cell1-----------+-cell2---+-cl3+\n| a1..... a2..... |  b1...  | d1 |\n+-cell4-----------+---------+    |\n| c1.. c2.. c3.. c4.. c5..  | d2 |\n| c6.. c7.. c8.. c9.. c10.  |    |\n+-cell5---------------+-cl6-+ d3 |\n|                     | f1. |    |\n| e1....... e5....... |     | d4 |\n|                     |     |    |\n| e2....... e6....... | f2. | d5 |\n|                     |     |    |\n| e3....... e7....... |     | d6 |\n|                     | f3. +-cl7+\n| e4....... e8....... |     |    |\n|                     |     |    |\n+---------------------+-----+----+\n```\n",
            "Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.",
            List.of(
            new GeneratedElementMetadata("a1", "Button in the first red cell.", true),
            new GeneratedElementMetadata("a2", "Second button in the first red cell.", true),
            new GeneratedElementMetadata("b1", "Wide button in the green cell.", true),
            new GeneratedElementMetadata("c1", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c2", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c3", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c4", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c5", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c6", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c7", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c8", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c9", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("c10", "Button in the yellow cell group.", true),
            new GeneratedElementMetadata("d1", "Top button in the right column.", true),
            new GeneratedElementMetadata("d2", "Button in the right column.", true),
            new GeneratedElementMetadata("d3", "Button in the right column.", true),
            new GeneratedElementMetadata("d4", "Button in the right column.", true),
            new GeneratedElementMetadata("d5", "Button in the right column.", true),
            new GeneratedElementMetadata("d6", "Bottom button in the right column.", true),
            new GeneratedElementMetadata("e1", "Single-character E button in the lower left cell.", true),
            new GeneratedElementMetadata("e2", "Two-character E button in the lower left cell.", true),
            new GeneratedElementMetadata("e3", "Three-character E button in the lower left cell.", true),
            new GeneratedElementMetadata("e4", "Four-character E button in the lower left cell.", true),
            new GeneratedElementMetadata("e5", "Five-character E button in the lower left cell.", true),
            new GeneratedElementMetadata("e6", "Six-character E button in the lower left cell.", true),
            new GeneratedElementMetadata("e7", "Seven-character E button in the lower left cell.", true),
            new GeneratedElementMetadata("e8", "Eight-character E button in the lower left cell.", true),
            new GeneratedElementMetadata("f1", "Top button in the cyan cell.", true),
            new GeneratedElementMetadata("f2", "Middle button in the cyan cell with a local style override.", true),
            new GeneratedElementMetadata("f3", "Bottom button in the cyan cell.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#1f2946", "border-color", "transparent", "border-width-horizontal", "1", "border-width-vertical", "2"));
        a1 = addElement(new Button("a1", "A1"));
        a1.setStyle(Style.fromProperties("background", "#7a3f59", "color", "#eef2f7"));
        a1.setFocusStyle(Style.fromProperties("background", "#96536d", "color", "#ffffff"));
        a1.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        a2 = addElement(new Button("a2", "A2"));
        a2.setStyle(Style.fromProperties("background", "#7a3f59", "color", "#eef2f7"));
        a2.setFocusStyle(Style.fromProperties("background", "#96536d", "color", "#ffffff"));
        a2.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        b1 = addElement(new Button("b1", "BBB"));
        b1.setStyle(Style.fromProperties("background", "#3d7b6d", "color", "#eef2f7"));
        b1.setFocusStyle(Style.fromProperties("background", "#4f9484", "color", "#ffffff"));
        b1.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c1 = addElement(new Button("c1", "C1"));
        c1.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c1.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c1.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c2 = addElement(new Button("c2", "C2"));
        c2.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c2.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c2.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c3 = addElement(new Button("c3", "C3"));
        c3.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c3.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c3.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c4 = addElement(new Button("c4", "C4"));
        c4.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c4.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c4.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c5 = addElement(new Button("c5", "C5"));
        c5.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c5.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c5.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c6 = addElement(new Button("c6", "C6"));
        c6.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c6.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c6.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c7 = addElement(new Button("c7", "C7"));
        c7.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c7.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c7.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c8 = addElement(new Button("c8", "C8"));
        c8.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c8.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c8.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c9 = addElement(new Button("c9", "C9"));
        c9.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c9.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c9.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        c10 = addElement(new Button("c10", "C10"));
        c10.setStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5"));
        c10.setFocusStyle(Style.fromProperties("background", "#9a8d4a", "color", "#ffffff"));
        c10.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        d1 = addElement(new Button("d1", "D1"));
        d1.setStyle(Style.fromProperties("background", "#5267a3", "color", "#d7fbe8"));
        d1.setFocusStyle(Style.fromProperties("background", "#657abd", "color", "#ffffff"));
        d1.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        d2 = addElement(new Button("d2", "D2"));
        d2.setStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7"));
        d2.setFocusStyle(Style.fromProperties("background", "#657abd", "color", "#ffffff"));
        d2.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        d3 = addElement(new Button("d3", "D3"));
        d3.setStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7"));
        d3.setFocusStyle(Style.fromProperties("background", "#657abd", "color", "#ffffff"));
        d3.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        d4 = addElement(new Button("d4", "D4"));
        d4.setStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7"));
        d4.setFocusStyle(Style.fromProperties("background", "#657abd", "color", "#ffffff"));
        d4.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        d5 = addElement(new Button("d5", "D5"));
        d5.setStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7"));
        d5.setFocusStyle(Style.fromProperties("background", "#657abd", "color", "#ffffff"));
        d5.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        d6 = addElement(new Button("d6", "D6"));
        d6.setStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7"));
        d6.setFocusStyle(Style.fromProperties("background", "#657abd", "color", "#ffffff"));
        d6.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        e1 = addElement(new Button("e1", "E"));
        e1.setStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7"));
        e1.setFocusStyle(Style.fromProperties("background", "#9a65b0", "color", "#ffffff"));
        e1.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        e2 = addElement(new Button("e2", "EE"));
        e2.setStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7"));
        e2.setFocusStyle(Style.fromProperties("background", "#9a65b0", "color", "#ffffff"));
        e2.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        e3 = addElement(new Button("e3", "EEE"));
        e3.setStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7"));
        e3.setFocusStyle(Style.fromProperties("background", "#9a65b0", "color", "#ffffff"));
        e3.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        e4 = addElement(new Button("e4", "EEEE"));
        e4.setStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7"));
        e4.setFocusStyle(Style.fromProperties("background", "#9a65b0", "color", "#ffffff"));
        e4.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        e5 = addElement(new Button("e5", "EEEEE"));
        e5.setStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7"));
        e5.setFocusStyle(Style.fromProperties("background", "#9a65b0", "color", "#ffffff"));
        e5.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        e6 = addElement(new Button("e6", "EEEEEE"));
        e6.setStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7"));
        e6.setFocusStyle(Style.fromProperties("background", "#9a65b0", "color", "#ffffff"));
        e6.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        e7 = addElement(new Button("e7", "EEEEEEE"));
        e7.setStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7"));
        e7.setFocusStyle(Style.fromProperties("background", "#9a65b0", "color", "#ffffff"));
        e7.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        e8 = addElement(new Button("e8", "EEEEEEEE"));
        e8.setStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7"));
        e8.setFocusStyle(Style.fromProperties("background", "#9a65b0", "color", "#ffffff"));
        e8.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        f1 = addElement(new Button("f1", "F1"));
        f1.setStyle(Style.fromProperties("background", "#3a8890", "color", "#eef2f7"));
        f1.setFocusStyle(Style.fromProperties("background", "#4aa1aa", "color", "#ffffff"));
        f1.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        f2 = addElement(new Button("f2", "F2"));
        f2.setStyle(Style.fromProperties("background", "#31565f", "color", "#eef2f7"));
        f2.setFocusStyle(Style.fromProperties("background", "#4aa1aa", "color", "#ffffff"));
        f2.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        f3 = addElement(new Button("f3", "F3"));
        f3.setStyle(Style.fromProperties("background", "#3a8890", "color", "#eef2f7"));
        f3.setFocusStyle(Style.fromProperties("background", "#4aa1aa", "color", "#ffffff"));
        f3.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
    }

    private static TextGradient makeTextGradient(
        int intervalMs,
        int step,
        int segmentSize,
        String... colors)
    {
        TextGradient gradient = new TextGradient();
        gradient.setIntervalMs(intervalMs);
        gradient.setStep(step);
        gradient.setSegmentSize(segmentSize);
        for (String color : colors)
        {
            gradient.colors().add(new Color(color));
        }
        return gradient;
    }

    private static Style styleWithGradients(
        Style style,
        TextGradient colorGradient,
        TextGradient backgroundGradient)
    {
        style.setTextColorGradient(colorGradient);
        style.setTextBackgroundGradient(backgroundGradient);
        return style;
    }

    private static List<GeneratedLayoutEntry> buildLayout()
    {
        return List.of(
            new GeneratedLayoutEntry()
                .setName("")
                .setType("")
                .setCellName("cl7")
                .setRelative(new Rect(0, 0, 4, 2))
                .setSourceCell(new Rect(12, 28, 4, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(2))
                .setCellWidth(AxisDimension.auto(4))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 2))
                .setCellCharsSize(new Size(4, 2))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#2f3745"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("a1")
                .setType("button")
                .setCellName("cell1")
                .setRelative(new Rect(0, 1, 7, 1))
                .setSourceCell(new Rect(0, 0, 17, 1))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(17))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(7, 1))
                .setCellCharsSize(new Size(17, 1))
                .setMarginRight(9)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#3b1f2d"))
                .setElementStyle(Style.fromProperties("background", "#7a3f59", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("c1")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(0, 1, 4, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(22)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("d1")
                .setType("button")
                .setCellName("cl3")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 28, 4, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(4))
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(2, 1))
                .setCellCharsSize(new Size(4, 11))
                .setMarginRight(1)
                .setMarginBottom(10)
                .setCellStyle(Style.fromProperties("background", "#26345f"))
                .setElementStyle(Style.fromProperties("background", "#5267a3", "color", "#d7fbe8")),
            new GeneratedLayoutEntry()
                .setName("f1")
                .setType("button")
                .setCellName("cl6")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(5, 22, 5, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(5))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(5, 9))
                .setMarginRight(1)
                .setMarginBottom(8)
                .setCellStyle(Style.fromProperties("background", "#174f55"))
                .setElementStyle(Style.fromProperties("background", "#3a8890", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("b1")
                .setType("button")
                .setCellName("cell2")
                .setRelative(new Rect(0, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 18, 9, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(9))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(5, 1))
                .setCellCharsSize(new Size(9, 1))
                .setMarginRight(2)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f463f"))
                .setElementStyle(Style.fromProperties("background", "#3d7b6d", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("c2")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(0, 6, 4, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(17)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("a2")
                .setType("button")
                .setCellName("cell1")
                .setRelative(new Rect(0, 9, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 17, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(17))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(7, 1))
                .setCellCharsSize(new Size(17, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#3b1f2d"))
                .setElementStyle(Style.fromProperties("background", "#7a3f59", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("c3")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(0, 11, 4, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(12)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("c4")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(0, 16, 4, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(7)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("c5")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(0, 21, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("c6")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(1, 1, 4, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(22)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("e1")
                .setType("button")
                .setCellName("cell5")
                .setRelative(new Rect(1, 1, 9, 1))
                .setSourceCell(new Rect(5, 0, 21, 9))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(21, 9))
                .setMarginRight(11)
                .setMarginBottom(7)
                .setCellStyle(Style.fromProperties("background", "#482456"))
                .setElementStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("c7")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(1, 6, 4, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(17)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("c8")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(1, 11, 4, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(12)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("e5")
                .setType("button")
                .setCellName("cell5")
                .setRelative(new Rect(1, 11, 536870911, 1))
                .setSourceCell(new Rect(5, 0, 21, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(21, 9))
                .setMarginRight(1)
                .setMarginBottom(7)
                .setCellStyle(Style.fromProperties("background", "#482456"))
                .setElementStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("c9")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(1, 16, 4, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.auto(4))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(7)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("c10")
                .setType("button")
                .setCellName("cell4")
                .setRelative(new Rect(1, 21, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 27, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(4, 1))
                .setCellCharsSize(new Size(27, 2))
                .setMarginRight(2)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#4a4422"))
                .setElementStyle(Style.fromProperties("background", "#81753b", "color", "#ecfdf5")),
            new GeneratedLayoutEntry()
                .setName("d2")
                .setType("button")
                .setCellName("cl3")
                .setRelative(new Rect(2, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 28, 4, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(4))
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(2, 1))
                .setCellCharsSize(new Size(4, 11))
                .setMarginRight(1)
                .setMarginBottom(8)
                .setCellStyle(Style.fromProperties("background", "#26345f"))
                .setElementStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("e2")
                .setType("button")
                .setCellName("cell5")
                .setRelative(new Rect(3, 1, 9, 1))
                .setSourceCell(new Rect(5, 0, 21, 9))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(21, 9))
                .setMarginRight(11)
                .setMarginBottom(5)
                .setCellStyle(Style.fromProperties("background", "#482456"))
                .setElementStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("f2")
                .setType("button")
                .setCellName("cl6")
                .setRelative(new Rect(3, 1, 536870911, 1))
                .setSourceCell(new Rect(5, 22, 5, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(5))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(5, 9))
                .setMarginRight(1)
                .setMarginBottom(5)
                .setCellStyle(Style.fromProperties("background", "#174f55"))
                .setElementStyle(Style.fromProperties("background", "#31565f", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("e6")
                .setType("button")
                .setCellName("cell5")
                .setRelative(new Rect(3, 11, 536870911, 1))
                .setSourceCell(new Rect(5, 0, 21, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(21, 9))
                .setMarginRight(1)
                .setMarginBottom(5)
                .setCellStyle(Style.fromProperties("background", "#482456"))
                .setElementStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("d3")
                .setType("button")
                .setCellName("cl3")
                .setRelative(new Rect(4, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 28, 4, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(4))
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(2, 1))
                .setCellCharsSize(new Size(4, 11))
                .setMarginRight(1)
                .setMarginBottom(6)
                .setCellStyle(Style.fromProperties("background", "#26345f"))
                .setElementStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("e3")
                .setType("button")
                .setCellName("cell5")
                .setRelative(new Rect(5, 1, 9, 1))
                .setSourceCell(new Rect(5, 0, 21, 9))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(21, 9))
                .setMarginRight(11)
                .setMarginBottom(3)
                .setCellStyle(Style.fromProperties("background", "#482456"))
                .setElementStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("e7")
                .setType("button")
                .setCellName("cell5")
                .setRelative(new Rect(5, 11, 536870911, 1))
                .setSourceCell(new Rect(5, 0, 21, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(21, 9))
                .setMarginRight(1)
                .setMarginBottom(3)
                .setCellStyle(Style.fromProperties("background", "#482456"))
                .setElementStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("d4")
                .setType("button")
                .setCellName("cl3")
                .setRelative(new Rect(6, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 28, 4, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(4))
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(2, 1))
                .setCellCharsSize(new Size(4, 11))
                .setMarginRight(1)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#26345f"))
                .setElementStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("f3")
                .setType("button")
                .setCellName("cl6")
                .setRelative(new Rect(6, 1, 536870911, 1))
                .setSourceCell(new Rect(5, 22, 5, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(5))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(5, 9))
                .setMarginRight(1)
                .setMarginBottom(2)
                .setCellStyle(Style.fromProperties("background", "#174f55"))
                .setElementStyle(Style.fromProperties("background", "#3a8890", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("e4")
                .setType("button")
                .setCellName("cell5")
                .setRelative(new Rect(7, 1, 9, 1))
                .setSourceCell(new Rect(5, 0, 21, 9))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(21, 9))
                .setMarginRight(11)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#482456"))
                .setElementStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("e8")
                .setType("button")
                .setCellName("cell5")
                .setRelative(new Rect(7, 11, 536870911, 1))
                .setSourceCell(new Rect(5, 0, 21, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(21, 9))
                .setMarginRight(1)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#482456"))
                .setElementStyle(Style.fromProperties("background", "#805196", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("d5")
                .setType("button")
                .setCellName("cl3")
                .setRelative(new Rect(8, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 28, 4, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(4))
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(2, 1))
                .setCellCharsSize(new Size(4, 11))
                .setMarginRight(1)
                .setMarginBottom(2)
                .setCellStyle(Style.fromProperties("background", "#26345f"))
                .setElementStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7")),
            new GeneratedLayoutEntry()
                .setName("d6")
                .setType("button")
                .setCellName("cl3")
                .setRelative(new Rect(10, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 28, 4, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(4))
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(2, 1))
                .setCellCharsSize(new Size(4, 11))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#26345f"))
                .setElementStyle(Style.fromProperties("background", "#5267a3", "color", "#eef2f7"))
        );
    }

    protected void onA1Click()
    {
    }

    protected void onA2Click()
    {
    }

    protected void onB1Click()
    {
    }

    protected void onC1Click()
    {
    }

    protected void onC2Click()
    {
    }

    protected void onC3Click()
    {
    }

    protected void onC4Click()
    {
    }

    protected void onC5Click()
    {
    }

    protected void onC6Click()
    {
    }

    protected void onC7Click()
    {
    }

    protected void onC8Click()
    {
    }

    protected void onC9Click()
    {
    }

    protected void onC10Click()
    {
    }

    protected void onD1Click()
    {
    }

    protected void onD2Click()
    {
    }

    protected void onD3Click()
    {
    }

    protected void onD4Click()
    {
    }

    protected void onD5Click()
    {
    }

    protected void onD6Click()
    {
    }

    protected void onE1Click()
    {
    }

    protected void onE2Click()
    {
    }

    protected void onE3Click()
    {
    }

    protected void onE4Click()
    {
    }

    protected void onE5Click()
    {
    }

    protected void onE6Click()
    {
    }

    protected void onE7Click()
    {
    }

    protected void onE8Click()
    {
    }

    protected void onF1Click()
    {
    }

    protected void onF2Click()
    {
    }

    protected void onF3Click()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("a1".equals(name))
        {
            onA1Click();
            return true;
        }
        if ("a2".equals(name))
        {
            onA2Click();
            return true;
        }
        if ("b1".equals(name))
        {
            onB1Click();
            return true;
        }
        if ("c1".equals(name))
        {
            onC1Click();
            return true;
        }
        if ("c2".equals(name))
        {
            onC2Click();
            return true;
        }
        if ("c3".equals(name))
        {
            onC3Click();
            return true;
        }
        if ("c4".equals(name))
        {
            onC4Click();
            return true;
        }
        if ("c5".equals(name))
        {
            onC5Click();
            return true;
        }
        if ("c6".equals(name))
        {
            onC6Click();
            return true;
        }
        if ("c7".equals(name))
        {
            onC7Click();
            return true;
        }
        if ("c8".equals(name))
        {
            onC8Click();
            return true;
        }
        if ("c9".equals(name))
        {
            onC9Click();
            return true;
        }
        if ("c10".equals(name))
        {
            onC10Click();
            return true;
        }
        if ("d1".equals(name))
        {
            onD1Click();
            return true;
        }
        if ("d2".equals(name))
        {
            onD2Click();
            return true;
        }
        if ("d3".equals(name))
        {
            onD3Click();
            return true;
        }
        if ("d4".equals(name))
        {
            onD4Click();
            return true;
        }
        if ("d5".equals(name))
        {
            onD5Click();
            return true;
        }
        if ("d6".equals(name))
        {
            onD6Click();
            return true;
        }
        if ("e1".equals(name))
        {
            onE1Click();
            return true;
        }
        if ("e2".equals(name))
        {
            onE2Click();
            return true;
        }
        if ("e3".equals(name))
        {
            onE3Click();
            return true;
        }
        if ("e4".equals(name))
        {
            onE4Click();
            return true;
        }
        if ("e5".equals(name))
        {
            onE5Click();
            return true;
        }
        if ("e6".equals(name))
        {
            onE6Click();
            return true;
        }
        if ("e7".equals(name))
        {
            onE7Click();
            return true;
        }
        if ("e8".equals(name))
        {
            onE8Click();
            return true;
        }
        if ("f1".equals(name))
        {
            onF1Click();
            return true;
        }
        if ("f2".equals(name))
        {
            onF2Click();
            return true;
        }
        if ("f3".equals(name))
        {
            onF3Click();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        return false;
    }
}
