#include "edit_field_row_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "field_input", .type = "textinput", .cellName = "row", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 90, 3}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{88, 1}, .cellCharsSize = ui::Size{90, 3}, .marginRight = 1, .marginBottom = 2, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "field_btn", .type = "button", .cellName = "row", .relative = ui::Rect{2, 1, 19, 1}, .sourceCell = ui::Rect{0, 0, 90, 3}, .width = dimension("auto", 19), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{19, 1}, .cellCharsSize = ui::Size{90, 3}, .marginRight = 70, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

EditFieldRowUI::EditFieldRowUI() : ui::GeneratedWindowBase("Edit Field Row")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "EditFieldRow", R"UI_MCP_MD(# Edit Field Row

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: regression
description: "One editable field row with an action button inside the edit ScrollView."
tags: [regression, issue-5, scrollview]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
field_input:
  type: textinput
  value: ""
  maxlength: 120
  description: "Editable regression field."

field_btn:
  type: button
  title: Action
  description: "Action button beside the editable field."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@row:
  background: "#111827"
  padding: 1, 1, 1, 1
textinput:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  cursor-background: "#facc15"
  cursor-color: "#111827"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@field_btn:
  background: "#14532d"
  focus-background: "#16a34a"
```

## User Interface

```ui
+-row-**-----------------------------------------------------------------------------------+
| field_input..**......................................................................... |
|                                                                                          |
| field_btn.......... .................................................................... |
+------------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "One editable field row with an action button inside the edit ScrollView.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"field_input", "Editable regression field.", true}, ui::GeneratedElementMetadata{"field_btn", "Action button beside the editable field.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    field_input = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("field_input", "", 120));
    field_input->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    field_input->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    field_input->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    field_input->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    field_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    field_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    field_btn = &static_cast<ui::Button&>(addElement<ui::Button>("field_btn", "Action"));
    field_btn->setStyle(makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}}));
    field_btn->setFocusStyle(makeStyle({{"background", "#16a34a"}, {"color", "#ffffff"}}));
    field_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void EditFieldRowUI::onFieldInputChange(std::string_view value)
{
    (void)value;
}

void EditFieldRowUI::onFieldInputSubmit(std::string_view value)
{
    (void)value;
}

void EditFieldRowUI::onFieldBtnClick()
{
}

bool EditFieldRowUI::shouldClose() const
{
    return false;
}

bool EditFieldRowUI::handleGeneratedButton(std::string_view name)
{
    (void)name;
    if (name == "field_btn")
    {
        onFieldBtnClick();
        return true;
    }
    return false;
}

bool EditFieldRowUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    if (name == "field_input")
    {
        onFieldInputChange(value);
        return true;
    }
    return false;
}

bool EditFieldRowUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    if (name == "field_input")
    {
        onFieldInputSubmit(value);
        return true;
    }
    return false;
}

bool EditFieldRowUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    (void)name;
    (void)value;
    return false;
}

ui::GeneratedWindowRuntimeOptions EditFieldRowUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "field_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "field_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "field_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
