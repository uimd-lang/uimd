// Auto-generated UI code for contact_form - DO NOT EDIT MANUALLY.
package contact_form

import uimd "uimd"

type ContactFormUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Header *uimd.Label
    FirstLabel *uimd.Label
    FirstName *uimd.TextInput
    LastLabel *uimd.Label
    LastName *uimd.TextInput
    EmailLabel *uimd.Label
    Email *uimd.TextInput
    PhoneLabel *uimd.Label
    Phone *uimd.TextInput
    CompanyLabel *uimd.Label
    Company *uimd.TextInput
    CityLabel *uimd.Label
    City *uimd.TextInput
    CategoryLabel *uimd.Label
    Category *uimd.ComboBox
    Active *uimd.CheckBox
    NotesLabel *uimd.Label
    Notes *uimd.TextArea
    SaveBtn *uimd.Button
    CancelBtn *uimd.Button
    Message *uimd.Label
}

func NewContactFormUI() *ContactFormUI {
    ui := &ContactFormUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Contact Form")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 1,
            CellName: "", CellWidth: 58, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 58, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "header",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 2,
            CellRow: 26, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 2,
            CellName: "", CellWidth: 58, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 26, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 30, MarginBottom: 1,
            Content: "save_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 30,
            CellRow: 26, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 2,
            CellName: "", CellWidth: 58, CellHeight: 2,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 26, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 1,
            Content: "cancel_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 45, MarginBottom: 21,
            Content: "first_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 17,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 21,
            Content: "first_name",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 10, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 46, MarginBottom: 19,
            Content: "last_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 3, Col: 17,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 19,
            Content: "last_name",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 5, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 45, MarginBottom: 17,
            Content: "email_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 5, Col: 17,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 17,
            Content: "email",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 7, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 45, MarginBottom: 15,
            Content: "phone_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 7, Col: 17,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 15,
            Content: "phone",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 9, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 13, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 43, MarginBottom: 13,
            Content: "company_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 9, Col: 17,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 13,
            Content: "company",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 11, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 10, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 46, MarginBottom: 11,
            Content: "city_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 11, Col: 17,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 39, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 11,
            Content: "city",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 13, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 14, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 42, MarginBottom: 9,
            Content: "category_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 13, Col: 18,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 38, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 9,
            Content: "category",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 15, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 54, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 7,
            Content: "active",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#b8c4d8"),
    },
        },
        {
            Row: 17, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 45, MarginBottom: 5,
            Content: "notes_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 17, Col: 14,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 3,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 42, CharsHeight: 3,
            MarginRight: 2, MarginBottom: 3,
            Content: "notes",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 21, Col: 2,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 58, CellCharsHeight: 23,
            CellName: "", CellWidth: 58, CellHeight: 23,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 54, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 1,
            Content: "message",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#fbbf24"),
    },
        },
    })
    ui.SetGeneratedKind("window")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ContactForm", "# Contact Form\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Contact\n  description: \"Dialog title showing that a contact is being edited.\"\n  expose: false\n\nfirst_label:\n  type: label\n  text: \"First name\"\n  description: \"Label for the first name input.\"\n\nfirst_name:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Required first name field.\"\n\nlast_label:\n  type: label\n  text: \"Last name\"\n  description: \"Label for the last name input.\"\n\nlast_name:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Required last name field.\"\n\nemail_label:\n  type: label\n  text: Email\n  description: \"Label for the email input.\"\n\nemail:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Required email address field.\"\n\nphone_label:\n  type: label\n  text: Phone\n  description: \"Label for the phone input.\"\n\nphone:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Phone number field.\"\n\ncompany_label:\n  type: label\n  text: Company\n  description: \"Label for the company input.\"\n\ncompany:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Company name field.\"\n\ncity_label:\n  type: label\n  text: City\n  description: \"Label for the city input.\"\n\ncity:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"City field.\"\n\ncategory_label:\n  type: label\n  text: Category\n  description: \"Label for the contact category selector.\"\n\ncategory:\n  type: combobox\n  options: [Client, Vendor, Partner, Personal]\n  selected_item: Client\n  description: \"Contact category combobox with client, vendor, partner, and personal options.\"\n\nactive:\n  type: checkbox\n  title: \"Active contact\"\n  value: 1\n  description: \"Toggle whether the contact is active.\"\n\nnotes_label:\n  type: label\n  text: Notes\n  description: \"Label for the notes field.\"\n\nnotes:\n  type: textarea\n  value: \"\"\n  maxlength: 300\n  description: \"Multiline notes field for the contact.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Validate and save the contact form.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Close the form without saving changes.\"\n\nmessage:\n  type: label\n  text: \"Required: first name, last name, email.\"\n  description: \"Validation and status message area.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@message:\n  color: \"#fbbf24\"\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------------+\n|header....................................................|\n+----------------------------------------------------------+\n|                                                          |\n|  first_label    first_name.............................  |\n|                                                          |\n|  last_label     last_name..............................  |\n|                                                          |\n|  email_label    email..................................  |\n|                                                          |\n|  phone_label    phone..................................  |\n|                                                          |\n|  company_label  company................................  |\n|                                                          |\n|  city_label     city...................................  |\n|                                                          |\n|  category_label  category..............................  |\n|                                                          |\n|  active................................................  |\n|                                                          |\n|  notes_label notes.....................................  |\n|              ..........................................  |\n|              ..........................................  |\n|                                                          |\n|  message...............................................  |\n|                                                          |\n+----------------------------------------------------------+\n|  save_btn..................  cancel_btn................  |\n|                                                          |\n+----------------------------------------------------------+\n```\n", "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.")
    ui.Header = uimd.NewLabel("header", "Contact")
    ui.AddElement(ui.Header)
    ui.Header.SetStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.Header.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.FirstLabel = uimd.NewLabel("first_label", "First name")
    ui.AddElement(ui.FirstLabel)
    ui.FirstLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.FirstLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.FirstName = uimd.NewTextInput("first_name", "", 0)
    ui.AddElement(ui.FirstName)
    ui.FirstName.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.FirstName.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.FirstName.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.FirstName.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.FirstName.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.FirstName.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.LastLabel = uimd.NewLabel("last_label", "Last name")
    ui.AddElement(ui.LastLabel)
    ui.LastLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.LastLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.LastName = uimd.NewTextInput("last_name", "", 0)
    ui.AddElement(ui.LastName)
    ui.LastName.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.LastName.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.LastName.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.LastName.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.LastName.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.LastName.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.EmailLabel = uimd.NewLabel("email_label", "Email")
    ui.AddElement(ui.EmailLabel)
    ui.EmailLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.EmailLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Email = uimd.NewTextInput("email", "", 0)
    ui.AddElement(ui.Email)
    ui.Email.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Email.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Email.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Email.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.Email.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Email.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.PhoneLabel = uimd.NewLabel("phone_label", "Phone")
    ui.AddElement(ui.PhoneLabel)
    ui.PhoneLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.PhoneLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Phone = uimd.NewTextInput("phone", "", 0)
    ui.AddElement(ui.Phone)
    ui.Phone.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Phone.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Phone.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Phone.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.Phone.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Phone.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CompanyLabel = uimd.NewLabel("company_label", "Company")
    ui.AddElement(ui.CompanyLabel)
    ui.CompanyLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.CompanyLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Company = uimd.NewTextInput("company", "", 0)
    ui.AddElement(ui.Company)
    ui.Company.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Company.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Company.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Company.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.Company.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Company.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CityLabel = uimd.NewLabel("city_label", "City")
    ui.AddElement(ui.CityLabel)
    ui.CityLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.CityLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.City = uimd.NewTextInput("city", "", 0)
    ui.AddElement(ui.City)
    ui.City.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.City.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.City.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.City.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.City.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.City.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CategoryLabel = uimd.NewLabel("category_label", "Category")
    ui.AddElement(ui.CategoryLabel)
    ui.CategoryLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.CategoryLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Category = uimd.NewComboBox("category", []string{"Client", "Vendor", "Partner", "Personal"})
    ui.AddElement(ui.Category)
    ui.Category.SetSelectedIndex(0)
    ui.Category.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Category.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Category.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Category.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Category.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Active = uimd.NewCheckBox("active", "Active contact", true)
    ui.AddElement(ui.Active)
    ui.Active.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#b8c4d8"),
    })
    ui.Active.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Active.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#44cc88"),
    })
    ui.Active.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
    })
    ui.Active.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.NotesLabel = uimd.NewLabel("notes_label", "Notes")
    ui.AddElement(ui.NotesLabel)
    ui.NotesLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.NotesLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Notes = uimd.NewTextArea("notes", "")
    ui.AddElement(ui.Notes)
    ui.Notes.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.Notes.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1e293b"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Notes.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Notes.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#facc15"),
        Color: uimd.NewColor("#111827"),
    })
    ui.Notes.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Notes.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SaveBtn = uimd.NewButton("save_btn", "Save")
    ui.AddElement(ui.SaveBtn)
    ui.SaveBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SaveBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SaveBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CancelBtn = uimd.NewButton("cancel_btn", "Cancel")
    ui.AddElement(ui.CancelBtn)
    ui.CancelBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.CancelBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CancelBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Message = uimd.NewLabel("message", "Required: first name, last name, email.")
    ui.AddElement(ui.Message)
    ui.Message.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#fbbf24"),
    })
    ui.Message.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *ContactFormUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ContactFormUI) HandleGeneratedButton(name string) bool {
    if name == "save_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnSaveBtnClick() }); ok {
            handler.OnSaveBtnClick()
            return true
        }
        return false
    }
    if name == "cancel_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnCancelBtnClick() }); ok {
            handler.OnCancelBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ContactFormUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "first_name" {
        if handler, ok := ui.eventHandler.(interface{ OnFirstNameChange(string) }); ok {
            handler.OnFirstNameChange(value)
            return true
        }
        return false
    }
    if name == "last_name" {
        if handler, ok := ui.eventHandler.(interface{ OnLastNameChange(string) }); ok {
            handler.OnLastNameChange(value)
            return true
        }
        return false
    }
    if name == "email" {
        if handler, ok := ui.eventHandler.(interface{ OnEmailChange(string) }); ok {
            handler.OnEmailChange(value)
            return true
        }
        return false
    }
    if name == "phone" {
        if handler, ok := ui.eventHandler.(interface{ OnPhoneChange(string) }); ok {
            handler.OnPhoneChange(value)
            return true
        }
        return false
    }
    if name == "company" {
        if handler, ok := ui.eventHandler.(interface{ OnCompanyChange(string) }); ok {
            handler.OnCompanyChange(value)
            return true
        }
        return false
    }
    if name == "city" {
        if handler, ok := ui.eventHandler.(interface{ OnCityChange(string) }); ok {
            handler.OnCityChange(value)
            return true
        }
        return false
    }
    if name == "active" {
        if handler, ok := ui.eventHandler.(interface{ OnActiveChange(string) }); ok {
            handler.OnActiveChange(value)
            return true
        }
        return false
    }
    if name == "notes" {
        if handler, ok := ui.eventHandler.(interface{ OnNotesChange(string) }); ok {
            handler.OnNotesChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ContactFormUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "first_name" {
        if handler, ok := ui.eventHandler.(interface{ OnFirstNameSubmit(string) }); ok {
            handler.OnFirstNameSubmit(value)
            return true
        }
        return false
    }
    if name == "last_name" {
        if handler, ok := ui.eventHandler.(interface{ OnLastNameSubmit(string) }); ok {
            handler.OnLastNameSubmit(value)
            return true
        }
        return false
    }
    if name == "email" {
        if handler, ok := ui.eventHandler.(interface{ OnEmailSubmit(string) }); ok {
            handler.OnEmailSubmit(value)
            return true
        }
        return false
    }
    if name == "phone" {
        if handler, ok := ui.eventHandler.(interface{ OnPhoneSubmit(string) }); ok {
            handler.OnPhoneSubmit(value)
            return true
        }
        return false
    }
    if name == "company" {
        if handler, ok := ui.eventHandler.(interface{ OnCompanySubmit(string) }); ok {
            handler.OnCompanySubmit(value)
            return true
        }
        return false
    }
    if name == "city" {
        if handler, ok := ui.eventHandler.(interface{ OnCitySubmit(string) }); ok {
            handler.OnCitySubmit(value)
            return true
        }
        return false
    }
    if name == "notes" {
        if handler, ok := ui.eventHandler.(interface{ OnNotesSubmit(string) }); ok {
            handler.OnNotesSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ContactFormUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "category" {
        if handler, ok := ui.eventHandler.(interface{ OnCategoryChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnCategoryChange(selected)
            return true
        }
        return false
    }
    return false
}

func (ui *ContactFormUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ContactFormUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
