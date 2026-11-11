// Auto-generated UI code for reasoning_entry - DO NOT EDIT MANUALLY.
package reasoning_entry

import uimd "uimd"

type ReasoningEntryUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Heading *uimd.Label
    Body *uimd.SpanLabel
}

func NewReasoningEntryUI() *ReasoningEntryUI {
    ui := &ReasoningEntryUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Agent Chat Reasoning Entry")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 74, CellCharsHeight: 5,
            CellName: "panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 72, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 4,
            Content: "heading",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 90, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#000000aa"), uimd.NewColor("#00000077"), uimd.NewColor("#00000044"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000044"), uimd.NewColor("#00000077"), uimd.NewColor("#000000aa")}},
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 74, CellCharsHeight: 5,
            CellName: "panel", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 72, CharsHeight: 4,
            MarginRight: 1, MarginBottom: 0,
            Content: "body",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Color: uimd.NewColor("#94a3b8"),
        UserSelect: "text",
    },
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#00000000"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ReasoningEntry", "# Agent Chat Reasoning Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Animated Heron-style thinking and status message.\"\ntags: [component, simulation, reasoning]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: label\n  text: \"Thinking\"\n  description: \"Animated thinking or activity phase.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Visible simulated reasoning detail.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#e5e7eb\"\n  text-color-gradient:\n    interval: 90\n    step: 1\n    segment-size: 1\n    colors: [\"#000000aa\", \"#00000077\", \"#00000044\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000044\", \"#00000077\", \"#000000aa\"]\n@body:\n  color: \"#94a3b8\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-panel-**-----------------------------------------------------------------+\n| heading..**............................................................. |\n| body..**................................................................ |\n| ........................................................................ |\n# #....................................................................... |\n# #....................................................................... |\n+--------------------------------------------------------------------------+\n```\n", "Animated Heron-style thinking and status message.")
    ui.Heading = uimd.NewLabel("heading", "Thinking")
    ui.AddElement(ui.Heading)
    ui.Heading.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#e5e7eb"),
        TextColorGradient: &uimd.TextGradient{IntervalMs: 90, Step: 1, SegmentSize: 1, Colors: []uimd.Color{uimd.NewColor("#000000aa"), uimd.NewColor("#00000077"), uimd.NewColor("#00000044"), uimd.NewColor("#00000011"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000000"), uimd.NewColor("#00000011"), uimd.NewColor("#00000044"), uimd.NewColor("#00000077"), uimd.NewColor("#000000aa")}},
    })
    ui.Heading.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Body = uimd.NewSpanLabel("body", "")
    ui.AddElement(ui.Body)
    ui.Body.SetStyle(uimd.Style{
        Color: uimd.NewColor("#94a3b8"),
        UserSelect: "text",
    })
    return ui
}

func (ui *ReasoningEntryUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ReasoningEntryUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *ReasoningEntryUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ReasoningEntryUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ReasoningEntryUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *ReasoningEntryUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ReasoningEntryUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
