#include "table_block.hpp"

#include <utility>

namespace markdown_viewer_example {

void TableBlock::setTable(std::string value) {
    table->setText(std::move(value));
}

}  // namespace markdown_viewer_example
