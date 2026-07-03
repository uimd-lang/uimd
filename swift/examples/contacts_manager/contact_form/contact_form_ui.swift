// Auto-generated UI code for contact_form - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ContactFormUI: GeneratedWindowBase
{
    public var header: Label!
    public var first_label: Label!
    public var first_name: TextInput!
    public var last_label: Label!
    public var last_name: TextInput!
    public var email_label: Label!
    public var email: TextInput!
    public var phone_label: Label!
    public var phone: TextInput!
    public var company_label: Label!
    public var company: TextInput!
    public var city_label: Label!
    public var city: TextInput!
    public var category_label: Label!
    public var category: ComboBox!
    public var active: CheckBox!
    public var notes_label: Label!
    public var notes: TextArea!
    public var save_btn: Button!
    public var cancel_btn: Button!
    public var message: Label!

    public init()
    {
        super.init("Contact Form")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ContactForm", source: "# Contact Form\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Contact\n  description: \"Dialog title showing that a contact is being edited.\"\n  expose: false\n\nfirst_label:\n  type: label\n  text: \"First name\"\n  description: \"Label for the first name input.\"\n\nfirst_name:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Required first name field.\"\n\nlast_label:\n  type: label\n  text: \"Last name\"\n  description: \"Label for the last name input.\"\n\nlast_name:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Required last name field.\"\n\nemail_label:\n  type: label\n  text: Email\n  description: \"Label for the email input.\"\n\nemail:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Required email address field.\"\n\nphone_label:\n  type: label\n  text: Phone\n  description: \"Label for the phone input.\"\n\nphone:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Phone number field.\"\n\ncompany_label:\n  type: label\n  text: Company\n  description: \"Label for the company input.\"\n\ncompany:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Company name field.\"\n\ncity_label:\n  type: label\n  text: City\n  description: \"Label for the city input.\"\n\ncity:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"City field.\"\n\ncategory_label:\n  type: label\n  text: Category\n  description: \"Label for the contact category selector.\"\n\ncategory:\n  type: combobox\n  options: [Client, Vendor, Partner, Personal]\n  selected_item: Client\n  description: \"Contact category combobox with client, vendor, partner, and personal options.\"\n\nactive:\n  type: checkbox\n  title: \"Active contact\"\n  value: 1\n  description: \"Toggle whether the contact is active.\"\n\nnotes_label:\n  type: label\n  text: Notes\n  description: \"Label for the notes field.\"\n\nnotes:\n  type: textarea\n  value: \"\"\n  maxlength: 300\n  description: \"Multiline notes field for the contact.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Validate and save the contact form.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Close the form without saving changes.\"\n\nmessage:\n  type: label\n  text: \"Required: first name, last name, email.\"\n  description: \"Validation and status message area.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@message:\n  color: \"#fbbf24\"\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------------+\n|header....................................................|\n+----------------------------------------------------------+\n|                                                          |\n|  first_label    first_name.............................  |\n|                                                          |\n|  last_label     last_name..............................  |\n|                                                          |\n|  email_label    email..................................  |\n|                                                          |\n|  phone_label    phone..................................  |\n|                                                          |\n|  company_label  company................................  |\n|                                                          |\n|  city_label     city...................................  |\n|                                                          |\n|  category_label  category..............................  |\n|                                                          |\n|  active................................................  |\n|                                                          |\n|  notes_label notes.....................................  |\n|              ..........................................  |\n|              ..........................................  |\n|                                                          |\n|  message...............................................  |\n|                                                          |\n+----------------------------------------------------------+\n|  save_btn..................  cancel_btn................  |\n|                                                          |\n+----------------------------------------------------------+\n```\n", description: "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header = addElement(Label("header", "Contact"))
        header.setStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        header.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        first_label = addElement(Label("first_label", "First name"))
        first_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        first_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        first_name = addElement(TextInput("first_name", "", maxLength: 80))
        first_name.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        first_name.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        first_name.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        first_name.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        first_name.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        first_name.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        last_label = addElement(Label("last_label", "Last name"))
        last_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        last_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        last_name = addElement(TextInput("last_name", "", maxLength: 80))
        last_name.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        last_name.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        last_name.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        last_name.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        last_name.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        last_name.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        email_label = addElement(Label("email_label", "Email"))
        email_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        email_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        email = addElement(TextInput("email", "", maxLength: 120))
        email.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        email.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        email.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        email.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        email.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        email.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        phone_label = addElement(Label("phone_label", "Phone"))
        phone_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        phone_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        phone = addElement(TextInput("phone", "", maxLength: 80))
        phone.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        phone.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        phone.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        phone.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        phone.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        phone.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        company_label = addElement(Label("company_label", "Company"))
        company_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        company_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        company = addElement(TextInput("company", "", maxLength: 120))
        company.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        company.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        company.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        company.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        company.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        company.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        city_label = addElement(Label("city_label", "City"))
        city_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        city_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        city = addElement(TextInput("city", "", maxLength: 80))
        city.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        city.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        city.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        city.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        city.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        city.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        category_label = addElement(Label("category_label", "Category"))
        category_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        category_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        category = addElement(ComboBox("category", ["Client", "Vendor", "Partner", "Personal"], selectedIndex: 0))
        category.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        category.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        category.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        category.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        category.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        active = addElement(CheckBox("active", "Active contact", true))
        active.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#b8c4d8")
                    return style
                }())
        active.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#ffffff")
                    return style
                }())
        active.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#44cc88")
                    return style
                }())
        active.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    return style
                }())
        active.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        notes_label = addElement(Label("notes_label", "Notes"))
        notes_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        notes_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        notes = addElement(TextArea("notes", "", maxLength: 300))
        notes.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        notes.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        notes.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        notes.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        notes.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        notes.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        save_btn = addElement(Button("save_btn", "Save"))
        save_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        save_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        save_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        cancel_btn = addElement(Button("cancel_btn", "Cancel"))
        cancel_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        cancel_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        cancel_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        message = addElement(Label("message", "Required: first name, last name, email."))
        message.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#fbbf24")
                    return style
                }())
        message.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "header",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 58, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(1),
                charsSize: Size(width: 58, height: 1),
                cellCharsSize: Size(width: 58, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "save_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 0, col: 2, width: 26, height: 1),
                sourceCell: Rect(row: 26, col: 0, width: 58, height: 2),
                width: .auto(26),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(2),
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 58, height: 2),
                marginRight: 30,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "cancel_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 0, col: 30, width: 536870911, height: 1),
                sourceCell: Rect(row: 26, col: 0, width: 58, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(2),
                charsSize: Size(width: 26, height: 1),
                cellCharsSize: Size(width: 58, height: 2),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "first_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 1, col: 2, width: 11, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 45,
                marginBottom: 21,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "first_name",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 1, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 39, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 21,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "last_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 3, col: 2, width: 10, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .auto(10),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 46,
                marginBottom: 19,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "last_name",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 3, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 39, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 19,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "email_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 5, col: 2, width: 11, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 45,
                marginBottom: 17,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "email",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 5, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 39, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 17,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "phone_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 7, col: 2, width: 11, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 45,
                marginBottom: 15,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "phone",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 7, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 39, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 15,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "company_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 9, col: 2, width: 13, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .auto(13),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 43,
                marginBottom: 13,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "company",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 9, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 39, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 13,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "city_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 11, col: 2, width: 10, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .auto(10),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 46,
                marginBottom: 11,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "city",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 11, col: 17, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 39, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 11,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "category_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 13, col: 2, width: 14, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .auto(14),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 14, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 42,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "category",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 13, col: 18, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 38, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "active",
                type: "checkbox",
                cellName: "",
                relative: Rect(row: 15, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 54, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 7,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#b8c4d8")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "notes_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 17, col: 2, width: 11, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 45,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "notes",
                type: "textarea",
                cellName: "",
                relative: Rect(row: 17, col: 14, width: 536870911, height: 3),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(3),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 42, height: 3),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "message",
                type: "label",
                cellName: "",
                relative: Rect(row: 21, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 58, height: 23),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(58),
                cellHeight: .auto(23),
                charsSize: Size(width: 54, height: 1),
                cellCharsSize: Size(width: 58, height: 23),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#fbbf24")
                    return style
                }()
            ),
        ]
    }

    open func onFirstNameChange(_ value: String) { }
    open func onFirstNameSubmit(_ value: String) { }
    open func onLastNameChange(_ value: String) { }
    open func onLastNameSubmit(_ value: String) { }
    open func onEmailChange(_ value: String) { }
    open func onEmailSubmit(_ value: String) { }
    open func onPhoneChange(_ value: String) { }
    open func onPhoneSubmit(_ value: String) { }
    open func onCompanyChange(_ value: String) { }
    open func onCompanySubmit(_ value: String) { }
    open func onCityChange(_ value: String) { }
    open func onCitySubmit(_ value: String) { }
    open func onCategoryChange(_ value: String) { }
    open func onActiveChange(_ value: String) { }
    open func onNotesChange(_ value: String) { }
    open func onNotesSubmit(_ value: String) { }
    open func onSaveBtnClick() { }
    open func onCancelBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "save_btn"
        {
            onSaveBtnClick()
            return true
        }
        else if name == "cancel_btn"
        {
            onCancelBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "first_name"
        {
            onFirstNameChange(value)
            return true
        }
        else if name == "last_name"
        {
            onLastNameChange(value)
            return true
        }
        else if name == "email"
        {
            onEmailChange(value)
            return true
        }
        else if name == "phone"
        {
            onPhoneChange(value)
            return true
        }
        else if name == "company"
        {
            onCompanyChange(value)
            return true
        }
        else if name == "city"
        {
            onCityChange(value)
            return true
        }
        else if name == "active"
        {
            onActiveChange(value)
            return true
        }
        else if name == "notes"
        {
            onNotesChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "first_name"
        {
            onFirstNameSubmit(value)
            return true
        }
        else if name == "last_name"
        {
            onLastNameSubmit(value)
            return true
        }
        else if name == "email"
        {
            onEmailSubmit(value)
            return true
        }
        else if name == "phone"
        {
            onPhoneSubmit(value)
            return true
        }
        else if name == "company"
        {
            onCompanySubmit(value)
            return true
        }
        else if name == "city"
        {
            onCitySubmit(value)
            return true
        }
        else if name == "notes"
        {
            onNotesSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "category"
        {
            onCategoryChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}
