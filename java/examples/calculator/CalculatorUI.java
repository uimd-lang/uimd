// Auto-generated UI code for calculator - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class CalculatorUI extends GeneratedWindowBase
{
    public Label display;
    public Button n0;
    public Button n1;
    public Button n2;
    public Button n3;
    public Button n4;
    public Button n5;
    public Button n6;
    public Button n7;
    public Button n8;
    public Button n9;
    public Button sb;
    public Button dt;
    public Button ac;
    public Button dl;
    public Button pr;
    public Button dv;
    public Button ml;
    public Button mn;
    public Button ad;

    public CalculatorUI()
    {
        super("Calculator");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "Calculator",
            "# Calculator\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\ndisplay:\n  type: label\n  text: \"\"\n  description: \"Shows the current number, operator result, or calculation state.\"\n\nn0:\n  type: button\n  title: 0\n  description: \"Enter digit 0.\"\n\nn1:\n  type: button\n  title: 1\n  description: \"Enter digit 1.\"\n\nn2:\n  type: button\n  title: 2\n  description: \"Enter digit 2.\"\n\nn3:\n  type: button\n  title: 3\n  description: \"Enter digit 3.\"\n\nn4:\n  type: button\n  title: 4\n  description: \"Enter digit 4.\"\n\nn5:\n  type: button\n  title: 5\n  description: \"Enter digit 5.\"\n\nn6:\n  type: button\n  title: 6\n  description: \"Enter digit 6.\"\n\nn7:\n  type: button\n  title: 7\n  description: \"Enter digit 7.\"\n\nn8:\n  type: button\n  title: 8\n  description: \"Enter digit 8.\"\n\nn9:\n  type: button\n  title: 9\n  description: \"Enter digit 9.\"\n\nsb:\n  type: button\n  title: \"=\"\n  description: \"Evaluate the pending calculation.\"\n\ndt:\n  type: button\n  title: .\n  description: \"Insert a decimal point into the current number.\"\n\nac:\n  type: button\n  title: AC\n  description: \"Clear the current calculator state.\"\n\ndl:\n  type: button\n  title: DEL\n  description: \"Delete the last entered character.\"\n\npr:\n  type: button\n  title: \"%\"\n  description: \"Convert or apply percentage behavior for the current value.\"\n\ndv:\n  type: button\n  title: /\n  description: \"Choose division as the pending operator.\"\n\nml:\n  type: button\n  title: \"*\"\n  description: \"Choose multiplication as the pending operator.\"\n\nmn:\n  type: button\n  title: -\n  description: \"Choose subtraction as the pending operator.\"\n\nad:\n  type: button\n  title: \"+\"\n  description: \"Choose addition as the pending operator.\"\n```\n\n## Style\n\n```yaml\n@display:\n  background: #32446d\n  color: #c8e0ff\n  text-align: right\n```\n\n## User Interface\n\n```ui\n+----------------------+\n|                      |\n|  display...........  |\n|                      |\n|  ac.  dl.  pr.  dv.  |\n|                      |\n|  n7.  n8.  n9.  ml.  |\n|                      |\n|  n4.  n5.  n6.  mn.  |\n|                      |\n|  n1.  n2.  n3.  sb.  |\n|                      |\n|  n0......  dt.  ad.  |\n|                      |\n+----------------------+\n```\n",
            "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.",
            List.of(
            new GeneratedElementMetadata("display", "Shows the current number, operator result, or calculation state.", true),
            new GeneratedElementMetadata("n0", "Enter digit 0.", true),
            new GeneratedElementMetadata("n1", "Enter digit 1.", true),
            new GeneratedElementMetadata("n2", "Enter digit 2.", true),
            new GeneratedElementMetadata("n3", "Enter digit 3.", true),
            new GeneratedElementMetadata("n4", "Enter digit 4.", true),
            new GeneratedElementMetadata("n5", "Enter digit 5.", true),
            new GeneratedElementMetadata("n6", "Enter digit 6.", true),
            new GeneratedElementMetadata("n7", "Enter digit 7.", true),
            new GeneratedElementMetadata("n8", "Enter digit 8.", true),
            new GeneratedElementMetadata("n9", "Enter digit 9.", true),
            new GeneratedElementMetadata("sb", "Evaluate the pending calculation.", true),
            new GeneratedElementMetadata("dt", "Insert a decimal point into the current number.", true),
            new GeneratedElementMetadata("ac", "Clear the current calculator state.", true),
            new GeneratedElementMetadata("dl", "Delete the last entered character.", true),
            new GeneratedElementMetadata("pr", "Convert or apply percentage behavior for the current value.", true),
            new GeneratedElementMetadata("dv", "Choose division as the pending operator.", true),
            new GeneratedElementMetadata("ml", "Choose multiplication as the pending operator.", true),
            new GeneratedElementMetadata("mn", "Choose subtraction as the pending operator.", true),
            new GeneratedElementMetadata("ad", "Choose addition as the pending operator.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        display = addElement(new Label("display", ""));
        display.setStyle(Style.fromProperties("background", "#32446d", "color", "#c8e0ff", "text-align", "right"));
        display.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        n0 = addElement(new Button("n0", "0"));
        n0.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n0.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n0.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n1 = addElement(new Button("n1", "1"));
        n1.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n1.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n1.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n2 = addElement(new Button("n2", "2"));
        n2.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n2.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n2.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n3 = addElement(new Button("n3", "3"));
        n3.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n3.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n3.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n4 = addElement(new Button("n4", "4"));
        n4.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n4.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n4.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n5 = addElement(new Button("n5", "5"));
        n5.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n5.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n5.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n6 = addElement(new Button("n6", "6"));
        n6.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n6.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n6.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n7 = addElement(new Button("n7", "7"));
        n7.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n7.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n7.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n8 = addElement(new Button("n8", "8"));
        n8.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n8.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n8.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        n9 = addElement(new Button("n9", "9"));
        n9.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        n9.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        n9.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        sb = addElement(new Button("sb", "="));
        sb.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        sb.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        sb.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        dt = addElement(new Button("dt", "."));
        dt.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        dt.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        dt.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        ac = addElement(new Button("ac", "AC"));
        ac.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        ac.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        ac.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        dl = addElement(new Button("dl", "DEL"));
        dl.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        dl.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        dl.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        pr = addElement(new Button("pr", "%"));
        pr.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        pr.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        pr.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        dv = addElement(new Button("dv", "/"));
        dv.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        dv.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        dv.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        ml = addElement(new Button("ml", "*"));
        ml.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        ml.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        ml.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        mn = addElement(new Button("mn", "-"));
        mn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        mn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        mn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        ad = addElement(new Button("ad", "+"));
        ad.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        ad.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        ad.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("display")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(1, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(18, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(2)
                .setMarginBottom(11)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#32446d", "color", "#c8e0ff", "text-align", "right")),
            new GeneratedLayoutEntry()
                .setName("ac")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(3, 2, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(17)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("dl")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(3, 7, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(12)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("pr")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(3, 12, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(7)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("dv")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(3, 17, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(2)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n7")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(5, 2, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(17)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n8")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(5, 7, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(12)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n9")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(5, 12, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(7)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("ml")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(5, 17, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(2)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n4")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(7, 2, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(17)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n5")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(7, 7, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(12)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n6")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(7, 12, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(7)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("mn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(7, 17, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(2)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n1")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(9, 2, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(17)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n2")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(9, 7, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(12)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n3")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(9, 12, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(7)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("sb")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(9, 17, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(2)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("n0")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(11, 2, 8, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(8))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(8, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(12)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("dt")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(11, 12, 3, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(7)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("ad")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(11, 17, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 22, 13))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(13))
                .setCharsSize(new Size(3, 1))
                .setCellCharsSize(new Size(22, 13))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onN0Click()
    {
    }

    protected void onN1Click()
    {
    }

    protected void onN2Click()
    {
    }

    protected void onN3Click()
    {
    }

    protected void onN4Click()
    {
    }

    protected void onN5Click()
    {
    }

    protected void onN6Click()
    {
    }

    protected void onN7Click()
    {
    }

    protected void onN8Click()
    {
    }

    protected void onN9Click()
    {
    }

    protected void onSbClick()
    {
    }

    protected void onDtClick()
    {
    }

    protected void onAcClick()
    {
    }

    protected void onDlClick()
    {
    }

    protected void onPrClick()
    {
    }

    protected void onDvClick()
    {
    }

    protected void onMlClick()
    {
    }

    protected void onMnClick()
    {
    }

    protected void onAdClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("n0".equals(name))
        {
            onN0Click();
            return true;
        }
        if ("n1".equals(name))
        {
            onN1Click();
            return true;
        }
        if ("n2".equals(name))
        {
            onN2Click();
            return true;
        }
        if ("n3".equals(name))
        {
            onN3Click();
            return true;
        }
        if ("n4".equals(name))
        {
            onN4Click();
            return true;
        }
        if ("n5".equals(name))
        {
            onN5Click();
            return true;
        }
        if ("n6".equals(name))
        {
            onN6Click();
            return true;
        }
        if ("n7".equals(name))
        {
            onN7Click();
            return true;
        }
        if ("n8".equals(name))
        {
            onN8Click();
            return true;
        }
        if ("n9".equals(name))
        {
            onN9Click();
            return true;
        }
        if ("sb".equals(name))
        {
            onSbClick();
            return true;
        }
        if ("dt".equals(name))
        {
            onDtClick();
            return true;
        }
        if ("ac".equals(name))
        {
            onAcClick();
            return true;
        }
        if ("dl".equals(name))
        {
            onDlClick();
            return true;
        }
        if ("pr".equals(name))
        {
            onPrClick();
            return true;
        }
        if ("dv".equals(name))
        {
            onDvClick();
            return true;
        }
        if ("ml".equals(name))
        {
            onMlClick();
            return true;
        }
        if ("mn".equals(name))
        {
            onMnClick();
            return true;
        }
        if ("ad".equals(name))
        {
            onAdClick();
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
