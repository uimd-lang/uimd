package uimd

import (
	"fmt"
	"os"
	"path/filepath"
	"slices"
	"strings"
	"testing"
	"time"
)

type standardToolInterceptProvider struct {
	called bool
}

type activatingGeneratedControl struct {
	*GeneratedWindowBase
	activationCount int
}

func (control *activatingGeneratedControl) ActivateGeneratedControl() bool {
	control.activationCount++
	return true
}

type scopedConfirmGeneratedScrollView struct {
	*GeneratedWindowBase
	leading *Button
}

func (window *scopedConfirmGeneratedScrollView) HandleGeneratedTextConfirmed(string, string) bool {
	window.leading.SetEnabled(false)
	return true
}

type customElementWithoutCommitMode struct {
	name  string
	frame Rect
}

var _ Element = (*customElementWithoutCommitMode)(nil)

func (element *customElementWithoutCommitMode) ElementName() string {
	return element.name
}

func (element *customElementWithoutCommitMode) ElementType() string {
	return "custom"
}

func (element *customElementWithoutCommitMode) IsFocusable() bool {
	return false
}

func (element *customElementWithoutCommitMode) IsEnabled() bool {
	return true
}

func (element *customElementWithoutCommitMode) SetFrame(frame Rect) {
	element.frame = frame
}

func (element *customElementWithoutCommitMode) ElementFrame() Rect {
	return element.frame
}

func (element *customElementWithoutCommitMode) Render(Size, ElementRenderState) [][]TerminalCell {
	return nil
}

func (element *customElementWithoutCommitMode) HandleKey(string) bool {
	return false
}

func (element *customElementWithoutCommitMode) SetText(string) {
}

func TestCustomElementDoesNotRequireCommitMode(t *testing.T) {
	element := &customElementWithoutCommitMode{name: "custom"}
	if runtimeElementUsesLeaveCommit(element) {
		t.Fatal("custom element without commit mode unexpectedly uses leave commit")
	}
	panel := NewScrollView("panel", 0)
	if panel.AddChild(element) != element {
		t.Fatal("ScrollView did not accept a custom element without commit mode")
	}
}

func TestTerminalInputBytesNeedMoreFramesEscapeSequences(t *testing.T) {
	tests := []struct {
		name string
		data []byte
		want bool
	}{
		{name: "plain text", data: []byte("a"), want: false},
		{name: "standalone escape awaiting timeout", data: []byte("\x1b"), want: true},
		{name: "CSI prefix", data: []byte("\x1b["), want: true},
		{name: "partial modified arrow", data: []byte("\x1b[1;2"), want: true},
		{name: "arrow", data: []byte("\x1b[A"), want: false},
		{name: "modified arrow", data: []byte("\x1b[1;2A"), want: false},
		{name: "SS3 prefix", data: []byte("\x1bO"), want: true},
		{name: "SS3 key", data: []byte("\x1bOA"), want: false},
		{name: "partial mouse", data: []byte("\x1b[<0;4;5"), want: true},
		{name: "mouse", data: []byte("\x1b[<0;4;5M"), want: false},
		{name: "partial paste", data: []byte("\x1b[200~hello"), want: true},
		{name: "paste", data: []byte("\x1b[200~hello\x1b[201~"), want: false},
		{name: "partial UTF-8", data: []byte{0xc3}, want: true},
		{name: "complete UTF-8", data: []byte("é"), want: false},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			if got := terminalInputBytesNeedMore(test.data); got != test.want {
				t.Fatalf("terminalInputBytesNeedMore(%q) = %v, want %v", test.data, got, test.want)
			}
		})
	}
}

func TestDirectTerminalInputReaderFramesSplitAndStandaloneEscape(t *testing.T) {
	start := time.Unix(0, 0)
	reader := directTerminalInputReader{}
	if inputs := reader.Feed([]byte("\x1b"), start); len(inputs) != 0 {
		t.Fatalf("initial Escape produced inputs before its continuation window: %v", inputs)
	}
	if inputs := reader.Feed(
		[]byte("[A"),
		start.Add(terminalEscapeSequenceTimeout/2),
	); len(inputs) != 1 || inputs[0].Key != "Up" {
		t.Fatalf("split CSI Up inputs = %v, want one Up key", inputs)
	}

	reader = directTerminalInputReader{}
	if inputs := reader.Feed([]byte("\x1b"), start); len(inputs) != 0 {
		t.Fatalf("initial standalone Escape produced inputs too early: %v", inputs)
	}
	if inputs := reader.Feed(
		nil,
		start.Add(terminalEscapeSequenceTimeout),
	); len(inputs) != 1 || inputs[0].Key != "Escape" {
		t.Fatalf("standalone Escape inputs = %v, want one Escape key", inputs)
	}
}

func TestCoalesceMouseWheelInputsMatchesCppBoundedRuns(t *testing.T) {
	wheel := func(x int, y int, delta int) terminalInput {
		return terminalInput{Mouse: &directMouseEvent{Name: "scroll", X: x, Y: y, Delta: delta}}
	}
	press := terminalInput{Mouse: &directMouseEvent{Name: "mouse_press", X: 7, Y: 8}}
	inputs := []terminalInput{
		wheel(1, 2, 8),
		wheel(3, 4, 9),
		{Key: "Enter"},
		wheel(5, 6, 1),
		wheel(6, 7, -1),
		press,
		wheel(8, 9, -8),
		wheel(10, 11, -9),
	}

	got := coalesceMouseWheelInputs(inputs)
	if len(got) != 4 {
		t.Fatalf("coalesced input count = %d, want 4: %#v", len(got), got)
	}
	if got[0].Mouse == nil || *got[0].Mouse != (directMouseEvent{Name: "scroll", X: 3, Y: 4, Delta: 12}) {
		t.Fatalf("positive wheel run = %#v, want latest position and delta 12", got[0].Mouse)
	}
	if got[1].Key != "Enter" {
		t.Fatalf("non-wheel boundary = %#v, want Enter", got[1])
	}
	if got[2].Mouse == nil || *got[2].Mouse != *press.Mouse {
		t.Fatalf("mouse boundary = %#v, want %#v", got[2].Mouse, press.Mouse)
	}
	if got[3].Mouse == nil || *got[3].Mouse != (directMouseEvent{Name: "scroll", X: 10, Y: 11, Delta: -12}) {
		t.Fatalf("negative wheel run = %#v, want latest position and delta -12", got[3].Mouse)
	}
}

func TestParseSgrMouseAcceptsOnlyCanonicalWheelButtons(t *testing.T) {
	tests := []struct {
		sequence  string
		wantName  string
		wantDelta int
	}{
		{sequence: "\x1b[<64;4;5M", wantName: "scroll", wantDelta: 1},
		{sequence: "\x1b[<65;4;5M", wantName: "scroll", wantDelta: -1},
		{sequence: "\x1b[<66;4;5M", wantName: "mouse_press", wantDelta: 0},
		{sequence: "\x1b[<67;4;5M", wantName: "mouse_press", wantDelta: 0},
	}
	for _, test := range tests {
		event, ok := parseSgrMouse(test.sequence)
		if !ok {
			t.Fatalf("parseSgrMouse(%q) failed", test.sequence)
		}
		if event.Name != test.wantName || event.Delta != test.wantDelta {
			t.Errorf("parseSgrMouse(%q) = %#v, want name %q delta %d", test.sequence, event, test.wantName, test.wantDelta)
		}
	}
}

func TestParseKeySupportsCsiAndSs3Arrows(t *testing.T) {
	tests := []struct {
		sequence string
		want     string
	}{
		{sequence: "\x1b[A", want: "Up"},
		{sequence: "\x1b[B", want: "Down"},
		{sequence: "\x1b[C", want: "Right"},
		{sequence: "\x1b[D", want: "Left"},
		{sequence: "\x1bOA", want: "Up"},
		{sequence: "\x1bOB", want: "Down"},
		{sequence: "\x1bOC", want: "Right"},
		{sequence: "\x1bOD", want: "Left"},
	}
	for _, test := range tests {
		if got := parseKey(test.sequence); got != test.want {
			t.Errorf("parseKey(%q) = %q, want %q", test.sequence, got, test.want)
		}
	}
}

func (provider *standardToolInterceptProvider) MCPToolNames() []string {
	return []string{"custom_tool"}
}

func (provider *standardToolInterceptProvider) HandleMCPTool(name string, _ map[string]any) (any, bool) {
	if name != "click_element" {
		return nil, false
	}
	provider.called = true
	return map[string]any{"intercepted": true}, true
}

func TestTerminalSupportsSixelMatchesReferenceTerminalDetection(t *testing.T) {
	tests := []struct {
		name        string
		environment map[string]string
		want        bool
	}{
		{
			name: "force overrides disable",
			environment: map[string]string{
				"UIMD_FORCE_SIXEL":   "1",
				"UIMD_DISABLE_SIXEL": "1",
			},
			want: true,
		},
		{
			name: "Apple Terminal overrides TERM capability text",
			environment: map[string]string{
				"TERM_PROGRAM": "Apple_Terminal",
				"TERM":         "xterm-sixel",
			},
			want: false,
		},
		{
			name: "iTerm session",
			environment: map[string]string{
				"ITERM_SESSION_ID": "w0t0p0:session",
			},
			want: true,
		},
		{
			name: "known terminal program",
			environment: map[string]string{
				"TERM_PROGRAM": "WezTerm",
			},
			want: true,
		},
		{
			name: "explicit TERM capability",
			environment: map[string]string{
				"TERM": "xterm-sixel",
			},
			want: true,
		},
		{
			name: "plain xterm",
			environment: map[string]string{
				"TERM": "xterm-256color",
			},
			want: false,
		},
	}
	environmentNames := []string{
		"UIMD_FORCE_SIXEL",
		"UIMD_DISABLE_SIXEL",
		"TERM_PROGRAM",
		"TERM",
		"COLORTERM",
		"ITERM_SESSION_ID",
		"LC_TERMINAL",
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			for _, name := range environmentNames {
				t.Setenv(name, "")
			}
			for name, value := range test.environment {
				t.Setenv(name, value)
			}
			if got := terminalSupportsSixel(); got != test.want {
				t.Fatalf("terminalSupportsSixel() = %v, want %v", got, test.want)
			}
		})
	}
}

