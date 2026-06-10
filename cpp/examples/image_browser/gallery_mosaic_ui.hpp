// Auto-generated UI code for gallery_mosaic - DO NOT EDIT MANUALLY.
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

class GalleryMosaicUI : public ui::GeneratedWindowBase
{
public:
    GalleryMosaicUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Image* gal_1 = nullptr;
    ui::Image* gal_2 = nullptr;
    ui::Image* gal_3 = nullptr;
    ui::Image* gal_4 = nullptr;
    ui::Image* gal_5 = nullptr;

protected:
    virtual void onGal1Click();
    virtual void onGal2Click();
    virtual void onGal3Click();
    virtual void onGal4Click();
    virtual void onGal5Click();
    virtual bool shouldClose() const;

};
