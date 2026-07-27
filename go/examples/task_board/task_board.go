package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"

	taskdialog "uimd_examples/task_board/task_dialog"
	taskfilters "uimd_examples/task_board/task_filters"
	tasklist "uimd_examples/task_board/task_list"
	taskrow "uimd_examples/task_board/task_row"

	uimd "uimd"
)

const (
	anyFilter             = "Any"
	defaultAssignee       = "Ava"
	defaultPriority       = "Med"
	defaultStatus         = "Todo"
	doneStatus            = "Done"
	taskIDPrefix          = "t-"
	canceledStatusText    = "Action canceled."
	markAllDoneStatusText = "All tasks marked done."
	clearBoardStatusText  = "Board cleared."
	initialTaskNumberBase = 100
)

type Task struct {
	ID          string
	Title       string
	Status      string
	Assignee    string
	Priority    string
	Description string
	Done        bool
}

type bulkAction int

const (
	bulkNone bulkAction = iota
	bulkMarkAllDone
	bulkClearBoard
)

var seedTasks = []Task{
	{"t-101", "Review invoice export", "Doing", "Ava", "High", "Check exported columns and totals before finance signs off.", false},
	{"t-102", "Rewrite signup emails", "Blocked", "Bruno", "Med", "Waiting for brand review on the onboarding sequence.", false},
	{"t-103", "Improve search ranking", "Doing", "Chen", "High", "Tune ranking for exact title matches and recent activity.", false},
	{"t-104", "Draft data deletion policy", "Todo", "Dana", "Urgent", "Prepare policy draft for privacy review.", false},
	{"t-105", "Update support templates", "Todo", "Ava", "Med", "Refresh billing and account recovery snippets.", false},
	{"t-106", "Fix API contract tests", "Blocked", "Chen", "High", "Coordinate schema change with partner sandbox.", false},
	{"t-107", "Clean left navigation", "Todo", "Bruno", "Low", "Align labels and collapse unused spacing in admin views.", false},
	{"t-108", "Finalize launch checklist", "Done", "Dana", "Med", "Checklist completed and reviewed by release owner.", true},
	{"t-109", "Define calendar sharing", "Doing", "Bruno", "Med", "Document private, team, and organization visibility.", false},
	{"t-110", "Clean knowledge editor", "Todo", "Chen", "Low", "Remove stale toolbar actions and simplify publish flow.", false},
	{"t-111", "Publish usage report", "Doing", "Dana", "Med", "Send account summary to customer success leads.", false},
	{"t-112", "Retry failed imports", "Blocked", "Ava", "Urgent", "Needs import worker patch before rerun.", false},
	{"t-113", "Tune digest settings", "Todo", "Bruno", "Med", "Review default digest cadence for new workspaces.", false},
	{"t-114", "Verify account password reset cleanup", "Done", "Chen", "Low", "Old reset token cleanup verified in staging.", true},
}

type TaskBoardApp struct {
	*TaskBoardUI
	filters             *taskfilters.TaskFilters
	board               *tasklist.TaskList
	stack               uimd.GeneratedWindowStack
	tasks               []Task
	taskDialog          *taskdialog.TaskDialog
	deleteDialog        *uimd.MessageBoxYesNo
	bulkDialog          *uimd.MessageBoxYesNo
	pendingDeleteTaskID string
	pendingBulkAction   bulkAction
	quitRequested       bool
}

func NewTaskBoardApp() *TaskBoardApp {
	app := &TaskBoardApp{
		TaskBoardUI: NewTaskBoardUI(),
		filters:     taskfilters.NewTaskFilters(),
		board:       tasklist.NewTaskList(),
	}
	app.SetEventHandler(app)
	app.Filters.SetChild(app.filters)
	app.Board.SetChild(app.board)
	app.Open()
	return app
}

func (app *TaskBoardApp) Open() {
	app.tasks = append([]Task(nil), seedTasks...)
	app.filters.SetCallbacks(
		func() { app.RefreshBoard() },
		func() {
			app.ResetFilters()
			app.RefreshBoard()
		},
	)
	app.board.SetRowsProvider(func() []taskrow.TaskRowView {
		return app.VisibleRows()
	})
	app.board.SetCallbacks(
		func(id string) { app.OpenTaskByID(id) },
		func(id string) { app.ConfirmDeleteTask(id) },
		func(id string, done bool) { app.SetTaskDone(id, done) },
	)
	app.RefreshBoard()
}

