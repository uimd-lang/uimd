#pragma once

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"
#include "ui/generated/GeneratedWindowBase.hpp"

#include <memory>
#include <string>

namespace ui {

class ReusableElement : public Element {
public:
    explicit ReusableElement(std::string name);
    ReusableElement(std::string name, std::unique_ptr<GeneratedWindowBase> child);

    void setChild(std::unique_ptr<GeneratedWindowBase> child);

    [[nodiscard]] GeneratedWindowBase* child() const {
        return child_.get();
    }

    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    std::unique_ptr<GeneratedWindowBase> child_;
};

class ViewHost : public ReusableElement {
public:
    explicit ViewHost(std::string name);

    void setView(std::unique_ptr<GeneratedWindowBase> view);
    void clearView();

    [[nodiscard]] GeneratedWindowBase* currentView() const {
        return child();
    }
};

}  // namespace ui
