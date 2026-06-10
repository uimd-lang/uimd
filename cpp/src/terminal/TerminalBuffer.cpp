#include "ui/terminal/TerminalBuffer.hpp"
#include "ui/core/TextVisual.hpp"

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

namespace ui {

namespace {

constexpr int kAnsiBaseRow = 1;
constexpr int kAnsiBaseCol = 1;
constexpr int kAnsiDefaultForeground = 39;
constexpr int kAnsiDefaultBackground = 49;
constexpr char kAnsiSyncUpdateBegin[] = "\x1b[?2026h";
constexpr char kAnsiSyncUpdateEnd[] = "\x1b[?2026l";

[[nodiscard]] std::size_t cellCountFor(int width, int height) {
    if (width < 0 || height < 0) {
        throw std::invalid_argument("terminal buffer dimensions must not be negative");
    }
    return static_cast<std::size_t>(width * height);
}

[[nodiscard]] std::string sgrForColor(const std::optional<Color>& color, bool foreground) {
    if (!color.has_value() || color->kind() == Color::Kind::Unset || color->isTransparent()) {
        return std::to_string(foreground ? kAnsiDefaultForeground : kAnsiDefaultBackground);
    }
    if (color->rgba().has_value()) {
        const Rgba& rgba = *color->rgba();
        std::ostringstream out;
        out << (foreground ? "38" : "48") << ";2;"
            << static_cast<int>(rgba.red) << ';'
            << static_cast<int>(rgba.green) << ';'
            << static_cast<int>(rgba.blue);
        return out.str();
    }
    return std::to_string(foreground ? kAnsiDefaultForeground : kAnsiDefaultBackground);
}

[[nodiscard]] std::string sgrForCell(const TerminalCell& cell) {
    return "\x1b[" + sgrForColor(cell.foreground, true) + ";" +
           sgrForColor(cell.background, false) + "m";
}

[[nodiscard]] std::string terminalCellText(const std::string& text) {
    return safeTerminalCellText(text);
}

}  // namespace

TerminalBuffer::TerminalBuffer(int width, int height)
    : width_(width),
      height_(height),
      cells_(cellCountFor(width, height)),
      previous_(cells_) {
}

void TerminalBuffer::resize(int width, int height) {
    if (width < 0 || height < 0) {
        throw std::invalid_argument("terminal buffer dimensions must not be negative");
    }
    if (width == width_ && height == height_) {
        return;
    }
    width_ = width;
    height_ = height;
    cells_.assign(cellCountFor(width_, height_), TerminalCell{});
    previous_.assign(cells_.size(), TerminalCell{});
    forceFullRedraw_ = true;
}

void TerminalBuffer::clear(TerminalCell cell) {
    std::ranges::fill(cells_, std::move(cell));
}

void TerminalBuffer::setCell(int row, int col, TerminalCell cell) {
    if (!inBounds(row, col)) {
        return;
    }
    cells_[index(row, col)] = std::move(cell);
}

void TerminalBuffer::requestFullRedraw() {
    forceFullRedraw_ = true;
}

void TerminalBuffer::beginRenderStats() {
    renderStats_ = TerminalBufferRenderStats{};
}

const TerminalCell& TerminalBuffer::cell(int row, int col) const {
    if (!inBounds(row, col)) {
        throw std::out_of_range("terminal cell is out of bounds");
    }
    return cells_[index(row, col)];
}

TerminalBufferRenderStats TerminalBuffer::renderStats() const {
    return renderStats_;
}

std::string TerminalBuffer::renderDiff(int rowOffset, int colOffset) {
    return renderDiffRegion(rowOffset, colOffset, 0, 0, height_, width_);
}

std::string TerminalBuffer::renderDiffRegion(
    int rowOffset,
    int colOffset,
    int startRow,
    int startCol,
    int height,
    int width
) {
    std::string output;
    const bool fullRedraw = forceFullRedraw_;
    bool synchronizeUpdate = false;
    bool rawEmitted = false;
    renderStats_.fullRedraw = renderStats_.fullRedraw || fullRedraw;
    const int firstRow = std::max(0, startRow);
    const int firstCol = std::max(0, startCol);
    const int lastRow = std::min(height_, startRow + std::max(0, height));
    const int lastCol = std::min(width_, startCol + std::max(0, width));
    for (int row = firstRow; row < lastRow; ++row) {
        int col = firstCol;
        while (col < lastCol) {
            const std::size_t startIndex = index(row, col);
            if (cells_[startIndex].rawSkip) {
                if (fullRedraw || cells_[startIndex] != previous_[startIndex]) {
                    ++renderStats_.changedCells;
                }
                previous_[startIndex] = cells_[startIndex];
                ++col;
                continue;
            }
            if (!fullRedraw && cells_[startIndex] == previous_[startIndex]) {
                ++col;
                continue;
            }

            const TerminalCell styleCell = cells_[startIndex];
            if (!styleCell.raw.empty()) {
                synchronizeUpdate = true;
                const int rawWidth = std::max(1, styleCell.rawWidth);
                const int rawHeight = std::max(1, styleCell.rawHeight);
                const int clearWidth = std::min(rawWidth, width_ - col);
                const int clearHeight = std::min(rawHeight, height_ - row);
                for (int clearRow = row; clearRow < row + clearHeight; ++clearRow) {
                    output += "\x1b[" + std::to_string(clearRow + rowOffset + kAnsiBaseRow) + ";" +
                              std::to_string(col + colOffset + kAnsiBaseCol) + "H";
                    output += sgrForCell(styleCell);
                    output.append(static_cast<std::size_t>(clearWidth), ' ');
                }
                if (clearHeight >= rawHeight) {
                    output += "\x1b[" + std::to_string(row + rowOffset + kAnsiBaseRow) + ";" +
                              std::to_string(col + colOffset + kAnsiBaseCol) + "H";
                    output += styleCell.raw;
                    rawEmitted = true;
                }
                for (int coveredRow = row; coveredRow < row + clearHeight; ++coveredRow) {
                    for (int coveredCol = col; coveredCol < col + clearWidth; ++coveredCol) {
                        previous_[index(coveredRow, coveredCol)] = cells_[index(coveredRow, coveredCol)];
                        ++renderStats_.changedCells;
                    }
                }
                col += clearWidth;
                renderStats_.changedRuns += static_cast<std::size_t>(clearHeight + 1);
                continue;
            }
            std::string run;
            int runCol = col;
            while (col < lastCol) {
                const std::size_t currentIndex = index(row, col);
                const TerminalCell& current = cells_[currentIndex];
                if (!fullRedraw && current == previous_[currentIndex]) {
                    break;
                }
                if (current.rawSkip) {
                    break;
                }
                if (!current.raw.empty()) {
                    break;
                }
                if (current.foreground != styleCell.foreground ||
                    current.background != styleCell.background) {
                    break;
                }
                run += terminalCellText(current.text);
                previous_[currentIndex] = current;
                ++col;
                ++renderStats_.changedCells;
            }

            output += "\x1b[" + std::to_string(row + rowOffset + kAnsiBaseRow) + ";" +
                      std::to_string(runCol + colOffset + kAnsiBaseCol) + "H";
            output += sgrForCell(styleCell);
            output += run;
            ++renderStats_.changedRuns;
        }
    }
    if (!output.empty()) {
        if (rawEmitted) {
            for (int row = firstRow; row < lastRow; ++row) {
                int col = firstCol;
                while (col < lastCol) {
                    const TerminalCell& cell = cells_[index(row, col)];
                    if (cell.rawSkip || !cell.raw.empty()) {
                        ++col;
                        continue;
                    }
                    const TerminalCell styleCell = cell;
                    const int runCol = col;
                    std::string run;
                    while (col < lastCol) {
                        const TerminalCell& current = cells_[index(row, col)];
                        if (current.rawSkip || !current.raw.empty()) {
                            break;
                        }
                        if (current.foreground != styleCell.foreground ||
                            current.background != styleCell.background) {
                            break;
                        }
                        run += terminalCellText(current.text);
                        ++col;
                    }
                    output += "\x1b[" + std::to_string(row + rowOffset + kAnsiBaseRow) + ";" +
                              std::to_string(runCol + colOffset + kAnsiBaseCol) + "H";
                    output += sgrForCell(styleCell);
                    output += run;
                    ++renderStats_.changedRuns;
                }
            }
        }
        output += "\x1b[0m";
    }
    if (!output.empty() && synchronizeUpdate) {
        output = std::string{kAnsiSyncUpdateBegin} + output + kAnsiSyncUpdateEnd;
    }
    renderStats_.outputBytes += output.size();
    forceFullRedraw_ = false;
    return output;
}

std::string TerminalBuffer::renderScrollRegion(int rowOffset, int startRow, int height, int delta) {
    const int firstRow = std::max(0, startRow);
    const int lastRow = std::min(height_, startRow + std::max(0, height));
    const int regionHeight = lastRow - firstRow;
    const int distance = std::min(std::abs(delta), regionHeight);
    if (forceFullRedraw_ || regionHeight <= 1 || distance <= 0 || distance >= regionHeight) {
        return "";
    }

    const std::vector<TerminalCell> before = previous_;
    if (delta > 0) {
        for (int row = lastRow - 1; row >= firstRow + distance; --row) {
            for (int col = 0; col < width_; ++col) {
                previous_[index(row, col)] = before[index(row - distance, col)];
            }
        }
        for (int row = firstRow; row < firstRow + distance; ++row) {
            for (int col = 0; col < width_; ++col) {
                previous_[index(row, col)] = TerminalCell{};
            }
        }
    } else {
        for (int row = firstRow; row < lastRow - distance; ++row) {
            for (int col = 0; col < width_; ++col) {
                previous_[index(row, col)] = before[index(row + distance, col)];
            }
        }
        for (int row = lastRow - distance; row < lastRow; ++row) {
            for (int col = 0; col < width_; ++col) {
                previous_[index(row, col)] = TerminalCell{};
            }
        }
    }

    const char command = delta > 0 ? 'T' : 'S';
    std::string output =
        "\x1b[" + std::to_string(firstRow + rowOffset + kAnsiBaseRow) + ";" +
        std::to_string(lastRow + rowOffset) + "r" +
        "\x1b[" + std::to_string(firstRow + rowOffset + kAnsiBaseRow) + ";1H" +
        "\x1b[" + std::to_string(distance) + command +
        "\x1b[r";
    renderStats_.outputBytes += output.size();
    return output;
}

std::size_t TerminalBuffer::index(int row, int col) const {
    return static_cast<std::size_t>(row * width_ + col);
}

bool TerminalBuffer::inBounds(int row, int col) const {
    return row >= 0 && row < height_ && col >= 0 && col < width_;
}

}  // namespace ui
