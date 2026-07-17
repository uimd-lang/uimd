package uimd

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"image"
	_ "image/gif"
	_ "image/jpeg"
	_ "image/png"
	"math"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

const (
	imageCellPixelWidth           = 8
	imageCellPixelHeight          = 16
	imageFallbackVerticalSamples  = 2
	testFallbackBlendDenominator  = 255
	testFallbackCheckerTilePixels = 4
	testFallbackCheckerLightAlpha = 160
	testFallbackCheckerDarkAlpha  = 0
	testFallbackColorQuantum      = 32
	imageInfoSampleGridSize       = 3
	imageInfoColorQuantum         = 64
	imageFallbackUpperHalfBlock   = "▀"
	imageFallbackFullBlock        = "█"
	imageMinimumSampleArea        = 0.000001
	colorOpaqueAlpha              = 255
	defaultImageFit               = "contain"
	defaultImageAlign             = "center"
	defaultImageVerticalAlign     = "middle"
	scrollIndicatorUp             = "^"
	scrollIndicatorDown           = "v"
)

var imageCellBackgroundRenderingDepth int

type ElementRenderState struct {
	Focused                  bool
	EditMode                 bool
	ChildEditMode            bool
	SuppressFocusVisuals     bool
	SuppressScrollIndicators bool
	FocusedElement           Element
	EditElement              Element
}

type Element interface {
	ElementName() string
	ElementType() string
	IsFocusable() bool
	IsEnabled() bool
	SetFrame(Rect)
	ElementFrame() Rect
	Render(Size, ElementRenderState) [][]TerminalCell
	HandleKey(string) bool
	SetText(string)
}

type preferredHeightElement interface {
	PreferredHeight(width int) int
}

type elementBase struct {
	Name           string
	Text           string
	Value          string
	Focusable      bool
	Enabled        bool
	frame          Rect
	style          Style
	focusStyle     *Style
	editStyle      *Style
	cursorStyle    *Style
	selectedStyle  *Style
	activeStyle    *Style
	checkedStyle   *Style
	uncheckedStyle *Style
	disabledStyle  *Style
}

func newElementBase(name string, text string, focusable bool) elementBase {
	return elementBase{
		Name:      name,
		Text:      text,
		Value:     text,
		Focusable: focusable,
		Enabled:   true,
		style:     NewStyle(),
	}
}

func (element *elementBase) ElementName() string {
	return element.Name
}

func (element *elementBase) ElementType() string {
	return "element"
}

func (element *elementBase) IsFocusable() bool {
	return element.Enabled && element.Focusable
}

func (element *elementBase) SetFocusable(focusable bool) {
	element.Focusable = focusable
}

func (element *elementBase) IsEnabled() bool {
	return element.Enabled
}

func (element *elementBase) SetFrame(frame Rect) {
	element.frame = frame
}

func (element *elementBase) ElementFrame() Rect {
	return element.frame
}

func (element *elementBase) SetEnabled(enabled bool) {
	element.Enabled = enabled
}

func (element *elementBase) SetText(text string) {
	element.Text = text
	element.Value = text
}

func (element *elementBase) SetStyle(style Style) {
	element.style = style
}

func (element *elementBase) SetFocusStyle(style Style) {
	element.focusStyle = &style
}

func (element *elementBase) SetEditStyle(style Style) {
	element.editStyle = &style
}

func (element *elementBase) SetCursorStyle(style Style) {
	element.cursorStyle = &style
}

func (element *elementBase) SetSelectedStyle(style Style) {
	element.selectedStyle = &style
}

func (element *elementBase) SetActiveStyle(style Style) {
	element.activeStyle = &style
}

func (element *elementBase) SetCheckedStyle(style Style) {
	element.checkedStyle = &style
}

func (element *elementBase) SetUncheckedStyle(style Style) {
	element.uncheckedStyle = &style
}

func (element *elementBase) SetDisabledStyle(style Style) {
	element.disabledStyle = &style
}

var (
	effectiveStyleParentBackground    Color
	effectiveStyleParentBackgroundSet bool
)

func hasPartialAlpha(color Color) bool {
	return !color.Empty() && color.rgba != nil && color.rgba.alpha < 255
}

func mergedStateStyle(base Style, state Style) Style {
	baseBackground := base.Background
	stateBackground := state.Background
	if hasPartialAlpha(baseBackground) &&
		effectiveStyleParentBackgroundSet &&
		effectiveStyleParentBackground.rgba != nil {
		base.Background = baseBackground.BlendOver(effectiveStyleParentBackground)
		baseBackground = base.Background
	}
	base.Merge(state)
	if hasPartialAlpha(stateBackground) &&
		!baseBackground.Empty() &&
		baseBackground.rgba != nil &&
		stateBackground.rgba.alpha == 0 {
		base.Background = baseBackground
	} else if hasPartialAlpha(stateBackground) &&
		!baseBackground.Empty() &&
		baseBackground.rgba != nil {
		base.Background = stateBackground.BlendOver(baseBackground)
	}
	return base
}

func withElementParentBackground(background Color, set bool, render func() [][]TerminalCell) [][]TerminalCell {
	previousBackground := effectiveStyleParentBackground
	previousSet := effectiveStyleParentBackgroundSet
	effectiveStyleParentBackground = background
	effectiveStyleParentBackgroundSet = set
	defer func() {
		effectiveStyleParentBackground = previousBackground
		effectiveStyleParentBackgroundSet = previousSet
	}()
	return render()
}

func (element *elementBase) EffectiveStyle(focused bool, editMode bool) Style {
	result := element.style.Clone()
	if !element.Enabled && element.disabledStyle != nil {
		return mergedStateStyle(result, *element.disabledStyle)
	}
	if editMode && element.editStyle != nil {
		return mergedStateStyle(result, *element.editStyle)
	}
	if focused && element.focusStyle != nil {
		return mergedStateStyle(result, *element.focusStyle)
	}
	return result
}

func (element *elementBase) collectStyleBackgrounds(backgrounds *[]Color) {
	appendUniqueOpaqueBackground(backgrounds, element.style.Background)
	if element.focusStyle != nil {
		appendUniqueOpaqueBackground(backgrounds, element.focusStyle.Background)
	}
	if element.editStyle != nil {
		appendUniqueOpaqueBackground(backgrounds, element.editStyle.Background)
	}
}

func (element *elementBase) HandleKey(string) bool {
	return false
}

func (element *elementBase) Render(size Size, state ElementRenderState) [][]TerminalCell {
	return RenderPlainText(element.Text, size.Width, size.Height, element.EffectiveStyle(state.Focused, state.EditMode))
}

type Label struct {
	elementBase
	spans []LabelSpan
}

func NewLabel(name string, text string) *Label {
	return &Label{elementBase: newElementBase(name, text, false)}
}

func (element *Label) ElementType() string {
	return "label"
}

func (element *Label) SetText(text string) {
	element.Text = text
	element.Value = text
	element.spans = nil
}

func (element *Label) SetSpans(spans []LabelSpan) {
	element.spans = append([]LabelSpan(nil), spans...)
	var builder strings.Builder
	for _, span := range element.spans {
		builder.WriteString(span.Text)
	}
	element.Text = builder.String()
	element.Value = element.Text
}

func (element *Label) Render(size Size, state ElementRenderState) [][]TerminalCell {
	if len(element.spans) == 0 {
		return RenderPlainText(element.Text, size.Width, size.Height, element.EffectiveStyle(state.Focused, state.EditMode))
	}
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	rows := RenderPlainText("", size.Width, size.Height, style)
	row := 0
	col := 0
	for _, span := range element.spans {
		foreground := NewColor(span.Foreground)
		if foreground.Empty() {
			foreground = NewColor(span.Color)
		}
		background := NewColor(span.Background)
		for _, ch := range span.Text {
			if ch == '\n' || col >= size.Width {
				row++
				col = 0
				if ch == '\n' {
					continue
				}
			}
			if row >= size.Height {
				return rows
			}
			cell := RenderPlainText(string(ch), 1, 1, style)[0][0]
			if !foreground.Empty() {
				cell.Foreground = foreground
			}
			if !background.Empty() {
				cell.Background = background
			}
			rows[row][col] = cell
			col++
		}
	}
	return rows
}

type LabelSpan struct {
	Text       string
	Color      string
	Foreground string
	Background string
}

type SpanLabel struct {
	Label
}

func NewSpanLabel(name string, text string) *SpanLabel {
	return &SpanLabel{Label: *NewLabel(name, text)}
}

type InfoLabel struct {
	Label
}

func NewInfoLabel(name string, text string) *InfoLabel {
	return &InfoLabel{Label: *NewLabel(name, text)}
}

func (element *InfoLabel) ElementType() string {
	return "infolabel"
}

func (element *InfoLabel) Render(size Size, state ElementRenderState) [][]TerminalCell {
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	if style.TextColorGradient == nil {
		style.TextColorGradient = defaultInfoLabelGradient()
	}
	return RenderPlainText(element.Text, size.Width, size.Height, style)
}

func defaultInfoLabelGradient() *TextGradient {
	return &TextGradient{
		IntervalMs:  textGradientDefaultIntervalMs,
		Step:        textGradientDefaultStep,
		SegmentSize: textGradientDefaultSegmentSize,
		Colors: []Color{
			NewColor("#ffffff66"),
			NewColor("#ffffff88"),
			NewColor("#ffffffaa"),
			NewColor("#ffffffcc"),
			NewColor("#ffffffff"),
			NewColor("#ffffffcc"),
			NewColor("#ffffffaa"),
			NewColor("#ffffff88"),
			NewColor("#ffffff66"),
		},
	}
}

type FrameBufferView struct {
	elementBase
	content [][]TerminalCell
}

func NewFrameBufferView(name string) *FrameBufferView {
	return &FrameBufferView{elementBase: newElementBase(name, "", false)}
}

func (element *FrameBufferView) ElementType() string {
	return "framebufferview"
}

func (element *FrameBufferView) SetContent(rows [][]TerminalCell) {
	element.content = rows
}

func (element *FrameBufferView) Render(size Size, state ElementRenderState) [][]TerminalCell {
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	rows := RenderPlainText("", size.Width, size.Height, style)
	for row := 0; row < minInt(size.Height, len(element.content)); row++ {
		for col := 0; col < minInt(size.Width, len(element.content[row])); col++ {
			rows[row][col] = element.content[row][col].Clone()
		}
	}
	return rows
}

type MessageTable struct {
	Label
}

func NewMessageTable(name string) *MessageTable {
	return &MessageTable{Label: *NewLabel(name, "")}
}

func (element *MessageTable) ElementType() string {
	return "messagetable"
}

func (element *MessageTable) PreferredHeight(width int) int {
	parsed, ok := parseMarkdownTable(element.Text)
	if !ok {
		return minimumRenderableSize
	}
	return len(parsed.Rows) + 4
}

func (element *MessageTable) Render(size Size, state ElementRenderState) [][]TerminalCell {
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	width := maxInt(minimumRenderableSize, size.Width)
	parsed, ok := parseMarkdownTable(element.Text)
	if !ok {
		return [][]TerminalCell{messageTableBlankRow(width, style)}
	}
	widths := expandMessageTableColumnWidths(messageTableColumnWidths(parsed), width)
	rows := [][]TerminalCell{
		messageTableSeparatorRow(widths, style, "┌", "┬", "┐"),
		messageTableDataRow(parsed.Headers, widths, parsed.Alignments, style),
		messageTableSeparatorRow(widths, style, "├", "┼", "┤"),
	}
	for _, row := range parsed.Rows {
		rows = append(rows, messageTableDataRow(row, widths, parsed.Alignments, style))
	}
	rows = append(rows, messageTableSeparatorRow(widths, style, "└", "┴", "┘"))
	return rows
}

type messageTableAlignment int

const (
	messageTableAlignLeft messageTableAlignment = iota
	messageTableAlignCenter
	messageTableAlignRight
)

type parsedMarkdownTable struct {
	Headers    []string
	Alignments []messageTableAlignment
	Rows       [][]string
}

func parseMarkdownTable(text string) (parsedMarkdownTable, bool) {
	lines := strings.Split(strings.ReplaceAll(strings.ReplaceAll(text, "\r\n", "\n"), "\r", "\n"), "\n")
	if len(lines) < 3 {
		return parsedMarkdownTable{}, false
	}
	headers := splitMarkdownTableCells(lines[0])
	separator := splitMarkdownTableCells(lines[1])
	if len(headers) == 0 || len(headers) != len(separator) || !isMarkdownSeparatorRow(separator) {
		return parsedMarkdownTable{}, false
	}
	parsed := parsedMarkdownTable{
		Headers:    headers,
		Alignments: parseMarkdownTableAlignments(separator),
	}
	for _, line := range lines[2:] {
		row := splitMarkdownTableCells(line)
		if len(row) == len(headers) {
			parsed.Rows = append(parsed.Rows, row)
		}
	}
	if len(parsed.Rows) == 0 {
		return parsedMarkdownTable{}, false
	}
	return parsed, true
}

