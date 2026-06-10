#include "ui/elements/InfoLabel.hpp"

#include <algorithm>

namespace ui {

namespace {

constexpr int kInfoLabelGradientIntervalMs = 70;
constexpr int kInfoLabelGradientStep = 1;
constexpr int kInfoLabelGradientSegmentSize = 1;

[[nodiscard]] int safeWidth(Size size, const std::string& text) {
    return std::max(1, size.width > 0 ? size.width : static_cast<int>(text.size()));
}

[[nodiscard]] int safeHeight(Size size) {
    return std::max(1, size.height > 0 ? size.height : 1);
}

[[nodiscard]] TextGradient infoLabelGradient() {
    return TextGradient{
        .intervalMs = kInfoLabelGradientIntervalMs,
        .step = kInfoLabelGradientStep,
        .segmentSize = kInfoLabelGradientSegmentSize,
        .colors = {
            Color("#ffffff66"),
            Color("#ffffff88"),
            Color("#ffffffaa"),
            Color("#ffffffcc"),
            Color("#ffffffff"),
            Color("#ffffffcc"),
            Color("#ffffffaa"),
            Color("#ffffff88"),
            Color("#ffffff66"),
        },
    };
}

}  // namespace

RenderedContent InfoLabel::render(Size size, ElementRenderState state) const {
    Style style = effectiveStyle(state.focused, state.editMode);
    if (!style.textColorGradient.has_value()) {
        style.textColorGradient = infoLabelGradient();
    }
    return renderPlainText(text(), safeWidth(size, text()), safeHeight(size), style);
}

}  // namespace ui
