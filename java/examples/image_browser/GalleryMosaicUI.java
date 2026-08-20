// Auto-generated UI code for gallery_mosaic - DO NOT EDIT MANUALLY.
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

public class GalleryMosaicUI extends GeneratedWindowBase
{
    public Image gal_1;
    public Image gal_2;
    public Image gal_3;
    public Image gal_4;
    public Image gal_5;

    public GalleryMosaicUI()
    {
        super("Gallery Mosaic");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(true);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "GalleryMosaic",
            "# Gallery Mosaic\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"3+2 image mosaic that opens the scrollable gallery when clicked.\"\ntags: [example, image, gallery]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\ngal_1:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"Camera\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 1. Click to open gallery.\"\n\ngal_2:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"Astronaut\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 2. Click to open gallery.\"\n\ngal_3:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"Coffee\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 3. Click to open gallery.\"\n\ngal_4:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"Coins\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 4. Click to open gallery.\"\n\ngal_5:\n  type: image\n  source: \"shared/assets/image_samples/chelsea.png\"\n  alt: \"Chelsea\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 5. Click to open gallery.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 0, 1, 0\n  background: \"#172033\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**------------------+\n| gal_1.. gal_2.. gal_3... |\n| ....... ....... ........ |\n| ....... ....... ........ |\n|                          |\n| gal_4.. gal_5..          |\n| ....... .......          |\n| ....... .......          |\n+--------------------------+\n```\n",
            "3+2 image mosaic that opens the scrollable gallery when clicked.",
            List.of(
            new GeneratedElementMetadata("gal_1", "Gallery mosaic slot 1. Click to open gallery.", true),
            new GeneratedElementMetadata("gal_2", "Gallery mosaic slot 2. Click to open gallery.", true),
            new GeneratedElementMetadata("gal_3", "Gallery mosaic slot 3. Click to open gallery.", true),
            new GeneratedElementMetadata("gal_4", "Gallery mosaic slot 4. Click to open gallery.", true),
            new GeneratedElementMetadata("gal_5", "Gallery mosaic slot 5. Click to open gallery.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#172033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        gal_1 = addElement(new Image("gal_1", "shared/assets/image_samples/camera.png", "Camera", "cover", "auto", "center", "middle"));
        gal_1.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        gal_1.setFocusStyle(Style.fromProperties("background", "#25364f"));
        gal_2 = addElement(new Image("gal_2", "shared/assets/image_samples/astronaut.png", "Astronaut", "cover", "auto", "center", "middle"));
        gal_2.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        gal_2.setFocusStyle(Style.fromProperties("background", "#25364f"));
        gal_3 = addElement(new Image("gal_3", "shared/assets/image_samples/coffee.png", "Coffee", "cover", "auto", "center", "middle"));
        gal_3.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        gal_3.setFocusStyle(Style.fromProperties("background", "#25364f"));
        gal_4 = addElement(new Image("gal_4", "shared/assets/image_samples/coins.png", "Coins", "cover", "auto", "center", "middle"));
        gal_4.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        gal_4.setFocusStyle(Style.fromProperties("background", "#25364f"));
        gal_5 = addElement(new Image("gal_5", "shared/assets/image_samples/chelsea.png", "Chelsea", "cover", "auto", "center", "middle"));
        gal_5.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        gal_5.setFocusStyle(Style.fromProperties("background", "#25364f"));
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
                .setName("gal_1")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(0, 1, 7, 3))
                .setSourceCell(new Rect(0, 0, 26, 7))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(7, 3))
                .setCellCharsSize(new Size(26, 7))
                .setMarginRight(18)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "0", "padding-right", "0", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("gal_2")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(0, 9, 7, 3))
                .setSourceCell(new Rect(0, 0, 26, 7))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(7, 3))
                .setCellCharsSize(new Size(26, 7))
                .setMarginRight(10)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "0", "padding-right", "0", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("gal_3")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(0, 17, 8, 3))
                .setSourceCell(new Rect(0, 0, 26, 7))
                .setWidth(AxisDimension.auto(8))
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(8, 3))
                .setCellCharsSize(new Size(26, 7))
                .setMarginRight(1)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "0", "padding-right", "0", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("gal_4")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(4, 1, 7, 3))
                .setSourceCell(new Rect(0, 0, 26, 7))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(7, 3))
                .setCellCharsSize(new Size(26, 7))
                .setMarginRight(18)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "0", "padding-right", "0", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("gal_5")
                .setType("image")
                .setCellName("view")
                .setRelative(new Rect(4, 9, 7, 3))
                .setSourceCell(new Rect(0, 0, 26, 7))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(7, 3))
                .setCellCharsSize(new Size(26, 7))
                .setMarginRight(10)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "0", "padding-right", "0", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"))
        );
    }

    protected void onGal1Click()
    {
    }

    protected void onGal2Click()
    {
    }

    protected void onGal3Click()
    {
    }

    protected void onGal4Click()
    {
    }

    protected void onGal5Click()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("gal_1".equals(name))
        {
            onGal1Click();
            return true;
        }
        if ("gal_2".equals(name))
        {
            onGal2Click();
            return true;
        }
        if ("gal_3".equals(name))
        {
            onGal3Click();
            return true;
        }
        if ("gal_4".equals(name))
        {
            onGal4Click();
            return true;
        }
        if ("gal_5".equals(name))
        {
            onGal5Click();
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
