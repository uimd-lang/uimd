package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	uimd "uimd"
)

const (
	textEditorNotesDirEnv       = "UI_TEXT_EDITOR_NOTES_DIR"
	textEditorDefaultNoteEnv    = "UI_TEXT_EDITOR_DEFAULT_NOTE"
	textEditorBrowserRootEnv    = "UI_TEXT_EDITOR_BROWSER_ROOT_DIR"
	textEditorBrowserDirEnv     = "UI_TEXT_EDITOR_BROWSER_DIR"
	textEditorDefaultFileName   = "welcome.txt"
	textEditorUntitledFileName  = "untitled.txt"
	textEditorDefaultStatusText = "Ready"
)

type TextEditorApp struct {
	*TextEditorUI
	stack           uimd.GeneratedWindowStack
	currentPath     string
	loadedText      string
	pendingAction   func()
	afterSave       func()
	browser         *uimd.FileBrowser
	unsavedDialog   *uimd.MessageBoxYesNoCancel
	overwriteDialog *uimd.MessageBoxYesNo
	overwritePath   string
	finished        bool
}

func NewTextEditorApp() *TextEditorApp {
	app := &TextEditorApp{TextEditorUI: NewTextEditorUI()}
	app.SetEventHandler(app)
	app.Status.SetText(textEditorDefaultStatusText)
	app.loadPath(filepath.Join(notesDir(), defaultNoteName()))
	return app
}

func (app *TextEditorApp) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.TextEditorUI.RuntimeOptions()
	options.WindowStack = &app.stack
	options.ShouldClose = app.ShouldClose
	return options
}

func (app *TextEditorApp) ShouldClose() bool {
	return app.finished || app.GeneratedWindowBase.ShouldClose()
}

func (app *TextEditorApp) OnNewBtnClick() {
	app.withSavedChanges(func() {
		app.newFile()
	})
}

func (app *TextEditorApp) OnOpenBtnClick() {
	app.withSavedChanges(func() {
		app.openBrowser()
	})
}

func (app *TextEditorApp) OnSaveBtnClick() {
	app.saveOrSaveAs()
}

func (app *TextEditorApp) OnSaveAsBtnClick() {
	app.saveAsBrowser()
}

func (app *TextEditorApp) OnQuitBtnClick() {
	app.withSavedChanges(func() {
		app.finished = true
		app.RequestClose()
	})
}

func (app *TextEditorApp) OnEditorChange(string) {
	app.refreshStatus("Modified")
}

func (app *TextEditorApp) newFile() {
	app.currentPath = ""
	app.Filename.SetText(textEditorUntitledFileName)
	app.setEditorText("")
	app.loadedText = ""
	app.refreshStatus("New file")
}

func (app *TextEditorApp) loadPath(path string) {
	absolute := absolutePath(path)
	text := readTextFile(absolute)
	app.currentPath = absolute
	app.Filename.SetText(displayFileName(absolute))
	app.setEditorText(text)
	app.loadedText = text
	app.refreshStatus("Loaded")
}

func (app *TextEditorApp) saveOrSaveAs() {
	if app.currentPath == "" {
		app.saveAsBrowser()
		return
	}
	app.savePath(app.currentPath)
}

func (app *TextEditorApp) savePath(path string) {
	absolute := absolutePath(path)
	_ = os.WriteFile(absolute, []byte(app.Editor.Value), 0o644)
	app.currentPath = absolute
	app.Filename.SetText(displayFileName(absolute))
	app.loadedText = app.Editor.Value
	app.refreshStatus("Saved")
}

func (app *TextEditorApp) openBrowser() {
	app.browser = uimd.NewFileBrowser(browserRootDir(), defaultBrowserDir(), "open", func(path string) {
		if path != "" {
			app.loadPath(path)
		}
	})
	app.pushBrowserFrame()
}

