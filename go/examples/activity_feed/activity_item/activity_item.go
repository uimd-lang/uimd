package activity_item

import (
	"strings"

	uimd "uimd"
)

const (
	activityItemMinimumExtent = 1
	ActivityItemDefaultHeight = activityItemMinimumExtent
)

type ActivityItemData struct {
	Timestamp     string
	EventType     string
	Message       string
	ShowTimestamp bool
}

type ActivityItemElement struct {
	name  string
	frame uimd.Rect
	data  ActivityItemData
}

func NewActivityItemElement(name string, data ActivityItemData) *ActivityItemElement {
	return &ActivityItemElement{
		name:  name,
		frame: uimd.Rect{Width: 1, Height: ActivityItemDefaultHeight},
		data:  data,
	}
}

func (element *ActivityItemElement) ElementName() string {
	return element.name
}

func (element *ActivityItemElement) ElementType() string {
	return "activity_item"
}

func (element *ActivityItemElement) IsFocusable() bool {
	return false
}

func (element *ActivityItemElement) IsEnabled() bool {
	return true
}

func (element *ActivityItemElement) SetFrame(frame uimd.Rect) {
	element.frame = frame
}

func (element *ActivityItemElement) ElementFrame() uimd.Rect {
	return element.frame
}

func (element *ActivityItemElement) HandleKey(string) bool {
	return false
}

func (element *ActivityItemElement) SetText(string) {
}

func (element *ActivityItemElement) Render(size uimd.Size, state uimd.ElementRenderState) [][]uimd.TerminalCell {
	view := NewActivityItemUI()
	configureActivityItemView(view, element.data)
	renderHeight := element.PreferredHeight(size.Width)
	renderSize := uimd.Size{Width: maxPositive(size.Width), Height: renderHeight}
	buffer := uimd.RenderGeneratedWindowContent(view.GeneratedWindowBase, renderSize, -1, nil, false)
	rows := bufferCells(buffer, renderSize)
	color := uimd.NewColor(markerColor(element.data.EventType))
	for row := range rows {
		if len(rows[row]) == 0 {
			continue
		}
		rows[row][0].Text = "▐"
		rows[row][0].Foreground = color
	}
	return rows
}

func (element *ActivityItemElement) PreferredHeight(width int) int {
	view := NewActivityItemUI()
	configureActivityItemView(view, element.data)
	sourceHeight := generatedSourceHeight(view.GeneratedWindowBase)
	_ = uimd.RenderGeneratedWindowContent(
		view.GeneratedWindowBase,
		uimd.Size{Width: maxPositive(width), Height: sourceHeight},
		-1,
		nil,
		false)
	messageRows := WrappedLineCount(element.data.Message, maxPositive(view.Message.ElementFrame().Width))
	return compactContentHeight(view.GeneratedWindowBase, messageRows)
}

func RenderActivityItem(data ActivityItemData, width int) *ActivityItemElement {
	element := NewActivityItemElement("activity_item", data)
	element.SetFrame(uimd.Rect{Width: width, Height: element.PreferredHeight(width)})
	return element
}

func configureActivityItemView(view *ActivityItemUI, data ActivityItemData) {
	if data.ShowTimestamp {
		view.Timestamp.SetText(data.Timestamp)
	} else {
		view.Timestamp.SetText("")
	}
	view.EventType.SetText(data.EventType)
	view.Message.SetText(data.Message)
}

func generatedSourceHeight(window *uimd.GeneratedWindowBase) int {
	height := activityItemMinimumExtent
	for _, entry := range window.GeneratedLayout() {
		height = maxValue(height, entry.CellRow+entry.CellCharsHeight)
	}
	return height
}

func compactContentHeight(window *uimd.GeneratedWindowBase, messageRows int) int {
	height := activityItemMinimumExtent
	for _, entry := range window.GeneratedLayout() {
		if entry.Content == "" {
			continue
		}
		entryHeight := entry.CharsHeight
		if entry.Content == "message" {
			entryHeight = maxPositive(messageRows)
		}
		height = maxValue(height, entry.Row+maxPositive(entryHeight))
	}
	return height
}

func maxPositive(value int) int {
	if value < activityItemMinimumExtent {
		return activityItemMinimumExtent
	}
	return value
}

func maxValue(first int, second int) int {
	if first > second {
		return first
	}
	return second
}

func markerColor(eventType string) string {
	switch eventType {
	case "Task":
		return "#a78bfa"
	case "Warning":
		return "#facc15"
	case "Deploy":
		return "#22c55e"
	case "Note":
		return "#fb7185"
	default:
		return "#38bdf8"
	}
}

func bufferCells(buffer *uimd.TerminalBuffer, size uimd.Size) [][]uimd.TerminalCell {
	rows := make([][]uimd.TerminalCell, size.Height)
	for row := 0; row < size.Height; row++ {
		rows[row] = make([]uimd.TerminalCell, size.Width)
		for col := 0; col < size.Width; col++ {
			rows[row][col] = buffer.Cell(row, col)
		}
	}
	return rows
}

func WrappedLineCount(text string, width int) int {
	if width < 1 {
		width = 1
	}
	count := 0
	for _, line := range strings.Split(text, "\n") {
		lineWidth := len([]rune(line))
		rows := (lineWidth + width - 1) / width
		if rows < 1 {
			rows = 1
		}
		count += rows
	}
	if count < 1 {
		return 1
	}
	return count
}
