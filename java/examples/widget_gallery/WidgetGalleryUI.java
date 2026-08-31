// Auto-generated UI code for widget_gallery - DO NOT EDIT MANUALLY.
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
import uimd.ListBox;
import uimd.NumberInput;
import uimd.Rect;
import uimd.Size;
import uimd.SpanLabel;
import uimd.Style;
import uimd.TextGradient;
import uimd.TextInput;

public class WidgetGalleryUI extends GeneratedWindowBase
{
    public Label header;
    public Button close_btn;
    public Label plain_label;
    public Label selectable_label;
    public SpanLabel spans;
    public Label animation;
    public Label name_label;
    public TextInput name_input;
    public Label count_label;
    public NumberInput count_input;
    public CheckBox enabled_check;
    public Label theme_label;
    public ComboBox theme_combo;
    public Label mode_label;
    public ListBox mode_list;
    public Button apply_btn;
    public Button reset_btn;
    public Label summary;

    public WidgetGalleryUI()
    {
        super("Widget Gallery");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "WidgetGallery",
            "# Widget Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Widget Gallery\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the widget gallery.\"\n\nplain_label:\n  type: label\n  text: \"Plain label with inherited theme colors.\"\n  description: \"Plain label using inherited theme colors.\"\n\nselectable_label:\n  type: label\n  text: \"Select this text with the mouse to test label selection.\"\n  description: \"Selectable label used to test mouse text selection.\"\n\nspans:\n  type: spanlabel\n  text: \"span label\"\n  description: \"Span label demonstrating mixed foreground and background styling.\"\n\nanimation:\n  type: label\n  text: \"Animated label shows gradient-based rendering.\"\n  description: \"Animated gradient label.\"\n\nname_label:\n  type: label\n  text: Name\n  description: \"Label for the name input.\"\n\nname_input:\n  type: textinput\n  value: \"Ada Lovelace\"\n  maxlength: 80\n  description: \"Editable name text input.\"\n\ncount_label:\n  type: label\n  text: Count\n  description: \"Label for the count input.\"\n\ncount_input:\n  type: numberinput\n  value: 3\n  step_size: 1\n  min_value: 0\n  max_value: 99\n  format_str: 0\n  description: \"Numeric count input with min, max, and step constraints.\"\n\nenabled_check:\n  type: checkbox\n  title: \"Feature enabled\"\n  value: 1\n  description: \"Toggle for the feature enabled state.\"\n\ntheme_label:\n  type: label\n  text: Theme\n  description: \"Label for the theme selector.\"\n\ntheme_combo:\n  type: combobox\n  options: [Dark, Light, Solarized]\n  selected_item: Dark\n  description: \"Combobox selecting a visual theme value.\"\n\nmode_label:\n  type: label\n  text: Mode\n  description: \"Label for the mode list.\"\n\nmode_list:\n  type: listbox\n  options: [Preview, Edit, Review, Publish, Archive]\n  selected_items: [Preview]\n  multiple: \"false\"\n  description: \"Single-select listbox choosing the current workflow mode.\"\n\napply_btn:\n  type: button\n  title: Apply\n  description: \"Apply the current widget values.\"\n\nreset_btn:\n  type: button\n  title: Reset\n  description: \"Reset widget values to defaults.\"\n\nsummary:\n  type: label\n  text: \"values:\\n  name: Ada Lovelace\\n  count: 3\\n  theme: Dark\\n  mode: Preview\\n  enabled: true\"\n  description: \"Read-only summary of current widget values.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#18324f\"\n  color: \"#ffffff\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@title:\n  background: \"#18324f\"\n@selectable_label:\n  user-select: text\n  color: \"#a7f3d0\"\n@spans:\n  user-select: text\n  color: \"#cbd5e1\"\n@animation:\n  color: \"#fbbf24\"\n  text-color-gradient:\n    interval: 100\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\ntextinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nnumberinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncheckbox:\n  color: \"#d1d5db\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#f97316\"\n@summary:\n  background: \"#0d1524\"\n  color: \"#dbeafe\"\n```\n\n## User Interface\n\n```ui\n+-title------------------------------------------------+\n| header................................ close_btn.....|\n+-label_section----------------------------------------+\n|                                                      |\n|  plain_label.......................................  |\n|                                                      |\n|  selectable_label..................................  |\n|                                                      |\n|  spans.............................................  |\n|                                                      |\n|  animation.........................................  |\n|                                                      |\n+-control_section-----------+--section2----------------+\n|  name_label               |  theme_label             |\n|  name_input.............  |   theme_combo..........  |\n|                           |                          |\n|  count_label              |  mode_label              |\n|  count_input............  |  mode_list.............  |\n|                           |  ......................  |\n|  enabled_check..........  |  ......................  |\n|                           |  ......................  |\n|                           |  ......................  |\n+---------------------------+--------------------------+\n|                                                      |\n|  summary...........................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|                                                      |\n|  apply_btn..............  reset_btn................  |\n|                                                      |\n+------------------------------------------------------+\n```\n",
            "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.",
            List.of(
            new GeneratedElementMetadata("header", "Window title.", false),
            new GeneratedElementMetadata("close_btn", "Close the widget gallery.", true),
            new GeneratedElementMetadata("plain_label", "Plain label using inherited theme colors.", true),
            new GeneratedElementMetadata("selectable_label", "Selectable label used to test mouse text selection.", true),
            new GeneratedElementMetadata("spans", "Span label demonstrating mixed foreground and background styling.", true),
            new GeneratedElementMetadata("animation", "Animated gradient label.", true),
            new GeneratedElementMetadata("name_label", "Label for the name input.", true),
            new GeneratedElementMetadata("name_input", "Editable name text input.", true),
            new GeneratedElementMetadata("count_label", "Label for the count input.", true),
            new GeneratedElementMetadata("count_input", "Numeric count input with min, max, and step constraints.", true),
            new GeneratedElementMetadata("enabled_check", "Toggle for the feature enabled state.", true),
            new GeneratedElementMetadata("theme_label", "Label for the theme selector.", true),
            new GeneratedElementMetadata("theme_combo", "Combobox selecting a visual theme value.", true),
            new GeneratedElementMetadata("mode_label", "Label for the mode list.", true),
            new GeneratedElementMetadata("mode_list", "Single-select listbox choosing the current workflow mode.", true),
            new GeneratedElementMetadata("apply_btn", "Apply the current widget values.", true),
            new GeneratedElementMetadata("reset_btn", "Reset widget values to defaults.", true),
            new GeneratedElementMetadata("summary", "Read-only summary of current widget values.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header = addElement(new Label("header", "Widget Gallery"));
        header.setStyle(Style.fromProperties("background", "#18324f", "color", "#ffffff", "text-align", "center"));
        header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        close_btn = addElement(new Button("close_btn", "Quit"));
        close_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#ffffff"));
        close_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        close_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        plain_label = addElement(new Label("plain_label", "Plain label with inherited theme colors."));
        plain_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        plain_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        selectable_label = addElement(new Label("selectable_label", "Select this text with the mouse to test label selection."));
        selectable_label.setStyle(Style.fromProperties("background", "transparent", "color", "#a7f3d0", "user-select", "text"));
        selectable_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        spans = addElement(new SpanLabel("spans", "span label"));
        spans.setStyle(Style.fromProperties("color", "#cbd5e1", "user-select", "text"));
        animation = addElement(new Label("animation", "Animated label shows gradient-based rendering."));
        animation.setStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#fbbf24"), makeTextGradient(100, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null));
        animation.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        name_label = addElement(new Label("name_label", "Name"));
        name_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        name_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        name_input = addElement(new TextInput("name_input", "Ada Lovelace", 80));
        name_input.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        name_input.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        name_input.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        name_input.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        name_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        name_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        count_label = addElement(new Label("count_label", "Count"));
        count_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        count_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        count_input = addElement(new NumberInput("count_input", 3.0, 1.0));
        count_input.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        count_input.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        count_input.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        count_input.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        count_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        enabled_check = addElement(new CheckBox("enabled_check", "Feature enabled", true));
        enabled_check.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db"));
        enabled_check.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#ffffff"));
        enabled_check.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#22c55e"));
        enabled_check.setUncheckedStyle(Style.fromProperties("background", "transparent", "color", "#f97316"));
        enabled_check.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        theme_label = addElement(new Label("theme_label", "Theme"));
        theme_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        theme_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        theme_combo = addElement(new ComboBox("theme_combo", List.of("Dark", "Light", "Solarized")));
        theme_combo.setSelectedIndex(0);
        theme_combo.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        theme_combo.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        theme_combo.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#e5e7eb"));
        theme_combo.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        theme_combo.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        mode_label = addElement(new Label("mode_label", "Mode"));
        mode_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        mode_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        mode_list = addElement(new ListBox("mode_list", List.of("Preview", "Edit", "Review", "Publish", "Archive")));
        mode_list.setSelectedIndex(0);
        mode_list.setStyle(Style.fromProperties("background", "#0d1524", "color", "#cbd5e1"));
        mode_list.setFocusStyle(Style.fromProperties("background", "#1d2f4d", "color", "#ffffff"));
        mode_list.setEditStyle(Style.fromProperties("background", "#243a5c", "color", "#cbd5e1"));
        mode_list.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        mode_list.setActiveStyle(Style.fromProperties("background", "#DDDDDD99", "color", "#FFFFFF"));
        mode_list.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        apply_btn = addElement(new Button("apply_btn", "Apply"));
        apply_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        apply_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        apply_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        reset_btn = addElement(new Button("reset_btn", "Reset"));
        reset_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        reset_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        reset_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        summary = addElement(new Label("summary", "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true"));
        summary.setStyle(Style.fromProperties("background", "#0d1524", "color", "#dbeafe"));
        summary.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setName("header")
                .setType("label")
                .setCellName("title")
                .setRelative(new Rect(0, 1, 38, 1))
                .setSourceCell(new Rect(0, 0, 54, 1))
                .setWidth(AxisDimension.auto(38))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(38, 1))
                .setCellCharsSize(new Size(54, 1))
                .setMarginRight(15)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#18324f"))
                .setElementStyle(Style.fromProperties("background", "#18324f", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("name_label")
                .setType("label")
                .setCellName("control_section")
                .setRelative(new Rect(0, 2, 536870911, 1))
                .setSourceCell(new Rect(12, 0, 27, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(27, 9))
                .setMarginRight(15)
                .setMarginBottom(8)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("theme_label")
                .setType("label")
                .setCellName("section2")
                .setRelative(new Rect(0, 2, 536870911, 1))
                .setSourceCell(new Rect(12, 28, 26, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(26, 9))
                .setMarginRight(13)
                .setMarginBottom(8)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("close_btn")
                .setType("button")
                .setCellName("title")
                .setRelative(new Rect(0, 40, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 54, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(54, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#18324f"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#ffffff")),
            new GeneratedLayoutEntry()
                .setName("name_input")
                .setType("textinput")
                .setCellName("control_section")
                .setRelative(new Rect(1, 2, 536870911, 1))
                .setSourceCell(new Rect(12, 0, 27, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(23, 1))
                .setCellCharsSize(new Size(27, 9))
                .setMarginRight(2)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("plain_label")
                .setType("label")
                .setCellName("label_section")
                .setRelative(new Rect(1, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(50, 1))
                .setCellCharsSize(new Size(54, 9))
                .setMarginRight(2)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("summary")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(1, 2, 536870911, 6))
                .setSourceCell(new Rect(22, 0, 54, 10))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(6))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(10))
                .setCharsSize(new Size(50, 6))
                .setCellCharsSize(new Size(54, 10))
                .setMarginRight(2)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#dbeafe")),
            new GeneratedLayoutEntry()
                .setName("theme_combo")
                .setType("combobox")
                .setCellName("section2")
                .setRelative(new Rect(1, 3, 536870911, 1))
                .setSourceCell(new Rect(12, 28, 26, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(21, 1))
                .setCellCharsSize(new Size(26, 9))
                .setMarginRight(2)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("count_label")
                .setType("label")
                .setCellName("control_section")
                .setRelative(new Rect(3, 2, 536870911, 1))
                .setSourceCell(new Rect(12, 0, 27, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(27, 9))
                .setMarginRight(14)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("mode_label")
                .setType("label")
                .setCellName("section2")
                .setRelative(new Rect(3, 2, 536870911, 1))
                .setSourceCell(new Rect(12, 28, 26, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(26, 9))
                .setMarginRight(14)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("selectable_label")
                .setType("label")
                .setCellName("label_section")
                .setRelative(new Rect(3, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(50, 1))
                .setCellCharsSize(new Size(54, 9))
                .setMarginRight(2)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#a7f3d0", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("count_input")
                .setType("numberinput")
                .setCellName("control_section")
                .setRelative(new Rect(4, 2, 536870911, 1))
                .setSourceCell(new Rect(12, 0, 27, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(23, 1))
                .setCellCharsSize(new Size(27, 9))
                .setMarginRight(2)
                .setMarginBottom(4)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("mode_list")
                .setType("listbox")
                .setCellName("section2")
                .setRelative(new Rect(4, 2, 536870911, 5))
                .setSourceCell(new Rect(12, 28, 26, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(5))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(22, 5))
                .setCellCharsSize(new Size(26, 9))
                .setMarginRight(2)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("spans")
                .setType("spanlabel")
                .setCellName("label_section")
                .setRelative(new Rect(5, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(50, 1))
                .setCellCharsSize(new Size(54, 9))
                .setMarginRight(2)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("color", "#cbd5e1", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("enabled_check")
                .setType("checkbox")
                .setCellName("control_section")
                .setRelative(new Rect(6, 2, 536870911, 1))
                .setSourceCell(new Rect(12, 0, 27, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(27))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(23, 1))
                .setCellCharsSize(new Size(27, 9))
                .setMarginRight(2)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db")),
            new GeneratedLayoutEntry()
                .setName("animation")
                .setType("label")
                .setCellName("label_section")
                .setRelative(new Rect(7, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 9))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(9))
                .setCharsSize(new Size(50, 1))
                .setCellCharsSize(new Size(54, 9))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#fbbf24"), makeTextGradient(100, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null)),
            new GeneratedLayoutEntry()
                .setName("apply_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(8, 2, 23, 1))
                .setSourceCell(new Rect(22, 0, 54, 10))
                .setWidth(AxisDimension.auto(23))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(10))
                .setCharsSize(new Size(23, 1))
                .setCellCharsSize(new Size(54, 10))
                .setMarginRight(29)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("reset_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(8, 27, 536870911, 1))
                .setSourceCell(new Rect(22, 0, 54, 10))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(10))
                .setCharsSize(new Size(25, 1))
                .setCellCharsSize(new Size(54, 10))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onCloseBtnClick()
    {
    }

    protected void onNameInputChange(String value)
    {
    }

    protected void onNameInputSubmit(String value)
    {
    }

    protected void onCountInputChange(String value)
    {
    }

    protected void onCountInputSubmit(String value)
    {
    }

    protected void onEnabledCheckChange(String value)
    {
    }

    protected void onThemeComboChange(String value)
    {
    }

    protected void onModeListSelectionChange(List<String> value)
    {
    }

    protected boolean onModeListItemActivate(int index, String value)
    {
        return false;
    }

    protected void onApplyBtnClick()
    {
    }

    protected void onResetBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("close_btn".equals(name))
        {
            onCloseBtnClick();
            return true;
        }
        if ("apply_btn".equals(name))
        {
            onApplyBtnClick();
            return true;
        }
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
        if ("name_input".equals(name))
        {
            onNameInputChange(value);
            return true;
        }
        if ("count_input".equals(name))
        {
            onCountInputChange(value);
            return true;
        }
        if ("enabled_check".equals(name))
        {
            onEnabledCheckChange(value);
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
        if ("count_input".equals(name))
        {
            onCountInputSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("theme_combo".equals(name))
        {
            onThemeComboChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        if ("mode_list".equals(name))
        {
            onModeListSelectionChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedListBoxItemActivate(
        String name,
        String elementId,
        int index,
        String value)
    {
        if ("mode_list".equals(name))
        {
            return onModeListItemActivate(index, value);
        }
        return false;
    }
}
