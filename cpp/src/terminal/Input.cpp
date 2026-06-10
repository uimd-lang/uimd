#include "ui/terminal/Input.hpp"

#include <charconv>
#include <cstddef>
#include <string>
#include <system_error>
#include <string_view>

namespace ui {

namespace {

constexpr std::string_view kPasteStart = "\x1b[200~";
constexpr std::string_view kPasteEnd = "\x1b[201~";
constexpr std::string_view kTerminalCellPixelResponsePrefix = "\x1b[6;";
constexpr std::string_view kTerminalTextAreaPixelResponsePrefix = "\x1b[4;";
constexpr char kTerminalCellPixelResponseTerminator = 't';
constexpr char kEscape = '\x1b';
constexpr char kCtrlC = '\x03';
constexpr char kCarriageReturn = '\r';
constexpr char kLineFeed = '\n';
constexpr int kSgrMouseFieldCount = 3;
constexpr int kSgrMouseWheelUpButton = 64;
constexpr int kSgrMouseWheelDownButton = 65;
constexpr int kSgrMousePressedSuffix = 'M';
constexpr int kTerminalCoordinateBase = 1;
constexpr std::string_view kAltEnterSequences[] = {
    "\x1b[13;3u",
    "\x1b[13;5u",
    "\x1b[27;3;13~",
    "\x1b[27;5;13~",
};
constexpr int kEnterCodepoint = 13;
constexpr int kCtrlCCodepoint = 3;
constexpr int kUppercaseCCodepoint = 67;
constexpr int kLowercaseCCodepoint = 99;
constexpr int kUppercaseVCodepoint = 86;
constexpr int kLowercaseVCodepoint = 118;
constexpr int kNoModifier = 1;
constexpr int kShiftModifier = 2;
constexpr int kAltModifier = 3;
constexpr int kAltShiftModifier = 4;
constexpr int kCtrlModifier = 5;
constexpr int kCtrlShiftModifier = 6;
constexpr int kMetaModifier = 9;
constexpr int kMetaShiftModifier = 10;

[[nodiscard]] bool startsWith(std::string_view text, std::string_view prefix) {
    return text.size() >= prefix.size() && text.substr(0, prefix.size()) == prefix;
}

[[nodiscard]] bool isPrefixOf(std::string_view prefix, std::string_view text) {
    return prefix.size() < text.size() && text.substr(0, prefix.size()) == prefix;
}

[[nodiscard]] bool parseInt(std::string_view text, int& value) {
    const auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), value);
    return ec == std::errc{} && ptr == text.data() + text.size();
}

[[nodiscard]] std::string keyForCsiFinal(char final) {
    switch (final) {
        case 'A':
            return "Up";
        case 'B':
            return "Down";
        case 'C':
            return "Right";
        case 'D':
            return "Left";
        case 'H':
            return "Home";
        case 'F':
            return "End";
        case 'Z':
            return "Shift+Tab";
        default:
            return {};
    }
}

[[nodiscard]] std::string modifiedArrowKey(char final, int modifier) {
    std::string key = keyForCsiFinal(final);
    if (key.empty()) {
        return {};
    }
    if (modifier == kShiftModifier) {
        return "Shift+" + key;
    }
    if (modifier == kAltModifier) {
        return "Alt+" + key;
    }
    if (modifier == kAltShiftModifier) {
        return "Alt+Shift+" + key;
    }
    if (modifier == kCtrlModifier) {
        return "Ctrl+" + key;
    }
    if (modifier == kCtrlShiftModifier) {
        return "Ctrl+Shift+" + key;
    }
    return key;
}

[[nodiscard]] bool isCsiFinal(char ch) {
    return (ch >= '@' && ch <= '~');
}

[[nodiscard]] bool parseModifiedKeyPayload(std::string_view text, int& codepoint, int& modifier) {
    if (!startsWith(text, "\x1b[") || text.empty() || text.back() != 'u') {
        return false;
    }

    const std::string_view payload = text.substr(2, text.size() - 3);
    const std::size_t separator = payload.find(';');
    if (separator == std::string_view::npos) {
        return false;
    }

    return parseInt(payload.substr(0, separator), codepoint) &&
           parseInt(payload.substr(separator + 1), modifier);
}

