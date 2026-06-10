#pragma once

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

class Button : public Element {
public:
    Button(std::string name, std::string title = "Button");

    [[nodiscard]] const std::string& title() const {
        return title_;
    }

    void setTitle(std::string title);

    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    std::string title_;
};

}  // namespace ui
