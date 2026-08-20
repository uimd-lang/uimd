// Auto-generated UI code for image_show_dialog - DO NOT EDIT MANUALLY.
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

public class ImageShowDialogUI extends GeneratedWindowBase
{
    public Label title;
    public Image photo;
    public Button close_btn;

    public ImageShowDialogUI()
    {
        super("Image Show Dialog");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("dialog");
        setMcpMetadata(
            true,
            "ImageShowDialog",
            "# Image Show Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\ndescription: \"Dialog for showing a selected image.\"\ntags: [example, image, dialog]\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Image\"\n  description: \"Dialog title.\"\n  expose: false\n\nphoto:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Selected image preview.\"\n\nclose_btn:\n  type: button\n  title: Close\n  description: \"Close the dialog.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 1\n  background: \"#081426\"\n  margin: 4, 6, 4, 6\ncell:\n  padding: 1, 1, 1, 1\n@title:\n  color: \"#f8fafc\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-dialog-**--------------------------------------------------------------------------------+\n| title..**............................................................................... |\n| photo..**............................................................................... |\n* ........................................................................................ |\n* *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+-**-------------------------------+---------------------+-**------------------------------+\n|                                  |  close_btn........  |                                 |\n+----------------------------------+---------------------+---------------------------------+\n```\n",
            "Dialog for showing a selected image.",
            List.of(
            new GeneratedElementMetadata("title", "Dialog title.", false),
            new GeneratedElementMetadata("photo", "Selected image preview.", true),
            new GeneratedElementMetadata("close_btn", "Close the dialog.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#081426", "border-color", "transparent", "border-width-horizontal", "1", "border-width-vertical", "1", "margin-bottom", "4", "margin-left", "6", "margin-right", "6", "margin-top", "4"));
        title = addElement(new Label("title", "Image"));
        title.setStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "text-align", "center"));
        title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        photo = addElement(new Image("photo", "", "", "contain", "auto", "center", "middle"));
        photo.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        close_btn = addElement(new Button("close_btn", "Close"));
        close_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        close_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        close_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("")
                .setType("")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(22, 0, 34, 1))
                .setWidth(AxisDimension.auto(536870911))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(34, 1))
                .setCellCharsSize(new Size(34, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("")
                .setType("")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(22, 57, 33, 1))
                .setWidth(AxisDimension.auto(536870911))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(33, 1))
                .setCellCharsSize(new Size(33, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("title")
                .setType("label")
                .setCellName("dialog")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 90, 21))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(88, 1))
                .setCellCharsSize(new Size(90, 21))
                .setMarginRight(1)
                .setMarginBottom(20)
                .setCellStyle(Style.fromProperties("padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("close_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(0, 2, 536870911, 1))
                .setSourceCell(new Rect(22, 35, 21, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(21))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(17, 1))
                .setCellCharsSize(new Size(21, 1))
                .setMarginRight(2)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("photo")
                .setType("image")
                .setCellName("dialog")
                .setRelative(new Rect(1, 1, 536870911, 536870911))
                .setSourceCell(new Rect(0, 0, 90, 21))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(88, 20))
                .setCellCharsSize(new Size(90, 21))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"))
        );
    }

    protected void onPhotoClick()
    {
    }

    protected void onCloseBtnClick()
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
        return false;
    }
}