func splitMarkdownTableCells(line string) []string {
	if strings.HasPrefix(line, "|") {
		line = line[1:]
	}
	if strings.HasSuffix(line, "|") {
		line = line[:len(line)-1]
	}
	parts := strings.Split(line, "|")
	cells := make([]string, 0, len(parts))
	for _, part := range parts {
		cells = append(cells, strings.TrimSpace(part))
	}
	return cells
}

func isMarkdownSeparatorRow(cells []string) bool {
	if len(cells) == 0 {
		return false
	}
	for _, cell := range cells {
		compact := strings.TrimSpace(cell)
		if compact == "" {
			return false
		}
		for _, ch := range compact {
			if ch != '-' && ch != ':' {
				return false
			}
		}
	}
	return true
}

func parseMarkdownTableAlignments(cells []string) []messageTableAlignment {
	alignments := make([]messageTableAlignment, 0, len(cells))
	for _, cell := range cells {
		compact := strings.TrimSpace(cell)
		left := strings.HasPrefix(compact, ":")
		right := strings.HasSuffix(compact, ":")
		if left && right {
			alignments = append(alignments, messageTableAlignCenter)
		} else if right {
			alignments = append(alignments, messageTableAlignRight)
		} else {
			alignments = append(alignments, messageTableAlignLeft)
		}
	}
	return alignments
}

func messageTableColumnWidths(table parsedMarkdownTable) []int {
	widths := make([]int, len(table.Headers))
	for column, header := range table.Headers {
		widths[column] = messageTableVisibleWidth(header)
	}
	for _, row := range table.Rows {
		for column, cell := range row {
			if column >= len(widths) {
				break
			}
			widths[column] = maxInt(widths[column], messageTableVisibleWidth(cell))
		}
	}
	return widths
}

func renderedMessageTableWidth(widths []int) int {
	if len(widths) == 0 {
		return 0
	}
	total := 1
	for _, width := range widths {
		total += width + 2
	}
	total += len(widths)
	return total
}

func expandMessageTableColumnWidths(widths []int, targetWidth int) []int {
	if len(widths) == 0 {
		return widths
	}
	extraWidth := targetWidth - renderedMessageTableWidth(widths)
	if extraWidth <= 0 {
		return widths
	}
	perColumn := extraWidth / len(widths)
	remainder := extraWidth % len(widths)
	expanded := append([]int(nil), widths...)
	for index := range expanded {
		expanded[index] += perColumn
		if index < remainder {
			expanded[index]++
		}
	}
	return expanded
}

func messageTableSeparatorRow(widths []int, style Style, left string, middle string, right string) []TerminalCell {
	row := []TerminalCell{}
	row = append(row, messageTableCell(left, style))
	for column, width := range widths {
		for index := 0; index < width+2; index++ {
			row = append(row, messageTableCell("─", style))
		}
		if column+1 < len(widths) {
			row = append(row, messageTableCell(middle, style))
		}
	}
	row = append(row, messageTableCell(right, style))
	return row
}

func messageTableDataRow(cells []string, widths []int, alignments []messageTableAlignment, style Style) []TerminalCell {
	row := []TerminalCell{messageTableCell("│", style)}
	for column, width := range widths {
		value := ""
		if column < len(cells) {
			value = cells[column]
		}
		alignment := messageTableAlignLeft
		if column < len(alignments) {
			alignment = alignments[column]
		}
		row = append(row, messageTableCell(" ", style))
		row = append(row, messageTableCells(messageTableAlignText(value, width, alignment), style)...)
		row = append(row, messageTableCell(" ", style))
		if column+1 < len(widths) {
			row = append(row, messageTableCell("│", style))
		}
	}
	row = append(row, messageTableCell("│", style))
	return row
}

func messageTableBlankRow(width int, style Style) []TerminalCell {
	row := make([]TerminalCell, width)
	for column := range row {
		row[column] = messageTableCell(" ", style)
	}
	return row
}

func messageTableCell(text string, style Style) TerminalCell {
	return TerminalCell{
		Text:       safeTerminalText(text),
		Foreground: style.Color,
		Background: style.Background,
	}
}

func messageTableCells(text string, style Style) []TerminalCell {
	glyphs := visualGlyphs(text, 0, 0)
	cells := make([]TerminalCell, 0, len(glyphs))
	for _, glyph := range glyphs {
		cells = append(cells, messageTableCell(glyph.Text, style))
	}
	return cells
}

func messageTableAlignText(text string, width int, alignment messageTableAlignment) string {
	textWidth := messageTableVisibleWidth(text)
	if textWidth >= width {
		return messageTableFitVisibleText(text, width)
	}
	padding := width - textWidth
	visible := glyphsText(visualGlyphs(text, 0, 0))
	switch alignment {
	case messageTableAlignRight:
		return strings.Repeat(" ", padding) + visible
	case messageTableAlignCenter:
		left := padding / 2
		return strings.Repeat(" ", left) + visible + strings.Repeat(" ", padding-left)
	default:
		return visible + strings.Repeat(" ", padding)
	}
}

func messageTableFitVisibleText(text string, width int) string {
	glyphs := visualGlyphs(text, 0, 0)
	if len(glyphs) > width {
		glyphs = glyphs[:width]
	}
	return glyphsText(glyphs)
}

func messageTableVisibleWidth(text string) int {
	return len(visualGlyphs(text, 0, 0))
}

type Image struct {
	elementBase
	Source        string
	Alt           string
	Fit           string
	RenderMode    string
	Align         string
	VerticalAlign string
}

func NewImage(name string, source string, alt string, fit string, renderMode string, align string, verticalAlign string) *Image {
	return &Image{
		elementBase:   newElementBase(name, alt, false),
		Source:        source,
		Alt:           alt,
		Fit:           fit,
		RenderMode:    renderMode,
		Align:         align,
		VerticalAlign: verticalAlign,
	}
}

func (element *Image) ElementType() string {
	return "image"
}

func (element *Image) Render(size Size, state ElementRenderState) [][]TerminalCell {
	width := maxInt(minimumRenderableSize, size.Width)
	height := maxInt(minimumRenderableSize, size.Height)
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	raster, ok := loadImageRaster(element.Source)
	if !ok || raster.width == 0 || raster.height == 0 {
		text := element.Alt
		if text == "" {
			text = filepath.Base(element.Source)
		}
		return RenderPlainText(text, width, height, style)
	}
	background := rgbFromColor(style.Background)
	if imageCellBackgroundRenderingDepth > 0 {
		return cellBackgroundImageContent(
			raster,
			width,
			height,
			normalizedImageValue(element.Fit, defaultImageFit),
			normalizedImageValue(element.Align, defaultImageAlign),
			normalizedImageValue(element.VerticalAlign, defaultImageVerticalAlign),
			style,
			background)
	}
	return deterministicImageFallbackContent(
		raster,
		width,
		height,
		normalizedImageValue(element.Fit, defaultImageFit),
		normalizedImageValue(element.Align, defaultImageAlign),
		normalizedImageValue(element.VerticalAlign, defaultImageVerticalAlign),
		style,
		background,
		strings.ToLower(strings.TrimSpace(element.RenderMode)) != "fallback")
}

func (element *Image) RenderInfo(size Size, state ElementRenderState) map[string]any {
	width := maxInt(minimumRenderableSize, size.Width)
	height := maxInt(minimumRenderableSize, size.Height)
	fit := normalizedImageValue(element.Fit, defaultImageFit)
	renderMode := normalizedImageValue(element.RenderMode, "auto")
	align := normalizedImageValue(element.Align, defaultImageAlign)
	verticalAlign := normalizedImageValue(element.VerticalAlign, defaultImageVerticalAlign)
	resolvedRenderMode := resolvedImageRenderMode(renderMode)
	info := map[string]any{
		"source":                 element.Source,
		"path":                   resolveImagePath(element.Source),
		"fit":                    fit,
		"configured_render_mode": renderMode,
		"resolved_render_mode":   resolvedRenderMode,
		"source_loaded":          false,
		"source_width":           0,
		"source_height":          0,
		"element_width":          width,
		"element_height":         height,
		"cell_pixel_width":       imageCellPixelWidth,
		"cell_pixel_height":      imageCellPixelHeight,
		"image_left":             0,
		"image_top":              0,
		"image_width":            0,
		"image_height":           0,
		"image_right":            0,
		"image_bottom":           0,
		"visible_left":           0,
		"visible_top":            0,
		"visible_width":          0,
		"visible_height":         0,
		"visible_right":          0,
		"visible_bottom":         0,
		"raw_expected":           false,
		"raw_present":            false,
		"sample_signature":       []string{},
	}
	raster, ok := loadImageRaster(element.Source)
	if !ok || raster.width == 0 || raster.height == 0 {
		info["resolved_render_mode"] = "placeholder"
		return info
	}
	region := imageRegion(width, height, raster.width, raster.height, fit, align, verticalAlign)
	visibleTop := region.rowOffset
	visibleHeight := maxInt(0, region.rows)
	visibleWidth := 0
	if visibleHeight > 0 {
		visibleWidth = region.cols
	}
	background := rgbFromColor(element.EffectiveStyle(state.Focused, state.EditMode).Background)
	regionFit := fit
	if regionFit == "contain" {
		regionFit = "cover"
	}
	signatureRows := region.rows
	if resolvedRenderMode == "sixel" {
		signatureRows = maxInt(1, visibleHeight)
	}
	info["source_loaded"] = true
	info["source_width"] = raster.width
	info["source_height"] = raster.height
	info["image_left"] = region.colOffset
	info["image_top"] = region.rowOffset
	info["image_width"] = region.cols
	info["image_height"] = region.rows
	info["image_right"] = region.colOffset + region.cols
	info["image_bottom"] = region.rowOffset + region.rows
	info["visible_left"] = region.colOffset
	info["visible_top"] = visibleTop
	info["visible_width"] = visibleWidth
	info["visible_height"] = visibleHeight
	info["visible_right"] = region.colOffset + visibleWidth
	info["visible_bottom"] = visibleTop + visibleHeight
	info["raw_expected"] = resolvedRenderMode == "sixel" && visibleHeight > 0
	info["raw_present"] = info["raw_expected"]
	info["sample_signature"] = imageInfoSampleSignature(raster, region.cols, maxInt(1, signatureRows), regionFit, align, verticalAlign, background)
	return info
}

func resolvedImageRenderMode(renderMode string) string {
	if imageCellBackgroundRenderingDepth > 0 {
		return "cell_background"
	}
	if deterministicImageFallbackEnabled() {
		return "deterministic"
	}
	if shouldRenderSixelForMode(renderMode) {
		return "sixel"
	}
	return "fallback"
}

func shouldRenderSixelForMode(renderMode string) bool {
	return (renderMode == "sixel" || renderMode == "auto") && terminalSupportsSixel()
}

func terminalSupportsSixel() bool {
	if truthyEnvironment("UIMD_FORCE_SIXEL") {
		return true
	}
	if truthyEnvironment("UIMD_DISABLE_SIXEL") {
		return false
	}
	return false
}

func deterministicImageFallbackEnabled() bool {
	return truthyEnvironment("UIMD_DETERMINISTIC_IMAGE_FALLBACK")
}

func truthyEnvironment(name string) bool {
	text := strings.ToLower(strings.TrimSpace(os.Getenv(name)))
	return text == "1" || text == "true" || text == "yes" || text == "on"
}

type imageRgb struct {
	red   int
	green int
	blue  int
}

type imageRaster struct {
	width  int
	height int
	pixels []imageRgb
	alpha  []int
}

type imageCellRegion struct {
	cols      int
	rows      int
	colOffset int
	rowOffset int
}

func loadImageRaster(path string) (imageRaster, bool) {
	resolvedPath := resolveImagePath(path)
	if raster, ok := loadDecodedImageRaster(resolvedPath); ok {
		return raster, true
	}
	data, err := os.ReadFile(resolvedPath)
	if err != nil {
		return imageRaster{}, false
	}
	if decoded, _, err := image.Decode(bytes.NewReader(data)); err == nil {
		return rasterFromDecodedImage(decoded)
	}
	if raster, ok := loadBmpRaster(data); ok {
		return raster, true
	}
	if raster, ok := loadTgaRaster(data); ok {
		return raster, true
	}
	return imageRaster{}, false
}

