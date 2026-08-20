// Auto-generated UI code for special_elements - DO NOT EDIT MANUALLY.
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
import uimd.ListBox;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextArea;
import uimd.TextGradient;
import uimd.TextInput;

public class SpecialElementsUI extends GeneratedWindowBase
{
    public Label header_label;
    public Label info_processing;
    public Label info_loading;
    public Label info_complete;
    public Label info_warning;
    public Label info_multiline;
    public Label status_label;
    public Label normal_info;
    public Label speed_label;
    public Label raw_label;
    public Button raw_button;
    public ComboBox raw_combo;
    public ListBox raw_list;
    public TextInput raw_input;
    public TextArea raw_area;

    public SpecialElementsUI()
    {
        super("Special Elements Demo");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "SpecialElements",
            "# Special Elements Demo\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_label:\n  type: label\n  text: \"Special UI Elements\"\n  description: \"Window title.\"\n\ninfo_processing:\n  type: label\n  text: \"Processing request...\"\n  color: \"#ff0000\"\n  description: \"Animated processing status label.\"\n\ninfo_loading:\n  type: label\n  text: \"Loading data from server...\"\n  color: \"#00ff00\"\n  description: \"Animated loading status label.\"\n\ninfo_complete:\n  type: label\n  text: \"Operation completed successfully!\"\n  color: \"#0000ff\"\n  description: \"Animated completed status label.\"\n\ninfo_warning:\n  type: label\n  text: \"Warning: Low disk space detected\"\n  color: \"#ffffff\"\n  description: \"Animated warning status label.\"\n\ninfo_multiline:\n  type: label\n  text: \"Line one of animated text\\nLine two with different phase\\nLine three final row\"\n  description: \"Multiline animated label with different text rows.\"\n\nstatus_label:\n  type: label\n  text: \"Status: Idle\"\n  description: \"Current status text.\"\n\nnormal_info:\n  type: label\n  text: \"Regular static label for comparison\"\n  description: \"Regular static comparison label.\"\n\nspeed_label:\n  type: label\n  text: \"Fast animation speed demo\"\n  description: \"Fast animation speed demonstration label.\"\n\nraw_label:\n  type: label\n  text: \"Label tab\\tcontrol\\x1b bell\\x07 combining e\\u0301 wide\\u4e2d\"\n  user-select: text\n  description: \"Label containing tab, control, combining, and wide characters.\"\n\nraw_button:\n  type: button\n  title: \"Button\\tESC\\x1bWide\\u4e2d\"\n  description: \"Button title containing raw terminal-sensitive characters.\"\n\nraw_combo:\n  type: combobox\n  options: [\"Plain option\", \"Tab\\toption\", \"Esc\\x1boption\", \"Wide\\u4e2doption\"]\n  selected_item: \"Tab\\toption\"\n  description: \"ComboBox options containing raw terminal-sensitive characters.\"\n\nraw_list:\n  type: listbox\n  options: [\"Plain row\", \"Tab\\trow\", \"Esc\\x1brow\", \"Wide\\u4e2drow\"]\n  selected_items: [\"Tab\\trow\"]\n  description: \"ListBox options containing raw terminal-sensitive characters.\"\n\nraw_input:\n  type: textinput\n  value: \"Input\\tvalue\\x1bwide\\u4e2d\"\n  maxlength: 80\n  description: \"Text input containing raw terminal-sensitive characters.\"\n\nraw_area:\n  type: textarea\n  value: \"Python tabs example:\\ndef hello(name):\\n\\tif name:\\n\\t\\tprint(\\\"Hello, \\\" + name)\\n\\telse:\\n\\t\\tprint(\\\"Hello, world!\\\")\\n\\nControl\\x1bplaceholder\\nWide\\u4e2dplaceholder\"\n  maxlength: 240\n  description: \"Text area containing multiline raw terminal-sensitive characters.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\n@info_processing:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_loading:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_complete:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_warning:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_multiline:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@speed_label:\n  text-color-gradient:\n    interval: 35\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\ntextinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nlistbox:\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------+\n|                                                                    |\n|  header_label....................................................  |\n|                                                                    |\n|  info_processing................  info_loading...................  |\n|  info_complete..................  info_warning...................  |\n|                                                                    |\n|  \"Multiline animated text:\"                                         |\n|  info_multiline..................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  status_label...................  normal_info....................  |\n|  speed_label.....................................................  |\n|                                                                    |\n|  \"Raw read-only text:\"                                             |\n|  raw_label.......................................................  |\n|                                                                    |\n|  \"Raw button:\" raw_button................                          |\n|                                                                    |\n|  \"Raw combo:\"  raw_combo......................                     |\n|                                                                    |\n|  \"Raw list:\"                                                       |\n|  raw_list........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  \"Text input:\" raw_input.........................................  |\n|                                                                    |\n|  \"Text area:\"                                                      |\n|  raw_area........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n+--------------------------------------------------------------------+\n```\n",
            "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.",
            List.of(
            new GeneratedElementMetadata("header_label", "Window title.", true),
            new GeneratedElementMetadata("info_processing", "Animated processing status label.", true),
            new GeneratedElementMetadata("info_loading", "Animated loading status label.", true),
            new GeneratedElementMetadata("info_complete", "Animated completed status label.", true),
            new GeneratedElementMetadata("info_warning", "Animated warning status label.", true),
            new GeneratedElementMetadata("info_multiline", "Multiline animated label with different text rows.", true),
            new GeneratedElementMetadata("status_label", "Current status text.", true),
            new GeneratedElementMetadata("normal_info", "Regular static comparison label.", true),
            new GeneratedElementMetadata("speed_label", "Fast animation speed demonstration label.", true),
            new GeneratedElementMetadata("raw_label", "Label containing tab, control, combining, and wide characters.", true),
            new GeneratedElementMetadata("raw_button", "Button title containing raw terminal-sensitive characters.", true),
            new GeneratedElementMetadata("raw_combo", "ComboBox options containing raw terminal-sensitive characters.", true),
            new GeneratedElementMetadata("raw_list", "ListBox options containing raw terminal-sensitive characters.", true),
            new GeneratedElementMetadata("raw_input", "Text input containing raw terminal-sensitive characters.", true),
            new GeneratedElementMetadata("raw_area", "Text area containing multiline raw terminal-sensitive characters.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header_label = addElement(new Label("header_label", "Special UI Elements"));
        header_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        header_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        info_processing = addElement(new Label("info_processing", "Processing request..."));
        info_processing.setStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#ff0000"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null));
        info_processing.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        info_loading = addElement(new Label("info_loading", "Loading data from server..."));
        info_loading.setStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#00ff00"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null));
        info_loading.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        info_complete = addElement(new Label("info_complete", "Operation completed successfully!"));
        info_complete.setStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#0000ff"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null));
        info_complete.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        info_warning = addElement(new Label("info_warning", "Warning: Low disk space detected"));
        info_warning.setStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#ffffff"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null));
        info_warning.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        info_multiline = addElement(new Label("info_multiline", "Line one of animated text\nLine two with different phase\nLine three final row"));
        info_multiline.setStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#cbd5e1"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null));
        info_multiline.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        status_label = addElement(new Label("status_label", "Status: Idle"));
        status_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        status_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        normal_info = addElement(new Label("normal_info", "Regular static label for comparison"));
        normal_info.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        normal_info.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        speed_label = addElement(new Label("speed_label", "Fast animation speed demo"));
        speed_label.setStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#cbd5e1"), makeTextGradient(35, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null));
        speed_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        raw_label = addElement(new Label("raw_label", "Label tab\tcontrol\u001b bell\u0007 combining é wide中"));
        raw_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        raw_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        raw_button = addElement(new Button("raw_button", "Button\tESC\u001bWide中"));
        raw_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        raw_button.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        raw_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        raw_combo = addElement(new ComboBox("raw_combo", List.of("Plain option", "Tab\toption", "Esc\u001boption", "Wide中option")));
        raw_combo.setSelectedIndex(1);
        raw_combo.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        raw_combo.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        raw_combo.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#e5e7eb"));
        raw_combo.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        raw_combo.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        raw_list = addElement(new ListBox("raw_list", List.of("Plain row", "Tab\trow", "Esc\u001brow", "Wide中row")));
        raw_list.setSelectedIndex(1);
        raw_list.setStyle(Style.fromProperties("background", "#0d1524", "color", "#cbd5e1"));
        raw_list.setFocusStyle(Style.fromProperties("background", "#1d2f4d", "color", "#ffffff"));
        raw_list.setEditStyle(Style.fromProperties("background", "#243a5c", "color", "#cbd5e1"));
        raw_list.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        raw_list.setActiveStyle(Style.fromProperties("background", "#DDDDDD99", "color", "#FFFFFF"));
        raw_list.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        raw_input = addElement(new TextInput("raw_input", "Input\tvalue\u001bwide中", 80));
        raw_input.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        raw_input.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        raw_input.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        raw_input.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        raw_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        raw_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        raw_area = addElement(new TextArea("raw_area", "Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint(\"Hello, \" + name)\n\telse:\n\t\tprint(\"Hello, world!\")\n\nControl\u001bplaceholder\nWide中placeholder", 240));
        raw_area.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        raw_area.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        raw_area.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        raw_area.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        raw_area.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        raw_area.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
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
                .setName("header_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(1, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(64, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(32)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("info_processing")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(3, 2, 31, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.auto(31))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(35)
                .setMarginBottom(30)
                .setCellStyle(new Style())
                .setElementStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#cbd5e1"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null)),
            new GeneratedLayoutEntry()
                .setName("info_loading")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(3, 35, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(30)
                .setCellStyle(new Style())
                .setElementStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#cbd5e1"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null)),
            new GeneratedLayoutEntry()
                .setName("info_complete")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(4, 2, 31, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.auto(31))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(35)
                .setMarginBottom(29)
                .setCellStyle(new Style())
                .setElementStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#cbd5e1"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null)),
            new GeneratedLayoutEntry()
                .setName("info_warning")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(4, 35, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(29)
                .setCellStyle(new Style())
                .setElementStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#cbd5e1"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null)),
            new GeneratedLayoutEntry()
                .setName("\"Multiline animated text:\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(6, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(40)
                .setMarginBottom(27)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("info_multiline")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(7, 2, 536870911, 3))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(64, 3))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(24)
                .setCellStyle(new Style())
                .setElementStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#cbd5e1"), makeTextGradient(70, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null)),
            new GeneratedLayoutEntry()
                .setName("status_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(11, 2, 31, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.auto(31))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(35)
                .setMarginBottom(22)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("normal_info")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(11, 35, 536870911, 2))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(2))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(31, 2))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(21)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("speed_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(12, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(64, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(21)
                .setCellStyle(new Style())
                .setElementStyle(styleWithGradients(Style.fromProperties("background", "transparent", "color", "#cbd5e1"), makeTextGradient(35, 1, 1, "#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"), null)),
            new GeneratedLayoutEntry()
                .setName("\"Raw read-only text:\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(14, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(21, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(45)
                .setMarginBottom(19)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("raw_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(15, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(64, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(18)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("\"Raw button:\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(17, 2, 13, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.auto(13))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(53)
                .setMarginBottom(16)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("raw_button")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(17, 16, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(26)
                .setMarginBottom(16)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("\"Raw combo:\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(19, 2, 12, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(54)
                .setMarginBottom(14)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("raw_combo")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(19, 16, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(21)
                .setMarginBottom(14)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("\"Raw list:\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(21, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(55)
                .setMarginBottom(12)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("raw_list")
                .setType("listbox")
                .setCellName("")
                .setRelative(new Rect(22, 2, 536870911, 3))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(64, 3))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("\"Text input:\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(26, 2, 13, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.auto(13))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(53)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("raw_input")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(26, 16, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(50, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("\"Text area:\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(28, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(54)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("raw_area")
                .setType("textarea")
                .setCellName("")
                .setRelative(new Rect(29, 2, 536870911, 4))
                .setSourceCell(new Rect(0, 0, 68, 34))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(4))
                .setCellWidth(AxisDimension.auto(68))
                .setCellHeight(AxisDimension.auto(34))
                .setCharsSize(new Size(64, 4))
                .setCellCharsSize(new Size(68, 34))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(new Style())
        );
    }

    protected void onRawButtonClick()
    {
    }

    protected void onRawComboChange(String value)
    {
    }

    protected void onRawListSelectionChange(List<String> value)
    {
    }

    protected void onRawInputChange(String value)
    {
    }

    protected void onRawInputSubmit(String value)
    {
    }

    protected void onRawAreaChange(String value)
    {
    }

    protected void onRawAreaSubmit(String value)
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("raw_button".equals(name))
        {
            onRawButtonClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("raw_input".equals(name))
        {
            onRawInputChange(value);
            return true;
        }
        if ("raw_area".equals(name))
        {
            onRawAreaChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("raw_input".equals(name))
        {
            onRawInputSubmit(value);
            return true;
        }
        if ("raw_area".equals(name))
        {
            onRawAreaSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("raw_combo".equals(name))
        {
            onRawComboChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        if ("raw_list".equals(name))
        {
            onRawListSelectionChange(value);
            return true;
        }
        return false;
    }
}