func TestForcedSixelImageRendersRawPayloadIntoAnsiFrame(t *testing.T) {
	t.Setenv("UIMD_FORCE_SIXEL", "1")
	t.Setenv("UIMD_DISABLE_SIXEL", "")
	t.Setenv("UIMD_DETERMINISTIC_IMAGE_FALLBACK", "")

	imagePath := writeTestTga(t)

	image := NewImage("photo", imagePath, "", "stretch", "sixel", "left", "top")
	rows := image.Render(Size{Width: 2, Height: 2}, ElementRenderState{})
	anchor := rows[0][0]
	if !strings.HasPrefix(anchor.Raw, "\x1bPq") || !strings.HasSuffix(anchor.Raw, "\x1b\\") {
		t.Fatalf("raw payload = %q, want a complete Sixel sequence", anchor.Raw)
	}
	if anchor.RawWidth != 2 || anchor.RawHeight != 1 || anchor.RawSkip {
		t.Fatalf("raw anchor metadata = width %d height %d skip %v, want 2/1/false", anchor.RawWidth, anchor.RawHeight, anchor.RawSkip)
	}
	for row := range rows {
		for col := range rows[row] {
			if col == 0 {
				if rows[row][col].Raw == "" || rows[row][col].RawHeight != 1 {
					t.Fatalf("row %d is missing its one-row raw segment", row)
				}
				continue
			}
			if !rows[row][col].RawSkip {
				t.Fatalf("covered cell (%d,%d) is not marked raw-skip", row, col)
			}
		}
	}

	buffer := NewTerminalBuffer(2, 2)
	buffer.Blit(rows, 0, 0, Rect{Row: 0, Col: 0, Width: 2, Height: 2})
	frame := buffer.AnsiFrame()
	if !strings.Contains(frame, anchor.Raw) {
		t.Fatalf("ANSI frame does not emit the Sixel payload: %q", frame)
	}
}

func TestTerminalBufferRenderDiffSkipsUnchangedTextAndRawPayload(t *testing.T) {
	buffer := NewTerminalBuffer(3, 2)
	buffer.SetCell(0, 0, TerminalCell{
		Raw:       "\x1bPq#0;2;0;0;0~\x1b\\",
		RawWidth:  2,
		RawHeight: 1,
	})
	buffer.SetCell(0, 1, TerminalCell{RawSkip: true})
	buffer.SetCell(1, 0, TerminalCell{Text: "A"})

	first := buffer.RenderDiff()
	if !strings.Contains(first, "\x1bPq") {
		t.Fatalf("first diff omitted raw payload: %q", first)
	}
	if second := buffer.RenderDiff(); second != "" {
		t.Fatalf("unchanged second diff = %q, want no terminal output", second)
	}

	buffer.SetCell(1, 0, TerminalCell{Text: "B"})
	third := buffer.RenderDiff()
	if !strings.Contains(third, "B") {
		t.Fatalf("changed diff omitted text update: %q", third)
	}
	if strings.Contains(third, "\x1bPq") {
		t.Fatalf("text-only diff retransmitted unchanged raw payload: %q", third)
	}
}

func TestScrollViewTopClippedSixelKeepsCroppedRawAnchor(t *testing.T) {
	t.Setenv("UIMD_FORCE_SIXEL", "1")
	t.Setenv("UIMD_DISABLE_SIXEL", "")
	t.Setenv("UIMD_DETERMINISTIC_IMAGE_FALLBACK", "")

	image := NewImage("photo", writeTestTga(t), "", "stretch", "sixel", "left", "top")
	image.SetFrame(Rect{Width: 2, Height: 3})
	scrollView := NewScrollView("images", 0)
	scrollView.SetFrame(Rect{Width: 2, Height: 2})
	scrollView.AddChild(image)
	scrollView.ViewOffset = 1

	rows := scrollView.Render(Size{Width: 2, Height: 2}, ElementRenderState{})
	anchor := rows[0][0]
	if anchor.Raw == "" {
		t.Fatal("top-clipped Sixel lost its raw anchor")
	}
	if anchor.RawWidth != 2 || anchor.RawHeight != 1 || anchor.RawSkip {
		t.Fatalf("cropped raw anchor metadata = width %d height %d skip %v, want 2/1/false", anchor.RawWidth, anchor.RawHeight, anchor.RawSkip)
	}
	if rows[1][0].Raw == "" || rows[1][0].RawHeight != 1 {
		t.Fatal("second clipped row is missing its one-row Sixel segment")
	}
}

func writeTestTga(t *testing.T) string {
	t.Helper()
	imagePath := t.TempDir() + "/pixel.tga"
	tga := []byte{
		0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 0, 32, 0x28,
		0, 0, 255, 255,
	}
	if err := os.WriteFile(imagePath, tga, 0o600); err != nil {
		t.Fatalf("write test TGA: %v", err)
	}
	return imagePath
}

func TestLoadImageRasterCachesDecodedSourceLikeReferenceRuntimes(t *testing.T) {
	imagePath := writeTestTga(t)
	first, ok := loadImageRaster(imagePath)
	if !ok || first.width != 1 || first.height != 1 {
		t.Fatalf("initial raster = %#v, %v; want decoded 1x1 source", first, ok)
	}
	if err := os.Remove(imagePath); err != nil {
		t.Fatalf("remove decoded image source: %v", err)
	}
	second, ok := loadImageRaster(imagePath)
	if !ok || second.width != first.width || second.height != first.height {
		t.Fatalf("cached raster = %#v, %v; want retained decoded source", second, ok)
	}
}

func TestMouseReleaseShowsCopyNotification(t *testing.T) {
	window := NewGeneratedWindowBase("Test")
	window.SetGeneratedLayout([]LayoutItem{
		{
			CellName:        "name",
			Content:         "name",
			CellRow:         0,
			CellCol:         0,
			CellCharsWidth:  20,
			CellCharsHeight: 1,
			CellWidth:       20,
			CellHeight:      1,
			Width:           20,
			Height:          1,
			CharsWidth:      20,
			CharsHeight:     1,
		},
	})
	input := NewTextInput("name", "Ada Lovelace", 0)
	window.AddElement(input)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	config := &mcpRuntimeConfig{ViewportWidth: 40, ViewportHeight: 5}
	renderForMcp(window, state, config)
	frame := input.ElementFrame()

	_, err := callMcpTool(window, state, "mouse_press", map[string]any{"x": frame.Col, "y": frame.Row}, config)
	if err != nil {
		t.Fatalf("mouse_press failed: %v", err)
	}
	_, err = callMcpTool(window, state, "mouse_drag", map[string]any{"x": frame.Col + 3, "y": frame.Row}, config)
	if err != nil {
		t.Fatalf("mouse_drag failed: %v", err)
	}
	if selected := input.SelectionText(); selected != "Ada" {
		t.Fatalf("selection after drag = %q, want Ada; frame=%+v press=%T", selected, input.ElementFrame(), state.mousePressElement)
	}
	_, err = callMcpTool(window, state, "mouse_release", map[string]any{"x": frame.Col + 3, "y": frame.Row}, config)
	if err != nil {
		t.Fatalf("mouse_release failed: %v", err)
	}

	if state.rootState().notification != copyNotificationText {
		t.Fatalf("notification = %q, want %q", state.rootState().notification, copyNotificationText)
	}
	buffer := RenderGeneratedRuntimeContent(state, Size{Width: 40, Height: 5})
	if !strings.Contains(buffer.PlainText(), copyNotificationText) {
		t.Fatalf("rendered frame does not contain copy notification:\n%s", buffer.PlainText())
	}
}

func TestDirectSgrMouseReleaseKeepsCopyNotificationThroughReplacement(t *testing.T) {
	window := NewGeneratedWindowBase("Test")
	window.SetGeneratedLayout([]LayoutItem{
		{
			CellName:        "name",
			Content:         "name",
			CellRow:         0,
			CellCol:         0,
			CellCharsWidth:  20,
			CellCharsHeight: 1,
			CellWidth:       20,
			CellHeight:      1,
			Width:           20,
			Height:          1,
			CharsWidth:      20,
			CharsHeight:     1,
		},
	})
	input := NewTextInput("name", "Ada Lovelace", 0)
	window.AddElement(input)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	size := Size{Width: 40, Height: 5}
	renderForMcp(window, state, &mcpRuntimeConfig{ViewportWidth: size.Width, ViewportHeight: size.Height})
	frame := input.ElementFrame()
	data := fmt.Sprintf(
		"\x1b[<0;%d;%dM\x1b[<32;%d;%dM\x1b[<0;%d;%dmEve",
		frame.Col+1,
		frame.Row+1,
		frame.Col+4,
		frame.Row+1,
		frame.Col+4,
		frame.Row+1,
	)

	for _, event := range parseTerminalInputs(data) {
		active := activeRuntimeState(state)
		if event.Mouse != nil {
			active.handleDirectMouse(*event.Mouse, size)
		} else {
			active.handleKey(event.Key)
		}
	}

	if input.Value != "Eve Lovelace" {
		t.Fatalf("input value = %q, want Eve Lovelace", input.Value)
	}
	if state.rootState().notification != copyNotificationText {
		t.Fatalf("notification = %q, want %q", state.rootState().notification, copyNotificationText)
	}
	buffer := RenderGeneratedRuntimeContent(state, size)
	if !strings.Contains(buffer.PlainText(), copyNotificationText) {
		t.Fatalf("rendered frame does not contain copy notification:\n%s", buffer.PlainText())
	}
}

func TestDirectMouseReleaseCopiesFocusedSelectionWhenPressedElementWasCleared(t *testing.T) {
	window := NewGeneratedWindowBase("Test")
	window.SetGeneratedLayout([]LayoutItem{
		{
			CellName:        "name",
			Content:         "name",
			CellRow:         0,
			CellCol:         0,
			CellCharsWidth:  20,
			CellCharsHeight: 1,
			CellWidth:       20,
			CellHeight:      1,
			Width:           20,
			Height:          1,
			CharsWidth:      20,
			CharsHeight:     1,
		},
	})
	input := NewTextInput("name", "Ada Lovelace", 0)
	window.AddElement(input)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	size := Size{Width: 40, Height: 5}
	renderForMcp(window, state, &mcpRuntimeConfig{ViewportWidth: size.Width, ViewportHeight: size.Height})
	focusElement(state, input)
	input.SetSelection(0, 3)
	frame := input.ElementFrame()

	if !state.handleDirectMouse(directMouseEvent{Name: "mouse_release", X: frame.Col + 3, Y: frame.Row}, size) {
		t.Fatal("mouse_release was not handled")
	}
	if state.rootState().notification != copyNotificationText {
		t.Fatalf("notification = %q, want %q", state.rootState().notification, copyNotificationText)
	}
}

