// Auto-generated UI code for image_browser - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.ComboBox;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;
import uimd.ViewHost;

public class ImageBrowserUI extends GeneratedWindowBase
{
    public Label title;
    public Button close_btn;
    public Label render_label;
    public ComboBox mode_select;
    public Label sidebar_title;
    public ReusableElement camera_thumb;
    public ReusableElement astro_thumb;
    public ReusableElement coffee_thumb;
    public Label gallery_label;
    public Label image_list_label;
    public ReusableElement gallery_mosaic;
    public ReusableElement image_list_btn;
    public Label spacer;
    public ViewHost main;

    public ImageBrowserUI()
    {
        super("Image Browser");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "ImageBrowser",
            "# Image Browser\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: \"Image browser with a thumbnail sidebar and full-size image panel.\"\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_view\n  - gallery_view\n  - gallery_mosaic\n  - image_button\n  - image_list_button\n  - image_list_view\n  - image_show_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Image Browser\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the image browser.\"\n\nrender_label:\n  type: label\n  text: \"Render\"\n  description: \"Label above the render mode selector.\"\n  expose: false\n\nmode_select:\n  type: combobox\n  options: [Normal, Fallback]\n  selected_item: Normal\n  description: \"Switch between Sixel and fallback image rendering.\"\n\nsidebar_title:\n  type: label\n  text: \"Photos\"\n  description: \"Sidebar section label.\"\n  expose: false\n\ncamera_thumb:\n  type: image_button\n  description: \"Camera thumbnail. Click to view full size.\"\n\nastro_thumb:\n  type: image_button\n  description: \"Astronaut thumbnail. Click to view full size.\"\n\ncoffee_thumb:\n  type: image_button\n  description: \"Coffee thumbnail. Click to view full size.\"\n\ngallery_label:\n  type: label\n  text: \"Gallery\"\n  description: \"Label above the gallery mosaic.\"\n  expose: false\n\nimage_list_label:\n  type: label\n  text: \"Image items\"\n  description: \"List preview label.\"\n  expose: false\n\ngallery_mosaic:\n  type: gallery_mosaic\n  description: \"Click any image to open the full scrollable gallery.\"\n\nimage_list_btn:\n  type: image_list_button\n  description: \"Click to open the image item list.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Spacer that absorbs leftover sidebar height.\"\n  expose: false\n\nmain:\n  type: viewhost\n  description: \"Full-size image panel.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: \"#1f2937\"\n  padding: 0\n@sidebar:\n  background: \"#172033\"\n  padding: 0\n@content:\n  background: \"#030712\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@render_label:\n  color: \"#93c5fd\"\n@sidebar_title:\n  color: \"#93c5fd\"\n@gallery_label:\n  color: \"#93c5fd\"\n@image_list_label:\n  color: \"#93c5fd\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+--header-**-------------------------------------------------------------------+\n| title..**....................................................... close_btn.. |\n+--sidebar-------------------+--content-**-------------------------------------+\n| render_label.............. | main..**....................................... |\n* mode_select............... | ............................................... |\n*                            | *.............................................. |\n| sidebar_title............. | *.............................................. |\n| camera_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| astro_thumb............... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| coffee_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| gallery_label............. | ............................................... |\n| gallery_mosaic............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| image_list_label.......... | ............................................... |\n| image_list_btn............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n* spacer.................... | ............................................... |\n* .......................... | ............................................... |\n| .......................... | ............................................... |\n+----------------------------+-------------------------------------------------+\n```\n",
            "Image browser with a thumbnail sidebar and full-size image panel.",
            List.of(
            new GeneratedElementMetadata("title", "Window title.", false),
            new GeneratedElementMetadata("close_btn", "Close the image browser.", true),
            new GeneratedElementMetadata("render_label", "Label above the render mode selector.", false),
            new GeneratedElementMetadata("mode_select", "Switch between Sixel and fallback image rendering.", true),
            new GeneratedElementMetadata("sidebar_title", "Sidebar section label.", false),
            new GeneratedElementMetadata("camera_thumb", "Camera thumbnail. Click to view full size.", true),
            new GeneratedElementMetadata("astro_thumb", "Astronaut thumbnail. Click to view full size.", true),
            new GeneratedElementMetadata("coffee_thumb", "Coffee thumbnail. Click to view full size.", true),
            new GeneratedElementMetadata("gallery_label", "Label above the gallery mosaic.", false),
            new GeneratedElementMetadata("image_list_label", "List preview label.", false),
            new GeneratedElementMetadata("gallery_mosaic", "Click any image to open the full scrollable gallery.", true),
            new GeneratedElementMetadata("image_list_btn", "Click to open the image item list.", true),
            new GeneratedElementMetadata("spacer", "Spacer that absorbs leftover sidebar height.", false),
            new GeneratedElementMetadata("main", "Full-size image panel.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        title = addElement(new Label("title", "Image Browser"));
        title.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center"));
        title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        close_btn = addElement(new Button("close_btn", "Quit"));
        close_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#ffffff"));
        close_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        close_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        render_label = addElement(new Label("render_label", "Render"));
        render_label.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        render_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        mode_select = addElement(new ComboBox("mode_select", List.of("Normal", "Fallback")));
        mode_select.setSelectedIndex(0);
        mode_select.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        mode_select.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        mode_select.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        mode_select.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        mode_select.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        sidebar_title = addElement(new Label("sidebar_title", "Photos"));
        sidebar_title.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        sidebar_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        camera_thumb = addElement(new ReusableElement("camera_thumb", "image_button"));
        camera_thumb.setChild(new ImageButtonUI());
        camera_thumb.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        camera_thumb.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        camera_thumb.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        astro_thumb = addElement(new ReusableElement("astro_thumb", "image_button"));
        astro_thumb.setChild(new ImageButtonUI());
        astro_thumb.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        astro_thumb.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        astro_thumb.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        coffee_thumb = addElement(new ReusableElement("coffee_thumb", "image_button"));
        coffee_thumb.setChild(new ImageButtonUI());
        coffee_thumb.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        coffee_thumb.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        coffee_thumb.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        gallery_label = addElement(new Label("gallery_label", "Gallery"));
        gallery_label.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        gallery_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        image_list_label = addElement(new Label("image_list_label", "Image items"));
        image_list_label.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        image_list_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        gallery_mosaic = addElement(new ReusableElement("gallery_mosaic", "gallery_mosaic"));
        gallery_mosaic.setChild(new GalleryMosaicUI());
        gallery_mosaic.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        gallery_mosaic.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        gallery_mosaic.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        image_list_btn = addElement(new ReusableElement("image_list_btn", "image_list_button"));
        image_list_btn.setChild(new ImageListButtonUI());
        image_list_btn.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        image_list_btn.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        image_list_btn.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        spacer = addElement(new Label("spacer", ""));
        spacer.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        spacer.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        main = addElement(new ViewHost("main"));
        main.setStyle(Style.fromProperties("background", "#030712"));
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
                .setName("main")
                .setType("viewhost")
                .setCellName("content")
                .setRelative(new Rect(0, 1, 536870911, 536870911))
                .setSourceCell(new Rect(2, 29, 49, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(41))
                .setCharsSize(new Size(47, 41))
                .setCellCharsSize(new Size(49, 41))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "#030712")),
            new GeneratedLayoutEntry()
                .setName("render_label")
                .setType("label")
                .setCellName("sidebar")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(40)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("title")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 78, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(64, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(13)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("close_btn")
                .setType("button")
                .setCellName("header")
                .setRelative(new Rect(0, 66, 11, 1))
                .setSourceCell(new Rect(0, 0, 78, 1))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#ffffff")),
            new GeneratedLayoutEntry()
                .setName("mode_select")
                .setType("combobox")
                .setCellName("sidebar")
                .setRelative(new Rect(1, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(39)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("sidebar_title")
                .setType("label")
                .setCellName("sidebar")
                .setRelative(new Rect(3, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(37)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("camera_thumb")
                .setType("image_button")
                .setCellName("sidebar")
                .setRelative(new Rect(4, 1, 536870911, 6))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(6))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 6))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(31)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("astro_thumb")
                .setType("image_button")
                .setCellName("sidebar")
                .setRelative(new Rect(10, 1, 536870911, 6))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(6))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 6))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(25)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("coffee_thumb")
                .setType("image_button")
                .setCellName("sidebar")
                .setRelative(new Rect(16, 1, 536870911, 6))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(6))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 6))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(19)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("gallery_label")
                .setType("label")
                .setCellName("sidebar")
                .setRelative(new Rect(22, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(18)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("gallery_mosaic")
                .setType("gallery_mosaic")
                .setCellName("sidebar")
                .setRelative(new Rect(23, 1, 536870911, 9))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(9))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 9))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(9)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("image_list_label")
                .setType("label")
                .setCellName("sidebar")
                .setRelative(new Rect(32, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(8)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("image_list_btn")
                .setType("image_list_button")
                .setCellName("sidebar")
                .setRelative(new Rect(33, 1, 536870911, 5))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(5))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 5))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(3)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("spacer")
                .setType("label")
                .setCellName("sidebar")
                .setRelative(new Rect(38, 1, 536870911, 3))
                .setSourceCell(new Rect(2, 0, 28, 41))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(26, 3))
                .setCellCharsSize(new Size(28, 41))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"))
        );
    }

    protected void onCloseBtnClick()
    {
    }

    protected void onModeSelectChange(String value)
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
        if ("mode_select".equals(name))
        {
            onModeSelectChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}
