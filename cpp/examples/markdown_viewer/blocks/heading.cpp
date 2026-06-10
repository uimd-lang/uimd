#include "heading.hpp"

#include <algorithm>
#include <utility>

namespace markdown_viewer_example {

void Heading::setHeading(int level, std::string value) {
    const int clamped = std::max(1, std::min(3, level));
    const std::string color = clamped == 1 ? "#facc15" : (clamped == 2 ? "#93c5fd" : "#a7f3d0");
    heading_text->setSpans({
        ui::LabelSpan{.text = std::string(static_cast<std::size_t>(clamped), '#'), .foreground = "#64748b"},
        ui::LabelSpan{.text = " "},
        ui::LabelSpan{.text = std::move(value), .foreground = color},
    });
}

}  // namespace markdown_viewer_example
