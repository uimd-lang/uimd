// Auto-generated UI code for settings - DO NOT EDIT MANUALLY.
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

class SettingsUI : public ui::GeneratedWindowBase
{
public:
    SettingsUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* header_title = nullptr;
    ui::CheckBox* auto_scroll = nullptr;
    ui::CheckBox* show_timestamps = nullptr;
    ui::Label* default_type_label = nullptr;
    ui::ComboBox* default_type = nullptr;
    ui::Button* save_button = nullptr;
    ui::Button* cancel_button = nullptr;

protected:
    virtual void onAutoScrollChange(std::string_view value);
    virtual void onShowTimestampsChange(std::string_view value);
    virtual void onDefaultTypeChange(std::string_view value);
    virtual void onSaveButtonClick();
    virtual void onCancelButtonClick();
    virtual bool shouldClose() const;

};
