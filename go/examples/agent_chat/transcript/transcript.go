package transcript

import (
	chatentry "uimd_examples/agent_chat/chat_entry"

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
	Panel *uimd.ScrollView
}

func NewTranscript() *Transcript {
	transcript := &Transcript{TranscriptUI: NewTranscriptUI()}
	transcript.Panel = transcript.NewGeneratedScrollViewElement()
	transcript.Panel.SetAutoScroll(false)
	transcript.AddElement(transcript.Panel)
	return transcript
}

func (transcript *Transcript) AppendEntry(entry Entry) {
	view := chatentry.NewChatEntryUI()
	setEntry(view, entry)
	natural := uimd.GeneratedWindowContentSize(view)
	reusable := uimd.NewReusableElement("entry", "chat_entry")
	reusable.SetChild(view)
	reusable.SetFrame(uimd.Rect{Height: maxInt(1, natural.Height)})
	transcript.Panel.AddChild(reusable)
}

func (transcript *Transcript) ReplaceLastEntry(entry Entry) {
	if len(transcript.Panel.Children) == 0 {
		transcript.AppendEntry(entry)
		return
	}
	reusable, ok := transcript.Panel.Children[len(transcript.Panel.Children)-1].(*uimd.ReusableElement)
	if !ok {
		return
	}
	view, ok := reusable.ChildObject.(*chatentry.ChatEntryUI)
	if !ok {
		return
	}
	setEntry(view, entry)
}

func (transcript *Transcript) ClearEntries() {
	transcript.Panel.ClearChildren()
	transcript.Panel.ScrollToTop()
}

func (transcript *Transcript) EntryCount() int {
	return len(transcript.Panel.Children)
}

func setEntry(view *chatentry.ChatEntryUI, entry Entry) {
	view.Role.SetText(entry.Role)
	view.Phase.SetText(entry.Phase)
	view.Body.SetText(entry.Body)
	view.Detail.SetText(entry.Detail)
}

func maxInt(left int, right int) int {
	if left > right {
		return left
	}
	return right
}
