#pragma once

#include "ui/elements/Label.hpp"

namespace ui {

class InfoLabel : public Label {
public:
    using Label::Label;

    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;
};

}  // namespace ui
