#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

namespace ui {

struct Rgba {
    std::uint8_t red = 0;
    std::uint8_t green = 0;
    std::uint8_t blue = 0;
    std::uint8_t alpha = 255;

    friend bool operator==(const Rgba&, const Rgba&) = default;
};

class Color {
public:
    enum class Kind {
        Unset,
        Transparent,
        Rgb,
        Named,
    };

    Color() = default;
    explicit Color(std::string value);

    [[nodiscard]] static Color transparent();
    [[nodiscard]] static Color rgb(std::uint8_t red, std::uint8_t green, std::uint8_t blue,
                                   std::uint8_t alpha = 255);
    [[nodiscard]] static Color named(std::string value);

    [[nodiscard]] Kind kind() const {
        return kind_;
    }

    [[nodiscard]] const std::string& value() const {
        return value_;
    }

    [[nodiscard]] const std::optional<Rgba>& rgba() const {
        return rgba_;
    }

    [[nodiscard]] bool isSet() const {
        return kind_ != Kind::Unset;
    }

    [[nodiscard]] bool isTransparent() const {
        return kind_ == Kind::Transparent;
    }

    [[nodiscard]] Color blendOver(const Color& background) const;

    [[nodiscard]] std::string toString() const;

    friend bool operator==(const Color&, const Color&) = default;

private:
    Kind kind_ = Kind::Unset;
    std::string value_;
    std::optional<Rgba> rgba_;
};

}  // namespace ui
