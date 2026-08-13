package uimd

import (
	"fmt"
	"math"
	"strconv"
	"strings"
	"time"
)

const (
	Expanded   = 0x1fffffff
	FitContent = 0x1ffffffe
)

const (
	minimumRenderableSize   = 1
	defaultViewportWidth    = 100
	defaultViewportHeight   = 32
	ansiTerminalBaseRow     = 1
	ansiTerminalBaseCol     = 1
	ansiSyncUpdateBegin     = "\x1b[?2026h"
	ansiSyncUpdateEnd       = "\x1b[?2026l"
	ansiResetScrollRegion   = "\x1b[r"
	minimumScrollRegionRows = 2
)

const (
	textGradientDefaultIntervalMs  = 70
	textGradientDefaultStep        = 1
	textGradientDefaultSegmentSize = 1
	textTabWidth                   = 4
)

type Point struct {
	Row int
	Col int
}

type Size struct {
	Width  int
	Height int
}

type Rect struct {
	Row    int
	Col    int
	Width  int
	Height int
}

func (rect Rect) Contains(point Point) bool {
	return point.Row >= rect.Row &&
		point.Row < rect.Row+rect.Height &&
		point.Col >= rect.Col &&
		point.Col < rect.Col+rect.Width
}

type LayoutItem struct {
	Row             int
	Col             int
	CellRow         int
	CellCol         int
	CellCharsWidth  int
	CellCharsHeight int
	CellName        string
	CellWidth       int
	CellHeight      int
	CellWidthMode   string
	CellHeightMode  string
	Width           int
	Height          int
	WidthMode       string
	HeightMode      string
	CharsWidth      int
	CharsHeight     int
	MarginRight     int
	MarginBottom    int
	Content         string
	CellStyle       Style
	ElementStyle    Style
}

type Color struct {
	Text string
	rgba *rgba
}

type rgba struct {
	red   int
	green int
	blue  int
	alpha int
}

func NewColor(value string) Color {
	value = strings.TrimSpace(value)
	parsed := parseRgba(value)
	return Color{Text: value, rgba: parsed}
}

func (color Color) Empty() bool {
	return color.Text == ""
}

func (color Color) Transparent() bool {
	return color.Text == "transparent" || (color.rgba != nil && color.rgba.alpha == 0)
}

func (color Color) String() string {
	if color.Text == "transparent" {
		return "transparent"
	}
	if color.rgba != nil {
		return fmt.Sprintf("#%02x%02x%02x", color.rgba.red, color.rgba.green, color.rgba.blue)
	}
	return color.Text
}

func (color Color) BlendOver(background Color) Color {
	if color.rgba == nil || background.rgba == nil {
		return color
	}
	foreground := color.rgba
	base := background.rgba
	if foreground.alpha >= 255 {
		return color
	}
	if foreground.alpha == 0 {
		return background
	}
	alpha := float64(foreground.alpha) / 255.0
	blend := func(fg int, bg int) int {
		return int(math.Round(alpha*float64(fg) + (1.0-alpha)*float64(bg)))
	}
	return NewColor(fmt.Sprintf("#%02x%02x%02x",
		blend(foreground.red, base.red),
		blend(foreground.green, base.green),
		blend(foreground.blue, base.blue)))
}

func parseRgba(value string) *rgba {
	if value == "" {
		return nil
	}
	if value == "transparent" {
		return &rgba{red: 0, green: 0, blue: 0, alpha: 0}
	}
	if !strings.HasPrefix(value, "#") {
		return nil
	}
	hex := strings.TrimPrefix(value, "#")
	if len(hex) != 6 && len(hex) != 8 {
		return nil
	}
	component := func(start int) (int, bool) {
		parsed, err := strconv.ParseInt(hex[start:start+2], 16, 32)
		if err != nil {
			return 0, false
		}
		return int(parsed), true
	}
	red, ok := component(0)
	if !ok {
		return nil
	}
	green, ok := component(2)
	if !ok {
		return nil
	}
	blue, ok := component(4)
	if !ok {
		return nil
	}
	alpha := 255
	if len(hex) == 8 {
		alpha, ok = component(6)
		if !ok {
			return nil
		}
	}
	return &rgba{red: red, green: green, blue: blue, alpha: alpha}
}