func (app *TaskBoardApp) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.TaskBoardUI.RuntimeOptions()
	options.WindowStack = &app.stack
	options.ShouldClose = app.ShouldClose
	return options
}

func (app *TaskBoardApp) ShouldClose() bool {
	return app.quitRequested || app.GeneratedWindowBase.ShouldClose()
}

func (app *TaskBoardApp) OnMarkAllDoneBtnClick() {
	app.ConfirmMarkAllDone()
}

func (app *TaskBoardApp) OnClearBoardBtnClick() {
	app.ConfirmClearBoard()
}

func (app *TaskBoardApp) OnNewTaskBtnClick() {
	app.OpenTaskDialog(nil)
}

func (app *TaskBoardApp) OnQuitBtnClick() {
	app.quitRequested = true
	app.RequestClose()
}

func (app *TaskBoardApp) TaskCount() int {
	return len(app.tasks)
}

func (app *TaskBoardApp) VisibleCount() int {
	return len(app.MatchingTasks())
}

func (app *TaskBoardApp) VisibleRows() []taskrow.TaskRowView {
	rows := []taskrow.TaskRowView{}
	for _, task := range app.MatchingTasks() {
		rows = append(rows, taskrow.TaskRowView{
			ID:       task.ID,
			Title:    task.Title,
			Status:   task.Status,
			Assignee: task.Assignee,
			Priority: task.Priority,
			Done:     task.Done,
		})
	}
	return rows
}

func (app *TaskBoardApp) MatchingTasks() []*Task {
	query := app.filters.Search.Value
	statusFilter := selectedComboValue(app.filters.StatusFilter, anyFilter)
	assigneeFilter := selectedComboValue(app.filters.OwnerFilter, anyFilter)
	records := []*Task{}
	for index := range app.tasks {
		task := &app.tasks[index]
		haystack := task.Title + " " + task.Description + " " + task.Assignee + " " + task.Priority
		if query != "" && !strings.Contains(strings.ToLower(haystack), strings.ToLower(query)) {
			continue
		}
		if statusFilter != anyFilter && task.Status != statusFilter {
			continue
		}
		if assigneeFilter != anyFilter && task.Assignee != assigneeFilter {
			continue
		}
		records = append(records, task)
	}
	return records
}

func (app *TaskBoardApp) RefreshBoard() {
	app.board.RefreshRows()
	app.Status.SetText(fmt.Sprintf("%d visible / %d total", app.VisibleCount(), len(app.tasks)))
}

func (app *TaskBoardApp) ResetFilters() {
	app.filters.Search.SetValue("")
	app.filters.Search.SetCursor(0)
	selectComboValue(app.filters.StatusFilter, anyFilter)
	selectComboValue(app.filters.OwnerFilter, anyFilter)
}

func (app *TaskBoardApp) ConfirmMarkAllDone() {
	app.OpenBulkActionDialog(bulkMarkAllDone, "Mark All Done", "Mark every task as done?")
}

func (app *TaskBoardApp) ConfirmClearBoard() {
	app.OpenBulkActionDialog(bulkClearBoard, "Clear Board", "Delete every task from the board?")
}

func (app *TaskBoardApp) OpenBulkActionDialog(action bulkAction, title string, message string) {
	app.pendingBulkAction = action
	app.bulkDialog = uimd.NewMessageBoxYesNo(title, message)
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNo",
		InitialFocusName: "no_btn",
		OnButton: func(name string) {
			switch name {
			case "yes_btn":
				app.BulkActionConfirmed(true)
			case "no_btn":
				app.BulkActionConfirmed(false)
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				app.BulkActionConfirmed(false)
				return true
			}
			return false
		},
	}
	app.stack.Push(app.bulkDialog, frame)
}

