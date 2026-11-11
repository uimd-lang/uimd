package transcript

import (
	"strconv"
	"strings"

	chatentry "uimd_examples/agent_chat/chat_entry"
	codeentry "uimd_examples/agent_chat/code_entry"
	reasoningentry "uimd_examples/agent_chat/reasoning_entry"
	tableentry "uimd_examples/agent_chat/table_entry"
	userentry "uimd_examples/agent_chat/user_entry"

	uimd "uimd"
)

type Entry struct {
	Role   string
	Phase  string
	Body   string
	Detail string
}

type Transcript struct {
	*TranscriptUI
	Panel   *uimd.ScrollView
	entries []Entry
}

func NewTranscript() *Transcript {
	transcript := &Transcript{TranscriptUI: NewTranscriptUI()}
	transcript.Panel = transcript.NewGeneratedScrollViewElement()
	transcript.Panel.SetAutoScroll(false)
	transcript.Panel.SetVirtualDataSource(uimd.VirtualScrollDataSource{
		ItemCount: func() int {
			return len(transcript.entries)
		},
		ItemKey: func(index int) string {
			return strconv.Itoa(index)
		},
		Materialize: func(index int, recycled uimd.Element) uimd.Element {
			reusable, ok := recycled.(*uimd.ReusableElement)
			if !ok {
				reusable = uimd.NewReusableElement("entry", "")
			}
			setEntry(reusable, transcript.entries[index])
			return reusable
		},
	})
	transcript.AddElement(transcript.Panel)
	return transcript
}

func (transcript *Transcript) AppendEntry(entry Entry) {
	index := len(transcript.entries)
	transcript.entries = append(transcript.entries, entry)
	transcript.Panel.VirtualItemsInserted(index, 1)
}

func (transcript *Transcript) ReplaceLastEntry(entry Entry) {
	if len(transcript.entries) == 0 {
		transcript.AppendEntry(entry)
		return
	}
	index := len(transcript.entries) - 1
	transcript.entries[index] = entry
	transcript.Panel.VirtualItemsReplaced(index, 1)
}

func (transcript *Transcript) ReplaceEntries(entries []Entry) {
	transcript.ClearEntries()
	transcript.entries = append(transcript.entries, entries...)
	if len(transcript.entries) > 0 {
		transcript.Panel.VirtualItemsInserted(0, len(transcript.entries))
	}
}

func (transcript *Transcript) ClearEntries() {
	previousCount := len(transcript.entries)
	transcript.entries = transcript.entries[:0]
	if previousCount > 0 {
		transcript.Panel.VirtualItemsRemoved(0, previousCount)
	}
	transcript.Panel.ScrollToTop()
}

func (transcript *Transcript) EntryCount() int {
	return len(transcript.entries)
}

func setEntry(reusable *uimd.ReusableElement, entry Entry) {
	if entry.Role == "You" {
		view, ok := reusable.ChildObject.(*userentry.UserEntryUI)
		if !ok {
			view = userentry.NewUserEntryUI()
			reusable.SetChild(view)
		}
		view.Body.SetText(entry.Body)
	} else if entry.Phase == "reasoning" || entry.Phase == "status" {
		view, ok := reusable.ChildObject.(*reasoningentry.ReasoningEntryUI)
		if !ok {
			view = reasoningentry.NewReasoningEntryUI()
			reusable.SetChild(view)
		}
		view.Heading.SetText(entry.Detail)
		view.Body.SetText(entry.Body)
	} else if entry.Phase == "table" {
		view, ok := reusable.ChildObject.(*tableentry.TableEntryUI)
		if !ok {
			view = tableentry.NewTableEntryUI()
			reusable.SetChild(view)
		}
		view.Heading.SetText(entry.Detail)
		view.Table.SetText(entry.Body)
	} else if codePhases[entry.Phase] {
		view, ok := reusable.ChildObject.(*codeentry.CodeEntryUI)
		if !ok {
			view = codeentry.NewCodeEntryUI()
			reusable.SetChild(view)
		}
		view.Heading.SetText(entry.Detail)
		if entry.Phase == "code" || entry.Phase == "code · python" {
			view.Code.SetText(numberedCode(entry.Body))
		} else {
			view.Code.SetText(withoutCodeFence(entry.Body))
		}
	} else {
		view, ok := reusable.ChildObject.(*chatentry.ChatEntryUI)
		if !ok {
			view = chatentry.NewChatEntryUI()
			reusable.SetChild(view)
		}
		if entry.Role == "Tool" {
			view.Heading.SetText(entry.Detail)
		} else if entry.Role == "Agent" {
			view.Heading.SetText("Assistant")
		} else {
			view.Heading.SetText(entry.Role)
		}
		view.Body.SetText(entry.Body)
	}
}

var codePhases = map[string]bool{
	"code":            true,
	"code · python":   true,
	"diff":            true,
	"patch":           true,
	"result":          true,
	"search_code":     true,
	"structured data": true,
	"test failure":    true,
	"warning":         true,
}

func numberedCode(text string) string {
	lines := strings.Split(withoutCodeFence(text), "\n")
	for index, line := range lines {
		lines[index] = strconv.Itoa(index+1) + "  " + line
	}
	return strings.Join(lines, "\n")
}

func withoutCodeFence(text string) string {
	lines := strings.Split(text, "\n")
	if len(lines) > 0 && strings.HasPrefix(lines[0], "```") {
		lines = lines[1:]
	}
	if len(lines) > 0 && strings.HasPrefix(lines[len(lines)-1], "```") {
		lines = lines[:len(lines)-1]
	}
	return strings.Join(lines, "\n")
}