[[nodiscard]] std::string parseModifiedKeyCsi(std::string_view text) {
    int codepoint = 0;
    int modifier = 0;
    if (!parseModifiedKeyPayload(text, codepoint, modifier) || modifier == kNoModifier) {
        return {};
    }
    if (codepoint == kEnterCodepoint) {
        return "Alt+Enter";
    }
    if (codepoint == kCtrlCCodepoint ||
        ((codepoint == kLowercaseCCodepoint || codepoint == kUppercaseCCodepoint) &&
         (modifier == kCtrlModifier || modifier == kCtrlShiftModifier))) {
        return "Ctrl+C";
    }
    if ((codepoint == kLowercaseCCodepoint || codepoint == kUppercaseCCodepoint) &&
        (modifier == kMetaModifier || modifier == kMetaShiftModifier)) {
        return "cmd_c";
    }
    if ((codepoint == kLowercaseVCodepoint || codepoint == kUppercaseVCodepoint) &&
        (modifier == kMetaModifier || modifier == kMetaShiftModifier)) {
        return "cmd_v";
    }
    return {};
}

[[nodiscard]] std::string parseLegacyModifiedKeyCsi(std::string_view text) {
    if (!startsWith(text, "\x1b[27;") || text.empty() || text.back() != '~') {
        return {};
    }

    const std::string_view payload = text.substr(2, text.size() - 3);
    std::size_t first = payload.find(';');
    std::size_t second = first == std::string_view::npos ? std::string_view::npos : payload.find(';', first + 1);
    if (first == std::string_view::npos || second == std::string_view::npos) {
        return {};
    }

    int modifier = 0;
    int codepoint = 0;
    if (!parseInt(payload.substr(first + 1, second - first - 1), modifier) ||
        !parseInt(payload.substr(second + 1), codepoint)) {
        return {};
    }
    if (codepoint == kCtrlCCodepoint ||
        ((codepoint == kLowercaseCCodepoint || codepoint == kUppercaseCCodepoint) &&
         (modifier == kCtrlModifier || modifier == kCtrlShiftModifier))) {
        return "Ctrl+C";
    }
    return {};
}

[[nodiscard]] std::string parseCsiKey(std::string_view sequence) {
    if (!startsWith(sequence, "\x1b[") || sequence.size() < 3) {
        return {};
    }
    const char final = sequence.back();
    if (sequence.size() == 3) {
        return keyForCsiFinal(final);
    }

    std::string_view payload = sequence.substr(2, sequence.size() - 3);
    const std::size_t separator = payload.rfind(';');
    if (separator == std::string_view::npos) {
        return {};
    }
    int modifier = 0;
    if (!parseInt(payload.substr(separator + 1), modifier)) {
        return {};
    }
    return modifiedArrowKey(final, modifier);
}

}  // namespace

std::vector<Event> InputParser::feed(std::string_view bytes) {
    buffer_.append(bytes);

    std::vector<Event> events;
    while (parseNext(events)) {
    }
    return events;
}

bool InputParser::parseNext(std::vector<Event>& events) {
    if (buffer_.empty()) {
        return false;
    }

    if (startsWith(buffer_, kPasteStart)) {
        return parseBracketedPaste(events);
    }

    const char ch = buffer_.front();
    if (ch == kEscape) {
        return parseEscapeSequence(events);
    }

    buffer_.erase(0, 1);
    if (ch == kCtrlC) {
        events.push_back(Event{.type = EventType::Key, .key = "Ctrl+C"});
    } else if (ch == kCarriageReturn || ch == kLineFeed) {
        events.push_back(Event{.type = EventType::Key, .key = "Enter"});
    } else if (ch == '\t') {
        events.push_back(Event{.type = EventType::Key, .key = "Tab"});
    } else if (ch == '\x7f') {
        events.push_back(Event{.type = EventType::Key, .key = "Backspace"});
    } else {
        events.push_back(Event{.type = EventType::Key, .key = std::string(1, ch)});
    }
    return true;
}

