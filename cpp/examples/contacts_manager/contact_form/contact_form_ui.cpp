#include "contact_form_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header", .type = "label", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 58, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{58, 1}, .cellCharsSize = ui::Size{58, 1}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#1f2937"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "save_btn", .type = "button", .cellName = "", .relative = ui::Rect{0, 2, 26, 1}, .sourceCell = ui::Rect{26, 0, 58, 2}, .width = dimension("auto", 26), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{58, 2}, .marginRight = 30, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "cancel_btn", .type = "button", .cellName = "", .relative = ui::Rect{0, 30, 536870911, 1}, .sourceCell = ui::Rect{26, 0, 58, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{26, 1}, .cellCharsSize = ui::Size{58, 2}, .marginRight = 2, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "first_label", .type = "label", .cellName = "", .relative = ui::Rect{1, 2, 11, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 45, .marginBottom = 21, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "first_name", .type = "textinput", .cellName = "", .relative = ui::Rect{1, 17, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{39, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 21, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "last_label", .type = "label", .cellName = "", .relative = ui::Rect{3, 2, 10, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("auto", 10), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 46, .marginBottom = 19, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "last_name", .type = "textinput", .cellName = "", .relative = ui::Rect{3, 17, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{39, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 19, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "email_label", .type = "label", .cellName = "", .relative = ui::Rect{5, 2, 11, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 45, .marginBottom = 17, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "email", .type = "textinput", .cellName = "", .relative = ui::Rect{5, 17, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{39, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 17, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "phone_label", .type = "label", .cellName = "", .relative = ui::Rect{7, 2, 11, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 45, .marginBottom = 15, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "phone", .type = "textinput", .cellName = "", .relative = ui::Rect{7, 17, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{39, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 15, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "company_label", .type = "label", .cellName = "", .relative = ui::Rect{9, 2, 13, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("auto", 13), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 43, .marginBottom = 13, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "company", .type = "textinput", .cellName = "", .relative = ui::Rect{9, 17, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{39, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 13, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "city_label", .type = "label", .cellName = "", .relative = ui::Rect{11, 2, 10, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("auto", 10), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 46, .marginBottom = 11, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "city", .type = "textinput", .cellName = "", .relative = ui::Rect{11, 17, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{39, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 11, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "category_label", .type = "label", .cellName = "", .relative = ui::Rect{13, 2, 14, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("auto", 14), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 42, .marginBottom = 9, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "category", .type = "combobox", .cellName = "", .relative = ui::Rect{13, 18, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{38, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 9, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "active", .type = "checkbox", .cellName = "", .relative = ui::Rect{15, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{54, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 7, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#b8c4d8"}})},
        ui::GeneratedLayoutEntry{.name = "notes_label", .type = "label", .cellName = "", .relative = ui::Rect{17, 2, 11, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 45, .marginBottom = 5, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "notes", .type = "textarea", .cellName = "", .relative = ui::Rect{17, 14, 536870911, 3}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 3), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{42, 3}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 3, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "message", .type = "label", .cellName = "", .relative = ui::Rect{21, 2, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 58, 23}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 58), .cellHeight = dimension("auto", 23), .charsSize = ui::Size{54, 1}, .cellCharsSize = ui::Size{58, 23}, .marginRight = 2, .marginBottom = 1, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#fbbf24"}})},
    };
}

}  // namespace

ContactFormUI::ContactFormUI() : ui::GeneratedWindowBase("Contact Form")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "ContactForm", R"UI_MCP_MD(# Contact Form

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback."
tags: [example]
```

## Members

```yaml
header:
  type: label
  text: Contact
  description: "Dialog title showing that a contact is being edited."
  expose: false

first_label:
  type: label
  text: "First name"
  description: "Label for the first name input."

first_name:
  type: textinput
  value: ""
  maxlength: 80
  description: "Required first name field."

last_label:
  type: label
  text: "Last name"
  description: "Label for the last name input."

last_name:
  type: textinput
  value: ""
  maxlength: 80
  description: "Required last name field."

email_label:
  type: label
  text: Email
  description: "Label for the email input."

email:
  type: textinput
  value: ""
  maxlength: 120
  description: "Required email address field."

phone_label:
  type: label
  text: Phone
  description: "Label for the phone input."

phone:
  type: textinput
  value: ""
  maxlength: 80
  description: "Phone number field."

company_label:
  type: label
  text: Company
  description: "Label for the company input."

company:
  type: textinput
  value: ""
  maxlength: 120
  description: "Company name field."

city_label:
  type: label
  text: City
  description: "Label for the city input."

city:
  type: textinput
  value: ""
  maxlength: 80
  description: "City field."

