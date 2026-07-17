package uimd

import (
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"
)

type MessageBoxYesNoCancel struct {
	*GeneratedWindowBase
	DialogHeader *Label
	Message      *Label
	YesBtn       *Button
	NoBtn        *Button
	CancelBtn    *Button
	result       string
}

type MessageBoxYesNo struct {
	*GeneratedWindowBase
	DialogHeader *Label
	Message      *Label
	YesBtn       *Button
	NoBtn        *Button
	result       string
}

type MessageBoxOkCancel struct {
	*GeneratedWindowBase
	DialogHeader *Label
	Message      *Label
	OkBtn        *Button
	CancelBtn    *Button
	result       string
}

type MessageBoxOk struct {
	*GeneratedWindowBase
	DialogHeader *Label
	Message      *Label
	OkBtn        *Button
	result       string
}

type FileBrowser struct {
	*GeneratedWindowBase
	DialogHeader    *Label
	PathLabel       *Label
	Entries         *ListBox
	Filename        *TextInput
	OpenBtn         *Button
	CloseBtn        *Button
	rootDir         string
	currentDir      string
	mode            string
	onClose         func(string)
	closed          bool
	result          string
	filter          string
	initialFilename string
}

func NewMessageBoxYesNoCancel(header string, message string) *MessageBoxYesNoCancel {
	box := &MessageBoxYesNoCancel{GeneratedWindowBase: NewGeneratedWindowBase("Message Box")}
	box.configureMessageBox("MessageBoxYesNoCancel", header, message)
	box.YesBtn = newDialogButton("yes_btn", "Yes")
	box.NoBtn = newDialogButton("no_btn", "No")
	box.CancelBtn = newDialogButton("cancel_btn", "Cancel")
	box.AddElement(box.YesBtn)
	box.AddElement(box.NoBtn)
	box.AddElement(box.CancelBtn)
	box.SetGeneratedLayout(messageBoxYesNoCancelLayout())
	return box
}

func NewMessageBoxYesNo(header string, message string) *MessageBoxYesNo {
	box := &MessageBoxYesNo{GeneratedWindowBase: NewGeneratedWindowBase("Message Box")}
	box.configureMessageBox("MessageBoxYesNo", header, message)
	box.YesBtn = newDialogButton("yes_btn", "Yes")
	box.NoBtn = newDialogButton("no_btn", "No")
	box.AddElement(box.YesBtn)
	box.AddElement(box.NoBtn)
	box.SetGeneratedLayout(messageBoxYesNoLayout())
	return box
}

func NewMessageBoxOkCancel(header string, message string) *MessageBoxOkCancel {
	box := &MessageBoxOkCancel{GeneratedWindowBase: NewGeneratedWindowBase("Message Box")}
	box.configureMessageBox("MessageBoxOkCancel", header, message)
	box.OkBtn = newDialogButton("ok_btn", "OK")
	box.CancelBtn = newDialogButton("cancel_btn", "Cancel")
	box.AddElement(box.OkBtn)
	box.AddElement(box.CancelBtn)
	box.SetGeneratedLayout(messageBoxOkCancelLayout())
	return box
}

func NewMessageBoxOk(header string, message string) *MessageBoxOk {
	box := &MessageBoxOk{GeneratedWindowBase: NewGeneratedWindowBase("Message Box")}
	box.configureMessageBox("MessageBoxOk", header, message)
	box.OkBtn = newDialogButton("ok_btn", "OK")
	box.AddElement(box.OkBtn)
	box.SetGeneratedLayout(messageBoxOkLayout())
	return box
}

func (box *MessageBoxYesNoCancel) configureMessageBox(className string, header string, message string) {
	configureMessageBoxBase(box.GeneratedWindowBase, className)
	box.DialogHeader = newDialogHeaderLabel(header)
	box.Message = newDialogTextLabel("message", message)
	box.AddElement(box.DialogHeader)
	box.AddElement(box.Message)
}

