// Auto-generated UI code for document_view - DO NOT EDIT MANUALLY.
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

public class DocumentViewUI extends GeneratedScrollViewBase
{

    public DocumentViewUI()
    {
        super("Document View");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "DocumentView",
            "# Document View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable scroll view that contains rendered markdown blocks such as paragraphs, headings, code blocks, and tables.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - blocks/paragraph\n  - blocks/heading\n  - blocks/code_block\n  - blocks/table_block\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@panel:\n  gap: 1\n  background: \"#00000000\"\n  padding: 1, 2, 1, 2\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------+\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n*                                                      |\n*                                                      |\n*                                                      |\n*                                                      |\n|                                                      |\n+------------------------------------------------------+\n```\n",
            "Reusable scroll view that contains rendered markdown blocks such as paragraphs, headings, code blocks, and tables.",
            List.of());
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        setGeneratedScrollView(addElement(new ScrollView("__scrollview", 1)));
        scrollView().setStyle(Style.fromProperties("background", "#00000000", "gap", "1", "padding-bottom", "1", "padding-left", "2", "padding-right", "2", "padding-top", "1"));
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
                .setSourceCell(new Rect(0, 0, 54, 10))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(54, 10))
                .setCellCharsSize(new Size(54, 10))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#00000000", "gap", "1"))
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
