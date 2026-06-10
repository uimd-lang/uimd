#include "special_elements_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header_label", .type = "label", .cellName = "", .relative = ui::Rect{1, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{64, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 32, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "info_processing", .type = "label", .cellName = "", .relative = ui::Rect{3, 2, 31, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("auto", 31), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 35, .marginBottom = 30, .elementStyle = []
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }()},
        ui::GeneratedLayoutEntry{.name = "info_loading", .type = "label", .cellName = "", .relative = ui::Rect{3, 35, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 30, .elementStyle = []
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }()},
        ui::GeneratedLayoutEntry{.name = "info_complete", .type = "label", .cellName = "", .relative = ui::Rect{4, 2, 31, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("auto", 31), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 35, .marginBottom = 29, .elementStyle = []
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }()},
        ui::GeneratedLayoutEntry{.name = "info_warning", .type = "label", .cellName = "", .relative = ui::Rect{4, 35, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 29, .elementStyle = []
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }()},
        ui::GeneratedLayoutEntry{.name = "\"Multiline animated text:\"", .type = "label", .cellName = "", .relative = ui::Rect{6, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 40, .marginBottom = 27, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "info_multiline", .type = "label", .cellName = "", .relative = ui::Rect{7, 2, 536870911, 3}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 3), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{64, 3}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 24, .elementStyle = []
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }()},
        ui::GeneratedLayoutEntry{.name = "status_label", .type = "label", .cellName = "", .relative = ui::Rect{11, 2, 31, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("auto", 31), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 35, .marginBottom = 22, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "normal_info", .type = "label", .cellName = "", .relative = ui::Rect{11, 35, 536870911, 2}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 2), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{31, 2}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 21, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "speed_label", .type = "label", .cellName = "", .relative = ui::Rect{12, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{64, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 21, .elementStyle = []
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}});
        style.textColorGradient = makeTextGradient(35, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }()},
        ui::GeneratedLayoutEntry{.name = "\"Raw read-only text:\"", .type = "label", .cellName = "", .relative = ui::Rect{14, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{21, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 45, .marginBottom = 19, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "raw_label", .type = "label", .cellName = "", .relative = ui::Rect{15, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{64, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 18, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "\"Raw button:\"", .type = "label", .cellName = "", .relative = ui::Rect{17, 2, 13, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("auto", 13), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 53, .marginBottom = 16, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "raw_button", .type = "button", .cellName = "", .relative = ui::Rect{17, 16, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 26, .marginBottom = 16, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "\"Raw combo:\"", .type = "label", .cellName = "", .relative = ui::Rect{19, 2, 12, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 54, .marginBottom = 14, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "raw_combo", .type = "combobox", .cellName = "", .relative = ui::Rect{19, 16, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 21, .marginBottom = 14, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "\"Raw list:\"", .type = "label", .cellName = "", .relative = ui::Rect{21, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 55, .marginBottom = 12, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "raw_list", .type = "listbox", .cellName = "", .relative = ui::Rect{22, 2, 536870911, 3}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 3), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{64, 3}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 9, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "\"Text input:\"", .type = "label", .cellName = "", .relative = ui::Rect{26, 2, 13, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("auto", 13), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 53, .marginBottom = 7, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "raw_input", .type = "textinput", .cellName = "", .relative = ui::Rect{26, 16, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{50, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 7, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "\"Text area:\"", .type = "label", .cellName = "", .relative = ui::Rect{28, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 54, .marginBottom = 5, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "raw_area", .type = "textarea", .cellName = "", .relative = ui::Rect{29, 2, 536870911, 4}, .sourceCell = ui::Rect{0, 0, 68, 34}, .width = dimension("expanded", 536870911), .height = dimension("auto", 4), .cellWidth = dimension("auto", 68), .cellHeight = dimension("auto", 34), .charsSize = ui::Size{64, 4}, .cellCharsSize = ui::Size{68, 34}, .marginRight = 2, .marginBottom = 1},
    };
}

}  // namespace

SpecialElementsUI::SpecialElementsUI() : ui::GeneratedWindowBase("Special Elements Demo")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "SpecialElements", R"UI_MCP_MD(# Special Elements Demo

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text."
tags: [example]
```

## Members

```yaml
header_label:
  type: label
  text: "Special UI Elements"
  description: "Window title."

info_processing:
  type: label
  text: "Processing request..."
  color: "#ff0000"
  description: "Animated processing status label."

info_loading:
  type: label
  text: "Loading data from server..."
  color: "#00ff00"
  description: "Animated loading status label."

info_complete:
  type: label
  text: "Operation completed successfully!"
  color: "#0000ff"
  description: "Animated completed status label."

info_warning:
  type: label
  text: "Warning: Low disk space detected"
  color: "#ffffff"
  description: "Animated warning status label."

info_multiline:
  type: label
  text: "Line one of animated text\nLine two with different phase\nLine three final row"
  description: "Multiline animated label with different text rows."

status_label:
  type: label
  text: "Status: Idle"
  description: "Current status text."

normal_info:
  type: label
  text: "Regular static label for comparison"
  description: "Regular static comparison label."

speed_label:
  type: label
  text: "Fast animation speed demo"
  description: "Fast animation speed demonstration label."

raw_label:
  type: label
  text: "Label tab\tcontrol\x1b bell\x07 combining e\u0301 wide\u4e2d"
  user-select: text
  description: "Label containing tab, control, combining, and wide characters."

raw_button:
  type: button
  title: "Button\tESC\x1bWide\u4e2d"
  description: "Button title containing raw terminal-sensitive characters."

raw_combo:
  type: combobox
  options: ["Plain option", "Tab\toption", "Esc\x1boption", "Wide\u4e2doption"]
  selected_item: "Tab\toption"
  description: "ComboBox options containing raw terminal-sensitive characters."

raw_list:
  type: listbox
  options: ["Plain row", "Tab\trow", "Esc\x1brow", "Wide\u4e2drow"]
  selected_items: ["Tab\trow"]
  description: "ListBox options containing raw terminal-sensitive characters."

raw_input:
  type: textinput
  value: "Input\tvalue\x1bwide\u4e2d"
  maxlength: 80
  description: "Text input containing raw terminal-sensitive characters."

raw_area:
  type: textarea
  value: "Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint(\"Hello, \" + name)\n\telse:\n\t\tprint(\"Hello, world!\")\n\nControl\x1bplaceholder\nWide\u4e2dplaceholder"
  maxlength: 240
  description: "Text area containing multiline raw terminal-sensitive characters."
```

## Style

```yaml
include: dark
@info_processing:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@info_loading:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@info_complete:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@info_warning:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@info_multiline:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@speed_label:
  text-color-gradient:
    interval: 35
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
textinput:
  cursor-background: "#facc15"
  cursor-color: "#111827"
textarea:
  cursor-background: "#facc15"
  cursor-color: "#111827"
combobox:
  selected-background: "#2563eb"
  selected-color: "#ffffff"
listbox:
  selected-background: "#2563eb"
  selected-color: "#ffffff"
```

## User Interface

```ui
+--------------------------------------------------------------------+
|                                                                    |
|  header_label....................................................  |
|                                                                    |
|  info_processing................  info_loading...................  |
|  info_complete..................  info_warning...................  |
|                                                                    |
|  "Multiline animated text:"                                         |
|  info_multiline..................................................  |
|  ................................................................  |
|  ................................................................  |
|                                                                    |
|  status_label...................  normal_info....................  |
|  speed_label.....................................................  |
|                                                                    |
|  "Raw read-only text:"                                             |
|  raw_label.......................................................  |
|                                                                    |
|  "Raw button:" raw_button................                          |
|                                                                    |
|  "Raw combo:"  raw_combo......................                     |
|                                                                    |
|  "Raw list:"                                                       |
|  raw_list........................................................  |
|  ................................................................  |
|  ................................................................  |
|                                                                    |
|  "Text input:" raw_input.........................................  |
|                                                                    |
|  "Text area:"                                                      |
|  raw_area........................................................  |
|  ................................................................  |
|  ................................................................  |
|  ................................................................  |
|                                                                    |
+--------------------------------------------------------------------+
```
)UI_MCP_MD", "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header_label", "Window title.", true}, ui::GeneratedElementMetadata{"info_processing", "Animated processing status label.", true}, ui::GeneratedElementMetadata{"info_loading", "Animated loading status label.", true}, ui::GeneratedElementMetadata{"info_complete", "Animated completed status label.", true}, ui::GeneratedElementMetadata{"info_warning", "Animated warning status label.", true}, ui::GeneratedElementMetadata{"info_multiline", "Multiline animated label with different text rows.", true}, ui::GeneratedElementMetadata{"status_label", "Current status text.", true}, ui::GeneratedElementMetadata{"normal_info", "Regular static comparison label.", true}, ui::GeneratedElementMetadata{"speed_label", "Fast animation speed demonstration label.", true}, ui::GeneratedElementMetadata{"raw_label", "Label containing tab, control, combining, and wide characters.", true}, ui::GeneratedElementMetadata{"raw_button", "Button title containing raw terminal-sensitive characters.", true}, ui::GeneratedElementMetadata{"raw_combo", "ComboBox options containing raw terminal-sensitive characters.", true}, ui::GeneratedElementMetadata{"raw_list", "ListBox options containing raw terminal-sensitive characters.", true}, ui::GeneratedElementMetadata{"raw_input", "Text input containing raw terminal-sensitive characters.", true}, ui::GeneratedElementMetadata{"raw_area", "Text area containing multiline raw terminal-sensitive characters.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header_label = &static_cast<ui::Label&>(addElement<ui::Label>("header_label", "Special UI Elements"));
    header_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    header_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    info_processing = &static_cast<ui::Label&>(addElement<ui::Label>("info_processing", "Processing request..."));
    info_processing->setStyle([]
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#ff0000"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }());
    info_processing->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    info_loading = &static_cast<ui::Label&>(addElement<ui::Label>("info_loading", "Loading data from server..."));
    info_loading->setStyle([]
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#00ff00"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }());
    info_loading->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    info_complete = &static_cast<ui::Label&>(addElement<ui::Label>("info_complete", "Operation completed successfully!"));
    info_complete->setStyle([]
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#0000ff"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }());
    info_complete->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    info_warning = &static_cast<ui::Label&>(addElement<ui::Label>("info_warning", "Warning: Low disk space detected"));
    info_warning->setStyle([]
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }());
    info_warning->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    info_multiline = &static_cast<ui::Label&>(addElement<ui::Label>("info_multiline", "Line one of animated text\nLine two with different phase\nLine three final row"));
    info_multiline->setStyle([]
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}});
        style.textColorGradient = makeTextGradient(70, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }());
    info_multiline->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    status_label = &static_cast<ui::Label&>(addElement<ui::Label>("status_label", "Status: Idle"));
    status_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    status_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    normal_info = &static_cast<ui::Label&>(addElement<ui::Label>("normal_info", "Regular static label for comparison"));
    normal_info->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    normal_info->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    speed_label = &static_cast<ui::Label&>(addElement<ui::Label>("speed_label", "Fast animation speed demo"));
    speed_label->setStyle([]
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}});
        style.textColorGradient = makeTextGradient(35, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }());
    speed_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    raw_label = &static_cast<ui::Label&>(addElement<ui::Label>("raw_label", "Label tab	control bell combining é wide中"));
    raw_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    raw_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    raw_button = &static_cast<ui::Button&>(addElement<ui::Button>("raw_button", "Button	ESCWide中"));
    raw_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    raw_button->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    raw_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    raw_combo = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("raw_combo", std::vector<std::string>{"Plain option", "Tab	option", "Escoption", "Wide中option"}));
    raw_combo->setSelectedIndex(1);
    raw_combo->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    raw_combo->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    raw_combo->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#e5e7eb"}}));
    raw_combo->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    raw_combo->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    raw_list = &static_cast<ui::ListBox&>(addElement<ui::ListBox>("raw_list", std::vector<std::string>{"Plain row", "Tab	row", "Escrow", "Wide中row"}));
    raw_list->setSelectedIndex(1);
    raw_list->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#cbd5e1"}}));
    raw_list->setFocusStyle(makeStyle({{"background", "#1d2f4d"}, {"color", "#ffffff"}}));
    raw_list->setEditStyle(makeStyle({{"background", "#243a5c"}, {"color", "#cbd5e1"}}));
    raw_list->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    raw_list->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    raw_input = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("raw_input", "Input	valuewide中", 80));
    raw_input->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    raw_input->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    raw_input->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    raw_input->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    raw_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    raw_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    raw_area = &static_cast<ui::TextArea&>(addElement<ui::TextArea>("raw_area", "Python tabs example:\ndef hello(name):\n	if name:\n		print(\"Hello, \" + name)\n	else:\n		print(\"Hello, world!\")\n\nControlplaceholder\nWide中placeholder", 240));
    raw_area->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    raw_area->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    raw_area->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    raw_area->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    raw_area->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    raw_area->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
}

void SpecialElementsUI::onRawButtonClick()
{
}

void SpecialElementsUI::onRawComboChange(std::string_view value)
{
    (void)value;
}

void SpecialElementsUI::onRawListSelectionChange(const std::vector<std::string>& value)
{
    (void)value;
}

void SpecialElementsUI::onRawInputChange(std::string_view value)
{
    (void)value;
}

void SpecialElementsUI::onRawInputSubmit(std::string_view value)
{
    (void)value;
}

void SpecialElementsUI::onRawAreaChange(std::string_view value)
{
    (void)value;
}

void SpecialElementsUI::onRawAreaSubmit(std::string_view value)
{
    (void)value;
}

bool SpecialElementsUI::shouldClose() const
{
    return false;
}

bool SpecialElementsUI::handleGeneratedButton(std::string_view name)
{
    if (name == "raw_button")
    {
        onRawButtonClick();
        return true;
    }
    return false;
}

bool SpecialElementsUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "raw_input")
    {
        onRawInputChange(value);
        return true;
    }
    else if (name == "raw_area")
    {
        onRawAreaChange(value);
        return true;
    }
    return false;
}

bool SpecialElementsUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "raw_input")
    {
        onRawInputSubmit(value);
        return true;
    }
    else if (name == "raw_area")
    {
        onRawAreaSubmit(value);
        return true;
    }
    return false;
}

bool SpecialElementsUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "raw_combo")
    {
        onRawComboChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    else if (name == "raw_list")
    {
        onRawListSelectionChange(value);
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions SpecialElementsUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "raw_button")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "raw_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "raw_area")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "raw_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "raw_area")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "raw_combo")
        {
            (void)handleGeneratedSelectionChanged(name, value);
        }
        else if (name == "raw_list")
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