func (box *MessageBoxYesNo) configureMessageBox(className string, header string, message string) {
	configureMessageBoxBase(box.GeneratedWindowBase, className)
	box.DialogHeader = newDialogHeaderLabel(header)
	box.Message = newDialogTextLabel("message", message)
	box.AddElement(box.DialogHeader)
	box.AddElement(box.Message)
}

func (box *MessageBoxOkCancel) configureMessageBox(className string, header string, message string) {
	configureMessageBoxBase(box.GeneratedWindowBase, className)
	box.DialogHeader = newDialogHeaderLabel(header)
	box.Message = newDialogTextLabel("message", message)
	box.AddElement(box.DialogHeader)
	box.AddElement(box.Message)
}

func (box *MessageBoxOk) configureMessageBox(className string, header string, message string) {
	configureMessageBoxBase(box.GeneratedWindowBase, className)
	box.DialogHeader = newDialogHeaderLabel(header)
	box.Message = newDialogTextLabel("message", message)
	box.AddElement(box.DialogHeader)
	box.AddElement(box.Message)
}

func configureMessageBoxBase(window *GeneratedWindowBase, className string) {
	window.SetGeneratedKind("dialog")
	window.SetGeneratedFocusable(false)
	window.SetGeneratedWindowStyle(dialogWindowStyle())
	window.SetMCPMetadata(true, className, "", "Message Box UI source.")
}

func (box *MessageBoxYesNoCancel) RuntimeOptions() GeneratedWindowRuntimeOptions {
	return GeneratedWindowRuntimeOptions{
		InitialFocusName: "cancel_btn",
		OnButton: func(name string) {
			switch name {
			case "yes_btn":
				box.close("yes")
			case "no_btn":
				box.close("no")
			case "cancel_btn":
				box.close("cancel")
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				box.close("cancel")
				return true
			}
			return false
		},
		ShouldClose: box.ShouldClose,
	}
}

func (box *MessageBoxYesNo) RuntimeOptions() GeneratedWindowRuntimeOptions {
	return GeneratedWindowRuntimeOptions{
		InitialFocusName: "no_btn",
		OnButton: func(name string) {
			switch name {
			case "yes_btn":
				box.close("yes")
			case "no_btn":
				box.close("no")
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				box.close("no")
				return true
			}
			return false
		},
		ShouldClose: box.ShouldClose,
	}
}

func (box *MessageBoxOkCancel) RuntimeOptions() GeneratedWindowRuntimeOptions {
	return GeneratedWindowRuntimeOptions{
		InitialFocusName: "cancel_btn",
		OnButton: func(name string) {
			switch name {
			case "ok_btn":
				box.close("ok")
			case "cancel_btn":
				box.close("cancel")
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				box.close("cancel")
				return true
			}
			return false
		},
		ShouldClose: box.ShouldClose,
	}
}

func (box *MessageBoxOk) RuntimeOptions() GeneratedWindowRuntimeOptions {
	return GeneratedWindowRuntimeOptions{
		InitialFocusName: "ok_btn",
		OnButton: func(name string) {
			if name == "ok_btn" {
				box.close("ok")
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				box.close("ok")
				return true
			}
			return false
		},
		ShouldClose: box.ShouldClose,
	}
}

func (box *MessageBoxYesNoCancel) Result() string {
	return box.result
}

func (box *MessageBoxYesNo) Result() string {
	return box.result
}

func (box *MessageBoxOkCancel) Result() string {
	return box.result
}

func (box *MessageBoxOk) Result() string {
	return box.result
}

func (box *MessageBoxYesNoCancel) close(result string) {
	box.result = result
	box.RequestClose()
}

func (box *MessageBoxYesNo) close(result string) {
	box.result = result
	box.RequestClose()
}

func (box *MessageBoxOkCancel) close(result string) {
	box.result = result
	box.RequestClose()
}

