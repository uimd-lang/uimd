// Auto-generated UI code for task_dialog - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class TaskDialogUI: GeneratedWindowBase
{
    public var header: Label!
    public var name_label: Label!
    public var name: TextInput!
    public var status_label: Label!
    public var project_status: ComboBox!
    public var owner_label: Label!
    public var owner: ComboBox!
    public var priority_label: Label!
    public var priority: ComboBox!
    public var note_label: Label!
    public var note: TextArea!
    public var message: Label!
    public var add_btn: Button!
    public var cancel_btn: Button!

    public init()
    {
        super.init("Task Dialog")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("dialog")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "TaskDialog", source: "# Task Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: draft\ndescription: \"Dialog form for creating or editing a task.\"\ntags: [example, dialog]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Task\n  description: \"Dialog title.\"\n  expose: false\n\nname_label:\n  type: label\n  text: Title\n  description: \"Label for task title.\"\n  expose: false\n\nname:\n  type: textinput\n  value: \"\"\n  maxlength: 60\n  description: \"Required task title.\"\n\nstatus_label:\n  type: label\n  text: Status\n  description: \"Label for task status.\"\n  expose: false\n\nproject_status:\n  type: combobox\n  options: [Todo, Doing, Blocked, Done]\n  selected_item: Todo\n  description: \"Task status.\"\n\nowner_label:\n  type: label\n  text: Assignee\n  description: \"Label for task assignee.\"\n  expose: false\n\nowner:\n  type: combobox\n  options: [Ava, Bruno, Chen, Dana]\n  selected_item: Ava\n  description: \"Task assignee.\"\n\npriority_label:\n  type: label\n  text: Priority\n  description: \"Label for task priority.\"\n  expose: false\n\npriority:\n  type: combobox\n  options: [Low, Med, High, Urgent]\n  selected_item: Med\n  description: \"Task priority.\"\n\nnote_label:\n  type: label\n  text: Description\n  description: \"Label for task description.\"\n  expose: false\n\nnote:\n  type: textarea\n  value: \"\"\n  maxlength: 160\n  description: \"Task description.\"\n\nmessage:\n  type: label\n  text: \"\"\n  description: \"Validation message.\"\n\nadd_btn:\n  type: button\n  title: Save task\n  description: \"Save the task and close the dialog.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Close the dialog without saving the task.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@add_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@message:\n  color: \"#fbbf24\"\n```\n\n## User Interface\n\n```ui\n+------------------------------------------------------+\n|header................................................|\n+------------------------------------------------------+\n|                                                      |\n|  name_label      name.............................   |\n|                                                      |\n|  status_label    project_status...................   |\n|                                                      |\n|  owner_label     owner............................   |\n|                                                      |\n|  priority_label  priority.........................   |\n|                                                      |\n|  note_label.     note.............................   |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                                                      |\n|  message..........................................   |\n|                                                      |\n+------------------------------------------------------+\n|  add_btn................  cancel_btn..............   |\n|                                                      |\n+------------------------------------------------------+\n```\n", description: "Dialog form for creating or editing a task.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header = addElement(Label("header", "Task"))
        header.setStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
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
        name_label = addElement(Label("name_label", "Title"))
        name_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        name_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        name = addElement(TextInput("name", "", maxLength: 60))
        name.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        name.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        name.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        name.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        name.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        name.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        status_label = addElement(Label("status_label", "Status"))
        status_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        status_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        project_status = addElement(ComboBox("project_status", ["Todo", "Doing", "Blocked", "Done"], selectedIndex: 0))
        project_status.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        project_status.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        project_status.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        project_status.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        project_status.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        owner_label = addElement(Label("owner_label", "Assignee"))
        owner_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        owner_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        owner = addElement(ComboBox("owner", ["Ava", "Bruno", "Chen", "Dana"], selectedIndex: 0))
        owner.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        owner.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        owner.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        owner.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        owner.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        priority_label = addElement(Label("priority_label", "Priority"))
        priority_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        priority_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        priority = addElement(ComboBox("priority", ["Low", "Med", "High", "Urgent"], selectedIndex: 1))
        priority.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        priority.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        priority.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        priority.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        priority.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        note_label = addElement(Label("note_label", "Description"))
        note_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        note_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        note = addElement(TextArea("note", "", maxLength: 160))
        note.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        note.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        note.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#ffffff")
                    return style
                }())
        note.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        note.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        note.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        message = addElement(Label("message", ""))
        message.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#fbbf24")
                    return style
                }())
        message.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        add_btn = addElement(Button("add_btn", "Save task"))
        add_btn.setStyle({
                    var style = Style()
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        add_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#16a34a")
                    style.color = Color("#ffffff")
                    return style
                }())
        add_btn.setDisabledStyle({
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
                    style.background = Color("#2563eb")
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
                name: "header",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(1),
                charsSize: Size(width: 54, height: 1),
                cellCharsSize: Size(width: 54, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "add_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 0, col: 2, width: 23, height: 1),
                sourceCell: Rect(row: 20, col: 0, width: 54, height: 2),
                width: .auto(23),
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(2),
                charsSize: Size(width: 23, height: 1),
                cellCharsSize: Size(width: 54, height: 2),
                marginRight: 29,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#14532d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "cancel_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 0, col: 27, width: 536870911, height: 1),
                sourceCell: Rect(row: 20, col: 0, width: 54, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(2),
                charsSize: Size(width: 24, height: 1),
                cellCharsSize: Size(width: 54, height: 2),
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
            GeneratedLayoutEntry(
                name: "name_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 1, col: 2, width: 10, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .auto(10),
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 10, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 42,
                marginBottom: 15,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "name",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 1, col: 18, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 33, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 3,
                marginBottom: 15,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 3, col: 2, width: 12, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 40,
                marginBottom: 13,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "project_status",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 3, col: 18, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 33, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 3,
                marginBottom: 13,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "owner_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 5, col: 2, width: 11, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 41,
                marginBottom: 11,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "owner",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 5, col: 18, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 33, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 3,
                marginBottom: 11,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "priority_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 7, col: 2, width: 14, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .auto(14),
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 14, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 38,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "priority",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 7, col: 18, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 33, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 3,
                marginBottom: 9,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "note_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 9, col: 2, width: 11, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 41,
                marginBottom: 7,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "note",
                type: "textarea",
                cellName: "",
                relative: Rect(row: 9, col: 18, width: 536870911, height: 5),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .expanded,
                height: .auto(5),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 33, height: 5),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 3,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "message",
                type: "label",
                cellName: "",
                relative: Rect(row: 15, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 54, height: 17),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(54),
                cellHeight: .auto(17),
                charsSize: Size(width: 49, height: 1),
                cellCharsSize: Size(width: 54, height: 17),
                marginRight: 3,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#fbbf24")
                    return style
                }()
            ),
        ]
    }

    open func onNameChange(_ value: String) { }
    open func onNameSubmit(_ value: String) { }
    open func onProjectStatusChange(_ value: String) { }
    open func onOwnerChange(_ value: String) { }
    open func onPriorityChange(_ value: String) { }
    open func onNoteChange(_ value: String) { }
    open func onNoteSubmit(_ value: String) { }
    open func onAddBtnClick() { }
    open func onCancelBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "add_btn"
        {
            onAddBtnClick()
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
        if name == "name"
        {
            onNameChange(value)
            return true
        }
        else if name == "note"
        {
            onNoteChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "name"
        {
            onNameSubmit(value)
            return true
        }
        else if name == "note"
        {
            onNoteSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "project_status"
        {
            onProjectStatusChange(value.isEmpty ? "" : value[0])
            return true
        }
        else if name == "owner"
        {
            onOwnerChange(value.isEmpty ? "" : value[0])
            return true
        }
        else if name == "priority"
        {
            onPriorityChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}
