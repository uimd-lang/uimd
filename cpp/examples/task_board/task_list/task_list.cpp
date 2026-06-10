#include "task_list.hpp"

#include "../task_row/task_row_ui.hpp"
#include "ui/elements/ReusableElement.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

namespace task_board_example
{

namespace
{

constexpr int kMinimumRenderHeight = 1;

[[nodiscard]] std::string taskMeta(const TaskRowView& row)
{
    return row.status + " / " + row.assignee + " / " + row.priority;
}

class TaskRowWindow : public TaskRowUI
{
public:
    TaskRowWindow(TaskRowView row,
                  TaskList::OpenCallback onOpen,
                  TaskList::DeleteCallback onDelete,
                  TaskList::DoneCallback onDone)
        : row_(std::move(row)),
          onOpen_(std::move(onOpen)),
          onDelete_(std::move(onDelete)),
          onDone_(std::move(onDone))
    {
        task_title->setText(row_.title);
        meta->setText(taskMeta(row_));
        done->setChecked(row_.done);
    }

protected:
    void onOpenBtnClick() override
    {
        if (onOpen_)
        {
            onOpen_(row_.id);
        }
    }

    void onDeleteBtnClick() override
    {
        if (onDelete_)
        {
            onDelete_(row_.id);
        }
    }

    void onDoneChange(std::string_view) override
    {
        if (onDone_)
        {
            onDone_(row_.id, done->checked());
        }
    }

private:
    TaskRowView row_;
    TaskList::OpenCallback onOpen_;
    TaskList::DeleteCallback onDelete_;
    TaskList::DoneCallback onDone_;
};

}  // namespace

TaskList::TaskList()
{}

void TaskList::setRowsProvider(std::function<std::vector<TaskRowView>()> rowsProvider)
{
    rowsProvider_ = std::move(rowsProvider);
    refreshRows();
}

void TaskList::setCallbacks(OpenCallback onOpen, DeleteCallback onDelete, DoneCallback onDone)
{
    onOpen_ = std::move(onOpen);
    onDelete_ = std::move(onDelete);
    onDone_ = std::move(onDone);
    refreshRows();
}

void TaskList::refreshRows()
{
    clearChildren();
    if (!rowsProvider_)
    {
        return;
    }

    const std::vector<TaskRowView> rows = rowsProvider_();
    for (const TaskRowView& row : rows)
    {
        auto rowWindow = std::make_unique<TaskRowWindow>(row, onOpen_, onDelete_, onDone_);
        const ui::Size naturalSize = ui::generatedWindowContentSize(*rowWindow);
        auto reusable = std::make_unique<ui::ReusableElement>("row", std::move(rowWindow));
        reusable->setFrame(ui::Rect{0, 0, 0, std::max(kMinimumRenderHeight, naturalSize.height)});
        addChild(std::move(reusable));
    }
    const ui::Rect currentFrame = frame();
    if (currentFrame.width > 0 && currentFrame.height > 0)
    {
        (void)scrollToTop(ui::Size{currentFrame.width, currentFrame.height});
    }
}

}  // namespace task_board_example
