// Auto-generated UI code for activity_item - DO NOT EDIT MANUALLY.
package activity_item

import uimd "uimd"

type ActivityItemUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Timestamp *uimd.Label
    EventType *uimd.Label
    Message *uimd.Label
}

func NewActivityItemUI() *ActivityItemUI {
    ui := &ActivityItemUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Activity Item")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 10, CellCharsHeight: 6,
            CellName: "marker", CellWidth: 1, CellHeight: uimd.FitContent,
            CellWidthMode: "fixed", CellHeightMode: "fit-content",
            Width: 1, Height: uimd.FitContent,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 6,
            MarginRight: 0, MarginBottom: 0,
            Content: "",
            CellStyle: uimd.Style{
        BackgroundTexture: "▐",
        BackgroundTextureColor: uimd.NewColor("#38bdf8"),
    },
            ElementStyle: uimd.NewStyle(),
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 11,
            CellCharsWidth: 42, CellCharsHeight: 6,
            CellName: "body", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 30, MarginBottom: 5,
            Content: "timestamp",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2e46"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#94a3b8"),
        UserSelect: "text",
    },
        },
        {
            Row: 0, Col: 13,
            CellRow: 0, CellCol: 11,
            CellCharsWidth: 42, CellCharsHeight: 6,
            CellName: "body", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 15, MarginBottom: 5,
            Content: "event_type",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2e46"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
        UserSelect: "text",
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 0, CellCol: 11,
            CellCharsWidth: 42, CellCharsHeight: 6,
            CellName: "body", CellWidth: 0, CellHeight: uimd.FitContent,
            CellWidthMode: "expanded", CellHeightMode: "fit-content",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 11, CharsHeight: 5,
            MarginRight: 30, MarginBottom: 0,
            Content: "message",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#1f2e46"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        UserSelect: "text",
    },
        },
    })
    ui.SetGeneratedKind("component")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ActivityItem", "# Activity Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable activity feed item with timestamp, event type, and message text.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntimestamp:\n  type: label\n  text: \"10:42\"\n  description: \"Optional activity timestamp.\"\n\nevent_type:\n  type: label\n  text: Info\n  description: \"Activity category label.\"\n\nmessage:\n  type: label\n  text: \"Activity message\"\n  description: \"Selectable activity message body.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@marker:\n  background-texture: \"▐\"\n  background-texture-color: \"#38bdf8\"\n@marker_info:\n  background-texture-color: \"#38bdf8\"\n@marker_task:\n  background-texture-color: \"#a78bfa\"\n@marker_warning:\n  background-texture-color: \"#facc15\"\n@marker_deploy:\n  background-texture-color: \"#22c55e\"\n@marker_note:\n  background-texture-color: \"#fb7185\"\n@body:\n  background: \"#1f2e46\"\n@timestamp:\n  color: \"#94a3b8\"\n  user-select: text\n@event_type:\n  color: \"#bfdbfe\"\n  user-select: text\n@message:\n  color: \"#ffffff\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-marker-1-+-body-**----------------------------------+\n|          | timestamp.. event_type..**               |\n|          | message..**                              |\n|          | ........................................ |\n|          | ........................................ |\n#          # #                                        |\n#          # #                                        |\n+----------+------------------------------------------+\n```\n", "Reusable activity feed item with timestamp, event type, and message text.")
    ui.Timestamp = uimd.NewLabel("timestamp", "10:42")
    ui.AddElement(ui.Timestamp)
    ui.Timestamp.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#94a3b8"),
        UserSelect: "text",
    })
    ui.Timestamp.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.EventType = uimd.NewLabel("event_type", "Info")
    ui.AddElement(ui.EventType)
    ui.EventType.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#bfdbfe"),
        UserSelect: "text",
    })
    ui.EventType.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Message = uimd.NewLabel("message", "Activity message")
    ui.AddElement(ui.Message)
    ui.Message.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        UserSelect: "text",
    })
    ui.Message.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *ActivityItemUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ActivityItemUI) HandleGeneratedButton(name string) bool {
    return false
}

func (ui *ActivityItemUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ActivityItemUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ActivityItemUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *ActivityItemUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ActivityItemUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
