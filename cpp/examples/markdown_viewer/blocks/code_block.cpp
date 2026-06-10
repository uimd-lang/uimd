#include "code_block.hpp"

#include <cctype>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

namespace markdown_viewer_example {

namespace {

const std::unordered_set<std::string_view> kPythonKeywords = {
    "False", "None", "True", "and", "as", "assert", "async", "await", "break", "class",
    "continue", "def", "del", "elif", "else", "except", "finally", "for", "from",
    "global", "if", "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass",
    "raise", "return", "try", "while", "with", "yield",
};

[[nodiscard]] bool isIdentifierChar(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

}  // namespace

void CodeBlock::setCode(std::string language, std::string source) {
    lang->setText(language.empty() ? "text" : std::move(language));
    std::vector<ui::LabelSpan> spans;
    std::size_t cursor = 0;
    for (std::size_t index = 0; index < source.size();) {
        if (!isIdentifierChar(source[index])) {
            ++index;
            continue;
        }
        const std::size_t start = index;
        while (index < source.size() && isIdentifierChar(source[index])) {
            ++index;
        }
        const std::string_view word(source.data() + start, index - start);
        if (!kPythonKeywords.contains(word)) {
            continue;
        }
        if (start > cursor) {
            spans.push_back(ui::LabelSpan{.text = source.substr(cursor, start - cursor)});
        }
        spans.push_back(ui::LabelSpan{.text = std::string(word), .foreground = "#c084fc"});
        cursor = index;
    }
    if (cursor < source.size()) {
        spans.push_back(ui::LabelSpan{.text = source.substr(cursor)});
    }
    code->setSpans(std::move(spans));
}

}  // namespace markdown_viewer_example
