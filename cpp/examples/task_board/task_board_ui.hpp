// Auto-generated UI code for task_board - DO NOT EDIT MANUALLY.
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
#include "task_list/task_list.hpp"


#include <string>
#include <string_view>
#include <vector>

class TaskBoardUI : public ui::GeneratedWindowBase
{
public:
    TaskBoardUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;

    std::string callAppTool(std::string_view name, std::string_view inputJson) override;

    ui::Label* title = nullptr;
    ui::Button* quit_btn = nullptr;
    ui::Label* search_label = nullptr;
    ui::TextInput* search = nullptr;
    ui::Label* status_label = nullptr;
    ui::ComboBox* status_filter = nullptr;
    ui::Label* owner_label = nullptr;
    ui::ComboBox* owner_filter = nullptr;
    ui::Button* reset_filters_btn = nullptr;
    ui::Button* new_task_btn = nullptr;
    ui::ReusableElement* board = nullptr;
    ui::Label* status = nullptr;

protected:
    virtual void onQuitBtnClick();
    virtual void onSearchChange(std::string_view value);
    virtual void onSearchSubmit(std::string_view value);
    virtual void onStatusFilterChange(std::string_view value);
    virtual void onOwnerFilterChange(std::string_view value);
    virtual void onResetFiltersBtnClick();
    virtual void onNewTaskBtnClick();
    virtual bool shouldClose() const;

    virtual std::string deleteTask(std::string_view inputJson);
    virtual std::string getTask(std::string_view inputJson);
    virtual std::string queryTasks(std::string_view inputJson);
    virtual std::string updateTask(std::string_view inputJson);
};
