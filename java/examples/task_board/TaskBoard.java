import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Optional;
import java.util.function.Consumer;

import uimd.ComboBox;
import uimd.GeneratedWindowRuntime;
import uimd.MessageBoxYesNo;
import uimd.Rect;
import uimd.RenderHelpers;
import uimd.ReusableElement;
import uimd.Size;

public final class TaskBoard extends TaskBoardUI
{
    private static final String ANY_FILTER = "Any";
    private static final String DEFAULT_ASSIGNEE = "Ava";
    private static final String DEFAULT_PRIORITY = "Med";
    private static final String DEFAULT_STATUS = "Todo";
    private static final String DONE_STATUS = "Done";
    private static final String TASK_ID_PREFIX = "t-";
    private static final int INITIAL_TASK_NUMBER_BASE = 100;
    private static final String CANCELED_STATUS_TEXT = "Action canceled.";
    private static final String MARK_ALL_DONE_STATUS_TEXT = "All tasks marked done.";
    private static final String CLEAR_BOARD_STATUS_TEXT = "Board cleared.";
    private static final int EXPECTED_SEED_TASKS = 14;
    private static final int SMOKE_RENDER_WIDTH = 112;
    private static final int SMOKE_RENDER_HEIGHT = 38;

    private final TaskFilters filterPanel;
    private final TaskList taskList;
    private final List<TaskItem> tasks;
    private String pendingDeleteTaskId = "";
    private BulkAction pendingBulkAction = BulkAction.NONE;
    private boolean quitRequested;

    public TaskBoard()
    {
        filterPanel = new TaskFilters();
        filterPanel.setApplyRequested(this::refreshBoard);
        filterPanel.setResetRequested(() ->
        {
            resetFilters();
            refreshBoard();
        });
        filters.setChild(filterPanel);

        taskList = new TaskList();
        taskList.setOpenRequested(this::openTaskById);
        taskList.setDeleteRequested(this::confirmDeleteTask);
        taskList.setDoneChanged(this::setTaskDone);
        board.setChild(taskList);

        tasks = seedTasks();
        refreshBoard();
    }

    @Override
    protected void onMarkAllDoneBtnClick()
    {
        confirmMarkAllDone();
    }

    @Override
    protected void onClearBoardBtnClick()
    {
        confirmClearBoard();
    }

    @Override
    protected void onNewTaskBtnClick()
    {
        openTaskDialog(null);
    }

    @Override
    protected void onQuitBtnClick()
    {
        quitRequested = true;
    }

    @Override
    protected boolean shouldClose()
    {
        return quitRequested;
    }

    @Override
    public String callAppTool(String name, String inputJson)
    {
        return switch (name)
        {
            case "query_tasks" -> queryTasks(inputJson);
            case "get_task" -> getTask(inputJson);
            case "update_task" -> updateTask(inputJson);
            case "delete_task" -> deleteTask(inputJson);
            default -> super.callAppTool(name, inputJson);
        };
    }

    private List<TaskItem> matchingTasks()
    {
        String query = filterPanel.search.value();
        String statusFilter = selectedComboValue(filterPanel.status_filter, ANY_FILTER);
        String assigneeFilter = selectedComboValue(filterPanel.owner_filter, ANY_FILTER);
        List<TaskItem> matches = new ArrayList<>();
        for (TaskItem task : tasks)
        {
            String haystack = task.title() + " " + task.description() + " "
                + task.assignee() + " " + task.priority();
            if (!query.isEmpty() && !containsText(haystack, query))
            {
                continue;
            }
            if (!ANY_FILTER.equals(statusFilter) && !task.status().equals(statusFilter))
            {
                continue;
            }
            if (!ANY_FILTER.equals(assigneeFilter) && !task.assignee().equals(assigneeFilter))
            {
                continue;
            }
            matches.add(task);
        }
        return matches;
    }