func newFocusedRuntimeForTest(elements ...Element) (*GeneratedWindowBase, *runtimeState) {
	window := NewGeneratedWindowBase("Test")
	for index, element := range elements {
		element.SetFrame(Rect{Row: index, Col: 0, Width: 20, Height: 1})
		window.AddElement(element)
	}
	options := GeneratedWindowRuntimeOptions{}
	if len(elements) > 0 {
		options.InitialFocusName = elements[0].ElementName()
	}
	return window, newRuntimeState(window, options)
}

func TestKeyboardFocusMovementNotifiesOnlyActualPreviousAndNextElements(t *testing.T) {
	first := NewButton("first", "First")
	first.SetFrame(Rect{Row: 0, Col: 0, Width: 8, Height: 1})
	second := NewButton("second", "Second")
	second.SetFrame(Rect{Row: 2, Col: 0, Width: 8, Height: 1})
	events := []string{}
	window := NewGeneratedWindowBase("Test")
	window.AddElement(first)
	window.AddElement(second)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{
		InitialFocusName: "first",
		OnFocusChanged: func(name string, focused bool) {
			events = append(events, fmt.Sprintf("%s:%t", name, focused))
		},
	})

	if !state.handleKey("Tab") {
		t.Fatal("Tab was not handled")
	}
	if got, want := events, []string{"first:false", "second:true"}; !slices.Equal(got, want) {
		t.Fatalf("focus events after Tab = %v, want %v", got, want)
	}
	events = events[:0]

	state.handleKey("Right")
	if len(events) != 0 {
		t.Fatalf("no-op spatial movement emitted focus events: %v", events)
	}
}

func TestEditableEntryMovesCaretToEndAndClearsSelection(t *testing.T) {
	textInput := NewTextInput("text_input", "alpha", 0)
	textArea := NewTextArea("text_area", "alpha\nbeta")
	tests := []struct {
		name    string
		element Element
		input   *TextInput
	}{
		{name: "text input", element: textInput, input: textInput},
		{name: "text area", element: textArea, input: &textArea.TextInput},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			test.input.SetSelection(0, 2)
			_, state := newFocusedRuntimeForTest(test.element)

			if !state.handleKey("Enter") {
				t.Fatal("Enter was not handled")
			}
			if !state.editMode {
				t.Fatal("editable element did not enter edit mode")
			}
			if test.input.Cursor != len([]rune(test.input.Value)) {
				t.Fatalf("cursor = %d, want end %d", test.input.Cursor, len([]rune(test.input.Value)))
			}
			if test.input.SelectionStart != nil || test.input.SelectionEnd != nil {
				t.Fatalf("selection persisted on edit entry: start=%v end=%v", test.input.SelectionStart, test.input.SelectionEnd)
			}
		})
	}
}

func TestOpenComboBoxConsumesMousePressAndReleaseAboveBackgroundControl(t *testing.T) {
	comboBox := NewComboBox("choice", []string{"First", "Second", "Third"})
	comboBox.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 1})
	backgroundButton := NewButton("background", "Background")
	backgroundButton.SetFrame(Rect{Row: 2, Col: 0, Width: 12, Height: 1})
	buttonPresses := 0
	window := NewGeneratedWindowBase("Test")
	window.AddElement(comboBox)
	window.AddElement(backgroundButton)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{
		InitialFocusName: "choice",
		OnButton: func(name string) {
			if name == "background" {
				buttonPresses++
			}
		},
	})
	config := &mcpRuntimeConfig{ViewportWidth: 40, ViewportHeight: 8}

	if !state.handleKey("Enter") {
		t.Fatal("Enter did not open the ComboBox")
	}
	if !state.editMode {
		t.Fatal("ComboBox did not enter edit mode")
	}
	if _, err := callMcpTool(window, state, "mouse_press", map[string]any{"x": 1, "y": 2}, config); err != nil {
		t.Fatalf("mouse_press failed: %v", err)
	}
	if comboBox.SelectedIndex != 1 {
		t.Fatalf("selected ComboBox index after press = %d, want second option", comboBox.SelectedIndex)
	}
	if _, err := callMcpTool(window, state, "mouse_release", map[string]any{"x": 1, "y": 2}, config); err != nil {
		t.Fatalf("mouse_release failed: %v", err)
	}

	if comboBox.SelectedIndex != 1 {
		t.Fatalf("selected ComboBox index = %d, want second option", comboBox.SelectedIndex)
	}
	if buttonPresses != 0 {
		t.Fatalf("background button received %d fall-through presses, want 0", buttonPresses)
	}
	if state.editMode {
		t.Fatal("ComboBox remained open after selecting an option")
	}
}

func TestOpenComboBoxMouseSelectsEveryRenderedOptionRow(t *testing.T) {
	options := []string{"Option 0", "Option 1", "Option 2", "Option 3", "Option 4", "Option 5", "Option 6", "Option 7"}
	comboBox := NewComboBox("choice", options)
	comboBox.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 1})
	window := NewGeneratedWindowBase("Test")
	window.AddElement(comboBox)
	selectionChanges := 0
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{
		InitialFocusName: "choice",
		OnSelectionChanged: func(string, []string) {
			selectionChanges++
		},
	})
	config := &mcpRuntimeConfig{ViewportWidth: 20, ViewportHeight: 12}

	if !state.handleKey("Enter") {
		t.Fatal("Enter did not open the ComboBox")
	}
	if _, err := callMcpTool(window, state, "mouse_press", map[string]any{"x": 1, "y": 7}, config); err != nil {
		t.Fatalf("mouse_press failed: %v", err)
	}
	if comboBox.SelectedIndex != 6 {
		t.Fatalf("selected ComboBox index = %d, want 6", comboBox.SelectedIndex)
	}
	if selectionChanges != 1 {
		t.Fatalf("selection changes = %d, want 1", selectionChanges)
	}
	if state.editMode {
		t.Fatal("ComboBox remained open after selecting a lower option")
	}
}

func TestClosedComboBoxOpensOnMousePressAndStaysOpenOnRelease(t *testing.T) {
	comboBox := NewComboBox("choice", []string{"First", "Second", "Third"})
	comboBox.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 1})
	window := NewGeneratedWindowBase("Test")
	window.AddElement(comboBox)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{InitialFocusName: "choice"})
	config := &mcpRuntimeConfig{ViewportWidth: 40, ViewportHeight: 8}

	if _, err := callMcpTool(window, state, "mouse_press", map[string]any{"x": 1, "y": 0}, config); err != nil {
		t.Fatalf("mouse_press failed: %v", err)
	}
	if !state.editMode {
		t.Fatal("closed ComboBox did not open on mouse press")
	}
	if activeComboBox(state) != comboBox {
		t.Fatalf("active ComboBox = %v, want clicked ComboBox", activeComboBox(state))
	}
	if state.editSnapshot == nil || state.editSnapshot.element != comboBox {
		t.Fatalf("edit snapshot = %v, want clicked ComboBox snapshot", state.editSnapshot)
	}

	if _, err := callMcpTool(window, state, "mouse_release", map[string]any{"x": 1, "y": 0}, config); err != nil {
		t.Fatalf("mouse_release failed: %v", err)
	}
	if !state.editMode || activeComboBox(state) != comboBox {
		t.Fatal("releasing the closed-value row closed the newly opened ComboBox")
	}
}

func TestClosedComboBoxMousePressOpensInsideScrollViewScope(t *testing.T) {
	panel := NewScrollView("panel", 0)
	panel.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 2})
	comboBox := NewComboBox("choice", []string{"First", "Second", "Third"})
	comboBox.SetFrame(Rect{Width: 12, Height: 1})
	panel.AddChild(comboBox)
	window := NewGeneratedWindowBase("Test")
	window.AddElement(panel)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	syncScrollViewChildFrames(panel, panel.ElementFrame())

	performMcpMousePress(state, window, Point{Row: 0, Col: 1})

	if state.scopeDimElement != panel {
		t.Fatalf("active scope = %v, want containing ScrollView", state.scopeDimElement)
	}
	if state.scopeEditElement != comboBox {
		t.Fatalf("scope edit element = %v, want clicked ComboBox", state.scopeEditElement)
	}
	if !state.editMode || activeComboBox(state) != comboBox {
		t.Fatal("nested ComboBox did not open on mouse press")
	}
	if state.editSnapshot == nil || state.editSnapshot.element != comboBox {
		t.Fatalf("edit snapshot = %v, want clicked ComboBox snapshot", state.editSnapshot)
	}
}

func TestDirectMouseWheelScrollsContainingScrollView(t *testing.T) {
	scrollView := NewScrollView("items", 0)
	scrollView.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 2})
	for index := 0; index < 4; index++ {
		button := NewButton(fmt.Sprintf("item_%d", index), fmt.Sprintf("Item %d", index))
		button.SetFrame(Rect{Width: 12, Height: 1})
		scrollView.AddChild(button)
	}
	window := NewGeneratedWindowBase("Test")
	window.AddElement(scrollView)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	size := Size{Width: 20, Height: 4}
	_ = scrollView.Render(Size{Width: 12, Height: 2}, ElementRenderState{})
	syncScrollViewChildFrames(scrollView, scrollView.ElementFrame())

	if !state.handleDirectMouse(directMouseEvent{Name: "scroll", X: 1, Y: 1, Delta: -1}, size) {
		t.Fatal("mouse wheel over a ScrollView descendant was not handled")
	}
	if scrollView.ViewOffset != 1 {
		t.Fatalf("ScrollView offset = %d, want 1", scrollView.ViewOffset)
	}
}

func TestDirectMouseWheelScrollsTextAreaBeforeContainingScrollView(t *testing.T) {
	textArea := NewTextArea("description", "one\ntwo\nthree\nfour")
	textArea.SetFrame(Rect{Width: 12, Height: 2})
	scrollView := NewScrollView("panel", 0)
	scrollView.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 2})
	scrollView.AddChild(textArea)
	filler := NewButton("filler", "Filler")
	filler.SetFrame(Rect{Width: 12, Height: 2})
	scrollView.AddChild(filler)
	window := NewGeneratedWindowBase("Test")
	window.AddElement(scrollView)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	_ = scrollView.Render(Size{Width: 12, Height: 2}, ElementRenderState{})
	syncScrollViewChildFrames(scrollView, scrollView.ElementFrame())

	if !state.handleDirectMouse(directMouseEvent{Name: "scroll", X: 1, Y: 1, Delta: -1}, Size{Width: 20, Height: 4}) {
		t.Fatal("mouse wheel over an overflowing TextArea was not handled")
	}
	if textArea.rowScrollOffset != 1 {
		t.Fatalf("TextArea row offset = %d, want 1", textArea.rowScrollOffset)
	}
	if scrollView.ViewOffset != 0 {
		t.Fatalf("containing ScrollView offset = %d, want unchanged 0", scrollView.ViewOffset)
	}
}

