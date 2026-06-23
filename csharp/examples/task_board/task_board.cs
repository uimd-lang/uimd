using System.Text.Json.Nodes;
using Uimd;

public sealed record TaskItem(
    string Id,
    string Title,
    string Status,
    string Assignee,
    string Priority,
    string Description,
    bool Done);

public sealed record TaskDialogResult(
    string Id,
    string Title,
    string Status,
    string Assignee,
    string Priority,
    string Description,
    bool Done);

public sealed class TaskFilters : TaskFiltersUI
{
    public Action? ApplyRequested { get; set; }
    public Action? ResetRequested { get; set; }

    protected override void onApplyFiltersBtnClick()
    {
        ApplyRequested?.Invoke();
    }

    protected override void onResetFiltersBtnClick()
    {
        ResetRequested?.Invoke();
    }
}

public sealed class TaskDialog : TaskDialogUI
{
    private const string DefaultAssignee = "Ava";
    private const string DefaultPriority = "Med";
    private const string DefaultStatus = "Todo";
    private const string DoneStatus = "Done";

    private readonly string taskId = "";

    public Action<TaskDialogResult?>? OnClosed { get; set; }

    public TaskDialog(TaskItem? task = null)
    {
        if (task is not null)
        {
            header.SetText("Edit task");
            taskId = task.Id;
            name.SetValue(task.Title);
            SelectCombo(project_status, task.Status);
            SelectCombo(owner, task.Assignee);
            SelectCombo(priority, task.Priority);
            note.SetValue(task.Description);
        }
        else
        {
            header.SetText("New task");
        }
    }

    protected override void onAddBtnClick()
    {
        string title = name.Value.Trim();
        if (string.IsNullOrWhiteSpace(title))
        {
            message.SetText("Title is required.");
            return;
        }

        TaskDialogResult result = new(
            taskId,
            title,
            string.IsNullOrEmpty(project_status.SelectedText) ? DefaultStatus : project_status.SelectedText,
            string.IsNullOrEmpty(owner.SelectedText) ? DefaultAssignee : owner.SelectedText,
            string.IsNullOrEmpty(priority.SelectedText) ? DefaultPriority : priority.SelectedText,
            note.Value.Trim(),
            project_status.SelectedText == DoneStatus);
        CloseWindow();
        OnClosed?.Invoke(result);
    }

    protected override void onCancelBtnClick()
    {
        CloseWindow();
        OnClosed?.Invoke(null);
    }

    private static void SelectCombo(ComboBox combo, string value)
    {
        int index = combo.Options.IndexOf(value);
        combo.SetSelectedIndex(index >= 0 ? index : 0);
    }
}

public sealed class TaskList : TaskListUI
{
    private readonly List<TaskItem> rows = new();

    public Action<string>? OpenRequested { get; set; }
    public Action<string>? DeleteRequested { get; set; }
    public Action<string, bool>? DoneChanged { get; set; }

    public void SetRows(IEnumerable<TaskItem> nextRows)
    {
        rows.Clear();
        rows.AddRange(nextRows);
        ScrollView().ClearChildren();
        for (int index = 0; index < rows.Count; ++index)
        {
            TaskItem task = rows[index];
            TaskRow row = new(task, OpenRequested, DeleteRequested, DoneChanged);
            ReusableElement reusable = new("board[" + index + "]");
            reusable.SetChild(row);
            row.Mode = "expand_width";
            reusable.Frame = new Rect(0, 0, 0, GeneratedWindowRuntime.GeneratedWindowContentSize(row).Height);
            AddChild(reusable);
        }
    }
}

public sealed class TaskRow : TaskRowUI
{
    private readonly TaskItem record;
    private readonly Action<string>? onOpen;
    private readonly Action<string>? onDelete;
    private readonly Action<string, bool>? onChange;

    public TaskRow(
        TaskItem record,
        Action<string>? onOpen,
        Action<string>? onDelete,
        Action<string, bool>? onChange)
    {
        this.record = record;
        this.onOpen = onOpen;
        this.onDelete = onDelete;
        this.onChange = onChange;
        SyncFromRecord();
    }

    protected override void onOpenBtnClick()
    {
        onOpen?.Invoke(record.Id);
    }

    protected override void onDeleteBtnClick()
    {
        onDelete?.Invoke(record.Id);
    }

    protected override void onDoneChange(string value)
    {
        onChange?.Invoke(record.Id, done.Checked);
    }

    private void SyncFromRecord()
    {
        task_title.SetText(record.Title);
        meta.SetText(record.Status + " / " + record.Assignee + " / " + record.Priority);
        done.SetChecked(record.Done);
    }
}

