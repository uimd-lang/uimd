// Auto-generated UI code for message_box_yes_no_cancel - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class MessageBoxYesNoCancelUI: GeneratedWindowBase
{
    public var dialog_header: Label!
    public var message: Label!
    public var yes_btn: Button!
    public var no_btn: Button!
    public var cancel_btn: Button!

    public init()
    {
        super.init("Message Box")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("dialog")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "MessageBoxYesNoCancel", source: "# Message Box\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: stable\ndescription: \"Message Box UI source.\"\ntags: [dialog]\n```\n\n## Members\n\n```yaml\ndialog_header:\n  type: label\n  text: Message\n\nmessage:\n  type: label\n  text: \"\"\n\nyes_btn:\n  type: button\n  title: \"Yes\"\n\nno_btn:\n  type: button\n  title: \"No\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------+\n|dialog_header.................................|\n+----------------------------------------------+\n|                                              |\n|  message...................................  |\n|  ..........................................  |\n|                                              |\n|  yes_btn.....  no_btn.......  cancel_btn...  |\n|                                              |\n+----------------------------------------------+\n```\n", description: "Message Box UI source.")
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        dialog_header = addElement(Label("dialog_header", "Message"))
        dialog_header.setStyle({
                    var style = Style()
                    style.background = Color("#1d314c")
                    style.color = Color("#cbd5e1")
                    style.textAlign = "center"
                    return style
                }())
        dialog_header.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        message = addElement(Label("message", ""))
        message.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        message.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        yes_btn = addElement(Button("yes_btn", "Yes"))
        yes_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        yes_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        yes_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        no_btn = addElement(Button("no_btn", "No"))
        no_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        no_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        no_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        cancel_btn = addElement(Button("cancel_btn", "Cancel"))
        cancel_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        cancel_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        cancel_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "dialog_header",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 46, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(46),
                cellHeight: .auto(1),
                charsSize: Size(width: 46, height: 1),
                cellCharsSize: Size(width: 46, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#1d314c")
                    style.color = Color("#cbd5e1")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "message",
                type: "label",
                cellName: "",
                relative: Rect(row: 1, col: 2, width: 536870911, height: 2),
                sourceCell: Rect(row: 2, col: 0, width: 46, height: 6),
                width: .expanded,
                height: .auto(2),
                cellWidth: .auto(46),
                cellHeight: .auto(6),
                charsSize: Size(width: 42, height: 2),
                cellCharsSize: Size(width: 46, height: 6),
                marginRight: 2,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "yes_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 4, col: 2, width: 12, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 46, height: 6),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .auto(46),
                cellHeight: .auto(6),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 46, height: 6),
                marginRight: 32,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "no_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 4, col: 16, width: 13, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 46, height: 6),
                width: .auto(13),
                height: .auto(1),
                cellWidth: .auto(46),
                cellHeight: .auto(6),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 46, height: 6),
                marginRight: 17,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "cancel_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 4, col: 31, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 46, height: 6),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(46),
                cellHeight: .auto(6),
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 46, height: 6),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onYesBtnClick() { }
    open func onNoBtnClick() { }
    open func onCancelBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "yes_btn"
        {
            onYesBtnClick()
            return true
        }
        else if name == "no_btn"
        {
            onNoBtnClick()
            return true
        }
        else if name == "cancel_btn"
        {
            onCancelBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        return false
    }
}
