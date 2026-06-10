#include "activity_feed_ui.hpp"
#include "activity_feed_panel/activity_feed_panel_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header_title", .type = "label", .cellName = "header", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 65, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{33, 1}, .cellCharsSize = ui::Size{65, 1}, .marginRight = 32, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "add_button", .type = "button", .cellName = "actions", .relative = ui::Rect{0, 1, 19, 1}, .sourceCell = ui::Rect{16, 0, 65, 1}, .width = dimension("auto", 19), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{19, 1}, .cellCharsSize = ui::Size{65, 1}, .marginRight = 45, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "feed", .type = "activity_feed_panel", .cellName = "content", .relative = ui::Rect{0, 1, 536870911, 536870911}, .sourceCell = ui::Rect{2, 0, 65, 7}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{63, 7}, .cellCharsSize = ui::Size{65, 7}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#030712"}}), .elementStyle = makeStyle({{"background", "#00000000"}})},
        ui::GeneratedLayoutEntry{.name = "status", .type = "label", .cellName = "", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{18, 0, 65, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 65), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{63, 1}, .cellCharsSize = ui::Size{65, 1}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "type_label", .type = "label", .cellName = "controls", .relative = ui::Rect{0, 1, 10, 1}, .sourceCell = ui::Rect{10, 0, 65, 5}, .width = dimension("auto", 10), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 5), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{65, 5}, .marginRight = 54, .marginBottom = 4, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "activity_type", .type = "combobox", .cellName = "controls", .relative = ui::Rect{0, 16, 22, 1}, .sourceCell = ui::Rect{10, 0, 65, 5}, .width = dimension("auto", 22), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 5), .charsSize = ui::Size{22, 1}, .cellCharsSize = ui::Size{65, 5}, .marginRight = 27, .marginBottom = 4, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "seed_button", .type = "button", .cellName = "actions", .relative = ui::Rect{0, 21, 20, 1}, .sourceCell = ui::Rect{16, 0, 65, 1}, .width = dimension("auto", 20), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{65, 1}, .marginRight = 24, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "settings_button", .type = "button", .cellName = "header", .relative = ui::Rect{0, 34, 17, 1}, .sourceCell = ui::Rect{0, 0, 65, 1}, .width = dimension("auto", 17), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{17, 1}, .cellCharsSize = ui::Size{65, 1}, .marginRight = 14, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "clear_button", .type = "button", .cellName = "actions", .relative = ui::Rect{0, 42, 22, 1}, .sourceCell = ui::Rect{16, 0, 65, 1}, .width = dimension("auto", 22), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{22, 1}, .cellCharsSize = ui::Size{65, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "quit_button", .type = "button", .cellName = "header", .relative = ui::Rect{0, 52, 13, 1}, .sourceCell = ui::Rect{0, 0, 65, 1}, .width = dimension("auto", 13), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{65, 1}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "message_label", .type = "label", .cellName = "controls", .relative = ui::Rect{2, 1, 13, 1}, .sourceCell = ui::Rect{10, 0, 65, 5}, .width = dimension("auto", 13), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 5), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{65, 5}, .marginRight = 51, .marginBottom = 2, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "message", .type = "textarea", .cellName = "controls", .relative = ui::Rect{2, 16, 536870911, 3}, .sourceCell = ui::Rect{10, 0, 65, 5}, .width = dimension("expanded", 536870911), .height = dimension("auto", 3), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 5), .charsSize = ui::Size{47, 3}, .cellCharsSize = ui::Size{65, 5}, .marginRight = 2, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}})},
    };
}

}  // namespace

ActivityFeedUI::ActivityFeedUI() : ui::GeneratedWindowBase("Activity Feed")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "ActivityFeed", R"UI_MCP_MD(# Activity Feed

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "Activity feed example with dynamically appended scroll view items and a settings dialog."
tags: [example]
```

## Definition

```yaml
uses:
  - activity_feed_panel/activity_feed_panel
  - settings/settings
```

## Members

```yaml
header_title:
  type: label
  text: "Activity Feed"
  description: "Application title."
  expose: false

settings_button:
  type: button
  title: Settings
  description: "Open the activity feed settings dialog."

quit_button:
  type: button
  title: Quit
  description: "Close the activity feed example."

feed:
  type: activity_feed_panel
  description: "Scrollable activity timeline."

type_label:
  type: label
  text: Type
  description: "Label for activity type selector."
  expose: false

activity_type:
  type: combobox
  options: [Info, Task, Warning, Deploy, Note]
  selected_item: Info
  description: "Activity type selector."

message_label:
  type: label
  text: Message
  description: "Label for activity message input."
  expose: false

message:
  type: textarea
  value: ""
  maxlength: 180
  description: "Activity message input."

add_button:
  type: button
  title: Add
  description: "Append one activity item to the feed."

seed_button:
  type: button
  title: Seed
  description: "Append sample activity items to the feed."

clear_button:
  type: button
  title: Clear
  description: "Remove all activity items from the feed."

status:
  type: label
  text: "Ready"
  description: "Shows activity feed status."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  background: "#1f2937"
@header_title:
  text-align: center
  color: "#ffffff"
@content:
  background: "#030712"
@feed:
  background: "#00000000"
@controls:
  background: "#172033"
  padding: 1, 1, 1, 1
@actions:
  background: "#172033"
  padding: 1, 1, 1, 1
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@clear_button:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@quit_button:
  background: "#7f1d1d"
  focus-background: "#dc2626"
textinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
textarea:
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
@status:
  background: "#0f172a"
  color: "#93c5fd"
