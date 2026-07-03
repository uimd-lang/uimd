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

    init(_ record: TaskItem)
    {
        self.record = record
        super.init()
        mode = "expand_width"
        task_title.setText(record.title)
        meta.setText(record.status + " / " + record.assignee + " / " + record.priority)
        done.checked = record.done
    }
}

private final class TaskList: TaskListUI, GeneratedScrollableElementVisibility
{
    private var rows: [TaskItem] = []
    private var rowViews: [TaskRow] = []
    private var viewOffset = 0
    var activeFocusedId: String?
    var activeEditMode = false

    func setRows(_ nextRows: [TaskItem])
    {
        rows = nextRows
        rowViews = rows.map(TaskRow.init)
        viewOffset = min(viewOffset, max(0, renderedContentHeight(width: generatedContentSize().width) - 1))
    }

    override func scrollBy(_ delta: Int, viewport: Size)
    {
        _ = scrollLines(delta, viewport: viewport)
    }

    override func handleScrollKey(_ key: String, viewport: Size) -> Bool
    {
        switch key
        {
        case "ArrowUp", "Up":
            return scrollLines(-generatedScrollKeyboardStepRows, viewport: viewport)
        case "ArrowDown", "Down":
            return scrollLines(generatedScrollKeyboardStepRows, viewport: viewport)
        case "PageUp":
            return scrollLines(-max(1, viewport.height), viewport: viewport)
        case "PageDown":
            return scrollLines(max(1, viewport.height), viewport: viewport)
        case "Home":
            let changed = viewOffset != 0
            viewOffset = 0
            return changed
        case "End":
            let previous = viewOffset
            viewOffset = maxLineOffset(viewport: viewport)
            return viewOffset != previous
        default:
            return false
        }
    }

    func ensureElementVisible(_ elementId: String, viewport: Size)
    {
        guard let index = rowIndex(from: elementId),
              index >= 0,
              index < rowViews.count
        else
        {
            return
        }
        let scrollViewport = generatedScrollViewport(size: viewport, style: panelStyle())
        let viewportHeight = max(1, scrollViewport.height)
        var rowStart = 0
        for rowIndex in 0..<index
        {
            rowStart += rowHeight(rowViews[rowIndex], width: max(1, scrollViewport.width))
            if rowIndex < rowViews.count - 1
            {
                rowStart += panelGap()
            }
        }
        let rowEnd = rowStart + rowHeight(rowViews[index], width: max(1, scrollViewport.width))
        if rowStart < viewOffset
        {
            viewOffset = rowStart
        }
        else if rowEnd > viewOffset + viewportHeight
        {
            viewOffset = max(0, rowEnd - viewportHeight - generatedScrollIndicatorOverlapRows)
        }
        viewOffset = max(0, min(maxLineOffset(viewport: viewport), viewOffset))
    }

    func focusOrder(hostId: String) -> [String]
    {
        rowViews.indices.flatMap
        {
            ["\(hostId)[\($0)].done", "\(hostId)[\($0)].open_btn", "\(hostId)[\($0)].delete_btn"]
        }
    }

    func registerDynamicElements(on root: GeneratedWindowBase, hostId: String, hostFrame: Rect)
    {
        let viewport = generatedScrollViewport(size: Size(width: hostFrame.width, height: hostFrame.height), style: panelStyle())
        let gap = panelGap()
        var rowCursor = 0
        for index in rowViews.indices
        {
            let row = rowViews[index]
            let prefix = "\(hostId)[\(index)]"
            let rowHeight = rowHeight(row, width: max(1, viewport.width))
            for element in row.elements
            {
                let id = prefix + "." + element.name
                element.frame = Rect(
                    row: hostFrame.row + viewport.row + rowCursor + element.frame.row - viewOffset,
                    col: hostFrame.col + viewport.col + element.frame.col,
                    width: element.frame.width,
                    height: element.frame.height
                )
                root.registerDynamicElement(id, element: element, parentFocusHostId: hostId)
            }
            rowCursor += rowHeight
            if index + 1 < rowViews.count
            {
                rowCursor += gap
            }
        }
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        _ = focusedName
        _ = editMode
        let width = max(1, size.width)
        let height = max(1, size.height)
        let style = panelStyle()
        let blank = Array(repeating: TerminalCell(" ", foreground: style.color, background: style.background), count: width)
        var output = Array(repeating: blank, count: height)
        let viewport = generatedScrollViewport(size: Size(width: width, height: height), style: style)
        guard viewport.width > 0 && viewport.height > 0 else
        {
            return output
        }
        let rows = renderedRows(width: max(1, viewport.width), focusedName: focusedName, editMode: editMode)
        let maxOffset = max(0, rows.count - viewport.height)
        viewOffset = min(viewOffset, maxOffset)
        for row in 0..<viewport.height
        {
            let sourceRow = viewOffset + row
            guard sourceRow >= 0 && sourceRow < rows.count else
            {
                continue
            }
            let targetRow = viewport.row + row
            guard targetRow >= 0 && targetRow < output.count else
            {
                continue
            }
            let source = rows[sourceRow]
            for col in 0..<min(viewport.width, source.count)
            {
                let targetCol = viewport.col + col
                guard targetCol >= 0 && targetCol < output[targetRow].count else
                {
                    continue
                }
                output[targetRow][targetCol] = source[col]
            }
        }
        applyGeneratedScrollIndicators(
            content: &output,
            viewOffset: viewOffset,
            maxViewOffset: maxOffset,
            viewport: Size(width: width, height: height),
            style: style,
            childContentBelowViewport: viewOffset + viewport.height < rows.count
        )
        return output
    }

