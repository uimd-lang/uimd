#pragma once

#include "heading_ui.hpp"

#include <string>

namespace markdown_viewer_example {

class Heading : public HeadingUI {
public:
    void setHeading(int level, std::string value);
};

}  // namespace markdown_viewer_example
