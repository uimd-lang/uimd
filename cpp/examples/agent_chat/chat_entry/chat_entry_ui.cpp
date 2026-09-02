#include "chat_entry_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "body", .type = "spanlabel", .cellName = "body_panel", .relative = ui::Rect{0, 1, 536870911, 6}, .sourceCell = ui::Rect{2, 0, 73, 6}, .width = dimension("expanded", 536870911), .height = dimension("auto", 6), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 6), .charsSize = ui::Size{71, 6}, .cellCharsSize = ui::Size{73, 6}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#0b1220"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"color", "#d1d5db"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "detail", .type = "label", .cellName = "footer", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{9, 0, 73, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{71, 1}, .cellCharsSize = ui::Size{73, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "0, 1, 0, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#64748b"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "role", .type = "label", .cellName = "header", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 73, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{48, 1}, .cellCharsSize = ui::Size{73, 1}, .marginRight = 24, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#162033"}, {"padding", "0, 1, 0, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#a7f3d0"}})},
        ui::GeneratedLayoutEntry{.name = "phase", .type = "label", .cellName = "header", .relative = ui::Rect{0, 50, 22, 1}, .sourceCell = ui::Rect{0, 0, 73, 1}, .width = dimension("auto", 22), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{22, 1}, .cellCharsSize = ui::Size{73, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#162033"}, {"padding", "0, 1, 0, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#c4b5fd"}, {"text-align", "right"}})},
    };
}

}  // namespace

ChatEntryUI::ChatEntryUI() : ui::GeneratedWindowBase("Agent Chat Entry")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "ChatEntry", R"UI_MCP_MD(# Agent Chat Entry

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Fixed-height simulated agent chat entry for prose and structured transcript blocks."
tags: [component, simulation]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
role:
  type: label
  text: Agent
  description: "Simulated transcript speaker or activity type."

phase:
  type: label
  text: response
  description: "Deterministic scenario phase."

body:
  type: spanlabel
  text: ""
  description: "Selectable simulated transcript content."

detail:
  type: label
  text: "simulated · no external action"
  description: "Simulation-only activity detail."
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@header:
  background: "#162033"
  padding: 0, 1, 0, 1
@role:
  color: "#a7f3d0"
@phase:
  color: "#c4b5fd"
  text-align: right
@body_panel:
  background: "#0b1220"
  padding: 1, 1, 1, 1
@body:
  color: "#d1d5db"
  word-wrap: false
  user-select: text
@footer:
  background: "#111827"
  padding: 0, 1, 0, 1
@detail:
  color: "#64748b"
  user-select: text
```

## User Interface

```ui
+-header-**---------------------------------------------------------------+
| role..**........................................ phase................. |
+-body_panel-**-----------------------------------------------------------+
| body..**............................................................... |
| ....................................................................... |
| ....................................................................... |
| ....................................................................... |
| ....................................................................... |
| ....................................................................... |
+-footer-**---------------------------------------------------------------+
| detail..**............................................................. |
+-------------------------------------------------------------------------+
```
)UI_MCP_MD", "Fixed-height simulated agent chat entry for prose and structured transcript blocks.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"role", "Simulated transcript speaker or activity type.", true}, ui::GeneratedElementMetadata{"phase", "Deterministic scenario phase.", true}, ui::GeneratedElementMetadata{"body", "Selectable simulated transcript content.", true}, ui::GeneratedElementMetadata{"detail", "Simulation-only activity detail.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#00000000"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    role = &static_cast<ui::Label&>(addElement<ui::Label>("role", "Agent"));
    role->setStyle(makeStyle({{"background", "transparent"}, {"color", "#a7f3d0"}}));
    role->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    phase = &static_cast<ui::Label&>(addElement<ui::Label>("phase", "response"));
    phase->setStyle(makeStyle({{"background", "transparent"}, {"color", "#c4b5fd"}, {"text-align", "right"}}));
    phase->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    body = &static_cast<ui::Label&>(addElement<ui::Label>("body", ""));
    body->setStyle(makeStyle({{"color", "#d1d5db"}, {"user-select", "text"}}));
    detail = &static_cast<ui::Label&>(addElement<ui::Label>("detail", "simulated · no external action"));
    detail->setStyle(makeStyle({{"background", "transparent"}, {"color", "#64748b"}, {"user-select", "text"}}));
    detail->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

bool ChatEntryUI::shouldClose() const
{
    return false;
}

bool ChatEntryUI::handleGeneratedButton(std::string_view name)
{
    (void)name;
    return false;
}

bool ChatEntryUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool ChatEntryUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool ChatEntryUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    (void)name;
    (void)value;
    return false;
}

ui::GeneratedWindowRuntimeOptions ChatEntryUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
