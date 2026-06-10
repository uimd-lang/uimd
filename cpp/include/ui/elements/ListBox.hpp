#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

class ListBox : public Element {
public:
    ListBox(std::string name, std::vector<std::string> options = {});

    [[nodiscard]] int selectedIndex() const {
        return selectedIndex_;
    }

    [[nodiscard]] const std::vector<int>& selectedIndices() const {
        return selectedIndices_;
    }

    [[nodiscard]] std::vector<std::string> selectedValues() const;

    [[nodiscard]] bool multiple() const {
        return multiple_;
    }

    [[nodiscard]] int scrollOffset() const {
        return scrollOffset_;
    }

    [[nodiscard]] const std::vector<std::string>& options() const {
        return options_;
    }

    [[nodiscard]] const std::vector<std::string>& disabledValues() const {
        return disabledValues_;
    }

    void setOptions(std::vector<std::string> options);
    void setDisabledValues(std::vector<std::string> disabledValues);
    void setMultiple(bool multiple);
    void setSelectedIndex(int selectedIndex);
    void setSelectedIndices(std::vector<int> selectedIndices);
    void setSelectedValues(const std::vector<std::string>& selectedValues);
    void scrollBy(int delta, int viewportHeight);

    [[nodiscard]] bool handleKey(std::string_view key);
    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    void ensureSelectedVisible(int height);

    std::vector<std::string> options_;
    std::vector<std::string> disabledValues_;
    int selectedIndex_ = 0;
    std::vector<int> selectedIndices_;
    bool multiple_ = false;
    int scrollOffset_ = 0;
    mutable int lastViewportHeight_ = 0;
};

}  // namespace ui