type Style struct {
	Color                  Color
	Background             Color
	BackgroundTexture      string
	BackgroundTextureColor Color
	ScopeDimBackground     Color
	BorderColor            Color
	BorderWidthHorizontal  *int
	BorderWidthVertical    *int
	Padding                *int
	PaddingTop             *int
	PaddingRight           *int
	PaddingBottom          *int
	PaddingLeft            *int
	Margin                 *int
	MarginTop              *int
	MarginRight            *int
	MarginBottom           *int
	MarginLeft             *int
	Gap                    *int
	TextAlign              string
	UserSelect             string
	ScrollX                *bool
	ScrollY                *bool
	TextColorGradient      *TextGradient
	TextBackgroundGradient *TextGradient
}

func NewStyle() Style {
	return Style{ScopeDimBackground: NewColor("#00000066")}
}

type TextGradient struct {
	IntervalMs  int
	Step        int
	SegmentSize int
	Colors      []Color
}

func Int(value int) *int {
	return &value
}

func Bool(value bool) *bool {
	return &value
}

func (style Style) Clone() Style {
	return style
}

func (style *Style) Merge(other Style) {
	if !other.Color.Empty() {
		style.Color = other.Color
	}
	if !other.Background.Empty() {
		style.Background = other.Background
	}
	if other.BackgroundTexture != "" {
		style.BackgroundTexture = other.BackgroundTexture
	}
	if !other.BackgroundTextureColor.Empty() {
		style.BackgroundTextureColor = other.BackgroundTextureColor
	}
	if !other.ScopeDimBackground.Empty() {
		style.ScopeDimBackground = other.ScopeDimBackground
	}
	if !other.BorderColor.Empty() {
		style.BorderColor = other.BorderColor
	}
	if other.BorderWidthHorizontal != nil {
		style.BorderWidthHorizontal = other.BorderWidthHorizontal
	}
	if other.BorderWidthVertical != nil {
		style.BorderWidthVertical = other.BorderWidthVertical
	}
	if other.Padding != nil {
		style.Padding = other.Padding
	}
	if other.PaddingTop != nil {
		style.PaddingTop = other.PaddingTop
	}
	if other.PaddingRight != nil {
		style.PaddingRight = other.PaddingRight
	}
	if other.PaddingBottom != nil {
		style.PaddingBottom = other.PaddingBottom
	}
	if other.PaddingLeft != nil {
		style.PaddingLeft = other.PaddingLeft
	}
	if other.Margin != nil {
		style.Margin = other.Margin
	}
	if other.MarginTop != nil {
		style.MarginTop = other.MarginTop
	}
	if other.MarginRight != nil {
		style.MarginRight = other.MarginRight
	}
	if other.MarginBottom != nil {
		style.MarginBottom = other.MarginBottom
	}
	if other.MarginLeft != nil {
		style.MarginLeft = other.MarginLeft
	}
	if other.Gap != nil {
		style.Gap = other.Gap
	}
	if other.TextAlign != "" {
		style.TextAlign = other.TextAlign
	}
	if other.UserSelect != "" {
		style.UserSelect = other.UserSelect
	}
	if other.ScrollX != nil {
		style.ScrollX = other.ScrollX
	}
	if other.ScrollY != nil {
		style.ScrollY = other.ScrollY
	}
	if other.TextColorGradient != nil {
		style.TextColorGradient = other.TextColorGradient
	}
	if other.TextBackgroundGradient != nil {
		style.TextBackgroundGradient = other.TextBackgroundGradient
	}
}

var renderTimeOverrideMs *int64

type TerminalCell struct {
	Text       string
	Raw        string
	RawWidth   int
	RawHeight  int
	RawSkip    bool
	Foreground Color
	Background Color
}

func NewTerminalCell() TerminalCell {
	return TerminalCell{Text: " ", RawWidth: 1, RawHeight: 1}
}

func (cell TerminalCell) Clone() TerminalCell {
	if cell.Text == "" {
		cell.Text = " "
	}
	if cell.RawWidth <= 0 {
		cell.RawWidth = 1
	}
	if cell.RawHeight <= 0 {
		cell.RawHeight = 1
	}
	return cell
}

type TerminalBuffer struct {
	width           int
	height          int
	cells           [][]TerminalCell
	previous        [][]TerminalCell
	forceFullRedraw bool
}

func NewTerminalBuffer(width int, height int) *TerminalBuffer {
	width = maxInt(minimumRenderableSize, width)
	height = maxInt(minimumRenderableSize, height)
	buffer := &TerminalBuffer{width: width, height: height}
	buffer.cells = newTerminalCellGrid(width, height)
	buffer.previous = newTerminalCellGrid(width, height)
	return buffer
}

