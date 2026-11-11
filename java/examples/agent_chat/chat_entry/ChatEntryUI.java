// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
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

public class ChatEntryUI extends GeneratedWindowBase
{
    public SpanLabel heading;
    public SpanLabel body;

    public ChatEntryUI()
    {
        super("Agent Chat Entry");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ChatEntry",
            "# Agent Chat Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style assistant text entry.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Assistant\"\n  description: \"Assistant role label.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable Markdown-style transcript prose.\"\n\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@body_panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#a7f3d0\"\n  user-select: text\n@body:\n  color: \"#cbd5e1\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-body_panel-**-----------------------------------------------------------+\n| heading..**............................................................ |\n| body..**............................................................... |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n",
            "Heron-style assistant text entry.",
            List.of(
            new GeneratedElementMetadata("heading", "Assistant role label.", true),
            new GeneratedElementMetadata("body", "Selectable Markdown-style transcript prose.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        heading = addElement(new SpanLabel("heading", "Assistant"));
        heading.setStyle(Style.fromProperties("color", "#a7f3d0", "user-select", "text"));
        body = addElement(new SpanLabel("body", ""));
        body.setStyle(Style.fromProperties("color", "#cbd5e1", "user-select", "text"));
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
                .setCellName("body_panel")
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
                .setCellStyle(Style.fromProperties("background", "#00000000", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("color", "#a7f3d0", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("body")
                .setType("spanlabel")
                .setCellName("body_panel")
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
                .setCellStyle(Style.fromProperties("background", "#00000000", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("color", "#cbd5e1", "user-select", "text"))
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
