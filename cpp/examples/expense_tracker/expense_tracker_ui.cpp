#include "expense_tracker_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "app_title", .type = "label", .cellName = "header", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 53, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{39, 1}, .cellCharsSize = ui::Size{53, 1}, .marginRight = 13, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "main", .type = "viewhost", .cellName = "content", .relative = ui::Rect{0, 1, 536870911, 536870911}, .sourceCell = ui::Rect{2, 24, 29, 9}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{27, 9}, .cellCharsSize = ui::Size{29, 9}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "1"}, {"background", "#030712"}}), .elementStyle = makeStyle({{"background", "#030712"}})},
        ui::GeneratedLayoutEntry{.name = "nav_title", .type = "label", .cellName = "nav", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 23, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 23), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{21, 1}, .cellCharsSize = ui::Size{23, 9}, .marginRight = 1, .marginBottom = 8, .cellStyle = makeStyle({{"padding", "1"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}})},
        ui::GeneratedLayoutEntry{.name = "quit_btn", .type = "button", .cellName = "header", .relative = ui::Rect{0, 41, 11, 1}, .sourceCell = ui::Rect{0, 0, 53, 1}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{53, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "0"}, {"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "expenses_btn", .type = "button", .cellName = "nav", .relative = ui::Rect{2, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 23, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 23), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{21, 1}, .cellCharsSize = ui::Size{23, 9}, .marginRight = 1, .marginBottom = 6, .cellStyle = makeStyle({{"padding", "1"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "categories_btn", .type = "button", .cellName = "nav", .relative = ui::Rect{4, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 23, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 23), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{21, 1}, .cellCharsSize = ui::Size{23, 9}, .marginRight = 1, .marginBottom = 4, .cellStyle = makeStyle({{"padding", "1"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "settings_btn", .type = "button", .cellName = "nav", .relative = ui::Rect{6, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 23, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 23), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{21, 1}, .cellCharsSize = ui::Size{23, 9}, .marginRight = 1, .marginBottom = 2, .cellStyle = makeStyle({{"padding", "1"}, {"background", "#172033"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

ExpenseTrackerUI::ExpenseTrackerUI() : ui::GeneratedWindowBase("Expense Tracker")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "ExpenseTracker", R"UI_MCP_MD(# Expense Tracker

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: experimental
description: "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost."
tags: [example, viewhost, expenses]
```

## Definition

```yaml
uses:
  - expenses_view
  - categories_view
  - settings_view
```

## Members

```yaml
app_title:
  type: label
  text: "Expense Tracker"

quit_btn:
  type: button
  title: Quit

nav_title:
  type: label
  text: Expenses

expenses_btn:
  type: button
  title: Expenses

categories_btn:
  type: button
  title: Categories

settings_btn:
  type: button
  title: Settings

main:
  type: viewhost
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
@app_title:
  color: "#ffffff"
  text-align: center
@nav:
  background: "#172033"
@content:
  background: "#030712"
@nav_title:
  color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@quit_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
viewhost:
  background: "#030712"
```

## User Interface

```ui
+--header-**------------------------------------------+
| app_title..**.......................... quit_btn... |
+--nav------------------+--content-**-----------------+
| nav_title............ | main..**................... |
|                       | *.......................... |
| expenses_btn......... | *.......................... |
|                       | ........................... |
| categories_btn....... | ........................... |
|                       | ........................... |
| settings_btn......... | ........................... |
*                       * ........................... |
*                       * ........................... |
+-----------------------+-----------------------------+
```
)UI_MCP_MD", "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"app_title", "Expense Tracker", true}, ui::GeneratedElementMetadata{"quit_btn", "Quit", true}, ui::GeneratedElementMetadata{"nav_title", "Expenses", true}, ui::GeneratedElementMetadata{"expenses_btn", "Expenses", true}, ui::GeneratedElementMetadata{"categories_btn", "Categories", true}, ui::GeneratedElementMetadata{"settings_btn", "Settings", true}, ui::GeneratedElementMetadata{"main", "main", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    app_title = &static_cast<ui::Label&>(addElement<ui::Label>("app_title", "Expense Tracker"));
    app_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    app_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    quit_btn = &static_cast<ui::Button&>(addElement<ui::Button>("quit_btn", "Quit"));
    quit_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    quit_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    quit_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    nav_title = &static_cast<ui::Label&>(addElement<ui::Label>("nav_title", "Expenses"));
    nav_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}}));
    nav_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    expenses_btn = &static_cast<ui::Button&>(addElement<ui::Button>("expenses_btn", "Expenses"));
    expenses_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    expenses_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    expenses_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    categories_btn = &static_cast<ui::Button&>(addElement<ui::Button>("categories_btn", "Categories"));
    categories_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    categories_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    categories_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    settings_btn = &static_cast<ui::Button&>(addElement<ui::Button>("settings_btn", "Settings"));
    settings_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    settings_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    settings_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    main = &static_cast<ui::ViewHost&>(addElement<ui::ViewHost>("main"));
    main->setStyle(makeStyle({{"background", "#030712"}}));
}

void ExpenseTrackerUI::onQuitBtnClick()
{
}

void ExpenseTrackerUI::onExpensesBtnClick()
{
}

void ExpenseTrackerUI::onCategoriesBtnClick()
{
}

void ExpenseTrackerUI::onSettingsBtnClick()
{
}

bool ExpenseTrackerUI::shouldClose() const
{
    return false;
}

bool ExpenseTrackerUI::handleGeneratedButton(std::string_view name)
{
    if (name == "quit_btn")
    {
        onQuitBtnClick();
        return true;
    }
    else if (name == "expenses_btn")
    {
        onExpensesBtnClick();
        return true;
    }
    else if (name == "categories_btn")
    {
        onCategoriesBtnClick();
        return true;
    }
    else if (name == "settings_btn")
    {
        onSettingsBtnClick();
        return true;
    }
    return false;
}

bool ExpenseTrackerUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool ExpenseTrackerUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ExpenseTrackerUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions ExpenseTrackerUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "quit_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "expenses_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "categories_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "settings_btn")
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
