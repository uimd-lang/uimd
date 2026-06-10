#pragma once

#include <string>

#include "ui/core/Rect.hpp"

namespace ui {

enum class EventType {
    Key,
    MousePress,
    MouseDrag,
    MouseRelease,
    MouseWheel,
    Paste,
    Resize,
    Focus,
    Blur,
    Open,
    Close,
    Save,
    ElementChanged,
};

struct Event {
    EventType type = EventType::Key;
    std::string key;
    std::string text;
    Point position;
    int wheelDelta = 0;
    Size size;
};

}  // namespace ui
