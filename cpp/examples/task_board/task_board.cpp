#include "task_board_ui.hpp"

#include "message_box.hpp"
#include "task_dialog/task_dialog.hpp"
#include "task_list/task_list.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cctype>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace
{

constexpr std::string_view kAnyFilter = "Any";
constexpr std::string_view kDefaultAssignee = "Ava";
constexpr std::string_view kDefaultPriority = "Med";
constexpr std::string_view kDefaultStatus = "Todo";
constexpr std::string_view kDoneStatus = "Done";
constexpr std::string_view kTaskIdPrefix = "t-";
constexpr std::string_view kCanceledStatusText = "Action canceled.";
constexpr int kInitialTaskNumberBase = 100;
constexpr int kSmokeRenderWidth = 112;
constexpr int kSmokeRenderHeight = 38;
constexpr std::size_t kSeedTaskCount = 14;

struct Task
{
    std::string id;
    std::string title;
    std::string status;
    std::string assignee;
    std::string priority;
    std::string description;
    bool done = false;
};

struct SeedTask
{
    std::string_view id;
    std::string_view title;
    std::string_view status;
    std::string_view assignee;
    std::string_view priority;
    std::string_view description;
    bool done = false;
};

constexpr std::array<SeedTask, kSeedTaskCount> kSeedTasks{{
    {"t-101", "Review invoice export", "Doing", "Ava", "High", "Check exported columns and totals before finance signs off.", false},
    {"t-102", "Rewrite signup emails", "Blocked", "Bruno", "Med", "Waiting for brand review on the onboarding sequence.", false},
    {"t-103", "Improve search ranking", "Doing", "Chen", "High", "Tune ranking for exact title matches and recent activity.", false},
    {"t-104", "Draft data deletion policy", "Todo", "Dana", "Urgent", "Prepare policy draft for privacy review.", false},
    {"t-105", "Update support templates", "Todo", "Ava", "Med", "Refresh billing and account recovery snippets.", false},
    {"t-106", "Fix API contract tests", "Blocked", "Chen", "High", "Coordinate schema change with partner sandbox.", false},
    {"t-107", "Clean left navigation", "Todo", "Bruno", "Low", "Align labels and collapse unused spacing in admin views.", false},
    {"t-108", "Finalize launch checklist", "Done", "Dana", "Med", "Checklist completed and reviewed by release owner.", true},
    {"t-109", "Define calendar sharing", "Doing", "Bruno", "Med", "Document private, team, and organization visibility.", false},
    {"t-110", "Clean knowledge editor", "Todo", "Chen", "Low", "Remove stale toolbar actions and simplify publish flow.", false},
    {"t-111", "Publish usage report", "Doing", "Dana", "Med", "Send account summary to customer success leads.", false},
    {"t-112", "Retry failed imports", "Blocked", "Ava", "Urgent", "Needs import worker patch before rerun.", false},
    {"t-113", "Tune digest settings", "Todo", "Bruno", "Med", "Review default digest cadence for new workspaces.", false},
    {"t-114", "Verify account password reset cleanup", "Done", "Chen", "Low", "Old reset token cleanup verified in staging.", true},
}};

[[nodiscard]] std::vector<Task> seedTasks()
{
    std::vector<Task> tasks;
    tasks.reserve(kSeedTasks.size());
    for (const SeedTask& task : kSeedTasks)
    {
        tasks.push_back(Task{
            .id = std::string{task.id},
            .title = std::string{task.title},
            .status = std::string{task.status},
            .assignee = std::string{task.assignee},
            .priority = std::string{task.priority},
            .description = std::string{task.description},
            .done = task.done,
        });
    }
    return tasks;
}

[[nodiscard]] std::string lower(std::string_view text)
{
    std::string result{text};
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch)
    {
        return static_cast<char>(std::tolower(ch));
    });
    return result;
}

[[nodiscard]] bool containsText(std::string_view haystack, std::string_view needle)
{
    return lower(haystack).find(lower(needle)) != std::string::npos;
}

[[nodiscard]] std::string jsonEscape(std::string_view text)
{
    std::string result;
    for (char ch : text)
    {
        if (ch == '\\' || ch == '"')
        {
            result.push_back('\\');
            result.push_back(ch);
        }
        else if (ch == '\n')
        {
            result += "\\n";
        }
        else
        {
            result.push_back(ch);
        }
    }
    return result;
}

