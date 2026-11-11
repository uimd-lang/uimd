// Auto-generated UI code for user_entry - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Rect;
import uimd.Size;
import uimd.SpanLabel;
import uimd.Style;
import uimd.TextGradient;

public class UserEntryUI extends GeneratedWindowBase
{
    public SpanLabel body;

    public UserEntryUI()
    {
        super("Agent Chat User Entry");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "UserEntry",
            "# Agent Chat User Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style user message with a separate accent.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable user prompt.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@body:\n  color: \"#e0f2fe\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-panel-**-----------------------------------------------------------------+\n| body..**................................................................ |\n| ........................................................................ |\n# #....................................................................... |\n# #....................................................................... |\n+--------------------------------------------------------------------------+\n```\n",
            "Heron-style user message with a separate accent.",
            List.of(
            new GeneratedElementMetadata("body", "Selectable user prompt.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        body = addElement(new SpanLabel("body", ""));
        body.setStyle(Style.fromProperties("color", "#e0f2fe", "user-select", "text"));
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
                .setName("body")
                .setType("spanlabel")
                .setCellName("panel")
                .setRelative(new Rect(0, 1, 536870911, 536870910))
                .setSourceCell(new Rect(0, 0, 74, 4))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(72, 4))
                .setCellCharsSize(new Size(74, 4))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("color", "#e0f2fe", "user-select", "text"))
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
