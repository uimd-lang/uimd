#pragma once

#include "document_view_ui.hpp"

#include <string>
#include <vector>

namespace markdown_viewer_example {

struct MarkdownBlock {
    std::string kind;
    int level = 0;
    std::string text;
    std::string language;
};

class DocumentView : public DocumentViewUI {
public:
    DocumentView();

    void setBlocks(const std::vector<MarkdownBlock>& blocks);

private:
    [[nodiscard]] std::vector<ui::RenderedContent> renderBlocks(int width) const;

    std::vector<MarkdownBlock> blocks_;
};

}  // namespace markdown_viewer_example
