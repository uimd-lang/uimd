#include "edit_page_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "back_btn", .type = "button", .cellName = "top", .relative = ui::Rect{0, 1, 12, 1}, .sourceCell = ui::Rect{0, 0, 90, 1}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{90, 1}, .marginRight = 77, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "name_label", .type = "label", .cellName = "form", .relative = ui::Rect{0, 1, 14, 1}, .sourceCell = ui::Rect{2, 0, 90, 21}, .width = dimension("auto", 14), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 21), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{90, 21}, .marginRight = 75, .marginBottom = 20, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "edit_title", .type = "label", .cellName = "top", .relative = ui::Rect{0, 14, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 90, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{60, 1}, .cellCharsSize = ui::Size{90, 1}, .marginRight = 16, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "name_input", .type = "textinput", .cellName = "form", .relative = ui::Rect{0, 16, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 90, 21}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 21), .charsSize = ui::Size{73, 1}, .cellCharsSize = ui::Size{90, 21}, .marginRight = 1, .marginBottom = 20, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "save_btn", .type = "button", .cellName = "top", .relative = ui::Rect{0, 75, 14, 1}, .sourceCell = ui::Rect{0, 0, 90, 1}, .width = dimension("auto", 14), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{90, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}}), .elementStyle = makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "status", .type = "label", .cellName = "form", .relative = ui::Rect{2, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 90, 21}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 21), .charsSize = ui::Size{88, 1}, .cellCharsSize = ui::Size{90, 21}, .marginRight = 1, .marginBottom = 18, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "body_label", .type = "label", .cellName = "form", .relative = ui::Rect{4, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 90, 21}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 21), .charsSize = ui::Size{88, 1}, .cellCharsSize = ui::Size{90, 21}, .marginRight = 1, .marginBottom = 16, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "fields", .type = "viewhost", .cellName = "form", .relative = ui::Rect{5, 1, 536870911, 536870911}, .sourceCell = ui::Rect{2, 0, 90, 21}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 21), .charsSize = ui::Size{88, 15}, .cellCharsSize = ui::Size{90, 21}, .marginRight = 1, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#030712"}, {"scope-dim-background", "#0000004d"}})},
    };
}

}  // namespace