func (box *MessageBoxOk) close(result string) {
	box.result = result
	box.RequestClose()
}

func NewFileBrowser(rootDir string, startPath string, mode string, onClose func(string), initialFilename ...string) *FileBrowser {
	browser := &FileBrowser{
		GeneratedWindowBase: NewGeneratedWindowBase("File Browser"),
		rootDir:             cleanExistingDir(rootDir),
		mode:                mode,
		onClose:             onClose,
	}
	if browser.mode == "" {
		browser.mode = "open"
	}
	start := startPath
	if start == "" {
		start = browser.rootDir
	}
	if len(initialFilename) > 0 {
		browser.initialFilename = initialFilename[0]
	}
	if info, err := os.Stat(start); err == nil && !info.IsDir() {
		browser.currentDir = browser.clampDir(filepath.Dir(start))
		if browser.initialFilename == "" {
			browser.initialFilename = filepath.Base(start)
		}
	} else {
		browser.currentDir = browser.clampDir(start)
	}
	browser.SetGeneratedKind("dialog")
	browser.SetGeneratedFocusable(false)
	browser.SetGeneratedWindowStyle(dialogWindowStyle())
	browser.SetMCPMetadata(true, "FileBrowser", "", "File Browser UI source.")
	browser.SetGeneratedLayout(fileBrowserLayout())
	browser.DialogHeader = newDialogHeaderLabel("Open File")
	browser.PathLabel = newDialogTextLabel("path_label", ".")
	browser.Entries = NewListBox("entries", nil, false)
	browser.Entries.SetStyle(Style{Background: NewColor("#0d1524"), Color: NewColor("#cbd5e1")})
	browser.Entries.SetFocusStyle(Style{Background: NewColor("#1d2f4d"), Color: NewColor("#ffffff")})
	browser.Entries.SetEditStyle(Style{Background: NewColor("#243a5c"), Color: NewColor("#cbd5e1")})
	browser.Entries.SetSelectedStyle(Style{Background: NewColor("#2563eb"), Color: NewColor("#ffffff")})
	browser.Entries.SetActiveStyle(Style{Background: NewColor("#DDDDDD99"), Color: NewColor("#FFFFFF")})
	browser.Entries.SetDisabledStyle(Style{Color: NewColor("#64748b")})
	browser.Filename = NewTextInput("filename", "", 120)
	browser.Filename.SetStyle(Style{Background: NewColor("#0d1524"), Color: NewColor("#e5e7eb")})
	browser.Filename.SetFocusStyle(Style{Background: NewColor("#243a5c"), Color: NewColor("#ffffff")})
	browser.Filename.SetEditStyle(Style{Background: NewColor("#2d456d"), Color: NewColor("#ffffff")})
	browser.Filename.SetCursorStyle(Style{Background: NewColor("#ffffff"), Color: NewColor("#0e1117")})
	browser.OpenBtn = newDialogButton("open_btn", "Open")
	browser.CloseBtn = newDialogButton("close_btn", "Close")
	browser.AddElement(browser.DialogHeader)
	browser.AddElement(browser.PathLabel)
	browser.AddElement(browser.Entries)
	browser.AddElement(browser.Filename)
	browser.AddElement(browser.OpenBtn)
	browser.AddElement(browser.CloseBtn)
	if browser.mode == "save" {
		browser.DialogHeader.SetText("Save As")
		browser.OpenBtn.SetTitle("Save")
		if browser.initialFilename != "" {
			browser.Filename.SetValue(browser.initialFilename)
			browser.Filename.SetCursor(len([]rune(browser.Filename.Value)))
		}
	}
	browser.RefreshEntries()
	return browser
}

