// Auto-generated UI code for file_browser - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class FileBrowserUI: GeneratedWindowBase
{
    public var dialog_header: Label!
    public var path_label: Label!
    public var entries: ListBox!
    public var filename: TextInput!
    public var open_btn: Button!
    public var close_btn: Button!

    public init()
    {
        super.init("File Browser")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("dialog")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "FileBrowser", source: "# File Browser\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: stable\ndescription: \"File Browser UI source.\"\ntags: [dialog]\n```\n\n## Members\n\n```yaml\ndialog_header:\n  type: label\n  text: \"Open File\"\n\npath_label:\n  type: label\n  text: .\n\nentries:\n  type: listbox\n  options: []\n  selected_items: []\n  multiple: \"false\"\n  commit-mode: leave\n\nfilename:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n\nopen_btn:\n  type: button\n  title: Open\n\nclose_btn:\n  type: button\n  title: Close\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@entries:\n  background: \"#0d1524\"\n  color: \"#cbd5e1\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n```\n\n## User Interface\n\n```ui\n+------------------------------------------------+\n|dialog_header...................................|\n+------------------------------------------------+\n|                                                |\n|   path_label................................   |\n|                                                |\n|   entries...................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|                                                |\n|   \"File:\"..  filename.......................   |\n|                                                |\n|   open_btn............  close_btn...........   |\n|                                                |\n+------------------------------------------------+\n```\n", description: "File Browser UI source.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        dialog_header = addElement(Label("dialog_header", "Open File"))
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
        path_label = addElement(Label("path_label", "."))
        path_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        path_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        entries = addElement(ListBox("entries", [], selectedValues: []))
        entries.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        entries.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1d2f4d")
                    style.color = Color("#ffffff")
                    return style
                }())
        entries.setEditStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        entries.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        entries.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        filename = addElement(TextInput("filename", "", maxLength: 120))
        filename.setStyle({
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        filename.setFocusStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#ffffff")
                    return style
                }())
        filename.setEditStyle({
                    var style = Style()
                    style.background = Color("#2d456d")
                    style.color = Color("#ffffff")
                    return style
                }())
        filename.setCursorStyle({
                    var style = Style()
                    style.background = Color("#ffffff")
                    style.color = Color("#0e1117")
                    return style
                }())
        filename.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        filename.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
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
        close_btn = addElement(Button("close_btn", "Close"))
        close_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        close_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn.setDisabledStyle({
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
                sourceCell: Rect(row: 0, col: 0, width: 48, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(48),
                cellHeight: .auto(1),
                charsSize: Size(width: 48, height: 1),
                cellCharsSize: Size(width: 48, height: 1),
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
                name: "path_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 1, col: 3, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 48, height: 14),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(48),
                cellHeight: .auto(14),
                charsSize: Size(width: 42, height: 1),
                cellCharsSize: Size(width: 48, height: 14),
                marginRight: 3,
                marginBottom: 12,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "entries",
                type: "listbox",
                cellName: "",
                relative: Rect(row: 3, col: 3, width: 536870911, height: 6),
                sourceCell: Rect(row: 2, col: 0, width: 48, height: 14),
                width: .expanded,
                height: .auto(6),
                cellWidth: .auto(48),
                cellHeight: .auto(14),
                charsSize: Size(width: 42, height: 6),
                cellCharsSize: Size(width: 48, height: 14),
                marginRight: 3,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"File:\"",
                type: "label",
                cellName: "",
                relative: Rect(row: 10, col: 3, width: 9, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 48, height: 14),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .auto(48),
                cellHeight: .auto(14),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 48, height: 14),
                marginRight: 36,
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
                name: "filename",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 10, col: 14, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 48, height: 14),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(48),
                cellHeight: .auto(14),
                charsSize: Size(width: 31, height: 1),
                cellCharsSize: Size(width: 48, height: 14),
                marginRight: 3,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0d1524")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "open_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 12, col: 3, width: 20, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 48, height: 14),
                width: .auto(20),
                height: .auto(1),
                cellWidth: .auto(48),
                cellHeight: .auto(14),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 48, height: 14),
                marginRight: 25,
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
                name: "close_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 12, col: 25, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 48, height: 14),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(48),
                cellHeight: .auto(14),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 48, height: 14),
                marginRight: 3,
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

    open func onEntriesSelectionChange(_ value: [String]) { }
    open func onFilenameChange(_ value: String) { }
    open func onFilenameSubmit(_ value: String) { }
    open func onOpenBtnClick() { }
    open func onCloseBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "open_btn"
        {
            onOpenBtnClick()
            return true
        }
        else if name == "close_btn"
        {
            onCloseBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "filename"
        {
            onFilenameChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "filename"
        {
            onFilenameSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "entries"
        {
            onEntriesSelectionChange(value)
            return true
        }
        return false
    }
}
