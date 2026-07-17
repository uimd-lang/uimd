package main

import (
	"os"
	"strconv"
	"strings"

	uimd "uimd"
)

const (
	minimumRenderHeight       = 1
	fieldCount                = 40
	primaryFieldIndex         = 0
	bodyFieldIndex            = 1
	displayIndexOffset        = 1
	fieldNumberPaddingWidth   = 2
	fieldNumberPaddingCutover = 10
)

type reproItem struct {
	name    string
	summary string
	body    string
}

var reproItems = []reproItem{
	{
		name:    "Alpha Apartments",
		summary: "First row, visible without scrolling",
		body: "Alpha item was opened from the top of the ScrollView.\n" +
			"The edit page should not keep any stale list focus background.",
	},
	{
		name:    "Bravo Offices",
		summary: "Second row, used by the automated repro",
		body: "Bravo item was opened by pressing Enter on a row button.\n" +
			"The old ScrollView page must be gone before this page renders.",
	},
	{
		name:    "Charlie Lofts",
		summary: "Third row, keeps the source list taller",
		body: "Charlie item keeps the source page closer to the reported list shape.\n" +
			"The opened edit page still owns the new nested ScrollView.",
	},
	{
		name:    "Delta Studios",
		summary: "Fourth row, manual repro filler",
		body: "Delta item is extra manual test data for scroll and focus traversal.\n" +
			"Activating Open should replace the whole page cleanly.",
	},
	{
		name:    "Echo Retail",
		summary: "Fifth row, enough rows to browse",
		body: "Echo item makes the source ScrollView visibly list-like.\n" +
			"The edit form below contains many alternating controls.",
	},
}

type openCallback func(reproItem)
type backCallback func()
type fieldActionCallback func(int, string)

func fieldNumber(index int) string {
	displayIndex := index + displayIndexOffset
	value := strconv.Itoa(displayIndex)
	if displayIndex < fieldNumberPaddingCutover {
		for len(value) < fieldNumberPaddingWidth {
			value = "0" + value
		}
	}
	return value
}

func firstLine(value string) string {
	line, _, _ := strings.Cut(value, "\n")
	return line
}

func fieldValue(item reproItem, index int) string {
	if index == primaryFieldIndex {
		return item.name + " primary field"
	}
	if index == bodyFieldIndex {
		return firstLine(item.body)
	}
	return item.name + " detail field " + fieldNumber(index)
}

type ItemRow struct {
	*ItemRowUI
	item   reproItem
	onOpen openCallback
}

func NewItemRow(item reproItem, onOpen openCallback) *ItemRow {
	row := &ItemRow{
		ItemRowUI: NewItemRowUI(),
		item:      item,
		onOpen:    onOpen,
	}
	row.SetEventHandler(row)
	row.Name.SetText(item.name)
	row.Summary.SetText(item.summary)
	return row
}

func (row *ItemRow) OnOpenBtnClick() {
	if row.onOpen != nil {
		row.onOpen(row.item)
	}
}

type ListPage struct {
	*ListPageUI
	Panel  *uimd.ScrollView
	onOpen openCallback
}

func NewListPage(onOpen openCallback) *ListPage {
	page := &ListPage{
		ListPageUI: NewListPageUI(),
		onOpen:     onOpen,
	}
	page.Panel = page.NewGeneratedScrollViewElement()
	page.AddElement(page.Panel)
	page.populate()
	return page
}

func (page *ListPage) populate() {
	page.Panel.ClearChildren()
	for _, item := range reproItems {
		row := NewItemRow(item, page.onOpen)
		reusable := uimd.NewReusableElement("row", "item_row")
		reusable.SetChild(row)
		naturalSize := uimd.GeneratedWindowContentSize(row)
		reusable.SetFrame(uimd.Rect{Height: maxInt(minimumRenderHeight, naturalSize.Height)})
		page.Panel.AddChild(reusable)
	}
	page.Panel.ScrollToTop()
}

type EditFieldRow struct {
	*EditFieldRowUI
	index    int
	onAction fieldActionCallback
}