func newTerminalCellGrid(width int, height int) [][]TerminalCell {
	cells := make([][]TerminalCell, height)
	for row := 0; row < height; row++ {
		cells[row] = make([]TerminalCell, width)
		for col := 0; col < width; col++ {
			cells[row][col] = NewTerminalCell()
		}
	}
	return cells
}

func (buffer *TerminalBuffer) Width() int {
	return buffer.width
}

func (buffer *TerminalBuffer) Height() int {
	return buffer.height
}

func (buffer *TerminalBuffer) Cell(row int, col int) TerminalCell {
	if row < 0 || row >= buffer.height || col < 0 || col >= buffer.width {
		return NewTerminalCell()
	}
	return buffer.cells[row][col]
}

func (buffer *TerminalBuffer) SetCell(row int, col int, cell TerminalCell) {
	if row < 0 || row >= buffer.height || col < 0 || col >= buffer.width {
		return
	}
	buffer.cells[row][col] = cell.Clone()
}

func (buffer *TerminalBuffer) ReplaceContent(current *TerminalBuffer) {
	if buffer == nil || current == nil {
		return
	}
	if buffer.width != current.width || buffer.height != current.height {
		buffer.width = current.width
		buffer.height = current.height
		buffer.cells = newTerminalCellGrid(buffer.width, buffer.height)
		buffer.previous = newTerminalCellGrid(buffer.width, buffer.height)
		buffer.forceFullRedraw = true
	}
	for row := 0; row < buffer.height; row++ {
		copy(buffer.cells[row], current.cells[row])
	}
}

func (buffer *TerminalBuffer) RequestFullRedraw() {
	if buffer != nil {
		buffer.forceFullRedraw = true
	}
}

func (buffer *TerminalBuffer) Fill(rect Rect, style Style) {
	for row := rect.Row; row < rect.Row+rect.Height; row++ {
		for col := rect.Col; col < rect.Col+rect.Width; col++ {
			cell := styledFillCell(style, row-rect.Row, col-rect.Col, rect.Width)
			base := buffer.Cell(row, col)
			cell.Background = blendBackgroundOverExisting(cell.Background, base.Background)
			cell.Foreground = blendForegroundOverBackground(cell.Foreground, base.Foreground, cell.Background)
			buffer.SetCell(row, col, cell)
		}
	}
}

func (buffer *TerminalBuffer) Blit(rows [][]TerminalCell, rowOffset int, colOffset int, clip Rect) {
	for row := 0; row < len(rows); row++ {
		for col := 0; col < len(rows[row]); col++ {
			target := Point{Row: rowOffset + row, Col: colOffset + col}
			if !clip.Contains(target) {
				continue
			}
			cell := rows[row][col].Clone()
			base := buffer.Cell(target.Row, target.Col)
			cell.Background = blendBackgroundOverExisting(cell.Background, base.Background)
			cell.Foreground = blendForegroundOverBackground(cell.Foreground, base.Foreground, cell.Background)
			buffer.SetCell(target.Row, target.Col, cell)
		}
	}
}

func (buffer *TerminalBuffer) Merge(overlay *TerminalBuffer) {
	if overlay == nil {
		return
	}
	height := minInt(buffer.height, overlay.height)
	width := minInt(buffer.width, overlay.width)
	for row := 0; row < height; row++ {
		for col := 0; col < width; col++ {
			cell := overlay.cells[row][col].Clone()
			if cell.Text == " " && cell.Foreground.Empty() && cell.Background.Empty() && cell.Raw == "" {
				continue
			}
			base := buffer.Cell(row, col)
			cell.Background = blendBackgroundOverExisting(cell.Background, base.Background)
			cell.Foreground = blendForegroundOverBackground(cell.Foreground, base.Foreground, cell.Background)
			buffer.SetCell(row, col, cell)
		}
	}
}

func styledFillCell(style Style, rowOffset int, colOffset int, textureWidth int) TerminalCell {
	glyphs := []rune(style.BackgroundTexture)
	if len(glyphs) == 0 {
		return TerminalCell{Text: " ", Foreground: style.Color, Background: style.Background}
	}
	safeTextureWidth := maxInt(minimumRenderableSize, textureWidth)
	index := (rowOffset*safeTextureWidth + colOffset) % len(glyphs)
	foreground := style.Color
	if !style.BackgroundTextureColor.Empty() {
		foreground = style.BackgroundTextureColor
	}
	return TerminalCell{Text: string(glyphs[index]), Foreground: foreground, Background: style.Background}
}

