package uimd

import (
	"bufio"
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net"
	"os"
	"os/exec"
	"reflect"
	"sort"
	"strconv"
	"strings"
	"time"
	"unicode/utf8"
)

var runtimeClipboardText string

type GeneratedWindowRuntimeOptions struct {
	Output                    io.Writer
	Footer                    string
	InitialFocusName          string
	StartInEditMode           bool
	KeepEditModeAfterConfirm  bool
	KeepEditModeAfterEscape   bool
	WindowStack               *GeneratedWindowStack
	OnKey                     func(string) bool
	OnKeyBeforeFocusedElement func(string, string, bool) bool
	OnButton                  func(string)
	OnTextChanged             func(string, string)
	OnTextConfirmed           func(string, string)
	OnSelectionChanged        func(string, []string)
	OnFocusChanged            func(string, bool)
	ShouldClose               func() bool
}

type GeneratedWindowFrameOptions struct {
	ClassName                 string
	InitialFocusName          string
	StartInEditMode           bool
	KeepEditModeAfterConfirm  bool
	KeepEditModeAfterEscape   bool
	OnKey                     func(string) bool
	OnKeyBeforeFocusedElement func(string, string, bool) bool
	OnButton                  func(string)
	OnTextChanged             func(string, string)
	OnTextConfirmed           func(string, string)
	OnSelectionChanged        func(string, []string)
	OnFocusChanged            func(string, bool)
	ShouldClose               func() bool
}

type GeneratedWindowStack struct {
	frames []*GeneratedWindowStackFrame
	root   *runtimeState
}

type GeneratedWindowStackFrame struct {
	Window    *GeneratedWindowBase
	Options   GeneratedWindowRuntimeOptions
	ClassName string
	state     *runtimeState
	stack     *GeneratedWindowStack
	returnTo  *modalReturnState
}

type modalReturnState struct {
	state        *runtimeState
	scopeHost    Element
	focused      Element
	focusedIndex int
	scrollView   *ScrollView
	viewOffset   int
	autoScroll   bool
}

type GeneratedWindowBase struct {
	title                          string
	kind                           string
	scrollViewRoot                 bool
	focusable                      bool
	closeRequested                 bool
	layout                         []LayoutItem
	elements                       []Element
	windowStyle                    Style
	scrollViewStyle                *Style
	scrollViewFocusStyle           *Style
	scrollViewEditStyle            *Style
	scrollViewSelectedStyle        *Style
	scrollViewDescendantFocusStyle *Style
	mcpEnabled                     bool
	mcpClassName                   string
	mcpSource                      string
	mcpDescription                 string
	mcpToolProvider                MCPToolProvider
	ownerObject                    any
}

type MCPToolProvider interface {
	MCPToolNames() []string
	HandleMCPTool(name string, arguments map[string]any) (any, bool)
}

const (
	generatedScrollViewPanelCellName    = "panel"
	generatedScrollViewViewportCellName = "viewport"
	generatedScrollViewDefaultGap       = 0
)

func NewGeneratedWindowBase(title string) *GeneratedWindowBase {
	return &GeneratedWindowBase{title: title, kind: "window", windowStyle: NewStyle()}
}

func NewGeneratedScrollViewBase(title string) *GeneratedWindowBase {
	return &GeneratedWindowBase{title: title, kind: "scrollview", scrollViewRoot: true, windowStyle: NewStyle()}
}

func (window *GeneratedWindowBase) RuntimeOptions() GeneratedWindowRuntimeOptions {
	options := GeneratedWindowRuntimeOptions{}
	options.ShouldClose = window.ShouldClose
	return options
}

func (window *GeneratedWindowBase) SetGeneratedLayout(layout []LayoutItem) {
	window.layout = layout
}

func (window *GeneratedWindowBase) GeneratedLayout() []LayoutItem {
	return append([]LayoutItem{}, window.layout...)
}

func (window *GeneratedWindowBase) SetGeneratedKind(kind string) {
	window.kind = kind
}

func (window *GeneratedWindowBase) SetGeneratedFocusable(focusable bool) {
	window.focusable = focusable
}

func (window *GeneratedWindowBase) SetGeneratedWindowStyle(style Style) {
	window.windowStyle = style
}

func (window *GeneratedWindowBase) SetGeneratedScrollViewStyle(style Style) {
	window.scrollViewStyle = &style
}

func (window *GeneratedWindowBase) SetGeneratedScrollViewFocusStyle(style Style) {
	window.scrollViewFocusStyle = &style
}

func (window *GeneratedWindowBase) SetGeneratedScrollViewEditStyle(style Style) {
	window.scrollViewEditStyle = &style
}

func (window *GeneratedWindowBase) SetGeneratedScrollViewSelectedStyle(style Style) {
	window.scrollViewSelectedStyle = &style
}

func (window *GeneratedWindowBase) SetGeneratedScrollViewDescendantFocusStyle(style Style) {
	window.scrollViewDescendantFocusStyle = &style
}

func (window *GeneratedWindowBase) SetMCPMetadata(enabled bool, className string, source string, description string) {
	window.mcpEnabled = enabled
	window.mcpClassName = className
	window.mcpSource = source
	window.mcpDescription = description
}

func (window *GeneratedWindowBase) AddElement(element Element) Element {
	if window.scrollViewRoot {
		if scrollView, ok := element.(*ScrollView); ok && firstScrollViewInWindow(window) == nil {
			window.applyGeneratedScrollViewConfiguration(scrollView)
		}
	}
	window.elements = append(window.elements, element)
	return element
}

func (window *GeneratedWindowBase) NewGeneratedScrollViewElement() *ScrollView {
	name := generatedScrollViewPanelCellName
	gap := generatedScrollViewDefaultGap
	style := NewStyle()
	for _, item := range window.layout {
		if item.CellName != generatedScrollViewPanelCellName && item.CellName != generatedScrollViewViewportCellName {
			continue
		}
		name = item.CellName
		style = item.CellStyle
		if item.CellStyle.Gap != nil {
			gap = *item.CellStyle.Gap
		}
		break
	}
	scrollView := NewScrollView(name, gap)
	scrollView.SetStyle(style)
	window.applyGeneratedScrollViewConfiguration(scrollView)
	return scrollView
}

func (window *GeneratedWindowBase) applyGeneratedScrollViewConfiguration(scrollView *ScrollView) {
	if window == nil || scrollView == nil {
		return
	}
	style := scrollView.style.Clone()
	for _, item := range window.layout {
		if item.CellName != generatedScrollViewPanelCellName && item.CellName != generatedScrollViewViewportCellName {
			continue
		}
		style = item.CellStyle.Clone()
		break
	}
	if window.scrollViewStyle != nil {
		style.Merge(*window.scrollViewStyle)
	}
	if style.Gap != nil {
		scrollView.Gap = *style.Gap
	}
	scrollView.SetStyle(style)
	if window.scrollViewFocusStyle != nil {
		scrollView.SetFocusStyle(*window.scrollViewFocusStyle)
	}
	if window.scrollViewEditStyle != nil {
		scrollView.SetEditStyle(*window.scrollViewEditStyle)
	}
	if window.scrollViewSelectedStyle != nil {
		scrollView.SetSelectedStyle(*window.scrollViewSelectedStyle)
	}
	if window.scrollViewDescendantFocusStyle != nil {
		scrollView.SetDescendantFocusStyle(*window.scrollViewDescendantFocusStyle)
	}
}

func (window *GeneratedWindowBase) RequestClose() {
	window.closeRequested = true
}

func (window *GeneratedWindowBase) ShouldClose() bool {
	return window.closeRequested
}

func (window *GeneratedWindowBase) Title() string {
	return window.title
}

type renderableGeneratedWindow interface {
	Title() string
	ShouldClose() bool
}

func RunGeneratedAppMain(run func() int) int {
	defer func() {
		if recovered := recover(); recovered != nil {
			_, _ = fmt.Fprintf(os.Stderr, "error: %v\n", recovered)
		}
	}()
	return run()
}

func RunGeneratedWindow(window any, options GeneratedWindowRuntimeOptions, args []string) int {
	if base, ok := extractGeneratedWindowBase(window); ok {
		base.ownerObject = window
		bindGeneratedHandlers(window, &options)
		if provider, ok := window.(MCPToolProvider); ok {
			base.mcpToolProvider = provider
		}
		return runGeneratedWindowBase(base, options, args)
	}
	output := options.Output
	if output == nil {
		output = os.Stdout
	}
	title := ""
	if titled, ok := window.(interface{ Title() string }); ok {
		title = titled.Title()
	}
	_, _ = fmt.Fprintf(output, "%s\n", title)
	return 0
}

func extractGeneratedWindowBase(value any) (*GeneratedWindowBase, bool) {
	if base, ok := value.(*GeneratedWindowBase); ok {
		return base, true
	}
	return extractGeneratedWindowBaseValue(reflect.ValueOf(value))
}

func extractGeneratedWindowBaseValue(value reflect.Value) (*GeneratedWindowBase, bool) {
	if !value.IsValid() {
		return nil, false
	}
	if value.Kind() == reflect.Pointer {
		if value.IsNil() {
			return nil, false
		}
		if base, ok := value.Interface().(*GeneratedWindowBase); ok {
			return base, true
		}
		return extractGeneratedWindowBaseValue(value.Elem())
	}
	if value.Kind() != reflect.Struct {
		return nil, false
	}
	for index := 0; index < value.NumField(); index++ {
		field := value.Field(index)
		if !field.CanInterface() {
			continue
		}
		if base, ok := field.Interface().(*GeneratedWindowBase); ok && base != nil {
			return base, true
		}
		if field.Kind() == reflect.Pointer || field.Kind() == reflect.Struct {
			if base, ok := extractGeneratedWindowBaseValue(field); ok {
				return base, true
			}
		}
	}
	return nil, false
}

func (options GeneratedWindowFrameOptions) runtimeOptions() GeneratedWindowRuntimeOptions {
	return GeneratedWindowRuntimeOptions{
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
		OnFocusChanged:            options.OnFocusChanged,
		ShouldClose:               options.ShouldClose,
	}
}

func (stack *GeneratedWindowStack) Push(window any, options GeneratedWindowFrameOptions) {
	if stack == nil {
		return
	}
	base, ok := extractGeneratedWindowBase(window)
	if !ok || base == nil {
		return
	}
	base.ownerObject = window
	returnTo := stack.captureModalReturnState()
	frame := &GeneratedWindowStackFrame{
		Window:    base,
		Options:   options.runtimeOptions(),
		ClassName: options.ClassName,
		stack:     stack,
		returnTo:  returnTo,
	}
	stack.frames = append(stack.frames, frame)
}

func (stack *GeneratedWindowStack) Remove(window any) {
	if stack == nil {
		return
	}
	base, ok := extractGeneratedWindowBase(window)
	if !ok || base == nil {
		return
	}
	for index := len(stack.frames) - 1; index >= 0; index-- {
		if stack.frames[index].Window == base {
			returnTo := stack.frames[index].returnTo
			stack.frames = append(stack.frames[:index], stack.frames[index+1:]...)
			if !stack.restoreModalReturnState(returnTo) {
				stack.restoreRootScopeAfterOverlayClose()
			}
			return
		}
	}
}

func (stack *GeneratedWindowStack) captureModalReturnState() *modalReturnState {
	if stack == nil || stack.root == nil {
		return nil
	}
	state := stack.root
	if frame := stack.Top(); frame != nil {
		state = frame.runtimeState()
	}
	if state == nil || state.scopeDimElement == nil {
		return nil
	}
	host := state.scopeDimElement
	child := childWindowForElement(host)
	if child == nil {
		return nil
	}
	focused := state.focusedElement()
	focusable := focusableDescendantsInWindow(child)
	focusedIndex := -1
	for index, candidate := range focusable {
		if candidate == focused {
			focusedIndex = index
			break
		}
	}
	if focusedIndex < 0 && state.scopeLastIndex != nil {
		if rememberedIndex, ok := state.scopeLastIndex[host]; ok {
			focusedIndex = rememberedIndex
		}
	}
	scrollView := scrollViewContainingElement(child, focused)
	if scrollView == nil {
		scrollView = firstScrollViewInWindow(child)
	}
	result := &modalReturnState{
		state:        state,
		scopeHost:    host,
		focused:      focused,
		focusedIndex: focusedIndex,
		scrollView:   scrollView,
	}
	if scrollView != nil {
		result.viewOffset = scrollView.ViewOffset
		result.autoScroll = scrollView.AutoScroll
	}
	return result
}

func (stack *GeneratedWindowStack) restoreModalReturnState(saved *modalReturnState) bool {
	if stack == nil || saved == nil || saved.state == nil || saved.scopeHost == nil {
		return false
	}
	if activeRuntimeState(stack.root) != saved.state ||
		!elementInWindow(saved.state.window, saved.scopeHost) {
		return false
	}
	child := childWindowForElement(saved.scopeHost)
	if child == nil {
		return false
	}
	focusable := focusableDescendantsInWindow(child)
	target := saved.focused
	targetIndex := -1
	for index, candidate := range focusable {
		if candidate == target {
			targetIndex = index
			break
		}
	}
	if targetIndex < 0 && len(focusable) > 0 {
		targetIndex = clampInt(saved.focusedIndex, 0, len(focusable)-1)
		target = focusable[targetIndex]
	}

	previous := saved.state.focusedElement()
	saved.state.focusedIndex = -1
	saved.state.focusedOverride = target
	if target == nil {
		saved.state.focusedOverride = saved.scopeHost
	}
	saved.state.scopeDimElement = saved.scopeHost
	saved.state.scopeEditElement = nil
	saved.state.editMode = true
	saved.state.editSnapshot = nil
	if target != nil {
		if saved.state.scopeLastFocus == nil {
			saved.state.scopeLastFocus = map[Element]Element{}
		}
		if saved.state.scopeLastIndex == nil {
			saved.state.scopeLastIndex = map[Element]int{}
		}
		saved.state.scopeLastFocus[saved.scopeHost] = target
		saved.state.scopeLastIndex[saved.scopeHost] = targetIndex
	}
	if saved.scrollView != nil {
		viewportHeight := maxInt(minimumRenderableSize, saved.scrollView.viewportHeight())
		maxOffset := maxInt(0, saved.scrollView.contentHeight()-viewportHeight)
		saved.scrollView.ViewOffset = clampInt(saved.viewOffset, 0, maxOffset)
		saved.scrollView.AutoScroll = saved.autoScroll
	}
	saved.state.notifyFocusTransition(previous)
	return true
}

func (stack *GeneratedWindowStack) restoreRootScopeAfterOverlayClose() {
	if stack == nil || stack.root == nil || len(stack.frames) > 0 {
		return
	}
	focused := stack.root.focusedElement()
	if stack.root.scopeDimElement == nil || focused == stack.root.scopeDimElement {
		return
	}
	if elementInWindow(stack.root.window, focused) {
		stack.root.editMode = true
		stack.root.rememberReusableScrollScopeFocus()
		return
	}
	host := stack.root.scopeDimElement
	if childWindowForElement(host) != nil {
		if descendant := stack.root.reusableScrollScopeEntryTarget(host); descendant != nil {
			previous := stack.root.focusedElement()
			stack.root.focusedIndex = -1
			stack.root.focusedOverride = descendant
			stack.root.editMode = true
			stack.root.rememberReusableScrollScopeFocus()
			stack.root.notifyFocusTransition(previous)
			return
		}
		focusElement(stack.root, host)
	}
}

func (stack *GeneratedWindowStack) Top() *GeneratedWindowStackFrame {
	if stack == nil || len(stack.frames) == 0 {
		return nil
	}
	return stack.frames[len(stack.frames)-1]
}

func (stack *GeneratedWindowStack) Empty() bool {
	return stack == nil || len(stack.frames) == 0
}

func (frame *GeneratedWindowStackFrame) runtimeState() *runtimeState {
	if frame == nil {
		return nil
	}
	if frame.state == nil {
		frame.state = newRuntimeState(frame.Window, frame.Options)
		frame.state.className = frame.ClassName
		if frame.stack != nil && frame.stack.root != nil {
			frame.state.root = frame.stack.root
		}
	}
	return frame.state
}

func bindGeneratedHandlers(window any, options *GeneratedWindowRuntimeOptions) {
	if options.OnButton == nil {
		if handler, ok := window.(interface{ HandleGeneratedButton(string) bool }); ok {
			options.OnButton = func(name string) {
				handler.HandleGeneratedButton(name)
			}
		}
	}
	if options.OnTextChanged == nil {
		if handler, ok := window.(interface{ HandleGeneratedTextChanged(string, string) bool }); ok {
			options.OnTextChanged = func(name string, value string) {
				handler.HandleGeneratedTextChanged(name, value)
			}
		}
	}
	if options.OnTextConfirmed == nil {
		if handler, ok := window.(interface{ HandleGeneratedTextConfirmed(string, string) bool }); ok {
			options.OnTextConfirmed = func(name string, value string) {
				handler.HandleGeneratedTextConfirmed(name, value)
			}
		}
	}
	if options.OnSelectionChanged == nil {
		if handler, ok := window.(interface{ HandleGeneratedSelectionChanged(string, []string) bool }); ok {
			options.OnSelectionChanged = func(name string, values []string) {
				handler.HandleGeneratedSelectionChanged(name, values)
			}
		}
	}
}

func runGeneratedWindowBase(window *GeneratedWindowBase, options GeneratedWindowRuntimeOptions, args []string) int {
	if options.ShouldClose == nil {
		options.ShouldClose = window.ShouldClose
	}
	config := parseMcpRuntimeArgs(args)
	if config.Enabled {
		return serveMcp(window, options, config)
	}
	output := options.Output
	if output == nil {
		output = os.Stdout
	}
	if output != os.Stdout || !isTerminal(os.Stdout) || !isTerminal(os.Stdin) {
		rendered := RenderGeneratedWindowContent(window, Size{Width: defaultViewportWidth, Height: defaultViewportHeight}, -1, nil, false)
		_, _ = fmt.Fprintln(output, rendered.PlainText())
		return 0
	}
	return runInteractiveTerminal(window, options)
}

type runtimeState struct {
	window                   *GeneratedWindowBase
	options                  GeneratedWindowRuntimeOptions
	focusedIndex             int
	focusedOverride          Element
	scopeDimElement          Element
	scopeEditElement         Element
	scopeLastFocus           map[Element]Element
	scopeLastIndex           map[Element]int
	mousePressElement        Element
	editMode                 bool
	editSnapshot             *runtimeEditSnapshot
	root                     *runtimeState
	className                string
	notification             string
	notificationUntil        time.Time
	beforeDialogButtonAction func()
}

type runtimeEditSnapshot struct {
	element       Element
	textValue     string
	textCursor    int
	numberValue   float64
	selectedIndex int
}

func newRuntimeState(window *GeneratedWindowBase, options GeneratedWindowRuntimeOptions) *runtimeState {
	state := &runtimeState{window: window, options: options, focusedIndex: -1}
	state.root = state
	if options.WindowStack != nil {
		options.WindowStack.root = state
	}
	focusable := focusableElements(window)
	if options.InitialFocusName != "" {
		for index, element := range focusable {
			if element.ElementName() == options.InitialFocusName {
				state.focusedIndex = index
				break
			}
		}
	} else if options.StartInEditMode && len(focusable) > 0 {
		state.focusedIndex = 0
	}
	state.editMode = options.StartInEditMode && state.focusedIndex >= 0
	if state.editMode {
		focused := state.focusedElement()
		state.editSnapshot = captureRuntimeEditSnapshot(focused)
		beginRuntimeElementEdit(focused)
	}
	return state
}

func captureRuntimeEditSnapshot(element Element) *runtimeEditSnapshot {
	if element == nil {
		return nil
	}
	snapshot := &runtimeEditSnapshot{element: element}
	if input, ok := asTextInput(element); ok {
		snapshot.textValue = input.Value
		snapshot.textCursor = input.Cursor
		return snapshot
	}
	switch control := element.(type) {
	case *NumberInput:
		snapshot.numberValue = control.NumberValue
		control.BeginEdit()
	case *ComboBox:
		snapshot.selectedIndex = control.SelectedIndex
	case *ListBox:
		snapshot.selectedIndex = control.SelectedIndex
	}
	return snapshot
}

func beginRuntimeElementEdit(element Element) {
	if input, ok := asTextInput(element); ok {
		input.SetCursor(len([]rune(input.Value)))
		return
	}
	switch control := element.(type) {
	case *NumberInput:
		control.BeginEdit()
	case *ListBox:
		control.ActiveItemVisible = false
	}
}

func restoreRuntimeEditSnapshot(snapshot *runtimeEditSnapshot) {
	if snapshot == nil || snapshot.element == nil {
		return
	}
	if input, ok := asTextInput(snapshot.element); ok {
		input.SetValue(snapshot.textValue)
		input.SetCursor(snapshot.textCursor)
		return
	}
	switch control := snapshot.element.(type) {
	case *NumberInput:
		control.CancelEdit()
		control.SetValue(snapshot.numberValue)
	case *ComboBox:
		control.SetSelectedIndex(snapshot.selectedIndex)
	case *ListBox:
		control.SetSelectedIndex(snapshot.selectedIndex)
	}
}

func commitRuntimeElementEdit(element Element) {
	if numberInput, ok := element.(*NumberInput); ok {
		numberInput.CommitEdit()
	}
}

type runtimeCommitModeElement interface {
	CommitMode() string
}

func runtimeElementUsesLeaveCommit(element Element) bool {
	if listBox, ok := element.(*ListBox); ok {
		if listBox.Multi {
			return true
		}
	}
	commitModeElement, ok := element.(runtimeCommitModeElement)
	return ok && commitModeElement.CommitMode() == CommitModeLeave
}

func (state *runtimeState) beginElementEdit(element Element) {
	if state == nil || element == nil {
		return
	}
	if state.editSnapshot != nil && state.editSnapshot.element == element {
		return
	}
	state.editSnapshot = captureRuntimeEditSnapshot(element)
	beginRuntimeElementEdit(element)
}

func (state *runtimeState) endElementEdit(element Element, commit bool) {
	if state == nil {
		return
	}
	if element == nil {
		state.editSnapshot = nil
		return
	}
	leaveCommit := runtimeElementUsesLeaveCommit(element)
	if commit || leaveCommit {
		before := valueForElement(element)
		commitRuntimeElementEdit(element)
		if commit && !leaveCommit {
			state.dispatchConfirm(element)
		}
		state.dispatchChangeIfNeeded(element, before)
	} else if state.editSnapshot != nil && state.editSnapshot.element == element {
		restoreRuntimeEditSnapshot(state.editSnapshot)
	}
	state.editSnapshot = nil
}

func (state *runtimeState) commitEditBeforeFocusChange(target Element) {
	if state == nil || !state.editMode {
		return
	}
	element := state.scopeEditElement
	if element == nil {
		element = state.focusedElement()
	}
	if element == nil || element == target || state.editSnapshot == nil {
		return
	}
	state.endElementEdit(element, true)
	state.scopeEditElement = nil
	state.editMode = false
}

func (state *runtimeState) rootState() *runtimeState {
	if state == nil || state.root == nil {
		return state
	}
	return state.root
}

func activeRuntimeState(root *runtimeState) *runtimeState {
	if root == nil {
		return nil
	}
	if stack := root.rootState().options.WindowStack; stack != nil {
		if frame := stack.Top(); frame != nil {
			return frame.runtimeState()
		}
	}
	return root.rootState()
}

func (state *runtimeState) activeStackFrame() *GeneratedWindowStackFrame {
	if state == nil {
		return nil
	}
	root := state.rootState()
	if root == nil || root.options.WindowStack == nil {
		return nil
	}
	frame := root.options.WindowStack.Top()
	if frame == nil || frame.Window != state.window {
		return nil
	}
	return frame
}

func isStandardEscapeDialog(window *GeneratedWindowBase, className string) bool {
	if window == nil {
		return false
	}
	name := className
	if name == "" {
		name = window.mcpClassName
	}
	return name == "FileBrowser" || strings.HasPrefix(name, "MessageBox")
}

func standardEscapeButtonName(window *GeneratedWindowBase, className string) string {
	if !isStandardEscapeDialog(window, className) {
		return ""
	}
	name := className
	if name == "" {
		name = window.mcpClassName
	}
	if name == "FileBrowser" {
		if _, ok := findElement(window, "close_btn").(*Button); ok {
			return "close_btn"
		}
		return ""
	}
	for _, buttonName := range []string{"cancel_btn", "no_btn", "ok_btn"} {
		if _, ok := findElement(window, buttonName).(*Button); ok {
			return buttonName
		}
	}
	return ""
}

func (state *runtimeState) handleStandardEscapeButton() bool {
	if state == nil || state.window == nil {
		return false
	}
	root := state.rootState()
	buttonName := standardEscapeButtonName(state.window, state.className)
	if buttonName == "" {
		return false
	}
	button := findElement(state.window, buttonName)
	if button == nil {
		return false
	}
	focusElement(state, button)
	state.editMode = false
	state.scopeDimElement = nil
	state.scopeEditElement = nil
	if root != nil && root.beforeDialogButtonAction != nil {
		root.beforeDialogButtonAction()
	}
	state.dispatchButton(button)
	return true
}

