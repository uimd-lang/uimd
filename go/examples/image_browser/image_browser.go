package main

import (
	"os"
	"path/filepath"
	"runtime"
	"strings"

	uimd "uimd"
)

type ImageEntry struct {
	Caption string
	Source  string
}

type SidebarImageEntry struct {
	MemberName string
	Caption    string
	Source     string
}

type ImageRecord struct {
	Name     string
	Source   string
	Selected bool
}

const (
	initialListCount         = 20
	firstGeneratedSample     = 6
	lastGeneratedSample      = 50
	imageSampleDir           = "shared/assets/image_samples"
	imageExtensionFilter     = "(png|jpg|jpeg|gif|bmp|tga|ppm|pgm)"
	thumbNormalBackground    = "transparent"
	thumbSelectedBackground  = "#2563eb"
	mosaicNormalBackground   = "#172033"
	renderModeSixel          = "sixel"
	renderModeFallback       = "fallback"
	logicTestExpectedRecords = 20
)

func sampleExtension(index int) string {
	switch index % 6 {
	case 0:
		return ".bmp"
	case 1:
		return ".gif"
	case 2:
		return ".png"
	case 3:
		return ".tga"
	default:
		return ".jpg"
	}
}

func imageLibrary() []ImageEntry {
	images := []ImageEntry{
		{Caption: "Camera", Source: "shared/assets/image_samples/camera.png"},
		{Caption: "Astronaut", Source: "shared/assets/image_samples/astronaut.png"},
		{Caption: "Coffee", Source: "shared/assets/image_samples/coffee.png"},
		{Caption: "Coins", Source: "shared/assets/image_samples/coins.png"},
		{Caption: "Chelsea", Source: "shared/assets/image_samples/chelsea.png"},
	}
	for index := firstGeneratedSample; index <= lastGeneratedSample; index++ {
		images = append(images, ImageEntry{
			Caption: "Sample " + intToString(index),
			Source:  "shared/assets/image_samples/sample_" + intToString(index) + sampleExtension(index),
		})
	}
	return images
}

func sidebarImages() []SidebarImageEntry {
	images := imageLibrary()
	return []SidebarImageEntry{
		{MemberName: "camera_thumb", Caption: images[0].Caption, Source: images[0].Source},
		{MemberName: "astro_thumb", Caption: images[1].Caption, Source: images[1].Source},
		{MemberName: "coffee_thumb", Caption: images[2].Caption, Source: images[2].Source},
	}
}

func initialImageRecords() []ImageRecord {
	images := imageLibrary()
	count := minIntLocal(initialListCount, len(images))
	records := make([]ImageRecord, 0, count)
	for index := 0; index < count; index++ {
		records = append(records, ImageRecord{Name: images[index].Caption, Source: images[index].Source})
	}
	return records
}

func projectRoot() string {
	_, sourceFile, _, ok := runtime.Caller(0)
	if !ok {
		cwd, err := os.Getwd()
		if err != nil {
			return "."
		}
		return filepath.Clean(cwd)
	}
	return filepath.Clean(filepath.Join(filepath.Dir(sourceFile), "..", "..", ".."))
}

func imageSamplePath() string {
	return filepath.Join(projectRoot(), imageSampleDir)
}

func imageNameFromPath(path string) string {
	stem := strings.TrimSuffix(filepath.Base(strings.TrimRight(path, string(os.PathSeparator))), filepath.Ext(path))
	if stem == "" {
		return path
	}
	return titleCaseImageName(stem)
}

func titleCaseImageName(name string) string {
	text := strings.TrimSpace(strings.ReplaceAll(strings.ReplaceAll(name, "_", " "), "-", " "))
	if text == "" {
		return ""
	}
	parts := strings.Fields(text)
	for index, part := range parts {
		if part == "" {
			continue
		}
		parts[index] = strings.ToUpper(part[:1]) + part[1:]
	}
	return strings.Join(parts, " ")
}