[[nodiscard]] std::optional<std::string> parseJsonStringAt(std::string_view text, std::size_t& index)
{
    if (index >= text.size() || text[index] != '"')
    {
        return std::nullopt;
    }
    ++index;
    std::string result;
    while (index < text.size())
    {
        const char ch = text[index++];
        if (ch == '"')
        {
            return result;
        }
        if (ch == '\\' && index < text.size())
        {
            const char escaped = text[index++];
            if (escaped == 'n')
            {
                result.push_back('\n');
            }
            else
            {
                result.push_back(escaped);
            }
            continue;
        }
        result.push_back(ch);
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<std::size_t> jsonValueStart(std::string_view objectJson, std::string_view field)
{
    std::size_t index = 0;
    while (index < objectJson.size())
    {
        if (objectJson[index] != '"')
        {
            ++index;
            continue;
        }
        std::size_t keyIndex = index;
        std::optional<std::string> key = parseJsonStringAt(objectJson, keyIndex);
        if (!key.has_value())
        {
            return std::nullopt;
        }
        while (keyIndex < objectJson.size() && std::isspace(static_cast<unsigned char>(objectJson[keyIndex])))
        {
            ++keyIndex;
        }
        if (keyIndex >= objectJson.size() || objectJson[keyIndex] != ':')
        {
            index = keyIndex;
            continue;
        }
        ++keyIndex;
        while (keyIndex < objectJson.size() && std::isspace(static_cast<unsigned char>(objectJson[keyIndex])))
        {
            ++keyIndex;
        }
        if (*key == field)
        {
            return keyIndex;
        }
        index = keyIndex;
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<std::string> jsonStringField(std::string_view objectJson, std::string_view field)
{
    std::optional<std::size_t> index = jsonValueStart(objectJson, field);
    if (!index.has_value())
    {
        return std::nullopt;
    }
    return parseJsonStringAt(objectJson, *index);
}

[[nodiscard]] std::optional<bool> jsonBoolField(std::string_view objectJson, std::string_view field)
{
    std::optional<std::size_t> index = jsonValueStart(objectJson, field);
    if (!index.has_value())
    {
        return std::nullopt;
    }
    const std::string_view value = objectJson.substr(*index);
    if (value.substr(0, std::string_view{"true"}.size()) == "true")
    {
        return true;
    }
    if (value.substr(0, std::string_view{"false"}.size()) == "false")
    {
        return false;
    }
    return std::nullopt;
}

[[nodiscard]] std::string taskJson(const Task& task)
{
    return std::string{"{\"id\":\""} + jsonEscape(task.id) +
           "\",\"title\":\"" + jsonEscape(task.title) +
           "\",\"status\":\"" + jsonEscape(task.status) +
           "\",\"assignee\":\"" + jsonEscape(task.assignee) +
           "\",\"priority\":\"" + jsonEscape(task.priority) +
           "\",\"description\":\"" + jsonEscape(task.description) +
           "\",\"done\":" + (task.done ? "true" : "false") + "}";
}

void selectComboValue(ui::ComboBox& comboBox, std::string_view value)
{
    const auto& options = comboBox.options();
    for (std::size_t index = 0; index < options.size(); ++index)
    {
        if (options[index] == value)
        {
            comboBox.setSelectedIndex(static_cast<int>(index));
            return;
        }
    }
    comboBox.setSelectedIndex(0);
}

[[nodiscard]] std::string selectedComboValue(const ui::ComboBox& comboBox, std::string_view fallback)
{
    const auto& options = comboBox.options();
    const int index = comboBox.selectedIndex();
    if (index < 0 || index >= static_cast<int>(options.size()))
    {
        return std::string{fallback};
    }
    return options[static_cast<std::size_t>(index)];
}

[[nodiscard]] task_board_example::TaskDialogResult toDialogResult(const Task& task)
{
    return task_board_example::TaskDialogResult{
        .id = task.id,
        .title = task.title,
        .status = task.status,
        .assignee = task.assignee,
        .priority = task.priority,
        .description = task.description,
        .done = task.done,
    };
}

[[nodiscard]] int taskNumber(std::string_view id)
{
    if (id.substr(0, kTaskIdPrefix.size()) != kTaskIdPrefix)
    {
        return kInitialTaskNumberBase;
    }
    int number = kInitialTaskNumberBase;
    const std::string_view suffix = id.substr(kTaskIdPrefix.size());
    (void)std::from_chars(suffix.data(), suffix.data() + suffix.size(), number);
    return number;
}

[[nodiscard]] std::string renderedWindowText(ui::GeneratedWindowBase& window, ui::Size size)
{
    std::string text;
    for (const std::string& row : ui::renderedText(ui::renderGeneratedWindowContent(window, size)))
    {
        text += row;
        text += '\n';
    }
    return text;
}

class TaskBoardApp : public TaskBoardUI
{
public:
    void open()
    {
        tasks_ = seedTasks();
        taskList().setRowsProvider([this]
        {
            return visibleRows();
        });
        taskList().setCallbacks(
            [this](std::string_view id)
            {
                openTaskById(id);
            },
            [this](std::string_view id)
            {
                confirmDeleteTask(id);
            },
            [this](std::string_view id, bool done)
            {
                setTaskDone(id, done);
            });
        refreshBoard();
    }

    [[nodiscard]] ui::GeneratedWindowStack& modalStack()
    {
        return modalStack_;
    }

    [[nodiscard]] bool hasModalOverlay() const
    {
        return !modalStack_.empty();
    }

    [[nodiscard]] std::size_t taskCount() const
    {
        return tasks_.size();
    }

    [[nodiscard]] std::size_t visibleCount() const
    {
        return matchingTasks().size();
    }

    void button(std::string_view name)
    {
        if (name == "reset_filters_btn")
        {
            resetFilters();
            refreshBoard();
        }
        else if (name == "new_task_btn")
        {
            openTaskDialog(std::nullopt);
        }
        else if (name == "quit_btn")
        {
            quitRequested_ = true;
        }
    }

    void addTaskForTest(const task_board_example::TaskDialogResult& result)
    {
        saveTask(result);
    }

    void refreshForTest()
    {
        refreshBoard();
    }

    [[nodiscard]] ui::GeneratedWindowRuntimeOptions runtimeOptions() override
    {
        ui::GeneratedWindowRuntimeOptions options = TaskBoardUI::runtimeOptions();
        options.windowStack = &modalStack_;
        return options;
    }

protected:
    void onSearchChange(std::string_view) override
    {
        refreshBoard();
    }

    void onSearchSubmit(std::string_view) override
    {
        refreshBoard();
    }

    void onStatusFilterChange(std::string_view) override
    {
        refreshBoard();
    }

    void onOwnerFilterChange(std::string_view) override
    {
        refreshBoard();
    }

    void onResetFiltersBtnClick() override
    {
        resetFilters();
        refreshBoard();
    }

    void onNewTaskBtnClick() override
    {
        openTaskDialog(std::nullopt);
    }

    void onQuitBtnClick() override
    {
        quitRequested_ = true;
    }

    std::string queryTasks(std::string_view inputJson) override
    {
        std::string result = "{\"tasks\":[";
        bool first = true;
        for (const Task* task : matchingTasksForTool(inputJson))
        {
            if (!first)
            {
                result.push_back(',');
            }
            first = false;
            result += taskJson(*task);
        }
        result += "]}";
        return result;
    }

    std::string getTask(std::string_view inputJson) override
    {
        const std::optional<std::string> taskId = jsonStringField(inputJson, "id");
        Task* task = taskById(taskId.value_or(""));
        return std::string{"{\"task\":"} + (task == nullptr ? "null" : taskJson(*task)) + "}";
    }

    std::string updateTask(std::string_view inputJson) override
    {
        const std::optional<std::string> taskId = jsonStringField(inputJson, "id");
        Task* task = taskById(taskId.value_or(""));
        if (task == nullptr)
        {
            return "{\"task\":null}";
        }

        if (std::optional<std::string> title = jsonStringField(inputJson, "title"))
        {
            task->title = *title;
        }
        if (std::optional<std::string> status = jsonStringField(inputJson, "status"))
        {
            task->status = status->empty() ? std::string{kDefaultStatus} : *status;
        }
        if (std::optional<std::string> assignee = jsonStringField(inputJson, "assignee"))
        {
            task->assignee = assignee->empty() ? std::string{kDefaultAssignee} : *assignee;
        }
        if (std::optional<std::string> priority = jsonStringField(inputJson, "priority"))
        {
            task->priority = priority->empty() ? std::string{kDefaultPriority} : *priority;
        }
        if (std::optional<std::string> description = jsonStringField(inputJson, "description"))
        {
            task->description = *description;
        }
        if (std::optional<bool> done = jsonBoolField(inputJson, "done"))
        {
            task->done = *done;
        }
        syncDoneStatus(*task);
        refreshBoard();
        return std::string{"{\"task\":"} + taskJson(*task) + "}";
    }

    std::string deleteTask(std::string_view inputJson) override
    {
        const std::optional<std::string> taskId = jsonStringField(inputJson, "id");
        const auto before = tasks_.size();
        tasks_.erase(
            std::remove_if(tasks_.begin(), tasks_.end(), [&](const Task& task)
            {
                return task.id == taskId.value_or("");
            }),
            tasks_.end());
        const bool deleted = tasks_.size() != before;
        if (deleted)
        {
            refreshBoard();
            status->setText("Task deleted.");
        }
        return std::string{"{\"deleted\":"} + (deleted ? "true" : "false") + "}";
    }

    bool shouldClose() const override
    {
        return quitRequested_;
    }

private:
    [[nodiscard]] task_board_example::TaskList& taskList()
    {
        auto* list = dynamic_cast<task_board_example::TaskList*>(board->child());
        if (list == nullptr)
        {
            throw std::runtime_error("Task list child is not initialized.");
        }
        return *list;
    }

    [[nodiscard]] const task_board_example::TaskList& taskList() const
    {
        auto* list = dynamic_cast<const task_board_example::TaskList*>(board->child());
        if (list == nullptr)
        {
            throw std::runtime_error("Task list child is not initialized.");
        }
        return *list;
    }

    [[nodiscard]] std::vector<const Task*> matchingTasks() const
    {
        const std::string query = search->value();
        const std::string statusFilter = selectedComboValue(*status_filter, kAnyFilter);
        const std::string assigneeFilter = selectedComboValue(*owner_filter, kAnyFilter);

        std::vector<const Task*> records;
        for (const Task& task : tasks_)
        {
            const std::string haystack = task.title + " " + task.description + " " + task.assignee + " " + task.priority;
            if (!query.empty() && !containsText(haystack, query))
            {
                continue;
            }
            if (statusFilter != kAnyFilter && task.status != statusFilter)
            {
                continue;
            }
            if (assigneeFilter != kAnyFilter && task.assignee != assigneeFilter)
            {
                continue;
            }
            records.push_back(&task);
        }
        return records;
    }

    [[nodiscard]] std::vector<const Task*> matchingTasksForTool(std::string_view inputJson) const
    {
        const std::string query = jsonStringField(inputJson, "title")
            .value_or(jsonStringField(inputJson, "name").value_or(""));
        const std::string statusFilter = jsonStringField(inputJson, "status").value_or(std::string{kAnyFilter});
        const std::string assigneeFilter = jsonStringField(inputJson, "assignee")
            .value_or(jsonStringField(inputJson, "owner").value_or(std::string{kAnyFilter}));
        const std::string priorityFilter = jsonStringField(inputJson, "priority").value_or(std::string{kAnyFilter});
        const std::optional<bool> doneFilter = jsonBoolField(inputJson, "done");

        std::vector<const Task*> records;
        for (const Task& task : tasks_)
        {
            const std::string haystack = task.title + " " + task.description + " " + task.assignee + " " + task.priority;
            if (!query.empty() && !containsText(haystack, query))
            {
                continue;
            }
            if (statusFilter != kAnyFilter && task.status != statusFilter)
            {
                continue;
            }
            if (assigneeFilter != kAnyFilter && task.assignee != assigneeFilter)
            {
                continue;
            }
            if (priorityFilter != kAnyFilter && task.priority != priorityFilter)
            {
                continue;
            }
            if (doneFilter.has_value() && task.done != *doneFilter)
            {
                continue;
            }
            records.push_back(&task);
        }
        return records;
    }

    [[nodiscard]] std::vector<task_board_example::TaskRowView> visibleRows() const
    {
        std::vector<task_board_example::TaskRowView> rows;
        for (const Task* task : matchingTasks())
        {
            rows.push_back(task_board_example::TaskRowView{
                .id = task->id,
                .title = task->title,
                .status = task->status,
                .assignee = task->assignee,
                .priority = task->priority,
                .done = task->done,
            });
        }
        return rows;
    }

    void refreshBoard()
    {
        taskList().refreshRows();
        status->setText(std::to_string(visibleCount()) + " visible / " + std::to_string(tasks_.size()) + " total");
    }

    void resetFilters()
    {
        search->setValue("");
        search->setCursor(0);
        selectComboValue(*status_filter, kAnyFilter);
        selectComboValue(*owner_filter, kAnyFilter);
    }

    void openTaskDialog(std::optional<Task> task)
    {
        taskDialog_ = std::make_unique<task_board_example::TaskDialog>(
            task.has_value() ? std::optional<task_board_example::TaskDialogResult>{toDialogResult(*task)} : std::nullopt);

        ui::GeneratedWindowFrameOptions frame;
        frame.onButton = [this](std::string_view name)
        {
            if (!taskDialog_)
            {
                return;
            }
            if (name == "add_btn")
            {
                if (taskDialog_->submit())
                {
                    saveTask(taskDialog_->result());
                    closeTaskDialog();
                }
            }
            else
            {
                status->setText(std::string{kCanceledStatusText});
                taskDialog_->cancel();
                closeTaskDialog();
            }
        };
        modalStack_.push(*taskDialog_, std::move(frame));
    }

    void openTaskById(std::string_view id)
    {
        if (Task* task = taskById(id))
        {
            openTaskDialog(*task);
        }
    }

    void confirmDeleteTask(std::string_view id)
    {
        Task* task = taskById(id);
        if (task == nullptr)
        {
            return;
        }

        pendingDeleteTaskId_ = task->id;
        deleteDialog_ = std::make_unique<ui::dialogs::MessageBoxYesNo>(
            "Delete Task",
            "Delete " + task->title + "?");

        ui::GeneratedWindowFrameOptions frame;
        frame.className = "MessageBoxYesNo";
        frame.initialFocusName = "no_btn";
        frame.onButton = [this](std::string_view name)
        {
            if (name == "yes_btn")
            {
                deleteConfirmed(pendingDeleteTaskId_, true);
            }
            else if (name == "no_btn")
            {
                deleteConfirmed(pendingDeleteTaskId_, false);
            }
        };
        modalStack_.push(*deleteDialog_, std::move(frame));
    }

    void deleteConfirmed(std::string_view id, bool confirmed)
    {
        const std::string taskId{id};
        closeDeleteDialog();
        if (!confirmed)
        {
            status->setText(std::string{kCanceledStatusText});
            return;
        }

        const auto before = tasks_.size();
        tasks_.erase(
            std::remove_if(tasks_.begin(), tasks_.end(), [&](const Task& task)
            {
                return task.id == taskId;
            }),
            tasks_.end());
        if (tasks_.size() != before)
        {
            refreshBoard();
            status->setText("Task deleted.");
        }
    }

    void closeDeleteDialog()
    {
        if (!deleteDialog_)
        {
            return;
        }
        modalStack_.remove(*deleteDialog_);
        deleteDialog_.reset();
        pendingDeleteTaskId_.clear();
    }

    void setTaskDone(std::string_view id, bool done)
    {
        Task* task = taskById(id);
        if (task == nullptr)
        {
            return;
        }
        task->done = done;
        syncDoneStatus(*task);
        status->setText("Updated " + task->title);
    }

    void closeTaskDialog()
    {
        if (!taskDialog_)
        {
            return;
        }
        modalStack_.remove(*taskDialog_);
        taskDialog_.reset();
    }

    void saveTask(const std::optional<task_board_example::TaskDialogResult>& result)
    {
        if (!result.has_value())
        {
            status->setText(std::string{kCanceledStatusText});
            return;
        }
        saveTask(*result);
    }

    void saveTask(const task_board_example::TaskDialogResult& result)
    {
        Task* task = result.id.empty() ? nullptr : taskById(result.id);
        if (task == nullptr)
        {
            tasks_.insert(tasks_.begin(), Task{
                .id = nextTaskId(),
                .title = {},
                .status = std::string{kDefaultStatus},
                .assignee = std::string{kDefaultAssignee},
                .priority = std::string{kDefaultPriority},
                .description = {},
                .done = false,
            });
            task = &tasks_.front();
            resetFilters();
        }

        task->title = result.title;
        task->status = result.status.empty() ? std::string{kDefaultStatus} : result.status;
        task->assignee = result.assignee.empty() ? std::string{kDefaultAssignee} : result.assignee;
        task->priority = result.priority.empty() ? std::string{kDefaultPriority} : result.priority;
        task->description = result.description;
        task->done = result.done;
        syncDoneStatus(*task);
        refreshBoard();
        status->setText("Task saved.");
    }

    [[nodiscard]] Task* taskById(std::string_view id)
    {
        for (Task& task : tasks_)
        {
            if (task.id == id)
            {
                return &task;
            }
        }
        return nullptr;
    }

    void syncDoneStatus(Task& task)
    {
        if (task.done)
        {
            task.status = std::string{kDoneStatus};
        }
        else if (task.status == kDoneStatus)
        {
            task.status = std::string{kDefaultStatus};
        }
    }

    [[nodiscard]] std::string nextTaskId() const
    {
        int next = kInitialTaskNumberBase + 1;
        for (const Task& task : tasks_)
        {
            next = std::max(next, taskNumber(task.id) + 1);
        }
        return std::string{kTaskIdPrefix} + std::to_string(next);
    }

    std::vector<Task> tasks_;
    ui::GeneratedWindowStack modalStack_;
    std::unique_ptr<task_board_example::TaskDialog> taskDialog_;
    std::unique_ptr<ui::dialogs::MessageBoxYesNo> deleteDialog_;
    std::string pendingDeleteTaskId_;
    bool quitRequested_ = false;
};

void smoke()
{
    TaskBoardUI window;
    assert(static_cast<const ui::Window&>(window).title() == "Task Board");
    assert(window.title != nullptr);
    assert(window.quit_btn != nullptr);
    assert(window.search != nullptr);
    assert(window.status_filter != nullptr);
    assert(window.owner_filter != nullptr);
    assert(window.reset_filters_btn != nullptr);
    assert(window.new_task_btn != nullptr);
    assert(window.board != nullptr);
    assert(window.status != nullptr);

    ui::TerminalBuffer buffer{kSmokeRenderWidth, kSmokeRenderHeight};
    ui::renderGeneratedWindow(window, buffer);
    assert(window.board->frame().width > window.search->frame().width);
}

void logicTest()
{
    TaskBoardApp app;
    app.open();

    assert(app.taskCount() == kSeedTaskCount);
    assert(app.visibleCount() == kSeedTaskCount);
    std::string boardText = renderedWindowText(app, ui::Size{kSmokeRenderWidth, kSmokeRenderHeight});
    assert(boardText.find("Review invoice export") != std::string::npos);
    assert(boardText.find("Done") != std::string::npos);
    assert(boardText.find("Open") != std::string::npos);
    assert(boardText.find("Delete") != std::string::npos);

    app.search->setValue("calendar");
    app.refreshForTest();
    assert(app.visibleCount() == 1);
    boardText = renderedWindowText(app, ui::Size{kSmokeRenderWidth, kSmokeRenderHeight});
    assert(boardText.find("Define calendar sharing") != std::string::npos);

    app.button("reset_filters_btn");
    assert(app.visibleCount() == kSeedTaskCount);

    app.addTaskForTest(task_board_example::TaskDialogResult{
        .title = "Write C++ task board",
        .status = std::string{kDefaultStatus},
        .assignee = std::string{kDefaultAssignee},
        .priority = std::string{kDefaultPriority},
        .description = "Port task board example.",
        .done = false,
    });
    assert(app.taskCount() == kSeedTaskCount + 1);
    assert(app.status->text() == "Task saved.");

    app.button("new_task_btn");
    assert(app.hasModalOverlay());
    app.button("quit_btn");
    const ui::GeneratedWindowRuntimeOptions options = app.runtimeOptions();
    assert(options.shouldClose && options.shouldClose());
}

}  // namespace

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view(argv[1]) == "--smoke")
    {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--logic-test")
    {
        logicTest();
        return 0;
    }

    TaskBoardApp app;
    app.open();
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}
