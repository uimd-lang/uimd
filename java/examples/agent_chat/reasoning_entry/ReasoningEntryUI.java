// Auto-generated UI code for reasoning_entry - DO NOT EDIT MANUALLY.
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

public class ReasoningEntryUI extends GeneratedWindowBase
{
    public Label heading;
    public SpanLabel body;

    public ReasoningEntryUI()
    {
        super("Agent Chat Reasoning Entry");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ReasoningEntry",
            "# Agent Chat Reasoning Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Animated Heron-style thinking and status message.\"\ntags: [component, simulation, reasoning]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: label\n  text: \"Thinking\"\n  description: \"Animated thinking or activity phase.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Visible simulated reasoning detail.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#e5e7eb\"\n  text-color-gradient:\n    interval: 90\n    step: 1\n    segment-size: 1\n    colors: [\"#000000aa\", \"#00000077\", \"#00000044\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000044\", \"#00000077\", \"#000000aa\"]\n@body:\n  color: \"#94a3b8\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-panel-**-----------------------------------------------------------------+\n| heading..**............................................................. |\n| body..**................................................................ |\n| ........................................................................ |\n# #....................................................................... |\n# #....................................................................... |\n+--------------------------------------------------------------------------+\n```\n",
            "Animated Heron-style thinking and status message.",
            List.of(
            new GeneratedElementMetadata("heading", "Animated thinking or activity phase.", true),
            new GeneratedElementMetadata("body", "Visible simulated reasoning detail.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        heading = addElement(new Label("heading", "Thinking"));
        heading.setStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#e5e7eb"), makeTextGradient(90, 1, 1, "#000000aa", "#00000077", "#00000044", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000044", "#00000077", "#000000aa"), null));
        heading.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        body = addElement(new SpanLabel("body", ""));
        body.setStyle(Style.fromProperties("color", "#94a3b8", "user-select", "text"));
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
                .setType("label")
                .setCellName("panel")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 74, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(72, 1))
                .setCellCharsSize(new Size(74, 5))
                .setMarginRight(1)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#00000000", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#e5e7eb"), makeTextGradient(90, 1, 1, "#000000aa", "#00000077", "#00000044", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000044", "#00000077", "#000000aa"), null)),
            new GeneratedLayoutEntry()
                .setName("body")
                .setType("spanlabel")
                .setCellName("panel")
                .setRelative(new Rect(1, 1, 536870911, 536870910))
                .setSourceCell(new Rect(0, 0, 74, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(72, 4))
                .setCellCharsSize(new Size(74, 5))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#00000000", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("color", "#94a3b8", "user-select", "text"))
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
