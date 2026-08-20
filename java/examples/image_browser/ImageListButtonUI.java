// Auto-generated UI code for image_list_button - DO NOT EDIT MANUALLY.
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

public class ImageListButtonUI extends GeneratedWindowBase
{
    public Image prw_1;
    public Image prw_2;
    public Image prw_3;

    public ImageListButtonUI()
    {
        super("Image List Button");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(true);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "ImageListButton",
            "# Image List Button\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Clickable image-list preview tile with three thumbnails.\"\ntags: [example, image, button, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\nprw_1:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"Preview image 1\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 1. Click to open image items.\"\n\nprw_2:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"Preview image 2\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 2. Click to open image items.\"\n\nprw_3:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"Preview image 3\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 3. Click to open image items.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 0, 1, 0\n  background: \"#172033\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------+\n| prw_1.. prw_2.. prw_3.. |\n| ....... ....... ....... |\n| ....... ....... ....... |\n+-------------------------+\n```\n",
            "Clickable image-list preview tile with three thumbnails.",
            List.of(
            new GeneratedElementMetadata("prw_1", "List preview slot 1. Click to open image items.", true),
            new GeneratedElementMetadata("prw_2", "List preview slot 2. Click to open image items.", true),
            new GeneratedElementMetadata("prw_3", "List preview slot 3. Click to open image items.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#172033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        prw_1 = addElement(new Image("prw_1", "shared/assets/image_samples/camera.png", "Preview image 1", "cover", "auto", "center", "middle"));
        prw_1.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        prw_1.setFocusStyle(Style.fromProperties("background", "#25364f"));
        prw_2 = addElement(new Image("prw_2", "shared/assets/image_samples/astronaut.png", "Preview image 2", "cover", "auto", "center", "middle"));
        prw_2.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        prw_2.setFocusStyle(Style.fromProperties("background", "#25364f"));
        prw_3 = addElement(new Image("prw_3", "shared/assets/image_samples/coffee.png", "Preview image 3", "cover", "auto", "center", "middle"));
        prw_3.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        prw_3.setFocusStyle(Style.fromProperties("background", "#25364f"));
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
                .setName("prw_1")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(0, 1, 7, 3))
                .setSourceCell(new Rect(0, 0, 25, 3))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(7, 3))
                .setCellCharsSize(new Size(25, 3))
                .setMarginRight(17)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "0", "padding-right", "0", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("prw_2")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(0, 9, 7, 3))
                .setSourceCell(new Rect(0, 0, 25, 3))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(7, 3))
                .setCellCharsSize(new Size(25, 3))
                .setMarginRight(9)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "0", "padding-right", "0", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("prw_3")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(0, 17, 7, 3))
                .setSourceCell(new Rect(0, 0, 25, 3))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(7, 3))
                .setCellCharsSize(new Size(25, 3))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "0", "padding-right", "0", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"))
        );
    }

    protected void onPrw1Click()
    {
    }

    protected void onPrw2Click()
    {
    }

    protected void onPrw3Click()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("prw_1".equals(name))
        {
            onPrw1Click();
            return true;
        }
        if ("prw_2".equals(name))
        {
            onPrw2Click();
            return true;
        }
        if ("prw_3".equals(name))
        {
            onPrw3Click();
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
