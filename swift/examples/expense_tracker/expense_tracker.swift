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
private let kScrollIndicatorAbove = "^"
private let kScrollIndicatorBelow = "v"

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

    init(_ record: ExpenseItem, categories: [String])
    {
        self.record = record
        super.init()
        mode = "expand_width"
        item.setText(record.item)
        amount.setText(String(format: "%.2f", record.amount))
        paid.checked = record.paid
        category.options = categories
        category.selectValue(record.category)
    }
}

private final class ExpensesList: ExpensesListUI, GeneratedScrollableElementVisibility
{
    private var rowViews: [ExpenseRow] = []
    private var scrollOffset = 0
    private var viewOffset = 0
    var activeFocusedId: String?
    var activeEditMode = false

    func setRows(_ rows: [ExpenseItem], categories: [String])
    {
        rowViews = rows.map { ExpenseRow($0, categories: categories) }
        scrollOffset = 0
        viewOffset = 0
    }

    func focusOrder(hostId: String) -> [String]
    {
        rowViews.indices.flatMap
        {
            ["\(hostId)[\($0)].paid", "\(hostId)[\($0)].category", "\(hostId)[\($0)].delete_btn"]
        }
    }

    override func scrollBy(_ delta: Int, viewport: Size)
    {
        _ = scrollLines(delta, viewport: viewport)
    }

    override func handleScrollKey(_ key: String, viewport: Size) -> Bool
    {
        switch key
        {
        case "ArrowUp", "Up":
            return scrollLines(-generatedScrollKeyboardStepRows, viewport: viewport)
        case "ArrowDown", "Down":
            return scrollLines(generatedScrollKeyboardStepRows, viewport: viewport)
        case "PageUp":
            return scrollLines(-max(1, viewport.height), viewport: viewport)
        case "PageDown":
            return scrollLines(max(1, viewport.height), viewport: viewport)
        case "Home":
            let changed = scrollOffset != 0 || viewOffset != 0
            scrollOffset = 0
            viewOffset = 0
            return changed
        case "End":
            let previousOffset = scrollOffset
            let previousView = viewOffset
            scrollOffset = maxItemOffset(viewport: viewport)
            viewOffset = maxLineOffset(viewport: viewport)
            return scrollOffset != previousOffset || viewOffset != previousView
        default:
            return false
        }
    }

    func ensureElementVisible(_ elementId: String, viewport: Size)
    {
        guard let index = rowIndex(from: elementId),
              index >= scrollOffset,
              index < rowViews.count
        else
        {
            return
        }
        let scrollViewport = generatedScrollViewport(size: viewport, style: panelStyle())
        let viewportHeight = max(1, scrollViewport.height)
        var rowStart = 0
        for rowIndex in scrollOffset..<index
        {
            rowStart += rowHeight(rowViews[rowIndex], width: max(1, scrollViewport.width))
            if rowIndex < rowViews.count - 1
            {
                rowStart += panelGap()
            }
        }
        let rowEnd = rowStart + rowHeight(rowViews[index], width: max(1, scrollViewport.width))
        if rowStart < viewOffset
        {
            viewOffset = rowStart
        }
        else if rowEnd > viewOffset + viewportHeight
        {
            viewOffset = max(0, rowEnd - viewportHeight - generatedScrollIndicatorOverlapRows)
        }
        viewOffset = max(0, min(maxLineOffset(viewport: viewport), viewOffset))
    }

