package main

import (
	"fmt"
	"os"
	"strings"

	uimd "uimd"
)

const (
	defaultName  = "Ada Lovelace"
	defaultCount = 3
)

type WidgetGallery struct {
	*WidgetGalleryUI
	finished bool
}

func NewWidgetGallery() *WidgetGallery {
	app := &WidgetGallery{WidgetGalleryUI: NewWidgetGalleryUI()}
	app.SetEventHandler(app)
	app.refreshSummary()
	return app
}

func (app *WidgetGallery) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.WidgetGalleryUI.RuntimeOptions()
	options.ShouldClose = app.ShouldClose
	return options
}

func (app *WidgetGallery) OnResetBtnClick() {
	app.resetValues()
}

func (app *WidgetGallery) OnApplyBtnClick() {
	app.refreshSummary()
}

func (app *WidgetGallery) OnCloseBtnClick() {
	app.finished = true
	app.RequestClose()
}

func (app *WidgetGallery) ShouldClose() bool {
	return app.finished || app.GeneratedWindowBase.ShouldClose()
}

func (app *WidgetGallery) summaryText() string {
	mode := ""
	values := app.ModeList.SelectedValues()
	if len(values) > 0 {
		mode = values[0]
	}
	return strings.Join([]string{
		"values:",
		"  name: " + app.NameInput.Value,
		"  count: " + fmt.Sprintf("%g", app.CountInput.NumberValue),
		"  theme: " + app.ThemeCombo.Value,
		"  mode: " + mode,
		"  enabled: " + boolTitle(app.EnabledCheck.Checked),
	}, "\n")
}

func (app *WidgetGallery) refreshSummary() {
	app.Spans.SetSpans([]uimd.LabelSpan{
		{Text: "SpanLabel", Color: "#facc15"},
		{Text: " can mix "},
		{Text: "foreground", Color: "#38bdf8"},
		{Text: " and "},
		{Text: "background", Color: "#111827", Background: "#f97316"},
		{Text: " colors."},
	})
	app.Summary.SetText(app.summaryText())
}

func (app *WidgetGallery) resetValues() {
	app.NameInput.SetText(defaultName)
	app.CountInput.SetNumberValue(defaultCount)
	app.EnabledCheck.SetChecked(true)
	app.ThemeCombo.SetSelectedIndex(0)
	app.ModeList.SetSelectedIndex(0)
	app.refreshSummary()
}

func boolTitle(value bool) string {
	if value {
		return "True"
	}
	return "False"
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewWidgetGallery()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
