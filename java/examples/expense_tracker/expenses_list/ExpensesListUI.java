// Auto-generated UI code for expenses_list - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedScrollViewBase;
import uimd.GeneratedWindowBase;
import uimd.Rect;
import uimd.ScrollView;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class ExpensesListUI extends GeneratedScrollViewBase
{

    public ExpensesListUI()
    {
        super("Expenses List");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ExpensesList",
            "# Expenses List\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: \"Scrollable list of interactive expense rows.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - ../expense_row/expense_row\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#172033\"\n@panel:\n  gap: 1\n  padding: 1\n  background: \"#172033\"\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------+\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n*                                                      |\n*                                                      |\n|                                                      |\n+------------------------------------------------------+\n```\n",
            "Scrollable list of interactive expense rows.",
            List.of());
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#172033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        setGeneratedScrollView(addElement(new ScrollView("__scrollview", 1)));
        scrollView().setStyle(Style.fromProperties("background", "#172033", "gap", "1", "padding", "1"));
        scrollView().setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        scrollView().setEditStyle(Style.fromProperties("background", "#ffffff18"));
        scrollView().setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
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
                .setName("__scrollview")
                .setType("uiscrollview")
                .setCellName("panel")
                .setRelative(new Rect(0, 0, 536870911, 536870911))
                .setSourceCell(new Rect(0, 0, 54, 7))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(54, 7))
                .setCellCharsSize(new Size(54, 7))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "gap", "1"))
                .setElementStyle(new Style())
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
