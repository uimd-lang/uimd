// Auto-generated UI code for settings - DO NOT EDIT MANUALLY.
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

public class SettingsUI extends GeneratedWindowBase
{
    public Label header_title;
    public CheckBox auto_scroll;
    public CheckBox show_timestamps;
    public Label default_type_label;
    public ComboBox default_type;
    public Button save_button;
    public Button cancel_button;

    public SettingsUI()
    {
        super("Activity Feed Settings");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "Settings",
            "# Activity Feed Settings\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Settings dialog for activity feed auto-scroll, timestamps, and default activity type.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: \"Activity Feed Settings\"\n  description: \"Dialog title.\"\n  expose: false\n\nauto_scroll:\n  type: checkbox\n  title: \"Auto-scroll to newest\"\n  value: 1\n  description: \"Toggle whether new activity items scroll into view.\"\n\nshow_timestamps:\n  type: checkbox\n  title: \"Show timestamps\"\n  value: 1\n  description: \"Toggle whether newly added activity items include timestamps.\"\n\ndefault_type_label:\n  type: label\n  text: \"Default type\"\n  description: \"Label for the default activity type selector.\"\n  expose: false\n\ndefault_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: \"Default activity type used by the main window.\"\n\nsave_button:\n  type: button\n  title: Save\n  description: \"Apply settings and close the dialog.\"\n\ncancel_button:\n  type: button\n  title: Cancel\n  description: \"Close the dialog without applying settings.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@header_title:\n  text-align: center\n  color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\ncheckbox:\n  color: \"#d1d5db\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#f97316\"\n@buttons:\n  padding: 0, 2, 1, 2\n```\n\n## User Interface\n\n```ui\n+-header-----------------------------------+\n|header_title..**..........................|\n+-content----------------------------------+\n|                                          |\n|  auto_scroll...........................  |\n|                                          |\n|  show_timestamps.......................  |\n|                                          |\n|  default_type_label..  default_type....  |\n|                                          |\n+-buttons----------------------------------+\n|  save_button......  cancel_button......  |\n+------------------------------------------+\n```\n",
            "Settings dialog for activity feed auto-scroll, timestamps, and default activity type.",
            List.of(
            new GeneratedElementMetadata("header_title", "Dialog title.", false),
            new GeneratedElementMetadata("auto_scroll", "Toggle whether new activity items scroll into view.", true),
            new GeneratedElementMetadata("show_timestamps", "Toggle whether newly added activity items include timestamps.", true),
            new GeneratedElementMetadata("default_type_label", "Label for the default activity type selector.", false),
            new GeneratedElementMetadata("default_type", "Default activity type used by the main window.", true),
            new GeneratedElementMetadata("save_button", "Apply settings and close the dialog.", true),
            new GeneratedElementMetadata("cancel_button", "Close the dialog without applying settings.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header_title = addElement(new Label("header_title", "Activity Feed Settings"));
        header_title.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center"));
        header_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        auto_scroll = addElement(new CheckBox("auto_scroll", "Auto-scroll to newest", true));
        auto_scroll.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        auto_scroll.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#ffffff"));
        auto_scroll.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#22c55e"));
        auto_scroll.setUncheckedStyle(Style.fromProperties("background", "transparent", "color", "#f97316"));
        auto_scroll.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        show_timestamps = addElement(new CheckBox("show_timestamps", "Show timestamps", true));
        show_timestamps.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        show_timestamps.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#ffffff"));
        show_timestamps.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#22c55e"));
        show_timestamps.setUncheckedStyle(Style.fromProperties("background", "transparent", "color", "#f97316"));
        show_timestamps.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        default_type_label = addElement(new Label("default_type_label", "Default type"));
        default_type_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        default_type_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        default_type = addElement(new ComboBox("default_type", List.of("Info", "Task", "Warning", "Deploy", "Note")));
        default_type.setSelectedIndex(0);
        default_type.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        default_type.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        default_type.setEditStyle(Style.fromProperties("background", "#111827", "color", "#e5e7eb"));
        default_type.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        default_type.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        save_button = addElement(new Button("save_button", "Save"));
        save_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        save_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        save_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        cancel_button = addElement(new Button("cancel_button", "Cancel"));
        cancel_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        cancel_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        cancel_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("header_title")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 42, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(42))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(42, 1))
                .setCellCharsSize(new Size(42, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("save_button")
                .setType("button")
                .setCellName("buttons")
                .setRelative(new Rect(0, 2, 17, 1))
                .setSourceCell(new Rect(10, 0, 42, 1))
                .setWidth(AxisDimension.auto(17))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(42))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(17, 1))
                .setCellCharsSize(new Size(42, 1))
                .setMarginRight(23)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("padding-bottom", "1", "padding-left", "2", "padding-right", "2", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("cancel_button")
                .setType("button")
                .setCellName("buttons")
                .setRelative(new Rect(0, 21, 536870911, 1))
                .setSourceCell(new Rect(10, 0, 42, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(42))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(19, 1))
                .setCellCharsSize(new Size(42, 1))
                .setMarginRight(2)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("padding-bottom", "1", "padding-left", "2", "padding-right", "2", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("auto_scroll")
                .setType("checkbox")
                .setCellName("content")
                .setRelative(new Rect(1, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 42, 7))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(42))
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(38, 1))
                .setCellCharsSize(new Size(42, 7))
                .setMarginRight(2)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("show_timestamps")
                .setType("checkbox")
                .setCellName("content")
                .setRelative(new Rect(3, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 42, 7))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(42))
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(38, 1))
                .setCellCharsSize(new Size(42, 7))
                .setMarginRight(2)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("default_type_label")
                .setType("label")
                .setCellName("content")
                .setRelative(new Rect(5, 2, 20, 1))
                .setSourceCell(new Rect(2, 0, 42, 7))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(42))
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(42, 7))
                .setMarginRight(20)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("default_type")
                .setType("combobox")
                .setCellName("content")
                .setRelative(new Rect(5, 24, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 42, 7))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(42))
                .setCellHeight(AxisDimension.auto(7))
                .setCharsSize(new Size(16, 1))
                .setCellCharsSize(new Size(42, 7))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"))
        );
    }

    protected void onAutoScrollChange(String value)
    {
    }

    protected void onShowTimestampsChange(String value)
    {
    }

    protected void onDefaultTypeChange(String value)
    {
    }

    protected void onSaveButtonClick()
    {
    }

    protected void onCancelButtonClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("save_button".equals(name))
        {
            onSaveButtonClick();
            return true;
        }
        if ("cancel_button".equals(name))
        {
            onCancelButtonClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("auto_scroll".equals(name))
        {
            onAutoScrollChange(value);
            return true;
        }
        if ("show_timestamps".equals(name))
        {
            onShowTimestampsChange(value);
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
        if ("default_type".equals(name))
        {
            onDefaultTypeChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}
