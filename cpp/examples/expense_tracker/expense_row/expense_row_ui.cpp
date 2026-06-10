#include "expense_row_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "paid", .type = "checkbox", .cellName = "row", .relative = ui::Rect{0, 1, 9, 1}, .sourceCell = ui::Rect{0, 0, 54, 2}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{54, 2}, .marginRight = 44, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "item", .type = "label", .cellName = "row", .relative = ui::Rect{0, 11, 536870911, 536870911}, .sourceCell = ui::Rect{0, 0, 54, 2}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{29, 2}, .cellCharsSize = ui::Size{54, 2}, .marginRight = 14, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}})},
        ui::GeneratedLayoutEntry{.name = "amount", .type = "label", .cellName = "row", .relative = ui::Rect{0, 41, 12, 1}, .sourceCell = ui::Rect{0, 0, 54, 2}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{54, 2}, .marginRight = 1, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}})},
        ui::GeneratedLayoutEntry{.name = "category", .type = "combobox", .cellName = "row", .relative = ui::Rect{1, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 54, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{39, 1}, .cellCharsSize = ui::Size{54, 2}, .marginRight = 14, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1"}}), .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "delete_btn", .type = "button", .cellName = "row", .relative = ui::Rect{1, 41, 12, 1}, .sourceCell = ui::Rect{0, 0, 54, 2}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{54, 2}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

ExpenseRowUI::ExpenseRowUI() : ui::GeneratedWindowBase("Expense Row")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "ExpenseRow", R"UI_MCP_MD(# Expense Row

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: experimental
description: "Reusable expense row with paid state, category selector, and delete action."
tags: [component, example]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
paid:
  type: checkbox
  title: Paid
  value: false

item:
  type: label
  text: Item
  user-select: text

amount:
  type: label
  text: "0.00"

category:
  type: combobox
  options: [Food, Transport, Home, Health, Other]
  selected_item: Food

delete_btn:
  type: button
  title: Delete
```

## Style

```yaml
this:
  border-width: 0
@row:
  background: "#172033"
  padding: 1
@item:
  color: "#ffffff"
@amount:
  color: "#bfdbfe"
checkbox:
  color: "#cbd5e1"
  checked-color: "#22c55e"
  unchecked-color: "#94a3b8"
combobox:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@delete_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
```

## User Interface

```ui
+-row-**-----------------------------------------------+
| paid..... item..**..................... amount...... |
| category..**........................... delete_btn.. |
+------------------------------------------------------+
```
)UI_MCP_MD", "Reusable expense row with paid state, category selector, and delete action.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"paid", "Paid", true}, ui::GeneratedElementMetadata{"item", "Item", true}, ui::GeneratedElementMetadata{"amount", "0.00", true}, ui::GeneratedElementMetadata{"category", "category", true}, ui::GeneratedElementMetadata{"delete_btn", "Delete", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    paid = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("paid", "Paid", false));
    paid->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    paid->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#ffffff"}}));
    paid->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#22c55e"}}));
    paid->setUncheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#94a3b8"}}));
    paid->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    item = &static_cast<ui::Label&>(addElement<ui::Label>("item", "Item"));
    item->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}}));
    item->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    amount = &static_cast<ui::Label&>(addElement<ui::Label>("amount", "0.00"));
    amount->setStyle(makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}}));
    amount->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    category = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("category", std::vector<std::string>{"Food", "Transport", "Home", "Health", "Other"}));
    category->setSelectedIndex(0);
    category->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    category->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    category->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    category->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    category->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    delete_btn = &static_cast<ui::Button&>(addElement<ui::Button>("delete_btn", "Delete"));
    delete_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    delete_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    delete_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void ExpenseRowUI::onPaidChange(std::string_view value)
{
    (void)value;
}

void ExpenseRowUI::onCategoryChange(std::string_view value)
{
    (void)value;
}

void ExpenseRowUI::onDeleteBtnClick()
{
}

bool ExpenseRowUI::shouldClose() const
{
    return false;
}

bool ExpenseRowUI::handleGeneratedButton(std::string_view name)
{
    if (name == "delete_btn")
    {
        onDeleteBtnClick();
        return true;
    }
    return false;
}

bool ExpenseRowUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "paid")
    {
        onPaidChange(value);
        return true;
    }
    return false;
}

bool ExpenseRowUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ExpenseRowUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "category")
    {
        onCategoryChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions ExpenseRowUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "delete_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "paid")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "category")
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