func (app *TaskBoardApp) BulkActionConfirmed(confirmed bool) {
	action := app.pendingBulkAction
	app.CloseBulkActionDialog()
	if !confirmed {
		app.Status.SetText(canceledStatusText)
		return
	}
	switch action {
	case bulkMarkAllDone:
		app.MarkAllDone()
	case bulkClearBoard:
		app.ClearBoard()
	}
}

func (app *TaskBoardApp) MarkAllDone() {
	for index := range app.tasks {
		app.tasks[index].Done = true
		app.syncDoneStatus(&app.tasks[index])
	}
	app.ResetFilters()
	app.RefreshBoard()
	app.Status.SetText(markAllDoneStatusText)
}

func (app *TaskBoardApp) ClearBoard() {
	app.tasks = nil
	app.ResetFilters()
	app.RefreshBoard()
	app.Status.SetText(clearBoardStatusText)
}

func (app *TaskBoardApp) CloseBulkActionDialog() {
	if app.bulkDialog == nil {
		return
	}
	app.stack.Remove(app.bulkDialog)
	app.bulkDialog = nil
	app.pendingBulkAction = bulkNone
}

func (app *TaskBoardApp) OpenTaskDialog(task *Task) {
	var result *taskdialog.TaskDialogResult
	if task != nil {
		dialogResult := toDialogResult(*task)
		result = &dialogResult
	}
	app.taskDialog = taskdialog.NewTaskDialog(result)
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName: "TaskDialog",
		OnButton: func(name string) {
			if app.taskDialog == nil {
				return
			}
			if name == "add_btn" {
				if app.taskDialog.Submit() {
					app.SaveTask(app.taskDialog.Result())
					app.CloseTaskDialog()
				}
				return
			}
			app.Status.SetText(canceledStatusText)
			app.taskDialog.Cancel()
			app.CloseTaskDialog()
		},
	}
	app.stack.Push(app.taskDialog, frame)
}

func (app *TaskBoardApp) OpenTaskByID(id string) {
	if task := app.TaskByID(id); task != nil {
		app.OpenTaskDialog(task)
	}
}

func (app *TaskBoardApp) ConfirmDeleteTask(id string) {
	task := app.TaskByID(id)
	if task == nil {
		return
	}
	app.pendingDeleteTaskID = task.ID
	app.deleteDialog = uimd.NewMessageBoxYesNo("Delete Task", "Delete "+task.Title+"?")
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNo",
		InitialFocusName: "no_btn",
		OnButton: func(name string) {
			switch name {
			case "yes_btn":
				app.DeleteConfirmed(app.pendingDeleteTaskID, true)
			case "no_btn":
				app.DeleteConfirmed(app.pendingDeleteTaskID, false)
			}
		},
	}
	app.stack.Push(app.deleteDialog, frame)
}

func (app *TaskBoardApp) DeleteConfirmed(id string, confirmed bool) {
	app.CloseDeleteDialog()
	if !confirmed {
		app.Status.SetText(canceledStatusText)
		return
	}
	before := len(app.tasks)
	filtered := app.tasks[:0]
	for _, task := range app.tasks {
		if task.ID != id {
			filtered = append(filtered, task)
		}
	}
	app.tasks = filtered
	if len(app.tasks) != before {
		app.RefreshBoard()
		app.Status.SetText("Task deleted.")
	}
}

func (app *TaskBoardApp) CloseDeleteDialog() {
	if app.deleteDialog == nil {
		return
	}
	app.stack.Remove(app.deleteDialog)
	app.deleteDialog = nil
	app.pendingDeleteTaskID = ""
}

func (app *TaskBoardApp) SetTaskDone(id string, done bool) {
	task := app.TaskByID(id)
	if task == nil {
		return
	}
	task.Done = done
	app.syncDoneStatus(task)
	app.board.SetRowDone(id, done)
	app.Status.SetText("Updated " + task.Title)
}

func (app *TaskBoardApp) CloseTaskDialog() {
	if app.taskDialog == nil {
		return
	}
	app.stack.Remove(app.taskDialog)
	app.taskDialog = nil
}

