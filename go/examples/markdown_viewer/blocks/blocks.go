package blocks

import (
	"strings"
	"unicode"

	uimd "uimd"
)

const (
	HeadingBlockHeight   = 1
	ParagraphBlockHeight = 3
	CodeBlockBaseHeight  = 2
	TableBlockBaseHeight = 2
)

var pythonKeywords = map[string]bool{
	"False": true, "None": true, "True": true, "and": true, "as": true,
	"assert": true, "async": true, "await": true, "break": true, "class": true,
	"continue": true, "def": true, "del": true, "elif": true, "else": true,
	"except": true, "finally": true, "for": true, "from": true, "global": true,
	"if": true, "import": true, "in": true, "is": true, "lambda": true,
	"nonlocal": true, "not": true, "or": true, "pass": true, "raise": true,
	"return": true, "try": true, "while": true, "with": true, "yield": true,
}

type Heading struct {
	*HeadingUI
}

func NewHeading() *Heading {
	return &Heading{HeadingUI: NewHeadingUI()}
}

func (block *Heading) SetHeading(level int, value string) {
	if level < 1 {
		level = 1
	}
	if level > 3 {
		level = 3
	}
	color := "#a7f3d0"
	if level == 1 {
		color = "#facc15"
	} else if level == 2 {
		color = "#93c5fd"
	}
	block.HeadingText.SetSpans([]uimd.LabelSpan{
		{Text: strings.Repeat("#", level), Color: "#64748b"},
		{Text: " "},
		{Text: value, Color: color},
	})
}

type Paragraph struct {
	*ParagraphUI
}

func NewParagraph() *Paragraph {
	return &Paragraph{ParagraphUI: NewParagraphUI()}
}

func (block *Paragraph) SetText(value string) {
	block.Text.SetText(value)
}

type CodeBlock struct {
	*CodeBlockUI
}

func NewCodeBlock() *CodeBlock {
	return &CodeBlock{CodeBlockUI: NewCodeBlockUI()}
}

func (block *CodeBlock) SetCode(language string, source string) {
	if language == "" {
		language = "text"
	}
	block.Lang.SetText(language)
	block.Code.SetSpans(highlightPythonKeywords(source))
}

type TableBlock struct {
	*TableBlockUI
}

func NewTableBlock() *TableBlock {
	return &TableBlock{TableBlockUI: NewTableBlockUI()}
}

func (block *TableBlock) SetTable(value string) {
	block.Table.SetText(value)
}

func highlightPythonKeywords(source string) []uimd.LabelSpan {
	spans := []uimd.LabelSpan{}
	cursor := 0
	runes := []rune(source)
	for index := 0; index < len(runes); {
		if !isIdentifierRune(runes[index]) {
			index++
			continue
		}
		start := index
		for index < len(runes) && isIdentifierRune(runes[index]) {
			index++
		}
		word := string(runes[start:index])
		if !pythonKeywords[word] {
			continue
		}
		if start > cursor {
			spans = append(spans, uimd.LabelSpan{Text: string(runes[cursor:start])})
		}
		spans = append(spans, uimd.LabelSpan{Text: word, Color: "#c084fc"})
		cursor = index
	}
	if cursor < len(runes) {
		spans = append(spans, uimd.LabelSpan{Text: string(runes[cursor:])})
	}
	if len(spans) == 0 {
		spans = append(spans, uimd.LabelSpan{Text: source})
	}
	return spans
}

func isIdentifierRune(value rune) bool {
	return value == '_' || unicode.IsLetter(value) || unicode.IsDigit(value)
}
