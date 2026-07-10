import Foundation
import Uimd

private struct ExpenseItem
{
    var id: String
    var item: String
    var amount: Double
    var category: String
    var paid: Bool
}

private let kDefaultCategories = ["Food", "Transport", "Home", "Health", "Other"]
private let kDefaultCurrency = "EUR"
private let kDefaultShowPaidExpenses = true
private let kDefaultMonthlyLimit = 500.0
private let kMinimumRenderHeight = 1

private func seedExpenses() -> [ExpenseItem]
{
    [
        ExpenseItem(id: "e-1", item: "Groceries", amount: 42.80, category: "Food", paid: true),
        ExpenseItem(id: "e-2", item: "Bus ticket", amount: 2.50, category: "Transport", paid: true),
        ExpenseItem(id: "e-3", item: "Light bulbs", amount: 8.90, category: "Home", paid: false),
        ExpenseItem(id: "e-4", item: "Pharmacy", amount: 14.20, category: "Health", paid: true),
        ExpenseItem(id: "e-5", item: "Notebook", amount: 5.40, category: "Other", paid: false),
        ExpenseItem(id: "e-6", item: "Internet bill", amount: 29.99, category: "Home", paid: true),
        ExpenseItem(id: "e-7", item: "Team lunch", amount: 18.50, category: "Food", paid: false),
        ExpenseItem(id: "e-8", item: "Train pass", amount: 24.00, category: "Transport", paid: true),
        ExpenseItem(id: "e-9", item: "Vitamins", amount: 11.75, category: "Health", paid: false),
        ExpenseItem(id: "e-10", item: "Desk lamp", amount: 21.30, category: "Home", paid: false),
        ExpenseItem(id: "e-11", item: "Cloud storage", amount: 9.99, category: "Other", paid: true),
        ExpenseItem(id: "e-12", item: "Replacement keyboard", amount: 44.00, category: "Other", paid: false),
    ]
}

private final class ExpenseRow: ExpenseRowUI
{
    let record: ExpenseItem
    private let onDelete: ((String) -> Void)?
    private let onChange: ((String, String, Bool) -> Void)?

    init(
        _ record: ExpenseItem,
        categories: [String],
        onDelete: ((String) -> Void)? = nil,
        onChange: ((String, String, Bool) -> Void)? = nil
    )
    {
        self.record = record
        self.onDelete = onDelete
        self.onChange = onChange
        super.init()
        mode = "expand_width"
        item.setText(record.item)
        amount.setText(String(format: "%.2f", record.amount))
        paid.checked = record.paid
        category.options = categories
        category.selectValue(record.category)
    }

    override func onPaidChange(_ value: String)
    {
        _ = value
        onChange?(record.id, category.terminalText, paid.checked)
    }

    override func onCategoryChange(_ value: String)
    {
        onChange?(record.id, value, paid.checked)
    }

    override func onDeleteBtnClick()
    {
        onDelete?(record.id)
    }
}

private final class ExpensesList: ExpensesListUI
{
    private var rows: [ExpenseItem] = []
    private var categories: [String] = []
    private var onDelete: ((String) -> Void)?
    private var onChange: ((String, String, Bool) -> Void)?

    override init()
    {
        super.init()
        setAutoScroll(false)
    }

    func setRows(_ rows: [ExpenseItem], categories: [String])
    {
        self.rows = rows
        self.categories = categories
        refreshRows()
    }

    func setCallbacks(
        onDelete: ((String) -> Void)?,
        onChange: ((String, String, Bool) -> Void)?
    )
    {
        self.onDelete = onDelete
        self.onChange = onChange
        refreshRows()
    }

    func refreshRows()
    {
        clearChildren()
        for row in rows
        {
            let rowWindow = ExpenseRow(
                row,
                categories: categories,
                onDelete: onDelete,
                onChange: onChange
            )
            let naturalSize = rowWindow.generatedContentSize()
            let measuredSize = rowWindow.generatedContentSizeForWidth(max(1, naturalSize.width))
            let reusable = ReusableElement("row")
            reusable.setChild(rowWindow)
            reusable.frame = Rect(row: 0, col: 0, width: 0, height: max(kMinimumRenderHeight, measuredSize.height))
            _ = scrollView().addChild(reusable)
        }
        scrollToTop()
    }
}

private final class ExpensesView: ExpensesViewUI
{
    private weak var shell: ExpenseTracker?
    let list = ExpensesList()

