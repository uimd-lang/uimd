// Auto-generated UI code for item_row - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class ItemRowUI extends GeneratedWindowBase
{
    public Label name;
    public Label summary;
    public Label spacer;
    public Button open_btn;

    public ItemRowUI()
    {
        super("Item Row");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ItemRow",
            "# Item Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Interactive row with an Open button inside a ScrollView.\"\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nname:\n  type: label\n  text: \"Item\"\n  description: \"Item name.\"\n\nsummary:\n  type: label\n  text: \"Summary\"\n  description: \"Item summary.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible row spacer.\"\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open this item in the edit page.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@name:\n  color: \"#f8fafc\"\n  user-select: text\n@summary:\n  color: \"#93c5fd\"\n  user-select: text\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@open_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-card-**----------------------------------------------------------------------------------+\n| name..**................................................... summary..................... |\n| spacer..**................................................................ open_btn..... |\n+------------------------------------------------------------------------------------------+\n```\n",
            "Interactive row with an Open button inside a ScrollView.",
            List.of(
            new GeneratedElementMetadata("name", "Item name.", true),
            new GeneratedElementMetadata("summary", "Item summary.", true),
            new GeneratedElementMetadata("spacer", "Flexible row spacer.", false),
            new GeneratedElementMetadata("open_btn", "Open this item in the edit page.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        name = addElement(new Label("name", "Item"));
        name.setStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "user-select", "text"));
        name.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        summary = addElement(new Label("summary", "Summary"));
        summary.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd", "user-select", "text"));
        summary.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        spacer = addElement(new Label("spacer", ""));
        spacer.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        spacer.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        open_btn = addElement(new Button("open_btn", "Open"));
        open_btn.setStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"));
        open_btn.setFocusStyle(Style.fromProperties("background", "#16a34a", "color", "#ffffff"));
        open_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("name")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 90, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(59, 1))
                .setCellCharsSize(new Size(90, 2))
                .setMarginRight(30)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("summary")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(0, 61, 28, 2))
                .setSourceCell(new Rect(0, 0, 90, 2))
                .setWidth(AxisDimension.auto(28))
                .setHeight(AxisDimension.auto(2))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(28, 2))
                .setCellCharsSize(new Size(90, 2))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("spacer")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(1, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 90, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(74, 1))
                .setCellCharsSize(new Size(90, 2))
                .setMarginRight(15)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("open_btn")
                .setType("button")
                .setCellName("card")
                .setRelative(new Rect(1, 76, 13, 1))
                .setSourceCell(new Rect(0, 0, 90, 2))
                .setWidth(AxisDimension.auto(13))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(90, 2))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"))
        );
    }

    protected void onOpenBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("open_btn".equals(name))
        {
            onOpenBtnClick();
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
