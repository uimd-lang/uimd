// Auto-generated UI code for edit_page - DO NOT EDIT MANUALLY.
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
import uimd.TextInput;
import uimd.ViewHost;

public class EditPageUI extends GeneratedWindowBase
{
    public Label edit_title;
    public Button back_btn;
    public Button save_btn;
    public Label name_label;
    public TextInput name_input;
    public Label body_label;
    public ViewHost fields;
    public Label status;

    public EditPageUI()
    {
        super("Edit Page");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "EditPage",
            "# Edit Page\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Edit page shown after a ScrollView row button replaces the active ViewHost page.\"\ntags: [regression, issue-5, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - edit_fields_scroll\n```\n\n## Members\n\n```yaml\nedit_title:\n  type: label\n  text: \"Edit item\"\n  description: \"Edit page title.\"\n  expose: false\n\nback_btn:\n  type: button\n  title: Back\n  description: \"Return to the list page.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Dummy save action used by the regression app.\"\n\nname_label:\n  type: label\n  text: Name\n  description: \"Name field label.\"\n  expose: false\n\nname_input:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Editable item name.\"\n\nbody_label:\n  type: label\n  text: Fields\n  description: \"Fields panel label.\"\n  expose: false\n\nfields:\n  type: viewhost\n  description: \"Scrollable edit fields panel.\"\n\nstatus:\n  type: label\n  text: \"Opened from ScrollView\"\n  description: \"Edit page status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@top:\n  background: \"#172033\"\n@edit_title:\n  color: \"#ffffff\"\n  text-align: center\n@form:\n  background: \"#111827\"\n  padding: 1, 1, 1, 1\n@fields:\n  background: \"#030712\"\n  focus-background: \"#07111f\"\n  edit-background: \"#0f172a\"\n  scope-dim-background: \"#0000004d\"\n@status:\n  color: \"#93c5fd\"\nlabel:\n  color: \"#e5e7eb\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@save_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-top-**-----------------------------------------------------------------------------------+\n| back_btn.... edit_title..**.............................................. save_btn...... |\n+-form-**----------------------------------------------------------------------------------+\n| name_label.... name_input..**........................................................... |\n|                                                                                          |\n| status..**.............................................................................. |\n|                                                                                          |\n| body_label..**.......................................................................... |\n| fields..**.............................................................................. |\n| *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n|                                                                                          |\n+------------------------------------------------------------------------------------------+\n```\n",
            "Edit page shown after a ScrollView row button replaces the active ViewHost page.",
            List.of(
            new GeneratedElementMetadata("edit_title", "Edit page title.", false),
            new GeneratedElementMetadata("back_btn", "Return to the list page.", true),
            new GeneratedElementMetadata("save_btn", "Dummy save action used by the regression app.", true),
            new GeneratedElementMetadata("name_label", "Name field label.", false),
            new GeneratedElementMetadata("name_input", "Editable item name.", true),
            new GeneratedElementMetadata("body_label", "Fields panel label.", false),
            new GeneratedElementMetadata("fields", "Scrollable edit fields panel.", true),
            new GeneratedElementMetadata("status", "Edit page status.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        edit_title = addElement(new Label("edit_title", "Edit item"));
        edit_title.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center"));
        edit_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        back_btn = addElement(new Button("back_btn", "Back"));
        back_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        back_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        back_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        save_btn = addElement(new Button("save_btn", "Save"));
        save_btn.setStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"));
        save_btn.setFocusStyle(Style.fromProperties("background", "#16a34a", "color", "#ffffff"));
        save_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        name_label = addElement(new Label("name_label", "Name"));
        name_label.setStyle(Style.fromProperties("background", "transparent", "color", "#e5e7eb"));
        name_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        name_input = addElement(new TextInput("name_input", "", 80));
        name_input.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        name_input.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        name_input.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        name_input.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        name_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        name_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        body_label = addElement(new Label("body_label", "Fields"));
        body_label.setStyle(Style.fromProperties("background", "transparent", "color", "#e5e7eb"));
        body_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        fields = addElement(new ViewHost("fields"));
        fields.setStyle(Style.fromProperties("background", "#030712", "scope-dim-background", "#0000004d"));
        fields.setFocusStyle(Style.fromProperties("background", "#07111f"));
        fields.setEditStyle(Style.fromProperties("background", "#0f172a"));
        status = addElement(new Label("status", "Opened from ScrollView"));
        status.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        status.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setName("back_btn")
                .setType("button")
                .setCellName("top")
                .setRelative(new Rect(0, 1, 12, 1))
                .setSourceCell(new Rect(0, 0, 90, 1))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(90, 1))
                .setMarginRight(77)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("name_label")
                .setType("label")
                .setCellName("form")
                .setRelative(new Rect(0, 1, 14, 1))
                .setSourceCell(new Rect(2, 0, 90, 21))
                .setWidth(AxisDimension.auto(14))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(21))
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(90, 21))
                .setMarginRight(75)
                .setMarginBottom(20)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("edit_title")
                .setType("label")
                .setCellName("top")
                .setRelative(new Rect(0, 14, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 90, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(60, 1))
                .setCellCharsSize(new Size(90, 1))
                .setMarginRight(16)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("name_input")
                .setType("textinput")
                .setCellName("form")
                .setRelative(new Rect(0, 16, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 90, 21))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(21))
                .setCharsSize(new Size(73, 1))
                .setCellCharsSize(new Size(90, 21))
                .setMarginRight(1)
                .setMarginBottom(20)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("save_btn")
                .setType("button")
                .setCellName("top")
                .setRelative(new Rect(0, 75, 14, 1))
                .setSourceCell(new Rect(0, 0, 90, 1))
                .setWidth(AxisDimension.auto(14))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(90, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033"))
                .setElementStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("status")
                .setType("label")
                .setCellName("form")
                .setRelative(new Rect(2, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 90, 21))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(21))
                .setCharsSize(new Size(88, 1))
                .setCellCharsSize(new Size(90, 21))
                .setMarginRight(1)
                .setMarginBottom(18)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("body_label")
                .setType("label")
                .setCellName("form")
                .setRelative(new Rect(4, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 90, 21))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(21))
                .setCharsSize(new Size(88, 1))
                .setCellCharsSize(new Size(90, 21))
                .setMarginRight(1)
                .setMarginBottom(16)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("fields")
                .setType("viewhost")
                .setCellName("form")
                .setRelative(new Rect(5, 1, 536870911, 536870911))
                .setSourceCell(new Rect(2, 0, 90, 21))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(21))
                .setCharsSize(new Size(88, 15))
                .setCellCharsSize(new Size(90, 21))
                .setMarginRight(1)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#030712", "scope-dim-background", "#0000004d"))
        );
    }

    protected void onBackBtnClick()
    {
    }

    protected void onSaveBtnClick()
    {
    }

    protected void onNameInputChange(String value)
    {
    }

    protected void onNameInputSubmit(String value)
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("back_btn".equals(name))
        {
            onBackBtnClick();
            return true;
        }
        if ("save_btn".equals(name))
        {
            onSaveBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("name_input".equals(name))
        {
            onNameInputChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("name_input".equals(name))
        {
            onNameInputSubmit(value);
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