func blendBackgroundOverExisting(color Color, existing Color) Color {
	if color.Empty() || color.Transparent() {
		return existing
	}
	if color.rgba != nil && color.rgba.alpha < 255 && !existing.Empty() {
		return color.BlendOver(existing)
	}
	return color
}

func blendForegroundOverBackground(color Color, inherited Color, background Color) Color {
	if color.Empty() || color.Transparent() {
		return inherited
	}
	if color.rgba != nil && color.rgba.alpha < 255 && !background.Empty() {
		return color.BlendOver(background)
	}
	return color
}

func (buffer *TerminalBuffer) Dim(color Color) {
	if color.Empty() || color.Transparent() {
		return
	}
	for row := 0; row < buffer.height; row++ {
		for col := 0; col < buffer.width; col++ {
			cell := buffer.cells[row][col].Clone()
			if !cell.Background.Empty() {
				cell.Background = color.BlendOver(cell.Background)
			}
			if !cell.Foreground.Empty() {
				cell.Foreground = color.BlendOver(cell.Foreground)
			}
			buffer.SetCell(row, col, cell)
		}
	}
}

func (buffer *TerminalBuffer) PlainText() string {
	var builder strings.Builder
	for row := 0; row < buffer.height; row++ {
		if row > 0 {
			builder.WriteByte('\n')
		}
		line := make([]string, buffer.width)
		for col := 0; col < buffer.width; col++ {
			text := buffer.cells[row][col].Text
			if text == "" {
				text = " "
			}
			line[col] = text
		}
		builder.WriteString(strings.TrimRight(strings.Join(line, ""), " "))
	}
	return strings.TrimRight(builder.String(), "\n")
}

func (buffer *TerminalBuffer) AnsiFrame() string {
	if buffer.hasRawCells() {
		return buffer.ansiFrameWithRawCells()
	}
	var builder strings.Builder
	builder.WriteString("\x1b[H")
	var foreground Color
	var background Color
	for row := 0; row < buffer.height; row++ {
		if row > 0 {
			builder.WriteString("\x1b[0m\r\n")
			foreground = Color{}
			background = Color{}
		}
		for col := 0; col < buffer.width; col++ {
			cell := buffer.cells[row][col]
			if !sameColor(cell.Foreground, foreground) || !sameColor(cell.Background, background) {
				builder.WriteString(sgrForCell(cell))
				foreground = cell.Foreground
				background = cell.Background
			}
			text := safeTerminalText(cell.Text)
			if text == "" {
				text = " "
			}
			builder.WriteString(text)
		}
	}
	builder.WriteString("\x1b[0m")
	return builder.String()
}

