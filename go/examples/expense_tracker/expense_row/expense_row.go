package expense_row

import (
	"fmt"

	uimd "uimd"
)

type DeleteCallback func(string)
type ChangeCallback func(string, string, bool)

type ExpenseRowView struct {
	ID       string
	Item     string
	Amount   float64
	Category string
	Paid     bool
}

type ExpenseRow struct {
	*ExpenseRowUI
	row        ExpenseRowView
	categories []string
	onDelete   DeleteCallback
	onChange   ChangeCallback
}

func NewExpenseRow(row ExpenseRowView, categories []string, onDelete DeleteCallback, onChange ChangeCallback) *ExpenseRow {
	view := &ExpenseRow{
		ExpenseRowUI: NewExpenseRowUI(),
		row:          row,
		categories:   append([]string{}, categories...),
		onDelete:     onDelete,
		onChange:     onChange,
	}
	view.SetEventHandler(view)
	view.syncFromRow()
	return view
}

func (row *ExpenseRow) ID() string {
	return row.row.ID
}

func (row *ExpenseRow) OnPaidChange(string) {
	row.row.Paid = row.Paid.Checked
	if row.onChange != nil {
		row.onChange(row.row.ID, selectedComboValue(row.Category), row.row.Paid)
	}
}

func (row *ExpenseRow) OnCategoryChange(value string) {
	row.row.Category = value
	if row.onChange != nil {
		row.onChange(row.row.ID, row.row.Category, row.Paid.Checked)
	}
}

func (row *ExpenseRow) OnDeleteBtnClick() {
	if row.onDelete != nil {
		row.onDelete(row.row.ID)
	}
}

func (row *ExpenseRow) syncFromRow() {
	row.Item.SetText(row.row.Item)
	row.Amount.SetText(fmt.Sprintf("%.2f", row.row.Amount))
	row.Paid.SetChecked(row.row.Paid)
	row.Category.Options = append([]string{}, row.categories...)
	selectComboValue(row.Category, row.row.Category)
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

func selectedComboValue(comboBox *uimd.ComboBox) string {
	if comboBox == nil || comboBox.SelectedIndex < 0 || comboBox.SelectedIndex >= len(comboBox.Options) {
		return ""
	}
	return comboBox.Options[comboBox.SelectedIndex]
}
