#include "task_dialog.hpp"

#include "ui/elements/ComboBox.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string_view>
#include <utility>

namespace task_board_example
{

namespace
{

constexpr std::string_view kDefaultStatus = "Todo";
constexpr std::string_view kDefaultAssignee = "Ava";
constexpr std::string_view kDefaultPriority = "Med";
constexpr std::string_view kDoneStatus = "Done";
constexpr std::string_view kNewTaskTitle = "New task";
constexpr std::string_view kEditTaskTitle = "Edit task";
constexpr std::string_view kTitleRequiredMessage = "Title is required.";

[[nodiscard]] std::string trim(std::string_view text)
{
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin])))
    {
        ++begin;
    }

    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])))
    {
        --end;
    }

    return std::string{text.substr(begin, end - begin)};
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

}  // namespace

TaskDialog::TaskDialog(std::optional<TaskDialogResult> task, TaskDialogCallback onClose)
    : initialTask_(std::move(task)),
      onClose_(std::move(onClose))
{
    if (initialTask_.has_value())
    {
        loadTask(*initialTask_);
    }
    else
    {
        header->setText(std::string{kNewTaskTitle});
        selectComboValue(*project_status, kDefaultStatus);
        selectComboValue(*owner, kDefaultAssignee);
        selectComboValue(*priority, kDefaultPriority);
    }
}

bool TaskDialog::submit()
{
    const std::string error = validationError();
    if (!error.empty())
    {
        message->setText(error);
        return false;
    }

    close(formResult());
    return true;
}

void TaskDialog::cancel()
{
    close(std::nullopt);
}

void TaskDialog::onAddBtnClick()
{
    (void)submit();
}

void TaskDialog::onCancelBtnClick()
{
    cancel();
}

void TaskDialog::loadTask(const TaskDialogResult& task)
{
    header->setText(std::string{kEditTaskTitle});
    name->setValue(task.title);
    selectComboValue(*project_status, task.status.empty() ? kDefaultStatus : std::string_view(task.status));
    selectComboValue(*owner, task.assignee.empty() ? kDefaultAssignee : std::string_view(task.assignee));
    selectComboValue(*priority, task.priority.empty() ? kDefaultPriority : std::string_view(task.priority));
    note->setValue(task.description);
}

void TaskDialog::close(std::optional<TaskDialogResult> result)
{
    result_ = std::move(result);
    closed_ = true;
    if (onClose_)
    {
        onClose_(result_);
    }
}

TaskDialogResult TaskDialog::formResult() const
{
    const std::string statusValue = selectedComboValue(*project_status, kDefaultStatus);
    return TaskDialogResult{
        .id = initialTask_.has_value() ? initialTask_->id : std::string{},
        .title = trim(name->value()),
        .status = statusValue,
        .assignee = selectedComboValue(*owner, kDefaultAssignee),
        .priority = selectedComboValue(*priority, kDefaultPriority),
        .description = trim(note->value()),
        .done = statusValue == kDoneStatus,
    };
}

std::string TaskDialog::validationError() const
{
    if (trim(name->value()).empty())
    {
        return std::string{kTitleRequiredMessage};
    }
    return {};
}

}  // namespace task_board_example
