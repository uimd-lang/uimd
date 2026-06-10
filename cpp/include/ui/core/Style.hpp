#pragma once

#include <optional>
#include <string>
#include <vector>

#include "ui/core/Color.hpp"

namespace ui {

struct TextGradient {
    int intervalMs = 70;
    int step = 1;
    int segmentSize = 1;
    std::vector<Color> colors;

    friend bool operator==(const TextGradient&, const TextGradient&) = default;
};

struct Style {
    inline static const Color defaultScopeDimBackground = Color{"#00000066"};

    std::optional<Color> color;
    std::optional<Color> background;
    std::optional<std::string> backgroundTexture;
    std::optional<Color> backgroundTextureColor;
    std::optional<Color> borderColor;
    std::optional<int> borderWidthHorizontal;
    std::optional<int> borderWidthVertical;
    std::optional<int> padding;
    std::optional<int> paddingTop;
    std::optional<int> paddingRight;
    std::optional<int> paddingBottom;
    std::optional<int> paddingLeft;
    std::optional<int> margin;
    std::optional<int> marginTop;
    std::optional<int> marginRight;
    std::optional<int> marginBottom;
    std::optional<int> marginLeft;
    std::optional<int> gap;
    std::optional<std::string> textAlign;
    std::optional<std::string> userSelect;
    std::optional<bool> scrollX;
    std::optional<bool> scrollY;
    std::optional<TextGradient> textColorGradient;
    std::optional<TextGradient> textBackgroundGradient;
    std::optional<Color> scopeDimBackground = defaultScopeDimBackground;

    void merge(const Style& override);

    [[nodiscard]] Style merged(const Style& override) const {
        Style result = *this;
        result.merge(override);
        return result;
    }

    friend bool operator==(const Style&, const Style&) = default;
};

}  // namespace ui
