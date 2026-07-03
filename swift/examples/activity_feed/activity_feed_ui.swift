// Auto-generated UI code for activity_feed - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ActivityFeedUI: GeneratedWindowBase
{
    public var header_title: Label!
    public var settings_button: Button!
    public var quit_button: Button!
    public var feed: ReusableElement!
    public var type_label: Label!
    public var activity_type: ComboBox!
    public var message_label: Label!
    public var message: TextArea!
    public var add_button: Button!
    public var seed_button: Button!
    public var clear_button: Button!
    public var status: Label!

    public init()
    {
        super.init("Activity Feed")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ActivityFeed", source: "# Activity Feed\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Activity feed example with dynamically appended scroll view items and a settings dialog.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - activity_feed_panel/activity_feed_panel\n  - settings/settings\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: \"Activity Feed\"\n  description: \"Application title.\"\n  expose: false\n\nsettings_button:\n  type: button\n  title: Settings\n  description: \"Open the activity feed settings dialog.\"\n\nquit_button:\n  type: button\n  title: Quit\n  description: \"Close the activity feed example.\"\n\nfeed:\n  type: activity_feed_panel\n  description: \"Scrollable activity timeline.\"\n\ntype_label:\n  type: label\n  text: Type\n  description: \"Label for activity type selector.\"\n  expose: false\n\nactivity_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: \"Activity type selector.\"\n\nmessage_label:\n  type: label\n  text: Message\n  description: \"Label for activity message input.\"\n  expose: false\n\nmessage:\n  type: textarea\n  value: \"\"\n  maxlength: 180\n  description: \"Activity message input.\"\n\nadd_button:\n  type: button\n  title: Add\n  description: \"Append one activity item to the feed.\"\n\nseed_button:\n  type: button\n  title: Seed\n  description: \"Append sample activity items to the feed.\"\n\nclear_button:\n  type: button\n  title: Clear\n  description: \"Remove all activity items from the feed.\"\n\nstatus:\n  type: label\n  text: \"Ready\"\n  description: \"Shows activity feed status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@header_title:\n  text-align: center\n  color: \"#ffffff\"\n@content:\n  background: \"#030712\"\n@feed:\n  background: \"#00000000\"\n@controls:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@clear_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n@status:\n  background: \"#0f172a\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-header-**-------------------------------------------------------+\n|header_title..**................. settings_button.. quit_button..|\n+-content-**------------------------------------------------------+\n| feed..**....................................................... |\n| ............................................................... |\n| ............................................................... |\n* *.............................................................. |\n* *.............................................................. |\n* *.............................................................. |\n| ............................................................... |\n+-controls-**-----------------------------------------------------+\n| type_label     activity_type.........                           |\n|                                                                 |\n| message_label  message..**....................................  |\n|               ...............................................   |\n|               ...............................................   |\n+-actions-**------------------------------------------------------+\n| add_button......... seed_button......... clear_button.......... |\n+-----------------------------------------------------------------+\n| status..**..................................................... |\n+-----------------------------------------------------------------+\n```\n", description: "Activity feed example with dynamically appended scroll view items and a settings dialog.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header_title = addElement(Label("header_title", "Activity Feed"))
        header_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        header_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        settings_button = addElement(Button("settings_button", "Settings"))
        settings_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        settings_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        settings_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        quit_button = addElement(Button("quit_button", "Quit"))
        quit_button.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        quit_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        quit_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        feed = addElement(ReusableElement("feed"))
        feed.setStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    return style
                }())
        feed.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        feed.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        feed.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
        type_label = addElement(Label("type_label", "Type"))
        type_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        type_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        activity_type = addElement(ComboBox("activity_type", ["Info", "Task", "Warning", "Deploy", "Note"], selectedIndex: 0))
        activity_type.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        activity_type.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        activity_type.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        activity_type.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        activity_type.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        message_label = addElement(Label("message_label", "Message"))
        message_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        message_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        message = addElement(TextArea("message", "", maxLength: 180))
        message.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        message.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        message.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#ffffff")
                    return style
                }())
        message.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        message.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        message.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        add_button = addElement(Button("add_button", "Add"))
        add_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        add_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        add_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        seed_button = addElement(Button("seed_button", "Seed"))
        seed_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        seed_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        seed_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        clear_button = addElement(Button("clear_button", "Clear"))
        clear_button.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        clear_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        clear_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        status = addElement(Label("status", "Ready"))
        status.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#93c5fd")
                    return style
                }())
        status.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "header_title",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 65, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 33, height: 1),
                cellCharsSize: Size(width: 65, height: 1),
                marginRight: 32,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "add_button",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 1, width: 19, height: 1),
                sourceCell: Rect(row: 16, col: 0, width: 65, height: 1),
                width: .auto(19),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 19, height: 1),
                cellCharsSize: Size(width: 65, height: 1),
                marginRight: 45,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "feed",
                type: "activity_feed_panel",
                cellName: "content",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 2, col: 0, width: 65, height: 7),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 63, height: 7),
                cellCharsSize: Size(width: 65, height: 7),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#00000000")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 18, col: 0, width: 65, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(65),
                cellHeight: .auto(1),
                charsSize: Size(width: 63, height: 1),
                cellCharsSize: Size(width: 65, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "type_label",
                type: "label",
                cellName: "controls",
                relative: Rect(row: 0, col: 1, width: 10, height: 1),
                sourceCell: Rect(row: 10, col: 0, width: 65, height: 5),
                width: .auto(10),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(5),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 65, height: 5),
                marginRight: 54,
                marginBottom: 4,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "activity_type",
                type: "combobox",
                cellName: "controls",
                relative: Rect(row: 0, col: 16, width: 22, height: 1),
                sourceCell: Rect(row: 10, col: 0, width: 65, height: 5),
                width: .auto(22),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(5),
                charsSize: Size(width: 22, height: 1),
                cellCharsSize: Size(width: 65, height: 5),
                marginRight: 27,
                marginBottom: 4,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "seed_button",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 21, width: 20, height: 1),
                sourceCell: Rect(row: 16, col: 0, width: 65, height: 1),
                width: .auto(20),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 65, height: 1),
                marginRight: 24,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "settings_button",
                type: "button",
                cellName: "header",
                relative: Rect(row: 0, col: 34, width: 17, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 65, height: 1),
                width: .auto(17),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 17, height: 1),
                cellCharsSize: Size(width: 65, height: 1),
                marginRight: 14,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "clear_button",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 42, width: 22, height: 1),
                sourceCell: Rect(row: 16, col: 0, width: 65, height: 1),
                width: .auto(22),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 22, height: 1),
                cellCharsSize: Size(width: 65, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "quit_button",
                type: "button",
                cellName: "header",
                relative: Rect(row: 0, col: 52, width: 13, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 65, height: 1),
                width: .auto(13),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 65, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "message_label",
                type: "label",
                cellName: "controls",
                relative: Rect(row: 2, col: 1, width: 13, height: 1),
                sourceCell: Rect(row: 10, col: 0, width: 65, height: 5),
                width: .auto(13),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(5),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 65, height: 5),
                marginRight: 51,
                marginBottom: 2,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "message",
                type: "textarea",
                cellName: "controls",
                relative: Rect(row: 2, col: 16, width: 536870911, height: 3),
                sourceCell: Rect(row: 10, col: 0, width: 65, height: 5),
                width: .expanded,
                height: .auto(3),
                cellWidth: .expanded,
                cellHeight: .auto(5),
                charsSize: Size(width: 47, height: 3),
                cellCharsSize: Size(width: 65, height: 5),
                marginRight: 2,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
        ]
    }

    open func onSettingsButtonClick() { }
    open func onQuitButtonClick() { }
    open func onActivityTypeChange(_ value: String) { }
    open func onMessageChange(_ value: String) { }
    open func onMessageSubmit(_ value: String) { }
    open func onAddButtonClick() { }
    open func onSeedButtonClick() { }
    open func onClearButtonClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "settings_button"
        {
            onSettingsButtonClick()
            return true
        }
        else if name == "quit_button"
        {
            onQuitButtonClick()
            return true
        }
        else if name == "add_button"
        {
            onAddButtonClick()
            return true
        }
        else if name == "seed_button"
        {
            onSeedButtonClick()
            return true
        }
        else if name == "clear_button"
        {
            onClearButtonClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "message"
        {
            onMessageChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "message"
        {
            onMessageSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "activity_type"
        {
            onActivityTypeChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}
