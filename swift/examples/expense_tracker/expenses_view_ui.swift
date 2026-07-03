// Auto-generated UI code for expenses_view - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ExpensesViewUI: GeneratedWindowBase
{
    public var view_title: Label!
    public var item_input: TextInput!
    public var amount_input: NumberInput!
    public var category_input: ComboBox!
    public var currency_label: Label!
    public var add_expense_btn: Button!
    public var expenses: ReusableElement!

    public init()
    {
        super.init("Expenses View")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ExpensesView", source: "# Expenses View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Expense entry view with a dynamic scrollable list.\"\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - expenses_list/expenses_list\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Expenses\n\nitem_input:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n\namount_input:\n  type: numberinput\n  value: 0\n  format: \"0.00\"\n\ncategory_input:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ncurrency_label:\n  type: label\n  text: EUR\n\nadd_expense_btn:\n  type: button\n  title: Add expense\n\nexpenses:\n  type: expenses_list\n  cpp-class: expense_tracker_example::ExpensesList\n  cpp-header: \"expenses_list/expenses_list.hpp\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#bfdbfe\"\n@currency_label:\n  color: \"#bfdbfe\"\n@expenses:\n  background: \"#172033\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nnumberinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Item:\"     item_input.................................... |\n|                                                            |\n| \"Amount:\"   amount_input............. currency_label...... |\n|                                                            |\n| \"Category:\" category_input................................ |\n|                                                            |\n| add_expense_btn........................................... |\n|                                                            |\n| expenses..**.............................................. |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n", description: "Expense entry view with a dynamic scrollable list.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        view_title = addElement(Label("view_title", "Expenses"))
        view_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    return style
                }())
        view_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        item_input = addElement(TextInput("item_input", "", maxLength: 80))
        item_input.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        item_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        item_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#ffffff")
                    return style
                }())
        item_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        item_input.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        item_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        amount_input = addElement(NumberInput("amount_input", 0.0, step: 1.0))
        amount_input.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        amount_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        amount_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#ffffff")
                    return style
                }())
        amount_input.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        amount_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        category_input = addElement(ComboBox("category_input", ["Food", "Transport", "Home", "Health", "Other"], selectedIndex: 0))
        category_input.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        category_input.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        category_input.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        category_input.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        category_input.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        currency_label = addElement(Label("currency_label", "EUR"))
        currency_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    return style
                }())
        currency_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        add_expense_btn = addElement(Button("add_expense_btn", "Add expense"))
        add_expense_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        add_expense_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        add_expense_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        expenses = addElement(ReusableElement("expenses"))
        expenses.setStyle({
                    var style = Style()
                    style.background = Color("#172033")
                    return style
                }())
        expenses.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        expenses.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        expenses.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "view_title",
                type: "label",
                cellName: "view",
                relative: Rect(row: 0, col: 1, width: 58, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(58),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 58, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 1,
                marginBottom: 14,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Item:\"",
                type: "label",
                cellName: "view",
                relative: Rect(row: 2, col: 1, width: 7, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(7),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 7, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 52,
                marginBottom: 12,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "item_input",
                type: "textinput",
                cellName: "view",
                relative: Rect(row: 2, col: 13, width: 46, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(46),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 46, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 1,
                marginBottom: 12,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Amount:\"",
                type: "label",
                cellName: "view",
                relative: Rect(row: 4, col: 1, width: 9, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 50,
                marginBottom: 10,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "amount_input",
                type: "numberinput",
                cellName: "view",
                relative: Rect(row: 4, col: 13, width: 25, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(25),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 25, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 22,
                marginBottom: 10,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "currency_label",
                type: "label",
                cellName: "view",
                relative: Rect(row: 4, col: 39, width: 20, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(20),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 1,
                marginBottom: 10,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Category:\"",
                type: "label",
                cellName: "view",
                relative: Rect(row: 6, col: 1, width: 11, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 48,
                marginBottom: 8,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "category_input",
                type: "combobox",
                cellName: "view",
                relative: Rect(row: 6, col: 13, width: 46, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(46),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 46, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 1,
                marginBottom: 8,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "add_expense_btn",
                type: "button",
                cellName: "view",
                relative: Rect(row: 8, col: 1, width: 58, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .auto(58),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 58, height: 1),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 1,
                marginBottom: 6,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "expenses",
                type: "expenses_list",
                cellName: "view",
                relative: Rect(row: 10, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 15),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(15),
                charsSize: Size(width: 58, height: 5),
                cellCharsSize: Size(width: 60, height: 15),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    return style
                }()
            ),
        ]
    }

    open func onItemInputChange(_ value: String) { }
    open func onItemInputSubmit(_ value: String) { }
    open func onAmountInputChange(_ value: String) { }
    open func onAmountInputSubmit(_ value: String) { }
    open func onCategoryInputChange(_ value: String) { }
    open func onAddExpenseBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "add_expense_btn"
        {
            onAddExpenseBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "item_input"
        {
            onItemInputChange(value)
            return true
        }
        else if name == "amount_input"
        {
            onAmountInputChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "item_input"
        {
            onItemInputSubmit(value)
            return true
        }
        else if name == "amount_input"
        {
            onAmountInputSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "category_input"
        {
            onCategoryInputChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}