func rasterFromDecodedImage(decoded image.Image) (imageRaster, bool) {
	bounds := decoded.Bounds()
	width := bounds.Dx()
	height := bounds.Dy()
	if width <= 0 || height <= 0 {
		return imageRaster{}, false
	}
	raster := imageRaster{
		width:  width,
		height: height,
		pixels: make([]imageRgb, 0, width*height),
		alpha:  make([]int, 0, width*height),
	}
	for y := bounds.Min.Y; y < bounds.Max.Y; y++ {
		for x := bounds.Min.X; x < bounds.Max.X; x++ {
			red, green, blue, alpha := decoded.At(x, y).RGBA()
			raster.pixels = append(raster.pixels, imageRgb{red: int(red >> 8), green: int(green >> 8), blue: int(blue >> 8)})
			raster.alpha = append(raster.alpha, int(alpha>>8))
		}
	}
	return raster, true
}

func loadBmpRaster(data []byte) (imageRaster, bool) {
	const (
		fileHeaderSize      = 14
		infoHeaderSize      = 40
		bmpHeaderMinSize    = fileHeaderSize + infoHeaderSize
		bmpMagicFirst       = 'B'
		bmpMagicSecond      = 'M'
		bmpNoCompression    = 0
		bmpAlphaOpaque      = 255
		bmpBitsPerByte      = 8
		bmpRowAlignBits     = 31
		bmpRowAlignBytes    = 4
		bmpHeaderDataOffset = 10
		bmpDibSizeOffset    = 14
		bmpWidthOffset      = 18
		bmpHeightOffset     = 22
		bmpPlanesOffset     = 26
		bmpBitCountOffset   = 28
		bmpCompressionOff   = 30
	)
	if len(data) < bmpHeaderMinSize || data[0] != bmpMagicFirst || data[1] != bmpMagicSecond {
		return imageRaster{}, false
	}
	dibSize := int(binary.LittleEndian.Uint32(data[bmpDibSizeOffset:]))
	if dibSize < infoHeaderSize {
		return imageRaster{}, false
	}
	offset := int(binary.LittleEndian.Uint32(data[bmpHeaderDataOffset:]))
	width := int(int32(binary.LittleEndian.Uint32(data[bmpWidthOffset:])))
	heightSigned := int(int32(binary.LittleEndian.Uint32(data[bmpHeightOffset:])))
	planes := binary.LittleEndian.Uint16(data[bmpPlanesOffset:])
	bitCount := int(binary.LittleEndian.Uint16(data[bmpBitCountOffset:]))
	compression := binary.LittleEndian.Uint32(data[bmpCompressionOff:])
	if offset < bmpHeaderMinSize || offset > len(data) || width <= 0 || heightSigned == 0 || planes != 1 || compression != bmpNoCompression || (bitCount != 24 && bitCount != 32) {
		return imageRaster{}, false
	}
	height := heightSigned
	topDown := false
	if heightSigned < 0 {
		height = -heightSigned
		topDown = true
	}
	bytesPerPixel := bitCount / bmpBitsPerByte
	rowStride := ((width*bitCount + bmpRowAlignBits) / (bmpRowAlignBits + 1)) * bmpRowAlignBytes
	if offset+rowStride*height > len(data) {
		return imageRaster{}, false
	}
	raster := imageRaster{
		width:  width,
		height: height,
		pixels: make([]imageRgb, width*height),
		alpha:  make([]int, width*height),
	}
	for y := 0; y < height; y++ {
		sourceY := height - 1 - y
		if topDown {
			sourceY = y
		}
		rowStart := offset + sourceY*rowStride
		for x := 0; x < width; x++ {
			pixelStart := rowStart + x*bytesPerPixel
			target := y*width + x
			raster.pixels[target] = imageRgb{
				red:   int(data[pixelStart+2]),
				green: int(data[pixelStart+1]),
				blue:  int(data[pixelStart]),
			}
			alpha := bmpAlphaOpaque
			if bytesPerPixel == 4 {
				alpha = int(data[pixelStart+3])
			}
			raster.alpha[target] = alpha
		}
	}
	return raster, true
}

func loadTgaRaster(data []byte) (imageRaster, bool) {
	const (
		tgaHeaderSize       = 18
		tgaImageTypeTrue    = 2
		tgaImageTypeGray    = 3
		tgaOriginTopMask    = 0x20
		tgaBitsPerByte      = 8
		tgaAlphaOpaque      = 255
		tgaIDLengthOffset   = 0
		tgaColorMapOffset   = 1
		tgaImageTypeOffset  = 2
		tgaWidthOffset      = 12
		tgaHeightOffset     = 14
		tgaPixelDepthOffset = 16
		tgaDescriptorOffset = 17
	)
	if len(data) < tgaHeaderSize || data[tgaColorMapOffset] != 0 {
		return imageRaster{}, false
	}
	imageType := int(data[tgaImageTypeOffset])
	if imageType != tgaImageTypeTrue && imageType != tgaImageTypeGray {
		return imageRaster{}, false
	}
	width := int(binary.LittleEndian.Uint16(data[tgaWidthOffset:]))
	height := int(binary.LittleEndian.Uint16(data[tgaHeightOffset:]))
	bitCount := int(data[tgaPixelDepthOffset])
	if width <= 0 || height <= 0 || (bitCount != 8 && bitCount != 24 && bitCount != 32) {
		return imageRaster{}, false
	}
	bytesPerPixel := bitCount / tgaBitsPerByte
	offset := tgaHeaderSize + int(data[tgaIDLengthOffset])
	if offset+width*height*bytesPerPixel > len(data) {
		return imageRaster{}, false
	}
	topOrigin := data[tgaDescriptorOffset]&tgaOriginTopMask != 0
	raster := imageRaster{
		width:  width,
		height: height,
		pixels: make([]imageRgb, width*height),
		alpha:  make([]int, width*height),
	}
	for y := 0; y < height; y++ {
		targetY := y
		if !topOrigin {
			targetY = height - 1 - y
		}
		for x := 0; x < width; x++ {
			source := offset + (y*width+x)*bytesPerPixel
			target := targetY*width + x
			if imageType == tgaImageTypeGray {
				value := int(data[source])
				raster.pixels[target] = imageRgb{red: value, green: value, blue: value}
				raster.alpha[target] = tgaAlphaOpaque
				continue
			}
			raster.pixels[target] = imageRgb{
				red:   int(data[source+2]),
				green: int(data[source+1]),
				blue:  int(data[source]),
			}
			alpha := tgaAlphaOpaque
			if bytesPerPixel == 4 {
				alpha = int(data[source+3])
			}
			raster.alpha[target] = alpha
		}
	}
	return raster, true
}

func resolveImagePath(path string) string {
	if filepath.IsAbs(path) {
		return path
	}
	if _, err := os.Stat(path); err == nil {
		return path
	}
	current, err := os.Getwd()
	if err != nil {
		return path
	}
	for {
		candidate := filepath.Join(current, path)
		if _, err := os.Stat(candidate); err == nil {
			return candidate
		}
		parent := filepath.Dir(current)
		if parent == current {
			break
		}
		current = parent
	}
	return path
}

func deterministicImageFallbackContent(raster imageRaster, width int, height int, fit string, align string, verticalAlign string, style Style, background imageRgb, checker bool) [][]TerminalCell {
	content := RenderPlainText("", width, height, style)
	region := imageRegion(width, height, raster.width, raster.height, fit, align, verticalAlign)
	regionFit := fit
	if regionFit == "contain" {
		regionFit = "cover"
	}
	sampleHeight := region.rows * imageFallbackVerticalSamples
	for row := region.rowOffset; row < region.rowOffset+region.rows; row++ {
		if row < 0 || row >= height {
			continue
		}
		for col := region.colOffset; col < region.colOffset+region.cols; col++ {
			if col < 0 || col >= width {
				continue
			}
			sampleCol := col - region.colOffset
			sampleTop := (row - region.rowOffset) * imageFallbackVerticalSamples
			top := testFallbackSampleColor(raster, sampleCol, sampleTop, region.cols, sampleHeight, regionFit, align, verticalAlign, background, checker)
			bottom := testFallbackSampleColor(raster, sampleCol, sampleTop+1, region.cols, sampleHeight, regionFit, align, verticalAlign, background, checker)
			content[row][col] = TerminalCell{
				Text:       imageFallbackUpperHalfBlock,
				Foreground: colorFromImageRgb(top),
				Background: colorFromImageRgb(bottom),
			}
		}
	}
	return content
}

func beginImageCellBackgroundRendering() func() {
	imageCellBackgroundRenderingDepth++
	return func() {
		imageCellBackgroundRenderingDepth = maxInt(0, imageCellBackgroundRenderingDepth-1)
	}
}

func cellBackgroundImageContent(raster imageRaster, width int, height int, fit string, align string, verticalAlign string, style Style, background imageRgb) [][]TerminalCell {
	content := RenderPlainText("", width, height, style)
	region := imageRegion(width, height, raster.width, raster.height, fit, align, verticalAlign)
	regionFit := fit
	if regionFit == "contain" {
		regionFit = "cover"
	}
	image := resizeImageRaster(raster, region.cols, region.rows, regionFit, align, verticalAlign, background)
	if image.width <= 0 || image.height <= 0 {
		return content
	}
	for row := 0; row < region.rows; row++ {
		targetRow := region.rowOffset + row
		if targetRow < 0 || targetRow >= height {
			continue
		}
		for col := 0; col < region.cols; col++ {
			targetCol := region.colOffset + col
			if targetCol < 0 || targetCol >= width {
				continue
			}
			color := colorFromImageRgb(pixelAtImage(image, col, row))
			content[targetRow][targetCol] = TerminalCell{
				Text:       imageFallbackFullBlock,
				Foreground: color,
				Background: color,
			}
		}
	}
	return content
}

func imageInfoSampleSignature(raster imageRaster, targetWidth int, targetHeight int, fit string, align string, verticalAlign string, background imageRgb) []string {
	targetWidth = maxInt(minimumRenderableSize, targetWidth)
	targetHeight = maxInt(minimumRenderableSize, targetHeight)
	fitted := resizeImageRaster(raster, targetWidth, targetHeight, fit, align, verticalAlign, background)
	signature := make([]string, 0, imageInfoSampleGridSize*imageInfoSampleGridSize)
	for _, row := range imageInfoSamplePositions(targetHeight) {
		for _, col := range imageInfoSamplePositions(targetWidth) {
			color := pixelAtImage(fitted, col, row)
			color.red = quantizeImageInfoChannel(color.red)
			color.green = quantizeImageInfoChannel(color.green)
			color.blue = quantizeImageInfoChannel(color.blue)
			signature = append(signature, imageRgbHex(color))
		}
	}
	return signature
}

func imageInfoSamplePositions(extent int) []int {
	extent = maxInt(minimumRenderableSize, extent)
	positions := make([]int, 0, imageInfoSampleGridSize)
	if imageInfoSampleGridSize == minimumRenderableSize {
		return append(positions, extent/2)
	}
	for index := 0; index < imageInfoSampleGridSize; index++ {
		position := int(math.Round(float64(index) * float64(extent-1) / float64(imageInfoSampleGridSize-1)))
		positions = append(positions, clampInt(position, 0, extent-1))
	}
	return positions
}

func quantizeImageInfoChannel(value int) int {
	return clampInt((value/imageInfoColorQuantum)*imageInfoColorQuantum, 0, 255)
}

func imageRgbHex(color imageRgb) string {
	return fmt.Sprintf("#%02x%02x%02x", clampInt(color.red, 0, 255), clampInt(color.green, 0, 255), clampInt(color.blue, 0, 255))
}

