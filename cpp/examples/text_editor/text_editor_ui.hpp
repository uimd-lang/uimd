// Auto-generated UI code for text_editor - DO NOT EDIT MANUALLY.
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

class TextEditorUI : public ui::GeneratedWindowBase
{
public:
    TextEditorUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* window_header = nullptr;
    ui::Label* filename = nullptr;
    ui::TextArea* editor = nullptr;
    ui::Button* new_btn = nullptr;
    ui::Button* open_btn = nullptr;
    ui::Button* save_btn = nullptr;
    ui::Button* save_as_btn = nullptr;
    ui::Button* quit_btn = nullptr;
    ui::Label* status = nullptr;

protected:
    virtual void onEditorChange(std::string_view value);
    virtual void onEditorSubmit(std::string_view value);
    virtual void onNewBtnClick();
    virtual void onOpenBtnClick();
    virtual void onSaveBtnClick();
    virtual void onSaveAsBtnClick();
    virtual void onQuitBtnClick();
    virtual bool shouldClose() const;

};
