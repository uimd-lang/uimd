#pragma once

#include "task_filters_ui.hpp"

#include <functional>
#include <string_view>

namespace task_board_example
{

class TaskFilters : public TaskFiltersUI
{
public:
    using Callback = std::function<void()>;

    void setCallbacks(Callback onApply, Callback onReset);

protected:
    void onApplyFiltersBtnClick() override;
    void onResetFiltersBtnClick() override;

    Callback onApply_;
    Callback onReset_;
};

}  // namespace task_board_example
