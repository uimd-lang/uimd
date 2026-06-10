#include "ui/elements/ReusableElement.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <utility>

namespace ui {

ReusableElement::ReusableElement(std::string name)
    : Element(std::move(name)) {}

ReusableElement::ReusableElement(std::string name, std::unique_ptr<GeneratedWindowBase> child)
    : Element(std::move(name)), child_(std::move(child)) {}

void ReusableElement::setChild(std::unique_ptr<GeneratedWindowBase> child) {
    child_ = std::move(child);
}

RenderedContent ReusableElement::render(Size size, ElementRenderState state) const {
    if (child_ == nullptr) {
        return renderPlainText("", size.width, size.height, effectiveStyle(state.focused, state.editMode));
    }
    return renderGeneratedWindowContent(*child_, size);
}

ViewHost::ViewHost(std::string name)
    : ReusableElement(std::move(name)) {}

void ViewHost::setView(std::unique_ptr<GeneratedWindowBase> view) {
    setChild(std::move(view));
}

void ViewHost::clearView() {
    setChild(nullptr);
}

}  // namespace ui
