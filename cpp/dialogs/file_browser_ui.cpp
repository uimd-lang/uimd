#include "file_browser_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "dialog_header", .type = "label", .cellName = "", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 48, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 48), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{48, 1}, .cellCharsSize = ui::Size{48, 1}, .marginRight = 0, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#1d314c"}, {"color", "#cbd5e1"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "path_label", .type = "label", .cellName = "", .relative = ui::Rect{1, 3, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 48, 14}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 48), .cellHeight = dimension("auto", 14), .charsSize = ui::Size{42, 1}, .cellCharsSize = ui::Size{48, 14}, .marginRight = 3, .marginBottom = 12, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "entries", .type = "listbox", .cellName = "", .relative = ui::Rect{3, 3, 536870911, 6}, .sourceCell = ui::Rect{2, 0, 48, 14}, .width = dimension("expanded", 536870911), .height = dimension("auto", 6), .cellWidth = dimension("auto", 48), .cellHeight = dimension("auto", 14), .charsSize = ui::Size{42, 6}, .cellCharsSize = ui::Size{48, 14}, .marginRight = 3, .marginBottom = 5, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "\"File:\"", .type = "label", .cellName = "", .relative = ui::Rect{10, 3, 9, 1}, .sourceCell = ui::Rect{2, 0, 48, 14}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("auto", 48), .cellHeight = dimension("auto", 14), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{48, 14}, .marginRight = 36, .marginBottom = 3, .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "filename", .type = "textinput", .cellName = "", .relative = ui::Rect{10, 14, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 48, 14}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 48), .cellHeight = dimension("auto", 14), .charsSize = ui::Size{31, 1}, .cellCharsSize = ui::Size{48, 14}, .marginRight = 3, .marginBottom = 3, .elementStyle = makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}})},
        ui::GeneratedLayoutEntry{.name = "open_btn", .type = "button", .cellName = "", .relative = ui::Rect{12, 3, 20, 1}, .sourceCell = ui::Rect{2, 0, 48, 14}, .width = dimension("auto", 20), .height = dimension("auto", 1), .cellWidth = dimension("auto", 48), .cellHeight = dimension("auto", 14), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{48, 14}, .marginRight = 25, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "close_btn", .type = "button", .cellName = "", .relative = ui::Rect{12, 25, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 48, 14}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 48), .cellHeight = dimension("auto", 14), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{48, 14}, .marginRight = 3, .marginBottom = 1, .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

FileBrowserUI::FileBrowserUI() : ui::GeneratedWindowBase("File Browser")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("dialog");
    setMcpMetadata(true, "FileBrowser", R"UI_MCP_MD(# File Browser

## Metadata

```yaml
format: uimd
format-version: 1
kind: dialog
status: stable
description: "File Browser UI source."
tags: [dialog]
```

## Members

```yaml
dialog_header:
  type: label
  text: "Open File"

path_label:
  type: label
  text: .

entries:
  type: listbox
  options: []
  selected_items: []
  multiple: "false"
  commit-mode: leave

filename:
  type: textinput
  value: ""
  maxlength: 120

open_btn:
  type: button
  title: Open

close_btn:
  type: button
  title: Close
```

## Style

```yaml
include: dark
this:
  border-width: 0
@entries:
  background: "#0d1524"
  color: "#cbd5e1"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
```

## User Interface

```ui
+------------------------------------------------+
|dialog_header...................................|
+------------------------------------------------+
|                                                |
|   path_label................................   |
|                                                |
|   entries...................................   |
|   ..........................................   |
|   ..........................................   |
|   ..........................................   |
|   ..........................................   |
|   ..........................................   |
|                                                |
|   "File:"..  filename.......................   |
|                                                |
|   open_btn............  close_btn...........   |
|                                                |
+------------------------------------------------+
```
)UI_MCP_MD", "File Browser UI source.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"dialog_header", "Open File", true}, ui::GeneratedElementMetadata{"path_label", ".", true}, ui::GeneratedElementMetadata{"entries", "entries", true}, ui::GeneratedElementMetadata{"filename", "filename", true}, ui::GeneratedElementMetadata{"open_btn", "Open", true}, ui::GeneratedElementMetadata{"close_btn", "Close", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    dialog_header = &static_cast<ui::Label&>(addElement<ui::Label>("dialog_header", "Open File"));
    dialog_header->setStyle(makeStyle({{"background", "#1d314c"}, {"color", "#cbd5e1"}, {"text-align", "center"}}));
    dialog_header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    path_label = &static_cast<ui::Label&>(addElement<ui::Label>("path_label", "."));
    path_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    path_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    entries = &static_cast<ui::ListBox&>(addElement<ui::ListBox>("entries", std::vector<std::string>{}));
    entries->setCommitMode("leave");
    entries->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#cbd5e1"}}));
    entries->setFocusStyle(makeStyle({{"background", "#1d2f4d"}, {"color", "#ffffff"}}));
    entries->setEditStyle(makeStyle({{"background", "#243a5c"}, {"color", "#cbd5e1"}}));
    entries->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    entries->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    filename = &static_cast<ui::TextInput&>(addElement<ui::TextInput>("filename", "", 120));
    filename->setStyle(makeStyle({{"background", "#0d1524"}, {"color", "#e5e7eb"}}));
    filename->setFocusStyle(makeStyle({{"background", "#243a5c"}, {"color", "#ffffff"}}));
    filename->setEditStyle(makeStyle({{"background", "#2d456d"}, {"color", "#ffffff"}}));
    filename->setCursorStyle(makeStyle({{"background", "#ffffff"}, {"color", "#0e1117"}}));
    filename->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    filename->setDisabledStyle(makeStyle({{"background", "#0b1220"}, {"color", "#64748b"}}));
    open_btn = &static_cast<ui::Button&>(addElement<ui::Button>("open_btn", "Open"));
    open_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    open_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    open_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    close_btn = &static_cast<ui::Button&>(addElement<ui::Button>("close_btn", "Close"));
    close_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    close_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    close_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void FileBrowserUI::onEntriesSelectionChange(const std::vector<std::string>& value)
{
    (void)value;
}

void FileBrowserUI::onFilenameChange(std::string_view value)
{
    (void)value;
}

void FileBrowserUI::onFilenameSubmit(std::string_view value)
{
    (void)value;
}

void FileBrowserUI::onOpenBtnClick()
{
}

void FileBrowserUI::onCloseBtnClick()
{
}

bool FileBrowserUI::shouldClose() const
{
    return false;
}

bool FileBrowserUI::handleGeneratedButton(std::string_view name)
{
    if (name == "open_btn")
    {
        onOpenBtnClick();
        return true;
    }
    else if (name == "close_btn")
    {
        onCloseBtnClick();
        return true;
    }
    return false;
}

bool FileBrowserUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    if (name == "filename")
    {
        onFilenameChange(value);
        return true;
    }
    return false;
}

bool FileBrowserUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    if (name == "filename")
    {
        onFilenameSubmit(value);
        return true;
    }
    return false;
}

bool FileBrowserUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "entries")
    {
        onEntriesSelectionChange(value);
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions FileBrowserUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "open_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "close_btn")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.onTextChanged = [this](std::string_view name, std::string_view value)
    {
        if (name == "filename")
        {
            (void)handleGeneratedTextChanged(name, value);
        }
    };

    options.onTextConfirmed = [this](std::string_view name, std::string_view value)
    {
        if (name == "filename")
        {
            (void)handleGeneratedTextConfirmed(name, value);
        }
    };

    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)
    {
        if (name == "entries")
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
