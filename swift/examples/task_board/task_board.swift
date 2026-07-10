import Foundation
import Uimd

private let kAnyFilter = "Any"
private let kDefaultAssignee = "Ava"
private let kDefaultPriority = "Med"
private let kDefaultStatus = "Todo"
private let kDoneStatus = "Done"
private let kTaskIdPrefix = "t-"
private let kInitialTaskNumberBase = 100
private let kCanceledStatusText = "Action canceled."
private let kMarkAllDoneStatusText = "All tasks marked done."
private let kClearBoardStatusText = "Board cleared."

private struct TaskItem
{
    var id: String
    var title: String
    var status: String
    var assignee: String
    var priority: String
    var description: String
    var done: Bool
}

private struct TaskDialogResult
{
    var id: String
    var title: String
    var status: String
    var assignee: String
    var priority: String
    var description: String
    var done: Bool
}

private func selectedComboValue(_ combo: ComboBox, fallback: String) -> String
{
    combo.selectedIndex >= 0 && combo.selectedIndex < combo.options.count ? combo.options[combo.selectedIndex] : fallback
}

private func selectComboValue(_ combo: ComboBox, _ value: String)
{
    combo.selectedIndex = combo.options.firstIndex(of: value) ?? (combo.options.isEmpty ? -1 : 0)
}

private func jsonObject(_ text: String) -> [String: Any]
{
    guard let data = text.data(using: .utf8),
          let object = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
    else
    {
        return [:]
    }
    return object
}

private func jsonString(_ object: [String: Any], _ name: String, _ fallback: String) -> String
{
    object[name] as? String ?? fallback
}

private func jsonBool(_ object: [String: Any], _ name: String) -> Bool?
{
    object[name] as? Bool
}

private func encodeJson(_ object: [String: Any]) -> String
{
    let data = (try? JSONSerialization.data(withJSONObject: object, options: [.sortedKeys])) ?? Data("{}".utf8)
    return String(data: data, encoding: .utf8) ?? "{}"
}

private func taskJson(_ task: TaskItem) -> [String: Any]
{
    [
        "id": task.id,
        "title": task.title,
        "status": task.status,
        "assignee": task.assignee,
        "priority": task.priority,
        "description": task.description,
        "done": task.done,
    ]
}

private final class MessageBoxBehavior
{
    private var onClose: ((String) -> Void)?
    private var resultByButton: [String: String] = [:]
    private var defaultResult = ""
    private(set) var closed = false

    func configure(onClose: ((String) -> Void)?, resultByButton: [String: String], defaultResult: String)
    {
        self.onClose = onClose
        self.resultByButton = resultByButton
        self.defaultResult = defaultResult
        closed = false
    }

    func handleButton(_ name: String) -> Bool
    {
        guard let result = resultByButton[name] else
        {
            return false
        }
        close(result)
        return true
    }

    func handleKey(_ key: String) -> Bool
    {
        if key == "Escape", !defaultResult.isEmpty
        {
            close(defaultResult)
            return true
        }
        return false
    }

    func buttonNameForResult(_ result: String) -> String?
    {
        resultByButton.first { $0.value == result }?.key
    }

    func close(_ result: String)
    {
        closed = true
        onClose?(result)
    }
}

private final class MessageBoxYesNo: MessageBoxYesNoUI
{
    private let behavior = MessageBoxBehavior()

    init(_ header: String, _ text: String, onClose: ((String) -> Void)? = nil)
    {
        super.init()
        behavior.configure(onClose: onClose, resultByButton: ["yes_btn": "yes", "no_btn": "no"], defaultResult: "no")
        dialog_header.setText(header)
        message.setText(text)
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = GeneratedWindowRuntimeOptions()
        options.initialFocusName = behavior.buttonNameForResult("no")
        options.onButton = { [weak self] name in self?.behavior.handleButton(name) ?? false }
        options.onKey = { [weak self] key in self?.behavior.handleKey(key) ?? false }
        options.shouldClose = { [weak self] in self?.behavior.closed ?? false }
        return options
    }

    override func onYesBtnClick()
    {
        behavior.close("yes")
    }

    override func onNoBtnClick()
    {
        behavior.close("no")
    }
}

private final class TaskFilters: TaskFiltersUI
{
    var applyRequested: (() -> Void)?
    var resetRequested: (() -> Void)?

