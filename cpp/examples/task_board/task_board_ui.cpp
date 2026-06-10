#include "task_board_ui.hpp"
#include "task_list/task_list.hpp"
#include "task_list/task_list_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "board", .type = "task_list", .cellName = "board_cell", .relative = ui::Rect{0, 1, 536870911, 536870911}, .sourceCell = ui::Rect{2, 28, 50, 19}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 19), .charsSize = ui::Size{48, 19}, .cellCharsSize = ui::Size{50, 19}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#030712"}}), .elementStyle = makeStyle({{"background", "#030712"}, {"scope-dim-background", "#0000004d"}})},
        ui::GeneratedLayoutEntry{.name = "search_label", .type = "label", .cellName = "filters", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 18, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "title", .type = "label", .cellName = "header", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 78, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 52, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "quit_btn", .type = "button", .cellName = "header", .relative = ui::Rect{0, 65, 12, 1}, .sourceCell = ui::Rect{0, 0, 78, 1}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "search", .type = "textinput", .cellName = "filters", .relative = ui::Rect{1, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 17, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "status_label", .type = "label", .cellName = "filters", .relative = ui::Rect{3, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 15, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "status_filter", .type = "combobox", .cellName = "filters", .relative = ui::Rect{4, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 14, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "owner_label", .type = "label", .cellName = "filters", .relative = ui::Rect{6, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 12, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "owner_filter", .type = "combobox", .cellName = "filters", .relative = ui::Rect{7, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 11, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "reset_filters_btn", .type = "button", .cellName = "filters", .relative = ui::Rect{9, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 9, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#711212"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "new_task_btn", .type = "button", .cellName = "filters", .relative = ui::Rect{11, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 1}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 7, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "status", .type = "label", .cellName = "filters", .relative = ui::Rect{13, 1, 536870911, 3}, .sourceCell = ui::Rect{2, 0, 27, 19}, .width = dimension("expanded", 536870911), .height = dimension("auto", 3), .cellWidth = dimension("fixed", 24), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{25, 3}, .cellCharsSize = ui::Size{27, 19}, .marginRight = 1, .marginBottom = 3, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#111827"}, {"color", "#93c5fd"}})},
    };
}

}  // namespace

TaskBoardUI::TaskBoardUI() : ui::GeneratedWindowBase("Task Board")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "TaskBoard", R"UI_MCP_MD(# Task Board

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools."
tags: [example, mcp]
```

## Definition

```yaml
uses:
  - task_list/task_list
  - task_dialog/task_dialog
```

## Members

```yaml
title:
  type: label
  text: "Task Board"
  description: "Application title."
  expose: false

quit_btn:
  type: button
  title: Quit
  description: "Close the task list example."

search_label:
  type: label
  text: Search
  description: "Search filter label."
  expose: false

search:
  type: textinput
  value: ""
  maxlength: 60
  description: "Filter tasks by title or description."

status_label:
  type: label
  text: Filter status
  description: "Status filter label."
  expose: false

status_filter:
  type: combobox
  options: [Any, Todo, Doing, Blocked, Done]
  selected_item: Any
  description: "Filter tasks by status."

owner_label:
  type: label
  text: Filter assignee
  description: "Assignee filter label."
  expose: false

owner_filter:
  type: combobox
  options: [Any, Ava, Bruno, Chen, Dana]
  selected_item: Any
  description: "Filter tasks by assignee."

reset_filters_btn:
  type: button
  title: Reset filters
  description: "Reset search, status, and assignee filters."

new_task_btn:
  type: button
  title: New task
  description: "Open a dialog for creating a task."

board:
  type: task_list
  cpp-class: task_board_example::TaskList
  cpp-header: "task_list/task_list.hpp"
  description: "Scrollable list of interactive task rows."

status:
  type: label
  text: Ready
  description: "Current board status."
```

## Tools

```yaml
query_tasks:
  description: "Return tasks matching optional title, status, assignee, priority, and done filters."
  input:
    title: string
    status: string
    assignee: string
    priority: string
    done: bool
  output:
    tasks:
      - id: string
        title: string
        status: string
        assignee: string
        priority: string
        description: string
        done: bool

get_task:
  description: "Return one task by id."
  input:
    id: string
    required: [id]
  output:
    task:
      id: string
      title: string
      status: string
      assignee: string
      priority: string
      description: string
      done: bool

update_task:
  description: "Update a task by id and return the updated task."
  input:
    id: string
    title: string
    status: string
    assignee: string
    priority: string
    description: string
    done: bool
    required: [id]
  output:
    task:
      id: string
      title: string
      status: string
      assignee: string
      priority: string
      description: string
      done: bool

delete_task:
  description: "Delete a task by id and return whether it was removed."
  input:
    id: string
    required: [id]
  output:
    deleted: bool
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
@filters:
  background: "#172033"
  padding: 1, 1, 1, 1
@board_cell:
  background: "#030712"
@board:
  background: "#030712"
  focus-background: "#07111f"
  edit-background: "#0f172a"
  scope-dim-background: "#0000004d"
textinput:
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
@quit_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@reset_filters_btn:
  background: "#711212"
  focus-background: "#b91c1c"
@new_task_btn:
  background: "#14532d"
  focus-background: "#16a34a"
@status:
  background: "#111827"
  color: "#93c5fd"
```

## User Interface

```ui
+-header-**--------------------------------------------------------------------+
| title..**................ ..................................... quit_btn.... |
+-filters-24----------------+-board_cell-**------------------------------------+
| search_label..**......... | board..**....................................... |
| search..**............... | ................................................ |
*                           | *............................................... |
* status_label..**......... | *............................................... |
| status_filter............ | ................................................ |
|                           | ................................................ |
| owner_label..**.......... | ................................................ |
| owner_filter............. | ................................................ |
|                           | ................................................ |
| reset_filters_btn..**.... | ................................................ |
|                           | ................................................ |
| new_task_btn..**......... | ................................................ |
|                           | ................................................ |
| status..**............... | ................................................ |
| ......................... | ................................................ |
| ......................... | ................................................ |
|                           | ................................................ |
|                           | ................................................ |
|                           | ................................................ |
+---------------------------+--------------------------------------------------+
```
)UI_MCP_MD", "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"title", "Application title.", false}, ui::GeneratedElementMetadata{"quit_btn", "Close the task list example.", true}, ui::GeneratedElementMetadata{"search_label", "Search filter label.", false}, ui::GeneratedElementMetadata{"search", "Filter tasks by title or description.", true}, ui::GeneratedElementMetadata{"status_label", "Status filter label.", false}, ui::GeneratedElementMetadata{"status_filter", "Filter tasks by status.", true}, ui::GeneratedElementMetadata{"owner_label", "Assignee filter label.", false}, ui::GeneratedElementMetadata{"owner_filter", "Filter tasks by assignee.", true}, ui::GeneratedElementMetadata{"reset_filters_btn", "Reset search, status, and assignee filters.", true}, ui::GeneratedElementMetadata{"new_task_btn", "Open a dialog for creating a task.", true}, ui::GeneratedElementMetadata{"board", "Scrollable list of interactive task rows.", true}, ui::GeneratedElementMetadata{"status", "Current board status.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{ui::GeneratedAppToolMetadata{"delete_task", "Delete a task by id and return whether it was removed.", R"UI_MCP_MD({"type":"object","properties":{"id":{"type":"string"}},"required":["id"]})UI_MCP_MD", R"UI_MCP_MD({"type":"object","properties":{"deleted":{"type":"boolean"}}})UI_MCP_MD"}, ui::GeneratedAppToolMetadata{"get_task", "Return one task by id.", R"UI_MCP_MD({"type":"object","properties":{"id":{"type":"string"}},"required":["id"]})UI_MCP_MD", R"UI_MCP_MD({"type":"object","properties":{"task":{"type":"object","properties":{"id":{"type":"string"},"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"description":{"type":"string"},"done":{"type":"boolean"}}}}})UI_MCP_MD"}, ui::GeneratedAppToolMetadata{"query_tasks", "Return tasks matching optional title, status, assignee, priority, and done filters.", R"UI_MCP_MD({"type":"object","properties":{"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"done":{"type":"boolean"}}})UI_MCP_MD", R"UI_MCP_MD({"type":"object","properties":{"tasks":{"type":"array","items":{"type":"object","properties":{"id":{"type":"string"},"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"description":{"type":"string"},"done":{"type":"boolean"}}}}}})UI_MCP_MD"}, ui::GeneratedAppToolMetadata{"update_task", "Update a task by id and return the updated task.", R"UI_MCP_MD({"type":"object","properties":{"id":{"type":"string"},"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"description":{"type":"string"},"done":{"type":"boolean"}},"required":["id"]})UI_MCP_MD", R"UI_MCP_MD({"type":"object","properties":{"task":{"type":"object","properties":{"id":{"type":"string"},"title":{"type":"string"},"status":{"type":"string"},"assignee":{"type":"string"},"priority":{"type":"string"},"description":{"type":"string"},"done":{"type":"boolean"}}}}})UI_MCP_MD"}});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    title = &static_cast<ui::Label&>(addElement<ui::Label>("title", "Task Board"));
    title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    quit_btn = &static_cast<ui::Button&>(addElement<ui::Button>("quit_btn", "Quit"));
    quit_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    quit_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    quit_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    search_label = &static_cast<ui::Label&>(addElement<ui::Label>("search_label", "Search"));
    search_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    search_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    search = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("search", "", 60));
    search->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    search->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    search->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#ffffff"}}));
    search->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    search->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    search->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    status_label = &static_cast<ui::Label&>(addElement<ui::Label>("status_label", "Filter status"));
    status_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    status_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    status_filter = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("status_filter", std::vector<std::string>{"Any", "Todo", "Doing", "Blocked", "Done"}));
    status_filter->setSelectedIndex(0);
    status_filter->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    status_filter->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    status_filter->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    status_filter->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    status_filter->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    owner_label = &static_cast<ui::Label&>(addElement<ui::Label>("owner_label", "Filter assignee"));
    owner_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    owner_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    owner_filter = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("owner_filter", std::vector<std::string>{"Any", "Ava", "Bruno", "Chen", "Dana"}));
    owner_filter->setSelectedIndex(0);
    owner_filter->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    owner_filter->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    owner_filter->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    owner_filter->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    owner_filter->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    reset_filters_btn = &static_cast<ui::Button&>(addElement<ui::Button>("reset_filters_btn", "Reset filters"));
    reset_filters_btn->setStyle(makeStyle({{"background", "#711212"}, {"color", "#e2e8f0"}}));
    reset_filters_btn->setFocusStyle(makeStyle({{"background", "#b91c1c"}, {"color", "#ffffff"}}));
    reset_filters_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    new_task_btn = &static_cast<ui::Button&>(addElement<ui::Button>("new_task_btn", "New task"));
    new_task_btn->setStyle(makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}}));
    new_task_btn->setFocusStyle(makeStyle({{"background", "#16a34a"}, {"color", "#ffffff"}}));
    new_task_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    board = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("board"));
    board->setChild(std::make_unique<task_board_example::TaskList>());
    board->setStyle(makeStyle({{"background", "#030712"}, {"scope-dim-background", "#0000004d"}}));
    board->setFocusStyle(makeStyle({{"background", "#07111f"}}));
    board->setEditStyle(makeStyle({{"background", "#0f172a"}}));
    board->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    status = &static_cast<ui::Label&>(addElement<ui::Label>("status", "Ready"));
    status->setStyle(makeStyle({{"background", "#111827"}, {"color", "#93c5fd"}}));
    status->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void TaskBoardUI::onQuitBtnClick()
{
}

void TaskBoardUI::onSearchChange(std::string_view value)
{
    (void)value;
}

void TaskBoardUI::onSearchSubmit(std::string_view value)
{
    (void)value;
}

void TaskBoardUI::onStatusFilterChange(std::string_view value)
{
    (void)value;
}

void TaskBoardUI::onOwnerFilterChange(std::string_view value)
{
    (void)value;
}

void TaskBoardUI::onResetFiltersBtnClick()
{
}

void TaskBoardUI::onNewTaskBtnClick()
{
}

bool TaskBoardUI::shouldClose() const
{
    return false;
}

std::string TaskBoardUI::deleteTask(std::string_view inputJson)
{
    (void)inputJson;
    throw std::runtime_error("App tool not implemented: delete_task");
}

std::string TaskBoardUI::getTask(std::string_view inputJson)
{
    (void)inputJson;
    throw std::runtime_error("App tool not implemented: get_task");
}

std::string TaskBoardUI::queryTasks(std::string_view inputJson)
{
    (void)inputJson;
    throw std::runtime_error("App tool not implemented: query_tasks");
}

std::string TaskBoardUI::updateTask(std::string_view inputJson)
{
    (void)inputJson;
    throw std::runtime_error("App tool not implemented: update_task");
}

std::string TaskBoardUI::callAppTool(std::string_view name, std::string_view inputJson)
{
    if (name == "delete_task")
    {
        return deleteTask(inputJson);
    }
    else if (name == "get_task")
    {
        return getTask(inputJson);
    }
    else if (name == "query_tasks")
    {
        return queryTasks(inputJson);
    }
    else if (name == "update_task")
    {
        return updateTask(inputJson);
    }
    return ui::GeneratedWindowBase::callAppTool(name, inputJson);
}

bool TaskBoardUI::handleGeneratedButton(std::string_view name)
{
    if (name == "quit_btn")
    {
        onQuitBtnClick();
        return true;
    }
    else if (name == "reset_filters_btn")
    {
        onResetFiltersBtnClick();
        return true;
    }
    else if (name == "new_task_btn")
    {
        onNewTaskBtnClick();
        return true;
    }
    return false;
}

bool TaskBoardUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "search")
    {
        onSearchChange(value);
        return true;
    }
    return false;
}

bool TaskBoardUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "search")
    {
        onSearchSubmit(value);
        return true;
    }
    return false;
}

bool TaskBoardUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
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

ui::GeneratedWindowRuntimeOptions TaskBoardUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "quit_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "reset_filters_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "new_task_btn")
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
