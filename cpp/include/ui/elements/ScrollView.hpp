#pragma once

#include <functional>
#include <optional>
#include <string_view>
#include <utility>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

inline constexpr int kDefaultScrollViewGap = 0;
inline constexpr int kDefaultWheelScrollLines = 4;
inline constexpr int kWheelScrollViewportFraction = 3;
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

class ScrollView : public Element {
public:
    explicit ScrollView(std::string name, int gap = kDefaultScrollViewGap);

    using Element::addChild;

    void setGap(int gap);
    void addChild(RenderedContent child);
    void clearChildren();
    void invalidateHeightCache() const;
    void setDynamicChildrenRenderer(std::function<std::vector<RenderedContent>(int)> renderer);
    void invalidateDynamicChildren() const;

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
    [[nodiscard]] int maxChildrenInViewport(Size viewport) const;
    void ensureHeightCache() const;
    void ensureHeightCache(int width) const;
    void syncViewOffsetForNaturalSkip(int naturalSkip) const;
    [[nodiscard]] int actualSkip(Size viewport) const;
    [[nodiscard]] RenderedRow blankRow(int width, const Style& style) const;
    void refreshDynamicChildren(int width) const;

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
    std::optional<Style> descendantFocusStyle_;
};

}  // namespace ui
