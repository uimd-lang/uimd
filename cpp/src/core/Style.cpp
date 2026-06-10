#include "ui/core/Style.hpp"

namespace ui {

void Style::merge(const Style& override) {
    if (override.color.has_value()) {
        color = override.color;
    }
    if (override.background.has_value()) {
        background = override.background;
    }
    if (override.backgroundTexture.has_value()) {
        backgroundTexture = override.backgroundTexture;
    }
    if (override.backgroundTextureColor.has_value()) {
        backgroundTextureColor = override.backgroundTextureColor;
    }
    if (override.borderColor.has_value()) {
        borderColor = override.borderColor;
    }
    if (override.borderWidthHorizontal.has_value()) {
        borderWidthHorizontal = override.borderWidthHorizontal;
    }
    if (override.borderWidthVertical.has_value()) {
        borderWidthVertical = override.borderWidthVertical;
    }
    if (override.padding.has_value()) {
        padding = override.padding;
    }
    if (override.paddingTop.has_value()) {
        paddingTop = override.paddingTop;
    }
    if (override.paddingRight.has_value()) {
        paddingRight = override.paddingRight;
    }
    if (override.paddingBottom.has_value()) {
        paddingBottom = override.paddingBottom;
    }
    if (override.paddingLeft.has_value()) {
        paddingLeft = override.paddingLeft;
    }
    if (override.margin.has_value()) {
        margin = override.margin;
    }
    if (override.marginTop.has_value()) {
        marginTop = override.marginTop;
    }
    if (override.marginRight.has_value()) {
        marginRight = override.marginRight;
    }
    if (override.marginBottom.has_value()) {
        marginBottom = override.marginBottom;
    }
    if (override.marginLeft.has_value()) {
        marginLeft = override.marginLeft;
    }
    if (override.gap.has_value()) {
        gap = override.gap;
    }
    if (override.textAlign.has_value()) {
        textAlign = override.textAlign;
    }
    if (override.userSelect.has_value()) {
        userSelect = override.userSelect;
    }
    if (override.scrollX.has_value()) {
        scrollX = override.scrollX;
    }
    if (override.scrollY.has_value()) {
        scrollY = override.scrollY;
    }
    if (override.textColorGradient.has_value()) {
        textColorGradient = override.textColorGradient;
    }
    if (override.textBackgroundGradient.has_value()) {
        textBackgroundGradient = override.textBackgroundGradient;
    }
    if (override.scopeDimBackground.has_value()) {
        scopeDimBackground = override.scopeDimBackground;
    }
}

}  // namespace ui