    func registerDynamicElements(on root: GeneratedWindowBase, hostId: String, hostFrame: Rect)
    {
        let paddingTop = panelPaddingTop()
        let paddingLeft = panelPaddingLeft()
        let paddingRight = panelPaddingRight()
        let gap = panelGap()
        let rowWidth = max(1, hostFrame.width - paddingLeft - paddingRight)
        var rowCursor = paddingTop
        guard scrollOffset < rowViews.count else
        {
            return
        }
        for index in scrollOffset..<rowViews.count
        {
            let row = rowViews[index]
            let rowHeight = rowHeight(row, width: rowWidth)
            _ = row.renderContent(size: Size(width: rowWidth, height: rowHeight), focusedName: nil, editMode: false)
            let prefix = "\(hostId)[\(index)]"
            for element in row.elements
            {
                element.frame = Rect(
                    row: hostFrame.row + rowCursor + element.frame.row - viewOffset,
                    col: hostFrame.col + paddingLeft + element.frame.col,
                    width: element.frame.width,
                    height: element.frame.height
                )
                root.registerDynamicElement(prefix + "." + element.name, element: element, parentFocusHostId: hostId)
            }
            rowCursor += rowHeight
            if index < rowViews.count - 1
            {
                rowCursor += gap
            }
        }
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        _ = focusedName
        _ = editMode
        let width = max(1, size.width)
        let height = max(1, size.height)
        let style = panelStyle()
        let blank = Array(repeating: TerminalCell(" ", foreground: style.color, background: style.background), count: width)
        var output = Array(repeating: blank, count: height)
        let rows = renderedRows(width: width, startIndex: scrollOffset)
        let paddingTop = min(panelPaddingTop(), max(0, height - 1))
        let paddingBottom = min(panelPaddingBottom(), max(0, height - paddingTop - 1))
        let viewportHeight = max(0, height - paddingTop - paddingBottom)
        viewOffset = max(0, min(max(0, rows.count - max(1, viewportHeight)), viewOffset))
        for row in 0..<viewportHeight
        {
            let targetRow = paddingTop + row
            let sourceRow = viewOffset + row
            if targetRow >= 0 && targetRow < output.count && sourceRow >= 0 && sourceRow < rows.count
            {
                output[targetRow] = rows[sourceRow]
            }
        }
        if width > 0 && height > 0
        {
            let indicatorCol = max(0, width - panelPaddingRight() - 1)
            let topIndicatorRow = min(height - 1, max(0, paddingTop))
            let bottomIndicatorRow = max(0, height - paddingBottom - 1)
            if scrollOffset > 0 || viewOffset > 0
            {
                applyScrollIndicator(row: &output[topIndicatorRow], col: indicatorCol, indicator: kScrollIndicatorAbove)
            }
            if viewOffset + viewportHeight < rows.count
            {
                applyScrollIndicator(row: &output[bottomIndicatorRow], col: indicatorCol, indicator: kScrollIndicatorBelow)
            }
        }
        return output
    }

    private func renderedRows(width: Int, startIndex: Int = 0) -> [[TerminalCell]]
    {
        var rows: [[TerminalCell]] = []
        let style = panelStyle()
        let blank = Array(repeating: TerminalCell(" ", foreground: style.color, background: style.background), count: max(1, width))
        let paddingLeft = panelPaddingLeft()
        let paddingRight = panelPaddingRight()
        let rowWidth = max(1, width - paddingLeft - paddingRight)
        guard startIndex < rowViews.count else
        {
            return rows
        }
        for index in startIndex..<rowViews.count
        {
            let rowView = rowViews[index]
            let localFocus: String?
            if let activeFocusedId, activeFocusedId.hasPrefix("main.expenses[\(index)].")
            {
                localFocus = String(activeFocusedId.split(separator: ".").last ?? "")
            }
            else
            {
                localFocus = nil
            }
            let rowHeight = rowHeight(rowView, width: rowWidth)
            let rendered = rowView.renderContent(size: Size(width: rowWidth, height: rowHeight), focusedName: localFocus, editMode: activeEditMode && localFocus != nil)
            for localRow in 0..<min(rowHeight, rendered.count)
            {
                var outputRow = blank
                let renderedRow = rendered[localRow]
                for col in 0..<min(rowWidth, renderedRow.count)
                {
                    let targetCol = paddingLeft + col
                    if targetCol >= 0 && targetCol < outputRow.count
                    {
                        outputRow[targetCol] = renderedRow[col]
                    }
                }
                if paddingRight > 0
                {
                    let firstTrailingCol = max(0, paddingLeft + rowWidth - paddingRight)
                    for targetCol in firstTrailingCol..<min(outputRow.count, paddingLeft + rowWidth)
                    {
                        let sourceCol = targetCol - paddingLeft
                        if sourceCol >= 0 && sourceCol < renderedRow.count && renderedRow[sourceCol].text == " "
                        {
                            outputRow[targetCol] = blank[targetCol]
                        }
                    }
                }
                rows.append(outputRow)
            }
            if index < rowViews.count - 1
            {
                for _ in 0..<panelGap()
                {
                    rows.append(blank)
                }
            }
        }
        return rows
    }

