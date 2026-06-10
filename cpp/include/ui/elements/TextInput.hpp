#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

class TextInput : public Element {
public:
    TextInput(std::string name, std::string value = {}, int maxLength = 0);

    [[nodiscard]] const std::string& value() const {
        return value_;
    }

    void setValue(std::string value);

    [[nodiscard]] int cursor() const {
        return cursor_;
    }

    [[nodiscard]] std::string selectedText() const;
    [[nodiscard]] int selectionStart() const;
    [[nodiscard]] int selectionEnd() const;

    void setCursor(int cursor);
    void selectRange(int start, int end);
    [[nodiscard]] int cursorForPoint(int row, int col, Size size) const;
    [[nodiscard]] bool scrollByRows(int delta, int viewportHeight, bool manual = true);

    void insertText(std::string_view text);
    [[nodiscard]] bool handleKey(std::string_view key);
    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

protected:
    TextInput(std::string name, std::string value, int maxLength, bool multiline);

private:
    [[nodiscard]] bool hasSelection() const;
    [[nodiscard]] int lineStartForCursor() const;
    [[nodiscard]] int lineEndForCursor() const;
    void moveCursorVertical(int delta);
    void deleteSelection();

    std::string value_;
    int cursor_ = 0;
    int maxLength_ = 0;
    std::optional<int> selectionAnchor_;
    int colScrollOffset_ = 0;
    int rowScrollOffset_ = 0;
    bool manualRowScroll_ = false;
    bool multiline_ = false;
};

class TextArea : public TextInput {
public:
    TextArea(std::string name, std::string value = {}, int maxLength = 0)
        : TextInput(std::move(name), std::move(value), maxLength, true) {
        setCommitMode(std::string(kCommitModeLeave));
    }
};

}  // namespace ui
