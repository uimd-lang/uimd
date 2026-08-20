// Auto-generated UI code for image_list_scroll - DO NOT EDIT MANUALLY.
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

public class ImageListScrollUI extends GeneratedScrollViewBase
{

    public ImageListScrollUI()
    {
        super("Image List Scroll");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ImageListScroll",
            "# Image List Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\ndescription: \"Scrollable image item list.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - image_list_item\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n  focus-background: \"#1e3a5f\"\n@panel:\n  gap: 1\n  padding: 1, 1, 1, 1\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------------------------------------------------+\n|                                                                         |\n|                                                                         |\n|                                                                         |\n|                                                                         |\n|                                                                         |\n*                                                                         |\n*                                                                         |\n*                                                                         |\n*                                                                         |\n|                                                                         |\n+-------------------------------------------------------------------------+\n```\n",
            "Scrollable image item list.",
            List.of());
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        setGeneratedScrollView(addElement(new ScrollView("__scrollview", 1)));
        scrollView().setStyle(Style.fromProperties("background", "#030712", "gap", "1", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"));
        scrollView().setFocusStyle(Style.fromProperties("background", "#1e3a5f"));
        scrollView().setEditStyle(Style.fromProperties("background", "#ffffff18"));
        scrollView().setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        scrollView().setDescendantFocusStyle(Style.fromProperties("background", "#1e3a5f"));
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
                .setRelative(new Rect(0, 0, 536870911, 536870911))
                .setSourceCell(new Rect(0, 0, 73, 10))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(73, 10))
                .setCellCharsSize(new Size(73, 10))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712", "gap", "1"))
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
