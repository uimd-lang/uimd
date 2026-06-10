#pragma once

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

#include <string>

namespace ui {

class MessageTable : public Element {
public:
    MessageTable(std::string name, std::string text = {});

    [[nodiscard]] const std::string& text() const {
        return text_;
    }

    void setText(std::string text);

    [[nodiscard]] int fitContentHeight() const;
    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    std::string text_;
};

}  // namespace ui