func NewEditFieldRow(index int, value string, onAction fieldActionCallback) *EditFieldRow {
	row := &EditFieldRow{
		EditFieldRowUI: NewEditFieldRowUI(),
		index:          index,
		onAction:       onAction,
	}
	row.SetEventHandler(row)
	row.FieldInput.SetValue(value)
	row.FieldBtn.SetTitle("Action " + fieldNumber(index))
	return row
}

func (row *EditFieldRow) OnFieldBtnClick() {
	if row.onAction != nil {
		row.onAction(row.index, row.FieldInput.Value)
	}
}

type EditFieldsScroll struct {
	*EditFieldsScrollUI
	Panel    *uimd.ScrollView
	item     reproItem
	onAction fieldActionCallback
}

func NewEditFieldsScroll(item reproItem, onAction fieldActionCallback) *EditFieldsScroll {
	scroll := &EditFieldsScroll{
		EditFieldsScrollUI: NewEditFieldsScrollUI(),
		item:               item,
		onAction:           onAction,
	}
	scroll.Panel = scroll.NewGeneratedScrollViewElement()
	scroll.AddElement(scroll.Panel)
	scroll.populate()
	return scroll
}

func (scroll *EditFieldsScroll) populate() {
	scroll.Panel.ClearChildren()
	for index := range fieldCount {
		row := NewEditFieldRow(index, fieldValue(scroll.item, index), scroll.onAction)
		reusable := uimd.NewReusableElement("row", "edit_field_row")
		reusable.SetChild(row)
		naturalSize := uimd.GeneratedWindowContentSize(row)
		reusable.SetFrame(uimd.Rect{Height: maxInt(minimumRenderHeight, naturalSize.Height)})
		scroll.Panel.AddChild(reusable)
	}
	scroll.Panel.ScrollToTop()
}

type EditPage struct {
	*EditPageUI
	item   reproItem
	onBack backCallback
}

func NewEditPage(item reproItem, onBack backCallback) *EditPage {
	page := &EditPage{
		EditPageUI: NewEditPageUI(),
		item:       item,
		onBack:     onBack,
	}
	page.SetEventHandler(page)
	page.EditTitle.SetText("Edit " + item.name)
	page.NameInput.SetValue(item.name)
	page.Fields.SetView(NewEditFieldsScroll(item, page.fieldAction))
	return page
}

func (page *EditPage) OnBackBtnClick() {
	if page.onBack != nil {
		page.onBack()
	}
}

func (page *EditPage) OnSaveBtnClick() {
	page.Status.SetText("Saved in memory")
}

func (page *EditPage) fieldAction(index int, value string) {
	page.Status.SetText("Action " + fieldNumber(index) + ": " + value)
}

type StaleScrollviewFocusApp struct {
	*StaleScrollviewFocusUI
	quitRequested bool
}

func NewStaleScrollviewFocusApp() *StaleScrollviewFocusApp {
	app := &StaleScrollviewFocusApp{StaleScrollviewFocusUI: NewStaleScrollviewFocusUI()}
	app.SetEventHandler(app)
	app.showListPage()
	return app
}

func (app *StaleScrollviewFocusApp) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.StaleScrollviewFocusUI.RuntimeOptions()
	options.ShouldClose = app.ShouldClose
	return options
}

func (app *StaleScrollviewFocusApp) ShouldClose() bool {
	return app.quitRequested || app.GeneratedWindowBase.ShouldClose()
}

func (app *StaleScrollviewFocusApp) OnTabListClick() {
	app.showListPage()
}

func (app *StaleScrollviewFocusApp) OnTabOtherClick() {
	app.Hint.SetText("Other tab intentionally leaves the list unchanged.")
}

func (app *StaleScrollviewFocusApp) OnQuitBtnClick() {
	app.quitRequested = true
	app.RequestClose()
}

func (app *StaleScrollviewFocusApp) showListPage() {
	app.Page.SetView(NewListPage(func(item reproItem) {
		app.showEditPage(item)
	}))
	app.Hint.SetText("Focus list, press Enter, choose Open.")
}

func (app *StaleScrollviewFocusApp) showEditPage(item reproItem) {
	app.Page.SetView(NewEditPage(item, app.showListPage))
	app.Hint.SetText("Edit page opened from ScrollView row.")
}

func maxInt(lhs int, rhs int) int {
	if lhs > rhs {
		return lhs
	}
	return rhs
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewStaleScrollviewFocusApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
