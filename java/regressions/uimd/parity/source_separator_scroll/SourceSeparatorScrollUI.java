// Auto-generated UI code for source_separator_scroll - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedScrollViewBase;
import uimd.GeneratedWindowBase;
import uimd.Rect;
import uimd.ScrollView;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class SourceSeparatorScrollUI extends GeneratedScrollViewBase
{

    public SourceSeparatorScrollUI()
    {
        super("Source Separator Scroll");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "SourceSeparatorScroll",
            "# Source Separator Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"ScrollView regression for reusable children whose source rows include design-only separators.\"\ntags: [regression, issue-3, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - source_separator_row\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@panel:\n  gap: 0\n  padding: 0\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----+\n|             |\n|             |\n|             |\n|             |\n+-------------+\n```\n",
            "ScrollView regression for reusable children whose source rows include design-only separators.",
            List.of());
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        setGeneratedScrollView(addElement(new ScrollView("__scrollview", 0)));
        scrollView().setStyle(Style.fromProperties("background", "#030712", "gap", "0", "padding", "0"));
        scrollView().setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        scrollView().setEditStyle(Style.fromProperties("background", "#ffffff18"));
        scrollView().setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
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
                .setName("__scrollview")
                .setType("uiscrollview")
                .setCellName("panel")
                .setRelative(new Rect(0, 0, 536870911, 4))
                .setSourceCell(new Rect(0, 0, 13, 4))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(13, 4))
                .setCellCharsSize(new Size(13, 4))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712", "gap", "0"))
                .setElementStyle(new Style())
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