func TestDirectMouseWheelScrollsListBox(t *testing.T) {
	listBox := NewListBox("files", []string{"a", "b", "c", "d"}, false)
	listBox.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 2})
	window := NewGeneratedWindowBase("Test")
	window.AddElement(listBox)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	_ = listBox.Render(Size{Width: 12, Height: 2}, ElementRenderState{})

	if !state.handleDirectMouse(directMouseEvent{Name: "scroll", X: 1, Y: 1, Delta: -1}, Size{Width: 20, Height: 4}) {
		t.Fatal("mouse wheel over ListBox was not handled")
	}
	if listBox.ScrollOffset != 1 {
		t.Fatalf("ListBox offset = %d, want 1", listBox.ScrollOffset)
	}
}

func TestMcpMousePressActivatesContainingScrollViewScope(t *testing.T) {
	panel := NewScrollView("panel", 0)
	panel.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 2})
	paid := NewCheckBox("paid", "Paid", false)
	paid.SetFrame(Rect{Width: 12, Height: 1})
	panel.AddChild(paid)
	window := NewGeneratedWindowBase("Test")
	window.AddElement(panel)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	syncScrollViewChildFrames(panel, panel.ElementFrame())

	performMcpMousePress(state, window, Point{Row: 0, Col: 0})

	if !state.editMode {
		t.Fatal("mouse press on a ScrollView descendant did not activate the ScrollView scope")
	}
	if state.scopeDimElement != panel {
		t.Fatalf("active scope = %v, want containing ScrollView", state.scopeDimElement)
	}
	if state.focusedElement() != paid {
		t.Fatalf("focused element = %v, want clicked checkbox", state.focusedElement())
	}
}

func TestScrollViewNavigationFocusDoesNotOpenComboBoxOverlay(t *testing.T) {
	window := NewGeneratedWindowBase("Test")
	window.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 12, CellCharsHeight: 3, CellName: "panel", CellWidth: 12, CellHeight: 3, Width: 12, Height: 3, CharsWidth: 12, CharsHeight: 3, Content: "panel"},
	})
	panel := NewScrollView("panel", 0)
	comboBox := NewComboBox("category", []string{"Food", "Home"})
	comboBox.SetFrame(Rect{Width: 12, Height: 1})
	comboBox.SetStyle(Style{Background: NewColor("#111111")})
	comboBox.SetFocusStyle(Style{Background: NewColor("#ff0000")})
	comboBox.SetEditStyle(Style{Background: NewColor("#0000ff")})
	panel.AddChild(comboBox)
	window.AddElement(panel)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	state.focusedOverride = comboBox
	state.scopeDimElement = panel
	state.editMode = true

	buffer := RenderGeneratedRuntimeContent(state, Size{Width: 12, Height: 3})

	if got := buffer.Cell(0, 0).Background.String(); got != "#ff0000" {
		t.Fatalf("navigation-focused ComboBox background = %q, want focus background #ff0000", got)
	}
	if got := buffer.PlainText(); strings.Contains(got, "Home") {
		t.Fatalf("navigation-focused ComboBox rendered an open dropdown: %q", strings.TrimSpace(got))
	}
}

func TestReusablePartialFocusBackgroundMatchesReferenceChildComposition(t *testing.T) {
	parentBackground := NewColor("#172033")
	focusBackground := NewColor("#ffffff14")
	oneFocusLayer := focusBackground.BlendOver(parentBackground)
	tests := []struct {
		name               string
		childCellStyle     Style
		expectedBackground Color
	}{
		{
			name:               "transparent child cell composes through parent and child",
			childCellStyle:     Style{Background: NewColor("transparent")},
			expectedBackground: focusBackground.BlendOver(oneFocusLayer),
		},
		{
			name:               "opaque child cell receives one focus layer",
			childCellStyle:     Style{Background: parentBackground},
			expectedBackground: oneFocusLayer,
		},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			child := NewGeneratedWindowBase("Child")
			child.SetGeneratedFocusable(true)
			child.SetGeneratedWindowStyle(Style{Background: parentBackground})
			child.SetGeneratedLayout([]LayoutItem{
				{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 1, CellCharsHeight: 1, CellName: "child", CellWidth: 1, CellHeight: 1, Width: 1, Height: 1, CharsWidth: 1, CharsHeight: 1, CellStyle: test.childCellStyle},
			})
			reusable := NewReusableElement("card", "child")
			reusable.SetChild(child)
			reusable.SetFocusStyle(Style{Background: focusBackground})
			window := NewGeneratedWindowBase("Root")
			window.SetGeneratedWindowStyle(Style{Background: parentBackground})
			window.SetGeneratedLayout([]LayoutItem{
				{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 1, CellCharsHeight: 1, CellName: "card", CellWidth: 1, CellHeight: 1, Width: 1, Height: 1, CharsWidth: 1, CharsHeight: 1, Content: "card"},
			})
			window.AddElement(reusable)
			state := newRuntimeState(window, GeneratedWindowRuntimeOptions{InitialFocusName: "card"})

			buffer := RenderGeneratedRuntimeContent(state, Size{Width: 1, Height: 1})
			if got := buffer.Cell(0, 0).Background.String(); got != test.expectedBackground.String() {
				t.Fatalf("focused reusable background = %q, want %q", got, test.expectedBackground.String())
			}
		})
	}
}

func TestReusableGeneratedScrollViewFocusUnderlaysAlphaDescendantBackground(t *testing.T) {
	parentBackground := NewColor("#303545")
	focusBackground := NewColor("#ffffff14")
	descendantBackground := NewColor("#252a36cc")
	child := NewGeneratedScrollViewBase("Alpha focus scroll")
	child.SetGeneratedFocusable(true)
	child.SetGeneratedWindowStyle(Style{Background: parentBackground})
	child.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 1, CellCharsHeight: 3, CellName: "items", CellWidth: 1, CellHeight: 3, Width: 1, Height: 3, CharsWidth: 1, CharsHeight: 3, Content: "items"},
	})
	items := NewScrollView("items", 0)
	items.SetStyle(Style{Background: parentBackground})
	items.SetFocusStyle(Style{Background: focusBackground})
	alphaRow := NewLabel("alpha_row", " ")
	alphaRow.SetStyle(Style{Background: descendantBackground})
	opaqueRow := NewLabel("opaque_row", " ")
	opaqueRow.SetStyle(Style{Background: parentBackground})
	items.AddChild(alphaRow)
	items.AddChild(opaqueRow)
	child.AddElement(items)

	reusable := NewReusableElement("card", "alpha_focus_scroll")
	reusable.SetChild(child)
	reusable.SetStyle(Style{Background: parentBackground})
	reusable.SetFocusStyle(Style{Background: focusBackground})
	window := NewGeneratedWindowBase("Root")
	window.SetGeneratedWindowStyle(Style{Background: parentBackground})
	window.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 1, CellCharsHeight: 3, CellName: "card", CellWidth: 1, CellHeight: 3, Width: 1, Height: 3, CharsWidth: 1, CharsHeight: 3, Content: "card", CellStyle: Style{Background: parentBackground}},
	})
	window.AddElement(reusable)

	unfocusedState := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	unfocused := RenderGeneratedRuntimeContent(unfocusedState, Size{Width: 1, Height: 3})
	focusedState := newRuntimeState(window, GeneratedWindowRuntimeOptions{InitialFocusName: "card"})
	focused := RenderGeneratedRuntimeContent(focusedState, Size{Width: 1, Height: 3})
	expectedUnfocused := descendantBackground.BlendOver(parentBackground)
	expectedFocused := descendantBackground.BlendOver(focusBackground.BlendOver(parentBackground))
	if got := unfocused.Cell(0, 0).Background.String(); got != expectedUnfocused.String() {
		t.Fatalf("unfocused alpha descendant background = %q, want %q", got, expectedUnfocused.String())
	}
	if got := focused.Cell(0, 0).Background.String(); got != expectedFocused.String() {
		t.Fatalf("focused alpha descendant background = %q, want %q", got, expectedFocused.String())
	}
	expectedDirectFocus := focusBackground.BlendOver(parentBackground)
	if got := focused.Cell(1, 0).Background.String(); got != expectedDirectFocus.String() {
		t.Fatalf("focused opaque structural descendant background = %q, want %q", got, expectedDirectFocus.String())
	}
	if got := focused.Cell(2, 0).Background.String(); got != expectedDirectFocus.String() {
		t.Fatalf("focused structural gap background = %q, want %q", got, expectedDirectFocus.String())
	}

	partialChildFocus := NewColor("#00ff0014")
	items.SetFocusStyle(Style{Background: partialChildFocus})
	focused = RenderGeneratedRuntimeContent(focusedState, Size{Width: 1, Height: 3})
	expectedStackedFocus := partialChildFocus.BlendOver(focusBackground.BlendOver(parentBackground))
	if got := focused.Cell(1, 0).Background.String(); got != expectedStackedFocus.String() {
		t.Fatalf("partial child scroll focus background = %q, want %q", got, expectedStackedFocus.String())
	}

	// A generated panel may repaint the same structural background before the
	// root ScrollView focus pass. That structural-equivalent surface must still
	// participate in the rendered and interactive focus geometry.
	child.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 1, CellCharsHeight: 3, CellName: "items", CellWidth: 1, CellHeight: 3, Width: 1, Height: 3, CharsWidth: 1, CharsHeight: 3, Content: "items", CellStyle: Style{Background: parentBackground}},
	})
	opaqueChildFocus := NewColor("#1e3a5f")
	items.SetFocusStyle(Style{Background: opaqueChildFocus})
	focused = RenderGeneratedRuntimeContent(focusedState, Size{Width: 1, Height: 3})
	if got := focused.Cell(1, 0).Background.String(); got != opaqueChildFocus.String() {
		t.Fatalf("opaque child scroll focus background = %q, want %q", got, opaqueChildFocus.String())
	}
	if got := focused.Cell(2, 0).Background.String(); got != opaqueChildFocus.String() {
		t.Fatalf("opaque child scroll focus gap = %q, want %q", got, opaqueChildFocus.String())
	}

	// Transparent generated child content must inherit the focused ScrollView
	// surface. Replacing a ScrollView row outright would discard this underlay
	// and reproduce the MarkdownViewer edit-state mismatch.
	transparentChild := NewGeneratedWindowBase("Transparent child")
	transparentChild.SetGeneratedWindowStyle(Style{Background: NewColor("#00000000")})
	transparentChild.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 1, CellCharsHeight: 1, CellName: "text", CellWidth: 1, CellHeight: 1, Width: 1, Height: 1, CharsWidth: 1, CharsHeight: 1, Content: "text", CellStyle: Style{Background: NewColor("#00000000")}},
	})
	transparentText := NewLabel("text", " ")
	transparentText.SetStyle(Style{Background: NewColor("#00000000")})
	transparentChild.AddElement(transparentText)
	transparentRow := NewReusableElement("transparent_row", "transparent_child")
	transparentRow.SetChild(transparentChild)
	transparentRow.SetStyle(Style{Background: NewColor("#00000000")})
	items.ClearChildren()
	items.AddChild(transparentRow)
	items.SetFocusStyle(Style{Background: focusBackground})
	focused = RenderGeneratedRuntimeContent(focusedState, Size{Width: 1, Height: 3})
	if got := focused.Cell(0, 0).Background.String(); got != expectedDirectFocus.String() {
		t.Fatalf("transparent generated child focused surface = %q, want %q", got, expectedDirectFocus.String())
	}
}

