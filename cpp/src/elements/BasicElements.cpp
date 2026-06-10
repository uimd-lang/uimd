#include "ui/elements/Button.hpp"
#include "ui/elements/CheckBox.hpp"
#include "ui/elements/ComboBox.hpp"
#include "ui/elements/Label.hpp"
#include "ui/elements/ListBox.hpp"
#include "ui/elements/NumberInput.hpp"
#include "ui/elements/TextInput.hpp"
#include "ui/core/TextVisual.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cmath>
#include <exception>
#include <iomanip>
#include <optional>
#include <sstream>
#include <utility>
#include <vector>

namespace ui {

namespace {

thread_local std::optional<std::int64_t> renderTimeOverrideMs;

[[nodiscard]] int safeWidth(Size size, std::string_view text) {
    return std::max(1, size.width > 0 ? size.width : visibleWidth(text));
}

[[nodiscard]] int safeHeight(Size size) {
    return std::max(1, size.height > 0 ? size.height : 1);
}

[[nodiscard]] std::string fitText(std::string text, int width) {
    std::vector<VisualGlyph> glyphs = visualGlyphs(text);
    if (static_cast<int>(glyphs.size()) > width) {
        glyphs.resize(static_cast<std::size_t>(width));
    }
    std::string result;
    for (const VisualGlyph& glyph : glyphs) {
        result += glyph.text;
    }
    if (static_cast<int>(glyphs.size()) < width) {
        result.append(static_cast<std::size_t>(width - static_cast<int>(glyphs.size())), ' ');
    }
    return result;
}

[[nodiscard]] std::string centerText(std::string text, int width) {
    const int textWidth = visibleWidth(text);
    if (textWidth >= width) {
        return fitText(std::move(text), width);
    }
    const int totalPadding = width - textWidth;
    const int leftPadding = totalPadding / 2;
    const int rightPadding = totalPadding - leftPadding;
    return std::string(static_cast<std::size_t>(leftPadding), ' ') + visibleText(text) +
           std::string(static_cast<std::size_t>(rightPadding), ' ');
}

[[nodiscard]] std::int64_t gradientFrame(const TextGradient& gradient) {
    const int interval = std::max(1, gradient.intervalMs);
    if (renderTimeOverrideMs.has_value()) {
        return (*renderTimeOverrideMs / interval) * static_cast<std::int64_t>(gradient.step);
    }
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    return (millis / interval) * static_cast<std::int64_t>(gradient.step);
}

[[nodiscard]] std::optional<Color> gradientColorAt(const std::optional<TextGradient>& gradient, int sourceIndex) {
    if (!gradient.has_value() || gradient->colors.empty()) {
        return std::nullopt;
    }
    const int segmentSize = std::max(1, gradient->segmentSize);
    const std::int64_t frame = gradientFrame(*gradient);
    const std::int64_t size = static_cast<std::int64_t>(gradient->colors.size());
    std::int64_t index = ((sourceIndex / segmentSize) - frame) % size;
    if (index < 0) {
        index += size;
    }
    return gradient->colors[static_cast<std::size_t>(index)];
}

[[nodiscard]] std::optional<Color> blendGradientColor(const std::optional<Color>& overlay,
                                                      const std::optional<Color>& base) {
    if (!overlay.has_value()) {
        return base;
    }
    if (!overlay->rgba().has_value()) {
        return overlay;
    }
    if (overlay->rgba()->alpha == 0) {
        return base;
    }
    if (overlay->rgba()->alpha < 255 && base.has_value()) {
        return overlay->blendOver(*base);
    }
    return overlay;
}

[[nodiscard]] TerminalCell styledCell(std::string text, const Style& style,
                                      std::optional<Color> foreground = std::nullopt,
                                      std::optional<Color> background = std::nullopt) {
    return TerminalCell{
        .text = safeTerminalCellText(text),
        .foreground = foreground.has_value() ? std::move(foreground) : style.color,
        .background = background.has_value() ? std::move(background) : style.background,
    };
}

[[nodiscard]] TerminalCell styledCell(char ch, const Style& style,
                                      std::optional<Color> foreground = std::nullopt,
                                      std::optional<Color> background = std::nullopt) {
    return styledCell(std::string(1, ch), style, std::move(foreground), std::move(background));
}

[[nodiscard]] RenderedRow renderGlyphRow(std::vector<VisualGlyph> glyphs, int width, const Style& style) {
    if (static_cast<int>(glyphs.size()) > width) {
        glyphs.resize(static_cast<std::size_t>(width));
    }
    const int contentWidth = static_cast<int>(glyphs.size());
    const int offset = style.textAlign == "center"
        ? std::max(0, (width - contentWidth) / 2)
        : (style.textAlign == "right" ? std::max(0, width - contentWidth) : 0);
    RenderedRow row;
    row.reserve(static_cast<std::size_t>(width));
    for (int col = 0; col < offset && col < width; ++col) {
        row.push_back(styledCell(" ", style));
    }
    for (const VisualGlyph& glyph : glyphs) {
        if (static_cast<int>(row.size()) >= width) {
            break;
        }
        std::optional<Color> foreground;
        std::optional<Color> background;
        if (glyph.sourceStart >= 0) {
            const int sourceIndex = glyph.sourceStart;
            foreground = blendGradientColor(gradientColorAt(style.textColorGradient, sourceIndex), style.color);
            background = blendGradientColor(gradientColorAt(style.textBackgroundGradient, sourceIndex), style.background);
        }
        row.push_back(styledCell(glyph.text, style, foreground, background));
    }
    while (static_cast<int>(row.size()) < width) {
        row.push_back(styledCell(" ", style));
    }
    return row;
}

[[nodiscard]] RenderedRow renderRow(std::string text, int width, const Style& style, int sourceStart = 0) {
    return renderGlyphRow(visualGlyphs(text, 0, sourceStart), width, style);
}

[[nodiscard]] Style mergedOptional(Style base, const std::optional<Style>& override) {
    if (override.has_value()) {
        base.merge(*override);
    }
    return base;
}

[[nodiscard]] Style mergedIndicatorStyleForCheckBox(Style base,
                                                    const std::optional<Style>& override,
                                                    bool focusedOrEditing) {
    const std::optional<Color> inheritedBackground = base.background;
    if (override.has_value()) {
        base.merge(*override);
    }
    if (focusedOrEditing &&
        base.background.has_value() &&
        base.background->isTransparent() &&
        inheritedBackground.has_value()) {
        base.background = inheritedBackground;
    }
    return base;
}

[[nodiscard]] int clampIndex(int value, int low, int high) {
    return std::max(low, std::min(value, high));
}

void appendBlankCells(RenderedRow& row, int width, const Style& style) {
    while (static_cast<int>(row.size()) < width) {
        row.push_back(styledCell(' ', style));
    }
}

struct LabelVisualRow {
    int start = 0;
    int end = 0;
    std::vector<VisualGlyph> cells;
};

[[nodiscard]] LabelVisualRow makeLabelVisualRow(int fallbackStart, std::vector<VisualGlyph> cells) {
    if (cells.empty()) {
        return LabelVisualRow{fallbackStart, fallbackStart, {}};
    }
    return LabelVisualRow{cells.front().sourceStart, cells.back().sourceEnd, std::move(cells)};
}

[[nodiscard]] std::vector<LabelVisualRow> buildLabelVisualRows(const std::string& text, int width) {
    width = std::max(1, width);
    std::vector<LabelVisualRow> rows;
    std::size_t segmentStart = 0;
    while (segmentStart <= text.size()) {
        const std::size_t newline = text.find('\n', segmentStart);
        const std::size_t segmentEnd = newline == std::string::npos ? text.size() : newline;
        const std::string_view segment(text.data() + static_cast<std::ptrdiff_t>(segmentStart), segmentEnd - segmentStart);
        if (segment.empty()) {
            rows.push_back(LabelVisualRow{static_cast<int>(segmentStart), static_cast<int>(segmentStart), {}});
        } else {
            const std::vector<VisualGlyph> glyphs = visualGlyphs(segment, 0, static_cast<int>(segmentStart));
            std::size_t offset = 0;
            while (offset < glyphs.size()) {
                const std::size_t remaining = glyphs.size() - offset;
                const std::size_t chunkSize = std::min<std::size_t>(remaining, static_cast<std::size_t>(width));
                std::vector<VisualGlyph> chunk(glyphs.begin() + static_cast<std::ptrdiff_t>(offset),
                                               glyphs.begin() + static_cast<std::ptrdiff_t>(offset + chunkSize));
                if (chunkSize < static_cast<std::size_t>(width)) {
                    rows.push_back(makeLabelVisualRow(static_cast<int>(segmentStart), std::move(chunk)));
                    offset += chunkSize;
                    continue;
                }
                std::size_t lastSpace = std::string::npos;
                for (std::size_t index = chunk.size(); index > 1; --index) {
                    const VisualGlyph& glyph = chunk[index - 1];
                    if (glyph.text == " " &&
                        glyph.sourceStart >= 0 &&
                        glyph.sourceStart < static_cast<int>(text.size()) &&
                        text[static_cast<std::size_t>(glyph.sourceStart)] == ' ') {
                        lastSpace = index - 1;
                        break;
                    }
                }
                if (lastSpace != std::string::npos && lastSpace > 0) {
                    chunk.resize(lastSpace);
                    rows.push_back(makeLabelVisualRow(static_cast<int>(segmentStart), std::move(chunk)));
                    offset += lastSpace + 1;
                } else {
                    rows.push_back(makeLabelVisualRow(static_cast<int>(segmentStart), std::move(chunk)));
                    offset += chunkSize;
                }
            }
        }
        if (newline == std::string::npos) {
            break;
        }
        segmentStart = newline + 1;
    }
    if (rows.empty()) {
        rows.push_back(LabelVisualRow{0, 0, {}});
    }
    return rows;
}

[[nodiscard]] int visualWidthForRow(const LabelVisualRow& row) {
    return static_cast<int>(row.cells.size());
}

[[nodiscard]] int rawIndexForLabelVisualColumn(const LabelVisualRow& row, int col) {
    if (row.cells.empty()) {
        return row.start;
    }
    if (col >= static_cast<int>(row.cells.size())) {
        return row.end;
    }
    col = clampIndex(col, 0, static_cast<int>(row.cells.size()) - 1);
    const VisualGlyph& glyph = row.cells[static_cast<std::size_t>(col)];
    int runStart = col;
    int runEnd = col + 1;
    while (runStart > 0 &&
           row.cells[static_cast<std::size_t>(runStart - 1)].sourceStart == glyph.sourceStart &&
           row.cells[static_cast<std::size_t>(runStart - 1)].sourceEnd == glyph.sourceEnd) {
        --runStart;
    }
    while (runEnd < static_cast<int>(row.cells.size()) &&
           row.cells[static_cast<std::size_t>(runEnd)].sourceStart == glyph.sourceStart &&
           row.cells[static_cast<std::size_t>(runEnd)].sourceEnd == glyph.sourceEnd) {
        ++runEnd;
    }
    const double midpoint = static_cast<double>(runStart) + (static_cast<double>(runEnd - runStart) / 2.0);
    return static_cast<double>(col) >= midpoint ? glyph.sourceEnd : glyph.sourceStart;
}

}  // namespace

ScopedRenderTimeOverride::ScopedRenderTimeOverride(std::optional<std::int64_t> timeMs)
    : previous_(renderTimeOverrideMs) {
    renderTimeOverrideMs = timeMs;
}

ScopedRenderTimeOverride::~ScopedRenderTimeOverride() {
    renderTimeOverrideMs = previous_;
}

RenderedContent renderPlainText(const std::string& text, int width, int height, const Style& style) {
    const int safeRenderWidth = safeWidth(Size{width, height}, text);
    const int safeRenderHeight = safeHeight(Size{width, height});
    RenderedContent rendered;
    rendered.reserve(static_cast<std::size_t>(safeRenderHeight));

    if (safeRenderHeight > 1) {
        const std::vector<LabelVisualRow> rows = buildLabelVisualRows(text, safeRenderWidth);
        for (const LabelVisualRow& row : rows) {
            if (static_cast<int>(rendered.size()) >= safeRenderHeight) {
                break;
            }
            rendered.push_back(renderGlyphRow(row.cells, safeRenderWidth, style));
        }
        while (static_cast<int>(rendered.size()) < safeRenderHeight) {
            rendered.push_back(renderRow("", safeRenderWidth, style, 0));
        }
        return rendered;
    }

    const std::size_t newline = text.find('\n');
    const std::size_t segmentEnd = newline == std::string::npos ? text.size() : newline;
    std::vector<VisualGlyph> glyphs = visualGlyphs(std::string_view(text.data(), segmentEnd), 0, 0);
    rendered.push_back(renderGlyphRow(std::move(glyphs), safeRenderWidth, style));
    return rendered;
}

std::vector<std::string> renderedText(const RenderedContent& rendered) {
    std::vector<std::string> rows;
    rows.reserve(rendered.size());
    for (const RenderedRow& row : rendered) {
        std::string text;
        for (const TerminalCell& cell : row) {
            text += cell.text;
        }
        rows.push_back(std::move(text));
    }
    return rows;
}

Label::Label(std::string name, std::string text)
    : Element(std::move(name)), text_(std::move(text)) {}

void Label::setText(std::string text) {
    text_ = std::move(text);
    spans_.clear();
}

void Label::setSpans(std::vector<LabelSpan> spans) {
    spans_ = std::move(spans);
    text_.clear();
    for (const LabelSpan& span : spans_) {
        text_ += span.text;
    }
}

void Label::selectRange(int start, int end) {
    const int textLen = static_cast<int>(text_.size());
    selectionStart_ = std::max(0, std::min(textLen, start));
    selectionEnd_ = std::max(0, std::min(textLen, end));
}

void Label::clearSelection() {
    selectionStart_.reset();
    selectionEnd_.reset();
}

bool Label::hasSelection() const {
    return selectionStart_.has_value() && selectionEnd_.has_value() &&
           selectionStart_ != selectionEnd_;
}

std::string Label::selectedText() const {
    if (!hasSelection()) {
        return {};
    }
    const int selStart = std::min(*selectionStart_, *selectionEnd_);
    const int selEnd = std::max(*selectionStart_, *selectionEnd_);
    const int textLen = static_cast<int>(text_.size());
    const int from = std::max(0, std::min(textLen, selStart));
    const int to = std::max(0, std::min(textLen, selEnd));
    return text_.substr(static_cast<std::size_t>(from), static_cast<std::size_t>(to - from));
}

int Label::textPositionFromPoint(int localRow, int localCol, Size size) const {
    const int width = safeWidth(size, text_);
    const int textLen = static_cast<int>(text_.size());
    if (size.height == 1) {
        const std::size_t newline = text_.find('\n');
        const std::size_t segmentEnd = newline == std::string::npos ? text_.size() : newline;
        LabelVisualRow visible = makeLabelVisualRow(0, visualGlyphs(std::string_view(text_.data(), segmentEnd), 0, 0));
        const Style style = effectiveStyle(false, false);
        const int offset = style.textAlign == "center"
            ? std::max(0, (width - visualWidthForRow(visible)) / 2)
            : (style.textAlign == "right" ? std::max(0, width - visualWidthForRow(visible)) : 0);
        return std::max(0, std::min(textLen, rawIndexForLabelVisualColumn(visible, localCol - offset)));
    }
    const auto rows = buildLabelVisualRows(text_, width);
    if (rows.empty()) {
        return 0;
    }
    if (localRow < 0) {
        return 0;
    }
    if (localRow >= static_cast<int>(rows.size())) {
        return textLen;
    }
    const LabelVisualRow& row = rows[static_cast<std::size_t>(localRow)];
    const Style style = effectiveStyle(false, false);
    const int offset = style.textAlign == "center"
        ? std::max(0, (width - visualWidthForRow(row)) / 2)
        : (style.textAlign == "right" ? std::max(0, width - visualWidthForRow(row)) : 0);
    return std::min(textLen, rawIndexForLabelVisualColumn(row, localCol - offset));
}

RenderedContent Label::render(Size size, ElementRenderState state) const {
    const Style style = effectiveStyle(state.focused, state.editMode);
    const int width = safeWidth(size, text_);
    const int height = safeHeight(size);

    const bool activeSelection = hasSelection();
    const int selStart = activeSelection ? std::min(*selectionStart_, *selectionEnd_) : 0;
    const int selEnd = activeSelection ? std::max(*selectionStart_, *selectionEnd_) : 0;

    if (spans_.empty() && !activeSelection) {
        return renderPlainText(text_, width, height, style);
    }

    struct StyledChar {
        char ch = ' ';
        std::optional<Color> foreground;
        std::optional<Color> background;
    };
    std::vector<StyledChar> chars;
    if (spans_.empty()) {
        for (char ch : text_) {
            chars.push_back(StyledChar{.ch = ch});
        }
    } else {
        for (const LabelSpan& span : spans_) {
            std::optional<Color> foreground = span.foreground.empty() ? std::nullopt : std::optional<Color>(Color(span.foreground));
            std::optional<Color> background = span.background.empty() ? std::nullopt : std::optional<Color>(Color(span.background));
            for (char ch : span.text) {
                chars.push_back(StyledChar{.ch = ch, .foreground = foreground, .background = background});
            }
        }
    }

    const Style curStyle = cursorStyle().has_value() ? style.merged(*cursorStyle()) : style;

    RenderedContent rendered;
    rendered.reserve(static_cast<std::size_t>(height));
    RenderedRow renderedRow;
    renderedRow.reserve(static_cast<std::size_t>(width));
    bool clippingLine = false;
    int sourceIndex = 0;
    for (const StyledChar& current : chars) {
        if (current.ch == '\r') {
            ++sourceIndex;
            continue;
        }
        if (current.ch == '\n') {
            appendBlankCells(renderedRow, width, style);
            rendered.push_back(std::move(renderedRow));
            ++sourceIndex;
            if (static_cast<int>(rendered.size()) >= height) {
                break;
            }
            renderedRow = RenderedRow{};
            renderedRow.reserve(static_cast<std::size_t>(width));
            clippingLine = false;
            continue;
        }
        if (static_cast<int>(renderedRow.size()) >= width) {
            clippingLine = true;
        }
        if (!clippingLine) {
            const bool selected = activeSelection && sourceIndex >= selStart && sourceIndex < selEnd;
            if (selected) {
                renderedRow.push_back(styledCell(current.ch, curStyle, current.foreground, current.background));
            } else {
                renderedRow.push_back(styledCell(current.ch, style, current.foreground, current.background));
            }
        }
        ++sourceIndex;
    }
    if (static_cast<int>(rendered.size()) < height) {
        appendBlankCells(renderedRow, width, style);
        rendered.push_back(std::move(renderedRow));
    }
    while (static_cast<int>(rendered.size()) < height) {
        rendered.push_back(renderRow("", width, style));
    }
    return rendered;
}

Button::Button(std::string name, std::string title)
    : Element(std::move(name)), title_(std::move(title)) {}

void Button::setTitle(std::string title) {
    title_ = std::move(title);
}

RenderedContent Button::render(Size size, ElementRenderState state) const {
    const int width = safeWidth(size, title_) + (size.width > 0 ? 0 : 4);
    std::string body = title_;
    const int titleWidth = visibleWidth(title_);
    if (state.focused && width >= titleWidth + 2) {
        if (width >= titleWidth + 6) {
            body = " [" + centerText(" " + title_ + " ", width - 4) + "] ";
        } else if (width >= titleWidth + 4) {
            body = " [" + centerText(title_, width - 4) + "] ";
        } else {
            body = "[" + visibleText(title_) + "]";
        }
    }
    body = centerText(body, width);
    return renderPlainText(body, width, 1, effectiveStyle(state.focused, state.editMode));
}

CheckBox::CheckBox(std::string name, std::string title, bool checked)
    : Element(std::move(name)), title_(std::move(title)), checked_(checked) {}

bool CheckBox::handleKey(std::string_view key) {
    if (key == "Enter" || key == " ") {
        checked_ = !checked_;
        return true;
    }
    return false;
}

RenderedContent CheckBox::render(Size size, ElementRenderState state) const {
    const std::string indicator = checked_ ? "[x]" : "[ ]";
    const std::string text = title_.empty() ? indicator : indicator + " " + title_;
    const int width = size.width > 0 ? std::max(1, size.width) : std::max(1, visibleWidth(text));
    const Style base = effectiveStyle(state.focused, state.editMode);
    RenderedContent rendered = renderPlainText(text, width, 1, base);
    const Style indicatorStyle = checked_
        ? mergedIndicatorStyleForCheckBox(base, checkedStyle(), state.focused || state.editMode)
        : mergedIndicatorStyleForCheckBox(base, uncheckedStyle(), state.focused || state.editMode);
    for (int index = 0; index < std::min(3, width); ++index) {
        rendered[0][static_cast<std::size_t>(index)] = styledCell(text[static_cast<std::size_t>(index)], indicatorStyle);
    }
    return rendered;
}

namespace {

constexpr int kTextInputOptionHorizontalSteps = 5;
constexpr int kTextInputOptionVerticalSteps = 3;

std::string normalizeTextInputValue(std::string value, bool multiline) {
    if (multiline) {
        return value;
    }
    for (char& ch : value) {
        if (ch == '\r' || ch == '\n') {
            ch = ' ';
        }
    }
    return value;
}

struct VisualTextRow {
    int start = 0;
    int end = 0;
    std::vector<VisualGlyph> cells;
};

[[nodiscard]] VisualTextRow makeVisualTextRow(int fallbackStart, std::vector<VisualGlyph> cells) {
    if (cells.empty()) {
        return VisualTextRow{fallbackStart, fallbackStart, {}};
    }
    return VisualTextRow{cells.front().sourceStart, cells.back().sourceEnd, std::move(cells)};
}

std::vector<VisualTextRow> buildVisualRows(std::string_view text, int width) {
    width = std::max(1, width);
    std::vector<VisualTextRow> rows;
    std::size_t segmentStart = 0;
    while (segmentStart <= text.size()) {
        const std::size_t newline = text.find('\n', segmentStart);
        const std::size_t segmentEnd = newline == std::string_view::npos ? text.size() : newline;
        const std::string_view segment = text.substr(segmentStart, segmentEnd - segmentStart);
        if (segment.empty()) {
            rows.push_back(VisualTextRow{static_cast<int>(segmentStart), static_cast<int>(segmentStart), {}});
        } else {
            const std::vector<VisualGlyph> glyphs = visualGlyphs(segment, 0, static_cast<int>(segmentStart));
            std::size_t offset = 0;
            while (offset < glyphs.size()) {
                const std::size_t remaining = glyphs.size() - offset;
                const std::size_t chunkSize = std::min<std::size_t>(remaining, static_cast<std::size_t>(width));
                std::vector<VisualGlyph> chunk(glyphs.begin() + static_cast<std::ptrdiff_t>(offset),
                                               glyphs.begin() + static_cast<std::ptrdiff_t>(offset + chunkSize));
                if (chunkSize < static_cast<std::size_t>(width)) {
                    rows.push_back(makeVisualTextRow(static_cast<int>(segmentStart), std::move(chunk)));
                    offset += chunkSize;
                    continue;
                }
                std::size_t lastSpace = std::string::npos;
                for (std::size_t index = chunk.size(); index > 1; --index) {
                    const VisualGlyph& glyph = chunk[index - 1];
                    if (glyph.text == " " &&
                        glyph.sourceStart >= 0 &&
                        glyph.sourceStart < static_cast<int>(text.size()) &&
                        text[static_cast<std::size_t>(glyph.sourceStart)] == ' ') {
                        lastSpace = index - 1;
                        break;
                    }
                }
                if (lastSpace != std::string::npos && lastSpace > 0) {
                    chunk.resize(lastSpace);
                    rows.push_back(makeVisualTextRow(static_cast<int>(segmentStart), std::move(chunk)));
                    offset += lastSpace + 1;
                } else {
                    rows.push_back(makeVisualTextRow(static_cast<int>(segmentStart), std::move(chunk)));
                    offset += chunkSize;
                }
            }
        }
        if (newline == std::string_view::npos) {
            break;
        }
        segmentStart = newline + 1;
    }
    if (rows.empty()) {
        rows.push_back(VisualTextRow{0, 0, {}});
    }
    return rows;
}

int visualRowForCursor(const std::vector<VisualTextRow>& rows, int width, int cursor) {
    for (std::size_t index = 0; index < rows.size(); ++index) {
        const VisualTextRow& row = rows[index];
        if (cursor <= row.end && (static_cast<int>(row.cells.size()) < width || cursor < row.end)) {
            return static_cast<int>(index);
        }
    }
    return std::max(0, static_cast<int>(rows.size()) - 1);
}

int visualColumnForCursor(const VisualTextRow& row, int cursor, int width) {
    if (cursor < row.start || cursor > row.end) {
        return -1;
    }
    for (std::size_t index = 0; index < row.cells.size(); ++index) {
        const VisualGlyph& glyph = row.cells[index];
        if (glyph.sourceStart >= 0 && cursor <= glyph.sourceStart) {
            return static_cast<int>(index);
        }
        if (glyph.sourceStart >= 0 && glyph.sourceEnd >= 0 && cursor > glyph.sourceStart && cursor < glyph.sourceEnd) {
            return static_cast<int>(index);
        }
    }
    if (cursor >= row.start && cursor <= row.end) {
        return std::min(static_cast<int>(row.cells.size()), std::max(0, width - 1));
    }
    return -1;
}

int rawIndexForVisualColumn(const VisualTextRow& row, int col) {
    if (row.cells.empty()) {
        return row.start;
    }
    if (col >= static_cast<int>(row.cells.size())) {
        return row.end;
    }
    col = clampIndex(col, 0, static_cast<int>(row.cells.size()) - 1);
    const VisualGlyph& glyph = row.cells[static_cast<std::size_t>(col)];
    int runStart = col;
    int runEnd = col + 1;
    while (runStart > 0 &&
           row.cells[static_cast<std::size_t>(runStart - 1)].sourceStart == glyph.sourceStart &&
           row.cells[static_cast<std::size_t>(runStart - 1)].sourceEnd == glyph.sourceEnd) {
        --runStart;
    }
    while (runEnd < static_cast<int>(row.cells.size()) &&
           row.cells[static_cast<std::size_t>(runEnd)].sourceStart == glyph.sourceStart &&
           row.cells[static_cast<std::size_t>(runEnd)].sourceEnd == glyph.sourceEnd) {
        ++runEnd;
    }
    const double midpoint = static_cast<double>(runStart) + (static_cast<double>(runEnd - runStart) / 2.0);
    return static_cast<double>(col) >= midpoint ? glyph.sourceEnd : glyph.sourceStart;
}

}  // namespace

TextInput::TextInput(std::string name, std::string value, int maxLength)
    : TextInput(std::move(name), std::move(value), maxLength, false) {}

TextInput::TextInput(std::string name, std::string value, int maxLength, bool multiline)
    : Element(std::move(name)),
      value_(normalizeTextInputValue(std::move(value), multiline)),
      cursor_(static_cast<int>(value_.size())),
      maxLength_(maxLength),
      multiline_(multiline) {}

void TextInput::setValue(std::string value) {
    value_ = normalizeTextInputValue(std::move(value), multiline_);
    setCursor(cursor_);
}

void TextInput::setCursor(int cursor) {
    manualRowScroll_ = false;
    cursor_ = clampIndex(cursor, 0, static_cast<int>(value_.size()));
}

void TextInput::selectRange(int start, int end) {
    manualRowScroll_ = false;
    const int clampedStart = clampIndex(start, 0, static_cast<int>(value_.size()));
    const int clampedEnd = clampIndex(end, 0, static_cast<int>(value_.size()));
    if (clampedStart == clampedEnd) {
        selectionAnchor_.reset();
        cursor_ = clampedEnd;
        return;
    }
    selectionAnchor_ = clampedStart;
    cursor_ = clampedEnd;
}

std::string TextInput::selectedText() const {
    if (!hasSelection()) {
        return {};
    }
    return value_.substr(static_cast<std::size_t>(selectionStart()),
                         static_cast<std::size_t>(selectionEnd() - selectionStart()));
}

int TextInput::cursorForPoint(int row, int col, Size size) const {
    if (!multiline_) {
        (void)row;
        (void)size;
        const VisualTextRow visualRow = makeVisualTextRow(0, visualGlyphs(value_));
        return clampIndex(rawIndexForVisualColumn(visualRow, colScrollOffset_ + col), 0, static_cast<int>(value_.size()));
    }
    const int height = safeHeight(size);
    const int width = safeWidth(size, value_);
    const std::vector<VisualTextRow> rows = buildVisualRows(value_, width);
    const int targetRow = clampIndex(row, 0, height - 1) + rowScrollOffset_;
    if (targetRow >= static_cast<int>(rows.size())) {
        return static_cast<int>(value_.size());
    }
    const VisualTextRow& visualRow = rows[static_cast<std::size_t>(targetRow)];
    return clampIndex(rawIndexForVisualColumn(visualRow, col), visualRow.start, visualRow.end);
}

bool TextInput::scrollByRows(int delta, int viewportHeight, bool manual) {
    if (!multiline_) {
        return false;
    }
    const int height = safeHeight(Size{1, viewportHeight});
    if (height <= 1) {
        return false;
    }
    const int width = safeWidth(Size{frame().width, viewportHeight}, value_);
    const std::vector<VisualTextRow> rows = buildVisualRows(value_, width);
    const int maxOffset = std::max(0, static_cast<int>(rows.size()) - height);
    const int nextOffset = clampIndex(rowScrollOffset_ + delta, 0, maxOffset);
    if (nextOffset == rowScrollOffset_) {
        return false;
    }
    rowScrollOffset_ = nextOffset;
    manualRowScroll_ = manual;
    return true;
}

void TextInput::insertText(std::string_view text) {
    for (std::size_t index = 0; index < text.size(); ++index) {
        const char ch = text[index];
        if (ch == '\r' && index + 1 < text.size() && text[index + 1] == '\n') {
            (void)handleKey(multiline_ ? "Enter" : " ");
            ++index;
            continue;
        }
        const std::string key = (ch == '\r' || ch == '\n') ? std::string(multiline_ ? "Enter" : " ") : std::string(1, ch);
        (void)handleKey(key);
    }
}

bool TextInput::handleKey(std::string_view key) {
    manualRowScroll_ = false;

    if (key == "Left") {
        if (hasSelection()) {
            cursor_ = selectionStart();
            selectionAnchor_.reset();
        } else {
            setCursor(cursor_ - 1);
        }
        return true;
    }
    if (key == "Right") {
        if (hasSelection()) {
            cursor_ = selectionEnd();
            selectionAnchor_.reset();
        } else {
            setCursor(cursor_ + 1);
        }
        return true;
    }
    if (key == "Alt+Left") {
        selectionAnchor_.reset();
        for (int step = 0; step < kTextInputOptionHorizontalSteps; ++step) {
            setCursor(cursor_ - 1);
        }
        return true;
    }
    if (key == "Alt+Right") {
        selectionAnchor_.reset();
        for (int step = 0; step < kTextInputOptionHorizontalSteps; ++step) {
            setCursor(cursor_ + 1);
        }
        return true;
    }
    if (key == "Up") {
        if (!multiline_) {
            return false;
        }
        moveCursorVertical(-1);
        return true;
    }
    if (key == "Alt+Up") {
        if (!multiline_) {
            return false;
        }
        selectionAnchor_.reset();
        for (int step = 0; step < kTextInputOptionVerticalSteps; ++step) {
            moveCursorVertical(-1);
        }
        return true;
    }
    if (key == "Alt+Down") {
        if (!multiline_) {
            return false;
        }
        selectionAnchor_.reset();
        for (int step = 0; step < kTextInputOptionVerticalSteps; ++step) {
            moveCursorVertical(1);
        }
        return true;
    }
    if (key == "Down") {
        if (!multiline_) {
            return false;
        }
        moveCursorVertical(1);
        return true;
    }
    if (key == "Home") {
        setCursor(lineStartForCursor());
        return true;
    }
    if (key == "End") {
        setCursor(lineEndForCursor());
        return true;
    }
    if (key == "Shift+Left") {
        if (!selectionAnchor_.has_value()) {
            selectionAnchor_ = cursor_;
        }
        setCursor(cursor_ - 1);
        return true;
    }
    if (key == "Shift+Right") {
        if (!selectionAnchor_.has_value()) {
            selectionAnchor_ = cursor_;
        }
        setCursor(cursor_ + 1);
        return true;
    }
    if (key == "Shift+Up") {
        if (!multiline_) {
            return false;
        }
        if (!selectionAnchor_.has_value()) {
            selectionAnchor_ = cursor_;
        }
        moveCursorVertical(-1);
        return true;
    }
    if (key == "Shift+Down") {
        if (!multiline_) {
            return false;
        }
        if (!selectionAnchor_.has_value()) {
            selectionAnchor_ = cursor_;
        }
        moveCursorVertical(1);
        return true;
    }
    if (key == "Backspace") {
        if (hasSelection()) {
            deleteSelection();
        } else if (cursor_ > 0) {
            value_.erase(static_cast<std::size_t>(cursor_ - 1), 1);
            --cursor_;
        }
        return true;
    }
    if (key == "Enter" && multiline_) {
        key = "Alt+Enter";
    }
    if (key == "Alt+Enter") {
        if (!multiline_) {
            return false;
        }
        if (maxLength_ > 0 && static_cast<int>(value_.size()) >= maxLength_ && !hasSelection()) {
            return true;
        }
        if (hasSelection()) {
            deleteSelection();
        }
        value_.insert(static_cast<std::size_t>(cursor_), "\n");
        ++cursor_;
        selectionAnchor_.reset();
        return true;
    }
    if (key.size() == 1) {
        if (maxLength_ > 0 && static_cast<int>(value_.size()) >= maxLength_ && !hasSelection()) {
            return true;
        }
        if (hasSelection()) {
            deleteSelection();
        }
        value_.insert(static_cast<std::size_t>(cursor_), std::string(key));
        ++cursor_;
        selectionAnchor_.reset();
        return true;
    }
    return false;
}

RenderedContent TextInput::render(Size size, ElementRenderState state) const {
    const int width = safeWidth(size, value_) + (size.width > 0 ? 0 : 1);
    const int height = multiline_ ? safeHeight(size) : 1;
    const Style base = effectiveStyle(state.focused, state.editMode);
    RenderedContent rendered;
    rendered.reserve(static_cast<std::size_t>(height));

    const Style cursor = mergedOptional(base, cursorStyle());

    if (!multiline_) {
        const std::string text = value_;
        int& scrollOffset = const_cast<TextInput*>(this)->colScrollOffset_;
        scrollOffset = std::max(0, scrollOffset);
        const VisualTextRow row = makeVisualTextRow(0, visualGlyphs(text));
        const int textWidth = static_cast<int>(row.cells.size());
        if (textWidth <= width) {
            scrollOffset = 0;
        }
        const int cursorVisualCol = visualColumnForCursor(row, cursor_, std::max(width, textWidth + 1));
        if (state.editMode) {
            if (cursorVisualCol < scrollOffset) {
                scrollOffset = cursorVisualCol;
            } else if (cursorVisualCol >= scrollOffset + width) {
                scrollOffset = std::max(0, cursorVisualCol - width + 1);
            }
        }
        std::vector<VisualGlyph> visibleCells;
        if (scrollOffset < textWidth) {
            const int end = std::min(textWidth, scrollOffset + width);
            visibleCells.assign(row.cells.begin() + scrollOffset, row.cells.begin() + end);
        }
        RenderedRow renderedRow;
        renderedRow.reserve(static_cast<std::size_t>(width));
        for (const VisualGlyph& glyph : visibleCells) {
            renderedRow.push_back(styledCell(glyph.text, base));
        }
        while (static_cast<int>(renderedRow.size()) < width) {
            renderedRow.push_back(styledCell(" ", base));
        }
        if (!state.editMode && width > 0 && textWidth > scrollOffset + width) {
            renderedRow[static_cast<std::size_t>(width - 1)].text = ">";
        }
        rendered.push_back(std::move(renderedRow));
        if (state.editMode && hasSelection()) {
            for (int col = 0; col < width; ++col) {
                const int source = col < static_cast<int>(visibleCells.size())
                    ? visibleCells[static_cast<std::size_t>(col)].sourceStart
                    : -1;
                if (source >= selectionStart() && source < selectionEnd()) {
                    rendered[0][static_cast<std::size_t>(col)].foreground = cursor.color;
                    rendered[0][static_cast<std::size_t>(col)].background = cursor.background;
                }
            }
        } else if (state.editMode) {
            const int visibleCol = clampIndex(cursorVisualCol - scrollOffset, 0, width - 1);
            rendered[0][static_cast<std::size_t>(visibleCol)].foreground = cursor.color;
            rendered[0][static_cast<std::size_t>(visibleCol)].background = cursor.background;
        }
        return rendered;
    }

    const std::vector<VisualTextRow> rows = buildVisualRows(value_, width);
    const int cursorRow = visualRowForCursor(rows, width, cursor_);
    const_cast<TextInput*>(this)->rowScrollOffset_ = clampIndex(rowScrollOffset_, 0, std::max(0, static_cast<int>(rows.size()) - height));
    if (state.editMode && !manualRowScroll_) {
        if (cursorRow < rowScrollOffset_) {
            const_cast<TextInput*>(this)->rowScrollOffset_ = cursorRow;
        } else if (cursorRow >= rowScrollOffset_ + height) {
            const_cast<TextInput*>(this)->rowScrollOffset_ = cursorRow - height + 1;
        }
    }

    const bool hasAbove = rowScrollOffset_ > 0;
    const bool hasBelow = rowScrollOffset_ + height < static_cast<int>(rows.size());
    const bool selectionActive = state.editMode && hasSelection();
    const int selectionLow = selectionActive ? selectionStart() : 0;
    const int selectionHigh = selectionActive ? selectionEnd() : 0;

    for (int row = 0; row < height; ++row) {
        const int rowIndex = rowScrollOffset_ + row;
        const bool hasVisualRow = rowIndex < static_cast<int>(rows.size());
        const VisualTextRow visualRow = hasVisualRow
            ? rows[static_cast<std::size_t>(rowIndex)]
            : VisualTextRow{};
        RenderedRow renderedRow;
        renderedRow.reserve(static_cast<std::size_t>(width));
        for (const VisualGlyph& glyph : visualRow.cells) {
            if (static_cast<int>(renderedRow.size()) >= width) {
                break;
            }
            renderedRow.push_back(styledCell(glyph.text, base));
        }
        while (static_cast<int>(renderedRow.size()) < width) {
            renderedRow.push_back(styledCell(" ", base));
        }
        rendered.push_back(std::move(renderedRow));

        int cursorCol = -1;
        if (state.editMode && hasVisualRow) {
            cursorCol = visualColumnForCursor(visualRow, cursor_, width);
        }

        const bool firstVisibleRow = row == 0;
        const bool lastVisibleRow = row == height - 1;
        const bool indicatorVisible = width > 0 && static_cast<int>(visualRow.cells.size()) < width &&
            ((firstVisibleRow && hasAbove) || (lastVisibleRow && hasBelow));
        const int indicatorCol = width - 1;
        const bool indicatorOverlapsCursor = cursorCol == indicatorCol;
        const bool indicatorOverlapsSelection = false;
        if (indicatorVisible && !indicatorOverlapsCursor && !indicatorOverlapsSelection) {
            rendered.back()[static_cast<std::size_t>(indicatorCol)].text = firstVisibleRow && hasAbove ? "^" : "v";
        }

        if (selectionActive) {
            for (int col = 0; col < std::min(width, static_cast<int>(visualRow.cells.size())); ++col) {
                const int source = visualRow.cells[static_cast<std::size_t>(col)].sourceStart;
                if (source < selectionLow || source >= selectionHigh) {
                    continue;
                }
                rendered.back()[static_cast<std::size_t>(col)].foreground = cursor.color;
                rendered.back()[static_cast<std::size_t>(col)].background = cursor.background;
            }
        } else if (state.editMode && cursorCol >= 0 && cursorCol < width) {
            rendered.back()[static_cast<std::size_t>(cursorCol)].foreground = cursor.color;
            rendered.back()[static_cast<std::size_t>(cursorCol)].background = cursor.background;
        }
    }
    return rendered;
}

bool TextInput::hasSelection() const {
    return selectionAnchor_.has_value() && *selectionAnchor_ != cursor_;
}

int TextInput::selectionStart() const {
    return hasSelection() ? std::min(*selectionAnchor_, cursor_) : cursor_;
}

int TextInput::selectionEnd() const {
    return hasSelection() ? std::max(*selectionAnchor_, cursor_) : cursor_;
}

int TextInput::lineStartForCursor() const {
    const std::size_t pos = value_.rfind('\n', static_cast<std::size_t>(std::max(0, cursor_ - 1)));
    return pos == std::string::npos ? 0 : static_cast<int>(pos) + 1;
}

int TextInput::lineEndForCursor() const {
    const std::size_t pos = value_.find('\n', static_cast<std::size_t>(cursor_));
    return pos == std::string::npos ? static_cast<int>(value_.size()) : static_cast<int>(pos);
}

void TextInput::moveCursorVertical(int delta) {
    if (multiline_ && frame().width > 0) {
        const int width = safeWidth(Size{frame().width, frame().height}, value_);
        const std::vector<VisualTextRow> rows = buildVisualRows(value_, width);
        const int currentRow = visualRowForCursor(rows, width, cursor_);
        const int targetRow = currentRow + delta;
        if (targetRow < 0 || targetRow >= static_cast<int>(rows.size())) {
            return;
        }
        const VisualTextRow& current = rows[static_cast<std::size_t>(currentRow)];
        const VisualTextRow& target = rows[static_cast<std::size_t>(targetRow)];
        const int currentCol = clampIndex(visualColumnForCursor(current, cursor_, width), 0,
                                          static_cast<int>(current.cells.size()));
        setCursor(rawIndexForVisualColumn(target, currentCol));
        return;
    }

    const int currentStart = lineStartForCursor();
    const int currentCol = cursor_ - currentStart;
    int targetStart = currentStart;
    if (delta < 0) {
        if (currentStart == 0) {
            return;
        }
        const std::size_t previousNewline = value_.rfind('\n', static_cast<std::size_t>(currentStart - 2));
        targetStart = previousNewline == std::string::npos ? 0 : static_cast<int>(previousNewline) + 1;
    } else if (delta > 0) {
        const std::size_t nextNewline = value_.find('\n', static_cast<std::size_t>(cursor_));
        if (nextNewline == std::string::npos) {
            return;
        }
        targetStart = static_cast<int>(nextNewline) + 1;
    }

    const std::size_t targetEndPos = value_.find('\n', static_cast<std::size_t>(targetStart));
    const int targetEnd = targetEndPos == std::string::npos ? static_cast<int>(value_.size()) : static_cast<int>(targetEndPos);
    setCursor(std::min(targetStart + currentCol, targetEnd));
}

void TextInput::deleteSelection() {
    const int start = selectionStart();
    const int end = selectionEnd();
    value_.erase(static_cast<std::size_t>(start), static_cast<std::size_t>(end - start));
    cursor_ = start;
    selectionAnchor_.reset();
}

NumberInput::NumberInput(std::string name, double value, double step)
    : Element(std::move(name)), value_(value), step_(step) {}

void NumberInput::setValue(double value) {
    value_ = value;
    editText_.clear();
    editCursor_ = 0;
    editing_ = false;
    replaceOnFirstTextInput_ = false;
}

void NumberInput::setEditCursor(int cursor) {
    ensureEditText();
    editCursor_ = clampIndex(cursor, 0, static_cast<int>(editText_.size()));
    replaceOnFirstTextInput_ = false;
}

void NumberInput::setEditText(std::string text) {
    editText_ = std::move(text);
    editCursor_ = static_cast<int>(editText_.size());
    editing_ = true;
    replaceOnFirstTextInput_ = false;
}

std::string NumberInput::displayValue() const {
    std::ostringstream out;
    out << std::setprecision(12) << value_;
    std::string text = out.str();
    const std::size_t decimal = text.find('.');
    if (decimal != std::string::npos) {
        while (!text.empty() && text.back() == '0') {
            text.pop_back();
        }
        if (!text.empty() && text.back() == '.') {
            text.pop_back();
        }
    }
    return text.empty() ? "0" : text;
}

bool NumberInput::handleKey(std::string_view key) {
    ensureEditText();
    if (key == "Up") {
        value_ += step_;
        editText_ = displayValue();
        editCursor_ = static_cast<int>(editText_.size());
        replaceOnFirstTextInput_ = false;
        return true;
    }
    if (key == "Down") {
        value_ -= step_;
        editText_ = displayValue();
        editCursor_ = static_cast<int>(editText_.size());
        replaceOnFirstTextInput_ = false;
        return true;
    }
    if (key == "Left") {
        editCursor_ = clampIndex(editCursor_ - 1, 0, static_cast<int>(editText_.size()));
        replaceOnFirstTextInput_ = false;
        return true;
    }
    if (key == "Right") {
        editCursor_ = clampIndex(editCursor_ + 1, 0, static_cast<int>(editText_.size()));
        replaceOnFirstTextInput_ = false;
        return true;
    }
    if (key == "Home") {
        editCursor_ = 0;
        replaceOnFirstTextInput_ = false;
        return true;
    }
    if (key == "End") {
        editCursor_ = static_cast<int>(editText_.size());
        replaceOnFirstTextInput_ = false;
        return true;
    }
    if (key == "Backspace") {
        replaceOnFirstTextInput_ = false;
        if (editCursor_ > 0) {
            editText_.erase(static_cast<std::size_t>(editCursor_ - 1), 1);
            --editCursor_;
        }
        return true;
    }
    if (key == "Enter") {
        commitEdit();
        return true;
    }
    if (key.size() == 1 && (std::isdigit(static_cast<unsigned char>(key.front())) || key == "." || key == "-")) {
        if (replaceOnFirstTextInput_) {
            editText_.clear();
            editCursor_ = 0;
            replaceOnFirstTextInput_ = false;
        }
        editText_.insert(static_cast<std::size_t>(editCursor_), std::string(key));
        ++editCursor_;
        return true;
    }
    return false;
}

RenderedContent NumberInput::render(Size size, ElementRenderState state) const {
    if (state.editMode) {
        const_cast<NumberInput*>(this)->ensureEditText();
    }
    const std::string text = state.editMode ? editText_ : displayValue();
    RenderedContent rendered = renderPlainText(text, safeWidth(size, text), 1, effectiveStyle(state.focused, state.editMode));
    if (state.editMode) {
        const Style cursorStyleValue = mergedOptional(effectiveStyle(state.focused, state.editMode), cursorStyle());
        const int cursorCol = clampIndex(editCursor_, 0, rendered[0].empty() ? 0 : static_cast<int>(rendered[0].size()) - 1);
        rendered[0][static_cast<std::size_t>(cursorCol)].foreground = cursorStyleValue.color;
        rendered[0][static_cast<std::size_t>(cursorCol)].background = cursorStyleValue.background;
    }
    return rendered;
}

void NumberInput::beginEdit() {
    if (editing_) {
        return;
    }
    editOriginalValue_ = value_;
    editText_ = displayValue();
    editCursor_ = static_cast<int>(editText_.size());
    editing_ = true;
    replaceOnFirstTextInput_ = value_ == 0.0;
}

void NumberInput::cancelEdit() {
    if (!editing_) {
        return;
    }
    value_ = editOriginalValue_;
    editText_.clear();
    editCursor_ = 0;
    editing_ = false;
    replaceOnFirstTextInput_ = false;
}

void NumberInput::commitEdit() {
    ensureEditText();
    try {
        value_ = editText_.empty() ? 0.0 : std::stod(editText_);
    } catch (const std::exception&) {
        value_ = editOriginalValue_;
    }
    editText_.clear();
    editCursor_ = 0;
    editing_ = false;
    replaceOnFirstTextInput_ = false;
}

void NumberInput::ensureEditText() {
    if (!editing_) {
        beginEdit();
    }
}

ComboBox::ComboBox(std::string name, std::vector<std::string> options)
    : Element(std::move(name)), options_(std::move(options)) {}

void ComboBox::setOptions(std::vector<std::string> options) {
    options_ = std::move(options);
    setSelectedIndex(selectedIndex_);
}

void ComboBox::setSelectedIndex(int selectedIndex) {
    if (options_.empty()) {
        selectedIndex_ = 0;
        return;
    }
    selectedIndex_ = clampIndex(selectedIndex, 0, static_cast<int>(options_.size()) - 1);
}

bool ComboBox::handleKey(std::string_view key) {
    if (options_.empty()) {
        return false;
    }
    if (key == "Down") {
        setSelectedIndex(selectedIndex_ + 1);
        return true;
    }
    if (key == "Up") {
        setSelectedIndex(selectedIndex_ - 1);
        return true;
    }
    return false;
}

RenderedContent ComboBox::render(Size size, ElementRenderState state) const {
    const std::string selected = options_.empty() ? "" : options_[static_cast<std::size_t>(selectedIndex_)];
    const int width = safeWidth(size, selected) + (size.width > 0 ? 0 : 2);
    const int height = state.editMode ? static_cast<int>(options_.size()) + 1 : 1;
    const Style base = effectiveStyle(state.focused, state.editMode);
    RenderedContent rendered;
    rendered.reserve(static_cast<std::size_t>(height));

    std::string closed = fitText(selected, width);
    if (width > 0 && (state.focused || state.editMode)) {
        closed[static_cast<std::size_t>(width - 1)] = state.editMode ? 'v' : '>';
    }
    rendered.push_back(renderRow(closed, width, base));

    if (state.editMode) {
        for (int row = 1; row < height; ++row) {
            const int optionIndex = row - 1;
            const bool selectedRow = optionIndex == selectedIndex_;
            const std::string text = optionIndex < static_cast<int>(options_.size())
                ? "  " + options_[static_cast<std::size_t>(optionIndex)]
                : "";
            rendered.push_back(renderRow(text, width, selectedRow ? mergedOptional(base, selectedStyle()) : base));
        }
    }
    return rendered;
}

ListBox::ListBox(std::string name, std::vector<std::string> options)
    : Element(std::move(name)), options_(std::move(options)) {
    setSelectedIndex(selectedIndex_);
}

void ListBox::setOptions(std::vector<std::string> options) {
    options_ = std::move(options);
    if (selectedIndices_.empty()) {
        setSelectedIndex(selectedIndex_);
    } else {
        setSelectedIndices(selectedIndices_);
    }
    if (options_.empty()) {
        scrollOffset_ = 0;
    }
}

void ListBox::setDisabledValues(std::vector<std::string> disabledValues) {
    disabledValues_ = std::move(disabledValues);
}

void ListBox::setMultiple(bool multiple) {
    multiple_ = multiple;
    if (!multiple_ && selectedIndices_.size() > 1) {
        setSelectedIndex(selectedIndex_);
    }
}

void ListBox::setSelectedIndex(int selectedIndex) {
    if (options_.empty()) {
        selectedIndex_ = 0;
        selectedIndices_.clear();
        scrollOffset_ = 0;
        return;
    }
    selectedIndex_ = clampIndex(selectedIndex, 0, static_cast<int>(options_.size()) - 1);
    if (!multiple_) {
        selectedIndices_ = {selectedIndex_};
    } else if (selectedIndices_.empty()) {
        selectedIndices_.push_back(selectedIndex_);
    } else {
        selectedIndices_.back() = selectedIndex_;
    }
    if (lastViewportHeight_ > 0) {
        ensureSelectedVisible(lastViewportHeight_);
    }
}

void ListBox::setSelectedIndices(std::vector<int> selectedIndices) {
    selectedIndices_.clear();
    if (options_.empty()) {
        selectedIndex_ = 0;
        scrollOffset_ = 0;
        return;
    }
    for (int selectedIndex : selectedIndices) {
        const int clamped = clampIndex(selectedIndex, 0, static_cast<int>(options_.size()) - 1);
        if (std::find(selectedIndices_.begin(), selectedIndices_.end(), clamped) == selectedIndices_.end()) {
            selectedIndices_.push_back(clamped);
        }
        if (!multiple_) {
            break;
        }
    }
    if (selectedIndices_.empty()) {
        selectedIndex_ = clampIndex(selectedIndex_, 0, static_cast<int>(options_.size()) - 1);
        return;
    }
    selectedIndex_ = selectedIndices_.back();
    if (lastViewportHeight_ > 0) {
        ensureSelectedVisible(lastViewportHeight_);
    }
}

void ListBox::setSelectedValues(const std::vector<std::string>& selectedValues) {
    std::vector<int> indices;
    for (const std::string& selectedValue : selectedValues) {
        const auto found = std::find(options_.begin(), options_.end(), selectedValue);
        if (found != options_.end()) {
            indices.push_back(static_cast<int>(std::distance(options_.begin(), found)));
        }
    }
    setSelectedIndices(std::move(indices));
}

std::vector<std::string> ListBox::selectedValues() const {
    std::vector<std::string> values;
    for (int selectedIndex : selectedIndices_) {
        if (selectedIndex >= 0 && selectedIndex < static_cast<int>(options_.size())) {
            values.push_back(options_[static_cast<std::size_t>(selectedIndex)]);
        }
    }
    return values;
}

void ListBox::scrollBy(int delta, int viewportHeight) {
    if (options_.empty()) {
        scrollOffset_ = 0;
        return;
    }
    const int maxOffset = std::max(0, static_cast<int>(options_.size()) - std::max(1, viewportHeight));
    scrollOffset_ = clampIndex(scrollOffset_ + delta, 0, maxOffset);
}

bool ListBox::handleKey(std::string_view key) {
    if (options_.empty()) {
        return false;
    }
    if (key == "Down") {
        setSelectedIndex(selectedIndex_ + 1);
        return true;
    }
    if (key == "Up") {
        setSelectedIndex(selectedIndex_ - 1);
        return true;
    }
    if (key == "Enter" && multiple_) {
        const auto found = std::find(selectedIndices_.begin(), selectedIndices_.end(), selectedIndex_);
        if (found == selectedIndices_.end()) {
            selectedIndices_.push_back(selectedIndex_);
        } else {
            selectedIndices_.erase(found);
        }
        return true;
    }
    return false;
}

RenderedContent ListBox::render(Size size, ElementRenderState state) const {
    const int height = safeHeight(size);
    const int width = safeWidth(size, options_.empty() ? "" : options_[static_cast<std::size_t>(selectedIndex_)]) + (size.width > 0 ? 0 : 2);
    lastViewportHeight_ = height;
    const int maxOffset = std::max(0, static_cast<int>(options_.size()) - height);
    const_cast<ListBox*>(this)->scrollOffset_ = clampIndex(scrollOffset_, 0, maxOffset);

    const Style base = effectiveStyle(state.focused, state.editMode);
    RenderedContent rendered;
    const bool hasAbove = scrollOffset_ > 0;
    const bool hasBelow = scrollOffset_ + height < static_cast<int>(options_.size());
    for (int row = 0; row < height; ++row) {
        const int optionIndex = scrollOffset_ + row;
        const bool selected = optionIndex < static_cast<int>(options_.size()) &&
            std::find(selectedIndices_.begin(), selectedIndices_.end(), optionIndex) != selectedIndices_.end();
        const bool disabled = optionIndex < static_cast<int>(options_.size()) &&
            std::find(disabledValues_.begin(), disabledValues_.end(), options_[static_cast<std::size_t>(optionIndex)]) != disabledValues_.end();
        std::string text = optionIndex < static_cast<int>(options_.size())
            ? options_[static_cast<std::size_t>(optionIndex)]
            : "";
        text = fitText(std::move(text), width);
        if (row == 0 && hasAbove && width > 0) {
            text[static_cast<std::size_t>(width - 1)] = '^';
        } else if (row == height - 1 && hasBelow && width > 0) {
            text[static_cast<std::size_t>(width - 1)] = 'v';
        }
        Style rowStyle = selected ? mergedOptional(base, selectedStyle()) : base;
        if (disabled) {
            rowStyle = mergedOptional(base, disabledStyle());
        }
        rendered.push_back(renderRow(text, width, rowStyle));
    }
    return rendered;
}

void ListBox::ensureSelectedVisible(int height) {
    if (selectedIndex_ < scrollOffset_) {
        scrollOffset_ = selectedIndex_;
    } else if (selectedIndex_ >= scrollOffset_ + height) {
        scrollOffset_ = selectedIndex_ - height + 1;
    }
}

}  // namespace ui
