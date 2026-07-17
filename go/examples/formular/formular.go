package main

import (
	"fmt"
	"os"
	"strings"

	uimd "uimd"
)

type FormApp struct {
	*FormularUI
	action string
}

func NewFormApp() *FormApp {
	app := &FormApp{
		FormularUI: NewFormularUI(),
	}
	app.SetEventHandler(app)
	return app
}

func (app *FormApp) OnSaveBtnClick() {
	app.action = "save"
	app.RequestClose()
}

func (app *FormApp) OnCancelBtnClick() {
	app.action = "cancel"
	app.RequestClose()
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewFormApp()
		result := uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
		if app.action != "" {
			fmt.Print(formYaml(app.FormularUI, app.action))
		}
		return result
	})
	os.Exit(code)
}

func formYaml(window *FormularUI, action string) string {
	var builder strings.Builder
	builder.WriteString("action: ")
	builder.WriteString(action)
	builder.WriteString("\nform:\n")
	builder.WriteString("  name: ")
	builder.WriteString(yamlScalar(window.NameInput.Value))
	builder.WriteString("\n")
	builder.WriteString("  email: ")
	builder.WriteString(yamlScalar(window.EmailInput.Value))
	builder.WriteString("\n")
	builder.WriteString("  age: ")
	builder.WriteString(fmt.Sprintf("%g", window.AgeInput.NumberValue))
	builder.WriteString("\n")
	builder.WriteString("  description: ")
	builder.WriteString(yamlScalar(window.DescriptionInput.Value))
	builder.WriteString("\n")
	builder.WriteString("  country: ")
	builder.WriteString(yamlScalar(window.CountryCombo.Value))
	builder.WriteString("\n")
	builder.WriteString("  roles:\n")
	for _, role := range window.RoleListbox.SelectedValues() {
		builder.WriteString("  - ")
		builder.WriteString(yamlScalar(role))
		builder.WriteString("\n")
	}
	builder.WriteString("  email_notifications: ")
	builder.WriteString(yamlBool(window.NotifyCheck.Checked))
	builder.WriteString("\n")
	builder.WriteString("  accepted_terms: ")
	builder.WriteString(yamlBool(window.TermsCheck.Checked))
	builder.WriteString("\n")
	return builder.String()
}

func yamlScalar(value string) string {
	if value == "" {
		return "\"\""
	}
	return value
}

func yamlBool(value bool) string {
	if value {
		return "true"
	}
	return "false"
}