func TestSameRenderedColorIncludesAlpha(t *testing.T) {
	if sameRenderedColor(NewColor("#ffffff14"), NewColor("#ffffff18")) {
		t.Fatal("rendered colors with different alpha channels compared equal")
	}
}

func TestReusableScrollScopeVerticalNavigationReachesAdjacentComboBox(t *testing.T) {
	rowWindow := NewGeneratedWindowBase("Row")
	rowWindow.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 20, CellCharsHeight: 2, CellName: "row", CellWidth: 20, CellHeight: 2, Width: 8, Height: 1, CharsWidth: 8, CharsHeight: 1, Content: "paid"},
		{Row: 1, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 20, CellCharsHeight: 2, CellName: "row", CellWidth: 20, CellHeight: 2, Width: 8, Height: 1, CharsWidth: 8, CharsHeight: 1, Content: "category"},
	})
	paid := NewCheckBox("paid", "Paid", false)
	category := NewComboBox("category", []string{"Food", "Home"})
	categoryFocusStyle := NewStyle()
	categoryFocusStyle.Background = NewColor("#ff0000")
	category.SetFocusStyle(categoryFocusStyle)
	categoryEditStyle := NewStyle()
	categoryEditStyle.Background = NewColor("#0000ff")
	category.SetEditStyle(categoryEditStyle)
	rowWindow.AddElement(paid)
	rowWindow.AddElement(category)
	row := NewReusableElement("row", "expense_row")
	row.SetChild(rowWindow)
	row.SetFrame(Rect{Width: 20, Height: 2})

	listWindow := NewGeneratedScrollViewBase("Expenses")
	listWindow.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 20, CellCharsHeight: 4, CellName: "panel", CellWidth: 20, CellHeight: 4, Width: 20, Height: 4, CharsWidth: 20, CharsHeight: 4, CellStyle: Style{Background: NewColor("#172033")}},
	})
	panel := listWindow.NewGeneratedScrollViewElement()
	panelFocusStyle := NewStyle()
	panelFocusStyle.Background = NewColor("#ffffff14")
	panel.SetFocusStyle(panelFocusStyle)
	panelEditStyle := NewStyle()
	panelEditStyle.Background = NewColor("#ffffff18")
	panel.SetEditStyle(panelEditStyle)
	panel.SetFrame(Rect{Row: 0, Col: 0, Width: 20, Height: 4})
	panel.AddChild(row)
	listWindow.AddElement(panel)
	host := NewReusableElement("expenses", "expenses_list")
	host.SetChild(listWindow)
	host.SetStyle(Style{Background: NewColor("#172033")})
	hostFocusStyle := NewStyle()
	hostFocusStyle.Background = NewColor("#ffffff14")
	host.SetFocusStyle(hostFocusStyle)
	hostEditStyle := NewStyle()
	hostEditStyle.Background = NewColor("#ffffff18")
	host.SetEditStyle(hostEditStyle)
	host.SetFrame(Rect{Row: 0, Col: 0, Width: 20, Height: 4})
	viewWindow := NewGeneratedWindowBase("Expenses View")
	viewWindow.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 20, CellCharsHeight: 4, CellName: "view", CellWidth: 20, CellHeight: 4, Width: 20, Height: 4, CharsWidth: 20, CharsHeight: 4, Content: "expenses"},
	})
	viewWindow.AddElement(host)
	main := NewViewHost("main")
	main.SetView(viewWindow)
	main.SetFrame(Rect{Row: 0, Col: 0, Width: 20, Height: 4})
	window := NewGeneratedWindowBase("Test")
	window.SetGeneratedLayout([]LayoutItem{
		{Row: 0, Col: 0, CellRow: 0, CellCol: 0, CellCharsWidth: 20, CellCharsHeight: 4, CellName: "root", CellWidth: 20, CellHeight: 4, Width: 20, Height: 4, CharsWidth: 20, CharsHeight: 4, Content: "main"},
	})
	window.AddElement(main)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{InitialFocusName: "expenses"})
	syncScrollViewChildFrames(panel, panel.ElementFrame())

	state.handleKey("Enter")
	if state.focusedElement() != paid {
		t.Fatalf("initial ScrollView descendant = %v, want checkbox", state.focusedElement())
	}
	state.handleKey("Down")
	if state.focusedElement() != category {
		t.Fatalf("focused element after Down = %v, want adjacent ComboBox", state.focusedElement())
	}
	buffer := RenderGeneratedRuntimeContent(state, Size{Width: 20, Height: 4})
	categoryFrame := category.ElementFrame()
	if got := buffer.Cell(categoryFrame.Row, categoryFrame.Col).Background.String(); got != "#ff0000" {
		t.Fatalf("focused descendant background = %q, want ComboBox focus background #ff0000", got)
	}
	categoryPoint := Point{Row: categoryFrame.Row, Col: categoryFrame.Col}
	target := mouseTargetAtPoint(state, window, categoryPoint)
	performMcpMousePress(state, window, categoryPoint)
	if state.scopeEditElement != category {
		t.Fatalf("mouse-opened scope edit element = %v, want ComboBox; target=%v point=%+v frame=%+v", state.scopeEditElement, target, categoryPoint, categoryFrame)
	}
	buffer = RenderGeneratedRuntimeContent(state, Size{Width: 20, Height: 4})
	categoryFrame = category.ElementFrame()
	if got := buffer.Cell(categoryFrame.Row, categoryFrame.Col).Background.String(); got != "#0000ff" {
		t.Fatalf("mouse-opened descendant background = %q, want ComboBox edit background #0000ff", got)
	}
	state.handleKey("Escape")

	focusElementForMcp(state, window, paid, false)
	if state.editMode {
		t.Fatal("direct descendant focus unexpectedly entered edit mode")
	}
	if state.scopeDimElement != host {
		t.Fatalf("direct descendant focus scope = %v, want containing reusable ScrollView host", state.scopeDimElement)
	}
	state.handleKey("Down")
	if state.focusedElement() != category {
		t.Fatalf("directly focused descendant after Down = %v, want adjacent ComboBox", state.focusedElement())
	}
	state.handleKey("Escape")
	if state.scopeDimElement != nil {
		t.Fatalf("direct descendant scope after Escape = %v, want nil", state.scopeDimElement)
	}
	if state.editMode {
		t.Fatal("direct descendant Escape unexpectedly left edit mode active")
	}
	if state.focusedElement() != host {
		t.Fatalf("focus after direct descendant Escape = %v, want reusable ScrollView host", state.focusedElement())
	}

	focusElementForMcp(state, window, paid, false)
	state.handleKey("Down")
	state.handleKey("Enter")
	if !state.editMode {
		t.Fatal("directly focused ComboBox Enter did not enter scoped edit mode")
	}
	if state.scopeEditElement != category {
		t.Fatalf("scoped edit element = %v, want directly focused ComboBox", state.scopeEditElement)
	}
	state.handleKey("Escape")
	if state.scopeEditElement != nil {
		t.Fatalf("scoped edit element after first Escape = %v, want nil", state.scopeEditElement)
	}
	if state.scopeDimElement != host || !state.editMode {
		t.Fatal("first Escape should leave child edit while preserving the active ScrollView scope")
	}
	state.handleKey("Escape")
	if state.scopeDimElement != nil || state.editMode || state.focusedElement() != host {
		t.Fatal("second Escape should leave the ScrollView scope and focus its host")
	}
}

func TestScrollViewScopedReusableControlActivatesWithEnterAndSpace(t *testing.T) {
	child := &activatingGeneratedControl{GeneratedWindowBase: NewGeneratedWindowBase("Child")}
	child.SetGeneratedFocusable(true)
	reusable := NewReusableElement("action", "child")
	reusable.SetChild(child)
	reusable.SetFrame(Rect{Width: 12, Height: 1})
	panel := NewScrollView("panel", 0)
	panel.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 1})
	panel.AddChild(reusable)
	scrollWindow := NewGeneratedScrollViewBase("Items")
	scrollWindow.AddElement(panel)
	host := NewReusableElement("items", "items")
	host.SetChild(scrollWindow)
	host.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 1})
	window := NewGeneratedWindowBase("Test")
	window.AddElement(host)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{InitialFocusName: "items"})
	syncScrollViewChildFrames(panel, panel.ElementFrame())

	state.handleKey("Enter")
	if state.focusedElement() != reusable {
		t.Fatalf("focused ScrollView descendant = %v, want reusable control", state.focusedElement())
	}
	state.handleKey("Enter")
	if child.activationCount != 1 {
		t.Fatalf("activation count after Enter = %d, want 1", child.activationCount)
	}
	state.handleKey(" ")
	if child.activationCount != 2 {
		t.Fatalf("activation count after Space = %d, want 2", child.activationCount)
	}
}

func TestTextAreaMousePlacementUsesWrappedVisualRows(t *testing.T) {
	area := NewTextArea("description", "abcdef")
	frame := Rect{Row: 0, Col: 0, Width: 3, Height: 2}
	area.SetFrame(frame)

	if cursor := textInputCursorAtPoint(&area.TextInput, Point{Row: 1, Col: 1}, frame); cursor != 4 {
		t.Fatalf("cursor on second wrapped row = %d, want 4", cursor)
	}
}