func (buffer *TerminalBuffer) RenderDiff() string {
	if buffer == nil {
		return ""
	}
	var builder strings.Builder
	fullRedraw := buffer.forceFullRedraw
	synchronizeUpdate := false
	rawEmitted := false
	for row := 0; row < buffer.height; row++ {
		col := 0
		for col < buffer.width {
			current := buffer.cells[row][col]
			if current.RawSkip {
				buffer.previous[row][col] = current
				col++
				continue
			}
			if !fullRedraw && sameTerminalCell(current, buffer.previous[row][col]) {
				col++
				continue
			}
			if current.Raw != "" {
				synchronizeUpdate = true
				rawWidth := maxInt(minimumRenderableSize, current.RawWidth)
				rawHeight := maxInt(minimumRenderableSize, current.RawHeight)
				clearWidth := minInt(rawWidth, buffer.width-col)
				clearHeight := minInt(rawHeight, buffer.height-row)
				for clearRow := row; clearRow < row+clearHeight; clearRow++ {
					writeAnsiCursorPosition(&builder, clearRow, col)
					builder.WriteString(sgrForCell(current))
					builder.WriteString(strings.Repeat(" ", clearWidth))
				}
				if clearHeight >= rawHeight {
					noScrollRegion := rawNoScrollRegion(row, rawHeight, buffer.height)
					builder.WriteString(noScrollRegion)
					writeAnsiCursorPosition(&builder, row, col)
					builder.WriteString(current.Raw)
					if noScrollRegion != "" {
						builder.WriteString(ansiResetScrollRegion)
					}
					rawEmitted = true
				}
				for coveredRow := row; coveredRow < row+clearHeight; coveredRow++ {
					for coveredCol := col; coveredCol < col+clearWidth; coveredCol++ {
						buffer.previous[coveredRow][coveredCol] = buffer.cells[coveredRow][coveredCol]
					}
				}
				col += clearWidth
				continue
			}
			runCol := col
			styleCell := current
			var run strings.Builder
			for col < buffer.width {
				current = buffer.cells[row][col]
				if !fullRedraw && sameTerminalCell(current, buffer.previous[row][col]) {
					break
				}
				if current.RawSkip || current.Raw != "" ||
					!sameColor(current.Foreground, styleCell.Foreground) ||
					!sameColor(current.Background, styleCell.Background) {
					break
				}
				text := safeTerminalText(current.Text)
				if text == "" {
					text = " "
				}
				run.WriteString(text)
				buffer.previous[row][col] = current
				col++
			}
			writeAnsiCursorPosition(&builder, row, runCol)
			builder.WriteString(sgrForCell(styleCell))
			builder.WriteString(run.String())
		}
	}
	if builder.Len() == 0 {
		buffer.forceFullRedraw = false
		return ""
	}
	if rawEmitted {
		for row := 0; row < buffer.height; row++ {
			col := 0
			for col < buffer.width {
				cell := buffer.cells[row][col]
				if cell.RawSkip || cell.Raw != "" {
					col++
					continue
				}
				runCol := col
				styleCell := cell
				var run strings.Builder
				for col < buffer.width {
					current := buffer.cells[row][col]
					if current.RawSkip || current.Raw != "" ||
						!sameColor(current.Foreground, styleCell.Foreground) ||
						!sameColor(current.Background, styleCell.Background) {
						break
					}
					text := safeTerminalText(current.Text)
					if text == "" {
						text = " "
					}
					run.WriteString(text)
					col++
				}
				writeAnsiCursorPosition(&builder, row, runCol)
				builder.WriteString(sgrForCell(styleCell))
				builder.WriteString(run.String())
			}
		}
	}
	builder.WriteString("\x1b[0m")
	buffer.forceFullRedraw = false
	if synchronizeUpdate {
		return ansiSyncUpdateBegin + builder.String() + ansiSyncUpdateEnd
	}
	return builder.String()
}

func sameTerminalCell(left TerminalCell, right TerminalCell) bool {
	return left.Text == right.Text &&
		left.Raw == right.Raw &&
		left.RawWidth == right.RawWidth &&
		left.RawHeight == right.RawHeight &&
		left.RawSkip == right.RawSkip &&
		sameColor(left.Foreground, right.Foreground) &&
		sameColor(left.Background, right.Background)
}

func (buffer *TerminalBuffer) hasRawCells() bool {
	for row := 0; row < buffer.height; row++ {
		for col := 0; col < buffer.width; col++ {
			if buffer.cells[row][col].Raw != "" {
				return true
			}
		}
	}
	return false
}

func (buffer *TerminalBuffer) ansiFrameWithRawCells() string {
	var builder strings.Builder
	builder.WriteString(ansiSyncUpdateBegin)
	builder.WriteString("\x1b[H")
	for row := 0; row < buffer.height; row++ {
		for col := 0; col < buffer.width; col++ {
			cell := buffer.cells[row][col]
			if cell.Raw == "" {
				continue
			}
			rawWidth := maxInt(minimumRenderableSize, cell.RawWidth)
			rawHeight := maxInt(minimumRenderableSize, cell.RawHeight)
			clearWidth := minInt(rawWidth, buffer.width-col)
			clearHeight := minInt(rawHeight, buffer.height-row)
			for clearRow := row; clearRow < row+clearHeight; clearRow++ {
				writeAnsiCursorPosition(&builder, clearRow, col)
				builder.WriteString(sgrForCell(cell))
				builder.WriteString(strings.Repeat(" ", clearWidth))
			}
			if clearHeight >= rawHeight {
				noScrollRegion := rawNoScrollRegion(row, rawHeight, buffer.height)
				builder.WriteString(noScrollRegion)
				writeAnsiCursorPosition(&builder, row, col)
				builder.WriteString(cell.Raw)
				if noScrollRegion != "" {
					builder.WriteString(ansiResetScrollRegion)
				}
			}
		}
	}
	for row := 0; row < buffer.height; row++ {
		col := 0
		for col < buffer.width {
			cell := buffer.cells[row][col]
			if cell.RawSkip || cell.Raw != "" {
				col++
				continue
			}
			runCol := col
			foreground := cell.Foreground
			background := cell.Background
			var run strings.Builder
			for col < buffer.width {
				current := buffer.cells[row][col]
				if current.RawSkip || current.Raw != "" ||
					!sameColor(current.Foreground, foreground) ||
					!sameColor(current.Background, background) {
					break
				}
				text := safeTerminalText(current.Text)
				if text == "" {
					text = " "
				}
				run.WriteString(text)
				col++
			}
			writeAnsiCursorPosition(&builder, row, runCol)
			builder.WriteString(sgrForCell(cell))
			builder.WriteString(run.String())
		}
	}
	builder.WriteString("\x1b[0m")
	builder.WriteString(ansiSyncUpdateEnd)
	return builder.String()
}

