#include "image_browser_ui.hpp"
#include "image_button_ui.hpp"
#include "gallery_mosaic_ui.hpp"
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
        ui::GeneratedLayoutEntry{.name = "main", .type = "viewhost", .cellName = "content", .relative = ui::Rect{0, 1, 536870911, 536870911}, .sourceCell = ui::Rect{2, 29, 49, 41}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 41), .charsSize = ui::Size{47, 41}, .cellCharsSize = ui::Size{49, 41}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1"}, {"background", "#030712"}}), .elementStyle = makeStyle({{"background", "#030712"}})},
        ui::GeneratedLayoutEntry{.name = "render_label", .type = "label", .cellName = "sidebar", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 40, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "title", .type = "label", .cellName = "header", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 78, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{64, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 13, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "close_btn", .type = "button", .cellName = "header", .relative = ui::Rect{0, 66, 11, 1}, .sourceCell = ui::Rect{0, 0, 78, 1}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#ffffff"}})},
        ui::GeneratedLayoutEntry{.name = "mode_select", .type = "combobox", .cellName = "sidebar", .relative = ui::Rect{1, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 39, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "sidebar_title", .type = "label", .cellName = "sidebar", .relative = ui::Rect{3, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 37, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "camera_thumb", .type = "image_button", .cellName = "sidebar", .relative = ui::Rect{4, 1, 536870911, 6}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 6), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 6}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 31, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}})},
        ui::GeneratedLayoutEntry{.name = "astro_thumb", .type = "image_button", .cellName = "sidebar", .relative = ui::Rect{10, 1, 536870911, 6}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 6), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 6}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 25, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}})},
        ui::GeneratedLayoutEntry{.name = "coffee_thumb", .type = "image_button", .cellName = "sidebar", .relative = ui::Rect{16, 1, 536870911, 6}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 6), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 6}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 19, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}})},
        ui::GeneratedLayoutEntry{.name = "gallery_label", .type = "label", .cellName = "sidebar", .relative = ui::Rect{22, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 18, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "gallery_mosaic", .type = "gallery_mosaic", .cellName = "sidebar", .relative = ui::Rect{23, 1, 536870911, 9}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 9), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 9}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 9, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}})},
        ui::GeneratedLayoutEntry{.name = "image_list_label", .type = "label", .cellName = "sidebar", .relative = ui::Rect{32, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 8, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "image_list_btn", .type = "image_list_button", .cellName = "sidebar", .relative = ui::Rect{33, 1, 536870911, 5}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 5), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 5}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 3, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}})},
        ui::GeneratedLayoutEntry{.name = "spacer", .type = "label", .cellName = "sidebar", .relative = ui::Rect{38, 1, 536870911, 3}, .sourceCell = ui::Rect{2, 0, 28, 41}, .width = dimension("expanded", 536870911), .height = dimension("auto", 3), .cellWidth = dimension("auto", 28), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{26, 3}, .cellCharsSize = ui::Size{28, 41}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
    };
}

}  // namespace

ImageBrowserUI::ImageBrowserUI() : ui::GeneratedWindowBase("Image Browser")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "ImageBrowser", R"UI_MCP_MD(# Image Browser

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Image browser with a thumbnail sidebar and full-size image panel."
tags: [example, image, viewhost]
```

## Definition

```yaml
uses:
  - image_view
  - gallery_view
  - gallery_mosaic
  - image_button
  - image_list_button
  - image_list_view
  - image_show_dialog
```

## Members

```yaml
title:
  type: label
  text: "Image Browser"
  description: "Window title."
  expose: false

close_btn:
  type: button
  title: Quit
  description: "Close the image browser."

render_label:
  type: label
  text: "Render"
  description: "Label above the render mode selector."
  expose: false

mode_select:
  type: combobox
  options: [Normal, Fallback]
  selected_item: Normal
  description: "Switch between Sixel and fallback image rendering."

sidebar_title:
  type: label
  text: "Photos"
  description: "Sidebar section label."
  expose: false

camera_thumb:
  type: image_button
  description: "Camera thumbnail. Click to view full size."

astro_thumb:
  type: image_button
  description: "Astronaut thumbnail. Click to view full size."

coffee_thumb:
  type: image_button
  description: "Coffee thumbnail. Click to view full size."

gallery_label:
  type: label
  text: "Gallery"
  description: "Label above the gallery mosaic."
  expose: false

image_list_label:
  type: label
  text: "Image items"
  description: "List preview label."
  expose: false

gallery_mosaic:
  type: gallery_mosaic
  description: "Click any image to open the full scrollable gallery."

image_list_btn:
  type: image_list_button
  description: "Click to open the image item list."

spacer:
  type: label
  text: ""
  description: "Spacer that absorbs leftover sidebar height."
  expose: false

main:
  type: viewhost
  description: "Full-size image panel."
```

## Style

```yaml
include: dark
this:
  border-width: 0
cell:
  padding: 1
@header:
  background: "#1f2937"
  padding: 0
@sidebar:
  background: "#172033"
  padding: 0
@content:
  background: "#030712"
@title:
  color: "#ffffff"
  text-align: center
@close_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
  color: "#ffffff"
@render_label:
  color: "#93c5fd"
@sidebar_title:
  color: "#93c5fd"
