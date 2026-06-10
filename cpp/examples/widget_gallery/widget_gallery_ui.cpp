#include "widget_gallery_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header", .type = "label", .cellName = "title", .relative = ui::Rect{0, 1, 38, 1}, .sourceCell = ui::Rect{0, 0, 54, 1}, .width = dimension("auto", 38), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{38, 1}, .cellCharsSize = ui::Size{54, 1}, .marginRight = 15, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#18324f"}}), .elementStyle = makeStyle({{"background", "#18324f"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "name_label", .type = "label", .cellName = "control_section", .relative = ui::Rect{0, 2, 536870911, 1}, .sourceCell = ui::Rect{12, 0, 27, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{27, 9}, .marginRight = 15, .marginBottom = 8, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "theme_label", .type = "label", .cellName = "section2", .relative = ui::Rect{0, 2, 536870911, 1}, .sourceCell = ui::Rect{12, 28, 26, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 26), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{26, 9}, .marginRight = 13, .marginBottom = 8, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "close_btn", .type = "button", .cellName = "title", .relative = ui::Rect{0, 40, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 54, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{54, 1}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#18324f"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#ffffff"}})},
        ui::GeneratedLayoutEntry{.name = "name_input", .type = "textinput", .cellName = "control_section", .relative = ui::Rect{1, 2, 536870911, 1}, .sourceCell = ui::Rect{12, 0, 27, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{23, 1}, .cellCharsSize = ui::Size{27, 9}, .marginRight = 2, .marginBottom = 7, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "plain_label", .type = "label", .cellName = "label_section", .relative = ui::Rect{1, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{50, 1}, .cellCharsSize = ui::Size{54, 9}, .marginRight = 2, .marginBottom = 7, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "summary", .type = "label", .cellName = "", .relative = ui::Rect{1, 2, 536870911, 6}, .sourceCell = ui::Rect{22, 0, 54, 10}, .width = dimension("expanded", 536870911), .height = dimension("auto", 6), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 10), .charsSize = ui::Size{50, 6}, .cellCharsSize = ui::Size{54, 10}, .marginRight = 2, .marginBottom = 3, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#dbeafe"}})},
        ui::GeneratedLayoutEntry{.name = "theme_combo", .type = "combobox", .cellName = "section2", .relative = ui::Rect{1, 3, 536870911, 1}, .sourceCell = ui::Rect{12, 28, 26, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 26), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{21, 1}, .cellCharsSize = ui::Size{26, 9}, .marginRight = 2, .marginBottom = 7, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "count_label", .type = "label", .cellName = "control_section", .relative = ui::Rect{3, 2, 536870911, 1}, .sourceCell = ui::Rect{12, 0, 27, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{27, 9}, .marginRight = 14, .marginBottom = 5, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "mode_label", .type = "label", .cellName = "section2", .relative = ui::Rect{3, 2, 536870911, 1}, .sourceCell = ui::Rect{12, 28, 26, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 26), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{26, 9}, .marginRight = 14, .marginBottom = 5, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "selectable_label", .type = "label", .cellName = "label_section", .relative = ui::Rect{3, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{50, 1}, .cellCharsSize = ui::Size{54, 9}, .marginRight = 2, .marginBottom = 5, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#a7f3d0"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "count_input", .type = "numberinput", .cellName = "control_section", .relative = ui::Rect{4, 2, 536870911, 1}, .sourceCell = ui::Rect{12, 0, 27, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{23, 1}, .cellCharsSize = ui::Size{27, 9}, .marginRight = 2, .marginBottom = 4, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "mode_list", .type = "listbox", .cellName = "section2", .relative = ui::Rect{4, 2, 536870911, 5}, .sourceCell = ui::Rect{12, 28, 26, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 5), .cellWidth = dimension("auto", 26), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{22, 5}, .cellCharsSize = ui::Size{26, 9}, .marginRight = 2, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "spans", .type = "spanlabel", .cellName = "label_section", .relative = ui::Rect{5, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{50, 1}, .cellCharsSize = ui::Size{54, 9}, .marginRight = 2, .marginBottom = 3, .elementStyle = makeStyle({{"user-select", "text"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "enabled_check", .type = "checkbox", .cellName = "control_section", .relative = ui::Rect{6, 2, 536870911, 1}, .sourceCell = ui::Rect{12, 0, 27, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{23, 1}, .cellCharsSize = ui::Size{27, 9}, .marginRight = 2, .marginBottom = 2, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "animation", .type = "label", .cellName = "label_section", .relative = ui::Rect{7, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{50, 1}, .cellCharsSize = ui::Size{54, 9}, .marginRight = 2, .marginBottom = 1, .elementStyle = []
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#fbbf24"}});
        style.textColorGradient = makeTextGradient(100, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }()},
        ui::GeneratedLayoutEntry{.name = "apply_btn", .type = "button", .cellName = "", .relative = ui::Rect{8, 2, 23, 1}, .sourceCell = ui::Rect{22, 0, 54, 10}, .width = dimension("auto", 23), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 10), .charsSize = ui::Size{23, 1}, .cellCharsSize = ui::Size{54, 10}, .marginRight = 29, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "reset_btn", .type = "button", .cellName = "", .relative = ui::Rect{8, 27, 536870911, 1}, .sourceCell = ui::Rect{22, 0, 54, 10}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 10), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{54, 10}, .marginRight = 2, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

WidgetGalleryUI::WidgetGalleryUI() : ui::GeneratedWindowBase("Widget Gallery")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "WidgetGallery", R"UI_MCP_MD(# Widget Gallery

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons."
tags: [example]
```

## Members

```yaml
header:
  type: label
  text: "Widget Gallery"
  description: "Window title."
  expose: false

close_btn:
  type: button
  title: Quit
  description: "Close the widget gallery."

plain_label:
  type: label
  text: "Plain label with inherited theme colors."
  description: "Plain label using inherited theme colors."

selectable_label:
  type: label
  text: "Select this text with the mouse to test label selection."
  description: "Selectable label used to test mouse text selection."

spans:
  type: spanlabel
  text: "span label"
  description: "Span label demonstrating mixed foreground and background styling."

animation:
  type: label
  text: "Animated label shows gradient-based rendering."
  description: "Animated gradient label."

name_label:
  type: label
  text: Name
  description: "Label for the name input."

name_input:
  type: textinput
  value: "Ada Lovelace"
  maxlength: 80
  description: "Editable name text input."

count_label:
  type: label
  text: Count
  description: "Label for the count input."

count_input:
  type: numberinput
  value: 3
  step_size: 1
  min_value: 0
  max_value: 99
  format_str: 0
  description: "Numeric count input with min, max, and step constraints."

enabled_check:
  type: checkbox
  title: "Feature enabled"
  value: 1
  description: "Toggle for the feature enabled state."

theme_label:
  type: label
  text: Theme
  description: "Label for the theme selector."

theme_combo:
  type: combobox
  options: [Dark, Light, Solarized]
  selected_item: Dark
  description: "Combobox selecting a visual theme value."

mode_label:
  type: label
  text: Mode
  description: "Label for the mode list."

mode_list:
  type: listbox
  options: [Preview, Edit, Review, Publish, Archive]
  selected_items: [Preview]
  multiple: "false"
  description: "Single-select listbox choosing the current workflow mode."

apply_btn:
  type: button
  title: Apply
  description: "Apply the current widget values."

reset_btn:
  type: button
  title: Reset
  description: "Reset widget values to defaults."

summary:
  type: label
  text: "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true"
  description: "Read-only summary of current widget values."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  text-align: center
  background: "#18324f"
  color: "#ffffff"
@close_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
  color: "#ffffff"
@title:
  background: "#18324f"
@selectable_label:
  user-select: text
  color: "#a7f3d0"
@spans:
  user-select: text
  color: "#cbd5e1"
@animation:
  color: "#fbbf24"
  text-color-gradient:
    interval: 100
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
textinput:
  cursor-background: "#facc15"
  cursor-color: "#111827"
numberinput:
  cursor-background: "#facc15"
  cursor-color: "#111827"
checkbox:
  color: "#d1d5db"
  checked-color: "#22c55e"
  unchecked-color: "#f97316"
@summary:
  background: "#0d1524"
  color: "#dbeafe"
```

## User Interface

```ui
+-title------------------------------------------------+
| header................................ close_btn.....|
+-label_section----------------------------------------+
|                                                      |
|  plain_label.......................................  |
|                                                      |
|  selectable_label..................................  |
|                                                      |
|  spans.............................................  |
|                                                      |
|  animation.........................................  |
|                                                      |
+-control_section-----------+--section2----------------+
|  name_label               |  theme_label             |
|  name_input.............  |   theme_combo..........  |
|                           |                          |
|  count_label              |  mode_label              |
|  count_input............  |  mode_list.............  |
|                           |  ......................  |
|  enabled_check..........  |  ......................  |
|                           |  ......................  |
|                           |  ......................  |
+---------------------------+--------------------------+
|                                                      |
|  summary...........................................  |
|  ..................................................  |
|  ..................................................  |
|  ..................................................  |
|  ..................................................  |
|  ..................................................  |
|                                                      |
|  apply_btn..............  reset_btn................  |
|                                                      |
+------------------------------------------------------+
```
)UI_MCP_MD", "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header", "Window title.", false}, ui::GeneratedElementMetadata{"close_btn", "Close the widget gallery.", true}, ui::GeneratedElementMetadata{"plain_label", "Plain label using inherited theme colors.", true}, ui::GeneratedElementMetadata{"selectable_label", "Selectable label used to test mouse text selection.", true}, ui::GeneratedElementMetadata{"spans", "Span label demonstrating mixed foreground and background styling.", true}, ui::GeneratedElementMetadata{"animation", "Animated gradient label.", true}, ui::GeneratedElementMetadata{"name_label", "Label for the name input.", true}, ui::GeneratedElementMetadata{"name_input", "Editable name text input.", true}, ui::GeneratedElementMetadata{"count_label", "Label for the count input.", true}, ui::GeneratedElementMetadata{"count_input", "Numeric count input with min, max, and step constraints.", true}, ui::GeneratedElementMetadata{"enabled_check", "Toggle for the feature enabled state.", true}, ui::GeneratedElementMetadata{"theme_label", "Label for the theme selector.", true}, ui::GeneratedElementMetadata{"theme_combo", "Combobox selecting a visual theme value.", true}, ui::GeneratedElementMetadata{"mode_label", "Label for the mode list.", true}, ui::GeneratedElementMetadata{"mode_list", "Single-select listbox choosing the current workflow mode.", true}, ui::GeneratedElementMetadata{"apply_btn", "Apply the current widget values.", true}, ui::GeneratedElementMetadata{"reset_btn", "Reset widget values to defaults.", true}, ui::GeneratedElementMetadata{"summary", "Read-only summary of current widget values.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header = &static_cast<ui::Label&>(addElement<ui::Label>("header", "Widget Gallery"));
    header->setStyle(makeStyle({{"background", "#18324f"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    close_btn = &static_cast<ui::Button&>(addElement<ui::Button>("close_btn", "Quit"));
    close_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#ffffff"}}));
    close_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    close_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    plain_label = &static_cast<ui::Label&>(addElement<ui::Label>("plain_label", "Plain label with inherited theme colors."));
    plain_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    plain_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    selectable_label = &static_cast<ui::Label&>(addElement<ui::Label>("selectable_label", "Select this text with the mouse to test label selection."));
    selectable_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#a7f3d0"}, {"user-select", "text"}}));
    selectable_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    spans = &static_cast<ui::Label&>(addElement<ui::Label>("spans", "span label"));
    spans->setStyle(makeStyle({{"user-select", "text"}, {"color", "#cbd5e1"}}));
    animation = &static_cast<ui::Label&>(addElement<ui::Label>("animation", "Animated label shows gradient-based rendering."));
    animation->setStyle([]
    {
        ui::Style style = makeStyle({{"background", "transparent"}, {"color", "#fbbf24"}});
        style.textColorGradient = makeTextGradient(100, 1, 1, {"#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"});
        return style;
    }());
    animation->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    name_label = &static_cast<ui::Label&>(addElement<ui::Label>("name_label", "Name"));
    name_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    name_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    name_input = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("name_input", "Ada Lovelace", 80));
    name_input->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    name_input->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    name_input->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    name_input->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    name_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    name_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    count_label = &static_cast<ui::Label&>(addElement<ui::Label>("count_label", "Count"));
    count_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    count_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    count_input = &static_cast<ui::NumberInput&>(addElement<ui::NumberInput>("count_input", 3.0, 1.0));
    count_input->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    count_input->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    count_input->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    count_input->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    count_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    enabled_check = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("enabled_check", "Feature enabled", true));
    enabled_check->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    enabled_check->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#ffffff"}}));
    enabled_check->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#22c55e"}}));
    enabled_check->setUncheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#f97316"}}));
    enabled_check->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    theme_label = &static_cast<ui::Label&>(addElement<ui::Label>("theme_label", "Theme"));
    theme_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    theme_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    theme_combo = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("theme_combo", std::vector<std::string>{"Dark", "Light", "Solarized"}));
    theme_combo->setSelectedIndex(0);
    theme_combo->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    theme_combo->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    theme_combo->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#e5e7eb"}}));
    theme_combo->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    theme_combo->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    mode_label = &static_cast<ui::Label&>(addElement<ui::Label>("mode_label", "Mode"));
    mode_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    mode_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    mode_list = &static_cast<ui::ListBox&>(addElement<ui::ListBox>("mode_list", std::vector<std::string>{"Preview", "Edit", "Review", "Publish", "Archive"}));
    mode_list->setSelectedIndex(0);
    mode_list->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#cbd5e1"}}));
    mode_list->setFocusStyle(makeStyle({{"background", "#1d2f4d"}, {"color", "#ffffff"}}));
    mode_list->setEditStyle(makeStyle({{"background", "#243a5c"}, {"color", "#cbd5e1"}}));
    mode_list->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    mode_list->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    apply_btn = &static_cast<ui::Button&>(addElement<ui::Button>("apply_btn", "Apply"));
    apply_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    apply_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    apply_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    reset_btn = &static_cast<ui::Button&>(addElement<ui::Button>("reset_btn", "Reset"));
    reset_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    reset_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    reset_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    summary = &static_cast<ui::Label&>(addElement<ui::Label>("summary", "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true"));
    summary->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#dbeafe"}}));
    summary->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void WidgetGalleryUI::onCloseBtnClick()
{
}

void WidgetGalleryUI::onNameInputChange(std::string_view value)
{
    (void)value;
}

void WidgetGalleryUI::onNameInputSubmit(std::string_view value)
{
    (void)value;
}

void WidgetGalleryUI::onCountInputChange(std::string_view value)
{
    (void)value;
}

void WidgetGalleryUI::onCountInputSubmit(std::string_view value)
{
    (void)value;
}

void WidgetGalleryUI::onEnabledCheckChange(std::string_view value)
{
    (void)value;
}

void WidgetGalleryUI::onThemeComboChange(std::string_view value)
{
    (void)value;
}

void WidgetGalleryUI::onModeListSelectionChange(const std::vector<std::string>& value)
{
    (void)value;
}

void WidgetGalleryUI::onApplyBtnClick()
{
}

void WidgetGalleryUI::onResetBtnClick()
{
}

bool WidgetGalleryUI::shouldClose() const
{
    return false;
}

bool WidgetGalleryUI::handleGeneratedButton(std::string_view name)
{
    if (name == "close_btn")
    {
        onCloseBtnClick();
        return true;
    }
    else if (name == "apply_btn")
    {
        onApplyBtnClick();
        return true;
    }
    else if (name == "reset_btn")
    {
        onResetBtnClick();
        return true;
    }
    return false;
}

bool WidgetGalleryUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "name_input")
    {
        onNameInputChange(value);
        return true;
    }
    else if (name == "count_input")
    {
        onCountInputChange(value);
        return true;
    }
    else if (name == "enabled_check")
    {
        onEnabledCheckChange(value);
        return true;
    }
    return false;
}

bool WidgetGalleryUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "name_input")
    {
        onNameInputSubmit(value);
        return true;
    }
    else if (name == "count_input")
    {
        onCountInputSubmit(value);
        return true;
    }
    return false;
}

bool WidgetGalleryUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "theme_combo")
    {
        onThemeComboChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    else if (name == "mode_list")
    {
        onModeListSelectionChange(value);
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions WidgetGalleryUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "close_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "apply_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "reset_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "name_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "count_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "enabled_check")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "name_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "count_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "theme_combo")
        {
            (void)handleGeneratedSelectionChanged(name, value);
        }
        else if (name == "mode_list")
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
