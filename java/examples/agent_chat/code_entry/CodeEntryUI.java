// Auto-generated UI code for code_entry - DO NOT EDIT MANUALLY.
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

public class CodeEntryUI extends GeneratedWindowBase
{
    public SpanLabel heading;
    public Label code;

    public CodeEntryUI()
    {
        super("Agent Chat Code Entry");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "CodeEntry",
            "# Agent Chat Code Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style code, activity, command output, or diff block.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Code\"\n  description: \"Code language or activity title.\"\n\ncode:\n  type: label\n  text: \"\"\n  description: \"Selectable formatted code, command output, or diff content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@code_panel:\n  background: \"#0f1b2d\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#ddd6fe\"\n@code:\n  color: \"#d1d5db\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-code_panel-**-----------------------------------------------------------+\n| heading..**............................................................ |\n| code..**............................................................... |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n",
            "Heron-style code, activity, command output, or diff block.",
            List.of(
            new GeneratedElementMetadata("heading", "Code language or activity title.", true),
            new GeneratedElementMetadata("code", "Selectable formatted code, command output, or diff content.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        heading = addElement(new SpanLabel("heading", "Code"));
        heading.setStyle(Style.fromProperties("color", "#ddd6fe"));
        code = addElement(new Label("code", ""));
        code.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db", "user-select", "text"));
        code.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setName("heading")
                .setType("spanlabel")
                .setCellName("code_panel")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 73, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(71, 1))
                .setCellCharsSize(new Size(73, 5))
                .setMarginRight(1)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#0f1b2d", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("color", "#ddd6fe")),
            new GeneratedLayoutEntry()
                .setName("code")
                .setType("label")
                .setCellName("code_panel")
                .setRelative(new Rect(1, 1, 536870911, 536870910))
                .setSourceCell(new Rect(0, 0, 73, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(71, 4))
                .setCellCharsSize(new Size(73, 5))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#0f1b2d", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db", "user-select", "text"))
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
