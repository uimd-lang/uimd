package uimd

import (
	"fmt"
	"strings"
	"testing"
)

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
