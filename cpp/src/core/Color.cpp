#include "ui/core/Color.hpp"

#include <algorithm>
#include <charconv>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <string_view>

namespace ui {

namespace {

constexpr int kShortHexDigits = 3;
constexpr int kRgbHexDigits = 6;
constexpr int kRgbaHexDigits = 8;
constexpr int kHexByteBase = 16;

[[nodiscard]] std::string lowerCopy(std::string value) {
    std::ranges::transform(value, value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

[[nodiscard]] std::uint8_t parseHexByte(std::string_view text) {
    unsigned int value = 0;
    const auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), value, kHexByteBase);
    if (ec != std::errc{} || ptr != text.data() + text.size() || value > 255) {
        throw std::invalid_argument("invalid color hex byte");
    }
    return static_cast<std::uint8_t>(value);
}

[[nodiscard]] std::uint8_t parseShortHex(char ch) {
    char text[] = {ch, ch};
    return parseHexByte(std::string_view{text, sizeof(text)});
}

[[nodiscard]] bool isHexText(std::string_view text) {
    return std::ranges::all_of(text, [](unsigned char ch) {
        return std::isxdigit(ch) != 0;
    });
}

}  // namespace

Color::Color(std::string value) : value_(lowerCopy(std::move(value))) {
    if (value_.empty()) {
        kind_ = Kind::Unset;
        return;
    }
    if (value_ == "transparent") {
        kind_ = Kind::Transparent;
        return;
    }
    if (value_.front() != '#') {
        kind_ = Kind::Named;
        return;
    }

    const std::string_view hex(value_.data() + 1, value_.size() - 1);
    if (!isHexText(hex)) {
        throw std::invalid_argument("invalid color hex text");
    }

    kind_ = Kind::Rgb;
    if (hex.size() == kShortHexDigits) {
        rgba_ = Rgba{parseShortHex(hex[0]), parseShortHex(hex[1]), parseShortHex(hex[2]), 255};
        return;
    }
    if (hex.size() == kRgbHexDigits || hex.size() == kRgbaHexDigits) {
        rgba_ = Rgba{
            parseHexByte(hex.substr(0, 2)),
            parseHexByte(hex.substr(2, 2)),
            parseHexByte(hex.substr(4, 2)),
            hex.size() == kRgbaHexDigits ? parseHexByte(hex.substr(6, 2)) : static_cast<std::uint8_t>(255),
        };
        return;
    }

    throw std::invalid_argument("unsupported color hex length");
}

Color Color::transparent() {
    return Color("transparent");
}

Color Color::rgb(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha) {
    Color color;
    color.kind_ = Kind::Rgb;
    color.rgba_ = Rgba{red, green, blue, alpha};
    return color;
}

Color Color::named(std::string value) {
    Color color;
    color.kind_ = Kind::Named;
    color.value_ = lowerCopy(std::move(value));
    return color;
}

Color Color::blendOver(const Color& background) const {
    if (kind_ != Kind::Rgb || !rgba_.has_value()) {
        return *this;
    }
    const Rgba& foreground = *rgba_;
    if (foreground.alpha >= 255) {
        return *this;
    }
    if (foreground.alpha == 0) {
        return background;
    }
    if (background.kind_ != Kind::Rgb || !background.rgba().has_value()) {
        return *this;
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

std::string Color::toString() const {
    if (kind_ == Kind::Unset) {
        return {};
    }
    if (kind_ == Kind::Transparent || kind_ == Kind::Named) {
        return value_;
    }
    if (!rgba_) {
        return {};
    }

    constexpr char kHex[] = "0123456789abcdef";
    std::string result = "#000000";
    const std::uint8_t values[] = {rgba_->red, rgba_->green, rgba_->blue};
    for (std::size_t i = 0; i < std::size(values); ++i) {
        result[1 + (i * 2)] = kHex[values[i] >> 4];
        result[2 + (i * 2)] = kHex[values[i] & 0x0f];
    }
    return result;
}

}  // namespace ui