```

## User Interface

```ui
+-header-**-------------------------------------------------------+
|header_title..**................. settings_button.. quit_button..|
+-content-**------------------------------------------------------+
| feed..**....................................................... |
| ............................................................... |
| ............................................................... |
* *.............................................................. |
* *.............................................................. |
* *.............................................................. |
| ............................................................... |
+-controls-**-----------------------------------------------------+
| type_label     activity_type.........                           |
|                                                                 |
| message_label  message..**....................................  |
|               ...............................................   |
|               ...............................................   |
+-actions-**------------------------------------------------------+
| add_button......... seed_button......... clear_button.......... |
+-----------------------------------------------------------------+
| status..**..................................................... |
+-----------------------------------------------------------------+
```
)UI_MCP_MD", "Activity feed example with dynamically appended scroll view items and a settings dialog.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header_title", "Application title.", false}, ui::GeneratedElementMetadata{"settings_button", "Open the activity feed settings dialog.", true}, ui::GeneratedElementMetadata{"quit_button", "Close the activity feed example.", true}, ui::GeneratedElementMetadata{"feed", "Scrollable activity timeline.", true}, ui::GeneratedElementMetadata{"type_label", "Label for activity type selector.", false}, ui::GeneratedElementMetadata{"activity_type", "Activity type selector.", true}, ui::GeneratedElementMetadata{"message_label", "Label for activity message input.", false}, ui::GeneratedElementMetadata{"message", "Activity message input.", true}, ui::GeneratedElementMetadata{"add_button", "Append one activity item to the feed.", true}, ui::GeneratedElementMetadata{"seed_button", "Append sample activity items to the feed.", true}, ui::GeneratedElementMetadata{"clear_button", "Remove all activity items from the feed.", true}, ui::GeneratedElementMetadata{"status", "Shows activity feed status.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header_title = &static_cast<ui::Label&>(addElement<ui::Label>("header_title", "Activity Feed"));
    header_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    settings_button = &static_cast<ui::Button&>(addElement<ui::Button>("settings_button", "Settings"));
    settings_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    settings_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    settings_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    quit_button = &static_cast<ui::Button&>(addElement<ui::Button>("quit_button", "Quit"));
    quit_button->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    quit_button->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    quit_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    feed = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("feed"));
    feed->setChild(std::make_unique<ActivityFeedPanelUI>());
    feed->setStyle(makeStyle({{"background", "#00000000"}}));
    feed->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    feed->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    feed->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    type_label = &static_cast<ui::Label&>(addElement<ui::Label>("type_label", "Type"));
    type_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    type_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    activity_type = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("activity_type", std::vector<std::string>{"Info", "Task", "Warning", "Deploy", "Note"}));
    activity_type->setSelectedIndex(0);
    activity_type->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    activity_type->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    activity_type->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    activity_type->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    activity_type->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    message_label = &static_cast<ui::Label&>(addElement<ui::Label>("message_label", "Message"));
    message_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    message_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    message = &static_cast<ui::TextArea&>(addElement<ui::TextArea>("message", "", 180));
    message->setStyle(makeStyle({{"background", "#0b1220"}, {"color", "#e5e7eb"}}));
    message->setFocusStyle(makeStyle({{"background", "#25364f"}, {"color", "#ffffff"}}));
    message->setEditStyle(makeStyle({{"background", "#0b1220"}, {"color", "#ffffff"}}));
    message->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    message->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    message->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    add_button = &static_cast<ui::Button&>(addElement<ui::Button>("add_button", "Add"));
    add_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    add_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    add_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    seed_button = &static_cast<ui::Button&>(addElement<ui::Button>("seed_button", "Seed"));
    seed_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    seed_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    seed_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    clear_button = &static_cast<ui::Button&>(addElement<ui::Button>("clear_button", "Clear"));
    clear_button->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    clear_button->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    clear_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    status = &static_cast<ui::Label&>(addElement<ui::Label>("status", "Ready"));
    status->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#93c5fd"}}));
    status->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void ActivityFeedUI::onSettingsButtonClick()
{
}

void ActivityFeedUI::onQuitButtonClick()
{
}

void ActivityFeedUI::onActivityTypeChange(std::string_view value)
{
    (void)value;
}

void ActivityFeedUI::onMessageChange(std::string_view value)
{
    (void)value;
}

void ActivityFeedUI::onMessageSubmit(std::string_view value)
{
    (void)value;
}

void ActivityFeedUI::onAddButtonClick()
{
}

void ActivityFeedUI::onSeedButtonClick()
{
}

void ActivityFeedUI::onClearButtonClick()
{
}

bool ActivityFeedUI::shouldClose() const
{
    return false;
}

bool ActivityFeedUI::handleGeneratedButton(std::string_view name)
{
    if (name == "settings_button")
    {
        onSettingsButtonClick();
        return true;
    }
    else if (name == "quit_button")
    {
        onQuitButtonClick();
        return true;
    }
    else if (name == "add_button")
    {
        onAddButtonClick();
        return true;
    }
    else if (name == "seed_button")
    {
        onSeedButtonClick();
        return true;
    }
    else if (name == "clear_button")
    {
        onClearButtonClick();
        return true;
    }
    return false;
}

bool ActivityFeedUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "message")
    {
        onMessageChange(value);
        return true;
    }
    return false;
}

bool ActivityFeedUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "message")
    {
        onMessageSubmit(value);
        return true;
    }
    return false;
}

bool ActivityFeedUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "activity_type")
    {
        onActivityTypeChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions ActivityFeedUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "settings_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "quit_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "add_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "seed_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "clear_button")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "message")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "message")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "activity_type")
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
