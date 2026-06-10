#pragma once

#include <optional>
#include <string>
#include <vector>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

struct LabelSpan {
    std::string text;
    std::string foreground;
    std::string background;
};

class Label : public Element {
public:
    Label(std::string name, std::string text = {});

    [[nodiscard]] const std::string& text() const {
        return text_;
    }

    void setText(std::string text);
    void setSpans(std::vector<LabelSpan> spans);

    void selectRange(int start, int end);
    void clearSelection();
    [[nodiscard]] bool hasSelection() const;
    [[nodiscard]] std::string selectedText() const;
    [[nodiscard]] int textPositionFromPoint(int localRow, int localCol, Size size) const;

    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    std::string text_;
    std::vector<LabelSpan> spans_;
    std::optional<int> selectionStart_;
    std::optional<int> selectionEnd_;
};

}  // namespace ui
