#include "paragraph.hpp"

#include <utility>

namespace markdown_viewer_example {

void Paragraph::setText(std::string value) {
    text->setText(std::move(value));
}

}  // namespace markdown_viewer_example
