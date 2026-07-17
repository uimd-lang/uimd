package main

import (
	"os"

	uimd "uimd"
)

type SourceSeparatorScroll struct {
	*SourceSeparatorScrollUI
	Panel *uimd.ScrollView
}

func NewSourceSeparatorScroll() *SourceSeparatorScroll {
	app := &SourceSeparatorScroll{SourceSeparatorScrollUI: NewSourceSeparatorScrollUI()}
	app.Panel = app.NewGeneratedScrollViewElement()
	app.AddElement(app.Panel)
	app.Panel.ClearChildren()
	row := NewSourceSeparatorRowUI()
	reusable := uimd.NewReusableElement("row", "source_separator_row")
	reusable.SetChild(row)
	naturalSize := uimd.GeneratedWindowContentSize(row)
	reusable.SetFrame(uimd.Rect{Width: naturalSize.Width, Height: naturalSize.Height})
	app.Panel.AddChild(reusable)
	return app
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewSourceSeparatorScroll()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
