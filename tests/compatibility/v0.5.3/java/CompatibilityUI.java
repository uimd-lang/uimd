// Auto-generated UI code for compatibility - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.ComboBox;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;
import uimd.TextInput;

public class CompatibilityUI extends GeneratedWindowBase
{
    public TextInput aligned_input;
    public ComboBox choice;
    public ReusableElement scroller;

    public CompatibilityUI()
    {
        super("UIMD 0.5.3 Compatibility Application");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "Compatibility",
            "# UIMD 0.5.3 Compatibility Application\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"Retained generated application for patch-level runtime compatibility.\"\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nuses:\n  - compat_scroll\n```\n\n## Members\n\n```yaml\naligned_input:\n  type: textinput\n  value: \"abc\"\n  maxlength: 16\n\nchoice:\n  type: combobox\n  options: [one, two, three, four, five, six, seven, eight]\n  selected_item: one\n\nscroller:\n  type: compat_scroll\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#303545\"\n@aligned_input:\n  text-align: right\n@scroller:\n  background: \"#303545\"\n  focus-background: \"#ffffff14\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------+\n|aligned_input...................|\n|choice..........................|\n|................................|\n|scroller........................|\n|................................|\n+--------------------------------+\n```\n",
            "Retained generated application for patch-level runtime compatibility.",
            List.of(
            new GeneratedElementMetadata("aligned_input", "aligned_input", true),
            new GeneratedElementMetadata("choice", "choice", true),
            new GeneratedElementMetadata("scroller", "scroller", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#303545", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        aligned_input = addElement(new TextInput("aligned_input", "abc", 16));
        aligned_input.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb", "text-align", "right"));
        aligned_input.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        aligned_input.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        aligned_input.setCursorStyle(Style.fromProperties("background", "#ffffff", "color", "#0e1117"));
        aligned_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        aligned_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        choice = addElement(new ComboBox("choice", List.of("one", "two", "three", "four", "five", "six", "seven", "eight")));
        choice.setSelectedIndex(0);
        choice.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        choice.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        choice.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#e5e7eb"));
        choice.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        choice.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        scroller = addElement(new ReusableElement("scroller", "compat_scroll"));
        scroller.setChild(new CompatScrollUI());
        scroller.setStyle(Style.fromProperties("background", "#303545"));
        scroller.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        scroller.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        scroller.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
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
                .setName("aligned_input")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 32, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(32))
                .setCellHeight(AxisDimension.auto(5))
                .setCharsSize(new Size(32, 1))
                .setCellCharsSize(new Size(32, 5))
                .setMarginRight(0)
                .setMarginBottom(4)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb", "text-align", "right")),
            new GeneratedLayoutEntry()
                .setName("choice")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(1, 0, 536870911, 2))
                .setSourceCell(new Rect(0, 0, 32, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(2))
                .setCellWidth(AxisDimension.auto(32))
                .setCellHeight(AxisDimension.auto(5))
                .setCharsSize(new Size(32, 2))
                .setCellCharsSize(new Size(32, 5))
                .setMarginRight(0)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("scroller")
                .setType("compat_scroll")
                .setCellName("")
                .setRelative(new Rect(3, 0, 536870911, 2))
                .setSourceCell(new Rect(0, 0, 32, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(2))
                .setCellWidth(AxisDimension.auto(32))
                .setCellHeight(AxisDimension.auto(5))
                .setCharsSize(new Size(32, 2))
                .setCellCharsSize(new Size(32, 5))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#303545"))
        );
    }

    protected void onAlignedInputChange(String value)
    {
    }

    protected void onAlignedInputSubmit(String value)
    {
    }

    protected void onChoiceChange(String value)
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("aligned_input".equals(name))
        {
            onAlignedInputChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("aligned_input".equals(name))
        {
            onAlignedInputSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("choice".equals(name))
        {
            onChoiceChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}