    private List<TaskItem> matchingTasksForTool(String inputJson)
    {
        String query = jsonStringField(inputJson, "title")
            .orElseGet(() -> jsonStringField(inputJson, "name").orElse(""));
        String statusFilter = jsonStringField(inputJson, "status").orElse(ANY_FILTER);
        String assigneeFilter = jsonStringField(inputJson, "assignee")
            .orElseGet(() -> jsonStringField(inputJson, "owner").orElse(ANY_FILTER));
        String priorityFilter = jsonStringField(inputJson, "priority").orElse(ANY_FILTER);
        Optional<Boolean> doneFilter = jsonBoolField(inputJson, "done");
        List<TaskItem> matches = new ArrayList<>();
        for (TaskItem task : tasks)
        {
            String haystack = task.title() + " " + task.description() + " "
                + task.assignee() + " " + task.priority();
            if (!query.isEmpty() && !containsText(haystack, query))
            {
                continue;
            }
            if (!ANY_FILTER.equals(statusFilter) && !task.status().equals(statusFilter))
            {
                continue;
            }
            if (!ANY_FILTER.equals(assigneeFilter) && !task.assignee().equals(assigneeFilter))
            {
                continue;
            }
            if (!ANY_FILTER.equals(priorityFilter) && !task.priority().equals(priorityFilter))
            {
                continue;
            }
            if (doneFilter.isPresent() && task.done() != doneFilter.get())
            {
                continue;
            }
            matches.add(task);
        }
        return matches;
    }

    private void refreshBoard()
    {
        List<TaskItem> visible = matchingTasks();
        taskList.setRows(visible);
        status.setText(visible.size() + " visible / " + tasks.size() + " total");
    }

    private void resetFilters()
    {
        filterPanel.search.setValue("");
        filterPanel.search.setCursor(0);
        selectComboValue(filterPanel.status_filter, ANY_FILTER);
        selectComboValue(filterPanel.owner_filter, ANY_FILTER);
    }

    private void confirmMarkAllDone()
    {
        openBulkActionDialog(
            BulkAction.MARK_ALL_DONE,
            "Mark All Done",
            "Mark every task as done?");
    }

    private void confirmClearBoard()
    {
        openBulkActionDialog(
            BulkAction.CLEAR_BOARD,
            "Clear Board",
            "Delete every task from the board?");
    }

    private void openBulkActionDialog(BulkAction action, String header, String message)
    {
        pendingBulkAction = action;
        openWindow(new MessageBoxYesNo(
            header,
            message,
            result -> bulkActionConfirmed("yes".equals(result))));
    }

    private void bulkActionConfirmed(boolean confirmed)
    {
        BulkAction action = pendingBulkAction;
        pendingBulkAction = BulkAction.NONE;
        if (!confirmed)
        {
            status.setText(CANCELED_STATUS_TEXT);
            return;
        }
        if (action == BulkAction.MARK_ALL_DONE)
        {
            for (int index = 0; index < tasks.size(); ++index)
            {
                TaskItem task = tasks.get(index);
                tasks.set(index, new TaskItem(
                    task.id(),
                    task.title(),
                    DONE_STATUS,
                    task.assignee(),
                    task.priority(),
                    task.description(),
                    true));
            }
            resetFilters();
            refreshBoard();
            status.setText(MARK_ALL_DONE_STATUS_TEXT);
        }
        else if (action == BulkAction.CLEAR_BOARD)
        {
            tasks.clear();
            resetFilters();
            refreshBoard();
            status.setText(CLEAR_BOARD_STATUS_TEXT);
        }
    }

    private void openTaskDialog(TaskItem task)
    {
        TaskDialog dialog = new TaskDialog(task);
        dialog.setOnClosed(this::saveTask);
        openWindow(dialog);
    }

    private void openTaskById(String id)
    {
        TaskItem task = taskById(id);
        if (task != null)
        {
            openTaskDialog(task);
        }
    }

    private void confirmDeleteTask(String id)
    {
        TaskItem task = taskById(id);
        if (task == null)
        {
            return;
        }
        pendingDeleteTaskId = task.id();
        openWindow(new MessageBoxYesNo(
            "Delete Task",
            "Delete " + task.title() + "?",
            result -> deleteConfirmed(pendingDeleteTaskId, "yes".equals(result))));
    }

