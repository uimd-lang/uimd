#include "calculator_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "display", .type = "label", .cellName = "", .relative = ui::Rect{1, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{18, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 2, .marginBottom = 11, .elementStyle = makeStyle({{"background", "#32446d"}, {"color", "#c8e0ff"}, {"text-align", "right"}})},
        ui::GeneratedLayoutEntry{.name = "ac", .type = "button", .cellName = "", .relative = ui::Rect{3, 2, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 17, .marginBottom = 9, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "dl", .type = "button", .cellName = "", .relative = ui::Rect{3, 7, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 12, .marginBottom = 9, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "pr", .type = "button", .cellName = "", .relative = ui::Rect{3, 12, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 7, .marginBottom = 9, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "dv", .type = "button", .cellName = "", .relative = ui::Rect{3, 17, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 2, .marginBottom = 9, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n7", .type = "button", .cellName = "", .relative = ui::Rect{5, 2, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 17, .marginBottom = 7, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n8", .type = "button", .cellName = "", .relative = ui::Rect{5, 7, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 12, .marginBottom = 7, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n9", .type = "button", .cellName = "", .relative = ui::Rect{5, 12, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 7, .marginBottom = 7, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "ml", .type = "button", .cellName = "", .relative = ui::Rect{5, 17, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 2, .marginBottom = 7, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n4", .type = "button", .cellName = "", .relative = ui::Rect{7, 2, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 17, .marginBottom = 5, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n5", .type = "button", .cellName = "", .relative = ui::Rect{7, 7, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 12, .marginBottom = 5, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n6", .type = "button", .cellName = "", .relative = ui::Rect{7, 12, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 7, .marginBottom = 5, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "mn", .type = "button", .cellName = "", .relative = ui::Rect{7, 17, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 2, .marginBottom = 5, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n1", .type = "button", .cellName = "", .relative = ui::Rect{9, 2, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 17, .marginBottom = 3, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n2", .type = "button", .cellName = "", .relative = ui::Rect{9, 7, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 12, .marginBottom = 3, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n3", .type = "button", .cellName = "", .relative = ui::Rect{9, 12, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 7, .marginBottom = 3, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "sb", .type = "button", .cellName = "", .relative = ui::Rect{9, 17, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 2, .marginBottom = 3, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "n0", .type = "button", .cellName = "", .relative = ui::Rect{11, 2, 8, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 8), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{8, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 12, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "dt", .type = "button", .cellName = "", .relative = ui::Rect{11, 12, 3, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("auto", 3), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 7, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "ad", .type = "button", .cellName = "", .relative = ui::Rect{11, 17, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 22, 13}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 13), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{22, 13}, .marginRight = 2, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

CalculatorUI::CalculatorUI() : ui::GeneratedWindowBase("Calculator")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "Calculator", R"UI_MCP_MD(# Calculator

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression."
tags: [example]
```

## Members

```yaml
display:
  type: label
  text: ""
  description: "Shows the current number, operator result, or calculation state."

n0:
  type: button
  title: 0
  description: "Enter digit 0."

n1:
  type: button
  title: 1
  description: "Enter digit 1."

n2:
  type: button
  title: 2
  description: "Enter digit 2."

n3:
  type: button
  title: 3
  description: "Enter digit 3."

n4:
  type: button
  title: 4
  description: "Enter digit 4."

n5:
  type: button
  title: 5
  description: "Enter digit 5."

n6:
  type: button
  title: 6
  description: "Enter digit 6."

n7:
  type: button
  title: 7
  description: "Enter digit 7."

n8:
  type: button
  title: 8
  description: "Enter digit 8."

n9:
  type: button
  title: 9
  description: "Enter digit 9."

sb:
  type: button
  title: "="
  description: "Evaluate the pending calculation."

dt:
  type: button
  title: .
  description: "Insert a decimal point into the current number."

ac:
  type: button
  title: AC
  description: "Clear the current calculator state."

dl:
  type: button
  title: DEL
  description: "Delete the last entered character."

pr:
  type: button
  title: "%"
  description: "Convert or apply percentage behavior for the current value."

dv:
  type: button
  title: /
  description: "Choose division as the pending operator."

ml:
  type: button
  title: "*"
  description: "Choose multiplication as the pending operator."

mn:
  type: button
  title: -
  description: "Choose subtraction as the pending operator."

ad:
  type: button
  title: "+"
  description: "Choose addition as the pending operator."
```

## Style

```yaml
@display:
  background: #32446d
  color: #c8e0ff
  text-align: right
```

## User Interface

```ui
+----------------------+
|                      |
|  display...........  |
|                      |
|  ac.  dl.  pr.  dv.  |
|                      |
|  n7.  n8.  n9.  ml.  |
|                      |
|  n4.  n5.  n6.  mn.  |
|                      |
|  n1.  n2.  n3.  sb.  |
|                      |
|  n0......  dt.  ad.  |
|                      |
+----------------------+
```
)UI_MCP_MD", "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"display", "Shows the current number, operator result, or calculation state.", true}, ui::GeneratedElementMetadata{"n0", "Enter digit 0.", true}, ui::GeneratedElementMetadata{"n1", "Enter digit 1.", true}, ui::GeneratedElementMetadata{"n2", "Enter digit 2.", true}, ui::GeneratedElementMetadata{"n3", "Enter digit 3.", true}, ui::GeneratedElementMetadata{"n4", "Enter digit 4.", true}, ui::GeneratedElementMetadata{"n5", "Enter digit 5.", true}, ui::GeneratedElementMetadata{"n6", "Enter digit 6.", true}, ui::GeneratedElementMetadata{"n7", "Enter digit 7.", true}, ui::GeneratedElementMetadata{"n8", "Enter digit 8.", true}, ui::GeneratedElementMetadata{"n9", "Enter digit 9.", true}, ui::GeneratedElementMetadata{"sb", "Evaluate the pending calculation.", true}, ui::GeneratedElementMetadata{"dt", "Insert a decimal point into the current number.", true}, ui::GeneratedElementMetadata{"ac", "Clear the current calculator state.", true}, ui::GeneratedElementMetadata{"dl", "Delete the last entered character.", true}, ui::GeneratedElementMetadata{"pr", "Convert or apply percentage behavior for the current value.", true}, ui::GeneratedElementMetadata{"dv", "Choose division as the pending operator.", true}, ui::GeneratedElementMetadata{"ml", "Choose multiplication as the pending operator.", true}, ui::GeneratedElementMetadata{"mn", "Choose subtraction as the pending operator.", true}, ui::GeneratedElementMetadata{"ad", "Choose addition as the pending operator.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    display = &static_cast<ui::Label&>(addElement<ui::Label>("display", ""));
    display->setStyle(makeStyle({{"background", "#32446d"}, {"color", "#c8e0ff"}, {"text-align", "right"}}));
    display->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    n0 = &static_cast<ui::Button&>(addElement<ui::Button>("n0", "0"));
    n0->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n0->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n0->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n1 = &static_cast<ui::Button&>(addElement<ui::Button>("n1", "1"));
    n1->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n1->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n1->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n2 = &static_cast<ui::Button&>(addElement<ui::Button>("n2", "2"));
    n2->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n2->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n2->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n3 = &static_cast<ui::Button&>(addElement<ui::Button>("n3", "3"));
    n3->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n3->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n3->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n4 = &static_cast<ui::Button&>(addElement<ui::Button>("n4", "4"));
    n4->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n4->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n4->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n5 = &static_cast<ui::Button&>(addElement<ui::Button>("n5", "5"));
    n5->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n5->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n5->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n6 = &static_cast<ui::Button&>(addElement<ui::Button>("n6", "6"));
    n6->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n6->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n6->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n7 = &static_cast<ui::Button&>(addElement<ui::Button>("n7", "7"));
    n7->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n7->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n7->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n8 = &static_cast<ui::Button&>(addElement<ui::Button>("n8", "8"));
    n8->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n8->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n8->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    n9 = &static_cast<ui::Button&>(addElement<ui::Button>("n9", "9"));
    n9->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    n9->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    n9->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    sb = &static_cast<ui::Button&>(addElement<ui::Button>("sb", "="));
    sb->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    sb->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    sb->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    dt = &static_cast<ui::Button&>(addElement<ui::Button>("dt", "."));
    dt->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    dt->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    dt->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    ac = &static_cast<ui::Button&>(addElement<ui::Button>("ac", "AC"));
    ac->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    ac->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    ac->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    dl = &static_cast<ui::Button&>(addElement<ui::Button>("dl", "DEL"));
    dl->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    dl->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    dl->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    pr = &static_cast<ui::Button&>(addElement<ui::Button>("pr", "%"));
    pr->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    pr->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    pr->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    dv = &static_cast<ui::Button&>(addElement<ui::Button>("dv", "/"));
    dv->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    dv->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    dv->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    ml = &static_cast<ui::Button&>(addElement<ui::Button>("ml", "*"));
    ml->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    ml->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    ml->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    mn = &static_cast<ui::Button&>(addElement<ui::Button>("mn", "-"));
    mn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    mn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    mn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    ad = &static_cast<ui::Button&>(addElement<ui::Button>("ad", "+"));
    ad->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    ad->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    ad->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void CalculatorUI::onN0Click()
{
}

void CalculatorUI::onN1Click()
{
}

void CalculatorUI::onN2Click()
{
}

void CalculatorUI::onN3Click()
{
}

void CalculatorUI::onN4Click()
{
}

void CalculatorUI::onN5Click()
{
}

void CalculatorUI::onN6Click()
{
}

void CalculatorUI::onN7Click()
{
}

void CalculatorUI::onN8Click()
{
}

void CalculatorUI::onN9Click()
{
}

void CalculatorUI::onSbClick()
{
}

void CalculatorUI::onDtClick()
{
}

void CalculatorUI::onAcClick()
{
}

void CalculatorUI::onDlClick()
{
}

void CalculatorUI::onPrClick()
{
}

void CalculatorUI::onDvClick()
{
}

void CalculatorUI::onMlClick()
{
}

void CalculatorUI::onMnClick()
{
}

void CalculatorUI::onAdClick()
{
}

bool CalculatorUI::shouldClose() const
{
    return false;
}

bool CalculatorUI::handleGeneratedButton(std::string_view name)
{
    if (name == "n0")
    {
        onN0Click();
        return true;
    }
    else if (name == "n1")
    {
        onN1Click();
        return true;
    }
    else if (name == "n2")
    {
        onN2Click();
        return true;
    }
    else if (name == "n3")
    {
        onN3Click();
        return true;
    }
    else if (name == "n4")
    {
        onN4Click();
        return true;
    }
    else if (name == "n5")
    {
        onN5Click();
        return true;
    }
    else if (name == "n6")
    {
        onN6Click();
        return true;
    }
    else if (name == "n7")
    {
        onN7Click();
        return true;
    }
    else if (name == "n8")
    {
        onN8Click();
        return true;
    }
    else if (name == "n9")
    {
        onN9Click();
        return true;
    }
    else if (name == "sb")
    {
        onSbClick();
        return true;
    }
    else if (name == "dt")
    {
        onDtClick();
        return true;
    }
    else if (name == "ac")
    {
        onAcClick();
        return true;
    }
    else if (name == "dl")
    {
        onDlClick();
        return true;
    }
    else if (name == "pr")
    {
        onPrClick();
        return true;
    }
    else if (name == "dv")
    {
        onDvClick();
        return true;
    }
    else if (name == "ml")
    {
        onMlClick();
        return true;
    }
    else if (name == "mn")
    {
        onMnClick();
        return true;
    }
    else if (name == "ad")
    {
        onAdClick();
        return true;
    }
    return false;
}

bool CalculatorUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool CalculatorUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool CalculatorUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions CalculatorUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "n0")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n2")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n3")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n4")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n5")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n6")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n7")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n8")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "n9")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "sb")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "dt")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "ac")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "dl")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "pr")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "dv")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "ml")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "mn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "ad")
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
