// Auto-generated UI code for task_dialog - DO NOT EDIT MANUALLY.
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

class TaskDialogUI : public ui::GeneratedWindowBase
{
public:
    TaskDialogUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* header = nullptr;
    ui::Label* name_label = nullptr;
    ui::TextInput* name = nullptr;
    ui::Label* status_label = nullptr;
    ui::ComboBox* project_status = nullptr;
    ui::Label* owner_label = nullptr;
    ui::ComboBox* owner = nullptr;
    ui::Label* priority_label = nullptr;
    ui::ComboBox* priority = nullptr;
    ui::Label* note_label = nullptr;
    ui::TextArea* note = nullptr;
    ui::Label* message = nullptr;
    ui::Button* add_btn = nullptr;
    ui::Button* cancel_btn = nullptr;

protected:
    virtual void onNameChange(std::string_view value);
    virtual void onNameSubmit(std::string_view value);
    virtual void onProjectStatusChange(std::string_view value);
    virtual void onOwnerChange(std::string_view value);
    virtual void onPriorityChange(std::string_view value);
    virtual void onNoteChange(std::string_view value);
    virtual void onNoteSubmit(std::string_view value);
    virtual void onAddBtnClick();
    virtual void onCancelBtnClick();
    virtual bool shouldClose() const;

};
