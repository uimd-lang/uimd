// Auto-generated UI code for activity_item - DO NOT EDIT MANUALLY.
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
import uimd.Style;
import uimd.TextGradient;

public class ActivityItemUI extends GeneratedWindowBase
{
    public Label timestamp;
    public Label event_type;
    public Label message;

    public ActivityItemUI()
    {
        super("Activity Item");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ActivityItem",
            "# Activity Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable activity feed item with timestamp, event type, and message text.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntimestamp:\n  type: label\n  text: \"10:42\"\n  description: \"Optional activity timestamp.\"\n\nevent_type:\n  type: label\n  text: Info\n  description: \"Activity category label.\"\n\nmessage:\n  type: label\n  text: \"Activity message\"\n  description: \"Selectable activity message body.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@marker:\n  background-texture: \"▐\"\n  background-texture-color: \"#38bdf8\"\n@marker_info:\n  background-texture-color: \"#38bdf8\"\n@marker_task:\n  background-texture-color: \"#a78bfa\"\n@marker_warning:\n  background-texture-color: \"#facc15\"\n@marker_deploy:\n  background-texture-color: \"#22c55e\"\n@marker_note:\n  background-texture-color: \"#fb7185\"\n@body:\n  background: \"#1f2e46\"\n@timestamp:\n  color: \"#94a3b8\"\n  user-select: text\n@event_type:\n  color: \"#bfdbfe\"\n  user-select: text\n@message:\n  color: \"#ffffff\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-marker-1-+-body-**----------------------------------+\n|          | timestamp.. event_type..**               |\n|          | message..**                              |\n|          | ........................................ |\n|          | ........................................ |\n#          # #                                        |\n#          # #                                        |\n+----------+------------------------------------------+\n```\n",
            "Reusable activity feed item with timestamp, event type, and message text.",
            List.of(
            new GeneratedElementMetadata("timestamp", "Optional activity timestamp.", true),
            new GeneratedElementMetadata("event_type", "Activity category label.", true),
            new GeneratedElementMetadata("message", "Selectable activity message body.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        timestamp = addElement(new Label("timestamp", "10:42"));
        timestamp.setStyle(Style.fromProperties("background", "transparent", "color", "#94a3b8", "user-select", "text"));
        timestamp.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        event_type = addElement(new Label("event_type", "Info"));
        event_type.setStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe", "user-select", "text"));
        event_type.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        message = addElement(new Label("message", "Activity message"));
        message.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "user-select", "text"));
        message.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setCellName("marker")
                .setRelative(new Rect(0, 0, 1, 536870910))
                .setSourceCell(new Rect(0, 0, 10, 6))
                .setWidth(AxisDimension.auto(1))
                .setHeight(AxisDimension.auto(536870910))
                .setCellWidth(AxisDimension.fixed(1))
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(10, 6))
                .setCellCharsSize(new Size(10, 6))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background-texture", "▐", "background-texture-color", "#38bdf8"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("timestamp")
                .setType("label")
                .setCellName("body")
                .setRelative(new Rect(0, 1, 11, 1))
                .setSourceCell(new Rect(0, 11, 42, 6))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(42, 6))
                .setMarginRight(30)
                .setMarginBottom(5)
                .setCellStyle(Style.fromProperties("background", "#1f2e46"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#94a3b8", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("event_type")
                .setType("label")
                .setCellName("body")
                .setRelative(new Rect(0, 13, 536870911, 1))
                .setSourceCell(new Rect(0, 11, 42, 6))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(42, 6))
                .setMarginRight(15)
                .setMarginBottom(5)
                .setCellStyle(Style.fromProperties("background", "#1f2e46"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("message")
                .setType("label")
                .setCellName("body")
                .setRelative(new Rect(1, 1, 536870911, 536870910))
                .setSourceCell(new Rect(0, 11, 42, 6))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(11, 5))
                .setCellCharsSize(new Size(42, 6))
                .setMarginRight(30)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2e46"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "user-select", "text"))
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
