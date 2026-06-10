#include "task_row_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "task_title", .type = "label", .cellName = "card", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 47, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{21, 1}, .cellCharsSize = ui::Size{47, 2}, .marginRight = 25, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "meta", .type = "label", .cellName = "card", .relative = ui::Rect{0, 23, 23, 1}, .sourceCell = ui::Rect{0, 0, 47, 2}, .width = dimension("auto", 23), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{23, 1}, .cellCharsSize = ui::Size{47, 2}, .marginRight = 1, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "done", .type = "checkbox", .cellName = "card", .relative = ui::Rect{1, 1, 8, 1}, .sourceCell = ui::Rect{0, 0, 47, 2}, .width = dimension("auto", 8), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{8, 1}, .cellCharsSize = ui::Size{47, 2}, .marginRight = 38, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "spacer", .type = "label", .cellName = "card", .relative = ui::Rect{1, 10, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 47, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{47, 2}, .marginRight = 25, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "open_btn", .type = "button", .cellName = "card", .relative = ui::Rect{1, 23, 10, 1}, .sourceCell = ui::Rect{0, 0, 47, 2}, .width = dimension("auto", 10), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{47, 2}, .marginRight = 14, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "delete_btn", .type = "button", .cellName = "card", .relative = ui::Rect{1, 34, 12, 1}, .sourceCell = ui::Rect{0, 0, 47, 2}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{47, 2}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

TaskRowUI::TaskRowUI() : ui::GeneratedWindowBase("Task Row")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "TaskRow", R"UI_MCP_MD(# Task Row

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Reusable task row with editable controls."
tags: [component, example]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
task_title:
  type: label
  text: "Task"
  description: "Task title."

meta:
  type: label
  text: "Status / assignee / priority"
  description: "Task status, assignee, and priority."

done:
  type: checkbox
  title: Done
  value: false
  description: "Whether the task is done."

spacer:
  type: label
  text: ""
  description: "Flexible row spacer before task actions."
  expose: false

open_btn:
  type: button
  title: Open
  description: "Open this task in the edit dialog."

delete_btn:
  type: button
  title: Delete
  description: "Ask for confirmation and delete this task."
```

## Style

```yaml
this:
  border-width: 0
@card:
  background: "#172033"
  padding: 1, 1, 1, 1
@task_title:
  color: "#f8fafc"
  user-select: text
@meta:
  color: "#93c5fd"
  user-select: text
checkbox:
  color: "#cbd5e1"
  focus-color: "#facc15"
  checked-color: "#22c55e"
  unchecked-color: "#94a3b8"
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
+-card-**---------------------------------------+
| task_title..**....... meta................... |
| done.... spacer..**.. open_btn.. delete_btn.. |
+-----------------------------------------------+
```
)UI_MCP_MD", "Reusable task row with editable controls.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"task_title", "Task title.", true}, ui::GeneratedElementMetadata{"meta", "Task status, assignee, and priority.", true}, ui::GeneratedElementMetadata{"done", "Whether the task is done.", true}, ui::GeneratedElementMetadata{"spacer", "Flexible row spacer before task actions.", false}, ui::GeneratedElementMetadata{"open_btn", "Open this task in the edit dialog.", true}, ui::GeneratedElementMetadata{"delete_btn", "Ask for confirmation and delete this task.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    task_title = &static_cast<ui::Label&>(addElement<ui::Label>("task_title", "Task"));
    task_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"user-select", "text"}}));
    task_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    meta = &static_cast<ui::Label&>(addElement<ui::Label>("meta", "Status / assignee / priority"));
    meta->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}, {"user-select", "text"}}));
    meta->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    done = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("done", "Done", false));
    done->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    done->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#facc15"}}));
    done->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#22c55e"}}));
    done->setUncheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#94a3b8"}}));
    done->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    spacer = &static_cast<ui::Label&>(addElement<ui::Label>("spacer", ""));
    spacer->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    spacer->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    open_btn = &static_cast<ui::Button&>(addElement<ui::Button>("open_btn", "Open"));
    open_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    open_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    open_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    delete_btn = &static_cast<ui::Button&>(addElement<ui::Button>("delete_btn", "Delete"));
    delete_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    delete_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    delete_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void TaskRowUI::onDoneChange(std::string_view value)
{
    (void)value;
}

void TaskRowUI::onOpenBtnClick()
{
}

void TaskRowUI::onDeleteBtnClick()
{
}

bool TaskRowUI::shouldClose() const
{
    return false;
}

bool TaskRowUI::handleGeneratedButton(std::string_view name)
{
    if (name == "open_btn")
    {
        onOpenBtnClick();
        return true;
    }
    else if (name == "delete_btn")
    {
        onDeleteBtnClick();
        return true;
    }
    return false;
}

bool TaskRowUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "done")
    {
        onDoneChange(value);
        return true;
    }
    return false;
}

bool TaskRowUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool TaskRowUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions TaskRowUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "open_btn")
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
        if (name == "done")
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
