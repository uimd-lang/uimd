// Auto-generated UI code for task_filters - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.ComboBox;
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

public class TaskFiltersUI extends GeneratedWindowBase
{
    public Label filters_label;
    public Label search_label;
    public Label status_label;
    public Label owner_label;
    public TextInput search;
    public ComboBox status_filter;
    public ComboBox owner_filter;
    public Button apply_filters_btn;
    public Button reset_filters_btn;

    public TaskFiltersUI()
    {
        super("Task Filters");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "TaskFilters",
            "# Task Filters\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: draft\ndescription: \"Reusable task board filter panel.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfilters_label:\n  type: label\n  text: FILTERS\n  description: \"Filter panel label.\"\n  expose: false\n\nsearch_label:\n  type: label\n  text: Search\n  description: \"Search filter label.\"\n  expose: false\n\nstatus_label:\n  type: label\n  text: Status\n  description: \"Status filter label.\"\n  expose: false\n\nowner_label:\n  type: label\n  text: Assignee\n  description: \"Assignee filter label.\"\n  expose: false\n\nsearch:\n  type: textinput\n  value: \"\"\n  maxlength: 60\n  placeholder: \"Task title or description\"\n  description: \"Filter tasks by title or description.\"\n\nstatus_filter:\n  type: combobox\n  options: [Any, Todo, Doing, Blocked, Done]\n  selected_item: Any\n  description: \"Filter tasks by status.\"\n\nowner_filter:\n  type: combobox\n  options: [Any, Ava, Bruno, Chen, Dana]\n  selected_item: Any\n  description: \"Filter tasks by assignee.\"\n\napply_filters_btn:\n  type: button\n  title: \"Apply filter\"\n  description: \"Apply the current task filters.\"\n\nreset_filters_btn:\n  type: button\n  title: Reset\n  description: \"Reset search, status, and assignee filters.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  background: \"#15171c\"\n  border-width: 0\nlabel:\n  color: \"#f5f5f5\"\ntextinput:\n  background: \"#252b35\"\n  color: \"#ffffff\"\n  focus-background: \"#303a49\"\n  edit-background: \"#344459\"\n  cursor-background: \"#93c5fd\"\n  cursor-color: \"#ffffff\"\ncombobox:\n  background: \"#252b35\"\n  color: \"#ffffff\"\n  focus-background: \"#303a49\"\n  edit-background: \"#344459\"\n  selected-background: \"#2f80ed\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#2f3545\"\n  color: \"#f5f5f5\"\n  focus-background: \"#2f80ed\"\n@filters_label:\n  color: \"#aab2c3\"\n@search_label:\n  color: \"#cfd5e3\"\n@status_label:\n  color: \"#cfd5e3\"\n@owner_label:\n  color: \"#cfd5e3\"\n@apply_filters_btn:\n  background: \"#2f80ed\"\n  focus-background: \"#1d6fd8\"\n@reset_filters_btn:\n  background: \"#343b4b\"\n```\n\n## User Interface\n\n```ui\n+-**---------------------------------------------------------------------------+\n|filters_label..**.............................................................|\n+----------------------------+----------------------+--------------------------+\n|search_label..**........... | status_label..**.... | owner_label..**......... |\n|search..**................. | status_filter.**.... | owner_filter..**........ |\n+-**-------------------------+----------------------+--------------------------+\n|                                                                              |\n|apply_filters_btn.....  reset_filters_btn.....                                |\n|                                                                              |\n+------------------------------------------------------------------------------+\n```\n",
            "Reusable task board filter panel.",
            List.of(
            new GeneratedElementMetadata("filters_label", "Filter panel label.", false),
            new GeneratedElementMetadata("search_label", "Search filter label.", false),
            new GeneratedElementMetadata("status_label", "Status filter label.", false),
            new GeneratedElementMetadata("owner_label", "Assignee filter label.", false),
            new GeneratedElementMetadata("search", "Filter tasks by title or description.", true),
            new GeneratedElementMetadata("status_filter", "Filter tasks by status.", true),
            new GeneratedElementMetadata("owner_filter", "Filter tasks by assignee.", true),
            new GeneratedElementMetadata("apply_filters_btn", "Apply the current task filters.", true),
            new GeneratedElementMetadata("reset_filters_btn", "Reset search, status, and assignee filters.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#15171c", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        filters_label = addElement(new Label("filters_label", "FILTERS"));
        filters_label.setStyle(Style.fromProperties("background", "transparent", "color", "#aab2c3"));
        filters_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        search_label = addElement(new Label("search_label", "Search"));
        search_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cfd5e3"));
        search_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        status_label = addElement(new Label("status_label", "Status"));
        status_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cfd5e3"));
        status_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        owner_label = addElement(new Label("owner_label", "Assignee"));
        owner_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cfd5e3"));
        owner_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        search = addElement(new TextInput("search", "", 60));
        search.setStyle(Style.fromProperties("background", "#252b35", "color", "#ffffff"));
        search.setFocusStyle(Style.fromProperties("background", "#303a49", "color", "#ffffff"));
        search.setEditStyle(Style.fromProperties("background", "#344459", "color", "#ffffff"));
        search.setCursorStyle(Style.fromProperties("background", "#93c5fd", "color", "#ffffff"));
        search.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        search.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        status_filter = addElement(new ComboBox("status_filter", List.of("Any", "Todo", "Doing", "Blocked", "Done")));
        status_filter.setSelectedIndex(0);
        status_filter.setStyle(Style.fromProperties("background", "#252b35", "color", "#ffffff"));
        status_filter.setFocusStyle(Style.fromProperties("background", "#303a49", "color", "#ffffff"));
        status_filter.setEditStyle(Style.fromProperties("background", "#344459", "color", "#e5e7eb"));
        status_filter.setSelectedStyle(Style.fromProperties("background", "#2f80ed", "color", "#ffffff"));
        status_filter.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        owner_filter = addElement(new ComboBox("owner_filter", List.of("Any", "Ava", "Bruno", "Chen", "Dana")));
        owner_filter.setSelectedIndex(0);
        owner_filter.setStyle(Style.fromProperties("background", "#252b35", "color", "#ffffff"));
        owner_filter.setFocusStyle(Style.fromProperties("background", "#303a49", "color", "#ffffff"));
        owner_filter.setEditStyle(Style.fromProperties("background", "#344459", "color", "#e5e7eb"));
        owner_filter.setSelectedStyle(Style.fromProperties("background", "#2f80ed", "color", "#ffffff"));
        owner_filter.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        apply_filters_btn = addElement(new Button("apply_filters_btn", "Apply filter"));
        apply_filters_btn.setStyle(Style.fromProperties("background", "#2f80ed", "color", "#f5f5f5"));
        apply_filters_btn.setFocusStyle(Style.fromProperties("background", "#1d6fd8", "color", "#ffffff"));
        apply_filters_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        reset_filters_btn = addElement(new Button("reset_filters_btn", "Reset"));
        reset_filters_btn.setStyle(Style.fromProperties("background", "#343b4b", "color", "#f5f5f5"));
        reset_filters_btn.setFocusStyle(Style.fromProperties("background", "#2f80ed", "color", "#ffffff"));
        reset_filters_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("filters_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 78, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(78, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#aab2c3")),
            new GeneratedLayoutEntry()
                .setName("search_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 28, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(27, 1))
                .setCellCharsSize(new Size(28, 2))
                .setMarginRight(1)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cfd5e3")),
            new GeneratedLayoutEntry()
                .setName("owner_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 52, 26, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(24, 1))
                .setCellCharsSize(new Size(26, 2))
                .setMarginRight(1)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cfd5e3")),
            new GeneratedLayoutEntry()
                .setName("status_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 29, 22, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(22, 2))
                .setMarginRight(1)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cfd5e3")),
            new GeneratedLayoutEntry()
                .setName("apply_filters_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(1, 0, 22, 1))
                .setSourceCell(new Rect(5, 0, 78, 3))
                .setWidth(AxisDimension.auto(22))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(22, 1))
                .setCellCharsSize(new Size(78, 3))
                .setMarginRight(56)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#2f80ed", "color", "#f5f5f5")),
            new GeneratedLayoutEntry()
                .setName("search")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(1, 0, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 28, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(28))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(27, 1))
                .setCellCharsSize(new Size(28, 2))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#252b35", "color", "#ffffff")),
            new GeneratedLayoutEntry()
                .setName("owner_filter")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(1, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 52, 26, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(24, 1))
                .setCellCharsSize(new Size(26, 2))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#252b35", "color", "#ffffff")),
            new GeneratedLayoutEntry()
                .setName("status_filter")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(1, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 29, 22, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(22))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(22, 2))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#252b35", "color", "#ffffff")),
            new GeneratedLayoutEntry()
                .setName("reset_filters_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(1, 24, 22, 1))
                .setSourceCell(new Rect(5, 0, 78, 3))
                .setWidth(AxisDimension.auto(22))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(22, 1))
                .setCellCharsSize(new Size(78, 3))
                .setMarginRight(32)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#343b4b", "color", "#f5f5f5"))
        );
    }

    protected void onSearchChange(String value)
    {
    }

    protected void onSearchSubmit(String value)
    {
    }

    protected void onStatusFilterChange(String value)
    {
    }

    protected void onOwnerFilterChange(String value)
    {
    }

    protected void onApplyFiltersBtnClick()
    {
    }

    protected void onResetFiltersBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("apply_filters_btn".equals(name))
        {
            onApplyFiltersBtnClick();
            return true;
        }
        if ("reset_filters_btn".equals(name))
        {
            onResetFiltersBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("search".equals(name))
        {
            onSearchChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("search".equals(name))
        {
            onSearchSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("status_filter".equals(name))
        {
            onStatusFilterChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        if ("owner_filter".equals(name))
        {
            onOwnerFilterChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}
