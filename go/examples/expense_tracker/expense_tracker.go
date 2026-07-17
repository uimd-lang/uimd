package main

import (
	"os"
	"strconv"
	"strings"

	expenserow "uimd_examples/expense_tracker/expense_row"
	expenselist "uimd_examples/expense_tracker/expenses_list"

	uimd "uimd"
)

const (
	defaultCurrency                 = "EUR"
	defaultShowPaidExpenses         = true
	defaultMonthlyLimit     float64 = 500
	untitledExpense                 = "Untitled expense"
	expenseIDPrefix                 = "e-"
)

var defaultCategories = []string{"Food", "Transport", "Home", "Health", "Other"}

var seedExpenses = []Expense{
	{"e-1", "Groceries", 42.80, "Food", true},
	{"e-2", "Bus ticket", 2.50, "Transport", true},
	{"e-3", "Light bulbs", 8.90, "Home", false},
	{"e-4", "Pharmacy", 14.20, "Health", true},
	{"e-5", "Notebook", 5.40, "Other", false},
	{"e-6", "Internet bill", 29.99, "Home", true},
	{"e-7", "Team lunch", 18.50, "Food", false},
	{"e-8", "Train pass", 24.00, "Transport", true},
	{"e-9", "Vitamins", 11.75, "Health", false},
	{"e-10", "Desk lamp", 21.30, "Home", false},
	{"e-11", "Cloud storage", 9.99, "Other", true},
	{"e-12", "Replacement keyboard", 44.00, "Other", false},
}

type Expense struct {
	ID       string
	Item     string
	Amount   float64
	Category string
	Paid     bool
}

type ExpensesView struct {
	*ExpensesViewUI
	shell *ExpenseTrackerApp
	list  *expenselist.ExpensesList
}

func NewExpensesView(shell *ExpenseTrackerApp) *ExpensesView {
	view := &ExpensesView{
		ExpensesViewUI: NewExpensesViewUI(),
		shell:          shell,
		list:           expenselist.NewExpensesList(),
	}
	view.SetEventHandler(view)
	view.Expenses.SetChild(view.list)
	view.Refresh()
	return view
}

func (view *ExpensesView) Refresh() {
	view.CurrencyLabel.SetText(view.shell.currency)
	view.CategoryInput.Options = append([]string{}, view.shell.categories...)
	selectComboValue(view.CategoryInput, selectedComboValue(view.CategoryInput))
	view.list.SetRows(view.shell.ExpenseRows(), view.shell.categories)
	view.list.SetCallbacks(
		func(id string) {
			view.shell.ConfirmDeleteExpense(id)
		},
		func(id string, category string, paid bool) {
			view.shell.UpdateExpense(id, category, paid)
		},
	)
}

func (view *ExpensesView) OnAddExpenseBtnClick() {
	view.shell.AddExpense(
		strings.TrimSpace(view.ItemInput.Value),
		view.AmountInput.NumberValue,
		selectedComboValue(view.CategoryInput),
	)
	view.ItemInput.SetValue("")
	view.ItemInput.SetCursor(0)
	view.AmountInput.SetValue(0)
	view.Refresh()
}

type CategoriesView struct {
	*CategoriesViewUI
	shell *ExpenseTrackerApp
}

func NewCategoriesView(shell *ExpenseTrackerApp) *CategoriesView {
	view := &CategoriesView{
		CategoriesViewUI: NewCategoriesViewUI(),
		shell:            shell,
	}
	view.SetEventHandler(view)
	view.Refresh("")
	return view
}

func (view *CategoriesView) Refresh(selected string) {
	view.Categories.SetOptions(view.shell.categories)
	if len(view.shell.categories) == 0 {
		view.Categories.SetSelectedItems(nil)
		return
	}
	view.Categories.SetSelectedIndex(0)
	if selected != "" {
		view.Categories.SetSelectedItems([]string{selected})
	}
}

func (view *CategoriesView) OnAddCategoryBtnClick() {
	added := view.shell.AddCategory(strings.TrimSpace(view.CategoryName.Value))
	view.CategoryName.SetValue("")
	view.CategoryName.SetCursor(0)
	view.Refresh(added)
}

