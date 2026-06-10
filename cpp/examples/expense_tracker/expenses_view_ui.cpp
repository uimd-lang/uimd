#include "expenses_view_ui.hpp"
#include "expenses_list/expenses_list.hpp"
#include "expenses_list/expenses_list_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "view_title", .type = "label", .cellName = "view", .relative = ui::Rect{0, 1, 58, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 58), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{58, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 1, .marginBottom = 14, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}})},
        ui::GeneratedLayoutEntry{.name = "\"Item:\"", .type = "label", .cellName = "view", .relative = ui::Rect{2, 1, 7, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 7), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{7, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 52, .marginBottom = 12, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "item_input", .type = "textinput", .cellName = "view", .relative = ui::Rect{2, 13, 46, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 46), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{46, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 1, .marginBottom = 12, .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "\"Amount:\"", .type = "label", .cellName = "view", .relative = ui::Rect{4, 1, 9, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 50, .marginBottom = 10, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "amount_input", .type = "numberinput", .cellName = "view", .relative = ui::Rect{4, 13, 25, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 25), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 22, .marginBottom = 10, .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "currency_label", .type = "label", .cellName = "view", .relative = ui::Rect{4, 39, 20, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 20), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 1, .marginBottom = 10, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}})},
        ui::GeneratedLayoutEntry{.name = "\"Category:\"", .type = "label", .cellName = "view", .relative = ui::Rect{6, 1, 11, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 48, .marginBottom = 8, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "category_input", .type = "combobox", .cellName = "view", .relative = ui::Rect{6, 13, 46, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 46), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{46, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 1, .marginBottom = 8, .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "add_expense_btn", .type = "button", .cellName = "view", .relative = ui::Rect{8, 1, 58, 1}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("auto", 58), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{58, 1}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 1, .marginBottom = 6, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "expenses", .type = "expenses_list", .cellName = "view", .relative = ui::Rect{10, 1, 536870911, 536870911}, .sourceCell = ui::Rect{0, 0, 60, 15}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 15), .charsSize = ui::Size{58, 5}, .cellCharsSize = ui::Size{60, 15}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#172033"}})},
    };
}

}  // namespace

ExpensesViewUI::ExpensesViewUI() : ui::GeneratedWindowBase("Expenses View")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("control");
    setMcpMetadata(true, "ExpensesView", R"UI_MCP_MD(# Expenses View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
status: experimental
description: "Expense entry view with a dynamic scrollable list."
tags: [example, viewhost]
```

## Definition

```yaml
extends: uicontrol
uses:
  - expenses_list/expenses_list
```

## Members

```yaml
view_title:
  type: label
  text: Expenses

item_input:
  type: textinput
  value: ""
  maxlength: 80

amount_input:
  type: numberinput
  value: 0
  format: "0.00"

category_input:
  type: combobox
  options: [Food, Transport, Home, Health, Other]
  selected_item: Food

currency_label:
  type: label
  text: EUR

add_expense_btn:
  type: button
  title: Add expense

expenses:
  type: expenses_list
  cpp-class: expense_tracker_example::ExpensesList
  cpp-header: "expenses_list/expenses_list.hpp"
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@view_title:
  color: "#bfdbfe"
@currency_label:
  color: "#bfdbfe"
@expenses:
  background: "#172033"
textinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
numberinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
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
```

## User Interface

```ui
+-view-**----------------------------------------------------+
| view_title................................................ |
|                                                            |
| "Item:"     item_input.................................... |
|                                                            |
| "Amount:"   amount_input............. currency_label...... |
|                                                            |
| "Category:" category_input................................ |
|                                                            |
| add_expense_btn........................................... |
|                                                            |
| expenses..**.............................................. |
| *......................................................... |
| *......................................................... |
| .......................................................... |
| .......................................................... |
+------------------------------------------------------------+
```
)UI_MCP_MD", "Expense entry view with a dynamic scrollable list.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"view_title", "Expenses", true}, ui::GeneratedElementMetadata{"item_input", "item_input", true}, ui::GeneratedElementMetadata{"amount_input", "amount_input", true}, ui::GeneratedElementMetadata{"category_input", "category_input", true}, ui::GeneratedElementMetadata{"currency_label", "EUR", true}, ui::GeneratedElementMetadata{"add_expense_btn", "Add expense", true}, ui::GeneratedElementMetadata{"expenses", "expenses", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#030712"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    view_title = &static_cast<ui::Label&>(addElement<ui::Label>("view_title", "Expenses"));
    view_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}}));
    view_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    item_input = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("item_input", "", 80));
    item_input->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    item_input->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    item_input->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#ffffff"}}));
    item_input->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    item_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    item_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    amount_input = &static_cast<ui::NumberInput&>(addElement<ui::NumberInput>("amount_input", 0.0, 1.0));
    amount_input->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    amount_input->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    amount_input->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#ffffff"}}));
    amount_input->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    amount_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    category_input = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("category_input", std::vector<std::string>{"Food", "Transport", "Home", "Health", "Other"}));
    category_input->setSelectedIndex(0);
    category_input->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    category_input->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    category_input->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    category_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    category_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    currency_label = &static_cast<ui::Label&>(addElement<ui::Label>("currency_label", "EUR"));
    currency_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}}));
    currency_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    add_expense_btn = &static_cast<ui::Button&>(addElement<ui::Button>("add_expense_btn", "Add expense"));
    add_expense_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    add_expense_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    add_expense_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    expenses = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("expenses"));
    expenses->setChild(std::make_unique<expense_tracker_example::ExpensesList>());
    expenses->setStyle(makeStyle({{"background", "#172033"}}));
    expenses->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    expenses->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    expenses->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
}

void ExpensesViewUI::onItemInputChange(std::string_view value)
{
    (void)value;
}

void ExpensesViewUI::onItemInputSubmit(std::string_view value)
{
    (void)value;
}

void ExpensesViewUI::onAmountInputChange(std::string_view value)
{
    (void)value;
}

void ExpensesViewUI::onAmountInputSubmit(std::string_view value)
{
    (void)value;
}

void ExpensesViewUI::onCategoryInputChange(std::string_view value)
{
    (void)value;
}

void ExpensesViewUI::onAddExpenseBtnClick()
{
}

bool ExpensesViewUI::shouldClose() const
{
    return false;
}

bool ExpensesViewUI::handleGeneratedButton(std::string_view name)
{
    if (name == "add_expense_btn")
    {
        onAddExpenseBtnClick();
        return true;
    }
    return false;
}

bool ExpensesViewUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "item_input")
    {
        onItemInputChange(value);
        return true;
    }
    else if (name == "amount_input")
    {
        onAmountInputChange(value);
        return true;
    }
    return false;
}

bool ExpensesViewUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "item_input")
    {
        onItemInputSubmit(value);
        return true;
    }
    else if (name == "amount_input")
    {
        onAmountInputSubmit(value);
        return true;
    }
    return false;
}

bool ExpensesViewUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "category_input")
    {
        onCategoryInputChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions ExpensesViewUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "add_expense_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "item_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "amount_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "item_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "amount_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "category_input")
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
