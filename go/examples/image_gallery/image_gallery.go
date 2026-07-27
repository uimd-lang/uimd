package main

import (
	"os"

	uimd "uimd"
)

type ImageGallery struct {
	*ImageGalleryUI
}

func NewImageGallery() *ImageGallery {
	app := &ImageGallery{ImageGalleryUI: NewImageGalleryUI()}
	app.SetEventHandler(app)
	return app
}

func (app *ImageGallery) OnCloseBtnClick() {
	app.RequestClose()
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewImageGallery()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
