package main

import (
	"os"
	"path/filepath"
	"sort"
	"strings"
	"unicode"

	documentview "uimd_examples/markdown_viewer/document_view"

	uimd "uimd"
)

var preferredDocuments = []string{
	"overview.markdown",
	"styles.markdown",
	"tables.markdown",
	"code.markdown",
}

type MarkdownViewerApp struct {
	*MarkdownViewerUI
	view      *documentview.DocumentView
	documents map[string]string
	finished  bool
}

func NewMarkdownViewerApp() *MarkdownViewerApp {
	view := documentview.NewDocumentView()
	app := &MarkdownViewerApp{
		MarkdownViewerUI: NewMarkdownViewerUI(),
		view:             view,
		documents:        loadDocuments(),
	}
	app.SetEventHandler(app)
	app.Viewer.SetChild(view)
	names := app.documentNames()
	app.Docs.SetOptions(names)
	app.Docs.SetSelectedIndex(0)
	app.ShowSelected()
	return app
}

func (app *MarkdownViewerApp) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.MarkdownViewerUI.RuntimeOptions()
	options.InitialFocusName = "docs"
	options.StartInEditMode = true
	options.ShouldClose = app.ShouldClose
	options.OnFocusChanged = func(name string, focused bool) {
		if focused {
			app.Focus(name)
		}
	}
	return options
}

func (app *MarkdownViewerApp) ShouldClose() bool {
	return app.finished || app.GeneratedWindowBase.ShouldClose()
}

func (app *MarkdownViewerApp) OnUpBtnClick() {
	app.Scroll(1)
}

func (app *MarkdownViewerApp) OnDownBtnClick() {
	app.Scroll(-1)
}

func (app *MarkdownViewerApp) OnCloseBtnClick() {
	app.finished = true
	app.RequestClose()
}

func (app *MarkdownViewerApp) OnDocsSelectionChange([]string) {
	app.ShowSelected()
}

func (app *MarkdownViewerApp) Focus(name string) {
	if name == "viewer" {
		app.Status.SetText("viewer focus - Enter scroll mode, Esc navigation, arrows/PageUp/PageDown/Home/End scroll")
	} else if name == "up_btn" || name == "down_btn" {
		app.Status.SetText("buttons scroll the viewer; Right focuses viewer")
	}
}

func (app *MarkdownViewerApp) ShowSelected() {
	if len(app.Docs.Options) == 0 {
		return
	}
	index := app.Docs.SelectedIndex
	if index < 0 || index >= len(app.Docs.Options) {
		index = 0
	}
	name := app.Docs.Options[index]
	blocks := parseMarkdown(app.documents[name])
	app.view.SetBlocks(blocks)
	app.Status.SetText(name + " - " + intString(len(blocks)) + " blocks - arrows preview")
}

func (app *MarkdownViewerApp) Scroll(direction int) {
	app.view.Scroll(direction)
}

func (app *MarkdownViewerApp) documentNames() []string {
	names := []string{}
	seen := map[string]bool{}
	for _, preferred := range preferredDocuments {
		if _, ok := app.documents[preferred]; ok {
			names = append(names, preferred)
			seen[preferred] = true
		}
	}
	extras := []string{}
	for name := range app.documents {
		if !seen[name] {
			extras = append(extras, name)
		}
	}
	sort.Strings(extras)
	return append(names, extras...)
}

func docsDir() string {
	if executable, err := os.Executable(); err == nil {
		candidate := filepath.Join(filepath.Dir(executable), "docs")
		if info, statErr := os.Stat(candidate); statErr == nil && info.IsDir() {
			return candidate
		}
	}
	return "docs"
}

func loadDocuments() map[string]string {
	documents := map[string]string{}
	for _, name := range preferredDocuments {
		if text, err := os.ReadFile(filepath.Join(docsDir(), name)); err == nil {
			documents[name] = string(text)
		}
	}
	entries, err := os.ReadDir(docsDir())
	if err != nil {
		return documents
	}
	for _, entry := range entries {
		name := entry.Name()
		if entry.IsDir() || filepath.Ext(name) != ".markdown" {
			continue
		}
		if _, exists := documents[name]; exists {
			continue
		}
		if text, readErr := os.ReadFile(filepath.Join(docsDir(), name)); readErr == nil {
			documents[name] = string(text)
		}
	}
	return documents
}

