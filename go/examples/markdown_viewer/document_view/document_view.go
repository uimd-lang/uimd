package document_view

import (
	"strings"

	blocks "uimd_examples/markdown_viewer/blocks"

	uimd "uimd"
)

type MarkdownBlock struct {
	Kind     string
	Text     string
	Language string
	Level    int
}

type DocumentView struct {
	*DocumentViewUI
	Panel  *uimd.ScrollView
	blocks []MarkdownBlock
}

func NewDocumentView() *DocumentView {
	view := &DocumentView{DocumentViewUI: NewDocumentViewUI()}
	view.Panel = view.NewGeneratedScrollViewElement()
	view.Panel.SetAutoScroll(false)
	view.AddElement(view.Panel)
	return view
}

func (view *DocumentView) SetBlocks(blocksList []MarkdownBlock) {
	view.blocks = append([]MarkdownBlock(nil), blocksList...)
	view.Panel.ClearChildren()
	for index, block := range view.blocks {
		child, height := view.blockChild(block)
		reusable := uimd.NewReusableElement("block", block.Kind)
		reusable.SetChild(child)
		reusable.SetFrame(uimd.Rect{Width: 1, Height: height})
		view.Panel.AddChild(reusable)
		_ = index
	}
	view.Panel.SetAutoScroll(false)
	view.Panel.ViewOffset = 0
}

func (view *DocumentView) Scroll(direction int) {
	key := "ArrowDown"
	if direction > 0 {
		key = "ArrowUp"
	}
	view.Panel.HandleKey(key)
}

func (view *DocumentView) ContentHeight() int {
	height := 0
	for index, child := range view.Panel.Children {
		if index > 0 {
			height += view.Panel.Gap
		}
		height += child.ElementFrame().Height
	}
	return height
}

func (view *DocumentView) blockChild(block MarkdownBlock) (any, int) {
	switch block.Kind {
	case "heading":
		child := blocks.NewHeading()
		child.SetHeading(block.Level, block.Text)
		return child, blocks.HeadingBlockHeight
	case "code":
		child := blocks.NewCodeBlock()
		child.SetCode(block.Language, block.Text)
		return child, blocks.CodeBlockBaseHeight + lineCount(block.Text)
	case "table":
		child := blocks.NewTableBlock()
		child.SetTable(block.Text)
		return child, blocks.TableBlockBaseHeight + lineCount(block.Text)
	default:
		child := blocks.NewParagraph()
		child.SetText(block.Text)
		return child, maxInt(blocks.ParagraphBlockHeight, lineCount(block.Text))
	}
}

func lineCount(text string) int {
	if text == "" {
		return 1
	}
	return len(strings.Split(text, "\n"))
}

func maxInt(a int, b int) int {
	if a > b {
		return a
	}
	return b
}
