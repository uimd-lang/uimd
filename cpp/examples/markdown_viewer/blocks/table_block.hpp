#pragma once

#include "table_block_ui.hpp"

#include <string>

namespace markdown_viewer_example {

class TableBlock : public TableBlockUI {
public:
    void setTable(std::string value);
};

}  // namespace markdown_viewer_example