func imageDisplayPath(path string) string {
	if path == "" {
		return ""
	}
	absolute, err := filepath.Abs(path)
	if err != nil {
		return path
	}
	root, err := filepath.Abs(projectRoot())
	if err != nil {
		return path
	}
	absolute = filepath.Clean(absolute)
	root = filepath.Clean(root)
	if absolute == root || strings.HasPrefix(absolute, root+string(os.PathSeparator)) {
		if relative, err := filepath.Rel(root, absolute); err == nil {
			return relative
		}
	}
	return path
}

type ImageView struct {
	*ImageViewUI
}

func NewImageView(source string, caption string, renderMode string) *ImageView {
	view := &ImageView{ImageViewUI: NewImageViewUI()}
	view.Photo.Source = source
	view.Photo.RenderMode = renderMode
	view.Caption.SetText(caption)
	return view
}

type ImageButton struct {
	*ImageButtonUI
	onClick func()
}

func NewImageButton() *ImageButton {
	button := &ImageButton{ImageButtonUI: NewImageButtonUI()}
	button.SetEventHandler(button)
	button.Photo.SetFocusable(false)
	return button
}

func (button *ImageButton) SetSource(source string) {
	button.Photo.Source = source
}

func (button *ImageButton) SetAlt(alt string) {
	button.Photo.Alt = alt
}

func (button *ImageButton) SetRenderMode(renderMode string) {
	button.Photo.RenderMode = renderMode
}

func (button *ImageButton) SetSelected(selected bool) {
	background := thumbNormalBackground
	if selected {
		background = thumbSelectedBackground
	}
	setGeneratedCellBackground(button.GeneratedWindowBase, background)
}

func (button *ImageButton) SetOnClick(onClick func()) {
	button.onClick = onClick
}

func (button *ImageButton) OnPhotoClick() {
	button.fire()
}

func (button *ImageButton) ActivateGeneratedControl() bool {
	button.fire()
	return button.onClick != nil
}

func (button *ImageButton) fire() {
	if button.onClick != nil {
		button.onClick()
	}
}

type GalleryMosaic struct {
	*GalleryMosaicUI
	onClick func()
}

func NewGalleryMosaic(onClick func()) *GalleryMosaic {
	mosaic := &GalleryMosaic{GalleryMosaicUI: NewGalleryMosaicUI(), onClick: onClick}
	mosaic.SetEventHandler(mosaic)
	for _, image := range []*uimd.Image{mosaic.Gal1, mosaic.Gal2, mosaic.Gal3, mosaic.Gal4, mosaic.Gal5} {
		image.SetFocusable(false)
	}
	return mosaic
}

func (mosaic *GalleryMosaic) SetRenderMode(renderMode string) {
	for _, image := range []*uimd.Image{mosaic.Gal1, mosaic.Gal2, mosaic.Gal3, mosaic.Gal4, mosaic.Gal5} {
		image.RenderMode = renderMode
	}
}

func (mosaic *GalleryMosaic) SetSelected(selected bool) {
	background := mosaicNormalBackground
	if selected {
		background = thumbSelectedBackground
	}
	setGeneratedCellBackground(mosaic.GeneratedWindowBase, background)
}

func (mosaic *GalleryMosaic) OnGal1Click() { mosaic.fire() }
func (mosaic *GalleryMosaic) OnGal2Click() { mosaic.fire() }
func (mosaic *GalleryMosaic) OnGal3Click() { mosaic.fire() }
func (mosaic *GalleryMosaic) OnGal4Click() { mosaic.fire() }
func (mosaic *GalleryMosaic) OnGal5Click() { mosaic.fire() }

func (mosaic *GalleryMosaic) ActivateGeneratedControl() bool {
	mosaic.fire()
	return mosaic.onClick != nil
}

func (mosaic *GalleryMosaic) fire() {
	if mosaic.onClick != nil {
		mosaic.onClick()
	}
}

type ImageListButton struct {
	*ImageListButtonUI
	onClick func()
}