func resizeImageRaster(source imageRaster, targetWidth int, targetHeight int, fit string, align string, verticalAlign string, background imageRgb) imageRaster {
	targetWidth = maxInt(minimumRenderableSize, targetWidth)
	targetHeight = maxInt(minimumRenderableSize, targetHeight)
	if source.width <= 0 || source.height <= 0 || len(source.pixels) == 0 {
		return imageRaster{}
	}
	pixelCount := targetWidth * targetHeight
	result := imageRaster{
		width:  targetWidth,
		height: targetHeight,
		pixels: make([]imageRgb, pixelCount),
		alpha:  make([]int, pixelCount),
	}
	for index := 0; index < pixelCount; index++ {
		result.pixels[index] = background
		result.alpha[index] = testFallbackBlendDenominator
	}

	normalizedFit := normalizedImageValue(fit, defaultImageFit)
	stretch := normalizedFit == "stretch"
	cover := normalizedFit == "cover"
	scale := 0.0
	if !stretch {
		scale = math.Min(float64(targetWidth)/float64(source.width), float64(targetHeight)/float64(source.height))
		if cover {
			scale = math.Max(float64(targetWidth)/float64(source.width), float64(targetHeight)/float64(source.height))
		}
	}
	drawnWidth := float64(targetWidth)
	drawnHeight := float64(targetHeight)
	if !stretch {
		drawnWidth = math.Max(1.0, float64(source.width)*scale)
		drawnHeight = math.Max(1.0, float64(source.height)*scale)
	}
	xOffset := 0.0
	yOffset := 0.0
	if !stretch {
		if cover {
			xOffset = alignmentOffset(drawnWidth, float64(targetWidth), align, "left", "right")
			yOffset = alignmentOffset(drawnHeight, float64(targetHeight), verticalAlign, "top", "bottom")
		} else {
			xOffset = alignmentOffset(float64(targetWidth), drawnWidth, align, "left", "right")
			yOffset = alignmentOffset(float64(targetHeight), drawnHeight, verticalAlign, "top", "bottom")
		}
	}

	for y := 0; y < targetHeight; y++ {
		for x := 0; x < targetWidth; x++ {
			sourceLeft := 0.0
			sourceRight := 0.0
			sourceTop := 0.0
			sourceBottom := 0.0
			if stretch {
				sourceLeft = float64(x) * float64(source.width) / float64(targetWidth)
				sourceRight = float64(x+1) * float64(source.width) / float64(targetWidth)
				sourceTop = float64(y) * float64(source.height) / float64(targetHeight)
				sourceBottom = float64(y+1) * float64(source.height) / float64(targetHeight)
			} else if cover {
				sourceLeft = (float64(x) + xOffset) / scale
				sourceRight = (float64(x+1) + xOffset) / scale
				sourceTop = (float64(y) + yOffset) / scale
				sourceBottom = (float64(y+1) + yOffset) / scale
			} else {
				sourceLeft = (float64(x) - xOffset) / scale
				sourceRight = (float64(x+1) - xOffset) / scale
				sourceTop = (float64(y) - yOffset) / scale
				sourceBottom = (float64(y+1) - yOffset) / scale
			}
			index := y*targetWidth + x
			result.pixels[index] = sampleRasterArea(source, sourceLeft, sourceTop, sourceRight, sourceBottom, background)
			result.alpha[index] = testFallbackBlendDenominator
		}
	}
	return result
}

func sampleRasterArea(source imageRaster, left float64, top float64, right float64, bottom float64, background imageRgb) imageRgb {
	fullArea := math.Max(imageMinimumSampleArea, (right-left)*(bottom-top))
	if source.width <= 0 || source.height <= 0 || len(source.pixels) == 0 ||
		right <= 0 || bottom <= 0 ||
		left >= float64(source.width) || top >= float64(source.height) {
		return background
	}

	red := float64(background.red) * fullArea
	green := float64(background.green) * fullArea
	blue := float64(background.blue) * fullArea
	startX := maxInt(0, int(math.Floor(left)))
	endX := minInt(source.width, int(math.Ceil(right)))
	startY := maxInt(0, int(math.Floor(top)))
	endY := minInt(source.height, int(math.Ceil(bottom)))
	for y := startY; y < endY; y++ {
		yWeight := intervalOverlap(top, bottom, float64(y), float64(y+1))
		if yWeight <= 0 {
			continue
		}
		for x := startX; x < endX; x++ {
			xWeight := intervalOverlap(left, right, float64(x), float64(x+1))
			if xWeight <= 0 {
				continue
			}
			weight := xWeight * yWeight
			alpha := float64(alphaAtImage(source, x, y)) / float64(testFallbackBlendDenominator)
			pixel := pixelAtImage(source, x, y)
			red += (float64(pixel.red) - float64(background.red)) * alpha * weight
			green += (float64(pixel.green) - float64(background.green)) * alpha * weight
			blue += (float64(pixel.blue) - float64(background.blue)) * alpha * weight
		}
	}
	return imageRgb{
		red:   channelFromImageSample(red / fullArea),
		green: channelFromImageSample(green / fullArea),
		blue:  channelFromImageSample(blue / fullArea),
	}
}

func channelFromImageSample(value float64) int {
	return clampInt(int(math.Round(value)), 0, 255)
}

func intervalOverlap(firstStart float64, firstEnd float64, secondStart float64, secondEnd float64) float64 {
	return math.Max(0, math.Min(firstEnd, secondEnd)-math.Max(firstStart, secondStart))
}

func imageRegion(width int, height int, sourceWidth int, sourceHeight int, fit string, align string, verticalAlign string) imageCellRegion {
	if fit != "contain" || sourceWidth == 0 || sourceHeight == 0 {
		return imageCellRegion{cols: width, rows: height}
	}
	canvasWidth := float64(width * imageCellPixelWidth)
	canvasHeight := float64(height * imageCellPixelHeight)
	scale := math.Min(canvasWidth/float64(sourceWidth), canvasHeight/float64(sourceHeight))
	cols := clampInt(int(math.Round(float64(sourceWidth)*scale/float64(imageCellPixelWidth))), minimumRenderableSize, width)
	rows := clampInt(int(math.Round(float64(sourceHeight)*scale/float64(imageCellPixelHeight))), minimumRenderableSize, height)
	return imageCellRegion{
		cols:      cols,
		rows:      rows,
		colOffset: int(alignmentOffset(float64(width), float64(cols), align, "left", "right")),
		rowOffset: int(alignmentOffset(float64(height), float64(rows), verticalAlign, "top", "bottom")),
	}
}

func testFallbackSampleColor(raster imageRaster, x int, y int, targetWidth int, targetHeight int, fit string, align string, verticalAlign string, background imageRgb, checker bool) imageRgb {
	targetWidth = maxInt(minimumRenderableSize, targetWidth)
	targetHeight = maxInt(minimumRenderableSize, targetHeight)
	x = clampInt(x, 0, targetWidth-1)
	y = clampInt(y, 0, targetHeight-1)
	base := background
	if checker {
		base = testFallbackCheckerTint(base, x, y)
	}
	if raster.width == 0 || raster.height == 0 || len(raster.pixels) == 0 {
		return quantizeTestFallbackColor(base)
	}
	sourceX := 0
	sourceY := 0
	if fit == "stretch" {
		sourceX = int(float64(x) * float64(raster.width) / float64(targetWidth))
		sourceY = int(float64(y) * float64(raster.height) / float64(targetHeight))
	} else {
		scale := math.Min(float64(targetWidth)/float64(raster.width), float64(targetHeight)/float64(raster.height))
		if fit == "cover" {
			scale = math.Max(float64(targetWidth)/float64(raster.width), float64(targetHeight)/float64(raster.height))
		}
		drawnWidth := float64(raster.width) * scale
		drawnHeight := float64(raster.height) * scale
		xOffset := alignmentOffset(float64(targetWidth), drawnWidth, align, "left", "right")
		yOffset := alignmentOffset(float64(targetHeight), drawnHeight, verticalAlign, "top", "bottom")
		sourceXFloat := (float64(x) - xOffset) / scale
		sourceYFloat := (float64(y) - yOffset) / scale
		if sourceXFloat < 0 || sourceYFloat < 0 || sourceXFloat >= float64(raster.width) || sourceYFloat >= float64(raster.height) {
			return quantizeTestFallbackColor(base)
		}
		sourceX = int(sourceXFloat)
		sourceY = int(sourceYFloat)
	}
	sourceX = clampInt(sourceX, 0, raster.width-1)
	sourceY = clampInt(sourceY, 0, raster.height-1)
	pixel := blendImageRgb(pixelAtImage(raster, sourceX, sourceY), alphaAtImage(raster, sourceX, sourceY), base)
	if checker {
		pixel = testFallbackCheckerTint(pixel, x, y)
	}
	return quantizeTestFallbackColor(pixel)
}

func pixelAtImage(raster imageRaster, x int, y int) imageRgb {
	if raster.width == 0 || raster.height == 0 || len(raster.pixels) == 0 {
		return imageRgb{}
	}
	x = clampInt(x, 0, raster.width-1)
	y = clampInt(y, 0, raster.height-1)
	return raster.pixels[y*raster.width+x]
}

func alphaAtImage(raster imageRaster, x int, y int) int {
	if len(raster.alpha) == 0 {
		return testFallbackBlendDenominator
	}
	x = clampInt(x, 0, raster.width-1)
	y = clampInt(y, 0, raster.height-1)
	return raster.alpha[y*raster.width+x]
}

func testFallbackCheckerTint(color imageRgb, x int, y int) imageRgb {
	alpha := testFallbackCheckerDarkAlpha
	if ((x/testFallbackCheckerTilePixels)+(y/testFallbackCheckerTilePixels))%2 == 0 {
		alpha = testFallbackCheckerLightAlpha
	}
	return blendImageRgb(imageRgb{red: 255, green: 255, blue: 255}, alpha, color)
}

func blendImageRgb(source imageRgb, alpha int, destination imageRgb) imageRgb {
	alpha = clampInt(alpha, 0, testFallbackBlendDenominator)
	blend := func(src int, dst int) int {
		return (src*alpha + dst*(testFallbackBlendDenominator-alpha)) / testFallbackBlendDenominator
	}
	return imageRgb{
		red:   blend(source.red, destination.red),
		green: blend(source.green, destination.green),
		blue:  blend(source.blue, destination.blue),
	}
}

func quantizeTestFallbackColor(color imageRgb) imageRgb {
	quantize := func(channel int) int {
		return clampInt((channel/testFallbackColorQuantum)*testFallbackColorQuantum, 0, 255)
	}
	return imageRgb{red: quantize(color.red), green: quantize(color.green), blue: quantize(color.blue)}
}

func alignmentOffset(outer float64, inner float64, value string, startValue string, endValue string) float64 {
	span := math.Max(0, outer-inner)
	switch strings.ToLower(strings.TrimSpace(value)) {
	case startValue:
		return 0
	case endValue:
		return span
	default:
		return span / 2
	}
}

func normalizedImageValue(value string, fallback string) string {
	value = strings.ToLower(strings.TrimSpace(value))
	if value == "" {
		return fallback
	}
	return value
}

func rgbFromColor(color Color) imageRgb {
	if color.rgba == nil {
		return imageRgb{}
	}
	return imageRgb{red: color.rgba.red, green: color.rgba.green, blue: color.rgba.blue}
}

func colorFromImageRgb(color imageRgb) Color {
	return NewColor(fmt.Sprintf("#%02x%02x%02x", clampInt(color.red, 0, 255), clampInt(color.green, 0, 255), clampInt(color.blue, 0, 255)))
}

type TextInput struct {
	elementBase
	MaxLength       int
	Multiline       bool
	Cursor          int
	SelectionStart  *int
	SelectionEnd    *int
	colScrollOffset int
	rowScrollOffset int
}

func NewTextInput(name string, value string, maxLength int) *TextInput {
	normalized := normalizeTextValue(value, false)
	return &TextInput{
		elementBase: newElementBase(name, normalized, true),
		MaxLength:   maxLength,
		Cursor:      len([]rune(normalized)),
	}
}

func NewTextArea(name string, value string) *TextArea {
	normalized := normalizeTextValue(value, true)
	input := TextArea{TextInput: TextInput{
		elementBase: newElementBase(name, normalized, true),
		MaxLength:   0,
		Multiline:   true,
		Cursor:      len([]rune(normalized)),
	}}
	input.Multiline = true
	return &input
}

func (element *TextInput) ElementType() string {
	return "textinput"
}

func (element *TextInput) SetText(text string) {
	element.SetValue(text)
}

func (element *TextInput) SetValue(value string) {
	value = normalizeTextValue(value, element.Multiline)
	runes := []rune(value)
	if element.MaxLength > 0 && len(runes) > element.MaxLength {
		runes = runes[:element.MaxLength]
		value = string(runes)
	}
	element.Text = value
	element.Value = value
	element.Cursor = clampInt(element.Cursor, 0, len(runes))
	element.ClearSelection()
}

func (element *TextInput) SetCursor(cursor int) {
	element.Cursor = clampInt(cursor, 0, len([]rune(element.Value)))
	element.ClearSelection()
}

func (element *TextInput) ClearSelection() {
	element.SelectionStart = nil
	element.SelectionEnd = nil
}

func (element *TextInput) SetSelection(start int, end int) {
	valueLength := len([]rune(element.Value))
	start = clampInt(start, 0, valueLength)
	end = clampInt(end, 0, valueLength)
	element.SelectionStart = &start
	element.SelectionEnd = &end
	element.Cursor = end
}

func (element *TextInput) SelectionText() string {
	if !element.hasSelection() {
		return ""
	}
	runes := []rune(element.Value)
	start, end := element.selectionRange()
	return string(runes[start:end])
}

func (element *TextInput) hasSelection() bool {
	return element.SelectionStart != nil && element.SelectionEnd != nil && *element.SelectionStart != *element.SelectionEnd
}

func (element *TextInput) selectionRange() (int, int) {
	if !element.hasSelection() {
		return element.Cursor, element.Cursor
	}
	start := *element.SelectionStart
	end := *element.SelectionEnd
	if start > end {
		start, end = end, start
	}
	return start, end
}

