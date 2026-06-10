#pragma once

#include "expenses_list_ui.hpp"

#include <functional>
#include <string>
#include <vector>

namespace expense_tracker_example
{

struct ExpenseRowView
{
    std::string id;
    std::string item;
    double amount = 0.0;
    std::string category;
    bool paid = false;
};

class ExpensesList : public ExpensesListUI
{
public:
    using DeleteCallback = std::function<void(std::string_view)>;
    using ChangeCallback = std::function<void(std::string_view, std::string_view, bool)>;

    ExpensesList();

    void setRows(std::vector<ExpenseRowView> rows, std::vector<std::string> categories);
    void setCallbacks(DeleteCallback onDelete, ChangeCallback onChange);
    void refreshRows();

private:
    std::vector<ExpenseRowView> rows_;
    std::vector<std::string> categories_;
    DeleteCallback onDelete_;
    ChangeCallback onChange_;
};

}  // namespace expense_tracker_example
