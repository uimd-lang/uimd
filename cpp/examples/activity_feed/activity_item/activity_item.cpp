#include "activity_item.hpp"

#include "activity_item_ui.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace activity_feed_example {

namespace {

[[nodiscard]] std::vector<std::string_view> splitLines(std::string_view text) {
    std::vector<std::string_view> lines;
    std::size_t offset = 0;
    while (offset <= text.size()) {
        const std::size_t newline = text.find('\n', offset);
        if (newline == std::string_view::npos) {
            lines.push_back(text.substr(offset));
            break;
        }
        lines.push_back(text.substr(offset, newline - offset));
        offset = newline + 1;
    }
    if (lines.empty()) {
        lines.emplace_back();
    }
    return lines;
}

[[nodiscard]] int wrappedLineCount(std::string_view text, int width) {
    const int safeWidth = std::max(1, width);
    int count = 0;
    for (std::string_view line : splitLines(text)) {
        count += std::max(1, static_cast<int>((line.size() + static_cast<std::size_t>(safeWidth) - 1) /
                                              static_cast<std::size_t>(safeWidth)));
    }
    return std::max(1, count);
}

template <typename GeneratedView>
[[nodiscard]] int generatedSourceHeight(const GeneratedView& view) {
    int height = 0;
    for (const auto& entry : view.generatedLayout()) {
        height = std::max(height, entry.sourceCell.row + entry.sourceCell.height);
    }
    return std::max(1, height);
}

template <typename GeneratedView>
[[nodiscard]] int compactContentHeight(const GeneratedView& view, int messageRows) {
    int height = 1;
    for (const auto& entry : view.generatedLayout()) {
        if (entry.name.empty()) {
            continue;
        }
        const int entryHeight = entry.name == "message"
            ? std::max(1, messageRows)
            : std::max(1, entry.charsSize.height);
        height = std::max(height, entry.relative.row + entryHeight);
    }
    return height;
}

[[nodiscard]] ui::Color markerColor(std::string_view eventType) {
    if (eventType == "Task") {
        return ui::Color{"#a78bfa"};
    }
    if (eventType == "Warning") {
        return ui::Color{"#facc15"};
    }
    if (eventType == "Deploy") {
        return ui::Color{"#22c55e"};
    }
    if (eventType == "Note") {
        return ui::Color{"#fb7185"};
    }
    return ui::Color{"#38bdf8"};
}

void applyLeftMarker(ui::RenderedContent& rendered, const ui::Color& color) {
    for (ui::RenderedRow& row : rendered) {
        if (row.empty()) {
            continue;
        }
        ui::TerminalCell& cell = row.front();
        cell.text = "▐";
        cell.foreground = color;
    }
}

}  // namespace

ui::RenderedContent renderActivityItem(const ActivityItemData& item, int width) {
    ActivityItemUI view;
    view.timestamp->setText(item.showTimestamp ? item.timestamp : "");
    view.event_type->setText(item.eventType);
    view.message->setText(item.message);

    const int sourceHeight = generatedSourceHeight(view);
    (void)ui::renderGeneratedWindowContent(view, ui::Size{std::max(1, width), sourceHeight});
    const int messageRows = wrappedLineCount(item.message, std::max(1, view.message->frame().width));
    ui::RenderedContent rendered = ui::renderGeneratedWindowContent(
        view,
        ui::Size{
            std::max(1, width),
            compactContentHeight(view, messageRows),
        });
    applyLeftMarker(rendered, markerColor(item.eventType));
    return rendered;
}

}  // namespace activity_feed_example
