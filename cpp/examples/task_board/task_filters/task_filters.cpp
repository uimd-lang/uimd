#include "task_filters.hpp"

#include <utility>

namespace task_board_example
{

void TaskFilters::setCallbacks(Callback onApply, Callback onReset)
{
    onApply_ = std::move(onApply);
    onReset_ = std::move(onReset);
}

void TaskFilters::onApplyFiltersBtnClick()
{
    if (onApply_)
    {
        onApply_();
    }
}

void TaskFilters::onResetFiltersBtnClick()
{
    if (onReset_)
    {
        onReset_();
    }
}

}  // namespace task_board_example
