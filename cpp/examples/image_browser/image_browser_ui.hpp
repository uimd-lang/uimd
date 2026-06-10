// Auto-generated UI code for image_browser - DO NOT EDIT MANUALLY.
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

class ImageBrowserUI : public ui::GeneratedWindowBase
{
public:
    ImageBrowserUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* title = nullptr;
    ui::Button* close_btn = nullptr;
    ui::Label* render_label = nullptr;
    ui::ComboBox* mode_select = nullptr;
    ui::Label* sidebar_title = nullptr;
    ui::ReusableElement* camera_thumb = nullptr;
    ui::ReusableElement* astro_thumb = nullptr;
    ui::ReusableElement* coffee_thumb = nullptr;
    ui::Label* gallery_label = nullptr;
    ui::Label* image_list_label = nullptr;
    ui::ReusableElement* gallery_mosaic = nullptr;
    ui::ReusableElement* image_list_btn = nullptr;
    ui::Label* spacer = nullptr;
    ui::ViewHost* main = nullptr;

protected:
    virtual void onCloseBtnClick();
    virtual void onModeSelectChange(std::string_view value);
    virtual bool shouldClose() const;

};
