// Auto-generated UI code for activity_feed - DO NOT EDIT MANUALLY.
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

class ActivityFeedUI : public ui::GeneratedWindowBase
{
public:
    ActivityFeedUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* header_title = nullptr;
    ui::Button* settings_button = nullptr;
    ui::Button* quit_button = nullptr;
    ui::ReusableElement* feed = nullptr;
    ui::Label* type_label = nullptr;
    ui::ComboBox* activity_type = nullptr;
    ui::Label* message_label = nullptr;
    ui::TextArea* message = nullptr;
    ui::Button* add_button = nullptr;
    ui::Button* seed_button = nullptr;
    ui::Button* clear_button = nullptr;
    ui::Label* status = nullptr;

protected:
    virtual void onSettingsButtonClick();
    virtual void onQuitButtonClick();
    virtual void onActivityTypeChange(std::string_view value);
    virtual void onMessageChange(std::string_view value);
    virtual void onMessageSubmit(std::string_view value);
    virtual void onAddButtonClick();
    virtual void onSeedButtonClick();
    virtual void onClearButtonClick();
    virtual bool shouldClose() const;

};
