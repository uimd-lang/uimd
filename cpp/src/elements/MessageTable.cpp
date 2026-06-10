#include "ui/elements/MessageTable.hpp"
#include "ui/core/TextVisual.hpp"

#include <algorithm>
#include <cctype>
#include <numeric>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

namespace ui {

namespace {

enum class TableAlignment {
    Left,
    Center,
    Right,
};

struct ParsedTable {
    std::vector<std::string> headers;
    std::vector<TableAlignment> alignments;
    std::vector<std::vector<std::string>> rows;
};

[[nodiscard]] int safeWidth(Size size) {
    return std::max(1, size.width);
}

[[nodiscard]] int safeHeight(Size size) {
    return std::max(1, size.height);
}

[[nodiscard]] TerminalCell styledCell(std::string text, const Style& style, bool border = false) {
    return TerminalCell{
        .text = safeTerminalCellText(text),
        .foreground = style.color,
        .background = border ? style.background : style.background,
    };
}

[[nodiscard]] std::string trim(std::string_view text) {
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin]))) {
        ++begin;
    }
    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }
    return std::string{text.substr(begin, end - begin)};
}

[[nodiscard]] std::vector<std::string> splitLines(std::string_view text) {
    std::vector<std::string> lines;
    std::size_t offset = 0;
    while (offset <= text.size()) {
        const std::size_t newline = text.find('\n', offset);
        if (newline == std::string_view::npos) {
            lines.emplace_back(text.substr(offset));
            break;
        }
        lines.emplace_back(text.substr(offset, newline - offset));
        offset = newline + 1;
    }
    return lines;
}

[[nodiscard]] std::vector<std::string> splitTableCells(std::string_view line) {
    std::vector<std::string> cells;
    std::size_t begin = 0;
    std::size_t end = line.size();
    if (begin < end && line[begin] == '|') {
        ++begin;
    }
    if (end > begin && line[end - 1] == '|') {
        --end;
    }
    while (begin <= end) {
        const std::size_t separator = line.find('|', begin);
        const std::size_t cellEnd = separator == std::string_view::npos ? end : std::min(separator, end);
        cells.push_back(trim(line.substr(begin, cellEnd - begin)));
        if (separator == std::string_view::npos || separator >= end) {
            break;
        }
        begin = separator + 1;
    }
    return cells;
}

[[nodiscard]] bool isMarkdownSeparatorRow(const std::vector<std::string>& cells) {
    if (cells.empty()) {
        return false;
    }
    for (const std::string& cell : cells) {
        const std::string compact = trim(cell);
        if (compact.empty()) {
            return false;
        }
        for (char ch : compact) {
            if (ch != '-' && ch != ':') {
                return false;
            }
        }
    }
    return true;
}

[[nodiscard]] std::vector<TableAlignment> parseAlignments(const std::vector<std::string>& cells) {
    std::vector<TableAlignment> alignments;
    alignments.reserve(cells.size());
    for (const std::string& cell : cells) {
        const std::string compact = trim(cell);
        const bool left = !compact.empty() && compact.front() == ':';
        const bool right = !compact.empty() && compact.back() == ':';
        if (left && right) {
            alignments.push_back(TableAlignment::Center);
        } else if (right) {
            alignments.push_back(TableAlignment::Right);
        } else {
            alignments.push_back(TableAlignment::Left);
        }
    }
    return alignments;
}

[[nodiscard]] std::string fitVisibleText(std::string_view text, std::size_t width) {
    std::vector<VisualGlyph> glyphs = visualGlyphs(text);
    if (glyphs.size() > width) {
        glyphs.resize(width);
    }
    std::string result;
    for (const VisualGlyph& glyph : glyphs) {
        result += glyph.text;
    }
    return result;
}

[[nodiscard]] std::string alignText(std::string_view text, std::size_t width, TableAlignment alignment) {
    const std::size_t textWidth = static_cast<std::size_t>(std::max(0, visibleWidth(text)));
    if (textWidth >= width) {
        return fitVisibleText(text, width);
    }
    const std::size_t padding = width - textWidth;
    const std::string visible = visibleText(text);
    if (alignment == TableAlignment::Right) {
        return std::string(padding, ' ') + visible;
    }
    if (alignment == TableAlignment::Center) {
        const std::size_t left = padding / 2;
        const std::size_t right = padding - left;
        return std::string(left, ' ') + visible + std::string(right, ' ');
    }
    return visible + std::string(padding, ' ');
}

[[nodiscard]] std::vector<std::size_t> columnWidths(const ParsedTable& table) {
    std::vector<std::size_t> widths(table.headers.size(), 0);
    for (std::size_t column = 0; column < table.headers.size(); ++column) {
        widths[column] = static_cast<std::size_t>(std::max(0, visibleWidth(table.headers[column])));
    }
    for (const auto& row : table.rows) {
        for (std::size_t column = 0; column < row.size() && column < widths.size(); ++column) {
            widths[column] = std::max(widths[column],
                                      static_cast<std::size_t>(std::max(0, visibleWidth(row[column]))));
        }
    }
    return widths;
}

[[nodiscard]] int renderedTableWidth(const std::vector<std::size_t>& widths) {
    if (widths.empty()) {
        return 0;
    }
    return static_cast<int>(
        1 + std::accumulate(widths.begin(), widths.end(), std::size_t{0}, [](std::size_t sum, std::size_t value) {
            return sum + value + 2;
        }) + (widths.size() - 1) + 1);
}