func (state *runtimeState) focusedElement() Element {
	if state.focusedOverride != nil {
		return state.focusedOverride
	}
	focusable := focusableElements(state.window)
	if state.focusedIndex < 0 || state.focusedIndex >= len(focusable) {
		return nil
	}
	return focusable[state.focusedIndex]
}

func (state *runtimeState) notifyFocusTransition(previous Element) {
	if state == nil || state.options.OnFocusChanged == nil {
		return
	}
	next := state.focusedElement()
	if previous == next {
		return
	}
	if previous != nil {
		state.options.OnFocusChanged(previous.ElementName(), false)
	}
	if next != nil {
		state.options.OnFocusChanged(next.ElementName(), true)
	}
}

func (state *runtimeState) handleKey(key string) bool {
	if key == "" {
		return false
	}
	if key == "Ctrl+C" {
		state.window.RequestClose()
		return true
	}
	focused := state.focusedElement()
	focusedName := ""
	if focused != nil {
		focusedName = focused.ElementName()
	}
	if state.options.OnKeyBeforeFocusedElement != nil && state.options.OnKeyBeforeFocusedElement(key, focusedName, state.editMode) {
		return true
	}
	if state.editMode && focused != nil {
		if state.scopeDimElement != nil {
			return state.handleReusableScrollScopeKey(key, focused)
		}
		if key == "Escape" {
			state.endElementEdit(focused, runtimeElementUsesLeaveCommit(focused))
			state.editMode = false
			if state.options.KeepEditModeAfterEscape {
				state.beginElementEdit(focused)
				state.editMode = true
			}
			return true
		}
		before := valueForElement(focused)
		handled := focused.HandleKey(key)
		if key == "Enter" && !handled {
			state.endElementEdit(focused, true)
			state.editMode = false
			if state.options.KeepEditModeAfterConfirm {
				state.beginElementEdit(focused)
				state.editMode = true
			}
			return true
		}
		if handled {
			state.dispatchChangeIfNeeded(focused, before)
			if key == "Enter" {
				if listBox, ok := focused.(*ListBox); ok && !listBox.Multi {
					state.endElementEdit(focused, true)
					state.editMode = false
					if state.options.KeepEditModeAfterConfirm {
						state.beginElementEdit(focused)
						state.editMode = true
					}
				}
			}
		}
		return handled
	}
	if key == "Escape" && state.scopeDimElement != nil && focused != nil {
		return state.handleReusableScrollScopeKey(key, focused)
	}
	if key == "Escape" && state.handleStandardEscapeButton() {
		return true
	}
	if state.options.OnKey != nil && state.options.OnKey(key) {
		return true
	}
	if state.scopeDimElement != nil && focused != nil {
		switch key {
		case "Up", "Down", "Left", "Right":
			state.moveReusableScrollScopeFocus(key)
			return true
		case "Enter", " ":
			state.activateReusableScrollScopeFocused(focused, key)
			return true
		}
	}
	switch key {
	case "Tab":
		state.moveFocusLinear(1)
		return true
	case "Shift+Tab":
		state.moveFocusLinear(-1)
		return true
	case "Up", "Down", "Left", "Right":
		state.moveFocusSpatial(key)
		return true
	case "Enter", " ":
		state.activateFocused()
		return true
	case "Escape":
		return true
	default:
		return false
	}
}

func (state *runtimeState) moveFocusLinear(delta int) {
	previous := state.focusedElement()
	focusable := focusableElements(state.window)
	if len(focusable) == 0 {
		state.focusedIndex = -1
		state.focusedOverride = nil
		state.notifyFocusTransition(previous)
		return
	}
	if state.focusedIndex < 0 {
		state.focusedIndex = 0
		state.focusedOverride = nil
		state.notifyFocusTransition(previous)
		return
	}
	state.focusedIndex = (state.focusedIndex + delta + len(focusable)) % len(focusable)
	state.focusedOverride = nil
	state.notifyFocusTransition(previous)
}

func (state *runtimeState) moveFocusSpatial(direction string) {
	previous := state.focusedElement()
	focusable := focusableElements(state.window)
	if len(focusable) == 0 {
		state.notifyFocusTransition(previous)
		return
	}
	if state.focusedIndex < 0 || state.focusedIndex >= len(focusable) {
		state.focusedIndex = 0
		state.focusedOverride = nil
		state.notifyFocusTransition(previous)
		return
	}
	if bestIndex := spatialFocusTargetIndex(focusable, state.focusedIndex, direction); bestIndex >= 0 {
		state.focusedIndex = bestIndex
		state.focusedOverride = nil
	}
	state.notifyFocusTransition(previous)
}

func spatialFocusTargetIndex(focusable []Element, focusedIndex int, direction string) int {
	if len(focusable) == 0 {
		return -1
	}
	if focusedIndex < 0 || focusedIndex >= len(focusable) {
		return 0
	}
	current := focusable[focusedIndex].ElementFrame()
	currentRow := centerRow(current)
	currentCol := centerCol(current)
	bestIndex := -1
	bestBand := 0
	bestPrimary := 0
	bestGap := 0
	bestEdge := 0
	for index, element := range focusable {
		if index == focusedIndex {
			continue
		}
		candidate := element.ElementFrame()
		primary := 0
		gap := 0
		edge := 0
		band := -1
		switch direction {
		case "Up":
			if centerRow(candidate) >= currentRow || candidate.Row+candidate.Height > current.Row {
				continue
			}
			primary = maxInt(0, current.Row-(candidate.Row+candidate.Height))
			if value, ok := bandScanRank(current.Col, current.Col+current.Width, candidate.Col, candidate.Col+candidate.Width); ok {
				band = value
			}
			gap = axisGap(current.Col, current.Col+current.Width, candidate.Col, candidate.Col+candidate.Width)
			edge = absInt(candidate.Col - current.Col)
		case "Down":
			if centerRow(candidate) <= currentRow || candidate.Row < current.Row+current.Height {
				continue
			}
			primary = maxInt(0, candidate.Row-(current.Row+current.Height))
			if value, ok := bandScanRank(current.Col, current.Col+current.Width, candidate.Col, candidate.Col+candidate.Width); ok {
				band = value
			}
			gap = axisGap(current.Col, current.Col+current.Width, candidate.Col, candidate.Col+candidate.Width)
			edge = absInt(candidate.Col - current.Col)
		case "Right":
			if centerCol(candidate) <= currentCol || candidate.Col < current.Col+current.Width {
				continue
			}
			primary = maxInt(0, candidate.Col-(current.Col+current.Width))
			if value, ok := bandScanRank(current.Row, current.Row+current.Height, candidate.Row, candidate.Row+candidate.Height); ok {
				band = value
			}
			gap = axisGap(current.Row, current.Row+current.Height, candidate.Row, candidate.Row+candidate.Height)
			edge = absInt(candidate.Row - current.Row)
		case "Left":
			if centerCol(candidate) >= currentCol || candidate.Col+candidate.Width > current.Col {
				continue
			}
			primary = maxInt(0, current.Col-(candidate.Col+candidate.Width))
			if value, ok := bandScanRank(current.Row, current.Row+current.Height, candidate.Row, candidate.Row+candidate.Height); ok {
				band = value
			}
			gap = axisGap(current.Row, current.Row+current.Height, candidate.Row, candidate.Row+candidate.Height)
			edge = absInt(candidate.Row - current.Row)
		default:
			continue
		}
		if band < 0 {
			continue
		}
		if bestIndex < 0 ||
			band < bestBand ||
			(band == bestBand && primary < bestPrimary) ||
			(band == bestBand && primary == bestPrimary && gap < bestGap) ||
			(band == bestBand && primary == bestPrimary && gap == bestGap && edge < bestEdge) {
			bestIndex = index
			bestBand = band
			bestPrimary = primary
			bestGap = gap
			bestEdge = edge
		}
	}
	return bestIndex
}

func centerRow(rect Rect) int {
	return rect.Row + rect.Height/2
}

func centerCol(rect Rect) int {
	return rect.Col + rect.Width/2
}

func axisOverlaps(startA int, endA int, startB int, endB int) bool {
	return maxInt(startA, startB) < minInt(endA, endB)
}

func axisGap(startA int, endA int, startB int, endB int) int {
	if endA <= startB {
		return startB - endA
	}
	if endB <= startA {
		return startA - endB
	}
	return 0
}

func bandScanRank(currentStart int, currentEnd int, candidateStart int, candidateEnd int) (int, bool) {
	if axisOverlaps(currentStart, currentEnd, candidateStart, candidateEnd) {
		return 0, true
	}
	maxDistance := maxInt(
		maxInt(absInt(candidateStart-currentStart), absInt(candidateEnd-currentEnd)),
		maxInt(absInt(candidateStart-currentEnd), absInt(candidateEnd-currentStart)),
	) + maxInt(1, currentEnd-currentStart) + maxInt(1, candidateEnd-candidateStart)
	rank := 1
	for distance := 1; distance <= maxDistance; distance++ {
		for _, offset := range []int{distance, -distance} {
			if axisOverlaps(currentStart+offset, currentEnd+offset, candidateStart, candidateEnd) {
				return rank, true
			}
			rank++
		}
	}
	return 0, false
}

func (state *runtimeState) activateFocused() {
	focused := state.focusedElement()
	if focused == nil {
		return
	}
	if state.scopeDimElement != nil && focused != state.scopeDimElement {
		state.activateReusableScrollScopeFocused(focused, "Enter")
		return
	}
	defer state.repairInvalidRootScopeFocusAfterAction()
	before := valueForElement(focused)
	switch element := focused.(type) {
	case *Button:
		state.dispatchButton(element)
	case *CheckBox:
		if element.HandleKey("Enter") {
			state.dispatchChangeIfNeeded(element, before)
		}
	case *NumberInput:
		state.beginElementEdit(element)
		state.editMode = true
	case *TextInput:
		state.beginElementEdit(element)
		state.editMode = true
	case *TextArea:
		state.beginElementEdit(element)
		state.editMode = true
	case *ComboBox:
		state.beginElementEdit(element)
		state.editMode = true
	case *ListBox:
		state.beginElementEdit(element)
		state.editMode = true
	case *ReusableElement:
		if activateReusableControl(element) {
			return
		}
		if !state.enterReusableScrollScope(element) {
			state.editMode = true
		}
	case *ViewHost:
		if activateReusableControl(&element.ReusableElement) {
			return
		}
		if !state.enterReusableScrollScope(element) {
			state.editMode = true
		}
	default:
		state.editMode = true
	}
}

func (state *runtimeState) handleReusableScrollScopeKey(key string, focused Element) bool {
	if key == "Escape" {
		if state.scopeEditElement != nil {
			state.endElementEdit(state.scopeEditElement, runtimeElementUsesLeaveCommit(state.scopeEditElement))
			state.scopeEditElement = nil
			return true
		}
		state.rememberReusableScrollScopeFocus()
		focusElement(state, state.scopeDimElement)
		return true
	}
	if state.scopeEditElement == nil {
		switch key {
		case "Up", "Down", "Left", "Right":
			return state.moveReusableScrollScopeFocus(key)
		case "Enter", " ":
			return state.activateReusableScrollScopeFocused(focused, key)
		default:
			return false
		}
	}
	before := valueForElement(focused)
	handled := focused.HandleKey(key)
	if key == "Enter" && !handled {
		state.endElementEdit(focused, true)
		state.scopeEditElement = nil
		return true
	}
	if handled {
		state.dispatchChangeIfNeeded(focused, before)
		if key == "Enter" {
			if listBox, ok := focused.(*ListBox); ok && !listBox.Multi {
				state.endElementEdit(focused, true)
				state.scopeEditElement = nil
			}
		}
	}
	return handled
}

func (state *runtimeState) activateReusableScrollScopeFocused(focused Element, key string) bool {
	if focused == nil {
		return true
	}
	before := valueForElement(focused)
	switch element := focused.(type) {
	case *Button:
		state.dispatchButton(element)
		state.repairInvalidRootScopeFocusAfterAction()
		return true
	case *CheckBox:
		if element.HandleKey(key) {
			state.dispatchChangeIfNeeded(element, before)
		}
		state.repairInvalidRootScopeFocusAfterAction()
		return true
	case *NumberInput, *TextInput, *TextArea, *ComboBox, *ListBox:
		state.beginElementEdit(element)
		state.scopeEditElement = element
		state.editMode = true
		return true
	case *ReusableElement:
		if state.enterReusableScrollScope(element) {
			return true
		}
	case *ViewHost:
		if state.enterReusableScrollScope(element) {
			return true
		}
	}
	return false
}

func (state *runtimeState) repairInvalidRootScopeFocusAfterAction() {
	root := state.rootState()
	if root == nil || root.scopeDimElement == nil {
		return
	}
	if root.options.WindowStack != nil && !root.options.WindowStack.Empty() {
		return
	}
	if !elementInWindow(root.window, root.scopeDimElement) {
		previous := root.focusedElement()
		delete(root.scopeLastFocus, root.scopeDimElement)
		delete(root.scopeLastIndex, root.scopeDimElement)
		root.focusedIndex = -1
		root.focusedOverride = nil
		root.scopeDimElement = nil
		root.scopeEditElement = nil
		root.editMode = false
		root.editSnapshot = nil
		root.notifyFocusTransition(previous)
		return
	}
	focused := root.focusedElement()
	if focused != nil && elementInWindow(root.window, focused) {
		return
	}
	host := root.scopeDimElement
	if childWindowForElement(host) == nil {
		return
	}
	if descendant := root.reusableScrollScopeEntryTarget(host); descendant != nil {
		previous := root.focusedElement()
		root.focusedIndex = -1
		root.focusedOverride = descendant
		root.editMode = true
		root.rememberReusableScrollScopeFocus()
		root.notifyFocusTransition(previous)
		return
	}
	focusElement(root, host)
}

func (state *runtimeState) enterReusableScrollScope(element Element) bool {
	child := childWindowForElement(element)
	if state == nil || element == nil || !generatedWindowHasScrollView(child) {
		return false
	}
	previous := state.focusedElement()
	state.scopeDimElement = element
	state.scopeEditElement = nil
	state.editMode = true
	if descendant := state.reusableScrollScopeEntryTarget(element); descendant != nil {
		state.focusedIndex = -1
		state.focusedOverride = descendant
		state.rememberReusableScrollScopeFocus()
		state.notifyFocusTransition(previous)
	}
	return true
}

func (state *runtimeState) reusableScrollScopeEntryTarget(element Element) Element {
	if state == nil || element == nil {
		return nil
	}
	focusable := focusableDescendantsInWindow(childWindowForElement(element))
	if len(focusable) == 0 {
		return nil
	}
	if state.scopeLastFocus != nil {
		remembered := state.scopeLastFocus[element]
		for _, candidate := range focusable {
			if candidate == remembered {
				return candidate
			}
		}
		delete(state.scopeLastFocus, element)
	}
	if state.scopeLastIndex != nil {
		rememberedIndex, ok := state.scopeLastIndex[element]
		if ok && rememberedIndex >= 0 {
			return focusable[minInt(rememberedIndex, len(focusable)-1)]
		}
	}
	return focusable[0]
}

func (state *runtimeState) rememberReusableScrollScopeFocus() {
	if state == nil || state.scopeDimElement == nil {
		return
	}
	host := state.scopeDimElement
	child := childWindowForElement(host)
	if child == nil {
		return
	}
	current := state.focusedElement()
	if current == nil || current == host {
		return
	}
	for index, candidate := range focusableDescendantsInWindow(child) {
		if candidate == current {
			if state.scopeLastFocus == nil {
				state.scopeLastFocus = map[Element]Element{}
			}
			if state.scopeLastIndex == nil {
				state.scopeLastIndex = map[Element]int{}
			}
			state.scopeLastFocus[host] = current
			state.scopeLastIndex[host] = index
			return
		}
	}
}

func (state *runtimeState) moveReusableScrollScopeFocus(key string) bool {
	if state == nil || state.scopeDimElement == nil {
		return false
	}
	child := childWindowForElement(state.scopeDimElement)
	if child == nil {
		return false
	}
	focusable := focusableDescendantsInWindow(child)
	if len(focusable) == 0 {
		return false
	}
	current := state.focusedElement()
	currentIndex := -1
	for index, element := range focusable {
		if element == current {
			currentIndex = index
			break
		}
	}
	if currentIndex < 0 {
		if state.scopeLastIndex != nil {
			if rememberedIndex, ok := state.scopeLastIndex[state.scopeDimElement]; ok && rememberedIndex >= 0 {
				currentIndex = minInt(rememberedIndex, len(focusable)-1)
			}
		}
		if currentIndex < 0 {
			previous := state.focusedElement()
			state.focusedIndex = -1
			state.focusedOverride = focusable[0]
			state.scopeEditElement = nil
			state.rememberReusableScrollScopeFocus()
			state.notifyFocusTransition(previous)
			return true
		}
	}
	if key != "Up" && key != "Down" && key != "Left" && key != "Right" {
		return false
	}
	targetIndex := spatialFocusTargetIndex(focusable, currentIndex, key)
	if targetIndex < 0 {
		if key != "Up" && key != "Down" {
			return false
		}
		delta := 1
		if key == "Up" {
			delta = -1
		}
		if !scrollReusableScopeBy(child, focusable[currentIndex], delta) {
			return false
		}
		syncReusableChildFrames(state.scopeDimElement, state.scopeDimElement.ElementFrame())
		focusable = focusableDescendantsInWindow(child)
		currentIndex = -1
		for index, element := range focusable {
			if element == current {
				currentIndex = index
				break
			}
		}
		if currentIndex < 0 {
			return true
		}
		targetIndex = spatialFocusTargetIndex(focusable, currentIndex, key)
		if targetIndex < 0 {
			return true
		}
	}
	previous := state.focusedElement()
	state.focusedIndex = -1
	state.focusedOverride = focusable[targetIndex]
	state.scopeEditElement = nil
	state.rememberReusableScrollScopeFocus()
	state.notifyFocusTransition(previous)
	ensureElementVisibleInScrollViews(child, focusable[targetIndex])
	return true
}

func scrollReusableScopeBy(window *GeneratedWindowBase, target Element, delta int) bool {
	if window == nil || target == nil {
		return false
	}
	for _, element := range window.elements {
		switch control := element.(type) {
		case *ScrollView:
			if control == target || scrollViewContainsElement(control, target) {
				before := control.ViewOffset
				control.ScrollBy(delta)
				return control.ViewOffset != before
			}
		}
		if child := childWindowForElement(element); child != nil && scrollReusableScopeBy(child, target, delta) {
			return true
		}
	}
	return false
}

func ensureElementVisibleInScrollViews(window *GeneratedWindowBase, target Element) bool {
	if window == nil || target == nil {
		return false
	}
	for _, element := range window.elements {
		switch control := element.(type) {
		case *ScrollView:
			if ensureElementVisibleInScrollView(control, target) {
				return true
			}
		}
		if child := childWindowForElement(element); child != nil && ensureElementVisibleInScrollViews(child, target) {
			return true
		}
	}
	return false
}

func ensureElementVisibleInScrollView(scrollView *ScrollView, target Element) bool {
	if scrollView == nil || target == nil {
		return false
	}
	cursor := 0
	for _, child := range scrollView.Children {
		childHeight := maxInt(minimumRenderableSize, child.ElementFrame().Height)
		if elementContainsTarget(child, target) {
			viewportHeight := maxInt(minimumRenderableSize, scrollView.viewportHeight())
			before := scrollView.ViewOffset
			targetTop := cursor
			targetBottom := cursor + childHeight
			if child != target {
				if childWindow := childWindowForElement(child); childWindow != nil && elementInWindow(childWindow, target) {
					childWidth := maxInt(minimumRenderableSize, scrollView.lastSize.Width)
					if childWidth == minimumRenderableSize && scrollView.frame.Width > 0 {
						childWidth = maxInt(minimumRenderableSize, scrollView.frame.Width)
					}
					syncWindowElementFramesTo(
						childWindow,
						Rect{Row: 0, Col: 0, Width: childWidth, Height: childHeight},
						true)
				}
				targetFrame := target.ElementFrame()
				if targetFrame.Row >= 0 && targetFrame.Row < childHeight {
					targetTop = cursor + targetFrame.Row
					targetBottom = targetTop + maxInt(minimumRenderableSize, targetFrame.Height)
				}
			}
			if targetTop < scrollView.ViewOffset {
				scrollView.ViewOffset = targetTop
			} else if targetBottom > scrollView.ViewOffset+viewportHeight {
				scrollView.ViewOffset = targetBottom - viewportHeight
			}
			scrollView.ViewOffset = clampInt(scrollView.ViewOffset, 0, maxInt(0, scrollView.contentHeight()-viewportHeight))
			return scrollView.ViewOffset != before
		}
		if nested, ok := child.(*ScrollView); ok && ensureElementVisibleInScrollView(nested, target) {
			return true
		}
		if childWindow := childWindowForElement(child); childWindow != nil && ensureElementVisibleInScrollViews(childWindow, target) {
			return true
		}
		cursor += childHeight + scrollView.Gap
	}
	return false
}

func elementContainsTarget(element Element, target Element) bool {
	if element == nil || target == nil {
		return false
	}
	if element == target {
		return true
	}
	if child := childWindowForElement(element); child != nil && elementInWindow(child, target) {
		return true
	}
	if scrollView, ok := element.(*ScrollView); ok && scrollViewContainsElement(scrollView, target) {
		return true
	}
	return false
}

func (state *runtimeState) dispatchChangeIfNeeded(element Element, before string) {
	after := valueForElement(element)
	if before == after {
		return
	}
	switch control := element.(type) {
	case *TextInput:
		state.dispatchTextChanged(control, control.Value)
	case *TextArea:
		state.dispatchTextChanged(control, control.Value)
	case *NumberInput:
		state.dispatchTextChanged(control, control.Value)
	case *CheckBox:
		state.dispatchTextChanged(control, strconv.FormatBool(control.Checked))
	case *ComboBox:
		state.dispatchSelectionChanged(control, []string{control.Value})
	case *ListBox:
		state.dispatchSelectionChanged(control, control.SelectedValues())
	}
}

func (state *runtimeState) dispatchConfirm(element Element) {
	switch control := element.(type) {
	case *TextInput:
		state.dispatchTextConfirmed(control, control.Value)
	case *TextArea:
		state.dispatchTextConfirmed(control, control.Value)
	case *NumberInput:
		state.dispatchTextConfirmed(control, control.Value)
	case *ComboBox:
		state.dispatchTextConfirmed(control, control.Value)
	case *ListBox:
		values := control.SelectedValues()
		value := ""
		if len(values) > 0 {
			value = values[0]
		}
		state.dispatchTextConfirmed(control, value)
	}
}

func (state *runtimeState) dispatchButton(element Element) {
	if state.dispatchChildOwnerButton(element) {
		return
	}
	if frame := state.activeStackFrame(); frame != nil && frame.Options.OnButton != nil {
		frame.Options.OnButton(element.ElementName())
		return
	}
	if state.dispatchGeneratedButton(element) {
		return
	}
	if state.options.OnButton != nil {
		state.options.OnButton(element.ElementName())
	}
}

func (state *runtimeState) dispatchTextChanged(element Element, value string) {
	if state.dispatchChildOwnerTextChanged(element, value) {
		return
	}
	if frame := state.activeStackFrame(); frame != nil && frame.Options.OnTextChanged != nil {
		frame.Options.OnTextChanged(element.ElementName(), value)
		return
	}
	if state.dispatchGeneratedTextChanged(element, value) {
		return
	}
	if state.options.OnTextChanged != nil {
		state.options.OnTextChanged(element.ElementName(), value)
	}
}

func (state *runtimeState) dispatchTextConfirmed(element Element, value string) {
	if state.dispatchChildOwnerTextConfirmed(element, value) {
		return
	}
	if frame := state.activeStackFrame(); frame != nil && frame.Options.OnTextConfirmed != nil {
		frame.Options.OnTextConfirmed(element.ElementName(), value)
		return
	}
	if state.dispatchGeneratedTextConfirmed(element, value) {
		return
	}
	if state.options.OnTextConfirmed != nil {
		state.options.OnTextConfirmed(element.ElementName(), value)
	}
}

func (state *runtimeState) dispatchSelectionChanged(element Element, values []string) {
	if state.dispatchChildOwnerSelectionChanged(element, values) {
		return
	}
	if frame := state.activeStackFrame(); frame != nil && frame.Options.OnSelectionChanged != nil {
		frame.Options.OnSelectionChanged(element.ElementName(), values)
		return
	}
	if state.dispatchGeneratedSelectionChanged(element, values) {
		return
	}
	if state.options.OnSelectionChanged != nil {
		state.options.OnSelectionChanged(element.ElementName(), values)
	}
}

