// Auto-generated UI code for expense_row - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ExpenseRowUI: GeneratedWindowBase
{
    public var paid: CheckBox!
    public var item: Label!
    public var amount: Label!
    public var category: ComboBox!
    public var delete_btn: Button!

    public init()
    {
        super.init("Expense Row")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ExpenseRow", source: "# Expense Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: \"Reusable expense row with paid state, category selector, and delete action.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\npaid:\n  type: checkbox\n  title: Paid\n  value: false\n\nitem:\n  type: label\n  text: Item\n  user-select: text\n\namount:\n  type: label\n  text: \"0.00\"\n\ncategory:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ndelete_btn:\n  type: button\n  title: Delete\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@row:\n  background: \"#172033\"\n  padding: 1\n@item:\n  color: \"#ffffff\"\n@amount:\n  color: \"#bfdbfe\"\ncheckbox:\n  color: \"#cbd5e1\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------+\n| paid..... item..**..................... amount...... |\n| category..**........................... delete_btn.. |\n+------------------------------------------------------+\n```\n", description: "Reusable expense row with paid state, category selector, and delete action.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        paid = addElement(CheckBox("paid", "Paid", false))
        paid.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        paid.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#ffffff")
                    return style
                }())
        paid.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#22c55e")
                    return style
                }())
        paid.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#94a3b8")
                    return style
                }())
        paid.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        item = addElement(Label("item", "Item"))
        item.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    return style
                }())
        item.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        amount = addElement(Label("amount", "0.00"))
        amount.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    return style
                }())
        amount.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        category = addElement(ComboBox("category", ["Food", "Transport", "Home", "Health", "Other"], selectedIndex: 0))
        category.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        category.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        category.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
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
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "paid",
                type: "checkbox",
                cellName: "row",
                relative: Rect(row: 0, col: 1, width: 9, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 2),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 54, height: 2),
                marginRight: 44,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
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
                name: "item",
                type: "label",
                cellName: "row",
                relative: Rect(row: 0, col: 11, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 2),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 29, height: 2),
                cellCharsSize: Size(width: 54, height: 2),
                marginRight: 14,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "amount",
                type: "label",
                cellName: "row",
                relative: Rect(row: 0, col: 41, width: 12, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 2),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 54, height: 2),
                marginRight: 1,
                marginBottom: 1,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "category",
                type: "combobox",
                cellName: "row",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 39, height: 1),
                cellCharsSize: Size(width: 54, height: 2),
                marginRight: 14,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "delete_btn",
                type: "button",
                cellName: "row",
                relative: Rect(row: 1, col: 41, width: 12, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 2),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 54, height: 2),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onPaidChange(_ value: String) { }
    open func onCategoryChange(_ value: String) { }
    open func onDeleteBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "delete_btn"
        {
            onDeleteBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "paid"
        {
            onPaidChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
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