    private func renderedRows(width: Int, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        var rows: [[TerminalCell]] = []
        let style = panelStyle()
        let gap = panelGap()
        let gapRow = Array(repeating: TerminalCell(" ", foreground: style.color, background: style.background), count: max(1, width))
        for index in rowViews.indices
        {
            let rowView = rowViews[index]
            let focusedLocalName: String?
            if let activeFocusedId, activeFocusedId.hasPrefix("board[\(index)].")
            {
                focusedLocalName = String(activeFocusedId.split(separator: ".").last ?? "")
            }
            else
            {
                focusedLocalName = nil
            }
            let rowHeight = rowHeight(rowView, width: width)
            let rendered = renderGeneratedContentForHost(
                rowView,
                size: Size(width: width, height: rowHeight),
                focusedName: focusedLocalName,
                editMode: activeEditMode && focusedLocalName != nil
            )
            for localRow in 0..<min(rowHeight, rendered.count)
            {
                rows.append(rendered[localRow])
            }
            if index + 1 < rowViews.count
            {
                for _ in 0..<gap
                {
                    rows.append(gapRow)
                }
            }
        }
        return rows
    }

    private func renderedContentHeight(width: Int) -> Int
    {
        let rowsHeight = rowViews.reduce(0) { $0 + rowHeight($1, width: max(1, width)) }
        return rowsHeight + max(0, rowViews.count - 1) * panelGap()
    }

    private func maxLineOffset(viewport: Size) -> Int
    {
        let scrollViewport = generatedScrollViewport(size: viewport, style: panelStyle())
        return max(0, renderedContentHeight(width: max(1, scrollViewport.width)) - max(1, scrollViewport.height))
    }

    @discardableResult
    private func scrollLines(_ delta: Int, viewport: Size) -> Bool
    {
        let previous = viewOffset
        viewOffset = max(0, min(maxLineOffset(viewport: viewport), viewOffset + delta))
        return viewOffset != previous
    }

    private func rowIndex(from id: String) -> Int?
    {
        guard let open = id.firstIndex(of: "["),
              let close = id[open...].firstIndex(of: "]")
        else
        {
            return nil
        }
        return Int(id[id.index(after: open)..<close])
    }

    private func rowHeight(_ rowView: TaskRow, width: Int) -> Int
    {
        max(1, rowView.generatedContentSizeForWidth(max(1, width)).height)
    }

    private func panelStyle() -> Style
    {
        scrollView().style
    }

    private func panelGap() -> Int
    {
        generatedScrollGap(style: panelStyle())
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
        refreshBoard()
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.windowStack = modalStack
        return options
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        clearDynamicElements()
        taskList.activeFocusedId = focusedName
        taskList.activeEditMode = editMode
        let rendered = super.renderContent(size: size, focusedName: focusedName, editMode: editMode)
        taskList.registerDynamicElements(on: self, hostId: "board", hostFrame: board.frame)
        board.childFocusOrder = taskList.focusOrder(hostId: "board")
        registerPanel(filterPanel, hostId: "filters", hostFrame: filters.frame)
        filters.childFocusOrder = filterPanel.elements.filter { $0.focusable }.map { "filters." + $0.name }
        return rendered
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

    override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name.hasPrefix("board[")
        {
            return handleBoardButton(name)
        }
        if name == "filters.apply_filters_btn"
        {
            refreshBoard()
            return true
        }
        if name == "filters.reset_filters_btn"
        {
            resetFilters()
            refreshBoard()
            return true
        }
        return super.handleGeneratedButton(name)
    }

    override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name.hasPrefix("board["), name.hasSuffix(".done"), let index = rowIndex(name)
        {
            let visible = matchingTasks()
            guard index >= 0 && index < visible.count else
            {
                return true
            }
            setTaskDone(visible[index].id, done: value == "true")
            return true
        }
        return super.handleGeneratedTextChanged(name, value: value)
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

    private func registerPanel(_ panel: GeneratedWindowBase, hostId: String, hostFrame: Rect)
    {
        for element in panel.elements
        {
            element.frame = Rect(row: hostFrame.row + element.frame.row, col: hostFrame.col + element.frame.col, width: element.frame.width, height: element.frame.height)
            registerDynamicElement(hostId + "." + element.name, element: element, parentFocusHostId: hostId)
        }
    }

    private func handleBoardButton(_ name: String) -> Bool
    {
        guard let index = rowIndex(name) else
        {
            return false
        }
        let visible = matchingTasks()
        guard index >= 0 && index < visible.count else
        {
            return false
        }
        let id = visible[index].id
        if name.hasSuffix(".open_btn")
        {
            openTaskById(id)
            return true
        }
        if name.hasSuffix(".delete_btn")
        {
            confirmDeleteTask(id)
            return true
        }
        return false
    }

    private func rowIndex(_ id: String) -> Int?
    {
        guard let open = id.firstIndex(of: "["),
              let close = id[open...].firstIndex(of: "]")
        else
        {
            return nil
        }
        return Int(id[id.index(after: open)..<close])
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
        let visible = matchingTasks()
        taskList.setRows(visible)
        status.setText("\(visible.count) visible / \(tasks.count) total")
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