func (state *runtimeState) dispatchChildOwnerButton(element Element) bool {
	owner := state.childOwnerObjectForElement(element)
	if owner == nil {
		return false
	}
	if handler, ok := owner.(interface{ HandleGeneratedButton(string) bool }); ok {
		handler.HandleGeneratedButton(element.ElementName())
	}
	return true
}

func (state *runtimeState) dispatchChildOwnerTextChanged(element Element, value string) bool {
	owner := state.childOwnerObjectForElement(element)
	if owner == nil {
		return false
	}
	if handler, ok := owner.(interface{ HandleGeneratedTextChanged(string, string) bool }); ok {
		handler.HandleGeneratedTextChanged(element.ElementName(), value)
	}
	return true
}

func (state *runtimeState) dispatchChildOwnerTextConfirmed(element Element, value string) bool {
	owner := state.childOwnerObjectForElement(element)
	if owner == nil {
		return false
	}
	if handler, ok := owner.(interface{ HandleGeneratedTextConfirmed(string, string) bool }); ok {
		handler.HandleGeneratedTextConfirmed(element.ElementName(), value)
	}
	return true
}

func (state *runtimeState) dispatchChildOwnerSelectionChanged(element Element, values []string) bool {
	owner := state.childOwnerObjectForElement(element)
	if owner == nil {
		return false
	}
	if handler, ok := owner.(interface{ HandleGeneratedSelectionChanged(string, []string) bool }); ok {
		handler.HandleGeneratedSelectionChanged(element.ElementName(), values)
	}
	return true
}

func (state *runtimeState) childOwnerObjectForElement(element Element) any {
	if state == nil || element == nil || state.window == nil {
		return nil
	}
	owner := ownerWindowForElement(state.window, element)
	if owner == nil || owner == state.window {
		return nil
	}
	return ownerObjectForWindow(owner)
}

func (state *runtimeState) dispatchGeneratedButton(element Element) bool {
	owner := ownerObjectForElement(state.window, element)
	if handler, ok := owner.(interface{ HandleGeneratedButton(string) bool }); ok {
		return handler.HandleGeneratedButton(element.ElementName())
	}
	return false
}

func activateReusableControl(element *ReusableElement) bool {
	if element == nil || element.ChildObject == nil || !element.IsEnabled() {
		return false
	}
	if handler, ok := element.ChildObject.(interface{ ActivateGeneratedControl() bool }); ok {
		return handler.ActivateGeneratedControl()
	}
	return false
}

func (state *runtimeState) dispatchGeneratedTextChanged(element Element, value string) bool {
	owner := ownerObjectForElement(state.window, element)
	if handler, ok := owner.(interface{ HandleGeneratedTextChanged(string, string) bool }); ok {
		return handler.HandleGeneratedTextChanged(element.ElementName(), value)
	}
	return false
}

func (state *runtimeState) dispatchGeneratedTextConfirmed(element Element, value string) bool {
	owner := ownerObjectForElement(state.window, element)
	if handler, ok := owner.(interface{ HandleGeneratedTextConfirmed(string, string) bool }); ok {
		return handler.HandleGeneratedTextConfirmed(element.ElementName(), value)
	}
	return false
}

func (state *runtimeState) dispatchGeneratedSelectionChanged(element Element, values []string) bool {
	owner := ownerObjectForElement(state.window, element)
	if handler, ok := owner.(interface{ HandleGeneratedSelectionChanged(string, []string) bool }); ok {
		return handler.HandleGeneratedSelectionChanged(element.ElementName(), values)
	}
	return false
}

const (
	generatedWindowModeFullscreen   = "fullscreen"
	generatedWindowModeExpandWidth  = "expand_width"
	generatedWindowModeExpandHeight = "expand_height"
	generatedWindowModeNormal       = "normal"
)

func RenderGeneratedWindowContent(window *GeneratedWindowBase, size Size, focusedIndex int, focusedElement Element, editMode bool) *TerminalBuffer {
	return renderGeneratedWindowContent(window, size, focusedIndex, focusedElement, editMode, false)
}

func GeneratedWindowContentSize(window any) Size {
	base, ok := extractGeneratedWindowBase(window)
	if !ok || base == nil {
		return Size{Width: minimumRenderableSize, Height: minimumRenderableSize}
	}
	natural := naturalLayoutSize(base.layout, base.windowStyle, base)
	cellRects := resolvedRuntimeCellRects(base.layout, natural, borderWidthHorizontal(base.windowStyle), borderWidthVertical(base.windowStyle), generatedWindowMode(base.layout), base)
	return resolvedContentExtent(cellRects, natural)
}

func GeneratedWindowContentSizeForWidth(window any, width int) Size {
	base, ok := extractGeneratedWindowBase(window)
	if !ok || base == nil {
		return Size{Width: minimumRenderableSize, Height: minimumRenderableSize}
	}
	targetWidth := maxInt(minimumRenderableSize, width)
	natural := naturalLayoutSize(base.layout, base.windowStyle, base)
	contentSize := Size{Width: targetWidth, Height: natural.Height}
	cellRects := resolvedRuntimeCellRects(base.layout, contentSize, borderWidthHorizontal(base.windowStyle), borderWidthVertical(base.windowStyle), generatedWindowMode(base.layout), base)
	result := resolvedContentExtent(cellRects, contentSize)
	for _, item := range base.layout {
		cellRect := resolvedCellRect(item, cellRects)
		entrySize := renderSizeFor(item, cellRect, layoutElementForItem(base, item))
		result.Width = maxInt(result.Width, cellRect.Col+paddingLeft(item.CellStyle)+item.Col+entrySize.Width)
		result.Height = maxInt(result.Height, cellRect.Row+paddingTop(item.CellStyle)+item.Row+entrySize.Height)
	}
	return result
}

func renderGeneratedWindowContent(window *GeneratedWindowBase, size Size, focusedIndex int, focusedElement Element, editMode bool, forceFullscreenLayout bool) *TerminalBuffer {
	return renderGeneratedWindowContentWithOptions(window, size, focusedIndex, focusedElement, editMode, forceFullscreenLayout, false)
}

func renderGeneratedWindowContentWithOptions(window *GeneratedWindowBase, size Size, focusedIndex int, focusedElement Element, editMode bool, forceFullscreenLayout bool, suppressScopeFocus bool) *TerminalBuffer {
	return renderGeneratedWindowContentWithEditElement(window, size, focusedIndex, focusedElement, editMode, forceFullscreenLayout, suppressScopeFocus, nil, false)
}

func renderGeneratedWindowContentWithEditElement(window *GeneratedWindowBase, size Size, focusedIndex int, focusedElement Element, editMode bool, forceFullscreenLayout bool, suppressScopeFocus bool, editElement Element, explicitEditElement bool) *TerminalBuffer {
	return renderGeneratedWindowContentWithEditElementOptions(window, size, focusedIndex, focusedElement, editMode, forceFullscreenLayout, suppressScopeFocus, editElement, explicitEditElement, false)
}

func renderGeneratedWindowContentWithEditElementOptions(window *GeneratedWindowBase, size Size, focusedIndex int, focusedElement Element, editMode bool, forceFullscreenLayout bool, suppressScopeFocus bool, editElement Element, explicitEditElement bool, useHostViewportForRootScrollViewIndicators bool) *TerminalBuffer {
	return renderGeneratedWindowContentWithEditElementClipOptions(
		window,
		size,
		focusedIndex,
		focusedElement,
		editMode,
		forceFullscreenLayout,
		suppressScopeFocus,
		editElement,
		explicitEditElement,
		nil,
		nil,
		useHostViewportForRootScrollViewIndicators,
		editMode)
}

func renderGeneratedWindowContentWithEditElementClipOptions(window *GeneratedWindowBase, size Size, focusedIndex int, focusedElement Element, editMode bool, forceFullscreenLayout bool, suppressScopeFocus bool, editElement Element, explicitEditElement bool, clipTop *int, clipBottom *int, useHostViewportForRootScrollViewIndicators bool, scopeFocusActive bool) *TerminalBuffer {
	width := maxInt(minimumRenderableSize, size.Width)
	height := maxInt(minimumRenderableSize, size.Height)
	buffer := NewTerminalBuffer(width, height)
	horizontalBorder := borderWidthHorizontal(window.windowStyle)
	verticalBorder := borderWidthVertical(window.windowStyle)
	contentSize := naturalLayoutSize(window.layout, window.windowStyle, window)
	mode := generatedWindowMode(window.layout)
	if forceFullscreenLayout {
		contentSize.Width = maxInt(minimumRenderableSize, width-verticalBorder*2)
		contentSize.Height = maxInt(minimumRenderableSize, height-horizontalBorder*2)
	} else if window.kind != "dialog" {
		if mode == generatedWindowModeFullscreen || mode == generatedWindowModeExpandWidth {
			contentSize.Width = maxInt(contentSize.Width, width-verticalBorder*2)
		}
		if mode == generatedWindowModeFullscreen || mode == generatedWindowModeExpandHeight {
			contentSize.Height = maxInt(contentSize.Height, height-horizontalBorder*2)
		}
	}
	outerSize := Size{
		Width:  maxInt(minimumRenderableSize, contentSize.Width+verticalBorder*2),
		Height: maxInt(minimumRenderableSize, contentSize.Height+horizontalBorder*2),
	}
	rowOffset := maxInt(0, (height-outerSize.Height)/2)
	colOffset := maxInt(0, (width-outerSize.Width)/2)
	if forceFullscreenLayout {
		rowOffset = 0
		colOffset = 0
	}
	buffer.Fill(Rect{Row: rowOffset, Col: colOffset, Width: outerSize.Width, Height: outerSize.Height}, window.windowStyle)
	contentRowOffset := rowOffset + horizontalBorder
	contentColOffset := colOffset + verticalBorder
	if forceFullscreenLayout {
		mode = generatedWindowModeFullscreen
	}
	renderGeneratedWindowLayout(
		buffer,
		window,
		contentSize,
		contentRowOffset,
		contentColOffset,
		mode,
		focusedIndex,
		focusedElement,
		editMode,
		suppressScopeFocus,
		editElement,
		explicitEditElement,
		clipTop,
		clipBottom,
		useHostViewportForRootScrollViewIndicators,
		scopeFocusActive)
	if useHostViewportForRootScrollViewIndicators {
		applyRootScrollViewHostViewportIndicators(buffer, window, Size{Width: width, Height: height})
	}
	return buffer
}

func renderGeneratedWindowLayout(buffer *TerminalBuffer, window *GeneratedWindowBase, contentSize Size, contentRowOffset int, contentColOffset int, mode string, focusedIndex int, focusedElement Element, editMode bool, suppressScopeFocus bool, editElement Element, explicitEditElement bool, renderClipTop *int, renderClipBottom *int, useHostViewportForRootScrollViewIndicators bool, scopeFocusActive bool) {
	horizontalBorder := borderWidthHorizontal(window.windowStyle)
	verticalBorder := borderWidthVertical(window.windowStyle)
	cellRects := resolvedRuntimeCellRects(window.layout, contentSize, horizontalBorder, verticalBorder, mode, window)
	frames := resolveLayout(window.layout, cellRects, window)
	for _, item := range window.layout {
		if !styleHasPaint(item.CellStyle) {
			continue
		}
		cellRect := resolvedCellRect(item, cellRects)
		cellRect.Row += contentRowOffset
		cellRect.Col += contentColOffset
		buffer.Fill(cellRect, item.CellStyle)
	}
	focusable := focusableElements(window)
	var focused Element
	if focusedElement != nil {
		focused = focusedElement
	} else if focusedIndex >= 0 && focusedIndex < len(focusable) {
		focused = focusable[focusedIndex]
	}
	if editMode && !explicitEditElement {
		editElement = focused
	}
	var scopeElement Element
	if scopeFocusActive {
		scopeElement = scopeDimElementForTarget(window, focused)
	}
	if suppressScopeFocus {
		scopeElement = nil
	}
	var deferredComboBox Element
	var deferredComboBoxFrame Rect
	var deferredComboBoxClipTop *int
	var deferredComboBoxClipBottom *int
	for _, item := range window.layout {
		frame, ok := frames[item]
		if !ok {
			continue
		}
		cellRect := resolvedCellRect(item, cellRects)
		cellRect.Row += contentRowOffset
		cellRect.Col += contentColOffset
		clip := cellContentClip(item, cellRect)
		frame.Row += contentRowOffset
		frame.Col += contentColOffset
		element := findElement(window, item.Content)
		if element == nil {
			element = findElement(window, item.CellName)
		}
		if element == nil {
			element = findElement(window, itemName(item))
		}
		if element == nil {
			text := staticTextFor(item)
			if text == "" {
				continue
			}
			rendered := RenderPlainText(text, frame.Width, frame.Height, item.ElementStyle)
			buffer.Blit(rendered, frame.Row, frame.Col, clip)
			continue
		}
		baseClipTop := maxInt(0, clip.Row)
		baseClipBottom := maxInt(baseClipTop, minInt(buffer.Height(), clip.Row+clip.Height))
		absoluteClipTop := baseClipTop
		if renderClipTop != nil {
			absoluteClipTop = maxInt(absoluteClipTop, *renderClipTop)
		}
		absoluteClipBottom := baseClipBottom
		if renderClipBottom != nil {
			absoluteClipBottom = minInt(absoluteClipBottom, *renderClipBottom)
		}
		absoluteClipBottom = maxInt(absoluteClipTop, absoluteClipBottom)
		visibleTop := maxInt(frame.Row, absoluteClipTop)
		visibleBottom := minInt(frame.Row+frame.Height, absoluteClipBottom)
		elementClipTopValue := maxInt(0, visibleTop-frame.Row)
		elementClipBottomValue := maxInt(0, visibleBottom-frame.Row)
		var elementClipTop *int
		var elementClipBottom *int
		if elementClipTopValue != 0 || elementClipBottomValue != frame.Height {
			elementClipTop = &elementClipTopValue
			elementClipBottom = &elementClipBottomValue
		}
		element.SetFrame(frame)
		elementFocused := element == focused || (scopeElement != nil && element == scopeElement)
		elementEditMode := element == editElement && editMode
		if elementEditMode {
			if _, ok := element.(*ComboBox); ok {
				deferredComboBox = element
				deferredComboBoxFrame = frame
				deferredComboBoxClipTop = elementClipTop
				deferredComboBoxClipBottom = elementClipBottom
				continue
			}
		}
		parentBackground := Color{}
		parentBackgroundSet := false
		if frame.Row >= 0 && frame.Row < buffer.Height() && frame.Col >= 0 && frame.Col < buffer.Width() {
			parentBackground = buffer.Cell(frame.Row, frame.Col).Background
			parentBackgroundSet = true
		}
		if scrollView, ok := element.(*ScrollView); ok {
			scrollView.visibleSize = Size{
				Width:  maxInt(minimumRenderableSize, minInt(frame.Width, clip.Width)),
				Height: maxInt(minimumRenderableSize, minInt(frame.Height, clip.Height)),
			}
		} else if scrollView := generatedScrollViewForReusableElement(element); scrollView != nil {
			scrollView.visibleSize = Size{
				Width:  maxInt(minimumRenderableSize, minInt(frame.Width, clip.Width)),
				Height: maxInt(minimumRenderableSize, minInt(frame.Height, clip.Height)),
			}
		}
		rendered := withElementParentBackground(parentBackground, parentBackgroundSet, func() [][]TerminalCell {
			childEditMode := false
			if child := childWindowForElement(element); child != nil {
				childEditMode = editMode && elementInWindow(child, focused)
			}
			if scrollView, ok := element.(*ScrollView); ok && scrollViewContainsElement(scrollView, focused) {
				childEditMode = editMode
			}
			suppressScrollIndicators := false
			if useHostViewportForRootScrollViewIndicators && window.scrollViewRoot {
				if scrollView, ok := element.(*ScrollView); ok && firstScrollViewInWindow(window) == scrollView {
					suppressScrollIndicators = true
				}
			}
			return element.Render(
				Size{Width: frame.Width, Height: frame.Height},
				ElementRenderState{Focused: elementFocused, EditMode: elementEditMode, ChildEditMode: childEditMode, SuppressFocusVisuals: suppressScopeFocus, SuppressScrollIndicators: suppressScrollIndicators, FocusedElement: focused, EditElement: editElement, ScopeFocusActive: scopeFocusActive, ClipTop: elementClipTop, ClipBottom: elementClipBottom})
		})
		buffer.Blit(rendered, frame.Row, frame.Col, clip)
		if child := childWindowForElement(element); child != nil {
			offsetWindowElementFrames(child, frame)
		} else if scrollView, ok := element.(*ScrollView); ok {
			syncScrollViewChildFrames(scrollView, frame)
		}
	}
	if deferredComboBox != nil {
		parentBackground := Color{}
		parentBackgroundSet := false
		if deferredComboBoxFrame.Row >= 0 && deferredComboBoxFrame.Row < buffer.Height() &&
			deferredComboBoxFrame.Col >= 0 && deferredComboBoxFrame.Col < buffer.Width() {
			parentBackground = buffer.Cell(deferredComboBoxFrame.Row, deferredComboBoxFrame.Col).Background
			parentBackgroundSet = true
		}
		rendered := withElementParentBackground(parentBackground, parentBackgroundSet, func() [][]TerminalCell {
			return deferredComboBox.Render(
				Size{Width: deferredComboBoxFrame.Width, Height: deferredComboBoxFrame.Height},
				ElementRenderState{Focused: true, EditMode: true, SuppressFocusVisuals: suppressScopeFocus, FocusedElement: focused, EditElement: editElement, ClipTop: deferredComboBoxClipTop, ClipBottom: deferredComboBoxClipBottom})
		})
		buffer.Blit(rendered, deferredComboBoxFrame.Row, deferredComboBoxFrame.Col, Rect{Row: 0, Col: 0, Width: buffer.Width(), Height: buffer.Height()})
	}
}

func RenderGeneratedRuntimeContent(state *runtimeState, size Size) *TerminalBuffer {
	root := state.rootState()
	stackActive := root.options.WindowStack != nil && !root.options.WindowStack.Empty()
	focusedIndex := root.focusedIndex
	focusedElement := root.focusedElement()
	editMode := root.editMode
	var editElement Element
	if editMode {
		editElement = focusedElement
		if root.scopeDimElement != nil {
			editElement = root.scopeEditElement
		}
	}
	if stackActive {
		editMode = false
		editElement = nil
	}
	var buffer *TerminalBuffer
	if stackActive {
		endImageCellBackgroundRendering := beginImageCellBackgroundRendering()
		buffer = renderGeneratedWindowContentWithEditElementClipOptions(root.window, size, focusedIndex, focusedElement, editMode, false, false, editElement, true, nil, nil, false, false)
		endImageCellBackgroundRendering()
	} else {
		buffer = renderGeneratedWindowContentWithEditElementClipOptions(root.window, size, focusedIndex, focusedElement, editMode, false, false, editElement, true, nil, nil, false, root.scopeDimElement != nil)
	}
	if editMode && root.scopeDimElement != nil && !stackActive {
		dimOutsideElement(buffer, root.window, root.scopeDimElement)
	}
	if !stackActive {
		overlayFocusedComboBox(buffer, root.window, focusedElement, editMode && editElement == focusedElement, 0, 0, false)
		root.renderNotification(buffer)
		return buffer
	}
	var frames []*GeneratedWindowStackFrame
	for _, frame := range root.options.WindowStack.frames {
		if frame.runtimeState() != nil {
			frames = append(frames, frame)
		}
	}
	if len(frames) == 0 {
		return buffer
	}
	for _, frame := range frames[:len(frames)-1] {
		renderGeneratedWindowStackFrame(buffer, frame, false)
	}
	buffer.Dim(NewColor("#00000080"))
	topFrame := frames[len(frames)-1]
	renderGeneratedWindowStackFrame(buffer, topFrame, true)
	root.renderNotification(buffer)
	return buffer
}

func (state *runtimeState) showNotification(message string) {
	if state == nil || message == "" {
		return
	}
	root := state.rootState()
	root.notification = message
	root.notificationUntil = time.Now().Add(copyNotificationDuration)
}

func (state *runtimeState) renderNotification(buffer *TerminalBuffer) {
	if state == nil || buffer == nil || state.notification == "" {
		return
	}
	if !state.notificationUntil.IsZero() && !time.Now().Before(state.notificationUntil) {
		state.notification = ""
		state.notificationUntil = time.Time{}
		return
	}
	renderNotification(buffer, state.notification)
}

func renderNotification(buffer *TerminalBuffer, message string) {
	if message == "" || buffer.Width() <= 0 || buffer.Height() <= 0 {
		return
	}
	style := NewStyle()
	style.Color = NewColor(notificationForeground)
	style.Background = NewColor(notificationBackground)
	text := " " + message + " "
	runes := []rune(text)
	width := len(runes)
	col := maxInt(0, buffer.Width()-width-notificationRightMarginCells)
	for index, ch := range runes {
		targetCol := col + index
		if targetCol >= buffer.Width() {
			break
		}
		buffer.SetCell(notificationRow, targetCol, TerminalCell{Text: string(ch), Foreground: style.Color, Background: style.Background})
	}
}

func renderGeneratedWindowStackFrame(buffer *TerminalBuffer, frame *GeneratedWindowStackFrame, topFrame bool) {
	if buffer == nil || frame == nil || frame.Window == nil {
		return
	}
	frameState := frame.runtimeState()
	if frameState == nil {
		return
	}
	bounds := generatedWindowBounds(frame.Window, Size{Width: buffer.Width(), Height: buffer.Height()})
	frameBuffer := NewTerminalBuffer(bounds.Width, bounds.Height)
	localBounds := Rect{Row: 0, Col: 0, Width: bounds.Width, Height: bounds.Height}
	frameBuffer.Fill(localBounds, frame.Window.windowStyle)
	content := generatedWindowContentBounds(localBounds, frame.Window.windowStyle)
	frameBuffer.Fill(content, frame.Window.windowStyle)
	focusedIndex := frameState.focusedIndex
	focusedElement := frameState.focusedElement()
	editMode := false
	var editElement Element
	if topFrame {
		editMode = frameState.editMode
		if editMode {
			editElement = focusedElement
			if frameState.scopeDimElement != nil {
				editElement = frameState.scopeEditElement
			}
		}
	}
	if topFrame {
		renderGeneratedWindowLayout(
			frameBuffer,
			frame.Window,
			Size{Width: content.Width, Height: content.Height},
			content.Row,
			content.Col,
			generatedWindowMode(frame.Window.layout),
			focusedIndex,
			focusedElement,
			editMode,
			false,
			editElement,
			editMode,
			nil,
			nil,
			false,
			frameState.scopeDimElement != nil)
	} else {
		endImageCellBackgroundRendering := beginImageCellBackgroundRendering()
		renderGeneratedWindowLayout(
			frameBuffer,
			frame.Window,
			Size{Width: content.Width, Height: content.Height},
			content.Row,
			content.Col,
			generatedWindowMode(frame.Window.layout),
			focusedIndex,
			focusedElement,
			false,
			false,
			nil,
			false,
			nil,
			nil,
			false,
			false)
		endImageCellBackgroundRendering()
	}
	buffer.Blit(frameBuffer.cells, bounds.Row, bounds.Col, Rect{Row: 0, Col: 0, Width: buffer.Width(), Height: buffer.Height()})
	if topFrame {
		overlayFocusedComboBox(buffer, frame.Window, focusedElement, editMode && editElement == focusedElement, bounds.Row, bounds.Col, false)
	}
}

func overlayFocusedComboBox(buffer *TerminalBuffer, window *GeneratedWindowBase, focusedElement Element, editMode bool, rowOffset int, colOffset int, suppressFocusVisuals bool) {
	if buffer == nil || !editMode {
		return
	}
	comboBox, ok := focusedElement.(*ComboBox)
	if !ok {
		return
	}
	frame := comboBox.ElementFrame()
	parentBackground := Color{}
	parentBackgroundSet := false
	globalRow := rowOffset + frame.Row
	globalCol := colOffset + frame.Col
	if globalRow >= 0 && globalRow < buffer.Height() && globalCol >= 0 && globalCol < buffer.Width() {
		parentBackground = buffer.Cell(globalRow, globalCol).Background
		parentBackgroundSet = true
	}
	rendered := withElementParentBackground(parentBackground, parentBackgroundSet, func() [][]TerminalCell {
		return comboBox.Render(
			Size{Width: frame.Width, Height: maxInt(minimumRenderableSize, len(comboBox.Options)+1)},
			ElementRenderState{Focused: true, EditMode: true, SuppressFocusVisuals: suppressFocusVisuals, FocusedElement: focusedElement, EditElement: focusedElement})
	})
	buffer.Blit(rendered, globalRow, globalCol, Rect{Row: 0, Col: 0, Width: buffer.Width(), Height: buffer.Height()})
}

