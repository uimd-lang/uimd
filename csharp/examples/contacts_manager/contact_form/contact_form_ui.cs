// Auto-generated UI code for contact_form - DO NOT EDIT MANUALLY.
using Uimd;

public class ContactFormUI : GeneratedWindowBase
{
    public Label header = null!;
    public Label first_label = null!;
    public TextInput first_name = null!;
    public Label last_label = null!;
    public TextInput last_name = null!;
    public Label email_label = null!;
    public TextInput email = null!;
    public Label phone_label = null!;
    public TextInput phone = null!;
    public Label company_label = null!;
    public TextInput company = null!;
    public Label city_label = null!;
    public TextInput city = null!;
    public Label category_label = null!;
    public ComboBox category = null!;
    public CheckBox active = null!;
    public Label notes_label = null!;
    public TextArea notes = null!;
    public Button save_btn = null!;
    public Button cancel_btn = null!;
    public Label message = null!;

    public ContactFormUI() : base("Contact Form")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "ContactForm", """
# Contact Form

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback."
tags: [example]
```

## Members

```yaml
header:
  type: label
  text: Contact
  description: "Dialog title showing that a contact is being edited."
  expose: false

first_label:
  type: label
  text: "First name"
  description: "Label for the first name input."

first_name:
  type: textinput
  value: ""
  maxlength: 80
  description: "Required first name field."

last_label:
  type: label
  text: "Last name"
  description: "Label for the last name input."

last_name:
  type: textinput
  value: ""
  maxlength: 80
  description: "Required last name field."

email_label:
  type: label
  text: Email
  description: "Label for the email input."

email:
  type: textinput
  value: ""
  maxlength: 120
  description: "Required email address field."

phone_label:
  type: label
  text: Phone
  description: "Label for the phone input."

phone:
  type: textinput
  value: ""
  maxlength: 80
  description: "Phone number field."

company_label:
  type: label
  text: Company
  description: "Label for the company input."

company:
  type: textinput
  value: ""
  maxlength: 120
  description: "Company name field."

city_label:
  type: label
  text: City
  description: "Label for the city input."

city:
  type: textinput
  value: ""
  maxlength: 80
  description: "City field."

category_label:
  type: label
  text: Category
  description: "Label for the contact category selector."

category:
  type: combobox
  options: [Client, Vendor, Partner, Personal]
  selected_item: Client
  description: "Contact category combobox with client, vendor, partner, and personal options."

active:
  type: checkbox
  title: "Active contact"
  value: 1
  description: "Toggle whether the contact is active."

notes_label:
  type: label
  text: Notes
  description: "Label for the notes field."

notes:
  type: textarea
  value: ""
  maxlength: 300
  description: "Multiline notes field for the contact."

save_btn:
  type: button
  title: Save
  description: "Validate and save the contact form."

cancel_btn:
  type: button
  title: Cancel
  description: "Close the form without saving changes."

message:
  type: label
  text: "Required: first name, last name, email."
  description: "Validation and status message area."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  text-align: center
  background: "#1f2937"
  color: "#ffffff"
textinput:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  cursor-background: "#facc15"
  cursor-color: "#111827"
textarea:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  cursor-background: "#facc15"
  cursor-color: "#111827"
combobox:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@message:
  color: "#fbbf24"
```

## User Interface

```ui
+----------------------------------------------------------+
|header....................................................|
+----------------------------------------------------------+
|                                                          |
|  first_label    first_name.............................  |
|                                                          |
|  last_label     last_name..............................  |
|                                                          |
|  email_label    email..................................  |
|                                                          |
|  phone_label    phone..................................  |
|                                                          |
|  company_label  company................................  |
|                                                          |
|  city_label     city...................................  |
|                                                          |
|  category_label  category..............................  |
|                                                          |
|  active................................................  |
|                                                          |
|  notes_label notes.....................................  |
|              ..........................................  |
|              ..........................................  |
|                                                          |
|  message...............................................  |
|                                                          |
+----------------------------------------------------------+
|  save_btn..................  cancel_btn................  |
|                                                          |
+----------------------------------------------------------+
```

""", "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.", new[]
        {
            new GeneratedElementMetadata { Name = "header", Description = "Dialog title showing that a contact is being edited.", Expose = false },
            new GeneratedElementMetadata { Name = "first_label", Description = "Label for the first name input.", Expose = true },
            new GeneratedElementMetadata { Name = "first_name", Description = "Required first name field.", Expose = true },
            new GeneratedElementMetadata { Name = "last_label", Description = "Label for the last name input.", Expose = true },
            new GeneratedElementMetadata { Name = "last_name", Description = "Required last name field.", Expose = true },
            new GeneratedElementMetadata { Name = "email_label", Description = "Label for the email input.", Expose = true },
            new GeneratedElementMetadata { Name = "email", Description = "Required email address field.", Expose = true },
            new GeneratedElementMetadata { Name = "phone_label", Description = "Label for the phone input.", Expose = true },
            new GeneratedElementMetadata { Name = "phone", Description = "Phone number field.", Expose = true },
            new GeneratedElementMetadata { Name = "company_label", Description = "Label for the company input.", Expose = true },
            new GeneratedElementMetadata { Name = "company", Description = "Company name field.", Expose = true },
            new GeneratedElementMetadata { Name = "city_label", Description = "Label for the city input.", Expose = true },
            new GeneratedElementMetadata { Name = "city", Description = "City field.", Expose = true },
            new GeneratedElementMetadata { Name = "category_label", Description = "Label for the contact category selector.", Expose = true },
            new GeneratedElementMetadata { Name = "category", Description = "Contact category combobox with client, vendor, partner, and personal options.", Expose = true },
            new GeneratedElementMetadata { Name = "active", Description = "Toggle whether the contact is active.", Expose = true },
            new GeneratedElementMetadata { Name = "notes_label", Description = "Label for the notes field.", Expose = true },
            new GeneratedElementMetadata { Name = "notes", Description = "Multiline notes field for the contact.", Expose = true },
            new GeneratedElementMetadata { Name = "save_btn", Description = "Validate and save the contact form.", Expose = true },
            new GeneratedElementMetadata { Name = "cancel_btn", Description = "Close the form without saving changes.", Expose = true },
            new GeneratedElementMetadata { Name = "message", Description = "Validation and status message area.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header = AddElement(new Label("header", "Contact"));
        header.SetStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        header.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        first_label = AddElement(new Label("first_label", "First name"));
        first_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        first_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        first_name = AddElement(new TextInput("first_name", "", 80));
        first_name.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        first_name.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        first_name.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        first_name.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        first_name.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        first_name.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        last_label = AddElement(new Label("last_label", "Last name"));
        last_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        last_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        last_name = AddElement(new TextInput("last_name", "", 80));
        last_name.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        last_name.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        last_name.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        last_name.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        last_name.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        last_name.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        email_label = AddElement(new Label("email_label", "Email"));
        email_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        email_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        email = AddElement(new TextInput("email", "", 120));
        email.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        email.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        email.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        email.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        email.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        email.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        phone_label = AddElement(new Label("phone_label", "Phone"));
        phone_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        phone_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        phone = AddElement(new TextInput("phone", "", 80));
        phone.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        phone.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        phone.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        phone.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        phone.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        phone.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        company_label = AddElement(new Label("company_label", "Company"));
        company_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        company_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        company = AddElement(new TextInput("company", "", 120));
        company.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        company.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        company.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        company.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        company.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        company.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        city_label = AddElement(new Label("city_label", "City"));
        city_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        city_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        city = AddElement(new TextInput("city", "", 80));
        city.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        city.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        city.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        city.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        city.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        city.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        category_label = AddElement(new Label("category_label", "Category"));
        category_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        category_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        category = AddElement(new ComboBox("category", new[] {"Client", "Vendor", "Partner", "Personal"}));
        category.SetSelectedIndex(0);
        category.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        category.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        category.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#e5e7eb")
    });
        category.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        category.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        active = AddElement(new CheckBox("active", "Active contact", true));
        active.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#b8c4d8")
    });
        active.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#ffffff")
    });
        active.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#44cc88")
    });
        active.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent")
    });
        active.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        notes_label = AddElement(new Label("notes_label", "Notes"));
        notes_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        notes_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        notes = AddElement(new TextArea("notes", "", 300));
        notes.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        notes.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        notes.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        notes.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        notes.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        notes.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        save_btn = AddElement(new Button("save_btn", "Save"));
        save_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        save_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        save_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        cancel_btn = AddElement(new Button("cancel_btn", "Cancel"));
        cancel_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        cancel_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        cancel_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        message = AddElement(new Label("message", "Required: first name, last name, email."));
        message.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#fbbf24")
    });
        message.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "header",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 58, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(58, 1),
                CellCharsSize = new Size(58, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "save_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(0, 2, 26, 1),
                SourceCell = new Rect(26, 0, 58, 2),
                Width = AxisDimension.Auto(26),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(58, 2),
                MarginRight = 30,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "cancel_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(0, 30, 536870911, 1),
                SourceCell = new Rect(26, 0, 58, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(58, 2),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "first_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(1, 2, 11, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 45,
                MarginBottom = 21,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "first_name",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(1, 17, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(39, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 21,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "last_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(3, 2, 10, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Auto(10),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 46,
                MarginBottom = 19,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "last_name",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(3, 17, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(39, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 19,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "email_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(5, 2, 11, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 45,
                MarginBottom = 17,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "email",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(5, 17, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(39, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 17,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "phone_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(7, 2, 11, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 45,
                MarginBottom = 15,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "phone",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(7, 17, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(39, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 15,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "company_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(9, 2, 13, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Auto(13),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(13, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 43,
                MarginBottom = 13,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "company",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(9, 17, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(39, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 13,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "city_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(11, 2, 10, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Auto(10),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 46,
                MarginBottom = 11,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "city",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(11, 17, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(39, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 11,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "category_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(13, 2, 14, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Auto(14),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(14, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 42,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "category",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(13, 18, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(38, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "active",
                Type = "checkbox",
                CellName = "",
                Relative = new Rect(15, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(54, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 7,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#b8c4d8")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "notes_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(17, 2, 11, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 45,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "notes",
                Type = "textarea",
                CellName = "",
                Relative = new Rect(17, 14, 536870911, 3),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(42, 3),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "message",
                Type = "label",
                CellName = "",
                Relative = new Rect(21, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 58, 23),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(58),
                CellHeight = AxisDimension.Auto(23),
                CharsSize = new Size(54, 1),
                CellCharsSize = new Size(58, 23),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#fbbf24")
    },
            },
        };
    }

    protected virtual void onFirstNameChange(string value) { }
    protected virtual void onFirstNameSubmit(string value) { }
    protected virtual void onLastNameChange(string value) { }
    protected virtual void onLastNameSubmit(string value) { }
    protected virtual void onEmailChange(string value) { }
    protected virtual void onEmailSubmit(string value) { }
    protected virtual void onPhoneChange(string value) { }
    protected virtual void onPhoneSubmit(string value) { }
    protected virtual void onCompanyChange(string value) { }
    protected virtual void onCompanySubmit(string value) { }
    protected virtual void onCityChange(string value) { }
    protected virtual void onCitySubmit(string value) { }
    protected virtual void onCategoryChange(string value) { }
    protected virtual void onActiveChange(string value) { }
    protected virtual void onNotesChange(string value) { }
    protected virtual void onNotesSubmit(string value) { }
    protected virtual void onSaveBtnClick() { }
    protected virtual void onCancelBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "save_btn")
        {
            onSaveBtnClick();
            return true;
        }
        else if (name == "cancel_btn")
        {
            onCancelBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "first_name")
        {
            onFirstNameChange(value);
            return true;
        }
        else if (name == "last_name")
        {
            onLastNameChange(value);
            return true;
        }
        else if (name == "email")
        {
            onEmailChange(value);
            return true;
        }
        else if (name == "phone")
        {
            onPhoneChange(value);
            return true;
        }
        else if (name == "company")
        {
            onCompanyChange(value);
            return true;
        }
        else if (name == "city")
        {
            onCityChange(value);
            return true;
        }
        else if (name == "active")
        {
            onActiveChange(value);
            return true;
        }
        else if (name == "notes")
        {
            onNotesChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "first_name")
        {
            onFirstNameSubmit(value);
            return true;
        }
        else if (name == "last_name")
        {
            onLastNameSubmit(value);
            return true;
        }
        else if (name == "email")
        {
            onEmailSubmit(value);
            return true;
        }
        else if (name == "phone")
        {
            onPhoneSubmit(value);
            return true;
        }
        else if (name == "company")
        {
            onCompanySubmit(value);
            return true;
        }
        else if (name == "city")
        {
            onCitySubmit(value);
            return true;
        }
        else if (name == "notes")
        {
            onNotesSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "category")
        {
            onCategoryChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}
