#include "ui/core/Element.hpp"

#include <atomic>
#include <cmath>
#include <stdexcept>

namespace ui {

namespace {

thread_local std::optional<Color> effectiveStyleParentBackground;
std::atomic<std::uint64_t> nextElementIdentity{1};

[[nodiscard]] bool hasPartialAlpha(const std::optional<Color>& color) {
    return color.has_value() && color->rgba().has_value() && color->rgba()->alpha < 255;
}

[[nodiscard]] Style mergedStateStyle(Style base, const Style& state) {
    std::optional<Color> baseBackground = base.background;
    const std::optional<Color> stateBackground = state.background;
    if (hasPartialAlpha(baseBackground) &&
        effectiveStyleParentBackground.has_value() &&
        effectiveStyleParentBackground->rgba().has_value()) {
        base.background = baseBackground->blendOver(*effectiveStyleParentBackground);
        baseBackground = base.background;
    }
    base.merge(state);
    if (hasPartialAlpha(stateBackground) &&
        baseBackground.has_value() &&
        baseBackground->rgba().has_value() &&
        stateBackground->rgba()->alpha == 0) {
        base.background = baseBackground;
    } else if (hasPartialAlpha(stateBackground) &&
        baseBackground.has_value() &&
        baseBackground->rgba().has_value()) {
        base.background = stateBackground->blendOver(*baseBackground);
    }
    return base;
}

}  // namespace

Element::Element(std::string name)
    : name_(std::move(name)),
      identity_(nextElementIdentity.fetch_add(1, std::memory_order_relaxed)) {}

std::optional<Color> Element::renderingParentBackground() {
    return effectiveStyleParentBackground;
}

ScopedElementParentBackground::ScopedElementParentBackground(std::optional<Color> background)
    : previous_(effectiveStyleParentBackground) {
    effectiveStyleParentBackground = std::move(background);
}

ScopedElementParentBackground::~ScopedElementParentBackground() {
    effectiveStyleParentBackground = std::move(previous_);
}

Style Element::effectiveStyle(bool focused, bool editMode) const {
    Style result = style_;
    if (!enabled_ && disabledStyle_.has_value()) {
        return mergedStateStyle(result, *disabledStyle_);
    }
    if (editMode && editStyle_.has_value()) {
        return mergedStateStyle(result, *editStyle_);
    }
    if (focused && focusStyle_.has_value()) {
        return mergedStateStyle(result, *focusStyle_);
    }
    return result;
}

Element& Element::addChild(std::unique_ptr<Element> child) {
    if (!child) {
        throw std::invalid_argument("child element must not be null");
    }
    child->setParent(this);
    Element& ref = *child;
    children_.push_back(std::move(child));
    return ref;
}

void Element::clearChildElements() {
    children_.clear();
}

}  // namespace ui
