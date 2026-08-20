// Auto-generated UI code for gallery_view - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class GalleryViewUI extends GeneratedWindowBase
{
    public ReusableElement gallery_scroll;

    public GalleryViewUI()
    {
        super("Gallery View");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "GalleryView",
            "# Gallery View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Gallery view hosting a scrollable list of full-size images.\"\ntags: [example, image, gallery, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - gallery_scroll\n```\n\n## Members\n\n```yaml\ngallery_scroll:\n  type: gallery_scroll\n  description: \"Scrollable image list.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-view-**-------------------+\n| gallery_scroll..**....... |\n| ......................... |\n| *........................ |\n| *........................ |\n| ......................... |\n| ......................... |\n+---------------------------+\n```\n",
            "Gallery view hosting a scrollable list of full-size images.",
            List.of(
            new GeneratedElementMetadata("gallery_scroll", "Scrollable image list.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        gallery_scroll = addElement(new ReusableElement("gallery_scroll", "gallery_scroll"));
        gallery_scroll.setChild(new GalleryScrollUI());
        gallery_scroll.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        gallery_scroll.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        gallery_scroll.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
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
                .setName("gallery_scroll")
                .setType("gallery_scroll")
                .setCellName("view")
                .setRelative(new Rect(0, 1, 536870911, 536870911))
                .setSourceCell(new Rect(0, 0, 27, 6))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(6))
                .setCharsSize(new Size(25, 6))
                .setCellCharsSize(new Size(27, 6))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
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
