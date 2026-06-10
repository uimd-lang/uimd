// Auto-generated UI code for markdown_viewer - DO NOT EDIT MANUALLY.
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
#include "document_view/document_view.hpp"


#include <string>
#include <string_view>
#include <vector>

class MarkdownViewerUI : public ui::GeneratedWindowBase
{
public:
    MarkdownViewerUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* header = nullptr;
    ui::Label* docs_label = nullptr;
    ui::ListBox* docs = nullptr;
    ui::Button* up_btn = nullptr;
    ui::Button* down_btn = nullptr;
    ui::ReusableElement* viewer = nullptr;
    ui::Button* close_btn = nullptr;
    ui::Label* status = nullptr;

protected:
    virtual void onDocsSelectionChange(const std::vector<std::string>& value);
    virtual void onUpBtnClick();
    virtual void onDownBtnClick();
    virtual void onCloseBtnClick();
    virtual bool shouldClose() const;

};
