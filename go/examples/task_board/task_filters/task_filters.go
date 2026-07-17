package task_filters

type Callback func()

type TaskFilters struct {
	*TaskFiltersUI
	onApply Callback
	onReset Callback
}

func NewTaskFilters() *TaskFilters {
	filters := &TaskFilters{TaskFiltersUI: NewTaskFiltersUI()}
	filters.SetEventHandler(filters)
	return filters
}

func (filters *TaskFilters) SetCallbacks(onApply Callback, onReset Callback) {
	filters.onApply = onApply
	filters.onReset = onReset
}

func (filters *TaskFilters) OnApplyFiltersBtnClick() {
	if filters.onApply != nil {
		filters.onApply()
	}
}

func (filters *TaskFilters) OnResetFiltersBtnClick() {
	if filters.onReset != nil {
		filters.onReset()
	}
}
