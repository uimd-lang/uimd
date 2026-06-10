// Auto-generated UI code for special_elements - DO NOT EDIT MANUALLY.
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

class SpecialElementsUI : public ui::GeneratedWindowBase
{
public:
    SpecialElementsUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* header_label = nullptr;
    ui::Label* info_processing = nullptr;
    ui::Label* info_loading = nullptr;
    ui::Label* info_complete = nullptr;
    ui::Label* info_warning = nullptr;
    ui::Label* info_multiline = nullptr;
    ui::Label* status_label = nullptr;
    ui::Label* normal_info = nullptr;
    ui::Label* speed_label = nullptr;
    ui::Label* raw_label = nullptr;
    ui::Button* raw_button = nullptr;
    ui::ComboBox* raw_combo = nullptr;
    ui::ListBox* raw_list = nullptr;
    ui::TextInput* raw_input = nullptr;
    ui::TextArea* raw_area = nullptr;

protected:
    virtual void onRawButtonClick();
    virtual void onRawComboChange(std::string_view value);
    virtual void onRawListSelectionChange(const std::vector<std::string>& value);
    virtual void onRawInputChange(std::string_view value);
    virtual void onRawInputSubmit(std::string_view value);
    virtual void onRawAreaChange(std::string_view value);
    virtual void onRawAreaSubmit(std::string_view value);
    virtual bool shouldClose() const;

};
