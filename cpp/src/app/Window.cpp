#include "ui/app/Window.hpp"

#include <stdexcept>

namespace ui {

Element& Window::addElement(std::unique_ptr<Element> element) {
    if (!element) {
        throw std::invalid_argument("element must not be null");
    }
    Element& ref = *element;
    elements_.push_back(std::move(element));
    return ref;
}

}  // namespace ui
