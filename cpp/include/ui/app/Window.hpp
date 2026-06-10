#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ui/core/Element.hpp"

namespace ui {

class Window {
public:
    explicit Window(std::string title = {}) : title_(std::move(title)) {}
    virtual ~Window() = default;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;

    [[nodiscard]] const std::string& title() const {
        return title_;
    }

    template <typename T, typename... Args>
    T& addElement(Args&&... args) {
        auto element = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *element;
        addElement(std::move(element));
        return ref;
    }

    Element& addElement(std::unique_ptr<Element> element);

    [[nodiscard]] const std::vector<std::unique_ptr<Element>>& elements() const {
        return elements_;
    }

private:
    std::string title_;
    std::vector<std::unique_ptr<Element>> elements_;
};

}  // namespace ui
