#pragma once

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

class CheckBox : public Element {
public:
    CheckBox(std::string name, std::string title = {}, bool checked = false);

    [[nodiscard]] bool checked() const {
        return checked_;
    }

    void setChecked(bool checked) {
        checked_ = checked;
    }

    [[nodiscard]] bool handleKey(std::string_view key);
    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    std::string title_;
    bool checked_ = false;
};

}  // namespace ui
