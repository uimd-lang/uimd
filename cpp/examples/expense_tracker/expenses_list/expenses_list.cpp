#include "expenses_list.hpp"

#include "../expense_row/expense_row_ui.hpp"
#include "ui/elements/ReusableElement.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

namespace expense_tracker_example
{

namespace
{

constexpr int kMinimumRenderHeight = 1;

[[nodiscard]] std::string amountText(double amount)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << amount;
    return out.str();
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

[[nodiscard]] std::string selectedComboValue(const ui::ComboBox& comboBox)
{
    const auto& options = comboBox.options();
    const int index = comboBox.selectedIndex();
    if (index < 0 || index >= static_cast<int>(options.size()))
    {
        return {};
    }
    return options[static_cast<std::size_t>(index)];
}

class ExpenseRowWindow : public ExpenseRowUI
{
public:
    ExpenseRowWindow(ExpenseRowView row,
                     std::vector<std::string> categories,
                     ExpensesList::DeleteCallback onDelete,
                     ExpensesList::ChangeCallback onChange)
        : row_(std::move(row)),
          categories_(std::move(categories)),
          onDelete_(std::move(onDelete)),
          onChange_(std::move(onChange))
    {
        item->setText(row_.item);
        amount->setText(amountText(row_.amount));
        paid->setChecked(row_.paid);
        category->setOptions(categories_);
        selectComboValue(*category, row_.category);
    }

protected:
    void onPaidChange(std::string_view) override
    {
        if (onChange_)
        {
            onChange_(row_.id, selectedComboValue(*category), paid->checked());
        }
    }

    void onCategoryChange(std::string_view value) override
    {
        if (onChange_)
        {
            onChange_(row_.id, value, paid->checked());
        }
    }

    void onDeleteBtnClick() override
    {
        if (onDelete_)
        {
            onDelete_(row_.id);
        }
    }

private:
    ExpenseRowView row_;
    std::vector<std::string> categories_;
    ExpensesList::DeleteCallback onDelete_;
    ExpensesList::ChangeCallback onChange_;
};

}  // namespace

ExpensesList::ExpensesList()
{
    setAutoScroll(false);
}

void ExpensesList::setRows(std::vector<ExpenseRowView> rows, std::vector<std::string> categories)
{
    rows_ = std::move(rows);
    categories_ = std::move(categories);
    refreshRows();
}

void ExpensesList::setCallbacks(DeleteCallback onDelete, ChangeCallback onChange)
{
    onDelete_ = std::move(onDelete);
    onChange_ = std::move(onChange);
    refreshRows();
}

void ExpensesList::refreshRows()
{
    clearChildren();
    for (const ExpenseRowView& row : rows_)
    {
        auto rowWindow = std::make_unique<ExpenseRowWindow>(row, categories_, onDelete_, onChange_);
        const ui::Size naturalSize = ui::generatedWindowContentSize(*rowWindow);
        const ui::Size measuredSize = ui::generatedWindowContentSizeForWidth(*rowWindow, naturalSize.width);
        auto reusable = std::make_unique<ui::ReusableElement>("row", std::move(rowWindow));
        reusable->setFrame(ui::Rect{0, 0, 0, std::max(kMinimumRenderHeight, measuredSize.height)});
        addChild(std::move(reusable));
    }
}

}  // namespace expense_tracker_example
