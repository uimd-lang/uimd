#include "contacts_manager_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header", .type = "label", .cellName = "title", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 88, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{77, 1}, .cellCharsSize = ui::Size{88, 1}, .marginRight = 11, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#1f2937"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "contacts_label", .type = "label", .cellName = "list", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{6, 0, 35, 10}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("fixed", 32), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{33, 1}, .cellCharsSize = ui::Size{35, 10}, .marginRight = 1, .marginBottom = 9, .cellStyle = makeStyle({{"background", "#101a2b"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "detail", .type = "label", .cellName = "detail_cell", .relative = ui::Rect{0, 1, 536870911, 536870910}, .sourceCell = ui::Rect{6, 36, 52, 10}, .width = dimension("expanded", 536870911), .height = dimension("fit-content", 536870910), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{50, 10}, .cellCharsSize = ui::Size{52, 10}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#0b1120"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}, {"user-select", "text"}})},
        ui::GeneratedLayoutEntry{.name = "status", .type = "label", .cellName = "", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{17, 0, 88, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 88), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{86, 1}, .cellCharsSize = ui::Size{88, 1}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#111827"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "close_btn", .type = "button", .cellName = "title", .relative = ui::Rect{0, 77, 11, 1}, .sourceCell = ui::Rect{0, 0, 88, 1}, .width = dimension("auto", 11), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{11, 1}, .cellCharsSize = ui::Size{88, 1}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "add_btn", .type = "button", .cellName = "actions", .relative = ui::Rect{1, 1, 14, 1}, .sourceCell = ui::Rect{2, 0, 88, 3}, .width = dimension("auto", 14), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{88, 3}, .marginRight = 73, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "contacts", .type = "listbox", .cellName = "list", .relative = ui::Rect{1, 1, 536870911, 536870910}, .sourceCell = ui::Rect{6, 0, 35, 10}, .width = dimension("expanded", 536870911), .height = dimension("fit-content", 536870910), .cellWidth = dimension("fixed", 32), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{33, 8}, .cellCharsSize = ui::Size{35, 10}, .marginRight = 1, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#101a2b"}}), .elementStyle = makeStyle({{"background", "#ffffff11"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "edit_btn", .type = "button", .cellName = "actions", .relative = ui::Rect{1, 16, 15, 1}, .sourceCell = ui::Rect{2, 0, 88, 3}, .width = dimension("auto", 15), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{15, 1}, .cellCharsSize = ui::Size{88, 3}, .marginRight = 57, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "delete_btn", .type = "button", .cellName = "actions", .relative = ui::Rect{1, 32, 17, 1}, .sourceCell = ui::Rect{2, 0, 88, 3}, .width = dimension("auto", 17), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{17, 1}, .cellCharsSize = ui::Size{88, 3}, .marginRight = 39, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "save_btn", .type = "button", .cellName = "actions", .relative = ui::Rect{1, 50, 15, 1}, .sourceCell = ui::Rect{2, 0, 88, 3}, .width = dimension("auto", 15), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{15, 1}, .cellCharsSize = ui::Size{88, 3}, .marginRight = 23, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "reload_btn", .type = "button", .cellName = "actions", .relative = ui::Rect{1, 66, 17, 1}, .sourceCell = ui::Rect{2, 0, 88, 3}, .width = dimension("auto", 17), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 3), .charsSize = ui::Size{17, 1}, .cellCharsSize = ui::Size{88, 3}, .marginRight = 5, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

ContactsManagerUI::ContactsManagerUI() : ui::GeneratedWindowBase("Contacts Manager")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "ContactsManager", R"UI_MCP_MD(# Contacts Manager

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage."
tags: [example]
```

## Definition

```yaml
uses:
  - contact_form
```

## Members

```yaml
header:
  type: label
  text: "Contacts Manager"
  description: "Application title."
  expose: false

contacts_label:
  type: label
  text: Contacts
  description: "Label for the contacts list."

contacts:
  type: listbox
  options: []
  selected_items: []
  multiple: "false"
  description: "List of contact records; selecting a row updates the detail panel."

detail:
  type: label
  text: "Select a contact."
  description: "Read-only details for the selected contact."

add_btn:
  type: button
  title: Add
  description: "Open the contact form to create a new contact."

edit_btn:
  type: button
  title: Edit
  description: "Open the selected contact for editing."

delete_btn:
  type: button
  title: Delete
  description: "Delete the selected contact."

save_btn:
  type: button
  title: "Save CSV"
  description: "Save contacts to the configured CSV file."

reload_btn:
  type: button
  title: Reload
  description: "Reload contacts from the configured CSV file."

close_btn:
  type: button
  title: Quit
  width: 9
  description: "Close the contacts manager."

status:
  type: label
  text: Ready
  description: "Shows save, load, and validation status messages."
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
@list:
  background: "#101a2b"
@contacts:
  background: "#ffffff11"
  color: "#cbd5e1"
  focus-background: "#ffffff22"
  edit-background: "#00000000"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
@detail_cell:
  background: "#0b1120"
  padding: 1, 1, 1, 1
@detail:
  color: "#d1d5db"
  user-select: text
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@delete_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@close_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@status:
  background: "#111827"
  color: "#93c5fd"
```

## User Interface

```ui
+-title-**-------------------------------------------------------------------------------+
|header..**...................................................................close_btn..|
+-actions-**-----------------------------------------------------------------------------+
|                                                                                        |
| add_btn....... edit_btn....... delete_btn....... save_btn....... reload_btn.......     |
|                                                                                        |
+-list-32---------------------------+-detail_cell-**-------------------------------------+
| contacts_label.**................ | detail..**........................................ |
| contacts.......**................ | .................................................. |
| #................................ | #................................................. |
| #................................ | #................................................. |
| ................................. | .................................................. |
* ................................. | .................................................. |
* ................................. * .................................................. |
| ................................. * .................................................. |
| ................................. | .................................................. |
|                                   | .................................................. |
+-----------------------------------+----------------------------------------------------+
| status..**............................................................................ |
+----------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header", "Application title.", false}, ui::GeneratedElementMetadata{"contacts_label", "Label for the contacts list.", true}, ui::GeneratedElementMetadata{"contacts", "List of contact records; selecting a row updates the detail panel.", true}, ui::GeneratedElementMetadata{"detail", "Read-only details for the selected contact.", true}, ui::GeneratedElementMetadata{"add_btn", "Open the contact form to create a new contact.", true}, ui::GeneratedElementMetadata{"edit_btn", "Open the selected contact for editing.", true}, ui::GeneratedElementMetadata{"delete_btn", "Delete the selected contact.", true}, ui::GeneratedElementMetadata{"save_btn", "Save contacts to the configured CSV file.", true}, ui::GeneratedElementMetadata{"reload_btn", "Reload contacts from the configured CSV file.", true}, ui::GeneratedElementMetadata{"close_btn", "Close the contacts manager.", true}, ui::GeneratedElementMetadata{"status", "Shows save, load, and validation status messages.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header = &static_cast<ui::Label&>(addElement<ui::Label>("header", "Contacts Manager"));
    header->setStyle(makeStyle({{"background", "#1f2937"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    contacts_label = &static_cast<ui::Label&>(addElement<ui::Label>("contacts_label", "Contacts"));
    contacts_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    contacts_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    contacts = &static_cast<ui::ListBox&>(addElement<ui::ListBox>("contacts", std::vector<std::string>{}));
    contacts->setStyle(makeStyle({{"background", "#ffffff11"}, {"color", "#cbd5e1"}}));
    contacts->setFocusStyle(makeStyle({{"background", "#ffffff22"}, {"color", "#ffffff"}}));
    contacts->setEditStyle(makeStyle({{"background", "#00000000"}, {"color", "#cbd5e1"}}));
    contacts->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    contacts->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    detail = &static_cast<ui::Label&>(addElement<ui::Label>("detail", "Select a contact."));
    detail->setStyle(makeStyle({{"background", "transparent"}, {"color", "#d1d5db"}, {"user-select", "text"}}));
    detail->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    add_btn = &static_cast<ui::Button&>(addElement<ui::Button>("add_btn", "Add"));
    add_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    add_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    add_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    edit_btn = &static_cast<ui::Button&>(addElement<ui::Button>("edit_btn", "Edit"));
    edit_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    edit_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    edit_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    delete_btn = &static_cast<ui::Button&>(addElement<ui::Button>("delete_btn", "Delete"));
    delete_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    delete_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    delete_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    save_btn = &static_cast<ui::Button&>(addElement<ui::Button>("save_btn", "Save CSV"));
    save_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    save_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    save_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    reload_btn = &static_cast<ui::Button&>(addElement<ui::Button>("reload_btn", "Reload"));
    reload_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    reload_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    reload_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    close_btn = &static_cast<ui::Button&>(addElement<ui::Button>("close_btn", "Quit"));
    close_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    close_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    close_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    status = &static_cast<ui::Label&>(addElement<ui::Label>("status", "Ready"));
    status->setStyle(makeStyle({{"background", "#111827"}, {"color", "#93c5fd"}}));
    status->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void ContactsManagerUI::onContactsSelectionChange(const std::vector<std::string>& value)
{
    (void)value;
}

void ContactsManagerUI::onAddBtnClick()
{
}

void ContactsManagerUI::onEditBtnClick()
{
}

void ContactsManagerUI::onDeleteBtnClick()
{
}

void ContactsManagerUI::onSaveBtnClick()
{
}

void ContactsManagerUI::onReloadBtnClick()
{
}

void ContactsManagerUI::onCloseBtnClick()
{
}

bool ContactsManagerUI::shouldClose() const
{
    return false;
}

bool ContactsManagerUI::handleGeneratedButton(std::string_view name)
{
    if (name == "add_btn")
    {
        onAddBtnClick();
        return true;
    }
    else if (name == "edit_btn")
    {
        onEditBtnClick();
        return true;
    }
    else if (name == "delete_btn")
    {
        onDeleteBtnClick();
        return true;
    }
    else if (name == "save_btn")
    {
        onSaveBtnClick();
        return true;
    }
    else if (name == "reload_btn")
    {
        onReloadBtnClick();
        return true;
    }
    else if (name == "close_btn")
    {
        onCloseBtnClick();
        return true;
    }
    return false;
}

bool ContactsManagerUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool ContactsManagerUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool ContactsManagerUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "contacts")
    {
        onContactsSelectionChange(value);
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions ContactsManagerUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "add_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "edit_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "delete_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "save_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "reload_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "close_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "contacts")
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
