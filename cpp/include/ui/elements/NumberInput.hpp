#pragma once

#include <string>
#include <string_view>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

class NumberInput : public Element {
public:
    NumberInput(std::string name, double value = 0.0, double step = 1.0);

    [[nodiscard]] double value() const {
        return value_;
    }

    void setValue(double value);
    void setEditCursor(int cursor);
    void setEditText(std::string text);
    void beginEdit();
    void cancelEdit();
    void commitEdit();

    [[nodiscard]] std::string displayValue() const;
    [[nodiscard]] bool handleKey(std::string_view key);
    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    void ensureEditText();

    double value_ = 0.0;
    double step_ = 1.0;
    std::string editText_;
    int editCursor_ = 0;
    double editOriginalValue_ = 0.0;
    bool editing_ = false;
    bool replaceOnFirstTextInput_ = false;
};

}  // namespace ui