bool InputParser::parseEscapeSequence(std::vector<Event>& events) {
    if (buffer_.size() == 1) {
        buffer_.erase(0, 1);
        events.push_back(Event{.type = EventType::Key, .key = "Escape"});
        return true;
    }

    if (buffer_.size() >= 2 && (buffer_[1] == kCarriageReturn || buffer_[1] == kLineFeed)) {
        buffer_.erase(0, 2);
        events.push_back(Event{.type = EventType::Key, .key = "Alt+Enter"});
        return true;
    }
    for (std::string_view sequence : kAltEnterSequences) {
        if (startsWith(buffer_, sequence)) {
            buffer_.erase(0, sequence.size());
            events.push_back(Event{.type = EventType::Key, .key = "Alt+Enter"});
            return true;
        }
        if (isPrefixOf(buffer_, sequence)) {
            return false;
        }
    }
    if (startsWith(buffer_, "\x1b[") && buffer_.find('u') != std::string::npos) {
        const std::size_t end = buffer_.find('u');
        const std::string_view sequence(buffer_.data(), end + 1);
        const std::string key = parseModifiedKeyCsi(sequence);
        if (!key.empty()) {
            buffer_.erase(0, end + 1);
            events.push_back(Event{.type = EventType::Key, .key = key});
            return true;
        }
    }
    if (startsWith(buffer_, "\x1b[13;") && buffer_.find('u') == std::string::npos) {
        return false;
    }
    if (startsWith(buffer_, "\x1b[27;") && buffer_.find('~') != std::string::npos) {
        const std::size_t end = buffer_.find('~');
        const std::string_view sequence(buffer_.data(), end + 1);
        const std::string key = parseLegacyModifiedKeyCsi(sequence);
        if (!key.empty()) {
            buffer_.erase(0, end + 1);
            events.push_back(Event{.type = EventType::Key, .key = key});
            return true;
        }
    }
    if (startsWith(buffer_, kPasteStart)) {
        return parseBracketedPaste(events);
    }
    if (startsWith(buffer_, "\x1b[<")) {
        return parseSgrMouse(events);
    }
    if (startsWith(buffer_, kTerminalCellPixelResponsePrefix) ||
        startsWith(buffer_, kTerminalTextAreaPixelResponsePrefix)) {
        const std::size_t end = buffer_.find(kTerminalCellPixelResponseTerminator);
        if (end == std::string::npos) {
            return false;
        }
        buffer_.erase(0, end + 1);
        return true;
    }
    return consumeSimpleEscape(events);
}

bool InputParser::parseBracketedPaste(std::vector<Event>& events) {
    const std::size_t end = buffer_.find(kPasteEnd, kPasteStart.size());
    if (end == std::string::npos) {
        return false;
    }

    const std::string text = buffer_.substr(kPasteStart.size(), end - kPasteStart.size());
    buffer_.erase(0, end + kPasteEnd.size());
    events.push_back(Event{.type = EventType::Paste, .text = text});
    return true;
}

bool InputParser::parseSgrMouse(std::vector<Event>& events) {
    const std::size_t final = buffer_.find_first_of("Mm", 3);
    if (final == std::string::npos) {
        return false;
    }

    const char suffix = buffer_[final];
    const std::string_view payload(buffer_.data() + 3, final - 3);
    int values[kSgrMouseFieldCount] = {};
    std::size_t start = 0;
    for (int index = 0; index < kSgrMouseFieldCount; ++index) {
        const std::size_t sep = index == kSgrMouseFieldCount - 1 ? payload.size() : payload.find(';', start);
        if (sep == std::string_view::npos || !parseInt(payload.substr(start, sep - start), values[index])) {
            buffer_.erase(0, final + 1);
            return true;
        }
        start = sep + 1;
    }

    buffer_.erase(0, final + 1);
    const int button = values[0];
    const Point position{values[2] - kTerminalCoordinateBase, values[1] - kTerminalCoordinateBase};

    if (button == kSgrMouseWheelUpButton || button == kSgrMouseWheelDownButton) {
        events.push_back(Event{
            .type = EventType::MouseWheel,
            .position = position,
            .wheelDelta = button == kSgrMouseWheelUpButton ? 1 : -1,
        });
        return true;
    }

    EventType type = EventType::MouseRelease;
    if (suffix == kSgrMousePressedSuffix) {
        type = (button & 32) != 0 ? EventType::MouseDrag : EventType::MousePress;
    }
    events.push_back(Event{.type = type, .position = position});
    return true;
}

bool InputParser::consumeSimpleEscape(std::vector<Event>& events) {
    if (startsWith(buffer_, "\x1b[")) {
        std::size_t finalIndex = std::string::npos;
        for (std::size_t index = 2; index < buffer_.size(); ++index) {
            if (isCsiFinal(buffer_[index])) {
                finalIndex = index;
                break;
            }
        }
        if (finalIndex == std::string::npos) {
            return false;
        }
        const std::string_view sequence(buffer_.data(), finalIndex + 1);
        const std::string key = parseCsiKey(sequence);
        if (!key.empty()) {
            buffer_.erase(0, finalIndex + 1);
            events.push_back(Event{.type = EventType::Key, .key = key});
            return true;
        }
    }

    buffer_.erase(0, 1);
    events.push_back(Event{.type = EventType::Key, .key = "Escape"});
    return true;
}

}  // namespace ui