func NewImageListButton() *ImageListButton {
	button := &ImageListButton{ImageListButtonUI: NewImageListButtonUI()}
	button.SetEventHandler(button)
	for _, image := range []*uimd.Image{button.Prw1, button.Prw2, button.Prw3} {
		image.SetFocusable(false)
	}
	return button
}

func (button *ImageListButton) SetImages(images []ImageEntry) {
	previews := []*uimd.Image{button.Prw1, button.Prw2, button.Prw3}
	for index := 0; index < len(previews) && index < len(images); index++ {
		previews[index].Source = images[index].Source
		previews[index].Alt = images[index].Caption
	}
}

func (button *ImageListButton) SetRenderMode(renderMode string) {
	for _, image := range []*uimd.Image{button.Prw1, button.Prw2, button.Prw3} {
		image.RenderMode = renderMode
	}
}

func (button *ImageListButton) SetSelected(selected bool) {
	background := mosaicNormalBackground
	if selected {
		background = thumbSelectedBackground
	}
	setGeneratedCellBackground(button.GeneratedWindowBase, background)
}

func (button *ImageListButton) SetOnClick(onClick func()) {
	button.onClick = onClick
}

func (button *ImageListButton) OnPrw1Click() { button.fire() }
func (button *ImageListButton) OnPrw2Click() { button.fire() }
func (button *ImageListButton) OnPrw3Click() { button.fire() }

func (button *ImageListButton) ActivateGeneratedControl() bool {
	button.fire()
	return button.onClick != nil
}

func (button *ImageListButton) fire() {
	if button.onClick != nil {
		button.onClick()
	}
}

type GalleryItem struct {
	*GalleryItemUI
}

func NewGalleryItem(source string, caption string, renderMode string) *GalleryItem {
	item := &GalleryItem{GalleryItemUI: NewGalleryItemUI()}
	item.Photo.Source = source
	item.Photo.RenderMode = renderMode
	item.Caption.SetText(caption)
	return item
}

type GalleryScroll struct {
	*GalleryScrollUI
	Panel *uimd.ScrollView
}

func NewGalleryScroll() *GalleryScroll {
	scroll := &GalleryScroll{GalleryScrollUI: NewGalleryScrollUI()}
	scroll.Panel = scroll.NewGeneratedScrollViewElement()
	scroll.AddElement(scroll.Panel)
	return scroll
}

func (scroll *GalleryScroll) Populate(renderMode string) {
	scroll.Panel.ClearChildren()
	images := imageLibrary()
	count := minIntLocal(len(images), 5)
	for index := 0; index < count; index++ {
		item := NewGalleryItem(images[index].Source, images[index].Caption, renderMode)
		reusable := uimd.NewReusableElement("gallery_item", "gallery_item")
		reusable.SetChild(item)
		naturalSize := uimd.GeneratedWindowContentSize(item)
		measuredSize := uimd.GeneratedWindowContentSizeForWidth(item, naturalSize.Width)
		reusable.SetFrame(uimd.Rect{Width: 0, Height: maxIntLocal(1, measuredSize.Height)})
		scroll.Panel.AddChild(reusable)
	}
	scroll.Panel.ViewOffset = 0
}

type GalleryView struct {
	*GalleryViewUI
	galleryScroll *GalleryScroll
}

func NewGalleryView(renderMode string) *GalleryView {
	view := &GalleryView{GalleryViewUI: NewGalleryViewUI()}
	view.galleryScroll = NewGalleryScroll()
	view.GalleryScroll.SetChild(view.galleryScroll)
	view.galleryScroll.Populate(renderMode)
	return view
}

func (view *GalleryView) SetRenderMode(renderMode string) {
	if view.galleryScroll != nil {
		view.galleryScroll.Populate(renderMode)
	}
}

type ImageShowDialog struct {
	*ImageShowDialogUI
}

