#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace ui
{

constexpr int kTextTabWidth = 4;

struct VisualGlyph
{
    std::string text = " ";
    int sourceStart = -1;
    int sourceEnd = -1;
};

[[nodiscard]] bool isUnsafeTerminalCodepoint(char32_t codepoint);
[[nodiscard]] std::string safeTerminalCellText(std::string_view text);
[[nodiscard]] std::vector<VisualGlyph> visualGlyphs(std::string_view text, int startColumn = 0, int sourceStart = 0);
[[nodiscard]] std::string visibleText(std::string_view text, int startColumn = 0);
[[nodiscard]] int visibleWidth(std::string_view text, int startColumn = 0);
[[nodiscard]] int tabSpacesForColumn(int column);

}  // namespace ui