func (element *TextInput) InsertText(text string) {
	text = normalizeTextValue(text, element.Multiline)
	valueRunes := []rune(element.Value)
	start, end := element.selectionRange()
	prefix := string(valueRunes[:start])
	suffix := string(valueRunes[end:])
	next := prefix + text + suffix
	nextRunes := []rune(next)
	cursor := start + len([]rune(text))
	if element.MaxLength > 0 && len(nextRunes) > element.MaxLength {
		nextRunes = nextRunes[:element.MaxLength]
		next = string(nextRunes)
		cursor = minInt(cursor, len(nextRunes))
	}
	element.Value = next
	element.Text = next
	element.Cursor = clampInt(cursor, 0, len([]rune(next)))
	element.ClearSelection()
}

func (element *TextInput) HandleKey(key string) bool {
	valueRunes := []rune(element.Value)
	switch key {
	case "Left":
		if element.hasSelection() {
			start, _ := element.selectionRange()
			element.SetCursor(start)
		} else {
			element.SetCursor(element.Cursor - 1)
		}
		return true
	case "Right":
		if element.hasSelection() {
			_, end := element.selectionRange()
			element.SetCursor(end)
		} else {
			element.SetCursor(element.Cursor + 1)
		}
		return true
	case "Shift+Left":
		if element.SelectionStart == nil {
			start := element.Cursor
			element.SelectionStart = &start
		}
		element.Cursor = clampInt(element.Cursor-1, 0, len(valueRunes))
		end := element.Cursor
		element.SelectionEnd = &end
		return true
	case "Shift+Right":
		if element.SelectionStart == nil {
			start := element.Cursor
			element.SelectionStart = &start
		}
		element.Cursor = clampInt(element.Cursor+1, 0, len(valueRunes))
		end := element.Cursor
		element.SelectionEnd = &end
		return true
	case "Home":
		element.SetCursor(lineStartForCursor(element.Value, element.Cursor))
		return true
	case "End":
		element.SetCursor(lineEndForCursor(element.Value, element.Cursor))
		return true
	case "Backspace":
		if element.hasSelection() {
			element.deleteSelection()
		} else if element.Cursor > 0 {
			valueRunes = append(valueRunes[:element.Cursor-1], valueRunes[element.Cursor:]...)
			element.Cursor--
			element.Value = string(valueRunes)
			element.Text = element.Value
		}
		return true
	case "Delete":
		if element.hasSelection() {
			element.deleteSelection()
		} else if element.Cursor < len(valueRunes) {
			valueRunes = append(valueRunes[:element.Cursor], valueRunes[element.Cursor+1:]...)
			element.Value = string(valueRunes)
			element.Text = element.Value
		}
		return true
	case "Enter":
		if element.Multiline {
			element.InsertText("\n")
			return true
		}
		return false
	default:
		if len([]rune(key)) == 1 && key >= " " {
			element.InsertText(key)
			return true
		}
		return false
	}
}

func (element *TextInput) deleteSelection() {
	if !element.hasSelection() {
		return
	}
	valueRunes := []rune(element.Value)
	start, end := element.selectionRange()
	valueRunes = append(valueRunes[:start], valueRunes[end:]...)
	element.Value = string(valueRunes)
	element.Text = element.Value
	element.Cursor = start
	element.ClearSelection()
}

func (element *TextInput) Render(size Size, state ElementRenderState) [][]TerminalCell {
	width := maxInt(minimumRenderableSize, size.Width)
	height := minimumRenderableSize
	if element.Multiline {
		height = maxInt(minimumRenderableSize, size.Height)
	}
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	cursorStyle := style.Clone()
	if element.cursorStyle != nil {
		cursorStyle.Merge(*element.cursorStyle)
	}
	rows := RenderPlainText("", width, height, style)
	valueRunes := []rune(element.Value)
	startSelection, endSelection := element.selectionRange()
	if !element.Multiline {
		cursorVisual := element.Cursor
		if state.EditMode {
			if cursorVisual < element.colScrollOffset {
				element.colScrollOffset = cursorVisual
			}
			if cursorVisual >= element.colScrollOffset+width {
				element.colScrollOffset = maxInt(0, cursorVisual-width+1)
			}
		}
		end := minInt(len(valueRunes), element.colScrollOffset+width)
		visible := ""
		if element.colScrollOffset < len(valueRunes) {
			visible = string(valueRunes[element.colScrollOffset:end])
		}
		rows[0] = renderLine(visible, width, style)
		for col := 0; col < width; col++ {
			source := element.colScrollOffset + col
			if state.EditMode && element.hasSelection() && source >= startSelection && source < endSelection {
				rows[0][col].Foreground = cursorStyle.Color
				rows[0][col].Background = cursorStyle.Background
			}
		}
		if state.EditMode && !element.hasSelection() {
			cursorCol := clampInt(element.Cursor-element.colScrollOffset, 0, width-1)
			rows[0][cursorCol].Foreground = cursorStyle.Color
			rows[0][cursorCol].Background = cursorStyle.Background
		}
		return rows
	}

	visualRows := buildWrappedTextRows(element.Value, width)
	cursorRow := visualRowForCursor(visualRows, width, element.Cursor)
	element.rowScrollOffset = clampInt(element.rowScrollOffset, 0, maxInt(0, len(visualRows)-height))
	if state.EditMode {
		if cursorRow < element.rowScrollOffset {
			element.rowScrollOffset = cursorRow
		} else if cursorRow >= element.rowScrollOffset+height {
			element.rowScrollOffset = cursorRow - height + 1
		}
	}
	hasAbove := element.rowScrollOffset > 0
	hasBelow := element.rowScrollOffset+height < len(visualRows)
	for row := 0; row < height; row++ {
		rowIndex := element.rowScrollOffset + row
		visualRow := wrappedTextRow{}
		if rowIndex < len(visualRows) {
			visualRow = visualRows[rowIndex]
		}
		rows[row] = renderLine(visualRow.Text, width, style)
		lineRunes := []rune(visualRow.Text)
		cursorCol := -1
		if state.EditMode && rowIndex < len(visualRows) {
			cursorCol = visualColumnForCursor(visualRow, element.Cursor, width)
		}
		indicatorVisible := width > 0 && len(lineRunes) < width && ((row == 0 && hasAbove) || (row == height-1 && hasBelow))
		if indicatorVisible && cursorCol != width-1 {
			indicator := "v"
			if row == 0 && hasAbove {
				indicator = "^"
			}
			rows[row][width-1].Text = indicator
		}
		for col := 0; col < minInt(width, len(lineRunes)); col++ {
			source := visualRow.Start + col
			if state.EditMode && element.hasSelection() && source >= startSelection && source < endSelection {
				rows[row][col].Foreground = cursorStyle.Color
				rows[row][col].Background = cursorStyle.Background
			}
		}
		if state.EditMode && !element.hasSelection() && cursorCol >= 0 && cursorCol < width {
			rows[row][cursorCol].Foreground = cursorStyle.Color
			rows[row][cursorCol].Background = cursorStyle.Background
		}
	}
	return rows
}

type TextArea struct {
	TextInput
}

func (element *TextArea) ElementType() string {
	return "textarea"
}

type NumberInput struct {
	elementBase
	NumberValue float64
	StepSize    float64
	editText    string
	editCursor  int
	editing     bool
}

func NewNumberInput(name string, value float64) *NumberInput {
	return &NumberInput{
		elementBase: newElementBase(name, fmt.Sprintf("%g", value), true),
		NumberValue: value,
		StepSize:    1.0,
	}
}

func (element *NumberInput) ElementType() string {
	return "numberinput"
}

func (element *NumberInput) SetValue(value float64) {
	element.NumberValue = value
	element.Value = fmt.Sprintf("%g", value)
	element.Text = element.Value
	element.editText = ""
	element.editing = false
}

func (element *NumberInput) SetNumberValue(value float64) {
	element.SetValue(value)
}

func (element *NumberInput) BeginEdit() {
	if element.editing {
		return
	}
	element.editText = fmt.Sprintf("%g", element.NumberValue)
	element.editCursor = len([]rune(element.editText))
	element.editing = true
}

func (element *NumberInput) CommitEdit() {
	element.BeginEdit()
	if parsed, err := strconv.ParseFloat(element.editText, 64); err == nil {
		element.NumberValue = parsed
	}
	element.Value = fmt.Sprintf("%g", element.NumberValue)
	element.Text = element.Value
	element.editText = ""
	element.editCursor = 0
	element.editing = false
}

func (element *NumberInput) HandleKey(key string) bool {
	element.BeginEdit()
	runes := []rune(element.editText)
	switch key {
	case "Up":
		element.NumberValue += element.StepSize
		element.editText = fmt.Sprintf("%g", element.NumberValue)
		element.editCursor = len([]rune(element.editText))
		return true
	case "Down":
		element.NumberValue -= element.StepSize
		element.editText = fmt.Sprintf("%g", element.NumberValue)
		element.editCursor = len([]rune(element.editText))
		return true
	case "Left":
		element.editCursor = clampInt(element.editCursor-1, 0, len(runes))
		return true
	case "Right":
		element.editCursor = clampInt(element.editCursor+1, 0, len(runes))
		return true
	case "Backspace":
		if element.editCursor > 0 {
			runes = append(runes[:element.editCursor-1], runes[element.editCursor:]...)
			element.editCursor--
			element.editText = string(runes)
		}
		return true
	case "Delete":
		if element.editCursor < len(runes) {
			runes = append(runes[:element.editCursor], runes[element.editCursor+1:]...)
			element.editText = string(runes)
		}
		return true
	case "Enter":
		return false
	default:
		if len([]rune(key)) == 1 && strings.Contains("0123456789.-+", key) {
			runes = append(runes[:element.editCursor], append([]rune(key), runes[element.editCursor:]...)...)
			element.editCursor++
			element.editText = string(runes)
			return true
		}
		return false
	}
}

func (element *NumberInput) Render(size Size, state ElementRenderState) [][]TerminalCell {
	text := element.Value
	if state.EditMode {
		element.BeginEdit()
		text = element.editText
	}
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	rows := RenderPlainText(text, size.Width, 1, style)
	if state.EditMode {
		cursorStyle := style.Clone()
		if element.cursorStyle != nil {
			cursorStyle.Merge(*element.cursorStyle)
		}
		col := clampInt(element.editCursor, 0, maxInt(0, size.Width-1))
		rows[0][col].Foreground = cursorStyle.Color
		rows[0][col].Background = cursorStyle.Background
	}
	return rows
}

type Button struct {
	elementBase
	Title string
}

func NewButton(name string, title string) *Button {
	return &Button{elementBase: newElementBase(name, title, true), Title: title}
}

func (element *Button) ElementType() string {
	return "button"
}

func (element *Button) SetTitle(title string) {
	element.Title = title
	element.SetText(title)
}

func (element *Button) Render(size Size, state ElementRenderState) [][]TerminalCell {
	width := maxInt(minimumRenderableSize, size.Width)
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	body := element.Title
	titleWidth := len([]rune(element.Title))
	if state.Focused && width >= titleWidth+2 {
		if width >= titleWidth+6 {
			body = " [" + alignText(" "+element.Title+" ", width-4, "center") + "] "
		} else if width >= titleWidth+4 {
			body = " [" + alignText(element.Title, width-4, "center") + "] "
		} else {
			body = "[" + element.Title + "]"
		}
	}
	return RenderPlainText(alignText(body, width, "center"), width, 1, style)
}

type CheckBox struct {
	elementBase
	Checked bool
}

func NewCheckBox(name string, title string, checked bool) *CheckBox {
	return &CheckBox{elementBase: newElementBase(name, title, true), Checked: checked}
}

func (element *CheckBox) ElementType() string {
	return "checkbox"
}

func (element *CheckBox) SetChecked(checked bool) {
	element.Checked = checked
}

func (element *CheckBox) HandleKey(key string) bool {
	if key != "Enter" && key != " " {
		return false
	}
	element.Checked = !element.Checked
	return true
}

func (element *CheckBox) Render(size Size, state ElementRenderState) [][]TerminalCell {
	indicator := "[ ]"
	if element.Checked {
		indicator = "[x]"
	}
	text := indicator
	if element.Text != "" {
		text += " " + element.Text
	}
	width := maxInt(minimumRenderableSize, size.Width)
	base := element.EffectiveStyle(state.Focused, state.EditMode)
	rows := RenderPlainText(text, width, 1, base)
	indicatorStyle := base
	if element.Checked {
		indicatorStyle = mergedCheckBoxIndicatorStyle(base, element.checkedStyle, state.Focused || state.EditMode)
	} else {
		indicatorStyle = mergedCheckBoxIndicatorStyle(base, element.uncheckedStyle, state.Focused || state.EditMode)
	}
	for index := 0; index < minInt(len([]rune(indicator)), width); index++ {
		rows[0][index].Foreground = indicatorStyle.Color
		rows[0][index].Background = indicatorStyle.Background
	}
	return rows
}

