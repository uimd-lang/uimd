#include "item_row_ui.hpp"

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

[[maybe_unused]] ui::TextGradient makeTextGradient(int intervalMs, int step, int segmentSize, std::initializer_list<const char*> colors)
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
        ui::GeneratedLayoutEntry{.name = "name", .type = "label", .cellName = "card", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 90, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{59, 1}, .cellCharsSize = ui::Size{90, 2}, .marginRight = 30, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "summary", .type = "label", .cellName = "card", .relative = ui::Rect{0, 61, 28, 2}, .sourceCell = ui::Rect{0, 0, 90, 2}, .width = dimension("auto", 28), .height = dimension("auto", 2), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{28, 2}, .cellCharsSize = ui::Size{90, 2}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "spacer", .type = "label", .cellName = "card", .relative = ui::Rect{1, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 90, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{74, 1}, .cellCharsSize = ui::Size{90, 2}, .marginRight = 15, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "open_btn", .type = "button", .cellName = "card", .relative = ui::Rect{1, 76, 13, 1}, .sourceCell = ui::Rect{0, 0, 90, 2}, .width = dimension("auto", 13), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{90, 2}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

ItemRowUI::ItemRowUI() : ui::GeneratedWindowBase("Item Row")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "ItemRow", R"UI_MCP_MD(# Item Row

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: regression
description: "Interactive row with an Open button inside a ScrollView."
tags: [regression, issue-5, scrollview]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
name:
  type: label
  text: "Item"
  description: "Item name."

summary:
  type: label
  text: "Summary"
  description: "Item summary."

spacer:
  type: label
  text: ""
  description: "Flexible row spacer."
  expose: false

open_btn:
  type: button
  title: Open
  description: "Open this item in the edit page."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@card:
  background: "#172033"
  padding: 1, 1, 1, 1
@name:
  color: "#f8fafc"
  user-select: text
@summary:
  color: "#93c5fd"
  user-select: text
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@open_btn:
  background: "#14532d"
  focus-background: "#16a34a"
```

## User Interface

```ui
+-card-**----------------------------------------------------------------------------------+
| name..**................................................... summary..................... |
| spacer..**................................................................ open_btn..... |
+------------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Interactive row with an Open button inside a ScrollView.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"name", "Item name.", true}, ui::GeneratedElementMetadata{"summary", "Item summary.", true}, ui::GeneratedElementMetadata{"spacer", "Flexible row spacer.", false}, ui::GeneratedElementMetadata{"open_btn", "Open this item in the edit page.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    name = &static_cast<ui::Label&>(addElement<ui::Label>("name", "Item"));
    name->setStyle(makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"user-select", "text"}}));
    name->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    summary = &static_cast<ui::Label&>(addElement<ui::Label>("summary", "Summary"));
    summary->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}, {"user-select", "text"}}));
    summary->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    spacer = &static_cast<ui::Label&>(addElement<ui::Label>("spacer", ""));
    spacer->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    spacer->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    open_btn = &static_cast<ui::Button&>(addElement<ui::Button>("open_btn", "Open"));
    open_btn->setStyle(makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}}));
    open_btn->setFocusStyle(makeStyle({{"background", "#16a34a"}, {"color", "#ffffff"}}));
    open_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void ItemRowUI::onOpenBtnClick()
{
}

bool ItemRowUI::shouldClose() const
{
    return false;
}

bool ItemRowUI::handleGeneratedButton(std::string_view name)
{
    (void)name;
    if (name == "open_btn")
    {
        onOpenBtnClick();
        return true;
    }
    return false;
}

bool ItemRowUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool ItemRowUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool ItemRowUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    (void)name;
    (void)value;
    return false;
}

ui::GeneratedWindowRuntimeOptions ItemRowUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "open_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
