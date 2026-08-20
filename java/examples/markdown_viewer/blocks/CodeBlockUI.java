// Auto-generated UI code for code_block - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.Size;
import uimd.SpanLabel;
import uimd.Style;
import uimd.TextGradient;

public class CodeBlockUI extends GeneratedWindowBase
{
    public Label lang;
    public SpanLabel code;

    public CodeBlockUI()
    {
        super("Code Block");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "CodeBlock",
            "# Code Block\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown code block with a language header and selectable code text.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nlang:\n  type: label\n  text: code\n  description: \"Language label for the code block.\"\n\ncode:\n  type: spanlabel\n  text: \"\"\n  description: \"Rendered code content with horizontal preservation and mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@head:\n  background: \"#1d314c\"\n@body:\n  background: \"#0c1a2d\"\n@lang:\n  color: \"#93c5fd\"\n@code:\n  color: \"#d1d5db\"\n  word-wrap: false\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-head-**----------------------------------------+\n| lang..**...................................... |\n+-body-**----------------------------------------+\n| code..**...................................... |\n| .............................................. |\n# #............................................ |\n# #............................................ |\n+------------------------------------------------+\n```\n",
            "Reusable markdown code block with a language header and selectable code text.",
            List.of(
            new GeneratedElementMetadata("lang", "Language label for the code block.", true),
            new GeneratedElementMetadata("code", "Rendered code content with horizontal preservation and mouse selection enabled.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        lang = addElement(new Label("lang", "code"));
        lang.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        lang.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        code = addElement(new SpanLabel("code", ""));
        code.setStyle(Style.fromProperties("color", "#d1d5db", "user-select", "text"));
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
                .setName("code")
                .setType("spanlabel")
                .setCellName("body")
                .setRelative(new Rect(0, 1, 536870911, 536870910))
                .setSourceCell(new Rect(2, 0, 48, 4))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(46, 4))
                .setCellCharsSize(new Size(48, 4))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#0c1a2d"))
                .setElementStyle(Style.fromProperties("color", "#d1d5db", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("lang")
                .setType("label")
                .setCellName("head")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 48, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(46, 1))
                .setCellCharsSize(new Size(48, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1d314c"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"))
        );
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
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
