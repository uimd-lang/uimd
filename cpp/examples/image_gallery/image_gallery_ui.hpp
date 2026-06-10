// Auto-generated UI code for image_gallery - DO NOT EDIT MANUALLY.
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

class ImageGalleryUI : public ui::GeneratedWindowBase
{
public:
    ImageGalleryUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* header = nullptr;
    ui::Button* close_btn = nullptr;
    ui::Label* fallback_label = nullptr;
    ui::Label* sixel_label = nullptr;
    ui::Label* stretch_label = nullptr;
    ui::Image* camera_fb = nullptr;
    ui::Image* astro_fb = nullptr;
    ui::Image* coffee_fb = nullptr;
    ui::Image* coins_fb = nullptr;
    ui::Image* camera_ctn = nullptr;
    ui::Image* astro_ctn = nullptr;
    ui::Image* coffee_ctn = nullptr;
    ui::Image* coins_ctn = nullptr;
    ui::Image* camera_str = nullptr;
    ui::Image* astro_str = nullptr;
    ui::Image* coffee_str = nullptr;
    ui::Image* coins_str = nullptr;
    ui::Label* status = nullptr;

protected:
    virtual void onCloseBtnClick();
    virtual void onCameraFbClick();
    virtual void onAstroFbClick();
    virtual void onCoffeeFbClick();
    virtual void onCoinsFbClick();
    virtual void onCameraCtnClick();
    virtual void onAstroCtnClick();
    virtual void onCoffeeCtnClick();
    virtual void onCoinsCtnClick();
    virtual void onCameraStrClick();
    virtual void onAstroStrClick();
    virtual void onCoffeeStrClick();
    virtual void onCoinsStrClick();
    virtual bool shouldClose() const;

};
