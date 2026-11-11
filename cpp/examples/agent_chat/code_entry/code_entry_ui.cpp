#include "code_entry_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "heading", .type = "spanlabel", .cellName = "code_panel", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 73, 5}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("fit-content", 536870910), .charsSize = ui::Size{71, 1}, .cellCharsSize = ui::Size{73, 5}, .marginRight = 1, .marginBottom = 4, .cellStyle = makeStyle({{"background", "#0f1b2d"}, {"padding", "0, 1, 0, 1"}}), .elementStyle = makeStyle({{"color", "#ddd6fe"}})},
        ui::GeneratedLayoutEntry{.name = "code", .type = "label", .cellName = "code_panel", .relative = ui::Rect{1, 1, 536870911, 536870910}, .sourceCell = ui::Rect{0, 0, 73, 5}, .width = dimension("expanded", 536870911), .height = dimension("fit-content", 536870910), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("fit-content", 536870910), .charsSize = ui::Size{71, 4}, .cellCharsSize = ui::Size{73, 5}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#0f1b2d"}, {"padding", "0, 1, 0, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}, {"user-select", "text"}})},
    };
}

}  // namespace

CodeEntryUI::CodeEntryUI() : ui::GeneratedWindowBase("Agent Chat Code Entry")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "CodeEntry", R"UI_MCP_MD(# Agent Chat Code Entry

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Heron-style code, activity, command output, or diff block."
tags: [component, simulation]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
heading:
  type: spanlabel
  text: "Code"
  description: "Code language or activity title."

code:
  type: label
  text: ""
  description: "Selectable formatted code, command output, or diff content."
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@code_panel:
  background: "#0f1b2d"
  padding: 0, 1, 0, 1
@heading:
  color: "#ddd6fe"
@code:
  color: "#d1d5db"
  word-wrap: true
  user-select: text
```

## User Interface

```ui
+-code_panel-**-----------------------------------------------------------+
| heading..**............................................................ |
| code..**............................................................... |
| ....................................................................... |
# #...................................................................... |
# #...................................................................... |
+-------------------------------------------------------------------------+
```
)UI_MCP_MD", "Heron-style code, activity, command output, or diff block.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"heading", "Code language or activity title.", true}, ui::GeneratedElementMetadata{"code", "Selectable formatted code, command output, or diff content.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#00000000"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    heading = &static_cast<ui::Label&>(addElement<ui::Label>("heading", "Code"));
    heading->setStyle(makeStyle({{"color", "#ddd6fe"}}));
    code = &static_cast<ui::Label&>(addElement<ui::Label>("code", ""));
    code->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}, {"user-select", "text"}}));
    code->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

bool CodeEntryUI::shouldClose() const
{
    return false;
}

bool CodeEntryUI::handleGeneratedButton(std::string_view name)
{
    (void)name;
    return false;
}

bool CodeEntryUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool CodeEntryUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool CodeEntryUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    (void)name;
    (void)value;
    return false;
}

ui::GeneratedWindowRuntimeOptions CodeEntryUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