func (browser *FileBrowser) RuntimeOptions() GeneratedWindowRuntimeOptions {
	return GeneratedWindowRuntimeOptions{
		InitialFocusName: "entries",
		StartInEditMode:  true,
		OnButton: func(name string) {
			switch name {
			case "open_btn":
				if browser.mode == "save" {
					browser.AcceptFilename()
				} else {
					browser.AcceptCurrent()
				}
			case "close_btn":
				browser.Close("")
			}
		},
		OnTextChanged: func(name string, _ string) {
			if name == "entries" {
				browser.PreviewSelected()
			} else if name == "filename" {
				browser.UpdateOpenEnabled()
			}
		},
		OnSelectionChanged: func(name string, _ []string) {
			if name == "entries" {
				browser.PreviewSelected()
			}
		},
		OnTextConfirmed: func(name string, _ string) {
			if name == "entries" {
				browser.AcceptCurrent()
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				browser.Close("")
				return true
			}
			return false
		},
		ShouldClose: browser.ShouldClose,
	}
}

func (browser *FileBrowser) StackFrameOptions() GeneratedWindowFrameOptions {
	options := browser.RuntimeOptions()
	return GeneratedWindowFrameOptions{
		ClassName:                 "FileBrowser",
		InitialFocusName:          options.InitialFocusName,
		StartInEditMode:           options.StartInEditMode,
		KeepEditModeAfterConfirm:  options.KeepEditModeAfterConfirm,
		KeepEditModeAfterEscape:   options.KeepEditModeAfterEscape,
		OnKey:                     options.OnKey,
		OnKeyBeforeFocusedElement: options.OnKeyBeforeFocusedElement,
		OnButton:                  options.OnButton,
		OnTextChanged:             options.OnTextChanged,
		OnTextConfirmed:           options.OnTextConfirmed,
		OnSelectionChanged:        options.OnSelectionChanged,
		ShouldClose:               options.ShouldClose,
	}
}

func (browser *FileBrowser) CurrentDir() string {
	return browser.currentDir
}

func (browser *FileBrowser) Closed() bool {
	return browser.closed
}

func (browser *FileBrowser) Result() string {
	return browser.result
}

func (browser *FileBrowser) SetExtensionFilter(filter string) {
	browser.filter = strings.TrimSpace(filter)
	browser.RefreshEntries()
}

func (browser *FileBrowser) Close(path string) {
	browser.result = path
	browser.closed = true
	browser.RequestClose()
	if browser.onClose != nil {
		browser.onClose(path)
	}
}

func (browser *FileBrowser) RefreshEntries() {
	rows := []string{".."}
	var disabledRows []string
	entries, err := os.ReadDir(browser.currentDir)
	if err == nil {
		var dirs []string
		var files []string
		for _, entry := range entries {
			name := entry.Name()
			if entry.IsDir() {
				dirs = append(dirs, name+"/")
			} else {
				files = append(files, name)
			}
		}
		sort.Strings(dirs)
		sort.Strings(files)
		rows = append(rows, dirs...)
		rows = append(rows, files...)
		for _, file := range files {
			if !browser.pathMatchesFilter(filepath.Join(browser.currentDir, file)) {
				disabledRows = append(disabledRows, file)
			}
		}
	}
	browser.PathLabel.SetText(browser.currentDir)
	browser.Entries.SetOptions(rows)
	browser.Entries.SetDisabledValues(disabledRows)
	selectedIndex := 0
	if browser.initialFilename != "" {
		for index, row := range rows {
			if row == browser.initialFilename {
				selectedIndex = index
				break
			}
		}
	}
	browser.Entries.SetSelectedIndex(selectedIndex)
	browser.PreviewSelected()
}

func (browser *FileBrowser) AcceptCurrent() bool {
	path := browser.SelectedPath()
	if path == "" {
		return false
	}
	if info, err := os.Stat(path); err == nil && info.IsDir() {
		browser.currentDir = browser.clampDir(path)
		browser.RefreshEntries()
		return true
	}
	name := strings.TrimSpace(browser.Filename.Value)
	if name == "" {
		name = filepath.Base(path)
	}
	namedPath := filepath.Join(browser.currentDir, name)
	if browser.mode == "open" {
		if info, err := os.Stat(namedPath); err == nil && !info.IsDir() && browser.pathMatchesFilter(namedPath) {
			browser.Close(namedPath)
			return true
		}
		return false
	}
	return browser.AcceptFilename()
}

