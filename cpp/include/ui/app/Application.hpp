#pragma once

#include <memory>
#include <vector>

#include "ui/app/Window.hpp"

namespace ui {

class Application {
public:
    Application() = default;

    Window& open(std::unique_ptr<Window> window);

    [[nodiscard]] Window* activeWindow() const;

    [[nodiscard]] const std::vector<std::unique_ptr<Window>>& windows() const {
        return windows_;
    }

private:
    std::vector<std::unique_ptr<Window>> windows_;
};

}  // namespace ui
