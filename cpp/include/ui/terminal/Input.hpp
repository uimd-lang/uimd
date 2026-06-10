#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "ui/core/Event.hpp"

namespace ui {

class InputParser {
public:
    [[nodiscard]] std::vector<Event> feed(std::string_view bytes);

private:
    [[nodiscard]] bool parseNext(std::vector<Event>& events);
    [[nodiscard]] bool parseEscapeSequence(std::vector<Event>& events);
    [[nodiscard]] bool parseBracketedPaste(std::vector<Event>& events);
    [[nodiscard]] bool parseSgrMouse(std::vector<Event>& events);
    [[nodiscard]] bool consumeSimpleEscape(std::vector<Event>& events);

    std::string buffer_;
};

}  // namespace ui
