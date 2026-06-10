#include "settings_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header_title", .type = "label", .cellName = "header", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 42, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 42), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{42, 1}, .cellCharsSize = ui::Size{42, 1}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f2937"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "save_button", .type = "button", .cellName = "buttons", .relative = ui::Rect{0, 2, 17, 1}, .sourceCell = ui::Rect{10, 0, 42, 1}, .width = dimension("auto", 17), .height = dimension("auto", 1), .cellWidth = dimension("auto", 42), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{17, 1}, .cellCharsSize = ui::Size{42, 1}, .marginRight = 23, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "0, 2, 1, 2"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "cancel_button", .type = "button", .cellName = "buttons", .relative = ui::Rect{0, 21, 536870911, 1}, .sourceCell = ui::Rect{10, 0, 42, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 42), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{19, 1}, .cellCharsSize = ui::Size{42, 1}, .marginRight = 2, .marginBottom = 0, .cellStyle = makeStyle({{"padding", "0, 2, 1, 2"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "auto_scroll", .type = "checkbox", .cellName = "content", .relative = ui::Rect{1, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 42, 7}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 42), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{38, 1}, .cellCharsSize = ui::Size{42, 7}, .marginRight = 2, .marginBottom = 5, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "show_timestamps", .type = "checkbox", .cellName = "content", .relative = ui::Rect{3, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 42, 7}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 42), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{38, 1}, .cellCharsSize = ui::Size{42, 7}, .marginRight = 2, .marginBottom = 3, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}})},
        ui::GeneratedLayoutEntry{.name = "default_type_label", .type = "label", .cellName = "content", .relative = ui::Rect{5, 2, 20, 1}, .sourceCell = ui::Rect{2, 0, 42, 7}, .width = dimension("auto", 20), .height = dimension("auto", 1), .cellWidth = dimension("auto", 42), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{42, 7}, .marginRight = 20, .marginBottom = 1, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "default_type", .type = "combobox", .cellName = "content", .relative = ui::Rect{5, 24, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 42, 7}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 42), .cellHeight = dimension("auto", 7), .charsSize = ui::Size{16, 1}, .cellCharsSize = ui::Size{42, 7}, .marginRight = 2, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
    };
}

}  // namespace

SettingsUI::SettingsUI() : ui::GeneratedWindowBase("Activity Feed Settings")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "Settings", R"UI_MCP_MD(# Activity Feed Settings

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "Settings dialog for activity feed auto-scroll, timestamps, and default activity type."
tags: [example]
```

## Members

```yaml
header_title:
  type: label
  text: "Activity Feed Settings"
  description: "Dialog title."
  expose: false

auto_scroll:
  type: checkbox
  title: "Auto-scroll to newest"
  value: 1
  description: "Toggle whether new activity items scroll into view."

show_timestamps:
  type: checkbox
  title: "Show timestamps"
  value: 1
  description: "Toggle whether newly added activity items include timestamps."

default_type_label:
  type: label
  text: "Default type"
  description: "Label for the default activity type selector."
  expose: false

default_type:
  type: combobox
  options: [Info, Task, Warning, Deploy, Note]
  selected_item: Info
  description: "Default activity type used by the main window."

save_button:
  type: button
  title: Save
  description: "Apply settings and close the dialog."

cancel_button:
  type: button
  title: Cancel
  description: "Close the dialog without applying settings."
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
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
combobox:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
checkbox:
  color: "#d1d5db"
  checked-color: "#22c55e"
  unchecked-color: "#f97316"
@buttons:
  padding: 0, 2, 1, 2
```

## User Interface

```ui
+-header-----------------------------------+
|header_title..**..........................|
+-content----------------------------------+
|                                          |
|  auto_scroll...........................  |
|                                          |
|  show_timestamps.......................  |
|                                          |
|  default_type_label..  default_type....  |
|                                          |
+-buttons----------------------------------+
|  save_button......  cancel_button......  |
+------------------------------------------+
```
)UI_MCP_MD", "Settings dialog for activity feed auto-scroll, timestamps, and default activity type.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header_title", "Dialog title.", false}, ui::GeneratedElementMetadata{"auto_scroll", "Toggle whether new activity items scroll into view.", true}, ui::GeneratedElementMetadata{"show_timestamps", "Toggle whether newly added activity items include timestamps.", true}, ui::GeneratedElementMetadata{"default_type_label", "Label for the default activity type selector.", false}, ui::GeneratedElementMetadata{"default_type", "Default activity type used by the main window.", true}, ui::GeneratedElementMetadata{"save_button", "Apply settings and close the dialog.", true}, ui::GeneratedElementMetadata{"cancel_button", "Close the dialog without applying settings.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header_title = &static_cast<ui::Label&>(addElement<ui::Label>("header_title", "Activity Feed Settings"));
    header_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    auto_scroll = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("auto_scroll", "Auto-scroll to newest", true));
    auto_scroll->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    auto_scroll->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#ffffff"}}));
    auto_scroll->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#22c55e"}}));
    auto_scroll->setUncheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#f97316"}}));
    auto_scroll->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    show_timestamps = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("show_timestamps", "Show timestamps", true));
    show_timestamps->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}}));
    show_timestamps->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#ffffff"}}));
    show_timestamps->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#22c55e"}}));
    show_timestamps->setUncheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#f97316"}}));
    show_timestamps->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    default_type_label = &static_cast<ui::Label&>(addElement<ui::Label>("default_type_label", "Default type"));
    default_type_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    default_type_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    default_type = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("default_type", std::vector<std::string>{"Info", "Task", "Warning", "Deploy", "Note"}));
    default_type->setSelectedIndex(0);
    default_type->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    default_type->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    default_type->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#e5e7eb"}}));
    default_type->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    default_type->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    save_button = &static_cast<ui::Button&>(addElement<ui::Button>("save_button", "Save"));
    save_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    save_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    save_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    cancel_button = &static_cast<ui::Button&>(addElement<ui::Button>("cancel_button", "Cancel"));
    cancel_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    cancel_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    cancel_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void SettingsUI::onAutoScrollChange(std::string_view value)
{
    (void)value;
}

void SettingsUI::onShowTimestampsChange(std::string_view value)
{
    (void)value;
}

void SettingsUI::onDefaultTypeChange(std::string_view value)
{
    (void)value;
}

void SettingsUI::onSaveButtonClick()
{
}

void SettingsUI::onCancelButtonClick()
{
}

bool SettingsUI::shouldClose() const
{
    return false;
}

bool SettingsUI::handleGeneratedButton(std::string_view name)
{
    if (name == "save_button")
    {
        onSaveButtonClick();
        return true;
    }
    else if (name == "cancel_button")
    {
        onCancelButtonClick();
        return true;
    }
    return false;
}

bool SettingsUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "auto_scroll")
    {
        onAutoScrollChange(value);
        return true;
    }
    else if (name == "show_timestamps")
    {
        onShowTimestampsChange(value);
        return true;
    }
    return false;
}

bool SettingsUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool SettingsUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "default_type")
    {
        onDefaultTypeChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions SettingsUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "save_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "cancel_button")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "auto_scroll")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "show_timestamps")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "default_type")
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
