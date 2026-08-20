// Auto-generated UI code for image_gallery - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
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

public class ImageGalleryUI extends GeneratedWindowBase
{
    public Label header;
    public Button close_btn;
    public Label fallback_label;
    public Label sixel_label;
    public Label stretch_label;
    public Image camera_fb;
    public Image astro_fb;
    public Image coffee_fb;
    public Image coins_fb;
    public Image camera_ctn;
    public Image astro_ctn;
    public Image coffee_ctn;
    public Image coins_ctn;
    public Image camera_str;
    public Image astro_str;
    public Image coffee_str;
    public Image coins_str;
    public Label status;

    public ImageGalleryUI()
    {
        super("Image Gallery");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "ImageGallery",
            "# Image Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: \"Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.\"\ntags: [example, image]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Image Gallery\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the image gallery.\"\n\nfallback_label:\n  type: label\n  text: \"Fallback cover\"\n  description: \"Fallback row label.\"\n\nsixel_label:\n  type: label\n  text: \"Sixel contain\"\n  description: \"Sixel contain row label.\"\n\nstretch_label:\n  type: label\n  text: \"Sixel stretch\"\n  description: \"Sixel stretch row label.\"\n\ncamera_fb:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Camera image forced through fallback rendering.\"\n\nastro_fb:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Astronaut image forced through fallback rendering.\"\n\ncoffee_fb:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Coffee image forced through fallback rendering.\"\n\ncoins_fb:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Coins image forced through fallback rendering.\"\n\ncamera_ctn:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Camera image in forced Sixel contain mode.\"\n\nastro_ctn:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Astronaut image in forced Sixel contain mode.\"\n\ncoffee_ctn:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Coffee image in forced Sixel contain mode.\"\n\ncoins_ctn:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Coins image in forced Sixel contain mode.\"\n\ncamera_str:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Camera image in forced Sixel stretch mode.\"\n\nastro_str:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Astronaut image in forced Sixel stretch mode.\"\n\ncoffee_str:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Coffee image in forced Sixel stretch mode.\"\n\ncoins_str:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Coins image in forced Sixel stretch mode.\"\n\nstatus:\n  type: label\n  text: \"Rows: fallback cover, forced Sixel contain, forced Sixel stretch.\"\n  description: \"Current image validation status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@fallback_label:\n  color: \"#93c5fd\"\n@sixel_label:\n  color: \"#93c5fd\"\n@stretch_label:\n  color: \"#93c5fd\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n@status:\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------------------------+\n| header.................................................................. close_btn.. |\n|                                                                                      |\n| fallback_label...................................................................... |\n|  camera_fb........... astro_fb............ coffee_fb........... coins_fb............. |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| sixel_label......................................................................... |\n|  camera_ctn.......... astro_ctn........... coffee_ctn.......... coins_ctn........... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| stretch_label....................................................................... |\n|  camera_str.......... astro_str........... coffee_str.......... coins_str........... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| status.............................................................................. |\n+--------------------------------------------------------------------------------------+\n```\n",
            "Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.",
            List.of(
            new GeneratedElementMetadata("header", "Window title.", false),
            new GeneratedElementMetadata("close_btn", "Close the image gallery.", true),
            new GeneratedElementMetadata("fallback_label", "Fallback row label.", true),
            new GeneratedElementMetadata("sixel_label", "Sixel contain row label.", true),
            new GeneratedElementMetadata("stretch_label", "Sixel stretch row label.", true),
            new GeneratedElementMetadata("camera_fb", "Camera image forced through fallback rendering.", true),
            new GeneratedElementMetadata("astro_fb", "Astronaut image forced through fallback rendering.", true),
            new GeneratedElementMetadata("coffee_fb", "Coffee image forced through fallback rendering.", true),
            new GeneratedElementMetadata("coins_fb", "Coins image forced through fallback rendering.", true),
            new GeneratedElementMetadata("camera_ctn", "Camera image in forced Sixel contain mode.", true),
            new GeneratedElementMetadata("astro_ctn", "Astronaut image in forced Sixel contain mode.", true),
            new GeneratedElementMetadata("coffee_ctn", "Coffee image in forced Sixel contain mode.", true),
            new GeneratedElementMetadata("coins_ctn", "Coins image in forced Sixel contain mode.", true),
            new GeneratedElementMetadata("camera_str", "Camera image in forced Sixel stretch mode.", true),
            new GeneratedElementMetadata("astro_str", "Astronaut image in forced Sixel stretch mode.", true),
            new GeneratedElementMetadata("coffee_str", "Coffee image in forced Sixel stretch mode.", true),
            new GeneratedElementMetadata("coins_str", "Coins image in forced Sixel stretch mode.", true),
            new GeneratedElementMetadata("status", "Current image validation status.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header = addElement(new Label("header", "Image Gallery"));
        header.setStyle(Style.fromProperties("background", "#1f2937", "color", "#ffffff", "text-align", "center"));
        header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        close_btn = addElement(new Button("close_btn", "Quit"));
        close_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#ffffff"));
        close_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        close_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        fallback_label = addElement(new Label("fallback_label", "Fallback cover"));
        fallback_label.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        fallback_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        sixel_label = addElement(new Label("sixel_label", "Sixel contain"));
        sixel_label.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        sixel_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        stretch_label = addElement(new Label("stretch_label", "Sixel stretch"));
        stretch_label.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        stretch_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        camera_fb = addElement(new Image("camera_fb", "shared/assets/image_samples/camera.png", "camera fallback", "cover", "fallback", "center", "middle"));
        camera_fb.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        astro_fb = addElement(new Image("astro_fb", "shared/assets/image_samples/astronaut.png", "astronaut fallback", "cover", "fallback", "center", "middle"));
        astro_fb.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        coffee_fb = addElement(new Image("coffee_fb", "shared/assets/image_samples/coffee.png", "coffee fallback", "cover", "fallback", "center", "middle"));
        coffee_fb.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        coins_fb = addElement(new Image("coins_fb", "shared/assets/image_samples/coins.png", "coins fallback", "cover", "fallback", "center", "middle"));
        coins_fb.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        camera_ctn = addElement(new Image("camera_ctn", "shared/assets/image_samples/camera.png", "camera sixel", "contain", "sixel", "center", "middle"));
        camera_ctn.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        astro_ctn = addElement(new Image("astro_ctn", "shared/assets/image_samples/astronaut.png", "astronaut sixel", "contain", "sixel", "center", "middle"));
        astro_ctn.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        coffee_ctn = addElement(new Image("coffee_ctn", "shared/assets/image_samples/coffee.png", "coffee sixel", "contain", "sixel", "center", "middle"));
        coffee_ctn.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        coins_ctn = addElement(new Image("coins_ctn", "shared/assets/image_samples/coins.png", "coins sixel", "contain", "sixel", "center", "middle"));
        coins_ctn.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        camera_str = addElement(new Image("camera_str", "shared/assets/image_samples/camera.png", "camera stretch", "stretch", "sixel", "center", "middle"));
        camera_str.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        astro_str = addElement(new Image("astro_str", "shared/assets/image_samples/astronaut.png", "astronaut stretch", "stretch", "sixel", "center", "middle"));
        astro_str.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        coffee_str = addElement(new Image("coffee_str", "shared/assets/image_samples/coffee.png", "coffee stretch", "stretch", "sixel", "center", "middle"));
        coffee_str.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        coins_str = addElement(new Image("coins_str", "shared/assets/image_samples/coins.png", "coins stretch", "stretch", "sixel", "center", "middle"));
        coins_str.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        status = addElement(new Label("status", "Rows: fallback cover, forced Sixel contain, forced Sixel stretch."));
        status.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        status.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setName("header")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 1, 72, 1))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(72))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(72, 1))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(13)
                .setMarginBottom(38)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#1f2937", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("close_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(0, 74, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(1)
                .setMarginBottom(38)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#ffffff")),
            new GeneratedLayoutEntry()
                .setName("fallback_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(2, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(84, 1))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(1)
                .setMarginBottom(36)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("camera_fb")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(3, 2, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(64)
                .setMarginBottom(26)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("astro_fb")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(3, 23, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(43)
                .setMarginBottom(26)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("coffee_fb")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(3, 44, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(22)
                .setMarginBottom(26)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("coins_fb")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(3, 65, 536870911, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(21, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(0)
                .setMarginBottom(26)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("sixel_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(14, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(84, 1))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(1)
                .setMarginBottom(24)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("camera_ctn")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(15, 2, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(64)
                .setMarginBottom(14)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("astro_ctn")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(15, 23, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(43)
                .setMarginBottom(14)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("coffee_ctn")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(15, 44, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(22)
                .setMarginBottom(14)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("coins_ctn")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(15, 65, 536870911, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(1)
                .setMarginBottom(14)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("stretch_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(26, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(84, 1))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(1)
                .setMarginBottom(12)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("camera_str")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(27, 2, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(64)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("astro_str")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(27, 23, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(43)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("coffee_str")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(27, 44, 20, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(22)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("coins_str")
                .setType("image")
                .setCellName("")
                .setRelative(new Rect(27, 65, 536870911, 10))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(10))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(20, 10))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(1)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("status")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(38, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 86, 39))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(86))
                .setCellHeight(AxisDimension.auto(39))
                .setCharsSize(new Size(84, 1))
                .setCellCharsSize(new Size(86, 39))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"))
        );
    }

    protected void onCloseBtnClick()
    {
    }

    protected void onCameraFbClick()
    {
    }

    protected void onAstroFbClick()
    {
    }

    protected void onCoffeeFbClick()
    {
    }

    protected void onCoinsFbClick()
    {
    }

    protected void onCameraCtnClick()
    {
    }

    protected void onAstroCtnClick()
    {
    }

    protected void onCoffeeCtnClick()
    {
    }

    protected void onCoinsCtnClick()
    {
    }

    protected void onCameraStrClick()
    {
    }

    protected void onAstroStrClick()
    {
    }

    protected void onCoffeeStrClick()
    {
    }

    protected void onCoinsStrClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("close_btn".equals(name))
        {
            onCloseBtnClick();
            return true;
        }
        if ("camera_fb".equals(name))
        {
            onCameraFbClick();
            return true;
        }
        if ("astro_fb".equals(name))
        {
            onAstroFbClick();
            return true;
        }
        if ("coffee_fb".equals(name))
        {
            onCoffeeFbClick();
            return true;
        }
        if ("coins_fb".equals(name))
        {
            onCoinsFbClick();
            return true;
        }
        if ("camera_ctn".equals(name))
        {
            onCameraCtnClick();
            return true;
        }
        if ("astro_ctn".equals(name))
        {
            onAstroCtnClick();
            return true;
        }
        if ("coffee_ctn".equals(name))
        {
            onCoffeeCtnClick();
            return true;
        }
        if ("coins_ctn".equals(name))
        {
            onCoinsCtnClick();
            return true;
        }
        if ("camera_str".equals(name))
        {
            onCameraStrClick();
            return true;
        }
        if ("astro_str".equals(name))
        {
            onAstroStrClick();
            return true;
        }
        if ("coffee_str".equals(name))
        {
            onCoffeeStrClick();
            return true;
        }
        if ("coins_str".equals(name))
        {
            onCoinsStrClick();
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
