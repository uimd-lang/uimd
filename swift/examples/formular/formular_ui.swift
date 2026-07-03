// Auto-generated UI code for formular - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class FormularUI: GeneratedWindowBase
{
    public var title_label: Label!
    public var name_label: Label!
    public var name_input: TextInput!
    public var email_label: Label!
    public var email_input: TextInput!
    public var age_label: Label!
    public var age_input: NumberInput!
    public var description_input: TextArea!
    public var country_label: Label!
    public var country_combo: ComboBox!
    public var role_label: Label!
    public var role_listbox: ListBox!
    public var notify_check: CheckBox!
    public var terms_check: CheckBox!
    public var save_btn: Button!
    public var cancel_btn: Button!

    public init()
    {
        super.init("FormApp")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "Formular", source: "# FormApp\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\ntitle_label:\n  type: label\n  text: \"User profile\"\n  description: \"Title of the form.\"\n  expose: false\n\nname_label:\n  type: label\n  text: Name\n  description: \"Label for the name field.\"\n\nname_input:\n  type: textinput\n  value: \"\"\n  description: \"User name text field.\"\n\nemail_label:\n  type: label\n  text: Email\n  description: \"Label for the email field.\"\n\nemail_input:\n  type: textinput\n  value: \"\"\n  description: \"Email address text field.\"\n\nage_label:\n  type: label\n  text: Age\n  description: \"Label for the age field.\"\n\nage_input:\n  type: numberinput\n  description: \"Numeric age input.\"\n\ndescription_input:\n  type: textarea\n  description: \"Multiline profile description field.\"\n\ncountry_label:\n  type: label\n  text: Country\n  description: \"Label for the country selector.\"\n\ncountry_combo:\n  type: combobox\n  options: [Slovakia, \"Czech Republic\", Poland, Hungary, Austria]\n  selected_item: Slovakia\n  description: \"Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.\"\n\nrole_label:\n  type: label\n  text: Role\n  description: \"Label for the role selector.\"\n\nrole_listbox:\n  type: listbox\n  options: [Developer, Designer, Manager, QA, DevOps, Tester, Hacker]\n  selected_items: [Developer]\n  multiple: \"true\"\n  description: \"Multi-select role listbox for choosing one or more user roles.\"\n\nnotify_check:\n  type: checkbox\n  title: \"Email notifications\"\n  description: \"Toggle email notifications.\"\n\nterms_check:\n  type: checkbox\n  title: \"I accept the terms and conditions\"\n  description: \"Toggle acceptance of terms and conditions.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Submit the form and serialize the entered data.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Cancel the form action.\"\n```\n\n## Style\n\n```yaml\n@title_label:\n  text-align: center\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------+\n|                                                    |\n|  title_label.....................................  |\n|                                                    |\n|  name_label       name_input.....................  |\n|                                                    |\n|  email_label      email_input....................  |\n|                                                    |\n|  age_label        age_input......................  |\n|                                                    |\n|  country_label    country_combo..................  |\n|                                                    |\n|  \"Description\"    description_input..............  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  role_label       role_listbox...................  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  notify_check....................................  |\n|  terms_check.....................................  |\n|                                                    |\n|       save_btn......      cancel_btn.......        |\n|                                                    |\n+----------------------------------------------------+\n```\n", description: "User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        title_label = addElement(Label("title_label", "User profile"))
        title_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textAlign = "center"
                    return style
                }())
        title_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        name_label = addElement(Label("name_label", "Name"))
        name_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        name_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        name_input = addElement(TextInput("name_input", "", maxLength: 0))
        name_input.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        name_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        name_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        name_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#ffffff")
                    style.color = Color("#0e1117")
                    return style
                }())
        name_input.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        name_input.setDisabledStyle({
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
        email_input = addElement(TextInput("email_input", "", maxLength: 0))
        email_input.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        email_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        email_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        email_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#ffffff")
                    style.color = Color("#0e1117")
                    return style
                }())
        email_input.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        email_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        age_label = addElement(Label("age_label", "Age"))
        age_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        age_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        age_input = addElement(NumberInput("age_input", 0.0, step: 1.0))
        age_input.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        age_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        age_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        age_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#ffffff")
                    style.color = Color("#0e1117")
                    return style
                }())
        age_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        description_input = addElement(TextArea("description_input", "", maxLength: 0))
        description_input.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        description_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        description_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        description_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#ffffff")
                    style.color = Color("#0e1117")
                    return style
                }())
        description_input.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        description_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        country_label = addElement(Label("country_label", "Country"))
        country_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        country_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        country_combo = addElement(ComboBox("country_combo", ["Slovakia", "Czech Republic", "Poland", "Hungary", "Austria"], selectedIndex: 0))
        country_combo.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        country_combo.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        country_combo.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        country_combo.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        country_combo.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        role_label = addElement(Label("role_label", "Role"))
        role_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        role_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        role_listbox = addElement(ListBox("role_listbox", ["Developer", "Designer", "Manager", "QA", "DevOps", "Tester", "Hacker"], selectedValues: ["Developer"]))
        role_listbox.multiple = true
        role_listbox.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        role_listbox.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1d2f4d")
                    style.color = Color("#ffffff")
                    return style
                }())
        role_listbox.setEditStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        role_listbox.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        role_listbox.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        notify_check = addElement(CheckBox("notify_check", "Email notifications", false))
        notify_check.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#b8c4d8")
                    return style
                }())
        notify_check.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#ffffff")
                    return style
                }())
        notify_check.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#44cc88")
                    return style
                }())
        notify_check.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    return style
                }())
        notify_check.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        terms_check = addElement(CheckBox("terms_check", "I accept the terms and conditions", false))
        terms_check.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#b8c4d8")
                    return style
                }())
        terms_check.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#ffffff")
                    return style
                }())
        terms_check.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#44cc88")
                    return style
                }())
        terms_check.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    return style
                }())
        terms_check.setDisabledStyle({
                    var style = Style()
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
                    style.background = Color("#2b5f9f")
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
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        cancel_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "title_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 1, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 48, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 24,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "name_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 3, col: 2, width: 10, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .auto(10),
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 40,
                marginBottom: 22,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "name_input",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 3, col: 19, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 22,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "email_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 5, col: 2, width: 11, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 39,
                marginBottom: 20,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "email_input",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 5, col: 19, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 20,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "age_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 7, col: 2, width: 9, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 41,
                marginBottom: 18,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "age_input",
                type: "numberinput",
                cellName: "",
                relative: Rect(row: 7, col: 19, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 18,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "country_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 9, col: 2, width: 13, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .auto(13),
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 37,
                marginBottom: 16,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "country_combo",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 9, col: 19, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 16,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Description\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 11, col: 2, width: 13, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .auto(13),
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 37,
                marginBottom: 14,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "description_input",
                type: "textarea",
                cellName: "",
                relative: Rect(row: 11, col: 19, width: 536870911, height: 4),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(4),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 31, height: 4),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 11,
                cellStyle: Style(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "role_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 16, col: 2, width: 10, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .auto(10),
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 40,
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
                name: "role_listbox",
                type: "listbox",
                cellName: "",
                relative: Rect(row: 16, col: 19, width: 536870911, height: 4),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(4),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 31, height: 4),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 6,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "notify_check",
                type: "checkbox",
                cellName: "",
                relative: Rect(row: 21, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 48, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 4,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#b8c4d8")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "terms_check",
                type: "checkbox",
                cellName: "",
                relative: Rect(row: 22, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 48, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 2,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#b8c4d8")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "save_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 24, col: 7, width: 14, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .auto(14),
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 14, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 31,
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
                relative: Rect(row: 24, col: 27, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 52, height: 26),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(52),
                cellHeight: .auto(26),
                charsSize: Size(width: 17, height: 1),
                cellCharsSize: Size(width: 52, height: 26),
                marginRight: 8,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onNameInputChange(_ value: String) { }
    open func onNameInputSubmit(_ value: String) { }
    open func onEmailInputChange(_ value: String) { }
    open func onEmailInputSubmit(_ value: String) { }
    open func onAgeInputChange(_ value: String) { }
    open func onAgeInputSubmit(_ value: String) { }
    open func onDescriptionInputChange(_ value: String) { }
    open func onDescriptionInputSubmit(_ value: String) { }
    open func onCountryComboChange(_ value: String) { }
    open func onRoleListboxSelectionChange(_ value: [String]) { }
    open func onNotifyCheckChange(_ value: String) { }
    open func onTermsCheckChange(_ value: String) { }
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
        if name == "name_input"
        {
            onNameInputChange(value)
            return true
        }
        else if name == "email_input"
        {
            onEmailInputChange(value)
            return true
        }
        else if name == "age_input"
        {
            onAgeInputChange(value)
            return true
        }
        else if name == "description_input"
        {
            onDescriptionInputChange(value)
            return true
        }
        else if name == "notify_check"
        {
            onNotifyCheckChange(value)
            return true
        }
        else if name == "terms_check"
        {
            onTermsCheckChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "name_input"
        {
            onNameInputSubmit(value)
            return true
        }
        else if name == "email_input"
        {
            onEmailInputSubmit(value)
            return true
        }
        else if name == "age_input"
        {
            onAgeInputSubmit(value)
            return true
        }
        else if name == "description_input"
        {
            onDescriptionInputSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "country_combo"
        {
            onCountryComboChange(value.isEmpty ? "" : value[0])
            return true
        }
        else if name == "role_listbox"
        {
            onRoleListboxSelectionChange(value)
            return true
        }
        return false
    }
}