func TestTextInputAlignmentUsesOneRenderAndMouseOffset(t *testing.T) {
	input := NewTextInput("field", "abc", 10)
	input.SetStyle(Style{TextAlign: "right"})
	input.SetCursorStyle(Style{Background: NewColor("#facc15")})
	input.SetFrame(Rect{Width: 6, Height: 1})

	rendered := input.Render(Size{Width: 6, Height: 1}, ElementRenderState{})
	text := ""
	for _, cell := range rendered[0] {
		text += cell.Text
	}
	if text != "   abc" {
		t.Fatalf("right-aligned text = %q, want %q", text, "   abc")
	}

	input.SetCursor(1)
	rendered = input.Render(Size{Width: 6, Height: 1}, ElementRenderState{EditMode: true})
	if background := rendered[0][4].Background.String(); background != "#facc15" {
		t.Fatalf("cursor background at aligned column = %q, want #facc15", background)
	}

	input.SetSelection(0, 2)
	rendered = input.Render(Size{Width: 6, Height: 1}, ElementRenderState{EditMode: true})
	if rendered[0][3].Background.String() != "#facc15" || rendered[0][4].Background.String() != "#facc15" {
		t.Fatal("selection did not use the text alignment offset")
	}
	if rendered[0][5].Background.String() == "#facc15" {
		t.Fatal("selection extended past its source range")
	}
	if cursor := textInputCursorAtPoint(input, Point{Row: 0, Col: 0}, input.ElementFrame()); cursor != 0 {
		t.Fatalf("cursor in leading padding = %d, want 0", cursor)
	}
	if cursor := textInputCursorAtPoint(input, Point{Row: 0, Col: 4}, input.ElementFrame()); cursor != 1 {
		t.Fatalf("cursor over aligned text = %d, want 1", cursor)
	}

	input.SetValue("abcdefgh")
	input.SetCursor(8)
	rendered = input.Render(Size{Width: 6, Height: 1}, ElementRenderState{EditMode: true})
	text = ""
	for _, cell := range rendered[0] {
		text += cell.Text
	}
	if text != "defgh " {
		t.Fatalf("overflowing right-aligned text = %q, want %q", text, "defgh ")
	}
}

func TestTextAreaMouseReleasePreservesClickedCaret(t *testing.T) {
	area := NewTextArea("description", "abcdef")
	window, state := newFocusedRuntimeForTest(area)
	area.SetFrame(Rect{Row: 0, Col: 0, Width: 3, Height: 2})
	state.handleKey("Enter")
	point := Point{Row: 1, Col: 1}

	performMcpMousePress(state, window, point)
	performMcpMouseRelease(state, window, &mcpRuntimeConfig{}, point)

	if area.Cursor != 4 {
		t.Fatalf("cursor after mouse release = %d, want 4", area.Cursor)
	}
	if !state.editMode {
		t.Fatal("mouse release unexpectedly left TextArea edit mode")
	}
}

func TestTextInputEscapeRestoresEditSnapshot(t *testing.T) {
	input := NewTextInput("field", "alpha", 0)
	_, state := newFocusedRuntimeForTest(input)
	state.handleKey("Enter")
	state.handleKey("X")
	if input.Value != "alphaX" {
		t.Fatalf("edited value = %q, want alphaX", input.Value)
	}

	state.handleKey("Escape")

	if state.editMode {
		t.Fatal("Escape did not leave edit mode")
	}
	if input.Value != "alpha" {
		t.Fatalf("value after Escape = %q, want original alpha", input.Value)
	}
}

func TestTextAreaMovesCaretVertically(t *testing.T) {
	area := NewTextArea("field", "abc\ndef")
	area.SetFrame(Rect{Row: 0, Col: 0, Width: 10, Height: 3})
	area.SetCursor(6)

	if !area.HandleKey("Up") {
		t.Fatal("Up was not handled by multiline TextArea")
	}
	if area.Cursor != 2 {
		t.Fatalf("cursor after Up = %d, want 2", area.Cursor)
	}
	if !area.HandleKey("Down") {
		t.Fatal("Down was not handled by multiline TextArea")
	}
	if area.Cursor != 6 {
		t.Fatalf("cursor after Down = %d, want 6", area.Cursor)
	}
}

func TestZeroNumberInputHighlightsAndReplacesZero(t *testing.T) {
	number := NewNumberInput("amount", 0)
	cursorStyle := NewStyle()
	cursorStyle.Background = NewColor("#ff0000")
	number.SetCursorStyle(cursorStyle)
	number.BeginEdit()

	rows := number.Render(Size{Width: 3, Height: 1}, ElementRenderState{Focused: true, EditMode: true})
	if got := rows[0][0].Background.String(); got != "#ff0000" {
		t.Fatalf("zero cell background = %q, want cursor selection background #ff0000", got)
	}
	if !number.HandleKey("7") {
		t.Fatal("numeric key was not handled")
	}
	if number.editText != "7" {
		t.Fatalf("edit text after replacing zero = %q, want 7", number.editText)
	}
}

func TestNumberInputProgrammaticZeroSelectsOnlyOnFreshEdit(t *testing.T) {
	number := NewNumberInput("amount", 1)
	editStyle := NewStyle()
	editStyle.Background = NewColor("#0000ff")
	number.SetEditStyle(editStyle)
	cursorStyle := NewStyle()
	cursorStyle.Background = NewColor("#ff0000")
	number.SetCursorStyle(cursorStyle)
	number.BeginEdit()
	number.SetValue(0)
	rows := number.Render(Size{Width: 4, Height: 1}, ElementRenderState{EditMode: true})
	if got := rows[0][0].Background.String(); got == "#ff0000" {
		t.Fatalf("programmatic zero was selected before a fresh edit entry")
	}
	number.CommitEdit()
	number.BeginEdit()
	rows = number.Render(Size{Width: 4, Height: 1}, ElementRenderState{EditMode: true})
	if got := rows[0][0].Background.String(); got != "#ff0000" {
		t.Fatalf("fresh zero edit background = %q, want #ff0000", got)
	}
}

func TestMcpTypeTextPreservesExplicitCursor(t *testing.T) {
	window := NewGeneratedWindowBase("Test")
	input := NewTextInput("name", "Ada Lovelace", 0)
	window.AddElement(input)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	config := &mcpRuntimeConfig{ViewportWidth: 40, ViewportHeight: 5}

	if _, err := callMcpTool(window, state, "set_text", map[string]any{
		"element_id": "name",
		"text":       "Grace Hopper",
	}, config); err != nil {
		t.Fatalf("set_text failed: %v", err)
	}
	if _, err := callMcpTool(window, state, "set_cursor", map[string]any{
		"element_id": "name",
		"offset":     0,
	}, config); err != nil {
		t.Fatalf("set_cursor failed: %v", err)
	}
	if _, err := callMcpTool(window, state, "type_text", map[string]any{
		"element_id": "name",
		"text":       "Dr. ",
	}, config); err != nil {
		t.Fatalf("type_text failed: %v", err)
	}
	if input.Value != "Dr. Grace Hopper" {
		t.Fatalf("value after explicit cursor typing = %q, want %q", input.Value, "Dr. Grace Hopper")
	}
}

func TestMcpClickElementUsesMousePressCursorPlacement(t *testing.T) {
	window := NewGeneratedWindowBase("Test")
	input := NewTextInput("name", "abcdefghij", 0)
	input.SetFrame(Rect{Row: 1, Col: 2, Width: 10, Height: 1})
	window.AddElement(input)
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	config := &mcpRuntimeConfig{ViewportWidth: 20, ViewportHeight: 5}

	if _, err := callMcpTool(window, state, "click_element", map[string]any{
		"element_id": "name",
	}, config); err != nil {
		t.Fatalf("click_element failed: %v", err)
	}
	if input.Cursor != 5 {
		t.Fatalf("cursor after center click = %d, want mouse-derived cursor 5", input.Cursor)
	}
	if !state.editMode {
		t.Fatal("click_element did not enter TextInput edit mode")
	}
}

func TestStandardMcpClickCannotBeOverriddenByApplicationProvider(t *testing.T) {
	window := NewGeneratedWindowBase("Test")
	input := NewTextInput("name", "abcdefghij", 0)
	input.SetFrame(Rect{Row: 1, Col: 2, Width: 10, Height: 1})
	window.AddElement(input)
	provider := &standardToolInterceptProvider{}
	window.mcpToolProvider = provider
	state := newRuntimeState(window, GeneratedWindowRuntimeOptions{})
	config := &mcpRuntimeConfig{ViewportWidth: 20, ViewportHeight: 5}

	if _, err := callMcpTool(window, state, "click_element", map[string]any{
		"element_id": "name",
	}, config); err != nil {
		t.Fatalf("click_element failed: %v", err)
	}
	if provider.called {
		t.Fatal("application MCP provider intercepted the standard click_element tool")
	}
	if input.Cursor != 5 {
		t.Fatalf("cursor after standard click = %d, want runtime mouse-derived cursor 5", input.Cursor)
	}
}

func TestNumberInputCommitsWhenMouseMovesFocusAway(t *testing.T) {
	number := NewNumberInput("amount", 1)
	button := NewButton("done", "Done")
	window, state := newFocusedRuntimeForTest(number, button)
	state.handleKey("Enter")
	state.handleKey("2")

	performMcpMousePress(state, window, Point{Row: 1, Col: 0})

	if number.NumberValue != 12 {
		t.Fatalf("number after blur = %v, want committed value 12", number.NumberValue)
	}
	if number.editing {
		t.Fatal("number input remained in its stale edit session after blur")
	}
	if state.focusedElement() != button {
		t.Fatalf("focused element after click = %v, want button", state.focusedElement())
	}
}

func TestRootEscapeDoesNotRequestApplicationClose(t *testing.T) {
	window, state := newFocusedRuntimeForTest(NewButton("action", "Action"))

	if !state.handleKey("Escape") {
		t.Fatal("root Escape was not consumed")
	}
	if window.ShouldClose() {
		t.Fatal("root Escape requested application close")
	}
}

