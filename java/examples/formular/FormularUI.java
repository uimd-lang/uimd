// Auto-generated UI code for formular - DO NOT EDIT MANUALLY.
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
import uimd.Style;
import uimd.TextArea;
import uimd.TextGradient;
import uimd.TextInput;

public class FormularUI extends GeneratedWindowBase
{
    public Label title_label;
    public Label name_label;
    public TextInput name_input;
    public Label email_label;
    public TextInput email_input;
    public Label age_label;
    public NumberInput age_input;
    public TextArea description_input;
    public Label country_label;
    public ComboBox country_combo;
    public Label role_label;
    public ListBox role_listbox;
    public CheckBox notify_check;
    public CheckBox terms_check;
    public Button save_btn;
    public Button cancel_btn;

    public FormularUI()
    {
        super("FormApp");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "Formular",
            "# FormApp\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\ntitle_label:\n  type: label\n  text: \"User profile\"\n  description: \"Title of the form.\"\n  expose: false\n\nname_label:\n  type: label\n  text: Name\n  description: \"Label for the name field.\"\n\nname_input:\n  type: textinput\n  value: \"\"\n  description: \"User name text field.\"\n\nemail_label:\n  type: label\n  text: Email\n  description: \"Label for the email field.\"\n\nemail_input:\n  type: textinput\n  value: \"\"\n  description: \"Email address text field.\"\n\nage_label:\n  type: label\n  text: Age\n  description: \"Label for the age field.\"\n\nage_input:\n  type: numberinput\n  description: \"Numeric age input.\"\n\ndescription_input:\n  type: textarea\n  description: \"Multiline profile description field.\"\n\ncountry_label:\n  type: label\n  text: Country\n  description: \"Label for the country selector.\"\n\ncountry_combo:\n  type: combobox\n  options: [Slovakia, \"Czech Republic\", Poland, Hungary, Austria]\n  selected_item: Slovakia\n  description: \"Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.\"\n\nrole_label:\n  type: label\n  text: Role\n  description: \"Label for the role selector.\"\n\nrole_listbox:\n  type: listbox\n  options: [Developer, Designer, Manager, QA, DevOps, Tester, Hacker]\n  selected_items: [Developer]\n  multiple: \"true\"\n  description: \"Multi-select role listbox for choosing one or more user roles.\"\n\nnotify_check:\n  type: checkbox\n  title: \"Email notifications\"\n  description: \"Toggle email notifications.\"\n\nterms_check:\n  type: checkbox\n  title: \"I accept the terms and conditions\"\n  description: \"Toggle acceptance of terms and conditions.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Submit the form and serialize the entered data.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Cancel the form action.\"\n```\n\n## Style\n\n```yaml\n@title_label:\n  text-align: center\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------+\n|                                                    |\n|  title_label.....................................  |\n|                                                    |\n|  name_label       name_input.....................  |\n|                                                    |\n|  email_label      email_input....................  |\n|                                                    |\n|  age_label        age_input......................  |\n|                                                    |\n|  country_label    country_combo..................  |\n|                                                    |\n|  \"Description\"    description_input..............  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  role_label       role_listbox...................  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  notify_check....................................  |\n|  terms_check.....................................  |\n|                                                    |\n|       save_btn......      cancel_btn.......        |\n|                                                    |\n+----------------------------------------------------+\n```\n",
            "User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.",
            List.of(
            new GeneratedElementMetadata("title_label", "Title of the form.", false),
            new GeneratedElementMetadata("name_label", "Label for the name field.", true),
            new GeneratedElementMetadata("name_input", "User name text field.", true),
            new GeneratedElementMetadata("email_label", "Label for the email field.", true),
            new GeneratedElementMetadata("email_input", "Email address text field.", true),
            new GeneratedElementMetadata("age_label", "Label for the age field.", true),
            new GeneratedElementMetadata("age_input", "Numeric age input.", true),
            new GeneratedElementMetadata("description_input", "Multiline profile description field.", true),
            new GeneratedElementMetadata("country_label", "Label for the country selector.", true),
            new GeneratedElementMetadata("country_combo", "Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.", true),
            new GeneratedElementMetadata("role_label", "Label for the role selector.", true),
            new GeneratedElementMetadata("role_listbox", "Multi-select role listbox for choosing one or more user roles.", true),
            new GeneratedElementMetadata("notify_check", "Toggle email notifications.", true),
            new GeneratedElementMetadata("terms_check", "Toggle acceptance of terms and conditions.", true),
            new GeneratedElementMetadata("save_btn", "Submit the form and serialize the entered data.", true),
            new GeneratedElementMetadata("cancel_btn", "Cancel the form action.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        title_label = addElement(new Label("title_label", "User profile"));
        title_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1", "text-align", "center"));
        title_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        name_label = addElement(new Label("name_label", "Name"));
        name_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        name_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        name_input = addElement(new TextInput("name_input", "", 0));
        name_input.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        name_input.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        name_input.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        name_input.setCursorStyle(Style.fromProperties("background", "#ffffff", "color", "#0e1117"));
        name_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        name_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        email_label = addElement(new Label("email_label", "Email"));
        email_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        email_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        email_input = addElement(new TextInput("email_input", "", 0));
        email_input.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        email_input.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        email_input.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        email_input.setCursorStyle(Style.fromProperties("background", "#ffffff", "color", "#0e1117"));
        email_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        email_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        age_label = addElement(new Label("age_label", "Age"));
        age_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        age_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        age_input = addElement(new NumberInput("age_input", 0.0, 1.0));
        age_input.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        age_input.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        age_input.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        age_input.setCursorStyle(Style.fromProperties("background", "#ffffff", "color", "#0e1117"));
        age_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        description_input = addElement(new TextArea("description_input", "", 0));
        description_input.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        description_input.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        description_input.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        description_input.setCursorStyle(Style.fromProperties("background", "#ffffff", "color", "#0e1117"));
        description_input.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        description_input.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        country_label = addElement(new Label("country_label", "Country"));
        country_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        country_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        country_combo = addElement(new ComboBox("country_combo", List.of("Slovakia", "Czech Republic", "Poland", "Hungary", "Austria")));
        country_combo.setSelectedIndex(0);
        country_combo.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        country_combo.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        country_combo.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#e5e7eb"));
        country_combo.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        country_combo.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        role_label = addElement(new Label("role_label", "Role"));
        role_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        role_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        role_listbox = addElement(new ListBox("role_listbox", List.of("Developer", "Designer", "Manager", "QA", "DevOps", "Tester", "Hacker")));
        role_listbox.setMultiple(true);
        role_listbox.setSelectedValues(List.of("Developer"));
        role_listbox.setStyle(Style.fromProperties("background", "#0d1524", "color", "#cbd5e1"));
        role_listbox.setFocusStyle(Style.fromProperties("background", "#1d2f4d", "color", "#ffffff"));
        role_listbox.setEditStyle(Style.fromProperties("background", "#243a5c", "color", "#cbd5e1"));
        role_listbox.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        role_listbox.setActiveStyle(Style.fromProperties("background", "#DDDDDD99", "color", "#FFFFFF"));
        role_listbox.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        notify_check = addElement(new CheckBox("notify_check", "Email notifications", false));
        notify_check.setStyle(Style.fromProperties("background", "transparent", "color", "#b8c4d8"));
        notify_check.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#ffffff"));
        notify_check.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#44cc88"));
        notify_check.setUncheckedStyle(Style.fromProperties("background", "transparent"));
        notify_check.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        terms_check = addElement(new CheckBox("terms_check", "I accept the terms and conditions", false));
        terms_check.setStyle(Style.fromProperties("background", "transparent", "color", "#b8c4d8"));
        terms_check.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#ffffff"));
        terms_check.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#44cc88"));
        terms_check.setUncheckedStyle(Style.fromProperties("background", "transparent"));
        terms_check.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        save_btn = addElement(new Button("save_btn", "Save"));
        save_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        save_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        save_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        cancel_btn = addElement(new Button("cancel_btn", "Cancel"));
        cancel_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        cancel_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        cancel_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("title_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(1, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(48, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(24)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("name_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(3, 2, 10, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.auto(10))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(40)
                .setMarginBottom(22)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("name_input")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(3, 19, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(22)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("email_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(5, 2, 11, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(39)
                .setMarginBottom(20)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("email_input")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(5, 19, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(20)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("age_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(7, 2, 9, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(41)
                .setMarginBottom(18)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("age_input")
                .setType("numberinput")
                .setCellName("")
                .setRelative(new Rect(7, 19, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(18)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("country_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(9, 2, 13, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.auto(13))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(37)
                .setMarginBottom(16)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("country_combo")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(9, 19, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(16)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("\"Description\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(11, 2, 13, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.auto(13))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(37)
                .setMarginBottom(14)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("description_input")
                .setType("textarea")
                .setCellName("")
                .setRelative(new Rect(11, 19, 536870911, 4))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(4))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(31, 4))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(11)
                .setCellStyle(new Style())
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("role_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(16, 2, 10, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.auto(10))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(40)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("role_listbox")
                .setType("listbox")
                .setCellName("")
                .setRelative(new Rect(16, 19, 536870911, 4))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(4))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(31, 4))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(6)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("notify_check")
                .setType("checkbox")
                .setCellName("")
                .setRelative(new Rect(21, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(48, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(4)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#b8c4d8")),
            new GeneratedLayoutEntry()
                .setName("terms_check")
                .setType("checkbox")
                .setCellName("")
                .setRelative(new Rect(22, 2, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(48, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(2)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#b8c4d8")),
            new GeneratedLayoutEntry()
                .setName("save_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(24, 7, 14, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.auto(14))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(31)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("cancel_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(24, 27, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 52, 26))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(52))
                .setCellHeight(AxisDimension.auto(26))
                .setCharsSize(new Size(17, 1))
                .setCellCharsSize(new Size(52, 26))
                .setMarginRight(8)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onNameInputChange(String value)
    {
    }

    protected void onNameInputSubmit(String value)
    {
    }

    protected void onEmailInputChange(String value)
    {
    }

    protected void onEmailInputSubmit(String value)
    {
    }

    protected void onAgeInputChange(String value)
    {
    }

    protected void onAgeInputSubmit(String value)
    {
    }

    protected void onDescriptionInputChange(String value)
    {
    }

    protected void onDescriptionInputSubmit(String value)
    {
    }

    protected void onCountryComboChange(String value)
    {
    }

    protected void onRoleListboxSelectionChange(List<String> value)
    {
    }

    protected boolean onRoleListboxItemActivate(int index, String value)
    {
        return false;
    }

    protected void onNotifyCheckChange(String value)
    {
    }

    protected void onTermsCheckChange(String value)
    {
    }

    protected void onSaveBtnClick()
    {
    }

    protected void onCancelBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("save_btn".equals(name))
        {
            onSaveBtnClick();
            return true;
        }
        if ("cancel_btn".equals(name))
        {
            onCancelBtnClick();
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
        if ("email_input".equals(name))
        {
            onEmailInputChange(value);
            return true;
        }
        if ("age_input".equals(name))
        {
            onAgeInputChange(value);
            return true;
        }
        if ("description_input".equals(name))
        {
            onDescriptionInputChange(value);
            return true;
        }
        if ("notify_check".equals(name))
        {
            onNotifyCheckChange(value);
            return true;
        }
        if ("terms_check".equals(name))
        {
            onTermsCheckChange(value);
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
        if ("email_input".equals(name))
        {
            onEmailInputSubmit(value);
            return true;
        }
        if ("age_input".equals(name))
        {
            onAgeInputSubmit(value);
            return true;
        }
        if ("description_input".equals(name))
        {
            onDescriptionInputSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("country_combo".equals(name))
        {
            onCountryComboChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        if ("role_listbox".equals(name))
        {
            onRoleListboxSelectionChange(value);
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
        if ("role_listbox".equals(name))
        {
            return onRoleListboxItemActivate(index, value);
        }
        return false;
    }
}