public sealed class TaskBoard : TaskBoardUI
{
    private const string AnyFilter = "Any";
    private const string DefaultAssignee = "Ava";
    private const string DefaultPriority = "Med";
    private const string DefaultStatus = "Todo";
    private const string DoneStatus = "Done";
    private const string TaskIdPrefix = "t-";
    private const int InitialTaskNumberBase = 100;
    private const string CanceledStatusText = "Action canceled.";
    private const string MarkAllDoneStatusText = "All tasks marked done.";
    private const string ClearBoardStatusText = "Board cleared.";

    private readonly TaskFilters filterPanel;
    private readonly TaskList taskList;
    private readonly List<TaskItem> tasks;
    private string pendingDeleteTaskId = "";
    private string pendingBulkAction = "";
    private bool quitRequested;

    public TaskBoard()
    {
        filterPanel = new TaskFilters();
        filterPanel.ApplyRequested = RefreshBoard;
        filterPanel.ResetRequested = () =>
        {
            ResetFilters();
            RefreshBoard();
        };
        filters.SetChild(filterPanel);

        taskList = new TaskList();
        taskList.OpenRequested = OpenTaskById;
        taskList.DeleteRequested = ConfirmDeleteTask;
        taskList.DoneChanged = SetTaskDone;
        board.SetChild(taskList);

        tasks = SeedTasks();
        RefreshBoard();
    }

    protected override void onMarkAllDoneBtnClick()
    {
        ConfirmMarkAllDone();
    }

    protected override void onClearBoardBtnClick()
    {
        ConfirmClearBoard();
    }

    protected override void onNewTaskBtnClick()
    {
        OpenTaskDialog(null);
    }

    protected override void onQuitBtnClick()
    {
        quitRequested = true;
    }

    protected override bool shouldClose()
    {
        return quitRequested;
    }

    public override string CallAppTool(string name, string inputJson)
    {
        return name switch
        {
            "query_tasks" => QueryTasks(inputJson),
            "get_task" => GetTask(inputJson),
            "update_task" => UpdateTask(inputJson),
            "delete_task" => DeleteTask(inputJson),
            _ => base.CallAppTool(name, inputJson),
        };
    }

    private IEnumerable<TaskItem> MatchingTasks()
    {
        string query = filterPanel.search.Value;
        string statusFilter = SelectedComboValue(filterPanel.status_filter, AnyFilter);
        string assigneeFilter = SelectedComboValue(filterPanel.owner_filter, AnyFilter);
        foreach (TaskItem task in tasks)
        {
            string haystack = task.Title + " " + task.Description + " " + task.Assignee + " " + task.Priority;
            if (!string.IsNullOrEmpty(query) && !ContainsText(haystack, query))
            {
                continue;
            }
            if (statusFilter != AnyFilter && task.Status != statusFilter)
            {
                continue;
            }
            if (assigneeFilter != AnyFilter && task.Assignee != assigneeFilter)
            {
                continue;
            }
            yield return task;
        }
    }

    private IEnumerable<TaskItem> MatchingTasksForTool(string inputJson)
    {
        JsonObject input = JsonNode.Parse(inputJson) as JsonObject ?? new JsonObject();
        string query = JsonString(input, "title", JsonString(input, "name", ""));
        string statusFilter = JsonString(input, "status", AnyFilter);
        string assigneeFilter = JsonString(input, "assignee", JsonString(input, "owner", AnyFilter));
        string priorityFilter = JsonString(input, "priority", AnyFilter);
        bool? doneFilter = JsonBool(input, "done");
        foreach (TaskItem task in tasks)
        {
            string haystack = task.Title + " " + task.Description + " " + task.Assignee + " " + task.Priority;
            if (!string.IsNullOrEmpty(query) && !ContainsText(haystack, query))
            {
                continue;
            }
            if (statusFilter != AnyFilter && task.Status != statusFilter)
            {
                continue;
            }
            if (assigneeFilter != AnyFilter && task.Assignee != assigneeFilter)
            {
                continue;
            }
            if (priorityFilter != AnyFilter && task.Priority != priorityFilter)
            {
                continue;
            }
            if (doneFilter.HasValue && task.Done != doneFilter.Value)
            {
                continue;
            }
            yield return task;
        }
    }

    private void RefreshBoard()
    {
        List<TaskItem> visible = MatchingTasks().ToList();
        taskList.SetRows(visible);
        status.SetText(visible.Count + " visible / " + tasks.Count + " total");
    }

    private void ResetFilters()
    {
        filterPanel.search.SetValue("");
        filterPanel.search.SetCursor(0);
        SelectComboValue(filterPanel.status_filter, AnyFilter);
        SelectComboValue(filterPanel.owner_filter, AnyFilter);
    }

    private void ConfirmMarkAllDone()
    {
        OpenBulkActionDialog("mark", "Mark All Done", "Mark every task as done?");
    }

    private void ConfirmClearBoard()
    {
        OpenBulkActionDialog("clear", "Clear Board", "Delete every task from the board?");
    }

