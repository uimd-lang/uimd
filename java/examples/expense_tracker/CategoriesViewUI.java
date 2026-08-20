// Auto-generated UI code for categories_view - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.ListBox;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;
import uimd.TextInput;

public class CategoriesViewUI extends GeneratedWindowBase
{
    public Label view_title;
    public TextInput category_name;
    public Button add_category_btn;
    public ListBox categories;
    public Button remove_category_btn;

    public CategoriesViewUI()
    {
        super("Categories View");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "CategoriesView",
            "# Categories View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Simple category list management view.\"\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Categories\n\ncategory_name:\n  type: textinput\n  value: \"\"\n  maxlength: 40\n\nadd_category_btn:\n  type: button\n  title: Add\n\ncategories:\n  type: listbox\n  options: [Food, Transport, Home, Health, Other]\n  selected_items: [Food]\n\nremove_category_btn:\n  type: button\n  title: Remove selected\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#bbf7d0\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nlistbox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Name:\" category_name..**................................ |\n|                                                            |\n| add_category_btn................ remove_category_btn..... |\n|                                                            |\n| categories..**............................................ |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n",
            "Simple category list management view.",
            List.of(
            new GeneratedElementMetadata("view_title", "Categories", true),
            new GeneratedElementMetadata("category_name", "category_name", true),
            new GeneratedElementMetadata("add_category_btn", "Add", true),
            new GeneratedElementMetadata("categories", "categories", true),
            new GeneratedElementMetadata("remove_category_btn", "Remove selected", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        view_title = addElement(new Label("view_title", "Categories"));
        view_title.setStyle(Style.fromProperties("background", "transparent", "color", "#bbf7d0"));
        view_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        category_name = addElement(new TextInput("category_name", "", 40));
        category_name.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        category_name.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        category_name.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#ffffff"));
        category_name.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        category_name.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        category_name.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        add_category_btn = addElement(new Button("add_category_btn", "Add"));
        add_category_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        add_category_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        add_category_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        categories = addElement(new ListBox("categories", List.of("Food", "Transport", "Home", "Health", "Other")));
        categories.setSelectedIndex(0);
        categories.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        categories.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        categories.setEditStyle(Style.fromProperties("background", "#243a5c", "color", "#cbd5e1"));
        categories.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        categories.setActiveStyle(Style.fromProperties("background", "#DDDDDD99", "color", "#FFFFFF"));
        categories.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        remove_category_btn = addElement(new Button("remove_category_btn", "Remove selected"));
        remove_category_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        remove_category_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        remove_category_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("view_title")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(0, 1, 58, 1))
                .setSourceCell(new Rect(0, 0, 60, 11))
                .setWidth(AxisDimension.auto(58))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(58, 1))
                .setCellCharsSize(new Size(60, 11))
                .setMarginRight(1)
                .setMarginBottom(10)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#bbf7d0")),
            new GeneratedLayoutEntry()
                .setName("\"Name:\"")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(2, 1, 7, 1))
                .setSourceCell(new Rect(0, 0, 60, 11))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(7, 1))
                .setCellCharsSize(new Size(60, 11))
                .setMarginRight(52)
                .setMarginBottom(8)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("category_name")
                .setType("textinput")
                .setCellName("view")
                .setRelative(new Rect(2, 9, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 60, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(49, 1))
                .setCellCharsSize(new Size(60, 11))
                .setMarginRight(2)
                .setMarginBottom(8)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("add_category_btn")
                .setType("button")
                .setCellName("view")
                .setRelative(new Rect(4, 1, 32, 1))
                .setSourceCell(new Rect(0, 0, 60, 11))
                .setWidth(AxisDimension.auto(32))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(32, 1))
                .setCellCharsSize(new Size(60, 11))
                .setMarginRight(27)
                .setMarginBottom(6)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("remove_category_btn")
                .setType("button")
                .setCellName("view")
                .setRelative(new Rect(4, 34, 24, 1))
                .setSourceCell(new Rect(0, 0, 60, 11))
                .setWidth(AxisDimension.auto(24))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(24, 1))
                .setCellCharsSize(new Size(60, 11))
                .setMarginRight(2)
                .setMarginBottom(6)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("categories")
                .setType("listbox")
                .setCellName("view")
                .setRelative(new Rect(6, 1, 536870911, 536870911))
                .setSourceCell(new Rect(0, 0, 60, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(58, 5))
                .setCellCharsSize(new Size(60, 11))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"))
        );
    }

    protected void onCategoryNameChange(String value)
    {
    }

    protected void onCategoryNameSubmit(String value)
    {
    }

    protected void onAddCategoryBtnClick()
    {
    }

    protected void onCategoriesSelectionChange(List<String> value)
    {
    }

    protected void onRemoveCategoryBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("add_category_btn".equals(name))
        {
            onAddCategoryBtnClick();
            return true;
        }
        if ("remove_category_btn".equals(name))
        {
            onRemoveCategoryBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("category_name".equals(name))
        {
            onCategoryNameChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("category_name".equals(name))
        {
            onCategoryNameSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("categories".equals(name))
        {
            onCategoriesSelectionChange(value);
            return true;
        }
        return false;
    }
}