func (app *TaskBoardApp) SaveTask(result *taskdialog.TaskDialogResult) {
	if result == nil {
		app.Status.SetText(canceledStatusText)
		return
	}
	task := app.TaskByID(result.ID)
	if task == nil {
		app.tasks = append([]Task{{
			ID:       app.NextTaskID(),
			Status:   defaultStatus,
			Assignee: defaultAssignee,
			Priority: defaultPriority,
		}}, app.tasks...)
		task = &app.tasks[0]
		app.ResetFilters()
	}
	task.Title = result.Title
	task.Status = valueOr(result.Status, defaultStatus)
	task.Assignee = valueOr(result.Assignee, defaultAssignee)
	task.Priority = valueOr(result.Priority, defaultPriority)
	task.Description = result.Description
	task.Done = result.Done
	app.syncDoneStatus(task)
	app.RefreshBoard()
	app.Status.SetText("Task saved.")
}

func (app *TaskBoardApp) TaskByID(id string) *Task {
	for index := range app.tasks {
		if app.tasks[index].ID == id {
			return &app.tasks[index]
		}
	}
	return nil
}

func (app *TaskBoardApp) syncDoneStatus(task *Task) {
	if task.Done {
		task.Status = doneStatus
	} else if task.Status == doneStatus {
		task.Status = defaultStatus
	}
}

func (app *TaskBoardApp) NextTaskID() string {
	next := initialTaskNumberBase + 1
	for _, task := range app.tasks {
		number := taskNumber(task.ID)
		if number >= next {
			next = number + 1
		}
	}
	return taskIDPrefix + strconv.Itoa(next)
}

func (app *TaskBoardApp) MCPToolNames() []string {
	return []string{"query_tasks", "get_task", "update_task", "delete_task"}
}

func (app *TaskBoardApp) HandleMCPTool(name string, arguments map[string]any) (any, bool) {
	switch name {
	case "query_tasks":
		return map[string]any{"tasks": app.taskMaps(app.MatchingTasksForTool(arguments))}, true
	case "get_task":
		task := app.TaskByID(stringMapValue(arguments, "id"))
		if task == nil {
			return map[string]any{"task": nil}, true
		}
		return map[string]any{"task": taskMap(*task)}, true
	case "update_task":
		return app.handleUpdateTask(arguments), true
	case "delete_task":
		return map[string]any{"deleted": app.deleteTaskByID(stringMapValue(arguments, "id"))}, true
	}
	return nil, false
}

func (app *TaskBoardApp) MatchingTasksForTool(arguments map[string]any) []*Task {
	query := valueOr(stringMapValue(arguments, "title"), stringMapValue(arguments, "name"))
	statusFilter := valueOr(stringMapValue(arguments, "status"), anyFilter)
	assigneeFilter := valueOr(stringMapValue(arguments, "assignee"), valueOr(stringMapValue(arguments, "owner"), anyFilter))
	priorityFilter := valueOr(stringMapValue(arguments, "priority"), anyFilter)
	doneValue, hasDone := boolMapValue(arguments, "done")
	records := []*Task{}
	for index := range app.tasks {
		task := &app.tasks[index]
		haystack := task.Title + " " + task.Description + " " + task.Assignee + " " + task.Priority
		if query != "" && !strings.Contains(strings.ToLower(haystack), strings.ToLower(query)) {
			continue
		}
		if statusFilter != anyFilter && task.Status != statusFilter {
			continue
		}
		if assigneeFilter != anyFilter && task.Assignee != assigneeFilter {
			continue
		}
		if priorityFilter != anyFilter && task.Priority != priorityFilter {
			continue
		}
		if hasDone && task.Done != doneValue {
			continue
		}
		records = append(records, task)
	}
	return records
}

func (app *TaskBoardApp) handleUpdateTask(arguments map[string]any) any {
	task := app.TaskByID(stringMapValue(arguments, "id"))
	if task == nil {
		return map[string]any{"task": nil}
	}
	if value := stringMapValue(arguments, "title"); value != "" {
		task.Title = value
	}
	if value, ok := arguments["status"].(string); ok {
		task.Status = valueOr(value, defaultStatus)
	}
	if value, ok := arguments["assignee"].(string); ok {
		task.Assignee = valueOr(value, defaultAssignee)
	}
	if value, ok := arguments["priority"].(string); ok {
		task.Priority = valueOr(value, defaultPriority)
	}
	if value, ok := arguments["description"].(string); ok {
		task.Description = value
	}
	if value, ok := boolMapValue(arguments, "done"); ok {
		task.Done = value
	}
	app.syncDoneStatus(task)
	app.RefreshBoard()
	return map[string]any{"task": taskMap(*task)}
}