func (view *CategoriesView) OnRemoveCategoryBtnClick() {
	values := view.Categories.SelectedValues()
	selected := ""
	if len(values) > 0 {
		selected = values[0]
	}
	view.shell.RemoveCategory(selected)
	view.Refresh("")
}

type SettingsView struct {
	*SettingsViewUI
	shell *ExpenseTrackerApp
}

func NewSettingsView(shell *ExpenseTrackerApp) *SettingsView {
	view := &SettingsView{
		SettingsViewUI: NewSettingsViewUI(),
		shell:          shell,
	}
	view.SetEventHandler(view)
	view.Refresh()
	return view
}

func (view *SettingsView) Refresh() {
	selectComboValue(view.Currency, view.shell.currency)
	view.ShowPaid.SetChecked(view.shell.showPaidExpenses)
	view.MonthlyLimit.SetValue(view.shell.monthlyLimit)
}

func (view *SettingsView) OnResetBtnClick() {
	view.shell.ResetDemoData()
}

func (view *SettingsView) OnCurrencyChange(value string) {
	view.shell.SetCurrency(value)
}

func (view *SettingsView) OnShowPaidChange(value string) {
	parsed, err := strconv.ParseBool(value)
	if err == nil {
		view.shell.SetShowPaidExpenses(parsed)
	}
}

func (view *SettingsView) OnMonthlyLimitChange(value string) {
	view.shell.SetMonthlyLimit(parseFloat(value, view.shell.monthlyLimit))
}

func (view *SettingsView) OnMonthlyLimitSubmit(value string) {
	view.shell.SetMonthlyLimit(parseFloat(value, view.shell.monthlyLimit))
}

type ExpenseTrackerApp struct {
	*ExpenseTrackerUI
	stack             uimd.GeneratedWindowStack
	categories        []string
	expenses          []Expense
	currency          string
	showPaidExpenses  bool
	monthlyLimit      float64
	nextExpenseNumber int
	deleteDialog      *uimd.MessageBoxYesNo
	pendingDeleteID   string
	quitRequested     bool
	normalNavStyle    uimd.Style
	activeNavStyle    uimd.Style
}

func NewExpenseTrackerApp() *ExpenseTrackerApp {
	app := &ExpenseTrackerApp{
		ExpenseTrackerUI:  NewExpenseTrackerUI(),
		categories:        append([]string{}, defaultCategories...),
		expenses:          cloneExpenses(seedExpenses),
		currency:          defaultCurrency,
		showPaidExpenses:  defaultShowPaidExpenses,
		monthlyLimit:      defaultMonthlyLimit,
		nextExpenseNumber: len(seedExpenses) + 1,
	}
	app.SetEventHandler(app)
	app.normalNavStyle = app.ExpensesBtn.EffectiveStyle(false, false)
	app.activeNavStyle = app.ExpensesBtn.EffectiveStyle(true, false)
	app.ShowExpenses()
	return app
}

func (app *ExpenseTrackerApp) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.ExpenseTrackerUI.RuntimeOptions()
	options.WindowStack = &app.stack
	options.InitialFocusName = "expenses_btn"
	options.ShouldClose = app.ShouldClose
	return options
}

func (app *ExpenseTrackerApp) ShouldClose() bool {
	return app.quitRequested || app.GeneratedWindowBase.ShouldClose()
}

func (app *ExpenseTrackerApp) OnExpensesBtnClick() {
	app.ShowExpenses()
}

func (app *ExpenseTrackerApp) OnCategoriesBtnClick() {
	app.ShowCategories()
}

func (app *ExpenseTrackerApp) OnSettingsBtnClick() {
	app.ShowSettings()
}

func (app *ExpenseTrackerApp) OnQuitBtnClick() {
	app.quitRequested = true
	app.RequestClose()
}

func (app *ExpenseTrackerApp) ShowExpenses() {
	app.selectNavButton(app.ExpensesBtn)
	app.Main.SetView(NewExpensesView(app))
}

