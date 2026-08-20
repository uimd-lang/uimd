// Auto-generated UI code for image_button - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Image;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class ImageButtonUI extends GeneratedWindowBase
{
    public Image photo;

    public ImageButtonUI()
    {
        super("Image Button");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(true);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "ImageButton",
            "# Image Button\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Single-image clickable tile with padding around a centered square image.\"\ntags: [example, image, button]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\nphoto:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: cover\n  render_mode: auto\n  align: center\n  valign: middle\n  description: \"Tile image. Click to select.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 4, 1, 4\n  background: transparent\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------+\n| photo..**............. |\n| ...................... |\n| ...................... |\n| ...................... |\n+------------------------+\n```\n",
            "Single-image clickable tile with padding around a centered square image.",
            List.of(
            new GeneratedElementMetadata("photo", "Tile image. Click to select.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#172033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        photo = addElement(new Image("photo", "", "", "cover", "auto", "center", "middle"));
        photo.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        photo.setFocusStyle(Style.fromProperties("background", "#25364f"));
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
                .setName("photo")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(0, 1, 536870911, 4))
                .setSourceCell(new Rect(0, 0, 24, 4))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(4))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(22, 4))
                .setCellCharsSize(new Size(24, 4))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "transparent", "padding-bottom", "1", "padding-left", "4", "padding-right", "4", "padding-top", "1"))
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