category_label:
  type: label
  text: Category
  description: "Label for the contact category selector."

category:
  type: combobox
  options: [Client, Vendor, Partner, Personal]
  selected_item: Client
  description: "Contact category combobox with client, vendor, partner, and personal options."

active:
  type: checkbox
  title: "Active contact"
  value: 1
  description: "Toggle whether the contact is active."

notes_label:
  type: label
  text: Notes
  description: "Label for the notes field."

notes:
  type: textarea
  value: ""
  maxlength: 300
  description: "Multiline notes field for the contact."

save_btn:
  type: button
  title: Save
  description: "Validate and save the contact form."

cancel_btn:
  type: button
  title: Cancel
  description: "Close the form without saving changes."

message:
  type: label
  text: "Required: first name, last name, email."
  description: "Validation and status message area."
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
@message:
  color: "#fbbf24"
```

## User Interface

```ui
+----------------------------------------------------------+
|header....................................................|
+----------------------------------------------------------+
|                                                          |
|  first_label    first_name.............................  |
|                                                          |
|  last_label     last_name..............................  |
|                                                          |
|  email_label    email..................................  |
|                                                          |
|  phone_label    phone..................................  |
|                                                          |
|  company_label  company................................  |
|                                                          |
|  city_label     city...................................  |
|                                                          |
|  category_label  category..............................  |
|                                                          |
|  active................................................  |
|                                                          |
|  notes_label notes.....................................  |
|              ..........................................  |
|              ..........................................  |
|                                                          |
|  message...............................................  |
|                                                          |
+----------------------------------------------------------+
|  save_btn..................  cancel_btn................  |
|                                                          |
+----------------------------------------------------------+
```
)UI_MCP_MD", "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header", "Dialog title showing that a contact is being edited.", false}, ui::GeneratedElementMetadata{"first_label", "Label for the first name input.", true}, ui::GeneratedElementMetadata{"first_name", "Required first name field.", true}, ui::GeneratedElementMetadata{"last_label", "Label for the last name input.", true}, ui::GeneratedElementMetadata{"last_name", "Required last name field.", true}, ui::GeneratedElementMetadata{"email_label", "Label for the email input.", true}, ui::GeneratedElementMetadata{"email", "Required email address field.", true}, ui::GeneratedElementMetadata{"phone_label", "Label for the phone input.", true}, ui::GeneratedElementMetadata{"phone", "Phone number field.", true}, ui::GeneratedElementMetadata{"company_label", "Label for the company input.", true}, ui::GeneratedElementMetadata{"company", "Company name field.", true}, ui::GeneratedElementMetadata{"city_label", "Label for the city input.", true}, ui::GeneratedElementMetadata{"city", "City field.", true}, ui::GeneratedElementMetadata{"category_label", "Label for the contact category selector.", true}, ui::GeneratedElementMetadata{"category", "Contact category combobox with client, vendor, partner, and personal options.", true}, ui::GeneratedElementMetadata{"active", "Toggle whether the contact is active.", true}, ui::GeneratedElementMetadata{"notes_label", "Label for the notes field.", true}, ui::GeneratedElementMetadata{"notes", "Multiline notes field for the contact.", true}, ui::GeneratedElementMetadata{"save_btn", "Validate and save the contact form.", true}, ui::GeneratedElementMetadata{"cancel_btn", "Close the form without saving changes.", true}, ui::GeneratedElementMetadata{"message", "Validation and status message area.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header = &static_cast<ui::Label&>(addElement<ui::Label>("header", "Contact"));
    header->setStyle(makeStyle({{"background", "#1f2937"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    first_label = &static_cast<ui::Label&>(addElement<ui::Label>("first_label", "First name"));
    first_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    first_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    first_name = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("first_name", "", 80));
    first_name->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    first_name->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    first_name->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    first_name->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    first_name->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    first_name->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    last_label = &static_cast<ui::Label&>(addElement<ui::Label>("last_label", "Last name"));
    last_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    last_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    last_name = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("last_name", "", 80));
    last_name->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    last_name->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    last_name->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    last_name->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    last_name->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    last_name->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    email_label = &static_cast<ui::Label&>(addElement<ui::Label>("email_label", "Email"));
    email_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    email_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    email = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("email", "", 120));
    email->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    email->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    email->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    email->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    email->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    email->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    phone_label = &static_cast<ui::Label&>(addElement<ui::Label>("phone_label", "Phone"));
    phone_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    phone_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    phone = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("phone", "", 80));
    phone->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    phone->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    phone->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    phone->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    phone->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    phone->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    company_label = &static_cast<ui::Label&>(addElement<ui::Label>("company_label", "Company"));
    company_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    company_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    company = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("company", "", 120));
    company->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    company->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    company->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    company->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    company->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    company->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    city_label = &static_cast<ui::Label&>(addElement<ui::Label>("city_label", "City"));
    city_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    city_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    city = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("city", "", 80));
    city->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    city->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    city->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    city->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    city->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    city->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    category_label = &static_cast<ui::Label&>(addElement<ui::Label>("category_label", "Category"));
    category_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    category_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    category = &static_cast<ui::ComboBox&>(addElement<ui::ComboBox>("category", std::vector<std::string>{"Client", "Vendor", "Partner", "Personal"}));
    category->setSelectedIndex(0);
    category->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    category->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    category->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#e5e7eb"}}));
    category->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    category->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    active = &static_cast<ui::CheckBox&>(addElement<ui::CheckBox>("active", "Active contact", true));
    active->setStyle(makeStyle({{"background", "transparent"}, {"color", "#b8c4d8"}}));
    active->setFocusStyle(makeStyle({{"background", "#ffffff44"}, {"color", "#ffffff"}}));
    active->setCheckedStyle(makeStyle({{"background", "transparent"}, {"color", "#44cc88"}}));
    active->setUncheckedStyle(makeStyle({{"background", "transparent"}}));
    active->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    notes_label = &static_cast<ui::Label&>(addElement<ui::Label>("notes_label", "Notes"));
    notes_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    notes_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    notes = &static_cast<ui::TextArea&>(addElement<ui::TextArea>("notes", "", 300));
    notes->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    notes->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    notes->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    notes->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    notes->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    notes->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    save_btn = &static_cast<ui::Button&>(addElement<ui::Button>("save_btn", "Save"));
    save_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    save_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    save_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    cancel_btn = &static_cast<ui::Button&>(addElement<ui::Button>("cancel_btn", "Cancel"));
    cancel_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    cancel_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    cancel_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    message = &static_cast<ui::Label&>(addElement<ui::Label>("message", "Required: first name, last name, email."));
    message->setStyle(makeStyle({{"background", "transparent"}, {"color", "#fbbf24"}}));
    message->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void ContactFormUI::onFirstNameChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onFirstNameSubmit(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onLastNameChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onLastNameSubmit(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onEmailChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onEmailSubmit(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onPhoneChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onPhoneSubmit(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onCompanyChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onCompanySubmit(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onCityChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onCitySubmit(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onCategoryChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onActiveChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onNotesChange(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onNotesSubmit(std::string_view value)
{
    (void)value;
}

void ContactFormUI::onSaveBtnClick()
{
}

void ContactFormUI::onCancelBtnClick()
{
}

bool ContactFormUI::shouldClose() const
{
    return false;
}

bool ContactFormUI::handleGeneratedButton(std::string_view name)
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

bool ContactFormUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "first_name")
    {
        onFirstNameChange(value);
        return true;
    }
    else if (name == "last_name")
    {
        onLastNameChange(value);
        return true;
    }
    else if (name == "email")
    {
        onEmailChange(value);
        return true;
    }
    else if (name == "phone")
    {
        onPhoneChange(value);
        return true;
    }
    else if (name == "company")
    {
        onCompanyChange(value);
        return true;
    }
    else if (name == "city")
    {
        onCityChange(value);
        return true;
    }
    else if (name == "active")
    {
        onActiveChange(value);
        return true;
    }
    else if (name == "notes")
    {
        onNotesChange(value);
        return true;
    }
    return false;
}

bool ContactFormUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "first_name")
    {
        onFirstNameSubmit(value);
        return true;
    }
    else if (name == "last_name")
    {
        onLastNameSubmit(value);
        return true;
    }
    else if (name == "email")
    {
        onEmailSubmit(value);
        return true;
    }
    else if (name == "phone")
    {
        onPhoneSubmit(value);
        return true;
    }
    else if (name == "company")
    {
        onCompanySubmit(value);
        return true;
    }
    else if (name == "city")
    {
        onCitySubmit(value);
        return true;
    }
    else if (name == "notes")
    {
        onNotesSubmit(value);
        return true;
    }
    return false;
}

bool ContactFormUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "category")
    {
        onCategoryChange(value.empty() ? std::string_view{} : std::string_view{value.front()});
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions ContactFormUI::runtimeOptions()
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
        if (name == "first_name")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "last_name")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "email")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "phone")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "company")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "city")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "active")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
        else if (name == "notes")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "first_name")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "last_name")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "email")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "phone")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "company")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "city")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
        else if (name == "notes")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "category")
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
