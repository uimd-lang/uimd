// Auto-generated UI code for cells - DO NOT EDIT MANUALLY.
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

class CellsUI : public ui::GeneratedWindowBase
{
public:
    CellsUI();

    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();
    bool handleGeneratedButton(std::string_view name) override;
    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;
    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;
    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;


    ui::Button* a1 = nullptr;
    ui::Button* a2 = nullptr;
    ui::Button* b1 = nullptr;
    ui::Button* c1 = nullptr;
    ui::Button* c2 = nullptr;
    ui::Button* c3 = nullptr;
    ui::Button* c4 = nullptr;
    ui::Button* c5 = nullptr;
    ui::Button* c6 = nullptr;
    ui::Button* c7 = nullptr;
    ui::Button* c8 = nullptr;
    ui::Button* c9 = nullptr;
    ui::Button* c10 = nullptr;
    ui::Button* d1 = nullptr;
    ui::Button* d2 = nullptr;
    ui::Button* d3 = nullptr;
    ui::Button* d4 = nullptr;
    ui::Button* d5 = nullptr;
    ui::Button* d6 = nullptr;
    ui::Button* e1 = nullptr;
    ui::Button* e2 = nullptr;
    ui::Button* e3 = nullptr;
    ui::Button* e4 = nullptr;
    ui::Button* e5 = nullptr;
    ui::Button* e6 = nullptr;
    ui::Button* e7 = nullptr;
    ui::Button* e8 = nullptr;
    ui::Button* f1 = nullptr;
    ui::Button* f2 = nullptr;
    ui::Button* f3 = nullptr;

protected:
    virtual void onA1Click();
    virtual void onA2Click();
    virtual void onB1Click();
    virtual void onC1Click();
    virtual void onC2Click();
    virtual void onC3Click();
    virtual void onC4Click();
    virtual void onC5Click();
    virtual void onC6Click();
    virtual void onC7Click();
    virtual void onC8Click();
    virtual void onC9Click();
    virtual void onC10Click();
    virtual void onD1Click();
    virtual void onD2Click();
    virtual void onD3Click();
    virtual void onD4Click();
    virtual void onD5Click();
    virtual void onD6Click();
    virtual void onE1Click();
    virtual void onE2Click();
    virtual void onE3Click();
    virtual void onE4Click();
    virtual void onE5Click();
    virtual void onE6Click();
    virtual void onE7Click();
    virtual void onE8Click();
    virtual void onF1Click();
    virtual void onF2Click();
    virtual void onF3Click();
    virtual bool shouldClose() const;

};
