#include "categories_view_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "view_title", .type = "label", .cellName = "view", .relative = ui::Rect{0, 1, 58, 1}, .sourceCell = ui::Rect{0, 0, 60, 11}, .width = dimension("auto", 58), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{58, 1}, .cellCharsSize = ui::Size{60, 11}, .marginRight = 1, .marginBottom = 10, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#bbf7d0"}})},
        ui::GeneratedLayoutEntry{.name = "\"Name:\"", .type = "label", .cellName = "view", .relative = ui::Rect{2, 1, 7, 1}, .sourceCell = ui::Rect{0, 0, 60, 11}, .width = dimension("auto", 7), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{7, 1}, .cellCharsSize = ui::Size{60, 11}, .marginRight = 52, .marginBottom = 8, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "category_name", .type = "textinput", .cellName = "view", .relative = ui::Rect{2, 9, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 60, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{49, 1}, .cellCharsSize = ui::Size{60, 11}, .marginRight = 2, .marginBottom = 8, .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "add_category_btn", .type = "button", .cellName = "view", .relative = ui::Rect{4, 1, 32, 1}, .sourceCell = ui::Rect{0, 0, 60, 11}, .width = dimension("auto", 32), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{32, 1}, .cellCharsSize = ui::Size{60, 11}, .marginRight = 27, .marginBottom = 6, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "remove_category_btn", .type = "button", .cellName = "view", .relative = ui::Rect{4, 34, 24, 1}, .sourceCell = ui::Rect{0, 0, 60, 11}, .width = dimension("auto", 24), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{24, 1}, .cellCharsSize = ui::Size{60, 11}, .marginRight = 2, .marginBottom = 6, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "categories", .type = "listbox", .cellName = "view", .relative = ui::Rect{6, 1, 536870911, 536870911}, .sourceCell = ui::Rect{0, 0, 60, 11}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{58, 5}, .cellCharsSize = ui::Size{60, 11}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
    };
}

}  // namespace

CategoriesViewUI::CategoriesViewUI() : ui::GeneratedWindowBase("Categories View")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("control");
    setMcpMetadata(true, "CategoriesView", R"UI_MCP_MD(# Categories View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
status: experimental
description: "Simple category list management view."
tags: [example, viewhost]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
view_title:
  type: label
  text: Categories

category_name:
  type: textinput
  value: ""
  maxlength: 40

add_category_btn:
  type: button
  title: Add

categories:
  type: listbox
  options: [Food, Transport, Home, Health, Other]
  selected_items: [Food]

remove_category_btn:
  type: button
  title: Remove selected
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@view_title:
  color: "#bbf7d0"
textinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
listbox:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
```

## User Interface

```ui
+-view-**----------------------------------------------------+
| view_title................................................ |
|                                                            |
| "Name:" category_name..**................................ |
|                                                            |
| add_category_btn................ remove_category_btn..... |
|                                                            |
| categories..**............................................ |
| *......................................................... |
| *......................................................... |
| .......................................................... |
| .......................................................... |
+------------------------------------------------------------+
```
)UI_MCP_MD", "Simple category list management view.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"view_title", "Categories", true}, ui::GeneratedElementMetadata{"category_name", "category_name", true}, ui::GeneratedElementMetadata{"add_category_btn", "Add", true}, ui::GeneratedElementMetadata{"categories", "categories", true}, ui::GeneratedElementMetadata{"remove_category_btn", "Remove selected", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#030712"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    view_title = &static_cast<ui::Label&>(addElement<ui::Label>("view_title", "Categories"));
    view_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#bbf7d0"}}));
    view_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    category_name = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("category_name", "", 40));
    category_name->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    category_name->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    category_name->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#ffffff"}}));
    category_name->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    category_name->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    category_name->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    add_category_btn = &static_cast<ui::Button&>(addElement<ui::Button>("add_category_btn", "Add"));
    add_category_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    add_category_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    add_category_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    categories = &static_cast<ui::ListBox&>(addElement<ui::ListBox>("categories", std::vector<std::string>{"Food", "Transport", "Home", "Health", "Other"}));
    categories->setSelectedIndex(0);
    categories->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    categories->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    categories->setEditStyle(makeStyle({{"background", "#243a5c"}, {"color", "#cbd5e1"}}));
    categories->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    categories->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    remove_category_btn = &static_cast<ui::Button&>(addElement<ui::Button>("remove_category_btn", "Remove selected"));
    remove_category_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    remove_category_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    remove_category_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void CategoriesViewUI::onCategoryNameChange(std::string_view value)
{
    (void)value;
}

void CategoriesViewUI::onCategoryNameSubmit(std::string_view value)
{
    (void)value;
}

void CategoriesViewUI::onAddCategoryBtnClick()
{
}

void CategoriesViewUI::onCategoriesSelectionChange(const std::vector<std::string>& value)
{
    (void)value;
}

void CategoriesViewUI::onRemoveCategoryBtnClick()
{
}

bool CategoriesViewUI::shouldClose() const
{
    return false;
}

bool CategoriesViewUI::handleGeneratedButton(std::string_view name)
{
    if (name == "add_category_btn")
    {
        onAddCategoryBtnClick();
        return true;
    }
    else if (name == "remove_category_btn")
    {
        onRemoveCategoryBtnClick();
        return true;
    }
    return false;
}

bool CategoriesViewUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "category_name")
    {
        onCategoryNameChange(value);
        return true;
    }
    return false;
}

bool CategoriesViewUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "category_name")
    {
        onCategoryNameSubmit(value);
        return true;
    }
    return false;
}

bool CategoriesViewUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "categories")
    {
        onCategoriesSelectionChange(value);
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions CategoriesViewUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "add_category_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "remove_category_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "category_name")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "category_name")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "categories")
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