    override func onApplyFiltersBtnClick()
    {
        applyRequested?()
    }

    override func onResetFiltersBtnClick()
    {
        resetRequested?()
    }
}

private final class TaskDialog: TaskDialogUI
{
    private let taskId: String
    var onClosed: ((TaskDialogResult?) -> Void)?

    init(_ task: TaskItem? = nil)
    {
        taskId = task?.id ?? ""
        super.init()
        if let task
        {
            header.setText("Edit task")
            name.setValue(task.title)
            selectComboValue(project_status, task.status)
            selectComboValue(owner, task.assignee)
            selectComboValue(priority, task.priority)
            note.setValue(task.description)
        }
        else
        {
            header.setText("New task")
        }
    }

    override func onAddBtnClick()
    {
        let title = name.value.trimmingCharacters(in: .whitespacesAndNewlines)
        if title.isEmpty
        {
            message.setText("Title is required.")
            return
        }
        requestClose()
        onClosed?(TaskDialogResult(
            id: taskId,
            title: title,
            status: selectedComboValue(project_status, fallback: kDefaultStatus),
            assignee: selectedComboValue(owner, fallback: kDefaultAssignee),
            priority: selectedComboValue(priority, fallback: kDefaultPriority),
            description: note.value.trimmingCharacters(in: .whitespacesAndNewlines),
            done: selectedComboValue(project_status, fallback: kDefaultStatus) == kDoneStatus
        ))
    }

    override func onCancelBtnClick()
    {
        requestClose()
        onClosed?(nil)
    }
}

private final class TaskRow: TaskRowUI
{
    let record: TaskItem
    private let onOpen: ((String) -> Void)?
    private let onDelete: ((String) -> Void)?
    private let onDone: ((String, Bool) -> Void)?

    init(
        _ record: TaskItem,
        onOpen: ((String) -> Void)?,
        onDelete: ((String) -> Void)?,
        onDone: ((String, Bool) -> Void)?
    )
    {
        self.record = record
        self.onOpen = onOpen
        self.onDelete = onDelete
        self.onDone = onDone
        super.init()
        mode = "expand_width"
        task_title.setText(record.title)
        meta.setText(record.status + " / " + record.assignee + " / " + record.priority)
        done.checked = record.done
    }

    override func onOpenBtnClick()
    {
        onOpen?(record.id)
    }

    override func onDeleteBtnClick()
    {
        onDelete?(record.id)
    }

    override func onDoneChange(_ value: String)
    {
        _ = value
        onDone?(record.id, done.checked)
    }
}

private final class TaskList: TaskListUI
{
    private let minimumRenderHeight = 1
    private var rowsProvider: (() -> [TaskItem])?
    private var onOpen: ((String) -> Void)?
    private var onDelete: ((String) -> Void)?
    private var onDone: ((String, Bool) -> Void)?

    func setRowsProvider(_ rowsProvider: @escaping () -> [TaskItem])
    {
        self.rowsProvider = rowsProvider
        refreshRows()
    }

    func setCallbacks(
        onOpen: @escaping (String) -> Void,
        onDelete: @escaping (String) -> Void,
        onDone: @escaping (String, Bool) -> Void
    )
    {
        self.onOpen = onOpen
        self.onDelete = onDelete
        self.onDone = onDone
        refreshRows()
    }

    func refreshRows()
    {
        clearChildren()
        guard let rowsProvider else
        {
            return
        }
        for row in rowsProvider()
        {
            let rowWindow = TaskRow(row, onOpen: onOpen, onDelete: onDelete, onDone: onDone)
            let naturalSize = rowWindow.generatedContentSize()
            let reusable = ReusableElement("row")
            reusable.setChild(rowWindow)
            reusable.frame = Rect(row: 0, col: 0, width: 0, height: max(minimumRenderHeight, naturalSize.height))
            _ = scrollView().addChild(reusable)
        }
        let currentFrame = scrollView().frame
        if currentFrame.width > 0 && currentFrame.height > 0
        {
            _ = scrollView().scrollToTop(Size(width: currentFrame.width, height: currentFrame.height))
        }
    }
}

private final class TaskBoard: TaskBoardUI
{
    private let filterPanel = TaskFilters()
    private let taskList = TaskList()
    private let modalStack = GeneratedWindowStack()
    private var tasks = seedTasks()
    private var pendingDeleteTaskId = ""
    private var pendingBulkAction = ""
    private var quitRequested = false