func (app *ExpenseTrackerApp) ShowCategories() {
	app.selectNavButton(app.CategoriesBtn)
	app.Main.SetView(NewCategoriesView(app))
}

func (app *ExpenseTrackerApp) ShowSettings() {
	app.selectNavButton(app.SettingsBtn)
	app.Main.SetView(NewSettingsView(app))
}

func (app *ExpenseTrackerApp) SetCurrency(currency string) {
	if currency == "" {
		currency = defaultCurrency
	}
	app.currency = currency
	app.RefreshExpensesView()
}

func (app *ExpenseTrackerApp) SetShowPaidExpenses(value bool) {
	app.showPaidExpenses = value
	app.RefreshExpensesView()
}

func (app *ExpenseTrackerApp) SetMonthlyLimit(value float64) {
	app.monthlyLimit = value
}

func (app *ExpenseTrackerApp) ExpenseRows() []expenserow.ExpenseRowView {
	rows := []expenserow.ExpenseRowView{}
	for _, expense := range app.expenses {
		if !app.showPaidExpenses && expense.Paid {
			continue
		}
		rows = append(rows, expenserow.ExpenseRowView{
			ID:       expense.ID,
			Item:     expense.Item,
			Amount:   expense.Amount,
			Category: expense.Category,
			Paid:     expense.Paid,
		})
	}
	return rows
}

func (app *ExpenseTrackerApp) AddExpense(item string, amount float64, category string) {
	if item == "" {
		item = untitledExpense
	}
	if !containsString(app.categories, category) {
		category = firstString(app.categories)
	}
	app.expenses = append([]Expense{{
		ID:       expenseIDPrefix + strconv.Itoa(app.nextExpenseNumber),
		Item:     item,
		Amount:   amount,
		Category: category,
		Paid:     false,
	}}, app.expenses...)
	app.nextExpenseNumber++
}

func (app *ExpenseTrackerApp) UpdateExpense(id string, category string, paid bool) {
	for index := range app.expenses {
		if app.expenses[index].ID != id {
			continue
		}
		app.expenses[index].Category = category
		app.expenses[index].Paid = paid
		return
	}
}

func (app *ExpenseTrackerApp) ConfirmDeleteExpense(id string) {
	app.pendingDeleteID = id
	item := "expense"
	if expense := app.expenseByID(id); expense != nil {
		item = expense.Item
	}
	app.deleteDialog = uimd.NewMessageBoxYesNo("Delete Expense", "Delete "+item+"?")
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNo",
		InitialFocusName: "no_btn",
		OnButton: func(name string) {
			if name == "yes_btn" {
				app.closeDeleteDialog("yes")
			} else if name == "no_btn" {
				app.closeDeleteDialog("no")
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				app.closeDeleteDialog("no")
				return true
			}
			return false
		},
	}
	app.stack.Push(app.deleteDialog, frame)
}

func (app *ExpenseTrackerApp) DeleteExpense(id string, result string) {
	if result != "yes" {
		return
	}
	filtered := app.expenses[:0]
	for _, expense := range app.expenses {
		if expense.ID != id {
			filtered = append(filtered, expense)
		}
	}
	app.expenses = filtered
	app.RefreshExpensesView()
}

func (app *ExpenseTrackerApp) AddCategory(name string) string {
	if name == "" {
		return ""
	}
	if !containsString(app.categories, name) {
		app.categories = append(app.categories, name)
	}
	return name
}

func (app *ExpenseTrackerApp) RemoveCategory(name string) {
	if name == "" || len(app.categories) <= 1 || !containsString(app.categories, name) {
		return
	}
	nextCategories := app.categories[:0]
	for _, category := range app.categories {
		if category != name {
			nextCategories = append(nextCategories, category)
		}
	}
	app.categories = nextCategories
	fallback := firstString(app.categories)
	for index := range app.expenses {
		if app.expenses[index].Category == name {
			app.expenses[index].Category = fallback
		}
	}
}

func (app *ExpenseTrackerApp) ResetDemoData() {
	app.categories = append([]string{}, defaultCategories...)
	app.expenses = cloneExpenses(seedExpenses)
	app.currency = defaultCurrency
	app.showPaidExpenses = defaultShowPaidExpenses
	app.monthlyLimit = defaultMonthlyLimit
	app.nextExpenseNumber = len(app.expenses) + 1
	app.RefreshCurrentView()
}

