#include "ui/elements/ScrollView.hpp"

#include "ui/elements/Label.hpp"
#include "ui/elements/ReusableElement.hpp"
#include "ui/elements/TextInput.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <unordered_map>

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

void resetVirtualChildForReuse(Element& child) {
    if (auto* label = dynamic_cast<Label*>(&child)) {
        label->clearSelection();
    }
    if (auto* textInput = dynamic_cast<TextInput*>(&child)) {
        textInput->selectRange(textInput->cursor(), textInput->cursor());
    }
    for (const auto& nested : child.children()) {
        resetVirtualChildForReuse(*nested);
    }
    if (auto* reusable = dynamic_cast<ReusableElement*>(&child); reusable != nullptr && reusable->child() != nullptr) {
        for (const auto& nested : reusable->child()->elements()) {
            resetVirtualChildForReuse(*nested);
        }
    }
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
        return std::max(1, resolved.height);
    }
    return std::max(1, child.frame().height);
}

}  // namespace

class ScrollView::VirtualHeightIndex {
public:
    VirtualHeightIndex(int count, int estimatedHeight, int gap)
        : count_(std::max(0, count)),
          estimatedHeight_(std::max(1, estimatedHeight)),
          gap_(std::max(0, gap)) {}

    [[nodiscard]] int count() const { return count_; }
    [[nodiscard]] int estimatedHeight() const { return estimatedHeight_; }
    [[nodiscard]] int gap() const { return gap_; }

    [[nodiscard]] std::int64_t extentBefore(int index) const {
        const int end = std::clamp(index, 0, count_);
        return static_cast<std::int64_t>(end) * unitHeight() + deltaPrefix(end);
    }

    [[nodiscard]] std::int64_t contentHeight() const {
        return count_ <= 0 ? 0 : std::max<std::int64_t>(0, extentBefore(count_) - gap_);
    }

    [[nodiscard]] int itemHeight(int index) const {
        if (index < 0 || index >= count_) {
            throw std::out_of_range("virtual ScrollView item index out of range");
        }
        const std::int64_t delta = deltaPrefix(index + 1) - deltaPrefix(index);
        return std::max(1, estimatedHeight_ + static_cast<int>(delta));
    }

    bool setItemHeight(int index, int height) {
        height = std::max(1, height);
        const std::int64_t previousDelta = itemHeight(index) - estimatedHeight_;
        const std::int64_t nextDelta = height - estimatedHeight_;
        const std::int64_t change = nextDelta - previousDelta;
        for (int treeIndex = index + 1; change != 0 && treeIndex <= count_; treeIndex += treeIndex & -treeIndex) {
            const std::int64_t value = tree_[treeIndex] + change;
            if (value == 0) {
                tree_.erase(treeIndex);
            } else {
                tree_[treeIndex] = value;
            }
        }
        return change != 0;
    }

    bool resetItemHeight(int index) {
        return setItemHeight(index, estimatedHeight_);
    }

    [[nodiscard]] int indexAtOffset(std::int64_t offset) const {
        if (count_ <= 0) {
            return 0;
        }
        const std::int64_t target = std::clamp<std::int64_t>(offset, 0, contentHeight());
        int index = 0;
        std::int64_t prefix = 0;
        int bit = 1;
        while (bit <= count_ / 2) {
            bit <<= 1;
        }
        while (bit > 0) {
            const int candidate = index + bit;
            if (candidate <= count_) {
                const int nodeWidth = candidate & -candidate;
                const auto found = tree_.find(candidate);
                const std::int64_t delta = found == tree_.end() ? 0 : found->second;
                const std::int64_t nodeExtent = static_cast<std::int64_t>(nodeWidth) * unitHeight() + delta;
                if (prefix + nodeExtent <= target) {
                    index = candidate;
                    prefix += nodeExtent;
                }
            }
            bit >>= 1;
        }
        return std::min(index, count_ - 1);
    }

private:
    [[nodiscard]] std::int64_t unitHeight() const {
        return static_cast<std::int64_t>(estimatedHeight_) + gap_;
    }

    [[nodiscard]] std::int64_t deltaPrefix(int end) const {
        std::int64_t total = 0;
        for (int index = std::clamp(end, 0, count_); index > 0; index -= index & -index) {
            const auto found = tree_.find(index);
            if (found != tree_.end()) {
                total += found->second;
            }
        }
        return total;
    }