func parseMarkdown(text string) []documentview.MarkdownBlock {
	lines := splitLines(text)
	blocks := []documentview.MarkdownBlock{}
	paragraph := []string{}
	flushParagraph := func() {
		value := strings.TrimSpace(strings.Join(paragraph, "\n"))
		if value != "" {
			blocks = append(blocks, documentview.MarkdownBlock{Kind: "paragraph", Text: value})
		}
		paragraph = nil
	}
	for index := 0; index < len(lines); {
		line := lines[index]
		if strings.TrimSpace(line) == "" {
			flushParagraph()
			index++
			continue
		}
		if strings.HasPrefix(line, "```") {
			flushParagraph()
			language := strings.TrimSpace(strings.TrimPrefix(line, "```"))
			if language == "" {
				language = "text"
			}
			index++
			codeLines := []string{}
			for index < len(lines) && !strings.HasPrefix(lines[index], "```") {
				codeLines = append(codeLines, lines[index])
				index++
			}
			if index < len(lines) {
				index++
			}
			blocks = append(blocks, documentview.MarkdownBlock{Kind: "code", Text: strings.Join(codeLines, "\n"), Language: language})
			continue
		}
		if strings.HasPrefix(line, "#") {
			flushParagraph()
			level := headingLevel(line)
			blocks = append(blocks, documentview.MarkdownBlock{Kind: "heading", Level: level, Text: strings.TrimSpace(strings.TrimLeft(line, "#"))})
			index++
			continue
		}
		if strings.HasPrefix(line, "|") && index+1 < len(lines) && isTableSeparator(lines[index+1]) {
			flushParagraph()
			tableLines := []string{line, lines[index+1]}
			index += 2
			for index < len(lines) && strings.HasPrefix(lines[index], "|") {
				tableLines = append(tableLines, lines[index])
				index++
			}
			blocks = append(blocks, documentview.MarkdownBlock{Kind: "table", Text: strings.Join(tableLines, "\n")})
			continue
		}
		paragraph = append(paragraph, line)
		index++
	}
	flushParagraph()
	return blocks
}

func splitLines(text string) []string {
	text = strings.ReplaceAll(strings.ReplaceAll(text, "\r\n", "\n"), "\r", "\n")
	if text == "" {
		return []string{""}
	}
	return strings.Split(text, "\n")
}

func isTableSeparator(line string) bool {
	value := strings.TrimSpace(line)
	if len(value) < 3 || !strings.HasPrefix(value, "|") || !strings.HasSuffix(value, "|") {
		return false
	}
	for _, ch := range value {
		if ch != '|' && ch != ' ' && ch != '-' && ch != ':' {
			return false
		}
	}
	return true
}

func headingLevel(line string) int {
	level := 0
	for _, ch := range line {
		if ch != '#' {
			break
		}
		level++
	}
	if level < 1 {
		return 1
	}
	return level
}

func intString(value int) string {
	if value == 0 {
		return "0"
	}
	digits := []rune{}
	for value > 0 {
		digits = append([]rune{rune('0' + value%10)}, digits...)
		value /= 10
	}
	return string(digits)
}

func smoke() {
	app := NewMarkdownViewerApp()
	if app.Title() != "Markdown Viewer" || app.Header == nil || app.Docs == nil ||
		app.Viewer == nil || app.CloseBtn == nil || len(app.Docs.Options) != len(preferredDocuments) ||
		!strings.Contains(app.Status.Text, "blocks") {
		panic("markdown viewer smoke failed")
	}
}

func logicTest() {
	app := NewMarkdownViewerApp()
	if app.view.ContentHeight() <= 0 || !strings.HasPrefix(app.Status.Text, "overview.markdown") {
		panic("initial document not loaded")
	}
	app.Docs.SetSelectedIndex(2)
	app.ShowSelected()
	if !strings.HasPrefix(app.Status.Text, "tables.markdown") || app.view.ContentHeight() <= 0 {
		panic("table document not loaded")
	}
	blocks := parseMarkdown("# H\n\ntext\n\n```python\nprint(1)\n```\n\n| A |\n| - |\n| B |")
	if len(blocks) != 4 {
		panic("markdown parser failed")
	}
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		if len(os.Args) > 1 {
			switch os.Args[1] {
			case "--smoke":
				smoke()
				return 0
			case "--logic-test":
				logicTest()
				return 0
			}
		}
		app := NewMarkdownViewerApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}

var _ = unicode.IsLetter