func NewImageShowDialog(source string, caption string, renderMode string) *ImageShowDialog {
	dialog := &ImageShowDialog{ImageShowDialogUI: NewImageShowDialogUI()}
	dialog.Title.SetText(caption)
	dialog.Photo.Source = source
	dialog.Photo.Alt = caption
	dialog.Photo.RenderMode = renderMode
	return dialog
}

type ImageListItem struct {
	*ImageListItemUI
	record     *ImageRecord
	renderMode string
	onShow     func(*ImageRecord)
	onBrowse   func(*ImageRecord)
	onDelete   func(*ImageRecord)
	onChange   func(*ImageRecord)
}

func NewImageListItem(record *ImageRecord, renderMode string, onShow func(*ImageRecord), onBrowse func(*ImageRecord), onDelete func(*ImageRecord), onChange func(*ImageRecord)) *ImageListItem {
	item := &ImageListItem{
		ImageListItemUI: NewImageListItemUI(),
		record:          record,
		renderMode:      renderMode,
		onShow:          onShow,
		onBrowse:        onBrowse,
		onDelete:        onDelete,
		onChange:        onChange,
	}
	item.SetEventHandler(item)
	item.syncFromRecord()
	return item
}

func (item *ImageListItem) OnSelChange(_ string) {
	item.record.Selected = item.Sel.Checked
	if item.onChange != nil {
		item.onChange(item.record)
	}
}

func (item *ImageListItem) OnThumbClick() {
	item.OnShowBtnClick()
}

func (item *ImageListItem) OnShowBtnClick() {
	if item.onShow != nil {
		item.onShow(item.record)
	}
}

func (item *ImageListItem) OnBrowseBtnClick() {
	if item.onBrowse != nil {
		item.onBrowse(item.record)
	}
}

func (item *ImageListItem) OnDeleteBtnClick() {
	if item.onDelete != nil {
		item.onDelete(item.record)
	}
}

func (item *ImageListItem) syncFromRecord() {
	item.Thumb.Source = item.record.Source
	item.Thumb.Alt = item.record.Name
	item.Thumb.Fit = "contain"
	item.Thumb.RenderMode = item.renderMode
	item.NameLabel.SetText(item.record.Name)
	item.PathLabel.SetText(imageDisplayPath(item.record.Source))
	item.Sel.SetChecked(item.record.Selected)
}

type ImageListScroll struct {
	*ImageListScrollUI
	Panel *uimd.ScrollView
}

func NewImageListScroll() *ImageListScroll {
	scroll := &ImageListScroll{ImageListScrollUI: NewImageListScrollUI()}
	scroll.Panel = scroll.NewGeneratedScrollViewElement()
	scroll.AddElement(scroll.Panel)
	scroll.Panel.SetAutoScroll(false)
	return scroll
}

func (scroll *ImageListScroll) SetItems(records []ImageRecord, renderMode string, onShow func(*ImageRecord), onBrowse func(*ImageRecord), onDelete func(*ImageRecord), onChange func(*ImageRecord), scrollToEnd bool, preserveScroll bool) {
	position := scroll.Panel.ViewOffset
	scroll.Panel.ClearChildren()
	for index := range records {
		item := NewImageListItem(&records[index], renderMode, onShow, onBrowse, onDelete, onChange)
		reusable := uimd.NewReusableElement("image_list_item", "image_list_item")
		reusable.SetChild(item)
		naturalSize := uimd.GeneratedWindowContentSize(item)
		measuredSize := uimd.GeneratedWindowContentSizeForWidth(item, naturalSize.Width)
		reusable.SetFrame(uimd.Rect{Width: 0, Height: maxIntLocal(1, measuredSize.Height)})
		scroll.Panel.AddChild(reusable)
	}
	if scrollToEnd {
		scroll.Panel.ScrollToBottom()
	} else if preserveScroll {
		scroll.Panel.ViewOffset = position
	} else {
		scroll.Panel.ViewOffset = 0
	}
}

