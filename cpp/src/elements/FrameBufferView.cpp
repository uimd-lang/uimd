#include "ui/elements/FrameBufferView.hpp"
#include "ui/core/TextVisual.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <tuple>
#include <utility>

namespace ui {

namespace {

[[nodiscard]] std::optional<Color> colorFromString(const std::string& value) {
    if (value.empty()) {
        return std::nullopt;
    }
    try {
        return Color(value);
    } catch (...) {
        return std::nullopt;
    }
}

[[nodiscard]] std::string dimHexColor(const std::string& value, double dimAlpha = 0.4) {
    if (value.size() != 7 || value.front() != '#') {
        return value;
    }
    try {
        const int red = static_cast<int>(std::round((1.0 - dimAlpha) * std::stoi(value.substr(1, 2), nullptr, 16)));
        const int green = static_cast<int>(std::round((1.0 - dimAlpha) * std::stoi(value.substr(3, 2), nullptr, 16)));
        const int blue = static_cast<int>(std::round((1.0 - dimAlpha) * std::stoi(value.substr(5, 2), nullptr, 16)));
        std::ostringstream out;
        out << "#" << std::hex << std::setfill('0') << std::setw(2) << red
            << std::setw(2) << green << std::setw(2) << blue;
        return out.str();
    } catch (...) {
        return value;
    }
}

[[nodiscard]] std::vector<int> splitDimension(int size, int parts) {
    size = std::max(1, size);
    parts = std::max(1, std::min(parts, size));
    const int base = size / parts;
    const int extra = size % parts;
    std::vector<int> result;
    result.reserve(static_cast<std::size_t>(parts));
    for (int index = 0; index < parts; ++index) {
        result.push_back(base + (index < extra ? 1 : 0));
    }
    return result;
}

[[nodiscard]] std::string firstCellGlyph(const std::string& text) {
    return safeTerminalCellText(text);
}

}  // namespace

FrameBufferView::FrameBufferView(std::string name) : Element(std::move(name)) {}

void FrameBufferView::clearFrames() {
    frames_.clear();
    targetOrder_.clear();
    regions_.clear();
}

void FrameBufferView::setFrames(std::map<std::string, std::vector<std::vector<FrameBufferCell>>> frames,
                                std::vector<std::string> targetOrder,
                                std::map<std::string, FrameBufferRegion> regions) {
    frames_ = std::move(frames);
    targetOrder_ = std::move(targetOrder);
    regions_ = std::move(regions);
}

void FrameBufferView::setPan(int row, int col) {
    panRow_ = std::max(0, row);
    panCol_ = std::max(0, col);
}

void FrameBufferView::setDimmed(bool dimmed) {
    dimmed_ = dimmed;
}

void FrameBufferView::setInteractionState(bool focused, bool edit) {
    panelFocused_ = focused;
    panelEdit_ = edit;
}

std::map<std::string, FrameBufferRegion> FrameBufferView::defaultRegions(Size size) const {
    std::map<std::string, FrameBufferRegion> result;
    const int count = std::max(1, static_cast<int>(targetOrder_.size()));
    const std::vector<int> widths = splitDimension(std::max(1, size.width), count);
    int col = 0;
    for (int index = 0; index < static_cast<int>(targetOrder_.size()); ++index) {
        result[targetOrder_[static_cast<std::size_t>(index)]] = FrameBufferRegion{
            .row = 0,
            .col = col,
            .width = widths[static_cast<std::size_t>(index)],
            .height = std::max(1, size.height),
        };
        col += widths[static_cast<std::size_t>(index)];
    }
    return result;
}

void FrameBufferView::overlayCorners(RenderedContent& content,
                                     const std::map<std::string, FrameBufferRegion>& regions,
                                     const Style& style) const {
    if (!panelFocused_ && !panelEdit_) {
        return;
    }
    const Style& stateStyle = panelEdit_ && editStyle().has_value()
        ? *editStyle()
        : (panelFocused_ && focusStyle().has_value() ? *focusStyle() : style);
    const std::optional<Color> color = stateStyle.color.has_value()
        ? stateStyle.color
        : std::optional<Color>(Color(panelEdit_ ? "#22c55e" : "#facc15"));
    for (const std::string& name : targetOrder_) {
        auto regionIt = regions.find(name);
        if (regionIt == regions.end()) {
            continue;
        }
        const FrameBufferRegion& region = regionIt->second;
        if (region.width < 2 || region.height < 2) {
            continue;
        }
        const std::vector<std::tuple<int, int, std::string>> points = {
            {region.row, region.col, "┌"},
            {region.row, region.col + 1, "─"},
            {region.row, region.col + region.width - 2, "─"},
            {region.row, region.col + region.width - 1, "┐"},
            {region.row + region.height - 1, region.col, "└"},
            {region.row + region.height - 1, region.col + 1, "─"},
            {region.row + region.height - 1, region.col + region.width - 2, "─"},
            {region.row + region.height - 1, region.col + region.width - 1, "┘"},
        };
        for (const auto& [row, col, text] : points) {
            if (row >= 0 && row < static_cast<int>(content.size()) &&
                col >= 0 && col < static_cast<int>(content[static_cast<std::size_t>(row)].size())) {
                TerminalCell& cell = content[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
                cell.text = text;
                cell.foreground = color;
            }
        }
    }
}

RenderedContent FrameBufferView::render(Size size, ElementRenderState state) const {
    const Style style = effectiveStyle(state.focused, state.editMode);
    const int width = std::max(1, size.width);
    const int height = std::max(1, size.height);
    RenderedContent content;
    content.reserve(static_cast<std::size_t>(height));
    for (int row = 0; row < height; ++row) {
        RenderedRow renderedRow;
        renderedRow.reserve(static_cast<std::size_t>(width));
        for (int col = 0; col < width; ++col) {
            renderedRow.push_back(TerminalCell{.text = " ", .foreground = style.color, .background = style.background});
        }
        content.push_back(std::move(renderedRow));
    }

    const std::map<std::string, FrameBufferRegion> regions = regions_.empty() ? defaultRegions(size) : regions_;
    for (const std::string& name : targetOrder_) {
        auto regionIt = regions.find(name);
        if (regionIt == regions.end()) {
            continue;
        }
        auto frameIt = frames_.find(name);
        if (frameIt == frames_.end()) {
            continue;
        }
        const FrameBufferRegion& region = regionIt->second;
        const auto& rows = frameIt->second;
        for (int localRow = 0; localRow < region.height; ++localRow) {
            const int outRow = region.row + localRow;
            if (outRow < 0 || outRow >= height) {
                continue;
            }
            const int sourceRow = panRow_ + localRow;
            if (sourceRow < 0 || sourceRow >= static_cast<int>(rows.size())) {
                continue;
            }
            const auto& sourceCells = rows[static_cast<std::size_t>(sourceRow)];
            for (int localCol = 0; localCol < region.width; ++localCol) {
                const int outCol = region.col + localCol;
                if (outCol < 0 || outCol >= width) {
                    continue;
                }
                const int sourceCol = panCol_ + localCol;
                if (sourceCol < 0 || sourceCol >= static_cast<int>(sourceCells.size())) {
                    continue;
                }
                FrameBufferCell cell = sourceCells[static_cast<std::size_t>(sourceCol)];
                if (dimmed_) {
                    cell.foreground = dimHexColor(cell.foreground);
                    cell.background = dimHexColor(cell.background);
                }
                content[static_cast<std::size_t>(outRow)][static_cast<std::size_t>(outCol)] = TerminalCell{
                    .text = firstCellGlyph(cell.text),
                    .foreground = colorFromString(cell.foreground).value_or(style.color.value_or(Color())),
                    .background = colorFromString(cell.background).value_or(style.background.value_or(Color())),
                };
                if (!colorFromString(cell.foreground).has_value() && !style.color.has_value()) {
                    content[static_cast<std::size_t>(outRow)][static_cast<std::size_t>(outCol)].foreground = std::nullopt;
                }
                if (!colorFromString(cell.background).has_value() && !style.background.has_value()) {
                    content[static_cast<std::size_t>(outRow)][static_cast<std::size_t>(outCol)].background = std::nullopt;
                }
            }
        }
    }
    overlayCorners(content, regions, style);
    return content;
}

}  // namespace ui
