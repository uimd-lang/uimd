// Auto-generated UI code for task_board - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class TaskBoardUI: GeneratedWindowBase
{
    public var title: Label!
    public var quit_btn: Button!
    public var filters: ReusableElement!
    public var new_task_btn: Button!
    public var mark_all_done_btn: Button!
    public var clear_board_btn: Button!
    public var board: ReusableElement!
    public var status: Label!

    public init()
    {
        super.init("Task Board")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "TaskBoard", source: "# Task Board\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.\"\ntags: [example, mcp]\n```\n\n## Definition\n\n```yaml\nuses:\n  - task_filters/task_filters\n  - task_list/task_list\n  - task_dialog/task_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Task Board\"\n  description: \"Application title.\"\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the task list example.\"\n\nfilters:\n  type: task_filters\n  cpp-class: task_board_example::TaskFilters\n  cpp-header: \"task_filters/task_filters.hpp\"\n  description: \"Reusable task filter panel.\"\n\nnew_task_btn:\n  type: button\n  title: New task\n  description: \"Open a dialog for creating a task.\"\n\nmark_all_done_btn:\n  type: button\n  title: Mark all done\n  description: \"Ask before marking every task as done.\"\n\nclear_board_btn:\n  type: button\n  title: Clear board\n  description: \"Ask before deleting every task.\"\n\nboard:\n  type: task_list\n  cpp-class: task_board_example::TaskList\n  cpp-header: \"task_list/task_list.hpp\"\n  description: \"Scrollable list of interactive task rows.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Current board status.\"\n```\n\n## Tools\n\n```yaml\nquery_tasks:\n  description: \"Return tasks matching optional title, status, assignee, priority, and done filters.\"\n  input:\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    done: bool\n  output:\n    tasks:\n      - id: string\n        title: string\n        status: string\n        assignee: string\n        priority: string\n        description: string\n        done: bool\n\nget_task:\n  description: \"Return one task by id.\"\n  input:\n    id: string\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\nupdate_task:\n  description: \"Update a task by id and return the updated task.\"\n  input:\n    id: string\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    description: string\n    done: bool\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\ndelete_task:\n  description: \"Delete a task by id and return whether it was removed.\"\n  input:\n    id: string\n    required: [id]\n  output:\n    deleted: bool\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@filter_cell:\n  background: \"#15171c\"\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@board_cell:\n  background: \"#030712\"\n@filters:\n  background: \"#15171c\"\n@board:\n  background: \"#030712\"\n  focus-background: \"#07111f\"\n  edit-background: \"#0f172a\"\n  scope-dim-background: \"#0000004d\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@new_task_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@mark_all_done_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@clear_board_btn:\n  background: \"#711212\"\n  focus-background: \"#b91c1c\"\n@status:\n  background: \"#111827\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**................ ..................................... quit_btn.... |\n+-filter_cell-**---------------------------------------------------------------+\n| filters..**................................................................. |\n| *........................................................................... |\n| *........................................................................... |\n| ............................................................................ |\n| ............................................................................ |\n| ............................................................................ |\n+-actions-24----------------+-board_cell-**------------------------------------+\n| new_task_btn..**......... | board..**....................................... |\n|                           | ................................................ |\n* mark_all_done_btn..**.... | ................................................ |\n*                           | ................................................ |\n| clear_board_btn..**...... | ................................................ |\n|                           | *............................................... |\n| status..**............... | *............................................... |\n| ......................... | ................................................ |\n| ......................... | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n+---------------------------+--------------------------------------------------+\n```\n", description: "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.")
        setMcpAppTools([
            GeneratedAppToolMetadata(name: "delete_task", description: "Delete a task by id and return whether it was removed.", inputSchemaJson: "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"}},\"required\":[\"id\"]}", outputSchemaJson: "{\"type\":\"object\",\"properties\":{\"deleted\":{\"type\":\"boolean\"}}}"),
            GeneratedAppToolMetadata(name: "get_task", description: "Return one task by id.", inputSchemaJson: "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"}},\"required\":[\"id\"]}", outputSchemaJson: "{\"type\":\"object\",\"properties\":{\"task\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}"),
            GeneratedAppToolMetadata(name: "query_tasks", description: "Return tasks matching optional title, status, assignee, priority, and done filters.", inputSchemaJson: "{\"type\":\"object\",\"properties\":{\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}", outputSchemaJson: "{\"type\":\"object\",\"properties\":{\"tasks\":{\"type\":\"array\",\"items\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}}"),
            GeneratedAppToolMetadata(name: "update_task", description: "Update a task by id and return the updated task.", inputSchemaJson: "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}},\"required\":[\"id\"]}", outputSchemaJson: "{\"type\":\"object\",\"properties\":{\"task\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}"),
        ])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        title = addElement(Label("title", "Task Board"))
        title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
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
        filters = addElement(ReusableElement("filters"))
        filters.setStyle({
                    var style = Style()
                    style.background = Color("#15171c")
                    return style
                }())
        filters.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        filters.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        filters.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
        new_task_btn = addElement(Button("new_task_btn", "New task"))
        new_task_btn.setStyle({
                    var style = Style()
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        new_task_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#16a34a")
                    style.color = Color("#ffffff")
                    return style
                }())
        new_task_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        mark_all_done_btn = addElement(Button("mark_all_done_btn", "Mark all done"))
        mark_all_done_btn.setStyle({
                    var style = Style()
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        mark_all_done_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#16a34a")
                    style.color = Color("#ffffff")
                    return style
                }())
        mark_all_done_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        clear_board_btn = addElement(Button("clear_board_btn", "Clear board"))
        clear_board_btn.setStyle({
                    var style = Style()
                    style.background = Color("#711212")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        clear_board_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#b91c1c")
                    style.color = Color("#ffffff")
                    return style
                }())
        clear_board_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        board = addElement(ReusableElement("board"))
        board.setStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.scopeDimBackground = Color("#0000004d")
                    return style
                }())
        board.setFocusStyle({
                    var style = Style()
                    style.background = Color("#07111f")
                    return style
                }())
        board.setEditStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    return style
                }())
        board.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
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
                name: "board",
                type: "task_list",
                cellName: "board_cell",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 9, col: 28, width: 50, height: 12),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(12),
                charsSize: Size(width: 48, height: 12),
                cellCharsSize: Size(width: 50, height: 12),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    style.scopeDimBackground = Color("#0000004d")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "filters",
                type: "task_filters",
                cellName: "filter_cell",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 2, col: 0, width: 78, height: 6),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(6),
                charsSize: Size(width: 76, height: 6),
                cellCharsSize: Size(width: 78, height: 6),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#15171c")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#15171c")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "new_task_btn",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 9, col: 0, width: 27, height: 12),
                width: .expanded,
                height: .auto(1),
                cellWidth: .fixed(24),
                cellHeight: .expanded,
                charsSize: Size(width: 25, height: 1),
                cellCharsSize: Size(width: 27, height: 12),
                marginRight: 1,
                marginBottom: 11,
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
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "title",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 78, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 25, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 52,
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
                name: "quit_btn",
                type: "button",
                cellName: "header",
                relative: Rect(row: 0, col: 65, width: 12, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 78, height: 1),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 1,
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
                name: "mark_all_done_btn",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 2, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 9, col: 0, width: 27, height: 12),
                width: .expanded,
                height: .auto(1),
                cellWidth: .fixed(24),
                cellHeight: .expanded,
                charsSize: Size(width: 25, height: 1),
                cellCharsSize: Size(width: 27, height: 12),
                marginRight: 1,
                marginBottom: 9,
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
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "clear_board_btn",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 4, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 9, col: 0, width: 27, height: 12),
                width: .expanded,
                height: .auto(1),
                cellWidth: .fixed(24),
                cellHeight: .expanded,
                charsSize: Size(width: 25, height: 1),
                cellCharsSize: Size(width: 27, height: 12),
                marginRight: 1,
                marginBottom: 7,
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
                    style.background = Color("#711212")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status",
                type: "label",
                cellName: "actions",
                relative: Rect(row: 6, col: 1, width: 536870911, height: 3),
                sourceCell: Rect(row: 9, col: 0, width: 27, height: 12),
                width: .expanded,
                height: .auto(3),
                cellWidth: .fixed(24),
                cellHeight: .expanded,
                charsSize: Size(width: 25, height: 3),
                cellCharsSize: Size(width: 27, height: 12),
                marginRight: 1,
                marginBottom: 3,
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
                    style.background = Color("#111827")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
        ]
    }

    open func onQuitBtnClick() { }
    open func onNewTaskBtnClick() { }
    open func onMarkAllDoneBtnClick() { }
    open func onClearBoardBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "quit_btn"
        {
            onQuitBtnClick()
            return true
        }
        else if name == "new_task_btn"
        {
            onNewTaskBtnClick()
            return true
        }
        else if name == "mark_all_done_btn"
        {
            onMarkAllDoneBtnClick()
            return true
        }
        else if name == "clear_board_btn"
        {
            onClearBoardBtnClick()
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