@gallery_label:
  color: "#93c5fd"
@image_list_label:
  color: "#93c5fd"
combobox:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
viewhost:
  background: "#030712"
```

## User Interface

```ui
+--header-**-------------------------------------------------------------------+
| title..**....................................................... close_btn.. |
+--sidebar-------------------+--content-**-------------------------------------+
| render_label.............. | main..**....................................... |
* mode_select............... | ............................................... |
*                            | *.............................................. |
| sidebar_title............. | *.............................................. |
| camera_thumb.............. | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| astro_thumb............... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| coffee_thumb.............. | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| gallery_label............. | ............................................... |
| gallery_mosaic............ | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| image_list_label.......... | ............................................... |
| image_list_btn............ | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
* spacer.................... | ............................................... |
* .......................... | ............................................... |
| .......................... | ............................................... |
+----------------------------+-------------------------------------------------+
```
)UI_MCP_MD", "Image browser with a thumbnail sidebar and full-size image panel.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"title", "Window title.", false}, ui::GeneratedElementMetadata{"close_btn", "Close the image browser.", true}, ui::GeneratedElementMetadata{"render_label", "Label above the render mode selector.", false}, ui::GeneratedElementMetadata{"mode_select", "Switch between Sixel and fallback image rendering.", true}, ui::GeneratedElementMetadata{"sidebar_title", "Sidebar section label.", false}, ui::GeneratedElementMetadata{"camera_thumb", "Camera thumbnail. Click to view full size.", true}, ui::GeneratedElementMetadata{"astro_thumb", "Astronaut thumbnail. Click to view full size.", true}, ui::GeneratedElementMetadata{"coffee_thumb", "Coffee thumbnail. Click to view full size.", true}, ui::GeneratedElementMetadata{"gallery_label", "Label above the gallery mosaic.", false}, ui::GeneratedElementMetadata{"image_list_label", "List preview label.", false}, ui::GeneratedElementMetadata{"gallery_mosaic", "Click any image to open the full scrollable gallery.", true}, ui::GeneratedElementMetadata{"image_list_btn", "Click to open the image item list.", true}, ui::GeneratedElementMetadata{"spacer", "Spacer that absorbs leftover sidebar height.", false}, ui::GeneratedElementMetadata{"main", "Full-size image panel.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    title = &static_cast<ui::Label&>(addElement<ui::Label>("title", "Image Browser"));
    title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    close_btn = &static_cast<ui::Button&>(addElement<ui::Button>("close_btn", "Quit"));
    close_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#ffffff"}}));
    close_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    close_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    render_label = &static_cast<ui::Label&>(addElement<ui::Label>("render_label", "Render"));
    render_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    render_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    mode_select = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("mode_select", std::vector<std::string>{"Normal", "Fallback"}));
    mode_select->setSelectedIndex(0);
    mode_select->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    mode_select->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    mode_select->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    mode_select->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    mode_select->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    sidebar_title = &static_cast<ui::Label&>(addElement<ui::Label>("sidebar_title", "Photos"));
    sidebar_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    sidebar_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    camera_thumb = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("camera_thumb"));
    camera_thumb->setChild(std::make_unique<ImageButtonUI>());
    camera_thumb->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    camera_thumb->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    camera_thumb->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    astro_thumb = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("astro_thumb"));
    astro_thumb->setChild(std::make_unique<ImageButtonUI>());
    astro_thumb->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    astro_thumb->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    astro_thumb->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    coffee_thumb = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("coffee_thumb"));
    coffee_thumb->setChild(std::make_unique<ImageButtonUI>());
    coffee_thumb->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    coffee_thumb->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    coffee_thumb->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    gallery_label = &static_cast<ui::Label&>(addElement<ui::Label>("gallery_label", "Gallery"));
    gallery_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    gallery_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    image_list_label = &static_cast<ui::Label&>(addElement<ui::Label>("image_list_label", "Image items"));
    image_list_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    image_list_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    gallery_mosaic = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("gallery_mosaic"));
    gallery_mosaic->setChild(std::make_unique<GalleryMosaicUI>());
    gallery_mosaic->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    gallery_mosaic->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    gallery_mosaic->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    image_list_btn = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("image_list_btn"));
    image_list_btn->setChild(std::make_unique<ImageListButtonUI>());
    image_list_btn->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    image_list_btn->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    image_list_btn->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    spacer = &static_cast<ui::Label&>(addElement<ui::Label>("spacer", ""));
    spacer->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    spacer->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    main = &static_cast<ui::ViewHost&>(addElement<ui::ViewHost>("main"));
    main->setStyle(makeStyle({{"background", "#030712"}}));
}

void ImageBrowserUI::onCloseBtnClick()
{
}

void ImageBrowserUI::onModeSelectChange(std::string_view value)
{
    (void)value;
}

bool ImageBrowserUI::shouldClose() const
{
    return false;
}

bool ImageBrowserUI::handleGeneratedButton(std::string_view name)
{
    if (name == "close_btn")
    {
        onCloseBtnClick();
        return true;
    }
    return false;
}

bool ImageBrowserUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageBrowserUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageBrowserUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "mode_select")
    {
        onModeSelectChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions ImageBrowserUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "close_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "mode_select")
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
