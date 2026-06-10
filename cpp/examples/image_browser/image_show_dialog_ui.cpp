#include "image_show_dialog_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "", .type = "", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{22, 0, 34, 1}, .width = dimension("auto", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{34, 1}, .cellCharsSize = ui::Size{34, 1}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 1, 1, 1"}})},
        ui::GeneratedLayoutEntry{.name = "", .type = "", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{22, 57, 33, 1}, .width = dimension("auto", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{33, 1}, .cellCharsSize = ui::Size{33, 1}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 1, 1, 1"}})},
        ui::GeneratedLayoutEntry{.name = "title", .type = "label", .cellName = "dialog", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 90, 21}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{88, 1}, .cellCharsSize = ui::Size{90, 21}, .marginRight = 1, .marginBottom = 20, .cellStyle = makeStyle({{"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "close_btn", .type = "button", .cellName = "", .relative = ui::Rect{0, 2, 536870911, 1}, .sourceCell = ui::Rect{22, 35, 21, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{17, 1}, .cellCharsSize = ui::Size{21, 1}, .marginRight = 2, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "photo", .type = "image", .cellName = "dialog", .relative = ui::Rect{1, 1, 536870911, 536870911}, .sourceCell = ui::Rect{0, 0, 90, 21}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{88, 20}, .cellCharsSize = ui::Size{90, 21}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
    };
}

}  // namespace

ImageShowDialogUI::ImageShowDialogUI() : ui::GeneratedWindowBase("Image Show Dialog")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("dialog");
    setMcpMetadata(true, "ImageShowDialog", R"UI_MCP_MD(# Image Show Dialog

## Metadata

```yaml
format: uimd
format-version: 1
kind: dialog
description: "Dialog for showing a selected image."
tags: [example, image, dialog]
```

## Members

```yaml
title:
  type: label
  text: "Image"
  description: "Dialog title."
  expose: false

photo:
  type: image
  source: ""
  alt: ""
  fit: contain
  render_mode: auto
  description: "Selected image preview."

close_btn:
  type: button
  title: Close
  description: "Close the dialog."
```

## Style

```yaml
include: dark
this:
  border-width: 1
  background: "#081426"
  margin: 4, 6, 4, 6
cell:
  padding: 1, 1, 1, 1
@title:
  color: "#f8fafc"
  text-align: center
image:
  background: transparent
  color: "#d1d5db"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
```

## User Interface

```ui
+-dialog-**--------------------------------------------------------------------------------+
| title..**............................................................................... |
| photo..**............................................................................... |
* ........................................................................................ |
* *....................................................................................... |
| *....................................................................................... |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
+-**-------------------------------+---------------------+-**------------------------------+
|                                  |  close_btn........  |                                 |
+----------------------------------+---------------------+---------------------------------+
```
)UI_MCP_MD", "Dialog for showing a selected image.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"title", "Dialog title.", false}, ui::GeneratedElementMetadata{"photo", "Selected image preview.", true}, ui::GeneratedElementMetadata{"close_btn", "Close the dialog.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    ui::requireSixelForImageRendering();
    setGeneratedWindowStyle(makeStyle({{"background", "#081426"}, {"border-color", "transparent"}, {"border-width", "1"}, {"margin", "4, 6, 4, 6"}}));
    title = &static_cast<ui::Label&>(addElement<ui::Label>("title", "Image"));
    title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"text-align", "center"}}));
    title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    photo = &static_cast<ui::Image&>(addElement<ui::Image>("photo", "", "", "contain", "auto", "center", "middle"));
    photo->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    close_btn = &static_cast<ui::Button&>(addElement<ui::Button>("close_btn", "Close"));
    close_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    close_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    close_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void ImageShowDialogUI::onPhotoClick()
{
}

void ImageShowDialogUI::onCloseBtnClick()
{
}

bool ImageShowDialogUI::shouldClose() const
{
    return false;
}

bool ImageShowDialogUI::handleGeneratedButton(std::string_view name)
{
    if (name == "photo")
    {
        onPhotoClick();
        return true;
    }
    else if (name == "close_btn")
    {
        onCloseBtnClick();
        return true;
    }
    return false;
}

bool ImageShowDialogUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageShowDialogUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageShowDialogUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions ImageShowDialogUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "photo")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "close_btn")
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