func dimOutsideElement(buffer *TerminalBuffer, window *GeneratedWindowBase, element Element) {
	if buffer == nil || element == nil {
		return
	}
	style := elementStyleForDim(element)
	dim := style.ScopeDimBackground
	if dim.Empty() || dim.Transparent() {
		return
	}
	frame := element.ElementFrame()
	if absolute, ok := absoluteFrameForElement(window, element); ok {
		frame = absolute
	}
	for row := 0; row < buffer.Height(); row++ {
		for col := 0; col < buffer.Width(); col++ {
			if frame.Contains(Point{Row: row, Col: col}) {
				continue
			}
			cell := buffer.Cell(row, col).Clone()
			if cell.Text == imageFallbackUpperHalfBlock && !cell.Foreground.Empty() {
				cell.Foreground = dim.BlendOver(cell.Foreground)
			}
			if cell.Background.Empty() {
				cell.Background = dim
			} else {
				cell.Background = dim.BlendOver(cell.Background)
			}
			buffer.SetCell(row, col, cell)
		}
	}
}

func elementStyleForDim(element Element) Style {
	if styler, ok := element.(interface {
		EffectiveStyle(bool, bool) Style
	}); ok {
		style := styler.EffectiveStyle(false, false)
		if style.ScopeDimBackground.Empty() {
			style.ScopeDimBackground = NewStyle().ScopeDimBackground
		}
		return style
	}
	return NewStyle()
}

func styleHasPaint(style Style) bool {
	return !style.Background.Empty() ||
		!style.Color.Empty() ||
		style.BackgroundTexture != "" ||
		!style.BackgroundTextureColor.Empty()
}

func borderWidthHorizontal(style Style) int {
	if style.BorderWidthHorizontal == nil {
		return 0
	}
	return maxInt(0, *style.BorderWidthHorizontal)
}

func borderWidthVertical(style Style) int {
	if style.BorderWidthVertical == nil {
		return 0
	}
	return maxInt(0, *style.BorderWidthVertical)
}

func paddingTop(style Style) int {
	if style.PaddingTop != nil {
		return maxInt(0, *style.PaddingTop)
	}
	if style.Padding != nil {
		return maxInt(0, *style.Padding)
	}
	return 0
}

func paddingRight(style Style) int {
	if style.PaddingRight != nil {
		return maxInt(0, *style.PaddingRight)
	}
	if style.Padding != nil {
		return maxInt(0, *style.Padding)
	}
	return 0
}

func paddingBottom(style Style) int {
	if style.PaddingBottom != nil {
		return maxInt(0, *style.PaddingBottom)
	}
	if style.Padding != nil {
		return maxInt(0, *style.Padding)
	}
	return 0
}

func paddingLeft(style Style) int {
	if style.PaddingLeft != nil {
		return maxInt(0, *style.PaddingLeft)
	}
	if style.Padding != nil {
		return maxInt(0, *style.Padding)
	}
	return 0
}

func marginTop(style Style) int {
	if style.MarginTop != nil {
		return maxInt(0, *style.MarginTop)
	}
	if style.Margin != nil {
		return maxInt(0, *style.Margin)
	}
	return 0
}

func marginRight(style Style) int {
	if style.MarginRight != nil {
		return maxInt(0, *style.MarginRight)
	}
	if style.Margin != nil {
		return maxInt(0, *style.Margin)
	}
	return 0
}

func marginBottom(style Style) int {
	if style.MarginBottom != nil {
		return maxInt(0, *style.MarginBottom)
	}
	if style.Margin != nil {
		return maxInt(0, *style.Margin)
	}
	return 0
}

func marginLeft(style Style) int {
	if style.MarginLeft != nil {
		return maxInt(0, *style.MarginLeft)
	}
	if style.Margin != nil {
		return maxInt(0, *style.Margin)
	}
	return 0
}

func generatedWindowBounds(window *GeneratedWindowBase, size Size) Rect {
	if window == nil {
		return Rect{Row: 0, Col: 0, Width: maxInt(minimumRenderableSize, size.Width), Height: maxInt(minimumRenderableSize, size.Height)}
	}
	style := window.windowStyle
	content := GeneratedWindowContentSize(window)
	mode := generatedWindowMode(window.layout)
	naturalWidth := content.Width + borderWidthVertical(style)*2
	naturalHeight := content.Height + borderWidthHorizontal(style)*2
	leftMargin := marginLeft(style)
	rightMargin := marginRight(style)
	topMargin := marginTop(style)
	bottomMargin := marginBottom(style)
	availableWidth := maxInt(minimumRenderableSize, size.Width-leftMargin-rightMargin)
	availableHeight := maxInt(minimumRenderableSize, size.Height-topMargin-bottomMargin)
	width := minInt(naturalWidth, availableWidth)
	height := minInt(naturalHeight, availableHeight)
	if mode == generatedWindowModeFullscreen || mode == generatedWindowModeExpandWidth {
		width = availableWidth
	}
	if mode == generatedWindowModeFullscreen || mode == generatedWindowModeExpandHeight {
		height = availableHeight
	}
	row := topMargin + maxInt(0, (availableHeight-height)/2)
	col := leftMargin + maxInt(0, (availableWidth-width)/2)
	if mode == generatedWindowModeFullscreen || mode == generatedWindowModeExpandHeight {
		row = topMargin
	}
	if mode == generatedWindowModeFullscreen || mode == generatedWindowModeExpandWidth {
		col = leftMargin
	}
	return Rect{Row: row, Col: col, Width: maxInt(minimumRenderableSize, width), Height: maxInt(minimumRenderableSize, height)}
}

func generatedWindowContentBounds(bounds Rect, style Style) Rect {
	horizontal := borderWidthHorizontal(style)
	vertical := borderWidthVertical(style)
	return Rect{
		Row:    bounds.Row + horizontal,
		Col:    bounds.Col + vertical,
		Width:  maxInt(minimumRenderableSize, bounds.Width-vertical*2),
		Height: maxInt(minimumRenderableSize, bounds.Height-horizontal*2),
	}
}

type runtimeCell struct {
	sourceRow     int
	sourceCol     int
	sourceWidth   int
	sourceHeight  int
	logicalWidth  *int
	logicalHeight *int
}

type runtimeNode struct {
	cell          *runtimeCell
	orientation   string
	children      []*runtimeNode
	left          int
	top           int
	right         int
	bottom        int
	logicalWidth  *int
	logicalHeight *int
}

func naturalLayoutSize(layout []LayoutItem, style Style, owners ...*GeneratedWindowBase) Size {
	tree := buildRuntimeTree(collectRuntimeCells(layout, owners...), borderWidthHorizontal(style), borderWidthVertical(style))
	return runtimeTreeLayoutSize(tree)
}

func layoutRootExpansion(layout []LayoutItem, style Style, owners ...*GeneratedWindowBase) (bool, bool) {
	tree := buildRuntimeTree(collectRuntimeCells(layout, owners...), borderWidthHorizontal(style), borderWidthVertical(style))
	if tree == nil {
		return false, false
	}
	return tree.logicalWidth == nil, tree.logicalHeight == nil
}

func generatedWindowMode(layout []LayoutItem) string {
	expandsWidth := false
	expandsHeight := false
	for _, item := range layout {
		expandsWidth = expandsWidth || item.CellWidthMode == "expanded" || item.CellWidth == Expanded
		expandsHeight = expandsHeight || item.CellHeightMode == "expanded" || item.CellHeight == Expanded
	}
	if expandsWidth && expandsHeight {
		return generatedWindowModeFullscreen
	}
	if expandsWidth {
		return generatedWindowModeExpandWidth
	}
	if expandsHeight {
		return generatedWindowModeExpandHeight
	}
	return generatedWindowModeNormal
}

func collectRuntimeCells(layout []LayoutItem, owners ...*GeneratedWindowBase) []runtimeCell {
	var cells []runtimeCell
	seen := map[string]bool{}
	var owner *GeneratedWindowBase
	if len(owners) > 0 {
		owner = owners[0]
	}
	for _, item := range layout {
		key := fmt.Sprintf("%d:%d:%d:%d", item.CellRow, item.CellCol, item.CellCharsWidth, item.CellCharsHeight)
		if seen[key] {
			continue
		}
		seen[key] = true
		cells = append(cells, runtimeCellForItem(item, layout, owner))
	}
	return cells
}

func runtimeCellForItem(item LayoutItem, layout []LayoutItem, owner *GeneratedWindowBase) runtimeCell {
	logicalWidth := logicalCellSize(item.CellWidthMode, item.CellWidth, item.CellCharsWidth)
	logicalHeight := logicalCellSize(item.CellHeightMode, item.CellHeight, item.CellCharsHeight)
	if owner != nil {
		if item.CellWidthMode == "fit-content" || item.CellWidth == FitContent {
			value := fitCellWidth(owner, layout, item)
			logicalWidth = &value
		}
		if item.CellHeightMode == "fit-content" || item.CellHeight == FitContent {
			value := fitCellHeight(owner, layout, item, nil)
			logicalHeight = &value
		}
	}
	if logicalWidth != nil &&
		item.CellWidthMode != "expanded" &&
		item.CellWidth != Expanded &&
		item.CellWidthMode != "fit-content" &&
		item.CellWidth != FitContent {
		value := maxInt(minimumRenderableSize, *logicalWidth+paddingLeft(item.CellStyle)+paddingRight(item.CellStyle))
		logicalWidth = &value
	}
	if logicalHeight != nil &&
		item.CellHeightMode != "expanded" &&
		item.CellHeight != Expanded &&
		item.CellHeightMode != "fit-content" &&
		item.CellHeight != FitContent {
		value := maxInt(minimumRenderableSize, *logicalHeight+paddingTop(item.CellStyle)+paddingBottom(item.CellStyle))
		logicalHeight = &value
	}
	return runtimeCell{
		sourceRow:     item.CellRow,
		sourceCol:     item.CellCol,
		sourceWidth:   maxInt(minimumRenderableSize, item.CellCharsWidth),
		sourceHeight:  maxInt(minimumRenderableSize, item.CellCharsHeight),
		logicalWidth:  logicalWidth,
		logicalHeight: logicalHeight,
	}
}

func fitEntryHeight(window *GeneratedWindowBase, item LayoutItem, availableCellWidth *int) int {
	fallback := item.CharsHeight
	if fallback <= 0 {
		fallback = item.CellCharsHeight
	}
	var contentWidth int
	if availableCellWidth != nil {
		value := maxInt(minimumRenderableSize, *availableCellWidth-item.Col-item.MarginRight)
		contentWidth = value
	}
	contentHeight := maxInt(minimumRenderableSize, fallback)
	if item.HeightMode == "fit-content" || item.Height == FitContent {
		element := layoutElementForItem(window, item)
		width := contentWidth
		if width <= 0 {
			width = item.CharsWidth
			if width <= 0 {
				width = item.CellCharsWidth
			}
		}
		contentHeight = fitContentHeightFor(element, fallback, maxInt(minimumRenderableSize, width))
	}
	return maxInt(minimumRenderableSize, item.Row+contentHeight+item.MarginBottom)
}

func fitCellHeight(window *GeneratedWindowBase, layout []LayoutItem, cellItem LayoutItem, resolvedCellWidth *int) int {
	contentHeight := cellItem.CellCharsHeight
	var availableCellWidth *int
	if resolvedCellWidth != nil {
		value := maxInt(minimumRenderableSize, *resolvedCellWidth-paddingLeft(cellItem.CellStyle)-paddingRight(cellItem.CellStyle))
		availableCellWidth = &value
	}
	for _, item := range layout {
		if sameSourceCell(item, cellItem) {
			contentHeight = maxInt(contentHeight, fitEntryHeight(window, item, availableCellWidth))
		}
	}
	return maxInt(minimumRenderableSize, contentHeight+paddingTop(cellItem.CellStyle)+paddingBottom(cellItem.CellStyle))
}

func fitEntryWidth(window *GeneratedWindowBase, item LayoutItem) int {
	element := layoutElementForItem(window, item)
	fallback := item.CharsWidth
	if fallback <= 0 {
		fallback = item.CellCharsWidth
	}
	contentWidth := maxInt(minimumRenderableSize, fallback)
	switch control := element.(type) {
	case *ListBox:
		for _, option := range control.Options {
			contentWidth = maxInt(contentWidth, len([]rune(option)))
		}
	case *Label:
		lineWidth := 0
		for _, ch := range control.Text {
			if ch == '\n' {
				contentWidth = maxInt(contentWidth, lineWidth)
				lineWidth = 0
			} else if ch != '\r' {
				lineWidth++
			}
		}
		contentWidth = maxInt(contentWidth, lineWidth)
	}
	return maxInt(minimumRenderableSize, item.Col+contentWidth+item.MarginRight)
}

func fitCellWidth(window *GeneratedWindowBase, layout []LayoutItem, cellItem LayoutItem) int {
	contentWidth := cellItem.CellCharsWidth
	for _, item := range layout {
		if sameSourceCell(item, cellItem) {
			contentWidth = maxInt(contentWidth, fitEntryWidth(window, item))
		}
	}
	return maxInt(minimumRenderableSize, contentWidth+paddingLeft(cellItem.CellStyle)+paddingRight(cellItem.CellStyle))
}

func logicalCellSize(mode string, value int, source int) *int {
	if mode == "expanded" || value == Expanded {
		return nil
	}
	size := source
	if value > 0 && value != FitContent {
		size = value
	}
	size = maxInt(minimumRenderableSize, size)
	return &size
}

func buildRuntimeTree(cells []runtimeCell, horizontalSeparator int, verticalSeparator int) *runtimeNode {
	nodes := make([]*runtimeNode, 0, len(cells))
	for _, cell := range cells {
		cellCopy := cell
		nodes = append(nodes, &runtimeNode{
			cell:          &cellCopy,
			left:          cell.sourceCol,
			top:           cell.sourceRow,
			right:         cell.sourceCol + cell.sourceWidth + 1,
			bottom:        cell.sourceRow + cell.sourceHeight + 1,
			logicalWidth:  cell.logicalWidth,
			logicalHeight: cell.logicalHeight,
		})
	}
	return buildRuntimeNodeTree(nodes, horizontalSeparator, verticalSeparator)
}

func buildRuntimeNodeTree(nodes []*runtimeNode, horizontalSeparator int, verticalSeparator int) *runtimeNode {
	if len(nodes) == 0 {
		return nil
	}
	if len(nodes) == 1 {
		return nodes[0]
	}
	sort.Slice(nodes, func(i int, j int) bool {
		if nodes[i].top != nodes[j].top {
			return nodes[i].top < nodes[j].top
		}
		if nodes[i].left != nodes[j].left {
			return nodes[i].left < nodes[j].left
		}
		if nodes[i].bottom != nodes[j].bottom {
			return nodes[i].bottom < nodes[j].bottom
		}
		return nodes[i].right < nodes[j].right
	})
	left, top, right, bottom := nodes[0].left, nodes[0].top, nodes[0].right, nodes[0].bottom
	for _, node := range nodes[1:] {
		left = minInt(left, node.left)
		top = minInt(top, node.top)
		right = maxInt(right, node.right)
		bottom = maxInt(bottom, node.bottom)
	}
	if cuts := horizontalCuts(nodes, left, top, right, bottom); len(cuts) > 0 {
		boundaries := append([]int{top}, cuts...)
		boundaries = append(boundaries, bottom)
		groups := sliceByRows(nodes, boundaries)
		if len(groups) > 1 {
			children := make([]*runtimeNode, 0, len(groups))
			for _, group := range groups {
				children = append(children, buildRuntimeNodeTree(group, horizontalSeparator, verticalSeparator))
			}
			return makeGroupNode("vertical", children, left, top, right, bottom, horizontalSeparator, verticalSeparator)
		}
	}
	if cuts := verticalCuts(nodes, left, top, right, bottom); len(cuts) > 0 {
		boundaries := append([]int{left}, cuts...)
		boundaries = append(boundaries, right)
		groups := sliceByCols(nodes, boundaries)
		if len(groups) > 1 {
			children := make([]*runtimeNode, 0, len(groups))
			for _, group := range groups {
				children = append(children, buildRuntimeNodeTree(group, horizontalSeparator, verticalSeparator))
			}
			return makeGroupNode("horizontal", children, left, top, right, bottom, horizontalSeparator, verticalSeparator)
		}
	}
	return nodes[0]
}

func makeGroupNode(orientation string, children []*runtimeNode, left int, top int, right int, bottom int, horizontalSeparator int, verticalSeparator int) *runtimeNode {
	node := &runtimeNode{orientation: orientation, children: children, left: left, top: top, right: right, bottom: bottom}
	if orientation == "vertical" {
		for _, child := range children {
			if child.logicalWidth == nil {
				node.logicalWidth = nil
				break
			}
			if node.logicalWidth == nil || *child.logicalWidth > *node.logicalWidth {
				value := *child.logicalWidth
				node.logicalWidth = &value
			}
		}
		height := 0
		for _, child := range children {
			if child.logicalHeight == nil {
				node.logicalHeight = nil
				return node
			}
			height += *child.logicalHeight
		}
		height += maxInt(0, len(children)-1) * horizontalSeparator
		node.logicalHeight = &height
		return node
	}
	for _, child := range children {
		if child.logicalHeight == nil {
			node.logicalHeight = nil
			break
		}
		if node.logicalHeight == nil || *child.logicalHeight > *node.logicalHeight {
			value := *child.logicalHeight
			node.logicalHeight = &value
		}
	}
	width := 0
	for _, child := range children {
		if child.logicalWidth == nil {
			node.logicalWidth = nil
			return node
		}
		width += *child.logicalWidth
	}
	width += maxInt(0, len(children)-1) * verticalSeparator
	node.logicalWidth = &width
	return node
}

func horizontalCuts(nodes []*runtimeNode, left int, top int, right int, bottom int) []int {
	candidateSet := map[int]bool{}
	for _, node := range nodes {
		candidateSet[node.top] = true
		candidateSet[node.bottom] = true
	}
	var cuts []int
	for _, candidate := range sortedIntKeys(candidateSet) {
		if candidate <= top || candidate >= bottom {
			continue
		}
		var spans [][2]int
		for _, node := range nodes {
			if node.top == candidate || node.bottom == candidate {
				spans = append(spans, [2]int{node.left, node.right})
			}
		}
		if intervalsCover(spans, left, right) {
			cuts = append(cuts, candidate)
		}
	}
	return cuts
}

func verticalCuts(nodes []*runtimeNode, left int, top int, right int, bottom int) []int {
	candidateSet := map[int]bool{}
	for _, node := range nodes {
		candidateSet[node.left] = true
		candidateSet[node.right] = true
	}
	var cuts []int
	for _, candidate := range sortedIntKeys(candidateSet) {
		if candidate <= left || candidate >= right {
			continue
		}
		var spans [][2]int
		for _, node := range nodes {
			if node.left == candidate || node.right == candidate {
				spans = append(spans, [2]int{node.top, node.bottom})
			}
		}
		if intervalsCover(spans, top, bottom) {
			cuts = append(cuts, candidate)
		}
	}
	return cuts
}

func intervalsCover(spans [][2]int, start int, end int) bool {
	if len(spans) == 0 {
		return false
	}
	sort.Slice(spans, func(i int, j int) bool {
		if spans[i][0] != spans[j][0] {
			return spans[i][0] < spans[j][0]
		}
		return spans[i][1] < spans[j][1]
	})
	cursor := start
	for _, span := range spans {
		if span[0] > cursor {
			return false
		}
		cursor = maxInt(cursor, span[1])
		if cursor >= end {
			return true
		}
	}
	return cursor >= end
}

func sliceByRows(nodes []*runtimeNode, boundaries []int) [][]*runtimeNode {
	groups := make([][]*runtimeNode, 0, maxInt(0, len(boundaries)-1))
	used := make([]bool, len(nodes))
	for index := 0; index+1 < len(boundaries); index++ {
		bandTop := boundaries[index]
		bandBottom := boundaries[index+1]
		var group []*runtimeNode
		for nodeIndex, node := range nodes {
			if node.top >= bandTop && node.bottom <= bandBottom {
				group = append(group, node)
				used[nodeIndex] = true
			}
		}
		if len(group) == 0 {
			return nil
		}
		sort.Slice(group, func(i int, j int) bool {
			if group[i].top != group[j].top {
				return group[i].top < group[j].top
			}
			return group[i].left < group[j].left
		})
		groups = append(groups, group)
	}
	for _, value := range used {
		if !value {
			return nil
		}
	}
	return groups
}

func sliceByCols(nodes []*runtimeNode, boundaries []int) [][]*runtimeNode {
	groups := make([][]*runtimeNode, 0, maxInt(0, len(boundaries)-1))
	used := make([]bool, len(nodes))
	for index := 0; index+1 < len(boundaries); index++ {
		bandLeft := boundaries[index]
		bandRight := boundaries[index+1]
		var group []*runtimeNode
		for nodeIndex, node := range nodes {
			if node.left >= bandLeft && node.right <= bandRight {
				group = append(group, node)
				used[nodeIndex] = true
			}
		}
		if len(group) == 0 {
			return nil
		}
		sort.Slice(group, func(i int, j int) bool {
			if group[i].left != group[j].left {
				return group[i].left < group[j].left
			}
			return group[i].top < group[j].top
		})
		groups = append(groups, group)
	}
	for _, value := range used {
		if !value {
			return nil
		}
	}
	return groups
}

func sortedIntKeys(values map[int]bool) []int {
	keys := make([]int, 0, len(values))
	for value := range values {
		keys = append(keys, value)
	}
	sort.Ints(keys)
	return keys
}

func resolvedRuntimeCellRects(layout []LayoutItem, size Size, horizontalSeparator int, verticalSeparator int, mode string, owners ...*GeneratedWindowBase) map[string]Rect {
	var owner *GeneratedWindowBase
	if len(owners) > 0 {
		owner = owners[0]
	}
	cells := collectRuntimeCells(layout, owner)
	var result map[string]Rect
	for pass := 0; pass < fitContentLayoutMaxPasses; pass++ {
		tree := buildRuntimeTree(cells, horizontalSeparator, verticalSeparator)
		result = map[string]Rect{}
		resolveRuntimeNode(tree, layoutContentRect(tree, Rect{Row: 0, Col: 0, Width: size.Width, Height: size.Height}, mode), result, horizontalSeparator, verticalSeparator, mode)
		if owner == nil || !recomputeFitContentHeightsPass2(owner, cells, result) {
			return result
		}
	}
	return result
}

const fitContentLayoutMaxPasses = 4

func recomputeFitContentHeightsPass2(window *GeneratedWindowBase, cells []runtimeCell, resolvedCells map[string]Rect) bool {
	changed := false
	for index := range cells {
		var cellEntry *LayoutItem
		for itemIndex := range window.layout {
			item := &window.layout[itemIndex]
			if sameRuntimeCellItem(cells[index], *item) &&
				(item.CellHeightMode == "fit-content" || item.CellHeight == FitContent) {
				cellEntry = item
				break
			}
		}
		if cellEntry == nil {
			continue
		}
		resolved, ok := resolvedCells[runtimeCellKey(cells[index])]
		if !ok {
			continue
		}
		width := resolved.Width
		newHeight := fitCellHeight(window, window.layout, *cellEntry, &width)
		if cells[index].logicalHeight == nil || *cells[index].logicalHeight != newHeight {
			cells[index].logicalHeight = &newHeight
			changed = true
		}
	}
	return changed
}

func layoutContentRect(tree *runtimeNode, rect Rect, mode string) Rect {
	natural := runtimeTreeLayoutSize(tree)
	if mode == generatedWindowModeFullscreen || mode == generatedWindowModeExpandWidth {
		rect.Width = maxInt(rect.Width, natural.Width)
	}
	if mode == generatedWindowModeFullscreen || mode == generatedWindowModeExpandHeight {
		rect.Height = maxInt(rect.Height, natural.Height)
	}
	return rect
}

func runtimeTreeLayoutSize(tree *runtimeNode) Size {
	if tree == nil {
		return Size{Width: minimumRenderableSize, Height: minimumRenderableSize}
	}
	width := maxInt(minimumRenderableSize, tree.right-tree.left-1)
	height := maxInt(minimumRenderableSize, tree.bottom-tree.top-1)
	if tree.logicalWidth != nil {
		width = maxInt(minimumRenderableSize, *tree.logicalWidth)
	}
	if tree.logicalHeight != nil {
		height = maxInt(minimumRenderableSize, *tree.logicalHeight)
	}
	return Size{Width: width, Height: height}
}

func resolvedContentExtent(cellRects map[string]Rect, fallback Size) Size {
	result := Size{
		Width:  maxInt(minimumRenderableSize, fallback.Width),
		Height: maxInt(minimumRenderableSize, fallback.Height),
	}
	for _, rect := range cellRects {
		result.Width = maxInt(result.Width, rect.Col+rect.Width)
		result.Height = maxInt(result.Height, rect.Row+rect.Height)
	}
	return result
}

