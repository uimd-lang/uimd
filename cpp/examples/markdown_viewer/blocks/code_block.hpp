#pragma once

#include "code_block_ui.hpp"

#include <string>

namespace markdown_viewer_example {

class CodeBlock : public CodeBlockUI {
public:
    void setCode(std::string language, std::string source);
};

}  // namespace markdown_viewer_example
