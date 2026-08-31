// Auto-generated UI code for formular - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type FormularUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    TitleLabel *uimd.Label
    NameLabel *uimd.Label
    NameInput *uimd.TextInput
    EmailLabel *uimd.Label
    EmailInput *uimd.TextInput
    AgeLabel *uimd.Label
    AgeInput *uimd.NumberInput
    DescriptionInput *uimd.TextArea
    CountryLabel *uimd.Label
    CountryCombo *uimd.ComboBox
    RoleLabel *uimd.Label
    RoleListbox *uimd.ListBox
    NotifyCheck *uimd.CheckBox
    TermsCheck *uimd.CheckBox
    SaveBtn *uimd.Button
    CancelBtn *uimd.Button
}

func NewFormularUI() *FormularUI {
    ui := &FormularUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("FormApp")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 1, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 48, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 24,
            Content: "title_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextAlign: "center",
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 10, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 40, MarginBottom: 22,
            Content: "name_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 3, Col: 19,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 22,
            Content: "name_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 5, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 39, MarginBottom: 20,
            Content: "email_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 5, Col: 19,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 20,
            Content: "email_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 7, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 9, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 9, CharsHeight: 1,
            MarginRight: 41, MarginBottom: 18,
            Content: "age_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 7, Col: 19,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 18,
            Content: "age_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 9, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 13, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 37, MarginBottom: 16,
            Content: "country_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 9, Col: 19,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 16,
            Content: "country_combo",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    },
        },
        {
            Row: 11, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 13, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 37, MarginBottom: 14,
            Content: "\"Description\"",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 11, Col: 19,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 4,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 4,
            MarginRight: 2, MarginBottom: 11,
            Content: "description_input",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 16, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 10, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 40, MarginBottom: 9,
            Content: "role_label",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 16, Col: 19,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 4,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 31, CharsHeight: 4,
            MarginRight: 2, MarginBottom: 6,
            Content: "role_listbox",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 21, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 48, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 4,
            Content: "notify_check",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#b8c4d8"),
    },
        },
        {
            Row: 22, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 48, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 3,
            Content: "terms_check",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#b8c4d8"),
    },
        },
        {
            Row: 24, Col: 7,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 14, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 31, MarginBottom: 1,
            Content: "save_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 24, Col: 27,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 52, CellCharsHeight: 26,
            CellName: "", CellWidth: 52, CellHeight: 26,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 17, CharsHeight: 1,
            MarginRight: 8, MarginBottom: 1,
            Content: "cancel_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
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
    ui.SetMCPMetadata(true, "Formular", "# FormApp\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\ntitle_label:\n  type: label\n  text: \"User profile\"\n  description: \"Title of the form.\"\n  expose: false\n\nname_label:\n  type: label\n  text: Name\n  description: \"Label for the name field.\"\n\nname_input:\n  type: textinput\n  value: \"\"\n  description: \"User name text field.\"\n\nemail_label:\n  type: label\n  text: Email\n  description: \"Label for the email field.\"\n\nemail_input:\n  type: textinput\n  value: \"\"\n  description: \"Email address text field.\"\n\nage_label:\n  type: label\n  text: Age\n  description: \"Label for the age field.\"\n\nage_input:\n  type: numberinput\n  description: \"Numeric age input.\"\n\ndescription_input:\n  type: textarea\n  description: \"Multiline profile description field.\"\n\ncountry_label:\n  type: label\n  text: Country\n  description: \"Label for the country selector.\"\n\ncountry_combo:\n  type: combobox\n  options: [Slovakia, \"Czech Republic\", Poland, Hungary, Austria]\n  selected_item: Slovakia\n  description: \"Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.\"\n\nrole_label:\n  type: label\n  text: Role\n  description: \"Label for the role selector.\"\n\nrole_listbox:\n  type: listbox\n  options: [Developer, Designer, Manager, QA, DevOps, Tester, Hacker]\n  selected_items: [Developer]\n  multiple: \"true\"\n  description: \"Multi-select role listbox for choosing one or more user roles.\"\n\nnotify_check:\n  type: checkbox\n  title: \"Email notifications\"\n  description: \"Toggle email notifications.\"\n\nterms_check:\n  type: checkbox\n  title: \"I accept the terms and conditions\"\n  description: \"Toggle acceptance of terms and conditions.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Submit the form and serialize the entered data.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Cancel the form action.\"\n```\n\n## Style\n\n```yaml\n@title_label:\n  text-align: center\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------+\n|                                                    |\n|  title_label.....................................  |\n|                                                    |\n|  name_label       name_input.....................  |\n|                                                    |\n|  email_label      email_input....................  |\n|                                                    |\n|  age_label        age_input......................  |\n|                                                    |\n|  country_label    country_combo..................  |\n|                                                    |\n|  \"Description\"    description_input..............  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  role_label       role_listbox...................  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  notify_check....................................  |\n|  terms_check.....................................  |\n|                                                    |\n|       save_btn......      cancel_btn.......        |\n|                                                    |\n+----------------------------------------------------+\n```\n", "User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.")
    ui.TitleLabel = uimd.NewLabel("title_label", "User profile")
    ui.AddElement(ui.TitleLabel)
    ui.TitleLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
        TextAlign: "center",
    })
    ui.TitleLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.NameLabel = uimd.NewLabel("name_label", "Name")
    ui.AddElement(ui.NameLabel)
    ui.NameLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.NameLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.NameInput = uimd.NewTextInput("name_input", "", 0)
    ui.AddElement(ui.NameInput)
    ui.NameInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.NameInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NameInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NameInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff"),
        Color: uimd.NewColor("#0e1117"),
    })
    ui.NameInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NameInput.SetDisabledStyle(uimd.Style{
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
    ui.EmailInput = uimd.NewTextInput("email_input", "", 0)
    ui.AddElement(ui.EmailInput)
    ui.EmailInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.EmailInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.EmailInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.EmailInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff"),
        Color: uimd.NewColor("#0e1117"),
    })
    ui.EmailInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.EmailInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.AgeLabel = uimd.NewLabel("age_label", "Age")
    ui.AddElement(ui.AgeLabel)
    ui.AgeLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.AgeLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.AgeInput = uimd.NewNumberInput("age_input", 0.000000)
    ui.AddElement(ui.AgeInput)
    ui.AgeInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.AgeInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AgeInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AgeInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff"),
        Color: uimd.NewColor("#0e1117"),
    })
    ui.AgeInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.DescriptionInput = uimd.NewTextArea("description_input", "")
    ui.AddElement(ui.DescriptionInput)
    ui.DescriptionInput.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.DescriptionInput.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DescriptionInput.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DescriptionInput.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff"),
        Color: uimd.NewColor("#0e1117"),
    })
    ui.DescriptionInput.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DescriptionInput.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CountryLabel = uimd.NewLabel("country_label", "Country")
    ui.AddElement(ui.CountryLabel)
    ui.CountryLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.CountryLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.CountryCombo = uimd.NewComboBox("country_combo", []string{"Slovakia", "Czech Republic", "Poland", "Hungary", "Austria"})
    ui.AddElement(ui.CountryCombo)
    ui.CountryCombo.SetSelectedIndex(0)
    ui.CountryCombo.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.CountryCombo.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CountryCombo.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#2d456d"),
        Color: uimd.NewColor("#e5e7eb"),
    })
    ui.CountryCombo.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CountryCombo.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#0b1220"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.RoleLabel = uimd.NewLabel("role_label", "Role")
    ui.AddElement(ui.RoleLabel)
    ui.RoleLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.RoleLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.RoleListbox = uimd.NewListBox("role_listbox", []string{"Developer", "Designer", "Manager", "QA", "DevOps", "Tester", "Hacker"}, true)
    ui.AddElement(ui.RoleListbox)
    ui.RoleListbox.SetSelectedIndex(0)
    ui.RoleListbox.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0d1524"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.RoleListbox.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#1d2f4d"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RoleListbox.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#243a5c"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.RoleListbox.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.RoleListbox.SetActiveStyle(uimd.Style{
        Background: uimd.NewColor("#DDDDDD99"),
        Color: uimd.NewColor("#FFFFFF"),
    })
    ui.RoleListbox.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.NotifyCheck = uimd.NewCheckBox("notify_check", "Email notifications", false)
    ui.AddElement(ui.NotifyCheck)
    ui.NotifyCheck.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#b8c4d8"),
    })
    ui.NotifyCheck.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NotifyCheck.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#44cc88"),
    })
    ui.NotifyCheck.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
    })
    ui.NotifyCheck.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.TermsCheck = uimd.NewCheckBox("terms_check", "I accept the terms and conditions", false)
    ui.AddElement(ui.TermsCheck)
    ui.TermsCheck.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#b8c4d8"),
    })
    ui.TermsCheck.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff44"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.TermsCheck.SetCheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#44cc88"),
    })
    ui.TermsCheck.SetUncheckedStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
    })
    ui.TermsCheck.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.SaveBtn = uimd.NewButton("save_btn", "Save")
    ui.AddElement(ui.SaveBtn)
    ui.SaveBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SaveBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
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
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CancelBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *FormularUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *FormularUI) HandleGeneratedButton(name string) bool {
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

func (ui *FormularUI) HandleGeneratedTextChanged(name string, value string) bool {
    if name == "name_input" {
        if handler, ok := ui.eventHandler.(interface{ OnNameInputChange(string) }); ok {
            handler.OnNameInputChange(value)
            return true
        }
        return false
    }
    if name == "email_input" {
        if handler, ok := ui.eventHandler.(interface{ OnEmailInputChange(string) }); ok {
            handler.OnEmailInputChange(value)
            return true
        }
        return false
    }
    if name == "age_input" {
        if handler, ok := ui.eventHandler.(interface{ OnAgeInputChange(string) }); ok {
            handler.OnAgeInputChange(value)
            return true
        }
        return false
    }
    if name == "description_input" {
        if handler, ok := ui.eventHandler.(interface{ OnDescriptionInputChange(string) }); ok {
            handler.OnDescriptionInputChange(value)
            return true
        }
        return false
    }
    if name == "notify_check" {
        if handler, ok := ui.eventHandler.(interface{ OnNotifyCheckChange(string) }); ok {
            handler.OnNotifyCheckChange(value)
            return true
        }
        return false
    }
    if name == "terms_check" {
        if handler, ok := ui.eventHandler.(interface{ OnTermsCheckChange(string) }); ok {
            handler.OnTermsCheckChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *FormularUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    if name == "name_input" {
        if handler, ok := ui.eventHandler.(interface{ OnNameInputSubmit(string) }); ok {
            handler.OnNameInputSubmit(value)
            return true
        }
        return false
    }
    if name == "email_input" {
        if handler, ok := ui.eventHandler.(interface{ OnEmailInputSubmit(string) }); ok {
            handler.OnEmailInputSubmit(value)
            return true
        }
        return false
    }
    if name == "age_input" {
        if handler, ok := ui.eventHandler.(interface{ OnAgeInputSubmit(string) }); ok {
            handler.OnAgeInputSubmit(value)
            return true
        }
        return false
    }
    if name == "description_input" {
        if handler, ok := ui.eventHandler.(interface{ OnDescriptionInputSubmit(string) }); ok {
            handler.OnDescriptionInputSubmit(value)
            return true
        }
        return false
    }
    return false
}

func (ui *FormularUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "country_combo" {
        if handler, ok := ui.eventHandler.(interface{ OnCountryComboChange(string) }); ok {
            selected := ""
            if len(value) > 0 {
                selected = value[0]
            }
            handler.OnCountryComboChange(selected)
            return true
        }
        return false
    }
    if name == "role_listbox" {
        if handler, ok := ui.eventHandler.(interface{ OnRoleListboxSelectionChange([]string) }); ok {
            handler.OnRoleListboxSelectionChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *FormularUI) HandleGeneratedListBoxItemActivate(name string, elementID string, index int, value string) bool {
    _ = elementID
    if name == "role_listbox" {
        if handler, ok := ui.eventHandler.(interface{ OnRoleListboxItemActivate(int, string) bool }); ok {
            return handler.OnRoleListboxItemActivate(index, value)
        }
        return false
    }
    return false
}

func (ui *FormularUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *FormularUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
