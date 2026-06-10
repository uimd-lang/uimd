#include "image_gallery_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header", .type = "label", .cellName = "", .relative = ui::Rect{0, 1, 72, 1}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 72), .height = dimension("auto", 1), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{72, 1}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 13, .marginBottom = 38, .elementStyle = makeStyle({{"background", "#1f2937"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "close_btn", .type = "button", .cellName = "", .relative = ui::Rect{0, 74, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 1, .marginBottom = 38, .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#ffffff"}})},
        ui::GeneratedLayoutEntry{.name = "fallback_label", .type = "label", .cellName = "", .relative = ui::Rect{2, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{84, 1}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 1, .marginBottom = 36, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "camera_fb", .type = "image", .cellName = "", .relative = ui::Rect{3, 2, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 64, .marginBottom = 26, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "astro_fb", .type = "image", .cellName = "", .relative = ui::Rect{3, 23, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 43, .marginBottom = 26, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "coffee_fb", .type = "image", .cellName = "", .relative = ui::Rect{3, 44, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 22, .marginBottom = 26, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "coins_fb", .type = "image", .cellName = "", .relative = ui::Rect{3, 65, 536870911, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("expanded", 536870911), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{21, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 0, .marginBottom = 26, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "sixel_label", .type = "label", .cellName = "", .relative = ui::Rect{14, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{84, 1}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 1, .marginBottom = 24, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "camera_ctn", .type = "image", .cellName = "", .relative = ui::Rect{15, 2, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 64, .marginBottom = 14, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "astro_ctn", .type = "image", .cellName = "", .relative = ui::Rect{15, 23, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 43, .marginBottom = 14, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "coffee_ctn", .type = "image", .cellName = "", .relative = ui::Rect{15, 44, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 22, .marginBottom = 14, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "coins_ctn", .type = "image", .cellName = "", .relative = ui::Rect{15, 65, 536870911, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("expanded", 536870911), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 1, .marginBottom = 14, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "stretch_label", .type = "label", .cellName = "", .relative = ui::Rect{26, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{84, 1}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 1, .marginBottom = 12, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "camera_str", .type = "image", .cellName = "", .relative = ui::Rect{27, 2, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 64, .marginBottom = 2, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "astro_str", .type = "image", .cellName = "", .relative = ui::Rect{27, 23, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 43, .marginBottom = 2, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "coffee_str", .type = "image", .cellName = "", .relative = ui::Rect{27, 44, 20, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("auto", 20), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 22, .marginBottom = 2, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "coins_str", .type = "image", .cellName = "", .relative = ui::Rect{27, 65, 536870911, 10}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("expanded", 536870911), .height = dimension("auto", 10), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{20, 10}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 1, .marginBottom = 2, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "status", .type = "label", .cellName = "", .relative = ui::Rect{38, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 86, 39}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 86), .cellHeight = dimension("auto", 39), .charsSize = ui::Size{84, 1}, .cellCharsSize = ui::Size{86, 39}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
    };
}

}  // namespace

ImageGalleryUI::ImageGalleryUI() : ui::GeneratedWindowBase("Image Gallery")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "ImageGallery", R"UI_MCP_MD(# Image Gallery

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes."
tags: [example, image]
```

## Members

```yaml
header:
  type: label
  text: "Image Gallery"
  description: "Window title."
  expose: false

close_btn:
  type: button
  title: Quit
  description: "Close the image gallery."

fallback_label:
  type: label
  text: "Fallback cover"
  description: "Fallback row label."

sixel_label:
  type: label
  text: "Sixel contain"
  description: "Sixel contain row label."

stretch_label:
  type: label
  text: "Sixel stretch"
  description: "Sixel stretch row label."

camera_fb:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "camera fallback"
  fit: cover
  render_mode: fallback
  description: "Camera image forced through fallback rendering."

astro_fb:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "astronaut fallback"
  fit: cover
  render_mode: fallback
  description: "Astronaut image forced through fallback rendering."

coffee_fb:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "coffee fallback"
  fit: cover
  render_mode: fallback
  description: "Coffee image forced through fallback rendering."

coins_fb:
  type: image
  source: "shared/assets/image_samples/coins.png"
  alt: "coins fallback"
  fit: cover
  render_mode: fallback
  description: "Coins image forced through fallback rendering."

camera_ctn:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "camera sixel"
  fit: contain
  render_mode: sixel
  description: "Camera image in forced Sixel contain mode."

astro_ctn:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "astronaut sixel"
  fit: contain
  render_mode: sixel
  description: "Astronaut image in forced Sixel contain mode."

coffee_ctn:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "coffee sixel"
  fit: contain
  render_mode: sixel
  description: "Coffee image in forced Sixel contain mode."

coins_ctn:
  type: image
  source: "shared/assets/image_samples/coins.png"
  alt: "coins sixel"
  fit: contain
  render_mode: sixel
  description: "Coins image in forced Sixel contain mode."

camera_str:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "camera stretch"
  fit: stretch
  render_mode: sixel
  description: "Camera image in forced Sixel stretch mode."

astro_str:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "astronaut stretch"
  fit: stretch
  render_mode: sixel
  description: "Astronaut image in forced Sixel stretch mode."

coffee_str:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "coffee stretch"
  fit: stretch
  render_mode: sixel
  description: "Coffee image in forced Sixel stretch mode."

coins_str:
  type: image
  source: "shared/assets/image_samples/coins.png"
  alt: "coins stretch"
  fit: stretch
  render_mode: sixel
  description: "Coins image in forced Sixel stretch mode."

status:
  type: label
  text: "Rows: fallback cover, forced Sixel contain, forced Sixel stretch."
  description: "Current image validation status."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@header:
  text-align: center
  background: "#1f2937"
  color: "#ffffff"
@close_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
  color: "#ffffff"
@fallback_label:
  color: "#93c5fd"
@sixel_label:
  color: "#93c5fd"
@stretch_label:
  color: "#93c5fd"
image:
  background: transparent
  color: "#d1d5db"
@status:
  color: "#93c5fd"
```

## User Interface

```ui
+--------------------------------------------------------------------------------------+
| header.................................................................. close_btn.. |
|                                                                                      |
| fallback_label...................................................................... |
|  camera_fb........... astro_fb............ coffee_fb........... coins_fb............. |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|                                                                                      |
| sixel_label......................................................................... |
|  camera_ctn.......... astro_ctn........... coffee_ctn.......... coins_ctn........... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|                                                                                      |
| stretch_label....................................................................... |
|  camera_str.......... astro_str........... coffee_str.......... coins_str........... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|                                                                                      |
| status.............................................................................. |
+--------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header", "Window title.", false}, ui::GeneratedElementMetadata{"close_btn", "Close the image gallery.", true}, ui::GeneratedElementMetadata{"fallback_label", "Fallback row label.", true}, ui::GeneratedElementMetadata{"sixel_label", "Sixel contain row label.", true}, ui::GeneratedElementMetadata{"stretch_label", "Sixel stretch row label.", true}, ui::GeneratedElementMetadata{"camera_fb", "Camera image forced through fallback rendering.", true}, ui::GeneratedElementMetadata{"astro_fb", "Astronaut image forced through fallback rendering.", true}, ui::GeneratedElementMetadata{"coffee_fb", "Coffee image forced through fallback rendering.", true}, ui::GeneratedElementMetadata{"coins_fb", "Coins image forced through fallback rendering.", true}, ui::GeneratedElementMetadata{"camera_ctn", "Camera image in forced Sixel contain mode.", true}, ui::GeneratedElementMetadata{"astro_ctn", "Astronaut image in forced Sixel contain mode.", true}, ui::GeneratedElementMetadata{"coffee_ctn", "Coffee image in forced Sixel contain mode.", true}, ui::GeneratedElementMetadata{"coins_ctn", "Coins image in forced Sixel contain mode.", true}, ui::GeneratedElementMetadata{"camera_str", "Camera image in forced Sixel stretch mode.", true}, ui::GeneratedElementMetadata{"astro_str", "Astronaut image in forced Sixel stretch mode.", true}, ui::GeneratedElementMetadata{"coffee_str", "Coffee image in forced Sixel stretch mode.", true}, ui::GeneratedElementMetadata{"coins_str", "Coins image in forced Sixel stretch mode.", true}, ui::GeneratedElementMetadata{"status", "Current image validation status.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    ui::requireSixelForImageRendering();
    setGeneratedWindowStyle(makeStyle({{"background", "#030712"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header = &static_cast<ui::Label&>(addElement<ui::Label>("header", "Image Gallery"));
    header->setStyle(makeStyle({{"background", "#1f2937"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    close_btn = &static_cast<ui::Button&>(addElement<ui::Button>("close_btn", "Quit"));
    close_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#ffffff"}}));
    close_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    close_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    fallback_label = &static_cast<ui::Label&>(addElement<ui::Label>("fallback_label", "Fallback cover"));
    fallback_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    fallback_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    sixel_label = &static_cast<ui::Label&>(addElement<ui::Label>("sixel_label", "Sixel contain"));
    sixel_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    sixel_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    stretch_label = &static_cast<ui::Label&>(addElement<ui::Label>("stretch_label", "Sixel stretch"));
    stretch_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    stretch_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    camera_fb = &static_cast<ui::Image&>(addElement<ui::Image>("camera_fb", "shared/assets/image_samples/camera.png", "camera fallback", "cover", "fallback", "center", "middle"));
    camera_fb->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    astro_fb = &static_cast<ui::Image&>(addElement<ui::Image>("astro_fb", "shared/assets/image_samples/astronaut.png", "astronaut fallback", "cover", "fallback", "center", "middle"));
    astro_fb->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    coffee_fb = &static_cast<ui::Image&>(addElement<ui::Image>("coffee_fb", "shared/assets/image_samples/coffee.png", "coffee fallback", "cover", "fallback", "center", "middle"));
    coffee_fb->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    coins_fb = &static_cast<ui::Image&>(addElement<ui::Image>("coins_fb", "shared/assets/image_samples/coins.png", "coins fallback", "cover", "fallback", "center", "middle"));
    coins_fb->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    camera_ctn = &static_cast<ui::Image&>(addElement<ui::Image>("camera_ctn", "shared/assets/image_samples/camera.png", "camera sixel", "contain", "sixel", "center", "middle"));
    camera_ctn->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    astro_ctn = &static_cast<ui::Image&>(addElement<ui::Image>("astro_ctn", "shared/assets/image_samples/astronaut.png", "astronaut sixel", "contain", "sixel", "center", "middle"));
    astro_ctn->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    coffee_ctn = &static_cast<ui::Image&>(addElement<ui::Image>("coffee_ctn", "shared/assets/image_samples/coffee.png", "coffee sixel", "contain", "sixel", "center", "middle"));
    coffee_ctn->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    coins_ctn = &static_cast<ui::Image&>(addElement<ui::Image>("coins_ctn", "shared/assets/image_samples/coins.png", "coins sixel", "contain", "sixel", "center", "middle"));
    coins_ctn->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    camera_str = &static_cast<ui::Image&>(addElement<ui::Image>("camera_str", "shared/assets/image_samples/camera.png", "camera stretch", "stretch", "sixel", "center", "middle"));
    camera_str->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    astro_str = &static_cast<ui::Image&>(addElement<ui::Image>("astro_str", "shared/assets/image_samples/astronaut.png", "astronaut stretch", "stretch", "sixel", "center", "middle"));
    astro_str->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    coffee_str = &static_cast<ui::Image&>(addElement<ui::Image>("coffee_str", "shared/assets/image_samples/coffee.png", "coffee stretch", "stretch", "sixel", "center", "middle"));
    coffee_str->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    coins_str = &static_cast<ui::Image&>(addElement<ui::Image>("coins_str", "shared/assets/image_samples/coins.png", "coins stretch", "stretch", "sixel", "center", "middle"));
    coins_str->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    status = &static_cast<ui::Label&>(addElement<ui::Label>("status", "Rows: fallback cover, forced Sixel contain, forced Sixel stretch."));
    status->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    status->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void ImageGalleryUI::onCloseBtnClick()
{
}

void ImageGalleryUI::onCameraFbClick()
{
}

void ImageGalleryUI::onAstroFbClick()
{
}

void ImageGalleryUI::onCoffeeFbClick()
{
}

void ImageGalleryUI::onCoinsFbClick()
{
}

void ImageGalleryUI::onCameraCtnClick()
{
}

void ImageGalleryUI::onAstroCtnClick()
{
}

void ImageGalleryUI::onCoffeeCtnClick()
{
}

void ImageGalleryUI::onCoinsCtnClick()
{
}

void ImageGalleryUI::onCameraStrClick()
{
}

void ImageGalleryUI::onAstroStrClick()
{
}

void ImageGalleryUI::onCoffeeStrClick()
{
}

void ImageGalleryUI::onCoinsStrClick()
{
}

bool ImageGalleryUI::shouldClose() const
{
    return false;
}

bool ImageGalleryUI::handleGeneratedButton(std::string_view name)
{
    if (name == "close_btn")
    {
        onCloseBtnClick();
        return true;
    }
    else if (name == "camera_fb")
    {
        onCameraFbClick();
        return true;
    }
    else if (name == "astro_fb")
    {
        onAstroFbClick();
        return true;
    }
    else if (name == "coffee_fb")
    {
        onCoffeeFbClick();
        return true;
    }
    else if (name == "coins_fb")
    {
        onCoinsFbClick();
        return true;
    }
    else if (name == "camera_ctn")
    {
        onCameraCtnClick();
        return true;
    }
    else if (name == "astro_ctn")
    {
        onAstroCtnClick();
        return true;
    }
    else if (name == "coffee_ctn")
    {
        onCoffeeCtnClick();
        return true;
    }
    else if (name == "coins_ctn")
    {
        onCoinsCtnClick();
        return true;
    }
    else if (name == "camera_str")
    {
        onCameraStrClick();
        return true;
    }
    else if (name == "astro_str")
    {
        onAstroStrClick();
        return true;
    }
    else if (name == "coffee_str")
    {
        onCoffeeStrClick();
        return true;
    }
    else if (name == "coins_str")
    {
        onCoinsStrClick();
        return true;
    }
    return false;
}

bool ImageGalleryUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageGalleryUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageGalleryUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions ImageGalleryUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "close_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "camera_fb")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "astro_fb")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "coffee_fb")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "coins_fb")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "camera_ctn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "astro_ctn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "coffee_ctn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "coins_ctn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "camera_str")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "astro_str")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "coffee_str")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "coins_str")
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
