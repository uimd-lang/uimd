// Auto-generated UI code for edit_field_row - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;
import uimd.TextInput;

public class EditFieldRowUI extends GeneratedWindowBase
{
    public TextInput field_input;
    public Button field_btn;

    public EditFieldRowUI()
    {
        super("Edit Field Row");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "EditFieldRow",
            "# Edit Field Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"One editable field row with an action button inside the edit ScrollView.\"\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfield_input:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Editable regression field.\"\n\nfield_btn:\n  type: button\n  title: Action\n  description: \"Action button beside the editable field.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@row:\n  background: \"#111827\"\n  padding: 1, 1, 1, 1\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@field_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------------------------------------------+\n| field_input..**......................................................................... |\n|                                                                                          |\n| field_btn.......... .................................................................... |\n+------------------------------------------------------------------------------------------+\n```\n",
            "One editable field row with an action button inside the edit ScrollView.",
            List.of(
            new GeneratedElementMetadata("field_input", "Editable regression field.", true),
            new GeneratedElementMetadata("field_btn", "Action button beside the editable field.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        field_input = addElement(new TextInput("field_input", "", 120));
        field_input.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        field_input.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        field_input.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        field_input.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        field_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        field_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        field_btn = addElement(new Button("field_btn", "Action"));
        field_btn.setStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"));
        field_btn.setFocusStyle(Style.fromProperties("background", "#16a34a", "color", "#ffffff"));
        field_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("field_input")
                .setType("textinput")
                .setCellName("row")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 90, 3))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(88, 1))
                .setCellCharsSize(new Size(90, 3))
                .setMarginRight(1)
                .setMarginBottom(2)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("field_btn")
                .setType("button")
                .setCellName("row")
                .setRelative(new Rect(2, 1, 19, 1))
                .setSourceCell(new Rect(0, 0, 90, 3))
                .setWidth(AxisDimension.auto(19))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(19, 1))
                .setCellCharsSize(new Size(90, 3))
                .setMarginRight(70)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"))
        );
    }

    protected void onFieldInputChange(String value)
    {
    }

    protected void onFieldInputSubmit(String value)
    {
    }

    protected void onFieldBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("field_btn".equals(name))
        {
            onFieldBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("field_input".equals(name))
        {
            onFieldInputChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("field_input".equals(name))
        {
            onFieldInputSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        return false;
    }
}
