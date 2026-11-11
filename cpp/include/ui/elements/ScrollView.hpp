#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

inline constexpr int kDefaultScrollViewGap = 0;
inline constexpr int kDefaultWheelScrollLines = 4;
inline constexpr int kWheelScrollViewportFraction = 3;
inline constexpr int kVirtualScrollOverscanViewports = 1;
inline constexpr int kVirtualScrollLayoutStabilizationPasses = 2;
inline constexpr std::string_view kScrollIndicatorAbove = "^";
inline constexpr std::string_view kScrollIndicatorBelow = "v";

struct ScrollViewChildView {
    Element* element = nullptr;
    Rect frame;
    bool visible = false;
    int index = 0;
    bool clipped = false;
};

struct ScrollViewContentWindow {
    Size viewport;
    int actualSkip = 0;
    int naturalSkip = 0;
    int gap = 0;
};

struct ScrollViewPosition {
    int scrollOffset = 0;
    int viewOffset = 0;
    bool autoScroll = false;
};

struct VirtualScrollDataSource {
    std::function<int()> itemCount;
    std::function<std::string(int)> itemKey;
    std::function<std::unique_ptr<Element>(int, std::unique_ptr<Element>)> materialize;
    std::function<int(int)> estimatedHeight;
};

struct VirtualScrollDiagnostics {
    int logicalItems = 0;
    int materializedRows = 0;
    int recycledRows = 0;
    int lastRowsMeasured = 0;
    int lastRowsConsidered = 0;
    int lastRowsRendered = 0;
};

class ScrollView : public Element {
public:
    explicit ScrollView(std::string name, int gap = kDefaultScrollViewGap);
    ~ScrollView() override;

    template <typename T, typename... Args>
    T& addChild(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *child;
        addChild(std::move(child));
        return ref;
    }

    void setGap(int gap);
    Element& addChild(std::unique_ptr<Element> child);
    void addChild(RenderedContent child);
    void clearChildren();
    void invalidateHeightCache() const;
    void setDynamicChildrenRenderer(std::function<std::vector<RenderedContent>(int)> renderer);
    void invalidateDynamicChildren() const;
    void setVirtualDataSource(VirtualScrollDataSource source);
    void clearVirtualDataSource();
    void virtualItemsInserted(int index, int count = 1);
    void virtualItemsRemoved(int index, int count = 1);
    void virtualItemsReplaced(int index, int count = 1);
    void invalidateVirtualHeights();
    void invalidateVirtualHeights(int index, int count);
    [[nodiscard]] bool hasVirtualDataSource() const;
    [[nodiscard]] VirtualScrollDiagnostics virtualDiagnostics() const;
    void setVirtualPinnedElement(const Element* element) const;

    void setAutoScroll(bool autoScroll);
    void setDescendantFocusStyle(Style style) {
        descendantFocusStyle_ = std::move(style);
    }

    [[nodiscard]] const std::optional<Style>& descendantFocusStyle() const {
        return descendantFocusStyle_;
    }

    [[nodiscard]] bool autoScroll() const {
        return autoScroll_;
    }

    [[nodiscard]] int viewOffset() const {
        return viewOffset_;
    }

    [[nodiscard]] int scrollOffset() const {
        return scrollOffset_;
    }

    [[nodiscard]] int hViewOffset() const {
        return hViewOffset_;
    }

    [[nodiscard]] ScrollViewPosition scrollPosition() const;
    void restoreScrollPosition(ScrollViewPosition position);

    [[nodiscard]] const std::vector<int>& cachedChildHeights() const;
    [[nodiscard]] int contentHeight(int width) const;
    [[nodiscard]] int contentHeight() const;
    [[nodiscard]] int maxViewOffset(Size viewport) const;
    [[nodiscard]] std::optional<ScrollViewContentWindow> contentWindow(Size viewport) const;
    [[nodiscard]] std::vector<ScrollViewChildView> childViews(Size viewport) const;

