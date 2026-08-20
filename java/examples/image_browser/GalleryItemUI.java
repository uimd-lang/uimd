// Auto-generated UI code for gallery_item - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Image;
import uimd.Label;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class GalleryItemUI extends GeneratedWindowBase
{
    public Label caption;
    public Image photo;

    public GalleryItemUI()
    {
        super("Gallery Item");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "GalleryItem",
            "# Gallery Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Full-size image item for the gallery scroll view.\"\ntags: [example, image, gallery]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ncaption:\n  type: label\n  text: \"\"\n  description: \"Image title.\"\n\nphoto:\n  type: image\n  source: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Full-size image.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@caption:\n  color: \"#bfdbfe\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n```\n\n## User Interface\n\n```ui\n+-view-**--------------------------+\n| caption..**..................... |\n| photo..**....................... |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n| ................................ |\n+----------------------------------+\n```\n",
            "Full-size image item for the gallery scroll view.",
            List.of(
            new GeneratedElementMetadata("caption", "Image title.", true),
            new GeneratedElementMetadata("photo", "Full-size image.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        caption = addElement(new Label("caption", ""));
        caption.setStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe", "text-align", "center"));
        caption.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        photo = addElement(new Image("photo", "", "", "contain", "auto", "center", "middle"));
        photo.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
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
                .setName("caption")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 34, 21))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(21))
                .setCharsSize(new Size(32, 1))
                .setCellCharsSize(new Size(34, 21))
                .setMarginRight(1)
                .setMarginBottom(20)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("photo")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(1, 1, 536870911, 20))
                .setSourceCell(new Rect(0, 0, 34, 21))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(20))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(21))
                .setCharsSize(new Size(32, 20))
                .setCellCharsSize(new Size(34, 21))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"))
        );
    }

    protected void onPhotoClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("photo".equals(name))
        {
            onPhotoClick();
            return true;
        }
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
