package task_row

type OpenCallback func(string)
type DeleteCallback func(string)
type DoneCallback func(string, bool)

type TaskRowView struct {
	ID       string
	Title    string
	Status   string
	Assignee string
	Priority string
	Done     bool
}

type TaskRow struct {
	*TaskRowUI
	row      TaskRowView
	onOpen   OpenCallback
	onDelete DeleteCallback
	onDone   DoneCallback
}

func NewTaskRow(row TaskRowView, onOpen OpenCallback, onDelete DeleteCallback, onDone DoneCallback) *TaskRow {
	view := &TaskRow{
		TaskRowUI: NewTaskRowUI(),
		row:       row,
		onOpen:    onOpen,
		onDelete:  onDelete,
		onDone:    onDone,
	}
	view.SetEventHandler(view)
	view.TaskTitle.SetText(row.Title)
	view.Meta.SetText(row.Status + " / " + row.Assignee + " / " + row.Priority)
	view.Done.SetChecked(row.Done)
	return view
}

func (row *TaskRow) ID() string {
	return row.row.ID
}

func (row *TaskRow) OnOpenBtnClick() {
	if row.onOpen != nil {
		row.onOpen(row.row.ID)
	}
}

func (row *TaskRow) OnDeleteBtnClick() {
	if row.onDelete != nil {
		row.onDelete(row.row.ID)
	}
}

func (row *TaskRow) OnDoneChange(string) {
	if row.onDone != nil {
		row.onDone(row.row.ID, row.Done.Checked)
	}
}
