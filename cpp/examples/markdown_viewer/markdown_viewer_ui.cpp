#include "markdown_viewer_ui.hpp"
#include "document_view/document_view.hpp"
#include "document_view/document_view_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "header", .type = "label", .cellName = "title", .relative = ui::Rect{0, 0, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 67, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{58, 1}, .cellCharsSize = ui::Size{67, 1}, .marginRight = 9, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#223a5a"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "viewer", .type = "document_view", .cellName = "content", .relative = ui::Rect{0, 0, 536870911, 536870911}, .sourceCell = ui::Rect{2, 27, 40, 11}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{40, 11}, .cellCharsSize = ui::Size{40, 11}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#000000"}}), .elementStyle = makeStyle({{"background", "#000000"}, {"padding", "1, 2, 1, 2"}})},
        ui::GeneratedLayoutEntry{.name = "status", .type = "label", .cellName = "", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{14, 0, 67, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 67), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{65, 1}, .cellCharsSize = ui::Size{67, 1}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#111827"}, {"color", "#93c5fd"}})},
        ui::GeneratedLayoutEntry{.name = "close_btn", .type = "button", .cellName = "title", .relative = ui::Rect{0, 58, 9, 1}, .sourceCell = ui::Rect{0, 0, 67, 1}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{67, 1}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#223a5a"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "docs_label", .type = "label", .cellName = "sidebar", .relative = ui::Rect{1, 1, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 26, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 26), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{24, 1}, .cellCharsSize = ui::Size{26, 11}, .marginRight = 1, .marginBottom = 9, .cellStyle = makeStyle({{"background", "#101a2b"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "docs", .type = "listbox", .cellName = "sidebar", .relative = ui::Rect{2, 1, 536870911, 536870910}, .sourceCell = ui::Rect{2, 0, 26, 11}, .width = dimension("expanded", 536870911), .height = dimension("fit-content", 536870910), .cellWidth = dimension("auto", 26), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{24, 4}, .cellCharsSize = ui::Size{26, 11}, .marginRight = 1, .marginBottom = 5, .cellStyle = makeStyle({{"background", "#101a2b"}}), .elementStyle = makeStyle({{"background", "#ffffff11"}, {"color", "#cbd5e1"}})},
        ui::GeneratedLayoutEntry{.name = "up_btn", .type = "button", .cellName = "sidebar", .relative = ui::Rect{7, 1, 10, 1}, .sourceCell = ui::Rect{2, 0, 26, 11}, .width = dimension("auto", 10), .height = dimension("auto", 1), .cellWidth = dimension("auto", 26), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{10, 1}, .cellCharsSize = ui::Size{26, 11}, .marginRight = 15, .marginBottom = 3, .cellStyle = makeStyle({{"background", "#101a2b"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "down_btn", .type = "button", .cellName = "sidebar", .relative = ui::Rect{7, 12, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 26, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 26), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{13, 1}, .cellCharsSize = ui::Size{26, 11}, .marginRight = 1, .marginBottom = 3, .cellStyle = makeStyle({{"background", "#101a2b"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

MarkdownViewerUI::MarkdownViewerUI() : ui::GeneratedWindowBase("Markdown Viewer")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "MarkdownViewer", R"UI_MCP_MD(# Markdown Viewer

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents."
tags: [example]
```

## Definition

```yaml
uses:
  - document_view
```

## Members

```yaml
header:
  type: label
  text: "Markdown Viewer"
  description: "Application title."
  expose: false

docs_label:
  type: label
  text: Documents
  description: "Label for the document list."

docs:
  type: listbox
  options: [overview.markdown, styles.markdown, tables.markdown, code.markdown]
  selected_items: [overview.markdown]
  multiple: "false"
  description: "Document listbox used to select the markdown document to render."

up_btn:
  type: button
  title: up
  description: "Scroll the rendered document upward."

down_btn:
  type: button
  title: down
  description: "Scroll the rendered document downward."

viewer:
  type: document_view
  cpp-class: "markdown_viewer_example::DocumentView"
  cpp-header: document_view/document_view.hpp
  description: "Scrollable rendered markdown document area."

close_btn:
  type: button
  title: Quit
  width: 9
  description: "Close the markdown viewer."

status:
  type: label
  text: Ready
  description: "Shows the current document and scroll state."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  text-align: center
  color: "#ffffff"
@title:
  background: "#223a5a"
@sidebar:
  background: "#101a2b"
@content:
  background: "#000000"
@viewer:
  background: "#000000"
  padding: 1, 2, 1, 2
@docs:
  background: "#ffffff11"
  color: "#cbd5e1"
  focus-background: "#ffffff22"
  edit-background: "#00000000"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2b5f9f"
@close_btn:
  background: "#7f1d1d"
  color: "#e2e8f0"
  focus-background: "#dc2626"
@status:
  color: "#93c5fd"
  background: "#111827"
```

## User Interface

```ui
+-title-**----------------------------------------------------------+
|header..**................................................close_btn|
+-sidebar------------------+-content-**-----------------------------+
|                          |viewer..**..............................|
| docs_label...**......... |........................................|
| docs.........**......... |*.......................................|
| ........................ |*.......................................|
| #....................... |........................................|
| #....................... |........................................|
|                          |........................................|
| up_btn.... down_btn..... |........................................|
*                          |........................................|
*                          |........................................|
|                          |........................................|
+--------------------------+----------------------------------------+
| status..**....................................................... |
+-------------------------------------------------------------------+
```
)UI_MCP_MD", "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"header", "Application title.", false}, ui::GeneratedElementMetadata{"docs_label", "Label for the document list.", true}, ui::GeneratedElementMetadata{"docs", "Document listbox used to select the markdown document to render.", true}, ui::GeneratedElementMetadata{"up_btn", "Scroll the rendered document upward.", true}, ui::GeneratedElementMetadata{"down_btn", "Scroll the rendered document downward.", true}, ui::GeneratedElementMetadata{"viewer", "Scrollable rendered markdown document area.", true}, ui::GeneratedElementMetadata{"close_btn", "Close the markdown viewer.", true}, ui::GeneratedElementMetadata{"status", "Shows the current document and scroll state.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#162033"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    header = &static_cast<ui::Label&>(addElement<ui::Label>("header", "Markdown Viewer"));
    header->setStyle(makeStyle({{"background", "transparent"}, {"color", "#ffffff"}, {"text-align", "center"}}));
    header->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    docs_label = &static_cast<ui::Label&>(addElement<ui::Label>("docs_label", "Documents"));
    docs_label->setStyle(makeStyle({{"background", "transparent"}, {"color", "#cbd5e1"}}));
    docs_label->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    docs = &static_cast<ui::ListBox&>(addElement<ui::ListBox>("docs", std::vector<std::string>{"overview.markdown", "styles.markdown", "tables.markdown", "code.markdown"}));
    docs->setSelectedIndex(0);
    docs->setStyle(makeStyle({{"background", "#ffffff11"}, {"color", "#cbd5e1"}}));
    docs->setFocusStyle(makeStyle({{"background", "#ffffff22"}, {"color", "#ffffff"}}));
    docs->setEditStyle(makeStyle({{"background", "#00000000"}, {"color", "#cbd5e1"}}));
    docs->setSelectedStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    docs->setDisabledStyle(makeStyle({{"color", "#64748b"}}));
    up_btn = &static_cast<ui::Button&>(addElement<ui::Button>("up_btn", "up"));
    up_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    up_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    up_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    down_btn = &static_cast<ui::Button&>(addElement<ui::Button>("down_btn", "down"));
    down_btn->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    down_btn->setFocusStyle(makeStyle({{"background", "#2b5f9f"}, {"color", "#ffffff"}}));
    down_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    viewer = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("viewer"));
    viewer->setChild(std::make_unique<markdown_viewer_example::DocumentView>());
    viewer->setStyle(makeStyle({{"background", "#000000"}, {"padding", "1, 2, 1, 2"}}));
    viewer->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    viewer->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    viewer->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    close_btn = &static_cast<ui::Button&>(addElement<ui::Button>("close_btn", "Quit"));
    close_btn->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    close_btn->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    close_btn->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    status = &static_cast<ui::Label&>(addElement<ui::Label>("status", "Ready"));
    status->setStyle(makeStyle({{"background", "#111827"}, {"color", "#93c5fd"}}));
    status->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void MarkdownViewerUI::onDocsSelectionChange(const std::vector<std::string>& value)
{
    (void)value;
}

void MarkdownViewerUI::onUpBtnClick()
{
}

void MarkdownViewerUI::onDownBtnClick()
{
}

void MarkdownViewerUI::onCloseBtnClick()
{
}

bool MarkdownViewerUI::shouldClose() const
{
    return false;
}

bool MarkdownViewerUI::handleGeneratedButton(std::string_view name)
{
    if (name == "up_btn")
    {
        onUpBtnClick();
        return true;
    }
    else if (name == "down_btn")
    {
        onDownBtnClick();
        return true;
    }
    else if (name == "close_btn")
    {
        onCloseBtnClick();
        return true;
    }
    return false;
}

bool MarkdownViewerUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool MarkdownViewerUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool MarkdownViewerUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    if (name == "docs")
    {
        onDocsSelectionChange(value);
        return true;
    }
    return false;
}

ui::GeneratedWindowRuntimeOptions MarkdownViewerUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "up_btn")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "down_btn")
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
        if (name == "docs")
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