    private void deleteConfirmed(String id, boolean confirmed)
    {
        pendingDeleteTaskId = "";
        if (!confirmed)
        {
            status.setText(CANCELED_STATUS_TEXT);
            return;
        }
        int before = tasks.size();
        tasks.removeIf(task -> task.id().equals(id));
        if (tasks.size() != before)
        {
            refreshBoard();
            status.setText("Task deleted.");
        }
    }

    private void setTaskDone(String id, boolean done)
    {
        int index = taskIndex(id);
        if (index < 0)
        {
            return;
        }
        TaskItem current = tasks.get(index);
        TaskItem updated = syncDoneStatus(new TaskItem(
            current.id(),
            current.title(),
            current.status(),
            current.assignee(),
            current.priority(),
            current.description(),
            done));
        tasks.set(index, updated);
        status.setText("Updated " + updated.title());
    }

    private void saveTask(TaskDialogResult result)
    {
        if (result == null)
        {
            status.setText(CANCELED_STATUS_TEXT);
            return;
        }
        int index = result.id().isEmpty() ? -1 : taskIndex(result.id());
        if (index < 0)
        {
            tasks.add(0, new TaskItem(
                nextTaskId(),
                "",
                DEFAULT_STATUS,
                DEFAULT_ASSIGNEE,
                DEFAULT_PRIORITY,
                "",
                false));
            index = 0;
            resetFilters();
        }
        TaskItem saved = syncDoneStatus(new TaskItem(
            tasks.get(index).id(),
            result.title(),
            result.status().isEmpty() ? DEFAULT_STATUS : result.status(),
            result.assignee().isEmpty() ? DEFAULT_ASSIGNEE : result.assignee(),
            result.priority().isEmpty() ? DEFAULT_PRIORITY : result.priority(),
            result.description(),
            result.done()));
        tasks.set(index, saved);
        refreshBoard();
        status.setText("Task saved.");
    }

    private String queryTasks(String inputJson)
    {
        StringBuilder result = new StringBuilder("{\"tasks\":[");
        boolean first = true;
        for (TaskItem task : matchingTasksForTool(inputJson))
        {
            if (!first)
            {
                result.append(',');
            }
            first = false;
            result.append(taskJson(task));
        }
        return result.append("]}").toString();
    }

    private String getTask(String inputJson)
    {
        String id = jsonStringField(inputJson, "id").orElse("");
        TaskItem task = taskById(id);
        return "{\"task\":" + (task == null ? "null" : taskJson(task)) + "}";
    }

    private String updateTask(String inputJson)
    {
        String id = jsonStringField(inputJson, "id").orElse("");
        int index = taskIndex(id);
        if (index < 0)
        {
            return "{\"task\":null}";
        }
        TaskItem current = tasks.get(index);
        String nextStatus = jsonStringField(inputJson, "status").orElse(current.status());
        String nextAssignee = jsonStringField(inputJson, "assignee").orElse(current.assignee());
        String nextPriority = jsonStringField(inputJson, "priority").orElse(current.priority());
        TaskItem updated = syncDoneStatus(new TaskItem(
            current.id(),
            jsonStringField(inputJson, "title").orElse(current.title()),
            nextStatus.isEmpty() ? DEFAULT_STATUS : nextStatus,
            nextAssignee.isEmpty() ? DEFAULT_ASSIGNEE : nextAssignee,
            nextPriority.isEmpty() ? DEFAULT_PRIORITY : nextPriority,
            jsonStringField(inputJson, "description").orElse(current.description()),
            jsonBoolField(inputJson, "done").orElse(current.done())));
        tasks.set(index, updated);
        refreshBoard();
        return "{\"task\":" + taskJson(updated) + "}";
    }

    private String deleteTask(String inputJson)
    {
        String id = jsonStringField(inputJson, "id").orElse("");
        int before = tasks.size();
        tasks.removeIf(task -> task.id().equals(id));
        boolean deleted = tasks.size() != before;
        if (deleted)
        {
            refreshBoard();
            status.setText("Task deleted.");
        }
        return "{\"deleted\":" + deleted + "}";
    }

    private TaskItem taskById(String id)
    {
        int index = taskIndex(id);
        return index < 0 ? null : tasks.get(index);
    }

