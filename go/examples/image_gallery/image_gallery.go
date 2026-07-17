package main

import (
	"os"

	uimd "uimd"
)

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewImageGalleryUI()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
