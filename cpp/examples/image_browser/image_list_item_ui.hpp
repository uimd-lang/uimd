// Auto-generated UI code for image_list_item - DO NOT EDIT MANUALLY.
#pragma once

#include "ui/generated/GeneratedWindowBase.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"
#include "ui/elements/Button.hpp"
#include "ui/elements/CheckBox.hpp"
#include "ui/elements/ComboBox.hpp"
#include "ui/elements/FrameBufferView.hpp"
#include "ui/elements/Image.hpp"
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

class ImageListItemUI : public ui::GeneratedWindowBase
{
public:
    ImageListItemUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::CheckBox* sel = nullptr;
    ui::Image* thumb = nullptr;
    ui::Label* name_label = nullptr;
    ui::Label* path_label = nullptr;
    ui::Label* spacer = nullptr;
    ui::Button* show_btn = nullptr;
    ui::Button* browse_btn = nullptr;
    ui::Button* delete_btn = nullptr;

protected:
    virtual void onSelChange(std::string_view value);
    virtual void onThumbClick();
    virtual void onShowBtnClick();
    virtual void onBrowseBtnClick();
    virtual void onDeleteBtnClick();
    virtual bool shouldClose() const;

};