    private func renderedContentHeight(width: Int, startIndex: Int = 0) -> Int
    {
        guard startIndex < rowViews.count else
        {
            return 0
        }
        let rowWidth = max(1, max(1, width) - panelPaddingLeft() - panelPaddingRight())
        let visibleRows = rowViews[startIndex..<rowViews.count]
        let rowsHeight = visibleRows.reduce(0) { $0 + rowHeight($1, width: rowWidth) }
        let gapsHeight = max(0, visibleRows.count - 1) * panelGap()
        return rowsHeight + gapsHeight
    }

    private func maxLineOffset(viewport: Size) -> Int
    {
        let scrollViewport = generatedScrollViewport(size: viewport, style: panelStyle())
        return max(
            0,
            renderedContentHeight(width: max(1, viewport.width), startIndex: scrollOffset) - max(1, scrollViewport.height)
        )
    }

    @discardableResult
    private func scrollLines(_ delta: Int, viewport: Size) -> Bool
    {
        let previous = viewOffset
        viewOffset = max(0, min(maxLineOffset(viewport: viewport), viewOffset + delta))
        return viewOffset != previous
    }

    private func maxItemOffset(viewport: Size) -> Int
    {
        max(0, rowViews.count - maxChildrenInViewport(viewport: viewport))
    }

    private func maxChildrenInViewport(viewport: Size) -> Int
    {
        guard !rowViews.isEmpty else
        {
            return 0
        }
        let scrollViewport = generatedScrollViewport(size: viewport, style: panelStyle())
        let rowWidth = max(1, scrollViewport.width)
        let viewportHeight = max(1, scrollViewport.height)
        var usedHeight = 0
        var visibleCount = 0
        for row in rowViews
        {
            let nextHeight = rowHeight(row, width: rowWidth) + (visibleCount == 0 ? 0 : panelGap())
            if visibleCount > 0 && usedHeight + nextHeight > viewportHeight
            {
                break
            }
            usedHeight += nextHeight
            visibleCount += 1
        }
        return max(1, visibleCount)
    }

    private func rowIndex(from id: String) -> Int?
    {
        guard let open = id.firstIndex(of: "["),
              let close = id[open...].firstIndex(of: "]")
        else
        {
            return nil
        }
        return Int(id[id.index(after: open)..<close])
    }

    private func rowHeight(_ row: ExpenseRow, width: Int) -> Int
    {
        max(1, row.generatedContentSizeForWidth(max(1, width)).height)
    }

    private func inferredForeground(in row: [TerminalCell], endCol: Int) -> Color?
    {
        var foreground: Color?
        for col in 0...min(max(0, endCol), max(0, row.count - 1))
        {
            if let color = row[col].foreground, !color.isTransparent
            {
                foreground = color
            }
        }
        return foreground
    }

    private func applyScrollIndicator(row: inout [TerminalCell], col: Int, indicator: String)
    {
        guard col >= 0 && col < row.count else
        {
            return
        }
        row[col].text = indicator
        if let foreground = row[col].foreground, !foreground.isTransparent
        {
            return
        }
        row[col].foreground = inferredForeground(in: row, endCol: col)
    }

    private func panelStyle() -> Style
    {
        scrollView().style
    }

    private func panelPaddingTop() -> Int
    {
        let style = panelStyle()
        return max(0, style.paddingTop ?? style.padding ?? 0)
    }

    private func panelPaddingRight() -> Int
    {
        let style = panelStyle()
        return max(0, style.paddingRight ?? style.padding ?? 0)
    }

    private func panelPaddingBottom() -> Int
    {
        let style = panelStyle()
        return max(0, style.paddingBottom ?? style.padding ?? 0)
    }

    private func panelPaddingLeft() -> Int
    {
        let style = panelStyle()
        return max(0, style.paddingLeft ?? style.padding ?? 0)
    }

    private func panelGap() -> Int
    {
        max(0, panelStyle().gap ?? 0)
    }
}

