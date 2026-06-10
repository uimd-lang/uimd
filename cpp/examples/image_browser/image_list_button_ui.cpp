#include "image_list_button_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "prw_1", .type = "image", .cellName = "view", .relative = ui::Rect{0, 1, 7, 3}, .sourceCell = ui::Rect{0, 0, 25, 3}, .width = dimension("auto", 7), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{7, 3}, .cellCharsSize = ui::Size{25, 3}, .marginRight = 17, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 0, 1, 0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "prw_2", .type = "image", .cellName = "view", .relative = ui::Rect{0, 9, 7, 3}, .sourceCell = ui::Rect{0, 0, 25, 3}, .width = dimension("auto", 7), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{7, 3}, .cellCharsSize = ui::Size{25, 3}, .marginRight = 9, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 0, 1, 0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "prw_3", .type = "image", .cellName = "view", .relative = ui::Rect{0, 17, 7, 3}, .sourceCell = ui::Rect{0, 0, 25, 3}, .width = dimension("auto", 7), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{7, 3}, .cellCharsSize = ui::Size{25, 3}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 0, 1, 0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
    };
}

}  // namespace

ImageListButtonUI::ImageListButtonUI() : ui::GeneratedWindowBase("Image List Button")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(true);
    setGeneratedKind("control");
    setMcpMetadata(true, "ImageListButton", R"UI_MCP_MD(# Image List Button

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "Clickable image-list preview tile with three thumbnails."
tags: [example, image, button, list]
```

## Definition

```yaml
extends: uicontrol
focusable: true
```

## Members

```yaml
prw_1:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "Preview image 1"
  fit: cover
  render_mode: auto
  description: "List preview slot 1. Click to open image items."

prw_2:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "Preview image 2"
  fit: cover
  render_mode: auto
  description: "List preview slot 2. Click to open image items."

prw_3:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "Preview image 3"
  fit: cover
  render_mode: auto
  description: "List preview slot 3. Click to open image items."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#172033"
cell:
  padding: 1, 0, 1, 0
  background: "#172033"
image:
  background: transparent
  color: "#d1d5db"
  focus-background: "#25364f"
```

## User Interface

```ui
+-view-**-----------------+
| prw_1.. prw_2.. prw_3.. |
| ....... ....... ....... |
| ....... ....... ....... |
+-------------------------+
```
)UI_MCP_MD", "Clickable image-list preview tile with three thumbnails.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"prw_1", "List preview slot 1. Click to open image items.", true}, ui::GeneratedElementMetadata{"prw_2", "List preview slot 2. Click to open image items.", true}, ui::GeneratedElementMetadata{"prw_3", "List preview slot 3. Click to open image items.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    ui::requireSixelForImageRendering();
    setGeneratedWindowStyle(makeStyle({{"background", "#172033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    prw_1 = &static_cast<ui::Image&>(addElement<ui::Image>("prw_1", "shared/assets/image_samples/camera.png", "Preview image 1", "cover", "auto", "center", "middle"));
    prw_1->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    prw_1->setFocusStyle(makeStyle({{"background", "#25364f"}}));
    prw_2 = &static_cast<ui::Image&>(addElement<ui::Image>("prw_2", "shared/assets/image_samples/astronaut.png", "Preview image 2", "cover", "auto", "center", "middle"));
    prw_2->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    prw_2->setFocusStyle(makeStyle({{"background", "#25364f"}}));
    prw_3 = &static_cast<ui::Image&>(addElement<ui::Image>("prw_3", "shared/assets/image_samples/coffee.png", "Preview image 3", "cover", "auto", "center", "middle"));
    prw_3->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    prw_3->setFocusStyle(makeStyle({{"background", "#25364f"}}));
}

void ImageListButtonUI::onPrw1Click()
{
}

void ImageListButtonUI::onPrw2Click()
{
}

void ImageListButtonUI::onPrw3Click()
{
}

bool ImageListButtonUI::shouldClose() const
{
    return false;
}

bool ImageListButtonUI::handleGeneratedButton(std::string_view name)
{
    if (name == "prw_1")
    {
        onPrw1Click();
        return true;
    }
    else if (name == "prw_2")
    {
        onPrw2Click();
        return true;
    }
    else if (name == "prw_3")
    {
        onPrw3Click();
        return true;
    }
    return false;
}

bool ImageListButtonUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageListButtonUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageListButtonUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions ImageListButtonUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "prw_1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "prw_2")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "prw_3")
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