func resolveRuntimeNode(node *runtimeNode, rect Rect, result map[string]Rect, horizontalSeparator int, verticalSeparator int, mode string) {
	if node == nil {
		return
	}
	rect.Width = maxInt(minimumRenderableSize, rect.Width)
	rect.Height = maxInt(minimumRenderableSize, rect.Height)
	if len(node.children) == 0 {
		if node.cell != nil {
			result[runtimeCellKey(*node.cell)] = rect
		}
		return
	}
	if node.orientation == "vertical" {
		fixedTotal := 0
		expandedCount := 0
		for _, child := range node.children {
			if child.logicalHeight != nil {
				fixedTotal += *child.logicalHeight
			} else {
				expandedCount++
			}
		}
		separators := maxInt(0, len(node.children)-1) * horizontalSeparator
		remaining := maxInt(0, rect.Height-fixedTotal-separators)
		distributeFixedExtra := expandedCount == 0 && remaining > 0 && mode == generatedWindowModeFullscreen
		appendTrailingExtra := expandedCount == 0 && remaining > 0 && mode != generatedWindowModeFullscreen && horizontalSeparator > 0
		expandTargets := expandedCount
		if distributeFixedExtra {
			expandTargets = len(node.children)
		}
		share := 0
		remainder := 0
		if expandTargets > 0 {
			share = remaining / expandTargets
			remainder = remaining % expandTargets
		}
		expandIndex := 0
		cursor := rect.Row
		for index, child := range node.children {
			childHeight := 0
			if child.logicalHeight != nil {
				childHeight = *child.logicalHeight
			}
			if child.logicalHeight == nil || distributeFixedExtra {
				if !distributeFixedExtra {
					childHeight = 0
				}
				childHeight += share
				if expandIndex < remainder {
					childHeight++
				}
				expandIndex++
			} else if appendTrailingExtra && index+1 == len(node.children) {
				childHeight += remaining
			}
			resolveRuntimeNode(child, Rect{Row: cursor, Col: rect.Col, Width: rect.Width, Height: maxInt(minimumRenderableSize, childHeight)}, result, horizontalSeparator, verticalSeparator, mode)
			cursor += maxInt(minimumRenderableSize, childHeight) + horizontalSeparator
		}
		return
	}
	fixedTotal := 0
	expandedCount := 0
	for _, child := range node.children {
		if child.logicalWidth != nil {
			fixedTotal += *child.logicalWidth
		} else {
			expandedCount++
		}
	}
	separators := maxInt(0, len(node.children)-1) * verticalSeparator
	remaining := maxInt(0, rect.Width-fixedTotal-separators)
	distributeFixedExtra := expandedCount == 0 && remaining > 0 && mode == generatedWindowModeFullscreen
	appendTrailingExtra := expandedCount == 0 && remaining > 0 && mode != generatedWindowModeFullscreen
	expandTargets := expandedCount
	if distributeFixedExtra {
		expandTargets = len(node.children)
	}
	share := 0
	remainder := 0
	if expandTargets > 0 {
		share = remaining / expandTargets
		remainder = remaining % expandTargets
	}
	expandIndex := 0
	cursor := rect.Col
	for index, child := range node.children {
		childWidth := 0
		if child.logicalWidth != nil {
			childWidth = *child.logicalWidth
		}
		if child.logicalWidth == nil || distributeFixedExtra {
			if !distributeFixedExtra {
				childWidth = 0
			}
			childWidth += share
			if expandIndex < remainder {
				childWidth++
			}
			expandIndex++
		} else if appendTrailingExtra && index+1 == len(node.children) {
			childWidth += remaining
		}
		resolveRuntimeNode(child, Rect{Row: rect.Row, Col: cursor, Width: maxInt(minimumRenderableSize, childWidth), Height: rect.Height}, result, horizontalSeparator, verticalSeparator, mode)
		cursor += maxInt(minimumRenderableSize, childWidth) + verticalSeparator
	}
}

func runtimeCellKey(cell runtimeCell) string {
	return fmt.Sprintf("%d:%d:%d:%d", cell.sourceRow, cell.sourceCol, cell.sourceWidth, cell.sourceHeight)
}

func runtimeCellKeyForItem(item LayoutItem) string {
	return fmt.Sprintf("%d:%d:%d:%d",
		item.CellRow,
		item.CellCol,
		maxInt(minimumRenderableSize, item.CellCharsWidth),
		maxInt(minimumRenderableSize, item.CellCharsHeight))
}

func pointerIntKey(value *int) string {
	if value == nil {
		return "nil"
	}
	return strconv.Itoa(*value)
}

func sameRuntimeCellItem(cell runtimeCell, item LayoutItem) bool {
	return cell.sourceRow == item.CellRow &&
		cell.sourceCol == item.CellCol &&
		cell.sourceWidth == maxInt(minimumRenderableSize, item.CellCharsWidth) &&
		cell.sourceHeight == maxInt(minimumRenderableSize, item.CellCharsHeight)
}

func resolvedCellRect(item LayoutItem, cellRects map[string]Rect) Rect {
	if rect, ok := cellRects[runtimeCellKeyForItem(item)]; ok {
		return rect
	}
	return Rect{Row: item.CellRow, Col: item.CellCol, Width: maxInt(minimumRenderableSize, item.CellCharsWidth), Height: maxInt(minimumRenderableSize, item.CellCharsHeight)}
}

func resolveLayout(layout []LayoutItem, cellRects map[string]Rect, owners ...*GeneratedWindowBase) map[LayoutItem]Rect {
	var owner *GeneratedWindowBase
	if len(owners) > 0 {
		owner = owners[0]
	}
	result := map[LayoutItem]Rect{}
	for _, item := range layout {
		cellRect := resolvedCellRect(item, cellRects)
		var element Element
		if owner != nil {
			element = layoutElementForItem(owner, item)
		}
		size := renderSizeFor(item, cellRect, element)
		frame := Rect{
			Row:    maxInt(0, renderRowFor(layout, item, cellRect)),
			Col:    maxInt(0, renderColFor(layout, item, cellRect, size)),
			Width:  maxInt(minimumRenderableSize, size.Width),
			Height: maxInt(minimumRenderableSize, size.Height),
		}
		result[item] = frame
	}
	return result
}

func renderSizeFor(item LayoutItem, cellRect Rect, elements ...Element) Size {
	availableWidth := cellRect.Width - paddingLeft(item.CellStyle) - paddingRight(item.CellStyle) - item.Col - item.MarginRight
	availableHeight := cellRect.Height - paddingTop(item.CellStyle) - paddingBottom(item.CellStyle) - item.Row - item.MarginBottom
	fallbackWidth := item.CharsWidth
	if fallbackWidth <= 0 {
		fallbackWidth = item.CellCharsWidth
	}
	fallbackHeight := item.CharsHeight
	if fallbackHeight <= 0 {
		fallbackHeight = item.CellCharsHeight
	}
	var element Element
	if len(elements) > 0 {
		element = elements[0]
	}
	if item.HeightMode == "fit-content" || item.Height == FitContent {
		fallbackHeight = fitContentHeightFor(element, fallbackHeight, maxInt(minimumRenderableSize, availableWidth))
	}
	return Size{
		Width:  dimensionWithin(item.WidthMode, item.Width, availableWidth, fallbackWidth),
		Height: dimensionWithin(item.HeightMode, item.Height, availableHeight, fallbackHeight),
	}
}

func fitContentHeightFor(element Element, fallback int, width int) int {
	if element == nil {
		return maxInt(minimumRenderableSize, fallback)
	}
	switch control := element.(type) {
	case *ListBox:
		if len(control.Options) > 0 {
			return maxInt(minimumRenderableSize, len(control.Options))
		}
	case *MessageTable:
		return maxInt(minimumRenderableSize, control.PreferredHeight(width))
	case *InfoLabel:
		text := strings.ReplaceAll(strings.ReplaceAll(control.Text, "\r\n", "\n"), "\r", "\n")
		return maxInt(minimumRenderableSize, strings.Count(text, "\n")+1)
	case *SpanLabel:
		text := strings.ReplaceAll(strings.ReplaceAll(control.Text, "\r\n", "\n"), "\r", "\n")
		return maxInt(minimumRenderableSize, strings.Count(text, "\n")+1)
	case *Label:
		return maxInt(minimumRenderableSize, len(buildWrappedTextRows(control.Text, width)))
	}
	return maxInt(minimumRenderableSize, fallback)
}

func dimensionWithin(mode string, value int, available int, fallback int) int {
	available = maxInt(minimumRenderableSize, available)
	if mode == "expanded" || value == Expanded {
		return available
	}
	if mode == "fit-content" || value == FitContent {
		return maxInt(minimumRenderableSize, minInt(maxInt(minimumRenderableSize, fallback), available))
	}
	requested := fallback
	if value > 0 {
		requested = value
	}
	return maxInt(minimumRenderableSize, requested)
}

func sameSourceCell(first LayoutItem, second LayoutItem) bool {
	return first.CellRow == second.CellRow &&
		first.CellCol == second.CellCol &&
		first.CellCharsWidth == second.CellCharsWidth &&
		first.CellCharsHeight == second.CellCharsHeight
}

func hasExpandedEntryBeforeInSourceRow(layout []LayoutItem, item LayoutItem) bool {
	for _, other := range layout {
		if !sameSourceCell(other, item) {
			continue
		}
		if other.Row != item.Row {
			continue
		}
		if other.Col >= item.Col {
			continue
		}
		if other.WidthMode == "expanded" || other.Width == Expanded {
			return true
		}
	}
	return false
}

func entrySourceColSpan(item LayoutItem) (int, int) {
	width := item.CharsWidth
	if width <= 0 {
		width = item.CellCharsWidth
	}
	width = maxInt(minimumRenderableSize, width)
	return item.Col, item.Col + width
}

func sourceSpansOverlap(firstStart int, firstEnd int, secondStart int, secondEnd int) bool {
	return firstStart < secondEnd && secondStart < firstEnd
}

func hasExpandedEntryBeforeInSourceColumn(layout []LayoutItem, item LayoutItem) bool {
	itemStart, itemEnd := entrySourceColSpan(item)
	for _, other := range layout {
		if !sameSourceCell(other, item) {
			continue
		}
		if other.Row >= item.Row {
			continue
		}
		otherStart, otherEnd := entrySourceColSpan(other)
		if !sourceSpansOverlap(otherStart, otherEnd, itemStart, itemEnd) {
			continue
		}
		if other.HeightMode == "expanded" || other.Height == Expanded {
			return true
		}
	}
	return false
}

func renderColFor(layout []LayoutItem, item LayoutItem, cellRect Rect, size Size) int {
	baseCol := paddingLeft(item.CellStyle) + item.Col
	cellWidthDelta := maxInt(0, cellRect.Width-item.CellCharsWidth)
	followsExpandedEntry := hasExpandedEntryBeforeInSourceRow(layout, item)
	if item.MarginRight == 0 &&
		(item.CellWidthMode == "expanded" || item.CellWidth == Expanded) &&
		item.CellCharsWidth > 0 &&
		cellRect.Width > item.CellCharsWidth &&
		item.WidthMode != "expanded" &&
		item.Width != Expanded {
		return cellRect.Col + maxInt(baseCol, cellRect.Width-size.Width-item.MarginRight-paddingRight(item.CellStyle))
	}
	if followsExpandedEntry &&
		(item.CellWidthMode == "expanded" || item.CellWidth == Expanded) &&
		item.CellCharsWidth > 0 &&
		cellWidthDelta > 0 &&
		item.WidthMode != "expanded" &&
		item.Width != Expanded {
		return cellRect.Col + maxInt(baseCol, baseCol+cellWidthDelta)
	}
	return cellRect.Col + baseCol
}

func renderRowFor(layout []LayoutItem, item LayoutItem, cellRect Rect) int {
	baseRow := paddingTop(item.CellStyle) + item.Row
	cellHeightDelta := maxInt(0, cellRect.Height-item.CellCharsHeight)
	followsExpandedEntry := hasExpandedEntryBeforeInSourceColumn(layout, item)
	if followsExpandedEntry &&
		(item.CellHeightMode == "expanded" || item.CellHeight == Expanded) &&
		item.CellCharsHeight > 0 &&
		cellHeightDelta > 0 &&
		item.HeightMode != "expanded" &&
		item.Height != Expanded {
		return cellRect.Row + maxInt(baseRow, baseRow+cellHeightDelta)
	}
	return cellRect.Row + baseRow
}

func cellContentClip(item LayoutItem, cellRect Rect) Rect {
	top := paddingTop(item.CellStyle)
	right := paddingRight(item.CellStyle)
	bottom := paddingBottom(item.CellStyle)
	left := paddingLeft(item.CellStyle)
	return Rect{
		Row:    cellRect.Row + top,
		Col:    cellRect.Col + left,
		Width:  maxInt(0, cellRect.Width-left-right),
		Height: maxInt(0, cellRect.Height-top-bottom),
	}
}

func scrollViewViewportClipClampedToContent(frame Rect, style Style, contentSize Size) Rect {
	left := paddingLeft(style)
	right := paddingRight(style)
	top := paddingTop(style)
	bottom := paddingBottom(style)
	viewport := Rect{
		Row:    frame.Row + top,
		Col:    frame.Col + left,
		Width:  maxInt(0, frame.Width-left-right),
		Height: maxInt(0, frame.Height-top-bottom),
	}
	viewport.Width = minInt(viewport.Width, maxInt(0, contentSize.Width-viewport.Col))
	viewport.Height = minInt(viewport.Height, maxInt(0, contentSize.Height-viewport.Row))
	return viewport
}

func generatedScrollViewSourceFrame(window *GeneratedWindowBase, scrollView *ScrollView, fallback Size) Rect {
	if window != nil && scrollView != nil {
		for _, item := range window.layout {
			if strings.TrimSpace(item.Content) == scrollView.Name {
				return Rect{
					Row:    item.CellRow,
					Col:    item.CellCol,
					Width:  maxInt(minimumRenderableSize, item.CellCharsWidth),
					Height: maxInt(minimumRenderableSize, item.CellCharsHeight),
				}
			}
		}
	}
	width := maxInt(minimumRenderableSize, fallback.Width)
	height := maxInt(minimumRenderableSize, fallback.Height)
	if window != nil {
		for _, item := range window.layout {
			if item.CellName != generatedScrollViewPanelCellName && item.CellName != generatedScrollViewViewportCellName {
				continue
			}
			width = maxInt(width, item.CellCharsWidth)
			height = maxInt(height, item.CellCharsHeight)
			break
		}
	}
	return Rect{Row: 0, Col: 0, Width: width, Height: height}
}

func applyRootScrollViewHostViewportIndicators(buffer *TerminalBuffer, window *GeneratedWindowBase, contentSize Size) {
	if buffer == nil || window == nil {
		return
	}
	if !window.scrollViewRoot {
		return
	}
	scrollView := firstScrollViewInWindow(window)
	if scrollView == nil {
		return
	}
	scrollFrame := generatedScrollViewSourceFrame(window, scrollView, contentSize)
	scrollViewport := scrollViewViewportClipClampedToContent(scrollFrame, scrollView.style, contentSize)
	if scrollViewport.Width <= 0 || scrollViewport.Height <= 0 {
		return
	}
	childContentAboveViewport := false
	childContentBelowViewport := false
	viewportTop := paddingTop(scrollView.style)
	viewportLeft := paddingLeft(scrollView.style)
	cursor := -scrollView.ViewOffset
	contentHeight := 0
	for index, child := range scrollView.Children {
		if index > 0 {
			cursor += scrollView.Gap
			contentHeight += scrollView.Gap
		}
		childHeight := scrollViewChildHeight(child, scrollViewport.Width)
		childFrame := Rect{
			Row:    scrollFrame.Row + viewportTop + cursor,
			Col:    scrollFrame.Col + viewportLeft,
			Width:  scrollViewport.Width,
			Height: childHeight,
		}
		if childFrame.Row < scrollViewport.Row {
			childContentAboveViewport = true
		}
		if childFrame.Row+childFrame.Height > scrollViewport.Row+scrollViewport.Height {
			childContentBelowViewport = true
		}
		cursor += childHeight
		contentHeight += childHeight
	}
	viewportSize := Size{Width: scrollViewport.Width, Height: scrollViewport.Height}
	viewportHeight := maxInt(minimumRenderableSize, viewportSize.Height)
	maxViewOffset := maxInt(0, contentHeight-viewportHeight)
	indicatorCol := scrollViewport.Col + scrollViewport.Width - minimumRenderableSize
	if scrollView.ViewOffset > 0 || childContentAboveViewport {
		applyScrollIndicatorToBuffer(buffer, scrollViewport.Row, indicatorCol, scrollIndicatorUp, scrollViewport.Col)
	}
	if scrollView.ViewOffset < maxViewOffset || childContentBelowViewport {
		applyScrollIndicatorToBuffer(buffer, scrollViewport.Row+scrollViewport.Height-minimumRenderableSize, indicatorCol, scrollIndicatorDown, scrollViewport.Col)
	}
}

func applyScrollIndicatorToBuffer(buffer *TerminalBuffer, row int, col int, indicator string, foregroundStartCol int) {
	if buffer == nil || row < 0 || row >= buffer.Height() || col < 0 || col >= buffer.Width() {
		return
	}
	cell := buffer.Cell(row, col).Clone()
	cell.Text = indicator
	if cell.Foreground.Empty() || cell.Foreground.Transparent() {
		cell.Foreground = inferredScrollIndicatorForegroundInBuffer(buffer, row, foregroundStartCol, col)
	}
	buffer.SetCell(row, col, cell)
}

func inferredScrollIndicatorForegroundInBuffer(buffer *TerminalBuffer, row int, startCol int, endCol int) Color {
	foreground := Color{}
	if buffer == nil || row < 0 || row >= buffer.Height() {
		return foreground
	}
	start := maxInt(0, startCol)
	end := minInt(buffer.Width()-minimumRenderableSize, endCol)
	for scanCol := start; scanCol <= end; scanCol++ {
		scanned := buffer.Cell(row, scanCol)
		if !scanned.Foreground.Empty() && !scanned.Foreground.Transparent() {
			foreground = scanned.Foreground
		}
	}
	return foreground
}

func adjustedCellCol(item LayoutItem, layout []LayoutItem, separator int) int {
	if separator <= 1 {
		return item.CellCol
	}
	row := item.CellRow
	minCol := item.CellCol
	boundaries := map[int]bool{}
	for _, candidate := range layout {
		top := candidate.CellRow
		bottom := candidate.CellRow + maxInt(minimumRenderableSize, candidate.CellCharsHeight)
		if row < top || row >= bottom {
			continue
		}
		if candidate.CellCol < minCol {
			minCol = candidate.CellCol
		}
		boundaries[candidate.CellCol] = true
	}
	extra := 0
	for _, boundary := range sortedIntKeys(boundaries) {
		if boundary > minCol && boundary <= item.CellCol {
			extra += separator - 1
		}
	}
	return item.CellCol + extra
}

func adjustedCellRow(item LayoutItem, layout []LayoutItem, separator int) int {
	if separator <= 1 {
		return item.CellRow
	}
	col := item.CellCol
	minRow := item.CellRow
	boundaries := map[int]bool{}
	for _, candidate := range layout {
		left := candidate.CellCol
		right := candidate.CellCol + maxInt(minimumRenderableSize, candidate.CellCharsWidth)
		if col < left || col >= right {
			continue
		}
		if candidate.CellRow < minRow {
			minRow = candidate.CellRow
		}
		boundaries[candidate.CellRow] = true
	}
	extra := 0
	for _, boundary := range sortedIntKeys(boundaries) {
		if boundary > minRow && boundary <= item.CellRow {
			extra += separator - 1
		}
	}
	return item.CellRow + extra
}

func dimensionForCell(mode string, value int, chars int, available int) int {
	switch mode {
	case "expanded":
		return maxInt(minimumRenderableSize, available)
	case "fit-content":
		return maxInt(minimumRenderableSize, chars)
	default:
		if value == Expanded {
			return maxInt(minimumRenderableSize, available)
		}
		if value == FitContent {
			return maxInt(minimumRenderableSize, chars)
		}
		if value > 0 {
			return value
		}
		if chars > 0 {
			return chars
		}
		return maxInt(minimumRenderableSize, available)
	}
}

func dimensionForElement(mode string, value int, chars int, available int) int {
	switch mode {
	case "expanded":
		return maxInt(minimumRenderableSize, available)
	case "fit-content":
		return maxInt(minimumRenderableSize, chars)
	default:
		if value == Expanded {
			return maxInt(minimumRenderableSize, available)
		}
		if value == FitContent {
			return maxInt(minimumRenderableSize, chars)
		}
		if value > 0 {
			return value
		}
		if chars > 0 {
			return chars
		}
		return maxInt(minimumRenderableSize, available)
	}
}

func focusableElements(window *GeneratedWindowBase) []Element {
	var result []Element
	appendFocusableElements(&result, window)
	return result
}

func appendFocusableElements(result *[]Element, window *GeneratedWindowBase) {
	if window == nil {
		return
	}
	for _, element := range window.elements {
		if element == nil {
			continue
		}
		if child := childWindowForElement(element); child != nil {
			syncReusableChildFrames(element, element.ElementFrame())
			if generatedScrollViewForReusableElement(element) != nil {
				if element.IsEnabled() && element.IsFocusable() {
					*result = append(*result, element)
				}
				continue
			}
			before := len(*result)
			appendFocusableElements(result, child)
			if len(*result) == before && element.IsEnabled() && element.IsFocusable() && child.focusable {
				if _, isViewHost := element.(*ViewHost); !isViewHost {
					*result = append(*result, element)
				}
			}
			continue
		}
		if isFocusableElement(element) {
			*result = append(*result, element)
		}
	}
}

func isFocusableElement(element Element) bool {
	return element != nil && element.IsEnabled() && element.IsFocusable() && isFocusableElementType(element)
}

func isFocusableElementType(element Element) bool {
	switch element.(type) {
	case *Button, *CheckBox, *TextInput, *TextArea, *NumberInput, *ComboBox, *ListBox, *ScrollView:
		return true
	default:
		return false
	}
}

func syncReusableChildFrames(element Element, frame Rect) {
	child := childWindowForElement(element)
	if child == nil {
		return
	}
	syncWindowElementFramesTo(child, frame, true)
}

func offsetWindowElementFrames(window *GeneratedWindowBase, origin Rect) {
	if window == nil {
		return
	}
	for _, element := range window.elements {
		if element == nil {
			continue
		}
		frame := element.ElementFrame()
		frame.Row += origin.Row
		frame.Col += origin.Col
		element.SetFrame(frame)
		if child := childWindowForElement(element); child != nil {
			offsetWindowElementFrames(child, origin)
			continue
		}
		if scrollView, ok := element.(*ScrollView); ok {
			syncScrollViewChildFrames(scrollView, frame)
		}
	}
}

func syncWindowElementFramesTo(window *GeneratedWindowBase, frame Rect, forceFullscreenLayout bool) {
	if window == nil {
		return
	}
	width := maxInt(minimumRenderableSize, frame.Width)
	height := maxInt(minimumRenderableSize, frame.Height)
	mode := generatedWindowMode(window.layout)
	if forceFullscreenLayout {
		mode = generatedWindowModeFullscreen
	}
	cellRects := resolvedRuntimeCellRects(
		window.layout,
		Size{Width: width, Height: height},
		borderWidthHorizontal(window.windowStyle),
		borderWidthVertical(window.windowStyle),
		mode,
		window)
	frames := resolveLayout(window.layout, cellRects, window)
	for _, item := range window.layout {
		element := layoutElementForItem(window, item)
		if element == nil {
			continue
		}
		elementFrame, ok := frames[item]
		if !ok {
			continue
		}
		elementFrame.Row += frame.Row
		elementFrame.Col += frame.Col
		element.SetFrame(elementFrame)
		if childWindowForElement(element) != nil {
			syncReusableChildFrames(element, elementFrame)
			continue
		}
		if scrollView, ok := element.(*ScrollView); ok {
			syncScrollViewChildFrames(scrollView, elementFrame)
		}
	}
}

func layoutElementForItem(window *GeneratedWindowBase, item LayoutItem) Element {
	if window == nil {
		return nil
	}
	element := findElement(window, item.Content)
	if element == nil {
		element = findElement(window, item.CellName)
	}
	if element == nil {
		element = findElement(window, itemName(item))
	}
	return element
}

func syncScrollViewChildFrames(scrollView *ScrollView, frame Rect) {
	if scrollView == nil {
		return
	}
	cursor := -scrollView.ViewOffset
	for _, child := range scrollView.Children {
		if child == nil {
			continue
		}
		childFrame := child.ElementFrame()
		childHeight := maxInt(minimumRenderableSize, childFrame.Height)
		absolute := Rect{Row: frame.Row + cursor, Col: frame.Col, Width: frame.Width, Height: childHeight}
		child.SetFrame(absolute)
		if childWindowForElement(child) != nil {
			syncReusableChildFrames(child, absolute)
		} else if nested, ok := child.(*ScrollView); ok {
			syncScrollViewChildFrames(nested, absolute)
		}
		cursor += childHeight + scrollView.Gap
	}
}