    int count_ = 0;
    int estimatedHeight_ = 1;
    int gap_ = 0;
    std::unordered_map<int, std::int64_t> tree_;
};

ScrollView::ScrollView(std::string name, int gap)
    : Element(std::move(name)), gap_(clampNonNegative(gap)) {}

ScrollView::~ScrollView() = default;

void ScrollView::setGap(int gap) {
    gap_ = clampNonNegative(gap);
    invalidateHeightCache();
    rebuildVirtualHeightIndex();
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
    refreshDynamicChildren(std::max(1, frame().width));
    const int total = virtualDataSource_.has_value()
        ? virtualCount()
        : static_cast<int>(children_.size() + children().size());
    scrollOffset_ = std::clamp(position.scrollOffset, 0, std::max(0, total - 1));
    viewOffset_ = std::max(0, position.viewOffset);
    autoScroll_ = position.autoScroll;
    invalidateHeightCache();
    const Size frameViewport{
        .width = std::max(1, frame().width),
        .height = std::max(1, frame().height),
    };
    const Size viewport = paddedViewportSize(
        frameViewport,
        constrainedPadding(frameViewport, style()));
    const int naturalSkip = clampNonNegative(
        contentHeight(viewportWidth(viewport)) - viewportHeight(viewport));
    viewOffset_ = autoScroll_ ? 0 : std::min(viewOffset_, naturalSkip);
    lastNaturalSkip_ = naturalSkip;
    pendingTerminalScrollDelta_ = 0;
}

Element& ScrollView::addChild(std::unique_ptr<Element> child) {
    if (virtualDataSource_.has_value()) {
        clearVirtualDataSource();
    }
    Element& added = Element::addChild(std::move(child));
    invalidateHeightCache();
    if (autoScroll_) {
        viewOffset_ = 0;
    }
    return added;
}

void ScrollView::addChild(RenderedContent child) {
    if (virtualDataSource_.has_value()) {
        clearVirtualDataSource();
    }
    children_.push_back(std::move(child));
    invalidateHeightCache();
    if (autoScroll_) {
        viewOffset_ = 0;
    }
}

