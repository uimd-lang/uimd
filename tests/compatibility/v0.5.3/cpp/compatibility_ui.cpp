#include "compatibility_ui.hpp"
#include "../source/compat_scroll_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "aligned_input", .type = "textinput", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 32, 5}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 32), .cellHeight = dimension("auto", 5), .charsSize = ui::Size{32, 1}, .cellCharsSize = ui::Size{32, 5}, .marginRight = 0, .marginBottom = 4, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}, {"text-align", "right"}})},
        ui::GeneratedLayoutEntry{.name = "choice", .type = "combobox", .cellName = "", .relative = ui::Rect{1, 0, 536870911, 2}, .sourceCell = ui::Rect{0, 0, 32, 5}, .width = dimension("expanded", 536870911), .height = dimension("auto", 2), .cellWidth = dimension("auto", 32), .cellHeight = dimension("auto", 5), .charsSize = ui::Size{32, 2}, .cellCharsSize = ui::Size{32, 5}, .marginRight = 0, .marginBottom = 2, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "scroller", .type = "compat_scroll", .cellName = "", .relative = ui::Rect{3, 0, 536870911, 2}, .sourceCell = ui::Rect{0, 0, 32, 5}, .width = dimension("expanded", 536870911), .height = dimension("auto", 2), .cellWidth = dimension("auto", 32), .cellHeight = dimension("auto", 5), .charsSize = ui::Size{32, 2}, .cellCharsSize = ui::Size{32, 5}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#303545"}})},
    };
}

}  // namespace

CompatibilityUI::CompatibilityUI() : ui::GeneratedWindowBase("UIMD 0.5.3 Compatibility Application")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "Compatibility", R"UI_MCP_MD(# UIMD 0.5.3 Compatibility Application

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: regression
description: "Retained generated application for patch-level runtime compatibility."
tags: [compatibility]
```

## Definition

```yaml
uses:
  - compat_scroll
```

## Members

```yaml
aligned_input:
  type: textinput
  value: "abc"
  maxlength: 16

choice:
  type: combobox
  options: [one, two, three, four, five, six, seven, eight]
  selected_item: one

scroller:
  type: compat_scroll
```

## Style

```yaml
this:
  border-width: 0
  background: "#303545"
@aligned_input:
  text-align: right
@scroller:
  background: "#303545"
  focus-background: "#ffffff14"
```

## User Interface

```ui
+--------------------------------+
|aligned_input...................|
|choice..........................|
|................................|
|scroller........................|
|................................|
+--------------------------------+
```
)UI_MCP_MD", "Retained generated application for patch-level runtime compatibility.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"aligned_input", "aligned_input", true}, ui::GeneratedElementMetadata{"choice", "choice", true}, ui::GeneratedElementMetadata{"scroller", "scroller", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#303545"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    aligned_input = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("aligned_input", "abc", 16));
    aligned_input->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}, {"text-align", "right"}}));
    aligned_input->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    aligned_input->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    aligned_input->setCursorStyle(makeStyle({{"background", "#ffffff"}, {"color", "#0e1117"}}));
    aligned_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    aligned_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    choice = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("choice", std::vector<std::string>{"one", "two", "three", "four", "five", "six", "seven", "eight"}));
    choice->setSelectedIndex(0);
    choice->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    choice->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    choice->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#e5e7eb"}}));
    choice->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    choice->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    scroller = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("scroller"));
    scroller->setChild(std::make_unique<CompatScrollUI>());
    scroller->setStyle(makeStyle({{"background", "#303545"}}));
    scroller->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    scroller->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    scroller->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
}

void CompatibilityUI::onAlignedInputChange(std::string_view value)
{
    (void)value;
}

void CompatibilityUI::onAlignedInputSubmit(std::string_view value)
{
    (void)value;
}

void CompatibilityUI::onChoiceChange(std::string_view value)
{
    (void)value;
}

bool CompatibilityUI::shouldClose() const
{
    return false;
}

bool CompatibilityUI::handleGeneratedButton(std::string_view name)
{
    (void)name;
    return false;
}

bool CompatibilityUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    if (name == "aligned_input")
    {
        onAlignedInputChange(value);
        return true;
    }
    return false;
}

bool CompatibilityUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    if (name == "aligned_input")
    {
        onAlignedInputSubmit(value);
        return true;
    }
    return false;
}

bool CompatibilityUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    (void)name;
    (void)value;
    if (name == "choice")
    {
        onChoiceChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions CompatibilityUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "aligned_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "aligned_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "choice")
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
