#include "code_block_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "code", .type = "spanlabel", .cellName = "body", .relative = ui::Rect{0, 1, 536870911, 536870910}, .sourceCell = ui::Rect{2, 0, 48, 4}, .width = dimension("expanded", 536870911), .height = dimension("fit-content", 536870910), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("fit-content", 536870910), .charsSize = ui::Size{46, 4}, .cellCharsSize = ui::Size{48, 4}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#0c1a2d"}}), .elementStyle = makeStyle({{"color", "#d1d5db"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "lang", .type = "label", .cellName = "head", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 48, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{46, 1}, .cellCharsSize = ui::Size{48, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1d314c"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
    };
}

}  // namespace

CodeBlockUI::CodeBlockUI() : ui::GeneratedWindowBase("Code Block")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "CodeBlock", R"UI_MCP_MD(# Code Block

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: stable
description: "Code Block UI source."
tags: [component]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
lang:
  type: label
  text: code

code:
  type: spanlabel
  text: ""
```

## Style

```yaml
this:
  background: "#00000000"
  border-width: 0
@head:
  background: "#1d314c"
@body:
  background: "#0c1a2d"
@lang:
  color: "#93c5fd"
@code:
  color: "#d1d5db"
  word-wrap: false
  user-select: text
```

## User Interface

```ui
+-head-**----------------------------------------+
| lang..**...................................... |
+-body-**----------------------------------------+
| code..**...................................... |
| .............................................. |
# #............................................ |
# #............................................ |
+------------------------------------------------+
```
)UI_MCP_MD", "Code Block UI source.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"lang", "code", true}, ui::GeneratedElementMetadata{"code", "code", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#00000000"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    lang = &static_cast<ui::Label&>(addElement<ui::Label>("lang", "code"));
    lang->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    lang->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    code = &static_cast<ui::Label&>(addElement<ui::Label>("code", ""));
    code->setStyle(makeStyle({{"color", "#d1d5db"}, {"user-select", "text"}}));
}

bool CodeBlockUI::shouldClose() const
{
    return false;
}

bool CodeBlockUI::handleGeneratedButton(std::string_view name)
{
    return false;
}

bool CodeBlockUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool CodeBlockUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool CodeBlockUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions CodeBlockUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
