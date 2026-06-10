#include "mcp_tester_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "app_header", .type = "label", .cellName = "app", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 87, 15}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{86, 1}, .cellCharsSize = ui::Size{87, 15}, .marginRight = 1, .marginBottom = 14, .elementStyle = makeStyle({{"background", "#000000"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "log_output", .type = "log_panel", .cellName = "log", .relative = ui::Rect{0, 0, 536870911, 10}, .sourceCell = ui::Rect{18, 0, 87, 10}, .width = dimension("expanded", 536870911), .height = dimension("auto", 10), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 10), .charsSize = ui::Size{87, 10}, .cellCharsSize = ui::Size{87, 10}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#000000"}})},
        ui::GeneratedLayoutEntry{.name = "header", .type = "label", .cellName = "header_cell", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 87, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{41, 1}, .cellCharsSize = ui::Size{87, 1}, .marginRight = 45, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#1f3a5f"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "run", .type = "button", .cellName = "header_cell", .relative = ui::Rect{0, 43, 6, 1}, .sourceCell = ui::Rect{0, 0, 87, 1}, .width = dimension("auto", 6), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{6, 1}, .cellCharsSize = ui::Size{87, 1}, .marginRight = 38, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#14532d"}, {"color", "#dcfce7"}})},
        ui::GeneratedLayoutEntry{.name = "next", .type = "button", .cellName = "header_cell", .relative = ui::Rect{0, 50, 7, 1}, .sourceCell = ui::Rect{0, 0, 87, 1}, .width = dimension("auto", 7), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{7, 1}, .cellCharsSize = ui::Size{87, 1}, .marginRight = 30, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#1e3a8a"}, {"color", "#dbeafe"}})},
        ui::GeneratedLayoutEntry{.name = "pause", .type = "button", .cellName = "header_cell", .relative = ui::Rect{0, 58, 8, 1}, .sourceCell = ui::Rect{0, 0, 87, 1}, .width = dimension("auto", 8), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{8, 1}, .cellCharsSize = ui::Size{87, 1}, .marginRight = 21, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#713f12"}, {"color", "#fef3c7"}})},
        ui::GeneratedLayoutEntry{.name = "copy", .type = "button", .cellName = "header_cell", .relative = ui::Rect{0, 67, 11, 1}, .sourceCell = ui::Rect{0, 0, 87, 1}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{87, 1}, .marginRight = 9, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "quit", .type = "button", .cellName = "header_cell", .relative = ui::Rect{0, 79, 7, 1}, .sourceCell = ui::Rect{0, 0, 87, 1}, .width = dimension("auto", 7), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{7, 1}, .cellCharsSize = ui::Size{87, 1}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#fee2e2"}})},
        ui::GeneratedLayoutEntry{.name = "app_viewport", .type = "framebufferview", .cellName = "app", .relative = ui::Rect{1, 0, 536870911, 536870911}, .sourceCell = ui::Rect{2, 0, 87, 15}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{86, 14}, .cellCharsSize = ui::Size{87, 15}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#0b1018"}, {"color", "#e5e7eb"}})},
    };
}

}  // namespace

McpTesterUI::McpTesterUI() : ui::GeneratedWindowBase("MCP Tester")
{
    setGeneratedLayout(buildLayout());
    setMcpMetadata(true, "McpTester", R"UI_MCP_MD(# MCP Tester

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "MCP Tester UI source."
tags: [example]
```

## Definition

```yaml
uses:
  - log_panel
```

## Members

```yaml
header:
  type: label
  text: "MCP Tester"
  description: "MCP tester window title."
  expose: false

run:
  type: button
  title: Run

next:
  type: button
  title: Next

pause:
  type: button
  title: Pause

copy:
  type: button
  title: "Copy Log"

quit:
  type: button
  title: Quit

app_viewport:
  type: framebufferview

app_header:
  type: label
  text: ""

log_output:
  type: log_panel
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#0b1018"
@header:
  text-align: center
  color: "#ffffff"
  background: "#1f3a5f"
@run:
  background: "#14532d"
  color: "#dcfce7"
  focus-background: "#16a34a"
@next:
  background: "#1e3a8a"
  color: "#dbeafe"
  focus-background: "#2563eb"
@pause:
  background: "#713f12"
  color: "#fef3c7"
  focus-background: "#ca8a04"
@copy:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@quit:
  background: "#7f1d1d"
  color: "#fee2e2"
  focus-background: "#dc2626"
@app_viewport:
  background: "#0b1018"
  color: "#e5e7eb"
  focus-color: "#facc15"
  edit-color: "#22c55e"
  word-wrap: false
@app_header:
  background: "#000000"
  color: "#e5e7eb"
  word-wrap: false
@log_output:
  background: "#000000"
  focus-background: "#1e3a5f"
  edit-background: "#0f2a4a"
```

## User Interface

```ui
+-header_cell-**------------------------------------------------------------------------+
| header..**............................... run... next... pause... copy....... quit... |
+-app-**--------------------------------------------------------------------------------+
|app_header...**....................................................................... |
|app_viewport..**...................................................................... |
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
**......................................................................................|
**......................................................................................|
+-log-**--------------------------------------------------------------------------------+
|log_output..**.........................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
|.......................................................................................|
+---------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "MCP Tester UI source.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header", "MCP tester window title.", false}, ui::GeneratedElementMetadata{"run", "Run", true}, ui::GeneratedElementMetadata{"next", "Next", true}, ui::GeneratedElementMetadata{"pause", "Pause", true}, ui::GeneratedElementMetadata{"copy", "Copy Log", true}, ui::GeneratedElementMetadata{"quit", "Quit", true}, ui::GeneratedElementMetadata{"app_viewport", "app_viewport", true}, ui::GeneratedElementMetadata{"app_header", "app_header", true}, ui::GeneratedElementMetadata{"log_output", "log_output", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#0b1018"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header = &static_cast<ui::Label&>(addElement<ui::Label>("header", "MCP Tester"));
    header->setStyle(makeStyle({{"background", "#1f3a5f"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    run = &static_cast<ui::Button&>(addElement<ui::Button>("run", "Run"));
    run->setStyle(makeStyle({{"background", "#14532d"}, {"color", "#dcfce7"}}));
    run->setFocusStyle(makeStyle({{"background", "#16a34a"}, {"color", "#ffffff"}}));
    next = &static_cast<ui::Button&>(addElement<ui::Button>("next", "Next"));
    next->setStyle(makeStyle({{"background", "#1e3a8a"}, {"color", "#dbeafe"}}));
    next->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    pause = &static_cast<ui::Button&>(addElement<ui::Button>("pause", "Pause"));
    pause->setStyle(makeStyle({{"background", "#713f12"}, {"color", "#fef3c7"}}));
    pause->setFocusStyle(makeStyle({{"background", "#ca8a04"}, {"color", "#ffffff"}}));
    copy = &static_cast<ui::Button&>(addElement<ui::Button>("copy", "Copy Log"));
    copy->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    copy->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    quit = &static_cast<ui::Button&>(addElement<ui::Button>("quit", "Quit"));
    quit->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#fee2e2"}}));
    quit->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    app_viewport = &static_cast<ui::FrameBufferView&>(addElement<ui::FrameBufferView>("app_viewport"));
    app_viewport->setStyle(makeStyle({{"background", "#0b1018"}, {"color", "#e5e7eb"}}));
    app_viewport->setFocusStyle(makeStyle({{"color", "#facc15"}}));
    app_viewport->setEditStyle(makeStyle({{"color", "#22c55e"}}));
    app_header = &static_cast<ui::Label&>(addElement<ui::Label>("app_header", ""));
    app_header->setStyle(makeStyle({{"background", "#000000"}, {"color", "#e5e7eb"}}));
    app_header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    log_output = &static_cast<ui::ScrollView&>(addElement<ui::ScrollView>("log_output", 0));
    log_output->setStyle(makeStyle({{"background", "#000000"}, {"gap", "0"}}));
    log_output->setFocusStyle(makeStyle({{"background", "#1e3a5f"}}));
    log_output->setEditStyle(makeStyle({{"background", "#0f2a4a"}}));
    log_output->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
}

void McpTesterUI::onRunClick()
{
}

void McpTesterUI::onNextClick()
{
}

void McpTesterUI::onPauseClick()
{
}

void McpTesterUI::onCopyClick()
{
}

void McpTesterUI::onQuitClick()
{
}

bool McpTesterUI::shouldClose() const
{
    return false;
}

bool McpTesterUI::handleGeneratedButton(std::string_view name)
{
    if (name == "run")
    {
        onRunClick();
        return true;
    }
    else if (name == "next")
    {
        onNextClick();
        return true;
    }
    else if (name == "pause")
    {
        onPauseClick();
        return true;
    }
    else if (name == "copy")
    {
        onCopyClick();
        return true;
    }
    else if (name == "quit")
    {
        onQuitClick();
        return true;
    }
    return false;
}

bool McpTesterUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool McpTesterUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool McpTesterUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions McpTesterUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "run")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "next")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "pause")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "copy")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "quit")
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
