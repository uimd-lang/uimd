#include "task_dialog_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header", .type = "label", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 54, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{54, 1}, .cellCharsSize = ui::Size{54, 1}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#1f2937"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "add_btn", .type = "button", .cellName = "", .relative = ui::Rect{0, 2, 23, 1}, .sourceCell = ui::Rect{20, 0, 54, 2}, .width = dimension("auto", 23), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{23, 1}, .cellCharsSize = ui::Size{54, 2}, .marginRight = 29, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "cancel_btn", .type = "button", .cellName = "", .relative = ui::Rect{0, 27, 536870911, 1}, .sourceCell = ui::Rect{20, 0, 54, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{24, 1}, .cellCharsSize = ui::Size{54, 2}, .marginRight = 3, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "name_label", .type = "label", .cellName = "", .relative = ui::Rect{1, 2, 10, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("auto", 10), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 42, .marginBottom = 15, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "name", .type = "textinput", .cellName = "", .relative = ui::Rect{1, 18, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{33, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 3, .marginBottom = 15, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "status_label", .type = "label", .cellName = "", .relative = ui::Rect{3, 2, 12, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 40, .marginBottom = 13, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "project_status", .type = "combobox", .cellName = "", .relative = ui::Rect{3, 18, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{33, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 3, .marginBottom = 13, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "owner_label", .type = "label", .cellName = "", .relative = ui::Rect{5, 2, 11, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 41, .marginBottom = 11, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "owner", .type = "combobox", .cellName = "", .relative = ui::Rect{5, 18, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{33, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 3, .marginBottom = 11, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "priority_label", .type = "label", .cellName = "", .relative = ui::Rect{7, 2, 14, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("auto", 14), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 38, .marginBottom = 9, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "priority", .type = "combobox", .cellName = "", .relative = ui::Rect{7, 18, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{33, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 3, .marginBottom = 9, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "note_label", .type = "label", .cellName = "", .relative = ui::Rect{9, 2, 11, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 41, .marginBottom = 7, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "note", .type = "textarea", .cellName = "", .relative = ui::Rect{9, 18, 536870911, 5}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("expanded", 536870911), .height = dimension("auto", 5), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{33, 5}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 3, .marginBottom = 3, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "message", .type = "label", .cellName = "", .relative = ui::Rect{15, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 54, 17}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 54), .cellHeight = dimension("auto", 17), .charsSize = ui::Size{49, 1}, .cellCharsSize = ui::Size{54, 17}, .marginRight = 3, .marginBottom = 1, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#fbbf24"}})},
    };
}

}  // namespace

