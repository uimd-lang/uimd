// Auto-generated UI code for table_entry - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.MessageTable;
import uimd.Rect;
import uimd.Size;
import uimd.SpanLabel;
import uimd.Style;
import uimd.TextGradient;

public class TableEntryUI extends GeneratedWindowBase
{
    public SpanLabel heading;
    public MessageTable table;

    public TableEntryUI()
    {
        super("Agent Chat Table Entry");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "TableEntry",
            "# Agent Chat Table Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style simulated agent Markdown table block.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Table\"\n  description: \"Table title.\"\n\ntable:\n  type: messagetable\n  text: \"\"\n  description: \"Rendered Markdown table content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@table_panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#ddd6fe\"\n@table:\n  color: \"#cbd5e1\"\n```\n\n## User Interface\n\n```ui\n+-table_panel-**----------------------------------------------------------+\n| heading..**........................................................... .|\n| table..**.............................................................. |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n",
            "Heron-style simulated agent Markdown table block.",
            List.of(
            new GeneratedElementMetadata("heading", "Table title.", true),
            new GeneratedElementMetadata("table", "Rendered Markdown table content.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        heading = addElement(new SpanLabel("heading", "Table"));
        heading.setStyle(Style.fromProperties("color", "#ddd6fe"));
        table = addElement(new MessageTable("table", ""));
        table.setStyle(Style.fromProperties("color", "#cbd5e1"));
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
                .setName("heading")
                .setType("spanlabel")
                .setCellName("table_panel")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 73, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(70, 1))
                .setCellCharsSize(new Size(73, 5))
                .setMarginRight(2)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#00000000", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("color", "#ddd6fe")),
            new GeneratedLayoutEntry()
                .setName("table")
                .setType("messagetable")
                .setCellName("table_panel")
                .setRelative(new Rect(1, 1, 536870911, 536870910))
                .setSourceCell(new Rect(0, 0, 73, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.fitContent())
                .setCharsSize(new Size(71, 4))
                .setCellCharsSize(new Size(73, 5))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#00000000", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("color", "#cbd5e1"))
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
