#include "text_editor_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "window_header", .type = "label", .cellName = "title", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 87, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{87, 1}, .cellCharsSize = ui::Size{87, 1}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#1d314c"}, {"color", "#cbd5e1"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "status", .type = "label", .cellName = "", .relative = ui::Rect{0, 1, 85, 1}, .sourceCell = ui::Rect{18, 0, 87, 1}, .width = dimension("auto", 85), .height = dimension("auto", 1), .cellWidth = dimension("auto", 87), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{85, 1}, .cellCharsSize = ui::Size{87, 1}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "\"file\"", .type = "label", .cellName = "filebar", .relative = ui::Rect{1, 1, 6, 1}, .sourceCell = ui::Rect{2, 0, 87, 4}, .width = dimension("auto", 6), .height = dimension("auto", 1), .cellWidth = dimension("auto", 87), .cellHeight = dimension("auto", 4), .charsSize = ui::Size{6, 1}, .cellCharsSize = ui::Size{87, 4}, .marginRight = 80, .marginBottom = 2, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "editor", .type = "textarea", .cellName = "main", .relative = ui::Rect{1, 1, 536870911, 536870911}, .sourceCell = ui::Rect{7, 0, 87, 10}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("auto", 87), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{86, 9}, .cellCharsSize = ui::Size{87, 10}, .marginRight = 0, .marginBottom = 0},
        ui::GeneratedLayoutEntry{.name = "filename", .type = "label", .cellName = "filebar", .relative = ui::Rect{1, 8, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 87, 4}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 87), .cellHeight = dimension("auto", 4), .charsSize = ui::Size{78, 1}, .cellCharsSize = ui::Size{87, 4}, .marginRight = 1, .marginBottom = 2, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}})},
        ui::GeneratedLayoutEntry{.name = "new_btn", .type = "button", .cellName = "filebar", .relative = ui::Rect{3, 1, 14, 1}, .sourceCell = ui::Rect{2, 0, 87, 4}, .width = dimension("auto", 14), .height = dimension("auto", 1), .cellWidth = dimension("auto", 87), .cellHeight = dimension("auto", 4), .charsSize = ui::Size{14, 1}, .cellCharsSize = ui::Size{87, 4}, .marginRight = 72, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "open_btn", .type = "button", .cellName = "filebar", .relative = ui::Rect{3, 16, 15, 1}, .sourceCell = ui::Rect{2, 0, 87, 4}, .width = dimension("auto", 15), .height = dimension("auto", 1), .cellWidth = dimension("auto", 87), .cellHeight = dimension("auto", 4), .charsSize = ui::Size{15, 1}, .cellCharsSize = ui::Size{87, 4}, .marginRight = 56, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "save_btn", .type = "button", .cellName = "filebar", .relative = ui::Rect{3, 32, 15, 1}, .sourceCell = ui::Rect{2, 0, 87, 4}, .width = dimension("auto", 15), .height = dimension("auto", 1), .cellWidth = dimension("auto", 87), .cellHeight = dimension("auto", 4), .charsSize = ui::Size{15, 1}, .cellCharsSize = ui::Size{87, 4}, .marginRight = 40, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "save_as_btn", .type = "button", .cellName = "filebar", .relative = ui::Rect{3, 48, 18, 1}, .sourceCell = ui::Rect{2, 0, 87, 4}, .width = dimension("auto", 18), .height = dimension("auto", 1), .cellWidth = dimension("auto", 87), .cellHeight = dimension("auto", 4), .charsSize = ui::Size{18, 1}, .cellCharsSize = ui::Size{87, 4}, .marginRight = 21, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "quit_btn", .type = "button", .cellName = "filebar", .relative = ui::Rect{3, 67, 15, 1}, .sourceCell = ui::Rect{2, 0, 87, 4}, .width = dimension("auto", 15), .height = dimension("auto", 1), .cellWidth = dimension("auto", 87), .cellHeight = dimension("auto", 4), .charsSize = ui::Size{15, 1}, .cellCharsSize = ui::Size{87, 4}, .marginRight = 5, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

TextEditorUI::TextEditorUI() : ui::GeneratedWindowBase("Text Editor")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "TextEditor", R"UI_MCP_MD(# Text Editor

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Text Editor UI source."
tags: [example]
```

## Definition

```yaml
uses:
  - ../../dialogs/file_browser
```

## Members

```yaml
window_header:
  type: label
  text: "Text Editor"
  expose: false

filename:
  type: label
  text: welcome.txt

editor:
  type: textarea
  value: ""

new_btn:
  type: button
  title: New

open_btn:
  type: button
  title: Open

save_btn:
  type: button
  title: Save

save_as_btn:
  type: button
  title: "Save As"

quit_btn:
  type: button
  title: Quit

status:
  type: label
  text: Ready
```

## Style

```yaml
include: dark
this:
  border-width: 0
@filename:
  color: "#bfdbfe"
@quit_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@status:
  color: "#93c5fd"
```

## User Interface

```ui
+-title-**------------------------------------------------------------------------------+
|window_header..**......................................................................|
+-filebar-------------------------------------------------------------------------------+
|                                                                                       |
| "file" filename..**.................................................................. |
|                                                                                       |
| new_btn....... open_btn....... save_btn....... save_as_btn....... quit_btn.......     |
+-main----------------------------------------------------------------------------------+
|                                                                                       |
| editor..**............................................................................|
| ..................................................................................... |
* *.................................................................................... |
* *.................................................................................... |
| ..................................................................................... |
| ..................................................................................... |
| ..................................................................................... |
| ..................................................................................... |
| ..................................................................................... |
+---------------------------------------------------------------------------------------+
| status............................................................................... |
+---------------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Text Editor UI source.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"window_header", "Text Editor", false}, ui::GeneratedElementMetadata{"filename", "welcome.txt", true}, ui::GeneratedElementMetadata{"editor", "editor", true}, ui::GeneratedElementMetadata{"new_btn", "New", true}, ui::GeneratedElementMetadata{"open_btn", "Open", true}, ui::GeneratedElementMetadata{"save_btn", "Save", true}, ui::GeneratedElementMetadata{"save_as_btn", "Save As", true}, ui::GeneratedElementMetadata{"quit_btn", "Quit", true}, ui::GeneratedElementMetadata{"status", "Ready", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    window_header = &static_cast<ui::Label&>(addElement<ui::Label>("window_header", "Text Editor"));
    window_header->setStyle(makeStyle({{"background", "#1d314c"}, {"color", "#cbd5e1"}, {"text-align", "center"}}));
    window_header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    filename = &static_cast<ui::Label&>(addElement<ui::Label>("filename", "welcome.txt"));
    filename->setStyle(makeStyle({{"background", "transparent"}, {"color", "#bfdbfe"}}));
    filename->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    editor = &static_cast<ui::TextArea&>(addElement<ui::TextArea>("editor", "", 0));
    editor->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    editor->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    editor->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    editor->setCursorStyle(makeStyle({{"background", "#ffffff"}, {"color", "#0e1117"}}));
    editor->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    editor->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    new_btn = &static_cast<ui::Button&>(addElement<ui::Button>("new_btn", "New"));
    new_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    new_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    new_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    open_btn = &static_cast<ui::Button&>(addElement<ui::Button>("open_btn", "Open"));
    open_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    open_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    open_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    save_btn = &static_cast<ui::Button&>(addElement<ui::Button>("save_btn", "Save"));
    save_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    save_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    save_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    save_as_btn = &static_cast<ui::Button&>(addElement<ui::Button>("save_as_btn", "Save As"));
    save_as_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    save_as_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    save_as_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    quit_btn = &static_cast<ui::Button&>(addElement<ui::Button>("quit_btn", "Quit"));
    quit_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    quit_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    quit_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    status = &static_cast<ui::Label&>(addElement<ui::Label>("status", "Ready"));
    status->setStyle(makeStyle({{"background", "transparent"}, {"color", "#93c5fd"}}));
    status->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void TextEditorUI::onEditorChange(std::string_view value)
{
    (void)value;
}

void TextEditorUI::onEditorSubmit(std::string_view value)
{
    (void)value;
}

void TextEditorUI::onNewBtnClick()
{
}

void TextEditorUI::onOpenBtnClick()
{
}

void TextEditorUI::onSaveBtnClick()
{
}

void TextEditorUI::onSaveAsBtnClick()
{
}

void TextEditorUI::onQuitBtnClick()
{
}

bool TextEditorUI::shouldClose() const
{
    return false;
}

bool TextEditorUI::handleGeneratedButton(std::string_view name)
{
    if (name == "new_btn")
    {
        onNewBtnClick();
        return true;
    }
    else if (name == "open_btn")
    {
        onOpenBtnClick();
        return true;
    }
    else if (name == "save_btn")
    {
        onSaveBtnClick();
        return true;
    }
    else if (name == "save_as_btn")
    {
        onSaveAsBtnClick();
        return true;
    }
    else if (name == "quit_btn")
    {
        onQuitBtnClick();
        return true;
    }
    return false;
}

bool TextEditorUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "editor")
    {
        onEditorChange(value);
        return true;
    }
    return false;
}

bool TextEditorUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "editor")
    {
        onEditorSubmit(value);
        return true;
    }
    return false;
}

bool TextEditorUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions TextEditorUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "new_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "open_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "save_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "save_as_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "quit_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "editor")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "editor")
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
