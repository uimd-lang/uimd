// Auto-generated UI code for compat_scroll - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedScrollViewBase;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.ScrollView;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class CompatScrollUI extends GeneratedScrollViewBase
{
    public Label alpha_row;

    public CompatScrollUI()
    {
        super("UIMD 0.5.3 Compatibility Scroll");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "CompatScroll",
            "# UIMD 0.5.3 Compatibility Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.\"\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\n```\n\n## Members\n\n```yaml\nalpha_row:\n  type: label\n  text: \"legacy alpha row\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#303545\"\n@panel:\n  background: \"#303545\"\n  padding: 0\n@alpha_row:\n  background: \"#252a36cc\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------+\n|alpha_row.....................|\n|..............................|\n+------------------------------+\n```\n",
            "Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.",
            List.of(
            new GeneratedElementMetadata("alpha_row", "legacy alpha row", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#303545", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        setGeneratedScrollView(addElement(new ScrollView("__scrollview", 0)));
        scrollView().setStyle(Style.fromProperties("background", "#303545", "padding", "0"));
        scrollView().setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        scrollView().setEditStyle(Style.fromProperties("background", "#ffffff18"));
        scrollView().setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        alpha_row = addElement(new Label("alpha_row", "legacy alpha row"));
        alpha_row.setStyle(Style.fromProperties("background", "#252a36cc", "color", "#cbd5e1"));
        alpha_row.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
        );
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
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