func TestDialogEscapeRendersNegativeButtonBeforeClosing(t *testing.T) {
	stack := &GeneratedWindowStack{}
	rootWindow := NewGeneratedWindowBase("Root")
	rootState := newRuntimeState(rootWindow, GeneratedWindowRuntimeOptions{WindowStack: stack})
	dialog := NewMessageBoxYesNo("Confirm", "Continue?")
	dialogOptions := dialog.RuntimeOptions()
	stack.Push(dialog, GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNo",
		InitialFocusName: "yes_btn",
		OnButton:         dialogOptions.OnButton,
		OnKey:            dialogOptions.OnKey,
		ShouldClose:      dialogOptions.ShouldClose,
	})
	dialogState := activeRuntimeState(rootState)
	if dialogState == nil || dialogState.window != dialog.GeneratedWindowBase {
		t.Fatal("message box did not become the active runtime window")
	}

	callbackCount := 0
	rootState.beforeDialogButtonAction = func() {
		callbackCount++
		if dialogState.focusedElement() != dialog.NoBtn {
			t.Fatalf("focused element before close = %v, want No button", dialogState.focusedElement())
		}
		if dialog.ShouldClose() {
			t.Fatal("dialog closed before the negative-button frame was exposed")
		}
	}

	if !dialogState.handleKey("Escape") {
		t.Fatal("dialog Escape was not handled")
	}
	if callbackCount != 1 {
		t.Fatalf("visible-focus callback count = %d, want 1", callbackCount)
	}
	if dialog.Result() != "no" || !dialog.ShouldClose() {
		t.Fatalf("dialog result after Escape = %q close=%v, want no/closed", dialog.Result(), dialog.ShouldClose())
	}
}

func TestFileBrowserParentNavigationAndDirectMouseWheel(t *testing.T) {
	root := t.TempDir()
	images := filepath.Join(root, "image_samples")
	if err := os.Mkdir(images, 0o755); err != nil {
		t.Fatalf("create image directory: %v", err)
	}
	for index := 0; index < 12; index++ {
		path := filepath.Join(images, fmt.Sprintf("sample_%02d.txt", index))
		if err := os.WriteFile(path, []byte("sample"), 0o644); err != nil {
			t.Fatalf("create browser fixture %q: %v", path, err)
		}
	}

	browser := NewFileBrowser(root, images, "open", nil)
	if browser.CurrentDir() != images {
		t.Fatalf("initial directory = %q, want %q", browser.CurrentDir(), images)
	}
	browser.SelectEntry(0)
	if !browser.AcceptCurrent() {
		t.Fatal("parent entry was not accepted")
	}
	if browser.CurrentDir() != root {
		t.Fatalf("directory after parent entry = %q, want %q", browser.CurrentDir(), root)
	}

	childIndex := -1
	for index, option := range browser.Entries.Options {
		if option == "image_samples/" {
			childIndex = index
			break
		}
	}
	if childIndex < 0 {
		t.Fatalf("image_samples directory missing from options: %v", browser.Entries.Options)
	}
	browser.SelectEntry(childIndex)
	if !browser.AcceptCurrent() {
		t.Fatal("child directory was not accepted")
	}
	if browser.CurrentDir() != images {
		t.Fatalf("directory after child entry = %q, want %q", browser.CurrentDir(), images)
	}

	state := newRuntimeState(browser.GeneratedWindowBase, browser.RuntimeOptions())
	size := Size{Width: 48, Height: 14}
	_ = RenderGeneratedRuntimeContent(state, size)
	frame := browser.Entries.ElementFrame()
	if !state.handleDirectMouse(directMouseEvent{
		Name:  "scroll",
		X:     frame.Col,
		Y:     frame.Row,
		Delta: -1,
	}, size) {
		t.Fatal("mouse wheel over FileBrowser entries was not handled")
	}
	if browser.Entries.ScrollOffset == 0 {
		t.Fatal("FileBrowser entries did not scroll after a direct mouse-wheel event")
	}
}

func TestFileBrowserDirectoryEnterKeepsEntriesEditMode(t *testing.T) {
	root := t.TempDir()
	child := filepath.Join(root, "child")
	if err := os.Mkdir(child, 0o755); err != nil {
		t.Fatalf("create child directory: %v", err)
	}
	stack := &GeneratedWindowStack{}
	rootWindow := NewGeneratedWindowBase("Root")
	rootState := newRuntimeState(rootWindow, GeneratedWindowRuntimeOptions{WindowStack: stack})
	browser := NewFileBrowser(root, root, "open", nil)
	stack.Push(browser, browser.StackFrameOptions())
	browserState := activeRuntimeState(rootState)
	childIndex := -1
	for index, option := range browser.Entries.Options {
		if option == "child/" {
			childIndex = index
			break
		}
	}
	if childIndex < 0 {
		t.Fatalf("child directory missing from options: %v", browser.Entries.Options)
	}
	browser.Entries.ActiveIndex = childIndex
	browser.Entries.ShowActiveItem()
	if !browser.Entries.ActiveItemVisible {
		t.Fatal("FileBrowser active row was not visible before directory Enter")
	}

	if !browserState.handleKey("Enter") {
		t.Fatal("Enter on a FileBrowser directory was not handled")
	}
	if browser.CurrentDir() != child {
		t.Fatalf("directory after Enter = %q, want %q", browser.CurrentDir(), child)
	}
	if !browserState.editMode || browserState.focusedElement() != browser.Entries {
		t.Fatal("directory Enter left the FileBrowser entries edit mode")
	}
	if browser.Closed() || browser.ShouldClose() {
		t.Fatal("directory Enter closed the FileBrowser")
	}
	if browser.Entries.ActiveItemVisible {
		t.Fatal("directory Enter left the FileBrowser keyboard-active row visible")
	}
}

func TestFileBrowserMouseClickEntersDirectoryAndOnlySelectsFile(t *testing.T) {
	root := t.TempDir()
	child := filepath.Join(root, "child")
	if err := os.Mkdir(child, 0o755); err != nil {
		t.Fatalf("create child directory: %v", err)
	}
	file := filepath.Join(root, "photo.png")
	if err := os.WriteFile(file, []byte("png"), 0o644); err != nil {
		t.Fatalf("create file: %v", err)
	}
	browser := NewFileBrowser(root, root, "open", nil)
	state := newRuntimeState(browser.GeneratedWindowBase, browser.RuntimeOptions())
	size := Size{Width: 48, Height: 14}
	_ = RenderGeneratedRuntimeContent(state, size)

	childIndex := slices.Index(browser.Entries.Options, "child/")
	if childIndex < 0 {
		t.Fatalf("child directory missing from options: %v", browser.Entries.Options)
	}
	frame := browser.Entries.ElementFrame()
	if !state.handleDirectMouse(directMouseEvent{
		Name: "mouse_press",
		X:    frame.Col,
		Y:    frame.Row + childIndex - browser.Entries.ScrollOffset,
	}, size) {
		t.Fatal("directory mouse press was not handled")
	}
	if browser.CurrentDir() != child {
		t.Fatalf("directory after click = %q, want %q", browser.CurrentDir(), child)
	}

	_ = RenderGeneratedRuntimeContent(state, size)
	frame = browser.Entries.ElementFrame()
	if !state.handleDirectMouse(directMouseEvent{Name: "mouse_press", X: frame.Col, Y: frame.Row}, size) {
		t.Fatal("parent mouse press was not handled")
	}
	if browser.CurrentDir() != root {
		t.Fatalf("directory after parent click = %q, want %q", browser.CurrentDir(), root)
	}

	_ = RenderGeneratedRuntimeContent(state, size)
	fileIndex := slices.Index(browser.Entries.Options, "photo.png")
	frame = browser.Entries.ElementFrame()
	if !state.handleDirectMouse(directMouseEvent{
		Name: "mouse_press",
		X:    frame.Col,
		Y:    frame.Row + fileIndex - browser.Entries.ScrollOffset,
	}, size) {
		t.Fatal("file mouse press was not handled")
	}
	if browser.Entries.SelectedIndex != fileIndex || browser.Filename.Value != "photo.png" {
		t.Fatalf("file click selected index/value = %d/%q, want %d/photo.png", browser.Entries.SelectedIndex, browser.Filename.Value, fileIndex)
	}
	if browser.Closed() {
		t.Fatal("file click closed FileBrowser")
	}
}

func TestScrollViewScopedConfirmRetainsLiveInputAndRebasesFocusAfterMutation(t *testing.T) {
	newFixture := func(keep bool) (*runtimeState, *Button, *TextInput, *Button, *int) {
		panel := NewScrollView("panel", 0)
		panel.SetFrame(Rect{Row: 0, Col: 0, Width: 12, Height: 3})
		leading := NewButton("leading", "Leading")
		leading.SetFrame(Rect{Width: 12, Height: 1})
		input := NewTextInput("filter", "", 0)
		input.SetFrame(Rect{Width: 12, Height: 1})
		trailing := NewButton("trailing", "Trailing")
		trailing.SetFrame(Rect{Width: 12, Height: 1})
		panel.AddChild(leading)
		panel.AddChild(input)
		panel.AddChild(trailing)
		scrollWindow := NewGeneratedScrollViewBase("Items")
		scrollWindow.AddElement(panel)
		scrollOwner := &scopedConfirmGeneratedScrollView{
			GeneratedWindowBase: scrollWindow,
			leading:             leading,
		}
		host := NewReusableElement("items", "items")
		host.SetChild(scrollOwner)
		host.SetFrame(Rect{Width: 12, Height: 3})
		window := NewGeneratedWindowBase("Scoped confirm")
		window.AddElement(host)
		trailingActivations := 0
		options := GeneratedWindowRuntimeOptions{
			InitialFocusName:         "items",
			KeepEditModeAfterConfirm: keep,
			OnButton: func(name string) {
				if name == "trailing" {
					trailingActivations++
				}
			},
		}
		state := newRuntimeState(window, options)
		syncScrollViewChildFrames(panel, panel.ElementFrame())
		return state, leading, input, trailing, &trailingActivations
	}

	t.Run("keep edit mode captures a fresh snapshot", func(t *testing.T) {
		state, leading, input, _, _ := newFixture(true)
		focusElementForMcp(state, state.window, input, false)
		for _, key := range []string{"Enter", "a", "Enter"} {
			state.handleKey(key)
		}
		if leading.IsEnabled() {
			t.Fatal("confirm callback did not mutate the focusable set")
		}
		if state.focusedElement() != input || state.scopeEditElement != input {
			t.Fatal("keep-after-confirm did not retain the same live scoped input")
		}
		if state.editSnapshot == nil || state.editSnapshot.element != input {
			t.Fatal("keep-after-confirm did not capture a fresh scoped edit snapshot")
		}
		state.handleKey("b")
		state.handleKey("Escape")
		if input.Value != "a" {
			t.Fatalf("Escape restored %q, want freshly committed value a", input.Value)
		}
	})

	t.Run("navigation focus follows the submitted element instead of the old index", func(t *testing.T) {
		state, _, input, _, trailingActivations := newFixture(false)
		focusElementForMcp(state, state.window, input, false)
		for _, key := range []string{"Enter", "a", "Enter"} {
			state.handleKey(key)
		}
		if state.focusedElement() != input || state.scopeEditElement != nil {
			t.Fatal("confirm callback rebased focus away from the submitted input")
		}
		state.handleKey("Enter")
		if state.scopeEditElement != input {
			t.Fatal("Enter after confirm did not reopen the submitted input")
		}
		if *trailingActivations != 0 {
			t.Fatalf("trailing control activated %d times through a stale focus index", *trailingActivations)
		}
	})
}

