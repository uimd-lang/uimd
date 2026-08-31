package main

import (
	"fmt"
	"os"
	"strings"
	"time"

	activityfeedpanel "uimd_examples/activity_feed/activity_feed_panel"
	settingsdialog "uimd_examples/activity_feed/settings"

	uimd "uimd"
)

const (
	defaultActivityType = "Info"
	timestampEnv        = "UIMD_ACTIVITY_FEED_TIMESTAMP"
)

type sampleActivity struct {
	eventType string
	message   string
}

var sampleActivities = []sampleActivity{
	{"Info", "Workspace opened"},
	{"Task", "Report export queued"},
	{"Deploy", "Version 1.4 deployed"},
	{"Warning", "Review queue is almost full"},
	{"Note", "Follow-up scheduled"},
	{"Task", "Background sync finished"},
}

const initialSampleCount = 2

type ActivityFeedApp struct {
	*ActivityFeedUI
	feed             *activityfeedpanel.ActivityFeedPanel
	settingsDialog   *settingsdialog.SettingsDialog
	stack            uimd.GeneratedWindowStack
	autoScroll       bool
	showTimestamps   bool
	quitRequested    bool
	defaultType      string
	settingsWasShown bool
}

func NewActivityFeedApp() *ActivityFeedApp {
	panel := activityfeedpanel.NewActivityFeedPanel()
	app := &ActivityFeedApp{
		ActivityFeedUI: NewActivityFeedUI(),
		feed:           panel,
		settingsDialog: settingsdialog.NewSettingsDialog(),
		autoScroll:     true,
		showTimestamps: true,
		defaultType:    defaultActivityType,
	}
	app.SetEventHandler(app)
	app.Feed.SetChild(panel)
	return app
}

func (app *ActivityFeedApp) Open() {
	selectComboValue(app.ActivityType, app.defaultType)
	uimd.RenderGeneratedWindowContent(app.GeneratedWindowBase, uimd.Size{Width: 100, Height: 32}, -1, nil, false)
	app.feed.SetAutoScroll(app.autoScroll)
	if app.feed.ActivityCount() == 0 {
		app.seedInitial()
	}
}

func (app *ActivityFeedApp) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.ActivityFeedUI.RuntimeOptions()
	options.WindowStack = &app.stack
	options.ShouldClose = app.ShouldClose
	return options
}

func (app *ActivityFeedApp) ShouldClose() bool {
	return app.quitRequested || app.GeneratedWindowBase.ShouldClose()
}

func (app *ActivityFeedApp) OnAddButtonClick() {
	app.AddCurrentActivity()
}

func (app *ActivityFeedApp) OnSeedButtonClick() {
	app.SeedActivities()
}

func (app *ActivityFeedApp) OnClearButtonClick() {
	app.ClearActivities()
}

func (app *ActivityFeedApp) OnSettingsButtonClick() {
	app.OpenSettings()
}

func (app *ActivityFeedApp) OnQuitButtonClick() {
	app.quitRequested = true
	app.RequestClose()
}

func (app *ActivityFeedApp) OnMessageSubmit(value string) {
	app.Message.SetValue(value)
}

func (app *ActivityFeedApp) AddCurrentActivity() {
	message := strings.TrimSpace(app.Message.Value)
	if message == "" {
		app.Status.SetText("Enter an activity message.")
		return
	}
	app.AppendActivity(selectedComboValue(app.ActivityType, app.defaultType), message)
	app.Message.SetValue("")
	app.Message.SetCursor(0)
	app.updateCountStatus()
}

func (app *ActivityFeedApp) AppendActivity(eventType string, message string) {
	app.feed.AppendActivity(currentTimestamp(), eventType, message, app.showTimestamps)
}

func (app *ActivityFeedApp) SeedActivities() {
	for _, activity := range sampleActivities {
		app.AppendActivity(activity.eventType, activity.message)
	}
	app.updateCountStatus()
}

func (app *ActivityFeedApp) ClearActivities() {
	app.feed.ClearActivities()
	app.Status.SetText("Feed cleared")
}

func (app *ActivityFeedApp) ActivityCount() int {
	return app.feed.ActivityCount()
}

func (app *ActivityFeedApp) HasModalOverlay() bool {
	return !app.stack.Empty()
}

func (app *ActivityFeedApp) OpenSettings() {
	app.settingsDialog.Configure(settingsdialog.SettingsResult{
		AutoScroll:     app.autoScroll,
		ShowTimestamps: app.showTimestamps,
		DefaultType:    app.defaultType,
	})
	app.settingsDialog.Open()
	app.settingsDialog.OnCancel = func() {
		app.Status.SetText("Settings canceled")
		app.closeSettings()
	}
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName: "Settings",
		OnButton: func(name string) {
			if name == "save_button" {
				app.applySettings(app.settingsDialog.Result())
			} else {
				app.Status.SetText("Settings canceled")
			}
			app.closeSettings()
		},
	}
	app.settingsWasShown = true
	app.stack.Push(app.settingsDialog, frame)
}

func (app *ActivityFeedApp) closeSettings() {
	app.stack.Remove(app.settingsDialog.UI)
	app.settingsDialog.Close()
}

func (app *ActivityFeedApp) applySettings(result settingsdialog.SettingsResult) {
	app.autoScroll = result.AutoScroll
	app.showTimestamps = result.ShowTimestamps
	app.defaultType = result.DefaultType
	if app.defaultType == "" {
		app.defaultType = defaultActivityType
	}
	selectComboValue(app.ActivityType, app.defaultType)
	app.feed.SetAutoScroll(app.autoScroll)
	app.Status.SetText("Settings saved")
}

func (app *ActivityFeedApp) seedInitial() {
	count := initialSampleCount
	if count > len(sampleActivities) {
		count = len(sampleActivities)
	}
	for index := 0; index < count; index++ {
		app.AppendActivity(sampleActivities[index].eventType, sampleActivities[index].message)
	}
	app.updateCountStatus()
}

func (app *ActivityFeedApp) updateCountStatus() {
	app.Status.SetText(fmt.Sprintf("%d activities", app.feed.ActivityCount()))
}

func currentTimestamp() string {
	if fixed := os.Getenv(timestampEnv); fixed != "" {
		return fixed
	}
	return time.Now().Format("15:04")
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

func smoke() {
	window := NewActivityFeedUI()
	if window.Title() != "Activity Feed" || window.HeaderTitle == nil || window.SettingsButton == nil ||
		window.QuitButton == nil || window.Feed == nil || window.ActivityType == nil ||
		window.Message == nil || window.AddButton == nil || window.SeedButton == nil ||
		window.ClearButton == nil || window.Status == nil {
		panic("activity feed smoke failed")
	}
}

func logicTest() {
	app := NewActivityFeedApp()
	app.Open()
	if app.ActivityCount() != initialSampleCount {
		panic("initial seed failed")
	}
	app.Message.SetValue("Manual event")
	app.AddCurrentActivity()
	if app.Message.Value != "" || app.Status.Text != "3 activities" {
		panic("manual activity failed")
	}
	app.SeedActivities()
	if app.ActivityCount() != initialSampleCount+1+len(sampleActivities) {
		panic("seed activities failed")
	}
	app.ClearActivities()
	if app.ActivityCount() != 0 || app.Status.Text != "Feed cleared" {
		panic("clear failed")
	}
	app.OpenSettings()
	if !app.HasModalOverlay() {
		panic("settings modal not open")
	}
	app.OnQuitButtonClick()
	if !app.ShouldClose() {
		panic("quit did not request close")
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
		app := NewActivityFeedApp()
		app.Open()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
