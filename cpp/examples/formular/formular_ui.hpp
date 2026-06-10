// Auto-generated UI code for formular - DO NOT EDIT MANUALLY.
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

class FormularUI : public ui::GeneratedWindowBase
{
public:
    FormularUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* title_label = nullptr;
    ui::Label* name_label = nullptr;
    ui::TextInput* name_input = nullptr;
    ui::Label* email_label = nullptr;
    ui::TextInput* email_input = nullptr;
    ui::Label* age_label = nullptr;
    ui::NumberInput* age_input = nullptr;
    ui::TextArea* description_input = nullptr;
    ui::Label* country_label = nullptr;
    ui::ComboBox* country_combo = nullptr;
    ui::Label* role_label = nullptr;
    ui::ListBox* role_listbox = nullptr;
    ui::CheckBox* notify_check = nullptr;
    ui::CheckBox* terms_check = nullptr;
    ui::Button* save_btn = nullptr;
    ui::Button* cancel_btn = nullptr;

protected:
    virtual void onNameInputChange(std::string_view value);
    virtual void onNameInputSubmit(std::string_view value);
    virtual void onEmailInputChange(std::string_view value);
    virtual void onEmailInputSubmit(std::string_view value);
    virtual void onAgeInputChange(std::string_view value);
    virtual void onAgeInputSubmit(std::string_view value);
    virtual void onDescriptionInputChange(std::string_view value);
    virtual void onDescriptionInputSubmit(std::string_view value);
    virtual void onCountryComboChange(std::string_view value);
    virtual void onRoleListboxSelectionChange(const std::vector<std::string>& value);
    virtual void onNotifyCheckChange(std::string_view value);
    virtual void onTermsCheckChange(std::string_view value);
    virtual void onSaveBtnClick();
    virtual void onCancelBtnClick();
    virtual bool shouldClose() const;

};