    init(_ shell: ExpenseTracker)
    {
        self.shell = shell
        super.init()
        expenses.setChild(list)
        refresh()
    }

    func refresh()
    {
        guard let shell else
        {
            return
        }
        currency_label.setText(shell.currency)
        category_input.options = shell.categories
        if !shell.categories.contains(category_input.terminalText)
        {
            category_input.selectValue(shell.categories.first ?? "")
        }
        list.setRows(shell.visibleExpenses(), categories: shell.categories)
        list.setCallbacks(
            onDelete: { [weak shell] id in
                shell?.confirmDeleteExpense(id)
            },
            onChange: { [weak shell] id, category, paid in
                shell?.updateExpense(id: id, category: category, paid: paid)
            }
        )
    }

    override func onAddExpenseBtnClick()
    {
        addExpenseFromInputs()
    }

    func addExpenseFromInputs()
    {
        shell?.addExpense(item: item_input.value, amount: amount_input.numberValue, category: category_input.terminalText)
        item_input.setValue("")
        amount_input.setValue(0)
        refresh()
    }
}

private final class CategoriesView: CategoriesViewUI
{
    private weak var shell: ExpenseTracker?

    init(_ shell: ExpenseTracker)
    {
        self.shell = shell
        super.init()
        refresh()
    }

    func refresh(selected: String? = nil)
    {
        guard let shell else
        {
            return
        }
        categories.options = shell.categories
        let preferred = selected.flatMap { shell.categories.contains($0) ? $0 : nil } ?? categories.selectedValues.first
        if let preferred, shell.categories.contains(preferred)
        {
            categories.setSelectedValues([preferred])
        }
        else if let first = shell.categories.first
        {
            categories.setSelectedValues([first])
        }
    }

    override func onAddCategoryBtnClick()
    {
        addCategoryFromInput()
    }

    override func onRemoveCategoryBtnClick()
    {
        removeSelectedCategory()
    }

    func addCategoryFromInput()
    {
        let added = shell?.addCategory(category_name.value) ?? ""
        category_name.setValue("")
        refresh(selected: added.isEmpty ? nil : added)
    }

    func removeSelectedCategory()
    {
        shell?.removeCategory(categories.selectedValues.first ?? "")
        refresh()
    }
}

private final class SettingsView: SettingsViewUI
{
    private weak var shell: ExpenseTracker?

    init(_ shell: ExpenseTracker)
    {
        self.shell = shell
        super.init()
        refresh()
    }

    func refresh()
    {
        guard let shell else
        {
            return
        }
        currency.selectValue(shell.currency)
        show_paid.checked = shell.showPaidExpenses
        monthly_limit.setValue(shell.monthlyLimit)
    }

    override func onResetBtnClick()
    {
        shell?.resetDemoData()
    }

    override func onCurrencyChange(_ value: String)
    {
        shell?.setCurrency(value)
    }

    override func onShowPaidChange(_ value: String)
    {
        _ = value
        shell?.setShowPaidExpenses(show_paid.checked)
    }

    override func onMonthlyLimitChange(_ value: String)
    {
        _ = value
        shell?.setMonthlyLimit(monthly_limit.numberValue)
    }

    override func onMonthlyLimitSubmit(_ value: String)
    {
        _ = value
        shell?.setMonthlyLimit(monthly_limit.numberValue)
    }
}

private final class ExpenseTracker: ExpenseTrackerUI
{
    var categories = kDefaultCategories
    var currency = kDefaultCurrency
    var showPaidExpenses = kDefaultShowPaidExpenses
    var monthlyLimit = kDefaultMonthlyLimit
    private var expenses = seedExpenses()
    private var nextExpenseNumber = 13
    private var pendingDeleteId = ""
    private var finished = false
    private let modalStack = GeneratedWindowStack()

    override init()
    {
        super.init()
        showExpenses()
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.windowStack = modalStack
        options.initialFocusName = "expenses_btn"
        return options
    }

    override func shouldClose() -> Bool
    {
        finished
    }

    override func onQuitBtnClick()
    {
        finished = true
    }

    override func onExpensesBtnClick()
    {
        showExpenses()
    }

    override func onCategoriesBtnClick()
    {
        showCategories()
    }

    override func onSettingsBtnClick()
    {
        showSettings()
    }

    func visibleExpenses() -> [ExpenseItem]
    {
        showPaidExpenses ? expenses : expenses.filter { !$0.paid }
    }