func mergedCheckBoxIndicatorStyle(base Style, override *Style, focusedOrEditing bool) Style {
	inheritedBackground := base.Background
	if override != nil {
		base.Merge(*override)
	}
	if focusedOrEditing &&
		!base.Background.Empty() &&
		base.Background.Transparent() &&
		!inheritedBackground.Empty() {
		base.Background = inheritedBackground
	}
	return base
}

type ComboBox struct {
	elementBase
	Options       []string
	SelectedIndex int
}

func NewComboBox(name string, options []string) *ComboBox {
	selected := -1
	value := ""
	if len(options) > 0 {
		selected = 0
		value = options[0]
	}
	return &ComboBox{
		elementBase:   newElementBase(name, value, true),
		Options:       append([]string{}, options...),
		SelectedIndex: selected,
	}
}

func (element *ComboBox) ElementType() string {
	return "combobox"
}

func (element *ComboBox) SetSelectedItem(value string) {
	for index, option := range element.Options {
		if option == value {
			element.SetSelectedIndex(index)
			return
		}
	}
}

func (element *ComboBox) SetSelectedIndex(index int) {
	if len(element.Options) == 0 {
		element.SelectedIndex = -1
		element.Value = ""
		element.Text = ""
		return
	}
	index = clampInt(index, 0, len(element.Options)-1)
	element.SelectedIndex = index
	element.Value = element.Options[index]
	element.Text = element.Value
}

func (element *ComboBox) HandleKey(key string) bool {
	if len(element.Options) == 0 {
		return false
	}
	switch key {
	case "Up":
		element.SetSelectedIndex(element.SelectedIndex - 1)
		return true
	case "Down":
		element.SetSelectedIndex(element.SelectedIndex + 1)
		return true
	default:
		return false
	}
}

func (element *ComboBox) Render(size Size, state ElementRenderState) [][]TerminalCell {
	width := maxInt(minimumRenderableSize, size.Width)
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	value := element.Value
	if value == "" && element.SelectedIndex >= 0 && element.SelectedIndex < len(element.Options) {
		value = element.Options[element.SelectedIndex]
	}
	closed := value
	if state.Focused || state.EditMode {
		if state.EditMode {
			closed = fitTextWithIndicator(closed, width, "v")
		} else {
			closed = fitTextWithIndicator(closed, width, ">")
		}
	}
	rows := [][]TerminalCell{renderLine(closed, width, style)}
	if state.EditMode {
		selectedStyle := style.Clone()
		if element.selectedStyle != nil {
			selectedStyle.Merge(*element.selectedStyle)
		}
		for index := 0; index < len(element.Options); index++ {
			rowStyle := style
			if index == element.SelectedIndex {
				rowStyle = selectedStyle
			}
			rows = append(rows, renderLine("  "+element.Options[index], width, rowStyle))
		}
	}
	return rows
}

type ListBox struct {
	elementBase
	Options           []string
	Multi             bool
	Selected          map[int]bool
	SelectedIndex     int
	ActiveIndex       int
	ScrollOffset      int
	ActiveItemVisible bool
	DisabledValues    []string
	lastViewportRows  int
}

func NewListBox(name string, options []string, multi bool) *ListBox {
	selected := -1
	if len(options) > 0 {
		selected = 0
	}
	selection := map[int]bool{}
	if selected >= 0 {
		selection[selected] = true
	}
	return &ListBox{
		elementBase:   newElementBase(name, "", true),
		Options:       append([]string{}, options...),
		Multi:         multi,
		Selected:      selection,
		SelectedIndex: selected,
		ActiveIndex:   maxInt(0, selected),
	}
}

func (element *ListBox) ElementType() string {
	return "listbox"
}

func (element *ListBox) SetOptions(options []string) {
	element.Options = append([]string{}, options...)
	element.Selected = map[int]bool{}
	if len(element.Options) == 0 {
		element.SelectedIndex = -1
		element.ActiveIndex = 0
		element.ScrollOffset = 0
		element.ActiveItemVisible = false
		return
	}
	element.SelectedIndex = clampInt(element.SelectedIndex, 0, len(element.Options)-1)
	element.ActiveIndex = clampInt(element.ActiveIndex, 0, len(element.Options)-1)
	element.Selected[element.SelectedIndex] = true
	element.ScrollOffset = clampInt(element.ScrollOffset, 0, maxInt(0, len(element.Options)-maxInt(minimumRenderableSize, element.lastViewportRows)))
}

func (element *ListBox) SetDisabledValues(values []string) {
	element.DisabledValues = append([]string{}, values...)
}

func (element *ListBox) SetSelectedItems(values []string) {
	element.Selected = map[int]bool{}
	for _, value := range values {
		for index, option := range element.Options {
			if option == value {
				element.Selected[index] = true
				element.SelectedIndex = index
				element.ActiveIndex = index
				if !element.Multi {
					break
				}
			}
		}
	}
	if len(element.Selected) == 0 && len(element.Options) > 0 {
		element.SelectedIndex = clampInt(element.SelectedIndex, 0, len(element.Options)-1)
		element.ActiveIndex = element.SelectedIndex
	}
	if element.lastViewportRows > 0 {
		element.ensureSelectedVisible(element.lastViewportRows)
	}
}

func (element *ListBox) SetSelectedIndex(index int) {
	if len(element.Options) == 0 {
		element.Selected = map[int]bool{}
		element.SelectedIndex = -1
		element.ActiveIndex = 0
		return
	}
	index = clampInt(index, 0, len(element.Options)-1)
	element.Selected = map[int]bool{index: true}
	element.SelectedIndex = index
	element.ActiveIndex = index
	if element.lastViewportRows > 0 {
		element.ensureSelectedVisible(element.lastViewportRows)
	}
}

func (element *ListBox) SetMultiple(multiple bool) {
	element.Multi = multiple
	if !element.Multi && len(element.Selected) > 1 {
		element.SetSelectedIndex(element.SelectedIndex)
	}
}

func (element *ListBox) ScrollBy(delta int, viewportHeight int) {
	if len(element.Options) == 0 {
		element.ScrollOffset = 0
		return
	}
	maxOffset := maxInt(0, len(element.Options)-maxInt(minimumRenderableSize, viewportHeight))
	element.ScrollOffset = clampInt(element.ScrollOffset+delta, 0, maxOffset)
}

func (element *ListBox) SelectedValues() []string {
	var result []string
	for index, option := range element.Options {
		if element.Selected[index] {
			result = append(result, option)
		}
	}
	return result
}

func (element *ListBox) HandleKey(key string) bool {
	if len(element.Options) == 0 {
		return false
	}
	switch key {
	case "Up":
		element.setActiveIndex(element.ActiveIndex - 1)
		element.ActiveItemVisible = true
		return true
	case "Down":
		element.setActiveIndex(element.ActiveIndex + 1)
		element.ActiveItemVisible = true
		return true
	case "Enter":
		if element.Multi {
			element.Selected[element.ActiveIndex] = !element.Selected[element.ActiveIndex]
			return true
		}
		element.SetSelectedIndex(element.ActiveIndex)
		element.ActiveItemVisible = false
		return true
	default:
		return false
	}
}

func (element *ListBox) Render(size Size, state ElementRenderState) [][]TerminalCell {
	width := maxInt(minimumRenderableSize, size.Width)
	height := maxInt(minimumRenderableSize, size.Height)
	element.lastViewportRows = height
	maxOffset := maxInt(0, len(element.Options)-height)
	element.ScrollOffset = clampInt(element.ScrollOffset, 0, maxOffset)
	if state.EditMode && element.ActiveItemVisible {
		element.ensureIndexVisible(element.ActiveIndex, height)
	} else {
		element.ensureSelectedVisible(height)
	}
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	rows := RenderPlainText("", width, height, style)
	hasAbove := element.ScrollOffset > 0
	hasBelow := element.ScrollOffset+height < len(element.Options)
	for row := 0; row < height; row++ {
		optionIndex := element.ScrollOffset + row
		rowStyle := style
		text := ""
		if optionIndex < len(element.Options) {
			text = element.Options[optionIndex]
		}
		if row == 0 && hasAbove && width > 0 {
			text = fitTextWithIndicator(text, width, "^")
		} else if row == height-1 && hasBelow && width > 0 {
			text = fitTextWithIndicator(text, width, "v")
		}
		if element.Selected[optionIndex] {
			rowStyle = mergedListBoxRowLayer(rowStyle, element.selectedStyle)
		}
		active := optionIndex < len(element.Options) &&
			state.EditMode &&
			element.ActiveItemVisible &&
			optionIndex == element.ActiveIndex
		if active {
			if element.activeStyle != nil {
				rowStyle = mergedListBoxRowLayer(rowStyle, element.activeStyle)
			} else if !element.Selected[optionIndex] {
				rowStyle = mergedListBoxRowLayer(rowStyle, element.selectedStyle)
			}
		}
		if optionIndex < len(element.Options) && element.optionDisabled(element.Options[optionIndex]) && element.disabledStyle != nil {
			rowStyle = style
			rowStyle.Merge(*element.disabledStyle)
		}
		rows[row] = renderLine(text, width, rowStyle)
	}
	return rows
}

func (element *ListBox) optionDisabled(value string) bool {
	for _, disabled := range element.DisabledValues {
		if disabled == value {
			return true
		}
	}
	return false
}

func (element *ListBox) setActiveIndex(index int) {
	if len(element.Options) == 0 {
		element.SelectedIndex = 0
		element.ActiveIndex = 0
		element.ScrollOffset = 0
		return
	}
	element.ActiveIndex = clampInt(index, 0, len(element.Options)-1)
	if element.lastViewportRows > 0 {
		element.ensureIndexVisible(element.ActiveIndex, element.lastViewportRows)
	}
}

func (element *ListBox) ensureIndexVisible(index int, height int) {
	if height <= 0 || index < 0 {
		return
	}
	if index < element.ScrollOffset {
		element.ScrollOffset = index
	} else if index >= element.ScrollOffset+height {
		element.ScrollOffset = index - height + 1
	}
}

func (element *ListBox) ensureSelectedVisible(height int) {
	element.ensureIndexVisible(element.SelectedIndex, height)
}

func mergedListBoxRowLayer(base Style, override *Style) Style {
	if override == nil {
		return base
	}
	inheritedBackground := base.Background
	base.Merge(*override)
	if !override.Background.Empty() &&
		override.Background.rgba != nil &&
		override.Background.rgba.alpha < 255 &&
		!inheritedBackground.Empty() {
		base.Background = override.Background.BlendOver(inheritedBackground)
	}
	return base
}

func fitTextWithIndicator(text string, width int, indicator string) string {
	runes := []rune(text)
	if len(runes) < width {
		padding := make([]rune, width-len(runes))
		for index := range padding {
			padding[index] = ' '
		}
		runes = append(runes, padding...)
	}
	if len(runes) > width {
		runes = runes[:width]
	}
	runes[width-1] = []rune(indicator)[0]
	return string(runes)
}

type ScrollView struct {
	elementBase
	Gap                  int
	Children             []Element
	ViewOffset           int
	AutoScroll           bool
	descendantFocusStyle *Style
	lastSize             Size
	visibleSize          Size
}

func NewScrollView(name string, gap int) *ScrollView {
	return &ScrollView{elementBase: newElementBase(name, "", true), Gap: gap}
}

func (element *ScrollView) ElementType() string {
	return "uiscrollview"
}

func (element *ScrollView) AddChild(child Element) Element {
	element.Children = append(element.Children, child)
	if element.AutoScroll {
		element.ScrollToBottom()
	}
	return child
}

func (element *ScrollView) ClearChildren() {
	element.Children = nil
	element.ViewOffset = 0
}

func (element *ScrollView) SetAutoScroll(enabled bool) {
	element.AutoScroll = enabled
	if enabled {
		element.ScrollToBottom()
	}
}

func (element *ScrollView) SetDescendantFocusStyle(style Style) {
	element.descendantFocusStyle = &style
}

func (element *ScrollView) ScrollToBottom() bool {
	before := element.ViewOffset
	element.ViewOffset = maxInt(0, element.contentHeight()-maxInt(minimumRenderableSize, element.viewportHeight()))
	element.AutoScroll = true
	return element.ViewOffset != before
}

func (element *ScrollView) ScrollToTop() bool {
	before := element.ViewOffset
	element.ViewOffset = 0
	element.AutoScroll = false
	return element.ViewOffset != before
}

func (element *ScrollView) ScrollBy(delta int) bool {
	before := element.ViewOffset
	viewportHeight := maxInt(minimumRenderableSize, element.viewportHeight())
	element.ViewOffset = clampInt(element.ViewOffset+delta, 0, maxInt(0, element.contentHeight()-viewportHeight))
	element.AutoScroll = element.ViewOffset == maxInt(0, element.contentHeight()-viewportHeight)
	return element.ViewOffset != before
}

