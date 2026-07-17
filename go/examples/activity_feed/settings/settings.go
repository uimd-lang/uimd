package settings

type SettingsResult struct {
	AutoScroll     bool
	ShowTimestamps bool
	DefaultType    string
}

type SettingsDialog struct {
	UI   *SettingsUI
	open bool
}

func NewSettingsDialog() *SettingsDialog {
	return &SettingsDialog{UI: NewSettingsUI()}
}

func (dialog *SettingsDialog) Configure(result SettingsResult) {
	dialog.UI.AutoScroll.SetChecked(result.AutoScroll)
	dialog.UI.ShowTimestamps.SetChecked(result.ShowTimestamps)
	dialog.selectDefaultType(result.DefaultType)
}

func (dialog *SettingsDialog) Open() {
	dialog.open = true
}

func (dialog *SettingsDialog) Close() {
	dialog.open = false
}

func (dialog *SettingsDialog) Result() SettingsResult {
	return SettingsResult{
		AutoScroll:     dialog.UI.AutoScroll.Checked,
		ShowTimestamps: dialog.UI.ShowTimestamps.Checked,
		DefaultType:    dialog.selectedDefaultType(),
	}
}

func (dialog *SettingsDialog) selectDefaultType(value string) {
	for index, option := range dialog.UI.DefaultType.Options {
		if option == value {
			dialog.UI.DefaultType.SetSelectedIndex(index)
			return
		}
	}
	dialog.UI.DefaultType.SetSelectedIndex(0)
}

func (dialog *SettingsDialog) selectedDefaultType() string {
	index := dialog.UI.DefaultType.SelectedIndex
	if index < 0 || index >= len(dialog.UI.DefaultType.Options) {
		return "Info"
	}
	return dialog.UI.DefaultType.Options[index]
}