    func addExpense(item: String, amount: Double, category: String)
    {
        let trimmed = item.trimmingCharacters(in: .whitespacesAndNewlines)
        let name = trimmed.isEmpty ? "Untitled expense" : trimmed
        let selectedCategory = categories.contains(category) ? category : (categories.first ?? "")
        expenses.insert(ExpenseItem(id: "e-\(nextExpenseNumber)", item: name, amount: amount, category: selectedCategory, paid: false), at: 0)
        nextExpenseNumber += 1
    }

    func setCurrency(_ value: String)
    {
        currency = value.isEmpty ? kDefaultCurrency : value
        refreshExpensesView()
    }

    func setShowPaidExpenses(_ value: Bool)
    {
        showPaidExpenses = value
        refreshExpensesView()
    }

    func setMonthlyLimit(_ value: Double)
    {
        monthlyLimit = value
    }

    func addCategory(_ name: String) -> String
    {
        let trimmed = name.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !trimmed.isEmpty else
        {
            return ""
        }
        if !categories.contains(trimmed)
        {
            categories.append(trimmed)
        }
        return trimmed
    }

    func removeCategory(_ name: String)
    {
        guard !name.isEmpty, categories.count > 1, let index = categories.firstIndex(of: name) else
        {
            return
        }
        categories.remove(at: index)
        let fallback = categories.first ?? ""
        for index in expenses.indices where expenses[index].category == name
        {
            expenses[index].category = fallback
        }
    }

    private func showExpenses()
    {
        selectNavButton(expenses_btn)
        main.setChild(ExpensesView(self))
    }

    private func showCategories()
    {
        selectNavButton(categories_btn)
        main.setChild(CategoriesView(self))
    }

    private func showSettings()
    {
        selectNavButton(settings_btn)
        main.setChild(SettingsView(self))
    }

    func updateExpense(id: String, category: String, paid: Bool)
    {
        guard let index = expenses.firstIndex(where: { $0.id == id }) else
        {
            return
        }
        expenses[index].category = category
        expenses[index].paid = paid
    }

    func confirmDeleteExpense(_ id: String)
    {
        pendingDeleteId = id
        let item = expenses.first { $0.id == id }?.item ?? "expense"
        let dialog = MessageBoxYesNo("Delete Expense", "Delete " + item + "?")
        dialog.onClose = { [weak self] confirmed in
            guard let self else
            {
                return
            }
            self.modalStack.popTop()
            if confirmed
            {
                self.expenses.removeAll { $0.id == self.pendingDeleteId }
                self.refreshCurrentView()
            }
            self.pendingDeleteId = ""
        }
        modalStack.push(dialog)
    }

    func resetDemoData()
    {
        categories = kDefaultCategories
        currency = kDefaultCurrency
        showPaidExpenses = kDefaultShowPaidExpenses
        monthlyLimit = kDefaultMonthlyLimit
        expenses = seedExpenses()
        nextExpenseNumber = 13
        refreshCurrentView()
    }

    private func refreshExpensesView()
    {
        if let view = main.child as? ExpensesView
        {
            view.refresh()
        }
    }

    private func refreshCurrentView()
    {
        if let view = main.child as? ExpensesView
        {
            view.refresh()
        }
        else if let view = main.child as? CategoriesView
        {
            view.refresh()
        }
        else if let view = main.child as? SettingsView
        {
            view.refresh()
        }
    }

    private func selectNavButton(_ target: Button)
    {
        let normalStyle = normalNavButtonStyle(target)
        let activeStyle = activeNavButtonStyle(target)
        expenses_btn.setStyle(normalStyle)
        categories_btn.setStyle(normalStyle)
        settings_btn.setStyle(normalStyle)
        target.setStyle(activeStyle)
    }

    private func activeNavButtonStyle(_ button: Button) -> Style
    {
        button.style.merged(button.focusStyle)
    }

    private func normalNavButtonStyle(_ target: Button) -> Style
    {
        if target !== expenses_btn && !(main.child is ExpensesView)
        {
            return expenses_btn.style
        }
        if target !== categories_btn && !(main.child is CategoriesView)
        {
            return categories_btn.style
        }
        if target !== settings_btn && !(main.child is SettingsView)
        {
            return settings_btn.style
        }
        return target.style
    }

}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            let app = ExpenseTracker()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}
