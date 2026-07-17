package main

import (
	"os"
	"strings"

	uimd "uimd"
)

type HelloApp struct {
	*HelloUI
}

func NewHelloApp() *HelloApp {
	app := &HelloApp{
		HelloUI: NewHelloUI(),
	}
	app.SetEventHandler(app)
	return app
}

func (app *HelloApp) OnHelloButtonClick() {
	value := strings.TrimSpace(app.Name.Value)
	if value == "" {
		value = "world"
	}
	app.Headline.SetText("Hello, " + value + "!")
}

func (app *HelloApp) OnQuitButtonClick() {
	app.RequestClose()
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewHelloApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