    private int taskIndex(String id)
    {
        for (int index = 0; index < tasks.size(); ++index)
        {
            if (tasks.get(index).id().equals(id))
            {
                return index;
            }
        }
        return -1;
    }

    private static TaskItem syncDoneStatus(TaskItem task)
    {
        if (task.done())
        {
            return new TaskItem(
                task.id(),
                task.title(),
                DONE_STATUS,
                task.assignee(),
                task.priority(),
                task.description(),
                true);
        }
        if (DONE_STATUS.equals(task.status()))
        {
            return new TaskItem(
                task.id(),
                task.title(),
                DEFAULT_STATUS,
                task.assignee(),
                task.priority(),
                task.description(),
                false);
        }
        return task;
    }

    private String nextTaskId()
    {
        int next = INITIAL_TASK_NUMBER_BASE + 1;
        for (TaskItem task : tasks)
        {
            next = Math.max(next, taskNumber(task.id()) + 1);
        }
        return TASK_ID_PREFIX + next;
    }

    private static int taskNumber(String id)
    {
        if (!id.startsWith(TASK_ID_PREFIX))
        {
            return INITIAL_TASK_NUMBER_BASE;
        }
        try
        {
            return Integer.parseInt(id.substring(TASK_ID_PREFIX.length()));
        }
        catch (NumberFormatException exception)
        {
            return INITIAL_TASK_NUMBER_BASE;
        }
    }

    private static void selectComboValue(ComboBox combo, String value)
    {
        int index = combo.options().indexOf(value);
        combo.setSelectedIndex(index >= 0 ? index : 0);
    }

    private static String selectedComboValue(ComboBox combo, String fallback)
    {
        int index = combo.selectedIndex();
        return index >= 0 && index < combo.options().size()
            ? combo.options().get(index)
            : fallback;
    }

    private static boolean containsText(String haystack, String needle)
    {
        return haystack.toLowerCase(Locale.ROOT).contains(needle.toLowerCase(Locale.ROOT));
    }

    private static String taskJson(TaskItem task)
    {
        return "{\"id\":\"" + jsonEscape(task.id())
            + "\",\"title\":\"" + jsonEscape(task.title())
            + "\",\"status\":\"" + jsonEscape(task.status())
            + "\",\"assignee\":\"" + jsonEscape(task.assignee())
            + "\",\"priority\":\"" + jsonEscape(task.priority())
            + "\",\"description\":\"" + jsonEscape(task.description())
            + "\",\"done\":" + task.done() + "}";
    }

    private static String jsonEscape(String text)
    {
        StringBuilder result = new StringBuilder();
        for (int index = 0; index < text.length(); ++index)
        {
            char character = text.charAt(index);
            if (character == '\\' || character == '"')
            {
                result.append('\\').append(character);
            }
            else if (character == '\n')
            {
                result.append("\\n");
            }
            else
            {
                result.append(character);
            }
        }
        return result.toString();
    }

    private static Optional<String> jsonStringField(String objectJson, String field)
    {
        Optional<Integer> index = jsonValueStart(objectJson, field);
        if (index.isEmpty())
        {
            return Optional.empty();
        }
        ParsedJsonString parsed = parseJsonStringAt(normalizeJson(objectJson), index.get());
        return parsed == null ? Optional.empty() : Optional.of(parsed.value());
    }

    private static Optional<Boolean> jsonBoolField(String objectJson, String field)
    {
        String json = normalizeJson(objectJson);
        Optional<Integer> index = jsonValueStart(json, field);
        if (index.isEmpty())
        {
            return Optional.empty();
        }
        String value = json.substring(index.get());
        if (value.startsWith("true"))
        {
            return Optional.of(true);
        }
        if (value.startsWith("false"))
        {
            return Optional.of(false);
        }
        return Optional.empty();
    }

    private static Optional<Integer> jsonValueStart(String objectJson, String field)
    {
        String json = normalizeJson(objectJson);
        int index = 0;
        while (index < json.length())
        {
            if (json.charAt(index) != '"')
            {
                ++index;
                continue;
            }
            ParsedJsonString key = parseJsonStringAt(json, index);
            if (key == null)
            {
                return Optional.empty();
            }
            int keyIndex = skipWhitespace(json, key.nextIndex());
            if (keyIndex >= json.length() || json.charAt(keyIndex) != ':')
            {
                index = keyIndex;
                continue;
            }
            keyIndex = skipWhitespace(json, keyIndex + 1);
            if (key.value().equals(field))
            {
                return Optional.of(keyIndex);
            }
            index = keyIndex;
        }
        return Optional.empty();
    }

