#include "image_list_item_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "sel", .type = "checkbox", .cellName = "card", .relative = ui::Rect{1, 2, 3, 5}, .sourceCell = ui::Rect{0, 0, 71, 7}, .width = dimension("auto", 3), .height = dimension("auto", 5), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{3, 5}, .cellCharsSize = ui::Size{71, 7}, .marginRight = 66, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "0, 0, 0, 0"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "thumb", .type = "image", .cellName = "card", .relative = ui::Rect{1, 6, 8, 5}, .sourceCell = ui::Rect{0, 0, 71, 7}, .width = dimension("auto", 8), .height = dimension("auto", 5), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{8, 5}, .cellCharsSize = ui::Size{71, 7}, .marginRight = 57, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "0, 0, 0, 0"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "name_label", .type = "label", .cellName = "card", .relative = ui::Rect{1, 16, 536870911, 2}, .sourceCell = ui::Rect{0, 0, 71, 7}, .width = dimension("expanded", 536870911), .height = dimension("auto", 2), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{16, 2}, .cellCharsSize = ui::Size{71, 7}, .marginRight = 39, .marginBottom = 4, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "0, 0, 0, 0"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "show_btn", .type = "button", .cellName = "card", .relative = ui::Rect{1, 34, 11, 5}, .sourceCell = ui::Rect{0, 0, 71, 7}, .width = dimension("auto", 11), .height = dimension("auto", 5), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{11, 5}, .cellCharsSize = ui::Size{71, 7}, .marginRight = 26, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "0, 0, 0, 0"}}), .elementStyle = makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "browse_btn", .type = "button", .cellName = "card", .relative = ui::Rect{1, 46, 11, 5}, .sourceCell = ui::Rect{0, 0, 71, 7}, .width = dimension("auto", 11), .height = dimension("auto", 5), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{11, 5}, .cellCharsSize = ui::Size{71, 7}, .marginRight = 14, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "0, 0, 0, 0"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "delete_btn", .type = "button", .cellName = "card", .relative = ui::Rect{1, 58, 11, 5}, .sourceCell = ui::Rect{0, 0, 71, 7}, .width = dimension("auto", 11), .height = dimension("auto", 5), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{11, 5}, .cellCharsSize = ui::Size{71, 7}, .marginRight = 2, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "0, 0, 0, 0"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "path_label", .type = "label", .cellName = "card", .relative = ui::Rect{3, 16, 536870911, 2}, .sourceCell = ui::Rect{0, 0, 71, 7}, .width = dimension("expanded", 536870911), .height = dimension("auto", 2), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{53, 2}, .cellCharsSize = ui::Size{71, 7}, .marginRight = 2, .marginBottom = 2, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "0, 0, 0, 0"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#64748b"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "spacer", .type = "label", .cellName = "card", .relative = ui::Rect{5, 16, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 71, 7}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{53, 1}, .cellCharsSize = ui::Size{71, 7}, .marginRight = 2, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "0, 0, 0, 0"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
    };
}

}  // namespace

ImageListItemUI::ImageListItemUI() : ui::GeneratedWindowBase("Image List Item")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "ImageListItem", R"UI_MCP_MD(# Image List Item

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
description: "Reusable row for one image item."
tags: [example, image, list]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
sel:
  type: checkbox
  title: ""
  value: false
  description: "Select this image item."

thumb:
  type: image
  source: ""
  alt: ""
  fit: contain
  render_mode: auto
  description: "Small image thumbnail."

name_label:
  type: label
  text: ""
  description: "Image file name."

path_label:
  type: label
  text: ""
  description: "Full image path."

spacer:
  type: label
  text: ""
  description: "Flexible spacer before item actions."
  expose: false

show_btn:
  type: button
  title: Show
  description: "Show this image in a dialog."

browse_btn:
  type: button
  title: Browse
  description: "Choose a replacement image."

delete_btn:
  type: button
  title: Delete
  description: "Delete this image item after confirmation."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@card:
  background: "#172033"
  padding: 0, 0, 0, 0
