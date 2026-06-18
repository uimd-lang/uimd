#include "task_filters_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "filters_label", .type = "label", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 78, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{78, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#aab2c3"}})},
        ui::GeneratedLayoutEntry{.name = "search_label", .type = "label", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 28, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 28), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{27, 1}, .cellCharsSize = ui::Size{28, 2}, .marginRight = 1, .marginBottom = 1, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cfd5e3"}})},
        ui::GeneratedLayoutEntry{.name = "owner_label", .type = "label", .cellName = "", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 52, 26, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 26), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{24, 1}, .cellCharsSize = ui::Size{26, 2}, .marginRight = 1, .marginBottom = 1, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cfd5e3"}})},
        ui::GeneratedLayoutEntry{.name = "status_label", .type = "label", .cellName = "", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 29, 22, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{22, 2}, .marginRight = 1, .marginBottom = 1, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cfd5e3"}})},
        ui::GeneratedLayoutEntry{.name = "apply_filters_btn", .type = "button", .cellName = "", .relative = ui::Rect{1, 0, 22, 1}, .sourceCell = ui::Rect{5, 0, 78, 3}, .width = dimension("auto", 22), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{22, 1}, .cellCharsSize = ui::Size{78, 3}, .marginRight = 56, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#2f80ed"}, {"color", "#f5f5f5"}})},
        ui::GeneratedLayoutEntry{.name = "search", .type = "textinput", .cellName = "", .relative = ui::Rect{1, 0, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 28, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 28), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{27, 1}, .cellCharsSize = ui::Size{28, 2}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#252b35"}, {"color", "#ffffff"}})},
        ui::GeneratedLayoutEntry{.name = "owner_filter", .type = "combobox", .cellName = "", .relative = ui::Rect{1, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 52, 26, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 26), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{24, 1}, .cellCharsSize = ui::Size{26, 2}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#252b35"}, {"color", "#ffffff"}})},
        ui::GeneratedLayoutEntry{.name = "status_filter", .type = "combobox", .cellName = "", .relative = ui::Rect{1, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 29, 22, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 22), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{22, 2}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#252b35"}, {"color", "#ffffff"}})},
        ui::GeneratedLayoutEntry{.name = "reset_filters_btn", .type = "button", .cellName = "", .relative = ui::Rect{1, 24, 22, 1}, .sourceCell = ui::Rect{5, 0, 78, 3}, .width = dimension("auto", 22), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{22, 1}, .cellCharsSize = ui::Size{78, 3}, .marginRight = 32, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#343b4b"}, {"color", "#f5f5f5"}})},
    };
}

}  // namespace

TaskFiltersUI::TaskFiltersUI() : ui::GeneratedWindowBase("Task Filters")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("control");
    setMcpMetadata(true, "TaskFilters", R"UI_MCP_MD(# Task Filters

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
status: draft
description: "Reusable task board filter panel."
tags: [component, example]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
filters_label:
  type: label
  text: FILTERS
  description: "Filter panel label."
  expose: false

search_label:
  type: label
  text: Search
  description: "Search filter label."
  expose: false

status_label:
  type: label
  text: Status
  description: "Status filter label."
  expose: false

owner_label:
  type: label
  text: Assignee
  description: "Assignee filter label."
  expose: false

search:
  type: textinput
  value: ""
  maxlength: 60
  placeholder: "Task title or description"
  description: "Filter tasks by title or description."

status_filter:
  type: combobox
  options: [Any, Todo, Doing, Blocked, Done]
  selected_item: Any
  description: "Filter tasks by status."

owner_filter:
  type: combobox
  options: [Any, Ava, Bruno, Chen, Dana]
  selected_item: Any
  description: "Filter tasks by assignee."

apply_filters_btn:
  type: button
  title: "Apply filter"
  description: "Apply the current task filters."

reset_filters_btn:
  type: button
  title: Reset
  description: "Reset search, status, and assignee filters."
```

## Style

```yaml
include: dark
this:
  background: "#15171c"
  border-width: 0
label:
  color: "#f5f5f5"
textinput:
  background: "#252b35"
  color: "#ffffff"
  focus-background: "#303a49"
  edit-background: "#344459"
  cursor-background: "#93c5fd"
  cursor-color: "#ffffff"
combobox:
  background: "#252b35"
  color: "#ffffff"
  focus-background: "#303a49"
  edit-background: "#344459"
  selected-background: "#2f80ed"
  selected-color: "#ffffff"
button:
  background: "#2f3545"
  color: "#f5f5f5"
  focus-background: "#2f80ed"
@filters_label:
  color: "#aab2c3"
@search_label:
  color: "#cfd5e3"
@status_label:
  color: "#cfd5e3"
@owner_label:
  color: "#cfd5e3"
@apply_filters_btn:
  background: "#2f80ed"
  focus-background: "#1d6fd8"
@reset_filters_btn:
  background: "#343b4b"
```

## User Interface

```ui
+-**---------------------------------------------------------------------------+
|filters_label..**.............................................................|
+----------------------------+----------------------+--------------------------+
|search_label..**........... | status_label..**.... | owner_label..**......... |
|search..**................. | status_filter.**.... | owner_filter..**........ |
+-**-------------------------+----------------------+--------------------------+
|                                                                              |
|apply_filters_btn.....  reset_filters_btn.....                                |
|                                                                              |
+------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Reusable task board filter panel.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"filters_label", "Filter panel label.", false}, ui::GeneratedElementMetadata{"search_label", "Search filter label.", false}, ui::GeneratedElementMetadata{"status_label", "Status filter label.", false}, ui::GeneratedElementMetadata{"owner_label", "Assignee filter label.", false}, ui::GeneratedElementMetadata{"search", "Filter tasks by title or description.", true}, ui::GeneratedElementMetadata{"status_filter", "Filter tasks by status.", true}, ui::GeneratedElementMetadata{"owner_filter", "Filter tasks by assignee.", true}, ui::GeneratedElementMetadata{"apply_filters_btn", "Apply the current task filters.", true}, ui::GeneratedElementMetadata{"reset_filters_btn", "Reset search, status, and assignee filters.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#15171c"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    filters_label = &static_cast<ui::Label&>(addElement<ui::Label>("filters_label", "FILTERS"));
    filters_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#aab2c3"}}));
    filters_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    search_label = &static_cast<ui::Label&>(addElement<ui::Label>("search_label", "Search"));
    search_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cfd5e3"}}));
    search_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    status_label = &static_cast<ui::Label&>(addElement<ui::Label>("status_label", "Status"));
    status_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cfd5e3"}}));
    status_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    owner_label = &static_cast<ui::Label&>(addElement<ui::Label>("owner_label", "Assignee"));
    owner_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cfd5e3"}}));
    owner_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    search = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("search", "", 60));
    search->setStyle(makeStyle({{"background", "#252b35"}, {"color", "#ffffff"}}));
    search->setFocusStyle(makeStyle({{"background", "#303a49"}, {"color", "#ffffff"}}));
    search->setEditStyle(makeStyle({{"background", "#344459"}, {"color", "#ffffff"}}));
    search->setCursorStyle(makeStyle({{"background", "#93c5fd"}, {"color", "#ffffff"}}));
    search->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    search->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    status_filter = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("status_filter", std::vector<std::string>{"Any", "Todo", "Doing", "Blocked", "Done"}));
    status_filter->setSelectedIndex(0);
    status_filter->setStyle(makeStyle({{"background", "#252b35"}, {"color", "#ffffff"}}));
    status_filter->setFocusStyle(makeStyle({{"background", "#303a49"}, {"color", "#ffffff"}}));
    status_filter->setEditStyle(makeStyle({{"background", "#344459"}, {"color", "#e5e7eb"}}));
    status_filter->setSelectedStyle(makeStyle({{"background", "#2f80ed"}, {"color", "#ffffff"}}));
    status_filter->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    owner_filter = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("owner_filter", std::vector<std::string>{"Any", "Ava", "Bruno", "Chen", "Dana"}));
    owner_filter->setSelectedIndex(0);
    owner_filter->setStyle(makeStyle({{"background", "#252b35"}, {"color", "#ffffff"}}));
    owner_filter->setFocusStyle(makeStyle({{"background", "#303a49"}, {"color", "#ffffff"}}));
    owner_filter->setEditStyle(makeStyle({{"background", "#344459"}, {"color", "#e5e7eb"}}));
    owner_filter->setSelectedStyle(makeStyle({{"background", "#2f80ed"}, {"color", "#ffffff"}}));
    owner_filter->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    apply_filters_btn = &static_cast<ui::Button&>(addElement<ui::Button>("apply_filters_btn", "Apply filter"));
    apply_filters_btn->setStyle(makeStyle({{"background", "#2f80ed"}, {"color", "#f5f5f5"}}));
    apply_filters_btn->setFocusStyle(makeStyle({{"background", "#1d6fd8"}, {"color", "#ffffff"}}));
    apply_filters_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    reset_filters_btn = &static_cast<ui::Button&>(addElement<ui::Button>("reset_filters_btn", "Reset"));
    reset_filters_btn->setStyle(makeStyle({{"background", "#343b4b"}, {"color", "#f5f5f5"}}));
    reset_filters_btn->setFocusStyle(makeStyle({{"background", "#2f80ed"}, {"color", "#ffffff"}}));
    reset_filters_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void TaskFiltersUI::onSearchChange(std::string_view value)
{
    (void)value;
}

void TaskFiltersUI::onSearchSubmit(std::string_view value)
{
    (void)value;
}

void TaskFiltersUI::onStatusFilterChange(std::string_view value)
{
    (void)value;
}

void TaskFiltersUI::onOwnerFilterChange(std::string_view value)
{
    (void)value;
}

void TaskFiltersUI::onApplyFiltersBtnClick()
{
}

void TaskFiltersUI::onResetFiltersBtnClick()
{
}

bool TaskFiltersUI::shouldClose() const
{
    return false;
}

bool TaskFiltersUI::handleGeneratedButton(std::string_view name)
{
    if (name == "apply_filters_btn")
    {
        onApplyFiltersBtnClick();
        return true;
    }
    else if (name == "reset_filters_btn")
    {
        onResetFiltersBtnClick();
        return true;
    }
    return false;
}

bool TaskFiltersUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "search")
    {
        onSearchChange(value);
        return true;
    }
    return false;
}

bool TaskFiltersUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "search")
    {
        onSearchSubmit(value);
        return true;
    }
    return false;
}

bool TaskFiltersUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "status_filter")
    {
        onStatusFilterChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    else if (name == "owner_filter")
    {
        onOwnerFilterChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions TaskFiltersUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "apply_filters_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "reset_filters_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "search")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "search")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "status_filter")
        {
            (void)handleGeneratedSelectionChanged(name, value);
        }
        else if (name == "owner_filter")
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
