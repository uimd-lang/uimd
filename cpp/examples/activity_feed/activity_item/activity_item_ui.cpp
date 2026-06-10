#include "activity_item_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "", .type = "", .cellName = "marker", .relative = ui::Rect{0, 0, 1, 536870910}, .sourceCell = ui::Rect{0, 0, 10, 6}, .width = dimension("auto", 1), .height = dimension("auto", 536870910), .cellWidth = dimension("fixed", 1), .cellHeight = dimension("fit-content", 536870910), .charsSize = ui::Size{10, 6}, .cellCharsSize = ui::Size{10, 6}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background-texture", "▐"}, {"background-texture-color", "#38bdf8"}})},
        ui::GeneratedLayoutEntry{.name = "timestamp", .type = "label", .cellName = "body", .relative = ui::Rect{0, 1, 11, 1}, .sourceCell = ui::Rect{0, 11, 42, 6}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("fit-content", 536870910), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{42, 6}, .marginRight = 30, .marginBottom = 5, .cellStyle = makeStyle({{"background", "#1f2e46"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#94a3b8"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "event_type", .type = "label", .cellName = "body", .relative = ui::Rect{0, 13, 536870911, 1}, .sourceCell = ui::Rect{0, 11, 42, 6}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("fit-content", 536870910), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{42, 6}, .marginRight = 15, .marginBottom = 5, .cellStyle = makeStyle({{"background", "#1f2e46"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "message", .type = "label", .cellName = "body", .relative = ui::Rect{1, 1, 536870911, 536870910}, .sourceCell = ui::Rect{0, 11, 42, 6}, .width = dimension("expanded", 536870911), .height = dimension("fit-content", 536870910), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("fit-content", 536870910), .charsSize = ui::Size{11, 5}, .cellCharsSize = ui::Size{42, 6}, .marginRight = 30, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2e46"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"user-select", "text"}})},
    };
}

}  // namespace

ActivityItemUI::ActivityItemUI() : ui::GeneratedWindowBase("Activity Item")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "ActivityItem", R"UI_MCP_MD(# Activity Item

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Reusable activity feed item with timestamp, event type, and message text."
tags: [component]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
timestamp:
  type: label
  text: "10:42"
  description: "Optional activity timestamp."

event_type:
  type: label
  text: Info
  description: "Activity category label."

message:
  type: label
  text: "Activity message"
  description: "Selectable activity message body."
```

## Style

```yaml
this:
  border-width: 0
@marker:
  background-texture: "▐"
  background-texture-color: "#38bdf8"
@marker_info:
  background-texture-color: "#38bdf8"
@marker_task:
  background-texture-color: "#a78bfa"
@marker_warning:
  background-texture-color: "#facc15"
@marker_deploy:
  background-texture-color: "#22c55e"
@marker_note:
  background-texture-color: "#fb7185"
@body:
  background: "#1f2e46"
@timestamp:
  color: "#94a3b8"
  user-select: text
@event_type:
  color: "#bfdbfe"
  user-select: text
@message:
  color: "#ffffff"
  user-select: text
```

## User Interface

```ui
+-marker-1-+-body-**----------------------------------+
|          | timestamp.. event_type..**               |
|          | message..**                              |
|          | ........................................ |
|          | ........................................ |
#          # #                                        |
#          # #                                        |
+----------+------------------------------------------+
```
)UI_MCP_MD", "Reusable activity feed item with timestamp, event type, and message text.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"timestamp", "Optional activity timestamp.", true}, ui::GeneratedElementMetadata{"event_type", "Activity category label.", true}, ui::GeneratedElementMetadata{"message", "Selectable activity message body.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    timestamp = &static_cast<ui::Label&>(addElement<ui::Label>("timestamp", "10:42"));
    timestamp->setStyle(makeStyle({{"background", "transparent"}, {"color", "#94a3b8"}, {"user-select", "text"}}));
    timestamp->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    event_type = &static_cast<ui::Label&>(addElement<ui::Label>("event_type", "Info"));
    event_type->setStyle(makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}, {"user-select", "text"}}));
    event_type->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    message = &static_cast<ui::Label&>(addElement<ui::Label>("message", "Activity message"));
    message->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"user-select", "text"}}));
    message->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

bool ActivityItemUI::shouldClose() const
{
    return false;
}

bool ActivityItemUI::handleGeneratedButton(std::string_view name)
{
    return false;
}

bool ActivityItemUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool ActivityItemUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ActivityItemUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions ActivityItemUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
