#include "message_box_ok_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "dialog_header", .type = "label", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 44, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 44), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{44, 1}, .cellCharsSize = ui::Size{44, 1}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#1d314c"}, {"color", "#cbd5e1"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "message", .type = "label", .cellName = "", .relative = ui::Rect{1, 2, 536870911, 2}, .sourceCell = ui::Rect{2, 0, 44, 6}, .width = dimension("expanded", 536870911), .height = dimension("auto", 2), .cellWidth = dimension("auto", 44), .cellHeight = dimension("auto", 6), .charsSize = ui::Size{40, 2}, .cellCharsSize = ui::Size{44, 6}, .marginRight = 2, .marginBottom = 3, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "ok_btn", .type = "button", .cellName = "", .relative = ui::Rect{4, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 44, 6}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 44), .cellHeight = dimension("auto", 6), .charsSize = ui::Size{40, 1}, .cellCharsSize = ui::Size{44, 6}, .marginRight = 2, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

MessageBoxOkUI::MessageBoxOkUI() : ui::GeneratedWindowBase("Message Box")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("dialog");
    setMcpMetadata(true, "MessageBoxOk", R"UI_MCP_MD(# Message Box

## Metadata

```yaml
format: uimd
format-version: 1
kind: dialog
status: stable
description: "Message Box UI source."
tags: [dialog]
```

## Members

```yaml
dialog_header:
  type: label
  text: Message

message:
  type: label
  text: ""

ok_btn:
  type: button
  title: OK
```

## Style

```yaml
include: dark
this:
  border-width: 0
```

## User Interface

```ui
+--------------------------------------------+
|dialog_header...............................|
+--------------------------------------------+
|                                            |
|  message.................................  |
|  ........................................  |
|                                            |
|  ok_btn..................................  |
|                                            |
+--------------------------------------------+
```
)UI_MCP_MD", "Message Box UI source.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"dialog_header", "Message", true}, ui::GeneratedElementMetadata{"message", "message", true}, ui::GeneratedElementMetadata{"ok_btn", "OK", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    dialog_header = &static_cast<ui::Label&>(addElement<ui::Label>("dialog_header", "Message"));
    dialog_header->setStyle(makeStyle({{"background", "#1d314c"}, {"color", "#cbd5e1"}, {"text-align", "center"}}));
    dialog_header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    message = &static_cast<ui::Label&>(addElement<ui::Label>("message", ""));
    message->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    message->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    ok_btn = &static_cast<ui::Button&>(addElement<ui::Button>("ok_btn", "OK"));
    ok_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    ok_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    ok_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void MessageBoxOkUI::onOkBtnClick()
{
}

bool MessageBoxOkUI::shouldClose() const
{
    return false;
}

bool MessageBoxOkUI::handleGeneratedButton(std::string_view name)
{
    if (name == "ok_btn")
    {
        onOkBtnClick();
        return true;
    }
    return false;
}

bool MessageBoxOkUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool MessageBoxOkUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool MessageBoxOkUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions MessageBoxOkUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "ok_btn")
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
