// Auto-generated UI code for contacts_manager - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ContactsManagerUI: GeneratedWindowBase
{
    public var header: Label!
    public var contacts_label: Label!
    public var contacts: ListBox!
    public var detail: Label!
    public var add_btn: Button!
    public var edit_btn: Button!
    public var delete_btn: Button!
    public var save_btn: Button!
    public var reload_btn: Button!
    public var close_btn: Button!
    public var status: Label!

    public init()
    {
        super.init("Contacts Manager")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ContactsManager", source: "# Contacts Manager\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - contact_form\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Contacts Manager\"\n  description: \"Application title.\"\n  expose: false\n\ncontacts_label:\n  type: label\n  text: Contacts\n  description: \"Label for the contacts list.\"\n\ncontacts:\n  type: listbox\n  options: []\n  selected_items: []\n  multiple: \"false\"\n  description: \"List of contact records; selecting a row updates the detail panel.\"\n\ndetail:\n  type: label\n  text: \"Select a contact.\"\n  description: \"Read-only details for the selected contact.\"\n\nadd_btn:\n  type: button\n  title: Add\n  description: \"Open the contact form to create a new contact.\"\n\nedit_btn:\n  type: button\n  title: Edit\n  description: \"Open the selected contact for editing.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Delete the selected contact.\"\n\nsave_btn:\n  type: button\n  title: \"Save CSV\"\n  description: \"Save contacts to the configured CSV file.\"\n\nreload_btn:\n  type: button\n  title: Reload\n  description: \"Reload contacts from the configured CSV file.\"\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: \"Close the contacts manager.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows save, load, and validation status messages.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\n@list:\n  background: \"#101a2b\"\n@contacts:\n  background: \"#ffffff11\"\n  color: \"#cbd5e1\"\n  focus-background: \"#ffffff22\"\n  edit-background: \"#00000000\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n@detail_cell:\n  background: \"#0b1120\"\n  padding: 1, 1, 1, 1\n@detail:\n  color: \"#d1d5db\"\n  user-select: text\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@status:\n  background: \"#111827\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-title-**-------------------------------------------------------------------------------+\n|header..**...................................................................close_btn..|\n+-actions-**-----------------------------------------------------------------------------+\n|                                                                                        |\n| add_btn....... edit_btn....... delete_btn....... save_btn....... reload_btn.......     |\n|                                                                                        |\n+-list-32---------------------------+-detail_cell-**-------------------------------------+\n| contacts_label.**................ | detail..**........................................ |\n| contacts.......**................ | .................................................. |\n| #................................ | #................................................. |\n| #................................ | #................................................. |\n| ................................. | .................................................. |\n* ................................. | .................................................. |\n* ................................. * .................................................. |\n| ................................. * .................................................. |\n| ................................. | .................................................. |\n|                                   | .................................................. |\n+-----------------------------------+----------------------------------------------------+\n| status..**............................................................................ |\n+----------------------------------------------------------------------------------------+\n```\n", description: "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header = addElement(Label("header", "Contacts Manager"))
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
        contacts_label = addElement(Label("contacts_label", "Contacts"))
        contacts_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        contacts_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        contacts = addElement(ListBox("contacts", [], selectedValues: []))
        contacts.setStyle({
                    var style = Style()
                    style.background = Color("#ffffff11")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        contacts.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff22")
                    style.color = Color("#ffffff")
                    return style
                }())
        contacts.setEditStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        contacts.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        contacts.setActiveStyle({
                    var style = Style()
                    style.background = Color("#DDDDDD99")
                    style.color = Color("#FFFFFF")
                    return style
                }())
        contacts.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        detail = addElement(Label("detail", "Select a contact."))
        detail.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    style.userSelect = "text"
                    return style
                }())
        detail.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        add_btn = addElement(Button("add_btn", "Add"))
        add_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        add_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        add_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        edit_btn = addElement(Button("edit_btn", "Edit"))
        edit_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        edit_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        edit_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        delete_btn = addElement(Button("delete_btn", "Delete"))
        delete_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        delete_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        delete_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        save_btn = addElement(Button("save_btn", "Save CSV"))
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
        reload_btn = addElement(Button("reload_btn", "Reload"))
        reload_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        reload_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        reload_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        close_btn = addElement(Button("close_btn", "Quit"))
        close_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        close_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        status = addElement(Label("status", "Ready"))
        status.setStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#93c5fd")
                    return style
                }())
        status.setCursorStyle({
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
                cellName: "title",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 88, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 77, height: 1),
                cellCharsSize: Size(width: 88, height: 1),
                marginRight: 11,
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
                name: "contacts_label",
                type: "label",
                cellName: "list",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 6, col: 0, width: 35, height: 10),
                width: .expanded,
                height: .auto(1),
                cellWidth: .fixed(32),
                cellHeight: .expanded,
                charsSize: Size(width: 33, height: 1),
                cellCharsSize: Size(width: 35, height: 10),
                marginRight: 1,
                marginBottom: 9,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#101a2b")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "detail",
                type: "label",
                cellName: "detail_cell",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 6, col: 36, width: 52, height: 10),
                width: .expanded,
                height: .fitContent,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 50, height: 10),
                cellCharsSize: Size(width: 52, height: 10),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#0b1120")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 17, col: 0, width: 88, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(88),
                cellHeight: .auto(1),
                charsSize: Size(width: 86, height: 1),
                cellCharsSize: Size(width: 88, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "close_btn",
                type: "button",
                cellName: "title",
                relative: Rect(row: 0, col: 77, width: 11, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 88, height: 1),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 88, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "add_btn",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 1, col: 1, width: 14, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 88, height: 3),
                width: .auto(14),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 14, height: 1),
                cellCharsSize: Size(width: 88, height: 3),
                marginRight: 73,
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
                name: "contacts",
                type: "listbox",
                cellName: "list",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 6, col: 0, width: 35, height: 10),
                width: .expanded,
                height: .fitContent,
                cellWidth: .fixed(32),
                cellHeight: .expanded,
                charsSize: Size(width: 33, height: 8),
                cellCharsSize: Size(width: 35, height: 10),
                marginRight: 1,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#101a2b")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#ffffff11")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "edit_btn",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 1, col: 16, width: 15, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 88, height: 3),
                width: .auto(15),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 15, height: 1),
                cellCharsSize: Size(width: 88, height: 3),
                marginRight: 57,
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
                name: "delete_btn",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 1, col: 32, width: 17, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 88, height: 3),
                width: .auto(17),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 17, height: 1),
                cellCharsSize: Size(width: 88, height: 3),
                marginRight: 39,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "save_btn",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 1, col: 50, width: 15, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 88, height: 3),
                width: .auto(15),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 15, height: 1),
                cellCharsSize: Size(width: 88, height: 3),
                marginRight: 23,
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
                name: "reload_btn",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 1, col: 66, width: 17, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 88, height: 3),
                width: .auto(17),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 17, height: 1),
                cellCharsSize: Size(width: 88, height: 3),
                marginRight: 5,
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

    open func onContactsSelectionChange(_ value: [String]) { }
    open func onAddBtnClick() { }
    open func onEditBtnClick() { }
    open func onDeleteBtnClick() { }
    open func onSaveBtnClick() { }
    open func onReloadBtnClick() { }
    open func onCloseBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "add_btn"
        {
            onAddBtnClick()
            return true
        }
        else if name == "edit_btn"
        {
            onEditBtnClick()
            return true
        }
        else if name == "delete_btn"
        {
            onDeleteBtnClick()
            return true
        }
        else if name == "save_btn"
        {
            onSaveBtnClick()
            return true
        }
        else if name == "reload_btn"
        {
            onReloadBtnClick()
            return true
        }
        else if name == "close_btn"
        {
            onCloseBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "contacts"
        {
            onContactsSelectionChange(value)
            return true
        }
        return false
    }
}
