// Auto-generated UI code for expenses_view - DO NOT EDIT MANUALLY.
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
import uimd.NumberInput;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;
import uimd.TextInput;

public class ExpensesViewUI extends GeneratedWindowBase
{
    public Label view_title;
    public TextInput item_input;
    public NumberInput amount_input;
    public ComboBox category_input;
    public Label currency_label;
    public Button add_expense_btn;
    public ReusableElement expenses;

    public ExpensesViewUI()
    {
        super("Expenses View");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "ExpensesView",
            "# Expenses View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Expense entry view with a dynamic scrollable list.\"\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - expenses_list/expenses_list\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Expenses\n\nitem_input:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n\namount_input:\n  type: numberinput\n  value: 0\n  format: \"0.00\"\n\ncategory_input:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ncurrency_label:\n  type: label\n  text: EUR\n\nadd_expense_btn:\n  type: button\n  title: Add expense\n\nexpenses:\n  type: expenses_list\n  cpp-class: expense_tracker_example::ExpensesList\n  cpp-header: \"expenses_list/expenses_list.hpp\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#bfdbfe\"\n@currency_label:\n  color: \"#bfdbfe\"\n@expenses:\n  background: \"#172033\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nnumberinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Item:\"     item_input.................................... |\n|                                                            |\n| \"Amount:\"   amount_input............. currency_label...... |\n|                                                            |\n| \"Category:\" category_input................................ |\n|                                                            |\n| add_expense_btn........................................... |\n|                                                            |\n| expenses..**.............................................. |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n",
            "Expense entry view with a dynamic scrollable list.",
            List.of(
            new GeneratedElementMetadata("view_title", "Expenses", true),
            new GeneratedElementMetadata("item_input", "item_input", true),
            new GeneratedElementMetadata("amount_input", "amount_input", true),
            new GeneratedElementMetadata("category_input", "category_input", true),
            new GeneratedElementMetadata("currency_label", "EUR", true),
            new GeneratedElementMetadata("add_expense_btn", "Add expense", true),
            new GeneratedElementMetadata("expenses", "expenses", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        view_title = addElement(new Label("view_title", "Expenses"));
        view_title.setStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe"));
        view_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        item_input = addElement(new TextInput("item_input", "", 80));
        item_input.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        item_input.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        item_input.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#ffffff"));
        item_input.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        item_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        item_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        amount_input = addElement(new NumberInput("amount_input", 0.0, 1.0));
        amount_input.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        amount_input.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        amount_input.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#ffffff"));
        amount_input.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        amount_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        category_input = addElement(new ComboBox("category_input", List.of("Food", "Transport", "Home", "Health", "Other")));
        category_input.setSelectedIndex(0);
        category_input.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        category_input.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        category_input.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        category_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        category_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        currency_label = addElement(new Label("currency_label", "EUR"));
        currency_label.setStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe"));
        currency_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        add_expense_btn = addElement(new Button("add_expense_btn", "Add expense"));
        add_expense_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        add_expense_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        add_expense_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        expenses = addElement(new ReusableElement("expenses", "expenses_list"));
        expenses.setChild(new ExpensesListUI());
        expenses.setStyle(Style.fromProperties("background", "#172033"));
        expenses.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        expenses.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        expenses.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
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
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(58))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(58, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(1)
                .setMarginBottom(14)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe")),
            new GeneratedLayoutEntry()
                .setName("\"Item:\"")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(2, 1, 7, 1))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(7))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(7, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(52)
                .setMarginBottom(12)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("item_input")
                .setType("textinput")
                .setCellName("view")
                .setRelative(new Rect(2, 13, 46, 1))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(46))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(46, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(1)
                .setMarginBottom(12)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("\"Amount:\"")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(4, 1, 9, 1))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(50)
                .setMarginBottom(10)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("amount_input")
                .setType("numberinput")
                .setCellName("view")
                .setRelative(new Rect(4, 13, 25, 1))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(25))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(25, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(22)
                .setMarginBottom(10)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("currency_label")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(4, 39, 20, 1))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(1)
                .setMarginBottom(10)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe")),
            new GeneratedLayoutEntry()
                .setName("\"Category:\"")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(6, 1, 11, 1))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(48)
                .setMarginBottom(8)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("category_input")
                .setType("combobox")
                .setCellName("view")
                .setRelative(new Rect(6, 13, 46, 1))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(46))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(46, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(1)
                .setMarginBottom(8)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("add_expense_btn")
                .setType("button")
                .setCellName("view")
                .setRelative(new Rect(8, 1, 58, 1))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.auto(58))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(58, 1))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(1)
                .setMarginBottom(6)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("expenses")
                .setType("expenses_list")
                .setCellName("view")
                .setRelative(new Rect(10, 1, 536870911, 536870911))
                .setSourceCell(new Rect(0, 0, 60, 15))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(15))
                .setCharsSize(new Size(58, 5))
                .setCellCharsSize(new Size(60, 15))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#172033"))
        );
    }

    protected void onItemInputChange(String value)
    {
    }

    protected void onItemInputSubmit(String value)
    {
    }

    protected void onAmountInputChange(String value)
    {
    }

    protected void onAmountInputSubmit(String value)
    {
    }

    protected void onCategoryInputChange(String value)
    {
    }

    protected void onAddExpenseBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("add_expense_btn".equals(name))
        {
            onAddExpenseBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("item_input".equals(name))
        {
            onItemInputChange(value);
            return true;
        }
        if ("amount_input".equals(name))
        {
            onAmountInputChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("item_input".equals(name))
        {
            onItemInputSubmit(value);
            return true;
        }
        if ("amount_input".equals(name))
        {
            onAmountInputSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("category_input".equals(name))
        {
            onCategoryInputChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}