func (element *ScrollView) HandleKey(key string) bool {
	scrollY := true
	if element.style.ScrollY != nil {
		scrollY = *element.style.ScrollY
	}
	if key == "Alt+Up" || key == "Ctrl+Up" {
		return element.ScrollBy(-maxInt(minimumRenderableSize, element.viewportHeight()))
	}
	if key == "Alt+Down" || key == "Ctrl+Down" {
		return element.ScrollBy(maxInt(minimumRenderableSize, element.viewportHeight()))
	}
	if scrollY {
		switch key {
		case "ArrowUp", "Up":
			return element.ScrollBy(-1)
		case "ArrowDown", "Down":
			return element.ScrollBy(1)
		case "PageUp":
			return element.ScrollBy(-maxInt(minimumRenderableSize, element.viewportHeight()))
		case "PageDown":
			return element.ScrollBy(maxInt(minimumRenderableSize, element.viewportHeight()))
		case "Home":
			return element.ScrollToTop()
		case "End":
			return element.ScrollToBottom()
		}
	}
	return false
}

func (element *ScrollView) contentHeight() int {
	if len(element.Children) == 0 {
		return 0
	}
	width := element.lastSize.Width
	if width <= 0 {
		width = element.frame.Width
	}
	height := 0
	for index, child := range element.Children {
		if index > 0 {
			height += element.Gap
		}
		height += scrollViewChildHeight(child, width)
	}
	return height
}

func scrollViewChildHeight(child Element, width int) int {
	if child == nil {
		return minimumRenderableSize
	}
	if provider, ok := child.(preferredHeightElement); ok {
		return maxInt(minimumRenderableSize, provider.PreferredHeight(maxInt(minimumRenderableSize, width)))
	}
	return maxInt(minimumRenderableSize, child.ElementFrame().Height)
}

func (element *ScrollView) Render(size Size, state ElementRenderState) [][]TerminalCell {
	element.lastSize = size
	style := element.EffectiveStyle(state.Focused, state.EditMode)
	rows := RenderPlainText("", size.Width, size.Height, style)
	contentHeight := element.contentHeight()
	viewportHeight := maxInt(minimumRenderableSize, element.viewportHeight())
	element.ViewOffset = clampInt(element.ViewOffset, 0, maxInt(0, contentHeight-viewportHeight))
	cursor := 0
	var deferredOverflow []scrollViewOverflowRender
	for _, child := range element.Children {
		childHeight := scrollViewChildHeight(child, size.Width)
		childState := ElementRenderState{
			FocusedElement:       state.FocusedElement,
			EditElement:          state.EditElement,
			SuppressFocusVisuals: state.SuppressFocusVisuals,
		}
		if child == state.FocusedElement {
			childState.Focused = true
			childState.EditMode = state.EditMode && state.EditElement == child
		} else if childHasFocusedDescendant(child, state.FocusedElement) {
			childState.Focused = true
			childState.ChildEditMode = state.EditMode || state.ChildEditMode
		}
		rendered := child.Render(Size{Width: size.Width, Height: childHeight}, childState)
		visibleRows := minInt(childHeight, len(rendered))
		for row := 0; row < visibleRows; row++ {
			targetRow := cursor + row - element.ViewOffset
			if targetRow < 0 {
				continue
			}
			if targetRow >= viewportHeight {
				break
			}
			rows[targetRow] = rendered[row]
		}
		if len(rendered) > childHeight {
			deferredOverflow = append(deferredOverflow, scrollViewOverflowRender{
				startRow: cursor + childHeight,
				rows:     rendered[childHeight:],
			})
		}
		cursor += childHeight + element.Gap
		if cursor-element.ViewOffset >= viewportHeight {
			break
		}
	}
	for _, overflow := range deferredOverflow {
		for row, renderedRow := range overflow.rows {
			targetRow := overflow.startRow + row - element.ViewOffset
			if targetRow < 0 {
				continue
			}
			if targetRow >= viewportHeight {
				break
			}
			overlayBase := Color{}
			if len(renderedRow) > 0 {
				overlayBase = renderedRow[0].Background
			}
			for col := 0; col < minInt(len(rows[targetRow]), len(renderedRow)); col++ {
				if shouldSkipOverflowCell(renderedRow[col], overlayBase) {
					continue
				}
				rows[targetRow][col] = renderedRow[col]
			}
		}
	}
	if !state.SuppressScrollIndicators && size.Width > 0 && viewportHeight > 0 {
		if element.ViewOffset > 0 {
			applyScrollIndicatorToRow(rows[0], scrollIndicatorUp)
		}
		if element.ViewOffset+viewportHeight < contentHeight {
			applyScrollIndicatorToRow(rows[viewportHeight-1], scrollIndicatorDown)
		}
	}
	return rows
}

type scrollViewOverflowRender struct {
	startRow int
	rows     [][]TerminalCell
}

func shouldSkipOverflowCell(cell TerminalCell, baseBackground Color) bool {
	if cell.Text != "" && cell.Text != " " {
		return false
	}
	if !cell.Foreground.Empty() && !cell.Foreground.Transparent() {
		return false
	}
	return !baseBackground.Empty() && sameRenderedColor(cell.Background, baseBackground)
}

func childHasFocusedDescendant(child Element, focused Element) bool {
	if child == nil || focused == nil {
		return false
	}
	if nested, ok := child.(*ScrollView); ok && scrollViewContainsElement(nested, focused) {
		return true
	}
	if childWindow := childWindowForElement(child); childWindow != nil && elementInWindow(childWindow, focused) {
		return true
	}
	return false
}

func applyScrollIndicatorToRow(row []TerminalCell, indicator string) {
	if len(row) == 0 {
		return
	}
	index := len(row) - 1
	row[index].Text = indicator
	if row[index].Foreground.Empty() || row[index].Foreground.Transparent() {
		row[index].Foreground = inferredScrollIndicatorForeground(row, index)
	}
}

func inferredScrollIndicatorForeground(row []TerminalCell, end int) Color {
	foreground := Color{}
	end = minInt(maxInt(0, end), len(row)-1)
	for index := 0; index <= end; index++ {
		if !row[index].Foreground.Empty() && !row[index].Foreground.Transparent() {
			foreground = row[index].Foreground
		}
	}
	return foreground
}

func (element *ScrollView) viewportHeight() int {
	if element.visibleSize.Height > 0 {
		return element.visibleSize.Height
	}
	if element.lastSize.Height > 0 {
		return element.lastSize.Height
	}
	return element.frame.Height
}

type ReusableElement struct {
	elementBase
	TypeName    string
	Child       *GeneratedWindowBase
	ChildObject any
}

func NewReusableElement(name string, typeName string) *ReusableElement {
	return &ReusableElement{elementBase: newElementBase(name, "", true), TypeName: typeName}
}

func (element *ReusableElement) ElementType() string {
	return "uielement"
}

type ViewHost struct {
	ReusableElement
}

func NewViewHost(name string) *ViewHost {
	return &ViewHost{ReusableElement: *NewReusableElement(name, "viewhost")}
}

func (element *ViewHost) ElementType() string {
	return "viewhost"
}

func (element *ViewHost) SetView(child any) {
	element.SetChild(child)
}

func (element *ViewHost) ClearView() {
	element.SetChild(nil)
}

func (element *ViewHost) CurrentView() any {
	return element.ChildObject
}

func (element *ReusableElement) SetChild(child any) {
	base, ok := extractGeneratedWindowBase(child)
	if !ok {
		element.Child = nil
		element.ChildObject = nil
		return
	}
	element.Child = base
	element.ChildObject = child
	base.ownerObject = child
}

func (element *ReusableElement) PreferredHeight(width int) int {
	if element.Child == nil {
		return maxInt(minimumRenderableSize, element.frame.Height)
	}
	resolved := GeneratedWindowContentSizeForWidth(element.Child, maxInt(minimumRenderableSize, width))
	return maxInt(minimumRenderableSize, resolved.Height)
}

func (element *ReusableElement) Render(size Size, state ElementRenderState) [][]TerminalCell {
	if element.Child == nil {
		return RenderPlainText("", size.Width, size.Height, element.EffectiveStyle(state.Focused, state.EditMode))
	}
	var childFocused Element
	if elementInWindow(element.Child, state.FocusedElement) {
		childFocused = state.FocusedElement
	}
	childEditMode := (state.EditMode || state.ChildEditMode) && childFocused != nil
	var childEditElement Element
	if state.EditElement != nil && elementInWindow(element.Child, state.EditElement) {
		childEditElement = state.EditElement
	}
	renderHeight := size.Height
	if childEditMode {
		if comboBox, ok := childEditElement.(*ComboBox); ok && childEditElement == childFocused && directElementInWindow(element.Child, childFocused) {
			renderHeight += len(comboBox.Options)
		}
	}
	var childScrollView *ScrollView
	if !state.SuppressFocusVisuals {
		childScrollView = generatedScrollViewForReusableChild(element.Child)
	}
	childFocusActive := !state.SuppressFocusVisuals && (state.Focused || childEditMode || childEditElement != nil || (childScrollView != nil && childFocused == childScrollView))
	suppressChildFocusVisuals := state.SuppressFocusVisuals
	if childScrollView != nil && (childFocused == childScrollView || scrollViewContainsElement(childScrollView, childFocused)) {
		suppressChildFocusVisuals = true
	}
	applyReusableFocusStyle := childFocusActive && reusableFocusStyleAppliesToChild(element, state.Focused, childFocusActive && !state.Focused)
	childWindowFocusStyleApplied := false
	previousChildWindowStyle := element.Child.windowStyle
	if applyReusableFocusStyle &&
		childScrollView == nil &&
		element.focusStyle != nil &&
		!element.focusStyle.Background.Empty() {
		childWindowStyle := previousChildWindowStyle.Clone()
		childWindowStyle.Merge(*element.focusStyle)
		element.Child.windowStyle = childWindowStyle
		childWindowFocusStyleApplied = true
	}
	renderSize := Size{Width: size.Width, Height: renderHeight}
	childRows := renderGeneratedWindowContentWithEditElementOptions(element.Child, renderSize, -1, childFocused, childEditMode, true, suppressChildFocusVisuals, childEditElement, true, true).cells
	if childWindowFocusStyleApplied {
		element.Child.windowStyle = previousChildWindowStyle
	}
	baseFocused := state.Focused
	if childScrollView != nil && childFocusActive {
		baseFocused = false
	}
	focusBaseBackground := element.style.Background
	if focusBaseBackground.Empty() && effectiveStyleParentBackgroundSet {
		focusBaseBackground = effectiveStyleParentBackground
	}
	rows := RenderPlainText("", size.Width, maxInt(size.Height, len(childRows)), element.EffectiveStyle(baseFocused, state.EditMode))
	for row := 0; row < minInt(len(rows), len(childRows)); row++ {
		for col := 0; col < minInt(len(rows[row]), len(childRows[row])); col++ {
			cell := childRows[row][col].Clone()
			base := rows[row][col]
			cell.Background = blendBackgroundOverExisting(cell.Background, base.Background)
			cell.Foreground = blendForegroundOverBackground(cell.Foreground, base.Foreground, cell.Background)
			rows[row][col] = cell
		}
	}
	if childFocusActive {
		childFocusApplied := false
		if applyReusableFocusStyle && element.focusStyle != nil && !element.focusStyle.Background.Empty() {
			descendantBackgrounds := collectDescendantBaseStyleBackgrounds(element.Child)
			protectedBackgrounds := collectDescendantReusableProtectedBackgrounds(element.Child, descendantBackgrounds)
			if childScrollView != nil {
				applyReusableFocusBackgroundToRootScrollViewGaps(
					rows,
					childScrollView,
					element.focusStyle.Background,
					focusBaseBackground,
					protectedBackgrounds,
					true,
					true)
			} else if !childWindowFocusStyleApplied {
				applyReusableFocusBackgroundToDescendantBackgrounds(
					rows,
					element.focusStyle.Background,
					descendantBackgrounds)
				applyReusableFocusBackground(rows, element.focusStyle.Background, focusBaseBackground)
			}
			childFocusApplied = true
		}
		if !childFocusApplied &&
			childScrollView != nil &&
			!(state.Focused && state.EditMode && childFocused == nil) &&
			childScrollView.focusStyle != nil &&
			!childScrollView.focusStyle.Background.Empty() {
			descendantBackgrounds := collectDescendantBaseStyleBackgrounds(element.Child)
			protectedBackgrounds := collectDescendantReusableProtectedBackgrounds(element.Child, descendantBackgrounds)
			applyReusableFocusBackgroundToRootScrollViewGaps(
				rows,
				childScrollView,
				childScrollView.focusStyle.Background,
				focusBaseBackground,
				protectedBackgrounds,
				true,
				false)
			childFocusApplied = true
		}
	}
	return rows
}

