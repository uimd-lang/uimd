package expenses_list

import (
	expenserow "uimd_examples/expense_tracker/expense_row"

	uimd "uimd"
)

type DeleteCallback func(string)
type ChangeCallback func(string, string, bool)

type ExpensesList struct {
	*ExpensesListUI
	Panel      *uimd.ScrollView
	rows       []expenserow.ExpenseRowView
	categories []string
	onDelete   DeleteCallback
	onChange   ChangeCallback
}

func NewExpensesList() *ExpensesList {
	list := &ExpensesList{ExpensesListUI: NewExpensesListUI()}
	list.Panel = list.NewGeneratedScrollViewElement()
	list.AddElement(list.Panel)
	list.Panel.SetAutoScroll(false)
	return list
}

func (list *ExpensesList) SetRows(rows []expenserow.ExpenseRowView, categories []string) {
	list.rows = append([]expenserow.ExpenseRowView{}, rows...)
	list.categories = append([]string{}, categories...)
	list.RefreshRows()
}

func (list *ExpensesList) SetCallbacks(onDelete DeleteCallback, onChange ChangeCallback) {
	list.onDelete = onDelete
	list.onChange = onChange
	list.RefreshRows()
}

func (list *ExpensesList) RefreshRows() {
	list.Panel.ClearChildren()
	for _, row := range list.rows {
		rowWindow := expenserow.NewExpenseRow(
			row,
			list.categories,
			func(id string) {
				if list.onDelete != nil {
					list.onDelete(id)
				}
			},
			func(id string, category string, paid bool) {
				if list.onChange != nil {
					list.onChange(id, category, paid)
				}
			},
		)
		reusable := uimd.NewReusableElement("row", "expense_row")
		reusable.SetChild(rowWindow)
		naturalSize := uimd.GeneratedWindowContentSize(rowWindow)
		reusable.SetFrame(uimd.Rect{Width: naturalSize.Width, Height: naturalSize.Height})
		list.Panel.AddChild(reusable)
	}
	list.Panel.ViewOffset = 0
}
