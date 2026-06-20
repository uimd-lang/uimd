// Auto-generated UI code for edit_page - DO NOT EDIT MANUALLY.
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

class EditPageUI : public ui::GeneratedWindowBase
{
public:
    EditPageUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* edit_title = nullptr;
    ui::Button* back_btn = nullptr;
    ui::Button* save_btn = nullptr;
    ui::Label* name_label = nullptr;
    ui::TextInput* name_input = nullptr;
    ui::Label* body_label = nullptr;
    ui::ViewHost* fields = nullptr;
    ui::Label* status = nullptr;

protected:
    virtual void onBackBtnClick();
    virtual void onSaveBtnClick();
    virtual void onNameInputChange(std::string_view value);
    virtual void onNameInputSubmit(std::string_view value);
    virtual bool shouldClose() const;

};
