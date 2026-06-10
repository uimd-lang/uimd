// Auto-generated UI code for categories_view - DO NOT EDIT MANUALLY.
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


#include <string>
#include <string_view>
#include <vector>

class CategoriesViewUI : public ui::GeneratedWindowBase
{
public:
    CategoriesViewUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* view_title = nullptr;
    ui::TextInput* category_name = nullptr;
    ui::Button* add_category_btn = nullptr;
    ui::ListBox* categories = nullptr;
    ui::Button* remove_category_btn = nullptr;

protected:
    virtual void onCategoryNameChange(std::string_view value);
    virtual void onCategoryNameSubmit(std::string_view value);
    virtual void onAddCategoryBtnClick();
    virtual void onCategoriesSelectionChange(const std::vector<std::string>& value);
    virtual void onRemoveCategoryBtnClick();
    virtual bool shouldClose() const;

};
