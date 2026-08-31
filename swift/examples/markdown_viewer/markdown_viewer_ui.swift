// Auto-generated UI code for markdown_viewer - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class MarkdownViewerUI: GeneratedWindowBase
{
    public var header: Label!
    public var docs_label: Label!
    public var docs: ListBox!
    public var up_btn: Button!
    public var down_btn: Button!
    public var viewer: ReusableElement!
    public var close_btn: Button!
    public var status: Label!

    public init()
    {
        super.init("Markdown Viewer")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "MarkdownViewer", source: "# Markdown Viewer\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - document_view\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Markdown Viewer\"\n  description: \"Application title.\"\n  expose: false\n\ndocs_label:\n  type: label\n  text: Documents\n  description: \"Label for the document list.\"\n\ndocs:\n  type: listbox\n  options: [overview.markdown, styles.markdown, tables.markdown, code.markdown]\n  selected_items: [overview.markdown]\n  multiple: \"false\"\n  description: \"Document listbox used to select the markdown document to render.\"\n\nup_btn:\n  type: button\n  title: up\n  description: \"Scroll the rendered document upward.\"\n\ndown_btn:\n  type: button\n  title: down\n  description: \"Scroll the rendered document downward.\"\n\nviewer:\n  type: document_view\n  cpp-class: \"markdown_viewer_example::DocumentView\"\n  cpp-header: document_view/document_view.hpp\n  description: \"Scrollable rendered markdown document area.\"\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: \"Close the markdown viewer.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows the current document and scroll state.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  color: \"#ffffff\"\n@title:\n  background: \"#223a5a\"\n@sidebar:\n  background: \"#101a2b\"\n@content:\n  background: \"#000000\"\n@viewer:\n  background: \"#000000\"\n  padding: 1, 2, 1, 2\n@docs:\n  background: \"#ffffff11\"\n  color: \"#cbd5e1\"\n  focus-background: \"#ffffff22\"\n  edit-background: \"#00000000\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2b5f9f\"\n@close_btn:\n  background: \"#7f1d1d\"\n  color: \"#e2e8f0\"\n  focus-background: \"#dc2626\"\n@status:\n  color: \"#93c5fd\"\n  background: \"#111827\"\n```\n\n## User Interface\n\n```ui\n+-title-**----------------------------------------------------------+\n|header..**................................................close_btn|\n+-sidebar------------------+-content-**-----------------------------+\n|                          |viewer..**..............................|\n| docs_label...**......... |........................................|\n| docs.........**......... |*.......................................|\n| ........................ |*.......................................|\n| #....................... |........................................|\n| #....................... |........................................|\n|                          |........................................|\n| up_btn.... down_btn..... |........................................|\n*                          |........................................|\n*                          |........................................|\n|                          |........................................|\n+--------------------------+----------------------------------------+\n| status..**....................................................... |\n+-------------------------------------------------------------------+\n```\n", description: "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header = addElement(Label("header", "Markdown Viewer"))
        header.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        header.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        docs_label = addElement(Label("docs_label", "Documents"))
        docs_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        docs_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        docs = addElement(ListBox("docs", ["overview.markdown", "styles.markdown", "tables.markdown", "code.markdown"], selectedValues: ["overview.markdown"]))
        docs.setStyle({
                    var style = Style()
                    style.background = Color("#ffffff11")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        docs.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff22")
                    style.color = Color("#ffffff")
                    return style
                }())
        docs.setEditStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        docs.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        docs.setActiveStyle({
                    var style = Style()
                    style.background = Color("#DDDDDD99")
                    style.color = Color("#FFFFFF")
                    return style
                }())
        docs.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        up_btn = addElement(Button("up_btn", "up"))
        up_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        up_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        up_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        down_btn = addElement(Button("down_btn", "down"))
        down_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        down_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2b5f9f")
                    style.color = Color("#ffffff")
                    return style
                }())
        down_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        viewer = addElement(ReusableElement("viewer"))
        viewer.setStyle({
                    var style = Style()
                    style.background = Color("#000000")
                    style.paddingBottom = 1
                    style.paddingLeft = 2
                    style.paddingRight = 2
                    style.paddingTop = 1
                    return style
                }())
        viewer.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        viewer.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        viewer.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn = addElement(Button("close_btn", "Quit"))
        close_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        close_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        close_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        status = addElement(Label("status", "Ready"))
        status.setStyle({
                    var style = Style()
                    style.background = Color("#111827")
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
                name: "header",
                type: "label",
                cellName: "title",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 67, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 58, height: 1),
                cellCharsSize: Size(width: 67, height: 1),
                marginRight: 9,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#223a5a")
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
                name: "viewer",
                type: "document_view",
                cellName: "content",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 2, col: 27, width: 40, height: 11),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(11),
                charsSize: Size(width: 40, height: 11),
                cellCharsSize: Size(width: 40, height: 11),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#000000")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#000000")
                    style.paddingBottom = 1
                    style.paddingLeft = 2
                    style.paddingRight = 2
                    style.paddingTop = 1
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 14, col: 0, width: 67, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(67),
                cellHeight: .auto(1),
                charsSize: Size(width: 65, height: 1),
                cellCharsSize: Size(width: 67, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "close_btn",
                type: "button",
                cellName: "title",
                relative: Rect(row: 0, col: 58, width: 9, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 67, height: 1),
                width: .auto(9),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 9, height: 1),
                cellCharsSize: Size(width: 67, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#223a5a")
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
                name: "docs_label",
                type: "label",
                cellName: "sidebar",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 26, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(26),
                cellHeight: .expanded,
                charsSize: Size(width: 24, height: 1),
                cellCharsSize: Size(width: 26, height: 11),
                marginRight: 1,
                marginBottom: 9,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#101a2b")
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
                name: "docs",
                type: "listbox",
                cellName: "sidebar",
                relative: Rect(row: 2, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 2, col: 0, width: 26, height: 11),
                width: .expanded,
                height: .fitContent,
                cellWidth: .auto(26),
                cellHeight: .expanded,
                charsSize: Size(width: 24, height: 4),
                cellCharsSize: Size(width: 26, height: 11),
                marginRight: 1,
                marginBottom: 5,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#101a2b")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#ffffff11")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "up_btn",
                type: "button",
                cellName: "sidebar",
                relative: Rect(row: 7, col: 1, width: 10, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 26, height: 11),
                width: .auto(10),
                height: .auto(1),
                cellWidth: .auto(26),
                cellHeight: .expanded,
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 26, height: 11),
                marginRight: 15,
                marginBottom: 3,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#101a2b")
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
                name: "down_btn",
                type: "button",
                cellName: "sidebar",
                relative: Rect(row: 7, col: 12, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 26, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(26),
                cellHeight: .expanded,
                charsSize: Size(width: 13, height: 1),
                cellCharsSize: Size(width: 26, height: 11),
                marginRight: 1,
                marginBottom: 3,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#101a2b")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onDocsSelectionChange(_ value: [String]) { }
    open func onDocsItemActivate(_ index: Int, value: String) -> Bool { false }
    open func onUpBtnClick() { }
    open func onDownBtnClick() { }
    open func onCloseBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "up_btn"
        {
            onUpBtnClick()
            return true
        }
        else if name == "down_btn"
        {
            onDownBtnClick()
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
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "docs"
        {
            onDocsSelectionChange(value)
            return true
        }
        return false
    }

    open override func handleGeneratedListBoxItemActivate(_ name: String, elementId: String, index: Int, value: String) -> Bool
    {
        if name == "docs"
        {
            return onDocsItemActivate(index, value: value)
        }
        return false
    }
}
