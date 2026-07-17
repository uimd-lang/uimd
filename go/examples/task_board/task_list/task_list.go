package task_list

import (
	taskrow "uimd_examples/task_board/task_row"

	uimd "uimd"
)

type RowsProvider func() []taskrow.TaskRowView
type OpenCallback func(string)
type DeleteCallback func(string)
type DoneCallback func(string, bool)

const taskListMinimumRenderHeight = 1

type TaskList struct {
	*TaskListUI
	Panel        *uimd.ScrollView
	rowsProvider RowsProvider
	onOpen       OpenCallback
	onDelete     DeleteCallback
	onDone       DoneCallback
}

func NewTaskList() *TaskList {
	list := &TaskList{TaskListUI: NewTaskListUI()}
	list.Panel = list.NewGeneratedScrollViewElement()
	list.AddElement(list.Panel)
	return list
}

func (list *TaskList) SetRowsProvider(provider RowsProvider) {
	list.rowsProvider = provider
	list.RefreshRows()
}

func (list *TaskList) SetCallbacks(onOpen OpenCallback, onDelete DeleteCallback, onDone DoneCallback) {
	list.onOpen = onOpen
	list.onDelete = onDelete
	list.onDone = onDone
	list.RefreshRows()
}

func (list *TaskList) RefreshRows() {
	list.Panel.ClearChildren()
	if list.rowsProvider == nil {
		return
	}
	rows := list.rowsProvider()
	for _, row := range rows {
		rowWindow := taskrow.NewTaskRow(
			row,
			func(id string) {
				if list.onOpen != nil {
					list.onOpen(id)
				}
			},
			func(id string) {
				if list.onDelete != nil {
					list.onDelete(id)
				}
			},
			func(id string, done bool) {
				if list.onDone != nil {
					list.onDone(id, done)
				}
			},
		)
		reusable := uimd.NewReusableElement("row", "task_row")
		reusable.SetChild(rowWindow)
		naturalSize := uimd.GeneratedWindowContentSize(rowWindow)
		reusable.SetFrame(uimd.Rect{Height: maxInt(taskListMinimumRenderHeight, naturalSize.Height)})
		list.Panel.AddChild(reusable)
	}
	list.Panel.ViewOffset = 0
}

func (list *TaskList) SetRowDone(id string, done bool) {
	for _, child := range list.Panel.Children {
		reusable, ok := child.(*uimd.ReusableElement)
		if !ok {
			continue
		}
		row, ok := reusable.ChildObject.(*taskrow.TaskRow)
		if !ok || row.ID() != id {
			continue
		}
		row.Done.SetChecked(done)
		return
	}
}

func maxInt(lhs int, rhs int) int {
	if lhs > rhs {
		return lhs
	}
	return rhs
}
