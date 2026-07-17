package main

import (
	"os"

	uimd "uimd"
)

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewCellsUI()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
