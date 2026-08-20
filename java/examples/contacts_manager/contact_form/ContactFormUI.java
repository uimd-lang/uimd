// Auto-generated UI code for contact_form - DO NOT EDIT MANUALLY.
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
import uimd.TextArea;
import uimd.TextGradient;
import uimd.TextInput;

public class ContactFormUI extends GeneratedWindowBase
{
    public Label header;
    public Label first_label;
    public TextInput first_name;
    public Label last_label;
    public TextInput last_name;
    public Label email_label;
    public TextInput email;
    public Label phone_label;
    public TextInput phone;
    public Label company_label;
    public TextInput company;
    public Label city_label;
    public TextInput city;
    public Label category_label;
    public ComboBox category;
    public CheckBox active;
    public Label notes_label;
    public TextArea notes;
    public Button save_btn;
    public Button cancel_btn;
    public Label message;

    public ContactFormUI()
    {
        super("Contact Form");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "ContactForm",
            "# Contact Form\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Contact\n  description: \"Dialog title showing that a contact is being edited.\"\n  expose: false\n\nfirst_label:\n  type: label\n  text: \"First name\"\n  description: \"Label for the first name input.\"\n\nfirst_name:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Required first name field.\"\n\nlast_label:\n  type: label\n  text: \"Last name\"\n  description: \"Label for the last name input.\"\n\nlast_name:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Required last name field.\"\n\nemail_label:\n  type: label\n  text: Email\n  description: \"Label for the email input.\"\n\nemail:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Required email address field.\"\n\nphone_label:\n  type: label\n  text: Phone\n  description: \"Label for the phone input.\"\n\nphone:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Phone number field.\"\n\ncompany_label:\n  type: label\n  text: Company\n  description: \"Label for the company input.\"\n\ncompany:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Company name field.\"\n\ncity_label:\n  type: label\n  text: City\n  description: \"Label for the city input.\"\n\ncity:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"City field.\"\n\ncategory_label:\n  type: label\n  text: Category\n  description: \"Label for the contact category selector.\"\n\ncategory:\n  type: combobox\n  options: [Client, Vendor, Partner, Personal]\n  selected_item: Client\n  description: \"Contact category combobox with client, vendor, partner, and personal options.\"\n\nactive:\n  type: checkbox\n  title: \"Active contact\"\n  value: 1\n  description: \"Toggle whether the contact is active.\"\n\nnotes_label:\n  type: label\n  text: Notes\n  description: \"Label for the notes field.\"\n\nnotes:\n  type: textarea\n  value: \"\"\n  maxlength: 300\n  description: \"Multiline notes field for the contact.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Validate and save the contact form.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Close the form without saving changes.\"\n\nmessage:\n  type: label\n  text: \"Required: first name, last name, email.\"\n  description: \"Validation and status message area.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@message:\n  color: \"#fbbf24\"\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------------+\n|header....................................................|\n+----------------------------------------------------------+\n|                                                          |\n|  first_label    first_name.............................  |\n|                                                          |\n|  last_label     last_name..............................  |\n|                                                          |\n|  email_label    email..................................  |\n|                                                          |\n|  phone_label    phone..................................  |\n|                                                          |\n|  company_label  company................................  |\n|                                                          |\n|  city_label     city...................................  |\n|                                                          |\n|  category_label  category..............................  |\n|                                                          |\n|  active................................................  |\n|                                                          |\n|  notes_label notes.....................................  |\n|              ..........................................  |\n|              ..........................................  |\n|                                                          |\n|  message...............................................  |\n|                                                          |\n+----------------------------------------------------------+\n|  save_btn..................  cancel_btn................  |\n|                                                          |\n+----------------------------------------------------------+\n```\n",
            "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.",
            List.of(
            new GeneratedElementMetadata("header", "Dialog title showing that a contact is being edited.", false),
            new GeneratedElementMetadata("first_label", "Label for the first name input.", true),
            new GeneratedElementMetadata("first_name", "Required first name field.", true),
            new GeneratedElementMetadata("last_label", "Label for the last name input.", true),
            new GeneratedElementMetadata("last_name", "Required last name field.", true),
            new GeneratedElementMetadata("email_label", "Label for the email input.", true),
            new GeneratedElementMetadata("email", "Required email address field.", true),
            new GeneratedElementMetadata("phone_label", "Label for the phone input.", true),
            new GeneratedElementMetadata("phone", "Phone number field.", true),
            new GeneratedElementMetadata("company_label", "Label for the company input.", true),
            new GeneratedElementMetadata("company", "Company name field.", true),
            new GeneratedElementMetadata("city_label", "Label for the city input.", true),
            new GeneratedElementMetadata("city", "City field.", true),
            new GeneratedElementMetadata("category_label", "Label for the contact category selector.", true),
            new GeneratedElementMetadata("category", "Contact category combobox with client, vendor, partner, and personal options.", true),
            new GeneratedElementMetadata("active", "Toggle whether the contact is active.", true),
            new GeneratedElementMetadata("notes_label", "Label for the notes field.", true),
            new GeneratedElementMetadata("notes", "Multiline notes field for the contact.", true),
            new GeneratedElementMetadata("save_btn", "Validate and save the contact form.", true),
            new GeneratedElementMetadata("cancel_btn", "Close the form without saving changes.", true),
            new GeneratedElementMetadata("message", "Validation and status message area.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header = addElement(new Label("header", "Contact"));
        header.setStyle(Style.fromProperties("background", "#1f2937", "color", "#ffffff", "text-align", "center"));
        header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        first_label = addElement(new Label("first_label", "First name"));
        first_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        first_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        first_name = addElement(new TextInput("first_name", "", 80));
        first_name.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        first_name.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        first_name.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        first_name.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        first_name.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        first_name.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        last_label = addElement(new Label("last_label", "Last name"));
        last_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        last_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        last_name = addElement(new TextInput("last_name", "", 80));
        last_name.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        last_name.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        last_name.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        last_name.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        last_name.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        last_name.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        email_label = addElement(new Label("email_label", "Email"));
        email_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        email_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        email = addElement(new TextInput("email", "", 120));
        email.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        email.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        email.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        email.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        email.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        email.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        phone_label = addElement(new Label("phone_label", "Phone"));
        phone_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        phone_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        phone = addElement(new TextInput("phone", "", 80));
        phone.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        phone.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        phone.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        phone.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        phone.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        phone.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        company_label = addElement(new Label("company_label", "Company"));
        company_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        company_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        company = addElement(new TextInput("company", "", 120));
        company.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        company.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        company.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        company.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        company.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        company.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        city_label = addElement(new Label("city_label", "City"));
        city_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        city_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        city = addElement(new TextInput("city", "", 80));
        city.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        city.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        city.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        city.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        city.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        city.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        category_label = addElement(new Label("category_label", "Category"));
        category_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        category_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        category = addElement(new ComboBox("category", List.of("Client", "Vendor", "Partner", "Personal")));
        category.setSelectedIndex(0);
        category.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        category.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        category.setEditStyle(Style.fromProperties("background", "#111827", "color", "#e5e7eb"));
        category.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        category.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        active = addElement(new CheckBox("active", "Active contact", true));
        active.setStyle(Style.fromProperties("background", "transparent", "color", "#b8c4d8"));
        active.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#ffffff"));
        active.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#44cc88"));
        active.setUncheckedStyle(Style.fromProperties("background", "transparent"));
        active.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        notes_label = addElement(new Label("notes_label", "Notes"));
        notes_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        notes_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        notes = addElement(new TextArea("notes", "", 300));
        notes.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        notes.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        notes.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        notes.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        notes.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        notes.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        save_btn = addElement(new Button("save_btn", "Save"));
        save_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        save_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        save_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        cancel_btn = addElement(new Button("cancel_btn", "Cancel"));
        cancel_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        cancel_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        cancel_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        message = addElement(new Label("message", "Required: first name, last name, email."));
        message.setStyle(Style.fromProperties("background", "transparent", "color", "#fbbf24"));
        message.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 58, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(58, 1))
                .setCellCharsSize(new Size(58, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#1f2937", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("save_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(0, 2, 26, 1))
                .setSourceCell(new Rect(26, 0, 58, 2))
                .setWidth(AxisDimension.auto(26))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(58, 2))
                .setMarginRight(30)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("cancel_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(0, 30, 536870911, 1))
                .setSourceCell(new Rect(26, 0, 58, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(26, 1))
                .setCellCharsSize(new Size(58, 2))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("first_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(1, 2, 11, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(45)
                .setMarginBottom(21)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("first_name")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(1, 17, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(39, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(21)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("last_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(3, 2, 10, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.auto(10))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(46)
                .setMarginBottom(19)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("last_name")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(3, 17, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(39, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(19)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("email_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(5, 2, 11, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(45)
                .setMarginBottom(17)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("email")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(5, 17, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(39, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(17)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("phone_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(7, 2, 11, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(45)
                .setMarginBottom(15)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("phone")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(7, 17, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(39, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(15)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("company_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(9, 2, 13, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.auto(13))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(43)
                .setMarginBottom(13)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("company")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(9, 17, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(39, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(13)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("city_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(11, 2, 10, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.auto(10))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(46)
                .setMarginBottom(11)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("city")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(11, 17, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(39, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(11)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("category_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(13, 2, 14, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.auto(14))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(42)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("category")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(13, 18, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(38, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("active")
                .setType("checkbox")
                .setCellName("")
                .setRelative(new Rect(15, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(54, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#b8c4d8")),
            new GeneratedLayoutEntry()
                .setName("notes_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(17, 2, 11, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(45)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("notes")
                .setType("textarea")
                .setCellName("")
                .setRelative(new Rect(17, 14, 536870911, 3))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(42, 3))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("message")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(21, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 58, 23))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(58))
                .setCellHeight(AxisDimension.auto(23))
                .setCharsSize(new Size(54, 1))
                .setCellCharsSize(new Size(58, 23))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#fbbf24"))
        );
    }

    protected void onFirstNameChange(String value)
    {
    }

    protected void onFirstNameSubmit(String value)
    {
    }

    protected void onLastNameChange(String value)
    {
    }

    protected void onLastNameSubmit(String value)
    {
    }

    protected void onEmailChange(String value)
    {
    }

    protected void onEmailSubmit(String value)
    {
    }

    protected void onPhoneChange(String value)
    {
    }

    protected void onPhoneSubmit(String value)
    {
    }

    protected void onCompanyChange(String value)
    {
    }

    protected void onCompanySubmit(String value)
    {
    }

    protected void onCityChange(String value)
    {
    }

    protected void onCitySubmit(String value)
    {
    }

    protected void onCategoryChange(String value)
    {
    }

    protected void onActiveChange(String value)
    {
    }

    protected void onNotesChange(String value)
    {
    }

    protected void onNotesSubmit(String value)
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
        if ("first_name".equals(name))
        {
            onFirstNameChange(value);
            return true;
        }
        if ("last_name".equals(name))
        {
            onLastNameChange(value);
            return true;
        }
        if ("email".equals(name))
        {
            onEmailChange(value);
            return true;
        }
        if ("phone".equals(name))
        {
            onPhoneChange(value);
            return true;
        }
        if ("company".equals(name))
        {
            onCompanyChange(value);
            return true;
        }
        if ("city".equals(name))
        {
            onCityChange(value);
            return true;
        }
        if ("active".equals(name))
        {
            onActiveChange(value);
            return true;
        }
        if ("notes".equals(name))
        {
            onNotesChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("first_name".equals(name))
        {
            onFirstNameSubmit(value);
            return true;
        }
        if ("last_name".equals(name))
        {
            onLastNameSubmit(value);
            return true;
        }
        if ("email".equals(name))
        {
            onEmailSubmit(value);
            return true;
        }
        if ("phone".equals(name))
        {
            onPhoneSubmit(value);
            return true;
        }
        if ("company".equals(name))
        {
            onCompanySubmit(value);
            return true;
        }
        if ("city".equals(name))
        {
            onCitySubmit(value);
            return true;
        }
        if ("notes".equals(name))
        {
            onNotesSubmit(value);
            return true;
        }
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