EditPageUI::EditPageUI() : ui::GeneratedWindowBase("Edit Page")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("component");
    setMcpMetadata(true, "EditPage", R"UI_MCP_MD(# Edit Page

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: regression
description: "Edit page shown after a ScrollView row button replaces the active ViewHost page."
tags: [regression, issue-5, viewhost]
```

## Definition

```yaml
extends: uicontrol
uses:
  - edit_fields_scroll
```

## Members

```yaml
edit_title:
  type: label
  text: "Edit item"
  description: "Edit page title."
  expose: false

back_btn:
  type: button
  title: Back
  description: "Return to the list page."

save_btn:
  type: button
  title: Save
  description: "Dummy save action used by the regression app."

name_label:
  type: label
  text: Name
  description: "Name field label."
  expose: false

name_input:
  type: textinput
  value: ""
  maxlength: 80
  description: "Editable item name."

body_label:
  type: label
  text: Fields
  description: "Fields panel label."
  expose: false

fields:
  type: viewhost
  description: "Scrollable edit fields panel."

status:
  type: label
  text: "Opened from ScrollView"
  description: "Edit page status."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@top:
  background: "#172033"
@edit_title:
  color: "#ffffff"
  text-align: center
@form:
  background: "#111827"
  padding: 1, 1, 1, 1
@fields:
  background: "#030712"
  focus-background: "#07111f"
  edit-background: "#0f172a"
  scope-dim-background: "#0000004d"
@status:
  color: "#93c5fd"
label:
  color: "#e5e7eb"
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
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@save_btn:
  background: "#14532d"
  focus-background: "#16a34a"
```

## User Interface

```ui
+-top-**-----------------------------------------------------------------------------------+
| back_btn.... edit_title..**.............................................. save_btn...... |
+-form-**----------------------------------------------------------------------------------+
| name_label.... name_input..**........................................................... |
|                                                                                          |
| status..**.............................................................................. |
|                                                                                          |
| body_label..**.......................................................................... |
| fields..**.............................................................................. |
| *....................................................................................... |
| *....................................................................................... |
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
|                                                                                          |
+------------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Edit page shown after a ScrollView row button replaces the active ViewHost page.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"edit_title", "Edit page title.", false}, ui::GeneratedElementMetadata{"back_btn", "Return to the list page.", true}, ui::GeneratedElementMetadata{"save_btn", "Dummy save action used by the regression app.", true}, ui::GeneratedElementMetadata{"name_label", "Name field label.", false}, ui::GeneratedElementMetadata{"name_input", "Editable item name.", true}, ui::GeneratedElementMetadata{"body_label", "Fields panel label.", false}, ui::GeneratedElementMetadata{"fields", "Scrollable edit fields panel.", true}, ui::GeneratedElementMetadata{"status", "Edit page status.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#030712"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    edit_title = &static_cast<ui::Label&>(addElement<ui::Label>("edit_title", "Edit item"));
    edit_title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    edit_title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    back_btn = &static_cast<ui::Button&>(addElement<ui::Button>("back_btn", "Back"));
    back_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    back_btn->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    back_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    save_btn = &static_cast<ui::Button&>(addElement<ui::Button>("save_btn", "Save"));
    save_btn->setStyle(makeStyle({{"background", "#14532d"}, {"color", "#e2e8f0"}}));
    save_btn->setFocusStyle(makeStyle({{"background", "#16a34a"}, {"color", "#ffffff"}}));
    save_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    name_label = &static_cast<ui::Label&>(addElement<ui::Label>("name_label", "Name"));
    name_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#e5e7eb"}}));
    name_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    name_input = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("name_input", "", 80));
    name_input->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#e5e7eb"}}));
    name_input->setFocusStyle(makeStyle({{"background", "#1e293b"}, {"color", "#ffffff"}}));
    name_input->setEditStyle(makeStyle({{"background", "#111827"}, {"color", "#ffffff"}}));
    name_input->setCursorStyle(makeStyle({{"background", "#facc15"}, {"color", "#111827"}}));
    name_input->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    name_input->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    body_label = &static_cast<ui::Label&>(addElement<ui::Label>("body_label", "Fields"));
    body_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#e5e7eb"}}));
    body_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    fields = &static_cast<ui::ViewHost&>(addElement<ui::ViewHost>("fields"));
    fields->setStyle(makeStyle({{"background", "#030712"}, {"scope-dim-background", "#0000004d"}}));
    fields->setFocusStyle(makeStyle({{"background", "#07111f"}}));
    fields->setEditStyle(makeStyle({{"background", "#0f172a"}}));
    status = &static_cast<ui::Label&>(addElement<ui::Label>("status", "Opened from ScrollView"));
    status->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    status->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void EditPageUI::onBackBtnClick()
{
}

void EditPageUI::onSaveBtnClick()
{
}

void EditPageUI::onNameInputChange(std::string_view value)
{
    (void)value;
}

void EditPageUI::onNameInputSubmit(std::string_view value)
{
    (void)value;
}

bool EditPageUI::shouldClose() const
{
    return false;
}

bool EditPageUI::handleGeneratedButton(std::string_view name)
{
    (void)name;
    if (name == "back_btn")
    {
        onBackBtnClick();
        return true;
    }
    else if (name == "save_btn")
    {
        onSaveBtnClick();
        return true;
    }
    return false;
}

bool EditPageUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    if (name == "name_input")
    {
        onNameInputChange(value);
        return true;
    }
    return false;
}

bool EditPageUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    if (name == "name_input")
    {
        onNameInputSubmit(value);
        return true;
    }
    return false;
}

bool EditPageUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    (void)name;
    (void)value;
    return false;
}

ui::GeneratedWindowRuntimeOptions EditPageUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "back_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "save_btn")
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
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "name_input")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