[[nodiscard]] std::vector<std::size_t> expandColumnWidths(std::vector<std::size_t> widths, int targetWidth) {
    if (widths.empty()) {
        return widths;
    }
    const int extraWidth = targetWidth - renderedTableWidth(widths);
    if (extraWidth <= 0) {
        return widths;
    }
    const int perColumn = extraWidth / static_cast<int>(widths.size());
    const int remainder = extraWidth % static_cast<int>(widths.size());
    for (std::size_t index = 0; index < widths.size(); ++index) {
        widths[index] += static_cast<std::size_t>(perColumn + (static_cast<int>(index) < remainder ? 1 : 0));
    }
    return widths;
}

[[nodiscard]] std::optional<ParsedTable> parseMarkdownTable(std::string_view text) {
    const std::vector<std::string> lines = splitLines(text);
    if (lines.size() < 3) {
        return std::nullopt;
    }

    const std::vector<std::string> headers = splitTableCells(lines[0]);
    const std::vector<std::string> separator = splitTableCells(lines[1]);
    if (headers.empty() || headers.size() != separator.size() || !isMarkdownSeparatorRow(separator)) {
        return std::nullopt;
    }

    ParsedTable table{
        .headers = headers,
        .alignments = parseAlignments(separator),
    };
    for (std::size_t index = 2; index < lines.size(); ++index) {
        std::vector<std::string> row = splitTableCells(lines[index]);
        if (!row.empty() && row.size() == headers.size()) {
            table.rows.push_back(std::move(row));
        }
    }
    if (table.rows.empty()) {
        return std::nullopt;
    }
    return table;
}

[[nodiscard]] RenderedRow blankRow(int width, const Style& style) {
    RenderedRow row;
    row.reserve(static_cast<std::size_t>(width));
    for (int column = 0; column < width; ++column) {
        row.push_back(styledCell(" ", style));
    }
    return row;
}

void appendGlyph(RenderedRow& row, std::string glyph, const Style& style) {
    row.push_back(styledCell(std::move(glyph), style, true));
}

void appendText(RenderedRow& row, std::string_view text, const Style& style) {
    for (const VisualGlyph& glyph : visualGlyphs(text)) {
        row.push_back(styledCell(glyph.text, style));
    }
}

[[nodiscard]] RenderedRow separatorRow(
    const std::vector<std::size_t>& widths,
    const Style& borderStyle,
    std::string_view left,
    std::string_view middle,
    std::string_view right) {
    RenderedRow row;
    appendGlyph(row, std::string(left), borderStyle);
    for (std::size_t column = 0; column < widths.size(); ++column) {
        for (std::size_t index = 0; index < widths[column] + 2; ++index) {
            appendGlyph(row, "\xE2\x94\x80", borderStyle);
        }
        if (column + 1 < widths.size()) {
            appendGlyph(row, std::string(middle), borderStyle);
        }
    }
    appendGlyph(row, std::string(right), borderStyle);
    return row;
}

[[nodiscard]] RenderedRow dataRow(
    const std::vector<std::string>& cells,
    const std::vector<std::size_t>& widths,
    const std::vector<TableAlignment>& alignments,
    const Style& cellStyle,
    const Style& borderStyle) {
    RenderedRow row;
    appendGlyph(row, "\xE2\x94\x82", borderStyle);
    for (std::size_t column = 0; column < widths.size(); ++column) {
        const std::string value = column < cells.size() ? alignText(cells[column], widths[column], alignments[column]) : std::string(widths[column], ' ');
        appendText(row, " ", cellStyle);
        appendText(row, value, cellStyle);
        appendText(row, " ", cellStyle);
        if (column + 1 < widths.size()) {
            appendGlyph(row, "\xE2\x94\x82", borderStyle);
        }
    }
    appendGlyph(row, "\xE2\x94\x82", borderStyle);
    return row;
}

}  // namespace

MessageTable::MessageTable(std::string name, std::string text)
    : Element(std::move(name)), text_(std::move(text)) {}

void MessageTable::setText(std::string text) {
    text_ = std::move(text);
}

int MessageTable::fitContentHeight() const {
    const std::optional<ParsedTable> parsed = parseMarkdownTable(text_);
    if (!parsed.has_value()) {
        return 1;
    }
    return static_cast<int>(parsed->rows.size()) + 4;
}

RenderedContent MessageTable::render(Size size, ElementRenderState state) const {
    const Style style = effectiveStyle(state.focused, state.editMode);
    const int width = safeWidth(size);
    const int height = safeHeight(size);
    const std::optional<ParsedTable> parsed = parseMarkdownTable(text_);
    if (!parsed.has_value()) {
        return RenderedContent{blankRow(width, style)};
    }

    const std::vector<std::size_t> widths = expandColumnWidths(columnWidths(*parsed), width);

    Style borderStyle = style;
    RenderedContent rendered;
    rendered.push_back(separatorRow(widths, borderStyle, "\xE2\x94\x8C", "\xE2\x94\xAC", "\xE2\x94\x90"));
    rendered.push_back(dataRow(parsed->headers, widths, parsed->alignments, style, borderStyle));
    rendered.push_back(separatorRow(widths, borderStyle, "\xE2\x94\x9C", "\xE2\x94\xBC", "\xE2\x94\xA4"));
    for (const auto& row : parsed->rows) {
        rendered.push_back(dataRow(row, widths, parsed->alignments, style, borderStyle));
    }
    rendered.push_back(separatorRow(widths, borderStyle, "\xE2\x94\x94", "\xE2\x94\xB4", "\xE2\x94\x98"));

    (void)height;
    return rendered;
}

}  // namespace ui