    override init()
    {
        super.init()
        filterPanel.applyRequested = { [weak self] in self?.refreshBoard() }
        filterPanel.resetRequested = { [weak self] in
            self?.resetFilters()
            self?.refreshBoard()
        }
        filters.setChild(filterPanel)
        board.setChild(taskList)
        taskList.setRowsProvider { [weak self] in
            self?.matchingTasks() ?? []
        }
        taskList.setCallbacks(
            onOpen: { [weak self] id in
                self?.openTaskById(id)
            },
            onDelete: { [weak self] id in
                self?.confirmDeleteTask(id)
            },
            onDone: { [weak self] id, done in
                self?.setTaskDone(id, done: done)
            }
        )
        refreshBoard()
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.windowStack = modalStack
        return options
    }

    override func onQuitBtnClick()
    {
        quitRequested = true
    }

    override func onNewTaskBtnClick()
    {
        openTaskDialog(nil)
    }

    override func onMarkAllDoneBtnClick()
    {
        openBulkActionDialog("mark", "Mark All Done", "Mark every task as done?")
    }

    override func onClearBoardBtnClick()
    {
        openBulkActionDialog("clear", "Clear Board", "Delete every task from the board?")
    }

    override func shouldClose() -> Bool
    {
        quitRequested
    }

    override func callAppTool(_ name: String, inputJson: String) throws -> String
    {
        switch name
        {
        case "query_tasks":
            return queryTasks(inputJson)
        case "get_task":
            return getTask(inputJson)
        case "update_task":
            return updateTask(inputJson)
        case "delete_task":
            return deleteTask(inputJson)
        default:
            return try super.callAppTool(name, inputJson: inputJson)
        }
    }

    private func matchingTasks() -> [TaskItem]
    {
        let query = filterPanel.search.value
        let statusFilter = selectedComboValue(filterPanel.status_filter, fallback: kAnyFilter)
        let assigneeFilter = selectedComboValue(filterPanel.owner_filter, fallback: kAnyFilter)
        return tasks.filter
        {
            task in
            let haystack = task.title + " " + task.description + " " + task.assignee + " " + task.priority
            if !query.isEmpty && haystack.range(of: query, options: .caseInsensitive) == nil
            {
                return false
            }
            if statusFilter != kAnyFilter && task.status != statusFilter
            {
                return false
            }
            if assigneeFilter != kAnyFilter && task.assignee != assigneeFilter
            {
                return false
            }
            return true
        }
    }

    private func matchingTasksForTool(_ inputJson: String) -> [TaskItem]
    {
        let input = jsonObject(inputJson)
        let query = jsonString(input, "title", jsonString(input, "name", ""))
        let statusFilter = jsonString(input, "status", kAnyFilter)
        let assigneeFilter = jsonString(input, "assignee", jsonString(input, "owner", kAnyFilter))
        let priorityFilter = jsonString(input, "priority", kAnyFilter)
        let doneFilter = jsonBool(input, "done")
        return tasks.filter
        {
            task in
            let haystack = task.title + " " + task.description + " " + task.assignee + " " + task.priority
            if !query.isEmpty && haystack.range(of: query, options: .caseInsensitive) == nil { return false }
            if statusFilter != kAnyFilter && task.status != statusFilter { return false }
            if assigneeFilter != kAnyFilter && task.assignee != assigneeFilter { return false }
            if priorityFilter != kAnyFilter && task.priority != priorityFilter { return false }
            if let doneFilter, task.done != doneFilter { return false }
            return true
        }
    }

    private func refreshBoard()
    {
        taskList.refreshRows()
        status.setText("\(matchingTasks().count) visible / \(tasks.count) total")
    }

    private func resetFilters()
    {
        filterPanel.search.setValue("")
        filterPanel.search.cursor = 0
        selectComboValue(filterPanel.status_filter, kAnyFilter)
        selectComboValue(filterPanel.owner_filter, kAnyFilter)
    }

    private func openBulkActionDialog(_ action: String, _ header: String, _ message: String)
    {
        pendingBulkAction = action
        let dialog = MessageBoxYesNo(header, message)
        {
            [weak self] result in self?.bulkActionConfirmed(result == "yes")
        }
        var frame = GeneratedWindowFrameOptions()
        frame.className = "MessageBoxYesNo"
        frame.initialFocusName = "no_btn"
        modalStack.push(dialog, frame)
    }