func (browser *FileBrowser) AcceptFilename() bool {
	name := strings.TrimSpace(browser.Filename.Value)
	if name == "" {
		return false
	}
	path := filepath.Join(browser.currentDir, name)
	if info, err := os.Stat(path); err == nil && info.IsDir() {
		return false
	}
	browser.Close(path)
	return true
}

func (browser *FileBrowser) SelectEntry(index int) {
	browser.Entries.SetSelectedIndex(index)
	browser.PreviewSelected()
}

func (browser *FileBrowser) PreviewSelected() {
	values := browser.Entries.SelectedValues()
	if len(values) == 0 {
		browser.UpdateOpenEnabled()
		return
	}
	selected := values[0]
	if selected != ".." {
		browser.Filename.SetValue(strings.TrimSuffix(selected, "/"))
		browser.Filename.SetCursor(len([]rune(browser.Filename.Value)))
	} else if browser.mode == "open" {
		browser.Filename.SetValue("")
	}
	browser.UpdateOpenEnabled()
}

func (browser *FileBrowser) SelectedPath() string {
	values := browser.Entries.SelectedValues()
	if len(values) == 0 {
		return ""
	}
	selected := values[0]
	if selected == ".." {
		return browser.clampDir(filepath.Dir(browser.currentDir))
	}
	return filepath.Join(browser.currentDir, strings.TrimSuffix(selected, "/"))
}

func (browser *FileBrowser) UpdateOpenEnabled() {
	if browser.mode == "save" {
		browser.OpenBtn.SetEnabled(strings.TrimSpace(browser.Filename.Value) != "")
		return
	}
	path := browser.SelectedPath()
	if path == "" {
		browser.OpenBtn.SetEnabled(false)
		return
	}
	if info, err := os.Stat(path); err == nil {
		browser.OpenBtn.SetEnabled(info.IsDir() || browser.pathMatchesFilter(path))
		return
	}
	browser.OpenBtn.SetEnabled(false)
}

func (browser *FileBrowser) pathMatchesFilter(path string) bool {
	if browser.filter == "" {
		return true
	}
	extension := strings.TrimPrefix(strings.ToLower(filepath.Ext(path)), ".")
	if extension == "" {
		return false
	}
	matched, err := regexp.MatchString("(?i)^"+browser.filter+"$", extension)
	if err == nil {
		return matched
	}
	for _, part := range strings.Split(browser.filter, "|") {
		allowed := strings.TrimPrefix(strings.ToLower(strings.TrimSpace(part)), ".")
		if allowed != "" && extension == allowed {
			return true
		}
	}
	return false
}

func (browser *FileBrowser) clampDir(path string) string {
	if path == "" {
		return browser.rootDir
	}
	cleaned, err := filepath.Abs(path)
	if err != nil {
		return browser.rootDir
	}
	if info, err := os.Stat(cleaned); err != nil || !info.IsDir() {
		return browser.rootDir
	}
	if !strings.HasPrefix(cleaned, browser.rootDir) {
		return browser.rootDir
	}
	return cleaned
}

func cleanExistingDir(path string) string {
	if path == "" {
		if cwd, err := os.Getwd(); err == nil {
			return cwd
		}
		return "."
	}
	cleaned, err := filepath.Abs(path)
	if err != nil {
		return path
	}
	if info, err := os.Stat(cleaned); err == nil && info.IsDir() {
		return cleaned
	}
	return filepath.Dir(cleaned)
}

