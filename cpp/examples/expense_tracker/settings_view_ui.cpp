#include "settings_view_ui.hpp"

#include <memory>
#include <array>
#include <charconv>
#include <initializer_list>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>

namespace
{

ui::AxisDimension dimension(const char* mode, int value)
{
    const std::string text(mode);
    if (text == "fixed")
    {
        return ui::AxisDimension::fixed(value);
    }
    if (text == "expanded")
    {
        return ui::AxisDimension::expanded();
    }
    if (text == "fit-content")
    {
        return ui::AxisDimension::fitContent();
    }
    return ui::AxisDimension{};
}

ui::Style makeStyle(std::initializer_list<std::pair<const char*, const char*>> props)
{
    ui::Style style;
    auto parseInt = [](std::string_view text, int fallback = 0)
    {
        int value = fallback;
        const auto* begin = text.data();
        const auto* end = text.data() + text.size();
        (void)std::from_chars(begin, end, value);
        return value;
    };
    auto setPadding = [&](std::string_view value)
    {
        std::array<int, 4> values{0, 0, 0, 0};
        std::size_t start = 0;
        int count = 0;
        while (start <= value.size() && count < 4)
        {
            const std::size_t comma = value.find(',', start);
            const std::size_t end = comma == std::string_view::npos ? value.size() : comma;
            while (start < end && value[start] == ' ')
            {
                ++start;
            }
            values[static_cast<std::size_t>(count++)] = parseInt(value.substr(start, end - start));
            if (comma == std::string_view::npos)
            {
                break;
            }
            start = comma + 1;
        }
        if (count == 1)
        {
            style.padding = values[0];
            style.paddingTop = values[0];
            style.paddingRight = values[0];
            style.paddingBottom = values[0];
            style.paddingLeft = values[0];
        }
        else if (count == 2)
        {
            style.paddingTop = values[0];
            style.paddingRight = values[1];
            style.paddingBottom = values[0];
            style.paddingLeft = values[1];
        }
        else if (count >= 4)
        {
            style.paddingTop = values[0];
            style.paddingRight = values[1];
            style.paddingBottom = values[2];
            style.paddingLeft = values[3];
        }
    };
    auto setMargin = [&](std::string_view value)
    {
        std::array<int, 4> values{0, 0, 0, 0};
        std::size_t start = 0;
        int count = 0;
        while (start <= value.size() && count < 4)
        {
            const std::size_t comma = value.find(',', start);
            const std::size_t end = comma == std::string_view::npos ? value.size() : comma;
            while (start < end && value[start] == ' ')
            {
                ++start;
            }
            values[static_cast<std::size_t>(count++)] = parseInt(value.substr(start, end - start));
            if (comma == std::string_view::npos)
            {
                break;
            }
            start = comma + 1;
        }
        if (count == 1)
        {
            style.margin = values[0];
            style.marginTop = values[0];
            style.marginRight = values[0];
            style.marginBottom = values[0];
            style.marginLeft = values[0];
        }
        else if (count == 2)
        {
            style.marginTop = values[0];
            style.marginRight = values[1];
            style.marginBottom = values[0];
            style.marginLeft = values[1];
        }
        else if (count >= 4)
        {
            style.marginTop = values[0];
            style.marginRight = values[1];
            style.marginBottom = values[2];
            style.marginLeft = values[3];
        }
    };
    for (const auto& [key, value] : props)
    {
        const std::string prop(key);
        if (prop == "color")
        {
            style.color = ui::Color(value);
        }
        else if (prop == "background")
        {
            style.background = ui::Color(value);
        }
        else if (prop == "background-texture")
        {
            style.backgroundTexture = value;
        }
        else if (prop == "background-texture-color")
        {
            style.backgroundTextureColor = ui::Color(value);
        }
        else if (prop == "scope-dim-background")
        {
            style.scopeDimBackground = ui::Color(value);
        }
        else if (prop == "border-color")
        {
            style.borderColor = ui::Color(value);
        }
        else if (prop == "border-width")
        {
            style.borderWidthHorizontal = parseInt(value);
            style.borderWidthVertical = parseInt(value);
        }
        else if (prop == "border-width-horizontal")
        {
            style.borderWidthHorizontal = parseInt(value);
        }
        else if (prop == "border-width-vertical")
        {
            style.borderWidthVertical = parseInt(value);
        }
        else if (prop == "padding")
        {
            setPadding(value);
        }
        else if (prop == "margin")
        {
            setMargin(value);
        }
        else if (prop == "gap")
        {
            style.gap = parseInt(value);
        }
        else if (prop == "text-align")
        {
            style.textAlign = value;
        }
        else if (prop == "user-select")
        {
            style.userSelect = value;
        }
        else if (prop == "scroll-x")
        {
            style.scrollX = (std::string_view(value) == "true" || std::string_view(value) == "1" || std::string_view(value) == "yes");
        }
        else if (prop == "scroll-y")
        {
            style.scrollY = (std::string_view(value) == "true" || std::string_view(value) == "1" || std::string_view(value) == "yes");
        }
    }
    return style;
}

ui::TextGradient makeTextGradient(int intervalMs, int step, int segmentSize, std::initializer_list<const char*> colors)
{
    ui::TextGradient gradient;
    gradient.intervalMs = intervalMs;
    gradient.step = step;
    gradient.segmentSize = segmentSize;
    for (const char* color : colors)
    {
        gradient.colors.emplace_back(color);
    }
    return gradient;
}

std::vector<ui::GeneratedLayoutEntry> buildLayout()
{
    return {
        ui::GeneratedLayoutEntry{.name = "view_title", .type = "label", .cellName = "view", .relative = ui::Rect{0, 1, 58, 1}, .sourceCell = ui::Rect{0, 0, 60, 9}, .width = dimension("auto", 58), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{58, 1}, .cellCharsSize = ui::Size{60, 9}, .marginRight = 1, .marginBottom = 8, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#fde68a"}})},
        ui::GeneratedLayoutEntry{.name = "\"Currency:\"", .type = "label", .cellName = "view", .relative = ui::Rect{2, 1, 11, 1}, .sourceCell = ui::Rect{0, 0, 60, 9}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{60, 9}, .marginRight = 48, .marginBottom = 6, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "currency", .type = "combobox", .cellName = "view", .relative = ui::Rect{2, 18, 41, 1}, .sourceCell = ui::Rect{0, 0, 60, 9}, .width = dimension("auto", 41), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{41, 1}, .cellCharsSize = ui::Size{60, 9}, .marginRight = 1, .marginBottom = 6, .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "show_paid", .type = "checkbox", .cellName = "view", .relative = ui::Rect{4, 1, 58, 1}, .sourceCell = ui::Rect{0, 0, 60, 9}, .width = dimension("auto", 58), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{58, 1}, .cellCharsSize = ui::Size{60, 9}, .marginRight = 1, .marginBottom = 4, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#b8c4d8"}})},
        ui::GeneratedLayoutEntry{.name = "\"Monthly limit:\"", .type = "label", .cellName = "view", .relative = ui::Rect{6, 1, 16, 1}, .sourceCell = ui::Rect{0, 0, 60, 9}, .width = dimension("auto", 16), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{16, 1}, .cellCharsSize = ui::Size{60, 9}, .marginRight = 43, .marginBottom = 2, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "monthly_limit", .type = "numberinput", .cellName = "view", .relative = ui::Rect{6, 18, 41, 1}, .sourceCell = ui::Rect{0, 0, 60, 9}, .width = dimension("auto", 41), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{41, 1}, .cellCharsSize = ui::Size{60, 9}, .marginRight = 1, .marginBottom = 2, .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "reset_btn", .type = "button", .cellName = "view", .relative = ui::Rect{8, 1, 58, 1}, .sourceCell = ui::Rect{0, 0, 60, 9}, .width = dimension("auto", 58), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{58, 1}, .cellCharsSize = ui::Size{60, 9}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

SettingsViewUI::SettingsViewUI() : ui::GeneratedWindowBase("Settings View")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("control");
    setMcpMetadata(true, "SettingsView", R"UI_MCP_MD(# Settings View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
status: experimental
description: "Expense tracker settings view."
tags: [example, viewhost, expenses]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
view_title:
  type: label
  text: Settings

currency:
  type: combobox
  options: [EUR, USD, CZK]
  selected_item: EUR

show_paid:
  type: checkbox
  title: Show paid expenses
  value: true

monthly_limit:
  type: numberinput
  value: 500
  format: "0"

reset_btn:
  type: button
  title: Reset demo data
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@view_title:
  color: "#fde68a"
numberinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
combobox:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
```

## User Interface

```ui
+-view-**----------------------------------------------------+
| view_title................................................ |
|                                                            |
| "Currency:"      currency................................. |
|                                                            |
| show_paid................................................. |
|                                                            |
| "Monthly limit:" monthly_limit............................ |
|                                                            |
| reset_btn................................................. |
+------------------------------------------------------------+
```
)UI_MCP_MD", "Expense tracker settings view.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"view_title", "Settings", true}, ui::GeneratedElementMetadata{"currency", "currency", true}, ui::GeneratedElementMetadata{"show_paid", "Show paid expenses", true}, ui::GeneratedElementMetadata{"monthly_limit", "monthly_limit", true}, ui::GeneratedElementMetadata{"reset_btn", "Reset demo data", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#030712"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    view_title = &static_cast<ui::Label&>(addElement<ui::Label>("view_title", "Settings"));
    view_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#fde68a"}}));
    view_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    currency = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("currency", std::vector<std::string>{"EUR", "USD", "CZK"}));
    currency->setSelectedIndex(0);
    currency->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    currency->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    currency->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    currency->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    currency->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    show_paid = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("show_paid", "Show paid expenses", true));
    show_paid->setStyle(makeStyle({{"background", "transparent"}, {"color", "#b8c4d8"}}));
    show_paid->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#ffffff"}}));
    show_paid->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#44cc88"}}));
    show_paid->setUncheckedStyle(makeStyle({{"background", "transparent"}}));
    show_paid->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    monthly_limit = &static_cast<ui::NumberInput&>(addElement<ui::NumberInput>("monthly_limit", 500.0, 1.0));
    monthly_limit->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    monthly_limit->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    monthly_limit->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#ffffff"}}));
    monthly_limit->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    monthly_limit->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    reset_btn = &static_cast<ui::Button&>(addElement<ui::Button>("reset_btn", "Reset demo data"));
    reset_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    reset_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    reset_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void SettingsViewUI::onCurrencyChange(std::string_view value)
{
    (void)value;
}

void SettingsViewUI::onShowPaidChange(std::string_view value)
{
    (void)value;
}

void SettingsViewUI::onMonthlyLimitChange(std::string_view value)
{
    (void)value;
}

void SettingsViewUI::onMonthlyLimitSubmit(std::string_view value)
{
    (void)value;
}

void SettingsViewUI::onResetBtnClick()
{
}

bool SettingsViewUI::shouldClose() const
{
    return false;
}

bool SettingsViewUI::handleGeneratedButton(std::string_view name)
{
    if (name == "reset_btn")
    {
        onResetBtnClick();
        return true;
    }
    return false;
}

bool SettingsViewUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "show_paid")
    {
        onShowPaidChange(value);
        return true;
    }
    else if (name == "monthly_limit")
    {
        onMonthlyLimitChange(value);
        return true;
    }
    return false;
}

bool SettingsViewUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "monthly_limit")
    {
        onMonthlyLimitSubmit(value);
        return true;
    }
    return false;
}

bool SettingsViewUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "currency")
    {
        onCurrencyChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions SettingsViewUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "reset_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "show_paid")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "monthly_limit")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "monthly_limit")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "currency")
        {
            (void)handleGeneratedSelectionChanged(name, value);
        }
    };

    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
