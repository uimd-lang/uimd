// Auto-generated UI code for mcp_tester - DO NOT EDIT MANUALLY.
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

class McpTesterUI : public ui::GeneratedWindowBase
{
public:
    McpTesterUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* header = nullptr;
    ui::Button* run = nullptr;
    ui::Button* next = nullptr;
    ui::Button* pause = nullptr;
    ui::Button* copy = nullptr;
    ui::Button* quit = nullptr;
    ui::FrameBufferView* app_viewport = nullptr;
    ui::Label* app_header = nullptr;
    ui::ScrollView* log_output = nullptr;

protected:
    virtual void onRunClick();
    virtual void onNextClick();
    virtual void onPauseClick();
    virtual void onCopyClick();
    virtual void onQuitClick();
    virtual bool shouldClose() const;

};