func (app *ExpenseTrackerApp) RefreshExpensesView() {
	if view, ok := app.Main.CurrentView().(*ExpensesView); ok {
		view.Refresh()
	}
}

func (app *ExpenseTrackerApp) RefreshCurrentView() {
	switch view := app.Main.CurrentView().(type) {
	case *ExpensesView:
		view.Refresh()
	case *CategoriesView:
		view.Refresh("")
	case *SettingsView:
		view.Refresh()
	}
}

func (app *ExpenseTrackerApp) closeDeleteDialog(result string) {
	if app.deleteDialog != nil {
		app.stack.Remove(app.deleteDialog)
	}
	id := app.pendingDeleteID
	app.deleteDialog = nil
	app.pendingDeleteID = ""
	app.DeleteExpense(id, result)
}

func (app *ExpenseTrackerApp) expenseByID(id string) *Expense {
	for index := range app.expenses {
		if app.expenses[index].ID == id {
			return &app.expenses[index]
		}
	}
	return nil
}

func (app *ExpenseTrackerApp) selectNavButton(target *uimd.Button) {
	app.ExpensesBtn.SetStyle(app.normalNavStyle.Clone())
	app.CategoriesBtn.SetStyle(app.normalNavStyle.Clone())
	app.SettingsBtn.SetStyle(app.normalNavStyle.Clone())
	target.SetStyle(app.activeNavStyle.Clone())
}

func cloneExpenses(source []Expense) []Expense {
	result := make([]Expense, len(source))
	copy(result, source)
	return result
}

func containsString(values []string, target string) bool {
	for _, value := range values {
		if value == target {
			return true
		}
	}
	return false
}

func firstString(values []string) string {
	if len(values) == 0 {
		return ""
	}
	return values[0]
}

func selectedComboValue(comboBox *uimd.ComboBox) string {
	if comboBox == nil || comboBox.SelectedIndex < 0 || comboBox.SelectedIndex >= len(comboBox.Options) {
		return ""
	}
	return comboBox.Options[comboBox.SelectedIndex]
}

func selectComboValue(comboBox *uimd.ComboBox, value string) {
	if comboBox == nil {
		return
	}
	for index, option := range comboBox.Options {
		if option == value {
			comboBox.SetSelectedIndex(index)
			return
		}
	}
	comboBox.SetSelectedIndex(0)
}

func parseFloat(value string, fallback float64) float64 {
	parsed, err := strconv.ParseFloat(value, 64)
	if err != nil {
		return fallback
	}
	return parsed
}

func runLogicTest() int {
	app := NewExpenseTrackerApp()
	if _, ok := app.Main.CurrentView().(*ExpensesView); !ok {
		return 1
	}
	app.AddExpense("Coffee", 3.5, "Food")
	if len(app.expenses) == 0 || app.expenses[0].Item != "Coffee" {
		return 1
	}
	app.DeleteExpense(app.expenses[0].ID, "yes")
	if len(app.expenses) == 0 || app.expenses[0].Item == "Coffee" {
		return 1
	}
	app.ShowCategories()
	if _, ok := app.Main.CurrentView().(*CategoriesView); !ok {
		return 1
	}
	app.ShowSettings()
	if _, ok := app.Main.CurrentView().(*SettingsView); !ok {
		return 1
	}
	app.SetCurrency("USD")
	if app.currency != "USD" {
		return 1
	}
	app.SetShowPaidExpenses(false)
	for _, row := range app.ExpenseRows() {
		if row.Paid {
			return 1
		}
	}
	app.SetMonthlyLimit(750)
	if app.monthlyLimit != 750 {
		return 1
	}
	return 0
}

func main() {
	for _, arg := range os.Args[1:] {
		if arg == "--logic-test" || arg == "--smoke" {
			os.Exit(runLogicTest())
		}
	}
	app := NewExpenseTrackerApp()
	os.Exit(uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args))
}
