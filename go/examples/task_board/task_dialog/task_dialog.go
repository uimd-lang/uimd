package task_dialog

import (
	"strings"

	uimd "uimd"
)

const (
	defaultStatus        = "Todo"
	defaultAssignee      = "Ava"
	defaultPriority      = "Med"
	doneStatus           = "Done"
	newTaskTitle         = "New task"
	editTaskTitle        = "Edit task"
	titleRequiredMessage = "Title is required."
)

type TaskDialogResult struct {
	ID          string
	Title       string
	Status      string
	Assignee    string
	Priority    string
	Description string
	Done        bool
}

type TaskDialog struct {
	*TaskDialogUI
	initial *TaskDialogResult
	result  *TaskDialogResult
	closed  bool
}

func NewTaskDialog(task *TaskDialogResult) *TaskDialog {
	dialog := &TaskDialog{TaskDialogUI: NewTaskDialogUI()}
	dialog.SetEventHandler(dialog)
	if task != nil {
		copyTask := *task
		dialog.initial = &copyTask
		dialog.LoadTask(copyTask)
	} else {
		dialog.Header.SetText(newTaskTitle)
		selectComboValue(dialog.ProjectStatus, defaultStatus)
		selectComboValue(dialog.Owner, defaultAssignee)
		selectComboValue(dialog.Priority, defaultPriority)
	}
	return dialog
}

func (dialog *TaskDialog) OnAddBtnClick() {
	dialog.Submit()
}

func (dialog *TaskDialog) OnCancelBtnClick() {
	dialog.Cancel()
}

func (dialog *TaskDialog) Submit() bool {
	if message := dialog.validationError(); message != "" {
		dialog.Message.SetText(message)
		return false
	}
	result := dialog.formResult()
	dialog.result = &result
	dialog.closed = true
	return true
}

func (dialog *TaskDialog) Cancel() {
	dialog.result = nil
	dialog.closed = true
}

func (dialog *TaskDialog) Result() *TaskDialogResult {
	if dialog.result == nil {
		return nil
	}
	result := *dialog.result
	return &result
}

func (dialog *TaskDialog) LoadTask(task TaskDialogResult) {
	dialog.Header.SetText(editTaskTitle)
	dialog.Name.SetValue(task.Title)
	if task.Status == "" {
		task.Status = defaultStatus
	}
	if task.Assignee == "" {
		task.Assignee = defaultAssignee
	}
	if task.Priority == "" {
		task.Priority = defaultPriority
	}
	selectComboValue(dialog.ProjectStatus, task.Status)
	selectComboValue(dialog.Owner, task.Assignee)
	selectComboValue(dialog.Priority, task.Priority)
	dialog.Note.SetValue(task.Description)
}

func (dialog *TaskDialog) formResult() TaskDialogResult {
	status := selectedComboValue(dialog.ProjectStatus, defaultStatus)
	result := TaskDialogResult{
		Title:       strings.TrimSpace(dialog.Name.Value),
		Status:      status,
		Assignee:    selectedComboValue(dialog.Owner, defaultAssignee),
		Priority:    selectedComboValue(dialog.Priority, defaultPriority),
		Description: strings.TrimSpace(dialog.Note.Value),
		Done:        status == doneStatus,
	}
	if dialog.initial != nil {
		result.ID = dialog.initial.ID
	}
	return result
}

func (dialog *TaskDialog) validationError() string {
	if strings.TrimSpace(dialog.Name.Value) == "" {
		return titleRequiredMessage
	}
	return ""
}

func selectComboValue(combo *uimd.ComboBox, value string) {
	for index, option := range combo.Options {
		if option == value {
			combo.SetSelectedIndex(index)
			return
		}
	}
	combo.SetSelectedIndex(0)
}

func selectedComboValue(combo *uimd.ComboBox, fallback string) string {
	index := combo.SelectedIndex
	if index < 0 || index >= len(combo.Options) {
		return fallback
	}
	return combo.Options[index]
}
