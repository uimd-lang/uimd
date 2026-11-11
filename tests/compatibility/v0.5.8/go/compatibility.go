package main

import (
    "os"

    uimd "uimd"
)

type CompatibilityApp struct {
    *CompatibilityUI
}

func NewCompatibilityApp() *CompatibilityApp {
    app := &CompatibilityApp{
        CompatibilityUI: NewCompatibilityUI(),
    }
    app.SetEventHandler(app)
    app.Scroller.SetChild(NewCompatScrollUI())
    return app
}

func main() {
    code := uimd.RunGeneratedAppMain(func() int {
        app := NewCompatibilityApp()
        return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
    })
    os.Exit(code)
}
