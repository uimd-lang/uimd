#include "document_view.hpp"

#include "../blocks/code_block.hpp"
#include "../blocks/heading.hpp"
#include "../blocks/paragraph.hpp"
#include "../blocks/table_block.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <utility>

namespace markdown_viewer_example {

namespace {

template <typename Block>
ui::RenderedContent renderBlock(Block& block, int width) {
    const ui::Size base = ui::generatedWindowContentSize(block);
    const int renderWidth = std::max(base.width, width);
    const ui::Size resolved = ui::generatedWindowResolvedContentSize(
        block,
        ui::Size{renderWidth, base.height});
    return ui::renderGeneratedWindowContent(
        block,
        ui::Size{renderWidth, std::max(base.height, resolved.height)});
}

}  // namespace

DocumentView::DocumentView() {
    setAutoScroll(false);
    setDynamicChildrenRenderer([this](int width) {
        return renderBlocks(width);
    });
}

void DocumentView::setBlocks(const std::vector<MarkdownBlock>& blocks) {
    blocks_ = blocks;
    invalidateDynamicChildren();
    setAutoScroll(false);
}

std::vector<ui::RenderedContent> DocumentView::renderBlocks(int width) const {
    const int blockWidth = std::max(1, width);
    std::vector<ui::RenderedContent> rendered;
    rendered.reserve(blocks_.size());
    for (const MarkdownBlock& block : blocks_) {
        if (block.kind == "heading") {
            Heading child;
            child.setHeading(block.level, block.text);
            rendered.push_back(renderBlock(child, blockWidth));
        } else if (block.kind == "code") {
            CodeBlock child;
            child.setCode(block.language, block.text);
            rendered.push_back(renderBlock(child, blockWidth));
        } else if (block.kind == "table") {
            TableBlock child;
            child.setTable(block.text);
            rendered.push_back(renderBlock(child, blockWidth));
        } else {
            Paragraph child;
            child.setText(block.text);
            rendered.push_back(renderBlock(child, blockWidth));
        }
    }
    return rendered;
}

}  // namespace markdown_viewer_example