func generatedScrollViewForReusableElement(element Element) *ScrollView {
	child := childWindowForElement(element)
	if child == nil || !child.scrollViewRoot {
		return nil
	}
	return firstScrollViewInWindow(child)
}

func findElement(window *GeneratedWindowBase, name string) Element {
	name = strings.TrimSpace(name)
	if name == "" || strings.HasPrefix(name, "\"") {
		return nil
	}
	if strings.Contains(name, ".") || strings.Contains(name, "[") {
		return findElementPath(window, name)
	}
	return findDirectElement(window, name)
}

func findDirectElement(window *GeneratedWindowBase, name string) Element {
	for _, element := range window.elements {
		if element.ElementName() == name {
			return element
		}
	}
	return nil
}

func findElementPath(window *GeneratedWindowBase, path string) Element {
	parts := strings.Split(path, ".")
	currentWindow := window
	var current Element
	for index, part := range parts {
		name, childIndex, hasIndex := parseElementPathPart(part)
		if currentWindow == nil {
			return nil
		}
		current = findDirectElement(currentWindow, name)
		if current == nil {
			return nil
		}
		if hasIndex {
			current = indexedReusableChild(current, childIndex)
			if current == nil {
				return nil
			}
		}
		if index == len(parts)-1 {
			return current
		}
		currentWindow = childWindowForElement(current)
	}
	return current
}

func parseElementPathPart(part string) (string, int, bool) {
	start := strings.Index(part, "[")
	end := strings.Index(part, "]")
	if start < 0 || end <= start {
		return part, 0, false
	}
	index, err := strconv.Atoi(part[start+1 : end])
	if err != nil {
		return part[:start], 0, false
	}
	return part[:start], index, true
}

func childWindowForElement(element Element) *GeneratedWindowBase {
	if reusable, ok := element.(*ReusableElement); ok {
		return reusable.Child
	}
	if viewHost, ok := element.(*ViewHost); ok {
		return viewHost.Child
	}
	return nil
}

func indexedReusableChild(element Element, index int) Element {
	if index < 0 {
		return nil
	}
	if scrollView, ok := element.(*ScrollView); ok {
		if index >= len(scrollView.Children) {
			return nil
		}
		return scrollView.Children[index]
	}
	childWindow := childWindowForElement(element)
	if childWindow == nil {
		return nil
	}
	for _, child := range childWindow.elements {
		if scrollView, ok := child.(*ScrollView); ok {
			if index >= len(scrollView.Children) {
				return nil
			}
			return scrollView.Children[index]
		}
	}
	return nil
}

func elementPath(window *GeneratedWindowBase, target Element) string {
	if path, ok := elementPathInWindow(window, target); ok {
		return path
	}
	if target == nil {
		return ""
	}
	return target.ElementName()
}

func elementPathInWindow(window *GeneratedWindowBase, target Element) (string, bool) {
	if window == nil || target == nil {
		return "", false
	}
	for _, element := range window.elements {
		name := element.ElementName()
		if element == target {
			return name, true
		}
		if childWindow := childWindowForElement(element); childWindow != nil {
			if suffix, ok := elementPathInRootScrollChildren(childWindow, target); ok {
				return name + suffix, true
			}
			if childPath, ok := elementPathInWindow(childWindow, target); ok {
				return name + "." + childPath, true
			}
		}
		if scrollView, ok := element.(*ScrollView); ok {
			if suffix, ok := elementPathInScrollView(scrollView, target); ok {
				return name + suffix, true
			}
		}
	}
	return "", false
}

func elementPathInRootScrollChildren(window *GeneratedWindowBase, target Element) (string, bool) {
	if window == nil {
		return "", false
	}
	for _, element := range window.elements {
		if scrollView, ok := element.(*ScrollView); ok {
			return elementPathInScrollView(scrollView, target)
		}
	}
	return "", false
}

func elementPathInScrollView(scrollView *ScrollView, target Element) (string, bool) {
	if scrollView == nil || target == nil {
		return "", false
	}
	for index, child := range scrollView.Children {
		prefix := "[" + strconv.Itoa(index) + "]"
		if child == target {
			return prefix, true
		}
		if childWindow := childWindowForElement(child); childWindow != nil {
			if childPath, ok := elementPathInWindow(childWindow, target); ok {
				return prefix + "." + childPath, true
			}
		}
		if nested, ok := child.(*ScrollView); ok {
			if suffix, ok := elementPathInScrollView(nested, target); ok {
				return prefix + suffix, true
			}
		}
	}
	return "", false
}

func itemName(item LayoutItem) string {
	if item.Content != "" {
		return item.Content
	}
	return item.CellName
}

func staticTextFor(item LayoutItem) string {
	content := strings.TrimSpace(item.Content)
	if content == "" {
		return ""
	}
	if strings.HasPrefix(content, "\"") && strings.HasSuffix(content, "\"") && len(content) >= 2 {
		return strings.Trim(content, "\"")
	}
	if strings.Contains(content, ".") {
		return ""
	}
	return content
}

func valueForElement(element Element) string {
	switch control := element.(type) {
	case *TextInput:
		return control.Value
	case *TextArea:
		return control.Value
	case *NumberInput:
		return control.Value
	case *CheckBox:
		return strconv.FormatBool(control.Checked)
	case *ComboBox:
		return control.Value
	case *ListBox:
		return strings.Join(control.SelectedValues(), "\x00")
	default:
		return ""
	}
}

func runInteractiveTerminal(window *GeneratedWindowBase, options GeneratedWindowRuntimeOptions) int {
	state := newRuntimeState(window, options)
	terminalMode := enterTerminalMode(window.Title())
	defer terminalMode.Close()
	inputReader := directTerminalInputReader{}
	var presented *TerminalBuffer
	renderFrame := func(forceFullRedraw bool) {
		width, height := terminalSize()
		current := RenderGeneratedRuntimeContent(state, Size{Width: width, Height: height})
		if presented == nil {
			presented = NewTerminalBuffer(width, height)
		}
		presented.ReplaceContent(current)
		if forceFullRedraw {
			presented.RequestFullRedraw()
		}
		if output := presented.RenderDiff(); output != "" {
			_, _ = os.Stdout.WriteString(output)
		}
	}
	state.beforeDialogButtonAction = func() {
		renderFrame(false)
		time.Sleep(dialogButtonCloseDelay)
	}
	dirty := true
	forceFullRedraw := false
	previousSize := Size{}
	previousWindowStackSize := 0
	if options.WindowStack != nil {
		previousWindowStackSize = len(options.WindowStack.frames)
	}
	for {
		width, height := terminalSize()
		currentSize := Size{Width: width, Height: height}
		if currentSize != previousSize {
			previousSize = currentSize
			dirty = true
			forceFullRedraw = true
		}
		currentWindowStackSize := 0
		if options.WindowStack != nil {
			currentWindowStackSize = len(options.WindowStack.frames)
		}
		if currentWindowStackSize != previousWindowStackSize {
			previousWindowStackSize = currentWindowStackSize
			dirty = true
			forceFullRedraw = true
		}
		root := state.rootState()
		if root.notification != "" &&
			!root.notificationUntil.IsZero() &&
			!time.Now().Before(root.notificationUntil) {
			dirty = true
		}
		if dirty {
			renderFrame(forceFullRedraw)
			dirty = false
			forceFullRedraw = false
		}
		if window.ShouldClose() || (options.ShouldClose != nil && options.ShouldClose()) {
			break
		}
		inputs := inputReader.Read(os.Stdin, time.Now())
		if len(inputs) == 0 {
			time.Sleep(terminalInputIdleSleep)
			continue
		}
		for _, input := range inputs {
			if input.Empty() {
				continue
			}
			active := activeRuntimeState(state)
			if input.Mouse != nil {
				dirty = active.handleDirectMouse(*input.Mouse, currentSize) || dirty
			} else {
				dirty = active.handleKey(input.Key) || dirty
			}
			if window.ShouldClose() || (options.ShouldClose != nil && options.ShouldClose()) {
				break
			}
		}
	}
	return 0
}

type terminalModeScope struct {
	saved string
}

func enterTerminalMode(title string) terminalModeScope {
	saved := captureTerminalMode()
	runStty(
		"raw",
		"-echo",
		"min",
		terminalRawInputMinBytes,
		"time",
		terminalRawInputTimeoutDeciseconds,
	)
	_, _ = os.Stdout.WriteString("\x1b[?1049h\x1b[?1000h\x1b[?1002h\x1b[?1006h\x1b[?2004h\x1b[?7l\x1b[?25l\x1b[H\x1b[2J")
	_, _ = os.Stdout.WriteString("\x1b]0;" + title + " [go]\x07")
	return terminalModeScope{saved: saved}
}

func (scope terminalModeScope) Close() {
	_, _ = os.Stdout.WriteString("\x1b[0m\x1b[2J\x1b[H\x1b[?25h\x1b[?7h\x1b[?2004l\x1b[?1006l\x1b[?1002l\x1b[?1000l\x1b[?1049l\r\n")
	if scope.saved != "" {
		runStty(scope.saved)
	}
}

func captureTerminalMode() string {
	command := exec.Command("stty", "-g")
	command.Stdin = os.Stdin
	output, err := command.Output()
	if err != nil {
		return ""
	}
	return strings.TrimSpace(string(output))
}

func runStty(args ...string) {
	command := exec.Command("stty", args...)
	command.Stdin = os.Stdin
	_ = command.Run()
}

func terminalSize() (int, int) {
	command := exec.Command("stty", "size")
	command.Stdin = os.Stdin
	output, err := command.Output()
	if err != nil {
		return defaultViewportWidth, defaultViewportHeight
	}
	parts := strings.Fields(string(output))
	if len(parts) != 2 {
		return defaultViewportWidth, defaultViewportHeight
	}
	rows, rowErr := strconv.Atoi(parts[0])
	cols, colErr := strconv.Atoi(parts[1])
	if rowErr != nil || colErr != nil {
		return defaultViewportWidth, defaultViewportHeight
	}
	return maxInt(minimumRenderableSize, cols), maxInt(minimumRenderableSize, rows)
}

func isTerminal(file *os.File) bool {
	info, err := file.Stat()
	if err != nil {
		return false
	}
	return (info.Mode() & os.ModeCharDevice) != 0
}

const (
	terminalInputBufferSize            = 4096
	mouseCoordinateOffset              = 1
	mouseDragFlag                      = 32
	mouseWheelFlag                     = 64
	mouseButtonMask                    = 3
	bracketedPasteStart                = "\x1b[200~"
	bracketedPasteEnd                  = "\x1b[201~"
	copyNotificationText               = "Copied to clipboard"
	copyNotificationDuration           = 3 * time.Second
	textInputWheelScrollRows           = 1
	dialogButtonCloseDelay             = 180 * time.Millisecond
	terminalEscapeSequenceTimeout      = 50 * time.Millisecond
	terminalInputIdleSleep             = 10 * time.Millisecond
	terminalRawInputMinBytes           = "0"
	terminalRawInputTimeoutDeciseconds = "0"
	notificationForeground             = "#ffffff"
	notificationBackground             = "#2255bb"
	notificationPaddingCells           = 2
	notificationRightMarginCells       = 1
	notificationRow                    = 0
)

type terminalInput struct {
	Key   string
	Mouse *directMouseEvent
}

func (input terminalInput) Empty() bool {
	return input.Key == "" && input.Mouse == nil
}

type directMouseEvent struct {
	Name  string
	X     int
	Y     int
	Delta int
}

func (state *runtimeState) handleDirectMouse(event directMouseEvent, size Size) bool {
	if state == nil || state.window == nil {
		return false
	}
	config := &mcpRuntimeConfig{
		ViewportWidth:  maxInt(minimumRenderableSize, size.Width),
		ViewportHeight: maxInt(minimumRenderableSize, size.Height),
	}
	if event.Name == "scroll" {
		renderForMcp(state.window, state, config)
		point := mcpMousePoint(state, config, event.X, event.Y)
		target := mouseTargetAtPoint(state, state.window, point)
		if target == nil {
			return false
		}
		if listBox, ok := target.(*ListBox); ok {
			before := listBox.ScrollOffset
			listBox.ScrollBy(-event.Delta, maxInt(minimumRenderableSize, listBox.ElementFrame().Height))
			return listBox.ScrollOffset != before
		}
		if textInput, ok := asTextInput(target); ok {
			if textInput.ScrollByRows(-event.Delta*textInputWheelScrollRows, textInput.ElementFrame().Height) {
				return true
			}
		}
		scrollView := scrollViewContainingElement(state.window, target)
		if scrollView == nil {
			scrollView = scrollViewForElement(target)
		}
		if scrollView != nil {
			return scrollView.ScrollBy(-event.Delta)
		}
		return false
	}
	selectedTextBeforeRelease := ""
	if event.Name == "mouse_release" {
		selectedTextBeforeRelease = selectedText(state.mousePressElement)
		if selectedTextBeforeRelease == "" {
			selectedTextBeforeRelease = selectedText(state.focusedElement())
		}
	}
	_, err := callMcpTool(state.window, state, event.Name, map[string]any{
		"x": event.X,
		"y": event.Y,
	}, config)
	if event.Name == "mouse_release" && selectedTextBeforeRelease != "" {
		root := state.rootState()
		if root != nil && root.notification == "" {
			runtimeClipboardText = selectedTextBeforeRelease
			state.showNotification(copyNotificationText)
		}
	}
	return err == nil
}

type directTerminalInputReader struct {
	pending      []byte
	pendingSince time.Time
}

func (reader *directTerminalInputReader) Read(input *os.File, now time.Time) []terminalInput {
	if reader == nil || input == nil {
		return nil
	}
	data := make([]byte, 0, terminalInputBufferSize)
	for len(data) < cap(data) {
		buffer := make([]byte, cap(data)-len(data))
		count, err := input.Read(buffer)
		if err != nil || count == 0 {
			break
		}
		data = append(data, buffer[:count]...)
	}
	return reader.Feed(data, now)
}

func (reader *directTerminalInputReader) Feed(data []byte, now time.Time) []terminalInput {
	if reader == nil {
		return nil
	}
	if len(data) > 0 {
		if len(reader.pending) == 0 {
			reader.pendingSince = now
		}
		reader.pending = append(reader.pending, data...)
	}
	if len(reader.pending) == 0 {
		return nil
	}
	if terminalInputBytesNeedMore(reader.pending) &&
		now.Sub(reader.pendingSince) < terminalEscapeSequenceTimeout {
		return nil
	}
	inputs := parseTerminalInputs(string(reader.pending))
	reader.pending = reader.pending[:0]
	reader.pendingSince = time.Time{}
	return inputs
}

func terminalInputBytesNeedMore(data []byte) bool {
	if len(data) == 0 {
		return false
	}
	if start := bytes.LastIndex(data, []byte(bracketedPasteStart)); start >= 0 &&
		bytes.Index(data[start+len(bracketedPasteStart):], []byte(bracketedPasteEnd)) < 0 {
		return true
	}
	escape := bytes.LastIndexByte(data, '\x1b')
	if escape < 0 {
		return !utf8.Valid(data)
	}
	sequence := data[escape:]
	if len(sequence) == 1 {
		return true
	}
	switch sequence[1] {
	case '[':
		if len(sequence) == 2 {
			return true
		}
		for index, value := range sequence[2:] {
			if value >= '@' && value <= '~' {
				return !utf8.Valid(sequence[index+3:])
			}
		}
		return true
	case 'O':
		return len(sequence) < 3 || !utf8.Valid(sequence[3:])
	default:
		return !utf8.Valid(sequence[2:])
	}
}

func parseTerminalInputs(data string) []terminalInput {
	inputs := []terminalInput{}
	for data != "" {
		if strings.HasPrefix(data, bracketedPasteStart) {
			payloadStart := len(bracketedPasteStart)
			payloadEnd := strings.Index(data[payloadStart:], bracketedPasteEnd)
			if payloadEnd >= 0 {
				payload := data[payloadStart : payloadStart+payloadEnd]
				inputs = appendTextInputs(inputs, payload)
				data = data[payloadStart+payloadEnd+len(bracketedPasteEnd):]
				continue
			}
		}
		if mouse, consumed, ok := parseSgrMousePrefix(data); ok {
			event := mouse
			inputs = append(inputs, terminalInput{Mouse: &event})
			data = data[consumed:]
			continue
		}
		if key, consumed := parseKeyPrefix(data); key != "" && consumed > 0 {
			inputs = append(inputs, terminalInput{Key: key})
			data = data[consumed:]
			continue
		}
		_, consumed := utf8.DecodeRuneInString(data)
		if consumed <= 0 {
			break
		}
		data = data[consumed:]
	}
	return inputs
}

func appendTextInputs(inputs []terminalInput, text string) []terminalInput {
	for text != "" {
		key, consumed := parseTextKeyPrefix(text)
		if key != "" && consumed > 0 {
			inputs = append(inputs, terminalInput{Key: key})
			text = text[consumed:]
			continue
		}
		_, consumed = utf8.DecodeRuneInString(text)
		if consumed <= 0 {
			break
		}
		text = text[consumed:]
	}
	return inputs
}

func parseKeyPrefix(data string) (string, int) {
	for _, sequence := range []struct {
		text string
		key  string
	}{
		{text: "\x1b[1;2A", key: "Shift+Up"},
		{text: "\x1b[1;2B", key: "Shift+Down"},
		{text: "\x1b[1;2C", key: "Shift+Right"},
		{text: "\x1b[1;2D", key: "Shift+Left"},
		{text: "\x1b[1~", key: "Home"},
		{text: "\x1b[4~", key: "End"},
		{text: "\x1b[3~", key: "Delete"},
		{text: "\x1b[Z", key: "Shift+Tab"},
		{text: "\x1b[A", key: "Up"},
		{text: "\x1b[B", key: "Down"},
		{text: "\x1b[C", key: "Right"},
		{text: "\x1b[D", key: "Left"},
		{text: "\x1b[H", key: "Home"},
		{text: "\x1b[F", key: "End"},
		{text: "\x1bOA", key: "Up"},
		{text: "\x1bOB", key: "Down"},
		{text: "\x1bOC", key: "Right"},
		{text: "\x1bOD", key: "Left"},
		{text: "\x1b", key: "Escape"},
	} {
		if strings.HasPrefix(data, sequence.text) {
			return sequence.key, len(sequence.text)
		}
	}
	return parseTextKeyPrefix(data)
}

func parseTextKeyPrefix(data string) (string, int) {
	switch {
	case strings.HasPrefix(data, "\x03"):
		return "Ctrl+C", len("\x03")
	case strings.HasPrefix(data, "\r"), strings.HasPrefix(data, "\n"):
		return "Enter", len("\n")
	case strings.HasPrefix(data, "\t"):
		return "Tab", len("\t")
	case strings.HasPrefix(data, "\x7f"), strings.HasPrefix(data, "\b"):
		return "Backspace", len("\x7f")
	}
	runeValue, consumed := utf8.DecodeRuneInString(data)
	if runeValue >= ' ' && consumed > 0 {
		return string(runeValue), consumed
	}
	return "", 0
}

func parseKey(data string) string {
	key, consumed := parseKeyPrefix(data)
	if consumed == len(data) {
		return key
	}
	return ""
}

func parseSgrMouse(data string) (directMouseEvent, bool) {
	event, consumed, ok := parseSgrMousePrefix(data)
	return event, ok && consumed == len(data)
}

func parseSgrMousePrefix(data string) (directMouseEvent, int, bool) {
	if !strings.HasPrefix(data, "\x1b[<") {
		return directMouseEvent{}, 0, false
	}
	terminator := strings.IndexAny(data, "Mm")
	if terminator < 0 {
		return directMouseEvent{}, 0, false
	}
	final := data[terminator]
	body := data[len("\x1b[<"):terminator]
	parts := strings.Split(body, ";")
	if len(parts) != 3 {
		return directMouseEvent{}, 0, false
	}
	button, buttonErr := strconv.Atoi(parts[0])
	col, colErr := strconv.Atoi(parts[1])
	row, rowErr := strconv.Atoi(parts[2])
	if buttonErr != nil || colErr != nil || rowErr != nil {
		return directMouseEvent{}, 0, false
	}
	x := col - mouseCoordinateOffset
	y := row - mouseCoordinateOffset
	consumed := terminator + 1
	if button&mouseWheelFlag != 0 {
		delta := 1
		if button&mouseButtonMask != 0 {
			delta = -1
		}
		return directMouseEvent{Name: "scroll", X: x, Y: y, Delta: delta}, consumed, true
	}
	name := "mouse_press"
	if final == 'm' {
		name = "mouse_release"
	} else if button&mouseDragFlag != 0 {
		name = "mouse_drag"
	}
	return directMouseEvent{Name: name, X: x, Y: y}, consumed, true
}

type mcpRuntimeConfig struct {
	Enabled        bool
	Transport      string
	Host           string
	Port           int
	ViewportRow    int
	ViewportCol    int
	ViewportWidth  int
	ViewportHeight int
}

func parseMcpRuntimeArgs(args []string) mcpRuntimeConfig {
	config := mcpRuntimeConfig{
		Transport:      "tcp",
		Host:           "127.0.0.1",
		ViewportWidth:  90,
		ViewportHeight: 35,
	}
	for index := 0; index < len(args); index++ {
		arg := args[index]
		switch arg {
		case "--mcp", "--mcp-server":
			config.Enabled = true
		case "--headless":
			config.Enabled = true
		case "--mcp-transport":
			if index+1 < len(args) {
				index++
				config.Transport = args[index]
			}
		case "--mcp-host":
			if index+1 < len(args) {
				index++
				config.Host = args[index]
			}
		case "--mcp-port":
			if index+1 < len(args) {
				index++
				config.Port, _ = strconv.Atoi(args[index])
			}
		case "--viewport":
			if index+1 < len(args) {
				index++
				parts := strings.Split(args[index], ",")
				if len(parts) == 4 {
					config.ViewportRow, _ = strconv.Atoi(parts[0])
					config.ViewportCol, _ = strconv.Atoi(parts[1])
					config.ViewportWidth, _ = strconv.Atoi(parts[2])
					config.ViewportHeight, _ = strconv.Atoi(parts[3])
				}
			}
		case "--viewport-row":
			if index+1 < len(args) {
				index++
				config.ViewportRow, _ = strconv.Atoi(args[index])
			}
		case "--viewport-col":
			if index+1 < len(args) {
				index++
				config.ViewportCol, _ = strconv.Atoi(args[index])
			}
		case "--viewport-width":
			if index+1 < len(args) {
				index++
				config.ViewportWidth, _ = strconv.Atoi(args[index])
			}
		case "--viewport-height":
			if index+1 < len(args) {
				index++
				config.ViewportHeight, _ = strconv.Atoi(args[index])
			}
		case "--mcp-viewport":
			if index+1 < len(args) {
				index++
				parts := strings.Split(args[index], "x")
				if len(parts) == 2 {
					config.ViewportWidth, _ = strconv.Atoi(parts[0])
					config.ViewportHeight, _ = strconv.Atoi(parts[1])
				}
			}
		}
	}
	config.ViewportWidth = maxInt(minimumRenderableSize, config.ViewportWidth)
	config.ViewportHeight = maxInt(minimumRenderableSize, config.ViewportHeight)
	return config
}

func serveMcp(window *GeneratedWindowBase, options GeneratedWindowRuntimeOptions, config mcpRuntimeConfig) int {
	state := newRuntimeState(window, options)
	if config.Transport == "stdio" {
		return serveMcpConnection(window, state, bufio.NewReadWriter(bufio.NewReader(os.Stdin), bufio.NewWriter(os.Stdout)), &config)
	}
	listener, err := net.Listen("tcp", net.JoinHostPort(config.Host, strconv.Itoa(config.Port)))
	if err != nil {
		_, _ = fmt.Fprintln(os.Stderr, "error: "+err.Error())
		return 1
	}
	defer listener.Close()
	for {
		connection, err := listener.Accept()
		if err != nil {
			_, _ = fmt.Fprintln(os.Stderr, "error: "+err.Error())
			return 1
		}
		reader := bufio.NewReader(connection)
		writer := bufio.NewWriter(connection)
		serveMcpConnection(window, state, bufio.NewReadWriter(reader, writer), &config)
		_ = connection.Close()
		if window.ShouldClose() || (options.ShouldClose != nil && options.ShouldClose()) {
			return 0
		}
	}
}

func serveMcpConnection(window *GeneratedWindowBase, state *runtimeState, rw *bufio.ReadWriter, config *mcpRuntimeConfig) int {
	for {
		line, err := rw.ReadString('\n')
		if err != nil {
			return 0
		}
		line = strings.TrimSpace(line)
		if line == "" {
			continue
		}
		var request map[string]any
		if err := json.Unmarshal([]byte(line), &request); err != nil {
			writeMcpResponse(rw, nil, nil, map[string]any{"code": -32700, "message": err.Error()})
			continue
		}
		id := request["id"]
		method, _ := request["method"].(string)
		params, _ := request["params"].(map[string]any)
		result, rpcErr := handleMcpRequest(window, state, method, params, config)
		writeMcpResponse(rw, id, result, rpcErr)
		if window.ShouldClose() {
			return 0
		}
	}
}