    private void OpenBulkActionDialog(string action, string header, string message)
    {
        pendingBulkAction = action;
        MessageBoxYesNo dialog = new(header, message);
        dialog.OnClose = BulkActionConfirmed;
        OpenWindow(dialog);
    }

    private void BulkActionConfirmed(bool confirmed)
    {
        string action = pendingBulkAction;
        pendingBulkAction = "";
        if (!confirmed)
        {
            status.SetText(CanceledStatusText);
            return;
        }
        if (action == "mark")
        {
            for (int index = 0; index < tasks.Count; ++index)
            {
                TaskItem task = tasks[index] with { Done = true, Status = DoneStatus };
                tasks[index] = task;
            }
            ResetFilters();
            RefreshBoard();
            status.SetText(MarkAllDoneStatusText);
        }
        else if (action == "clear")
        {
            tasks.Clear();
            ResetFilters();
            RefreshBoard();
            status.SetText(ClearBoardStatusText);
        }
    }

    private void OpenTaskDialog(TaskItem? task)
    {
        TaskDialog dialog = new(task);
        dialog.OnClosed = SaveTask;
        OpenWindow(dialog);
    }

    private void OpenTaskById(string id)
    {
        TaskItem? task = TaskById(id);
        if (task is not null)
        {
            OpenTaskDialog(task);
        }
    }

    private void ConfirmDeleteTask(string id)
    {
        TaskItem? task = TaskById(id);
        if (task is null)
        {
            return;
        }
        pendingDeleteTaskId = task.Id;
        MessageBoxYesNo dialog = new("Delete Task", "Delete " + task.Title + "?");
        dialog.OnClose = confirmed => DeleteConfirmed(pendingDeleteTaskId, confirmed);
        OpenWindow(dialog);
    }

    private void DeleteConfirmed(string id, bool confirmed)
    {
        pendingDeleteTaskId = "";
        if (!confirmed)
        {
            status.SetText(CanceledStatusText);
            return;
        }
        int before = tasks.Count;
        tasks.RemoveAll(task => task.Id == id);
        if (tasks.Count != before)
        {
            RefreshBoard();
            status.SetText("Task deleted.");
        }
    }

    private void SetTaskDone(string id, bool done)
    {
        int index = tasks.FindIndex(task => task.Id == id);
        if (index < 0)
        {
            return;
        }
        TaskItem task = tasks[index] with { Done = done };
        task = SyncDoneStatus(task);
        tasks[index] = task;
        status.SetText("Updated " + task.Title);
    }

    private void SaveTask(TaskDialogResult? result)
    {
        if (result is null)
        {
            status.SetText(CanceledStatusText);
            return;
        }
        int index = string.IsNullOrEmpty(result.Id) ? -1 : tasks.FindIndex(task => task.Id == result.Id);
        if (index < 0)
        {
            tasks.Insert(0, new TaskItem(NextTaskId(), "", DefaultStatus, DefaultAssignee, DefaultPriority, "", false));
            index = 0;
            ResetFilters();
        }
        TaskItem saved = new(
            tasks[index].Id,
            result.Title,
            string.IsNullOrEmpty(result.Status) ? DefaultStatus : result.Status,
            string.IsNullOrEmpty(result.Assignee) ? DefaultAssignee : result.Assignee,
            string.IsNullOrEmpty(result.Priority) ? DefaultPriority : result.Priority,
            result.Description,
            result.Done);
        tasks[index] = SyncDoneStatus(saved);
        RefreshBoard();
        status.SetText("Task saved.");
    }

    private string QueryTasks(string inputJson)
    {
        JsonArray items = new();
        foreach (TaskItem task in MatchingTasksForTool(inputJson))
        {
            items.Add(TaskJson(task));
        }
        return new JsonObject { ["tasks"] = items }.ToJsonString();
    }

    private string GetTask(string inputJson)
    {
        JsonObject input = JsonNode.Parse(inputJson) as JsonObject ?? new JsonObject();
        TaskItem? task = TaskById(JsonString(input, "id", ""));
        return new JsonObject { ["task"] = task is null ? null : TaskJson(task) }.ToJsonString();
    }

    private string UpdateTask(string inputJson)
    {
        JsonObject input = JsonNode.Parse(inputJson) as JsonObject ?? new JsonObject();
        string id = JsonString(input, "id", "");
        int index = tasks.FindIndex(task => task.Id == id);
        if (index < 0)
        {
            return new JsonObject { ["task"] = null }.ToJsonString();
        }
        TaskItem task = tasks[index];
        task = task with
        {
            Title = JsonString(input, "title", task.Title),
            Status = JsonString(input, "status", task.Status),
            Assignee = JsonString(input, "assignee", task.Assignee),
            Priority = JsonString(input, "priority", task.Priority),
            Description = JsonString(input, "description", task.Description),
            Done = JsonBool(input, "done") ?? task.Done,
        };
        task = SyncDoneStatus(task);
        tasks[index] = task;
        RefreshBoard();
        return new JsonObject { ["task"] = TaskJson(task) }.ToJsonString();
    }

