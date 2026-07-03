// Auto-generated UI code for task_row - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class TaskRowUI: GeneratedWindowBase
{
    public var task_title: Label!
    public var meta: Label!
    public var done: CheckBox!
    public var spacer: Label!
    public var open_btn: Button!
    public var delete_btn: Button!

    public init()
    {
        super.init("Task Row")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "TaskRow", source: "# Task Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable task row with editable controls.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntask_title:\n  type: label\n  text: \"Task\"\n  description: \"Task title.\"\n\nmeta:\n  type: label\n  text: \"Status / assignee / priority\"\n  description: \"Task status, assignee, and priority.\"\n\ndone:\n  type: checkbox\n  title: Done\n  value: false\n  description: \"Whether the task is done.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible row spacer before task actions.\"\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open this task in the edit dialog.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Ask for confirmation and delete this task.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@task_title:\n  color: \"#f8fafc\"\n  user-select: text\n@meta:\n  color: \"#93c5fd\"\n  user-select: text\ncheckbox:\n  color: \"#cbd5e1\"\n  focus-color: \"#facc15\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------+\n| task_title..**....... meta................... |\n| done.... spacer..**.. open_btn.. delete_btn.. |\n+-----------------------------------------------+\n```\n", description: "Reusable task row with editable controls.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        task_title = addElement(Label("task_title", "Task"))
        task_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f8fafc")
                    style.userSelect = "text"
                    return style
                }())
        task_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        meta = addElement(Label("meta", "Status / assignee / priority"))
        meta.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    style.userSelect = "text"
                    return style
                }())
        meta.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        done = addElement(CheckBox("done", "Done", false))
        done.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        done.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#facc15")
                    return style
                }())
        done.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#22c55e")
                    return style
                }())
        done.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#94a3b8")
                    return style
                }())
        done.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        spacer = addElement(Label("spacer", ""))
        spacer.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        spacer.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
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
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        open_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        delete_btn = addElement(Button("delete_btn", "Delete"))
        delete_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        delete_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        delete_btn.setDisabledStyle({
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
                name: "task_title",
                type: "label",
                cellName: "card",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 47, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 21, height: 1),
                cellCharsSize: Size(width: 47, height: 2),
                marginRight: 25,
                marginBottom: 1,
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
                    style.color = Color("#f8fafc")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "meta",
                type: "label",
                cellName: "card",
                relative: Rect(row: 0, col: 23, width: 23, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 47, height: 2),
                width: .auto(23),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 23, height: 1),
                cellCharsSize: Size(width: 47, height: 2),
                marginRight: 1,
                marginBottom: 1,
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
                    style.color = Color("#93c5fd")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "done",
                type: "checkbox",
                cellName: "card",
                relative: Rect(row: 1, col: 1, width: 8, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 47, height: 2),
                width: .auto(8),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 8, height: 1),
                cellCharsSize: Size(width: 47, height: 2),
                marginRight: 38,
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
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "spacer",
                type: "label",
                cellName: "card",
                relative: Rect(row: 1, col: 10, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 47, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 47, height: 2),
                marginRight: 25,
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
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "open_btn",
                type: "button",
                cellName: "card",
                relative: Rect(row: 1, col: 23, width: 10, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 47, height: 2),
                width: .auto(10),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 47, height: 2),
                marginRight: 14,
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
                name: "delete_btn",
                type: "button",
                cellName: "card",
                relative: Rect(row: 1, col: 34, width: 12, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 47, height: 2),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(2),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 47, height: 2),
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
        ]
    }

    open func onDoneChange(_ value: String) { }
    open func onOpenBtnClick() { }
    open func onDeleteBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "open_btn"
        {
            onOpenBtnClick()
            return true
        }
        else if name == "delete_btn"
        {
            onDeleteBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "done"
        {
            onDoneChange(value)
            return true
        }
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