    private static ParsedJsonString parseJsonStringAt(String text, int start)
    {
        if (start < 0 || start >= text.length() || text.charAt(start) != '"')
        {
            return null;
        }
        StringBuilder result = new StringBuilder();
        int index = start + 1;
        while (index < text.length())
        {
            char character = text.charAt(index++);
            if (character == '"')
            {
                return new ParsedJsonString(result.toString(), index);
            }
            if (character == '\\' && index < text.length())
            {
                char escaped = text.charAt(index++);
                result.append(escaped == 'n' ? '\n' : escaped);
                continue;
            }
            result.append(character);
        }
        return null;
    }

    private static int skipWhitespace(String text, int start)
    {
        int index = start;
        while (index < text.length() && Character.isWhitespace(text.charAt(index)))
        {
            ++index;
        }
        return index;
    }

    private static String normalizeJson(String value)
    {
        return value == null ? "" : value;
    }

    private static List<TaskItem> seedTasks()
    {
        return new ArrayList<>(List.of(
            new TaskItem(
                "t-101",
                "Review invoice export",
                "Doing",
                "Ava",
                "High",
                "Check exported columns and totals before finance signs off.",
                false),
            new TaskItem(
                "t-102",
                "Rewrite signup emails",
                "Blocked",
                "Bruno",
                "Med",
                "Waiting for brand review on the onboarding sequence.",
                false),
            new TaskItem(
                "t-103",
                "Improve search ranking",
                "Doing",
                "Chen",
                "High",
                "Tune ranking for exact title matches and recent activity.",
                false),
            new TaskItem(
                "t-104",
                "Draft data deletion policy",
                "Todo",
                "Dana",
                "Urgent",
                "Prepare policy draft for privacy review.",
                false),
            new TaskItem(
                "t-105",
                "Update support templates",
                "Todo",
                "Ava",
                "Med",
                "Refresh billing and account recovery snippets.",
                false),
            new TaskItem(
                "t-106",
                "Fix API contract tests",
                "Blocked",
                "Chen",
                "High",
                "Coordinate schema change with partner sandbox.",
                false),
            new TaskItem(
                "t-107",
                "Clean left navigation",
                "Todo",
                "Bruno",
                "Low",
                "Align labels and collapse unused spacing in admin views.",
                false),
            new TaskItem(
                "t-108",
                "Finalize launch checklist",
                "Done",
                "Dana",
                "Med",
                "Checklist completed and reviewed by release owner.",
                true),
            new TaskItem(
                "t-109",
                "Define calendar sharing",
                "Doing",
                "Bruno",
                "Med",
                "Document private, team, and organization visibility.",
                false),
            new TaskItem(
                "t-110",
                "Clean knowledge editor",
                "Todo",
                "Chen",
                "Low",
                "Remove stale toolbar actions and simplify publish flow.",
                false),
            new TaskItem(
                "t-111",
                "Publish usage report",
                "Doing",
                "Dana",
                "Med",
                "Send account summary to customer success leads.",
                false),
            new TaskItem(
                "t-112",
                "Retry failed imports",
                "Blocked",
                "Ava",
                "Urgent",
                "Needs import worker patch before rerun.",
                false),
            new TaskItem(
                "t-113",
                "Tune digest settings",
                "Todo",
                "Bruno",
                "Med",
                "Review default digest cadence for new workspaces.",
                false),
            new TaskItem(
                "t-114",
                "Verify account password reset cleanup",
                "Done",
                "Chen",
                "Low",
                "Old reset token cleanup verified in staging.",
                true)));
    }

    private static int run(String[] arguments)
    {
        if (arguments.length > 0 && "--smoke".equals(arguments[0]))
        {
            smoke();
            return 0;
        }
        if (arguments.length > 0 && "--logic-test".equals(arguments[0]))
        {
            logicTest();
            return 0;
        }
        TaskBoard application = new TaskBoard();
        return GeneratedWindowRuntime.runGeneratedWindow(
            application,
            application.runtimeOptions(),
            arguments);
    }

