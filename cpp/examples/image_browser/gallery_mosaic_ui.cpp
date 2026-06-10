#include "gallery_mosaic_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "gal_1", .type = "image", .cellName = "view", .relative = ui::Rect{0, 1, 7, 3}, .sourceCell = ui::Rect{0, 0, 26, 7}, .width = dimension("auto", 7), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{7, 3}, .cellCharsSize = ui::Size{26, 7}, .marginRight = 18, .marginBottom = 4, .cellStyle = makeStyle({{"padding", "1, 0, 1, 0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "gal_2", .type = "image", .cellName = "view", .relative = ui::Rect{0, 9, 7, 3}, .sourceCell = ui::Rect{0, 0, 26, 7}, .width = dimension("auto", 7), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{7, 3}, .cellCharsSize = ui::Size{26, 7}, .marginRight = 10, .marginBottom = 4, .cellStyle = makeStyle({{"padding", "1, 0, 1, 0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "gal_3", .type = "image", .cellName = "view", .relative = ui::Rect{0, 17, 8, 3}, .sourceCell = ui::Rect{0, 0, 26, 7}, .width = dimension("auto", 8), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{8, 3}, .cellCharsSize = ui::Size{26, 7}, .marginRight = 1, .marginBottom = 4, .cellStyle = makeStyle({{"padding", "1, 0, 1, 0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "gal_4", .type = "image", .cellName = "view", .relative = ui::Rect{4, 1, 7, 3}, .sourceCell = ui::Rect{0, 0, 26, 7}, .width = dimension("auto", 7), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{7, 3}, .cellCharsSize = ui::Size{26, 7}, .marginRight = 18, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 0, 1, 0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "gal_5", .type = "image", .cellName = "view", .relative = ui::Rect{4, 9, 7, 3}, .sourceCell = ui::Rect{0, 0, 26, 7}, .width = dimension("auto", 7), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{7, 3}, .cellCharsSize = ui::Size{26, 7}, .marginRight = 10, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 0, 1, 0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
    };
}

}  // namespace

GalleryMosaicUI::GalleryMosaicUI() : ui::GeneratedWindowBase("Gallery Mosaic")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(true);
    setGeneratedKind("control");
    setMcpMetadata(true, "GalleryMosaic", R"UI_MCP_MD(# Gallery Mosaic

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "3+2 image mosaic that opens the scrollable gallery when clicked."
tags: [example, image, gallery]
```

## Definition

```yaml
extends: uicontrol
focusable: true
```

## Members

```yaml
gal_1:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "Camera"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 1. Click to open gallery."

gal_2:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "Astronaut"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 2. Click to open gallery."

gal_3:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "Coffee"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 3. Click to open gallery."

gal_4:
  type: image
  source: "shared/assets/image_samples/coins.png"
  alt: "Coins"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 4. Click to open gallery."

gal_5:
  type: image
  source: "shared/assets/image_samples/chelsea.png"
  alt: "Chelsea"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 5. Click to open gallery."
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
+-view-**------------------+
| gal_1.. gal_2.. gal_3... |
| ....... ....... ........ |
| ....... ....... ........ |
|                          |
| gal_4.. gal_5..          |
| ....... .......          |
| ....... .......          |
+--------------------------+
```
)UI_MCP_MD", "3+2 image mosaic that opens the scrollable gallery when clicked.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"gal_1", "Gallery mosaic slot 1. Click to open gallery.", true}, ui::GeneratedElementMetadata{"gal_2", "Gallery mosaic slot 2. Click to open gallery.", true}, ui::GeneratedElementMetadata{"gal_3", "Gallery mosaic slot 3. Click to open gallery.", true}, ui::GeneratedElementMetadata{"gal_4", "Gallery mosaic slot 4. Click to open gallery.", true}, ui::GeneratedElementMetadata{"gal_5", "Gallery mosaic slot 5. Click to open gallery.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    ui::requireSixelForImageRendering();
    setGeneratedWindowStyle(makeStyle({{"background", "#172033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    gal_1 = &static_cast<ui::Image&>(addElement<ui::Image>("gal_1", "shared/assets/image_samples/camera.png", "Camera", "cover", "auto", "center", "middle"));
    gal_1->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    gal_1->setFocusStyle(makeStyle({{"background", "#25364f"}}));
    gal_2 = &static_cast<ui::Image&>(addElement<ui::Image>("gal_2", "shared/assets/image_samples/astronaut.png", "Astronaut", "cover", "auto", "center", "middle"));
    gal_2->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    gal_2->setFocusStyle(makeStyle({{"background", "#25364f"}}));
    gal_3 = &static_cast<ui::Image&>(addElement<ui::Image>("gal_3", "shared/assets/image_samples/coffee.png", "Coffee", "cover", "auto", "center", "middle"));
    gal_3->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    gal_3->setFocusStyle(makeStyle({{"background", "#25364f"}}));
    gal_4 = &static_cast<ui::Image&>(addElement<ui::Image>("gal_4", "shared/assets/image_samples/coins.png", "Coins", "cover", "auto", "center", "middle"));
    gal_4->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    gal_4->setFocusStyle(makeStyle({{"background", "#25364f"}}));
    gal_5 = &static_cast<ui::Image&>(addElement<ui::Image>("gal_5", "shared/assets/image_samples/chelsea.png", "Chelsea", "cover", "auto", "center", "middle"));
    gal_5->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    gal_5->setFocusStyle(makeStyle({{"background", "#25364f"}}));
}

void GalleryMosaicUI::onGal1Click()
{
}

void GalleryMosaicUI::onGal2Click()
{
}

void GalleryMosaicUI::onGal3Click()
{
}

void GalleryMosaicUI::onGal4Click()
{
}

void GalleryMosaicUI::onGal5Click()
{
}

bool GalleryMosaicUI::shouldClose() const
{
    return false;
}

bool GalleryMosaicUI::handleGeneratedButton(std::string_view name)
{
    if (name == "gal_1")
    {
        onGal1Click();
        return true;
    }
    else if (name == "gal_2")
    {
        onGal2Click();
        return true;
    }
    else if (name == "gal_3")
    {
        onGal3Click();
        return true;
    }
    else if (name == "gal_4")
    {
        onGal4Click();
        return true;
    }
    else if (name == "gal_5")
    {
        onGal5Click();
        return true;
    }
    return false;
}

bool GalleryMosaicUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool GalleryMosaicUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool GalleryMosaicUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions GalleryMosaicUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "gal_1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "gal_2")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "gal_3")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "gal_4")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "gal_5")
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
