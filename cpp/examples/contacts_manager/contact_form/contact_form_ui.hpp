// Auto-generated UI code for contact_form - DO NOT EDIT MANUALLY.
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

class ContactFormUI : public ui::GeneratedWindowBase
{
public:
    ContactFormUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* header = nullptr;
    ui::Label* first_label = nullptr;
    ui::TextInput* first_name = nullptr;
    ui::Label* last_label = nullptr;
    ui::TextInput* last_name = nullptr;
    ui::Label* email_label = nullptr;
    ui::TextInput* email = nullptr;
    ui::Label* phone_label = nullptr;
    ui::TextInput* phone = nullptr;
    ui::Label* company_label = nullptr;
    ui::TextInput* company = nullptr;
    ui::Label* city_label = nullptr;
    ui::TextInput* city = nullptr;
    ui::Label* category_label = nullptr;
    ui::ComboBox* category = nullptr;
    ui::CheckBox* active = nullptr;
    ui::Label* notes_label = nullptr;
    ui::TextArea* notes = nullptr;
    ui::Button* save_btn = nullptr;
    ui::Button* cancel_btn = nullptr;
    ui::Label* message = nullptr;

protected:
    virtual void onFirstNameChange(std::string_view value);
    virtual void onFirstNameSubmit(std::string_view value);
    virtual void onLastNameChange(std::string_view value);
    virtual void onLastNameSubmit(std::string_view value);
    virtual void onEmailChange(std::string_view value);
    virtual void onEmailSubmit(std::string_view value);
    virtual void onPhoneChange(std::string_view value);
    virtual void onPhoneSubmit(std::string_view value);
    virtual void onCompanyChange(std::string_view value);
    virtual void onCompanySubmit(std::string_view value);
    virtual void onCityChange(std::string_view value);
    virtual void onCitySubmit(std::string_view value);
    virtual void onCategoryChange(std::string_view value);
    virtual void onActiveChange(std::string_view value);
    virtual void onNotesChange(std::string_view value);
    virtual void onNotesSubmit(std::string_view value);
    virtual void onSaveBtnClick();
    virtual void onCancelBtnClick();
    virtual bool shouldClose() const;

};