    private string DeleteTask(string inputJson)
    {
        JsonObject input = JsonNode.Parse(inputJson) as JsonObject ?? new JsonObject();
        string id = JsonString(input, "id", "");
        int before = tasks.Count;
        tasks.RemoveAll(task => task.Id == id);
        bool deleted = tasks.Count != before;
        if (deleted)
        {
            RefreshBoard();
            status.SetText("Task deleted.");
        }
        return new JsonObject { ["deleted"] = deleted }.ToJsonString();
    }

    private TaskItem? TaskById(string id)
    {
        return tasks.FirstOrDefault(task => task.Id == id);
    }

    private static TaskItem SyncDoneStatus(TaskItem task)
    {
        if (task.Done)
        {
            return task with { Status = DoneStatus };
        }
        if (task.Status == DoneStatus)
        {
            return task with { Status = DefaultStatus };
        }
        return task;
    }

    private string NextTaskId()
    {
        int next = InitialTaskNumberBase + 1;
        foreach (TaskItem task in tasks)
        {
            if (task.Id.StartsWith(TaskIdPrefix, StringComparison.Ordinal) &&
                int.TryParse(task.Id[TaskIdPrefix.Length..], out int number))
            {
                next = Math.Max(next, number + 1);
            }
        }
        return TaskIdPrefix + next;
    }

    private static void SelectComboValue(ComboBox combo, string value)
    {
        int index = combo.Options.IndexOf(value);
        combo.SetSelectedIndex(index >= 0 ? index : 0);
    }

    private static string SelectedComboValue(ComboBox combo, string fallback)
    {
        return combo.SelectedIndex >= 0 && combo.SelectedIndex < combo.Options.Count ? combo.Options[combo.SelectedIndex] : fallback;
    }

    private static bool ContainsText(string haystack, string needle)
    {
        return haystack.Contains(needle, StringComparison.OrdinalIgnoreCase);
    }

    private static JsonObject TaskJson(TaskItem task)
    {
        return new JsonObject
        {
            ["id"] = task.Id,
            ["title"] = task.Title,
            ["status"] = task.Status,
            ["assignee"] = task.Assignee,
            ["priority"] = task.Priority,
            ["description"] = task.Description,
            ["done"] = task.Done,
        };
    }

    private static string JsonString(JsonObject input, string name, string fallback)
    {
        return input[name]?.GetValue<string>() ?? fallback;
    }

    private static bool? JsonBool(JsonObject input, string name)
    {
        return input[name]?.GetValue<bool>();
    }

    private static List<TaskItem> SeedTasks()
    {
        return new List<TaskItem>
        {
            new("t-101", "Review invoice export", "Doing", "Ava", "High", "Check exported columns and totals before finance signs off.", false),
            new("t-102", "Rewrite signup emails", "Blocked", "Bruno", "Med", "Waiting for brand review on the onboarding sequence.", false),
            new("t-103", "Improve search ranking", "Doing", "Chen", "High", "Tune ranking for exact title matches and recent activity.", false),
            new("t-104", "Draft data deletion policy", "Todo", "Dana", "Urgent", "Prepare policy draft for privacy review.", false),
            new("t-105", "Update support templates", "Todo", "Ava", "Med", "Refresh billing and account recovery snippets.", false),
            new("t-106", "Fix API contract tests", "Blocked", "Chen", "High", "Coordinate schema change with partner sandbox.", false),
            new("t-107", "Clean left navigation", "Todo", "Bruno", "Low", "Align labels and collapse unused spacing in admin views.", false),
            new("t-108", "Finalize launch checklist", "Done", "Dana", "Med", "Checklist completed and reviewed by release owner.", true),
            new("t-109", "Define calendar sharing", "Doing", "Bruno", "Med", "Document private, team, and organization visibility.", false),
            new("t-110", "Clean knowledge editor", "Todo", "Chen", "Low", "Remove stale toolbar actions and simplify publish flow.", false),
            new("t-111", "Publish usage report", "Doing", "Dana", "Med", "Send account summary to customer success leads.", false),
            new("t-112", "Retry failed imports", "Blocked", "Ava", "Urgent", "Needs import worker patch before rerun.", false),
            new("t-113", "Tune digest settings", "Todo", "Bruno", "Med", "Review default digest cadence for new workspaces.", false),
            new("t-114", "Verify account password reset cleanup", "Done", "Chen", "Low", "Old reset token cleanup verified in staging.", true),
        };
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            TaskBoard app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