    private static void smoke()
    {
        TaskBoard window = new TaskBoard();
        GeneratedWindowRuntime.renderGeneratedWindowContent(
            window,
            new Size(SMOKE_RENDER_WIDTH, SMOKE_RENDER_HEIGHT),
            -1);
        require("Task Board".equals(window.title()), "unexpected title");
        require(window.filters.child() instanceof TaskFilters, "filter panel is not specialized");
        require(window.board.child() instanceof TaskList, "task list is not specialized");
        require(window.tasks.size() == EXPECTED_SEED_TASKS, "unexpected seed task count");
        require(
            (EXPECTED_SEED_TASKS + " visible / " + EXPECTED_SEED_TASKS + " total")
                .equals(window.status.text()),
            "unexpected initial status");
        require(
            window.board.frame().width() > window.filterPanel.search.frame().width(),
            "task board should be wider than the filter search after rendering");
    }

    private static void logicTest()
    {
        TaskBoard application = new TaskBoard();
        require(application.tasks.size() == EXPECTED_SEED_TASKS, "unexpected seed task count");
        List<String> rendered = RenderHelpers.renderedText(
            GeneratedWindowRuntime.renderGeneratedWindowContent(
                application,
                new Size(SMOKE_RENDER_WIDTH, SMOKE_RENDER_HEIGHT),
                -1));
        String boardText = String.join("\n", rendered);
        require(boardText.contains("Review invoice export"), "first task is not rendered");
        require(boardText.contains("Open"), "open action is not rendered");
        require(boardText.contains("Delete"), "delete action is not rendered");

        application.filterPanel.search.setValue("calendar");
        application.refreshBoard();
        require(application.matchingTasks().size() == 1, "search filter did not narrow tasks");
        application.resetFilters();
        application.refreshBoard();
        require(
            application.matchingTasks().size() == EXPECTED_SEED_TASKS,
            "reset did not restore all tasks");

        application.saveTask(new TaskDialogResult(
            "",
            "Write Java task board",
            DEFAULT_STATUS,
            DEFAULT_ASSIGNEE,
            DEFAULT_PRIORITY,
            "Port task board example.",
            false));
        require(
            application.tasks.size() == EXPECTED_SEED_TASKS + 1,
            "new task was not saved");
        require("Task saved.".equals(application.status.text()), "save status was not set");

        application.onQuitBtnClick();
        require(application.shouldClose(), "quit did not request close");
    }