TaskDialogUI::TaskDialogUI() : ui::GeneratedWindowBase("Task Dialog")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("dialog");
    setMcpMetadata(true, "TaskDialog", R"UI_MCP_MD(# Task Dialog

## Metadata

```yaml
format: uimd
format-version: 1
kind: dialog
status: draft
description: "Dialog form for creating or editing a task."
tags: [example, dialog]
```

## Members

```yaml
header:
  type: label
  text: Task
  description: "Dialog title."
  expose: false

name_label:
  type: label
  text: Title
  description: "Label for task title."
  expose: false

name:
  type: textinput
  value: ""
  maxlength: 60
  description: "Required task title."

status_label:
  type: label
  text: Status
  description: "Label for task status."
  expose: false

project_status:
  type: combobox
  options: [Todo, Doing, Blocked, Done]
  selected_item: Todo
  description: "Task status."

owner_label:
  type: label
  text: Assignee
  description: "Label for task assignee."
  expose: false

owner:
  type: combobox
  options: [Ava, Bruno, Chen, Dana]
  selected_item: Ava
  description: "Task assignee."

priority_label:
  type: label
  text: Priority
  description: "Label for task priority."
  expose: false

priority:
  type: combobox
  options: [Low, Med, High, Urgent]
  selected_item: Med
  description: "Task priority."

note_label:
  type: label
  text: Description
  description: "Label for task description."
  expose: false

note:
  type: textarea
  value: ""
  maxlength: 160
  description: "Task description."

message:
  type: label
  text: ""
  description: "Validation message."

add_btn:
  type: button
  title: Save task
  description: "Save the task and close the dialog."

cancel_btn:
  type: button
  title: Cancel
  description: "Close the dialog without saving the task."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  text-align: center
  background: "#1f2937"
  color: "#ffffff"
textinput:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  cursor-background: "#facc15"
  cursor-color: "#111827"
textarea:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  cursor-background: "#facc15"
  cursor-color: "#111827"
combobox:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@add_btn:
  background: "#14532d"
  focus-background: "#16a34a"
@message:
  color: "#fbbf24"
```

## User Interface

```ui
+------------------------------------------------------+
|header................................................|
+------------------------------------------------------+
|                                                      |
|  name_label      name.............................   |
|                                                      |
|  status_label    project_status...................   |
|                                                      |
|  owner_label     owner............................   |
|                                                      |
|  priority_label  priority.........................   |
|                                                      |
|  note_label.     note.............................   |
|                  ...............................     |
|                  ...............................     |
|                  ...............................     |
|                  ...............................     |
|                                                      |
|  message..........................................   |
|                                                      |
+------------------------------------------------------+
|  add_btn................  cancel_btn..............   |
|                                                      |
+------------------------------------------------------+
```
)UI_MCP_MD", "Dialog form for creating or editing a task.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header", "Dialog title.", false}, ui::GeneratedElementMetadata{"name_label", "Label for task title.", false}, ui::GeneratedElementMetadata{"name", "Required task title.", true}, ui::GeneratedElementMetadata{"status_label", "Label for task status.", false}, ui::GeneratedElementMetadata{"project_status", "Task status.", true}, ui::GeneratedElementMetadata{"owner_label", "Label for task assignee.", false}, ui::GeneratedElementMetadata{"owner", "Task assignee.", true}, ui::GeneratedElementMetadata{"priority_label", "Label for task priority.", false}, ui::GeneratedElementMetadata{"priority", "Task priority.", true}, ui::GeneratedElementMetadata{"note_label", "Label for task description.", false}, ui::GeneratedElementMetadata{"note", "Task description.", true}, ui::GeneratedElementMetadata{"message", "Validation message.", true}, ui::GeneratedElementMetadata{"add_btn", "Save the task and close the dialog.", true}, ui::GeneratedElementMetadata{"cancel_btn", "Close the dialog without saving the task.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header = &static_cast<ui::Label&>(addElement<ui::Label>("header", "Task"));
    header->setStyle(makeStyle({{"background", "#1f2937"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    name_label = &static_cast<ui::Label&>(addElement<ui::Label>("name_label", "Title"));
    name_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    name_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    name = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("name", "", 60));
    name->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    name->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    name->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    name->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    name->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    name->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    status_label = &static_cast<ui::Label&>(addElement<ui::Label>("status_label", "Status"));
    status_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    status_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    project_status = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("project_status", std::vector<std::string>{"Todo", "Doing", "Blocked", "Done"}));
    project_status->setSelectedIndex(0);
    project_status->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    project_status->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    project_status->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#e5e7eb"}}));
    project_status->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    project_status->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    owner_label = &static_cast<ui::Label&>(addElement<ui::Label>("owner_label", "Assignee"));
    owner_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    owner_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    owner = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("owner", std::vector<std::string>{"Ava", "Bruno", "Chen", "Dana"}));
    owner->setSelectedIndex(0);
    owner->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    owner->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    owner->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#e5e7eb"}}));
    owner->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    owner->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    priority_label = &static_cast<ui::Label&>(addElement<ui::Label>("priority_label", "Priority"));
    priority_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    priority_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    priority = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("priority", std::vector<std::string>{"Low", "Med", "High", "Urgent"}));
    priority->setSelectedIndex(1);
    priority->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    priority->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    priority->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#e5e7eb"}}));
    priority->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    priority->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    note_label = &static_cast<ui::Label&>(addElement<ui::Label>("note_label", "Description"));
    note_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    note_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    note = &static_cast<ui::TextArea&>(addElement<ui::TextArea>("note", "", 160));
    note->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    note->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    note->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    note->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    note->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    note->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    message = &static_cast<ui::Label&>(addElement<ui::Label>("message", ""));
    message->setStyle(makeStyle({{"background", "transparent"}, {"color", "#fbbf24"}}));
    message->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    add_btn = &static_cast<ui::Button&>(addElement<ui::Button>("add_btn", "Save task"));
    add_btn->setStyle(makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}}));
    add_btn->setFocusStyle(makeStyle({{"background", "#16a34a"}, {"color", "#ffffff"}}));
    add_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    cancel_btn = &static_cast<ui::Button&>(addElement<ui::Button>("cancel_btn", "Cancel"));
    cancel_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    cancel_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    cancel_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void TaskDialogUI::onNameChange(std::string_view value)
{
    (void)value;
}

void TaskDialogUI::onNameSubmit(std::string_view value)
{
    (void)value;
}

void TaskDialogUI::onProjectStatusChange(std::string_view value)
{
    (void)value;
}

void TaskDialogUI::onOwnerChange(std::string_view value)
{
    (void)value;
}

void TaskDialogUI::onPriorityChange(std::string_view value)
{
    (void)value;
}

void TaskDialogUI::onNoteChange(std::string_view value)
{
    (void)value;
}

void TaskDialogUI::onNoteSubmit(std::string_view value)
{
    (void)value;
}

void TaskDialogUI::onAddBtnClick()
{
}

void TaskDialogUI::onCancelBtnClick()
{
}

bool TaskDialogUI::shouldClose() const
{
    return false;
}

bool TaskDialogUI::handleGeneratedButton(std::string_view name)
{
    if (name == "add_btn")
    {
        onAddBtnClick();
        return true;
    }
    else if (name == "cancel_btn")
    {
        onCancelBtnClick();
        return true;
    }
    return false;
}

bool TaskDialogUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "name")
    {
        onNameChange(value);
        return true;
    }
    else if (name == "note")
    {
        onNoteChange(value);
        return true;
    }
    return false;
}

bool TaskDialogUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "name")
    {
        onNameSubmit(value);
        return true;
    }
    else if (name == "note")
    {
        onNoteSubmit(value);
        return true;
    }
    return false;
}

bool TaskDialogUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "project_status")
    {
        onProjectStatusChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    else if (name == "owner")
    {
        onOwnerChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    else if (name == "priority")
    {
        onPriorityChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions TaskDialogUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "add_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "cancel_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "name")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "note")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "name")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "note")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "project_status")
        {
            (void)handleGeneratedSelectionChanged(name, value);
        }
        else if (name == "owner")
        {
            (void)handleGeneratedSelectionChanged(name, value);
        }
        else if (name == "priority")
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
