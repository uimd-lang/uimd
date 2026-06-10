#pragma once

#include "paragraph_ui.hpp"

#include <string>

namespace markdown_viewer_example {

class Paragraph : public ParagraphUI {
public:
    void setText(std::string value);
};

}  // namespace markdown_viewer_example
