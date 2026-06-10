#include "log_panel_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "", .type = "", .cellName = "panel", .relative = ui::Rect{0, 0, 536870911, 10}, .sourceCell = ui::Rect{0, 0, 87, 10}, .width = dimension("auto", 536870911), .height = dimension("auto", 10), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 10), .charsSize = ui::Size{87, 10}, .cellCharsSize = ui::Size{87, 10}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background", "transparent"}, {"gap", "0"}})},
    };
}

}  // namespace

LogPanelUI::LogPanelUI() : ui::GeneratedWindowBase("Log Panel")
{
    setGeneratedLayout(buildLayout());
    setMcpMetadata(true, "LogPanel", R"UI_MCP_MD(# Log Panel

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: stable
description: "Log Panel UI source."
tags: [component]
```

## Definition

```yaml
extends: uiscrollview
```

## Style

```yaml
this:
  border-width: 0
  background: transparent
@panel:
  background: transparent
  gap: 0
@log_normal:
  background: transparent
  color: "#dddddd"
  user-select: text
  word-wrap: false
@log_assert_ok:
  background: transparent
  color: "#15803d"
  user-select: text
  word-wrap: false
@log_assert_fail:
  background: transparent
  color: "#b91c1c"
  user-select: text
  word-wrap: false
@log_result_ok:
  background: transparent
  color: "#22c55e"
  user-select: text
  word-wrap: false
@log_result_fail:
  background: transparent
  color: "#ef4444"
  user-select: text
  word-wrap: false
```

## User Interface

```ui
+-panel-**------------------------------------------------------------------------------+
|                                                                                       |
|                                                                                       |
|                                                                                       |
|                                                                                       |
|                                                                                       |
|                                                                                       |
|                                                                                       |
|                                                                                       |
|                                                                                       |
*                                                                                       |
+---------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Log Panel UI source.", std::vector<ui::GeneratedElementMetadata>{});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "transparent"}, {"border-color", "transparent"}, {"border-width", "0"}}));
}

bool LogPanelUI::shouldClose() const
{
    return false;
}

bool LogPanelUI::handleGeneratedButton(std::string_view name)
{
    return false;
}

bool LogPanelUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool LogPanelUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool LogPanelUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions LogPanelUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
