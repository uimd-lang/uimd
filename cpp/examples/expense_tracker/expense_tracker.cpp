#include "expense_tracker_ui.hpp"

#include "categories_view_ui.hpp"
#include "expenses_view_ui.hpp"
#include "expenses_list/expenses_list.hpp"
#include "settings_view_ui.hpp"

#include "../../dialogs/message_box.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{

struct Expense
{
    std::string id;
    std::string item;
    double amount = 0.0;
    std::string category;
    bool paid = false;
};

struct SeedExpense
{
    std::string_view id;
    std::string_view item;
    double amount = 0.0;
    std::string_view category;
    bool paid = false;
};

constexpr std::array<std::string_view, 5> kDefaultCategories{{"Food", "Transport", "Home", "Health", "Other"}};
constexpr std::string_view kDefaultCurrency = "EUR";
constexpr bool kDefaultShowPaidExpenses = true;
constexpr double kDefaultMonthlyLimit = 500.0;
constexpr std::array kSeedExpenses{
    SeedExpense{"e-1", "Groceries", 42.80, "Food", true},
    SeedExpense{"e-2", "Bus ticket", 2.50, "Transport", true},
    SeedExpense{"e-3", "Light bulbs", 8.90, "Home", false},
    SeedExpense{"e-4", "Pharmacy", 14.20, "Health", true},
    SeedExpense{"e-5", "Notebook", 5.40, "Other", false},
    SeedExpense{"e-6", "Internet bill", 29.99, "Home", true},
    SeedExpense{"e-7", "Team lunch", 18.50, "Food", false},
    SeedExpense{"e-8", "Train pass", 24.00, "Transport", true},
    SeedExpense{"e-9", "Vitamins", 11.75, "Health", false},
    SeedExpense{"e-10", "Desk lamp", 21.30, "Home", false},
    SeedExpense{"e-11", "Cloud storage", 9.99, "Other", true},
    SeedExpense{"e-12", "Replacement keyboard", 44.00, "Other", false},
};

[[nodiscard]] std::string trim(std::string text)
{
    const auto notSpace = [](unsigned char ch)
    {
        return !std::isspace(ch);
    };
    text.erase(text.begin(), std::find_if(text.begin(), text.end(), notSpace));
    text.erase(std::find_if(text.rbegin(), text.rend(), notSpace).base(), text.end());
    return text;
}

[[nodiscard]] std::vector<std::string> defaultCategories()
{
    std::vector<std::string> categories;
    categories.reserve(kDefaultCategories.size());
    for (std::string_view category : kDefaultCategories)
    {
        categories.emplace_back(category);
    }
    return categories;
}