func rawNoScrollRegion(anchorRow int, rawHeight int, bufferBottomExclusive int) string {
	if anchorRow >= minimumScrollRegionRows {
		return fmt.Sprintf("\x1b[1;%dr", anchorRow)
	}
	rawBottomExclusive := anchorRow + maxInt(minimumRenderableSize, rawHeight)
	if bufferBottomExclusive-rawBottomExclusive >= minimumScrollRegionRows {
		return fmt.Sprintf("\x1b[%d;%dr", rawBottomExclusive+ansiTerminalBaseRow, bufferBottomExclusive)
	}
	return ""
}

func writeAnsiCursorPosition(builder *strings.Builder, row int, col int) {
	fmt.Fprintf(builder, "\x1b[%d;%dH", row+ansiTerminalBaseRow, col+ansiTerminalBaseCol)
}

func RenderPlainText(text string, width int, height int, style Style) [][]TerminalCell {
	width = maxInt(minimumRenderableSize, width)
	height = maxInt(minimumRenderableSize, height)
	lines := wrapLineSegments(text, width, height)
	rows := make([][]TerminalCell, height)
	for row := 0; row < height; row++ {
		line := textLineSegment{Text: "", SourceStart: 0}
		if row < len(lines) {
			line = alignLineSegment(lines[row], width, style.TextAlign)
		}
		rows[row] = renderLineSegment(line, width, style)
	}
	return rows
}

func renderLine(text string, width int, style Style) []TerminalCell {
	glyphs := visualGlyphs(text, 0, 0)
	return renderLineSegment(textLineSegment{Text: text, Glyphs: glyphs, ContentLength: minInt(len(glyphs), maxInt(minimumRenderableSize, width))}, width, style)
}

type textLineSegment struct {
	Text          string
	Glyphs        []visualGlyph
	SourceStart   int
	ContentOffset int
	ContentLength int
}

func renderLineSegment(segment textLineSegment, width int, style Style) []TerminalCell {
	cells := make([]TerminalCell, width)
	glyphs := segmentGlyphs(segment)
	for col := 0; col < width; col++ {
		char := " "
		sourceIndex := -1
		if col < len(glyphs) {
			char = glyphs[col].Text
			sourceIndex = glyphs[col].SourceStart
		}
		foreground := style.Color
		background := style.Background
		if sourceIndex >= 0 {
			foreground = blendGradientColor(gradientColorAt(style.TextColorGradient, sourceIndex), style.Color)
			background = blendGradientColor(gradientColorAt(style.TextBackgroundGradient, sourceIndex), style.Background)
		}
		cells[col] = TerminalCell{
			Text:       char,
			Foreground: foreground,
			Background: background,
		}
	}
	return cells
}

type visualGlyph struct {
	Text        string
	SourceStart int
}

func segmentGlyphs(segment textLineSegment) []visualGlyph {
	if segment.Glyphs != nil {
		return segment.Glyphs
	}
	return visualGlyphs(segment.Text, 0, segment.SourceStart)
}