void ScrollView::clearChildren() {
    if (virtualDataSource_.has_value()) {
        clearVirtualDataSource();
        return;
    }
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

void ScrollView::setVirtualDataSource(VirtualScrollDataSource source) {
    if (!source.itemCount || !source.itemKey || !source.materialize) {
        throw std::invalid_argument("virtual ScrollView requires itemCount, itemKey, and materialize callbacks");
    }
    if (source.itemCount() < 0) {
        throw std::invalid_argument("virtual ScrollView item count must not be negative");
    }
    children_.clear();
    clearChildElements();
    virtualRecyclePool_.clear();
    virtualMaterialized_.clear();
    virtualDataSource_ = std::move(source);
    virtualHeightIndex_.reset();
    virtualHeightWidth_.reset();
    scrollOffset_ = 0;
    viewOffset_ = 0;
    lastNaturalSkip_ = 0;
    invalidateHeightCache();
}

void ScrollView::clearVirtualDataSource() {
    if (!virtualDataSource_.has_value()) {
        return;
    }
    clearChildElements();
    virtualRecyclePool_.clear();
    virtualMaterialized_.clear();
    virtualDataSource_.reset();
    virtualHeightIndex_.reset();
    virtualHeightWidth_.reset();
    virtualPinnedElement_ = nullptr;
    scrollOffset_ = 0;
    viewOffset_ = 0;
    lastNaturalSkip_ = 0;
    invalidateHeightCache();
}

void ScrollView::virtualItemsInserted(int index, int count) {
    if (!virtualDataSource_.has_value() || count <= 0) {
        return;
    }
    index = std::max(0, index);
    std::optional<VirtualAnchor> anchor = captureVirtualAnchor();
    std::map<int, VirtualMaterializedRow> shifted;
    for (auto& [oldIndex, row] : virtualMaterialized_) {
        shifted.emplace(oldIndex >= index ? oldIndex + count : oldIndex, std::move(row));
    }
    virtualMaterialized_ = std::move(shifted);
    sortVirtualChildren();
    rebuildVirtualHeightIndex();
    if (anchor.has_value() && index <= anchor->index) {
        anchor->index += count;
    }
    restoreVirtualAnchor(anchor);
}

void ScrollView::virtualItemsRemoved(int index, int count) {
    if (!virtualDataSource_.has_value() || count <= 0) {
        return;
    }
    const int first = std::max(0, index);
    const int end = first + count;
    std::optional<VirtualAnchor> anchor = captureVirtualAnchor();
    std::map<int, VirtualMaterializedRow> shifted;
    std::vector<Element*> removed;
    for (auto& [oldIndex, row] : virtualMaterialized_) {
        if (oldIndex >= first && oldIndex < end) {
            removed.push_back(row.element);
            continue;
        }
        shifted.emplace(oldIndex >= end ? oldIndex - count : oldIndex, std::move(row));
    }
    auto& owned = Element::children_;
    for (Element* element : removed) {
        const auto found = std::find_if(owned.begin(), owned.end(), [element](const auto& child) {
            return child.get() == element;
        });
        if (found != owned.end()) {
            std::unique_ptr<Element> child = std::move(*found);
            owned.erase(found);
            recycleVirtualChild(std::move(child));
        }
        if (virtualPinnedElement_ == element) {
            virtualPinnedElement_ = nullptr;
        }
    }
    virtualMaterialized_ = std::move(shifted);
    rebuildVirtualHeightIndex();
    sortVirtualChildren();
    if (anchor.has_value()) {
        if (anchor->index >= end) {
            anchor->index -= count;
        } else if (anchor->index >= first) {
            anchor->index = std::min(first, std::max(0, virtualCount() - 1));
            anchor->within = 0;
        }
    }
    restoreVirtualAnchor(anchor);
}

void ScrollView::virtualItemsReplaced(int index, int count) {
    if (!virtualDataSource_.has_value() || count <= 0) {
        return;
    }
    const int first = std::max(0, index);
    const int end = std::min(virtualCount(), first + count);
    const std::optional<VirtualAnchor> anchor = captureVirtualAnchor();
    auto& owned = Element::children_;
    for (int itemIndex = first; itemIndex < end; ++itemIndex) {
        const auto row = virtualMaterialized_.find(itemIndex);
        if (row != virtualMaterialized_.end()) {
            Element* element = row->second.element;
            const std::string nextKey = virtualDataSource_->itemKey(itemIndex);
            const auto found = std::find_if(owned.begin(), owned.end(), [element](const auto& child) {
                return child.get() == element;
            });
            if (found != owned.end()) {
                std::unique_ptr<Element> child = std::move(*found);
                owned.erase(found);
                if (row->second.key == nextKey) {
                    child = virtualDataSource_->materialize(itemIndex, std::move(child));
                    if (!child) {
                        throw std::runtime_error("virtual ScrollView materializer must return a child");
                    }
                    Element* pointer = child.get();
                    const_cast<ScrollView*>(this)->Element::addChild(std::move(child));
                    row->second = VirtualMaterializedRow{.key = nextKey, .element = pointer};
                } else {
                    recycleVirtualChild(std::move(child));
                    virtualMaterialized_.erase(row);
                }
            } else {
                virtualMaterialized_.erase(row);
            }
        }
        if (virtualHeightIndex_ && itemIndex < virtualHeightIndex_->count()) {
            virtualHeightIndex_->resetItemHeight(itemIndex);
        }
    }
    sortVirtualChildren();
    restoreVirtualAnchor(anchor);
}

void ScrollView::invalidateVirtualHeights() {
    const std::optional<VirtualAnchor> anchor = captureVirtualAnchor();
    rebuildVirtualHeightIndex();
    restoreVirtualAnchor(anchor);
}

void ScrollView::invalidateVirtualHeights(int index, int count) {
    if (!virtualHeightIndex_ || count <= 0) {
        return;
    }
    const std::optional<VirtualAnchor> anchor = captureVirtualAnchor();
    const int first = std::clamp(index, 0, virtualHeightIndex_->count());
    const int end = std::clamp(first + count, first, virtualHeightIndex_->count());
    for (int itemIndex = first; itemIndex < end; ++itemIndex) {
        virtualHeightIndex_->resetItemHeight(itemIndex);
    }
    restoreVirtualAnchor(anchor);
}

bool ScrollView::hasVirtualDataSource() const {
    return virtualDataSource_.has_value();
}

VirtualScrollDiagnostics ScrollView::virtualDiagnostics() const {
    VirtualScrollDiagnostics result = virtualDiagnostics_;
    result.logicalItems = virtualCount();
    result.materializedRows = static_cast<int>(virtualMaterialized_.size());
    result.recycledRows = static_cast<int>(virtualRecyclePool_.size());
    return result;
}

void ScrollView::setVirtualPinnedElement(const Element* element) const {
    virtualPinnedElement_ = element;
}

int ScrollView::virtualCount() const {
    if (!virtualDataSource_.has_value()) {
        return 0;
    }
    const int count = virtualDataSource_->itemCount();
    if (count < 0) {
        throw std::runtime_error("virtual ScrollView item count must not be negative");
    }
    return count;
}

int ScrollView::virtualEstimateForWidth(int width) const {
    if (!virtualDataSource_.has_value()) {
        throw std::logic_error("virtual ScrollView estimate requested without a data source");
    }
    if (virtualDataSource_->estimatedHeight) {
        const int estimate = virtualDataSource_->estimatedHeight(std::max(1, width));
        if (estimate <= 0) {
            throw std::runtime_error("virtual ScrollView estimated height must be positive");
        }
        return estimate;
    }
    if (virtualCount() <= 0) {
        throw std::logic_error("empty virtual ScrollView has no representative row height");
    }
    Element& child = materializeVirtualChild(0);
    return nativeChildHeight(child, std::max(1, width));
}

std::optional<ScrollView::VirtualAnchor> ScrollView::captureVirtualAnchor(
    std::optional<int> viewportHeight) const {
    if (!virtualHeightIndex_ || virtualHeightIndex_->count() <= 0) {
        return std::nullopt;
    }
    const int height = std::max(
        1,
        viewportHeight.value_or(static_cast<int>(std::max<std::int64_t>(
            1,
            virtualHeightIndex_->contentHeight() - lastNaturalSkip_))));
    const int naturalSkip = static_cast<int>(std::min<std::int64_t>(
        std::numeric_limits<int>::max(),
        std::max<std::int64_t>(0, virtualHeightIndex_->contentHeight() - height)));
    const int actualSkip = naturalSkip - std::min(viewOffset_, naturalSkip);
    const int itemIndex = virtualHeightIndex_->indexAtOffset(actualSkip);
    return VirtualAnchor{
        .index = itemIndex,
        .within = actualSkip - virtualHeightIndex_->extentBefore(itemIndex),
        .viewportHeight = height,
    };
}

void ScrollView::restoreVirtualAnchor(std::optional<VirtualAnchor> anchor) const {
    if (autoScroll_) {
        viewOffset_ = 0;
        return;
    }
    if (!anchor.has_value() || !virtualHeightIndex_ || virtualHeightIndex_->count() <= 0) {
        viewOffset_ = 0;
        lastNaturalSkip_ = 0;
        return;
    }
    const int itemIndex = std::clamp(anchor->index, 0, virtualHeightIndex_->count() - 1);
    const std::int64_t anchorSpan =
        static_cast<std::int64_t>(virtualHeightIndex_->itemHeight(itemIndex))
        + (itemIndex + 1 < virtualHeightIndex_->count() ? virtualHeightIndex_->gap() : 0);
    const std::int64_t within = std::clamp<std::int64_t>(
        anchor->within,
        0,
        anchorSpan - 1);
    const int naturalSkip = static_cast<int>(std::min<std::int64_t>(
        std::numeric_limits<int>::max(),
        std::max<std::int64_t>(
            0,
            virtualHeightIndex_->contentHeight() - std::max(1, anchor->viewportHeight))));
    const int actualSkip = static_cast<int>(std::clamp<std::int64_t>(
        virtualHeightIndex_->extentBefore(itemIndex) + within,
        0,
        naturalSkip));
    viewOffset_ = naturalSkip - actualSkip;
    lastNaturalSkip_ = naturalSkip;
}

void ScrollView::rebuildVirtualHeightIndex(std::optional<int> width) const {
    if (!virtualDataSource_.has_value()) {
        virtualHeightIndex_.reset();
        virtualHeightWidth_.reset();
        return;
    }
    if (!width.has_value()) {
        width = virtualHeightWidth_;
    }
    if (!width.has_value()) {
        virtualHeightIndex_.reset();
        return;
    }
    const int renderWidth = std::max(1, *width);
    const int count = virtualCount();
    virtualHeightWidth_ = renderWidth;
    if (count <= 0) {
        virtualHeightIndex_.reset();
        return;
    }
    virtualHeightIndex_ = std::make_unique<VirtualHeightIndex>(
        count,
        virtualEstimateForWidth(renderWidth),
        gap_);
    for (const auto& [itemIndex, row] : virtualMaterialized_) {
        if (itemIndex < count && row.element != nullptr) {
            virtualHeightIndex_->setItemHeight(itemIndex, nativeChildHeight(*row.element, renderWidth));
        }
    }
}

void ScrollView::ensureVirtualHeightIndex(Size viewport) const {
    if (!virtualDataSource_.has_value()) {
        return;
    }
    const int width = std::max(1, viewport.width);
    const int count = virtualCount();
    if (!virtualHeightIndex_ ||
        virtualHeightWidth_ != width ||
        virtualHeightIndex_->count() != count ||
        virtualHeightIndex_->gap() != gap_) {
        const std::optional<VirtualAnchor> anchor = captureVirtualAnchor(viewport.height);
        rebuildVirtualHeightIndex(width);
        restoreVirtualAnchor(anchor);
    }
}

Element& ScrollView::materializeVirtualChild(int index) const {
    if (!virtualDataSource_.has_value()) {
        throw std::logic_error("virtual ScrollView materialization requested without a data source");
    }
    const std::string key = virtualDataSource_->itemKey(index);
    const auto current = virtualMaterialized_.find(index);
    if (current != virtualMaterialized_.end() && current->second.key == key && current->second.element != nullptr) {
        return *current->second.element;
    }

    auto& owned = const_cast<ScrollView*>(this)->Element::children_;
    if (current != virtualMaterialized_.end()) {
        Element* oldElement = current->second.element;
        const auto found = std::find_if(owned.begin(), owned.end(), [oldElement](const auto& child) {
            return child.get() == oldElement;
        });
        if (found != owned.end()) {
            std::unique_ptr<Element> oldChild = std::move(*found);
            owned.erase(found);
            recycleVirtualChild(std::move(oldChild));
        }
        virtualMaterialized_.erase(current);
    }

    std::unique_ptr<Element> recycled;
    if (!virtualRecyclePool_.empty()) {
        recycled = std::move(virtualRecyclePool_.back());
        virtualRecyclePool_.pop_back();
    }
    std::unique_ptr<Element> child = virtualDataSource_->materialize(index, std::move(recycled));
    if (!child) {
        throw std::runtime_error("virtual ScrollView materializer must return a child");
    }
    Element* pointer = child.get();
    const_cast<ScrollView*>(this)->Element::addChild(std::move(child));
    virtualMaterialized_[index] = VirtualMaterializedRow{.key = key, .element = pointer};
    return *pointer;
}

void ScrollView::recycleVirtualChild(std::unique_ptr<Element> child) const {
    if (child) {
        resetVirtualChildForReuse(*child);
        virtualRecyclePool_.push_back(std::move(child));
    }
}

void ScrollView::sortVirtualChildren() const {
    auto& owned = const_cast<ScrollView*>(this)->Element::children_;
    std::sort(owned.begin(), owned.end(), [this](const auto& left, const auto& right) {
        return virtualIndexForElement(left.get()).value_or(virtualCount()) <
               virtualIndexForElement(right.get()).value_or(virtualCount());
    });
}

std::optional<int> ScrollView::virtualIndexForElement(const Element* child) const {
    for (const auto& [index, row] : virtualMaterialized_) {
        if (row.element == child) {
            return index;
        }
    }
    return std::nullopt;
}

void ScrollView::refreshVirtualMaterialization(Size viewport) const {
    ensureVirtualHeightIndex(viewport);
    virtualDiagnostics_.lastRowsMeasured = 0;
    virtualDiagnostics_.lastRowsConsidered = 0;
    const int count = virtualCount();
    if (count <= 0 || !virtualHeightIndex_) {
        auto& owned = const_cast<ScrollView*>(this)->Element::children_;
        for (auto& child : owned) {
            recycleVirtualChild(std::move(child));
        }
        owned.clear();
        virtualMaterialized_.clear();
        return;
    }

    const int height = std::max(1, viewport.height);
    const std::int64_t contentHeight = virtualHeightIndex_->contentHeight();
    int naturalSkip = static_cast<int>(std::min<std::int64_t>(
        std::numeric_limits<int>::max(),
        std::max<std::int64_t>(0, contentHeight - height)));
    syncViewOffsetForNaturalSkip(naturalSkip);
    lastNaturalSkip_ = naturalSkip;
    int actualSkipValue = naturalSkip - std::min(viewOffset_, naturalSkip);
    const int anchorIndex = virtualHeightIndex_->indexAtOffset(actualSkipValue);
    const std::int64_t anchorWithin = actualSkipValue - virtualHeightIndex_->extentBefore(anchorIndex);
    std::map<int, bool> required;

    for (int pass = 0; pass < kVirtualScrollLayoutStabilizationPasses; ++pass) {
        const std::int64_t overscan = static_cast<std::int64_t>(height) * kVirtualScrollOverscanViewports;
        const std::int64_t rangeStart = std::max<std::int64_t>(0, actualSkipValue - overscan);
        const std::int64_t rangeEnd = std::min<std::int64_t>(
            virtualHeightIndex_->contentHeight(),
            static_cast<std::int64_t>(actualSkipValue) + height + overscan);
        const int first = virtualHeightIndex_->indexAtOffset(rangeStart);
        if (pass == 0) {
            const int coarseLast = virtualHeightIndex_->indexAtOffset(rangeEnd);
            auto& owned = const_cast<ScrollView*>(this)->Element::children_;
            for (auto row = virtualMaterialized_.begin(); row != virtualMaterialized_.end();) {
                const bool keep = row->first >= first && row->first <= coarseLast;
                const bool pinned = row->second.element == virtualPinnedElement_;
                if (keep || pinned) {
                    ++row;
                    continue;
                }
                Element* pointer = row->second.element;
                const auto found = std::find_if(owned.begin(), owned.end(), [pointer](const auto& child) {
                    return child.get() == pointer;
                });
                if (found != owned.end()) {
                    std::unique_ptr<Element> child = std::move(*found);
                    owned.erase(found);
                    recycleVirtualChild(std::move(child));
                }
                row = virtualMaterialized_.erase(row);
            }
        }

        int itemIndex = first;
        std::int64_t row = virtualHeightIndex_->extentBefore(first);
        bool considered = false;
        while (itemIndex < count && (row < rangeEnd || !considered)) {
            considered = true;
            ++virtualDiagnostics_.lastRowsConsidered;
            Element& child = materializeVirtualChild(itemIndex);
            const int measured = nativeChildHeight(child, viewport.width);
            ++virtualDiagnostics_.lastRowsMeasured;
            virtualHeightIndex_->setItemHeight(itemIndex, measured);
            const int childHeight = virtualHeightIndex_->itemHeight(itemIndex);
            required[itemIndex] = true;
            row += childHeight + virtualHeightIndex_->gap();
            ++itemIndex;
        }

        const std::int64_t nextContentHeight = virtualHeightIndex_->contentHeight();
        const int nextNatural = static_cast<int>(std::min<std::int64_t>(
            std::numeric_limits<int>::max(),
            std::max<std::int64_t>(0, nextContentHeight - height)));
        if (autoScroll_) {
            viewOffset_ = 0;
        } else {
            const std::int64_t refined = virtualHeightIndex_->extentBefore(anchorIndex) + anchorWithin;
            const int refinedSkip = static_cast<int>(std::clamp<std::int64_t>(refined, 0, nextNatural));
            viewOffset_ = nextNatural - refinedSkip;
        }
        const int nextActual = nextNatural - std::min(viewOffset_, nextNatural);
        naturalSkip = nextNatural;
        if (nextActual == actualSkipValue) {
            actualSkipValue = nextActual;
            break;
        }
        actualSkipValue = nextActual;
    }

    if (virtualPinnedElement_ != nullptr) {
        const auto pinnedIndex = virtualIndexForElement(virtualPinnedElement_);
        if (pinnedIndex.has_value()) {
            required[*pinnedIndex] = true;
        }
    }
    auto& owned = const_cast<ScrollView*>(this)->Element::children_;
    for (auto row = virtualMaterialized_.begin(); row != virtualMaterialized_.end();) {
        if (required.contains(row->first)) {
            ++row;
            continue;
        }
        Element* pointer = row->second.element;
        const auto found = std::find_if(owned.begin(), owned.end(), [pointer](const auto& child) {
            return child.get() == pointer;
        });
        if (found != owned.end()) {
            std::unique_ptr<Element> child = std::move(*found);
            owned.erase(found);
            recycleVirtualChild(std::move(child));
        }
        row = virtualMaterialized_.erase(row);
    }
    const std::size_t recycleLimit = required.size();
    if (virtualRecyclePool_.size() > recycleLimit) {
        virtualRecyclePool_.erase(
            virtualRecyclePool_.begin(),
            virtualRecyclePool_.begin() + static_cast<std::ptrdiff_t>(virtualRecyclePool_.size() - recycleLimit));
    }
    sortVirtualChildren();
    lastNaturalSkip_ = naturalSkip;
}

const std::vector<int>& ScrollView::cachedChildHeights() const {
    ensureHeightCache();
    return *cachedChildHeights_;
}

int ScrollView::contentHeight(int width) const {
    if (virtualDataSource_.has_value()) {
        ensureVirtualHeightIndex(Size{std::max(1, width), std::max(1, frame().height)});
        if (!virtualHeightIndex_) {
            return 0;
        }
        return static_cast<int>(std::min<std::int64_t>(
            std::numeric_limits<int>::max(),
            virtualHeightIndex_->contentHeight()));
    }
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
    if (virtualDataSource_.has_value()) {
        ensureVirtualHeightIndex(viewport);
        if (virtualCount() == 0 || !virtualHeightIndex_) {
            lastNaturalSkip_ = 0;
            return ScrollViewContentWindow{
                .viewport = viewport,
                .actualSkip = 0,
                .naturalSkip = 0,
                .gap = std::max(0, gap_),
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
    viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    if (virtualDataSource_.has_value()) {
        ensureVirtualHeightIndex(viewport);
        const int total = virtualCount();
        if (total == 0 || !virtualHeightIndex_) {
            const bool changed = viewOffset_ != 0 || scrollOffset_ != 0;
            viewOffset_ = 0;
            scrollOffset_ = 0;
            return changed;
        }
        const int targetIndex = std::clamp(index, 0, total - 1);
        const int naturalSkip = maxViewOffset(viewport);
        const int targetSkip = static_cast<int>(std::min<std::int64_t>(
            naturalSkip,
            virtualHeightIndex_->extentBefore(targetIndex)));
        const int previous = viewOffset_;
        scrollOffset_ = 0;
        viewOffset_ = naturalSkip - targetSkip;
        autoScroll_ = viewOffset_ == 0;
        return viewOffset_ != previous;
    }
    refreshDynamicChildren(viewport.width);
    const int total = static_cast<int>(children_.size() + children().size());
    if (total == 0) {
        const int previous = scrollOffset_;
        scrollOffset_ = 0;
        return scrollOffset_ != previous;
    }

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
    if (virtualDataSource_.has_value()) {
        const Size padded = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
        ensureVirtualHeightIndex(padded);
        if (!virtualHeightIndex_) {
            return false;
        }
        const int naturalSkip = maxViewOffset(padded);
        const int actual = naturalSkip - std::min(viewOffset_, naturalSkip);
        return scrollTo(virtualHeightIndex_->indexAtOffset(actual) + delta, viewport);
    }
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
    if (virtualDataSource_.has_value()) {
        viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
        const int maximum = maxViewOffset(viewport);
        return maximum - std::min(viewOffset_, maximum) > 0;
    }
    (void)viewport;
    return scrollOffset_ > 0;
}

bool ScrollView::canScrollDown(Size viewport) const {
    if (virtualDataSource_.has_value()) {
        return viewOffset_ > 0;
    }
    viewport = paddedViewportSize(viewport, constrainedPadding(viewport, style()));
    refreshDynamicChildren(viewport.width);
    const int total = static_cast<int>(children_.size() + children().size());
    if (total == 0) {
        return false;
    }
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
    if (virtualDataSource_.has_value()) {
        refreshVirtualMaterialization(viewport);
        ensureVirtualHeightIndex(viewport);
        std::vector<ScrollViewChildView> views;
        virtualDiagnostics_.lastRowsRendered = 0;
        if (!virtualHeightIndex_) {
            return views;
        }
        const int naturalSkip = maxViewOffset(viewport);
        const int skip = naturalSkip - std::min(viewOffset_, naturalSkip);
        const int viewportTop = padding.top;
        const int viewportBottom = padding.top + viewport.height;
        views.reserve(virtualMaterialized_.size());
        for (const auto& [itemIndex, row] : virtualMaterialized_) {
            if (row.element == nullptr) {
                continue;
            }
            const int childHeight = virtualHeightIndex_->itemHeight(itemIndex);
            const std::int64_t relativeRow = virtualHeightIndex_->extentBefore(itemIndex) - skip;
            const int localRow = static_cast<int>(std::clamp<std::int64_t>(
                relativeRow + padding.top,
                std::numeric_limits<int>::min(),
                std::numeric_limits<int>::max()));
            const int visibleTop = std::max(localRow, viewportTop);
            const int visibleBottom = std::min(localRow + childHeight, viewportBottom);
            const bool visible = visibleBottom > visibleTop;
            if (visible) {
                ++virtualDiagnostics_.lastRowsRendered;
            }
            views.push_back(ScrollViewChildView{
                .element = row.element,
                .frame = Rect{localRow, padding.left, std::max(1, viewport.width), childHeight},
                .visible = visible,
                .index = itemIndex,
                .clipped = visible && (visibleTop > localRow || visibleBottom < localRow + childHeight),
            });
        }
        return views;
    }
    refreshDynamicChildren(viewport.width);
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
    if (virtualDataSource_.has_value()) {
        ensureVirtualHeightIndex(viewport);
        const auto itemIndex = virtualIndexForElement(child);
        if (!itemIndex.has_value() || !virtualHeightIndex_) {
            return false;
        }
        const int targetTop = static_cast<int>(std::min<std::int64_t>(
            std::numeric_limits<int>::max(),
            virtualHeightIndex_->extentBefore(*itemIndex)));
        const int targetBottom = targetTop + virtualHeightIndex_->itemHeight(*itemIndex);
        const int naturalSkip = maxViewOffset(viewport);
        const int currentActualSkip = naturalSkip - std::min(viewOffset_, naturalSkip);
        int nextActualSkip = currentActualSkip;
        if (targetTop < currentActualSkip) {
            nextActualSkip = targetTop;
        } else if (targetBottom > currentActualSkip + viewport.height) {
            nextActualSkip = targetBottom - viewport.height;
        }
        nextActualSkip = std::clamp(nextActualSkip, 0, naturalSkip);
        const int previous = viewOffset_;
        viewOffset_ = naturalSkip - nextActualSkip;
        autoScroll_ = viewOffset_ == 0;
        return viewOffset_ != previous;
    }
    refreshDynamicChildren(viewport.width);
    ensureHeightCache(viewport.width);

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
    if (virtualDataSource_.has_value()) {
        refreshVirtualMaterialization(Size{renderWidth, viewport.height});
        const int naturalSkip = maxViewOffset(Size{renderWidth, viewport.height});
        const int clampedViewOffset = std::min(viewOffset_, naturalSkip);
        const int skip = naturalSkip - clampedViewOffset;
        const RenderedRow blank = blankRow(viewport.width, style);
        RenderedContent visible(
            static_cast<std::size_t>(viewport.height),
            blank);
        if (skip > 0 && !visible.empty() && !visible.front().empty()) {
            applyScrollIndicator(
                visible.front(),
                kScrollIndicatorAbove,
                inferredScrollIndicatorForeground(visible.front()));
        }
        if (clampedViewOffset > 0 && !visible.empty() && !visible.back().empty()) {
            applyScrollIndicator(
                visible.back(),
                kScrollIndicatorBelow,
                inferredScrollIndicatorForeground(visible.back()));
        }
        RenderedContent padded;
        padded.reserve(static_cast<std::size_t>(height));
        const RenderedRow fullBlank = blankRow(width, style);
        while (static_cast<int>(padded.size()) < padding.top) {
            padded.push_back(fullBlank);
        }
        for (const RenderedRow& row : visible) {
            RenderedRow paddedRow = fullBlank;
            for (std::size_t column = 0;
                 column < row.size() && padding.left + static_cast<int>(column) < width;
                 ++column) {
                paddedRow[static_cast<std::size_t>(padding.left) + column] = row[column];
            }
            padded.push_back(std::move(paddedRow));
        }
        while (static_cast<int>(padded.size()) < height) {
            padded.push_back(fullBlank);
        }
        return padded;
    }
    refreshDynamicChildren(renderWidth);
    const int naturalSkip = maxViewOffset(Size{renderWidth, viewport.height});
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
    if (virtualDataSource_.has_value()) {
        ensureVirtualHeightIndex(Size{width, std::max(1, frame().height)});
        std::vector<int> heights;
        heights.reserve(virtualMaterialized_.size());
        if (virtualHeightIndex_) {
            for (const auto& [itemIndex, row] : virtualMaterialized_) {
                (void)row;
                heights.push_back(virtualHeightIndex_->itemHeight(itemIndex));
            }
        }
        cachedChildHeightsWidth_ = width;
        cachedChildHeights_ = std::move(heights);
        return;
    }
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
    refreshDynamicChildren(viewportWidth(viewport));
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
