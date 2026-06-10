#include "image_list_view_ui.hpp"
#include "image_list_scroll_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "items", .type = "image_list_scroll", .cellName = "items_cell", .relative = ui::Rect{0, 0, 536870911, 536870911}, .sourceCell = ui::Rect{2, 0, 71, 11}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{71, 11}, .cellCharsSize = ui::Size{71, 11}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#030712"}})},
        ui::GeneratedLayoutEntry{.name = "add_btn", .type = "button", .cellName = "toolbar", .relative = ui::Rect{0, 1, 19, 1}, .sourceCell = ui::Rect{0, 0, 71, 1}, .width = dimension("auto", 19), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{19, 1}, .cellCharsSize = ui::Size{71, 1}, .marginRight = 51, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#030712"}}), .elementStyle = makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

ImageListViewUI::ImageListViewUI() : ui::GeneratedWindowBase("Image List View")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("control");
    setMcpMetadata(true, "ImageListView", R"UI_MCP_MD(# Image List View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "ViewHost page with add button and scrollable image item list."
tags: [example, image, list]
```

## Definition

```yaml
uses:
  - image_list_scroll
```

## Members

```yaml
add_btn:
  type: button
  title: Add
  description: "Add an image item."

items:
  type: image_list_scroll
  description: "Scrollable list of image items."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@toolbar:
  background: "#030712"
@items_cell:
  background: "#030712"
button:
  background: "#14532d"
  color: "#e2e8f0"
  focus-background: "#16a34a"
```

## User Interface

```ui
+-toolbar-**------------------------------------------------------------+
| add_btn............                                                   |
+-items_cell-**---------------------------------------------------------+
|items..**..............................................................|
*.......................................................................|
**......................................................................|
|*......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
+-----------------------------------------------------------------------+
```
)UI_MCP_MD", "ViewHost page with add button and scrollable image item list.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"add_btn", "Add an image item.", true}, ui::GeneratedElementMetadata{"items", "Scrollable list of image items.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#030712"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    add_btn = &static_cast<ui::Button&>(addElement<ui::Button>("add_btn", "Add"));
    add_btn->setStyle(makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}}));
    add_btn->setFocusStyle(makeStyle({{"background", "#16a34a"}, {"color", "#ffffff"}}));
    add_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    items = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("items"));
    items->setChild(std::make_unique<ImageListScrollUI>());
    items->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    items->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    items->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
}

void ImageListViewUI::onAddBtnClick()
{
}

bool ImageListViewUI::shouldClose() const
{
    return false;
}

bool ImageListViewUI::handleGeneratedButton(std::string_view name)
{
    if (name == "add_btn")
    {
        onAddBtnClick();
        return true;
    }
    return false;
}

bool ImageListViewUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageListViewUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageListViewUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions ImageListViewUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "add_btn")
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