func handleMcpRequest(window *GeneratedWindowBase, state *runtimeState, method string, params map[string]any, config *mcpRuntimeConfig) (any, any) {
	switch method {
	case "initialize":
		return map[string]any{
			"protocolVersion": "2024-11-05",
			"serverInfo": map[string]any{
				"name":    "uimd-go",
				"version": "0.0.0",
			},
			"capabilities": map[string]any{
				"tools": map[string]any{},
			},
		}, nil
	case "tools/list":
		names := mcpToolNames(state.window)
		tools := make([]map[string]any, 0, len(names))
		for _, name := range names {
			tools = append(tools, map[string]any{
				"name":        name,
				"description": "",
				"inputSchema": map[string]any{"type": "object", "properties": map[string]any{}},
			})
		}
		return map[string]any{"tools": tools}, nil
	case "tools/call":
		name, _ := params["name"].(string)
		arguments, _ := params["arguments"].(map[string]any)
		active := activeRuntimeState(state)
		result, err := callMcpTool(active.window, active, name, arguments, config)
		if err != nil {
			return nil, map[string]any{"code": -32603, "message": err.Error()}
		}
		return toolJSONResult(result), nil
	default:
		active := activeRuntimeState(state)
		result, err := callMcpTool(active.window, active, strings.TrimPrefix(method, "ui."), params, config)
		if err != nil {
			return nil, map[string]any{"code": -32601, "message": err.Error()}
		}
		return result, nil
	}
}

func callMcpTool(window *GeneratedWindowBase, state *runtimeState, name string, arguments map[string]any, config *mcpRuntimeConfig) (any, error) {
	name = resolveMcpToolName(name)
	switch name {
	case "get_window":
		className := window.mcpClassName
		if state.className != "" {
			className = state.className
		}
		return map[string]any{
			"title":       window.Title(),
			"class":       className,
			"backend":     "go",
			"mode":        window.kind,
			"description": window.mcpDescription,
			"width":       config.ViewportWidth,
			"height":      config.ViewportHeight,
			"mcp_enabled": window.mcpEnabled,
		}, nil
	case "get_elements":
		renderForMcp(window, state, config)
		var elements []any
		for _, element := range window.elements {
			elements = append(elements, snapshotElement(element, state))
		}
		return elements, nil
	case "get_element":
		renderForMcp(window, state, config)
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		return snapshotElement(element, state), nil
	case "get_value":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		return map[string]any{"value": elementValue(element)}, nil
	case "get_options":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		options, err := elementOptions(element)
		if err != nil {
			return nil, err
		}
		return map[string]any{"options": options}, nil
	case "get_image_render_info":
		renderForMcp(window, state, config)
		elementID := stringArgument(arguments, "element_id")
		element, err := requireElement(window, elementID)
		if err != nil {
			return nil, err
		}
		imageElement, ok := element.(*Image)
		if !ok {
			return nil, fmt.Errorf("element is not an image: %s", elementID)
		}
		frame := element.ElementFrame()
		if absolute, ok := absoluteFrameForElement(window, element); ok {
			frame = absolute
		}
		focused := state.focusedElement() == element
		info := imageElement.RenderInfo(
			Size{Width: maxInt(minimumRenderableSize, frame.Width), Height: maxInt(minimumRenderableSize, frame.Height)},
			ElementRenderState{Focused: focused, EditMode: focused && state.editMode},
		)
		imageLeft := intMapValue(info, "image_left")
		imageTop := intMapValue(info, "image_top")
		imageRight := intMapValue(info, "image_right")
		imageBottom := intMapValue(info, "image_bottom")
		visibleLeft := intMapValue(info, "visible_left")
		visibleTop := intMapValue(info, "visible_top")
		visibleRight := intMapValue(info, "visible_right")
		visibleBottom := intMapValue(info, "visible_bottom")
		info["id"] = elementID
		info["bounds"] = rectJSON(frame)
		info["absolute_image_left"] = frame.Col + imageLeft
		info["absolute_image_top"] = frame.Row + imageTop
		info["absolute_image_right"] = frame.Col + imageRight
		info["absolute_image_bottom"] = frame.Row + imageBottom
		info["absolute_visible_left"] = frame.Col + visibleLeft
		info["absolute_visible_top"] = frame.Row + visibleTop
		info["absolute_visible_right"] = frame.Col + visibleRight
		info["absolute_visible_bottom"] = frame.Row + visibleBottom
		return info, nil
	case "get_focused_element":
		renderForMcp(window, state, config)
		if focused := state.focusedElement(); focused != nil {
			return snapshotElement(focused, state), nil
		}
		return nil, nil
	case "get_edit_mode":
		return map[string]any{"edit_mode": state.editMode}, nil
	case "get_accessibility_snapshot", "get_state", "get_schema":
		renderForMcp(window, state, config)
		elements, _ := callMcpTool(window, state, "get_elements", nil, config)
		focused, _ := callMcpTool(window, state, "get_focused_element", nil, config)
		windowInfo, _ := callMcpTool(window, state, "get_window", nil, config)
		return map[string]any{
			"window":          windowInfo,
			"focused_element": focused,
			"edit_mode":       state.editMode,
			"elements":        elements,
		}, nil
	case "get_source":
		return map[string]any{
			"class":           window.mcpClassName,
			"mcp_enabled":     window.mcpEnabled,
			"source_markdown": window.mcpSource,
		}, nil
	case "get_viewport", "get_render_rect":
		return map[string]any{
			"row":    config.ViewportRow,
			"col":    config.ViewportCol,
			"width":  config.ViewportWidth,
			"height": config.ViewportHeight,
		}, nil
	case "set_viewport":
		if value, ok := intArgument(arguments, "row"); ok {
			config.ViewportRow = maxInt(0, value)
		}
		if value, ok := intArgument(arguments, "col"); ok {
			config.ViewportCol = maxInt(0, value)
		}
		if value, ok := intArgument(arguments, "width"); ok {
			config.ViewportWidth = maxInt(minimumRenderableSize, value)
		}
		if value, ok := intArgument(arguments, "height"); ok {
			config.ViewportHeight = maxInt(minimumRenderableSize, value)
		}
		return map[string]any{
			"row":    config.ViewportRow,
			"col":    config.ViewportCol,
			"width":  config.ViewportWidth,
			"height": config.ViewportHeight,
		}, nil
	case "get_render_frame":
		buffer := renderForMcpWithArguments(window, state, config, arguments)
		return map[string]any{
			"width":  buffer.Width(),
			"height": buffer.Height(),
			"cells":  compactCells(buffer),
		}, nil
	case "get_render_snapshot":
		buffer := renderForMcpWithArguments(window, state, config, arguments)
		return map[string]any{
			"ansi_lines": []any{},
			"text_lines": strings.Split(buffer.PlainText(), "\n"),
			"cells":      snapshotCells(buffer),
		}, nil
	case "get_render_snapshot_compact":
		buffer := renderForMcpWithArguments(window, state, config, arguments)
		return map[string]any{
			"format": "render-cells-v1",
			"cells":  compactCells(buffer),
		}, nil
	case "get_render_cell":
		buffer := renderForMcpWithArguments(window, state, config, arguments)
		x, _ := intArgument(arguments, "x")
		y, _ := intArgument(arguments, "y")
		cell := buffer.Cell(y, x)
		return renderCellJSON(cell, x, y), nil
	case "get_text_snapshot":
		buffer := renderForMcpWithArguments(window, state, config, arguments)
		return map[string]any{"lines": strings.Split(buffer.PlainText(), "\n")}, nil
	case "repaint":
		renderForMcpWithArguments(window, state, config, arguments)
		return map[string]any{}, nil
	case "focus_element":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		focusElementForMcp(state, window, element, false)
		renderForMcp(window, state, config)
		return snapshotElement(element, state), nil
	case "enter_edit_mode":
		if id := stringArgument(arguments, "element_id"); id != "" {
			element, err := requireElement(window, id)
			if err != nil {
				return nil, err
			}
			if state.editMode && state.scopeDimElement == nil {
				commitRuntimeElementEdit(state.focusedElement())
				state.editSnapshot = nil
				state.scopeEditElement = nil
				state.editMode = false
			}
			focusElementForMcp(state, window, element, false)
		}
		if childWindowForElement(state.focusedElement()) != nil {
			state.enterReusableScrollScope(state.focusedElement())
		}
		if state.scopeDimElement == nil || state.scopeEditElement != nil {
			state.beginElementEdit(state.focusedElement())
		}
		state.editMode = true
		return map[string]any{"edit_mode": state.editMode}, nil
	case "exit_edit_mode":
		if state.editSnapshot != nil {
			state.endElementEdit(state.editSnapshot.element, true)
		}
		state.scopeEditElement = nil
		state.editMode = false
		return map[string]any{"edit_mode": state.editMode}, nil
	case "activate_element":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		if !element.IsEnabled() {
			return snapshotElement(element, state), nil
		}
		stackDepthBefore := windowStackDepth(state)
		focusElementForMcp(state, window, element, true)
		immediateActivation := immediateMcpActivationElement(element) || immediateMcpActivationElement(state.focusedElement())
		state.activateFocused()
		if immediateActivation && windowStackDepth(state) == stackDepthBefore {
			state.editMode = false
			state.scopeEditElement = nil
		}
		renderForMcp(window, state, config)
		return snapshotElement(element, state), nil
	case "click_element":
		elementID := stringArgument(arguments, "element_id")
		element, err := requireElement(window, elementID)
		if err != nil {
			return nil, err
		}
		if !element.IsEnabled() {
			return snapshotElement(element, state), nil
		}
		renderForMcp(window, state, config)
		frame := element.ElementFrame()
		if absolute, ok := absoluteFrameForElement(window, element); ok {
			frame = absolute
		}
		performMcpMousePress(state, window, Point{
			Row: frame.Row + frame.Height/2,
			Col: frame.Col + frame.Width/2,
		})
		active := activeRuntimeState(state)
		if active != nil && active.window != nil {
			renderForMcp(active.window, active, config)
			if refreshed := findElement(active.window, elementID); refreshed != nil {
				return snapshotElement(refreshed, active), nil
			}
		}
		if elementInWindow(window, element) {
			return snapshotElement(element, state), nil
		}
		return map[string]any{"ok": true, "element_id": elementID}, nil
	case "press_key":
		key, _ := arguments["key"].(string)
		if key == "cmd_c" {
			runtimeClipboardText = selectedText(state.focusedElement())
			return map[string]any{"text": runtimeClipboardText}, nil
		}
		if key == "cmd_v" {
			if runtimeClipboardText != "" {
				if input, ok := state.focusedElement().(*TextInput); ok {
					before := valueForElement(input)
					input.InsertText(runtimeClipboardText)
					state.dispatchChangeIfNeeded(input, before)
				}
				if area, ok := state.focusedElement().(*TextArea); ok {
					before := valueForElement(area)
					area.InsertText(runtimeClipboardText)
					state.dispatchChangeIfNeeded(area, before)
				}
			}
			return map[string]any{"ok": true}, nil
		}
		state.handleKey(key)
		return map[string]any{"ok": true}, nil
	case "type_text", "paste_text", "replace_selection":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil && stringArgument(arguments, "element_id") != "" {
			return nil, err
		}
		if element != nil {
			if state.focusedElement() != element {
				focusElementForMcp(state, window, element, false)
			}
			if !state.editMode || (state.scopeDimElement != nil && state.scopeEditElement != element) {
				state.beginElementEdit(element)
				state.editMode = true
				if state.scopeDimElement != nil {
					state.scopeEditElement = element
				}
			}
		} else if focused := state.focusedElement(); focused != nil && !state.editMode {
			state.beginElementEdit(focused)
			state.editMode = true
		}
		text := stringArgument(arguments, "text")
		before := ""
		if focused := state.focusedElement(); focused != nil {
			before = valueForElement(focused)
		}
		insertTextIntoElement(state.focusedElement(), text)
		if focused := state.focusedElement(); focused != nil {
			state.dispatchChangeIfNeeded(focused, before)
			return snapshotElement(focused, state), nil
		}
		return map[string]any{"ok": true}, nil
	case "set_text":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		focusElement(state, element)
		value := arguments["text"]
		if value == nil {
			value = arguments["value"]
		}
		before := valueForElement(element)
		setElementValue(element, value)
		if input, ok := asTextInput(element); ok {
			state.editMode = true
			input.SetCursor(len([]rune(input.Value)))
		} else if numberInput, ok := element.(*NumberInput); ok {
			state.editMode = true
			numberInput.SetEditText(anyToString(value))
		}
		state.dispatchChangeIfNeeded(element, before)
		return snapshotElement(element, state), nil
	case "set_value":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		value := arguments["value"]
		if value == nil {
			value = arguments["text"]
		}
		before := valueForElement(element)
		setElementValue(element, value)
		if input, ok := asTextInput(element); ok {
			input.SetCursor(len([]rune(input.Value)))
		}
		state.dispatchChangeIfNeeded(element, before)
		return snapshotElement(element, state), nil
	case "clear_text":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		focusElement(state, element)
		before := valueForElement(element)
		setElementValue(element, "")
		if input, ok := asTextInput(element); ok {
			state.editMode = true
			input.SetCursor(0)
		} else if numberInput, ok := element.(*NumberInput); ok {
			state.editMode = true
			numberInput.BeginEdit()
		}
		state.dispatchChangeIfNeeded(element, before)
		return snapshotElement(element, state), nil
	case "set_checked", "check", "uncheck":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		checkBox, ok := element.(*CheckBox)
		if !ok {
			return nil, fmt.Errorf("element is not a checkbox: %s", element.ElementName())
		}
		checked := name == "check"
		if name == "uncheck" {
			checked = false
		} else if value, ok := boolArgument(arguments, "checked"); ok {
			checked = value
		}
		before := valueForElement(checkBox)
		checkBox.SetChecked(checked)
		state.dispatchChangeIfNeeded(checkBox, before)
		return snapshotElement(checkBox, state), nil
	case "select_option":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		value := stringArgument(arguments, "value")
		before := valueForElement(element)
		switch control := element.(type) {
		case *ComboBox:
			control.SetSelectedItem(value)
		case *ListBox:
			control.SetSelectedItems([]string{value})
		default:
			return nil, fmt.Errorf("element has no selectable options: %s", element.ElementName())
		}
		state.dispatchChangeIfNeeded(element, before)
		return snapshotElement(element, state), nil
	case "set_selection":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		before := valueForElement(element)
		if listBox, ok := element.(*ListBox); ok {
			listBox.SetSelectedItems(stringSliceArgument(arguments, "values"))
			state.dispatchChangeIfNeeded(listBox, before)
			return snapshotElement(listBox, state), nil
		}
		return nil, fmt.Errorf("element does not support set_selection: %s", element.ElementName())
	case "select_text":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		start, _ := intArgument(arguments, "start")
		end, _ := intArgument(arguments, "end")
		if input, ok := asTextInput(element); ok {
			if state.focusedElement() != element {
				focusElementForMcp(state, window, element, false)
			}
			if !state.editMode {
				state.beginElementEdit(element)
				state.editMode = true
				if state.scopeDimElement != nil {
					state.scopeEditElement = element
				}
			}
			input.SetSelection(start, end)
			selectionStart, selectionEnd := input.selectionRange()
			return map[string]any{
				"start": selectionStart,
				"end":   selectionEnd,
				"text":  input.SelectionText(),
			}, nil
		}
		return nil, fmt.Errorf("element does not support text selection: %s", element.ElementName())
	case "copy_selection":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil && stringArgument(arguments, "element_id") != "" {
			return nil, err
		}
		if element == nil {
			element = state.focusedElement()
		}
		runtimeClipboardText = selectedText(element)
		return map[string]any{"text": runtimeClipboardText}, nil
	case "set_cursor":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		cursor, ok := intArgument(arguments, "offset")
		if !ok {
			cursor, _ = intArgument(arguments, "cursor")
		}
		if input, ok := asTextInput(element); ok {
			input.SetCursor(cursor)
			return snapshotElement(element, state), nil
		}
		return nil, fmt.Errorf("element does not support cursor: %s", element.ElementName())
	case "mouse_click":
		x, _ := intArgument(arguments, "x")
		y, _ := intArgument(arguments, "y")
		renderForMcp(window, state, config)
		point := mcpMousePoint(state, config, x, y)
		performMcpMousePress(state, window, point)
		return performMcpMouseRelease(state, window, config, point), nil
	case "mouse_press":
		x, _ := intArgument(arguments, "x")
		y, _ := intArgument(arguments, "y")
		renderForMcp(window, state, config)
		performMcpMousePress(state, window, mcpMousePoint(state, config, x, y))
		return map[string]any{"ok": true}, nil
	case "mouse_release":
		x, _ := intArgument(arguments, "x")
		y, _ := intArgument(arguments, "y")
		renderForMcp(window, state, config)
		return performMcpMouseRelease(state, window, config, mcpMousePoint(state, config, x, y)), nil
	case "mouse_drag":
		renderForMcp(window, state, config)
		x, hasX := intArgument(arguments, "x")
		y, hasY := intArgument(arguments, "y")
		if !hasX {
			x, hasX = intArgument(arguments, "to_x")
		}
		if !hasY {
			y, hasY = intArgument(arguments, "to_y")
		}
		if fromX, ok := intArgument(arguments, "from_x"); ok {
			if fromY, fromYOk := intArgument(arguments, "from_y"); fromYOk {
				fromPoint := mcpMousePoint(state, config, fromX, fromY)
				state.mousePressElement = mouseTargetAtPoint(state, window, fromPoint)
				if state.mousePressElement != nil {
					beginMouseTextSelection(window, state.mousePressElement, fromPoint)
				}
			}
		}
		if hasX && hasY && state.mousePressElement != nil {
			updateMouseTextSelection(window, state.mousePressElement, mcpMousePoint(state, config, x, y))
		}
		return map[string]any{"ok": true}, nil
	case "mouse_move":
		return map[string]any{"ok": true}, nil
	case "scroll":
		element, err := requireElement(window, stringArgument(arguments, "element_id"))
		if err != nil {
			return nil, err
		}
		delta, _ := intArgument(arguments, "delta")
		steps := absInt(delta)
		scrollDelta := -1
		if delta < 0 {
			scrollDelta = 1
		}
		for index := 0; index < steps; index++ {
			if listBox, ok := element.(*ListBox); ok {
				listBox.ScrollBy(scrollDelta, listBox.ElementFrame().Height)
			} else if scrollView := scrollViewForElement(element); scrollView != nil {
				scrollView.ScrollBy(scrollDelta)
			}
		}
		return map[string]any{"ok": true, "value": elementValueByID(window, stringArgument(arguments, "element_id"))}, nil
	default:
		if window.mcpToolProvider != nil {
			if result, ok := window.mcpToolProvider.HandleMCPTool(name, arguments); ok {
				return result, nil
			}
		}
		return nil, fmt.Errorf("unknown tool: %s", name)
	}
}
func toolJSONResult(value any) map[string]any {
	payload, _ := json.Marshal(value)
	return map[string]any{
		"content": []map[string]any{
			{"type": "text", "text": string(payload)},
		},
	}
}

func mcpToolNames(window *GeneratedWindowBase) []string {
	names := []string{
		"get_window", "get_elements", "get_element", "get_value", "get_options",
		"get_focused_element", "get_edit_mode", "get_accessibility_snapshot",
		"get_render_frame", "get_render_snapshot", "get_render_snapshot_compact",
		"get_render_cell", "get_render_rect", "get_text_snapshot", "get_state",
		"get_schema", "get_source", "get_viewport", "set_viewport", "repaint",
		"focus_element", "enter_edit_mode", "exit_edit_mode", "activate_element",
		"click_element", "mouse_click", "mouse_drag", "mouse_move", "mouse_press",
		"mouse_release", "scroll", "press_key", "type_text", "set_text", "paste_text",
		"clear_text", "set_value", "set_checked", "check", "uncheck", "set_cursor",
		"select_text", "copy_selection", "replace_selection", "select_option", "set_selection",
		"get_image_render_info",
		"ui.snapshot", "ui.render_snapshot", "ui.text_snapshot", "ui.click", "ui.press",
		"ui.fill", "ui.check", "ui.uncheck", "ui.set_selection",
	}
	if window != nil && window.mcpToolProvider != nil {
		names = append(names, window.mcpToolProvider.MCPToolNames()...)
	}
	return names
}

func resolveMcpToolName(name string) string {
	switch name {
	case "ui.snapshot", "snapshot":
		return "get_accessibility_snapshot"
	case "ui.render_snapshot", "render_snapshot":
		return "get_render_snapshot"
	case "ui.text_snapshot", "text_snapshot":
		return "get_text_snapshot"
	case "ui.click", "click":
		return "click_element"
	case "ui.press", "press":
		return "press_key"
	case "ui.fill", "fill":
		return "set_text"
	case "ui.focus", "focus":
		return "focus_element"
	case "ui.activate", "activate":
		return "activate_element"
	case "ui.set_checked":
		return "set_checked"
	case "ui.check":
		return "check"
	case "ui.uncheck":
		return "uncheck"
	case "ui.set_selection":
		return "set_selection"
	case "ui.set_value":
		return "set_value"
	case "ui.get_options":
		return "get_options"
	case "ui.get_value":
		return "get_value"
	case "ui.select_option":
		return "select_option"
	}
	return strings.TrimPrefix(name, "ui.")
}

func renderForMcp(window *GeneratedWindowBase, state *runtimeState, config *mcpRuntimeConfig) *TerminalBuffer {
	_ = window
	return RenderGeneratedRuntimeContent(state, Size{Width: config.ViewportWidth, Height: config.ViewportHeight})
}

func renderForMcpWithArguments(window *GeneratedWindowBase, state *runtimeState, config *mcpRuntimeConfig, arguments map[string]any) *TerminalBuffer {
	value, ok := intArgument(arguments, "snapshot_time_ms")
	if !ok {
		return renderForMcp(window, state, config)
	}
	previous := renderTimeOverrideMs
	override := int64(value)
	renderTimeOverrideMs = &override
	defer func() {
		renderTimeOverrideMs = previous
	}()
	return renderForMcp(window, state, config)
}

func windowStackDepth(state *runtimeState) int {
	if state == nil {
		return 0
	}
	root := state.rootState()
	if root == nil || root.options.WindowStack == nil {
		return 0
	}
	return len(root.options.WindowStack.frames)
}

func immediateMcpActivationElement(element Element) bool {
	if element == nil {
		return false
	}
	switch element.(type) {
	case *Button, *CheckBox, *Image:
		return true
	default:
		elementType := element.ElementType()
		return elementType == "button" || elementType == "checkbox" || elementType == "image"
	}
}

func mcpMousePoint(state *runtimeState, config *mcpRuntimeConfig, x int, y int) Point {
	point := Point{Row: y - config.ViewportRow, Col: x - config.ViewportCol}
	if state == nil {
		return point
	}
	root := state.rootState()
	if root == nil || root.options.WindowStack == nil {
		return point
	}
	frame := root.options.WindowStack.Top()
	if frame == nil || frame.Window == nil || frame.runtimeState() != state {
		return point
	}
	bounds := generatedWindowBounds(frame.Window, Size{Width: config.ViewportWidth, Height: config.ViewportHeight})
	return Point{Row: point.Row - bounds.Row, Col: point.Col - bounds.Col}
}

func performMcpMousePress(state *runtimeState, window *GeneratedWindowBase, point Point) {
	target := mouseTargetAtPoint(state, window, point)
	if handleOpenComboBoxMousePress(state, window, target, point) {
		state.mousePressElement = nil
		return
	}
	state.mousePressElement = target
	if target == nil {
		state.commitEditBeforeFocusChange(nil)
		previous := state.focusedElement()
		state.focusedIndex = -1
		state.focusedOverride = nil
		state.scopeDimElement = nil
		state.scopeEditElement = nil
		state.editMode = false
		state.notifyFocusTransition(previous)
		return
	}
	if target != state.focusedElement() {
		focusElementForMcp(state, window, target, true)
	}
	if _, ok := asTextInput(target); ok {
		state.beginElementEdit(target)
		state.editMode = true
		if state.scopeDimElement != nil {
			state.scopeEditElement = target
		}
		beginMouseTextSelection(window, target, point)
		return
	}
	switch control := target.(type) {
	case *NumberInput:
		state.beginElementEdit(control)
		state.editMode = true
		if state.scopeDimElement != nil {
			state.scopeEditElement = control
		}
		frame, hasFrame := absoluteFrameForElement(window, control)
		if !hasFrame {
			frame = control.ElementFrame()
		}
		if control.NumberValue != 0 {
			control.editCursor = clampInt(point.Col-frame.Col, 0, len([]rune(control.editText)))
			control.replaceOnFirstTextInput = false
		}
		state.mousePressElement = nil
		return
	case *ComboBox:
		state.beginElementEdit(control)
		state.editMode = true
		if state.scopeDimElement != nil {
			state.scopeEditElement = control
		}
		state.mousePressElement = nil
		return
	case *ListBox:
		frame, hasFrame := absoluteFrameForElement(window, control)
		if !hasFrame {
			frame = control.ElementFrame()
		}
		optionIndex := control.ScrollOffset + point.Row - frame.Row
		if optionIndex >= 0 && optionIndex < len(control.Options) {
			before := valueForElement(control)
			if control.Multi {
				control.Selected[optionIndex] = !control.Selected[optionIndex]
			} else {
				control.SetSelectedIndex(optionIndex)
			}
			control.setActiveIndex(optionIndex)
			control.ActiveItemVisible = false
			state.beginElementEdit(control)
			state.editMode = true
			if state.scopeDimElement != nil {
				state.scopeEditElement = control
			}
			state.dispatchChangeIfNeeded(control, before)
		}
		state.mousePressElement = nil
		return
	}
	if activateElementOnMousePress(state, target) {
		state.mousePressElement = nil
	}
}

