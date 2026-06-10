#include "ui/app/Application.hpp"

#include <stdexcept>

namespace ui {

Window& Application::open(std::unique_ptr<Window> window) {
    if (!window) {
        throw std::invalid_argument("window must not be null");
    }
    Window& ref = *window;
    windows_.push_back(std::move(window));
    return ref;
}

Window* Application::activeWindow() const {
    if (windows_.empty()) {
        return nullptr;
    }
    return windows_.back().get();
}

}  // namespace ui
