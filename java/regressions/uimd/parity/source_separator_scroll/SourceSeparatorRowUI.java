// Auto-generated UI code for source_separator_row - DO NOT EDIT MANUALLY.
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

public class SourceSeparatorRowUI extends GeneratedWindowBase
{
    public Label sep;
    public Label left;

    public SourceSeparatorRowUI()
    {
        super("Source Separator Row");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "SourceSeparatorRow",
            "# Source Separator Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Regression row with a design-only ASCII separator between content rows.\"\ntags: [regression, issue-3, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nsep:\n  type: label\n  text: Header\n\nleft:\n  type: label\n  text: Left\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#15171c\"\ncell:\n  background: \"#15171c\"\nlabel:\n  color: \"#e5e7eb\"\n```\n\n## User Interface\n\n```ui\n+-----------+\n|sep........|\n+-----------+\n|left.......|\n+-----------+\n```\n",
            "Regression row with a design-only ASCII separator between content rows.",
            List.of(
            new GeneratedElementMetadata("sep", "Header", true),
            new GeneratedElementMetadata("left", "Left", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#15171c", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        sep = addElement(new Label("sep", "Header"));
        sep.setStyle(Style.fromProperties("background", "transparent", "color", "#e5e7eb"));
        sep.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        left = addElement(new Label("left", "Left"));
        left.setStyle(Style.fromProperties("background", "transparent", "color", "#e5e7eb"));
        left.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setName("left")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 11, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(11))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(11, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#15171c"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("sep")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 11, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(11))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(11, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#15171c"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#e5e7eb"))
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
