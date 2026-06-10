#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "ui/core/Color.hpp"

namespace ui {

struct TerminalCell {
    std::string text = " ";
    std::string raw;
    int rawWidth = 1;
    int rawHeight = 1;
    bool rawSkip = false;

    std::optional<Color> foreground;
    std::optional<Color> background;

    friend bool operator==(const TerminalCell&, const TerminalCell&) = default;
};

struct TerminalBufferRenderStats {
    std::size_t changedCells = 0;
    std::size_t changedRuns = 0;
    std::size_t outputBytes = 0;
    bool fullRedraw = false;
};

class TerminalBuffer {
public:
    TerminalBuffer(int width, int height);

    [[nodiscard]] int width() const {
        return width_;
    }

    [[nodiscard]] int height() const {
        return height_;
    }

    void resize(int width, int height);
    void clear(TerminalCell cell = TerminalCell{});
    void setCell(int row, int col, TerminalCell cell);
    void requestFullRedraw();
    void beginRenderStats();

    [[nodiscard]] const TerminalCell& cell(int row, int col) const;
    [[nodiscard]] TerminalBufferRenderStats renderStats() const;

    [[nodiscard]] std::string renderDiff(int rowOffset = 0, int colOffset = 0);
    [[nodiscard]] std::string renderDiffRegion(
        int rowOffset,
        int colOffset,
        int startRow,
        int startCol,
        int height,
        int width
    );
    [[nodiscard]] std::string renderScrollRegion(
        int rowOffset,
        int startRow,
        int height,
        int delta
    );

private:
    [[nodiscard]] std::size_t index(int row, int col) const;
    [[nodiscard]] bool inBounds(int row, int col) const;

    int width_;
    int height_;
    std::vector<TerminalCell> cells_;
    std::vector<TerminalCell> previous_;
    bool forceFullRedraw_ = false;
    TerminalBufferRenderStats renderStats_{};
};

}  // namespace ui