type ImageListView struct {
	*ImageListViewUI
	records             *[]ImageRecord
	renderMode          string
	modalStack          *uimd.GeneratedWindowStack
	itemsScroll         *ImageListScroll
	browser             *uimd.FileBrowser
	deleteDialog        *uimd.MessageBoxYesNo
	showDialog          *ImageShowDialog
	pendingBrowseRecord *ImageRecord
	pendingDeleteRecord *ImageRecord
}

func NewImageListView(records *[]ImageRecord, renderMode string, modalStack *uimd.GeneratedWindowStack) *ImageListView {
	view := &ImageListView{
		ImageListViewUI: NewImageListViewUI(),
		records:         records,
		renderMode:      renderMode,
		modalStack:      modalStack,
	}
	view.SetEventHandler(view)
	view.itemsScroll = NewImageListScroll()
	view.Items.SetChild(view.itemsScroll)
	view.refreshItems(false, false)
	return view
}

func (view *ImageListView) SetRenderMode(renderMode string) {
	view.renderMode = renderMode
	view.refreshItems(false, true)
}

func (view *ImageListView) OnAddBtnClick() {
	view.openBrowser(nil)
}

func (view *ImageListView) refreshItems(scrollToEnd bool, preserveScroll bool) {
	if view.itemsScroll == nil || view.records == nil {
		return
	}
	view.itemsScroll.SetItems(
		*view.records,
		view.renderMode,
		func(record *ImageRecord) { view.showImage(record) },
		func(record *ImageRecord) { view.openBrowser(record) },
		func(record *ImageRecord) { view.confirmDelete(record) },
		func(_ *ImageRecord) {},
		scrollToEnd,
		preserveScroll,
	)
}

func (view *ImageListView) showImage(record *ImageRecord) {
	if record == nil || view.modalStack == nil {
		return
	}
	view.closeShowDialog()
	view.showDialog = NewImageShowDialog(record.Source, record.Name, view.renderMode)
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName:        "ImageShowDialog",
		InitialFocusName: "close_btn",
		OnButton: func(name string) {
			if name == "close_btn" {
				view.closeShowDialog()
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				view.closeShowDialog()
				return true
			}
			return false
		},
	}
	view.modalStack.Push(view.showDialog, frame)
}

func (view *ImageListView) openBrowser(record *ImageRecord) {
	if view.modalStack == nil {
		return
	}
	view.closeBrowser()
	view.pendingBrowseRecord = record
	start := imageSamplePath()
	if record != nil && record.Source != "" {
		start = record.Source
	}
	view.browser = uimd.NewFileBrowser(projectRoot(), start, "open", func(path string) {
		view.onBrowserClosed(path)
	})
	view.browser.SetExtensionFilter(imageExtensionFilter)
	view.modalStack.Push(view.browser, view.browser.StackFrameOptions())
}

func (view *ImageListView) onBrowserClosed(path string) {
	if path == "" {
		view.closeBrowser()
		return
	}
	if view.records == nil {
		view.closeBrowser()
		return
	}
	if view.pendingBrowseRecord == nil {
		*view.records = append(*view.records, ImageRecord{Name: imageNameFromPath(path), Source: path})
		view.closeBrowser()
		view.refreshItems(true, false)
		return
	}
	view.pendingBrowseRecord.Name = imageNameFromPath(path)
	view.pendingBrowseRecord.Source = path
	view.closeBrowser()
	view.refreshItems(false, true)
}

func (view *ImageListView) confirmDelete(record *ImageRecord) {
	if view.modalStack == nil || record == nil {
		return
	}
	view.closeDeleteDialog()
	view.pendingDeleteRecord = record
	view.deleteDialog = uimd.NewMessageBoxYesNo("Delete Image", "Delete "+record.Name+"?")
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNo",
		InitialFocusName: "no_btn",
		OnButton: func(name string) {
			if name == "yes_btn" {
				view.deleteConfirmed(true)
			} else if name == "no_btn" {
				view.deleteConfirmed(false)
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				view.deleteConfirmed(false)
				return true
			}
			return false
		},
	}
	view.modalStack.Push(view.deleteDialog, frame)
}

