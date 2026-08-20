// Auto-generated UI code for image_list_view - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
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

public class ImageListViewUI extends GeneratedWindowBase
{
    public Button add_btn;
    public ReusableElement items;

    public ImageListViewUI()
    {
        super("Image List View");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "ImageListView",
            "# Image List View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"ViewHost page with add button and scrollable image item list.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_list_scroll\n```\n\n## Members\n\n```yaml\nadd_btn:\n  type: button\n  title: Add\n  description: \"Add an image item.\"\n\nitems:\n  type: image_list_scroll\n  description: \"Scrollable list of image items.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@toolbar:\n  background: \"#030712\"\n@items_cell:\n  background: \"#030712\"\nbutton:\n  background: \"#14532d\"\n  color: \"#e2e8f0\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-toolbar-**------------------------------------------------------------+\n| add_btn............                                                   |\n+-items_cell-**---------------------------------------------------------+\n|items..**..............................................................|\n*.......................................................................|\n**......................................................................|\n|*......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n|.......................................................................|\n+-----------------------------------------------------------------------+\n```\n",
            "ViewHost page with add button and scrollable image item list.",
            List.of(
            new GeneratedElementMetadata("add_btn", "Add an image item.", true),
            new GeneratedElementMetadata("items", "Scrollable list of image items.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        add_btn = addElement(new Button("add_btn", "Add"));
        add_btn.setStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"));
        add_btn.setFocusStyle(Style.fromProperties("background", "#16a34a", "color", "#ffffff"));
        add_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        items = addElement(new ReusableElement("items", "image_list_scroll"));
        items.setChild(new ImageListScrollUI());
        items.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        items.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        items.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
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
                .setName("items")
                .setType("image_list_scroll")
                .setCellName("items_cell")
                .setRelative(new Rect(0, 0, 536870911, 536870911))
                .setSourceCell(new Rect(2, 0, 71, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(71, 11))
                .setCellCharsSize(new Size(71, 11))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712"))
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("add_btn")
                .setType("button")
                .setCellName("toolbar")
                .setRelative(new Rect(0, 1, 19, 1))
                .setSourceCell(new Rect(0, 0, 71, 1))
                .setWidth(AxisDimension.auto(19))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(19, 1))
                .setCellCharsSize(new Size(71, 1))
                .setMarginRight(51)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712"))
                .setElementStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"))
        );
    }

    protected void onAddBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("add_btn".equals(name))
        {
            onAddBtnClick();
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