func newDialogHeaderLabel(text string) *Label {
	label := NewLabel("dialog_header", text)
	label.SetStyle(Style{Background: NewColor("#1d314c"), Color: NewColor("#cbd5e1"), TextAlign: "center"})
	label.SetCursorStyle(Style{Background: NewColor("#b8d7ff"), Color: NewColor("#000000")})
	return label
}

func newDialogTextLabel(name string, text string) *Label {
	label := NewLabel(name, text)
	label.SetStyle(Style{Background: NewColor("transparent"), Color: NewColor("#cbd5e1")})
	label.SetCursorStyle(Style{Background: NewColor("#b8d7ff"), Color: NewColor("#000000")})
	return label
}

func newDialogButton(name string, title string) *Button {
	button := NewButton(name, title)
	button.SetStyle(Style{Background: NewColor("#334155"), Color: NewColor("#e2e8f0")})
	button.SetFocusStyle(Style{Background: NewColor("#2b5f9f"), Color: NewColor("#ffffff")})
	button.SetDisabledStyle(Style{Background: NewColor("#1f2937"), Color: NewColor("#64748b")})
	return button
}

func dialogWindowStyle() Style {
	return Style{
		Background:            NewColor("#162033"),
		BorderColor:           NewColor("transparent"),
		BorderWidthHorizontal: Int(0),
		BorderWidthVertical:   Int(0),
	}
}

func dialogLayoutItem(content string, row int, col int, width int, height int, charsWidth int, charsHeight int, marginRight int, marginBottom int, cellWidth int, cellHeight int, elementStyle Style) LayoutItem {
	widthMode := "auto"
	if width == Expanded {
		widthMode = "expanded"
	}
	return LayoutItem{
		Row:             row,
		Col:             col,
		CellRow:         2,
		CellCol:         0,
		CellCharsWidth:  cellWidth,
		CellCharsHeight: cellHeight,
		CellWidth:       cellWidth,
		CellHeight:      cellHeight,
		CellWidthMode:   "auto",
		CellHeightMode:  "auto",
		Width:           width,
		Height:          height,
		WidthMode:       widthMode,
		HeightMode:      "auto",
		CharsWidth:      charsWidth,
		CharsHeight:     charsHeight,
		MarginRight:     marginRight,
		MarginBottom:    marginBottom,
		Content:         content,
		CellStyle:       NewStyle(),
		ElementStyle:    elementStyle,
	}
}

func dialogHeaderLayoutItem(content string, width int, elementStyle Style) LayoutItem {
	return LayoutItem{
		Row:             0,
		Col:             0,
		CellRow:         0,
		CellCol:         0,
		CellCharsWidth:  width,
		CellCharsHeight: 1,
		CellWidth:       width,
		CellHeight:      1,
		CellWidthMode:   "auto",
		CellHeightMode:  "auto",
		Width:           Expanded,
		Height:          1,
		WidthMode:       "expanded",
		HeightMode:      "auto",
		CharsWidth:      width,
		CharsHeight:     1,
		Content:         content,
		CellStyle:       NewStyle(),
		ElementStyle:    elementStyle,
	}
}

func messageBoxYesNoCancelLayout() []LayoutItem {
	header := Style{Background: NewColor("#1d314c"), Color: NewColor("#cbd5e1"), TextAlign: "center"}
	text := Style{Background: NewColor("transparent"), Color: NewColor("#cbd5e1")}
	button := Style{Background: NewColor("#334155"), Color: NewColor("#e2e8f0")}
	return []LayoutItem{
		dialogHeaderLayoutItem("dialog_header", 46, header),
		dialogLayoutItem("message", 1, 2, Expanded, 2, 42, 2, 2, 3, 46, 6, text),
		dialogLayoutItem("yes_btn", 4, 2, 12, 1, 12, 1, 32, 1, 46, 6, button),
		dialogLayoutItem("no_btn", 4, 16, 13, 1, 13, 1, 17, 1, 46, 6, button),
		dialogLayoutItem("cancel_btn", 4, 31, Expanded, 1, 13, 1, 2, 1, 46, 6, button),
	}
}