func wrapLineSegments(text string, width int, height int) []textLineSegment {
	if width <= 0 {
		width = minimumRenderableSize
	}
	normalized := strings.ReplaceAll(strings.ReplaceAll(text, "\r\n", "\n"), "\r", "\n")
	if height <= 1 {
		part := strings.Split(normalized, "\n")[0]
		glyphs := visualGlyphs(part, 0, 0)
		take := minInt(width, len(glyphs))
		return []textLineSegment{{
			Text:          glyphsText(glyphs[:take]),
			Glyphs:        glyphs[:take],
			SourceStart:   0,
			ContentOffset: 0,
			ContentLength: take,
		}}
	}
	var result []textLineSegment
	sourceStart := 0
	for _, part := range strings.Split(normalized, "\n") {
		glyphs := visualGlyphs(part, 0, sourceStart)
		if len(glyphs) == 0 {
			result = append(result, textLineSegment{Text: "", SourceStart: sourceStart})
		}
		offset := 0
		for len(glyphs) > 0 {
			take := minInt(width, len(glyphs))
			consume := take
			if take == width {
				lastSpace := -1
				for index := take; index > 1; index-- {
					if glyphs[index-1].Text == " " {
						lastSpace = index - 1
						break
					}
				}
				if lastSpace > 0 {
					take = lastSpace
					consume = lastSpace + 1
				}
			}
			chunk := append([]visualGlyph(nil), glyphs[:take]...)
			result = append(result, textLineSegment{
				Text:          glyphsText(chunk),
				Glyphs:        chunk,
				SourceStart:   sourceStart + offset,
				ContentOffset: 0,
				ContentLength: take,
			})
			glyphs = glyphs[consume:]
			offset += consume
		}
		sourceStart += len(part) + 1
	}
	if len(result) == 0 {
		result = append(result, textLineSegment{Text: "", SourceStart: 0})
	}
	if height > 0 && len(result) > height {
		return result[:height]
	}
	return result
}

func alignText(text string, width int, align string) string {
	glyphs := visualGlyphs(text, 0, 0)
	return alignLineSegment(textLineSegment{Text: text, Glyphs: glyphs, ContentLength: minInt(len(glyphs), maxInt(minimumRenderableSize, width))}, width, align).Text
}

func alignLineSegment(segment textLineSegment, width int, align string) textLineSegment {
	glyphs := segmentGlyphs(segment)
	if len(glyphs) >= width {
		result := segment
		result.Glyphs = append([]visualGlyph(nil), glyphs[:width]...)
		result.Text = glyphsText(result.Glyphs)
		result.ContentLength = minInt(segment.ContentLength, width)
		return result
	}
	padding := width - len(glyphs)
	switch align {
	case "center":
		left := padding / 2
		result := segment
		result.Glyphs = append(blankGlyphs(left), glyphs...)
		result.Glyphs = append(result.Glyphs, blankGlyphs(padding-left)...)
		result.Text = glyphsText(result.Glyphs)
		result.ContentOffset = segment.ContentOffset + left
		return result
	case "right":
		result := segment
		result.Glyphs = append(blankGlyphs(padding), glyphs...)
		result.Text = glyphsText(result.Glyphs)
		result.ContentOffset = segment.ContentOffset + padding
		return result
	default:
		result := segment
		result.Glyphs = append(append([]visualGlyph(nil), glyphs...), blankGlyphs(padding)...)
		result.Text = glyphsText(result.Glyphs)
		return result
	}
}

func glyphsText(glyphs []visualGlyph) string {
	var builder strings.Builder
	for _, glyph := range glyphs {
		builder.WriteString(glyph.Text)
	}
	return builder.String()
}

func blankGlyphs(count int) []visualGlyph {
	if count <= 0 {
		return nil
	}
	glyphs := make([]visualGlyph, count)
	for index := range glyphs {
		glyphs[index] = visualGlyph{Text: " ", SourceStart: -1}
	}
	return glyphs
}

func visualGlyphs(text string, startColumn int, sourceStart int) []visualGlyph {
	var glyphs []visualGlyph
	column := maxInt(0, startColumn)
	for offset, ch := range text {
		rawStart := sourceStart + offset
		if ch == '\r' {
			continue
		}
		if ch == '\t' {
			spaces := tabSpacesForColumn(column)
			for index := 0; index < spaces; index++ {
				glyphs = append(glyphs, visualGlyph{Text: " ", SourceStart: rawStart})
				column++
			}
			continue
		}
		visible := string(ch)
		if isUnsafeTerminalRune(ch) {
			visible = "?"
		}
		if visible == "" {
			visible = " "
		}
		glyphs = append(glyphs, visualGlyph{Text: visible, SourceStart: rawStart})
		column++
	}
	return glyphs
}

func tabSpacesForColumn(column int) int {
	normalized := maxInt(0, column)
	return textTabWidth - (normalized % textTabWidth)
}