private final class ExpensesView: ExpensesViewUI
{
    private weak var shell: ExpenseTracker?
    let list = ExpensesList()
    var activeFocusedId: String?
    var activeEditMode = false

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
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        list.activeFocusedId = activeFocusedId
        list.activeEditMode = activeEditMode
        return super.renderContent(size: size, focusedName: focusedName, editMode: editMode)
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

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        clearDynamicElements()
        if let expensesView = main.child as? ExpensesView
        {
            expensesView.activeFocusedId = focusedName
            expensesView.activeEditMode = editMode
        }
        let rendered = super.renderContent(size: size, focusedName: focusedName, editMode: editMode)
        registerMainChild()
        return rendered
    }

    override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "main.add_expense_btn", let view = main.child as? ExpensesView
        {
            view.addExpenseFromInputs()
            return true
        }
        if name == "main.add_category_btn", let view = main.child as? CategoriesView
        {
            view.addCategoryFromInput()
            return true
        }
        if name == "main.remove_category_btn", let view = main.child as? CategoriesView
        {
            view.removeSelectedCategory()
            return true
        }
        if name == "main.reset_btn"
        {
            resetDemoData()
            return true
        }
        if name.hasPrefix("main.expenses["), name.hasSuffix(".delete_btn"), let index = rowIndex(name)
        {
            let visible = visibleExpenses()
            guard index >= 0 && index < visible.count else
            {
                return true
            }
            confirmDeleteExpense(visible[index].id)
            return true
        }
        return super.handleGeneratedButton(name)
    }

    override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name.hasPrefix("main.expenses["), let index = rowIndex(name)
        {
            let visible = visibleExpenses()
            guard index >= 0 && index < visible.count else
            {
                return true
            }
            if name.hasSuffix(".paid")
            {
                updateExpense(id: visible[index].id, category: nil, paid: value == "true")
                return true
            }
        }
        if name == "main.show_paid"
        {
            showPaidExpenses = value == "true"
            return true
        }
        if name == "main.monthly_limit"
        {
            monthlyLimit = Double(value) ?? monthlyLimit
            return true
        }
        return super.handleGeneratedTextChanged(name, value: value)
    }

    override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name.hasPrefix("main.expenses["), name.hasSuffix(".category"), let index = rowIndex(name)
        {
            let visible = visibleExpenses()
            guard index >= 0 && index < visible.count else
            {
                return true
            }
            updateExpense(id: visible[index].id, category: value.first, paid: nil)
            return true
        }
        if name == "main.currency", let selected = value.first
        {
            currency = selected
            return true
        }
        return super.handleGeneratedSelectionChanged(name, value: value)
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

    private func updateExpense(id: String, category: String?, paid: Bool?)
    {
        guard let index = expenses.firstIndex(where: { $0.id == id }) else
        {
            return
        }
        if let category
        {
            expenses[index].category = category
        }
        if let paid
        {
            expenses[index].paid = paid
        }
    }

    private func confirmDeleteExpense(_ id: String)
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

    private func resetDemoData()
    {
        categories = kDefaultCategories
        currency = kDefaultCurrency
        showPaidExpenses = kDefaultShowPaidExpenses
        monthlyLimit = kDefaultMonthlyLimit
        expenses = seedExpenses()
        nextExpenseNumber = 13
        refreshCurrentView()
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

    private func registerMainChild()
    {
        guard let child = main.child else
        {
            return
        }
        for element in child.elements
        {
            element.frame = Rect(row: main.frame.row + element.frame.row, col: main.frame.col + element.frame.col, width: element.frame.width, height: element.frame.height)
            registerDynamicElement("main." + element.name, element: element, parentFocusHostId: "main")
        }
        main.childFocusOrder = child.elements.filter { $0.focusable }.map { "main." + $0.name }
        if let view = child as? ExpensesView
        {
            guard let host = view.expenses else
            {
                return
            }
            host.childFocusOrder = view.list.focusOrder(hostId: "main.expenses")
            view.list.registerDynamicElements(on: self, hostId: "main.expenses", hostFrame: host.frame)
        }
    }

    private func rowIndex(_ id: String) -> Int?
    {
        guard let open = id.firstIndex(of: "["),
              let close = id[open...].firstIndex(of: "]")
        else
        {
            return nil
        }
        return Int(id[id.index(after: open)..<close])
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
