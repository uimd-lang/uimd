#include "ui/core/TextVisual.hpp"

#include <algorithm>

namespace ui
{

namespace
{

struct DecodedGlyph
{
    std::string text;
    char32_t codepoint = U' ';
    std::size_t length = 1;
    bool valid = true;
};

[[nodiscard]] std::size_t utf8CodepointLength(unsigned char firstByte)
{
    if ((firstByte & 0x80U) == 0U)
    {
        return 1;
    }
    if ((firstByte & 0xE0U) == 0xC0U)
    {
        return 2;
    }
    if ((firstByte & 0xF0U) == 0xE0U)
    {
        return 3;
    }
    if ((firstByte & 0xF8U) == 0xF0U)
    {
        return 4;
    }
    return 1;
}

[[nodiscard]] bool continuationByte(unsigned char byte)
{
    return (byte & 0xC0U) == 0x80U;
}

[[nodiscard]] DecodedGlyph decodeGlyph(std::string_view text, std::size_t offset)
{
    if (offset >= text.size())
    {
        return DecodedGlyph{.text = " ", .codepoint = U' ', .length = 0, .valid = false};
    }

    const auto first = static_cast<unsigned char>(text[offset]);
    const std::size_t length = std::min(utf8CodepointLength(first), text.size() - offset);
    if (length == 1)
    {
        return DecodedGlyph{.text = std::string{text.substr(offset, 1)}, .codepoint = first, .length = 1};
    }

    if (length == 2 && continuationByte(static_cast<unsigned char>(text[offset + 1])))
    {
        const char32_t codepoint = ((first & 0x1FU) << 6U) |
            (static_cast<unsigned char>(text[offset + 1]) & 0x3FU);
        return DecodedGlyph{.text = std::string{text.substr(offset, length)}, .codepoint = codepoint, .length = length};
    }

    if (length == 3 &&
        continuationByte(static_cast<unsigned char>(text[offset + 1])) &&
        continuationByte(static_cast<unsigned char>(text[offset + 2])))
    {
        const char32_t codepoint = ((first & 0x0FU) << 12U) |
            ((static_cast<unsigned char>(text[offset + 1]) & 0x3FU) << 6U) |
            (static_cast<unsigned char>(text[offset + 2]) & 0x3FU);
        return DecodedGlyph{.text = std::string{text.substr(offset, length)}, .codepoint = codepoint, .length = length};
    }

    if (length == 4 &&
        continuationByte(static_cast<unsigned char>(text[offset + 1])) &&
        continuationByte(static_cast<unsigned char>(text[offset + 2])) &&
        continuationByte(static_cast<unsigned char>(text[offset + 3])))
    {
        const char32_t codepoint = ((first & 0x07U) << 18U) |
            ((static_cast<unsigned char>(text[offset + 1]) & 0x3FU) << 12U) |
            ((static_cast<unsigned char>(text[offset + 2]) & 0x3FU) << 6U) |
            (static_cast<unsigned char>(text[offset + 3]) & 0x3FU);
        return DecodedGlyph{.text = std::string{text.substr(offset, length)}, .codepoint = codepoint, .length = length};
    }

    return DecodedGlyph{.text = "?", .codepoint = U'?', .length = 1, .valid = false};
}

[[nodiscard]] bool inRange(char32_t codepoint, char32_t first, char32_t last)
{
    return codepoint >= first && codepoint <= last;
}

}  // namespace

int tabSpacesForColumn(int column)
{
    const int normalized = std::max(0, column);
    return kTextTabWidth - (normalized % kTextTabWidth);
}

bool isUnsafeTerminalCodepoint(char32_t codepoint)
{
    if (codepoint < 0x20 || codepoint == 0x7F)
    {
        return true;
    }
    if (inRange(codepoint, 0x0300, 0x036F) ||
        inRange(codepoint, 0x1AB0, 0x1AFF) ||
        inRange(codepoint, 0x1DC0, 0x1DFF) ||
        inRange(codepoint, 0x20D0, 0x20FF) ||
        inRange(codepoint, 0xFE20, 0xFE2F))
    {
        return true;
    }
    if (inRange(codepoint, 0x200B, 0x200F) ||
        inRange(codepoint, 0x202A, 0x202E) ||
        inRange(codepoint, 0x2060, 0x206F) ||
        inRange(codepoint, 0xFE00, 0xFE0F) ||
        inRange(codepoint, 0xE0100, 0xE01EF))
    {
        return true;
    }
    if (inRange(codepoint, 0x1100, 0x115F) ||
        inRange(codepoint, 0x2329, 0x232A) ||
        inRange(codepoint, 0x2E80, 0xA4CF) ||
        inRange(codepoint, 0xAC00, 0xD7A3) ||
        inRange(codepoint, 0xF900, 0xFAFF) ||
        inRange(codepoint, 0xFE10, 0xFE19) ||
        inRange(codepoint, 0xFE30, 0xFE6F) ||
        inRange(codepoint, 0xFF00, 0xFF60) ||
        inRange(codepoint, 0xFFE0, 0xFFE6) ||
        inRange(codepoint, 0x1F000, 0x1FAFF) ||
        inRange(codepoint, 0x20000, 0x3FFFD))
    {
        return true;
    }
    return false;
}

std::string safeTerminalCellText(std::string_view text)
{
    if (text.empty())
    {
        return " ";
    }
    const DecodedGlyph glyph = decodeGlyph(text, 0);
    if (!glyph.valid || glyph.codepoint == U'\t')
    {
        return glyph.codepoint == U'\t' ? " " : "?";
    }
    if (isUnsafeTerminalCodepoint(glyph.codepoint))
    {
        return "?";
    }
    return glyph.text.empty() ? std::string{" "} : glyph.text;
}

std::vector<VisualGlyph> visualGlyphs(std::string_view text, int startColumn, int sourceStart)
{
    std::vector<VisualGlyph> glyphs;
    int column = std::max(0, startColumn);
    std::size_t offset = 0;
    while (offset < text.size())
    {
        const DecodedGlyph glyph = decodeGlyph(text, offset);
        const int rawStart = sourceStart + static_cast<int>(offset);
        const int rawEnd = rawStart + static_cast<int>(std::max<std::size_t>(glyph.length, 1));
        if (glyph.valid && glyph.codepoint == U'\r')
        {
            offset += std::max<std::size_t>(glyph.length, 1);
            continue;
        }
        if (glyph.valid && glyph.codepoint == U'\t')
        {
            const int spaces = tabSpacesForColumn(column);
            for (int index = 0; index < spaces; ++index)
            {
                glyphs.push_back(VisualGlyph{.text = " ", .sourceStart = rawStart, .sourceEnd = rawEnd});
                ++column;
            }
        }
        else
        {
            const std::string visible = (!glyph.valid || isUnsafeTerminalCodepoint(glyph.codepoint)) ? "?" : glyph.text;
            glyphs.push_back(VisualGlyph{.text = visible.empty() ? std::string{" "} : visible,
                                         .sourceStart = rawStart,
                                         .sourceEnd = rawEnd});
            ++column;
        }
        offset += std::max<std::size_t>(glyph.length, 1);
    }
    return glyphs;
}

std::string visibleText(std::string_view text, int startColumn)
{
    std::string result;
    for (const VisualGlyph& glyph : visualGlyphs(text, startColumn))
    {
        result += glyph.text;
    }
    return result;
}

int visibleWidth(std::string_view text, int startColumn)
{
    return static_cast<int>(visualGlyphs(text, startColumn).size());
}

}  // namespace ui
