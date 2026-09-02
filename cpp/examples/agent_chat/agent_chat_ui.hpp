// Auto-generated UI code for agent_chat - DO NOT EDIT MANUALLY.
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
#include "transcript/transcript.hpp"


#include <string>
#include <string_view>
#include <vector>

class AgentChatUI : public ui::GeneratedWindowBase
{
public:
    AgentChatUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* title = nullptr;
    ui::Label* simulation_badge = nullptr;
    ui::Button* quit_button = nullptr;
    ui::Button* start_button = nullptr;
    ui::Button* pause_button = nullptr;
    ui::Button* next_step_button = nullptr;
    ui::Button* reset_button = nullptr;
    ui::Button* load_stress_button = nullptr;
    ui::ReusableElement* transcript = nullptr;
    ui::Label* status = nullptr;

protected:
    virtual void onQuitButtonClick();
    virtual void onStartButtonClick();
    virtual void onPauseButtonClick();
    virtual void onNextStepButtonClick();
    virtual void onResetButtonClick();
    virtual void onLoadStressButtonClick();
    virtual bool shouldClose() const;

};
