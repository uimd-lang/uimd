#include "formular_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "title_label", .type = "label", .cellName = "", .relative = ui::Rect{1, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{48, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 24, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "name_label", .type = "label", .cellName = "", .relative = ui::Rect{3, 2, 10, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("auto", 10), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 40, .marginBottom = 22, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "name_input", .type = "textinput", .cellName = "", .relative = ui::Rect{3, 19, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 22, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "email_label", .type = "label", .cellName = "", .relative = ui::Rect{5, 2, 11, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 39, .marginBottom = 20, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "email_input", .type = "textinput", .cellName = "", .relative = ui::Rect{5, 19, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 20, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "age_label", .type = "label", .cellName = "", .relative = ui::Rect{7, 2, 9, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 41, .marginBottom = 18, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "age_input", .type = "numberinput", .cellName = "", .relative = ui::Rect{7, 19, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 18, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "country_label", .type = "label", .cellName = "", .relative = ui::Rect{9, 2, 13, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("auto", 13), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 37, .marginBottom = 16, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "country_combo", .type = "combobox", .cellName = "", .relative = ui::Rect{9, 19, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 16, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "\"Description\"", .type = "label", .cellName = "", .relative = ui::Rect{11, 2, 13, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("auto", 13), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 37, .marginBottom = 14, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "description_input", .type = "textarea", .cellName = "", .relative = ui::Rect{11, 19, 536870911, 4}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 4), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{31, 4}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 11},
        ui::GeneratedLayoutEntry{.name = "role_label", .type = "label", .cellName = "", .relative = ui::Rect{16, 2, 10, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("auto", 10), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 40, .marginBottom = 9, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "role_listbox", .type = "listbox", .cellName = "", .relative = ui::Rect{16, 19, 536870911, 4}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 4), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{31, 4}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 6, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "notify_check", .type = "checkbox", .cellName = "", .relative = ui::Rect{21, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{48, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 4, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#b8c4d8"}})},
        ui::GeneratedLayoutEntry{.name = "terms_check", .type = "checkbox", .cellName = "", .relative = ui::Rect{22, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{48, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 2, .marginBottom = 3, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#b8c4d8"}})},
        ui::GeneratedLayoutEntry{.name = "save_btn", .type = "button", .cellName = "", .relative = ui::Rect{24, 7, 14, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("auto", 14), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 31, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "cancel_btn", .type = "button", .cellName = "", .relative = ui::Rect{24, 27, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 52, 26}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 52), .cellHeight = dimension("auto", 26), .charsSize = ui::Size{17, 1}, .cellCharsSize = ui::Size{52, 26}, .marginRight = 8, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

FormularUI::FormularUI() : ui::GeneratedWindowBase("FormApp")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "Formular", R"UI_MCP_MD(# FormApp

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "FormApp UI source."
tags: [example]
```

## Members

```yaml
title_label:
  type: label
  text: "User profile"
  expose: false

name_label:
  type: label
  text: Name

name_input:
  type: textinput
  value: ""

email_label:
  type: label
  text: Email

email_input:
  type: textinput
  value: ""

age_label:
  type: label
  text: Age

age_input:
  type: numberinput

description_input:
  type: textarea

country_label:
  type: label
  text: Country

country_combo:
  type: combobox
  options: [Slovakia, "Czech Republic", Poland, Hungary, Austria]
  selected_item: Slovakia

role_label:
  type: label
  text: Role

role_listbox:
  type: listbox
  options: [Developer, Designer, Manager, QA, DevOps, Tester, Hacker]
  selected_items: [Developer]
  multiple: "true"

notify_check:
  type: checkbox
  title: "Email notifications"

terms_check:
  type: checkbox
  title: "I accept the terms and conditions"

save_btn:
  type: button
  title: Save

cancel_btn:
  type: button
  title: Cancel
```

## Style

```yaml
@title_label:
  text-align: center
```

## User Interface

```ui
+----------------------------------------------------+
|                                                    |
|  title_label.....................................  |
|                                                    |
|  name_label       name_input.....................  |
|                                                    |
|  email_label      email_input....................  |
|                                                    |
|  age_label        age_input......................  |
|                                                    |
|  country_label    country_combo..................  |
|                                                    |
|  "Description"    description_input..............  |
|                  ..............................    |
|                  ..............................    |
|                  ..............................    |
|                                                    |
|  role_label       role_listbox...................  |
|                  ..............................    |
|                  ..............................    |
|                  ..............................    |
|                                                    |
|  notify_check....................................  |
|  terms_check.....................................  |
|                                                    |
|       save_btn......      cancel_btn.......        |
|                                                    |
+----------------------------------------------------+
```
)UI_MCP_MD", "FormApp UI source.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"title_label", "User profile", false}, ui::GeneratedElementMetadata{"name_label", "Name", true}, ui::GeneratedElementMetadata{"name_input", "name_input", true}, ui::GeneratedElementMetadata{"email_label", "Email", true}, ui::GeneratedElementMetadata{"email_input", "email_input", true}, ui::GeneratedElementMetadata{"age_label", "Age", true}, ui::GeneratedElementMetadata{"age_input", "age_input", true}, ui::GeneratedElementMetadata{"description_input", "description_input", true}, ui::GeneratedElementMetadata{"country_label", "Country", true}, ui::GeneratedElementMetadata{"country_combo", "country_combo", true}, ui::GeneratedElementMetadata{"role_label", "Role", true}, ui::GeneratedElementMetadata{"role_listbox", "role_listbox", true}, ui::GeneratedElementMetadata{"notify_check", "Email notifications", true}, ui::GeneratedElementMetadata{"terms_check", "I accept the terms and conditions", true}, ui::GeneratedElementMetadata{"save_btn", "Save", true}, ui::GeneratedElementMetadata{"cancel_btn", "Cancel", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    title_label = &static_cast<ui::Label&>(addElement<ui::Label>("title_label", "User profile"));
    title_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}, {"text-align", "center"}}));
    title_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    name_label = &static_cast<ui::Label&>(addElement<ui::Label>("name_label", "Name"));
    name_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    name_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    name_input = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("name_input", "", 0));
    name_input->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    name_input->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    name_input->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    name_input->setCursorStyle(makeStyle({{"background", "#ffffff"}, {"color", "#0e1117"}}));
    name_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    name_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    email_label = &static_cast<ui::Label&>(addElement<ui::Label>("email_label", "Email"));
    email_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    email_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    email_input = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("email_input", "", 0));
    email_input->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    email_input->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    email_input->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    email_input->setCursorStyle(makeStyle({{"background", "#ffffff"}, {"color", "#0e1117"}}));
    email_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    email_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    age_label = &static_cast<ui::Label&>(addElement<ui::Label>("age_label", "Age"));
    age_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    age_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    age_input = &static_cast<ui::NumberInput&>(addElement<ui::NumberInput>("age_input", 0.0, 1.0));
    age_input->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    age_input->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    age_input->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    age_input->setCursorStyle(makeStyle({{"background", "#ffffff"}, {"color", "#0e1117"}}));
    age_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    description_input = &static_cast<ui::TextArea&>(addElement<ui::TextArea>("description_input", "", 0));
    description_input->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    description_input->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    description_input->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    description_input->setCursorStyle(makeStyle({{"background", "#ffffff"}, {"color", "#0e1117"}}));
    description_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    description_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    country_label = &static_cast<ui::Label&>(addElement<ui::Label>("country_label", "Country"));
    country_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    country_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    country_combo = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("country_combo", std::vector<std::string>{"Slovakia", "Czech Republic", "Poland", "Hungary", "Austria"}));
    country_combo->setSelectedIndex(0);
    country_combo->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    country_combo->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    country_combo->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#e5e7eb"}}));
    country_combo->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    country_combo->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    role_label = &static_cast<ui::Label&>(addElement<ui::Label>("role_label", "Role"));
    role_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    role_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    role_listbox = &static_cast<ui::ListBox&>(addElement<ui::ListBox>("role_listbox", std::vector<std::string>{"Developer", "Designer", "Manager", "QA", "DevOps", "Tester", "Hacker"}));
    role_listbox->setMultiple(true);
    role_listbox->setSelectedIndex(0);
    role_listbox->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#cbd5e1"}}));
    role_listbox->setFocusStyle(makeStyle({{"background", "#1d2f4d"}, {"color", "#ffffff"}}));
    role_listbox->setEditStyle(makeStyle({{"background", "#243a5c"}, {"color", "#cbd5e1"}}));
    role_listbox->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    role_listbox->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    notify_check = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("notify_check", "Email notifications", false));
    notify_check->setStyle(makeStyle({{"background", "transparent"}, {"color", "#b8c4d8"}}));
    notify_check->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#ffffff"}}));
    notify_check->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#44cc88"}}));
    notify_check->setUncheckedStyle(makeStyle({{"background", "transparent"}}));
    notify_check->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    terms_check = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("terms_check", "I accept the terms and conditions", false));
    terms_check->setStyle(makeStyle({{"background", "transparent"}, {"color", "#b8c4d8"}}));
    terms_check->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#ffffff"}}));
    terms_check->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#44cc88"}}));
    terms_check->setUncheckedStyle(makeStyle({{"background", "transparent"}}));
    terms_check->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    save_btn = &static_cast<ui::Button&>(addElement<ui::Button>("save_btn", "Save"));
    save_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    save_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    save_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    cancel_btn = &static_cast<ui::Button&>(addElement<ui::Button>("cancel_btn", "Cancel"));
    cancel_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    cancel_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    cancel_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void FormularUI::onNameInputChange(std::string_view value)
{
    (void)value;
}

void FormularUI::onNameInputSubmit(std::string_view value)
{
    (void)value;
}

void FormularUI::onEmailInputChange(std::string_view value)
{
    (void)value;
}

void FormularUI::onEmailInputSubmit(std::string_view value)
{
    (void)value;
}

void FormularUI::onAgeInputChange(std::string_view value)
{
    (void)value;
}

void FormularUI::onAgeInputSubmit(std::string_view value)
{
    (void)value;
}

void FormularUI::onDescriptionInputChange(std::string_view value)
{
    (void)value;
}

void FormularUI::onDescriptionInputSubmit(std::string_view value)
{
    (void)value;
}

void FormularUI::onCountryComboChange(std::string_view value)
{
    (void)value;
}

void FormularUI::onRoleListboxSelectionChange(const std::vector<std::string>& value)
{
    (void)value;
}

void FormularUI::onNotifyCheckChange(std::string_view value)
{
    (void)value;
}

void FormularUI::onTermsCheckChange(std::string_view value)
{
    (void)value;
}

void FormularUI::onSaveBtnClick()
{
}

void FormularUI::onCancelBtnClick()
{
}

bool FormularUI::shouldClose() const
{
    return false;
}

bool FormularUI::handleGeneratedButton(std::string_view name)
{
    if (name == "save_btn")
    {
        onSaveBtnClick();
        return true;
    }
    else if (name == "cancel_btn")
    {
        onCancelBtnClick();
        return true;
    }
    return false;
}

bool FormularUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "name_input")
    {
        onNameInputChange(value);
        return true;
    }
    else if (name == "email_input")
    {
        onEmailInputChange(value);
        return true;
    }
    else if (name == "age_input")
    {
        onAgeInputChange(value);
        return true;
    }
    else if (name == "description_input")
    {
        onDescriptionInputChange(value);
        return true;
    }
    else if (name == "notify_check")
    {
        onNotifyCheckChange(value);
        return true;
    }
    else if (name == "terms_check")
    {
        onTermsCheckChange(value);
        return true;
    }
    return false;
}

bool FormularUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "name_input")
    {
        onNameInputSubmit(value);
        return true;
    }
    else if (name == "email_input")
    {
        onEmailInputSubmit(value);
        return true;
    }
    else if (name == "age_input")
    {
        onAgeInputSubmit(value);
        return true;
    }
    else if (name == "description_input")
    {
        onDescriptionInputSubmit(value);
        return true;
    }
    return false;
}

bool FormularUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "country_combo")
    {
        onCountryComboChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    else if (name == "role_listbox")
    {
        onRoleListboxSelectionChange(value);
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions FormularUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "save_btn")
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
        if (name == "name_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "email_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "age_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "description_input")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "notify_check")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "terms_check")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "name_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "email_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "age_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "description_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "country_combo")
        {
            (void)handleGeneratedSelectionChanged(name, value);
        }
        else if (name == "role_listbox")
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
