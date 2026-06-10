#pragma once

#include "task_list_ui.hpp"

#include <functional>
#include <string>
#include <vector>

namespace task_board_example
{

struct TaskRowView
{
    std::string id;
    std::string title;
    std::string status;
    std::string assignee;
    std::string priority;
    bool done = false;
};

class TaskList : public TaskListUI
{
public:
    using OpenCallback = std::function<void(std::string_view)>;
    using DeleteCallback = std::function<void(std::string_view)>;
    using DoneCallback = std::function<void(std::string_view, bool)>;

    TaskList();

    void setRowsProvider(std::function<std::vector<TaskRowView>()> rowsProvider);
    void setCallbacks(OpenCallback onOpen, DeleteCallback onDelete, DoneCallback onDone);
    void refreshRows();

private:
    std::function<std::vector<TaskRowView>()> rowsProvider_;
    OpenCallback onOpen_;
    DeleteCallback onDelete_;
    DoneCallback onDone_;
};

}  // namespace task_board_example
