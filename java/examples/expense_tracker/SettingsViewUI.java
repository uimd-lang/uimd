// Auto-generated UI code for settings_view - DO NOT EDIT MANUALLY.
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
import uimd.NumberInput;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class SettingsViewUI extends GeneratedWindowBase
{
    public Label view_title;
    public ComboBox currency;
    public CheckBox show_paid;
    public NumberInput monthly_limit;
    public Button reset_btn;

    public SettingsViewUI()
    {
        super("Settings View");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("control");
        setMcpMetadata(
            true,
            "SettingsView",
            "# Settings View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Expense tracker settings view.\"\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Settings\n\ncurrency:\n  type: combobox\n  options: [EUR, USD, CZK]\n  selected_item: EUR\n\nshow_paid:\n  type: checkbox\n  title: Show paid expenses\n  value: true\n\nmonthly_limit:\n  type: numberinput\n  value: 500\n  format: \"0\"\n\nreset_btn:\n  type: button\n  title: Reset demo data\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#fde68a\"\nnumberinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Currency:\"      currency................................. |\n|                                                            |\n| show_paid................................................. |\n|                                                            |\n| \"Monthly limit:\" monthly_limit............................ |\n|                                                            |\n| reset_btn................................................. |\n+------------------------------------------------------------+\n```\n",
            "Expense tracker settings view.",
            List.of(
            new GeneratedElementMetadata("view_title", "Settings", true),
            new GeneratedElementMetadata("currency", "currency", true),
            new GeneratedElementMetadata("show_paid", "Show paid expenses", true),
            new GeneratedElementMetadata("monthly_limit", "monthly_limit", true),
            new GeneratedElementMetadata("reset_btn", "Reset demo data", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        view_title = addElement(new Label("view_title", "Settings"));
        view_title.setStyle(Style.fromProperties("background", "transparent", "color", "#fde68a"));
        view_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        currency = addElement(new ComboBox("currency", List.of("EUR", "USD", "CZK")));
        currency.setSelectedIndex(0);
        currency.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        currency.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        currency.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        currency.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        currency.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        show_paid = addElement(new CheckBox("show_paid", "Show paid expenses", true));
        show_paid.setStyle(Style.fromProperties("background", "transparent", "color", "#b8c4d8"));
        show_paid.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#ffffff"));
        show_paid.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#44cc88"));
        show_paid.setUncheckedStyle(Style.fromProperties("background", "transparent"));
        show_paid.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        monthly_limit = addElement(new NumberInput("monthly_limit", 500.0, 1.0));
        monthly_limit.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        monthly_limit.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        monthly_limit.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#ffffff"));
        monthly_limit.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        monthly_limit.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        reset_btn = addElement(new Button("reset_btn", "Reset demo data"));
        reset_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        reset_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        reset_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setSourceCell(new Rect(0, 0, 60, 9))
                .setWidth(AxisDimension.auto(58))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(58, 1))
                .setCellCharsSize(new Size(60, 9))
                .setMarginRight(1)
                .setMarginBottom(8)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#fde68a")),
            new GeneratedLayoutEntry()
                .setName("\"Currency:\"")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(2, 1, 11, 1))
                .setSourceCell(new Rect(0, 0, 60, 9))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(60, 9))
                .setMarginRight(48)
                .setMarginBottom(6)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("currency")
                .setType("combobox")
                .setCellName("view")
                .setRelative(new Rect(2, 18, 41, 1))
                .setSourceCell(new Rect(0, 0, 60, 9))
                .setWidth(AxisDimension.auto(41))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(41, 1))
                .setCellCharsSize(new Size(60, 9))
                .setMarginRight(1)
                .setMarginBottom(6)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("show_paid")
                .setType("checkbox")
                .setCellName("view")
                .setRelative(new Rect(4, 1, 58, 1))
                .setSourceCell(new Rect(0, 0, 60, 9))
                .setWidth(AxisDimension.auto(58))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(58, 1))
                .setCellCharsSize(new Size(60, 9))
                .setMarginRight(1)
                .setMarginBottom(4)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#b8c4d8")),
            new GeneratedLayoutEntry()
                .setName("\"Monthly limit:\"")
                .setType("label")
                .setCellName("view")
                .setRelative(new Rect(6, 1, 16, 1))
                .setSourceCell(new Rect(0, 0, 60, 9))
                .setWidth(AxisDimension.auto(16))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(16, 1))
                .setCellCharsSize(new Size(60, 9))
                .setMarginRight(43)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("monthly_limit")
                .setType("numberinput")
                .setCellName("view")
                .setRelative(new Rect(6, 18, 41, 1))
                .setSourceCell(new Rect(0, 0, 60, 9))
                .setWidth(AxisDimension.auto(41))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(41, 1))
                .setCellCharsSize(new Size(60, 9))
                .setMarginRight(1)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("reset_btn")
                .setType("button")
                .setCellName("view")
                .setRelative(new Rect(8, 1, 58, 1))
                .setSourceCell(new Rect(0, 0, 60, 9))
                .setWidth(AxisDimension.auto(58))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(58, 1))
                .setCellCharsSize(new Size(60, 9))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onCurrencyChange(String value)
    {
    }

    protected void onShowPaidChange(String value)
    {
    }

    protected void onMonthlyLimitChange(String value)
    {
    }

    protected void onMonthlyLimitSubmit(String value)
    {
    }

    protected void onResetBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("reset_btn".equals(name))
        {
            onResetBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("show_paid".equals(name))
        {
            onShowPaidChange(value);
            return true;
        }
        if ("monthly_limit".equals(name))
        {
            onMonthlyLimitChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("monthly_limit".equals(name))
        {
            onMonthlyLimitSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("currency".equals(name))
        {
            onCurrencyChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}