    private static void require(boolean condition, String message)
    {
        if (!condition)
        {
            throw new IllegalStateException(message);
        }
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() -> run(arguments));
        System.exit(exitCode);
    }

    private enum BulkAction
    {
        NONE,
        MARK_ALL_DONE,
        CLEAR_BOARD,
    }

    private record TaskItem(
        String id,
        String title,
        String status,
        String assignee,
        String priority,
        String description,
        boolean done)
    {
    }

    private record TaskDialogResult(
        String id,
        String title,
        String status,
        String assignee,
        String priority,
        String description,
        boolean done)
    {
    }

    private record ParsedJsonString(String value, int nextIndex)
    {
    }

    @FunctionalInterface
    private interface TaskDoneChanged
    {
        void accept(String id, boolean done);
    }

    private static final class TaskFilters extends TaskFiltersUI
    {
        private Runnable applyRequested;
        private Runnable resetRequested;

        void setApplyRequested(Runnable value)
        {
            applyRequested = value;
        }

        void setResetRequested(Runnable value)
        {
            resetRequested = value;
        }

        @Override
        protected void onApplyFiltersBtnClick()
        {
            if (applyRequested != null)
            {
                applyRequested.run();
            }
        }

        @Override
        protected void onResetFiltersBtnClick()
        {
            if (resetRequested != null)
            {
                resetRequested.run();
            }
        }
    }

    private static final class TaskDialog extends TaskDialogUI
    {
        private final String taskId;
        private Consumer<TaskDialogResult> onClosed;

        TaskDialog(TaskItem task)
        {
            taskId = task == null ? "" : task.id();
            if (task == null)
            {
                header.setText("New task");
                selectComboValue(project_status, DEFAULT_STATUS);
                selectComboValue(owner, DEFAULT_ASSIGNEE);
                selectComboValue(priority, DEFAULT_PRIORITY);
            }
            else
            {
                header.setText("Edit task");
                name.setValue(task.title());
                selectComboValue(
                    project_status,
                    task.status().isEmpty() ? DEFAULT_STATUS : task.status());
                selectComboValue(
                    owner,
                    task.assignee().isEmpty() ? DEFAULT_ASSIGNEE : task.assignee());
                selectComboValue(
                    priority,
                    task.priority().isEmpty() ? DEFAULT_PRIORITY : task.priority());
                note.setValue(task.description());
            }
        }

        void setOnClosed(Consumer<TaskDialogResult> value)
        {
            onClosed = value;
        }

        @Override
        protected void onAddBtnClick()
        {
            String title = name.value().trim();
            if (title.isEmpty())
            {
                message.setText("Title is required.");
                return;
            }
            String selectedStatus = selectedComboValue(project_status, DEFAULT_STATUS);
            TaskDialogResult result = new TaskDialogResult(
                taskId,
                title,
                selectedStatus,
                selectedComboValue(owner, DEFAULT_ASSIGNEE),
                selectedComboValue(priority, DEFAULT_PRIORITY),
                note.value().trim(),
                DONE_STATUS.equals(selectedStatus));
            closeWindow();
            if (onClosed != null)
            {
                onClosed.accept(result);
            }
        }

        @Override
        protected void onCancelBtnClick()
        {
            closeWindow();
            if (onClosed != null)
            {
                onClosed.accept(null);
            }
        }
    }

    private static final class TaskList extends TaskListUI
    {
        private Consumer<String> openRequested;
        private Consumer<String> deleteRequested;
        private TaskDoneChanged doneChanged;

        void setOpenRequested(Consumer<String> value)
        {
            openRequested = value;
        }

        void setDeleteRequested(Consumer<String> value)
        {
            deleteRequested = value;
        }

        void setDoneChanged(TaskDoneChanged value)
        {
            doneChanged = value;
        }

        void setRows(List<TaskItem> rows)
        {
            clearChildren();
            for (int index = 0; index < rows.size(); ++index)
            {
                TaskRow row = new TaskRow(
                    rows.get(index),
                    openRequested,
                    deleteRequested,
                    doneChanged);
                ReusableElement reusable = new ReusableElement("board[" + index + "]");
                reusable.setChild(row);
                row.setMode("expand_width");
                reusable.setFrame(new Rect(
                    0,
                    0,
                    0,
                    GeneratedWindowRuntime.generatedWindowContentSize(row).height()));
                addChild(reusable);
            }
            if (scrollView().frame().width() > 0 && scrollView().frame().height() > 0)
            {
                scrollToTop();
            }
        }
    }

    private static final class TaskRow extends TaskRowUI
    {
        private final TaskItem task;
        private final Consumer<String> openRequested;
        private final Consumer<String> deleteRequested;
        private final TaskDoneChanged doneChanged;

        TaskRow(
            TaskItem task,
            Consumer<String> openRequested,
            Consumer<String> deleteRequested,
            TaskDoneChanged doneChanged)
        {
            this.task = task;
            this.openRequested = openRequested;
            this.deleteRequested = deleteRequested;
            this.doneChanged = doneChanged;
            task_title.setText(task.title());
            meta.setText(task.status() + " / " + task.assignee() + " / " + task.priority());
            done.setChecked(task.done());
        }

        @Override
        protected void onOpenBtnClick()
        {
            if (openRequested != null)
            {
                openRequested.accept(task.id());
            }
        }

        @Override
        protected void onDeleteBtnClick()
        {
            if (deleteRequested != null)
            {
                deleteRequested.accept(task.id());
            }
        }

        @Override
        protected void onDoneChange(String ignoredValue)
        {
            if (doneChanged != null)
            {
                doneChanged.accept(task.id(), done.checked());
            }
        }
    }
}
