// Auto-generated UI code for image_list_item - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.CheckBox;
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

public class ImageListItemUI extends GeneratedWindowBase
{
    public CheckBox sel;
    public Image thumb;
    public Label name_label;
    public Label path_label;
    public Label spacer;
    public Button show_btn;
    public Button browse_btn;
    public Button delete_btn;

    public ImageListItemUI()
    {
        super("Image List Item");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ImageListItem",
            "# Image List Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\ndescription: \"Reusable row for one image item.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nsel:\n  type: checkbox\n  title: \"\"\n  value: false\n  description: \"Select this image item.\"\n\nthumb:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Small image thumbnail.\"\n\nname_label:\n  type: label\n  text: \"\"\n  description: \"Image file name.\"\n\npath_label:\n  type: label\n  text: \"\"\n  description: \"Full image path.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible spacer before item actions.\"\n  expose: false\n\nshow_btn:\n  type: button\n  title: Show\n  description: \"Show this image in a dialog.\"\n\nbrowse_btn:\n  type: button\n  title: Browse\n  description: \"Choose a replacement image.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Delete this image item after confirmation.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 0, 0, 0, 0\n@name_label:\n  color: \"#f8fafc\"\n  user-select: text\n@path_label:\n  color: \"#64748b\"\n  user-select: text\ncheckbox:\n  color: \"#cbd5e1\"\n  focus-color: \"#facc15\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@show_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------------------------------+\n|                                                                       |\n|  sel thumb...  name_label..**..  show_btn... browse_btn. delete_btn.  |\n|  ... ........  .....................................................  |\n|  ... ........  path_label..**.......................................  |\n|  ... ........  .....................................................  |\n|  ... ........  spacer..**...........................................  |\n|                                                                       |\n+-----------------------------------------------------------------------+\n```\n",
            "Reusable row for one image item.",
            List.of(
            new GeneratedElementMetadata("sel", "Select this image item.", true),
            new GeneratedElementMetadata("thumb", "Small image thumbnail.", true),
            new GeneratedElementMetadata("name_label", "Image file name.", true),
            new GeneratedElementMetadata("path_label", "Full image path.", true),
            new GeneratedElementMetadata("spacer", "Flexible spacer before item actions.", false),
            new GeneratedElementMetadata("show_btn", "Show this image in a dialog.", true),
            new GeneratedElementMetadata("browse_btn", "Choose a replacement image.", true),
            new GeneratedElementMetadata("delete_btn", "Delete this image item after confirmation.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        sel = addElement(new CheckBox("sel", "", false));
        sel.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        sel.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#facc15"));
        sel.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#22c55e"));
        sel.setUncheckedStyle(Style.fromProperties("background", "transparent", "color", "#94a3b8"));
        sel.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        thumb = addElement(new Image("thumb", "", "", "contain", "auto", "center", "middle"));
        thumb.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        name_label = addElement(new Label("name_label", ""));
        name_label.setStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "user-select", "text"));
        name_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        path_label = addElement(new Label("path_label", ""));
        path_label.setStyle(Style.fromProperties("background", "transparent", "color", "#64748b", "user-select", "text"));
        path_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        spacer = addElement(new Label("spacer", ""));
        spacer.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        spacer.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        show_btn = addElement(new Button("show_btn", "Show"));
        show_btn.setStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"));
        show_btn.setFocusStyle(Style.fromProperties("background", "#16a34a", "color", "#ffffff"));
        show_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        browse_btn = addElement(new Button("browse_btn", "Browse"));
        browse_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        browse_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        browse_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        delete_btn = addElement(new Button("delete_btn", "Delete"));
        delete_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        delete_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        delete_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("sel")
                .setType("checkbox")
                .setCellName("card")
                .setRelative(new Rect(1, 2, 3, 5))
                .setSourceCell(new Rect(0, 0, 71, 7))
                .setWidth(AxisDimension.auto(3))
                .setHeight(AxisDimension.auto(5))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(3, 5))
                .setCellCharsSize(new Size(71, 7))
                .setMarginRight(66)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "0", "padding-left", "0", "padding-right", "0", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("thumb")
                .setType("image")
                .setCellName("card")
                .setRelative(new Rect(1, 6, 8, 5))
                .setSourceCell(new Rect(0, 0, 71, 7))
                .setWidth(AxisDimension.auto(8))
                .setHeight(AxisDimension.auto(5))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(8, 5))
                .setCellCharsSize(new Size(71, 7))
                .setMarginRight(57)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "0", "padding-left", "0", "padding-right", "0", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("name_label")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(1, 16, 536870911, 2))
                .setSourceCell(new Rect(0, 0, 71, 7))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(2))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(16, 2))
                .setCellCharsSize(new Size(71, 7))
                .setMarginRight(39)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "0", "padding-left", "0", "padding-right", "0", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("show_btn")
                .setType("button")
                .setCellName("card")
                .setRelative(new Rect(1, 34, 11, 5))
                .setSourceCell(new Rect(0, 0, 71, 7))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(5))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(11, 5))
                .setCellCharsSize(new Size(71, 7))
                .setMarginRight(26)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "0", "padding-left", "0", "padding-right", "0", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("browse_btn")
                .setType("button")
                .setCellName("card")
                .setRelative(new Rect(1, 46, 11, 5))
                .setSourceCell(new Rect(0, 0, 71, 7))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(5))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(11, 5))
                .setCellCharsSize(new Size(71, 7))
                .setMarginRight(14)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "0", "padding-left", "0", "padding-right", "0", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("delete_btn")
                .setType("button")
                .setCellName("card")
                .setRelative(new Rect(1, 58, 11, 5))
                .setSourceCell(new Rect(0, 0, 71, 7))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(5))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(11, 5))
                .setCellCharsSize(new Size(71, 7))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "0", "padding-left", "0", "padding-right", "0", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("path_label")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(3, 16, 536870911, 2))
                .setSourceCell(new Rect(0, 0, 71, 7))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(2))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(53, 2))
                .setCellCharsSize(new Size(71, 7))
                .setMarginRight(2)
                .setMarginBottom(2)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "0", "padding-left", "0", "padding-right", "0", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#64748b", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("spacer")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(5, 16, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 71, 7))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(53, 1))
                .setCellCharsSize(new Size(71, 7))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "0", "padding-left", "0", "padding-right", "0", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"))
        );
    }

    protected void onSelChange(String value)
    {
    }

    protected void onThumbClick()
    {
    }

    protected void onShowBtnClick()
    {
    }

    protected void onBrowseBtnClick()
    {
    }

    protected void onDeleteBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("thumb".equals(name))
        {
            onThumbClick();
            return true;
        }
        if ("show_btn".equals(name))
        {
            onShowBtnClick();
            return true;
        }
        if ("browse_btn".equals(name))
        {
            onBrowseBtnClick();
            return true;
        }
        if ("delete_btn".equals(name))
        {
            onDeleteBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("sel".equals(name))
        {
            onSelChange(value);
            return true;
        }
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
