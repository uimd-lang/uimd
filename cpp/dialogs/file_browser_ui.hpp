// Auto-generated UI code for file_browser - DO NOT EDIT MANUALLY.
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

class FileBrowserUI : public ui::GeneratedWindowBase
{
public:
    FileBrowserUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* dialog_header = nullptr;
    ui::Label* path_label = nullptr;
    ui::ListBox* entries = nullptr;
    ui::TextInput* filename = nullptr;
    ui::Button* open_btn = nullptr;
    ui::Button* close_btn = nullptr;

protected:
    virtual void onEntriesSelectionChange(const std::vector<std::string>& value);
    virtual void onFilenameChange(std::string_view value);
    virtual void onFilenameSubmit(std::string_view value);
    virtual void onOpenBtnClick();
    virtual void onCloseBtnClick();
    virtual bool shouldClose() const;

};
