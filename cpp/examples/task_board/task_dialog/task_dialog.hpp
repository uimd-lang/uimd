#pragma once

#include "task_dialog_ui.hpp"

#include <functional>
#include <optional>
#include <string>

namespace task_board_example
{

struct TaskDialogResult
{
    std::string id;
    std::string title;
    std::string status;
    std::string assignee;
    std::string priority;
    std::string description;
    bool done = false;
};

using TaskDialogCallback = std::function<void(std::optional<TaskDialogResult>)>;

class TaskDialog : public TaskDialogUI
{
public:
    TaskDialog(std::optional<TaskDialogResult> task = std::nullopt, TaskDialogCallback onClose = {});

    bool submit();
    void cancel();

    [[nodiscard]] const std::optional<TaskDialogResult>& result() const
    {
        return result_;
    }

    [[nodiscard]] bool closed() const
    {
        return closed_;
    }

protected:
    void onAddBtnClick() override;
    void onCancelBtnClick() override;

private:
    void loadTask(const TaskDialogResult& task);
    void close(std::optional<TaskDialogResult> result);
    [[nodiscard]] TaskDialogResult formResult() const;
    [[nodiscard]] std::string validationError() const;

    std::optional<TaskDialogResult> initialTask_;
    std::optional<TaskDialogResult> result_;
    TaskDialogCallback onClose_;
    bool closed_ = false;
};

}  // namespace task_board_example
