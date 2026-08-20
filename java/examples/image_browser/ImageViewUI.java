// Auto-generated UI code for image_view - DO NOT EDIT MANUALLY.
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

public class ImageViewUI extends GeneratedWindowBase
{
    public Label caption;
    public Image photo;

    public ImageViewUI()
    {
        super("Image View");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "ImageView",
            "# Image View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Full-size image display panel hosted inside a ViewHost.\"\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ncaption:\n  type: label\n  text: \"Select an image\"\n  description: \"Name of the currently displayed image.\"\n\nphoto:\n  type: image\n  source: \"\"\n  alt: \"photo\"\n  fit: contain\n  render_mode: auto\n  description: \"Full-size image display.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@caption:\n  color: \"#bfdbfe\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------------+\n| caption..**.................. |\n| photo..**.................... |\n| ............................. |\n| *............................ |\n| *............................ |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n+-------------------------------+\n```\n",
            "Full-size image display panel hosted inside a ViewHost.",
            List.of(
            new GeneratedElementMetadata("caption", "Name of the currently displayed image.", true),
            new GeneratedElementMetadata("photo", "Full-size image display.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        caption = addElement(new Label("caption", "Select an image"));
        caption.setStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe", "text-align", "center"));
        caption.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        photo = addElement(new Image("photo", "", "photo", "contain", "auto", "center", "middle"));
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
                .setSourceCell(new Rect(0, 0, 31, 12))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(12))
                .setCharsSize(new Size(29, 1))
                .setCellCharsSize(new Size(31, 12))
                .setMarginRight(1)
                .setMarginBottom(11)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("photo")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(1, 1, 536870911, 536870911))
                .setSourceCell(new Rect(0, 0, 31, 12))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(12))
                .setCharsSize(new Size(29, 11))
                .setCellCharsSize(new Size(31, 12))
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
