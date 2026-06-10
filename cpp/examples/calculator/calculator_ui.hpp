// Auto-generated UI code for calculator - DO NOT EDIT MANUALLY.
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

class CalculatorUI : public ui::GeneratedWindowBase
{
public:
    CalculatorUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Label* display = nullptr;
    ui::Button* n0 = nullptr;
    ui::Button* n1 = nullptr;
    ui::Button* n2 = nullptr;
    ui::Button* n3 = nullptr;
    ui::Button* n4 = nullptr;
    ui::Button* n5 = nullptr;
    ui::Button* n6 = nullptr;
    ui::Button* n7 = nullptr;
    ui::Button* n8 = nullptr;
    ui::Button* n9 = nullptr;
    ui::Button* sb = nullptr;
    ui::Button* dt = nullptr;
    ui::Button* ac = nullptr;
    ui::Button* dl = nullptr;
    ui::Button* pr = nullptr;
    ui::Button* dv = nullptr;
    ui::Button* ml = nullptr;
    ui::Button* mn = nullptr;
    ui::Button* ad = nullptr;

protected:
    virtual void onN0Click();
    virtual void onN1Click();
    virtual void onN2Click();
    virtual void onN3Click();
    virtual void onN4Click();
    virtual void onN5Click();
    virtual void onN6Click();
    virtual void onN7Click();
    virtual void onN8Click();
    virtual void onN9Click();
    virtual void onSbClick();
    virtual void onDtClick();
    virtual void onAcClick();
    virtual void onDlClick();
    virtual void onPrClick();
    virtual void onDvClick();
    virtual void onMlClick();
    virtual void onMnClick();
    virtual void onAdClick();
    virtual bool shouldClose() const;

};