func (app *TextEditorApp) saveAsBrowser() {
	start := notesDir()
	if app.currentPath != "" {
		start = filepath.Dir(app.currentPath)
	}
	if !pathStartsWith(start, browserRootDir()) {
		start = defaultBrowserDir()
	}
	initialName := textEditorUntitledFileName
	if app.currentPath != "" {
		initialName = filepath.Base(app.currentPath)
	}
	app.browser = uimd.NewFileBrowser(browserRootDir(), start, "save", func(path string) {
		action := app.afterSave
		app.afterSave = nil
		if path != "" {
			app.savePath(path)
			if action != nil {
				action()
			}
		} else if action != nil {
			app.refreshStatus("Cancelled")
		}
	}, initialName)
	app.pushBrowserFrame()
}

func (app *TextEditorApp) withSavedChanges(action func()) {
	app.afterSave = nil
	if !app.modified() {
		action()
		return
	}
	app.pendingAction = action
	app.unsavedDialog = uimd.NewMessageBoxYesNoCancel("Unsaved Changes", "Save current file before continuing?")
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNoCancel",
		InitialFocusName: "cancel_btn",
		OnButton: func(name string) {
			switch name {
			case "yes_btn":
				app.closeUnsaved("yes")
			case "no_btn":
				app.closeUnsaved("no")
			case "cancel_btn":
				app.closeUnsaved("cancel")
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				app.closeUnsaved("cancel")
				return true
			}
			return false
		},
	}
	app.stack.Push(app.unsavedDialog, frame)
}

func (app *TextEditorApp) modified() bool {
	return app.Editor.Value != app.loadedText
}

func (app *TextEditorApp) closeUnsaved(choice string) {
	if app.unsavedDialog != nil {
		app.stack.Remove(app.unsavedDialog)
		app.unsavedDialog = nil
	}
	action := app.pendingAction
	app.pendingAction = nil
	if choice == "cancel" || action == nil {
		app.refreshStatus("Cancelled")
		return
	}
	if choice == "yes" {
		if app.currentPath == "" {
			app.afterSave = action
			app.saveAsBrowser()
			return
		}
		app.saveOrSaveAs()
		action()
		return
	}
	action()
}

func (app *TextEditorApp) pushBrowserFrame() {
	if app.browser == nil {
		return
	}
	frame := app.browser.StackFrameOptions()
	frame.ClassName = "FileBrowser"
	frame.InitialFocusName = "entries"
	frame.StartInEditMode = true
	frame.OnButton = func(name string) {
		switch name {
		case "open_btn":
			app.acceptBrowserOpenButton()
		case "close_btn":
			app.closeBrowser("")
		}
	}
	frame.OnTextChanged = func(name string, _ string) {
		if app.browser != nil && name == "entries" {
			app.browser.SelectEntry(app.browser.Entries.SelectedIndex)
		}
		if app.browser != nil && name == "filename" {
			app.browser.UpdateOpenEnabled()
		}
	}
	frame.OnSelectionChanged = func(name string, _ []string) {
		if app.browser != nil && name == "entries" {
			app.browser.SelectEntry(app.browser.Entries.SelectedIndex)
		}
	}
	frame.OnTextConfirmed = func(name string, _ string) {
		if app.browser != nil && name == "entries" {
			app.browser.SelectEntry(app.browser.Entries.SelectedIndex)
			app.acceptBrowserCurrent()
		}
	}
	frame.OnKey = func(key string) bool {
		if key == "Escape" {
			app.closeBrowser("")
			return true
		}
		return false
	}
	app.stack.Push(app.browser, frame)
}

func (app *TextEditorApp) acceptBrowserCurrent() {
	if app.browser == nil {
		return
	}
	app.browser.AcceptCurrent()
	app.closeBrowserIfClosed()
}

func (app *TextEditorApp) acceptBrowserOpenButton() {
	if app.browser == nil {
		return
	}
	if app.browser.OpenBtn.Title == "Save" {
		name := strings.TrimSpace(app.browser.Filename.Value)
		if name == "" {
			return
		}
		path := filepath.Join(app.browser.CurrentDir(), name)
		if info, err := os.Stat(path); err == nil && !info.IsDir() {
			app.overwritePath = path
			app.overwriteDialog = uimd.NewMessageBoxYesNo("Overwrite File", "File already exists:\n"+path+"\n\nOverwrite it?")
			app.pushOverwriteFrame()
			return
		}
		app.closeBrowser(path)
		return
	}
	name := strings.TrimSpace(app.browser.Filename.Value)
	if name != "" {
		path := filepath.Join(app.browser.CurrentDir(), name)
		if info, err := os.Stat(path); err == nil && !info.IsDir() {
			app.closeBrowser(path)
			return
		}
	}
	app.acceptBrowserCurrent()
}