func performMcpMouseRelease(state *runtimeState, window *GeneratedWindowBase, config *mcpRuntimeConfig, point Point) map[string]any {
	target := mouseTargetAtPoint(state, window, point)
	pressed := state.mousePressElement
	state.mousePressElement = nil
	if selected := selectedText(pressed); selected != "" {
		runtimeClipboardText = selected
		state.showNotification(copyNotificationText)
		return map[string]any{"ok": true}
	}
	if _, ok := asTextInput(pressed); ok {
		return map[string]any{"ok": true}
	}
	if target != nil && pressed == target {
		return handleMcpMouseClick(state, config, point)
	}
	return map[string]any{"ok": true}
}

func handleMcpMouseClick(state *runtimeState, config *mcpRuntimeConfig, point Point) map[string]any {
	_ = config
	element := elementAtPoint(state.window, point)
	image, ok := element.(*Image)
	if !ok {
		return map[string]any{"ok": true}
	}
	focusElementForMcp(state, state.window, image, true)
	state.dispatchButton(image)
	return snapshotElement(image, state)
}

func beginMouseTextSelection(window *GeneratedWindowBase, element Element, point Point) bool {
	input, ok := asTextInput(element)
	if !ok {
		return false
	}
	frame, hasFrame := absoluteFrameForElement(window, element)
	if !hasFrame {
		frame = element.ElementFrame()
	}
	cursor := textInputCursorAtPoint(input, point, frame)
	input.SetCursor(cursor)
	start := cursor
	end := cursor
	input.SelectionStart = &start
	input.SelectionEnd = &end
	return true
}

func updateMouseTextSelection(window *GeneratedWindowBase, element Element, point Point) bool {
	input, ok := asTextInput(element)
	if !ok {
		return false
	}
	frame, hasFrame := absoluteFrameForElement(window, element)
	if !hasFrame {
		frame = element.ElementFrame()
	}
	cursor := textInputCursorAtPoint(input, point, frame)
	if input.SelectionStart == nil {
		start := input.Cursor
		input.SelectionStart = &start
	}
	input.Cursor = cursor
	end := cursor
	input.SelectionEnd = &end
	return true
}

func textInputCursorAtPoint(input *TextInput, point Point, frame Rect) int {
	localCol := clampInt(point.Col-frame.Col, 0, maxInt(0, frame.Width))
	if !input.Multiline {
		return clampInt(localCol+input.colScrollOffset, 0, len([]rune(input.Value)))
	}
	rows := buildWrappedTextRows(input.Value, maxInt(minimumRenderableSize, frame.Width))
	if len(rows) == 0 {
		return 0
	}
	localRow := clampInt(point.Row-frame.Row, 0, maxInt(0, frame.Height-1))
	rowIndex := clampInt(localRow+input.rowScrollOffset, 0, len(rows)-1)
	return clampInt(rawIndexForVisualColumn(rows[rowIndex], localCol), 0, len([]rune(input.Value)))
}

func activateElementOnMousePress(state *runtimeState, element Element) bool {
	if state == nil || element == nil || !element.IsEnabled() {
		return false
	}
	before := valueForElement(element)
	switch control := element.(type) {
	case *Button:
		state.dispatchButton(control)
		return true
	case *CheckBox:
		control.SetChecked(!control.Checked)
		state.dispatchChangeIfNeeded(control, before)
		return true
	case *ReusableElement:
		return activateReusableControl(control)
	default:
		return false
	}
}

func elementAtPoint(window *GeneratedWindowBase, point Point) Element {
	return elementAtPointInWindow(window, point, 0, 0)
}

func activeComboBox(state *runtimeState) *ComboBox {
	if state == nil || !state.editMode {
		return nil
	}
	var dropdownElement Element
	if state.scopeDimElement != nil {
		dropdownElement = state.scopeEditElement
	} else {
		dropdownElement = state.focusedElement()
	}
	comboBox, _ := dropdownElement.(*ComboBox)
	return comboBox
}

func comboBoxMouseFrame(window *GeneratedWindowBase, comboBox *ComboBox) Rect {
	frame, hasFrame := absoluteFrameForElement(window, comboBox)
	if !hasFrame {
		frame = comboBox.ElementFrame()
	}
	frame.Height = maxInt(frame.Height, len(comboBox.Options)+1)
	return frame
}

func mouseTargetAtPoint(state *runtimeState, window *GeneratedWindowBase, point Point) Element {
	if comboBox := activeComboBox(state); comboBox != nil {
		if comboBoxMouseFrame(window, comboBox).Contains(point) {
			return comboBox
		}
	}
	return elementAtPoint(window, point)
}

func handleOpenComboBoxMousePress(state *runtimeState, window *GeneratedWindowBase, target Element, point Point) bool {
	comboBox := activeComboBox(state)
	if comboBox == nil || target != comboBox {
		return false
	}
	frame := comboBoxMouseFrame(window, comboBox)
	localRow := point.Row - frame.Row
	if localRow <= 0 {
		return true
	}
	before := valueForElement(comboBox)
	comboBox.SetSelectedIndex(localRow - 1)
	state.editSnapshot = nil
	state.dispatchConfirm(comboBox)
	if host := state.scopeDimElement; host != nil {
		state.rememberReusableScrollScopeFocus()
		focusElement(state, host)
	} else {
		state.scopeEditElement = nil
		state.editMode = false
	}
	state.dispatchChangeIfNeeded(comboBox, before)
	return true
}

func elementAtPointInWindow(window *GeneratedWindowBase, point Point, rowOffset int, colOffset int) Element {
	if window == nil {
		return nil
	}
	for index := len(window.elements) - 1; index >= 0; index-- {
		element := window.elements[index]
		frame := element.ElementFrame()
		absolute := Rect{Row: frame.Row + rowOffset, Col: frame.Col + colOffset, Width: frame.Width, Height: frame.Height}
		if !absolute.Contains(point) {
			continue
		}
		if scrollView, ok := element.(*ScrollView); ok {
			if child := elementAtPointInScrollView(scrollView, point, absolute); child != nil {
				return child
			}
		}
		if childWindow := childWindowForElement(element); childWindow != nil {
			_ = renderGeneratedWindowContent(childWindow, Size{Width: frame.Width, Height: frame.Height}, -1, nil, false, true)
			if child := elementAtPointInWindow(childWindow, point, absolute.Row, absolute.Col); child != nil {
				return child
			}
		}
		element.SetFrame(absolute)
		return element
	}
	return nil
}

func elementAtPointInScrollView(scrollView *ScrollView, point Point, frame Rect) Element {
	if scrollView == nil {
		return nil
	}
	cursor := -scrollView.ViewOffset
	for _, child := range scrollView.Children {
		childFrame := child.ElementFrame()
		childHeight := maxInt(minimumRenderableSize, childFrame.Height)
		absolute := Rect{Row: frame.Row + cursor, Col: frame.Col, Width: frame.Width, Height: childHeight}
		if absolute.Contains(point) {
			if childScrollView, ok := child.(*ScrollView); ok {
				if nested := elementAtPointInScrollView(childScrollView, point, absolute); nested != nil {
					return nested
				}
			}
			if childWindow := childWindowForElement(child); childWindow != nil {
				_ = renderGeneratedWindowContent(childWindow, Size{Width: absolute.Width, Height: absolute.Height}, -1, nil, false, true)
				if nested := elementAtPointInWindow(childWindow, point, absolute.Row, absolute.Col); nested != nil {
					return nested
				}
			}
			child.SetFrame(absolute)
			return child
		}
		cursor += childHeight + scrollView.Gap
	}
	return nil
}

func absoluteFrameForElement(window *GeneratedWindowBase, target Element) (Rect, bool) {
	if window == nil || target == nil || !elementInWindow(window, target) {
		return Rect{}, false
	}
	return target.ElementFrame(), true
}

func requireElement(window *GeneratedWindowBase, id string) (Element, error) {
	if element := findElement(window, id); element != nil {
		return element, nil
	}
	return nil, fmt.Errorf("unknown element: %s", id)
}

func snapshotElement(element Element, state *runtimeState) map[string]any {
	frame := element.ElementFrame()
	if absolute, ok := absoluteFrameForElement(state.window, element); ok {
		frame = absolute
	}
	path := elementPath(state.window, element)
	rect := map[string]any{
		"top":    frame.Row,
		"left":   frame.Col,
		"width":  frame.Width,
		"height": frame.Height,
	}
	result := map[string]any{
		"id":          path,
		"name":        path,
		"type":        elementType(element),
		"role":        roleForElement(element),
		"description": element.ElementName(),
		"value":       elementValue(element),
		"focused":     state.focusedElement() == element,
		"enabled":     element.IsEnabled(),
		"visible":     true,
		"rect":        rect,
		"bounds": map[string]any{
			"top":    frame.Row,
			"left":   frame.Col,
			"width":  frame.Width,
			"height": frame.Height,
			"right":  frame.Col + frame.Width,
			"bottom": frame.Row + frame.Height,
		},
	}
	if input, ok := asTextInput(element); ok {
		result["cursor"] = input.Cursor
		if input.SelectionStart != nil && input.SelectionEnd != nil {
			result["selection"] = map[string]any{
				"start": *input.SelectionStart,
				"end":   *input.SelectionEnd,
				"text":  input.SelectionText(),
			}
		}
	}
	return result
}

func elementValue(element Element) any {
	switch control := element.(type) {
	case *Label:
		return control.Text
	case *SpanLabel:
		return control.Text
	case *InfoLabel:
		return control.Text
	case *MessageTable:
		return control.Text
	case *Button:
		return control.Title
	case *CheckBox:
		return control.Checked
	case *TextInput:
		return control.Value
	case *TextArea:
		return control.Value
	case *NumberInput:
		return control.NumberValue
	case *ComboBox:
		return control.Value
	case *ListBox:
		return control.SelectedValues()
	case *Image:
		return control.Source
	default:
		return nil
	}
}

func elementValueByID(window *GeneratedWindowBase, id string) any {
	if element := findElement(window, id); element != nil {
		return elementValue(element)
	}
	return nil
}

func elementType(element Element) string {
	return element.ElementType()
}

func roleForElement(element Element) string {
	switch element.(type) {
	case *TextInput, *TextArea, *NumberInput:
		return "input"
	case *Button:
		return "action"
	case *CheckBox:
		return "toggle"
	case *ComboBox, *ListBox:
		return "selection"
	default:
		return "text"
	}
}

func elementOptions(element Element) ([]string, error) {
	switch control := element.(type) {
	case *ComboBox:
		return append([]string{}, control.Options...), nil
	case *ListBox:
		return append([]string{}, control.Options...), nil
	default:
		return nil, fmt.Errorf("element has no selectable options: %s", element.ElementName())
	}
}

func focusElement(state *runtimeState, element Element) {
	previous := state.focusedElement()
	focusable := focusableElements(state.window)
	for index, candidate := range focusable {
		if candidate == element {
			state.focusedIndex = index
			state.focusedOverride = nil
			state.scopeDimElement = nil
			state.scopeEditElement = nil
			state.editMode = false
			state.notifyFocusTransition(previous)
			return
		}
	}
	if element != nil && elementInWindow(state.window, element) {
		state.focusedIndex = -1
		state.focusedOverride = element
		state.scopeDimElement = nil
		state.scopeEditElement = nil
		state.editMode = false
		state.notifyFocusTransition(previous)
	}
}

func focusElementForMcp(state *runtimeState, window *GeneratedWindowBase, element Element, activateScope bool) {
	if state == nil || element == nil {
		return
	}
	state.commitEditBeforeFocusChange(element)
	state.rememberReusableScrollScopeFocus()
	focusElement(state, element)
	ensureElementVisibleInScrollViews(window, element)
	if scope := scopeDimElementForTarget(window, element); scope != nil {
		state.scopeDimElement = scope
		state.scopeEditElement = nil
		if activateScope {
			state.editMode = true
		}
		state.rememberReusableScrollScopeFocus()
		return
	}
	if child := childWindowForElement(element); child != nil && generatedWindowHasScrollView(child) {
		state.scopeDimElement = element
	}
}

func elementInWindow(window *GeneratedWindowBase, target Element) bool {
	if window == nil || target == nil {
		return false
	}
	for _, element := range window.elements {
		if element == target {
			return true
		}
		if scrollView, ok := element.(*ScrollView); ok && scrollViewContainsElement(scrollView, target) {
			return true
		}
		if child := childWindowForElement(element); child != nil && elementInWindow(child, target) {
			return true
		}
	}
	return false
}

func ownerObjectForElement(window *GeneratedWindowBase, target Element) any {
	owner := ownerWindowForElement(window, target)
	if owner == nil {
		return nil
	}
	return ownerObjectForWindow(owner)
}

func ownerObjectForWindow(window *GeneratedWindowBase) any {
	if window == nil {
		return nil
	}
	if window.ownerObject != nil {
		return window.ownerObject
	}
	return window
}

func ownerWindowForElement(window *GeneratedWindowBase, target Element) *GeneratedWindowBase {
	if window == nil || target == nil {
		return nil
	}
	for _, element := range window.elements {
		if element == target {
			return window
		}
		if scrollView, ok := element.(*ScrollView); ok {
			if owner := ownerWindowForScrollViewChild(window, scrollView, target); owner != nil {
				return owner
			}
		}
		if child := childWindowForElement(element); child != nil {
			if owner := ownerWindowForElement(child, target); owner != nil {
				return owner
			}
		}
	}
	return nil
}

func ownerWindowForScrollViewChild(parent *GeneratedWindowBase, scrollView *ScrollView, target Element) *GeneratedWindowBase {
	if scrollView == nil || target == nil {
		return nil
	}
	for _, child := range scrollView.Children {
		if child == target {
			return parent
		}
		if nested, ok := child.(*ScrollView); ok {
			if owner := ownerWindowForScrollViewChild(parent, nested, target); owner != nil {
				return owner
			}
		}
		if childWindow := childWindowForElement(child); childWindow != nil {
			if owner := ownerWindowForElement(childWindow, target); owner != nil {
				return owner
			}
		}
	}
	return nil
}

func generatedWindowHasScrollView(window *GeneratedWindowBase) bool {
	if window == nil {
		return false
	}
	if window.scrollViewRoot {
		return true
	}
	for _, element := range window.elements {
		if _, ok := element.(*ScrollView); ok {
			return true
		}
		if child := childWindowForElement(element); child != nil && generatedWindowHasScrollView(child) {
			return true
		}
	}
	return false
}

func scrollViewForElement(element Element) *ScrollView {
	if scrollView, ok := element.(*ScrollView); ok {
		return scrollView
	}
	if child := childWindowForElement(element); child != nil {
		return firstScrollViewInWindow(child)
	}
	return nil
}

func scrollViewContainingElement(window *GeneratedWindowBase, target Element) *ScrollView {
	if window == nil || target == nil {
		return nil
	}
	for _, element := range window.elements {
		if child := childWindowForElement(element); child != nil {
			if scrollView := scrollViewContainingElement(child, target); scrollView != nil {
				return scrollView
			}
		}
		scrollView, ok := element.(*ScrollView)
		if !ok {
			continue
		}
		for _, child := range scrollView.Children {
			if child == nil {
				continue
			}
			if child == target {
				return scrollView
			}
			if childWindow := childWindowForElement(child); childWindow != nil && elementInWindow(childWindow, target) {
				return scrollView
			}
			if nested, ok := child.(*ScrollView); ok && scrollViewContainsElement(nested, target) {
				return nested
			}
		}
	}
	return nil
}

func firstScrollViewInWindow(window *GeneratedWindowBase) *ScrollView {
	if window == nil {
		return nil
	}
	for _, element := range window.elements {
		if scrollView, ok := element.(*ScrollView); ok {
			return scrollView
		}
		if child := childWindowForElement(element); child != nil {
			if scrollView := firstScrollViewInWindow(child); scrollView != nil {
				return scrollView
			}
		}
	}
	return nil
}

func firstFocusableDescendantInWindow(window *GeneratedWindowBase) Element {
	focusable := focusableDescendantsInWindow(window)
	if len(focusable) == 0 {
		return nil
	}
	return focusable[0]
}

func focusableDescendantsInWindow(window *GeneratedWindowBase) []Element {
	var result []Element
	if window == nil {
		return result
	}
	for _, element := range window.elements {
		appendFocusableDescendants(&result, element)
	}
	return result
}

func appendFocusableDescendants(result *[]Element, element Element) {
	if child := childWindowForElement(element); child != nil {
		before := len(*result)
		*result = append(*result, focusableDescendantsInWindow(child)...)
		if len(*result) == before && element.IsEnabled() && element.IsFocusable() && child.focusable {
			if _, isViewHost := element.(*ViewHost); !isViewHost {
				*result = append(*result, element)
			}
		}
		return
	}
	switch control := element.(type) {
	case *ScrollView:
		before := len(*result)
		for _, child := range control.Children {
			appendFocusableDescendants(result, child)
		}
		if len(*result) == before && isFocusableElement(element) {
			*result = append(*result, element)
		}
		return
	}
	if isFocusableElement(element) {
		*result = append(*result, element)
	}
}

func scrollViewContainsElement(scrollView *ScrollView, target Element) bool {
	if scrollView == nil || target == nil {
		return false
	}
	for _, child := range scrollView.Children {
		if child == target {
			return true
		}
		if childWindow := childWindowForElement(child); childWindow != nil && elementInWindow(childWindow, target) {
			return true
		}
		if nested, ok := child.(*ScrollView); ok && scrollViewContainsElement(nested, target) {
			return true
		}
	}
	return false
}

func scopeDimElementForTarget(window *GeneratedWindowBase, target Element) Element {
	if window == nil || target == nil {
		return nil
	}
	for _, element := range window.elements {
		if element == target {
			return nil
		}
		if scrollView, ok := element.(*ScrollView); ok && scrollViewContainsElement(scrollView, target) {
			return element
		}
		if child := childWindowForElement(element); child != nil && elementInWindow(child, target) {
			nested := scopeDimElementForTarget(child, target)
			if nested == nil {
				return nil
			}
			if _, isViewHost := element.(*ViewHost); isViewHost {
				return nested
			}
			return element
		}
	}
	return nil
}

func asTextInput(element Element) (*TextInput, bool) {
	switch control := element.(type) {
	case *TextInput:
		return control, true
	case *TextArea:
		return &control.TextInput, true
	default:
		return nil, false
	}
}

func insertTextIntoElement(element Element, text string) {
	switch control := element.(type) {
	case *TextInput:
		control.InsertText(text)
	case *TextArea:
		control.InsertText(text)
	case *NumberInput:
		for _, ch := range text {
			control.HandleKey(string(ch))
		}
	}
}

func setElementValue(element Element, value any) {
	switch control := element.(type) {
	case *TextInput:
		control.SetValue(anyToString(value))
	case *TextArea:
		control.SetValue(anyToString(value))
	case *NumberInput:
		if number, err := strconv.ParseFloat(anyToString(value), 64); err == nil {
			control.SetValue(number)
		}
	case *CheckBox:
		control.SetChecked(anyToBool(value))
	case *ComboBox:
		control.SetSelectedItem(anyToString(value))
	case *ListBox:
		control.SetSelectedItems(anyToStringSlice(value))
	case *Label:
		control.SetText(anyToString(value))
	case *Button:
		control.SetTitle(anyToString(value))
	}
}

func selectedText(element Element) string {
	if input, ok := asTextInput(element); ok {
		return input.SelectionText()
	}
	return ""
}

func compactCells(buffer *TerminalBuffer) []any {
	rows := make([]any, 0, buffer.Height())
	for row := 0; row < buffer.Height(); row++ {
		cells := make([]any, 0, buffer.Width())
		for col := 0; col < buffer.Width(); col++ {
			cell := buffer.Cell(row, col)
			cells = append(cells, []any{cellText(cell), cellColor(cell.Foreground), cellColor(cell.Background)})
		}
		rows = append(rows, cells)
	}
	return rows
}

func snapshotCells(buffer *TerminalBuffer) []any {
	rows := make([]any, 0, buffer.Height())
	for row := 0; row < buffer.Height(); row++ {
		cells := make([]any, 0, buffer.Width())
		for col := 0; col < buffer.Width(); col++ {
			cells = append(cells, renderCellSnapshotJSON(buffer.Cell(row, col)))
		}
		rows = append(rows, cells)
	}
	return rows
}

func renderCellJSON(cell TerminalCell, x int, y int) map[string]any {
	result := renderCellSnapshotJSON(cell)
	result["x"] = x
	result["y"] = y
	return result
}

func renderCellSnapshotJSON(cell TerminalCell) map[string]any {
	return map[string]any{
		"char":       cellText(cell),
		"foreground": cellColor(cell.Foreground),
		"background": cellColor(cell.Background),
		"attributes": []any{},
	}
}

func cellText(cell TerminalCell) string {
	text := cell.Text
	if text == "" {
		text = " "
	}
	return text
}

func cellColor(color Color) any {
	var foreground any
	if !color.Empty() && !color.Transparent() {
		foreground = color.String()
	}
	return foreground
}

func rectJSON(rect Rect) map[string]any {
	return map[string]any{
		"top":    rect.Row,
		"left":   rect.Col,
		"bottom": rect.Row + rect.Height,
		"right":  rect.Col + rect.Width,
		"width":  rect.Width,
		"height": rect.Height,
	}
}

func intMapValue(values map[string]any, key string) int {
	switch value := values[key].(type) {
	case int:
		return value
	case int64:
		return int(value)
	case float64:
		return int(value)
	case string:
		parsed, err := strconv.Atoi(value)
		if err == nil {
			return parsed
		}
	}
	return 0
}

func stringArgument(arguments map[string]any, key string) string {
	if arguments == nil {
		return ""
	}
	return anyToString(arguments[key])
}

func intArgument(arguments map[string]any, key string) (int, bool) {
	if arguments == nil {
		return 0, false
	}
	switch value := arguments[key].(type) {
	case float64:
		return int(value), true
	case int:
		return value, true
	case string:
		parsed, err := strconv.Atoi(value)
		return parsed, err == nil
	default:
		return 0, false
	}
}

func boolArgument(arguments map[string]any, key string) (bool, bool) {
	if arguments == nil {
		return false, false
	}
	switch value := arguments[key].(type) {
	case bool:
		return value, true
	case string:
		parsed, err := strconv.ParseBool(value)
		return parsed, err == nil
	default:
		return false, false
	}
}

func stringSliceArgument(arguments map[string]any, key string) []string {
	if arguments == nil {
		return nil
	}
	return anyToStringSlice(arguments[key])
}

func anyToString(value any) string {
	switch typed := value.(type) {
	case nil:
		return ""
	case string:
		return typed
	case fmt.Stringer:
		return typed.String()
	case float64:
		return strconv.FormatFloat(typed, 'f', -1, 64)
	case bool:
		return strconv.FormatBool(typed)
	default:
		return fmt.Sprint(typed)
	}
}

func anyToBool(value any) bool {
	switch typed := value.(type) {
	case bool:
		return typed
	case string:
		parsed, _ := strconv.ParseBool(typed)
		return parsed
	case float64:
		return typed != 0
	default:
		return false
	}
}

func anyToStringSlice(value any) []string {
	switch typed := value.(type) {
	case []string:
		return append([]string{}, typed...)
	case []any:
		result := make([]string, 0, len(typed))
		for _, item := range typed {
			result = append(result, anyToString(item))
		}
		return result
	case string:
		if typed == "" {
			return nil
		}
		return []string{typed}
	default:
		return nil
	}
}

func writeMcpResponse(rw *bufio.ReadWriter, id any, result any, rpcErr any) {
	response := map[string]any{"jsonrpc": "2.0"}
	if id != nil {
		response["id"] = id
	}
	if rpcErr != nil {
		response["error"] = rpcErr
	} else {
		response["result"] = result
	}
	payload, _ := json.Marshal(response)
	_, _ = rw.Write(append(payload, '\n'))
	_ = rw.Flush()
}

func absInt(value int) int {
	if value < 0 {
		return -value
	}
	return value
}