    [[nodiscard]] bool scrollTo(int index, Size viewport);
    [[nodiscard]] bool scrollBy(int delta, Size viewport);
    [[nodiscard]] bool scrollLines(int delta, Size viewport);
    [[nodiscard]] bool scrollToTop(Size viewport);
    [[nodiscard]] bool scrollPageUp(Size viewport);
    [[nodiscard]] bool scrollPageDown(Size viewport);
    [[nodiscard]] bool scrollHorizontal(int delta);
    [[nodiscard]] int wheelScrollLines(Size viewport) const;
    [[nodiscard]] bool canScrollUp(Size viewport) const;
    [[nodiscard]] bool canScrollDown(Size viewport) const;

    [[nodiscard]] bool handleWheel(int wheelDelta, Size viewport);
    [[nodiscard]] bool handleKey(std::string_view key, Size viewport);
    [[nodiscard]] bool ensureChildVisible(const Element* child, Size viewport);
    [[nodiscard]] bool scrollToBottom(Size viewport);
    [[nodiscard]] int consumeTerminalScrollDelta() const;

    [[nodiscard]] virtual RenderedContent render(Size size, ElementRenderState state = {}) const;

protected:
    [[nodiscard]] RenderedContent renderWithStyle(Size size, const Style& style) const;
    [[nodiscard]] RenderedContent renderWithStyle(Size size, const Style& style, const Style& childInheritStyle) const;

private:
    class VirtualHeightIndex;

    struct VirtualMaterializedRow {
        std::string key;
        Element* element = nullptr;
    };

    struct VirtualAnchor {
        int index = 0;
        std::int64_t within = 0;
        int viewportHeight = 1;
    };

    [[nodiscard]] int maxChildrenInViewport(Size viewport) const;
    void ensureHeightCache() const;
    void ensureHeightCache(int width) const;
    void syncViewOffsetForNaturalSkip(int naturalSkip) const;
    [[nodiscard]] int actualSkip(Size viewport) const;
    [[nodiscard]] RenderedRow blankRow(int width, const Style& style) const;
    void refreshDynamicChildren(int width) const;
    [[nodiscard]] int virtualCount() const;
    [[nodiscard]] int virtualEstimateForWidth(int width) const;
    [[nodiscard]] std::optional<VirtualAnchor> captureVirtualAnchor(
        std::optional<int> viewportHeight = std::nullopt) const;
    void restoreVirtualAnchor(std::optional<VirtualAnchor> anchor) const;
    void rebuildVirtualHeightIndex(std::optional<int> width = std::nullopt) const;
    void ensureVirtualHeightIndex(Size viewport) const;
    [[nodiscard]] Element& materializeVirtualChild(int index) const;
    void recycleVirtualChild(std::unique_ptr<Element> child) const;
    void refreshVirtualMaterialization(Size viewport) const;
    void sortVirtualChildren() const;
    [[nodiscard]] std::optional<int> virtualIndexForElement(const Element* child) const;

    int gap_ = kDefaultScrollViewGap;
    bool autoScroll_ = false;
    mutable int scrollOffset_ = 0;
    mutable int viewOffset_ = 0;
    mutable int hViewOffset_ = 0;
    mutable int lastNaturalSkip_ = 0;
    mutable int pendingTerminalScrollDelta_ = 0;
    mutable std::vector<RenderedContent> children_;
    std::function<std::vector<RenderedContent>(int)> dynamicChildrenRenderer_;
    mutable std::optional<int> dynamicChildrenWidth_;
    mutable std::optional<int> cachedChildHeightsWidth_;
    mutable std::optional<std::vector<int>> cachedChildHeights_;
    std::optional<VirtualScrollDataSource> virtualDataSource_;
    mutable std::unique_ptr<VirtualHeightIndex> virtualHeightIndex_;
    mutable std::optional<int> virtualHeightWidth_;
    mutable std::map<int, VirtualMaterializedRow> virtualMaterialized_;
    mutable std::vector<std::unique_ptr<Element>> virtualRecyclePool_;
    mutable VirtualScrollDiagnostics virtualDiagnostics_;
    mutable const Element* virtualPinnedElement_ = nullptr;
    std::optional<Style> descendantFocusStyle_;
};

}  // namespace ui
