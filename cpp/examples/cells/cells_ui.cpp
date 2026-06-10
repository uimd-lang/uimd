#include "cells_ui.hpp"

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
        ui::GeneratedLayoutEntry{.name = "", .type = "", .cellName = "cl7", .relative = ui::Rect{0, 0, 4, 2}, .sourceCell = ui::Rect{12, 28, 4, 2}, .width = dimension("auto", 4), .height = dimension("auto", 2), .cellWidth = dimension("auto", 4), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 2}, .cellCharsSize = ui::Size{4, 2}, .marginRight = 0, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#2f3745"}})},
        ui::GeneratedLayoutEntry{.name = "a1", .type = "button", .cellName = "cell1", .relative = ui::Rect{0, 1, 7, 1}, .sourceCell = ui::Rect{0, 0, 17, 1}, .width = dimension("auto", 7), .height = dimension("auto", 1), .cellWidth = dimension("auto", 17), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{7, 1}, .cellCharsSize = ui::Size{17, 1}, .marginRight = 9, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#3b1f2d"}}), .elementStyle = makeStyle({{"background", "#7a3f59"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "c1", .type = "button", .cellName = "cell4", .relative = ui::Rect{0, 1, 4, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("auto", 4), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 22, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "d1", .type = "button", .cellName = "cl3", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 28, 4, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 4), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{2, 1}, .cellCharsSize = ui::Size{4, 11}, .marginRight = 1, .marginBottom = 10, .cellStyle = makeStyle({{"background", "#26345f"}}), .elementStyle = makeStyle({{"background", "#5267a3"}, {"color", "#d7fbe8"}})},
        ui::GeneratedLayoutEntry{.name = "f1", .type = "button", .cellName = "cl6", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{5, 22, 5, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 5), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{5, 9}, .marginRight = 1, .marginBottom = 8, .cellStyle = makeStyle({{"background", "#174f55"}}), .elementStyle = makeStyle({{"background", "#3a8890"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "b1", .type = "button", .cellName = "cell2", .relative = ui::Rect{0, 2, 536870911, 1}, .sourceCell = ui::Rect{0, 18, 9, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 9), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{5, 1}, .cellCharsSize = ui::Size{9, 1}, .marginRight = 2, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#1f463f"}}), .elementStyle = makeStyle({{"background", "#3d7b6d"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "c2", .type = "button", .cellName = "cell4", .relative = ui::Rect{0, 6, 4, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("auto", 4), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 17, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "a2", .type = "button", .cellName = "cell1", .relative = ui::Rect{0, 9, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 17, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 17), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{7, 1}, .cellCharsSize = ui::Size{17, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#3b1f2d"}}), .elementStyle = makeStyle({{"background", "#7a3f59"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "c3", .type = "button", .cellName = "cell4", .relative = ui::Rect{0, 11, 4, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("auto", 4), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 12, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "c4", .type = "button", .cellName = "cell4", .relative = ui::Rect{0, 16, 4, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("auto", 4), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 7, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "c5", .type = "button", .cellName = "cell4", .relative = ui::Rect{0, 21, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 2, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "c6", .type = "button", .cellName = "cell4", .relative = ui::Rect{1, 1, 4, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("auto", 4), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 22, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "e1", .type = "button", .cellName = "cell5", .relative = ui::Rect{1, 1, 9, 1}, .sourceCell = ui::Rect{5, 0, 21, 9}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{21, 9}, .marginRight = 11, .marginBottom = 7, .cellStyle = makeStyle({{"background", "#482456"}}), .elementStyle = makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "c7", .type = "button", .cellName = "cell4", .relative = ui::Rect{1, 6, 4, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("auto", 4), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 17, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "c8", .type = "button", .cellName = "cell4", .relative = ui::Rect{1, 11, 4, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("auto", 4), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 12, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "e5", .type = "button", .cellName = "cell5", .relative = ui::Rect{1, 11, 536870911, 1}, .sourceCell = ui::Rect{5, 0, 21, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{21, 9}, .marginRight = 1, .marginBottom = 7, .cellStyle = makeStyle({{"background", "#482456"}}), .elementStyle = makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "c9", .type = "button", .cellName = "cell4", .relative = ui::Rect{1, 16, 4, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("auto", 4), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 7, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "c10", .type = "button", .cellName = "cell4", .relative = ui::Rect{1, 21, 536870911, 1}, .sourceCell = ui::Rect{2, 0, 27, 2}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 27), .cellHeight = dimension("auto", 2), .charsSize = ui::Size{4, 1}, .cellCharsSize = ui::Size{27, 2}, .marginRight = 2, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#4a4422"}}), .elementStyle = makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}})},
        ui::GeneratedLayoutEntry{.name = "d2", .type = "button", .cellName = "cl3", .relative = ui::Rect{2, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 28, 4, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 4), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{2, 1}, .cellCharsSize = ui::Size{4, 11}, .marginRight = 1, .marginBottom = 8, .cellStyle = makeStyle({{"background", "#26345f"}}), .elementStyle = makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "e2", .type = "button", .cellName = "cell5", .relative = ui::Rect{3, 1, 9, 1}, .sourceCell = ui::Rect{5, 0, 21, 9}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{21, 9}, .marginRight = 11, .marginBottom = 5, .cellStyle = makeStyle({{"background", "#482456"}}), .elementStyle = makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "f2", .type = "button", .cellName = "cl6", .relative = ui::Rect{3, 1, 536870911, 1}, .sourceCell = ui::Rect{5, 22, 5, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 5), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{5, 9}, .marginRight = 1, .marginBottom = 5, .cellStyle = makeStyle({{"background", "#174f55"}}), .elementStyle = makeStyle({{"background", "#31565f"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "e6", .type = "button", .cellName = "cell5", .relative = ui::Rect{3, 11, 536870911, 1}, .sourceCell = ui::Rect{5, 0, 21, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{21, 9}, .marginRight = 1, .marginBottom = 5, .cellStyle = makeStyle({{"background", "#482456"}}), .elementStyle = makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "d3", .type = "button", .cellName = "cl3", .relative = ui::Rect{4, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 28, 4, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 4), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{2, 1}, .cellCharsSize = ui::Size{4, 11}, .marginRight = 1, .marginBottom = 6, .cellStyle = makeStyle({{"background", "#26345f"}}), .elementStyle = makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "e3", .type = "button", .cellName = "cell5", .relative = ui::Rect{5, 1, 9, 1}, .sourceCell = ui::Rect{5, 0, 21, 9}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{21, 9}, .marginRight = 11, .marginBottom = 3, .cellStyle = makeStyle({{"background", "#482456"}}), .elementStyle = makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "e7", .type = "button", .cellName = "cell5", .relative = ui::Rect{5, 11, 536870911, 1}, .sourceCell = ui::Rect{5, 0, 21, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{21, 9}, .marginRight = 1, .marginBottom = 3, .cellStyle = makeStyle({{"background", "#482456"}}), .elementStyle = makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "d4", .type = "button", .cellName = "cl3", .relative = ui::Rect{6, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 28, 4, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 4), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{2, 1}, .cellCharsSize = ui::Size{4, 11}, .marginRight = 1, .marginBottom = 4, .cellStyle = makeStyle({{"background", "#26345f"}}), .elementStyle = makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "f3", .type = "button", .cellName = "cl6", .relative = ui::Rect{6, 1, 536870911, 1}, .sourceCell = ui::Rect{5, 22, 5, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 5), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{3, 1}, .cellCharsSize = ui::Size{5, 9}, .marginRight = 1, .marginBottom = 2, .cellStyle = makeStyle({{"background", "#174f55"}}), .elementStyle = makeStyle({{"background", "#3a8890"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "e4", .type = "button", .cellName = "cell5", .relative = ui::Rect{7, 1, 9, 1}, .sourceCell = ui::Rect{5, 0, 21, 9}, .width = dimension("auto", 9), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{21, 9}, .marginRight = 11, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#482456"}}), .elementStyle = makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "e8", .type = "button", .cellName = "cell5", .relative = ui::Rect{7, 11, 536870911, 1}, .sourceCell = ui::Rect{5, 0, 21, 9}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 21), .cellHeight = dimension("auto", 9), .charsSize = ui::Size{9, 1}, .cellCharsSize = ui::Size{21, 9}, .marginRight = 1, .marginBottom = 1, .cellStyle = makeStyle({{"background", "#482456"}}), .elementStyle = makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "d5", .type = "button", .cellName = "cl3", .relative = ui::Rect{8, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 28, 4, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 4), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{2, 1}, .cellCharsSize = ui::Size{4, 11}, .marginRight = 1, .marginBottom = 2, .cellStyle = makeStyle({{"background", "#26345f"}}), .elementStyle = makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}})},
        ui::GeneratedLayoutEntry{.name = "d6", .type = "button", .cellName = "cl3", .relative = ui::Rect{10, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 28, 4, 11}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 4), .cellHeight = dimension("auto", 11), .charsSize = ui::Size{2, 1}, .cellCharsSize = ui::Size{4, 11}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#26345f"}}), .elementStyle = makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}})},
    };
}

}  // namespace

CellsUI::CellsUI() : ui::GeneratedWindowBase("Cells")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "Cells", R"UI_MCP_MD(# Cells

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior."
tags: [example]
```

## Members

```yaml
a1:
  type: button
  title: A1
  description: "Button in the first red cell."

a2:
  type: button
  title: A2
  description: "Second button in the first red cell."

b1:
  type: button
  title: BBB
  description: "Wide button in the green cell."

c1:
  type: button
  title: C1
  description: "Button in the yellow cell group."

c2:
  type: button
  title: C2
  description: "Button in the yellow cell group."

c3:
  type: button
  title: C3
  description: "Button in the yellow cell group."

c4:
  type: button
  title: C4
  description: "Button in the yellow cell group."

c5:
  type: button
  title: C5
  description: "Button in the yellow cell group."

c6:
  type: button
  title: C6
  description: "Button in the yellow cell group."

c7:
  type: button
  title: C7
  description: "Button in the yellow cell group."

c8:
  type: button
  title: C8
  description: "Button in the yellow cell group."

c9:
  type: button
  title: C9
  description: "Button in the yellow cell group."

c10:
  type: button
  title: C10
  description: "Button in the yellow cell group."

d1:
  type: button
  title: D1
  description: "Top button in the right column."

d2:
  type: button
  title: D2
  description: "Button in the right column."

d3:
  type: button
  title: D3
  description: "Button in the right column."

d4:
  type: button
  title: D4
  description: "Button in the right column."

d5:
  type: button
  title: D5
  description: "Button in the right column."

d6:
  type: button
  title: D6
  description: "Bottom button in the right column."

e1:
  type: button
  title: E
  description: "Single-character E button in the lower left cell."

e2:
  type: button
  title: EE
  description: "Two-character E button in the lower left cell."

e3:
  type: button
  title: EEE
  description: "Three-character E button in the lower left cell."

e4:
  type: button
  title: EEEE
  description: "Four-character E button in the lower left cell."

e5:
  type: button
  title: EEEEE
  description: "Five-character E button in the lower left cell."

e6:
  type: button
  title: EEEEEE
  description: "Six-character E button in the lower left cell."

e7:
  type: button
  title: EEEEEEE
  description: "Seven-character E button in the lower left cell."

e8:
  type: button
  title: EEEEEEEE
  description: "Eight-character E button in the lower left cell."

f1:
  type: button
  title: F1
  description: "Top button in the cyan cell."

f2:
  type: button
  title: F2
  description: "Middle button in the cyan cell with a local style override."

f3:
  type: button
  title: F3
  description: "Bottom button in the cyan cell."
```

## Style

```yaml
this:
  background: "#1f2946"
  border-color: transparent
  border-width: 1
  border-width-vertical: 2
button:
  background: "#526173"
  color: "#eef2f7"
  focus-background: "#6b7c91"
@d1:
  color: "#d7fbe8"
cell1:
  background: "#3b1f2d"
  button:
    background: "#7a3f59"
    focus-background: "#96536d"
cell2:
  background: "#1f463f"
  button:
    background: "#3d7b6d"
    focus-background: "#4f9484"
cl3:
  background: "#26345f"
  button:
    background: "#5267a3"
    focus-background: "#657abd"
cell4:
  background: "#4a4422"
  button:
    background: "#81753b"
    color: "#ecfdf5"
    focus-background: "#9a8d4a"
cell5:
  background: "#482456"
  button:
    background: "#805196"
    focus-background: "#9a65b0"
cl6:
  background: "#174f55"
  button:
    background: "#3a8890"
    focus-background: "#4aa1aa"
  @f2:
    background: "#31565f"
cl7:
  background: "#2f3745"
  button:
    background: "#64748b"
    focus-background: "#7b8ba0"
```

## User Interface

```ui
+-cell1-----------+-cell2---+-cl3+
| a1..... a2..... |  b1...  | d1 |
+-cell4-----------+---------+    |
| c1.. c2.. c3.. c4.. c5..  | d2 |
| c6.. c7.. c8.. c9.. c10.  |    |
+-cell5---------------+-cl6-+ d3 |
|                     | f1. |    |
| e1....... e5....... |     | d4 |
|                     |     |    |
| e2....... e6....... | f2. | d5 |
|                     |     |    |
| e3....... e7....... |     | d6 |
|                     | f3. +-cl7+
| e4....... e8....... |     |    |
|                     |     |    |
+---------------------+-----+----+
```
)UI_MCP_MD", "Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"a1", "Button in the first red cell.", true}, ui::GeneratedElementMetadata{"a2", "Second button in the first red cell.", true}, ui::GeneratedElementMetadata{"b1", "Wide button in the green cell.", true}, ui::GeneratedElementMetadata{"c1", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c2", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c3", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c4", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c5", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c6", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c7", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c8", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c9", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"c10", "Button in the yellow cell group.", true}, ui::GeneratedElementMetadata{"d1", "Top button in the right column.", true}, ui::GeneratedElementMetadata{"d2", "Button in the right column.", true}, ui::GeneratedElementMetadata{"d3", "Button in the right column.", true}, ui::GeneratedElementMetadata{"d4", "Button in the right column.", true}, ui::GeneratedElementMetadata{"d5", "Button in the right column.", true}, ui::GeneratedElementMetadata{"d6", "Bottom button in the right column.", true}, ui::GeneratedElementMetadata{"e1", "Single-character E button in the lower left cell.", true}, ui::GeneratedElementMetadata{"e2", "Two-character E button in the lower left cell.", true}, ui::GeneratedElementMetadata{"e3", "Three-character E button in the lower left cell.", true}, ui::GeneratedElementMetadata{"e4", "Four-character E button in the lower left cell.", true}, ui::GeneratedElementMetadata{"e5", "Five-character E button in the lower left cell.", true}, ui::GeneratedElementMetadata{"e6", "Six-character E button in the lower left cell.", true}, ui::GeneratedElementMetadata{"e7", "Seven-character E button in the lower left cell.", true}, ui::GeneratedElementMetadata{"e8", "Eight-character E button in the lower left cell.", true}, ui::GeneratedElementMetadata{"f1", "Top button in the cyan cell.", true}, ui::GeneratedElementMetadata{"f2", "Middle button in the cyan cell with a local style override.", true}, ui::GeneratedElementMetadata{"f3", "Bottom button in the cyan cell.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#1f2946"}, {"border-color", "transparent"}, {"border-width", "1"}, {"border-width-vertical", "2"}}));
    a1 = &static_cast<ui::Button&>(addElement<ui::Button>("a1", "A1"));
    a1->setStyle(makeStyle({{"background", "#7a3f59"}, {"color", "#eef2f7"}}));
    a1->setFocusStyle(makeStyle({{"background", "#96536d"}, {"color", "#ffffff"}}));
    a1->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    a2 = &static_cast<ui::Button&>(addElement<ui::Button>("a2", "A2"));
    a2->setStyle(makeStyle({{"background", "#7a3f59"}, {"color", "#eef2f7"}}));
    a2->setFocusStyle(makeStyle({{"background", "#96536d"}, {"color", "#ffffff"}}));
    a2->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    b1 = &static_cast<ui::Button&>(addElement<ui::Button>("b1", "BBB"));
    b1->setStyle(makeStyle({{"background", "#3d7b6d"}, {"color", "#eef2f7"}}));
    b1->setFocusStyle(makeStyle({{"background", "#4f9484"}, {"color", "#ffffff"}}));
    b1->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c1 = &static_cast<ui::Button&>(addElement<ui::Button>("c1", "C1"));
    c1->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c1->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c1->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c2 = &static_cast<ui::Button&>(addElement<ui::Button>("c2", "C2"));
    c2->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c2->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c2->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c3 = &static_cast<ui::Button&>(addElement<ui::Button>("c3", "C3"));
    c3->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c3->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c3->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c4 = &static_cast<ui::Button&>(addElement<ui::Button>("c4", "C4"));
    c4->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c4->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c4->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c5 = &static_cast<ui::Button&>(addElement<ui::Button>("c5", "C5"));
    c5->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c5->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c5->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c6 = &static_cast<ui::Button&>(addElement<ui::Button>("c6", "C6"));
    c6->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c6->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c6->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c7 = &static_cast<ui::Button&>(addElement<ui::Button>("c7", "C7"));
    c7->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c7->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c7->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c8 = &static_cast<ui::Button&>(addElement<ui::Button>("c8", "C8"));
    c8->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c8->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c8->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c9 = &static_cast<ui::Button&>(addElement<ui::Button>("c9", "C9"));
    c9->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c9->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c9->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    c10 = &static_cast<ui::Button&>(addElement<ui::Button>("c10", "C10"));
    c10->setStyle(makeStyle({{"background", "#81753b"}, {"color", "#ecfdf5"}}));
    c10->setFocusStyle(makeStyle({{"background", "#9a8d4a"}, {"color", "#ffffff"}}));
    c10->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    d1 = &static_cast<ui::Button&>(addElement<ui::Button>("d1", "D1"));
    d1->setStyle(makeStyle({{"background", "#5267a3"}, {"color", "#d7fbe8"}}));
    d1->setFocusStyle(makeStyle({{"background", "#657abd"}, {"color", "#ffffff"}}));
    d1->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    d2 = &static_cast<ui::Button&>(addElement<ui::Button>("d2", "D2"));
    d2->setStyle(makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}}));
    d2->setFocusStyle(makeStyle({{"background", "#657abd"}, {"color", "#ffffff"}}));
    d2->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    d3 = &static_cast<ui::Button&>(addElement<ui::Button>("d3", "D3"));
    d3->setStyle(makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}}));
    d3->setFocusStyle(makeStyle({{"background", "#657abd"}, {"color", "#ffffff"}}));
    d3->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    d4 = &static_cast<ui::Button&>(addElement<ui::Button>("d4", "D4"));
    d4->setStyle(makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}}));
    d4->setFocusStyle(makeStyle({{"background", "#657abd"}, {"color", "#ffffff"}}));
    d4->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    d5 = &static_cast<ui::Button&>(addElement<ui::Button>("d5", "D5"));
    d5->setStyle(makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}}));
    d5->setFocusStyle(makeStyle({{"background", "#657abd"}, {"color", "#ffffff"}}));
    d5->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    d6 = &static_cast<ui::Button&>(addElement<ui::Button>("d6", "D6"));
    d6->setStyle(makeStyle({{"background", "#5267a3"}, {"color", "#eef2f7"}}));
    d6->setFocusStyle(makeStyle({{"background", "#657abd"}, {"color", "#ffffff"}}));
    d6->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    e1 = &static_cast<ui::Button&>(addElement<ui::Button>("e1", "E"));
    e1->setStyle(makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}}));
    e1->setFocusStyle(makeStyle({{"background", "#9a65b0"}, {"color", "#ffffff"}}));
    e1->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    e2 = &static_cast<ui::Button&>(addElement<ui::Button>("e2", "EE"));
    e2->setStyle(makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}}));
    e2->setFocusStyle(makeStyle({{"background", "#9a65b0"}, {"color", "#ffffff"}}));
    e2->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    e3 = &static_cast<ui::Button&>(addElement<ui::Button>("e3", "EEE"));
    e3->setStyle(makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}}));
    e3->setFocusStyle(makeStyle({{"background", "#9a65b0"}, {"color", "#ffffff"}}));
    e3->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    e4 = &static_cast<ui::Button&>(addElement<ui::Button>("e4", "EEEE"));
    e4->setStyle(makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}}));
    e4->setFocusStyle(makeStyle({{"background", "#9a65b0"}, {"color", "#ffffff"}}));
    e4->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    e5 = &static_cast<ui::Button&>(addElement<ui::Button>("e5", "EEEEE"));
    e5->setStyle(makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}}));
    e5->setFocusStyle(makeStyle({{"background", "#9a65b0"}, {"color", "#ffffff"}}));
    e5->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    e6 = &static_cast<ui::Button&>(addElement<ui::Button>("e6", "EEEEEE"));
    e6->setStyle(makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}}));
    e6->setFocusStyle(makeStyle({{"background", "#9a65b0"}, {"color", "#ffffff"}}));
    e6->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    e7 = &static_cast<ui::Button&>(addElement<ui::Button>("e7", "EEEEEEE"));
    e7->setStyle(makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}}));
    e7->setFocusStyle(makeStyle({{"background", "#9a65b0"}, {"color", "#ffffff"}}));
    e7->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    e8 = &static_cast<ui::Button&>(addElement<ui::Button>("e8", "EEEEEEEE"));
    e8->setStyle(makeStyle({{"background", "#805196"}, {"color", "#eef2f7"}}));
    e8->setFocusStyle(makeStyle({{"background", "#9a65b0"}, {"color", "#ffffff"}}));
    e8->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    f1 = &static_cast<ui::Button&>(addElement<ui::Button>("f1", "F1"));
    f1->setStyle(makeStyle({{"background", "#3a8890"}, {"color", "#eef2f7"}}));
    f1->setFocusStyle(makeStyle({{"background", "#4aa1aa"}, {"color", "#ffffff"}}));
    f1->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    f2 = &static_cast<ui::Button&>(addElement<ui::Button>("f2", "F2"));
    f2->setStyle(makeStyle({{"background", "#31565f"}, {"color", "#eef2f7"}}));
    f2->setFocusStyle(makeStyle({{"background", "#4aa1aa"}, {"color", "#ffffff"}}));
    f2->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    f3 = &static_cast<ui::Button&>(addElement<ui::Button>("f3", "F3"));
    f3->setStyle(makeStyle({{"background", "#3a8890"}, {"color", "#eef2f7"}}));
    f3->setFocusStyle(makeStyle({{"background", "#4aa1aa"}, {"color", "#ffffff"}}));
    f3->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
}

void CellsUI::onA1Click()
{
}

void CellsUI::onA2Click()
{
}

void CellsUI::onB1Click()
{
}

void CellsUI::onC1Click()
{
}

void CellsUI::onC2Click()
{
}

void CellsUI::onC3Click()
{
}

void CellsUI::onC4Click()
{
}

void CellsUI::onC5Click()
{
}

void CellsUI::onC6Click()
{
}

void CellsUI::onC7Click()
{
}

void CellsUI::onC8Click()
{
}

void CellsUI::onC9Click()
{
}

void CellsUI::onC10Click()
{
}

void CellsUI::onD1Click()
{
}

void CellsUI::onD2Click()
{
}

void CellsUI::onD3Click()
{
}

void CellsUI::onD4Click()
{
}

void CellsUI::onD5Click()
{
}

void CellsUI::onD6Click()
{
}

void CellsUI::onE1Click()
{
}

void CellsUI::onE2Click()
{
}

void CellsUI::onE3Click()
{
}

void CellsUI::onE4Click()
{
}

void CellsUI::onE5Click()
{
}

void CellsUI::onE6Click()
{
}

void CellsUI::onE7Click()
{
}

void CellsUI::onE8Click()
{
}

void CellsUI::onF1Click()
{
}

void CellsUI::onF2Click()
{
}

void CellsUI::onF3Click()
{
}

bool CellsUI::shouldClose() const
{
    return false;
}

bool CellsUI::handleGeneratedButton(std::string_view name)
{
    if (name == "a1")
    {
        onA1Click();
        return true;
    }
    else if (name == "a2")
    {
        onA2Click();
        return true;
    }
    else if (name == "b1")
    {
        onB1Click();
        return true;
    }
    else if (name == "c1")
    {
        onC1Click();
        return true;
    }
    else if (name == "c2")
    {
        onC2Click();
        return true;
    }
    else if (name == "c3")
    {
        onC3Click();
        return true;
    }
    else if (name == "c4")
    {
        onC4Click();
        return true;
    }
    else if (name == "c5")
    {
        onC5Click();
        return true;
    }
    else if (name == "c6")
    {
        onC6Click();
        return true;
    }
    else if (name == "c7")
    {
        onC7Click();
        return true;
    }
    else if (name == "c8")
    {
        onC8Click();
        return true;
    }
    else if (name == "c9")
    {
        onC9Click();
        return true;
    }
    else if (name == "c10")
    {
        onC10Click();
        return true;
    }
    else if (name == "d1")
    {
        onD1Click();
        return true;
    }
    else if (name == "d2")
    {
        onD2Click();
        return true;
    }
    else if (name == "d3")
    {
        onD3Click();
        return true;
    }
    else if (name == "d4")
    {
        onD4Click();
        return true;
    }
    else if (name == "d5")
    {
        onD5Click();
        return true;
    }
    else if (name == "d6")
    {
        onD6Click();
        return true;
    }
    else if (name == "e1")
    {
        onE1Click();
        return true;
    }
    else if (name == "e2")
    {
        onE2Click();
        return true;
    }
    else if (name == "e3")
    {
        onE3Click();
        return true;
    }
    else if (name == "e4")
    {
        onE4Click();
        return true;
    }
    else if (name == "e5")
    {
        onE5Click();
        return true;
    }
    else if (name == "e6")
    {
        onE6Click();
        return true;
    }
    else if (name == "e7")
    {
        onE7Click();
        return true;
    }
    else if (name == "e8")
    {
        onE8Click();
        return true;
    }
    else if (name == "f1")
    {
        onF1Click();
        return true;
    }
    else if (name == "f2")
    {
        onF2Click();
        return true;
    }
    else if (name == "f3")
    {
        onF3Click();
        return true;
    }
    return false;
}

bool CellsUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    return false;
}

bool CellsUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    return false;
}

bool CellsUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    return false;
}

ui::GeneratedWindowRuntimeOptions CellsUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "a1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "a2")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "b1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c2")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c3")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c4")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c5")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c6")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c7")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c8")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c9")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "c10")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "d1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "d2")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "d3")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "d4")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "d5")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "d6")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "e1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "e2")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "e3")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "e4")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "e5")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "e6")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "e7")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "e8")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "f1")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "f2")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "f3")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}