[[nodiscard]] std::vector<Expense> seedExpenses()
{
    std::vector<Expense> expenses;
    expenses.reserve(kSeedExpenses.size());
    for (const SeedExpense& expense : kSeedExpenses)
    {
        expenses.push_back(Expense{
            .id = std::string{expense.id},
            .item = std::string{expense.item},
            .amount = expense.amount,
            .category = std::string{expense.category},
            .paid = expense.paid,
        });
    }
    return expenses;
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

class ExpenseTracker;

class ExpensesView : public ExpensesViewUI
{
public:
    explicit ExpensesView(ExpenseTracker& shell)
        : shell_(shell)
    {
        refresh();
    }

    void refresh();

protected:
    void onAddExpenseBtnClick() override;

private:
    ExpenseTracker& shell_;
};

class CategoriesView : public CategoriesViewUI
{
public:
    explicit CategoriesView(ExpenseTracker& shell)
        : shell_(shell)
    {
        refresh();
    }

    void refresh();

protected:
    void onAddCategoryBtnClick() override;
    void onRemoveCategoryBtnClick() override;

private:
    ExpenseTracker& shell_;
};

class SettingsView : public SettingsViewUI
{
public:
    explicit SettingsView(ExpenseTracker& shell);
    void refresh();

protected:
    void onResetBtnClick() override;
    void onCurrencyChange(std::string_view value) override;
    void onShowPaidChange(std::string_view value) override;
    void onMonthlyLimitChange(std::string_view value) override;
    void onMonthlyLimitSubmit(std::string_view value) override;

private:
    ExpenseTracker& shell_;
};

class ExpenseTracker : public ExpenseTrackerUI
{
public:
    ExpenseTracker()
        : categories_(defaultCategories()),
          expenses_(seedExpenses()),
          currency_(kDefaultCurrency),
          showPaidExpenses_(kDefaultShowPaidExpenses),
          monthlyLimit_(kDefaultMonthlyLimit),
          nextExpenseNumber_(static_cast<int>(expenses_.size()) + 1)
    {
        showExpenses();
    }

    [[nodiscard]] ui::GeneratedWindowRuntimeOptions runtimeOptions() override
    {
        ui::GeneratedWindowRuntimeOptions options = ExpenseTrackerUI::runtimeOptions();
        options.windowStack = &modalStack_;
        options.initialFocusName = "expenses_btn";
        return options;
    }

    void onExpensesBtnClick() override
    {
        showExpenses();
    }

    void onCategoriesBtnClick() override
    {
        showCategories();
    }

    void onSettingsBtnClick() override
    {
        showSettings();
    }

    void onQuitBtnClick() override
    {
        quitRequested_ = true;
    }

    void showExpenses()
    {
        selectNavButton(*expenses_btn);
        main->setView(std::make_unique<ExpensesView>(*this));
    }

    void showCategories()
    {
        selectNavButton(*categories_btn);
        main->setView(std::make_unique<CategoriesView>(*this));
    }

    void showSettings()
    {
        selectNavButton(*settings_btn);
        main->setView(std::make_unique<SettingsView>(*this));
    }

    [[nodiscard]] const std::string& currency() const
    {
        return currency_;
    }

    void setCurrency(std::string_view currency)
    {
        currency_ = currency.empty() ? std::string{kDefaultCurrency} : std::string{currency};
        refreshExpensesView();
    }

    [[nodiscard]] const std::vector<std::string>& categories() const
    {
        return categories_;
    }

    [[nodiscard]] std::vector<expense_tracker_example::ExpenseRowView> expenseRows() const
    {
        std::vector<expense_tracker_example::ExpenseRowView> rows;
        rows.reserve(expenses_.size());
        for (const Expense& expense : expenses_)
        {
            if (!showPaidExpenses_ && expense.paid)
            {
                continue;
            }
            rows.push_back(expense_tracker_example::ExpenseRowView{
                .id = expense.id,
                .item = expense.item,
                .amount = expense.amount,
                .category = expense.category,
                .paid = expense.paid,
            });
        }
        return rows;
    }

    [[nodiscard]] bool showPaidExpenses() const
    {
        return showPaidExpenses_;
    }

    void setShowPaidExpenses(bool value)
    {
        showPaidExpenses_ = value;
        refreshExpensesView();
    }

    [[nodiscard]] double monthlyLimit() const
    {
        return monthlyLimit_;
    }

    void setMonthlyLimit(double value)
    {
        monthlyLimit_ = value;
    }

    void addExpense(std::string item, double amount, std::string category)
    {
        item = trim(std::move(item));
        if (item.empty())
        {
            item = "Untitled expense";
        }
        if (std::find(categories_.begin(), categories_.end(), category) == categories_.end())
        {
            category = categories_.front();
        }
        expenses_.insert(expenses_.begin(), Expense{
            .id = "e-" + std::to_string(nextExpenseNumber_++),
            .item = std::move(item),
            .amount = amount,
            .category = std::move(category),
            .paid = false,
        });
    }

    void updateExpense(std::string_view id, std::string_view category, bool paid)
    {
        for (Expense& expense : expenses_)
        {
            if (expense.id == id)
            {
                expense.category = std::string{category};
                expense.paid = paid;
                return;
            }
        }
    }

    void confirmDeleteExpense(std::string_view id)
    {
        pendingDeleteId_ = std::string{id};
        const Expense* expense = expenseById(pendingDeleteId_);
        const std::string item = expense == nullptr ? "expense" : expense->item;
        deleteDialog_ = std::make_unique<ui::dialogs::MessageBoxYesNo>(
            "Delete Expense",
            "Delete " + item + "?");
        ui::GeneratedWindowFrameOptions frame;
        frame.className = "MessageBoxYesNo";
        frame.initialFocusName = "no_btn";
        frame.onButton = [this](std::string_view name)
        {
            if (name == "yes_btn")
            {
                closeDeleteDialog("yes");
            }
            else if (name == "no_btn")
            {
                closeDeleteDialog("no");
            }
        };
        frame.onKey = [this](std::string_view key)
        {
            if (key == "Escape")
            {
                closeDeleteDialog("no");
                return true;
            }
            return false;
        };
        modalStack_.push(*deleteDialog_, std::move(frame));
    }

    void deleteExpense(std::string_view id, std::string_view result)
    {
        if (result != "yes")
        {
            return;
        }
        expenses_.erase(std::remove_if(expenses_.begin(), expenses_.end(), [&](const Expense& expense)
        {
            return expense.id == id;
        }), expenses_.end());
        refreshExpensesView();
    }

    [[nodiscard]] std::string addCategory(std::string name)
    {
        name = trim(std::move(name));
        if (name.empty())
        {
            return {};
        }
        if (std::find(categories_.begin(), categories_.end(), name) == categories_.end())
        {
            categories_.push_back(name);
        }
        return name;
    }

    void removeCategory(std::string_view name)
    {
        if (name.empty() || categories_.size() <= 1)
        {
            return;
        }
        const auto it = std::find(categories_.begin(), categories_.end(), name);
        if (it == categories_.end())
        {
            return;
        }
        categories_.erase(it);
        const std::string& fallback = categories_.front();
        for (Expense& expense : expenses_)
        {
            if (expense.category == name)
            {
                expense.category = fallback;
            }
        }
    }

    void resetDemoData()
    {
        categories_ = defaultCategories();
        expenses_ = seedExpenses();
        currency_ = kDefaultCurrency;
        showPaidExpenses_ = kDefaultShowPaidExpenses;
        monthlyLimit_ = kDefaultMonthlyLimit;
        nextExpenseNumber_ = static_cast<int>(expenses_.size()) + 1;
        refreshCurrentView();
    }

    [[nodiscard]] ui::GeneratedWindowBase* currentView() const
    {
        return main->currentView();
    }

    bool shouldClose() const override
    {
        return quitRequested_;
    }

private:
    [[nodiscard]] const Expense* expenseById(std::string_view id) const
    {
        for (const Expense& expense : expenses_)
        {
            if (expense.id == id)
            {
                return &expense;
            }
        }
        return nullptr;
    }

    void refreshExpensesView()
    {
        if (auto* view = dynamic_cast<ExpensesView*>(main->currentView()))
        {
            view->refresh();
        }
    }

    void refreshCurrentView()
    {
        if (auto* view = dynamic_cast<ExpensesView*>(main->currentView()))
        {
            view->refresh();
        }
        else if (auto* view = dynamic_cast<CategoriesView*>(main->currentView()))
        {
            view->refresh();
        }
        else if (auto* view = dynamic_cast<SettingsView*>(main->currentView()))
        {
            view->refresh();
        }
    }

    void closeDeleteDialog(std::string result)
    {
        if (deleteDialog_)
        {
            modalStack_.remove(*deleteDialog_);
        }
        const std::string id = pendingDeleteId_;
        deleteDialog_.reset();
        pendingDeleteId_.clear();
        deleteExpense(id, result);
    }

    void selectNavButton(ui::Button& target)
    {
        const ui::Style normalStyle = normalNavButtonStyle(target);
        const ui::Style activeStyle = activeNavButtonStyle(target);
        expenses_btn->setStyle(normalStyle);
        categories_btn->setStyle(normalStyle);
        settings_btn->setStyle(normalStyle);
        target.setStyle(activeStyle);
    }

    [[nodiscard]] ui::Style activeNavButtonStyle(const ui::Button& button) const
    {
        ui::Style style = button.style();
        if (button.focusStyle())
        {
            style.merge(*button.focusStyle());
        }
        return style;
    }

    [[nodiscard]] ui::Style normalNavButtonStyle(const ui::Button& target) const
    {
        if (&target != expenses_btn && dynamic_cast<ExpensesView*>(main->currentView()) == nullptr)
        {
            return expenses_btn->style();
        }
        if (&target != categories_btn && dynamic_cast<CategoriesView*>(main->currentView()) == nullptr)
        {
            return categories_btn->style();
        }
        if (&target != settings_btn && dynamic_cast<SettingsView*>(main->currentView()) == nullptr)
        {
            return settings_btn->style();
        }
        return target.style();
    }

    ui::GeneratedWindowStack modalStack_;
    std::vector<std::string> categories_;
    std::vector<Expense> expenses_;
    std::string currency_;
    bool showPaidExpenses_ = kDefaultShowPaidExpenses;
    double monthlyLimit_ = kDefaultMonthlyLimit;
    std::unique_ptr<ui::dialogs::MessageBoxYesNo> deleteDialog_;
    std::string pendingDeleteId_;
    int nextExpenseNumber_ = 1;
    bool quitRequested_ = false;
};

void ExpensesView::refresh()
{
    currency_label->setText(shell_.currency());
    category_input->setOptions(shell_.categories());
    if (category_input->options().empty())
    {
        return;
    }
    selectComboValue(*category_input, selectedComboValue(*category_input));
    auto* expensesList = dynamic_cast<expense_tracker_example::ExpensesList*>(expenses->child());
    if (expensesList == nullptr)
    {
        return;
    }
    expensesList->setRows(shell_.expenseRows(), shell_.categories());
    expensesList->setCallbacks(
        [this](std::string_view id)
        {
            shell_.confirmDeleteExpense(id);
        },
        [this](std::string_view id, std::string_view category, bool paid)
        {
            shell_.updateExpense(id, category, paid);
        });
}

void ExpensesView::onAddExpenseBtnClick()
{
    shell_.addExpense(item_input->value(), amount_input->value(), selectedComboValue(*category_input));
    item_input->setValue("");
    amount_input->setValue(0);
    refresh();
}

void CategoriesView::refresh()
{
    categories->setOptions(shell_.categories());
    if (!categories->options().empty())
    {
        categories->setSelectedIndex(0);
    }
}

void CategoriesView::onAddCategoryBtnClick()
{
    const std::string added = shell_.addCategory(category_name->value());
    category_name->setValue("");
    refresh();
    if (!added.empty())
    {
        categories->setSelectedValues({added});
    }
}

void CategoriesView::onRemoveCategoryBtnClick()
{
    const auto selected = categories->selectedValues();
    if (!selected.empty())
    {
        shell_.removeCategory(selected.front());
    }
    refresh();
}

SettingsView::SettingsView(ExpenseTracker& shell)
    : shell_(shell)
{
    refresh();
}

void SettingsView::refresh()
{
    selectComboValue(*currency, shell_.currency());
    show_paid->setChecked(shell_.showPaidExpenses());
    monthly_limit->setValue(shell_.monthlyLimit());
}

void SettingsView::onResetBtnClick()
{
    shell_.resetDemoData();
}

void SettingsView::onCurrencyChange(std::string_view value)
{
    shell_.setCurrency(value);
}

void SettingsView::onShowPaidChange(std::string_view)
{
    shell_.setShowPaidExpenses(show_paid->checked());
}

void SettingsView::onMonthlyLimitChange(std::string_view)
{
    shell_.setMonthlyLimit(monthly_limit->value());
}

void SettingsView::onMonthlyLimitSubmit(std::string_view)
{
    shell_.setMonthlyLimit(monthly_limit->value());
}

void runLogicTest()
{
    ExpenseTracker shell;
    assert(shell.currentView() != nullptr);
    assert(dynamic_cast<ExpensesView*>(shell.currentView()) != nullptr);
    shell.addExpense("Coffee", 3.5, "Food");
    shell.deleteExpense("e-6", "yes");
    shell.showCategories();
    assert(dynamic_cast<CategoriesView*>(shell.currentView()) != nullptr);
    shell.showSettings();
    assert(dynamic_cast<SettingsView*>(shell.currentView()) != nullptr);
    shell.setCurrency("USD");
    assert(shell.currency() == "USD");
    shell.setShowPaidExpenses(false);
    for (const auto& row : shell.expenseRows())
    {
        assert(!row.paid);
    }
    shell.setMonthlyLimit(750.0);
    assert(shell.monthlyLimit() == 750.0);
}

}  // namespace

int main(int argc, char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    for (std::string_view arg : args)
    {
        if (arg == "--logic-test" || arg == "--smoke")
        {
            runLogicTest();
            return 0;
        }
    }

    ExpenseTracker shell;
    return ui::runGeneratedWindow(shell, shell.runtimeOptions(), argc, argv);
}