func messageBoxYesNoLayout() []LayoutItem {
	header := Style{Background: NewColor("#1d314c"), Color: NewColor("#cbd5e1"), TextAlign: "center"}
	text := Style{Background: NewColor("transparent"), Color: NewColor("#cbd5e1")}
	button := Style{Background: NewColor("#334155"), Color: NewColor("#e2e8f0")}
	return []LayoutItem{
		dialogHeaderLayoutItem("dialog_header", 44, header),
		dialogLayoutItem("message", 1, 2, Expanded, 2, 40, 2, 2, 3, 44, 6, text),
		dialogLayoutItem("yes_btn", 4, 3, 18, 1, 18, 1, 23, 1, 44, 6, button),
		dialogLayoutItem("no_btn", 4, 23, Expanded, 1, 18, 1, 3, 1, 44, 6, button),
	}
}

func messageBoxOkCancelLayout() []LayoutItem {
	header := Style{Background: NewColor("#1d314c"), Color: NewColor("#cbd5e1"), TextAlign: "center"}
	text := Style{Background: NewColor("transparent"), Color: NewColor("#cbd5e1")}
	button := Style{Background: NewColor("#334155"), Color: NewColor("#e2e8f0")}
	return []LayoutItem{
		dialogHeaderLayoutItem("dialog_header", 44, header),
		dialogLayoutItem("message", 1, 2, Expanded, 2, 40, 2, 2, 3, 44, 6, text),
		dialogLayoutItem("ok_btn", 4, 3, 18, 1, 18, 1, 23, 1, 44, 6, button),
		dialogLayoutItem("cancel_btn", 4, 23, Expanded, 1, 18, 1, 3, 1, 44, 6, button),
	}
}

func messageBoxOkLayout() []LayoutItem {
	header := Style{Background: NewColor("#1d314c"), Color: NewColor("#cbd5e1"), TextAlign: "center"}
	text := Style{Background: NewColor("transparent"), Color: NewColor("#cbd5e1")}
	button := Style{Background: NewColor("#334155"), Color: NewColor("#e2e8f0")}
	return []LayoutItem{
		dialogHeaderLayoutItem("dialog_header", 44, header),
		dialogLayoutItem("message", 1, 2, Expanded, 2, 40, 2, 2, 3, 44, 6, text),
		dialogLayoutItem("ok_btn", 4, 2, Expanded, 1, 40, 1, 2, 1, 44, 6, button),
	}
}

func fileBrowserLayout() []LayoutItem {
	header := Style{Background: NewColor("#1d314c"), Color: NewColor("#cbd5e1"), TextAlign: "center"}
	text := Style{Background: NewColor("transparent"), Color: NewColor("#cbd5e1")}
	list := Style{Background: NewColor("#0d1524"), Color: NewColor("#cbd5e1")}
	input := Style{Background: NewColor("#0d1524"), Color: NewColor("#e5e7eb")}
	button := Style{Background: NewColor("#334155"), Color: NewColor("#e2e8f0")}
	return []LayoutItem{
		dialogHeaderLayoutItem("dialog_header", 48, header),
		dialogLayoutItem("path_label", 1, 3, Expanded, 1, 42, 1, 3, 12, 48, 14, text),
		dialogLayoutItem("entries", 3, 3, Expanded, 6, 42, 6, 3, 5, 48, 14, list),
		dialogLayoutItem("\"File:\"", 10, 3, 9, 1, 9, 1, 36, 3, 48, 14, text),
		dialogLayoutItem("filename", 10, 14, Expanded, 1, 31, 1, 3, 3, 48, 14, input),
		dialogLayoutItem("open_btn", 12, 3, 20, 1, 20, 1, 25, 1, 48, 14, button),
		dialogLayoutItem("close_btn", 12, 25, Expanded, 1, 20, 1, 3, 1, 48, 14, button),
	}
}
