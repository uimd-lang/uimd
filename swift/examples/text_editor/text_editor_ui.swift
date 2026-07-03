// Auto-generated UI code for text_editor - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class TextEditorUI: GeneratedWindowBase
{
    public var window_header: Label!
    public var filename: Label!
    public var editor: TextArea!
    public var new_btn: Button!
    public var open_btn: Button!
    public var save_btn: Button!
    public var save_as_btn: Button!
    public var quit_btn: Button!
    public var status: Label!

    public init()
    {
        super.init("Text Editor")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "TextEditor", source: "# Text Editor\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - ../../dialogs/file_browser\n```\n\n## Members\n\n```yaml\nwindow_header:\n  type: label\n  text: \"Text Editor\"\n  description: \"Application title.\"\n  expose: false\n\nfilename:\n  type: label\n  text: welcome.txt\n  description: \"Current file path or file name.\"\n\neditor:\n  type: textarea\n  value: \"\"\n  description: \"Main multiline text editing area.\"\n\nnew_btn:\n  type: button\n  title: New\n  description: \"Create a new empty document, asking about unsaved changes when needed.\"\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open the file browser to load a text file, asking about unsaved changes when needed.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Save the current document to its current path.\"\n\nsave_as_btn:\n  type: button\n  title: \"Save As\"\n  description: \"Open the file browser in save mode and save the document to a chosen path.\"\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the editor, asking about unsaved changes when needed.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows load, save, cancel, and error messages.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@filename:\n  color: \"#bfdbfe\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@status:\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-title-**------------------------------------------------------------------------------+\n|window_header..**......................................................................|\n+-filebar-------------------------------------------------------------------------------+\n|                                                                                       |\n| \"file\" filename..**.................................................................. |\n|                                                                                       |\n| new_btn....... open_btn....... save_btn....... save_as_btn....... quit_btn.......     |\n+-main----------------------------------------------------------------------------------+\n|                                                                                       |\n| editor..**............................................................................|\n| ..................................................................................... |\n* *.................................................................................... |\n* *.................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n+---------------------------------------------------------------------------------------+\n| status............................................................................... |\n+---------------------------------------------------------------------------------------+\n```\n", description: "Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        window_header = addElement(Label("window_header", "Text Editor"))
        window_header.setStyle({
                    var style = Style()
                    style.background = Color("#1d314c")
                    style.color = Color("#cbd5e1")
                    style.textAlign = "center"
                    return style
                }())
        window_header.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        filename = addElement(Label("filename", "welcome.txt"))
        filename.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    return style
                }())
        filename.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        editor = addElement(TextArea("editor", "", maxLength: 0))
        editor.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        editor.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        editor.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        editor.setCursorStyle({
                    var style = Style()
                    style.background = Color("#ffffff")
                    style.color = Color("#0e1117")
                    return style
                }())
        editor.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        editor.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        new_btn = addElement(Button("new_btn", "New"))
        new_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        new_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        new_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        open_btn = addElement(Button("open_btn", "Open"))
        open_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        open_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        open_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        save_btn = addElement(Button("save_btn", "Save"))
        save_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        save_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        save_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        save_as_btn = addElement(Button("save_as_btn", "Save As"))
        save_as_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        save_as_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        save_as_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        quit_btn = addElement(Button("quit_btn", "Quit"))
        quit_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        quit_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        quit_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        status = addElement(Label("status", "Ready"))
        status.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
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
                name: "window_header",
                type: "label",
                cellName: "title",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 87, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 87, height: 1),
                cellCharsSize: Size(width: 87, height: 1),
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
                name: "status",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 1, width: 85, height: 1),
                sourceCell: Rect(row: 18, col: 0, width: 87, height: 1),
                width: .auto(85),
                height: .auto(1),
                cellWidth: .auto(87),
                cellHeight: .auto(1),
                charsSize: Size(width: 85, height: 1),
                cellCharsSize: Size(width: 87, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"file\"",
                type: "label",
                cellName: "filebar",
                relative: Rect(row: 1, col: 1, width: 6, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 87, height: 4),
                width: .auto(6),
                height: .auto(1),
                cellWidth: .auto(87),
                cellHeight: .auto(4),
                charsSize: Size(width: 6, height: 1),
                cellCharsSize: Size(width: 87, height: 4),
                marginRight: 80,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "editor",
                type: "textarea",
                cellName: "main",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 7, col: 0, width: 87, height: 10),
                width: .expanded,
                height: .expanded,
                cellWidth: .auto(87),
                cellHeight: .expanded,
                charsSize: Size(width: 86, height: 9),
                cellCharsSize: Size(width: 87, height: 10),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: Style()
            ),
            GeneratedLayoutEntry(
                name: "filename",
                type: "label",
                cellName: "filebar",
                relative: Rect(row: 1, col: 8, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 87, height: 4),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(87),
                cellHeight: .auto(4),
                charsSize: Size(width: 78, height: 1),
                cellCharsSize: Size(width: 87, height: 4),
                marginRight: 1,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bfdbfe")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "new_btn",
                type: "button",
                cellName: "filebar",
                relative: Rect(row: 3, col: 1, width: 14, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 87, height: 4),
                width: .auto(14),
                height: .auto(1),
                cellWidth: .auto(87),
                cellHeight: .auto(4),
                charsSize: Size(width: 14, height: 1),
                cellCharsSize: Size(width: 87, height: 4),
                marginRight: 72,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "open_btn",
                type: "button",
                cellName: "filebar",
                relative: Rect(row: 3, col: 16, width: 15, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 87, height: 4),
                width: .auto(15),
                height: .auto(1),
                cellWidth: .auto(87),
                cellHeight: .auto(4),
                charsSize: Size(width: 15, height: 1),
                cellCharsSize: Size(width: 87, height: 4),
                marginRight: 56,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "save_btn",
                type: "button",
                cellName: "filebar",
                relative: Rect(row: 3, col: 32, width: 15, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 87, height: 4),
                width: .auto(15),
                height: .auto(1),
                cellWidth: .auto(87),
                cellHeight: .auto(4),
                charsSize: Size(width: 15, height: 1),
                cellCharsSize: Size(width: 87, height: 4),
                marginRight: 40,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "save_as_btn",
                type: "button",
                cellName: "filebar",
                relative: Rect(row: 3, col: 48, width: 18, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 87, height: 4),
                width: .auto(18),
                height: .auto(1),
                cellWidth: .auto(87),
                cellHeight: .auto(4),
                charsSize: Size(width: 18, height: 1),
                cellCharsSize: Size(width: 87, height: 4),
                marginRight: 21,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "quit_btn",
                type: "button",
                cellName: "filebar",
                relative: Rect(row: 3, col: 67, width: 15, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 87, height: 4),
                width: .auto(15),
                height: .auto(1),
                cellWidth: .auto(87),
                cellHeight: .auto(4),
                charsSize: Size(width: 15, height: 1),
                cellCharsSize: Size(width: 87, height: 4),
                marginRight: 5,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onEditorChange(_ value: String) { }
    open func onEditorSubmit(_ value: String) { }
    open func onNewBtnClick() { }
    open func onOpenBtnClick() { }
    open func onSaveBtnClick() { }
    open func onSaveAsBtnClick() { }
    open func onQuitBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "new_btn"
        {
            onNewBtnClick()
            return true
        }
        else if name == "open_btn"
        {
            onOpenBtnClick()
            return true
        }
        else if name == "save_btn"
        {
            onSaveBtnClick()
            return true
        }
        else if name == "save_as_btn"
        {
            onSaveAsBtnClick()
            return true
        }
        else if name == "quit_btn"
        {
            onQuitBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "editor"
        {
            onEditorChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "editor"
        {
            onEditorSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        return false
    }
}