func (app *TaskBoardApp) deleteTaskByID(id string) bool {
	before := len(app.tasks)
	filtered := app.tasks[:0]
	for _, task := range app.tasks {
		if task.ID != id {
			filtered = append(filtered, task)
		}
	}
	app.tasks = filtered
	deleted := len(app.tasks) != before
	if deleted {
		app.RefreshBoard()
		app.Status.SetText("Task deleted.")
	}
	return deleted
}

func (app *TaskBoardApp) taskMaps(tasks []*Task) []map[string]any {
	result := []map[string]any{}
	for _, task := range tasks {
		result = append(result, taskMap(*task))
	}
	return result
}

func taskMap(task Task) map[string]any {
	return map[string]any{
		"id":          task.ID,
		"title":       task.Title,
		"status":      task.Status,
		"assignee":    task.Assignee,
		"priority":    task.Priority,
		"description": task.Description,
		"done":        task.Done,
	}
}

func toDialogResult(task Task) taskdialog.TaskDialogResult {
	return taskdialog.TaskDialogResult{
		ID:          task.ID,
		Title:       task.Title,
		Status:      task.Status,
		Assignee:    task.Assignee,
		Priority:    task.Priority,
		Description: task.Description,
		Done:        task.Done,
	}
}

func taskNumber(id string) int {
	if !strings.HasPrefix(id, taskIDPrefix) {
		return initialTaskNumberBase
	}
	number, err := strconv.Atoi(strings.TrimPrefix(id, taskIDPrefix))
	if err != nil {
		return initialTaskNumberBase
	}
	return number
}

func selectedComboValue(combo *uimd.ComboBox, fallback string) string {
	index := combo.SelectedIndex
	if index < 0 || index >= len(combo.Options) {
		return fallback
	}
	return combo.Options[index]
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

func valueOr(value string, fallback string) string {
	if value == "" {
		return fallback
	}
	return value
}

func stringMapValue(values map[string]any, key string) string {
	if values == nil {
		return ""
	}
	value, _ := values[key].(string)
	return value
}

func boolMapValue(values map[string]any, key string) (bool, bool) {
	if values == nil {
		return false, false
	}
	value, ok := values[key].(bool)
	return value, ok
}

func smoke() {
	app := NewTaskBoardApp()
	if app.GeneratedWindowBase.Title() != "Task Board" || app.Title == nil || app.QuitBtn == nil ||
		app.Filters == nil || app.NewTaskBtn == nil || app.MarkAllDoneBtn == nil ||
		app.ClearBoardBtn == nil || app.Board == nil || app.Status == nil {
		panic("task board smoke failed")
	}
}

func logicTest() {
	app := NewTaskBoardApp()
	if app.TaskCount() != len(seedTasks) || app.VisibleCount() != len(seedTasks) {
		panic("seed tasks failed")
	}
	app.filters.Search.SetValue("calendar")
	app.RefreshBoard()
	if app.VisibleCount() != 1 {
		panic("filter failed")
	}
	app.ResetFilters()
	app.RefreshBoard()
	app.SaveTask(&taskdialog.TaskDialogResult{
		Title:       "Write C++ task board",
		Status:      defaultStatus,
		Assignee:    defaultAssignee,
		Priority:    defaultPriority,
		Description: "Port task board example.",
	})
	if app.TaskCount() != len(seedTasks)+1 || app.Status.Text != "Task saved." {
		panic("save task failed")
	}
	app.OpenTaskDialog(nil)
	if app.stack.Empty() {
		panic("dialog did not open")
	}
	app.OnQuitBtnClick()
	if !app.ShouldClose() {
		panic("quit failed")
	}
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		if len(os.Args) > 1 {
			switch os.Args[1] {
			case "--smoke":
				smoke()
				return 0
			case "--logic-test":
				logicTest()
				return 0
			}
		}
		app := NewTaskBoardApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