func (view *ImageListView) deleteConfirmed(confirmed bool) {
	record := view.pendingDeleteRecord
	view.closeDeleteDialog()
	if !confirmed || record == nil || view.records == nil {
		return
	}
	for index := range *view.records {
		if &(*view.records)[index] == record {
			*view.records = append((*view.records)[:index], (*view.records)[index+1:]...)
			break
		}
	}
	view.refreshItems(false, true)
}

func (view *ImageListView) closeBrowser() {
	if view.browser != nil && view.modalStack != nil {
		view.modalStack.Remove(view.browser)
	}
	view.browser = nil
	view.pendingBrowseRecord = nil
}

func (view *ImageListView) closeDeleteDialog() {
	if view.deleteDialog != nil && view.modalStack != nil {
		view.modalStack.Remove(view.deleteDialog)
	}
	view.deleteDialog = nil
	view.pendingDeleteRecord = nil
}

func (view *ImageListView) closeShowDialog() {
	if view.showDialog != nil && view.modalStack != nil {
		view.modalStack.Remove(view.showDialog)
	}
	view.showDialog = nil
}

type ImageBrowserApp struct {
	*ImageBrowserUI
	records         []ImageRecord
	stack           uimd.GeneratedWindowStack
	renderMode      string
	thumbs          []*ImageButton
	galleryMosaic   *GalleryMosaic
	imageListButton *ImageListButton
	quitRequested   bool
}

func NewImageBrowserApp() *ImageBrowserApp {
	app := &ImageBrowserApp{
		ImageBrowserUI: NewImageBrowserUI(),
		records:        initialImageRecords(),
		renderMode:     renderModeSixel,
	}
	app.SetEventHandler(app)
	app.galleryMosaic = NewGalleryMosaic(func() { app.ShowGallery() })
	app.GalleryMosaic.SetChild(app.galleryMosaic)
	app.imageListButton = NewImageListButton()
	app.imageListButton.SetImages(imageLibrary())
	app.imageListButton.SetOnClick(func() { app.ShowImageList() })
	app.ImageListBtn.SetChild(app.imageListButton)
	slots := []*uimd.ReusableElement{app.CameraThumb, app.AstroThumb, app.CoffeeThumb}
	sidebar := sidebarImages()
	for index := range slots {
		button := NewImageButton()
		button.SetSource(sidebar[index].Source)
		button.SetAlt(sidebar[index].Caption)
		current := index
		button.SetOnClick(func() { app.ShowImage(current) })
		slots[index].SetChild(button)
		app.thumbs = append(app.thumbs, button)
	}
	app.ApplyRenderMode(renderModeSixel)
	app.ShowImage(0)
	return app
}

func (app *ImageBrowserApp) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.ImageBrowserUI.RuntimeOptions()
	options.InitialFocusName = "camera_thumb"
	options.WindowStack = &app.stack
	options.ShouldClose = app.ShouldClose
	return options
}

func (app *ImageBrowserApp) ShouldClose() bool {
	return app.quitRequested || app.GeneratedWindowBase.ShouldClose()
}

func (app *ImageBrowserApp) OnCloseBtnClick() {
	app.quitRequested = true
	app.RequestClose()
}

func (app *ImageBrowserApp) OnModeSelectChange(value string) {
	if value == "Normal" {
		app.ApplyRenderMode(renderModeSixel)
	} else {
		app.ApplyRenderMode(renderModeFallback)
	}
}

func (app *ImageBrowserApp) ShowImage(index int) {
	sidebar := sidebarImages()
	if index < 0 || index >= len(sidebar) {
		return
	}
	app.selectThumb(index)
	app.setMosaicSelected(false)
	if app.imageListButton != nil {
		app.imageListButton.SetSelected(false)
	}
	app.Main.SetView(NewImageView(sidebar[index].Source, sidebar[index].Caption, app.renderMode))
}