    private func bulkActionConfirmed(_ confirmed: Bool)
    {
        let action = pendingBulkAction
        pendingBulkAction = ""
        modalStack.popTop()
        if !confirmed
        {
            status.setText(kCanceledStatusText)
            return
        }
        if action == "mark"
        {
            tasks = tasks.map
            {
                var task = $0
                task.done = true
                task.status = kDoneStatus
                return task
            }
            resetFilters()
            refreshBoard()
            status.setText(kMarkAllDoneStatusText)
        }
        else if action == "clear"
        {
            tasks.removeAll()
            resetFilters()
            refreshBoard()
            status.setText(kClearBoardStatusText)
        }
    }

    private func openTaskDialog(_ task: TaskItem?)
    {
        let dialog = TaskDialog(task)
        dialog.onClosed = { [weak self] result in self?.saveTask(result) }
        modalStack.push(dialog)
    }

    private func openTaskById(_ id: String)
    {
        if let task = taskById(id)
        {
            openTaskDialog(task)
        }
    }

    private func confirmDeleteTask(_ id: String)
    {
        guard let task = taskById(id) else
        {
            return
        }
        pendingDeleteTaskId = task.id
        let dialog = MessageBoxYesNo("Delete Task", "Delete " + task.title + "?")
        {
            [weak self] result in self?.deleteConfirmed(id, confirmed: result == "yes")
        }
        var frame = GeneratedWindowFrameOptions()
        frame.className = "MessageBoxYesNo"
        frame.initialFocusName = "no_btn"
        modalStack.push(dialog, frame)
    }

    private func deleteConfirmed(_ id: String, confirmed: Bool)
    {
        pendingDeleteTaskId = ""
        modalStack.popTop()
        if !confirmed
        {
            status.setText(kCanceledStatusText)
            return
        }
        let before = tasks.count
        tasks.removeAll { $0.id == id }
        if tasks.count != before
        {
            refreshBoard()
            status.setText("Task deleted.")
        }
    }

    private func setTaskDone(_ id: String, done: Bool)
    {
        guard let index = tasks.firstIndex(where: { $0.id == id }) else
        {
            return
        }
        tasks[index].done = done
        tasks[index] = syncDoneStatus(tasks[index])
        status.setText("Updated " + tasks[index].title)
    }

    private func saveTask(_ result: TaskDialogResult?)
    {
        modalStack.popTop()
        guard let result else
        {
            status.setText(kCanceledStatusText)
            return
        }
        var index = result.id.isEmpty ? nil : tasks.firstIndex { $0.id == result.id }
        if index == nil
        {
            tasks.insert(TaskItem(id: nextTaskId(), title: "", status: kDefaultStatus, assignee: kDefaultAssignee, priority: kDefaultPriority, description: "", done: false), at: 0)
            index = 0
            resetFilters()
        }
        guard let index else
        {
            return
        }
        tasks[index] = syncDoneStatus(TaskItem(
            id: tasks[index].id,
            title: result.title,
            status: result.status.isEmpty ? kDefaultStatus : result.status,
            assignee: result.assignee.isEmpty ? kDefaultAssignee : result.assignee,
            priority: result.priority.isEmpty ? kDefaultPriority : result.priority,
            description: result.description,
            done: result.done
        ))
        refreshBoard()
        status.setText("Task saved.")
    }

    private func queryTasks(_ inputJson: String) -> String
    {
        encodeJson(["tasks": matchingTasksForTool(inputJson).map(taskJson)])
    }

    private func getTask(_ inputJson: String) -> String
    {
        let input = jsonObject(inputJson)
        if let task = taskById(jsonString(input, "id", ""))
        {
            return encodeJson(["task": taskJson(task)])
        }
        return "{\"task\":null}"
    }

    private func updateTask(_ inputJson: String) -> String
    {
        let input = jsonObject(inputJson)
        let id = jsonString(input, "id", "")
        guard let index = tasks.firstIndex(where: { $0.id == id }) else
        {
            return "{\"task\":null}"
        }
        tasks[index].title = jsonString(input, "title", tasks[index].title)
        tasks[index].status = jsonString(input, "status", tasks[index].status)
        tasks[index].assignee = jsonString(input, "assignee", tasks[index].assignee)
        tasks[index].priority = jsonString(input, "priority", tasks[index].priority)
        tasks[index].description = jsonString(input, "description", tasks[index].description)
        tasks[index].done = jsonBool(input, "done") ?? tasks[index].done
        tasks[index] = syncDoneStatus(tasks[index])
        refreshBoard()
        return encodeJson(["task": taskJson(tasks[index])])
    }