func TestModalCloseRestoresRebuiltScrollScopeFocusAndOffset(t *testing.T) {
	buildRows := func(count int) ([]Element, []*Button) {
		children := make([]Element, 0, count)
		browseButtons := make([]*Button, 0, count)
		for index := 0; index < count; index++ {
			rowWindow := NewGeneratedWindowBase("Image Row")
			rowWindow.AddElement(NewButton("show", "Show"))
			browse := NewButton("browse", "Browse")
			rowWindow.AddElement(browse)
			row := NewReusableElement(fmt.Sprintf("row_%d", index), "image_row")
			row.SetChild(rowWindow)
			row.SetFrame(Rect{Width: 20, Height: 1})
			children = append(children, row)
			browseButtons = append(browseButtons, browse)
		}
		return children, browseButtons
	}

	listWindow := NewGeneratedScrollViewBase("Image Items")
	panel := listWindow.NewGeneratedScrollViewElement()
	panel.SetFrame(Rect{Width: 20, Height: 3})
	children, browseButtons := buildRows(8)
	for _, child := range children {
		panel.AddChild(child)
	}
	listWindow.AddElement(panel)
	host := NewReusableElement("items", "image_items")
	host.SetChild(listWindow)
	host.SetFrame(Rect{Width: 20, Height: 3})
	rootWindow := NewGeneratedWindowBase("Image Browser")
	rootWindow.AddElement(host)
	stack := &GeneratedWindowStack{}
	rootState := newRuntimeState(rootWindow, GeneratedWindowRuntimeOptions{WindowStack: stack})
	rootState.scopeDimElement = host
	rootState.focusedOverride = browseButtons[5]
	rootState.editMode = true
	rootState.rememberReusableScrollScopeFocus()
	panel.ViewOffset = 3

	dialog := NewGeneratedWindowBase("File Browser")
	stack.Push(dialog, GeneratedWindowFrameOptions{ClassName: "FileBrowser"})
	panel.ClearChildren()
	children, browseButtons = buildRows(8)
	for _, child := range children {
		panel.AddChild(child)
	}
	stack.Remove(dialog)

	if rootState.scopeDimElement != host || !rootState.editMode {
		t.Fatal("modal close did not restore the active reusable ScrollView scope")
	}
	if rootState.focusedElement() != browseButtons[5] {
		t.Fatalf("focused element after rebuild = %v, want same logical Browse button", rootState.focusedElement())
	}
	if panel.ViewOffset != 3 {
		t.Fatalf("scroll offset after same-size rebuild = %d, want 3", panel.ViewOffset)
	}

	escapeDialog := NewMessageBoxYesNo("Delete Image", "Delete this image?")
	stack.Push(escapeDialog, GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNo",
		InitialFocusName: "yes_btn",
		OnButton: func(name string) {
			if name == "no_btn" {
				stack.Remove(escapeDialog)
			}
		},
	})
	dialogState := activeRuntimeState(rootState)
	if dialogState == nil || dialogState.window != escapeDialog.GeneratedWindowBase {
		t.Fatal("message box did not become the active runtime window")
	}
	if !dialogState.handleKey("Escape") {
		t.Fatal("standard dialog Escape was not handled")
	}
	if !stack.Empty() {
		t.Fatal("standard dialog Escape did not remove the modal")
	}
	if rootState.scopeDimElement != host || !rootState.editMode {
		t.Fatal("standard dialog Escape cleared the restored reusable ScrollView scope")
	}
	if rootState.focusedElement() != browseButtons[5] {
		t.Fatalf("focused element after standard dialog Escape = %v, want same Browse button", rootState.focusedElement())
	}
	if panel.ViewOffset != 3 {
		t.Fatalf("scroll offset after standard dialog Escape = %d, want 3", panel.ViewOffset)
	}

	stack.Push(dialog, GeneratedWindowFrameOptions{ClassName: "FileBrowser"})
	panel.ClearChildren()
	children, browseButtons = buildRows(2)
	for _, child := range children {
		panel.AddChild(child)
	}
	stack.Remove(dialog)

	if rootState.focusedElement() != browseButtons[1] {
		t.Fatalf("focused element after shrink = %v, want last logical Browse button", rootState.focusedElement())
	}
	if panel.ViewOffset != 0 {
		t.Fatalf("scroll offset after shrink = %d, want clamped offset 0", panel.ViewOffset)
	}
}

func TestTopModalImageKeepsSixelPayload(t *testing.T) {
	t.Setenv("UIMD_FORCE_SIXEL", "1")
	t.Setenv("UIMD_DISABLE_SIXEL", "")
	t.Setenv("UIMD_DETERMINISTIC_IMAGE_FALLBACK", "")

	stack := &GeneratedWindowStack{}
	rootWindow := NewGeneratedWindowBase("Root")
	rootState := newRuntimeState(rootWindow, GeneratedWindowRuntimeOptions{WindowStack: stack})
	modal := NewGeneratedWindowBase("Preview")
	modal.SetGeneratedKind("dialog")
	modal.SetGeneratedWindowStyle(Style{BorderWidthHorizontal: Int(0), BorderWidthVertical: Int(0)})
	modal.SetGeneratedLayout([]LayoutItem{{
		CellName: "photo", Content: "photo", CellRow: 0, CellCol: 0,
		CellCharsWidth: 2, CellCharsHeight: 2, CellWidth: 2, CellHeight: 2,
		Width: 2, Height: 2, CharsWidth: 2, CharsHeight: 2,
	}})
	photo := NewImage("photo", writeTestTga(t), "Preview", "stretch", "sixel", "left", "top")
	modal.AddElement(photo)
	stack.Push(modal, GeneratedWindowFrameOptions{ClassName: "ImageShowDialog"})

	buffer := RenderGeneratedRuntimeContent(rootState, Size{Width: 10, Height: 5})
	if !strings.Contains(buffer.AnsiFrame(), imageSixelIntroducer) {
		t.Fatal("top modal image rendered fallback instead of a Sixel payload")
	}
}

func TestFileBrowserEscapeExitsListEditThenFlashesCloseBeforeClosing(t *testing.T) {
	stack := &GeneratedWindowStack{}
	rootWindow := NewGeneratedWindowBase("Root")
	rootState := newRuntimeState(rootWindow, GeneratedWindowRuntimeOptions{WindowStack: stack})
	browser := NewFileBrowser(t.TempDir(), "", "open", nil)
	stack.Push(browser.GeneratedWindowBase, browser.StackFrameOptions())
	browserState := activeRuntimeState(rootState)
	if browserState == nil || browserState.window != browser.GeneratedWindowBase {
		t.Fatal("FileBrowser did not become the active runtime window")
	}
	if !browserState.editMode {
		t.Fatal("FileBrowser did not start with its entries ListBox in edit mode")
	}

	callbackCount := 0
	rootState.beforeDialogButtonAction = func() {
		callbackCount++
		if browserState.focusedElement() != browser.CloseBtn {
			t.Fatalf("focused element before close = %v, want Close button", browserState.focusedElement())
		}
		if browser.Closed() || browser.ShouldClose() {
			t.Fatal("FileBrowser closed before the Close-button frame was exposed")
		}
	}

	if !browserState.handleKey("Escape") {
		t.Fatal("first FileBrowser Escape was not handled")
	}
	if browserState.editMode {
		t.Fatal("first FileBrowser Escape did not leave entries edit mode")
	}
	if callbackCount != 0 || browser.Closed() || browser.ShouldClose() {
		t.Fatalf("first Escape closed FileBrowser: callback=%d closed=%v requested=%v", callbackCount, browser.Closed(), browser.ShouldClose())
	}
	if !browserState.handleKey("Escape") {
		t.Fatal("second FileBrowser Escape was not handled")
	}
	if callbackCount != 1 {
		t.Fatalf("visible-focus callback count = %d, want 1", callbackCount)
	}
	if !browser.Closed() || !browser.ShouldClose() {
		t.Fatalf("FileBrowser after second Escape: closed=%v requested=%v, want closed", browser.Closed(), browser.ShouldClose())
	}
}

func TestFileBrowserEscapeCommitsCurrentListSelectionWithoutClosing(t *testing.T) {
	root := t.TempDir()
	for _, name := range []string{"alpha.png", "camera.png"} {
		if err := os.WriteFile(filepath.Join(root, name), []byte(name), 0o600); err != nil {
			t.Fatalf("write %s: %v", name, err)
		}
	}

	stack := &GeneratedWindowStack{}
	rootWindow := NewGeneratedWindowBase("Root")
	rootState := newRuntimeState(rootWindow, GeneratedWindowRuntimeOptions{WindowStack: stack})
	browser := NewFileBrowser(root, root, "open", nil, "alpha.png")
	stack.Push(browser.GeneratedWindowBase, browser.StackFrameOptions())
	browserState := activeRuntimeState(rootState)
	if browserState == nil || !browserState.editMode {
		t.Fatal("FileBrowser entries did not start in edit mode")
	}

	cameraIndex := -1
	for index, option := range browser.Entries.Options {
		if option == "camera.png" {
			cameraIndex = index
			break
		}
	}
	if cameraIndex < 0 {
		t.Fatalf("camera.png not found in entries: %v", browser.Entries.Options)
	}
	browser.SelectEntry(cameraIndex)

	if !browserState.handleKey("Escape") {
		t.Fatal("FileBrowser entries Escape was not handled")
	}
	if browserState.editMode {
		t.Fatal("FileBrowser entries Escape did not exit edit mode")
	}
	if browser.Closed() || browser.ShouldClose() {
		t.Fatal("FileBrowser entries Escape closed the dialog")
	}
	if browser.Entries.SelectedIndex != cameraIndex {
		t.Fatalf("selected index after Escape = %d, want committed index %d", browser.Entries.SelectedIndex, cameraIndex)
	}
	if browser.Filename.Value != "camera.png" {
		t.Fatalf("filename after Escape = %q, want camera.png", browser.Filename.Value)
	}
}