func isUnsafeTerminalRune(ch rune) bool {
	return ch < 0x20 ||
		ch == 0x7f ||
		inRuneRange(ch, 0x0300, 0x036f) ||
		inRuneRange(ch, 0x1ab0, 0x1aff) ||
		inRuneRange(ch, 0x1dc0, 0x1dff) ||
		inRuneRange(ch, 0x20d0, 0x20ff) ||
		inRuneRange(ch, 0xfe20, 0xfe2f) ||
		inRuneRange(ch, 0x200b, 0x200f) ||
		inRuneRange(ch, 0x202a, 0x202e) ||
		inRuneRange(ch, 0x2060, 0x206f) ||
		inRuneRange(ch, 0xfe00, 0xfe0f) ||
		inRuneRange(ch, 0xe0100, 0xe01ef) ||
		inRuneRange(ch, 0x1100, 0x115f) ||
		inRuneRange(ch, 0x2329, 0x232a) ||
		inRuneRange(ch, 0x2e80, 0xa4cf) ||
		inRuneRange(ch, 0xac00, 0xd7a3) ||
		inRuneRange(ch, 0xf900, 0xfaff) ||
		inRuneRange(ch, 0xfe10, 0xfe19) ||
		inRuneRange(ch, 0xfe30, 0xfe6f) ||
		inRuneRange(ch, 0xff00, 0xff60) ||
		inRuneRange(ch, 0xffe0, 0xffe6) ||
		inRuneRange(ch, 0x1f000, 0x1faff) ||
		inRuneRange(ch, 0x20000, 0x3fffd)
}

func inRuneRange(ch rune, first rune, last rune) bool {
	return ch >= first && ch <= last
}

func gradientFrame(gradient *TextGradient) int64 {
	if gradient == nil {
		return 0
	}
	interval := maxInt(textGradientDefaultIntervalMs, gradient.IntervalMs)
	if gradient.IntervalMs > 0 {
		interval = gradient.IntervalMs
	}
	var millis int64
	if renderTimeOverrideMs != nil {
		millis = *renderTimeOverrideMs
	} else {
		millis = time.Now().UnixMilli()
	}
	step := gradient.Step
	if step == 0 {
		step = textGradientDefaultStep
	}
	return (millis / int64(maxInt(1, interval))) * int64(step)
}

func gradientColorAt(gradient *TextGradient, sourceIndex int) *Color {
	if gradient == nil || len(gradient.Colors) == 0 {
		return nil
	}
	segmentSize := gradient.SegmentSize
	if segmentSize <= 0 {
		segmentSize = textGradientDefaultSegmentSize
	}
	frame := gradientFrame(gradient)
	size := int64(len(gradient.Colors))
	index := (int64(sourceIndex/segmentSize) - frame) % size
	if index < 0 {
		index += size
	}
	color := gradient.Colors[index]
	return &color
}

func blendGradientColor(overlay *Color, base Color) Color {
	if overlay == nil {
		return base
	}
	if overlay.rgba == nil {
		return *overlay
	}
	if overlay.rgba.alpha == 0 {
		return base
	}
	if overlay.rgba.alpha < 255 && !base.Empty() {
		return overlay.BlendOver(base)
	}
	return *overlay
}

func safeTerminalText(text string) string {
	if text == "" {
		return " "
	}
	glyphs := visualGlyphs(text, 0, 0)
	if len(glyphs) == 0 {
		return " "
	}
	return glyphs[0].Text
}

func sgrForCell(cell TerminalCell) string {
	parts := []string{"0"}
	if code := sgrForColor(cell.Foreground, true); code != "" {
		parts = append(parts, code)
	}
	if code := sgrForColor(cell.Background, false); code != "" {
		parts = append(parts, code)
	}
	return "\x1b[" + strings.Join(parts, ";") + "m"
}

func sgrForColor(color Color, foreground bool) string {
	if color.Empty() || color.Transparent() || color.rgba == nil {
		return ""
	}
	prefix := "38"
	if !foreground {
		prefix = "48"
	}
	return fmt.Sprintf("%s;2;%d;%d;%d", prefix, color.rgba.red, color.rgba.green, color.rgba.blue)
}

func sameColor(lhs Color, rhs Color) bool {
	return lhs.String() == rhs.String()
}

func maxInt(lhs int, rhs int) int {
	if lhs > rhs {
		return lhs
	}
	return rhs
}

func minInt(lhs int, rhs int) int {
	if lhs < rhs {
		return lhs
	}
	return rhs
}

func clampInt(value int, low int, high int) int {
	if high < low {
		return low
	}
	if value < low {
		return low
	}
	if value > high {
		return high
	}
	return value
}

func boolValue(value *bool, fallback bool) bool {
	if value == nil {
		return fallback
	}
	return *value
}

func intValue(value *int, fallback int) int {
	if value == nil {
		return fallback
	}
	return *value
}