func reusableFocusStyleAppliesToChild(element *ReusableElement, directFocus bool, descendantOnlyFocus bool) bool {
	if element == nil || element.focusStyle == nil {
		return false
	}
	if !hasPartialAlpha(element.focusStyle.Background) {
		return true
	}
	if element.Child != nil && generatedScrollViewForReusableChild(element.Child) != nil {
		return false
	}
	return directFocus || !descendantOnlyFocus
}

func generatedScrollViewForReusableChild(window *GeneratedWindowBase) *ScrollView {
	if window == nil || !window.scrollViewRoot {
		return nil
	}
	return firstScrollViewInWindow(window)
}

func applyReusableFocusBackground(rows [][]TerminalCell, focusBackground Color, baseBackground Color) {
	focusedBaseBackground := Color{}
	if !baseBackground.Empty() {
		focusedBaseBackground = reusableFocusedBackground(focusBackground, baseBackground)
	}
	for row := range rows {
		for col := range rows[row] {
			cell := rows[row][col]
			matchesBase := cell.Background.Empty() ||
				(!baseBackground.Empty() && sameRenderedColor(cell.Background, baseBackground))
			matchesFocusedBase := !focusedBaseBackground.Empty() &&
				sameRenderedColor(cell.Background, focusedBaseBackground)
			if !matchesBase && !matchesFocusedBase {
				continue
			}
			if matchesFocusedBase {
				continue
			}
			cell.Background = reusableFocusedBackground(focusBackground, cell.Background)
			rows[row][col] = cell
		}
	}
}

type styleBackgroundCollector interface {
	collectStyleBackgrounds(*[]Color)
}

func collectDescendantBaseStyleBackgrounds(window *GeneratedWindowBase) []Color {
	var backgrounds []Color
	if window == nil {
		return backgrounds
	}
	for _, element := range window.elements {
		collectDescendantElementBackgrounds(element, &backgrounds)
	}
	return backgrounds
}

func collectDescendantElementBackgrounds(element Element, backgrounds *[]Color) {
	if element == nil {
		return
	}
	if collector, ok := element.(styleBackgroundCollector); ok {
		collector.collectStyleBackgrounds(backgrounds)
	}
	if reusable, ok := element.(*ReusableElement); ok && reusable.Child != nil {
		for _, childElement := range reusable.Child.elements {
			collectDescendantElementBackgrounds(childElement, backgrounds)
		}
	}
	if scrollView, ok := element.(*ScrollView); ok {
		for _, child := range scrollView.Children {
			collectDescendantElementBackgrounds(child, backgrounds)
		}
	}
}

func collectDescendantReusableProtectedBackgrounds(window *GeneratedWindowBase, backgrounds []Color) []Color {
	protected := append([]Color{}, backgrounds...)
	collectDescendantLayoutCellBackgrounds(window, &protected)
	return protected
}

func collectDescendantLayoutCellBackgrounds(window *GeneratedWindowBase, backgrounds *[]Color) {
	if window == nil {
		return
	}
	for _, item := range window.layout {
		appendUniqueOpaqueBackground(backgrounds, item.CellStyle.Background)
	}
	for _, element := range window.elements {
		if reusable, ok := element.(*ReusableElement); ok && reusable.Child != nil {
			collectDescendantLayoutCellBackgrounds(reusable.Child, backgrounds)
		}
		if scrollView, ok := element.(*ScrollView); ok {
			for _, child := range scrollView.Children {
				collectDescendantChildLayoutCellBackgrounds(child, backgrounds)
			}
		}
	}
}

func collectDescendantChildLayoutCellBackgrounds(element Element, backgrounds *[]Color) {
	if element == nil {
		return
	}
	if reusable, ok := element.(*ReusableElement); ok && reusable.Child != nil {
		collectDescendantLayoutCellBackgrounds(reusable.Child, backgrounds)
	}
	if scrollView, ok := element.(*ScrollView); ok {
		for _, child := range scrollView.Children {
			collectDescendantChildLayoutCellBackgrounds(child, backgrounds)
		}
	}
}

func appendUniqueOpaqueBackground(backgrounds *[]Color, background Color) {
	if background.Empty() || background.rgba == nil || background.rgba.alpha != 255 {
		return
	}
	for _, existing := range *backgrounds {
		if sameRenderedColor(existing, background) {
			return
		}
	}
	*backgrounds = append(*backgrounds, background)
}

func applyReusableFocusBackgroundToDescendantBackgrounds(rows [][]TerminalCell, focusBackground Color, backgrounds []Color) {
	if len(backgrounds) == 0 {
		return
	}
	for row := range rows {
		for col := range rows[row] {
			cell := rows[row][col]
			if cell.Background.Empty() {
				continue
			}
			if !cell.Foreground.Empty() && !cell.Foreground.Transparent() {
				continue
			}
			for _, background := range backgrounds {
				focusedBackground := reusableFocusedBackground(focusBackground, background)
				if sameRenderedColor(cell.Background, background) ||
					sameRenderedColor(cell.Background, focusedBackground) {
					cell.Background = focusedBackground
					rows[row][col] = cell
					break
				}
			}
		}
	}
}

func applyReusableFocusBackgroundToRootScrollViewGaps(rows [][]TerminalCell, scrollView *ScrollView, focusBackground Color, baseBackground Color, backgrounds []Color, includeScrollIndicators bool, allowOpaqueBlankBackground bool) {
	if focusBackground.Empty() {
		return
	}
	focusedBaseBackground := Color{}
	if !baseBackground.Empty() {
		focusedBaseBackground = reusableFocusedBackground(focusBackground, baseBackground)
	}
	transparentViewportBackground := false
	protectedContentStartCol := 0
	protectedContentEndCol := -1
	if scrollView != nil && scrollView.style.Background.rgba != nil && scrollView.style.Background.rgba.alpha < colorOpaqueAlpha {
		transparentViewportBackground = true
	}
	for row := range rows {
		if transparentViewportBackground && rowHasRenderedContent(rows[row]) {
			protectedContentStartCol = clampInt(paddingLeft(scrollView.style), 0, len(rows[row]))
			protectedContentEndCol = clampInt(len(rows[row])-paddingRight(scrollView.style)-1, -1, len(rows[row])-1)
		} else {
			protectedContentStartCol = 0
			protectedContentEndCol = -1
		}
		for col := range rows[row] {
			cell := rows[row][col]
			if col >= protectedContentStartCol && col <= protectedContentEndCol {
				continue
			}
			if !includeScrollIndicators && (cell.Text == scrollIndicatorUp || cell.Text == scrollIndicatorDown) {
				continue
			}
			opaqueFocusBackground := focusBackground.rgba == nil || focusBackground.rgba.alpha == colorOpaqueAlpha
			if opaqueFocusBackground {
				if cell.Text != "" &&
					cell.Text != " " {
					continue
				}
				if !cell.Foreground.Empty() && !cell.Foreground.Transparent() {
					continue
				}
			}
			if sameRenderedColor(cell.Background, focusBackground) {
				continue
			}
			if !focusedBaseBackground.Empty() && sameRenderedColor(cell.Background, focusedBaseBackground) {
				continue
			}
			matchesBaseBackground := cell.Background.Empty() ||
				(!baseBackground.Empty() && sameRenderedColor(cell.Background, baseBackground))
			if opaqueFocusBackground && allowOpaqueBlankBackground && !matchesBaseBackground {
				if matchesDescendantReusableBackground(cell.Background, focusBackground, backgrounds) {
					continue
				}
				cell.Background = focusBackground
				rows[row][col] = cell
				continue
			}
			if !matchesBaseBackground && focusedBaseBackground.Empty() {
				continue
			}
			if !matchesBaseBackground && !sameRenderedColor(cell.Background, focusedBaseBackground) {
				continue
			}
			if !matchesBaseBackground && matchesDescendantReusableBackground(cell.Background, focusBackground, backgrounds) {
				continue
			}
			targetBackground := cell.Background
			if targetBackground.Empty() && !baseBackground.Empty() {
				targetBackground = baseBackground
			}
			cell.Background = reusableFocusedBackground(focusBackground, targetBackground)
			rows[row][col] = cell
		}
	}
}

func rowHasRenderedContent(row []TerminalCell) bool {
	for _, cell := range row {
		if cell.Raw != "" {
			return true
		}
		if cell.Text != "" &&
			cell.Text != " " &&
			cell.Text != scrollIndicatorUp &&
			cell.Text != scrollIndicatorDown {
			return true
		}
	}
	return false
}

func matchesDescendantReusableBackground(background Color, focusBackground Color, backgrounds []Color) bool {
	for _, descendantBackground := range backgrounds {
		focusedBackground := reusableFocusedBackground(focusBackground, descendantBackground)
		if sameRenderedColor(background, descendantBackground) ||
			sameRenderedColor(background, focusedBackground) {
			return true
		}
	}
	return false
}

func reusableFocusedBackground(focusBackground Color, baseBackground Color) Color {
	if focusBackground.rgba != nil && !baseBackground.Empty() {
		return focusBackground.BlendOver(baseBackground)
	}
	return focusBackground
}

func sameRenderedColor(lhs Color, rhs Color) bool {
	if lhs.Empty() || rhs.Empty() {
		return lhs.Empty() && rhs.Empty()
	}
	return lhs.String() == rhs.String()
}

func directElementInWindow(window *GeneratedWindowBase, target Element) bool {
	if window == nil || target == nil {
		return false
	}
	for _, element := range window.elements {
		if element == target {
			return true
		}
	}
	return false
}

func normalizeTextValue(value string, multiline bool) string {
	value = strings.ReplaceAll(strings.ReplaceAll(value, "\r\n", "\n"), "\r", "\n")
	if !multiline {
		value = strings.ReplaceAll(value, "\n", " ")
	}
	return value
}

type wrappedTextRow struct {
	Start int
	End   int
	Text  string
}

func buildWrappedTextRows(text string, width int) []wrappedTextRow {
	width = maxInt(minimumRenderableSize, width)
	var rows []wrappedTextRow
	textRunes := []rune(text)
	segmentStart := 0
	for segmentStart <= len(textRunes) {
		segmentEnd := segmentStart
		for segmentEnd < len(textRunes) && textRunes[segmentEnd] != '\n' {
			segmentEnd++
		}
		segment := textRunes[segmentStart:segmentEnd]
		if len(segment) == 0 {
			rows = append(rows, wrappedTextRow{Start: segmentStart, End: segmentStart})
		} else {
			offset := 0
			for offset < len(segment) {
				remaining := len(segment) - offset
				chunkSize := minInt(remaining, width)
				chunk := append([]rune{}, segment[offset:offset+chunkSize]...)
				if chunkSize < width {
					rows = append(rows, wrappedTextRow{
						Start: segmentStart + offset,
						End:   segmentStart + offset + len(chunk),
						Text:  string(chunk),
					})
					offset += chunkSize
					continue
				}
				lastSpace := -1
				for index := len(chunk); index > 1; index-- {
					if chunk[index-1] == ' ' {
						lastSpace = index - 1
						break
					}
				}
				if lastSpace > 0 {
					chunk = chunk[:lastSpace]
					rows = append(rows, wrappedTextRow{
						Start: segmentStart + offset,
						End:   segmentStart + offset + len(chunk),
						Text:  string(chunk),
					})
					offset += lastSpace + 1
				} else {
					rows = append(rows, wrappedTextRow{
						Start: segmentStart + offset,
						End:   segmentStart + offset + len(chunk),
						Text:  string(chunk),
					})
					offset += chunkSize
				}
			}
		}
		if segmentEnd >= len(textRunes) {
			break
		}
		segmentStart = segmentEnd + 1
	}
	if len(rows) == 0 {
		rows = append(rows, wrappedTextRow{})
	}
	return rows
}

func visualRowForCursor(rows []wrappedTextRow, width int, cursor int) int {
	for index, row := range rows {
		if cursor <= row.End && (len([]rune(row.Text)) < width || cursor < row.End) {
			return index
		}
	}
	return maxInt(0, len(rows)-1)
}

func visualColumnForCursor(row wrappedTextRow, cursor int, width int) int {
	if cursor < row.Start || cursor > row.End {
		return -1
	}
	textRunes := []rune(row.Text)
	for index := range textRunes {
		source := row.Start + index
		if cursor <= source {
			return index
		}
	}
	return minInt(len(textRunes), maxInt(0, width-1))
}

func lineStartForCursor(text string, cursor int) int {
	runes := []rune(text)
	cursor = clampInt(cursor, 0, len(runes))
	for index := cursor - 1; index >= 0; index-- {
		if runes[index] == '\n' {
			return index + 1
		}
	}
	return 0
}

func lineEndForCursor(text string, cursor int) int {
	runes := []rune(text)
	cursor = clampInt(cursor, 0, len(runes))
	for index := cursor; index < len(runes); index++ {
		if runes[index] == '\n' {
			return index
		}
	}
	return len(runes)
}