    private func deleteTask(_ inputJson: String) -> String
    {
        let input = jsonObject(inputJson)
        let id = jsonString(input, "id", "")
        let before = tasks.count
        tasks.removeAll { $0.id == id }
        let deleted = tasks.count != before
        if deleted
        {
            refreshBoard()
            status.setText("Task deleted.")
        }
        return encodeJson(["deleted": deleted])
    }

    private func taskById(_ id: String) -> TaskItem?
    {
        tasks.first { $0.id == id }
    }

    private func syncDoneStatus(_ task: TaskItem) -> TaskItem
    {
        var result = task
        if result.done
        {
            result.status = kDoneStatus
        }
        else if result.status == kDoneStatus
        {
            result.status = kDefaultStatus
        }
        return result
    }

    private func nextTaskId() -> String
    {
        var next = kInitialTaskNumberBase + 1
        for task in tasks where task.id.hasPrefix(kTaskIdPrefix)
        {
            if let number = Int(task.id.dropFirst(kTaskIdPrefix.count))
            {
                next = max(next, number + 1)
            }
        }
        return kTaskIdPrefix + String(next)
    }
}

private func seedTasks() -> [TaskItem]
{
    [
        TaskItem(id: "t-101", title: "Review invoice export", status: "Doing", assignee: "Ava", priority: "High", description: "Check exported columns and totals before finance signs off.", done: false),
        TaskItem(id: "t-102", title: "Rewrite signup emails", status: "Blocked", assignee: "Bruno", priority: "Med", description: "Waiting for brand review on the onboarding sequence.", done: false),
        TaskItem(id: "t-103", title: "Improve search ranking", status: "Doing", assignee: "Chen", priority: "High", description: "Tune ranking for exact title matches and recent activity.", done: false),
        TaskItem(id: "t-104", title: "Draft data deletion policy", status: "Todo", assignee: "Dana", priority: "Urgent", description: "Prepare policy draft for privacy review.", done: false),
        TaskItem(id: "t-105", title: "Update support templates", status: "Todo", assignee: "Ava", priority: "Med", description: "Refresh billing and account recovery snippets.", done: false),
        TaskItem(id: "t-106", title: "Fix API contract tests", status: "Blocked", assignee: "Chen", priority: "High", description: "Coordinate schema change with partner sandbox.", done: false),
        TaskItem(id: "t-107", title: "Clean left navigation", status: "Todo", assignee: "Bruno", priority: "Low", description: "Align labels and collapse unused spacing in admin views.", done: false),
        TaskItem(id: "t-108", title: "Finalize launch checklist", status: "Done", assignee: "Dana", priority: "Med", description: "Checklist completed and reviewed by release owner.", done: true),
        TaskItem(id: "t-109", title: "Define calendar sharing", status: "Doing", assignee: "Bruno", priority: "Med", description: "Document private, team, and organization visibility.", done: false),
        TaskItem(id: "t-110", title: "Clean knowledge editor", status: "Todo", assignee: "Chen", priority: "Low", description: "Remove stale toolbar actions and simplify publish flow.", done: false),
        TaskItem(id: "t-111", title: "Publish usage report", status: "Doing", assignee: "Dana", priority: "Med", description: "Send account summary to customer success leads.", done: false),
        TaskItem(id: "t-112", title: "Retry failed imports", status: "Blocked", assignee: "Ava", priority: "Urgent", description: "Needs import worker patch before rerun.", done: false),
        TaskItem(id: "t-113", title: "Tune digest settings", status: "Todo", assignee: "Bruno", priority: "Med", description: "Review default digest cadence for new workspaces.", done: false),
        TaskItem(id: "t-114", title: "Verify account password reset cleanup", status: "Done", assignee: "Chen", priority: "Low", description: "Old reset token cleanup verified in staging.", done: true),
    ]
}

@main
enum Program
{
    static func main()
    {
        let exitCode = GeneratedWindowRuntime.runGeneratedAppMain
        {
            let app = TaskBoard()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(exitCode))
    }
}
