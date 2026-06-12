#include "ui/generated/GeneratedWindowRuntime.hpp"

#include "ui/elements/Button.hpp"
#include "ui/elements/CheckBox.hpp"
#include "ui/elements/ComboBox.hpp"
#include "ui/elements/FrameBufferView.hpp"
#include "ui/elements/Image.hpp"
#include "ui/elements/InfoLabel.hpp"
#include "ui/elements/Label.hpp"
#include "ui/elements/ListBox.hpp"
#include "ui/elements/MessageTable.hpp"
#include "ui/elements/NumberInput.hpp"
#include "ui/elements/ReusableElement.hpp"
#include "ui/elements/ScrollView.hpp"
#include "ui/elements/TextArea.hpp"
#include "ui/elements/TextInput.hpp"
#include "ui/terminal/Clipboard.hpp"
#include "ui/terminal/TerminalBackend.hpp"

#include <algorithm>
#include <charconv>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <cerrno>
#include <termios.h>
#include <condition_variable>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <unistd.h>
#include <mutex>
#include <stdexcept>
#include <system_error>
#include <variant>
#include <vector>

#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

namespace ui {

namespace {

constexpr int kFallbackTerminalWidth = 100;
constexpr int kFallbackTerminalHeight = 32;
constexpr int kContentTopRow = 0;
constexpr int kContentLeftCol = 0;
constexpr int kMinimumRenderableSize = 1;
constexpr int kComboBoxClosedRows = 1;
constexpr int kComboBoxDropdownRows = 6;
constexpr int kTextInputWheelScrollRows = 1;
constexpr int kMaxCoalescedMouseWheelDelta = 12;
constexpr int kNoBorderWidth = 0;
constexpr double kModalBackgroundDimFactor = 0.5;
constexpr auto kCopyNotificationDuration = std::chrono::seconds(3);
constexpr auto kInputIdleSleep = std::chrono::milliseconds(10);
constexpr auto kEmbeddedViewportIdleSleep = kInputIdleSleep;
constexpr auto kAnimatedRenderInterval = std::chrono::milliseconds(70);
constexpr int kMcpRenderWaitTicks = 25;
constexpr auto kMcpRenderWaitTimeout = kInputIdleSleep * kMcpRenderWaitTicks;
constexpr int kDefaultMcpPort = 8765;
constexpr int kDefaultGuiMcpActionDelayMs = 500;
constexpr int kDefaultGuiMcpTypeDelayMs = 50;
constexpr int kDefaultHeadlessMcpActionDelayMs = 0;
constexpr int kDefaultHeadlessMcpTypeDelayMs = 0;
constexpr int kDialogButtonCloseDelayMs = 180;
constexpr int kJsonRpcErrorCode = -32000;
constexpr std::string_view kImageHalfBlockGlyph = "▀";
constexpr std::string_view kAnsiClearScreen = "\x1b[H\x1b[2J";
constexpr std::string_view kImageHalfBlockDimBackground = "#000000";
constexpr std::string_view kDefaultMcpHost = "127.0.0.1";
constexpr std::string_view kMcpTransportTcp = "tcp";
constexpr std::string_view kMcpTransportStdio = "stdio";
constexpr std::string_view kMcpTransportHttp = "http";
constexpr std::string_view kMcpHttpEndpoint = "/mcp";
constexpr std::string_view kHttpApplicationJson = "application/json";
constexpr std::string_view kHttpTextEventStream = "text/event-stream";
constexpr std::string_view kJsonRpcVersion = "2.0";
constexpr std::string_view kMcpProtocolVersion = "2024-11-05";

struct ScrollSelection {
    ScrollView* element = nullptr;
    Point anchor{};
    Point current{};
    bool active = false;
    bool changed = false;
};

struct MouseClickCandidate {
    Element* element = nullptr;
    bool moved = false;
};

struct ScrollRegionHint {
    Rect rect{};
    int delta = 0;
};

struct JsonValue {
    using Object = std::map<std::string, JsonValue>;
    using Array = std::vector<JsonValue>;

    std::variant<std::nullptr_t, bool, double, std::string, Array, Object> value = nullptr;

    JsonValue() = default;
    JsonValue(std::nullptr_t) : value(nullptr) {}
    JsonValue(bool item) : value(item) {}
    JsonValue(int item) : value(static_cast<double>(item)) {}
    JsonValue(double item) : value(item) {}
    JsonValue(std::string item) : value(std::move(item)) {}
    JsonValue(const char* item) : value(std::string(item)) {}
    JsonValue(Array item) : value(std::move(item)) {}
    JsonValue(Object item) : value(std::move(item)) {}

    [[nodiscard]] bool isObject() const { return std::holds_alternative<Object>(value); }
    [[nodiscard]] bool isArray() const { return std::holds_alternative<Array>(value); }
    [[nodiscard]] bool isString() const { return std::holds_alternative<std::string>(value); }
    [[nodiscard]] bool isNumber() const { return std::holds_alternative<double>(value); }
    [[nodiscard]] bool isBool() const { return std::holds_alternative<bool>(value); }
    [[nodiscard]] bool isNull() const { return std::holds_alternative<std::nullptr_t>(value); }
    [[nodiscard]] const Object& object() const { return std::get<Object>(value); }
    [[nodiscard]] Object& object() { return std::get<Object>(value); }
    [[nodiscard]] const Array& array() const { return std::get<Array>(value); }
    [[nodiscard]] const std::string& string() const { return std::get<std::string>(value); }
    [[nodiscard]] double number() const { return std::get<double>(value); }
    [[nodiscard]] bool boolean() const { return std::get<bool>(value); }
};

class JsonParser {
public:
    explicit JsonParser(std::string_view input) : input_(input) {}

    [[nodiscard]] JsonValue parse() {
        skipWhitespace();
        JsonValue result = parseValue();
        skipWhitespace();
        if (pos_ != input_.size()) {
            throw std::runtime_error("unexpected trailing JSON content");
        }
        return result;
    }

private:
    [[nodiscard]] JsonValue parseValue() {
        skipWhitespace();
        if (pos_ >= input_.size()) {
            throw std::runtime_error("unexpected end of JSON");
        }
        const char ch = input_[pos_];
        if (ch == '{') return parseObject();
        if (ch == '[') return parseArray();
        if (ch == '"') return JsonValue(parseString());
        if (ch == 't') return parseLiteral("true", JsonValue(true));
        if (ch == 'f') return parseLiteral("false", JsonValue(false));
        if (ch == 'n') return parseLiteral("null", JsonValue(nullptr));
        if (ch == '-' || std::isdigit(static_cast<unsigned char>(ch))) return parseNumber();
        throw std::runtime_error("invalid JSON value");
    }

    [[nodiscard]] JsonValue parseObject() {
        expect('{');
        JsonValue::Object object;
        skipWhitespace();
        if (consume('}')) {
            return JsonValue(std::move(object));
        }
        while (true) {
            skipWhitespace();
            const std::string key = parseString();
            skipWhitespace();
            expect(':');
            object[key] = parseValue();
            skipWhitespace();
            if (consume('}')) {
                break;
            }
            expect(',');
        }
        return JsonValue(std::move(object));
    }

    [[nodiscard]] JsonValue parseArray() {
        expect('[');
        JsonValue::Array array;
        skipWhitespace();
        if (consume(']')) {
            return JsonValue(std::move(array));
        }
        while (true) {
            array.push_back(parseValue());
            skipWhitespace();
            if (consume(']')) {
                break;
            }
            expect(',');
        }
        return JsonValue(std::move(array));
    }

    [[nodiscard]] std::string parseString() {
        expect('"');
        std::string result;
        while (pos_ < input_.size()) {
            const char ch = input_[pos_++];
            if (ch == '"') {
                return result;
            }
            if (ch != '\\') {
                result.push_back(ch);
                continue;
            }
            if (pos_ >= input_.size()) {
                throw std::runtime_error("unterminated JSON escape");
            }
            const char escaped = input_[pos_++];
            switch (escaped) {
                case '"': result.push_back('"'); break;
                case '\\': result.push_back('\\'); break;
                case '/': result.push_back('/'); break;
                case 'b': result.push_back('\b'); break;
                case 'f': result.push_back('\f'); break;
                case 'n': result.push_back('\n'); break;
                case 'r': result.push_back('\r'); break;
                case 't': result.push_back('\t'); break;
                default: throw std::runtime_error("unsupported JSON escape");
            }
        }
        throw std::runtime_error("unterminated JSON string");
    }

    [[nodiscard]] JsonValue parseNumber() {
        const std::size_t start = pos_;
        if (input_[pos_] == '-') {
            ++pos_;
        }
        while (pos_ < input_.size() && std::isdigit(static_cast<unsigned char>(input_[pos_]))) {
            ++pos_;
        }
        if (pos_ < input_.size() && input_[pos_] == '.') {
            ++pos_;
            while (pos_ < input_.size() && std::isdigit(static_cast<unsigned char>(input_[pos_]))) {
                ++pos_;
            }
        }
        return JsonValue(std::stod(std::string(input_.substr(start, pos_ - start))));
    }

    [[nodiscard]] JsonValue parseLiteral(std::string_view literal, JsonValue value) {
        if (input_.substr(pos_, literal.size()) != literal) {
            throw std::runtime_error("invalid JSON literal");
        }
        pos_ += literal.size();
        return value;
    }

    void skipWhitespace() {
        while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) {
            ++pos_;
        }
    }

    void expect(char ch) {
        if (!consume(ch)) {
            throw std::runtime_error("unexpected JSON character");
        }
    }

    [[nodiscard]] bool consume(char ch) {
        skipWhitespace();
        if (pos_ < input_.size() && input_[pos_] == ch) {
            ++pos_;
            return true;
        }
        return false;
    }

    std::string_view input_;
    std::size_t pos_ = 0;
};

[[nodiscard]] std::string jsonEscape(std::string_view text) {
    std::string result;
    for (char ch : text) {
        switch (ch) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                result.push_back(ch);
                break;
        }
    }
    return result;
}

[[nodiscard]] std::string jsonSerialize(const JsonValue& value);

[[nodiscard]] std::string jsonSerializeObject(const JsonValue::Object& object) {
    std::string result = "{";
    bool first = true;
    for (const auto& [key, item] : object) {
        if (!first) {
            result += ",";
        }
        first = false;
        result += "\"" + jsonEscape(key) + "\":" + jsonSerialize(item);
    }
    result += "}";
    return result;
}

[[nodiscard]] std::string jsonSerializeArray(const JsonValue::Array& array) {
    std::string result = "[";
    for (std::size_t index = 0; index < array.size(); ++index) {
        if (index > 0) {
            result += ",";
        }
        result += jsonSerialize(array[index]);
    }
    result += "]";
    return result;
}

[[nodiscard]] std::string jsonSerialize(const JsonValue& value) {
    if (std::holds_alternative<std::nullptr_t>(value.value)) return "null";
    if (value.isBool()) return value.boolean() ? "true" : "false";
    if (value.isNumber()) {
        const double number = value.number();
        const auto integer = static_cast<long long>(number);
        if (static_cast<double>(integer) == number) {
            return std::to_string(integer);
        }
        std::ostringstream out;
        out << number;
        return out.str();
    }
    if (value.isString()) return "\"" + jsonEscape(value.string()) + "\"";
    if (value.isArray()) return jsonSerializeArray(value.array());
    return jsonSerializeObject(value.object());
}

[[nodiscard]] const JsonValue* jsonField(const JsonValue::Object& object, std::string_view key) {
    const auto it = object.find(std::string(key));
    return it == object.end() ? nullptr : &it->second;
}

[[nodiscard]] std::string jsonStringField(const JsonValue::Object& object, std::string_view key, std::string fallback = {}) {
    const JsonValue* value = jsonField(object, key);
    return value != nullptr && value->isString() ? value->string() : std::move(fallback);
}

[[nodiscard]] std::string jsonTextField(const JsonValue::Object& object, std::string_view key, std::string fallback = {}) {
    const JsonValue* value = jsonField(object, key);
    if (value == nullptr) {
        return fallback;
    }
    if (value->isString()) {
        return value->string();
    }
    if (value->isNumber() || value->isBool()) {
        return jsonSerialize(*value);
    }
    return fallback;
}

[[nodiscard]] std::string compactClassName(std::string_view title) {
    std::string result;
    for (char ch : title) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            result.push_back(ch);
        }
    }
    return result.empty() ? std::string("GeneratedWindowBase") : result;
}

[[nodiscard]] int jsonIntField(const JsonValue::Object& object, std::string_view key, int fallback = 0) {
    const JsonValue* value = jsonField(object, key);
    return value != nullptr && value->isNumber() ? static_cast<int>(value->number()) : fallback;
}

[[nodiscard]] bool jsonBoolField(const JsonValue::Object& object, std::string_view key, bool fallback = false) {
    const JsonValue* value = jsonField(object, key);
    return value != nullptr && value->isBool() ? value->boolean() : fallback;
}

[[nodiscard]] std::optional<std::int64_t> jsonInt64Field(const JsonValue::Object& object, std::string_view key) {
    const JsonValue* value = jsonField(object, key);
    if (value == nullptr || !value->isNumber()) {
        return std::nullopt;
    }
    return static_cast<std::int64_t>(value->number());
}

struct McpRuntimeConfig {
    bool enabled = false;
    bool gui = true;
    std::string transport = std::string(kMcpTransportTcp);
    std::string host = std::string(kDefaultMcpHost);
    int port = kDefaultMcpPort;
    int actionDelayMs = kDefaultGuiMcpActionDelayMs;
    int typeDelayMs = kDefaultGuiMcpTypeDelayMs;
    bool waitRender = false;
    bool controlledRender = false;
    std::optional<Rect> viewport;
};

struct McpRuntimeState {
    std::mutex mutex;
    std::recursive_mutex uiMutex;
    std::condition_variable renderCondition;
    std::uint64_t renderGeneration = 0;
    int focusedIndex = -1;
    bool editMode = false;
    Element* activeScrollView = nullptr;
    Element* activeScrollViewEditElement = nullptr;
    GeneratedWindowBase* overlayFocusedWindow = nullptr;
    std::string overlayFocusedClassName;
    int overlayFocusedIndex = -1;
    bool overlayEditMode = false;
    bool closeRequested = false;
    bool fullRedrawRequested = false;
    std::optional<Rect> viewport;
};

struct FocusIdentity {
    const Element* element = nullptr;
    std::uint64_t identity = 0;
};

enum class GeneratedWindowMode {
    Normal,
    ExpandWidth,
    ExpandHeight,
    Fullscreen,
};

struct RuntimeCell {
    int sourceRow = 0;
    int sourceCol = 0;
    int sourceWidth = 1;
    int sourceHeight = 1;
    std::optional<int> logicalWidth;
    std::optional<int> logicalHeight;
};

struct RuntimeNode {
    std::string orientation;
    std::vector<std::shared_ptr<RuntimeNode>> children;
    RuntimeCell cell;
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
    std::optional<int> logicalWidth;
    std::optional<int> logicalHeight;
};

struct ResolvedRuntimeCell {
    RuntimeCell cell;
    Rect rect;
};

[[nodiscard]] int fitContentHeightFor(const Element* element, std::string_view type, int fallback,
                                      std::optional<int> width = std::nullopt);
[[nodiscard]] bool recomputeFitContentHeightsPass2(const GeneratedWindowBase& window,
                                                   std::vector<RuntimeCell>& cells,
                                                   const std::vector<ResolvedRuntimeCell>& resolvedCells);
[[nodiscard]] Element* findElement(GeneratedWindowBase& window, std::string_view name);
[[nodiscard]] const Element* findElement(const GeneratedWindowBase& window, std::string_view name);
[[nodiscard]] std::vector<Element*> focusableElements(GeneratedWindowBase& window, Element* activeScrollView = nullptr);
[[nodiscard]] int indexOfElement(const std::vector<Element*>& elements, const Element* target);
[[nodiscard]] std::vector<RuntimeCell> collectRuntimeCells(const GeneratedWindowBase& window);
void resolveRuntimeCellsWithFitPass(const GeneratedWindowBase& window,
                                    std::vector<RuntimeCell>& runtimeCells,
                                    Rect content,
                                    int horizontalSeparator,
                                    int verticalSeparator,
                                    GeneratedWindowMode mode,
                                    std::vector<ResolvedRuntimeCell>& resolvedCells);
[[nodiscard]] const ResolvedRuntimeCell* resolvedCellFor(const std::vector<ResolvedRuntimeCell>& cells,
                                                        const GeneratedLayoutEntry& entry);
[[nodiscard]] Size renderSizeFor(const GeneratedLayoutEntry& entry, Rect cellRect, const Element* element);
[[nodiscard]] GeneratedWindowMode windowMode(const GeneratedWindowBase& window);
[[nodiscard]] int borderWidthHorizontal(const Style& style);
[[nodiscard]] int borderWidthVertical(const Style& style);
[[nodiscard]] int renderColFor(const std::vector<GeneratedLayoutEntry>& layout,
                               const GeneratedLayoutEntry& entry,
                               Rect cellRect,
                               Size size);
[[nodiscard]] int renderRowFor(const std::vector<GeneratedLayoutEntry>& layout,
                               const GeneratedLayoutEntry& entry,
                               Rect cellRect,
                               Size size);
void syncReusableChildFrames(ReusableElement& reusable, Rect frame);
void syncWindowElementFramesTo(GeneratedWindowBase& window, Rect frame);
[[nodiscard]] bool windowElementFramesSyncedTo(const GeneratedWindowBase& window, Rect frame);

[[nodiscard]] TerminalCell styledTextCell(char ch, const Style& style) {
    return TerminalCell{
        .text = std::string(1, ch),
        .foreground = style.color,
        .background = style.background,
    };
}

[[nodiscard]] std::vector<std::string> utf8Glyphs(std::string_view text) {
    std::vector<std::string> glyphs;
    for (std::size_t index = 0; index < text.size();) {
        const unsigned char lead = static_cast<unsigned char>(text[index]);
        std::size_t length = 1;
        if ((lead & 0b11100000U) == 0b11000000U) {
            length = 2;
        } else if ((lead & 0b11110000U) == 0b11100000U) {
            length = 3;
        } else if ((lead & 0b11111000U) == 0b11110000U) {
            length = 4;
        }
        if (index + length > text.size()) {
            length = 1;
        }
        glyphs.emplace_back(text.substr(index, length));
        index += length;
    }
    return glyphs;
}

[[nodiscard]] TerminalCell styledFillCell(const Style& style,
                                          const std::vector<std::string>& textureGlyphs,
                                          int rowOffset,
                                          int colOffset,
                                          int textureWidth) {
    if (textureGlyphs.empty()) {
        return styledTextCell(' ', style);
    }
    const int safeTextureWidth = std::max(kMinimumRenderableSize, textureWidth);
    const auto textureIndex = static_cast<std::size_t>(
        (rowOffset * safeTextureWidth + colOffset) % static_cast<int>(textureGlyphs.size()));
    return TerminalCell{
        .text = textureGlyphs[textureIndex],
        .foreground = style.backgroundTextureColor.has_value() ? style.backgroundTextureColor : style.color,
        .background = style.background,
    };
}

[[nodiscard]] bool transparentColor(const std::optional<Color>& color) {
    return !color.has_value() || color->kind() == Color::Kind::Unset || color->isTransparent();
}

[[nodiscard]] std::optional<Color> dimColor(const std::optional<Color>& color) {
    if (transparentColor(color) || !color->rgba().has_value()) {
        return color;
    }
    const Rgba& rgba = *color->rgba();
    return Color::rgb(
        static_cast<std::uint8_t>(static_cast<double>(rgba.red) * kModalBackgroundDimFactor),
        static_cast<std::uint8_t>(static_cast<double>(rgba.green) * kModalBackgroundDimFactor),
        static_cast<std::uint8_t>(static_cast<double>(rgba.blue) * kModalBackgroundDimFactor),
        rgba.alpha);
}

void dimBufferForModalOverlay(TerminalBuffer& buffer) {
    for (int row = 0; row < buffer.height(); ++row) {
        for (int col = 0; col < buffer.width(); ++col) {
            TerminalCell cell = buffer.cell(row, col);
            cell.foreground = dimColor(cell.foreground);
            cell.background = dimColor(cell.background);
            buffer.setCell(row, col, std::move(cell));
        }
    }
}

[[nodiscard]] std::optional<Color> blendBackgroundOverExisting(const std::optional<Color>& color,
                                                               const std::optional<Color>& existing) {
    if (transparentColor(color)) {
        return existing;
    }
    if (color->rgba().has_value() && color->rgba()->alpha < 255 && existing.has_value()) {
        return color->blendOver(*existing);
    }
    return color;
}

[[nodiscard]] std::optional<Color> blendForegroundOverBackground(const std::optional<Color>& color,
                                                                 const std::optional<Color>& inherited,
                                                                 const std::optional<Color>& background) {
    if (transparentColor(color)) {
        return inherited;
    }
    if (color->rgba().has_value() && color->rgba()->alpha < 255 && background.has_value()) {
        return color->blendOver(*background);
    }
    return color;
}

[[nodiscard]] bool sameRenderedColor(const Color& lhs, const Color& rhs) {
    if (lhs.rgba().has_value() && rhs.rgba().has_value()) {
        return lhs.rgba() == rhs.rgba();
    }
    return lhs.toString() == rhs.toString();
}

[[nodiscard]] bool sameRenderedColor(const std::optional<Color>& lhs, const std::optional<Color>& rhs) {
    if (!lhs.has_value() || !rhs.has_value()) {
        return !lhs.has_value() && !rhs.has_value();
    }
    return sameRenderedColor(*lhs, *rhs);
}

[[nodiscard]] Color blendOverStyleAlpha(const Color& color, const Color& background) {
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
    const double alpha = std::nearbyint((static_cast<double>(foreground.alpha) / 255.0) * 100.0) / 100.0;
    const auto channel = [alpha](std::uint8_t fg, std::uint8_t bg) {
        return static_cast<std::uint8_t>(std::nearbyint(alpha * fg + (1.0 - alpha) * bg));
    };
    return Color::rgb(
        channel(foreground.red, base.red),
        channel(foreground.green, base.green),
        channel(foreground.blue, base.blue));
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

void applyReusableFocusBackground(RenderedContent& content,
                                  const Color& focusBackground,
                                  const std::optional<Color>& baseBackground) {
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
                (baseBackground.has_value() && sameRenderedColor(cell.background, baseBackground));
            const bool matchesFocusedBase = focusedBaseBackground.has_value() &&
                sameRenderedColor(cell.background, focusedBaseBackground);
            if (!matchesBase && !matchesFocusedBase) {
                continue;
            }
            if (matchesFocusedBase) {
                continue;
            }
            cell.background = focusBackground.rgba().has_value() && cell.background.has_value()
                ? blendOverExactAlpha(focusBackground, *cell.background)
                : focusBackground;
        }
    }
}

void applyReusableFocusBackgroundToBuffer(TerminalBuffer& buffer,
                                          Rect rect,
                                          const Color& focusBackground,
                                          const std::optional<Color>& baseBackground) {
    std::optional<Color> focusedBaseBackground;
    if (baseBackground.has_value()) {
        focusedBaseBackground = focusBackground.rgba().has_value()
            ? blendOverExactAlpha(focusBackground, *baseBackground)
            : focusBackground;
    }
    const int top = std::max(0, rect.row);
    const int bottom = std::min(buffer.height(), rect.row + rect.height);
    const int left = std::max(0, rect.col);
    const int right = std::min(buffer.width(), rect.col + rect.width);
    for (int row = top; row < bottom; ++row) {
        for (int col = left; col < right; ++col) {
            TerminalCell cell = buffer.cell(row, col);
            const bool matchesBase =
                !cell.background.has_value() ||
                (baseBackground.has_value() && sameRenderedColor(cell.background, baseBackground)) ||
                (focusedBaseBackground.has_value() && sameRenderedColor(cell.background, focusedBaseBackground));
            if (!matchesBase) {
                continue;
            }
            cell.background = focusBackground.rgba().has_value() && cell.background.has_value()
                ? blendOverExactAlpha(focusBackground, *cell.background)
                : focusBackground;
            buffer.setCell(row, col, std::move(cell));
        }
    }
}

[[nodiscard]] bool opaqueBackground(const std::optional<Color>& background) {
    return background.has_value() && background->rgba().has_value() && background->rgba()->alpha == 255;
}

[[nodiscard]] bool partialBackground(const std::optional<Color>& background) {
    return background.has_value() && background->rgba().has_value() && background->rgba()->alpha < 255;
}

void appendUniqueBackground(std::vector<Color>& backgrounds, const std::optional<Color>& background) {
    if (!opaqueBackground(background)) {
        return;
    }
    if (std::find(backgrounds.begin(), backgrounds.end(), *background) == backgrounds.end()) {
        backgrounds.push_back(*background);
    }
}

void collectDescendantBaseStyleBackgrounds(const GeneratedWindowBase& window, std::vector<Color>& backgrounds);

void collectDescendantBaseStyleBackgrounds(const Element& element, std::vector<Color>& backgrounds) {
    appendUniqueBackground(backgrounds, element.style().background);
    if (element.focusStyle().has_value()) {
        appendUniqueBackground(backgrounds, element.focusStyle()->background);
    }
    if (element.editStyle().has_value()) {
        appendUniqueBackground(backgrounds, element.editStyle()->background);
    }
    if (const auto* reusable = dynamic_cast<const ReusableElement*>(&element);
        reusable != nullptr && reusable->child() != nullptr) {
        collectDescendantBaseStyleBackgrounds(*reusable->child(), backgrounds);
    }
    if (const auto* scrollView = dynamic_cast<const ScrollView*>(&element)) {
        for (const auto& child : scrollView->children()) {
            if (child != nullptr) {
                collectDescendantBaseStyleBackgrounds(*child, backgrounds);
            }
        }
    }
}

void collectDescendantBaseStyleBackgrounds(const GeneratedWindowBase& window, std::vector<Color>& backgrounds) {
    for (const auto& element : window.elements()) {
        if (element != nullptr) {
            collectDescendantBaseStyleBackgrounds(*element, backgrounds);
        }
    }
}

void applyReusableFocusBackgroundToDescendantBackgrounds(RenderedContent& content,
                                                        const Color& focusBackground,
                                                        const std::vector<Color>& backgrounds) {
    if (backgrounds.empty()) {
        return;
    }
    for (RenderedRow& row : content) {
        for (TerminalCell& cell : row) {
            if (!cell.background.has_value()) {
                continue;
            }
            for (const Color& background : backgrounds) {
                const Color focusedBackground = focusBackground.rgba().has_value()
                    ? blendOverExactAlpha(focusBackground, background)
                    : focusBackground;
                if (sameRenderedColor(*cell.background, background) ||
                    sameRenderedColor(*cell.background, focusedBackground)) {
                    cell.background = focusedBackground;
                    break;
                }
            }
        }
    }
}

void applySelectedBackgroundToFocusedReusableChild(RenderedContent& content,
                                                  const Color& selectedBackground,
                                                  const std::optional<Color>& focusBackground,
                                                  const std::vector<Color>& descendantBackgrounds) {
    for (RenderedRow& row : content) {
        for (TerminalCell& cell : row) {
            if (!cell.background.has_value()) {
                continue;
            }

            bool descendantBackground = false;
            for (const Color& background : descendantBackgrounds) {
                if (sameRenderedColor(*cell.background, background)) {
                    descendantBackground = true;
                    break;
                }
                if (focusBackground.has_value()) {
                    const Color focusedBackground = focusBackground->rgba().has_value()
                        ? blendOverExactAlpha(*focusBackground, background)
                        : *focusBackground;
                    if (sameRenderedColor(*cell.background, focusedBackground)) {
                        descendantBackground = true;
                        break;
                    }
                }
            }
            if (descendantBackground) {
                continue;
            }

            cell.background = selectedBackground.rgba().has_value()
                ? blendOverStyleAlpha(selectedBackground, *cell.background)
                : selectedBackground;
        }
    }
}

[[nodiscard]] std::optional<Color> reusableFocusBaseBackground(const ReusableElement& reusable,
                                                               const GeneratedLayoutEntry& entry) {
    return reusable.style().background.has_value()
        ? reusable.style().background
        : entry.cellStyle.background;
}

[[nodiscard]] Element* findDirectElement(GeneratedWindowBase& window, std::string_view name) {
    for (const auto& element : window.elements()) {
        if (element->name() == name) {
            return element.get();
        }
    }
    return nullptr;
}

[[nodiscard]] ScrollView* generatedScrollViewForReusable(ReusableElement& reusable) {
    return reusable.child() == nullptr ? nullptr : reusable.child()->generatedScrollView();
}

[[nodiscard]] Element* generatedScrollViewProxyFor(GeneratedWindowBase& window, const ScrollView* target) {
    if (target == nullptr) {
        return nullptr;
    }
    for (const auto& element : window.elements()) {
        if (element == nullptr) {
            continue;
        }
        if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            if (generatedScrollViewForReusable(*reusable) == target) {
                return reusable;
            }
            if (Element* proxy = generatedScrollViewProxyFor(*reusable->child(), target)) {
                return proxy;
            }
        }
        auto* scrollView = dynamic_cast<ScrollView*>(element.get());
        if (scrollView == nullptr) {
            continue;
        }
        for (const auto& child : scrollView->children()) {
            auto* reusable = child == nullptr ? nullptr : dynamic_cast<ReusableElement*>(child.get());
            if (reusable == nullptr || reusable->child() == nullptr) {
                continue;
            }
            if (generatedScrollViewForReusable(*reusable) == target) {
                return reusable;
            }
            if (Element* proxy = generatedScrollViewProxyFor(*reusable->child(), target)) {
                return proxy;
            }
        }
    }
    return nullptr;
}

[[nodiscard]] std::optional<Color> generatedScrollViewScopeDimBackground(GeneratedWindowBase& window,
                                                                         const ScrollView* target) {
    Element* proxy = generatedScrollViewProxyFor(window, target);
    if (proxy != nullptr && proxy->style().scopeDimBackground.has_value()) {
        return proxy->style().scopeDimBackground;
    }
    return std::nullopt;
}

[[nodiscard]] bool reusableFocusStyleAppliesToChild(const ReusableElement& reusable,
                                                    bool directFocus,
                                                    bool descendantOnlyFocus) {
    if (!reusable.focusStyle().has_value()) {
        return false;
    }
    const std::optional<Color>& background = reusable.focusStyle()->background;
    if (!partialBackground(background)) {
        return true;
    }
    if (reusable.child() != nullptr && reusable.child()->generatedScrollView() != nullptr) {
        return false;
    }
    return directFocus || !descendantOnlyFocus;
}

void syncReusableChildFrames(ReusableElement& reusable, Rect frame);

[[nodiscard]] Element* findElementInReusableChild(ReusableElement& reusable, std::string_view childElementId) {
    if (reusable.child() == nullptr || childElementId.empty()) {
        return nullptr;
    }
    const Rect frame = reusable.frame();
    if (frame.width > 0 && frame.height > 0) {
        syncReusableChildFrames(reusable, frame);
    }
    return findElement(*reusable.child(), childElementId);
}

[[nodiscard]] Element* findElementInScrollViewChild(ScrollView& scrollView, std::string_view childElementId) {
    const std::size_t bracket = childElementId.find('[');
    const std::size_t close = childElementId.find(']', bracket == std::string_view::npos ? 0 : bracket);
    if (bracket == std::string_view::npos || close == std::string_view::npos || close + 1 >= childElementId.size() ||
        childElementId[close + 1] != '.') {
        return nullptr;
    }

    int childIndex = 0;
    const std::string indexText{childElementId.substr(bracket + 1, close - bracket - 1)};
    const auto [ptr, ec] = std::from_chars(indexText.data(), indexText.data() + indexText.size(), childIndex);
    if (ec != std::errc{} || ptr != indexText.data() + indexText.size() || childIndex < 0) {
        return nullptr;
    }

    const auto& children = scrollView.children();
    if (childIndex >= static_cast<int>(children.size()) || children[static_cast<std::size_t>(childIndex)] == nullptr) {
        return nullptr;
    }

    auto* reusable = dynamic_cast<ReusableElement*>(children[static_cast<std::size_t>(childIndex)].get());
    if (reusable == nullptr || reusable->child() == nullptr) {
        return nullptr;
    }

    const Rect frame = scrollView.frame();
    for (const ScrollViewChildView& childView : scrollView.childViews(Size{frame.width, frame.height})) {
        if (childView.element == reusable) {
            syncReusableChildFrames(*reusable, Rect{
                frame.row + childView.frame.row,
                frame.col + childView.frame.col,
                childView.frame.width,
                childView.frame.height,
            });
            break;
        }
    }

    return findElement(*reusable->child(), childElementId.substr(close + 2));
}

[[nodiscard]] Element* findElement(GeneratedWindowBase& window, std::string_view name) {
    if (Element* direct = findDirectElement(window, name)) {
        return direct;
    }

    const std::size_t bracket = name.find('[');
    const std::size_t dot = name.find('.');
    if (dot != std::string_view::npos && (bracket == std::string_view::npos || dot < bracket)) {
        const std::string_view hostName = name.substr(0, dot);
        auto* reusable = dynamic_cast<ReusableElement*>(findDirectElement(window, hostName));
        if (reusable == nullptr) {
            return nullptr;
        }
        return findElementInReusableChild(*reusable, name.substr(dot + 1));
    }

    if (bracket == std::string_view::npos) {
        for (const auto& element : window.elements()) {
            auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            if (reusable == nullptr || reusable->child() == nullptr) {
                continue;
            }
            if (Element* nested = findElement(*reusable->child(), name)) {
                return nested;
            }
        }
        return nullptr;
    }
    const std::string_view scrollViewName = name.substr(0, bracket);
    Element* host = findDirectElement(window, scrollViewName);
    if (auto* scrollView = dynamic_cast<ScrollView*>(host)) {
        return findElementInScrollViewChild(*scrollView, name);
    }
    if (auto* reusable = dynamic_cast<ReusableElement*>(host)) {
        const Rect frame = reusable->frame();
        if (frame.width > 0 && frame.height > 0) {
            syncReusableChildFrames(*reusable, frame);
        }
        if (ScrollView* scrollView = generatedScrollViewForReusable(*reusable)) {
            return findElementInScrollViewChild(*scrollView, name);
        }
    }
    return nullptr;
}

[[nodiscard]] const Element* findElement(const GeneratedWindowBase& window, std::string_view name) {
    return findElement(const_cast<GeneratedWindowBase&>(window), name);
}

[[nodiscard]] bool windowOwnsElement(const GeneratedWindowBase& window, const Element* target) {
    if (target == nullptr) {
        return false;
    }
    for (const auto& element : window.elements()) {
        if (element.get() == target) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] GeneratedWindowBase* ownerWindowForElement(GeneratedWindowBase& window, const Element* target) {
    if (windowOwnsElement(window, target)) {
        return &window;
    }
    for (const auto& element : window.elements()) {
        if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            if (GeneratedWindowBase* owner = ownerWindowForElement(*reusable->child(), target)) {
                return owner;
            }
        }
        auto* scrollView = dynamic_cast<ScrollView*>(element.get());
        if (scrollView == nullptr) {
            continue;
        }
        for (const auto& child : scrollView->children()) {
            auto* reusable = child == nullptr ? nullptr : dynamic_cast<ReusableElement*>(child.get());
            if (reusable != nullptr && reusable->child() != nullptr) {
                if (GeneratedWindowBase* owner = ownerWindowForElement(*reusable->child(), target)) {
                    return owner;
                }
            }
        }
    }
    return nullptr;
}

[[nodiscard]] ScrollView* scrollViewContainingElement(GeneratedWindowBase& window, const Element* target) {
    if (target == nullptr) {
        return nullptr;
    }
    for (const auto& element : window.elements()) {
        if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            if (ScrollView* scrollView = scrollViewContainingElement(*reusable->child(), target)) {
                return scrollView;
            }
        }
        auto* scrollView = dynamic_cast<ScrollView*>(element.get());
        if (scrollView == nullptr) {
            continue;
        }
        for (const auto& child : scrollView->children()) {
            if (child == nullptr) {
                continue;
            }
            if (child.get() == target) {
                return scrollView;
            }
            if (auto* reusable = dynamic_cast<ReusableElement*>(child.get());
                reusable != nullptr && reusable->child() != nullptr) {
                if (ownerWindowForElement(*reusable->child(), target) != nullptr) {
                    return scrollView;
                }
            }
            if (auto* nestedScrollView = dynamic_cast<ScrollView*>(child.get())) {
                for (const auto& nestedChild : nestedScrollView->children()) {
                    if (nestedChild.get() == target) {
                        return nestedScrollView;
                    }
                }
            }
        }
    }
    return nullptr;
}

[[nodiscard]] std::optional<std::string> elementIdForElement(GeneratedWindowBase& window, const Element* target) {
    if (target == nullptr) {
        return std::nullopt;
    }
    for (const auto& element : window.elements()) {
        if (element.get() == target) {
            return element->name();
        }
        if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            if (ScrollView* scrollView = generatedScrollViewForReusable(*reusable)) {
                if (scrollView == target) {
                    return reusable->name();
                }
                const auto& children = scrollView->children();
                for (std::size_t index = 0; index < children.size(); ++index) {
                    const auto& child = children[index];
                    if (child == nullptr) {
                        continue;
                    }
                    if (child.get() == target) {
                        return reusable->name() + "[" + std::to_string(index) + "]." + child->name();
                    }
                    if (auto* childReusable = dynamic_cast<ReusableElement*>(child.get());
                        childReusable != nullptr && childReusable->child() != nullptr) {
                        if (std::optional<std::string> childId = elementIdForElement(*childReusable->child(), target);
                            childId.has_value()) {
                            return reusable->name() + "[" + std::to_string(index) + "]." + *childId;
                        }
                    }
                }
            }
            if (std::optional<std::string> childId = elementIdForElement(*reusable->child(), target);
                childId.has_value()) {
                return reusable->name() + "." + *childId;
            }
        }
        auto* scrollView = dynamic_cast<ScrollView*>(element.get());
        if (scrollView == nullptr) {
            continue;
        }
        const auto& children = scrollView->children();
        for (std::size_t index = 0; index < children.size(); ++index) {
            const auto& child = children[index];
            if (child == nullptr) {
                continue;
            }
            if (child.get() == target) {
                return scrollView->name() + "[" + std::to_string(index) + "]." + child->name();
            }
            if (auto* reusable = dynamic_cast<ReusableElement*>(child.get());
                reusable != nullptr && reusable->child() != nullptr) {
                if (std::optional<std::string> childId = elementIdForElement(*reusable->child(), target);
                    childId.has_value()) {
                    return scrollView->name() + "[" + std::to_string(index) + "]." + *childId;
                }
            }
        }
    }
    return std::nullopt;
}

void syncReusableChildFrames(ReusableElement& reusable, Rect frame);

void offsetWindowElementFrames(GeneratedWindowBase& window, Rect origin) {
    for (const auto& element : window.elements()) {
        if (element == nullptr) {
            continue;
        }
        Rect frame = element->frame();
        frame.row += origin.row;
        frame.col += origin.col;
        element->setFrame(frame);
        if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            offsetWindowElementFrames(*reusable->child(), origin);
            continue;
        }
        auto* scrollView = dynamic_cast<ScrollView*>(element.get());
        if (scrollView == nullptr) {
            continue;
        }
        const Rect scrollFrame = scrollView->frame();
        for (const ScrollViewChildView& childView : scrollView->childViews(Size{scrollFrame.width, scrollFrame.height})) {
            if (childView.element == nullptr) {
                continue;
            }
            if (!childView.visible) {
                childView.element->setFrame(Rect{0, 0, 0, 0});
                continue;
            }
            childView.element->setFrame(Rect{
                scrollFrame.row + childView.frame.row,
                scrollFrame.col + childView.frame.col,
                childView.frame.width,
                childView.frame.height,
            });
            if (auto* reusable = dynamic_cast<ReusableElement*>(childView.element);
                reusable != nullptr && reusable->child() != nullptr) {
                syncReusableChildFrames(*reusable, childView.element->frame());
            }
        }
    }
}

void syncWindowElementFramesTo(GeneratedWindowBase& window, Rect frame) {
    const int width = std::max(kMinimumRenderableSize, frame.width);
    const int height = std::max(kMinimumRenderableSize, frame.height);
    const Style& style = window.generatedWindowStyle();
    std::vector<RuntimeCell> runtimeCells = collectRuntimeCells(window);
    std::vector<ResolvedRuntimeCell> resolvedCells;
    resolveRuntimeCellsWithFitPass(
        window,
        runtimeCells,
        Rect{0, 0, width, height},
        borderWidthHorizontal(style),
        borderWidthVertical(style),
        windowMode(window),
        resolvedCells);

    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        if (entry.name.empty()) {
            continue;
        }
        Element* element = findElement(window, entry.name);
        if (element == nullptr) {
            continue;
        }
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, entry);
        if (cell == nullptr) {
            continue;
        }
        const Size size = renderSizeFor(entry, cell->rect, element);
        const int row = frame.row + renderRowFor(window.generatedLayout(), entry, cell->rect, size);
        const int col = frame.col + renderColFor(window.generatedLayout(), entry, cell->rect, size);
        element->setFrame(Rect{row, col, size.width, size.height});

        if (auto* reusable = dynamic_cast<ReusableElement*>(element);
            reusable != nullptr && reusable->child() != nullptr) {
            syncReusableChildFrames(*reusable, element->frame());
            continue;
        }
        auto* scrollView = dynamic_cast<ScrollView*>(element);
        if (scrollView == nullptr) {
            continue;
        }
        const Rect scrollFrame = scrollView->frame();
        for (const ScrollViewChildView& childView : scrollView->childViews(Size{scrollFrame.width, scrollFrame.height})) {
            if (childView.element == nullptr) {
                continue;
            }
            if (!childView.visible) {
                childView.element->setFrame(Rect{0, 0, 0, 0});
                continue;
            }
            childView.element->setFrame(Rect{
                scrollFrame.row + childView.frame.row,
                scrollFrame.col + childView.frame.col,
                childView.frame.width,
                childView.frame.height,
            });
            if (auto* reusable = dynamic_cast<ReusableElement*>(childView.element);
                reusable != nullptr && reusable->child() != nullptr) {
                syncReusableChildFrames(*reusable, childView.element->frame());
            }
        }
    }
}

void syncReusableChildFrames(ReusableElement& reusable, Rect frame) {
    if (reusable.child() == nullptr) {
        return;
    }
    if (windowElementFramesSyncedTo(*reusable.child(), frame)) {
        return;
    }
    syncWindowElementFramesTo(*reusable.child(), frame);
}

[[nodiscard]] int focusIndexForElement(GeneratedWindowBase& window, const Element* target, Element* activeScrollView = nullptr) {
    const std::vector<Element*> focusable = focusableElements(window, activeScrollView);
    return indexOfElement(focusable, target);
}

[[nodiscard]] bool isFocusableType(const Element& element) {
    return dynamic_cast<const Button*>(&element) != nullptr ||
           dynamic_cast<const CheckBox*>(&element) != nullptr ||
           dynamic_cast<const TextInput*>(&element) != nullptr ||
           dynamic_cast<const NumberInput*>(&element) != nullptr ||
           dynamic_cast<const ComboBox*>(&element) != nullptr ||
           dynamic_cast<const ListBox*>(&element) != nullptr ||
           dynamic_cast<const ScrollView*>(&element) != nullptr;
}

[[nodiscard]] bool isFocusable(const Element& element) {
    return element.enabled() && element.focusable() && isFocusableType(element);
}

[[nodiscard]] std::vector<Element*> focusableElements(GeneratedWindowBase& window, Element* activeScrollView) {
    std::vector<Element*> elements;
    for (const auto& element : window.elements()) {
        if (element == nullptr) {
            continue;
        }
        if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            syncReusableChildFrames(*reusable, reusable->frame());
            if (ScrollView* generatedScrollView = generatedScrollViewForReusable(*reusable)) {
                if (reusable->enabled() && reusable->focusable()) {
                    elements.push_back(reusable);
                }
                if (activeScrollView == generatedScrollView) {
                    std::vector<Element*> childFocusable = focusableElements(*reusable->child(), activeScrollView);
                    for (Element* childElement : childFocusable) {
                        if (childElement != generatedScrollView) {
                            elements.push_back(childElement);
                        }
                    }
                }
                continue;
            }
            std::vector<Element*> childFocusable = focusableElements(*reusable->child(), activeScrollView);
            if (childFocusable.empty() &&
                reusable->enabled() &&
                reusable->focusable() &&
                reusable->child()->generatedFocusable() &&
                dynamic_cast<ViewHost*>(reusable) == nullptr) {
                elements.push_back(reusable);
            } else {
                elements.insert(elements.end(), childFocusable.begin(), childFocusable.end());
            }
            continue;
        }
        if (auto* scrollView = dynamic_cast<ScrollView*>(element.get())) {
            if (isFocusable(*element)) {
                elements.push_back(element.get());
            }
            if (activeScrollView == scrollView) {
                const Rect frame = scrollView->frame();
                for (const ScrollViewChildView& childView : scrollView->childViews(Size{frame.width, frame.height})) {
                    if (childView.element == nullptr) {
                        continue;
                    }
                    childView.element->setFrame(Rect{
                        frame.row + childView.frame.row,
                        frame.col + childView.frame.col,
                        childView.frame.width,
                        childView.frame.height,
                    });
                    if (auto* reusable = dynamic_cast<ReusableElement*>(childView.element);
                        reusable != nullptr && reusable->child() != nullptr) {
                        syncReusableChildFrames(*reusable, childView.element->frame());
                        std::vector<Element*> childFocusable = focusableElements(*reusable->child(), activeScrollView);
                        elements.insert(elements.end(), childFocusable.begin(), childFocusable.end());
                        continue;
                    }
                    if (isFocusable(*childView.element)) {
                        elements.push_back(childView.element);
                    }
                }
            }
            continue;
        }
        if (isFocusable(*element)) {
            elements.push_back(element.get());
        }
    }
    return elements;
}

[[nodiscard]] bool hasMultipleFocusableElements(GeneratedWindowBase& window, Element* activeScrollView = nullptr) {
    return focusableElements(window, activeScrollView).size() > 1;
}

[[nodiscard]] std::vector<Element*> mouseTargetElements(GeneratedWindowBase& window, Element* activeScrollView) {
    std::vector<Element*> elements;
    for (const auto& element : window.elements()) {
        if (element == nullptr) {
            continue;
        }
        if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            syncReusableChildFrames(*reusable, reusable->frame());
            if (ScrollView* generatedScrollView = generatedScrollViewForReusable(*reusable)) {
                if (reusable->enabled() && reusable->focusable()) {
                    elements.push_back(reusable);
                }
                if (activeScrollView == generatedScrollView) {
                    std::vector<Element*> childTargets = mouseTargetElements(*reusable->child(), activeScrollView);
                    for (Element* childElement : childTargets) {
                        if (childElement != generatedScrollView) {
                            elements.push_back(childElement);
                        }
                    }
                }
                continue;
            }
            std::vector<Element*> childTargets = mouseTargetElements(*reusable->child(), activeScrollView);
            if (childTargets.empty() &&
                reusable->focusable() &&
                reusable->child()->generatedFocusable() &&
                dynamic_cast<ViewHost*>(reusable) == nullptr) {
                elements.push_back(reusable);
            } else {
                elements.insert(elements.end(), childTargets.begin(), childTargets.end());
            }
            continue;
        }
        if (auto* scrollView = dynamic_cast<ScrollView*>(element.get())) {
            if (isFocusableType(*element)) {
                elements.push_back(element.get());
            }
            if (activeScrollView == scrollView) {
                const Rect frame = scrollView->frame();
                for (const ScrollViewChildView& childView : scrollView->childViews(Size{frame.width, frame.height})) {
                    if (childView.element == nullptr) {
                        continue;
                    }
                    childView.element->setFrame(Rect{
                        frame.row + childView.frame.row,
                        frame.col + childView.frame.col,
                        childView.frame.width,
                        childView.frame.height,
                    });
                    if (auto* reusable = dynamic_cast<ReusableElement*>(childView.element);
                        reusable != nullptr && reusable->child() != nullptr) {
                        syncReusableChildFrames(*reusable, childView.element->frame());
                        std::vector<Element*> childTargets = mouseTargetElements(*reusable->child(), activeScrollView);
                        elements.insert(elements.end(), childTargets.begin(), childTargets.end());
                        continue;
                    }
                    if (isFocusableType(*childView.element)) {
                        elements.push_back(childView.element);
                    }
                }
            }
            continue;
        }
        if (isFocusableType(*element)) {
            elements.push_back(element.get());
        }
    }
    return elements;
}

[[nodiscard]] std::vector<Element*> layoutElements(GeneratedWindowBase& window) {
    std::vector<Element*> elements;
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        if (entry.name.empty()) {
            continue;
        }
        Element* element = findElement(window, entry.name);
        if (element != nullptr) {
            elements.push_back(element);
        }
    }
    return elements;
}

[[nodiscard]] std::optional<int> logicalCellSize(const AxisDimension& dimension, int sourceSize) {
    if (dimension.mode == DimensionMode::Expanded) {
        return std::nullopt;
    }
    if (dimension.mode == DimensionMode::Fixed) {
        return std::max(kMinimumRenderableSize, dimension.value);
    }
    return std::max(kMinimumRenderableSize, dimension.value > 0 ? dimension.value : sourceSize);
}

[[nodiscard]] bool sameRuntimeCell(const RuntimeCell& cell, const GeneratedLayoutEntry& entry) {
    return cell.sourceRow == entry.sourceCell.row &&
           cell.sourceCol == entry.sourceCell.col &&
           cell.sourceWidth == entry.sourceCell.width &&
           cell.sourceHeight == entry.sourceCell.height;
}

[[nodiscard]] bool sameSourceCell(const GeneratedLayoutEntry& lhs, const GeneratedLayoutEntry& rhs) {
    return lhs.sourceCell.row == rhs.sourceCell.row &&
           lhs.sourceCell.col == rhs.sourceCell.col &&
           lhs.sourceCell.width == rhs.sourceCell.width &&
           lhs.sourceCell.height == rhs.sourceCell.height;
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

[[nodiscard]] int fitEntryHeight(const GeneratedWindowBase& window,
                                 const GeneratedLayoutEntry& entry,
                                 std::optional<int> availableCellWidth = std::nullopt) {
    const Element* element = findElement(window, entry.name);
    const int fallback = entry.charsSize.height > 0 ? entry.charsSize.height : entry.sourceCell.height;
    std::optional<int> contentWidth;
    if (availableCellWidth.has_value()) {
        contentWidth = std::max(kMinimumRenderableSize, *availableCellWidth - entry.relative.col - entry.marginRight);
    }
    const int contentHeight = entry.height.mode == DimensionMode::FitContent
        ? fitContentHeightFor(element, entry.type, fallback, contentWidth)
        : std::max(kMinimumRenderableSize, fallback);
    return std::max(kMinimumRenderableSize, entry.relative.row + contentHeight + entry.marginBottom);
}

[[nodiscard]] int fitCellHeight(const GeneratedWindowBase& window,
                                const GeneratedLayoutEntry& cellEntry,
                                std::optional<int> resolvedCellWidth = std::nullopt) {
    int contentHeight = cellEntry.sourceCell.height;
    std::optional<int> availableCellWidth;
    if (resolvedCellWidth.has_value()) {
        availableCellWidth = std::max(kMinimumRenderableSize,
                                      *resolvedCellWidth - stylePaddingLeft(cellEntry.cellStyle) -
                                      stylePaddingRight(cellEntry.cellStyle));
    }
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        if (sameSourceCell(entry, cellEntry)) {
            contentHeight = std::max(contentHeight, fitEntryHeight(window, entry, availableCellWidth));
        }
    }
    return std::max(kMinimumRenderableSize,
                    contentHeight + stylePaddingTop(cellEntry.cellStyle) + stylePaddingBottom(cellEntry.cellStyle));
}

[[nodiscard]] int fitEntryWidth(const GeneratedWindowBase& window, const GeneratedLayoutEntry& entry) {
    const Element* element = findElement(window, entry.name);
    const int fallback = entry.charsSize.width > 0 ? entry.charsSize.width : entry.sourceCell.width;
    int contentWidth = fallback;
    if (const auto* listBox = dynamic_cast<const ListBox*>(element)) {
        for (const std::string& option : listBox->options()) {
            contentWidth = std::max(contentWidth, static_cast<int>(option.size()));
        }
    } else if (const auto* label = dynamic_cast<const Label*>(element)) {
        std::string_view text = label->text();
        int lineWidth = 0;
        for (char ch : text) {
            if (ch == '\n') {
                contentWidth = std::max(contentWidth, lineWidth);
                lineWidth = 0;
            } else if (ch != '\r') {
                ++lineWidth;
            }
        }
        contentWidth = std::max(contentWidth, lineWidth);
    }
    return std::max(kMinimumRenderableSize, entry.relative.col + contentWidth + entry.marginRight);
}

[[nodiscard]] int fitCellWidth(const GeneratedWindowBase& window, const GeneratedLayoutEntry& cellEntry) {
    int contentWidth = cellEntry.sourceCell.width;
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        if (sameSourceCell(entry, cellEntry)) {
            contentWidth = std::max(contentWidth, fitEntryWidth(window, entry));
        }
    }
    return std::max(kMinimumRenderableSize,
                    contentWidth + stylePaddingLeft(cellEntry.cellStyle) + stylePaddingRight(cellEntry.cellStyle));
}

[[nodiscard]] std::vector<RuntimeCell> collectRuntimeCells(const GeneratedWindowBase& window) {
    std::vector<RuntimeCell> cells;
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        const auto exists = std::any_of(cells.begin(), cells.end(), [&](const RuntimeCell& cell) {
            return sameRuntimeCell(cell, entry);
        });
        if (exists) {
            continue;
        }
        std::optional<int> logicalWidth = logicalCellSize(entry.cellWidth, entry.sourceCell.width);
        std::optional<int> logicalHeight = logicalCellSize(entry.cellHeight, entry.sourceCell.height);
        if (entry.cellWidth.mode == DimensionMode::FitContent) {
            logicalWidth = fitCellWidth(window, entry);
        }
        if (entry.cellHeight.mode == DimensionMode::FitContent) {
            logicalHeight = fitCellHeight(window, entry);
        }
        if (logicalWidth.has_value() &&
            entry.cellWidth.mode != DimensionMode::Expanded &&
            entry.cellWidth.mode != DimensionMode::FitContent) {
            logicalWidth = std::max(kMinimumRenderableSize,
                                    *logicalWidth + stylePaddingLeft(entry.cellStyle) +
                                                    stylePaddingRight(entry.cellStyle));
        }
        if (logicalHeight.has_value() &&
            entry.cellHeight.mode != DimensionMode::Expanded &&
            entry.cellHeight.mode != DimensionMode::FitContent) {
            logicalHeight = std::max(kMinimumRenderableSize,
                                     *logicalHeight + stylePaddingTop(entry.cellStyle) +
                                                      stylePaddingBottom(entry.cellStyle));
        }
        cells.push_back(RuntimeCell{
            .sourceRow = entry.sourceCell.row,
            .sourceCol = entry.sourceCell.col,
            .sourceWidth = std::max(kMinimumRenderableSize, entry.sourceCell.width),
            .sourceHeight = std::max(kMinimumRenderableSize, entry.sourceCell.height),
            .logicalWidth = logicalWidth,
            .logicalHeight = logicalHeight,
        });
    }
    return cells;
}

[[nodiscard]] std::shared_ptr<RuntimeNode> makeLeafNode(const RuntimeCell& cell) {
    auto node = std::make_shared<RuntimeNode>();
    node->cell = cell;
    node->left = cell.sourceCol;
    node->top = cell.sourceRow;
    node->right = node->left + cell.sourceWidth + 1;
    node->bottom = node->top + cell.sourceHeight + 1;
    node->logicalWidth = cell.logicalWidth;
    node->logicalHeight = cell.logicalHeight;
    return node;
}

[[nodiscard]] bool intervalsCover(std::vector<std::pair<int, int>> spans, int start, int end) {
    if (spans.empty()) {
        return false;
    }
    std::sort(spans.begin(), spans.end());
    int cursor = start;
    for (const auto& [spanStart, spanEnd] : spans) {
        if (spanStart > cursor) {
            return false;
        }
        cursor = std::max(cursor, spanEnd);
        if (cursor >= end) {
            return true;
        }
    }
    return cursor >= end;
}

[[nodiscard]] std::vector<int> horizontalCuts(const std::vector<std::shared_ptr<RuntimeNode>>& nodes,
                                              int left, int top, int right, int bottom) {
    std::vector<int> candidates;
    for (const auto& node : nodes) {
        candidates.push_back(node->top);
        candidates.push_back(node->bottom);
    }
    std::sort(candidates.begin(), candidates.end());
    candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());

    std::vector<int> cuts;
    for (int candidate : candidates) {
        if (candidate <= top || candidate >= bottom) {
            continue;
        }
        std::vector<std::pair<int, int>> spans;
        for (const auto& node : nodes) {
            if (node->top == candidate || node->bottom == candidate) {
                spans.emplace_back(node->left, node->right);
            }
        }
        if (intervalsCover(std::move(spans), left, right)) {
            cuts.push_back(candidate);
        }
    }
    return cuts;
}

[[nodiscard]] std::vector<int> verticalCuts(const std::vector<std::shared_ptr<RuntimeNode>>& nodes,
                                            int left, int top, int right, int bottom) {
    std::vector<int> candidates;
    for (const auto& node : nodes) {
        candidates.push_back(node->left);
        candidates.push_back(node->right);
    }
    std::sort(candidates.begin(), candidates.end());
    candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());

    std::vector<int> cuts;
    for (int candidate : candidates) {
        if (candidate <= left || candidate >= right) {
            continue;
        }
        std::vector<std::pair<int, int>> spans;
        for (const auto& node : nodes) {
            if (node->left == candidate || node->right == candidate) {
                spans.emplace_back(node->top, node->bottom);
            }
        }
        if (intervalsCover(std::move(spans), top, bottom)) {
            cuts.push_back(candidate);
        }
    }
    return cuts;
}

[[nodiscard]] std::vector<std::vector<std::shared_ptr<RuntimeNode>>> sliceByRows(
    const std::vector<std::shared_ptr<RuntimeNode>>& nodes, const std::vector<int>& boundaries) {
    std::vector<std::vector<std::shared_ptr<RuntimeNode>>> groups;
    std::vector<bool> used(nodes.size(), false);
    for (std::size_t index = 0; index + 1 < boundaries.size(); ++index) {
        const int bandTop = boundaries[index];
        const int bandBottom = boundaries[index + 1];
        std::vector<std::shared_ptr<RuntimeNode>> group;
        for (std::size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) {
            if (nodes[nodeIndex]->top >= bandTop && nodes[nodeIndex]->bottom <= bandBottom) {
                group.push_back(nodes[nodeIndex]);
                used[nodeIndex] = true;
            }
        }
        if (group.empty()) {
            return {};
        }
        std::sort(group.begin(), group.end(), [](const auto& lhs, const auto& rhs) {
            return std::tie(lhs->top, lhs->left) < std::tie(rhs->top, rhs->left);
        });
        groups.push_back(std::move(group));
    }
    return std::all_of(used.begin(), used.end(), [](bool value) { return value; }) ? groups : std::vector<std::vector<std::shared_ptr<RuntimeNode>>>{};
}

[[nodiscard]] std::vector<std::vector<std::shared_ptr<RuntimeNode>>> sliceByCols(
    const std::vector<std::shared_ptr<RuntimeNode>>& nodes, const std::vector<int>& boundaries) {
    std::vector<std::vector<std::shared_ptr<RuntimeNode>>> groups;
    std::vector<bool> used(nodes.size(), false);
    for (std::size_t index = 0; index + 1 < boundaries.size(); ++index) {
        const int bandLeft = boundaries[index];
        const int bandRight = boundaries[index + 1];
        std::vector<std::shared_ptr<RuntimeNode>> group;
        for (std::size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) {
            if (nodes[nodeIndex]->left >= bandLeft && nodes[nodeIndex]->right <= bandRight) {
                group.push_back(nodes[nodeIndex]);
                used[nodeIndex] = true;
            }
        }
        if (group.empty()) {
            return {};
        }
        std::sort(group.begin(), group.end(), [](const auto& lhs, const auto& rhs) {
            return std::tie(lhs->left, lhs->top) < std::tie(rhs->left, rhs->top);
        });
        groups.push_back(std::move(group));
    }
    return std::all_of(used.begin(), used.end(), [](bool value) { return value; }) ? groups : std::vector<std::vector<std::shared_ptr<RuntimeNode>>>{};
}

[[nodiscard]] std::shared_ptr<RuntimeNode> buildRuntimeTree(std::vector<std::shared_ptr<RuntimeNode>> nodes,
                                                            int horizontalSeparator,
                                                            int verticalSeparator);

[[nodiscard]] std::shared_ptr<RuntimeNode> makeGroupNode(std::string orientation,
                                                         std::vector<std::shared_ptr<RuntimeNode>> children,
                                                         int left, int top, int right, int bottom,
                                                         int horizontalSeparator,
                                                         int verticalSeparator) {
    auto node = std::make_shared<RuntimeNode>();
    node->orientation = std::move(orientation);
    node->children = std::move(children);
    node->left = left;
    node->top = top;
    node->right = right;
    node->bottom = bottom;

    if (node->orientation == "vertical") {
        for (const auto& child : node->children) {
            if (!child->logicalWidth.has_value()) {
                node->logicalWidth.reset();
                break;
            }
            node->logicalWidth = std::max(node->logicalWidth.value_or(kMinimumRenderableSize), *child->logicalWidth);
        }
        int height = 0;
        for (const auto& child : node->children) {
            if (!child->logicalHeight.has_value()) {
                node->logicalHeight.reset();
                return node;
            }
            height += *child->logicalHeight;
        }
        height += std::max(0, static_cast<int>(node->children.size()) - 1) * horizontalSeparator;
        node->logicalHeight = height;
    } else {
        for (const auto& child : node->children) {
            if (!child->logicalHeight.has_value()) {
                node->logicalHeight.reset();
                break;
            }
            node->logicalHeight = std::max(node->logicalHeight.value_or(kMinimumRenderableSize), *child->logicalHeight);
        }
        int width = 0;
        for (const auto& child : node->children) {
            if (!child->logicalWidth.has_value()) {
                node->logicalWidth.reset();
                return node;
            }
            width += *child->logicalWidth;
        }
        width += std::max(0, static_cast<int>(node->children.size()) - 1) * verticalSeparator;
        node->logicalWidth = width;
    }
    return node;
}

[[nodiscard]] std::shared_ptr<RuntimeNode> buildRuntimeTree(std::vector<std::shared_ptr<RuntimeNode>> nodes,
                                                            int horizontalSeparator,
                                                            int verticalSeparator) {
    if (nodes.empty()) {
        return nullptr;
    }
    if (nodes.size() == 1) {
        return nodes.front();
    }
    std::sort(nodes.begin(), nodes.end(), [](const auto& lhs, const auto& rhs) {
        return std::tie(lhs->top, lhs->left, lhs->bottom, lhs->right) <
               std::tie(rhs->top, rhs->left, rhs->bottom, rhs->right);
    });

    const int left = std::min_element(nodes.begin(), nodes.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->left < rhs->left;
    })->get()->left;
    const int top = std::min_element(nodes.begin(), nodes.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->top < rhs->top;
    })->get()->top;
    const int right = std::max_element(nodes.begin(), nodes.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->right < rhs->right;
    })->get()->right;
    const int bottom = std::max_element(nodes.begin(), nodes.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->bottom < rhs->bottom;
    })->get()->bottom;

    std::vector<int> cuts = horizontalCuts(nodes, left, top, right, bottom);
    if (!cuts.empty()) {
        std::vector<int> boundaries{top};
        boundaries.insert(boundaries.end(), cuts.begin(), cuts.end());
        boundaries.push_back(bottom);
        auto groups = sliceByRows(nodes, boundaries);
        if (groups.size() > 1) {
            std::vector<std::shared_ptr<RuntimeNode>> children;
            for (auto& group : groups) {
                children.push_back(buildRuntimeTree(std::move(group), horizontalSeparator, verticalSeparator));
            }
            return makeGroupNode("vertical", std::move(children), left, top, right, bottom,
                                 horizontalSeparator, verticalSeparator);
        }
    }

    cuts = verticalCuts(nodes, left, top, right, bottom);
    if (!cuts.empty()) {
        std::vector<int> boundaries{left};
        boundaries.insert(boundaries.end(), cuts.begin(), cuts.end());
        boundaries.push_back(right);
        auto groups = sliceByCols(nodes, boundaries);
        if (groups.size() > 1) {
            std::vector<std::shared_ptr<RuntimeNode>> children;
            for (auto& group : groups) {
                children.push_back(buildRuntimeTree(std::move(group), horizontalSeparator, verticalSeparator));
            }
            return makeGroupNode("horizontal", std::move(children), left, top, right, bottom,
                                 horizontalSeparator, verticalSeparator);
        }
    }

    return nodes.front();
}

[[nodiscard]] std::shared_ptr<RuntimeNode> buildRuntimeTree(const std::vector<RuntimeCell>& cells,
                                                            int horizontalSeparator,
                                                            int verticalSeparator) {
    std::vector<std::shared_ptr<RuntimeNode>> leaves;
    leaves.reserve(cells.size());
    for (const RuntimeCell& cell : cells) {
        leaves.push_back(makeLeafNode(cell));
    }
    return buildRuntimeTree(std::move(leaves), horizontalSeparator, verticalSeparator);
}

void resolveRuntimeNode(const std::shared_ptr<RuntimeNode>& node, Rect rect, std::vector<ResolvedRuntimeCell>& resolved,
                        int horizontalSeparator, int verticalSeparator, GeneratedWindowMode mode) {
    if (node == nullptr) {
        return;
    }
    rect.width = std::max(kMinimumRenderableSize, rect.width);
    rect.height = std::max(kMinimumRenderableSize, rect.height);
    if (node->children.empty()) {
        resolved.push_back(ResolvedRuntimeCell{.cell = node->cell, .rect = rect});
        return;
    }

    if (node->orientation == "vertical") {
        int fixedTotal = 0;
        int expandedCount = 0;
        for (const auto& child : node->children) {
            if (child->logicalHeight.has_value()) {
                fixedTotal += *child->logicalHeight;
            } else {
                ++expandedCount;
            }
        }
        const int separators = std::max(0, static_cast<int>(node->children.size()) - 1) * horizontalSeparator;
        int remaining = std::max(0, rect.height - fixedTotal - separators);
        const bool distributeFixedExtra = expandedCount == 0 && remaining > 0 && mode == GeneratedWindowMode::Fullscreen;
        const bool appendTrailingExtra = expandedCount == 0 && remaining > 0 && mode != GeneratedWindowMode::Fullscreen &&
                                         horizontalSeparator > 0;
        const int expandTargets = distributeFixedExtra ? static_cast<int>(node->children.size()) : expandedCount;
        const int share = expandTargets > 0 ? remaining / expandTargets : 0;
        const int remainder = expandTargets > 0 ? remaining % expandTargets : 0;
        int expandIndex = 0;
        int cursor = rect.row;
        for (std::size_t index = 0; index < node->children.size(); ++index) {
            const auto& child = node->children[index];
            int childHeight = child->logicalHeight.value_or(0);
            if (!child->logicalHeight.has_value() || distributeFixedExtra) {
                childHeight = (distributeFixedExtra ? childHeight : 0) + share + (expandIndex < remainder ? 1 : 0);
                ++expandIndex;
            } else if (appendTrailingExtra && index + 1 == node->children.size()) {
                childHeight += remaining;
            }
            resolveRuntimeNode(child, Rect{cursor, rect.col, rect.width, std::max(kMinimumRenderableSize, childHeight)},
                               resolved, horizontalSeparator, verticalSeparator, mode);
            cursor += std::max(kMinimumRenderableSize, childHeight) + horizontalSeparator;
        }
        return;
    }

    int fixedTotal = 0;
    int expandedCount = 0;
    for (const auto& child : node->children) {
        if (child->logicalWidth.has_value()) {
            fixedTotal += *child->logicalWidth;
        } else {
            ++expandedCount;
        }
    }
    const int separators = std::max(0, static_cast<int>(node->children.size()) - 1) * verticalSeparator;
    int remaining = std::max(0, rect.width - fixedTotal - separators);
    const bool distributeFixedExtra = expandedCount == 0 && remaining > 0 && mode == GeneratedWindowMode::Fullscreen;
    const bool appendTrailingExtra = expandedCount == 0 && remaining > 0 && mode != GeneratedWindowMode::Fullscreen;
    const int expandTargets = distributeFixedExtra ? static_cast<int>(node->children.size()) : expandedCount;
    const int share = expandTargets > 0 ? remaining / expandTargets : 0;
    const int remainder = expandTargets > 0 ? remaining % expandTargets : 0;
    int expandIndex = 0;
    int cursor = rect.col;
    for (std::size_t index = 0; index < node->children.size(); ++index) {
        const auto& child = node->children[index];
        int childWidth = child->logicalWidth.value_or(0);
        if (!child->logicalWidth.has_value() || distributeFixedExtra) {
            childWidth = (distributeFixedExtra ? childWidth : 0) + share + (expandIndex < remainder ? 1 : 0);
            ++expandIndex;
        } else if (appendTrailingExtra && index + 1 == node->children.size()) {
            childWidth += remaining;
        }
        resolveRuntimeNode(child, Rect{rect.row, cursor, std::max(kMinimumRenderableSize, childWidth), rect.height},
                           resolved, horizontalSeparator, verticalSeparator, mode);
        cursor += std::max(kMinimumRenderableSize, childWidth) + verticalSeparator;
    }
}

[[nodiscard]] GeneratedWindowMode windowMode(const GeneratedWindowBase& window) {
    bool expandsWidth = false;
    bool expandsHeight = false;
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        expandsWidth = expandsWidth || entry.cellWidth.mode == DimensionMode::Expanded;
        expandsHeight = expandsHeight || entry.cellHeight.mode == DimensionMode::Expanded;
    }
    if (expandsWidth && expandsHeight) {
        return GeneratedWindowMode::Fullscreen;
    }
    if (expandsWidth) {
        return GeneratedWindowMode::ExpandWidth;
    }
    if (expandsHeight) {
        return GeneratedWindowMode::ExpandHeight;
    }
    return GeneratedWindowMode::Normal;
}

[[nodiscard]] std::string windowModeName(GeneratedWindowMode mode) {
    switch (mode) {
        case GeneratedWindowMode::Fullscreen:
            return "fullscreen";
        case GeneratedWindowMode::ExpandWidth:
            return "expand_width";
        case GeneratedWindowMode::ExpandHeight:
            return "expand_height";
        case GeneratedWindowMode::Normal:
            return "normal";
    }
    return "normal";
}

[[nodiscard]] Size activeWindowReportedSize(GeneratedWindowBase& window, const McpRuntimeState& state) {
    Size size = generatedWindowContentSize(window);
    const Style& style = window.generatedWindowStyle();
    const int horizontalBorder = std::max(kNoBorderWidth, style.borderWidthHorizontal.value_or(kNoBorderWidth));
    const int verticalBorder = std::max(kNoBorderWidth, style.borderWidthVertical.value_or(kNoBorderWidth));
    size.width += verticalBorder * 2;
    size.height += horizontalBorder * 2;
    const GeneratedWindowMode mode = windowMode(window);
    if (state.viewport.has_value()) {
        const int leftMargin = std::max(0, style.marginLeft.value_or(style.margin.value_or(0)));
        const int rightMargin = std::max(0, style.marginRight.value_or(style.margin.value_or(0)));
        const int topMargin = std::max(0, style.marginTop.value_or(style.margin.value_or(0)));
        const int bottomMargin = std::max(0, style.marginBottom.value_or(style.margin.value_or(0)));
        const int availableWidth = std::max(kMinimumRenderableSize, state.viewport->width - leftMargin - rightMargin);
        const int availableHeight = std::max(kMinimumRenderableSize, state.viewport->height - topMargin - bottomMargin);
        if (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandWidth) {
            size.width = availableWidth;
        } else {
            size.width = std::min(size.width, availableWidth);
        }
        if (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandHeight) {
            size.height = availableHeight;
        } else {
            size.height = std::min(size.height, availableHeight);
        }
    }
    return size;
}

[[nodiscard]] int borderWidthHorizontal(const Style& style) {
    return std::max(kNoBorderWidth, style.borderWidthHorizontal.value_or(kNoBorderWidth));
}

[[nodiscard]] int borderWidthVertical(const Style& style) {
    return std::max(kNoBorderWidth, style.borderWidthVertical.value_or(kNoBorderWidth));
}

[[nodiscard]] int paddingTop(const Style& style) {
    return std::max(0, style.paddingTop.value_or(style.padding.value_or(0)));
}

[[nodiscard]] int paddingRight(const Style& style) {
    return std::max(0, style.paddingRight.value_or(style.padding.value_or(0)));
}

[[nodiscard]] int paddingBottom(const Style& style) {
    return std::max(0, style.paddingBottom.value_or(style.padding.value_or(0)));
}

[[nodiscard]] int paddingLeft(const Style& style) {
    return std::max(0, style.paddingLeft.value_or(style.padding.value_or(0)));
}

[[nodiscard]] int marginTop(const Style& style) {
    return std::max(0, style.marginTop.value_or(style.margin.value_or(0)));
}

[[nodiscard]] int marginRight(const Style& style) {
    return std::max(0, style.marginRight.value_or(style.margin.value_or(0)));
}

[[nodiscard]] int marginBottom(const Style& style) {
    return std::max(0, style.marginBottom.value_or(style.margin.value_or(0)));
}

[[nodiscard]] int marginLeft(const Style& style) {
    return std::max(0, style.marginLeft.value_or(style.margin.value_or(0)));
}

[[nodiscard]] bool windowElementFramesSyncedTo(const GeneratedWindowBase& window, Rect frame) {
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        if (entry.name.empty()) {
            continue;
        }
        const Element* element = findElement(window, entry.name);
        if (element == nullptr) {
            continue;
        }
        const Rect elementFrame = element->frame();
        if (elementFrame.width <= 0 || elementFrame.height <= 0) {
            return false;
        }
        const int expectedRow = frame.row + entry.sourceCell.row + paddingTop(entry.cellStyle) + entry.relative.row;
        const int expectedCol = frame.col + entry.sourceCell.col + paddingLeft(entry.cellStyle) + entry.relative.col;
        return elementFrame.row == expectedRow && elementFrame.col == expectedCol;
    }
    return false;
}

[[nodiscard]] Size layoutSize(const std::shared_ptr<RuntimeNode>& root) {
    if (root == nullptr) {
        return Size{kMinimumRenderableSize, kMinimumRenderableSize};
    }
    const int sourceWidth = std::max(kMinimumRenderableSize, root->right - root->left - 1);
    const int sourceHeight = std::max(kMinimumRenderableSize, root->bottom - root->top - 1);
    const int width = std::max(kMinimumRenderableSize, root->logicalWidth.value_or(sourceWidth));
    const int height = std::max(kMinimumRenderableSize, root->logicalHeight.value_or(sourceHeight));
    return Size{width, height};
}

[[nodiscard]] Rect layoutContentRect(const std::shared_ptr<RuntimeNode>& runtimeTree,
                                     Rect content,
                                     GeneratedWindowMode mode) {
    const Size naturalContent = layoutSize(runtimeTree);
    Rect layoutContent = content;
    if (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandWidth) {
        layoutContent.width = std::max(layoutContent.width, naturalContent.width);
    }
    if (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandHeight) {
        layoutContent.height = std::max(layoutContent.height, naturalContent.height);
    }
    return layoutContent;
}

void resolveRuntimeCellsWithFitPass(const GeneratedWindowBase& window,
                                    std::vector<RuntimeCell>& runtimeCells,
                                    Rect content,
                                    int horizontalSeparator,
                                    int verticalSeparator,
                                    GeneratedWindowMode mode,
                                    std::vector<ResolvedRuntimeCell>& resolvedCells) {
    std::shared_ptr<RuntimeNode> runtimeTree = buildRuntimeTree(runtimeCells, horizontalSeparator, verticalSeparator);
    resolvedCells.clear();
    resolveRuntimeNode(runtimeTree, layoutContentRect(runtimeTree, content, mode), resolvedCells,
                       horizontalSeparator, verticalSeparator, mode);
    if (!recomputeFitContentHeightsPass2(window, runtimeCells, resolvedCells)) {
        return;
    }
    runtimeTree = buildRuntimeTree(runtimeCells, horizontalSeparator, verticalSeparator);
    resolvedCells.clear();
    resolveRuntimeNode(runtimeTree, layoutContentRect(runtimeTree, content, mode), resolvedCells,
                       horizontalSeparator, verticalSeparator, mode);
}

[[nodiscard]] Size resolvedContentExtent(const std::vector<ResolvedRuntimeCell>& resolvedCells, Size fallback) {
    Size result{
        std::max(kMinimumRenderableSize, fallback.width),
        std::max(kMinimumRenderableSize, fallback.height),
    };
    for (const ResolvedRuntimeCell& cell : resolvedCells) {
        result.width = std::max(result.width, cell.rect.col + cell.rect.width);
        result.height = std::max(result.height, cell.rect.row + cell.rect.height);
    }
    return result;
}

[[nodiscard]] Rect windowBounds(const GeneratedWindowBase& window, const TerminalBuffer& buffer) {
    const Style& style = window.generatedWindowStyle();
    const Size content = generatedWindowContentSize(window);
    const GeneratedWindowMode mode = windowMode(window);
    const int naturalWidth = content.width + borderWidthVertical(style) * 2;
    const int naturalHeight = content.height + borderWidthHorizontal(style) * 2;
    const int leftMargin = marginLeft(style);
    const int rightMargin = marginRight(style);
    const int topMargin = marginTop(style);
    const int bottomMargin = marginBottom(style);
    const int availableWidth = std::max(kMinimumRenderableSize, buffer.width() - leftMargin - rightMargin);
    const int availableHeight = std::max(kMinimumRenderableSize, buffer.height() - topMargin - bottomMargin - kContentTopRow);
    const int width = (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandWidth)
        ? availableWidth
        : std::min(naturalWidth, availableWidth);
    const int height = (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandHeight)
        ? availableHeight
        : std::min(naturalHeight, availableHeight);
    return Rect{
        (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandHeight)
            ? kContentTopRow + topMargin
            : kContentTopRow + topMargin + std::max(0, (availableHeight - height) / 2),
        (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandWidth)
            ? kContentLeftCol + leftMargin
            : kContentLeftCol + leftMargin + std::max(0, (availableWidth - width) / 2),
        width,
        height,
    };
}

[[nodiscard]] Rect refreshWindowStackFrameBounds(GeneratedWindowStackFrame& frame, const TerminalBuffer& buffer) {
    if (frame.window == nullptr) {
        frame.bounds = Rect{};
        return frame.bounds;
    }
    frame.bounds = windowBounds(*frame.window, buffer);
    return frame.bounds;
}

[[nodiscard]] Rect contentBounds(Rect bounds, const Style& style) {
    const int horizontal = borderWidthHorizontal(style);
    const int vertical = borderWidthVertical(style);
    return Rect{
        bounds.row + horizontal,
        bounds.col + vertical,
        std::max(kMinimumRenderableSize, bounds.width - vertical * 2),
        std::max(kMinimumRenderableSize, bounds.height - horizontal * 2),
    };
}

[[nodiscard]] Point stackFrameContentPoint(const GeneratedWindowStackFrame& frame,
                                           Rect frameBounds,
                                           Point terminalPoint) {
    if (frame.window == nullptr) {
        return terminalPoint;
    }
    const Rect content = contentBounds(Rect{0, 0, frameBounds.width, frameBounds.height},
                                       frame.window->generatedWindowStyle());
    return Point{
        terminalPoint.row - frameBounds.row - content.row,
        terminalPoint.col - frameBounds.col - content.col,
    };
}

[[nodiscard]] Point windowContentPoint(const GeneratedWindowBase& window,
                                       const TerminalBuffer& buffer,
                                       Point terminalPoint) {
    const Rect bounds = windowBounds(window, buffer);
    const Rect content = contentBounds(Rect{0, 0, bounds.width, bounds.height},
                                       window.generatedWindowStyle());
    return Point{
        terminalPoint.row - bounds.row - content.row,
        terminalPoint.col - bounds.col - content.col,
    };
}

[[nodiscard]] RenderedContent renderElement(Element& element, Size size, ElementRenderState state) {
    if (auto* infoLabel = dynamic_cast<InfoLabel*>(&element)) {
        return infoLabel->render(size, state);
    }
    if (auto* messageTable = dynamic_cast<MessageTable*>(&element)) {
        return messageTable->render(size, state);
    }
    if (auto* frameBufferView = dynamic_cast<FrameBufferView*>(&element)) {
        return frameBufferView->render(size, state);
    }
    if (auto* image = dynamic_cast<Image*>(&element)) {
        return image->render(size, state);
    }
    if (auto* label = dynamic_cast<Label*>(&element)) {
        return label->render(size, state);
    }
    if (auto* button = dynamic_cast<Button*>(&element)) {
        return button->render(size, state);
    }
    if (auto* checkbox = dynamic_cast<CheckBox*>(&element)) {
        return checkbox->render(size, state);
    }
    if (auto* textInput = dynamic_cast<TextInput*>(&element)) {
        return textInput->render(size, state);
    }
    if (auto* numberInput = dynamic_cast<NumberInput*>(&element)) {
        return numberInput->render(size, state);
    }
    if (auto* comboBox = dynamic_cast<ComboBox*>(&element)) {
        return comboBox->render(size, state);
    }
    if (auto* listBox = dynamic_cast<ListBox*>(&element)) {
        return listBox->render(size, state);
    }
    if (auto* reusable = dynamic_cast<ReusableElement*>(&element)) {
        return reusable->render(size, state);
    }
    if (auto* scrollView = dynamic_cast<ScrollView*>(&element)) {
        return scrollView->render(size, state);
    }
    return renderPlainText(element.name(), size.width, size.height, element.effectiveStyle(state.focused, state.editMode));
}

void blit(TerminalBuffer& buffer, const RenderedContent& content, int row, int col) {
    for (std::size_t localRow = 0; localRow < content.size(); ++localRow) {
        for (std::size_t localCol = 0; localCol < content[localRow].size(); ++localCol) {
            const int targetRow = row + static_cast<int>(localRow);
            const int targetCol = col + static_cast<int>(localCol);
            if (targetRow < 0 || targetRow >= buffer.height() || targetCol < 0 || targetCol >= buffer.width()) {
                continue;
            }
            TerminalCell cell = content[localRow][localCol];
            const TerminalCell& existing = buffer.cell(targetRow, targetCol);
            cell.background = blendBackgroundOverExisting(cell.background, existing.background);
            cell.foreground = blendForegroundOverBackground(cell.foreground, existing.foreground, cell.background);
            buffer.setCell(targetRow, targetCol, std::move(cell));
        }
    }
}

void blitClipped(TerminalBuffer& buffer, const RenderedContent& content, int row, int col, Rect clip) {
    for (std::size_t localRow = 0; localRow < content.size(); ++localRow) {
        for (std::size_t localCol = 0; localCol < content[localRow].size(); ++localCol) {
            const int targetRow = row + static_cast<int>(localRow);
            const int targetCol = col + static_cast<int>(localCol);
            if (!clip.contains(Point{targetRow, targetCol}) ||
                targetRow < 0 || targetRow >= buffer.height() || targetCol < 0 || targetCol >= buffer.width()) {
                continue;
            }
            TerminalCell cell = content[localRow][localCol];
            const TerminalCell& existing = buffer.cell(targetRow, targetCol);
            cell.background = blendBackgroundOverExisting(cell.background, existing.background);
            cell.foreground = blendForegroundOverBackground(cell.foreground, existing.foreground, cell.background);
            buffer.setCell(targetRow, targetCol, std::move(cell));
        }
    }
}

[[nodiscard]] Rect intersectRects(Rect lhs, Rect rhs) {
    const int row = std::max(lhs.row, rhs.row);
    const int col = std::max(lhs.col, rhs.col);
    const int bottom = std::min(lhs.row + lhs.height, rhs.row + rhs.height);
    const int right = std::min(lhs.col + lhs.width, rhs.col + rhs.width);
    return Rect{
        row,
        col,
        std::max(0, right - col),
        std::max(0, bottom - row),
    };
}

[[nodiscard]] RenderedContent contentFromBuffer(const TerminalBuffer& buffer) {
    RenderedContent rendered;
    rendered.reserve(static_cast<std::size_t>(buffer.height()));
    for (int row = 0; row < buffer.height(); ++row) {
        RenderedRow renderedRow;
        renderedRow.reserve(static_cast<std::size_t>(buffer.width()));
        for (int col = 0; col < buffer.width(); ++col) {
            renderedRow.push_back(buffer.cell(row, col));
        }
        rendered.push_back(std::move(renderedRow));
    }
    return rendered;
}

void fillRect(TerminalBuffer& buffer, Rect rect, const Style& style) {
    const std::vector<std::string> textureGlyphs = utf8Glyphs(style.backgroundTexture.value_or(""));
    for (int row = std::max(0, rect.row); row < std::min(buffer.height(), rect.row + rect.height); ++row) {
        for (int col = std::max(0, rect.col); col < std::min(buffer.width(), rect.col + rect.width); ++col) {
            TerminalCell cell = styledFillCell(style, textureGlyphs, row - rect.row, col - rect.col, rect.width);
            const TerminalCell& existing = buffer.cell(row, col);
            cell.background = blendBackgroundOverExisting(cell.background, existing.background);
            cell.foreground = blendForegroundOverBackground(cell.foreground, existing.foreground, cell.background);
            buffer.setCell(row, col, std::move(cell));
        }
    }
}

[[nodiscard]] Style borderStyleFor(const Style& style) {
    Style borderStyle;
    borderStyle.color = style.borderColor;
    borderStyle.background = style.background;
    return borderStyle;
}

[[nodiscard]] const ResolvedRuntimeCell* resolvedCellFor(const std::vector<ResolvedRuntimeCell>& cells,
                                                        const GeneratedLayoutEntry& entry) {
    const auto it = std::find_if(cells.begin(), cells.end(), [&](const ResolvedRuntimeCell& cell) {
        return sameRuntimeCell(cell.cell, entry);
    });
    return it == cells.end() ? nullptr : &*it;
}

[[nodiscard]] bool recomputeFitContentHeightsPass2(const GeneratedWindowBase& window,
                                                   std::vector<RuntimeCell>& cells,
                                                   const std::vector<ResolvedRuntimeCell>& resolvedCells) {
    bool changed = false;
    for (RuntimeCell& runtimeCell : cells) {
        const GeneratedLayoutEntry* cellEntry = nullptr;
        for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
            if (sameRuntimeCell(runtimeCell, entry) && entry.cellHeight.mode == DimensionMode::FitContent) {
                cellEntry = &entry;
                break;
            }
        }
        if (cellEntry == nullptr) {
            continue;
        }

        const ResolvedRuntimeCell* resolved = resolvedCellFor(resolvedCells, *cellEntry);
        if (resolved == nullptr) {
            continue;
        }

        const int newHeight = fitCellHeight(window, *cellEntry, resolved->rect.width);
        if (!runtimeCell.logicalHeight.has_value() || *runtimeCell.logicalHeight != newHeight) {
            runtimeCell.logicalHeight = newHeight;
            changed = true;
        }
    }
    return changed;
}

[[nodiscard]] int dimensionWithin(const AxisDimension& dimension, int available, int fallback) {
    available = std::max(kMinimumRenderableSize, available);
    if (dimension.mode == DimensionMode::Expanded) {
        return available;
    }
    if (dimension.mode == DimensionMode::FitContent) {
        return std::max(kMinimumRenderableSize, std::min(std::max(kMinimumRenderableSize, fallback), available));
    }
    const int requested = dimension.value > 0 ? dimension.value : fallback;
    return std::max(kMinimumRenderableSize, requested);
}

[[nodiscard]] int textLineCount(std::string_view text) {
    if (text.empty()) {
        return 0;
    }
    int lines = 1;
    for (char ch : text) {
        if (ch == '\n') {
            ++lines;
        }
    }
    return lines;
}

[[nodiscard]] int wrappedTextLineCount(std::string_view text, int width) {
    width = std::max(kMinimumRenderableSize, width);
    if (text.empty()) {
        return kMinimumRenderableSize;
    }
    int rows = 0;
    std::size_t segmentStart = 0;
    while (segmentStart <= text.size()) {
        const std::size_t newline = text.find('\n', segmentStart);
        const std::size_t segmentEnd = newline == std::string_view::npos ? text.size() : newline;
        const std::string_view segment = text.substr(segmentStart, segmentEnd - segmentStart);
        if (segment.empty()) {
            ++rows;
        } else {
            std::size_t offset = 0;
            while (offset < segment.size()) {
                const std::size_t remaining = segment.size() - offset;
                const std::size_t chunkSize = std::min<std::size_t>(remaining, static_cast<std::size_t>(width));
                const std::string_view chunk = segment.substr(offset, chunkSize);
                ++rows;
                if (chunkSize < static_cast<std::size_t>(width)) {
                    offset += chunkSize;
                    continue;
                }
                const std::size_t lastSpace = chunk.rfind(' ');
                if (lastSpace != std::string_view::npos && lastSpace > 0) {
                    offset += lastSpace + 1;
                } else {
                    offset += chunkSize;
                }
            }
        }
        if (newline == std::string_view::npos) {
            break;
        }
        segmentStart = newline + 1;
    }
    return std::max(kMinimumRenderableSize, rows);
}

[[nodiscard]] int fitContentHeightFor(const Element* element, std::string_view type, int fallback,
                                      std::optional<int> width) {
    if (element == nullptr) {
        return std::max(kMinimumRenderableSize, fallback);
    }
    if (const auto* listBox = dynamic_cast<const ListBox*>(element)) {
        if (!listBox->options().empty()) {
            return std::max(kMinimumRenderableSize, static_cast<int>(listBox->options().size()));
        }
    }
    if (const auto* label = dynamic_cast<const Label*>(element)) {
        if (width.has_value() && type != "spanlabel" && type != "infolabel") {
            return wrappedTextLineCount(label->text(), *width);
        }
        const int lines = textLineCount(label->text());
        if (lines > 0) {
            return std::max(kMinimumRenderableSize, lines);
        }
        return kMinimumRenderableSize;
    }
    if (const auto* messageTable = dynamic_cast<const MessageTable*>(element)) {
        return std::max(kMinimumRenderableSize, messageTable->fitContentHeight());
    }
    return std::max(kMinimumRenderableSize, fallback);
}

[[nodiscard]] Size renderSizeFor(const GeneratedLayoutEntry& entry, Rect cellRect, const Element* element = nullptr) {
    const int availableWidth = cellRect.width - paddingLeft(entry.cellStyle) - paddingRight(entry.cellStyle) -
                               entry.relative.col - entry.marginRight;
    const int availableHeight = cellRect.height - paddingTop(entry.cellStyle) - paddingBottom(entry.cellStyle) -
                                entry.relative.row - entry.marginBottom;
    const int fallbackWidth = entry.charsSize.width > 0 ? entry.charsSize.width : entry.sourceCell.width;
    int fallbackHeight = entry.charsSize.height > 0 ? entry.charsSize.height : entry.sourceCell.height;
    if (entry.height.mode == DimensionMode::FitContent) {
        fallbackHeight = fitContentHeightFor(element, entry.type, fallbackHeight,
                                             std::max(kMinimumRenderableSize, availableWidth));
    }
    if (element == nullptr && entry.type == "label") {
        const int availableStaticWidth = cellRect.width - paddingLeft(entry.cellStyle) - entry.relative.col;
        return Size{
            std::max(kMinimumRenderableSize,
                     std::min(std::max(kMinimumRenderableSize, fallbackWidth),
                              std::max(kMinimumRenderableSize, availableStaticWidth))),
            dimensionWithin(entry.height, availableHeight, fallbackHeight),
        };
    }
    return Size{
        dimensionWithin(entry.width, availableWidth, fallbackWidth),
        dimensionWithin(entry.height, availableHeight, fallbackHeight),
    };
}

[[nodiscard]] bool hasExpandedEntryBeforeInSourceRow(const std::vector<GeneratedLayoutEntry>& layout,
                                                    const GeneratedLayoutEntry& entry) {
    for (const GeneratedLayoutEntry& other : layout) {
        if (!sameSourceCell(other, entry)) {
            continue;
        }
        if (other.relative.row != entry.relative.row) {
            continue;
        }
        if (other.relative.col >= entry.relative.col) {
            continue;
        }
        if (other.width.mode == DimensionMode::Expanded) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] std::pair<int, int> entrySourceColSpan(const GeneratedLayoutEntry& entry) {
    const int width = std::max(kMinimumRenderableSize, entry.charsSize.width > 0 ? entry.charsSize.width : entry.sourceCell.width);
    return {entry.relative.col, entry.relative.col + width};
}

[[nodiscard]] bool sourceSpansOverlap(std::pair<int, int> first, std::pair<int, int> second) {
    return first.first < second.second && second.first < first.second;
}

[[nodiscard]] bool hasExpandedEntryBeforeInSourceColumn(const std::vector<GeneratedLayoutEntry>& layout,
                                                       const GeneratedLayoutEntry& entry) {
    const std::pair<int, int> entrySpan = entrySourceColSpan(entry);
    for (const GeneratedLayoutEntry& other : layout) {
        if (!sameSourceCell(other, entry)) {
            continue;
        }
        if (other.relative.row >= entry.relative.row) {
            continue;
        }
        if (!sourceSpansOverlap(entrySourceColSpan(other), entrySpan)) {
            continue;
        }
        if (other.height.mode == DimensionMode::Expanded) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] int renderColFor(const std::vector<GeneratedLayoutEntry>& layout,
                               const GeneratedLayoutEntry& entry,
                               Rect cellRect,
                               Size size) {
    const int baseCol = paddingLeft(entry.cellStyle) + entry.relative.col;
    const int cellWidthDelta = std::max(0, cellRect.width - entry.cellCharsSize.width);
    const bool followsExpandedEntry = hasExpandedEntryBeforeInSourceRow(layout, entry);
    if (entry.marginRight == 0 &&
        entry.cellWidth.mode == DimensionMode::Expanded &&
        entry.cellCharsSize.width > 0 &&
        cellRect.width > entry.cellCharsSize.width &&
        entry.width.mode != DimensionMode::Expanded) {
        return cellRect.col + std::max(
            baseCol,
            cellRect.width - size.width - entry.marginRight - paddingRight(entry.cellStyle));
    }
    if (followsExpandedEntry &&
        entry.cellWidth.mode == DimensionMode::Expanded &&
        entry.cellCharsSize.width > 0 &&
        cellWidthDelta > 0 &&
        entry.width.mode != DimensionMode::Expanded) {
        return cellRect.col + std::max(baseCol, baseCol + cellWidthDelta);
    }
    return cellRect.col + baseCol;
}

[[nodiscard]] int renderRowFor(const std::vector<GeneratedLayoutEntry>& layout,
                               const GeneratedLayoutEntry& entry,
                               Rect cellRect,
                               Size) {
    const int baseRow = paddingTop(entry.cellStyle) + entry.relative.row;
    const int cellHeightDelta = std::max(0, cellRect.height - entry.cellCharsSize.height);
    const bool followsExpandedEntry = hasExpandedEntryBeforeInSourceColumn(layout, entry);
    if (followsExpandedEntry &&
        entry.cellHeight.mode == DimensionMode::Expanded &&
        entry.cellCharsSize.height > 0 &&
        cellHeightDelta > 0 &&
        entry.height.mode != DimensionMode::Expanded) {
        return cellRect.row + std::max(baseRow, baseRow + cellHeightDelta);
    }
    return cellRect.row + baseRow;
}

[[nodiscard]] Rect cellContentClip(const GeneratedLayoutEntry& entry, Rect cellRect) {
    const int top = paddingTop(entry.cellStyle);
    const int right = paddingRight(entry.cellStyle);
    const int bottom = paddingBottom(entry.cellStyle);
    const int left = paddingLeft(entry.cellStyle);
    return Rect{
        cellRect.row + top,
        cellRect.col + left,
        std::max(0, cellRect.width - left - right),
        std::max(0, cellRect.height - top - bottom),
    };
}

[[nodiscard]] Rect scrollViewViewportClip(Rect frame, const Style& style) {
    const int left = std::min(paddingLeft(style), std::max(0, frame.width - kMinimumRenderableSize));
    const int right = std::min(paddingRight(style), std::max(0, frame.width - left - kMinimumRenderableSize));
    const int top = std::min(paddingTop(style), std::max(0, frame.height - kMinimumRenderableSize));
    const int bottom = std::min(paddingBottom(style), std::max(0, frame.height - top - kMinimumRenderableSize));
    return Rect{
        frame.row + top,
        frame.col + left,
        std::max(0, frame.width - left - right),
        std::max(0, frame.height - top - bottom),
    };
}

[[nodiscard]] std::optional<Color> inferredScrollIndicatorForeground(
    const TerminalBuffer& buffer,
    int row,
    int startCol,
    int endCol
) {
    if (row < 0 || row >= buffer.height()) {
        return std::nullopt;
    }
    std::optional<Color> foreground;
    const int start = std::max(0, startCol);
    const int end = std::min(buffer.width() - kMinimumRenderableSize, endCol);
    for (int scanCol = start; scanCol <= end; ++scanCol) {
        const TerminalCell& scanned = buffer.cell(row, scanCol);
        if (!transparentColor(scanned.foreground)) {
            foreground = scanned.foreground;
        }
    }
    return foreground;
}

void applyScrollIndicator(TerminalBuffer& buffer, int row, int col, std::string_view indicator, int foregroundStartCol) {
    if (row < 0 || row >= buffer.height() || col < 0 || col >= buffer.width()) {
        return;
    }
    TerminalCell cell = buffer.cell(row, col);
    cell.text = std::string{indicator};
    if (transparentColor(cell.foreground)) {
        cell.foreground = inferredScrollIndicatorForeground(buffer, row, foregroundStartCol, col);
    }
    buffer.setCell(row, col, std::move(cell));
}

[[nodiscard]] bool cellHasRenderedContent(const TerminalCell& cell) {
    return !cell.raw.empty() ||
           (cell.text != " " &&
            cell.text != kScrollIndicatorAbove &&
            cell.text != kScrollIndicatorBelow);
}

[[nodiscard]] std::optional<std::pair<int, int>> rowRenderedContentBounds(const TerminalBuffer& buffer,
                                                                          int row,
                                                                          int startCol,
                                                                          int endCol) {
    if (row < 0 || row >= buffer.height()) {
        return std::nullopt;
    }
    const int start = std::max(0, startCol);
    const int end = std::min(buffer.width(), endCol);
    std::optional<int> first;
    std::optional<int> last;
    for (int col = start; col < end; ++col) {
        if (cellHasRenderedContent(buffer.cell(row, col))) {
            if (!first.has_value()) {
                first = col;
            }
            last = col;
        }
    }
    if (first.has_value() && last.has_value()) {
        return std::make_pair(*first, *last);
    }
    return std::nullopt;
}

void dimOutsideActiveScrollView(TerminalBuffer& buffer,
                                const ScrollView* activeScrollView,
                                std::optional<Color> dimBackgroundOverride = std::nullopt) {
    if (activeScrollView == nullptr ||
        (!dimBackgroundOverride.has_value() && !activeScrollView->style().scopeDimBackground.has_value())) {
        return;
    }
    const Rect active = activeScrollView->frame();
    const Color dim = dimBackgroundOverride.value_or(*activeScrollView->style().scopeDimBackground);
    for (int row = 0; row < buffer.height(); ++row) {
        for (int col = 0; col < buffer.width(); ++col) {
            if (active.contains(Point{row, col})) {
                continue;
            }
            TerminalCell cell = buffer.cell(row, col);
            if (cell.text == kImageHalfBlockGlyph) {
                cell.background = Color{std::string{kImageHalfBlockDimBackground}};
                buffer.setCell(row, col, std::move(cell));
                continue;
            }
            if (dim.rgba().has_value() && cell.background.has_value()) {
                cell.background = dim.blendOver(*cell.background);
            } else {
                cell.background = dim;
            }
            buffer.setCell(row, col, std::move(cell));
        }
    }
}

void applyActiveScrollViewFocusBackgroundGaps(TerminalBuffer& buffer,
                                              const ScrollView* activeScrollView,
                                              Rect active,
                                              const std::optional<Color>& baseBackground,
                                              std::optional<Color> focusBackgroundOverride = std::nullopt,
                                              std::optional<Color> fallbackBackground = std::nullopt,
                                              const std::vector<Rect>& excludedRects = {},
                                              bool doubleBlendBaseBackground = true,
                                              bool exactAlphaBlend = false) {
    if (activeScrollView == nullptr ||
        (!focusBackgroundOverride.has_value() &&
         (!activeScrollView->focusStyle().has_value() ||
          !activeScrollView->focusStyle()->background.has_value()))) {
        return;
    }
    const Color focusBackground = focusBackgroundOverride.has_value()
        ? *focusBackgroundOverride
        : *activeScrollView->focusStyle()->background;
    std::optional<Color> effectiveBaseBackground = baseBackground;
    if ((!effectiveBaseBackground.has_value() ||
         (effectiveBaseBackground->rgba().has_value() && effectiveBaseBackground->rgba()->alpha < 255)) &&
        active.row >= 0 && active.row < buffer.height() && active.col >= 0 && active.col < buffer.width()) {
        effectiveBaseBackground = buffer.cell(active.row, active.col).background;
    }
    std::optional<Color> focusedBaseBackground;
    const auto blend = [exactAlphaBlend](const Color& color, const Color& background) {
        return exactAlphaBlend ? blendOverExactAlpha(color, background) : blendOverStyleAlpha(color, background);
    };
    if (effectiveBaseBackground.has_value()) {
        if (focusBackground.rgba().has_value()) {
            focusedBaseBackground = blend(focusBackground, *effectiveBaseBackground);
        } else {
            focusedBaseBackground = focusBackground;
        }
    }
    const auto blendFocus = [&focusBackground, &blend](std::optional<Color> background) -> Color {
        if (focusBackground.rgba().has_value() && background.has_value()) {
            return blend(focusBackground, *background);
        }
        return focusBackground;
    };
    const bool transparentViewportBackground =
        baseBackground.has_value() &&
        baseBackground->rgba().has_value() &&
        baseBackground->rgba()->alpha < 255;
    for (int row = std::max(0, active.row); row < std::min(buffer.height(), active.row + active.height); ++row) {
        std::optional<int> protectedContentStartCol;
        std::optional<int> protectedContentEndCol;
        if (transparentViewportBackground) {
            std::optional<std::pair<int, int>> contentBounds =
                rowRenderedContentBounds(buffer, row, active.col, active.col + active.width);
            if (contentBounds.has_value()) {
                protectedContentStartCol = std::clamp(
                    active.col + stylePaddingLeft(activeScrollView->style()),
                    active.col,
                    active.col + active.width);
                protectedContentEndCol = std::clamp(
                    active.col + active.width - stylePaddingRight(activeScrollView->style()) - 1,
                    active.col - 1,
                    active.col + active.width - 1);
            }
        }
        for (int col = std::max(0, active.col); col < std::min(buffer.width(), active.col + active.width); ++col) {
            const auto excluded = std::find_if(
                excludedRects.begin(),
                excludedRects.end(),
                [row, col](const Rect& rect) {
                    return rect.contains(Point{row, col});
                });
            if (excluded != excludedRects.end()) {
                continue;
            }
            TerminalCell cell = buffer.cell(row, col);
            if (protectedContentStartCol.has_value() &&
                protectedContentEndCol.has_value() &&
                col >= *protectedContentStartCol &&
                col <= *protectedContentEndCol) {
                continue;
            }
            const bool matchesBaseBackground =
                !effectiveBaseBackground.has_value() ||
                !cell.background.has_value() ||
                *cell.background == *effectiveBaseBackground;
            const bool matchesFallbackBackground =
                fallbackBackground.has_value() &&
                cell.background.has_value() &&
                *cell.background == *fallbackBackground;
            const bool matchesFocusedBaseBackground =
                focusedBaseBackground.has_value() &&
                cell.background.has_value() &&
                *cell.background == *focusedBaseBackground;
            if (!matchesBaseBackground && !matchesFallbackBackground && !matchesFocusedBaseBackground) {
                continue;
            }
            if (matchesFocusedBaseBackground) {
                continue;
            }
            cell.background = blendFocus(cell.background);
            if (doubleBlendBaseBackground &&
                (matchesBaseBackground || matchesFallbackBackground)) {
                cell.background = blendFocus(cell.background);
            }
            buffer.setCell(row, col, std::move(cell));
        }
    }
}

[[nodiscard]] std::optional<Rect> layoutFrameForElement(
    GeneratedWindowBase& window,
    Element* element,
    const std::vector<ResolvedRuntimeCell>& resolvedCells
) {
    if (element == nullptr) {
        return std::nullopt;
    }
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        if (entry.name.empty() || findElement(window, entry.name) != element) {
            continue;
        }
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, entry);
        if (cell == nullptr) {
            return std::nullopt;
        }
        const Size size = renderSizeFor(entry, cell->rect, element);
        const int row = renderRowFor(window.generatedLayout(), entry, cell->rect, size);
        const int col = renderColFor(window.generatedLayout(), entry, cell->rect, size);
        return Rect{row, col, size.width, size.height};
    }
    return std::nullopt;
}

void applyActiveScrollViewFocusBackgroundForLayout(
    TerminalBuffer& buffer,
    GeneratedWindowBase& window,
    const std::vector<ResolvedRuntimeCell>& resolvedCells,
    ScrollView* activeScrollView,
    const std::optional<Color>& baseBackground,
    std::optional<Color> focusBackgroundOverride = std::nullopt
) {
    if (activeScrollView == nullptr) {
        return;
    }
    const std::optional<Rect> localFrame = layoutFrameForElement(window, activeScrollView, resolvedCells);
    Rect frame = localFrame.value_or(activeScrollView->frame());
    const Rect renderedFrame = activeScrollView->frame();
    frame.width = std::max(frame.width, renderedFrame.width);
    frame.height = std::max(frame.height, renderedFrame.height);
    applyActiveScrollViewFocusBackgroundGaps(
        buffer,
        activeScrollView,
        frame,
        baseBackground,
        focusBackgroundOverride);
}

[[nodiscard]] std::optional<Color> activeScrollViewBaseBackground(
    GeneratedWindowBase& renderWindow,
    ScrollView* activeScrollView,
    const Style& renderStyle
) {
    if (activeScrollView == nullptr) {
        return renderStyle.background;
    }
    GeneratedWindowBase* owner = ownerWindowForElement(renderWindow, activeScrollView);
    if (owner != nullptr && owner != &renderWindow) {
        return owner->generatedWindowStyle().background.has_value()
            ? owner->generatedWindowStyle().background
            : renderStyle.background;
    }
    return renderStyle.background;
}

[[nodiscard]] bool elementExtendsBeyondReusableFrame(const Element* element, Rect origin, Size size) {
    if (element == nullptr) {
        return false;
    }
    const Rect frame = element->frame();
    const int localRight = frame.col - origin.col + frame.width;
    const int localBottom = frame.row - origin.row + frame.height;
    return localRight > size.width || localBottom > size.height;
}

[[nodiscard]] bool scrollViewByLines(ScrollView& scrollView, int delta) {
    if (delta == 0) {
        return false;
    }
    const Size size{scrollView.frame().width, scrollView.frame().height};
    bool changed = false;
    const std::string_view key = delta < 0 ? "Up" : "Down";
    const int steps = delta < 0 ? -delta : delta;
    for (int step = 0; step < steps; ++step) {
        changed = scrollView.handleKey(key, size) || changed;
    }
    return changed;
}

[[nodiscard]] bool ensureElementFrameVisibleInScrollView(ScrollView& scrollView, const Element* element) {
    if (element == nullptr) {
        return false;
    }
    const Rect viewport = scrollViewViewportClip(scrollView.frame(), scrollView.style());
    if (viewport.width <= 0 || viewport.height <= 0) {
        return false;
    }
    const Rect target = element->frame();
    int delta = 0;
    if (target.row < viewport.row) {
        delta = target.row - viewport.row;
    } else if (target.row + target.height > viewport.row + viewport.height) {
        delta = target.row + target.height - viewport.row - viewport.height;
    }
    return scrollViewByLines(scrollView, delta);
}

void renderEntryCellStyle(TerminalBuffer& buffer, const GeneratedLayoutEntry& entry, Rect cellRect) {
    if (!entry.cellStyle.background.has_value() &&
        !entry.cellStyle.color.has_value() &&
        !entry.cellStyle.backgroundTexture.has_value()) {
        return;
    }
    fillRect(buffer, cellRect, entry.cellStyle);
}

void renderEntry(GeneratedWindowBase& window, TerminalBuffer& buffer, const GeneratedLayoutEntry& entry,
                 Element* focusedElement, bool editMode, Rect cellRect, Element* activeScrollView = nullptr,
                 Element* activeScrollViewEditElement = nullptr,
                 std::optional<int> renderClipTop = std::nullopt,
                 std::optional<int> renderClipBottom = std::nullopt,
                 std::optional<Color> activeScrollViewFocusBackgroundOverride = std::nullopt) {
    Element* element = entry.name.empty() ? nullptr : findElement(window, entry.name);
    if (element == nullptr) {
        if (!entry.name.empty()) {
            std::string text = entry.name;
            if (text.size() >= 2 && text.front() == '"' && text.back() == '"') {
                text = text.substr(1, text.size() - 2);
            }
            const Size size = renderSizeFor(entry, cellRect);
            const int row = renderRowFor(window.generatedLayout(), entry, cellRect, size);
            if (row >= buffer.height()) {
                return;
            }
            const int col = renderColFor(window.generatedLayout(), entry, cellRect, size);
            if (col >= buffer.width()) {
                return;
            }
            blitClipped(buffer, renderPlainText(text, size.width, size.height, entry.elementStyle),
                        row, col, cellContentClip(entry, cellRect));
        }
        return;
    }

    const bool activeScrollViewFocused = editMode &&
        (activeScrollView == element ||
         (dynamic_cast<ScrollView*>(element) != nullptr &&
          (scrollViewContainingElement(window, focusedElement) == element ||
           scrollViewContainingElement(window, activeScrollViewEditElement) == element)));
    const bool focused = element == focusedElement;
    const bool scrollViewSelfFocused = dynamic_cast<ScrollView*>(element) != nullptr && focused && !activeScrollViewFocused;
    Size size = renderSizeFor(entry, cellRect, element);
    const bool elementEditActive = element == focusedElement && editMode &&
        (activeScrollView == nullptr || activeScrollViewEditElement == element);
    int row = renderRowFor(window.generatedLayout(), entry, cellRect, size);
    if (elementEditActive) {
        if (auto* comboBox = dynamic_cast<ComboBox*>(element)) {
            const int dropdownRows = kComboBoxClosedRows + static_cast<int>(comboBox->options().size());
            const int visibleRows = std::min(kComboBoxDropdownRows, dropdownRows);
            size.height = std::min(visibleRows, std::max(size.height, buffer.height() - row));
            row = renderRowFor(window.generatedLayout(), entry, cellRect, size);
        }
    }
    if (row >= buffer.height()) {
        return;
    }
    const int col = renderColFor(window.generatedLayout(), entry, cellRect, size);
    if (col >= buffer.width()) {
        return;
    }
    std::optional<int> elementClipTop;
    std::optional<int> elementClipBottom;
    if (renderClipTop.has_value() || renderClipBottom.has_value()) {
        const int clipTop = renderClipTop.value_or(row);
        const int clipBottom = renderClipBottom.value_or(row + size.height);
        const int visibleTop = std::max(row, clipTop);
        const int visibleBottom = std::min(row + size.height, clipBottom);
        elementClipTop = std::max(0, visibleTop - row);
        elementClipBottom = std::max(0, visibleBottom - row);
    }
    element->setFrame(Rect{row, col, size.width, size.height});
    const std::optional<Color> parentBackground =
        row >= 0 && row < buffer.height() && col >= 0 && col < buffer.width()
            ? buffer.cell(row, col).background
            : std::nullopt;
    ScopedElementParentBackground scopedParentBackground(parentBackground);
    RenderedContent content;
    std::optional<Size> reusableChildRenderSize;
    if (auto* reusable = dynamic_cast<ReusableElement*>(element);
        reusable != nullptr && reusable->child() != nullptr) {
        int childFocusedIndex = focusedElement != nullptr
            ? focusIndexForElement(*reusable->child(), focusedElement, activeScrollView)
            : -1;
        if (childFocusedIndex < 0 &&
            activeScrollViewEditElement != nullptr &&
            ownerWindowForElement(*reusable->child(), activeScrollViewEditElement) != nullptr) {
            childFocusedIndex = focusIndexForElement(*reusable->child(), activeScrollViewEditElement, activeScrollView);
        }
        if (childFocusedIndex < 0 &&
            activeScrollView != nullptr &&
            ownerWindowForElement(*reusable->child(), activeScrollView) != nullptr) {
            childFocusedIndex = focusIndexForElement(*reusable->child(), activeScrollView, activeScrollView);
        }
        const bool childOwnsActiveScrollView =
            activeScrollView != nullptr &&
            ownerWindowForElement(*reusable->child(), activeScrollView) != nullptr;
        const bool childOwnsActiveScrollViewEditElement =
            activeScrollViewEditElement != nullptr &&
            ownerWindowForElement(*reusable->child(), activeScrollViewEditElement) != nullptr;
        const bool childHasFocusedElement =
            focusedElement != nullptr && ownerWindowForElement(*reusable->child(), focusedElement) != nullptr;
        const bool reusableDescendantFocused =
            focused ||
            (editMode && childOwnsActiveScrollView) ||
            (editMode && childOwnsActiveScrollViewEditElement) ||
            (editMode && childHasFocusedElement);
        const bool childEditMode = childFocusedIndex >= 0 && editMode &&
            (activeScrollView == nullptr ||
             childOwnsActiveScrollView ||
             childOwnsActiveScrollViewEditElement);
        std::optional<Style> childWindowStyle;
        std::optional<Color> childActiveScrollViewFocusBackground;
        bool applyChildDescendantFocusBackground = true;
        Size childRenderSize = size;
        ScrollView* reusableGeneratedScrollView = generatedScrollViewForReusable(*reusable);
        const Size childContentSizeForWidth = generatedWindowContentSizeForWidth(
            *reusable->child(),
            std::max(kMinimumRenderableSize, childRenderSize.width));
        childRenderSize.height = std::max(childRenderSize.height, childContentSizeForWidth.height);
        if (reusableDescendantFocused) {
            childWindowStyle = reusable->child()->generatedWindowStyle();
            const bool applyReusableFocusStyle = reusableFocusStyleAppliesToChild(
                *reusable,
                focused,
                reusableDescendantFocused && !focused);
            if (applyReusableFocusStyle) {
                if (reusableGeneratedScrollView == nullptr) {
                    childWindowStyle->merge(*reusable->focusStyle());
                }
                if (reusable->focusStyle()->background.has_value()) {
                    childActiveScrollViewFocusBackground = reusable->focusStyle()->background;
                }
            }
            if (!childActiveScrollViewFocusBackground.has_value() && !childOwnsActiveScrollView) {
                if (ScrollView* childScrollView = reusableGeneratedScrollView) {
                    if (childScrollView->descendantFocusStyle().has_value() &&
                        childScrollView->descendantFocusStyle()->background.has_value()) {
                        childActiveScrollViewFocusBackground =
                            childScrollView->descendantFocusStyle()->background;
                    } else if (childScrollView->focusStyle().has_value() &&
                               childScrollView->focusStyle()->background.has_value()) {
                        childActiveScrollViewFocusBackground =
                            childScrollView->focusStyle()->background;
                    }
                    applyChildDescendantFocusBackground =
                        childOwnsActiveScrollView ||
                        childOwnsActiveScrollViewEditElement ||
                        (focusedElement != nullptr &&
                         ownerWindowForElement(*reusable->child(), focusedElement) != nullptr &&
                         focusedElement != childScrollView);
                }
            }
        }
        reusableChildRenderSize = childRenderSize;
        content = renderGeneratedWindowContent(
            *reusable->child(),
            childRenderSize,
            childFocusedIndex,
            childEditMode,
            activeScrollView,
            activeScrollViewEditElement,
            childWindowStyle.has_value() ? &*childWindowStyle : nullptr,
            childActiveScrollViewFocusBackground,
            elementClipTop,
            elementClipBottom,
            false);
        if (childActiveScrollViewFocusBackground.has_value() && reusableGeneratedScrollView == nullptr) {
            if (applyChildDescendantFocusBackground) {
                std::vector<Color> descendantBackgrounds;
                collectDescendantBaseStyleBackgrounds(*reusable->child(), descendantBackgrounds);
                applyReusableFocusBackgroundToDescendantBackgrounds(
                    content,
                    *childActiveScrollViewFocusBackground,
                    descendantBackgrounds);
            }
            applyReusableFocusBackground(
                content,
                *childActiveScrollViewFocusBackground,
                reusableFocusBaseBackground(*reusable, entry));
        }
    } else {
        const bool elementEditMode = focused && editMode &&
            (activeScrollView == nullptr || activeScrollViewEditElement == element);
        content = renderElement(
            *element,
            size,
            ElementRenderState{
                .focused = focused && !scrollViewSelfFocused && !activeScrollViewFocused,
                .editMode = elementEditMode && !activeScrollViewFocused,
                .passiveFocus =
                    !scrollViewSelfFocused &&
                    !activeScrollViewFocused &&
                    activeScrollView != nullptr &&
                    focused &&
                    !elementEditMode,
                .clipTop = elementClipTop,
                .clipBottom = elementClipBottom,
            });
    }
    Rect clip = cellContentClip(entry, cellRect);
    if (dynamic_cast<ComboBox*>(element) != nullptr && elementEditActive && !content.empty()) {
        const int clipBottom = std::max(clip.row + clip.height, row + static_cast<int>(content.size()));
        clip.height = std::max(0, clipBottom - clip.row);
    }
    if (auto* reusable = dynamic_cast<ReusableElement*>(element);
        reusable != nullptr && reusable->child() != nullptr &&
        editMode && dynamic_cast<ComboBox*>(focusedElement) != nullptr &&
        ownerWindowForElement(*reusable->child(), focusedElement) != nullptr) {
        const Size childRenderSize = reusableChildRenderSize.value_or(size);
        const int clipBottom = std::max(clip.row + clip.height, row + childRenderSize.height);
        const int clipRight = std::max(clip.col + clip.width, col + childRenderSize.width);
        clip.height = std::max(0, clipBottom - clip.row);
        clip.width = std::max(0, clipRight - clip.col);
    }
    if (auto* reusable = dynamic_cast<ReusableElement*>(element);
        reusable != nullptr && reusable->child() != nullptr && editMode &&
        ((activeScrollView != nullptr && ownerWindowForElement(*reusable->child(), activeScrollView) != nullptr) ||
         (activeScrollViewEditElement != nullptr &&
          ownerWindowForElement(*reusable->child(), activeScrollViewEditElement) != nullptr))) {
        const Size childRenderSize = reusableChildRenderSize.value_or(size);
        const int clipBottom = std::max(clip.row + clip.height, row + childRenderSize.height);
        const int clipRight = std::max(clip.col + clip.width, col + childRenderSize.width);
        clip.height = std::max(0, clipBottom - clip.row);
        clip.width = std::max(0, clipRight - clip.col);
    }
    if (dynamic_cast<ScrollView*>(element) != nullptr) {
        const Size scrollViewRenderSize = size;
        const int clipBottom = std::max(clip.row + clip.height, row + scrollViewRenderSize.height);
        const int clipRight = std::max(clip.col + clip.width, col + scrollViewRenderSize.width);
        clip.height = std::max(0, clipBottom - clip.row);
        clip.width = std::max(0, clipRight - clip.col);
    }
    blitClipped(buffer, content, row, col, clip);
    if (auto* reusable = dynamic_cast<ReusableElement*>(element);
        reusable != nullptr && reusable->child() != nullptr) {
        const bool childOwnsActiveScrollView =
            activeScrollView != nullptr &&
            ownerWindowForElement(*reusable->child(), activeScrollView) != nullptr;
        const bool childOwnsActiveScrollViewEditElement =
            activeScrollViewEditElement != nullptr &&
            ownerWindowForElement(*reusable->child(), activeScrollViewEditElement) != nullptr;
        const bool reusableDescendantFocused =
            focused ||
            (editMode && childOwnsActiveScrollView) ||
            (editMode && childOwnsActiveScrollViewEditElement) ||
            (editMode &&
             focusedElement != nullptr &&
             ownerWindowForElement(*reusable->child(), focusedElement) != nullptr);
        const bool applyReusableFocusStyle = reusableFocusStyleAppliesToChild(
            *reusable,
            focused,
            reusableDescendantFocused && !focused);
        if (reusableDescendantFocused &&
            applyReusableFocusStyle &&
            generatedScrollViewForReusable(*reusable) == nullptr &&
            reusable->focusStyle()->background.has_value()) {
            applyReusableFocusBackgroundToBuffer(
                buffer,
                Rect{row, col, size.width, size.height},
                *reusable->focusStyle()->background,
                reusableFocusBaseBackground(*reusable, entry));
        }
    }
    if (auto* reusable = dynamic_cast<ReusableElement*>(element);
        reusable != nullptr && reusable->child() != nullptr) {
        offsetWindowElementFrames(*reusable->child(), Rect{row, col, size.width, size.height});
    }
    if (auto* scrollView = dynamic_cast<ScrollView*>(element)) {
        const Size scrollViewRenderSize = size;
        const Rect scrollViewport = scrollViewViewportClip(
            Rect{row, col, scrollViewRenderSize.width, scrollViewRenderSize.height},
            scrollView->style());
        const Rect scrollClip = scrollViewport;
        struct ScrollViewOverlay {
            Element* element;
            Rect frame;
            int height;
        };
        std::vector<ScrollViewOverlay> overlays;
        std::vector<Rect> renderedChildRects;
        for (const ScrollViewChildView& childView : scrollView->childViews(scrollViewRenderSize)) {
            if (!childView.visible) {
                continue;
            }
            const Rect childFrame{
                row + childView.frame.row,
                col + childView.frame.col,
                childView.frame.width,
                childView.frame.height,
            };
            if (childView.element == nullptr) {
                renderedChildRects.push_back(childFrame);
                continue;
            }
            childView.element->setFrame(childFrame);
            if (auto* reusable = dynamic_cast<ReusableElement*>(childView.element);
                reusable != nullptr && reusable->child() != nullptr) {
                const int childFocusedIndex = focusedElement != nullptr
                    ? focusIndexForElement(*reusable->child(), focusedElement, activeScrollView)
                    : -1;
                int effectiveChildFocusedIndex = childFocusedIndex;
                if (effectiveChildFocusedIndex < 0 &&
                    activeScrollViewEditElement != nullptr &&
                    ownerWindowForElement(*reusable->child(), activeScrollViewEditElement) != nullptr) {
                    effectiveChildFocusedIndex = focusIndexForElement(
                        *reusable->child(),
                        activeScrollViewEditElement,
                        activeScrollView);
                }
                if (effectiveChildFocusedIndex < 0 &&
                    activeScrollView != nullptr &&
                    ownerWindowForElement(*reusable->child(), activeScrollView) != nullptr) {
                    effectiveChildFocusedIndex = focusIndexForElement(
                        *reusable->child(),
                        activeScrollView,
                        activeScrollView);
                }
                ComboBox* activeChildComboBox = dynamic_cast<ComboBox*>(activeScrollViewEditElement);
                const bool childOwnsActiveScrollView =
                    activeScrollView != nullptr &&
                    ownerWindowForElement(*reusable->child(), activeScrollView) != nullptr;
                const bool childOwnsActiveScrollViewEditElement =
                    activeScrollViewEditElement != nullptr &&
                    ownerWindowForElement(*reusable->child(), activeScrollViewEditElement) != nullptr;
                const bool overlayActiveChildComboBox = childOwnsActiveScrollViewEditElement && activeChildComboBox != nullptr;
                const bool childHasFocusedElement =
                    focusedElement != nullptr && ownerWindowForElement(*reusable->child(), focusedElement) != nullptr;
                const bool reusableDescendantFocused =
                    childView.element == focusedElement ||
                    (editMode &&
                     (childFocusedIndex >= 0 ||
                      effectiveChildFocusedIndex >= 0 ||
                      childOwnsActiveScrollViewEditElement ||
                      childHasFocusedElement ||
                      childOwnsActiveScrollView));
                const bool reusableWholeChildFocus =
                    reusableDescendantFocused &&
                    !hasMultipleFocusableElements(*reusable->child(), activeScrollView);
                const bool childEditMode = effectiveChildFocusedIndex >= 0 && editMode &&
                    (activeScrollView == nullptr ||
                     childOwnsActiveScrollView ||
                     (activeScrollViewEditElement != nullptr &&
                      ownerWindowForElement(*reusable->child(), activeScrollViewEditElement) != nullptr));
                std::optional<Style> childWindowStyle;
                std::optional<Color> childActiveScrollViewFocusBackground;
                std::optional<Color> childDescendantFocusBackground;
                ScrollView* reusableGeneratedScrollView = generatedScrollViewForReusable(*reusable);
                if (reusableWholeChildFocus) {
                    childWindowStyle = reusable->child()->generatedWindowStyle();
                    const bool reusableDirectFocus = childView.element == focusedElement;
                    const bool applyReusableFocusStyle = reusableFocusStyleAppliesToChild(
                        *reusable,
                        reusableDirectFocus,
                        reusableDescendantFocused && !reusableDirectFocus);
                    if (applyReusableFocusStyle) {
                        if (reusableGeneratedScrollView == nullptr) {
                            childWindowStyle->merge(*reusable->focusStyle());
                        }
                        if (reusable->focusStyle()->background.has_value()) {
                            childActiveScrollViewFocusBackground = reusable->focusStyle()->background;
                        }
                    }
                    if (!childActiveScrollViewFocusBackground.has_value()) {
                        const auto* scopedScrollView = dynamic_cast<const ScrollView*>(activeScrollView);
                        if (scopedScrollView != nullptr &&
                            scopedScrollView->descendantFocusStyle().has_value() &&
                            scopedScrollView->descendantFocusStyle()->background.has_value()) {
                            childActiveScrollViewFocusBackground =
                                scopedScrollView->descendantFocusStyle()->background;
                        }
                    }
                    if (!childActiveScrollViewFocusBackground.has_value() &&
                        scrollView->descendantFocusStyle().has_value() &&
                        scrollView->descendantFocusStyle()->background.has_value()) {
                        childActiveScrollViewFocusBackground =
                            scrollView->descendantFocusStyle()->background;
                    }
                    if (!childActiveScrollViewFocusBackground.has_value() &&
                        scrollView->focusStyle().has_value() &&
                        scrollView->focusStyle()->background.has_value()) {
                        childActiveScrollViewFocusBackground =
                            scrollView->focusStyle()->background;
                    }
                    childDescendantFocusBackground = childActiveScrollViewFocusBackground;
                }
                const bool activeScrollViewDescendantFocused =
                    activeScrollView == scrollView &&
                    focusedElement != nullptr &&
                    focusedElement != scrollView &&
                    scrollViewContainingElement(window, focusedElement) == scrollView;
                if (!childDescendantFocusBackground.has_value() && activeScrollViewDescendantFocused && reusableWholeChildFocus) {
                    if (scrollView->descendantFocusStyle().has_value() &&
                        scrollView->descendantFocusStyle()->background.has_value()) {
                        childDescendantFocusBackground =
                            scrollView->descendantFocusStyle()->background;
                    }
                    else if (scrollView->focusStyle().has_value() &&
                             scrollView->focusStyle()->background.has_value()) {
                        childDescendantFocusBackground =
                            scrollView->focusStyle()->background;
                    }
                }
                std::optional<int> childClipTop;
                std::optional<int> childClipBottom;
                if (childView.clipped) {
                    childClipTop = std::max(0, scrollClip.row - childFrame.row);
                    childClipBottom = std::max(0, std::min(childFrame.height, scrollClip.row + scrollClip.height - childFrame.row));
                }
                RenderedContent reusableChildContent = renderGeneratedWindowContent(
                    *reusable->child(), Size{childFrame.width, childFrame.height},
                    effectiveChildFocusedIndex, childEditMode, activeScrollView,
                    activeScrollViewEditElement,
                    childWindowStyle.has_value() ? &*childWindowStyle : nullptr,
                    childActiveScrollViewFocusBackground,
                    childClipTop,
                    childClipBottom,
                    false);
                if (childDescendantFocusBackground.has_value()) {
                    if (reusableGeneratedScrollView == nullptr) {
                        std::vector<Color> descendantBackgrounds;
                        collectDescendantBaseStyleBackgrounds(*reusable->child(), descendantBackgrounds);
                        applyReusableFocusBackgroundToDescendantBackgrounds(
                            reusableChildContent,
                            *childDescendantFocusBackground,
                            descendantBackgrounds);
                        if (reusableWholeChildFocus &&
                            scrollView->selectedStyle().has_value() &&
                            scrollView->selectedStyle()->background.has_value()) {
                            applySelectedBackgroundToFocusedReusableChild(
                                reusableChildContent,
                                *scrollView->selectedStyle()->background,
                                childDescendantFocusBackground,
                                descendantBackgrounds);
                        }
                    }
                }
                if (childActiveScrollViewFocusBackground.has_value() && reusableGeneratedScrollView == nullptr) {
                    applyReusableFocusBackground(reusableChildContent, *childActiveScrollViewFocusBackground,
                                                 reusable->style().background);
                }
                blitClipped(buffer, reusableChildContent, childFrame.row, childFrame.col, scrollClip);
                offsetWindowElementFrames(*reusable->child(), childFrame);
                if (overlayActiveChildComboBox) {
                    Rect comboFrame = activeChildComboBox->frame();
                    const int comboRenderHeight = std::max(
                        comboFrame.height,
                        scrollClip.row + scrollClip.height - comboFrame.row);
                    overlays.push_back(ScrollViewOverlay{
                        activeChildComboBox,
                        comboFrame,
                        comboRenderHeight,
                    });
                }
                continue;
            }
            const bool childFocused = childView.element == focusedElement;
            const bool childEditMode = childFocused && editMode &&
                (activeScrollView == nullptr || activeScrollViewEditElement == childView.element);
            std::optional<int> childClipTop;
            std::optional<int> childClipBottom;
            if (childView.clipped) {
                childClipTop = std::max(0, scrollClip.row - childFrame.row);
                childClipBottom = std::max(0, std::min(childFrame.height, scrollClip.row + scrollClip.height - childFrame.row));
            }
            blitClipped(
                buffer,
                renderElement(*childView.element, Size{childFrame.width, childFrame.height},
                              ElementRenderState{
                                  .focused = childFocused,
                                  .editMode = childEditMode,
                                  .passiveFocus = activeScrollView != nullptr && childFocused && !childEditMode,
                                  .clipTop = childClipTop,
                                  .clipBottom = childClipBottom,
                              }),
                childFrame.row,
                childFrame.col,
                scrollClip
            );
            if (childEditMode && dynamic_cast<ComboBox*>(childView.element) != nullptr) {
                const int comboRenderHeight = std::max(
                    childFrame.height,
                    scrollClip.row + scrollClip.height - childFrame.row);
                overlays.push_back(ScrollViewOverlay{
                    childView.element,
                    childFrame,
                    comboRenderHeight,
                });
            }
        }
        for (const ScrollViewOverlay& overlay : overlays) {
            overlay.element->setFrame(Rect{
                overlay.frame.row,
                overlay.frame.col,
                overlay.frame.width,
                overlay.height,
            });
            blitClipped(
                buffer,
                renderElement(*overlay.element, Size{overlay.frame.width, overlay.height},
                              ElementRenderState{.focused = true, .editMode = true}),
                overlay.frame.row,
                overlay.frame.col,
                scrollClip
            );
        }
        if (scrollClip.width > 0 && scrollClip.height > 0) {
            const Size viewportSize{scrollViewport.width, scrollViewport.height};
            const int indicatorCol = scrollViewport.col + scrollViewport.width - kMinimumRenderableSize;
            if (scrollView->viewOffset() < scrollView->maxViewOffset(viewportSize)) {
                applyScrollIndicator(buffer, scrollViewport.row, indicatorCol, kScrollIndicatorAbove, scrollViewport.col);
            }
            if (scrollView->viewOffset() > 0) {
                applyScrollIndicator(buffer, scrollViewport.row + scrollViewport.height - kMinimumRenderableSize,
                                     indicatorCol, kScrollIndicatorBelow, scrollViewport.col);
            }
        }
        const bool generatedScrollViewProxyFocused =
            activeScrollViewFocusBackgroundOverride.has_value() &&
            window.generatedScrollView() == scrollView &&
            !activeScrollViewFocused &&
            !scrollViewSelfFocused;
        if (activeScrollViewFocused || generatedScrollViewProxyFocused) {
            const std::optional<Color> scrollViewBaseBackground = scrollView->style().background.has_value()
                ? scrollView->style().background
                : (parentBackground.has_value() ? parentBackground : entry.cellStyle.background);
            applyActiveScrollViewFocusBackgroundGaps(
                buffer,
                scrollView,
                Rect{row, col, scrollViewRenderSize.width, scrollViewRenderSize.height},
                scrollViewBaseBackground,
                activeScrollViewFocusBackgroundOverride,
                parentBackground.has_value() ? parentBackground : entry.cellStyle.background,
                {},
                false,
                true);
        } else if (scrollViewSelfFocused) {
            const std::optional<Color> scrollViewBaseBackground = scrollView->style().background.has_value()
                ? scrollView->style().background
                : (parentBackground.has_value() ? parentBackground : entry.cellStyle.background);
            applyActiveScrollViewFocusBackgroundGaps(
                buffer,
                scrollView,
                Rect{row, col, scrollViewRenderSize.width, scrollViewRenderSize.height},
                scrollViewBaseBackground,
                activeScrollViewFocusBackgroundOverride,
                parentBackground.has_value() ? parentBackground : entry.cellStyle.background,
                {},
                false,
                true);
        }
    }
}

[[nodiscard]] bool isFocusedComboBoxEntry(GeneratedWindowBase& window, const GeneratedLayoutEntry& entry,
                                          const Element* focusedElement, bool editMode) {
    return editMode && focusedElement != nullptr && findElement(window, entry.name) == focusedElement &&
           dynamic_cast<const ComboBox*>(focusedElement) != nullptr;
}

[[nodiscard]] bool handleElementKey(Element& element, std::string_view key) {
    if (auto* checkbox = dynamic_cast<CheckBox*>(&element)) {
        return checkbox->handleKey(key);
    }
    if (auto* textInput = dynamic_cast<TextInput*>(&element)) {
        return textInput->handleKey(key);
    }
    if (auto* numberInput = dynamic_cast<NumberInput*>(&element)) {
        return numberInput->handleKey(key);
    }
    if (auto* comboBox = dynamic_cast<ComboBox*>(&element)) {
        return comboBox->handleKey(key);
    }
    if (auto* listBox = dynamic_cast<ListBox*>(&element)) {
        return listBox->handleKey(key);
    }
    if (auto* scrollView = dynamic_cast<ScrollView*>(&element)) {
        return scrollView->handleKey(key, Size{scrollView->frame().width, scrollView->frame().height});
    }
    return false;
}

[[nodiscard]] bool isButton(const Element& element) {
    return element.enabled() && dynamic_cast<const Button*>(&element) != nullptr;
}

[[nodiscard]] bool isClickableImage(const Element& element) {
    return element.enabled() && dynamic_cast<const Image*>(&element) != nullptr;
}

[[nodiscard]] bool activateReusableControl(Element* element) {
    auto* reusable = dynamic_cast<ReusableElement*>(element);
    return reusable != nullptr &&
           reusable->enabled() &&
           reusable->child() != nullptr &&
           reusable->child()->generatedFocusable() &&
           reusable->child()->activateGeneratedControl();
}

[[nodiscard]] bool isImmediateInput(const Element& element) {
    return element.enabled() && dynamic_cast<const CheckBox*>(&element) != nullptr;
}

[[nodiscard]] bool isEditableElement(const Element& element) {
    if (const auto* reusable = dynamic_cast<const ReusableElement*>(&element);
        reusable != nullptr && reusable->child() != nullptr &&
        reusable->child()->generatedScrollView() != nullptr) {
        return element.enabled();
    }
    return element.enabled() &&
           (dynamic_cast<const TextInput*>(&element) != nullptr ||
           dynamic_cast<const NumberInput*>(&element) != nullptr ||
           dynamic_cast<const ComboBox*>(&element) != nullptr ||
           dynamic_cast<const ListBox*>(&element) != nullptr ||
           dynamic_cast<const ScrollView*>(&element) != nullptr);
}

[[nodiscard]] bool usesLeaveCommit(const Element* element) {
    return element != nullptr && element->commitMode() == kCommitModeLeave;
}

[[nodiscard]] int indexOfElement(const std::vector<Element*>& elements, const Element* target) {
    for (std::size_t index = 0; index < elements.size(); ++index) {
        if (elements[index] == target) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

[[nodiscard]] Rect mouseHitFrame(const Element& element) {
    Rect frame = element.frame();
    if (dynamic_cast<const ComboBox*>(&element) != nullptr) {
        frame.height = kComboBoxClosedRows;
    }
    return frame;
}

[[nodiscard]] Element* elementAt(const std::vector<Element*>& elements, Point position) {
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        if (mouseHitFrame(**it).contains(position)) {
            return *it;
        }
    }
    return nullptr;
}

[[nodiscard]] Element* mouseTargetElement(const std::vector<Element*>& elements, Element* focusedElement,
                                          bool editMode, Point position, Element* activeEditElement = nullptr) {
    Element* dropdownElement = activeEditElement != nullptr ? activeEditElement : focusedElement;
    if (editMode) {
        if (auto* comboBox = dynamic_cast<ComboBox*>(dropdownElement)) {
            Rect dropdownFrame = comboBox->frame();
            dropdownFrame.height = std::max(
                dropdownFrame.height,
                std::min(kComboBoxDropdownRows, kComboBoxClosedRows + static_cast<int>(comboBox->options().size())));
            if (dropdownFrame.contains(position)) {
                return dropdownElement;
            }
        }
    }
    return elementAt(elements, position);
}

[[nodiscard]] int comboBoxOptionIndexForLocalRow(const ComboBox& comboBox, int localRow, int height) {
    (void)height;
    return std::max(0, std::min(static_cast<int>(comboBox.options().size()) - 1, localRow - 1));
}

void ensureElementVisibleInParentScrollView(Element* element) {
    Element* child = element;
    while (child != nullptr && child->parent() != nullptr) {
        Element* parent = child->parent();
        if (auto* scrollView = dynamic_cast<ScrollView*>(parent)) {
            (void)ensureElementFrameVisibleInScrollView(*scrollView, child);
        }
        child = parent;
    }
}

void moveFocus(int& focusedIndex, const std::vector<Element*>& focusable, int delta) {
    if (focusable.empty()) {
        focusedIndex = -1;
        return;
    }
    const int count = static_cast<int>(focusable.size());
    focusedIndex = (focusedIndex + delta + count) % count;
    ensureElementVisibleInParentScrollView(focusable[static_cast<std::size_t>(focusedIndex)]);
}

[[nodiscard]] bool isArrowKey(std::string_view key) {
    return key == "Up" || key == "Down" || key == "Left" || key == "Right";
}

[[nodiscard]] int centerRow(const Rect& rect) {
    return rect.row + rect.height / 2;
}

[[nodiscard]] int centerCol(const Rect& rect) {
    return rect.col + rect.width / 2;
}

[[nodiscard]] bool axisOverlaps(int startA, int endA, int startB, int endB) {
    return std::max(startA, startB) < std::min(endA, endB);
}

[[nodiscard]] int axisGap(int startA, int endA, int startB, int endB) {
    if (endA <= startB) {
        return startB - endA;
    }
    if (endB <= startA) {
        return startA - endB;
    }
    return 0;
}

[[nodiscard]] std::optional<int> bandScanRank(int currentStart, int currentEnd, int candidateStart, int candidateEnd) {
    if (axisOverlaps(currentStart, currentEnd, candidateStart, candidateEnd)) {
        return 0;
    }

    const int maxDistance =
        std::max({
            std::abs(candidateStart - currentStart),
            std::abs(candidateEnd - currentEnd),
            std::abs(candidateStart - currentEnd),
            std::abs(candidateEnd - currentStart),
        }) +
        std::max(1, currentEnd - currentStart) +
        std::max(1, candidateEnd - candidateStart);

    int rank = 1;
    for (int distance = 1; distance <= maxDistance; ++distance) {
        const int offsets[] = {distance, -distance};
        for (const int offset : offsets) {
            if (axisOverlaps(currentStart + offset, currentEnd + offset, candidateStart, candidateEnd)) {
                return rank;
            }
            ++rank;
        }
    }
    return std::nullopt;
}

void moveFocusSpatial(int& focusedIndex, const std::vector<Element*>& focusable, std::string_view key) {
    if (focusable.empty()) {
        focusedIndex = -1;
        return;
    }
    if (focusedIndex < 0 || focusedIndex >= static_cast<int>(focusable.size())) {
        focusedIndex = 0;
        return;
    }

    const Rect current = focusable[static_cast<std::size_t>(focusedIndex)]->frame();
    const int currentRow = centerRow(current);
    const int currentCol = centerCol(current);
    int bestIndex = -1;
    int bestBand = 0;
    int bestPrimary = 0;
    int bestGap = 0;
    int bestCenter = 0;

    for (std::size_t index = 0; index < focusable.size(); ++index) {
        if (static_cast<int>(index) == focusedIndex) {
            continue;
        }
        const Rect candidate = focusable[index]->frame();
        int primary = 0;
        int gap = 0;
        int edge = 0;
        std::optional<int> band;
        if (key == "Up" && centerRow(candidate) < currentRow) {
            if (candidate.row + candidate.height > current.row) {
                continue;
            }
            primary = std::max(0, current.row - (candidate.row + candidate.height));
            band = bandScanRank(current.col, current.col + current.width, candidate.col, candidate.col + candidate.width);
            gap = axisGap(current.col, current.col + current.width, candidate.col, candidate.col + candidate.width);
            edge = std::abs(candidate.col - current.col);
        } else if (key == "Down" && centerRow(candidate) > currentRow) {
            if (candidate.row < current.row + current.height) {
                continue;
            }
            primary = std::max(0, candidate.row - (current.row + current.height));
            band = bandScanRank(current.col, current.col + current.width, candidate.col, candidate.col + candidate.width);
            gap = axisGap(current.col, current.col + current.width, candidate.col, candidate.col + candidate.width);
            edge = std::abs(candidate.col - current.col);
        } else if (key == "Right" && centerCol(candidate) > currentCol) {
            if (candidate.col < current.col + current.width) {
                continue;
            }
            primary = std::max(0, candidate.col - (current.col + current.width));
            if (!axisOverlaps(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height)) {
                continue;
            }
            band = bandScanRank(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height);
            gap = axisGap(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height);
            edge = std::abs(candidate.row - current.row);
        } else if (key == "Left" && centerCol(candidate) < currentCol) {
            if (candidate.col + candidate.width > current.col) {
                continue;
            }
            primary = std::max(0, current.col - (candidate.col + candidate.width));
            if (!axisOverlaps(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height)) {
                continue;
            }
            band = bandScanRank(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height);
            gap = axisGap(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height);
            edge = std::abs(candidate.row - current.row);
        } else {
            continue;
        }

        if (!band.has_value()) {
            continue;
        }

        const auto score = std::tuple{*band, primary, gap, edge};
        const auto bestScore = std::tuple{bestBand, bestPrimary, bestGap, bestCenter};
        if (bestIndex < 0 || score < bestScore) {
            bestIndex = static_cast<int>(index);
            bestBand = *band;
            bestPrimary = primary;
            bestGap = gap;
            bestCenter = edge;
        }
    }

    if (bestIndex >= 0) {
        focusedIndex = bestIndex;
        ensureElementVisibleInParentScrollView(focusable[static_cast<std::size_t>(focusedIndex)]);
    }
}

[[nodiscard]] std::vector<Element*> scrollViewScopeElements(GeneratedWindowBase& window, ScrollView& scrollView) {
    const std::vector<Element*> activeFocusable = focusableElements(window, &scrollView);
    Element* scopeRoot = generatedScrollViewProxyFor(window, &scrollView);
    if (scopeRoot == nullptr) {
        scopeRoot = &scrollView;
    }
    const int scrollIndex = indexOfElement(activeFocusable, scopeRoot);
    if (scrollIndex < 0) {
        return {};
    }

    Element* nextNavigationElement = nullptr;
    const std::vector<Element*> navigationFocusable = focusableElements(window, nullptr);
    const int navigationIndex = indexOfElement(navigationFocusable, scopeRoot);
    if (navigationIndex >= 0 && navigationIndex + 1 < static_cast<int>(navigationFocusable.size())) {
        nextNavigationElement = navigationFocusable[static_cast<std::size_t>(navigationIndex + 1)];
    }

    std::vector<Element*> scoped;
    for (std::size_t index = static_cast<std::size_t>(scrollIndex); index < activeFocusable.size(); ++index) {
        Element* element = activeFocusable[index];
        if (index != static_cast<std::size_t>(scrollIndex) && nextNavigationElement != nullptr && element == nextNavigationElement) {
            break;
        }
        scoped.push_back(element);
    }
    return scoped;
}

[[nodiscard]] Element* firstFocusableDescendantInScrollView(GeneratedWindowBase& window,
                                                            ScrollView& scrollView,
                                                            const std::unordered_map<Element*, Element*>& remembered) {
    std::vector<Element*> scoped = scrollViewScopeElements(window, scrollView);
    Element* scopeRoot = generatedScrollViewProxyFor(window, &scrollView);
    const auto rememberedIt = remembered.find(&scrollView);
    if (rememberedIt != remembered.end() && indexOfElement(scoped, rememberedIt->second) >= 0) {
        return rememberedIt->second;
    }
    for (Element* element : scoped) {
        if (element != &scrollView && element != scopeRoot) {
            return element;
        }
    }
    return nullptr;
}

[[nodiscard]] bool enterScrollViewScope(GeneratedWindowBase& window,
                                        int& focusedIndex,
                                        bool& editMode,
                                        Element*& activeScrollView,
                                        std::unordered_map<Element*, Element*>& remembered) {
    std::vector<Element*> navigationFocusable = focusableElements(window, nullptr);
    Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(navigationFocusable.size()))
        ? navigationFocusable[static_cast<std::size_t>(focusedIndex)]
        : nullptr;
    auto* scrollView = dynamic_cast<ScrollView*>(focused);
    if (scrollView == nullptr) {
        if (auto* reusable = dynamic_cast<ReusableElement*>(focused);
            reusable != nullptr && reusable->child() != nullptr) {
            scrollView = generatedScrollViewForReusable(*reusable);
        }
    }
    if (scrollView == nullptr) {
        return false;
    }
    activeScrollView = scrollView;
    editMode = true;
    Element* target = firstFocusableDescendantInScrollView(window, *scrollView, remembered);
    std::vector<Element*> scoped = focusableElements(window, activeScrollView);
    if (target != nullptr) {
        focusedIndex = indexOfElement(scoped, target);
        remembered[scrollView] = target;
        (void)ensureElementFrameVisibleInScrollView(*scrollView, target);
    } else {
        Element* scopeRoot = generatedScrollViewProxyFor(window, scrollView);
        focusedIndex = indexOfElement(scoped, scopeRoot == nullptr ? scrollView : scopeRoot);
    }
    return true;
}

void exitScrollViewScope(GeneratedWindowBase& window,
                         int& focusedIndex,
                         bool& editMode,
                         Element*& activeScrollView,
                         std::unordered_map<Element*, Element*>& remembered) {
    if (activeScrollView == nullptr) {
        editMode = false;
        return;
    }
    auto* activeScroll = dynamic_cast<ScrollView*>(activeScrollView);
    std::vector<Element*> activeFocusable = focusableElements(window, activeScrollView);
    std::vector<Element*> scoped = activeScroll == nullptr ? std::vector<Element*>{} : scrollViewScopeElements(window, *activeScroll);
    if (focusedIndex >= 0 && focusedIndex < static_cast<int>(activeFocusable.size())) {
        Element* focused = activeFocusable[static_cast<std::size_t>(focusedIndex)];
        if (focused != activeScrollView && indexOfElement(scoped, focused) >= 0) {
            remembered[activeScrollView] = focused;
        }
    }
    Element* scrollView = activeScrollView;
    activeScrollView = nullptr;
    editMode = false;
    Element* scopeRoot = generatedScrollViewProxyFor(window, dynamic_cast<ScrollView*>(scrollView));
    focusedIndex = indexOfElement(focusableElements(window, nullptr), scopeRoot == nullptr ? scrollView : scopeRoot);
}

[[nodiscard]] bool moveScrollViewScopeFocus(GeneratedWindowBase& window,
                                            int& focusedIndex,
                                            Element* activeScrollView,
                                            std::unordered_map<Element*, Element*>& remembered,
                                            std::string_view key) {
    auto* scrollView = dynamic_cast<ScrollView*>(activeScrollView);
    if (scrollView == nullptr) {
        return false;
    }
    std::vector<Element*> activeFocusable = focusableElements(window, activeScrollView);
    Element* current = (focusedIndex >= 0 && focusedIndex < static_cast<int>(activeFocusable.size()))
        ? activeFocusable[static_cast<std::size_t>(focusedIndex)]
        : activeScrollView;
    std::vector<Element*> scoped = scrollViewScopeElements(window, *scrollView);
    Element* scopeRoot = generatedScrollViewProxyFor(window, scrollView);
    scoped.erase(
        std::remove_if(scoped.begin(), scoped.end(), [activeScrollView, scopeRoot](Element* element) {
            return element == activeScrollView || element == scopeRoot;
        }),
        scoped.end()
    );
    if (scoped.empty()) {
        if (key == "Up" || key == "Down") {
            return scrollView->handleKey(key, Size{scrollView->frame().width, scrollView->frame().height});
        }
        return false;
    }
    int scopedIndex = indexOfElement(scoped, current);
    if (scopedIndex < 0) {
        scopedIndex = -1;
    }
    const int before = scopedIndex;
    moveFocusSpatial(scopedIndex, scoped, key);
    if (scopedIndex != before && scopedIndex >= 0 && scopedIndex < static_cast<int>(scoped.size())) {
        Element* focused = scoped[static_cast<std::size_t>(scopedIndex)];
        focusedIndex = indexOfElement(activeFocusable, focused);
        if (focused != activeScrollView) {
            remembered[activeScrollView] = focused;
        }
        (void)ensureElementFrameVisibleInScrollView(*scrollView, focused);
        return true;
    }
    if (key == "Up") {
        if (!scrollView->handleKey("Up", Size{scrollView->frame().width, scrollView->frame().height})) {
            return false;
        }
    }
    else if (key == "Down") {
        if (!scrollView->handleKey("Down", Size{scrollView->frame().width, scrollView->frame().height})) {
            return false;
        }
    }
    else {
        return false;
    }

    activeFocusable = focusableElements(window, activeScrollView);
    scoped = scrollViewScopeElements(window, *scrollView);
    scoped.erase(
        std::remove_if(scoped.begin(), scoped.end(), [activeScrollView, scopeRoot](Element* element) {
            return element == activeScrollView || element == scopeRoot;
        }),
        scoped.end()
    );
    if (scoped.empty()) {
        return true;
    }
    int nextIndex = indexOfElement(scoped, current);
    if (nextIndex >= 0) {
        const int beforeScrollMove = nextIndex;
        moveFocusSpatial(nextIndex, scoped, key);
        if (nextIndex == beforeScrollMove) {
            return true;
        }
    }
    else {
        nextIndex = key == "Down" ? 0 : static_cast<int>(scoped.size()) - 1;
    }
    if (nextIndex >= 0 && nextIndex < static_cast<int>(scoped.size())) {
        Element* focused = scoped[static_cast<std::size_t>(nextIndex)];
        focusedIndex = indexOfElement(activeFocusable, focused);
        if (focused != activeScrollView) {
            remembered[activeScrollView] = focused;
        }
        (void)ensureElementFrameVisibleInScrollView(*scrollView, focused);
    }
    return true;
}

[[nodiscard]] Size terminalSize(TerminalBackend& backend) {
    return backend.size().value_or(Size{kFallbackTerminalWidth, kFallbackTerminalHeight});
}

void writeFrame(
    TerminalBackend& backend,
    TerminalBuffer& buffer,
    const std::optional<Rect>& viewport = std::nullopt,
    const std::optional<Rect>& paintBounds = std::nullopt,
    const std::optional<ScrollRegionHint>& scrollRegion = std::nullopt
) {
    std::string scrollOutput;
    if (!viewport.has_value() && !paintBounds.has_value() && scrollRegion.has_value() &&
        scrollRegion->rect.col == 0 && scrollRegion->rect.width >= buffer.width()) {
        scrollOutput = buffer.renderScrollRegion(0, scrollRegion->rect.row, scrollRegion->rect.height,
                                                 scrollRegion->delta);
    }
    if (viewport.has_value()) {
        if (paintBounds.has_value()) {
            backend.write(buffer.renderDiffRegion(
                viewport->row,
                viewport->col,
                paintBounds->row,
                paintBounds->col,
                paintBounds->height,
                paintBounds->width
            ));
            return;
        }
        backend.write(buffer.renderDiff(viewport->row, viewport->col));
        return;
    }
    backend.write(scrollOutput + buffer.renderDiff());
}

[[nodiscard]] EditSnapshot captureSnapshot(Element* element) {
    EditSnapshot snapshot{.element = element};
    if (auto* textInput = dynamic_cast<TextInput*>(element)) {
        snapshot.textValue = textInput->value();
        snapshot.textCursor = textInput->cursor();
    } else if (auto* numberInput = dynamic_cast<NumberInput*>(element)) {
        snapshot.numberValue = numberInput->value();
        numberInput->beginEdit();
    } else if (auto* comboBox = dynamic_cast<ComboBox*>(element)) {
        snapshot.selectedIndex = comboBox->selectedIndex();
    } else if (auto* listBox = dynamic_cast<ListBox*>(element)) {
        snapshot.selectedIndex = listBox->selectedIndex();
    }
    return snapshot;
}

void beginElementEdit(Element* element) {
    if (auto* textInput = dynamic_cast<TextInput*>(element)) {
        textInput->setCursor(static_cast<int>(textInput->value().size()));
    } else if (auto* numberInput = dynamic_cast<NumberInput*>(element)) {
        numberInput->beginEdit();
    }
}

void restoreSnapshot(const EditSnapshot& snapshot) {
    if (auto* textInput = dynamic_cast<TextInput*>(snapshot.element)) {
        textInput->setValue(snapshot.textValue);
        textInput->setCursor(snapshot.textCursor);
    } else if (auto* numberInput = dynamic_cast<NumberInput*>(snapshot.element)) {
        numberInput->cancelEdit();
        numberInput->setValue(snapshot.numberValue);
    } else if (auto* comboBox = dynamic_cast<ComboBox*>(snapshot.element)) {
        comboBox->setSelectedIndex(snapshot.selectedIndex);
    } else if (auto* listBox = dynamic_cast<ListBox*>(snapshot.element)) {
        listBox->setSelectedIndex(snapshot.selectedIndex);
    }
}

void commitEdit(Element* element) {
    if (auto* numberInput = dynamic_cast<NumberInput*>(element)) {
        numberInput->commitEdit();
    }
}

[[nodiscard]] bool closeComboBoxOnOutsideClick(Element* focusedElement, bool& editMode,
                                               std::optional<EditSnapshot>& editSnapshot, Element* target) {
    if (editMode && dynamic_cast<ComboBox*>(focusedElement) != nullptr && target == nullptr) {
        commitEdit(focusedElement);
        editSnapshot.reset();
        editMode = false;
        return true;
    }
    return false;
}

void applyEditEscape(std::optional<EditSnapshot>& editSnapshot, bool& editMode) {
    if (editSnapshot.has_value()) {
        restoreSnapshot(*editSnapshot);
        editSnapshot.reset();
    }
    editMode = false;
}

void escapeElementEdit(Element* element, std::optional<EditSnapshot>& editSnapshot, bool& editMode) {
    if (usesLeaveCommit(element)) {
        commitEdit(element);
        editSnapshot.reset();
        editMode = false;
        return;
    }
    applyEditEscape(editSnapshot, editMode);
}

[[nodiscard]] bool pointOutsideActiveScrollView(Element* activeScrollView, Point position) {
    auto* scrollView = dynamic_cast<ScrollView*>(activeScrollView);
    return scrollView != nullptr && !scrollView->frame().contains(position);
}

void renderNotification(TerminalBuffer& buffer, std::string_view message) {
    if (message.empty() || buffer.width() <= 0 || buffer.height() <= 0) {
        return;
    }
    Style style;
    style.color = Color{"#ffffff"};
    style.background = Color{"#2255bb"};
    const int width = static_cast<int>(message.size()) + 2;
    const int row = 0;
    const int col = std::max(0, buffer.width() - width - 1);
    std::string text = " " + std::string(message) + " ";
    for (std::size_t index = 0; index < text.size() && col + static_cast<int>(index) < buffer.width(); ++index) {
        buffer.setCell(row, col + static_cast<int>(index), styledTextCell(text[index], style));
    }
}

[[nodiscard]] bool pasteIntoFocused(Element* focused, std::string_view text) {
    if (auto* textInput = dynamic_cast<TextInput*>(focused)) {
        textInput->insertText(text);
        return true;
    }
    if (auto* numberInput = dynamic_cast<NumberInput*>(focused)) {
        for (char ch : text) {
            if (ch == '\r' || ch == '\n') {
                continue;
            }
            (void)numberInput->handleKey(std::string(1, ch));
        }
        return true;
    }
    return false;
}

[[nodiscard]] bool copyFocusedText(Element* focused) {
    if (auto* textInput = dynamic_cast<TextInput*>(focused)) {
        const std::string selectedText = textInput->selectedText();
        const std::string& text = selectedText.empty() ? textInput->value() : selectedText;
        return !text.empty() && copyTextToClipboard(text);
    }
    return false;
}

[[nodiscard]] bool isSelectionElement(const Element* element) {
    return dynamic_cast<const ComboBox*>(element) != nullptr ||
           dynamic_cast<const ListBox*>(element) != nullptr;
}

[[nodiscard]] int selectedIndexOf(const Element* element) {
    if (const auto* comboBox = dynamic_cast<const ComboBox*>(element)) {
        return comboBox->selectedIndex();
    }
    if (const auto* listBox = dynamic_cast<const ListBox*>(element)) {
        return listBox->selectedIndex();
    }
    return -1;
}

[[nodiscard]] std::vector<std::string> selectedValuesOf(const Element* element) {
    if (const auto* comboBox = dynamic_cast<const ComboBox*>(element)) {
        const auto& choices = comboBox->options();
        if (!choices.empty()) {
            return {choices[static_cast<std::size_t>(comboBox->selectedIndex())]};
        }
    }
    if (const auto* listBox = dynamic_cast<const ListBox*>(element)) {
        return listBox->selectedValues();
    }
    return {};
}

[[nodiscard]] std::string textValueOf(const Element* element) {
    if (const auto* textInput = dynamic_cast<const TextInput*>(element)) {
        return textInput->value();
    }
    if (const auto* numberInput = dynamic_cast<const NumberInput*>(element)) {
        return numberInput->displayValue();
    }
    if (const auto* checkbox = dynamic_cast<const CheckBox*>(element)) {
        return checkbox->checked() ? "True" : "False";
    }
    if (const auto* comboBox = dynamic_cast<const ComboBox*>(element)) {
        const auto values = selectedValuesOf(comboBox);
        return values.empty() ? std::string{} : values.front();
    }
    if (const auto* listBox = dynamic_cast<const ListBox*>(element)) {
        const auto values = selectedValuesOf(listBox);
        return values.empty() ? std::string{} : values.front();
    }
    return {};
}

void notifyTextChanged(const GeneratedWindowRuntimeOptions& options, Element* element) {
    if (!options.onTextChanged) {
        return;
    }
    if (dynamic_cast<TextInput*>(element) != nullptr ||
        dynamic_cast<NumberInput*>(element) != nullptr ||
        dynamic_cast<CheckBox*>(element) != nullptr) {
        options.onTextChanged(element->name(), textValueOf(element));
    }
}

void notifyTextConfirmed(const GeneratedWindowRuntimeOptions& options, Element* element) {
    if (!options.onTextConfirmed) {
        return;
    }
    if (dynamic_cast<TextInput*>(element) != nullptr ||
        dynamic_cast<NumberInput*>(element) != nullptr ||
        dynamic_cast<ComboBox*>(element) != nullptr ||
        dynamic_cast<ListBox*>(element) != nullptr) {
        options.onTextConfirmed(element->name(), textValueOf(element));
    }
}

void notifySelectionChanged(const GeneratedWindowRuntimeOptions& options, Element* element, int previousIndex) {
    if (!isSelectionElement(element)) {
        return;
    }
    if (selectedIndexOf(element) == previousIndex) {
        return;
    }
    if (!options.onSelectionChanged) {
        return;
    }
    const auto selectedValues = selectedValuesOf(element);
    options.onSelectionChanged(element->name(), selectedValues);
}

void notifyEditStarted(const GeneratedWindowRuntimeOptions& options, Element* element) {
    if (options.onEditStarted && element != nullptr) {
        options.onEditStarted(element->name());
    }
}

void notifyEditStarted(const GeneratedWindowFrameOptions& options, Element* element) {
    if (options.onEditStarted && element != nullptr) {
        options.onEditStarted(element->name());
    }
}

void notifyFocusChanged(const GeneratedWindowRuntimeOptions& options, Element* element, bool focused) {
    if (options.onFocusChanged && element != nullptr) {
        options.onFocusChanged(element->name(), focused);
    }
}

void notifyFocusChanged(const GeneratedWindowFrameOptions& options, Element* element, bool focused) {
    if (options.onFocusChanged && element != nullptr) {
        options.onFocusChanged(element->name(), focused);
    }
}

void notifyOwnerAwareButton(GeneratedWindowBase& window,
                            const GeneratedWindowRuntimeOptions& options,
                            Element* element) {
    if (element == nullptr) {
        return;
    }
    if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
        owner != nullptr && owner != &window) {
        (void)owner->handleGeneratedButton(element->name());
        return;
    }
    if (options.onButton) {
        options.onButton(element->name());
    }
}

void notifyOwnerAwareTextChanged(GeneratedWindowBase& window,
                                 const GeneratedWindowRuntimeOptions& options,
                                 Element* element) {
    if (element == nullptr) {
        return;
    }
    if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
        owner != nullptr && owner != &window) {
        (void)owner->handleGeneratedTextChanged(element->name(), textValueOf(element));
        return;
    }
    notifyTextChanged(options, element);
}

void notifyOwnerAwareTextConfirmed(GeneratedWindowBase& window,
                                   const GeneratedWindowRuntimeOptions& options,
                                   Element* element) {
    if (element == nullptr) {
        return;
    }
    if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
        owner != nullptr && owner != &window) {
        (void)owner->handleGeneratedTextConfirmed(element->name(), textValueOf(element));
        return;
    }
    notifyTextConfirmed(options, element);
}

void notifyOwnerAwareSelectionChanged(GeneratedWindowBase& window,
                                      const GeneratedWindowRuntimeOptions& options,
                                      Element* element,
                                      int previousIndex) {
    if (element == nullptr || !isSelectionElement(element) || selectedIndexOf(element) == previousIndex) {
        return;
    }
    if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
        owner != nullptr && owner != &window) {
        (void)owner->handleGeneratedSelectionChanged(element->name(), selectedValuesOf(element));
        return;
    }
    notifySelectionChanged(options, element, previousIndex);
}

void notifyOwnerAwareSelectionChanged(GeneratedWindowBase& window,
                                      const GeneratedWindowRuntimeOptions& options,
                                      Element* element,
                                      const std::vector<std::string>& previousValues) {
    if (element == nullptr || !isSelectionElement(element)) {
        return;
    }
    const auto currentValues = selectedValuesOf(element);
    if (currentValues == previousValues) {
        return;
    }
    if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
        owner != nullptr && owner != &window) {
        (void)owner->handleGeneratedSelectionChanged(element->name(), currentValues);
        return;
    }
    if (options.onSelectionChanged) {
        options.onSelectionChanged(element->name(), currentValues);
    }
}

void notifyOwnerAwareValueChangedAfterHandledKey(GeneratedWindowBase& window,
                                                 const GeneratedWindowRuntimeOptions& options,
                                                 Element* element,
                                                 int previousSelectionIndex) {
    if (isSelectionElement(element)) {
        notifyOwnerAwareSelectionChanged(window, options, element, previousSelectionIndex);
        return;
    }
    notifyOwnerAwareTextChanged(window, options, element);
}

[[nodiscard]] GeneratedWindowRuntimeOptions runtimeOptionsForFrame(const GeneratedWindowFrameOptions& frameOptions) {
    GeneratedWindowRuntimeOptions options;
    options.onButton = frameOptions.onButton;
    options.onMousePressBeforeFocused = frameOptions.onMousePressBeforeFocused;
    options.onMouseWheelBeforeFocused = frameOptions.onMouseWheelBeforeFocused;
    options.onTextChanged = frameOptions.onTextChanged;
    options.onTextConfirmed = frameOptions.onTextConfirmed;
    options.onSelectionChanged = frameOptions.onSelectionChanged;
    options.onEditStarted = frameOptions.onEditStarted;
    options.onFocusChanged = frameOptions.onFocusChanged;
    return options;
}

bool focusElement(int& focusedIndex, const std::vector<Element*>& focusable, Element* element) {
    const int index = indexOfElement(focusable, element);
    if (index >= 0) {
        const bool changed = focusedIndex != index;
        focusedIndex = index;
        ensureElementVisibleInParentScrollView(element);
        return changed;
    }
    return false;
}

[[nodiscard]] bool clearFocusForEmptyMouseTarget(GeneratedWindowBase& window,
                                                 const GeneratedWindowRuntimeOptions& options,
                                                 int& focusedIndex,
                                                 std::optional<EditSnapshot>& editSnapshot,
                                                 bool& editMode,
                                                 Element* activeScrollView,
                                                 Element*& activeScrollViewEditElement,
                                                 const std::vector<Element*>& focusable) {
    if (!hasMultipleFocusableElements(window, activeScrollView)) {
        return false;
    }
    Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
        ? focusable[static_cast<std::size_t>(focusedIndex)]
        : nullptr;
    bool changed = false;
    if (editMode) {
        Element* editElement = activeScrollViewEditElement != nullptr ? activeScrollViewEditElement : focused;
        commitEdit(editElement);
        editSnapshot.reset();
        editMode = false;
        activeScrollViewEditElement = nullptr;
        changed = true;
    }
    if (focused != nullptr) {
        focusedIndex = -1;
        notifyFocusChanged(options, focused, false);
        changed = true;
    }
    return changed;
}

[[nodiscard]] bool isStandardEscapeDialog(const GeneratedWindowBase& window, std::string_view className) {
    const std::string_view name = className.empty() ? std::string_view(window.mcpClassName()) : className;
    return name == "FileBrowser" || name.starts_with("MessageBox");
}

[[nodiscard]] std::string standardEscapeButtonName(const GeneratedWindowBase& window, std::string_view className) {
    if (!isStandardEscapeDialog(window, className)) {
        return {};
    }
    const std::string_view name = className.empty() ? std::string_view(window.mcpClassName()) : className;
    if (name == "FileBrowser") {
        return dynamic_cast<const Button*>(findElement(window, "close_btn")) != nullptr ? "close_btn" : "";
    }
    for (std::string_view buttonName : {"cancel_btn", "no_btn", "ok_btn"}) {
        if (dynamic_cast<const Button*>(findElement(window, buttonName)) != nullptr) {
            return std::string{buttonName};
        }
    }
    return {};
}

[[nodiscard]] bool handleStandardEscapeButton(GeneratedWindowBase& window,
                                              std::string_view className,
                                              int& focusedIndex,
                                              const GeneratedWindowRuntimeOptions& options,
                                              const std::function<void()>& beforeButtonAction) {
    const std::string buttonName = standardEscapeButtonName(window, className);
    if (buttonName.empty()) {
        return false;
    }
    Element* button = findElement(window, buttonName);
    if (button == nullptr) {
        return false;
    }
    std::vector<Element*> focusable = focusableElements(window);
    (void)focusElement(focusedIndex, focusable, button);
    if (beforeButtonAction) {
        beforeButtonAction();
    }
    notifyOwnerAwareButton(window, options, button);
    return true;
}

[[nodiscard]] bool handleStandardEscapeButton(GeneratedWindowStackFrame& frame,
                                              const std::function<void()>& beforeButtonAction) {
    if (frame.window == nullptr) {
        return false;
    }
    GeneratedWindowRuntimeOptions options = runtimeOptionsForFrame(frame.options);
    frame.editMode = false;
    frame.activeScrollView = nullptr;
    frame.activeScrollViewEditElement = nullptr;
    frame.editSnapshot.reset();
    return handleStandardEscapeButton(*frame.window, frame.options.className, frame.focusedIndex,
                                      options, beforeButtonAction);
}

bool handleStackFrameKey(GeneratedWindowStackFrame& frame, std::string_view key, const std::function<void()>& beforeButtonAction) {
    if (frame.window == nullptr) {
        return false;
    }
    GeneratedWindowRuntimeOptions notifyOptions = runtimeOptionsForFrame(frame.options);
    std::vector<Element*> focusable = focusableElements(*frame.window, frame.activeScrollView);
    Element* focused = (frame.focusedIndex >= 0 && frame.focusedIndex < static_cast<int>(focusable.size()))
        ? focusable[static_cast<std::size_t>(frame.focusedIndex)]
        : nullptr;
    const std::string_view focusedName = focused == nullptr ? std::string_view{} : std::string_view(focused->name());
    if (key == "Escape" && handleStandardEscapeButton(frame, beforeButtonAction)) {
        return true;
    }
    if (frame.editMode && key == "Escape") {
        if (frame.activeScrollView != nullptr) {
            if (frame.activeScrollViewEditElement != nullptr) {
                if (!usesLeaveCommit(frame.activeScrollViewEditElement)) {
                    if (frame.editSnapshot.has_value() &&
                        frame.editSnapshot->element == frame.activeScrollViewEditElement) {
                        restoreSnapshot(*frame.editSnapshot);
                    }
                } else {
                    commitEdit(frame.activeScrollViewEditElement);
                }
                frame.editSnapshot.reset();
                frame.activeScrollViewEditElement = nullptr;
                return true;
            }
            exitScrollViewScope(*frame.window, frame.focusedIndex, frame.editMode,
                                frame.activeScrollView, frame.scrollViewLastDescendant);
            return true;
        }
        escapeElementEdit(focused, frame.editSnapshot, frame.editMode);
        return true;
    }
    if (frame.options.onKeyBeforeFocusedElement && frame.options.onKeyBeforeFocusedElement(key, focusedName, frame.editMode)) {
        return true;
    }
    if (frame.options.onKeyBeforeFocused && frame.options.onKeyBeforeFocused(key)) {
        return true;
    }
    if (frame.editMode && focused != nullptr) {
        if (frame.activeScrollView != nullptr) {
            if (frame.activeScrollViewEditElement != nullptr) {
                if (key == "Enter") {
                    (void)handleElementKey(*frame.activeScrollViewEditElement, key);
                    notifyOwnerAwareTextConfirmed(*frame.window, notifyOptions, frame.activeScrollViewEditElement);
                    commitEdit(frame.activeScrollViewEditElement);
                    frame.editSnapshot.reset();
                    frame.activeScrollViewEditElement = nullptr;
                    return true;
                }
                if (isArrowKey(key)) {
                    const int previousSelectionIndex = selectedIndexOf(frame.activeScrollViewEditElement);
                    (void)handleElementKey(*frame.activeScrollViewEditElement, key);
                    notifyOwnerAwareValueChangedAfterHandledKey(*frame.window, notifyOptions,
                                                                frame.activeScrollViewEditElement,
                                                                previousSelectionIndex);
                    return true;
                }
                const int previousSelectionIndex = selectedIndexOf(frame.activeScrollViewEditElement);
                (void)handleElementKey(*frame.activeScrollViewEditElement, key);
                notifyOwnerAwareValueChangedAfterHandledKey(*frame.window, notifyOptions,
                                                            frame.activeScrollViewEditElement,
                                                            previousSelectionIndex);
                return true;
            }
            if (isArrowKey(key)) {
                (void)moveScrollViewScopeFocus(*frame.window, frame.focusedIndex, frame.activeScrollView,
                                               frame.scrollViewLastDescendant, key);
                return true;
            }
            if (key == "Enter" || key == " ") {
                focusable = focusableElements(*frame.window, frame.activeScrollView);
                focused = (frame.focusedIndex >= 0 && frame.focusedIndex < static_cast<int>(focusable.size()))
                    ? focusable[static_cast<std::size_t>(frame.focusedIndex)]
                    : nullptr;
                if (focused == nullptr) {
                    return true;
                }
                if (isButton(*focused)) {
                    if (beforeButtonAction) {
                        beforeButtonAction();
                    }
                    notifyOwnerAwareButton(*frame.window, notifyOptions, focused);
                    return true;
                }
                if (isClickableImage(*focused)) {
                    notifyOwnerAwareButton(*frame.window, notifyOptions, focused);
                    return true;
                }
                if (activateReusableControl(focused)) {
                    return true;
                }
                if (isImmediateInput(*focused)) {
                    const int previousSelectionIndex = selectedIndexOf(focused);
                    (void)handleElementKey(*focused, key);
                    notifyOwnerAwareValueChangedAfterHandledKey(*frame.window, notifyOptions, focused, previousSelectionIndex);
                    return true;
                }
                if (isEditableElement(*focused)) {
                    frame.editSnapshot = captureSnapshot(focused);
                    beginElementEdit(focused);
                    frame.activeScrollViewEditElement = focused;
                    notifyEditStarted(frame.options, focused);
                    return true;
                }
            }
            return true;
        }
        if (key == "Enter" && usesLeaveCommit(focused)) {
            const int previousSelectionIndex = selectedIndexOf(focused);
            (void)handleElementKey(*focused, key);
            notifyOwnerAwareValueChangedAfterHandledKey(*frame.window, notifyOptions, focused, previousSelectionIndex);
            frame.editSnapshot.reset();
        } else if (key == "Enter" && !usesLeaveCommit(focused)) {
            (void)handleElementKey(*focused, key);
            notifyOwnerAwareTextConfirmed(*frame.window, notifyOptions, focused);
            commitEdit(focused);
            frame.editMode = false;
            frame.editSnapshot.reset();
        } else {
            const int previousSelectionIndex = selectedIndexOf(focused);
            (void)handleElementKey(*focused, key);
            notifyOwnerAwareValueChangedAfterHandledKey(*frame.window, notifyOptions, focused, previousSelectionIndex);
        }
        return true;
    }
    if (frame.options.onKey && frame.options.onKey(key)) {
        return true;
    }
    if (key == "Tab") {
        moveFocus(frame.focusedIndex, focusable, 1);
        return true;
    }
    if (key == "Shift+Tab") {
        moveFocus(frame.focusedIndex, focusable, -1);
        return true;
    }
    if (isArrowKey(key)) {
        moveFocusSpatial(frame.focusedIndex, focusable, key);
        return true;
    }
    if (focused == nullptr) {
        return true;
    }
    if (key == "Enter" || key == " ") {
        if (isButton(*focused)) {
            if (beforeButtonAction) {
                beforeButtonAction();
            }
            notifyOwnerAwareButton(*frame.window, notifyOptions, focused);
        } else if (isClickableImage(*focused)) {
            notifyOwnerAwareButton(*frame.window, notifyOptions, focused);
        } else if (activateReusableControl(focused)) {
        } else if (isImmediateInput(*focused)) {
            const int previousSelectionIndex = selectedIndexOf(focused);
            (void)handleElementKey(*focused, key);
            notifyOwnerAwareValueChangedAfterHandledKey(*frame.window, notifyOptions, focused, previousSelectionIndex);
        } else if (isEditableElement(*focused)) {
            if (!enterScrollViewScope(*frame.window, frame.focusedIndex, frame.editMode,
                                      frame.activeScrollView, frame.scrollViewLastDescendant)) {
                frame.editSnapshot = captureSnapshot(focused);
                beginElementEdit(focused);
                frame.editMode = true;
                notifyEditStarted(frame.options, focused);
            } else {
                frame.activeScrollViewEditElement = nullptr;
                frame.editSnapshot.reset();
            }
        }
        return true;
    }
    if (isImmediateInput(*focused)) {
        const int previousSelectionIndex = selectedIndexOf(focused);
        (void)handleElementKey(*focused, key);
        notifyOwnerAwareValueChangedAfterHandledKey(*frame.window, notifyOptions, focused, previousSelectionIndex);
        return true;
    }
    return true;
}

[[nodiscard]] bool handleMousePress(GeneratedWindowBase& window,
                                    GeneratedWindowRuntimeOptions& options,
                                    int& focusedIndex,
                                    std::optional<EditSnapshot>& editSnapshot, bool& editMode,
                                    Element* activeScrollView, Element*& activeScrollViewEditElement,
                                    Element*& mouseSelectionElement, int& mouseSelectionAnchor,
                                    MouseClickCandidate* mouseClickCandidate,
                                    const std::vector<Element*>& focusable, Element* element,
                                    Point position) {
    if (element == nullptr) {
        return clearFocusForEmptyMouseTarget(window, options, focusedIndex, editSnapshot, editMode,
                                            activeScrollView, activeScrollViewEditElement, focusable);
    }
    if (!element->enabled()) {
        return false;
    }

    const bool scrollViewScopeActive = editMode && activeScrollView != nullptr;
    const bool targetEditingInScope = scrollViewScopeActive && activeScrollViewEditElement == element;
    if (scrollViewScopeActive && activeScrollViewEditElement != nullptr && activeScrollViewEditElement != element) {
        commitEdit(activeScrollViewEditElement);
        activeScrollViewEditElement = nullptr;
    }
    if (editMode && !scrollViewScopeActive && focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()) &&
        focusable[static_cast<std::size_t>(focusedIndex)] != element) {
        commitEdit(focusable[static_cast<std::size_t>(focusedIndex)]);
        editSnapshot.reset();
        editMode = false;
    }

    mouseSelectionElement = nullptr;
    mouseSelectionAnchor = 0;
    Element* previousFocused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
        ? focusable[static_cast<std::size_t>(focusedIndex)]
        : nullptr;
    if (focusElement(focusedIndex, focusable, element)) {
        notifyFocusChanged(options, previousFocused, false);
        notifyFocusChanged(options, element, true);
    }
    const Rect frame = element->frame();
    const int localRow = position.row - frame.row;
    const int localCol = position.col - frame.col;

    if (isButton(*element)) {
        notifyOwnerAwareButton(window, options, element);
        return true;
    }
    if (isClickableImage(*element)) {
        if (mouseClickCandidate != nullptr) {
            mouseClickCandidate->element = element;
            mouseClickCandidate->moved = false;
            return true;
        }
        notifyOwnerAwareButton(window, options, element);
        return true;
    }
    if (activateReusableControl(element)) {
        return true;
    }
    if (auto* checkbox = dynamic_cast<CheckBox*>(element)) {
        (void)checkbox->handleKey("Enter");
        notifyOwnerAwareTextChanged(window, options, element);
        return true;
    }
    if (auto* textInput = dynamic_cast<TextInput*>(element)) {
        if (!editMode || !editSnapshot.has_value() || editSnapshot->element != element) {
            editSnapshot = captureSnapshot(element);
        }
        editMode = true;
        if (scrollViewScopeActive) {
            activeScrollViewEditElement = element;
        }
        notifyEditStarted(options, element);
        const int cursor = textInput->cursorForPoint(localRow, localCol, Size{frame.width, frame.height});
        textInput->selectRange(cursor, cursor);
        mouseSelectionElement = element;
        mouseSelectionAnchor = cursor;
        return true;
    }
    if (auto* numberInput = dynamic_cast<NumberInput*>(element)) {
        if (!editMode || !editSnapshot.has_value() || editSnapshot->element != element) {
            editSnapshot = captureSnapshot(element);
        }
        editMode = true;
        if (scrollViewScopeActive) {
            activeScrollViewEditElement = element;
        }
        notifyEditStarted(options, element);
        numberInput->setEditCursor(localCol);
        return true;
    }
    if (auto* comboBox = dynamic_cast<ComboBox*>(element)) {
        const bool comboEditMode = scrollViewScopeActive ? targetEditingInScope : editMode;
        if (comboEditMode && localRow > 0) {
            const int previousIndex = selectedIndexOf(element);
            comboBox->setSelectedIndex(comboBoxOptionIndexForLocalRow(*comboBox, localRow, frame.height));
            commitEdit(element);
            editSnapshot.reset();
            if (scrollViewScopeActive) {
                activeScrollViewEditElement = nullptr;
            } else {
                editMode = false;
            }
            notifyOwnerAwareSelectionChanged(window, options, element, previousIndex);
            notifyOwnerAwareTextConfirmed(window, options, element);
        } else if (comboEditMode) {
            commitEdit(element);
            editSnapshot.reset();
            if (scrollViewScopeActive) {
                activeScrollViewEditElement = nullptr;
            } else {
                editMode = false;
            }
            notifyOwnerAwareTextConfirmed(window, options, element);
        } else {
            editSnapshot = captureSnapshot(element);
            editMode = true;
            if (scrollViewScopeActive) {
                activeScrollViewEditElement = element;
            }
            notifyEditStarted(options, element);
        }
        return true;
    }
    if (auto* listBox = dynamic_cast<ListBox*>(element)) {
        const int optionIndex = listBox->scrollOffset() + localRow;
        if (optionIndex < 0 || optionIndex >= static_cast<int>(listBox->options().size())) {
            return false;
        }
        const std::vector<std::string> previousValues = selectedValuesOf(element);
        if (listBox->multiple()) {
            std::vector<int> nextIndices = listBox->selectedIndices();
            const auto found = std::find(nextIndices.begin(), nextIndices.end(), optionIndex);
            if (found == nextIndices.end()) {
                nextIndices.push_back(optionIndex);
            } else {
                nextIndices.erase(found);
            }
            listBox->setSelectedIndices(std::move(nextIndices));
        } else {
            listBox->setSelectedIndex(optionIndex);
        }
        editSnapshot = captureSnapshot(element);
        editMode = true;
        if (scrollViewScopeActive) {
            activeScrollViewEditElement = element;
        }
        notifyEditStarted(options, element);
        notifyOwnerAwareSelectionChanged(window, options, element, previousValues);
        return true;
    }
    return false;
}

[[nodiscard]] bool labelAllowsTextSelection(const Label& label) {
    return label.effectiveStyle().userSelect == std::optional<std::string>{"text"};
}

void clearLabelSelectionsInWindow(GeneratedWindowBase& window) {
    for (const auto& element : window.elements()) {
        if (element == nullptr) {
            continue;
        }
        if (auto* label = dynamic_cast<Label*>(element.get())) {
            label->clearSelection();
            continue;
        }
        if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            clearLabelSelectionsInWindow(*reusable->child());
        }
    }
}

[[nodiscard]] std::string selectedLabelText(GeneratedWindowBase& window) {
    std::string result;
    for (const auto& element : window.elements()) {
        if (element == nullptr) {
            continue;
        }
        if (const auto* label = dynamic_cast<const Label*>(element.get())) {
            result += label->selectedText();
            continue;
        }
        if (const auto* reusable = dynamic_cast<const ReusableElement*>(element.get());
            reusable != nullptr && reusable->child() != nullptr) {
            result += selectedLabelText(*reusable->child());
        }
    }
    return result;
}

[[nodiscard]] bool handleLabelMousePress(GeneratedWindowBase& window, Point position,
                                         Element*& mouseSelectionElement, int& mouseSelectionAnchor) {
    for (const auto& element : window.elements()) {
        if (element == nullptr) {
            continue;
        }
        auto* label = dynamic_cast<Label*>(element.get());
        if (label == nullptr) {
            if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
                reusable != nullptr && reusable->child() != nullptr) {
                if (handleLabelMousePress(*reusable->child(), position, mouseSelectionElement, mouseSelectionAnchor)) {
                    return true;
                }
            }
            continue;
        }
        if (!labelAllowsTextSelection(*label)) {
            continue;
        }
        const Rect frame = label->frame();
        if (!frame.contains(position)) {
            continue;
        }
        const int localRow = position.row - frame.row;
        const int localCol = position.col - frame.col;
        const int pos = label->textPositionFromPoint(localRow, localCol, Size{frame.width, frame.height});
        label->selectRange(pos, pos);
        mouseSelectionElement = label;
        mouseSelectionAnchor = pos;
        return true;
    }
    return false;
}

[[nodiscard]] bool handleMouseDrag(Element* mouseSelectionElement, int mouseSelectionAnchor, Point position) {
    if (auto* textInput = dynamic_cast<TextInput*>(mouseSelectionElement)) {
        const Rect frame = textInput->frame();
        int localRow = position.row - frame.row;
        const int localCol = position.col - frame.col;
        if (dynamic_cast<TextArea*>(textInput) != nullptr && frame.height > 1) {
            if (localRow < 0) {
                (void)textInput->scrollByRows(-1, frame.height, false);
                localRow = 0;
            } else if (localRow >= frame.height) {
                (void)textInput->scrollByRows(1, frame.height, false);
                localRow = frame.height - 1;
            }
        }
        const int cursor = textInput->cursorForPoint(localRow, localCol, Size{frame.width, frame.height});
        textInput->selectRange(mouseSelectionAnchor, cursor);
        return true;
    }
    if (auto* label = dynamic_cast<Label*>(mouseSelectionElement)) {
        const Rect frame = label->frame();
        const int cursor = label->textPositionFromPoint(position.row - frame.row, position.col - frame.col,
                                                        Size{frame.width, frame.height});
        label->selectRange(mouseSelectionAnchor, cursor);
        return true;
    }
    return false;
}

[[nodiscard]] Point clampedPointInFrame(Point position, const Rect& frame) {
    return Point{
        std::clamp(position.row, frame.row, frame.row + std::max(0, frame.height - 1)),
        std::clamp(position.col, frame.col, frame.col + std::max(0, frame.width - 1)),
    };
}

[[nodiscard]] bool handleScrollSelectionPress(const std::vector<Element*>& elements,
                                              ScrollSelection& selection,
                                              Point position) {
    Element* target = elementAt(elements, position);
    auto* scrollView = dynamic_cast<ScrollView*>(target);
    if (scrollView == nullptr) {
        selection = ScrollSelection{};
        return false;
    }
    const Point clamped = clampedPointInFrame(position, scrollView->frame());
    selection = ScrollSelection{
        .element = scrollView,
        .anchor = clamped,
        .current = clamped,
        .active = true,
        .changed = false,
    };
    return true;
}

[[nodiscard]] bool handleScrollSelectionDrag(ScrollSelection& selection, Point position) {
    if (selection.element == nullptr || !selection.active) {
        return false;
    }
    const Point clamped = clampedPointInFrame(position, selection.element->frame());
    selection.changed = selection.changed || clamped.row != selection.anchor.row || clamped.col != selection.anchor.col;
    selection.current = clamped;
    return true;
}

[[nodiscard]] bool pointInSelection(Point point, const ScrollSelection& selection) {
    if (selection.element == nullptr || !selection.changed) {
        return false;
    }
    Point start = selection.anchor;
    Point end = selection.current;
    if (std::tie(end.row, end.col) < std::tie(start.row, start.col)) {
        std::swap(start, end);
    }
    if (point.row < start.row || point.row > end.row) {
        return false;
    }
    if (start.row == end.row) {
        return point.col >= start.col && point.col <= end.col;
    }
    if (point.row == start.row) {
        return point.col >= start.col;
    }
    if (point.row == end.row) {
        return point.col <= end.col;
    }
    return true;
}

void applyScrollSelection(TerminalBuffer& buffer, const ScrollSelection& selection) {
    if (selection.element == nullptr || !selection.changed) {
        return;
    }
    Style selectionStyle = selection.element->effectiveStyle();
    if (selection.element->selectedStyle().has_value()) {
        selectionStyle.merge(*selection.element->selectedStyle());
    } else if (selection.element->cursorStyle().has_value()) {
        selectionStyle.merge(*selection.element->cursorStyle());
    }
    const Rect frame = selection.element->frame();
    for (int row = std::max(0, frame.row); row < std::min(buffer.height(), frame.row + frame.height); ++row) {
        for (int col = std::max(0, frame.col); col < std::min(buffer.width(), frame.col + frame.width); ++col) {
            if (!pointInSelection(Point{row, col}, selection)) {
                continue;
            }
            TerminalCell cell = buffer.cell(row, col);
            const std::optional<Color> originalForeground = cell.foreground;
            const std::optional<Color> originalBackground = cell.background;
            cell.foreground = selectionStyle.color.has_value() ? selectionStyle.color : originalBackground;
            cell.background = selectionStyle.background.has_value() ? selectionStyle.background : originalForeground;
            buffer.setCell(row, col, std::move(cell));
        }
    }
}

[[nodiscard]] std::string selectedScrollText(const TerminalBuffer& buffer, const ScrollSelection& selection) {
    if (selection.element == nullptr || !selection.changed) {
        return {};
    }
    Point start = selection.anchor;
    Point end = selection.current;
    if (std::tie(end.row, end.col) < std::tie(start.row, start.col)) {
        std::swap(start, end);
    }
    std::string result;
    for (int row = std::max(0, start.row); row <= std::min(buffer.height() - 1, end.row); ++row) {
        const int firstCol = std::max(0, row == start.row ? start.col : selection.element->frame().col);
        const int lastCol = std::min(buffer.width() - 1, row == end.row ? end.col : selection.element->frame().col + selection.element->frame().width - 1);
        std::string line;
        for (int col = firstCol; col <= lastCol; ++col) {
            line += buffer.cell(row, col).text;
        }
        while (!line.empty() && line.back() == ' ') {
            line.pop_back();
        }
        if (!result.empty()) {
            result.push_back('\n');
        }
        result += line;
    }
    return result;
}

[[nodiscard]] ScrollView* parentScrollView(Element* element) {
    Element* child = element;
    while (child != nullptr && child->parent() != nullptr) {
        Element* parent = child->parent();
        if (auto* scrollView = dynamic_cast<ScrollView*>(parent)) {
            return scrollView;
        }
        child = parent;
    }
    return nullptr;
}

[[nodiscard]] ScrollView* scrollViewAtPosition(GeneratedWindowBase& window, Point position) {
    for (auto it = window.elements().rbegin(); it != window.elements().rend(); ++it) {
        Element* element = it->get();
        if (element == nullptr) {
            continue;
        }
        if (auto* reusable = dynamic_cast<ReusableElement*>(element);
            reusable != nullptr && reusable->child() != nullptr) {
            const Rect frame = reusable->frame();
            if (frame.width > 0 && frame.height > 0) {
                syncReusableChildFrames(*reusable, frame);
            }
            if (ScrollView* nested = scrollViewAtPosition(*reusable->child(), position)) {
                return nested;
            }
        }
        if (auto* scrollView = dynamic_cast<ScrollView*>(element);
            scrollView != nullptr && scrollView->frame().contains(position)) {
            return scrollView;
        }
    }
    return nullptr;
}

[[nodiscard]] bool handleMouseWheel(const std::vector<Element*>& focusable, Point position, int wheelDelta) {
    Element* target = elementAt(focusable, position);
    if (auto* scrollView = dynamic_cast<ScrollView*>(target)) {
        return scrollView->handleWheel(wheelDelta, Size{scrollView->frame().width, scrollView->frame().height});
    }
    if (auto* scrollView = parentScrollView(target)) {
        return scrollView->handleWheel(wheelDelta, Size{scrollView->frame().width, scrollView->frame().height});
    }
    if (auto* listBox = dynamic_cast<ListBox*>(target)) {
        const int delta = wheelDelta > 0 ? -std::abs(wheelDelta) : std::abs(wheelDelta);
        listBox->scrollBy(delta, listBox->frame().height);
        return true;
    }
    if (auto* textInput = dynamic_cast<TextInput*>(target)) {
        const int delta = (wheelDelta > 0 ? -std::abs(wheelDelta) : std::abs(wheelDelta)) *
            kTextInputWheelScrollRows;
        return textInput->scrollByRows(delta, textInput->frame().height);
    }
    return false;
}

[[nodiscard]] bool handleScrollViewMouseWheel(GeneratedWindowBase& window, Point position, int wheelDelta) {
    if (auto* nestedScrollView = scrollViewAtPosition(window, position)) {
        return nestedScrollView->handleWheel(
            wheelDelta,
            Size{nestedScrollView->frame().width, nestedScrollView->frame().height});
    }
    Element* target = elementAt(layoutElements(window), position);
    if (auto* scrollView = dynamic_cast<ScrollView*>(target)) {
        return scrollView->handleWheel(wheelDelta, Size{scrollView->frame().width, scrollView->frame().height});
    }
    return false;
}

void queueScrollRegionHint(std::optional<ScrollRegionHint>& pending, ScrollView* scrollView) {
    if (scrollView == nullptr) {
        return;
    }
    const int delta = scrollView->consumeTerminalScrollDelta();
    if (delta == 0) {
        return;
    }
    const Rect rect = scrollView->frame();
    if (pending.has_value() &&
        pending->rect.row == rect.row &&
        pending->rect.col == rect.col &&
        pending->rect.width == rect.width &&
        pending->rect.height == rect.height) {
        pending->delta += delta;
        if (pending->delta == 0) {
            pending.reset();
        }
        return;
    }
    pending = ScrollRegionHint{.rect = rect, .delta = delta};
}

void queueScrollRegionHintForPosition(
    std::optional<ScrollRegionHint>& pending,
    GeneratedWindowBase& window,
    const std::vector<Element*>& focusable,
    Point position
) {
    if (Element* target = elementAt(focusable, position)) {
        if (auto* scrollView = dynamic_cast<ScrollView*>(target)) {
            queueScrollRegionHint(pending, scrollView);
        }
        if (auto* scrollView = parentScrollView(target)) {
            queueScrollRegionHint(pending, scrollView);
        }
    }
    if (auto* scrollView = scrollViewAtPosition(window, position)) {
        queueScrollRegionHint(pending, scrollView);
    }
}

[[nodiscard]] bool handleAppMouseWheel(GeneratedWindowBase& window, const GeneratedWindowRuntimeOptions& options,
                                       Point position, int wheelDelta) {
    if (!options.onMouseWheel) {
        return false;
    }
    Element* target = elementAt(layoutElements(window), position);
    if (target == nullptr) {
        return false;
    }
    return options.onMouseWheel(target->name(), wheelDelta);
}

[[nodiscard]] int clampedMouseWheelDelta(int delta)
{
    return std::clamp(delta, -kMaxCoalescedMouseWheelDelta, kMaxCoalescedMouseWheelDelta);
}

[[nodiscard]] std::vector<Event> coalesceMouseWheelEvents(const std::vector<Event>& events)
{
    std::vector<Event> coalesced;
    coalesced.reserve(events.size());
    for (std::size_t index = 0; index < events.size();)
    {
        const Event& event = events[index];
        if (event.type != EventType::MouseWheel)
        {
            coalesced.push_back(event);
            ++index;
            continue;
        }

        Event merged = event;
        int delta = 0;
        while (index < events.size() && events[index].type == EventType::MouseWheel)
        {
            merged.position = events[index].position;
            delta += events[index].wheelDelta;
            ++index;
        }
        merged.wheelDelta = clampedMouseWheelDelta(delta);
        if (merged.wheelDelta != 0)
        {
            coalesced.push_back(merged);
        }
    }
    return coalesced;
}

[[nodiscard]] const char* optionValue(int argc, char** argv, std::string_view name, const char* fallback = nullptr) {
    for (int index = 1; index < argc; ++index) {
        if (std::string_view(argv[index]) == name && index + 1 < argc) {
            return argv[index + 1];
        }
    }
    return fallback;
}

[[nodiscard]] bool hasOption(int argc, char** argv, std::string_view name) {
    for (int index = 1; index < argc; ++index) {
        if (std::string_view(argv[index]) == name) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] std::optional<Rect> parseViewportArg(int argc, char** argv) {
    if (const char* value = optionValue(argc, argv, "--viewport")) {
        std::stringstream input(value);
        std::string part;
        std::vector<int> values;
        while (std::getline(input, part, ',')) {
            values.push_back(std::stoi(part));
        }
        if (values.size() != 4) {
            throw std::runtime_error("--viewport must have form row,col,width,height");
        }
        return Rect{std::max(0, values[0]), std::max(0, values[1]), std::max(1, values[2]), std::max(1, values[3])};
    }
    if (!hasOption(argc, argv, "--viewport-row") &&
        !hasOption(argc, argv, "--viewport-col") &&
        !hasOption(argc, argv, "--viewport-width") &&
        !hasOption(argc, argv, "--viewport-height")) {
        return std::nullopt;
    }
    return Rect{
        std::max(0, std::stoi(optionValue(argc, argv, "--viewport-row", "0"))),
        std::max(0, std::stoi(optionValue(argc, argv, "--viewport-col", "0"))),
        std::max(1, std::stoi(optionValue(argc, argv, "--viewport-width", "1"))),
        std::max(1, std::stoi(optionValue(argc, argv, "--viewport-height", "1"))),
    };
}

[[nodiscard]] McpRuntimeConfig parseMcpRuntimeArgs(int argc, char** argv) {
    McpRuntimeConfig config;
    config.enabled = hasOption(argc, argv, "--mcp-server");
    config.gui = !hasOption(argc, argv, "--headless") || hasOption(argc, argv, "--gui");
    const std::string defaultTransport = config.gui ? std::string(kMcpTransportTcp) : std::string(kMcpTransportStdio);
    config.transport = optionValue(argc, argv, "--mcp-transport", defaultTransport.c_str());
    config.host = optionValue(argc, argv, "--mcp-host", kDefaultMcpHost.data());
    config.port = std::stoi(optionValue(argc, argv, "--mcp-port", "8765"));
    const bool fast = hasOption(argc, argv, "--mcp-fast");
    const int defaultActionDelay = fast ? 0 : (config.gui ? kDefaultGuiMcpActionDelayMs : kDefaultHeadlessMcpActionDelayMs);
    const int defaultTypeDelay = fast ? 0 : (config.gui ? kDefaultGuiMcpTypeDelayMs : kDefaultHeadlessMcpTypeDelayMs);
    config.actionDelayMs = std::stoi(optionValue(argc, argv, "--mcp-action-delay-ms", std::to_string(defaultActionDelay).c_str()));
    config.typeDelayMs = std::stoi(optionValue(argc, argv, "--mcp-type-delay-ms", std::to_string(defaultTypeDelay).c_str()));
    config.waitRender = hasOption(argc, argv, "--mcp-wait-render");
    config.controlledRender = hasOption(argc, argv, "--mcp-controlled-render");
    config.viewport = parseViewportArg(argc, argv);
    return config;
}

[[nodiscard]] std::string elementType(const Element& element) {
    if (dynamic_cast<const TextArea*>(&element) != nullptr) return "textarea";
    if (dynamic_cast<const TextInput*>(&element) != nullptr) return "textinput";
    if (dynamic_cast<const NumberInput*>(&element) != nullptr) return "numberinput";
    if (dynamic_cast<const Button*>(&element) != nullptr) return "button";
    if (dynamic_cast<const CheckBox*>(&element) != nullptr) return "checkbox";
    if (dynamic_cast<const ComboBox*>(&element) != nullptr) return "combobox";
    if (dynamic_cast<const ListBox*>(&element) != nullptr) return "listbox";
    if (dynamic_cast<const Image*>(&element) != nullptr) return "image";
    if (dynamic_cast<const Label*>(&element) != nullptr) return "label";
    if (dynamic_cast<const InfoLabel*>(&element) != nullptr) return "infolabel";
    if (dynamic_cast<const MessageTable*>(&element) != nullptr) return "messagetable";
    if (dynamic_cast<const ScrollView*>(&element) != nullptr) return "scrollview";
    if (dynamic_cast<const ViewHost*>(&element) != nullptr) return "viewhost";
    return "element";
}

[[nodiscard]] std::string elementRole(const Element& element) {
    const std::string type = elementType(element);
    if (type == "textinput" || type == "textarea" || type == "numberinput") return "input";
    if (type == "button") return "action";
    if (type == "checkbox") return "toggle";
    if (type == "combobox" || type == "listbox") return "selection";
    return "text";
}

[[nodiscard]] bool elementExposed(const GeneratedWindowBase& window, const Element& element) {
    const GeneratedElementMetadata* metadata = window.mcpElementMetadata(element.name());
    return metadata == nullptr || metadata->expose;
}

[[nodiscard]] std::string elementDescription(const GeneratedWindowBase& window, const Element& element) {
    const GeneratedElementMetadata* metadata = window.mcpElementMetadata(element.name());
    return metadata == nullptr ? std::string{} : metadata->description;
}

[[nodiscard]] bool styleHasAnimatedTextGradient(const Style& style) {
    return style.textColorGradient.has_value() || style.textBackgroundGradient.has_value();
}

[[nodiscard]] bool elementHasAnimatedTextGradient(const Element& element) {
    if (styleHasAnimatedTextGradient(element.style())) {
        return true;
    }
    if (element.focusStyle().has_value() && styleHasAnimatedTextGradient(*element.focusStyle())) {
        return true;
    }
    if (element.editStyle().has_value() && styleHasAnimatedTextGradient(*element.editStyle())) {
        return true;
    }
    if (element.cursorStyle().has_value() && styleHasAnimatedTextGradient(*element.cursorStyle())) {
        return true;
    }
    if (element.selectedStyle().has_value() && styleHasAnimatedTextGradient(*element.selectedStyle())) {
        return true;
    }
    for (const auto& child : element.children()) {
        if (elementHasAnimatedTextGradient(*child)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] bool windowHasAnimatedTextGradient(const GeneratedWindowBase& window) {
    if (styleHasAnimatedTextGradient(window.generatedWindowStyle())) {
        return true;
    }
    for (const auto& element : window.elements()) {
        if (elementHasAnimatedTextGradient(*element)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] JsonValue rectJson(const Rect& rect) {
    return JsonValue::Object{
        {"top", rect.row},
        {"left", rect.col},
        {"bottom", rect.row + rect.height},
        {"right", rect.col + rect.width},
        {"width", rect.width},
        {"height", rect.height},
    };
}

[[nodiscard]] JsonValue elementValueJson(const Element& element) {
    if (const auto* textInput = dynamic_cast<const TextInput*>(&element)) return textInput->value();
    if (const auto* numberInput = dynamic_cast<const NumberInput*>(&element)) return numberInput->value();
    if (const auto* checkbox = dynamic_cast<const CheckBox*>(&element)) return checkbox->checked();
    if (const auto* comboBox = dynamic_cast<const ComboBox*>(&element)) {
        const auto& options = comboBox->options();
        if (options.empty()) return "";
        return options[static_cast<std::size_t>(comboBox->selectedIndex())];
    }
    if (const auto* listBox = dynamic_cast<const ListBox*>(&element)) {
        JsonValue::Array selected;
        for (const std::string& value : listBox->selectedValues()) {
            selected.emplace_back(value);
        }
        return selected;
    }
    if (const auto* label = dynamic_cast<const Label*>(&element)) return label->text();
    if (const auto* image = dynamic_cast<const Image*>(&element)) return image->source();
    if (const auto* button = dynamic_cast<const Button*>(&element)) return button->title();
    return nullptr;
}

[[nodiscard]] std::optional<JsonValue::Array> elementOptionsJson(const Element& element) {
    const std::vector<std::string>* options = nullptr;
    if (const auto* comboBox = dynamic_cast<const ComboBox*>(&element)) {
        options = &comboBox->options();
    } else if (const auto* listBox = dynamic_cast<const ListBox*>(&element)) {
        options = &listBox->options();
    }
    if (options == nullptr) {
        return std::nullopt;
    }
    JsonValue::Array result;
    for (const std::string& option : *options) {
        result.emplace_back(option);
    }
    return result;
}

[[nodiscard]] JsonValue::Array elementCapabilitiesJson(const Element& element) {
    JsonValue::Array capabilities{"ui.get_element"};
    if (!elementValueJson(element).isNull()) {
        capabilities.emplace_back("ui.get_value");
    }
    if (dynamic_cast<const TextInput*>(&element) != nullptr ||
        dynamic_cast<const NumberInput*>(&element) != nullptr) {
        capabilities.emplace_back("ui.set_value");
        capabilities.emplace_back("ui.fill");
        capabilities.emplace_back("ui.clear");
        capabilities.emplace_back("ui.focus");
    } else if (dynamic_cast<const Button*>(&element) != nullptr) {
        capabilities.emplace_back("ui.activate");
        capabilities.emplace_back("ui.click");
        capabilities.emplace_back("ui.focus");
    } else if (dynamic_cast<const CheckBox*>(&element) != nullptr) {
        capabilities.emplace_back("ui.set_value");
        capabilities.emplace_back("ui.check");
        capabilities.emplace_back("ui.uncheck");
        capabilities.emplace_back("ui.set_checked");
        capabilities.emplace_back("ui.activate");
        capabilities.emplace_back("ui.click");
        capabilities.emplace_back("ui.focus");
    } else if (dynamic_cast<const ComboBox*>(&element) != nullptr) {
        capabilities.emplace_back("ui.set_value");
        capabilities.emplace_back("ui.select_option");
        capabilities.emplace_back("ui.get_options");
        capabilities.emplace_back("ui.focus");
        capabilities.emplace_back("ui.press");
    } else if (dynamic_cast<const ListBox*>(&element) != nullptr) {
        capabilities.emplace_back("ui.set_value");
        capabilities.emplace_back("ui.select_option");
        capabilities.emplace_back("ui.set_selection");
        capabilities.emplace_back("ui.get_options");
        capabilities.emplace_back("ui.focus");
        capabilities.emplace_back("ui.press");
    }
    return capabilities;
}

void overlayFocusedComboBox(TerminalBuffer& buffer, GeneratedWindowBase& window, int focusedIndex, bool editMode,
                            int windowRow, int windowCol, Element* activeScrollView = nullptr,
                            Element* activeScrollViewEditElement = nullptr) {
    if (!editMode) {
        return;
    }
    const std::vector<Element*> focusable = focusableElements(window, activeScrollView);
    Element* focusedElement = focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size())
        ? focusable[static_cast<std::size_t>(focusedIndex)]
        : nullptr;
    if (activeScrollView != nullptr && activeScrollViewEditElement != focusedElement) {
        return;
    }
    auto* comboBox = dynamic_cast<ComboBox*>(focusedElement);
    if (comboBox == nullptr) {
        return;
    }
    const Rect comboFrame = comboBox->frame();
    const Size overlaySize{comboFrame.width, kComboBoxClosedRows + static_cast<int>(comboBox->options().size())};
    blit(buffer,
         renderElement(*comboBox, overlaySize, ElementRenderState{.focused = true, .editMode = true}),
         windowRow + comboFrame.row,
         windowCol + comboFrame.col);
}

[[nodiscard]] RenderedContent renderWindowFrameContent(GeneratedWindowBase& window, Size frameSize,
                                                       int focusedIndex, bool editMode,
                                                       Element* activeScrollView,
                                                       Element* activeScrollViewEditElement) {
    frameSize.width = std::max(kMinimumRenderableSize, frameSize.width);
    frameSize.height = std::max(kMinimumRenderableSize, frameSize.height);
    TerminalBuffer frameBuffer(frameSize.width, frameSize.height);
    frameBuffer.clear();
    const Style& style = window.generatedWindowStyle();
    const Rect localBounds{0, 0, frameSize.width, frameSize.height};
    fillRect(frameBuffer, localBounds, borderStyleFor(style));
    const Rect content = contentBounds(localBounds, style);
    fillRect(frameBuffer, content, style);
    blit(frameBuffer,
         renderGeneratedWindowContent(
             window,
             Size{content.width, content.height},
             focusedIndex,
             editMode,
             activeScrollView,
             activeScrollViewEditElement),
         content.row,
         content.col);
    overlayFocusedComboBox(frameBuffer, window, focusedIndex, editMode, 0, 0,
                           activeScrollView, activeScrollViewEditElement);
    return contentFromBuffer(frameBuffer);
}

[[nodiscard]] RenderedContent renderViewportContent(GeneratedWindowBase& window, const McpRuntimeState& state,
                                                    int focusedIndex, bool editMode) {
    Element* scopedActiveScrollView =
        state.activeScrollView != nullptr && ownerWindowForElement(window, state.activeScrollView) != nullptr
            ? state.activeScrollView
            : nullptr;
    Element* scopedActiveScrollViewEditElement =
        state.activeScrollViewEditElement != nullptr &&
                ownerWindowForElement(window, state.activeScrollViewEditElement) != nullptr
            ? state.activeScrollViewEditElement
            : nullptr;
    const Size contentSize = activeWindowReportedSize(window, state);
    if (state.viewport.has_value()) {
        const Size viewportSize{
            std::max(kMinimumRenderableSize, state.viewport->width),
            std::max(kMinimumRenderableSize, state.viewport->height),
        };
        TerminalBuffer buffer(viewportSize.width, viewportSize.height);
        buffer.clear();

        const GeneratedWindowMode mode = windowMode(window);
        const int row = (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandHeight)
            ? kContentTopRow
            : kContentTopRow + std::max(0, (viewportSize.height - contentSize.height) / 2);
        const int col = (mode == GeneratedWindowMode::Fullscreen || mode == GeneratedWindowMode::ExpandWidth)
            ? kContentLeftCol
            : std::max(kContentLeftCol, (viewportSize.width - contentSize.width) / 2);
        blit(buffer,
             renderWindowFrameContent(window, contentSize, focusedIndex, editMode,
                                      scopedActiveScrollView, scopedActiveScrollViewEditElement),
             row,
             col);
        overlayFocusedComboBox(buffer, window, focusedIndex, editMode, row, col,
                               scopedActiveScrollView, scopedActiveScrollViewEditElement);
        return contentFromBuffer(buffer);
    }

    return renderWindowFrameContent(window, contentSize, focusedIndex, editMode,
                                    scopedActiveScrollView, scopedActiveScrollViewEditElement);
}

void renderWindowStackOverlay(TerminalBuffer& buffer, GeneratedWindowStack& stack) {
    std::vector<GeneratedWindowStackFrame>& frames = stack.frames();
    for (GeneratedWindowStackFrame& frame : frames) {
        if (frame.window == nullptr) {
            continue;
        }
        if (frame.options.dimBackground) {
            dimBufferForModalOverlay(buffer);
        }
        const Style& style = frame.window->generatedWindowStyle();
        const Rect bounds = refreshWindowStackFrameBounds(frame, buffer);
        TerminalBuffer frameBuffer(bounds.width, bounds.height);
        frameBuffer.clear();
        const Rect localBounds{0, 0, bounds.width, bounds.height};
        fillRect(frameBuffer, localBounds, borderStyleFor(style));
        const Rect content = contentBounds(localBounds, style);
        fillRect(frameBuffer, content, style);
        const bool topFrame = &frame == &frames.back();
        RenderedContent contentRows;
        if (topFrame) {
            contentRows = renderGeneratedWindowContent(
                *frame.window,
                Size{content.width, content.height},
                frame.focusedIndex,
                frame.editMode,
                frame.activeScrollView,
                frame.activeScrollViewEditElement);
        } else {
            ScopedImageCellBackgroundRendering imageFallback;
            contentRows = renderGeneratedWindowContent(
                *frame.window,
                Size{content.width, content.height},
                frame.focusedIndex,
                false,
                nullptr,
                nullptr);
        }
        blit(frameBuffer, contentRows, content.row, content.col);
        RenderedContent rendered = contentFromBuffer(frameBuffer);
        blit(buffer, rendered, bounds.row, bounds.col);
        if (topFrame) {
            overlayFocusedComboBox(buffer, *frame.window, frame.focusedIndex, frame.editMode, bounds.row, bounds.col,
                                   frame.activeScrollView, frame.activeScrollViewEditElement);
        }
    }
}

[[nodiscard]] JsonValue snapshotColor(const std::optional<Color>& color) {
    if (!color.has_value() || color->kind() == Color::Kind::Unset || color->isTransparent()) {
        return nullptr;
    }
    const std::string value = color->toString();
    if (value.empty()) {
        return nullptr;
    }
    return value;
}

[[nodiscard]] TerminalCell snapshotVisualCell(const TerminalCell& cell) {
    return cell;
}

[[nodiscard]] JsonValue renderCellsFromContent(const RenderedContent& content) {
    JsonValue::Array rows;
    for (const RenderedRow& row : content) {
        JsonValue::Array cells;
        for (const TerminalCell& cell : row) {
            const TerminalCell visual = snapshotVisualCell(cell);
            cells.emplace_back(JsonValue::Object{
                {"char", visual.text.empty() ? std::string(" ") : visual.text},
                {"foreground", snapshotColor(visual.foreground)},
                {"background", snapshotColor(visual.background)},
                {"attributes", JsonValue::Array{}},
            });
        }
        rows.emplace_back(std::move(cells));
    }
    return rows;
}

[[nodiscard]] JsonValue renderCompactCellsFromContent(const RenderedContent& content) {
    JsonValue::Array rows;
    for (const RenderedRow& row : content) {
        JsonValue::Array cells;
        for (const TerminalCell& cell : row) {
            const TerminalCell visual = snapshotVisualCell(cell);
            JsonValue::Array compactCell;
            compactCell.emplace_back(visual.text.empty() ? std::string(" ") : visual.text);
            compactCell.emplace_back(snapshotColor(visual.foreground));
            compactCell.emplace_back(snapshotColor(visual.background));
            compactCell.emplace_back(JsonValue::Array{});
            cells.emplace_back(std::move(compactCell));
        }
        rows.emplace_back(std::move(cells));
    }
    return rows;
}

class McpController {
public:
    McpController(GeneratedWindowBase& window, GeneratedWindowRuntimeOptions& options,
                  McpRuntimeState& state, McpRuntimeConfig config)
        : window_(window), options_(options), state_(state), config_(std::move(config)) {}

    [[nodiscard]] JsonValue callTool(const std::string& name, const JsonValue::Object& arguments) {
        std::unique_lock<std::recursive_mutex> uiLock(state_.uiMutex);
        ScopedUiMutationLock scopedLock(uiLock);
        return callToolLocked(name, arguments);
    }

    [[nodiscard]] JsonValue callToolLocked(const std::string& name, const JsonValue::Object& arguments) {
        const std::string toolName = resolveToolName(name);
        if (toolName == "get_window") return toolGetWindow();
        if (toolName == "get_elements") return toolGetElements();
        if (toolName == "get_element") return toolGetElement(jsonStringField(arguments, "element_id"));
        if (toolName == "get_value") return toolGetValue(jsonStringField(arguments, "element_id"));
        if (toolName == "get_options") return toolGetOptions(jsonStringField(arguments, "element_id"));
        if (toolName == "get_schema") return toolGetSchema();
        if (toolName == "get_source") return toolGetSource();
        if (toolName == "get_focused_element") return toolGetFocusedElement();
        if (toolName == "get_edit_mode") return JsonValue::Object{{"edit_mode", activeEditModeValue(activeWindow())}};
        if (toolName == "get_state") return toolGetState();
        if (toolName == "get_accessibility_snapshot") return toolGetState();
        if (toolName == "get_render_frame") return toolGetRenderFrame();
        if (toolName == "get_render_snapshot") return toolGetRenderSnapshot(arguments);
        if (toolName == "get_render_snapshot_compact") return toolGetRenderSnapshotCompact(arguments);
        if (toolName == "get_text_snapshot") return toolGetTextSnapshot();
        if (toolName == "get_viewport") return toolGetViewport();
        if (toolName == "get_render_rect") return toolGetRenderRect();
        if (toolName == "set_viewport") return toolSetViewport(arguments);
        if (toolName == "repaint") return toolRepaint();
        if (toolName == "focus_element") return toolFocusElement(jsonStringField(arguments, "element_id"));
        if (toolName == "enter_edit_mode") return toolEnterEditMode(arguments);
        if (toolName == "exit_edit_mode") return toolExitEditMode();
        if (toolName == "activate_element") return toolActivateElement(jsonStringField(arguments, "element_id"));
        if (toolName == "click_element") return toolClickElement(jsonStringField(arguments, "element_id"));
        if (toolName == "press_key") return toolPressKey(jsonStringField(arguments, "key"));
        if (toolName == "set_text") return toolSetText(arguments, false);
        if (toolName == "paste_text") return toolSetText(arguments, true);
        if (toolName == "type_text") return toolTypeText(arguments);
        if (toolName == "replace_selection") return toolReplaceSelection(arguments);
        if (toolName == "clear_text") return toolClearText(arguments);
        if (toolName == "set_value") return toolSetValue(arguments);
        if (toolName == "set_checked") return toolSetChecked(arguments);
        if (toolName == "check") return toolSetChecked(arguments, true);
        if (toolName == "uncheck") return toolSetChecked(arguments, false);
        if (toolName == "select_option") return toolSelectOption(arguments);
        if (toolName == "set_selection") return toolSetSelection(arguments);
        if (toolName == "move_cursor") return toolMoveCursor(arguments);
        if (toolName == "set_cursor") return toolSetCursor(arguments);
        if (toolName == "select_text") return toolSelectText(arguments);
        if (toolName == "select_all") return toolSelectAll(arguments);
        if (toolName == "copy_selection") return toolCopySelection(arguments);
        if (toolName == "cut_selection") return toolCutSelection(arguments);
        if (toolName == "get_text") return toolGetText(arguments);
        if (toolName == "get_selection") return toolGetSelection(arguments);
        if (toolName == "get_cursor") return toolGetCursor(arguments);
        if (toolName == "mouse_click") return toolMouseClick(arguments);
        if (toolName == "mouse_drag") return toolMouseDrag(arguments);
        if (toolName == "scroll") return toolScroll(arguments);
        if (appToolMetadata(toolName) != nullptr) return toolCallAppTool(toolName, arguments);
        throw std::runtime_error("Unknown MCP UI tool: " + name);
    }

    [[nodiscard]] JsonValue listTools() const {
        std::lock_guard<std::recursive_mutex> uiLock(state_.uiMutex);
        JsonValue::Array tools;
        std::vector<std::string> names = mcpToolNames();
        for (const GeneratedAppToolMetadata& appTool : activeWindow().mcpAppTools()) {
            names.push_back(appTool.name);
        }
        std::sort(names.begin(), names.end());
        names.erase(std::unique(names.begin(), names.end()), names.end());
        for (std::string name : names) {
            const std::string resolved = resolveToolName(name);
            if (const GeneratedAppToolMetadata* appTool = appToolMetadata(resolved); appTool != nullptr) {
                JsonValue::Object definition{
                    {"name", std::move(name)},
                    {"description", appTool->description},
                    {"inputSchema", parseJsonSchema(appTool->inputSchemaJson)},
                };
                if (!appTool->outputSchemaJson.empty()) {
                    definition["outputSchema"] = parseJsonSchema(appTool->outputSchemaJson);
                }
                tools.emplace_back(std::move(definition));
            } else {
                tools.emplace_back(JsonValue::Object{
                    {"name", std::move(name)},
                    {"description", ""},
                    {"inputSchema", toolInputSchema(resolved)},
                });
            }
        }
        return JsonValue::Object{{"tools", std::move(tools)}};
    }

private:
    struct ScopedUiMutationLock {
        explicit ScopedUiMutationLock(std::unique_lock<std::recursive_mutex>& lock)
            : previous(currentUiLock), lock(&lock) {
            currentUiLock = this;
        }

        ~ScopedUiMutationLock() {
            currentUiLock = previous;
        }

        ScopedUiMutationLock* previous = nullptr;
        std::unique_lock<std::recursive_mutex>* lock = nullptr;
    };

    static thread_local ScopedUiMutationLock* currentUiLock;

    [[nodiscard]] static std::vector<std::string> mcpToolNames() {
        std::vector<std::string> names{
            "activate_element",
            "check",
            "clear_text",
            "click_element",
            "copy_selection",
            "cut_selection",
            "enter_edit_mode",
            "exit_edit_mode",
            "focus_element",
            "get_accessibility_snapshot",
            "get_cursor",
            "get_edit_mode",
            "get_element",
            "get_elements",
            "get_focused_element",
            "get_options",
            "get_render_rect",
            "get_render_frame",
            "get_render_snapshot",
            "get_render_snapshot_compact",
            "get_schema",
            "get_selection",
            "get_source",
            "get_state",
            "get_text",
            "get_text_snapshot",
            "get_value",
            "get_viewport",
            "get_window",
            "mouse_click",
            "mouse_drag",
            "move_cursor",
            "paste_text",
            "press_key",
            "repaint",
            "replace_selection",
            "scroll",
            "select_all",
            "select_option",
            "select_text",
            "set_checked",
            "set_cursor",
            "set_selection",
            "set_text",
            "set_value",
            "set_viewport",
            "type_text",
            "ui.activate",
            "ui.check",
            "ui.clear",
            "ui.click",
            "ui.copy_selection",
            "ui.cut_selection",
            "ui.fill",
            "ui.focus",
            "ui.get_element",
            "ui.get_elements",
            "ui.get_focused_element",
            "ui.get_options",
            "ui.get_schema",
            "ui.get_source",
            "ui.get_value",
            "ui.get_window",
            "ui.press",
            "ui.render_snapshot",
            "ui.select_option",
            "ui.select_text",
            "ui.set_checked",
            "ui.set_cursor",
            "ui.set_selection",
            "ui.set_value",
            "ui.snapshot",
            "ui.text_snapshot",
            "ui.uncheck",
            "uncheck",
        };
        std::sort(names.begin(), names.end());
        names.erase(std::unique(names.begin(), names.end()), names.end());
        return names;
    }

    [[nodiscard]] static std::string resolveToolName(const std::string& name) {
        std::string local = name;
        if (local.rfind("ui.", 0) == 0) {
            local = local.substr(3);
        }
        if (local == "get_window") return "get_window";
        if (local == "get_elements") return "get_elements";
        if (local == "get_element") return "get_element";
        if (local == "get_value") return "get_value";
        if (local == "get_options") return "get_options";
        if (local == "get_focused_element") return "get_focused_element";
        if (local == "snapshot") return "get_accessibility_snapshot";
        if (local == "render_snapshot") return "get_render_snapshot";
        if (local == "text_snapshot") return "get_text_snapshot";
        if (local == "get_schema") return "get_schema";
        if (local == "get_source") return "get_source";
        if (local == "set_value") return "set_value";
        if (local == "focus") return "focus_element";
        if (local == "activate") return "activate_element";
        if (local == "click") return "click_element";
        if (local == "press") return "press_key";
        if (local == "fill") return "set_text";
        if (local == "clear") return "clear_text";
        if (local == "select_option") return "select_option";
        if (local == "set_selection") return "set_selection";
        if (local == "set_checked") return "set_checked";
        if (local == "check") return "check";
        if (local == "uncheck") return "uncheck";
        if (local == "set_cursor") return "set_cursor";
        if (local == "select_text") return "select_text";
        if (local == "copy_selection") return "copy_selection";
        if (local == "cut_selection") return "cut_selection";
        return local;
    }

    [[nodiscard]] static JsonValue stringType() {
        return JsonValue::Object{{"type", "string"}};
    }

    [[nodiscard]] static JsonValue integerType() {
        return JsonValue::Object{{"type", "integer"}};
    }

    [[nodiscard]] static JsonValue booleanType() {
        return JsonValue::Object{{"type", "boolean"}};
    }

    [[nodiscard]] static JsonValue emptyObjectSchema() {
        return JsonValue::Object{{"type", "object"}, {"properties", JsonValue::Object{}}};
    }

    [[nodiscard]] static JsonValue parseJsonSchema(std::string_view json) {
        if (json.empty()) {
            return emptyObjectSchema();
        }
        return JsonParser(json).parse();
    }

    [[nodiscard]] static JsonValue toolInputSchema(const std::string& name) {
        JsonValue::Object properties;
        JsonValue::Array required;
        auto addElementId = [&] {
            properties["element_id"] = stringType();
            required.emplace_back("element_id");
        };
        const auto elementOnly = std::vector<std::string>{
            "get_element", "get_value", "get_options", "focus_element", "activate_element",
            "click_element", "clear_text", "select_all", "copy_selection", "cut_selection",
            "get_text", "get_selection", "get_cursor", "check", "uncheck",
        };
        if (std::find(elementOnly.begin(), elementOnly.end(), name) != elementOnly.end()) {
            addElementId();
        } else if (name == "set_value") {
            addElementId();
            properties["value"] = JsonValue::Object{{"oneOf", JsonValue::Array{
                JsonValue::Object{{"type", "string"}},
                JsonValue::Object{{"type", "number"}},
                JsonValue::Object{{"type", "boolean"}},
                JsonValue::Object{{"type", "array"}, {"items", stringType()}},
            }}};
            required.emplace_back("value");
        } else if (name == "set_text" || name == "paste_text" || name == "type_text" || name == "replace_selection") {
            addElementId();
            properties["text"] = stringType();
            required.emplace_back("text");
        } else if (name == "set_checked") {
            addElementId();
            properties["checked"] = booleanType();
            required.emplace_back("checked");
        } else if (name == "select_option") {
            addElementId();
            properties["value"] = stringType();
            required.emplace_back("value");
        } else if (name == "set_selection") {
            addElementId();
            properties["values"] = JsonValue::Object{{"type", "array"}, {"items", stringType()}};
            required.emplace_back("values");
        } else if (name == "press_key") {
            properties["key"] = stringType();
            required.emplace_back("key");
        } else if (name == "move_cursor") {
            addElementId();
            properties["direction"] = stringType();
            properties["count"] = JsonValue::Object{{"type", "integer"}, {"minimum", 1}};
            required.emplace_back("direction");
        } else if (name == "set_cursor") {
            addElementId();
            properties["offset"] = JsonValue::Object{{"type", "integer"}, {"minimum", 0}};
            required.emplace_back("offset");
        } else if (name == "select_text") {
            addElementId();
            properties["start"] = JsonValue::Object{{"type", "integer"}, {"minimum", 0}};
            properties["end"] = JsonValue::Object{{"type", "integer"}, {"minimum", 0}};
            required.emplace_back("start");
            required.emplace_back("end");
        } else if (name == "set_viewport") {
            for (const std::string field : {"row", "col", "width", "height"}) {
                properties[field] = integerType();
                required.emplace_back(field);
            }
        } else if (name == "get_render_snapshot" || name == "get_render_snapshot_compact") {
            properties["snapshot_time_ms"] = integerType();
        } else if (name == "repaint") {
            properties["full"] = booleanType();
        } else if (name == "mouse_click") {
            properties["x"] = integerType();
            properties["y"] = integerType();
            required.emplace_back("x");
            required.emplace_back("y");
        } else if (name == "mouse_drag") {
            for (const std::string field : {"from_x", "from_y", "to_x", "to_y"}) {
                properties[field] = integerType();
                required.emplace_back(field);
            }
        } else if (name == "scroll") {
            addElementId();
            properties["delta"] = integerType();
            required.emplace_back("delta");
        }
        JsonValue::Object schema{{"type", "object"}, {"properties", std::move(properties)}};
        if (!required.empty()) {
            schema["required"] = std::move(required);
        }
        return schema;
    }

    [[nodiscard]] GeneratedWindowBase& activeWindow() const {
        if (options_.windowStack != nullptr) {
            if (const GeneratedWindowStackFrame* frame = options_.windowStack->top(); frame != nullptr && frame->window != nullptr) {
                return *frame->window;
            }
        }
        return window_;
    }

    [[nodiscard]] std::string activeWindowClassName() const {
        if (options_.windowStack != nullptr) {
            if (const GeneratedWindowStackFrame* frame = options_.windowStack->top(); frame != nullptr) {
                if (!frame->options.className.empty()) {
                    return frame->options.className;
                }
                if (frame->window != nullptr) {
                    if (!frame->window->mcpClassName().empty()) {
                        return frame->window->mcpClassName();
                    }
                    return compactClassName(frame->window->title());
                }
            }
        }
        if (!window_.mcpClassName().empty()) {
            return window_.mcpClassName();
        }
        return compactClassName(window_.title());
    }

    [[nodiscard]] GeneratedWindowStackFrame* activeStackFrame() const {
        return options_.windowStack == nullptr ? nullptr : options_.windowStack->top();
    }

    [[nodiscard]] Size viewportSizeForStackBounds() const {
        if (state_.viewport.has_value()) {
            return Size{
                std::max(kMinimumRenderableSize, state_.viewport->width),
                std::max(kMinimumRenderableSize, state_.viewport->height),
            };
        }
        const Size fallback = activeWindowReportedSize(window_, state_);
        return Size{
            std::max(kMinimumRenderableSize, fallback.width),
            std::max(kMinimumRenderableSize, fallback.height),
        };
    }

    void refreshActiveStackFrameBounds() const {
        GeneratedWindowStackFrame* frame = activeStackFrame();
        if (frame == nullptr || frame->window == nullptr) {
            return;
        }
        const Size viewportSize = viewportSizeForStackBounds();
        TerminalBuffer buffer(viewportSize.width, viewportSize.height);
        (void)refreshWindowStackFrameBounds(*frame, buffer);
    }

    void refreshActiveWindowLayoutForMouse() const {
        GeneratedWindowBase& window = activeWindow();
        const int focusedIndex = focusedIndexValue(window);
        const bool editMode = activeEditModeValue(window);
        if (GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr && frame->window == &window) {
            const Size viewportSize = viewportSizeForStackBounds();
            TerminalBuffer buffer(viewportSize.width, viewportSize.height);
            const Rect bounds = refreshWindowStackFrameBounds(*frame, buffer);
            const Rect content = contentBounds(Rect{0, 0, bounds.width, bounds.height}, window.generatedWindowStyle());
            (void)renderGeneratedWindowContent(
                window,
                Size{content.width, content.height},
                focusedIndex,
                editMode,
                frame->activeScrollView,
                frame->activeScrollViewEditElement);
            return;
        }
        (void)renderViewportContent(window, state_, focusedIndex, editMode);
    }

    void syncOverlayFocusState(GeneratedWindowBase& window) const {
        if (&window == &window_) {
            state_.overlayFocusedWindow = nullptr;
            state_.overlayFocusedClassName.clear();
            return;
        }
        const std::string className = activeWindowClassName();
        if (state_.overlayFocusedWindow == &window && state_.overlayFocusedClassName == className) {
            return;
        }
        const std::vector<Element*> focusable = focusableElements(window);
        state_.overlayFocusedWindow = &window;
        state_.overlayFocusedClassName = className;
        state_.overlayFocusedIndex = -1;
        state_.overlayEditMode = false;
    }

    void refreshActiveMcpEditMode() const {
        GeneratedWindowBase& window = activeWindow();
        syncOverlayFocusState(window);
        (void)window;
    }

    [[nodiscard]] int focusedIndexValue(GeneratedWindowBase& window) const {
        if (options_.windowStack != nullptr) {
            if (const GeneratedWindowStackFrame* frame = options_.windowStack->top(); frame != nullptr && frame->window == &window) {
                return frame->focusedIndex;
            }
        }
        if (&window == &window_) {
            return state_.focusedIndex;
        }
        return -1;
    }

    [[nodiscard]] bool activeEditModeValue(GeneratedWindowBase& window) const {
        if (options_.windowStack != nullptr) {
            if (const GeneratedWindowStackFrame* frame = options_.windowStack->top(); frame != nullptr && frame->window == &window) {
                return frame->editMode;
            }
        }
        if (&window == &window_) {
            return state_.editMode;
        }
        return false;
    }

    [[nodiscard]] int& focusedIndexRef(GeneratedWindowBase& window) {
        if (options_.windowStack != nullptr) {
            if (GeneratedWindowStackFrame* frame = options_.windowStack->top(); frame != nullptr && frame->window == &window) {
                return frame->focusedIndex;
            }
        }
        return &window == &window_ ? state_.focusedIndex : state_.overlayFocusedIndex;
    }

    [[nodiscard]] bool& activeEditModeRef(GeneratedWindowBase& window) {
        if (options_.windowStack != nullptr) {
            if (GeneratedWindowStackFrame* frame = options_.windowStack->top(); frame != nullptr && frame->window == &window) {
                return frame->editMode;
            }
        }
        return &window == &window_ ? state_.editMode : state_.overlayEditMode;
    }

    [[nodiscard]] const GeneratedAppToolMetadata* appToolMetadata(std::string_view name) const {
        for (const GeneratedAppToolMetadata& appTool : activeWindow().mcpAppTools()) {
            if (appTool.name == name) {
                return &appTool;
            }
        }
        return nullptr;
    }

    [[nodiscard]] JsonValue toolCallAppTool(const std::string& name, const JsonValue::Object& arguments) {
        const std::string result = activeWindow().callAppTool(name, jsonSerialize(JsonValue(arguments)));
        if (result.empty()) {
            return JsonValue::Object{};
        }
        return JsonParser(result).parse();
    }

    [[nodiscard]] Element* focusedElementFor(GeneratedWindowBase& window, const std::vector<Element*>& focusable) const {
        const int focusedIndex = focusedIndexValue(window);
        return focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size())
            ? focusable[static_cast<std::size_t>(focusedIndex)]
            : nullptr;
    }

    [[nodiscard]] FocusIdentity focusIdentityFor(GeneratedWindowBase& window, const std::vector<Element*>& focusable) const {
        Element* element = focusedElementFor(window, focusable);
        return FocusIdentity{
            .element = element,
            .identity = element == nullptr ? 0 : element->identity(),
        };
    }

    [[nodiscard]] Element* activeScrollViewForWindow(GeneratedWindowBase& window) const {
        return activeScrollView_ != nullptr && ownerWindowForElement(window, activeScrollView_) != nullptr
            ? activeScrollView_
            : nullptr;
    }

    void notifyActiveFrameFocusChanged(GeneratedWindowBase& window, Element* element, bool focused) {
        if (element == nullptr) {
            return;
        }
        if (options_.windowStack != nullptr) {
            if (GeneratedWindowStackFrame* frame = options_.windowStack->top(); frame != nullptr && frame->window == &window) {
                notifyFocusChanged(frame->options, element, focused);
                return;
            }
        }
        notifyFocusChanged(options_, element, focused);
    }

    void clearFocusIfElementRemoved(GeneratedWindowBase& window, FocusIdentity previous) {
        if (previous.element == nullptr) {
            return;
        }
        Element* activeScrollView = activeScrollViewForWindow(window);
        const std::vector<Element*> focusable = focusableElements(window, activeScrollView);
        for (const Element* element : focusable) {
            if (element == previous.element && element->identity() == previous.identity) {
                return;
            }
        }
        const std::vector<Element*> navigationFocusable = focusableElements(window, nullptr);
        if (focusedElementFor(window, navigationFocusable) != nullptr) {
            return;
        }
        if (activeScrollView != nullptr) {
            scrollViewLastDescendant_.erase(activeScrollView);
            activeScrollView_ = nullptr;
            activeScrollViewEditElement_ = nullptr;
            state_.activeScrollView = nullptr;
            state_.activeScrollViewEditElement = nullptr;
        }
        focusedIndexRef(window) = -1;
        activeEditModeRef(window) = false;
    }

    struct BackgroundFocusCleanupContext {
        bool hadActiveStackFrame = false;
        FocusIdentity focused;
    };

    [[nodiscard]] BackgroundFocusCleanupContext captureBackgroundFocusCleanupContext() const {
        BackgroundFocusCleanupContext context;
        context.hadActiveStackFrame = activeStackFrame() != nullptr;
        if (!context.hadActiveStackFrame) {
            return context;
        }
        const std::vector<Element*> focusable = focusableElements(window_, activeScrollViewForWindow(window_));
        context.focused = focusIdentityFor(window_, focusable);
        return context;
    }

    void cleanupBackgroundFocusAfterModalClose(const BackgroundFocusCleanupContext& context) {
        if (!context.hadActiveStackFrame || &activeWindow() != &window_) {
            return;
        }
        exitBackgroundEditModeAfterModalClose(window_);
        clearFocusIfElementRemoved(window_, context.focused);
    }

    void exitBackgroundEditModeAfterModalClose(GeneratedWindowBase& window) {
        bool& editMode = activeEditModeRef(window);
        Element* activeScrollView = activeScrollViewForWindow(window);
        const std::vector<Element*> focusable = focusableElements(window, activeScrollView);
        Element* focused = focusedElementFor(window, focusable);
        if (editMode) {
            if (activeScrollViewEditElement_ != nullptr) {
                commitEdit(activeScrollViewEditElement_);
                notifyActiveFrameTextChanged(activeScrollViewEditElement_);
            } else if (focused != nullptr && usesLeaveCommit(focused)) {
                commitEdit(focused);
                notifyActiveFrameTextChanged(focused);
            }
        }
        editSnapshot_.reset();
        editMode = false;
        activeScrollViewEditElement_ = nullptr;
        state_.activeScrollViewEditElement = nullptr;
        if (activeScrollView != nullptr) {
            scrollViewLastDescendant_[activeScrollView] = focused;
            const std::vector<Element*> navigationFocusable = focusableElements(window, nullptr);
            Element* scopeRoot = generatedScrollViewProxyFor(window, dynamic_cast<ScrollView*>(activeScrollView));
            const int scrollViewIndex = indexOfElement(
                navigationFocusable,
                scopeRoot == nullptr ? activeScrollView : scopeRoot);
            if (scrollViewIndex >= 0) {
                focusedIndexRef(window) = scrollViewIndex;
            }
            activeScrollView_ = nullptr;
            state_.activeScrollView = nullptr;
        }
    }

    void focusActiveWindowElement(GeneratedWindowBase& window, Element* element) {
        const bool activeScrollViewBelongsToWindow = activeScrollViewForWindow(window) != nullptr;
        Element* activeScrollViewProxy = generatedScrollViewProxyFor(window, dynamic_cast<ScrollView*>(activeScrollView_));
        if (activeScrollViewBelongsToWindow && (element == activeScrollView_ || element == activeScrollViewProxy)) {
            activeScrollView_ = nullptr;
            activeScrollViewEditElement_ = nullptr;
            state_.activeScrollView = nullptr;
            state_.activeScrollViewEditElement = nullptr;
            activeEditModeRef(window) = false;
        }
        if (activeScrollViewBelongsToWindow &&
            element != activeScrollView_ &&
            scrollViewContainingElement(window, element) != activeScrollView_) {
            activeScrollView_ = nullptr;
            activeScrollViewEditElement_ = nullptr;
            state_.activeScrollView = nullptr;
            state_.activeScrollViewEditElement = nullptr;
            activeEditModeRef(window) = false;
        }
        std::vector<Element*> focusable =
            focusableElements(window, activeScrollViewBelongsToWindow ? activeScrollView_ : nullptr);
        Element* previous = focusedElementFor(window, focusable);
        int& focusedIndex = focusedIndexRef(window);
        const int index = indexOfElement(focusable, element);
        if (index >= 0) {
            const bool changed = focusedIndex != index;
            focusedIndex = index;
            ensureElementVisibleInParentScrollView(element);
            if (changed) {
                notifyActiveFrameFocusChanged(window, previous, false);
                notifyActiveFrameFocusChanged(window, element, true);
            }
            return;
        }
        if (previous != nullptr) {
            focusedIndex = -1;
            activeEditModeRef(window) = false;
            notifyActiveFrameFocusChanged(window, previous, false);
        }
    }

    void focusActiveWindowElementWithScrollViewScope(GeneratedWindowBase& window, Element* element) {
        if (ScrollView* scrollView = scrollViewContainingElement(window, element)) {
            activeScrollView_ = scrollView;
            activeScrollViewEditElement_ = nullptr;
            activeEditModeRef(window) = true;
            state_.activeScrollView = activeScrollView_;
            state_.activeScrollViewEditElement = activeScrollViewEditElement_;
            std::vector<Element*> focusable = focusableElements(window, activeScrollView_);
            focusedIndexRef(window) = indexOfElement(focusable, element);
            scrollViewLastDescendant_[scrollView] = element;
            (void)ensureElementFrameVisibleInScrollView(*scrollView, element);
            return;
        }
        focusActiveWindowElement(window, element);
    }

    [[nodiscard]] JsonValue toolGetWindow() const {
        GeneratedWindowBase& window = activeWindow();
        const GeneratedWindowMode mode = windowMode(window);
        const Size size = activeWindowReportedSize(window, state_);
        return JsonValue::Object{
            {"title", window.title()},
            {"class", activeWindowClassName()},
            {"backend", "cpp"},
            {"mode", windowModeName(mode)},
            {"description", window.mcpWindowDescription()},
            {"width", size.width},
            {"height", size.height},
            {"mcp_enabled", window.mcpEnabled()},
        };
    }

    [[nodiscard]] JsonValue toolGetElements() const {
        JsonValue::Array elements;
        GeneratedWindowBase& window = activeWindow();
        std::function<void(GeneratedWindowBase&)> appendWindowElements = [&](GeneratedWindowBase& sourceWindow)
        {
            for (const auto& element : sourceWindow.elements())
            {
                if (elementExposed(sourceWindow, *element))
                {
                    elements.push_back(snapshot(*element));
                }
                if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
                    reusable != nullptr && reusable->child() != nullptr)
                {
                    appendWindowElements(*reusable->child());
                }
                if (auto* scrollView = dynamic_cast<ScrollView*>(element.get()))
                {
                    for (const auto& child : scrollView->children())
                    {
                        auto* reusable = child == nullptr ? nullptr : dynamic_cast<ReusableElement*>(child.get());
                        if (reusable != nullptr && reusable->child() != nullptr)
                        {
                            appendWindowElements(*reusable->child());
                        }
                    }
                }
            }
        };
        appendWindowElements(window);
        return elements;
    }

    [[nodiscard]] JsonValue toolGetElement(const std::string& elementId) const {
        return snapshot(*requireElement(elementId));
    }

    [[nodiscard]] JsonValue toolGetValue(const std::string& elementId) const {
        return JsonValue::Object{{"value", elementValueJson(*requireElement(elementId))}};
    }

    [[nodiscard]] JsonValue toolGetOptions(const std::string& elementId) const {
        Element* element = requireElement(elementId);
        std::optional<JsonValue::Array> options = elementOptionsJson(*element);
        if (!options.has_value()) {
            throw std::runtime_error("Element has no selectable options: " + elementId);
        }
        return JsonValue::Object{{"options", JsonValue(std::move(*options))}};
    }

    [[nodiscard]] JsonValue toolGetFocusedElement() const {
        GeneratedWindowBase& window = activeWindow();
        const std::vector<Element*> focusable = focusableElements(window, state_.activeScrollView);
        const int focusedIndex = focusedIndexValue(window);
        if (focusedIndex < 0 || focusedIndex >= static_cast<int>(focusable.size())) {
            return nullptr;
        }
        Element& element = *focusable[static_cast<std::size_t>(focusedIndex)];
        GeneratedWindowBase* owner = ownerWindowForElement(window, &element);
        if (!elementExposed(owner == nullptr ? window : *owner, element)) {
            return nullptr;
        }
        return snapshot(element);
    }

    [[nodiscard]] JsonValue toolGetState() const {
        return JsonValue::Object{
            {"window", toolGetWindow()},
            {"focused_element", toolGetFocusedElement()},
            {"edit_mode", activeEditModeValue(activeWindow())},
            {"elements", toolGetElements()},
        };
    }

    [[nodiscard]] JsonValue toolGetSchema() const {
        JsonValue::Array elements;
        GeneratedWindowBase& window = activeWindow();
        std::function<void(GeneratedWindowBase&)> appendWindowElements = [&](GeneratedWindowBase& sourceWindow)
        {
            for (const auto& element : sourceWindow.elements())
            {
                if (elementExposed(sourceWindow, *element))
                {
                    JsonValue item = snapshot(*element);
                    item.object()["capabilities"] = elementCapabilitiesJson(*element);
                    elements.push_back(std::move(item));
                }
                if (auto* reusable = dynamic_cast<ReusableElement*>(element.get());
                    reusable != nullptr && reusable->child() != nullptr)
                {
                    appendWindowElements(*reusable->child());
                }
                if (auto* scrollView = dynamic_cast<ScrollView*>(element.get()))
                {
                    for (const auto& child : scrollView->children())
                    {
                        auto* reusable = child == nullptr ? nullptr : dynamic_cast<ReusableElement*>(child.get());
                        if (reusable != nullptr && reusable->child() != nullptr)
                        {
                            appendWindowElements(*reusable->child());
                        }
                    }
                }
            }
        };
        appendWindowElements(window);
        return JsonValue::Object{
            {"window", toolGetWindow()},
            {"elements", std::move(elements)},
        };
    }

    [[nodiscard]] JsonValue toolGetSource() const {
        const GeneratedWindowBase& window = activeWindow();
        return JsonValue::Object{
            {"class", activeWindowClassName()},
            {"mcp_enabled", window.mcpEnabled()},
            {"source_markdown", window.mcpSourceMarkdown()},
        };
    }

    [[nodiscard]] RenderedContent toolRenderContent(GeneratedWindowBase& window,
                                                    int focusedIndex,
                                                    bool editMode) const {
        return renderViewportContent(window, state_, focusedIndex, editMode);
    }

    [[nodiscard]] JsonValue toolGetTextSnapshot() const {
        GeneratedWindowBase& window = activeWindow();
        const int focusedIndex = focusedIndexValue(window);
        const bool editMode = activeEditModeValue(window);
        JsonValue::Array lines;
        for (std::string line : renderedText(toolRenderContent(window, focusedIndex, editMode))) {
            lines.emplace_back(std::move(line));
        }
        return JsonValue::Object{{"lines", std::move(lines)}};
    }

    [[nodiscard]] JsonValue toolGetRenderFrame() const {
        // Render base window into viewport-sized buffer, then overlay modal stack.
        const int baseFocused = focusedIndexValue(window_);
        const bool baseEditMode = activeEditModeValue(window_);
        RenderedContent content = renderViewportContent(window_, state_, baseFocused, baseEditMode);
        const int height = static_cast<int>(content.size());
        const int width = height > 0 ? static_cast<int>(content.front().size()) : 0;
        if (width > 0 && height > 0
                && options_.windowStack != nullptr
                && !options_.windowStack->frames().empty()) {
            TerminalBuffer buffer(width, height);
            for (int r = 0; r < height; ++r) {
                const auto& row = content[static_cast<std::size_t>(r)];
                for (int c = 0; c < width && c < static_cast<int>(row.size()); ++c)
                    buffer.setCell(r, c, row[static_cast<std::size_t>(c)]);
            }
            renderWindowStackOverlay(buffer, *options_.windowStack);
            content = contentFromBuffer(buffer);
        }
        return JsonValue::Object{
            {"width",  static_cast<int>(content.empty() ? 0 : content.front().size())},
            {"height", static_cast<int>(content.size())},
            {"cells",  renderCompactCellsFromContent(content)},
        };
    }

    [[nodiscard]] JsonValue toolGetRenderSnapshot(const JsonValue::Object& arguments) const {
        GeneratedWindowBase& window = activeWindow();
        const int focusedIndex = focusedIndexValue(window);
        const bool editMode = activeEditModeValue(window);
        ScopedRenderTimeOverride renderTime(jsonInt64Field(arguments, "snapshot_time_ms"));
        JsonValue text = toolGetTextSnapshot();
        return JsonValue::Object{
            {"ansi_lines", text.object().at("lines")},
            {"text_lines", text.object().at("lines")},
            {"cells", renderCellsFromContent(toolRenderContent(window, focusedIndex, editMode))},
        };
    }

    [[nodiscard]] JsonValue toolGetRenderSnapshotCompact(const JsonValue::Object& arguments) const {
        GeneratedWindowBase& window = activeWindow();
        const int focusedIndex = focusedIndexValue(window);
        const bool editMode = activeEditModeValue(window);
        ScopedRenderTimeOverride renderTime(jsonInt64Field(arguments, "snapshot_time_ms"));
        return JsonValue::Object{
            {"format", "render-cells-v1"},
            {"cells", renderCompactCellsFromContent(toolRenderContent(window, focusedIndex, editMode))},
        };
    }

    [[nodiscard]] JsonValue toolGetViewport() const {
        GeneratedWindowBase& window = activeWindow();
        Rect rect = state_.viewport.value_or(Rect{0, 0, generatedWindowContentSize(window).width, generatedWindowContentSize(window).height});
        return JsonValue::Object{{"row", rect.row}, {"col", rect.col}, {"width", rect.width}, {"height", rect.height}};
    }

    [[nodiscard]] JsonValue toolGetRenderRect() const {
        return toolGetViewport();
    }

    [[nodiscard]] JsonValue toolSetViewport(const JsonValue::Object& arguments) {
        state_.viewport = Rect{
            std::max(0, jsonIntField(arguments, "row")),
            std::max(0, jsonIntField(arguments, "col")),
            std::max(1, jsonIntField(arguments, "width", 1)),
            std::max(1, jsonIntField(arguments, "height", 1)),
        };
        state_.fullRedrawRequested = true;
        return toolGetViewport();
    }

    [[nodiscard]] JsonValue toolRepaint() {
        const std::uint64_t generation = requestRedraw();
        waitForRender(generation);
        return JsonValue::Object{{"ok", true}};
    }

    [[nodiscard]] JsonValue toolFocusElement(const std::string& elementId) {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        Element* element = requireElement(elementId);
        focusActiveWindowElement(window, element);
        notifyMcpFocusElement(element);
        state_.fullRedrawRequested = true;
        return snapshot(*element);
    }

    [[nodiscard]] JsonValue toolEnterEditMode(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        if (const JsonValue* value = jsonField(arguments, "element_id"); value != nullptr && value->isString()) {
            Element* element = requireElement(value->string());
            focusActiveWindowElement(window, element);
            notifyMcpFocusElement(element);
        }
        int& focusedIndex = focusedIndexRef(window);
        bool& editMode = activeEditModeRef(window);
        if (enterScrollViewScope(window, focusedIndex, editMode, activeScrollView_, scrollViewLastDescendant_)) {
            activeScrollViewEditElement_ = nullptr;
            state_.activeScrollView = activeScrollView_;
            state_.activeScrollViewEditElement = activeScrollViewEditElement_;
            state_.fullRedrawRequested = true;
            return JsonValue::Object{{"edit_mode", activeEditModeValue(window)}};
        }
        activeScrollViewEditElement_ = nullptr;
        editMode = true;
        state_.activeScrollView = activeScrollView_;
        state_.activeScrollViewEditElement = activeScrollViewEditElement_;
        std::vector<Element*> focusable = focusableElements(window, activeScrollView_);
        Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
            ? focusable[static_cast<std::size_t>(focusedIndex)]
            : nullptr;
        if (focused != nullptr) {
            editSnapshot_ = captureSnapshot(focused);
            beginElementEdit(focused);
        }
        notifyMcpEditStarted(focused);
        state_.fullRedrawRequested = true;
        return JsonValue::Object{{"edit_mode", activeEditModeValue(window)}};
    }

    [[nodiscard]] JsonValue toolExitEditMode() {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        bool& editMode = activeEditModeRef(window);
        int& focusedIndex = focusedIndexRef(window);
        if (activeScrollView_ != nullptr) {
            exitScrollViewScope(window, focusedIndex, editMode, activeScrollView_, scrollViewLastDescendant_);
            activeScrollViewEditElement_ = nullptr;
            state_.activeScrollView = activeScrollView_;
            state_.activeScrollViewEditElement = activeScrollViewEditElement_;
            state_.fullRedrawRequested = true;
            return JsonValue::Object{{"edit_mode", activeEditModeValue(window)}};
        }
        std::vector<Element*> focusable = focusableElements(window);
        Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
            ? focusable[static_cast<std::size_t>(focusedIndex)]
            : nullptr;
        if (editMode && focused != nullptr) {
            if (usesLeaveCommit(focused)) {
                commitEdit(focused);
                notifyActiveFrameTextChanged(focused);
            } else {
                notifyActiveFrameTextConfirmed(focused);
                commitEdit(focused);
            }
        }
        editSnapshot_.reset();
        editMode = false;
        activeScrollViewEditElement_ = nullptr;
        state_.fullRedrawRequested = true;
        return JsonValue::Object{{"edit_mode", activeEditModeValue(window)}};
    }

    [[nodiscard]] JsonValue toolActivateElement(const std::string& elementId) {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        Element* element = requireElement(elementId);
        if (!element->enabled()) {
            return snapshot(*element);
        }
        focusActiveWindowElementWithScrollViewScope(window, element);
        notifyMcpFocusElement(element);
        if (isButton(*element)) {
            const std::string activatedName = element->name();
            GeneratedWindowBase* activatedWindow = &window;
            const bool hadActiveStackFrame = activeStackFrame() != nullptr;
            const std::vector<Element*> backgroundFocusable =
                hadActiveStackFrame ? focusableElements(window_, activeScrollViewForWindow(window_)) : std::vector<Element*>{};
            const FocusIdentity backgroundFocused = hadActiveStackFrame ? focusIdentityFor(window_, backgroundFocusable) : FocusIdentity{};
            if (activeStackFrame() != nullptr) {
                delayModalButtonActionForVisibleFocus();
            }
            activeEditModeRef(window) = false;
            if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
                owner != nullptr && owner != &window) {
                (void)owner->handleGeneratedButton(activatedName);
            } else {
                (void)handleActiveFrameButton(activatedName);
            }
            if (hadActiveStackFrame && &activeWindow() == &window_) {
                exitBackgroundEditModeAfterModalClose(window_);
                clearFocusIfElementRemoved(window_, backgroundFocused);
            }
            state_.fullRedrawRequested = true;
            if (&activeWindow() == activatedWindow) {
                if (Element* activeElement = findElement(activeWindow(), elementId)) {
                    focusActiveWindowElement(activeWindow(), activeElement);
                    return snapshot(*activeElement);
                }
            }
            else if (Element* activeElement = findElement(activeWindow(), elementId)) {
                return snapshot(*activeElement);
            }
            return JsonValue::Object{{"ok", true}, {"element_id", activatedName}};
        } else if (isClickableImage(*element)) {
            const std::string activatedName = element->name();
            activeEditModeRef(window) = false;
            if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
                owner != nullptr && owner != &window) {
                (void)owner->handleGeneratedButton(activatedName);
            } else {
                (void)handleActiveFrameButton(activatedName);
            }
            state_.fullRedrawRequested = true;
            if (Element* activeElement = findElement(activeWindow(), elementId)) {
                return snapshot(*activeElement);
            }
            return JsonValue::Object{{"ok", true}, {"element_id", activatedName}};
        } else if (activateReusableControl(element)) {
            activeEditModeRef(window) = false;
            state_.fullRedrawRequested = true;
            if (Element* activeElement = findElement(activeWindow(), elementId)) {
                focusActiveWindowElement(activeWindow(), activeElement);
                return snapshot(*activeElement);
            }
            return JsonValue::Object{{"ok", true}, {"element_id", element->name()}};
        } else if (auto* checkbox = dynamic_cast<CheckBox*>(element)) {
            const std::string activatedName = element->name();
            activeEditModeRef(window) = false;
            activeScrollViewEditElement_ = nullptr;
            state_.activeScrollViewEditElement = nullptr;
            (void)checkbox->handleKey("Enter");
            notifyActiveFrameTextChanged(element);
            state_.fullRedrawRequested = true;
            if (Element* activeElement = findElement(activeWindow(), elementId)) {
                focusActiveWindowElementWithScrollViewScope(activeWindow(), activeElement);
                activeEditModeRef(activeWindow()) = false;
                activeScrollViewEditElement_ = nullptr;
                state_.activeScrollView = activeScrollView_;
                state_.activeScrollViewEditElement = activeScrollViewEditElement_;
                return snapshot(*activeElement);
            }
            return JsonValue::Object{{"ok", true}, {"element_id", activatedName}};
        } else if (auto* comboBox = dynamic_cast<ComboBox*>(element)) {
            (void)comboBox->handleKey("Enter");
        } else if (auto* listBox = dynamic_cast<ListBox*>(element)) {
            (void)listBox->handleKey("Enter");
        } else if (isEditableElement(*element)) {
            activeEditModeRef(window) = true;
            notifyMcpEditStarted(element);
        }
        state_.fullRedrawRequested = true;
        return snapshot(*element);
    }

    [[nodiscard]] JsonValue toolClickElement(const std::string& elementId) {
        sleepMs(config_.actionDelayMs);
        refreshActiveWindowLayoutForMouse();
        Element* element = requireElement(elementId);
        if (!element->enabled()) {
            return snapshot(*element);
        }
        focusActiveWindowElementWithScrollViewScope(activeWindow(), element);
        if (Element* scopedElement = findElement(activeWindow(), elementId)) {
            element = scopedElement;
        }
        if (activateReusableControl(element)) {
            state_.fullRedrawRequested = true;
            if (Element* activeElement = findElement(activeWindow(), elementId)) {
                return snapshot(*activeElement);
            }
            return JsonValue::Object{{"ok", true}, {"element_id", elementId}};
        }
        const BackgroundFocusCleanupContext cleanupContext = captureBackgroundFocusCleanupContext();
        const Rect frame = element->frame();
        performMousePress(Point{frame.row + frame.height / 2, frame.col + frame.width / 2});
        cleanupBackgroundFocusAfterModalClose(cleanupContext);
        state_.fullRedrawRequested = true;
        if (Element* activeElement = findElement(activeWindow(), elementId)) {
            return snapshot(*activeElement);
        }
        return JsonValue::Object{{"ok", true}, {"element_id", elementId}};
    }

    [[nodiscard]] JsonValue toolSetChecked(const JsonValue::Object& arguments) {
        return toolSetChecked(arguments, jsonBoolField(arguments, "checked"));
    }

    [[nodiscard]] JsonValue toolSetChecked(const JsonValue::Object& arguments, bool checked) {
        sleepMs(config_.actionDelayMs);
        const std::string elementId = jsonStringField(arguments, "element_id");
        Element* element = requireElement(elementId);
        auto* checkbox = dynamic_cast<CheckBox*>(element);
        if (checkbox == nullptr) {
            throw std::runtime_error("Element is not a checkbox: " + element->name());
        }
        if (checkbox->checked() != checked) {
            checkbox->setChecked(checked);
            notifyActiveFrameTextChanged(element);
            state_.fullRedrawRequested = true;
            if (Element* activeElement = findElement(activeWindow(), elementId)) {
                return snapshot(*activeElement);
            }
            return JsonValue::Object{{"ok", true}, {"element_id", elementId}};
        }
        state_.fullRedrawRequested = true;
        return snapshot(*element);
    }

    void setOptionValue(Element* element, const std::string& value) {
        const std::optional<JsonValue::Array> optionValues = elementOptionsJson(*element);
        if (!optionValues.has_value()) {
            throw std::runtime_error("Element is not a supported selection control: " + element->name());
        }
        int nextIndex = -1;
        const auto* comboBox = dynamic_cast<ComboBox*>(element);
        const auto* listBox = dynamic_cast<ListBox*>(element);
        const std::vector<std::string>& options = comboBox != nullptr ? comboBox->options() : listBox->options();
        for (int index = 0; index < static_cast<int>(options.size()); ++index) {
            if (options[static_cast<std::size_t>(index)] == value) {
                nextIndex = index;
                break;
            }
        }
        if (nextIndex < 0) {
            throw std::runtime_error("Unknown option for " + element->name() + ": " + value);
        }
        const int previousIndex = selectedIndexOf(element);
        if (auto* mutableComboBox = dynamic_cast<ComboBox*>(element)) {
            mutableComboBox->setSelectedIndex(nextIndex);
        } else if (auto* mutableListBox = dynamic_cast<ListBox*>(element)) {
            mutableListBox->setSelectedIndex(nextIndex);
        }
        notifyActiveFrameSelectionChanged(element, previousIndex);
    }

    [[nodiscard]] JsonValue toolSetValue(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        const JsonValue* value = jsonField(arguments, "value");
        if (value == nullptr) {
            throw std::runtime_error("Missing value for: " + element->name());
        }
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            const std::string text = jsonTextField(arguments, "value");
            textInput->setValue(text);
            textInput->setCursor(static_cast<int>(textInput->value().size()));
            notifyActiveFrameTextChanged(element);
        } else if (auto* numberInput = dynamic_cast<NumberInput*>(element)) {
            const std::string text = jsonTextField(arguments, "value");
            numberInput->setValue(text.empty() ? 0.0 : std::stod(text));
            notifyActiveFrameTextChanged(element);
        } else if (auto* checkbox = dynamic_cast<CheckBox*>(element)) {
            if (!value->isBool()) {
                throw std::runtime_error("Checkbox value must be boolean: " + element->name());
            }
            if (checkbox->checked() != value->boolean()) {
                checkbox->setChecked(value->boolean());
                notifyActiveFrameTextChanged(element);
            }
        } else if (dynamic_cast<ComboBox*>(element) != nullptr) {
            setOptionValue(element, jsonTextField(arguments, "value"));
        } else if (dynamic_cast<ListBox*>(element) != nullptr) {
            if (value->isArray()) {
                return toolSetSelection(JsonValue::Object{{"element_id", element->name()}, {"values", *value}});
            } else {
                setOptionValue(element, jsonTextField(arguments, "value"));
            }
        } else {
            throw std::runtime_error("Element value cannot be set directly: " + element->name());
        }
        state_.fullRedrawRequested = true;
        return snapshot(*element);
    }

    [[nodiscard]] JsonValue toolSelectOption(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        const std::string value = jsonTextField(arguments, "value");
        setOptionValue(element, value);
        state_.fullRedrawRequested = true;
        return snapshot(*element);
    }

    [[nodiscard]] JsonValue toolSetSelection(const JsonValue::Object& arguments) {
        const std::string elementId = jsonStringField(arguments, "element_id");
        Element* element = requireElement(elementId);
        auto* listBox = dynamic_cast<ListBox*>(element);
        if (listBox == nullptr) {
            throw std::runtime_error("Element is not a listbox: " + element->name());
        }
        const JsonValue* values = jsonField(arguments, "values");
        if (values == nullptr || !values->isArray()) {
            throw std::runtime_error("ListBox selection values must be an array: " + element->name());
        }
        if (!listBox->multiple() && values->array().size() > 1) {
            throw std::runtime_error("ListBox is not multi-select: " + element->name());
        }
        const int previousSelectionIndex = selectedIndexOf(element);
        std::vector<std::string> requestedValues;
        const auto& options = listBox->options();
        for (const JsonValue& value : values->array()) {
            const std::string nextValue = value.isString() ? value.string() : jsonSerialize(value);
            if (std::find(options.begin(), options.end(), nextValue) == options.end()) {
                throw std::runtime_error("Unknown option for " + element->name() + ": " + nextValue);
            }
            requestedValues.push_back(nextValue);
        }
        std::vector<std::string> nextValues;
        for (const std::string& option : options) {
            if (std::find(requestedValues.begin(), requestedValues.end(), option) != requestedValues.end()) {
                nextValues.push_back(option);
            }
        }
        listBox->setSelectedValues(nextValues);
        notifyActiveFrameSelectionChanged(element, previousSelectionIndex);
        state_.fullRedrawRequested = true;
        return snapshot(*element);
    }

    [[nodiscard]] JsonValue toolPressKey(const std::string& key) {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        int& focusedIndex = focusedIndexRef(window);
        bool& editMode = activeEditModeRef(window);
        state_.activeScrollView = activeScrollView_;
        state_.activeScrollViewEditElement = activeScrollViewEditElement_;
        std::vector<Element*> focusable = focusableElements(window, activeScrollView_);
        Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
            ? focusable[static_cast<std::size_t>(focusedIndex)]
            : nullptr;
        if (key == "cmd_c") {
            (void)copyFocusedText(focused);
            state_.fullRedrawRequested = true;
            return toolGetState();
        }
        if (key == "cmd_v") {
            if (focused != nullptr && !editMode && isEditableElement(*focused)) {
                editSnapshot_ = captureSnapshot(focused);
                beginElementEdit(focused);
                editMode = true;
                notifyMcpEditStarted(focused);
            }
            if (pasteIntoFocused(focused, runtimeClipboardText())) {
                notifyActiveFrameTextChanged(focused);
            }
            state_.fullRedrawRequested = true;
            return toolGetState();
        }
        if (key == "Escape") {
            GeneratedWindowStackFrame* frame = activeStackFrame();
            if (frame != nullptr && frame->window == &window) {
                if (handleStandardEscapeButton(*frame, [this] {
                    delayModalButtonActionForVisibleFocus();
                })) {
                    state_.fullRedrawRequested = true;
                    return toolGetState();
                }
            } else if (handleStandardEscapeButton(window, window.mcpClassName(), focusedIndex, options_, [this] {
                           delayModalButtonActionForVisibleFocus();
                       })) {
                state_.fullRedrawRequested = true;
                return toolGetState();
            }
        }
        if (editMode && key == "Escape") {
            if (activeScrollView_ != nullptr) {
                if (activeScrollViewEditElement_ != nullptr) {
                    if (!usesLeaveCommit(activeScrollViewEditElement_)) {
                        if (editSnapshot_.has_value() &&
                            editSnapshot_->element == activeScrollViewEditElement_) {
                            restoreSnapshot(*editSnapshot_);
                        }
                    } else {
                        commitEdit(activeScrollViewEditElement_);
                    }
                    editSnapshot_.reset();
                    activeScrollViewEditElement_ = nullptr;
                } else {
                    exitScrollViewScope(window, focusedIndex, editMode, activeScrollView_, scrollViewLastDescendant_);
                }
            } else {
                escapeElementEdit(focused, editSnapshot_, editMode);
            }
            state_.fullRedrawRequested = true;
            state_.activeScrollView = activeScrollView_;
            state_.activeScrollViewEditElement = activeScrollViewEditElement_;
            return toolGetState();
        }
        if (handleActiveFrameKeyBeforeFocused(key, focused, editMode)) {
            state_.fullRedrawRequested = true;
            return toolGetState();
        }
        if (editMode && focused != nullptr) {
            if (activeScrollView_ != nullptr) {
                if (activeScrollViewEditElement_ != nullptr) {
                    if (key == "Enter") {
                        notifyActiveFrameTextConfirmed(activeScrollViewEditElement_);
                        commitEdit(activeScrollViewEditElement_);
                        editSnapshot_.reset();
                        activeScrollViewEditElement_ = nullptr;
                    } else if (isArrowKey(key)) {
                        const int previousSelectionIndex = selectedIndexOf(activeScrollViewEditElement_);
                        (void)handleElementKey(*activeScrollViewEditElement_, key);
                        notifyActiveFrameValueChangedAfterHandledKey(activeScrollViewEditElement_, previousSelectionIndex);
                    } else {
                        const int previousSelectionIndex = selectedIndexOf(activeScrollViewEditElement_);
                        (void)handleElementKey(*activeScrollViewEditElement_, key);
                        notifyActiveFrameValueChangedAfterHandledKey(activeScrollViewEditElement_, previousSelectionIndex);
                    }
                } else if (isArrowKey(key)) {
                    (void)moveScrollViewScopeFocus(window, focusedIndex, activeScrollView_, scrollViewLastDescendant_, key);
                } else if (key == "Enter" || key == " ") {
                    focusable = focusableElements(window, activeScrollView_);
                    focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
                        ? focusable[static_cast<std::size_t>(focusedIndex)]
                        : nullptr;
                    if (focused != nullptr && (isButton(*focused) || isClickableImage(*focused))) {
                        (void)handleActiveFrameButton(focused->name());
                    } else if (focused != nullptr && isImmediateInput(*focused)) {
                        const int previousSelectionIndex = selectedIndexOf(focused);
                        (void)handleElementKey(*focused, key);
                        notifyActiveFrameValueChangedAfterHandledKey(focused, previousSelectionIndex);
                    } else if (focused != nullptr && isEditableElement(*focused)) {
                        editSnapshot_ = captureSnapshot(focused);
                        beginElementEdit(focused);
                        activeScrollViewEditElement_ = focused;
                        notifyMcpEditStarted(focused);
                    }
                }
            } else if (key == "Enter" && usesLeaveCommit(focused)) {
                const int previousSelectionIndex = selectedIndexOf(focused);
                (void)handleElementKey(*focused, key);
                notifyActiveFrameValueChangedAfterHandledKey(focused, previousSelectionIndex);
                editSnapshot_.reset();
            } else if (key == "Enter" && !usesLeaveCommit(focused)) {
                (void)handleElementKey(*focused, key);
                notifyActiveFrameTextConfirmed(focused);
                commitEdit(focused);
                editMode = options_.keepEditModeAfterConfirm && isEditableElement(*focused);
                if (!editMode) {
                    editSnapshot_.reset();
                }
            } else {
                const int previousSelectionIndex = selectedIndexOf(focused);
                (void)handleElementKey(*focused, key);
                notifyActiveFrameValueChangedAfterHandledKey(focused, previousSelectionIndex);
            }
        } else if (handleActiveFrameKey(key)) {
        } else if (key == "Tab") {
            moveFocus(focusedIndex, focusable, 1);
        } else if (key == "Shift+Tab") {
            moveFocus(focusedIndex, focusable, -1);
        } else if (isArrowKey(key)) {
            moveFocusSpatial(focusedIndex, focusable, key);
        } else if (focused != nullptr && (key == "Enter" || key == " ")) {
            const bool hadActiveStackFrame = isButton(*focused) && activeStackFrame() != nullptr;
            const std::vector<Element*> backgroundFocusable =
                hadActiveStackFrame ? focusableElements(window_, activeScrollViewForWindow(window_)) : std::vector<Element*>{};
            const FocusIdentity backgroundFocused = hadActiveStackFrame ? focusIdentityFor(window_, backgroundFocusable) : FocusIdentity{};
            if (isButton(*focused) && activeStackFrame() != nullptr) {
                delayModalButtonActionForVisibleFocus();
            }
            if (isButton(*focused) || isClickableImage(*focused)) {
                editMode = false;
            }
            if (isButton(*focused)) {
                (void)handleActiveFrameButton(focused->name());
                if (hadActiveStackFrame && &activeWindow() == &window_) {
                    exitBackgroundEditModeAfterModalClose(window_);
                    clearFocusIfElementRemoved(window_, backgroundFocused);
                }
            } else if (isClickableImage(*focused)) {
                (void)handleActiveFrameButton(focused->name());
            } else if (activateReusableControl(focused)) {
                editMode = false;
            } else if (isEditableElement(*focused)) {
                if (!enterScrollViewScope(window, focusedIndex, editMode, activeScrollView_, scrollViewLastDescendant_)) {
                    editSnapshot_ = captureSnapshot(focused);
                    beginElementEdit(focused);
                    editMode = true;
                    notifyMcpEditStarted(focused);
                } else {
                    activeScrollViewEditElement_ = nullptr;
                    editSnapshot_.reset();
                }
            }
        } else if (focused != nullptr && isImmediateInput(*focused)) {
            const int previousSelectionIndex = selectedIndexOf(focused);
            (void)handleElementKey(*focused, key);
            notifyActiveFrameValueChangedAfterHandledKey(focused, previousSelectionIndex);
        }
        state_.fullRedrawRequested = true;
        state_.activeScrollView = activeScrollView_;
        state_.activeScrollViewEditElement = activeScrollViewEditElement_;
        return toolGetState();
    }

    [[nodiscard]] JsonValue toolSetText(const JsonValue::Object& arguments, bool paste) {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        const std::string text = jsonTextField(arguments, "text");
        focusActiveWindowElement(window, element);
        notifyMcpFocusElement(element);
        bool& editMode = activeEditModeRef(window);
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            if (paste) {
                editMode = true;
                notifyEditStarted(options_, element);
                notifyMcpEditStarted(element);
                textInput->insertText(text);
                notifyActiveFrameTextChanged(element);
                markTextProgress();
            } else {
                editMode = true;
                notifyEditStarted(options_, element);
                notifyMcpEditStarted(element);
                textInput->setValue("");
                textInput->setCursor(0);
                notifyActiveFrameTextChanged(element);
                markTextProgress();
                for (char ch : text) {
                    sleepMs(config_.typeDelayMs);
                    textInput->insertText(std::string_view(&ch, 1));
                    notifyActiveFrameTextChanged(element);
                    markTextProgress();
                }
                textInput->setValue(text);
                textInput->setCursor(static_cast<int>(text.size()));
                notifyActiveFrameTextChanged(element);
                markTextProgress();
            }
        } else if (auto* numberInput = dynamic_cast<NumberInput*>(element)) {
            if (paste) {
                editMode = true;
                notifyEditStarted(options_, element);
                notifyMcpEditStarted(element);
                numberInput->setValue(text.empty() ? 0.0 : std::stod(text));
                notifyActiveFrameTextChanged(element);
                markTextProgress();
            } else {
                editMode = true;
                notifyEditStarted(options_, element);
                notifyMcpEditStarted(element);
                numberInput->setEditText("");
                notifyActiveFrameTextChanged(element);
                markTextProgress();
                for (char ch : text) {
                    sleepMs(config_.typeDelayMs);
                    (void)numberInput->handleKey(std::string_view(&ch, 1));
                    notifyActiveFrameTextChanged(element);
                    markTextProgress();
                }
                numberInput->commitEdit();
                numberInput->setValue(text.empty() ? 0.0 : std::stod(text));
                numberInput->setEditText(text);
                notifyActiveFrameTextChanged(element);
                markTextProgress();
            }
        } else {
            throw std::runtime_error("Element is not editable text: " + element->name());
        }
        state_.fullRedrawRequested = true;
        return snapshot(*element);
    }

    [[nodiscard]] JsonValue toolTypeText(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        const std::string text = jsonTextField(arguments, "text");
        focusActiveWindowElement(window, element);
        notifyMcpFocusElement(element);
        bool& editMode = activeEditModeRef(window);
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            editMode = true;
            notifyEditStarted(options_, element);
            notifyMcpEditStarted(element);
            for (char ch : text) {
                sleepMs(config_.typeDelayMs);
                textInput->insertText(std::string_view(&ch, 1));
                notifyActiveFrameTextChanged(element);
                markTextProgress();
            }
            state_.fullRedrawRequested = true;
            return snapshot(*element);
        }
        if (auto* numberInput = dynamic_cast<NumberInput*>(element)) {
            editMode = true;
            notifyEditStarted(options_, element);
            notifyMcpEditStarted(element);
            numberInput->beginEdit();
            for (char ch : text) {
                sleepMs(config_.typeDelayMs);
                (void)numberInput->handleKey(std::string_view(&ch, 1));
                notifyActiveFrameTextChanged(element);
                markTextProgress();
            }
            state_.fullRedrawRequested = true;
            return snapshot(*element);
        }
        throw std::runtime_error("Element is not editable text: " + element->name());
    }

    [[nodiscard]] JsonValue toolReplaceSelection(const JsonValue::Object& arguments) {
        return toolTypeText(arguments);
    }

    [[nodiscard]] JsonValue toolClearText(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        focusActiveWindowElement(window, element);
        notifyMcpFocusElement(element);
        activeEditModeRef(window) = true;
        notifyMcpEditStarted(element);
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            textInput->selectRange(0, static_cast<int>(textInput->value().size()));
            markTextProgress();
            sleepMs(config_.actionDelayMs);
            (void)textInput->handleKey("Backspace");
            notifyActiveFrameTextChanged(element);
            markTextProgress();
            return snapshot(*element);
        }
        if (auto* numberInput = dynamic_cast<NumberInput*>(element)) {
            numberInput->setEditText("");
            notifyActiveFrameTextChanged(element);
            markTextProgress();
            return snapshot(*element);
        }
        throw std::runtime_error("Element is not editable text: " + element->name());
    }

    [[nodiscard]] JsonValue toolMoveCursor(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        GeneratedWindowBase& window = activeWindow();
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        focusActiveWindowElement(window, element);
        notifyMcpFocusElement(element);
        activeEditModeRef(window) = true;
        notifyMcpEditStarted(element);
        const int count = std::max(1, jsonIntField(arguments, "count", 1));
        const std::string direction = jsonStringField(arguments, "direction");
        const std::string key = cursorKey(direction);
        if (key.empty()) {
            throw std::runtime_error("Unsupported cursor direction: " + direction);
        }
        for (int index = 0; index < count; ++index) {
            (void)handleElementKey(*element, key);
        }
        state_.fullRedrawRequested = true;
        return cursorSnapshot(*element);
    }

    [[nodiscard]] JsonValue toolSetCursor(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            const int offset = jsonIntField(arguments, "offset");
            textInput->selectRange(offset, offset);
            state_.fullRedrawRequested = true;
            return JsonValue::Object{{"cursor", textInput->cursor()}};
        }
        return JsonValue::Object{{"cursor", nullptr}};
    }

    [[nodiscard]] JsonValue toolSelectText(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            const int start = jsonIntField(arguments, "start");
            const int end = jsonIntField(arguments, "end");
            textInput->selectRange(start, end);
            state_.fullRedrawRequested = true;
            return JsonValue::Object{
                {"start", std::min(start, end)},
                {"end", std::max(start, end)},
                {"text", textInput->selectedText()},
            };
        }
        return JsonValue::Object{{"start", nullptr}, {"end", nullptr}, {"text", ""}};
    }

    [[nodiscard]] JsonValue toolSelectAll(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            textInput->selectRange(0, static_cast<int>(textInput->value().size()));
            state_.fullRedrawRequested = true;
            return selectionSnapshot(*textInput);
        }
        return JsonValue::Object{{"start", nullptr}, {"end", nullptr}, {"text", ""}};
    }

    [[nodiscard]] JsonValue toolCopySelection(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            const std::string selected = textInput->selectedText();
            (void)copyTextToClipboard(selected);
            state_.fullRedrawRequested = true;
            return JsonValue::Object{{"text", selected}};
        }
        return JsonValue::Object{{"text", ""}};
    }

    [[nodiscard]] JsonValue toolCutSelection(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        if (auto* textInput = dynamic_cast<TextInput*>(element)) {
            const std::string selected = textInput->selectedText();
            if (!selected.empty()) {
                (void)textInput->handleKey("Backspace");
                notifyActiveFrameTextChanged(element);
            }
            state_.fullRedrawRequested = true;
            return JsonValue::Object{{"text", selected}};
        }
        return JsonValue::Object{{"text", ""}};
    }

    [[nodiscard]] JsonValue toolGetText(const JsonValue::Object& arguments) {
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        if (const auto* textInput = dynamic_cast<const TextInput*>(element)) {
            return JsonValue::Object{{"text", textInput->value()}};
        }
        if (const auto* numberInput = dynamic_cast<const NumberInput*>(element)) {
            return JsonValue::Object{{"text", numberInput->displayValue()}};
        }
        return JsonValue::Object{{"text", ""}};
    }

    [[nodiscard]] JsonValue toolGetSelection(const JsonValue::Object& arguments) {
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        if (const auto* textInput = dynamic_cast<const TextInput*>(element)) {
            return selectionSnapshot(*textInput);
        }
        return JsonValue::Object{{"start", nullptr}, {"end", nullptr}, {"text", ""}};
    }

    [[nodiscard]] JsonValue toolGetCursor(const JsonValue::Object& arguments) {
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        return cursorSnapshot(*element);
    }

    [[nodiscard]] JsonValue toolMouseClick(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        refreshActiveWindowLayoutForMouse();
        const BackgroundFocusCleanupContext cleanupContext = captureBackgroundFocusCleanupContext();
        performMousePress(windowPointFromTerminalPoint(Point{
            jsonIntField(arguments, "y"),
            jsonIntField(arguments, "x"),
        }));
        cleanupBackgroundFocusAfterModalClose(cleanupContext);
        state_.fullRedrawRequested = true;
        return toolGetState();
    }

    [[nodiscard]] JsonValue toolMouseDrag(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        refreshActiveWindowLayoutForMouse();
        Element* mouseSelectionElement = nullptr;
        int mouseSelectionAnchor = 0;
        std::optional<EditSnapshot> editSnapshot;
        Point from = windowPointFromTerminalPoint(Point{
            jsonIntField(arguments, "from_y"),
            jsonIntField(arguments, "from_x"),
        });
        Point to = windowPointFromTerminalPoint(Point{
            jsonIntField(arguments, "to_y"),
            jsonIntField(arguments, "to_x"),
        });
        const BackgroundFocusCleanupContext cleanupContext = captureBackgroundFocusCleanupContext();
        MouseClickCandidate mouseClickCandidate;
        performMousePress(from, &editSnapshot, &mouseSelectionElement, &mouseSelectionAnchor, &mouseClickCandidate);
        cleanupBackgroundFocusAfterModalClose(cleanupContext);
        if (mouseClickCandidate.element != nullptr) {
            mouseClickCandidate.moved = true;
        }
        (void)handleMouseDrag(mouseSelectionElement, mouseSelectionAnchor, to);
        state_.fullRedrawRequested = true;
        return toolGetState();
    }

    [[nodiscard]] JsonValue toolScroll(const JsonValue::Object& arguments) {
        sleepMs(config_.actionDelayMs);
        Element* element = requireElement(jsonStringField(arguments, "element_id"));
        const int delta = jsonIntField(arguments, "delta");
        ScrollView* scrollView = dynamic_cast<ScrollView*>(element);
        if (scrollView == nullptr) {
            if (auto* reusable = dynamic_cast<ReusableElement*>(element)) {
                const Rect frame = reusable->frame();
                if (frame.width > 0 && frame.height > 0) {
                    syncReusableChildFrames(*reusable, frame);
                }
                scrollView = generatedScrollViewForReusable(*reusable);
            }
        }
        if (scrollView != nullptr) {
            (void)scrollView->scrollLines(delta, Size{scrollView->frame().width, scrollView->frame().height});
            (void)scrollView->consumeTerminalScrollDelta();
            state_.fullRedrawRequested = true;
            return snapshot(*element);
        }
        const int steps = std::max(1, std::abs(delta));
        const int scrollDelta = delta < 0 ? 1 : -1;
        for (int index = 0; index < steps; ++index) {
            if (auto* listBox = dynamic_cast<ListBox*>(element)) {
                listBox->scrollBy(scrollDelta, listBox->frame().height);
            } else if (auto* textInput = dynamic_cast<TextInput*>(element)) {
                (void)textInput->scrollByRows(scrollDelta * kTextInputWheelScrollRows, textInput->frame().height);
            } else {
                throw std::runtime_error("Element is not scrollable: " + element->name());
            }
        }
        state_.fullRedrawRequested = true;
        return snapshot(*element);
    }

    [[nodiscard]] Point windowPointFromTerminalPoint(Point point) const {
        if (state_.viewport.has_value()) {
            point.row -= state_.viewport->row;
            point.col -= state_.viewport->col;
        }
        const Size viewportSize = viewportSizeForStackBounds();
        TerminalBuffer buffer(viewportSize.width, viewportSize.height);
        if (const GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr && frame->window != nullptr) {
            GeneratedWindowStackFrame* mutableFrame = activeStackFrame();
            if (mutableFrame != nullptr) {
                const Rect frameBounds = refreshWindowStackFrameBounds(*mutableFrame, buffer);
                point = stackFrameContentPoint(*frame, frameBounds, point);
            }
        } else {
            point = windowContentPoint(activeWindow(), buffer, point);
        }
        return point;
    }

    bool performMousePress(Point position, std::optional<EditSnapshot>* editSnapshotArg = nullptr,
                           Element** mouseSelectionElementArg = nullptr, int* mouseSelectionAnchorArg = nullptr,
                           MouseClickCandidate* mouseClickCandidateArg = nullptr) {
        GeneratedWindowBase& window = activeWindow();
        Element* localMouseSelectionElement = nullptr;
        int localMouseSelectionAnchor = 0;
        Element*& mouseSelectionElement = mouseSelectionElementArg == nullptr ? localMouseSelectionElement : *mouseSelectionElementArg;
        int& mouseSelectionAnchor = mouseSelectionAnchorArg == nullptr ? localMouseSelectionAnchor : *mouseSelectionAnchorArg;

        if (GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr && frame->window == &window) {
            std::optional<EditSnapshot>& editSnapshot = editSnapshotArg == nullptr ? frame->editSnapshot : *editSnapshotArg;
            if (frame->options.onMousePressBeforeFocused && frame->options.onMousePressBeforeFocused(position)) {
                return true;
            }
            GeneratedWindowRuntimeOptions frameOptions = runtimeOptionsForFrame(frame->options);
            if (pointOutsideActiveScrollView(frame->activeScrollView, position)) {
                exitScrollViewScope(*frame->window, frame->focusedIndex, frame->editMode,
                                    frame->activeScrollView, frame->scrollViewLastDescendant);
                frame->activeScrollViewEditElement = nullptr;
                frame->editSnapshot.reset();
            }
            Element* mouseScrollView = frame->activeScrollView;
            if (mouseScrollView == nullptr) {
                mouseScrollView = scrollViewAtPosition(window, position);
            }
            std::vector<Element*> focusable = focusableElements(window, mouseScrollView);
            std::vector<Element*> mouseTargets = mouseTargetElements(window, mouseScrollView);
            Element* focused = focusedElementFor(window, focusable);
            Element* target = mouseTargetElement(
                mouseTargets,
                focused,
                frame->editMode,
                position,
                frame->activeScrollViewEditElement
            );
            if (mouseScrollView != nullptr && target != nullptr && target != mouseScrollView) {
                frame->activeScrollView = mouseScrollView;
                frame->editMode = true;
                frame->focusedIndex = indexOfElement(focusable, target);
            }
            if (closeComboBoxOnOutsideClick(focused, frame->editMode, editSnapshot, target)) {
                frame->activeScrollViewEditElement = nullptr;
                (void)clearFocusForEmptyMouseTarget(*frame->window, frameOptions, frame->focusedIndex,
                                                    editSnapshot, frame->editMode, frame->activeScrollView,
                                                    frame->activeScrollViewEditElement, focusable);
                state_.fullRedrawRequested = true;
                return true;
            }
            const bool handled = handleMousePress(*frame->window, frameOptions, frame->focusedIndex, editSnapshot,
                                                  frame->editMode, frame->activeScrollView,
                                                  frame->activeScrollViewEditElement, mouseSelectionElement,
                                                  mouseSelectionAnchor, mouseClickCandidateArg, focusable, target,
                                                  position);
            state_.fullRedrawRequested = state_.fullRedrawRequested || handled;
            return handled;
        }

        if (options_.onMousePressBeforeFocused && options_.onMousePressBeforeFocused(position)) {
            return true;
        }
        int& focusedIndex = focusedIndexRef(window);
        bool& editMode = activeEditModeRef(window);
        std::optional<EditSnapshot>& editSnapshot = editSnapshotArg == nullptr ? editSnapshot_ : *editSnapshotArg;
        if (pointOutsideActiveScrollView(activeScrollView_, position)) {
            exitScrollViewScope(window, focusedIndex, editMode, activeScrollView_, scrollViewLastDescendant_);
            activeScrollViewEditElement_ = nullptr;
            editSnapshot.reset();
        }
        Element* mouseScrollView = activeScrollView_;
        if (mouseScrollView == nullptr) {
            mouseScrollView = scrollViewAtPosition(window, position);
        }
        std::vector<Element*> focusable = focusableElements(window, mouseScrollView);
        std::vector<Element*> mouseTargets = mouseTargetElements(window, mouseScrollView);
        Element* focused = focusedElementFor(window, focusable);
        Element* target = mouseTargetElement(
            mouseTargets,
            focused,
            editMode,
            position,
            activeScrollViewEditElement_
        );
        if (mouseScrollView != nullptr && target != nullptr && target != mouseScrollView) {
            activeScrollView_ = mouseScrollView;
            editMode = true;
            focusedIndex = indexOfElement(focusable, target);
        }
        if (closeComboBoxOnOutsideClick(focused, editMode, editSnapshot, target)) {
            (void)clearFocusForEmptyMouseTarget(window, options_, focusedIndex, editSnapshot, editMode,
                                                activeScrollView_, activeScrollViewEditElement_, focusable);
            state_.activeScrollView = activeScrollView_;
            state_.activeScrollViewEditElement = activeScrollViewEditElement_;
            state_.fullRedrawRequested = true;
            return true;
        }
        const bool handled = handleMousePress(window, options_, focusedIndex, editSnapshot, editMode,
                                             activeScrollView_, activeScrollViewEditElement_, mouseSelectionElement,
                                             mouseSelectionAnchor, mouseClickCandidateArg, focusable, target,
                                             position);
        state_.activeScrollView = activeScrollView_;
        state_.activeScrollViewEditElement = activeScrollViewEditElement_;
        state_.fullRedrawRequested = state_.fullRedrawRequested || handled;
        return handled;
    }

    [[nodiscard]] static std::string cursorKey(const std::string& direction) {
        if (direction == "left" || direction == "Left") return "Left";
        if (direction == "right" || direction == "Right") return "Right";
        if (direction == "up" || direction == "Up") return "Up";
        if (direction == "down" || direction == "Down") return "Down";
        if (direction == "home" || direction == "Home") return "Home";
        if (direction == "end" || direction == "End") return "End";
        return "";
    }

    [[nodiscard]] static JsonValue selectionSnapshot(const TextInput& textInput) {
        const std::string selected = textInput.selectedText();
        if (selected.empty()) {
            return JsonValue::Object{{"start", nullptr}, {"end", nullptr}, {"text", ""}};
        }
        return JsonValue::Object{
            {"start", textInput.selectionStart()},
            {"end", textInput.selectionEnd()},
            {"text", selected},
        };
    }

    [[nodiscard]] static JsonValue cursorSnapshot(const Element& element) {
        if (const auto* textInput = dynamic_cast<const TextInput*>(&element)) {
            return JsonValue::Object{{"cursor", textInput->cursor()}};
        }
        return JsonValue::Object{{"cursor", nullptr}};
    }

    [[nodiscard]] JsonValue snapshot(const Element& element) const {
        GeneratedWindowBase& window = activeWindow();
        GeneratedWindowBase* metadataWindow = ownerWindowForElement(window, &element);
        if (metadataWindow == nullptr) {
            metadataWindow = &window;
        }
        const std::vector<Element*> focusable = focusableElements(window, state_.activeScrollView);
        const int focusedIndex = focusedIndexValue(window);
        const bool editMode = activeEditModeValue(window);
        const Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
            ? focusable[static_cast<std::size_t>(focusedIndex)]
            : nullptr;
        const std::optional<std::string> elementId = elementIdForElement(window, &element);
        JsonValue::Object result{
            {"id", elementId.value_or(element.name())},
            {"type", elementType(element)},
            {"role", elementRole(element)},
            {"description", elementDescription(*metadataWindow, element)},
            {"focused", focused == &element},
            {"visible", true},
            {"enabled", element.enabled()},
            {"bounds", rectJson(element.frame())},
            {"style_state", focused == &element ? (editMode ? "edit" : "focus") : "base"},
            {"value", elementValueJson(element)},
        };
        if (std::optional<JsonValue::Array> options = elementOptionsJson(element); options.has_value()) {
            result["options"] = JsonValue(std::move(*options));
        }
        if (const auto* textInput = dynamic_cast<const TextInput*>(&element)) {
            result["cursor"] = textInput->cursor();
            result["selection"] = selectionSnapshot(*textInput);
        }
        return result;
    }

    [[nodiscard]] Element* requireElement(const std::string& elementId) const {
        if (Element* element = findElement(activeWindow(), elementId)) {
            return element;
        }
        throw std::runtime_error("Unknown element: " + elementId);
    }

    static void sleepMs(int milliseconds) {
        if (milliseconds > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }
    }

    void notifyMcpFocusElement(Element* element) {
        (void)element;
    }

    void notifyMcpEditStarted(Element* element) {
        if (element == nullptr) {
            return;
        }
        if (GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr && frame->options.onEditStarted) {
            frame->options.onEditStarted(element->name());
            return;
        }
        if (&activeWindow() == &window_) {
            notifyEditStarted(options_, element);
        }
    }

    [[nodiscard]] bool handleActiveFrameButton(std::string_view name) {
        if (GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr && frame->options.onButton) {
            frame->options.onButton(name);
            return true;
        }
        if (options_.onButton) {
            options_.onButton(name);
            return true;
        }
        return false;
    }

    [[nodiscard]] bool handleActiveFrameKeyBeforeFocused(std::string_view key, Element* focused, bool editMode) {
        const std::string_view focusedName = focused == nullptr ? std::string_view{} : std::string_view(focused->name());
        if (GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr) {
            if (frame->options.onKeyBeforeFocusedElement && frame->options.onKeyBeforeFocusedElement(key, focusedName, editMode)) {
                return true;
            }
            if (frame->options.onKeyBeforeFocused && frame->options.onKeyBeforeFocused(key)) {
                return true;
            }
            return false;
        }
        if (options_.onKeyBeforeFocusedElement && options_.onKeyBeforeFocusedElement(key, focusedName, editMode)) {
            return true;
        }
        return options_.onKeyBeforeFocused && options_.onKeyBeforeFocused(key);
    }

    [[nodiscard]] bool handleActiveFrameKey(std::string_view key) {
        if (GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr && frame->options.onKey) {
            return frame->options.onKey(key);
        }
        return options_.onKey && options_.onKey(key);
    }

    void notifyActiveFrameTextChanged(Element* element) {
        if (element == nullptr) {
            return;
        }
        GeneratedWindowBase& window = activeWindow();
        if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
            owner != nullptr && owner != &window) {
            (void)owner->handleGeneratedTextChanged(element->name(), textValueOf(element));
            return;
        }
        if (GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr && frame->options.onTextChanged) {
            GeneratedWindowRuntimeOptions notifyOptions;
            notifyOptions.onTextChanged = frame->options.onTextChanged;
            notifyTextChanged(notifyOptions, element);
            return;
        }
        notifyTextChanged(options_, element);
    }

    void notifyActiveFrameTextConfirmed(Element* element) {
        if (element == nullptr) {
            return;
        }
        GeneratedWindowBase& window = activeWindow();
        if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
            owner != nullptr && owner != &window) {
            (void)owner->handleGeneratedTextConfirmed(element->name(), textValueOf(element));
            return;
        }
        if (GeneratedWindowStackFrame* frame = activeStackFrame(); frame != nullptr && frame->options.onTextConfirmed) {
            GeneratedWindowRuntimeOptions notifyOptions;
            notifyOptions.onTextConfirmed = frame->options.onTextConfirmed;
            notifyTextConfirmed(notifyOptions, element);
            return;
        }
        notifyTextConfirmed(options_, element);
    }

    void notifyActiveFrameSelectionChanged(Element* element, int previousSelectionIndex) {
        if (element == nullptr) {
            return;
        }
        GeneratedWindowBase& window = activeWindow();
        if (GeneratedWindowBase* owner = ownerWindowForElement(window, element);
            owner != nullptr && owner != &window) {
            if (selectedIndexOf(element) != previousSelectionIndex) {
                (void)owner->handleGeneratedSelectionChanged(element->name(), selectedValuesOf(element));
            }
            return;
        }
        if (GeneratedWindowStackFrame* frame = activeStackFrame();
            frame != nullptr && (frame->options.onSelectionChanged || frame->options.onTextChanged)) {
            GeneratedWindowRuntimeOptions notifyOptions;
            notifyOptions.onSelectionChanged = frame->options.onSelectionChanged;
            notifyOptions.onTextChanged = frame->options.onTextChanged;
            notifySelectionChanged(notifyOptions, element, previousSelectionIndex);
            return;
        }
        notifySelectionChanged(options_, element, previousSelectionIndex);
    }

    void notifyActiveFrameValueChangedAfterHandledKey(Element* element, int previousSelectionIndex) {
        if (isSelectionElement(element)) {
            notifyActiveFrameSelectionChanged(element, previousSelectionIndex);
        } else {
            notifyActiveFrameTextChanged(element);
        }
    }

    std::uint64_t requestRedraw() {
        std::lock_guard lock(state_.mutex);
        const std::uint64_t generation = state_.renderGeneration;
        state_.fullRedrawRequested = true;
        return generation;
    }

    void waitForRender(std::uint64_t generation) {
        if (!config_.waitRender || !config_.gui) {
            return;
        }
        std::unique_lock lock(state_.mutex);
        state_.renderCondition.wait_for(lock, kMcpRenderWaitTimeout, [&] {
            return state_.renderGeneration != generation;
        });
    }

    void delayModalButtonActionForVisibleFocus() {
        if (!config_.gui || &activeWindow() == &window_) {
            return;
        }
        ScopedUiMutationLock* scopedLock = currentUiLock;
        const bool shouldUnlockUi = scopedLock != nullptr &&
            scopedLock->lock != nullptr &&
            scopedLock->lock->owns_lock();
        if (shouldUnlockUi) {
            scopedLock->lock->unlock();
        }
        const std::uint64_t generation = requestRedraw();
        waitForRender(generation);
        sleepMs(kDialogButtonCloseDelayMs);
        if (shouldUnlockUi) {
            scopedLock->lock->lock();
        }
    }

    void markTextProgress() {
        const std::uint64_t generation = requestRedraw();
        waitForRender(generation);
    }

    GeneratedWindowBase& window_;
    GeneratedWindowRuntimeOptions& options_;
    McpRuntimeState& state_;
    McpRuntimeConfig config_;
    Element* activeScrollView_ = nullptr;
    Element* activeScrollViewEditElement_ = nullptr;
    std::optional<EditSnapshot> editSnapshot_;
    std::unordered_map<Element*, Element*> scrollViewLastDescendant_;
};

thread_local McpController::ScopedUiMutationLock* McpController::currentUiLock = nullptr;

[[nodiscard]] std::string lowerAscii(std::string text) {
    for (char& ch : text) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return text;
}

[[nodiscard]] bool containsText(std::string_view text, std::string_view needle) {
    return text.find(needle) != std::string_view::npos;
}

[[nodiscard]] bool mcpHttpAcceptAllowed(std::string_view accept) {
    return accept.empty()
        || containsText(accept, "*/*")
        || containsText(accept, kHttpApplicationJson)
        || containsText(accept, kHttpTextEventStream);
}

[[nodiscard]] bool mcpHttpProtocolAllowed(std::string_view protocol) {
    return protocol.empty()
        || protocol == kMcpProtocolVersion
        || protocol == "2025-03-26"
        || protocol == "2025-06-18";
}

[[nodiscard]] bool mcpHttpOriginAllowed(std::string_view origin) {
    return origin.empty()
        || origin == "null"
        || origin.starts_with("http://127.0.0.1")
        || origin.starts_with("http://localhost")
        || origin.starts_with("https://127.0.0.1")
        || origin.starts_with("https://localhost");
}

class McpJsonRpcDispatcher {
public:
    explicit McpJsonRpcDispatcher(McpController& controller) : controller_(controller) {}

    [[nodiscard]] JsonValue handleRequest(const JsonValue& request) {
        const auto& object = request.object();
        const std::string method = jsonStringField(object, "method");
        const JsonValue* paramsValue = jsonField(object, "params");
        const JsonValue::Object emptyParams;
        const JsonValue::Object& params = paramsValue != nullptr && paramsValue->isObject()
            ? paramsValue->object()
            : emptyParams;

        if (method == "initialize") {
            return JsonValue::Object{
                {"protocolVersion", std::string(kMcpProtocolVersion)},
                {"serverInfo", JsonValue::Object{{"name", "ui-cpp-runtime"}, {"version", "0.1"}}},
                {"capabilities", JsonValue::Object{{"tools", JsonValue::Object{}}}},
            };
        }
        if (method == "tools/list") {
            return controller_.listTools();
        }
        if (method == "tools/call") {
            const std::string name = jsonStringField(params, "name");
            const JsonValue* argumentsValue = jsonField(params, "arguments");
            const JsonValue::Object emptyArguments;
            const JsonValue::Object& arguments = argumentsValue != nullptr && argumentsValue->isObject()
                ? argumentsValue->object()
                : emptyArguments;
            const std::string text = jsonSerialize(controller_.callTool(name, arguments));
            return JsonValue::Object{{"content", JsonValue::Array{JsonValue::Object{{"type", "text"}, {"text", text}}}}};
        }
        if (method.starts_with("ui.")) {
            return controller_.callTool(method.substr(3), params);
        }
        return controller_.callTool(method, params);
    }

    [[nodiscard]] std::string handleLine(std::string_view line) {
        JsonValue request;
        try {
            request = JsonParser(line).parse();
            std::optional<JsonValue> response = handleJsonRpcValue(request);
            if (!response.has_value()) {
                return {};
            }
            return jsonSerialize(*response) + "\n";
        } catch (const std::exception& exc) {
            return jsonSerialize(jsonRpcError(nullptr, exc.what())) + "\n";
        }
    }

private:
    [[nodiscard]] std::optional<JsonValue> handleJsonRpcValue(const JsonValue& request) {
        if (request.isArray()) {
            if (request.array().empty()) {
                return jsonRpcError(nullptr, "Invalid JSON-RPC batch: empty array");
            }
            JsonValue::Array responses;
            for (const JsonValue& item : request.array()) {
                if (!item.isObject()) {
                    responses.push_back(jsonRpcError(nullptr, "JSON-RPC request must be an object"));
                    continue;
                }
                std::optional<JsonValue> response = handleJsonRpcObject(item);
                if (response.has_value()) {
                    responses.push_back(std::move(*response));
                }
            }
            if (responses.empty()) {
                return std::nullopt;
            }
            return JsonValue(std::move(responses));
        }
        if (!request.isObject()) {
            throw std::runtime_error("JSON-RPC request must be an object");
        }
        return handleJsonRpcObject(request);
    }

    [[nodiscard]] std::optional<JsonValue> handleJsonRpcObject(const JsonValue& request) {
        JsonValue id = nullptr;
        bool hasId = false;
        try {
            if (const JsonValue* idValue = jsonField(request.object(), "id")) {
                id = *idValue;
                hasId = true;
            }
            const JsonValue result = handleRequest(request);
            if (!hasId) {
                return std::nullopt;
            }
            return JsonValue::Object{
                {"jsonrpc", std::string(kJsonRpcVersion)},
                {"id", id},
                {"result", result},
            };
        } catch (const std::exception& exc) {
            return jsonRpcError(hasId ? id : JsonValue(nullptr), exc.what());
        }
    }

    [[nodiscard]] static JsonValue jsonRpcError(JsonValue id, std::string_view message) {
        return JsonValue::Object{
            {"jsonrpc", std::string(kJsonRpcVersion)},
            {"id", std::move(id)},
            {"error", JsonValue::Object{{"code", kJsonRpcErrorCode}, {"message", std::string(message)}}},
        };
    }

    McpController& controller_;
};

[[nodiscard]] std::string readSocketLine(int clientFd) {
    std::string line;
    char ch = '\0';
    while (true) {
        const ssize_t count = ::recv(clientFd, &ch, 1, 0);
        if (count == 0) {
            break;
        }
        if (count < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw std::system_error(errno, std::generic_category(), "socket read failed");
        }
        if (ch == '\n') {
            break;
        }
        line.push_back(ch);
    }
    return line;
}

void writeSocketAll(int clientFd, std::string_view text) {
    const char* cursor = text.data();
    std::size_t remaining = text.size();
    while (remaining > 0) {
        const ssize_t written = ::send(clientFd, cursor, remaining, 0);
        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw std::system_error(errno, std::generic_category(), "socket write failed");
        }
        cursor += written;
        remaining -= static_cast<std::size_t>(written);
    }
}

[[nodiscard]] std::string readSocketBytes(int clientFd, std::size_t byteCount) {
    std::string result;
    result.resize(byteCount);
    std::size_t offset = 0;
    while (offset < byteCount) {
        const ssize_t count = ::recv(clientFd, result.data() + offset, byteCount - offset, 0);
        if (count == 0) {
            result.resize(offset);
            break;
        }
        if (count < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw std::system_error(errno, std::generic_category(), "socket read failed");
        }
        offset += static_cast<std::size_t>(count);
    }
    return result;
}

struct HttpRequest {
    std::string method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;
};

[[nodiscard]] std::string trimAscii(std::string text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
        text.erase(text.begin());
    }
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) {
        text.pop_back();
    }
    return text;
}

[[nodiscard]] HttpRequest readHttpRequest(int clientFd) {
    std::string headerText;
    char ch = '\0';
    while (headerText.find("\r\n\r\n") == std::string::npos) {
        const ssize_t count = ::recv(clientFd, &ch, 1, 0);
        if (count == 0) {
            break;
        }
        if (count < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw std::system_error(errno, std::generic_category(), "socket read failed");
        }
        headerText.push_back(ch);
    }

    HttpRequest request;
    std::istringstream stream(headerText);
    std::string line;
    if (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        std::istringstream firstLine(line);
        firstLine >> request.method >> request.path;
    }

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            break;
        }
        const std::size_t colon = line.find(':');
        if (colon == std::string::npos) {
            continue;
        }
        const std::string name = lowerAscii(trimAscii(line.substr(0, colon)));
        const std::string value = trimAscii(line.substr(colon + 1));
        request.headers[name] = value;
    }

    const auto lengthIt = request.headers.find("content-length");
    if (lengthIt != request.headers.end()) {
        request.body = readSocketBytes(clientFd, static_cast<std::size_t>(std::stoul(lengthIt->second)));
    }
    return request;
}

[[nodiscard]] std::string httpHeader(const HttpRequest& request, std::string_view name) {
    const auto it = request.headers.find(std::string(name));
    return it == request.headers.end() ? std::string{} : it->second;
}

[[nodiscard]] std::string httpStatusText(int status) {
    if (status == 200) return "OK";
    if (status == 202) return "Accepted";
    if (status == 400) return "Bad Request";
    if (status == 403) return "Forbidden";
    if (status == 404) return "Not Found";
    if (status == 405) return "Method Not Allowed";
    if (status == 406) return "Not Acceptable";
    return "Error";
}

[[nodiscard]] std::string httpResponse(int status, std::string_view body,
                                       std::map<std::string, std::string> headers = {}) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status << " " << httpStatusText(status) << "\r\n";
    response << "Connection: close\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    for (const auto& [name, value] : headers) {
        response << name << ": " << value << "\r\n";
    }
    response << "\r\n";
    response << body;
    return response.str();
}

class McpTcpServer {
public:
    McpTcpServer(McpController& controller, McpRuntimeConfig config)
        : dispatcher_(controller), config_(std::move(config)) {}

    void start() {
        thread_ = std::thread([this] { serve(); });
    }

    void serveForever() {
        serve();
    }

    void stop() {
        running_ = false;
#ifndef _WIN32
        if (serverFd_ >= 0) {
            ::shutdown(serverFd_, SHUT_RDWR);
            ::close(serverFd_);
            serverFd_ = -1;
        }
#endif
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    ~McpTcpServer() {
        stop();
    }

private:
    void serve() {
#ifndef _WIN32
        serverFd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (serverFd_ < 0) {
            return;
        }
        int reuse = 1;
        (void)::setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(static_cast<uint16_t>(config_.port));
        if (::inet_pton(AF_INET, config_.host.c_str(), &address.sin_addr) != 1) {
            address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        if (::bind(serverFd_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
            return;
        }
        if (::listen(serverFd_, 16) != 0) {
            return;
        }
        while (running_) {
            const int clientFd = ::accept(serverFd_, nullptr, nullptr);
            if (clientFd < 0) {
                if (errno == EINTR) {
                    continue;
                }
                break;
            }
            std::thread([this, clientFd] {
                try {
                    const std::string line = readSocketLine(clientFd);
                    if (!line.empty()) {
                        const std::string response = dispatcher_.handleLine(line);
                        if (!response.empty()) {
                            writeSocketAll(clientFd, response);
                        }
                    }
                } catch (...) {
                }
                ::close(clientFd);
            }).detach();
        }
#endif
    }

    McpJsonRpcDispatcher dispatcher_;
    McpRuntimeConfig config_;
    std::thread thread_;
    bool running_ = true;
    int serverFd_ = -1;
};

class McpStdioServer {
public:
    explicit McpStdioServer(McpController& controller) : dispatcher_(controller) {}

    void serveForever() {
        std::string line;
        while (std::getline(std::cin, line)) {
            const std::string response = dispatcher_.handleLine(line);
            if (!response.empty()) {
                std::cout << response;
                std::cout.flush();
            }
        }
    }

private:
    McpJsonRpcDispatcher dispatcher_;
};

class McpHttpServer {
public:
    McpHttpServer(McpController& controller, McpRuntimeConfig config)
        : dispatcher_(controller), config_(std::move(config)) {}

    void start() {
        thread_ = std::thread([this] { serve(); });
    }

    void serveForever() {
        serve();
    }

    void stop() {
        running_ = false;
#ifndef _WIN32
        if (serverFd_ >= 0) {
            ::shutdown(serverFd_, SHUT_RDWR);
            ::close(serverFd_);
            serverFd_ = -1;
        }
#endif
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    ~McpHttpServer() {
        stop();
    }

private:
    [[nodiscard]] std::string handleHttpRequest(const HttpRequest& request) {
        if (request.path != kMcpHttpEndpoint) {
            return httpResponse(404, "");
        }
        if (!mcpHttpOriginAllowed(httpHeader(request, "origin"))) {
            return httpResponse(403, "");
        }
        if (!mcpHttpProtocolAllowed(httpHeader(request, "mcp-protocol-version"))) {
            return httpResponse(400, "");
        }
        if (request.method == "GET" || request.method == "DELETE") {
            return httpResponse(405, "");
        }
        if (request.method != "POST") {
            return httpResponse(405, "");
        }
        if (!mcpHttpAcceptAllowed(httpHeader(request, "accept"))) {
            return httpResponse(406, "");
        }

        const std::string body = dispatcher_.handleLine(request.body);
        if (body.empty()) {
            return httpResponse(202, "");
        }
        std::map<std::string, std::string> headers;
        headers["Content-Type"] = std::string(kHttpApplicationJson);
        headers["MCP-Protocol-Version"] = std::string(kMcpProtocolVersion);
        std::string jsonBody = body;
        if (!jsonBody.empty() && jsonBody.back() == '\n') {
            jsonBody.pop_back();
        }
        return httpResponse(200, jsonBody, std::move(headers));
    }

    void serve() {
#ifndef _WIN32
        serverFd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (serverFd_ < 0) {
            return;
        }
        int reuse = 1;
        (void)::setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(static_cast<uint16_t>(config_.port));
        if (::inet_pton(AF_INET, config_.host.c_str(), &address.sin_addr) != 1) {
            address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        if (::bind(serverFd_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
            return;
        }
        if (::listen(serverFd_, 16) != 0) {
            return;
        }
        while (running_) {
            const int clientFd = ::accept(serverFd_, nullptr, nullptr);
            if (clientFd < 0) {
                if (errno == EINTR) {
                    continue;
                }
                break;
            }
            std::thread([this, clientFd] {
                try {
                    writeSocketAll(clientFd, handleHttpRequest(readHttpRequest(clientFd)));
                } catch (...) {
                }
                ::close(clientFd);
            }).detach();
        }
#endif
    }

    McpJsonRpcDispatcher dispatcher_;
    McpRuntimeConfig config_;
    std::thread thread_;
    bool running_ = true;
    int serverFd_ = -1;
};

// Restore the terminal on signals that would otherwise bypass the normal
// teardown (SIGTERM from `kill`, SIGHUP on disconnect, SIGINT). Without this the
// alternate screen and mouse tracking stay enabled, so the shell prints raw
// mouse-report sequences when the user scrolls. Only async-signal-safe calls
// (write/tcsetattr/sigaction/raise) run inside the handler.
struct TerminalSignalRestoreState {
    int stdinFd = -1;
    int stdoutFd = -1;
    termios savedTermios{};
    bool hasSavedTermios = false;
    const char* leaveSequence = nullptr;
    std::size_t leaveLength = 0;
};

volatile std::sig_atomic_t g_terminalRestoreArmed = 0;
TerminalSignalRestoreState g_terminalRestore;
struct sigaction g_previousSigterm {};
struct sigaction g_previousSighup {};
struct sigaction g_previousSigint {};

extern "C" void handleTerminalRestoreSignal(int signalNumber) {
    if (g_terminalRestoreArmed) {
        g_terminalRestoreArmed = 0;
        if (g_terminalRestore.leaveSequence != nullptr && g_terminalRestore.stdoutFd >= 0) {
            (void)::write(g_terminalRestore.stdoutFd, g_terminalRestore.leaveSequence,
                          g_terminalRestore.leaveLength);
        }
        if (g_terminalRestore.hasSavedTermios && g_terminalRestore.stdinFd >= 0) {
            termios restored = g_terminalRestore.savedTermios;
            restored.c_oflag |= static_cast<tcflag_t>(OPOST | ONLCR);
            (void)tcsetattr(g_terminalRestore.stdinFd, TCSANOW, &restored);
        }
    }
    // Re-raise with the default disposition so the exit status reflects the signal.
    struct sigaction defaultAction {};
    defaultAction.sa_handler = SIG_DFL;
    sigemptyset(&defaultAction.sa_mask);
    defaultAction.sa_flags = 0;
    (void)sigaction(signalNumber, &defaultAction, nullptr);
    (void)raise(signalNumber);
}

void installTerminalRestoreHandlers() {
    struct sigaction action {};
    action.sa_handler = handleTerminalRestoreSignal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGTERM, &action, &g_previousSigterm);
    sigaction(SIGHUP, &action, &g_previousSighup);
    sigaction(SIGINT, &action, &g_previousSigint);
}

void removeTerminalRestoreHandlers() {
    g_terminalRestoreArmed = 0;
    sigaction(SIGTERM, &g_previousSigterm, nullptr);
    sigaction(SIGHUP, &g_previousSighup, nullptr);
    sigaction(SIGINT, &g_previousSigint, nullptr);
}

}  // namespace

GeneratedWindowStackFrame& GeneratedWindowStack::push(GeneratedWindowBase& window, GeneratedWindowFrameOptions options) {
    GeneratedWindowStackFrame frame;
    frame.window = &window;
    frame.options = std::move(options);
    const std::vector<Element*> focusable = focusableElements(window);
    if (!frame.options.initialFocusName.empty()) {
        focusElement(frame.focusedIndex, focusable, findElement(window, frame.options.initialFocusName));
    } else if (frame.options.startInEditMode && !focusable.empty()) {
        frame.focusedIndex = 0;
    }
    frame.editMode = frame.options.startInEditMode;
    frames_.push_back(std::move(frame));
    return frames_.back();
}

void GeneratedWindowStack::pop() {
    if (!frames_.empty()) {
        frames_.pop_back();
    }
}

void GeneratedWindowStack::remove(GeneratedWindowBase& window) {
    frames_.erase(std::remove_if(frames_.begin(), frames_.end(), [&](const GeneratedWindowStackFrame& frame) {
        return frame.window == &window;
    }), frames_.end());
}

void GeneratedWindowStack::clear() {
    frames_.clear();
}

GeneratedWindowStackFrame* GeneratedWindowStack::top() {
    return frames_.empty() ? nullptr : &frames_.back();
}

const GeneratedWindowStackFrame* GeneratedWindowStack::top() const {
    return frames_.empty() ? nullptr : &frames_.back();
}

void dimGeneratedWindowModalBackground(TerminalBuffer& buffer) {
    dimBufferForModalOverlay(buffer);
}

void renderGeneratedWindow(GeneratedWindowBase& window, TerminalBuffer& buffer, int focusedIndex, bool editMode,
                           std::string_view footer, Element* activeScrollView,
                           Element* activeScrollViewEditElement) {
    (void)footer;
    buffer.clear();

    bool ownsActiveScrollView =
        activeScrollView != nullptr && ownerWindowForElement(window, activeScrollView) != nullptr;
    Element* effectiveActiveScrollView = activeScrollView;
    Element* effectiveActiveScrollViewEditElement =
        activeScrollViewEditElement != nullptr && ownerWindowForElement(window, activeScrollViewEditElement) != nullptr
            ? activeScrollViewEditElement
            : nullptr;
    Element* focusedElement = nullptr;
    if (focusedIndex >= 0) {
        const std::vector<Element*> focusable = focusableElements(window, effectiveActiveScrollView);
        if (focusedIndex < static_cast<int>(focusable.size())) {
            focusedElement = focusable[static_cast<std::size_t>(focusedIndex)];
        }
    }
    if (editMode && effectiveActiveScrollView == nullptr) {
        effectiveActiveScrollView = scrollViewContainingElement(window, focusedElement);
        if (effectiveActiveScrollView == nullptr) {
            effectiveActiveScrollView = scrollViewContainingElement(window, effectiveActiveScrollViewEditElement);
        }
        ownsActiveScrollView = effectiveActiveScrollView != nullptr;
    }
    const Style& style = window.generatedWindowStyle();
    const Rect bounds = windowBounds(window, buffer);
    fillRect(buffer, bounds, borderStyleFor(style));
    const Rect content = contentBounds(bounds, style);
    fillRect(buffer, content, style);
    std::vector<RuntimeCell> runtimeCells = collectRuntimeCells(window);
    std::vector<ResolvedRuntimeCell> resolvedCells;
    const int horizontalSeparator = borderWidthHorizontal(style);
    const int verticalSeparator = borderWidthVertical(style);
    resolveRuntimeCellsWithFitPass(window, runtimeCells, content, horizontalSeparator, verticalSeparator,
                                   windowMode(window), resolvedCells);
    std::vector<const ResolvedRuntimeCell*> paintedCells;
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, entry);
        if (cell == nullptr) {
            continue;
        }
        if (std::find(paintedCells.begin(), paintedCells.end(), cell) == paintedCells.end()) {
            paintedCells.push_back(cell);
            renderEntryCellStyle(buffer, entry, cell->rect);
        }
    }
    ScrollView* activeScrollViewForFocus = editMode ? dynamic_cast<ScrollView*>(
        ownsActiveScrollView
            ? effectiveActiveScrollView
            : (scrollViewContainingElement(window, focusedElement) != nullptr
                   ? scrollViewContainingElement(window, focusedElement)
                   : scrollViewContainingElement(window, effectiveActiveScrollViewEditElement))
    ) : nullptr;
    (void)activeScrollViewForFocus;
    const GeneratedLayoutEntry* deferredComboBoxEntry = nullptr;
    const bool elementEditMode = editMode && effectiveActiveScrollView == nullptr;
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, entry);
        if (cell != nullptr) {
            if (isFocusedComboBoxEntry(window, entry, focusedElement, elementEditMode)) {
                deferredComboBoxEntry = &entry;
                continue;
            }
            renderEntry(window, buffer, entry, focusedElement, editMode, cell->rect, effectiveActiveScrollView,
                        effectiveActiveScrollViewEditElement);
        }
    }
    if (deferredComboBoxEntry != nullptr) {
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, *deferredComboBoxEntry);
        if (cell != nullptr) {
            renderEntry(window, buffer, *deferredComboBoxEntry, focusedElement, elementEditMode, cell->rect,
                        effectiveActiveScrollView, effectiveActiveScrollViewEditElement);
        }
    }
    if (effectiveActiveScrollView != nullptr) {
        (void)focusableElements(window, effectiveActiveScrollView);
    }
    dimOutsideActiveScrollView(
        buffer,
        editMode && ownsActiveScrollView ? dynamic_cast<ScrollView*>(effectiveActiveScrollView) : nullptr,
        generatedScrollViewScopeDimBackground(
            window,
            dynamic_cast<ScrollView*>(effectiveActiveScrollView)));
}

RenderedContent renderGeneratedWindowContent(GeneratedWindowBase& window, Size size, int focusedIndex, bool editMode,
                                             Element* activeScrollView, Element* activeScrollViewEditElement,
                                             const Style* windowStyleOverride,
                                             std::optional<Color> activeScrollViewFocusBackground,
                                             std::optional<int> clipTop,
                                             std::optional<int> clipBottom,
                                             bool applyActiveScrollViewDim) {
    const int width = std::max(kMinimumRenderableSize, size.width);
    const int height = std::max(kMinimumRenderableSize, size.height);
    Element* effectiveActiveScrollView = activeScrollView;
    Element* effectiveActiveScrollViewEditElement =
        activeScrollViewEditElement != nullptr && ownerWindowForElement(window, activeScrollViewEditElement) != nullptr
            ? activeScrollViewEditElement
            : nullptr;
    const std::vector<Element*> focusable = focusableElements(window, effectiveActiveScrollView);
    Element* focusedElement = nullptr;
    if (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size())) {
        focusedElement = focusable[static_cast<std::size_t>(focusedIndex)];
    }
    if (editMode && effectiveActiveScrollView == nullptr) {
        effectiveActiveScrollView = scrollViewContainingElement(window, focusedElement);
        if (effectiveActiveScrollView == nullptr) {
            effectiveActiveScrollView = scrollViewContainingElement(window, effectiveActiveScrollViewEditElement);
        }
    }
    TerminalBuffer buffer(width, height);
    buffer.clear();

    const Style& style = windowStyleOverride != nullptr ? *windowStyleOverride : window.generatedWindowStyle();
    const Rect content{0, 0, width, height};
    fillRect(buffer, content, style);
    std::vector<RuntimeCell> runtimeCells = collectRuntimeCells(window);
    std::vector<ResolvedRuntimeCell> resolvedCells;
    const int horizontalSeparator = borderWidthHorizontal(style);
    const int verticalSeparator = borderWidthVertical(style);
    const GeneratedWindowMode mode = windowMode(window);
    resolveRuntimeCellsWithFitPass(window, runtimeCells, content, horizontalSeparator, verticalSeparator,
                                   mode, resolvedCells);
    std::vector<const ResolvedRuntimeCell*> paintedCells;
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, entry);
        if (cell == nullptr) {
            continue;
        }
        if (std::find(paintedCells.begin(), paintedCells.end(), cell) == paintedCells.end()) {
            paintedCells.push_back(cell);
            renderEntryCellStyle(buffer, entry, cell->rect);
        }
    }
    ScrollView* activeScrollViewForFocus = editMode ? dynamic_cast<ScrollView*>(
        effectiveActiveScrollView != nullptr
            ? effectiveActiveScrollView
            : (scrollViewContainingElement(window, focusedElement) != nullptr
                   ? scrollViewContainingElement(window, focusedElement)
                   : scrollViewContainingElement(window, effectiveActiveScrollViewEditElement))
    ) : nullptr;
    (void)activeScrollViewForFocus;
    const GeneratedLayoutEntry* deferredComboBoxEntry = nullptr;
    const bool elementEditMode = editMode && effectiveActiveScrollView == nullptr;
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, entry);
        if (cell != nullptr) {
            if (isFocusedComboBoxEntry(window, entry, focusedElement, elementEditMode)) {
                deferredComboBoxEntry = &entry;
                continue;
            }
            renderEntry(window, buffer, entry, focusedElement, editMode, cell->rect, effectiveActiveScrollView,
                        effectiveActiveScrollViewEditElement, clipTop, clipBottom, activeScrollViewFocusBackground);
        }
    }
    if (deferredComboBoxEntry != nullptr) {
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, *deferredComboBoxEntry);
        if (cell != nullptr) {
            renderEntry(window, buffer, *deferredComboBoxEntry, focusedElement, elementEditMode, cell->rect,
                        effectiveActiveScrollView, effectiveActiveScrollViewEditElement, clipTop, clipBottom,
                        activeScrollViewFocusBackground);
        }
    }
    if (applyActiveScrollViewDim && editMode && effectiveActiveScrollView != nullptr &&
        ownerWindowForElement(window, effectiveActiveScrollView) != nullptr) {
        auto* scrollView = dynamic_cast<ScrollView*>(effectiveActiveScrollView);
        dimOutsideActiveScrollView(
            buffer,
            scrollView,
            generatedScrollViewScopeDimBackground(window, scrollView));
    }
    RenderedContent rendered = contentFromBuffer(buffer);
    if (activeScrollViewFocusBackground.has_value() && window.generatedScrollView() == nullptr) {
        applyReusableFocusBackground(rendered, *activeScrollViewFocusBackground, window.generatedWindowStyle().background);
    }
    return rendered;
}

Size generatedWindowContentSize(const GeneratedWindowBase& window) {
    const Style& style = window.generatedWindowStyle();
    std::vector<RuntimeCell> runtimeCells = collectRuntimeCells(window);
    const int horizontalSeparator = borderWidthHorizontal(style);
    const int verticalSeparator = borderWidthVertical(style);
    const Size natural = layoutSize(buildRuntimeTree(runtimeCells, horizontalSeparator, verticalSeparator));
    std::vector<ResolvedRuntimeCell> resolvedCells;
    resolveRuntimeCellsWithFitPass(window, runtimeCells, Rect{0, 0, natural.width, natural.height},
                                   horizontalSeparator, verticalSeparator, windowMode(window), resolvedCells);
    return resolvedContentExtent(resolvedCells, natural);
}

Size generatedWindowContentSizeForWidth(const GeneratedWindowBase& window, int width) {
    const Style& style = window.generatedWindowStyle();
    std::vector<RuntimeCell> runtimeCells = collectRuntimeCells(window);
    const int horizontalSeparator = borderWidthHorizontal(style);
    const int verticalSeparator = borderWidthVertical(style);
    const Size natural = layoutSize(buildRuntimeTree(runtimeCells, horizontalSeparator, verticalSeparator));
    std::vector<ResolvedRuntimeCell> resolvedCells;
    resolveRuntimeCellsWithFitPass(window, runtimeCells,
                                   Rect{0, 0, std::max(kMinimumRenderableSize, width), natural.height},
                                   horizontalSeparator, verticalSeparator, windowMode(window), resolvedCells);
    Size result = resolvedContentExtent(resolvedCells, Size{std::max(kMinimumRenderableSize, width), natural.height});
    for (const GeneratedLayoutEntry& entry : window.generatedLayout()) {
        const ResolvedRuntimeCell* cell = resolvedCellFor(resolvedCells, entry);
        if (cell == nullptr) {
            continue;
        }
        const Element* element = entry.name.empty() ? nullptr : findElement(window, entry.name);
        const Size entrySize = renderSizeFor(entry, cell->rect, element);
        result.width = std::max(result.width, cell->rect.col + paddingLeft(entry.cellStyle) +
                                              entry.relative.col + entrySize.width);
        result.height = std::max(result.height, cell->rect.row + paddingTop(entry.cellStyle) +
                                               entry.relative.row + entrySize.height);
    }
    return result;
}

Size generatedWindowResolvedContentSize(const GeneratedWindowBase& window, Size size) {
    const int width = std::max(kMinimumRenderableSize, size.width);
    const int height = std::max(kMinimumRenderableSize, size.height);
    const Style& style = window.generatedWindowStyle();
    std::vector<RuntimeCell> runtimeCells = collectRuntimeCells(window);
    std::vector<ResolvedRuntimeCell> resolvedCells;
    const int horizontalSeparator = borderWidthHorizontal(style);
    const int verticalSeparator = borderWidthVertical(style);
    resolveRuntimeCellsWithFitPass(window, runtimeCells, Rect{0, 0, width, height},
                                   horizontalSeparator, verticalSeparator, windowMode(window), resolvedCells);
    return resolvedContentExtent(resolvedCells, Size{width, height});
}

int runGeneratedWindow(GeneratedWindowBase& window, GeneratedWindowRuntimeOptions options) {
    return runGeneratedWindow(window, std::move(options), 0, nullptr);
}

int runGeneratedWindow(GeneratedWindowBase& window, GeneratedWindowRuntimeOptions options, int argc, char** argv) {
    const McpRuntimeConfig mcpConfig = parseMcpRuntimeArgs(argc, argv);

    if (mcpConfig.enabled && !mcpConfig.gui) {
        std::vector<Element*> focusable = focusableElements(window);
        int focusedIndex = -1;
        if (!options.initialFocusName.empty()) {
            Element* initialElement = findElement(window, options.initialFocusName);
            const int initialIndex = indexOfElement(focusable, initialElement);
            if (initialIndex >= 0) {
                focusedIndex = initialIndex;
            }
        } else if (options.startInEditMode && !focusable.empty()) {
            focusedIndex = 0;
        }

        bool editMode = false;
        if (options.startInEditMode && focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()) &&
            isEditableElement(*focusable[static_cast<std::size_t>(focusedIndex)])) {
            editMode = true;
            notifyEditStarted(options, focusable[static_cast<std::size_t>(focusedIndex)]);
        }

        McpRuntimeState mcpState;
        mcpState.focusedIndex = focusedIndex;
        mcpState.editMode = editMode;
        mcpState.viewport = mcpConfig.viewport;
        McpController mcpController(window, options, mcpState, mcpConfig);
        if (mcpConfig.transport == kMcpTransportStdio) {
            McpStdioServer(mcpController).serveForever();
            return 0;
        }
        if (mcpConfig.transport == kMcpTransportTcp) {
            McpTcpServer(mcpController, mcpConfig).serveForever();
            return 0;
        }
        if (mcpConfig.transport == kMcpTransportHttp) {
            McpHttpServer(mcpController, mcpConfig).serveForever();
            return 0;
        }
        throw std::runtime_error("Unsupported C++ MCP transport: " + mcpConfig.transport);
    }

    PosixTerminalBackend backend(STDIN_FILENO, STDOUT_FILENO);
    termios originalTermios{};
    const bool hasOriginalTermios = tcgetattr(STDIN_FILENO, &originalTermios) == 0;
    TerminalModeGuard mode(STDIN_FILENO, STDOUT_FILENO);
    if (const std::optional<Size> cellPixelSize = backend.cellPixelSize()) {
        setImageTerminalCellPixels(*cellPixelSize);
    }

    const Size initialSize = mcpConfig.viewport.has_value()
        ? Size{mcpConfig.viewport->width, mcpConfig.viewport->height}
        : terminalSize(backend);
    TerminalBuffer buffer(initialSize.width, initialSize.height);
    const std::string enterAlternateScreen = "\x1b[?1049h\x1b[?1000h\x1b[?1002h\x1b[?1006h\x1b[?2004h\x1b[>4;2m\x1b[?25l\x1b[2J";
    const std::string leaveAlternateScreen = "\x1b[0m\x1b[2J\x1b[H\x1b[>4;0m\x1b[?25h\x1b[?2004l\x1b[?1006l\x1b[?1002l\x1b[?1000l\x1b[?1049l\r\n";
    const bool embeddedViewport = mcpConfig.viewport.has_value();
    const std::string embeddedLeaveScreen = "\x1b[0m\x1b[?25h";
    if (embeddedViewport) {
        backend.write("\x1b[?25l");
    } else {
        backend.write(enterAlternateScreen);
    }

    g_terminalRestore.stdinFd = STDIN_FILENO;
    g_terminalRestore.stdoutFd = STDOUT_FILENO;
    g_terminalRestore.savedTermios = originalTermios;
    g_terminalRestore.hasSavedTermios = hasOriginalTermios;
    const std::string& terminalLeaveSequence = embeddedViewport ? embeddedLeaveScreen : leaveAlternateScreen;
    g_terminalRestore.leaveSequence = terminalLeaveSequence.c_str();
    g_terminalRestore.leaveLength = terminalLeaveSequence.size();
    g_terminalRestoreArmed = 1;
    installTerminalRestoreHandlers();

    std::vector<Element*> initialFocusable = focusableElements(window);
    int focusedIndex = -1;
    if (!options.initialFocusName.empty()) {
        Element* initialElement = findElement(window, options.initialFocusName);
        const int initialIndex = indexOfElement(initialFocusable, initialElement);
        if (initialIndex >= 0) {
            focusedIndex = initialIndex;
        }
    } else if (options.startInEditMode && !initialFocusable.empty()) {
        focusedIndex = 0;
    }
    bool editMode = false;
    Element* activeScrollView = nullptr;
    Element* activeScrollViewEditElement = nullptr;
    std::unordered_map<Element*, Element*> scrollViewLastDescendant;
    std::optional<EditSnapshot> editSnapshot;
    if (options.startInEditMode && focusedIndex >= 0 && focusedIndex < static_cast<int>(initialFocusable.size()) &&
        isEditableElement(*initialFocusable[static_cast<std::size_t>(focusedIndex)])) {
        editSnapshot = captureSnapshot(initialFocusable[static_cast<std::size_t>(focusedIndex)]);
        editMode = true;
        notifyEditStarted(options, initialFocusable[static_cast<std::size_t>(focusedIndex)]);
    }
    Element* mouseSelectionElement = nullptr;
    int mouseSelectionAnchor = 0;
    MouseClickCandidate mouseClickCandidate;
    ScrollSelection scrollSelection;
    std::optional<ScrollRegionHint> pendingScrollRegion;
    std::string notification;
    std::chrono::steady_clock::time_point notificationExpiresAt{};
    bool running = true;
    bool dirty = true;
    bool clearBeforeNextFrame = false;
    std::size_t previousWindowStackSize = options.windowStack == nullptr ? 0 : options.windowStack->frames().size();
    const bool hasAnimatedTextGradient = windowHasAnimatedTextGradient(window);
    auto lastAnimatedRenderAt = std::chrono::steady_clock::now();
    McpRuntimeState mcpState;
    mcpState.focusedIndex = focusedIndex;
    mcpState.editMode = editMode;
    mcpState.activeScrollView = activeScrollView;
    mcpState.activeScrollViewEditElement = activeScrollViewEditElement;
    mcpState.viewport = mcpConfig.viewport;
    McpController mcpController(window, options, mcpState, mcpConfig);
    std::unique_ptr<McpTcpServer> mcpServer;
    std::unique_ptr<McpHttpServer> mcpHttpServer;
    if (mcpConfig.enabled) {
        if (mcpConfig.transport == kMcpTransportTcp) {
            mcpServer = std::make_unique<McpTcpServer>(mcpController, mcpConfig);
            mcpServer->start();
        } else if (mcpConfig.transport == kMcpTransportHttp) {
            mcpHttpServer = std::make_unique<McpHttpServer>(mcpController, mcpConfig);
            mcpHttpServer->start();
        } else {
            throw std::runtime_error("Unsupported C++ MCP GUI transport: " + mcpConfig.transport);
        }
    }

    while (running) {
        if (options.shouldClose && options.shouldClose()) {
            break;
        }
        bool requestedFullRedraw = false;
        {
            std::lock_guard lock(mcpState.mutex);
            focusedIndex = mcpState.focusedIndex;
            editMode = mcpState.editMode;
            activeScrollView = mcpState.activeScrollView;
            activeScrollViewEditElement = mcpState.activeScrollViewEditElement;
            if (mcpState.fullRedrawRequested) {
                requestedFullRedraw = true;
                dirty = true;
                mcpState.fullRedrawRequested = false;
            }
            if (mcpState.closeRequested) {
                running = false;
            }
        }
        const std::optional<Rect> activeViewport = [&]() {
            std::lock_guard lock(mcpState.mutex);
            return mcpState.viewport;
        }();
        const Size currentSize = activeViewport.has_value()
            ? Size{activeViewport->width, activeViewport->height}
            : terminalSize(backend);
        if (currentSize.width != buffer.width() || currentSize.height != buffer.height()) {
            if (const std::optional<Size> cellPixelSize = backend.cellPixelSize()) {
                setImageTerminalCellPixels(*cellPixelSize);
            }
            buffer.resize(currentSize.width, currentSize.height);
            dirty = true;
            requestedFullRedraw = true;
            clearBeforeNextFrame = !activeViewport.has_value();
        }
        const auto now = std::chrono::steady_clock::now();
        if (!mcpConfig.controlledRender && hasAnimatedTextGradient && now - lastAnimatedRenderAt >= kAnimatedRenderInterval) {
            dirty = true;
            lastAnimatedRenderAt = now;
        }
        if (!notification.empty() && now >= notificationExpiresAt) {
            dirty = true;
        }
        const std::size_t currentWindowStackSize = options.windowStack == nullptr ? 0 : options.windowStack->frames().size();
        if (currentWindowStackSize != previousWindowStackSize) {
            previousWindowStackSize = currentWindowStackSize;
            dirty = true;
            requestedFullRedraw = true;
            clearBeforeNextFrame = !activeViewport.has_value();
        }
        if (dirty) {
            std::optional<Rect> paintBounds;
            {
                std::lock_guard<std::recursive_mutex> uiLock(mcpState.uiMutex);
                if (activeViewport.has_value()) {
                    paintBounds = Rect{0, 0, buffer.width(), buffer.height()};
                }
                const bool dimOverlayBackground = options.shouldDimOverlayBackground && options.shouldDimOverlayBackground();
                const bool modalStackOpen = options.windowStack != nullptr && !options.windowStack->empty();
                if (modalStackOpen) {
                    ScopedImageCellBackgroundRendering imageFallback;
                    renderGeneratedWindow(window, buffer, focusedIndex, editMode, options.footer, activeScrollView,
                                          activeScrollViewEditElement);
                } else {
                    renderGeneratedWindow(window, buffer, focusedIndex, editMode, options.footer, activeScrollView,
                                          activeScrollViewEditElement);
                }
                if (options.windowStack != nullptr && !options.windowStack->empty()) {
                    renderWindowStackOverlay(buffer, *options.windowStack);
                }
                if (options.onRenderOverlay) {
                    if (dimOverlayBackground) {
                        dimBufferForModalOverlay(buffer);
                    }
                    options.onRenderOverlay(buffer);
                }
                applyScrollSelection(buffer, scrollSelection);
                if (!notification.empty()) {
                    if (std::chrono::steady_clock::now() < notificationExpiresAt) {
                        renderNotification(buffer, notification);
                    } else {
                        notification.clear();
                    }
                }
                if (requestedFullRedraw) {
                    buffer.requestFullRedraw();
                }
            }
            if (clearBeforeNextFrame) {
                backend.write(std::string{kAnsiClearScreen});
                clearBeforeNextFrame = false;
            }
            writeFrame(backend, buffer, activeViewport, paintBounds, pendingScrollRegion);
            pendingScrollRegion.reset();
            {
                std::lock_guard lock(mcpState.mutex);
                ++mcpState.renderGeneration;
            }
            mcpState.renderCondition.notify_all();
            dirty = false;
        }

        bool handledInput = false;
        for (const Event& event : coalesceMouseWheelEvents(backend.readEvents())) {
            std::lock_guard<std::recursive_mutex> uiLock(mcpState.uiMutex);
            if (event.type == EventType::MouseDrag) {
                if (mouseClickCandidate.element != nullptr) {
                    mouseClickCandidate.moved = true;
                    handledInput = true;
                    continue;
                }
                handledInput = handleScrollSelectionDrag(scrollSelection, event.position) ||
                               handleMouseDrag(mouseSelectionElement, mouseSelectionAnchor, event.position) ||
                               handledInput;
                continue;
            }
            if (event.type == EventType::MouseRelease) {
                if (mouseClickCandidate.element != nullptr) {
                    Point position = event.position;
                    if (activeViewport.has_value()) {
                        position.row -= activeViewport->row;
                        position.col -= activeViewport->col;
                    }
                    if (options.windowStack != nullptr) {
                        if (GeneratedWindowStackFrame* frame = options.windowStack->top();
                            frame != nullptr && frame->window != nullptr) {
                            const Rect frameBounds = refreshWindowStackFrameBounds(*frame, buffer);
                            if (!frameBounds.contains(position)) {
                                mouseClickCandidate = MouseClickCandidate{};
                                handledInput = true;
                                continue;
                            }
                            const Point local = stackFrameContentPoint(*frame, frameBounds, position);
                            if (!mouseClickCandidate.moved && mouseClickCandidate.element->frame().contains(local)) {
                                GeneratedWindowRuntimeOptions frameOptions = runtimeOptionsForFrame(frame->options);
                                notifyOwnerAwareButton(*frame->window, frameOptions, mouseClickCandidate.element);
                            }
                        }
                    } else if (!mouseClickCandidate.moved && mouseClickCandidate.element->frame().contains(position)) {
                        notifyOwnerAwareButton(window, options, mouseClickCandidate.element);
                    }
                    mouseClickCandidate = MouseClickCandidate{};
                    handledInput = true;
                    continue;
                }
                if (scrollSelection.element != nullptr && scrollSelection.active) {
                    const std::string text = selectedScrollText(buffer, scrollSelection);
                    if (!text.empty()) {
                        (void)copyTextToClipboard(text);
                        notification = "Copied to clipboard";
                        notificationExpiresAt = std::chrono::steady_clock::now() + kCopyNotificationDuration;
                    }
                    if (scrollSelection.changed) {
                        scrollSelection.active = false;
                    } else {
                        scrollSelection = ScrollSelection{};
                    }
                    handledInput = true;
                    continue;
                }
                if (auto* textInput = dynamic_cast<TextInput*>(mouseSelectionElement)) {
                    const std::string selectedText = textInput->selectedText();
                    if (!selectedText.empty()) {
                        (void)copyTextToClipboard(selectedText);
                        notification = "Copied to clipboard";
                        notificationExpiresAt = std::chrono::steady_clock::now() + kCopyNotificationDuration;
                    }
                } else if (auto* label = dynamic_cast<Label*>(mouseSelectionElement)) {
                    const std::string selectedText = label->selectedText();
                    if (!selectedText.empty()) {
                        (void)copyTextToClipboard(selectedText);
                        notification = "Copied to clipboard";
                        notificationExpiresAt = std::chrono::steady_clock::now() + kCopyNotificationDuration;
                    }
                }
                mouseSelectionElement = nullptr;
                mouseSelectionAnchor = 0;
                handledInput = true;
                continue;
            }
            if (event.type == EventType::MouseWheel) {
                Point position = event.position;
                if (activeViewport.has_value()) {
                    position.row -= activeViewport->row;
                    position.col -= activeViewport->col;
                }
                if (options.windowStack != nullptr) {
                    if (GeneratedWindowStackFrame* frame = options.windowStack->top(); frame != nullptr && frame->window != nullptr) {
                        const Rect frameBounds = refreshWindowStackFrameBounds(*frame, buffer);
                        const Point local = stackFrameContentPoint(*frame, frameBounds, position);
                        if (frame->options.onMouseWheelBeforeFocused &&
                            frame->options.onMouseWheelBeforeFocused(local, event.wheelDelta)) {
                            handledInput = true;
                            continue;
                        }
                        const std::vector<Element*> focusable = focusableElements(*frame->window, frame->activeScrollView);
                        const bool wheelHandled = handleMouseWheel(focusable, local, event.wheelDelta);
                        if (wheelHandled) {
                            queueScrollRegionHintForPosition(pendingScrollRegion, *frame->window, focusable, local);
                        }
                        handledInput = wheelHandled || handledInput;
                        continue;
                    }
                }
                if (options.onMouseWheelBeforeFocused && options.onMouseWheelBeforeFocused(event.position, event.wheelDelta)) {
                    handledInput = true;
                    continue;
                }
                std::vector<Element*> focusable = focusableElements(window);
                const bool wheelHandled =
                    handleMouseWheel(focusable, position, event.wheelDelta) ||
                    handleScrollViewMouseWheel(window, position, event.wheelDelta) ||
                    handleAppMouseWheel(window, options, position, event.wheelDelta);
                if (wheelHandled) {
                    queueScrollRegionHintForPosition(pendingScrollRegion, window, focusable, position);
                }
                handledInput = wheelHandled || handledInput;
                continue;
            }
            if (event.type == EventType::MousePress) {
                Point position = event.position;
                if (activeViewport.has_value()) {
                    position.row -= activeViewport->row;
                    position.col -= activeViewport->col;
                }
                if (options.windowStack != nullptr) {
                    if (GeneratedWindowStackFrame* frame = options.windowStack->top(); frame != nullptr && frame->window != nullptr) {
                        const Rect frameBounds = refreshWindowStackFrameBounds(*frame, buffer);
                        if (!frameBounds.contains(position)) {
                            handledInput = true;
                            continue;
                        }
                        const Point local = stackFrameContentPoint(*frame, frameBounds, position);
                        if (frame->options.onMousePressBeforeFocused && frame->options.onMousePressBeforeFocused(local)) {
                            handledInput = true;
                            continue;
                        }
                        GeneratedWindowRuntimeOptions frameOptions = runtimeOptionsForFrame(frame->options);
                        if (pointOutsideActiveScrollView(frame->activeScrollView, local)) {
                            exitScrollViewScope(*frame->window,
                                                frame->focusedIndex,
                                                frame->editMode,
                                                frame->activeScrollView,
                                                frame->scrollViewLastDescendant);
                            frame->activeScrollViewEditElement = nullptr;
                            frame->editSnapshot.reset();
                        }
                        Element* mouseScrollView = frame->activeScrollView;
                        if (mouseScrollView == nullptr) {
                            mouseScrollView = scrollViewAtPosition(*frame->window, local);
                        }
                        std::vector<Element*> focusable = focusableElements(*frame->window, mouseScrollView);
                        std::vector<Element*> mouseTargets = mouseTargetElements(*frame->window, mouseScrollView);
                        Element* focused = (frame->focusedIndex >= 0 && frame->focusedIndex < static_cast<int>(focusable.size()))
                            ? focusable[static_cast<std::size_t>(frame->focusedIndex)]
                            : nullptr;
                        Element* target = mouseTargetElement(
                            mouseTargets,
                            focused,
                            frame->editMode,
                            local,
                            frame->activeScrollViewEditElement
                        );
                        if (mouseScrollView != nullptr && target != nullptr && target != mouseScrollView) {
                            frame->activeScrollView = mouseScrollView;
                            frame->editMode = true;
                            frame->focusedIndex = indexOfElement(focusable, target);
                        }
                        if (closeComboBoxOnOutsideClick(focused, frame->editMode, frame->editSnapshot, target)) {
                            frame->activeScrollViewEditElement = nullptr;
                            (void)clearFocusForEmptyMouseTarget(*frame->window, frameOptions, frame->focusedIndex,
                                                                frame->editSnapshot, frame->editMode,
                                                                frame->activeScrollView,
                                                                frame->activeScrollViewEditElement, focusable);
                            handledInput = true;
                            continue;
                        }
                        clearLabelSelectionsInWindow(*frame->window);
                        if (target == nullptr &&
                            handleLabelMousePress(*frame->window, local, mouseSelectionElement, mouseSelectionAnchor)) {
                            handledInput = true;
                            continue;
                        }
                        handledInput = handleMousePress(*frame->window,
                                                        frameOptions,
                                                        frame->focusedIndex,
                                                        frame->editSnapshot,
                                                        frame->editMode,
                                                        frame->activeScrollView,
                                                        frame->activeScrollViewEditElement,
                                                        mouseSelectionElement,
                                                        mouseSelectionAnchor,
                                                        &mouseClickCandidate,
                                                        focusable,
                                                        target,
                                                        local) ||
                                       handledInput;
                        continue;
                    }
                }
                if (options.onMousePressBeforeFocused && options.onMousePressBeforeFocused(position)) {
                    handledInput = true;
                    continue;
                }
                if (pointOutsideActiveScrollView(activeScrollView, position)) {
                    exitScrollViewScope(window, focusedIndex, editMode, activeScrollView, scrollViewLastDescendant);
                    activeScrollViewEditElement = nullptr;
                    editSnapshot.reset();
                }
                Element* mouseScrollView = activeScrollView;
                if (mouseScrollView == nullptr) {
                    mouseScrollView = scrollViewAtPosition(window, position);
                }
                std::vector<Element*> focusable = focusableElements(window, mouseScrollView);
                std::vector<Element*> mouseTargets = mouseTargetElements(window, mouseScrollView);
                Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
                    ? focusable[static_cast<std::size_t>(focusedIndex)]
                    : nullptr;
                Element* target = mouseTargetElement(
                    mouseTargets,
                    focused,
                    editMode,
                    position,
                    activeScrollViewEditElement
                );
                if (mouseScrollView != nullptr && target != nullptr && target != mouseScrollView) {
                    activeScrollView = mouseScrollView;
                    editMode = true;
                    focusedIndex = indexOfElement(focusable, target);
                }
                if (closeComboBoxOnOutsideClick(focused, editMode, editSnapshot, target)) {
                    (void)clearFocusForEmptyMouseTarget(window, options, focusedIndex, editSnapshot, editMode,
                                                        activeScrollView, activeScrollViewEditElement, focusable);
                    handledInput = true;
                    continue;
                }
                if (dynamic_cast<ScrollView*>(target) == nullptr) {
                    scrollSelection = ScrollSelection{};
                }
                clearLabelSelectionsInWindow(window);
                if (target == nullptr && handleScrollSelectionPress(layoutElements(window), scrollSelection, position)) {
                    handledInput = true;
                    continue;
                }
                if (target == nullptr &&
                    handleLabelMousePress(window, position, mouseSelectionElement, mouseSelectionAnchor)) {
                    handledInput = true;
                    continue;
                }
                handledInput = handleMousePress(window, options, focusedIndex, editSnapshot, editMode,
                                                activeScrollView, activeScrollViewEditElement, mouseSelectionElement,
                                                mouseSelectionAnchor, &mouseClickCandidate, focusable, target,
                                                position) || handledInput;
                if (dynamic_cast<ScrollView*>(target) != nullptr &&
                    handleScrollSelectionPress(layoutElements(window), scrollSelection, position)) {
                    handledInput = true;
                }
                continue;
            }
            if (event.type == EventType::Paste) {
                if (options.windowStack != nullptr) {
                    if (GeneratedWindowStackFrame* frame = options.windowStack->top(); frame != nullptr && frame->window != nullptr) {
                        std::vector<Element*> focusable = focusableElements(*frame->window);
                        Element* focused = (frame->focusedIndex >= 0 && frame->focusedIndex < static_cast<int>(focusable.size()))
                            ? focusable[static_cast<std::size_t>(frame->focusedIndex)]
                            : nullptr;
                        if (focused != nullptr && !frame->editMode && isEditableElement(*focused)) {
                            frame->editSnapshot = captureSnapshot(focused);
                            beginElementEdit(focused);
                            frame->editMode = true;
                            notifyEditStarted(frame->options, focused);
                        }
                        handledInput = pasteIntoFocused(focused, event.text) || handledInput;
                        GeneratedWindowRuntimeOptions frameOptions = runtimeOptionsForFrame(frame->options);
                        notifyOwnerAwareTextChanged(*frame->window, frameOptions, focused);
                        continue;
                    }
                }
                std::vector<Element*> focusable = focusableElements(window);
                Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
                    ? focusable[static_cast<std::size_t>(focusedIndex)]
                    : nullptr;
                if (focused != nullptr && !editMode && isEditableElement(*focused)) {
                    editSnapshot = captureSnapshot(focused);
                    beginElementEdit(focused);
                    editMode = true;
                    notifyEditStarted(options, focused);
                }
                handledInput = pasteIntoFocused(focused, event.text) || handledInput;
                notifyOwnerAwareTextChanged(window, options, focused);
                continue;
            }
            if (event.type != EventType::Key) {
                continue;
            }
            handledInput = true;

            const std::string& key = event.key;
            clearLabelSelectionsInWindow(window);
            scrollSelection = ScrollSelection{};
            if (options.windowStack != nullptr) {
                if (GeneratedWindowStackFrame* frame = options.windowStack->top(); frame != nullptr) {
                    if (key == "Ctrl+C") {
                        running = false;
                        continue;
                    }
                    handledInput = handleStackFrameKey(*frame, key, [&] {
                        if (options.windowStack != nullptr && !options.windowStack->empty()) {
                            ScopedImageCellBackgroundRendering imageFallback;
                            renderGeneratedWindow(window, buffer, focusedIndex, editMode, options.footer,
                                                  activeScrollView, activeScrollViewEditElement);
                        } else {
                            renderGeneratedWindow(window, buffer, focusedIndex, editMode, options.footer,
                                                  activeScrollView, activeScrollViewEditElement);
                        }
                        renderWindowStackOverlay(buffer, *options.windowStack);
                        writeFrame(backend, buffer, activeViewport, std::nullopt);
                        std::this_thread::sleep_for(std::chrono::milliseconds(kDialogButtonCloseDelayMs));
                    }) || handledInput;
                    if (options.windowStack->empty()) {
                        editSnapshot.reset();
                        editMode = false;
                        activeScrollViewEditElement = nullptr;
                    }
                    continue;
                }
            }
            std::vector<Element*> focusable = focusableElements(window, activeScrollView);
            Element* focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
                ? focusable[static_cast<std::size_t>(focusedIndex)]
                : nullptr;

            if (key == "Ctrl+C") {
                running = false;
                continue;
            }
            if (key == "cmd_c") {
                if (copyFocusedText(focused)) {
                    notification = "Copied to clipboard";
                    notificationExpiresAt = std::chrono::steady_clock::now() + kCopyNotificationDuration;
                }
                continue;
            }
            if (key == "cmd_v") {
                if (focused != nullptr && !editMode && isEditableElement(*focused)) {
                    editSnapshot = captureSnapshot(focused);
                    beginElementEdit(focused);
                    editMode = true;
                    notifyEditStarted(options, focused);
                }
                handledInput = pasteIntoFocused(focused, runtimeClipboardText()) || handledInput;
                notifyOwnerAwareTextChanged(window, options, focused);
                continue;
            }
            if (key == "Escape" &&
                handleStandardEscapeButton(window, window.mcpClassName(), focusedIndex, options, [&] {
                    if (options.windowStack != nullptr && !options.windowStack->empty()) {
                        ScopedImageCellBackgroundRendering imageFallback;
                        renderGeneratedWindow(window, buffer, focusedIndex, editMode, options.footer,
                                              activeScrollView, activeScrollViewEditElement);
                    } else {
                        renderGeneratedWindow(window, buffer, focusedIndex, editMode, options.footer,
                                              activeScrollView, activeScrollViewEditElement);
                    }
                    if (options.windowStack != nullptr && !options.windowStack->empty()) {
                        renderWindowStackOverlay(buffer, *options.windowStack);
                    }
                    writeFrame(backend, buffer, activeViewport, std::nullopt);
                    std::this_thread::sleep_for(std::chrono::milliseconds(kDialogButtonCloseDelayMs));
                })) {
                continue;
            }
            if (editMode && key == "Escape") {
                if (activeScrollView != nullptr) {
                    if (activeScrollViewEditElement != nullptr) {
                        if (!usesLeaveCommit(activeScrollViewEditElement)) {
                            if (editSnapshot.has_value() &&
                                editSnapshot->element == activeScrollViewEditElement) {
                                restoreSnapshot(*editSnapshot);
                            }
                        } else {
                            commitEdit(activeScrollViewEditElement);
                        }
                        editSnapshot.reset();
                        activeScrollViewEditElement = nullptr;
                    } else {
                        exitScrollViewScope(window, focusedIndex, editMode, activeScrollView, scrollViewLastDescendant);
                    }
                } else {
                    escapeElementEdit(focused, editSnapshot, editMode);
                    if (options.keepEditModeAfterEscape && focused != nullptr && isEditableElement(*focused)) {
                        editSnapshot = captureSnapshot(focused);
                        beginElementEdit(focused);
                        editMode = true;
                        notifyEditStarted(options, focused);
                    }
                }
                continue;
            }
            if (options.onKeyBeforeFocusedElement &&
                options.onKeyBeforeFocusedElement(
                    key,
                    focused == nullptr ? std::string_view{} : std::string_view(focused->name()),
                    editMode)) {
                continue;
            }
            if (options.onKeyBeforeFocused && options.onKeyBeforeFocused(key)) {
                continue;
            }
            if (editMode && focused != nullptr) {
                if (activeScrollView != nullptr) {
                    if (activeScrollViewEditElement != nullptr) {
                        if (key == "Enter") {
                            (void)handleElementKey(*activeScrollViewEditElement, key);
                            notifyOwnerAwareTextConfirmed(window, options, activeScrollViewEditElement);
                            commitEdit(activeScrollViewEditElement);
                            editSnapshot.reset();
                            activeScrollViewEditElement = nullptr;
                        } else if (isArrowKey(key)) {
                            const int previousSelectionIndex = selectedIndexOf(activeScrollViewEditElement);
                            (void)handleElementKey(*activeScrollViewEditElement, key);
                            notifyOwnerAwareValueChangedAfterHandledKey(window, options, activeScrollViewEditElement,
                                                                        previousSelectionIndex);
                        } else {
                            const int previousSelectionIndex = selectedIndexOf(activeScrollViewEditElement);
                            (void)handleElementKey(*activeScrollViewEditElement, key);
                            notifyOwnerAwareValueChangedAfterHandledKey(window, options, activeScrollViewEditElement,
                                                                        previousSelectionIndex);
                        }
                    } else if (isArrowKey(key)) {
                        (void)moveScrollViewScopeFocus(window, focusedIndex, activeScrollView, scrollViewLastDescendant, key);
                    } else if (key == "Enter" || key == " ") {
                        focusable = focusableElements(window, activeScrollView);
                        focused = (focusedIndex >= 0 && focusedIndex < static_cast<int>(focusable.size()))
                            ? focusable[static_cast<std::size_t>(focusedIndex)]
                            : nullptr;
                        if (focused != nullptr && (isButton(*focused) || isClickableImage(*focused))) {
                            notifyOwnerAwareButton(window, options, focused);
                        } else if (focused != nullptr && isImmediateInput(*focused)) {
                            const int previousSelectionIndex = selectedIndexOf(focused);
                            (void)handleElementKey(*focused, key);
                            notifyOwnerAwareValueChangedAfterHandledKey(window, options, focused, previousSelectionIndex);
                        } else if (focused != nullptr && isEditableElement(*focused)) {
                            editSnapshot = captureSnapshot(focused);
                            beginElementEdit(focused);
                            activeScrollViewEditElement = focused;
                            notifyEditStarted(options, focused);
                        }
                    }
                } else if (key == "Tab" && options.onKey && options.onKey(key)) {
                    editSnapshot = captureSnapshot(focused);
                } else if (key == "Enter") {
                    if (usesLeaveCommit(focused)) {
                        const int previousSelectionIndex = selectedIndexOf(focused);
                        (void)handleElementKey(*focused, key);
                        notifyOwnerAwareValueChangedAfterHandledKey(window, options, focused, previousSelectionIndex);
                        continue;
                    }
                    (void)handleElementKey(*focused, key);
                    notifyOwnerAwareTextConfirmed(window, options, focused);
                    commitEdit(focused);
                    if (options.keepEditModeAfterConfirm && isEditableElement(*focused)) {
                        editSnapshot = captureSnapshot(focused);
                        editMode = true;
                        notifyEditStarted(options, focused);
                    } else {
                        editSnapshot.reset();
                        editMode = false;
                    }
                } else {
                    const int previousSelectionIndex = selectedIndexOf(focused);
                    (void)handleElementKey(*focused, key);
                    notifyOwnerAwareValueChangedAfterHandledKey(window, options, focused, previousSelectionIndex);
                }
                continue;
            }

            if (options.onKey && options.onKey(key)) {
                continue;
            }
            if (key == "Tab") {
                moveFocus(focusedIndex, focusable, 1);
                continue;
            }
            if (key == "Shift+Tab") {
                moveFocus(focusedIndex, focusable, -1);
                continue;
            }
            if (isArrowKey(key)) {
                moveFocusSpatial(focusedIndex, focusable, key);
                continue;
            }
            if (focused == nullptr) {
                continue;
            }
            if (key == "Enter" || key == " ") {
                if (isButton(*focused) || isClickableImage(*focused)) {
                    notifyOwnerAwareButton(window, options, focused);
                } else if (activateReusableControl(focused)) {
                } else if (isImmediateInput(*focused)) {
                    const int previousSelectionIndex = selectedIndexOf(focused);
                    (void)handleElementKey(*focused, key);
                    notifyOwnerAwareValueChangedAfterHandledKey(window, options, focused, previousSelectionIndex);
                } else if (isEditableElement(*focused)) {
                    if (!enterScrollViewScope(window, focusedIndex, editMode, activeScrollView, scrollViewLastDescendant)) {
                        editSnapshot = captureSnapshot(focused);
                        beginElementEdit(focused);
                        editMode = true;
                        notifyEditStarted(options, focused);
                    } else {
                        activeScrollViewEditElement = nullptr;
                        editSnapshot.reset();
                    }
                }
                continue;
            }
            if (isImmediateInput(*focused)) {
                const int previousSelectionIndex = selectedIndexOf(focused);
                (void)handleElementKey(*focused, key);
                notifyOwnerAwareValueChangedAfterHandledKey(window, options, focused, previousSelectionIndex);
            }
        }

        if (!handledInput) {
            std::this_thread::sleep_for(embeddedViewport ? kEmbeddedViewportIdleSleep : kInputIdleSleep);
        } else {
            queueScrollRegionHint(pendingScrollRegion, dynamic_cast<ScrollView*>(activeScrollView));
            dirty = true;
            std::lock_guard lock(mcpState.mutex);
            mcpState.focusedIndex = focusedIndex;
            mcpState.editMode = editMode;
            mcpState.activeScrollView = activeScrollView;
            mcpState.activeScrollViewEditElement = activeScrollViewEditElement;
        }
        if (options.shouldClose && options.shouldClose()) {
            running = false;
        }
    }

    if (mcpServer) {
        mcpServer->stop();
    }
    if (mcpHttpServer) {
        mcpHttpServer->stop();
    }
    removeTerminalRestoreHandlers();
    mode.restore();
    if (embeddedViewport) {
        backend.write("\x1b[0m\x1b[?25h");
    } else {
        backend.write(leaveAlternateScreen);
    }
    return 0;
}

}  // namespace ui