@name_label:
  color: "#f8fafc"
  user-select: text
@path_label:
  color: "#64748b"
  user-select: text
checkbox:
  color: "#cbd5e1"
  focus-color: "#facc15"
  checked-color: "#22c55e"
  unchecked-color: "#94a3b8"
image:
  background: transparent
  color: "#d1d5db"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@show_btn:
  background: "#14532d"
  focus-background: "#16a34a"
@delete_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
```

## User Interface

```ui
+-card-**---------------------------------------------------------------+
|                                                                       |
|  sel thumb...  name_label..**..  show_btn... browse_btn. delete_btn.  |
|  ... ........  .....................................................  |
|  ... ........  path_label..**.......................................  |
|  ... ........  .....................................................  |
|  ... ........  spacer..**...........................................  |
|                                                                       |
+-----------------------------------------------------------------------+
```
)UI_MCP_MD", "Reusable row for one image item.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"sel", "Select this image item.", true}, ui::GeneratedElementMetadata{"thumb", "Small image thumbnail.", true}, ui::GeneratedElementMetadata{"name_label", "Image file name.", true}, ui::GeneratedElementMetadata{"path_label", "Full image path.", true}, ui::GeneratedElementMetadata{"spacer", "Flexible spacer before item actions.", false}, ui::GeneratedElementMetadata{"show_btn", "Show this image in a dialog.", true}, ui::GeneratedElementMetadata{"browse_btn", "Choose a replacement image.", true}, ui::GeneratedElementMetadata{"delete_btn", "Delete this image item after confirmation.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    ui::requireSixelForImageRendering();
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    sel = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("sel", "", false));
    sel->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    sel->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#facc15"}}));
    sel->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#22c55e"}}));
    sel->setUncheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#94a3b8"}}));
    sel->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    thumb = &static_cast<ui::Image&>(addElement<ui::Image>("thumb", "", "", "contain", "auto", "center", "middle"));
    thumb->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    name_label = &static_cast<ui::Label&>(addElement<ui::Label>("name_label", ""));
    name_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"user-select", "text"}}));
    name_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    path_label = &static_cast<ui::Label&>(addElement<ui::Label>("path_label", ""));
    path_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#64748b"}, {"user-select", "text"}}));
    path_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    spacer = &static_cast<ui::Label&>(addElement<ui::Label>("spacer", ""));
    spacer->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    spacer->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    show_btn = &static_cast<ui::Button&>(addElement<ui::Button>("show_btn", "Show"));
    show_btn->setStyle(makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}}));
    show_btn->setFocusStyle(makeStyle({{"background", "#16a34a"}, {"color", "#ffffff"}}));
    show_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    browse_btn = &static_cast<ui::Button&>(addElement<ui::Button>("browse_btn", "Browse"));
    browse_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    browse_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    browse_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    delete_btn = &static_cast<ui::Button&>(addElement<ui::Button>("delete_btn", "Delete"));
    delete_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    delete_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    delete_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void ImageListItemUI::onSelChange(std::string_view value)
{
    (void)value;
}

void ImageListItemUI::onThumbClick()
{
}

void ImageListItemUI::onShowBtnClick()
{
}

void ImageListItemUI::onBrowseBtnClick()
{
}

void ImageListItemUI::onDeleteBtnClick()
{
}

bool ImageListItemUI::shouldClose() const
{
    return false;
}

bool ImageListItemUI::handleGeneratedButton(std::string_view name)
{
    if (name == "thumb")
    {
        onThumbClick();
        return true;
    }
    else if (name == "show_btn")
    {
        onShowBtnClick();
        return true;
    }
    else if (name == "browse_btn")
    {
        onBrowseBtnClick();
        return true;
    }
    else if (name == "delete_btn")
    {
        onDeleteBtnClick();
        return true;
    }
    return false;
}

bool ImageListItemUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "sel")
    {
        onSelChange(value);
        return true;
    }
    return false;
}

bool ImageListItemUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ImageListItemUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions ImageListItemUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "thumb")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "show_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "browse_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "delete_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "sel")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
