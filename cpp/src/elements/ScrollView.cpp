#include "ui/elements/ScrollView.hpp"

#include "ui/elements/ReusableElement.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <cmath>

namespace ui {

namespace {

struct ConstrainedPadding {
    int top = 0;
    int right = 0;
    int bottom = 0;
    int left = 0;
};

[[nodiscard]] int clampNonNegative(int value) {
    return std::max(0, value);
}

[[nodiscard]] int viewportWidth(Size size) {
    return std::max(1, size.width);
}

[[nodiscard]] int viewportHeight(Size size) {
    return std::max(1, size.height);
}

[[nodiscard]] int stylePaddingTop(const Style& style) {
    return std::max(0, style.paddingTop.value_or(style.padding.value_or(0)));
}

[[nodiscard]] int stylePaddingRight(const Style& style) {
    return std::max(0, style.paddingRight.value_or(style.padding.value_or(0)));
}

[[nodiscard]] int stylePaddingBottom(const Style& style) {
    return std::max(0, style.paddingBottom.value_or(style.padding.value_or(0)));
}

[[nodiscard]] int stylePaddingLeft(const Style& style) {
    return std::max(0, style.paddingLeft.value_or(style.padding.value_or(0)));
}

[[nodiscard]] ConstrainedPadding constrainedPadding(Size size, const Style& style) {
    const int width = viewportWidth(size);
    const int height = viewportHeight(size);
    ConstrainedPadding padding;
    padding.left = std::min(stylePaddingLeft(style), width - 1);
    padding.right = std::min(stylePaddingRight(style), width - padding.left - 1);
    padding.top = std::min(stylePaddingTop(style), height - 1);
    padding.bottom = std::min(stylePaddingBottom(style), height - padding.top - 1);
    return padding;
}

[[nodiscard]] Size paddedViewportSize(Size size, ConstrainedPadding padding) {
    return Size{
        viewportWidth(Size{viewportWidth(size) - padding.left - padding.right, size.height}),
        viewportHeight(Size{size.width, viewportHeight(size) - padding.top - padding.bottom}),
    };
}

[[nodiscard]] RenderedRow fitRow(RenderedRow row, int width, const TerminalCell& fillCell) {
    if (static_cast<int>(row.size()) > width) {
        row.resize(static_cast<std::size_t>(width));
    }
    while (static_cast<int>(row.size()) < width) {
        row.push_back(fillCell);
    }
    return row;
}

[[nodiscard]] bool transparentColor(const std::optional<Color>& color) {
    return !color.has_value() || color->kind() == Color::Kind::Unset || color->isTransparent();
}

[[nodiscard]] std::optional<Color> blendOverParent(const std::optional<Color>& color,
                                                   const std::optional<Color>& parent) {
    if (transparentColor(color)) {
        return parent;
    }
    if (color->rgba().has_value() && color->rgba()->alpha < 255 && parent.has_value()) {
        return color->blendOver(*parent);
    }
    return color;
}

void applyScrollIndicator(RenderedRow& row, std::string_view indicator, std::optional<Color> fallbackForeground = std::nullopt) {
    if (row.empty()) {
        return;
    }
    TerminalCell& cell = row.back();
    cell.text = std::string{indicator};
    if (transparentColor(cell.foreground)) {
        cell.foreground = fallbackForeground;
    }
}

[[nodiscard]] std::optional<Color> inferredScrollIndicatorForeground(const RenderedRow& row) {
    std::optional<Color> foreground;
    for (const TerminalCell& cell : row) {
        if (!transparentColor(cell.foreground)) {
            foreground = cell.foreground;
        }
    }
    return foreground;
}

[[nodiscard]] RenderedRow inheritRowStyle(RenderedRow row, const Style& parentStyle) {
    for (TerminalCell& cell : row) {
        cell.background = blendOverParent(cell.background, parentStyle.background);
        if (transparentColor(cell.foreground)) {
            cell.foreground = parentStyle.color;
        }
    }
    return row;
}

[[nodiscard]] Color blendOverExactAlpha(const Color& color, const Color& background) {
    if (!color.rgba().has_value()) {
        return color;
    }
    const Rgba& foreground = *color.rgba();
    if (foreground.alpha >= 255) {
        return color;
    }
    if (foreground.alpha == 0) {
        return background;
    }
    if (!background.rgba().has_value()) {
        return color;
    }

    const Rgba& base = *background.rgba();
    const double alpha = static_cast<double>(foreground.alpha) / 255.0;
    const auto channel = [alpha](std::uint8_t fg, std::uint8_t bg) {
        return static_cast<std::uint8_t>(std::nearbyint(alpha * fg + (1.0 - alpha) * bg));
    };
    return Color::rgb(
        channel(foreground.red, base.red),
        channel(foreground.green, base.green),
        channel(foreground.blue, base.blue));
}

void applyFocusBackgroundToViewport(RenderedContent& content,
                                    const Color& focusBackground,
                                    const std::optional<Color>& baseBackground,
                                    const std::optional<Color>& focusedBackground) {
    std::optional<Color> focusedBaseBackground;
    if (baseBackground.has_value()) {
        focusedBaseBackground = focusBackground.rgba().has_value()
            ? blendOverExactAlpha(focusBackground, *baseBackground)
            : focusBackground;
    }
    for (RenderedRow& row : content) {
        for (TerminalCell& cell : row) {
            const bool matchesBase =
                !cell.background.has_value() ||
                (baseBackground.has_value() && cell.background == baseBackground) ||
                (focusedBackground.has_value() && cell.background == focusedBackground) ||
                (focusedBaseBackground.has_value() && cell.background == focusedBaseBackground);
            if (!matchesBase) {
                continue;
            }
            cell.background = focusBackground.rgba().has_value() && cell.background.has_value()
                ? blendOverExactAlpha(focusBackground, *cell.background)
                : focusBackground;
        }
    }
}

[[nodiscard]] int nativeChildHeight(const Element& child, int width) {
    if (const auto* reusable = dynamic_cast<const ReusableElement*>(&child);
        reusable != nullptr && reusable->child() != nullptr) {
        const Size resolved = generatedWindowContentSizeForWidth(*reusable->child(), std::max(1, width));
        int sourceHeight = 1;
        for (const auto& entry : reusable->child()->generatedLayout()) {
            const int entryHeight = std::max(1, std::min(entry.charsSize.height, entry.sourceCell.height));
            sourceHeight = std::max(sourceHeight,
                                    entry.sourceCell.row + stylePaddingTop(entry.cellStyle) +
                                    entry.relative.row + entryHeight +
                                    stylePaddingBottom(entry.cellStyle));
        }
        return std::max({1, resolved.height, sourceHeight});
    }
    return std::max(1, child.frame().height);
}

}  // namespace

ScrollView::ScrollView(std::string name, int gap)
    : Element(std::move(name)), gap_(clampNonNegative(gap)) {}

void ScrollView::setGap(int gap) {
    gap_ = clampNonNegative(gap);
    invalidateHeightCache();
}

void ScrollView::setAutoScroll(bool autoScroll) {
    autoScroll_ = autoScroll;
    if (autoScroll_) {
        viewOffset_ = 0;
    }
}

ScrollViewPosition ScrollView::scrollPosition() const {
    return ScrollViewPosition{
        .scrollOffset = scrollOffset_,
        .viewOffset = viewOffset_,
        .autoScroll = autoScroll_,
    };
}

void ScrollView::restoreScrollPosition(ScrollViewPosition position) {
    const int total = static_cast<int>(children_.size() + children().size());
    scrollOffset_ = std::clamp(position.scrollOffset, 0, std::max(0, total - 1));
    viewOffset_ = std::max(0, position.viewOffset);
    autoScroll_ = position.autoScroll;
    invalidateHeightCache();
}

void ScrollView::addChild(RenderedContent child) {
    children_.push_back(std::move(child));
    invalidateHeightCache();
    if (autoScroll_) {
        viewOffset_ = 0;
    }
}

void ScrollView::clearChildren() {
    children_.clear();
    clearChildElements();
    scrollOffset_ = 0;
    viewOffset_ = 0;
    lastNaturalSkip_ = 0;
    dynamicChildrenWidth_.reset();
    invalidateHeightCache();
}

void ScrollView::invalidateHeightCache() const {
    cachedChildHeightsWidth_.reset();
    cachedChildHeights_.reset();
}

void ScrollView::setDynamicChildrenRenderer(std::function<std::vector<RenderedContent>(int)> renderer) {
    dynamicChildrenRenderer_ = std::move(renderer);
    invalidateDynamicChildren();
}

void ScrollView::invalidateDynamicChildren() const {
    dynamicChildrenWidth_.reset();
    invalidateHeightCache();
}

const std::vector<int>& ScrollView::cachedChildHeights() const {
    ensureHeightCache();
    return *cachedChildHeights_;
}

int ScrollView::contentHeight(int width) const {
    ensureHeightCache(width);
    int height = 0;
    int itemCount = 0;
    for (std::size_t index = static_cast<std::size_t>(std::max(0, scrollOffset_));
         index < cachedChildHeights_->size();
         ++index) {
        height += (*cachedChildHeights_)[index];
        ++itemCount;
    }
    if (itemCount > 0) {
        height += gap_ * (itemCount - 1);
    }
    return height;
}

int ScrollView::contentHeight() const {
    return contentHeight(viewportWidth(Size{frame().width, frame().height}));
}

int ScrollView::maxViewOffset(Size viewport) const {
    const int naturalSkip = clampNonNegative(contentHeight(viewportWidth(viewport)) - viewportHeight(viewport));
    syncViewOffsetForNaturalSkip(naturalSkip);
    lastNaturalSkip_ = naturalSkip;
    return naturalSkip;
}

std::optional<ScrollViewContentWindow> ScrollView::contentWindow(Size viewport) const {
    viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    refreshDynamicChildren(viewport.width);
    const int totalChildren = static_cast<int>(children_.size() + children().size());
    if (totalChildren == 0) {
        lastNaturalSkip_ = 0;
        return ScrollViewContentWindow{
            .viewport = viewport,
            .actualSkip = 0,
            .naturalSkip = 0,
            .gap = 0,
        };
    }
    const int naturalSkip = maxViewOffset(viewport);
    const int clampedViewOffset = std::min(viewOffset_, naturalSkip);
    return ScrollViewContentWindow{
        .viewport = viewport,
        .actualSkip = naturalSkip - clampedViewOffset,
        .naturalSkip = naturalSkip,
        .gap = std::max(0, gap_),
    };
}

bool ScrollView::scrollTo(int index, Size viewport) {
    const int total = static_cast<int>(children_.size() + children().size());
    if (total == 0) {
        const int previous = scrollOffset_;
        scrollOffset_ = 0;
        return scrollOffset_ != previous;
    }

    viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    const int previous = scrollOffset_;
    const int maxVisible = maxChildrenInViewport(viewport);
    const int maxOffset = std::max(0, total - maxVisible);
    scrollOffset_ = std::clamp(index, 0, maxOffset);
    if (scrollOffset_ == 0) {
        autoScroll_ = false;
    }
    if (scrollOffset_ != previous) {
        return true;
    }
    return false;
}

bool ScrollView::scrollToBottom(Size viewport) {
    (void)maxViewOffset(paddedViewportSize(viewport, constrainedPadding(viewport, style())));
    const bool changed = scrollOffset_ != 0 || viewOffset_ != 0 || !autoScroll_;
    if (viewOffset_ != 0) {
        pendingTerminalScrollDelta_ -= viewOffset_;
    }
    scrollOffset_ = 0;
    viewOffset_ = 0;
    autoScroll_ = true;
    return changed;
}

bool ScrollView::scrollBy(int delta, Size viewport) {
    return scrollTo(scrollOffset_ + delta, viewport);
}

bool ScrollView::scrollLines(int delta, Size viewport) {
    viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    const int maxOffset = maxViewOffset(viewport);
    const int previous = std::clamp(viewOffset_, 0, maxOffset);
    viewOffset_ = std::clamp(previous + delta, 0, maxOffset);
    autoScroll_ = viewOffset_ == 0;
    if (viewOffset_ != previous) {
        pendingTerminalScrollDelta_ += viewOffset_ - previous;
    }
    return viewOffset_ != previous;
}

int ScrollView::consumeTerminalScrollDelta() const {
    const int delta = pendingTerminalScrollDelta_;
    pendingTerminalScrollDelta_ = 0;
    return delta;
}

bool ScrollView::scrollToTop(Size viewport) {
    viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    const int previous = viewOffset_;
    viewOffset_ = maxViewOffset(viewport);
    autoScroll_ = false;
    return viewOffset_ != previous;
}

bool ScrollView::scrollHorizontal(int delta) {
    const int previous = hViewOffset_;
    hViewOffset_ = std::max(0, hViewOffset_ + delta);
    return hViewOffset_ != previous;
}

bool ScrollView::scrollPageUp(Size viewport) {
    const Size paddedViewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    return scrollLines(std::max(1, viewportHeight(paddedViewport)), viewport);
}

bool ScrollView::scrollPageDown(Size viewport) {
    const Size paddedViewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    return scrollLines(-std::max(1, viewportHeight(paddedViewport)), viewport);
}

int ScrollView::wheelScrollLines(Size viewport) const {
    viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    const int viewportLimited = std::max(1, viewportHeight(viewport) / kWheelScrollViewportFraction);
    return std::max(1, std::min(kDefaultWheelScrollLines, viewportLimited));
}

bool ScrollView::canScrollUp(Size viewport) const {
    (void)viewport;
    return scrollOffset_ > 0;
}

bool ScrollView::canScrollDown(Size viewport) const {
    const int total = static_cast<int>(children_.size() + children().size());
    if (total == 0) {
        return false;
    }
    viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    const int maxVisible = maxChildrenInViewport(viewport);
    const int visibleFromOffset = total - scrollOffset_;
    return visibleFromOffset > maxVisible;
}

bool ScrollView::handleWheel(int wheelDelta, Size viewport) {
    if (wheelDelta == 0) {
        return false;
    }

    const int step = wheelScrollLines(viewport) * std::max(1, std::abs(wheelDelta));
    if (wheelDelta > 0) {
        return scrollLines(step, viewport);
    }
    return scrollLines(-step, viewport);
}

bool ScrollView::handleKey(std::string_view key, Size viewport) {
    const bool scrollY = style().scrollY.value_or(true);
    const bool scrollX = style().scrollX.value_or(false);
    if (key == "Alt+Up") {
        const int step = wheelScrollLines(viewport);
        return scrollLines(step, viewport);
    }
    if (key == "Alt+Down") {
        const int step = wheelScrollLines(viewport);
        return scrollLines(-step, viewport);
    }
    if (key == "Ctrl+Up") {
        return scrollPageUp(viewport);
    }
    if (key == "Ctrl+Down") {
        return scrollPageDown(viewport);
    }
    if (scrollY) {
        if (key == "ArrowUp" || key == "Up") {
            return scrollLines(1, viewport);
        }
        if (key == "ArrowDown" || key == "Down") {
            return scrollLines(-1, viewport);
        }
        if (key == "PageUp") {
            return scrollPageUp(viewport);
        }
        if (key == "PageDown") {
            return scrollPageDown(viewport);
        }
        if (key == "Home") {
            return scrollToTop(viewport);
        }
        if (key == "End") {
            return scrollToBottom(viewport);
        }
    }
    if (scrollX) {
        if (key == "ArrowLeft" || key == "Left") {
            return scrollHorizontal(-1);
        }
        if (key == "ArrowRight" || key == "Right") {
            return scrollHorizontal(1);
        }
    }
    return false;
}

std::vector<ScrollViewChildView> ScrollView::childViews(Size size) const {
    const ConstrainedPadding padding = constrainedPadding(size, style());
    const Size viewport = paddedViewportSize(size, padding);
    refreshDynamicChildren(viewport.width);
    ensureHeightCache(viewport.width);
    ensureHeightCache(viewport.width);
    const int skip = actualSkip(viewport);

    std::vector<ScrollViewChildView> views;
    int globalRow = 0;
    const int totalItems = static_cast<int>(children_.size() + children().size());
    int itemIndex = 0;

    auto appendGap = [&] {
        if (itemIndex >= scrollOffset_ && itemIndex + 1 < totalItems) {
            globalRow += gap_;
        }
        ++itemIndex;
    };

    for (std::size_t index = 0; index < children_.size(); ++index) {
        if (itemIndex >= scrollOffset_) {
            const int childHeight = static_cast<int>(children_[index].size());
            const int localRow = padding.top + globalRow - skip;
            const int visibleTop = std::max(localRow, padding.top);
            const int visibleBottom = std::min(localRow + childHeight, padding.top + viewport.height);
            const bool visible = visibleBottom > visibleTop;
            const bool clipped = visible && (visibleTop > localRow || visibleBottom < localRow + childHeight);
            const int visibleHeight = visible ? visibleBottom - visibleTop : 0;
            views.push_back(ScrollViewChildView{
                .element = nullptr,
                .frame = Rect{visibleTop, padding.left, std::max(1, viewport.width), visibleHeight},
                .visible = visible,
                .index = itemIndex,
                .clipped = clipped,
            });
            globalRow += childHeight;
        }
        appendGap();
    }

    for (const auto& child : children()) {
        if (child == nullptr) {
            appendGap();
            continue;
        }
        const int childWidth = viewport.width;
        const int childHeight = itemIndex >= 0 && itemIndex < static_cast<int>(cachedChildHeights_->size())
            ? (*cachedChildHeights_)[static_cast<std::size_t>(itemIndex)]
            : nativeChildHeight(*child, childWidth);
        if (itemIndex >= scrollOffset_) {
            const int localRow = padding.top + globalRow - skip;
            const int visibleTop = std::max(localRow, padding.top);
            const int visibleBottom = std::min(localRow + childHeight, padding.top + viewport.height);
            const bool visible = visibleBottom > visibleTop;
            const bool clipped = visible && (visibleTop > localRow || visibleBottom < localRow + childHeight);
            views.push_back(ScrollViewChildView{
                .element = child.get(),
                .frame = Rect{localRow, padding.left, std::max(1, childWidth), childHeight},
                .visible = visible,
                .index = itemIndex,
                .clipped = clipped,
            });
            globalRow += childHeight;
        }
        appendGap();
    }

    return views;
}

bool ScrollView::ensureChildVisible(const Element* child, Size size) {
    if (child == nullptr) {
        return false;
    }
    const ConstrainedPadding padding = constrainedPadding(size, style());
    const Size viewport = paddedViewportSize(size, padding);
    refreshDynamicChildren(viewport.width);

    int globalRow = 0;
    const int totalItems = static_cast<int>(children_.size() + children().size());
    int itemIndex = 0;
    auto appendGap = [&] {
        if (itemIndex >= scrollOffset_ && itemIndex + 1 < totalItems) {
            globalRow += gap_;
        }
        ++itemIndex;
    };

    for (const RenderedContent& renderedChild : children_) {
        if (itemIndex >= scrollOffset_) {
            globalRow += static_cast<int>(renderedChild.size());
        }
        appendGap();
    }

    for (const auto& nativeChild : children()) {
        if (nativeChild == nullptr) {
            appendGap();
            continue;
        }
        const int childHeight = itemIndex >= 0 && itemIndex < static_cast<int>(cachedChildHeights_->size())
            ? (*cachedChildHeights_)[static_cast<std::size_t>(itemIndex)]
            : nativeChildHeight(*nativeChild, viewport.width);
        if (nativeChild.get() == child) {
            if (itemIndex < scrollOffset_) {
                return false;
            }
            const int naturalSkip = maxViewOffset(viewport);
            const int currentActualSkip = naturalSkip - std::min(viewOffset_, naturalSkip);
            int nextActualSkip = currentActualSkip;
            if (globalRow < currentActualSkip) {
                nextActualSkip = globalRow;
            } else if (globalRow + childHeight > currentActualSkip + viewport.height) {
                nextActualSkip = globalRow + childHeight - viewport.height;
            }
            nextActualSkip = std::clamp(nextActualSkip, 0, naturalSkip);
            const int nextViewOffset = naturalSkip - nextActualSkip;
            const int previous = viewOffset_;
            viewOffset_ = nextViewOffset;
            autoScroll_ = viewOffset_ == 0;
            return viewOffset_ != previous;
        }
        if (itemIndex >= scrollOffset_) {
            globalRow += childHeight;
        }
        appendGap();
    }
    return false;
}

RenderedContent ScrollView::render(Size size, ElementRenderState state) const {
    const Style effective = effectiveStyle(state.focused, state.editMode);
    RenderedContent rendered = renderWithStyle(size, effective, effective);
    if (state.passiveFocus && focusStyle().has_value() && focusStyle()->background.has_value()) {
        applyFocusBackgroundToViewport(rendered, *focusStyle()->background, style().background, effective.background);
    }
    return rendered;
}

RenderedContent ScrollView::renderWithStyle(Size size, const Style& style) const {
    return renderWithStyle(size, style, style);
}

RenderedContent ScrollView::renderWithStyle(Size size, const Style& style, const Style& childInheritStyle) const {
    const int hOffset = std::max(0, hViewOffset_);
    const int width = viewportWidth(size);
    const int height = viewportHeight(size);
    const ConstrainedPadding padding = constrainedPadding(size, style);
    const Size viewport = paddedViewportSize(size, padding);
    const int renderWidth = viewport.width + hOffset;
    refreshDynamicChildren(renderWidth);
    const int naturalSkip = maxViewOffset(viewport);
    ensureHeightCache(renderWidth);
    const int clampedViewOffset = std::min(viewOffset_, naturalSkip);
    const int skip = naturalSkip - clampedViewOffset;

    RenderedContent visible;
    visible.reserve(static_cast<std::size_t>(viewport.height));
    const RenderedRow blank = blankRow(viewport.width, style);
    const RenderedRow renderBlank = hOffset > 0 ? blankRow(renderWidth, style) : blank;

    int globalRow = 0;
    const int totalItems = static_cast<int>(children_.size() + children().size());
    int itemIndex = 0;
    const int viewportStart = skip;
    const int viewportEnd = skip + viewport.height;
    auto appendVisibleRows = [&](const RenderedContent& rows) {
        const int startRow = globalRow;
        const int endRow = globalRow + static_cast<int>(rows.size());
        if (endRow > viewportStart && startRow < viewportEnd) {
            const int start = std::max(viewportStart, startRow) - startRow;
            const int end = std::min(viewportEnd, endRow) - startRow;
            const RenderedRow& fillRow = hOffset > 0 ? renderBlank : blank;
            const TerminalCell fillCell = fillRow.empty() ? TerminalCell{} : fillRow.front();
            for (int row = start; row < end && static_cast<int>(visible.size()) < viewport.height; ++row) {
                visible.push_back(inheritRowStyle(fitRow(rows[static_cast<std::size_t>(row)], renderWidth, fillCell),
                                                  childInheritStyle));
            }
        }
        globalRow = endRow;
    };
    auto appendVisibleBlank = [&](int rowCount) {
        RenderedContent rows;
        rows.reserve(static_cast<std::size_t>(rowCount));
        for (int row = 0; row < rowCount; ++row) {
            rows.push_back(hOffset > 0 ? renderBlank : blank);
        }
        appendVisibleRows(rows);
    };
    auto appendGap = [&] {
        if (itemIndex >= scrollOffset_ && itemIndex + 1 < totalItems) {
            appendVisibleBlank(gap_);
        }
        ++itemIndex;
    };

    for (std::size_t childIndex = 0; childIndex < children_.size() && static_cast<int>(visible.size()) < viewport.height; ++childIndex) {
        const RenderedContent& child = children_[childIndex];
        if (itemIndex >= scrollOffset_) {
            appendVisibleRows(child);
        }
        appendGap();
    }

    for (const auto& child : children()) {
        if (static_cast<int>(visible.size()) >= viewport.height) {
            break;
        }
        if (itemIndex >= scrollOffset_) {
            const int childHeight = itemIndex >= 0 && itemIndex < static_cast<int>(cachedChildHeights_->size())
                ? (*cachedChildHeights_)[static_cast<std::size_t>(itemIndex)]
                : (child == nullptr ? 0 : nativeChildHeight(*child, renderWidth));
            appendVisibleBlank(childHeight);
        }
        appendGap();
    }

    while (static_cast<int>(visible.size()) < viewport.height) {
        visible.push_back(hOffset > 0 ? renderBlank : blank);
    }
    if (hOffset > 0) {
        const TerminalCell fillCell = blank.empty() ? TerminalCell{} : blank.front();
        for (RenderedRow& row : visible) {
            if (static_cast<int>(row.size()) > hOffset) {
                row.erase(row.begin(), row.begin() + hOffset);
            } else {
                row.assign(static_cast<std::size_t>(viewport.width), fillCell);
            }
            if (static_cast<int>(row.size()) > viewport.width) {
                row.resize(static_cast<std::size_t>(viewport.width));
            }
            while (static_cast<int>(row.size()) < viewport.width) {
                row.push_back(fillCell);
            }
        }
    }
    if (skip > 0 && !visible.empty() && !visible.front().empty()) {
        applyScrollIndicator(visible.front(), kScrollIndicatorAbove, inferredScrollIndicatorForeground(visible.front()));
    }
    if (clampedViewOffset > 0 && !visible.empty() && !visible.back().empty()) {
        applyScrollIndicator(visible.back(), kScrollIndicatorBelow, inferredScrollIndicatorForeground(visible.back()));
    }

    RenderedContent padded;
    padded.reserve(static_cast<std::size_t>(height));
    const RenderedRow fullBlank = blankRow(width, style);
    while (static_cast<int>(padded.size()) < padding.top) {
        padded.push_back(fullBlank);
    }
    for (const RenderedRow& row : visible) {
        RenderedRow paddedRow = fullBlank;
        for (std::size_t col = 0; col < row.size() && padding.left + static_cast<int>(col) < width; ++col) {
            paddedRow[static_cast<std::size_t>(padding.left) + col] = row[col];
        }
        padded.push_back(std::move(paddedRow));
    }
    while (static_cast<int>(padded.size()) < height) {
        padded.push_back(fullBlank);
    }
    return padded;
}

void ScrollView::ensureHeightCache() const {
    const Size size{frame().width, frame().height};
    ensureHeightCache(paddedViewportSize(size, constrainedPadding(size, style())).width);
}

void ScrollView::ensureHeightCache(int width) const {
    width = viewportWidth(Size{width, 1});
    if (cachedChildHeights_.has_value() &&
        cachedChildHeightsWidth_.has_value() &&
        *cachedChildHeightsWidth_ == width) {
        return;
    }
    refreshDynamicChildren(width);
    if (cachedChildHeights_.has_value() &&
        cachedChildHeightsWidth_.has_value() &&
        *cachedChildHeightsWidth_ == width) {
        return;
    }
    std::vector<int> heights;
    heights.reserve(children_.size() + children().size());
    for (const RenderedContent& child : children_) {
        heights.push_back(static_cast<int>(child.size()));
    }
    for (const auto& child : children()) {
        heights.push_back(child == nullptr ? 0 : nativeChildHeight(*child, width));
    }
    cachedChildHeightsWidth_ = width;
    cachedChildHeights_ = std::move(heights);
}

void ScrollView::syncViewOffsetForNaturalSkip(int naturalSkip) const {
    const int previousNaturalSkip = std::max(0, lastNaturalSkip_);
    naturalSkip = std::max(0, naturalSkip);
    if (autoScroll_) {
        viewOffset_ = 0;
    } else if (viewOffset_ >= previousNaturalSkip) {
        viewOffset_ = naturalSkip;
    } else if (viewOffset_ > naturalSkip) {
        viewOffset_ = naturalSkip;
    }
}

int ScrollView::actualSkip(Size viewport) const {
    const int naturalSkip = maxViewOffset(viewport);
    const int clampedViewOffset = std::min(viewOffset_, naturalSkip);
    return naturalSkip - clampedViewOffset;
}

int ScrollView::maxChildrenInViewport(Size viewport) const {
    const int total = static_cast<int>(children_.size() + children().size());
    if (total == 0) {
        return 0;
    }
    const int totalHeight = viewportHeight(viewport);
    const int sampleSize = std::min(kWheelScrollViewportFraction + 2, total);
    if (sampleSize == 0) {
        return 1;
    }

    double measured = 0.0;
    int measuredCount = 0;
    ensureHeightCache(viewportWidth(viewport));
    for (int index = 0; index < sampleSize && index < static_cast<int>(cachedChildHeights_->size()); ++index) {
        measured += (*cachedChildHeights_)[static_cast<std::size_t>(index)];
        ++measuredCount;
    }
    const double averageHeight = std::max(1.0, measuredCount == 0 ? 1.0 : measured / measuredCount);
    const int denominator = static_cast<int>(averageHeight) + gap_;
    if (denominator <= 0) {
        return 1;
    }
    return std::max(1, (totalHeight + gap_) / denominator);
}

RenderedRow ScrollView::blankRow(int width, const Style& style) const {
    RenderedRow row;
    row.reserve(static_cast<std::size_t>(width));
    for (int col = 0; col < width; ++col) {
        row.push_back(TerminalCell{
            .text = " ",
            .foreground = std::nullopt,
            .background = style.background,
        });
    }
    return row;
}

void ScrollView::refreshDynamicChildren(int width) const {
    if (!dynamicChildrenRenderer_) {
        return;
    }
    width = viewportWidth(Size{width, 1});
    if (dynamicChildrenWidth_.has_value() && *dynamicChildrenWidth_ == width) {
        return;
    }
    children_ = dynamicChildrenRenderer_(width);
    dynamicChildrenWidth_ = width;
    invalidateHeightCache();
    if (autoScroll_) {
        viewOffset_ = 0;
    }
}

}  // namespace ui
