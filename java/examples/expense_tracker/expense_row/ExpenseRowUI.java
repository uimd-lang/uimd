// Auto-generated UI code for expense_row - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.CheckBox;
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

public class ExpenseRowUI extends GeneratedWindowBase
{
    public CheckBox paid;
    public Label item;
    public Label amount;
    public ComboBox category;
    public Button delete_btn;

    public ExpenseRowUI()
    {
        super("Expense Row");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ExpenseRow",
            "# Expense Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: \"Reusable expense row with paid state, category selector, and delete action.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\npaid:\n  type: checkbox\n  title: Paid\n  value: false\n\nitem:\n  type: label\n  text: Item\n  user-select: text\n\namount:\n  type: label\n  text: \"0.00\"\n\ncategory:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ndelete_btn:\n  type: button\n  title: Delete\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@row:\n  background: \"#172033\"\n  padding: 1\n@item:\n  color: \"#ffffff\"\n@amount:\n  color: \"#bfdbfe\"\ncheckbox:\n  color: \"#cbd5e1\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------+\n| paid..... item..**..................... amount...... |\n| category..**........................... delete_btn.. |\n+------------------------------------------------------+\n```\n",
            "Reusable expense row with paid state, category selector, and delete action.",
            List.of(
            new GeneratedElementMetadata("paid", "Paid", true),
            new GeneratedElementMetadata("item", "Item", true),
            new GeneratedElementMetadata("amount", "0.00", true),
            new GeneratedElementMetadata("category", "category", true),
            new GeneratedElementMetadata("delete_btn", "Delete", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        paid = addElement(new CheckBox("paid", "Paid", false));
        paid.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        paid.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#ffffff"));
        paid.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#22c55e"));
        paid.setUncheckedStyle(Style.fromProperties("background", "transparent", "color", "#94a3b8"));
        paid.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        item = addElement(new Label("item", "Item"));
        item.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff"));
        item.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        amount = addElement(new Label("amount", "0.00"));
        amount.setStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe"));
        amount.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        category = addElement(new ComboBox("category", List.of("Food", "Transport", "Home", "Health", "Other")));
        category.setSelectedIndex(0);
        category.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        category.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        category.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        category.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        category.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        delete_btn = addElement(new Button("delete_btn", "Delete"));
        delete_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        delete_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        delete_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("paid")
                .setType("checkbox")
                .setCellName("row")
                .setRelative(new Rect(0, 1, 9, 1))
                .setSourceCell(new Rect(0, 0, 54, 2))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(54, 2))
                .setMarginRight(44)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("item")
                .setType("label")
                .setCellName("row")
                .setRelative(new Rect(0, 11, 536870911, 536870911))
                .setSourceCell(new Rect(0, 0, 54, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(29, 2))
                .setCellCharsSize(new Size(54, 2))
                .setMarginRight(14)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff")),
            new GeneratedLayoutEntry()
                .setName("amount")
                .setType("label")
                .setCellName("row")
                .setRelative(new Rect(0, 41, 12, 1))
                .setSourceCell(new Rect(0, 0, 54, 2))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(54, 2))
                .setMarginRight(1)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe")),
            new GeneratedLayoutEntry()
                .setName("category")
                .setType("combobox")
                .setCellName("row")
                .setRelative(new Rect(1, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 54, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(39, 1))
                .setCellCharsSize(new Size(54, 2))
                .setMarginRight(14)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("delete_btn")
                .setType("button")
                .setCellName("row")
                .setRelative(new Rect(1, 41, 12, 1))
                .setSourceCell(new Rect(0, 0, 54, 2))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(54, 2))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"))
        );
    }

    protected void onPaidChange(String value)
    {
    }

    protected void onCategoryChange(String value)
    {
    }

    protected void onDeleteBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("delete_btn".equals(name))
        {
            onDeleteBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("paid".equals(name))
        {
            onPaidChange(value);
            return true;
        }
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
        if ("category".equals(name))
        {
            onCategoryChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}
