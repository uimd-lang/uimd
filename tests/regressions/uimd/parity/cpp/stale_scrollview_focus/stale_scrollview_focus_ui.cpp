#include "stale_scrollview_focus_ui.hpp"

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

[[maybe_unused]] ui::TextGradient makeTextGradient(int intervalMs, int step, int segmentSize, std::initializer_list<const char*> colors)
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
        ui::GeneratedLayoutEntry{.name = "page", .type = "viewhost", .cellName = "content", .relative = ui::Rect{0, 1, 536870911, 536870911}, .sourceCell = ui::Rect{4, 0, 90, 29}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{88, 29}, .cellCharsSize = ui::Size{90, 29}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#030712"}}), .elementStyle = makeStyle({{"background", "#030712"}})},
        ui::GeneratedLayoutEntry{.name = "tab_list", .type = "button", .cellName = "nav", .relative = ui::Rect{0, 1, 16, 1}, .sourceCell = ui::Rect{2, 0, 90, 1}, .width = dimension("auto", 16), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{16, 1}, .cellCharsSize = ui::Size{90, 1}, .marginRight = 73, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}}), .elementStyle = makeStyle({{"background", "#2f80ed"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "title", .type = "label", .cellName = "header", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 90, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{75, 1}, .cellCharsSize = ui::Size{90, 1}, .marginRight = 14, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "tab_other", .type = "button", .cellName = "nav", .relative = ui::Rect{0, 18, 17, 1}, .sourceCell = ui::Rect{2, 0, 90, 1}, .width = dimension("auto", 17), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{17, 1}, .cellCharsSize = ui::Size{90, 1}, .marginRight = 55, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}}), .elementStyle = makeStyle({{"background", "#343b4b"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "hint", .type = "label", .cellName = "nav", .relative = ui::Rect{0, 36, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 90, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{53, 1}, .cellCharsSize = ui::Size{90, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "quit_btn", .type = "button", .cellName = "header", .relative = ui::Rect{0, 77, 12, 1}, .sourceCell = ui::Rect{0, 0, 90, 1}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{90, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

StaleScrollviewFocusUI::StaleScrollviewFocusUI() : ui::GeneratedWindowBase("Stale ScrollView Focus")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "StaleScrollviewFocus", R"UI_MCP_MD(# Stale ScrollView Focus

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: regression
description: "Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button."
tags: [regression, issue-5, viewhost, scrollview]
```

## Definition

```yaml
uses:
  - list_page
  - edit_page
```

## Members

```yaml
title:
  type: label
  text: "Scroll Focus Repro"
  description: "Regression window title."
  expose: false

quit_btn:
  type: button
  title: Quit
  description: "Close the regression app."

tab_list:
  type: button
  title: List
  description: "Show the list page."

tab_other:
  type: button
  title: Other
  description: "Inactive top-level navigation button used to expose stale focus."

hint:
  type: label
  text: "Focus list, press Enter, choose Open."
  description: "Manual reproduction hint."
  expose: false

page:
  type: viewhost
  description: "Dynamic page host."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  background: "#1f2937"
@title:
  color: "#ffffff"
  text-align: center
@nav:
  background: "#172033"
@content:
  background: "#030712"
@hint:
  color: "#93c5fd"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@tab_list:
  background: "#2f80ed"
  focus-background: "#22c55e"
@tab_other:
  background: "#343b4b"
  focus-background: "#9333ea"
@quit_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
viewhost:
  background: "#030712"
```

## User Interface

```ui
+-header-**--------------------------------------------------------------------------------+
| title..**.................................................................. quit_btn.... |
+-nav-**-----------------------------------------------------------------------------------+
| tab_list........ tab_other........ hint..**............................................. |
+-content-**-------------------------------------------------------------------------------+
| page..**................................................................................ |
* *....................................................................................... |
* *....................................................................................... |
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
+------------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"title", "Regression window title.", false}, ui::GeneratedElementMetadata{"quit_btn", "Close the regression app.", true}, ui::GeneratedElementMetadata{"tab_list", "Show the list page.", true}, ui::GeneratedElementMetadata{"tab_other", "Inactive top-level navigation button used to expose stale focus.", true}, ui::GeneratedElementMetadata{"hint", "Manual reproduction hint.", false}, ui::GeneratedElementMetadata{"page", "Dynamic page host.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    title = &static_cast<ui::Label&>(addElement<ui::Label>("title", "Scroll Focus Repro"));
    title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    quit_btn = &static_cast<ui::Button&>(addElement<ui::Button>("quit_btn", "Quit"));
    quit_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    quit_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    quit_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    tab_list = &static_cast<ui::Button&>(addElement<ui::Button>("tab_list", "List"));
    tab_list->setStyle(makeStyle({{"background", "#2f80ed"}, {"color", "#e2e8f0"}}));
    tab_list->setFocusStyle(makeStyle({{"background", "#22c55e"}, {"color", "#ffffff"}}));
    tab_list->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    tab_other = &static_cast<ui::Button&>(addElement<ui::Button>("tab_other", "Other"));
    tab_other->setStyle(makeStyle({{"background", "#343b4b"}, {"color", "#e2e8f0"}}));
    tab_other->setFocusStyle(makeStyle({{"background", "#9333ea"}, {"color", "#ffffff"}}));
    tab_other->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    hint = &static_cast<ui::Label&>(addElement<ui::Label>("hint", "Focus list, press Enter, choose Open."));
    hint->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    hint->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    page = &static_cast<ui::ViewHost&>(addElement<ui::ViewHost>("page"));
    page->setStyle(makeStyle({{"background", "#030712"}}));
}

void StaleScrollviewFocusUI::onQuitBtnClick()
{
}

void StaleScrollviewFocusUI::onTabListClick()
{
}

void StaleScrollviewFocusUI::onTabOtherClick()
{
}

bool StaleScrollviewFocusUI::shouldClose() const
{
    return false;
}

bool StaleScrollviewFocusUI::handleGeneratedButton(std::string_view name)
{
    (void)name;
    if (name == "quit_btn")
    {
        onQuitBtnClick();
        return true;
    }
    else if (name == "tab_list")
    {
        onTabListClick();
        return true;
    }
    else if (name == "tab_other")
    {
        onTabOtherClick();
        return true;
    }
    return false;
}

bool StaleScrollviewFocusUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool StaleScrollviewFocusUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool StaleScrollviewFocusUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    (void)name;
    (void)value;
    return false;
}

ui::GeneratedWindowRuntimeOptions StaleScrollviewFocusUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "quit_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "tab_list")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "tab_other")
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
