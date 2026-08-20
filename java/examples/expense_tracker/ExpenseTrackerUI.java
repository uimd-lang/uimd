// Auto-generated UI code for expense_tracker - DO NOT EDIT MANUALLY.
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
import uimd.ViewHost;

public class ExpenseTrackerUI extends GeneratedWindowBase
{
    public Label app_title;
    public Button quit_btn;
    public Label nav_title;
    public Button expenses_btn;
    public Button categories_btn;
    public Button settings_btn;
    public ViewHost main;

    public ExpenseTrackerUI()
    {
        super("Expense Tracker");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "ExpenseTracker",
            "# Expense Tracker\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: experimental\ndescription: \"Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.\"\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nuses:\n  - expenses_view\n  - categories_view\n  - settings_view\n```\n\n## Members\n\n```yaml\napp_title:\n  type: label\n  text: \"Expense Tracker\"\n\nquit_btn:\n  type: button\n  title: Quit\n\nnav_title:\n  type: label\n  text: Expenses\n\nexpenses_btn:\n  type: button\n  title: Expenses\n\ncategories_btn:\n  type: button\n  title: Categories\n\nsettings_btn:\n  type: button\n  title: Settings\n\nmain:\n  type: viewhost\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: \"#1f2937\"\n  padding: 0\n@app_title:\n  color: \"#ffffff\"\n  text-align: center\n@nav:\n  background: \"#172033\"\n@content:\n  background: \"#030712\"\n@nav_title:\n  color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+--header-**------------------------------------------+\n| app_title..**.......................... quit_btn... |\n+--nav------------------+--content-**-----------------+\n| nav_title............ | main..**................... |\n|                       | *.......................... |\n| expenses_btn......... | *.......................... |\n|                       | ........................... |\n| categories_btn....... | ........................... |\n|                       | ........................... |\n| settings_btn......... | ........................... |\n*                       * ........................... |\n*                       * ........................... |\n+-----------------------+-----------------------------+\n```\n",
            "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.",
            List.of(
            new GeneratedElementMetadata("app_title", "Expense Tracker", true),
            new GeneratedElementMetadata("quit_btn", "Quit", true),
            new GeneratedElementMetadata("nav_title", "Expenses", true),
            new GeneratedElementMetadata("expenses_btn", "Expenses", true),
            new GeneratedElementMetadata("categories_btn", "Categories", true),
            new GeneratedElementMetadata("settings_btn", "Settings", true),
            new GeneratedElementMetadata("main", "main", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        app_title = addElement(new Label("app_title", "Expense Tracker"));
        app_title.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center"));
        app_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        quit_btn = addElement(new Button("quit_btn", "Quit"));
        quit_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        quit_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        quit_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        nav_title = addElement(new Label("nav_title", "Expenses"));
        nav_title.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff"));
        nav_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        expenses_btn = addElement(new Button("expenses_btn", "Expenses"));
        expenses_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        expenses_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        expenses_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        categories_btn = addElement(new Button("categories_btn", "Categories"));
        categories_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        categories_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        categories_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        settings_btn = addElement(new Button("settings_btn", "Settings"));
        settings_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        settings_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        settings_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        main = addElement(new ViewHost("main"));
        main.setStyle(Style.fromProperties("background", "#030712"));
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
                .setName("app_title")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 53, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(39, 1))
                .setCellCharsSize(new Size(53, 1))
                .setMarginRight(13)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("main")
                .setType("viewhost")
                .setCellName("content")
                .setRelative(new Rect(0, 1, 536870911, 536870911))
                .setSourceCell(new Rect(2, 24, 29, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(27, 9))
                .setCellCharsSize(new Size(29, 9))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "#030712")),
            new GeneratedLayoutEntry()
                .setName("nav_title")
                .setType("label")
                .setCellName("nav")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 23, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(23))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(21, 1))
                .setCellCharsSize(new Size(23, 9))
                .setMarginRight(1)
                .setMarginBottom(8)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff")),
            new GeneratedLayoutEntry()
                .setName("quit_btn")
                .setType("button")
                .setCellName("header")
                .setRelative(new Rect(0, 41, 11, 1))
                .setSourceCell(new Rect(0, 0, 53, 1))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(53, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937", "padding", "0"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("expenses_btn")
                .setType("button")
                .setCellName("nav")
                .setRelative(new Rect(2, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 23, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(23))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(21, 1))
                .setCellCharsSize(new Size(23, 9))
                .setMarginRight(1)
                .setMarginBottom(6)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("categories_btn")
                .setType("button")
                .setCellName("nav")
                .setRelative(new Rect(4, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 23, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(23))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(21, 1))
                .setCellCharsSize(new Size(23, 9))
                .setMarginRight(1)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("settings_btn")
                .setType("button")
                .setCellName("nav")
                .setRelative(new Rect(6, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 23, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(23))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(21, 1))
                .setCellCharsSize(new Size(23, 9))
                .setMarginRight(1)
                .setMarginBottom(2)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onQuitBtnClick()
    {
    }

    protected void onExpensesBtnClick()
    {
    }

    protected void onCategoriesBtnClick()
    {
    }

    protected void onSettingsBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("quit_btn".equals(name))
        {
            onQuitBtnClick();
            return true;
        }
        if ("expenses_btn".equals(name))
        {
            onExpensesBtnClick();
            return true;
        }
        if ("categories_btn".equals(name))
        {
            onCategoriesBtnClick();
            return true;
        }
        if ("settings_btn".equals(name))
        {
            onSettingsBtnClick();
            return true;
        }
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
