// Auto-generated UI code for paragraph - DO NOT EDIT MANUALLY.
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

public class ParagraphUI extends GeneratedWindowBase
{
    public Label text;

    public ParagraphUI()
    {
        super("Paragraph");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "Paragraph",
            "# Paragraph\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown paragraph block.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntext:\n  type: label\n  text: \"\"\n  description: \"Rendered paragraph text with mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@text:\n  color: \"#cbd5e1\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-body-**----------------------------------------+\n| text..**...................................... |\n| .............................................. |\n# #............................................ |\n+------------------------------------------------+\n```\n",
            "Reusable markdown paragraph block.",
            List.of(
            new GeneratedElementMetadata("text", "Rendered paragraph text with mouse selection enabled.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        text = addElement(new Label("text", ""));
        text.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1", "user-select", "text"));
        text.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setName("text")
                .setType("label")
                .setCellName("body")
                .setRelative(new Rect(0, 1, 536870911, 3))
                .setSourceCell(new Rect(0, 0, 48, 3))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(46, 3))
                .setCellCharsSize(new Size(48, 3))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1", "user-select", "text"))
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