func (app *TextEditorApp) pushOverwriteFrame() {
	if app.overwriteDialog == nil {
		return
	}
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNo",
		InitialFocusName: "no_btn",
		OnButton: func(name string) {
			switch name {
			case "yes_btn":
				app.closeOverwrite("yes")
			case "no_btn":
				app.closeOverwrite("no")
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				app.closeOverwrite("no")
				return true
			}
			return false
		},
	}
	app.stack.Push(app.overwriteDialog, frame)
}

func (app *TextEditorApp) closeOverwrite(choice string) {
	if app.overwriteDialog != nil {
		app.stack.Remove(app.overwriteDialog)
		app.overwriteDialog = nil
	}
	if choice == "yes" && app.overwritePath != "" {
		app.closeBrowser(app.overwritePath)
	}
	app.overwritePath = ""
}

func (app *TextEditorApp) closeBrowserIfClosed() {
	if app.browser != nil && app.browser.Closed() {
		app.stack.Remove(app.browser)
		app.browser = nil
	}
}

func (app *TextEditorApp) closeBrowser(path string) {
	if app.browser == nil {
		return
	}
	browser := app.browser
	browser.Close(path)
	app.stack.Remove(browser)
	app.browser = nil
}

func (app *TextEditorApp) setEditorText(text string) {
	app.Editor.SetValue(text)
	app.Editor.SetSelection(0, 0)
}

func (app *TextEditorApp) refreshStatus(prefix string) {
	text := app.Editor.Value
	modifiedMarker := ""
	if app.modified() {
		modifiedMarker = "*"
	}
	lineCount := 0
	if text != "" {
		lineCount = strings.Count(text, "\n") + 1
	}
	app.Status.SetText(fmt.Sprintf("%s: %s%s - %d lines, %d chars", prefix, app.Filename.Text, modifiedMarker, lineCount, len(text)))
}

func notesDir() string {
	if value := os.Getenv(textEditorNotesDirEnv); value != "" {
		return absolutePath(value)
	}
	return filepath.Join(currentDir(), "notes")
}

func defaultNoteName() string {
	if value := os.Getenv(textEditorDefaultNoteEnv); value != "" {
		return value
	}
	return textEditorDefaultFileName
}

func browserRootDir() string {
	if value := os.Getenv(textEditorBrowserRootEnv); value != "" {
		return absolutePath(value)
	}
	return userHomeDir()
}

func defaultBrowserDir() string {
	if value := os.Getenv(textEditorBrowserDirEnv); value != "" {
		return absolutePath(value)
	}
	desktop := filepath.Join(userHomeDir(), "Desktop")
	if info, err := os.Stat(desktop); err == nil && info.IsDir() {
		return desktop
	}
	return userHomeDir()
}

func userHomeDir() string {
	if home := os.Getenv("HOME"); home != "" {
		return home
	}
	return currentDir()
}

func currentDir() string {
	if dir, err := os.Getwd(); err == nil {
		return dir
	}
	return "."
}

func pathStartsWith(path string, root string) bool {
	absolutePathValue := absolutePath(path)
	absoluteRoot := absolutePath(root)
	relative, err := filepath.Rel(absoluteRoot, absolutePathValue)
	return err == nil && relative != ".." && !strings.HasPrefix(relative, "../")
}

func readTextFile(path string) string {
	data, err := os.ReadFile(path)
	if err != nil {
		return ""
	}
	return string(data)
}

func displayFileName(path string) string {
	name := filepath.Base(path)
	if name == "." || name == string(filepath.Separator) {
		return path
	}
	return name
}

func absolutePath(path string) string {
	absolute, err := filepath.Abs(path)
	if err != nil {
		return path
	}
	return absolute
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewTextEditorApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
