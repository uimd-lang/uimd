// Auto-generated UI code for expenses_view - DO NOT EDIT MANUALLY.
#pragma once

#include "ui/generated/GeneratedWindowBase.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"
#include "ui/elements/Button.hpp"
#include "ui/elements/CheckBox.hpp"
#include "ui/elements/ComboBox.hpp"
#include "ui/elements/FrameBufferView.hpp"
#include "ui/elements/InfoLabel.hpp"
#include "ui/elements/Label.hpp"
#include "ui/elements/ListBox.hpp"
#include "ui/elements/MessageTable.hpp"
#include "ui/elements/NumberInput.hpp"
#include "ui/elements/ReusableElement.hpp"
#include "ui/elements/ScrollView.hpp"
#include "ui/elements/TextArea.hpp"
#include "ui/elements/TextInput.hpp"
#include "expenses_list/expenses_list.hpp"


#include <string>
#include <string_view>
#include <vector>

class ExpensesViewUI : public ui::GeneratedWindowBase
{
public:
    ExpensesViewUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* view_title = nullptr;
    ui::TextInput* item_input = nullptr;
    ui::NumberInput* amount_input = nullptr;
    ui::ComboBox* category_input = nullptr;
    ui::Label* currency_label = nullptr;
    ui::Button* add_expense_btn = nullptr;
    ui::ReusableElement* expenses = nullptr;

protected:
    virtual void onItemInputChange(std::string_view value);
    virtual void onItemInputSubmit(std::string_view value);
    virtual void onAmountInputChange(std::string_view value);
    virtual void onAmountInputSubmit(std::string_view value);
    virtual void onCategoryInputChange(std::string_view value);
    virtual void onAddExpenseBtnClick();
    virtual bool shouldClose() const;

};
