#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

class ComboBox : public Element {
public:
    ComboBox(std::string name, std::vector<std::string> options = {});

    [[nodiscard]] int selectedIndex() const {
        return selectedIndex_;
    }

    [[nodiscard]] const std::vector<std::string>& options() const {
        return options_;
    }

    void setOptions(std::vector<std::string> options);
    void setSelectedIndex(int selectedIndex);

    [[nodiscard]] bool handleKey(std::string_view key);
    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    std::vector<std::string> options_;
    int selectedIndex_ = 0;
};

}  // namespace ui