func (app *ImageBrowserApp) ShowGallery() {
	app.deselectAllThumbs()
	app.setMosaicSelected(true)
	if app.imageListButton != nil {
		app.imageListButton.SetSelected(false)
	}
	app.Main.SetView(NewGalleryView(app.renderMode))
}

func (app *ImageBrowserApp) ShowImageList() {
	app.deselectAllThumbs()
	app.setMosaicSelected(false)
	if app.imageListButton != nil {
		app.imageListButton.SetSelected(true)
	}
	app.Main.SetView(NewImageListView(&app.records, app.renderMode, &app.stack))
}

func (app *ImageBrowserApp) ApplyRenderMode(renderMode string) {
	app.renderMode = renderMode
	for _, thumb := range app.thumbs {
		thumb.SetRenderMode(renderMode)
	}
	if app.galleryMosaic != nil {
		app.galleryMosaic.SetRenderMode(renderMode)
	}
	if app.imageListButton != nil {
		app.imageListButton.SetRenderMode(renderMode)
	}
	switch view := app.Main.CurrentView().(type) {
	case *ImageView:
		view.Photo.RenderMode = renderMode
	case *GalleryView:
		view.SetRenderMode(renderMode)
	case *ImageListView:
		view.SetRenderMode(renderMode)
	}
}

func (app *ImageBrowserApp) setMosaicSelected(selected bool) {
	if app.galleryMosaic != nil {
		app.galleryMosaic.SetSelected(selected)
	}
}

func (app *ImageBrowserApp) selectThumb(selectedIndex int) {
	for index, thumb := range app.thumbs {
		thumb.SetSelected(index == selectedIndex)
	}
}

func (app *ImageBrowserApp) deselectAllThumbs() {
	for _, thumb := range app.thumbs {
		thumb.SetSelected(false)
	}
}

func setGeneratedCellBackground(window *uimd.GeneratedWindowBase, background string) {
	layout := window.GeneratedLayout()
	for index := range layout {
		layout[index].CellStyle.Background = uimd.NewColor(background)
	}
	window.SetGeneratedLayout(layout)
}

func intToString(value int) string {
	if value == 0 {
		return "0"
	}
	negative := value < 0
	if negative {
		value = -value
	}
	var digits []byte
	for value > 0 {
		digits = append([]byte{byte('0' + value%10)}, digits...)
		value /= 10
	}
	if negative {
		digits = append([]byte{'-'}, digits...)
	}
	return string(digits)
}

func minIntLocal(first int, second int) int {
	if first < second {
		return first
	}
	return second
}

func maxIntLocal(first int, second int) int {
	if first > second {
		return first
	}
	return second
}

func runLogicTest() int {
	if imageNameFromPath("/tmp/chelsea.png") != "Chelsea" {
		return 1
	}
	images := imageLibrary()
	if len(images) < 9 || !strings.HasSuffix(images[5].Source, ".bmp") || !strings.HasSuffix(images[6].Source, ".gif") || !strings.HasSuffix(images[7].Source, ".png") || !strings.HasSuffix(images[8].Source, ".tga") {
		return 1
	}
	app := NewImageBrowserApp()
	if app.Main.CurrentView() == nil {
		return 1
	}
	app.ShowImage(1)
	if _, ok := app.Main.CurrentView().(*ImageView); !ok {
		return 1
	}
	app.OnModeSelectChange("Fallback")
	if app.renderMode != renderModeFallback {
		return 1
	}
	app.OnModeSelectChange("Normal")
	if app.renderMode != renderModeSixel {
		return 1
	}
	app.ShowGallery()
	if _, ok := app.Main.CurrentView().(*GalleryView); !ok {
		return 1
	}
	app.ShowImageList()
	if _, ok := app.Main.CurrentView().(*ImageListView); !ok {
		return 1
	}
	if len(app.records) != logicTestExpectedRecords {
		return 1
	}
	return 0
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		if len(os.Args) > 1 {
			switch os.Args[1] {
			case "--smoke", "--logic-test":
				return runLogicTest()
			}
		}
		app := NewImageBrowserApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
